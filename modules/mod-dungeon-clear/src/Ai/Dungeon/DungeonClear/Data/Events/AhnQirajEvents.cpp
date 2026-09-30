/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option) any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"
#include "Creature.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"
#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"

// --- Ruins (509) / Temple (531) of Ahn'Qiraj -------------------------------
//
// Every row here exists because a filename, a spawn row, or a boss-order line
// ALONE is not support. Each one is written against the LIVE native contract
// (tw_world + instance/boss scripts) and only adds the orchestration the clear
// otherwise lacks; it never re-implements a boss mechanic.
//
//  509 RAJAXX (order 2). 15341 is a REAL static spawn but the EventAI spawn
//  script (1534105) makes it PASSIVE + INVINCIBLE on load. The whole fight is a
//  NATIVE seven-officer chain (Andorov 15471 gates it; engaging ANY officer runs
//  generic_scripts 153911->...->153411, each success spawning the next officer and
//  finally flipping Rajaxx hostile; a 180s timeout or any evade runs 153912 and
//  resets the event). So the clear must NOT treat 15341 as a plain static boss.
//  Fix: drop the raw 15341 entry (it would otherwise be targeted while passive and
//  stall) and anchor an OBJECTIVE at the arena whose event engages the seven
//  officers IN THE NATIVE CHAIN ORDER and then Rajaxx. Each step is an idempotent
//  kill-gate, so a wipe/restart re-evaluates correctly; Persistent keeps the chain
//  from rewinding across the (many) combat gaps. The native chain stays the owner of
//  activation, timers and failure reset — the event only walks the tank in and
//  pulls the current officer.
//
//  531 OURO (order 11). 15517 has NO static spawn: the Ouro Spawner 15957 summons
//  him (npc_ouro_spawner, 25yd player LoS) and despawns. The order row alone can
//  never enter BossSpawnIndex. Fix: an OBJECTIVE anchored on the spawner that waits
//  for the real 15517 to appear and kills it — identical in shape to Blackwing
//  Lair's spawnless Nefarian throne objective. Ouro is never summoned directly.
//
//  531 BUG TRIO (orders 2-4). Kri/Yauj/Vem are a COMPOSITE: killing one makes the
//  survivors devour the corpse and heal to FULL (boss_bug_trio.cpp), and the
//  instance only flips TYPE_BUG_TRIO to DONE after all three SPECIAL deaths. Three
//  independent order rows + three independent death bits advance after ONE death,
//  which is wrong. Fix: remove all three raw entries and add ONE objective whose
//  event has THREE kill-gates (Vem, Yauj, Kri). The objective carries no kill-bit,
//  so the clear cannot advance until the event (all three dead) completes — the
//  Stratholme Dathrohan->Balnazzar two-step precedent, extended to three steps.
//
//  531 TWINS (orders 9-10). Vek'nilash 15275 and Vek'lor 15276 are a SHARED-health
//  LINKED pair: the native JustDied kills the other, so ONE death ends the fight.
//  Two order rows is the wrong shape. Fix: drop 15276 and keep 15275 as the single
//  boss — its death (which takes 15276 with it) is the native joint completion.
//
//  531 C'THUN (order 12). The body 15727 is a real static boss, but boss_cthun
//  spawns it NON-SELECTABLE and the ATTACKABLE Eye 15589 (no static row) is the
//  phase-1 target. A clear that only knows 15727 walks to a non-targetable body
//  and stalls. Fix: a near-gated conditional event that engages the Eye while it
//  is alive; once it dies the body becomes the selectable target the normal boss
//  flow already owns (tentacle adds are the playerbot Cthun strategy's job).
//
//  509 OSSIRIAN (order 6). The Strength-of-Ossirian windows only open by clicking
//  the rotating crystals (GO 180619, script_name=ossirian_crystal). Best-effort:
//  a Repeatable/DrivesInCombat conditional that clicks a live crystal while the
//  fight runs. Optional, so a bot that cannot reach a crystal never blocks.

namespace
{
    constexpr uint32 AQ20_MAP = 509;
    constexpr uint32 AQ40_MAP = 531;

