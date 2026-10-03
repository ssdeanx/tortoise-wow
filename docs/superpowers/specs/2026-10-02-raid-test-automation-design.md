# Design Specification: Automated Raid Test Runs (.dc test)

- **Date**: 2026-10-02
- **Topic**: Raid Test Automation in `mod-dungeon-clear`
- **Scope**: C++ Core module `mod-dungeon-clear` (`TestRun` subsystem) and data overlays (`dc_roster.txt`)

---

## 1. Problem Statement & Motivation

The `.dc test` command suite in `mod-dungeon-clear` allows headless execution and telemetry verification of dungeon clears using bot parties without requiring human players in the group.

While the low-level infrastructure supports raid conversion (`group->ConvertToRaid()` in `DcTestRunJob.cpp:1302`) and gear-score-based raid leader election (`DcLeaderSignal::FindLeaderTank`), running raids via `.dc test start <dungeon>` currently fails due to three constraints:
1. **Missing Raid Entries**: Only Zul'Gurub and Scarlet Citadel are present in `DcTestDungeonRegistry.cpp`, and both leave `partySize` at 0 (falling back to a 5-bot party). Classic 40-man raids (`mc`, `bwl`, `onyxia`, `aq40`, `naxx`) and 20-man raids (`aq20`) are missing entirely.
2. **Fixed 1 Tank / 1 Healer Composition**: `DcTestComp::BuildComp` hardcodes slot 0 to Tank, slot 1 to Healer, and all remaining slots to DPS, yielding 1 tank, 1 healer, and 38 DPS for a 40-man raid.
3. **Chat Frame Buffer Constraint**: Hand-picking a 40-bot roster via `party=...` exceeds the 255-character chat box limit in WoW 1.12.

---

## 2. Architecture & Components

```
                   [.dc test start <raid_token>]
                               │
                               ▼
                   [DcTestDungeonRegistry::Find]
                 (Validates mapId, capacity, partySize)
                               │
                               ▼
                   [DcTestComp::BuildComp]
     (Scales role quotas: 5-man vs 10-man vs 20-man vs 40-man)
                               │
                               ▼
                    [DcTestRunJob::Create]
    (Claims offline randombots, provisions specs, assigns roles)
                               │
                               ▼
                   [DcTestRunJob::TickGrouping]
              (Exceeds MAX_GROUP_SIZE -> ConvertToRaid())
                               │
                               ▼
                  [DcTestRunJob::Teleporting]
          (Teleports raid to instance entrance coordinates)
                               │
                               ▼
                  [DcLeaderSignal::FindLeaderTank]
        (Elects highest gear score Tank bot as raid driver)
```

---

## 3. Detailed Specifications

### 3.1 Dynamic Role Scaling (`DcTestComp.cpp` & `DcTestComp.h`)

Update `DcTestComp::BuildComp(std::uint32_t seed, std::size_t size)` to determine role quotas dynamically based on team size:

| Group Size Bracket | Tanks | Healers | DPS | Target Scenarios |
| :--- | :---: | :---: | :---: | :--- |
| **5-man** ($N \le 5$) | 1 | 1 | $N - 2$ | Stock 5-man dungeons (100% backward compatible) |
| **10-man** ($6 \le N \le 15$) | 2 | 2 | $N - 4$ | Scarlet Citadel, UBRS |
| **20-man** ($16 \le N \le 25$) | 2 | 4 | $N - 6$ | Zul'Gurub, Ruins of Ahn'Qiraj (AQ20) |
| **40-man** ($N \ge 26$) | 3 | 8 | $N - 11$ | Molten Core, Blackwing Lair, Onyxia, AQ40, Naxxramas |

- **Tank Allocation**: Filled sequentially from `kTankPool` (Warrior Protection, Paladin Protection).
- **Healer Allocation**: Filled sequentially from `kHealPool` (Priest Holy, Paladin Holy, Shaman Restoration, Druid Restoration).
- **DPS Allocation**: Remaining slots filled from `kDpsPool`.
- The distinct-class preference is preserved for initial slots of each role before recycling the pool.

