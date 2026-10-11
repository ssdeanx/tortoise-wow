# Streamlined Command Handbooks Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Transform all 9 reference files in `references/` into table-free, high-density command handbooks with copy-pasteable examples and `-- comments`, preserving 100% of commands, items, spells, tokens, and options without any data loss.

**Architecture:** Each markdown file is refactored from table-heavy layouts into categorized Lua/command code blocks where every command/ID is annotated with a concise `-- comment`. All tables are eliminated while retaining every single command, parameter, item ID, and configuration entry.

**Tech Stack:** Markdown, World of Warcraft / CMaNGOS in-game slash commands (`.`, `/p`, `/ra`, `/w`), Lua.

## Global Constraints

- **100% Data Retention (CRITICAL)**: Preserve every command, subcommand, argument variation, item entry ID, spell ID, teleport token, talent distribution, and configuration key. Do NOT omit any items or commands.
- **Zero Markdown Tables**: No `| Column |` markdown tables anywhere in the 9 files. Every single table row must be converted into a copy-pasteable command example with `-- comments`.
- **Copy-Paste Readability**: Structure commands in clear, thematic code blocks (` ```lua `) annotated with `-- comments`.
- **Concise Inline Caveats**: Convert important warnings, engine traps, or argument constraints into inline `-- NOTE:` comments.
- **Link Preservation**: Maintain working markdown cross-links between the handbooks.

---

### Task 1: Refactor `references/01_game_master_handbook.md` (GM & World Administration)

**Files:**
- Modify: `references/01_game_master_handbook.md`

**Interfaces:**
- Consumes: Current `01_game_master_handbook.md` command catalog, macro cards, reload targets, and security levels.
- Produces: Clean, table-free handbook with copy-pasteable GM commands and macros with `-- comments`.

- [ ] **Step 1: Convert all tables in 01_game_master_handbook.md to commented code blocks**
  - Architecture table -> Clean bulleted link list.
  - Section 2 Character Progression table -> Lua code block with `-- comments` for each command (`.levelup`, `.learn`, `.learn all_recipes`, `.unlearn`, `.maxskill`, `.setskill`, `.getskillvalue`, `.taxicheat`, `.explorecheat`, `.repairitems`, `.replenish`, `.cooldown`, `.reset`, etc.).
  - Section 3 Combat & Auras table -> Lua block with `.god`, `.combatstop`, `.aura`, `.unaura`, `.die`, `.revive`, `.damage`, etc.
  - Section 4 Stats & Morphs table -> Lua block with `.modify`, `.morph`, `.demorph`, `.setfaction`, etc.
  - Section 5 Items & Economy table -> Lua block with `.additem`, `.additemset`, `.bank`, `.mailbox`, `.modify money`, etc.
  - Section 6 Security Ranks table -> Code block listing account security commands and rank values.
  - Section 7 Spawning & World Editing table -> Lua block with `.npc add`, `.npc delete`, `.gobject add`, `.gobject delete`, etc.
  - Section 8 Quests & Instances table -> Lua block with `.quest add`, `.quest complete`, `.instance unbind`, etc.
  - Section 9 Server Management table -> Lua block with `.saveall`, `.server restart`, `.server shutdown`, `.server resetallraids`, etc.
  - Section 10 Moderation & Bans table -> Lua block with `.ticket`, `.kick`, `.ban`, `.pban`, `.mute`, etc.
  - Section 11 Turtle Custom Systems table -> Lua block with vanity, titles, and custom tokens.
  - Section 12 Diagnostics & Navmesh table -> Lua block with `.debug`, `.gps`, `.wpgps`, etc.
  - Section 13 Hot-Reload Catalog table -> Lua block with all 106+ `.reload <target>` commands grouped by domain (core, creature, quest, script, etc.) with `-- NOTE:` for targets requiring restarts.
  - Section 14 AHBot Administration table -> Lua block with all `.ahbot` subcommands.

- [ ] **Step 2: Verify zero tables remain in 01_game_master_handbook.md**
  Run: `grep -c '^|' references/01_game_master_handbook.md`
  Expected: 0

- [ ] **Step 3: Commit Task 1**
  Run: `git add references/01_game_master_handbook.md && git commit -m "docs: streamline 01_game_master_handbook.md to table-free commented examples"`

---

### Task 2: Refactor `references/02_playerbot_and_raid_automation.md` (Playerbot & Raid Automation)

**Files:**
- Modify: `references/02_playerbot_and_raid_automation.md`

**Interfaces:**
- Consumes: Current `02_playerbot_and_raid_automation.md` bot controls, strategy tokens, whisper commands, formations, and aura buff IDs.
- Produces: Clean, table-free bot automation reference with 100% retained bot commands, whisper shortcuts, and buff IDs.

- [ ] **Step 1: Convert all tables and C++ packet essays in 02_playerbot_and_raid_automation.md**
  - Master Reference Architecture table -> Bulleted link list.
  - Buff Spell IDs tables (Dungeon buffs, world buffs, resistances, food/drink, flasks, elixirs) -> Grouped Lua blocks:
    ```lua
    -- World Buff: Rallying Cry of the Dragonslayer (+10% Spell Crit, +5% Melee Crit, +140 AP)
    .aura 22888
    -- World Buff: Songflower Serenade (+15 All Stats, +5% Crit)
    .aura 15366
    ```
  - Chat channels & whisper route tables -> Copy-paste blocks explaining `/w`, `/p`, `/ra` with `-- comments`.
  - Chat Filters table (@tank, @heal, @dps, @melee, @ranged, @class) -> Clear examples with `-- comments`.
  - Single-letter whisper shortcuts table (`c`, `u`, `e`, `t`, `s`, `r`) -> Clean commented blocks.
  - Core `.bot` and `.rndbot` tables -> Clean commented blocks.
  - Formations and Stances tables -> Clean commented `/p formation <type>` and stance blocks.
  - Combat / Non-Combat strategy tables -> Clean commented `co +<token>` and `nc +<token>` blocks.
  - Bot Cheats table -> Clean `/w %t cheat <cheat>` blocks.

- [ ] **Step 2: Verify zero tables remain in 02_playerbot_and_raid_automation.md**
  Run: `grep -c '^|' references/02_playerbot_and_raid_automation.md`
  Expected: 0

- [ ] **Step 3: Commit Task 2**
  Run: `git add references/02_playerbot_and_raid_automation.md && git commit -m "docs: streamline 02_playerbot_and_raid_automation.md to table-free commented examples"`

---

### Task 3: Refactor `references/03_turtle_world_and_teleports.md` (World, Dungeons & Teleports)

**Files:**
- Modify: `references/03_turtle_world_and_teleports.md`

**Interfaces:**
- Consumes: Current `03_turtle_world_and_teleports.md` teleport directory (Classic + Turtle custom), attunement quests, and keyring IDs.
- Produces: Clean, table-free navigation directory with all 647+ teleport tokens, key items, and attunement commands.

- [ ] **Step 1: Convert all tables in 03_turtle_world_and_teleports.md**
  - Architecture table -> Bulleted link list.
  - Custom Turtle Dungeons & Raids tables (Crescent Grove, Hateforge, Scarlet Citadel, Karazhan Crypts, Gilneas City, Alah'Thalas, Emerald Sanctum, Timbermaw Hold) -> Commented `.tele <token>` blocks with map IDs and level ranges in comments.
  - Eastern Kingdoms & Kalimdor dungeon tables -> Commented `.tele <token>` blocks.
  - Classic & Custom raid tables -> Commented `.tele <token>` blocks.
  - World Boss spawn tables -> Commented `.tele <token>` blocks with location notes.
  - Capital cities and zone tables -> Commented `.tele <token>` blocks.
  - Attunement and keyring sections -> Clean `.quest add/complete` and `.additem` blocks with comments.

- [ ] **Step 2: Verify zero tables remain in 03_turtle_world_and_teleports.md**
  Run: `grep -c '^|' references/03_turtle_world_and_teleports.md`
  Expected: 0

- [ ] **Step 3: Commit Task 3**
  Run: `git add references/03_turtle_world_and_teleports.md && git commit -m "docs: streamline 03_turtle_world_and_teleports.md to table-free commented examples"`

---

### Task 4: Refactor `references/04_class_specs_and_endgame_gear.md` (9-Class Talent Builds & BiS Gear)

**Files:**
- Modify: `references/04_class_specs_and_endgame_gear.md`

**Interfaces:**
- Consumes: Current `04_class_specs_and_endgame_gear.md` 9-class talent trees, item entry IDs, Tier 1/2/3 set IDs, weapons, trinkets, and enchants.
- Produces: Clean, table-free class builds and item spawner cheat sheet with 100% retained item IDs and talent distributions.

- [ ] **Step 1: Convert all tables and SQL analysis in 04_class_specs_and_endgame_gear.md**
  - Verification tier and architecture tables -> Brief guidelines in text.
  - 9 Class Specs (Warrior, Paladin, Shaman, Druid, Rogue, Hunter, Mage, Warlock, Priest) -> Clean talent allocation summaries and `.learn` commands with comments.
  - 30 One-Click BiS Loadout tables -> Convert all slots (Head, Neck, Shoulders, Back, Chest, Wrists, Hands, Waist, Legs, Feet, Rings, Trinkets, Weapons, Ranged) into commented `.additem <id> 1` commands:
    ```lua
    -- Head: Lionheart Helm
    .additem 12640 1
    -- Neck: Onyxia Tooth Pendant
    .additem 18404 1
    ```
  - Tier 1, Tier 2, Tier 3, and Zul'Gurub item sets -> Commented `.additem <id> 1` blocks per set.
  - Top endgame weapons, trinkets, and enchants -> Commented command blocks.

- [ ] **Step 2: Verify zero tables remain in 04_class_specs_and_endgame_gear.md**
  Run: `grep -c '^|' references/04_class_specs_and_endgame_gear.md`
  Expected: 0

- [ ] **Step 3: Commit Task 4**
  Run: `git add references/04_class_specs_and_endgame_gear.md && git commit -m "docs: streamline 04_class_specs_and_endgame_gear.md to table-free commented examples"`

---

### Task 5: Refactor `references/05_developer_eluna_and_engine.md` (Eluna Scripting & Core Development)

**Files:**
- Modify: `references/05_developer_eluna_and_engine.md`

**Interfaces:**
- Consumes: Current `05_developer_eluna_and_engine.md` Eluna hooks, VMangos method listings, event registries, and example blueprints.
- Produces: Clean, table-free Lua scripting reference with practical Lua code blocks and `-- comments`.

- [ ] **Step 1: Convert all tables in 05_developer_eluna_and_engine.md**
  - Architecture and registrars tables -> Clean code snippets showing exact `Register*Event` syntax with `-- comments`.
  - Server & World events, Player events, Creature events, GameObject events, Item events, Gossip events -> Lua code examples with `-- comments`.
  - Object method tables (Player, Creature, GameObject, Map, Unit) -> Grouped method calls with comments.
  - Production blueprints (Gossip NPC, raid buffs, chat commands) -> Pristine copy-paste Lua scripts.

- [ ] **Step 2: Verify zero tables remain in 05_developer_eluna_and_engine.md**
  Run: `grep -c '^|' references/05_developer_eluna_and_engine.md`
  Expected: 0

- [ ] **Step 3: Commit Task 5**
  Run: `git add references/05_developer_eluna_and_engine.md && git commit -m "docs: streamline 05_developer_eluna_and_engine.md to table-free commented examples"`

---

### Task 6: Refactor `references/06_automated_raid_and_dungeon_operations.md` (Automated Raid & Dungeon Operations)

**Files:**
- Modify: `references/06_automated_raid_and_dungeon_operations.md`

**Interfaces:**
- Consumes: Current `06_automated_raid_and_dungeon_operations.md` `.raid` and `.dc` command surfaces, dungeon tokens, autopilot routines, and runbooks.
- Produces: Clean, table-free raid & dungeon operations handbook with copy-pasteable commands and `-- comments`.

- [ ] **Step 1: Convert all tables in 06_automated_raid_and_dungeon_operations.md**
  - Architecture and overview tables -> Bulleted link list.
  - Master Macro Cheat Sheet -> Clean Lua code blocks with comments.
  - `.raid ready` subcommand tables -> Commented `.raid ready <subcommand>` blocks.
  - Complete `.dc` command tables (live commands, `.dc test` commands, 68 tokens) -> Commented `.dc <subcommand>` blocks with all 68 tokens listed as commented examples:
    ```lua
    -- Deadmines
    .dc start deadmines
    -- Molten Core
    .dc start moltencore
    ```
  - Instance runbooks and boss trigger tables -> Clean tactical macro blocks.

- [ ] **Step 2: Verify zero tables remain in 06_automated_raid_and_dungeon_operations.md**
  Run: `grep -c '^|' references/06_automated_raid_and_dungeon_operations.md`
  Expected: 0

- [ ] **Step 3: Commit Task 6**
  Run: `git add references/06_automated_raid_and_dungeon_operations.md && git commit -m "docs: streamline 06_automated_raid_and_dungeon_operations.md to table-free commented examples"`

---

### Task 7: Refactor `references/07_guilds_and_guild_operations.md` (Guilds & Guild Operations)

**Files:**
- Modify: `references/07_guilds_and_guild_operations.md`

**Interfaces:**
- Consumes: Current `07_guilds_and_guild_operations.md` guild targeting scopes, management whispers, guild bank (`gb`), and config keys.
- Produces: Clean, table-free guild operations handbook.

- [ ] **Step 1: Convert all tables in 07_guilds_and_guild_operations.md**
  - Architecture table -> Bulleted links.
  - `.bot guild` operations table -> Commented `.bot <operation> guild` blocks.
  - Guild management whispers table -> Commented `/w <bot> guild <action>` blocks.
  - Guild bank and crafting order tables -> Commented command blocks.
  - `aiplayerbot.conf` config settings table -> Clean commented `.conf` snippets.

- [ ] **Step 2: Verify zero tables remain in 07_guilds_and_guild_operations.md**
  Run: `grep -c '^|' references/07_guilds_and_guild_operations.md`
  Expected: 0

- [ ] **Step 3: Commit Task 7**
  Run: `git add references/07_guilds_and_guild_operations.md && git commit -m "docs: streamline 07_guilds_and_guild_operations.md to table-free commented examples"`

---

### Task 8: Refactor `references/08_open_world_control_and_strategies.md` (Open World Bot Control & Strategies)

**Files:**
- Modify: `references/08_open_world_control_and_strategies.md`

**Interfaces:**
- Consumes: Current `08_open_world_control_and_strategies.md` `.rndbot` commands, strategy catalog (co/nc/react/de), RTS controls, and whispers.
- Produces: Clean, table-free open world bot handbook.

- [ ] **Step 1: Convert all tables in 08_open_world_control_and_strategies.md**
  - Architecture table -> Bulleted links.
  - `.rndbot` population commands table -> Commented `.rndbot <cmd>` blocks.
  - Lag governor tuning table -> Commented `.rndbot diff/pid/clean` blocks.
  - Strategy catalog tables (combat, non-combat, movement, reaction, dead) -> Commented `co +<token>` and `nc +<token>` blocks.
  - Tactical inspection whispers table -> Commented `/w <bot> <query>` blocks.
  - RTS click-to-move tables -> Commented `/w <bot> rtsc <cmd>` blocks.

- [ ] **Step 2: Verify zero tables remain in 08_open_world_control_and_strategies.md**
  Run: `grep -c '^|' references/08_open_world_control_and_strategies.md`
  Expected: 0

- [ ] **Step 3: Commit Task 8**
  Run: `git add references/08_open_world_control_and_strategies.md && git commit -m "docs: streamline 08_open_world_control_and_strategies.md to table-free commented examples"`

---

### Task 9: Refactor `references/09_soap_agent_command_interface.md` (SOAP Agent Command Interface)

**Files:**
- Modify: `references/09_soap_agent_command_interface.md`

**Interfaces:**
- Consumes: Current `09_soap_agent_command_interface.md` SOAP request syntax, curl examples, and bash helper functions.
- Produces: Clean, table-free SOAP interface guide with copy-pasteable curl, bash, and Python command runners.

- [ ] **Step 1: Convert all tables in 09_soap_agent_command_interface.md**
  - Operation / Namespace table -> Commented text notes.
  - Setup and configuration tables -> Clean bash and config snippets with comments.
  - Request envelopes and curl commands -> Copy-pasteable scripts with `--` and `#` comments.

- [ ] **Step 2: Verify zero tables remain in 09_soap_agent_command_interface.md**
  Run: `grep -c '^|' references/09_soap_agent_command_interface.md`
  Expected: 0

- [ ] **Step 3: Commit Task 9**
  Run: `git add references/09_soap_agent_command_interface.md && git commit -m "docs: streamline 09_soap_agent_command_interface.md to table-free commented examples"`

---

### Task 10: Validation & Cross-Reference Audit

**Files:**
- Read/Audit: All files in `references/`

- [ ] **Step 1: Check that no markdown tables exist across the entire references directory**
  Run: `grep -rn '^|' references/`
  Expected: 0 matches

- [ ] **Step 2: Verify internal markdown links**
  Run: `grep -rn '\[.*\](\./.*\.md)' references/`
  Expected: All relative links point to existing volume files.

- [ ] **Step 3: Final sanity commit and git status check**
  Run: `git status references/`
