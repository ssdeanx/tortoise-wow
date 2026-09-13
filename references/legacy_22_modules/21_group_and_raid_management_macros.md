# 21. Group & Raid Management Macros (Master Control Center)

> **Target Core**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + PlayerBots Framework + mod-dungeon-clear  
> **Source Verification**: `modules/mod-playerbots/src/playerbot/strategy/generic/ChatCommandHandlerStrategy.cpp`, `ChangeStrategyAction.cpp`, `modules/mod-dungeon-clear/src/DungeonClearCommand.cpp`  
> **Quick Navigation**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📖 Project Readme](../README.md) • [🎯 Bot Macro Book (13_bot_whispers_and_macros.md)](./13_bot_whispers_and_macros.md) • [⚔️ Class Strategies (14_bot_strategies_and_tactics.md)](./14_bot_strategies_and_tactics.md) • [🏰 Dungeon Clear AI (15_dungeon_clear_module.md)](./15_dungeon_clear_module.md) • [🛡️ Endgame Gear (17_tier_items_and_endgame_gear.md)](./17_tier_items_and_endgame_gear.md)

---

## Complete 21-Module Reference Library

| Security & Server | World & Characters | Gameplay & Items | Bots & Modules |
| :--- | :--- | :--- | :--- |
| [00. Self & Own Character](./00_self_and_player_commands.md) | [03. Teleports & Movement](./03_teleports_and_movement.md) | [06. Quests & Instances](./06_quests_instances_and_events.md) | [11. Playerbot Suite](./11_playerbot_suite.md) |
| [01. Security & Accounts](./01_security_and_account.md) | [04. Stats & Visuals](./04_character_stats_and_visuals.md) | [07. Items & Economy](./07_items_economy_and_mail.md) | [12. RandomBot Manager](./12_randombot_manager.md) |
| [02. Server & Reloads](./02_server_and_reloads.md) | [05. Combat & Spells](./05_combat_spells_and_skills.md) | [08. Spawning & Editing](./08_spawning_and_world_editing.md) | [13. Bot Whispers & Macros](./13_bot_whispers_and_macros.md) |
| [09. Moderation & Anticheat](./09_moderation_tickets_and_anticheat.md) | [10. Turtle Custom](./10_turtle_custom_features.md) | [17. Tier Sets & Gear](./17_tier_items_and_endgame_gear.md) | [14. Bot Strategies & Tactics](./14_bot_strategies_and_tactics.md) |
| [16. Diagnostics & Lookups](./16_developer_and_diagnostics.md) | [18. Class Talent Specs](./18_class_talent_specs.md) | [20. Warrior Endgame & Raids](./20_warrior_dps_dungeons_and_raids.md) | [15. Dungeon Clear AI](./15_dungeon_clear_module.md) |
| *(Server Scripting)* | *(Engine Core)* | *(Runtime Lua API)* | [19. Eluna Lua Engine](./19_eluna_lua_engine.md) |
| *(Master Hub)* | *(Raid Commands)* | *(Bot Operations)* | **[21. Group & Raid Macros](./21_group_and_raid_management_macros.md)** |

---

## 1. The Master Macro UI Cheat Sheet (Copy-Paste Ready)

