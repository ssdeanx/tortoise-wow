# Automated Raid Test Runs (.dc test) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Enable automated headless raid test runs in `mod-dungeon-clear` (`.dc test start <raid>`) with scaled role compositions for 20-man and 40-man classic raids.

**Architecture:** Extend `DcTestComp::BuildComp` to dynamically calculate tank, healer, and DPS quotas based on party size. Register all classic 20-man and 40-man raids in `DcTestDungeonRegistry` with verified database entrance coordinates. Append encounter orders in `dc_roster.txt` so `mod-dungeon-clear` can navigate and track boss progression in raids.

**Tech Stack:** C++17, CMaNGOS Zero/Turtle WoW Core, GoogleTest, `mod-dungeon-clear`, `mod-playerbots`.

## Global Constraints
- Preserve core native systems, generic behaviors, and Turtle-WoW compatibility per `AGENTS.md`.
- Stay strictly within `/home/sam/tortoise-wow/`.
- Ensure 100% backward compatibility for all existing 5-man dungeon runs.
- All code must build cleanly with `cmake --build build -j$(nproc)`.

---

### Task 1: Dynamically Scale Role Composition in `DcTestComp`

**Files:**
- Modify: `modules/mod-dungeon-clear/src/TestRun/DcTestComp.h`
- Modify: `modules/mod-dungeon-clear/src/TestRun/DcTestComp.cpp`
- Test: `modules/mod-dungeon-clear/t/TestTestComp.cpp`

**Interfaces:**
- Consumes: `kTankPool`, `kHealPool`, `kDpsPool`, `Draw`
- Produces: `BuildComp(std::uint32_t seed, std::size_t size = kPartySize)` with scaled tank/healer/DPS counts.

- [ ] **Step 1: Write failing unit tests for 20-man and 40-man compositions**

Add tests to `modules/mod-dungeon-clear/t/TestTestComp.cpp`:

```cpp
TEST(DcTestComp, Raid20CompositionScaling)
{
    for (std::uint32_t seed = 0; seed < 50; ++seed)
    {
        auto comp = BuildComp(seed, 20);
        ASSERT_EQ(comp.size(), 20u);

        std::size_t tanks = 0, heals = 0, dps = 0;
        for (auto const& s : comp)
        {
            if (std::string_view(s.role) == "tank") ++tanks;
            else if (std::string_view(s.role) == "heal") ++heals;
            else if (std::string_view(s.role) == "dps") ++dps;
        }
        EXPECT_EQ(tanks, 2u) << "seed=" << seed;
        EXPECT_EQ(heals, 4u) << "seed=" << seed;
        EXPECT_EQ(dps, 14u) << "seed=" << seed;
    }
}

TEST(DcTestComp, Raid40CompositionScaling)
{
    for (std::uint32_t seed = 0; seed < 50; ++seed)
    {
        auto comp = BuildComp(seed, 40);
        ASSERT_EQ(comp.size(), 40u);

        std::size_t tanks = 0, heals = 0, dps = 0;
        for (auto const& s : comp)
        {
            if (std::string_view(s.role) == "tank") ++tanks;
            else if (std::string_view(s.role) == "heal") ++heals;
            else if (std::string_view(s.role) == "dps") ++dps;
        }
        EXPECT_EQ(tanks, 3u) << "seed=" << seed;
        EXPECT_EQ(heals, 8u) << "seed=" << seed;
        EXPECT_EQ(dps, 29u) << "seed=" << seed;
    }
}
```

- [ ] **Step 2: Run test to verify it fails**

Run: `cmake --build build --target test_dungeon_clear -j$(nproc) && ./build/modules/mod-dungeon-clear/test_dungeon_clear --gtest_filter=DcTestComp.Raid*`
Expected: FAIL (tanks == 1, heals == 1, dps == 18/38)

- [ ] **Step 3: Implement scaled composition in `DcTestComp.cpp`**

Update `DcTestComp::BuildComp` in `modules/mod-dungeon-clear/src/TestRun/DcTestComp.cpp`:

