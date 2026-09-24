/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"

#include "GameObject.h"
#include "Player.h"
#include "Playerbots.h"
#include "Ai/Dungeon/DungeonClear/Util/DcTargeting.h"

// --- Zul'Gurub (map 309) --------------------------------------------------
// Three rows, each following an existing dungeon precedent:
//
// 1. ROOM-AGGRO PRE-CLEAR for the current room-aggro boss (SM Cathedral /
//    Scholomance shape; the shared DcRoomAggroPreClearCondition reads the
//    DungeonClearRoomTrashValue for whatever boss is next, so this ONE event
//    serves every RoomAggroRegistry row on the map):
//      * High Priestess Jeklik (14517), the FIRST boss. Her bat cave/terrace
//        trash is not script-force-pulled, but the room is pre-cleared before
//        the engage per the server owner (the first boss's room must be
//        clear): Bloodseeker Bat 11368, Gurubashi Bat Rider 14750, Hakkari
//        Witch Doctor 11831, Gurubashi Headhunter 11351.
//      * High Priest Venoxis (14507), the second boss. His temple's trash is
//        not script-force-pulled either, but the room is pre-cleared for the
//        same reason, and his z-band evade makes pulling the room out unsafe
//        anyway (see the map-309 FightInPlaceRegistry zone).
//      * High Priestess Mar'li (14510). Her instance SetData(TYPE_MARLI,
//        IN_PROGRESS) walks every live Mar'li-room trash mob onto her victim
//        the moment she is engaged (bounded by the script's AreaId 3379 check
//        rather than a radius): Razzashi Skitterer 14880, Razzashi Venombrood
//        14532, Hakkari Shadowcaster 11338, Razzashi Broodwidow 11370.
//        Pulling her with the room up feeds the party the whole pile.
//    Geometry for all three rows lives in RoomAggroRegistry.
//
// 2. Bloodlord Mandokir (11382) — VILEBRANCH SPEAKER PREREQUISITE. Mandokir
//    spawns pacified and immune to players (boss_mandokir START_FLAGS) and only
//    becomes attackable when the Vilebranch Speaker (11391) at the bottom of
//    his stairs dies. The event seeks and kills the Speaker once boss-nav has
//    delivered the tank to the stairs, so the boss engage is never blocked by
//    the immunity (Sunken Temple defender / RFD gong prerequisite shape). The
//    condition is proximity-gated (the integrity rule for a conditional event
//    with no arrival step); it is Repeatable so a wipe that respawns the
//    Speaker re-arms it, and it no-ops once the Speaker is already dead (the
//    route trash usually kills him on the way in).
//
// 3. High Priestess Arlokk (14515) — SUMMON-ONLY BOSS AT AN OBJECTIVE ANCHOR
//    (Zul'Farrak's Sacred Pool / Gahz'rilla shape, verbatim). She has NO
//    creature spawn: the Gong of Bethekk (GO 180526) runs event_scripts 9066
//    and summons her at (-11540.7, -1627.7, 41.3). With no spawn row,
//    BossSpawnIndex can never emit her and her kill can never set a completion
//    bit, so a plain boss anchor would wedge the run on an uncreditable leg.
//    Instead the roster patch adds an OBJECTIVE anchor ON THE GONG (map 309,
//    order key 5, between Thekal and Mandokir) and this anchored event rings
//    the gong, waits for her to materialise, then kills her through the normal
//    engage pipeline; the objective latches when the event completes.
//    Persistent so the kill's >1 s combat tick-gap cannot rewind the step
//    list, and WaitForSpawn is essential between the ring and the kill - the
//    engage gate would otherwise read "no live Arlokk" before the summon and
//    false-complete.

namespace
{
    bool ZgMandokirSpeakerAlive(Player* bot, AiObjectContext* context);

    constexpr uint32 ZG_MANDOKIR = 11382;
    constexpr uint32 ZG_VILEBRANCH_SPEAKER = 11391;
    constexpr uint32 ZG_ARLOKK = 14515;
    constexpr uint32 ZG_GONG_OF_BETHEKK = 180526;

    // Mandokir's spawn at the top of the stairs; the Speaker stands ~36yd
    // below him. The gate radius covers the anchor plus the stairs without
    // reaching the next encounter (Hakkar's temple is ~250yd away).
    constexpr float ZG_MANDOKIR_ANCHOR_X = -12167.8f;
    constexpr float ZG_MANDOKIR_ANCHOR_Y = -1927.2f;
    constexpr float ZG_MANDOKIR_ANCHOR_Z = 153.7f;
    constexpr float ZG_MANDOKIR_GATE_RANGE = 80.0f;

    // The kill step's own seek radius: the Speaker is ~36yd from the anchor,
    // so 60yd covers the stairs landing without reaching into the arena.
    constexpr float ZG_SPEAKER_SEARCH = 60.0f;

    // Approach + kill budget for a single elite; generous, and the route trash
    // normally beats it to the kill anyway.
    constexpr uint32 ZG_SPEAKER_TIMEOUT = 60000;

    // The Gong of Bethekk and Arlokk's summon point (event_scripts 9066). The
    // objective anchor sits ON the gong; the event's ring search radius covers
    // the anchor's arrive radius so a tank parked at the edge still reaches it.
    constexpr float ZG_GONG_X = -11569.2f;
    constexpr float ZG_GONG_Y = -1627.9f;
    constexpr float ZG_GONG_Z = 41.3f;
    constexpr float ZG_GONG_SEARCH = 30.0f;

