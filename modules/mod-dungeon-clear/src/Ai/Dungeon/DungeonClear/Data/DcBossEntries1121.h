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
639,642,644,645,646,647,1663,1666,1696,1716,1717,1763,1853,2139,2748,3653,3654,3669,3670,3671,3673,3674,3886,3887,3914,3927,3974,3975,4274,4275,4278,4279,4421,4424,4543,4829,4830,4831,4832,4842,4854,4887,5709,5710,5712,5715,5719,5720,5721,5722,5775,6228,6229,6235,6243,6487,6488,6910,7023,7206,7228,7267,7271,7291,7355,7356,7357,7358,7604,7800,8127,8443,8580,8983,9016,9017,9024,9030,9033,9156,9196,9218,9236,9237,9568,9736,9816,9938,10184,10220,10264,10268,10339,10363,10429,10430,10432,10433,10435,10436,10437,10438,10439,10440,10503,10504,10505,10506,10507,10508,10516,10558,10584,10596,10808,10811,10812,10813,10899,10901,10997,11032,11143,11261,11380,11382,11486,11487,11488,11489,11490,11492,11496,11501,11502,11517,11518,11519,11520,11583,11622,11981,11982,11983,11988,12017,12018,12056,12057,12098,12118,12119,12129,12201,12203,12236,12237,12258,12259,12264,12435,13020,13280,13282,13601,14020,14321,14322,14323,14324,14325,14326,14327,14328,14354,14507,14510,14517,14599,14601,14834,14861,15263,15275,15276,15299,15339,15340,15341,15348,15369,15370,15509,15510,15511,15516,15517,15543,15544,15727,15928,15931,15932,15936,15952,15953,15954,15956,15989,15990,16011,16028,16060,16061,16062,16063,16064,16065,40068,50080,50117,52145,57642,59967,59968,59970,59971,59981,59991,60734,60735,60736,60737,60747,60748,60829,61204,61221,61223,61224,61225,61939,61946,61951,61958,61961,61963,61965,61968,61969,62037,62038,62056,62057,62067,62069,62070,62071,62072,62530,62547,62548,62549,62550,62551,62552,62652,62661,62664,62665,62671,62934,62935,62936,62937,62938,62940,62941,62946,63129,63130,63131,63132,63133,65020,65021,80850,80851,80852,80853,80854,91916,91917,91919,91920,91928,91929,92935,93107,2000000,2000001,2000002,2000003,2000004,2000092
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
    // Daelus -> Ardaeus -> Mariella -> Eric Vesper -> Abbendis.
    { 45, 2000000, 1 }, // Sacred Fist Daelus
    { 45, 2000001, 2 }, // Grand Sorcerer Ardaeus
    { 45, 2000002, 3 }, // High Inquisitor Mariella
    { 45, 2000004, 4 }, // Brother Eric Vesper
    { 45, 2000003, 5 }, // High General Abbendis (end boss)

    // Lower Karazhan Halls (map 532): the 5-man half of Turtle's THREE
    // Karazhan maps (532 Halls / 800 Crypt / 814 Tower). This map had no order
    // block and no credit entries at all, so `.dc on` refused it outright with
    // "No bosses found for this map."
    //
    // ONE KEY IS DELIBERATELY VACANT. Lord Blackwald II (61222) is summon-only
    // with no spawn row: instance_lower_karazhan_halls.cpp summons him from
    // Update() once DATA_DR_CHAMPION is DONE, at a hard-coded position. A
    // summon-only boss can never be emitted by BossSpawnIndex, so an order row
    // could never credit him - the same reason Majordomo and Ragnaros are
    // code-side objectives for Molten Core. Order 3 is therefore left empty and
    // filled by an objective in RegisterLowerKarazhanHallsRoster, placed
    // immediately after the Dark Rider Champion whose DONE state summons him.
    // No door or GameObject gate exists on this map, so the remaining order is
    // free-form; Moroes takes the last slot as the deepest encounter.
    { 532, 61221, 1 }, // Brood Queen Araxxna
    { 532, 61204, 2 }, // Dark Rider Champion (summons 2 x 61203 apprentices)
    // key 2 = Lord Blackwald II (objective, see the roster patch)
    { 532, 61223, 4 }, // Clawlord Howlfang
    { 532, 61224, 5 }, // Grizikil
    { 532, 61225, 6 }, // Moroes (4 stages, DATA_MOROES_STAGE 0->3)

    // Karazhan Crypt (map 800): custom 5-man dungeon under Deadwind Pass.
    //
    // Alarus (91928) is summon-only with no spawn row: summoned by trigger_summon_alarus
    // once all 6 Necrotic Runes in Pauper's Walk / Andreon's wing are active.
    // Order 2 (key 1) is left vacant and filled by an objective in RegisterKarazhanCryptRoster.
    // Marrowspike (91920) opens door 177312 to Hivaxxis (91929).
    // Alarus (91928) opens Pauper's Walk door 2006634.
    { 800, 91919, 1 }, // Commander Andreon
    // key 1 = Alarus (objective, see the roster patch)
    { 800, 91917, 3 }, // Corpsemuncher
    { 800, 92935, 4 }, // Guard Captain Gort
    { 800, 91920, 5 }, // Marrowspike (opens door 177312 to Hivaxxis)
    { 800, 91929, 6 }, // Hivaxxis (behind door 177312)
    { 800, 91916, 7 }, // Archlich Enkhraz (end boss)

    // Timbermaw Hold (map 819): the Withermaw Furbolg citadel, added in patch
    // 1.18.1 "Nightmares of Ursol". 20-player limit, entered through
    // areatrigger_teleport 5613. Like 532 it had no order block, so `.dc on`
    // refused it too.
    //
    // There is NO instance script (`map_template.script_name` is empty and there
    // is no src/scripts directory for it) and no door, rune or curse gate on
    // this map, so nothing in the core forces an order - the sequence below is
    // the published encounter order.
    //
    // TWO ROWS THAT ARE NOT BOSSES BY `rank`. Karrsh the Sentinel (62934) and
    // Selenaxx Foulheart (62940) are both named encounters but are rank 0
    // (elite, not elite-boss), so a rank-3-only sweep of the map misses them
    // entirely. They are real fight legs and are listed here.
    //
    // Rotgrowl (62936) and Kodiak (62937) are a PAIRED encounter - the
    // published guide lists them as one fight, "Kodiak & Rotgrowl". They get
    // two consecutive rows rather than one shared row because both must die and
    // nothing in the core couples them: there is no instance script and no
    // EventAI on this map (every 819 template carries ai_name='EventAI' but
    // creature_ai_events has zero rows for every one of them), so there is no
    // death-counter to satisfy.
    //
    // DELIBERATELY INCOMPLETE - THE END BOSSES ARE NOT IMPLEMENTED ON THIS
    // FORK. Ursol (62947, the level's end boss) and Peroth'arn (60686, "The
    // Herald of Nightmares") exist in creature_template as rank 3 but have NO
    // spawn row anywhere, no EventAI rows, no creature_spells, and nothing that
    // summons them. They are absent from the order block on purpose: an order
    // row would never credit them, and a roster OBJECTIVE would stall the run
    // forever waiting on a boss that cannot exist. An incomplete clear is
    // strictly better than a deadlock. If the content lands later they slot in
    // at orders 10 and 11.
    { 819, 62934, 1 }, // Karrsh the Sentinel            (rank 0, named boss)
    { 819, 62936, 2 }, // Rotgrowl                       (paired leg)
    { 819, 62937, 3 }, // Kodiak                         (paired leg)
    { 819, 62935, 4 }, // Ormanos the Cracked
    { 819, 62938, 5 }, // Archdruid Kronn
    { 819, 2139,  6 }, // Loktanag the Vile
    { 819, 62946, 7 }, // Trioch the Devourer
    { 819, 62940, 8 }, // Selenaxx Foulheart             (rank 0, named boss)
    { 819, 62941, 9 }, // Chieftain Partath
    // orders 10/11 reserved: Ursol 62947 + Peroth'arn 60686 - no spawn, no
    // summon, not implemented on this fork. See the note above.

    // Blackrock Spire (map 229): 10-player limit (UBRS 1-6) and 5-player (LBRS 7-15).
    // UBRS: Preceded by the Hall of Binding rune alcove clear (objective at key 0),
    // followed by Emberseer, Solakar in the Rookery, Goraluk, the Stadium gauntlet
    // with Gyth & Rend, The Beast, and General Drakkisath.
    { 229, 9816,  1 }, // Pyroguard Emberseer (released by Hall of Binding altar event)
    { 229, 10264, 2 }, // Solakar Flamewreath (Rookery event)
    { 229, 10899, 3 }, // Goraluk Anvilcrack
    { 229, 10429, 4 }, // Warchief Rend Blackhand (Stadium event with Gyth)
    { 229, 10430, 5 }, // The Beast
    { 229, 10363, 6 }, // General Drakkisath (end boss)

    // LBRS:
    { 229, 9196,  7 },  // Highlord Omokk
    { 229, 9236,  8 },  // Shadow Hunter Vosh'gajin
    { 229, 9237,  9 },  // War Master Voone
    { 229, 10596, 10 }, // Mother Smolderweb
    { 229, 10584, 11 }, // Urok Doomhowl
    { 229, 9736,  12 }, // Quartermaster Zigris
    { 229, 10220, 13 }, // Halycon
    { 229, 10268, 14 }, // Gizrul the Slavener
    { 229, 9568,  15 }, // Overlord Wyrmthalak (end boss)

    // Stormwind Vault (map 35): Turtle custom 5-man dungeon
    { 35, 80851, 1 }, // Volkan Cruelblade
    { 35, 80852, 2 }, // Tham'Grarr
    { 35, 80850, 3 }, // Black Bride
    { 35, 80854, 4 }, // Damian
    { 35, 80853, 5 }, // Aszosh Grimflame
    { 35, 93107, 6 }, // Arc'tiras (end boss)

    // Hateforge Quarry (map 808): Turtle custom 5-man dungeon
    { 808, 60736, 1 }, // Engineer Figgles
    { 808, 60735, 2 }, // High Foreman Bargul Blackhammer
    { 808, 60829, 3 }, // Corrosis
    { 808, 60734, 4 }, // Hatereaver Annihilator
    { 808, 60737, 5 }, // Har'gesh Doomcaller (end boss)

    // Stratholme (map 329): Scarlet Wing (1-5) and Undead Wing (6-11)
    { 329, 10808, 1 },  // Timmy the Cruel
    { 329, 11032, 2 },  // Malor the Zealous
    { 329, 10997, 3 },  // Cannon Master Willey
    { 329, 10811, 4 },  // Archivist Galford
    { 329, 10812, 5 },  // Grand Crusader Dathrohan (Balnazzar)
    { 329, 10436, 6 },  // Baroness Anastari
    { 329, 10437, 7 },  // Nerub'enkan
    { 329, 10438, 8 },  // Maleki the Pallid
    { 329, 10435, 9 },  // Magistrate Barthilas
    { 329, 10439, 10 }, // Ramstein the Gorger
    { 329, 10440, 11 }, // Baron Rivendare (end boss)

    // Scholomance (map 289): 1-13 full clear
    { 289, 14861, 1 },  // Blood Steward of Kirtonos
    { 289, 10506, 2 },  // Kirtonos the Herald
    { 289, 10503, 3 },  // Jandice Barov
    { 289, 11622, 4 },  // Rattlegore
    { 289, 10504, 5 },  // Lord Alexei Barov
    { 289, 10433, 6 },  // Marduk Blackpool
    { 289, 10432, 7 },  // Vectus
    { 289, 10508, 8 },  // Ras Frostwhisper
    { 289, 10505, 9 },  // Instructor Malicia
    { 289, 11261, 10 }, // Doctor Theolen Krastinov
    { 289, 10901, 11 }, // Lorekeeper Polkelt
    { 289, 10507, 12 }, // The Ravenian
    { 289, 1853,  13 }, // Darkmaster Gandling (end boss)

    // Dire Maul (map 429): East (1-5), West (6-10), North (11-17)
    { 429, 14354, 1 },  // Pusillin
    { 429, 14326, 2 },  // Lethtendris
    { 429, 13280, 3 },  // Hydrospawn
    { 429, 11490, 4 },  // Zevrim Thornhoof
    { 429, 11492, 5 },  // Alzzin the Wildshaper
    { 429, 11489, 6 },  // Tendris Warpwood
    { 429, 11488, 7 },  // Illyanna Ravenoak
    { 429, 11487, 8 },  // Magister Kalendris
    { 429, 11496, 9 },  // Immol'thar
    { 429, 11486, 10 }, // Prince Tortheldrin
    { 429, 14321, 11 }, // Guard Mol'dar
    { 429, 14322, 12 }, // Stomper Kreeg
    { 429, 14323, 13 }, // Guard Fengus
    { 429, 14324, 14 }, // Guard Slip'kik
    { 429, 14325, 15 }, // Captain Kromcrush
    { 429, 14328, 16 }, // Cho'Rush the Observer
    { 429, 11501, 17 }, // King Gordok (end boss)

    // Tower of Karazhan / Upper Karazhan (map 814): 10-man / 40-man raid
    { 814, 59967, 1 }, // King's Council (Chess)
    { 814, 59981, 2 }, // Sanv Tas'dal
    { 814, 59991, 3 }, // Kruul
    { 814, 61939, 4 }, // Keeper Gnarlmoon
    { 814, 61946, 5 }, // Ley-Watcher Incantagos
    { 814, 61951, 6 }, // Anomalus
    { 814, 61958, 7 }, // Echo of Medivh (end boss)

    // Black Morass (map 269): Turtle custom level 60
    { 269, 50117, 1 }, // Chronormu
    { 269, 50080, 2 }, // Commander Gerastrasz (end boss)

    // Frostmane Hollow (map 822): Turtle custom dungeon
    { 822, 63129, 1 }, // Kan'za the Seer
    { 822, 63133, 2 }, // Tan'sha the Sleek
    { 822, 63131, 3 }, // Battlemaster Ubukaz
    { 822, 63132, 4 }, // Handler Oboka
    { 822, 63130, 5 }, // Hailar the Frigid (end boss)

    // Stormwrought Ruins (map 818): Turtle custom 20-man raid
    { 818, 62547, 1 },  // Dagar the Glutton
    { 818, 62548, 2 },  // Oronok Torn-Heart
    { 818, 62552, 3 },  // Librarian Theodorus
    { 818, 62549, 4 },  // Duke Balor the IV
    { 818, 62551, 5 },  // Chieftain Stormsong
    { 818, 62550, 6 },  // Deathlord Tidebane
    { 818, 62652, 7 },  // Subjugator Halthas Shadecrest
    { 818, 62661, 8 },  // Eldermaw the Primordial
    { 818, 62664, 9 },  // Mycellakos
    { 818, 62665, 10 }, // Lady Drazare
    { 818, 62671, 11 }, // Ighal'for (end boss)
};
#endif
