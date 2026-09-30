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

// --- Blackwing Lair (map 469) ---------------------------------------------
//
// 1. CHROMAGGUS GATE LEVER (event 1): The iron portcullis (GO 179116) blocks
//    access to Chromaggus. After Firemaw (11983), Ebonroc (14601), and
//    Flamegor (11981) are defeated, the lever in the laboratory (GO 179148,
//    GO_LEVER_CHROMAGGUS_SIDE) must be pulled to open the gate.
//
// 2. NEFARIAN THRONE GOSSIP (event 2, anchored objective at the throne):
//    Lord Victor Nefarius (10162) sits on his throne (-7526.27, -1135.04, 473.45).
//    Nefarian in dragon form (11583) has NO static creature spawn in the DB;
//    he is summoned dynamically once 42 drakonids are killed during Phase 1.
//    Gossiping Victor Nefarius drills down 3 menus to trigger the trial,
//    defends against the 42 drakonid waves, waits for Nefarian (11583) to land,
//    and engages him to completion.

namespace
{
    constexpr uint32 BWL_MAP = 469;

    // Boss & NPC entries
    constexpr uint32 BWL_FIREMAW            = 11983;
    constexpr uint32 BWL_EBONROC            = 14601;
    constexpr uint32 BWL_FLAMEGOR           = 11981;
    constexpr uint32 BWL_CHROMAGGUS         = 14020;
    constexpr uint32 BWL_LORD_NEFARIUS      = 10162;
    constexpr uint32 BWL_NEFARIAN           = 11583;

    // GameObjects
    constexpr uint32 BWL_GO_CHROM_LEVER     = 179148;
    constexpr uint32 BWL_GO_CHROM_GATE      = 179116;

    // Instance encounter IDs (from blackwing_lair.h)
    constexpr uint32 BWL_TYPE_FIREMAW       = 3;
    constexpr uint32 BWL_TYPE_EBONROC       = 4;
    constexpr uint32 BWL_TYPE_FLAMEGOR      = 5;
    constexpr uint32 BWL_TYPE_CHROMAGGUS    = 6;
    constexpr uint32 BWL_TYPE_NEFARIAN      = 7;
    constexpr uint32 BWL_STATE_DONE         = 3;

    // Event IDs
    constexpr uint32 BWL_EVENT_CHROMAGGUS_LEVER = 1;
    constexpr uint32 BWL_EVENT_NEFARIAN_THRONE  = 2;

    // Clear-path order keys (8th boss)
    constexpr uint32 BWL_NEFARIAN_ORDER = 8;

    // Throne coords (Lord Victor Nefarius spawn)
    constexpr float BWL_NEFARIAN_X = -7526.27f;
    constexpr float BWL_NEFARIAN_Y = -1135.04f;
    constexpr float BWL_NEFARIAN_Z = 473.445f;

    // Condition: Chromaggus lever is needed when all 3 drakes are dead but Chromaggus is not yet engaged/done
    bool BwlChromaggusLeverNeeded(Player* bot, AiObjectContext* /*context*/)
    {
        if (!bot)
            return false;

        InstanceScript* inst = DcTargeting::GetInstanceScript(bot);
        if (!inst)
            return false;

        bool const drakesDead = (inst->GetData(BWL_TYPE_FIREMAW) == BWL_STATE_DONE) &&
                                (inst->GetData(BWL_TYPE_EBONROC) == BWL_STATE_DONE) &&
                                (inst->GetData(BWL_TYPE_FLAMEGOR) == BWL_STATE_DONE);

        if (!drakesDead)
            return false;

        if (inst->GetData(BWL_TYPE_CHROMAGGUS) == BWL_STATE_DONE)
            return false;

        // Check if the lever is present and usable
        GameObject* lever = bot->FindNearestGameObject(BWL_GO_CHROM_LEVER, 80.0f);
        return lever != nullptr;
    }

    // Condition: Lord Victor Nefarius is on his throne waiting to be gossiped
    bool BwlNefariusGossipReady(Player* bot, AiObjectContext* /*context*/)
    {
        if (!bot)
            return false;

        InstanceScript* inst = DcTargeting::GetInstanceScript(bot);
        if (!inst)
            return false;

        if (inst->GetData(BWL_TYPE_CHROMAGGUS) != BWL_STATE_DONE)
            return false;

        if (inst->GetData(BWL_TYPE_NEFARIAN) == BWL_STATE_DONE)
            return false;

        Creature* nefarius = DcTargeting::FindLiveCreatureOnMap(bot, BWL_LORD_NEFARIUS);
        return nefarius != nullptr && nefarius->HasFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }
}

void RegisterBlackwingLairEvents(std::vector<DungeonEvent>& out)
{
    // 1. Chromaggus Gate Lever: pull the lever on the lab balcony to raise the portcullis.
    out.push_back(EventBuilder(BWL_MAP, BWL_EVENT_CHROMAGGUS_LEVER, "Pull lever to open Chromaggus portcullis")
                      .Conditional(&BwlChromaggusLeverNeeded)
                      .Repeatable()
                      .UseGO(BWL_GO_CHROM_LEVER)
                          .Timeout(15000)
                      .Build());

    // 2. Nefarian Throne Gossip: speak to Lord Victor Nefarius on the throne to begin the trial.
    //    WaitForSpawn bridges the 42 drakonid wave combat until Nefarian lands, then engages him.
    out.push_back(EventBuilder(BWL_MAP, BWL_EVENT_NEFARIAN_THRONE, "Summon and defeat Nefarian (Throne Gossip)")
                      .Anchored(BWL_NEFARIAN_ORDER)
                      .Persistent()
                      .Gossip(BWL_LORD_NEFARIUS, 0)
                          .Timeout(30000)
                      .WaitForSpawn(BWL_NEFARIAN, /*wantAlive*/ true)
                          .Timeout(600000)
                      .KillCreatureEngage(BWL_NEFARIAN, /*count*/ 1, 150.0f)
                          .Timeout(600000)
                      .Build());
}

void RegisterBlackwingLairRoster(std::vector<BossRosterPatch>& t)
{
    using namespace DcRoster;

    // Nefarian (11583) has no static spawn row in creature table - he is summoned
    // dynamically by Lord Victor Nefarius (10162) after the 42 drakonid add wave.
    // Without this patch, BossSpawnIndex cannot emit him as a static boss.
    // We remove 11583 from raw creature lookup and add him as an anchored objective
    // tied to BWL_EVENT_NEFARIAN_THRONE at Lord Victor Nefarius's throne.
    {
        BossRosterPatch p;
        p.mapId = BWL_MAP;
        p.remove = { BWL_NEFARIAN };
        t.push_back(std::move(p));
    }

    {
        BossRosterPatch p;
        p.mapId = BWL_MAP;
        p.add = {
            MakeObjective(BossRosterRegistry::ObjectiveEntry(1), BWL_NEFARIAN_ORDER,
                          BWL_MAP, "Nefarian (Throne Gossip)",
                          BWL_NEFARIAN_X, BWL_NEFARIAN_Y, BWL_NEFARIAN_Z,
                          /*arriveRadius*/ 15.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ BWL_EVENT_NEFARIAN_THRONE,
                          /*orderOverride*/ BWL_NEFARIAN_ORDER),
        };
        t.push_back(std::move(p));
    }
}
