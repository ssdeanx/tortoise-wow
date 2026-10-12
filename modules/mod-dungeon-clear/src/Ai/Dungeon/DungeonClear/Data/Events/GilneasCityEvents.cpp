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

// --- Gilneas City (map 815) -------------------------------------------------
// Custom Turtle WoW level 44-49 dungeon set in the quarantined city of Gilneas.
//
// 1. Matthias Holtz (61419, lvl 44) - Worgen Hunter.
// 2. Packmaster Ragetooth (61420, lvl 44) - Worgen pack leader.
// 3. Judge Sutherland (61421, lvl 46) - High Court Magistrate.
// 4. Dustivan Blackcowl (61422, lvl 46) - Shadow assassin.
// 5. Marshal Magnus Greystone (61423, lvl 47) - Veteran military commander.
// 6 & 7. THE HARLOW COUNCIL (Regent-Lord Mortimer 61264 & Regent-Lady Celia 61263):
//    They stand ~11yd apart and fight simultaneously.
//    At 50% HP, both transform into Dragonkin and cast AoE Blast Wave (13021).
//    Upon Celia's death, the Harlow Family Chest (GO 2020027) respawns at
//    (-968.23, 2959.41, 166.31). The party clears both nobles and loots the chest.
// 8. Genn Greymane (61418, lvl 49) - King of Gilneas (End Boss).
//    Uses Mortal Strike (21551), Drain Life (17620), and Curse of Agony (11711).
//    At 50% HP, triggers heavy melee burst and AoE Fear.

namespace {
constexpr uint32 GC_MAP = 815;

// Boss entries
constexpr uint32 GC_MATTHIAS = 61419;
constexpr uint32 GC_RAGETOOTH = 61420;
constexpr uint32 GC_SUTHERLAND = 61421;
constexpr uint32 GC_BLACKCOWL = 61422;
constexpr uint32 GC_GREYSTONE = 61423;
constexpr uint32 GC_MORTIMER = 61264;
constexpr uint32 GC_CELIA = 61263;
constexpr uint32 GC_GREYMANE = 61418;

// Optional rare
constexpr uint32 GC_HORSEMASTER_LEVIN = 61605;

// GameObjects
constexpr uint32 GC_GO_HARLOW_CHEST = 2020027;

// Order indices (0-based)
constexpr uint32 GC_ORDER_MORTIMER = 5; // Order 6 -> key 5
constexpr uint32 GC_ORDER_CELIA = 6;    // Order 7 -> key 6
constexpr uint32 GC_ORDER_GREYMANE = 7; // Order 8 -> key 7

// Event IDs
constexpr uint32 GC_EVENT_HARLOW_COUNCIL = 1;
constexpr uint32 GC_EVENT_HARLOW_CHEST = 2;
constexpr uint32 GC_EVENT_GREYMANE = 3;
} // namespace

void RegisterGilneasCityEvents(std::vector<DungeonEvent>& out)
{
    // 1. Harlow Council - coordinate kill of Lord Mortimer and Lady Celia
    out.push_back(
        EventBuilder(GC_MAP, GC_EVENT_HARLOW_COUNCIL, "Harlow Council (Mortimer & Celia)")
            .Anchored(GC_ORDER_MORTIMER)
            .Persistent()
            .KillCreatureEngage(GC_MORTIMER, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(180000)
            .KillCreatureEngage(GC_CELIA, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(180000)
            .Build());

    // 2. Harlow Family Chest - loots the chest spawned after Celia's defeat
    out.push_back(
        EventBuilder(GC_MAP, GC_EVENT_HARLOW_CHEST, "Harlow Family Chest")
            .Anchored(GC_ORDER_CELIA)
            .Persistent()
            .UseGO(GC_GO_HARLOW_CHEST, /*searchRadius*/ 40.0f, -968.23f, 2959.41f, 166.314f)
            .Timeout(30000)
            .Build());

    // 3. Genn Greymane - final encounter
    out.push_back(
        EventBuilder(GC_MAP, GC_EVENT_GREYMANE, "King Genn Greymane")
            .Anchored(GC_ORDER_GREYMANE)
            .Persistent()
            .KillCreatureEngage(GC_GREYMANE, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(240000)
            .Build());
}
