/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license, you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"
#include "Creature.h"
#include "DungeonEventTables.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"

// --- Onyxia's Lair (map 249) -----------------------------------------------
//
// Single-encounter 40-man classic raid dungeon.
// 1. Entrance corridor: Onyxian Warders (12129) cleared along tunnel.
// 2. Main Chamber: Onyxia (10184) located at (29.81f, -215.50f, -85.0f).
//    Phase 1 Ground: MT anchors to North wall (-8.86f, -212.75f, -88.54f), raid
//    flanks. Phase 2 Flight (65-40%): Whelp waves (11262) & entrance Warders
//    (12129) burn, Deep Breath avoidance. Phase 3 Ground (40-0%): Instant
//    threat wipe recovery (taunt lock), Bellowing Roar fear management.

namespace {
constexpr uint32 ONY_MAP = 249;
constexpr uint32 ONY_BOSS_ENTRY = 10184;
constexpr uint32 ONY_WARDER_ENTRY = 12129;
constexpr uint32 ONY_WHELP_ENTRY = 11262;

constexpr uint32 ONY_EVENT_ROOM_PRECLEAR = 1;
constexpr uint32 ONY_EVENT_ONYXIA_FIGHT = 2;
constexpr uint32 ONY_BOSS_ORDER = 1;

// Onyxia chamber center coords
constexpr float ONY_CHAMBER_X = -12.867f;
constexpr float ONY_CHAMBER_Y = -216.626f;
constexpr float ONY_CHAMBER_Z = -88.058f;
} // namespace

void RegisterOnyxiasLairEvents(std::vector<DungeonEvent> &out) {
  // 1. Clear entrance tunnel Onyxian Warders before pulling Onyxia
  out.push_back(EventBuilder(ONY_MAP, ONY_EVENT_ROOM_PRECLEAR,
                             "Clear Onyxian Warders before Onyxia")
                    .Conditional(&DcRoomAggroPreClearCondition)
                    .KillCreature(/*room trash*/ 0)
                    .Build());

  // 2. Onyxia Encounter: prioritize Onyxian Warders & Whelps, engage and kill
  // Onyxia.
  out.push_back(EventBuilder(ONY_MAP, ONY_EVENT_ONYXIA_FIGHT,
                             "Engage and defeat Onyxia & adds")
                    .Anchored(ONY_BOSS_ORDER)
                    .Persistent()
                    .KillCreatureEngage(ONY_WARDER_ENTRY, /*count*/ 0, 80.0f)
                    .KillCreatureEngage(ONY_WHELP_ENTRY, /*count*/ 0, 80.0f)
                    .KillCreatureEngage(ONY_BOSS_ENTRY, /*count*/ 1, 150.0f)
                    .Timeout(900000)
                    .Build());
}

void RegisterOnyxiasLairRoster(std::vector<BossRosterPatch> &t) {
  using namespace DcRoster;

  // Ensure Onyxia objective is properly registered on map 249.
  {
    BossRosterPatch p;
    p.mapId = ONY_MAP;
    p.add = {MakeObjective(BossRosterRegistry::ObjectiveEntry(1),
                           ONY_BOSS_ORDER, ONY_MAP, "Onyxia", ONY_CHAMBER_X,
                           ONY_CHAMBER_Y, ONY_CHAMBER_Z,
                           /*arriveRadius*/ 20.0f, /*gateEntry*/ 0,
                           /*hook*/ 0, /*eventId*/ ONY_EVENT_ONYXIA_FIGHT,
                           /*orderOverride*/ ONY_BOSS_ORDER)};
    t.push_back(std::move(p));
  }
}
