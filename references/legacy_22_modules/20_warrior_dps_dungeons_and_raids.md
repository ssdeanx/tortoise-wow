---
title: "20. Warrior DPS Master Guide: Dungeons, Raids, Attunements & Turtle-WoW Endgame"
version: "1.3.0"
author: "Sam"
created: "2026-09-04"
updated: "2026-09-04"
---

> **Target Core**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS Engine + PlayerBots Framework
> **Client Macro Engine**: Vanilla 1.12.1 Lua API (`/script`, `/run`, `CastSpellByName`, `SpellStopCasting`, `GetShapeshiftFormInfo`)
> **Role Specialization**: Melee DPS (Dual-Wield Fury & Two-Handed Arms / Cleave)
> **Quick Navigation**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [⚡ Self Commands (00_self_and_player_commands.md)](./00_self_and_player_commands.md) • [🗺️ Teleport Directory (03_teleports_and_movement.md)](./03_teleports_and_movement.md) • [🎒 Endgame Gear & BiS (17_tier_items_and_endgame_gear.md)](./17_tier_items_and_endgame_gear.md) • [⚔️ Bot Tactics (14_bot_strategies_and_tactics.md)](./14_bot_strategies_and_tactics.md) • [🏰 Dungeon Clear AI (15_dungeon_clear_module.md)](./15_dungeon_clear_module.md)

---

## Complete 21-Module Reference Library

| Security & Server | World & Characters | Gameplay & Items | Bots & Modules |
| :--- | :--- | :--- | :--- |
| [00. Self & Own Character](./00_self_and_player_commands.md) | [03. Teleports & Movement](./03_teleports_and_movement.md) | [06. Quests & Instances](./06_quests_instances_and_events.md) | [11. Playerbot Suite](./11_playerbot_suite.md) |
| [01. Security & Accounts](./01_security_and_account.md) | [04. Stats & Visuals](./04_character_stats_and_visuals.md) | [07. Items & Economy](./07_items_economy_and_mail.md) | [12. RandomBot Manager](./12_randombot_manager.md) |
| [02. Server & Reloads](./02_server_and_reloads.md) | [05. Combat & Spells](./05_combat_spells_and_skills.md) | [08. Spawning & Editing](./08_spawning_and_world_editing.md) | [13. Bot Whispers & Macros](./13_bot_whispers_and_macros.md) |
| [09. Moderation & Anticheat](./09_moderation_tickets_and_anticheat.md) | [10. Turtle Custom](./10_turtle_custom_features.md) | [17. Tier Sets & Gear](./17_tier_items_and_endgame_gear.md) | [14. Bot Strategies & Tactics](./14_bot_strategies_and_tactics.md) |
| [16. Diagnostics & Lookups](./16_developer_and_diagnostics.md) | [18. Class Talent Specs](./18_class_talent_specs.md) | *(Standalone Portals)* | [15. Dungeon Clear AI](./15_dungeon_clear_module.md) |
| *(Server Scripting)* | *(Engine Core)* | *(Runtime Lua API)* | [19. Eluna Lua Engine](./19_eluna_lua_engine.md) |
| **[20. Warrior DPS & Raids](./20_warrior_dps_dungeons_and_raids.md)** | *(Master Hub)* | *(Raid Commands)* | **[21. Group & Raid Macros](./21_group_and_raid_management_macros.md)** |

---

## 1. 1-Click Warrior DPS Master Bootstrap

Execute these commands in chat or bind them to an in-game macro (`/m`) to completely configure your character, skills, bags, travel, gear, enchants, consumables, keys, and world buffs:

```lua
-- 1. Level, Spells, Max Weapon & Defense Skills
.levelup 60
.learn all_myspells
.learn all_trainer
.learn all_recipes
.maxskill
.modify money 10000000
.additem 14156 4
.taxicheat on
.explorecheat on

-- 2. Master Dungeon & Raid Keyring (Classic + Turtle Custom)
.additem 12344 1   -- Seal of Ascension (Upper Blackrock Spire 10-Man Door)
.additem 13704 1   -- Skeleton Key (Scholomance Outside Gate)
.additem 12382 1   -- Key to the City (Stratholme Service Gate)
.additem 11000 1   -- Shadowforge Key (Blackrock Depths Gates & Coffer)
.additem 11140 1   -- Prison Cell Key (BRD Grim Guzzler / Jail)
.additem 18249 1   -- Crescent Key (Dire Maul North/West/East Libraries)
.additem 7146 1    -- The Scarlet Key (Scarlet Monastery Armory & Cathedral)
.additem 9287 1    -- Workshop Key (Gnomeregan Backdoor)
.additem 5396 1    -- Key to Searing Gorge
.additem 9240 1    -- Mallet of Zul'Farrak (Summons Gahz'rilla in ZF)
.additem 17191 1   -- Scepter of Celebras (Maraudon Inner Princess Portal)
.additem 16309 1   -- Drakefire Amulet (Onyxia's Lair Raid Attunement)
.additem 22754 1   -- Eternal Quintessence (Molten Core Boss Rune Dousing)
.additem 50545 1   -- Gemstone of Ysera (Emerald Sanctum Custom 40m Attunement)
.additem 61234 1   -- Upper Karazhan Tower Key (Custom Karazhan 10m/40m Key)
.additem 51356 1   -- Karazhan Crypt Key (Custom Karazhan Crypts Key)
.additem 42379 1   -- Rune of Illumination (Timbermaw Hold Custom 20m Key)
.additem 53005 1   -- Scarlet Crusade Insignia (Scarlet Citadel Custom 10m Access)

-- 3. High-End Consumables & Buff Imbues Stock
.additem 13510 5   -- Flask of the Titans (+1200 Max HP)
.additem 13452 20  -- Elixir of the Mongoose (+25 Agility, +2% Crit)
.additem 9206 20   -- Elixir of Giants (+25 Strength)
.additem 13442 20  -- Mighty Rage Potion (Instant 45-75 Rage + 60 Str for 20s!)
.additem 13446 20  -- Major Healing Potion
.additem 3387 10   -- Limited Immunity Potion (Immune to physical attacks for 6s - drops aggro!)
.additem 5634 10   -- Free Action Potion (Immune to Stuns/Immobilize for 30s)
.additem 18262 20  -- Elemental Sharpening Stone (+2% Crit for 30m)
.additem 12820 20  -- Winterfall Firewater (+35 Attack Power)
.additem 20452 40  -- Smoked Desert Dumplings (+20 Strength Food)
.additem 12451 20  -- Juju Power (+30 Strength for 30m)
.additem 12460 20  -- Juju Might (+40 Attack Power for 10m)
.additem 12450 20  -- Juju Flurry (+3% Attack Speed for 20s)
.additem 8410 5    -- R.O.I.D.S. (+25 Strength for 60m - Blasted Lands)
.additem 8412 5    -- Ground Scorpok Assay (+25 Agility for 60m - Blasted Lands)
.additem 61181 10  -- Potion of Quickness (Turtle Custom: +Haste for 30s)
.additem 61225 10  -- Lucidity Potion (Turtle Custom: Immune to Sleep/Charm/Polymorph)
.additem 15997 200 -- Thorium Shells (Ranged ammo for gun pulls)

-- 4. Auto-Apply Permanent Enchants & Durability
.bot enchants *
.repairitems
.replenish
```

---

## 2. Instant GM Skip Suite: Attunements, Reputations, Arcanums & Buffs (No Quests Required)

If you don't want to run long quest chains or farm reputation, copy and run these command blocks to unlock everything instantly:

### A. Instant Attunement Auto-Complete & Keyring (Classic & Turtle Custom)

