# 02. Playerbot & Raid Automation Handbook (Master Control Center)

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + PlayerBots Framework (ike3 / r-o-sh) + mod-dungeon-clear
> **Source Verification**: `modules/mod-playerbots/src/playerbot/`, `ChatTriggerContext.h`, `ChatActionContext.h`, `Formations.cpp`, `Stances.cpp`, `ChatFilter.cpp`, `modules/mod-dungeon-clear/`
> **Security Coverage**: Accessible to all players (`SEC_PLAYER` / Level 0) with elevated GM commands where noted
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md) • [🏰 Volume 06: Raid Operations](./06_automated_raid_and_dungeon_operations.md) • [🛡️ Volume 07: Guilds](./07_guilds_and_guild_operations.md) • [🌍 Volume 08: World & Strategies](./08_open_world_control_and_strategies.md)

---

## 📑 Table of Contents

1. [Unified 8-Volume Master Reference Architecture](#unified-8-volume-master-reference-architecture)
2. [Master Macro UI Cheat Sheet (Copy-Paste Ready)](#1-master-macro-ui-cheat-sheet-copy-paste-ready)
   - [Target Tokens & Scope Syntax](#target-tokens--scope-syntax)
   - [Category A: Group Assembly, Gearing & Preparation Macros](#category-a-group-assembly-gearing--preparation-macros)
   - [Category B: Live Group & Raid Combat Controls](#category-b-live-group--raid-combat-controls)
   - [Category C: Autonomous Dungeon Clear AI Deck (.dc)](#category-c-autonomous-dungeon-clear-ai-deck-dc)
   - [Category D: Post-Combat Recovery, Looting & Breaches](#category-d-post-combat-recovery-looting--breaches)
3. [Playerbot Core Control Suite (.bot)](#2-playerbot-core-control-suite-bot)
   - [Holder & Global Session Handlers](#holder--global-session-handlers)
   - [Targeted Bot Commands & Gearing Operations](#targeted-bot-commands--gearing-operations)
4. [RandomBot Population & Realm AI Manager (.rndbot)](#3-randombot-population--realm-ai-manager-rndbot)
   - [Realm Telemetry & Population Health](#realm-telemetry--population-health)
   - [World Bot Dispatch & RPG Simulation](#world-bot-dispatch--rpg-simulation)
5. [In-Game Whispers & Single-Letter Shortcuts Dictionary](#4-in-game-whispers--single-letter-shortcuts-dictionary)
   - [Fast Single-Letter Shortcuts](#fast-single-letter-shortcuts)
   - [Complete Whisper Command Directory](#complete-whisper-command-directory)
   - [Bot Cheats Subsystem](#bot-cheats-subsystem-w-bot-cheat)
   - [Bot Professions, Skills & Crafting](#bot-professions-skills--crafting)
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
   - [Pull Modes: Off / On / Dynamic](#pull-modes-off--on--dynamic)
   - [Scripted Dungeon Events & Navmesh Break Handlers](#scripted-dungeon-events--navmesh-break-handlers)
   - [Live Configuration Inspection (.dc config)](#live-configuration-inspection-dc-config)
   - [Companion Addon (1.12 Classic Port)](#companion-addon-112-classic-port)
   - [Spectator Camera Controls (.dc spectate)](#spectator-camera-controls-dc-spectate)
   - [Headless Automated 5-Bot Test Harness (.dc test)](#headless-automated-5-bot-test-harness-dc-test)
9. [Raid Automation Operator Facade (.raid)](#8-raid-automation-operator-facade-raid)
   - [Layer 1: BATTLE](#layer-1-battle)
   - [Layer 2: ROSTER](#layer-2-roster)
   - [Layer 3: TOGGLES](#layer-3-toggles)
   - [Layer 4: MOVE](#layer-4-move)
   - [Layer 5: PASS](#layer-5-pass)
   - [Filters: the @token Argument](#filters-the-token-argument)
   - [Help: Bare .raid and .raid help](#help-bare-raid-and-raid-help-layer)
   - [Traps: Commands That Fail Silently](#traps-commands-that-fail-silently)
10. [Class Combat Strategy Assignment Deck (/w %t)](#9-class-combat-strategy-assignment-deck-w-t)
   - [Tank Specializations (Warrior, Paladin, Feral Druid)](#tank-specializations)
   - [Healer Specializations (Priest, Shaman, Druid, Paladin)](#healer-specializations)
   - [Melee DPS Specializations (Rogue, Warrior, Paladin, Shaman, Druid)](#melee-dps-specializations)
   - [Ranged & Caster DPS Specializations (Hunter, Mage, Warlock, Priest, Druid)](#ranged--caster-dps-specializations)
11. [Raid Assembly, Attunement & Instance Reset Blueprint](#10-raid-assembly-attunement--instance-reset-blueprint)
    - [10-Man, 20-Man & 40-Man Raid Compositions](#10-man-20-man--40-man-raid-compositions)
    - [Instance Lockouts, Unbinding & Raid Resets](#instance-lockouts-unbinding--raid-resets)

---

## Unified 8-Volume Master Reference Architecture

| Volume | Handbook Title | Domain & Core Coverage | Direct Link |
| :---: | :--- | :--- | :---: |
| **01** | **[Game Master & World Administration](./01_game_master_handbook.md)** | **100% of GM commands**: Self progression/cheats (`.god`, `.levelup 60`), accounts, security (0–4), spawning (`.npc`, `.gobject`), economy, moderation, 106+ `.reload` tables & diagnostics. | [Open Volume 01](./01_game_master_handbook.md) |
| **02** | **Playerbot & Raid Automation** | **Complete Bot Control Center**: `.bot` & `.rndbot` suites, single-letter whisper shortcuts (`c`, `u`, `e`, `t`, `s`, `r`), chat filters (`@tank`, `@skull`), strategy tokens (`co`/`nc`), `.dc` autonomous dungeon clear solver & raid macros. | **Current File** |
| **03** | **[World, Dungeons & Teleports Directory](./03_turtle_world_and_teleports.md)** | **647+ .tele tokens**: Kalimdor, Eastern Kingdoms, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements. | [Open Volume 03](./03_turtle_world_and_teleports.md) |
| **04** | **[Class Specs & Endgame Gear Directory](./04_class_specs_and_endgame_gear.md)** | **All 9 Classes**: 1-Click BiS gear loadout macros, complete Tier 1/2/3 set IDs, weapons, trinkets, enchants & verified 1.18.1 talent builds across 30 distinct specializations. | [Open Volume 04](./04_class_specs_and_endgame_gear.md) |
| **05** | **[Eluna Scripting & Core Development Engine](./05_developer_eluna_and_engine.md)** | **Eluna Lua API**: Live reloads (`.reload eluna`), Event 42 Custom Command dispatch, Turtle-WoW `SpellInfo` compatibility layer, and production server hook mechanics. | [Open Volume 05](./05_developer_eluna_and_engine.md) |
| **06** | **[Automated Raid & Dungeon Operations](./06_automated_raid_and_dungeon_operations.md)** | **1-Click .raid ready pipeline**, instant raid attunements, master keyring spawner, autonomous `.dc` tank autopilot, and tactical macros. | [Open Volume 06](./06_automated_raid_and_dungeon_operations.md) |
| **07** | **[Guilds & Guild Operations](./07_guilds_and_guild_operations.md)** | **Complete Guild System**: `.bot <guild>` batch operations (`gear guild best`, `prep guild epic`, `summon guild`), charter signing, `/w <bot> guild` invites/promotions/demotions/kicks/transfers, guild bank (`gb`), and 25-guild realm simulation. | [Open Volume 07](./07_guilds_and_guild_operations.md) |
| **08** | **[Open World Bot Control & Strategies](./08_open_world_control_and_strategies.md)** | **Autonomous Open World Operations**: `.rndbot grind`, `.rndbot rpg`, `.rndbot teleport/revive/upgrade`, dynamic realm load balancing (diff/pid/clean), bot persistence (`.bot always`), complete strategy token encyclopedia (co/nc/dead/react), and full real-time feedback inspection suite. | [Open Volume 08](./08_open_world_control_and_strategies.md) |

---

## 1. Master Macro UI Cheat Sheet (Copy-Paste Ready)

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`).

### Target Tokens & Scope Syntax

- **`%t`**: Current Target. In a macro, `/w %t co +tank` resolves dynamically to the character or bot you currently have targeted.
- **`*`**: All Party Bots. `.bot gear * best` applies instantly to every bot in your active party simultaneously.

#### 🚨 There is NO chat channel that reaches a whole raid of bots

Three of the four routes work. The fourth is a debug stub that reports success
and does nothing.

| Route | Packet written | Reaches a bot? |
| :--- | :--- | :--- |
| `/w <bot> <cmd>` or `.bot w <bot> <cmd>` | `CHAT_MSG_WHISPER` | ✅ yes — one named bot |
| `/ra <msg>` or `.bot r <msg>` | `CHAT_MSG_RAID` | ✅ **yes — every bot in the raid.** One message instead of forty |
| `/p <msg>` or `.bot p <msg>` | `CHAT_MSG_PARTY` | ⚠️ yes, but **party chat only** — your own subgroup in a raid |
| `.bot c <bot> <cmd>` | — | ❌ **no one** (debug stub, see below) |

**How `/ra` reaches bots** — it is not the route you would guess, and this
handbook used to get it wrong. `.bot r` does not message the bots; it builds a
`CMSG_MESSAGECHAT` on **your own session**, which `HandleMessagechatOpcode` turns
into `PLAYERHOOK_ON_CHAT_COMMAND` → `PlayerbotMgr::HandleCommand`, and *that* has
no channel allowlist, so it dispatches to the whole roster. Full trace, and the
correction, in [Raid chat, and a wrong claim this handbook used to
make](#raid-chat-and-a-wrong-claim-this-handbook-used-to-make).

So:

* **`.bot r <msg>` and `/ra <msg>` work**, including the `@tank` / `@heal` /
  `@dps` / `@group1` chat filters, because every bot evaluates the filter for
  itself. They cannot address a *subset by name* — an order meant for the five
  melee bots also lands on the tanks and healers — so role orders either use a
  `@role` filter or two separate lines.
* **`.bot c <bot> <cmd>`** calls `DoSpecificAction("cdebug", Event(".bot",
  "monstertalk " + cmd))`. `DebugAction` matches `"monstertalk "` and calls
  `HandleMonsterTalk`, which only prints `Usage: debug monstertalk <text>`. It
  then returns `"ok"` to the caller, so the command *looks* successful. This
  single defect is what made an earlier version of `raid_automation.lua` inert.
* **`/p` genuinely works**, but in a 20–40 man raid party chat is one subgroup,
  so `/p @all co +aoe` reaches roughly ten bots, not the raid. Turtle-WoW
  40-man raids scale from 20 players, so confirm your real group size.

**Practical rule:** broadcast with `.bot r` for anything every bot should run
(`.bot r co +aoe`, `.bot r nc +food`), add a `@role` filter when the targets
differ (`.bot r @tank co +protection`), and whisper a single bot only when the
order is genuinely about that one bot (a talent path, or `tank3`). A `bot`
subcommand that takes `*` (`.bot gear * best`, `.bot consumes *`) iterates the
group server-side and is unaffected by channels entirely.

- **`@all`**: everyone in the channel that can see it — subject to the table above.
- **`@<role>`**: Role Filter — `@tank @dps @heal @notank @nodps @noheal @ranged @melee`. Only meaningful on `/p` or a whisper.
- **`@<class>` / `@<spec>`**: `@warrior @paladin @priest …` and `@protection @fury @holy @restoration @shadow @balance …`.
- **`@<rti>`**: Raid Target Icon Filter — `@star @circle @diamond @triangle @moon @square @cross @skull`.
- **`@group<N>`**: subgroup filter. **No space**: `@group1` works, `@group 1` parses to "0 to 0" and matches nobody.

---

## Raid chat, and a wrong claim this handbook used to make

`.bot r <msg>` and `/ra <msg>` **work**, and they always have. This handbook
previously said the opposite. The correction matters, because the reason it
worked is not the obvious one.

### The wrong story

The claim was that `/ra` reached nobody, because `CHAT_MSG_RAID` was missing from
the accepting set in `PlayerbotAI::ProcessIncomingChatPacket`, which fell through
to `default: return`.

That is a real property of that function — but it is the wrong function.
`ProcessIncomingChatPacket` is the **reactive** path: a chat message a bot
*receives*, which drives conversational replies, LLM replies and `ChatHelper`
link handling. It is not how a master issues orders.

### The path that actually carries orders

1. `bot r <msg>` (`PlayerbotHolder::HandleRaid`) builds a real `CMSG_MESSAGECHAT`
   carrying `CHAT_MSG_RAID` and queues it to **your own session** — not to the
   bots'.
2. Your session runs `WorldSession::HandleMessagechatOpcode`, which after
   validating the line fires `PLAYERHOOK_ON_CHAT_COMMAND`
   (`src/game/Handlers/ChatHandler.cpp`).
3. That hook is `PlayerbotScripts::OnChatCommand` → `PlayerbotMgr::HandleCommand`.
4. `PlayerbotMgr::HandleCommand` calls `ForEachPlayerbot` and, for each bot,
   `PlayerbotAI::HandleCommand`. It narrows recipients only for
   `CHAT_MSG_WHISPER` (by name), `CHAT_MSG_SAY` (25 yd) and `CHAT_MSG_YELL`
   (300 yd). **There is no channel allowlist**, so `CHAT_MSG_RAID` dispatches to
   the entire roster.
5. Per bot, `PlayerbotAI::HandleCommand` runs `chatFilter.Filter()`, which is
   what applies `@tank` / `@heal` / `@dps`. A bot the filter rejects gets an
   empty string back and returns without acting.

So `bot r co +aoe` reaches every bot, and `bot r @tank co +protection` reaches
only the tanks. **No core change is required for any of this.**

### What `ProcessIncomingChatPacket` does affect

Only the reactive path: with `CHAT_MSG_RAID` filtered out, bots would not
acknowledge or link-react to anything said in raid, even though the same words in
party chat worked. That is a real but minor gap, it only matters if you have
`sPlayerbotAIConfig.llmEnabled` or bot chat reactions turned on, and it is
**not** fixed in this tree — the change was reverted as unnecessary.

### Two limits worth knowing

* **A broadcast cannot address a subset by name.** `@tank` and `@dps` in the same
  message both fire. Use two lines when the targets differ.
* **Only the master is heard.** `if (HasRealPlayerMaster() && guid1 !=
  GetMaster()->GetObjectGuid()) return;` drops anything from anyone else, so bot
  chat cannot loop.

## Buff Spell IDs

```lua
-- Dungeon Buffs
.additem 22818
.additem 22817
.additem 22820

-- World Buffs
.additem 15366
.additem 23768
.additem 23736
.additem 23767
.additem 23766
.additem 23769
.additem 23738
.additem 23737
.additem 23735
.additem 16609
```

### Category A: Group Assembly, Gearing & Preparation Macros

> ⚠️ **These macros only work on a party-sized group.** `/p` reaches your party
> and nothing else, and `/ra` does not reach bots at all on this build (see
> [There is NO chat channel that reaches a whole raid of
> bots](#target-tokens--scope-syntax)). In a 20–40 man raid you must whisper
> bots individually, or use the `bot <verb> *` subcommands below, which iterate
> the group server-side. `.bot gear * best`, `.bot consumes *` and friends are
> unaffected by channel.

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

##### ⚙️ `.bot group` Mechanics (`f776c7a` Native Behavior)

- **`size=<2..40>`**: Target group size (2 to 5 for party; 6 to 40 automatically converts to raid). Default is 5.
- **How it works**: Evaluates missing party roles (Tank, Healer, DPS) using `LfgAction::AllowedClassRoleNr`, randomly selects appropriate classes for missing roles, and calls `HandleCreate` to generate fresh level-matched bots directly on your master account.
- **Pass-through parameters**: Any extra parameters passed to `.bot group` (e.g. `race=<race>`, `spec=<spec>`) pass through directly to `HandleCreate`.

> [!WARNING]
> **Account Character Limit (10 Max) & `"Account has max characters"`**:
> Because native `f776c7a` creates characters directly on your master account, it is bound by the 10-character realm limit (1 player + 9 bots). If your account already has 10 characters, running `.bot group` will fail with `"Account has max characters"`.

---

##### 🔍 How to Check & Delete Characters on Your Account

###### 1. Check Characters on an Account

- **List characters on any account (In-Game GM)**:

  ```text
  .account characters <AccountName>
  ```

  *(Displays all character names, levels, and GUIDs on that account).*

- **List active bots in world**:

  ```text
  .bot list
  ```

- **Inspect target's account**: Target any player/bot and type `.pinfo`.

- **Client login screen**: Log into the account with your WoW client to see the visual 10-slot character list.

###### 2. Delete Characters to Free Account Slots

- **In-Game (Quick Bot Delete)**:

  ```text
  .bot delete <BotName>
  ```

  *(Or target the bot and run `.bot delete`). If the bot was created on your account, this immediately and permanently purges it from the database, instantly freeing your character slot!*

- **Client Character Selection Screen**:
  Select the character &rarr; click the red **Delete Character** button &rarr; type **`DELETE`** to confirm.

- **Wipe an entire alt account**:

  ```text
  .account delete <AccountName>
  ```

---

##### 🛡️ Raids with Your Own Characters (Multi-Account Alt Bots)

Because a single account cannot exceed 10 characters, use **Multi-Account Alt Bots** (`AiPlayerbot.AllowMultiAccountAltBots = 1`) to field a 10, 20, or 40-man roster using your own custom characters without clogging your main account:

- **Do I need to create the accounts first?** **Yes.** Run `.account create <name> <password>`.
- **Should I make a couple of them?** **Yes**, based on your raid bracket:
  - **10-Man Raid**: 1 alt account (holds up to 10 characters)
  - **20-Man Raid**: 2 alt accounts (holds up to 20 characters)
  - **40-Man Raid**: 4 alt accounts (holds up to 40 characters)

###### 💡 Recommended Account Structure & 1-Click Summon

```text
-- 1. Create dedicated alt accounts in-game:
.account create sam_tanks password
.account create sam_healers password
.account create sam_melee password
.account create sam_ranged password

-- 2. Log into each account and create up to 10 characters with your desired specs/races

-- 3. Log onto your main character and summon entire account rosters at once:
.bot add sam_tanks     -- Adds all characters from sam_tanks
.bot add sam_healers   -- Adds all characters from sam_healers
.bot add sam_melee     -- Adds all characters from sam_melee
.bot add sam_ranged    -- Adds all characters from sam_ranged

-- 4. Teleport and fully prepare the raid:
.bot summon *
.raid roles             -- formations split by role, DPS focus lock
.raid ready             -- gear, enchants, consumables, attunements, master keys
```

> ⚠️ These characters were specced **by you**, on your own accounts — `.bot group`
> was not involved, so nothing has set their roles. `.raid roles` splits
> formations by role and reports whether the class mix can field the bracket, but
> it will **not** re-spec anyone: if a character is on the wrong tree, fix it by
> hand with `/w <name> talents do <spec>` then `/w <name> reset ai`. `.raid tanks
> <n>` promotes tank-capable bots if the group is short of tanks.

#### 📋 Macro 4: Party / Raid Ready Check

Queries all bots in party for health, mana, consumable stock, and readiness before pulling:

```lua
/p @all ready check
/p @all c ?
```

---

### Category B: Live Group & Raid Combat Controls

> ⚠️ **Every macro in this category is written for `/p`, which is party-scoped —
> in a 20–40 man raid that is one subgroup.** For a raid-wide order, change
> `/p` to `/ra` (which reaches every bot) or whisper bots individually
> (`/w BotName co +aoe`). Prefer individual whispers for anything
> role-specific: a broadcast cannot address a subset, so `@tank` and `@dps` in
> the *same* message both fire, whereas two separate whispers each reach only
> their target. `run/lua_scripts/raid_automation.lua` (the **live** copy — see
> [§8](#8-raid-automation-operator-facade-raid) for the stale duplicate that is
> not loaded) enumerates the raid and
> addresses each bot by name for exactly this reason. The macro bodies below are
> otherwise correct.

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
/p @all attack rti
```

> ⚠️ **`attack rti` takes no argument.** An earlier version of this macro used
> `/p @all attack rti 8`, which looks like it selects the skull by index and
> silently does not. `AttackRTITargetAction::Execute`
> (`strategy/actions/AttackAction.cpp:75`) never reads `event.getParam()` — it
> resolves the `rti target` **value**, which is the icon named by that bot's own
> `rti` string, and defaults to `skull`. The `8` is discarded. There is no
> numeric form anywhere in mod-playerbots; icons are selected by name
> (`rti skull`, `rti cross`, …) via `RtiTargetValue::GetRtiIndex`, which returns
> **-1** for anything that is not one of the eight literal words.
>
> To retarget a different icon, change the value first, then attack:
> `/p @all rti cross` then `/p @all attack rti`.

```lua
/p @all rti cross      -- point this bot's focus at the CROSS icon
/p @all attack rti     -- attack whatever that icon is on
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

Silences casting mobs. There is **no generic `interrupt` strategy** — `Engine::addStrategy` silently discards unknown names, so `co +interrupt` does nothing. Interrupts are per-class, and each bot's own class strategy already includes its interrupt by default (Kick/Pummel for warriors and rogues, Counterspell for mages, Hammer of Justice for paladins, Silence for hunters, and so on).

To force them on explicitly, name the class strategy that carries them:

```lua
/p @all co +fury raid
/p @all co +fire
/p @all co +shadow
```

`kick` also exists as a standalone strategy if you want a specific spell rather than the class kit.

#### 🧹 Macro 12: Mass Dispel & Cleanse Debuffs

Prioritizes healers and hybrid casters curing magic debuffs, curses, poisons, and diseases:

```lua
/p @all co +cure,+cleanse cure magic,+cleanse cure disease,+cleanse cure poison
```

---

### Category C: Autonomous Dungeon Clear AI Deck (`.dc`)

> ⚠️ These run in a **raid**, and the `/p` lines below reach only your subgroup.
> `/ra` **does** reach every bot (see [Raid chat, and a wrong claim this handbook
> used to make](#raid-chat-and-a-wrong-claim-this-handbook-used-to-make)), but a
> single broadcast cannot address a *subset by name*, so per-role orders need one
> order per role — `.bot r @tank …` and `.bot r @dps …` as separate lines, which
> is exactly what the Eluna controller does. Use
> `run/lua_scripts/raid_automation.lua` instead — the **live** copy; the
> same-named file at `lua_scripts/raid_automation.lua` is a stale v1.1.0 and is
> not loaded (see [§8](#8-raid-automation-operator-facade-raid)):
> `.raid roles` splits formations by
> role and focus-locks the DPS, and `.raid start` sequences `dc bosses` → `dc pull
> <mode>` → `dc on` → `dc status`. See §5.1 of
> `06_automated_raid_and_dungeon_operations.md`.

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
| `.bot list` | `.bot list [prefix]` | Dumps list of all active bots in world memory with online status, class, and level. The optional filter is a **name prefix**, not a substring. | `.bot list Priest` |
| `.bot help` | `.bot help [command]` | Displays built-in help documentation for playerbot commands. | `.bot help gear` |
| `.bot reload` | `.bot reload` | Reloads `aiplayerbot.conf` settings in-memory without server reboot. **`SEC_GAMEMASTER` only.** | `.bot reload` |
| `.bot tweak` | `.bot tweak` *(no arguments)* | **GM-only and a no-op — it cannot change any setting.** It reads and writes nothing; see the correction note below. | `.bot tweak` |
| `.bot self` | `.bot self [login]` | Toggles self-bot mode. It **always applies to the calling player only**; any name argument is ignored, so it is not a way to put account alts into self-bot mode. | `.bot self login` |
| `.bot spoof` | `.bot spoof <botname>` | Per-holder, and read **only when `master == nullptr`** — i.e. console only. In-game it is a **no-op**. | `.bot spoof Tankman` |
| `.bot group` | `.bot group [size=N] [level=N]` | **Auto-Party Generator**: Assembles a balanced party (Tank, Healer, DPS) matching level. | `.bot group size=5 level=60` |
| `.bot create` | `.bot create <key=val...>` | Generates a new bot character with explicit attributes: `name=`, `race=`, `class=`, `level=`, `role=`, `gear=`, `group=`, `login=1`, `temporary=1`. | `.bot create name=Dunpriest class=priest race=dwarf level=60 role=healer login=1` |
| `.bot p` | `.bot p <msg>` | Builds a `CMSG_MESSAGECHAT` / `CHAT_MSG_PARTY` packet on **your own session** — it is not a direct push to bots. | `.bot p follow` |
| `.bot g` | `.bot g <msg>` | Builds a `CMSG_MESSAGECHAT` / `CHAT_MSG_GUILD` packet on **your own session** — it is not a direct push to bots. | `.bot g hello guild` |
| `.bot r` | `.bot r <msg>` | Builds a `CMSG_MESSAGECHAT` / `CHAT_MSG_RAID` packet on **your own session**; `PlayerbotMgr::HandleCommand` then dispatches it to the whole roster. | `.bot r ready check` |
| `.bot rl` | `.bot rl <msg>` | **Does not transfer leadership.** It queues the *identical* `CHAT_MSG_RAID` packet as `.bot r`, then answers `Sent raid leader transfer request as <name>`. No transfer occurs. | `.bot rl Focus Skull!` |
| `.bot runtest` | `.bot runtest <action>` | **Does not exist in a normal build.** Registered only under `#ifdef GenerateBotTests`, and that macro is commented out, so it returns `unknown command`. | `.bot runtest combat` |

> [!NOTE]
> **Corrections to this table.** Three rows previously described behaviour the
> engine does not have.
>
> * **`.bot tweak` is not a config editor.** The whole handler is four lines
>   (`PlayerbotMgr.cpp:1721-1733`): a `SEC_GAMEMASTER` check, then
>   `sPlayerbotAIConfig.tweakValue = sPlayerbotAIConfig.tweakValue++;`, then a
>   `> 2 → 0` wrap, then a "Set tweakvalue to N" reply. It takes **no
>   arguments**, and the assignment is `x = x++`, which **discards the
>   post-increment result** — so the value never actually changes. Every
>   example this handbook used to give (`.bot tweak sightDistance 40.0` and
>   friends) read nothing, wrote nothing, and silently did nothing. To change
>   AI tuning, edit `aiplayerbot.conf` and use `.bot reload`.
> * **`.bot spoof` is console-only.** Its body is guarded on
>   `master == nullptr`, so in-game it takes the same path as the other
>   guarded handlers and does nothing. The old "spoofs chat/console actions"
>   wording is only true from the server console.
> * **`.bot rl` does not hand over leadership.** `PlayerbotMgr.cpp:2279-2307`
>   builds the same raid-chat packet `.bot r` builds and then returns
>   `"Sent raid leader transfer request as <name>"`. The message is
>   misleading. Use the real leadership command (`/w %t give leader`) instead.
>
> `.bot p` / `.bot g` / `.bot r` are described above as "broadcasts" because that
> is the observable effect, but none of them pushes a packet to a bot. Each
> queues a chat packet on the **master's own session**, which the client
> handler converts back into a chat command that `PlayerbotMgr::HandleCommand`
> re-dispatches. The full trace is in
> [Raid chat, and a wrong claim this handbook used to
> make](#raid-chat-and-a-wrong-claim-this-handbook-used-to-make).

> [!NOTE]
> **`.bot add` has three different behaviours depending on what the argument
> resolves to**, and the multi-account raid workflow depends on the third one:
>
> * **Character online** → group-invites it and resets its AI.
> * **Character offline** → the argument must be a **decimal GUID**, or the
>   command errors. A bare character *name* will not work here.
> * **Argument resolves to an account name** → **every character on that
>   account** is added. This is what makes `.bot add sam_tanks` summon a whole
>   ten-character roster in one line, which is the basis of the multi-account
>   raid workflow in [§10 Raid Assembly](#10-raid-assembly-attunement--instance-reset-blueprint).

### Targeted Bot Commands & Gearing Operations

These commands target individual bots, or `*` for all bots in your party (`PlayerbotHolder.cpp`):

| Command | Aliases | Syntax & Arguments | Description | Example |
| :--- | :--- | :--- | :--- | :--- |
| `.bot add` | `login` | `.bot add <name\|*>` | Online character → group-invites and resets. Offline → the argument **must be a decimal GUID** or it errors. A token that resolves to an **account name** adds every character on that account. | `.bot add *` |
| `.bot remove` | `logout`, `rm` | `.bot remove <name\|*>` | Safely saves and despawns specified bot or all active party bots. | `.bot remove *` |
| `.bot delete` | - | `.bot delete <name>` | Permanently purges bot character from `tw_char` database (`Player::DeleteFromDB`). | `.bot delete Stuckbot` |
| `.bot summon` | `recall`, `come` | `.bot summon <name\|*>` | Teleports bots to you. It **does not sync the instance map**, and refuses across instances. Also refuses in combat / BG / queue. | `.bot summon *` |
| `.bot level` | `levelup` | `.bot level <name\|*> <1–60>` | Sets character level of specified bot or all party bots. | `.bot level * 60` |
| `.bot gear` | `equip` | `.bot gear <name\|*> [quality]` | Generates and equips level-scaled gear. Accepted qualities: `green`, `uncommon`, `blue`, `rare`, `purple`, `epic`, `upgrade`, `sync`, `best`, `partial`. | `.bot gear * best` |
| `.bot enchants` | - | `.bot enchants <name\|*>` | Applies optimal permanent enchantments to all equipped armor and weapons. | `.bot enchants *` |
| `.bot train` | `learn` | `.bot train <name\|*>` | A single call to `learnClassLevelSpells()`. It does **not** visit a trainer and does **not** teach weapon proficiencies. | `.bot train *` |
| `.bot prep` | `prepare` | `.bot prep <name\|*>` | Calls `factory.Refresh()` only — it does **not** repair gear, enchant, clear cooldowns, or restore HP/mana. `init` is a *different* command, not an alias. | `.bot prep *` |
| `.bot consumes` | `consumables`, `consums` | `.bot consumes <name\|*>` | Stocks level-appropriate flasks, elixirs, scrolls, bandages, and consumable buffs. | `.bot consumes *` |
| `.bot potions` | `pots` | `.bot potions <name\|*>` | Stocks level-appropriate healing, mana, and utility potions in bot bags. | `.bot potions *` |
| `.bot regs` | `reg`, `reagents` | `.bot regs <name\|*>` | Stocks class reagents (Soul Shards, Ankhs, Feathers, Flash Powder, Holy Candles). | `.bot regs *` |
| `.bot food` | `drink` | `.bot food <name\|*>` | Stocks level-appropriate food and water drinks in inventory. | `.bot food *` |
| `.bot ammo` | - | `.bot ammo <name\|*>` | Generates level-appropriate arrows/bullets and equips best quiver/pouch. | `.bot ammo *` |
| `.bot pet` | - | `.bot pet <name\|*>` | Summons, feeds, and trains combat pet for Hunter or Warlock. | `.bot pet *` |
| `.bot random` | - | `.bot random <name\|*>` | Randomizes bot talent tree, appearance, and non-combat characteristics. | `.bot random *` |
| `.bot always` | - | `.bot always <name\|*>` *(name mandatory)* | Toggles persistence on a **raw GUID** (`std::stoull`). Blocked entirely when `selfBotLevel == DISABLED`. | `.bot always Tankman` |
| `.bot debug` | - | `.bot debug <cdebug-sub>` *(arg required)* | Requires a `cdebug` sub-command. It is **not** a bare status dump. | `.bot debug co ?` |
| `.bot c` | - | `.bot c <name\|*> <cmd>` | **Inert.** Forces `"monstertalk " + param` into `cdebug`; `DebugAction` only prints a usage line, and the handler always returns `"ok"` — it reports success and does nothing. | `.bot c * c ?` |
| `.bot w` | - | `.bot w <botname> <msg>` | Sends a whisper message directly from console/chat to the specified bot. | `.bot w Dunpriest follow` |
| `.bot cmd` | - | `.bot cmd <name\|*> <action>` | **Async dispatch**: fires a trigger and the action runs on a later AI tick, so the command returns before the action executes. | `.bot cmd Tankman attack` |
| `.bot do` | - | `.bot do <name\|*> <action>` | Runs a C++ action from `ChatActionContext` **immediately**. **Multi-word actions are broken** — only single words resolve. | `.bot do Tankman attack` |
| `.bot test` | - | `.bot test <strategy>` | Runs `ChangeStrategy("+test::<name>")` on the target. | `.bot test dundee` |
| `.bot record` | - | `.bot record <botname> [param]` | **Chat-message tap**, not a macro recorder. Captures the bot's outgoing text to you, and optionally its incoming chat. | `.bot record Tankman enable` |
| `.bot read` | - | `.bot read <botname>` | **Destructively drains** the record buffer: returns the buffered lines and disarms recording. | `.bot read Tankman` |
| `.bot clear` | - | `.bot clear <botname>` | Clears the chat buffer and forces both record flags off. | `.bot clear Tankman` |
| `.bot init` | - | `.bot init <name\|*> [quality]` | **Not an alias of `prep`.** A separate handler that randomises gear to a quality. | `.bot init * rare` |

> [!WARNING]
> **Corrections to this table.** Every row below was wrong before this pass.
>
> * **`.bot summon` / `recall` / `come` do not sync the map.** The handler calls
>   `bot->TeleportTo(master->GetMapId(), x, y, z, orient)` and **refuses** with
>   `Bot is in an instance — can't pull cross-instance.` It also refuses in
>   combat, in a Battleground, and while queued. To move a whole raid between
>   instances, use a map change on the master first.
> * **`.bot gear` rejects `poor` and `common`.** The accepted list is exactly
>   `green`, `uncommon`, `blue`, `rare`, `purple`, `epic`, `upgrade`, `sync`,
>   `best`, `partial`; anything else returns `unknown gear command`.
> * **`.bot train` / `learn` do not visit a trainer.** It is one call to
>   `bot->learnClassLevelSpells()`. Weapon proficiencies are untouched; for
>   those, whisper the bot `trainer learn` at a trainer.
> * **`.bot prep` is much weaker than advertised, and `init` is not one of its
>   aliases.** `prep` calls only `factory.Refresh()`. It does not repair gear,
>   enchant, clear cooldowns, or restore HP/mana. `init` is a *different*
>   handler (`HandleBotInit`) that randomises gear to a quality. Use
>   `.bot enchants`, `.bot gear`, `.bot consumes` explicitly for the rest of
>   what `prep` used to be described as doing.
> * **`.bot always` requires a name** and works on a raw GUID, so a bot
>   *name* will not parse. It is disabled outright when
>   `selfBotLevel == DISABLED`.
> * **`.bot debug` requires an argument** — a `cdebug` sub-command. A bare
>   `.bot debug` is not a status dump.
> * **`.bot c` is inert** and always answers `"ok"`. This was previously
>   documented as "executes internal bot chat command directly from console";
>   the full trace is in [Raid chat, and a wrong claim this handbook used to
>   make](#raid-chat-and-a-wrong-claim-this-handbook-used-to-make).
> * **`.bot cmd` is asynchronous** — it fires a trigger and returns; the action
>   runs on a later AI tick. If you read a result immediately, you will read
>   the *previous* state.
> * **`.bot do` cannot take multi-word actions.** The loop calls
>   `GetAction(param)` with an **invariant** `param` on every iteration, so
>   the trimming computed for each word is discarded and `tank attack` can
>   never resolve — it returns `action not found`. Use `.bot do <bot> attack`
>   (single word), or whisper `tank attack`, which takes the multi-word path
>   correctly.

> [!NOTE]
> **`.bot record` / `read` / `clear` are a chat tap, not a macro recorder.** The
> three rows here were substantially wrong, and the block is worth reading
> before you use it.
>
> * It captures the bot's **outgoing text to you** (`PlayerbotAI.cpp:3952`) and,
>   optionally, its incoming chat. It does not record your macros, your
>   actions, or a replayable sequence.
> * Syntax is `.bot record <botname> [param]`. The **bot name is mandatory** —
>   without it the handler returns `record requires a bot`.
> * `disable` **does not disable**. The call is
>   `RecordMessages(true, !param.empty())`: the first argument is hardcoded
>   `true`, so the trailing word only toggles *incoming* capture, and the
>   handler still replies `Recording enabled on …`.
> * `.bot read` **destructively drains** the buffer. `GetRecordedMessages()`
>   clears the store and un-arms recording; it returns the buffered lines, or
>   `(no messages)`.
> * `.bot clear` clears the buffer and forces both flags off, replying
>   `Messages cleared`.
> * Real workflow: `.bot record <bot> enable` → `.bot cmd <bot> <chatcmd>` →
>   wait a tick or two → `.bot read <bot>`.
> * `.bot do` and `.bot debug` arm and drain the **same** buffer internally, so
>   running either one clobbers a pending `record`.
>
> A new row, `.bot test`, was added: it exists (`PlayerbotMgr.cpp:404` →
> `HandleBotTest`) and runs `ChangeStrategy("+test::<name>")`.


---

## 3. RandomBot Population & Realm AI Manager (`.rndbot`)

The `.rndbot` suite controls autonomous open-world population, server lag tuning, and realm simulation (`RandomPlayerbotMgr.cpp`).

> [!NOTE]
> **Universal Command Inheritance — with three exceptions.** Most `.bot` verbs
> (`prep`, `gear`, `summon`, `group`, `train`, …) also work under `.rndbot`,
> and `.rndbot` adds server-wide population handlers on top. But `.rndbot`
> registers its **own** handlers for `init`, `refresh` and `remove`
> (`RandomPlayerbotMgr.cpp:4034-4043`), and those **shadow** the `.bot` verb of
> the same name:
>
> | Verb | What `.rndbot` actually runs | Not what you get |
> | :--- | :--- | :--- |
> | `.rndbot init` | `RandomizeFirst` | `PlayerbotFactory::Randomize` |
> | `.rndbot refresh` | `RandomPlayerbotMgr::Refresh` | `.bot refresh` (`HandleBotRefresh`) |
> | `.rndbot remove` | Randombot removal | the `.bot remove` master-link guard |
>
> This note previously claimed *every* `.bot` command was inherited unchanged,
> which is wrong for exactly these three.


### Realm Telemetry & Population Health

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.rndbot stats` | 0 | `.rndbot stats` | Formats realm population telemetry: online count, level brackets, race/class breakdown, active roles (Tank/Heal/DPS), questing, and combat status. **In-game only** — see the note below. | `.rndbot stats` |
| `.rndbot diff` | 1 | `.rndbot diff [p_diff e_diff]` | Displays tick latency metrics: `Avg diff`, `Max diff`, Character DB ping, sessions online, and active bots. Pass arguments to tune target diffs. | `.rndbot diff 100 200` |
| `.rndbot pid` | 3 | `.rndbot pid` * *(see note — the map key has a trailing space)* `[p i d]` | Displays or tunes the real-time Proportional-Integral-Derivative (PID) dynamic controller that balances bot activity based on server tick load. | `.rndbot pid  0.05 0.001 0.05` |
| `.rndbot update` | 3 | `.rndbot update` | Forces an immediate AI update tick across the entire realm (`UpdateAIInternal(0)`), evaluating zone population, quests, and login cycles. | `.rndbot update` |
| `.rndbot login debug` | 3 | `.rndbot login debug` | Toggles real-time verbose console logging for the bot login queue manager (`sPlayerBotLoginMgr`). | `.rndbot login debug` |
| `.rndbot clean map` | 3 | `.rndbot clean map` | Spawns detached threads to unload unreferenced map geometry and VMaps (`unloadMapAndVMaps`) across all maps, freeing memory. | `.rndbot clean map` |
| `.rndbot reset` | 3 | `.rndbot reset` | Wipes persistent random bot records from database (`DELETE FROM ai_playerbot_random_bots WHERE event != 'temporary'`). | `.rndbot reset` |

> [!WARNING]
> **Two `.rndbot` traps in the table above.**
>
> * **`.rndbot pid` needs the literal map key `"pid "` — with a trailing
>   space** (`RandomPlayerbotMgr.cpp:4005`). A bare `.rndbot pid` therefore does
>   **not** match, and the example above shows the extra space deliberately.
> * **`.rndbot stats` is effectively in-game only.** The handler overwrites its
>   own parameter with the session player's GUID before doing anything
>   (`:4019-4020`). Run from the console or RA with no session player and it
>   fails with `Stats: Error parsing ` and an empty value. (The old wording,
>   "spawns a background thread", was also wrong — the format runs inline.)

### World Bot Dispatch & RPG Simulation

| Command | Scope Selector | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.rndbot init` | `name\|%\|guild` | `.rndbot init [name\|%]` | Re-initializes random bots, rerolling specs, gear brackets, and faction quotas. This is `RandomizeFirst`, **not** the `.bot init` gear randomiser. | `.rndbot init %` |
| `.rndbot upgrade` | `name\|%\|guild` | `.rndbot upgrade [name\|%]` | Scans bots and upgrades equipment and spells matching current level brackets. | `.rndbot upgrade %` |
| `.rndbot refresh` | `name\|%\|guild` | `.rndbot refresh [name\|*]` | Soft-reboots bots, clearing combat deadlocks, pathing loops, and broken AI states. Runs `RandomPlayerbotMgr::Refresh` — a **different function** from the `.bot refresh` help text, which is itself broken (see the note below). | `.rndbot refresh *` |
| `.rndbot change_strategy` | `name\|%\|guild` | `.rndbot change_strategy [name\|%]` | Applies a strategy change across randombots (`RandomPlayerbotMgr.cpp:4042`). | `.rndbot change_strategy %` |
| `.rndbot grind` | `name\|%` | `.rndbot grind [name\|%]` | Orders idle bots to travel to level-appropriate zones and farm mobs. | `.rndbot grind %` |
| `.rndbot rpg` | `name\|%` | `.rndbot rpg [name\|%]` | Dispatches bots to taverns, cities, crafting trainers, and quest hubs for RPG social life. | `.rndbot rpg %` |
| `.rndbot revive` | `name\|%` | `.rndbot revive [name\|*]` | Instantly resurrects dead/ghost bots at full health and mana. | `.rndbot revive *` |
| `.rndbot teleport` | `name\|%` | `.rndbot teleport [name\|%]` | Teleports bot(s) to a randomized level-appropriate zone. | `.rndbot teleport %` |

> [!NOTE]
> **`.bot refresh` does not work, but `.rndbot refresh` does.** This is worth
> stating separately because the engine's own help advertises the broken one:
> `refresh` is offered in the `.bot help` output at `PlayerbotMgr.cpp:3421` and
> implemented at `:3355`, but it is **absent from the `.bot` handler map**, so
> `.bot refresh` returns `unknown command`. `.rndbot refresh` is a different
> function (`RandomPlayerbotMgr::Refresh`) and it works. `.rndbot
> change_strategy` had no row at all before this pass.

---

## 4. In-Game Whispers & Single-Letter Shortcuts Dictionary

Direct chat communication with party bots via `/w %t <action>` or `/p @all <action>` (`ChatTriggerContext.h` & `ChatActionContext.h`).

### Fast Single-Letter Shortcuts

> [!IMPORTANT]
> **Ground Truth Correction**: In earlier documentation, shortcut `c` was mislabeled as a combat toggle. In actual C++ source (`ChatTriggerContext.h:36-39`), `c` executes `TellItemCountAction` (inventory inspection). Combat engagement is handled via `attack` or `tank attack`.

| Shortcut | Action Name | Full Trigger Aliases | Description |
| :---: | :--- | :--- | :--- |
| **`c`** | Inventory Inspection | `c`, `items`, `inventory`, `inv` | Reports equipped items, free bag slots, gold count, and durability percentage. |
| **`u`** | Use Object / Item | *(none — see warning)* | ⚠️ **Both `u` and `use` are silently ignored.** Commands bot to right-click interact with nearest GameObject (lever, door, chest, node) or use designated item. |
| **`e`** | Equip Item | `e`, `equip` | Forces bot to evaluate inventory bags and equip the highest stat upgrades available. |
| **`ue`** | Unequip Item | `ue` | Orders bot to unequip item from equipment slot into inventory bags. |
| **`keep`** | Keep Item | `keep` | Marks item in bot bags to never be auto-vendored or destroyed. |
| **`drop`** | Drop Quest / Item | `drop` | Drops specified quest from quest log or destroys designated item. |
| **`s`** | Sell Gray Junk | `s` *(not `sell`)* | Orders bot to vendor all gray-quality trash items at targeted vendor NPC. |
| **`b`** | Buy Vendor Item | `b` *(not `buy`)* | Orders bot to purchase designated item from vendor. |
| **`bb`** | Buyback Item | `bb` *(not `buy back`)* | Restores accidentally sold items from vendor buyback tab. |
| **`r`** | Repair Durability | `r` *(not `repair` — that is its own separate command)* | Orders bot to repair 100% equipment durability at targeted blacksmith/vendor. |
| **`t`** | Initiate Trade | `t` *(not `trade`)* | Opens secure trade transaction window between player and bot. |
| **`nt`** | Cancel Trade | `nt` | Cancels active trade transaction window. |
| **`q`** | Quest Log Audit | `q`, `quests` | Dumps current bot quest log, active objectives, and completion percentages. |
| **`ll`** | Loot Strategy | `ll` | Inspects or sets bot loot filtering strategy (`normal`, `gray`, `disenchant`, `all`). |
| **`ss`** | Skip Spells List | `ss` | Dumps list of blacklisted or skipped spells on bot. |

> [!WARNING]
> **The "aliases" column was wrong for five rows, and one row is dead.**
>
> * **`sell`, `buy`, `buy back` and `trade` are not aliases here.** They are
>   **action** names in `ChatActionContext.h:124-128`. The runtime resolves
>   **trigger** names: `ExternalEventHelper::HandleCommand` calls
>   `GetTrigger(name)`, and `ChatTriggerContext.h` never registers those words.
>   Whispering `/w %t sell` therefore finds no trigger and is dropped.
> * **`repair` is not an alias of `r` either** — it is its own separate
>   trigger, and it does dispatch on its own. It simply is not reachable by
>   typing `r`.
> * **`u` / `use` do not work at all.** Both produce a trigger named `use`,
>   and no strategy anywhere installs a `TriggerNode` for it, so the runtime
>   drops the name with no error. This row previously described a working
>   "use nearest object" command; it does not exist in this tree. See
>   [Traps: Commands That Fail
>   Silently](#traps-commands-that-fail-silently).

> [!NOTE]
> **How a whisper command becomes a bot action** — worth knowing, because it
> explains every "it silently did nothing" case above. A playerbot chat command
> is **live only if all three** hold:
>
> 1. a **trigger** of that name exists in `ChatTriggerContext.h`;
> 2. a **`TriggerNode`** of that name is installed by some live strategy
>    (`ChatCommandHandlerStrategy`'s `supported` list becomes nodes
>    automatically via `PassTroughStrategy::InitNonCombatTriggers`; other
>    strategies install their own);
> 3. that node points at a **real action** in one of the contexts
>    `AiObjectContext` installs.
>
> Miss any one and the command is dropped with **no error** — an unknown name
> returns `false` at `ExternalEventHelper.h:84` and nothing is logged. Absence
> from `ChatCommandHandlerStrategy` alone does **not** prove a command is dead:
> `castnc` has no entry there yet works, because `generic/RpgStrategy.cpp:210`
> installs the node.


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
| `cheat` | `CheatAction` | Manages bot engine cheats (`+`, `-`, `~`, `?`). | `/w %t cheat +health,+mana` |
| `skill` | `TellSkillAction` | Dumps list of all bot learned skills and current ratings. | `/w %t skill` |
| `craft` | `CraftAction` | Orders bot to craft linked recipe item from inventory materials. | `/w %t craft [Lesser Healing Potion]` |
| `trainer learn` | `TrainerAction` | Orders bot to purchase all available abilities from targeted trainer. | `/w %t trainer learn` |
| `give leader` | `GiveLeaderAction` | Transfers group leadership to the bot or master. | `/w %t give leader` |

---

### Bot Cheats Subsystem (`/w <bot> cheat`)

> **Source**: `CheatAction.cpp`, `PlayerbotAIConfig.h`
> **Security Requirement**: `SEC_GAMEMASTER` (Level 2+) for manual `/w` overrides.

Bots can be granted individual cheat flags using prefix operators:

- **`?`**: Query active and default configuration cheats (`/w %t cheat ?`).
- **`+<name>`**: Enable cheat flag (`/w %t cheat +health`).
- **`-<name>`**: Disable cheat flag (`/w %t cheat -health`).
- **`~<name>`**: Toggle cheat flag (`/w %t cheat ~health`).
- **Multi-Cheat Chaining**: Combine multiple cheats with commas: `/w %t cheat +health,+mana,+cooldown,+movespeed`

| Cheat Token | Engine Effect on Bot |
| :--- | :--- |
| **`health`** | Infinite health; bot cannot be killed. |
| **`mana`** | Infinite mana pool; bot never runs oom. |
| **`power`** | Infinite rage, energy, and mana resources. |
| **`cooldown`** | Bypasses all ability and spell cooldown timers. |
| **`movespeed`** | Grants accelerated run, walk, and swim speeds. |
| **`attackspeed`** | Applies massive melee, ranged, and spell haste buffs (Spells `15181`, `15182`, `15183`). |
| **`gold`** | Free gold for all training, repairs, and vendor purchases. |
| **`repair`** | Automatically keeps equipped armor at 100% durability without cost. |
| **`taxi`** | Unlocks all worldwide flight path taxi nodes immediately. |
| **`item`** | Bypasses consumable reagent requirements (shards, powder, reagents). |
| **`breath`** | Grants infinite underwater breathing. |
| **`glyph`** | Bypasses Turtle-WoW class glyph limitations. |
| **`quest`** | Auto-completes and accepts quest objectives. |

#### In-Game Macro: All Party Bots Ultra God Mode

```lua
/p @all cheat +health,+mana,+cooldown,+movespeed,+repair
```

---

### Bot Professions, Skills & Crafting

Bots can gather, learn tradeskills, and craft items on command:

- **Check Bot Skills**: `/w %t skill` — Whispers back all learned weapon, defense, and profession skill ratings.
- **Order Bot to Craft**: `/w %t craft [Item Link]` — If the bot knows the recipe and possesses the materials in its inventory, it will immediately cast the tradeskill recipe and trade the crafted item.
- **Learn from Class / Profession Trainer**: Target any trainer NPC and whisper `/w %t trainer learn` to order the bot to train all available spells.

---

## 5. Chat Filter Syntax & Targeting Token Engine (`@filter`)

When issuing orders in party chat (`/p`), raid chat (`/ra`), or guild chat (`/g`), use `@filter` prefixes to route orders with surgical precision (`ChatFilter.cpp`).

### Role, Combat Type & Specialization Filters

| Filter Prefix | Inverse Prefix | Target Audience | Example Usage |
| :--- | :--- | :--- | :--- |
| **`@tank`** | `@notank` | All active tank bots | `/p @tank attack` • `/p @notank flee` |
| **`@heal`** | `@noheal` | All active healer bots | `/p @heal drink` • `/p @heal save mana` |
| **`@dps`** | `@nodps` | All damage dealers (melee & ranged) | `/p @dps attack` • `/p @dps max dps` |
| **`@melee`** | — | All melee combatants (Rogue, Warrior, Paladin, Druid in cat/bear) | `/p @melee formation melee` |
| **`@ranged`** | — | All ranged combatants — Hunter, Mage, Warlock, Priest, and Druid **not** in cat/bear | `/p @ranged formation far` |

> **There is no `@caster` filter**, and no `@nomelee` / `@noranged` either. The
> complete set of role filters is exactly `@tank @notank @heal @noheal @dps
> @nodps @melee @ranged`. Anything else is silently ignored and the message goes
> to every bot, so a typo such as `/p @caster formation line` still runs — on
> the tanks as well. `PlayerbotAI::IsRanged` is what `@ranged`/`@melee` test:
> everything except Paladin, Warrior and Rogue is ranged, and a Druid counts as
> ranged unless it is in cat, bear or dire-bear form.

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

> The **Numeric ID column is reference only — you must use the word.**
> `RtiChatFilter` matches the eight literal strings above and nothing else, so
> `/p @8 attack` does **not** mean "attack the skull". It falls straight through
> to `LevelChatFilter`, which reads any leading `@<digits>` as a level, so `@8`
> silently selects every level-8 bot in the group and runs the order on all of
> them.

### Class, Race, Level & Gear Score Filters

- **Class Filters**: `@warrior`, `@paladin`, `@hunter`, `@rogue`, `@priest`,
  `@shaman`, `@mage`, `@warlock`, `@druid` — those nine and nothing else. There
  is **no `@nowarrior` / `@nopriest` form**; `ClassChatFilter` holds a plain
  name→class map with no negation entries.
  - Example: `/p @mage cast blizzard` • `/p @priest cast power word: shield` • `/p @druid cast innervate`
- **Race Filters**: `@human`, `@orc`, `@dwarf`, `@nightelf`, `@undead`, `@tauren`, `@gnome`, `@troll`, `@highelf`, `@goblin`.
- **Level Filters**: a **bare number** after `@` — `@60` selects bots of exactly
  level 60, `@10-20` selects a range. There is **no `@lvl` prefix and no `<`, `>`
  or `=` operator**; `atoi()` just reads whatever follows the `@`.
  - Example: `/p @60 follow` • `/p @58-60 prep`
  - ⚠️ This is the one filter that will happily swallow something you meant
    differently, because any leading `@<digits>` is read as a level.
- **Gear Score Filters**: `@tier<N>` (one tier) or `@tier<N>-<M>` (a range), where
  `<N>` is a **tier index**, not a number. There is **no `@gs` and no `@ilvl`**.
  - Example: `/p @tier1 gear best` • `/p @tier2-3 follow`
  - `GearChatFilter` maps `PlayerbotAI::GetEquipGearScore` onto ten hard-coded
    bands, and they are the TBC-era ones, so they are wrong for a Classic gear
    curve: tier 1 is `gs` 61–69, tier 2 is 71–76, tier 3 is 77–99, and tier 4
    jumps to 120–132. A well-geared vanilla player lands around tier 2–3.

### Group, Raid, Guild & State Filters

- **Group Scope**: `@group` (any bot in a group), `@group<N>` (subgroup 1–8), `@group<N>-<M>` (a range), `@nogroup`, `@raid`, `@noraid`, `@leader`, `@rleader`, `@gleader`.
  - Example: `/ra @group1 formation shield`
  - ⚠️ **The number must be attached to the word.** `@group1` works; `@group 1` does **not** — `GroupChatFilter` parses the argument as `message.substr(6, message.find(" ") - 6)`, so with a space it reads an empty string, `atoi("")` yields 0, and the filter becomes "subgroup 0 to 0", which never matches (subgroups are stored 1-based via `GetSubGroup() + 1`).
  - ⚠️ There is no `@subgroup=<N>`, no `@party` and no `@noleader`. An unrecognised leading `@token` is not safely ignored either — `LocationChatFilter` is last in the chain and treats any unclaimed `@...` message as a map/zone name, so typos tend to be dropped rather than passed through.
- **Combat State**: expressed through the *strategy-state* filters, not a bare state token — `@co=<strat>` / `@noco=<strat>` (has / has not that combat strategy), and the same shape for `@nonc=`, `@nc=`, `@dead=`, `@nodead=`, `@react=`, `@noreact=`.
  - Example: `/p @noco=flee co +aoe` • `/p @co=heal nc +food`
  - ⚠️ `@dead`, `@alive`, `@combat` and `@noncombat` are **not** filters in this tree. `StateChatFilter` implements only `@inside`, `@outside` and `@needrepair`.
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

> ⚠️ **A `stance` here is a POSITION, not a warrior class stance.** It is a
> `Stance*` value that computes a `WorldLocation` for the bot to stand at
> relative to its target. It has nothing to do with Defensive / Battle /
> Berserker, and **mod-playerbots exposes no command to set a warrior's class
> stance at all** — a warrior bot's own `protection` / `fury` class strategy
> picks the class stance itself as part of its kit. So `/w Bot stance defensive`
> is **not a valid command**: `StanceValue::Load` (`Stances.cpp`) matches only
> the four words below and returns `false` for anything else.

Stances dictate combat positioning relative to active combat targets:

| Stance Name | Combat Behavior | Best Suited For |
| :--- | :--- | :--- |
| **`near`** / **`default`** | Standard combat range facing target. | General melee combat. |
| **`tank`** | Positions in front of mob and turns boss away from group. | Main Tank and Off-Tank bots. |
| **`turnback`** / **`turn`** | Actively rotates mob 180 degrees away from raid party. | Bosses with frontal cleaves, breath attacks. |
| **`behind`** | Maneuvers continuously behind target's back hitbox. | Rogues, Cat Druids, Fury Warriors. |

Those four are the complete set. An unrecognised word is rejected by
`StanceValue::Load`, so a typo leaves the bot on its existing stance rather than
erroring loudly.

**Stance Commands**:

- `/w %t stance <name>` — Sets combat positioning stance on targeted bot.
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
| `.dc config` | - | Dumps all live `DungeonClear.*` configuration values, overrides, and heroic defaults. **No GM gate** — any player can run it. |

> [!WARNING]
> **`.dc` is case-sensitive, everywhere.** The dispatcher matches `dc` and every
> verb and sub-verb with exact `strcmp` (`DungeonClearCommand.cpp:287`). The
> *only* place anything is lowercased is the `.dc spectate` argument
> (`:164-165`). So `.dc pull ON` does not set a mode — `ON` fails to match, and
> the argument falls into the **bare `(current+1)%3` cycle branch**, silently
> changing pull mode instead of erroring. Type verbs and arguments in exactly
> the case shown: lower-case throughout.

> [!NOTE]
> **`.dc config` has no security gate.** `HandleConfig` (`:90`) checks only for
> a session player; it does not test security level. Any player can therefore
> dump the entire `DungeonClear.*` tunable table, including your server's
> override values. This section previously implied a GM restriction.

### Pull Modes: Off / On / Dynamic

Choose how the tank bot approaches trash packs. The engine accepts **exactly
three tokens, plus a bare cycle** (`DungeonClearChatActions.cpp:1206-1216`):

| Pull Mode | Accepted As | Tactical Behavior | Clear Speed | Risk Profile | Best Content |
| :--- | :--- | :---: | :---: | :---: | :--- |
| **Off** | `off` | Tank melees each pack in place, no line-of-sight pull. | Moderate | Low | Cleared content, cautious runs. |
| **On** | `on` | Always drags the pack out to a camp first and pulls from LoS. | Methodical | Minimal | Undergeared runs, custom Turtle instances, raids. |
| **Dynamic** *(default)* | `dynamic`, `dyn` | Decides **per pack** whether to Leeroy or camp-pull, using the `DungeonClear.PullDynamicMaxLeeroyMobs` ceiling. | Balanced | Moderate | **Recommended for most dungeons.** |
| *(bare)* | *(no argument)* | Cycles `off → on → dynamic → off` via `(current+1)%3`. | — | — | Quick toggling, or a typo — see the case warning above. |

**"Leeroy" is not a mode you can set.** It is a per-pack **outcome** the dynamic
governor may choose — charge straight in and engage without a LoS pull — for
small packs. `leeroy` as an argument does not match any mode and falls into the
bare cycle instead. Likewise `advanced` is the other half of the dynamic
decision, not a mode name.

**Dynamic Pull Tuning**: Governed by `DungeonClear.PullDynamicMaxLeeroyMobs`
(default: 3). If a pack's size exceeds that threshold, `dynamic` switches that
pack to camp-pull.

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
.dc spectate free                      -- Same as bare above — accepted, and undocumented until now
.dc spectate follow <botname>          -- Rides camera smoothly on specified bot
.dc spectate next                      -- Cycles camera to next party bot (alias: .dc spectate n)
.dc spectate prev                      -- Cycles camera to previous party bot (aliases: .dc spectate p, .dc spectate previous)
.dc spectate list                      -- Lists all watchable bots in instance (alias: .dc spectate who)
```

> [!NOTE]
> **Three additional accepted `.dc spectate` tokens.** `free` is accepted
> alongside the bare form and does the same thing (`DungeonClearCommand.cpp:212`),
> and `previous` is accepted alongside `prev` and `p` (`:177`). None of the
> three is case-folded by the spectate branch — the argument is lowercased at
> `:164-165` before matching, so `FREE` and `PREVIOUS` do work, unlike every
> other `.dc` verb.

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
.dc test watch stop                    -- Accepted alias for 'off'

-- Batched Campaigns (.dc test plan)
.dc test plan start <spec>             -- Launches automated N-run test campaign
.dc test plan status                   -- Displays batch campaign pass/fail rates and wipe statistics
.dc test plan stop [planId|all]        -- Halts running test plans
```

> [!WARNING]
> **`.dc test plan start` requires `total=N`.** It is **mandatory** — the
> handler bails out if it is absent (`DcTestPlan.cpp:99-100`). Nothing in this
> handbook said so until now, and the omission fails as a bare "no campaign
> started" with no prompt for the value. Also accepted on the same line:
> `concurrent=` (how many runs execute at once), `level=` (bot level for the
> generated party), and `seed=` (replay a specific random seed).
>
> ```lua
> .dc test plan start blackrock-depths total=10 concurrent=2 level=60 seed=12345
> ```

> [!NOTE]
> **The `.dc test` argument surface is wider than the examples above.** From
> `DcTestGearTiers.cpp:174-187, 202-224`:
>
> * **`ilvl=`** accepts `none`, `off`, `unlimited`, `0`, or an explicit item
>   level from **1 to 400**.
> * **`quality=`** accepts `1`–`5` as well as the five colour names.
> * A **numeric mapId** works as a token in place of a dungeon name.
> * `.dc test watch stop` is an accepted alias for `off` (`:535`).

---

## 8. Raid Automation Operator Facade (`.raid`)

The live Eluna controller is **`run/lua_scripts/raid_automation.lua`**, now at
**v1.3.0** (it was v1.2.0). It has grown from roughly 25 verbs into a
five-layer operator facade over `.bot` and `.dc`. Its whole point is that you
should not have to know the underlying commands: it picks the right transport
and spells out what it is doing.

> [!WARNING]
> **There are two copies of this file in the tree, and only one is live.**
>
> | Path | Version | Live? |
> | :--- | :--- | :---: |
> | `run/lua_scripts/raid_automation.lua` | **v1.3.0** | ✅ **yes** |
> | `lua_scripts/raid_automation.lua` | v1.1.0 (stale) | ❌ no |
>
> `run/mangosd.conf` sets `Eluna.ScriptPath="./lua_scripts"`, and that path
> resolves **relative to the `run/` working directory** — so the live file is
> the one under `run/`. The stale v1.1.0 copy is missing `tank3` / `tank4` /
> `tanks` / `marks` / `ccmark` / `resume` / `gather` / `avoid` / `roll`
> entirely. **Auditing or editing the wrong one produces a large set of false
> "command does not exist" findings.** This mistake was made during the audit
> that produced this section.

### Layer 1: BATTLE

Driving the autonomous clear engine. These wrap `.dc` verbs and sequence them
for you.

| Command | What It Emits | Notes |
| :--- | :--- | :--- |
| `.raid start [off\|on\|dynamic]` | `dc bosses` → `dc pull <m>` → `dc on` → `dc status` | The argument is optional; bare `start` uses the engine default. |
| `.raid stop` | `dc off` | |
| `.raid pause` | `dc pause pause` | **Explicit intent** — it never uses the bare `dc pause` toggle, so it cannot be flipped by accident. |
| `.raid resume` | `dc pause resume` | Same reason: the explicit resume verb, not the toggle. |
| `.raid go <boss>` | boss-targeting order | |
| `.raid skip` | skip current pack/boss | |
| `.raid pull <mode>` | `dc pull <mode>` | Only `off`, `on`, `dynamic` — see [Pull Modes](#pull-modes-off--on--dynamic). |
| `.raid status` | `dc status` | |
| `.raid bosses` | `dc bosses` | |
| `.raid config` | `dc config` | |
| `.raid spectate` | `dc spectate` | |

### Layer 2: ROSTER

Building, inspecting and diagnosing the raid roster.

| Command | What It Emits | Notes |
| :--- | :--- | :--- |
| `.raid group <size>` | `bot group size=N` | The documented first step of assembly, now wrapped. |
| `.raid roles` | role-split formation orders | Turns the **DPS focus lock on**. |
| `.raid ready [best\|epic]` | gear + enchant + consumable + attunement + master keys | Bare form is `best`; `epic` gives baseline epics. |
| `.raid gear [quality]` | gear equip order | |
| `.raid attune` | attunement completion | |
| `.raid tanks <n> [names]` | promotes tank-capable bots | The one command that genuinely re-specs, and it only ever *promotes*. |
| `.raid tank2` / `tank3` / `tank4` / `tank5` `[<name>]` | assigns a specific tank slot | Bare form picks automatically; the optional name targets one bot. |
| `.raid marks <tank…>` | RTI marks for each tank | |
| `.raid marks <tank> <icon>` | one icon on one tank | |
| `.raid unmark [tank]` | clears marks | |
| `.raid clearmarks` | clears all marks | |
| `.raid ccmark` | CC assignment mark | |
| `.raid skull` | skull mark | |
| `.raid check [<name>]` | `bot r all ?` | **The most useful diagnostic in the whole facade** — see below. |
| `.raid rc` | `bot r ready` | Per-bot HP/MP, distance, food/water/potion stock. |
| `.raid gearscore` | `bot r who` | This one **does** print gear score. |
| `.raid stats` | gold, bags, repair cost, XP, power | **Does NOT print gear score.** |
| `.raid spells [<name>]` | the 50-spell spec dump | The reliable way to confirm what a bot actually specced. |
| `.raid who` | roster listing | |
| `.raid where [<name>]` | bot position | |
| `.raid list` | active bot list | |
| `.raid add <name>` | adds a bot | |
| `.raid remove <name>` | removes a bot | |
| `.raid avoidmob <id>` | `bot r avoid creature <id>` | "The raid should not path through these." `-<id>` removes one; bare lists the current set. |

> [!NOTE]
> **`stats` and `gearscore` are different commands — do not swap them.**
>
> * **`.raid check`** emits `bot r all ?`, which calls
>   `ai->PrintStrategies(requester, BOT_STATE_ALL)`
>   (`ChangeStrategyAction.cpp:150`) — so **every** bot reports its live
>   installed strategy set. This is the command to reach for when a bot is not
>   doing what you expect: it tells you what the bot actually installed, not
>   what you think you told it to. Sibling forms exist per state: `co ?`,
>   `nc ?`, `de ?`, `react ?`.
> * **`.raid rc`** emits `bot r ready` — HP/MP, distance, consumable stock.
> * **`.raid stats`** reports gold, bags, repair cost, XP and power. It does
>   **not** print gear score.
> * **`.raid gearscore`** is the one that prints gear score, via
>   `WhoAction.cpp:127`.

### Layer 3: TOGGLES

Every toggle now has an explicit off form, so you never have to guess a
double-toggle.

| On | Off | |
| :--- | :--- | :--- |
| `aoe` | `noaoe` | `single` remains an alias of `noaoe`. |
| `focus` | `unfocus` | |
| `mark` | `unmark` | |
| `loot` | `noloot` | |
| `rest` | `norest` | |
| `avoid` | `noavoid` | |
| `roll` | `noroll` | |
| `gather` | `nogather` | |

Plus the non-toggling verbs: `open`, `ccmark`, `skull`, `clearmarks`.

> [!NOTE]
> **`unfocus` is the new one.** `.raid roles` turns the DPS focus lock **on**,
> and before v1.3.0 there was no way to turn it back off through the facade.
> If the raid keeps focusing one target after you meant to release it, this
> was the missing piece.

### Layer 4: MOVE

| Command | Behaviour |
| :--- | :--- |
| `.raid summon` | Teleport bots to you. Alias: `.raid rally`. |
| `.raid follow` | **Soft regroup** — bots *walk* to you. New in v1.3.0, and distinct from `summon`. There is no `.raid regroup`; `follow` is the soft form. |
| `.raid tele <alias>` | Teleport to a named destination. |
| `.raid unbind` | Clears instance lockouts. Alias: `.raid resetlock` — there is no `.raid reset`. |

### Layer 5: PASS

Passthrough for anything the layers above do not cover.

| Form | Emits | Notes |
| :--- | :--- | :--- |
| `.raid all [@filter] <cmd>` | `bot r …` | Broadcast. |
| `.raid one <name> <cmd>` | `bot w <name> …` | Single named bot. |
| `.raid dc <sub>` | `dc <sub>` | `dc test` is **refused** — that is the GM harness. |
| `.raid bot <verb>` | allowlisted `.bot` passthrough | See the allowlist below. |

**The allowlist**: `group add login remove logout rm list gear equip train
learn enchants ammo food drink potions pots consumes consumables consums regs
reg reagents prepare prep summon recall come pet w p g r rl debug read clear
record`.

**18 of the 55 registered verbs are refused**, and the refusal message states
the reason *and* the source line. Refused, with reason:

| Refused | Why |
| :--- | :--- |
| `delete` | Deletes the character. |
| `init` | Randomises gear. |
| `do` | Multi-word actions are broken (see [`.bot do`](#2-playerbot-core-control-suite-bot)). |
| `c` | Inert. |
| `tweak` | No-op. |
| `cmd` | Duplicate of `.raid one`. |
| `reload` | GM-only. |
| `refresh` | ⚠️ **Advertised in the engine's own help at `PlayerbotMgr.cpp:3421` and implemented at `:3355`, but absent from the handler map — so it returns `unknown command`.** |
| `self`, `spoof`, `create`, `runtest` | Restricted or inert verbs. |
| `levelup` / `level` | Use `.raid gear` / the `.bot` command directly. |
| `always`, `random`, `test` | Persistence / randomisation verbs. |

### Filters: the `@token` Argument

An optional leading `@token` works on **any** `.raid` command:

```lua
.raid all @frost co +aoe
```

Verified filter families:

| Family | Tokens |
| :--- | :--- |
| **Role** | `@tank` `@notank` `@heal` `@noheal` `@dps` `@nodps` `@melee` `@ranged` |
| **Class** | `@warrior` `@paladin` … |
| **Race** | `@human` `@orc` … |
| **Level** | `@60`, `@10-20` |
| **Gear tier** | `@tier1` … `@tier10` — see the caveat below |
| **State** | `@needrepair` `@bagfull` `@bagalmostfull` `@inside` `@outside` |
| **Strategy** | `@co=` `@noco=` `@nc=` `@nonc=` `@dead=` `@nodead=` `@react=` `@noreact=` |
| **TalentSpec** | `@frost` `@holy` … — target by **actual spec**, not by role |

The filter chain lives in `CompositeChatFilter`
(`modules/mod-playerbots/src/playerbot/ChatFilter.cpp`,
~`:1224-1240`).

> [!WARNING]
> **`@tier1`–`@tier10` are TBC-era bands, and they are broken for Classic gear.**
> The bands are hard-coded score comparisons in `GearChatFilter::Filter`
> (`ChatFilter.cpp:312-331`), not a computed tier:
>
> | Band | Gear score | | Band | Gear score |
> | :--- | :--- | :--- | :--- | :---: |
> | tier 1 | 61–69 | | tier 6 | 146–154 |
> | tier 2 | 71–76 | | tier 7 | 200–213 |
> | tier 3 | 77–99 | | tier 8 | 225–**232** |
> | tier 4 | 120–132 | | tier 9 | **232**–245 |
> | tier 5 | 133–145 | | tier 10 | 251–277 |
>
> Three defects follow from that table, and all three fail **silently**:
>
> 1. **`gs` 100–119 matches no band at all.** `botTier` stays `0`, and
>    `0 >= fromLevel` is false for every `@tier<N>`, so those bots are excluded
>    from *every* gear filter. Same for `gs <= 60`.
> 2. **Tiers 8 and 9 overlap at exactly `gs` 232** — `>=225 && <=232` and
>    `>=232 && <=245` both claim it.
> 3. **Only `@tier1` and `@tier2-3` are registered as filter *examples***
>    (`:280-281`), so help output advertises two of the ten. The parser accepts
>    any `@tier<N>` and any `@tier<N>-<M>` range (`:301-310`) — the other eight
>    work, they are just undiscoverable.
>
> A well-geared vanilla player lands around **tier 2–3** (`gs` 71–99), so treat
> these as approximate. A non-matching bot returns its message *unchanged*
> (`:336`) rather than dropping it, so the literal `@tier1` token stays attached
> and the order is silently discarded downstream — the filter "works" by
> producing an unresolvable command. See
> [Class, Race, Level & Gear Score Filters](#class-race-level--gear-score-filters)
> for the wider picture.

### Help: Bare `.raid` and `.raid help <layer>`

* **Bare `.raid`** prints a 5-line index of the layers.
* **`.raid help <layer>`** accepts `battle`, `roster`, `toggles`, `move`,
  `pass`, `filters`, `traps`, `chat`.
* **Every help line prints the underlying engine string it emits**, so the
  facade teaches you the raw commands rather than hiding them.
* An unrecognised `.raid <word>` now **suggests the 3–4 closest matches**
  instead of dumping 25 lines.

### Traps: Commands That Fail Silently

Each of these fails **silently** and looks like it worked. Read this before
trusting an unfamiliar order.

> [!WARNING]
> **`warning` is not a warning — it makes the entire raid run away.**
>
> `strategy/generic/ChatCommandHandlerStrategy.cpp:302` maps `warning` to
> `NextAction("runaway chat shortcut")` → `GoawayChatShortcutAction`
> (`ChatActionContext.h:159`). **`bot r warning` sends the whole raid
> fleeing.** The name is a genuine misnomer in the engine; it is not a caution
> or a status query.
>
> * **`attackers` is dead.** A `TriggerNode` exists (`:314`) and an
>   `AttackersValue` exists (`ValueContext.h:249`), but **no action named
>   `attackers` exists in any `*Context.h`**, so the node dispatches to
>   nothing.
> * **`use` / `u` are dead.** Both resolve to a trigger named `use` that has
>   **no `TriggerNode` anywhere**, so they are silently ignored. The
>   [shortcut table above](#fast-single-letter-shortcuts) previously listed
>   `u` / `use` as working; that row is now flagged.
> * **`inventory` maps to the trigger `c`** (item count), and **`gbank` maps to
>   `gb`** — neither does what the name suggests.
> * **`castnc` is alive despite being absent from `ChatCommandHandlerStrategy`.**
>   `strategy/generic/RpgStrategy.cpp:210` installs the node, so it works.

> [!NOTE]
> **The general rule this illustrates.** A playerbot chat command is live only
> if **all three** hold:
>
> 1. a **trigger** of that name exists in `ChatTriggerContext.h`;
> 2. a **`TriggerNode`** of that name is installed by a **live** strategy;
> 3. that node dispatches to an **action** that exists in one of the contexts
>    `AiObjectContext` installs (`ActionContext` + `ChatActionContext` +
>    `WorldPacketActionContext`).
>
> Miss (1) or (2) and the name is dropped with **no error** at all
> (`ExternalEventHelper.h:84`). `supported.push_back(...)` entries in
> `ChatCommandHandlerStrategy` become nodes automatically via
> `PassTroughStrategy::InitNonCombatTriggers`, and other strategies install
> their own — which is why **absence from `ChatCommandHandlerStrategy` does
> not prove a command is dead**, as `castnc` shows.

---

## 9. Class Combat Strategy Assignment Deck (`/w %t`)

Target a bot (`%t`) and click these macros to set their permanent combat strategies (`co`) and non-combat strategies (`nc`):

### Tank Specializations

- **Protection Warrior (Main Tank)**:

  ```lua
  /w %t co -dps,-fury,-arms,+tank,+protection,+threat,+potions,+avoid aoe
  /w %t nc +food
  /w %t formation near
  /w %t stance tank
  ```

- **Protection Paladin (AoE & Undead Tank)**:

  ```lua
  /w %t co -dps,-heal,-holy,-retribution,+tank,+protection,+consecration,+threat,+potions
  /w %t nc +food,+buff
  /w %t formation near
  /w %t stance tank
  ```

- **Feral Bear Druid (Armor & HP Tank)**:

  ```lua
  /w %t co -dps,-cat,-heal,+tank,+bear,+threat,+avoid aoe,+potions
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
  /w %t co -dps,-elemental,-enhancement,+heal,+totems,+mana spring totem,+healing wave,+cure
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Restoration Druid (HoT & Tank Support)**:

  ```lua
  /w %t co -dps,-cat,-bear,+heal,+cure,+rejuvenation,+save mana
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Holy Paladin (Single-Target Tank Healer)**:

  ```lua
  /w %t co -dps,-protection,-retribution,+heal,+holy,+blessing of wisdom,+blessing of salvation,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

### Melee DPS Specializations

- **Combat Rogue (Behind Flank DPS)**:

  ```lua
  /w %t co -tank,+dps,+behind,+kick,+stealth,+evasion,+potions
  /w %t nc +food,+stealth
  /w %t formation melee
  /w %t stance behind
  ```

- **Fury Warrior (Dual-Wield Melee DPS)**:

  ```lua
  /w %t co -tank,-protection,+dps,+fury,+behind,+pummel,+potions
  /w %t nc +food
  /w %t formation melee
  /w %t stance behind
  ```

- **Retribution Paladin (2-Handed Melee DPS)**:

  ```lua
  /w %t co -tank,-heal,+dps,+retribution,+consecration,+potions
  /w %t nc +food,+buff
  /w %t formation melee
  ```

- **Enhancement Shaman (Windfury Melee DPS)**:

  ```lua
  /w %t co -heal,+dps,+enhancement,+windfury totem,+totems,+cc
  /w %t nc +food,+buff
  /w %t formation melee
  ```

- **Feral Cat Druid (Behind Bleed DPS)**:

  ```lua
  /w %t co -tank,-heal,+dps,+cat,+behind,+stealth,+potions
  /w %t nc +food,+stealth
  /w %t formation melee
  /w %t stance behind
  ```

### Ranged & Caster DPS Specializations

- **Marksmanship Hunter (Ranged DPS & Pet)**:

  ```lua
  /w %t co -tank,+dps,+ranged,+pet,+potions
  /w %t nc +food
  /w %t formation far
  ```

- **Fire Mage (Maximum Burst DPS)**:

  ```lua
  /w %t co -tank,+dps,+fire,+aoe,+cc,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

- **Frost Mage (Control & AoE DPS)**:

  ```lua
  /w %t co -tank,+dps,+frost,+aoe,+cc,+potions
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
  /w %t co -tank,-heal,-bear,-cat,+dps,+moonkin form,+cure,+potions
  /w %t nc +food,+buff
  /w %t formation line
  ```

---

## 10. Raid Assembly, Attunement & Instance Reset Blueprint

Blueprints for assembling, tuning, and resetting raid instances with playerbots.

### 10-Man, 20-Man & 40-Man Raid Compositions

| Raid Bracket | Target Instance | Recommended Composition | Macro Assembly Sequence |
| :--- | :--- | :--- | :--- |
| **10-Man** | Upper Blackrock Spire (UBRS), Karazhan Crypts | 2 Tanks, 3 Healers, 5 DPS | `.bot group size=10` (or `.bot add <Account>`) + `/ra @group1 formation shield` |
| **20-Man** | Zul'Gurub (ZG), Ruins of Ahn'Qiraj (AQ20) | 3 Tanks, 5 Healers, 12 DPS | 4 balanced groups with class dispels (Priest/Paladin/Shaman/Mage) |
| **40-Man** | Molten Core, Blackwing Lair, AQ40, Naxxramas | 4–6 Tanks, 10–12 Healers, 22–26 DPS | 8 balanced subgroups, windfury totems, paladin blessing rotas |

> ⚠️ **These are generic recommendations, not what `.bot group size=N` builds.**
> The composition `.bot group size=N` actually targets comes from
> `LfgAction::AllowedClassRoleNr` (`InviteToGroupAction.cpp:89`):
>
> | size | tanks | healers | dps |
> | :--- | :--- | :--- | :--- |
> | 5 | 1 | 1 | 3 |
> | 10 | 2 | 3 | 5 |
> | 20 | **2** | 5 | **13** |
> | 25 | 3 | 7 | 15 |
> | 40 | **4** | **16** | **20** |
>
> The 40-man row also caps classes individually — priest healers 11, mage DPS 15,
> warrior DPS 8, and **paladin tanks 0**.
>
> #### `.bot group` assigns the spec too — do not re-spec after it
>
> This section previously claimed the opposite, and the error was expensive: it
> said `.bot group` uses the picked role only for the headcount, does **not**
> forward it to the created bot, and that the spec is then chosen by
> `AutoSelectTalents(..., BOT_ROLE_NONE)` across all premade paths — so a warrior
> picked for a tank slot would roll `fury` vs `protection` 50/50, and you needed
> `.raid roles` afterwards to force it correct.
>
> **That was wrong.** The role *is* forwarded. The chain, verified in source:
>
> 1. `HandleGroup` picks a role and a class that can fill it, then calls
>    `HandleCreate` (`PlayerbotMgr.cpp:2775-2815`).
> 2. `HandleCreate` calls
>    `ChangeTalentsAction::AutoSelectTalents(newBot, out, role)`
>    (`PlayerbotMgr.cpp:2684`) — **with the role**.
> 3. `AutoSelectTalents` filters the premade paths to those whose dominant tree
>    maps to that role (`getPremadePaths(cls, "", role)`), so a tank-role warrior
>    gets `protection`, not a coin-flip.
> 4. The bot joins and `AcceptInvitationAction` calls `ai->ResetStrategies()`
>    then `ai->Reset()`, rebuilding the kit from the real talent tab
>    (`AcceptInvitationAction.h:50-52`).
>
> Within a role the choice still **varies**, which is the part that matters:
> `PickPremadePath` rolls between the candidate paths, so a DPS warrior gets
> `arms` or `fury` and a mage gets one of `arcane`/`fire`/`frost`. That spread is
> a feature.
>
> #### ⚠️ Therefore: do not force specs after `.bot group`
>
> Forcing one spec per class per role (what the old `.raid roles` did) **discards
> a correct, role-matched, varied choice and replaces it with a bland one** — all
> eight DPS warriors onto `fury`, every mage onto `fire`, every warlock onto
> `destruction`. `.raid roles` no longer sends any talent command. The only
> command that re-specs is `.raid tanks <n>`, which is the one case that genuinely
> requires the protection tree, and it only ever *promotes*.
>
> This also means a role is a property of a bot's **installed spec strategy**, not
> of its class — a `fury` warrior and a `protection` warrior are both class 1.
> `PlayerbotAI::IsTank` / `IsRanged` read it off the strategy set. That is why
> `.bot group`'s role assignment survives, and why the `@tank` / `@heal` /
> `@melee` / `@ranged` chat filters can split a raid correctly without anyone
> knowing the specs.

---

### 🛡️ Personal Character Raids & Account Character Limits

#### 1. Why Single Accounts Are Capped at 10 Characters

- **Client Constraint**: The World of Warcraft 1.12.1 / Turtle 1.18.1 client GUI (`CharacterSelect.xml` and `CharacterSelect.lua`) possesses exactly **10 fixed character button frames** (`CharacterSelectCharacterButton1..10`). When the server transmits the character list (`SMSG_CHAR_ENUM`), sending more than 10 characters causes Lua `nil` index errors, crashing the client login screen or preventing character selection.
- **Server Constraint**: In `src/game/World.cpp`, `CharactersPerRealm` is clamped to a maximum of 10 (`CONFIG_UINT32_CHARACTERS_PER_REALM`).
- **"Account has max characters" Diagnostic**:
  - In commit `f776c7a`, `.bot group` creates bots directly on your master account in `tw_char.characters`.
  - Once your account reaches 10 characters (your main + 9 bots), `CreateBot` hits `maxCharsPerAccount = 9` and reports `"Account has max characters"`.

---

#### 2. How to Inspect & Delete Characters on an Account

##### 🔍 Inspecting Characters on an Account

- **In-Game GM Command**:

  ```text
  .account characters <AccountName>
  ```

  *(Lists all character names, GUIDs, and levels registered under that account).*

- **Client Login Screen**: Log into the account from your WoW client. All characters on that account are rendered visually on the character selection screen.

- **Active Party/World Bots**:

  ```text
  .bot list
  ```

  *(Displays all bots currently loaded and active in the world under your session).*

- **Target Account Inspection**: Target any bot or character and run `.pinfo` to see their assigned Account ID and account username.

##### 🗑️ Deleting Characters to Free Account Slots

- **In-Game Bot Delete (For Bots on Your Account)**:

  ```text
  .bot delete <BotName>
  ```

  *(Or target the bot and run `.bot delete`). If the bot was created on your account, this immediately and permanently purges the character from the `tw_char` database, instantly freeing the character slot for new bots or alts!*

- **Client Character Selection Screen**:
  Log into the account, click on the bot/character you wish to remove, click the red **Delete Character** button in the bottom right, and type **`DELETE`** to confirm.

- **Purge Entire Alt Account**:
  If you want to completely erase an entire alt account and all characters residing on it:

  ```text
  .account delete <AccountName>
  ```

---

#### 3. Field Your Own Characters via Multi-Account Alt Bots (`AiPlayerbot.AllowMultiAccountAltBots = 1`)

Playerbots natively solves the 10-character barrier through **Multi-Account Alt Bots**, allowing you to command a complete 40-man raid roster made entirely of your own custom characters without polluting your main account:

##### ❓ Do I Have to Create the Account First? Should I Make a Couple?

- **Yes, the account must be created first** before you can create characters on it or summon them with `.bot add`.
- **Yes, make a couple of accounts based on your target raid size**, because each account holds at most 10 characters:
  - **10-Man Raid (UBRS / Karazhan Crypts)**: Create **1 alt account** (holds up to 10 characters).
  - **20-Man Raid (ZG / AQ20)**: Create **2 alt accounts** (holds up to 20 characters total).
  - **40-Man Raid (MC / BWL / AQ40 / Naxxramas)**: Create **4 alt accounts** (holds up to 40 characters total).

##### 💡 Recommended Account Structure (Organized by Role)

Organizing accounts by role makes summoning your raid effortless:

| Account Name | Recommended Purpose | Maximum Roster | Example Summon Command |
| :--- | :--- | :---: | :--- |
| `sam_tanks` | Protection Warriors, Feral Druids, Paladins | Up to 10 | `.bot add sam_tanks` |
| `sam_healers` | Holy Priests, Resto Shamans/Druids, Holy Paladins | Up to 10 | `.bot add sam_healers` |
| `sam_melee` | Rogues, Fury Warriors, Enhancement Shamans | Up to 10 | `.bot add sam_melee` |
| `sam_ranged` | Mages, Warlocks, Hunters, Shadow Priests | Up to 10 | `.bot add sam_ranged` |

##### 🚀 Step-by-Step Setup Sequence

1. **Create Dedicated Alt Accounts**:
   From inside the game on your GM character (or via the server terminal):

   ```text
   .account create sam_tanks password
   .account create sam_healers password
   .account create sam_melee password
   .account create sam_ranged password
   ```

2. **Build Your Characters**:
   Log into each account with your WoW client. Create up to 10 characters per account with your desired names, races, classes, and talent specializations.

3. **Assemble Your Raid (1-Click Account Batches)**:
   Log into your main character. You can summon individual alts or **entire 10-character accounts** at once:

   ```text
   .bot add sam_tanks     -- Logs in and adds all characters from sam_tanks
   .bot add sam_healers   -- Logs in and adds all characters from sam_healers
   .bot add sam_melee     -- Logs in and adds all characters from sam_melee
   .bot add sam_ranged    -- Logs in and adds all characters from sam_ranged
   .bot add <CharName>    -- Adds an individual alt character by name
   ```

4. **Automatic Raid Conversion & Preparation**:
   - The engine automatically converts your party to a raid as soon as membership exceeds 5 (`group->ConvertToRaid()`).
   - Teleport the entire raid to your coordinates:

     ```text
     .bot summon *
     ```

   - Split formations by role, then gear and attune the entire raid:

     ```text
     .raid roles
     .raid ready
     ```

     *(`.raid roles` puts tanks on `shield`, healers on `far`, melee on `melee`
     and casters on `line`, then focus-locks the DPS. `.raid ready` equips gear,
     enchants, restocks consumables, completes attunements and adds master keys.
     Append `epic` to `.raid ready` for baseline epics instead of best-in-slot.)*


### Instance Lockouts, Unbinding & Raid Resets

When managing raid locks or practicing encounters:

| Operation | GM Command | Functionality |
| :--- | :--- | :--- |
| **Unbind Current Player** | `.instance unbind <map_id\|all>` | Clears instance ID lockout for your character. |
| **Unbind Entire Group** | `.instance groupunbind <map_id\|all>` | Clears instance ID lockouts for all bots and players in group. |
| **Instance Statistics** | `.instance stats` | Displays active instance saves, player references, and memory footprint. |

---

## 11. Autonomous Dungeon Clear (.dc) Architecture, Raid Mechanics & Blackwing Lair Master Blueprint

### 11.1 The Engine Lifecycle of `.dc on` in Raids

When a player issues `.dc on` (or `.raid start` via Eluna), the engine activates a multi-layered automation pipeline across `mod-dungeon-clear` and `mod-playerbots`:

```text
[ .dc on Command Issued ]
           │
           ▼
[ DungeonClearDispatch::DispatchToTankBots ]  ──► Identifies raid tanks (Shield spec / highest HP / armor)
           │
           ▼
[ DcLeaderSignal::IsDungeonClearLeader ]      ──► Elects ONE tank bot as Leader (Lowest GUID / Main Tank)
           │
           ├───────────────────────────────────────────────┐
           ▼                                               ▼
[ Leader Tank Bot ]                              [ Follower Bots (Raid Members) ]
- Activates +dungeon clear (non-combat)           - Activate +dungeon clear (non-combat)
- Activates +dungeon clear combat (combat)        - Activate +dungeon clear combat (combat)
- DcRun::Of(context).enabled = true              - follow strategy = dungeon clear follow tank
- Registers with DcStatusPublisher                - Read DungeonClearPartyTankValue to maintain formation
- Drives route navigation, pulling & events       - DcStayAtCampAction locks bots at safe camp during pulls
```

#### The Non-Combat Trigger Ladder (`DungeonClearStrategy.cpp`)
1. **`PartyDied` (Priority 100)**: Detects wipe, releases spirit, and initiates graveyard runback or Soulstone recovery.
2. **`AllCleared` (Priority 90)**: Instance complete check; halts pathfinding and triggers raid celebration.
3. **`StrandedRecovery` (Priority 42)**: Unstuck watchdog; teleports or leaps bot back onto the navmesh if geometry breaks.
4. **`Pull` (Priority 35)**: Coordinates with `DcPullPlanner` to calculate safe stand-off camps and line-of-sight corner pulls.
5. **`RezParty` (Priority 31.5)**: Halts tank movement while healers resurrect fallen raid members.
6. **`EventDue` (Priority 31)**: Hands execution to `DungeonEventExecutor` for declarative scripted steps (`MoveTo`, `Gossip`, `UseGameObject`, `ClearRadius`, etc.).
7. **`AtBoss` / `AtObjective` (Priority 30)**: Pre-boss staging gate; checks raid mana/health rest thresholds and consumable readiness.
8. **`RoomTrash` (Priority 26)**: Clears perimeter trash in boss rooms before engaging the boss (prevents accidental adds during boss movement).
9. **`BlockingTrash` (Priority 25)**: Engages packs directly obstructing the movement corridor.
10. **`Advance` (Priority 20)**: Advances along the recorded route or MMAP chunked pathfinder.

#### The Combat Trigger Ladder (`DungeonClearCombatStrategy.cpp`)
1. **`BreakStuckCombat` (Priority 65)**: Evades or drops combat if an enemy mob is wedged in unreachable geometry.
2. **`StayAtCamp` (Priority 60)**: Locks followers at the designated pull camp while the tank moves out to body-pull or ranged-pull.
3. **`PullManeuver` (Priority 60)**: Tank shoots/ranges target, establishes initial threat, and retreats around a corner to LoS casters.
4. **`Hazard` (Priority 55)**: Evaluates `DcHazard` to steer bots out of ground fire, void zones, shadowflames, and poison clouds.
5. **`HealReposition` (Priority 41)**: Healers reposition to regain Line of Sight on the tank if geometry cuts off heals.
6. **`AssistCampCombat` (Priority 35)**: Follower DPS step out to engage the pulled mobs once they reach the camp kill-zone.
7. **`ObjectiveEngageCombat` (Priority 34)**: Focuses high-threat priority targets (Sappers, Totems, Mind-Controlled allies, Healers).
8. **`RegroupCombat` (Priority 29)**: Pulls over-extended bots back into safe formation.

---

### 11.2 Comparative Audit of Existing Raids in `mod-dungeon-clear`

| Raid / Dungeon | Map ID | Key Automation Mechanics | Event File & Custom Facilities |
| :--- | :---: | :--- | :--- |
| **Molten Core** | `409` | • Core Hound pack sync (5 packs cleared within 10s before resurrection loop)<br>• Lucifron patrol timing window (> 65yd margin)<br>• 7 Rune douses (held until Aqual/Eternal Quintessence applied)<br>• Majordomo summon & add kill gate (he never dies, only adds die)<br>• Ragnaros gossip summon (option 1003) & spawn wait<br>• Shadowforge Descent navmesh gap teleport | `MoltenCoreEvents.cpp`<br>Uses `MoveToHoldUntilInstanceData`, `ClearRadius`, `Gossip`, `TeleportParty` |
| **Zul'Gurub** | `309` | • Room aggro pre-clears for Jeklik, Venoxis, and Mar'li<br>• Mandokir Vilebranch Speaker prerequisite kill<br>• Arlokk Gong of Bethekk summon objective | `ZulGurubEvents.cpp`<br>Uses `DcRoomAggroPreClearCondition`, `UseGameObject` |
| **Karazhan Crypts / Dragonmaw** | `800` / `816` | • Linear boss sequence driven by `dc_roster.txt`<br>• Ledge drops over geometry breaks (`DropInHole`)<br>• Multi-level pathing | `DragonmawEvents.cpp`<br>`KarazhanCryptEvents.cpp` |
| **Blackrock Depths** | `230` | • Shadowforge Key lock-680 traversal exemptions<br>• Giant Doors lever sequence (SmartAI door apparatus)<br>• Lyceum torch runner automation | `BlackrockDepthsEvents.cpp`<br>`DcEventDoorRegistry.h` |

---

### 11.3 Reusable Facilities in `Util/` for Raid Perfection

The `modules/mod-dungeon-clear/src/Ai/Dungeon/DungeonClear/Util/` subsystem provides enterprise-grade tools that dramatically elevate raid automation:

1. **`DcPullPlanner` (Corner Pulls & Standoff Camps)**:
   - Evaluates room geometry to find safe staging camps away from roaming patrols.
   - Calculates path splines for the tank to shoot a mob and retreat behind a wall/pillar.
   - Forces enemy spellcasters to run into melee range rather than nuking the raid from afar.
2. **`DcLeaderSignal` (Camp Holding & Synchronization)**:
   - Dispatches synchronization signals (`HoldFollowersAtCamp`, `ReleaseFollowers`).
   - Follower bots stand completely still at camp with `DcStayAtCampAction` while the MT pulls, eliminating accidental ninja-pulls.
3. **`DcHazard` (Dynamic Ground AoE Avoidance)**:
   - Maintains a real-time hazard grid for static traps (Suppression Devices) and dynamic effects (Void Zones, Flame Nova, Rain of Fire).
   - Bots calculate evasion vectors and reposition safely without dropping DPS rotations.
4. **`DcDoorPolicy` & `DcEventDoorRegistry` (Smart Gate Adjudication)**:
   - Automatically identifies script-only doors (`IsScriptOnly`) to prevent bots from clicking locked raid gates and desyncing server state.
   - Handles door lock exemptions (`IsKeyExempt`) and auto-pause timeouts.
5. **`DcSmartRest` (Pre-Pull Raid Readiness)**:
   - Gates boss pulls until all raid healers and casters have >= 85% mana and all members have >= 90% health.
   - Prevents pulling while raid members are drinking or recovering.
6. **`DcTargeting` (Raid Icon Focus & Crowd Control)**:
   - Integrates skull/cross target priorities, enforces kill orders, and forbids breaking crowd control (Polymorph, Banish, Shackle Undead).

---

### 11.4 Blackwing Lair (Map 469) Gap Analysis & Implementation Blueprint

#### 1. The Missing Boss Entry Bug
In `modules/mod-dungeon-clear/src/Ai/Dungeon/DungeonClear/Data/DcBossEntries1121.h`, **Broodlord Lashlayer (12017)** was completely missing from the compiled order!
Without Broodlord, the engine attempts to route directly from Vaelastrasz to Firemaw, ignoring the Suppression Room and deadlocking at the closed Lashlayer gate (`GO 179365`).

**Corrected Encounter Order (Map 469)**:
```text
1. Razorgore the Untamed  (12435)  ──► Door: 176965 opens on death
2. Vaelastrasz the Corrupt (13020)  ──► Door: 179364 opens on death
3. Broodlord Lashlayer    (12017)  ──► Door: 179365 opens on death [CRITICAL FIX]
4. Firemaw                (11983)  ──► Drake Wing Corridor
5. Ebonroc                (14601)  ──► Drake Wing Laboratory
6. Flamegor               (11981)  ──► Drake Wing Laboratory
7. Chromaggus             (14020)  ──► Side Lever: 179148 opens Gate: 179116
8. Nefarian               (11583)  ──► Throne Terrace Gate: 176966
```

#### 2. `BlackwingLairEvents.cpp` Declarative Blueprint
To integrate BWL into the event ladder, `BlackwingLairEvents.cpp` defines:
- **Event 1 (Razorgore Phase 1 & 2)**: Add kill priority (Mages 12420 -> Legionnaires 12416 -> Dragonspawn 12422); holds until Razorgore gains Warming Flames (`23040`).
- **Event 2 (Vaelastrasz RP Initiation)**: Tank approaches Vaelastrasz (13020) and triggers gossip dialogue (`GOSSIP_ITEM_VAEL_1` -> `GOSSIP_ITEM_VAEL_2`). Waits for Lord Victor Nefarius (10162) to finish corruption dialogue before combat engages.
- **Event 3 (Suppression Room Gauntlet)**: Coordinates moving camp progression up the ramps while Rogues stealth and disarm Suppression Devices (`GO 179784` via `SPELL_DISARM_TRAP = 1842`).
- **Event 4 (Broodlord Lashlayer Door Gate)**: Anchors Broodlord Lashlayer (12017) at order 3, verifying door 179365 opens upon death.
- **Event 5 (Chromaggus Side Lever)**: Directs the lead bot to operate `GO_LEVER_CHROMAGGUS_SIDE = 179148` to open the gate `GO_DOOR_CHROMAGGUS_SIDE = 179116`.
- **Event 6 (Nefarian Throne Dialogue & Phase 1 Drakonids)**: Gossips with Lord Victor Nefarius at his throne (`-7466.16, -1040.80, 412.05`), holds choke points at the south and north drakonid doors, and engages Phase 2 when Nefarian lands.

#### 3. Script-Only Door Protections (`DcEventDoorRegistry.h`)
Add the following gameobjects to `DcEventDoorRegistry::IsScriptOnly`:
- `176965`: Razorgore Exit Door
- `179364`: Vaelastrasz Exit Door
- `179365`: Broodlord Lashlayer Gate
- `179115`: Chromaggus Entrance Door
- `179116`: Chromaggus Side Gate (opened via lever 179148)
- `179117`: Chromaggus Exit Door
- `176966`: Nefarian Terrace Gate

---

### 11.5 Encounter-by-Encounter Combat Strategies & Bot Perfection

#### 🐲 1. Razorgore the Untamed (Entry 12435)
- **Phase 1 (Egg Destruction & Add Waves)**:
  - **Kill Priority**: Blackwing Mage (`12420`, channels heavy AoE) > Blackwing Guard (`14456`) > Blackwing Legionnaire (`12416`).
  - **Offtank Duty**: Offtanks pick up Death Talon Dragonspawn (`12422`, sleep-immune, dangerous cleave) and hold them away from the orb platform.
  - **Bot Orb Restriction**: Bots never touch the Orb of Domination (`GO 177808`) while the player controls Razorgore to destroy the 30 eggs.
- **Phase 2 (Razorgore Burn)**:
  - Triggered automatically when all 30 eggs are destroyed and Razorgore receives `Warming Flames` (`23040`).
  - **Conflagration Tank Swap**: Razorgore casts Conflagration (`23023`) which disorients the active tank and wipes threat. Offtanks must immediately execute `taunt razorgore` to prevent him from cleaving the raid.
  - **Fireball Volley LoS**: Casters and ranged hide behind the corner pillars during Fireball Volley (`22425`).
  - **Consumable**: Greater Fire Protection Potion (`SPELL 17543`).

#### 🔴 2. Vaelastrasz the Corrupt (Entry 13020)
- **Essence of the Red (`23513`)**:
  - Grants infinite resources (500 mana/s, 50 energy/s, 20 rage/s). Bots execute full unconstrained DPS rotations (`max dps`).
- **Burning Adrenaline Tank Swap (`23620`)**:
  - Cast on active tank every 45s and random mana users every 15s. Increases damage by 100% and makes spells instant, but kills the target in 20 seconds.
  - **Mana Users**: Run out at least 25 yards away from the raid (`move away from raid::25`) to avoid exploding on raid members (`SPELL 23478`).
  - **Main Tank**: When MT receives Burning Adrenaline, **Offtank #2 MUST immediately taunt Vaelastrasz** and turn him away from the raid. The dying MT runs to the designated sacrifice wall to explode safely.
- **Flank Stance**: All DPS and healers must stand strictly at Vaelastrasz's flanks (between front and back legs) to avoid frontal Flame Breath (`23461`) and rear Tail Sweep (`15847`).

#### ⚙️ 3. Suppression Room & Broodlord Lashlayer (Entry 12017)
- **Suppression Room Gauntlet**:
  - Suppression Devices (`GO 179784`) pulse an 80% movement slow aura (`22247`).
  - **Rogue Automation**: Rogue bots automatically enter stealth (`StealthForSuppressionDeviceAction`), move to device (`MoveToSuppressionDeviceAction`), and cast Disarm Trap (`DisarmSuppressionDeviceAction`, Spell 1842).
  - **Moving Camp**: The raid maintains a continuous moving camp using `DcPullPlanner` to clear small packs up the ramp, preventing infinite whelp respawns.
- **Broodlord Lashlayer Boss Tactics**:
  - **Knock Away Threat Reduction (`18670`)**: Knocks the active tank back and **cuts threat by 50%**. Offtanks must immediately taunt (`taunt broodlord`) or maintain #2 threat to take the boss.
  - **Wall-Backing**: Main tank must position their back flat against the corner wall so Knock Away does not punt them down into the Suppression Room.
  - **Mortal Strike (`24573`)**: Deals 4,000–5,000 physical damage and cuts healing by 50% for 5s. Healers pre-cast big heals on the tank.
  - **Blast Wave (`23331`)**: Heavy fire AoE + slow around boss. All ranged DPS and healers must stand > 20 yards away.

#### 🐉 4. Firemaw (Entry 11983)
- **Shadow Flame (`22539`)**:
  - Frontal cone dealing 4,000–5,000 shadow damage + 20,000 damage DoT.
  - **Mandatory Gear**: All tanks and bots facing Firemaw **MUST wear Onyxia Scale Cloak (`Item 15138`)** to negate the fatal Shadow Flame DoT.
- **Flame Buffet Reset (`23341`)**:
  - Stacking fire debuff applied to the entire room every 2 seconds.
  - Non-tank bots with **>= 3 stacks** automatically hide behind the doorway pillar (`hide from firemaw`) to let stacks drop to 0 before re-engaging.
- **Wing Buffet Tank Swap (`23339`)**:
  - Wipes active tank threat. Offtank steps into line of sight right before Wing Buffet, taunts Firemaw, and absorbs the threat wipe while MT resets.

#### 🖤 5. Ebonroc (Entry 14601)
- **Shadow of Ebonroc Tank Swap (`23340`) - CRITICAL FIX**:
  - Ebonroc places Shadow of Ebonroc on the main tank for 8 seconds.
  - **Healing Wipe Mechanic**: Every melee attack Ebonroc lands on a tank with this debuff **heals Ebonroc for 25,000 health**!
  - **Engine Fix**: `EbonrocTankSwitchTrigger` must detect `HasAura(23340, boss->GetVictim())` and order the offtank to **taunt immediately**!
  - The two tanks swap back and forth every time Shadow of Ebonroc is applied, completely starving Ebonroc of self-healing.
- **Shadow Flame**: Both tanks wear Onyxia Scale Cloaks. Inactive tank hides out of LoS to avoid accidental hits.

#### 🔥 6. Flamegor (Entry 11981)
- **Frenzy Dispels (`23342`)**:
  - Flamegor enrages periodically, increasing attack speed by 150% and casting fire novas.
  - **Hunter Tranquilizing Shot**: Hunter bots cast Tranquilizing Shot (`Spell 19801`) within 1.0 second of Frenzy.
- **Wing Buffet Swap (`23339`)**:
  - Handled via offtank taunt rotation identical to Firemaw.
- **Onyxia Scale Cloaks**: Maintained on both tanks.

#### 🧪 7. Chromaggus (Entry 14020)
- **The 5 Brood Afflictions (Preventing Chromatic Mutation)**:
  - If any player accumulates all 5 debuffs simultaneously, they undergo **Chromatic Mutation**, permanently mind-controlling them into a hostile drakonid.
  - **Black (`23154`, Curse)**: +100% Fire damage taken. Mages and Druids prioritize `Remove Lesser Curse` / `Remove Curse`.
  - **Blue (`23153`, Magic)**: -50% cast speed, burns mana. Priests and Paladins prioritize `Dispel Magic` / `Cleanse`.
  - **Red (`23155`, Disease)**: Deals fire damage, heals Chromaggus on death. Priests, Paladins, and Shamans prioritize `Cure Disease`.
  - **Green (`23169`, Poison)**: -50% healing taken. Druids, Paladins, and Shamans prioritize `Cure Poison` / `Purify`.
  - **Bronze (`23170`, Periodic Stun)**: Dispel-immune! Cured exclusively by using **Hourglass Sand (`Item 19183`)** or playerbot direct aura removal action (`clear chromaggus bronze`).
- **Breath LoS Rotation**:
  - Chromaggus uses 2 random breaths per raid lockout (Time Lapse, Ignite Flesh, Frost Burn, Corrosive Acid, Incinerate).
  - Raid and healers line-of-sight the breaths behind the ramp wall. For Time Lapse (`23310`, -50% threat and stun), the offtank taunts or absorbs Time Lapse to maintain threat stability.
- **20% Enrage Burn**:
  - At 20% health, Chromaggus enrages. Raid burns all offensive cooldowns, consumables, and execute abilities.

#### 👑 8. Nefarian (Entry 11583)
- **Phase 1 (Drakonid Gauntlet)**:
  - Lord Victor Nefarius (`10162`) watches from his throne while waves of drakonids spawn from the South and North doors.
  - Tanks hold each door choke point; Mages and Warlocks AoE burn drakonids.
  - After 42 drakonids are slain, Nefarian transforms into dragon form and lands.
- **Phase 2 (Landing & Class Calls)**:
  - **Initial Shadowflame (`22992`)**: Hits the entire room on landing. Every raid member **MUST wear Onyxia Scale Cloak (`Item 15138`)** or take fatal damage.
  - **Bellowing Roar (`22686`)**: 35-yard AoE fear. Warriors use Berserker Rage, Shamans drop Tremor Totem, Priests cast Fear Ward.
  - **Veil of Shadow (`22687`)**: -75% healing on tank. Mages and Druids instantly decurse the MT.
  - **Class Call Counters (Every 25–35 seconds)**:
    - **Priest Call (`23401`)**: Direct heals apply a stacking lethal DoT (Corrupted Healing) to the target. **Priest bots MUST stop direct heals** (`heal`, `flash heal`, `greater heal`) and only use `Power Word: Shield`, `Renew`, or wand!
    - **Mage Call (`23410`)**: Wild Polymorph hits allies. Mages stand away from the raid; other dispellers instantly decurse sheeped players.
    - **Warrior Call (`23397`)**: Forces Berserker Stance (+30% damage taken). Tanks pop Shield Wall / Last Stand; healers maximize healing output.
    - **Paladin Call (`23418`)**: Forces Blessing of Protection on Nefarian (immune to physical). Casters burst; physical DPS hold or dispel BoP.
    - **Hunter Call (`23436`)**: Weapons instantly break (durability 0). Hunters equip backup weapons or drop Field Repair Bot 75B.
    - **Rogue Call (`23414`)**: Rogues teleport to Nefarian's head and become rooted. MT immediately turns Nefarian to prevent Cleave/Shadowflame from wiping the rogues.
    - **Shaman Call (`23425`)**: Drops Corrupted Totems (Windfury, Fire Nova, Stoneskin, Healing Stream). Raid DPS immediately focus and destroy totems (`NPC 14662–14666`).
    - **Warlock Call (`23427`)**: Spawns Corrupted Infernals on raid members. Offtanks pick up Infernals; DPS AoE them down.
    - **Druid Call (`23398`)**: Forces Cat Form. Druids cannot heal or bear tank during the call.
- **Phase 3 (Bone Constructs at 20% Health)**:
  - Nefarian raises all dead Phase 1 drakonids as Bone Constructs (`NPC 14605`).
  - Mages cast Frost Nova and Blizzard; tanks grab AoE threat or kite with Stratholme Holy Water / Goblin Sappers while the raid burns Nefarian to 0%.

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md) • [🤖 RandomBot Guide (rndbot_commands.md)](../rndbot_commands.md)
