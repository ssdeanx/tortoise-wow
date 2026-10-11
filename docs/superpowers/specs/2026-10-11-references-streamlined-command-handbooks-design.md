# Design Specification: Streamlined Command Handbooks for Turtle-WoW

**Date**: 2026-10-11  
**Target Directory**: `/home/sam/tortoise-wow/references/`  
**Goal**: Overhaul all 9 reference files into enterprise-grade, high-density, readable command cheat sheets. Remove unnecessary markdown tables and C++ engine theory, replacing them with copy-pasteable command examples with `-- comments`.

---

## 1. Problem Statement & User Requirements

### Pain Points
- Existing reference documents in `/home/sam/tortoise-wow/references/` are bloated (up to 3,100+ lines per file), heavily laden with wide markdown tables, and filled with deep C++ engine citations, compiler macros, and SQL verification arguments.
- Tables are difficult to read, copy, or use in-game or via terminal.
- Finding specific command syntax, item IDs, or macro snippets requires scrolling through excessive architectural and debugging commentary.

### Requirements
- **High Command Density**: Maximize actual commands, arguments, and practical usage examples.
- **Minimal Fluff / Commentary**: Eliminate academic C++ packet flow discussions, theoretical tables, and redundant metadata. Keep essential warnings/caveats strictly as concise inline `-- NOTE:` comments.
- **Table-Free Design**: Replace markdown tables with structured, categorized command blocks using `-- comments`.
- **Enterprise-Grade Copy-Paste Usability**: All commands formatted for direct use in the WoW chat bar (`/m` macros, console, or CLI).

---

## 2. Formatting & Architecture Standard

Every handbook in `references/` will conform to the following clean structure:

```markdown
# [Volume Number]. [Handbook Title]

> **Quick Summary**: [1-2 sentences on what this handbook covers]
> **Syntax Key**: `<arg>` = required, `[arg]` = optional, `%t` = current target, `*` = all party bots

---

## Table of Contents
[Numbered links to sections]

---

## 1. Fast-Action Macro Suites (Top Priority)
[Ready-to-paste grouped command blocks for common multi-command workflows]

## [N]. [Categorized Command Sections]
[High-density command blocks with inline -- comments]
```

### Command Block Standard
```lua
-- [Concise description of action or purpose]
-- NOTE: [Any critical caveat or argument constraint if applicable]
.command <required_arg> [optional_arg]
```

---

## 3. Scope of Transformation Across All 9 Files

### File 01: `01_game_master_handbook.md`
- **Scope**: Complete GM & Admin Command Suite.
- **Content**:
  - Top Priority Macro Suites (Instant Boost, God Mode, Fast Travel, Realm Maintenance).
  - Character Progression & Skills (`.levelup`, `.learn all_myspells`, `.learn all_trainer`, `.maxskill`, `.learn all_recipes <prof>`, `.reset`).
  - Combat, Cheats & Auras (`.god`, `.combatstop`, `.cooldown`, `.replenish`, `.gm on/off`, `.gm visible`, `.gm fly`).
  - Items, Economy & Spawning (`.additem`, `.modify money`, `.bank`, `.mailbox`, `.npc add`, `.gobject add`).
  - Server Management & Maintenance (`.saveall`, `.server restart`, `.server resetallraids`, `.broadcast`).
  - Moderation, Tickets & Bans (`.ticket`, `.ban`, `.kick`, `.mute`).
  - Complete Hot-Reload Catalog (`.reload <target>` for all supported tables, clearly grouping what works vs what requires server restart).
  - AHBot Administration (`.ahbot` commands).

### File 02: `02_playerbot_and_raid_automation.md`
- **Scope**: Playerbot Core Control, Raid Channels, Strategies & Formations.
- **Content**:
  - Top Priority Macro Suites (Party Assemble, Raid Ready, Combat Engagement, Recovery).
  - Core `.bot` Suite (`.bot add`, `.bot remove`, `.bot gear * best`, `.bot prep *`, `.bot level * 60`, `.bot summon *`).
  - Communication Channels & Scope:
    - Single-letter whispers (`c` summary, `u` unbind, `e` equipment, `t` talents, `s` spells, `r` reset).
    - Channel routing (`/w <bot>`, `/p`, `/ra`).
    - Chat targeting filters (`@tank`, `@heal`, `@dps`, `@melee`, `@ranged`, `@class`).
  - Strategy Catalog:
    - Combat strategies (`co +prot`, `co +tank`, `co +heal`, `co +dps`, `co +aoe`, `co +shadow`, etc.).
    - Non-combat strategies (`nc +food`, `nc +follow`, `nc +stay`, `nc +loot`).
  - Formations & Stances (`/p formation line`, `near`, `shield`, `circle`; stances: `passive`, `defensive`, `aggressive`).
  - Bot Cheats Subsystem (`/w %t cheat taxicheat`, `maxskill`, `god`, `gold`).
  - Aura Buff Lists (`.aura <id>` for world buffs, raid buffs, resistances, flasks).

