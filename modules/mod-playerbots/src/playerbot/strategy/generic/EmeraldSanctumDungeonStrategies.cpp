#include "playerbot/playerbot.h"
#include "EmeraldSanctumDungeonStrategies.h"

using namespace ai;

void EmeraldSanctumDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start erennius fight",
        NextAction::array(0, new NextAction("enable erennius fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start solnius fight",
        NextAction::array(0, new NextAction("enable solnius fight strategy", 100.0f), NULL)));
}

void EmeraldSanctumDungeonStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "ahnqiraj supplies needed",
        NextAction::array(0, new NextAction("give ahnqiraj supplies", 90.0f), NULL)));
}

// --- Erennius (60747) ------------------------------------------------------
void ErenniusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "nature resistance aura",
            NextAction::array(0, new NextAction("nature resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void ErenniusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end erennius fight",
        NextAction::array(0, new NextAction("disable erennius fight strategy", 100.0f), NULL)));
}

void ErenniusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end erennius fight",
        NextAction::array(0, new NextAction("disable erennius fight strategy", 100.0f), NULL)));
}

// --- Solnius (60748) -------------------------------------------------------
void SolniusFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Priority adds during phase transitions:
    // Sanctum Suppressor (61212) -> Sanctum Dragonkin (60743) -> Sanctum Wyrm (60744)
    triggers.push_back(new TriggerNode(
        "attackers entry::61212",
        NextAction::array(0, new NextAction("attack entry::61212", 85.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::60743",
        NextAction::array(0, new NextAction("attack entry::60743", 80.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::60744",
        NextAction::array(0, new NextAction("attack entry::60744", 75.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "nature resistance aura",
            NextAction::array(0, new NextAction("nature resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "shadow protection potion ready",
        NextAction::array(0, new NextAction("shadow protection potion", 90.0f), NULL)));
}

void SolniusFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end solnius fight",
        NextAction::array(0, new NextAction("disable solnius fight strategy", 100.0f), NULL)));
}

void SolniusFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end solnius fight",
        NextAction::array(0, new NextAction("disable solnius fight strategy", 100.0f), NULL)));
}
