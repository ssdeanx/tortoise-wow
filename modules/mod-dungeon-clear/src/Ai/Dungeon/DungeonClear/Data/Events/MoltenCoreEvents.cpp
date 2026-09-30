/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "Creature.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"
#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"

// --- Molten Core (map 409) -------------------------------------------------
// Five groups of rows, each following an existing dungeon precedent:
//
// 1. CORE HOUND PACK CLEARS (events 1-5, one per pack). mob_core_houndAI
//    (molten_core.cpp:249-375): lethal damage is NEGATED and the hound feigns
//    death on a 10s timer; at 10s it scans 100yd for any Core Hound that is
//    in combat AND alive - if one exists it RESURRECTS, else it truly dies.
//    So a pack must be finished inside one ~10s window and two packs must
//    never be in combat together. Owner rules (critical): the whole pack is
//    TANKED and KILLED TOGETHER - engaging one hound brings the formation
//    (creature_groups) and the DPS burns them down as one, never one at a
//    time; and TWO packs must be cleared before Lucifron - pack 1 while he is
//    away, and only once every hound of it is down, pack 2 (the strict
//    sequence is enforced in the predicate).
//
//    The five packs sit in the Lucifron/Magmadar corridor: the two PRE-LUCIFRON
//    packs (A 993,-972 and B 1035,-962) are in the area the corridor opens into
//    past the exit - the stretch LUCIFRON PATROLS (they are not at the exit
//    itself) - mixed with the Flame Imps (11669, 40 of them in the same band),
//    so expect the imps in the pull; the other three (C 1078,-973,
//    D 1088,-1020, E 1114,-990) are the Magmadar side. That same stretch lies
//    straight across Lucifron's 39-waypoint patrol loop (movement_type 2,
//    x 1000..1120 / y -1018..-958), so every pack event is gated on him being
//    AWAY on the far side of the loop (65yd; see MC_PATROL_SAFE for the
//    measured alternation - A/B open while he is east, C/D/E while he is
//    west). Each event is conditional (proximity + patrol window), Repeatable,
//    and a tight ClearRadius restricted to 11671 - never a wide radius (a
//    100yd clear would pull the next pack, and that pack's combat is exactly
//    what reignites the feign-dead). The completion gate is sound as-is: a
//    reignited hound is alive again, and all-feign-dead with no live
//    in-combat packmate resolves to true death.
//
//    NOTE ON THE PULL SIDE: while the run's pull machinery (DcRel::Pull, 35)
//    claims the tick it engages the packs as route trash on its own; this
//    event (DcRel::EventDue, 31) owns them whenever it does not (pull off /
//    between pulls), and the live config's PullDynamicMaxLeeroyMobs = 5 makes
//    a 5-elite pack patrol-contended against Lucifron, so the governor's
//    PatrolHold also waits him out. See Phase 13 of the raid plan.
//
// 2. RUNE-CHAIN SAFETY NETS (events 6-12, one per rune). The playerbot layer
//    douses the runes (move-to + Aqual Quintessence 17333, gated on the boss
//    DONE); GOHello_go_rune_MC (instance_molten_core.cpp:556-658) summons
//    Majordomo once all 7 runes are DONE. Each event walks the tank to its
//    rune with MoveToHoldUntilInstanceData on TypeRuneActiveN (16..22, DONE=3)
//    and holds there until the douse lands, so the Domo leg can never
//    deadlock on a rune the bots missed. Condition: the rune's own boss DONE
//    and the rune not yet DONE. The MoveTo IS the arrival step (no whitelist
//    needed for the near-gate lint).
//
// 3. MAJORDOMO EXECUTUS (event 13, anchored objective at his summon point
//    758.089,-1176.71,-118.64). He is summon-only (the 7th rune douse spawns
//    him; no spawn row -> BossSpawnIndex can never emit him and his kill can
//    never flip a bit). His 8 adds (4x Flamewaker Elite 11664 + 4x Flamewaker
//    Healer 11663) are summoned by his AI; when the last one dies he goes
//    FRIENDLY (faction 35), evades and sets TYPE_MAJORDOMO DONE - he never
//    dies, so the objective must NEVER gate on his death. The event waits for
//    him to exist, then gates on both add entries being gone; the objective
//    latches when the event completes.
//
// 4. RAGNAROS (event 14, anchored objective at the Domo-copy spot
//    847.1,-816.2,-229.8). After the surrender, Domo evades back to his
//    terrace and - if left alone 28s - teleports a gossipable COPY of himself
//    to (847.1,-816.2). Gossiping that copy (3rd option = action 1003,
//    GossipSelect_event_domo) starts the scripted summon: Domo walks to
//    (839.17,-811.27,-229.59) and summons Ragnaros at (842.24,-833.68,
//    -231.92) ~28s later. The event gossips, waits for the spawn, then kills
//    him through the normal engage pipeline. The Gossip executor's
//    script-select path handles the chained menu (option 1003 >= item count ->
//    direct handler call). Both routes (terrace <-> chamber) are navmesh-
//    connected - probed, 163/170 polys.
//
// 5. SHADOWFORGE DESCENT (event 15, anchored objective at the navmesh break
//    (1035.2,-1274.8,-195.3), order key 5). Turtle's custom Sorcerer-Thane
//    Thaurissan (57642, 989,-1339,-197) sits in a navmesh POCKET: every route
//    (west camp, south camp, Sulfuron, Golemagg) funnels to a ~1yd step at
//    (1035,-1275) where the mesh holds two disconnected polygons (north
//    z -195.3 / south z -196.1). Ground navigation cannot cross it, so the
//    party is teleported across (TeleportParty: checkpoint = the north lip,
//    landing = (1035,-1280,-196.3) on the south island, which paths on to
//    Thaurissan). Anchored only - it is one-way.
//
// Owner rules applied here (see the raid plan Phase 13): dogs one pack at a
// time in Lucifron's patrol window; Luci pulled back to the party with his 2
// adds (pull-back camp to be measured in game - the BossPullbackRegistry row
// is deliberately NOT written until then); giants split-tanked; Gehennas'
// adds bursted apart; Garr's Firesworn one at a time (banish blocks the
// forced eruptions); Sulfuron's 4 Flamewaker Priests pulled out and
// interrupted; Geddon patrols like Lucifron.