```cpp
    std::vector<Slot> BuildComp(std::uint32_t seed, std::size_t size)
    {
        std::vector<Slot> comp;
        if (size < 2)
            return comp;  // a run needs at least the tank + healer pair

        std::size_t numTanks = 1;
        std::size_t numHealers = 1;

        if (size >= 26)
        {
            numTanks = 3;
            numHealers = 8;
        }
        else if (size >= 16)
        {
            numTanks = 2;
            numHealers = 4;
        }
        else if (size >= 6)
        {
            numTanks = 2;
            numHealers = 2;
        }

        // Defensive cap so tanks + healers never exceed total slots
        if (numTanks + numHealers >= size)
        {
            numTanks = 1;
            numHealers = 1;
        }

        std::uint32_t state = seed;
        std::set<std::uint8_t> used;
        comp.resize(size);

        std::size_t slotIdx = 0;
        for (std::size_t i = 0; i < numTanks; ++i)
        {
            comp[slotIdx] = Draw(kTankPool, std::size(kTankPool), used, state);
            used.insert(comp[slotIdx].classId);
            ++slotIdx;
        }

        for (std::size_t i = 0; i < numHealers; ++i)
        {
            comp[slotIdx] = Draw(kHealPool, std::size(kHealPool), used, state);
            used.insert(comp[slotIdx].classId);
            ++slotIdx;
        }

        while (slotIdx < size)
        {
            comp[slotIdx] = Draw(kDpsPool, std::size(kDpsPool), used, state);
            used.insert(comp[slotIdx].classId);
            ++slotIdx;
        }

        return comp;
    }
```

- [ ] **Step 4: Run test to verify it passes**

Run: `cmake --build build --target test_dungeon_clear -j$(nproc) && ./build/modules/mod-dungeon-clear/test_dungeon_clear --gtest_filter=DcTestComp.*`
Expected: PASS (all tests passing including `DeterministicForSameSeed`, `RoleLayoutIsStable`, `Raid20CompositionScaling`, `Raid40CompositionScaling`)

- [ ] **Step 5: Commit**

```bash
git add modules/mod-dungeon-clear/src/TestRun/DcTestComp.cpp modules/mod-dungeon-clear/t/TestTestComp.cpp
git commit -m "feat(mod-dungeon-clear): scale raid composition by group size in DcTestComp"
```

---

### Task 2: Align Role Introspection in `DcTestRoster`

**Files:**
- Modify: `modules/mod-dungeon-clear/src/TestRun/DcTestRoster.h`
- Modify: `modules/mod-dungeon-clear/src/TestRun/DcTestRoster.cpp`
- Test: `modules/mod-dungeon-clear/t/TestTestRoster.cpp`

**Interfaces:**
- Consumes: `totalSize`
- Produces: `RoleForIndex(std::size_t i, std::size_t totalSize = 5)`

- [ ] **Step 1: Write unit tests for scaled `RoleForIndex`**

Add tests to `modules/mod-dungeon-clear/t/TestTestRoster.cpp`:

```cpp
TEST(DcTestRoster, Raid40RolesScaledCorrectly)
{
    // 40-man: 3 tanks (0..2), 8 healers (3..10), 29 dps (11..39)
    EXPECT_STREQ("tank", RoleForIndex(0, 40));
    EXPECT_STREQ("tank", RoleForIndex(2, 40));
    EXPECT_STREQ("heal", RoleForIndex(3, 40));
    EXPECT_STREQ("heal", RoleForIndex(10, 40));
    EXPECT_STREQ("dps", RoleForIndex(11, 40));
    EXPECT_STREQ("dps", RoleForIndex(39, 40));
}

TEST(DcTestRoster, Raid20RolesScaledCorrectly)
{
    // 20-man: 2 tanks (0..1), 4 healers (2..5), 14 dps (6..19)
    EXPECT_STREQ("tank", RoleForIndex(0, 20));
    EXPECT_STREQ("tank", RoleForIndex(1, 20));
    EXPECT_STREQ("heal", RoleForIndex(2, 20));
    EXPECT_STREQ("heal", RoleForIndex(5, 20));
    EXPECT_STREQ("dps", RoleForIndex(6, 20));
    EXPECT_STREQ("dps", RoleForIndex(19, 20));
}
```

- [ ] **Step 2: Run test to verify it fails**

Run: `cmake --build build --target test_dungeon_clear -j$(nproc) && ./build/modules/mod-dungeon-clear/test_dungeon_clear --gtest_filter=DcTestRoster.Raid*`
Expected: FAIL (RoleForIndex(2, 40) is "dps")

- [ ] **Step 3: Implement scaled `RoleForIndex` in `DcTestRoster.h` and `DcTestRoster.cpp`**

Update `DcTestRoster.h`:
```cpp
char const* RoleForIndex(std::size_t i, std::size_t totalSize = 5);
```

Update `DcTestRoster.cpp`:
```cpp
char const* RoleForIndex(std::size_t i, std::size_t totalSize)
{
    std::size_t numTanks = 1;
    std::size_t numHealers = 1;

    if (totalSize >= 26)
    {
        numTanks = 3;
        numHealers = 8;
    }
    else if (totalSize >= 16)
    {
        numTanks = 2;
        numHealers = 4;
    }
    else if (totalSize >= 6)
    {
        numTanks = 2;
        numHealers = 2;
    }

    if (i < numTanks)
        return "tank";
    if (i < numTanks + numHealers)
        return "heal";
    return "dps";
}
```

