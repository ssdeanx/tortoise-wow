#ifndef MOD_LLM_CHAT_CONFIG_H
#define MOD_LLM_CHAT_CONFIG_H

#include "Common.h"
#include <string>

class ModLlmChatConfig
{
public:
    static ModLlmChatConfig& Instance();

    void Load();

    bool IsEnabled() const { return m_enabled; }
    std::string const& GetApiEndpoint() const { return m_apiEndpoint; }
    std::string const& GetApiKey() const { return m_apiKey; }
    std::string const& GetModel() const { return m_model; }
    uint32 GetMaxTokens() const { return m_maxTokens; }
    float GetTemperature() const { return m_temperature; }

    uint32 GetWhisperRate() const { return m_whisperRate; }
    uint32 GetPartyRate() const { return m_partyRate; }
    uint32 GetGuildRate() const { return m_guildRate; }
    uint32 GetSayRate() const { return m_sayRate; }
    uint32 GetRaidRate() const { return m_raidRate; }

    bool RequireHumanPresent() const { return m_requireHumanPresent; }
    float GetHumanPresenceRadius() const { return m_humanPresenceRadius; }

    bool IsBotToBotEnabled() const { return m_botToBotEnabled; }
    uint32 GetMaxConversationDepth() const { return m_maxConversationDepth; }
    uint32 GetConversationCooldown() const { return m_conversationCooldown; }

    bool EmoteOnWhisper() const { return m_emoteOnWhisper; }
    uint32 GetWhisperCooldown() const { return m_whisperCooldown; }
    bool IsDebug() const { return m_debug; }

private:
    ModLlmChatConfig();

    bool m_enabled;
    std::string m_apiEndpoint;
    std::string m_apiKey;
    std::string m_model;
    uint32 m_maxTokens;
    float m_temperature;

    uint32 m_whisperRate;
    uint32 m_partyRate;
    uint32 m_guildRate;
    uint32 m_sayRate;
    uint32 m_raidRate;

    bool m_requireHumanPresent;
    float m_humanPresenceRadius;

    bool m_botToBotEnabled;
    uint32 m_maxConversationDepth;
    uint32 m_conversationCooldown;

    bool m_emoteOnWhisper;
    uint32 m_whisperCooldown;
    bool m_debug;
};

#define sLlmConfig ModLlmChatConfig::Instance()

#endif // MOD_LLM_CHAT_CONFIG_H
