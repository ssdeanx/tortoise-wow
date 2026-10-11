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

// --- Timbermaw Hold (map 819) -----------------------------------------------
// Custom Turtle WoW 20-player raid instance added in patch 1.18.1 "Nightmares
// of Ursol", entered through areatrigger_teleport 5613.
//
// 1. KARRSH THE SENTINEL (62934)
//    Entrance guardian at (-7893.7, -3432.4, 248.1). Named elite encounter
//    (rank 0, 483k HP).
//
// 2. ROTGROWL (62936) & KODIAK (62937) - PAIRED ENCOUNTER
//    Corrupted dire bear pair at (-7770.1, -3197.6, 248.5) and (-7765.3, -3199.0).
//    Kodiak has 112k HP and Rotgrowl has 217k HP. Target priority burns Kodiak
//    first to eliminate the secondary bear quickly.
//
// 3. ORMANOS THE CRACKED (62935)
//    Ancient Furbolg earthbreaker at (-7528.1, -3451.5, 201.0). Rank 3 boss
//    (690k HP) with heavy physical cleaves.
//
// 4. ARCHDRUID KRONN (62938)
//    Corrupted druid ancient at (-7484.8, -3625.7, 261.7). Rank 3 boss (638k HP)
//    surrounded by Withermaw Ursa and Totemic trash.
//
// 5. LOKTANAG THE VILE (2139)
//    Deep cave warlock/shaman boss at (-7659.8, -3183.8, 171.5). Rank 3 boss
//    (614k HP) accompanied by Corruption of Loktanag (62879) and Tainted Mass (62878).
//
// 6. TRIOCH THE DEVOURER (62946)
//    Hydra/behemoth beast at (-7722.1, -3723.6, 229.6). Rank 3 boss (533k HP).
//
// 7. SELENAXX FOULHEART (62940)
//    Corrupted satyr lord at (-7746.5, -3645.5, 283.1). Rank 0 named encounter
//    (581k HP) in the Foulheart glade.
//
// 8. CHIEFTAIN PARTATH (62941)
//    Current final live encounter at (-7250.3, -3280.2, 261.7). Rank 3 boss
//    (689k HP) ruling the Withermaw stronghold.
//
// NOTE: Ursol (62947) and Peroth'arn (60686) are unspawned in the baseline DB
// and are held off the roster to prevent stalled clears.

namespace {
constexpr uint32 TMH_MAP = 819;

// Boss entries
constexpr uint32 TMH_KARRSH = 62934;
constexpr uint32 TMH_ROTGROWL = 62936;
constexpr uint32 TMH_KODIAK = 62937;
constexpr uint32 TMH_ORMANOS = 62935;
constexpr uint32 TMH_KRONN = 62938;
constexpr uint32 TMH_LOKTANAG = 2139;
constexpr uint32 TMH_TRIOCH = 62946;
constexpr uint32 TMH_SELENAXX = 62940;
constexpr uint32 TMH_PARTATH = 62941;

// Encounter Adds
constexpr uint32 TMH_CORRUPTION_OF_LOKTANAG = 62879;
constexpr uint32 TMH_TAINTED_MASS = 62878;

// Spawn Coordinates
constexpr float TMH_KARRSH_X = -7893.67f;
constexpr float TMH_KARRSH_Y = -3432.38f;
constexpr float TMH_KARRSH_Z = 248.07f;

constexpr float TMH_ROTGROWL_X = -7770.10f;
constexpr float TMH_ROTGROWL_Y = -3197.55f;
constexpr float TMH_ROTGROWL_Z = 248.53f;

constexpr float TMH_KODIAK_X = -7765.33f;
constexpr float TMH_KODIAK_Y = -3199.05f;
constexpr float TMH_KODIAK_Z = 248.53f;

constexpr float TMH_ORMANOS_X = -7528.06f;
constexpr float TMH_ORMANOS_Y = -3451.49f;
constexpr float TMH_ORMANOS_Z = 200.97f;

constexpr float TMH_KRONN_X = -7484.78f;
constexpr float TMH_KRONN_Y = -3625.66f;
constexpr float TMH_KRONN_Z = 261.68f;

constexpr float TMH_LOKTANAG_X = -7659.78f;
constexpr float TMH_LOKTANAG_Y = -3183.77f;
constexpr float TMH_LOKTANAG_Z = 171.47f;

constexpr float TMH_TRIOCH_X = -7722.14f;
constexpr float TMH_TRIOCH_Y = -3723.56f;
constexpr float TMH_TRIOCH_Z = 229.56f;

constexpr float TMH_SELENAXX_X = -7746.45f;
constexpr float TMH_SELENAXX_Y = -3645.47f;
constexpr float TMH_SELENAXX_Z = 283.10f;

constexpr float TMH_PARTATH_X = -7250.34f;
constexpr float TMH_PARTATH_Y = -3280.21f;
constexpr float TMH_PARTATH_Z = 261.66f;

// Order indices
constexpr uint32 TMH_ORDER_KARRSH = 1;
constexpr uint32 TMH_ORDER_ROTGROWL = 2;
constexpr uint32 TMH_ORDER_KODIAK = 3;
constexpr uint32 TMH_ORDER_ORMANOS = 4;
constexpr uint32 TMH_ORDER_KRONN = 5;
constexpr uint32 TMH_ORDER_LOKTANAG = 6;
constexpr uint32 TMH_ORDER_TRIOCH = 7;
constexpr uint32 TMH_ORDER_SELENAXX = 8;
constexpr uint32 TMH_ORDER_PARTATH = 9;

// Event IDs
constexpr uint32 TMH_EVENT_KARRSH = 1;
constexpr uint32 TMH_EVENT_BEAR_DUO = 2;
constexpr uint32 TMH_EVENT_ORMANOS = 3;
constexpr uint32 TMH_EVENT_KRONN = 4;
constexpr uint32 TMH_EVENT_LOKTANAG = 5;
constexpr uint32 TMH_EVENT_TRIOCH = 6;
constexpr uint32 TMH_EVENT_SELENAXX = 7;
constexpr uint32 TMH_EVENT_PARTATH = 8;
constexpr uint32 TMH_EVENT_ROOM_PRECLEAR = 9;
} // namespace