namespace
{
    constexpr uint32 MC_MAP = 409;

    // Instance data (molten_core.h:37-64) and the shared DONE value
    // (ScriptedInstance.h:13-19).
    constexpr uint32 MC_DONE = 3;
    constexpr uint32 MC_TYPE_SULFURON = 0;
    constexpr uint32 MC_TYPE_GEDDON = 1;
    constexpr uint32 MC_TYPE_SHAZZRAH = 2;
    constexpr uint32 MC_TYPE_GOLEMAGG = 3;
    constexpr uint32 MC_TYPE_GARR = 4;
    constexpr uint32 MC_TYPE_MAGMADAR = 5;
    constexpr uint32 MC_TYPE_GEHENNAS = 6;
    constexpr uint32 MC_RUNE_ACTIVE_0 = 16;  // TypeRuneActive0..6 = 16..22

    constexpr uint32 MC_LUCIFRON = 12118;
    constexpr uint32 MC_CORE_HOUND = 11671;
    constexpr uint32 MC_DOMO = 12018;
    constexpr uint32 MC_RAGNAROS = 11502;
    constexpr uint32 MC_DOMO_ELITE = 11664;
    constexpr uint32 MC_DOMO_HEALER = 11663;

    // --- event ids (per map) -------------------------------------------------
    constexpr uint32 MC_EVENT_PACK_A = 1;
    constexpr uint32 MC_EVENT_PACK_B = 2;
    constexpr uint32 MC_EVENT_PACK_C = 3;
    constexpr uint32 MC_EVENT_PACK_D = 4;
    constexpr uint32 MC_EVENT_PACK_E = 5;
    constexpr uint32 MC_EVENT_RUNE_0 = 6;   // .. 12
    constexpr uint32 MC_EVENT_DOMO = 13;
    constexpr uint32 MC_EVENT_RAGNAROS = 14;
    constexpr uint32 MC_EVENT_BRIDGE = 15;
    constexpr uint32 MC_EVENT_LUCI_HOLD = 16;

