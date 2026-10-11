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

// --- Naxxramas (map 533) ---------------------------------------------------
//
// 40-man classic raid across 5 quarters:
// 1. Spider Wing (Arachnid Quarter):
//    - Anub'Rekhan (15956, order 1): Crypt Guards (16573) cleave & web, Corpse Scarabs (16698).
//    - Grand Widow Faerlina (15953, order 2): Naxxramas Worshipper (16506) & Follower (16505).
//    - Maexxna (15952, order 3): Web Wrap (16486) cocoon rescue & Maexxna Spiderlings (17055).
// 2. Plague Wing (Plague Quarter):
//    - Noth the Plaguebringer (15954, order 4): Plagued Champion (16983), Guardian (16984), Warrior (16982).
//    - Heigan the Unclean (15936, order 5): Dance gauntlet & room trash.
//    - Loatheb (16011, order 6): Corrupted Mind / Spores room pre-clear.
// 3. Deathknight Wing (Military Quarter):
//    - Instructor Razuvious (16061, order 7): Death Knight Understudies (16803).
//    - Gothik the Harvester (16060, order 8): Waves of living adds (16124, 16125, 16126) and
//      spectral adds (16127, 16148, 16149, 16150).
//    - Four Horsemen (16062, 16063, 16064, 16065, orders 9-12).
// 4. Abomination Wing (Construct Quarter):
//    - Patchwerk (16028, order 13): Hateful Strike DPS race, sludge trash pre-clear.
//    - Grobbulus (15931, order 14): Slime river pre-clear.
//    - Gluth (15932, order 15): Zombie Chow (16360) devours healing boss, must be prioritized.
//    - Thaddius (15928, order 16): Stalagg (15929) and Feugen (15930) dual platform adds.
// 5. Frostwyrm Lair & Upper Citadel:
//    - Sapphiron (15989, order 17).
//    - Kel'Thuzad (15990, order 18): Phase 1 Soldiers (16427), Abominations (16428), Weavers (16429),
//      Phase 3 Guardians of Icecrown (16441).

namespace
{
    constexpr uint32 NAXX_MAP = 533;

    // Boss entries
    constexpr uint32 NAXX_ANUBREKHAN          = 15956;
    constexpr uint32 NAXX_FAERLINA            = 15953;
    constexpr uint32 NAXX_MAEXXNA             = 15952;
    constexpr uint32 NAXX_NOTH                = 15954;
    constexpr uint32 NAXX_HEIGAN              = 15936;
    constexpr uint32 NAXX_LOATHEB             = 16011;
    constexpr uint32 NAXX_RAZUVIOUS           = 16061;
    constexpr uint32 NAXX_GOTHIK              = 16060;
    constexpr uint32 NAXX_PATCHWERK           = 16028;
    constexpr uint32 NAXX_GROBBULUS           = 15931;
    constexpr uint32 NAXX_GLUTH               = 15932;
    constexpr uint32 NAXX_THADDIUS            = 15928;
    constexpr uint32 NAXX_SAPPHIRON           = 15989;
    constexpr uint32 NAXX_KELTHUZAD           = 15990;

    // Add entries
    constexpr uint32 NAXX_CRYPT_GUARD         = 16573;
    constexpr uint32 NAXX_CORPSE_SCARAB       = 16698;
    constexpr uint32 NAXX_FOLLOWER            = 16505;
    constexpr uint32 NAXX_WORSHIPPER          = 16506;
    constexpr uint32 NAXX_WEB_WRAP            = 16486;
    constexpr uint32 NAXX_SPIDERLING          = 17055;
    constexpr uint32 NAXX_PLAGUED_CHAMPION    = 16983;
    constexpr uint32 NAXX_PLAGUED_GUARDIAN    = 16984;
    constexpr uint32 NAXX_PLAGUED_WARRIOR     = 16982;
    constexpr uint32 NAXX_DK_UNDERSTUDY       = 16803;
    constexpr uint32 NAXX_UNREL_TRAINEE       = 16124;
    constexpr uint32 NAXX_UNREL_DEATH_KNIGHT  = 16125;
    constexpr uint32 NAXX_UNREL_RIDER         = 16126;
    constexpr uint32 NAXX_SPECT_TRAINEE       = 16127;
    constexpr uint32 NAXX_SPECT_DEATH_KNIGHT  = 16148;
    constexpr uint32 NAXX_SPECT_RIDER         = 16150;
    constexpr uint32 NAXX_SPECT_HORSE         = 16149;
    constexpr uint32 NAXX_ZOMBIE_CHOW         = 16360;
    constexpr uint32 NAXX_FALLOUT_SLIME       = 16290;
    constexpr uint32 NAXX_STALAGG             = 15929;
    constexpr uint32 NAXX_FEUGEN              = 15930;
    constexpr uint32 NAXX_SOLDIER_FROZEN      = 16427;
    constexpr uint32 NAXX_UNSTOPPABLE_ABOM    = 16428;
    constexpr uint32 NAXX_SOUL_WEAVER         = 16429;
    constexpr uint32 NAXX_GUARDIAN_ICECROWN   = 16441;

