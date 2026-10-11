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

// --- Stormwind Vault (map 35) -----------------------------------------------
// Custom Turtle WoW level 60 5-man dungeon under Stormwind City.
//
// 1. CELL CORRIDOR & RAT TRAP GAUNTLET
//    Traps in the lower cell corridors spawn swarms of Vault Rats (93106).
//    When triggered, Rat Doors 3000275 and 3000276 close until all rats are
//    cleared.
//
// 2. PRISONER BOSSES & VAULT DOOR (4000509)
//    Access to the final chamber is blocked by the massive Vault Door
//    (4000509). instance_stormwind_vaults.cpp monitors the four inmate
//    ringleaders:
//      - Black Bride (80850)
//      - Tham'Grarr (80852)
//      - Aszosh Grimflame (80853)
//      - Damian (80854)
//    Once all four are defeated, the instance script opens Vault Door 4000509
//    to GO_STATE_ACTIVE.
//
// 3. ARC'TIRAS (93107) - THE GLACIER OF MADNESS (END BOSS)
//    Rooted caster boss in the central vault. At 60% and 35% HP, Arc'tiras
//    summons Frigid Guardians (60612) and casts Frostbolt / Cause Insanity.

namespace {
constexpr uint32 SWV_MAP = 35;

// Boss entries
constexpr uint32 SWV_VOLKAN = 80851;
constexpr uint32 SWV_THAMGRARR = 80852;
constexpr uint32 SWV_BLACK_BRIDE = 80850;
constexpr uint32 SWV_DAMIAN = 80854;
constexpr uint32 SWV_ASZOSH = 80853;
constexpr uint32 SWV_ARCTIRAS = 93107;

// Adds & hazards
constexpr uint32 SWV_VAULT_RAT = 93106;
constexpr uint32 SWV_FRIGID_GUARDIAN = 60612;

// GameObjects
constexpr uint32 GO_RAT_DOOR_ONE = 3000275;
constexpr uint32 GO_RAT_DOOR_TWO = 3000276;
constexpr uint32 GO_VAULT_DOOR = 4000509;

// Order indices
constexpr uint32 SWV_ORDER_ARCTIRAS = 6;

// Event IDs
constexpr uint32 SWV_EVENT_RAT_TRAP = 1;
constexpr uint32 SWV_EVENT_ARCTIRAS = 2;
} // namespace

void RegisterStormwindVaultEvents(std::vector<DungeonEvent> &out) {
  // 1. Rat Trap gauntlet defense (clears rat swarms to reopen cell gates)
  out.push_back(
      EventBuilder(SWV_MAP, SWV_EVENT_RAT_TRAP, "Clear Vault Rat swarms")
          .Conditional([](Player *bot, AiObjectContext *) -> bool {
            if (!bot || bot->GetMapId() != SWV_MAP)
              return false;
            // Active if any live Vault Rat is within 25 yds of the bot
            return bot->FindNearestCreature(SWV_VAULT_RAT, 25.0f, true) !=
                   nullptr;
          })
          .KillCreatureEngage(SWV_VAULT_RAT, /*count*/ 0, /*seekRadius*/ 35.0f)
          .Timeout(60000)
          .Build());

  // 2. Arc'tiras encounter with Frigid Guardian adds
  out.push_back(
      EventBuilder(SWV_MAP, SWV_EVENT_ARCTIRAS,
                   "Defeat Arc'tiras & Frigid Guardians")
          .Anchored(SWV_ORDER_ARCTIRAS)
          .Persistent()
          .KillCreatureEngage(SWV_ARCTIRAS, /*count*/ 1, /*seekRadius*/ 60.0f)
          .Timeout(180000)
          .KillCreatureEngage(SWV_FRIGID_GUARDIAN, /*count*/ 0,
                              /*seekRadius*/ 50.0f)
          .Timeout(60000)
          .Build());
}