void RegisterTimbermawHoldEvents(std::vector<DungeonEvent> &out) {
  // 1. Karrsh the Sentinel: Front gatekeeper
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_KARRSH,
                             "Karrsh the Sentinel")
                    .Anchored(TMH_ORDER_KARRSH)
                    .Persistent()
                    .KillCreatureEngage(TMH_KARRSH, 1, 75.0f)
                    .Timeout(300000)
                    .Build());

  // 2. Kodiak & Rotgrowl: Burn Kodiak (112k) first, then finish Rotgrowl (217k)
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_BEAR_DUO,
                             "Kodiak & Rotgrowl Duo")
                    .Anchored(TMH_ORDER_ROTGROWL)
                    .Persistent()
                    .KillCreatureEngage(TMH_KODIAK, 1, 60.0f)
                    .Timeout(180000)
                    .KillCreatureEngage(TMH_ROTGROWL, 1, 60.0f)
                    .Timeout(240000)
                    .Build());

  // 3. Ormanos the Cracked: Giant Furbolg earthbreaker
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_ORMANOS,
                             "Ormanos the Cracked")
                    .Anchored(TMH_ORDER_ORMANOS)
                    .Persistent()
                    .KillCreatureEngage(TMH_ORMANOS, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 4. Archdruid Kronn: Corrupted druid in the grove
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_KRONN,
                             "Archdruid Kronn")
                    .Anchored(TMH_ORDER_KRONN)
                    .Persistent()
                    .KillCreatureEngage(TMH_KRONN, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 5. Loktanag the Vile: Clear corruption adds then burn Loktanag
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_LOKTANAG,
                             "Loktanag the Vile & Corruptions")
                    .Anchored(TMH_ORDER_LOKTANAG)
                    .Persistent()
                    .KillCreatureEngage(TMH_CORRUPTION_OF_LOKTANAG, 0, 50.0f)
                    .Timeout(60000)
                    .KillCreatureEngage(TMH_LOKTANAG, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 6. Trioch the Devourer: Deep cavern beast
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_TRIOCH,
                             "Trioch the Devourer")
                    .Anchored(TMH_ORDER_TRIOCH)
                    .Persistent()
                    .KillCreatureEngage(TMH_TRIOCH, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 7. Selenaxx Foulheart: Satyr lord in the glade
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_SELENAXX,
                             "Selenaxx Foulheart")
                    .Anchored(TMH_ORDER_SELENAXX)
                    .Persistent()
                    .KillCreatureEngage(TMH_SELENAXX, 1, 75.0f)
                    .Timeout(360000)
                    .Build());

  // 8. Chieftain Partath: Grand den boss
  out.push_back(EventBuilder(TMH_MAP, TMH_EVENT_PARTATH,
                             "Chieftain Partath (End Boss)")
                    .Anchored(TMH_ORDER_PARTATH)
                    .Persistent()
                    .KillCreatureEngage(TMH_PARTATH, 1, 80.0f)
                    .Timeout(420000)
                    .Build());

  // 9. Room-aggro pre-clear for Loktanag, Kronn, Selenaxx, and Partath
  out.push_back(
      EventBuilder(TMH_MAP, TMH_EVENT_ROOM_PRECLEAR,
                   "Clear the room before the room-aggro boss (Timbermaw Hold)")
          .Conditional(&DcRoomAggroPreClearCondition)
          .KillCreature(/*room trash*/ 0)
          .Build());
}

