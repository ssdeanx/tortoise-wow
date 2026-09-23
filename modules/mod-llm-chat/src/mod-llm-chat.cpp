#include "ScriptObjects.h"
#include "Log.h"
#include "Objects/Player.h"
#include "Group/Group.h"
#include "Guild/Guild.h"
#include "Guild/GuildMgr.h"
#include "Chat/Chat.h"
#include "ModLlmChatConfig.h"
#include "LlmPromptBuilder.h"
#include "LlmChatFilter.h"
#include "LlmAsyncWorker.h"
#include "SharedDefines.h"
#include "Util.h"

namespace
{
    class ModLlmChatWorldScript : public WorldScript
    {
    public:
        ModLlmChatWorldScript()
            : WorldScript("mod-llm-chat_world", { WORLDHOOK_ON_STARTUP, WORLDHOOK_ON_UPDATE, WORLDHOOK_ON_SHUTDOWN })
        {
        }

        void OnStartup() override
        {
            sLlmConfig.Load();
            if (sLlmConfig.IsEnabled())
            {
                sLlmWorker.Start();
                sLog.outString("[mod-llm-chat] Generative Dialogue Engine initialized.");
            }
        }

        void OnUpdate(uint32 /*diff*/) override
        {
            if (!sLlmConfig.IsEnabled())
                return;

            std::vector<ChatResponse> completed = sLlmWorker.DrainCompletedResponses();
            for (auto const& resp : completed)
            {
                Player* bot = sObjectMgr.GetPlayer(resp.botGuid);
                if (!bot || !bot->IsInWorld() || !bot->IsAlive())
                    continue;

                switch (resp.channel)
                {
                    case LLM_CHANNEL_WHISPER:
                    {
                        bot->Whisper(resp.replyText, LANG_UNIVERSAL, resp.senderGuid);
                        break;
                    }
                    case LLM_CHANNEL_PARTY:
                    case LLM_CHANNEL_RAID:
                    {
                        Group* group = bot->GetGroup();
                        if (group)
                        {
                            WorldPacket data;
                            ChatMsg msgType = (resp.channel == LLM_CHANNEL_PARTY) ? CHAT_MSG_PARTY : CHAT_MSG_RAID;
                            ChatHandler::BuildChatPacket(data, msgType, resp.replyText, Language(resp.language),
                                bot->GetChatTag(), bot->GetObjectGuid(), bot->GetName());
                            group->BroadcastPacket(&data, false, -1, bot->GetObjectGuid());
                        }
                        break;
                    }
                    case LLM_CHANNEL_GUILD:
                    {
                        uint32 guildId = bot->GetGuildId();
                        if (guildId)
                        {
                            Guild* guild = sGuildMgr.GetGuildById(guildId);
                            if (guild && bot->GetSession())
                                guild->BroadcastToGuild(bot->GetSession(), resp.replyText, resp.language);
                        }
                        break;
                    }
                    case LLM_CHANNEL_SAY:
                    {
                        bot->Say(resp.replyText, Language(resp.language));
                        break;
                    }
                }

                sLlmFilter.RecordMessageSent(resp.channelKey, true);
            }
        }

        void OnShutdown() override
        {
            sLlmWorker.Stop();
        }
    };

    class ModLlmChatPlayerScript : public PlayerScript
    {
    public:
        ModLlmChatPlayerScript()
            : PlayerScript("mod-llm-chat_player", { PLAYERHOOK_ON_CHAT_COMMAND })
        {
        }