    // --- clear-path keys (see DcBossEntries1121.h) ---------------------------
    constexpr uint32 MC_BRIDGE_ORDER = 5;    // between Smoldaris (key 4) and Thaurissan (key 6)
    constexpr uint32 MC_DOMO_ORDER = 13;     // after Golemagg (key 12)
    constexpr uint32 MC_RAGNAROS_ORDER = 14; // last

    // --- Core Hound packs (spawn clusters from tw_world) ---------------------
    struct McPack
    {
        float x;
        float y;
        float z;
    };

    constexpr McPack MC_PACK_A = { 993.0f, -972.0f, -181.0f };
    constexpr McPack MC_PACK_B = { 1035.0f, -962.0f, -180.0f };
    constexpr McPack MC_PACK_C = { 1078.0f, -973.0f, -184.0f };
    constexpr McPack MC_PACK_D = { 1088.0f, -1020.0f, -186.5f };
    constexpr McPack MC_PACK_E = { 1114.0f, -990.0f, -186.0f };

    // Live-hound scan around the pack centre (a pack is ~20yd across).
    constexpr float MC_PACK_LIVE_RADIUS = 35.0f;
    constexpr float MC_PACK_ZBAND = 15.0f;
    // The ClearRadius the event drives: the pack's own footprint ONLY.
    constexpr float MC_PACK_CLEAR_RADIUS = 30.0f;
    constexpr float MC_PACK_CLEAR_ZBAND = 18.0f;
    // Proximity gate: the tank has to be at the pack (no arrival step).
    constexpr float MC_PACK_APPROACH = 70.0f;    // Patrol window: Lucifron must be at least this far from the pack centre.
    // Fighting a pack with him close is an auto wipe, so the margin over his own
    // aggro + social-chain reach (~45yd worst case) is deliberate. 65yd also
    // produces the clean ALTERNATION the corridor wants, measured against his
    // 39 waypoints:
    //   * packs A (993,-972) and B (1035,-962) open while he is on the EAST leg
    //     (A 133yd / B 99yd away) and are shut while he is at the west end
    //     (A ~15yd / B ~35yd);
    //   * packs C (1078,-973), D (1088,-1020) and E (1114,-990) open while he
    //     is at the WEST end (79/107/118yd) and are shut while he is east
    //     (C ~60yd at the east cap - the tightest of the five, a ~15-20s
    //     window; a full raid kills a pack well inside it).
    constexpr float MC_PATROL_SAFE = 65.0f;
    constexpr uint32 MC_PACK_TIMEOUT = 300000;  // a 5-elite pack, generous

    // "Clear the packs before Lucifron" hold (see the McLucifronClearFirst
    // predicate). His home (his spawn, tw_world) and the radius inside which
    // the tank counts as committed to his approach - the two pre-Lucifron
    // packs sit 15-31yd from it.
    constexpr float MC_LUCIFRON_HOME_X = 1024.4f;
    constexpr float MC_LUCIFRON_HOME_Y = -973.3f;
    constexpr float MC_LUCIFRON_HOME_Z = -181.5f;
    constexpr float MC_LUCIFRON_HOLD_RANGE = 60.0f;
    constexpr uint32 MC_HOLD_WAIT_MS = 5000;

    // --- runes (GOs 176951-957; instance_molten_core.cpp:556-658) ------------
    struct McRune
    {
        float x;
        float y;
        float z;
        uint32 runeData;  // TypeRuneActiveN
        uint32 bossData;  // TYPE_* of the boss whose death opens the rune
    };

