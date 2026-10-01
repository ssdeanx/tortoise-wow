# Volume 08: Open World Bot Control, Autonomous Strategies & Real-Time Feedback

> **Target Core**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + PlayerBots Framework + Eluna Engine  
> **Quick Navigation**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📖 Server Operations (commands.md)](../commands.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [🏰 Volume 07: Guilds & Guild Operations](./07_guilds_and_guild_operations.md)

---

## 📑 Table of Contents

1. [Open World Population & Autonomous Simulation Engine](#1-open-world-population--autonomous-simulation-engine)
2. [Realm Health, Performance & Lag Governor Tuning](#2-realm-health-performance--lag-governor-tuning)
3. [Bot Persistence & Dynamic World Spawning](#3-bot-persistence--dynamic-world-spawning)
4. [Master Strategy Engine Catalog (`co` / `nc` / `de` / `react`)](#4-master-strategy-engine-catalog-co--nc--de--react)
   - [Combat Strategies (`co`)](#combat-strategies-co)
   - [Non-Combat Strategies (`nc`)](#non-combat-strategies-nc)
   - [Movement & Positioning Strategies](#movement--positioning-strategies)
   - [Reaction & Dead Strategies (`react` / `de`)](#reaction--dead-strategies-react--de)
5. [Real-Time Feedback & Bot Inspection Subsystem](#5-real-time-feedback--bot-inspection-subsystem)
   - [Tactical Inspection Whispers](#tactical-inspection-whispers)
   - [Loot & Roll Feedback](#loot--roll-feedback)
   - [Tactical RTS Click-to-Move Control (`rtsc`)](#tactical-rts-click-to-move-control-rtsc)
6. [Master Macro UI Cheat Sheet](#6-master-macro-ui-cheat-sheet)

---

## 1. Open World Population & Autonomous Simulation Engine

The open-world simulation engine (`RandomPlayerbotMgr.cpp`) governs hundreds of bots exploring, questing, farming, and traveling across Kalimdor and the Eastern Kingdoms:

| Command | Target Scope | Description | Example |
| :--- | :---: | :--- | :--- |
| **`.rndbot grind`** | `[name\|%]` | Dispatches idle bots to travel to level-appropriate zones and farm mobs for XP and gold. | `.rndbot grind %` / `.rndbot grind Dunpriest` |
| **`.rndbot rpg`** | `[name\|%]` | Orders bots to visit inns, cities, vendors, and trainers to live an autonomous RPG lifestyle (crafting, training, chatting). | `.rndbot rpg %` |
| **`.rndbot teleport`** | `[name\|%]` | Instantly teleports bot(s) to a randomized level-appropriate zone across the world. | `.rndbot teleport %` |
| **`.rndbot revive`** | `[name\|*]` | Instantly resurrects all dead or ghost bots worldwide at 100% health and mana. | `.rndbot revive *` |
| **`.rndbot upgrade`** | `[name\|%]` | Scans bots and upgrades equipment and spells matching their current level brackets. | `.rndbot upgrade %` |
| **`.rndbot refresh`** | `[name\|*]` | Soft-reboots bots, clearing combat deadlocks, pathing loops, and broken AI states. | `.rndbot refresh *` |
| **`.rndbot change_strategy`** | `[name\|%]` | Rerolls active combat and non-combat strategies across specified bots. | `.rndbot change_strategy %` |

---

## 2. Realm Health, Performance & Lag Governor Tuning

The bot governor dynamically throttles bot activity to maintain smooth tick rates (`RandomPlayerbotMgr.cpp`):

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| **`.rndbot stats`** | 0 | `.rndbot stats` | Spawns a background thread to format complete realm population telemetry: online count, level brackets, race/class breakdown, and roles (Tank/Heal/DPS). | `.rndbot stats` |
| **`.rndbot diff`** | 0 | `.rndbot diff [p_diff e_diff]` | Displays tick latency metrics: `Avg diff`, `Max diff`, Character DB ping, sessions online, and active bots. Pass arguments to tune target diffs. | `.rndbot diff 100 200` |
| **`.rndbot pid`** | 0 | `.rndbot pid [p i d]` | Displays or tunes the real-time Proportional-Integral-Derivative (PID) dynamic controller that balances bot activity based on server tick load. | `.rndbot pid 0.05 0.001 0.05` |
| **`.rndbot update`** | 0 | `.rndbot update` | Forces an immediate AI update tick across the entire realm (`UpdateAIInternal(0)`), evaluating zone population, quests, and login cycles. | `.rndbot update` |
| **`.rndbot clean map`** | 0 | `.rndbot clean map` | Spawns detached threads to unload unreferenced map geometry and VMaps (`unloadMapAndVMaps`) across all maps, freeing server RAM. | `.rndbot clean map` |
| **`.rndbot reset`** | 0 | `.rndbot reset` | Wipes persistent random bot records from database (`DELETE FROM ai_playerbot_random_bots WHERE event != 'temporary'`). | `.rndbot reset` |

> ⚠️ **Every `.rndbot` sub-command is security level 0 — there are no per-command gates.** `rndbot` is a *single* entry in the core chat table at `SEC_PLAYER` (`src/game/Chat/Chat.cpp:1013`), and `RandomPlayerbotMgr::HandleRandomPlayerbotCommand` (`RandomPlayerbotMgr.cpp:3996-4108`) dispatches straight into the handler map without any further security check. `HandleConsoleReset` (`:5082`), `HandleConsoleUpdate` (`:5122`), `HandleConsolePid` (`:5131`), `HandleConsoleDiff` (`:5150`) and `HandleConsoleCleanMap` (`:5182`) each take only a `std::string param`.
>
> This matters most for **`.rndbot reset`**, which any level-0 player can issue: it runs `delete from ai_playerbot_random_bots where event not in ('temporary')` (`RandomPlayerbotMgr.cpp:5087`) and destroys the persisted random-bot roster realm-wide. Same for `.rndbot clean map`, which spawns a detached `unloadMapAndVMaps` thread per map (`:5188-5192`) that will yank map data out from under everyone online. Raise the `rndbot` row in `src/game/Chat/Chat.cpp` if you do not want that.

---

## 3. Bot Persistence & Dynamic World Spawning

Control how bots enter, inhabit, and persist in the world. These are `PlayerbotHolder::*` methods in `modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp` — there is no `PlayerbotHolder.cpp` in this tree (dispatch at `HandlePlayerbotCommand`, `PlayerbotMgr.cpp:1120`; `CreateBot` at `:2527`; `HandleGroup` at `:2771`; `HandleGuild` at `:2178`):

### A. World Persistence (`.bot always`)

By default, party bots despawn when their master logs off. You can toggle offline persistence:

- **`.bot always <name>`**: Toggles persistence flag on a specific bot.
- **`.bot always *`**: Keeps all current party bots alive in the open world indefinitely.
- **What Happens**: When you log off, the bot transitions into an autonomous RandomBot, continuing to roam, fight, and participate in guild activities.

### B. Group Assembly (`.bot group`)

- **`.bot group`**: Fills your group up to `size` (default **5**, counting you — so a solo start gains **4** bots) at your level. There is **no 1 Tank / 1 Healer / 2 DPS allocation**: `PlayerbotHolder::HandleGroup` (`PlayerbotMgr.cpp:2771`) picks each bot's role with `BotRoles(urand(BOT_ROLE_TANK, BOT_ROLE_DPS))` (`:2825`), filtered only by `LfgAction::AllowedClassRoleNr(master, groupSize)` (`:2809`). There is no healer quota, and the class/race is drawn per-bot too. The loop gives up after `10 * size` failed attempts (`:2813`, `:2821`).
- **Arguments**: `.bot group [size=N] [level=N]` (e.g. `.bot group size=5 level=60`). `size` is parsed at `:2803`; `level` and any other key pass through to `CreateBot` (`:2855`).

### C. Custom Bot Character Generator (`.bot create`)

Generates a brand new bot character with exact specifications:

```lua
.bot create name=Tankman class=warrior race=orc level=60 role=tank login=1
```

Supported keys: `name=`, `race=`, `class=`, `level=`, `role=`, `gear=`, `group=`, `login=1`, `temporary=1`.

---

## 4. Master Strategy Engine Catalog (`co` / `nc` / `de` / `react`)

The Strategy Engine dictates bot decision-making (`StrategyContext.h`). Strategies are toggled via prefixes:

- **`+<name>`**: Enables strategy (`/p @tank co +threat`).
- **`-<name>`**: Disables strategy (`/p @tank co -threat`).
- **`~<name>`**: Toggles strategy (`/p @tank co ~threat`).
- **`?`**: Queries active strategies (`/p @tank co ?`).
- **`reset strats`**: Restores class default strategies (`/p @tank reset strats`).

> ⚠️ **The dead-state token is `de`, not `dead`.** `ChatTriggerContext.h:54` defines `creators["de"]` — alongside `co`, `nc`, `react` and `all` — and `ChatCommandHandlerStrategy.cpp:51` adds the matching `"de"` node. There is **no `dead` chat trigger**, so `/p dead +corpse run` is silently dropped. (A literal `"dead ?"` does appear in `SayAction.cpp:14`'s `noReplyMsgs`, but that is only a chat-reply suppression list — not a parser, and it does not list `"dead +"` or `"dead -"` either.)

---

### Combat Strategies (`co`)

Combat strategies activate when the bot enters combat. There is no per-state header: both state variants live in the single pair `strategy/actions/ChangeStrategyAction.{h,cpp}` — `ChangeCombatStrategyAction::Execute` at `ChangeStrategyAction.cpp:8` and `ChangeNonCombatStrategyAction::Execute` at `:46`:

| Strategy Token | Description | Best Used For |
| :--- | :--- | :--- |
| **`threat`** | Maximizes high-threat abilities (Taunt, Sunder Armor, Shield Slam, Righteous Fury, Growl). | Main Tanks |
| **`dps`** | Standard damage rotation prioritizing high-damage cooldowns. | Melee & Ranged DPS |
| **`aoe`** / **`dps aoe`** | Prioritizes Area of Effect spells (Blizzard, Rain of Fire, Whirlwind, Multi-Shot, Consecration). | Large trash packs |
| **`heal`** | Prioritizes party health triage, defensive cooldowns, and direct healing. | Primary Healers |
| **`preheal`** | Pre-casts healing spells on tank before damage lands. | High burst encounters |
| **`focus heal targets`** | Restricts healing exclusively to designated focus targets. | Tank preservation |
| **`conserve mana`** / **`save mana`** | Throttles high-cost spells when mana drops below 40%. | Long raid encounters |
| **`avoid aoe`** | Detects enemy ground spells and immediately strafes out of damaging visual circles. | Boss fights (e.g. Flamestrike, Poison Clouds) |
| **`kite`** | Maintains maximum range, running and slowing melee attackers. | Hunters / Frost Mages |
| **`potions`** | Automatically consumes level-appropriate healing and mana potions on cooldown. | All party members |
| **`racials`** | Automatically activates racial abilities (Blood Fury, Berserking, Stoneform, Will of the Forsaken). | All races |
| **`cast time`** | Prioritizes instant-cast abilities while moving, casting hard spells only when stationary. | High mobility encounters |
| **`behind`** | Maneuvers behind the target to avoid parries/blocks and enable Backstab/Shred. | Rogues / Feral Druids |
| **`close`** | Forces caster/hybrid to close to melee range. | Melee-weaving |
| **`ranged`** | Forces character to stay at maximum spell/ranged range. | Casters / Hunters |
| **`dungeon clear on`** / **`dungeon clear off`** | ⚠️ **Not a `+`/`-` strategy token** — this is a toggle. `dungeon clear on` turns autonomous dungeon clearing on, `dungeon clear off` turns it off. Equivalently `.dc on` / `.dc off`. | Autonomous dungeon runs |

The `dungeon clear` row is the **only** row in this catalog that is not a bare token. Its full chain lives in `mod-dungeon-clear`:

`dungeon clear on` / `dungeon clear off` triggers (`DungeonClearTriggerContext.h:58,60`) → `TriggerNode`s (`Strategy/DungeonClearStrategy.cpp:363,365`) → the `dc on` / `dc off` actions (`DungeonClearActionContext.h:55-56`).

There is no trigger or node named plain `dungeon clear`, so `/p co +dungeon clear` is silently dropped.

---

### Non-Combat Strategies (`nc`)

Non-combat strategies govern out-of-combat behavior, recovery, gathering, and social interaction:

| Strategy Token | Description | Best Used For |
| :--- | :--- | :--- |
| **`food`** | Automatically sits and eats food/drink when health or mana drops below rest threshold. | Downtime recovery |
| **`loot`** | Autonomously approaches corpses and loots items. | Trash cleanup |
| **`gather`** | Detects and gathers nearby herbalism, mining, and skinning nodes. | Open-world farming |
| **`roll`** | Automatically rolls Need, Greed, or Pass on group loot prompts based on item suitability. | Dungeons & Raids |
| **`mount`** | Automatically mounts level-appropriate mount when master mounts or travels long distances. | Open-world travel |
| **`stealth`** | Automatically activates Stealth or Prowl out of combat. | Rogues / Druids |
| **`rpg`** | Engages in autonomous social behaviors (wandering, sitting, chatting, inspecting items). | City & tavern life |
| **`travel`** | Follows calculated world travel routes to quest objectives and flight masters. | Open-world questing |
| **`wbuff`** | Travels to world buff drop locations (Onyxia head, Rend buff, Hakkar buff). | Raid prep |
| **`fish`** / **`tfish`** | Equips fishing pole and casts fishing line near water. | Fishing automation |
| **`chat`** | Listens for whispers and chat channels and generates replies. | Social interaction |
| **`ai chat`** | Routes chat replies through the LLM personality subsystem. | Roleplaying |

---

### Movement & Positioning Strategies

Control how bots move in relation to master and enemies:

| Strategy Token | Description |
| :--- | :--- |
| **`follow`** | Follows master maintaining standard follow formation (`SetFormationAction`). |
| **`stay`** | Holds ground at current coordinates until commanded to move. |
| **`runaway`** / **`flee`** | Sprints away from melee attackers to regroup at safe distance. |
| **`guard`** | Holds ground at current location and attacks any hostile mob entering aggro radius. |
| **`free`** | Disables tethering to master; bot wanders and acts autonomously. |
| **`wander`** | Roams within a localized radius around anchor point. |

---

### Reaction & Dead Strategies (`react` / `de`)

- **Reaction Strategies (`react`)**:
  - `flee from adds`: Automatically breaks combat and retreats when unexpected adds pull.
  - `avoid mobs`: Paths around aggressive mobs during travel to avoid accidental aggro.
- **Dead Strategies (`de`)**:
  - `corpse run`: Ghost bot automatically navigates from graveyard back to corpse and revives.
  - `release`: Automatically releases spirit to graveyard upon death.
  - `repop`: Forces instant spirit release and graveyard respawn.

  Usage: `/p de +corpse run`, `/p de -corpse run`, `/p de ?`.

---

## 5. Real-Time Feedback & Bot Inspection Subsystem

Bots provide instantaneous feedback and inspection data via whispers (`ChatActionContext.h`):

### Tactical Inspection Whispers

| Whisper Query | Action | Description of Feedback Returned |
| :--- | :--- | :--- |
| **`/w %t stats`** | `StatsAction` | Reports current Level, Health/Mana, Armor, Attack Power, Spell Power, Defense rating, and primary stats (Strength, Agility, Stamina, Intellect, Spirit). |
| **`/w %t who`** | `WhoAction` | Reports current bot action, combat target name, health percentage, and active strategy state. |
| **`/w %t where`** | `GoAction` | Dumps current Map ID, Zone name, Subzone name, and exact XYZ coordinates. |
| **`/w %t spells`** | `ListSpellsAction` | Whispers complete list of learned spells, ranks, and cooldown timers. |
| **`/w %t talents`** | `ChangeTalentsAction` | Reports allocated talent points across all three class talent trees. |
| **`/w %t items`** *(or `c`)* | `TellItemCountAction` | Reports equipped gear, durability percentage, free bag slots, and total gold count. |
| **`/w %t quests`** *(or `q`)* | `ListQuestsAction` | Dumps active quest log, quest objectives, and completion percentages. |
| **`/w %t los`** | `TellLosAction` | Confirms whether bot has direct unobstructed Line of Sight (LoS) to target. |
| **`/w %t reputation`** | `TellReputationAction` | Dumps reputation standings with major factions. |

---

### Loot & Roll Feedback

When group loot drops in dungeons and raids:

- **`/p roll feedback`**: Causes bots to whisper why they rolled Need, Greed, or Pass (e.g. *"Rolling Need: Upgrade (+14 Agility)"* or *"Rolling Pass: Class cannot use Mail"*).
- **Manual Roll Override**: Command all bots to roll uniformly:
  - `/p roll need` — All bots roll Need.
  - `/p roll greed` — All bots roll Greed.
  - `/p roll pass` — All bots roll Pass.

---

### Tactical RTS Click-to-Move Control (`rtsc`)

The **Real-Time Strategy Control (RTSC)** subsystem transforms bot control into an RTS tactical squad game:

1. **Activate RTSC**: Whisper `/w %t rtsc` to enter tactical mode.
2. **Select Bots**: Click ground circles or whisper `/w %t rtsc select`.
3. **Click to Move**: Left-click on the ground or minimap; selected bots path directly to that ground coordinate.
4. **Target Attack**: Right-click an enemy mob; bots surround and engage target.
5. **Cancel RTSC**: Whisper `/w %t rtsc cancel` to return bots to standard follow AI.

---

## 6. Master Macro UI Cheat Sheet

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`):

> ⚠️ **Read this before using any `/p @…` macro**
>
> **There is no `@all` filter, and no `@healer`.** `CompositeChatFilter` composes 16 filters (`ChatFilter.cpp:1224-1242`), and the *complete* role-filter set is just these eight (`ChatFilter.cpp:182-217`, self-documented examples at `:159-170`):
>
> `@tank` · `@dps` · `@heal` · `@notank` · `@nodps` · `@noheal` · `@ranged` · `@melee`
>
> Note `@heal`, **not** `@healer`. Anything outside that set is not merely ignored — it is silently destroyed:
>
> 1. `ChatFilter::Filter` (`ChatFilter.cpp:10-19`) only strips the `@…` token when a filter actually *matched*. An unmatched prefix like `@all` is therefore **left in the message**.
> 2. The surviving text is queued by `PlayerbotAI::HandleCommand` (`PlayerbotAI.cpp:1714`).
> 3. `ExternalEventHelper::ParseChatCommand` then trims it down to a bare `all` / `healer` / `co` / `nc`.
> 4. `all` is a *real* trigger but `healer` is not, and neither reaches the intended action. Either way **the macro does nothing, with zero feedback.**
>
> Plain `/p …` already reaches every bot in the party, so the `@` prefix is only worth using when you genuinely want to address one role subset.

### 🛡️ Macro 1: Tank & Threat Lock

Locks tanks into maximum threat generation while ranged casters maximize DPS. Note `@heal`, **not** `@healer` — both prefixes below are from the real role-filter set:

```lua
/p @tank co +threat,-dps
/p @dps co +dps,-threat,+avoid aoe
/p @heal co +heal,+preheal,+conserve mana
```

### 🏃 Macro 2: Panic Disengage & Mass Regroup

Forces all party bots to break combat and sprint back to master:

```lua
/p flee
/p co +flee,-threat,-dps
/p formation near
```

### 🗺️ Macro 3: Open World Mob Farming Mode

Configures party bots to gather nodes, loot corpses, eat food, and mount automatically:

```lua
/p nc +gather,+loot,+food,+mount
/p formation circle
```

### 🔍 Macro 4: Full Party Status Audit

Commands all party bots to report their health, mana, durability, and free bag space:

```lua
/p stats
/p items
```