    // ---- 509 Ruins entries, in the native Rajaxx chain order ----------------
    constexpr uint32 AQ20_RAJAXX = 15341;
    constexpr uint32 AQ20_QEEZ = 15391;
    constexpr uint32 AQ20_TUUBID = 15392;
    constexpr uint32 AQ20_DRENN = 15389;
    constexpr uint32 AQ20_XURREM = 15390;
    constexpr uint32 AQ20_YEGGETH = 15386;
    constexpr uint32 AQ20_PAKKON = 15388;
    constexpr uint32 AQ20_ZERRAN = 15385;

    // ---- 531 Temple entries -------------------------------------------------
    constexpr uint32 AQ40_OURO = 15517;
    constexpr uint32 AQ40_VEM = 15544;
    constexpr uint32 AQ40_YAUJ = 15543;
    constexpr uint32 AQ40_KRI = 15511;
    constexpr uint32 AQ40_TWIN_CASTER = 15276; // Vek'lor; Vek'nilash 15275 stays
    constexpr uint32 AQ40_CTHUN_EYE = 15589;

    // ---- 509 Ossirian crystal GO --------------------------------------------
    constexpr uint32 AQ20_OSSIRIAN_CRYSTAL = 180619;

    // Clear-path order slots, on the SAME encounterIndex scale the base list uses.
    // BossSpawnIndex sets a raw boss's key to (DcBossEntries order - 1), and
    // NextDungeonBossValue advances to the lowest key STRICTLY GREATER than the one
    // just finished. So an objective that TIES the next boss's key would make that
    // boss invisible to the picker (it is not strictly greater). Each objective
    // therefore takes the exact (order - 1) slot of the entry it replaces:
    //   509 Rajaxx replaces 15341 (order 2 -> key 1), between Kurinnaxx(0) and Buru(2)
    //   531 Trio   replaces Vem  (order 2 -> key 1), between Skeram(0) and Sartura(4)
    //   531 Ouro   occupies Ouro's own absent slot (order 11 -> key 10), between
    //              Vek'nilash(8) and C'Thun(11)
    constexpr uint32 AQ20_ORDER_RAJAXX = 1;
    constexpr uint32 AQ40_ORDER_BUG_TRIO = 1;
    constexpr uint32 AQ40_ORDER_OURO = 10;

    // Event ids.
    constexpr uint32 AQ20_EVENT_RAJAXX = 1;
    constexpr uint32 AQ20_EVENT_OSSIRIAN_CRYSTAL = 2;
    constexpr uint32 AQ40_EVENT_BUG_TRIO = 1;
    constexpr uint32 AQ40_EVENT_OURO = 2;
    constexpr uint32 AQ40_EVENT_CTHUN_EYE = 3;

    // Live anchors (tw_world).
    constexpr float AQ20_RAJAXX_ARENA_X = -9015.17f; // Qeez 15391, the first pull
    constexpr float AQ20_RAJAXX_ARENA_Y = 1555.58f;
    constexpr float AQ20_RAJAXX_ARENA_Z = 21.388f;

    constexpr float AQ40_OURO_SPAWNER_X = -9188.45f; // Ouro Spawner 15957
    constexpr float AQ40_OURO_SPAWNER_Y = 2091.56f;
    constexpr float AQ40_OURO_SPAWNER_Z = -64.17f;

    constexpr float AQ40_BUG_TRIO_X = -8534.08f; // Vem 15544
    constexpr float AQ40_BUG_TRIO_Y = 2169.40f;
    constexpr float AQ40_BUG_TRIO_Z = -3.22f;

    constexpr float AQ20_OSSIRIAN_X = -9493.60f; // Ossirian 15339
    constexpr float AQ20_OSSIRIAN_Y = 2030.27f;
    constexpr float AQ20_OSSIRIAN_Z = 105.48f;

    constexpr float AQ40_CTHUN_BODY_X = -8577.27f; // C'Thun body 15727
    constexpr float AQ40_CTHUN_BODY_Y = 1986.94f;
    constexpr float AQ40_CTHUN_BODY_Z = 100.40f;