    constexpr McRune MC_RUNES[7] = {
        { 602.0f, -1175.0f, -196.0f, MC_RUNE_ACTIVE_0 + 0, MC_TYPE_SULFURON },
        { 749.0f, -985.0f, -178.0f, MC_RUNE_ACTIVE_0 + 1, MC_TYPE_GEDDON },
        { 584.0f, -807.0f, -205.0f, MC_RUNE_ACTIVE_0 + 2, MC_TYPE_SHAZZRAH },
        { 796.0f, -974.0f, -208.0f, MC_RUNE_ACTIVE_0 + 3, MC_TYPE_GOLEMAGG },
        { 694.0f, -496.0f, -214.0f, MC_RUNE_ACTIVE_0 + 4, MC_TYPE_GARR },
        { 1132.0f, -1017.0f, -186.0f, MC_RUNE_ACTIVE_0 + 5, MC_TYPE_MAGMADAR },
        { 897.0f, -552.0f, -204.0f, MC_RUNE_ACTIVE_0 + 6, MC_TYPE_GEHENNAS },
    };

    constexpr float MC_RUNE_ARRIVE_RADIUS = 12.0f;
    constexpr uint32 MC_RUNE_TIMEOUT = 300000;

    // --- Domo / Ragnaros / the bridge ----------------------------------------
    constexpr float MC_DOMO_X = 758.089f;
    constexpr float MC_DOMO_Y = -1176.71f;
    constexpr float MC_DOMO_Z = -118.640f;
    constexpr float MC_DOMO_ADD_SEARCH = 70.0f;
    constexpr uint32 MC_DOMO_SPAWN_TIMEOUT = 300000;

    constexpr float MC_RAGNAROS_X = 847.1f;
    constexpr float MC_RAGNAROS_Y = -816.2f;
    constexpr float MC_RAGNAROS_Z = -229.8f;
    constexpr float MC_GOSSIP_SEARCH = 40.0f;
    constexpr uint32 MC_GOSSIP_TIMEOUT = 120000;  // wait out the Domo-copy spawn
    constexpr uint32 MC_RAGNAROS_SPAWN_TIMEOUT = 300000;
    constexpr float MC_RAGNAROS_ENGAGE_SEARCH = 150.0f;

    // The navmesh break (mesh-probed): north lip -> south island landing.
    constexpr float MC_BRIDGE_X = 1035.2f;
    constexpr float MC_BRIDGE_Y = -1274.8f;
    constexpr float MC_BRIDGE_Z = -195.3f;
    constexpr float MC_BRIDGE_LAND_X = 1035.0f;
    constexpr float MC_BRIDGE_LAND_Y = -1280.0f;
    constexpr float MC_BRIDGE_LAND_Z = -196.3f;
    constexpr float MC_BRIDGE_RADIUS = 10.0f;

    // --- predicates -----------------------------------------------------------

    // A pack clear is due when the tank is at the pack, the pack still has live
    // hounds, the previous pack in the owner's sequence is fully dead, and
    // Lucifron is away on the far side of his loop. Proximity-gated inside the
    // predicate (this event has no arrival step) - vetted in
    // t/TestEventRegistry.cpp's near-gate whitelist.
    //
    // The owner's sequence is strict: "you have to clear two trash packs before
    // luci. you pull 1 while luci is away, and once they are all dead, you pull
    // the 2nd while away. kill all at sametime." Pack B therefore waits on pack
    // A having no live hounds left. (The ordinary pull machinery may still
    // choose its own nearest-first target when pull mode owns the tick - both
    // paths end in one pack at a time, in a window, dragged to a camp.)
    bool McPackWindow(Player* bot, AiObjectContext* context, McPack const& pack,
                      McPack const* prerequisite)
    {
        if (!bot)
            return false;

        if (bot->GetDistance(pack.x, pack.y, pack.z) > MC_PACK_APPROACH)
            return false;

        std::vector<uint32> const hounds{ MC_CORE_HOUND };
        if (!DcTargeting::NearestHostileNearPoint(bot, context, pack.x, pack.y, pack.z,
                                                  MC_PACK_LIVE_RADIUS, MC_PACK_ZBAND,
                                                  &hounds))
            return false;

        if (prerequisite &&
            DcTargeting::NearestHostileNearPoint(bot, context,
                                                 prerequisite->x, prerequisite->y,
                                                 prerequisite->z,
                                                 MC_PACK_LIVE_RADIUS, MC_PACK_ZBAND,
                                                 &hounds))
            return false;  // the previous pack still has live hounds

        // Dead or far away -> the window is open. He is a waypoint patroller
        // (movement_type 2) whose route runs straight through these packs.
        if (Creature* lucifron = DcTargeting::FindLiveCreatureOnMap(bot, MC_LUCIFRON))
        {
            if (lucifron->GetDistance(pack.x, pack.y, pack.z) < MC_PATROL_SAFE)
                return false;
        }

        return true;
    }

