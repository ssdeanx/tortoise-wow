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

// --- Frostmane Hollow (map 822) ---------------------------------------------
// Custom Turtle WoW level 12-16 intro dungeon in Dun Morogh.
//
// 1. Kan'za the Seer (63129, lvl 13) - Frostmane seer.
// 2. Tan'sha the Sleek (63133, lvl 14) - Stealth stalker.
// 3. Battlemaster Ubukaz (63131, lvl 15) - Frostmane chieftain / warrior.
// 4. Handler Oboka (63132, lvl 15) - Beastmaster with leopard pets.
// 5. Hailar the Frigid (63130, lvl 16) - End Boss.
//    Spawns in the deep cavern at (-8019.12, -3462.60, 187.53).
//    Five Frostmane Ritualists (36519) stand in a circle around Hailar,
//    channeling Frostmane Ritual (spell 41592). Bots clear the ritualists
//    around the platform and defeat Hailar.

namespace {
constexpr uint32 FMH_MAP = 822;

// Boss entries
constexpr uint32 FMH_KANZA = 63129;
constexpr uint32 FMH_TANSHA = 63133;
constexpr uint32 FMH_UBUKAZ = 63131;
constexpr uint32 FMH_OBOKA = 63132;
constexpr uint32 FMH_HAILAR = 63130;

// Add entries
constexpr uint32 FMH_RITUALIST = 36519;

// Order indices (0-based)
constexpr uint32 FMH_ORDER_HAILAR = 4; // Order 5 -> key 4

// Event IDs
constexpr uint32 FMH_EVENT_HAILAR = 1;
} // namespace

void RegisterFrostmaneHollowEvents(std::vector<DungeonEvent>& out)
{
    // Hailar the Frigid & Ritualists
    out.push_back(
        EventBuilder(FMH_MAP, FMH_EVENT_HAILAR, "Hailar the Frigid & Ritualists")
            .Anchored(FMH_ORDER_HAILAR)
            .Persistent()
            // 1. Clear the channeling ritualists surrounding Hailar
            .ClearRadius(-8019.12f, -3462.60f, 187.528f, /*radius*/ 25.0f)
            .OnlyEntries({FMH_RITUALIST})
            .Timeout(60000)
            // 2. Engage and kill Hailar the Frigid
            .KillCreatureEngage(FMH_HAILAR, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(180000)
            .Build());
}
