#include "LlmAsyncWorker.h"
#include "ModLlmChatConfig.h"
#include "Log.h"
#include "json.hpp"
#include <curl/curl.h>

namespace
{
    size_t CurlWriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        size_t total = size * nmemb;
        std::string* s = static_cast<std::string*>(userp);
        s->append(static_cast<char const*>(contents), total);
        return total;
    }
}

LlmAsyncWorker& LlmAsyncWorker::Instance()
{
    static LlmAsyncWorker instance;
    return instance;
}

LlmAsyncWorker::LlmAsyncWorker()
    : m_running(false)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

LlmAsyncWorker::~LlmAsyncWorker()
{
    Stop();
    curl_global_cleanup();
}

void LlmAsyncWorker::Start()
{
    if (m_running.exchange(true))
        return;

    m_workerThread = std::thread(&LlmAsyncWorker::WorkerLoop, this);
    sLog.outString("[mod-llm-chat] Async worker thread started.");
}

void LlmAsyncWorker::Stop()
{
    if (!m_running.exchange(false))
        return;

    m_requestCv.notify_all();
    if (m_workerThread.joinable())
        m_workerThread.join();

    sLog.outString("[mod-llm-chat] Async worker thread stopped.");
}

void LlmAsyncWorker::EnqueueRequest(ChatRequest const& req)
{
    {
        std::lock_guard<std::mutex> lock(m_requestMutex);
        // Cap queue to 50 pending requests to prevent unbounded memory growth under spam
        if (m_requestQueue.size() >= 50)
            m_requestQueue.pop_front();

        m_requestQueue.push_back(req);
    }
    m_requestCv.notify_one();
}

std::vector<ChatResponse> LlmAsyncWorker::DrainCompletedResponses()
{
    std::vector<ChatResponse> drained;
    {
        std::lock_guard<std::mutex> lock(m_responseMutex);
        drained.swap(m_completedResponses);
    }
    return drained;
}

void LlmAsyncWorker::WorkerLoop()
{
    while (m_running.load())
    {
        ChatRequest req;
        {
            std::unique_lock<std::mutex> lock(m_requestMutex);
            m_requestCv.wait(lock, [this]() {
                return !m_running.load() || !m_requestQueue.empty();
            });

            if (!m_running.load() && m_requestQueue.empty())
                break;

            if (m_requestQueue.empty())
                continue;

            req = m_requestQueue.front();
            m_requestQueue.pop_front();
        }

        std::string rawReply;
        if (ExecuteHttpRequest(req, rawReply))
        {
            std::string sanitized = LlmPromptBuilder::SanitizeReply(rawReply);
            if (!sanitized.empty())
            {
                ChatResponse resp;
                resp.botGuid = req.botGuid;
                resp.senderGuid = req.senderGuid;
                resp.channel = req.channel;
                resp.channelKey = req.channelKey;
                resp.replyText = sanitized;
                resp.language = req.language;

                std::lock_guard<std::mutex> lock(m_responseMutex);
                m_completedResponses.push_back(resp);
            }
        }
    }
}