    bool McPackWindowA(Player* bot, AiObjectContext* context) { return McPackWindow(bot, context, MC_PACK_A, nullptr); }
    bool McPackWindowB(Player* bot, AiObjectContext* context) { return McPackWindow(bot, context, MC_PACK_B, &MC_PACK_A); }
    bool McPackWindowC(Player* bot, AiObjectContext* context) { return McPackWindow(bot, context, MC_PACK_C, nullptr); }
    bool McPackWindowD(Player* bot, AiObjectContext* context) { return McPackWindow(bot, context, MC_PACK_D, nullptr); }
    bool McPackWindowE(Player* bot, AiObjectContext* context) { return McPackWindow(bot, context, MC_PACK_E, nullptr); }

    // "CLEAR THE TWO PACKS BEFORE LUCIFRON" (owner rule). Due while Lucifron is
    // alive, the tank is committed to his approach (within MC_LUCIFRON_HOLD_RANGE
    // of his home), at least one pre-Lucifron pack still has live hounds, and
    // NEITHER pack can be worked right now (he is inside the patrol-safe radius
    // of both) - i.e. the party is waiting him out. The event is a Wait (the
    // drive's default hold), so it PREEMPTS the at-boss engage (31 > 30) and the
    // tank holds instead of pulling him with the packs still up ("if the dogs are
    // too close all will pull"). When he patrols away a pack window opens, the
    // pack event (registered earlier, checked first) takes the tick, and the
    // clear proceeds. Proximity-gated inside the predicate - whitelisted in
    // t/TestEventRegistry.cpp.
    bool McLucifronClearFirst(Player* bot, AiObjectContext* context)
    {
        if (!bot)
            return false;

        Creature* lucifron = DcTargeting::FindLiveCreatureOnMap(bot, MC_LUCIFRON);
        if (!lucifron)
            return false;  // dead -> no hold

        // Approach zone: within approach range of Pack A, Pack B, or Lucifron's home.
        bool const inApproach =
            bot->GetDistance(MC_PACK_A.x, MC_PACK_A.y, MC_PACK_A.z) <= MC_PACK_APPROACH ||
            bot->GetDistance(MC_PACK_B.x, MC_PACK_B.y, MC_PACK_B.z) <= MC_PACK_APPROACH ||
            bot->GetDistance(MC_LUCIFRON_HOME_X, MC_LUCIFRON_HOME_Y, MC_LUCIFRON_HOME_Z) <= 90.0f;

        if (!inApproach)
            return false;

        std::vector<uint32> const hounds{ MC_CORE_HOUND };
        bool const packAAlive =
            DcTargeting::NearestHostileNearPoint(bot, context, MC_PACK_A.x, MC_PACK_A.y,
                                                 MC_PACK_A.z, MC_PACK_LIVE_RADIUS,
                                                 MC_PACK_ZBAND, &hounds) != nullptr;
        bool const packBAlive =
            DcTargeting::NearestHostileNearPoint(bot, context, MC_PACK_B.x, MC_PACK_B.y,
                                                 MC_PACK_B.z, MC_PACK_LIVE_RADIUS,
                                                 MC_PACK_ZBAND, &hounds) != nullptr;
        if (!packAAlive && !packBAlive)
            return false;  // both packs cleared -> no hold

        bool const windowA =
            lucifron->GetDistance(MC_PACK_A.x, MC_PACK_A.y, MC_PACK_A.z) >= MC_PATROL_SAFE;
        bool const windowB =
            lucifron->GetDistance(MC_PACK_B.x, MC_PACK_B.y, MC_PACK_B.z) >= MC_PATROL_SAFE;

        // Strict alternation: Pack A must be cleared before Pack B.
        if (packAAlive)
            return !windowA;  // If window A is open, let Pack A event drive; if closed, hold!

        // Pack A is cleared; if Pack B is alive, hold until window B is open.
        return !windowB;
    }

