# 01. Game Master & World Administration Handbook (Master Codex)

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS Core Engine
> **Source Verification**: `src/game/Chat/Chat.cpp`, `Commands.cpp`, `Level3.cpp`
> **Security Coverage**: Security Levels 0–4 (Player, Moderator, GameMaster, Administrator, Console)
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear & Specs](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md)

---

## Table of Contents

1. [Categorized In-Game Lua Macros](#1-categorized-in-game-lua-macros-top-priority)
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

---

## Unified 5-Volume Master Reference Architecture

| Volume | Handbook Title | Domain & Core Coverage | Direct Link |
| :---: | :--- | :--- | :---: |
| **01** | **Game Master & World Administration** | **100% of GM commands, server maintenance, character progression, cheats, security, spawning, moderation, tickets, reloads & diagnostics.** | **Current File** |
| **02** | **[Playerbot & Raid Automation](./02_playerbot_and_raid_automation.md)** | **Complete .bot & .rndbot control, whisper shortcuts, strategy tokens (co/nc), .dc dungeon clear solver & raid macros.** | [Open Volume 02](./02_playerbot_and_raid_automation.md) |
| **03** | **[World, Dungeons & Teleports Directory](./03_turtle_world_and_teleports.md)** | **647+ .tele tokens, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements.** | [Open Volume 03](./03_turtle_world_and_teleports.md) |
| **04** | **[Class Specs & Endgame Gear Directory](./04_class_specs_and_endgame_gear.md)** | **30 Class Specs across all 9 classes, 100% explicit 16–17 slot loadouts, Tier 1/2/3 set IDs, weapons, trinkets, enchants & verified talent builds.** | [Open Volume 04](./04_class_specs_and_endgame_gear.md) |
| **05** | **[Eluna Scripting & Core Development Engine](./05_developer_eluna_and_engine.md)** | **Eluna Lua API, live reloads (.reload eluna), Event 42 Custom Command dispatch, SpellInfo layer & C++ hook mechanics.** | [Open Volume 05](./05_developer_eluna_and_engine.md) |

---

## 1. Categorized In-Game Lua Macros (Top Priority)

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`) or run them directly in chat:

### Category A: Character Progression, Boost & Survival

#### 🌟 Macro 1: Complete Level 60 Instant Boost Suite (Spells + Skills + Travel + Bags + Gold)

Instantly raises your character to level 60, teaches all class spells, trainer recipes, maxes weapon/defense skills to 300, unlocks flight paths/maps, awards 1,000 Gold, 4x 18-slot Bottomless Bags, and fully replenishes HP and mana:

```lua
.levelup 60
.learn all_myspells
.learn all_trainer
.learn all_recipes
.maxskill
.modify money 10000000
.additem 14156 4
.taxicheat on
.explorecheat on
.repairitems
.replenish
```

#### 🛡️ Macro 2: Panic Survival & Instant God Mode

Instantly toggles invulnerability, drops all combat threat, clears all spell cooldowns, and restores 100% health and mana:

```lua
.god on
.combatstop
.cooldown
.replenish
```

#### ⚡ Macro 3: Fast Travel Flight & Forward Blink

Activates free-floating hover flight mode, sets movement speed to 250%, blinks 20 yards forward through geometry, and lifts 15 yards:

```lua
.hover on
.modify speed 2.5
.go forward 20
.go up 15
```

#### 🛑 Macro 4: Normal Mobility & God Mode Reset

Disables hover flight, resets movement speed to normal (1.0), turns off god mode, and restores standard player physics:

```lua
.hover off
.modify speed 1.0
.god off
```

#### 💰 Macro 5: Remote Bank & Mail Access Anywhere

Opens your personal bank vault and remote mailbox interface from anywhere in the world without visiting an NPC:

```lua
.bank
.mailbox
```

---

### Category B: GM Stealth Patrol, Admin Duty & Server Maintenance

#### 🕵️ Macro 6: GM Invisible Duty & Silent Patrol

Enables GM tag, turns completely invisible to players and creatures, enables free flight, and silences public GM chat broadcasts:

```lua
.gm on
.gm visible off
.gm fly on
.gm chat off
```

#### 🔄 Macro 7: Hotfix Core Reload Suite (World, Quests, Spells, Scripts)

Immediately flushes and hot-reloads the most critical database tables without rebooting the server:

```lua
.reload quest_template
.reload spell_template
.reload creature_template
.reload item_template
.reload gameobject_template
.reload eluna
```

#### 🧹 Macro 8: Realm Save & Graceful Restart (5-Minute Warning)

Broadcasts a 5-minute countdown announcement, saves all character and guild data, and schedules a clean server restart:

```lua
.saveall
.broadcast Server restart in 5 minutes for maintenance. Please finish encounters and log out safely.
.server restart 300 Server restart in 5 minutes for maintenance.
```

#### 🛑 Macro 9: Emergency Raid Save Reset

Forces an immediate server-wide reset of all raid instances and player lockout binds:

```lua
.server resetallraids
```

---

## 2. Character Progression, Training & Skills

Commands dedicated to your player character or targeted player (`%t`):

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.levelup` | 3 | `.levelup [#]` | Increases character level by specified amount (e.g. 60 or 1). | `.levelup 60` |
| `.learn` | 3 | `.learn <all_myspells\|all_trainer\|all_recipes\|all_items\|id>` | Teaches spells, all class spells, trainer abilities, or specific spell ID. | `.learn all_myspells` |
| `.unlearn` | 3 | `.unlearn <spell_id>` | Removes a specific spell from character spellbook. | `.unlearn 133` |
| `.unlearnoffline` | 3 | `.unlearnoffline <player> <spell_id>` | Removes spell from an offline player character. | `.unlearnoffline Bob 133` |
| `.maxskill` | 3 | `.maxskill` | Advances all learned weapon, defense, and profession skills to level cap (300). | `.maxskill` |
| `.setskill` | 3 | `.setskill <skill_id> [#cur] [#max]` | Sets specific skill ID value and maximum capacity. | `.setskill 26 300 300` |
| `.getskillvalue` | 3 | `.getskillvalue <skill_id>` | Queries current value of specified skill. | `.getskillvalue 26` |
| `.taxicheat` | 3 | `.taxicheat <on\|off>` | Temporarily unlocks all flight paths across Kalimdor and Eastern Kingdoms. | `.taxicheat on` |
| `.character fillflys` | 3 | `.character fillflys` | Permanently writes all flight path taxi nodes into character database record. | `.character fillflys` |
| `.explorecheat` | 3 | `.explorecheat <on\|off>` | Reveals all world map fog and fully explores every zone. | `.explorecheat on` |
| `.repairitems` | 3 | `.repairitems` | Restores all equipped and inventory gear to 100% durability at zero cost. | `.repairitems` |
| `.replenish` | 3 | `.replenish` | Instantly fills Health, Mana, Rage, and Energy pools to 100% capacity. | `.replenish` |
| `.cooldown` | 3 | `.cooldown [spell_id]` | Resets all active cooldown timers or a specific ability cooldown. | `.cooldown` |
| `.reset` | 3 | `.reset <talents\|spells\|stats\|honor\|level>` | Resets talent trees, learned spells, base attributes, or PvP rank honor. | `.reset talents` |
| `.character diffitems` | 1 | `.character diffitems` | Audits inventory and equipment for item anomalies or duplicate uniques. | `.character diffitems` |
| `.character itemlog` | 3 | `.character itemlog` | Dumps acquisition, trade, and vendor history for targeted character. | `.character itemlog` |
| `.character inactivity` | 1 | `.character inactivity` | Inspects account/character activity timestamp and inactivity thresholds. | `.character inactivity` |
| `.unstuck` | 2 | `.unstuck` | Forces instant player rescue teleport to bound home tavern (Hearthstone). | `.unstuck` |

---

## 3. Combat, Spells, Auras & Cheats

Combat mechanics, invulnerability, and aura manipulation:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.god` | 3 | `.god <on\|off>` | Enables total damage immunity and status effect invulnerability. | `.god on` |
| `.combatstop` | 2 | `.combatstop [target]` | Drops all combat state, clears threat lists, and forces hostile mobs to reset. | `.combatstop` |
| `.revive` | 3 | `.revive [target]` | Resurrects targeted corpse with 100% health and mana, clearing sickness. | `.revive` |
| `.die` | 3 | `.die [target]` | Instantly kills targeted unit, bypassing armor and defensive cooldowns. | `.die` |
| `.cast` | 3 | `.cast <spell_id> [triggered]` | Casts specified spell ID directly from your character onto target. | `.cast 133` |
| `.cast self` | 3 | `.cast self <spell_id>` | Forces target or self to cast specified spell on self. | `.cast self 20549` |
| `.cast target` | 3 | `.cast target <spell_id>` | Casts specified spell targeting current target. | `.cast target 133` |
| `.castcustom` | 3 | `.castcustom <spell_id> <effect_mask>` | Fires spell with custom effect bitmask override. | `.castcustom 133 1` |
| `.damage` | 3 | `.damage <amount>` | Deals specified flat damage value to current target. | `.damage 5000` |
| `.aoedamage` | 3 | `.aoedamage <amount> [radius]` | Deals damage to all hostile units within designated yard radius. | `.aoedamage 10000 30` |
| `.aura` | 3 | `.aura <spell_id>` | Applies positive or negative spell aura to targeted character/NPC. | `.aura 15366` |
| `.unaura` | 3 | `.unaura <spell_id>` | Removes active aura effect by spell ID. | `.unaura 15366` |
| `.fear` | 3 | `.fear <on\|off>` | Forces target to run in panic fear. | `.fear on` |
| `.waterwalk` | 3 | `.waterwalk <on\|off>` | Enables character to traverse water surfaces without sinking. | `.waterwalk on` |
| `.dismount` | 3 | `.dismount` | Dismounts targeted player or bot from their mount. | `.dismount` |

---

## 4. Character Stats, Appearance, Morphs & Factions

Fine-grained attribute modification and cosmetic transformations via `.modify`:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.modify hp` | 3 | `.modify hp <amount>` | Sets current and maximum Health points. | `.modify hp 10000` |
| `.modify mana` | 3 | `.modify mana <amount>` | Sets current and maximum Mana points. | `.modify mana 10000` |
| `.modify rage` | 3 | `.modify rage <amount>` | Sets Warrior/Druid Rage pool (100 = 10 rage, 1000 = full rage). | `.modify rage 1000` |
| `.modify energy` | 3 | `.modify energy <amount>` | Sets Rogue/Cat Energy pool (100 = full energy). | `.modify energy 100` |
| `.modify speed` | 3 | `.modify speed <0.1–50>` | Modifies running and walking speed multiplier (1.0 = default). | `.modify speed 2.5` |
| `.modify aspeed` | 3 | `.modify aspeed <0.1–50>` | Modifies all-speed multiplier (run, swim, flight). | `.modify aspeed 3.0` |
| `.modify scale` | 3 | `.modify scale <0.1–10>` | Adjusts character 3D model scale factor (1.0 = standard size). | `.modify scale 1.2` |
| `.modify morph` | 3 | `.modify morph <display_id>` | Transforms character model into any NPC display model ID. | `.modify morph 10045` |
| `.demorph` | 2 | `.demorph` | Removes active model morph and restores natural character appearance. | `.demorph` |
| `.modify gender` | 4 | `.modify gender <male\|female>` | Changes character gender model permanently. | `.modify gender female` |
| `.modify strength` | 4 | `.modify strength <amount>` | Directly overrides character Strength attribute. | `.modify strength 300` |
| `.modify agility` | 4 | `.modify agility <amount>` | Directly overrides character Agility attribute. | `.modify agility 300` |
| `.modify stamina` | 4 | `.modify stamina <amount>` | Directly overrides character Stamina attribute. | `.modify stamina 300` |
| `.modify intellect` | 4 | `.modify intellect <amount>` | Directly overrides character Intellect attribute. | `.modify intellect 300` |
| `.modify spirit` | 4 | `.modify spirit <amount>` | Directly overrides character Spirit attribute. | `.modify spirit 300` |
| `.modify ap` | 4 | `.modify ap <amount>` | Directly overrides melee Attack Power attribute. | `.modify ap 1000` |
| `.modify spellpower` | 4 | `.modify spellpower <amount>` | Directly overrides bonus Spell Power attribute. | `.modify spellpower 500` |
| `.modify parry` | 4 | `.modify parry <percentage>` | Directly overrides character Parry chance. | `.modify parry 25` |
| `.modify hairstyle` | 3 | `.modify hairstyle <id>` | Modifies character hair style index live. | `.modify hairstyle 2` |
| `.modify haircolor` | 3 | `.modify haircolor <id>` | Modifies character hair color index live. | `.modify haircolor 4` |
| `.modify skincolor` | 3 | `.modify skincolor <id>` | Modifies character skin color index live. | `.modify skincolor 3` |
| `.modify face` | 3 | `.modify face <id>` | Modifies character facial features index live. | `.modify face 1` |
| `.modify accessories` | 3 | `.modify accessories <id>` | Modifies character accessories (piercings, horns, tusks). | `.modify accessories 2` |
| `.modify title` | 3 | `.modify title <title_id>` | Equips PvP or world title on character. | `.modify title 14` |
| `.modify drunk` | 3 | `.modify drunk <0–100>` | Sets character drunkenness visual distortion (0 = sober, 100 = max). | `.modify drunk 0` |
| `.modify honor` | 4 | `.modify honor <amount>` | Awards PvP honor points toward weekly rank calculation. | `.modify honor 50000` |
| `.modify rep` | 3 | `.modify rep <faction_id> <rep>` | Sets exact standing with designated faction (42999 = Exalted). | `.modify rep 529 42999` |
| `.modify money` | 3 | `.modify money <copper>` | Adds copper currency to character wallet (10000000 = 1,000 Gold). | `.modify money 10000000` |

### Major Faction IDs for Instant Exalted Standing (`42999`)

- **Argent Dawn**: `529` • **Thorium Brotherhood**: `59` • **Timbermaw Hold**: `576` • **Cenarion Circle**: `609`
- **Hydraxian Waterlords**: `749` • **Zandalar Tribe**: `270` • **Brood of Nozdormu**: `910`
- **Stormwind**: `72` • **Ironforge**: `47` • **Darnassus**: `69` • **Gnomeregan Exiles**: `54`
- **Orgrimmar**: `76` • **Darkspear Trolls**: `530` • **Thunder Bluff**: `81` • **Undercity**: `68`

---

## 5. Items, Bags, Economy & Remote Mail

Inventory manipulation, gear spawning, remote banking, and player/bot mail delivery:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.additem` | 3 | `.additem <item_id> [count]` | Spawns specified item directly into inventory bags. | `.additem 19019 1` |
| `.additemset` | 3 | `.additemset <set_id>` | Spawns entire equipment set into bags (e.g. Tier 1–3 sets). | `.additemset 209` |
| `.deleteitem` | 3 | `.deleteitem <item_id> [count]` | Destroys specified item ID from player inventory. | `.deleteitem 19019 1` |
| `.cleaninventory` | 3 | `.cleaninventory` | Deletes all non-equipped items in bags. | `.cleaninventory` |
| `.bank` | 3 | `.bank` | Opens personal bank storage vault remotely anywhere. | `.bank` |
| `.mailbox` | 3 | `.mailbox` | Opens mailbox interface remotely without finding a mailbox object. | `.mailbox` |
| `.character mail list` | 3 | `.character mail list` | Lists all mail items in target player/bot mailbox. | `.character mail list` |
| `.character mail delete` | 3 | `.character mail delete <mail_id>` | Deletes specific stuck or corrupted mail message. | `.character mail delete 42` |
| `.send items` | 3 | `.send items <name> "<subject>" "<body>" <item_id[:count]> ...` | Sends parcel with up to 12 items attached via game mail. | `.send items Alice "Gear" "Enjoy" 19019:1` |
| `.send money` | 3 | `.send money <name> "<subject>" "<body>" <copper>` | Sends money directly to player or bot via game mail. | `.send money Bob "Gold" "Here" 10000000` |
| `.send mail` | 2 | `.send mail <name> "<subject>" "<body>"` | Sends standard text letter to player or bot. | `.send mail Alice "Hello" "Welcome"` |
| `.send message` | 3 | `.send message <name> "<message>"` | Sends server system message directly to a player. | `.send message Alice "Hello"` |

---

## 6. Accounts, Security Ranks & Permissions

Account provisioning, security ranks (0–4), IP locks, and GM status:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.account create` | 4 | `.account create <name> <password>` | Creates new game account with player security (Level 0). | `.account create Alice pass123` |
| `.account delete` | 4 | `.account delete <name>` | Deletes account and all associated characters permanently. | `.account delete Alice` |
| `.account set gmlevel` | 4 | `.account set gmlevel <name> <0–4>` | Sets rank: 0=Player, 1=Observer/Mod, 2=GameMaster, 3=Developer, 4=Admin. | `.account set gmlevel Bob 3` |
| `.account set password` | 3 | `.account set password <name> <pass> <pass>` | Resets user account password. | `.account set password Bob pw pw` |
| `.account characters` | 3 | `.account characters [account_id\|name]` | Lists all character names and GUIDs belonging to account. | `.account characters Alice` |
| `.account twofa` | 4 | `.account twofa <setup\|disable>` | Configures or removes 2-Factor Authentication on account. | `.account twofa setup` |
| `.gm` | 2 | `.gm <on\|off>` | Toggles GM mode tag on your active session. | `.gm on` |
| `.gm visible` | 2 | `.gm visible <on\|off>` | Toggles complete invisibility from players and creature aggro. | `.gm visible off` |
| `.gm fly` | 4 | `.gm fly <on\|off>` | Enables noclip style 3D flying movement through air. | `.gm fly on` |
| `.gm chat` | 4 | `.gm chat <on\|off>` | Toggles GM badge display in public chat channels. | `.gm chat off` |
| `.gm options` | 4 | `.gm options` | Dumps current GM mode flags and settings. | `.gm options` |
| `.pinfo` | 2 | `.pinfo [name]` | Dumps account ID, security rank, IP address, latency, and zone for player. | `.pinfo Alice` |

---

## 7. World Spawning, NPCs & GameObject Editing

In-game world editing, creature spawning, pathing, and interactive gameobjects:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.npc add` | 3 | `.npc add <creature_entry>` | Spawns NPC permanently at your current coordinates facing your heading. | `.npc add 10184` |
| `.npc delete` | 3 | `.npc delete [guid]` | Permanently despawns and deletes targeted NPC from world database. | `.npc delete` |
| `.npc info` | 3 | `.npc info` | Dumps creature entry ID, GUID, faction, health, display ID, and AI script. | `.npc info` |
| `.npc move` | 3 | `.npc move [guid]` | Teleports targeted NPC to your current character coordinates. | `.npc move` |
| `.npc summon` | 3 | `.npc summon <creature_entry>` | Summons temporary NPC to your coordinates. | `.npc summon 10184` |
| `.npc set entry` | 3 | `.npc set entry <entry_id>` | Transforms targeted creature spawn into another template entry. | `.npc set entry 10184` |
| `.npc set movetype` | 3 | `.npc set movetype <0\|1\|2>` | Sets movement: 0=Stay Idle, 1=Random Walk, 2=Waypoints. | `.npc set movetype 1` |
| `.npc set wanderdistance` | 3 | `.npc set wanderdistance <dist>` | Sets max random wander radius in yards. | `.npc set wanderdistance 15` |
| `.npc set factionid` | 3 | `.npc set factionid <faction_id>` | Overrides creature faction standing (e.g. 35 = Friendly). | `.npc set factionid 35` |
| `.npc follow` | 3 | `.npc follow` | Orders targeted NPC to follow GM. | `.npc follow` |
| `.npc unfollow` | 3 | `.npc unfollow` | Halts NPC follow behavior. | `.npc unfollow` |
| `.npc evade` | 3 | `.npc evade` | Forces NPC to evade combat and return to spawn point. | `.npc evade` |
| `.npc toptarget` | 3 | `.npc toptarget` | Queries current top threat target on mob threat table. | `.npc toptarget` |
| `.npc say` | 3 | `.npc say <text>` | Forces targeted NPC to speak message in local say channel. | `.npc say Welcome traveler!` |
| `.npc yell` | 3 | `.npc yell <text>` | Forces targeted NPC to shout message in local yell channel. | `.npc yell To arms!` |
| `.gobject add` | 3 | `.gobject add <entry_id>` | Permanently spawns GameObject (chest, door, anvil, chair) at location. | `.gobject add 176580` |
| `.gobject tmpadd` | 3 | `.gobject tmpadd <entry_id>` | Spawns temporary GameObject that automatically despawns on reboot. | `.gobject tmpadd 176580` |
| `.gobject delete` | 3 | `.gobject delete <guid>` | Permanently deletes targeted or nearest GameObject. | `.gobject delete 12345` |
| `.gobject near` | 3 | `.gobject near [radius]` | Lists all GameObjects, entry IDs, GUIDs, and distances within radius. | `.gobject near 20` |
| `.gobject target` | 3 | `.gobject target [entry_id]` | Selects nearest GameObject matching entry ID. | `.gobject target 176580` |
| `.gobject move` | 3 | `.gobject move <guid> [x y z]` | Moves GameObject to target coordinates or character location. | `.gobject move 12345` |
| `.gobject turn` | 3 | `.gobject turn <guid> [o]` | Adjusts orientation rotation angle of GameObject. | `.gobject turn 12345 3.14` |
| `.gobject scale` | 3 | `.gobject scale <guid> <0.1–10>` | Adjusts physical scale factor of GameObject model. | `.gobject scale 12345 2.0` |
| `.gobject info` | 3 | `.gobject info [guid]` | Dumps entry, display ID, state, respawn timer, and owner. | `.gobject info` |
| `.gobject toggle` | 3 | `.gobject toggle <guid>` | Opens or closes doors, gates, portcullises, and levers instantly. | `.gobject toggle 12345` |
| `.wchange` | 3 | `.wchange <0–3> <0–1>` | Modifies zone weather: 0=Fine, 1=Rain, 2=Snow, 3=Storm with intensity. | `.wchange 1 0.8` |

---

## 8. Quests, Instances, Events & Lockouts

Quest state manipulation, raid lockout resets, instance diagnostics, and world events:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.quest add` | 3 | `.quest add <quest_id>` | Adds quest directly to player quest log in active state. | `.quest add 87` |
| `.quest complete` | 3 | `.quest complete <quest_id>` | Instantly marks all objectives complete and ready for reward turn-in. | `.quest complete 87` |
| `.quest remove` | 3 | `.quest remove <quest_id>` | Drops quest from quest log. | `.quest remove 87` |
| `.quest test` | 3 | `.quest test <quest_id>` | Tests quest availability, prerequisites, and flags. | `.quest test 87` |
| `.quest statuses` | 0 | `.quest statuses` | Dumps progress and state of all active quests for player. | `.quest statuses` |
| `.instance unbind` | 3 | `.instance unbind <map_id\|all>` | Clears personal raid lockout bind for specified map or all raids. | `.instance unbind all` |
| `.instance groupunbind` | 3 | `.instance groupunbind <map_id\|all>` | Clears raid lockout binds for all members in current group. | `.instance groupunbind all` |
| `.instance listbinds` | 3 | `.instance listbinds` | Displays active instance lockouts, difficulty, and reset timestamps. | `.instance listbinds` |
| `.instance stats` | 3 | `.instance stats` | Shows live memory statistics for all active dungeon and raid instances. | `.instance stats` |
| `.instance perfinfos` | 3 | `.instance perfinfos` | Dumps tick execution time and entity count per instance. | `.instance perfinfos` |
| `.event list` | 4 | `.event list` | Lists all seasonal world events, IDs, and active/inactive status. | `.event list` |
| `.event start` | 4 | `.event start <event_id>` | Forces immediate activation of world event (e.g. Darkmoon Faire). | `.event start 1` |
| `.event stop` | 4 | `.event stop <event_id>` | Stops active world event. | `.event stop 1` |
| `.event enable` | 4 | `.event enable <event_id>` | Enables scheduled event execution in calendar. | `.event enable 1` |
| `.event disable` | 4 | `.event disable <event_id>` | Disables scheduled event execution. | `.event disable 1` |
| `.wareffort info` | 3 | `.wareffort info` | Displays Ahn'Qiraj War Effort resource turn-in progression percentages. | `.wareffort info` |
| `.wareffort setstage` | 3 | `.wareffort setstage <stage>` | Advances AQ Gates event stage (1=Collecting, 2=Ready, 3=War). | `.wareffort setstage 3` |
| `.wareffort setgongtime` | 3 | `.wareffort setgongtime <timestamp>` | Sets exact epoch timestamp for the 10-hour gong war window. | `.wareffort setgongtime 1700000000` |

---

## 9. Server Management, Performance & Backups

CMaNGOS core lifecycle, performance profiling, character export/import tools, and host-level systemd service controls:

### Host-Level Server Lifecycle & Systemd Service Controls (`commands.md`)

The server runs as non-root **systemd user services** (`systemctl --user`). Use these shell commands to operate the world and auth daemons:

| Task | Shell Command | Notes |
| :--- | :--- | :--- |
| **Start Server** | `systemctl --user start turtle-mangosd turtle-realmd` | Boots world and auth realms. |
| **Stop Server** | `systemctl --user stop turtle-mangosd turtle-realmd` | Frees CPU & memory; MariaDB stays up. |
| **Check Status** | `systemctl --user status turtle-mangosd turtle-realmd` | Verifies active PID and uptime. |
| **Live Stream Logs** | `journalctl --user -u turtle-mangosd -f` | Real-time world server console output. |
| **Recent Crash Logs** | `journalctl --user -u turtle-mangosd --since "5 min ago"` | Essential when troubleshooting boot crashes. |
| **Reset Crash Lock** | `systemctl --user reset-failed turtle-mangosd turtle-realmd` | Clears systemd start rate-limiting after crash. |
| **Restart Server** | `systemctl --user restart turtle-mangosd` | Re-reads binary & core configuration. |
| **Recompile Binary** | `systemctl --user stop turtle-mangosd && make -C build mangosd && systemctl --user start turtle-mangosd` | Rebuilds world server safely. |

---

### In-Game Server & Performance Commands

CMaNGOS core lifecycle, performance profiling, and character export/import tools:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.server info` | 0 | `.server info` | Dumps server uptime, active human players, playerbots, core revision. | `.server info` |
| `.server restart` | 4 | `.server restart <seconds> [reason]` | Schedules graceful server restart with timer and system message. | `.server restart 300 Maintenance` |
| `.server restart cancel` | 4 | `.server restart cancel` | Aborts a pending scheduled restart. | `.server restart cancel` |
| `.server shutdown` | 4 | `.server shutdown <seconds> [reason]` | Schedules clean server shutdown with timer and system message. | `.server shutdown 300 Server off` |
| `.server shutdown cancel` | 4 | `.server shutdown cancel` | Aborts a pending scheduled shutdown. | `.server shutdown cancel` |
| `.server idlerestart` | 4 | `.server idlerestart <seconds> [reason]` | Schedules restart that occurs only when server has 0 active players. | `.server idlerestart 60 Idle restart` |
| `.server idleshutdown` | 4 | `.server idleshutdown <seconds> [reason]` | Schedules shutdown that occurs only when server has 0 active players. | `.server idleshutdown 60 Idle shutdown` |
| `.server resetallraids` | 4 | `.server resetallraids` | Forces an immediate server-wide reset of all raid instances and player binds. | `.server resetallraids` |
| `.server corpses` | 4 | `.server corpses` | Triggers immediate cleanup and expiration cycle for abandoned corpses. | `.server corpses` |
| `.server exit` | 4 | `.server exit` | Terminates server process immediately without waiting for worker shutdown. | `.server exit` |
| `.saveall` | 4 | `.saveall` | Synchronously flushes and saves all character and guild data to database. | `.saveall` |
| `.broadcast` | 4 | `.broadcast <message>` | Sends global red-text announcement to all online players. | `.broadcast Event starting!` |
| `.notify` | 4 | `.notify <message>` | Displays global screen popup notification in center of screen for all players. | `.notify Welcome to Tortoise!` |
| `.minchatlevel` | 4 | `.minchatlevel <level>` | Sets minimum player level required to speak in public world channels. | `.minchatlevel 10` |
| `.perf enable` | 4 | `.perf enable <0\|1>` | Enables or disables core performance statistics gathering. | `.perf enable 1` |
| `.perf resources` | 3 | `.perf resources` | Dumps process memory usage, virtual memory, threads, and socket handles. | `.perf resources` |
| `.perf cpu` | 3 | `.perf cpu` | Dumps CPU time and processor utilization breakdown. | `.perf cpu` |
| `.perf memory` | 3 | `.perf memory` | Dumps heap allocation and allocated memory blocks. | `.perf memory` |
| `.perfmon` | 2 | `.perfmon` | Toggles mod-playerbots performance monitor and AI tick counters. | `.perfmon` |
| `.pdump write` | 4 | `.pdump write <filename> <player>` | Exports character database record (spells, items, quests, talents) to file. | `.pdump write mychar.dump Bob` |
| `.pdump load` | 4 | `.pdump load <filename> <account> <name> [guid]` | Imports and recreates character from dump file into target account. | `.pdump load mychar.dump Alice Alice` |
| `.pdump list` | 4 | `.pdump list` | Lists all character dump files available in the server dumps directory. | `.pdump list` |
| `.character deleted list name` | 3 | `.character deleted list name <pattern>` | Searches deleted characters table by name pattern. | `.character deleted list name Bob` |
| `.character deleted restore` | 3 | `.character deleted restore <guid\|name> [new_name] [account]` | Restores a deleted character back into active characters table. | `.character deleted restore Bob` |
| `.character erase` | 4 | `.character erase <name>` | Permanently purges character record and inventory from database. | `.character erase Spammer` |
| `.service del_characters` | 4 | `.service del_characters` | Purges expired deleted characters exceeding server retention window. | `.service del_characters` |

---

## 10. Moderation, Tickets, Bans & Anticheat

Player discipline, support ticket queue, IP management, and Warden anticheat telemetry:

| Command | Sec | Syntax & Arguments | Description | Example |
| --- | --- | --- | --- | --- |
| `.kick` | 2 | `.kick <player_name>` | Disconnects player or bot session from realm immediately. | `.kick Goldspammer` |
| `.freeze` | 2 | `.freeze [player_name]` | Paralyzes player, preventing all movement, spellcasting, and chat. | `.freeze Hacker` |
| `.unfreeze` | 2 | `.unfreeze [player_name]` | Restores full player control. | `.unfreeze Hacker` |
| `.mute` | 2 | `.mute <name> <minutes> [reason]` | Silences player in all public and whisper chat channels. | `.mute Spammer 60 Spamming` |
| `.pausingmute` | 2 | `.pausingmute <name> <minutes> [reason]` | Silences player where mute timer decrements only while online. | `.pausingmute Spammer 60 Spamming` |
| `.unmute` | 2 | `.unmute <name>` | Lifts active chat mute penalty. | `.unmute Spammer` |
| `.ban account` | 2 | `.ban account <name> <time> [reason]` | Bans account (time: `1d`, `7d`, `permanent`). | `.ban account Badguy 7d Botting` |
| `.ban character` | 2 | `.ban character <name> <time> [reason]` | Bans character name from logging in. | `.ban character Badguy 1d Cheating` |
| `.ban ip` | 2 | `.ban ip <ip> <time> [reason]` | Bans IP address subnet from connecting to realm. | `.ban ip 1.2.3.4 permanent Exploit` |
| `.ban allip` | 4 | `.ban allip <name> <time> [reason]` | Bans all IP addresses ever used by designated account. | `.ban allip Badguy 30d Exploit` |
| `.ban fingerprint` | 4 | `.ban fingerprint <print> <time> [reason]` | Bans hardware fingerprint identifier. | `.ban fingerprint 0xDEADBEEF permanent Bot` |
| `.unban account` | 3 | `.unban account <name>` | Removes ban penalty from account. | `.unban account Badguy` |
| `.unban character` | 3 | `.unban character <name>` | Removes ban penalty from character. | `.unban character Badguy` |
| `.unban ip` | 3 | `.unban ip <ip>` | Unbans IP address subnet. | `.unban ip 1.2.3.4` |
| `.baninfo account` | 2 | `.baninfo account <account_name>` | Inspects active account ban reason, issuer, and expiration date. | `.baninfo account Badguy` |
| `.baninfo character` | 2 | `.baninfo character <char_name>` | Inspects active character ban record. | `.baninfo character Badguy` |
| `.baninfo ip` | 2 | `.baninfo ip <ip>` | Inspects active IP ban record. | `.baninfo ip 1.2.3.4` |
| `.ticket list` | 2 | `.ticket list` | Dumps queue of open player GM support tickets. | `.ticket list` |
| `.ticket onlinelist` | 2 | `.ticket onlinelist` | Lists open tickets from currently online players. | `.ticket onlinelist` |
| `.ticket assign` | 2 | `.ticket assign <ticket_id> <gm_name>` | Assigns ticket to designated GM staff member. | `.ticket assign 12 Alice` |
| `.ticket unassign` | 2 | `.ticket unassign <ticket_id>` | Unassigns ticket back to public queue. | `.ticket unassign 12` |
| `.ticket close` | 2 | `.ticket close <ticket_id>` | Closes resolved player ticket. | `.ticket close 12` |
| `.sus enable` | 3 | `.sus enable <0\|1>` | Enables automatic heuristics detector for speed/teleport hacks. | `.sus enable 1` |
| `.sus fishers` | 1 | `.sus fishers` | Audits realm for unattended automated fishing bots. | `.sus fishers` |
| `.sus movementenable` | 3 | `.sus movementenable <0\|1>` | Toggles movement cheat detection. | `.sus movementenable 1` |
| `.sus notify` | 2 | `.sus notify <0\|1>` | Toggles in-game GM alerts when suspicious actions are detected. | `.sus notify 1` |

---

## 11. Turtle-WoW 1.18.1 Custom Systems & Vanity Features

Guild management, housing, Cartographer reveal, and custom challenge modes:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.guild create` | 3 | `.guild create <leader> "<guild_name>"` | Instantly creates guild without charter signature requirement. | `.guild create Alice "Vanguard"` |
| `.guild invite` | 2 | `.guild invite <player> "<guild_name>"` | Forces player or bot into designated guild. | `.guild invite Bob "Vanguard"` |
| `.guild uninvite` | 2 | `.guild uninvite <player>` | Removes player from guild. | `.guild uninvite Bob` |
| `.guild rank` | 3 | `.guild rank <player> <0–9>` | Sets guild rank index (0 = Guild Master). | `.guild rank Bob 1` |
| `.guild rename` | 3 | `.guild rename "<old_name>" "<new_name>"` | Renames existing guild. | `.guild rename "Old" "New"` |
| `.guild leader` | 3 | `.guild leader <player>` | Transfers guild leadership to player. | `.guild leader Bob` |
| `.guild house` | 3 | `.guild house <teleport\|buy>` | Guild housing management: teleport to guild hall or purchase hall. | `.guild house teleport` |
| `.cartographer` | 0 | `.cartographer` | Toggles full map exploration reveal matching Cartographer addon. | `.cartographer` |
| `.hcmessages` | 0 | `.hcmessages <on\|off>` | Toggles global server-wide Hardcore death announcement broadcasts. | `.hcmessages on` |
| `.hcchat` | 0 | `.hcchat <on\|off>` | Toggles Hardcore channel communication mode. | `.hcchat on` |
| `.toggletrainee` | 3 | `.toggletrainee` | Toggles Slow & Steady / Trainee experience challenge mode. | `.toggletrainee` |
| `.toggleinferno` | 3 | `.toggleinferno` | Toggles Inferno hardcore mode challenge. | `.toggleinferno` |
| `.toggleillusions` | 3 | `.toggleillusions` | Toggles custom Illusion visual transfiguration system. | `.toggleillusions` |
| `.tmogdelete` | 3 | `.tmogdelete <item_id>` | Removes item from character transmog appearance library. | `.tmogdelete 19019` |
| `.shellcoin` | 0 | `.shellcoin` | Shows balance of Turtle shell coin vanity tokens. | `.shellcoin` |
| `.radio` | 0 | `.radio <station_id\|stop>` | Plays streaming Turtle radio soundtrack in client. | `.radio 1` |
| `.blockegg` | 3 | `.blockegg <egg_id>` | Blocks or unblocks Easter egg interaction. | `.blockegg 1` |
| `.showborders` | 3 | `.showborders <on\|off>` | Visualizes zone boundary lines in client. | `.showborders on` |
| `.anonymous whispers` | 3 | `.anonymous whispers <on\|off>` | Sends GM whispers without revealing GM character identity. | `.anonymous whispers on` |
| `.anonymous mail` | 3 | `.anonymous mail <on\|off>` | Sends GM mail without revealing character name. | `.anonymous mail on` |
| `.shop log` | 3 | `.shop log <account_id>` | Dumps donation shop transaction purchase logs. | `.shop log 1` |
| `.shop refund` | 3 | `.shop refund <order_id>` | Processes refund for shop donation item. | `.shop refund 42` |

---

## 12. Developer Diagnostics, Navmesh & Waypoints

Universal database entity lookups, memory inspectors, and waypoint path recording:

| Command | Sec | Syntax & Arguments | Description | Example |
| :--- | :---: | :--- | :--- | :--- |
| `.lookup creature` | 2 | `.lookup creature <name\|entry>` | Searches creature database table for matching names and IDs. | `.lookup creature Nefarian` |
| `.lookup item` | 2 | `.lookup item <name>` | Searches item template table for equipment, weapons, and consumables. | `.lookup item Thunderfury` |
| `.lookup spell` | 2 | `.lookup spell <name>` | Searches spell DBC/table for ability names and spell IDs. | `.lookup spell Bloodlust` |
| `.lookup quest` | 2 | `.lookup quest <name>` | Searches quest templates by title. | `.lookup quest Onyxia` |
| `.lookup object` | 2 | `.lookup object <name>` | Searches GameObject templates (chests, doors, levers). | `.lookup object Black Lotus` |
| `.lookup skill` | 2 | `.lookup skill <name>` | Finds profession, weapon, and class skill IDs. | `.lookup skill Swords` |
| `.lookup faction` | 2 | `.lookup faction <name>` | Searches faction templates and reputation IDs. | `.lookup faction Argent Dawn` |
| `.lookup itemset` | 2 | `.lookup itemset <name>` | Searches item sets (Tier 1–3, PvP sets) and outputs set ID. | `.lookup itemset Dreadmist` |
| `.list auras` | 3 | `.list auras` | Dumps all active buffs, debuffs, durations, and stacks on target. | `.list auras` |
| `.list threat` | 1 | `.list threat` | Dumps live threat table values and target rankings for mob. | `.list threat` |
| `.list creature` | 3 | `.list creature <entry_id>` | Lists all active world spawn coordinates and map instances for creature. | `.list creature 10184` |
| `.list object` | 3 | `.list object <entry_id>` | Lists all world spawn instances of specified GameObject. | `.list object 176580` |
| `.wp show` | 4 | `.wp show on\|off [entry]` | Displays visual waypoint path markers (sparks/spawns) in-game. | `.wp show on 10184` |
| `.wp add` | 4 | `.wp add [entry]` | Adds waypoint point at current GM coordinates. | `.wp add 10184` |
| `.wp modify` | 4 | `.wp modify <entry> <point>` | Edits coordinates or delay of existing waypoint node. | `.wp modify 10184 1` |
| `.wp export` | 4 | `.wp export <entry>` | Exports waypoint path data directly to SQL script format. | `.wp export 10184` |
| `.mmaps path` | 3 | `.mmaps path` | Traces navmesh path between player and target, highlighting poly hops. | `.mmaps path` |
| `.mmaps near` | 3 | `.mmaps near` | Shows nearby navmesh polygon boundaries. | `.mmaps near` |
| `.distance` | 3 | `.distance` | Measures exact 3D Euclidean distance in yards between self and target. | `.distance` |
| `.gps` | 1 | `.gps` | Dumps exact X, Y, Z coordinates, orientation, map ID, zone ID, and area ID. | `.gps` |
| `.debug condition` | 3 | `.debug condition <condition_id>` | Tests conditional logic requirements for quests and loot. | `.debug condition 42` |
| `.debug instancedata` | 3 | `.debug instancedata` | Dumps boss encounter kill bitmasks and instance script state. | `.debug instancedata` |
| `.debug packetstats` | 3 | `.debug packetstats` | Displays network packet throughput, opcodes per second, and drops. | `.debug packetstats` |
| `.debug leakreport` | 4 | `.debug leakreport` | Dumps memory leak audit and allocated object reference tallies. | `.debug leakreport` |

---

## 13. Complete Database Hot-Reload Catalog (`.reload`)

Flushes server memory caches and re-reads world database tables without server restart.

### Essential Core Content & Dynamic Module Reloads

```bash
.reload eluna                   # Flushes Lua state and reloads all Eluna scripts & hooks
.reload quest_template          # Reloads quest objectives, prerequisites, and rewards
.reload spell_template          # Reloads spell data, spell effects, and custom spell overrides
.reload creature_template       # Reloads NPC attributes, damage, armor, and base stats
.reload creature                # Reloads NPC in-world placements and active spawns
.reload gameobject_template     # Reloads GameObject metadata, chests, doors, and nodes
.reload gameobject              # Reloads in-world GameObject spawns and coordinates
.reload item_template           # Reloads item stats, weapon damage, requirements, and prices
.reload creature_loot_template  # Flushes and re-reads creature drop tables
.reload gameobject_loot_template # Flushes and re-reads chest and node loot tables
.reload areatrigger_teleport    # Reloads dungeon and world teleport entrance triggers
.reload config                  # Re-reads mangosd.conf server configuration values
.reload housing                 # Reloads Turtle-WoW custom guild housing coordinates
```

### Complete Target Directory (`.reload <target>`) Verified from `src/game/Chat/Chat.cpp`

```
account_banned                  areatrigger_involvedrelation    areatrigger_tavern
areatrigger_teleport            autobroadcast                   bgplayers
character_pet                   cinematic_waypoints             conditions
config                          creature                        creature_ai_events
creature_battleground           creature_display_info_addon     creature_groups
creature_involvedrelation       creature_loot_template          creature_onkill_reputation
creature_questrelation          creature_spells                 creature_spells_scripts
creature_template               disenchant_loot_template        eluna
event_scripts                   exploration_basexp              fishing_loot_template
game_graveyard_zone             game_tele                       game_weather
gameobject                      gameobject_battleground         gameobject_involvedrelation
gameobject_loot_template        gameobject_questrelation        gameobject_requirement
gameobject_scripts              generic_scripts                 gm_ticket_template
gmlevels                        gossip_menu                     gossip_menu_option
gossip_scripts                  housing                         instance_buff_removal
ip_banned                       item_enchantment_template       item_loot_template
item_required_target            item_template                   locales_creature
locales_gameobject              locales_gossip_menu_option      locales_item
locales_page_text               locales_points_of_interest      locales_quest
mail_loot_template              mangos_string                   map_loot_disabled
module_string                   module_string_locale            npc_gossip
npc_text                        npc_trainer                     npc_vendor
page_text                       pet_name_generation             petitions
pickpocketing_loot_template     player_factionchange_items      player_factionchange_mounts
player_factionchange_quests     player_factionchange_reputations player_factionchange_spells
points_of_interest              quest_end_scripts               quest_greeting
quest_start_scripts             quest_template                  reference_loot_template
reputation_reward_rate          reputation_spillover_template   reserved_name
shop                            skill_fishing_base_level        skinning_loot_template
spell_affect                    spell_area                      spell_chain
spell_disabled                  spell_elixir                    spell_group
spell_group_stack_rules         spell_learn_spell               spell_mod
spell_pet_auras                 spell_proc_event                spell_proc_item_enchant
spell_script_target             spell_scripts                   spell_target_position
spell_threats                   taxi_path_transitions           trainer_greeting
variables                       visibilities
```

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [⚔️ Volume 04: BiS Gear](./04_class_specs_and_endgame_gear.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md)