```lua
-- Classic Raids & Dungeons Attunement Quests
.quest add 7848
.quest complete 7848       -- Molten Core (Attunement to the Core)
.quest add 7761
.quest complete 7761       -- Blackwing Lair (Blackhand's Command)
.quest add 6502
.quest complete 6502       -- Onyxia's Lair Alliance (Drakefire Amulet)
.quest add 6602
.quest complete 6602       -- Onyxia's Lair Horde (Blood of the Black Dragon Champion)
.quest add 9123
.quest complete 9123       -- Naxxramas Exalted Attunement (The Dread Citadel)

-- Turtle-WoW Custom Raids Attunement Quests
.quest add 40962
.quest complete 40962      -- Emerald Sanctum 40-Man (Into the Dream VI)
.quest add 40829
.quest complete 40829      -- Karazhan 10-Man / 40-Man (The Key to Karazhan X)
.quest add 41931
.quest complete 41931      -- Timbermaw Hold 20-Man (The Corruption of Timbermaw Hold)
.quest add 40489
.quest complete 40489      -- Hateforge Quarry 5-Man (Assaulting Hateforge)

-- Master Physical Key & Access Item Spawner
.additem 16309 1           -- Drakefire Amulet (Onyxia Key)
.additem 22754 1           -- Eternal Quintessence (Molten Core Infinite Rune Dousing)
.additem 12344 1           -- Seal of Ascension (UBRS 10-Man Gate Key)
.additem 13704 1           -- Skeleton Key (Scholomance Key)
.additem 12382 1           -- Key to the City (Stratholme Service Gate Key)
.additem 11000 1           -- Shadowforge Key (BRD Gates Key)
.additem 11140 1           -- Prison Cell Key (BRD Prison Key)
.additem 11144 12          -- Relic Coffer Key (BRD Secret Vault Safe)
.additem 18249 1           -- Crescent Key (Dire Maul North/West/East Key)
.additem 7146 1            -- The Scarlet Key (Scarlet Monastery Key)
.additem 9287 1            -- Workshop Key (Gnomeregan Backdoor Key)
.additem 5396 1            -- Key to Searing Gorge
.additem 9240 1            -- Mallet of Zul'Farrak (Gahz'rilla Summon Gong)
.additem 17191 1           -- Scepter of Celebras (Maraudon Inner Portal)
.additem 50545 1           -- Gemstone of Ysera (Emerald Sanctum 40m Key)
.additem 61234 1           -- Upper Karazhan Tower Key (Tower of Karazhan Key)
.additem 51356 1           -- Karazhan Crypt Key (Karazhan Crypts Key)
.additem 42379 1           -- Rune of Illumination (Timbermaw Hold 20m Key)
.additem 53005 1           -- Scarlet Crusade Insignia (Scarlet Citadel 10m Key)
.additem 60302 1           -- Lapidis Tower Key (Lapidis Isle Key)
```

### B. Instant Maximum Faction Reputations (Exalted)

```lua
.modify rep 529 42999      -- Argent Dawn (Exalted: Free Naxxramas Attunement & Shoulder Enchants)
.modify rep 270 42999      -- Zandalar Tribe (Exalted: ZG Class Enchants & +30 AP Shoulder Signet)
.modify rep 749 42999      -- Hydraxian Waterlords (Exalted: Eternal Quintessence)
.modify rep 576 42999      -- Timbermaw Hold (Exalted: Custom Timbermaw Raid Access & Recipes)
.modify rep 910 42999      -- Brood of Nozdormu (Exalted: AQ40 Ring Upgrades)
.modify rep 609 42999      -- Cenarion Circle (Exalted: AQ20 & Earthstrike Trinket)
```

### C. Database Arcanums & Head / Leg / Shoulder Enchants Catalog

Scanned directly from `tw_world.item_template` and `tw_world.spell_template`:

| Item ID | Item Name | Spell ID | Slot | Stat / Enchant Bonus | Source & Instant Command |
| :---: | :--- | :---: | :---: | :--- | :--- |
| **`11645`** | Lesser Arcanum of Voracity | `15397` | Head / Legs | **+8 Strength** (BiS DPS) | `.additem 11645 5` (Libram of Voracity `11737`) |
| **`11647`** | Lesser Arcanum of Voracity | `15402` | Head / Legs | **+8 Agility** | `.additem 11647 5` (Libram of Voracity `11737`) |
| **`11646`** | Lesser Arcanum of Voracity | `15400` | Head / Legs | **+8 Stamina** | `.additem 11646 5` (Libram of Voracity `11737`) |
| **`11648`** | Lesser Arcanum of Voracity | `15404` | Head / Legs | **+8 Intellect** | `.additem 11648 5` (Libram of Voracity `11737`) |
| **`11649`** | Lesser Arcanum of Voracity | `15406` | Head / Legs | **+8 Spirit** | `.additem 11649 5` (Libram of Voracity `11737`) |
| **`18329`** | Arcanum of Rapidity | `22840` | Head / Legs | **+1% Haste / Attack Speed** | `.additem 18329 5` (Libram of Rapidity `18332`) |
| **`18330`** | Arcanum of Focus | `22844` | Head / Legs | **+12 Spell Damage / Healing** | `.additem 18330 5` (Libram of Focus `18333`) |
| **`18331`** | Arcanum of Protection | `22846` | Head / Legs | **+1% Dodge** | `.additem 18331 5` (Libram of Protection `18334`) |
| **`11642`** | Lesser Arcanum of Constitution | `15389` | Head / Legs | **+100 Health** | `.additem 11642 5` (Libram of Constitution `11733`) |
| **`11643`** | Lesser Arcanum of Tenacity | `15391` | Head / Legs | **+125 Armor** | `.additem 11643 5` (Libram of Tenacity `11734`) |
| **`11644`** | Lesser Arcanum of Resilience | `15394` | Head / Legs | **+20 Fire Resistance** | `.additem 11644 5` (Libram of Resilience `11736`) |
| **`11622`** | Lesser Arcanum of Rumination | `15340` | Head / Legs | **+150 Mana** | `.additem 11622 5` (Libram of Rumination `11732`) |
| **`19782`** | Presence of Might | `24149` | Head / Legs | **+10 Stamina, +7 Defense, +15 Block Value** | `.additem 19782 5` (Warrior ZG Class Arcanum) |
| **`19784`** | Death's Embrace | `24161` | Head / Legs | **+28 Attack Power, +1% Dodge** | `.additem 19784 5` (Rogue ZG Class Arcanum) |
| **`19785`** | Falcon's Call | `24162` | Head / Legs | **+24 Ranged AP, +10 Stamina, +1% Hit** | `.additem 19785 5` (Hunter ZG Class Arcanum) |
| **`19783`** | Syncretist's Sigil | `24160` | Head / Legs | **+10 Stamina, +7 Defense, +24 Healing** | `.additem 19783 5` (Paladin ZG Class Arcanum) |
| **`23548`** | Might of the Scourge | `29483` | Shoulders | **+26 Attack Power, +1% Crit** (BiS DPS) | `.additem 23548 5` (Naxxramas Sapphiron Drop) |
| **`23549`** | Fortitude of the Scourge | `29480` | Shoulders | **+16 Stamina, +100 Armor** (BiS Tank) | `.additem 23549 5` (Naxxramas Sapphiron Drop) |
| **`23545`** | Power of the Scourge | `29467` | Shoulders | **+15 Spell Dmg/Heal, +1% Spell Crit** | `.additem 23545 5` (Naxxramas Sapphiron Drop) |
| **`23547`** | Resilience of the Scourge | `29475` | Shoulders | **+31 Healing, +5 MP5** | `.additem 23547 5` (Naxxramas Sapphiron Drop) |
| **`20077`** | Zandalar Signet of Might | `24422` | Shoulders | **+30 Attack Power** | `.additem 20077 5` (Zandalar Tribe Exalted) |
| **`20076`** | Zandalar Signet of Mojo | `24421` | Shoulders | **+18 Spell Damage / Healing** | `.additem 20076 5` (Zandalar Tribe Exalted) |
| **`20078`** | Zandalar Signet of Serenity | `24420` | Shoulders | **+33 Healing** | `.additem 20078 5` (Zandalar Tribe Exalted) |
| **`22635`** | Savage Guard | `28161` | Head / Legs | **+10 Nature Resistance** | `.additem 22635 5` (Cenarion Exalted) |
| **`22636`** | Ice Guard | `28163` | Head / Legs | **+10 Frost Resistance** | `.additem 22636 5` (Argent Dawn Exalted) |
| **`22638`** | Shadow Guard | `28165` | Head / Legs | **+10 Shadow Resistance** | `.additem 22638 5` (Argent Dawn Exalted) |
| **`18182`** | Chromatic Mantle of the Dawn | `23184` | Shoulders | **+5 All Resistances** | `.additem 18182 5` (Argent Dawn Revered) |