    // Arlokk's emerge/approach delay after the ring; a generous wait bridges
    // the summon without escalating to a stall (the ZF Gahz'rilla number).
    constexpr uint32 ZG_ARLOKK_SPAWN_TIMEOUT = 60000;
    constexpr float ZG_ARLOKK_ENGAGE_SEARCH = 100.0f;

    // Her clear-path slot. Boss order keys are (order - 1): Jeklik 0, Venoxis
    // 1, Mar'li 2, Mandokir 3, Thekal 4, Jin'do 6, Hakkar 7 - slot 5 is hers,
    // between Thekal (tiger) and Jin'do. Same value for the objective's
    // encounterIndex (the event's .Anchored() key) and its orderOverride (the
    // path slot), matching the Zul'Farrak objective convention.
    constexpr uint32 ZG_ARLOKK_ORDER = 5;

    // Event ids on map 309.
    constexpr uint32 ZG_EVENT_ROOM_PRECLEAR = 1;
    constexpr uint32 ZG_EVENT_MANDOKIR = 2;
    constexpr uint32 ZG_EVENT_ARLOKK = 3;
}

void RegisterZulGurubEvents(std::vector<DungeonEvent>& out)
{
    out.push_back(EventBuilder(309, ZG_EVENT_ROOM_PRECLEAR, "Clear the room before the room-aggro boss (Jeklik / Venoxis / Mar'li)")
                      .Conditional(&DcRoomAggroPreClearCondition)
                      .KillCreature(/*room trash*/ 0)
                      .Build());

    out.push_back(EventBuilder(309, ZG_EVENT_MANDOKIR, "Kill the Vilebranch Speaker (Mandokir gate)")
                      .Conditional(&ZgMandokirSpeakerAlive)
                      .Repeatable()
                      .PanelBeforeBoss(/*Bloodlord Mandokir*/ 11382)
                      .KillCreatureEngage(/*Vilebranch Speaker*/ 11391, 1, ZG_SPEAKER_SEARCH)
                      .Timeout(ZG_SPEAKER_TIMEOUT)
                      .Build());

    // The Sacred-Pool shape: ring -> wait for the summon -> engage and kill.
    out.push_back(EventBuilder(309, ZG_EVENT_ARLOKK, "Gong of Bethekk (High Priestess Arlokk)")
                      .Anchored(ZG_ARLOKK_ORDER)
                      .Persistent()
                      .UseGO(ZG_GONG_OF_BETHEKK, ZG_GONG_SEARCH)
                      .WaitForSpawn(ZG_ARLOKK, /*alive*/ true)
                          .Timeout(ZG_ARLOKK_SPAWN_TIMEOUT)
                      .KillCreatureEngage(ZG_ARLOKK, /*count*/ 1, ZG_ARLOKK_ENGAGE_SEARCH)
                      .Build());
}

// --- roster patch : Arlokk as a travel objective --------------------------
void RegisterZulGurubRoster(std::vector<BossRosterPatch>& t)
{
    using namespace DcRoster;

    // Arlokk has no creature spawn, so BossSpawnIndex never emits her and her
    // kill cannot flip a completion bit. The objective anchor gives her a real
    // leg anyway: boss-nav drives the party to the gong exactly as to a boss,
    // the anchored event above does the summon + kill, and the objective
    // latches cleared when the event completes (DcObjectiveArriveAction), so
    // the clear advances without a phantom boss stalling it. Ordered into her
    // slot (key 5, between Thekal the tiger's 4 and Jin'do's 6) so the
    // owner's clear order is kept.
    {
        BossRosterPatch p;
        p.mapId = 309;
        p.add = {
            MakeObjective(BossRosterRegistry::ObjectiveEntry(1), /*encounterIndex*/ ZG_ARLOKK_ORDER,
                          309, "High Priestess Arlokk (Gong of Bethekk)",
                          ZG_GONG_X, ZG_GONG_Y, ZG_GONG_Z,
                          /*arriveRadius*/ 12.0f, /*gateEntry*/ 0,
                          /*hook*/ 0, /*eventId*/ ZG_EVENT_ARLOKK,
                          /*orderOverride*/ ZG_ARLOKK_ORDER),
        };
        t.push_back(std::move(p));
    }
}

// --- the Mandokir prerequisite gate ---------------------------------------
// DUE while Mandokir is still alive AND the Vilebranch Speaker is still up AND
// the tank is at the stairs (within the gate radius of Mandokir's anchor). The
// kill step drives the tank onto the Speaker; once the Speaker dies the
// condition reads false and Mandokir's engage proceeds (he loses the pacify /
// player-immunity flags the moment the Speaker's death runs the script).
namespace
{
    bool ZgMandokirSpeakerAlive(Player* bot, AiObjectContext* /*context*/)
    {
        if (!bot)
            return false;

        // Done for good once Mandokir is dead.
        if (!DcTargeting::FindLiveCreatureOnMap(bot, ZG_MANDOKIR))
            return false;

        // Fire only while the Speaker is still up (the gate he removes).
        if (!DcTargeting::FindLiveCreatureOnMap(bot, ZG_VILEBRANCH_SPEAKER))
            return false;

        // Proximity gate: only once boss-nav has parked the tank at the stairs
        // (this event has no arrival step of its own).
        return bot->GetDistance(ZG_MANDOKIR_ANCHOR_X, ZG_MANDOKIR_ANCHOR_Y,
                                ZG_MANDOKIR_ANCHOR_Z) <= ZG_MANDOKIR_GATE_RANGE;
    }
}
