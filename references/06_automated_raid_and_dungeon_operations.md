---
name: Automated Raid & Dungeon Operations Handbook
description: Engine-verified operator guide for Turtle-WoW raid and dungeon play — the .raid ready one-click pipeline, instant attunements and the master keyring spawner, the full .dc autonomous dungeon-clear command surface including .dc test, the automatic role and formation matrix, per-instance fast-teleport directory, and the .raid Eluna facade's five verb layers with its trap register. Use it when running a bot raid, debugging a .dc or .raid command that silently fails, or checking which script file is actually live.
version: 1.0.0
platform: Turtle-WoW 1.18.1 (Build 7272)
stack: CMaNGOS + mod-playerbots (ike3 / r-o-sh) + mod-dungeon-clear
glob:
  - run/lua_scripts/raid_automation.lua
  - run/mangosd.conf
  - run/mod_dungeon_clear.conf
  - run/dc_test_dungeons.json
  - modules/mod-dungeon-clear/src/DungeonClearCommand.cpp
  - modules/mod-dungeon-clear/src/Ai/Dungeon/DungeonClear/
  - modules/mod-dungeon-clear/src/TestRun/
  - modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp
  - modules/mod-playerbots/src/playerbot/PlayerbotAI.cpp
  - modules/mod-playerbots/src/playerbot/strategy/generic/ChatCommandHandlerStrategy.cpp
  - modules/mod-playerbots/src/playerbot/strategy/values/Stances.cpp
  - src/game/Chat/Chat.cpp
  - src/scripts/dungeons/
tags:
  - raids
  - dungeon-clear
  - raid-automation
  - eluna
  - formations
  - attunements
  - teleports
  - encounters
  - bot-tanks
  - turtle-wow
---

# 06. Automated Raid & Dungeon Operations Handbook

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + mod-playerbots + mod-dungeon-clear + Eluna
> **Engine Verification**: `run/lua_scripts/raid_automation.lua` (**v1.4.0** — `local SCRIPT_VERSION = "1.4.0"` at `run/lua_scripts/raid_automation.lua:632`), `PlayerbotMgr.cpp`, `src/game/Chat/Chat.cpp`, `DungeonClearCommand.cpp`
> **Security Requirement**: Accessible to all players (`SEC_PLAYER` / Level 0) with elevated GM features where noted
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Playerbot Handbook](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md)

> ⚠️ **Read `run/lua_scripts/raid_automation.lua`, and only that file, when you audit the script.**
> `run/mangosd.conf` sets `Eluna.ScriptPath = "./lua_scripts"`
> **relative to the `run/` working directory**, so `run/` is the only directory
> Eluna ever loads, and `find . -name raid_automation.lua` returns exactly
> **one** hit: `run/lua_scripts/raid_automation.lua` (**v1.4.0**). There is no
> `lua_scripts/` directory at the repo root and none in git `HEAD`
> (`git show HEAD:lua_scripts/raid_automation.lua` → *fatal: path does not exist*).
> If a future `git checkout`, branch switch or merge ever restores a
> repo-root `lua_scripts/raid_automation.lua`, **delete it before auditing
> anything** — an older v1.1.0 tree lived there and lacks `tank3` / `tank4` /
> `tanks` / `marks` / `ccmark` / `resume` / `gather` / `avoid` / `roll`
> entirely, which produces a large set of false "this command does not exist"
> findings.

---

## 📑 Table of Contents