```lua
-- 1-Click Spawner for Warrior BiS Arcanums & Shoulder Enchants
.additem 11645 5   -- Lesser Arcanum of Voracity (+8 Strength)
.additem 18329 5   -- Arcanum of Rapidity (+1% Haste)
.additem 19782 5   -- Presence of Might (Warrior ZG: +10 Stam, +7 Def, +15 Block)
.additem 23548 5   -- Might of the Scourge (+26 AP, +1% Crit)
.additem 20077 5   -- Zandalar Signet of Might (+30 AP)
.additem 23549 5   -- Fortitude of the Scourge (+16 Stam, +100 Armor)
```

### D. Scanned Buffs, Auras & Consumables Catalog

Scanned from `tw_world.spell_template`. Apply them directly to your character using `.aura <id>`:

#### 1. World Buffs (Working in 5-Mans & Open World)

```lua
.aura 22888    -- Rallying Cry of the Dragonslayer (+140 AP, +5% Melee/Ranged Crit, +10% Spell Crit for 2h)
.aura 16609    -- Warchief's Blessing (+300 HP, +15% Melee Haste, 10 MP5 for 1h)
.aura 15366    -- Songflower Serenade (+15 to All Stats, +5% Crit for 1h)
.aura 24425    -- Spirit of Zandalar (+15% to All Stats, +10% Movement Speed for 2h)
.aura 22817    -- Fengus' Ferocity (Dire Maul North Tribute: +200 Attack Power for 2h)
.aura 22818    -- Moldar's Moxie (Dire Maul North Tribute: +15% Stamina for 2h)
.aura 22820    -- Slip'kik's Savvy (Dire Maul North Tribute: +3% Spell Crit for 2h)
.aura 23768    -- Sayge's Dark Fortune of Damage (Darkmoon Faire: +10% Total Damage Dealt for 2h)
.aura 26393    -- Traces of Silithyst (Silithus Geyser: +5% Damage Dealt for 30m)
```

#### 2. Consumable Buff Auras (Instant, Bags Not Required)

```lua
-- Classic Flasks, Elixirs & Food
.aura 17628    -- Flask of the Titans (+1200 Max Health)
.aura 17538    -- Elixir of the Mongoose (+25 Agility, +2% Crit)
.aura 11405    -- Elixir of Giants (+25 Strength)
.aura 17038    -- Winterfall Firewater (+35 Attack Power)
.aura 25661    -- Well Fed: Smoked Desert Dumplings (+20 Strength)
.aura 18192    -- Well Fed: Grilled Squid (+10 Agility)
.aura 25660    -- Well Fed: Dirge's Kickin' Chimaerok Chops (+25 Stamina)
.aura 22756    -- Elemental Sharpening Stone (+2% Melee Crit on Weapon)
.aura 17539    -- Greater Stoneshield (+2000 Armor for 2m)
-- Winterspring Jujus (Frostmaul E'ko)
.aura 16323    -- Juju Power (+30 Strength for 30m - stacks with Giants/Mongoose!)
.aura 16329    -- Juju Might (+40 Attack Power for 10m - stacks with Firewater!)
.aura 16322    -- Juju Flurry (+3% Attack Speed for 20s)
.aura 16321    -- Juju Escape (+5% Dodge for 10s)
.aura 16326    -- Juju Ember (+15 Fire Resistance for 10m)
.aura 16325    -- Juju Chill (+15 Frost Resistance for 10m)
-- Blasted Lands Consumables
.aura 10667    -- R.O.I.D.S. (+25 Strength for 60m)
.aura 10669    -- Ground Scorpok Assay (+25 Agility for 60m)
.aura 10668    -- Lung Juice Cocktail (+25 Stamina for 60m)
-- Turtle-WoW Custom Consumable Auras
.aura 45425    -- Potion of Quickness (+Haste for 30s)
.aura 45426    -- Lucidity Potion (Immune to Sleep, Polymorph, Charm)
.aura 52792    -- Blackroot Brew (+Melee Attack Power & Size)
```

#### 3. Class & Raid Support Auras (Inject Full Raid Buffs)

```lua
.aura 25782    -- Greater Blessing of Might (+185 Attack Power)
.aura 25898    -- Greater Blessing of Kings (+10% All Stats)
.aura 21849    -- Gift of the Wild (+12 All Stats, +16 All Resists, +285 Armor)
.aura 21564    -- Prayer of Fortitude (+54 Stamina)
.aura 25587    -- Windfury Weapon (Shaman Imbue: Chance for extra attacks with +315 AP!)
.aura 25359    -- Grace of Air Totem (+77 Agility)
.aura 25361    -- Strength of Earth Totem (+77 Strength)
.aura 25289    -- Battle Shout (+232 Attack Power)
.aura 23028    -- Arcane Brilliance (+31 Intellect)
.aura 27683    -- Prayer of Shadow Protection (+60 Shadow Resist)
```

#### 4. 1-Click "God Mode" Warrior DPS Mega-Buff Macro

Copy this macro to instantly turn your Warrior into a raid boss:

```lua
.buff
.aura 22888
.aura 16609
.aura 15366
.aura 24425
.aura 22817
.aura 22818
.aura 23768
.aura 17628
.aura 17538
.aura 11405
.aura 17038
.aura 25661
.aura 16323
.aura 16329
.aura 10667
.aura 10669
.aura 25782
.aura 25898
.aura 21849
.aura 21564
.aura 25587
.aura 25359
.aura 25361
```

### E. Complete Instance Teleport & Location Directory

Scanned from `tw_world.game_tele` (Classic & Turtle-WoW Custom Content):

