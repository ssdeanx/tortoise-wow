/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU
 * AGPL v3 license, you may redistribute it and/or modify it under version 3 of
 * the License, or (at your option), any later version.
 */

#include "RoomAggroRegistry.h"

#include <algorithm>

namespace {
// Tier-1 MVP scope (deployment-files/docs/mod-dungeon-clear_room-aggro-bosses
// _plan.md). Radii are read straight from the script that force-pulls the
// room, verified against the live source: SM Cathedral CATHEDRAL_PULL_RANGE
// 80, Pandemonius ROOM_PULL_RANGE 70, Blackheart/Grand-Champions
// CallForHelp(100), Dagran CallForHelp(VISIBLE_RANGE==166), and the SmartAI
// CALL_FOR_HELP radii for Jammal'an (90), Pusillin (50), Krik'thir (40).
//
// memberEntries is left EMPTY (any hostile in radius) on purpose: the plan
// endorses this over-approximation as the safe side (CallForHelp only wakes
// assist-capable, non-evading friendlies, so clearing every hostile in the
// radius is a strict superset), and it avoids hard-coding member entry IDs
// we cannot verify per realm. The boss itself and any other encounter boss
// are excluded at the use site (DcTargeting::IsDungeonBossEntry), and the
// boss-aggro-sphere exclusion (IsRoomTrash) keeps the gate from livelocking
// on a mob glued to the boss. A precise whitelist can be added later as a
// pure optimisation without changing behaviour.
RoomAggroBoss const kRoomAggroBosses[] = {
    // SM Cathedral (189): Mograine -> Whitemane. The grid AttackStart fires
    // on Mograine's engage; both encounter entries are flagged so the room
    // gate holds whichever the boss index selects.
    {189, 3976, 80.0f, {}}, // Highlord Mograine
    {189, 3977, 80.0f, {}}, // Inquisitor Whitemane

    // Scholomance (289): the linked pair Marduk Blackpool / Vectus. They do
    // not pre-aggro but are wired together in SmartAI (each "On Aggro -> Set
    // Data 3" and "On Data Set 3 -> Attack Start"), so pulling EITHER pulls
    // BOTH. They stand ~18yd apart in a chamber of ~32 Scholomance Students;
    // an AoE that wakes one wakes the pair while the room is still up. The DC
    // roster collapses the two into ONE boss anchored on Vectus (see
    // BossRosterRegistry, map 289), so 10432 is the live tracked boss (room
    // centre, nearest the close trash) and 10433 (Marduk, off the roster) is
    // listed here ONLY so the room-trash value's partner check excludes it
    // from being chased as a faction-15 "student". Radius 55 covers the
    // student room; the RoomClearTimeout valve handles far stragglers.
    //
    // CRITICAL: the whole room — students AND the two bosses — stands NEUTRAL
    // (yellow), not hostile, until struck. An EXPLICIT memberEntries whitelist
    // is what makes this work: (1) it scopes "the room" to the students alone,
    // so the unrelated HOSTILE packs that happen to sit within 55yd (Diseased
    // Ghouls to the east, Plagued Hatchlings to the north, Risen Adepts) are
    // NOT counted as room trash — without it those hostiles were the only
    // things the value could see, and as they cleared the count flicked to 0
    // and the boss was pulled with the students still up; and (2) a non-empty
    // whitelist signals DungeonClearRoomTrashValue to clear these entries even
    // though they are neutral (the hostile gate is bypassed for explicit
    // members only — see that file). Marduk is whitelisted identically so the
    // partner row behaves the same if it is ever the tracked boss.
    {289, 10432, 55.0f, {10475}}, // Scholomance — Vectus (tracked boss)
    {289, 10433, 55.0f, {10475}}, // Scholomance — Marduk Blackpool (partner)

    // Zul'Gurub (309) — High Priestess Jeklik (the FIRST boss). Her cave's
    // trash is not script-force-pulled, but the room is pre-cleared before
    // the engage per the server owner (the first boss's room has to be
    // clear), so the bat terrace below is not dragged into her pull.
    // Radius 120 covers her cave plus the bat-rider/witch-doctor landing
    // ~110yd south; the whitelist is the terrace's mobs: Bloodseeker Bat
    // 11368, Gurubashi Bat Rider 14750, Hakkari Witch Doctor 11831,
    // Gurubashi Headhunter 11351. (The invisible Zul'Gurub Trigger 14758
    // beside her is not a creature to clear.) The matching pre-clear event
    // is shared (Data/Events/ZulGurubEvents.cpp, event 1).
    {309, 14517, 120.0f, {11368, 14750, 11831, 11351}}, // Jeklik

    // Zul'Gurub (309) — High Priest Venoxis (second boss). His temple's
    // trash is NOT script-force-pulled, but the room is pre-cleared before
    // the engage for the same reason, and the z-band evade in boss_venoxis
    // (z < 27 or > 43) makes dragging the room out through the doorway
    // unsafe anyway — see the FightInPlaceRegistry zone for map 309. Radius
    // 55 covers the temple interior around him (-12029.8, -1707.9, z 39);
    // the whitelist is the room's own mobs: Razzashi Serpent 11371,
    // Razzashi Adder 11372, Razzashi Cobra 11373 (the script respawns the
    // cobras on his reset), Gurubashi Axe Thrower 11350, Hakkari Priest
    // 11830, Sandfury Speaker 11387.
    {309, 14507, 55.0f, {11371, 11372, 11373, 11350, 11830, 11387}}, // Venoxis

    {555, 18667, 100.0f, {}}, // Shadow Labyrinth — Blackheart the Inciter
    {109,
     5710,
     90.0f,
     {}}, // Sunken Temple — Jammal'an the Prophet
          // Mana-Tombs — Pandemonius. boss_pandemonius::PullRoom() gathers the
          // three room adds within ROOM_PULL_RANGE(70) but force-pulls ONLY
          // those with ROOM_EXIT(-145) < Y < ROOM_ENTERANCE(-50): the
          // Scavengers/Crypt Raiders/Sorcerers BEHIND him (toward Tavarok, Y <=
          // -145) never join the fight. Mirror that exactly — entry whitelist +
          // the Y corridor — or the room-clear fixates on the unreachable
          // behind-the-boss adds, orbits him, and burns the full
          // RoomClearTimeout before giving up (live: kept=2..7 flickering on
          // 18309/18311/18313, far=108+, never 0).
    {557, 18341, 70.0f, {18309, 18311, 18313}, true, -145.0f, -50.0f},
    {601, 28684, 40.0f, {}}, // Azjol-Nerub — Krik'thir the Gatewatcher
    {230, 9019, 166.0f, {}}, // Blackrock Depths — Emperor Dagran Thaurissan
    {429, 14354, 50.0f, {}}, // Dire Maul East — Pusillin

    // Zul'Gurub (309) — High Priestess Mar'li. Her instance
    // SetData(TYPE_MARLI, IN_PROGRESS) walks every live Mar'li-room trash
    // mob onto her victim the moment she is engaged: Razzashi Skitterer
    // 14880, Razzashi Venombrood 14532, Hakkari Shadowcaster 11338,
    // Razzashi Broodwidow 11370. The script's own bound is an AreaId check
    // (3379), not a radius, so the radius here is the over-approximation
    // the registry endorses; the explicit whitelist is what keeps the rest
    // of the zone's spiders out of the pre-clear. Radius 130 covers her
    // room's spawn cluster (measured 21-121yd from her); the
    // RoomClearTimeout valve handles stragglers. The matching pre-clear
    // event lives in Data/Events/ZulGurubEvents.cpp.
    {309, 14510, 130.0f, {14880, 14532, 11338, 11370}}, // Mar'li

    // Deadmines (36) — Gilnid, the goblin foundry.
    //
    // THE RADIUS IS NOT READ FROM A SCRIPT HERE, because there is no script to
    // read: Gilnid's whole SmartAI is a Molten Metal cast, an out-of-combat
    // yell, and a door open on death. He force-pulls nothing. This row rests on
    // the SCHOLOMANCE precedent above — "does not pre-aggro, but the room and
    // the boss cannot be separated by ordinary means" — and 60yd is the
    // measured spawn envelope of the foundry, not a script constant.
    //
    // What makes the room undecomposable: 19 ELITES at level 18, against a
    // level-18 party, standing 7.1-57.6yd from Gilnid. At a same-level elite's
    // ~20yd aggro radius, 17 of the 19 form ONE connected chain —
    // single-linkage clustering finds no pack boundary anywhere in the room, so
    // waking any of them can cascade through the lot. The dynamic pull governor
    // cannot see this: it sizes a pack with 6yd spread / 10yd assist windows,
    // so on a room whose mobs sit 15-20yd apart it reports "estimated 2 aggro"
    // and returns LEEROY every time (424 LEEROY vs 4 ADVANCED across
    // tp-20260815-171416-1). The party then walks in, collects the room, and
    // Gilnid — who is simply the next roster target — joins on top of it.
    //
    // Live (tp-20260815-171416-1): 5 of 20 runs wiped "on Gilnid" with the
    // status line alternating fighting_boss / fighting_trash every 1-2 seconds
    // (tr-...-10: Gilnid, Craftsman, Engineer, Remote-Controlled Golem,
    // Craftsman in fourteen seconds). Four of those five logged EVERY death as
    // ON BOSS with no trash on the sheet at all — Gilnid lands the killing
    // blows, so the record reads like a boss-difficulty problem when it is a
    // room problem.
    //
    // 60 sweeps all 19 (50 would leave the three western-most
    // at 52.0/55.3/57.6). It also takes in the adjacent mine trash — 2 Defias
    // Taskmaster, 7 Strip Miner, 1 Overseer, 1 Miner — which costs nothing in
    // practice: the clear approaches the foundry from the west THROUGH that
    // tunnel, so those are already dead when the gate is evaluated, and
    // RoomClearTimeout is the valve for any straggler that is not.
    //
    // No skirtRadius / pullOutRadius override. The computed boss sphere already
    // does the right thing at both ends: the two Craftsmen at 7.1 and 9.2yd
    // fall INSIDE it and are excluded from room trash, so they come with the
    // boss as a normal 1-boss + 2-adds pull, and everything past it is ordinary
    // clearable room trash.
    {36, 1763, 60.0f, {}}, // Deadmines — Gilnid (goblin foundry)

    // NOTE: Tendris Warpwood (11489) is deliberately NOT a room-aggro boss.
    // The radius-around-the-boss model is the wrong tool for him: live runs
    // showed it clearing the Eldreth ghosts standing behind him (which don't
    // assist the pull) while the mobs that actually matter — the Warpwood
    // treants in the entrance room ~200yd south, on a higher floor — sit far
    // outside any sane boss radius. That room is cleared instead by an
    // anchored ClearRadius objective co-located with crystal generator 1
    // (DireMaulEvents 429/11 / BossRosterRegistry OBJ(8)), ordered first.

    // Trial of the Champion (650) — Grand Champions, CallForHelp(100) on
    // unmount. Both faction rosters flagged; the other live champions are
    // themselves encounter bosses, so they fall out via the boss exclusion
    // and only genuine arena trash (if any) counts as the room.
    {650, 34701, 100.0f, {}}, // Colosos
    {650, 34702, 100.0f, {}}, // Jaelyne Evensong / Ambrose Boltspark
    {650, 34703, 100.0f, {}}, // Lana Stouthammer / Mokra
    {650, 34705, 100.0f, {}}, // Marshal Jacob Alerius
    {650, 35569, 100.0f, {}}, // Eressea Dawnsinger
    {650, 35570, 100.0f, {}}, // Zul'tore
    {650, 35571, 100.0f, {}}, // Runok Wildmane
    {650, 35572, 100.0f, {}}, // Mokra the Skullcrusher
    {650, 35617, 100.0f, {}}, // Visceri

    // NOTE: The Mechanar (554) Mechano-Lord Capacitus had a room-aggro entry
    // here to pre-clear his SE Driller pack, back when the clear reached him
    // LAST on floor 1 (up the corridor from the dead Iron-Hand), which put that
    // pack right on the pull approach. The boss order now runs Gyro-Kill ->
    // Capacitus -> Iron-Hand, so the tank approaches the pit from the NW and
    // the SE pack falls on the post-Capacitus walk down to Iron-Hand — plain
    // trash- clear handles it once the boss is dead. Entry removed (see
    // MechanarEvents).

    // The Mechanar (554) Nethermancer Sepethrea (floor 2). Her chamber holds
    // THREE elite trash groups the party must clear before the pull, and all
    // three should be dragged back WEST toward the entrance (the elevator
    // landing / Nethermancer door at ~x268) — NOT fought in place, because she
    // summons roaming Raging Flames and the fight wants the open floor. From
    // the world-DB spawns + a navmesh probe (TestMechanarElevatorProbe,
    // MechanarSepethreaRoomProbe):
    //   * Pack A  — 2 Astromage + 2 Centurion, ~(273,-23), 60-67yd from her.
    //   * Robots  — 2 Tempest-Forge Destroyers (one stationary, one roaming),
    //               ~(291,29) & (294,-15), 39-43yd out.
    //   * Pack B  — 2 Centurion + 2 Astromage, ~(309,13), only 17-19yd out —
    //               right on her ~18yd aggro line.
    // radius 70 covers the farthest (Pack A at 67yd); the whole set reads
    // hostile so no whitelist is needed. The catch is Pack B: it sits INSIDE
    // her ~28yd exclusion sphere, so the ordinary room-clear would treat it as
    // "comes with the boss" and leave it up. pullOutRadius 14 shrinks the
    // room-trash exclusion to 14yd (below Pack B's 17yd), KEEPING Pack B as
    // clearable room trash, and — because a non-zero pullOutRadius also forces
    // advanced pull-to-camp — the tank drags Pack B out from ~35yd west instead
    // of meleeing it in place. See RoomAggroBoss::pullOutRadius for the
    // coupling that makes this safe.
    //
    // skirtRadius 40 WIDENS the fight standoff (her raw ~28yd sphere is too
    // tight): Pack B sits only 17yd out, so a camp cleared to the generic ~25yd
    // from her still landed the kill on top of her aggro/CallForHelp and woke
    // her while the party was still on the trash. The wider skirt drags Pack
    // B's camp out toward the ~x268 entrance landing before the fight, clear of
    // her and of the open floor her Raging Flames roam. It does NOT reclassify
    // Pack B (membership uses pullOutRadius 14, not the skirt).
    {554,
     19221,
     70.0f,
     {},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 14.0f,
     /*skirtRadius*/ 40.0f},

    // Scarlet Citadel (45) — Sacred Fist Daelus. The courtyard trash
    // (Valiant 2000034, Footman 2000035, Inquisitor 2000033, Interrogator 2000036, Jordan 2000011)
    // must be cleared prior to engaging Daelus, otherwise pulling him with active courtyard
    // mobs triggers chain adds during the proximity tether mechanics.
    {45, 2000000, 60.0f, {2000034, 2000035, 2000033, 2000036, 2000011}},

    // Scarlet Citadel (45) — Grand Sorcerer Ardaeus. The statue hall trash
    // (Inquisitor 2000033, Valiant 2000034, Footman 2000035, Interrogator 2000036) must be
    // pre-cleared before triggering Ardaeus's gossip dialogue so the party can focus DPS
    // on the descending Sun and statue echoes.
    {45, 2000001, 45.0f, {2000033, 2000034, 2000035, 2000036}},

    // Scarlet Citadel (45) — High Inquisitor Mariella. The inquisition chamber
    // trash (Citadel Inquisitor 2000033, Valiant 2000034, Footman 2000035) must be
    // pre-cleared before fighting Mariella to avoid fighting them inside Void Zones.
    {45, 2000002, 45.0f, {2000033, 2000034, 2000035}},

    // Scarlet Citadel (45) — Brother Eric Vesper. The cloister and gauntlet trash
    // (Citadel Interrogator 2000036, Inquisitor 2000033, Valiant 2000034, Footman 2000035,
    // Chaplain 2000005, Sister 2000006) must be cleared so the raid can burst Eric Vesper
    // before Energize (25685) wipes the raid.
    {45, 2000004, 50.0f, {2000036, 2000033, 2000034, 2000035, 2000005, 2000006}},

    // Scarlet Citadel (45) — High General Abbendis. Her inner quarters are guarded by
    // Darkcaller Rayn (2000037) and elite Citadel guards (Inquisitor 2000033, Valiant 2000034,
    // Footman 2000035). Pre-clearing them prevents mortal strike and add overwhelming during
    // her holy cleave execution.
    {45, 2000003, 45.0f, {2000037, 2000033, 2000034, 2000035}},

    // Temple of Ahn'Qiraj (531) — The Prophet Skeram. His chamber is guarded by
    // EIGHT Anubisath Sentinels (15264) in TWO isolated clusters of four, and
    // the sentinels must be dealt with BEFORE he is pulled — a boss pull taken
    // with the room still up fights trash and boss together, which is the
    // worst possible version of this encounter.
    //
    // The self-gather is what makes the room un-decomposable. On aggro a
    // sentinel runs AddSentinelsNear() ->
    // GetCreatureListWithEntryInGrid(..., NPC_SENTINEL, 90.0f) and then
    // CallBuddiesToAttack(), which AttackStarts every sentinel in that
    // 90yd sphere. Measured 3D geometry from the live tw_world spawns around
    // Skeram (-8343.20, 2083.07, 125.65):
    //
    //   near cluster  43.5 / 45.9 / 58.3 / 59.5 yd   (guid
    //   87566/87565/87567/87564) far  cluster 163.9 / 181.9 / 192.5 / 195.9 yd
    //   (guid 87568-87571)
    //
    // Intra-cluster spacing is 18.5yd (near) / 19.2yd (far) but the SMALLEST
    // cross-cluster gap is 120.8yd — comfortably past the script's own 90yd
    // gather radius. So one pull always collects exactly one cluster of four
    // and the room falls as two natural four-packs, never as one eight-pull.
    // The pre-clear therefore walks the room, not the bosses.
    //
    // The whitelist (Anubisath Sentinel 15264 + Obsidian Eradicator 15262) is
    // what makes the generous radius safe: those are the ONLY spawns of those
    // two entries anywhere on 531, so a 210yd sphere around Skeram cannot pick
    // up a single unrelated mob, and it is sized to cover the farthest sentinel
    // (195.9yd) with margin. Both sentinel clusters and all three eradicators
    // (40.6 / 71.5 / 151.1yd) are inside it. Eradicators join on their own
    // EventAI aggro (creature_ai_events 1527602 raises their aggro radius to
    // 40yd) and are ordinary hostile trash besides.
    //
    // pullOutRadius 20 is the load-bearing field. Skeram's OWN engage is
    // 28yd (boss_skeram.cpp MoveInLineOfSight) and his DB detection_range is
    // 20, so the computed exclusion sphere lands in the high-30s/low-40s —
    // exactly where the near cluster sits (42.9yd at its closest). Left to the
    // computed value, the near four would intermittently fall INSIDE the
    // exclusion and be reclassified as "comes with the boss", which is the
    // exact failure this row exists to prevent, and it would flicker run to
    // run with the path-padding setting. 20yd pins the exclusion well below
    // the nearest sentinel so all eight are unambiguously clearable trash.
    // As RoomAggroBoss::pullOutRadius documents, a non-zero value also forces
    // the advanced pull-to-camp maneuver, so the tank drags each cluster out
    // and fights it clear of Skeram instead of meleeing it in his wake —
    // which is the "do not fight the trash and the boss at the same time"
    // requirement made mechanical. No skirtRadius: 28yd is Skeram's true
    // engage range and AggroReach already covers it, so the default computed
    // Skeram — sentinel room
    {531,
     15263,
     210.0f,
     {15264, 15262},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 0.0f},

    // Molten Core (409) — Magmadar (11982): surrounding Core Hound packs
    // (11671) must be swept and dragged out before pulling Magmadar so they
    // don't chain-aggro.
    {409,
     11982,
     70.0f,
     {11671},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 25.0f,
     /*skirtRadius*/ 0.0f},

    // Molten Core (409) — Baron Geddon (12056): patrols through surrounding
    // Lava Surgers (12101), Firelords (11668), and Lava Spawns (12265).
    {409,
     12056,
     80.0f,
     {12101, 11668, 12265},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 30.0f,
     /*skirtRadius*/ 0.0f},

    // Temple of Ahn'Qiraj (531) — Battleguard Sartura (15516): Sartura's Royal
    // Guards (15984)
    {531,
     15516,
     60.0f,
     {15984},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 0.0f,
     /*skirtRadius*/ 0.0f},

    // Lower Karazhan Halls (532) — Brood Queen Araxxna (61221): Skitterweb Eggs
    // and room spiders
    {532,
     61221,
     55.0f,
     {30008, 61206, 61207, 61209},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 0.0f,
     /*skirtRadius*/ 0.0f},

    // Lower Karazhan Halls (532) — Grizikil (61224): Grellkin pre-clear
    {532,
     61224,
     50.0f,
     {61195, 61196, 61197},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Blackrock Spire / UBRS (229) — General Drakkisath (10363): Chromatic
    // Elite Guards (10814) must be dragged out from the bridge / throne
    // platform before pulling Drakkisath.
    {229,
     10363,
     60.0f,
     {10814},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 18.0f,
     /*skirtRadius*/ 35.0f},

    // Stratholme (329) — Baron Rivendare (10440): Slaughter Square abomination
    // sweep before engaging Baron inside the ziggurat.
    {329,
     10440,
     60.0f,
     {10416, 10417, 10394, 11030},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 0.0f,
     /*skirtRadius*/ 0.0f},

    // Scholomance (289) — Ras Frostwhisper (10508): laboratory necromancers and
    // dark summoners.
    {289,
     10508,
     45.0f,
     {10476, 10477},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Scholomance (289) — Darkmaster Gandling (1853): viewing room laboratory
    // trash before Gandling.
    {289,
     1853,
     50.0f,
     {},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 0.0f,
     /*skirtRadius*/ 0.0f},

    // Tower of Karazhan (814) — Sanv Tas'dal (59981): ethereal protectors
    // pre-clear.
    {814,
     59981,
     55.0f,
     {59982, 59983, 59984},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Tower of Karazhan (814) — Ley-Watcher Incantagos (61946): Ley Seekers
    // (59989) channeling Draconic Ley Charge into the dragon must be cleared.
    {814,
     61946,
     50.0f,
     {59989},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Karazhan Crypt (800) — Archlich Enkhraz (91916): tomb chamber pre-clear.
    {800,
     91916,
     50.0f,
     {},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 0.0f,
     /*skirtRadius*/ 0.0f},

    // Timbermaw Hold (819) — Loktanag the Vile (2139): corruption pre-clear.
    {819,
     2139,
     50.0f,
     {62879, 62878},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Timbermaw Hold (819) — Archdruid Kronn (62938): corrupted druid pre-clear.
    {819,
     62938,
     50.0f,
     {62871, 62873, 62877},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Timbermaw Hold (819) — Selenaxx Foulheart (62940): satyr glade pre-clear.
    {819,
     62940,
     50.0f,
     {62881, 62882, 62883, 62884},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Timbermaw Hold (819) — Chieftain Partath (62941): chieftain den pre-clear.
    {819,
     62941,
     55.0f,
     {62867, 62870, 62876, 62942},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Stormwrought Ruins (818) — Deathlord Tidebane (62550)
    {818,
     62550,
     55.0f,
     {62645, 62646, 62647, 62648, 62649, 62650, 62654, 62657, 62658, 62659, 62660},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Stormwrought Ruins (818) — Subjugator Halthas Shadecrest (62652)
    {818,
     62652,
     50.0f,
     {62645, 62646, 62647, 62648, 62649, 62650, 62654, 62657, 62658, 62659, 62660},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Stormwrought Ruins (818) — Eldermaw the Primordial (62661)
    {818,
     62661,
     50.0f,
     {62645, 62646, 62647, 62648, 62649, 62650, 62654, 62657, 62658, 62659, 62660},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Stormwrought Ruins (818) — Mycellakos (62664)
    {818,
     62664,
     50.0f,
     {62645, 62646, 62647, 62648, 62649, 62650, 62654, 62657, 62658, 62659, 62660},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Stormwrought Ruins (818) — Lady Drazare (62665)
    {818,
     62665,
     50.0f,
     {62645, 62646, 62647, 62648, 62649, 62650, 62654, 62657, 62658, 62659, 62660},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Stormwrought Ruins (818) — Ighal'for (62671)
    {818,
     62671,
     60.0f,
     {62645, 62646, 62647, 62648, 62649, 62650, 62654, 62657, 62658, 62659, 62660},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Emerald Sanctum (807) — Erennius (60747)
    {807,
     60747,
     75.0f,
     {60742, 60743, 60744, 61212, 60745, 60746},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Emerald Sanctum (807) — Solnius (60748)
    {807,
     60748,
     75.0f,
     {60742, 60743, 60744, 61212, 60745, 60746},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Molten Core (409) — Lucifron (12118)
    {409,
     12118,
     65.0f,
     {12119, 11667, 11666, 11671, 11669},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Molten Core (409) — Gehennas (12259)
    {409,
     12259,
     65.0f,
     {11661, 11665, 12101},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Molten Core (409) — Garr (12057)
    {409,
     12057,
     65.0f,
     {12099},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Molten Core (409) — Sulfuron Harbinger (12098)
    {409,
     12098,
     65.0f,
     {11662},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Molten Core (409) — Golemagg the Incinerator (11988)
    {409,
     11988,
     70.0f,
     {11672, 11658, 11659},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Blackwing Lair (469) — Broodlord Lashlayer (12017)
    {469,
     12017,
     60.0f,
     {12422, 12460, 12468, 12458},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Blackwing Lair (469) — Firemaw (11983)
    {469,
     11983,
     60.0f,
     {13996, 12457, 12521},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Blackwing Lair (469) — Ebonroc (14601)
    {469,
     14601,
     60.0f,
     {13996, 12459, 12460},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Blackwing Lair (469) — Flamegor (11981)
    {469,
     11981,
     60.0f,
     {13996, 12459, 12460},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Blackwing Lair (469) — Chromaggus (14020)
    {469,
     14020,
     60.0f,
     {13996, 12460},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Zul'Gurub (309) — High Priest Thekal (14599)
    {309,
     14599,
     60.0f,
     {11347, 11348, 11361},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Zul'Gurub (309) — Bloodlord Mandokir (11382)
    {309,
     11382,
     60.0f,
     {14988, 11391, 11352},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Zul'Gurub (309) — Jin'do the Hexxer (11380)
    {309,
     11380,
     60.0f,
     {14987, 14986, 14983, 11352, 11339},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Temple of Ahn'Qiraj (531) — Fankriss the Unyielding (15510)
    {531,
     15510,
     60.0f,
     {15240, 15246, 15247},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Temple of Ahn'Qiraj (531) — Princess Huhuran (15509)
    {531,
     15509,
     60.0f,
     {15249},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 15.0f,
     /*skirtRadius*/ 30.0f},

    // Naxxramas (533) — Anub'Rekhan (15956)
    {533,
     15956,
     60.0f,
     {16573, 15974, 15975},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Grand Widow Faerlina (15953)
    {533,
     15953,
     60.0f,
     {16506, 16505, 15976},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Noth the Plaguebringer (15954)
    {533,
     15954,
     60.0f,
     {16983, 16984, 16982},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Patchwerk (16028)
    {533,
     16028,
     60.0f,
     {16021, 16022, 16020},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Gluth (15932)
    {533,
     15932,
     60.0f,
     {16360, 16029},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Grobbulus (15931)
    {533,
     15931,
     60.0f,
     {16024, 16375, 16021, 16025},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Heigan the Unclean (15936)
    {533,
     15936,
     60.0f,
     {16034, 16036, 16236, 16447},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Loatheb (16011)
    {533,
     16011,
     60.0f,
     {16290, 16243, 16297},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Naxxramas (533) — Instructor Razuvious (16061)
    {533,
     16061,
     60.0f,
     {16145, 16163, 16154, 16067},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},

    // Onyxia's Lair (249) — Onyxia (10184)
    {249,
     10184,
     80.0f,
     {12129, 11262},
     false,
     0.0f,
     0.0f,
     /*pullOutRadius*/ 20.0f,
     /*skirtRadius*/ 35.0f},
};
} // namespace

RoomAggroBoss const *RoomAggroRegistry::Find(uint32 mapId, uint32 bossEntry) {
  for (RoomAggroBoss const &b : kRoomAggroBosses)
    if (b.mapId == mapId && b.bossEntry == bossEntry)
      return &b;
  return nullptr;
}

float RoomAggroRegistry::SkirtOverride(uint32 mapId, uint32 bossEntry) {
  RoomAggroBoss const *b = Find(mapId, bossEntry);
  return b ? b->skirtRadius : 0.0f;
}

bool RoomAggroRegistry::IsMemberEntry(RoomAggroBoss const &boss, uint32 entry) {
  if (boss.memberEntries.empty())
    return true;
  return std::find(boss.memberEntries.begin(), boss.memberEntries.end(),
                   entry) != boss.memberEntries.end();
}

bool RoomAggroRegistry::InRoomBand(RoomAggroBoss const &boss, float worldY) {
  if (!boss.hasYBand)
    return true;
  return worldY > boss.minY && worldY < boss.maxY;
}

bool RoomAggroRegistry::IsRoomTrash(RoomAggroBoss const &boss, uint32 entry,
                                    float distToBoss, float bossSafeRadius) {
  if (!IsMemberEntry(boss, entry))
    return false;
  if (distToBoss > boss.radius)
    return false;
  if (distToBoss <= bossSafeRadius)
    return false;
  return true;
}
