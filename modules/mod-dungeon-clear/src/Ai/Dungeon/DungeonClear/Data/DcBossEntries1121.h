#ifndef DC_BOSS_ENTRIES_1121_H
#define DC_BOSS_ENTRIES_1121_H

#include "Define.h"

// Which creature entries count as dungeon bosses on a 1.12 core.
//
// Upstream builds its boss index from DungeonEncounter.dbc. That DBC arrived
// with Wrath; a 1.12 client has no encounter data at all, and creature rank
// cannot stand in for it - Deadmines has VanCleef and his trash both at rank 1.
//
// This list is the curated one from the Kith project's kith_boss table (133
// bosses across the classic instances, Molten Core and Onyxia included),
// baked in as data rather than read from a table so the module works on a
// realm that never imported it. Everything else the index needs - map, spawn
// coordinates, name - joins in from the spawn and template data at load.
//
// Ordering: DungeonEncounter carried an explicit per-dungeon order; this list
// does not. Maps present in DC_BOSS_ORDER_1121 below get the authored order
// (and any door bosses the curated list lacks); every other map falls back to
// ascending-entry numbering. The fallback is NOT harmless in door dungeons:
// live, a Deadmines party routed to Mr. Smite first (lowest reachable index)
// straight past Rhahk'Zor's closed door and walked off the navmesh - add an
// order block when a dungeon misroutes.

inline constexpr uint32 DC_BOSS_ENTRIES_1121[] = {
639,646,647,1663,1666,1696,1716,1717,1853,2748,3653,3654,3669,3670,3671,3673,3674,3886,3887,3914,3927,3974,3975,4274,4275,4278,4279,4421,4424,4543,4829,4830,4831,4832,4842,4854,4887,5709,5710,5712,5715,5719,5720,5721,5722,5775,6228,6229,6235,6243,6487,6488,6910,7023,7206,7228,7267,7271,7291,7355,7356,7357,7358,7604,7800,8127,8443,8580,8983,9016,9017,9024,9030,9033,9156,9196,9218,9236,9237,9568,9816,9938,10184,10220,10264,10339,10363,10429,10430,10432,10433,10435,10437,10440,10504,10507,10508,10516,10558,10584,10596,10811,10812,10813,10901,10997,11143,11488,11489,11490,11492,11496,11502,11517,11518,11519,11520,11622,11982,12018,12056,12057,12098,12118,12119,12129,12201,12203,12236,12237,12258,12259,12264,13280,13282,13601,14321,14323,14325,14326,14327,14354,40068,61961,61963,61965,61968,61969,2000092,63129,63130,63131,63132,63133,62037,62038,62056,62057,62067,62069,62070,62071,62072,62530,60747,60748,2000000,2000001,2000002,2000003
};

// Per-dungeon encounter order, plus the door bosses the curated kith_boss
// list skipped (it carried tactics bosses; the router also needs the bosses
// whose death opens doors). Entries listed here count as bosses even when
// absent from DC_BOSS_ENTRIES_1121. `order` is 1-based; the mask bit is
// order-1, so keep every dungeon's orders inside 1..32.
struct DcBossOrderRow
{
    uint16 mapId;
    uint32 entry;
    uint8 order;
};