| Destination Token | Zone / Instance Name | Type / Size | Map ID | Exact Coordinates (X, Y, Z) | Access / Key Required |
| :--- | :--- | :---: | :---: | :---: | :--- |
| `.tele group ubrs` | **Upper Blackrock Spire** | 10-Man Raid | `229` | `-7527, -1227, 285` | `12344` (Seal of Ascension) |
| `.tele group lbrs` | **Lower Blackrock Spire** | 5-Man Dungeon | `229` | `-7527, -1227, 285` | Open |
| `.tele group strat` | **Stratholme** | 5-Man Dungeon | `329` | `3392, -3379, 142` | `12382` (Key to the City) |
| `.tele group scholo` | **Scholomance** | 5-Man Dungeon | `289` | `1264, -2556, 94` | `13704` (Skeleton Key) |
| `.tele group brd` | **Blackrock Depths** | 5-Man Dungeon | `230` | `1082, -474, -107` | `11000` (Shadowforge Key) |
| `.tele group dmn` | **Dire Maul North** | 5-Man Dungeon | `429` | `254, -204, -2` | `18249` (Crescent Key) |
| `.tele group dmw` | **Dire Maul West** | 5-Man Dungeon | `429` | `34, 160, -3` | `18249` (Crescent Key) |
| `.tele group dme` | **Dire Maul East** | 5-Man Dungeon | `429` | `58, -315, -2` | Open (Pusillin inside) |
| `.tele group ony` | **Onyxia's Lair** | 40-Man Raid | `249` | `29.1, -71.3, -8.1` | `16309` (Drakefire Amulet) |
| `.tele group mc` | **Molten Core** | 40-Man Raid | `409` | `1082, -474, -107` | Quest `7848` + `22754` (Quintessence) |
| `.tele group bwl` | **Blackwing Lair** | 40-Man Raid | `469` | `-7664, -1100, 399` | Quest `7761` (Blackhand's Command) |
| `.tele group zg` | **Zul'Gurub** | 20-Man Raid | `309` | `-11916, -1243, 92` | Open |
| `.tele group aq20` | **Ruins of Ahn'Qiraj** | 20-Man Raid | `509` | `-8443, 1518, 31` | Open |
| `.tele group aq40` | **Temple of Ahn'Qiraj** | 40-Man Raid | `531` | `-8245, 1983, 129` | Scepter of the Shifting Sands |
| `.tele group Naxxramas` | **Naxxramas** | 40-Man Raid | `0` | `3120, -3724, 137` | Quest `9123` (Argent Dawn Exalted) |
| `.tele group smcath` | **Scarlet Monastery** | 5-Man Dungeon | `189` | `855, 1321, 18.6` | `7146` (The Scarlet Key) |
| `.tele group zf` | **Zul'Farrak** | 5-Man Dungeon | `209` | `1213, 841, 8.9` | `9240` (Mallet of Zul'Farrak) |
| `.tele group mara` | **Maraudon** | 5-Man Dungeon | `349` | `1010, -460, -43` | `17191` (Scepter of Celebras) |
| `.tele group hateforgeinstance` | **Hateforge Quarry** | **Turtle 5-Man** | `808` | `-8170, -3125, 199` | Open (Quest `40489`) |
| `.tele group alahthalasarrival` | **Alah'Thalas** | **Turtle 5-Man** | `0` | `4226, -2722, 121` | High Elf Ruins (Open) |
| `.tele group gilneascity` | **Gilneas City** | **Turtle 5-Man** | `815` | `-1095, 2234, 182` | Past Greymane Wall (Open) |
| `.tele group crescentgrove` | **Crescent Grove** | **Turtle 5-Man** | `802` | `560, 83.5, 276` | Ashenvale Barrow (Open) |
| `.tele group sanctumraid` | **Emerald Sanctum** | **Turtle 40-Man** | `807` | `2765, 2976, 26.6` | `50545` (Gemstone of Ysera) + Q `40962` |
| `.tele group emeraldsanctum` | **Emerald Sanctum (Hyjal)** | Zone Portal | `169` | `2762, 2972, 27.0` | Hyjal Dream Portal Entrance |
| `.tele group Karazhan` | **Tower of Karazhan** | **Turtle 10-Man** | `0` | `-11123, -2015, 47` | `61234` (Upper Karazhan Key) + Q `40829` |
| `.tele group karazhaninstance` | **Karazhan Instance** | **Turtle 40-Man** | `532` | `-11086, -1968, 49.8` | Medivh's Spire |
| `.tele group instancedkarazhancrypt` | **Karazhan Crypts** | **Turtle Dungeon** | `800` | `-11099, -1829, 71.8` | `51356` (Karazhan Crypt Key) |
| `.tele group ScarletCitadel` | **Scarlet Citadel** | **Turtle 10-Man** | `45` | `32.5, 13.2, 16.8` | `53005` (Scarlet Insignia) + `7146` |
| `.tele group TimbermawHold` | **Timbermaw Hold** | **Turtle 20-Man** | `1` | `6808, -2091, 624` | `42379` (Rune of Illumination) + Rep `576` |
| `.tele lapidis` | **Lapidis Isle** | Turtle Zone | `0` | `-12285, 3450, 10` | `60302` (Lapidis Tower Key) |
| `.tele gilijimcanyon` | **Gillijim's Isle** | Turtle Zone | `0` | `-13675, 2643, 53.5` | Open Exploration |

---

## 3. Deep Dive: Warrior DPS Talent Builds

### Spec A: Dual-Wield Fury DPS (17/34/0) — #1 Sustained Physical DPS

The most devastating physical DPS build in Turtle-WoW 1.18.1. Combines **Impale** (+20% critical damage multiplier) and **Deep Wounds** from Arms with **Death Wish**, **Flurry**, and **Bloodthirst** in Fury.

