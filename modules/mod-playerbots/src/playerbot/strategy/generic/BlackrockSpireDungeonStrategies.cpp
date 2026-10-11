#include "playerbot/playerbot.h"
#include "BlackrockSpireDungeonStrategies.h"

using namespace ai;

void BlackrockSpireDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start pyroguard emberseer fight",
        NextAction::array(0, new NextAction("enable pyroguard emberseer fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start rend blackhand fight",
        NextAction::array(0, new NextAction("enable rend blackhand fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start the beast fight",
        NextAction::array(0, new NextAction("enable the beast fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start general drakkisath fight",
        NextAction::array(0, new NextAction("enable general drakkisath fight strategy", 100.0f), NULL)));
}

void BlackrockSpireDungeonStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "ahnqiraj supplies needed",
        NextAction::array(0, new NextAction("give ahnqiraj supplies", 90.0f), NULL)));
}

// --- Pyroguard Emberseer (9816) -------------------------------------------
void PyroguardEmberseerFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Kill Incarcerators (10316) first to free Emberseer cleanly
    triggers.push_back(new TriggerNode(
        "attackers entry::10316",
        NextAction::array(0, new NextAction("attack entry::10316", 80.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "fire resistance aura",
            NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));
}

void PyroguardEmberseerFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end pyroguard emberseer fight",
        NextAction::array(0, new NextAction("disable pyroguard emberseer fight strategy", 100.0f), NULL)));
}

void PyroguardEmberseerFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end pyroguard emberseer fight",
        NextAction::array(0, new NextAction("disable pyroguard emberseer fight strategy", 100.0f), NULL)));
}

// --- Rend Blackhand (10429) & Gyth (10339) ---------------------------------
void RendBlackhandFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Stadium waves: chromatic whelps (10442), dragons (10447), handlers (10742)
    triggers.push_back(new TriggerNode(
        "attackers entry::10442",
        NextAction::array(0, new NextAction("attack entry::10442", 75.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::10447",
        NextAction::array(0, new NextAction("attack entry::10447", 74.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::10742",
        NextAction::array(0, new NextAction("attack entry::10742", 73.0f), NULL)));

    // Gyth priority when present
    triggers.push_back(new TriggerNode(
        "attackers entry::10339",
        NextAction::array(0, new NextAction("attack entry::10339", 80.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "fire resistance aura",
            NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));
}

void RendBlackhandFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end rend blackhand fight",
        NextAction::array(0, new NextAction("disable rend blackhand fight strategy", 100.0f), NULL)));
}

void RendBlackhandFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end rend blackhand fight",
        NextAction::array(0, new NextAction("disable rend blackhand fight strategy", 100.0f), NULL)));
}

// --- The Beast (10430) -----------------------------------------------------
void TheBeastFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "fire resistance aura",
            NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));
}

void TheBeastFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end the beast fight",
        NextAction::array(0, new NextAction("disable the beast fight strategy", 100.0f), NULL)));
}

void TheBeastFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end the beast fight",
        NextAction::array(0, new NextAction("disable the beast fight strategy", 100.0f), NULL)));
}

// --- General Drakkisath (10363) --------------------------------------------
void GeneralDrakkisathFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Off-tank / prioritize Chromatic Elite guards (10814)
    triggers.push_back(new TriggerNode(
        "attackers entry::10814",
        NextAction::array(0, new NextAction("attack entry::10814", 75.0f), NULL)));

    if (bot->getClass() == CLASS_PALADIN && IsGroupResistDropper(ai, bot))
    {
        triggers.push_back(new TriggerNode(
            "fire resistance aura",
            NextAction::array(0, new NextAction("fire resistance aura", 10.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "fire protection potion ready",
        NextAction::array(0, new NextAction("fire protection potion", 100.0f), NULL)));
}

void GeneralDrakkisathFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end general drakkisath fight",
        NextAction::array(0, new NextAction("disable general drakkisath fight strategy", 100.0f), NULL)));
}

void GeneralDrakkisathFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end general drakkisath fight",
        NextAction::array(0, new NextAction("disable general drakkisath fight strategy", 100.0f), NULL)));
}
