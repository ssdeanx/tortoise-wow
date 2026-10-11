/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license; you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option) any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

// --- Lower Karazhan Halls (map 532) ---------------------------------------
// One event at one objective anchor. Everything else this map needs is already
// carried by the authored order block in DcBossEntries1121.h (Araxxna, the Dark
// Rider Champion, Howlfang, Grizikil, Moroes), and there is no door, GameObject
// or trigger on any of the map's scripts to gate on - so the only two mechanics
// the core actually enforces here both belong to the same fight sequence:
//
// 1. LORD BLACKWALD II (61222) - SUMMON-ONLY BOSS AT AN OBJECTIVE ANCHOR
//    (Molten Core's Majordomo / Ragnaros shape, verbatim). He has NO creature
//    spawn row: instance_lower_karazhan_halls.cpp:115-126 summons him out of
//    Update() at the hard-coded g_lordBlackwaldPos (:4, -11088.2, -1995.74,
//    76.18) only once GetData(DATA_DR_CHAMPION) == DONE - i.e. only after the
//    Dark Rider Champion (61204, order 2 / key 1) has died - and gives him a
//    7-DAY respawn delay so the ordinary spawn settings cannot bring him back
//    inside a run. BossSpawnIndex can therefore never emit him and no order row
//    could ever credit him, so a plain boss anchor would wedge the clear on an
//    uncreditable leg. The roster patch below adds an OBJECTIVE in the vacant
//    order-3 slot (key 2, between the Champion's key 1 and Howlfang's key 3)
//    and this anchored event does the fight.
//
//    The completion condition is the EVENT's, never a completion bit. Contrary
//    to how he behaves in Karazhan proper, boss_blackwald_ii.cpp has NO
//    surrender and NO teleport-out: Aggro -> IN_PROGRESS, EnterEvadeMode ->
//    FAIL, and JustDied (:66-73) is the only writer of DATA_BLACKWALD_II =
//    DONE. He is killed like any other boss, and the objective latches when the
//    step list ends (DcObjectiveArriveAction) - so the run never waits on an
//    encounter bit it cannot observe.
//
//    His CallForHelp adds (61192 / 61193 / 61194 / 61211, one random entry per
//    44-51s, TEMPSUMMON_CORPSE_TIMED_DESPAWN 5s) are deliberately NOT the gate.
//    All four are ALSO ordinary ambient spawns on 532 (22 / 12 / 21 slots in
//    tw_world.creature), so "his adds are gone" is not a statement about him at
//    all; and he keeps calling every 44-51s until he dies, so an add-death gate
//    would open while he is still standing in the fight. That is the one shape
//    the Majordomo event gets away with (Domo stops summoning when his adds
//    stop coming) and it does not transfer here.
//
//    WaitForSpawn between the anchor arrival and the kill is essential: the
//    engage gate would otherwise read "no live Blackwald" before the instance's
//    Update() has summoned him and false-complete the objective. Persistent so
//    the kill's >1s combat tick-gap cannot rewind the step list.
//
// 2. THE DARK RIDER CHAMPION'S APPRENTICES (61203 x2). His OnCreatureCreate
//    (instance_lower_karazhan_halls.cpp:29-38) summons both as
//    TEMPSUMMON_DEAD_DESPAWN 60s temps at his own position, and his AI
//    respawns any that die WHILE HE IS ALIVE AND OUT OF COMBAT
//    (CheckAppretinces, :1066-1078, re-run once a second). So they cannot be
//    cleared before his leg is done - and they need no help after it, because
//    with him dead the watchdog stops and the two of them are ordinary
//    leftovers. That is why the clear sits at the END of the Blackwald event:
//    step 1 cannot pass until DATA_DR_CHAMPION is DONE, so by the time the
//    apprentice step is reached his respawn loop is dead by construction and
//    the gate can never be fought by a mob that reappears. While he IS alive
//    they are simply his adds, and the ordinary boss pipeline clears them with
//    him - which is why there is no separate event for them.
//
//    count is 1 on purpose: the executor's KillCreature gate is "any alive
//    creature of the entry blocks" (DungeonEventExecutor.cpp:728), so one step
//    covers both, and as TEMPSUMMON_DEAD_DESPAWN they also vanish on their own
//    within 60s - this step only has to catch them, it is never a precondition
//    the clear depends on.

