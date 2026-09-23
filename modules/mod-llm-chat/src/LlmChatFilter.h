#ifndef LLM_CHAT_FILTER_H
#define LLM_CHAT_FILTER_H

#include "Common.h"
#include "ObjectGuid.h"
#include <string>
#include <map>
#include <ctime>

class Player;
class Group;

class LlmChatFilter
{
public:
    static LlmChatFilter& Instance();

    // Check if player is a driven bot
    static bool IsBot(Player const* player);

    // Dialogue LOD: Human observer presence checks
    static bool IsHumanPresentForSay(Player* bot, float radius = 30.0f);
    static bool IsHumanInGroup(Group const* group);
    static bool IsHumanOnlineInGuild(uint32 guildId);

    // Rate roll (0 - 100%)
    static bool RollChance(uint32 ratePercentage);

    // Cooldown and conversation chain limiter
    bool CanInitiateOrReply(std::string const& channelKey, bool isSenderBot);
    void RecordMessageSent(std::string const& channelKey, bool isBot);

    // Whisper cooldown tracking per player
    bool CheckAndSetWhisperCooldown(ObjectGuid botGuid, uint32 cooldownSec);

private:
    LlmChatFilter();

    struct ConversationState
    {
        uint32 depth = 0;
        time_t lastMessageTime = 0;
        time_t cooldownUntil = 0;
    };

    std::map<std::string, ConversationState> m_channelStates;
    std::map<ObjectGuid, time_t> m_whisperCooldowns;
};

#define sLlmFilter LlmChatFilter::Instance()

#endif // LLM_CHAT_FILTER_H
