
#include "playerbot/playerbot.h"
#include "OnyxiasLairDungeonStrategies.h"

using namespace ai;

void OnyxiasLairDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    // If the bot is summoned straight into the fight.
    triggers.push_back(new TriggerNode(
        "start onyxia fight",
        NextAction::array(0, new NextAction("enable onyxia fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Onyxia (10184, boss_onyxia.cpp verified).
//
// Phase 1 (100-65%): ground. Flame Breath 18435 cone -> melee stay at the
//   side/behind; Fireball 18392; Cleave 19983; Tail Sweep 15847.
// Phase 2 (65-40%): airborne (HOVER 17131). Whelps 11262 spawn in waves of
//   5-7 from two side alcoves; Warders 12129 stand at the sides. Deep Breath
//   23461 carves the room - everyone scatters off the lane. Melee burn whelps,
//   ranged keep DPS on Onyxia; offtanks hold Warders.
// Phase 3 (40-0%): grounded burn. Whelps keep spawning; Bellowing Roar 18431
//   fear. DPS returns to the boss, offtanks hold the whelps away.
// ---------------------------------------------------------------------------

void OnyxiaFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    Player* bot = ai->GetBot();

    // Deep Breath: get off the lane - applies in every phase.
    triggers.push_back(new TriggerNode(
        "onyxia deep breath",
        NextAction::array(0, new NextAction("move away from onyxia", 100.0f), NULL)));

    // Airborne (phase 2): melee burns whelps, offtanks hold the Warders. Ranged
    // and healers keep working on the boss at range.
    if (!ai->IsRanged(bot) && !ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "onyxia airborne",
            NextAction::array(0, new NextAction("attack entry::11262", 65.0f), NULL)));
    }

    triggers.push_back(new TriggerNode(
        "attackers entry::12129",
        NextAction::array(0, new NextAction("offtank entry::12129", 60.0f), NULL)));

    // Grounded (phases 1 and 3): DPS returns to the boss; offtanks keep the
    // whelp packs off the raid.
    triggers.push_back(new TriggerNode(
        "onyxia grounded",
        NextAction::array(0, new NextAction("attack entry::10184", 75.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::11262",
        NextAction::array(0, new NextAction("offtank entry::11262", 55.0f), NULL)));

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

void OnyxiaFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end onyxia fight",
        NextAction::array(0, new NextAction("disable onyxia fight strategy", 100.0f), NULL)));
}

void OnyxiaFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end onyxia fight",
        NextAction::array(0, new NextAction("disable onyxia fight strategy", 100.0f), NULL)));
}

void OnyxiaFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "onyxia deep breath",
        NextAction::array(0, new NextAction("move away from onyxia", 100.0f), NULL)));
}
