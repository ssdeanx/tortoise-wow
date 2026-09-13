# 03. Turtle-WoW World, Dungeons & Teleports Directory

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS Core Engine
> **Source Verification**: `tw_world.game_tele`, `src/game/Chat/Chat.cpp`, `areatrigger_teleport`
> **Security Coverage**: Security Levels 0–3 (Player, Moderator, GameMaster, Administrator)
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
   - [Custom 5-Man Dungeons](#custom-turtle-wow-dungeons-5-man)
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
| **01** | **[Game Master & World Administration](./01_game_master_handbook.md)** | **100% of GM commands**: Self progression/cheats (`.god`, `.fly`, `.levelup 60`), accounts, security (0–6), spawning (`.npc`, `.gobject`), economy, moderation, 106+ `.reload` tables & diagnostics. | [Open Volume 01](./01_game_master_handbook.md) |
| **02** | **[Playerbot & Raid Automation](./02_playerbot_and_raid_automation.md)** | **Complete Bot Control Center**: `.bot` & `.rndbot` suites, single-letter whisper shortcuts (`c`, `u`, `e`, `t`, `s`, `r`), chat filters (`@tank`, `@skull`), strategy tokens (`co`/`nc`), `.dc` autonomous dungeon clear solver & raid macros. | [Open Volume 02](./02_playerbot_and_raid_automation.md) |
| **03** | **World, Dungeons & Teleports Directory** | **647+ .tele tokens, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements.** | **Current File** |
| **04** | **[Class Specs & Endgame Gear Directory](./04_class_specs_and_endgame_gear.md)** | **All 9 Classes**: 1-Click BiS gear loadout macros, complete Tier 1/2/3 set IDs, weapons, trinkets, enchants & verified 1.18.1 talent builds across 30 distinct specializations. | [Open Volume 04](./04_class_specs_and_endgame_gear.md) |
| **05** | **[Eluna Scripting & Core Development Engine](./05_developer_eluna_and_engine.md)** | **Eluna Lua API**: Live reloads (`.reload eluna`), Event 42 Custom Command dispatch, Turtle-WoW `SpellInfo` compatibility layer, and production server hook mechanics. | [Open Volume 05](./05_developer_eluna_and_engine.md) |

---

## 1. Master Macro UI Cheat Sheet (Travel & Group Rally)

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`):

### Category A: Raid & Custom Dungeon Fast Group Teleports

#### 🌋 Macro 1: Group Teleport to Molten Core & Rally

Teleports your character and all party/raid bots directly to the Molten Core entrance portal:

```lua
.tele group molten
.bot summon *
/p @all follow
```

#### 🏰 Macro 2: Group Teleport to Scarlet Citadel (Turtle Custom 10-Man Raid)

Teleports group to the custom 10-man Scarlet Citadel raid entrance in Tirisfal Glades:

```lua
.tele group ScarletCitadel
.bot summon *
/p @all follow
```

#### 🌿 Macro 3: Group Teleport to Crescent Grove (Turtle Custom Dungeon)

Teleports group to the custom level 32–38 Crescent Grove dungeon entrance in Ashenvale:

```lua
.tele group crescentgrove
.bot summon *
/p @all follow
```

#### ⛏️ Macro 4: Group Teleport to Hateforge Quarry (Turtle Custom Dungeon)

Teleports group to the custom level 50–60 Hateforge Quarry dungeon entrance in Burning Steppes:

```lua
.tele group hateforgeinstance
.bot summon *
/p @all follow
```

#### 💀 Macro 5: Group Teleport to Naxxramas

Teleports group to the Dread Citadel floating above Eastern Plaguelands:

```lua
.tele group naxxramas
.bot summon *
/p @all follow
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

Instantly shifts your position directly onto your targeted player, bot, or creature:

```lua
.go target
```

#### 🚀 Macro 9: Master Fast Travel Mode (Flight + Speed + Full Maps)

Activates noclip flight, boosts movement speed by 250%, and reveals all world map fog:

```lua
.hover on
.modify speed 2.5
.explorecheat on
```

---

### Category C: Capital Cities & Emergency Return

#### 🏛️ Macro 10: Teleport to Stormwind City (Alliance Hub)

Teleports to Trade District in Stormwind City and gathers party bots:

```lua
.tele group stormwind
.bot summon *
/p @all follow
```

#### 🐺 Macro 11: Teleport to Orgrimmar (Horde Hub)

Teleports to Valley of Strength in Orgrimmar and gathers party bots:

```lua
.tele group orgrimmar
.bot summon *
/p @all follow
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
.quest add 41931
.quest complete 41931      -- Timbermaw Hold 20-Man (The Corruption of Timbermaw Hold)
.quest add 40489
.quest complete 40489      -- Hateforge Quarry 5-Man (Assaulting Hateforge)
```

### Master Physical Keyring Spawner

```lua
.additem 16309 1           -- Drakefire Amulet (Onyxia Lair Key)
.additem 22754 1           -- Eternal Quintessence (Molten Core Infinite Rune Douse)
.additem 12344 1           -- Seal of Ascension (Upper Blackrock Spire 10-Man Gate Key)
.additem 7146 1            -- Key to the City (Stratholme Service Entrance Gate)
.additem 18249 1           -- Crescent Key (Dire Maul West & North Gates)
.additem 11000 1           -- Shadowforge Key (Blackrock Depths Gates & Mechanism)
.additem 13704 1           -- Skeleton Key (Scholomance Front Gate)
.additem 6893 1            -- Workshop Key (Gnomeregan Back Entrance)
.additem 5396 1            -- Scarlet Key (Scarlet Monastery Armory & Cathedral)
```

---

## 3. Complete Turtle-WoW Custom Instance Directory

All exclusive custom 5-man dungeons and 10/20/40-man raids added in Turtle-WoW 1.18.1:

### Custom Turtle-WoW Dungeons (5-Man)

| Destination Token | Dungeon Name | Level Range | Map ID | Location & Theme |
| :--- | :--- | :---: | :---: | :--- |
| `.tele crescentgrove` | Crescent Grove | 32–38 | **805** | Ashenvale (Night Elf Barrow Den) |
| `.tele hateforgeinstance` | Hateforge Quarry | 50–60 | **806** | Burning Steppes (Dark Iron Mining Facility) |
| `.tele gilneascity` | Gilneas City | 42–48 | **807** | Gilneas (Gothic Plagued City Ruins) |
| `.tele alahthalas` | Alah'Thalas | 55–60 | **808** | Quel'Thalas (High Elf Arcane Sanctum) |
| `.tele stormwindvault` | Stormwind Vault | 60 | **810** | Stormwind Canal (Subterranean Prison) |
| `.tele karazhancrypt` | Karazhan Crypts | 58–60 | **813** | Deadwind Pass (Forgotten Morgan's Plot Tombs) |

### Custom Turtle-WoW Raids (10-Man / 20-Man / 40-Man)

| Destination Token | Raid Name | Size | Map ID | Location & Theme |
| :--- | :--- | :---: | :---: | :--- |
| `.tele ScarletCitadel` | Scarlet Citadel | 10-Man | **811** | Tirisfal Glades (High Inquisitor's Bastion) |
| `.tele Karazhan` | Tower of Karazhan | 10-Man | **812** | Deadwind Pass (Medivh's Upper Quarters) |
| `.tele TimbermawHold` | Timbermaw Hold | 20-Man | **815** | Felwood (Uncorrupted Furbolg Sacred Burrow) |
| `.tele sanctumraid` | Emerald Sanctum | 40-Man | **814** | Mount Hyjal (Corrupted Emerald Dream Portal) |

---

## 4. Master Instance Teleport Directory (Classic & Custom)

Complete destination tokens for every dungeon and raid across Azeroth:

### Eastern Kingdoms Dungeons

- **Blackrock Depths**: `.tele brd`
- **Lower Blackrock Spire**: `.tele lbrs`
- **Upper Blackrock Spire (10-Man)**: `.tele ubrs`
- **Scholomance**: `.tele scholo`
- **Stratholme (Main Gate)**: `.tele strat`
- **Stratholme (Service Gate)**: `.tele stratservice`
- **Shadowfang Keep**: `.tele sfk`
- **Scarlet Monastery (All Wings)**: `.tele sm`
- **The Deadmines**: `.tele deadmines`
- **The Stockade**: `.tele stockades`
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
- **Zul'Farrak**: `.tele zf`
- **Razorfen Downs**: `.tele rfd`
- **Razorfen Kraul**: `.tele rfk`
- **Wailing Caverns**: `.tele wc`
- **Blackfathom Deeps**: `.tele bfd`
- **Ragefire Chasm**: `.tele rfc`
- **Crescent Grove (Turtle Custom)**: `.tele crescentgrove`
- **Hateforge Quarry (Turtle Custom)**: `.tele hateforgeinstance`

### Classic & Custom Raids

- **Molten Core (40-Man)**: `.tele molten`
- **Blackwing Lair (40-Man)**: `.tele bwl`
- **Onyxia's Lair (40-Man)**: `.tele ony`
- **Zul'Gurub (20-Man)**: `.tele zg`
- **Ruins of Ahn'Qiraj (20-Man)**: `.tele aq20`
- **Temple of Ahn'Qiraj (40-Man)**: `.tele aq40`
- **Naxxramas (40-Man)**: `.tele naxxramas`
- **Scarlet Citadel (Turtle Custom 10-Man)**: `.tele ScarletCitadel`
- **Tower of Karazhan (Turtle Custom 10-Man)**: `.tele Karazhan`
- **Timbermaw Hold (Turtle Custom 20-Man)**: `.tele TimbermawHold`
- **Emerald Sanctum (Turtle Custom 40-Man)**: `.tele sanctumraid`

### World Bosses & Dragons of Nightmare

| World Boss | Teleport Token / Location | Zone & Continent |
| :--- | :--- | :--- |
| **Lord Kazzak** | `.tele kazzak` | Tainted Scar, Blasted Lands (EK) |
| **Azuregos** | `.tele azuregos` | Ruins of Eldarath, Azshara (Kalimdor) |
| **Ysondre** | `.tele boughshadow` | Bough Shadow, Ashenvale (Kalimdor) |
| **Lethon** | `.tele seradane` | Seradane, The Hinterlands (EK) |
| **Emeriss** | `.tele dreambough` | Dream Bough, Feralas (Kalimdor) |
| **Taerar** | `.tele twilightgrove` | Twilight Grove, Duskwood (EK) |

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
  - Booty Bay (Stranglethorn): `.tele bootbay`
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
- **Stranglethorn Vale**: `.tele stv`
- **Tirisfal Glades**: `.tele tirisfal`
- **Silverpine Forest**: `.tele silverpine`
- **Western Plaguelands**: `.tele wpl`
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
- **Un'Goro Crater**: `.tele ungoro`
- **Silithus**: `.tele silithus`
- **Felwood**: `.tele felwood`
- **Winterspring**: `.tele winterspring`
- **Azshara**: `.tele azshara`

### Turtle-WoW Custom World Zones & Islands

- **Alah'Thalas (High Elf Haven)**: `.tele alahthalas`
- **Gillijim's Isle**: `.tele gillijim`
- **Island of Lapidis**: `.tele lapidis`
- **Mount Hyjal (Open-World Sanctuary)**: `.tele hyjal`
- **Tel'Abim Island**: `.tele telabim`
- **Grimtusk Outpost**: `.tele grimtusk`

---

## 6. Movement, Spatial Navigation & Summoning Commands

Verified against CMaNGOS core command handlers (`src/game/Chat/Chat.cpp`):

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.gps` | 1 | `.gps [target]` | Dumps Map ID, Area ID, Zone ID, 3D coordinates, and orientation. | `.gps` |
| `.go xyz` | 1 | `.go xyz <x> <y> <z> [map_id] [o]` | Teleports directly to exact 3D coordinates (map: 0=EK, 1=Kalimdor). | `.go xyz -8949 -132 83 0` |
| `.go xy` | 1 | `.go xy <x> <y> [map_id]` | Teleports to X, Y coordinates placed safely on terrain ground level. | `.go xy 1629 -4415 1` |
| `.go forward` | 1 | `.go forward [yards]` | Blinks forward through world geometry in facing direction (default 5yd). | `.go forward 20` |
| `.go up` | 1 | `.go up [yards]` | Teleports straight up along vertical Z axis. | `.go up 15` |
| `.go target` | 1 | `.go target` | Teleports directly on top of currently selected unit or player. | `.go target` |
| `.go corpse` | 3 | `.go corpse` | Teleports directly to player ghost dead body coordinates. | `.go corpse` |
| `.go creature` | 1 | `.go creature <guid>` | Teleports to NPC spawn location matching database GUID. | `.go creature 10182` |
| `.go object` | 1 | `.go object <guid>` | Teleports to static GameObject matching database GUID. | `.go object 179501` |
| `.go taxinode` | 1 | `.go taxinode <id>` | Teleports directly to taxi flight node ID. | `.go taxinode 2` |
| `.go trigger` | 1 | `.go trigger <id>` | Teleports to area trigger ID entrance. | `.go trigger 120` |
| `.go graveyard` | 1 | `.go graveyard <id>` | Teleports directly to graveyard ID coordinates. | `.go graveyard 4` |
| `.tele` | 1 | `.tele <dest_token>` | Teleports character to pre-defined location token. | `.tele stormwind` |
| `.tele group` | 3 | `.tele group <dest_token>` | Teleports character and all party/raid bots simultaneously. | `.tele group molten` |
| `.tele name` | 3 | `.tele name <player> <dest>` | Teleports specified player directly to destination token. | `.tele name Sam naxxramas` |
| `.tele add` | 3 | `.tele add <dest_token>` | Saves current player coordinates as new permanent `.tele` token in DB. | `.tele add mybase` |
| `.tele del` | 3 | `.tele del <dest_token>` | Deletes `.tele` token from database. | `.tele del mybase` |
| `.appear` | 1 | `.appear <player>` | Teleports you directly to specified player current location. | `.appear Alice` |
| `.summon` | 2 | `.summon <player>` | Teleports player or bot to your current location. | `.summon Bob` |
| `.groupsummon` | 2 | `.groupsummon <player>` | Teleports player and their entire group to your location. | `.groupsummon Bob` |
| `.recall` | 3 | `.recall [player]` | Returns player to coordinates occupied before last teleport. | `.recall` |
| `.hover` | 4 | `.hover <on\|off>` | Enables free noclip style levitation and flight. | `.hover on` |
| `.taxicheat` | 3 | `.taxicheat <on\|off>` | Unlocks all flight paths globally. | `.taxicheat on` |
| `.explorecheat` | 3 | `.explorecheat <on\|off>` | Uncovers all world map fog across all continents. | `.explorecheat on` |
| `.unstuck` | 0 | `.unstuck` | Casts emergency return to bound hearthstone innkeeper. | `.unstuck` |

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md) • [🤖 RandomBot Guide (rndbot_commands.md)](../rndbot_commands.md)
