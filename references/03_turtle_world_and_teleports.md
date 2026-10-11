---
name: Turtle-WoW World, Dungeons & Teleports Directory
description: Navigation directory for Turtle-WoW 1.18.1 — the copy-paste macro sheets that teleport a group anywhere, the instant raid attunement and master keyring suite, and a per-instance table of Turtle's custom maps with player limits and whether a token drops you inside the instance or at its world entrance. Use it when you need a .tele token, a fast-group-travel macro, or the map id and player cap of a Turtle custom dungeon.
version: 1.0.0
platform: Turtle-WoW 1.18.1 (Build 7272)
stack: CMaNGOS + mod-playerbots (ike3 / r-o-sh) + mod-dungeon-clear
glob:
  - sql/base/tw_world_game_tele.sql
  - sql/base/tw_world_map_template.sql
  - sql/base/tw_world_areatrigger_teleport.sql
  - sql/base/tw_world_areatrigger_template.sql
  - src/game/Chat/Chat.cpp
  - src/game/Commands/Commands.cpp
  - src/game/ObjectMgr.cpp
  - src/game/Maps/
tags:
  - teleports
  - game-tele
  - instances
  - attunements
  - keyring
  - custom-maps
  - navigation
  - macros
  - turtle-wow
---

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
   - [Custom Turtle-WoW Dungeons (5-Man & 10-Man)](#custom-turtle-wow-dungeons-5-man--10-man)
   - [Custom Turtle-WoW Raids (10-Man / 20-Man / 40-Man)](#custom-turtle-wow-raids-10-man--20-man--40-man)
5. [Master Instance Teleport Directory (Classic & Custom)](#4-master-instance-teleport-directory-classic--custom)
   - [Eastern Kingdoms Dungeons & Raids](#eastern-kingdoms-dungeons)
   - [Kalimdor Dungeons & Raids](#kalimdor-dungeons)
   - [Classic & Custom Raids](#classic--custom-raids)
   - [World Boss Spawns & Encounters](#world-bosses--dragons-of-nightmare)
6. [Open-World Zone & Capital Cities Directory](#5-open-world-zone--capital-cities-directory)
   - [Capital Cities & Major Faction Hubs](#capital-cities--major-faction-hubs)
   - [Eastern Kingdoms Zones](#eastern-kingdoms-zones)
   - [Kalimdor Zones](#kalimdor-zones)
   - [Turtle-WoW Custom World Zones & Islands](#turtle-wow-custom-world-zones--islands)
7. [Movement, Spatial Navigation & Summoning Commands](#6-movement-spatial-navigation--summoning-commands)

---

## Unified 5-Volume Master Reference Architecture

- **Volume 01: [Game Master & World Administration](./01_game_master_handbook.md)**
  - *Domain*: 100% of GM commands: Self progression/cheats (`.god`, `.gm fly`, `.levelup 60`), accounts, security (0–6), spawning (`.npc`, `.gobject`), economy, moderation, 106+ `.reload` tables & diagnostics.
- **Volume 02: [Playerbot & Raid Automation](./02_playerbot_and_raid_automation.md)**
  - *Domain*: Complete Bot Control Center: `.bot` & `.rndbot` suites, single-letter whisper shortcuts (`c`, `u`, `e`, `t`, `s`, `r`), chat filters (`@tank`, `@skull`), strategy tokens (`co`/`nc`), `.dc` autonomous dungeon clear solver & raid macros.
- **Volume 03: World, Dungeons & Teleports Directory (Current File)**
  - *Domain*: 647+ `.tele` tokens, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements.
- **Volume 04: [Class Specs & Endgame Gear Directory](./04_class_specs_and_endgame_gear.md)**
  - *Domain*: All 9 Classes: per-class talent skeletons, 30 one-click gear loadout examples, Tier 1/2/3 set IDs, weapons, trinkets, enchants & DB-verified ability citations (spell IDs + talent rows).
- **Volume 05: [Eluna Scripting & Core Development Engine](./05_developer_eluna_and_engine.md)**
  - *Domain*: Eluna Lua API: Live reloads (`.reload eluna`), Event 42 Custom Command dispatch, Turtle-WoW `SpellInfo` compatibility layer, and production server hook mechanics.

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
Teleports group to the custom Scarlet Citadel raid entrance in Tirisfal Glades:

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

> ⚠️ **Landing Maps vs Instance Maps**: Some tokens place you *inside* the instance copy; others are *world entrance* points in an outdoor map that lead to the portal door. Verified against `tw_world.map_template` and `game_tele`.

### Custom Turtle-WoW Dungeons (5-Man & 10-Man)

```lua
-- Crescent Grove: Level 32-38 | Instance Map: 802 instance_crescent_grove | Landing: 802 (inside) | Ashenvale (Night Elf Barrow Den)
.tele crescentgrove

-- Hateforge Quarry: Level 50-60 | Instance Map: 808 Hateforge (5-man) | Landing: 808 (inside) | Burning Steppes (Dark Iron Mining Facility)
.tele hateforgeinstance

-- Gilneas City: Level 42-48 | Instance Map: 815 instance_gilneas_city (10-player limit) | Landing: 815 (inside) | Gilneas (Gothic Plagued City Ruins)
.tele gilneascity

-- Alah'Thalas: Level 55-60 | Instance Map: 804 Eldrethalas (map_type 0) | Landing: 0 -> alahthalasarrival | Quel'Thalas (High Elf Arcane Sanctum)
.tele alahthalas

-- Stormwind Vault: Level 60 | Instance Map: 35 instance_stormwind_vault | Landing: 0 (entrance) | Stormwind Canal (Subterranean Prison)
.tele stormwindvault

-- Karazhan Crypts: Level 58-60 | Instance Map: 800 instance_karazhan_crypt | Landing: 0 (entrance) | Deadwind Pass (Forgotten Morgan's Plot Tombs)
.tele karazhancrypt
```

### Custom Turtle-WoW Raids (10-Man / 20-Man / 40-Man)

```lua
-- Scarlet Citadel: 40-Man | Instance Map: 45 instance_scarlet_citadel | Landing: 45 (inside) | Tirisfal Glades (High Inquisitor's Bastion)
.tele ScarletCitadel

-- Tower of Karazhan: 10-Man / 40-Man | Instance Map: 10-Man: 532 instance_lower_karazhan_halls / 40-Man: 814 Karazhan (Upper) | Landing: 0 (entrance) | Deadwind Pass (Medivh's Upper Quarters)
.tele Karazhan

-- Timbermaw Hold: World point only (no instance map) | Landing: 1 (Kalimdor) | Felwood (Uncorrupted Furbolg Sacred Burrow)
.tele TimbermawHold

-- Emerald Sanctum: 40-Man | Instance Map: 807 instance_emerald_sanctum | Landing: 807 (inside) | Mount Hyjal (Corrupted Emerald Dream Portal)
.tele sanctumraid
```

> **Notes on Custom Maps:**
> - `Gilneas City` (map 815) is a **10-player** instance, not 5-player.
> - `TimbermawHold` has no `map_template` row at all — `game_tele` id 280 is an outdoor world point on map 1, so it is **not** a 20-man raid.
> - `Scarlet Citadel` is a **40-man** raid, not 10-man.
> - Map IDs 805, 806, 810, 811, 812 and 813 are unrelated (`806` = `Frostmane Retreat`, `813` = `Winter Veil Vale`) or do not exist.

---

## 4. Master Instance Teleport Directory (Classic & Custom)

Complete destination tokens for every dungeon and raid across Azeroth:

> ⚠️ **A wrong `.tele` token does not error — it silently teleports you somewhere else.** `ObjectMgr::GetGameTele` (`src/game/ObjectMgr.cpp:8018-8034`) first tries an exact lowercased name match. If there is none it falls back to the **first `game_tele` row whose name merely *contains* your token, in ascending id order**. So an abbreviation or typo lands you on an unrelated row instead of returning "not found":

```lua
-- Substring match pitfalls (typing imprecise or abbreviated tokens):
.tele stv             -- Expected: Stranglethorn Vale -> Actually lands: 488 EastvaleLoggingCamp (map 0)
.tele sm              -- Expected: Scarlet Monastery  -> Actually lands: 46 RagefireChasm (map 1)
.tele molten          -- Expected: Molten Core        -> Actually lands: 163 TheMoltenSpan (map 0, EPL)
-- ALWAYS spell the token out in full. Tokens with no exact or substring match return LANG_COMMAND_TELE_NOTFOUND.
```

### Eastern Kingdoms Dungeons

```lua
.tele brd              -- Blackrock Depths
.tele lbrs             -- Lower Blackrock Spire
.tele ubrs             -- Upper Blackrock Spire (10-Man)
.tele scholo           -- Scholomance
.tele Stratholme       -- Stratholme Main Gate (also .tele strat - instance copy, map 329)
.tele sfk              -- Shadowfang Keep
.tele smcath           -- Scarlet Monastery: Cathedral (map 189)
.tele smarmory         -- Scarlet Monastery: Armory (map 189)
.tele smgy             -- Scarlet Monastery: Library / Graveyard (map 189)
.tele deadmines        -- The Deadmines
.tele st               -- The Sunken Temple
.tele uldaman          -- Uldaman
.tele gnomeregan       -- Gnomeregan
.tele stormwindvault   -- Stormwind Vault (Turtle Custom)
.tele karazhancrypt    -- Karazhan Crypts (Turtle Custom)
.tele gilneascity      -- Gilneas City (Turtle Custom)
```

### Kalimdor Dungeons

```lua
.tele dmeast           -- Dire Maul (East)
.tele dmnorth          -- Dire Maul (North)
.tele dmwest           -- Dire Maul (West)
.tele maraudon         -- Maraudon (Purple / Orange)
.tele Zul'Farrak       -- Zul'Farrak
.tele rfd              -- Razorfen Downs
.tele rfk              -- Razorfen Kraul
.tele wc               -- Wailing Caverns
.tele bfd              -- Blackfathom Deeps
.tele rfc              -- Ragefire Chasm
.tele crescentgrove    -- Crescent Grove (Turtle Custom)
.tele hateforgeinstance -- Hateforge Quarry (Turtle Custom)
```

### Classic & Custom Raids

```lua
.tele MoltenCore       -- Molten Core (40-Man)
.tele bwl              -- Blackwing Lair (40-Man)
.tele ony              -- Onyxia's Lair (40-Man)
.tele zg               -- Zul'Gurub (20-Man)
.tele aq20             -- Ruins of Ahn'Qiraj (20-Man)
.tele aq40             -- Temple of Ahn'Qiraj (40-Man)
.tele naxxramas        -- Naxxramas (40-Man)
.tele ScarletCitadel   -- Scarlet Citadel (Turtle Custom 40-Man)
.tele Karazhan         -- Tower of Karazhan (Turtle Custom 10-Man / 40-Man)
.tele TimbermawHold    -- Timbermaw Hold (Turtle Custom, world point only)
.tele sanctumraid      -- Emerald Sanctum (Turtle Custom 40-Man)
```

### World Bosses & Dragons of Nightmare

```lua
.tele seradane         -- Lethon: Seradane, The Hinterlands (Eastern Kingdoms)
.tele twilightgrove    -- Taerar: Twilight Grove, Duskwood (Eastern Kingdoms)
-- NOTE: No game_tele row exists for Lord Kazzak, Azuregos, Ysondre (Bough Shadow), or Emeriss (Dream Bough).
-- Workaround: Use nearby tokens (.tele TheTaintedScar for Kazzak, .tele Azshara for Azuregos) or add via '.tele add <name>'.
```

---

## 5. Open-World Zone & Capital Cities Directory

### Capital Cities & Major Faction Hubs

```lua
-- Alliance Capitals
.tele stormwind        -- Stormwind City
.tele ironforge        -- Ironforge
.tele darnassus        -- Darnassus
.tele alahthalas       -- Alah'Thalas (High Elf Capital)

-- Horde Capitals
.tele orgrimmar        -- Orgrimmar
.tele thunderbluff     -- Thunder Bluff
.tele undercity        -- Undercity

-- Neutral Goblin & Cenarion Ports
.tele BootyBay         -- Booty Bay (Stranglethorn)
.tele ratchet          -- Ratchet (Barrens)
.tele gadgetzan        -- Gadgetzan (Tanaris)
.tele everlook         -- Everlook (Winterspring)
.tele cenarion         -- Cenarion Hold (Silithus)
.tele lightshope       -- Light's Hope Chapel (Eastern Plaguelands)
.tele moonglade        -- Moonglade
```

### Eastern Kingdoms Zones

```lua
.tele elwynn           -- Elwynn Forest
.tele westfall         -- Westfall
.tele redridge         -- Redridge Mountains
.tele duskwood         -- Duskwood
.tele lochmodan        -- Loch Modan
.tele wetlands         -- Wetlands
.tele arathi           -- Arathi Highlands
.tele hillsbrad        -- Hillsbrad Foothills
.tele alterac          -- Alterac Mountains
.tele hinterlands      -- The Hinterlands
.tele badlands         -- Badlands
.tele searing          -- Searing Gorge
.tele burning          -- Burning Steppes
.tele swamp            -- Swamp of Sorrows
.tele blasted          -- Blasted Lands
.tele StranglethornVale -- Stranglethorn Vale
.tele tirisfal         -- Tirisfal Glades
.tele silverpine       -- Silverpine Forest
.tele WesternPlaguelands -- Western Plaguelands
.tele epl              -- Eastern Plaguelands
.tele deadwind         -- Deadwind Pass
```

### Kalimdor Zones

```lua
.tele durotar          -- Durotar
.tele barrens          -- The Barrens
.tele mulgore          -- Mulgore
.tele stonetalon       -- Stonetalon Mountains
.tele ashenvale        -- Ashenvale
.tele darkshore        -- Darkshore
.tele teldrassil       -- Teldrassil
.tele thousandneedles  -- Thousand Needles
.tele desolace         -- Desolace
.tele dustwallow       -- Dustwallow Marsh
.tele feralas          -- Feralas
.tele tanaris          -- Tanaris
.tele Un'GoroCrater    -- Un'Goro Crater
.tele silithus         -- Silithus
.tele felwood          -- Felwood
.tele winterspring     -- Winterspring
.tele azshara          -- Azshara
```

### Turtle-WoW Custom World Zones & Islands

```lua
.tele alahthalas       -- Alah'Thalas (High Elf Haven, lands at alahthalasarrival, map 0)
.tele gilijimisle      -- Gillijim's Isle
.tele lapidis          -- Island of Lapidis
.tele hyjal            -- Mount Hyjal (Open-World Sanctuary)
.tele telabim          -- Tel'Abim Island
```

---

## 6. Movement, Spatial Navigation & Summoning Commands

Verified against CMaNGOS command tables in `src/game/Chat/Chat.cpp` and handlers in `src/game/Commands/Commands.cpp`.  
*Security Levels*: 0 Player, 1 Observer, 2 Moderator, 3 Developer, 4 Administrator (`src/shared/Common.h:184-193`).

```lua
-- .gps [target]
-- Sec: 1 (Observer) | Dumps Map ID, Area ID, Zone ID, 3D coordinates (X, Y, Z), and orientation.
.gps

-- .go xyz <x> <y> <z> [map_id]
-- Sec: 1 (Observer) | Teleports to exact 3D coordinates. map_id: 0=EK, 1=Kalimdor (defaults to current map).
-- NOTE: Orientation is not a parameter for .go xyz; use .tele if orientation is needed.
.go xyz -8949 -132 83 0

-- .go forward [yards]
-- Sec: 1 (Observer) | Blinks forward through world geometry in facing direction (default 10 yd).
.go forward 20

-- .go up [yards]
-- Sec: 1 (Observer) | Teleports straight up along vertical Z axis (default 10 yd).
.go up 15

-- .go target
-- Sec: 1 (Observer) | Teleports to selected target's exact position. Same map only; cross-map fails with LANG_SELECT_CHAR_OR_CREATURE.
.go target

-- .go corpse
-- Sec: 3 (Developer) | Teleports directly to player ghost dead body coordinates.
.go corpse

-- .go creature <guid>
-- Sec: 1 (Observer) | Teleports to NPC spawn location matching database GUID.
.go creature 10182

-- .go object <guid>
-- Sec: 1 (Observer) | Teleports to static GameObject matching database GUID.
.go object 179501

-- .go taxinode <id>
-- Sec: 1 (Observer) | Teleports directly to taxi flight node ID coordinates.
.go taxinode 2

-- .go trigger <id>
-- Sec: 1 (Observer) | Teleports to area trigger ID entrance coordinates.
.go trigger 120

-- .go graveyard <id>
-- Sec: 1 (Observer) | Teleports directly to graveyard ID coordinates.
.go graveyard 4

-- .tele <dest_token>
-- Sec: 1 (Observer) | Teleports character to pre-defined location token.
-- NOTE: Unknown tokens silently fall back to substring match. Spell tokens out completely.
.tele stormwind

-- .tele group <dest_token>
-- Sec: 3 (Developer) | Teleports selected target player and their whole group simultaneously.
-- NOTE: Requires selected player. Skips members without active session (silently excludes playerbots).
.tele group MoltenCore

-- .tele name <player> <dest_token>
-- Sec: 3 (Developer) | Teleports specified player directly to destination token.
.tele name Sam naxxramas

-- .tele add <dest_token>
-- Sec: 3 (Developer) | Saves current player coordinates as new permanent .tele token in database.
.tele add mybase

-- .tele del <dest_token>
-- Sec: 3 (Developer) | Deletes .tele token from database.
.tele del mybase

-- .appear <player>
-- Sec: 1 (Observer) | Teleports you directly to specified player's current location.
.appear Alice

-- .summon <player>
-- Sec: 2 (Moderator) | Teleports player or bot to your current location.
.summon Bob

-- .groupsummon <player>
-- Sec: 2 (Moderator) | Teleports target player and their entire group to your location.
.groupsummon Bob

-- .recall [player]
-- Sec: 3 (Developer) | Returns player to coordinates occupied before last teleport.
.recall

-- .hover <0|1>
-- Sec: 4 (Administrator) | Enables free noclip-style levitation and flight (1=on, 0=off).
-- NOTE: Takes a numeric argument only. '.hover on' is rejected without syntax help.
.hover 1

-- .taxicheat <on|off>
-- Sec: 3 (Developer) | Unlocks all flight paths globally.
.taxicheat on

-- .explorecheat <0|1>
-- Sec: 3 (Developer) | Uncovers all world map fog across all continents (1=reveal, 0=clear).
-- NOTE: 'on' parses via atoi() as 0 and CLEARS the map! Always pass 1. Requires selected player; applies to caster.
.explorecheat 1

-- .unstuck
-- Sec: 2 (Moderator) | Casts emergency return to bound hearthstone innkeeper.
.unstuck
```

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md) • [🤖 RandomBot Guide (rndbot_commands.md)](../rndbot_commands.md)
