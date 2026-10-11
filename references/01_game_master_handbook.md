# 01. Game Master & World Administration Handbook

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS Core Engine  
> **Security Coverage**: Security Levels 0–6 (Player, Observer, Moderator, Developer, Administrator, SigMaChaD, Console)  
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📋 Operations (commands.md)](../commands.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md) • [🏰 Volume 06: Raid Operations](./06_automated_raid_and_dungeon_operations.md) • [🛡️ Volume 07: Guilds](./07_guilds_and_guild_operations.md) • [🌍 Volume 08: World & Strategies](./08_open_world_control_and_strategies.md)

---

## Table of Contents

1. [Categorized In-Game Lua Macros (Top Priority)](#1-categorized-in-game-lua-macros-top-priority)
2. [Character Progression, Training & Skills](#2-character-progression-training--skills)
3. [Combat, Spells, Auras & Cheats](#3-combat-spells-auras--cheats)
4. [Character Stats, Appearance, Morphs & Factions](#4-character-stats-appearance-morphs--factions)
5. [Items, Bags, Economy & Remote Mail](#5-items-bags-economy--remote-mail)
6. [Accounts, Security Ranks & Permissions](#6-accounts-security-ranks--permissions)
7. [World Spawning, NPCs & GameObject Editing](#7-world-spawning-npcs--gameobject-editing)
8. [Quests, Instances, Events & Lockouts](#8-quests-instances-events--lockouts)
9. [Server Management, Performance & Backups](#9-server-management-performance--backups)
10. [Moderation, Tickets, Bans & Anticheat](#10-moderation-tickets-bans--anticheat)
11. [Turtle-WoW 1.18.1 Custom Systems & Vanity Features](#11-turtle-wow-1181-custom-systems--vanity-features)
12. [Developer Diagnostics, Navmesh & Waypoints](#12-developer-diagnostics-navmesh--waypoints)
13. [Complete Database Hot-Reload Catalog (.reload)](#13-complete-database-hot-reload-catalog-reload)
14. [Auction House Bot Administration (.ahbot)](#14-auction-house-bot-administration-ahbot)

---

## 1. Categorized In-Game Lua Macros (Top Priority)

### Category A: Character Progression, Boost & Survival

```lua
-- ======================================================================
-- Macro 1: Complete Level 60 Instant Boost Suite
-- Sets level 60, learns class spells, trainer recipes, maxes weapon/defense skills,
-- learns all secondary professions, grants 1,000 Gold, 4x 18-slot bags, reveals maps/taxi
-- ======================================================================
.levelup 60
.learn all_myspells
.learn all_trainer
.maxskill
.learn all_recipes first aid
.learn all_recipes cooking
.learn all_recipes fishing
.learn all_recipes survival
.modify money 10000000
.additem 14156 4
.taxicheat on
.explorecheat on
.repairitems
.replenish

-- ======================================================================
-- Macro 2: Panic Survival & Instant God Mode
-- Toggles invulnerability, drops all threat, clears cooldowns, replenishes pools
-- ======================================================================
.god on
.combatstop
.cooldown
.replenish

-- ======================================================================
-- Macro 3: Fast Travel Flight & Forward Blink
-- Enables hover flight, 250% movement speed, blinks 20 yards forward, 15 yards up
-- ======================================================================
.hover on
.modify speed 2.5
.go forward 20
.go up 15

-- ======================================================================
-- Macro 4: Normal Mobility & God Mode Reset
-- Disables hover, resets speed to 1.0, turns off god mode
-- ======================================================================
.hover off
.modify speed 1.0
.god off

-- ======================================================================
-- Macro 5: Remote Bank & Mail Access Anywhere
-- Opens personal bank vault and remote mailbox from anywhere in the world
-- ======================================================================
.bank
.mailbox
```

### Category B: GM Stealth Patrol, Admin Duty & Server Maintenance

```lua
-- ======================================================================
-- Macro 6: GM Invisible Duty & Silent Patrol
-- Enables GM tag, turns invisible, enables flight, silences public GM chat
-- ======================================================================
.gm on
.gm visible off
.gm fly on
.gm chat off

-- ======================================================================
-- Macro 7: Hotfix Core Reload Suite
-- Flushes and re-reads world, creatures, items, gameobjects, and Eluna scripts
-- ======================================================================
.reload quest_template
.reload creature_template
.reload item_template
.reload creature
.reload gameobject
.reload eluna

-- ======================================================================
-- Macro 8: Realm Save & Graceful Restart (5-Minute Warning)
-- Saves all player/guild data and schedules clean 5-minute restart
-- ======================================================================
.saveall
.broadcast Server restart in 5 minutes for maintenance. Please finish encounters and log out safely.
.server restart 300 Server restart in 5 minutes for maintenance.

-- ======================================================================
-- Macro 9: Emergency Raid Save Reset
-- Forces immediate server-wide reset of all raid instances and player lockouts
-- ======================================================================
.server resetallraids
```

---

## 2. Character Progression, Training & Skills

Commands apply to your player or your currently selected target (`%t`):

```lua
-- Increase character level by specified amount (Sec 3)
.levelup 60

-- Teach all spells for player's class (Sec 3)
.learn all_myspells

-- Teach all trainer abilities for player's class (Sec 3)
.learn all_trainer

-- Teach all default items (Sec 3)
.learn all_items

-- Teach specific spell ID (Sec 3)
.learn 133

-- Teach all recipes and max skill for a specific profession (Sec 3)
.learn all_recipes first aid
.learn all_recipes cooking
.learn all_recipes fishing
.learn all_recipes survival

-- Remove specific spell from spellbook (Sec 3)
.unlearn 133

-- Remove spell from an offline player character (Sec 3)
.unlearnoffline Bob 133

-- Advance learned weapon and defense combat skills to 300 (Sec 3)
-- NOTE: Skips primary professions, secondary skills, and riding
.maxskill

-- Set specific skill ID value and max capacity (Sec 3)
-- Syntax: .setskill <skill_id> <current> <max>
.setskill 26 300 300

-- Query current value and max capacity of specified skill (Sec 3)
.getskillvalue 26

-- Temporarily unlock all flight paths across the world (Sec 3)
.taxicheat on
.taxicheat off

-- Permanently write all flight path taxi nodes into character database (Sec 3)
.character fillflys

-- Reveal world map fog across all zones (Sec 3)
-- NOTE: Use '1' or 'on'
.explorecheat 1

-- Restore all equipped and inventory gear to 100% durability at zero cost (Sec 3)
.repairitems

-- Instantly refill Health, Mana, Rage, and Energy pools to 100% (Sec 3)
.replenish

-- Reset all active spell cooldowns or a specific ability cooldown (Sec 3)
.cooldown
.cooldown 133

-- Reset talent points, learned spells, base stats, honor, or level (Sec 3)
.reset talents
.reset spells
.reset stats
.reset honor
.reset level

-- Audit inventory and equipment for item anomalies or duplicate uniques (Sec 1)
.character diffitems

-- Dump item acquisition, trade, and vendor history for character (Sec 3)
.character itemlog

-- Inspect account/character activity timestamp and inactivity thresholds (Sec 1)
.character inactivity

-- Force instant player rescue teleport to bound home tavern (Sec 2)
.unstuck
```

### Secondary Skills (All Players Can Learn All 4)

```lua
-- First Aid (Skill ID: 129, Apprentice Spell: 3273)
.learn all_recipes first aid
.setskill 129 300 300

-- Survival (Turtle Custom, Skill ID: 142, Apprentice Spell: 51296)
.learn all_recipes survival
.setskill 142 300 300

-- Cooking (Skill ID: 185, Apprentice Spell: 2550)
.learn all_recipes cooking
.setskill 185 300 300

-- Fishing (Skill ID: 356, Apprentice Spell: 7620)
.learn all_recipes fishing
.setskill 356 300 300

-- Riding (Skill ID: 762)
.setskill 762 300 300
```

### Primary Professions (Choose Up to 2)

```lua
-- Alchemy (Skill ID: 171, Apprentice Spell: 2259)
.learn all_recipes alchemy
.setskill 171 300 300

-- Blacksmithing (Skill ID: 164, Apprentice Spell: 2018)
.learn all_recipes blacksmithing
.setskill 164 300 300

-- Enchanting (Skill ID: 333, Apprentice Spell: 7411)
.learn all_recipes enchanting
.setskill 333 300 300

-- Engineering (Skill ID: 202, Apprentice Spell: 4036)
.learn all_recipes engineering
.setskill 202 300 300

-- Herbalism (Skill ID: 182, Apprentice Spell: 2366)
.learn all_recipes herbalism
.setskill 182 300 300

-- Leatherworking (Skill ID: 165, Apprentice Spell: 2108)
.learn all_recipes leatherworking
.setskill 165 300 300

-- Mining (Skill ID: 186, Apprentice Spell: 2575)
.learn all_recipes mining
.setskill 186 300 300

-- Skinning (Skill ID: 393, Apprentice Spell: 8613)
.learn all_recipes skinning
.setskill 393 300 300

-- Tailoring (Skill ID: 197, Apprentice Spell: 3908)
.learn all_recipes tailoring
.setskill 197 300 300

-- Lockpicking (Rogue Class Skill, Skill ID: 633)
.setskill 633 300 300
```

---

## 3. Combat, Spells, Auras & Cheats

```lua
-- Total damage immunity and status effect invulnerability (Sec 3)
.god on
.god off

-- Drop combat state, clear threat lists, and force hostile mobs to reset (Sec 2)
.combatstop

-- Resurrect targeted corpse with 100% health and mana, clearing sickness (Sec 3)
.revive

-- Instantly kill targeted unit, bypassing armor and defensive cooldowns (Sec 3)
.die

-- Cast specified spell ID directly from your character onto target (Sec 3)
-- Syntax: .cast <spell_id> [triggered]
.cast 133

-- Force target or self to cast specified spell on self (Sec 3)
.cast self 20549

-- Cast specified spell targeting current target (Sec 3)
.cast target 133

-- Fire spell with custom effect bitmask override (Sec 3)
.castcustom 133 1

-- Deal specified flat damage value to current target (Sec 3)
.damage 5000

-- Deal damage to all hostile units within designated yard radius (Sec 3)
-- Syntax: .aoedamage <amount> [radius]
.aoedamage 10000 30

-- Apply positive or negative spell aura to targeted character/NPC (Sec 3)
.aura 15366

-- Remove active aura effect by spell ID (Sec 3)
.unaura 15366

-- Force target to run in panic fear (Sec 3)
.fear on
.fear off

-- Paralyze target unit completely (movement, casting, chat) (Sec 2)
.freeze

-- Remove paralysis from target unit (Sec 2)
.unfreeze

-- Enable traversing water surfaces without sinking (Sec 3)
.waterwalk on
.waterwalk off

-- Dismount targeted player or bot from their mount (Sec 3)
.dismount
```

### Engine Cheat System (`.gm options`)

```lua
-- View currently active engine cheat flags (Sec 4)
.gm options

-- Enable all core engine cheats:
-- cd        = bypass spell and global cooldowns
-- cast      = instant casting (bypasses cast time)
-- power     = zero resource cost (infinite mana/rage/energy)
-- crit      = 100% critical hit chance
-- proc      = 100% weapon/talent proc chance
-- checkcast = bypass line-of-sight and range checks
-- speed     = unrestricted engine movement speed
.gm options cd cast power crit proc checkcast

-- Disable specific engine cheat flags
.gm options cd cast power crit proc checkcast off

-- Full Engine God Mode combo
.gm options cd cast power crit proc checkcast
.god on
.replenish
```

---

## 4. Character Stats, Appearance, Morphs & Factions

```lua
-- Set current and max Health points (Sec 3)
.modify hp 10000

-- Set current and max Mana points (Sec 3)
.modify mana 10000

-- Set Warrior/Druid Rage pool (100 = 10 rage, 1000 = full rage) (Sec 3)
.modify rage 1000

-- Set Rogue/Cat Energy pool (100 = full energy) (Sec 3)
.modify energy 100

-- Set walk/run/swim speed multiplier (1.0 = normal, 2.5 = fast) (Sec 3)
.modify speed 2.5
.modify aspeed 2.5

-- Adjust character 3D model scale factor (1.0 = normal) (Sec 3)
.modify scale 1.2

-- Transform character model into NPC display model ID (Sec 3)
.modify morph 10045

-- Remove active model morph and restore natural appearance (Sec 2)
.demorph

-- Change character gender model (Sec 4)
.modify gender male
.modify gender female

-- Override primary attributes (Sec 4)
.modify strength 300
.modify agility 300
.modify stamina 300
.modify intellect 300
.modify spirit 300

-- Override combat ratings (Sec 4)
.modify ap 1000
.modify spellpower 500
.modify parry 25

-- Modify cosmetic customization indices live (Sec 3)
.modify hairstyle 2
.modify haircolor 4
.modify skincolor 3
.modify face 1
.modify accessories 2

-- Equip PvP or world title on character (Sec 3)
.modify title 14

-- Set drunkenness distortion (0 = sober, 100 = max) (Sec 3)
.modify drunk 0

-- Award PvP honor points toward weekly rank calculation (Sec 4)
.modify honor 50000

-- Set exact standing with designated faction (42999 = Exalted) (Sec 3)
-- Syntax: .modify rep <faction_id> <rep>
.modify rep 529 42999

-- Add copper currency to character wallet (10000000 = 1,000 Gold) (Sec 3)
.modify money 10000000
```

### Major Faction IDs for Instant Exalted Standing (`42999`)

```lua
-- Argent Dawn (Faction 529)
.modify rep 529 42999

-- Thorium Brotherhood (Faction 59)
.modify rep 59 42999

-- Timbermaw Hold (Faction 576)
.modify rep 576 42999

-- Cenarion Circle (Faction 609)
.modify rep 609 42999

-- Hydraxian Waterlords (Faction 749)
.modify rep 749 42999

-- Zandalar Tribe (Faction 270)
.modify rep 270 42999

-- Brood of Nozdormu (Faction 910)
.modify rep 910 42999

-- Alliance Capitals: Stormwind (72), Ironforge (47), Darnassus (69), Gnomeregan (54)
.modify rep 72 42999
.modify rep 47 42999
.modify rep 69 42999
.modify rep 54 42999

-- Horde Capitals: Orgrimmar (76), Darkspear Trolls (530), Thunder Bluff (81), Undercity (68)
.modify rep 76 42999
.modify rep 530 42999
.modify rep 81 42999
.modify rep 68 42999
```

---

## 5. Items, Bags, Economy & Remote Mail

```lua
-- Spawn item directly into inventory bags (Sec 3)
-- Syntax: .additem <item_id> [count]
.additem 19019 1

-- Spawn entire equipment set into bags (e.g., Tier sets) (Sec 3)
-- Syntax: .additemset <set_id>
.additemset 209

-- Destroy specified item from inventory (Sec 3)
.deleteitem 19019 1

-- Delete all non-equipped items in bags (Sec 3)
.cleaninventory

-- Move item between slots (Sec 3)
-- Syntax: .itemmove <src_slot> <dst_slot>
.itemmove 1 19

-- Open personal bank storage vault remotely anywhere (Sec 3)
.bank

-- Open mailbox interface remotely without finding a mailbox object (Sec 3)
.mailbox

-- Display or modify Turtle Token donation coin balance for account (Sec 3)
-- Syntax: .balance <account_id> [amount]
.balance 1 500

-- List all mail items in target player/bot mailbox (Sec 3)
.character mail list

-- Delete specific stuck or corrupted mail message by mail ID (Sec 3)
.character mail delete 42

-- Send parcel with attached items via game mail (Sec 3)
-- Syntax: .send items <name> "<subject>" "<body>" <item_id[:count]> ...
.send items Alice "Gear" "Enjoy" 19019:1

-- Send money directly to player or bot via game mail (Sec 3)
-- Syntax: .send money <name> "<subject>" "<body>" <copper>
.send money Bob "Gold" "Here" 10000000

-- Send standard text letter via mail (Sec 2)
-- Syntax: .send mail <name> "<subject>" "<body>"
.send mail Alice "Hello" "Welcome to Tortoise"

-- Send server system message directly to a player (Sec 3)
.send message Alice "Welcome to the realm"
```

---

## 6. Accounts, Security Ranks & Permissions

### Security Level Ranks
```lua
-- Rank 0 = SEC_PLAYER (Standard player account)
-- Rank 1 = SEC_OBSERVER (Read-only GM assistance)
-- Rank 2 = SEC_MODERATOR (Bans, kicks, freezes, tickets, teleports)
-- Rank 3 = SEC_DEVELOPER (Spawns, .modify, cheats, hot-reloads)
-- Rank 4 = SEC_ADMINISTRATOR (Full administrative server control)
-- Rank 5 = SEC_SIGMACHAD (Highest assignable account rank via command)
-- Rank 6 = SEC_CONSOLE (Server console / CLI only; unreachable in-game)
```

### Account Commands

```lua
-- Create new game account with player security (Sec 4)
-- Syntax: .account create <username> <password>
.account create Alice pass123

-- Delete account permanently (Console only, Sec 6)
.account delete Alice

-- Set account security level (Sec 4)
-- Syntax: .account set gmlevel <account_name> <0-5>
.account set gmlevel Bob 3

-- Reset account password (Sec 3)
-- Syntax: .account set password <account_name> <new_pass> <new_pass>
.account set password Bob newpass newpass

-- List all characters belonging to account (Sec 3)
.account characters Alice

-- Configure or remove 2-Factor Authentication on account (Sec 4)
.account twofa setup
.account twofa disable

-- Toggle GM mode tag on your active session (Sec 2)
.gm on
.gm off

-- Toggle complete invisibility from players and creature aggro (Sec 2)
.gm visible off
.gm visible on

-- Enable noclip 3D flight mode (Sec 4)
.gm fly on
.gm fly off

-- Toggle GM badge display in public chat (Sec 4)
.gm chat off
.gm chat on

-- Dump account ID, security rank, IP, latency, and zone for player (Sec 2)
.pinfo Alice
```

---

## 7. World Spawning, NPCs & GameObject Editing

```lua
-- Spawn NPC permanently at your coordinates facing your heading (Sec 3)
.npc add 10184

-- Permanently despawn and delete targeted NPC from world database (Sec 3)
.npc delete

-- Dump creature entry ID, GUID, faction, health, display ID, and script (Sec 3)
.npc info

-- Dump active CreatureAI script state, timers, and combat phase (Sec 4)
.npc aiinfo

-- Teleport targeted NPC to your current character coordinates (Sec 3)
.npc move

-- Summon temporary NPC to your coordinates (Sec 3)
.npc summon 10184

-- Transform targeted creature spawn into another template entry (Sec 3)
.npc set entry 10184

-- Set NPC movement type: 0=Stay Idle, 1=Random Walk, 2=Waypoints (Sec 3)
.npc set movetype 1

-- Set max random wander radius in yards (Sec 3)
.npc set wanderdistance 15

-- Override faction of targeted creature spawn (35 = Friendly) (Sec 3)
.npc factionid 35

-- Equip visual weapon/shield model onto targeted NPC spawn (Sec 3)
-- Syntax: .npc addweapon <slot 1-3> <item_id>
.npc addweapon 1 19019

-- Write field override into 'creature' spawn record (Sec 3)
-- Valid fields: entry, displayid, emotestate, standstate, sheathstate, movetype, wanderdistance, respawntime, deathstate, auras
.npc spawn set respawntime 300

-- Write field override into 'creature_template' record (Sec 3)
-- Valid fields: displayid, mountdisplayid, level, faction, scale, detectionrange, callforhelprange, leashrange
.npc template set level 60

-- Order targeted NPC to follow GM (Sec 3)
.npc follow
.npc unfollow

-- Force NPC to evade combat and return to spawn point (Sec 3)
.npc evade

-- Query current top threat target on mob threat table (Sec 3)
.npc toptarget

-- Force targeted NPC to speak in say or yell channel (Sec 3)
.npc say Welcome traveler!
.npc yell To arms!

-- Permanently spawn GameObject (chest, door, anvil, chair) at location (Sec 3)
.gobject add 176580

-- Spawn temporary GameObject that despawns on reboot (Sec 3)
.gobject tmpadd 176580

-- Permanently delete GameObject by GUID (Sec 3)
.gobject delete 12345

-- Select designated GameObject GUID as current manipulation target (Sec 3)
.gobject select 12345

-- List all GameObjects within radius (Sec 3)
.gobject near 20

-- Select nearest GameObject matching entry ID (Sec 3)
.gobject target 176580

-- Move GameObject to target coordinates or character location (Sec 3)
.gobject move 12345

-- Adjust orientation rotation angle of GameObject (Sec 3)
.gobject turn 12345 3.14

-- Rotate GameObject around Euler axes (Sec 3)
.gobject rotate 12345

-- Adjust physical scale factor of GameObject model (range: 0.1 to 50.0) (Sec 3)
.gobject scale 12345 2.0

-- Dump GameObject entry, display ID, state, respawn timer, and owner (Sec 3)
.gobject info

-- Open or close doors, gates, portcullises, and levers instantly (Sec 3)
.gobject toggle 12345

-- Force GameObject loot state: 0=Not ready, 1=Ready, 2=Activated, 3=Despawned (Sec 3)
.gobject set lootstate 12345 1

-- Bind current area to specified graveyard ID (Sec 4)
.linkgrave 100

-- Discover nearest linked graveyard entry for current coordinates (Sec 3)
.neargrave

-- Modify zone weather: 0=Fine, 1=Rain, 2=Snow, 3=Storm with intensity 0.0-1.0 (Sec 3)
.wchange 1 0.8
```

---

## 8. Quests, Instances, Events & Lockouts

```lua
-- Add quest directly to player quest log in active state (Sec 3)
.quest add 87

-- Instantly mark all objectives complete and ready for reward turn-in (Sec 3)
.quest complete 87

-- Drop quest from quest log (Sec 3)
.quest remove 87

-- Test quest availability, prerequisites, and flags (Sec 3)
.quest test 87

-- Dump state, objectives, and counters of one quest on player (Sec 2)
.quest status 87

-- Send player an addon message (TWQUEST) listing all completed quests (Sec 0)
.queststatuses

-- Clear personal raid lockout bind for specific map ID or all raids (Sec 3)
.instance unbind 409
.instance unbind all

-- Clear raid lockout binds for all members in current group (Sec 3)
.instance groupunbind all

-- Display active instance lockouts, difficulty, and reset timestamps (Sec 3)
.instance listbinds

-- Show live memory statistics for all active dungeon and raid instances (Sec 3)
.instance stats

-- Dump tick execution time and entity count per instance (Sec 3)
.instance perfinfos

-- List all seasonal world events, IDs, and active/inactive status (Sec 4)
.event list

-- Force immediate start or stop of world event by ID (Sec 4)
.event start 1
.event stop 1

-- Enable or disable scheduled event execution in calendar (Sec 4)
.event enable 1
.event disable 1

-- Display Ahn'Qiraj War Effort resource turn-in progression percentages (Sec 3)
.wareffort info

-- Advance AQ Gates event stage: 1=Collecting, 2=Ready, 3=War (Sec 3)
.wareffort setstage 3

-- Set epoch timestamp for 10-hour gong war window (Sec 3)
.wareffort setgongtime 1700000000
```

---

## 9. Server Management, Performance & Backups

### Host-Level Systemd Controls (`commands.md`)

```bash
# Start world and auth server daemons
systemctl --user start turtle-mangosd turtle-realmd

# Stop world and auth server daemons
systemctl --user stop turtle-mangosd turtle-realmd

# Check daemon status and active PID
systemctl --user status turtle-mangosd turtle-realmd

# Live stream world server console output
journalctl --user -u turtle-mangosd -f

# Check recent crash or startup logs
journalctl --user -u turtle-mangosd --since "5 min ago"

# Reset rate-limiting lock after crash
systemctl --user reset-failed turtle-mangosd turtle-realmd

# Gracefully restart world daemon
systemctl --user restart turtle-mangosd
```

### In-Game Server & Performance Commands

```lua
-- Dump server uptime, active human players, playerbots, and core revision (Sec 0)
.server info

-- Schedule graceful server restart with timer in seconds and reason (Sec 4)
.server restart 300 Maintenance restart
.server restart cancel

-- Schedule clean server shutdown with timer and reason (Sec 4)
.server shutdown 300 Scheduled shutdown
.server shutdown cancel

-- Schedule restart that triggers only when 0 players are online (Sec 4)
.server idlerestart 60 Idle restart

-- Schedule shutdown that triggers only when 0 players are online (Sec 4)
.server idleshutdown 60 Idle shutdown

-- Force immediate server-wide reset of all raid instances and player lockouts (Sec 4)
.server resetallraids

-- Trigger immediate cleanup and expiration cycle for abandoned corpses (Sec 4)
.server corpses

-- Terminate server process immediately (Console only, Sec 6)
.server exit

-- Synchronously flush and save all character and guild data to database (Sec 4)
.saveall

-- Send global red-text announcement to all online players (Sec 4)
.broadcast Server maintenance in progress.

-- Display global screen popup notification in center of screen (Sec 4)
.notify Welcome to Turtle-WoW!

-- Set minimum player level required to speak in public world channels (Sec 4)
.minchatlevel 10

-- Enable or disable core performance statistics gathering (Sec 4)
.perf enable 1
.perf enable 0

-- Generate detailed breakdown of periodic tick times and diff statistics (Sec 4)
.perf intervalreport

-- Dump process memory usage, virtual memory, threads, and socket handles (Sec 3)
.perf resources

-- Dump CPU time and processor utilization breakdown (Sec 3)
.perf cpu

-- Dump heap allocation and memory blocks (Sec 3)
.perf memory

-- Toggle mod-playerbots performance monitor and AI tick counters (Sec 2)
.perfmon

-- Export character database record (spells, items, quests, talents) to file (Sec 4)
.pdump write mychar.dump Bob

-- Import and recreate character from dump file into target account (Sec 4)
.pdump load mychar.dump Alice Alice

-- List all character dump files in the server dumps directory (Sec 4)
.pdump list

-- Search deleted characters table by name pattern (Sec 3)
.character deleted list name Bob

-- Restore deleted character back into active characters table (Sec 3)
-- Syntax: .character deleted restore <guid|name> [new_name] [account]
.character deleted restore Bob

-- Permanently purge character record from database (Console only, Sec 6)
.character erase Spammer

-- Purge expired deleted characters exceeding retention window (Sec 4)
.service del_characters
```

---

## 10. Moderation, Tickets, Bans & Anticheat

```lua
-- Disconnect player or bot session from realm immediately (Sec 2)
.kick Goldspammer

-- Paralyze player, preventing movement, spellcasting, and chat (Sec 2)
.freeze Hacker
.unfreeze Hacker

-- Silence player in public and whisper chat channels (Sec 2)
-- Syntax: .mute <name> <minutes> [reason]
.mute Spammer 60 Spamming
.unmute Spammer

-- Silence player where mute timer decrements only while online (Sec 2)
.pausingmute Spammer 60 Spamming

-- Ban account (time: 1d, 7d, permanent) (Sec 2)
.ban account Badguy 7d Botting
.unban account Badguy

-- Ban character name from logging in (Sec 2)
.ban character Badguy 1d Cheating
.unban character Badguy

-- Ban IP address subnet from connecting to realm (Sec 2)
.ban ip 1.2.3.4 permanent Exploit
.unban ip 1.2.3.4

-- Ban all IP addresses ever used by designated account (Sec 4)
.ban allip Badguy 30d Exploit

-- Ban hardware fingerprint identifier (Sec 4)
.ban fingerprint 0xDEADBEEF permanent Bot

-- Inspect active ban records (Sec 2)
.baninfo account Badguy
.baninfo character Badguy
.baninfo ip 1.2.3.4

-- GM Support Ticket Queue Management (Sec 2)
.ticket list
.ticket onlinelist
.ticket assign 12 Alice
.ticket unassign 12
.ticket close 12

-- Anti-Cheat Telemetry (Sec 2-3)
.sus enable 1
.sus movementenable 1
.sus notify 1
.sus fishers
```

---

## 11. Turtle-WoW 1.18.1 Custom Systems & Vanity Features

```lua
-- Create guild without charter signature requirement (Sec 3)
.guild create Alice "Vanguard"

-- Force player or bot into designated guild (Sec 2)
.guild invite Bob "Vanguard"

-- Remove player from guild (Sec 2)
.guild uninvite Bob

-- Set guild rank index (0 = Guild Master) (Sec 3)
.guild rank Bob 1

-- Rename existing guild (Sec 3)
.guild rename "Old Name" "New Name"

-- Transfer guild leadership (Sec 3)
.guild leader Bob

-- Set current position as guild house teleport point (Sec 3)
-- NOTE: Run .reload housing after changing
.guild house on
.guild house off

-- Toggle full map exploration reveal matching Cartographer addon (Sec 0)
.cartographer

-- Toggle global Hardcore death announcement broadcasts (Sec 0)
.hcmessages on
.hcmessages off

-- Toggle Hardcore channel communication mode (Sec 0)
.hcchat on
.hcchat off

-- Toggle Slow & Steady / Trainee experience challenge mode (Sec 3)
.toggletrainee

-- Toggle Inferno hardcore mode challenge (Sec 3)
.toggleinferno

-- Toggle custom Illusion visual transfiguration system (Sec 3)
.toggleillusions

-- Remove item from character transmog appearance library (Sec 3)
.tmogdelete 19019

-- Duplicate physical appearance of target (uses Appearance Token 80699) (Sec 0)
.copy Bob

-- Spawn test dummy (entry 1001000) to simulate drops and loot chance (Sec 3)
.loothelper 10184

-- Show balance of Turtle shell coin vanity tokens (Sec 0)
.shellcoin

-- Play streaming Turtle radio track in client (1 = music ID 60401, 2 = 60423) (Sec 0)
.radio 1
.radio 2

-- Block or unblock Easter egg interaction by ID (Sec 3)
.blockegg 1

-- Visualize zone boundary lines in client (Sec 3)
.showborders on
.showborders off

-- Send GM whispers/mail without revealing GM identity (Sec 3)
.anonymous whispers on
.anonymous mail on

-- Dump donation shop transaction purchase logs for account (Sec 3)
.shop log Alice

-- Process refund for shop donation item by order ID (Sec 3)
.shop refund 42
```

---

## 12. Developer Diagnostics, Navmesh & Waypoints

```lua
-- Universal Database Entity Lookups (Sec 2)
.lookup creature Nefarian
.lookup item Thunderfury
.lookup spell Bloodlust
.lookup quest Onyxia
.lookup object Black Lotus
.lookup skill Swords
.lookup faction Argent Dawn
.lookup itemset Dreadmist

-- Dump all active buffs, debuffs, durations, and stacks on target (Sec 3)
.list auras

-- Dump live threat table values and target rankings for mob (Sec 1)
.list threat

-- List all active world spawn coordinates and map instances for creature (Sec 3)
.list creature 10184

-- List all world spawn instances of specified GameObject (Sec 3)
.list object 176580

-- Waypoint Path Recording (Sec 4)
.wp show on 10184
.wp show off 10184
.wp add 10184
.wp modify 10184 1
.wp export 10184

-- Navmesh (Mmaps) Navigation Diagnostics (Sec 3)
.mmaps path
.mmaps near

-- Measure exact 3D distance in yards between self and target (Sec 3)
.distance

-- Dump exact X, Y, Z coordinates, orientation, map ID, zone ID, and area ID (Sec 1)
.gps

-- Low-Level Engine Diagnostics (Sec 3-4)
.debug condition 42
.debug instancedata
.debug packetstats
.debug leakreport
```

---

## 13. Complete Database Hot-Reload Catalog (`.reload`)

Flushes server memory caches and re-reads world database tables without server restart:

```lua
-- ======================================================================
-- Essential Core Content & Dynamic Module Reloads
-- ======================================================================
-- Flushes Lua state and reloads all Eluna scripts & hooks
.reload eluna

-- Reloads quest objectives, prerequisites, and rewards
.reload quest_template

-- Reloads spell data, spell effects, and custom overrides
-- NOTE: Core spell_template changes require server restart
.reload spell_template

-- Reloads NPC attributes, damage, armor, and base stats
.reload creature_template

-- Reloads NPC in-world placements and active spawns
.reload creature

-- Reloads GameObject metadata, chests, doors, and nodes
.reload gameobject_template

-- Reloads in-world GameObject spawns and coordinates
.reload gameobject

-- Reloads item stats, weapon damage, requirements, and prices
.reload item_template

-- Flushes and re-reads creature drop tables
.reload creature_loot_template

-- Flushes and re-reads chest and node loot tables
.reload gameobject_loot_template

-- Reloads dungeon and world teleport entrance triggers
.reload areatrigger_teleport

-- Re-reads mangosd.conf server configuration values
.reload config

-- Reloads Turtle-WoW custom guild housing coordinates
.reload housing

-- ======================================================================
-- All 106 Core Database Reload Targets
-- ======================================================================
-- Account & Moderation
.reload account_banned
.reload ip_banned
.reload gmlevels
.reload gm_ticket_template
.reload autobroadcast
.reload reserved_name

-- Area Triggers & Teleports
.reload areatrigger_involvedrelation
.reload areatrigger_tavern
.reload areatrigger_teleport
.reload game_tele
.reload taxi_path_transitions

-- Creatures & Spawns
.reload creature
.reload creature_template
.reload creature_ai_events
.reload creature_battleground
.reload creature_display_info_addon
.reload creature_groups
.reload creature_involvedrelation
.reload creature_questrelation
.reload creature_onkill_reputation
.reload creature_spells
.reload creature_spells_scripts
.reload locales_creature

-- GameObjects
.reload gameobject
.reload gameobject_template
.reload gameobject_battleground
.reload gameobject_involvedrelation
.reload gameobject_questrelation
.reload gameobject_requirement
.reload gameobject_scripts
.reload locales_gameobject

-- Items & Equipment
.reload item_template
.reload item_enchantment_template
.reload item_required_target
.reload locales_item

-- Loot Tables
.reload creature_loot_template
.reload gameobject_loot_template
.reload item_loot_template
.reload disenchant_loot_template
.reload fishing_loot_template
.reload mail_loot_template
.reload pickpocketing_loot_template
.reload reference_loot_template
.reload skinning_loot_template
.reload map_loot_disabled

-- Quests
.reload quest_template
.reload quest_start_scripts
.reload quest_end_scripts
.reload quest_greeting
.reload locales_quest

-- Gossip & Dialog
.reload gossip_menu
.reload gossip_menu_option
.reload gossip_scripts
.reload npc_gossip
.reload npc_text
.reload npc_trainer
.reload npc_vendor
.reload trainer_greeting
.reload locales_gossip_menu_option

-- Spells & Auras
.reload spell_affect
.reload spell_area
.reload spell_chain
.reload spell_disabled
.reload spell_elixir
.reload spell_group
.reload spell_group_stack_rules
.reload spell_learn_spell
.reload spell_mod
.reload spell_pet_auras
.reload spell_proc_event
.reload spell_proc_item_enchant
.reload spell_scripts
.reload spell_script_target
.reload spell_target_position
.reload spell_threats
.reload instance_buff_removal

-- World, Weather & Systems
.reload conditions
.reload config
.reload event_scripts
.reload generic_scripts
.reload exploration_basexp
.reload game_graveyard_zone
.reload game_weather
.reload housing
.reload shop
.reload skill_fishing_base_level
.reload variables
.reload visibilities
.reload points_of_interest
.reload locales_points_of_interest
.reload page_text
.reload locales_page_text
.reload mangos_string
.reload module_string
.reload module_string_locale
.reload petitions
.reload character_pet
.reload cinematic_waypoints
.reload bgplayers
.reload pet_name_generation
.reload reputation_reward_rate
.reload reputation_spillover_template

-- Faction Change Systems
.reload player_factionchange_items
.reload player_factionchange_mounts
.reload player_factionchange_quests
.reload player_factionchange_reputations
.reload player_factionchange_spells
```

---

## 14. Auction House Bot Administration (`.ahbot`)

Requires `BUILD_PLAYERBOTS=ON` (Security Level 2 / Moderator):

```lua
-- Show active AHBot auction counts, items, and wallet stats (Sec 2)
.ahbot status
.ahbot status all

-- Re-read ahbot.conf settings without restarting world server (Sec 2)
.ahbot reload

-- Flush active bot auctions and rebuild market listings (Sec 2)
.ahbot rebuild
.ahbot rebuild all

-- Trigger immediate market update and bid/expire evaluation tick (Sec 2)
.ahbot update

-- Override market pricing, posting chance, and item quantity limits (Sec 2)
-- Syntax: .ahbot item <item_id> [price_ratio] [chance] [min_count] [max_count]
.ahbot item 19019 10000 50 1 3

-- Restore default configuration values for specific item ID (Sec 2)
.ahbot item 19019 reset
```

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📋 Operations (commands.md)](../commands.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md) • [🏰 Volume 06: Raid Operations](./06_automated_raid_and_dungeon_operations.md)