And update `Parse` to pass `size` into `RoleForIndex(i, size)`:
```cpp
members.push_back({names[i], RoleForIndex(i, size)});
```

- [ ] **Step 4: Run test to verify it passes**

Run: `cmake --build build --target test_dungeon_clear -j$(nproc) && ./build/modules/mod-dungeon-clear/test_dungeon_clear --gtest_filter=DcTestRoster.*`
Expected: PASS

- [ ] **Step 5: Commit**

```bash
git add modules/mod-dungeon-clear/src/TestRun/DcTestRoster.h modules/mod-dungeon-clear/src/TestRun/DcTestRoster.cpp modules/mod-dungeon-clear/t/TestTestRoster.cpp
git commit -m "feat(mod-dungeon-clear): support scaled roles by totalSize in DcTestRoster"
```

---

### Task 3: Register Classic Raids in `DcTestDungeonRegistry`

**Files:**
- Modify: `modules/mod-dungeon-clear/src/TestRun/DcTestDungeonRegistry.cpp`
- Test: `modules/mod-dungeon-clear/t/TestTestDungeonRegistry.cpp`

**Interfaces:**
- Produces: Entries in `DcTestDungeonRegistry::All()` for `mc`, `bwl`, `onyxia`, `zg`, `aq20`, `aq40`, `naxx`, `scarlet-citadel`.

- [ ] **Step 1: Write unit tests for raid token registrations**

Add tests to `modules/mod-dungeon-clear/t/TestTestDungeonRegistry.cpp`:

```cpp
TEST(DcTestDungeonRegistryTest, RaidTokensAreRegisteredWithCorrectPartySize)
{
    struct ExpectedRaid
    {
        char const* token;
        uint32 mapId;
        uint32 partySize;
    };

    ExpectedRaid const raids[] = {
        { "mc", 409, 40 },
        { "bwl", 469, 40 },
        { "onyxia", 249, 40 },
        { "zg", 309, 20 },
        { "aq20", 509, 20 },
        { "aq40", 531, 40 },
        { "naxx", 533, 40 },
        { "scarlet-citadel", 45, 40 },
    };

    for (auto const& expected : raids)
    {
        Row const* row = Find(expected.token);
        ASSERT_NE(row, nullptr) << "missing raid token: " << expected.token;
        EXPECT_EQ(row->mapId, expected.mapId) << expected.token;
        EXPECT_EQ(row->PartySize(), expected.partySize) << expected.token;
        EXPECT_EQ(row->recommendedLevel, expected.mapId == 45 ? 63u : 60u) << expected.token;
        EXPECT_EQ(row->heroicLevel, 0u) << expected.token;
    }
}
```

- [ ] **Step 2: Run test to verify it fails**

Run: `cmake --build build --target test_dungeon_clear -j$(nproc) && ./build/modules/mod-dungeon-clear/test_dungeon_clear --gtest_filter=DcTestDungeonRegistryTest.Raid*`
Expected: FAIL (missing raid token "mc")

- [ ] **Step 3: Add raid entries in `DcTestDungeonRegistry.cpp`**

In `modules/mod-dungeon-clear/src/TestRun/DcTestDungeonRegistry.cpp`, update lines around 90-100 to register the classic raids with verified teleport coordinates and party sizes:

```cpp
            // --- Classic Raids --------------------------------------------
            { "onyxia",          "Onyxia's Lair",                  249,    29.16f,   -71.34f,    -8.18f, 4.440f, 60, "", 0, 40 },
            { "zg",              "Zul'Gurub",                      309, -11916.90f, -1248.36f,   92.53f, 4.720f, 60, "", 0, 20 },
            { "mc",              "Molten Core",                    409,  1082.04f,  -474.60f,  -107.76f, 5.030f, 60, "", 0, 40 },
            { "bwl",             "Blackwing Lair",                 469, -7664.76f, -1100.87f,   399.68f, 0.560f, 60, "", 0, 40 },
            { "aq20",            "Ruins of Ahn'Qiraj",             509, -8437.74f,  1516.91f,    31.91f, 2.730f, 60, "", 0, 20 },
            { "aq40",            "Temple of Ahn'Qiraj",            531, -8213.48f,  2016.00f,   129.07f, 1.280f, 60, "", 0, 40 },
            { "naxx",            "Naxxramas",                      533,  3021.64f, -3402.99f,   298.22f, 2.970f, 60, "", 0, 40 },
            // Turtle custom raid: Scarlet Citadel (map 45, 40-man fortress).
            { "scarlet-citadel", "Scarlet Citadel",                 45,    32.55f,    13.30f,    16.87f, 6.280f, 63, "", 0, 40 },
```