    // Clear-path order keys (order - 1)
    constexpr uint32 NAXX_ORDER_ANUBREKHAN    = 0;   // order 1
    constexpr uint32 NAXX_ORDER_FAERLINA      = 1;   // order 2
    constexpr uint32 NAXX_ORDER_MAEXXNA       = 2;   // order 3
    constexpr uint32 NAXX_ORDER_NOTH          = 3;   // order 4
    constexpr uint32 NAXX_ORDER_RAZUVIOUS     = 6;   // order 7
    constexpr uint32 NAXX_ORDER_GOTHIK        = 7;   // order 8
    constexpr uint32 NAXX_ORDER_PATCHWERK     = 12;  // order 13
    constexpr uint32 NAXX_ORDER_GROBBULUS     = 13;  // order 14
    constexpr uint32 NAXX_ORDER_GLUTH         = 14;  // order 15
    constexpr uint32 NAXX_ORDER_THADDIUS      = 15;  // order 16
    constexpr uint32 NAXX_ORDER_SAPPHIRON     = 16;  // order 17
    constexpr uint32 NAXX_ORDER_KELTHUZAD     = 17;  // order 18

    // Event IDs
    constexpr uint32 NAXX_EVENT_ROOM_PRECLEAR = 1;
    constexpr uint32 NAXX_EVENT_ANUBREKHAN    = 2;
    constexpr uint32 NAXX_EVENT_FAERLINA      = 3;
    constexpr uint32 NAXX_EVENT_MAEXXNA       = 4;
    constexpr uint32 NAXX_EVENT_NOTH          = 5;
    constexpr uint32 NAXX_EVENT_RAZUVIOUS     = 6;
    constexpr uint32 NAXX_EVENT_GOTHIK        = 7;
    constexpr uint32 NAXX_EVENT_GLUTH         = 8;
    constexpr uint32 NAXX_EVENT_THADDIUS      = 9;
    constexpr uint32 NAXX_EVENT_KELTHUZAD     = 10;
    constexpr uint32 NAXX_EVENT_GROBBULUS     = 11;
    constexpr uint32 NAXX_EVENT_PATCHWERK     = 12;
    constexpr uint32 NAXX_EVENT_SAPPHIRON     = 13;
}

