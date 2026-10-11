/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license, you may redistribute it and/or modify it under version 3 of
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

// --- Scarlet Citadel (map 45) ---------------------------------------------
//
// Custom 40-man / 10-man Scarlet Crusade raid fortress (Turtle-WoW content).
// 1. Entrance / Courtyard: Sacred Fist Daelus (2000000) at (67.39, 13.31, 16.87).
//    Starts kneeling and neutral. Courtyard trash (Valiants, Footmen, Inquisitors,
//    Jordan) is pre-cleared. Gossiping begins encounter. Fallen Spirits (2000013)
//    spawn continuously; the Red spirit grants vulnerability while normal spirits
//    heal/buff Daelus. Poison Cloud (16363) emitters must be vacated.
//    Defeating Daelus opens doors 5000001, 5000002, and 5000003.
//
// 2. Statue Hall: Grand Sorcerer Ardaeus (2000001) at (283.91, -116.89, 16.87).
//    Starts neutral. Statue hall trash is pre-cleared. Gossip begins encounter.
//    Summons the Sun (2000019) which descends from the roof; raid must deal damage
//    to keep the Sun elevated or face a raid wipe on touchdown. Ardaeus also awakens
//    statue echoes (2000020-2000030) during "Call for Help" phases.
//    Defeating Ardaeus opens exit door 5000007.
//
// 3. Inquisitor Sanctum: High Inquisitor Mariella (2000002) at (188.30, 48.57, 32.84).
//    Gossip begins encounter. Sanctum guards pre-cleared. Summons Void Zones (2000016),
//    Kill Zones (2000018), and Felhound mana drain adds (2000017) which must be
//    rapidly prioritized by DPS. Defeating Mariella unlocks Right Wing door 5000009.
//
// 4. Caster's Nightmare / Gauntlet: Brother Eric Vesper (2000004) at (128.85, -73.62, 15.99).
//    Areatrigger summons Citadel Inquisitors (2000033), Valiants (2000034), and
//    Footmen (2000035). Eric Vesper channels Energize (25685) from 0% to 100% mana;
//    reaching full mana triggers DoExplosion (fatal raid wipe). Raid must burn down
//    adds and execute high burst DPS on Vesper.
//
// 5. High General's Quarters: High General Abbendis (2000003) at (420.76, -120.07, 35.75).
//    Guarded by Darkcaller Rayn (2000037) and elite citadel defenders. Heavy frontal
//    Crusader Strikes and Holy Cleave. Defeating Abbendis completes the raid.

namespace {
constexpr uint32 SC_MAP = 45;

// Boss entries
constexpr uint32 SC_DAELUS = 2000000;
constexpr uint32 SC_ARDAEUS = 2000001;
constexpr uint32 SC_MARIELLA = 2000002;
constexpr uint32 SC_ERIC_VESPER = 2000004;
constexpr uint32 SC_ABBENDIS = 2000003;

// Trash & Add entries
constexpr uint32 SC_CITADEL_JORDAN = 2000011;
constexpr uint32 SC_FALLEN_SPIRIT = 2000013;
constexpr uint32 SC_POISON_CLOUD = 16363;
constexpr uint32 SC_SUN = 2000019;
constexpr uint32 SC_VOIDZONE = 2000016;
constexpr uint32 SC_FELHOUND = 2000017;
constexpr uint32 SC_KILLZONE = 2000018;
constexpr uint32 SC_CITADEL_INQUISITOR = 2000033;
constexpr uint32 SC_CITADEL_VALIANT = 2000034;
constexpr uint32 SC_CITADEL_FOOTMAN = 2000035;
constexpr uint32 SC_CITADEL_INTERROGATOR = 2000036;
constexpr uint32 SC_DARKCALLER_RAYN = 2000037;

// Encounter Order matching dc_roster.txt and DcBossEntries1121.h (key = order - 1)
constexpr uint32 SC_ORDER_DAELUS = 0;       // Order 1 -> key 0
constexpr uint32 SC_ORDER_ARDAEUS = 1;      // Order 2 -> key 1
constexpr uint32 SC_ORDER_MARIELLA = 2;     // Order 3 -> key 2
constexpr uint32 SC_ORDER_ERIC_VESPER = 3;  // Order 4 -> key 3
constexpr uint32 SC_ORDER_ABBENDIS = 4;     // Order 5 -> key 4

// Event IDs
constexpr uint32 SC_EVENT_DAELUS = 1;
constexpr uint32 SC_EVENT_ARDAEUS = 2;
constexpr uint32 SC_EVENT_MARIELLA = 3;
constexpr uint32 SC_EVENT_ERIC_VESPER = 4;
constexpr uint32 SC_EVENT_ABBENDIS = 5;
constexpr uint32 SC_EVENT_ROOM_PRECLEAR = 6;

// Spawn Coordinates
constexpr float SC_DAELUS_X = 67.3897f;
constexpr float SC_DAELUS_Y = 13.3098f;
constexpr float SC_DAELUS_Z = 16.8691f;

constexpr float SC_ARDAEUS_X = 283.910f;
constexpr float SC_ARDAEUS_Y = -116.886f;
constexpr float SC_ARDAEUS_Z = 16.8717f;

constexpr float SC_MARIELLA_X = 188.304f;
constexpr float SC_MARIELLA_Y = 48.5719f;
constexpr float SC_MARIELLA_Z = 32.8437f;

constexpr float SC_ERIC_VESPER_X = 128.850f;
constexpr float SC_ERIC_VESPER_Y = -73.620f;
constexpr float SC_ERIC_VESPER_Z = 15.990f;

constexpr float SC_ABBENDIS_X = 420.760f;
constexpr float SC_ABBENDIS_Y = -120.074f;
constexpr float SC_ABBENDIS_Z = 35.7493f;
} // namespace

