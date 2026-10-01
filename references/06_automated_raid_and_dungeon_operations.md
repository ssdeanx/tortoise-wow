# 06. Automated Raid & Dungeon Operations Handbook

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + mod-playerbots + mod-dungeon-clear + Eluna
> **Engine Verification**: `run/lua_scripts/raid_automation.lua` (**v1.4.0** — `local SCRIPT_VERSION = "1.4.0"` at `run/lua_scripts/raid_automation.lua:632`), `PlayerbotMgr.cpp`, `src/game/Chat/Chat.cpp`, `DungeonClearCommand.cpp`
> ⚠️ An earlier revision of this header cited `Commands.cpp` as a source file. **No such file exists in this tree.** The chat table lives in `src/game/Chat/Chat.cpp`; the playerbot command handlers live in `modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp`.
> **Security Requirement**: Accessible to all players (`SEC_PLAYER` / Level 0) with elevated GM features where noted
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Playerbot Handbook](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md)

> ⚠️ **Read `run/lua_scripts/`, not `lua_scripts/`, when you audit the script.**
> *(Original warning, kept for history — the situation it describes has since
> changed. See the note immediately below it.)* Two copies of
> `raid_automation.lua` used to exist in this repository and **only one was
> live**: `run/mangosd.conf` sets `Eluna.ScriptPath = "./lua_scripts"`
> **relative to the `run/` working directory**, so the live script is
> `run/lua_scripts/raid_automation.lua`. A stale **v1.1.0** copy sat at the
> repo-root `lua_scripts/raid_automation.lua` and was **never loaded** — v1.1.0
> has no `tank3` / `tank4` / `tanks` / `marks` / `ccmark` / `resume` / `gather` /
> `avoid` / `roll` at all. Auditing or editing the wrong file produces a large
> set of false "this command does not exist" findings. That happened during this
> handbook's audit.
>
> **⚠️ The live-copy advice still holds; the "two copies exist on disk" premise no
> longer does.** `find . -name raid_automation.lua` now returns exactly **one**
> hit, `run/lua_scripts/raid_automation.lua` (**v1.4.0**). The v1.1.0 copy is
> still in git **HEAD** (`git show HEAD:lua_scripts/raid_automation.lua` → 700
> lines, `SCRIPT_VERSION = "1.1.0"` at `:18`) but has been **deleted from the
> working tree and not yet committed** — `git status --short` shows
> ` D lua_scripts/raid_automation.lua`. Two consequences:
> 1. **A fresh clone of HEAD, or a `git checkout` of that path, resurrects the
>    stale v1.1.0 file.** Do not do it. If a future `git checkout`/branch-switch
>    restores it, delete it again before auditing anything.
> 2. Until the deletion is committed, a colleague's clean clone and this working
>    tree disagree about what the repo contains. That is a git-state question, not
>    an engine question, and the engine answer is unchanged: `run/` is the only
>    directory Eluna ever loads.

---

## 📑 Table of Contents

