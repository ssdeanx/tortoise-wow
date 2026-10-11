/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license; you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "Ai/Dungeon/DungeonClear/Data/DungeonWingRegistry.h"
#include "Ai/Dungeon/DungeonClear/Overrides/BossRosterRegistry.h"
#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"
#include "Creature.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "Player.h"
#include "Playerbots.h"

// --- Blackrock Spire / UBRS (map 229) --------------------------------------
//
// 10-player endgame instance (Upper Blackrock Spire).
// Three major scripted set-pieces defined in instance_blackrock_spire.cpp:
//
// 1. HALL OF BINDING (TYPE_ROOM_EVENT):
//    The 7 rune alcoves surrounding the Hall of Binding hold Blackhand
//    Summoners (9818) and Blackhand Veterans (9819). When all mobs in an
//    alcove are killed, the corresponding rune is extinguished. When all 7
//    rooms are clear, TYPE_ROOM_EVENT reaches DONE and opens the entry door
//    GO_EMBERSEER_IN (175244) into Pyroguard Emberseer's chamber.
//
// 2. PYROGUARD EMBERSEER (9816) (TYPE_EMBERSEER):
//    Inside the chamber, Emberseer is encaged and invulnerable while channeled
//    by Blackhand Incarcerators (10316). Interacting with the Blackrock Altar
//    (175706) releases the Incarcerators. When all Incarcerators are dead,
//    Emberseer is liberated, attacks the party, and upon his death opens exit
//    doors GO_DOORS (175705) and GO_EMBERSEER_OUT (175153).
//
// 3. THE STADIUM / REND BLACKHAND & GYTH (TYPE_STADIUM):
//    Stepping onto the arena floor (AREATRIGGER_STADIUM 2026) shuts the
//    entrance door GO_GYTH_ENTRY_DOOR (164726) and starts Lord Victor
//    Nefarius's dialogue. Seven waves of Chromatic Whelps (10442), Chromatic
//    Dragons (10447), and Blackhand Handlers (10742) emerge. After the 7th
//    wave, Gyth (10339) enters with Rend Blackhand (10429) mounted. When Gyth
//    falls, Rend dismounts and fights. Defeating both completes TYPE_STADIUM
//    and opens the exit door GO_GYTH_EXIT_DOOR (175186) to The Beast and
//    Drakkisath.

namespace {
constexpr uint32 BRS_MAP = 229;

// Boss entries
constexpr uint32 BRS_EMBERSEER = 9816;
constexpr uint32 BRS_INCARCERATOR = 10316;
constexpr uint32 BRS_SOLAKAR = 10264;
constexpr uint32 BRS_GORALUK = 10899;
constexpr uint32 BRS_GYTH = 10339;
constexpr uint32 BRS_REND = 10429;
constexpr uint32 BRS_THE_BEAST = 10430;
constexpr uint32 BRS_DRAKKISATH = 10363;

// Trash / adds
constexpr uint32 BRS_SUMMONER = 9818;
constexpr uint32 BRS_VETERAN = 9819;
constexpr uint32 BRS_CHROMATIC_WHELP = 10442;
constexpr uint32 BRS_CHROMATIC_DRAGON = 10447;
constexpr uint32 BRS_BLACKHAND_HANDLER = 10742;
constexpr uint32 BRS_CHROMATIC_ELITE_GUARD = 10814;
constexpr uint32 BRS_ROOKERY_WHELP = 10161;
constexpr uint32 BRS_ROOKERY_HATCHER = 10275;
constexpr uint32 BRS_SPIRESTONE_BATTLE_LORD = 10317;
constexpr uint32 BRS_WYRMTHALAK = 9568;

// GameObjects
constexpr uint32 GO_BLACKROCK_ALTAR = 175706;
constexpr uint32 GO_EMBERSEER_IN = 175244;
constexpr uint32 GO_EMBERSEER_OUT = 175153;
constexpr uint32 GO_GYTH_ENTRY_DOOR = 164726;
constexpr uint32 GO_GYTH_EXIT_DOOR = 175186;
constexpr uint32 GO_DOOR_URBS = 164725;

// Event IDs
constexpr uint32 BRS_EVENT_HALL_OF_BINDING = 1;
constexpr uint32 BRS_EVENT_EMBERSEER = 2;
constexpr uint32 BRS_EVENT_STADIUM = 3;
constexpr uint32 BRS_EVENT_UROK = 4;
constexpr uint32 BRS_EVENT_ROOM_PRECLEAR = 5;
constexpr uint32 BRS_EVENT_DRAKKISATH = 6;
constexpr uint32 BRS_EVENT_SOLAKAR = 7;
constexpr uint32 BRS_EVENT_WYRMTHALAK = 8;

// LBRS Urok Doomhowl challenge
constexpr uint32 BRS_UROK_DOOMHOWL = 10584;
constexpr uint32 BRS_UROK_MASSACRER = 10601;
constexpr uint32 BRS_UROK_MAGE = 10602;
constexpr uint32 GO_CHALLENGE_UROK = 175584;
constexpr uint32 GO_AUTEL_OFFRANDES = 175621;
constexpr uint32 BRS_ORDER_UROK = 11;
constexpr uint32 BRS_ORDER_WYRMTHALAK = 14;

// Order indices
constexpr uint32 BRS_ORDER_BINDING = 0; // Pre-Emberseer objective
constexpr uint32 BRS_ORDER_EMBERSEER = 1;
constexpr uint32 BRS_ORDER_SOLAKAR = 2;
constexpr uint32 BRS_ORDER_GORALUK = 3;
constexpr uint32 BRS_ORDER_REND = 4;
constexpr uint32 BRS_ORDER_BEAST = 5;
constexpr uint32 BRS_ORDER_DRAKKISATH = 6;

// Coordinates
// Hall of Binding central waypoint
constexpr float BRS_BINDING_X = 165.0f;
constexpr float BRS_BINDING_Y = -300.0f;
constexpr float BRS_BINDING_Z = 71.0f;

// Emberseer chamber altar
constexpr float BRS_ALTAR_X = 142.971f;
constexpr float BRS_ALTAR_Y = -280.940f;
constexpr float BRS_ALTAR_Z = 91.551f;

// Stadium center
constexpr float BRS_STADIUM_X = 163.62f;
constexpr float BRS_STADIUM_Y = -420.33f;
constexpr float BRS_STADIUM_Z = 110.47f;
} // namespace