    // A rune safety net is due when its boss is DONE and the rune is not yet
    // doused. The event's MoveTo is the arrival step, so no proximity gate is
    // needed (it may walk the tank to a missed rune from anywhere).
    bool McRuneUndoused(Player* bot, McRune const& rune)
    {
        if (!bot)
            return false;

        InstanceScript* inst = DcTargeting::GetInstanceScript(bot);
        if (!inst)
            return false;

        if (inst->GetData(rune.runeData) == MC_DONE)
            return false;  // already doused
        return inst->GetData(rune.bossData) == MC_DONE;
    }

    bool McRune0(Player* bot, AiObjectContext* /*context*/) { return McRuneUndoused(bot, MC_RUNES[0]); }
    bool McRune1(Player* bot, AiObjectContext* /*context*/) { return McRuneUndoused(bot, MC_RUNES[1]); }
    bool McRune2(Player* bot, AiObjectContext* /*context*/) { return McRuneUndoused(bot, MC_RUNES[2]); }
    bool McRune3(Player* bot, AiObjectContext* /*context*/) { return McRuneUndoused(bot, MC_RUNES[3]); }
    bool McRune4(Player* bot, AiObjectContext* /*context*/) { return McRuneUndoused(bot, MC_RUNES[4]); }
    bool McRune5(Player* bot, AiObjectContext* /*context*/) { return McRuneUndoused(bot, MC_RUNES[5]); }
    bool McRune6(Player* bot, AiObjectContext* /*context*/) { return McRuneUndoused(bot, MC_RUNES[6]); }
}

