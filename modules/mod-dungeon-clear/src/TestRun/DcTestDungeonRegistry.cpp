/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "DcTestDungeonRegistry.h"

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <sstream>

#ifndef DC_STANDALONE_UNIT_TEST
#include "Map.h"
#include "PlayerbotAIConfig.h"
#include "SQLStorages.h"

#include "Ai/Dungeon/DungeonClear/Settings/DcSettings.h"

#include "TestRun/DcTestGearTiers.h"
#include "TestRun/DcTestRunRecord.h"
#endif

namespace DcTestDungeonRegistry
{
    std::vector<Row> const& All()
    {
        // Entrances = areatrigger_teleport targets (acore_world). Stratholme
        // uses the main (Crusaders' Square) gate, not the service entrance.
        // Dire Maul East uses the courtyard portal by Pusillin's start.
        // Every 3.3.5a 5-man dungeon, curated or not — uncurated maps ride the
        // auto-derived roster alone, and a failing run there is exactly the
        // signal the harness exists to produce. Sorted by recommended level.
        static std::vector<Row> const rows = {
            // --- Classic ---------------------------------------------------
            { "rfc",             "Ragefire Chasm",                389,     3.81f,   -14.82f,  -17.84f, 4.390f, 15, "" },
            // Turtle custom (map 822, gate level 13). Entrance from the
            // areatrigger_teleport row in migration 20260517121832.
            { "frostmane",       "Frostmane Hollow",              822, -7522.73f, -3588.76f,  199.98f, 2.202f, 16, "" },
            { "wc",              "Wailing Caverns",                43,  -163.49f,   132.90f,  -73.66f, 5.830f, 18, "" },
            // Deadmines runs at 20, not the 18 the instance nominally opens at.
            // Its back half is a level-18 party's problem: Gilnid is a level-20
            // elite and his foundry holds 19 level-18 ELITES, so at 18 the party
            // fights same-level elites the whole way in with no level margin at
            // all. See the RoomAggroRegistry row for map 36.
            { "deadmines",       "The Deadmines",                  36,   -16.40f,  -383.07f,   61.78f, 1.860f, 20, "" },
            { "sfk",             "Shadowfang Keep",                33,  -229.13f,  2109.18f,   76.89f, 1.267f, 20, "" },
            // Turtle custom (map 816, gate level 25). Entrance from the same
            // migration as Frostmane Hollow.
            { "dragonmaw",       "Dragonmaw Retreat",             816, -6105.44f, -3629.89f,  242.48f, 3.235f, 30, "" },
            { "stockade",        "The Stockade",                   34,    54.23f,     0.28f,  -18.34f, 6.260f, 24, "" },
            { "bfd",             "Blackfathom Deeps",              48,  -151.89f,   106.96f,  -39.87f, 4.530f, 24, "" },
            // Turtle custom (map 820). Entrance from areatrigger_teleport 5611,
            // "Windhorn Canyon - Entrance". Level 29 is the strongest enemy in
            // the instance: Chieftain Shalk Blackwind, and the Deathtotem trash
            // elites that share his level. The map has 10 mmap tiles, the same
            // coverage as Dragonmaw Retreat, which clears.
            { "windhorn",        "Windhorn Canyon",               820, -8025.66f, -3510.34f,  158.10f, 0.484f, 29, "" },
            { "rfk",             "Razorfen Kraul",                 47,  1943.00f,  1544.63f,   82.00f, 1.380f, 30, "" },
            { "sm-gy",           "Scarlet Monastery: Graveyard",  189,  1688.99f,  1053.48f,   18.68f, 0.001f, 30, "Graveyard" },
            { "gnomer",          "Gnomeregan",                     90,  -332.22f,    -2.28f, -150.86f, 2.770f, 32, "" },
            { "sm-lib",          "Scarlet Monastery: Library",    189,   255.35f,  -209.09f,   18.68f, 6.267f, 35, "Library" },
            { "sm-arm",          "Scarlet Monastery: Armory",     189,  1610.83f,  -323.43f,   18.67f, 6.280f, 38, "Armory" },
            { "sm-cath",         "Scarlet Monastery: Cathedral",  189,   855.68f,  1321.50f,   18.67f, 0.002f, 40, "Cathedral" },
            { "rfd",             "Razorfen Downs",                129,  2592.55f,  1107.50f,   51.29f, 4.740f, 40, "" },
            { "uldaman",         "Uldaman",                        70,  -226.80f,    49.09f,  -46.03f, 1.390f, 44, "" },
            { "zf",              "Zul'Farrak",                    209,  1213.52f,   841.59f,    8.93f, 6.090f, 46, "" },
            { "maraudon",        "Maraudon",                      349,  1019.69f,  -458.31f,  -43.43f, 0.310f, 48, "" },
            { "st",              "The Temple of Atal'Hakkar",     109,  -319.24f,    99.90f, -131.85f, 3.190f, 52, "" },
            { "brd",             "Blackrock Depths",              230,   456.93f,    34.09f,  -68.09f, 4.712f, 54, "" },
            { "brs",             "Blackrock Spire",               229,    78.51f,  -225.04f,   49.84f, 5.100f, 60, "", 0, 10 },
            { "ubrs",            "Upper Blackrock Spire",         229,    78.51f,  -225.04f,   49.84f, 5.100f, 60, "Upper", 0, 10 },
            { "lbrs",            "Lower Blackrock Spire",         229,    78.51f,  -225.04f,   49.84f, 5.100f, 60, "Lower", 0, 5 },
            // --- Turtle WoW custom dungeons (entrances = tw_world areatrigger_teleport,
            //     2026-09-05). Level = strongest boss of the roster in dc_roster.txt (turtle_world
            //     2026-09-05), capped at 60 where the bosses are 61-62. None of these has a compiled boss list - they run on the
            //     roster file alone, so do NOT put one on the ladder before its order
            //     block exists (an empty roster reads as an instant clear).
            { "crescent",        "The Crescent Grove",            802,   579.13f,    90.70f,  276.11f, 3.400f, 38, "" },
            { "stormwrought",    "Stormwrought Ruins",            818, -5632.67f, -4393.33f,  249.98f, 3.182f, 60, "", 0, 20 },
            { "swr",             "Stormwrought Ruins",            818, -5632.67f, -4393.33f,  249.98f, 3.182f, 60, "", 0, 20 },
            { "timbermaw",       "Timbermaw Hold",                819, -8133.95f, -3450.47f,  224.75f, 0.311f, 60, "", 0, 20 },
            { "tmh",             "Timbermaw Hold",                819, -8133.95f, -3450.47f,  224.75f, 0.311f, 60, "", 0, 20 },
            { "gilneas",         "Gilneas City",                  815, -1100.00f,  2237.75f,  182.86f, 2.600f, 49, "" },
            { "hateforge",       "Hateforge Quarry",              808, -8173.90f, -3120.60f,  199.80f, 4.700f, 57, "" },
            { "kara-crypt",      "Karazhan Crypt",                800, -11068.10f, -1806.40f,  52.70f, 1.500f, 60, "" },
            { "crypt",           "Karazhan Crypt",                800, -11068.10f, -1806.40f,  52.70f, 1.500f, 60, "" },
            { "sw-vault",        "Stormwind Vault",                35,    -0.91f,    40.57f,  -24.23f, 1.520f, 60, "" },
            // Turtle's own Black Morass shares map 269 with the TBC dungeon of the same
            // name (row further down, level 70). Distinct token; Find() by map id is
            // ambiguous for 269 on purpose.
            { "black-morass-tw", "The Black Morass (Turtle)",     269, -2002.50f,  6575.30f, -154.90f, 5.700f, 60, "" },
            { "dm-east",         "Dire Maul: East",               429,    44.45f,  -154.82f,   -2.71f, 0.000f, 58, "East" },
            { "dm-west",         "Dire Maul: West",               429,   -62.97f,   159.87f,   -3.46f, 3.148f, 60, "West" },
            { "dm-north",        "Dire Maul: North",              429,   255.25f,   -16.06f,   -2.59f, 4.700f, 60, "North" },
            { "scholo",          "Scholomance",                   289,   196.37f,   127.05f,  134.91f, 6.090f, 60, "" },
            { "strat",           "Stratholme",                    329,  3593.15f, -3646.56f,  138.50f, 5.330f, 60, "" },
            // --- Classic Raids --------------------------------------------
            { "onyxia",          "Onyxia's Lair",                  249,    29.16f,   -71.34f,    -8.18f, 4.440f, 60, "", 0, 40 },
            { "zg",              "Zul'Gurub",                      309, -11916.90f, -1248.36f,   92.53f, 4.720f, 60, "", 0, 20 },
            { "mc",              "Molten Core",                    409,  1082.04f,  -474.60f,  -107.76f, 5.030f, 60, "", 0, 40 },
            { "bwl",             "Blackwing Lair",                 469, -7664.76f, -1100.87f,   399.68f, 0.560f, 60, "", 0, 40 },
            { "aq20",            "Ruins of Ahn'Qiraj",             509, -8437.74f,  1516.91f,    31.91f, 2.730f, 60, "", 0, 20 },
            { "aq40",            "Temple of Ahn'Qiraj",            531, -8213.48f,  2016.00f,   129.07f, 1.280f, 60, "", 0, 40 },
            { "naxx",            "Naxxramas",                      533,  3021.64f, -3402.99f,   298.22f, 2.970f, 60, "", 0, 40 },
            // Turtle custom raid: Scarlet Citadel (map 45, 40-man fortress).
            // Entrance from areatrigger_teleport target / .tele ScarletCitadel
            { "scarlet-citadel", "Scarlet Citadel",                 45,    32.55f,    13.30f,    16.87f, 6.280f, 63, "", 0, 40 },
            // Turtle custom raid: Emerald Sanctum (map 807, 40-man raid).
            // Entrance from .tele sanctumraid / areatrigger 5017
            { "sanctum",         "Emerald Sanctum",                807,  2765.20f,  2976.20f,    26.65f, 3.620f, 60, "", 0, 40 },
            { "emerald-sanctum", "Emerald Sanctum",                807,  2765.20f,  2976.20f,    26.65f, 3.620f, 60, "", 0, 40 },
            // Turtle custom raid: Lower Karazhan Halls (map 532, 10-man raid).
            // Entrance from .tele kara / areatrigger 5018
            { "lkh",             "Lower Karazhan Halls",           532, -11099.80f, -1996.33f,   49.85f, 0.600f, 60, "", 0, 10 },
            { "kara10",          "Lower Karazhan Halls",           532, -11099.80f, -1996.33f,   49.85f, 0.600f, 60, "", 0, 10 },
            // Turtle custom raid: Upper Karazhan (map 814, 40-man raid).
            // Entrance from .tele karaforty
            { "kara-upper",      "Karazhan (Upper)",               814, -11048.20f, -1985.84f,  101.57f, 2.200f, 60, "", 0, 40 },
            { "kara40",          "Karazhan (Upper)",               814, -11048.20f, -1985.84f,  101.57f, 2.200f, 60, "", 0, 40 },
            // --- The Burning Crusade --------------------------------------
            { "ramparts",        "Hellfire Ramparts",             543, -1355.24f,  1641.12f,   68.25f, 0.669f, 62, "", 70 },
            { "blood-furnace",   "The Blood Furnace",             542,    -4.00f,    14.64f,  -44.80f, 4.887f, 63, "", 70 },
            { "slave-pens",      "The Slave Pens",                547,   120.10f,  -131.96f,   -0.80f, 1.476f, 64, "", 70 },
            { "underbog",        "The Underbog",                  546,     9.71f,   -16.20f,   -2.75f, 5.571f, 64, "", 70 },
            { "mana-tombs",      "Mana-Tombs",                    557,     0.02f,     0.95f,   -0.95f, 3.032f, 66, "", 70 },
            { "auchenai",        "Auchenai Crypts",               558,   -21.90f,     0.16f,   -0.12f, 0.035f, 67, "", 70 },
            { "sethekk",         "Sethekk Halls",                 556,    -4.68f,    -0.09f,    0.01f, 0.035f, 68, "", 70 },
            { "old-hillsbrad",   "Old Hillsbrad Foothills",       560,  2741.87f,  1315.25f,   14.04f, 2.960f, 68, "", 70 },
            { "shadow-labs",     "Shadow Labyrinth",              555,     0.49f,    -0.22f,   -1.13f, 3.159f, 70, "", 70 },
            { "steamvault",      "The Steamvault",                545,   -13.84f,     6.75f,   -4.26f, 0.000f, 70, "", 70 },
            { "shattered-halls", "The Shattered Halls",           540,   -40.87f,   -19.75f,  -13.81f, 1.111f, 70, "", 70 },
            { "black-morass",    "The Black Morass",              269, -1496.24f,  7034.70f,   32.56f, 1.757f, 70, "", 70 },
            { "botanica",        "The Botanica",                  553,    40.04f,   -28.61f,   -1.12f, 2.359f, 70, "", 70 },
            { "mechanar",        "The Mechanar",                  554,   -28.91f,     0.68f,   -1.81f, 0.035f, 70, "", 70 },
            { "arcatraz",        "The Arcatraz",                  552,    -1.23f,     0.01f,   -0.20f, 0.016f, 70, "", 70 },
            { "mgt",             "Magisters' Terrace",            585,     7.09f,    -0.45f,   -2.80f, 0.050f, 70, "", 70 },
            // --- Wrath of the Lich King -----------------------------------
            { "uk",              "Utgarde Keep",                  574,   153.79f,   -86.55f,   12.55f, 0.304f, 70, "", 80 },
            { "nexus",           "The Nexus",                     576,   145.87f,   -10.55f,  -16.64f, 1.528f, 71, "", 80 },
            { "an",              "Azjol-Nerub",                   601,   413.31f,   795.97f,  831.35f, 5.500f, 74, "", 80 },
            { "ok",              "Ahn'kahet: The Old Kingdom",    619,   333.35f, -1109.94f,   69.77f, 0.553f, 74, "", 80 },
            { "dtk",             "Drak'Tharon Keep",              600,  -517.34f,  -487.98f,   11.01f, 4.831f, 75, "", 80 },
            { "vh",              "The Violet Hold",               608,  1808.82f,   803.93f,   44.36f, 6.282f, 75, "", 80 },
            { "gundrak",         "Gundrak",                       604,  1891.84f,   832.17f,  176.67f, 2.109f, 77, "", 80 },
            { "hos",             "Halls of Stone",                599,  1153.24f,   806.16f,  195.94f, 4.715f, 78, "", 80 },
            { "hol",             "Halls of Lightning",            602,  1331.47f,   259.62f,   53.40f, 4.772f, 79, "", 80 },
            { "cos",             "The Culling of Stratholme",     595,  1431.10f,   556.92f,   36.69f, 5.160f, 79, "", 80 },
            { "oculus",          "The Oculus",                    578,  1055.93f,   986.85f,  361.07f, 5.745f, 79, "", 80 },
            { "up",              "Utgarde Pinnacle",              575,   584.12f,  -327.97f,  110.14f, 3.122f, 79, "", 80 },
            { "toc",             "Trial of the Champion",         650,   805.23f,   618.04f,  412.39f, 3.146f, 80, "", 80 },
            { "fos",             "The Forge of Souls",            632,  4922.86f,  2175.63f,  638.73f, 2.004f, 80, "", 80 },
            { "pos",             "Pit of Saron",                  658,   435.74f,   212.41f,  528.71f, 6.256f, 80, "", 80 },
            { "hor",             "Halls of Reflection",           668,  5239.01f,  1932.64f,  707.70f, 0.801f, 80, "", 80 },
        };
        return rows;
    }