1. [Architectural Overview & Core Philosophy](#1-architectural-overview--core-philosophy)
   - [Core Design Rules](#core-design-rules)
2. [Master Macro Cheat Sheet (Copy-Paste Ready)](#2-master-macro-cheat-sheet-copy-paste-ready)
    - [🌟 Macro 0: "Assemble + Prep"](#-macro-0-assemble--prep-the-one-you-actually-start-from) — **start here**
    - [🌟 Macro 1: "1-Click Raid Ready"](#-macro-1-1-click-raid-ready) · [⚔️ Macro 2: "Dungeon Autopilot Engaged"](#-macro-2-dungeon-autopilot-engaged)
    - [⏸️ Macro 3: "Party Hold / Drink Break"](#-macro-3-party-hold--drink-break) · [🚪 Macro 4: "Open Door / Focus Skull"](#-macro-4-open-door--focus-skull)
3. [The Master 1-Click Pipeline (.raid ready)](#3-the-master-1-click-pipeline-raid-ready)
   - [Ready Subcommands](#ready-subcommands)
4. [Instant Raid Attunements & Master Keyring Suite](#4-instant-raid-attunements--master-keyring-suite)
    - [Completed Attunement Quests](#completed-attunement-quests) · [Master Keyring Items Spawned in Bags](#master-keyring-items-spawned-in-bags)
5. [Autonomous Dungeon Clear (.dc) Operations Deck](#5-autonomous-dungeon-clear-dc-operations-deck)
    - [Navigation & Autopilot Commands](#navigation--autopilot-commands)
    - [5.1 Complete `.dc` Command Reference (Engine-Verified)](#51-complete-dc-command-reference-engine-verified) — **read this before testing a raid**
       - [🚨 Read this first: how to address a raid of bots](#-read-this-first-how-to-address-a-raid-of-bots)
       - [⚠️ `.dc test` cannot run raids](#-dc-test-cannot-run-raids) · [The two halves of `.dc`](#the-two-halves-of-dc)
       - [Live Commands](#live-commands) · [`.dc test` Commands (GM only)](#dc-test-commands-gm-only) · [Complete token list (68)](#complete-token-list-68)
       - [`.dc go` reference for Ahn'Qiraj (maps 509 / 531)](#dc-go-reference-for-ahnqiraj-maps-509--531)
       - [`.raid pull` and `.dc pause` — the forms that actually work](#raid-pull-and-dc-pause--the-forms-that-actually-work)
       - [`.raid tele` alias audit](#-raid-tele-alias-audit--several-were-landing-in-the-wrong-place)
       - [Recommended raid test sequence](#recommended-raid-test-sequence)
       - [Runbook: Onyxia's Lair](#runbook-onyxias-lair-the-shortest-one--start-here) · [`.raid` Command Reference (Eluna)](#raid-command-reference-eluna-engine-verified) · [Runbook: AQ](#runbook-ruins--temple-of-ahnqiraj)
6. [In-Dungeon Recovery, Doors, AoE & Target Focus](#6-in-dungeon-recovery-doors-aoe--target-focus)
    - [Tactical Bot Directives](#tactical-bot-directives)
7. [Automatic Role & Formation Matrix](#7-automatic-role--formation-matrix)
    - [Why formations are the whole job](#why-formations-are-the-whole-job) · [Quota Allocation by Group Size](#quota-allocation-by-group-size)
    - [Why the script does not need to know anyone's spec](#why-the-script-does-not-need-to-know-anyones-spec)
    - [Markers](#markers) · [Tank marking — one icon per tank](#tank-marking--one-icon-per-tank) · [Stances](#stances) · [Picking a 3rd / 4th off-tank](#picking-a-3rd--4th-off-tank)
8. [Master Instance Fast-Teleport Directory](#8-master-instance-fast-teleport-directory)
    - [Classic Raids & Dungeons](#classic-raids--dungeons) · [Turtle-WoW Custom Raids & Dungeons](#turtle-wow-custom-raids--dungeons)
9. [C++ Engine Contracts & Technical FAQ](#9-c-engine-contracts--technical-faq)
    - [Why `.bot gear * epic` instead of `epics?`](#why-bot-gear--epic-instead-of-epics) · [Why does `.bot prep *` take no quality argument?](#why-does-bot-prep--take-no-quality-argument)
    - [Why does `.dc on` require a tank bot?](#why-does-dc-on-require-a-tank-bot)
    - [⚠️ Greater Blessing of Kings vs. multiple blessings — a rule with no caller](#-greater-blessing-of-kings-vs-multiple-blessings--a-rule-with-no-caller)
10. [Blackwing Lair Automated Raid Operations](#10-blackwing-lair-automated-raid-operations--technical-reference)
    - [10.1 Quick-Start 1-Click BWL Deployment](#101-quick-start-1-click-bwl-deployment)
    - [10.2 Encounter Operations & Engine Automations](#102-encounter-operations--engine-automations)
    - [10.3 Cross-Instance Scripting Audit & Proven Tactical Patterns](#103-cross-instance-scripting-audit--proven-tactical-patterns-beyond-mc--zg)
11. [Onyxia's Lair Automated Raid Operations](#11-onyxias-lair-automated-raid-operations--technical-reference)
    - [11.1 Quick-Start 1-Click Onyxia Deployment](#111-quick-start-1-click-onyxia-deployment)
    - [11.2 Architectural Integration in mod-dungeon-clear & mod-playerbots](#112-architectural-integration-in-mod-dungeon-clear--mod-playerbots)
    - [11.3 Phase-by-Phase Encounter Operations & Critical Mechanics](#113-phase-by-phase-encounter-operations--critical-mechanics)
12. [10-Man Raids & Custom Dungeons Operations Guide](#12-10-man-raids--custom-dungeons-operations-guide)
    - [12.1 Lower Karazhan Halls (Map `532`)](#121-lower-karazhan-halls-map-532-10-man-raid)
    - [12.2 Upper Karazhan Halls (Map `814`)](#122-upper-karazhan-halls-map-814-40-man-raid)
    - [12.3 Scarlet Citadel (Map `45`, 40-Man Raid Fortress)](#123-scarlet-citadel-map-45-40-man-raid-fortress)
    - [12.4 Karazhan Crypt (Map `800`)](#124-karazhan-crypt-map-800-5-man-level-5860-dungeon)
13. [The `.raid` Operator Facade (v1.4.0)](#13-the-raid-operator-facade-v140) — **the full five-layer command surface, the `@filters`, and the trap register**
    - [13.1 What changed, and why the facade exists](#131-what-changed-and-why-the-facade-exists)
    - [13.2 ⚔️ BATTLE — run control](#132--battle--run-control) · [13.3 📋 ROSTER](#133--roster--who-is-here-and-what-they-can-do) · [13.4 🎚️ TOGGLES](#134--toggles--every-switch-now-has-an-off-form)
    - [13.5 🚚 MOVE](#135--move--soft-regroup-versus-hard-teleport) · [13.6 🔓 PASS](#136--pass--escape-hatches-to-the-raw-engines) · [13.7 🏷️ FILTERS](#137--filters--the-token-prefixes)
    - [13.8 ❓ HELP](#138--help--the-index-the-layers-and-the-typo-hint) · [13.9 🚨 Traps](#139--traps--strings-that-look-like-they-worked-and-did-not)

---

## 1. Architectural Overview & Core Philosophy

This operations handbook documents the **Tortoise-WoW Autonomous Raid Controller**, an enterprise-grade Eluna automation layer designed to eliminate manual command micromanagement during 5-man dungeon runs and 10/20/40-man raids.

### Core Design Rules

- **No Level Tampering**: Bots and characters invited via the in-game group finder (LFT / LFG) are already level 60. The controller never alters character levels.
- **Player-Controlled Group Topology**: In Vanilla WoW, a standard group is capped at 5 members. Raids hold 6 to 40 members across 8 subgroups. Group-to-raid conversion is handled by the player via the in-game UI. The controller never forces group conversions.
- **Engine-Verified Syntax**: All commands dispatch directly into the CMaNGOS command handler (`PlayerbotMgr.cpp`, `DungeonClearCommand.cpp`) using native server tokens. ⚠️ **There is no `Commands.cpp` in this tree** — the base chat table is `src/game/Chat/Chat.cpp` and the playerbot handlers are `modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp`. Client chat prefixes (`/p`, `/ra`) are never sent into server command queues.
- **Raid Attunement vs Quest Sharing**: Generic quest sharing does not grant raid access. The controller executes verified quest completions and spawns physical access keys for both Classic raids and Turtle-WoW custom content.

---

## 2. Master Macro Cheat Sheet (Copy-Paste Ready)

Create these 5 macros in your World of Warcraft **Macro UI** (`/m`) and place them on your action bar.

### 🌟 Macro 0: "Assemble + Prep" (the one you actually start from)

*Run this first. It assembles the raid.*

```text
.bot group size=20
.raid check
.raid tanks 2
.raid roles
.raid ready
```

*(`.bot group` fills the raid to the class shape its size row calls for. ⚠️ **It does
NOT assign role-matched specs** — see the correction below. `.raid check` then shows
you what every bot actually got, `.raid tanks 2` promotes real protection-specced
bots, `.raid roles` splits formations by role — the thing `.bot group` leaves
undone, since a joining bot inherits the inviter's formation — and focus-locks the
DPS. `.raid ready` gears, enchants, trains, restocks and attunes.)*

> 🚨 **CRITICAL CORRECTION — `.bot group` assigns no role-matched spec.** *(The
> original note under this macro said "`.bot group` picks a role and a class that
> can fill it per bot, then applies a spec matching that role and varies within
> it." **That is wrong. It never forwards the role it picks, so no bot ever gets a
> spec matched to it.**)*
>
> The verified chain, end to end:
> - `PlayerbotMgr.cpp:2825` rolls `BotRoles role = BotRoles(urand(BOT_ROLE_TANK,
>   BOT_ROLE_DPS))`. That `role` is used for exactly **one** thing: picking a class
>   that can fill it, `factory.GetRandomClass(0, role)` at `:2831`.
> - `PlayerbotMgr.cpp:2855` then builds the create string as `level=` `class=`
>   `group=` plus passthrough. **There is no `role=` in it.** `CreateBot`
>   (`PlayerbotHolder::HandleCreate`) therefore keeps its initialiser
>   `BotRoles role = BotRoles::BOT_ROLE_NONE;` (`:2536`) — the *only* place that
>   ever writes it is the `else if (key == "role")` parser branch at `:2562-2563`,
>   which nothing here reaches.
> - `PlayerbotMgr.cpp:2680` calls `ChangeTalentsAction::AutoSelectTalents(newBot,
>   &out, role)` with that `BOT_ROLE_NONE`.
> - `ChangeTalentsAction::getPremadePaths` (`:162`) applies its role filter **only**
>   under `if (role != BotRoles::BOT_ROLE_NONE && ...)` (`:170`). With
>   `BOT_ROLE_NONE` that filter is skipped, so it returns **every premade spec for
>   that class**, and `PickPremadePath` (`:230`, called at `:379`) picks one
>   **at random**.
> - `uint8 groupSize = 5;` at `:2785` is a plain local. Nothing persists it.
>
> **What this means operationally:** the tank/heal/dps numbers in the size row only
> decided **which classes got drawn**. The specs those bots ended up with are
> arbitrary picks from each class's entire premade list. A `size=20` raid can and
> routinely does come out with **zero protection-specced bots** — and
> `PlayerbotAI::IsTank`, which `@tank` filters and `.dc`'s
> `DispatchToTankBots` both read, goes by the *installed spec strategy*. Without
> real tanks, `.dc on` answers `No tank bot found in your group.`
>
> **So verify, don't assume:** run `.raid check` after `.bot group` and look for
> `protection` in each tank candidate's strategy dump, then promote with
> `.raid tanks <n>` — which is the one path that really does re-spec
> (`talents do protection` + `reset ai`, Lua `:1965-1972`). Do **not** run
> `.dc on` before that.

### 🌟 Macro 1: "1-Click Raid Ready"

*For a group you assembled some other way — `.bot add <account>`, or
hand-made characters:*

```text
.raid ready
```

*(Roster census, formations split by role, DPS focus lock, then best-in-slot gear,
enchants, spell training, consumables, attunements and master keys. Append
`epic` for baseline epics instead of best-in-slot.)*

### ⚔️ Macro 2: "Dungeon Autopilot Engaged"

*Run this once inside the dungeon or raid portal:*

```text
.raid start
```

*(Sequences `dc bosses` → `dc pull dynamic` → `dc on` → `dc status`. `dc bosses` is
the real pre-flight: it has no authorization gate, works before `dc on` has ever
been issued, and prints the exact anchor list the run will route through. An
empty list is what `dc on` will refuse on. ⚠️ `dc bosses` is **not** the only verb
with no authorization gate — `dc status` is the other; see the full gate list in
[§13.2](#132--battle--run-control).)*

### ⏸️ Macro 3: "Party Hold / Drink Break"

*Run this when you need to pause the tank, loot, or replenish mana:*

```text
.raid pause
.raid loot
.raid rest
```

*(Halts tank pathfinding, commands bots to vacuum nearby corpse loot, and forces
all bots to eat and drink back to full.)*

> ⚠️ `.raid pause` sends `dc pause pause` — the **explicit intent form**, never the
> bare toggle. The bare form is a `(current+1)%3`-style toggle that races DC's own
> auto-pause sites, so a pause landing just after a door or wait-at-boss
> auto-pause will **resume** instead. Resume with `.raid resume`.

### 🚪 Macro 4: "Open Door / Focus Skull"

```text
.raid open
.raid skull
```

*(`.raid open` commands bots to right-click the nearest gate, lever, chest or
door. `.raid skull` puts the skull icon — icon 7 — on your current target, which
is what the DPS focus-lock follows.)*

> ⚠️ **`.raid focus` is not a strategy command.** It is an alias of `skull` (the
> dispatch is `elseif sub == "skull" or sub == "focus" or sub == "mark"`), and it
> sets icon 7 on your target — it does **not** add `co +mark rti`. The focus
> *lock* is a separate thing: it is
> part of `.raid roles` (`co +focus rti targets`) and is released by
> `.raid unfocus`. Tank-assigned marking is `.raid marks <tank…>`.

---

## 3. The Master 1-Click Pipeline (`.raid ready`)

> ⚠️ **`.bot group` comes first — but it does not give anyone a role-matched
> spec, and treating it as if it does is the single most damaging belief in this
> handbook.**
>
> **The verified behaviour:** `.bot group size=N` picks a role at random
> (`PlayerbotMgr.cpp:2825`) and uses it only to choose a class (`:2831`), then
> forwards **no** `role=` to `HandleCreate` (`:2855`). `CreateBot` keeps
> `BOT_ROLE_NONE` (`:2536`, written only at `:2562-2563`), and
> `AutoSelectTalents` at `:2680` therefore skips its role filter entirely
> (`ChangeTalentsAction::getPremadePaths`, `:170`) and picks a **random premade
> spec for that class** (`PickPremadePath:230`). Full chain in
> the **🚨 CRITICAL CORRECTION** block under [§2 Macro 0](#2-master-macro-cheat-sheet-copy-paste-ready).
>
> **Reversed advice:** `.bot group` is *not* the spec step, and `.raid tanks <n>`
> is the only real one. So the correct order is `.bot group size=N` → **`.raid
> check`** (read the actual spec strategies) → **`.raid tanks <n>`** (make real
> tanks) → `.raid roles` → `.raid ready`. Running `.raid ready` on a group
> assembled some other way (`.bot add <account>`, hand-made characters) is
> equally fine; there the specs are simply whatever those characters already had,
> and `.raid check` tells you what that is.

Executing `.raid ready` runs a 3-stage preparation sequence:

```text
[ .bot group size=20  — fills the roster to the class shape; assigns NO specs ]
           │
           ▼
Stage 0: Verify & Promote  ──► .raid check   (read every bot's real spec strategy)
           │                 ──► .raid tanks <n>  (talents do protection + reset ai)
           ▼
Stage 1: Roster & Formations  ──► Class census + feasibility check on your chat
           │                      Splits formations by role (4 broadcasts):
           │                      @tank shield, @heal far,
           │                      @melee melee, @ranged line
           │                      Focus-locks the DPS: co +focus rti targets
           ▼
Stage 2: Gearing & Stock      ──► Equips gear (.bot gear * <quality>)
           │                      Applies permanent enchants (.bot enchants *)
           │                      Teaches class spells & ranks (.bot train *)
           │                      Restocks consumables, food, potions, reagents, ammo
           │                      Repairs and replenishes (repairitems, replenish)
           ▼
Stage 3: Raid Attunements     ──► Completes MC, BWL, Onyxia, Naxxramas attunement quests
                                  Completes Emerald Sanctum, Karazhan, Timbermaw, Hateforge
                                  Sets Argent Dawn rep to Exalted
                                  Adds 13 physical master keys to your inventory
```

**There is no "Instant Raid Buffs" stage, and adding one would be a regression.**
A per-class aura table inside the script is **actively harmful**: it sends
`water shield` to *every* shaman, overwriting the engine's correct per-spec
choice (enhancement gets *lightning shield*, restoration gets *water shield*).

The underlying need is already met by the engine — `buff` is in every default
combat kit and each spec strategy wires its own aura — and now that the spec
strategy is actually installed, the engine buffs correctly on its own. Any such
table in a script would break it again. `.raid buff` therefore **sends nothing**
and reports how this works instead.

### Ready Subcommands

```lua
-- roles + gear (quality best) + attune
.raid ready

-- Same, but gears baseline epics instead of best-in-slot
.raid ready epic

-- Re-gear and re-enchant without re-running attunements (quality: best, green/uncommon, blue/rare, purple/epic, partial, upgrade, sync)
.raid gear [quality]

-- Roster census + formations split by role + DPS focus lock (sends no talent commands)
.raid roles

-- 3rd-tank override: promotes your current target (needs target or name, never guesses)
.raid tank3

-- Promote to a 4-tank roster (add a name to choose who)
.raid tank4 [name]

-- Configure tank count (1-8); the only re-spec path
.raid tanks <1-8>

-- Assign one mark icon per named tank in priority order
.raid marks <tank1> [tank2...]

-- Buff explanation report (sends nothing; reports engine spec auto-buff behavior)
.raid buff
.raid worldbuff
.raid wbuff
```

---

## 4. Instant Raid Attunements & Master Keyring Suite

Running `.raid attune` (or `.raid ready`) guarantees your character and raid can enter every raid instance and open every dungeon door across Azeroth.

### Completed Attunement Quests

```lua
-- Molten Core: Attunement to the Core (Quest ID: 7848, Lothos Riftwaker)
-- Blackwing Lair: Blackhand's Command (Quest ID: 7761, Scarshield Quartermaster)
-- Onyxia's Lair (Alliance): Drakefire Amulet (Quest ID: 6502, Jailbreak / Marshall Windsor)
-- Onyxia's Lair (Horde): Blood of the Black Dragon Champion (Quest ID: 6602, Rexxar / General Drakkisath)
-- Naxxramas (Exalted): The Dread Citadel - Naxxramas (Quest ID: 9123, Archmage Angela Dosantos)
-- Emerald Sanctum (40-Man): Into the Dream VI (Quest ID: 40962, Turtle-WoW Custom 40-Man)
-- Tower of Karazhan (10/40-Man): The Key to Karazhan X (Quest ID: 40829, Turtle-WoW Custom Raid)
-- Timbermaw Hold (20-Man): The Corruption of Timbermaw Hold (Quest ID: 41931, Turtle-WoW Custom 20-Man)
-- Hateforge Quarry (5-Man): Assaulting Hateforge (Quest ID: 40489, Turtle-WoW Custom Dungeon)
```

*Note: The script also sets Argent Dawn reputation to Exalted (`.modify rep 529 42999`), satisfying the free Naxxramas entrance prerequisite.*

### Master Keyring Items Spawned in Bags

```lua
-- Onyxia's Lair Raid Entrance Key
.additem 16309 1 -- Drakefire Amulet

-- Molten Core Permanent Infinite Rune Dousing
.additem 22754 1 -- Eternal Quintessence

-- Upper Blackrock Spire (UBRS 10-Man) Entrance Gate
.additem 12344 1 -- Seal of Ascension

-- Scholomance Front Gate Key
.additem 13704 1 -- Skeleton Key

-- Stratholme Service Entrance Gate Key
.additem 12382 1 -- Key to the City

-- Blackrock Depths Gates & Mechanism Key
.additem 11000 1 -- Shadowforge Key

-- Dire Maul West & North Gates Key
.additem 18249 1 -- Crescent Key

-- Scarlet Monastery Armory & Cathedral Doors
.additem 7146 1 -- The Scarlet Key

-- Gnomeregan Back Entrance Key
.additem 9287 1 -- Workshop Key

-- Emerald Sanctum (40-Man) Raid Key
.additem 50545 1 -- Gemstone of Ysera

-- Tower of Karazhan (10/40-Man) Raid Key
.additem 61234 1 -- Upper Karazhan Tower Key

-- Karazhan Crypts (5-Man) Dungeon Key
.additem 51356 1 -- Karazhan Crypt Key

-- Timbermaw Hold (20-Man) Raid Key
.additem 42379 1 -- Rune of Illumination

-- Scarlet Citadel (10-Man) Raid Key
.additem 53005 1 -- Scarlet Crusade Insignia
```

---

## 5. Autonomous Dungeon Clear (`.dc`) Operations Deck

The autonomous solver engine (`mod-dungeon-clear`) drives your party's tank bot through the optimal path of any dungeon or raid instance.

### Navigation & Autopilot Commands

```lua
-- Engage autonomous dungeon clear engine and dynamic pulling (.dc on + .dc pull dynamic)
.raid start
.raid dc

-- Completely disable autonomous dungeon clear engine (.dc off)
.raid off
.raid stop

-- Pause tank bot movement and drop combat (.dc pause pause; resume with .raid resume)
.raid pause
.raid resume

-- Set advanced-pull LOS pull-to-camp mode: off (melee in place), on (drag to camp), dynamic (decide per pack)
.raid pull [off|on|dynamic]

-- Direct tank bot straight to a specific boss (e.g. .raid go Lucifron, .raid go Magmadar)
.raid go <boss_name>

-- Abandon current stuck pack or optional boss and route to next objective
.raid skip

-- Print live checklist of all bosses with Alive/Dead status
.raid bosses

-- Display completion percentage, active route segment, and current path target
.raid status

-- Inspect live solver tunables (pull radius, leash duration, rest thresholds)
.raid config

-- Activate cinematic spectator camera (modes: follow <name>, next, prev, list, bare free-fly)
.raid spectate [follow <name>|next|prev|list]
```

---

## 5.1 Complete `.dc` Command Reference (Engine-Verified)

*Source of truth: `modules/mod-dungeon-clear/src/DungeonClearCommand.cpp`, cross-checked against `DungeonClearChatActions.cpp`, `TestRun/DcTestDungeonRegistry.cpp`, and `run/lua_scripts/raid_automation.lua`. Everything below is read from code, not assumed.*

### 🚨 Read this first: how to address a raid of bots

```lua
-- One named bot (CHAT_MSG_WHISPER): reaches one bot
/w <bot> <cmd>
.bot w <bot> <cmd>

-- Every bot in raid (CHAT_MSG_RAID): reaches ALL 40 bots with a single message
/ra <msg>
.bot r <msg>

-- Subgroup in raid only (CHAT_MSG_PARTY): reaches party subgroup only
/p <msg>
.bot p <msg>

-- Debug stub (does not reach bots):
-- .bot c <bot> <cmd>
```

**`.bot r` works — no core change, no rebuild.** The reason it works is not the
obvious one: `.bot r` does not message the bots at all. It
builds a `CMSG_MESSAGECHAT` on **your own session**, `HandleMessagechatOpcode`
turns that into `PLAYERHOOK_ON_CHAT_COMMAND`, and
`PlayerbotMgr::HandleCommand` — which has **no channel allowlist**, only
name/distance narrowing for whisper, `/say` and `/yell` — dispatches it to every
bot. Each bot then runs the chat filter for itself, which is what makes
`.bot r @tank co +protection` hit only the tanks.

`CHAT_MSG_RAID` genuinely *is* missing from the accepting set in
`PlayerbotAI::ProcessIncomingChatPacket`, but that function is the **reactive**
path — bots reacting to chat they receive — not the ordering path. It only
matters if you use bot chat reactions or `sPlayerbotAIConfig.llmEnabled`, and
with the LLM off it buys you nothing. Full trace in
[Volume 02](./02_playerbot_and_raid_automation.md) → *There is NO chat channel
that reaches a whole raid of bots*.

`.bot c` is the genuinely broken one, and it still is: it calls
`DoSpecificAction("cdebug", Event(".bot", "monstertalk " + cmd))`, and
`DebugAction` only prints `Usage: debug monstertalk <text>` before returning
`"ok"` — so it *reports success and does nothing*. This was the single defect
that made the previous version of `raid_automation.lua` completely inert.

**Broadcast by default, whisper only what has to be whispered.** The `.raid`
commands emit one `.bot r` line per universal order, and one `.bot r @role ...`
line per role order — five broadcasts for `.raid roles`, not forty whispers.

**The only things still whispered per bot are the two that genuinely differ per
bot**, and both are on `.raid tank3` / `.raid tank4` / `.raid marks`:

- `talents do <spec>` + `reset ai` — only from the tank commands, and only for the
  bots being promoted. `.raid roles` sends none. ⚠️ **`.bot group` does not assign
  role-matched specs** (`HandleGroup` never forwards `role=`, so
  `AutoSelectTalents` gets `BOT_ROLE_NONE` and picks a random premade spec — see
  the Macro 0 correction). **The real reason `.raid roles` sends none is that it
  does not know which bots
  need promoting** — it sends no talent command at all, on purpose. Which means the
  tank commands are not a formality you can skip: they are the **only** place any
  bot in this pipeline ever gets a role-matched spec.
- `rti <icon>` + `co +mark rti` — only in `.raid marks <tank…>`, because each
  tank needs a *different* icon and no filter can express "tank #2 gets cross".

The role filters do **not** require the roles to exist first. `@tank` resolves
through `PlayerbotAI::IsTank`, which reads the installed spec strategy, so the
*mechanism* works with no prior setup step. But ⚠️ **do not conclude that this
makes the formation split safe on the first call** — `.bot group` does not build a
spec, so a bot it created may resolve as **not** a tank. The formation split will
then silently assign those bots `@ranged` or `@melee` instead of `shield`.
**Verify with `.raid check` and promote with `.raid tanks <n>` before
`.raid roles`**, or you are formatting a raid that nobody is tanking.

Use a `bot` subcommand that accepts `*` (`.bot gear * best`,
`.bot consumes *`, …) for server-side iteration.

### ✅ `.dc test` Raid & 10-Man Support

`.dc test` now fully supports both 5-man dungeons and **10-man, 20-man, and 40-man raids**!
The test comp generator automatically scales party sizes according to the instance template:
- **10-man Raids / Dungeons**: `brs` / `ubrs` (Map 229), `lkh` (Lower Karazhan Halls, Map 532).
- **20-man Raids**: `zg` (Zul'Gurub, Map 309), `aq20` (Ruins of Ahn'Qiraj, Map 509).
- **40-man Raids**: `mc` (Molten Core, Map 409), `bwl` (Blackwing Lair, Map 469), `onyxia` (Map 249), `aq40` (Temple of Ahn'Qiraj, Map 531), `naxx` (Map 533), `scarlet-citadel` (Map 45), `sanctum` / `emerald-sanctum` (Emerald Sanctum, Map 807), and `kara-upper` (Upper Karazhan, Map 814).
- **Custom / High-Level Dungeons**: `kara-crypt` (Karazhan Crypt, Map 800), `strat` (Stratholme, Map 329), `scholo` (Scholomance, Map 289), `crescent`, `stormwrought`, `gilneas`, `hateforge`, `sw-vault`.

To test any raid or dungeon unattended:
```text
.dc test start <token> [quality=epic] [ilvl=N]
.dc test watch
```
For manual / player-led raids, you can still use the live driver:
```text
.raid roles
.raid start
.dc status
.dc bosses
.dc spectate follow
```

### The two halves of `.dc`

```lua
-- ============================================================================
-- THE TWO HALVES OF .dc (LIVE COMMANDS VS .dc test HARNESS)
-- ============================================================================

-- Live commands:
--   Who may run it: Any player (SEC_PLAYER)
--   Where:          In-game session only
--   Who it drives:  The tank bot in your group
--   Content:        Any map DC supports — including all raids
--   Purpose:        You supervise a real run

-- .dc test harness:
--   Who may run it: GM only (SEC_GAMEMASTER)
--   Where:          In-game or the server console
--   Who it drives:  Its own generated party (you are not in it)
--   Content:        68 tokens: 5-man dungeons plus the raid-flavoured scarlet-citadel
--   Purpose:        Unattended batch regression runs
```

Every live command routes through `DungeonClearDispatch::DispatchToTankBots`, which searches your group for a bot holding the **tank** role. With no tank bot you get `No tank bot found in your group.`

#### 🚨 That role does NOT come from `.bot group`. Read this before you trust `.dc on`

**What actually happens.** `IsTank` reads the installed spec strategy, yes. But
`.bot group` does not install a tank spec, because it never forwards the role it
picks: `HandleGroup` rolls `BotRoles role = urand(BOT_ROLE_TANK, BOT_ROLE_DPS)`
at `PlayerbotMgr.cpp:2825` and uses it **only** to choose a class (`:2831`), then
builds the create string as `level=` `class=` `group=` plus passthrough at `:2855`
— **no `role=`**. `HandleCreate` therefore holds `BOT_ROLE_NONE` (`:2536`, written
only by the `else if (key == "role")` branch at `:2562-2563`), and
`AutoSelectTalents(newBot, &out, role)` at `:2680` runs with it.
`ChangeTalentsAction::getPremadePaths` skips its role filter entirely when
`role == BOT_ROLE_NONE` (`:170`), so it returns **every premade spec that class
has**, and `PickPremadePath` (`:230`) picks one **at random**.

**The consequence you must plan around:** a `size=20` raid can and routinely will
come out with **ZERO protection-specced bots**. The tank/heal/dps figures in the
size row only chose **which classes were drawn** — the specs are arbitrary picks
from each class's whole premade list. So the tank role in a `.bot group`-assembled
raid comes from **whichever random premade spec happened to land**, which is a
coin flip, not a design. `.dc on` will then answer
`No tank bot found in your group.`, and if one bot *does* land on protection by
chance, `@tank` filters and formations will bind to it while the rest of your
tank-capable roster sits in `@ranged`.

> ⚠️ **The script itself still tells you the false thing — and it is a source
> file, so this is a note, not a fix.** `ApplyRaidSetup` ends with a `Say()` at
> `raid_automation.lua:1852-1853`:
>
> ```text
> Formations split by role, DPS focus-locked. Specs and roles were already
> set by `.bot group` and were left alone.
> ```
>
> **That second sentence is false.** It is live in v1.4.0, so expect to read it
> every time you run `.raid roles`, and do not treat it as confirmation. The first
> sentence is accurate. The Lua is a
> source file and is left untouched; correcting the message belongs with whoever
> changes the script, and it should be corrected in the same pass as any real fix
> to the `HandleGroup` role forwarding. (The same applies to the header comment
> over the tank-promotion branch at `:1859-1863`, which asserts "`.bot group
> size=N` fixes the tank count to that row" — true of the *class quota*, false as
> a statement about specs. The branch's own first line, "the one place that
> genuinely re-specs", **is** correct, and is the finding this whole correction
> rests on.)

**What to do instead, in this order:**
```text
.bot group size=20     -- fills the roster; assigns NO specs
.raid check            -- prints every bot's live spec strategy; look for "protection"
.raid tanks 2          -- promotes 2 real tanks: talents do protection + reset ai
.raid roles            -- only now are @tank/@heal filters meaningful
.raid ready            -- gear, enchants, consumables, attunements
.dc bosses             -- confirm the anchor list
.dc on                 -- now it has a tank to drive
```

`.raid tanks <n>` is the **only** re-spec path in the whole pipeline, and it does
send the real thing: `talents do protection` followed by `reset ai`
(`run/lua_scripts/raid_automation.lua:1965-1972`, in v1.4.0 queued as two **ordered
deferred steps** so the respec and the kit rebuild cannot be separated). It needs a
target or a name — it never guesses, and `.raid roles` lists the tank-capable
candidates for you to paste in.

If you assembled with `.bot add <account>` or hand-made characters instead, the
same check applies: confirm at least one Warrior, Paladin or Feral Druid is
actually protection/feral-specced (and remember the level-60 druid cat/bear coin
flip below), then promote if not.

A second gate sits behind that: only the **DC leader** (the lead tank) actually answers. Non-leaders stay silent rather than replying in duplicate.

### Live Commands

```lua
-- ============================================================================
-- LIVE DUNGEON CLEAR COMMANDS (.dc)
-- ============================================================================

-- .dc on
-- Enables the run: installs advance/engage/stall trigger ladder, resets per-run state,
-- arms status pusher. Refuses if a party member is dead and cannot be recovered.
-- Applies any pull mode pre-set with .dc pull.
.dc on

-- .dc off
-- Disables the run and tears down strategy.
.dc off

-- .dc pause
-- Pauses the tank. Auto-resumes on its own if a door is later opened (DungeonClearDoorReopenedTrigger).
-- You do not have to hit resume manually.
.dc pause

-- .dc skip
-- Abandons current stuck pack / optional boss and routes to next objective.
.dc skip

-- .dc pull [off|on|dynamic|dyn]
-- Advanced-pull (LOS pull-to-camp) mode:
--   off:     Melees pack in place.
--   on:      Always drags pack out to a camp.
--   dynamic: (synonym 'dyn') decides per pack.
-- Bare call cycles off -> on -> dynamic -> off at engine level (DungeonClearChatActions.cpp:1206-1216).
-- WARNING: The .raid wrapper clamps unrecognised or empty words to dynamic first, so bare .raid pull
-- sets dynamic. Use '.raid dc pull' for the engine bare toggle. Settable before .dc on.
.dc pull dynamic
.dc pull off
.dc pull on

-- .dc status [addon|silent]
-- Status report: "Dungeon clear: on/off. Next boss: <name>. Skipped: <n>." plus latched stall reason.
-- Server also pushes status packets automatically on state changes.
.dc status
.dc status addon
.dc status silent

-- .dc bosses [addon|silent]
-- Full boss checklist with Alive/Dead state. Works even before .dc on has ever been issued
-- (resets stale completion latches so re-entered instances don't show phantom "Done" rows).
.dc bosses

-- .dc go <boss name or entry>
-- Overrides routing to specific boss. Accepts numeric creature entry or case-insensitive name substring.
-- Requires active run, else errors: "Dungeon clear is not enabled. Please enable it first."
.dc go 15509
.dc go Kurinnaxx

-- .dc config
-- Dumps every DungeonClear tunable as AI actually reads it this tick.
-- '*' = active per-run override, 'H' = heroic-mode default.
-- Confirms run/mod_dungeon_clear.conf edits took effect without .reload config.
-- (Template is DcSettingsRegistry.h:11; Logger.playerbots.dungeonclear.pull in DungeonClearUtil.h:36.
-- Nothing under modules/mod-dungeon-clear/src/ reads worldserver.conf.)
.dc config

-- .dc spectate [follow [name]|next|prev|list]
-- Camera control:
--   bare:          Free-fly mode.
--   follow:        Toggle ride bot (bare 'follow' turns off; 'follow <name>' jumps to named bot).
--   next / prev:   Walk every bot in instance, not just tanks.
--   list:          Shows roster.
-- Acts on YOU, not the tank (works even if not tank and outside party).
.dc spectate
.dc spectate follow
.dc spectate follow Tankbot
.dc spectate next
.dc spectate prev
.dc spectate list
```

### `.dc test` Commands (GM only)

```lua
-- ============================================================================
-- .dc test COMMANDS (GM ONLY - SEC_GAMEMASTER)
-- ============================================================================

-- .dc test list
-- Prints all 68 supported tokens with name, map id, and level. Run this first (live authority).
.dc test list

-- .dc test start <dungeon> [heroic] [level=N] [seed=N] [ilvl=N|none] [quality=normal|uncommon|rare|epic|legendary]
-- Rolls a random party from addclass pool, teleports it in, gears it, and runs.
-- seed=N replays one specific comp. ilvl=/quality= cap gear (defaults from AiPlayerbot.AutoGear* conf).
-- 'heroic' runs heroic variant.
.dc test start deadmines level=20 quality=rare
.dc test start scarlet-citadel seed=42

-- .dc test start <dungeon> party=Tank,Heal,Dps1,Dps2,Dps3 [heroic]
-- Hand-picks party of real player characters instead. Roles are positional.
-- level=, seed=, ilvl=, quality= are REJECTED (characters are never relevelled or re-geared).
.dc test start scholo party=MainTank,HolyHeal,MageOne,RogueTwo,LockThree

-- .dc test status
-- Run status, plus active plan status when a plan is running.
.dc test status

-- .dc test stop [selector]
-- Stops a run:
--   bare:          Stops single active run (errors if more than one).
--   all:           Stops all active runs AND every active plan first (prevents scheduler relaunch).
--   <runId>:       Stops exact run ID.
--   <dungeon>:     Stops all runs of that dungeon token.
.dc test stop
.dc test stop all
.dc test stop deadmines

-- .dc test gear <dungeon> [heroic]
-- Lists ilvl= ceilings worth running that dungeon at.
.dc test gear deadmines
.dc test gear scholo

-- .dc test watch [selector|next|off]
-- Puts camera on run: hides you (visible GM invalidates run), binds + teleports you to entrance,
-- and arms follow camera on arrival.
--   next: Tours a plan's concurrent runs.
--   off:  Ends watch, returns you to where you were, drops created instance binds. Needs session.
.dc test watch
.dc test watch next
.dc test watch off

-- .dc test plan <start|status|stop>
-- Batched campaigns: N runs with capped concurrency.
-- Does not need issuer up front (console start that kicks off driver login works).
.dc test plan start
.dc test plan status
.dc test plan stop
```

A numeric **mapId** also works as a token, but only when that map has exactly one row — on wing-split maps it deliberately refuses, so Dire Maul demands `dm-east` / `dm-west` / `dm-north`.

### Complete token list (68)

```text
rfc  frostmane  wc  deadmines  sfk  dragonmaw  stockade  bfd  windhorn  rfk
sm-gy  gnomer  sm-lib  sm-arm  sm-cath  rfd  uldaman  zf  maraudon  st
brd  brs  crescent  stormwrought  gilneas  hateforge  kara-crypt  sw-vault
black-morass-tw  dm-east  dm-west  dm-north  scholo  strat  zg
scarlet-citadel
ramparts  blood-furnace  slave-pens  underbog  mana-tombs  auchenai  sethekk
old-hillsbrad  shadow-labs  steamvault  shattered-halls  black-morass
botanica  mechanar  arcatraz  mgt  uk  nexus  an  ok  dtk  vh  gundrak
hos  hol  cos  oculus  up  toc  fos  pos  hor
```

**No vanilla raid tokens exist.** There **is** exactly one raid-flavoured row —
`scarlet-citadel`, map `45`, the Turtle-WoW 40-Man Raid Fortress described in
[§12.3](#123-scarlet-citadel-map-45-40-man-raid-fortress): a 40-man raid with
seven scripted bosses, not a 5-man. Everything vanilla — AQ20, AQ40, MC, BWL, Naxx,
Onyxia, Karazhan — has no token at all, and for those you use the live commands
above.

### `.dc go` reference for Ahn'Qiraj (maps 509 / 531)

Verified against `tw_world` — use these with `.dc go <entry>` to jump the run to a specific encounter. This core's entries differ from some classic databases, so **use these, not remembered ones** (e.g. Huhuran is `15509` here, and `15605` is Elder Riversong).

**Ruins of Ahn'Qiraj (map 509)**

```lua
-- ============================================================================
-- RUINS OF AHN'QIRAJ (MAP 509) - .dc go ENTRY REFERENCE
-- ============================================================================

-- Entry 15348: Kurinnaxx
.dc go 15348
.dc go Kurinnaxx

-- Entry 15341: General Rajaxx (Passive/invincible on spawn — 7-officer chain gates him)
.dc go 15341
.dc go General Rajaxx

-- Entry 15370: Buru the Gorger
.dc go 15370
.dc go Buru the Gorger

-- Entry 15340: Moam
.dc go 15340
.dc go Moam

-- Entry 15369: Ayamiss the Hunter
.dc go 15369
.dc go Ayamiss the Hunter

-- Entry 15339: Ossirian the Unscarred (End boss; weakness crystals are a mid-fight click)
.dc go 15339
.dc go Ossirian the Unscarred
```

**Temple of Ahn'Qiraj (map 531)**

```lua
-- ============================================================================
-- TEMPLE OF AHN'QIRAJ (MAP 531) - .dc go ENTRY REFERENCE
-- ============================================================================

-- Entry 15263: The Prophet Skeram (First boss; guarded by sentinel room)
.dc go 15263
.dc go The Prophet Skeram

-- Entry 15544 / 15543 / 15511: Vem / Yauj / Kri (Bug Trio — all three must die; deaths heal survivors)
.dc go 15544
.dc go 15543
.dc go 15511

-- Entry 15516: Battleguard Sartura
.dc go 15516
.dc go Battleguard Sartura

-- Entry 15510: Fankriss the Unyielding
.dc go 15510
.dc go Fankriss the Unyielding

-- Entry 15299: Viscidus (Side room between Fankriss and Huhuran)
.dc go 15299
.dc go Viscidus

-- Entry 15509: Princess Huhuran
.dc go 15509
.dc go Princess Huhuran

-- Entry 15275 / 15276: Emperor Vek'nilash / Vek'lor (Twin Emperors; linked pair — one death ends fight)
.dc go 15275
.dc go 15276

-- Entry 15517: Ouro (No static spawn / 0 rows; summoned by Ouro Spawner 15957; will NOT appear in .dc bosses)
.dc go 15517

-- Entry 15727: C'Thun (End boss; body is non-selectable until Eye dies)
.dc go 15727
.dc go C'Thun

-- Entry 15589: Eye of C'Thun (No static spawn / 0 rows — summon; Phase 1 only)
.dc go 15589

-- Entry 15957: Ouro Spawner (Not a boss; the NPC that summons 15517 Ouro)
.dc go 15957
```

Practical consequence: because Ouro and the C'Thun Eye have no spawn rows, they are driven by the DungeonClear **event** pipeline rather than the plain boss list. If `.dc bosses` looks like it is missing them, that is expected — the objective anchors show up instead, and `.dc status` reports what the run is actually walking toward.

### `.raid pull` and `.dc pause` — the forms that actually work

```lua
-- ============================================================================
-- .raid pull AND .dc pause REALITY
-- ============================================================================

-- .raid pull [dynamic|leeroy|advanced]
-- Reality: Only 'off', 'on', and 'dynamic' exist. raid_automation.lua clamps unrecognised
-- words to 'dynamic' before forwarding to 'dc pull', so '.raid pull leeroy' silently sets dynamic.
-- 'leeroy' is a pull-decision outcome, not a settable mode (dynamic auto-decides Leeroy vs pull per pack,
-- governed by PullDynamicMaxLeeroyMobs ceiling). 'advanced' is the other half of that decision.
.raid pull dynamic
.raid pull off
.raid pull on

-- .raid pull ("Forces an immediate tank pull")
-- Reality: It does NOT pull. It only sets the advanced-pull mode for the rest of the run.

-- .dc pause
-- Reality: Real engine verb. .raid pause / .raid resume wrap it with explicit intent forms:
-- 'dc pause pause' and 'dc pause resume' (never bare toggle, which races DC auto-pause sites).
-- NOTE: 'stop' and 'hold' are NOT aliases in v1.2.0 or v1.3.0. In v1.3.0 'stop' aliases 'dc off' (stops run);
-- there is no 'hold' command at all.
.raid pause
.raid resume
.dc pause

-- dc test scarlet-citadel
-- Reality: Correct. scarlet-citadel (map 45) is a real token, level 63, no heroic — the only raid entry.
.dc test start scarlet-citadel
```

### Recommended raid test sequence

```text
-- 1. Assemble (fills the roster; assigns NO specs), then verify, promote, gear
.bot group size=20
.raid check            -- what specs did they actually get?
.raid tanks 2          -- the only real re-spec: talents do protection + reset ai
.raid roles
.raid ready

-- 2. Teleport + collect
.raid tele aq40
.bot summon *

-- 3. Confirm the engine sees the roster BEFORE you start
.dc bosses
.dc config

-- 4. Run it
.dc on

-- 5. Supervise
.dc status              -- what is it walking to, and is it stalled?
.dc spectate follow     -- ride the tank
.dc go 15509            -- skip ahead to Princess Huhuran by creature entry
.dc skip                -- abandon a stuck pack
.dc pull dynamic        -- let it choose per-pack
```

### ⚠️ `.raid tele` alias audit — several were landing in the wrong place

`.raid tele <alias>` forwards to `.tele group <name>`, and `ObjectMgr::GetGameTele` lowercases the argument, takes an **exact** `game_tele.name` match, and otherwise falls back to the **first name that merely contains it**. That fallback is the trap: a wrong name does not fail cleanly, it silently resolves to whatever unrelated tele contains the string.

Verified against `tw_world.game_tele` and **fixed in `run/lua_scripts/raid_automation.lua`**:

```lua
-- ============================================================================
-- .raid tele ALIAS RESOLUTION AUDIT (tw_world.game_tele)
-- ============================================================================

-- .raid tele ony
--   Was resolving to: substring -> "Onyxia'sLair" id 143, map 1 (outdoors, Darkshire)
--   Now resolves to:  "onyxia" id 195, map 249 (inside) [FIXED]
.raid tele ony

-- .raid tele onyxia
--   Was resolving to: substring -> "Onyxia'sLair" id 143, map 1
--   Now resolves to:  "onyxia" id 195, map 249 (inside) [FIXED]
.raid tele onyxia

-- .raid tele mc
--   Was resolving to: substring -> "TheMoltenSpan" id 163, map 0 (world)
--   Now resolves to:  "mc" id 190, map 409 (inside) [FIXED]
.raid tele mc

-- .raid tele naxx
--   Was resolving to: "Naxxramas" id 270, map 0 (outdoors)
--   Now resolves to:  "nax" id 275, map 533 (inside) [FIXED]
.raid tele naxx

-- .raid tele scholo
--   Was resolving to: substring -> id 4, map 0
--   Now resolves to:  "scholo_kirtonos" id 428, map 289 [FIXED]
.raid tele scholo

-- .raid tele sm
--   Was resolving to: substring -> "RagefireChasm" id 46, map 1
--   Now resolves to:  "smEntry" id 470 [FIXED]
.raid tele sm

-- .raid tele zf
--   Status:           No game_tele row on map 209 (reports "not found")
--   Workaround:       Needs DB row. Use '.tele 209 <x> <y> <z>' at pyramid meanwhile.
.tele 209 -1186.74 382.44 8.61

-- .raid tele aq20
--   Resolves to:      "aq20" id 194, map 509 [CORRECT]
.raid tele aq20

-- .raid tele aq40
--   Resolves to:      "aq40" id 192, map 531 [CORRECT]
.raid tele aq40

-- .raid tele bwl / .raid tele zg
--   Resolves to:      ids 178 / 166, correct maps [CORRECT]
.raid tele bwl
.raid tele zg

-- .raid tele scarlet
--   Resolves to:      "ScarletCitadel" id 500, map 45 (Turtle 10-Man Scarlet Citadel raid) [CORRECT]
.raid tele scarlet
```

`.tele group` is **`SEC_DEVELOPER`** and requires a **selected, grouped player** — you cannot run it solo. The remaining Zul'Farrak gap needs a `game_tele` row, which is a database write and deliberately out of scope here; use `.tele 209 <x> <y> <z>` at the pyramid meanwhile.

**Not bugs — leave alone:** `.raid tele karazhan`, `.raid tele crypt` and `.raid tele vault` resolve to `game_tele` rows on **map 0**. That is correct, not a mis-resolution: `game_tele` has *no* row on their interior maps (**35** Stormwind Vault / **532** + **814** Karazhan / **800** Karazhan Crypt), so the outdoor row is the only teleport that exists and is serving as the instance entrance. ⚠️ Do **not** read an interior map id of `810`, `812` or `813` for these three: `812` is not a map at all, and `813` is `Winter Veil Vale`.

⚠️ **§8's map numbers for the custom instances are corrected by this section, which is the authority.** Verified `game_tele` maps: Scarlet Citadel = **45**, Karazhan/Crypt/Vault = 0 (entrances), `sanctumraid` = **807**, `crescentgrove` = **802**, `hateforgeinstance` = **808**, `gilneascity` = **815**, `TimbermawHold` = **1** — and **Timbermaw and Gilneas are the two most commonly swapped**. The §8 table carries the corrected values inline as well, and it omits seven working aliases the script implements — `molten`, `blackwing`, `zulgurub`, `naxxramas`, `onyxia`, `st`, `zf` — all listed there.

### Runbook: Onyxia's Lair (the shortest one — start here)

```text
.bot group size=20   -- fills the roster; assigns NO specs
.raid check          -- read the real spec strategies
.raid tanks 2        -- promote real tanks; this is what .dc needs
.raid roles          -- formations by role, focus lock
.raid ready          -- gear, enchants, consumables, attunements
.raid tele ony       -- now lands INSIDE the lair (map 249)
.bot summon *          -- pull the raid to you
.dc bosses             -- expect "Onyxia" listed TWICE, see note
.dc on
.dc spectate follow
.dc status
```

`.dc bosses` lists Onyxia **twice** on map 249, and that is expected: the raw boss `10184` (a real spawn at `0.7, -212.0, -86.1`, order slot 0) supplies the approach anchor, and a synthetic **"Onyxia" objective** at the chamber centre `-12.867, -216.626, -88.058` (slot 1) supplies the completion event that engages and kills her. Both refer to the same fight.

Onyxia is the cheapest full-fight test: one boss, no trash gating, and `.dc go 10184` jumps straight to her.

### `.raid` Command Reference (Eluna, engine-verified)

`run/lua_scripts/raid_automation.lua` (**v1.4.0** — `local SCRIPT_VERSION =
"1.4.0"` at `:632`, printed at `:2279`, `:2727` and `:3705`) registers a `.raid`
namespace on `PLAYER_EVENT_ON_COMMAND` (event `42`). The full v1.4.0 surface — the
five-layer operator facade, the filters, and the trap register — is documented in
[§13](#13-the-raid-operator-facade-v140).

> ⚠️ **The live copy is `run/lua_scripts/raid_automation.lua`, and nothing else.**
> `run/mangosd.conf` sets `Eluna.ScriptPath = "./lua_scripts"` **relative to the
> `run/` working directory**, so that is the only path Eluna ever loads. A v1.1.0
> copy of this script historically sat at the repo-root `lua_scripts/`, where
> Eluna never loaded it; v1.1.0 lacks `tank3` / `tank4` / `tanks` / `marks` /
> `ccmark` / `resume` / `gather` / `avoid` / `roll` entirely, so auditing it
> produces a large set of false "this command does not exist" findings. That path
> no longer exists in the working tree or in git `HEAD`; see the file-state
> warning in the header if a checkout or merge ever brings it back.

**Order that works:**

```text
.bot group size=20     ← fills the roster to the size row; assigns NO specs
.raid check            ← read the spec strategy each bot actually got
.raid tanks 2          ← THE spec step: talents do protection + reset ai
.raid roles            ← formations by role, focus lock, roster census
.raid marks T1 T2 T3   ← optional: one mark icon per tank
.raid ready            ← gear, enchants, spells, consumables, attunements
.raid start            ← dc bosses → dc pull dynamic → dc on → dc status
```

```lua
-- ============================================================================
-- .raid COMMAND REFERENCE (Eluna v1.4.0)
-- ============================================================================

-- .raid roles (alias .raid setup)
-- Roster census (class counts + feasibility, plus tank-capable name list),
-- then splits formations by role via one broadcast per role, and focus-locks DPS. Sends NO talent commands.
.raid roles
.raid setup

-- .raid tank2 / tank3 / tank4 / tank5 [<name>]
-- Promote a bot to that tank slot. All four land on the same branch (slot = tonumber(sub:sub(5))).
-- Without a name it promotes your current target; with neither it errors and changes nothing.
.raid tank2
.raid tank2 TankbotName
.raid tank3
.raid tank4
.raid tank5

-- .raid tanks <n> [names...]
-- Count form for any n (1-8). Names optional, but roster-order fallback is never silent.
.raid tanks 2
.raid tanks 2 MainTank OffTank

-- .raid marks <tank...>
-- One mark icon per named tank, in priority order: 1st skull, 2nd cross, 3rd square, then diamond/triangle/star/circle.
.raid marks MainTank OffTank

-- .raid marks <tank> <icon>
-- Set one tank's icon explicitly.
-- WARNING: 'moon' is deliberately refused as icon name: MARK_ICON_ORDER excludes it because
-- index 4 is the engine's own CC marker (strategy/values/TargetValue.cpp:57-59). Use .raid ccmark.
.raid marks MainTank skull

-- .raid unmark [tank]
-- 'co -mark rti' on one bot, or on whole roster.
.raid unmark
.raid unmark MainTank

-- .raid ccmark (alias .raid moon)
-- Moon (icon 4) on current target — the crowd-control marker.
.raid ccmark
.raid moon

-- .raid skull (aliases .raid mark, .raid focus)
-- Skull (icon 7) on current target (dispatch handles skull/focus/mark identically).
.raid skull
.raid mark
.raid focus

-- .raid clearmarks
-- Clears star / circle / moon / skull icons.
.raid clearmarks

-- .raid ready [best|epic] (alias .raid auto)
-- roles + gear + attune. Takes only two words; coerces anything else to 'best'.
-- (.raid ready purple -> .raid ready best silently; use .raid gear purple for specific tier).
.raid ready
.raid ready best
.raid ready epic
.raid auto

-- .raid attune (alias .raid keys)
-- Completes the 9 attunement quests, sets Argent Dawn rep, adds the 13 master key items.
.raid attune
.raid keys

-- .raid gear [quality] (alias .raid prep)
-- bot gear * <q>, enchants, train, prep, consumes, food, potions, regs, ammo, repairitems + replenish.
-- Accepts 10 values across 7 tiers: best, green/uncommon, blue/rare, purple/epic, partial, upgrade, sync.
-- Unrecognised words are refused with full list printed.
.raid gear best
.raid gear epic
.raid gear purple
.raid gear blue
.raid prep best

-- .raid buff / .raid wbuff / .raid worldbuff
-- Sends nothing — reports how buffing actually works.
.raid buff
.raid wbuff
.raid worldbuff

-- .raid summon (alias .raid rally)
-- Broadcasts: bot summon *
.raid summon
.raid rally

-- .raid start [mode]
-- Pipeline: dc bosses -> dc pull <mode> -> dc on -> dc status.
-- Requires dungeon/raid map. Default mode is dynamic.
.raid start
.raid start dynamic
.raid start off
.raid start on

-- .raid off (alias .raid stop)
-- Broadcasts: dc off
.raid off
.raid stop

-- .raid pause / .raid resume
-- Broadcasts: dc pause pause / dc pause resume (never bare toggle).
.raid pause
.raid resume

-- .raid pull <off|on|dynamic>
-- Forwards to 'dc pull <mode>'. Bare .raid pull sets dynamic; unrecognised words clamp to dynamic.
.raid pull dynamic
.raid pull off
.raid pull on

-- .raid go <boss> / .raid skip / .raid status / .raid bosses / .raid config / .raid spectate [follow <name>|next|prev|list]
-- Forwards to matching .dc subcommand.
.raid go 15509
.raid skip
.raid status
.raid bosses
.raid config
.raid spectate follow Tankbot

-- .raid tele <alias>
-- Group teleport to instance alias (e.g., ony, mc, bwl, zg, aq20, aq40, naxx, scholo, sm, scarlet).
.raid tele ony
.raid tele mc
.raid tele bwl
.raid tele aq40

-- .raid unbind (alias .raid resetlock)
-- Broadcasts: .instance groupunbind all.
-- NOTE: .raid reset does NOT exist (renamed in v1.2.0).
.raid unbind
.raid resetlock

-- Strategy toggles:
-- Broadcasts to bots for loot, resting, opening, AOE, gathering, avoiding, rolls, and focus:
.raid loot       -- nc +loot
.raid noloot     -- nc -loot
.raid rest       -- nc +food, nc +potions
.raid norest     -- nc -food, nc -potions
.raid open       -- u (inert in engine)
.raid aoe        -- co +aoe
.raid noaoe      -- co -aoe (alias .raid single)
.raid single     -- co -aoe
.raid gather     -- nc +gather
.raid nogather   -- nc -gather
.raid avoid      -- co +avoid aoe
.raid noavoid    -- co -avoid aoe
.raid roll       -- nc +roll
.raid noroll     -- nc -roll
.raid unfocus    -- co -focus rti targets
```

**Aliases** — each one lands on the same branch as the command it is an alias of,
so they are the same command in every respect:
`.raid setup` = `roles`, `.raid auto` = `ready`, `.raid prep` = `gear`,
`.raid rally` = `summon`, `.raid keys` = `attune`, `.raid moon` = `ccmark`,
`.raid mark` = `skull`, `.raid resetlock` = `unbind`, `.raid stop` = `off`,
`.raid noaoe` = `single`.

Things to know:

- **`.raid roles` does not assign specs, and that is deliberate.** ⚠️ **A belief
  worth killing outright: "`.bot group` already applies a role-matched spec and
  *varies within the role*; forcing one spec per class flattened it." That is
  false.**
  `.bot group` applies **no** role-matched spec at all — it never forwards the
  role it rolls (`PlayerbotMgr.cpp:2825` → `:2831` → `:2855`, no `role=`), so
  `AutoSelectTalents` gets `BOT_ROLE_NONE` (`:2680`) and picks a random premade
  spec (`ChangeTalentsAction::getPremadePaths:170`, `PickPremadePath:230`). The
  same correction is carried by [`.bot group` assigns the spec too](../02_playerbot_and_raid_automation.md)
  in Volume 02.
  **The real reason `.raid roles` sends none** is that it cannot know which bots
  need promoting — it addresses the whole roster through `@role` filters and has no
  per-bot talent reading. One spec per class would also flatten variety, which
  remains true, but it is a secondary argument, not the load-bearing one. **The
  load-bearing consequence: `.raid tanks <n>` is now the only re-spec path in the
  pipeline, and running it is not optional.**
- **Tanks already have `tank assist` by default — do not add it.** `AiFactory`
  puts it in the default kit for every tank spec, in **both** the combat and the
  noncombat engine: `protection` (warrior and paladin) at `AiFactory.cpp:366,410`
  and `:756,902`, and `tank feral` (druid) at `:460,878`. That is what makes
  `PlayerbotAI::IsTank` true, and it is also what lets a tank pick up adds and
  off-target. Adding `co +tank` from outside is not just redundant, it is
  **destructive on druids** — the druid sibling group is
  `{bear, tank, tank feral, cat, dps feral, balance, restoration, heal, leveling}`,
  so `co +tank` drops `tank feral` and un-tanks the druid.
- **⚠️ A level-60 feral druid is a 50/50 cat/bear coin flip.** `AiFactory.cpp:579-595`:
  ```cpp
  if (player->getClass() == CLASS_DRUID && tab == 1
      && urand(0, 100) > 50 && player->GetLevel() >= 20)
  {
      if (player->HasSpell(16961) || player->HasSpell(16958))   // moonkin or bear
      {
          combatEngine->addStrategies("dps feral", "close", "stealth", "behind", NULL);
          combatEngine->removeStrategy("tank feral");
          combatEngine->removeStrategy("tank assist");     // <- becomes a CAT
      }
      else
      {
          combatEngine->addStrategies("tank feral", "tank assist", "close", NULL);
          combatEngine->removeStrategy("dps feral");       // <- becomes a BEAR
          combatEngine->removeStrategy("dps assist");
          combatEngine->removeStrategy("behind");
      }
  }
  ```
  A 60 druid knows both spells, so the roll always decides. Consequences at raid
  scale: two feral druids can both come out cats when you wanted one tank, and it
  **re-rolls on every `reset ai`** — so any rebuild can flip one. Prefer Warriors
  or Paladins for off-tanks, and verify after any promotion. `.raid tanks`
  prints a warning when it promotes a druid for exactly this reason.
- **`.raid roles` and `.raid start` must be separate commands** — but **not**
  for the reason given above.
  ⚠️ **"Leave a second or two" is not the constraint, and Eluna here *does* have a
  timer API.** Our Eluna flavour is `ELUNA_VMANGOS` (`src/game/CMakeLists.txt:681`) and it
  **does** have a timer API:
  - `CreateLuaEvent` — `src/modules/Eluna/methods/VMangos/GlobalMethods.h:1368`,
    registered at `:2996`; accepts a delay (ms) or a `{min,max}` range and a
    repeat count.
  - `WorldObject:RegisterEvent` — `methods/VMangos/WorldObjectMethods.h:713`,
    registered at `:1127`; same shape, bound to that object.
  - `RemoveEventById` / `RemoveEvents` — `GlobalMethods.h:1405` / `:1423`.

  **And the script now uses it.** v1.4.0 registers `MAP_EVENT_ON_UPDATE`
  (event `23`) → `OnMapTick` at `raid_automation.lua:3702` to drain a **bounded
  deferred-step queue**: `STEPS_MAX = 64`, `STEP_MAX_AGE_TICKS = 200`
  (Lua `:1425-1487`, constants at `:1428-1430`). One step runs per map tick; a
  batch that waits longer than 200 ticks is **aborted and reported** rather than
  running forever. Each step is `pcall`ed, and a failure aborts the rest of the
  batch instead of leaving a half-applied state. `.raid attune` (`:1668`) and
  `.raid promote`/`.raid tanks` (`:1974`) both enqueue into it, and
  `.raid pending` (see [§13.6](#136--pass--escape-hatches-to-the-raw-engines))
  reports the queue depth.

  **The real constraint is ordering, not waiting.** `.raid start` dispatches `dc bosses` → `dc pull <mode>` → `dc on` →
  `dc status` as **four back-to-back `RunCommand` calls with no ordering
  guarantee** (Lua `:2181-2194`) — there is no queue between them, because the
  ordering problem here is not "wait long enough" but "four independent bot-AI
  ticks may interleave". Each of those four is processed by whichever bot runs its
  AI next. `.raid roles` therefore has to be a separate, earlier command: the
  formations and the focus lock must be installed before the run starts, and
  there is no mechanism that can order one chat command after another.
- **`dc on` is issued exactly once, ever.** It wipes skip and progress state, so
  re-running it mid-raid un-skips every boss.
- **You cannot be configured.** You are the sender, and a real player has no
  `PlayerbotAI`, so nothing here can set a strategy on you.
- **`.raid buff` sends nothing on purpose.** `buff` is already in every default
  combat kit and each spec wires its own aura — enhancement gets *lightning
  shield*, restoration gets *water shield*. ⚠️ A per-class aura table that sent
  `water shield` to **every** shaman would overwrite that correct per-spec choice,
  so the script carries no such table — do not add one.

#### Composition targets

An exact mirror of `LfgAction::AllowedClassRoleNr`
(`InviteToGroupAction.cpp:89`, per-role caps at `:105-121`) — **including the
20-man tank count of 2**. ⚠️ The script does **not** raise it to 3. The table is an
exact mirror of what `.bot group` itself targets, so the census and reality cannot
disagree.

```lua
-- ============================================================================
-- ROSTER COMPOSITION TARGETS (LfgAction::AllowedClassRoleNr)
-- ============================================================================

-- Group Size 5:  1 Tank,  1 Healer,  3 DPS
.bot group size=5

-- Group Size 10: 2 Tanks, 3 Healers, 5 DPS
.bot group size=10

-- Group Size 20: 2 Tanks, 5 Healers, 13 DPS (script does NOT raise tank count to 3)
.bot group size=20

-- Group Size 25: 3 Tanks, 7 Healers, 15 DPS
.bot group size=25

-- Group Size 40: 4 Tanks, 16 Healers, 20 DPS
.bot group size=40
```

⚠️ **These numbers shape the CLASS MIX. They do not shape the SPECS.**

**What is real.** The **class caps** in the table are the engine's own
`AllowedClassRoleNr` (`InviteToGroupAction.cpp:105-121`) and
`HandleGroup` decrements them as it goes (`PlayerbotMgr.cpp:2870-2873`), so they
genuinely govern which classes get drawn. **Faction is respected**:
`HandleGroup` skips `CLASS_PALADIN` on HORDE and `CLASS_SHAMAN` on ALLIANCE
(`PlayerbotMgr.cpp:2833-2842`), so a Paladin is never given a slot in a horde raid
and a Shaman never in an alliance one; off-faction bots are left untouched and
reported by name, because `.bot group` will not create them.

**What is not real.** A plausible-sounding description to discard: *"Assignment
rules: tank slots are filled first from tank-capable bots (Warrior, Paladin,
Druid) because they are the scarce resource; the **third** slot prefers a Warrior;
heal slots come from Priest, Paladin, Shaman or Druid; everything else is DPS."*
⚠️ **There is no slot-filling pass, no ordering, and no "prefers a Warrior".**
There are no slots at all. The actual loop (`PlayerbotMgr.cpp:2825-2873`) is:

1. `BotRoles role = BotRoles(urand(BOT_ROLE_TANK, BOT_ROLE_DPS))` — a **uniform
   random role**, redrawn every iteration.
2. If that role has no classes left (`allowedClassNr[0][role] == 0`) → `continue`.
3. `uint8 cls = factory.GetRandomClass(0, role)` — a random class that can fill
   that role.
4. Faction check: Paladin-on-HORDE and Shaman-on-ALLIANCE → `continue`.
5. Class-cap check → `continue` if exhausted.
6. `HandleCreate` with `level= class= group=` — **and no `role=`** (`:2855`).
7. Decrement `allowedClassNr[0][role]` and `allowedClassNr[cls][role]`
   (`:2870-2873`), then loop until the group is full or `maxTries` is spent.

There is no "first pass the tanks", no third-slot special case, and no class
preference of any kind — the role draw is uniform, so a 20-man consumes its tank
quota at the same rate as everything else, purely by luck of the draw.

**And the specs are not assigned at all**, which is the consequence that matters:
`CreateBot` receives no `role=`, holds `BOT_ROLE_NONE` (`:2536`), and
`AutoSelectTalents` at `:2680` skips its role filter
(`ChangeTalentsAction::getPremadePaths:170`) and picks a **random premade spec for
that class** (`PickPremadePath:230`). So "2 tanks" in the 20-man row means *two
tank-capable CLASSES were drawn* — not two protection warriors. Verify with
`.raid check`, promote with `.raid tanks <n>`.

#### Interrupts

There is no `interrupt` strategy to enable, because interrupting is a *trigger
inside each class's `cc` chain* — `FuryWarriorCcStrategy` pushes `pummel` with
`ACTION_INTERRUPT`, and the equivalents are Counterspell (mage), Hammer of
Justice (paladin), Silence (hunter) and so on. `cc` ships in every bot's default
combat set, which is why no `.raid` command turns it on.

> ⚠️ **`.raid roles` does NOT re-issue `co +cc` explicitly.** `ApplyRaidSetup` sends exactly five
> broadcasts — four formations plus `co +focus rti targets`. The string `co +cc`
> appears in the script only in its own header block, under the heading
> "COMMANDS THIS SCRIPT NEVER EMITS, AND WHY", which groups it with `nc +food`,
> `nc +potions`, `nc +loot` and `co +aoe` as strategies that are **already in
> every default kit**. The header's own point is that four of those five are
> operator toggles — they *are* sent, because `.raid aoe` / `.raid rest` /
> `.raid loot` and their off-forms exist — and that **`co +cc` is the one in that
> group with no toggle**, since crowd control has no useful off-switch that does
> not also break the pull. So: no `cc` broadcast, and no `cc` toggle, on purpose.

---

### Runbook: Ruins / Temple of Ahn'Qiraj

```text
-- AQ20 (map 509)
.raid tele aq20
.raid roles
.raid start

-- AQ40 (map 531)
.raid tele aq40
.raid roles
.raid start
```

Both aliases were already correct. Two things to expect when you read `.dc bosses`:

- **Ouro `15517` and the C'Thun Eye `15589` will not appear.** Neither has a `creature` spawn row (0 each) — Ouro is summoned by the Ouro Spawner `15957`, and the Eye is a summon of the body. They are driven by event objectives instead, so watch `.dc status` (which reports what the run is walking toward) rather than the boss list.
- On AQ40, **Skeram's room is pre-cleared before he is pulled**, so the run spends its first stretch clearing 8 Anubisath Sentinels and 3 Obsidian Eradicators rather than walking straight to the boss. That is the `RoomAggroRegistry` row doing its job, not a stall — `.dc status` shows the room clear in progress.

---

## 6. In-Dungeon Recovery, Doors, AoE & Target Focus

These commands apply to every bot, so in a raid each one goes out as a single
`.bot r` broadcast rather than a whisper per bot — see [how to address a raid of
bots](#-read-this-first-how-to-address-a-raid-of-bots) at the top of
§5.1. Outside a raid the script falls back to whispering each bot, because
`.bot r` answers `Sender is not in a raid group` in a party. None of them use
`.bot c`, which is a debug stub that reports success and does nothing.

> **On the leading dot.** Every command in this document is written the way you
> type it, **with the leading dot**: `.bot r co +aoe`, `.bot summon *`. The dot is
> what makes it a chat command rather than plain text — without it the client
> sends it as ordinary chat and the server never dispatches it. The Eluna
> `RunCommand` helper is the one exception: it takes the same string *without*
> the dot, so the script's internal calls read `bot r co +aoe`. **Anything in this
> document that is inside a `text` or `lua` block and is meant to be typed needs
> the dot.**
>
> These are all **one broadcast each** in a raid. The script falls back to
> whispering each bot in a party, because `.bot r` answers
> `Sender is not in a raid group` outside a raid. The identical payload runs
> either way.

### Tactical Bot Directives

**Every row now has an explicit off form.** In v1.2.0 the toggles were one-way:
`.raid aoe` existed and `.raid single` was a special case, and nothing could turn
`loot` / `rest` / `avoid` / `gather` / `roll` back off. v1.3.0 gives each one a
matching `no…` command that sends the `-` form of the same strategy.

```lua
-- ============================================================================
-- TACTICAL BOT DIRECTIVES (.raid TOGGLES AND DIRECTIVES)
-- ============================================================================

-- .raid loot / .raid noloot
-- Broadcasts: .bot r nc +loot / nc -loot
-- Orders every bot to add (or drop) loot strategy (vacuums or walks past nearby enemy corpses).
.raid loot
.raid noloot

-- .raid rest (aliases: .raid drink, .raid food) / .raid norest
-- Broadcasts: .bot r nc +food +nc +potions / nc -food +nc -potions
-- Orders bots to eat and drink back to full health and mana — or stop.
.raid rest
.raid food
.raid drink
.raid norest

-- .raid open (aliases: .raid use, .raid breach)
-- Broadcasts: .bot r u
-- WARNING: Inert in engine. There is no TriggerNode named 'use' anywhere; both words dropped in silence.
-- UseAction reachable only from LootAction and RpgSubActions. Included for backward compatibility.
.raid open

-- .raid aoe / .raid noaoe (alias: .raid single)
-- Broadcasts: .bot r co +aoe / co -aoe
-- Prioritises AoE damage for everyone who can use it, or leaves single-target behaviour.
.raid aoe
.raid noaoe
.raid single

-- .raid gather / .raid nogather
-- Broadcasts: .bot r nc +gather / nc -gather
-- Pick up herbs and ore while walking — or ignore nodes for a strict run.
.raid gather
.raid nogather

-- .raid avoid / .raid noavoid
-- Broadcasts: .bot r co +avoid aoe / co -avoid aoe
-- Step out of AoE hazards. Saves lives; leave on.
.raid avoid
.raid noavoid

-- .raid roll / .raid noroll
-- Broadcasts: .bot r nc +roll / nc -roll
-- Roll on loot, or ignore loot rolls entirely.
.raid roll
.raid noroll

-- .raid unfocus
-- Broadcasts: .bot r co -focus rti targets
-- Releases the DPS focus lock so bots may be pulled off the icon (reverses lock set by .raid roles).
.raid unfocus

-- .raid skull (aliases: .raid focus, .raid mark)
-- Engine: Group:SetTargetIcon(7, target)
-- Places Skull icon on current target — what DPS focus lock follows.
.raid skull
.raid focus
.raid mark

-- .raid ccmark (alias: .raid moon)
-- Engine: Group:SetTargetIcon(4, target)
-- Places Moon icon on current target. Raid deliberately avoids moon targets (crowd control).
.raid ccmark
.raid moon

-- .raid marks <tank...>
-- Whisper per named tank: assigns one distinct mark icon per tank.
.raid marks MainTank OffTank

-- .raid follow
-- Broadcasts: .bot r follow
-- Soft regroup: bots walk to your position and hold (movement, not teleport).
.raid follow

-- .raid summon
-- Broadcasts: .bot summon *
-- Hard regroup: teleports all group bots to your coordinates immediately.
.raid summon

-- .raid unbind (alias: .raid resetlock)
-- Engine: .instance groupunbind all
-- Clears instance ID lockouts for every player and bot in group.
.raid unbind
.raid resetlock
```

---

## 7. Automatic Role & Formation Matrix

`.raid roles` sends **no talent commands at all**. It issues four roster-wide
broadcasts plus one focus lock:

```text
.bot r @tank  formation shield      -- tanks
.bot r @heal  formation far          -- healers
.bot r @melee formation melee        -- melee DPS
.bot r @ranged formation line        -- casters and hunters
.bot r @dps   co +focus rti targets  -- stop anything peeling DPS off the skull
```

Plus a roster census on your chat, and nothing else.

### Why formations are the whole job

`.bot group` does **not** set formations per role. `AcceptInvitationAction.h:80-82`
loads the **inviter's** formation onto every bot as it joins:

```cpp
Formation* masterFormation = MAI_VALUE(Formation*, "formation");
FormationValue* value = (FormationValue*)context->GetValue<Formation*>("formation");
value->Load(masterFormation->getName());
```

So out of `.bot group size=20` the entire raid is on **one** shape — healers in
melee, casters in melee, everyone bunched behind the tank. Splitting by role is
the single most useful thing `.raid roles` does.

Order matters: `@tank` and `@heal` are tested before `@melee` and `@ranged`, and
each bot takes the first filter that matches, so a tank who is also melee keeps
`shield`.

### Why the script does not need to know anyone's spec

The role filters are evaluated by **each bot against its own installed strategy
set** (`RoleChatFilter`, `ChatFilter.cpp:177-213`). A bot's role is a property of
its **spec strategy**, not its class — a `fury` warrior and a `protection` warrior
are both class 1. So the script says "@tank, use shield" and every tank decides
for itself. Do not add a per-class spec table to assign formations from; it is
both unnecessary and less accurate.
⚠️ **A tempting wrong reason to believe: "`.bot group` already built the kit from
the real talent tab."** The filtering mechanism is correct, but that reason is
not: `.bot group` does **not** build a kit from a real talent tab for this purpose
— it hands `AutoSelectTalents` a `BOT_ROLE_NONE` and lets it pick a random
premade spec (`PlayerbotMgr.cpp:2680`,
`ChangeTalentsAction::getPremadePaths:170`). So the filters will correctly report
*whatever spec each bot ended up with* — including "not a tank", for most of a
freshly assembled roster. Run `.raid check` first, `.raid tanks <n>` second.

### Quota Allocation by Group Size

The census **rounds up** to the next row, so a scaled raid that fields fewer
players than its bracket still reports that bracket's shape. These are the same
**class-shape** numbers `.bot group` targets, and this is a **feasibility check,
not a measurement** — the script cannot read a bot's talents from Eluna.

```lua
-- ============================================================================
-- QUOTA ALLOCATION BY GROUP SIZE BRACKET
-- ============================================================================

-- 5-Man Bracket (Group Size <= 5):
--   Target Tanks:   1 Tank
--   Target Healers: 1 Healer
--   Target DPS:     3 DPS

-- 10-Man Bracket (Group Size 6-10):
--   Target Tanks:   2 Tanks
--   Target Healers: 3 Healers
--   Target DPS:     5 DPS

-- 20-Man Bracket (Group Size 11-20):
--   Target Tanks:   2 Tanks
--   Target Healers: 5 Healers
--   Target DPS:     13 DPS

-- 25-Man Bracket (Group Size 21-25):
--   Target Tanks:   3 Tanks
--   Target Healers: 7 Healers
--   Target DPS:     15 DPS

-- 40-Man Bracket (Group Size 26-40):
--   Target Tanks:   4 Tanks
--   Target Healers: 16 Healers
--   Target DPS:     20 DPS (caps: priest heals 11, mage DPS 15, warrior DPS 8, paladin tanks 0)
```

The 40-man row also caps classes in `.bot group`'s own table — priest healers
11, mage DPS 15, warrior DPS 8, and **paladin tanks 0**.

If the census reports fewer tank-capable or heal-capable bots than the row wants,
that is a real shortfall and `.dc on` will refuse. ⚠️ Note the census counts
**tank-capable classes**, which `.bot group` really does shape — so a healthy
census is *not* evidence that anyone is protection-specced. Confirm with
`.raid check`, and promote real tanks with `.raid tanks <n>`.

### Stances

`.raid roles` does **not** set stances, and there is no command that sets a
warrior's *class* stance — mod-playerbots exposes none. A warrior bot's own
`protection` / `fury` class strategy picks the class stance itself (`defensive`
for tanking, `battle` otherwise) as part of its kit, and forcing one from outside
would fight that.

The `stance` bot command is a **positioning** value (`Stance*` → a
`WorldLocation`), not a class stance. Its only valid words are `near` /
`default`, `tank`, `turnback` / `turn`, and `behind` (`StanceValue::Load`,
`Stances.cpp`). So `/w BotName stance defensive` is **not a valid command** — do
not try it. `stance tank` is a valid *positioning* stance and has nothing to do
with Defensive stance.

### Markers

```lua
-- ============================================================================
-- RAID MARKERS (RTI ICONS AND INDICES)
-- ============================================================================

-- Star (MT): Index 0 (Set by first tank slot, automatic)
-- Circle (assist / OT): Index 1 (Set by second tank slot, automatic)
-- Moon (CC): Index 4 (Set by your current target via '.raid ccmark')
-- Skull: Index 7 (Set by your current target via '.raid skull')
-- Cross: Index 6 (Set by tank with 'mark rti' via '.raid marks <tank> cross')
-- Square: Index 5 (Set by tank with 'mark rti' via '.raid marks <tank> square')
-- Diamond: Index 2 (Set by tank with 'mark rti' via '.raid marks <tank> diamond')
-- Triangle: Index 3 (Set by tank with 'mark rti' via '.raid marks <tank> triangle')
```

> ⚠️ **Moon is icon 4, and it is the engine's own CC marker** — hard-coded in
> `FindNonCcTargetStrategy::IsCcTarget` (`strategy/values/TargetValue.cpp:57-59`) and the
> default of the `rti cc` value. `dps aoe` skips whatever carries it. ⚠️ `.raid
> ccmark` sets **Moon (icon 4)**, not Triangle — the distinction matters, because a
> triangle-marked mob is a *priority kill* target while a moon-marked mob is one
> the raid deliberately **walks away from**.
> `.raid marks` refuses `moon` for the same reason.

All go out through `Group::SetTargetIcon`, which broadcasts
`MSG_RAID_TARGET_UPDATE` and draws on the client raid frame. **They are
cosmetic as far as mod-dungeon-clear is concerned**: DC elects the tank that
drives the run by gear score, because its own Main Tank branch is compiled out on
1.12 (`if (false)`, `DcLeaderSignal.cpp:398-413`) and the `groups`.mainTank
column is never sent to the client. **Gear your intended MT best.**

### Tank marking — one icon per tank

`.raid marks <tank1> <tank2> …` gives each named tank a **distinct** mark icon in
priority order, so several tanks can mark without fighting:

```lua
-- ============================================================================
-- TANK MARKING POSITION PRIORITY ORDER
-- ============================================================================

-- Position 1st: skull
-- Position 2nd: cross
-- Position 3rd: square
-- Position 4th: diamond
-- Position 5th: triangle
-- Position 6th: star
-- Position 7th: circle
```

**Why distinct icons are mandatory, not a preference.** `MarkRtiAction`
(`strategy/actions/RtiAction.cpp:48`) walks `possible attack targets`, picks the
**lowest-health unit not already carrying any icon**, and marks it with the index
of that bot's own `rti` value — which defaults to `skull`. So two tanks both left
on the default thrash deterministically:

1. Tick 1 — tank A marks Mob1 `skull`.
2. Tank B now sees Mob1 is marked, picks Mob2 (next lowest unmarked), and marks
   Mob2 `skull` — **wiping A's icon**.
3. Tick 2 — A sees Mob2 marked, re-marks Mob1. The skull alternates forever.

And because `rti` is *also* the focus target (`RtiTargetValue` resolves `rti
target` from the same icon, and `DpsTargetValue` prefers it), a thrashing skull
means every DPS bot's focus thrashes with it and the run never settles on a kill
order. One icon per tank fixes it: each claims the lowest-health *unmarked* mob,
so the priority order is stable.

**The script does not guess your tanks.** `rti` is a side-effecting value — it
decides what that bot attacks — so `EnableTankMarking` only touches bots whose
names you passed, and prints the exact mapping it applied so you can correct it.
`.raid check` (`bot r all ?` → `ai->PrintStrategies(requester, BOT_STATE_ALL)`,
`ChangeStrategyAction.cpp:150`) makes **every bot print its live installed
strategy set**, all four engines — and the spec strategy is in that list, so
"is this bot already a tank?" **is** answerable. The *script* still cannot parse
chat it does not read back, so `.raid marks` still takes names rather than
guessing; but you need not guess either. Run `.raid check` once, after
`.bot group`, and you can see it. A bare `.raid marks` enables `mark rti` on
`@tank` and **warns that this is the thrash case**.

Handy overrides:

```text
.raid marks Tankbot1 square     -- set one tank's icon explicitly
.raid unmark Tankbot1           -- stop one tank marking
.raid unmark                     -- stop everyone marking
```

### Picking a 3rd / 4th off-tank

`.bot group size=20` draws a **2-tank-class** roster, and `size=40` a 4-tank-class
one (`AllowedClassRoleNr`, `InviteToGroupAction.cpp:105-121`). Turtle-WoW raids
scale 20–40, so a 25- or 30-man fielded with a `size=40` group, or a 20-man that
wants a third tank, needs the count raised. That is the one thing `.bot group`
cannot do, because the count is fixed by the size row you asked for.

⚠️ **Read "tank" in that sentence as "tank-capable class", not "protection
specced".** The size row governs which classes get drawn; it says nothing about
which spec any of them ends up with, because `.bot group` forwards no `role=`
(`PlayerbotMgr.cpp:2855`) and `AutoSelectTalents` receives `BOT_ROLE_NONE`
(`:2680`) and picks a random premade spec. **`.raid tanks <n>` is therefore not
merely a convenience for going above the row — on a fresh roster it is how you get
tanks at all**, including the 2 the row already called for.

```text
.raid roles                 -- prints the tank-capable bot names, copy-paste

.raid tank3                 -- promotes your TARGET as the 3rd tank
.raid tank3 Gronk           -- same, naming it instead of targeting
.raid tank4                 -- 4th tank (also needs a target)
.raid tank4 Hondo           -- 4th tank, named

.raid tanks 3               -- count form: 3 tank-capable bots, roster order
.raid tanks 3 Gronk Hondo   -- count form, naming them
```

**`.raid tank3` and `.raid tank4` never guess.** They require either a target or
a name, and with neither they error out and change nothing:

```text
.raid tank3 needs a bot. Either TARGET one, or name it: .raid tank3 <botname>
```

That is deliberate. The script cannot read a bot's talents, so it only sees who is
*tank-capable* — not who already tanks. On a 40-man the third tank-capable name in
roster order is very unlikely to be the offtank you want, and because promotion
**re-specs**, a wrong pick is destructive rather than merely unhelpful. Targeting
removes the guess entirely. `.raid roles` lists the candidates so you can paste
one straight in.

What promotion does and does not do: it gives that bot the protection tree and
rebuilds its kit. It does **not** make them the run leader — mod-dungeon-clear
elects the best-geared tank bot, because the 1.12 Main Tank branch is `if (false)`.
It also never demotes, because demoting means picking a damage spec and that is
the variety-flattening move described above.

---

## 8. Master Instance Fast-Teleport Directory

Teleport your entire group/raid directly to the entrance of any dungeon or raid using `.raid tele <alias>`:

### Classic Raids & Dungeons

```lua
-- ============================================================================
-- CLASSIC RAIDS & DUNGEONS (.raid tele <alias>)
-- ============================================================================

-- Raids:
.raid tele mc        -- Molten Core (40-Man)
.raid tele bwl       -- Blackwing Lair (40-Man)
.raid tele ony       -- Onyxia's Lair (40-Man)
.raid tele zg        -- Zul'Gurub (20-Man)
.raid tele aq20      -- Ruins of Ahn'Qiraj (20-Man)
.raid tele aq40      -- Temple of Ahn'Qiraj (40-Man)
.raid tele naxx      -- Naxxramas (40-Man)

-- Dungeons:
.raid tele ubrs      -- Upper Blackrock Spire (10-Man)
.raid tele lbrs      -- Lower Blackrock Spire
.raid tele brd       -- Blackrock Depths
.raid tele strat     -- Stratholme
.raid tele scholo    -- Scholomance
.raid tele dm        -- Dire Maul (North)
.raid tele sm        -- Scarlet Monastery
```

### Turtle-WoW Custom Raids & Dungeons

⚠️ **The "Map ID" column is verified against `tw_world.game_tele`**, and stated
again in full in [§5.1](#51-complete-dc-command-reference-engine-verified).

```lua
-- ============================================================================
-- TURTLE-WOW CUSTOM RAIDS & DUNGEONS (.raid tele <alias>)
-- ============================================================================

-- Scarlet Citadel:
--   Token:    .raid tele scarlet
--   Bracket:  40-Man Raid
--   Map ID:   45
--   Location: Tirisfal Glades
.raid tele scarlet

-- Tower of Karazhan:
--   Token:    .raid tele karazhan
--   Bracket:  10/40-Man Raid
--   Map ID:   0 (exterior entrance; interior maps 532 + 814)
--   Location: Deadwind Pass
.raid tele karazhan

-- Timbermaw Hold:
--   Token:    .raid tele timbermaw
--   Bracket:  20-Man Raid
--   Map ID:   1
--   Location: Felwood
.raid tele timbermaw

-- Emerald Sanctum:
--   Token:    .raid tele sanctum
--   Bracket:  40-Man Raid
--   Map ID:   807
--   Location: Mount Hyjal
.raid tele sanctum

-- Karazhan Crypts:
--   Token:    .raid tele crypt
--   Bracket:  58-60 Dungeon
--   Map ID:   0 (exterior entrance; interior map 800)
--   Location: Deadwind Pass
.raid tele crypt

-- Hateforge Quarry:
--   Token:    .raid tele hateforge
--   Bracket:  50-60 Dungeon
--   Map ID:   808
--   Location: Burning Steppes
.raid tele hateforge

-- Crescent Grove:
--   Token:    .raid tele crescent
--   Bracket:  32-38 Dungeon
--   Map ID:   802
--   Location: Ashenvale
.raid tele crescent

-- Stormwind Vault:
--   Token:    .raid tele vault
--   Bracket:  Level 60 Dungeon
--   Map ID:   0 (exterior entrance; interior map 35)
--   Location: Stormwind City
.raid tele vault

-- Gilneas City:
--   Token:    .raid tele gilneas
--   Bracket:  42-48 Dungeon
--   Map ID:   815
--   Location: Gilneas
.raid tele gilneas
```

> ⚠️ **Map ID traps.** Verified `game_tele` values: `sanctumraid` = **807**,
> `crescentgrove` = **802**, `hateforgeinstance` = **808**, `gilneascity` = **815**,
> `TimbermawHold` = **1** — Timbermaw and Gilneas are the two most commonly
> swapped. Karazhan / Crypt / Vault = **0** is correct and not a bug: their
> interior maps **35** (Stormwind Vault), **532** + **814** (Karazhan) and **800**
> (Karazhan Crypt) have **no** `game_tele` row at all, so the outdoor entrance
> row is the only teleport that exists. ⚠️ `811`, `812` and `813` are not the maps
> for any of these — `812` is not a map at all, and `813` is `Winter Veil Vale`.
>
> The **Bracket** column's `40-Man Raid` for Scarlet Citadel matches
> [§12.3](#123-scarlet-citadel-map-45-40-man-raid-fortress), which describes map
> `45` as a **40-Man Raid Fortress** with seven scripted bosses and a
> `SealedEncounterRegistry` that waits for all 40 members.

> 📋 **Seven further working aliases.** The script's
> `TELEPORT_PRESETS` table (`raid_automation.lua:786-816`) implements a long-form
> synonym for most of these, and only `zf` appears anywhere in this handbook:
>
> | Extra alias | Resolves to | Present in §8? |
> | :--- | :--- | :---: |
> | `.raid tele molten` | `mc` (Molten Core) | ❌ |
> | `.raid tele blackwing` | `bwl` (Blackwing Lair) | ❌ |
> | `.raid tele zulgurub` | `zg` (Zul'Gurub) | ❌ |
> | `.raid tele naxxramas` | `nax` (Naxxramas, inside) | ❌ |
> | `.raid tele onyxia` | `onyxia` (Onyxia's Lair, inside) | ❌ |
> | `.raid tele st` | `st` (Sunken Temple) | ❌ |
> | `.raid tele zf` | `zf` (Zul'Farrak) | ⚠️ mapped, but **no `game_tele` row exists on map 209** — it reports "not found" |
>
> `.raid tele st` in particular is worth knowing: without it there is no listed
> teleport to Sunken Temple at all.

---

## 9. C++ Engine Contracts & Technical FAQ

### Why `.bot gear * epic` instead of `epics`?

In `modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp` (line **3178**; the factory is constructed at :3180):

```cpp
if (param == "purple" || param == "epic")
{
    PlayerbotFactory factory(bot, bot->GetLevel(), ITEM_QUALITY_EPIC);
    factory.EquipGear();
    return "random epic gear equipped";
}
```

The engine string parser performs an exact string equality check on `"epic"` or `"purple"`. Passing `"epics"` fails the check and outputs `"unknown gear command"`.

### Why does `.bot prep *` take no quality argument?

In `PlayerbotMgr.cpp` (line **3252**):

```cpp
std::string PlayerbotHolder::HandleBotPrepare(Player* bot, Player* master, const std::string param)
{
    uint32 level = master ? master->GetLevel() : bot->GetLevel();
    PlayerbotFactory factory(bot, level, ITEM_QUALITY_NORMAL);
    factory.Refresh();
    return "consumes/regs added";
}
```

`HandleBotPrepare` does not evaluate `param`. It only calls `factory.Refresh()`, which resets cooldowns, restores health and mana, and restocks food, water, and reagents. Quality belongs exclusively on the `.bot gear` command.

### Why does `.dc on` require a tank bot?

In `modules/mod-dungeon-clear/src/DungeonClearCommand.cpp` (line 62):

```cpp
if (!DungeonClearDispatch::DispatchToTankBots(issuer, action, param))
    handler->SendSysMessage("No tank bot found in your group.");
```

The autonomous solver does not take over arbitrary bots. It specifically searches
your group for a bot whose **role is tank**, and role comes from the installed spec
strategy (`PlayerbotAI::IsTank`).

⚠️ **`.bot group` does NOT assign that spec when it creates a bot for a tank slot.**
`HandleCreate` does call `AutoSelectTalents(newBot, out, role)`, but the role it
receives is **`BOT_ROLE_NONE`**, and the line is **`:2680`, not `:2684`.**
`HandleGroup`
rolls a role at `PlayerbotMgr.cpp:2825`, uses it only to pick a class (`:2831`),
and forwards **no `role=`** in the create string (`:2855`), so `HandleCreate` keeps
`BOT_ROLE_NONE` (`:2536`, written only at `:2562-2563`) and
`AutoSelectTalents(newBot, &out, role)` at **`:2680`** receives that.
`ChangeTalentsAction::getPremadePaths` skips its role filter when the role is
`BOT_ROLE_NONE` (`:170`) and `PickPremadePath` (`:230`) picks a **random** premade
spec for the class. **A tank-role spec is therefore not something `.bot group`
gives anyone.**

`.raid roles` does **not** assign it either. The one
command that does is **`.raid tanks <n>`** — `talents do protection` followed by
`reset ai` (`raid_automation.lua:1965-1972`). If your tank is not actually
protection/feral-specced, promote one with `.raid tanks <n>` (or check first with
`.raid check`).

### ⚠️ Greater Blessing of Kings vs. multiple blessings — a rule with no caller

> 🚨 **Nothing in the pipeline implements this, and this handbook contradicts it
> four other times.** Read the banner, then skip the paragraph.
>
> - **`.raid buff` / `.raid wbuff` / `.raid worldbuff` send nothing.** They only
>   `Say()` an explanation (`raid_automation.lua:3499-3506`). The whole script
>   contains **zero** `CastSpell` call sites — stated in
>   [the §5.1 command reference](#51-complete-dc-command-reference-engine-verified),
>   in the Stage 5 note in
>   [§3](#3-the-master-1-click-pipeline-raid-ready) and in
>   [§13.3](#133--roster--who-is-here-and-what-they-can-do). The
>   "Instant Raid Buffs" Stage 5 this FAQ belongs to was **removed**, together with
>   the per-class aura table that sat underneath it.
> - **A per-class aura table would now be actively harmful, so reintroducing it
>   would be a regression, not a fix.** The engine already handles auras per spec:
>   `buff` is in every default combat kit and each spec strategy wires its own.
>   A table keyed on class would send **`water shield` to every shaman**,
>   overwriting the engine's correct per-spec choice — enhancement should get
>   *lightning shield*, restoration *water shield*. That single wrong-aura case is
>   the reason the table must not be reintroduced.
> - **The one-blessing-per-caster rule below is still true game rules**, and the
>   reasoning about *why* a single shared blessing would be the right choice for a
>   hand-rolled table is sound. It is simply **moot**: no such table exists, and
>   this is not the layer where aura selection belongs.
>
> If you ever find yourself restoring this FAQ's table, that is a regression. Fix
> the spec strategy instead.

**The underlying game rule, for reference:** in Classic WoW, a player can only
receive **one blessing per Paladin caster**. When an Eluna script iterates through
multiple Greater Blessings from the same caster, each blessing overwrites the
previous one.
Greater Blessing of Kings (Spell ID `25898`) provides a flat +10% increase to
Strength, Agility, Stamina, Intellect, and Spirit across all classes without
generating negative threat interactions (such as Blessing of Salvation on a tank).

---

## 13. The `.raid` Operator Facade (v1.4.0)

> ⚠️ **Numbering note.** This section is §13 and sits before the encounter
> encyclopaedias on purpose — it is command reference, and the encyclopaedias
> (§10–§12) are left where they are. Read it after [§5.1](#51-complete-dc-command-reference-engine-verified);
> that table is the short form and this is the long one.

The stated purpose of this handbook is to **eliminate manual command
micromanagement**. In v1.4.0 `.raid` is not a thin convenience layer over `.dc`
and `.bot` — it is a **five-layer operator facade** with 12 + 27 + 19 + 4 + **6**
commands, an optional `@filter` prefix on every order that talks to bots, and a
built-in trap register.

```text
Layer        What it is for                                    Size
──────────────────────────────────────────────────────────────────────
BATTLE       run control, mod-dungeon-clear                      12
ROSTER       who is here, what they can do, what they are running 27
TOGGLES      operator switches — every one now has an off form   19
MOVE         getting bots where they need to be                   4
PASS         escape hatches to the raw engines, plus two
             no-op diagnostics (preview, pending)                  6
```

> ⚠️ **The total is 68 verbs, not 66** — the last layer is the one that moved.
> v1.4.0 added two PASS commands, **`.raid preview`** and **`.raid pending`**
> (`raid_automation.lua:1262` and `:1267`). BATTLE 12, ROSTER 27, TOGGLES 19 and
> MOVE 4 are all still correct — they match the `COMMANDS` table (Lua
> `:1170-1268`) counted by `LayerCount()` (`:2297-2311`), which is also what the
> bare `.raid` index prints.

Two functional gaps were closed in v1.3.0, and they were the two that
actually hurt:

- **`.raid group <size>`** wraps **step 1**, so the documented order is executable
  from inside `.raid`: `.raid group size=20`, and it is the same string
  `.bot group size=20` takes. ⚠️ Note what "step 1" means here: it **fills the
  roster**, it does not assign specs. The spec step is `.raid tanks <n>`, and the
  verify step is `.raid check`.
- **Every toggle has an off form.** In v1.2.0 `loot` / `rest` / `aoe` /
  `gather` / `avoid` / `roll` could be turned on and nothing could turn them off
  again — the dispatch was a bare chain of `elseif sub ==` branches with no
  inverse names. That is the entire reason the TOGGLES table exists in the script.

> **On the leading dot.** Everything below is written the way you type it, **with
> the leading dot**: `.raid all @frost co +aoe`, `.raid pull dynamic`. The
> `.raid bot` / `.raid all` / `.raid one` forms show the **engine string** they
> produce without the dot, because that is what the script passes to
> `RunCommand` — see the convention note at the top of
> [§6](#6-in-dungeon-recovery-doors-aoe--target-focus).

### 13.1 What changed, and why the facade exists

Three v1.2.0 behaviours were actively misleading, and all three are now fixed:

- **Verbs & Diagnostics**: v1.2.0 had ~25 verbs and no diagnostics at all; v1.3.0 expanded to **66 verbs** across five layers (12 + 27 + 19 + 4 + 4), including live per-bot strategy, gear score, spell list, position, and avoid-list queries.
- **Toggles**: v1.2.0 had one-way toggles; v1.3.0 gives every toggle an explicit `no...` off form, and `.raid unfocus` is genuinely new.
- **Error Handling**: v1.2.0 dumped a 40-line help screen on unrecognised words; v1.3.0 prints a one-line "no such command" plus closest match ("Did you mean X?" within edit distance 3, "Closest: X" otherwise).

The v1.2.0 help screen is still reachable as `.raid help full`. Nothing was
deleted — the druid coin-flip note, the cosmetic-marker note, the Naxx routing
warning and the MT-election warning all live in there.

#### v1.3.0 → v1.4.0

All rows verified against `run/lua_scripts/raid_automation.lua` (**v1.4.0**):

- **PASS Layer Expansion**: v1.3.0 had 4 PASS verbs (`all`, `one`, `dc`, `bot`). v1.4.0 expands to **6 verbs**, adding **`.raid preview <sub> [args]`** (`:2809-2851`) for dry-run buffer verification and **`.raid pending`** (`:2858-2861`) for step queue depth inspection.
- **Bounded Deferred-Step Queue**: v1.3.0 fired multi-step commands back-to-back immediately, risking silent partial failure. v1.4.0 drains commands via `MAP_EVENT_ON_UPDATE` (event 23) at **one step per map tick** (`STEPS_MAX = 64`, `STEP_MAX_AGE_TICKS = 200`), with `pcall` on every step.
- **Ordered Promotion**: v1.3.0 sent `talents do <spec>` and `reset ai` back-to-back. v1.4.0 enforces ordered two-consecutive queue steps per bot (`:1955-1972`) to guarantee strategies rebuild from the new talent tab, aborting if the queue is refused.
- **Direct Runtime Error Reporting**: v1.3.0 had zero `pcall` wrapping on dispatch. v1.4.0 wraps dispatch in `pcall` (`:3639-3654`), reporting mid-command failures directly to operator chat.

### 13.2 ⚔️ BATTLE — run control

All of these forward to mod-dungeon-clear, which elects its own leader tank. The
script never addresses a tank for DC.

```lua
-- ============================================================================
-- BATTLE RUN CONTROL COMMANDS (.raid RUN DISPATCH)
-- ============================================================================

-- .raid start [<off|on|dynamic>]
-- Pipeline in exact order: dc bosses -> dc pull <m> -> dc on -> dc status.
-- dc bosses is ungated and checks anchors; dc pull pre-sets mode before start.
-- dc on is issued exactly once (re-running it wipes skip and progress state).
.raid start
.raid start dynamic
.raid start off
.raid start on

-- .raid stop / .raid off
-- Broadcasts: dc off (tears down run).
.raid stop
.raid off

-- .raid pause / .raid resume
-- Broadcasts: dc pause pause / dc pause resume.
-- Explicit intent forms prevent racing DC internal auto-pause sites. Preserves boss progress.
.raid pause
.raid resume

-- .raid go <boss>
-- Broadcasts: dc go <boss> (matches numeric entry or case-insensitive name).
-- Clears pause automatically as part of intent (go is itself a resume).
.raid go 15509
.raid go Kurinnaxx

-- .raid skip
-- Broadcasts: dc skip (abandons current anchor or due event).
.raid skip

-- .raid pull <off|on|dynamic>
-- Broadcasts: dc pull <mode>. Bare sets dynamic; invalid words clamp to dynamic.
.raid pull dynamic
.raid pull off
.raid pull on

-- .raid status
-- Broadcasts: dc status (leader view of run state and stall reasons).
.raid status

-- .raid bosses
-- Broadcasts: dc bosses (pre-flight check; works before dc on).
.raid bosses

-- .raid config
-- Broadcasts: dc config (read-only dump of all active DC settings).
.raid config

-- .raid spectate [follow <n>|next|prev|list]
-- Broadcasts: dc spectate <rest> (camera on YOU, not tank; do not spectate from inside raid mid-run).
.raid spectate
.raid spectate follow Tankbot
.raid spectate next
.raid spectate prev
.raid spectate list
```

> **On the leading dot, one more time, because it is the single most common
> mistake with `go`:** `.raid go onyxia`, not `.raid go` followed by a chat
> message. The argument is parsed from the same line.

#### Which verbs have no authorization gate

⚠️ **`dc bosses` is NOT the only verb with no authorization gate.** Of the **nine**
`Execute` handlers in `DungeonClearChatActions.cpp`, **six** call
`IsAuthorized(bot, event)` and **three** do not:

```lua
-- ============================================================================
-- DUNGEON CLEAR HANDLER AUTHORIZATION GATES (DungeonClearChatActions.cpp)
-- ============================================================================

-- Gated handlers (require authorization via IsAuthorized(bot, event)):
--   DcOnAction:                  Execute at :223, gated at :225
--   DcOffAction:                 Execute at :355, gated at :357
--   DcSkipAction:                Execute at :394, gated at :396
--   DcGoAction:                  Execute at :896, gated at :898
--   DcPauseAction:               Execute at :1075, gated at :1077
--   DcPullAction:                Execute at :1185, gated at :1187

-- Ungated handlers (no authorization check required):
--   DcStatusAction:              Execute at :480 (none)
--   DcBossesAction:              Execute at :517 (none - safe pre-flight query)
--   DcResumeOnDoorOpenedAction:  Execute at :1159 (none - engine door auto-resume)
```

So among verbs an operator can *type*, **`dc bosses` and `dc status` have no
authorization gate.** (`DcResumeOnDoorOpenedAction` is the third ungated handler,
but it is not a chat verb at all — it is the engine's own auto-resume when a door
opens.)

⚠️ **Still run `dc bosses` as a pre-flight.** It works before `dc on` has ever
been issued, and an empty anchor list is exactly what `dc on` will refuse on.

### 13.3 📋 ROSTER — who is here, and what they can do

#### The commands

```lua
-- ============================================================================
-- ROSTER COMPOSITION AND PREPARATION COMMANDS
-- ============================================================================

-- .raid group <size>
-- STEP 1: Roster filler (not spec step). Fills group picking random role and eligible class.
-- AutoSelectTalents receives BOT_ROLE_NONE and picks random spec. Refuses above 40 (avoids uint8 wrap).
.bot group size=20
.bot group size=40

-- .raid roles (alias .raid setup)
-- Census, formations by role, co +focus rti targets on @dps. Sends no talent commands.
.raid roles
.raid setup

-- .raid ready [<quality>] (alias .raid auto)
-- Pipeline: roles + gear + attune. Takes only 'best' and 'epic' (coerces anything else to best).
.raid ready best
.raid ready epic
.raid auto

-- .raid gear [<quality>] (alias .raid prep)
-- bot gear * <q>, enchants, train, prep, consumes, food, potions, regs, ammo, repairitems, replenish.
.raid gear best
.raid gear epic
.raid prep best

-- .raid attune (alias .raid keys)
-- Quest add + complete, master keyring additem.
.raid attune
.raid keys

-- .raid tanks <n> [names...]
-- Re-spec path: talents do <spec> + reset ai per bot in bounded queue.
.raid tanks 2
.raid tanks 2 MainTank OffTank

-- .raid tank2 / tank3 / tank4 / tank5 [<name>]
-- Re-spec targeted or named bot for that specific tank slot.
.raid tank2 Tankbot
.raid tank3 OffTank

-- .raid marks <tank...>
-- Assigns rti <icon> + co +mark rti per tank in priority order.
.raid marks MainTank OffTank

-- .raid marks <tank> <icon>
-- Sets specific icon for named tank (refuses 'moon').
.raid marks MainTank skull

-- .raid unmark [<tank>]
-- Broadcasts: co -mark rti on named tank or roster.
.raid unmark
.raid unmark MainTank

-- .raid clearmarks
-- SetTargetIcon on operator GUID to clear active icons.
.raid clearmarks

-- .raid ccmark (alias .raid moon)
-- Group:SetTargetIcon(4, target) - sets CC Moon on target.
.raid ccmark
.raid moon

-- .raid skull (aliases: .raid mark, .raid focus)
-- Group:SetTargetIcon(7, target) - sets Skull on target.
.raid skull
.raid mark
.raid focus

-- .raid buff / .raid wbuff / .raid worldbuff
-- Explains engine aura handling per spec (zero CastSpell calls).
.raid buff
.raid wbuff

-- .raid list
-- Broadcasts: bot list (lists all bot accounts owned by player).
.raid list

-- .raid add <name>
-- Broadcasts: bot add <name> (logs in character or entire account).
.raid add BotName

-- .raid remove <name>
-- Broadcasts: bot remove <name> (logs bot out; does not delete).
.raid remove BotName
```

The layer totals **27**: the 12 command-table rows above, the diagnostics below,
and 15 names that predate the command table — the `roles` / `ready` / `gear` /
`tank3` family and their aliases, all of which are listed inline here.

#### The diagnostics — the genuinely new capability

These are the first thing to reach for when a bot is not behaving.

```lua
-- ============================================================================
-- LIVE BOT DIAGNOSTICS & INSPECTION
-- ============================================================================

-- .raid check [<name>]
-- Broadcasts: bot r all ? or bot w <name> all ?
-- Every bot prints live installed strategy set for all 4 engines (co :49, nc :50, de :51, react :52, all :53).
.raid check
.raid check Tankbot

-- .raid rc
-- Broadcasts: bot r ready
-- Reports per-bot HP/MP, distance, and food/water/potion stock. Answers raid ready-check.
.raid rc

-- .raid gearscore (alias .raid gs) / .raid who
-- Broadcasts: bot r who
-- Class, talent spec, level, gear score, and equipped-item quality counts (WhoAction.cpp:127).
.raid gearscore
.raid gs
.raid who

-- .raid stats
-- Broadcasts: bot r stats
-- Gold, bag slots, repair cost, XP, power (does NOT print gear score).
.raid stats

-- .raid spells [<name>]
-- Broadcasts: bot r spells or bot w <name> spells
-- Spell list bot has actually learned (confirms real spec vs class default).
.raid spells
.raid spells Tankbot

-- .raid where [<name>] (alias .raid pos)
-- Broadcasts: bot r where
-- Coordinates and location of each bot (finds stragglers that did not follow).
.raid where
.raid pos

-- .raid avoidmob <id>
-- Broadcasts: bot r avoid creature <id>
-- Avoid list management: bare prints list, -<id> removes, comma-separated adds.
.raid avoidmob
.raid avoidmob 1234,5678
.raid avoidmob -1234
```

**Two PASS-layer diagnostics belong with these, because they answer questions
about a command you have not sent yet.** Both are documented in full in
[§13.6](#136--pass--escape-hatches-to-the-raw-engines), and both send nothing:

```lua
-- ============================================================================
-- PASS-LAYER PREVIEW & QUEUE DIAGNOSTICS
-- ============================================================================

-- .raid preview <sub> [args]
-- True dry run: executes DispatchRaidCommand under pcall with DRY=true and Emit routed to buffer.
-- Prints exact numbered command list it would send without sending anything. Refuses nesting.
.raid preview tanks 2
.raid preview ready epic
.raid preview all @tank formation shield

-- .raid pending
-- Reports queue depth for deferred attune/tanks steps:
-- Prints: "Step queue: N pending (cap 64, T ticks since load)".
.raid pending
```

> ⚠️ **`.raid check` falsifies a claim the v1.2.0 script header made** — that no
> chat command could report a bot's live strategy set. **`all ?` does exactly
> that.** The script still cannot *parse* chat it does not read back, so it still
> cannot use the answer to choose tanks automatically; but you can. See also
> [Tank marking](#tank-marking--one-icon-per-tank).

**Read `.raid check` once, after `.raid group`, not twice.** It is a full dump
per bot, and on a 40-man it is a wall. What you are looking for is the spec
strategy (`protection`, `holy`, `frost`, …) and `tank assist` / `dps assist`,
which come from the engine's default kit rather than from anything the script
sent. ⚠️ **This run is not optional, and it is the step that replaces the
`.bot group`-assigns-the-spec belief.** Because `HandleGroup` forwards no `role=`,
the spec strategy in this dump is a **random pick from the class's whole premade
list** — so `protection` may be absent from every single bot. Look for
`protection` specifically, then run `.raid tanks <n>` for the count you need, then
`.raid check` again only if a promotion misbehaved.

### 13.4 🎚️ TOGGLES — every switch now has an off form

```lua
-- ============================================================================
-- STRATEGY TOGGLES (ON / OFF BROADCASTS)
-- ============================================================================

-- AOE:
.raid aoe        -- co +aoe (prioritise AoE damage)
.raid noaoe      -- co -aoe (single-target)
.raid single     -- co -aoe (alias of noaoe)

-- Looting:
.raid loot       -- nc +loot (vacuum corpses)
.raid noloot     -- nc -loot (walk past corpses)

-- Resting / Drinking / Eating:
.raid rest       -- nc +food +nc +potions (eat/drink full)
.raid drink      -- nc +food +nc +potions (alias of rest)
.raid food       -- nc +food +nc +potions (alias of rest)
.raid norest     -- nc -food +nc -potions (stop resting)

-- Gathering:
.raid gather     -- nc +gather (pick up herbs/ore while walking)
.raid nogather   -- nc -gather (ignore nodes)

-- Hazard avoidance:
.raid avoid      -- co +avoid aoe (step out of AoE hazards)
.raid noavoid    -- co -avoid aoe

-- Loot rolls:
.raid roll       -- nc +roll (roll on drops)
.raid noroll     -- nc -roll (ignore loot rolls)

-- DPS Focus Lock:
-- Note: '.raid roles' sets focus lock (co +focus rti targets on @dps).
.raid unfocus    -- co -focus rti targets (release DPS focus lock)

-- Opening / Breaching:
.raid open       -- u (inert in engine; aliases: .raid use, .raid breach)

-- Raid Target Icons:
.raid ccmark     -- Group:SetTargetIcon(4, target) [Moon / CC]
.raid skull      -- Group:SetTargetIcon(7, target) [Skull / Focus]
.raid clearmarks -- Clears active target icons
```

**`unfocus` is genuinely new.** `.raid roles` turns the DPS focus-lock on, and
before v1.3.0 **nothing could turn it off**. That matters in exactly the
situations where you want it off — after a break, or when a healer is assigned to
follow a add.

There is deliberately **no `focus` alias on the focus-lock on-form**, even though
that would read naturally. `focus` is already the skull-icon command, and
stealing the name would silently break it. The on-form of the focus lock is
`.raid roles`, which sends the same string.

**`single` is retained** as an alias of `noaoe` — it predates v1.3.0, and removing
it would be a behaviour change rather than a cleanup.

Every one of these strategy names is mutated by `Engine::ChangeStrategy`
(`Engine.cpp:1160-1194`), which splits the comma list and dispatches on the first
character: `+` adds, `-` removes, `~` toggles. Removing a strategy the bot does
not have is a **silent no-op, not an error**, which is why the off-forms are safe
to send blind — and why an off form can also be used to *clear* a strategy a
script other than yours installed.

### 13.5 🚚 MOVE — soft regroup versus hard teleport

```lua
-- ============================================================================
-- MOVE COMMANDS (SOFT REGROUP VS HARD TELEPORT)
-- ============================================================================

-- .raid follow [<name>]
-- Engine string: bot r follow or bot w <name> follow
-- Soft regroup: bots WALK to you and hold. FollowChatShortcutAction (ChatShortcutActions.cpp:35-66)
-- resets AI, then ChangeStrategy("+follow,-passive,-stay,-wander") in noncombat and "-stay,-guard,-wander" in combat.
-- Holds formation slot (custom formation or GetFollowAngle()). Movement, not a teleport.
.raid follow
.raid follow Tankbot

-- .raid summon (alias .raid rally)
-- Engine string: bot summon *
-- Hard regroup: every bot is TELEPORTED to you immediately (only fix for navmesh traps).
.raid summon
.raid rally

-- .raid tele <alias>
-- Engine string: tele group <alias>
-- Group teleport to instance entrance (SEC_DEVELOPER, requires selected grouped player).
.raid tele ony
.raid tele mc
.raid tele aq40

-- .raid unbind (alias .raid resetlock)
-- Engine string: instance groupunbind all
-- Clears instance lockouts. WARNING: '.raid reset' does NOT exist (renamed in v1.2.0).
.raid unbind
.raid resetlock
```

The two are opposites that are constantly confused, which is why they are in the
same layer with a note rather than scattered.

### 13.6 🔓 PASS — escape hatches to the raw engines

**Six**, and they are **last** on purpose. An operator who genuinely needs a raw
verb should not have to read source to find it; an operator who is guessing should
not have 55 verbs to guess from. The last two rows are **no-op diagnostics**, and
they belong here precisely because they answer "did that raw verb actually do
anything?".

```lua
-- ============================================================================
-- PASS LAYER ESCAPE HATCHES (RAW ENGINE PASSTHROUGH)
-- ============================================================================

-- .raid all [@filter] <cmd>
-- Engine string: bot r [@filter] <cmd> — broadcast order to whole roster or filtered role.
.raid all @tank formation shield
.raid all @heal formation far
.raid all co +aoe

-- .raid one <name> <cmd>
-- Engine string: bot w <name> <cmd> — order sent to single named bot.
.raid one Tankbot co +aoe
.raid one Healerbot formation stay

-- .raid dc <sub> [args]
-- Engine string: dc <sub> [args] — passes directly to 10 live DungeonClear verbs.
-- ('test' is refused as GM-only test harness).
.raid dc status
.raid dc go Kurinnaxx

-- .raid bot <verb> [args]
-- Engine string: bot <verb> [args] — allowlisted passthrough to playerbot commands.
.raid bot list
.raid bot add BotName

-- .raid preview <sub> [args]
-- Engine string: none (dry run). Re-runs subcommand with DRY=true and Emit routed to buffer.
.raid preview roles
.raid preview tanks 2

-- .raid pending
-- Engine string: none (reports queue depth of deferred attune/tanks steps).
.raid pending
```

#### `.raid preview <sub> [args]` — the dry run

`raid_automation.lua:2809-2851`. **It exists because the engine ignores
unrecognised commands SILENTLY**, so a typo in a raw verb is indistinguishable
from a success. `.raid preview` removes the ambiguity: it prints the exact
command list a subcommand **would** send, and sends **nothing**.

How it works: it sets `DRY = true`, then **re-invokes the real
`DispatchRaidCommand`** for your subcommand under `pcall` with `Emit` routed into
a buffer rather than to the server, then prints the buffer as a numbered list and
clears `DRY`. It is the *same* code path as the live command, so what you read is
what you would get — it is not a reimplementation that can drift.

```text
.raid preview roles
.raid preview tanks 3
.raid preview all @frost co +aoe
```

```
.raid preview roles would send 5 command(s). Nothing was sent:
   1. bot r @tank formation shield
   2. bot r @heal formation far
   ...
```

- A subcommand that really is a no-op prints `would send nothing` — which is
  itself the answer you wanted.
- **It refuses to nest**: `Refusing to nest a preview inside a preview.`
- **One honest limitation**: the previewed command's own narrative `Say()` output
  still prints and is **not** buffered. That is deliberate — buffering it would
  mean routing the messaging helpers too, which is a much wider change than a dry
  run needs. So read the numbered list and ignore the prose above it.
- A runtime error inside the previewed branch is caught and reported as
  `Preview of '<sub>' failed: <err>` — no `pcall` swallowed it silently.

#### `.raid pending` — is the deferred batch draining or stuck?

`raid_automation.lua:2858-2861`. One line of output:

```text
Step queue: 12 pending (cap 64, 37 ticks since load).
```

Read it like this:

```lua
-- ============================================================================
-- .raid pending QUEUE STATUS READINGS
-- ============================================================================

-- '0 pending'
-- Meaning: Nothing queued. The last batch finished.
-- Action:  Carry on.

-- 'A small N, T climbing'
-- Meaning: Normal. The queue runs one step per map tick (Lua :1461-1487).
--          A 40-man .raid tanks is 2 steps per bot (~40 ticks for 20 bots).
-- Action:  Wait.

-- 'N near cap 64'
-- Meaning: Queue full. Further .raid attune / .raid tanks refused:
--          "Step queue full (N queued, 64 max) - '<label>' refused".
-- Action:  Wait for it to drain. Nothing is half-applied.

-- 'T past ~200 with N not falling'
-- Meaning: Stuck. STEP_MAX_AGE_TICKS = 200 (:1430) aborts batch and reports:
--          "'<label>' waited more than 200 ticks without completing; batch abandoned."
-- Action:  Investigate failing step; '.eluna reload' rebuilds queue from scratch.
```

`.raid pending` is also the **first thing to run after any command that failed
mid-way** — the `pcall` wrapper's own error message tells you to use it
(Lua `:3648-3650`).

> **`preview` and `pending` are diagnostics, not orders.** They sit in the PASS
> layer because that is where "I am about to hand something to a raw engine"
> lives, and both answer a question about that handoff. Neither appears in any
> other layer's count, and neither sends a single command — `.raid preview` says
> so on every run, and `.raid pending` has no engine string at all.

> `bot w` reaches **every character on the named account**, not just one
> character: the name is resolved with `GetAccountId` and fanned out across that
> account's characters. "I named one bot" and "I named an account" are different
> outcomes, and this is the one that behaves unexpectedly.

**Bare `.raid dc` is still `.raid start`.** The v1.2.0 alias is not taken away —
with no argument it starts the run, with an argument it is the pass-through, and
the two do not overlap.

`.raid all` outside a raid is the one case worth warning about: `bot r` answers
`Sender is not in a raid group` and reaches nobody, so the script says so and
points you at `.raid one`.

#### The `.raid bot` allowlist (38 verbs)

```text
group add login remove logout rm list
gear equip train learn enchants ammo
food drink potions pots
consumes consumables consums
regs reg reagents prepare prep
summon recall come pet
w p g r rl
debug read clear record
```

#### The refused `.bot` verbs

`m_holderHandlers` (13) + `m_botCommandHandlers` (42) register **55 verbs**
between them. **38 are forwarded above and 17 of the registered 55 are refused
below** — and the script's refusal table carries 19 names, because it also lists
two that are *not* in the handler map at all: `refresh` (advertised, implemented,
never registered) and `!` (a GM-only shorthand that is not a `bot` verb). Every
refusal message states the reason and the source line. That is the whole teaching
value of the facade: a refusal with no reason is indistinguishable from the facade
not knowing.

```lua
-- Refused .bot verbs and why they are blocked

-- delete: Permanently deletes the character!
-- init: Randomises gear (HandleBotInit, PlayerbotMgr.cpp:3260). Distinct from prep — it is NOT an alias of it; .raid prep is the safe one.
-- do: Multi-word is broken. PlayerbotMgr.cpp:2023 re-reads an invariant param on every loop iteration, so trimming is discarded and 'tank attack' can never resolve. Only single-word actions work; use '.raid all tank attack' instead.
-- c: Inert. Forces "monstertalk " + param into cdebug, DebugAction prints usage line, handler returns "ok" (reports success while doing nothing).
-- tweak: No-op. sPlayerbotAIConfig.tweakValue = sPlayerbotAIConfig.tweakValue++ assigns increment to itself and discards it; value never changes. GM-only, takes no args.
-- cmd: Duplicate of .raid one, with a worse error message.
-- reload: SEC_GAMEMASTER, and reloads the entire AI config mid-raid.
-- help: Refused so it cannot shadow .raid help (the operator surface). Use '.raid help chat' for raw chat-command list.
-- refresh: Advertised in engine help at PlayerbotMgr.cpp:3414 and implemented at :3348 (HandleBotRefresh) — but absent from handler map (returns "unknown command").
-- levelup / level / always / random / self / spoof / create / runtest / test: Not exposed.
--   'always' flips whether you are treated as a self-bot, not a raid switch.
--   'self' turns caller into a self-bot.
--   'spoof' overrides who the "master" is (headless-testing hook).
--   'create': Does NOT bypass role-matched spec assignment (HandleGroup assigns none). What it bypasses is the class-shape loop and its caps (AllowedClassRoleNr) — you get exact class/level asked with no role quota, risking zero tanks. (HandleCreate honours 'role=' if passed, but .bot group never does).
-- !: Not a bot verb at all — GM-only "select every bot" shorthand. Refused so error is legible.
```

### 13.7 🏷️ FILTERS — the `@token` prefixes

An optional leading `@token` now works on **any** `.raid` command that talks to
bots, spliced in before the payload:

```text
.raid all @frost co +aoe
.raid @heal all avoid creature 1234
.raid @tier7-8 all stats
.raid one @tank Tankbot1 co -mark rti
```

The engine applies it at `PlayerbotAI.cpp:1606` (`chatFilter.Filter(trim(...))`),
**before** the command itself is parsed, and **each bot evaluates it against
itself** — which is how one order is aimed at a subset without the script knowing
a single talent. A bot that does not match returns an empty string and drops the
message entirely. Stacked filters are **AND**, not OR: every one must match. The
filter list is `CompositeChatFilter`,
`modules/mod-playerbots/src/playerbot/ChatFilter.cpp:1224-1240`.

```lua
-- Filter Families (CompositeChatFilter: modules/mod-playerbots/src/playerbot/ChatFilter.cpp:1224-1240)

-- 1. Role Filters:
--    @tank, @notank, @heal, @noheal, @dps, @nodps, @melee, @ranged
--    Note: And nothing else. There is no @caster, no @nomelee, no @noranged.

-- 2. Class Filters:
--    @warrior, @paladin, @hunter, @rogue, @priest, @shaman, @mage, @warlock, @druid
--    Note: Exactly nine. @deathknight is registered only under #ifdef MANGOSBOT_TWO (ClassChatFilter, ChatFilter.cpp), so absent on this core.

-- 3. Race Filters:
--    @human, @tauren, @orc, @undead, @dwarf, @nightelf, @gnome, @troll, ...
--    Note: Whatever Player::getRace() reports, lowercased.

-- 4. Level Filters:
--    @60, @10-20
--    Note: Bare number after @. No @lvl prefix, no < / > / = operator.
--    CAVEAT: This filter swallows text because any leading @<digits> reads as a level.

-- 5. Gear Tier Filters:
--    @tier1 ... @tier10, or @tier7-8
--    CAVEAT: Bands in GearChatFilter (ChatFilter.cpp:312-331) are TBC-era and wrong for Classic curve:
--    tier 1 is gs 61-69, nothing scores 100-119, tier 4 jumps straight to 120-132. Approximate only. No @gs, no @ilvl.

-- 6. State Filters:
--    @needrepair, @bagfull, @bagalmostfull, @inside, @outside
--    Note: @dead, @alive, @combat, @noncombat are NOT filters in this tree (@dead= is strategy-membership).

-- 7. Strategy State Filters:
--    @co=<s>, @noco=<s>, @nc=<s>, @nonc=<s>, @dead=<s>, @nodead=<s>, @react=<s>, @noreact=<s>
--    Note: The key is checked, but strategy name is not. ChatFilter.cpp:73-84 tests HasStrategy against substring. Typo silently matches nobody.

-- 8. TalentSpec Filters:
--    @<spec> — e.g., @frost, @holy, @arms, @elemental, etc.
--    Full spec set is specs[] table in ChatHelper.cpp:150-190.
--    TRAPS with spaces/typos:
--    - Druid feral tree is spelled 'feral combat' (with a space). ChatFilter.cpp:1008 prefix tests message.find("@" + specName) == 0.
--      '@feral' matches nobody! Must match full token or nothing.
--    - Hunter tree is spelled 'beast mastery' (with a space). '@beast' matches nobody!
--    - Rogue assassination is spelled 'assasination' (single s) in source: working token is '@assasination'.
```

> ⚠️ **An unrecognised `@token` is not safely ignored.**
> `LocationChatFilter` is late in the chain and treats any unclaimed `@…` message
> as a map or zone name, so a typo tends to be **dropped rather than passed
> through**. The script refuses an unknown `@token` outright, with the closest
> match, rather than forwarding something the engine would swallow.

### 13.8 ❓ HELP — the index, the layers and the typo hint

Bare `.raid` is a **one-line-per-layer index**, each line carrying the count of how
many commands sit behind it, plus pointers to the trap register and the raw
chat-command list. (The script's own comment calls it "the five-line index" — it
was written when there were five layers; `filters` is the sixth.)

```lua
-- Help Layer Index Commands

.raid                   -- The index (also: .raid help, .raid ?)
.raid help battle       -- 12 run-control commands
.raid help roster       -- 27 roster commands
.raid help toggles      -- Toggle table (on and off strings side by side)
.raid help move         -- Soft vs hard movement differences
.raid help pass         -- Escape hatches (4 raw passthroughs, preview and pending diagnostics)
.raid help filters      -- The @token reference
.raid help traps        -- The footguns and dead-end engine strings
.raid help chat         -- Raw chat-command list (useful / fights-DC / irrelevant)
.raid help full         -- The legacy v1.2.0 wall of text, preserved whole
```

**Every help line prints the underlying engine string it emits.** That is
deliberate: the complaint v1.3.0 answers is *"I have to learn all the dc and
playerbot commands"*, and hiding the engine string behind a nicer name only moves
the learning somewhere less obvious. Showing it means `.raid` **teaches** the raw
verbs instead of replacing them.

An unrecognised `.raid <word>` now gives a one-line "no such command" plus the
**closest** matching command — "Did you mean X?" when it is within edit distance 3,
"Closest: X" otherwise — instead of the 25-line help dump v1.2.0 produced. Same
for `.raid dc <verb>` and an unknown `@token`.

### 13.9 🚨 Traps — strings that look like they worked and did not

This is the most useful page in the script and the most important subsection of
this section. **Each of these fails silently and reports no error.** If any macro
in this handbook or in [Volume 02](../02_playerbot_and_raid_automation.md)
contains `warning`, remove it.

#### 🚨 `warning` is not a warning — it makes the entire raid run away

`strategy/generic/ChatCommandHandlerStrategy.cpp:302` maps `warning` to
`NextAction("runaway chat shortcut")` → `GoawayChatShortcutAction`
(`ChatActionContext.h:159`). **`bot r warning` makes the whole raid walk away
from the fight.**

Worse, it needs no command at all: `PlayerbotAI.cpp:1623-1628` turns **any
`CHAT_MSG_RAID_WARNING` whose text contains the bot's name** into the `warning`
command by itself. **Type a raid warning that names one of your bots and that bot
runs.**

#### The other dead and misleading names

```lua
-- Misleading and Dead Playerbot Command Names (Silent Failures)

-- attackers: Dead end. TriggerNode exists (:314) and AttackersValue exists (ValueContext.h:249), but no action named 'attackers' is registered in any action context. DoSpecificAction finds nothing; can never execute.
-- use / u: Silently ignored. Both create trigger named 'use' (ChatTriggerContext.h:34-35); only node is named 'u' (:150) which nothing maps to. UseAction only reachable from LootAction.cpp:230 and RpgSubActions.h:292, never from chat. (.raid open sends 'u', so it is inert too).
-- inventory (also: items, inv): Maps to trigger 'c' (ChatTriggerContext.h:36-38) -> "item count" -> TellItemCountAction. Reports item count only; does NOT open bags or loot.
-- gbank: GUILD bank only (ChatTriggerContext.h:76 -> GuildBankAction, ChatActionContext.h:151). On bot without a guild, does nothing.
-- .bot c: Inert. Forces monstertalk into cdebug and returns "ok" without performing action.
-- bot rl: NOT a leader-only broadcast. Queues CHAT_MSG_RAID identical to 'bot r', then claims it transferred leadership (it did not; reply string is false).
-- attack rti 8: Broken by design. AttackRTITargetAction discards parameter and reads 'rti target' value. Icons must be selected by name ('rti skull', 'rti moon'); GetRtiIndex returns -1 for numbers.
-- formation back: Not a formation. Valid set: melee, queue, chaos, circle, line, shield, arrow, near, default, far, custom. Healers default to far.
-- dc pull leeroy: Not a user mode. 'leeroy' is an autonomous pull outcome chosen by Dynamic governor per pack.
-- stats: Prints gold, bags, repair cost, XP, and power. Does NOT print gear score. Use 'who' for gear score.
-- ready: Works by coincidence across two files. Chat word creates trigger named 'ready check' (ChatTriggerContext.h:119). Node fires from WorldPacketHandlerStrategy supported list (:16) via pass-through, NOT from dead TriggerNode("ready") at ChatCommandHandlerStrategy.cpp:318.
```

#### `castnc` is **not** dead, despite appearances

It is listed in the trigger table (`ChatTriggerContext.h:96`) and it looks like a
dead end, because `ChatCommandHandlerStrategy` registers no such node. **It is not
dead:** `generic/RpgStrategy.cpp:210` registers
`TriggerNode("castnc") -> NextAction("cast custom nc spell")` →
`CastCustomNcSpellAction` (`ChatActionContext.h:164`). It works.

That is the general lesson, and it is worth stating once because getting it wrong
produces false "this is broken" findings:

> **A playerbot chat command is live only if all three of these hold:**
> 1. a **Trigger** of that name exists in `ChatTriggerContext.h`;
> 2. a **TriggerNode** of that name is installed by a *live* strategy — either as
>    an explicit `TriggerNode(...)`, or automatically from any strategy's
>    `supported` list via `PassTroughStrategy::InitNonCombatTriggers`
>    (`strategy/generic/PassTroughStrategy.h:13-19`);
> 3. that node points at an **Action** that exists in one of the three contexts
>    `AiObjectContext.cpp:34-36` installs.
>
> Miss (1) and `ExternalEventHelper.h:84` returns false and the command vanishes
> with no output. Miss (2) or (3) and `DoSpecificAction` finds nothing, also
> silently.
>
> **Absence from `ChatCommandHandlerStrategy.cpp` does not prove a command is
> dead.** That mistake produced two false "broken" findings in this very audit.

---

## 10. Blackwing Lair Automated Raid Operations & Technical Reference

### 10.1 Quick-Start 1-Click BWL Deployment

```text
-- 1. Assemble a 40-man roster using alt accounts (or .bot group size=10 in 10-man mode)
.bot add sam_tanks
.bot add sam_healers
.bot add sam_melee
.bot add sam_ranged

-- 2. Teleport the entire raid directly to Blackwing Lair entrance
.raid tele bwl
.bot summon *

-- 3. Verify specs, then PROMOTE REAL TANKS, then gear/attune
--    .bot add creates characters; it never assigns a role-matched spec, and
--    neither does .bot group. BWL needs an MT and an offtank, and
--    "sam_tanks" is an account name, not a talent tab.
.raid check
.raid tanks 2
.raid roles
.raid ready

-- 4. Engage autonomous raid clearing
.raid start
```

> ⚠️ **Do not skip `.raid tanks 2` in a BWL run.** `.raid start` ends in `dc on`,
> which refuses with `No tank bot found in your group.` unless a bot's *installed
> spec strategy* is protection or tank-feral. `.raid check` is how you find out
> whether one is; `.raid tanks 2` is how you make one. See
> [the `.dc on` tank requirement](#-that-role-does-not-come-from-bot-group-read-this-before-you-trust-dc-on).

### 10.2 Encounter Operations & Engine Automations

```lua
-- Blackwing Lair Encounter Automations (mod-dungeon-clear & mod-playerbots)

-- Razorgore the Untamed (Entry 12435)
-- Phase 1 (4-Corner Defense):
--   Raid deterministically spreads across 4 room corners:
--     North: -7539, -1052
--     South: -7651, -1054
--     East:  -7615, -1105
--     West:  -7576, -1001
--   Each corner team (tanks, healers, DPS) holds corner post ('move to razorgore corner') and defends against local spawns ('attack razorgore corner add').
--   Target priority: Mages (12420) burned first, then Legionnaires (12416). Bots never abandon corner post to chase across the room.
-- Dragonkin Hibernate:
--   Druid bots cast Hibernate (Spell 2637/8952/8953) on DEATH_TALON_DRAGONSPAWN (12422) via 'razorgore sleep dragonkin'.
--   Slept dragonkin are marked with breakable CC in PossibleAttackTargetsValue::HasBreakableCC and strictly ignored by all bots so sleep is never broken.
-- Egg Destruction: Player controls Razorgore via Orb (GO 177808) to destroy the 30 eggs while bots protect room corners.
-- Phase 2 Burn:
--   Razorgore receives Warming Flames (23040); adds flee (EvadeTroops).
--   Raid collapses on Razorgore ('attack entry::12435').
--   Offtank executes instant 'taunt razorgore' on Conflagration (23023) to prevent boss turning on raid.
--   Fire Protection Potions used for Fireball Volley (22425).

-- Vaelastrasz the Corrupt (Entry 13020)
-- Essence of the Red (23513): Bots run unconstrained maximum burst rotations.
-- Burning Adrenaline (23620): Mana users run out 25yd ('move away from raid::25').
-- When MT receives Burning Adrenaline, offtank taunts immediately ('vael tank switch' -> 'taunt vael') and dying MT runs to sacrifice wall.
-- Flank Stance: Raid stays strictly on flanks to avoid Flame Breath and Tail Sweep.

-- Suppression Room (Area)
-- Trap Disarm: Rogues stealth and disarm Suppression Devices (GO 179784 via Spell 1842).
-- Hazard Keep-Out: Registered in DcHazardRegistry (15yd keep-out for camps/standoff; vacate 0 so rogues disarm unimpeded).
-- Moving Camp: DcPullPlanner maintains continuous upward progress to prevent infinite whelp respawns.

-- Broodlord Lashlayer (Entry 12017)
-- Knock Away (18670): Knocks active tank and cuts threat by 50%. Offtank taunts ('broodlord tank switch' -> 'taunt broodlord').
-- Wall Position: MT anchors back against corner wall to prevent being punted into suppression room.
-- Blast Wave (23331): Casters and healers stay > 20 yards away.
-- Mortal Strike (24573): Healers pre-cast heavy heals on tank.

-- Firemaw (Entry 11983)
-- Shadow Flame Auto-Clear: 'clear shadow flame dot' removes DoT 22682 automatically without requiring Onyxia Scale Cloaks (Item 15138) on all 40 bots.
-- Flame Buffet (23341): Bots hide behind doorway pillar ('hide from firemaw') when stacks >= 3.
-- Wing Buffet (23339): Offtank steps into LoS and taunts ('taunt firemaw') right before Wing Buffet to absorb threat wipe.

-- Ebonroc (Entry 14601)
-- Shadow of Ebonroc (23340) [CRITICAL FIX]: Tank swap triggered immediately when MT receives aura 23340 ('ebonroc tank switch' -> 'taunt ebonroc'), starving Ebonroc of 25,000 HP healing per swing.
-- Shadow Flame Auto-Clear: 'clear shadow flame dot' cleanses DoT 22682 on contact. Inactive tank hides out of line of sight.

-- Flamegor (Entry 11981)
-- Frenzy (23342): Hunter bots cast Tranquilizing Shot (Spell 19801) within 1 second.
-- Wing Buffet: Standard offtank taunt swap ('taunt flamegor').
-- Shadow Flame Auto-Clear: 'clear shadow flame dot' cleanses DoT 22682 immediately.

-- Chromaggus (Entry 14020)
-- Portcullis Lever (DC Event 1): Laboratory lever (GO 179148) pulled automatically via BlackwingLairEvents.cpp once Firemaw, Ebonroc, and Flamegor are defeated, opening iron gate 179116.
-- Brood Affliction Dispels:
--   Mages/Druids decurse Black (23154).
--   Priests/Paladins dispel Blue (23153).
--   Priests/Shamans cure Red (23155).
--   Druids/Shamans cure Green (23169).
--   Bronze (23170) cured via 'clear chromaggus bronze'.
-- Time Lapse (23310): Offtank taunts Chromaggus upon MT threat wipe.
-- Breath LoS: Raid hides behind ramp wall during devastating breath casts.
-- 20% Enrage: Full cooldown and execute burn.

-- Nefarian (Entry 11583)
-- Throne Gossip (DC Event 2 & Roster Patch): Lord Victor Nefarius (10162) gossiped automatically on the throne (-7526.27, -1135.04, 473.45). Menu options drilled down to start trial.
-- Phase 1 Add Defense: Tanks hold South/North doors; AoE burn 42 drakonids.
-- Dynamic Spawn Wait: Event uses WaitForSpawn(11583) across wave combat until dragon form lands, then triggers KillCreatureEngage.
-- Shadow Flame Auto-Clear: 'clear shadow flame dot' removes DoT 22682 without requiring cloaks.
-- Class Calls:
--   Priests: Stop direct heals.
--   Shamans: Destroy corrupted totems (14662–14666).
--   Warriors: Shield Wall.
--   Rogues: Reposition after head teleport.
-- Phase 3 (20% HP): Nefarian raises Bone Constructs (14605). Mages AoE kite while raid burns Nefarian.
```

### 10.3 Cross-Instance Scripting Audit & Proven Tactical Patterns (Beyond MC & ZG)

To ensure robust autonomous clearing and reliable bot behaviors across all vanilla and Turtle-WoW content, our raid and dungeon operations leverage proven patterns implemented throughout `mod-playerbots` and `mod-dungeon-clear`. Below outlines how multi-wave defense, split-positioning, priority adds, and crowd control are unified across encounters:

```lua
-- Cross-Instance Scripting Audit & Proven Tactical Patterns

-- 1. Blackwing Lair (Map 469, 40-Man)
--    Razorgore the Untamed (12435):
--      4-Corner Defense: Raid splits into 4 balanced squads (North, South, East, West) via RazorgoreCornerIndex(ai). Bots garrison their assigned corner ('move to razorgore corner') and burn local wave spawns ('attack razorgore corner add').
--      Dragonkin Hibernate: Druids cast Hibernate (Spell 2637/8952/8953) on DEATH_TALON_DRAGONSPAWN (12422).
--      Breakable CC Protection: PossibleAttackTargetsValue::HasBreakableCC treats Hibernate, Sleep, Blind, and Wyvern Sting as breakable CC, strictly preventing bots from attacking slept targets while other enemies live.
--      Phase 2 Tank Swap: Offtanks taunt Razorgore on Conflagration (23023) to prevent boss turning on raid.
--    Suppression Room:
--      Rogue Stealth & Disarm: Rogues enter stealth, approach traps ('move to suppression device'), and disarm Suppression Devices (GO 179784 via Spell 1842).
--      15yd Keep-Out Zone: Registered in DcHazardRegistry so non-rogue bots never trigger the slowing aura.
--      Moving Camp: DcPullPlanner maintains steady forward progress to avoid infinite whelp spawns.
--    Drake Trio (Firemaw, Ebonroc, Flamegor):
--      Shadow Flame Cleanse: Auto-cleanses DoT 22682 to eliminate Onyxia Scale Cloak requirements.
--      Shadow of Ebonroc Swap: Immediate offtank taunt on aura 23340 to starve Ebonroc of 25,000 HP healing per swing.
--      Tranquilizing Shot: Hunters soothe Frenzy (23342) on Flamegor within 1 second.
--      LoS Corner Peeking: Bots reset Flame Buffet stacks (23341) behind doorway pillars.

-- 2. Naxxramas (Map 533, 40-Man)
--    Gothik the Harvester (16060):
--      Deterministic Room Split: Gothik requires >= 10 players on each side of the combat gate (HasLessPlayersPerSide). Raid divides into Dead Side and Live Side via GothikSideIndex(ai) and walks to assigned post ('move to gothik post').
--      Wave Prioritization: Bots burn 18 waves of Unrelenting and Spectral adds before Gothik teleports down into the arena.
--    Four Horsemen (16062–16065):
--      4-Corner Tank Rotation: Tanks rotate Thane, Mograine, Zeliek, and Blaumeux across 4 room corners to drop stacking marks before reaching lethal stacks.
--      Void Zone Evasion: Bots detect Void Zones (28863) and vacate within 1 second ('move away from void zone').
--    Instructor Razuvious (16061):
--      Dual Priest Mind Control: Priests mind control Deathknight Understudies (16803), immediately activating Understudy Shield Wall (29061) followed by Taunt (29060).
--      Mind Exhaustion Rotation: Priests alternate understudies to cycle around the 60-second Mind Exhaustion (29051) lockout.
--    Gluth (15932):
--      Zombie Chow Kiting: Mages and Hunters snare and kite Zombie Chows (16360) away from Gluth to prevent Decimate healing.
--      Mortal Wound Swap: Offtanks taunt at 3–4 stacks of Mortal Wound (25646).
--    Thaddius (15928):
--      Polarity Shift: Bots detect Positive (28059) and Negative (28084) charges and move to designated left/right charge camps to avoid devastating raid wipes.

-- 3. Temple of Ahn'Qiraj (Map 531, 40-Man)
--    The Prophet Skeram (15263):
--      Illusion Splits: At 75%, 50%, and 25%, Skeram creates 2 copies. Offtanks pick up copies while casters interrupt Arcane Explosion.
--      Mind Control CC: Players afflicted with True Fulfillment (785) are sheeped, blind, or trapped until the effect expires.
--    Bug Trio (Kri, Yauj, Vem):
--      Synchronized Burn: Killing one bug causes survivors to devour the corpse for a 100% full heal. Bots balance damage across all three targets before executing them simultaneously.
--    Twin Emperors (15275 / 15276):
--      Teleport Swap: Vek'nilash (Physical immune to spells) and Vek'lor (Caster immune to melee) swap positions every 30–40 seconds. Tanks and healers re-anchor across 60yd to avoid mutual heal link (23957).

-- 4. Ruins of Ahn'Qiraj (Map 509, 20-Man)
--    General Rajaxx (15341):
--      7-Wave Defense: Bots garrison entrance ramp, defending General Andorov (15471) against 7 waves of officers (15391 -> 15392 -> 15389 -> 15390 -> 15386 -> 15388 -> 15385 -> 15341).
--      180s Timeout Prevention: Bots stay in combat at the ramp to avoid wave reset timeouts (generic_scripts 153912).
--    Kurinnaxx (15348):
--      Sand Trap Avoidance: 15yd silence/slow hazard keep-out (GO 180647).
--      Mortal Wound Tank Swap: Offtanks taunt when MT reaches 5 stacks of Mortal Wound (25646).
--    Moam (15340):
--      Mana Fiend Priority: Mana Fiends (15527) burned immediately; Priests/Hunters drain Moam's mana to delay catastrophic Arcane Eruption (25672).

-- 5. Stratholme (Map 329, 5-Man)
--    Slaughter Square & Baron Rivendare:
--      13 Abomination Cleansing: Full square clear triggers Ramstein the Gorger (10439).
--      Mindless Undead Wave: 33 mindless undead surge from the gates; Mages/Paladins AoE burn the wave in center square.
--      Black Guard Ambush: 5 Black Guard Sentinels walk gate route to Baron's ziggurat; tanks intercept before door opens (STR_GO_BARON_DOOR 175796).

-- 6. Zul'Farrak (Map 209, 5-Man)
--    Pyramid Wave Event:
--      Ramp-Head Garrisoning: Bots hold coordinates at top of ramp (ZF_RAMP_X/Y/Z), intercepting 100 descending Sandfury trolls before they reach Sergeant Bly and Weegli Blastfuse.
--      Gossip Door Trigger: Blastfuse blows open the door; bots defeat Bly's crew upon betrayal.

-- 7. Sunken Temple (Map 109, 5-Man)
--    Avatar of Hakkar (8443):
--      Blood Pool Suppression: Hakkari Bloodkeepers and Suppressors channel into Hakkar's pool. Offtanks peel suppressors while raid holds center to summon the Avatar.

-- 8. Blackrock Depths (Map 230, 5-Man)
--    The Lyceum & Shadowforge Braziers:
--      Dual Flame Keeper Hunt: 2 Shadowforge Flame Keepers drop braziers. Bots light both braziers within 5 minutes to unlock Iron Hall doors before infinite dwarf waves overwhelm the party.
--      Princess Moira Preservation: Offtank holds Princess Moira Bronzebeard (8929) with fears/stuns without killing her, ensuring Emperor Thaurissan is defeated while preserving Ironforge royal succession.

-- 9. Tower of Karazhan (Map 532 10-Man / Map 814 40-Man)
--    Netherspite & Prince Malchezaar:
--      Portal Beam Rotation: Bots rotate Dominance, Serenity, and Perseverance beams on Netherspite to starve boss buffs and empower the tank/healer.
--      Infernal Avoidance: Bots detect incoming Malchezaar Infernals and relocate outside 20yd fire impact radius.

-- 10. Emerald Sanctum (Map 807, 40-Man)
--     Sanctum Dragons:
--       Corrupted Treant Wave Burn: Treant adds rooted and burned before reaching Sanctum guardians; nature breath cones faced away from raid.
```

---

## 11. Onyxia's Lair Automated Raid Operations & Technical Reference

### 11.1 Quick-Start 1-Click Onyxia Deployment

```text
-- 1. Assemble raid roster (standard 40-man or 10-man tuned comp)
.bot add sam_tanks
.bot add sam_healers
.bot add sam_dps

-- 2. Teleport raid directly into Onyxia's chamber entrance
.raid tele onyxia
.bot summon *

-- 3. Verify specs, then PROMOTE REAL TANKS, then gearing/attunements/enchants/consumables
.raid check
.raid tanks 2
.raid roles
.raid ready

-- 4. Engage autonomous raid clearing
.raid start
```

### 11.2 Architectural Integration in `mod-dungeon-clear` & `mod-playerbots`

Onyxia's Lair (Map `249`, Single Encounter Raid) operates on a fully deterministic declarative event pipeline and a 3-phase AI strategy ladder:

1. **Dungeon Clear Core Registration**:
   • **`OnyxiasLairEvents.cpp`**: Declares `RegisterOnyxiasLairEvents` (Event 1: approach chamber, engage, and defeat Onyxia entry `10184`) and `RegisterOnyxiasLairRoster` (anchors the encounter objective at chamber center `-12.867f, -216.626f, -88.058f`).
   • **`DungeonEventRegistry.cpp` & `BossRosterRegistry.cpp`**: Registers map `249` into the global clear tables.
   • **`DcBossEntries1121.h`**: Explicitly maps `{ 249, 10184, 1 }` in `DC_BOSS_ORDER_1121`, anchoring Onyxia as the primary objective.
2. **Playerbot Strategy Ladder (`OnyxiaFightStrategy`)**:
   • `100.0f` — `onyxia deep breath` $\rightarrow$ `move away from onyxia` (immediate 40yd scatter off breath trajectory).
   • `95.0f` — `onyxia phase 3 landing threat` $\rightarrow$ `taunt onyxia landing` (Main Tank snap taunt upon 40% landing threat reset).
   • `90.0f` — `onyxia need tank north` $\rightarrow$ `tank onyxia north` (Main Tank pulls and anchors back against North wall `-8.86f, -212.75f, -88.54f`).
   • `80.0f` — `onyxia has warder` $\rightarrow$ `taunt onyxian warder` (Offtanks intercept Onyxian Warders entry `12129` at the tunnel entrance).
   • `75.0f` — `onyxia has whelps` $\rightarrow$ `attack onyxia whelps` (Melee and AoE DPS burn Onyxian Whelps entry `11262` from side caves).
   • `70.0f` — `onyxia airborne` $\rightarrow$ `attack entry::10184` (Ranged DPS burn airborne Onyxia; melee clear adds).
   • `60.0f` — `onyxia grounded` $\rightarrow$ `taunt onyxian warder` (Offtanks peel residual adds).
   • `55.0f` — `onyxia grounded` $\rightarrow$ `attack entry::10184` (Raid collapses focus on Onyxia).

### 11.3 Phase-by-Phase Encounter Operations & Critical Mechanics

```lua
-- Onyxia Phase-by-Phase Encounter Operations & Critical Mechanics

-- Phase 1: Ground (100% -> 65% HP)
-- Mechanics & Spell IDs:
--   Flame Breath (18435): Frontal fire cone dealing 3,500–4,500 damage.
--   Wing Buffet (18500): Frontal knockback reducing tank threat.
--   Knock Away (19633): Knocks active target and cuts threat by 25%.
--   Tail Sweep (15847): Rear knockback dealing 1,500 damage and punting players into whelp egg caves.
-- Autonomous Bot Tactics & Engine Behaviors:
--   North Wall Tank Anchor: Main Tank immediately runs to North wall (-8.86f, -212.75f, -88.54f) with back against the wall, turning Onyxia South. Guarantees Flame Breath and Wing Buffet never hit raid.
--   Flank Positioning: Ranged, melee, and healers position strictly on Onyxia's left or right flanks, completely avoiding frontal cone and rear tail sweep arcs.
--   Threat Stacking: Tank builds heavy Sunder Armor and Shield Slam threat before Phase 2 transition.

-- Phase 2: Airborne (65% -> 40% HP)
-- Mechanics & Spell IDs:
--   Lift Off (17131 / HOVER): Onyxia takes flight, dropping melee reachability.
--   Deep Breath (23461): Devastating fire carpet covering room quadrants.
--   Fireball (18392): Targeted bombardment on individual players.
--   Whelp Waves (11262): 16 initial whelps, followed by periodic waves from East and West egg caves.
--   Onyxian Warders (12129): Heavy melee elite dragonkin spawning from entrance tunnel.
-- Autonomous Bot Tactics & Engine Behaviors:
--   Airborne Detection: Trigger OnyxiaAirborneTrigger detects aura 17131. Melee DPS switch off boss.
--   Deep Breath Scatter: When Onyxia casts 23461, OnyxiaDeepBreathTrigger triggers MoveAwayFromOnyxiaAction (40yd scatter perpendicular to flight lane) within 500ms.
--   Add Prioritization: Offtanks immediately taunt and face Onyxian Warders (TauntOnyxianWarderAction). Melee and AoE casters prioritize Onyxian Whelps (AttackOnyxiaWhelpsAction).
--   Ranged Focus: Hunters, Warlocks, and Mages burn airborne Onyxia down to 40%.

-- Phase 3: Landing & Burn (40% -> 0% HP)
-- Mechanics & Spell IDs:
--   Threat Reset (DoResetThreat): Onyxia lands at 40% HP with a completely wiped threat table!
--   Bellowing Roar (18431): Room-wide fear cast every 15–30s.
--   Floor Eruptions (20009): Cracking lava fissures dealing 1,500–2,000 fire damage.
--   Residual Whelps & Warders: Ongoing spawns from Phase 2.
-- Autonomous Bot Tactics & Engine Behaviors:
--   Landing Threat Snap Taunt: OnyxiaPhase3LandingThreatTrigger detects grounding and health <= 40%. Main Tank executes instant TauntOnyxiaLandingAction and Shield Slam to lock aggro before healers or casters take lethal hits.
--   Re-Anchoring North: Main Tank repositions back to -8.86f, -212.75f, -88.54f.
--   Fear Counter-Measures: Shaman Tremor Totems, Warrior Berserker Rage, Priest Fear Ward, and Undead Will of the Forsaken maintain tank control.
--   Execute Burn: Offtanks clean up remaining whelps; entire raid pops cooldowns and burns Onyxia to 0%.
```

---

## 12. 10-Man Raids & Custom Dungeons Operations Guide

In addition to 40-man raid automation, our core features verified scripts, event triggers, and combat handling for 10-man raids and custom Turtle-WoW dungeons.

### 12.1 Lower Karazhan Halls (Map `532`, 10-Man Raid)

> ⚠️ **There are two Karazhans and they are easy to mix up.** Lower Karazhan
> Halls is `532` (`instance_lower_karazhan_halls`, `player_limit` 10). Map `812`
> is **not a row in `map_template`** (`sql/base/tw_world_map_template.sql`) —
> never type it.

Located in Deadwind Pass, Lower Karazhan Halls represents an essential stepping stone between Tier 0.5/Tier 1 and 20/40-man raiding.

#### Moroes & The Dinner Guests (`boss_moroes.cpp`)

Moroes spawns with 4 random undead dinner guests from a pool of 6 possible nobles. Understanding guest priorities and CC assignment is mandatory for flawless execution:

```lua
-- Moroes Dinner Guests (boss_moroes.cpp) - Priorities, Spells & Strategy

-- 1. Baroness Dorothea Millstipe (Shadow Priest)
--    Critical Spells: Mana Burn (29405 - drains 1,000+ mana from healers), Mind Flay (17313 - high channeled shadow damage).
--    Priority: KILL FIRST (Priority #1) or Priest Shackle Undead. Must not be allowed to free-cast Mana Burn on healer bots.

-- 2. Lady Catriona Von'Indi (Holy Priest)
--    Critical Spells: Greater Heal (29564 - massive 8,000+ HP heal on Moroes or guests), Power Word: Shield (29408 - damage absorption).
--    Priority: KILL FIRST / INTERRUPT (Priority #1). Rogues/Warriors kick/pummel Greater Heal; burst down immediately.

-- 3. Baron Rafe Dreuger (Retribution Paladin)
--    Critical Spells: Hammer of Justice (13005 - 6s stun on tank), Cleanse (19703 - dispels CC from other guests).
--    Priority: Priority #2 / Shackle. Offtank holds away from main tank; Paladin/Priest cleanses stun.

-- 4. Lord Robin Daris (Arms Warrior)
--    Critical Spells: Mortal Strike (16856 - 50% healing cut and heavy physical burst), Whirlwind (15589 - 360-degree melee cleave).
--    Priority: Priority #2 / Offtank / Shackle. Melee stay at max melee range; offtank turns outward.

-- 5. Lady Keira Berrybuck (Holy Paladin)
--    Critical Spells: Holy Light (29562 - heals allies), Blessing of Protection (13007 - physical immunity on nobles).
--    Priority: Priority #3 / Interrupt. Counterspell/Kick Holy Light; purge or wait out BoP.

-- 6. Lord Crispin Ference (Protection Warrior)
--    Critical Spells: Disarm (6713 - disarms tank for 5s), Shield Wall (29390 - 75% damage reduction).
--    Priority: TANK LAST. High armor and defenses make him inefficient to kill early. Offtank holds until Moroes is dead.
```

• **Moroes Garrote Triage**: Moroes uses Vanish (`29448`) and reapplies Garrote (`37066`), dealing 1,000 physical bleed damage every 3 seconds for 5 minutes. Healers maintain HoTs (Rejuvenation, Renew) on all Garrote victims. At 30% HP, Moroes Frenzies (`37023`), increasing physical damage by 50%—pop defensive cooldowns and burn.

#### Brood Queen Araxxna (`boss_brood_queen_araxxna.cpp`)

- **Web Wrap (`29234`)**: Encases players in cocoons. Melee and ranged bots immediately switch to the cocoon to free wrapped allies within 3 seconds.
- **Spiderling Swarms**: AoE Frost Nova, Blizzard, and Consecration burn spider waves before they reach healers. Nature Protection Potions minimize poison volley damage.

#### Clawlord Howlfang (`boss_clawlord_howlfang.cpp`)

- **Worgen Pack Cleave**: Howlfang summons ferocious Shadowfang Worgen. Tanks stack adds tightly for cleave; Hunters tranquilize enrage effects.

### 12.2 Upper Karazhan Halls (Map `814`, 40-Man Raid)

> ⚠️ **Upper Karazhan is `814` (`Karazhan (Upper)`, `player_limit` 40)**; the
> 10-Man Karazhan is `532` (§12.1). Map `812` is not a row in `map_template`.

#### King's Council (`boss_kings_council.cpp`)

- **Synchronized Tank Separation**: Multi-target noble encounter requiring 2–3 tanks to anchor each council member $\ge 20$ yards apart to prevent defensive aura overlaps.
- **Interruption Chains**: Shaman Earth Shock and Mage Counterspell rotations interrupt targeted destruction spells on a strict 8-second cadence.

#### Echo of Medivh (`boss_echo_of_medivh.cpp`)

- **Flame Wreath (`29946`)**: Players afflicted with Flame Wreath must **STAY COMPLETELY STILL**. Any movement by the afflicted target detonates the wreath, wiping the raid. Bots enter forced hold position (`ai->SetHold(true)`).
- **Blizzard LoS**: Raid steps behind pillars to avoid channeled Blizzard damage and movement slows.

### 12.3 Scarlet Citadel (Map `45`, 40-Man Raid Fortress)

*Live verified from `tw_world` and `src/scripts/dungeons/scarlet_citadel/` (`instance_scarlet_citadel`).*
Entrance coordinates: `(32.55f, 13.30f, 16.87f)` (accessible via `.tele ScarletCitadel` or `dc test scarlet-citadel`).

The Scarlet Citadel is Turtle WoW's flagship raid fortress, featuring rigorous positional, tether, hazard, and add-priority mechanics:

#### 1. Sacred Fist Daelus (`NPC_DAELUS = 2000000`, `boss_daelus.cpp`)

- **Tether to Tank (`SPELL_LIFE_DRAIN_VISUAL = 21157`)**:
  - If the active tank moves $> 3.0$ yards away (`GetDistance2d > 3.0f`), Daelus immediately channels Life Drain, inflicting **10% of maximum HP every second** to all players across the dungeon and healing himself for the total damage dealt.
  - *Automated Counter*: `daelus close tether` forces the tank bot to maintain tight melee contact ($\le 2.0\text{yd}$) at all times.
- **Poison Cloud (`NPC_POISON_CLOUD = 16363`, `SPELL_POISON_CLOUD = 28240`)**:
  - Daelus afflicts a random non-tank player with `SPELL_GREEN_GLOW_VISUAL` (`22577`). 6 seconds later, a persistent Poison Cloud drops, ticking 1100–1300 Nature damage per second for 60 seconds.
  - *Automated Counter*: `daelus poison cloud carrier` orders afflicted bots to sprint 25 yards outward toward the perimeter wall to drop the cloud safely away from the raid group. All bots immediately evacuate if near creature `16363` (`move away from poison cloud`).
- **Fallen Spirits (`NPC_FALLEN_SPIRIT = 2000013`) & The Chosen One (`SPELL_RED_COLOR = 26235`)**:
  - 6 Fallen Spirits spawn every 30 seconds. If Daelus consumes a normal spirit, he regenerates **5% max HP**.
  - One spirit periodically gains the red aura `26235` (the **"Chosen One"**, slowed to 0.6x speed, 1% HP). When the Chosen One reaches Daelus, Daelus enters Phase 2 (vulnerable for 30s, stunned, taking amplified damage).
  - *Automated Counter*: `attack daelus spirits` directs DPS to burn down all normal spirits, but **strictly ignores and protects the Chosen One** so it can safely reach Daelus and trigger the burn window.
- **Sunder Armor (`SPELL_SUNDER_ARMOR = 25051`)**: Stacks on the tank every 10s. Offtanks execute a taunt swap at $\ge 3$ stacks (`daelus tank swap`).

#### 2. Grand Sorcerer Ardaeus (`NPC_ARDAEUS = 2000001`, `boss_ardaeus.cpp`)

- **The Descending Sun (`NPC_SUN = 2000019`)**:
  - The Sun descends from the ceiling ($Z=40.0\text{f}$) toward the floor ($Z=18.5\text{f}$). If the Sun touches the floor, the entire raid wipes!
  - Every 25,000 damage dealt to the Sun pushes it back upward by 5.0 yards (`INCREASE_Z_AXIS = 5.0f`).
  - *Automated Counter*: `ardaeus sun descending` overrides DPS priorities to immediately burst The Sun whenever it moves below $Z=35.0\text{f}$.
- **Statue Echoes (`2000020`–`2000030`)**:
  - Ardaeus summons one of 11 statue echoes (Barean Westwind, Harthal Truesight, Valea Twinblades, etc.) into the center room (`255.17f, -99.99f, 18.68f`) every 30s.
  - *Automated Counter*: Offtanks pick up adds via `taunt ardaeus statue` while DPS focuses them down (`attack ardaeus statue`).

#### 3. High Inquisitor Mariella (`NPC_MARIELLA = 2000002`, `boss_mariella.cpp`)

- **Void Zones (`2000016`) & Kill Zones (`2000018`)**:
  - Void zones tick for 3,000 damage every 2s; Kill zones execute players every 500ms.
  - *Automated Counter*: `move out of mariella void zone` immediately steps bots 8 yards out of hazard boundaries.
- **Felhounds (`NPC_FELHOUND = 2000017`)**:
  - 4 Felhounds spawn from summoning circles (`5000012`) every 15 seconds, channeling `Drain Mana` (`25676`) for $-1250$ mana per tick from casters and healers.
  - *Automated Counter*: Offtanks intercept (`taunt mariella felhound`) while DPS burns them down with top priority (`attack mariella felhounds`).
- **Shadow Volley (`21341`)**: Raidwide shadow damage every 3.5s (1.0s in enrage at 8 min). Handled via Shadow Protection Potions and raid healing.
- **Sacrifice Phases**: At 75%, 50%, and 25% HP, Mariella channels `SACRIFICE_VISUAL` (`22518`), regenerating 1% HP/sec.

#### 4. Mini-Boss: Brother Eric Vesper (`NPC_ERIC_VESPER = 2000004`, `trashbosses_scarlet_citadel.cpp`)

- **Energize Wipe Mechanic (`SPELL_ENERGIZE = 25685`)**:
  - Eric channels Energize. If his mana reaches 100%, he casts `DoExplosion()`, instantly executing all players in the raid map!
  - *Automated Counter*: `eric vesper energizing` triggers maximum burst DPS cooldowns to destroy Eric before mana capacity is reached.

#### 5. Mini-Boss: Darkcaller Rayn (`NPC_DARKCALLER_RAYN = 2000037`)

- Shadowform (`16592`), Mind Control (`15722`), Shadow Volley (`21341`), Mind Flay (`26044`), Impending Doom (`19702`). Casters dispel curses and interrupt channels.

#### 6. High General Abbendis (`NPC_ABBENDIS = 2000003`, `boss_abbendis.cpp`)

- **Frontal Crusader Strike and Cleave**: Main Tank anchors Abbendis against the back wall (`421.21f, -8.98f, 37.70f`) via `tank abbendis`, turning the boss away from the raid while melee attacks safely from behind.
- **Enrage / Frenzy**: At low health, Abbendis enters an enraged state dealing amplified physical damage. Hunter bots immediately dispel via `tranquilizing shot` (`dispel enrage` trigger), and tanks pop defensive cooldowns (Shield Wall, Last Stand).
- **Sealed Portal Gate**: Portal door `GO_DOOR_ABBENDIS` (`5000010`) seals on pull; `SealedEncounterRegistry` ensures all 40 raid members are gathered inside before engagement.

#### 7. Dungeon Clear Subsystem Integration (Map 45)

- **Fight-In-Place Zones (`FightInPlaceRegistry`)**: Daelus arena (`[10, 115] x [-20, 50]`), Ardaeus solar sanctum (`[250, 310] x [-145, -85]`), and Mariella library (`[165, 220] x [25, 75]`) forbid hallway pullbacks to prevent anti-exploit teleports and boss resets.
- **Pre-Clear Rooms (`RoomAggroRegistry`)**: Daelus (clears Valiants `2000034`, Footmen `2000035`), Mariella (clears Inquisitors `2000033`, Rayn `2000037`), and Eric Vesper (clears Interrogators `2000036`, Chaplain `2000005`, Sister `2000006`).
- **Hazard Keep-Out (`DcHazardRegistry`)**: Void Zone (`2000016`), Kill Zone (`2000018`), Poison Cloud (`16363`) maintain 10yd standoff and 8yd active vacate.
- **Script-Only Doors (`DcEventDoorRegistry`)**: Gates `5000001` through `5000010` are recognized as script-driven, preventing desync attempts.

### 12.4 Karazhan Crypt (Map `800`, 5-Man Level 58–60 Dungeon)

> ⚠️ **Karazhan Crypt is `800`** (`instance_karazhan_crypt`, `player_limit` 5).
> Map `813` is `Winter Veil Vale`, a 0-player map with no instance script — never
> type it.

Custom high-level dungeon featuring tight subterranean corridors and punishing undead mechanics:

- **Corridor Navigation & Line of Sight**:
  - Narrow catacomb architecture requires tanks to pull packs backward into intersections to avoid body-pulling patrolling ghoul packs.
  - `DcPullPlanner` uses conservative camp placement to prevent bots from falling through uneven crypt mesh geometry.
- **Necromantic Ritualists**:
  - Must be stunned and burned first to prevent summons of Crypt Fiends and Shadow Phantoms.
  - Priests use Shackle Undead to control elite crypt defenders.

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Playerbot Handbook](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md)
