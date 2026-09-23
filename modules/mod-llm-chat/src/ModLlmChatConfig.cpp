#include "ModLlmChatConfig.h"
#include "Config/Config.h"
#include "Log.h"
#include "json.hpp"
#include <fstream>
#include <cstdlib>

ModLlmChatConfig& ModLlmChatConfig::Instance()
{
    static ModLlmChatConfig instance;
    return instance;
}

ModLlmChatConfig::ModLlmChatConfig()
    : m_enabled(false)
    , m_apiEndpoint("https://opencode.ai/zen/go/v1")
    , m_apiKey("")
    , m_model("union-alpha")
    , m_maxTokens(400)
    , m_temperature(0.7f)
    , m_whisperRate(100)
    , m_partyRate(20)
    , m_guildRate(15)
    , m_sayRate(15)
    , m_raidRate(5)
    , m_requireHumanPresent(true)
    , m_humanPresenceRadius(30.0f)
    , m_botToBotEnabled(true)
    , m_maxConversationDepth(2)
    , m_conversationCooldown(45)
    , m_emoteOnWhisper(true)
    , m_whisperCooldown(10)
    , m_debug(false)
{
}

void ModLlmChatConfig::Load()
{
    m_enabled = sConfig.GetBoolDefault("ModLlmChat.Enable", true);
    m_apiEndpoint = sConfig.GetStringDefault("ModLlmChat.ApiEndpoint", "https://opencode.ai/zen/go/v1");
    m_apiKey = sConfig.GetStringDefault("ModLlmChat.ApiKey", "");
    m_model = sConfig.GetStringDefault("ModLlmChat.Model", "union-alpha");
    m_maxTokens = sConfig.GetIntDefault("ModLlmChat.MaxTokens", 400);
    m_temperature = sConfig.GetFloatDefault("ModLlmChat.Temperature", 0.7f);

    m_whisperRate = sConfig.GetIntDefault("ModLlmChat.WhisperRate", 100);
    m_partyRate = sConfig.GetIntDefault("ModLlmChat.PartyRate", 20);
    m_guildRate = sConfig.GetIntDefault("ModLlmChat.GuildRate", 15);
    m_sayRate = sConfig.GetIntDefault("ModLlmChat.SayRate", 15);
    m_raidRate = sConfig.GetIntDefault("ModLlmChat.RaidRate", 5);

    m_requireHumanPresent = sConfig.GetBoolDefault("ModLlmChat.RequireHumanPresent", true);
    m_humanPresenceRadius = sConfig.GetFloatDefault("ModLlmChat.HumanPresenceRadius", 30.0f);

    m_botToBotEnabled = sConfig.GetBoolDefault("ModLlmChat.BotToBotEnabled", true);
    m_maxConversationDepth = sConfig.GetIntDefault("ModLlmChat.MaxConversationDepth", 2);
    m_conversationCooldown = sConfig.GetIntDefault("ModLlmChat.ConversationCooldown", 45);

    m_emoteOnWhisper = sConfig.GetBoolDefault("ModLlmChat.EmoteOnWhisper", true);
    m_whisperCooldown = sConfig.GetIntDefault("ModLlmChat.WhisperCooldown", 10);
    m_debug = sConfig.GetBoolDefault("ModLlmChat.Debug", false);

    // Fallback: If API key is empty in config, attempt to read from OpenCode auth.json
    if (m_apiKey.empty())
    {
        char const* home = std::getenv("HOME");
        if (home)
        {
            std::string authPath = std::string(home) + "/.local/share/opencode/auth.json";
            std::ifstream authFile(authPath);
            if (authFile.is_open())
            {
                try
                {
                    nlohmann::json j;
                    authFile >> j;
                    if (j.find("opencode-go") != j.end() && j["opencode-go"].find("key") != j["opencode-go"].end())
                    {
                        m_apiKey = j["opencode-go"]["key"].get<std::string>();
                        sLog.outString("[mod-llm-chat] Loaded OpenCode API key from %s", authPath.c_str());
                    }
                }
                catch (std::exception const& e)
                {
                    sLog.outError("[mod-llm-chat] Failed to parse %s: %s", authPath.c_str(), e.what());
                }
            }
        }
    }

    sLog.outString("[mod-llm-chat] Config Loaded: Enabled=%d, Model=%s, MaxTokens=%u, Endpoint=%s",
        m_enabled ? 1 : 0, m_model.c_str(), m_maxTokens, m_apiEndpoint.c_str());
}