void RegisterScarletCitadelEvents(std::vector<DungeonEvent> &out) {
  // 1. Sacred Fist Daelus: Courtyard pre-clear, gossip, handle Fallen Spirits, defeat boss.
  out.push_back(EventBuilder(SC_MAP, SC_EVENT_DAELUS,
                             "Courtyard Pre-Clear & Sacred Fist Daelus")
                    .Anchored(SC_ORDER_DAELUS)
                    .Persistent()
                    .ClearRadius(SC_DAELUS_X, SC_DAELUS_Y, SC_DAELUS_Z, 45.0f)
                    .OnlyEntries({SC_CITADEL_VALIANT, SC_CITADEL_FOOTMAN,
                                  SC_CITADEL_INQUISITOR, SC_CITADEL_INTERROGATOR,
                                  SC_CITADEL_JORDAN})
                    .Timeout(180000)
                    .Gossip(SC_DAELUS, 0)
                    .Timeout(30000)
                    .KillCreatureEngage(SC_FALLEN_SPIRIT, 0, 50.0f)
                    .Timeout(60000)
                    .KillCreatureEngage(SC_DAELUS, 1, 80.0f)
                    .Timeout(600000)
                    .Build());

  // 2. Grand Sorcerer Ardaeus: Statue Hall pre-clear, gossip, keep Sun airborne, defeat boss.
  out.push_back(EventBuilder(SC_MAP, SC_EVENT_ARDAEUS,
                             "Statue Hall Pre-Clear & Grand Sorcerer Ardaeus")
                    .Anchored(SC_ORDER_ARDAEUS)
                    .Persistent()
                    .ClearRadius(SC_ARDAEUS_X, SC_ARDAEUS_Y, SC_ARDAEUS_Z, 40.0f)
                    .OnlyEntries({SC_CITADEL_INQUISITOR, SC_CITADEL_VALIANT,
                                  SC_CITADEL_FOOTMAN, SC_CITADEL_INTERROGATOR})
                    .Timeout(180000)
                    .Gossip(SC_ARDAEUS, 0)
                    .Timeout(30000)
                    .KillCreatureEngage(SC_SUN, 0, 60.0f)
                    .Timeout(60000)
                    .KillCreatureEngage(SC_ARDAEUS, 1, 80.0f)
                    .Timeout(600000)
                    .Build());

  // 3. High Inquisitor Mariella: Sanctum pre-clear, gossip, burn Felhound mana drains, defeat boss.
  out.push_back(EventBuilder(SC_MAP, SC_EVENT_MARIELLA,
                             "Sanctum Pre-Clear & High Inquisitor Mariella")
                    .Anchored(SC_ORDER_MARIELLA)
                    .Persistent()
                    .ClearRadius(SC_MARIELLA_X, SC_MARIELLA_Y, SC_MARIELLA_Z, 35.0f)
                    .OnlyEntries({SC_CITADEL_INQUISITOR, SC_CITADEL_VALIANT,
                                  SC_CITADEL_FOOTMAN})
                    .Timeout(180000)
                    .Gossip(SC_MARIELLA, 0)
                    .Timeout(30000)
                    .KillCreatureEngage(SC_FELHOUND, 0, 50.0f)
                    .Timeout(60000)
                    .KillCreatureEngage(SC_MARIELLA, 1, 80.0f)
                    .Timeout(600000)
                    .Build());

  // 4. Brother Eric Vesper: Gauntlet adds clear and burst burn before Energize wipe.
  out.push_back(EventBuilder(SC_MAP, SC_EVENT_ERIC_VESPER,
                             "Gauntlet Ambush & Brother Eric Vesper")
                    .Anchored(SC_ORDER_ERIC_VESPER)
                    .Persistent()
                    .KillCreatureEngage(SC_CITADEL_INQUISITOR, 0, 45.0f)
                    .Timeout(60000)
                    .KillCreatureEngage(SC_CITADEL_VALIANT, 0, 45.0f)
                    .Timeout(60000)
                    .KillCreatureEngage(SC_ERIC_VESPER, 1, 60.0f)
                    .Timeout(300000)
                    .Build());

  // 5. High General Abbendis: Quarters pre-clear, Darkcaller Rayn add switch, defeat boss.
  out.push_back(EventBuilder(SC_MAP, SC_EVENT_ABBENDIS,
                             "High General Quarters & Abbendis")
                    .Anchored(SC_ORDER_ABBENDIS)
                    .Persistent()
                    .ClearRadius(SC_ABBENDIS_X, SC_ABBENDIS_Y, SC_ABBENDIS_Z, 45.0f)
                    .Timeout(180000)
                    .KillCreatureEngage(SC_DARKCALLER_RAYN, 0, 50.0f)
                    .Timeout(90000)
                    .KillCreatureEngage(SC_ABBENDIS, 1, 80.0f)
                    .Timeout(600000)
                    .Build());

  // 6. Generic room-aggro pre-clear for Scarlet Citadel bosses
  out.push_back(
      EventBuilder(SC_MAP, SC_EVENT_ROOM_PRECLEAR,
                   "Clear the room before the room-aggro boss (Scarlet Citadel)")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());
}

