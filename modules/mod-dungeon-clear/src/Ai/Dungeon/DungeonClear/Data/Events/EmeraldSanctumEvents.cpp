/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license; you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"
#include "Creature.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"

// --- Emerald Sanctum (map 807) --------------------------------------------
//
// 40-player custom raid instance (Turtle-WoW content).
// Boss encounters:
// 1. Erennius (60747) at (3293.46, 3130.04, 24.51):
//    Acid Breath, Howl of Erennius, Curse of Erennius, Wall of Erennius.
// 2. Solnius (60748) at (3320.08, 3042.30, 25.27):
//    Acid Breath, Corrosive Bolt, Dreamestate, summoning waves of Sanctum
//    Wyrms (60744), Sanctum Dragonkins (60743), and Sanctum Suppressors
//    (61212).

namespace {
constexpr uint32 ES_MAP = 807;

// Boss entries
constexpr uint32 ES_ERENNIUS = 60747;
constexpr uint32 ES_SOLNIUS = 60748;

// Add entries
constexpr uint32 ES_WYRM = 60744;
constexpr uint32 ES_DRAGONKIN = 60743;
constexpr uint32 ES_SUPPRESSOR = 61212;
constexpr uint32 ES_WYRMKIN = 60745;
constexpr uint32 ES_SCALEBANE = 60746;
constexpr uint32 ES_DREAMER = 60742;

// Event IDs
constexpr uint32 ES_EVENT_ERENNIUS = 1;
constexpr uint32 ES_EVENT_SOLNIUS = 2;
constexpr uint32 ES_EVENT_ROOM_PRECLEAR = 3;

// Order indices matching DcBossEntries (key = order - 1)
constexpr uint32 ES_ORDER_ERENNIUS = 0; // Order 1 -> key 0
constexpr uint32 ES_ORDER_SOLNIUS = 1;  // Order 2 -> key 1
} // namespace

void RegisterEmeraldSanctumEvents(std::vector<DungeonEvent> &out) {
  // 1. Erennius (order 1) - prioritize adds if active
  out.push_back(
      EventBuilder(ES_MAP, ES_EVENT_ERENNIUS, "Erennius")
          .Anchored(ES_ORDER_ERENNIUS)
          .KillCreatureEngage(ES_SUPPRESSOR, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_WYRM, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_WYRMKIN, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_SCALEBANE, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_DRAGONKIN, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_DREAMER, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_ERENNIUS, /*count*/ 1, /*seekRadius*/ 80.0f)
          .Timeout(300000)
          .Build());

  // 2. Solnius (end boss, order 2) - clear adds if spawned during phase transitions
  out.push_back(
      EventBuilder(ES_MAP, ES_EVENT_SOLNIUS, "Solnius")
          .Anchored(ES_ORDER_SOLNIUS)
          .KillCreatureEngage(ES_SUPPRESSOR, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_WYRM, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_WYRMKIN, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_SCALEBANE, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_DRAGONKIN, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_DREAMER, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(ES_SOLNIUS, /*count*/ 1, /*seekRadius*/ 80.0f)
          .Timeout(600000)
          .Build());

  // 3. Room-aggro pre-clear for Erennius & Solnius
  out.push_back(
      EventBuilder(ES_MAP, ES_EVENT_ROOM_PRECLEAR, "Clear the room before the room-aggro boss (Emerald Sanctum)")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());
}

void RegisterEmeraldSanctumRoster(std::vector<BossRosterPatch> & /*t*/) {
  // Both bosses have natural creature spawns and compiled orders in
  // DcBossEntries1121.h. No roster patches needed.
}
