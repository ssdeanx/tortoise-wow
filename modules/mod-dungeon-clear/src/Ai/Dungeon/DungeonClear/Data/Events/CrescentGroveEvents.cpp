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

// --- Crescent Grove (map 802) -----------------------------------------------
// Custom Turtle WoW level 32-38 dungeon in Ashenvale.
//
// 1. Grovetender Engryss (92107, lvl 34) - Groveweald furbolg chieftain.
// 2. Keeper Ranathos (92109, lvl 35) - Corrupted keeper of the grove.
// 3. High Priestess A'lathea (92108, lvl 36) - Priestess of the Moon.
// 4. Fenektis the Deceiver (92111, lvl 37) - Shadow satyr.
// 5. Master Raxxieth (92110, lvl 38) - Satyr lord serving the Burning Legion (End Boss).
//    Fights with demonic spells and summons Raging Infernals / Felguard adds.

namespace {
constexpr uint32 CG_MAP = 802;

// Boss entries
constexpr uint32 CG_ENGRYSS = 92107;
constexpr uint32 CG_RANATHOS = 92109;
constexpr uint32 CG_ALATHEA = 92108;
constexpr uint32 CG_FENEKTIS = 92111;
constexpr uint32 CG_RAXXIETH = 92110;

// Order indices (0-based)
constexpr uint32 CG_ORDER_RAXXIETH = 4; // Order 5 -> key 4

// Event IDs
constexpr uint32 CG_EVENT_RAXXIETH = 1;
} // namespace

void RegisterCrescentGroveEvents(std::vector<DungeonEvent>& out)
{
    // Master Raxxieth - final encounter
    out.push_back(
        EventBuilder(CG_MAP, CG_EVENT_RAXXIETH, "Master Raxxieth")
            .Anchored(CG_ORDER_RAXXIETH)
            .Persistent()
            .KillCreatureEngage(CG_RAXXIETH, /*count*/ 1, /*seekRadius*/ 60.0f)
            .Timeout(240000)
            .Build());
}
