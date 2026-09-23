# 06. Automated Raid & Dungeon Operations Handbook

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + mod-playerbots + mod-dungeon-clear + Eluna
> **Engine Verification**: `lua_scripts/raid_automation.lua`, `PlayerbotMgr.cpp`, `Commands.cpp`, `DungeonClearCommand.cpp`
> **Security Requirement**: Accessible to all players (`SEC_PLAYER` / Level 0) with elevated GM features where noted
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Playerbot Handbook](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md)

---

## 📑 Table of Contents

1. [Architectural Overview & Core Philosophy](#1-architectural-overview--core-philosophy)
2. [Master Macro Cheat Sheet (Copy-Paste Ready)](#2-master-macro-cheat-sheet-copy-paste-ready)
3. [The Master 1-Click Pipeline (.raid ready)](#3-the-master-1-click-pipeline-raid-ready)
4. [Instant Raid Attunements & Master Keyring Suite](#4-instant-raid-attunements--master-keyring-suite)
5. [Autonomous Dungeon Clear (.dc) Operations Deck](#5-autonomous-dungeon-clear-dc-operations-deck)
6. [In-Dungeon Recovery, Doors, AoE & Target Focus](#6-in-dungeon-recovery-doors-aoe--target-focus)
7. [Automatic Role, Stance & Formation Matrix](#7-automatic-role-stance--formation-matrix)
8. [Master Instance Fast-Teleport Directory](#8-master-instance-fast-teleport-directory)
9. [C++ Engine Contracts & Technical FAQ](#9-c-engine-contracts--technical-faq)

---

## 1. Architectural Overview & Core Philosophy

This operations handbook documents the **Tortoise-WoW Autonomous Raid Controller**, an enterprise-grade Eluna automation layer designed to eliminate manual command micromanagement during 5-man dungeon runs and 10/20/40-man raids.

### Core Design Rules

- **No Level Tampering**: Bots and characters invited via the in-game group finder (LFT / LFG) are already level 60. The controller never alters character levels.
- **Player-Controlled Group Topology**: In Vanilla WoW, a standard group is capped at 5 members. Raids hold 6 to 40 members across 8 subgroups. Group-to-raid conversion is handled by the player via the in-game UI. The controller never forces group conversions.
- **Engine-Verified Syntax**: All commands dispatch directly into the CMaNGOS command handler (`PlayerbotMgr.cpp`, `Commands.cpp`, `DungeonClearCommand.cpp`) using native server tokens. Client chat prefixes (`/p`, `/ra`) are never sent into server command queues.
- **Raid Attunement vs Quest Sharing**: Generic quest sharing does not grant raid access. The controller executes verified quest completions and spawns physical access keys for both Classic raids and Turtle-WoW custom content.

---

## 2. Master Macro Cheat Sheet (Copy-Paste Ready)

Create these 4 macros in your World of Warcraft **Macro UI** (`/m`) and place them on your action bar:

### 🌟 Macro 1: "1-Click Raid Ready"

*Run this immediately after assembling your party or raid via the LFG tool:*

```text
.raid ready world
```

*(Gears bots in Epic armor, enchants gear, learns spells, restocks food/pots/reagents, assigns optimal Tank/Healer/DPS roles and formations, completes all raid attunements, spawns master keys, and applies full class buffs + Onyxia/Rend/ZG world buffs).*

### ⚔️ Macro 2: "Dungeon Autopilot Engaged"

*Run this once inside the dungeon or raid portal:*

```text
.raid start
```

*(Orders the lead tank bot to activate autonomous pathfinding, calculate optimal trash/boss routes, dynamic-pull packs, and hold aggro while party bots follow and assist).*

### ⏸️ Macro 3: "Party Hold / Drink Break"

*Run this when you need to pause the tank, loot, or replenish mana:*

```text
.raid pause
.raid loot
.raid rest
```

*(Halts tank pathfinding, stops combat, commands bots to vacuum nearby corpse loot, and forces all bots to sit down and drink/eat to 100% mana and health).*

### 🚪 Macro 4: "Open Door / Focus Skull"

*Run this to interact with gates/levers or burn down high-priority marked targets:*

```text
.raid open
.raid focus
```

*(Commands the nearest bot to right-click open locked gates, levers, chests, or doors, and forces all DPS bots to focus-burn the mob marked with the Skull icon).*

---

## 3. The Master 1-Click Pipeline (`.raid ready`)

Executing `.raid ready` (or `.raid ready world`) initiates a fully orchestrated 5-stage preparation sequence:

```text
[ LFG Group Assembled ]
           │
           ▼
Stage 1: Summon & Follow Lock  ──► Teleports bots to master coordinates (.bot summon *)
           │                        Enforces follow state (.bot c * follow, stay off, flee off)
           ▼
Stage 2: Epic Gearing & Stock  ──► Equips Level 60 Epic gear (.bot gear * epic)
           │                        Applies permanent enchants (.bot enchants *)
           │                        Teaches class spells & ranks (.bot train *)
           │                        Replenishes HP/mana, stocks food/potions/reagents (.bot prep *)
           ▼
Stage 3: Role & Stance Matrix  ──► Scans raid composition (Class IDs 1–11)
           │                        Assigns Tank (shield formation, defensive stance, threat co)
           │                        Assigns Healers (back formation, cure/dispel co)
           │                        Assigns DPS (melee behind target, casters in line formation)
           ▼
Stage 4: Raid Attunements      ──► Completes MC, BWL, Onyxia, Naxxramas attunement quests
           │                        Completes Emerald Sanctum, Karazhan, Timbermaw, Hateforge
           │                        Spawns 14 physical master keys into player inventory
           ▼
Stage 5: Instant Raid Buffs    ──► Fortitude, Shadow Protection, Spirit, Arcane Brilliance,
                                    Gift of the Wild, Greater Blessing of Kings (+10% all stats)
                                    Optional: World Buffs (Onyxia, Rend, Songflower, Zandalar)
```

### Ready Subcommands

| Command | Arguments | Description |
| :--- | :--- | :--- |
| `.raid ready` | *none* | Full pipeline with Epic gear, roles, attunements, keys, and class buffs. |
| `.raid ready world` | `world` | Full pipeline + Onyxia, Rend, Songflower, and Spirit of Zandalar world buffs. |
| `.raid ready best` | `best` | Full pipeline equipping Best-in-Slot loadouts instead of baseline Epics. |
| `.raid gear` | `[epic\|best]` | Re-equips and re-enchants all bots without re-running attunements or buffs. |
| `.raid roles` | *none* | Re-evaluates raid composition and updates Tank/Healer/DPS assignments. |
| `.raid buff` | *none* | Instantly refreshes 30-minute class buffs on all group members (no mana cost). |
| `.raid worldbuff` | *none* | Applies high-potency world buffs (+140 AP, +10% stats, +10% crit, +15% haste). |

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
| **`.raid off`** | *none* | Disables the autonomous dungeon clear engine completely. |
| **`.raid pause`** (or `.raid stop`) | *none* | Pauses the tank bot's movement and drops combat (`.dc pause`, `combatstop`). Ideal for breaks or loot distribution. |
| **`.raid pull`** | `[dynamic\|leeroy\|advanced]` | Forces an immediate tank pull. Optionally sets pull aggressiveness (`dynamic` = smart pacing; `leeroy` = non-stop chain pulls). |
| **`.raid go <boss>`** | `<boss_name>` | Overrides default routing to direct the tank bot straight to a specific boss (e.g. `.raid go Lucifron`, `.raid go Magmadar`). |
| **`.raid skip`** | *none* | Commands the solver to abandon pathing to the current stuck trash pack or optional boss and route to the next objective. |
| **`.raid bosses`** | *none* | Dumps a live checklist of all bosses in the current instance with their **Alive** or **Dead** status. |
| **`.raid status`** | *none* | Displays completion percentage, active route segment, and current pathing target. |
| **`.raid config`** | *none* | Inspects live solver tunables (pull radius, leash duration, rest thresholds). |
| **`.raid spectate`** | `[follow <name>\|free]` | Activates cinematic spectator camera to watch your bots clear encounters from third-person perspectives. |

---

## 6. In-Dungeon Recovery, Doors, AoE & Target Focus

These commands allow you to command all bots in your party/raid simultaneously without opening whisper windows:

### Tactical Bot Directives

| Command | Underlying Engine Action | Gameplay Purpose |
| :--- | :--- | :--- |
| **`.raid loot`** | `bot c * loot` | Orders every bot to immediately vacuum loot from nearby enemy corpses. |
| **`.raid rest`** (or `.raid drink`) | `bot c * drink`, `bot c * food` | Orders bots to sit down and consume drinks/food until reaching 100% mana and health. |
| **`.raid open`** (or `.raid breach`) | `bot c * u` | Orders the closest bot to right-click interact with the nearest door, gate, switch, lever, or chest. |
| **`.raid aoe`** | `bot c * co +aoe` | Forces all casters and melee to prioritize high-damage AoE spells (Blizzard, Rain of Fire, Whirlwind, Consecration). |
| **`.raid single`** | `bot c * co -aoe` | Disables AoE abilities, forcing all bots into single-target high-efficiency damage. |
| **`.raid focus`** (or `.raid skull`) | `bot c * attack rti 8` | Forces all damage dealers and pets to instantly switch targets and burn down the mob marked with the **Skull** icon. |
| **`.raid summon`** | `bot summon *`, follow lock | Teleports all bots to your exact coordinates and locks them in follow mode. |
| **`.raid unbind`** | `instance groupunbind all` | Clears instance ID lockouts for every player and bot in the group. |

---

## 7. Automatic Role, Stance & Formation Matrix

When `.raid ready` or `.raid roles` executes, the controller scans your group and applies optimal role assignments, stances, and formations:

### Quota Allocation by Raid Size

| Bracket | Raid Size | Target Tanks | Target Healers | Target DPS |
| :---: | :---: | :---: | :---: | :---: |
| **5-Man Party** | 2–5 | 1 Tank | 1 Healer | 1–3 DPS |
| **10-Man Raid** | 6–10 | 2 Tanks | 3 Healers | 5 DPS |
| **20-Man Raid** | 11–20 | 3 Tanks | 5 Healers | 12 DPS |
| **40-Man Raid** | 21–40 | 5 Tanks | 10 Healers | 25 DPS |

### Class Assignment Logic

| Class | Assigned Role | Formation | Stance | Active Combat Strategies (`co`) |
| :--- | :---: | :---: | :---: | :--- |
| **Warrior (Tank)** | Tank | `shield` | `tank` | `+tank,prot,tank assist,avoid aoe,potions` |
| **Warrior (DPS)** | DPS | `melee` | `behind` | `-tank,-prot,+dps,+fury,+behind,+interrupt,+potions` |
| **Paladin (Heal)** | Healer | `back` | default | `+heal,cure,buff,potions` |
| **Paladin (Tank)** | Tank | `shield` | default | `+tank,prot,consecration,avoid aoe,potions` |
| **Paladin (DPS)** | DPS | `melee` | default | `+dps,retrib,consecration,potions` |
| **Priest (Heal)** | Healer | `back` | default | `+heal,cure,buff,potions` |
| **Priest (Shadow)** | DPS | `line` | default | `+dps,shadow,shadowform,cure,potions` |
| **Druid (Heal)** | Healer | `back` | default | `+heal,cure,buff,potions` |
| **Druid (Tank)** | Tank | `shield` | default | `+tank,bear,tank assist,avoid aoe,potions` |
| **Druid (Feral)** | DPS | `melee` | `behind` | `+dps,cat,behind,stealth,potions` |
| **Rogue** | DPS | `melee` | `behind` | `+dps,behind,interrupt,stealth,evasion,potions` |
| **Hunter** | DPS | `far` | default | `+dps,ranged,pet,ammo,potions` |
| **Mage** | DPS | `line` | default | `+dps,fire,aoe,interrupt,potions` |
| **Warlock** | DPS | `line` | default | `+dps,shadow,pet,curse,potions` |
| **Shaman (Heal)** | Healer | `back` | default | `+heal,cure,totems,potions` |
| **Shaman (DPS)** | DPS | `melee` | default | `+dps,windfury totem,totems,interrupt,potions` |

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

| Token Alias | Destination Instance | Bracket | Map ID | Location |
| :--- | :--- | :---: | :---: | :--- |
| `.raid tele scarlet` | **Scarlet Citadel** | 10-Man Raid | **811** | Tirisfal Glades |
| `.raid tele karazhan` | **Tower of Karazhan** | 10/40-Man Raid | **812** | Deadwind Pass |
| `.raid tele timbermaw` | **Timbermaw Hold** | 20-Man Raid | **815** | Felwood |
| `.raid tele sanctum` | **Emerald Sanctum** | 40-Man Raid | **814** | Mount Hyjal |
| `.raid tele crypt` | **Karazhan Crypts** | 58–60 Dungeon | **813** | Deadwind Pass |
| `.raid tele hateforge` | **Hateforge Quarry** | 50–60 Dungeon | **806** | Burning Steppes |
| `.raid tele crescent` | **Crescent Grove** | 32–38 Dungeon | **805** | Ashenvale |
| `.raid tele vault` | **Stormwind Vault** | Level 60 Dungeon | **810** | Stormwind City |
| `.raid tele gilneas` | **Gilneas City** | 42–48 Dungeon | **807** | Gilneas |

---

## 9. C++ Engine Contracts & Technical FAQ

### Why `.bot gear * epic` instead of `epics`?

In `modules/mod-playerbots/src/playerbot/PlayerbotMgr.cpp` (line 3123):

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

In `PlayerbotMgr.cpp` (line 3197):

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

The autonomous solver does not take over arbitrary bots. It specifically searches your group for a bot assigned the **tank** role. Running `.raid ready` or `.raid roles` assigns the tank role and defensive threat strategy to your Warrior, Paladin, or Feral Bear bot before `.dc on` is engaged.

### Why use Greater Blessing of Kings (+10% all stats) over multiple blessings?

In Classic WoW, a player can only receive **one blessing per Paladin caster**. When an Eluna script iterates through multiple Greater Blessings from the same caster, each blessing overwrites the previous one. Greater Blessing of Kings (Spell ID `25898`) provides a flat +10% increase to Strength, Agility, Stamina, Intellect, and Spirit across all classes without generating negative threat interactions (such as Blessing of Salvation on a tank).

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Playerbot Handbook](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md)
