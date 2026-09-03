/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

// --- Dragonmaw Retreat (map 816) — the LEDGE down to the lower cave ---
//
// Turtle custom dungeon. Every boss past Gowlfang lives in a cave system that
// hangs ~150yd BELOW the entrance level, and the only way in is over a ledge:
// the walk down ends on a lip at z~221 and the next floor starts at z~214, a
// few yards across open air. Recast bakes no off-mesh link there, so stock
// boss-nav calls Web Master Torkon (62067, z 72.6) unreachable and the clear
// stalls with the party strung out along the descent.
//
// Measured 2026-08-31, ten parties over an afternoon: every run ended at 2 of
// 11 bosses (Bogpaw + Gowlfang, which stand 3.7yd apart at the top), nobody
// ever below z 240, and the followers spread over 80 vertical yards while the
// tank waited on a spread gate they could not close. Four separate follower
// mechanisms were measured and none was the cause - the dungeon simply had no
// event file, alone among the 29 that have one.
//
// Same shape as the Serpentis drop in WailingCavernsEvents.cpp, and for the
// same reasons:
//
//   ANCHORED   - an OBJECTIVE anchor sits on the LIP, which is approach-side
//                mesh and therefore reachable. Boss-nav drives the tank there
//                like to any boss; the event then jumps the one off-mesh leg.
//
//   PERSISTENT - the drop is ONE-WAY. A non-persistent anchored event rewinds
//                to step 0 after any >1s Drive gap (landing among trash is
//                enough), and that rewind would re-run MoveTo(lip) and walk the
//                bot back toward a lip it can no longer reach from below.
//
//   TWO STEPS  - settle exactly on the lip first (the objective's arrive radius
//                can leave the tank several yards off, and the leap is only
//                11.6yd across), then jump. The Jump step is idempotent: it is
//                done once the bot is within radius of the landing.
//
// Coordinates are from in-game observation (a GM stood at the ledge and read
// them off): 11.6yd across, 7.4yd down. That is comfortably inside what a
// run-speed MoveJump covers - Wailing Caverns' proven leap is 9.4 across and
// 5.6 down - and far short of the 148yd fall a straight drop from this lip
// would be.
//
// NOT verified yet: whether the FOLLOWERS make the leap. There is no jump
// handling anywhere in DcFollowerActions - the followers ride breadcrumb
// splines - so they presumably arrive by running off the lip and taking the
// 7.4yd fall, which is what they must already do at Wailing Caverns. If they
// strand on the near side instead, this is the spot for a TeleportParty step
// (see DungeonEventRegistry.h), which exists for exactly a one-way drop whose
// two sides are disconnected mesh islands.

void RegisterDragonmawEvents(std::vector<DungeonEvent>& out)
{
    // The lip the party leaps FROM (end of the walked descent, approach-side
    // mesh, where the objective anchor sits) and the shelf it lands ON, from
    // which stairs carry on down to Torkon.
    constexpr float DM_LIP_X  = -6192.31f;
    constexpr float DM_LIP_Y  = -3954.77f;
    constexpr float DM_LIP_Z  =   220.96f;
    constexpr float DM_LAND_X = -6193.41f;
    constexpr float DM_LAND_Y = -3966.28f;
    constexpr float DM_LAND_Z =   213.55f;

    out.push_back(
        EventBuilder(816, 1, "Drop into the lower cave")
            .Anchored(/*orderIndex*/ 2)
            .Persistent()
            // 1. Settle on the lip. Radius 3, not the objective's 6: at 6 the
            //    tank can stand far enough off-line that an 11.6yd leap misses.
            .MoveTo(DM_LIP_X, DM_LIP_Y, DM_LIP_Z, /*radius*/ 3.0f)
            // 2. Leap the gap onto the shelf. Done once landed; stock nav then
            //    walks the stairs down to Torkon.
            .Jump(DM_LAND_X, DM_LAND_Y, DM_LAND_Z, /*radius*/ 5.0f)
            .Build());
}

void RegisterDragonmawRoster(std::vector<BossRosterPatch>& t)
{
    using namespace DcRoster;

    // The boss ORDER for this map is data, not code: it lives in
    // data/dc_roster.txt as `order 816 <entry> <n>` lines, walked and dictated
    // by the server owner (Bogpaw 1, Gowlfang 2, Torkon 3, Broodmother 4, ...).
    // This patch adds only the travel OBJECTIVE, keyed into that same 1..11
    // scale via orderOverride so the two orderings stay in one system.
    //
    // Key 2, shared with Torkon. NOTE THE OFF-BY-ONE, it cost a build: the
    // roster FILE counts from 1 (`order 816 62067 3` for Torkon) while the
    // internal sort keys count from 0, so Torkon lands on key 2, not 3. Keyed
    // to 3 the objective sorted onto the Broodmother's slot - BEHIND the ledge -
    // and the party walked at an unreachable Torkon instead. The roster dump in
    // DungeonBossesValue prints the resolved keys; read it, do not count them.
    //
    // At an equal key an Objective sorts BEFORE a Boss (BossRosterRegistry's
    // tie-break), so the party is driven to the lip, drops, and only then goes
    // looking for Torkon - who is on the far side and unreachable until it has.
    BossRosterPatch p;
    p.mapId = 816;
    p.add = {
        MakeObjective(OBJ(1), /*encounterIndex*/ 2, 816,
                      "Drop into the lower cave",
                      -6192.31f, -3954.77f, 220.96f, /*arriveRadius*/ 6.0f,
                      /*gateEntry*/ 0, /*hook*/ 0, /*eventId*/ 1,
                      /*orderOverride*/ 2),
    };
    t.push_back(std::move(p));
}
