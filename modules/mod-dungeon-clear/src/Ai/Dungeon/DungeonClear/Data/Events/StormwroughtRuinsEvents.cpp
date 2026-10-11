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

// --- Stormwrought Ruins (map 818) -------------------------------------------
// Custom Turtle WoW 20-man raid instance on the Stormwrought Archipelago.
// 11 boss encounters with in-fight adds and an ancient Shadow Council inner sanctum:
//
// 1. Dagar the Glutton (62547) - Ogre brute with heavy cleaves and devour.
// 2. Oronok Torn-Heart (62548) - Corrupted shaman, chain lightning & totem buffs.
// 3. Librarian Theodorus (62552) - Arcane scholar, summons Sea Elementals (62742)
//    and rains Blizzard.
// 4. Duke Balor IV (62549) - Corrupted nobleman, shadow bolts & dark strikes.
// 5. Chieftain Stormsong (62551) - Lightning strikes and thunder stomps.
// 6. Deathlord Tidebane (62550) - Death and Decay, Wail of Souls fear aura.
// 7. Subjugator Halthas Shadecrest (62652) - Dominate Mind MC, fear & flamestrike.
// 8. Eldermaw the Primordial (62661) - Primordial Regeneration at 50% HP, summons
//    Eldermaw Crocolisks (62674).
// 9. Mycellakos (62664) - Fungal colossus, summons Volatile Fungus (300420) and
//    Growth of Mycellakos (62675).
// 10. Lady Drazare (62665) - Seductive dreadlord, Dark Seduction MC and
//     Drazare's Embrace explosive curse.
// 11. Ighal'for (62671) - High Shadow Council summoner. At 50% HP teleports to
//     the Nether Altar, becomes unattackable, and summons waves of Nether Imps
//     (62672) followed by the massive void behemoth Mergothid (62673). Once
//     Mergothid falls, Ighal'for resumes combat to the death.

namespace {
constexpr uint32 SWR_MAP = 818;

// Boss entries
constexpr uint32 SWR_DAGAR       = 62547;
constexpr uint32 SWR_ORONOK      = 62548;
constexpr uint32 SWR_THEODORUS   = 62552;
constexpr uint32 SWR_BALOR       = 62549;
constexpr uint32 SWR_STORMSONG   = 62551;
constexpr uint32 SWR_TIDEBANE    = 62550;
constexpr uint32 SWR_HALTHAS     = 62652;
constexpr uint32 SWR_ELDERMAW    = 62661;
constexpr uint32 SWR_MYCELLAKOS  = 62664;
constexpr uint32 SWR_DRAZARE     = 62665;
constexpr uint32 SWR_IGHALFOR    = 62671;

// Add entries
constexpr uint32 SWR_SEA_ELEMENTAL       = 62742;
constexpr uint32 SWR_ELDERMAW_CROCOLISK  = 62674;
constexpr uint32 SWR_GROWTH_MYCELLAKOS   = 62675;
constexpr uint32 SWR_NETHER_IMP          = 62672;
constexpr uint32 SWR_MERGOTHID           = 62673;

// Secret switches and door
constexpr uint32 SWR_GO_SECRET_SWITCH    = 2020236;
constexpr uint32 SWR_GO_SECRET_DOOR      = 2006138;

// Event IDs
constexpr uint32 SWR_EVENT_THEODORUS     = 1;
constexpr uint32 SWR_EVENT_ELDERMAW      = 2;
constexpr uint32 SWR_EVENT_MYCELLAKOS    = 3;
constexpr uint32 SWR_EVENT_SECRET_DOOR   = 4;
constexpr uint32 SWR_EVENT_IGHALFOR      = 5;
constexpr uint32 SWR_EVENT_ROOM_PRECLEAR = 6;

// Order keys (order - 1)
constexpr uint32 SWR_ORDER_THEODORUS     = 2; // Order 3 -> key 2
constexpr uint32 SWR_ORDER_ELDERMAW      = 7; // Order 8 -> key 7
constexpr uint32 SWR_ORDER_MYCELLAKOS    = 8; // Order 9 -> key 8
constexpr uint32 SWR_ORDER_IGHALFOR      = 10; // Order 11 -> key 10

// Condition: Secret switches needed if door is still closed
bool SwrSecretSwitchNeeded(Player* bot, AiObjectContext* /*context*/) {
  if (!bot)
    return false;
  GameObject* door = bot->FindNearestGameObject(SWR_GO_SECRET_DOOR, 100.0f);
  if (!door)
    return false;
  return door->GetGoState() != GO_STATE_ACTIVE;
}
} // namespace