inline constexpr DcBossOrderRow DC_BOSS_ORDER_1121[] = {
    // The Stockade (map 34). Two of the five were missing from the credit list,
    // one of them the END boss - Bazil Thredd - so the dungeon would have run
    // as three of five and stopped short of its own finish.
    //
    // Bruegal Ironknuckle (1720) is NOT a member: rank 2, a rare, and waiting
    // on a rare is not an objective. The Defias Insurgent/Convict/Inmate/
    // Captive/Prisoner lines are elite-flagged trash (10-33 spawns each).
    //
    // Order measured on the navmesh from the entrance (49.0, 0.5, -16.4) with
    // tools/meshprobe.cpp, in polygons per leg: entrance->Targorr 32,
    // Targorr->Dextren 43, Dextren->Kam 63, Kam->Hamhock 23, Hamhock->Bazil 9,
    // total 170. Running Dextren last instead (Targorr->Kam 31 ... Bazil->
    // Dextren 79) costs 174 - the same to within noise, but it would end the
    // clear somewhere other than the end boss. Hence Dextren as a northern spur
    // before the southern chain.
    { 34, 1696, 1 },   // Targorr the Dread
    { 34, 1663, 2 },   // Dextren Ward
    { 34, 1666, 3 },   // Kam Deepfury
    { 34, 1717, 4 },   // Hamhock
    { 34, 1716, 5 },   // Bazil Thredd        (end boss)

    // Blackfathom Deeps (map 48). Four of the seven were missing from the
    // credit list entirely - Old Serra'kis, Kelris, Gelihast and Turtle's own
    // Velthelaxx - so the roster read three.
    //
    // Order walked and supplied by the user, and it matches the travel path
    // from the entrance at (-150.2, 106.6, -39.8): Ghamoo-ra -> Sarevess (the
    // approach needs swimming) -> Gelihast -> Velthelaxx -> Old Serra'kis
    // (underwater) -> Kelris -> the four braziers -> Aku'mai.
    //
    // Key 7 is deliberately skipped here: it belongs to the Fires of Aku'mai
    // objective, which BlackfathomDeepsEvents.cpp inserts as OBJ(1) with
    // encounterIndex 7. Aku'mai therefore takes 8 - he cannot be reached
    // before the braziers are lit, since his portal is shut until then.
    //
    // Lorgus Jett (12902) is NOT a member: he is a rare with three possible
    // spots and a spawn chance, and waiting on a rare is not an objective.
    { 48, 4887, 1 },   // Ghamoo-ra
    { 48, 4831, 2 },   // Lady Sarevess
    { 48, 6243, 3 },   // Gelihast
    { 48, 62530, 4 },  // Velthelaxx the Defiler  (Turtle custom)
    { 48, 4830, 5 },   // Old Serra'kis
    { 48, 4832, 6 },   // Twilight Lord Kelris
    { 48, 4829, 8 },   // Aku'mai                 (7 = the braziers, see above)

    // Shadowfang Keep (map 33). Eleven bosses, of which five were missing from
    // the credit list entirely - Razorclaw, Silverlaine, Fenrus, Sever and
    // Turtle's own Prelate Ironmane - so the roster read six.
    //
    // Order measured against the navmesh from the entrance (-228.2, 2111.4,
    // 76.9) with tools/meshprobe.cpp. Those numbers hold for the REGENERATED
    // mesh: with the old one the whole upper keep (Odo, Deathsworn Captain,
    // Fenrus, Nandos, Arugal) had no path from the entrance at all, nor from
    // Springvale, the highest point reachable below - the courtyard stairs were
    // missing from it. Map 33 now carries the same mmapSettings line Turtle
    // uses for Scarlet Monastery, which merges stair steps into walkable
    // surface, and the mesh was rebuilt with it.
    // Order given by the server owner, who knows the place; mesh distance from
    // the entrance only decided nothing here. Sever (14682) is dropped from the
    // roster entirely - the DB spawns it on map 33, but it is Scourge Invasion
    // furniture and not part of the dungeon.
    //
    // Steps 3 and 4 are interchangeable in practice: whether Silverlaine or
    // Ironmane comes first depends on whether the party cuts through the house
    // on the way to Razorclaw.
    { 33, 3914, 1 },   // Rethilgore            147yd
    { 33, 3886, 2 },   // Razorclaw the Butcher 259
    { 33, 3887, 3 },   // Baron Silverlaine     343
    { 33, 61969, 4 },  // Prelate Ironmane      443   (Turtle custom, up the stairs right)
    { 33, 4278, 5 },   // Commander Springvale  454
    { 33, 4279, 6 },   // Odo the Blindwatcher  601   (gateway to the upper keep)
    // Deathsworn Captain (3872) is out of the roster as well: pool 1601 with
    // chance 30, so he stands in about one instance in three. Waiting on a rare
    // makes two runs out of three impossible to finish, and a real group could
    // not clear him either when he is not there.
    { 33, 4274, 7 },   // Fenrus the Devourer   1039
    { 33, 3927, 8 },   // Wolf Master Nandos    1311
    { 33, 4275, 9 },   // Archmage Arugal       1380

    // Wailing Caverns (map 43). Without these rows the map got no order at
    // all and the indices fell out in build order - Verdan 0, so the party
    // went for him FIRST. He is the one boss with no navmesh path from the
    // entrance (measured with tools/meshprobe.cpp: no complete corridor from
    // (-158,132,-74), and the only start that has one is Lord Serpentis's
    // shelf, 69yd away). Verdan sits BEHIND the drop, so every run opened by
    // searching 1100-1600yd for a boss it could not reach until somebody
    // stumbled into him.
    //
    // The keys leave room for the event objectives, which carry their own:
    // the drop to Serpentis is 5 and the hole-drop/escort are 7 (see
    // WailingCavernsEvents.cpp), so Verdan takes 7 (index 6) and Mutanus 8.
    { 43, 3671, 1 },  // Lady Anacondra   (221yd from the entrance; 4 pool spawns, one alive)
    { 43, 3653, 2 },  // Kresh            (298yd)
    { 43, 3669, 3 },  // Lord Cobrahn     (897yd)
    { 43, 3670, 4 },  // Lord Pythas      (1193yd)
    { 43, 3674, 5 },  // Skum             (western water; 360yd on from Pythas)
    { 43, 61965, 6 }, // Vangros          (Turtle custom, 693yd on from SKUM - and no
                      //                   corridor at all from Pythas, which is why he
                      //                   comes after Skum and not before)
    { 43, 61968, 7 }, // Zandara Windhoof (Turtle custom, 1229yd)
    //  key 8 (index 7) is the drop-to-Serpentis objective — see
    //  WailingCavernsEvents.cpp. Everything below it is only reachable from
    //  the shelf that drop lands on.
    { 43, 3673, 9 },  // Lord Serpentis   (on the landing shelf)
    { 43, 5775, 10 }, // Verdan the Everliving (69yd off Serpentis's shelf, and
                      //                        unreachable from anywhere else)
    //  key 11 (index 10) is the hole-drop and the Naralex escort.
    { 43, 3654, 12 }, // Mutanus the Devourer (summoned at the end of the escort)

    // The Deadmines (map 36): Rhahk'Zor -> Sneed's Shredder (Sneed rides it)
    // -> Gilnid -> Mr. Smite -> Captain Greenskin -> VanCleef -> Cookie.
    { 36,  644, 1 },  // Rhahk'Zor (opens the first door)
    { 36,  642, 2 },  // Sneed's Shredder (door)
    { 36, 1763, 3 },  // Gilnid (door)
    // Turtle's custom Deadmines wing (the alchemy lab), between the foundry
    // and the Iron Clad Door. Both carry the boss signature - fixed level and
    // several times the surrounding trash's health - while the Chemist (61959),
    // Mixologist (61960) and Manufactured Golem (61962) around them are lab
    // guards and stay trash.
    { 36, 61961, 4 },  // Jared Voss (-85,-526,54)
    { 36, 61963, 5 },  // Masterpiece Harvester (-67,-570,51)
    { 36,  646, 6 },  // Mr. Smite
    { 36,  647, 7 },  // Captain Greenskin
    { 36,  639, 8 },  // Edwin VanCleef
    { 36,  645, 9 },  // "Cookie"

    // Molten Core (map 409): full clear order, including Turtle's custom MC
    // bosses (verified in tw_world: Incindis 52145, the Twin Golems Basalthar
    // 65020 + Smoldaris 65021, Sorcerer-Thane Thaurissan 57642). Without an
    // authored block the fallback is ascending-entry numbering, which puts
    // Ragnaros (11502) at index 0 - the party would route straight at him past
    // every closed rune gate. Golemagg (11988) was also missing from the credit
    // list; the order line credits him, so the rune chain's boss set is
    // complete. An order line credits its entry, so the Turtle bosses need
    // nothing but their rows here.
    //
    // Keys are order-1. Two slots are deliberately skipped and filled by
    // code-side OBJECTIVES (RegisterMoltenCoreRoster, MoltenCoreEvents.cpp):
    //   * key 5 = the Shadowforge Descent bridge (between Smoldaris and
    //     Thaurissan; the navmesh break at 1035,-1275);
    //   * keys 13/14 = Majordomo Executus and Ragnaros - both summon-only (no
    //     spawn rows), so BossSpawnIndex can never emit them.
    // 12119 (Flamewaker Protector) is dropped by the roster patch: it is an
    // add, not a boss leg.
    //
    // Order is the proposed geometry-natural sequence (Lucifron -> Magmadar ->
    // Incindis -> the Twin Golems -> the Shadowforge camp -> Thaurissan, then
    // the classic ring); the owner confirms it in game.
    { 409, 12118, 1 },  // Lucifron
    { 409, 11982, 2 },  // Magmadar
    { 409, 52145, 3 },  // Incindis                  (Turtle custom)
    { 409, 65020, 4 },  // Basalthar                 (Turtle custom, Twin Golems)
    { 409, 65021, 5 },  // Smoldaris                 (Turtle custom, Twin Golems)
    // key 5 = Shadowforge Descent bridge objective (code-side)
    { 409, 57642, 7 },  // Sorcerer-Thane Thaurissan (Turtle custom)
    { 409, 12259, 8 },  // Gehennas
    { 409, 12057, 9 },  // Garr
    { 409, 12264, 10 }, // Shazzrah
    { 409, 12056, 11 }, // Baron Geddon
    { 409, 12098, 12 }, // Sulfuron Harbinger
    { 409, 11988, 13 }, // Golemagg the Incinerator
    // keys 13/14 = Majordomo / Ragnaros objectives (code-side)

    // Blackwing Lair (map 469): full clear order. The fallback here would put
    // Nefarian (11583) first. Razorgore -> Vaelastrasz -> Broodlord Lashlayer ->
    // the drake wing (Firemaw -> Ebonroc -> Flamegor, their rooms are sequential) ->
    // Chromaggus -> Nefarian. Turtle entries verified in tw_world: Firemaw is
    // 11983 and Flamegor 11981 - not the other way round.
    { 469, 12435, 1 }, // Razorgore the Untamed
    { 469, 13020, 2 }, // Vaelastrasz the Corrupt
    { 469, 12017, 3 }, // Broodlord Lashlayer
    { 469, 11983, 4 }, // Firemaw
    { 469, 14601, 5 }, // Ebonroc
    { 469, 11981, 6 }, // Flamegor
    { 469, 14020, 7 }, // Chromaggus
    { 469, 11583, 8 }, // Nefarian

    // Ruins of Ahn'Qiraj (map 509): full clear order, following the instance
    // encounter enum (TYPE_KURINNAXX -> TYPE_OSSIRIAN). The fallback would sort
    // by entry, which puts Ossirian (15339) first - straight past every locked
    // encounter. General Rajaxx (15341) is not a normal boss spawn: Andorov
    // (15471) spawns after Kurinnaxx and the seven officer waves (Qeez 15391,
    // Tuubid 15392, Drenn 15389, Xurrem 15390, Yeggeth 15386, Pakkon 15388,
    // Zerran 15385) start on his gossip or on any arena combat. Order measured
    // against the instance layout, re-validate routing in game.
    { 509, 15348, 1 }, // Kurinnaxx
    { 509, 15341, 2 }, // General Rajaxx (event)
    { 509, 15370, 3 }, // Buru the Gorger
    { 509, 15340, 4 }, // Moam
    { 509, 15369, 5 }, // Ayamiss the Hunter
    { 509, 15339, 6 }, // Ossirian the Unscarred (end boss)

    // Temple of Ahn'Qiraj (map 531): full clear order. Skeram's gate (180636)
    // opens on his death; Huhuran opens the Twins' door (180634) and the Twins
    // open the exit to C'Thun (180635), so those three anchor the order. The
    // Bug Trio (all three must die before their devour mechanic stops healing
    // the survivors) sits after the gate; Viscidus is the side room between
    // Fankriss and Huhuran. Ouro (15517) has no static spawn - he is summoned
    // by the Ouro Spawner (15957) when the raid approaches - so he is credited
    // by the order line like Ragnaros/Nefarian. Route re-validate in game.
    { 531, 15263, 1 },  // The Prophet Skeram
    { 531, 15544, 2 },  // Vem
    { 531, 15543, 3 },  // Princess Yauj
    { 531, 15511, 4 },  // Lord Kri
    { 531, 15516, 5 },  // Battleguard Sartura
    { 531, 15510, 6 },  // Fankriss the Unyielding
    { 531, 15299, 7 },  // Viscidus
    { 531, 15509, 8 },  // Princess Huhuran
    { 531, 15275, 9 },  // Emperor Vek'nilash
    { 531, 15276, 10 }, // Emperor Vek'lor
    { 531, 15517, 11 }, // Ouro (summoned)
    { 531, 15727, 12 }, // C'Thun (end boss)

    // Naxxramas (map 533): wing-grouped clear order. The four wings are entered
    // from the central hub (any wing order works; each wing's bosses are kept
    // together below), then Sapphiron and Kel'Thuzad. The Four Horsemen carry
    // four entries (one order each so the mask credits all of them). The
    // Abomination wing's Patchwerk/Grobbulus/Gluth/Thaddius are door-chained.
    // Wing routing through the hub portals to be validated in game.
    { 533, 15956, 1 },  // Anub'Rekhan          (spider wing)
    { 533, 15953, 2 },  // Grand Widow Faerlina
    { 533, 15952, 3 },  // Maexxna
    { 533, 15954, 4 },  // Noth the Plaguebringer (plague wing)
    { 533, 15936, 5 },  // Heigan the Unclean
    { 533, 16011, 6 },  // Loatheb
    { 533, 16061, 7 },  // Instructor Razuvious  (deathknight wing)
    { 533, 16060, 8 },  // Gothik the Harvester
    { 533, 16062, 9 },  // Highlord Mograine
    { 533, 16063, 10 }, // Sir Zeliek
    { 533, 16064, 11 }, // Thane Korth'azz
    { 533, 16065, 12 }, // Lady Blaumeux
    { 533, 16028, 13 }, // Patchwerk             (abomination wing)
    { 533, 15931, 14 }, // Grobbulus
    { 533, 15932, 15 }, // Gluth
    { 533, 15928, 16 }, // Thaddius
    { 533, 15989, 17 }, // Sapphiron
    { 533, 15990, 18 }, // Kel'Thuzad (end boss)

    // Zul'Gurub (map 309): clear order per the server owner -
    //   bat (Jeklik) -> snake (Venoxis) -> spider (Mar'li) -> raptor
    //   (Mandokir) -> tiger (Thekal) -> panther (Arlokk) -> Jin'do -> Hakkar.
    // Boss order keys are (order - 1): Jeklik 0, Venoxis 1, Mar'li 2,
    // Mandokir 3, Thekal 4, Arlokk 5 (objective), Jin'do 6, Hakkar 7;
    // priests before Hakkar (each dead priest removes a Hakkar Power stack and
    // its Aspect). Arlokk (14515) has NO creature spawn - she is summoned by
    // the Gong of Bethekk (GO 180526) - so she is NOT an order row: she is
    // added as a travel OBJECTIVE at order key 5 by RegisterZulGurubRoster
    // (BossRosterRegistry patch, see Data/Events/ZulGurubEvents.cpp), with an
    // anchored event that rings the gong, waits for her and kills her; the
    // objective latches when that event completes. Gahz'ranka (15114) is
    // deliberately absent (summon-only via the Mudskunk Lure, a fishing item -
    // skipped per user direction). The Edge of Madness bosses (15082-15085)
    // are not spawnable in tw_world at all. Route to re-validate in game.
    { 309, 14517, 1 }, // High Priestess Jeklik   (bat - first)
    { 309, 14507, 2 }, // High Priest Venoxis     (snake)
    { 309, 14510, 3 }, // High Priestess Mar'li   (spider)
    { 309, 11382, 4 }, // Bloodlord Mandokir      (raptor)
    { 309, 14599, 5 }, // High Priest Thekal      (tiger; phase 1, tiger form 14509)
    // key 5 = High Priestess Arlokk (panther; objective, see the roster patch)
    { 309, 11380, 7 }, // Jin'do the Hexxer
    { 309, 14834, 8 }, // Hakkar the Soulflayer   (end boss)

    // Emerald Sanctum (map 807).
    { 807, 60747, 1 }, // Erennius
    { 807, 60748, 2 }, // Solnius (end boss)

    // Onyxia's Lair (map 249): single encounter raid.
    { 249, 10184, 1 }, // Onyxia (end boss)

    // Scarlet Citadel (map 45): full clear order.
    // Daelus -> Ardaeus -> Mariella -> Abbendis.
    { 45, 2000000, 1 }, // Sacred Fist Daelus
    { 45, 2000001, 2 }, // Grand Sorcerer Ardaeus
    { 45, 2000002, 3 }, // High Inquisitor Mariella
    { 45, 2000003, 4 }, // High General Abbendis (end boss)
};
#endif