void RegisterBlackrockSpireEvents(std::vector<DungeonEvent> &out) {
  // 1. Hall of Binding rune alcove clear (opens door to Emberseer)
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_HALL_OF_BINDING,
                   "Clear Hall of Binding rune rooms")
          .Anchored(BRS_ORDER_BINDING)
          .Persistent()
          .KillCreatureEngage(BRS_SUMMONER, /*count*/ 0, /*seekRadius*/ 80.0f)
          .Timeout(180000)
          .KillCreatureEngage(BRS_VETERAN, /*count*/ 0, /*seekRadius*/ 80.0f)
          .Timeout(180000)
          .Build());

  // 2. Pyroguard Emberseer ritual & liberation
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_EMBERSEER, "Pyroguard Emberseer ritual")
          .Anchored(BRS_ORDER_EMBERSEER)
          .Persistent()
          .UseGO(GO_BLACKROCK_ALTAR, /*seekRadius*/ 30.0f)
          .KillCreatureEngage(BRS_INCARCERATOR, /*count*/ 0,
                              /*seekRadius*/ 60.0f)
          .Timeout(120000)
          .WaitForSpawn(BRS_EMBERSEER, /*wantAlive*/ true)
          .Timeout(60000)
          .KillCreatureEngage(BRS_EMBERSEER, /*count*/ 1, /*seekRadius*/ 60.0f)
          .Timeout(180000)
          .Build());

  // 3. The Stadium wave defense & Rend Blackhand / Gyth
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_STADIUM,
                   "The Stadium: Rend Blackhand & Gyth")
          .Anchored(BRS_ORDER_REND)
          .Persistent()
          .MoveTo(BRS_STADIUM_X, BRS_STADIUM_Y, BRS_STADIUM_Z)
          // Defeat waves of chromatic dragons, whelps, and handlers
          .KillCreatureEngage(BRS_CHROMATIC_WHELP, /*count*/ 0,
                              /*seekRadius*/ 80.0f)
          .Timeout(180000)
          .KillCreatureEngage(BRS_CHROMATIC_DRAGON, /*count*/ 0,
                              /*seekRadius*/ 80.0f)
          .Timeout(180000)
          .KillCreatureEngage(BRS_BLACKHAND_HANDLER, /*count*/ 0,
                              /*seekRadius*/ 80.0f)
          .Timeout(180000)
          .WaitForSpawn(BRS_GYTH, /*wantAlive*/ true)
          .Timeout(120000)
          .KillCreatureEngage(BRS_GYTH, /*count*/ 1, /*seekRadius*/ 80.0f)
          .Timeout(180000)
          .WaitForSpawn(BRS_REND, /*wantAlive*/ true)
          .Timeout(60000)
          .KillCreatureEngage(BRS_REND, /*count*/ 1, /*seekRadius*/ 80.0f)
          .Timeout(180000)
          .Build());

  // 4. LBRS Urok Doomhowl challenge (waves of ogres, then boss spawn)
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_UROK, "Urok Doomhowl challenge")
          .Anchored(BRS_ORDER_UROK)
          .Persistent()
          .UseGO(GO_CHALLENGE_UROK, /*seekRadius*/ 40.0f)
          .SkipIfTargetMissing()
          .KillCreatureEngage(BRS_UROK_MASSACRER, /*count*/ 0,
                              /*seekRadius*/ 60.0f)
          .Timeout(120000)
          .KillCreatureEngage(BRS_UROK_MAGE, /*count*/ 0, /*seekRadius*/ 60.0f)
          .Timeout(120000)
          .WaitForSpawn(BRS_UROK_DOOMHOWL, /*wantAlive*/ true)
          .Timeout(60000)
          .KillCreatureEngage(BRS_UROK_DOOMHOWL, /*count*/ 1,
                              /*seekRadius*/ 60.0f)
          .Timeout(180000)
          .Build());

  // 5. Room-aggro pre-clear for General Drakkisath (Chromatic Elite Guards)
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_ROOM_PRECLEAR,
                   "Clear the room before the room-aggro boss (Blackrock Spire / UBRS)")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());

  // 6. General Drakkisath: prioritize Chromatic Elite Guards
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_DRAKKISATH,
                   "General Drakkisath & Chromatic Elite Guards")
          .Anchored(BRS_ORDER_DRAKKISATH)
          .KillCreatureEngage(BRS_CHROMATIC_ELITE_GUARD, /*count*/ 0, 80.0f)
          .KillCreatureEngage(BRS_DRAKKISATH, /*count*/ 1, 80.0f)
          .Timeout(300000)
          .Build());

  // 7. Solakar Flamewreath: prioritize Rookery Whelps and Hatchers
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_SOLAKAR,
                   "Solakar Flamewreath & Rookery Whelps")
          .Anchored(BRS_ORDER_SOLAKAR)
          .KillCreatureEngage(BRS_ROOKERY_WHELP, /*count*/ 0, 80.0f)
          .KillCreatureEngage(BRS_ROOKERY_HATCHER, /*count*/ 0, 80.0f)
          .KillCreatureEngage(BRS_SOLAKAR, /*count*/ 1, 80.0f)
          .Timeout(300000)
          .Build());

  // 8. Overlord Wyrmthalak: prioritize Spirestone Battle Lords
  out.push_back(
      EventBuilder(BRS_MAP, BRS_EVENT_WYRMTHALAK,
                   "Overlord Wyrmthalak & Spirestone Battle Lords")
          .Anchored(BRS_ORDER_WYRMTHALAK)
          .KillCreatureEngage(BRS_SPIRESTONE_BATTLE_LORD, /*count*/ 0, 80.0f)
          .KillCreatureEngage(BRS_WYRMTHALAK, /*count*/ 1, 80.0f)
          .Timeout(300000)
          .Build());
}