### 3.2 Role Introspection (`DcTestRoster.cpp`)

Update `DcTestRoster::RoleForIndex(std::size_t i)` to accept or deduce group size so status lines and error reporting reflect the actual role distribution for raids rather than defaulting to `dps` for any index $\ge 2$.

### 3.3 Dungeon Catalogue Expansion (`DcTestDungeonRegistry.cpp`)

Add entries to `DcTestDungeonRegistry::All()` with coordinates verified from `tw_world.game_tele`:

```cpp
// --- Classic Raids ---------------------------------------------
{ "onyxia",          "Onyxia's Lair",                  249,    29.16f,   -71.34f,    -8.18f, 4.440f, 60, "", 0, 40 },
{ "zg",              "Zul'Gurub",                      309, -11916.90f, -1248.36f,   92.53f, 4.720f, 60, "", 0, 20 },
{ "mc",              "Molten Core",                    409,  1082.04f,  -474.60f,  -107.76f, 5.030f, 60, "", 0, 40 },
{ "bwl",             "Blackwing Lair",                 469, -7664.76f, -1100.87f,   399.68f, 0.560f, 60, "", 0, 40 },
{ "aq20",            "Ruins of Ahn'Qiraj",             509, -8437.74f,  1516.91f,    31.91f, 2.730f, 60, "", 0, 20 },
{ "aq40",            "Temple of Ahn'Qiraj",            531, -8213.48f,  2016.00f,   129.07f, 1.280f, 60, "", 0, 40 },
{ "naxx",            "Naxxramas",                      533,  3021.64f, -3402.99f,   298.22f, 2.970f, 60, "", 0, 40 },
{ "scarlet-citadel", "Scarlet Citadel",                 45,    32.55f,    13.30f,    16.87f, 6.280f, 63, "", 0, 40 },
```

### 3.4 Encounter Order Directives (`modules/mod-dungeon-clear/data/dc_roster.txt`)

Append ordered boss entries for the newly active raids:
- **Onyxia (Map 249)**: Onyxia (10184).
- **Molten Core (Map 409)**: Lucifron (12118) -> Magmadar (11982) -> Gehennas (12259) -> Garr (12057) -> Shazzrah (12264) -> Baron Geddon (12056) -> Golemagg (11988) -> Sulfuron (12098) -> Majordomo (12018) -> Ragnaros (11502).
- **Zul'Gurub (Map 309)**: Jeklik (14517) -> Venoxis (14515) -> Mar'li (14510) -> Mandokir (11382) -> Thekal (14509) -> Gahz'ranka (15114) -> Arlokk (14511) -> Jin'do (11380) -> Hakkar (14834).
- **AQ20 (Map 509)**: Kurinnaxx (15348) -> Rajaxx (15341) -> Moam (15340) -> Buru (15370) -> Ayamiss (15369) -> Ossirian (15339).
- **AQ40 (Map 531)**: Skeram (15263) -> Bug Trio (15543) -> Sartura (15516) -> Fankriss (15510) -> Huhuran (15509) -> Twin Emperors (15275) -> Ouro (15517) -> C'Thun (15727).

---

## 4. Verification & Testing

1. **Compilation**: Execute `cmake --build build -j$(nproc)`; verify 0 compiler errors or warnings.
2. **Registry Verification**: Inspect `.dc test list` to ensure all raid tokens appear with their designated party size (20 or 40) and recommended levels.
3. **Comp Balance Unit Check**: Verify `BuildComp(1, 40)` returns exactly 3 tanks, 8 healers, and 29 DPS.
4. **End-to-End Test Execution**: Start a test raid (`.dc test start ony` or `.dc test start mc`) and verify in `.dc test status` that 40 bots log in, form a raid, teleport to instance entrance, and assign the highest-geared tank as leader.