- [ ] **Step 4: Run test to verify it passes**

Run: `cmake --build build --target test_dungeon_clear -j$(nproc) && ./build/modules/mod-dungeon-clear/test_dungeon_clear --gtest_filter=DcTestDungeonRegistryTest.*`
Expected: PASS

- [ ] **Step 5: Commit**

```bash
git add modules/mod-dungeon-clear/src/TestRun/DcTestDungeonRegistry.cpp modules/mod-dungeon-clear/t/TestTestDungeonRegistry.cpp
git commit -m "feat(mod-dungeon-clear): register classic raid instances in DcTestDungeonRegistry"
```

---

### Task 4: Populate Boss Sequences in `dc_roster.txt`

**Files:**
- Modify: `modules/mod-dungeon-clear/data/dc_roster.txt`

**Interfaces:**
- Consumes: Map IDs and creature entries for MC, Onyxia, ZG, AQ20, AQ40.
- Produces: `order <mapId> <entry> <order>` directives parsed by `DcRosterFile`.

- [ ] **Step 1: Append ordered boss entries in `modules/mod-dungeon-clear/data/dc_roster.txt`**

Append to `modules/mod-dungeon-clear/data/dc_roster.txt`:

```text
# --- Onyxia's Lair (map 249) -------------------------------------------------
order 249 10184 1     # Onyxia

# --- Molten Core (map 409) ---------------------------------------------------
order 409 12118 1     # Lucifron
order 409 11982 2     # Magmadar
order 409 12259 3     # Gehennas
order 409 12057 4     # Garr
order 409 12264 5     # Shazzrah
order 409 12056 6     # Baron Geddon
order 409 11988 7     # Golemagg the Incinerator
order 409 12098 8     # Sulfuron Harbinger
order 409 12018 9     # Majordomo Executus
order 409 11502 10    # Ragnaros (end boss)

# --- Zul'Gurub (map 309) -----------------------------------------------------
order 309 14517 1     # High Priestess Jeklik
order 309 14515 2     # High Priest Venoxis
order 309 14510 3     # High Priestess Mar'li
order 309 11382 4     # Bloodlord Mandokir
order 309 14509 5     # High Priest Thekal
order 309 15114 6     # Gahz'ranka
order 309 14511 7     # High Priestess Arlokk
order 309 11380 8     # Jin'do the Hexxer
order 309 14834 9     # Hakkar (end boss)

# --- Ruins of Ahn'Qiraj - AQ20 (map 509) -------------------------------------
order 509 15348 1     # Kurinnaxx
order 509 15341 2     # General Rajaxx
order 509 15340 3     # Moam
order 509 15370 4     # Buru the Gorger
order 509 15369 5     # Ayamiss the Hunter
order 509 15339 6     # Ossirian the Unscarred (end boss)

# --- Temple of Ahn'Qiraj - AQ40 (map 531) ------------------------------------
order 531 15263 1     # The Prophet Skeram
order 531 15543 2     # Princess Yauj (Bug Trio)
order 531 15516 3     # Battleguard Sartura
order 531 15510 4     # Fankriss the Unyielding
order 531 15299 5     # Viscidus
order 531 15509 6     # Princess Huhuran
order 531 15275 7     # Emperor Vek'lor (Twin Emperors)
order 531 15517 8     # Ouro
order 531 15727 9     # C'Thun (end boss)
```

- [ ] **Step 2: Verify `dc_roster.txt` parsing with unit tests**

Run: `cmake --build build --target test_dungeon_clear -j$(nproc) && ./build/modules/mod-dungeon-clear/test_dungeon_clear --gtest_filter=*Roster*`
Expected: PASS

- [ ] **Step 3: Commit**

```bash
git add modules/mod-dungeon-clear/data/dc_roster.txt
git commit -m "feat(mod-dungeon-clear): add boss encounter orders for classic raids in dc_roster.txt"
```

---

### Task 5: Full Build & End-to-End Validation

**Files:**
- Test target: `mangosd` server binary & unit tests

- [ ] **Step 1: Run full unit test suite**

Run: `./build/modules/mod-dungeon-clear/test_dungeon_clear`
Expected: PASS (all tests pass)

- [ ] **Step 2: Full core compilation**

Run: `cmake --build build -j$(nproc)`
Expected: Built target `mangosd` with exit code 0.

- [ ] **Step 3: Verify `.dc test list` binary output / sidecar generation**

Run: `grep -E '"mc"|"bwl"|"onyxia"|"zg"|"aq20"|"aq40"|"naxx"' modules/mod-dungeon-clear/src/TestRun/DcTestDungeonRegistry.cpp`
Expected: Match all raid tokens with proper party sizes.

- [ ] **Step 4: Final commit & status report**

```bash
git status
```
