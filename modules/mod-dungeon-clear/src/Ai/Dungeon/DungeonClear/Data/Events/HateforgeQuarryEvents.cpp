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

// --- Hateforge Quarry (map 808) ---------------------------------------------
// Custom Turtle WoW level 50-60 5-man dungeon in Burning Steppes.
//
// 1. HIGH FOREMAN BARGUL BLACKHAMMER (60735)
//    Guards the forge workshop. Defeating him allows access deeper into the
//    quarry.
//
// 2. HAR'GESH DOOMCALLER (60737) - FINAL ENCOUNTER
//    At 50% HP, Har'gesh transitions to Phase 2:
//      - Casts SPELL_IMMUNE_ALL (29230) and channels SPELL_SHADOW_CHANNELING
//      (12380)
//      - Summons Faceless Terrors (60738) that attack the party
//      - Har'gesh remains immune and rooted until all Faceless Terrors are
//      killed
//    Bots must immediately target and clear the Faceless Terrors to break his
//    immunity.

namespace {
constexpr uint32 HQ_MAP = 808;

// Boss entries
constexpr uint32 HQ_FIGGLES = 60736;
constexpr uint32 HQ_BARGUL = 60735;
constexpr uint32 HQ_CORROSIS = 60829;
constexpr uint32 HQ_HATEREAVER = 60734;
constexpr uint32 HQ_HARGESH_DOOMCALLER = 60737;

// Adds
constexpr uint32 HQ_FACELESS_TERROR = 60738;

// Order indices
constexpr uint32 HQ_ORDER_HARGESH = 5;

// Event IDs
constexpr uint32 HQ_EVENT_HARGESH = 1;
} // namespace

void RegisterHateforgeQuarryEvents(std::vector<DungeonEvent> &out) {
  // Har'gesh Doomcaller Phase 2 Faceless Terror adds & immunity break
  out.push_back(EventBuilder(HQ_MAP, HQ_EVENT_HARGESH,
                             "Har'gesh Doomcaller & Faceless Terrors")
                    .Anchored(HQ_ORDER_HARGESH)
                    .Persistent()
                    .KillCreatureEngage(HQ_HARGESH_DOOMCALLER, /*count*/ 1,
                                        /*seekRadius*/ 60.0f)
                    .Timeout(180000)
                    // Priority target switch when Faceless Terrors spawn
                    .KillCreatureEngage(HQ_FACELESS_TERROR, /*count*/ 0,
                                        /*seekRadius*/ 60.0f)
                    .Timeout(90000)
                    .KillCreatureEngage(HQ_HARGESH_DOOMCALLER, /*count*/ 1,
                                        /*seekRadius*/ 60.0f)
                    .Timeout(180000)
                    .Build());
}