    Row const* Find(std::string const& tokenOrMapId)
    {
        if (tokenOrMapId.empty())
            return nullptr;

        for (Row const& row : All())
            if (tokenOrMapId == row.token)
                return &row;

        // Numeric fallback — only unambiguous for maps with a single row or identical wings.
        char* end = nullptr;
        unsigned long const asMap = std::strtoul(tokenOrMapId.c_str(), &end, 10);
        if (!end || *end != '\0' || asMap == 0)
            return nullptr;

        Row const* hit = nullptr;
        for (Row const& row : All())
            if (row.mapId == asMap)
            {
                if (hit)
                {
                    if (std::strcmp(hit->wing, row.wing) != 0)
                        return nullptr;  // wing-split map: demand a wing token
                }
                hit = &row;
            }
        return hit;
    }

    std::uint32_t PlayerLimit(std::uint32_t mapId)
    {
#ifndef DC_STANDALONE_UNIT_TEST
        // The same read the core makes, from the same sMapStorage over
        // map_template — not Map.dbc, and not a query of our own. MapEntry is
        // packed with maxPlayers where the core reads it, so there is nothing to
        // reimplement here: ask it, or ask nobody.
        MapEntry const* entry = sMapStorage.LookupEntry<MapEntry>(mapId);
        return entry ? entry->maxPlayers : 0;
#else
        (void)mapId;
        return 0;
#endif
    }