    // Native instance data ids (ruins_of_ahnqiraj.h / temple_of_ahnqiraj.h).
    constexpr uint32 AQ20_TYPE_OSSIRIAN = 6;
    constexpr uint32 AQ40_TYPE_CTHUN = 5;

    constexpr uint32 STATE_IN_PROGRESS = 1;
    constexpr uint32 STATE_DONE = 3;

    // Best-effort crystal click while Ossirian is engaged and not done.
    bool OssirianFightRunning(Player* bot, AiObjectContext* /*context*/)
    {
        InstanceScript* inst = DcTargeting::GetInstanceScript(bot);
        if (!inst)
            return false;
        return inst->GetData(AQ20_TYPE_OSSIRIAN) == STATE_IN_PROGRESS;
    }

    // C'thun phase 1: the Eye (15589) is the attackable target while it lives.
    // Once it is dead the condition goes false and the normal body flow resumes.
    // NEAR-GATED on purpose: the Eye is a map-wide FindLiveCreatureOnMap hit the
    // moment the room loads, so without the 70yd body check the event could arm
    // (and its lone KillCreatureEngage step could false-latch) from across the
    // map. The tank must be in C'thun's chamber before the Eye is pulled.
    bool CthunEyeActive(Player* bot, AiObjectContext* /*context*/)
    {
        if (!bot)
            return false;
        InstanceScript* inst = DcTargeting::GetInstanceScript(bot);
        if (!inst)
            return false;
        if (inst->GetData(AQ40_TYPE_CTHUN) == STATE_DONE)
            return false;
        if (bot->GetDistance(AQ40_CTHUN_BODY_X, AQ40_CTHUN_BODY_Y, AQ40_CTHUN_BODY_Z) > 70.0f)
            return false;
        return DcTargeting::FindLiveCreatureOnMap(bot, AQ40_CTHUN_EYE) != nullptr;
    }
}

void RegisterAhnQirajEvents(std::vector<DungeonEvent>& out)
{
    // --- 509 Rajaxx: drive the native seven-officer chain -------------------
    // Each KillCreatureEngage is an idempotent "no alive <entry> remains" gate
    // that ACTIVELY pulls the named officer, in the exact order the native chain
    // spawns them. Out-of-order kills still resolve (already-dead = done), and a
    // wipe/restart re-evaluates from the top because every step re-checks liveness.
    out.push_back(EventBuilder(AQ20_MAP, AQ20_EVENT_RAJAXX, "Rajaxx (officer chain)")
                      .Anchored(AQ20_ORDER_RAJAXX)
                      .Persistent()
                      .KillCreatureEngage(AQ20_QEEZ, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ20_TUUBID, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ20_DRENN, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ20_XURREM, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ20_YEGGETH, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ20_PAKKON, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ20_ZERRAN, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ20_RAJAXX, 1, 100.0f).Timeout(600000)
                      .Build());

    // --- 509 Ossirian: rotate the weakness crystals (best effort) -----------
    // Repeatable so each freshly-spawned crystal pair can be clicked again;
    // DrivesInCombat because the windows are mid-fight. Optional: a bot that
    // cannot reach a crystal must never block the clear.
    out.push_back(EventBuilder(AQ20_MAP, AQ20_EVENT_OSSIRIAN_CRYSTAL, "Ossirian crystal")
                      .Conditional(&OssirianFightRunning)
                      .Repeatable()
                      .DrivesInCombat()
                      .Optional()
                      .UseGO(AQ20_OSSIRIAN_CRYSTAL, 100.0f,
                             AQ20_OSSIRIAN_X, AQ20_OSSIRIAN_Y, AQ20_OSSIRIAN_Z)
                          .Timeout(15000)
                      .Build());

    // --- 531 Bug Trio: one objective, three kill-gates -----------------------
    out.push_back(EventBuilder(AQ40_MAP, AQ40_EVENT_BUG_TRIO, "Bug Trio (composite)")
                      .Anchored(AQ40_ORDER_BUG_TRIO)
                      .Persistent()
                      .KillCreatureEngage(AQ40_VEM, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ40_YAUJ, 1, 100.0f).Timeout(300000)
                      .KillCreatureEngage(AQ40_KRI, 1, 100.0f).Timeout(300000)
                      .Build());

    // --- 531 Ouro: walk to the spawner, wait for the real Ouro, kill him -----
    out.push_back(EventBuilder(AQ40_MAP, AQ40_EVENT_OURO, "Ouro (spawner)")
                      .Anchored(AQ40_ORDER_OURO)
                      .Persistent()
                      .WaitForSpawn(AQ40_OURO, /*wantAlive*/ true, 300000)
                      .KillCreatureEngage(AQ40_OURO, 1, 100.0f).Timeout(600000)
                      .Build());

    // --- 531 C'thun phase 1: engage the Eye while it is the live target ------
    // NOT DrivesInCombat on purpose: the Eye is pulled by the stock out-of-combat
    // conditional rung the moment the party enters the chamber (the condition is
    // near-gated to the body), and once the eye is in combat the ordinary combat
    // engine owns the fight. Handing every combat tick to the event driver here
    // would starve the raid's damage for no gain.
    out.push_back(EventBuilder(AQ40_MAP, AQ40_EVENT_CTHUN_EYE, "C'thun (Eye phase)")
                      .Conditional(&CthunEyeActive)
                      .KillCreatureEngage(AQ40_CTHUN_EYE, 1, 80.0f).Timeout(600000)
                      .Build());
}

