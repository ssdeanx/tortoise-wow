#ifndef LLM_ASYNC_WORKER_H
#define LLM_ASYNC_WORKER_H

#include "Common.h"
#include "ObjectGuid.h"
#include "LlmPromptBuilder.h"
#include <string>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

enum ChatChannelType : uint8
{
    LLM_CHANNEL_WHISPER,
    LLM_CHANNEL_PARTY,
    LLM_CHANNEL_GUILD,
    LLM_CHANNEL_SAY,
    LLM_CHANNEL_RAID
};

struct ChatRequest
{
    ObjectGuid botGuid;
    ObjectGuid senderGuid;
    ChatChannelType channel;
    std::string channelKey;
    BotChatContext context;
    uint32 language;
};

struct ChatResponse
{
    ObjectGuid botGuid;
    ObjectGuid senderGuid;
    ChatChannelType channel;
    std::string channelKey;
    std::string replyText;
    uint32 language;
};

class LlmAsyncWorker
{
public:
    static LlmAsyncWorker& Instance();

    void Start();
    void Stop();

    void EnqueueRequest(ChatRequest const& req);
    std::vector<ChatResponse> DrainCompletedResponses();

private:
    LlmAsyncWorker();
    ~LlmAsyncWorker();

    void WorkerLoop();
    bool ExecuteHttpRequest(ChatRequest const& req, std::string& outReply);

    std::atomic<bool> m_running;
    std::thread m_workerThread;

    std::mutex m_requestMutex;
    std::condition_variable m_requestCv;
    std::deque<ChatRequest> m_requestQueue;

    std::mutex m_responseMutex;
    std::vector<ChatResponse> m_completedResponses;
};

#define sLlmWorker LlmAsyncWorker::Instance()

#endif // LLM_ASYNC_WORKER_H
