/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "Creature.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"
#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"

// --- Scarlet Citadel (map 45) ---------------------------------------------
//
// Custom 40-man / 10-man Scarlet Crusade raid fortress (Turtle-WoW content).
// 1. Entrance / Courtyard: Sacred Fist Daelus (2000000) at (67.39, 13.31, 16.87).
//    Starts kneeling and neutral. Gossiping begins encounter. Must be tanked
//    within 3yd to prevent raid-wide life drain channel (21157). Red spirit triggers
//    30s vulnerability window. Defeating Daelus opens doors 5000001 and 5000003.
// 2. Caster's Nightmare / Gauntlet: Brother Eric Vesper (2000004) at (128.85, -73.62, 15.99).
//    Channels Energize (25685); must be burned before mana reaches 100% (DoExplosion wipe).
// 3. Statue Hall: Grand Sorcerer Ardaeus (2000001) at (283.91, -116.89, 16.87).
//    Starts neutral. Gossip begins encounter. Summons Sun (2000019) which must be damaged
//    to stay airborne, plus 11 statue echoes. Defeating Ardaeus opens exit door 5000007.
// 4. Inquisitor Sanctum: High Inquisitor Mariella (2000002) at (188.30, 48.57, 32.84).
//    Gossip begins encounter. Void Zones (2000016), Felhound mana drain waves (2000017),
//    and Shadow Volley (21341). Defeating Mariella unlocks Right Wing door 5000009.
// 5. High General's Quarters: High General Abbendis (2000003) at (420.76, -120.07, 35.75).
//    Final boss encounter. Frontal Crusader Strikes, Holy Cleave, and Scarlet reinforcements.

namespace
{
    constexpr uint32 SC_MAP = 45;

    // Boss entries
    constexpr uint32 SC_DAELUS      = 2000000;
    constexpr uint32 SC_ARDAEUS     = 2000001;
    constexpr uint32 SC_MARIELLA    = 2000002;
    constexpr uint32 SC_ABBENDIS    = 2000003;
    constexpr uint32 SC_ERIC_VESPER = 2000004;

    // Encounter Order
    constexpr uint32 SC_ORDER_DAELUS   = 1;
    constexpr uint32 SC_ORDER_ARDAEUS  = 2;
    constexpr uint32 SC_ORDER_MARIELLA = 3;
    constexpr uint32 SC_ORDER_ABBENDIS = 4;

    // Event IDs
    constexpr uint32 SC_EVENT_DAELUS   = 1;
    constexpr uint32 SC_EVENT_ARDAEUS  = 2;
    constexpr uint32 SC_EVENT_MARIELLA = 3;
    constexpr uint32 SC_EVENT_ABBENDIS = 4;

    // Spawn Coordinates
    constexpr float SC_DAELUS_X   = 67.3897f;
    constexpr float SC_DAELUS_Y   = 13.3098f;
    constexpr float SC_DAELUS_Z   = 16.8691f;

    constexpr float SC_ARDAEUS_X  = 283.910f;
    constexpr float SC_ARDAEUS_Y  = -116.886f;
    constexpr float SC_ARDAEUS_Z  = 16.8717f;

    constexpr float SC_MARIELLA_X = 188.304f;
    constexpr float SC_MARIELLA_Y = 48.5719f;
    constexpr float SC_MARIELLA_Z = 32.8437f;

    constexpr float SC_ABBENDIS_X = 420.760f;
    constexpr float SC_ABBENDIS_Y = -120.074f;
    constexpr float SC_ABBENDIS_Z = 35.7493f;
}

