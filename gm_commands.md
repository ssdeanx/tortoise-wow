# Tortoise-WoW Master Game Master & Playerbot Command Portal

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272)  
> **Architecture**: CMaNGOS + PlayerBots (ike3 / r-o-sh) + Dynamic Module System + Eluna Engine  
> **Coverage**: 100% of all 540 core commands + Module & Script extensions documented & verified live.

---

## 📚 Unified 8-Volume Master Reference Architecture

To provide deep technical parameters, command tables, and copy-paste in-game macro templates without multi-file hopping, the command manual is organized into **8 comprehensive Master Handbooks**:

| Volume | Master Handbook | Scope & Core Topics | Ready Macros |
| :---: | :--- | :--- | :---: |
| **01** | [**Game Master & World Administration**](references/01_game_master_handbook.md) | **100% of GM commands**: Progression, `.learn all_recipes`, `.gm options` engine cheats, `.god`, accounts, security (0–6), spawning (`.npc`, `.gobject`), `.ahbot`, `.balance`, `.copy`, 106+ `.reload` tables & diagnostics. | [GM Macro Card](references/01_game_master_handbook.md#1-categorized-in-game-lua-macros-top-priority) |
| **02** | [**Playerbot & Raid Automation**](references/02_playerbot_and_raid_automation.md) | **Complete Bot Control Center**: `.bot` & `.rndbot` suites, single-letter whisper shortcuts (`c`, `u`, `e`, `t`, `s`, `r`), `/w <bot> cheat`, strategy tokens (`co`/`nc`), `.dc` autonomous dungeon clear solver & raid macros. | [Bot Macro Deck](references/02_playerbot_and_raid_automation.md#1-master-macro-ui-cheat-sheet-copy-paste-ready) |
| **03** | [**World, Dungeons & Teleports Directory**](references/03_turtle_world_and_teleports.md) | **647+ `.tele` tokens**: Kalimdor, Eastern Kingdoms, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), instant attunements & master keyring. | [Travel Macros](references/03_turtle_world_and_teleports.md#1-categorized-in-game-lua-macros-top-priority) |
| **04** | [**Class Specs & Endgame Gear Directory**](references/04_class_specs_and_endgame_gear.md) | **All 9 Classes**: 1-Click BiS gear loadout macros, complete Tier 1/2/3 set IDs, BiS weapons, trinkets, enchants & verified 1.18.1 talent builds with interactive calculator URLs. | [BiS Loadouts](references/04_class_specs_and_endgame_gear.md#1-1-click-bis-class-loadouts-complete-1617-slot-sets) |
| **05** | [**Eluna Scripting & Core Development Engine**](references/05_developer_eluna_and_engine.md) | **Eluna Lua API**: Live reloads (`.reload eluna`), Event 42 Custom Command dispatch, Turtle-WoW `SpellInfo` compatibility layer, and production server hook examples. | [Script Macros](references/05_developer_eluna_and_engine.md#1-categorized-in-game-lua-macros-top-priority) |
| **06** | [**Automated Raid & Dungeon Operations**](references/06_automated_raid_and_dungeon_operations.md) | **Autonomous 1-Click Operations**: `.raid ready` pipeline, instant raid attunements, master keyring spawner, autonomous `.dc` tank autopilot, and tactical macro deck. | [Raid Operations Deck](references/06_automated_raid_and_dungeon_operations.md#2-master-macro-cheat-sheet-copy-paste-ready) |
| **07** | [**Guilds & Guild Operations**](references/07_guilds_and_guild_operations.md) | **Complete Guild System**: `.bot <guild>` batch operations (`gear guild best`, `prep guild epic`, `summon guild`), charter signing, `/w <bot> guild` invites/promotions/demotions/kicks/transfers, guild bank (`gb`), and 25-guild realm simulation. | [Guild Macro Deck](references/07_guilds_and_guild_operations.md#1-guild-master-macro-deck-copy-paste-ready) |
| **08** | [**Open World Bot Control & Strategies**](references/08_open_world_control_and_strategies.md) | **Autonomous Open World Operations**: `.rndbot grind`, `.rndbot rpg`, `.rndbot teleport/revive/upgrade`, dynamic realm load balancing (diff/pid/clean), bot persistence (`.bot always`), complete strategy token encyclopedia (co/nc/dead/react), and full real-time feedback inspection suite. | [World Control Deck](references/08_open_world_control_and_strategies.md#1-open-world--strategy-macro-deck-copy-paste-ready) |

---

## 🔑 Command Syntax & Target Wildcard Key

Before executing commands or macros, refer to this syntax key for targeting wildcards and parameters:

| Symbol / Token | Target Meaning | Example Usage | Explanation |
| :---: | :--- | :--- | :--- |
| **`*`** | **All Controlled Bots** | `.bot add *` • `.bot level * 60` • `.bot gear * best` • `.bot prep *` | Applies the action to **all bots in your current party** or all alternate characters on your account simultaneously. |
| **`%`** | **All Random Bots** | `.rndbot init %` • `.rndbot refresh %` • `.rndbot grind %` | Applies command server-wide to all loaded random playerbots. |
| **`%t`** | **Current Target** | `.cast %t 133` • `.appear %t` • `/w %t follow` | Substitutes the name or GUID of your **currently selected in-game target** (player, bot, or NPC). |
| **`[name]`** | **Specific Character Name** | `.bot add Bob` • `.bot remove Alice` • `/w Tankbot flee` | Executes the command specifically on the named player or bot. |
| **`[#]` / `<id>`** | **Numerical Value / Item ID** | `.levelup 60` • `.additem 19019 1` • `.modify money 10000000` | Specifies levels, item entry IDs, quantities, or currency amounts. |
| **`[token]`** | **System / Location Token** | `.tele deadmines` • `.dc test start deadmines` | Pre-defined system destination or dungeon identifier. |
| **`[quality]`** | **Gear Quality Filter** | `.bot gear * epic` • `.bot gear * best` • `.bot gear * rare` | Quality ceiling for level-scaled random item generation. |
| **`/p @all`** | **Party-Wide Bot Broadcast** | `/p @all tank attack` • `/p @all flee` • `/p @all loot` | Sends tactical instructions to **all party bots** in party chat. |
| **`/p @<role>`** | **Role / Class Filter** | `/p @tank attack` • `/p @healer drink` • `/p @mage cast blizzard` | Directs party commands to specific combat roles or classes. |

---

## ⚡ Master Quick-Lookup Macro Cheat Sheet

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`):

### 🌟 1. Complete Player Power-Boost Suite (Level 60 + Spells + Weapons/Defense + Travel)

```lua
.levelup 60
.learn all_myspells
.learn all_trainer
.maxskill
.modify money 10000000
.taxicheat on
.explorecheat on
.repairitems
.replenish
```

---

### 🛠️ 2. Secondary Skills & Professions Maxing Suite (Level 60 - 300/300)

> [!IMPORTANT]
> `.maxskill` skips professions in the core (`Player.cpp:7250`). `.learn all_recipes <name>` learns all recipes AND maxes the skill to 300. Run these dedicated macros to instantly max your crafting and gathering skills:

#### Secondary Skills Maxer (All 4 Turtle-WoW Secondary Skills):
```lua
.learn all_recipes first aid
.learn all_recipes cooking
.learn all_recipes fishing
.learn all_recipes survival
```

#### Primary Professions Maxer (Pick the 2 matching your character):
```lua
-- Alchemy & Herbalism
.learn all_recipes alchemy
.learn all_recipes herbalism

-- Blacksmithing & Mining
.learn all_recipes blacksmithing
.learn all_recipes mining

-- Engineering & Mining
.learn all_recipes engineering
.learn all_recipes mining

-- Enchanting & Tailoring
.learn all_recipes enchanting
.learn all_recipes tailoring

-- Leatherworking & Skinning
.learn all_recipes leatherworking
.learn all_recipes skinning
```

#### Targeted Skill Adjustment (`.setskill <id> <cur> [max]`):
- First Aid: `.setskill 129 300 300` • Check: `.getskillvalue 129`
- Survival *(Turtle Custom)*: `.setskill 142 300 300` • Check: `.getskillvalue 142`
- Cooking: `.setskill 185 300 300` • Check: `.getskillvalue 185`
- Fishing: `.setskill 356 300 300` • Check: `.getskillvalue 356`
- Riding: `.setskill 762 300 300` • Check: `.getskillvalue 762`
- Lockpicking: `.setskill 633 300 300` • Check: `.getskillvalue 633`

---

### ⚡ 3. Godmode & Engine Caster Cheats (`.gm options` & `.god`)

```lua
-- Full Godmode Engine Cheats (Instant Cast + No CD + Infinite Mana + 100% Crit & Proc)
.gm options cd cast power crit proc
.god on

-- Revert Cheats Back to Normal
.gm options off
.god off
```

---

### 🤖 4. Complete Bot Party Bootstrap (Level 60 + BiS Gear + Enchants + Spells + Prep)

```lua
.bot add *
.bot summon *
.bot level * 60
.bot gear * best
.bot enchants *
.bot train *
.bot prep *
.bot food *
.bot potions *
.bot regs *
.bot ammo *
.bot p follow
```

---

### 🏰 5. Autonomous Dungeon Clear Engine & Spectator (.dc)

```lua
-- In-Dungeon Tank Auto-Pilot (.dc)
.dc on                                 -- Activates autonomous dungeon routing & pulls on party tank bot
.dc off                                -- Disables autonomous dungeon clearing
.dc skip                               -- Skips current targeted mob pack / boss and recalculates route
.dc pause                              -- Pauses tank bot advance and pulls
.dc pull                               -- Orders tank bot to immediately pull next pack or target
.dc status                             -- Shows live dungeon progression, target boss, and party readiness
.dc bosses                             -- Lists all instance bosses, encounter status (alive/killed/skipped)
.dc go <boss_name>                     -- Sets direct destination boss target to route toward
.dc config                             -- Dumps all live DungeonClear.* config values and per-run overrides

-- Spectator Camera Controls (.dc spectate)
.dc spectate                           -- Toggles free-flying spectator camera
.dc spectate follow <botname>          -- Rides spectator camera on specified bot (run again to stop)
.dc spectate next                      -- Cycles spectator camera to next party bot (alias: .dc spectate n)
.dc spectate prev                      -- Cycles spectator camera to previous party bot (alias: .dc spectate p)
.dc spectate list                      -- Lists all watchable bots in the current instance (alias: .dc spectate who)

-- Headless 5-Bot Automated Test Runs (.dc test)
.dc test list                          -- Lists all supported test dungeons, tokens, map IDs, and levels
.dc test gear <dungeon> [heroic]       -- Displays recommended item level (ilvl) gear ladders and ceilings
.dc test start <dungeon> [heroic]      -- Starts automated 5-bot clear run with random party comp
.dc test start <dungeon> party=T,H,D1,D2,D3 -- Starts test run with specific named character roster
.dc test status                        -- Shows live test run status, boss count, kill bitmask, run duration
.dc test watch [selector]              -- Enters GM invisible mode, teleports into dungeon, follows tank bot
.dc test watch next                    -- Tours next active test run in batch campaign without leaving
.dc test watch off                     -- Stops camera, restores GM visibility, teleports back, drops binds
.dc test stop [selector|all]           -- Aborts active test run, specific run by ID/token, or all runs
```

---

### 🏪 6. Auction House Bot Management (`.ahbot`)

```lua
.ahbot status [all]                    -- Views active auction counts (Alliance, Horde, Neutral) & queues
.ahbot reload                          -- Hot-reloads ahbot.conf settings live without server restart
.ahbot rebuild [all]                   -- Flushes stale auctions and refills auction stock from DB
.ahbot update                          -- Forces an immediate auction house check and posting cycle
.ahbot item <id>                       -- Inspects item baseline market price and posting chance
.ahbot item <id> <val> [chance]        -- Sets custom price override (value 0 bans buying/posting)
.ahbot item <id> reset                 -- Resets item pricing and posting override back to defaults
```

---

### 🛡️ 7. Tactical Bot Party Combat Commands

```lua
-- Attack & DPS Burst
/p @all tank attack
/p @all attack
/p @all max dps

-- Emergency Retreat
/p @all flee
/p @all follow
/p @all stay
.combatstop

-- Post-Combat Cleanup
/p @all loot
/p @all food
.bot prep *

-- Bot Cheats (Whisper)
/w <botname> cheat ?                   -- List active cheats on bot
/w <botname> cheat +health             -- Infinite health on bot
/w <botname> cheat +mana               -- Infinite mana on bot
/w <botname> cheat +cooldown           -- Zero cooldowns on bot
/w <botname> cheat +attackspeed        -- Haste cap (+9999) on bot
```

---

### 🖥️ 8. Core Server Operations & Diagnostics

```lua
.saveall                               -- Immediately writes all online player data to database
.server info                           -- Realm uptime, player count, active bot count, tick stats
.server restart 60 Scheduled restart   -- Announces countdown and initiates clean server reboot
.server shutdown 60 Server maintenance -- Announces countdown and initiates clean shutdown
.server restart cancel                 -- Cancels active shutdown or restart timer
.server set motd <New MOTD Text>       -- Updates realm Message of the Day live in memory
.perf intervalreport 60                -- Emits performance CPU/memory reports every 60s
.perfmon toggle                        -- Toggles microsecond Playerbot AI profiler
.pdump write backup.dmp <PlayerName>   -- Generates portable binary character dump in storage directory
.pdump load backup.dmp <Account> <NewName> -- Restores portable character dump to target account
.character deleted list name <pattern> -- Searches soft-deleted characters in world database
.character deleted restore <name>      -- Restores soft-deleted character and restores inventory/spells
.reload all                            -- Reloads all game configuration and database tables
```

---

## 🧭 Direct Reference Navigation Matrix

| Volume | Handbook Title | Domain & Key Topics | Direct Link |
| :---: | :--- | :--- | :---: |
| **01** | [**01. Game Master & World Administration**](./references/01_game_master_handbook.md) | 100% of GM commands, player progression, cheats, security, spawning, moderation, tickets, reloads & diagnostics | [Open Volume 01](./references/01_game_master_handbook.md) |
| **02** | [**02. Playerbot & Raid Automation**](./references/02_playerbot_and_raid_automation.md) | Complete .bot & .rndbot control, whisper shortcuts, strategy tokens (co/nc), .dc dungeon clear solver & raid macros | [Open Volume 02](./references/02_playerbot_and_raid_automation.md) |
| **03** | [**03. World, Dungeons & Teleports Directory**](./references/03_turtle_world_and_teleports.md) | 647+ .tele tokens, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements | [Open Volume 03](./references/03_turtle_world_and_teleports.md) |
| **04** | [**04. Class Specs & Endgame Gear Directory**](./references/04_class_specs_and_endgame_gear.md) | 1-Click BiS gear loadouts, Tier 1/2/3 set IDs, weapons, trinkets, enchants & verified 1.18.1 talent builds across all 9 classes | [Open Volume 04](./references/04_class_specs_and_endgame_gear.md) |
| **05** | [**05. Eluna Scripting & Core Development Engine**](./references/05_developer_eluna_and_engine.md) | Eluna Lua API, live reloads (.reload eluna), Event 42 Custom Command dispatch, SpellInfo layer & C++ hook mechanics | [Open Volume 05](./references/05_developer_eluna_and_engine.md) |
| **06** | [**06. Automated Raid & Dungeon Operations**](./references/06_automated_raid_and_dungeon_operations.md) | Autonomous 1-Click Operations: .raid ready pipeline, instant raid attunements, master keyring spawner & tactical macro deck | [Open Volume 06](./references/06_automated_raid_and_dungeon_operations.md) |

---

[🏠 Master Portal (gm_commands.md)](./gm_commands.md) • [📖 Project Readme](./README.md) • [🤖 Bot Control Center (02_playerbot_and_raid_automation.md)](./references/02_playerbot_and_raid_automation.md)