1. [Architectural Overview & Core Philosophy](#1-architectural-overview--core-philosophy)
2. [Master Macro Cheat Sheet (Copy-Paste Ready)](#2-master-macro-cheat-sheet-copy-paste-ready)
3. [The Master 1-Click Pipeline (.raid ready)](#3-the-master-1-click-pipeline-raid-ready)
4. [Instant Raid Attunements & Master Keyring Suite](#4-instant-raid-attunements--master-keyring-suite)
5. [Autonomous Dungeon Clear (.dc) Operations Deck](#5-autonomous-dungeon-clear-dc-operations-deck)
   - [5.1 Complete `.dc` Command Reference (Engine-Verified)](#51-complete-dc-command-reference-engine-verified) — **read this before testing a raid**
6. [In-Dungeon Recovery, Doors, AoE & Target Focus](#6-in-dungeon-recovery-doors-aoe--target-focus)
7. [Automatic Role & Formation Matrix](#7-automatic-role--formation-matrix)
8. [Master Instance Fast-Teleport Directory](#8-master-instance-fast-teleport-directory)
9. [C++ Engine Contracts & Technical FAQ](#9-c-engine-contracts--technical-faq)
10. [The `.raid` Operator Facade (v1.4.0)](#13-the-raid-operator-facade-v140) — **the full five-layer command surface, the `@filters`, and the trap register** *(numbered §13; it sits here because it is command reference, not encounter mechanics)*

---

## 1. Architectural Overview & Core Philosophy

This operations handbook documents the **Tortoise-WoW Autonomous Raid Controller**, an enterprise-grade Eluna automation layer designed to eliminate manual command micromanagement during 5-man dungeon runs and 10/20/40-man raids.

### Core Design Rules

- **No Level Tampering**: Bots and characters invited via the in-game group finder (LFT / LFG) are already level 60. The controller never alters character levels.
- **Player-Controlled Group Topology**: In Vanilla WoW, a standard group is capped at 5 members. Raids hold 6 to 40 members across 8 subgroups. Group-to-raid conversion is handled by the player via the in-game UI. The controller never forces group conversions.
- **Engine-Verified Syntax**: All commands dispatch directly into the CMaNGOS command handler (`PlayerbotMgr.cpp`, `DungeonClearCommand.cpp`) using native server tokens. ⚠️ An earlier version of this bullet also listed `Commands.cpp`. **No such file exists in this tree** — the base chat table is `src/game/Chat/Chat.cpp` and the playerbot handlers are `modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp`. Client chat prefixes (`/p`, `/ra`) are never sent into server command queues.
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
empty list is what `dc on` will refuse on. ⚠️ An earlier version of this note
called `dc bosses` **the only** verb with no authorization gate. That exclusivity
is false — see the corrected gate list in [§13.2](#132--battle--run-control).)*

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

> ⚠️ An earlier version of this macro used `.raid focus`, which was documented as
> adding a `co +mark rti` strategy. That description was wrong, but the command
> **does** exist: `focus` is an alias of `skull` (the dispatch is
> `elseif sub == "skull" or sub == "focus" or sub == "mark"`), and it sets icon 7
> on your target — not a strategy. The focus *lock* is a separate thing: it is
> part of `.raid roles` (`co +focus rti targets`) and is released by
> `.raid unfocus`. Tank-assigned marking is `.raid marks <tank…>`.

---

## 3. The Master 1-Click Pipeline (`.raid ready`)

> ⚠️ **`.bot group` comes first — but it does not give anyone a role-matched
> spec, and treating it as if it does is the single most damaging belief in this
> handbook.**
> *(Superseded instruction, kept visible: this note previously read "**.bot group
> comes first, and it is not optional.** `.bot group size=N` is what gives every
> bot its role-matched spec. `.raid ready` does **not** do that, and deliberately
> does not try." The premise was false, and it pointed at a superseded cross-doc
> claim — [`.bot group` assigns the spec too](../02_playerbot_and_raid_automation.md)
> is corrected in the same way.)*
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

**There is no Stage 5 any more.** An earlier version of this pipeline described an
"Instant Raid Buffs" stage casting Fortitude / Shadow Protection / Spirit / Arcane
Brilliance / Greater Blessing of Kings across the group. That stage was built on a
per-class aura table inside the script, and it was **actively harmful**: it sent
`water shield` to *every* shaman, overwriting the engine's correct per-spec
choice (enhancement gets *lightning shield*, restoration gets *water shield*).

The underlying need is already met by the engine — `buff` is in every default
combat kit and each spec strategy wires its own aura — and now that the spec
strategy is actually installed, the engine buffs correctly on its own. Any such
table in a script would break it again. `.raid buff` therefore **sends nothing**
and reports how this works instead.

### Ready Subcommands

| Command | Arguments | Description |
| :--- | :--- | :--- |
| `.raid ready` | *none* | `roles` + `gear` (quality `best`) + `attune`. |
| `.raid ready epic` | `epic` | Same, but gears baseline epics instead of best-in-slot. |
| `.raid gear` | `[quality]` | Re-gears and re-enchants without re-running attunements. Accepts `best`, `green`/`uncommon`, `blue`/`rare`, `purple`/`epic`, `partial`, `upgrade`, `sync`. |
| `.raid roles` | *none* | Roster census + formations split by role + DPS focus lock. Sends no talent commands. |
| `.raid tank3` | *none* | **The 3rd-tank override.** Promotes your current target. Needs a target or a name — it never guesses. |
| `.raid tank4` | *none* | Promote to a 4-tank roster. Add a name to choose who. |
| `.raid tanks <n>` | `1`–`8` | Same thing, any count. **The only re-spec path.** |
| `.raid marks <tank…>` | bot names | One mark icon per named tank, in priority order. |
| `.raid buff` · `.raid worldbuff` · `.raid wbuff` | *none* | Sends nothing — explains how buffing works here. See Stage 5 note above. |

---

## 4. Instant Raid Attunements & Master Keyring Suite

Running `.raid attune` (or `.raid ready`) guarantees your character and raid can enter every raid instance and open every dungeon door across Azeroth.

### Completed Attunement Quests

| Raid / Instance | Attunement Quest ID | Quest Name & NPC Source |
| :--- | :---: | :--- |
| **Molten Core** | `7848` | *Attunement to the Core* (Lothos Riftwaker) |
| **Blackwing Lair** | `7761` | *Blackhand's Command* (Scarshield Quartermaster) |
| **Onyxia's Lair (Alliance)** | `6502` | *Drakefire Amulet* (Jailbreak / Marshall Windsor) |
| **Onyxia's Lair (Horde)** | `6602` | *Blood of the Black Dragon Champion* (Rexxar / General Drakkisath) |
| **Naxxramas (Exalted)** | `9123` | *The Dread Citadel - Naxxramas* (Archmage Angela Dosantos) |
| **Emerald Sanctum (40-Man)** | `40962` | *Into the Dream VI* (Turtle-WoW Custom 40-Man) |
| **Tower of Karazhan (10/40-Man)** | `40829` | *The Key to Karazhan X* (Turtle-WoW Custom Raid) |
| **Timbermaw Hold (20-Man)** | `41931` | *The Corruption of Timbermaw Hold* (Turtle-WoW Custom 20-Man) |
| **Hateforge Quarry (5-Man)** | `40489` | *Assaulting Hateforge* (Turtle-WoW Custom Dungeon) |

*Note: The script also sets Argent Dawn reputation to Exalted (`.modify rep 529 42999`), satisfying the free Naxxramas entrance prerequisite.*

### Master Keyring Items Spawned in Bags

| Item ID | Item Name | Associated Instance / Doors Unlocked |
| :---: | :--- | :--- |
| `16309` | **Drakefire Amulet** | Onyxia's Lair Raid Entrance Key |
| `22754` | **Eternal Quintessence** | Molten Core Permanent Infinite Rune Dousing |
| `12344` | **Seal of Ascension** | Upper Blackrock Spire (UBRS 10-Man) Entrance Gate |
| `13704` | **Skeleton Key** | Scholomance Front Gate Key |
| `12382` | **Key to the City** | Stratholme Service Entrance Gate Key |
| `11000` | **Shadowforge Key** | Blackrock Depths Gates & Mechanism Key |
| `18249` | **Crescent Key** | Dire Maul West & North Gates Key |
| `7146` | **The Scarlet Key** | Scarlet Monastery Armory & Cathedral Doors |
| `9287` | **Workshop Key** | Gnomeregan Back Entrance Key |
| `50545` | **Gemstone of Ysera** | Emerald Sanctum (40-Man) Raid Key |
| `61234` | **Upper Karazhan Tower Key** | Tower of Karazhan (10/40-Man) Raid Key |
| `51356` | **Karazhan Crypt Key** | Karazhan Crypts (5-Man) Dungeon Key |
| `42379` | **Rune of Illumination** | Timbermaw Hold (20-Man) Raid Key |
| `53005` | **Scarlet Crusade Insignia** | Scarlet Citadel (10-Man) Raid Key |

---

## 5. Autonomous Dungeon Clear (`.dc`) Operations Deck

The autonomous solver engine (`mod-dungeon-clear`) drives your party's tank bot through the optimal path of any dungeon or raid instance.

### Navigation & Autopilot Commands

| Command | Arguments | Operational Effect |
| :--- | :--- | :--- |
| **`.raid start`** (or `.raid dc`) | *none* | Engages `.dc on` and `.dc pull dynamic`. The tank bot calculates navmesh paths, engages trash packs, and pulls bosses. |
| **`.raid off`** (or `.raid stop`) | *none* | Disables the autonomous dungeon clear engine completely (`dc off`). `stop` is a v1.3.0 alias that lands on the same branch. |
| **`.raid pause`** | *none* | Pauses the tank bot's movement and drops combat. The script sends **`dc pause pause`** — the explicit intent form, never the bare toggle. Ideal for breaks or loot distribution; resume with `.raid resume` (`dc pause resume`). ⚠️ An earlier version of this row listed `.raid stop` as a pause alias and mentioned a `combatstop` string. **Neither exists**: `stop` is an alias of `off`, and `combatstop` appears nowhere in the script. |
| **`.raid pull`** | `[off\|on\|dynamic]` | Sets the **advanced-pull (LOS pull-to-camp) mode**. `off` = tank melees the pack in place. `on` = always drag the pack out to a camp first. `dynamic` = decide per pack. A **bare** `.raid pull` **sets `dynamic`** — it does not cycle. The script coerces anything unrecognised to `dynamic` before forwarding, so `.raid pull leeroy` silently becomes dynamic. The engine's own bare `(current+1)%3` toggle (`DungeonClearChatActions.cpp:1216`) is real but **`.raid pull` never reaches it**, because the wrapper clamps first. ⚠️ **Only these three words are valid** — see the correction note in [§5.1](#51-complete-dc-command-reference-engine-verified). |
| **`.raid go <boss>`** | `<boss_name>` | Overrides default routing to direct the tank bot straight to a specific boss (e.g. `.raid go Lucifron`, `.raid go Magmadar`). |
| **`.raid skip`** | *none* | Commands the solver to abandon pathing to the current stuck trash pack or optional boss and route to the next objective. |
| **`.raid bosses`** | *none* | Dumps a live checklist of all bosses in the current instance with their **Alive** or **Dead** status. |
| **`.raid status`** | *none* | Displays completion percentage, active route segment, and current pathing target. |
| **`.raid config`** | *none* | Inspects live solver tunables (pull radius, leash duration, rest thresholds). |
| **`.raid spectate`** | `[follow <name>\|next\|prev\|previous\|list]`, or bare | Activates the cinematic spectator camera to watch your bots clear encounters from third-person perspectives. The script forwards your whole argument to `dc spectate <rest>`, so the whole engine vocabulary works: `next` / `prev` / `previous` walk every bot in the instance, `list` shows the roster, `follow <name>` rides a named bot, and bare = free-fly. `free` is accepted by the engine (`DungeonClearCommand.cpp:212`) and is **identical to bare** — an earlier version of this row listed it as a qualifier; it is not one. |

---

## 5.1 Complete `.dc` Command Reference (Engine-Verified)

*Source of truth: `modules/mod-dungeon-clear/src/DungeonClearCommand.cpp`, cross-checked against `DungeonClearChatActions.cpp`, `TestRun/DcTestDungeonRegistry.cpp`, and `run/lua_scripts/raid_automation.lua`. Everything below is read from code, not assumed.*

### 🚨 Read this first: how to address a raid of bots

| Route | Packet | Reaches a bot? |
| :--- | :--- | :--- |
| `/w <bot> <cmd>` · `.bot w <bot> <cmd>` | `CHAT_MSG_WHISPER` | ✅ yes — one named bot |
| `/ra <msg>` · `.bot r <msg>` | `CHAT_MSG_RAID` | ✅ **yes — every bot. One message instead of forty** |
| `/p <msg>` · `.bot p <msg>` | `CHAT_MSG_PARTY` | ⚠️ yes, **party only** — one subgroup in a raid |
| `.bot c <bot> <cmd>` | — | ❌ nobody (debug stub) |

**`.bot r` works, and it always has — no core change, no rebuild.** An earlier
revision of this document claimed it was dead and needed a patch to
`PlayerbotAI.cpp`. That was wrong and the patch has been reverted. The reason it
works is not the obvious one: `.bot r` does not message the bots at all. It
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
with the LLM off it buys you nothing. Full trace and the correction:
`references/02_playerbot_and_raid_automation.md` → *Raid chat, and a wrong claim
this handbook used to make*.

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
  bots being promoted. `.raid roles` sends none. ⚠️ **The old justification here was
  "because `.bot group` already assigned role-matched, varied specs" — that is
  false** (see the Macro 0 correction: `HandleGroup` never forwards `role=`, so
  `AutoSelectTalents` gets `BOT_ROLE_NONE` and picks a random premade spec).
  **The real reason `.raid roles` sends none is that it does not know which bots
  need promoting** — it sends no talent command at all, on purpose. Which means the
  tank commands are not a formality you can skip: they are the **only** place any
  bot in this pipeline ever gets a role-matched spec.
- `rti <icon>` + `co +mark rti` — only in `.raid marks <tank…>`, because each
  tank needs a *different* icon and no filter can express "tank #2 gets cross".

Note the role filters do **not** require the roles to exist first, as an earlier
version of this document claimed. `@tank` resolves through
`PlayerbotAI::IsTank`, which reads the installed spec strategy. ⚠️ The old
conclusion here was "that is already correct before `.raid roles` runs, because
`.bot group` built it. So the formation split works on the first call." **That
inference does not hold.** The *mechanism* claim is right — the filters need no
prior setup step and they do work on the first call — but the *reason* was wrong:
`.bot group` does not build a spec, so a bot it created may resolve as **not** a
tank. The formation split will then silently assign those bots `@ranged` or
`@melee` instead of `shield`. **Verify with `.raid check` and promote with
`.raid tanks <n>` before `.raid roles`**, or you are formatting a raid that
nobody is tanking.

Use a `bot` subcommand that accepts `*` (`.bot gear * best`,
`.bot consumes *`, …) for server-side iteration.

### ⚠️ `.dc test` cannot run raids

**`.dc test` cannot run the raids you care about.** The automated harness is
registered for **68 dungeons**, and the count and the token list are exact
(`TestRun/DcTestDungeonRegistry.cpp` — 68 rows, names and order as printed below).
⚠️ **Superseded wording, kept visible:** an earlier version of this section said
the harness is registered for "**68 dungeons only — every one of them a 5-man**",
and further down it said flatly "**No raid tokens exist.**" **Both absolutes are
false**, and this document contradicted itself: §12.3 describes map 45 as a
**40-Man Raid Fortress**, and the corrections table below already admits
`scarlet-citadel` is "the only raid-flavoured entry in the list."

**The precise position:** the 68 rows are overwhelmingly 5-man dungeons, plus
**one raid-flavoured row** — `scarlet-citadel`, map `45`, level 63, no heroic
variant. So it is not "no raid tokens exist"; it is "the only raid token that
exists is the Turtle-WoW Scarlet Citadel." There is **no** token for Ruins or
Temple of Ahn'Qiraj, Molten Core, Blackwing Lair, Naxxramas, Onyxia's Lair or
Karazhan. Typing one of those returns:

```text
unknown dungeon 'aq40' — see .dc test list
```

**The practical conclusion is unchanged** — every vanilla raid and every AQ wing
has to be tested through the live half of the engine instead. You stand in the
raid, hand it a tank bot, and drive it:

```text
.raid roles
.raid start
.dc status
.dc bosses
.dc spectate follow
```

That is the supported path for AQ20/AQ40/MC/BWL/Naxx/Onyxia. `.dc test` is the unattended 5-man regression harness: it rolls its own party, teleports them in, gears them, and reports — and by design **you are not in that party**, so you watch it with `.dc test watch`.

### The two halves of `.dc`

| | Live commands | `.dc test` harness |
| :--- | :--- | :--- |
| **Who may run it** | Any player (`SEC_PLAYER`) | **GM only** (`SEC_GAMEMASTER`) |
| **Where** | In-game session only | In-game **or the server console** |
| **Who it drives** | The **tank bot in your group** | Its own generated party (you are not in it) |
| **Content** | Any map DC supports — **including all raids** | **68 tokens: 5-man dungeons plus the raid-flavoured `scarlet-citadel`** |
| **Purpose** | You supervise a real run | Unattended batch regression runs |

Every live command routes through `DungeonClearDispatch::DispatchToTankBots`, which searches your group for a bot holding the **tank** role. With no tank bot you get `No tank bot found in your group.`

#### 🚨 That role does NOT come from `.bot group`. Read this before you trust `.dc on`

*(Superseded claim, kept visible: this paragraph previously read "**That role
comes from `.bot group`, not from `.raid roles`.** `IsTank` reads the installed
spec strategy, and `.bot group` already built a tank-role spec for the bots it
created as tanks. So the requirement is satisfied before any `.raid` command runs
— as long as you assembled with `.bot group`." **Every sentence of that is
wrong.**)*

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
> **That second sentence is the same false claim this section corrects.** It is
> live in v1.4.0, so expect to read it every time you run `.raid roles`, and do
> not treat it as confirmation. The first sentence is accurate. The Lua is a
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

| Command | Arguments | What it actually does |
| :--- | :--- | :--- |
| `.dc on` | *none* | Enables the run: installs the advance/engage/stall trigger ladder, resets per-run state, arms the status pusher. **Refuses** if a party member is dead and cannot be recovered. Applies any pull mode you pre-set with `.dc pull`. |
| `.dc off` | *none* | Disables the run and tears down the strategy. |
| `.dc pause` | *none* | Pauses the tank. Auto-resumes on its own if a door is later opened (`DungeonClearDoorReopenedTrigger`) — you do not have to hit resume manually. |
| `.dc skip` | *none* | Abandons the current stuck pack / optional boss and routes to the next objective. |
| `.dc pull` | `[off\|on\|dynamic\|dyn]`, or bare | Advanced-pull (LOS pull-to-camp) mode. **`off`** melees the pack in place, **`on`** always drags it out to a camp, **`dynamic`** (synonym **`dyn`**) decides per pack. A bare call cycles `off → on → dynamic → off` — that is true **at the engine level** (`DungeonClearChatActions.cpp:1206-1216`). ⚠️ But the **`.raid` wrapper never reaches that path**: `.raid pull` clamps an unrecognised or empty word to `dynamic` first, so a bare `.raid pull` sets dynamic and the toggle is unreachable through the facade. Use `.raid dc pull` if you genuinely want the engine's bare toggle. Settable **before** `.dc on` — the preference is remembered and applied the moment the run starts. Turning it off mid-pull aborts the in-flight maneuver. |
| `.dc status` | `[addon\|silent]` | `Dungeon clear: on/off. Next boss: <name>. Skipped: <n>.` plus a stall reason when one is latched. The server also pushes status packets automatically on state changes, so you rarely need to poll. |
| `.dc bosses` | `[addon\|silent]` | Full boss checklist with Alive/Dead state. **Works even before `.dc on` has ever been issued** — it resets stale completion latches itself, so a re-entered instance does not show phantom "Done" rows. |
| `.dc go` | `<boss name or entry>` | Overrides routing to a specific boss. Accepts a **numeric creature entry** or a **case-insensitive name substring**. Requires an active run, else: `Dungeon clear is not enabled. Please enable it first.` |
| `.dc config` | *none — display only* | Dumps **every** DungeonClear tunable as the AI actually reads it this tick. `*` = active per-run override, `H` = heroic-mode default. Because it reads live config, it confirms a **`mod_dungeon_clear.conf`** edit took effect **without** `.reload config`. ⚠️ An earlier version of this row said `worldserver.conf`; that was wrong and it disagreed with [§13.2](#132--battle--run-control), which had it right. The file is `run/mod_dungeon_clear.conf` (the `.dist` template is `DcSettingsRegistry.h:11`), and `DungeonClearUtil.h:36` names the `Logger.playerbots.dungeonclear.pull` line in it. **Nothing under `modules/mod-dungeon-clear/src/` reads `worldserver.conf`.** |
| `.dc spectate` | `[follow [name]\|next\|prev\|list]`, or bare | Camera. Bare = free-fly. `follow` rides a bot (bare `follow` is a toggle — that is how you turn it off); `follow <name>` jumps to a named bot. `next`/`prev` walk every bot in the instance, not just tanks. `list` shows the roster. Acts on **you**, not the tank, so it works even if you are not the tank and even from outside the party. |

### `.dc test` Commands (GM only)

| Command | Arguments | What it does |
| :--- | :--- | :--- |
| `.dc test list` | *none* | Prints all 68 supported tokens with name, map id, and level. Run this first — it is the live authority. |
| `.dc test start` | `<dungeon> [heroic] [level=N] [seed=N] [ilvl=N\|none] [quality=normal\|uncommon\|rare\|epic\|legendary]` | Rolls a **random** party from the addclass pool, teleports it in, gears it, and runs. `seed=N` replays one specific comp. `ilvl=` / `quality=` cap gear (defaults come from the `AiPlayerbot.AutoGear*` conf lines). `heroic` runs the heroic variant. |
| `.dc test start` | `<dungeon> party=Tank,Heal,Dps1,Dps2,Dps3 [heroic]` | Hand-picks a party of **real player characters** instead. Roles are positional. `level=`, `seed=`, `ilvl=` and `quality=` are **rejected**, not ignored — your characters are never relevelled or re-geared. |
| `.dc test status` | *none* | Run status, plus active plan status when a plan is running. |
| `.dc test stop` | `[selector]` | Stops a run. Selector: **bare** = the single active run (errors if more than one), **`all`** (also stops every active plan first, or the scheduler would immediately relaunch what you aborted), an exact **runId**, or a **dungeon token** (stops all its runs). |
| `.dc test gear` | `<dungeon> [heroic]` | Lists the `ilvl=` ceilings worth running that dungeon at. |
| `.dc test watch` | `[selector\|next\|off]` | Puts your camera on a run: hides you (a visible GM invalidates the run), binds + teleports you to the run's **entrance**, and arms the follow camera on arrival. `next` tours a plan's concurrent runs. `off` ends it, returns you to where you were, and drops the instance binds it created. Needs a session — console cannot watch. |
| `.dc test plan` | `start \| status \| stop` | Batched campaigns — N runs with capped concurrency. A plan does **not** need an issuer up front, so the very first console start that kicks off the driver login works. |

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

**No vanilla raid tokens exist.** ⚠️ *(Superseded: this line previously read
"**No raid tokens exist.**" That absolute was false and contradicted this
document's own §5.1 corrections table and §12.3. There **is** one raid-flavoured
row — `scarlet-citadel`, map `45`, the Turtle-WoW 40-Man Raid Fortress described
in [§12.3](#123-scarlet-citadel-map-45-40-man-raid-fortress), which is a 40-man
raid with seven scripted bosses, not a 5-man. Everything vanilla — AQ20, AQ40, MC,
BWL, Naxx, Onyxia, Karazhan — has no token at all, and for those you use the live
commands above.)*

### `.dc go` reference for Ahn'Qiraj (maps 509 / 531)

Verified against `tw_world` — use these with `.dc go <entry>` to jump the run to a specific encounter. This core's entries differ from some classic databases, so **use these, not remembered ones** (e.g. Huhuran is `15509` here, and `15605` is Elder Riversong).

**Ruins of Ahn'Qiraj (map 509)**

| Entry | Boss | Notes |
| :---: | :--- | :--- |
| `15348` | Kurinnaxx | |
| `15341` | General Rajaxx | Passive/invincible on spawn — the native 7-officer chain gates him. |
| `15370` | Buru the Gorger | |
| `15340` | Moam | |
| `15369` | Ayamiss the Hunter | |
| `15339` | Ossirian the Unscarred | End boss. Weakness crystals are a mid-fight click. |

**Temple of Ahn'Qiraj (map 531)**

| Entry | Boss | Notes |
| :---: | :--- | :--- |
| `15263` | The Prophet Skeram | First boss. Guarded by the sentinel room. |
| `15544` / `15543` / `15511` | Vem / Yauj / Kri | Bug Trio — **all three** must die; each death heals the survivors. |
| `15516` | Battleguard Sartura | |
| `15510` | Fankriss the Unyielding | |
| `15299` | Viscidus | Side room between Fankriss and Huhuran. |
| `15509` | Princess Huhuran | |
| `15275` / `15276` | Emperor Vek'nilash / Vek'lor | Linked pair — one death ends the fight. |
| `15517` | Ouro | ⚠️ **No static spawn** (0 rows). Summoned by the Ouro Spawner. Will **not** appear in `.dc bosses`. |
| `15727` | C'Thun | End boss. Body is non-selectable until the Eye dies. |
| `15589` | Eye of C'Thun | ⚠️ **No static spawn** (0 rows) — a summon. Phase 1 only. |
| `15957` | Ouro Spawner | Not a boss. The NPC that summons `15517`. |

Practical consequence: because Ouro and the C'Thun Eye have no spawn rows, they are driven by the DungeonClear **event** pipeline rather than the plain boss list. If `.dc bosses` looks like it is missing them, that is expected — the objective anchors show up instead, and `.dc status` reports what the run is actually walking toward.

### Corrections to earlier sections of this handbook

| Earlier claim | Reality |
| :--- | :--- |
| §5: `.raid pull [dynamic\|leeroy\|advanced]` | Only **`off` / `on` / `dynamic`** exist. `raid_automation.lua` **clamps** an unrecognised word to `dynamic` before forwarding it to `dc pull`, so `.raid pull leeroy` **silently sets dynamic** — it never reaches the engine's bare-toggle branch. (An earlier version of this row said it passed the word straight through and therefore cycled the mode; the clamp is what the script actually does, and it also prints a correction on chat.) `leeroy` is a *pull-decision outcome*, not a settable mode: `dynamic` is documented in-engine as "auto-deciding **Leeroy vs pull** per pack" (`DungeonClearChatActions.cpp:1244`), governed by the `PullDynamicMaxLeeroyMobs` ceiling. `advanced` is the other half of that decision, likewise not a mode you type. |
| §5: `.raid pull` "Forces an immediate tank pull" | It does not pull. It only sets the advanced-pull **mode** for the rest of the run. |
| §5 table omits `.dc pause` | `.dc pause` is a real engine verb, and `.raid pause` / `.raid resume` wrap it — but with the **explicit intent forms** `dc pause pause` / `dc pause resume`, never the bare toggle, which races DC's own auto-pause sites (`DungeonClearChatActions.cpp:1096-1107`). ⚠️ An earlier version of this row also claimed `stop` and `hold` were aliases. **Neither exists in v1.2.0 or v1.3.0.** In v1.3.0 `stop` is a separate alias of `off` (`dc off`), i.e. it *stops the run*; there is no `hold` at all. |
| §12.3: `dc test scarlet-citadel` | Correct. `scarlet-citadel` (map 45) is a real token, level 63, no heroic — and it is the only raid-flavoured entry in the list. |

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

| Alias | Was resolving to | Now | Fixed |
| :--- | :--- | :--- | :---: |
| `.raid tele ony` | substring → `Onyxia'sLair` id 143, **map 1 (outdoors, Darkshire)** | `onyxia` id 195, **map 249 (inside)** | ✅ |
| `.raid tele onyxia` | same bug | `onyxia` id 195 | ✅ |
| `.raid tele mc` | substring → `TheMoltenSpan` id 163, **map 0 (world)** | `mc` id 190, **map 409 (inside)** | ✅ |
| `.raid tele naxx` | `Naxxramas` id 270, **map 0 (outdoors)** | `nax` id 275, **map 533 (inside)** | ✅ |
| `.raid tele scholo` | substring → id 4, **map 0** | `scholo_kirtonos` id 428, map 289 | ✅ |
| `.raid tele sm` | substring → **`RagefireChasm`** id 46, map 1 | `smEntry` id 470 | ✅ |
| `.raid tele zf` | **no game_tele row on map 209** — reports "not found" | unchanged (needs a DB row) | ❌ |
| `.raid tele aq20` | `aq20` id 194, map 509 | already correct | — |
| `.raid tele aq40` | `aq40` id 192, map 531 | already correct | — |
| `.raid tele bwl` / `zg` | ids 178 / 166, correct maps | already correct | — |
| `.raid tele scarlet` | `ScarletCitadel` id 500, **map 45** | already correct — this is the Turtle-WoW 10-Man Scarlet Citadel raid | — |

`.tele group` is **`SEC_DEVELOPER`** and requires a **selected, grouped player** — you cannot run it solo. The remaining Zul'Farrak gap needs a `game_tele` row, which is a database write and deliberately out of scope here; use `.tele 209 <x> <y> <z>` at the pyramid meanwhile.

**Not bugs — leave alone:** `.raid tele karazhan`, `.raid tele crypt` and `.raid tele vault` resolve to `game_tele` rows on **map 0**. That is correct, not a mis-resolution: `game_tele` has *no* row on their interior maps (**35** Stormwind Vault / **532** + **814** Karazhan / **800** Karazhan Crypt), so the outdoor row is the only teleport that exists and is serving as the instance entrance. *(The interior ids `810 / 812 / 813` given here previously are the old wrong numbers — `812` is not a map at all, and `813` is `Winter Veil Vale`.)*

⚠️ **But §8's map numbers for the custom instances do not match the database.** Verified `game_tele` maps: Scarlet Citadel = **45** (not 811), Karazhan/Crypt/Vault = 0 (entrances; their interior maps 532/814, 800 and 35 have no tele row), `sanctumraid` = **807** (not 814), `crescentgrove` = 802 (not 805), `hateforgeinstance` = 808 (not 806), `gilneascity` = 815 (not 807), `TimbermawHold` = 1. **§8 also had Timbermaw and Gilneas swapped with each other.** This note used to be the *only* record of the error, ~500 lines below the table it corrects; the §8 table now carries the correction inline as well, with this note as the authority. §8 also omits seven working aliases the script implements — `molten`, `blackwing`, `zulgurub`, `naxxramas`, `onyxia`, `st`, `zf` — all listed there.

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
> `run/` working directory**, so that is the only path Eluna ever loads.
>
> ⚠️ **Superseded, kept visible:** this block previously read "**There are two
> copies of this script in the repository, and only one is live.** … A stale
> **v1.1.0** copy also exists at `lua_scripts/raid_automation.lua` (repo root) and
> is **not** loaded — v1.1.0 lacks `tank3` / `tank4` / `tanks` / `marks` /
> `ccmark` / `resume` / `gather` / `avoid` / `roll` entirely." **The "two copies
> exist on disk" premise is no longer true**: `find . -name raid_automation.lua`
> returns a single hit. The v1.1.0 file still lives in **git HEAD**
> (`git show HEAD:lua_scripts/raid_automation.lua` → 700 lines,
> `SCRIPT_VERSION = "1.1.0"` at `:18`) but has been deleted from the working tree
> **without being committed** — `git status --short` shows
> ` D lua_scripts/raid_automation.lua`.
>
> **So the hazard has moved, it has not gone away.** The advice is still correct
> (audit `run/`), and the failure mode it warns about is still real — but the
> mechanism is now git, not a second file on disk: **a `git checkout` of that
> path, a branch switch, or a fresh clone of HEAD will resurrect the stale v1.1.0
> file.** Do not do that, and if it reappears, delete it before auditing anything.
> The v1.1.0-versus-live version gap it would create is exactly what produces a
> large set of false "this command does not exist" findings, which is what
> happened during this audit.

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

| Command | Does |
| :--- | :--- |
| `.raid roles` (alias `.raid setup`) | Roster census (class counts + feasibility, plus the tank-capable name list), then splits **formations by role** via one broadcast per role, and focus-locks the DPS. **Sends no talent commands.** |
| `.raid tank2` / `tank3` / `tank4` / `tank5` `[<name>]` | Promote a bot to that tank slot. All four land on the **same** branch — `slot = tonumber(sub:sub(5))` parses any trailing digit — so `tank2` and `tank5` work exactly as documented for `tank3`/`tank4`. Without a name it promotes your **current target**, and with neither it errors and changes nothing. |
| `.raid tanks <n> [names…]` | Count form for any n (1–8). Names optional, but the roster-order fallback is never silent. |
| `.raid marks <tank…>` | One mark icon per named tank, in priority order: 1st `skull`, 2nd `cross`, 3rd `square`, then `diamond`/`triangle`/`star`/`circle`. |
| `.raid marks <tank> <icon>` | Set one tank's icon explicitly. ⚠️ **`moon` is deliberately refused** as an icon name: `MARK_ICON_ORDER` excludes it because index 4 is the engine's own CC marker (`strategy/values/TargetValue.cpp:57-59`) and a tank marking it makes the whole raid skip that mob. Use `.raid ccmark`. |
| `.raid unmark [tank]` | `co -mark rti` on one bot, or on the whole roster. |
| `.raid ccmark` (alias `.raid moon`) | **Moon (icon 4)** on your current target — the crowd-control marker. |
| `.raid skull` (aliases `.raid mark`, `.raid focus`) | Skull (icon 7) on your current target. The dispatch is `elseif sub == "skull" or sub == "focus" or sub == "mark"`, so all three are the same command. |
| `.raid clearmarks` | Clears the star / circle / moon / skull icons. |
| `.raid ready [best\|epic]` (alias `.raid auto`) | `roles` + `gear` + `attune`. ⚠️ **This one takes only two words**, and unlike `.raid gear` it does **not** refuse an unknown one — `local quality = rest:lower() == "epic" and "epic" or "best"` coerces *anything* else to `best`. `.raid ready purple` is `.raid ready best`, silently. Use `.raid gear purple` if you meant a specific tier. |
| `.raid attune` · `.raid keys` | Completes the 9 attunement quests, sets Argent Dawn rep, adds the 13 master key items. |
| `.raid gear [quality]` (alias `.raid prep`) | `bot gear * <q>`, enchants, train, prep, consumes, food, potions, regs, ammo, then `repairitems` + `replenish`. Accepts **10 values**: `best`, `green`/`uncommon`, `blue`/`rare`, `purple`/`epic`, `partial`, `upgrade`, `sync` (10 strings, 7 tiers — the synonyms share a tier). An unrecognised word is **refused, with the full list printed**, not coerced. ⚠️ The script's *own* `.raid gear` help line shows only `<q>` and defers to a separate `Quality:` line beneath it; the one-line `.raid ready` summary shows only `best`/`epic`. Neither is the whole set — this table is. |
| `.raid buff` · `.raid wbuff` · `.raid worldbuff` | **Sends nothing** — reports how buffing actually works. See below. |
| `.raid summon` (alias `.raid rally`) | `bot summon *`. |
| `.raid start [mode]` | `dc bosses` → `dc pull <mode>` → `dc on` → `dc status`. Requires a dungeon/raid map. Default mode `dynamic`. |
| `.raid off` (alias `.raid stop`) | `dc off`. |
| `.raid pause` · `.raid resume` | `dc pause pause` / `dc pause resume` — never the bare toggle. |
| `.raid pull <off\|on\|dynamic>` | Forwards to `dc pull <mode>`. ⚠️ A **bare** `.raid pull` **sets `dynamic`**, and any unrecognised word is **clamped to `dynamic`** before forwarding — the engine's `(current+1)%3` bare toggle is never reached through this wrapper. |
| `.raid go <boss>` · `.raid skip` · `.raid status` · `.raid bosses` · `.raid config` · `.raid spectate [follow <name>\|next\|prev\|list]` | Forwards to the matching `.dc` subcommand. |
| `.raid tele <alias>` | Group teleport. See the alias audit below. |
| `.raid unbind` (alias `.raid resetlock`) | `.instance groupunbind all`. ⚠️ **`.raid reset` does not exist** — it was renamed in v1.2.0, so any earlier reference to `.raid reset` in this handbook family is wrong. |
| `.raid loot` / `noloot` · `rest` / `norest` · `open` · `aoe` / `noaoe` (`single`) · `gather` / `nogather` · `avoid` / `noavoid` · `roll` / `noroll` · `unfocus` | One broadcast each: `nc +loot`/`nc -loot`, `nc +food`+`nc +potions`/`nc -food`+`nc -potions`, `u` (⚠️ **inert** — see [§13.9 Traps](#139--traps--strings-that-look-like-they-worked-and-did-not)), `co +aoe`/`co -aoe`, `nc +gather`/`nc -gather`, `co +avoid aoe`/`co -avoid aoe`, `nc +roll`/`nc -roll`, `co -focus rti targets`. |

**Aliases the old table never listed** — each one lands on the same branch as the
command it is an alias of, so they are the same command in every respect:
`.raid setup` = `roles`, `.raid auto` = `ready`, `.raid prep` = `gear`,
`.raid rally` = `summon`, `.raid keys` = `attune`, `.raid moon` = `ccmark`,
`.raid mark` = `skull`, `.raid resetlock` = `unbind`, `.raid stop` = `off`,
`.raid noaoe` = `single`.

Things to know:

- **`.raid roles` no longer assigns specs, and that is deliberate.** An earlier
  version sent `talents do <spec>` + `reset ai` to every bot. ⚠️ **The old
  justification — quoted here verbatim because it is the belief this correction
  exists to kill: "`.bot group` already applies a role-matched spec and *varies
  within the role*; forcing one spec per class flattened it." That is false.**
  `.bot group` applies **no** role-matched spec at all — it never forwards the
  role it rolls (`PlayerbotMgr.cpp:2825` → `:2831` → `:2855`, no `role=`), so
  `AutoSelectTalents` gets `BOT_ROLE_NONE` (`:2680`) and picks a random premade
  spec (`ChangeTalentsAction::getPremadePaths:170`, `PickPremadePath:230`). The
  cross-doc claim it linked to, [`.bot group` assigns the spec too](../02_playerbot_and_raid_automation.md),
  carries the same correction.
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
  for the reason this handbook used to give.
  ⚠️ **Superseded, kept visible:** "Eluna here has no timer API, so the script
  cannot wait for commands to be parsed before `dc on` runs. Bots process them on
  their next AI tick — leave a second or two." **Both halves of that are wrong.**
  Our Eluna flavour is `ELUNA_VMANGOS` (`src/game/CMakeLists.txt:681`) and it
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

  **So "leave a second or two" was the wrong remedy, and the real constraint is
  different.** `.raid start` dispatches `dc bosses` → `dc pull <mode>` → `dc on` →
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
  shield*, restoration gets *water shield*. An earlier version of this script
  carried a per-class aura table that sent `water shield` to **every** shaman,
  overwriting that correct per-spec choice. Removing the table is the fix, not an
  omission.

#### Composition targets

An exact mirror of `LfgAction::AllowedClassRoleNr`
(`InviteToGroupAction.cpp:89`, per-role caps at `:105-121`) — **including the
20-man tank count of 2**. An
earlier version of this document claimed the script raised it to 3; that is no
longer true, and the table now matches what `.bot group` itself targets, so the
census and reality cannot disagree.

| size | tanks | healers | dps |
| :--- | :--- | :--- | :--- |
| 5 | 1 | 1 | 3 |
| 10 | 2 | 3 | 5 |
| 20 | 2 | 5 | 13 |
| 25 | 3 | 7 | 15 |
| 40 | 4 | 16 | 20 |

⚠️ **These numbers shape the CLASS MIX. They do not shape the SPECS.**

**What is real.** The **class caps** in the table are the engine's own
`AllowedClassRoleNr` (`InviteToGroupAction.cpp:105-121`) and
`HandleGroup` decrements them as it goes (`PlayerbotMgr.cpp:2870-2873`), so they
genuinely govern which classes get drawn. **Faction is respected**:
`HandleGroup` skips `CLASS_PALADIN` on HORDE and `CLASS_SHAMAN` on ALLIANCE
(`PlayerbotMgr.cpp:2833-2842`), so a Paladin is never given a slot in a horde raid
and a Shaman never in an alliance one; off-faction bots are left untouched and
reported by name, because `.bot group` will not create them.

**What is not real — the old "assignment rules" paragraph, kept visible:**
"Assignment rules: tank slots are filled first from tank-capable bots (Warrior,
Paladin, Druid) because they are the scarce resource; the **third** slot prefers a
Warrior; heal slots come from Priest, Paladin, Shaman or Druid; everything else is
DPS." **There is no slot-filling pass, no ordering, and no "prefers a Warrior".**
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

> ⚠️ An earlier version of this paragraph added that `.raid roles` re-issues
> `co +cc` explicitly. **It does not.** `ApplyRaidSetup` sends exactly five
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
bots](#%E2%9A%A8-read-this-first-how-to-address-a-raid-of-bots) at the top of
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

| Command | Broadcast | Gameplay Purpose |
| :--- | :--- | :--- |
| **`.raid loot`** · **`.raid noloot`** | `.bot r nc +loot` / `nc -loot` | Orders every bot to add (or drop) the loot strategy so it vacuums — or walks past — nearby enemy corpses. |
| **`.raid rest`** (or `.raid drink`/`.raid food`) · **`.raid norest`** | `.bot r nc +food` + `nc +potions` / `nc -food` + `nc -potions` | Orders bots to eat and drink back to full health and mana — or to stop. `potions` is a separate strategy from `food`, so the off form clears both. |
| **`.raid open`** (or `.raid use`/`.raid breach`) | `.bot r u` | ⚠️ **This row's old claim was wrong and the engine is the reason.** `u` and `use` both create a trigger named `use` (`ChatTriggerContext.h:34-35`) and the module's only node of that shape is named `u` (`ChatCommandHandlerStrategy.cpp:150`), which nothing maps to. There is **no TriggerNode named `use` anywhere**, so both words are dropped in silence at `ExternalEventHelper.h:84`. `UseAction` is reachable only from `LootAction.cpp:230` and `RpgSubActions.h:292`, never from a chat command. v1.3.0 keeps sending the identical `u` string (removing it would be a behaviour change) and the help says plainly that it is inert. **There is no working chat spelling of "use this item" on this build** — see [§13.9 Traps](#139--traps--strings-that-look-like-they-worked-and-did-not). |
| **`.raid aoe`** · **`.raid noaoe`** (alias **`.raid single`**) | `.bot r co +aoe` / `co -aoe` | Prioritises AoE damage for everyone who can use it, or removes the AoE strategy and leaves single-target behaviour. |
| **`.raid gather`** · **`.raid nogather`** | `.bot r nc +gather` / `nc -gather` | Pick up herbs and ore while walking — or not, for a strict run. |
| **`.raid avoid`** · **`.raid noavoid`** | `.bot r co +avoid aoe` / `co -avoid aoe` | Step out of AoE. This one saves lives; leave it on. |
| **`.raid roll`** · **`.raid noroll`** | `.bot r nc +roll` / `nc -roll` | Roll on loot, or ignore loot rolls entirely. |
| **`.raid unfocus`** | `.bot r co -focus rti targets` | **New in v1.3.0.** Releases the DPS focus lock so they may be pulled off the icon. The lock is set by `.raid roles`, and before v1.3.0 **nothing could turn it off**. |
| **`.raid skull`** (aliases `.raid focus`, `.raid mark`) | `Group:SetTargetIcon(7, target)` | Skull icon on your current target — what the DPS focus lock follows. |
| **`.raid ccmark`** (alias `.raid moon`) | `Group:SetTargetIcon(4, target)` | **Moon** icon on your current target. The raid deliberately *avoids* moon targets, so use it for CC, not priority. |
| **`.raid marks <tank…>`** | whisper per named tank | One icon per tank — see [Tank marking](#tank-marking--one-icon-per-tank). |
| **`.raid follow`** | `.bot r follow` | **New in v1.3.0.** Bots *walk* to you and hold — a soft regroup, movement not a teleport. Use `.raid summon` for the hard version. |
| **`.raid summon`** | `.bot summon *` | Teleports all group bots to your coordinates. |
| **`.raid unbind`** (alias `.raid resetlock`) | `.instance groupunbind all` | Clears instance ID lockouts for every player and bot in the group. |

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
for itself. An earlier version of this script invented a per-class spec table to
assign formations from; that was both unnecessary and less accurate.
⚠️ **The old justification here was "which is correct because `.bot group` already
built the kit from the real talent tab."** The filtering mechanism is correct, but
that reason is not: `.bot group` does **not** build a kit from a real talent tab
for this purpose — it hands `AutoSelectTalents` a `BOT_ROLE_NONE` and lets it pick
a random premade spec (`PlayerbotMgr.cpp:2680`,
`ChangeTalentsAction::getPremadePaths:170`). So the filters will correctly report
*whatever spec each bot ended up with* — including "not a tank", for most of a
freshly assembled roster. Run `.raid check` first, `.raid tanks <n>` second.

### Quota Allocation by Group Size

The census **rounds up** to the next row, so a scaled raid that fields fewer
players than its bracket still reports that bracket's shape. These are the same
**class-shape** numbers `.bot group` targets, and this is a **feasibility check,
not a measurement** — the script cannot read a bot's talents from Eluna.

| Bracket | Group Size | Target Tanks | Target Healers | Target DPS |
| :---: | :---: | :---: | :---: | :---: |
| **5-Man** | ≤ 5 | 1 Tank | 1 Healer | 3 DPS |
| **10-Man** | 6–10 | 2 Tanks | 3 Healers | 5 DPS |
| **20-Man** | 11–20 | 2 Tanks | 5 Healers | 13 DPS |
| **25-Man** | 21–25 | 3 Tanks | 7 Healers | 15 DPS |
| **40-Man** | 26–40 | 4 Tanks | 16 Healers | 20 DPS |

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
`Stances.cpp`). So `/w BotName stance defensive` is **not a valid command** — an
earlier version of this document recommended it. `stance tank` is a valid
*positioning* stance and has nothing to do with Defensive stance.

### Markers

| Icon | Index | Set by | Command |
| :--- | :---: | :--- | :--- |
| Star (MT) | 0 | first tank slot | automatic |
| Circle (assist / OT) | 1 | second tank slot | automatic |
| Moon (**CC**) | 4 | your current target | `.raid ccmark` |
| Skull | 7 | your current target | `.raid skull` |
| Cross / Square / Diamond / Triangle | 6 / 5 / 2 / 3 | a tank with `mark rti` | `.raid marks <tank> <icon>` |

> ⚠️ **Moon is icon 4, and it is the engine's own CC marker** — hard-coded in
> `FindNonCcTargetStrategy::IsCcTarget` (`strategy/values/TargetValue.cpp:57-59`) and the
> default of the `rti cc` value. `dps aoe` skips whatever carries it. An earlier
> version of this document said `.raid ccmark` sets **Triangle (icon 3)**; that
> was wrong, and it mattered, because a triangle-marked mob is a *priority kill*
> target while a moon-marked mob is one the raid deliberately **walks away from**.
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

| Position | 1st | 2nd | 3rd | 4th | 5th | 6th | 7th |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Icon | `skull` | `cross` | `square` | `diamond` | `triangle` | `star` | `circle` |

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
⚠️ An earlier version of this section said "there is no chat command that asks a
bot *are you already a tank?*". **That was false as of v1.3.0.** `.raid check`
(`bot r all ?` → `ai->PrintStrategies(requester, BOT_STATE_ALL)`,
`ChangeStrategyAction.cpp:150`) makes **every bot print its live installed
strategy set**, all four engines — and the spec strategy is in that list, so
"is this bot already a tank" is now answerable. The *script* still cannot parse
chat it does not read back, so it still takes names rather than guessing; but you
no longer have to guess either. Run `.raid check` once, after `.raid group`, and
you can see it. A bare `.raid marks` enables `mark rti` on `@tank` and
**warns that this is the thrash case**.

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

| Token Alias | Destination Instance | Token Alias | Destination Instance |
| :--- | :--- | :--- | :--- |
| `.raid tele mc` | Molten Core (40-Man) | `.raid tele ubrs` | Upper Blackrock Spire (10-Man) |
| `.raid tele bwl` | Blackwing Lair (40-Man) | `.raid tele lbrs` | Lower Blackrock Spire |
| `.raid tele ony` | Onyxia's Lair (40-Man) | `.raid tele brd` | Blackrock Depths |
| `.raid tele zg` | Zul'Gurub (20-Man) | `.raid tele strat` | Stratholme |
| `.raid tele aq20` | Ruins of Ahn'Qiraj (20-Man) | `.raid tele scholo` | Scholomance |
| `.raid tele aq40` | Temple of Ahn'Qiraj (40-Man) | `.raid tele dm` | Dire Maul (North) |
| `.raid tele naxx` | Naxxramas (40-Man) | `.raid tele sm` | Scarlet Monastery |

### Turtle-WoW Custom Raids & Dungeons

⚠️ **The "Map ID" column below is SUPERSEDED — it is the v1.3.0-era table and it is
known-wrong.** It is left in place so the change is visible, and the verified
values are given in the footnote immediately under it. The mistake was previously
documented only in [§5.1](#51-complete-dc-command-reference-engine-verified), some
500 lines away; it is called out here because this is where a reader looks.

| Token Alias | Destination Instance | Bracket | Map ID *(superseded — see footnote)* | Location |
| :--- | :--- | :---: | :---: | :--- |
| `.raid tele scarlet` | **Scarlet Citadel** | 10-Man Raid | ~~811~~ → **45** | Tirisfal Glades |
| `.raid tele karazhan` | **Tower of Karazhan** | 10/40-Man Raid | ~~812~~ → **0** *(entrance)* | Deadwind Pass |
| `.raid tele timbermaw` | **Timbermaw Hold** | 20-Man Raid | ~~815~~ → **1** | Felwood |
| `.raid tele sanctum` | **Emerald Sanctum** | 40-Man Raid | ~~814~~ → **807** | Mount Hyjal |
| `.raid tele crypt` | **Karazhan Crypts** | 58–60 Dungeon | ~~813~~ → **0** *(entrance)* | Deadwind Pass |
| `.raid tele hateforge` | **Hateforge Quarry** | 50–60 Dungeon | ~~806~~ → **808** | Burning Steppes |
| `.raid tele crescent` | **Crescent Grove** | 32–38 Dungeon | ~~805~~ → **802** | Ashenvale |
| `.raid tele vault` | **Stormwind Vault** | Level 60 Dungeon | ~~810~~ → **0** *(entrance)* | Stormwind City |
| `.raid tele gilneas` | **Gilneas City** | 42–48 Dungeon | ~~807~~ → **815** | Gilneas |

> ⚠️ **Map ID footnote — verified against `tw_world.game_tele`, and stated in full
> in [§5.1](#51-complete-dc-command-reference-engine-verified).** Every row of the
> old column was wrong, and **Timbermaw Hold and Gilneas City were swapped** with
> each other as well: the table gave Timbermaw `815` and Gilneas `807`, while the
> database says Timbermaw is map `1` and Gilneas is `815`. Verified values:
> Scarlet Citadel = **45**, `sanctumraid` = **807**, `crescentgrove` = **802**,
> `hateforgeinstance` = **808**, `gilneascity` = **815**, `TimbermawHold` = **1**,
> and Karazhan / Crypt / Vault = **0**, which is correct and not a bug — their
> interior maps **35** (Stormwind Vault), **532** + **814** (Karazhan) and **800**
> (Karazhan Crypt) have **no** `game_tele` row at all, so the outdoor entrance
> row is the only teleport that exists. *(The ids `810 / 812 / 813` this block
> previously gave are the old wrong numbers: `812` is not a map, and `813` is
> `Winter Veil Vale`.)*
>
> One more inconsistency in this table, left as-is but flagged: the **Bracket**
> column says `10-Man Raid` for Scarlet Citadel, while
> [§12.3](#123-scarlet-citadel-map-45-40-man-raid-fortress) describes map `45` as
> a **40-Man Raid Fortress** with seven scripted bosses and a
> `SealedEncounterRegistry` that waits for all 40 members. The encounter reference
> is the more detailed and more recently verified of the two, so read `40-Man`
> there. This bracket discrepancy was not part of the Map ID audit and has not
> been independently re-audited.

> 📋 **Seven working aliases are missing from this table.** The script's
> `TELEPORT_PRESETS` table (`raid_automation.lua:786-816`) implements a long-form
> synonym for most of these, and only `zf` was ever mentioned in this handbook:
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

In `modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp` (line **3178**; the factory is constructed at :3180 — an earlier revision of this section cited 3123, which no longer points at the check):

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

In `PlayerbotMgr.cpp` (line **3252**; an earlier revision cited 3197):

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

⚠️ **This paragraph previously said "`.bot group` already assigns that spec when it
creates a bot for a tank slot — `HandleCreate` calls `AutoSelectTalents(newBot,
out, role)` with the role it picked."** The function reference was right; **the
role is `BOT_ROLE_NONE`, and the line is `:2680`, not `:2684`.** `HandleGroup`
rolls a role at `PlayerbotMgr.cpp:2825`, uses it only to pick a class (`:2831`),
and forwards **no `role=`** in the create string (`:2855`), so `HandleCreate` keeps
`BOT_ROLE_NONE` (`:2536`, written only at `:2562-2563`) and
`AutoSelectTalents(newBot, &out, role)` at **`:2680`** receives that.
`ChangeTalentsAction::getPremadePaths` skips its role filter when the role is
`BOT_ROLE_NONE` (`:170`) and `PickPremadePath` (`:230`) picks a **random** premade
spec for the class. **A tank-role spec is therefore not something `.bot group`
gives anyone.**

`.raid roles` does **not** assign it either, and no longer claims to. The one
command that does is **`.raid tanks <n>`** — `talents do protection` followed by
`reset ai` (`raid_automation.lua:1965-1972`). If your tank is not actually
protection/feral-specced, promote one with `.raid tanks <n>` (or check first with
`.raid check`).

### 🗄️ ORPHANED / HISTORICAL — "Why use Greater Blessing of Kings over multiple blessings?"

> 🚨 **This FAQ is ORPHANED. The code it rationalises no longer exists, and this
> handbook contradicts it four other times.** Read the banner, then skip the
> paragraph.
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
>   what got the table removed in the first place.
> - **The one-blessing-per-caster rule below is still true game rules**, and the
>   reasoning about *why* a single shared blessing would be the right choice for a
>   hand-rolled table is sound. It is simply **moot**: no such table exists, and
>   this is not the layer where aura selection belongs.
>
> If you ever find yourself restoring this FAQ's table, that is a regression. Fix
> the spec strategy instead.

*Original FAQ text, kept for history:* In Classic WoW, a player can only receive
**one blessing per Paladin caster**. When an Eluna script iterates through multiple
Greater Blessings from the same caster, each blessing overwrites the previous one.
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
micromanagement**. v1.4.0 is what that actually looks like: `.raid` is no longer
a thin convenience layer over `.dc` and `.bot`, it is a **five-layer operator
facade** with 12 + 27 + 19 + 4 + **6** commands, an optional `@filter` prefix on
every order that talks to bots, and a built-in trap register.

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

> ⚠️ **Superseded counts, kept visible.** An earlier revision of this section said
> "**12 + 27 + 19 + 4 + 4**" and gave the total as **66 verbs**. **The total is now
> 68**, and only the last layer moved: v1.4.0 added two PASS commands,
> **`.raid preview`** and **`.raid pending`** (`raid_automation.lua:1262` and
> `:1267`). BATTLE 12, ROSTER 27, TOGGLES 19 and MOVE 4 are all still correct —
> they match the `COMMANDS` table (Lua `:1170-1268`) counted by `LayerCount()`
> (`:2297-2311`), which is also what the bare `.raid` index prints.

Two functional gaps were closed in v1.3.0, and they were the two that
actually hurt:

- **`.raid group <size>`** now wraps the missing **step 1**. v1.2.0's own
  documentation said `.bot group size=N` came first, and then never wrapped it —
  so the documented order was not executable from inside `.raid`. It is now
  `.raid group size=20`, and it is the same string. ⚠️ Note what "step 1" means
  here: it **fills the roster**, it does not assign specs. The spec step is
  `.raid tanks <n>`, and the verify step is `.raid check`.
- **Every toggle has an off form.** v1.2.0 could turn `loot` / `rest` / `aoe` /
  `gather` / `avoid` / `roll` on and nothing could turn them off again. That is
  the entire reason the TOGGLES table exists in the script; in v1.2.0 it was a
  bare chain of `elseif sub ==` branches, and the inverse names were simply
  absent from it.

> **On the leading dot.** Everything below is written the way you type it, **with
> the leading dot**: `.raid all @frost co +aoe`, `.raid pull dynamic`. The
> `.raid bot` / `.raid all` / `.raid one` forms show the **engine string** they
> produce without the dot, because that is what the script passes to
> `RunCommand` — see the convention note at the top of
> [§6](#6-in-dungeon-recovery-doors-aoe--target-focus).

### 13.1 What changed, and why the facade exists

Three v1.2.0 behaviours were actively misleading, and all three are now fixed:

| v1.2.0 behaviour | v1.3.0 |
| :--- | :--- |
| `.raid` had ~25 verbs and no diagnostics at all | **66 verbs** across five layers — 12 + 27 + 19 + 4 + **4** — including live per-bot strategy, gear score, spell list, position and avoid-list queries |
| One-way toggles | Every toggle has an explicit off form; `unfocus` is genuinely new |
| An unrecognised `.raid <word>` dumped a 40-line help screen | A one-line "no such command" plus the **closest** matching command, as "Did you mean X?" when it is within edit distance 3 and "Closest: X" otherwise |

The v1.2.0 help screen is still reachable as `.raid help full`. Nothing was
deleted — the druid coin-flip note, the cosmetic-marker note, the Naxx routing
warning and the MT-election warning all live in there.

#### v1.3.0 → v1.4.0

⚠️ This table was **missing entirely** — the section jumped straight from
`v1.2.0 → v1.3.0` and never covered the current release, so a reader auditing the
live script had no account of what changed. All four rows are verified against
`run/lua_scripts/raid_automation.lua` (**v1.4.0**).

| v1.3.0 behaviour | v1.4.0 |
| :--- | :--- |
| **PASS had 4 verbs** — `all`, `one`, `dc`, `bot`. A typo in a raw verb was invisible, because the engine ignores unrecognised commands **silently**, so a mis-typed order looks exactly like a successful one. | **PASS has 6.** Added **`.raid preview <sub> [args]`** (`:2809-2851`) — a true dry run. It sets `DRY = true`, re-invokes the real `DispatchRaidCommand` under `pcall`, routes `Emit` into a buffer instead of the server, then prints the exact numbered command list it **would** have sent and sends nothing. It refuses to nest (`Refusing to nest a preview inside a preview`) and reports `would send nothing` when the branch really is a no-op. Added **`.raid pending`** (`:2858-2861`) — prints `Step queue: N pending (cap 64, T ticks since load)`, so an operator can tell a long batch from a stuck one. |
| **Multi-step commands were back-to-back whispers.** `.raid attune` and `.raid tanks` fired every step immediately in one burst, so an error between two steps was invisible and left the raid half-applied. | **A bounded deferred-step queue**, drained by `MAP_EVENT_ON_UPDATE` (event `23`) → `OnMapTick` (`:3702`, implementation `:1461-1487`): **one step per map tick**, `STEPS_MAX = 64` (`:1429`) and `STEP_MAX_AGE_TICKS = 200` (`:1430`). Growth past the cap is **refused with a message**, a batch that stalls past 200 ticks is **aborted and reported**, and every step is `pcall`ed so one bad step aborts the rest instead of continuing half-applied. `.raid attune` (`:1668`) and `.raid promote`/`.raid tanks` (`:1974`) both enqueue into it. |
| **Promotion sent `talents do <spec>` and `reset ai` as two back-to-back whispers**, and could interleave with anything else. | **ORDERED promotion.** The same two commands are now two consecutive queue steps per bot (`:1955-1972`). The reason is precise and the failure was silent: `talents do` without a following `reset ai` leaves a bot **carrying new talents with a strategy set rebuilt from the OLD talent tab** — exactly the half-state `reset ai` exists to prevent. Queuing them in order makes an error between them visible at the moment it happens, instead of surfacing much later as "this bot behaves like the wrong spec". The script now prints `N steps queued - respec and rebuild stay paired`, and if the queue refuses the batch it says so explicitly: `Promotion queue refused - NO talent command was sent.` |
| **Runtime errors in a command were invisible to the operator.** The dispatch body was registered directly with the event, with **zero `pcall`**, so a mid-command failure only reached `ELUNA_LOG_ERROR` in a log file the operator is not reading — chat just stopped with no explanation. | **`OnRaidCommand` wraps the dispatch in `pcall`** (`:3639-3654`). On failure it reports in the operator's own chat: `Command failed mid-way: <err>`, then `Some commands may already have been sent. Use .raid pending and .raid status to see where it got to.` The success path is untouched. Combined with `.raid pending`, a partially-applied command is now both **visible** and **diagnosable**. |

### 13.2 ⚔️ BATTLE — run control

All of these forward to mod-dungeon-clear, which elects its own leader tank. The
script never addresses a tank for DC.

| Command | Engine string | Note |
| :--- | :--- | :--- |
| `.raid start [<off\|on\|dynamic>]` | `dc bosses` → `dc pull <m>` → `dc on` → `dc status` | In that order, and it **matters**. `dc bosses` is **one of only three verbs with no authorization gate** (see the gate list below) and works while DC is off; an empty anchor list is what `dc on` will refuse on. `dc pull` is sent **before** `dc on` because the preference survives the disabled window and `dc on` applies it at run start. **`dc on` is issued exactly once, ever** — re-running it wipes skip and progress state and un-skips every boss. |
| `.raid stop` | `dc off` | v1.3.0 alias of `off`; same string. `stop` is the word an operator reaches for first, `off` is the word DC itself uses. |
| `.raid off` | `dc off` | Tears down the run. |
| `.raid pause` | `dc pause pause` | **Explicit intent, never the bare toggle.** The bare form is a stateless toggle that races DC's own auto-pause sites, so a pause landing just after a door or wait-at-boss auto-pause **resumes** instead (`DungeonClearChatActions.cpp:1096-1107`). Boss progress is preserved. |
| `.raid resume` | `dc pause resume` | The other half of the same reasoning. |
| `.raid go <boss>` | `dc go <boss>` | Also **clears any pause** as part of its intent — `DcRun::Of(context).paused = false;` at `DungeonClearChatActions.cpp:1046` (an earlier revision cited `:1044-1046`, which no longer brackets it) — so `go` is itself a resume. Matching is exact numeric creature entry first, then a case-insensitive substring on the boss name. |
| `.raid skip` | `dc skip` | Abandons the current anchor or due event. |
| `.raid pull <off\|on\|dynamic>` | `dc pull <mode>` | Bare **sets `dynamic`**; anything unrecognised is **clamped to `dynamic`** with a correction printed. The engine's `(current+1)%3` bare toggle is unreachable from here. |
| `.raid status` | `dc status` | The leader's own view. |
| `.raid bosses` | `dc bosses` | The real pre-flight — works before `dc on` has ever been issued. |
| `.raid config` | `dc config` | **Read-only.** Dumps every DC tunable as the AI reads it this tick, so it confirms a `mod_dungeon_clear.conf` edit without `.reload config`. There is no chat or command path that writes them. |
| `.raid spectate [follow <n>\|next\|prev\|list]` | `dc spectate <rest>` | Camera on **you**, not the tank, so it works from outside the party. ⚠️ Do **not** spectate from a raid member mid-run: a visible player in the instance pulls aggro and invalidates the clear. |

> **On the leading dot, one more time, because it is the single most common
> mistake with `go`:** `.raid go onyxia`, not `.raid go` followed by a chat
> message. The argument is parsed from the same line.

#### Which verbs have no authorization gate

⚠️ **Superseded claim, kept visible:** this section used to say `dc bosses` is
"**the only verb with no authorization gate**". **That is false.** Of the **nine**
`Execute` handlers in `DungeonClearChatActions.cpp`, **six** call
`IsAuthorized(bot, event)` and **three** do not:

| Handler | `Execute` at | Authorization gate |
| :--- | :---: | :---: |
| `DcOnAction` | `:223` | ✅ `:225` |
| `DcOffAction` | `:355` | ✅ `:357` |
| `DcSkipAction` | `:394` | ✅ `:396` |
| `DcStatusAction` | `:480` | ❌ **none** |
| `DcBossesAction` | `:517` | ❌ **none** |
| `DcGoAction` | `:896` | ✅ `:898` |
| `DcPauseAction` | `:1075` | ✅ `:1077` |
| `DcResumeOnDoorOpenedAction` | `:1159` | ❌ **none** |
| `DcPullAction` | `:1185` | ✅ `:1187` |

So the correct statement is: **`dc bosses`, `dc status` and the
door-reopened auto-resume are the only three verbs with no authorization gate.**
(`DcResumeOnDoorOpenedAction` is not a chat verb at all — it is the engine's own
auto-resume when a door opens — so among the verbs an operator can *type*, it is
`dc bosses` and `dc status`.)

**The advice that rested on the old claim is unchanged and still worth following:**
run `dc bosses` as a pre-flight. It works before `dc on` has ever been issued, and
an empty anchor list is exactly what `dc on` will refuse on. The exclusivity was
never what made it useful.

### 13.3 📋 ROSTER — who is here, and what they can do

#### The commands

| Command | Engine string | Note |
| :--- | :--- | :--- |
| `.raid group <size>` | `bot group size=<n>` | **STEP 1 — the roster filler, not the spec step.** It loops until the group is full, picking a ROLE at random then a class that can fill it. ⚠️ **It does NOT give every bot a role-matched spec** — an earlier version of this row claimed it did. `HandleGroup` never forwards `role=`, so `AutoSelectTalents` receives `BOT_ROLE_NONE` and picks a random premade spec for the class (`PlayerbotMgr.cpp:2825`/`:2831`/`:2855` → `:2680` → `ChangeTalentsAction::getPremadePaths:170`, `PickPremadePath:230`). **Follow it with `.raid check` and `.raid tanks <n>`, which is the only re-spec path.** Waits for completion; it creates bots one at a time. Above 40 the script refuses outright, because the engine parses `size=` with `stoi` into a `uint8` and a larger number **wraps** instead of failing. |
| `.raid roles` · `.raid setup` | 5 broadcasts | Census, formations by role, `co +focus rti targets` on `@dps`. Sends **no** talent commands. |
| `.raid ready [<quality>]` · `.raid auto` | `roles` + `gear` + `attune` | ⚠️ **Takes only `best` and `epic`**, and unlike `gear` it silently coerces anything else to `best`. |
| `.raid gear [<quality>]` · `.raid prep` | `bot gear * <q>` + enchants/train/prep/consumes/food/potions/regs/ammo + `repairitems`/`replenish` | 10 accepted qualities — see [§5.1](#51-complete-dc-command-reference-engine-verified). |
| `.raid attune` · `.raid keys` | quest add+complete, `additem` | |
| `.raid tanks <n> [names…]` | `talents do <spec>` + `reset ai` per bot | The **only** re-spec path. |
| `.raid tank2`/`tank3`/`tank4`/`tank5` `[<name>]` | same, to a targeted or named bot | `slot = tonumber(sub:sub(5))` — the same branch parses any digit. |
| `.raid marks <tank…>` | `rti <icon>` + `co +mark rti` per tank | One icon per tank, in priority order. |
| `.raid marks <tank> <icon>` | same, one tank | `moon` is **refused** — see the icon note in [§5.1](#51-complete-dc-command-reference-engine-verified). |
| `.raid unmark [<tank>]` | `co -mark rti` | |
| `.raid clearmarks` | `SetTargetIcon` on your own guid | |
| `.raid ccmark` · `.raid moon` | `Group:SetTargetIcon(4, target)` | |
| `.raid skull` · `.raid mark` · `.raid focus` | `Group:SetTargetIcon(7, target)` | One branch: `elseif sub == "skull" or sub == "focus" or sub == "mark"`. |
| `.raid buff` · `.raid wbuff` · `.raid worldbuff` | **nothing** | Three `Say` calls explaining that the engine handles auras per spec. Verified: the whole file contains **zero** `CastSpell` call sites. |
| `.raid list` | `bot list` | Every bot account this character owns, online or not — what you want after a crash. |
| `.raid add <name>` | `bot add <name>` | `<name>` may be an **account**, which logs in every character on it. |
| `.raid remove <name>` | `bot remove <name>` | Logs a bot **out**. It does not delete it — `delete` is refused, see [Traps](#139--traps--strings-that-look-like-they-worked-and-did-not). |

The layer totals **27**: the 12 command-table rows above, the diagnostics below,
and 15 names that predate the command table — the `roles` / `ready` / `gear` /
`tank3` family and their aliases, all of which are listed inline here.

#### The diagnostics — the genuinely new capability

These answer questions this handbook previously could not answer at all, and they
are the first thing to reach for when a bot is not behaving.

| Command | Engine string | What it actually answers |
| :--- | :--- | :--- |
| **`.raid check [<name>]`** | `bot r all ?` · `bot w <name> all ?` | **Every bot prints its live installed strategy set, all four engines.** This is the single most useful command in the facade, and after the `.bot group` correction above it is the **verification step you should not skip**. `all ?` is a real node → `ChangeAllStrategyAction` (`ChatActionContext.h:135`) → `ChangeStrategyAction.cpp:148-151`, which calls `ai->PrintStrategies(requester, BOT_STATE_ALL)`. The `?` is the only character that makes it print, and it is a plain substring test, so it rides along harmlessly in the same message. The five state-prefixed forms are **five adjacent lines of one `supported` list** in `strategy/generic/ChatCommandHandlerStrategy.cpp`: `co` **:49**, `nc` **:50**, `de` **:51**, `react` **:52**, `all` **:53** — so `co ?` is `:49`, `nc ?` is `:50`, `de ?` is `:51` and `react ?` is `:52`. ⚠️ An earlier revision of this row cited `:40`, `:78`, `:94` and `:110`; those are `ss`, `save mana`, `give leader` and `jump` and have nothing to do with this. **The script's own help text gets it right** — `raid_automation.lua:2541` prints `co  nc  de  react  all  strategy add/remove/print (:49-53)`, and that span is exactly right. So the error was this document's, not the script's; the Lua is a source file and is left untouched. |
| **`.raid rc`** | `bot r ready` | Per-bot HP/MP, distance, and food / water / potion stock. Bots that fail report **which** check they failed. Also answers a raid ready-check. |
| **`.raid gearscore`** (`.raid gs`) | `bot r who` | Class, talent spec, **level**, **gear score**, and equipped-item quality counts. `GetEquipGearScore` is called at `WhoAction.cpp:127`. |
| **`.raid who`** | `bot r who` | The same string as `gearscore`; both print gear score. |
| **`.raid stats`** | `bot r stats` | Gold, bag-slot counts, repair cost, XP, power. ⚠️ **It does NOT print gear score.** `StatsAction::Execute` calls `ListGold` / `ListBagSlots` / `ListRepairCost` / `ListXP` / `ListPower` and nothing else — `GetEquipGearScore` is never called. **Keep the two clearly distinguished or you will reach for the wrong one.** |
| **`.raid spells [<name>]`** | `bot r spells` · `bot w <name> spells` | The spell list a bot has actually learned. This is the reliable way to confirm what it really specced, as opposed to what its class suggests. |
| **`.raid where [<name>]`** (`.raid pos`) | `bot r where` | Where each bot actually is. This is how you find the one that did not follow. |
| **`.raid avoidmob <id>` | `bot r avoid creature <id>` | "The raid should not path through these." `-<id>` removes one, bare **prints** the list. Comma-separate for several (`.raid avoidmob 1234,5678`). The engine accepts a creature **name** or a numeric id and answers either way, so a wrong guess is visible rather than silent. |

**Two PASS-layer diagnostics belong with these, because they answer questions
about a command you have not sent yet.** Both are documented in full in
[§13.6](#136--pass--escape-hatches-to-the-raw-engines), and both send nothing:

| Command | Engine string | What it actually answers |
| :--- | :--- | :--- |
| **`.raid preview <sub> [args]`** | *(none — dry run)* | "What would that command actually send, right now, for this roster?" It re-runs the real branch with `Emit` routed to a buffer and prints the numbered list. Use it **before** `.raid tanks <n>`, or before any `.raid all` you have not run before, because the engine ignores unrecognised commands silently and a typo otherwise looks exactly like a success. |
| **`.raid pending`** | *(none — reports queue depth)* | "Is the deferred `.raid attune` / `.raid tanks` batch still draining, or is it stuck?" Prints `Step queue: N pending (cap 64, T ticks since load)`. Run it after any command that failed mid-way. |

> ⚠️ **`.raid check` falsifies a claim the script itself used to make.** The
> v1.2.0 header asserted that no chat command could report a bot's live strategy
> set, and this handbook repeated it — see the corrected note under
> [Tank marking](#tank-marking--one-icon-per-tank). **`all ?` does exactly that.**
> The script still cannot *parse* chat it does not read back, so it still cannot
> use the answer to choose tanks automatically; but you can.

**Read `.raid check` once, after `.raid group`, not twice.** It is a full dump
per bot, and on a 40-man it is a wall. What you are looking for is the spec
strategy (`protection`, `holy`, `frost`, …) and `tank assist` / `dps assist`,
which come from the engine's default kit rather than from anything the script
sent. ⚠️ **This run is no longer optional, and it is the step that replaces the
`.bot group`-assigns-the-spec belief.** Because `HandleGroup` forwards no `role=`,
the spec strategy in this dump is a **random pick from the class's whole premade
list** — so `protection` may be absent from every single bot. Look for
`protection` specifically, then run `.raid tanks <n>` for the count you need, then
`.raid check` again only if a promotion misbehaved.

### 13.4 🎚️ TOGGLES — every switch now has an off form

| On | Off | Broadcast |
| :--- | :--- | :--- |
| `.raid aoe` | `.raid noaoe`, `.raid single` | `co +aoe` / `co -aoe` |
| `.raid loot` | `.raid noloot` | `nc +loot` / `nc -loot` |
| `.raid rest` (`.raid drink`, `.raid food`) | `.raid norest` | `nc +food` + `nc +potions` / `nc -food` + `nc -potions` |
| `.raid gather` | `.raid nogather` | `nc +gather` / `nc -gather` |
| `.raid avoid` | `.raid noavoid` | `co +avoid aoe` / `co -avoid aoe` |
| `.raid roll` | `.raid noroll` | `nc +roll` / `nc -roll` |
| — (`.raid roles` sets it) | **`.raid unfocus`** | `co -focus rti targets` |
| `.raid open` (`.raid use`, `.raid breach`) | — | `u` — ⚠️ **inert**, see [Traps](#139--traps--strings-that-look-like-they-worked-and-did-not) |
| `.raid ccmark` · `.raid skull` · `.raid clearmarks` | — | raid icons, not strategies |

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

| Command | Engine string | |
| :--- | :--- | :--- |
| `.raid follow [<name>]` | `bot r follow` · `bot w <name> follow` | **Soft regroup: bots WALK to you and hold.** `FollowChatShortcutAction` (`ChatShortcutActions.cpp:35-66`) resets the AI, then `ChangeStrategy("+follow,-passive,-stay,-wander")` in noncombat and `"-stay,-guard,-wander"` in combat, so it **drags the combat engine's stance out too**. It holds your formation slot — in a `custom` formation it takes a relative position to you, otherwise the default `follow` spot at `GetFollowAngle()`. Movement, not a teleport. |
| `.raid summon` · `.raid rally` | `bot summon *` | **Hard: every bot is TELEPORTED to you.** Instant, and the only fix for a bot stuck somewhere the navmesh cannot reach. |
| `.raid tele <alias>` | `tele group <alias>` | See the [alias audit](#%EF%B8%8F-raid-tele-alias-audit--several-were-landing-in-the-wrong-place). `SEC_DEVELOPER`, and it needs a selected, grouped player. |
| `.raid unbind` · `.raid resetlock` | `instance groupunbind all` | Clears instance lockouts. ⚠️ `.raid reset` does **not** exist — renamed in v1.2.0. |

The two are opposites that are constantly confused, which is why they are in the
same layer with a note rather than scattered.

### 13.6 🔓 PASS — escape hatches to the raw engines

**Six**, and they are **last** on purpose. An operator who genuinely needs a raw
verb should not have to read source to find it; an operator who is guessing should
not have 55 verbs to guess from. ⚠️ **Four was the v1.3.0 count** (superseded) —
v1.4.0 added the last two rows, both of which are **no-op diagnostics** that
belong here precisely because they answer "did that raw verb actually do
anything?".

| Command | Engine string |
| :--- | :--- |
| `.raid all [@filter] <cmd>` | `bot r [@filter] <cmd>` — one order to the whole roster |
| `.raid one <name> <cmd>` | `bot w <name> <cmd>` — one order to one bot |
| `.raid dc <sub> [args]` | `dc <sub> [args]` — the 10 live verbs in `DungeonClearDispatch` (`DungeonClearCommand.cpp:337-347`); the 11th, `test`, is **refused** as the GM harness |
| `.raid bot <verb> [args]` | `bot <verb> [args]` — **allowlisted** passthrough |
| **`.raid preview <sub> [args]`** | **nothing — dry run.** See below. |
| **`.raid pending`** | **nothing — reports queue depth.** See below. |

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

| Reading | Meaning | Action |
| :--- | :--- | :--- |
| `0 pending` | Nothing queued. The last batch finished. | Carry on. |
| A small `N`, `T` climbing | **Normal.** The queue runs **one step per map tick** (Lua `:1461-1487`). A 40-man `.raid tanks` is 2 steps per bot, so a 20-bot promotion legitimately takes ~40 ticks. | Wait. |
| `N` near `cap 64` | The queue is **full**. A further `.raid attune` or `.raid tanks` is **refused** with `Step queue full (N queued, 64 max) - '<label>' refused` — and for `promote` that means `Promotion queue refused - NO talent command was sent.` | Wait for it to drain. Nothing is half-applied. |
| `T` past ~200 with `N` not falling | **Stuck.** `STEP_MAX_AGE_TICKS = 200` (`:1430`) aborts the batch and reports `'<label>' waited more than 200 ticks without completing; the batch is abandoned.` | Investigate the failing step; `.eluna reload` rebuilds the queue from scratch. |

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

| Refused | Why |
| :--- | :--- |
| **`delete`** | **Permanently deletes the character.** |
| **`init`** | **Randomises gear** (`HandleBotInit`, `PlayerbotMgr.cpp:3260` — an earlier revision cited `:3267`). Distinct from `prep` — it is **not** an alias of it, and `.raid prep` is the safe one. |
| **`do`** | **Multi-word is broken.** `PlayerbotMgr.cpp:2023` re-reads an invariant `param` on every loop iteration, so the trimming is discarded and **`tank attack` can never resolve**. Only single-word actions work; use `.raid all tank attack` instead. |
| **`c`** | **Inert.** It forces `"monstertalk " + param` into `cdebug`, `DebugAction` prints a usage line, and the handler returns `"ok"` — **reporting success while doing nothing**. |
| **`tweak`** | **A no-op.** `sPlayerbotAIConfig.tweakValue = sPlayerbotAIConfig.tweakValue++` assigns the increment to itself and discards it, so the value never changes. GM-only, takes no arguments. |
| **`cmd`** | Duplicate of `.raid one`, with a worse error message. |
| **`reload`** | `SEC_GAMEMASTER`, and it reloads the whole AI config mid-raid. |
| **`help`** | Refused so it cannot shadow `.raid help`, which is the operator surface. Use `.raid help chat` for the raw chat-command list. |
| **`refresh`** | ⚠️ **Advertised in the engine's own help** at `PlayerbotMgr.cpp:3414` and implemented at `:3348` (`HandleBotRefresh`) — but **absent from the handler map**, so it answers "unknown command". An earlier revision cited `:3421` and `:3355`. Listed here because anyone who reads the help will try it. |
| `levelup` · `level` · `always` · `random` · `self` · `spoof` · `create` · `runtest` · `test` | Not exposed. `always` flips whether **you** are treated as a self-bot, not a raid switch; `self` turns the caller into a self-bot; `spoof` overrides who the "master" is (a headless-testing hook). ⚠️ On `create`: an earlier version of this row said it "bypasses the `HandleGroup` path **that assigns role-matched specs**, which is how you get a raid of badly-specced bots." **The relative clause was wrong** — `HandleGroup` assigns no role-matched specs at all, so `create` is not bypassing anything of the kind. What `create` actually bypasses is the **class-shape loop and its caps** (`AllowedClassRoleNr`): you get exactly the class and level you asked for, with no role quota to satisfy, which is how you end up with a raid that has no tank-capable class in it at all. `HandleCreate` still honours a `role=` if you pass one, because `:2562-2563` does parse it — `.bot group` is simply the path that never does. |
| `!` | Not a `bot` verb at all — GM-only "select every bot" shorthand. Refused only so the error is legible. |

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

| Family | Tokens | Notes |
| :--- | :--- | :--- |
| **Role** | `@tank` `@notank` `@heal` `@noheal` `@dps` `@nodps` `@melee` `@ranged` | **And nothing else.** There is no `@caster`, no `@nomelee`, no `@noranged`. |
| **Class** | `@warrior` `@paladin` `@hunter` `@rogue` `@priest` `@shaman` `@mage` `@warlock` `@druid` | Exactly nine. `@deathknight` is registered only under `#ifdef MANGOSBOT_TWO` (`ClassChatFilter`, `ChatFilter.cpp`), so it is **absent on this core**. |
| **Race** | `@human`, `@tauren`, … | Whatever `Player::getRace()` reports, lowercased. |
| **Level** | `@60`, `@10-20` | A **bare number** after `@`. No `@lvl` prefix, no `<` / `>` / `=` operator. ⚠️ **This is the one filter that will swallow something else**, because any leading `@<digits>` reads as a level. |
| **Gear tier** | `@tier1` … `@tier10`, or `@tier7-8` | ⚠️ **Carry the caveat:** the bands in `GearChatFilter` (`ChatFilter.cpp:312-331`) are the TBC-era ones and are **wrong for a Classic curve** — tier 1 is gs 61-69, nothing scores 100-119, and tier 4 jumps straight to 120-132. They are neither contiguous nor meaningful. Treat them as approximate. No `@gs`, no `@ilvl`. |
| **State** | `@needrepair` `@bagfull` `@bagalmostfull` `@inside` `@outside` | ⚠️ `@dead`, `@alive`, `@combat` and `@noncombat` are **not** filters in this tree. `@dead=` is a *strategy-membership* filter, which is a different thing. |
| **Strategy state** | `@co=<s>` `@noco=<s>` `@nc=<s>` `@nonc=<s>` `@dead=<s>` `@nodead=<s>` `@react=<s>` `@noreact=<s>` | The **key** is checked; the **name** is not. `ChatFilter.cpp:73-84` just tests `HasStrategy` against a substring, so a typo sends the order to **nobody** — the safer of the two failure directions, but still silent. |
| **TalentSpec** | `@<spec>` — `@frost`, `@holy`, `@arms`, `@elemental`, … | Target by **actual spec** rather than by role. The full set is the `specs[]` table in `ChatHelper.cpp:150-190`, and three of the names are traps: the druid feral tree is spelled **`feral combat`** and the hunter's is **`beast mastery`**, both with a space, and the filter is a `message.find("@" + specName) == 0` prefix test (`ChatFilter.cpp:1008`) — so **any token that stops at the space matches nothing at all**. `@feral` and `@beast` are half-tokens that select nobody. And the rogue's is spelled **`assasination`** in the source, so the working token is `@assasination`. |

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

| Command | |
| :--- | :--- |
| `.raid` · `.raid help` · `.raid ?` | The index. |
| `.raid help battle` | 12 run-control commands. |
| `.raid help roster` | 27 roster commands. |
| `.raid help toggles` | The toggle table, on and off strings side by side. |
| `.raid help move` | Soft vs hard, and the difference. |
| `.raid help pass` | The escape hatches — the four raw passthroughs, plus the `preview` and `pending` diagnostics. |
| `.raid help filters` | The `@token` reference. |
| `.raid help traps` | The footguns — see below. |
| `.raid help chat` | The raw chat-command list, sorted into useful / fights-DC / irrelevant. |
| `.raid help full` | The v1.2.0 wall of text, kept whole. |

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

| Name | What actually happens |
| :--- | :--- |
| **`attackers`** | **A dead end, not just a broken command.** A `TriggerNode` exists (`:314`) and an `AttackersValue` exists (`ValueContext.h:249`), but **no action named `attackers` is registered in any of the three action contexts**, so `DoSpecificAction` finds nothing. It can never do anything on this build. |
| **`use` / `u`** | **Both silently ignored.** Both create a trigger named `use` (`ChatTriggerContext.h:34-35`) and the module's only node of that shape is named `u` (`:150`), which nothing maps to. `UseAction` is reachable only from `LootAction.cpp:230` and `RpgSubActions.h:292`, never from a chat command. **There is no working chat spelling of "use this item" on this build** — and `.raid open` sends `u`, so `.raid open` is inert too. ⚠️ Volume 02's shortcut table used to list these as working; it now carries the correction. |
| **`inventory`** (also `items`, `inv`) | All three map to the trigger `c` (`ChatTriggerContext.h:36-38`) → `"item count"` → `TellItemCountAction`. It is an **item count report**. It does not open anything, and it is not the loot command people assume. |
| **`gbank`** | Works, and it is the **GUILD** bank (`ChatTriggerContext.h:76` → `GuildBankAction`, `ChatActionContext.h:151`). On a bot in no guild it does nothing at all. |
| **`.bot c`** | Registered and **inert** — see the refusal table in [§13.6](#136--pass--escape-hatches-to-the-raw-engines). |
| **`bot rl`** | **Not** a leader-only broadcast. It queues the same `CHAT_MSG_RAID` packet `bot r` does, then *reports that it transferred leadership*. It did not. **The reply string is the lie, not the behaviour.** |
| **`attack rti 8`** | Broken by design. `AttackRTITargetAction` **discards its parameter** and reads the `rti target` *value*. Icons are selected by **name** (`rti skull`, `rti moon`), and `GetRtiIndex` returns `-1` for a number. There is no numeric form anywhere in the module. |
| **`formation back`** | Not a formation. The valid set is `melee queue chaos circle line shield arrow near default far custom`. Healers get `far`. |
| **`dc pull leeroy`** | Not a mode. `leeroy` is a pull **outcome** the Dynamic governor chooses per pack. |
| **`stats` is not gear score** | Stated once and worth stating again: `StatsAction` prints gold, bags, repair cost, XP and power. **`who` is what prints gear score.** |
| **`ready`** | **Works, but by coincidence across two files** — which is why it is on this list at all. The chat word creates a trigger named **`ready check`**, not `ready` (`ChatTriggerContext.h:119`). The node that actually fires comes from `WorldPacketHandlerStrategy`'s own `supported` list (`:16`) through the shared pass-through — **not** from the dead-weight `TriggerNode("ready")` at `ChatCommandHandlerStrategy.cpp:318`, which can never fire because no trigger is ever named `ready`. It is the string behind `.raid rc`, and a future patch that tidied the dead node out of the wrong file would break it silently. |

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

| Boss / Area | Encounter ID | Key Automations in `mod-dungeon-clear` & `mod-playerbots` |
| :--- | :---: | :--- |
| **Razorgore the Untamed** | `12435` | • **Phase 1 (4-Corner Defense)**: The raid deterministically spreads across the 4 room corners (North: `-7539, -1052`; South: `-7651, -1054`; East: `-7615, -1105`; West: `-7576, -1001`). Each corner team (tanks, healers, DPS) holds their corner post (`move to razorgore corner`) and defends against local spawn waves (`attack razorgore corner add`). Mages (`12420`) are burned first, then Legionnaires (`12416`). Bots never abandon their corner post to chase across the room.<br>• **Dragonkin Hibernate**: Druid bots cast Hibernate (`Spell 2637/8952/8953`) on `DEATH_TALON_DRAGONSPAWN` (`12422`) via `razorgore sleep dragonkin`. Slept dragonkin are marked with breakable CC in `PossibleAttackTargetsValue::HasBreakableCC` and are strictly ignored by all bots so sleep is never broken.<br>• **Egg Destruction**: The player controls Razorgore via Orb (`GO 177808`) to destroy the 30 eggs while bots protect the room corners.<br>• **Phase 2 Burn**: Razorgore receives `Warming Flames` (`23040`). Adds flee (`EvadeTroops`). Raid collapses on Razorgore (`attack entry::12435`). Offtank executes instant `taunt razorgore` on Conflagration (`23023`) to prevent boss turning on raid. Fire Protection Potions used for Fireball Volley (`22425`). |
| **Vaelastrasz the Corrupt** | `13020` | • **Essence of the Red (`23513`)**: Bots run unconstrained maximum burst rotations.<br>• **Burning Adrenaline (`23620`)**: Mana users run out 25yd (`move away from raid::25`). When MT receives Burning Adrenaline, offtank taunts immediately (`vael tank switch` -> `taunt vael`) and dying MT runs to sacrifice wall.<br>• **Flank Stance**: Raid stays strictly on Vaelastrasz's flanks to avoid Flame Breath and Tail Sweep. |
| **Suppression Room** | Area | • **Trap Disarm**: Rogues stealth and disarm Suppression Devices (`GO 179784` via `Spell 1842`).<br>• **Hazard Keep-Out**: Registered in `DcHazardRegistry` (15yd keep-out for camps/standoff; vacate 0 so rogues disarm unimpeded).<br>• **Moving Camp**: `DcPullPlanner` maintains continuous upward progress to prevent infinite whelp respawns. |
| **Broodlord Lashlayer** | `12017` | • **Knock Away (`18670`)**: Knocks active tank and cuts threat by 50%. Offtank taunts (`broodlord tank switch` -> `taunt broodlord`).<br>• **Wall Position**: MT anchors back against corner wall to prevent being punted into suppression room.<br>• **Blast Wave (`23331`)**: Casters and healers stay > 20 yards away.<br>• **Mortal Strike (`24573`)**: Healers pre-cast heavy heals on tank. |
| **Firemaw** | `11983` | • **Shadow Flame Auto-Clear**: `clear shadow flame dot` removes DoT `22682` automatically without requiring Onyxia Scale Cloaks (`Item 15138`) on all 40 bots.<br>• **Flame Buffet (`23341`)**: Bots hide behind doorway pillar (`hide from firemaw`) when stacks >= 3.<br>• **Wing Buffet (`23339`)**: Offtank steps into LoS and taunts (`taunt firemaw`) right before Wing Buffet to absorb threat wipe. |
| **Ebonroc** | `14601` | • **Shadow of Ebonroc (`23340`) [CRITICAL FIX]**: Tank swap triggered immediately when MT receives aura 23340 (`ebonroc tank switch` -> `taunt ebonroc`), starving Ebonroc of 25,000 HP healing per swing.<br>• **Shadow Flame Auto-Clear**: `clear shadow flame dot` cleanses DoT 22682 on contact. Inactive tank hides out of line of sight. |
| **Flamegor** | `11981` | • **Frenzy (`23342`)**: Hunter bots cast Tranquilizing Shot (`Spell 19801`) within 1 second.<br>• **Wing Buffet**: Standard offtank taunt swap (`taunt flamegor`).<br>• **Shadow Flame Auto-Clear**: `clear shadow flame dot` cleanses DoT 22682 immediately. |
| **Chromaggus** | `14020` | • **Portcullis Lever (DC Event 1)**: Laboratory lever (`GO 179148`) pulled automatically via `BlackwingLairEvents.cpp` once Firemaw, Ebonroc, and Flamegor are defeated, opening iron gate `179116`.<br>• **Brood Affliction Dispels**: Mages/Druids decurse Black (`23154`); Priests/Paladins dispel Blue (`23153`); Priests/Shamans cure Red (`23155`); Druids/Shamans cure Green (`23169`); Bronze (`23170`) cured via `clear chromaggus bronze`.<br>• **Time Lapse (`23310`)**: Offtank taunts Chromaggus upon MT threat wipe.<br>• **Breath LoS**: Raid hides behind ramp wall during devastating breath casts.<br>• **20% Enrage**: Full cooldown and execute burn. |
| **Nefarian** | `11583` | • **Throne Gossip (DC Event 2 & Roster Patch)**: Lord Victor Nefarius (`10162`) gossiped automatically on the throne (`-7526.27, -1135.04, 473.45`). Menu options drilled down to start trial.<br>• **Phase 1 Add Defense**: Tanks hold South/North doors; AoE burn 42 drakonids.<br>• **Dynamic Spawn Wait**: Event uses `WaitForSpawn(11583)` across wave combat until dragon form lands, then triggers `KillCreatureEngage`.<br>• **Shadow Flame Auto-Clear**: `clear shadow flame dot` removes DoT 22682 without requiring cloaks.<br>• **Class Calls**: Priests stop direct heals; Shamans destroy corrupted totems (`14662–14666`); Warriors Shield Wall; Rogues reposition after head teleport.<br>• **Phase 3 (20% HP)**: Nefarian raises Bone Constructs (`14605`). Mages AoE kite while raid burns Nefarian. |

### 10.3 Cross-Instance Scripting Audit & Proven Tactical Patterns (Beyond MC & ZG)

To ensure robust autonomous clearing and reliable bot behaviors across all vanilla and Turtle-WoW content, our raid and dungeon operations leverage proven patterns implemented throughout `mod-playerbots` and `mod-dungeon-clear`. The table below outlines how multi-wave defense, split-positioning, priority adds, and crowd control are unified across encounters:

| Dungeon / Raid | Instance / Map | Scripted Encounter | Mechanics & Proven Tactical Solutions |
| :--- | :---: | :--- | :--- |
| **Blackwing Lair** | Map `469`<br>(40-Man) | **Razorgore the Untamed** (`12435`) | • **4-Corner Defense**: Raid splits into 4 balanced squads (North, South, East, West) via `RazorgoreCornerIndex(ai)`. Bots garrison their assigned corner (`move to razorgore corner`) and burn local wave spawns (`attack razorgore corner add`).<br>• **Dragonkin Hibernate**: Druids cast Hibernate (`Spell 2637/8952/8953`) on `DEATH_TALON_DRAGONSPAWN` (`12422`).<br>• **Breakable CC Protection**: `PossibleAttackTargetsValue::HasBreakableCC` treats Hibernate, Sleep, Blind, and Wyvern Sting as breakable CC, strictly preventing bots from attacking slept targets while other enemies live.<br>• **Phase 2 Tank Swap**: Offtanks taunt Razorgore on Conflagration (`23023`) to prevent boss turning on raid. |
| **Blackwing Lair** | Map `469`<br>(40-Man) | **Suppression Room** | • **Rogue Stealth & Disarm**: Rogues enter stealth, approach traps (`move to suppression device`), and disarm Suppression Devices (`GO 179784` via `Spell 1842`).<br>• **15yd Keep-Out Zone**: Registered in `DcHazardRegistry` so non-rogue bots never trigger the slowing aura.<br>• **Moving Camp**: `DcPullPlanner` maintains steady forward progress to avoid infinite whelp spawns. |
| **Blackwing Lair** | Map `469`<br>(40-Man) | **Drake Trio (Firemaw, Ebonroc, Flamegor)** | • **Shadow Flame Cleanse**: Auto-cleanses DoT `22682` to eliminate Onyxia Scale Cloak requirements.<br>• **Shadow of Ebonroc Swap**: Immediate offtank taunt on aura `23340` to starve Ebonroc of 25,000 HP healing per swing.<br>• **Tranquilizing Shot**: Hunters soothe Frenzy (`23342`) on Flamegor within 1 second.<br>• **LoS Corner Peeking**: Bots reset Flame Buffet stacks (`23341`) behind doorway pillars. |
| **Naxxramas** | Map `533`<br>(40-Man) | **Gothik the Harvester** (`16060`) | • **Deterministic Room Split**: Gothik requires $\ge 10$ players on each side of the combat gate (`HasLessPlayersPerSide`). The raid divides into Dead Side and Live Side via `GothikSideIndex(ai)` and walks to their assigned post (`move to gothik post`).<br>• **Wave Prioritization**: Bots burn 18 waves of Unrelenting and Spectral adds before Gothik teleports down into the arena. |
| **Naxxramas** | Map `533`<br>(40-Man) | **Four Horsemen** (`16062`–`16065`) | • **4-Corner Tank Rotation**: Tanks rotate Thane, Mograine, Zeliek, and Blaumeux across 4 room corners to drop stacking marks before reaching lethal stacks.<br>• **Void Zone Evasion**: Bots detect Void Zones (`28863`) and vacate within 1 second (`move away from void zone`). |
| **Naxxramas** | Map `533`<br>(40-Man) | **Instructor Razuvious** (`16061`) | • **Dual Priest Mind Control**: Priests mind control Deathknight Understudies (`16803`), immediately activating Understudy Shield Wall (`29061`) followed by Taunt (`29060`).<br>• **Mind Exhaustion Rotation**: Priests alternate understudies to cycle around the 60-second Mind Exhaustion (`29051`) lockout. |
| **Naxxramas** | Map `533`<br>(40-Man) | **Gluth** (`15932`) | • **Zombie Chow Kiting**: Mages and Hunters snare and kite Zombie Chows (`16360`) away from Gluth to prevent Decimate healing.<br>• **Mortal Wound Swap**: Offtanks taunt at 3–4 stacks of Mortal Wound (`25646`). |
| **Naxxramas** | Map `533`<br>(40-Man) | **Thaddius** (`15928`) | • **Polarity Shift**: Bots detect Positive (`28059`) and Negative (`28084`) charges and move to designated left/right charge camps to avoid devastating raid wipes. |
| **Temple of Ahn'Qiraj** | Map `531`<br>(40-Man) | **The Prophet Skeram** (`15263`) | • **Illusion Splits**: At 75%, 50%, and 25%, Skeram creates 2 copies. Offtanks pick up copies while casters interrupt Arcane Explosion.<br>• **Mind Control CC**: Players afflicted with True Fulfillment (`785`) are sheeped, blind, or trapped until the effect expires. |
| **Temple of Ahn'Qiraj** | Map `531`<br>(40-Man) | **Bug Trio (Kri, Yauj, Vem)** | • **Synchronized Burn**: Killing one bug causes the survivors to devour the corpse for a 100% full heal. Bots balance damage across all three targets before executing them simultaneously. |
| **Temple of Ahn'Qiraj** | Map `531`<br>(40-Man) | **Twin Emperors** (`15275` / `15276`) | • **Teleport Swap**: Vek'nilash (Physical immune to spells) and Vek'lor (Caster immune to melee) swap positions every 30–40 seconds. Tanks and healers re-anchor across 60yd to avoid mutual heal link (`23957`). |
| **Ruins of Ahn'Qiraj** | Map `509`<br>(20-Man) | **General Rajaxx** (`15341`) | • **7-Wave Defense**: Bots garrison the entrance ramp, defending General Andorov (`15471`) against 7 waves of officers (`15391` $\rightarrow$ `15392` $\rightarrow$ `15389` $\rightarrow$ `15390` $\rightarrow$ `15386` $\rightarrow$ `15388` $\rightarrow$ `15385` $\rightarrow$ `15341`).<br>• **180s Timeout Prevention**: Bots stay in combat at the ramp to avoid wave reset timeouts (`generic_scripts 153912`). |
| **Ruins of Ahn'Qiraj** | Map `509`<br>(20-Man) | **Kurinnaxx** (`15348`) | • **Sand Trap Avoidance**: 15yd silence/slow hazard keep-out (`GO 180647`).<br>• **Mortal Wound Tank Swap**: Offtanks taunt when MT reaches 5 stacks of Mortal Wound (`25646`). |
| **Ruins of Ahn'Qiraj** | Map `509`<br>(20-Man) | **Moam** (`15340`) | • **Mana Fiend Priority**: Mana Fiends (`15527`) burned immediately; Priests/Hunters drain Moam's mana to delay catastrophic Arcane Eruption (`25672`). |
| **Stratholme** | Map `329`<br>(5-Man) | **Slaughter Square & Baron Rivendare** | • **13 Abomination Cleansing**: Full square clear triggers Ramstein the Gorger (`10439`).<br>• **Mindless Undead Wave**: 33 mindless undead surge from the gates; Mages/Paladins AoE burn the wave in center square.<br>• **Black Guard Ambush**: 5 Black Guard Sentinels walk gate route to Baron's ziggurat; tanks intercept before door opens (`STR_GO_BARON_DOOR` `175796`). |
| **Zul'Farrak** | Map `209`<br>(5-Man) | **Pyramid Wave Event** | • **Ramp-Head Garrisoning**: Bots hold coordinates at top of ramp (`ZF_RAMP_X/Y/Z`), intercepting 100 descending Sandfury trolls before they reach Sergeant Bly and Weegli Blastfuse.<br>• **Gossip Door Trigger**: Blastfuse blows open the door; bots defeat Bly's crew upon betrayal. |
| **Sunken Temple** | Map `109`<br>(5-Man) | **Avatar of Hakkar** (`8443`) | • **Blood Pool Suppression**: Hakkari Bloodkeepers and Suppressors channel into Hakkar's pool. Offtanks peel suppressors while raid holds center to summon the Avatar. |
| **Blackrock Depths** | Map `230`<br>(5-Man) | **The Lyceum & Shadowforge Braziers** | • **Dual Flame Keeper Hunt**: 2 Shadowforge Flame Keepers drop braziers. Bots light both braziers within 5 minutes to unlock the Iron Hall doors before infinite dwarf waves overwhelm the party.<br>• **Princess Moira Preservation**: Offtank holds Princess Moira Bronzebeard (`8929`) with fears/stuns without killing her, ensuring Emperor Thaurissan is defeated while preserving the Ironforge royal succession. |
| **Tower of Karazhan** | Map `532` 10-Man<br>Map `814` 40-Man<br>~~812~~ superseded | **Netherspite & Prince Malchezaar** | • **Portal Beam Rotation**: Bots rotate Dominance, Serenity, and Perseverance beams on Netherspite to starve boss buffs and empower the tank/healer.<br>• **Infernal Avoidance**: Bots detect incoming Malchezaar Infernals and relocate outside 20yd fire impact radius. |
| **Emerald Sanctum** | Map `807`<br>(40-Man) | **Sanctum Dragons** | • **Corrupted Treant Wave Burn**: Treant adds rooted and burned before reaching Sanctum guardians; nature breath cones faced away from raid. |

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

| Phase & Threshold | Mechanics & Spell IDs | Autonomous Bot Tactics & Engine Behaviors |
| :--- | :--- | :--- |
| **Phase 1: Ground**<br>(100% $\rightarrow$ 65% HP) | • **Flame Breath (`18435`)**: Frontal fire cone dealing 3,500–4,500 damage.<br>• **Wing Buffet (`18500`)**: Frontal knockback reducing tank threat.<br>• **Knock Away (`19633`)**: Knocks active target and cuts threat by 25%.<br>• **Tail Sweep (`15847`)**: Rear knockback dealing 1,500 damage and punting players into the whelp egg caves. | • **North Wall Tank Anchor**: Main Tank immediately runs to North wall (`-8.86f, -212.75f, -88.54f`) with back against the wall, turning Onyxia South. This guarantees Flame Breath and Wing Buffet never hit the raid.<br>• **Flank Positioning**: Ranged, melee, and healers position strictly on Onyxia's left or right flanks, completely avoiding frontal cone and rear tail sweep arcs.<br>• **Threat Stacking**: Tank builds heavy Sunder Armor and Shield Slam threat before Phase 2 transition. |
| **Phase 2: Airborne**<br>(65% $\rightarrow$ 40% HP) | • **Lift Off (`17131` / `HOVER`)**: Onyxia takes flight, dropping melee reachability.<br>• **Deep Breath (`23461`)**: Devastating fire carpet covering room quadrants.<br>• **Fireball (`18392`)**: Targeted bombardment on individual players.<br>• **Whelp Waves (`11262`)**: 16 initial whelps, followed by periodic waves from East and West egg caves.<br>• **Onyxian Warders (`12129`)**: Heavy melee elite dragonkin spawning from the entrance tunnel. | • **Airborne Detection**: Trigger `OnyxiaAirborneTrigger` detects aura `17131`. Melee DPS switch off boss.<br>• **Deep Breath Scatter**: When Onyxia casts `23461`, `OnyxiaDeepBreathTrigger` triggers `MoveAwayFromOnyxiaAction` (40yd scatter perpendicular to flight lane) within 500ms.<br>• **Add Prioritization**: Offtanks immediately taunt and face Onyxian Warders (`TauntOnyxianWarderAction`). Melee and AoE casters prioritize Onyxian Whelps (`AttackOnyxiaWhelpsAction`).<br>• **Ranged Focus**: Hunters, Warlocks, and Mages burn airborne Onyxia down to 40%. |
| **Phase 3: Landing & Burn**<br>(40% $\rightarrow$ 0% HP) | • **Threat Reset (`DoResetThreat`)**: Onyxia lands at 40% HP with a completely wiped threat table!<br>• **Bellowing Roar (`18431`)**: Room-wide fear cast every 15–30s.<br>• **Floor Eruptions (`20009`)**: Cracking lava fissures dealing 1,500–2,000 fire damage.<br>• **Residual Whelps & Warders**: Ongoing spawns from Phase 2. | • **Landing Threat Snap Taunt**: `OnyxiaPhase3LandingThreatTrigger` detects grounding and health $\le 40\%$. Main Tank executes instant `TauntOnyxiaLandingAction` and Shield Slam to lock aggro before any healer or caster draws lethal attacks.<br>• **Re-Anchoring North**: Main Tank repositions back to `-8.86f, -212.75f, -88.54f`.<br>• **Fear Counter-Measures**: Shaman Tremor Totems, Warrior Berserker Rage, Priest Fear Ward, and Undead Will of the Forsaken maintain tank control.<br>• **Execute Burn**: Offtanks clean up remaining whelps; entire raid pops cooldowns and burns Onyxia to 0%. |

---

## 12. 10-Man Raids & Custom Dungeons Operations Guide

In addition to 40-man raid automation, our core features verified scripts, event triggers, and combat handling for 10-man raids and custom Turtle-WoW dungeons.

### 12.1 Lower Karazhan Halls (Map `532`, 10-Man Raid)

> This heading previously said Map `812`. `812` is not a row in `map_template`
> (`sql/base/tw_world_map_template.sql`); Lower Karazhan Halls is `532`
> (`instance_lower_karazhan_halls`, `player_limit` 10).

Located in Deadwind Pass, Lower Karazhan Halls represents an essential stepping stone between Tier 0.5/Tier 1 and 20/40-man raiding.

#### Moroes & The Dinner Guests (`boss_moroes.cpp`)

Moroes spawns with 4 random undead dinner guests from a pool of 6 possible nobles. Understanding guest priorities and CC assignment is mandatory for flawless execution:

| Guest Name | Class & Role | Critical Spells | Priority & Strategy |
| :--- | :--- | :--- | :--- |
| **Baroness Dorothea Millstipe** | Shadow Priest | • **Mana Burn (`29405`)**: Drains 1,000+ mana from healers.<br>• **Mind Flay (`17313`)**: High channeled shadow damage. | **KILL FIRST (Priority #1)** or **Priest Shackle Undead**. Must not be allowed to free-cast Mana Burn on healer bots. |
| **Lady Catriona Von'Indi** | Holy Priest | • **Greater Heal (`29564`)**: Massive 8,000+ HP heal on Moroes or guests.<br>• **Power Word: Shield (`29408`)**: Damage absorption. | **KILL FIRST / INTERRUPT (Priority #1)**. Rogues/Warriors kick/pummel Greater Heal; burst down immediately. |
| **Baron Rafe Dreuger** | Retribution Paladin | • **Hammer of Justice (`13005`)**: 6-second stun on tank.<br>• **Cleanse (`19703`)**: Dispels CC from other guests. | **Priority #2 / Shackle**. Offtank holds away from main tank; Paladin/Priest cleanses stun. |
| **Lord Robin Daris** | Arms Warrior | • **Mortal Strike (`16856`)**: 50% healing cut and heavy physical burst.<br>• **Whirlwind (`15589`)**: 360-degree melee cleave. | **Priority #2 / Offtank / Shackle**. Melee stay at max melee range; offtank turns outward. |
| **Lady Keira Berrybuck** | Holy Paladin | • **Holy Light (`29562`)**: Heals allies.<br>• **Blessing of Protection (`13007`)**: Physical immunity on nobles. | **Priority #3 / Interrupt**. Counterspell/Kick Holy Light; purge or wait out BoP. |
| **Lord Crispin Ference** | Protection Warrior | • **Disarm (`6713`)**: Disarms tank for 5s.<br>• **Shield Wall (`29390`)**: 75% damage reduction. | **TANK LAST**. High armor and defenses make him inefficient to kill early. Offtank holds until Moroes is dead. |

• **Moroes Garrote Triage**: Moroes uses Vanish (`29448`) and reapplies Garrote (`37066`), dealing 1,000 physical bleed damage every 3 seconds for 5 minutes. Healers maintain HoTs (Rejuvenation, Renew) on all Garrote victims. At 30% HP, Moroes Frenzies (`37023`), increasing physical damage by 50%—pop defensive cooldowns and burn.

#### Brood Queen Araxxna (`boss_brood_queen_araxxna.cpp`)

- **Web Wrap (`29234`)**: Encases players in cocoons. Melee and ranged bots immediately switch to the cocoon to free wrapped allies within 3 seconds.
- **Spiderling Swarms**: AoE Frost Nova, Blizzard, and Consecration burn spider waves before they reach healers. Nature Protection Potions minimize poison volley damage.

#### Clawlord Howlfang (`boss_clawlord_howlfang.cpp`)

- **Worgen Pack Cleave**: Howlfang summons ferocious Shadowfang Worgen. Tanks stack adds tightly for cleave; Hunters tranquilize enrage effects.

### 12.2 Upper Karazhan Halls (Map `814`, 40-Man Raid)

> This heading previously said Map `812` and 10-Man. Both were wrong: Upper
> Karazhan is `814` (`Karazhan (Upper)`, `player_limit` 40), while the 10-Man
> Karazhan map is `532` (§12.1). `812` is not a row in `map_template`.

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

> This heading previously said Map `813`. `813` is `Winter Veil Vale`, a
> 0-player map with no instance script. Karazhan Crypt is `800`
> (`instance_karazhan_crypt`, `player_limit` 5).

Custom high-level dungeon featuring tight subterranean corridors and punishing undead mechanics:

- **Corridor Navigation & Line of Sight**:
  - Narrow catacomb architecture requires tanks to pull packs backward into intersections to avoid body-pulling patrolling ghoul packs.
  - `DcPullPlanner` uses conservative camp placement to prevent bots from falling through uneven crypt mesh geometry.
- **Necromantic Ritualists**:
  - Must be stunned and burned first to prevent summons of Crypt Fiends and Shadow Phantoms.
  - Priests use Shackle Undead to control elite crypt defenders.

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Playerbot Handbook](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md)
