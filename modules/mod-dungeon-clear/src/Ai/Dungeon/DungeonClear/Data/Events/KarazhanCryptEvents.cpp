/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license; you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option) any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

// --- Karazhan Crypt (map 800) -----------------------------------------------
// Custom Turtle WoW 5-man dungeon under Deadwind Pass.
//
// 1. ALARUS (91928) - SUMMON-ONLY BOSS AT AN OBJECTIVE ANCHOR
//    Alarus has NO static creature spawn in `creature`:
//    instance_karazhan_crypt.cpp monitors six Necrotic Runes (177302, 177305,
//    177306, 177307, 177308, 177309) via the invisible Crypt Watcher trigger
//    (91931). Once all six runes are activated (GO_STATE_ACTIVE), the trigger
//    summons Alarus at (-11087.8, -1774.9, 51.8). BossSpawnIndex can never emit
//    him and a static order row could never credit him. The roster patch below
//    inserts an OBJECTIVE in the order-2 slot (key 1, between Commander
//    Andreon's key 0 and Corpsemuncher's key 2), and this anchored event drives
//    the sequence:
//      - Move to and activate each of the 6 Necrotic Runes in Andreon's tomb
//      hall.
//      - WaitForSpawn for Alarus (91928).
//      - KillCreatureEngage Alarus to defeat him.
//
//    On Alarus' death, instance_karazhan_crypt opens the Pauper's Walk door
//    (2006634), granting access forward.
//
// 2. MARROWSPIKE (91920) & HIVAXXIS (91929)
//    Door 177312 ("Tomb of the Unrepentant") gates the web chamber of Hivaxxis.
//    instance_karazhan_crypt opens door 177312 on Marrowspike's death, so
//    Marrowspike must strictly precede Hivaxxis in the clear order. Both doors
//    (177312 and 2006634) are registered as script-only in
//    DcEventDoorRegistry.h.

namespace {
constexpr uint32 KC_MAP = 800;

// Boss entries
constexpr uint32 KC_ALARUS = 91928;

// Event ID (per map)
constexpr uint32 KC_EVENT_ALARUS = 1;
constexpr uint32 KC_EVENT_ROOM_PRECLEAR = 2;

// Clear-path slot (see DcBossEntries1121.h)
// Andreon takes order 1 (key 0). Key 1 is the slot for the Alarus objective.
constexpr uint32 KC_ALARUS_ORDER = 1;

// Centroid of the Necrotic Rune tomb chamber in Andreon's wing
constexpr float KC_ALARUS_OBJ_X = -11068.0f;
constexpr float KC_ALARUS_OBJ_Y = -1600.0f;
constexpr float KC_ALARUS_OBJ_Z = 28.3f;

// The six Necrotic Runes around Andreon's chamber
struct KarazhanCryptRune {
  uint32 goEntry;
  float x, y, z;
};

constexpr KarazhanCryptRune KC_NECROTIC_RUNES[] = {
    {177302, -11094.0f, -1624.04f, 28.29f},
    {177305, -11030.5f, -1622.47f, 28.29f},
    {177306, -11062.8f, -1612.24f, 28.41f},
    {177307, -11096.5f, -1528.91f, 28.40f},
    {177308, -11030.8f, -1611.71f, 28.40f},
    {177309, -11094.2f, -1613.06f, 28.41f},
};

constexpr uint32 KC_ALARUS_SPAWN_TIMEOUT = 60000;
constexpr uint32 KC_ALARUS_KILL_TIMEOUT = 300000;
} // namespace

void RegisterKarazhanCryptEvents(std::vector<DungeonEvent> &out) {
  EventBuilder b(KC_MAP, KC_EVENT_ALARUS, "Summon and defeat Alarus");
  b.Anchored(KC_ALARUS_ORDER).Persistent();

  for (KarazhanCryptRune const &r : KC_NECROTIC_RUNES) {
    b.MoveTo(r.x, r.y, r.z, /*radius*/ 5.0f)
        .UseGO(r.goEntry, /*searchRadius*/ 12.0f, r.x, r.y, r.z);
  }

  b.WaitForSpawn(KC_ALARUS, /*wantAlive*/ true)
      .Timeout(KC_ALARUS_SPAWN_TIMEOUT)
      .KillCreatureEngage(KC_ALARUS, /*count*/ 1, /*searchRadius*/ 300.0f)
      .EngageOnlyWhenActive()
      .Timeout(KC_ALARUS_KILL_TIMEOUT);

  out.push_back(b.Build());

  // Room-aggro pre-clear for Archlich Enkhraz
  out.push_back(
      EventBuilder(KC_MAP, KC_EVENT_ROOM_PRECLEAR,
                   "Clear tomb chamber before Archlich Enkhraz")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());
}

void RegisterKarazhanCryptRoster(std::vector<BossRosterPatch> &t) {
  using namespace DcRoster;

  BossRosterPatch p;
  p.mapId = KC_MAP;
  p.add = {
      MakeObjective(OBJ(1), KC_ALARUS_ORDER, KC_MAP,
                    "Alarus (Summoned via Necrotic Runes)", KC_ALARUS_OBJ_X,
                    KC_ALARUS_OBJ_Y, KC_ALARUS_OBJ_Z,
                    /*arriveRadius*/ 12.0f, /*gateEntry*/ 0,
                    /*hook*/ 0, /*eventId*/ KC_EVENT_ALARUS,
                    /*orderOverride*/ KC_ALARUS_ORDER),
  };
  t.push_back(std::move(p));
}