void RegisterStormwroughtRuinsEvents(std::vector<DungeonEvent> &out) {
  // 1. Librarian Theodorus (order 3) - prioritize Sea Elemental adds
  out.push_back(
      EventBuilder(SWR_MAP, SWR_EVENT_THEODORUS, "Librarian Theodorus")
          .Anchored(SWR_ORDER_THEODORUS)
          .KillCreatureEngage(SWR_SEA_ELEMENTAL, /*count*/ 0, /*seekRadius*/ 60.0f)
          .KillCreatureEngage(SWR_THEODORUS, /*count*/ 1, /*seekRadius*/ 60.0f)
          .Timeout(300000)
          .Build());

  // 2. Eldermaw the Primordial (order 8) - clear crocolisk adds at 50% HP
  out.push_back(
      EventBuilder(SWR_MAP, SWR_EVENT_ELDERMAW, "Eldermaw the Primordial")
          .Anchored(SWR_ORDER_ELDERMAW)
          .KillCreatureEngage(SWR_ELDERMAW_CROCOLISK, /*count*/ 0, /*seekRadius*/ 60.0f)
          .KillCreatureEngage(SWR_ELDERMAW, /*count*/ 1, /*seekRadius*/ 60.0f)
          .Timeout(300000)
          .Build());

  // 3. Mycellakos (order 9) - clear fungal growth adds
  out.push_back(
      EventBuilder(SWR_MAP, SWR_EVENT_MYCELLAKOS, "Mycellakos")
          .Anchored(SWR_ORDER_MYCELLAKOS)
          .KillCreatureEngage(SWR_GROWTH_MYCELLAKOS, /*count*/ 0, /*seekRadius*/ 60.0f)
          .KillCreatureEngage(SWR_MYCELLAKOS, /*count*/ 1, /*seekRadius*/ 60.0f)
          .Timeout(300000)
          .Build());

  // 4. Secret switches to open inner sanctum door
  out.push_back(
      EventBuilder(SWR_MAP, SWR_EVENT_SECRET_DOOR, "Stormwrought Secret Switch")
          .Conditional(&SwrSecretSwitchNeeded)
          .Repeatable()
          .UseGO(SWR_GO_SECRET_SWITCH)
          .Timeout(30000)
          .Build());

  // 5. Ighal'for (end boss, order 11) - phase transition with Nether Imps & Mergothid
  out.push_back(
      EventBuilder(SWR_MAP, SWR_EVENT_IGHALFOR, "Ighal'for")
          .Anchored(SWR_ORDER_IGHALFOR)
          .Persistent()
          .KillCreatureEngage(SWR_NETHER_IMP, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(SWR_MERGOTHID, /*count*/ 0, /*seekRadius*/ 80.0f)
          .KillCreatureEngage(SWR_IGHALFOR, /*count*/ 1, /*seekRadius*/ 80.0f)
          .Timeout(600000)
          .Build());

  // 6. Generic room-aggro pre-clear for Tidebane, Halthas, Eldermaw, Mycellakos, Drazare, Ighal'for
  out.push_back(
      EventBuilder(SWR_MAP, SWR_EVENT_ROOM_PRECLEAR, "Clear the room before the room-aggro boss (Stormwrought Ruins)")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());
}

void RegisterStormwroughtRuinsRoster(std::vector<BossRosterPatch> & /*t*/) {
  // All 11 bosses have database spawns and compiled orders in DcBossEntries1121.h.
}