void RegisterScarletCitadelEvents(std::vector<DungeonEvent>& out)
{
    // 1. Sacred Fist Daelus: approach, gossip, and defeat.
    out.push_back(EventBuilder(SC_MAP, SC_EVENT_DAELUS, "Gossip and defeat Sacred Fist Daelus")
                      .Anchored(SC_ORDER_DAELUS)
                      .Persistent()
                      .Gossip(SC_DAELUS, 0)
                          .Timeout(30000)
                      .KillCreatureEngage(SC_DAELUS, 1, 80.0f)
                          .Timeout(600000)
                      .Build());

    // 2. Grand Sorcerer Ardaeus: approach, gossip, and defeat.
    out.push_back(EventBuilder(SC_MAP, SC_EVENT_ARDAEUS, "Gossip and defeat Grand Sorcerer Ardaeus")
                      .Anchored(SC_ORDER_ARDAEUS)
                      .Persistent()
                      .Gossip(SC_ARDAEUS, 0)
                          .Timeout(30000)
                      .KillCreatureEngage(SC_ARDAEUS, 1, 80.0f)
                          .Timeout(600000)
                      .Build());

    // 3. High Inquisitor Mariella: approach, gossip, and defeat.
    out.push_back(EventBuilder(SC_MAP, SC_EVENT_MARIELLA, "Gossip and defeat High Inquisitor Mariella")
                      .Anchored(SC_ORDER_MARIELLA)
                      .Persistent()
                      .Gossip(SC_MARIELLA, 0)
                          .Timeout(30000)
                      .KillCreatureEngage(SC_MARIELLA, 1, 80.0f)
                          .Timeout(600000)
                      .Build());

    // 4. High General Abbendis: engage and defeat final boss.
    out.push_back(EventBuilder(SC_MAP, SC_EVENT_ABBENDIS, "Engage and defeat High General Abbendis")
                      .Anchored(SC_ORDER_ABBENDIS)
                      .Persistent()
                      .KillCreatureEngage(SC_ABBENDIS, 1, 80.0f)
                          .Timeout(600000)
                      .Build());
}

void RegisterScarletCitadelRoster(std::vector<BossRosterPatch>& t)
{
    using namespace DcRoster;

    // Configure Scarlet Citadel bosses on map 45.
    // Daelus, Ardaeus, and Mariella start neutral with gossip flags, so
    // we tie their roster objectives directly to their gossip events so the
    // tank steps in, triggers the dialogue, and transitions into combat.
    {
        BossRosterPatch p;
        p.mapId = SC_MAP;
        p.add = {
            MakeObjective(BossRosterRegistry::ObjectiveEntry(1), SC_ORDER_DAELUS,
                          SC_MAP, "Sacred Fist Daelus (Gossip)",
                          SC_DAELUS_X, SC_DAELUS_Y, SC_DAELUS_Z,
                          /*arriveRadius*/ 15.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ SC_EVENT_DAELUS,
                          /*orderOverride*/ SC_ORDER_DAELUS),
            MakeObjective(BossRosterRegistry::ObjectiveEntry(2), SC_ORDER_ARDAEUS,
                          SC_MAP, "Grand Sorcerer Ardaeus (Gossip)",
                          SC_ARDAEUS_X, SC_ARDAEUS_Y, SC_ARDAEUS_Z,
                          /*arriveRadius*/ 15.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ SC_EVENT_ARDAEUS,
                          /*orderOverride*/ SC_ORDER_ARDAEUS),
            MakeObjective(BossRosterRegistry::ObjectiveEntry(3), SC_ORDER_MARIELLA,
                          SC_MAP, "High Inquisitor Mariella (Gossip)",
                          SC_MARIELLA_X, SC_MARIELLA_Y, SC_MARIELLA_Z,
                          /*arriveRadius*/ 15.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ SC_EVENT_MARIELLA,
                          /*orderOverride*/ SC_ORDER_MARIELLA),
            MakeBoss(SC_ABBENDIS, SC_MAP, "High General Abbendis",
                     SC_ABBENDIS_X, SC_ABBENDIS_Y, SC_ABBENDIS_Z,
                     /*completionFrom*/ SC_ABBENDIS,
                     /*orderOverride*/ SC_ORDER_ABBENDIS)
        };
        t.push_back(std::move(p));
    }
}