        void OnChatCommand(Player* player, uint32 type, std::string const& msg, uint32 lang, std::string const& to) override
        {
            if (!sLlmConfig.IsEnabled())
                return;
            if (!player || msg.empty())
                return;

            // Ignore system/control commands starting with '.', '!', or '#'
            if (msg[0] == '.' || msg[0] == '!' || msg[0] == '#')
                return;

            bool isSenderBot = sLlmFilter.IsBot(player);

            switch (type)
            {
                case CHAT_MSG_WHISPER:
                {
                    if (isSenderBot)
                        return; // Bots do not whisper other bots for LLM chat

                    Player* target = sObjectAccessor.FindPlayerByName(to.c_str());
                    if (!target || !sLlmFilter.IsBot(target))
                        return;

                    if (!sLlmFilter.CheckAndSetWhisperCooldown(target->GetObjectGuid(), sLlmConfig.GetWhisperCooldown()))
                        return;

                    if (!sLlmFilter.RollChance(sLlmConfig.GetWhisperRate()))
                        return;

                    if (sLlmConfig.EmoteOnWhisper())
                        target->HandleEmote(EMOTE_ONESHOT_QUESTION);

                    ChatRequest req;
                    req.botGuid = target->GetObjectGuid();
                    req.senderGuid = player->GetObjectGuid();
                    req.channel = LLM_CHANNEL_WHISPER;
                    req.channelKey = std::string("whisper:") + target->GetName();
                    req.context = LlmPromptBuilder::ExtractContext(target, player, "whisper", msg);
                    req.language = lang;

                    sLlmWorker.EnqueueRequest(req);
                    break;
                }
                case CHAT_MSG_PARTY:
                case CHAT_MSG_RAID:
                {
                    Group* group = player->GetGroup();
                    if (!group)
                        return;

                    if (sLlmConfig.RequireHumanPresent() && !sLlmFilter.IsHumanInGroup(group))
                        return;

                    std::string channelKey = (type == CHAT_MSG_PARTY ? "party:" : "raid:") + std::to_string(group->GetId());
                    if (!sLlmFilter.CanInitiateOrReply(channelKey, isSenderBot))
                        return;

                    uint32 rate = (type == CHAT_MSG_PARTY) ? sLlmConfig.GetPartyRate() : sLlmConfig.GetRaidRate();
                    if (!sLlmFilter.RollChance(rate))
                        return;

                    // Gather eligible bots in group
                    std::vector<Player*> botCandidates;
                    for (auto const& slot : group->GetMemberSlots())
                    {
                        Player* member = sObjectMgr.GetPlayer(slot.guid);
                        if (member && sLlmFilter.IsBot(member) && member != player && member->IsAlive())
                            botCandidates.push_back(member);
                    }

                    if (botCandidates.empty())
                        return;

                    Player* chosenBot = botCandidates[urand(0, botCandidates.size() - 1)];

                    ChatRequest req;
                    req.botGuid = chosenBot->GetObjectGuid();
                    req.senderGuid = player->GetObjectGuid();
                    req.channel = (type == CHAT_MSG_PARTY) ? LLM_CHANNEL_PARTY : LLM_CHANNEL_RAID;
                    req.channelKey = channelKey;
                    req.context = LlmPromptBuilder::ExtractContext(chosenBot, player, (type == CHAT_MSG_PARTY ? "party" : "raid"), msg);
                    req.language = lang;

                    sLlmFilter.RecordMessageSent(channelKey, isSenderBot);
                    sLlmWorker.EnqueueRequest(req);
                    break;
                }
                case CHAT_MSG_GUILD:
                {
                    uint32 guildId = player->GetGuildId();
                    if (!guildId)
                        return;

                    if (sLlmConfig.RequireHumanPresent() && !sLlmFilter.IsHumanOnlineInGuild(guildId))
                        return;

                    std::string channelKey = "guild:" + std::to_string(guildId);
                    if (!sLlmFilter.CanInitiateOrReply(channelKey, isSenderBot))
                        return;

                    if (!sLlmFilter.RollChance(sLlmConfig.GetGuildRate()))
                        return;

                    // Find online guild bots
                    std::vector<Player*> guildBots;
                    HashMapHolder<Player>::MapType const& players = sObjectAccessor.GetPlayers();
                    for (auto const& pair : players)
                    {
                        Player* p = pair.second;
                        if (p && p->GetGuildId() == guildId && sLlmFilter.IsBot(p) && p != player && p->IsAlive())
                            guildBots.push_back(p);
                    }

                    if (guildBots.empty())
                        return;

                    Player* chosenBot = guildBots[urand(0, guildBots.size() - 1)];

                    ChatRequest req;
                    req.botGuid = chosenBot->GetObjectGuid();
                    req.senderGuid = player->GetObjectGuid();
                    req.channel = LLM_CHANNEL_GUILD;
                    req.channelKey = channelKey;
                    req.context = LlmPromptBuilder::ExtractContext(chosenBot, player, "guild", msg);
                    req.language = lang;

                    sLlmFilter.RecordMessageSent(channelKey, isSenderBot);
                    sLlmWorker.EnqueueRequest(req);
                    break;
                }
                case CHAT_MSG_SAY:
                {
                    if (!player->GetMap())
                        return;

                    float radius = sLlmConfig.GetHumanPresenceRadius();
                    if (sLlmConfig.RequireHumanPresent() && !sLlmFilter.IsHumanPresentForSay(player, radius))
                        return;

                    std::string channelKey = "say:" + std::to_string(player->GetZoneId());
                    if (!sLlmFilter.CanInitiateOrReply(channelKey, isSenderBot))
                        return;

                    if (!sLlmFilter.RollChance(sLlmConfig.GetSayRate()))
                        return;

                    // Find nearby bots within radius
                    std::vector<Player*> nearbyBots;
                    for (auto const& ref : player->GetMap()->GetPlayers())
                    {
                        Player* p = ref.getSource();
                        if (p && sLlmFilter.IsBot(p) && p != player && p->IsAlive() && player->IsWithinDist(p, radius))
                            nearbyBots.push_back(p);
                    }

                    if (nearbyBots.empty())
                        return;

                    Player* chosenBot = nearbyBots[urand(0, nearbyBots.size() - 1)];

                    ChatRequest req;
                    req.botGuid = chosenBot->GetObjectGuid();
                    req.senderGuid = player->GetObjectGuid();
                    req.channel = LLM_CHANNEL_SAY;
                    req.channelKey = channelKey;
                    req.context = LlmPromptBuilder::ExtractContext(chosenBot, player, "say", msg);
                    req.language = lang;

                    sLlmFilter.RecordMessageSent(channelKey, isSenderBot);
                    sLlmWorker.EnqueueRequest(req);
                    break;
                }
            }
        }
    };
}

void Addmod_llm_chatScripts()
{
    new ModLlmChatWorldScript();
    new ModLlmChatPlayerScript();
}
