# 02. Playerbot & Raid Automation Handbook (Master Control Center)

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + PlayerBots Framework (ike3 / r-o-sh) + mod-dungeon-clear
> **Source Verification**: `modules/mod-playerbots/src/playerbot/`, `ChatTriggerContext.h`, `ChatActionContext.h`, `Formations.cpp`, `Stances.cpp`, `ChatFilter.cpp`, `modules/mod-dungeon-clear/`
> **Security Coverage**: Accessible to all players (`SEC_PLAYER` / Level 0) with elevated GM commands where noted
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md) • [🤖 RandomBot Guide (rndbot_commands.md)](../rndbot_commands.md)

---

## 📑 Table of Contents

1. [Unified 5-Volume Master Reference Architecture](#unified-5-volume-master-reference-architecture)
2. [Master Macro UI Cheat Sheet (Copy-Paste Ready)](#1-master-macro-ui-cheat-sheet-copy-paste-ready)
   - [Target Tokens & Scope Syntax](#target-tokens--scope-syntax)
   - [Category A: Group Assembly, Gearing & Preparation Macros](#category-a-group-assembly-gearing--preparation-macros)
   - [Category B: Live Group & Raid Combat Controls](#category-b-live-group--raid-combat-controls)
   - [Category C: Autonomous Dungeon Clear AI Deck (.dc)](#category-c-autonomous-dungeon-clear-ai-deck-dc)
   - [Category D: Post-Combat Recovery, Looting & Breaches](#category-d-post-combat-recovery-looting--breaches)
3. [Playerbot Core Control Suite (.bot)](#2-playerbot-core-control-suite-bot)
   - [Holder & Global Session Handlers](#holder--global-session-handlers)
   - [Targeted Bot Commands & Gearing Operations](#targeted-bot-commands--gearing-operations)
   - [Live Configuration Tweaks (.bot tweak)](#live-configuration-tweaks-bot-tweak)
4. [RandomBot Population & Realm AI Manager (.rndbot)](#3-randombot-population--realm-ai-manager-rndbot)
   - [Realm Telemetry & Population Health](#realm-telemetry--population-health)
   - [World Bot Dispatch & RPG Simulation](#world-bot-dispatch--rpg-simulation)
   - [Memory, Tick Rate & PID Controller Tuning](#memory-tick-rate--pid-controller-tuning)
5. [In-Game Whispers & Single-Letter Shortcuts Dictionary](#4-in-game-whispers--single-letter-shortcuts-dictionary)
   - [Fast Single-Letter Shortcuts](#fast-single-letter-shortcuts)
   - [Complete Whisper Command Directory](#complete-whisper-command-directory)
   - [Management, AI State & Custom Macros](#management-ai-state--custom-macros)
6. [Chat Filter Syntax & Targeting Token Engine (@filter)](#5-chat-filter-syntax--targeting-token-engine-filter)
   - [Role, Combat Type & Specialization Filters](#role-combat-type--specialization-filters)
   - [Raid Target Icon (RTI) Filters](#raid-target-icon-rti-filters)
   - [Class, Race, Level & Gear Score Filters](#class-race-level--gear-score-filters)
   - [Group, Raid, Guild & State Filters](#group-raid-guild--state-filters)
   - [Strategy Condition Filters (@co=, @nc=)](#strategy-condition-filters-co-nc)
7. [Formations & Stances Tactical Directory](#6-formations--stances-tactical-directory)
   - [Formations Directory (Formations.cpp)](#formations-directory-formationscpp)
   - [Stances Directory (Stances.cpp)](#stances-directory-stancescpp)
8. [Autonomous Dungeon Clear AI Engine (mod-dungeon-clear)](#7-autonomous-dungeon-clear-ai-engine-mod-dungeon-clear)
   - [In-Dungeon Tank Autopilot (.dc)](#in-dungeon-tank-autopilot-dc)
   - [Pull Modes: Dynamic vs Leeroy vs Advanced](#pull-modes-dynamic-vs-leeroy-vs-advanced)
   - [Scripted Dungeon Events & Navmesh Break Handlers](#scripted-dungeon-events--navmesh-break-handlers)
   - [Live Configuration Inspection (.dc config)](#live-configuration-inspection-dc-config)
   - [Companion Addon (1.12 Classic Port)](#companion-addon-112-classic-port)
   - [Spectator Camera Controls (.dc spectate)](#spectator-camera-controls-dc-spectate)
   - [Headless Automated 5-Bot Test Harness (.dc test)](#headless-automated-5-bot-test-harness-dc-test)
9. [Class Combat Strategy Assignment Deck (/w %t)](#8-class-combat-strategy-assignment-deck-w-t)
   - [Tank Specializations (Warrior, Paladin, Feral Druid)](#tank-specializations)
   - [Healer Specializations (Priest, Shaman, Druid, Paladin)](#healer-specializations)
   - [Melee DPS Specializations (Rogue, Warrior, Paladin, Shaman, Druid)](#melee-dps-specializations)
   - [Ranged & Caster DPS Specializations (Hunter, Mage, Warlock, Priest, Druid)](#ranged--caster-dps-specializations)
10. [Raid Assembly, Attunement & Instance Reset Blueprint](#9-raid-assembly-attunement--instance-reset-blueprint)
    - [10-Man, 20-Man & 40-Man Raid Compositions](#10-man-20-man--40-man-raid-compositions)
    - [Instance Lockouts, Unbinding & Raid Resets](#instance-lockouts-unbinding--raid-resets)

---

## Unified 5-Volume Master Reference Architecture

| Volume | Handbook Title | Domain & Core Coverage | Direct Link |
| :---: | :--- | :--- | :---: |
| **01** | **[Game Master & World Administration](./01_game_master_handbook.md)** | **100% of GM commands**: Self progression/cheats (`.god`, `.fly`, `.levelup 60`), accounts, security (0–6), spawning (`.npc`, `.gobject`), economy, moderation, 106+ `.reload` tables & diagnostics. | [Open Volume 01](./01_game_master_handbook.md) |
| **02** | **Playerbot & Raid Automation** | **Complete Bot Control Center**: `.bot` & `.rndbot` suites, single-letter whisper shortcuts (`c`, `u`, `e`, `t`, `s`, `r`), chat filters (`@tank`, `@skull`), strategy tokens (`co`/`nc`), `.dc` autonomous dungeon clear solver & raid macros. | **Current File** |
| **03** | **[World, Dungeons & Teleports Directory](./03_turtle_world_and_teleports.md)** | **647+ .tele tokens**: Kalimdor, Eastern Kingdoms, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements. | [Open Volume 03](./03_turtle_world_and_teleports.md) |
| **04** | **[Class Specs & Endgame Gear Directory](./04_class_specs_and_endgame_gear.md)** | **All 9 Classes**: 1-Click BiS gear loadout macros, complete Tier 1/2/3 set IDs, weapons, trinkets, enchants & verified 1.18.1 talent builds across 30 distinct specializations. | [Open Volume 04](./04_class_specs_and_endgame_gear.md) |
| **05** | **[Eluna Scripting & Core Development Engine](./05_developer_eluna_and_engine.md)** | **Eluna Lua API**: Live reloads (`.reload eluna`), Event 42 Custom Command dispatch, Turtle-WoW `SpellInfo` compatibility layer, and production server hook mechanics. | [Open Volume 05](./05_developer_eluna_and_engine.md) |

---

## 1. Master Macro UI Cheat Sheet (Copy-Paste Ready)

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`).

### Target Tokens & Scope Syntax

- **`%t`**: Current Target. In a macro, `/w %t co +tank` resolves dynamically to the character or bot you currently have targeted.
- **`*`**: All Party Bots. `.bot gear * best` applies instantly to every bot in your active party simultaneously.
- **`/p @all`**: Party Broadcast. Sends orders to every bot in your party simultaneously.
- **`/p @<role>`**: Role Filter. Sends instructions to specific roles (e.g. `/p @tank attack`, `/p @heal drink`, `/p @melee behind`).
- **`/p @<rti>`**: Raid Target Icon Filter. Directs commands to marks (e.g. `/p @skull attack`).

---

### Category A: Group Assembly, Gearing & Preparation Macros

#### 🏃 Macro 1: 1-Click Bot Party Summon & Follow Lock

Instantly teleports all party bots to your exact coordinates, resets pathing, and locks follow formation:

```lua
.bot summon *
/p @all follow
/p @all stay off
/p @all flee off
```

#### 💎 Macro 2: 1-Click Bot Level 60 Epics, Enchants & Stocking

Equips all party bots with level-scaled epic armor/weapons, optimal permanent enchants, food, water, potions, and class reagents:

```lua
.bot level * 60
.bot gear * best
.bot enchants *
.bot train *
.bot prep *
.bot food *
.bot potions *
.bot regs *
.bot ammo *
```

#### 👥 Macro 3: Auto-Assemble Balanced 5-Man Group

Creates a balanced 5-man party (1 Tank, 1 Healer, 3 DPS) matching your level:

```lua
.bot group size=5
.bot summon *
.bot prep *
```

#### 📋 Macro 4: Party / Raid Ready Check

Queries all bots in party for health, mana, consumable stock, and readiness before pulling:

```lua
/p @all ready check
/p @all c ?
```

---

### Category B: Live Group & Raid Combat Controls

#### 🛡️ Macro 5: Main Tank Pull & Hold Aggro

Orders the party tank bot to immediately charge/ranged-pull your current target and establish maximum threat:

```lua
/p @all tank attack
```

#### ⚔️ Macro 6: All DPS Engage Target (Maximum Burst)

Orders all damage dealers and pets to open with maximum cooldowns and burst damage on your current target:

```lua
/p @all attack
/p @all max dps
```

#### 🏹 Macro 7: Controlled Ranged Pull to Master

Orders ranged bots to shoot the target and retreat back to master position to safely group trash packs around a corner:

```lua
/p @all pull
/p @all follow
```

#### 💀 Macro 8: Focus Fire Raid Target Icon (Skull)

Forces all party damage dealers to immediately switch target and burn down the mob marked with the Skull icon:

```lua
/p @skull attack
/p @all attack rti 8
```

#### 🚨 Macro 9: Emergency Panic Disengage & Flee to Master

Instantly breaks combat, drops bot spellcasting, and commands all bots to sprint back to master:

```lua
/p @all flee
/p @all follow
.combatstop
```

#### 💥 Macro 10: Area-of-Effect Burst Mode (AoE Trash)

Toggles high-damage Area-of-Effect abilities (Blizzard, Rain of Fire, Volley, Whirlwind, Consecration):

```lua
/p @all co +aoe
```

#### 🛑 Macro 11: Prioritize Interrupts & Counterspells

Instructs all bots with interrupts (Kick, Pummel, Earth Shock, Counterspell) to save resources and silence casting mobs:

```lua
/p @all co +interrupt
```

#### 🧹 Macro 12: Mass Dispel & Cleanse Debuffs

Prioritizes healers and hybrid casters curing magic debuffs, curses, poisons, and diseases:

```lua
/p @all co +cure,+cleanse
```

---

### Category C: Autonomous Dungeon Clear AI Deck (`.dc`)

#### 🏰 Macro 13: Activate Autonomous Dungeon Solver

Turns on the autonomous dungeon clear engine on party tank bot, routing through optimal boss paths:

```lua
.dc on
/p @all co +dungeon clear,avoid aoe,roll,loot,potions
/p @all nc +gather,food
/p @all formation near
```

#### 🔄 Macro 14: Cycle Pull Mode (Dynamic / Leeroy / Advanced)

Cycles the tank bot's trash pulling behavior:

```lua
.dc pull
```

#### ⏸️ Macro 15: Pause / Resume Autonomous Run

Pauses tank bot pathing and combat advance (e.g. before mana breaks, patrols, or AFK):

```lua
.dc pause
```

#### 🎯 Macro 16: Order Instant Tank Pull on Next Pack

Forces the tank bot to immediately pull the next calculated mob pack in route without waiting:

```lua
.dc pull
```

#### ⏩ Macro 17: Skip Current Trash Pack or Boss

Commands autonomous solver to recalculate dungeon navigation path around difficult trash or optional bosses:

```lua
.dc skip
```

#### 📊 Macro 18: Instance Progression & Live Config Audit

Displays live dungeon completion percentage, dead/alive bosses, and dumps active config:

```lua
.dc status
.dc bosses
.dc config
```

---

### Category D: Post-Combat Recovery, Looting & Breaches

#### 💰 Macro 19: Fast Post-Boss Loot, Drink & Rebuff

Orders all bots to vacuum corpse loot, consume food/drink to 100% mana, and refresh 30-minute buffs:

```lua
/p @all loot
/p @all food
/p @all drink
/p @all buff
```

#### 🚪 Macro 20: Breach & Interact with Nearest Door / Lever / Gate

Commands closest bot to right-click interact with nearest GameObject (gates, portcullises, chests, lever switches):

```lua
/p @all u
```

#### 🛠️ Macro 21: Order Bots to Repair Gear & Sell Junk at Vendor

Orders all party bots to sell gray trash items and repair equipped gear at targeted vendor:

```lua
/p @all s
/p @all r
```

#### 🤝 Macro 22: Trade & Force Equip Best Gear on Bot

Opens trade window with targeted bot and orders bot to evaluate inventory and equip highest stat upgrades:

```lua
/w %t t
/w %t e
```

---

## 2. Playerbot Core Control Suite (`.bot`)

The `.bot` command suite provides direct programmatic control over party bots, account alternate characters, and character templates.

### Holder & Global Session Handlers

These commands operate at the holder/session level (`PlayerbotHolder.cpp`):

| Command | Syntax & Arguments | Description | Example |
| :--- | :--- | :--- | :--- |
| `.bot list` | `.bot list [filter]` | Dumps list of all active bots in world memory with online status, class, and level. | `.bot list Priest` |
| `.bot help` | `.bot help [command]` | Displays built-in help documentation for playerbot commands. | `.bot help gear` |
| `.bot reload` | `.bot reload` | Reloads `aiplayerbot.conf` settings in-memory without server reboot. | `.bot reload` |
| `.bot tweak` | `.bot tweak <var> [val]` | Inspects or updates an active configuration parameter directly in memory. | `.bot tweak sightDistance 30.0` |
| `.bot self` | `.bot self [login]` | Inspects or toggles self-bot mode on your own character or account alts. | `.bot self login` |
| `.bot spoof` | `.bot spoof <botname>` | Spoofs chat/console actions so commands execute as if the bot typed them. | `.bot spoof Tankman` |
| `.bot group` | `.bot group [size=N] [level=N]` | **Auto-Party Generator**: Assembles a balanced party (Tank, Healer, DPS) matching level. | `.bot group size=5 level=60` |
| `.bot create` | `.bot create <key=val...>` | Generates a new bot character with explicit attributes: `name=`, `race=`, `class=`, `level=`, `role=`, `gear=`, `group=`, `login=1`, `temporary=1`. | `.bot create name=Dunpriest class=priest race=dwarf level=60 role=healer login=1` |
| `.bot p` | `.bot p <msg>` | Broadcasts a party message/command to all bots in your party. | `.bot p follow` |
| `.bot g` | `.bot g <msg>` | Broadcasts a guild message/command to all bots in your guild. | `.bot g hello guild` |
| `.bot r` | `.bot r <msg>` | Broadcasts a raid message/command to all bots in your raid group. | `.bot r ready check` |
| `.bot rl` | `.bot rl <msg>` | Broadcasts a raid leader warning to all raid bots. | `.bot rl Focus Skull!` |
| `.bot runtest` | `.bot runtest <action>` | Executes an automated test action on the bot harness *(when enabled)*. | `.bot runtest combat` |

### Targeted Bot Commands & Gearing Operations

These commands target individual bots, or `*` for all bots in your party (`PlayerbotHolder.cpp`):

| Command | Aliases | Syntax & Arguments | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `.bot add` | `login` | `.bot add <name\|*>` | Spawns and logs in specified bot or all account alternate characters. | `.bot add *` |
| `.bot remove` | `logout`, `rm` | `.bot remove <name\|*>` | Safely saves and despawns specified bot or all active party bots. | `.bot remove *` |
| `.bot delete` | - | `.bot delete <name>` | Permanently purges bot character from `tw_char` database (`Player::DeleteFromDB`). | `.bot delete Stuckbot` |
| `.bot summon` | `recall`, `come` | `.bot summon <name\|*>` | Teleports party bots instantly to master coordinates and syncs instance map. | `.bot summon *` |
| `.bot level` | `levelup` | `.bot level <name\|*> <1–60>` | Sets character level of specified bot or all party bots. | `.bot level * 60` |
| `.bot gear` | `equip` | `.bot gear <name\|*> [quality]` | Generates and equips level-scaled gear: `poor`, `common`, `uncommon`, `rare`, `epic`, `best`, `upgrade`, `sync`, `partial`. | `.bot gear * best` |
| `.bot enchants` | - | `.bot enchants <name\|*>` | Applies optimal permanent enchantments to all equipped armor and weapons. | `.bot enchants *` |
| `.bot train` | `learn` | `.bot train <name\|*>` | Teaches all class spells, ranks, and weapon proficiencies up to current level. | `.bot train *` |
| `.bot prep` | `prepare`, `init` | `.bot prep <name\|*> [quality]` | **Master 1-Click Restock**: Repairs gear, learns spells, enchants gear, clears cooldowns, replenishes HP/Mana, stocks consumables. | `.bot prep * epic` |
| `.bot consumes` | `consumables` | `.bot consumes <name\|*>` | Stocks level-appropriate flasks, elixirs, scrolls, bandages, and consumable buffs. | `.bot consumes *` |
| `.bot potions` | `pots` | `.bot potions <name\|*>` | Stocks level-appropriate healing, mana, and utility potions in bot bags. | `.bot potions *` |
| `.bot regs` | `reg`, `reagents` | `.bot regs <name\|*>` | Stocks class reagents (Soul Shards, Ankhs, Feathers, Flash Powder, Holy Candles). | `.bot regs *` |
| `.bot food` | `drink` | `.bot food <name\|*>` | Stocks level-appropriate food and water drinks in inventory. | `.bot food *` |
| `.bot ammo` | - | `.bot ammo <name\|*>` | Generates level-appropriate arrows/bullets and equips best quiver/pouch. | `.bot ammo *` |
| `.bot pet` | - | `.bot pet <name\|*>` | Summons, feeds, and trains combat pet for Hunter or Warlock. | `.bot pet *` |
| `.bot random` | - | `.bot random <name\|*>` | Randomizes bot talent tree, appearance, and non-combat characteristics. | `.bot random *` |
| `.bot always` | - | `.bot always [name\|*]` | Toggles persistence flag so bot remains in the world even if master logs out. | `.bot always Tankman` |
| `.bot debug` | - | `.bot debug [name\|*]` | Dumps internal AI decision tree, current action, target, and strategy state. | `.bot debug Tankman` |
| `.bot c` | - | `.bot c <name\|*> <cmd>` | Executes internal bot chat command directly from console. | `.bot c * c ?` |
| `.bot w` | - | `.bot w <botname> <msg>` | Sends a whisper message directly from console/chat to the specified bot. | `.bot w Dunpriest follow` |
| `.bot cmd` | - | `.bot cmd <name\|*> <action>` | Issues a direct action command to the bot's AI queue. | `.bot cmd Tankman attack` |
| `.bot do` | - | `.bot do <name\|*> <action>` | Forces bot to immediately execute a specific C++ action from `ChatActionContext`. | `.bot do Tankman tank attack` |
| `.bot record` | - | `.bot record enable\|disable` | Starts or stops recording player actions for bot playback / macro profiling. | `.bot record enable` |
| `.bot read` | - | `.bot read [name\|*]` | Dumps recorded macro action sequence for playback evaluation. | `.bot read Tankman` |
| `.bot clear` | - | `.bot clear [name\|*]` | Clears recorded macro sequence buffer. | `.bot clear Tankman` |

### Live Configuration Tweaks (`.bot tweak`)

You can read or write in-memory AI settings live without editing config files:

- `.bot tweak sightDistance` — Displays active mob detection range (default: 30.0 yards).
- `.bot tweak sightDistance 40.0` — Expands detection range to 40 yards.
- `.bot tweak reactDistance 25.0` — Sets social aggro assist range.
- `.bot tweak followDistance 1.5` — Tightens follow formation distance.

---

## 3. RandomBot Population & Realm AI Manager (`.rndbot`)

The `.rndbot` suite controls autonomous open-world population, server lag tuning, and realm simulation (`RandomPlayerbotMgr.cpp`).

> [!NOTE]
> **Universal Command Inheritance**: Every command supported by `.bot` (such as `prep`, `gear`, `summon`, `group`, `train`) can also be executed via `.rndbot`! In addition, `.rndbot` provides server-wide population handlers.

### Realm Telemetry & Population Health

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.rndbot stats` | 0 | `.rndbot stats` | Spawns a background thread to format complete realm population telemetry: online count, level brackets, race/class breakdown, active roles (Tank/Heal/DPS), questing, and combat status. | `.rndbot stats` |
| `.rndbot diff` | 1 | `.rndbot diff [p_diff e_diff]` | Displays tick latency metrics: `Avg diff`, `Max diff`, Character DB ping, sessions online, and active bots. Pass arguments to tune target diffs. | `.rndbot diff 100 200` |
| `.rndbot pid` | 3 | `.rndbot pid [p i d]` | Displays or tunes the real-time Proportional-Integral-Derivative (PID) dynamic controller that balances bot activity based on server tick load. | `.rndbot pid 0.05 0.001 0.05` |
| `.rndbot update` | 3 | `.rndbot update` | Forces an immediate AI update tick across the entire realm (`UpdateAIInternal(0)`), evaluating zone population, quests, and login cycles. | `.rndbot update` |
| `.rndbot login debug` | 3 | `.rndbot login debug` | Toggles real-time verbose console logging for the bot login queue manager (`sPlayerBotLoginMgr`). | `.rndbot login debug` |
| `.rndbot clean map` | 3 | `.rndbot clean map` | Spawns detached threads to unload unreferenced map geometry and VMaps (`unloadMapAndVMaps`) across all maps, freeing memory. | `.rndbot clean map` |
| `.rndbot reset` | 3 | `.rndbot reset` | Wipes persistent random bot records from database (`DELETE FROM ai_playerbot_random_bots WHERE event != 'temporary'`). | `.rndbot reset` |

### World Bot Dispatch & RPG Simulation

| Command | Scope Selector | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.rndbot init` | `name\|%\|guild` | `.rndbot init [name\|%]` | Re-initializes random bots, rerolling specs, gear brackets, and faction quotas. | `.rndbot init %` |
| `.rndbot upgrade` | `name\|%\|guild` | `.rndbot upgrade [name\|%]` | Scans bots and upgrades equipment and spells matching current level brackets. | `.rndbot upgrade %` |
| `.rndbot refresh` | `name\|%\|guild` | `.rndbot refresh [name\|*]` | Soft-reboots bots, clearing combat deadlocks, pathing loops, and broken AI states. | `.rndbot refresh *` |
| `.rndbot grind` | `name\|%` | `.rndbot grind [name\|%]` | Orders idle bots to travel to level-appropriate zones and farm mobs. | `.rndbot grind %` |
| `.rndbot rpg` | `name\|%` | `.rndbot rpg [name\|%]` | Dispatches bots to taverns, cities, crafting trainers, and quest hubs for RPG social life. | `.rndbot rpg %` |
| `.rndbot revive` | `name\|%` | `.rndbot revive [name\|*]` | Instantly resurrects dead/ghost bots at full health and mana. | `.rndbot revive *` |
| `.rndbot teleport` | `name\|%` | `.rndbot teleport [name\|%]` | Teleports bot(s) to a randomized level-appropriate zone. | `.rndbot teleport %` |

---

## 4. In-Game Whispers & Single-Letter Shortcuts Dictionary

Direct chat communication with party bots via `/w %t <action>` or `/p @all <action>` (`ChatTriggerContext.h` & `ChatActionContext.h`).

### Fast Single-Letter Shortcuts

> [!IMPORTANT]
> **Ground Truth Correction**: In earlier documentation, shortcut `c` was mislabeled as a combat toggle. In actual C++ source (`ChatTriggerContext.h:36-39`), `c` executes `TellItemCountAction` (inventory inspection). Combat engagement is handled via `attack` or `tank attack`.

| Shortcut | Action Name | Full Trigger Aliases | Description |
| :---: | :--- | :--- | :--- |
| **`c`** | Inventory Inspection | `c`, `items`, `inventory`, `inv` | Reports equipped items, free bag slots, gold count, and durability percentage. |
| **`u`** | Use Object / Item | `u`, `use` | Commands bot to right-click interact with nearest GameObject (lever, door, chest, node) or use designated item. |
| **`e`** | Equip Item | `e`, `equip` | Forces bot to evaluate inventory bags and equip the highest stat upgrades available. |
| **`ue`** | Unequip Item | `ue` | Orders bot to unequip item from equipment slot into inventory bags. |
| **`keep`** | Keep Item | `keep` | Marks item in bot bags to never be auto-vendored or destroyed. |
| **`drop`** | Drop Quest / Item | `drop` | Drops specified quest from quest log or destroys designated item. |
| **`s`** | Sell Gray Junk | `s`, `sell` | Orders bot to vendor all gray-quality trash items at targeted vendor NPC. |
| **`b`** | Buy Vendor Item | `b`, `buy` | Orders bot to purchase designated item from vendor. |
| **`bb`** | Buyback Item | `bb`, `buy back` | Restores accidentally sold items from vendor buyback tab. |
| **`r`** | Repair Durability | `r`, `repair` | Orders bot to repair 100% equipment durability at targeted blacksmith/vendor. |
| **`t`** | Initiate Trade | `t`, `trade` | Opens secure trade transaction window between player and bot. |
| **`nt`** | Cancel Trade | `nt` | Cancels active trade transaction window. |
| **`q`** | Quest Log Audit | `q`, `quests` | Dumps current bot quest log, active objectives, and completion percentages. |
| **`ll`** | Loot Strategy | `ll` | Inspects or sets bot loot filtering strategy (`normal`, `gray`, `disenchant`, `all`). |
| **`ss`** | Skip Spells List | `ss` | Dumps list of blacklisted or skipped spells on bot. |

### Complete Whisper Command Directory

| Whisper | Action Executed | Description | Example Syntax |
| :--- | :--- | :--- | :--- |
| `follow` | `FollowChatShortcutAction` | Orders bot to follow master or designated player. | `/w %t follow` |
| `stay` | `StayChatShortcutAction` | Orders bot to hold ground at current coordinates. | `/w %t stay` |
| `flee` | `FleeChatShortcutAction` | Breaks combat, cancels casting, and sprints back to master. | `/w %t flee` |
| `attack` | `AttackMyTargetAction` | Commands all damage dealers and pets to engage target. | `/w %t attack` |
| `tank attack` | `TankAttackChatShortcutAction` | Orders main tank bot to charge/pull and establish threat. | `/w %t tank attack` |
| `pull` | `PullAction` | Orders ranged bot to shoot target and run back to group. | `/w %t pull` |
| `loot` | `AddAllLootAction` | Orders all nearby bots to vacuum corpse loot. | `/p @all loot` |
| `roll` | `RollAction` | Forces bot to roll on group loot prompt (`need`, `greed`, `pass`). | `/p @all roll need` |
| `food` / `drink` | Rest Consumables | Commands bots to sit and consume food/water until 100%. | `/p @all drink` |
| `buff` | `BuffAction` | Orders bots to cast class buffs on all group members. | `/p @all buff` |
| `save mana` | `SaveManaAction` | Toggles conservative mana conservation behavior. | `/w %t save mana` |
| `max dps` | `MaxDpsChatShortcutAction` | Forces bots to burn all DPS cooldowns and offensive trinkets. | `/p @all max dps` |
| `home` | `SetHomeAction` | Binds bot hearthstone to current innkeeper. | `/w %t home` |
| `accept` | `AcceptAction` | Orders bot to accept pending quest, trade, or resurrection prompt. | `/w %t accept` |
| `release` | `ReleaseSpiritAction` | Commands dead bot to release spirit to graveyard. | `/w %t release` |
| `corpse run` | `CorpseRunAction` | Forces ghost bot to navigate back to corpse and revive. | `/w %t corpse run` |
| `repop` | `RepopAction` | Forces spirit release and immediate respawn at graveyard. | `/w %t repop` |
| `taxi` | `TaxiAction` | Orders bot to board flight path to specified destination node. | `/w %t taxi 45` |
| `avoid creature` | `AvoidCreatureListAction` | Blacklists specific NPC patrol entry ID from bot navigation path. | `/w %t avoid creature 10184` |
| `share` | `ShareQuestAction` | Orders bot to share quest with party members. | `/w %t share [quest_link]` |
| `clean quest log` | `CleanQuestLogAction` | Abandons completed or obsolete quests from bot log. | `/w %t clean quest log` |
| `los` | `TellLosAction` | Queries whether bot has direct line of sight to current target. | `/w %t los` |
| `rtsc` | `RTSCAction` | Real-Time Strategy Control: click-to-move tactical squad mode. | `/w %t rtsc` |
| `glyph` | `GlyphAction` | Equips Turtle-WoW custom class glyph on bot. | `/w %t glyph 12` |
| `outfit` | `OutfitAction` | Saves or equips named equipment loadouts (`save`, `load`). | `/w %t outfit save pvp` |
| `cheat` | `CheatAction` | Toggles bot cheats (infinite mana, taxi cheat, godmode). | `/w %t cheat god` |
| `give leader` | `GiveLeaderAction` | Transfers group leadership to the bot or master. | `/w %t give leader` |

---

## 5. Chat Filter Syntax & Targeting Token Engine (`@filter`)

When issuing orders in party chat (`/p`), raid chat (`/ra`), or guild chat (`/g`), use `@filter` prefixes to route orders with surgical precision (`ChatFilter.cpp`).

### Role, Combat Type & Specialization Filters

| Filter Prefix | Inverse Prefix | Target Audience | Example Usage |
| :--- | :--- | :--- | :--- |
| **`@tank`** | `@notank` | All active tank bots | `/p @tank attack` • `/p @notank flee` |
| **`@heal`** | `@noheal` | All active healer bots | `/p @heal drink` • `/p @heal save mana` |
| **`@dps`** | `@nodps` | All damage dealers (melee & ranged) | `/p @dps attack` • `/p @dps max dps` |
| **`@melee`** | `@nomelee` | All melee combatants (Rogue, Warrior, Paladin, Feral) | `/p @melee formation melee` |
| **`@ranged`** | `@noranged` | All ranged combatants (Hunters) | `/p @ranged formation far` |
| **`@caster`** | `@nocaster` | All spellcasting DPS (Mage, Warlock, Spriest, Boomkin) | `/p @caster formation line` |

### Raid Target Icon (RTI) Filters

Assign priority targets by marking enemies with Raid Target Icons:

| Filter Prefix | RTI Mark | Numeric ID | Example Usage |
| :--- | :---: | :---: | :--- |
| **`@skull`** | 💀 Skull | 8 | `/p @skull attack` (All DPS focus Skull) |
| **`@cross`** | ❌ Cross | 7 | `/p @cross attack` (Secondary priority) |
| **`@square`** | 🟦 Square | 6 | `/p @square cc` (Crowd-control Square) |
| **`@moon`** | 🌙 Moon | 5 | `/p @moon cc` (Sheep / Polymorph Moon) |
| **`@triangle`** | 🔺 Triangle | 4 | `/p @triangle cc` (Trap / Hibernate) |
| **`@diamond`** | 💎 Diamond | 3 | `/p @diamond cc` (Seduce / Fear) |
| **`@circle`** | 🟠 Circle | 2 | `/p @circle attack` |
| **`@star`** | ⭐ Star | 1 | `/p @star attack` |

### Class, Race, Level & Gear Score Filters

- **Class Filters**: `@warrior`, `@paladin`, `@hunter`, `@rogue`, `@priest`, `@shaman`, `@mage`, `@warlock`, `@druid` (and `@no...`).
  - Example: `/p @mage cast blizzard` • `/p @priest cast power word: shield` • `/p @druid cast innervate`
- **Race Filters**: `@human`, `@orc`, `@dwarf`, `@nightelf`, `@undead`, `@tauren`, `@gnome`, `@troll`, `@highelf`, `@goblin`.
- **Level Filters**: `@lvl<X`, `@lvl>X`, `@lvl=X`.
  - Example: `/p @lvl<60 follow` • `/p @lvl=60 prep`
- **Gear Score & Item Level Filters**: `@gs<X`, `@gs>X`, `@ilvl<X`, `@ilvl>X`.
  - Example: `/p @ilvl<60 gear best`

### Group, Raid, Guild & State Filters

- **Group Scope**: `@party` (current party), `@raid` (entire raid), `@leader`, `@noleader`, `@subgroup=<N>` (specific raid group 1–8).
  - Example: `/ra @subgroup=1 formation shield`
- **Combat State**: `@dead`, `@alive`, `@combat` (currently fighting), `@noncombat` (out of combat).
  - Example: `/p @dead release` • `/p @noncombat food`
- **Item Usage Filters**: `@need`, `@greed`, `@vendor`, `@ah`, `@disenchant`, `@use`.

### Strategy Condition Filters (`@co=`, `@nc=`)

Filter bots based on active combat or non-combat strategies:

- `@co=<strat>`: Bots that have `<strat>` enabled in combat (e.g. `/p @co=tank attack`).
- `@noco=<strat>`: Bots that do NOT have `<strat>` enabled in combat.
- `@nc=<strat>`: Bots that have `<strat>` enabled out of combat (e.g. `/p @nc=rpg follow`).
- `@nonc=<strat>`: Bots that do NOT have `<strat>` enabled out of combat.

---

## 6. Formations & Stances Tactical Directory

Manage spatial positioning and combat stances (`Formations.cpp` & `Stances.cpp`).

### Formations Directory (`Formations.cpp`)

Formations determine where bots position themselves relative to master when following:

| Formation Name | Shape & Behavior | Best Suited For |
| :--- | :--- | :--- |
| **`near`** / **`default`** | Tight 2–4 yard cluster around master. | Tight dungeon corridors, doorways, elevator rides. |
| **`queue`** | Single-file line directly behind master. | Narrow bridges, stealth runs, cave paths. |
| **`chaos`** | Randomized dispersion around master. | Open-world travel, anti-AoE spacing. |
| **`circle`** | 360-degree equidistant circular ring surrounding master. | Defending flags, clearing trash waves. |
| **`line`** | Horizontal wall spread perpendicular to master facing. | Ranged caster lines, defensive raid walls. |
| **`shield`** | Tanks/Melee in front, healers and casters in rear row. | Boss encounters, structured dungeon progression. |
| **`arrow`** | Wedge / Spearhead formation with tank at tip. | Open-field pulls, aggressive patrol clearing. |
| **`melee`** | Clustered tightly within 2 yards behind target's back hitbox. | Melee DPS backstabs, preventing parry-haste. |
| **`far`** | Maximum 30–35 yard range circle from target/master. | Hunter and Caster DPS maximum range bracket. |
| **`custom`** | User-defined offsets stored in AI context. | Custom tactical positioning scripts. |

**Formation Commands**:

- `/p @all formation <name>` — Sets active formation across all party bots.
- `/p @all formation show` — Displays active formation name.
- `/p @all formation reset` — Resets formation to `default` (near).
- `/p @all formation ?` — Queries current formation status.

### Stances Directory (`Stances.cpp`)

Stances dictate combat positioning relative to active combat targets:

| Stance Name | Combat Behavior | Best Suited For |
| :--- | :--- | :--- |
| **`near`** / **`default`** | Standard combat range facing target. | General melee combat. |
| **`tank`** | Positions in front of mob and turns boss away from group. | Main Tank and Off-Tank bots. |
| **`turnback`** / **`turn`** | Actively rotates mob 180 degrees away from raid party. | Bosses with frontal cleaves, breath attacks. |
| **`behind`** | Maneuvers continuously behind target's back hitbox. | Rogues, Cat Druids, Fury Warriors. |

**Stance Commands**:

- `/w %t stance <name>` — Sets combat stance on targeted bot.
- `/w %t stance show` — Dumps active stance name.
- `/w %t stance ?` — Queries stance state.

---

## 7. Autonomous Dungeon Clear AI Engine (`mod-dungeon-clear`)

The `mod-dungeon-clear` module provides hands-free autonomous navigation, trash pulling, boss progression, and spectator tools (`DungeonClearCommand.cpp`).

### In-Dungeon Tank Autopilot (`.dc`)

> [!IMPORTANT]
> **Tank Bot Dependency**: The autonomous clear solver operates by driving the **Tank Bot's** AI. The group tank **must be a bot** (or your character in `.bot self` mode). Follower bots automatically assist and protect the tank.

| Command | Chat Keyword | Functionality |
| :--- | :--- | :--- |
| `.dc on` | `dc on` / `dungeon clear on` | Starts autonomous dungeon routing and pulling on group tank bot. |
| `.dc off` | `dc off` / `dungeon clear off` | Disables autonomous solver and returns all bots to following player. |
| `.dc pause` | `dc pause` / `dungeon clear pause` | Soft-pauses tank movement and pulls; resume with the same command. |
| `.dc pull` | `dc pull` / `dungeon clear pull` | **Cycles Pull Mode**: Dynamically toggles between Dynamic, Leeroy, and Advanced. |
| `.dc skip` | `dc skip` | Skips current objective/pack and recalculates route if stalled. |
| `.dc status` | `dc status` | Dumps current run status: duration, boss count, alive/dead state, party readiness. |
| `.dc bosses` | `dc bosses` | Lists all bosses in current instance and their kill bitmask. |
| `.dc go <boss>` | - | Overrides normal route to path directly to designated boss name. |
| `.dc config` | - | Dumps all live `DungeonClear.*` configuration values, overrides, and heroic defaults. |

### Pull Modes: Dynamic vs Leeroy vs Advanced

Choose how the tank bot approaches trash packs:

| Pull Mode | Tactical Behavior | Clear Speed | Risk Profile | Best Content |
| :--- | :--- | :---: | :---: | :--- |
| **Dynamic** *(Default)* | Evaluates pack density: Leeroys lone mobs; executes camp-pull on clustered/dangerous packs. | Balanced | Moderate | **Recommended for 90% of dungeons.** |
| **Leeroy** | Charges straight into every pack and engages in place without line-of-sight pulling. | Maximum | High | Farm content where party out-gears dungeon. |
| **Advanced** | Establishes a safe camp spot and ranged-pulls every pack back around corners. | Methodical | Minimal | Undergeared runs, custom Turtle instances, raids. |

**Dynamic Pull Tuning**: Controlled via `DungeonClear.PullDynamicMaxLeeroyMobs` (default: 3). If pack size exceeds this threshold, the tank bot automatically switches to camp-pull.

### Scripted Dungeon Events & Navmesh Break Handlers

`mod-dungeon-clear` automatically handles instance progression gates without human interaction:

- **Deadmines**: Operates Defias Cannon, blows door, navigates iron portcullises.
- **Shadowfang Keep**: Unlocks courtyard doors, operates prison cell gates.
- **Wailing Caverns**: Awakens Disciple of Naralex, escorts, triggers Mutanus the Devourer.
- **Blackrock Depths**: Solves Shadowforge Lock, Grim Guzzler bar brawl, Lyceum torch run, Ring of Law.
- **Stratholme**: Opens service entrance gates, operates ziggurat crystals, triggers Baroness / Rivendare gauntlet.
- **Zul'Farrak**: Solves Pyramid staircase wave defense, frees Sergeant Bly, opens Antu'sul gong.
- **Dire Maul**: Destroys highborne pylons, frees Gordok prisoners, acquires King of the Gordok buff.

### Live Configuration Inspection (`.dc config`)

Execute `.dc config` in-game to view all active module variables:

- `DungeonClear.LootQualityFloor`: Minimum item quality to stop and loot (default: 2 = Uncommon/Green).
- `DungeonClear.RestHealthPercent`: Health threshold to trigger eat breaks (default: 65%).
- `DungeonClear.RestManaPercent`: Mana threshold to trigger drink breaks (default: 50%).
- `DungeonClear.PostCombatRez`: Toggles resurrection recovery before continuing (default: 1).
- `DungeonClear.PullDynamicMaxLeeroyMobs`: Mob ceiling for Leeroy vs Camp pull (default: 3).
- Overridden settings display with an asterisk (`*`); heroic defaults display with an `H`.

### Companion Addon (1.12 Classic Port)

The module ships with an official Turtle-WoW 1.12 companion addon located in:
`modules/mod-dungeon-clear/addon/DungeonClear-1.12/`

- Provides an on-screen movable UI dashboard with **On**, **Off**, **Skip**, **Pause**, and **Pull Mode** buttons.
- Real-time status readout, boss kill tracker, and live settings configuration panel.
- Fully compatible with 1.12 Lua 5.0 client limitations (resolved all Lua 5.1 `%` regex issues).

### Spectator Camera Controls (`.dc spectate`)

Free-flying spectator camera controls for inspecting bot runs without drawing mob aggro:

```lua
.dc spectate                           -- Toggles free-flying spectator camera
.dc spectate follow <botname>          -- Rides camera smoothly on specified bot
.dc spectate next                      -- Cycles camera to next party bot (alias: .dc spectate n)
.dc spectate prev                      -- Cycles camera to previous party bot (alias: .dc spectate p)
.dc spectate list                      -- Lists all watchable bots in instance (alias: .dc spectate who)
```

### Headless Automated 5-Bot Test Harness (`.dc test`)

A GM-only test engine that rolls a random 5-bot party, equips it, teleports it to a dungeon, and tests autonomous clearance:

```lua
-- Directory & Gear Ceilings
.dc test list                          -- Lists all supported test dungeons and level brackets
.dc test gear <dungeon> [heroic]       -- Displays recommended item level (ilvl) ladders

-- Single Test Run Execution
.dc test start <dungeon> [heroic]      -- Starts autonomous 5-bot test run with random party comp
.dc test start <dungeon> party=T,H,D1,D2,D3 -- Starts test run with specific named bot roster
.dc test start <dungeon> seed=12345    -- Replays exact test run with identical random seed
.dc test status                        -- Displays live duration, boss kills, wipe counter
.dc test stop [selector|all]           -- Aborts active test run or all campaigns

-- GM Spectator Watch Mode
.dc test watch [selector]              -- Enters GM invisible mode, teleports to instance, follows tank
.dc test watch next                    -- Tours next active test run in batch campaign without leaving
.dc test watch off                     -- Stops camera, restores visibility, teleports back to origin

-- Batched Campaigns (.dc test plan)
.dc test plan start <spec>             -- Launches automated N-run test campaign
.dc test plan status                   -- Displays batch campaign pass/fail rates and wipe statistics
.dc test plan stop [planId|all]        -- Halts running test plans
```

---

## 8. Class Combat Strategy Assignment Deck (`/w %t`)

Target a bot (`%t`) and click these macros to set their permanent combat strategies (`co`) and non-combat strategies (`nc`):

### Tank Specializations

- **Protection Warrior (Main Tank)**:

  ```lua
  /w %t co -dps,-fury,-arms,+tank,+prot,+threat,+potions,+avoid aoe
  /w %t nc +food
  /w %t formation near
  /w %t stance tank
  ```

- **Protection Paladin (AoE & Undead Tank)**:

  ```lua
  /w %t co -dps,-heal,-holy,-retrib,+tank,+prot,+consecration,+threat,+bop,+potions
  /w %t nc +food,+buff
  /w %t formation near
  /w %t stance tank
  ```

- **Feral Bear Druid (Armor & HP Tank)**:

  ```lua
  /w %t co -dps,-cat,-caster,-heal,+tank,+bear,+threat,+avoid aoe,+potions
  /w %t nc +food,+buff
  /w %t formation near
  /w %t stance tank
  ```

### Healer Specializations

- **Holy Priest (Raid & Tank Healer)**:

  ```lua
  /w %t co -dps,-shadow,+heal,+holy,+cure,+save mana,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Restoration Shaman (Chain Healer & Totems)**:

  ```lua
  /w %t co -dps,-elemental,-enhancement,+heal,+totems,+mana spring,+healing stream,+cure
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Restoration Druid (HoT & Tank Support)**:

  ```lua
  /w %t co -dps,-cat,-bear,-caster,+heal,+cure,+rejuv,+save mana
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Holy Paladin (Single-Target Tank Healer)**:

  ```lua
  /w %t co -dps,-prot,-retrib,+heal,+holy,+bof,+bos,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

### Melee DPS Specializations

- **Combat Rogue (Behind Flank DPS)**:

  ```lua
  /w %t co -tank,+dps,+behind,+interrupt,+stealth,+evasion,+potions
  /w %t nc +food,+stealth
  /w %t formation melee
  /w %t stance behind
  ```

- **Fury Warrior (Dual-Wield Melee DPS)**:

  ```lua
  /w %t co -tank,-prot,+dps,+fury,+behind,+interrupt,+potions
  /w %t nc +food
  /w %t formation melee
  /w %t stance behind
  ```

- **Retribution Paladin (2-Handed Melee DPS)**:

  ```lua
  /w %t co -tank,-heal,+dps,+retrib,+consecration,+potions
  /w %t nc +food,+buff
  /w %t formation melee
  ```

- **Enhancement Shaman (Windfury Melee DPS)**:

  ```lua
  /w %t co -heal,-caster,+dps,+enhancement,+windfury totem,+totems,+interrupt
  /w %t nc +food,+buff
  /w %t formation melee
  ```

- **Feral Cat Druid (Behind Bleed DPS)**:

  ```lua
  /w %t co -tank,-heal,-caster,+dps,+cat,+behind,+stealth,+potions
  /w %t nc +food,+stealth
  /w %t formation melee
  /w %t stance behind
  ```

### Ranged & Caster DPS Specializations

- **Marksmanship Hunter (Ranged DPS & Pet)**:

  ```lua
  /w %t co -tank,+dps,+ranged,+pet,+ammo,+potions
  /w %t nc +food
  /w %t formation far
  ```

- **Fire Mage (Maximum Burst DPS)**:

  ```lua
  /w %t co -tank,+dps,+fire,+aoe,+interrupt,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Frost Mage (Control & AoE DPS)**:

  ```lua
  /w %t co -tank,+dps,+frost,+aoe,+interrupt,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Affliction / Destruction Warlock (Shadow DPS & Pet)**:

  ```lua
  /w %t co -tank,+dps,+shadow,+pet,+curse,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Shadow Priest (Shadow Caster DPS)**:

  ```lua
  /w %t co -tank,-heal,+dps,+shadow,+shadowform,+cure,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Balance Druid (Boomkin Caster DPS)**:

  ```lua
  /w %t co -tank,-heal,-bear,-cat,+dps,+caster,+moonkin,+cure,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

---

## 9. Raid Assembly, Attunement & Instance Reset Blueprint

Blueprints for assembling, tuning, and resetting raid instances with playerbots.

### 10-Man, 20-Man & 40-Man Raid Compositions

| Raid Bracket | Target Instance | Recommended Composition | Macro Assembly Sequence |
| :--- | :--- | :--- | :--- |
| **10-Man** | Upper Blackrock Spire (UBRS), Karazhan Crypts | 2 Tanks, 3 Healers, 5 DPS | `.bot group size=5` (Party 1)<br>`/ra @subgroup=1 formation shield` |
| **20-Man** | Zul'Gurub (ZG), Ruins of Ahn'Qiraj (AQ20) | 3 Tanks, 5 Healers, 12 DPS | 4 balanced groups with class dispels (Priest/Paladin/Shaman/Mage) |
| **40-Man** | Molten Core, Blackwing Lair, AQ40, Naxxramas | 4–6 Tanks, 10–12 Healers, 22–26 DPS | 8 balanced subgroups, windfury totems, paladin blessing rotas |

### Instance Lockouts, Unbinding & Raid Resets

When managing raid locks or practicing encounters:

| Operation | GM Command | Functionality |
| :--- | :--- | :--- |
| **Unbind Current Player** | `.instance unbind <map_id\|all>` | Clears instance ID lockout for your character. |
| **Unbind Entire Group** | `.instance groupunbind <map_id\|all>` | Clears instance ID lockouts for all bots and players in group. |
| **Instance Statistics** | `.instance stats` | Displays active instance saves, player references, and memory footprint. |
| **Server-Wide Raid Reset** | `.server resetallraids` | Resets all raid instance lockouts across the entire realm simultaneously. |

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md) • [🤖 RandomBot Guide (rndbot_commands.md)](../rndbot_commands.md)