### File 03: `03_turtle_world_and_teleports.md`
- **Scope**: World Navigation, Dungeons, Custom Instances & Attunements.
- **Content**:
  - Spatial Movement Commands (`.go forward`, `.go up`, `.go corpse`, `.go target`, `.go xy`, `.tele group <dest>`).
  - Instant Raid Attunement & Master Keyring Spawners (`.quest add/complete` chains and `.additem` for all dungeon/raid keys).
  - Turtle-WoW Custom Instance Teleports (Crescent Grove, Hateforge, Scarlet Citadel, Karazhan Crypts, Gilneas City, Alah'Thalas, Emerald Sanctum).
  - Complete Classic Dungeon Teleport Directory (Eastern Kingdoms & Kalimdor).
  - Classic & Custom Raid Teleport Directory (MC, BWL, Ony, ZG, AQ20, AQ40, Naxx, SC, Karazhan).
  - Capital Cities, Faction Hubs & World Boss Teleport Tokens.

### File 04: `04_class_specs_and_endgame_gear.md`
- **Scope**: 9-Class Talent Builds & Endgame BiS Gear Loadouts.
- **Content**:
  - High-density talent commands (`.learn` / talent distributions) for all 9 classes (Warrior, Paladin, Shaman, Druid, Rogue, Hunter, Mage, Warlock, Priest).
  - Copy-paste BiS Item Spawners for all major specs:
    - Categorized by class and spec (e.g., Fury/Prot Warrior, Holy/Ret Paladin, Resto/Ele Shaman, etc.).
    - Formatted as `.additem <id> 1` with `-- Slot: Item Name` comments.
  - Complete Tier 1, Tier 2, and Tier 3 Set Item IDs in copy-paste blocks.
  - Top weapons, trinkets, and endgame enchants.

### File 05: `05_developer_eluna_and_engine.md`
- **Scope**: Eluna Lua Scripting & Engine Developer Reference.
- **Content**:
  - Live Reload & Testing Commands (`.reload eluna`, debug output).
  - Event Registration Cheat Sheet (All verified `Register*Event` calls with exact signatures and minimal Lua examples).
  - Common Eluna Object Methods (Player, Creature, GameObject, Map, Unit).
  - Production Script Blueprints (Custom gossip NPC, automated raid buff trigger, custom chat command handler).

### File 06: `06_automated_raid_and_dungeon_operations.md`
- **Scope**: Automated Raid & Dungeon Operations (`.raid` & `.dc`).
- **Content**:
  - Master 1-Click Operations Macro Deck.
  - The `.raid ready` Pipeline (`.raid ready`, `.raid ready buff`, `.raid ready consumables`, `.raid ready gear`).
  - Autonomous Dungeon Clear AI Deck (`.dc` commands: `.dc start`, `.dc stop`, `.dc pause`, `.dc resume`, `.dc pull`, `.dc skip`, `.dc status`, `.dc test`).
  - Tactical Boss Engagement, Door Opening, and Recovery Macros.
  - Raid Attunement and Keyring Automation.

### File 07: `07_guilds_and_guild_operations.md`
- **Scope**: Bot Guild Management & Operations.
- **Content**:
  - Universal Guild Scope Commands (`.bot <guild>`: `.bot gear guild best`, `.bot prep guild epic`, `.bot summon guild`).
  - Guild Whispers & Management (`/w <bot> guild invite`, `promote`, `demote`, `kick`, `leader`).
  - Autonomous Charter Signing and Recruitment.
  - Guild Bank (`gb`) commands and autonomous Crafting Orders.
  - Configuration reference for `aiplayerbot.conf` guild parameters.

### File 08: `08_open_world_control_and_strategies.md`
- **Scope**: Open World Population & Random Bot Simulation.
- **Content**:
  - Autonomous Population Management (`.rndbot grind`, `.rndbot rpg`, `.rndbot teleport`, `.rndbot revive`, `.rndbot upgrade`, `.rndbot refresh`).
  - Performance & Lag Governor Tuning (`.rndbot diff`, `.rndbot pid`, `.rndbot clean`).
  - Bot Persistence & Creation (`.bot always`, `.bot group`, `.bot create`).
  - Full Strategy Token Encyclopedia (`co`, `nc`, `react`, `de` tokens with clean examples).
  - Real-Time Inspection Whispers (`/w <bot> status`, `stats`, `spells`, `talents`, `items`).

### File 09: `09_soap_agent_command_interface.md`
- **Scope**: SOAP Remote Command Execution for Agents & Scripts.
- **Content**:
  - Build & Enablement flags (`cmake -DENABLE_SOAP=ON`).
  - Ready-to-use Bash & Python dispatch scripts to execute server commands over HTTP SOAP.
  - Direct `curl` examples with XML payload.
  - Key operational rules (console security level, single command execution, error responses).

---

## 4. Verification & Validation Plan
- Verify that every command syntax matches active CMaNGOS / Playerbots / Eluna implementation.
- Ensure all markdown formatting is pristine, headers are consistent, and files are easy to read and navigate.
- Validate that all internal markdown links between reference files remain functional.