void RegisterAhnQirajRoster(std::vector<BossRosterPatch>& t)
{
    using namespace DcRoster;

    // --- 509 Rajaxx: replace the passive static boss with the chain objective --
    {
        BossRosterPatch p;
        p.mapId = AQ20_MAP;
        p.remove = { AQ20_RAJAXX };
        p.add = {
            MakeObjective(OBJ(1), AQ20_ORDER_RAJAXX, AQ20_MAP, "General Rajaxx (officer chain)",
                          AQ20_RAJAXX_ARENA_X, AQ20_RAJAXX_ARENA_Y, AQ20_RAJAXX_ARENA_Z,
                          /*arriveRadius*/ 20.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ AQ20_EVENT_RAJAXX,
                          /*orderOverride*/ AQ20_ORDER_RAJAXX),
        };
        t.push_back(std::move(p));
    }

    // --- 531 Bug Trio: three independent rows -> one composite objective -----
    {
        BossRosterPatch p;
        p.mapId = AQ40_MAP;
        p.remove = { AQ40_VEM, AQ40_YAUJ, AQ40_KRI };
        p.add = {
            MakeObjective(OBJ(2), AQ40_ORDER_BUG_TRIO, AQ40_MAP, "Bug Trio (composite)",
                          AQ40_BUG_TRIO_X, AQ40_BUG_TRIO_Y, AQ40_BUG_TRIO_Z,
                          /*arriveRadius*/ 20.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ AQ40_EVENT_BUG_TRIO,
                          /*orderOverride*/ AQ40_ORDER_BUG_TRIO),
        };
        t.push_back(std::move(p));
    }

    // --- 531 Ouro: no static spawn -> objective on the spawner ---------------
    {
        BossRosterPatch p;
        p.mapId = AQ40_MAP;
        p.add = {
            MakeObjective(OBJ(3), AQ40_ORDER_OURO, AQ40_MAP, "Ouro (Ouro Spawner)",
                          AQ40_OURO_SPAWNER_X, AQ40_OURO_SPAWNER_Y, AQ40_OURO_SPAWNER_Z,
                          /*arriveRadius*/ 20.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ AQ40_EVENT_OURO,
                          /*orderOverride*/ AQ40_ORDER_OURO),
        };
        t.push_back(std::move(p));
    }

    // --- 531 Twins: linked/shared-health pair -> one boss (15275) ------------
    {
        BossRosterPatch p;
        p.mapId = AQ40_MAP;
        p.remove = { AQ40_TWIN_CASTER };
        t.push_back(std::move(p));
    }
}
