
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

    // 1. Deep Breath (Phase 2): get off the breath lane immediately - applies with top priority.
    triggers.push_back(new TriggerNode(
        "onyxia deep breath",
        NextAction::array(0, new NextAction("move away from onyxia", 100.0f), NULL)));

    // 2. Phase 3 Landing Threat Reset: Tank must immediately taunt and lock Onyxia
    //    after the 40% threat wipe before DPS causes a turn-cleave/breath.
    triggers.push_back(new TriggerNode(
        "onyxia phase 3 landing threat",
        NextAction::array(0, new NextAction("taunt onyxia landing", 95.0f), NULL)));

    // 2b. Phase 3 Landing Threat Throttle: DPS throttles burst until tank secures threat
    //     or redirects damage to active whelps to prevent turning the boss toward the raid.
    triggers.push_back(new TriggerNode(
        "onyxia phase 3 dps throttle",
        NextAction::array(0, new NextAction("pause dps onyxia landing", 94.0f), NULL)));

    // 2c. Bellowing Roar Fear Counter: Berserker Rage, Tremor Totem, Fear Ward, Will of the Forsaken.
    triggers.push_back(new TriggerNode(
        "onyxia bellowing roar",
        NextAction::array(0, new NextAction("counter onyxia fear", 92.0f), NULL)));

    // 3. North Wall Tank Anchor (Phases 1 and 3): The main tank pulls and holds Onyxia
    //    against the North wall facing away from the raid.
    triggers.push_back(new TriggerNode(
        "onyxia need tank north",
        NextAction::array(0, new NextAction("tank onyxia north", 90.0f), NULL)));

    // 4. Airborne (Phase 2):
    //    - Offtanks immediately taunt and turn Onyxian Warders (12129).
    //    - Melee & AoE DPS burn Onyxian Whelps (11262).
    //    - Ranged and Healers continue DPS/healing on airborne Onyxia (10184).
    triggers.push_back(new TriggerNode(
        "onyxia has warder",
        NextAction::array(0, new NextAction("taunt onyxian warder", 80.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "onyxia has whelps",
        NextAction::array(0, new NextAction("attack onyxia whelps", 75.0f), NULL)));

    if (!ai->IsRanged(bot) && !ai->IsHeal(bot))
    {
        triggers.push_back(new TriggerNode(
            "onyxia airborne",
            NextAction::array(0, new NextAction("attack onyxia whelps", 70.0f), NULL)));
    }
    else
    {
        triggers.push_back(new TriggerNode(
            "onyxia airborne",
            NextAction::array(0, new NextAction("attack entry::10184", 70.0f), NULL)));
    }

    // 5. Grounded (Phases 1 and 3):
    //    - Offtanks hold runaway whelps away from the healers.
    //    - Main raid targets and burns Onyxia.
    triggers.push_back(new TriggerNode(
        "attackers entry::12129",
        NextAction::array(0, new NextAction("taunt onyxian warder", 65.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::11262",
        NextAction::array(0, new NextAction("offtank entry::11262", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "onyxia grounded",
        NextAction::array(0, new NextAction("attack entry::10184", 55.0f), NULL)));

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
    // Tank positions at the North wall before engagement
    triggers.push_back(new TriggerNode(
        "onyxia need tank north",
        NextAction::array(0, new NextAction("tank onyxia north", 20.0f), NULL)));

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

    triggers.push_back(new TriggerNode(
        "onyxia bellowing roar",
        NextAction::array(0, new NextAction("counter onyxia fear", 95.0f), NULL)));
}

