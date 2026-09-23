#ifndef LLM_PROMPT_BUILDER_H
#define LLM_PROMPT_BUILDER_H

#include "Common.h"
#include <string>

class Player;

struct BotChatContext
{
    std::string botName;
    std::string botRace;
    std::string botClass;
    std::string botGender;
    uint32 botLevel;
    std::string zoneName;
    std::string channel;
    std::string senderName;
    std::string incomingMessage;
};

class LlmPromptBuilder
{
public:
    static BotChatContext ExtractContext(Player* bot, Player* sender, std::string const& channel, std::string const& msg);

    static std::string BuildSystemPrompt(BotChatContext const& ctx);
    static std::string BuildUserPrompt(BotChatContext const& ctx);

    static std::string SanitizeReply(std::string const& rawReply);

    static std::string GetRaceName(uint8 race);
    static std::string GetClassName(uint8 cls);
};

#endif // LLM_PROMPT_BUILDER_H
