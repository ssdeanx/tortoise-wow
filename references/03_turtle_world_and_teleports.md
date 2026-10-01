# 03. Turtle-WoW World, Dungeons & Teleports Directory

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS Core Engine
> **Source Verification**: `tw_world.game_tele`, `tw_world.map_template`, command tables in `src/game/Chat/Chat.cpp` and handlers in `src/game/Commands/Commands.cpp`, `areatrigger_teleport`
> **Security Coverage**: Security Levels 0–4 (Player, Observer, Moderator, Developer, Administrator)
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md) • [🤖 RandomBot Guide (rndbot_commands.md)](../rndbot_commands.md)

---

## 📑 Table of Contents

1. [Unified 5-Volume Master Reference Architecture](#unified-5-volume-master-reference-architecture)
2. [Master Macro UI Cheat Sheet (Travel & Group Rally)](#1-master-macro-ui-cheat-sheet-travel--group-rally)
   - [Category A: Raid & Custom Dungeon Fast Group Teleports](#category-a-raid--custom-dungeon-fast-group-teleports)
   - [Category B: Spatial Navigation, Blink & Body Recovery](#category-b-spatial-navigation-blink--body-recovery)
   - [Category C: Capital Cities & Emergency Return](#category-c-capital-cities--emergency-return)
3. [Instant GM Attunement & Master Keyring Suite](#2-instant-gm-attunement--master-keyring-suite)
   - [Instant Attunement Auto-Complete](#instant-attunement-auto-complete)
   - [Master Physical Keyring Spawner](#master-physical-keyring-spawner)
4. [Complete Turtle-WoW Custom Instance Directory](#3-complete-turtle-wow-custom-instance-directory)
   - [Custom 5-Man & 10-Man Dungeons](#custom-turtle-wow-dungeons-5-man--10-man)
   - [Custom 10-Man, 20-Man & 40-Man Raids](#custom-turtle-wow-raids-10-man--20-man--40-man)
5. [Master Instance Teleport Directory (Classic & Custom)](#4-master-instance-teleport-directory-classic--custom)
   - [Eastern Kingdoms Dungeons & Raids](#eastern-kingdoms-dungeons)
   - [Kalimdor Dungeons & Raids](#kalimdor-dungeons)
   - [World Boss Spawns & Encounters](#world-bosses--dragons-of-nightmare)
6. [Open-World Zone & Capital Cities Directory](#5-open-world-zone--capital-cities-directory)
   - [Capital Cities & Major Faction Hubs](#capital-cities--major-faction-hubs)
   - [Eastern Kingdoms Zones](#eastern-kingdoms-zones)
   - [Kalimdor Zones](#kalimdor-zones)
   - [Turtle-WoW Custom World Zones & Islands](#turtle-wow-custom-world-zones--islands)
7. [Movement, Spatial Navigation & Summoning Commands](#6-movement-spatial-navigation--summoning-commands)

---

## Unified 5-Volume Master Reference Architecture

| Volume | Handbook Title | Domain & Core Coverage | Direct Link |
| :---: | :--- | :--- | :---: |
| **01** | **[Game Master & World Administration](./01_game_master_handbook.md)** | **100% of GM commands**: Self progression/cheats (`.god`, `.gm fly`, `.levelup 60`), accounts, security (0–6), spawning (`.npc`, `.gobject`), economy, moderation, 106+ `.reload` tables & diagnostics. | [Open Volume 01](./01_game_master_handbook.md) |
| **02** | **[Playerbot & Raid Automation](./02_playerbot_and_raid_automation.md)** | **Complete Bot Control Center**: `.bot` & `.rndbot` suites, single-letter whisper shortcuts (`c`, `u`, `e`, `t`, `s`, `r`), chat filters (`@tank`, `@skull`), strategy tokens (`co`/`nc`), `.dc` autonomous dungeon clear solver & raid macros. | [Open Volume 02](./02_playerbot_and_raid_automation.md) |
| **03** | **World, Dungeons & Teleports Directory** | **647+ .tele tokens, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements.** | **Current File** |
| **04** | **[Class Specs & Endgame Gear Directory](./04_class_specs_and_endgame_gear.md)** | **All 9 Classes**: per-class talent skeletons, 30 one-click gear loadout examples, Tier 1/2/3 set IDs, weapons, trinkets, enchants & DB-verified ability citations (spell IDs + talent rows). Turtle-WoW publishes **no canonical builds**, so doc 04 tags every claim by verification tier rather than asserting optimal spreads. | [Open Volume 04](./04_class_specs_and_endgame_gear.md) |
| **05** | **[Eluna Scripting & Core Development Engine](./05_developer_eluna_and_engine.md)** | **Eluna Lua API**: Live reloads (`.reload eluna`), Event 42 Custom Command dispatch, Turtle-WoW `SpellInfo` compatibility layer, and production server hook mechanics. | [Open Volume 05](./05_developer_eluna_and_engine.md) |

---

## 1. Master Macro UI Cheat Sheet (Travel & Group Rally)

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`):

### Category A: Raid & Custom Dungeon Fast Group Teleports

#### 🌋 Macro 1: Group Teleport to Molten Core & Rally

Teleports your character and all party/raid bots directly to the Molten Core entrance portal:

```lua
.tele group MoltenCore
.bot summon *
/p follow
```

#### 🏰 Macro 2: Group Teleport to Scarlet Citadel (Turtle Custom 40-Man Raid)

Teleports group to the custom 10-man Scarlet Citadel raid entrance in Tirisfal Glades:

```lua
.tele group ScarletCitadel
.bot summon *
/p follow
```

#### 🌿 Macro 3: Group Teleport to Crescent Grove (Turtle Custom Dungeon)

Teleports group to the custom level 32–38 Crescent Grove dungeon entrance in Ashenvale:

```lua
.tele group crescentgrove
.bot summon *
/p follow
```

#### ⛏️ Macro 4: Group Teleport to Hateforge Quarry (Turtle Custom Dungeon)

Teleports group to the custom level 50–60 Hateforge Quarry dungeon entrance in Burning Steppes:

```lua
.tele group hateforgeinstance
.bot summon *
/p follow
```

#### 💀 Macro 5: Group Teleport to Naxxramas

Teleports group to the Dread Citadel floating above Eastern Plaguelands:

```lua
.tele group naxxramas
.bot summon *
/p follow
```

---

### Category B: Spatial Navigation, Blink & Body Recovery

#### ⚡ Macro 6: 20-Yard Forward Blink & 15-Yard Upward Leap

Instantly blinks through world geometry in facing direction and lifts character 15 yards into the air:

```lua
.go forward 20
.go up 15
```

#### ⚰️ Macro 7: Teleport to Corpse & Instant Revive

Rescues dead characters by teleporting directly to corpse coordinates and resurrecting with full resources:

```lua
.go corpse
.revive
```

#### 🎯 Macro 8: Teleport Directly to Selected Target Unit

Instantly shifts your position directly onto your targeted player, bot, or creature. The target must be **on your current map** — a cross-map target fails with `LANG_SELECT_CHAR_OR_CREATURE`:

```lua
.go target
```

#### 🚀 Macro 9: Master Fast Travel Mode (Flight + Speed + Full Maps)

Activates noclip flight, boosts movement speed by 250%, and reveals all world map fog:

```lua
.hover 1
.modify speed 2.5
.explorecheat 1
```

> ⚠️ **Both cheats take a number, not `on`/`off`.** `.hover` parses only a numeric argument, so `.hover on` is silently rejected. `.explorecheat` is worse: it runs `atoi(args)`, so `on` evaluates to `0` and **clears** every explored zone — the exact opposite of what it reads like. It also requires a selected player, yet applies the flag to **you**, not the target.

---

### Category C: Capital Cities & Emergency Return

#### 🏛️ Macro 10: Teleport to Stormwind City (Alliance Hub)

Teleports to Trade District in Stormwind City and gathers party bots:

```lua
.tele group stormwind
.bot summon *
/p follow
```

#### 🐺 Macro 11: Teleport to Orgrimmar (Horde Hub)

Teleports to Valley of Strength in Orgrimmar and gathers party bots:

```lua
.tele group orgrimmar
.bot summon *
/p follow
```

#### 🆘 Macro 12: Emergency Hearthstone Return

Rescues stuck characters by triggering an instant hearthstone homebind return:

```lua
.unstuck
```

---

## 2. Instant GM Attunement & Master Keyring Suite

Copy and execute these command blocks to unlock all Classic and Turtle-WoW custom instance access requirements without farming quest chains:

### Instant Attunement Auto-Complete

```lua
-- Classic Raids & Dungeons
.quest add 7848
.quest complete 7848       -- Molten Core (Attunement to the Core)
.quest add 7761
.quest complete 7761       -- Blackwing Lair (Blackhand's Command)
.quest add 6502
.quest complete 6502       -- Onyxia's Lair Alliance (Drakefire Amulet)
.quest add 6602
.quest complete 6602       -- Onyxia's Lair Horde (Blood of the Black Dragon Champion)
.quest add 9123
.quest complete 9123       -- Naxxramas Exalted Attunement (The Dread Citadel)

-- Turtle-WoW Custom Raids & Dungeons
.quest add 40962
.quest complete 40962      -- Emerald Sanctum 40-Man (Into the Dream VI)
.quest add 40829
.quest complete 40829      -- Karazhan 10-Man / 40-Man (The Key to Karazhan X)
.quest add 40489
.quest complete 40489      -- Hateforge Quarry 5-Man (Assaulting Hateforge)
```

### Master Physical Keyring Spawner

```lua
.additem 16309 1           -- Drakefire Amulet (Onyxia Lair Key)
.additem 22754 1           -- Eternal Quintessence (Molten Core Infinite Rune Douse)
.additem 12344 1           -- Seal of Ascension (Upper Blackrock Spire 10-Man Gate Key)
.additem 12382 1           -- Key to the City (Stratholme Service Entrance Gate)
.additem 18249 1           -- Crescent Key (Dire Maul West & North Gates)
.additem 11000 1           -- Shadowforge Key (Blackrock Depths Gates & Mechanism)
.additem 13704 1           -- Skeleton Key (Scholomance Front Gate)
.additem 6893 1            -- Workshop Key (Gnomeregan Back Entrance)
.additem 7146 1            -- The Scarlet Key (Scarlet Monastery Armory & Cathedral)
```

---

## 3. Complete Turtle-WoW Custom Instance Directory

All exclusive custom dungeons and 10/20/40-man raids added in Turtle-WoW 1.18.1:

> ⚠️ **Read the two map columns separately.** Some tokens place you *inside* the instance copy; others are *world entrance* points in an outdoor map that merely lead you to the door. Verified against `tw_world.map_template` and the `game_tele` row for each token.

### Custom Turtle-WoW Dungeons (5-Man & 10-Man)

| Destination Token | Dungeon Name | Level Range | Instance Map | Landing Map | Location & Theme |
| :--- | :--- | :---: | :---: | :---: | :--- |
| `.tele crescentgrove` | Crescent Grove | 32–38 | **802** `instance_crescent_grove` | **802** (inside) | Ashenvale (Night Elf Barrow Den) |
| `.tele hateforgeinstance` | Hateforge Quarry | 50–60 | **808** `Hateforge` (5) | **808** (inside) | Burning Steppes (Dark Iron Mining Facility) |
| `.tele gilneascity` | Gilneas City | 42–48 | **815** `instance_gilneas_city` — **10-player limit** | **815** (inside) | Gilneas (Gothic Plagued City Ruins) |
| `.tele alahthalas` | Alah'Thalas | 55–60 | **804** `Eldrethalas` (map_type 0) | **0** → `alahthalasarrival` | Quel'Thalas (High Elf Arcane Sanctum) |
| `.tele stormwindvault` | Stormwind Vault | 60 | **35** `instance_stormwind_vault` | **0** (entrance) | Stormwind Canal (Subterranean Prison) |
| `.tele karazhancrypt` | Karazhan Crypts | 58–60 | **800** `instance_karazhan_crypt` | **0** (entrance) | Deadwind Pass (Forgotten Morgan's Plot Tombs) |

### Custom Turtle-WoW Raids (10-Man / 20-Man / 40-Man)

| Destination Token | Raid Name | Size | Instance Map | Landing Map | Location & Theme |
| :--- | :--- | :---: | :---: | :---: | :--- |
| `.tele ScarletCitadel` | Scarlet Citadel | 40-Man | **45** `instance_scarlet_citadel` | **45** (inside) | Tirisfal Glades (High Inquisitor's Bastion) |
| `.tele Karazhan` | Tower of Karazhan | 10-Man / 40-Man | 10-Man: **532** `instance_lower_karazhan_halls` • 40-Man: **814** `Karazhan (Upper)` | **0** (entrance) | Deadwind Pass (Medivh's Upper Quarters) |
| `.tele TimbermawHold` | Timbermaw Hold | — | **No instance map** — world point only | **1** (Kalimdor) | Felwood (Uncorrupted Furbolg Sacred Burrow) |
| `.tele sanctumraid` | Emerald Sanctum | 40-Man | **807** `instance_emerald_sanctum` | **807** (inside) | Mount Hyjal (Corrupted Emerald Dream Portal) |

> **Notes.** `Gilneas City` (map 815) is a **10-player** instance, not 5-player. `TimbermawHold` has no `map_template` row at all — `game_tele` id 280 is an outdoor world point on map 1, so it is **not** a 20-man raid. `Scarlet Citadel` is a **40-man** raid, not 10-man. Map IDs 805, 806, 810, 811, 812 and 813 are unrelated (`806` = `Frostmane Retreat`, `813` = `Winter Veil Vale`) or do not exist.

---

## 4. Master Instance Teleport Directory (Classic & Custom)

Complete destination tokens for every dungeon and raid across Azeroth:

> ⚠️ **A wrong `.tele` token does not error — it silently teleports you somewhere else.** `ObjectMgr::GetGameTele` (`src/game/ObjectMgr.cpp:8018-8034`) first tries an exact lowercased name match. If there is none it falls back to the **first `game_tele` row whose name merely *contains* your token, in ascending id order**. So an abbreviation or typo lands you on an unrelated row instead of returning "not found":
>
> | Token you typed | What you expected | Where you actually land |
> | :--- | :--- | :--- |
> | `.tele stv` | Stranglethorn Vale | **488 `EastvaleLoggingCamp`** (map 0) — "Ea**stv**ale…" |
> | `.tele sm` | Scarlet Monastery | **46 `RagefireChasm`** (map 1) — "cha**sm**" |
> | `.tele molten` | Molten Core | **163 `TheMoltenSpan`** (map 0, Eastern Plaguelands) |
>
> Always spell the token out in full. Tokens with no exact row *and* no substring match do return `LANG_COMMAND_TELE_NOTFOUND`.

### Eastern Kingdoms Dungeons

- **Blackrock Depths**: `.tele brd`
- **Lower Blackrock Spire**: `.tele lbrs`
- **Upper Blackrock Spire (10-Man)**: `.tele ubrs`
- **Scholomance**: `.tele scholo`
- **Stratholme (Main Gate)**: `.tele Stratholme` (also `.tele strat` — instance copy, map 329)
- **Shadowfang Keep**: `.tele sfk`
- **Scarlet Monastery**: `.tele smcath` (Cathedral) • `.tele smarmory` (Armory) • `.tele smgy` (Library) — all map 189
- **The Deadmines**: `.tele deadmines`
- **The Sunken Temple**: `.tele st`
- **Uldaman**: `.tele uldaman`
- **Gnomeregan**: `.tele gnomeregan`
- **Stormwind Vault (Turtle Custom)**: `.tele stormwindvault`
- **Karazhan Crypts (Turtle Custom)**: `.tele karazhancrypt`
- **Gilneas City (Turtle Custom)**: `.tele gilneascity`

### Kalimdor Dungeons

- **Dire Maul (East)**: `.tele dmeast`
- **Dire Maul (North)**: `.tele dmnorth`
- **Dire Maul (West)**: `.tele dmwest`
- **Maraudon (Purple / Orange)**: `.tele maraudon`
- **Zul'Farrak**: `.tele Zul'Farrak`
- **Razorfen Downs**: `.tele rfd`
- **Razorfen Kraul**: `.tele rfk`
- **Wailing Caverns**: `.tele wc`
- **Blackfathom Deeps**: `.tele bfd`
- **Ragefire Chasm**: `.tele rfc`
- **Crescent Grove (Turtle Custom)**: `.tele crescentgrove`
- **Hateforge Quarry (Turtle Custom)**: `.tele hateforgeinstance`

### Classic & Custom Raids

- **Molten Core (40-Man)**: `.tele MoltenCore`
- **Blackwing Lair (40-Man)**: `.tele bwl`
- **Onyxia's Lair (40-Man)**: `.tele ony`
- **Zul'Gurub (20-Man)**: `.tele zg`
- **Ruins of Ahn'Qiraj (20-Man)**: `.tele aq20`
- **Temple of Ahn'Qiraj (40-Man)**: `.tele aq40`
- **Naxxramas (40-Man)**: `.tele naxxramas`
- **Scarlet Citadel (Turtle Custom 40-Man)**: `.tele ScarletCitadel`
- **Tower of Karazhan (Turtle Custom 10-Man / 40-Man)**: `.tele Karazhan`
- **Timbermaw Hold (Turtle Custom, world point only)**: `.tele TimbermawHold`
- **Emerald Sanctum (Turtle Custom 40-Man)**: `.tele sanctumraid`

### World Bosses & Dragons of Nightmare

| World Boss | Teleport Token / Location | Zone & Continent |
| :--- | :--- | :--- |
| **Lethon** | `.tele seradane` | Seradane, The Hinterlands (EK) |
| **Taerar** | `.tele twilightgrove` | Twilight Grove, Duskwood (EK) |

> ⚠️ **No `game_tele` row exists for Lord Kazzak, Azuregos, Ysondre (Bough Shadow), or Emeriss (Dream Bough)** — those tokens return `LANG_COMMAND_TELE_NOTFOUND`. Use a nearby exact token (e.g. `.tele TheTaintedScar` for Kazzak, `.tele Azshara` for Azuregos) and finish on foot, or add a `game_tele` row with `.tele add <name>` first.

---

## 5. Open-World Zone & Capital Cities Directory

### Capital Cities & Major Faction Hubs

- **Alliance Capitals**:
  - Stormwind City: `.tele stormwind`
  - Ironforge: `.tele ironforge`
  - Darnassus: `.tele darnassus`
  - Alah'Thalas (High Elf Capital): `.tele alahthalas`
- **Horde Capitals**:
  - Orgrimmar: `.tele orgrimmar`
  - Thunder Bluff: `.tele thunderbluff`
  - Undercity: `.tele undercity`
- **Neutral Goblin & Cenarion Ports**:
  - Booty Bay (Stranglethorn): `.tele BootyBay`
  - Ratchet (Barrens): `.tele ratchet`
  - Gadgetzan (Tanaris): `.tele gadgetzan`
  - Everlook (Winterspring): `.tele everlook`
  - Cenarion Hold (Silithus): `.tele cenarion`
  - Light's Hope Chapel (EPL): `.tele lightshope`
  - Moonglade: `.tele moonglade`

### Eastern Kingdoms Zones

- **Elwynn Forest**: `.tele elwynn`
- **Westfall**: `.tele westfall`
- **Redridge Mountains**: `.tele redridge`
- **Duskwood**: `.tele duskwood`
- **Loch Modan**: `.tele lochmodan`
- **Wetlands**: `.tele wetlands`
- **Arathi Highlands**: `.tele arathi`
- **Hillsbrad Foothills**: `.tele hillsbrad`
- **Alterac Mountains**: `.tele alterac`
- **The Hinterlands**: `.tele hinterlands`
- **Badlands**: `.tele badlands`
- **Searing Gorge**: `.tele searing`
- **Burning Steppes**: `.tele burning`
- **Swamp of Sorrows**: `.tele swamp`
- **Blasted Lands**: `.tele blasted`
- **Stranglethorn Vale**: `.tele StranglethornVale`
- **Tirisfal Glades**: `.tele tirisfal`
- **Silverpine Forest**: `.tele silverpine`
- **Western Plaguelands**: `.tele WesternPlaguelands`
- **Eastern Plaguelands**: `.tele epl`
- **Deadwind Pass**: `.tele deadwind`

### Kalimdor Zones

- **Durotar**: `.tele durotar`
- **The Barrens**: `.tele barrens`
- **Mulgore**: `.tele mulgore`
- **Stonetalon Mountains**: `.tele stonetalon`
- **Ashenvale**: `.tele ashenvale`
- **Darkshore**: `.tele darkshore`
- **Teldrassil**: `.tele teldrassil`
- **Thousand Needles**: `.tele thousandneedles`
- **Desolace**: `.tele desolace`
- **Dustwallow Marsh**: `.tele dustwallow`
- **Feralas**: `.tele feralas`
- **Tanaris**: `.tele tanaris`
- **Un'Goro Crater**: `.tele Un'GoroCrater`
- **Silithus**: `.tele silithus`
- **Felwood**: `.tele felwood`
- **Winterspring**: `.tele winterspring`
- **Azshara**: `.tele azshara`

### Turtle-WoW Custom World Zones & Islands

- **Alah'Thalas (High Elf Haven)**: `.tele alahthalas` (lands at `alahthalasarrival`, map 0)
- **Gillijim's Isle**: `.tele gilijimisle`
- **Island of Lapidis**: `.tele lapidis`
- **Mount Hyjal (Open-World Sanctuary)**: `.tele hyjal`
- **Tel'Abim Island**: `.tele telabim`

---

## 6. Movement, Spatial Navigation & Summoning Commands

Verified against the CMaNGOS command **tables** in `src/game/Chat/Chat.cpp` and the **handlers** in `src/game/Commands/Commands.cpp` (`HandleGoForwardCommand:13550`, `HandleGoUpCommand:13563`, `HandleGoXYZCommand:8371`, `HandleGoTargetCommand:8962`, `HandleHoverCommand:3498`, `HandleTeleGroupCommand:8073`, `HandleUnstuckCommand:6319`, and the rest of the `.tele`/`.go` family cited inline). `Sec` uses the `AccountTypes` enum in `src/shared/Common.h:184-193` — 0 Player, 1 Observer, 2 Moderator, 3 Developer, 4 Administrator.

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.gps` | 1 | `.gps [target]` | Dumps Map ID, Area ID, Zone ID, 3D coordinates, and orientation. | `.gps` |
| `.go xyz` | 1 | `.go xyz <x> <y> <z> [map_id]` | Teleports directly to exact 3D coordinates (map: 0=EK, 1=Kalimdor). Orientation is **not** a parameter — use `.tele`, which stores one. `map_id` defaults to your current map. | `.go xyz -8949 -132 83 0` |
| `.go forward` | 1 | `.go forward [yards]` | Blinks forward through world geometry in facing direction (default 10yd). | `.go forward 20` |
| `.go up` | 1 | `.go up [yards]` | Teleports straight up along vertical Z axis (default 10yd). | `.go up 15` |
| `.go target` | 1 | `.go target` | Teleports to the target's exact position — **same map only**; a cross-map target fails with `LANG_SELECT_CHAR_OR_CREATURE`. | `.go target` |
| `.go corpse` | 3 | `.go corpse` | Teleports directly to player ghost dead body coordinates. | `.go corpse` |
| `.go creature` | 1 | `.go creature <guid>` | Teleports to NPC spawn location matching database GUID. | `.go creature 10182` |
| `.go object` | 1 | `.go object <guid>` | Teleports to static GameObject matching database GUID. | `.go object 179501` |
| `.go taxinode` | 1 | `.go taxinode <id>` | Teleports directly to taxi flight node ID. | `.go taxinode 2` |
| `.go trigger` | 1 | `.go trigger <id>` | Teleports to area trigger ID entrance. | `.go trigger 120` |
| `.go graveyard` | 1 | `.go graveyard <id>` | Teleports directly to graveyard ID coordinates. | `.go graveyard 4` |
| `.tele` | 1 | `.tele <dest_token>` | Teleports character to pre-defined location token. Unknown tokens silently fall back to a substring match — see the warning in §4. | `.tele stormwind` |
| `.tele group` | 3 | `.tele group <dest_token>` | Teleports the **selected target player** and their whole group simultaneously. Requires a selected player (`LANG_NO_CHAR_SELECTED` otherwise) and skips any member without a session — which silently excludes playerbots. | `.tele group MoltenCore` |
| `.tele name` | 3 | `.tele name <player> <dest>` | Teleports specified player directly to destination token. | `.tele name Sam naxxramas` |
| `.tele add` | 3 | `.tele add <dest_token>` | Saves current player coordinates as new permanent `.tele` token in DB. | `.tele add mybase` |
| `.tele del` | 3 | `.tele del <dest_token>` | Deletes `.tele` token from database. | `.tele del mybase` |
| `.appear` | 1 | `.appear <player>` | Teleports you directly to specified player current location. | `.appear Alice` |
| `.summon` | 2 | `.summon <player>` | Teleports player or bot to your current location. | `.summon Bob` |
| `.groupsummon` | 2 | `.groupsummon <player>` | Teleports player and their entire group to your location. | `.groupsummon Bob` |
| `.recall` | 3 | `.recall [player]` | Returns player to coordinates occupied before last teleport. | `.recall` |
| `.hover` | 4 | `.hover <0\|1>` | Enables free noclip style levitation and flight. Takes a **number** — `on` is non-numeric, is rejected, and emits no syntax help. | `.hover 1` |
| `.taxicheat` | 3 | `.taxicheat <on\|off>` | Unlocks all flight paths globally. | `.taxicheat on` |
| `.explorecheat` | 3 | `.explorecheat <0\|1>` | Uncovers all world map fog across all continents. ⚠️ `on` is parsed by `atoi()` as `0` and **clears** the map instead. Requires a selected player, but applies the flag to you, not the target. | `.explorecheat 1` |
| `.unstuck` | 2 | `.unstuck` | Casts emergency return to bound hearthstone innkeeper. | `.unstuck` |

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md) • [🤖 RandomBot Guide (rndbot_commands.md)](../rndbot_commands.md)