- **Interactive Calculator**: [Turtle WoW Warrior Calculator](https://talents.turtle-wow.org/warrior)

```text
Arms (17 points):
- 3/3 Improved Rend
- 2/2 Deep Wounds (Crits cause weapon damage bleed over 12s)
- 2/2 Impale (+20% Critical Strike Damage bonus — mandatory!)
- 2/2 Improved Overpower (+50% Crit chance on Overpower against dodging targets)
- 5/5 Tactical Mastery (Retain up to 25 Rage when shifting stances — allows stance dancing)
- 1/1 Anger Management (Generates 1 rage every 3s in combat; halts out-of-combat rage decay)

Fury (34 points):
- 5/5 Cruelty (+5% Melee Critical Strike)
- 5/5 Unbridled Wrath (Chance on swing to generate 1 extra rage)
- 5/5 Dual Wield Specialization (+25% Off-Hand weapon damage)
- 5/5 Enrage (+25% Melee damage bonus for 12s whenever you receive a critical hit)
- 1/1 Death Wish (+20% Physical damage, Fear immunity for 30s)
- 5/5 Flurry (+30% Attack Speed for next 3 swings after a critical strike)
- 2/2 Improved Berserker Rage (Generates 10 immediate rage, breaks fear/incapacitate)
- 1/1 Bloodthirst (Instant strike for 45% AP damage + heals for 30 HP per hit)
```

---

### Spec B: Two-Handed Arms / Cleave (31/20/0) — Massive Dungeon Trash AOE

If you want to wield iconic 2-Handers (Arcanite Reaper, Ashkandi, Might of Menethil) and obliterate 5-man trash packs:

```text
Arms (31 points):
- 5/5 Deflection (+5% Parry)
- 5/5 Tactical Mastery (Retain 25 rage)
- 2/2 Improved Overpower (+50% Crit)
- 3/3 Deep Wounds
- 2/2 Impale (+20% Crit Damage)
- 5/5 Two-Handed Weapon Specialization (+5% 2H damage)
- 1/1 Sweeping Strikes (Next 5 melee swings strike an additional nearby enemy!)
- 5/5 Axe Specialization (+5% Crit) OR Sword Specialization (5% extra swing)
- 1/1 Mortal Strike (Instant strike for weapon damage + 160 + 50% healing reduction)

Fury (20 points):
- 5/5 Cruelty (+5% Crit)
- 5/5 Unbridled Wrath
- 5/5 Enrage (+25% Damage)
- 5/5 Flurry (+30% Attack Speed)
```

---

## 4. High-Level Warrior DPS Mechanics & Rotations

### 1. The Heroic Strike / Cleave "Off-Hand Miss Bug"

In Vanilla and Turtle-WoW, dual-wielding has an inherent 24% miss penalty on auto-attacks. However:

> **The Secret Mechanic**: When **Heroic Strike** (or **Cleave**) is actively queued on your Main Hand swing, the game engine calculates your **Off-Hand** swing using the **Yellow (Special Attack) Hit Table** (which requires only 6% or 9% Hit Cap instead of 24%).

- **Rule**: Whenever your Rage is > 40–50, always queue **Heroic Strike** (single target) or **Cleave** (2+ targets). Your off-hand will almost never miss, dramatically increasing both off-hand damage and rage generation!

### 2. Single-Target Boss Rotation (Berserker Stance)

1. **Pre-Cast**: Pop **Bloodrage** before the pull. Wait 3 seconds for the Bot Main Tank to establish initial aggro.
2. **Priority 1: Bloodthirst** on cooldown (every 6s) — your highest damage per rage point.
3. **Priority 2: Whirlwind** on cooldown (every 10s).
4. **Priority 3: Overpower Stance Dancing**: When an enemy dodges your attack:
   - Shift to **Battle Stance** (Tactical Mastery keeps up to 25 rage).
   - Cast **Overpower** (+50% crit bonus; cannot be blocked, dodged, or parried).
   - Shift back to **Berserker Stance** immediately.
5. **Priority 4: Heroic Strike Queuing**: Queue when Rage > 45.
6. **Priority 5: Hamstring Rage Dump**: If BT and WW are on cooldown and you have excess rage, cast Hamstring to fish for Windfury, Flurry, or weapon procs.
7. **Execute Phase (Boss HP < 20%)**:
   - Stop casting Bloodthirst and Whirlwind.
   - Cast **Execute** on every global cooldown. If Flurry drops, weave in an auto-attack.

### 3. Cooldown Stacking & Snapshotting

At the start of boss fights (or during bloodlust / execute):

- Pop **Death Wish** (+20% damage) + **Diamond Flask** (`20130`, +75 Strength) + **Mighty Rage Potion** (`13442`, instant 45–75 Rage & +60 Strength).

### 4. Threat Management with Bot Tanks

- As Fury DPS, you can easily pull aggro from an AI bot tank.
- **Rule of Thumb**: Let the tank bot hit the mob with 1 Shield Slam and 1 Sunder before attacking.
- Keep **Limited Immunity Potions (LIP)** (`3387`) on your action bar: if you pull aggro on a boss, drinking a LIP makes you immune to physical damage for 6 seconds and temporarily clears your threat, forcing the boss back onto the tank bot!

---

## 5. The Definitive Turtle-WoW 1.12 Warrior Macro Suite (Native Engine & Group/Raid Ready)

> **CMaNGOS / Vanilla 1.12 Macro Rules**:
> In the native Vanilla 1.12 engine, modern retail conditional syntax (like `#showtooltip`, `/startattack`, `/stopcasting`, `/cast [stance:1]`) **does not exist natively**!
> Standard 1.12 macros use `/cast SpellName` and pure Vanilla Lua API via `/script` or `/run` (`CastSpellByName`, `AttackTarget()`, `SpellStopCasting()`, `GetShapeshiftFormInfo()`, `UseInventoryItem()`).
> Every macro below is written in **100% authentic Vanilla 1.12 syntax** guaranteed to execute seamlessly in the Turtle-WoW client.

### A. The Heroic Strike Queuing & "HS Cancel" Suite (Exploiting the Yellow Off-Hand Bug)

#### Macro 1: Heroic Strike Queue + Start Attack

In 1.12, if you lack rage, `/cast Heroic Strike` alone will not start auto-attacking. Pairing it with `/script AttackTarget()` guarantees you start swinging immediately:

```lua
/cast Heroic Strike
/script AttackTarget()
```

#### Macro 2: The Legendary Heroic Strike Cancel Macro (The Infinite Rage & Yellow Off-Hand Hack)

When you are rage-starved (<25 rage) or saving rage for Bloodthirst:

1. Press Macro 1 to queue Heroic Strike.
2. Watch your swing timer addon (**`SP_SwingTimer`**, pre-installed in your client).
3. The moment your **Off-Hand** lands (reaping the benefits of the 0% dual-wield miss penalty on the yellow hit table), tap this cancel macro **right before the Main-Hand connects**.
4. The MH reverts to a white auto-attack, connects with the boss, and generates 25–35 rage without spending a single point on Heroic Strike!

```lua
/script SpellStopCasting()
```

#### Macro 3: Cleave Queue + Start Attack (Dungeon & Raid Trash)

```lua
/cast Cleave
/script AttackTarget()
```

*(Use Macro 2 `/script SpellStopCasting()` to cancel Cleave if you need to generate rage instead).*

---

### B. 1.12 Stance Dancing Combat Macros

In the 1.12 engine, stances are indexed via `GetShapeshiftFormInfo(index)`:

- `1` = **Battle Stance**
- `2` = **Defensive Stance**
- `3` = **Berserker Stance**

#### Macro 4: 1-Click Overpower Stance Dance

When an enemy dodges, tap this macro once to shift into Battle Stance (Tactical Mastery retains 25 rage). Tap it a second time to cast Overpower:

```lua
/script local _,_,a=GetShapeshiftFormInfo(1); if not a then CastSpellByName("Battle Stance"); else CastSpellByName("Overpower"); end
/script AttackTarget()
```

#### Macro 5: Whirlwind & Return to Berserker Stance

Tap to ensure you are in Berserker Stance and immediately fire Whirlwind:

```lua
/script local _,_,a=GetShapeshiftFormInfo(3); if not a then CastSpellByName("Berserker Stance"); else CastSpellByName("Whirlwind"); end
/script AttackTarget()
```

#### Macro 6: Pummel (Instant Caster Interrupt from Any Stance)

Shifts directly into Berserker Stance and casts Pummel to lock out enemy spellcasting:

```lua
/script local _,_,a=GetShapeshiftFormInfo(3); if not a then CastSpellByName("Berserker Stance"); else CastSpellByName("Pummel"); end
```

#### Macro 7: Berserker Rage (Break Fear & Incapacitate Instantly)

```lua
/script local _,_,a=GetShapeshiftFormInfo(3); if not a then CastSpellByName("Berserker Stance"); else CastSpellByName("Berserker Rage"); end
```

#### Macro 8: Smart Charge & Intercept (2-in-1 Mobility)

Out of combat $\rightarrow$ shifts to Battle Stance and casts Charge.
In combat $\rightarrow$ shifts to Berserker Stance and casts Intercept:

```lua
/script if not UnitAffectingCombat("player") then local _,_,a=GetShapeshiftFormInfo(1); if not a then CastSpellByName("Battle Stance") else CastSpellByName("Charge") end else local _,_,a=GetShapeshiftFormInfo(3); if not a then CastSpellByName("Berserker Stance") else CastSpellByName("Intercept") end end
/script AttackTarget()
```

---

### C. Core Rotation & Rage Dump Macros

#### Macro 9: Bloodthirst + Start Attack

Guarantees auto-attack starts even if Bloodthirst is on cooldown or you are low on rage:

```lua
/cast Bloodthirst
/script AttackTarget()
```

#### Macro 10: Execute + Start Attack

```lua
/cast Execute
/script AttackTarget()
```

#### Macro 11: Hamstring Proc Fisher & Rage Dump

Fires Hamstring to proc Windfury Totem, Flurry, or weapon procs when Bloodthirst and Whirlwind are on cooldown:

```lua
/cast Hamstring
/script AttackTarget()
```

#### Macro 12: Sweeping Strikes (2H Arms Cleave Trash Annihilator)

Shifts to Battle Stance and activates Sweeping Strikes:

```lua
/script local _,_,a=GetShapeshiftFormInfo(1); if not a then CastSpellByName("Battle Stance"); else CastSpellByName("Sweeping Strikes"); end
```

---

### D. Burst Cooldown Stacking & Emergency Survival

#### Macro 13: 1-Click Death Wish & Double Trinket Burst

Activates Trinket Slot 13 (e.g. Diamond Flask `20130`), Trinket Slot 14 (e.g. Kiss of the Spider `22954`), pops Bloodrage for 10 immediate rage, and triggers Death Wish (+20% physical damage):

```lua
/script UseInventoryItem(13)
/script UseInventoryItem(14)
/cast Bloodrage
/cast Death Wish
```

#### Macro 14: Emergency 1H + Shield & Shield Wall (Panic Button)

Shifts to Defensive Stance and equips your shield:

```lua
/script local _,_,a=GetShapeshiftFormInfo(2); if not a then CastSpellByName("Defensive Stance"); else CastSpellByName("Shield Wall"); end
/script EquipItemByName("Elementium Reinforced Bulwark")
```

*(If using the pre-installed `ItemRack` addon, use `/itemrack equip def`)*

#### Macro 15: Emergency Limited Immunity Potion (LIP Aggro Drop)

Scans bags 0 to 4 for a `Limited Immunity Potion` and drinks it instantly, giving 6 seconds of physical immunity and clearing threat from the boss:

```lua
/script for b=0,4 do for s=1,GetContainerNumSlots(b) do local l=GetContainerItemLink(b,s); if l and string.find(l,"Limited Immunity Potion") then UseContainerItem(b,s) break end end end
```

*(Tip: Alternatively, drag your Limited Immunity Potion to Action Bar Slot 36 and use `/script UseAction(36)`).*

#### Macro 16: Emergency Taunt (Defensive Stance Dance)

```lua
/script local _,_,a=GetShapeshiftFormInfo(2); if not a then CastSpellByName("Defensive Stance"); else CastSpellByName("Taunt"); end
```

#### Macro 17: Mocking Blow (Taunt Fallback)

If Taunt is resisted or on cooldown, shifts to Battle Stance and forces the mob to attack you for 6s:

```lua
/script local _,_,a=GetShapeshiftFormInfo(1); if not a then CastSpellByName("Battle Stance"); else CastSpellByName("Mocking Blow"); end
```

---

### E. Raid & Group Bot Coordination Macros

#### Macro 18: Focus Target Assist & Attack

Assists your targeted bot tank (`%t`) and begins attacking:

```lua
/assist %t
/script AttackTarget()
```

#### Macro 19: All Bots Focus Fire Target

Forces all 4 or 9 party/raid bots to switch target and open fire:

```lua
/ra @all target %t
/ra @all attack
```

---

## 6. Stat Priorities & Hit Caps

| Stat | Cap / Target | Description & Mechanics |
| :--- | :---: | :--- |
| **Hit Cap (with 305+ Skill)** | **6%** | If you have 305+ Weapon Skill (Humans with Swords/Maces, Orcs with Axes, or wearing **Edgemaster's Handguards** `14551`), you only need 6% Hit from gear for abilities and queued swings. |
| **Hit Cap (with 300 Skill)** | **9%** | If you do not have 305 weapon skill, you must equip 9% Hit from gear to never miss special abilities against level 63 raid bosses. |
| **Weapon Skill** | **305+** | Eliminates 30% of glancing blow damage penalty (increases glancing blow damage from 65% to 95% of normal hit). The single biggest DPS stat in the game. |
| **Melee Critical Strike** | **30%+** | Keeps **Flurry** (+30% attack speed) active 70–85% of the fight. |
| **Attack Power / Strength** | As much as possible | 1 Strength = 2 Attack Power. Increases baseline weapon damage and Bloodthirst scaling. |

---

## 7. Gear Loadouts & 1-Click Spawning

### ⚔️ Loadout 1: Dual-Wield Fury DPS Best-in-Slot (Raid Ready)

Naxxramas Kel'Thuzad swords, Lionheart Helm, Drake Fang Talisman, Kiss of the Spider, Band of Accuria:

```lua
-- Armor & Accessories
.additem 12640 1   -- Lionheart Helm (+2% Hit, +2% Crit)
.additem 19377 1   -- Prestor's Talisman of Conscience (+33 Agility)
.additem 19394 1   -- Drake Talon Pauldrons
.additem 21701 1   -- Cloak of the Fallen God
.additem 21814 1   -- Breastplate of Annihilation
.additem 21618 1   -- Hive Defiler Wristguards
.additem 14551 1   -- Edgemaster's Handguards (+7 Swords, Axes, Daggers)
.additem 19137 1   -- Onslaught Girdle (+1% Hit, +1% Crit, 31 Str)
.additem 22385 1   -- Titanic Leggings (+2% Hit, +1% Crit, 30 Str)
.additem 19387 1   -- Chromatic Boots
.additem 17063 1   -- Band of Accuria (+2% Hit)
.additem 21709 1   -- Ring of the Fallen God (+1% Hit, +60 AP)

-- Weapons & Trinkets
.additem 23054 1   -- Gressil, Dawn of Ruin (MH 2.70 Speed Sword)
.additem 23577 1   -- The Hungering Cold (OH 1.50 Speed Sword, +14 Swords Skill!)
.additem 22811 1   -- Soulstring (Ranged Bow)
.additem 20130 1   -- Diamond Flask (Level 50 Warrior Quest Trinket)
.additem 22954 1   -- Kiss of the Spider (Naxx Trinket: +200 Haste on-use)
.additem 19406 1   -- Drake Fang Talisman (+2% Hit, +56 AP)
.repairitems
```

### 🔨 Loadout 2: 2-Handed Arms Cleave BiS

Might of Menethil, Dreadnaught plate, Onslaught Girdle:

```lua
.additemset 523
.additem 22798 1   -- Might of Menethil (3.80 Speed 2H Mace)
.additem 19137 1   -- Onslaught Girdle
.additem 19377 1   -- Prestor's Talisman of Conscience
.additem 22954 1   -- Kiss of the Spider
.additem 19406 1   -- Drake Fang Talisman
.additem 17063 1   -- Band of Accuria
.additem 21709 1   -- Ring of the Fallen God
.additem 22811 1   -- Soulstring
.repairitems
```

### 🗡️ Loadout 3: Classic Pre-Raid Dungeon BiS (Dal'Rend's Swords)

```lua
.additem 12640 1   -- Lionheart Helm
.additem 15411 1   -- Mark of Fordring (Neck)
.additem 13340 1   -- Cape of the Black Baron (Stratholme Baron)
.additem 11726 1   -- Savage Gladiator Chain (BRD Arena Chest: +2% Crit)
.additem 12936 1   -- Battleborn Armbraces
.additem 12940 1   -- Dal'Rend's Sacred Charge (MH Sword from Rend in UBRS)
.additem 12939 1   -- Dal'Rend's Tribal Guardian (OH Sword from Rend in UBRS)
.additem 12651 1   -- Blackcrow (Crossbow from LBRS)
.additem 11815 1   -- Hand of Justice (BRD Emperor Trinket: 2% Extra Swing)
.additem 13965 1   -- Blackhand's Breadth (UBRS Quest Trinket: +2% Crit)
.additem 20130 1   -- Diamond Flask (Level 50 Class Quest Trinket)
.repairitems
```

---

## 8. Permanent Enchants & Scanned Database Formulas

### A. Automatic Full-Gear Enchanting

```lua
.bot enchants *
```

*Scans all equipped gear slots on your character and applies the highest-tier level-appropriate permanent enchantment (Crusader on weapons, +4 All Stats on chest, +15 Agi on gloves, +9 Stamina on bracers, Minor Speed on boots).*

### B. Custom Turtle-WoW Enchanting Formulas

Scanned directly from `tw_world.item_template`:

| Recipe Item ID | Formula Name | Resulting Enchant Spell | Slot | Enchant Bonus |
| :---: | :--- | :---: | :---: | :--- |
| **`53017`** | Formula: Enchant Gloves - Major Strength | `46087` | Gloves | **+9 Strength** |
| **`42187`** | Formula: Enchant Cloak - Agility | `49017` | Cloak | **+7 Agility** |
| **`42188`** | Formula: Enchant Cloak - Greater Shadow Resistance | `49019` | Cloak | **+15 Shadow Resistance** |
| **`42189`** | Formula: Enchant 2H Weapon - Nature Damage | `49021` | 2H Weapon | **+40 Nature Damage** |
| **`42190`** | Formula: Enchant 2H Weapon - Shadow Damage | `49023` | 2H Weapon | **+40 Shadow Damage** |
| **`55044`** | Formula: Enchant Boots - Major Intellect | `56555` | Boots | **+9 Intellect** |

```lua
-- Spawn All Custom Turtle Enchanting Formulas
.additem 53017 1   -- Formula: Enchant Gloves - Major Strength (+9 Str)
.additem 42187 1   -- Formula: Enchant Cloak - Agility (+7 Agi)
.additem 42188 1   -- Formula: Enchant Cloak - Greater Shadow Resistance (+15 Shadow Res)
.additem 42189 1   -- Formula: Enchant 2H Weapon - Nature Damage (+40 Nature Dmg)
.additem 42190 1   -- Formula: Enchant 2H Weapon - Shadow Damage (+40 Shadow Dmg)
```

---

## 9. Detailed Key & Attunement Walkthroughs (With Instant GM Commands)

### 1. Upper Blackrock Spire (UBRS): Seal of Ascension

- **Item ID**: `12344` (**Seal of Ascension**)
- **Purpose**: Unlocks the runic door leading to the 10-man Upper Blackrock Spire instance.
- **Quest Chain**:
  1. Farm `Unadorned Seal of Ascension` (`12219`) in LBRS.
  2. Collect 3 Boss Gemstones in LBRS: Highlord Omokk (`12224`), War Master Voone (`12225`), Overlord Wyrmthalak (`12226`).
  3. Speak to **Vaelan** in LBRS alcove.
  4. Mind-control **Emberstrife** in Dustwallow Marsh via `Orb of Draconic Energy` (`12345`) to breathe dragonflame on the ring.
  5. Return to Vaelan.
- **Instant GM Bypass Command**:

  ```lua
  .additem 12344 1
  ```

---

### 2. Onyxia's Lair Attunement: Drakefire Amulet

- **Item ID**: `16309` (**Drakefire Amulet**)
- **Purpose**: Required to be in the inventory of **every single player** entering Onyxia's Lair.
- **Alliance Chain Summary**: Dragonkin Menace $\rightarrow$ Marshal Windsor rescue in BRD (*Jail Break!*) $\rightarrow$ Stormwind Great Masquerade unmasking Lady Katrana Prestor $\rightarrow$ Defeat General Drakkisath in UBRS for `General Drakkisath's Blood` (`16308`) $\rightarrow$ Haleh in Winterspring.
- **Horde Chain Summary**: Warlord's Command in LBRS $\rightarrow$ Rend Blackhand head in UBRS $\rightarrow$ Rexxar $\rightarrow$ Myranda the Hag disguise $\rightarrow$ Emberstrife 4-Dragon Test of Skulls $\rightarrow$ General Drakkisath's Blood in UBRS $\rightarrow$ Rexxar.
- **Instant GM Bypass Command**:

  ```lua
  .quest complete 6502       -- Alliance (The Dragon's Eye)
  .quest complete 6602       -- Horde (Blood of the Black Dragon Champion)
  .additem 16309 1           -- Drakefire Amulet (Equip or keep in bags)
  ```

---

### 3. Molten Core (MC): Attunement to the Core & Rune Dousing

- **Attunement Quest**: *Attunement to the Core* (`7848`) from **Lothos Riftwaker** at the base of Blackrock Mountain.
- **Objective**: Enter BRD, retrieve the **Core Fragment** (`18987`) from the bridge outside MC portal, and return to Lothos.
- **Boss Rune Dousing**:
  - To summon **Majordomo Executus** and **Ragnaros**, all 7 Runes inside Molten Core must be doused with water from Hydraxian Waterlords.
  - **Eternal Quintessence** (`22754`): Infinite, non-consumed version (Revered reputation).
- **Instant GM Bypass Command**:

  ```lua
  .quest complete 7848       -- Instant MC Portal Zoning
  .modify rep 749 42999      -- Hydraxian Waterlords Exalted
  .additem 22754 1           -- Eternal Quintessence (Infinite Rune Douses)
  ```

---

### 4. Blackwing Lair (BWL): Blackhand's Command

- **Attunement Quest**: *Blackhand's Command* (`7761`).
- **Objective**: Kill Scarshield Quartermaster outside UBRS, enter UBRS, kill General Drakkisath, and touch the Orb of Command.
- **Instant GM Bypass Command**:

  ```lua
  .quest complete 7761       -- Instant BWL Orb Teleport
  ```

---

### 5. Naxxramas: The Dread Citadel

- **Attunement Quest**: *The Dread Citadel - Naxxramas* (`9123`) from **Archmage Angela Dosantos** at Light's Hope Chapel.
- **Requirements**: At **Exalted** reputation with Argent Dawn, attunement is 100% free (no materials or gold).
- **Instant GM Bypass Command**:

  ```lua
  .modify rep 529 42999      -- Argent Dawn Exalted
  .quest complete 9123       -- Instant Naxxramas Attunement
  ```

---

### 6. Scholomance: Skeleton Key

- **Item ID**: `13704` (**Skeleton Key**)
- **Purpose**: Unlocks the iron gate outside Scholomance (Caer Darrow) and the Viewing Room door inside.
- **Instant GM Bypass Command**:

  ```lua
  .additem 13704 1
  ```

---

### 7. Stratholme: Key to the City

- **Item ID**: `12382` (**Key to the City**)
- **Purpose**: Unlocks the Eastwall Gate and Service Entrance to Stratholme (Undead / Baron Rivendare side).
- **Instant GM Bypass Command**:

  ```lua
  .additem 12382 1
  ```

---

### 8. Blackrock Depths (BRD): Shadowforge, Prison & Coffer Keys

- **Shadowforge Key** (`11000`): Unlocks all iron doors, gates, and the Shadowforge Lock.
- **Prison Cell Key** (`11140`): Drops from High Interrogator Gerstahn; opens jail cells.
- **Relic Coffer Keys** (`11144`): Collect 12 keys to open all 12 coffer doors in the Vault and the Secret Safe.
- **Instant GM Bypass Command**:

  ```lua
  .additem 11000 1
  .additem 11140 1
  .additem 11144 12
  ```

---

### 9. Dire Maul: Crescent Key

- **Item ID**: `18249` (**Crescent Key**)
- **Purpose**: Unlocks the entrance doors to **Dire Maul North**, **Dire Maul West**, and the **Shen'dralar Ancient Library**.
- **Instant GM Bypass Command**:

  ```lua
  .additem 18249 1
  ```

---

### 10. Zul'Farrak & Maraudon Keys

- **Mallet of Zul'Farrak** (`9240`): Strikes the gong to summon the hydra **Gahz'rilla**.
- **Scepter of Celebras** (`17191`): Creates an instant portal into the Earth Song Falls (Princess Theradras inner wing).
- **Instant GM Bypass Command**:

  ```lua
  .additem 9240 1
  .additem 17191 1
  ```

---

### 11. Scarlet Monastery & Gnomeregan Keys

- **The Scarlet Key** (`7146`): Unlocks the Armory and Cathedral wings of Scarlet Monastery.
- **Workshop Key** (`9287`): Unlocks the backdoor entrance to Gnomeregan.
- **Key to Searing Gorge** (`5396`): Unlocks the locked gate between Loch Modan and Searing Gorge.
- **Instant GM Bypass Command**:

  ```lua
  .additem 7146 1
  .additem 9287 1
  .additem 5396 1
  ```

---

### 12. Emerald Sanctum (Custom 40-Man Raid Attunement)

- **Location**: Mount Hyjal (Dream Portal at `.tele group sanctumraid` or `.tele group emeraldsanctum`).
- **Attunement Questline**: *"Into the Dream"* (Quests `40957`, `40958`, `40959`, `40960`, `40961`, `40962`).
- **Required Item**: `Gemstone of Ysera` (`50545`).
- **Instant GM Bypass Command**:

  ```lua
  .quest complete 40962      -- Complete Into the Dream VI
  .additem 50545 1           -- Gemstone of Ysera
  .tele group sanctumraid    -- Teleport directly into Emerald Sanctum
  ```

---

### 13. Tower of Karazhan & Karazhan Crypts (Custom 10-Man / 40-Man Content)

- **Location**: Deadwind Pass (Spire at `.tele group Karazhan` / Crypts at `.tele group instancedkarazhancrypt`).
- **Attunement Questline**: *"The Key to Karazhan"* (Quests `40817` through `40829`).
- **Required Keys**:
  - `Upper Karazhan Tower Key` (`61234`): Unlocks upper spire doors.
  - `Karazhan Crypt Key` (`51356`): Unlocks underground crypts.
- **Instant GM Bypass Command**:

  ```lua
  .quest complete 40829      -- Complete The Key to Karazhan X
  .additem 61234 1           -- Upper Karazhan Tower Key
  .additem 51356 1           -- Karazhan Crypt Key
  .tele group Karazhan       -- Teleport to Tower of Karazhan
  ```

---

### 14. Timbermaw Hold (Custom 20-Man Raid)

- **Location**: Azshara / Felwood tunnel (`.tele group TimbermawHold`).
- **Required Item**: `Rune of Illumination` (`42379`) ("Allows the wearer to pass through the hidden tunnels to Timbermaw Hold").
- **Required Faction**: Timbermaw Hold (`576`) at Exalted.
- **Quest**: `The Corruption of Timbermaw Hold` (`41931`).
- **Instant GM Bypass Command**:

  ```lua
  .modify rep 576 42999      -- Timbermaw Hold Exalted
  .quest complete 41931      -- Complete Raid Quest
  .additem 42379 1           -- Rune of Illumination
  .tele group TimbermawHold  -- Teleport to Raid Gate
  ```

---

### 15. Scarlet Citadel (Custom 10-Man Raid)

- **Location**: Western Plaguelands / Tirisfal grounds (`.tele group ScarletCitadel`).
- **Required Access**: `Scarlet Crusade Insignia` (`53005`) and `The Scarlet Key` (`7146`).
- **Instant GM Bypass Command**:

  ```lua
  .additem 53005 1           -- Scarlet Crusade Insignia
  .additem 7146 1            -- The Scarlet Key
  .tele group ScarletCitadel -- Teleport directly into Scarlet Citadel
  ```

---

## 10. Turtle-WoW Custom Dungeons & Raids Directory

### Custom Turtle-WoW 5-Man Dungeons

| Destination Token | Instance Name | Level Range | Map ID | Description & Location |
| :--- | :--- | :---: | :---: | :--- |
| `.tele group hateforgeinstance` | **Hateforge Quarry** | 50–60 | `808` | Dark Iron Dwarf mining instance in Burning Steppes (`-8170, -3125, 199`). Rich in endgame plate, weapons, and fire resistance gear. |
| `.tele group alahthalasarrival` | **Alah'Thalas** | 55–60 | `0` | High Elf Arcane Sanctum in northern Eastern Kingdoms (`4226, -2722, 121`). Custom dungeon bosses and caster/melee loot. |
| `.tele group gilneascity` | **Gilneas City** | 42–48 | `815` | Gothic walled city ruins of Gilneas (`-1095, 2234, 182`). Worgen and human rebel bosses. |
| `.tele group crescentgrove` | **Crescent Grove** | 32–38 | `802` | Corrupted Night Elf barrow dens in Ashenvale (`560, 83.5, 276`). |

### Custom Turtle-WoW Raids (10-Man / 20-Man / 40-Man)

| Destination Token | Raid Name | Raid Size | Map ID | Description & Location |
| :--- | :--- | :---: | :---: | :--- |
| `.tele group ScarletCitadel` | **Scarlet Citadel** | **10-Man** | `45` | Elite Scarlet Crusade bastion in Tirisfal Glades (`32.5, 13.2, 16.8`). High-tier 10-man loot progression. |
| `.tele group Karazhan` | **Tower of Karazhan** | **10-Man** | `0` / `532` | Medivh's tower in Deadwind Pass (`-11123, -2015, 47`). Custom 10-man version preceding the raid. |
| `.tele group TimbermawHold` | **Timbermaw Hold** | **20-Man** | `1` | Sacred furbolg barrow cavern in Felwood (`6808, -2091, 624`). Corrupted elemental and shamanic encounters. |
| `.tele group sanctumraid` | **Emerald Sanctum** | **40-Man** | `807` | Dragon raid instance accessed via Hyjal portal (`2765, 2976, 26.6`). Green Dragonflight bosses (Sanctum of Dreams). |

---

## 11. Raid Party Coordination: 10-Mans, Subgroups & Chat (`/ra` vs `/p`)

When running **10-man instances** (like **UBRS**, **Scarlet Citadel**, or **Tower of Karazhan**):

1. **Converting to Raid**:
   - Invite your initial bots, then right-click your character portrait $\rightarrow$ **"Convert to Raid"**.
   - Alternatively, use `.bot raid` or `.rndbot raid`.

2. **Channel Communication (`/ra` vs `/p`)**:
   - **`/p`** (Party Chat) only broadcasts to your immediate 5-man subgroup (Subgroup 1)!
   - **`/ra`** (Raid Chat) or **`/r`** broadcasts to **all 10 (or 40) members** of the entire raid!

3. **Controlling Raid Bots via `/ra`**:

   ```lua
   /ra @all follow     -- All 10 bots immediately follow you
   /ra @all food       -- All bots eat/drink for full mana/HP
   /ra @all buff       -- All casters buff Fortitude, Intellect, Mark of the Wild
   /ra @all loot       -- All bots loot surrounding corpses
   /ra @all flee       -- Emergency disengage
   ```

4. **Designating the Bot Main Tank**:
   - Target the tank bot (e.g. Protection Warrior or Protection Paladin):

     ```lua
     /ra mt
     /w %t co +tank,prot,tank assist,avoid aoe,potions
     /w %t nc +food
     ```

   - All other DPS and Healer bots will now assist the Main Tank's target automatically.

---

## 12. Autonomous Dungeon & Raid Clear AI (`.dc`)

The Dungeon Clear AI module (`mod-dungeon-clear`) fully automates 5-man and 10-man dungeon clears (including UBRS):

```lua
-- 1. Activate Auto Clear on Tank Bot
.dc on
/ra @all co +dungeon clear,avoid aoe,roll,loot,potions
/ra @all nc +food
/ra @all formation near
/ra @all follow
```

### In-Dungeon Operational Controls

- **Order Tank Pull**: `.dc pull` (Orders tank bot to initiate a controlled pull on target).
- **Skip Trash Pack / Boss**: `.dc skip` (Instructs the pathing solver to bypass current mob).
- **Pause Tank**: `.dc pause`
- **Resume Tank**: `.dc on`
- **Progress Bitmask**: `.dc status` & `.dc bosses`
- **Emergency Wipe Disengage**:

  ```lua
  .dc pause
  /ra @all flee
  /ra @all follow
  .combatstop
  ```

- **Post-Encounter Loot & Consumable Recovery**:

  ```lua
  /ra @all loot
  /ra @all food
  /ra @all buff
  ```

---

## 13. Pre-Flight Dungeon & Raid Readiness Checklist

Before zoning into Stratholme, Scholomance, UBRS, Karazhan, Emerald Sanctum, or Onyxia, do this 30-second audit:

- [ ] **Character Boost**: Level 60, all spells learned (`.learn all_myspells`), defense/weapon skills at 300/300 (`.maxskill`).
- [ ] **Talents**: Dual-Wield Fury (17/34/0) or 2H Arms Cleave (31/20/0) allocated.
- [ ] **Hit & Weapon Skill**: 305+ Weapon Skill (Edgemaster's Handguards `14551` or Human/Orc racials) with **6% Hit** from gear (or 9% Hit if at 300 skill).
- [ ] **Enchants Applied**: Ran `.bot enchants *` (Crusader on weapons, +4 Stats on chest, +15 Agi on gloves) and applied Arcanums (`11645` +8 Str on Head/Legs, `23548` Might of the Scourge on Shoulders).
- [ ] **Physical Keys in Bags**:
  - Classic: `12344` (UBRS), `13704` (Scholo), `12382` (Strat), `11000` (BRD), `18249` (DM), `16309` (Onyxia), `22754` (MC Quintessence).
  - Turtle Custom: `50545` (Emerald Sanctum), `61234` (Karazhan), `51356` (Karazhan Crypts), `42379` (Timbermaw Hold), `53005` (Scarlet Citadel).
- [ ] **Attunements Completed**: Ran instant completion commands for MC (`7848`), BWL (`7761`), Onyxia (`6502`/`6602`), Naxxramas (`9123`), Emerald Sanctum (`40962`), and Karazhan (`40829`).
- [ ] **Consumables Stocked**: Flask of the Titans (`13510`), Mongoose (`13452`), Giants (`9206`), Firewater (`12820`), Mighty Rage (`13442`), Limited Immunity Potions (`3387`), Dumplings (`20452`), and Sharpening Stones (`18262`).
- [ ] **Aura Imbues Active**: Ran the 1-Click Mega-Buff script (World Buffs + Jujus + Blasted Lands + Shaman Windfury + Paladin Might/Kings).
- [ ] **1.12 Action Bar Macros Bound**:
  - Macro 1 & 2: Heroic Strike Queue (`/cast Heroic Strike` + `/script AttackTarget()`) & HS Cancel (`/script SpellStopCasting()`).
  - Macro 4 & 5: Overpower Stance Dance (`GetShapeshiftFormInfo(1)`) & Whirlwind.
  - Macro 15: Emergency LIP aggro drop.
- [ ] **Raid Party Formed**: Group converted to raid (`.rndbot raid` or `.bot raid`), all 10 bots summoned (`.bot summon *`).
- [ ] **Main Tank Configured**: Target bot tank $\rightarrow$ `/ra mt` and `/w %t co +tank,prot,tank assist`.
- [ ] **Dungeon Clear AI**: Engaged with `.dc on` and `/ra @all follow`.
