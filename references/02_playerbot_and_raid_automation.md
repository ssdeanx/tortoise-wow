# 02. Playerbot & Raid Automation Handbook

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + mod-playerbots + mod-dungeon-clear + Eluna  
> **Security Coverage**: Accessible to all players (`SEC_PLAYER` / Level 0) with elevated GM features where noted  
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md) • [🏰 Volume 06: Raid Operations](./06_automated_raid_and_dungeon_operations.md) • [🛡️ Volume 07: Guilds](./07_guilds_and_guild_operations.md) • [🌍 Volume 08: World & Strategies](./08_open_world_control_and_strategies.md)

---

## Table of Contents

1. [Master Macro UI Cheat Sheet (Copy-Paste Ready)](#1-master-macro-ui-cheat-sheet-copy-paste-ready)
2. [Aura Buff Lists (.aura)](#2-aura-buff-lists-aura)
3. [Target Tokens & Chat Communication Scope](#3-target-tokens--chat-communication-scope)
4. [Playerbot Core Control Suite (.bot)](#4-playerbot-core-control-suite-bot)
5. [RandomBot Population & Realm AI Manager (.rndbot)](#5-randombot-population--realm-ai-manager-rndbot)
6. [In-Game Whispers & Single-Letter Shortcuts](#6-in-game-whispers--single-letter-shortcuts)
7. [Chat Filter Syntax & Targeting Token Engine (@filter)](#7-chat-filter-syntax--targeting-token-engine-filter)
8. [Formations & Stances Tactical Directory](#8-formations--stances-tactical-directory)
9. [Autonomous Dungeon Clear AI Engine (.dc)](#9-autonomous-dungeon-clear-ai-engine-dc)
10. [Raid Automation Operator Facade (.raid)](#10-raid-automation-operator-facade-raid)
11. [Class Combat Strategy Assignment Deck (/w %t)](#11-class-combat-strategy-assignment-deck-w-t)
12. [Raid Assembly, Multi-Account Alt Bots & Lockouts](#12-raid-assembly-multi-account-alt-bots--lockouts)
13. [Blackwing Lair Encounter Strategies](#13-blackwing-lair-encounter-strategies)

---

## 1. Master Macro UI Cheat Sheet (Copy-Paste Ready)

### Category A: Group Assembly, Gearing & Preparation Macros

```lua
-- ======================================================================
-- Macro 1: 1-Click Bot Party Summon & Follow Lock
-- Teleports all party bots to your position and locks follow
-- ======================================================================
.bot summon *
/p follow
/p stay off
/p flee off

-- ======================================================================
-- Macro 2: 1-Click Bot Level 60 Epics, Enchants & Stocking
-- Equips party bots with best gear, enchants, spells, and consumables
-- ======================================================================
.bot level * 60
.bot gear * best
.bot enchants *
.bot train *
.bot prep *
.bot food *
.bot potions *
.bot regs *
.bot ammo *

-- ======================================================================
-- Macro 3: Auto-Assemble Balanced 5-Man Group
-- Generates balanced party (1 Tank, 1 Healer, 3 DPS) matching level
-- ======================================================================
.bot group size=5
.bot summon *
.bot prep *

-- ======================================================================
-- Macro 4: Party / Raid Ready Check
-- Queries all bots for health, mana, consumable stock, and readiness
-- ======================================================================
/p ready check
/p c ?
```

### Category B: Live Group & Raid Combat Controls

```lua
-- ======================================================================
-- Macro 5: Main Tank Pull & Hold Aggro
-- Orders tank bot to charge/pull current target and hold threat
-- ======================================================================
/p tank attack

-- ======================================================================
-- Macro 6: All DPS Engage Target (Maximum Burst)
-- Orders all DPS bots and pets to burn offensive cooldowns on target
-- ======================================================================
/p attack
/p max dps

-- ======================================================================
-- Macro 7: Controlled Ranged Pull to Master
-- Orders ranged bot to shoot target and run back to group behind a corner
-- ======================================================================
/p pull
/p follow

-- ======================================================================
-- Macro 8: Focus Fire Raid Target Icon (Skull / Cross)
-- Sets target focus to skull or cross and commands focus attack
-- ======================================================================
/p rti skull
/p attack rti

-- Point focus to cross
/p rti cross
/p attack rti

-- ======================================================================
-- Macro 9: Emergency Panic Disengage & Flee to Master
-- Breaks combat, drops casting, and commands all bots to sprint to master
-- ======================================================================
/p flee
/p follow
.combatstop

-- ======================================================================
-- Macro 10: Area-of-Effect Burst Mode (AoE Trash Packs)
-- Toggles Blizzard, Rain of Fire, Volley, Whirlwind, Consecration
-- ======================================================================
/p co +aoe

-- ======================================================================
-- Macro 11: Mass Dispel & Cleanse Debuffs
-- Prioritizes curing magic, curses, poisons, and diseases
-- ======================================================================
/p co +cure,+cleanse cure magic,+cleanse cure disease,+cleanse cure poison
```

### Category C: Autonomous Dungeon Clear AI Deck (`.dc`)

```lua
-- ======================================================================
-- Macro 12: Activate Autonomous Dungeon Solver
-- Starts autonomous solver on tank bot, sets dungeon clear strategies
-- ======================================================================
.dc on
/p co +dungeon clear,avoid aoe,roll,loot,potions
/p nc +gather,food
/p formation near

-- ======================================================================
-- Macro 13: Cycle Pull Mode (Off / On / Dynamic)
-- Cycles pull mode: off -> on -> dynamic
-- ======================================================================
.dc pull

-- ======================================================================
-- Macro 14: Pause / Resume Autonomous Run
-- Pauses or resumes tank movement and pulls
-- ======================================================================
.dc pause

-- ======================================================================
-- Macro 15: Skip Current Trash Pack or Boss
-- Recalculates route around blocked trash or optional bosses
-- ======================================================================
.dc skip

-- ======================================================================
-- Macro 16: Instance Progression & Live Config Audit
-- Displays completion %, dead/alive bosses, and dumps active config
-- ======================================================================
.dc status
.dc bosses
.dc config
```

### Category D: Post-Combat Recovery, Looting & Breaches

```lua
-- ======================================================================
-- Macro 17: Post-Boss Loot, Drink & Rebuff
-- Vacuums corpse loot, eats/drinks to 100%, and refreshes class buffs
-- ======================================================================
/p loot
/p food
/p drink
/p buff

-- ======================================================================
-- Macro 18: Breach / Interact with Nearest Door, Gate, or Chest
-- Commands nearest bot to interact with target object
-- ======================================================================
/p u

-- ======================================================================
-- Macro 19: Order Bots to Repair Gear & Sell Junk at Vendor
-- Sells gray items and repairs 100% durability at targeted vendor NPC
-- ======================================================================
/p s
/p r

-- ======================================================================
-- Macro 20: Trade & Force Equip Best Gear on Bot
-- Opens trade window with targeted bot and commands upgrade evaluation
-- ======================================================================
/w %t t
/w %t e
```

---

## 2. Aura Buff Lists (`.aura`)

Commands apply to your currently selected target (`%t`):

### Raid-Wide & Stat Auras

```lua
-- Mark of the Wild (+285 Armor, +11 All Stats, +19 All Magic Resistance)
.aura 24752

-- Mark of the Wild (Rank 1: +25 Armor)
.aura 1126

-- Power Word: Fortitude (+54 Stamina)
.aura 10938

-- Prayer of Fortitude (+54 Stamina, 1 hour duration)
.aura 21564

-- Holy Word: Fortitude (+68 Stamina, 1 hour duration)
.aura 16873

-- Greater Blessing of Kings (+10% All Stats)
.aura 25898

-- Arcane Intellect (+31 Intellect)
.aura 10157

-- Divine Spirit (+50 Spirit)
.aura 27842

-- Greater Blessing of Wisdom (+33 Spirit / MP5)
.aura 25918

-- Power Infusion (+20% Spell Damage/Healing, -20% Mana Cost)
.aura 10060

-- Healing Done (+24 Healing)
.aura 49553

-- Increase Healing (+280 Healing, Permanent)
.aura 28151
```

### Attack Power, Armor & Tank Defensives

```lua
-- Battle Shout (+232 Attack Power)
.aura 25289

-- Battle Shout (Rank 1: +15 Attack Power)
.aura 6673

-- Greater Blessing of Might (+185 Attack Power)
.aura 25916

-- Greater Stoneshield (+2000 Armor)
.aura 17540

-- Stoneshield (+1000 Armor)
.aura 4941

-- Shield Wall (-75% Damage Taken, Warrior)
.aura 871

-- Barkskin (+3000 Armor, Druid Bear)
.aura 20655

-- Ice Barrier (438 Fire/Frost Absorb, Mage)
.aura 11426

-- Frost Armor (+200 Frost Resistance, +Armor)
.aura 7301

-- Mage Armor (+5 All Magic Resistance, 30% Mana Regen While Casting)
.aura 6117
```

### Dungeon & World Buffs

```lua
-- Mol'dar's Moxie (+15% Stamina, 2 hours)
.aura 22818

-- Fengus' Ferocity (+200 Attack Power, 2 hours)
.aura 22817

-- Slip'kik's Savvy (+3% Spell Crit, 2 hours)
.aura 22820

-- Rallying Cry of the Dragonslayer (+10% Spell Crit, +5% Melee/Ranged Crit, +140 AP, 2 hours)
.aura 22888

-- Warchief's Blessing (+300 Max Health, +15% Melee Haste, +10 MP5, 1 hour)
.aura 16609

-- Songflower Serenade (+15 All Stats, +5% Critical Strike, 1 hour)
.aura 15366

-- Blinding Light (+33 Spell Cast Speed, 20 seconds)
.aura 23733

-- Sayge's Dark Fortune of Strength (+10% Strength, 2 hours)
.aura 23735

-- Sayge's Dark Fortune of Agility (+10% Agility, 2 hours)
.aura 23736

-- Sayge's Dark Fortune of Stamina (+10% Stamina, 2 hours)
.aura 23737

-- Sayge's Dark Fortune of Intelligence (+10% Intellect, 2 hours)
.aura 23766

-- Sayge's Dark Fortune of Spirit (+10% Spirit, 2 hours)
.aura 23738

-- Sayge's Dark Fortune of Armor (+10% Armor, 2 hours)
.aura 23767

-- Sayge's Dark Fortune of Resistance (+25 All Magic Resistance, 2 hours)
.aura 23768

-- Sayge's Dark Fortune of Damage (+10% Damage All Schools, 2 hours)
.aura 23769
```

### Magic Resistances & Consumables

```lua
-- Fire Resistance (+60 Fire Resist)
.aura 27534

-- Nature Resistance (+60 Nature Resist)
.aura 27536

-- Frost Resistance (+60 Frost Resist)
.aura 27537

-- Shadow Resistance (+60 Shadow Resist)
.aura 27538

-- Arcane Resistance (+60 Arcane Resist)
.aura 27539

-- Chromatic Resistance (+50 All Magic Resist)
.aura 17629

-- Greater Fire Protection (+1950 to 2500 Fire Damage Absorb)
.aura 17543

-- Greater Nature Protection (+1950 to 2500 Nature Damage Absorb)
.aura 17544

-- Greater Frost Protection (+1950 to 2500 Frost Damage Absorb)
.aura 17545

-- Greater Shadow Protection (+1950 to 2500 Shadow Damage Absorb)
.aura 17546

-- Greater Arcane Protection (+1950 to 2500 Arcane Damage Absorb)
.aura 17548

-- Greater Holy Protection (+1950 to 2500 Holy Damage Absorb)
.aura 17549

-- Elixir of the Mongoose (+25 Agility, +2% Crit)
.aura 17538

-- Greater Arcane Elixir (+35 Spell Damage)
.aura 17539

-- Elixir of Superior Defense (+450 Armor)
.aura 11348

-- Elixir of Shadow Power (+40 Shadow Damage)
.aura 11474

-- Mageblood Potion (+12 MP5)
.aura 24363

-- Major Troll's Blood Potion (+20 HP5)
.aura 24361

-- Elixir of Fortitude (+120 Max Health)
.aura 3593

-- Elixir of Giants (+25 Strength)
.aura 11405

-- Gift of Arthas (+10 Shadow Resist, 8 Vulnerability to Attacker)
.aura 11371

-- Flask of the Titans (+1200 Max Health, 2 hours)
.aura 17626

-- Flask of Supreme Power (+150 Spell Damage, 2 hours)
.aura 17628

-- Flask of Distilled Wisdom (+2000 Max Mana, 2 hours)
.aura 17627

-- Flask of Petrification (Invulnerable, Cannot Move, 1 minute)
.aura 17624
```

### Food, Drink & Equipment Procs

```lua
-- Well Fed: Stamina & Spirit (+10 Stamina, +10 Spirit)
.aura 22730

-- Well Fed: Attack Power (+20 Attack Power)
.aura 25661

-- Well Fed: Spell Damage (+12 Spell Damage)
.aura 22731

-- Smoked Desert Dumplings (+20 Strength)
.aura 24799

-- Nightfin Soup (+8 MP5)
.aura 18194

-- Grilled Squid (+10 Agility)
.aura 18192

-- Runn Tum Tuber Surprise (+10 Intellect)
.aura 22734

-- Monster Omelet (+12 Stamina, +12 Spirit)
.aura 18191

-- Blessed Sunfruit (+10 Strength)
.aura 18141

-- Diamond Flask Proc (+150 Attack Power, +9 HP/s, 1 minute)
.aura 24427

-- Badge of the Swarmguard Proc (+Armor Penetration per hit)
.aura 26480

-- Earthstrike Proc (+280 Attack Power, 20 seconds)
.aura 25891

-- Kiss of the Spider Proc (+200 Attack Speed / Haste, 15 seconds)
.aura 28866

-- Slayer's Crest Proc (+280 Attack Power, 20 seconds)
.aura 28777

-- Jindo's Evil Eye Proc (+44 Healing, 20 seconds)
.aura 24584

-- Zandalarian Hero Charm Proc (+Damage and Healing per spell)
.aura 24659

-- Neltharion's Tear Proc (+Spell Damage)
.aura 23275

-- Restrained Essence of Sapphiron Proc (+130 Spell Damage, 20 seconds)
.aura 28779

-- Warmth of Forgiveness Proc (+Mana on cast)
.aura 28848

-- Fishliver Oil (+10% Melee Haste, 30 seconds)
.aura 7396

-- Remove all auras from target
.unaura all
```

### Instant Potions & Heals (Use `.cast`, NOT `.aura`)

```lua
-- Major Healing Potion (Restores 1050 to 1750 Health)
.cast 17534

-- Major Mana Potion (Restores 1350 to 2250 Mana)
.cast 17531

-- Combat Healing Potion
.cast 2337

-- Major Healthstone (Restores 1200 Health)
.cast 11732
```

---

## 3. Target Tokens & Chat Communication Scope

### Command Scopes & Routing

```lua
-- Whisper a single bot by name
/w Tankbot follow
.bot w Tankbot follow

-- Whisper your currently targeted bot
/w %t follow
/w %t co +tank

-- Send order to ALL bots in your raid (every bot evaluates filters)
/ra follow
.bot r follow
.bot r nc +food

-- Send order to bots in your party (current subgroup only in a raid)
/p follow
.bot p follow
```

### Chat Filtering Syntax (`@filter`)

```lua
-- Target by Role
/ra @tank co +protection
/ra @heal co +heal
/ra @dps co +aoe
/ra @melee formation melee
/ra @ranged formation far

-- Inverted Role Filter
/ra @notank flee
/ra @noheal attack
/ra @nodps save mana

-- Target by Class
/ra @warrior co +fury
/ra @paladin co +holy
/ra @priest co +heal
/ra @druid co +bear
/ra @shaman co +totems
/ra @rogue co +stealth
/ra @hunter co +ranged
/ra @mage co +blizzard
/ra @warlock co +shadow

-- Target by Raid Target Icon (RTI)
/p @skull attack
/p @cross attack
/p @square cc
/p @moon cc
/p @triangle cc
/p @diamond cc
/p @circle attack
/p @star attack

-- Target by Subgroup (No space between group and number)
/ra @group1 formation shield
/ra @group2 formation line
/ra @group1-2 follow

-- Target by Exact Level or Range
/ra @60 follow
/ra @58-60 prep

-- Target by Strategy Condition
/ra @co=tank attack
/ra @noco=flee co +aoe
/ra @nc=food follow
/ra @nonc=food nc +food
```

---

## 4. Playerbot Core Control Suite (`.bot`)

Operate party bots, account alternate characters, and character templates:

```lua
-- List all active bots in world memory (optional prefix filter)
.bot list
.bot list Priest

-- Display built-in help for bot commands
.bot help
.bot help gear

-- Reload aiplayerbot.conf configuration in memory
-- NOTE: Requires Security Level 2 (Moderator)
.bot reload

-- Toggle self-bot automation mode on your player character
.bot self login
.bot self logout

-- Auto-Generate balanced party matching your level (size=5 default)
.bot group size=5 level=60

-- Generate fresh bot character with explicit parameters
-- NOTE: role= is required for proper talent selection
.bot create name=Dunpriest class=priest race=dwarf level=60 role=healer login=1

-- Add bot to party by character name or account name
.bot add Dunpriest

-- Add ALL alternate characters from an entire account at once
.bot add sam_tanks
.bot add sam_healers
.bot add sam_melee
.bot add sam_ranged

-- Add all bots from current account
.bot add *

-- Remove specific bot or all bots in party
.bot remove Dunpriest
.bot remove *

-- Permanently delete bot character from database
.bot delete Stuckbot

-- Teleport bots to your current position
-- NOTE: Bots must be on the same map ID (cannot pull cross-instance)
.bot summon Dunpriest
.bot summon *

-- Set character level on bot or all party bots
.bot level Dunpriest 60
.bot level * 60

-- Generate and equip level-scaled gear on bots
-- Accepted qualities: green, uncommon, blue, rare, purple, epic, upgrade, sync, best, partial
.bot gear * best
.bot gear * epic
.bot gear Dunpriest rare

-- Apply optimal permanent enchantments to equipped armor and weapons
.bot enchants *
.bot enchants Dunpriest

-- Learn class level spells on bots
.bot train *
.bot train Dunpriest

-- Refresh bot factory state
.bot prep *

-- Stock consumables, potions, reagents, food, and ammo in bot bags
.bot consumes *
.bot potions *
.bot regs *
.bot food *
.bot ammo *

-- Summon, feed, and train combat pet for Hunter or Warlock
.bot pet *

-- Randomize bot talents, appearance, and non-combat characteristics
.bot random *

-- Set persistent group size control (Space required before number)
-- Accepted sizes: -1, 0, 1, 2, 3, 4, 5, 20, 40
.bot set value group size 20

-- Execute immediate C++ action on bot (single-word only)
.bot do Dunpriest attack
.bot do Dunpriest follow
```

---

## 5. RandomBot Population & Realm AI Manager (`.rndbot`)

Manage autonomous open-world population, server lag tuning, and realm simulation:

```lua
-- Telemetry: View realm bot count, levels, races, classes, and roles (In-game only)
.rndbot stats

-- Telemetry: View and tune tick latency metrics (Avg diff, Max diff, sessions)
.rndbot diff
.rndbot diff 100 200

-- PID Controller: Tune real-time bot governor (Note trailing space after 'pid')
.rndbot pid 
.rndbot pid  0.05 0.001 0.05

-- Force immediate AI update tick across realm (evaluates zones, quests, logins)
.rndbot update

-- Toggle verbose console logging for bot login queue
.rndbot login debug

-- Unload unreferenced map geometry and VMaps to free server memory
.rndbot clean map

-- Wipe persistent random bot records from database
.rndbot reset

-- Re-initialize random bots worldwide, rerolling specs and gear brackets
.rndbot init %

-- Upgrade equipment and spells on all random bots matching their level
.rndbot upgrade %

-- Soft-reboot bots, clearing combat deadlocks and broken AI states
.rndbot refresh *
.rndbot refresh %

-- Reroll active combat and non-combat strategies across random bots
.rndbot change_strategy %

-- Dispatch idle bots to level-appropriate zones to farm mobs for XP/gold
.rndbot grind %
.rndbot grind Dunpriest

-- Dispatch bots to inns, cities, and trainers to live autonomous RPG life
.rndbot rpg %
.rndbot rpg Dunpriest

-- Instantly resurrect dead or ghost bots worldwide at full health and mana
.rndbot revive *
.rndbot revive %

-- Teleport bot(s) to randomized level-appropriate world zones
.rndbot teleport %
.rndbot teleport Dunpriest
```

---

## 6. In-Game Whispers & Single-Letter Shortcuts

Send directly to a bot via `/w %t <cmd>` or `/w <botname> <cmd>`:

### Fast Single-Letter Shortcuts

```lua
-- Inspect bot inventory (equipped items, free slots, gold, durability %)
/w %t c
/w %t items
/w %t inv

-- Equip highest stat item upgrades from inventory bags
/w %t e
/w %t equip

-- Unequip item from equipment slot into inventory bags
/w %t ue

-- Mark item in bags to never be auto-vendored or destroyed
/w %t keep [Item Link]

-- Drop specified quest or destroy item
/w %t drop [Quest/Item Link]

-- Sell all gray trash items at targeted vendor NPC
/w %t s

-- Buy item from vendor NPC
/w %t b [Item Link]

-- Buy back accidentally sold item from vendor buyback tab
/w %t bb [Item Link]

-- Repair 100% equipment durability at targeted vendor NPC
/w %t r
/w %t repair

-- Initiate secure trade transaction window between player and bot
/w %t t

-- Cancel active trade window
/w %t nt

-- Dump bot quest log, active objectives, and completion percentages
/w %t q
/w %t quests

-- Set loot filtering strategy (normal, gray, disenchant, all)
/w %t ll all

-- Dump list of blacklisted or skipped spells on bot
/w %t ss
```

### Complete Whisper Command Directory

```lua
-- Follow master or designated player
/w %t follow

-- Hold ground at current coordinates
/w %t stay

-- Break combat, cancel casting, and sprint back to master
/w %t flee

-- Engage current target with damage abilities and pets
/w %t attack

-- Order tank bot to charge/pull and establish threat
/w %t tank attack

-- Order ranged bot to shoot target and run back to group
/w %t pull

-- Vacuum all nearby corpse loot
/p loot

-- Roll on group loot prompt (need, greed, pass)
/p roll need
/p roll greed
/p roll pass

-- Sit and consume food/water until 100% resources
/p food
/p drink

-- Cast class buffs on all group members
/p buff

-- Toggle conservative mana conservation behavior
/w %t save mana

-- Burn all offensive DPS cooldowns and trinkets
/p max dps

-- Bind bot hearthstone to current innkeeper
/w %t home

-- Accept pending quest, trade, or resurrection prompt
/w %t accept

-- Release spirit to graveyard when dead
/w %t release

-- Navigate ghost bot back to corpse and resurrect
/w %t corpse run

-- Release spirit and immediately respawn at graveyard
/w %t repop

-- Board flight path taxi node by node ID
/w %t taxi 45

-- Blacklist specific NPC entry ID from bot navigation pathing
/w %t avoid creature 10184

-- Share quest with party members
/w %t share [Quest Link]

-- Abandon completed or obsolete quests from bot log
/w %t clean quest log

-- Query whether bot has direct line of sight to target
/w %t los

-- Toggle Real-Time Strategy Control (click-to-move squad mode)
/w %t rtsc

-- Equip Turtle-WoW custom class glyph on bot
/w %t glyph 12

-- Save or equip named equipment loadout
/w %t outfit save pvp
/w %t outfit load pvp

-- Dump list of learned skills and ratings
/w %t skill

-- Order bot to craft linked recipe item from inventory materials
/w %t craft [Lesser Healing Potion]

-- Purchase all available abilities from targeted trainer NPC
/w %t trainer learn

-- Transfer group leadership
/w %t give leader
```

### Bot Cheats Subsystem (`/w %t cheat`)

Prefix operators: `?` (query), `+` (enable), `-` (disable), `~` (toggle):

```lua
-- Query active cheats on bot
/w %t cheat ?

-- Enable individual cheats
/w %t cheat +health       -- Infinite health (cannot die)
/w %t cheat +mana         -- Infinite mana pool
/w %t cheat +power        -- Infinite rage, energy, and mana
/w %t cheat +cooldown     -- Zero spell and ability cooldowns
/w %t cheat +movespeed    -- Accelerated run, walk, and swim speeds
/w %t cheat +attackspeed  -- Massive melee, ranged, and spell haste
/w %t cheat +gold         -- Free gold for training and repairs
/w %t cheat +repair       -- 100% durability at all times
/w %t cheat +taxi         -- All flight paths unlocked
/w %t cheat +item         -- Bypasses consumable reagent requirements
/w %t cheat +breath       -- Infinite underwater breathing
/w %t cheat +glyph        -- Bypasses class glyph limitations
/w %t cheat +quest        -- Auto-completes accepted quest objectives

-- Disable individual cheats
/w %t cheat -health
/w %t cheat -cooldown

-- Multi-cheat combination macro for party bots
/p cheat +health,+mana,+cooldown,+movespeed,+repair
```

---

## 7. Chat Filter Syntax & Targeting Token Engine (`@filter`)

Route orders in `/p`, `/ra`, or `/g` directly to specific subsets of bots:

```lua
-- Role Filters
/ra @tank attack
/ra @notank flee
/ra @heal drink
/ra @noheal save mana
/ra @dps max dps
/ra @nodps flee
/ra @melee formation melee
/ra @ranged formation far

-- Raid Target Icon (RTI) Filters
/p @skull attack
/p @cross attack
/p @square cc
/p @moon cc
/p @triangle cc
/p @diamond cc
/p @circle attack
/p @star attack

-- Class Filters
/ra @warrior formation shield
/ra @paladin co +holy
/ra @hunter co +ranged
/ra @rogue co +stealth
/ra @priest co +heal
/ra @shaman co +totems
/ra @mage cast blizzard
/ra @warlock cast curse of elements
/ra @druid co +bear

-- Subgroup Filters (No space allowed)
/ra @group1 formation shield
/ra @group2 formation line
/ra @group1-4 follow

-- Level Filters
/ra @60 follow
/ra @58-60 prep

-- Strategy State Filters
/ra @co=tank attack
/ra @noco=flee co +aoe
/ra @nc=food follow
/ra @nonc=food nc +food
```

---

## 8. Formations & Stances Tactical Directory

### Formations (`/p formation <name>`)

Determines where bots position relative to master when following:

```lua
-- near / default: Tight 2-4 yard cluster around master (doors, tight corridors)
/p formation near

-- queue: Single-file line directly behind master (bridges, narrow paths)
/p formation queue

-- chaos: Randomized dispersion around master (anti-AoE spacing)
/p formation chaos

-- circle: 360-degree equidistant circular ring surrounding master (defense)
/p formation circle

-- line: Horizontal wall spread perpendicular to master facing (caster line)
/p formation line

-- shield: Tanks/Melee in front, healers and casters in rear row (boss progression)
/p formation shield

-- arrow: Wedge / spearhead formation with tank at tip (open-field pulls)
/p formation arrow

-- melee: Clustered tightly within 2 yards behind target back hitbox (melee DPS)
/p formation melee

-- far: Maximum 30-35 yard range circle from target/master (hunters, casters)
/p formation far

-- Display active formation name
/p formation show

-- Reset formation to default
/p formation reset

-- Query current formation status
/p formation ?
```

### Stances (`/w %t stance <name>`)

Controls combat positioning relative to active targets:

```lua
-- near / default: Standard combat range facing target
/w %t stance near

-- tank: Positions in front of mob and turns boss away from group
/w %t stance tank

-- turnback / turn: Actively rotates mob 180 degrees away from raid party
/w %t stance turnback

-- behind: Maneuvers continuously behind target back hitbox (Rogues, Cat Druids)
/w %t stance behind

-- Display active stance name on targeted bot
/w %t stance show

-- Query stance status
/w %t stance ?
```

---

## 9. Autonomous Dungeon Clear AI Engine (`.dc`)

Drives the group Tank Bot through optimal boss paths and solves dungeon gates:

```lua
-- Start autonomous dungeon clear on group tank bot
.dc on

-- Stop autonomous solver and return all bots to following player
.dc off

-- Soft-pause or resume tank movement and pulls
.dc pause

-- Cycle pull mode: off -> on -> dynamic
.dc pull

-- Skip current objective/pack and recalculate route
.dc skip

-- Dump current run status: duration, boss count, alive/dead state, party readiness
.dc status

-- List all bosses in current instance and their kill bitmask
.dc bosses

-- Override route to path directly to designated boss name
.dc go <boss_name>

-- Dump all live DungeonClear configuration variables
.dc config

-- Free-flying spectator camera controls
.dc spectate
.dc spectate free
.dc spectate follow <botname>
.dc spectate next
.dc spectate prev
.dc spectate list
```

### Automated 5-Bot Test Harness (`.dc test`)

GM-only test suite to verify autonomous clearance:

```lua
-- List all supported test dungeons and level brackets
.dc test list

-- Display recommended item level ladders for dungeon
.dc test gear <dungeon> [heroic]

-- Start autonomous 5-bot test run with random party comp
.dc test start <dungeon> [heroic]

-- Start test run with specific named bot roster
.dc test start <dungeon> party=Tank,Healer,Dps1,Dps2,Dps3

-- Replay exact test run with random seed
.dc test start <dungeon> seed=12345

-- Display live duration, boss kills, and wipe counter
.dc test status

-- Abort active test run or all campaigns
.dc test stop
.dc test stop all

-- GM Spectator Watch Mode
.dc test watch
.dc test watch next
.dc test watch off

-- Automated Batch Campaigns (total=N is mandatory)
.dc test plan start blackrock-depths total=10 concurrent=2 level=60 seed=12345
.dc test plan status
.dc test plan stop all
```

---

## 10. Raid Automation Operator Facade (`.raid`)

Eluna operator facade (`run/lua_scripts/raid_automation.lua` v1.4.0):

### Layer 1: BATTLE

```lua
-- Sequence dc bosses -> dc pull -> dc on -> dc status
.raid start
.raid start off
.raid start on
.raid start dynamic

-- Halt autonomous run
.raid stop

-- Explicit pause / resume
.raid pause
.raid resume

-- Path directly to boss
.raid go <boss_name>

-- Skip current pack
.raid skip

-- Set pull mode
.raid pull off
.raid pull on
.raid pull dynamic

-- Diagnostic inspection
.raid status
.raid bosses
.raid config
.raid spectate
```

### Layer 2: ROSTER

```lua
-- Wrap bot group creation
.raid group 20
.raid group 40

-- Split formations by role and activate DPS focus lock
.raid roles

-- Full gear, enchants, consumables, attunements, and master keys
.raid ready
.raid ready epic

-- Equip gear quality
.raid gear best
.raid gear epic

-- Auto-complete raid attunements
.raid attune

-- Promote tank-capable bots
.raid tanks 3

-- Assign specific tank slots
.raid tank2 Tankbot
.raid tank3 Offtank

-- Raid Target Icon mark assignments
.raid marks
.raid marks Tankbot skull
.raid unmark
.raid clearmarks
.raid ccmark
.raid skull

-- Diagnostic strategy and status checks
.raid check
.raid rc
.raid gearscore
.raid stats
.raid spells
.raid who
.raid where
.raid list

-- Avoid patrol mob entry ID
.raid avoidmob 10184
.raid avoidmob -10184
```

### Layer 3: TOGGLES

```lua
-- AoE Abilities toggle
.raid aoe
.raid noaoe

-- DPS Target Focus lock toggle
.raid focus
.raid unfocus

-- Looting toggle
.raid loot
.raid noloot

-- Rest (eat/drink) breaks toggle
.raid rest
.raid norest

-- Ground Hazard Avoidance toggle
.raid avoid
.raid noavoid

-- Loot Roll prompt handling toggle
.raid roll
.raid noroll

-- Gathering tradeskill nodes toggle
.raid gather
.raid nogather
```

### Layer 4: MOVE

```lua
-- Teleport all raid bots directly to you (hard summon)
.raid summon
.raid rally

-- Order all raid bots to walk to you (soft regroup)
.raid follow

-- Teleport raid to named destination
.raid tele MoltenCore
.raid tele bwl

-- Clear raid instance lockouts
.raid unbind
.raid resetlock
```

### Layer 5: PASS

```lua
-- Broadcast command to all raid bots with optional filter
.raid all co +aoe
.raid all @frost co +aoe

-- Send command to single named bot
.raid one Tankbot co +tank

-- Pass through to underlying .dc command
.raid dc status
```

---

## 11. Class Combat Strategy Assignment Deck (`/w %t`)

Target a bot (`%t`) and click these macros to set their permanent combat (`co`) and non-combat (`nc`) strategies:

### Tank Specializations

```lua
-- Protection Warrior (Main Tank)
/w %t co -dps,-fury,-arms,+tank,+protection,+threat,+potions,+avoid aoe
/w %t nc +food
/w %t formation near
/w %t stance tank

-- Protection Paladin (AoE & Undead Tank)
/w %t co -dps,-heal,-holy,-retribution,+tank,+protection,+consecration,+threat,+potions
/w %t nc +food,+buff
/w %t formation near
/w %t stance tank

-- Feral Bear Druid (Armor & HP Tank)
/w %t co -dps,-cat,-heal,+tank,+bear,+threat,+avoid aoe,+potions
/w %t nc +food,+buff
/w %t formation near
/w %t stance tank
```

### Healer Specializations

```lua
-- Holy Priest (Raid & Tank Healer)
/w %t co -dps,-shadow,+heal,+holy,+cure,+save mana,+potions
/w %t nc +food,+buff
/w %t formation line

-- Restoration Shaman (Chain Healer & Totems)
/w %t co -dps,-elemental,-enhancement,+heal,+totems,+mana spring totem,+healing wave,+cure
/w %t nc +food,+buff
/w %t formation line

-- Restoration Druid (HoT & Tank Support)
/w %t co -dps,-cat,-bear,+heal,+cure,+rejuvenation,+save mana
/w %t nc +food,+buff
/w %t formation line

-- Holy Paladin (Single-Target Tank Healer)
/w %t co -dps,-protection,-retribution,+heal,+holy,+blessing of wisdom,+blessing of salvation,+potions
/w %t nc +food,+buff
/w %t formation line
```

### Melee DPS Specializations

```lua
-- Combat Rogue (Behind Flank DPS)
/w %t co -tank,+dps,+behind,+kick,+stealth,+evasion,+potions
/w %t nc +food,+stealth
/w %t formation melee
/w %t stance behind

-- Fury Warrior (Dual-Wield Melee DPS)
/w %t co -tank,-protection,+dps,+fury,+behind,+pummel,+potions
/w %t nc +food
/w %t formation melee
/w %t stance behind

-- Retribution Paladin (2-Handed Melee DPS)
/w %t co -tank,-heal,+dps,+retribution,+consecration,+potions
/w %t nc +food,+buff
/w %t formation melee

-- Enhancement Shaman (Windfury Melee DPS)
/w %t co -heal,+dps,+enhancement,+windfury totem,+totems,+cc
/w %t nc +food,+buff
/w %t formation melee

-- Feral Cat Druid (Behind Bleed DPS)
/w %t co -tank,-heal,+dps,+cat,+behind,+stealth,+potions
/w %t nc +food,+stealth
/w %t formation melee
/w %t stance behind
```

### Ranged & Caster DPS Specializations

```lua
-- Marksmanship Hunter (Ranged DPS & Pet)
/w %t co -tank,+dps,+ranged,+pet,+potions
/w %t nc +food
/w %t formation far

-- Fire Mage (Maximum Burst DPS)
/w %t co -tank,+dps,+fire,+aoe,+cc,+potions
/w %t nc +food,+buff
/w %t formation line

-- Frost Mage (Control & AoE DPS)
/w %t co -tank,+dps,+frost,+aoe,+cc,+potions
/w %t nc +food,+buff
/w %t formation line

-- Affliction / Destruction Warlock (Shadow DPS & Pet)
/w %t co -tank,+dps,+shadow,+pet,+curse,+potions
/w %t nc +food,+buff
/w %t formation line

-- Shadow Priest (Shadow Caster DPS)
/w %t co -tank,-heal,+dps,+shadow,+shadowform,+cure,+potions
/w %t nc +food,+buff
/w %t formation line

-- Balance Druid (Boomkin Caster DPS)
/w %t co -tank,-heal,-bear,-cat,+dps,+moonkin form,+cure,+potions
/w %t nc +food,+buff
/w %t formation line
```

---

## 12. Raid Assembly, Multi-Account Alt Bots & Lockouts

### Multi-Account Alt Bot Assembly Pipeline

Field a complete 40-man raid roster using your own custom characters across dedicated alt accounts:

```bash
# 1. Create dedicated alt accounts in server console or in-game
.account create sam_tanks password
.account create sam_healers password
.account create sam_melee password
.account create sam_ranged password
```

```lua
-- 2. Log into main character and summon entire account rosters at once
.bot add sam_tanks
.bot add sam_healers
.bot add sam_melee
.bot add sam_ranged

-- 3. Teleport and fully prepare the raid
.bot summon *
.raid roles
.raid ready

-- 4. Inspect or clear raid lockouts
.instance unbind all
.instance groupunbind all
.instance stats
```

---

## 13. Blackwing Lair Encounter Strategies

### Boss Encounter Sequence (Map 469)

```lua
-- 1. Razorgore the Untamed (NPC 12435) -> Door 176965 opens on death
-- Tanks hold Dragonspawn away; destroy 30 eggs; taunt on Conflagration threat wipe
/p @tank co +threat
/p @dps co +potions

-- 2. Vaelastrasz the Corrupt (NPC 13020) -> Door 179364 opens on death
-- Essence of the Red grants infinite resources; MT runs out on Burning Adrenaline (23620)
-- Offtank immediately taunts; all DPS stand at flanks
/p max dps
/w Offtank taunt vaelastrasz

-- 3. Suppression Room & Broodlord Lashlayer (NPC 12017) -> Door 179365 opens on death
-- Rogues stealth and disarm suppression devices (GO 179784); MT flat against wall
/p @rogue co +stealth
/w Offtank taunt broodlord

-- 4. Firemaw (NPC 11983)
-- All tanks must wear Onyxia Scale Cloak (Item 15138); non-tanks hide at >= 3 Flame Buffet stacks
-- Offtank taunts on Wing Buffet threat wipe
/w Offtank taunt firemaw

-- 5. Ebonroc (NPC 14601)
-- Shadow of Ebonroc (23340) heals boss for 25,000 per hit if not swapped!
-- Offtank must taunt immediately when MT receives debuff
/w Offtank taunt ebonroc

-- 6. Flamegor (NPC 11981)
-- Hunters instantly cast Tranquilizing Shot (Spell 19801) on Frenzy (23342)
-- Offtank taunts on Wing Buffet
/p @hunter cast tranquilizing shot

-- 7. Chromaggus (NPC 14020) -> Lever 179148 opens Gate 179116
-- Cleanse 5 Brood Afflictions (Curse 23154, Magic 23153, Disease 23155, Poison 23169, Bronze 23170)
-- Bronze cured via Hourglass Sand (Item 19183); burn offensive cooldowns at 20% enrage
/p co +cleanse cure magic,+cleanse cure disease,+cleanse cure poison

-- 8. Nefarian (NPC 11583) -> Throne Gate 176966
-- Phase 1: AoE 42 drakonids at South/North doors
-- Phase 2: Landing Shadowflame requires Onyxia Scale Cloak; counter class calls
-- Phase 3: AoE bone constructs at 20% health while burning Nefarian
/p co +aoe
/p max dps
```

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md) • [🏰 Volume 06: Raid Operations](./06_automated_raid_and_dungeon_operations.md)