namespace {
constexpr uint32 LKH_MAP = 532;

// Boss entries
constexpr uint32 LKH_ARAXXNA = 61221;
constexpr uint32 LKH_DR_CHAMPION = 61204;
constexpr uint32 LKH_BLACKWALD = 61222;
constexpr uint32 LKH_HOWLFANG = 61223;
constexpr uint32 LKH_GRIZIKIL = 61224;
constexpr uint32 LKH_MOROES = 61225;

// Add entries
constexpr uint32 LKH_SKITTERWEB_EGG = 30008;
constexpr uint32 LKH_SPIDER_1 = 61206;
constexpr uint32 LKH_SPIDER_2 = 61207;
constexpr uint32 LKH_SPIDER_3 = 61209;
constexpr uint32 LKH_DR_APPRENTICE = 61203;
constexpr uint32 LKH_GRELLKIN_1 = 61195;
constexpr uint32 LKH_GRELLKIN_2 = 61196;
constexpr uint32 LKH_GRELLKIN_3 = 61197;

// --- event id (per map) --------------------------------------------------
constexpr uint32 LKH_EVENT_BLACKWALD = 1;
constexpr uint32 LKH_EVENT_ROOM_PRECLEAR = 2;
constexpr uint32 LKH_EVENT_ARAXXNA = 3;
constexpr uint32 LKH_EVENT_CHAMPION = 4;
constexpr uint32 LKH_EVENT_GRIZIKIL = 5;
constexpr uint32 LKH_EVENT_MOROES = 6;

// --- clear-path slot (see DcBossEntries1121.h) --------------------------
// Order rows are keyed on (order - 1): Araxxna 0, the Champion 1, Howlfang
// 3 - key 2 is the deliberate vacancy Blackwald takes, and at equal key an
// Objective sorts BEFORE a Boss, so nothing here has to move. Same value for
// the objective's encounterIndex (the event's .Anchored() key) and its
// orderOverride (the path slot), matching the Zul'Gurub Arlokk / Molten Core
// objective convention.
constexpr uint32 LKH_ARAXXNA_ORDER = 0;
constexpr uint32 LKH_CHAMPION_ORDER = 1;
constexpr uint32 LKH_BLACKWALD_ORDER = 2;
constexpr uint32 LKH_HOWLFANG_ORDER = 3;
constexpr uint32 LKH_GRIZIKIL_ORDER = 4;
constexpr uint32 LKH_MOROES_ORDER = 5;

// --- geometry ------------------------------------------------------------
// g_lordBlackwaldPos (instance_lower_karazhan_halls.cpp:4). The objective
// sits exactly where the instance summons him, so the anchor navigation
// walks the tank into his arena before step 1 ever starts.
constexpr float LKH_BLACKWALD_X = -11088.2f;
constexpr float LKH_BLACKWALD_Y = -1995.74f;
constexpr float LKH_BLACKWALD_Z = 76.1774f;

// The kill's own seek radius. He spawns on the anchor itself, so a small
// radius reaches him; the apprentice step below is the one that may have to
// walk the tank off the anchor.
constexpr float LKH_BLACKWALD_SEARCH = 60.0f;
// Wide on purpose. The two 61203 follow their CHAMPION, not Blackwald, so
// they can be left anywhere his corpse is. KillCreature is entry-filtered,
// so a wide search can only ever find those two - it cannot sweep in any
// other mob, and if they are out of range the gate reads clear instead of
// stalling.
constexpr float LKH_APPRENTICE_SEARCH = 150.0f;

// He appears on the instance's own Update() the moment DATA_DR_CHAMPION is
// DONE, so the spawn wait only has to cover boss-nav's walk in. Generous,
// as elsewhere (MC_DOMO_SPAWN_TIMEOUT).
constexpr uint32 LKH_BLACKWALD_SPAWN_TIMEOUT = 300000;
constexpr uint32 LKH_BLACKWALD_KILL_TIMEOUT = 300000;
// Two 63-rank-0 temps, and only if they are still up at all - short.
constexpr uint32 LKH_APPRENTICE_TIMEOUT = 120000;
} // namespace

