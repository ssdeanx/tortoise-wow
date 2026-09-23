#include "LlmChatFilter.h"
#include "Objects/Player.h"
#include "Group/Group.h"
#include "Guild/Guild.h"
#include "Guild/GuildMgr.h"
#include "Maps/Map.h"
#include "World.h"
#include "ModuleSlots.h"
#include "ModLlmChatConfig.h"
#include "Util.h"

LlmChatFilter& LlmChatFilter::Instance()
{
    static LlmChatFilter instance;
    return instance;
}

LlmChatFilter::LlmChatFilter()
{
}

bool LlmChatFilter::IsBot(Player const* player)
{
    if (!player)
        return false;
    return player->GetModuleSlot(MODULE_SLOT_BOT_AI) != nullptr;
}

bool LlmChatFilter::IsHumanPresentForSay(Player* bot, float radius)
{
    if (!bot || !bot->GetMap())
        return false;

    // Scan players on the same map
    for (auto const& ref : bot->GetMap()->GetPlayers())
    {
        Player* p = ref.getSource();
        if (p && !IsBot(p) && bot->IsWithinDist(p, radius))
            return true;
    }
    return false;
}

bool LlmChatFilter::IsHumanInGroup(Group const* group)
{
    if (!group)
        return false;

    for (auto const& slot : group->GetMemberSlots())
    {
        Player* member = sObjectMgr.GetPlayer(slot.guid);
        if (member && !IsBot(member))
            return true;
    }
    return false;
}

bool LlmChatFilter::IsHumanOnlineInGuild(uint32 guildId)
{
    if (guildId == 0)
        return false;

    Guild* guild = sGuildMgr.GetGuildById(guildId);
    if (!guild)
        return false;

    // Check all active sessions
    HashMapHolder<Player>::MapType const& players = sObjectAccessor.GetPlayers();
    for (auto const& pair : players)
    {
        Player* p = pair.second;
        if (p && p->GetGuildId() == guildId && !IsBot(p))
            return true;
    }
    return false;
}

bool LlmChatFilter::RollChance(uint32 ratePercentage)
{
    if (ratePercentage >= 100)
        return true;
    if (ratePercentage == 0)
        return false;

    return urand(0, 99) < ratePercentage;
}

bool LlmChatFilter::CanInitiateOrReply(std::string const& channelKey, bool isSenderBot)
{
    time_t now = std::time(nullptr);
    auto it = m_channelStates.find(channelKey);
    if (it == m_channelStates.end())
        return true;

    ConversationState const& state = it->second;

    // Check active channel cooldown
    if (now < state.cooldownUntil)
        return false;

    // If sender is a bot and bot-to-bot chat is disabled
    if (isSenderBot && !sLlmConfig.IsBotToBotEnabled())
        return false;

    // If sender is a bot, check if depth limit is reached
    if (isSenderBot && state.depth >= sLlmConfig.GetMaxConversationDepth())
        return false;

    return true;
}

void LlmChatFilter::RecordMessageSent(std::string const& channelKey, bool isBot)
{
    time_t now = std::time(nullptr);
    ConversationState& state = m_channelStates[channelKey];

    if (!isBot)
    {
        // Real human spoke: reset depth to 1
        state.depth = 1;
        state.lastMessageTime = now;
        state.cooldownUntil = 0;
    }
    else
    {
        // Bot spoke: increment depth
        state.depth++;
        state.lastMessageTime = now;

        // If max depth reached, start conversation cooldown
        if (state.depth >= sLlmConfig.GetMaxConversationDepth())
        {
            state.cooldownUntil = now + sLlmConfig.GetConversationCooldown();
            state.depth = 0; // Reset for next cycle
        }
    }
}

bool LlmChatFilter::CheckAndSetWhisperCooldown(ObjectGuid botGuid, uint32 cooldownSec)
{
    time_t now = std::time(nullptr);
    auto it = m_whisperCooldowns.find(botGuid);
    if (it != m_whisperCooldowns.end())
    {
        if (now < it->second)
            return false; // Still on cooldown
    }

    m_whisperCooldowns[botGuid] = now + cooldownSec;
    return true;
}