bool LlmAsyncWorker::ExecuteHttpRequest(ChatRequest const& req, std::string& outReply)
{
    CURL* curl = curl_easy_init();
    if (!curl)
        return false;

    std::string base = sLlmConfig.GetApiEndpoint();
    while (!base.empty() && base.back() == '/')
        base.pop_back();

    std::string model = sLlmConfig.GetModel();
    std::string apiKey = sLlmConfig.GetApiKey();
    std::string systemPrompt = LlmPromptBuilder::BuildSystemPrompt(req.context);
    std::string userPrompt = LlmPromptBuilder::BuildUserPrompt(req.context);

    std::string targetUrl;
    struct curl_slist* headers = nullptr;
    nlohmann::json payload;

    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "User-Agent: tortoise-wow-agent/1.0");
    headers = curl_slist_append(headers, "x-opencode-session: tortoise-wow-session");

    if (model == "union-alpha" || model.find("minimax") != std::string::npos || model.find("qwen") != std::string::npos)
    {
        // Anthropic Messages Protocol
        targetUrl = (base.rfind("/messages") != std::string::npos) ? base : (base + "/messages");

        std::string xApiKey = "x-api-key: " + apiKey;
        std::string authBearer = "Authorization: Bearer " + apiKey;
        headers = curl_slist_append(headers, xApiKey.c_str());
        headers = curl_slist_append(headers, authBearer.c_str());
        headers = curl_slist_append(headers, "anthropic-version: 2023-06-01");

        payload["model"] = model;
        payload["max_tokens"] = sLlmConfig.GetMaxTokens();
        payload["system"] = systemPrompt;
        payload["messages"] = nlohmann::json::array({
            {{"role", "user"}, {"content", userPrompt}}
        });
    }
    else if (model.rfind("muse-spark", 0) == 0 || model.rfind("grok", 0) == 0 || model.rfind("gpt-5.6", 0) == 0)
    {
        // OpenAI Responses API Protocol
        targetUrl = (base.rfind("/responses") != std::string::npos) ? base : (base + "/responses");

        std::string authBearer = "Authorization: Bearer " + apiKey;
        headers = curl_slist_append(headers, authBearer.c_str());

        payload["model"] = model;
        payload["instructions"] = systemPrompt;
        payload["input"] = userPrompt;
    }
    else
    {
        // OpenAI Chat Completions Protocol (Default: glm-5.3-flash, Ollama, etc.)
        targetUrl = (base.rfind("/chat/completions") != std::string::npos) ? base : (base + "/chat/completions");

        if (!apiKey.empty())
        {
            std::string authBearer = "Authorization: Bearer " + apiKey;
            headers = curl_slist_append(headers, authBearer.c_str());
        }

        payload["model"] = model;
        payload["max_tokens"] = sLlmConfig.GetMaxTokens();
        payload["temperature"] = sLlmConfig.GetTemperature();
        payload["messages"] = nlohmann::json::array({
            {{"role", "system"}, {"content", systemPrompt}},
            {{"role", "user"}, {"content", userPrompt}}
        });
    }

    std::string postData = payload.dump();
    std::string responseString;

    curl_easy_setopt(curl, CURLOPT_URL, targetUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 25L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 6L);

    CURLcode res = curl_easy_perform(curl);
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK || httpCode < 200 || httpCode >= 300)
    {
        if (sLlmConfig.IsDebug())
            sLog.outError("[mod-llm-chat] HTTP request to %s failed (code %ld, curl error %d): %s",
                targetUrl.c_str(), httpCode, res, responseString.c_str());
        return false;
    }

    try
    {
        nlohmann::json parsed = nlohmann::json::parse(responseString);

        // 1. Anthropic format
        if (parsed.find("content") != parsed.end() && parsed["content"].is_array() && !parsed["content"].empty())
        {
            outReply = parsed["content"][0].value("text", "");
            return !outReply.empty();
        }

        // 2. OpenAI Responses format
        if (parsed.find("output") != parsed.end() && parsed["output"].is_array())
        {
            for (auto const& item : parsed["output"])
            {
                if (item.value("type", "") == "message" && item.find("content") != item.end() && item["content"].is_array())
                {
                    outReply = item["content"][0].value("text", "");
                    return !outReply.empty();
                }
            }
        }

        // 3. OpenAI Chat Completions format
        if (parsed.find("choices") != parsed.end() && parsed["choices"].is_array() && !parsed["choices"].empty())
        {
            auto const& choice = parsed["choices"][0];
            if (choice.find("message") != choice.end() && choice["message"].find("content") != choice["message"].end())
            {
                outReply = choice["message"].value("content", "");
                return !outReply.empty();
            }
        }
    }
    catch (std::exception const& e)
    {
        if (sLlmConfig.IsDebug())
            sLog.outError("[mod-llm-chat] Failed to parse JSON response: %s", e.what());
    }

    return false;
}