void RegisterTimbermawHoldRoster(std::vector<BossRosterPatch> &t) {
  using namespace DcRoster;

  // Complete 9-encounter roster for Timbermaw Hold (map 819).
  {
    BossRosterPatch p;
    p.mapId = TMH_MAP;
    p.add = {
        MakeBoss(TMH_KARRSH, TMH_MAP, "Karrsh the Sentinel", TMH_KARRSH_X,
                 TMH_KARRSH_Y, TMH_KARRSH_Z,
                 /*completionFrom*/ TMH_KARRSH,
                 /*orderOverride*/ TMH_ORDER_KARRSH),
        MakeBoss(TMH_ROTGROWL, TMH_MAP, "Rotgrowl", TMH_ROTGROWL_X,
                 TMH_ROTGROWL_Y, TMH_ROTGROWL_Z,
                 /*completionFrom*/ TMH_ROTGROWL,
                 /*orderOverride*/ TMH_ORDER_ROTGROWL),
        MakeBoss(TMH_KODIAK, TMH_MAP, "Kodiak", TMH_KODIAK_X,
                 TMH_KODIAK_Y, TMH_KODIAK_Z,
                 /*completionFrom*/ TMH_KODIAK,
                 /*orderOverride*/ TMH_ORDER_KODIAK),
        MakeBoss(TMH_ORMANOS, TMH_MAP, "Ormanos the Cracked", TMH_ORMANOS_X,
                 TMH_ORMANOS_Y, TMH_ORMANOS_Z,
                 /*completionFrom*/ TMH_ORMANOS,
                 /*orderOverride*/ TMH_ORDER_ORMANOS),
        MakeBoss(TMH_KRONN, TMH_MAP, "Archdruid Kronn", TMH_KRONN_X,
                 TMH_KRONN_Y, TMH_KRONN_Z,
                 /*completionFrom*/ TMH_KRONN,
                 /*orderOverride*/ TMH_ORDER_KRONN),
        MakeBoss(TMH_LOKTANAG, TMH_MAP, "Loktanag the Vile", TMH_LOKTANAG_X,
                 TMH_LOKTANAG_Y, TMH_LOKTANAG_Z,
                 /*completionFrom*/ TMH_LOKTANAG,
                 /*orderOverride*/ TMH_ORDER_LOKTANAG),
        MakeBoss(TMH_TRIOCH, TMH_MAP, "Trioch the Devourer", TMH_TRIOCH_X,
                 TMH_TRIOCH_Y, TMH_TRIOCH_Z,
                 /*completionFrom*/ TMH_TRIOCH,
                 /*orderOverride*/ TMH_ORDER_TRIOCH),
        MakeBoss(TMH_SELENAXX, TMH_MAP, "Selenaxx Foulheart", TMH_SELENAXX_X,
                 TMH_SELENAXX_Y, TMH_SELENAXX_Z,
                 /*completionFrom*/ TMH_SELENAXX,
                 /*orderOverride*/ TMH_ORDER_SELENAXX),
        MakeBoss(TMH_PARTATH, TMH_MAP, "Chieftain Partath", TMH_PARTATH_X,
                 TMH_PARTATH_Y, TMH_PARTATH_Z,
                 /*completionFrom*/ TMH_PARTATH,
                 /*orderOverride*/ TMH_ORDER_PARTATH)};
    t.push_back(std::move(p));
  }
}