void RegisterBlackrockSpireRoster(std::vector<BossRosterPatch> &t) {
  using namespace DcRoster;

  BossRosterPatch p;
  p.mapId = BRS_MAP;
  p.reorder = {
      {BRS_EMBERSEER, BRS_ORDER_EMBERSEER},
      {BRS_SOLAKAR, BRS_ORDER_SOLAKAR},
      {BRS_GORALUK, BRS_ORDER_GORALUK},
      {BRS_REND, BRS_ORDER_REND},
      {BRS_THE_BEAST, BRS_ORDER_BEAST},
      {BRS_DRAKKISATH, BRS_ORDER_DRAKKISATH},
  };
  p.add = {
      MakeObjective(OBJ(1), BRS_ORDER_BINDING, BRS_MAP, "Hall of Binding",
                    BRS_BINDING_X, BRS_BINDING_Y, BRS_BINDING_Z,
                    /*arriveRadius*/ 12.0f, /*gateEntry*/ 0,
                    /*hook*/ 0, /*eventId*/ BRS_EVENT_HALL_OF_BINDING,
                    /*orderOverride*/ BRS_ORDER_BINDING),
  };
  t.push_back(std::move(p));
}

void RegisterBlackrockSpireWings(
    std::unordered_map<uint32, DungeonWingLayout> &store) {
  store[BRS_MAP] = {true,
                    {
                        {"Upper Blackrock Spire (UBRS)",
                         {
                             BRS_EMBERSEER,
                             BRS_SOLAKAR,
                             BRS_GORALUK,
                             BRS_GYTH,
                             BRS_REND,
                             BRS_THE_BEAST,
                             BRS_DRAKKISATH,
                             BossRosterRegistry::ObjectiveEntry(
                                 1), // Hall of Binding objective
                         }},
                        {"Lower Blackrock Spire (LBRS)",
                         {
                             9196,  // Highlord Omokk
                             9236,  // Shadow Hunter Vosh'gajin
                             9237,  // War Master Voone
                             10596, // Mother Smolderweb
                             10584, // Urok Doomhowl
                             9736,  // Quartermaster Zigris
                             10220, // Halycon
                             10268, // Gizrul the Slavener
                             9568,  // Overlord Wyrmthalak
                         }},
                    }};
}
