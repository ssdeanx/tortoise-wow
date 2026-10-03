/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#ifndef _PLAYERBOT_DCTESTDUNGEONREGISTRY_H
#define _PLAYERBOT_DCTESTDUNGEONREGISTRY_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "TestRun/DcTestComp.h"

// Hand-authored catalogue of the dungeons the `.dc test` harness can run:
// every map with a curated clear definition (roster patch / event file), one
// row per *enterable unit*. Split-wing maps whose wings are physically
// isolated (Dire Maul, Scarlet Monastery) get one row per wing — the wing a
// run covers is decided by where the party stands, so each wing needs its own
// token and entrance. Maraudon's wings interconnect, so it stays one row.
//
// Entrance coordinates are the world-DB areatrigger_teleport targets (the
// point just inside the instance portal) — safely on the navmesh, where a
// walked-in party would stand. Static vanilla/TBC data, so it lives in code
// like the module's other registries (wings, room-aggro, routes).

namespace DcTestDungeonRegistry
{
    struct Row
    {
        char const* token;        // command token: ".dc test start <token>"
        char const* name;         // human-readable dungeon (wing) name
        std::uint32_t mapId;
        float x, y, z, o;         // entrance teleport target
        std::uint32_t recommendedLevel;  // default bot level for the run
        char const* wing;         // wing label for split maps, "" otherwise
        // Default bot level for a HEROIC run, and the "heroic offered" flag in
        // one: 0 = no heroic mode for this row. TBC rows carry 70, WotLK rows
        // 80. Classic dungeons have no heroic mode at all, so they stay 0 and
        // this stays a trailing member — those rows need no edit
        // (value-initialized).
        std::uint32_t heroicLevel = 0;
        // How many slots a run of this row fields. 0 — the value every shipped
        // row carries — means DcTestComp::kPartySize (5), i.e. the party shape
        // that has always run, so no row has to be edited to keep working.
        //
        // A nonzero value asks for that many. It is validated against the map's
        // own capacity before anything is created (PlayerLimit / DcTestRunManager),
        // it decides how many names a `party=` roster must carry, and it crosses
        // MAX_GROUP_SIZE the group is converted to a RAID at Grouping — which
        // changes DC's leader election (see DcLeaderSignal::FindLeaderTank) and
        // nothing else. 0 is deliberately not "1": a row that forgets the field
        // must reproduce the old five-slot run, not fail.
        std::uint32_t partySize = 0;

        // The size to actually build: partySize, or the default five when unset.
        std::size_t PartySize() const { return partySize ? partySize : DcTestComp::kPartySize; }
    };

    // Row for a command argument: exact token match, or a numeric mapId when
    // that map has exactly one row (wing-split maps must be named by token —
    // a bare "429" cannot say which Dire Maul wing). nullptr when unknown.
    Row const* Find(std::string const& tokenOrMapId);

    std::vector<Row> const& All();

    // How many players mapId's copy of a dungeon holds — map_template.player_limit,
    // i.e. exactly the number the core's own instance cap is built from
    // (DungeonMap::GetMaxPlayers, read through sMapStorage; DungeonMap::CanEnter
    // refuses the player past it with TRANSFER_ABORT_MAX_PLAYERS).
    //
    // 0 means the world has no map_template row for that id. That is NOT "the map
    // holds nobody": CreateInstance would dereference the same null entry, so an
    // unknown map is left for the core to complain about rather than refused here
    // with a capacity claim nobody can verify. Callers must treat 0 as "no cap to
    // check", not as "refuse everything".
    std::uint32_t PlayerLimit(std::uint32_t mapId);

    // Dump the catalogue (plus the test-run caps the dashboard's start form
    // needs) to dc_test_dungeons.json in the worldserver cwd — the same
    // sidecar pattern as the live/record files (env override
    // DC_TEST_DUNGEONS_FILE). Written once at the first world tick; the
    // dashboard serves it via /api/testdungeons.
    void WriteSidecar();
}

#endif  // _PLAYERBOT_DCTESTDUNGEONREGISTRY_H