void RegisterLowerKarazhanHallsEvents(std::vector<DungeonEvent> &out) {
  out.push_back(
      EventBuilder(LKH_MAP, LKH_EVENT_BLACKWALD,
                   "Lord Blackwald II (summoned by the Dark Rider Champion)")
          .Anchored(LKH_BLACKWALD_ORDER)
          .Persistent()
          .WaitForSpawn(LKH_BLACKWALD, /*wantAlive*/ true)
          .Timeout(LKH_BLACKWALD_SPAWN_TIMEOUT)
          .KillCreatureEngage(LKH_BLACKWALD, /*count*/ 1, LKH_BLACKWALD_SEARCH)
          .Timeout(LKH_BLACKWALD_KILL_TIMEOUT)
          // The Champion's leftovers - only reachable because step 1
          // waited on the Champion's DONE (see the header).
          .KillCreatureEngage(LKH_DR_APPRENTICE, /*count*/ 1,
                              LKH_APPRENTICE_SEARCH)
          .Timeout(LKH_APPRENTICE_TIMEOUT)
          .Build());

  // Room-aggro pre-clear for Brood Queen Araxxna (Skitterweb Eggs)
  out.push_back(
      EventBuilder(LKH_MAP, LKH_EVENT_ROOM_PRECLEAR,
                   "Clear room trash before room-aggro boss (Lower Karazhan Halls)")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());

  // Brood Queen Araxxna: prioritize Skitterweb Eggs and hatched spiders
  out.push_back(
      EventBuilder(LKH_MAP, LKH_EVENT_ARAXXNA, "Brood Queen Araxxna & Skitterweb Eggs")
          .Anchored(LKH_ARAXXNA_ORDER)
          .KillCreatureEngage(LKH_SKITTERWEB_EGG, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_SPIDER_1, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_SPIDER_2, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_SPIDER_3, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_ARAXXNA, /*count*/ 1, 80.0f)
          .Timeout(300000)
          .Build());

  // Dark Rider Champion: prioritize Dark Rider Apprentices
  out.push_back(
      EventBuilder(LKH_MAP, LKH_EVENT_CHAMPION, "Dark Rider Champion & Apprentices")
          .Anchored(LKH_CHAMPION_ORDER)
          .KillCreatureEngage(LKH_DR_APPRENTICE, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_DR_CHAMPION, /*count*/ 1, 80.0f)
          .Timeout(300000)
          .Build());

  // Grizikil: prioritize Grellkin adds
  out.push_back(
      EventBuilder(LKH_MAP, LKH_EVENT_GRIZIKIL, "Grizikil & Grellkins")
          .Anchored(LKH_GRIZIKIL_ORDER)
          .KillCreatureEngage(LKH_GRELLKIN_1, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_GRELLKIN_2, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_GRELLKIN_3, /*count*/ 0, 80.0f)
          .KillCreatureEngage(LKH_GRIZIKIL, /*count*/ 1, 80.0f)
          .Timeout(300000)
          .Build());

  // Moroes: challenge via gossip dialogue, then engage and defeat
  out.push_back(
      EventBuilder(LKH_MAP, LKH_EVENT_MOROES, "Challenge and defeat Moroes")
          .Anchored(LKH_MOROES_ORDER)
          .Persistent()
          .Gossip(LKH_MOROES, 0, 60.0f)
          .SkipIfTargetMissing()
          .KillCreatureEngage(LKH_MOROES, /*count*/ 1, 60.0f)
          .Timeout(300000)
          .Build());
}

// --- roster patch : Lord Blackwald II as an anchored objective -------------
void RegisterLowerKarazhanHallsRoster(std::vector<BossRosterPatch> &t) {
  using namespace DcRoster;

  // Blackwald has no creature spawn, so BossSpawnIndex never emits him and no
  // order row could ever credit him (see the header). The objective anchor
  // gives him a real leg anyway: boss-nav drives the party to his summon point
  // exactly as it would to a boss, the anchored event above does the
  // wait-then-kill, and the objective latches cleared when that event
  // completes (DcObjectiveArriveAction) - so the clear walks the vacant
  // order-3 slot instead of stalling on a boss nothing can credit.
  {
    BossRosterPatch p;
    p.mapId = LKH_MAP;
    p.add = {
        MakeObjective(OBJ(1), LKH_BLACKWALD_ORDER, LKH_MAP, "Lord Blackwald II",
                      LKH_BLACKWALD_X, LKH_BLACKWALD_Y, LKH_BLACKWALD_Z,
                      /*arriveRadius*/ 12.0f, /*gateEntry*/ 0,
                      /*hook*/ 0, /*eventId*/ LKH_EVENT_BLACKWALD,
                      /*orderOverride*/ LKH_BLACKWALD_ORDER),
    };
    t.push_back(std::move(p));
  }
}
