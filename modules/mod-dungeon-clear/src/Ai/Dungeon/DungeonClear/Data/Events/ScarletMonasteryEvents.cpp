/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonEventTables.h"
#include "Ai/Dungeon/DungeonClear/Data/Events/DungeonRosterBuilders.h"
#include "Ai/Dungeon/DungeonClear/Data/DungeonWingRegistry.h"

#include <unordered_map>

// --- Scarlet Monastery Cathedral (map 189) — ROOM-AGGRO PRE-CLEAR ---------
// Highlord Mograine's engage fires a grid AttackStart that drags the WHOLE
// cathedral into combat (CATHEDRAL_PULL_RANGE 80); pulling him before the room
// is clear wipes the party. Milestone 3 re-expresses that pre-clear (formerly
// the standalone RoomAggroRegistry path) as a CONDITIONAL room-aggro event:
// condition 3 (shared — see SharedConditions.cpp) reads DUE while the room-trash
// value (the RoomAggroRegistry geometry around the LIVE boss, minus the boss
// aggro sphere / unreachable / door-blocked, with the RoomClearTimeout give-up
// valve) still has anything to clear, and the lone KillCreature(0 = room-trash
// mode) step gates the boss pull until it is empty. DcRunEventAction drives the
// actual engage (nearest room trash first); the spatial logic stays in
// DungeonClearRoomTrashValue. Required (hold the pull until clear or the value
// gives up) and NEVER latched — the same row re-fires for each room-aggro boss
// on the map (Mograine, then Whitemane). This is the SM Cathedral test bed;
// other RoomAggroRegistry maps migrate by adding one more row each (and keep the
// legacy path until they do).

void RegisterScarletMonasteryEvents(std::vector<DungeonEvent>& out)
{
    out.push_back(EventBuilder(189, 1, "Clear the Cathedral (room-aggro pre-clear)")
                      .Conditional(&DcRoomAggroPreClearCondition)
                      .KillCreature(/*room trash*/ 0)
                      .Build());

    // KEIN Tuer-Ereignis fuer die Kathedrale. Eines stand hier am 2026-09-02
    // und wurde am selben Tag wieder entfernt: die Secret Door (GO 97700) vor
    // High Inquisitor Fairbanks oeffnet die GENERISCHE Tuerbehandlung des
    // Moduls von selbst -
    //     blocking-door: flagged 'Secret Door' (97700) 0.9yd from bot
    //     door-blocked:  opening 'Secret Door' as a player would (entitled)
    // - und zwar dieselbe Logik, die nebenbei auch die Chapel Door aufmacht.
    //
    // Das Ereignis hat nie gefeuert (0 Ausfuehrungen, Fackel 97701 nie benutzt)
    // und der Fluegel wurde trotzdem 3 von 3 geraeumt. Wer hier wieder eines
    // einbaut, sollte zuerst pruefen, ob die Tuer nicht laengst aufgeht.
}

// --- roster patch (relocated from BossRosterRegistry) --------------------
void RegisterScarletMonasteryRoster(std::vector<BossRosterPatch>& t)
{
    // Nothing here any more. The whole Cathedral correction moved into
    // data/dc_roster.txt on 2026-09-02, where the server owner dictates the
    // walked order for all four wings and it reloads without a build.
    //
    // What used to live here: Whitemane (3977) was REMOVED as event-locked and
    // Scarlet Commander Mograine (3976) injected in her place, borrowing her
    // kill-bit through inheritCompletionFrom because he has no DungeonEncounter
    // row of his own. The reasoning was sound - she is only attackable after
    // Mograine is engaged, so an anchor on her stalled the tank - but it was a
    // fix for the ORDER, not for her. Ordered AFTER Mograine, which is the real
    // fight (he dies, she resurrects him, you kill her, then him again), she is
    // a perfectly good target and the Cathedral is not actually clear until she
    // is down.
    //
    // The file gives both of them their own credit and their own kill-bit
    // (an `order` line credits its entry; the bit is index-1), so the borrow is
    // no longer needed. Re-adding Mograine here as well would duplicate him:
    // DcRosterFile feeds BossSpawnIndex, which is the `base` this patch is
    // applied ON TOP of.
    //
    (void)t;
}

// --- wing layout (relocated from DungeonWingRegistry) --------------------
void RegisterScarletMonasteryWings(std::unordered_map<uint32, DungeonWingLayout>& store)
{
    // --- Scarlet Monastery (map 189) -----------------------------
    // Four wings, each entered through its own portal off the shared
    // outdoor courtyard; you must leave to the courtyard to switch, so
    // no in-instance route connects them. The wing clusters sit far
    // apart in world space — Graveyard (x~1800, y~1270) and Cathedral
    // (x~1160, y~1370) in the north half, Library (x~130, y~-345) and
    // Armory (x~1965, y~-430) in the south half, each 600+ yds from the
    // others — so nearest-boss wing detection is unambiguous.
    // isolated == true: filter to the bot's wing.
    //
    // Entries are the kill-creature credit-entries from
    // instance_encounters (what BossSpawnIndex emits) PLUS any boss
    // injected by BossRosterRegistry. The Cathedral's tracked encounters
    // are Fairbanks and Whitemane; the roster patch removes Whitemane
    // (event-locked) and injects Scarlet Commander Mograine (3976), so
    // 3976 is listed here too — otherwise the wing filter, which runs
    // after the patch, would drop the injected boss.
    store[189] = {true, {
        {"Scarlet Monastery (Graveyard)", {
            3983,   // Interrogator Vishas
            61972,  // Duke Dreadmoore  (Turtle custom)
            4543,   // Bloodmage Thalnos
        }},
        {"Scarlet Monastery (Library)", {
            3974,   // Houndmaster Loksey
            61983,  // Brother Wystan   (Turtle custom)
            6487,   // Arcanist Doan
        }},
        {"Scarlet Monastery (Armory)", {
            61982,  // Armory Quartermaster Daghelm  (Turtle custom)
            3975,   // Herod
        }},
        {"Scarlet Monastery (Cathedral)", {
            4542,   // High Inquisitor Fairbanks
            3976,   // Scarlet Commander Mograine
            3977,   // High Inquisitor Whitemane — a real target again since
                    // 2026-09-02, ordered after Mograine (see dc_roster.txt).
        }},
    }};
}