void RegisterMoltenCoreEvents(std::vector<DungeonEvent>& out)
{
    // 1. Core Hound packs (see the header comment). Tight ClearRadius,
    //    hounds only, one pack at a time, patrol-window gated.
    //
    //    NO PanelBeforeBoss here on purpose: panelGatesBossEntry is also read
    //    by DcTargeting::HasPendingSummonEvent / IsHoldingForSummonEvent, and
    //    tagging these rows with Lucifron/Magmadar would make the dynamic pull
    //    stand down within 80yd of their anchors for as long as a pack event is
    //    pending. The packs are cleared by this event or the ordinary pull, not
    //    by a boss summon.
    struct PackRow
    {
        uint32 id;
        McPack const* pack;
        EventCondition condition;
        char const* name;
    };
    static PackRow const packs[] = {
        { MC_EVENT_PACK_A, &MC_PACK_A, &McPackWindowA, "Clear Core Hound pack A (Lucifron's patrol area, west - pre-Lucifron)" },
        { MC_EVENT_PACK_B, &MC_PACK_B, &McPackWindowB, "Clear Core Hound pack B (Lucifron's patrol area, his home - pre-Lucifron)" },
        { MC_EVENT_PACK_C, &MC_PACK_C, &McPackWindowC, "Clear Core Hound pack C (Magmadar corridor, east)" },
        { MC_EVENT_PACK_D, &MC_PACK_D, &McPackWindowD, "Clear Core Hound pack D (Magmadar corridor, south)" },
        { MC_EVENT_PACK_E, &MC_PACK_E, &McPackWindowE, "Clear Core Hound pack E (Magmadar's approach)" },
    };
    for (PackRow const& p : packs)
    {
        out.push_back(EventBuilder(MC_MAP, p.id, p.name)
                          .Conditional(p.condition)
                          .Repeatable()
                          .ClearRadius(p.pack->x, p.pack->y, p.pack->z,
                                       MC_PACK_CLEAR_RADIUS, MC_PACK_CLEAR_ZBAND)
                              .OnlyEntries({ MC_CORE_HOUND })
                          .Timeout(MC_PACK_TIMEOUT)
                          .Build());
    }

    // 2. Rune-chain safety nets (one per rune; walk + hold until doused).
    struct RuneRow
    {
        uint32 id;
        uint32 index;
        EventCondition condition;
    };
    static RuneRow const runes[] = {
        { MC_EVENT_RUNE_0, 0, &McRune0 },
        { MC_EVENT_RUNE_0 + 1, 1, &McRune1 },
        { MC_EVENT_RUNE_0 + 2, 2, &McRune2 },
        { MC_EVENT_RUNE_0 + 3, 3, &McRune3 },
        { MC_EVENT_RUNE_0 + 4, 4, &McRune4 },
        { MC_EVENT_RUNE_0 + 5, 5, &McRune5 },
        { MC_EVENT_RUNE_0 + 6, 6, &McRune6 },
    };
    for (RuneRow const& r : runes)
    {
        McRune const& rune = MC_RUNES[r.index];
        out.push_back(EventBuilder(MC_MAP, r.id, "Douse the rune (safety net)")
                          .Conditional(r.condition)
                          .Repeatable()
                          // Group under the Domo objective in the panel. Safe to
                          // tag with the synthetic entry: the two runtime
                          // consumers of panelGatesBossEntry
                          // (HasPendingSummonEvent via advance/engage) guard on
                          // kind == Boss, and an objective never reaches them.
                          .PanelBeforeBoss(BossRosterRegistry::ObjectiveEntry(2))
                          .MoveToHoldUntilInstanceData(rune.x, rune.y, rune.z,
                                                       MC_RUNE_ARRIVE_RADIUS,
                                                       rune.runeData, MC_DONE)
                          .Timeout(MC_RUNE_TIMEOUT)
                          .Build());
    }

    // 3. Majordomo Executus: wait for the summon, then gate on both add
    //    entries being gone. He goes FRIENDLY, he never dies - the objective
    //    latches on the event completing, never on his death.
    out.push_back(EventBuilder(MC_MAP, MC_EVENT_DOMO, "Majordomo Executus (defeat his 8 adds)")
                      .Anchored(MC_DOMO_ORDER)
                      .Persistent()
                      .WaitForSpawn(MC_DOMO, /*wantAlive*/ true)
                          .Timeout(MC_DOMO_SPAWN_TIMEOUT)
                      .KillCreature(MC_DOMO_ELITE, /*count*/ 1, MC_DOMO_ADD_SEARCH)
                          .Timeout(MC_DOMO_SPAWN_TIMEOUT)
                      .KillCreature(MC_DOMO_HEALER, /*count*/ 1, MC_DOMO_ADD_SEARCH)
                          .Timeout(MC_DOMO_SPAWN_TIMEOUT)
                      .Build());

    // 4. Ragnaros: gossip the friendly Domo copy (option 1003), wait for the
    //    summon, then engage and kill.
    out.push_back(EventBuilder(MC_MAP, MC_EVENT_RAGNAROS, "Summon and kill Ragnaros (Domo gossip)")
                      .Anchored(MC_RAGNAROS_ORDER)
                      .Persistent()
                      .Gossip(MC_DOMO, /*option*/ 1003, MC_GOSSIP_SEARCH)
                          // The friendly Domo COPY appears ~28s after his
                          // surrender (and the party walks ~370yd to get here),
                          // so give the approach room well past the 30s default.
                          .Timeout(MC_GOSSIP_TIMEOUT)
                      .WaitForSpawn(MC_RAGNAROS, /*wantAlive*/ true)
                          .Timeout(MC_RAGNAROS_SPAWN_TIMEOUT)
                      .KillCreatureEngage(MC_RAGNAROS, /*count*/ 1, MC_RAGNAROS_ENGAGE_SEARCH)
                          .Timeout(MC_RAGNAROS_SPAWN_TIMEOUT)
                      .Build());

    // 5. Shadowforge Descent: teleport the party across the navmesh break
    //    into Thaurissan's pocket (one-way; see the header comment).
    out.push_back(EventBuilder(MC_MAP, MC_EVENT_BRIDGE, "Shadowforge Descent (bridge to Thaurissan)")
                      .Anchored(MC_BRIDGE_ORDER)
                      .Persistent()
                      .TeleportParty(MC_BRIDGE_X, MC_BRIDGE_Y, MC_BRIDGE_Z,
                                     MC_BRIDGE_LAND_X, MC_BRIDGE_LAND_Y, MC_BRIDGE_LAND_Z,
                                     MC_BRIDGE_RADIUS)
                      .Build());

    // 6. "Clear the two packs before Lucifron" hold (see McLucifronClearFirst).
    //    Registered LAST so the pack events (earlier ids) always win the tick
    //    when a window is open - their conditions are mutually exclusive, so this
    //    only ever runs while both packs are waiting him out. The Wait step is
    //    the hold: the drive's default StopBot keeps the tank planted, and the
    //    event's relevance (31) preempts the at-boss engage (30).
    out.push_back(EventBuilder(MC_MAP, MC_EVENT_LUCI_HOLD, "Hold: clear the two Core Hound packs before Lucifron")
                      .Conditional(&McLucifronClearFirst)
                      .Repeatable()
                      .Wait(MC_HOLD_WAIT_MS)
                      .Build());
}

