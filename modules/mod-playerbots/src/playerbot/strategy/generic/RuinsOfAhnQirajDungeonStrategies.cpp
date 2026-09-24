
#include "playerbot/playerbot.h"
#include "RuinsOfAhnQirajDungeonStrategies.h"

using namespace ai;

// ---------------------------------------------------------------------------
// Ruins of Ahn'Qiraj (AQ20, map 509). NPC and spell ids verified from
// src/scripts/dungeons/ruins_of_ahnqiraj/*.cpp plus generic_scripts /
// creature_ai_events / spell_template in tw_world:
//   Kurinnaxx 15348 (Mortal Wound 25646 stacks to 10; Sand Trap GO 180647)
//   Rajaxx event: Andorov 15471 + Kaldorei Elite 15473; wave officers
//     Qeez 15391 -> Tuubid 15392 -> Drenn 15389 -> Xurrem 15390 ->
//     Yeggeth 15386 -> Pakkon 15388 -> Zerran 15385 -> Rajaxx 15341; adds
//     Qiraji Warrior 15387, Swarmguard Needler 15344, Qiraji Swarmguard 15343.
//     Any evade or 180 s wave timeout fails the event and respawns every
//     officer (generic_scripts 153912), so no run-out/kite actions here.
//   Buru the Gorger 15370 (kite: the locked target is chased; Hatchling 15521)
//   Moam 15340 (Mana Fiend 15527 priority)
//   Ayamiss the Hunter 15369 (Larva 15555 -> Hornet 15934; Swarmer 15546)
//   Ossirian the Unscarred 15339 (Sand Vortex 15428 hazard; the crystal
//     clicks remain a player action)
// ---------------------------------------------------------------------------

void RuinsOfAhnQirajDungeonStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "start kurinnaxx fight",
        NextAction::array(0, new NextAction("enable kurinnaxx fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start rajaxx fight",
        NextAction::array(0, new NextAction("enable rajaxx fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start buru fight",
        NextAction::array(0, new NextAction("enable buru fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start moam fight",
        NextAction::array(0, new NextAction("enable moam fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start ayamiss fight",
        NextAction::array(0, new NextAction("enable ayamiss fight strategy", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "start ossirian fight",
        NextAction::array(0, new NextAction("enable ossirian fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Kurinnaxx: Mortal Wound (25646) stacks to 10 -> offtank taunt at 5 (Gluth
// pattern); the Sand Trap GO (180647) is a 15 yd nature/slow/silence hazard
// the whole raid moves out of.
// ---------------------------------------------------------------------------

void KurinnaxxFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "kurinnaxx tank switch",
        NextAction::array(0, new NextAction("taunt kurinnaxx", 95.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "kurinnaxx sand trap",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

void KurinnaxxFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end kurinnaxx fight",
        NextAction::array(0, new NextAction("disable kurinnaxx fight strategy", 100.0f), NULL)));
}

void KurinnaxxFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end kurinnaxx fight",
        NextAction::array(0, new NextAction("disable kurinnaxx fight strategy", 100.0f), NULL)));
}

void KurinnaxxFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "kurinnaxx sand trap",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Rajaxx event: the raid waits at the room opening so each wave comes to it
// one at a time (the event activates one officer group at a time). The officer
// waves in spawn order (Qeez -> Tuubid -> Drenn -> Xurrem -> Yeggeth -> Pakkon
// -> Zerran) are burned first; the wave adds are held by the tanks; Rajaxx
// himself is the default assist target. Every priority action is range capped
// so no bot charges across the arena and starts the next wave early.
// ---------------------------------------------------------------------------

void RajaxxFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::15391",
        NextAction::array(0, new NextAction("attack entry::15391,25", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15392",
        NextAction::array(0, new NextAction("attack entry::15392,25", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15389",
        NextAction::array(0, new NextAction("attack entry::15389,25", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15390",
        NextAction::array(0, new NextAction("attack entry::15390,25", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15386",
        NextAction::array(0, new NextAction("attack entry::15386,25", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15388",
        NextAction::array(0, new NextAction("attack entry::15388,25", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15385",
        NextAction::array(0, new NextAction("attack entry::15385,25", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15387",
        NextAction::array(0, new NextAction("offtank entry::15387,25", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15344",
        NextAction::array(0, new NextAction("offtank entry::15344,25", 55.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15343",
        NextAction::array(0, new NextAction("offtank entry::15343,25", 55.0f), NULL)));
}

void RajaxxFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end rajaxx fight",
        NextAction::array(0, new NextAction("disable rajaxx fight strategy", 100.0f), NULL)));
}

void RajaxxFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end rajaxx fight",
        NextAction::array(0, new NextAction("disable rajaxx fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Buru the Gorger: the target Buru locks (1M threat) kites him while he is
// close; Hatchlings (15521) from popped eggs are killed on sight. Eggs are
// left to the player - breaking one resets Buru's chase.
// ---------------------------------------------------------------------------

void BuruFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "buru chasing me",
        NextAction::array(0, new NextAction("move away from buru", 100.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15521",
        NextAction::array(0, new NextAction("attack entry::15521,30", 55.0f), NULL)));

    // Creeping Plague (20512) is a raid-wide nature DoT.
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void BuruFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end buru fight",
        NextAction::array(0, new NextAction("disable buru fight strategy", 100.0f), NULL)));
}

void BuruFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end buru fight",
        NextAction::array(0, new NextAction("disable buru fight strategy", 100.0f), NULL)));
}

void BuruFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "buru chasing me",
        NextAction::array(0, new NextAction("move away from buru", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Moam: Mana Fiends (15527) drain the raid's mana and are burned first;
// Arcane Eruption is a single-target knockback on the remembered victim, so
// no spread is needed.
// ---------------------------------------------------------------------------

void MoamFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::15527",
        NextAction::array(0, new NextAction("attack entry::15527,30", 60.0f), NULL)));

    // Arcane Eruption (25672) is arcane - the potion choice for this fight.
    // Drain Mana (25676) is shadow but only drains, so it does not change the
    // pick (protection potions share a cooldown).
    triggers.push_back(new TriggerNode(
        "arcane protection potion ready",
        NextAction::array(0, new NextAction("arcane protection potion", 100.0f), NULL)));
}

void MoamFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end moam fight",
        NextAction::array(0, new NextAction("disable moam fight strategy", 100.0f), NULL)));
}

void MoamFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end moam fight",
        NextAction::array(0, new NextAction("disable moam fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Ayamiss the Hunter: the Hive'Zara Larva (15555) walking to the paralyzed
// player is the top priority (killing it prevents the Hornet summon); Hornets
// (15934) are next; Swarmers (15546) are transient and cleaned up last.
// ---------------------------------------------------------------------------

void AyamissFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "attackers entry::15555",
        NextAction::array(0, new NextAction("attack entry::15555,30", 70.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15934",
        NextAction::array(0, new NextAction("attack entry::15934,30", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        "attackers entry::15546",
        NextAction::array(0, new NextAction("attack entry::15546,25", 50.0f), NULL)));

    // Poison Stinger / Stinger Spray are nature.
    triggers.push_back(new TriggerNode(
        "nature protection potion ready",
        NextAction::array(0, new NextAction("nature protection potion", 100.0f), NULL)));
}

void AyamissFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ayamiss fight",
        NextAction::array(0, new NextAction("disable ayamiss fight strategy", 100.0f), NULL)));
}

void AyamissFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ayamiss fight",
        NextAction::array(0, new NextAction("disable ayamiss fight strategy", 100.0f), NULL)));
}

// ---------------------------------------------------------------------------
// Ossirian the Unscarred: the roaming Sand Vortex (15428) hazards are dodged
// with the shared hazard movement. The crystal clicks - and therefore the
// Strength of Ossirian windows - stay a player action.
// ---------------------------------------------------------------------------

void OssirianFightStrategy::InitCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "ossirian sand vortex",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}

void OssirianFightStrategy::InitNonCombatTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ossirian fight",
        NextAction::array(0, new NextAction("disable ossirian fight strategy", 100.0f), NULL)));
}

void OssirianFightStrategy::InitDeadTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "end ossirian fight",
        NextAction::array(0, new NextAction("disable ossirian fight strategy", 100.0f), NULL)));
}

void OssirianFightStrategy::InitReactionTriggers(std::list<TriggerNode*>& triggers)
{
    triggers.push_back(new TriggerNode(
        "ossirian sand vortex",
        NextAction::array(0, new NextAction("move away from hazard", 100.0f), NULL)));
}
