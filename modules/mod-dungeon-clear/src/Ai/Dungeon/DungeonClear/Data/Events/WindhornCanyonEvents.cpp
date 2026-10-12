/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license; you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"
#include "Creature.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"

// --- Windhorn Canyon (map 820) ----------------------------------------------
// Custom Turtle WoW level 24-29 dungeon in the Stonetalon Mountains.
//
// 1. Pathun Duskhide (62779, lvl 25) - Master of the Hunt.
// 2. Ahgk'tos the Pure (62778, lvl 24) - Water elemental / spirit.
// 3. Ambassador Vortalus (62783, lvl 26) - Centaur emissary.
// 4. Walgan Bloodcaller (62784, lvl 27) - Master of Arms.
// 5. Bonespeaker Narlgom (62780, lvl 27) - Witchdoctor / necromancer.
//    At 50% HP, casts Summon Rotag (spell 42004), which summons Spirit of
//    Champion Rotag (61410). Note: Champion Rotag (62785) is the unattackable
//    ritual corpse prop channeling in the cave (UNIT_FLAG_NON_ATTACKABLE,
//    REACT_PASSIVE, UNIT_STAND_STATE_DEAD), NOT the combat target. Bots must
//    target and defeat Spirit of Champion Rotag (61410) as a combat add.
// 6. Prophet Stormhoof (62781, lvl 28) - Deathtotem Prophet.
// 7. Chieftain Shalk Blackwind (62782, lvl 29) - End Boss.
//    At 50% HP, Shalk yells and summons a Blackwind Bloodguard (62771) to assist.
//    Casts Flame of Shalk (41121); extinguished if a player is underwater.
//    Bots prioritize the Bloodguard add and finish Shalk.

namespace {
constexpr uint32 WHC_MAP = 820;

// Boss entries
constexpr uint32 WHC_PATHUN = 62779;
constexpr uint32 WHC_AHGKTOS = 62778;
constexpr uint32 WHC_VORTALUS = 62783;
constexpr uint32 WHC_WALGAN = 62784;
constexpr uint32 WHC_NARLGOM = 62780;
constexpr uint32 WHC_STORMHOOF = 62781;
constexpr uint32 WHC_SHALK = 62782;

// Add entries
constexpr uint32 WHC_ROTAG_PROP = 62785;          // Unattackable ritual corpse
constexpr uint32 WHC_SPIRIT_OF_ROTAG = 61410;     // Actual summoned combat add
constexpr uint32 WHC_BLACKWIND_BLOODGUARD = 62771; // Shalk's 50% add
constexpr uint32 WHC_STORM_GUARDIAN = 62865;
constexpr uint32 WHC_STORM_RESIDUE = 62866;

// Order indices (0-based)
constexpr uint32 WHC_ORDER_NARLGOM = 4; // Order 5 -> key 4
constexpr uint32 WHC_ORDER_SHALK = 6;   // Order 7 -> key 6

// Event IDs
constexpr uint32 WHC_EVENT_NARLGOM = 1;
constexpr uint32 WHC_EVENT_SHALK = 2;
} // namespace

void RegisterWindhornCanyonEvents(std::vector<DungeonEvent>& out)
{
    // 1. Bonespeaker Narlgom & Spirit of Champion Rotag
    out.push_back(
        EventBuilder(WHC_MAP, WHC_EVENT_NARLGOM, "Bonespeaker Narlgom & Spirit of Rotag")
            .Anchored(WHC_ORDER_NARLGOM)
            .Persistent()
            .KillCreatureEngage(WHC_NARLGOM, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(180000)
            .KillCreatureEngage(WHC_SPIRIT_OF_ROTAG, /*count*/ 0, /*seekRadius*/ 60.0f)
            .Timeout(60000)
            .KillCreatureEngage(WHC_NARLGOM, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(180000)
            .Build());

    // 2. Chieftain Shalk Blackwind & Blackwind Bloodguard
    out.push_back(
        EventBuilder(WHC_MAP, WHC_EVENT_SHALK, "Chieftain Shalk Blackwind & Bloodguard")
            .Anchored(WHC_ORDER_SHALK)
            .Persistent()
            .KillCreatureEngage(WHC_SHALK, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(180000)
            .KillCreatureEngage(WHC_BLACKWIND_BLOODGUARD, /*count*/ 0, /*seekRadius*/ 60.0f)
            .Timeout(60000)
            .KillCreatureEngage(WHC_SHALK, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(180000)
            .Build());
}

void RegisterWindhornCanyonRoster(std::vector<BossRosterPatch>& t)
{
    using namespace DcRoster;
    BossRosterPatch p;
    p.mapId = WHC_MAP;
    // Explicitly drop Champion Rotag (62785) - corpse prop, not a boss
    p.remove.push_back(WHC_ROTAG_PROP);
    t.push_back(std::move(p));
}