void RegisterNaxxramasEvents(std::vector<DungeonEvent>& out)
{
    // 1. Generic room-aggro pre-clear for Naxxramas bosses
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_ROOM_PRECLEAR,
                              "Clear the room before the room-aggro boss (Naxxramas)")
                      .Conditional(&DcRoomAggroPreClearCondition)
                      .KillCreature(/*room trash*/ 0)
                      .Build());

    // 2. Anub'Rekhan: prioritize Crypt Guards and Corpse Scarabs
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_ANUBREKHAN, "Anub'Rekhan & Crypt Guards")
                      .Anchored(NAXX_ORDER_ANUBREKHAN)
                      .KillCreatureEngage(NAXX_CORPSE_SCARAB, /*count*/ 0, 70.0f)
                      .KillCreatureEngage(NAXX_CRYPT_GUARD, /*count*/ 0, 70.0f)
                      .KillCreatureEngage(NAXX_ANUBREKHAN, /*count*/ 1, 70.0f)
                      .Timeout(300000)
                      .Build());

    // 3. Grand Widow Faerlina: prioritize Followers and Worshippers
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_FAERLINA, "Grand Widow Faerlina & Worshippers")
                      .Anchored(NAXX_ORDER_FAERLINA)
                      .KillCreatureEngage(NAXX_FOLLOWER, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_WORSHIPPER, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_FAERLINA, /*count*/ 1, 80.0f)
                      .Timeout(300000)
                      .Build());

    // 4. Maexxna: prioritize freeing cocooned members via Web Wrap, AoE spiderlings
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_MAEXXNA, "Maexxna & Web Wraps / Spiderlings")
                      .Anchored(NAXX_ORDER_MAEXXNA)
                      .KillCreatureEngage(NAXX_WEB_WRAP, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_SPIDERLING, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_MAEXXNA, /*count*/ 1, 80.0f)
                      .Timeout(300000)
                      .Build());

    // 5. Noth the Plaguebringer: prioritize Champions, Guardians, and Warriors
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_NOTH, "Noth the Plaguebringer & Skeletal Waves")
                      .Anchored(NAXX_ORDER_NOTH)
                      .KillCreatureEngage(NAXX_PLAGUED_CHAMPION, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_PLAGUED_GUARDIAN, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_PLAGUED_WARRIOR, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_NOTH, /*count*/ 1, 80.0f)
                      .Timeout(600000)
                      .Build());

    // 6. Instructor Razuvious: prioritize Death Knight Understudies
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_RAZUVIOUS, "Instructor Razuvious & Understudies")
                      .Anchored(NAXX_ORDER_RAZUVIOUS)
                      .KillCreatureEngage(NAXX_DK_UNDERSTUDY, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_RAZUVIOUS, /*count*/ 1, 80.0f)
                      .Timeout(300000)
                      .Build());

    // 7. Gothik the Harvester: wave clearance across living & spectral sides
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_GOTHIK, "Gothik the Harvester & Undead Waves")
                      .Anchored(NAXX_ORDER_GOTHIK)
                      .KillCreatureEngage(NAXX_UNREL_TRAINEE, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_UNREL_DEATH_KNIGHT, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_UNREL_RIDER, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_SPECT_TRAINEE, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_SPECT_DEATH_KNIGHT, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_SPECT_RIDER, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_SPECT_HORSE, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_GOTHIK, /*count*/ 1, 90.0f)
                      .Timeout(600000)
                      .Build());

    // 8. Patchwerk: enrage timer & Hateful Strike DPS race
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_PATCHWERK, "Patchwerk Hateful Strike DPS Race")
                      .Anchored(NAXX_ORDER_PATCHWERK)
                      .KillCreatureEngage(NAXX_PATCHWERK, /*count*/ 1, 80.0f)
                      .Timeout(420000)
                      .Build());

    // 9. Grobbulus: prioritize Fallout Slime adds and poison cloud perimeter kiting
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_GROBBULUS, "Grobbulus & Fallout Slimes")
                      .Anchored(NAXX_ORDER_GROBBULUS)
                      .KillCreatureEngage(NAXX_FALLOUT_SLIME, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_GROBBULUS, /*count*/ 1, 80.0f)
                      .Timeout(400000)
                      .Build());

    // 10. Gluth: prioritize Zombie Chow adds so Gluth cannot consume them to heal
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_GLUTH, "Gluth & Zombie Chow")
                      .Anchored(NAXX_ORDER_GLUTH)
                      .KillCreatureEngage(NAXX_ZOMBIE_CHOW, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_GLUTH, /*count*/ 1, 80.0f)
                      .Timeout(400000)
                      .Build());

    // 11. Thaddius: burn Stalagg & Feugen first, safely teleport party across void to Thaddius platform, then engage Thaddius
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_THADDIUS, "Thaddius & Stalagg / Feugen")
                      .Anchored(NAXX_ORDER_THADDIUS)
                      .KillCreatureEngage(NAXX_STALAGG, /*count*/ 0, 80.0f)
                      .KillCreatureEngage(NAXX_FEUGEN, /*count*/ 0, 80.0f)
                      // Relocate bots safely across the air gap from the add platforms to the main Thaddius circle
                      // so bots never fall into the deadly slime moat.
                      .TeleportParty(/*platform lip*/ 3465.0f, -2955.0f, 312.0f,
                                     /*Thaddius platform ring*/ 3505.0f, -2935.0f, 303.0f,
                                     /*checkpoint radius*/ 45.0f)
                      .KillCreatureEngage(NAXX_THADDIUS, /*count*/ 1, 80.0f)
                      .Timeout(600000)
                      .Build());

    // 12. Sapphiron: frost aura coverage, blizzard avoidance, and ice block LoS
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_SAPPHIRON, "Sapphiron Frostwyrm Encounter")
                      .Anchored(NAXX_ORDER_SAPPHIRON)
                      .KillCreatureEngage(NAXX_SAPPHIRON, /*count*/ 1, 90.0f)
                      .Timeout(600000)
                      .Build());

    // 13. Kel'Thuzad: burn Soldiers, Abominations, Weavers, and Guardians
    out.push_back(EventBuilder(NAXX_MAP, NAXX_EVENT_KELTHUZAD, "Kel'Thuzad & Minions")
                      .Anchored(NAXX_ORDER_KELTHUZAD)
                      .KillCreatureEngage(NAXX_SOLDIER_FROZEN, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_UNSTOPPABLE_ABOM, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_SOUL_WEAVER, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_GUARDIAN_ICECROWN, /*count*/ 0, 90.0f)
                      .KillCreatureEngage(NAXX_KELTHUZAD, /*count*/ 1, 90.0f)
                      .Timeout(900000)
                      .Build());
}