void RegisterScarletCitadelRoster(std::vector<BossRosterPatch> &t) {
  using namespace DcRoster;

  // Configure Scarlet Citadel bosses on map 45.
  // Daelus, Ardaeus, and Mariella start neutral with gossip flags, so
  // we tie their roster objectives directly to their gossip events so the
  // tank steps in, triggers the dialogue, and transitions into combat.
  // Brother Eric Vesper and High General Abbendis are standard bosses.
  {
    BossRosterPatch p;
    p.mapId = SC_MAP;
    p.add = {
        MakeObjective(BossRosterRegistry::ObjectiveEntry(1), SC_ORDER_DAELUS,
                      SC_MAP, "Sacred Fist Daelus (Gossip)", SC_DAELUS_X,
                      SC_DAELUS_Y, SC_DAELUS_Z,
                      /*arriveRadius*/ 15.0f, /*gateEntry*/ 0,
                      /*hook*/ 0, /*eventId*/ SC_EVENT_DAELUS,
                      /*orderOverride*/ SC_ORDER_DAELUS),
        MakeObjective(BossRosterRegistry::ObjectiveEntry(2), SC_ORDER_ARDAEUS,
                      SC_MAP, "Grand Sorcerer Ardaeus (Gossip)", SC_ARDAEUS_X,
                      SC_ARDAEUS_Y, SC_ARDAEUS_Z,
                      /*arriveRadius*/ 15.0f, /*gateEntry*/ 0,
                      /*hook*/ 0, /*eventId*/ SC_EVENT_ARDAEUS,
                      /*orderOverride*/ SC_ORDER_ARDAEUS),
        MakeObjective(BossRosterRegistry::ObjectiveEntry(3), SC_ORDER_MARIELLA,
                      SC_MAP, "High Inquisitor Mariella (Gossip)",
                      SC_MARIELLA_X, SC_MARIELLA_Y, SC_MARIELLA_Z,
                      /*arriveRadius*/ 15.0f, /*gateEntry*/ 0,
                      /*hook*/ 0, /*eventId*/ SC_EVENT_MARIELLA,
                      /*orderOverride*/ SC_ORDER_MARIELLA),
        MakeBoss(SC_ERIC_VESPER, SC_MAP, "Brother Eric Vesper", SC_ERIC_VESPER_X,
                 SC_ERIC_VESPER_Y, SC_ERIC_VESPER_Z,
                 /*completionFrom*/ SC_ERIC_VESPER,
                 /*orderOverride*/ SC_ORDER_ERIC_VESPER),
        MakeBoss(SC_ABBENDIS, SC_MAP, "High General Abbendis", SC_ABBENDIS_X,
                 SC_ABBENDIS_Y, SC_ABBENDIS_Z,
                 /*completionFrom*/ SC_ABBENDIS,
                 /*orderOverride*/ SC_ORDER_ABBENDIS)};
    t.push_back(std::move(p));
  }
}
