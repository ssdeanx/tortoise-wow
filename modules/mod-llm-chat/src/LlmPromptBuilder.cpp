#include "LlmPromptBuilder.h"
#include "Objects/Player.h"
#include "Maps/Map.h"
#include <sstream>
#include <algorithm>

std::string LlmPromptBuilder::GetRaceName(uint8 race)
{
    switch (race)
    {
        case RACE_HUMAN:    return "Human";
        case RACE_ORC:      return "Orc";
        case RACE_DWARF:    return "Dwarf";
        case RACE_NIGHTELF: return "Night Elf";
        case RACE_UNDEAD:   return "Undead";
        case RACE_TAUREN:   return "Tauren";
        case RACE_GNOME:    return "Gnome";
        case RACE_TROLL:    return "Troll";
        default:            return "Adventurer";
    }
}

std::string LlmPromptBuilder::GetClassName(uint8 cls)
{
    switch (cls)
    {
        case CLASS_WARRIOR: return "Warrior";
        case CLASS_PALADIN: return "Paladin";
        case CLASS_HUNTER:  return "Hunter";
        case CLASS_ROGUE:   return "Rogue";
        case CLASS_PRIEST:  return "Priest";
        case CLASS_SHAMAN:  return "Shaman";
        case CLASS_MAGE:    return "Mage";
        case CLASS_WARLOCK: return "Warlock";
        case CLASS_DRUID:   return "Druid";
        default:            return "Hero";
    }
}

BotChatContext LlmPromptBuilder::ExtractContext(Player* bot, Player* sender, std::string const& channel, std::string const& msg)
{
    BotChatContext ctx;
    if (!bot)
        return ctx;

    ctx.botName = bot->GetName();
    ctx.botRace = GetRaceName(bot->getRace());
    ctx.botClass = GetClassName(bot->getClass());
    ctx.botGender = (bot->getGender() == GENDER_MALE ? "male" : "female");
    ctx.botLevel = bot->GetLevel();

    uint32 zoneId = bot->GetZoneId();
    AreaEntry const* area = AreaEntry::GetById(zoneId);
    if (area && area->Name)
        ctx.zoneName = area->Name;
    else
        ctx.zoneName = "Azeroth";

    ctx.channel = channel;
    ctx.senderName = sender ? sender->GetName() : "Someone";
    ctx.incomingMessage = msg;

    return ctx;
}

std::string LlmPromptBuilder::BuildSystemPrompt(BotChatContext const& ctx)
{
    std::ostringstream ss;
    ss << "You are " << ctx.botName << ", a level " << ctx.botLevel << " "
       << ctx.botRace << " " << ctx.botClass << " in World of Warcraft, currently in " << ctx.zoneName << ".\n"
       << "You are communicating via in-game " << ctx.channel << " chat.\n"
       << "Rules:\n"
       << "1. Stay strictly in character as a Warcraft adventurer. Match your race and class dialect.\n"
       << "2. Never reveal or mention that you are an AI, bot, or language model.\n"
       << "3. Do not output your internal thoughts or reasoning. Speak directly to the listener.\n"
       << "4. Keep your reply concise (1 or 2 short sentences, maximum 240 characters total).\n";
    return ss.str();
}

std::string LlmPromptBuilder::BuildUserPrompt(BotChatContext const& ctx)
{
    std::ostringstream ss;
    ss << ctx.senderName << " says: \"" << ctx.incomingMessage << "\"";
    return ss.str();
}

std::string LlmPromptBuilder::SanitizeReply(std::string const& rawReply)
{
    std::string text = rawReply;

    // 1. If response contains <think> ... </think>, remove it
    size_t startThink = text.find("<think>");
    if (startThink != std::string::npos)
    {
        size_t endThink = text.find("</think>", startThink);
        if (endThink != std::string::npos)
            text.erase(startThink, (endThink + 8) - startThink);
    }

    // 2. Trim whitespace and quotation marks
    size_t first = text.find_first_not_of(" \t\r\n\"'`");
    if (first == std::string::npos)
        return "";
    size_t last = text.find_last_not_of(" \t\r\n\"'`");
    text = text.substr(first, (last - first + 1));

    // 3. Remove leading speaker prefix like "Thorgar: "
    size_t colonPos = text.find(':');
    if (colonPos != std::string::npos && colonPos < 20)
    {
        std::string prefix = text.substr(0, colonPos);
        // If prefix is just the bot name or "Bot", strip it
        if (prefix.find(' ') == std::string::npos)
        {
            text = text.substr(colonPos + 1);
            first = text.find_first_not_of(" \t");
            if (first != std::string::npos)
                text = text.substr(first);
        }
    }

    // 4. Cap to 254 bytes (World of Warcraft chat packet limit is 255)
    if (text.size() > 250)
        text = text.substr(0, 247) + "...";

    return text;
}