// --- roster patch ---------------------------------------------------------
void RegisterMoltenCoreRoster(std::vector<BossRosterPatch>& t)
{
    using namespace DcRoster;

    // The 2 Flamewaker Protectors (12119) are Lucifron's ADDS. The curated
    // credit list carries 12119 with no order row, so without this they are
    // numbered past the authored block and routed as a boss leg of their own
    // (and the Lucifron fight already covers them). Drop them.
    {
        BossRosterPatch p;
        p.mapId = MC_MAP;
        p.remove = { 12119 };
        t.push_back(std::move(p));
    }

    // Three travel objectives (see the events above):
    //  * the Shadowforge Descent bridge - order key 5, between the Twin Golems
    //    and Thaurissan;
    //  * Majordomo (key 13) and Ragnaros (key 14) - both summon-only, no spawn
    //    rows, so neither can ever be credited by BossSpawnIndex.
    {
        BossRosterPatch p;
        p.mapId = MC_MAP;
        p.add = {
            MakeObjective(BossRosterRegistry::ObjectiveEntry(1), MC_BRIDGE_ORDER,
                          MC_MAP, "Shadowforge Descent (Thaurissan bridge)",
                          MC_BRIDGE_X, MC_BRIDGE_Y, MC_BRIDGE_Z,
                          /*arriveRadius*/ 8.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ MC_EVENT_BRIDGE,
                          /*orderOverride*/ MC_BRIDGE_ORDER),
            MakeObjective(BossRosterRegistry::ObjectiveEntry(2), MC_DOMO_ORDER,
                          MC_MAP, "Majordomo Executus",
                          MC_DOMO_X, MC_DOMO_Y, MC_DOMO_Z,
                          /*arriveRadius*/ 12.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ MC_EVENT_DOMO,
                          /*orderOverride*/ MC_DOMO_ORDER),
            MakeObjective(BossRosterRegistry::ObjectiveEntry(3), MC_RAGNAROS_ORDER,
                          MC_MAP, "Ragnaros (Domo gossip)",
                          MC_RAGNAROS_X, MC_RAGNAROS_Y, MC_RAGNAROS_Z,
                          /*arriveRadius*/ 12.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ MC_EVENT_RAGNAROS,
                          /*orderOverride*/ MC_RAGNAROS_ORDER),
        };
        t.push_back(std::move(p));
    }
}