    void WriteSidecar()
    {
#ifndef DC_STANDALONE_UNIT_TEST
        char const* path = "dc_test_dungeons.json";
        if (char const* env = std::getenv("DC_TEST_DUNGEONS_FILE"))
            if (env[0])
                path = env;

        using DcTestRunRecord::EscapeJson;

        // The ilvl ladder the dashboard's start form offers, per row and per
        // difficulty (heroic runs at a different level, so a TBC row's two
        // ladders differ). Emitted here rather than computed in the dashboard so
        // the two can't drift: this file IS the catalogue.
        auto appendLadder = [](std::ostringstream& out, std::uint32_t mapId, std::uint32_t level)
        {
            out << '[';
            bool firstChoice = true;
            for (DcTestGearTiers::Choice const& choice : DcTestGearTiers::Ladder(mapId, level))
            {
                if (!firstChoice)
                    out << ',';
                firstChoice = false;
                out << "{\"ilvl\":" << choice.ilvl << ",\"label\":\"" << EscapeJson(choice.label)
                    << "\"}";
            }
            out << ']';
        };

        std::ostringstream s;
        s << "{\"limits\":{\"maxConcurrent\":"
          << DcSettings::GetUInt(ObjectGuid::Empty, "TestRun.MaxConcurrent")
          << ",\"maxPlans\":"
          << DcSettings::GetUInt(ObjectGuid::Empty, "TestRun.MaxPlans")
          << ",\"planMaxTotal\":"
          << DcSettings::GetUInt(ObjectGuid::Empty, "TestRun.Plan.MaxTotal")
          << "}";

        // What a run gets when it asks for nothing. Read once at startup like
        // the rest of this file, so it is a label for the form's "server
        // default" entry, not an authority — the worldserver resolves the real
        // values when the run starts.
        s << ",\"gearDefaults\":{\"ilvl\":"
          << (sPlayerbotAIConfig.autoGearScoreLimit > 0 ? sPlayerbotAIConfig.autoGearScoreLimit : 0)
          << ",\"quality\":"
          << (sPlayerbotAIConfig.autoGearQualityLimit > 0 ? sPlayerbotAIConfig.autoGearQualityLimit
                                                          : 3)
          << "},\"qualities\":[";
        for (std::uint32_t q = 1; q <= 5; ++q)
            s << (q > 1 ? "," : "") << "{\"v\":" << q << ",\"label\":\""
              << DcTestGearTiers::QualityName(q) << "\"}";
        s << "],\"dungeons\":[";
        bool first = true;
        for (Row const& row : All())
        {
            if (!first)
                s << ',';
            first = false;
            s << "{\"token\":\"" << EscapeJson(row.token) << '"'
              << ",\"name\":\"" << EscapeJson(row.name) << '"'
              << ",\"mapId\":" << row.mapId
              // Additive: the form can label the party it is about to ask for,
              // and this is the only place outside the C++ row that knows it. The
              // map's own ceiling rides along so the form cannot offer a size the
              // core would refuse at the door (0 = the world has no map_template
              // row for this map — see PlayerLimit).
              << ",\"size\":" << row.PartySize()
              << ",\"playerLimit\":" << PlayerLimit(row.mapId)
              << ",\"level\":" << row.recommendedLevel
              << ",\"heroicLevel\":" << row.heroicLevel
              << ",\"wing\":\"" << EscapeJson(row.wing) << '"'
              << ",\"gear\":";
            appendLadder(s, row.mapId, row.recommendedLevel);
            if (row.heroicLevel)
            {
                s << ",\"gearHeroic\":";
                appendLadder(s, row.mapId, row.heroicLevel);
            }
            s << '}';
        }
        s << "]}";

        std::ofstream f(path, std::ios::out | std::ios::trunc);
        if (!f.is_open())
            return;
        f << s.str() << '\n';
#endif
    }
}