This reference module is your **single unified command deck** for running 5-man dungeons and 10/20/40-man raids with bots. Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`).

### 📌 How Target Tokens (`%t`) Work in Macros

* Whenever you see `/w %t ...`, the `%t` token is dynamically replaced with the **name of the bot you currently have targeted** (either in the 3D world or on your party frame).
* **You only need to run spec macros ONCE per bot!** The server commits strategy modifications directly to the character database (`sPlayerbotDbStore.Save(ai)`).
* For universal group orders, use `/p @all ...` (Party) or `/ra @all ...` (Raid).

---

## 2. Group Assembly, Gearing & Preparation Macros

### 🏃 Macro 1: 1-Click Bot Summon & Follow Lock

Teleports all bots in your party/raid directly to your feet and locks them in follow formation:

```lua
.bot summon *
/p @all follow
```

### 💎 Macro 2: 1-Click Bot Level 60 Epics, Enchants & Stocking

Equips all party bots with level-scaled BiS Epic gear, applies permanent weapon/armor enchants, trains all class spells, and restocks class reagents (Flash Powder, Ankhs, Soul Shards, Pet Food, Arrows/Bullets):

```lua
.bot gear * epic
.bot enchants *
.bot consumes *
.bot prep *
```

### 📋 Macro 3: Party / Raid Ready Check

Broadcasts a ready check across all bot party members:

```lua
/p @all ready
```

---

## 3. Class Role & Combat Strategy Assignment Deck (`/w %t`)

Target each party bot once and tap their respective role macro:

### 🛡️ Tanks

#### Protection Warrior (Main Tank)

Equips threat rotation, taunts, shield blocks, potion drinking, and food recovery:

```lua
/w %t role tank
/w %t co +tank,prot,tank assist,avoid aoe,potions
/w %t nc +food
```

#### Protection Paladin (AoE & Undead Tank)

Deploys Consecration, Holy Shield, Righteous Fury, and potion drinking:

```lua
/w %t role tank
/w %t co +tank,prot,consecration,avoid aoe,potions
/w %t nc +food
```

#### Feral Bear Druid (Armor & HP Tank)

Shifts to Dire Bear Form, casts Maul, Swipe, Enrage, and uses potions:

```lua
/w %t role tank
/w %t co +tank,bear,tank assist,avoid aoe,potions
/w %t nc +food
```

---

### ✨ Healers

#### Holy Priest (Raid & Tank Healer)

Enables priority Flash Heal, Renew, Prayer of Healing, mana conservation, and potions:

```lua
/w %t role healer
/w %t co +heal,holy,conserve mana,avoid aoe,potions
/w %t nc +food
```

#### Restoration Shaman (Chain Healer & Totems)

Deploys Windfury/Mana Spring totems, Chain Heal rotation, and mana potions:

```lua
/w %t role healer
/w %t co +heal,restoration,totems,conserve mana,potions
/w %t nc +food
```

#### Restoration Druid (HoT & Tank Support)

Maintains Rejuvenation and Regrowth, casts Healing Touch, and casts Innervate:

```lua
/w %t role healer
/w %t co +heal,restoration,conserve mana,potions
/w %t nc +food
```

#### Holy Paladin (Single-Target Tank Healer)

Casts Holy Light, Flash of Light, Blessings, cleanses magic/poison, and uses potions:

```lua
/w %t role healer
/w %t co +heal,holy,conserve mana,avoid aoe,potions
/w %t nc +food
```

---

### ⚔️ Melee DPS

#### Combat Rogue (Behind Flank DPS)

Positions behind boss, stealth opener, Sinister Strike, Slice and Dice, Eviscerate, and Blade Flurry:

```lua
/w %t co +dps,combat,behind,stealth,threat,potions
/w %t nc +food
```

#### Fury Warrior (Dual-Wield Melee DPS)

Positions behind boss, casts Bloodthirst, Whirlwind, Heroic Strike, and Execute:

```lua
/w %t co +dps,fury,behind,threat,avoid aoe,potions
/w %t nc +food
```

#### Retribution Paladin (2-Handed Melee DPS)

Casts Seal of Command, Judgement, Crusader Strike, and Sanctity Aura:

```lua
/w %t co +dps,retribution,behind,threat,potions
/w %t nc +food
```

#### Enhancement Shaman (Windfury Melee DPS)

Deploys Windfury Weapon/Totem, Stormstrike, Earth Shock, and Fire Nova:

```lua
/w %t co +dps,enhancement,behind,totems,threat,potions
/w %t nc +food
```

#### Feral Cat Druid (Behind Bleed DPS)

Shifts to Cat Form, opens from stealth, Shred, Rake, Rip, and Ferocious Bite:

```lua
/w %t co +dps,cat,behind,stealth,threat,potions
/w %t nc +food
```

---

### 🏹 Ranged & Caster DPS

#### Marksmanship Hunter (Ranged DPS & Pet)

Enables Aimed Shot, Multi-Shot, Serpent Sting, Hunter's Mark, and Pet attack coordination:

```lua
/w %t co +dps,marksmanship,threat,pet,potions
/w %t nc +food
```

#### Fire Mage (Maximum Burst DPS)

Casts Fireball, Pyroblast, Scorch, Combustion, and manages threat/potions:

```lua
/w %t co +dps,fire,threat,avoid aoe,potions
/w %t nc +food
```

#### Frost Mage (Control & AoE DPS)

Casts Frostbolt, Blizzard, Cone of Cold, and Ice Barrier:

```lua
/w %t co +dps,frost,threat,avoid aoe,potions
/w %t nc +food
```

#### Affliction / Destruction Warlock (Shadow DPS & Pet)

Applies Corruption, Curse of Shadows/Elements, Shadow Bolt, and Life Tap management:

```lua
/w %t co +dps,affliction,threat,pet,potions
/w %t nc +food
```

#### Shadow Priest (Shadow Caster DPS)

Enables Shadowform, Mind Blast, Shadow Word: Pain, and Mind Flay:

```lua
/w %t co +dps,shadow,threat,avoid aoe,potions
/w %t nc +food
```

#### Balance Druid (Boomkin Caster DPS)

Shifts to Moonkin Form, casts Starfire, Wrath, and Moonfire:

```lua
/w %t co +dps,balance,threat,avoid aoe,potions
/w %t nc +food
```

---

## 4. Live Group & Raid Combat Controls (`/p @all` and `/ra @all`)

### 🛡️ Macro 4: Main Tank Pull & Hold Aggro

Orders the designated tank bot to charge and establish maximum threat on selected target:

```lua
/p @all tank attack
```

### ⚔️ Macro 5: All DPS Engage Target (Maximum Burst)

Orders all DPS and pet bots to charge selected target and activate burst cooldowns:

```lua
/p @all attack
/p @all max dps
```

### 🏹 Macro 6: Controlled Ranged Pull to Master

Orders a ranged bot to shoot/cast on target and bring them back while the rest of the party holds position:

```lua
/p @all pull
/p @all stay
```

### 💀 Macro 7: Focus Fire Raid Target Icon (Skull)

Orders all bots to focus down the target marked with the Skull icon:

```lua
/p @all attack rti
```

### 🚨 Macro 8: Emergency Panic Disengage & Flee to Master

Forces all bots to drop combat actions, break target lock, and sprint directly to your feet:

```lua
/p @all flee
/p @all follow
```

### 💥 Macro 9: Area-of-Effect Burst Mode (AoE Trash)

Commands spellcasters to prioritize Area-of-Effect abilities (Blizzard, Rain of Fire, Hurricane, Volley, Consecration):

```lua
/p @all aoe
```

### 🛑 Macro 10: Prioritize Interrupts & Counterspells

Orders bots with kicks, counterspells, pummel, and earth shock to prioritize interrupting enemy casts:

```lua
/p @all interrupt
```

### 🧹 Macro 11: Mass Dispel & Cleanse Debuffs

Instructs healers and support bots to prioritize removing Magic, Poison, Disease, and Curse debuffs:

```lua
/p @all clean
```

---

## 5. Party Formations & Precision Positioning

| Macro Name | Lua Macro | Tactical Purpose |
| :--- | :--- | :--- |
| **🛡️ Defensive Shield Formation** | `/p @all formation shield`<br/>`/p @all follow` | Tanks position in front; healers and ranged casters stay safely behind. |
| **🏹 Forward Arrow Wedge** | `/p @all formation arrow`<br/>`/p @all follow` | Forward offensive wedge for trash corridor pushes. |
| **⭕ Circle Perimeter** | `/p @all formation circle`<br/>`/p @all follow` | Spreads bots evenly in a 360-degree perimeter around the player. |
| **📏 Horizontal Flank Line** | `/p @all formation line`<br/>`/p @all follow` | Spreads bots horizontally to avoid front/rear cone breath attacks. |
| **🛑 Hold Ground / Freeze** | `/p @all stay` | Freezes bot movement instantly (essential before line-of-sight pulls). |
| **🏃 Resume Close Stacking** | `/p @all follow` | Restores standard follow behavior behind player character. |

---

## 6. Autonomous Dungeon Clear AI Deck (`.dc`)

For hands-free dungeon navigation where the tank bot calculates waypoints, manages healer mana thresholds (>60%), and executes boss pulls:

### 🏰 Macro 12: Activate Autonomous Dungeon Solver

```lua
.dc on
/p @all co +dungeon clear,avoid aoe,roll,loot,potions
/p @all nc +food
/p @all formation near
/p @all follow
```

### ⏸️ Macro 13: Pause / Resume Autonomous Run

```lua
.dc pause
```

*(Run `.dc on` to resume advancement).*

### 🎯 Macro 14: Order Instant Tank Pull on Next Pack

Forces the tank bot to immediately initiate a pull on the current targeted enemy pack:

```lua
.dc pull
```

### ⏩ Macro 15: Skip Current Trash Pack or Boss

Instructs the tank routing solver to bypass the current encounter and advance to the next objective:

```lua
.dc skip
```

### 📊 Macro 16: Instance Progression & Boss Bitmask Audit

Displays live instance completion percentage, defeated bosses, skipped mobs, and wipe counters:

```lua
.dc status
.dc bosses
```

---

## 7. Post-Combat Recovery, Looting & Breaches

### 💰 Macro 17: Fast Post-Boss Loot, Drink & Rebuff

Orders party to loot all boss/trash corpses, eat/drink for full mana, and rebuff missing class auras:

```lua
/p @all loot
/p @all food
/p @all buff
```

### 🚪 Macro 18: Breach & Interact with Nearest Door / Lever / Gate

Targets the nearest locked door, gate, or lever and orders bots to interact and unlock:

```lua
.gobject target
/p @all talk
```

### 🛠️ Macro 19: Order Bots to Repair Gear & Sell Junk at Vendor

Target any friendly repair vendor NPC and run:

```lua
/p @all repair
/p @all s all gray
```

### 🤝 Macro 20: Trade & Force Equip Best Gear on Bot

Opens a trade window with the targeted bot and instructs it to equip the highest-stat items from its bags:

```lua
/w %t trade
/w %t equip
```

---

## 8. Raid Coordination (10-Man, 20-Man & 40-Man)

When scaling up from 5-man dungeons to 10-man (UBRS, Karazhan, Scarlet Citadel), 20-man (ZG, AQ20, Timbermaw Hold), or 40-man raids (MC, BWL, AQ40, Naxx, Emerald Sanctum):

### Step 1: Convert Group to Raid

Right-click your player portrait and select **Convert to Raid**, or run:

```lua
/raid
```

### Step 2: Raid Channel Broadcasts

In raid groups, replace `/p` with `/ra` to reach all party groups (1 through 8):

```lua
/ra @all buff
/ra @all food
/ra @all ready
/ra @all follow
```

### Step 3: Assign Main Tank & Off-Tanks

Target your primary tank bot and assign role:

```lua
/w %t role tank
```

Target secondary tank bots and assign off-tank role:

```lua
/w %t role ot
```

*(All healer bots automatically prioritize healing the Main Tank first, followed by Off-Tanks, and then low-HP raid members).*

---

## 9. Strategy Diagnostics & Reset Tools

### ❓ Macro 21: Query Target Bot's Active Strategies

Whispers the targeted bot to list all currently enabled combat (`co`), non-combat (`nc`), and dead (`de`) strategies:

```lua
/w %t co ?
/w %t nc ?
```

### 🔄 Macro 22: Reset Target Bot Strategies to Class Defaults

Clears custom strategy overrides and returns bot to baseline class rotation:

```lua
/w %t reset strats
```

### 🎒 Macro 23: Inspect Target Bot Inventory

Whispers selected bot to list free bag slots, gold, and item contents:

```lua
/w %t c
```

---

[⬅ Prev: 20. Warrior Endgame & Raids](./20_warrior_dps_dungeons_and_raids.md) • [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📖 Project Readme](../README.md) • [🎯 Bot Macro Book (13_bot_whispers_and_macros.md)](./13_bot_whispers_and_macros.md)
