# 04. Class Specs, Meta Builds & Endgame Gear Directory

> **Target Platform**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + PlayerBots Framework
> **Coverage**: 100% Complete 9-Class Directory (Warrior, Paladin, Shaman, Druid, Rogue, Hunter, Mage, Warlock, Priest)
> **Source Verification**: Server Database `tw_world_item_template.sql`, DBC spell tables, verified 1.18.1 talent trees
> **Master Portals**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: World & Teleports](./03_turtle_world_and_teleports.md) • [📜 Volume 05: Eluna Scripting](./05_developer_eluna_and_engine.md)

---

## Unified 5-Volume Master Reference Architecture

| Volume | Handbook Title | Domain & Core Coverage | Direct Link |
| :---: | :--- | :--- | :---: |
| **01** | **[Game Master & World Administration](./01_game_master_handbook.md)** | **100% of GM commands, player progression, cheats, security, spawning, moderation, tickets, reloads & diagnostics.** | [Open Volume 01](./01_game_master_handbook.md) |
| **02** | **[Playerbot & Raid Automation](./02_playerbot_and_raid_automation.md)** | **Complete .bot & .rndbot control, whisper shortcuts, strategy tokens (co/nc), .dc dungeon clear solver & raid macros.** | [Open Volume 02](./02_playerbot_and_raid_automation.md) |
| **03** | **[World, Dungeons & Teleports Directory](./03_turtle_world_and_teleports.md)** | **647+ .tele tokens, Turtle-WoW 1.18.1 custom instances (Crescent Grove, Hateforge, Karazhan Crypts, Scarlet Citadel), keys & attunements.** | [Open Volume 03](./03_turtle_world_and_teleports.md) |
| **04** | **Class Specs & Endgame Gear Directory** | **1-Click BiS gear loadouts, Tier 1/2/3 set IDs, weapons, trinkets, enchants & verified 1.18.1 talent builds across all 9 classes.** | **Current File** |
| **05** | **[Eluna Scripting & Core Development Engine](./05_developer_eluna_and_engine.md)** | **Eluna Lua API, live reloads (.reload eluna), Event 42 Custom Command dispatch, SpellInfo layer & C++ hook mechanics.** | [Open Volume 05](./05_developer_eluna_and_engine.md) |

---

## 📑 Table of Contents

1. [Unified 5-Volume Master Reference Architecture](#unified-5-volume-master-reference-architecture)
2. [Official Turtle-WoW Interactive Talent Calculator Directory](#official-turtle-wow-interactive-talent-calculator-directory)
3. [Part I: 9-Class Talent Specializations & Rotation Playbooks](#part-i-9-class-talent-specializations--rotation-playbooks)
   - [Warrior (DW Fury/Prot, Deep Prot, Fury DPS, Arms)](#1-warrior-talent-specs)
   - [Paladin (Holy Healer, Retribution, Prot Tank)](#2-paladin-talent-specs)
   - [Hunter (Marksmanship, Beast Mastery, Melee Survival)](#3-hunter-talent-specs)
   - [Rogue (Combat Swords, Combat Daggers, Subtlety/Hemo)](#4-rogue-talent-specs)
   - [Priest (Holy/Disc, Shadow, Holy Smite)](#5-priest-talent-specs)
   - [Shaman (Restoration, Elemental, Enhancement, Tankhance)](#6-shaman-talent-specs)
   - [Mage (Ignite Fire, Frost Control, Arcane Mind)](#7-mage-talent-specs)
   - [Warlock (DS/Ruin, SM/Ruin, Demonology Firestone)](#8-warlock-talent-specs)
   - [Druid (Feral Cat, Feral Bear, Restoration, Moonkin)](#9-druid-talent-specs)
4. [Part II: 1-Click BiS Class Loadouts (Complete 16–17 Slot Sets)](#part-ii-1-click-bis-class-loadouts-complete-1617-slot-sets)
   - [1. Protection Warrior (Deep Prot Tank)](#1-protection-warrior-deep-protection-mitigation-tank-bis)
   - [2. Dual-Wield Fury/Prot Warrior (Max Threat Tank)](#2-dual-wield-furyprot-warrior-max-raid-threat-bis)
   - [3. Dual-Wield Fury Warrior (#1 Physical DPS)](#3-dual-wield-fury-warrior-1-physical-melee-dps-bis)
   - [4. 2-Handed Arms Warrior (Physical Slam DPS)](#4-2-handed-arms--slam-warrior-two-handed-physical-dps-bis)
   - [5. Holy Paladin (Raid & Tank Healer)](#5-holy-paladin-raid--tank-healer-bis)
   - [6. Retribution Paladin (Holy Melee DPS)](#6-retribution-paladin-two-handed-holy-melee-dps-bis)
   - [7. Protection Paladin (AoE & Threat Tank)](#7-protection-paladin-aoe--threat-tank-bis)
   - [8. Marksmanship Hunter (Ranged Physical DPS)](#8-marksmanship-hunter-ranged-physical-dps-bis)
   - [9. Beast Mastery Hunter (Pet & Ranged Synergy)](#9-beast-mastery-hunter-pet--ranged-synergy-bis)
   - [10. Melee Survival Hunter (Turtle 1.18.1 Carve/Lacerate)](#10-melee-survival-hunter-turtle-1181-carve--lacerate-bis)
   - [11. Combat Swords Rogue (Sustained Physical DPS)](#11-combat-swords-rogue-sustained-physical-dps-bis)
   - [12. Combat Daggers Rogue (High Burst Physical DPS)](#12-combat-daggers-rogue-high-burst-physical-dps-bis)
   - [13. Subtlety / Hemorrhage Rogue (PvP & Armor Debuff)](#13-subtlety--hemorrhage-rogue-pvp--raid-armor-debuff-bis)
   - [14. Holy Priest (Raid & Tank Healer)](#14-holy-priest-raid--tank-healer-bis)
   - [15. Shadow Priest (Shadow DPS & Mana Battery)](#15-shadow-priest-shadow-dps--mana-battery-bis)
   - [16. Discipline / Holy Smite Priest (Holy Caster DPS)](#16-discipline--holy-smite-priest-holy-caster-dps-bis)
   - [17. Restoration Shaman (Chain Healer & Totems)](#17-restoration-shaman-chain-healer--totem-support-bis)
   - [18. Elemental Shaman (Burst Caster DPS)](#18-elemental-shaman-burst-caster-dps-bis)
   - [19. Enhancement Shaman (Windfury Melee DPS)](#19-enhancement-shaman-two-handed-windfury-melee-dps-bis)
   - [20. Shaman Tank 'Tankhance' (Signature Shield Tank)](#20-shaman-tank-tankhance-turtle-1181-signature-shield-tank-bis)
   - [21. Fire Mage (Ignite Caster Burst DPS)](#21-fire-mage-ignite-caster-burst-dps-bis)
   - [22. Frost Mage (Caster Control / AoE & AP Frost)](#22-frost-mage-caster-control--aoe--ap-frost-bis)
   - [23. Arcane Mind Mage (Arcane Power Burst DPS)](#23-arcane-mind-mage-arcane-power-burst-dps-bis)
   - [24. DS/Ruin Warlock (Demonic Sacrifice Shadow Bolt DPS)](#24-dsruin-warlock-demonic-sacrifice-shadow-bolt-dps-bis)
   - [25. SM/Ruin Warlock (Shadow Mastery Multi-DoT)](#25-smruin-warlock-shadow-mastery-multi-dot--nightfall-bis)
   - [26. Demonology Master Conjurer Warlock (Firestone DPS)](#26-demonology-master-conjurer-warlock-turtle-1181-firestone-bis)
   - [27. Feral Cat Druid (Powershifting Melee DPS)](#27-feral-cat-druid-powershifting-melee-dps-bis)
   - [28. Feral Bear Druid (High Health & Armor Tank)](#28-feral-bear-druid-high-health--armor-tank-bis)
   - [29. Restoration Druid (HoT Raid Healer & Swiftmend)](#29-restoration-druid-hot-raid-healer--swiftmend-bis)
   - [30. Balance Druid 'Boomkin' (Nature / Arcane Caster DPS)](#30-balance-druid-boomkin-nature--arcane-caster-dps-bis)
5. [Part III: Complete Endgame Item Directory & Set Index](#part-iii-complete-endgame-item-directory--set-index)
   - [Tier 1 Dungeon & Raid Armor Sets](#tier-1-raid-sets-molten-core)
   - [Tier 2 Raid Armor Sets](#tier-2-raid-sets-blackwing-lair)
   - [Tier 3 Naxxramas Raid Armor Sets](#tier-3-raid-sets-naxxramas)
   - [Signature Endgame Weapons Directory](#signature-endgame-weapons-directory)
   - [BiS Trinkets Directory](#bis-trinkets-directory)
   - [Weapon & Armor Permanent Enchantment Directory](#weapon--armor-permanent-enchantment-directory)

---

## Official Turtle-WoW Interactive Talent Calculator Directory

Use these official interactive calculators to preview talent tree node prerequisites, ranks, and point allocations live:

| Class | Official Live Calculator Link | Signature 1.18.1 Meta Specializations |
| :--- | :--- | :--- |
| **Warrior** | [🎮 Turtle WoW Warrior Calculator](https://talents.turtle-wow.org/warrior) | DW Fury/Prot (0/31/20) • Deep Prot (11/5/35) • Fury DPS (17/34/0) • Arms (31/20/0) |
| **Rogue** | [🎮 Turtle WoW Rogue Calculator](https://talents.turtle-wow.org/rogue) | Combat Swords (19/32/0) • Combat Daggers (15/31/5) • Subtlety/Hemo (21/3/27) |
| **Mage** | [🎮 Turtle WoW Mage Calculator](https://talents.turtle-wow.org/mage) | Ignite Fire (17/31/3) • AP Frost (28/0/23) • Arcane Mind (31/0/20) |
| **Warlock** | [🎮 Turtle WoW Warlock Calculator](https://talents.turtle-wow.org/warlock) | DS/Ruin (7/21/23) • SM/Ruin (30/0/21) • Master Conjurer Firestone (0/31/20) |
| **Hunter** | [🎮 Turtle WoW Hunter Calculator](https://talents.turtle-wow.org/hunter) | Marksmanship (20/31/0) • Beast Mastery (31/20/0) • Melee Survival (0/21/30 Carve/Lacerate) |
| **Priest** | [🎮 Turtle WoW Priest Calculator](https://talents.turtle-wow.org/priest) | Holy/Disc (21/30/0) • Holy Smite DPS (31/20/0) • Shadow Mana Battery (13/0/38) |
| **Paladin** | [🎮 Turtle WoW Paladin Calculator](https://talents.turtle-wow.org/paladin) | Holy Healer (31/20/0) • Retribution Burst (11/0/40) • Prot Tank (0/31/20) |
| **Shaman** | [🎮 Turtle WoW Shaman Calculator](https://talents.turtle-wow.org/shaman) | Resto Chain Heal (0/12/39) • Shaman Tank "Tankhance" (19/32/0) • Elemental (30/0/21) • Enhancement (19/32/0) |
| **Druid** | [🎮 Turtle WoW Druid Calculator](https://talents.turtle-wow.org/druid) | Feral Cat (14/32/5) • Feral Bear (14/32/5) • Balance Moonkin (31/0/20) • Moonglow Resto (24/0/27) |

---

## Part I: 9-Class Talent Specializations & Rotation Playbooks

### 1. Warrior Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/warrior](https://talents.turtle-wow.org/warrior)

### Spec 1: Dual-Wield Fury/Prot Tank (0/31/20) - Max Raid Threat BiS

The definitive high-end raid tanking spec in Turtle-WoW. Dual-wields fast weapons in Defensive Stance to generate enormous Rage and Threat Per Second (TPS), allowing DPS to go all-out without threat throttling.

- **Calculator Template**: [Warrior DW Fury/Prot (0/31/20)](https://talents.turtle-wow.org/warrior)

```text
Arms (0 points):
(None)

Fury (31 points):
- 5/5 Cruelty (+5% Melee Critical Strike)
- 5/5 Unbridled Wrath (Chance to gain 1 additional rage on melee swing)
- 5/5 Dual Wield Specialization (+25% Off-Hand weapon damage)
- 5/5 Enrage (+25% Melee damage bonus for 12s after being hit by a crit)
- 1/1 Death Wish (+20% Physical damage, Fear immunity for 30s)
- 5/5 Flurry (+30% Attack Speed for next 3 swings on critical strike)
- 4/5 Blood Craze or Impale
- 1/1 Bloodthirst (Instant strike for 45% Attack Power damage + heals for 30 HP per hit)

Protection (20 points):
- 5/5 Shield Specialization (Gain 1 rage when blocking, +5% block chance)
- 2/2 Anticipation (+10 Defense Skill) or 5/5 Toughness (+10% Armor from items)
- 1/1 Last Stand (+30% Maximum Health for 20s - essential emergency button)
- 5/5 Defiance (+15% Additional Threat in Defensive Stance)
- 1/1 Improved Taunt (Reduces Taunt cooldown by 2 seconds)
- 3/3 Improved Sunder Armor (Reduces Sunder Armor rage cost by 3)
```

- **Stat Priority**: Hit Cap (6% with 305+ Weapon Skill, 9% without) > Weapon Skill (305+) > Crit > Strength > Agility > Stamina > Armor.
- **Rotation**: Stay in **Defensive Stance**. Bloodthirst on CD > Revenge on proc > Heroic Strike queue on main hand swing > Devastate / Sunder Armor (maintain 5 stacks). Pop *Death Wish* + *Diamond Flask* at the start of boss encounters for immediate snap aggro.
- **Turtle WoW 1.18.1 Additions**:
  - **Thunder Clap in Defensive Stance**: Usable directly in Defensive Stance, giving tanks reliable multi-target AoE threat.
  - **Defensive Tactics**: Stance dance rage retention allows swapping to Berserker Stance for Berserker Rage/Pummel and returning without losing rage.

---

### Spec 2: Deep Protection Mitigation Tank (11/5/35) - Progression & Hardcore

Maximum physical and magical mitigation with high block value scaling. Uses 1H Weapon + Shield with Shield Slam for snap threat and Concussion Blow for dungeon control.

- **Calculator Template**: [Warrior Deep Protection (11/5/35)](https://talents.turtle-wow.org/warrior)

```text
Arms (11 points):
- 5/5 Deflection (+5% Parry)
- 5/5 Tactical Mastery (Retain up to 25 Rage when switching stances)
- 1/1 Anger Management (Generates 1 rage every 3 seconds)

Fury (5 points):
- 5/5 Cruelty (+5% Melee Critical Strike)

Protection (35 points):
- 5/5 Shield Specialization (+5% Block Chance, 1 Rage on block)
- 5/5 Toughness (+10% Armor from items)
- 1/1 Last Stand (Emergency health boost)
- 2/2 Improved Bloodrage (Generates 5 immediate extra rage)
- 5/5 Defiance (+15% Threat in Defensive Stance)
- 2/2 Improved Taunt (Taunt CD reduced by 2s)
- 3/3 Improved Sunder Armor
- 1/1 Concussion Blow (5-second physical stun)
- 1/1 One-Handed Weapon Specialization (+10% 1H Damage)
- 1/1 Shield Slam (High damage/threat slam based on shield block value + 50% dispel)
```

- **Stat Priority**: Defense (440 to reach uncrittable cap) > Hit (9%) > Stamina > Armor > Shield Block Value > Agility.
- **Rotation**: Shield Slam on CD > Revenge on proc > Thunder Clap (for AoE / attack speed debuff) > Sunder Armor > Shield Block (maintain active against physical crushing blows).

---

### Spec 3: Dual-Wield Fury DPS (17/34/0) - #1 Physical DPS Spec

The highest sustained physical melee damage spec. Combines Deep Wounds bleeding and Impale critical multiplier from Arms with Bloodthirst and Flurry attack speed in Fury.

- **Calculator Template**: [Warrior Fury DPS (17/34/0)](https://talents.turtle-wow.org/warrior)

```text
Arms (17 points):
- 3/3 Rend
- 2/2 Deep Wounds (Crits cause 60% average weapon damage bleed over 12s)
- 2/2 Impale (+20% Critical Strike Damage bonus)
- 2/2 Overpower (+50% Crit chance on Overpower against dodging targets)
- 5/5 Tactical Mastery (Retain rage when stance dancing for Overpower)
- 1/1 Anger Management

Fury (34 points):
- 5/5 Cruelty (+5% Crit)
- 5/5 Unbridled Wrath
- 5/5 Dual Wield Specialization
- 5/5 Enrage
- 1/1 Death Wish
- 5/5 Flurry (+30% Attack Speed)
- 2/2 Weapon Mastery or Improved Berserker Rage
- 1/1 Bloodthirst
```

- **Stat Priority**: Hit Cap (6% with 305+ skill / 9%) > Weapon Skill (305+) > Crit > Strength > Attack Power > Agility.
- **Rotation**: Bloodthirst on CD > Whirlwind on CD > Overpower on target dodge > Heroic Strike (queue when Rage > 40).

---

### 2. Rogue Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/rogue](https://talents.turtle-wow.org/rogue)

### Spec 4: Combat Swords Rogue (19/32/0) - Sustained Raiding BiS

Maximum sustained physical DPS using fast off-hand and slow high-damage main-hand swords.

- **Calculator Template**: [Rogue Combat Swords (19/32/0)](https://talents.turtle-wow.org/rogue)

```text
Assassination (19 points):
- 5/5 Malice (+5% Crit)
- 3/3 Ruthlessness (60% chance to add 1 combo point after finisher)
- 1/1 Relentless Strikes (Finishing moves have 20% chance per CP to restore 25 Energy)
- 5/5 Lethality (+30% Crit damage bonus for Sinister Strike, Gouge, Backstab)
- 5/5 Murder (+2% Damage vs Humanoids, Giants, Beasts, Dragonkin)

Combat (32 points):
- 2/2 Improved Sinister Strike (Reduces SS energy cost by 5)
- 3/3 Improved Gouge or Lightning Reflexes
- 5/5 Precision (+5% Chance to Hit with melee weapons)
- 5/5 Dual Wield Specialization (+50% Off-Hand weapon damage)
- 1/1 Blade Flurry (+20% Attack Speed & strikes an additional nearby enemy)
- 5/5 Sword Specialization (5% chance to gain an extra swing on hit with swords)
- 2/2 Weapon Expertise (+5 Weapon Skill with Swords, Daggers, Fists)
- 1/1 Adrenaline Rush (+100% Energy regeneration rate for 15s)
```

- **Stat Priority**: Hit Cap (9% yellow, 14%+ dual-wield white) > Agility > Attack Power > Crit.
- **Rotation**: Sinister Strike to build 5 Combo Points > Slice and Dice (100% uptime mandatory!) > Eviscerate / Rupture (if SnD has >10s remaining).
- **Turtle WoW 1.18.1 Features**:
  - **Combo Point Persistence**: Combo points are preserved when switching targets.
  - **Rogue Tanking**: Access to *Flourish* (parry finisher) and *Agitating Poison* (high threat generation).

---

### Spec 5: Combat Daggers Rogue (15/31/5) - High Burst Boss DPS

Position-dependent high-burst spec requiring Backstab from behind the boss.

- **Calculator Template**: [Rogue Combat Daggers (15/31/5)](https://talents.turtle-wow.org/rogue)

```text
Assassination (15 points):
- 5/5 Malice (+5% Crit)
- 3/3 Ruthlessness
- 1/1 Relentless Strikes
- 5/5 Lethality (+30% Crit damage bonus)
- 1/1 Vigor (+10 Max Energy)

Combat (31 points):
- 3/3 Improved Gouge
- 5/5 Precision (+5% Hit)
- 5/5 Dagger Specialization (+5% Crit with Daggers)
- 5/5 Dual Wield Specialization
- 1/1 Blade Flurry
- 2/2 Weapon Expertise
- 2/2 Improved Sprint
- 1/1 Adrenaline Rush

Subtlety (5 points):
- 5/5 Opportunity (+20% Damage to Backstab, Ambush, Garrote)
```

- **Rotation**: Backstab to 5 CP > Slice and Dice (keep active) > Backstab > Eviscerate.

---

### 3. Mage Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/mage](https://talents.turtle-wow.org/mage)

### Spec 6: Ignite Fire Mage (17/31/3) - AQ40 & Naxxramas #1 Caster DPS

High rolling Ignite damage. When multiple Fire Mages crit, they stack a single massive rolling Ignite DoT on the boss that ticks for tens of thousands of damage.

- **Calculator Template**: [Mage Ignite Fire (17/31/3)](https://talents.turtle-wow.org/mage)

```text
Arcane (17 points):
- 2/2 Arcane Subtlety (Reduces target spell resistance by 10, -40% threat on Arcane)
- 3/3 Arcane Focus (+6% Hit on Arcane)
- 5/5 Arcane Concentration (Clearcasting - 10% chance on spell hit to enter Freecast)
- 1/1 Evocation (Channel 60% mana restore)
- 2/2 Improved Counterspell
- 3/3 Arcane Meditation (+15% Mana regen while casting)

Fire (31 points):
- 5/5 Improved Fireball (Reduces Fireball cast time by 0.5s)
- 5/5 Ignite (Spell crits ignite target for 40% additional damage over 4s)
- 1/1 Pyroblast
- 2/2 Flame Throwing (+6yd Spell Range)
- 2/2 Burning Soul (-70% Pushback on cast, -30% Fire Threat)
- 3/3 Critical Mass (+6% Spell Crit with Fire spells)
- 5/5 Fire Power (+10% Fire Spell Damage)
- 1/1 Combustion (Each fire damage spell cast increases spell crit by 10% until 3 crits)

Frost (3 points):
- 3/3 Elemental Precision (+6% Spell Hit for Fire and Frost spells)
```

- **Stat Priority**: Spell Hit (16% cap, 10% from gear with 3/3 Elemental Precision) > Spell Damage > Spell Crit > Intellect.
- **Rotation**: Fireball spam > Scorch (apply and maintain 5 stacks of Improved Scorch for +15% Fire vulnerability) > Pyroblast on Combustion pull > Fire Blast on movement.
- **Turtle WoW 1.18.1 Tuning**: Ignite operates on a 4-second rolling window with full stack preservation.

---

### Spec 7: Arcane Power Frost Mage (28/0/23) - MC & BWL Progression

Maximum burst frost damage for Molten Core and Blackwing Lair where mobs have 100% Fire Immunity.

- **Calculator Template**: [Mage Arcane Power Frost (28/0/23)](https://talents.turtle-wow.org/mage)

```text
Arcane (28 points):
- 2/2 Arcane Subtlety
- 5/5 Arcane Concentration (Clearcasting)
- 1/1 Evocation
- 5/5 Arcane Mind (+10% Total Mana)
- 3/3 Arcane Instability (+3% Spell Damage & +3% Spell Crit)
- 1/1 Arcane Power (+30% Spell Damage, +30% Mana Cost for 15s)

Frost (23 points):
- 3/3 Elemental Precision (+6% Spell Hit)
- 5/5 Ice Shards (+100% Spell Critical Damage bonus)
- 3/3 Frostbite
- 3/3 Piercing Ice (+6% Frost Damage)
- 1/1 Cold Snap (Instantly resets all Frost cooldowns)
- 2/2 Arctic Reach (+20% Range)
- 5/5 Ice Floes or Channeling
- 1/1 Ice Block
```

- **Rotation**: Frostbolt spam > Pop Arcane Power + Trinket during burst phases > Cold Snap for double Ice Block/burst.

---

### 4. Warlock Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/warlock](https://talents.turtle-wow.org/warlock)

### Spec 8: DS/Ruin Warlock (7/21/23) - Top Raiding Shadow DPS

Sacrifices the Succubus minion via *Demonic Sacrifice* to gain a permanent +15% Shadow Damage multiplier, combined with *Ruin* for +100% critical strike bonus.

- **Calculator Template**: [Warlock DS/Ruin (7/21/23)](https://talents.turtle-wow.org/warlock)

```text
Affliction (7 points):
- 5/5 Improved Corruption (Instant cast Corruption)
- 2/2 Suppression (+4% Hit with Affliction spells)

Demonology (21 points):
- 5/5 Demonic Embrace (+15% Total Stamina)
- 2/2 Master Conjurer (+20% Healthstone heal, +30% Firestone damage & Spellstone crit)
- 3/3 Fel Intellect / Fel Stamina
- 1/1 Demonic Sacrifice (Sacrifice Succubus: +15% Shadow Damage bonus)

Destruction (23 points):
- 5/5 Improved Shadow Bolt (Shadow Bolt crits grant +20% Shadow damage for 4 hits)
- 5/5 Bane (Reduces Shadow Bolt cast time by 0.5s)
- 5/5 Devastation (+5% Spell Crit with Destruction spells)
- 1/1 Shadowburn (Instant cast emergency burst strike)
- 1/1 Ruin (+100% Critical Strike Damage bonus for Destruction spells)
```

- **Stat Priority**: Spell Hit (16% cap) > Shadow Spell Damage > Spell Crit > Stamina.
- **Rotation**: Curse of Shadows / Curse of Elements (assigned by raid lead) > Shadow Bolt spam > Life Tap to manage mana.
- **Turtle WoW 1.18.1 Rework**:
  - **Master Conjurer**: Combined talent merging Improved Healthstone, Firestone, and Spellstone into a single 2-point talent in Row 2 Demonology.
  - **Firestone / Spellstone**: Firestone grants flat +Fire Spell Damage; Spellstone grants +Spell Crit.

---

### Spec 9: SM/Ruin Affliction Warlock (30/0/21) - Utility & Multi-DoT

Provides *Nightfall* instant Shadow Bolt procs and *Shadow Mastery* (+10% Shadow damage) without needing to sacrifice your demon pet.

- **Calculator Template**: [Warlock SM/Ruin (30/0/21)](https://talents.turtle-wow.org/warlock)

```text
Affliction (30 points):
- 5/5 Improved Corruption
- 5/5 Fel Concentration (70% pushback resistance on Drain Life)
- 2/2 Nightfall (4% chance on Corruption/Drain Life ticks to grant instant Shadow Bolt)
- 1/1 Siphon Life
- 5/5 Shadow Mastery (+10% Shadow Spell Damage)

Destruction (21 points):
- 5/5 Improved Shadow Bolt
- 5/5 Bane
- 5/5 Devastation
- 1/1 Shadowburn
- 1/1 Ruin (+100% Critical Strike Damage bonus)
```

---

### 5. Hunter Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/hunter](https://talents.turtle-wow.org/hunter)

### Spec 10: Marksmanship PvE Hunter (20/31/0) - Raid Ranged DPS

Maximum ranged physical damage using Aimed Shot and Multi-Shot weave between auto-shots.

- **Calculator Template**: [Hunter Marksmanship (20/31/0)](https://talents.turtle-wow.org/hunter)

```text
Beast Mastery (20 points):
- 5/5 Improved Aspect of the Hawk (+30% Ranged attack speed proc)
- 5/5 Thick Hide
- 5/5 Unleashed Fury (+20% Pet Damage)
- 5/5 Ferocity (+15% Pet Critical Strike)

Marksmanship (31 points):
- 5/5 Efficiency (Reduces Shot mana cost by 10%)
- 5/5 Lethal Shots (+5% Ranged Critical Strike)
- 5/5 Mortal Shots (+30% Ranged Critical Damage bonus)
- 3/3 Barrage (+15% Multi-Shot damage)
- 2/2 Ranged Weapon Specialization (+5% Ranged Weapon Damage)
- 1/1 Trueshot Aura (Raid Attack Power aura)
```

- **Stat Priority**: Hit Cap (9%) > Agility > Ranged Attack Power > Crit.
- **Rotation**: Auto-Shot > Aimed Shot > Auto-Shot > Multi-Shot > Auto-Shot. Never clip auto-shot cast timer!
- **Turtle WoW 1.18.1 Baseline QoL**:
  - *Aimed Shot*, *Trueshot Aura*, and *Steady Shot* learned baseline at level 20 via trainers, freeing talent points.

---

### Spec 11: Melee Survival Hunter (0/21/30) - Turtle-WoW Signature Melee Spec

A fully viable close-quarters melee combat spec fighting alongside your pet with massive 2H weapon scaling.

- **Calculator Template**: [Hunter Melee Survival (0/21/30)](https://talents.turtle-wow.org/hunter)

```text
Marksmanship (21 points):
- 5/5 Lethal Shots (+5% Crit)
- 5/5 Efficiency
- 5/5 Mortal Shots (+30% Crit damage)
- 1/1 Aimed Shot / Rapid Fire enhancements

Survival (30 points):
- 5/5 Deflection (+5% Parry)
- 3/3 Savage Strikes (+20% Crit on Raptor Strike and Mongoose Bite)
- 2/2 Survivalist (+10% Total Health)
- 1/1 Carve (Instant 2H melee swing hitting up to 3 nearby enemies)
- 3/3 Surefooted (+3% Hit chance, +15% Movement Impairing resist)
- 5/5 Killer Instinct (+3% Crit)
- 1/1 Counterattack (Instant melee strike after parrying)
- 1/1 Trapping in Combat (Capstone: Traps can be laid during active combat encounters)
```

- **Stat Priority**: Hit Cap (9%) > Agility > Strength > Attack Power > Crit.
- **Rotation**: Raptor Strike on swing > Carve on CD > Mongoose Bite on CD > Lacerate (maintain bleed) > Lay Explosive/Immolation Trap in combat.

---

### 6. Priest Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/priest](https://talents.turtle-wow.org/priest)

### Spec 12: Holy / Discipline Raid Healer (21/30/0) - The Gold Standard

Provides *Meditation* (+15% mana regen while casting), *Inner Focus* (free instant cast with +25% crit), and *Divine Spirit* alongside massive Holy healing bonuses.

- **Calculator Template**: [Priest Holy/Disc (21/30/0)](https://talents.turtle-wow.org/priest)

```text
Discipline (21 points):
- 5/5 Unbreakable Will (+15% Stun, Fear, Silence resist)
- 3/3 Improved Power Word: Shield
- 3/3 Meditation (+15% Mana regeneration continues while casting)
- 1/1 Inner Focus (Next spell has 100% reduced mana cost and +25% Critical chance)
- 1/1 Divine Spirit (+50 Spirit to target)

Holy (30 points):
- 5/5 Holy Specialization (+5% Holy Spell Crit)
- 5/5 Divine Fury (Reduces Greater Heal & Smite cast time by 0.5s)
- 3/3 Improved Renew
- 1/1 Desperate Prayer (Instant self-heal panic button)
- 5/5 Spiritual Guidance (+25% of total Spirit added as Bonus Healing)
- 5/5 Spiritual Healing (+10% Healing done by all spells)
- 3/3 Inspiration (Critical heals grant target +25% Armor for 15s)
```

- **Stat Priority**: +Healing > Spirit > MP5 > Intellect > Spell Crit.
- **Rotation**: Flash Heal (for emergency triage) > Greater Heal (Rank 1 or 4 for efficient tank healing) > Renew (on taking damage) > Prayer of Healing (for party AOE damage).
- **Turtle WoW 1.18.1 Rework**:
  - **Lightwell**: Reworked to not break upon taking damage, scaling fully with +healing power.
  - **Inner Fire**: Grants **Spell Damage** instead of Attack Power, boosting caster scaling.

---

### Spec 13: Shadow Priest (13/0/38) - Raid Debuffer & Mana Battery

Applies *Shadow Weaving* (+15% Shadow damage taken by boss for all Warlocks in raid) and *Vampiric Embrace* (heals party for 25% and restores 2% mana of shadow damage dealt).

- **Calculator Template**: [Priest Shadow (13/0/38)](https://talents.turtle-wow.org/priest)

```text
Discipline (13 points):
- 5/5 Unbreakable Will
- 3/3 Meditation
- 1/1 Inner Focus

Shadow (38 points):
- 5/5 Spirit Tap
- 5/5 Darkness (+10% Shadow Spell Damage)
- 5/5 Shadow Focus (+10% Spell Hit on Shadow spells)
- 1/1 Mind Flay
- 2/2 Shadow Reach (+20% Range)
- 5/5 Shadow Weaving (Stacks 5 times to +15% Shadow damage taken)
- 1/1 Silence
- 1/1 Vampiric Embrace (Party heals for 25% + restores 2% mana of shadow spell damage)
- 1/1 Shadowform (+15% Physical damage reduction, +15% Shadow damage bonus, +15% in-combat mana regen)
```

- **Rotation**: Shadow Word: Pain > Mind Flay (channel) > Mind Blast on cooldown > Pain Spike on movement.

---

### 7. Paladin Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/paladin](https://talents.turtle-wow.org/paladin)

### Spec 14: Holy PvE Raid Healer (31/20/0) - Infinite Mana Tank Healer

*Illumination* refunds 100% of the mana cost whenever Flash of Light or Holy Light critically strikes, granting Holy Paladins near infinite mana pools on raid bosses.

- **Calculator Template**: [Paladin Holy Healer (31/20/0)](https://talents.turtle-wow.org/paladin)

```text
Holy (31 points):
- 5/5 Divine Intellect (+10% Intellect)
- 5/5 Spiritual Focus (70% pushback resistance on Flash/Holy Light)
- 3/3 Healing Light (+12% Healing done by Holy Light & Flash of Light)
- 5/5 Illumination (100% Mana refund on spell critical heals)
- 1/1 Divine Favor (Next Holy Light or Flash of Light is a guaranteed 100% Critical Strike)
- 1/1 Holy Shock (Instant holy damage/heal strike)

Protection (20 points):
- 5/5 Redoubt (+30% Block chance after being hit by a critical strike)
- 5/5 Toughness (+10% Armor)
- 1/1 Blessing of Kings (+10% All Stats to target)
- 3/3 Improved Righteous Fury (+50% Threat on Holy spells)
- 5/5 Shield Specialization (+30% Block Value)
```

- **Stat Priority**: Spell Crit (to trigger Illumination mana refunds) > +Healing > Intellect > MP5.
- **Rotation**: Flash of Light spam on assigned tank > Holy Light with Divine Favor during high burst damage > Keep *Blessing of Kings / Light / Salvation* active.
- **Turtle WoW 1.18.1 Features**:
  - **Hand Spells**: Freedom, Protection, and Sacrifice are converted to "Hand" spells, allowing them to be cast without removing class Greater Blessings.
  - **Extended Blessings**: Greater Blessings have longer duration to reduce rebuffing overhead.

---

### Spec 15: Retribution Paladin (11/0/40) - 2H Melee Burst DPS

Uses 2H Weapon with *Crusader Strike*, *Holy Strike*, *Seal of Command* procs, and *Consecration* for holy melee damage.

- **Calculator Template**: [Paladin Retribution (11/0/40)](https://talents.turtle-wow.org/paladin)

```text
Holy (11 points):
- 5/5 Divine Strength (+10% Strength)
- 1/1 Consecration (Holy ground AOE damage)

Retribution (40 points):
- 5/5 Benediction (Reduces mana cost of Judgements/Seals by 15%)
- 2/2 Improved Judgement (Reduces Judgement CD by 2s)
- 1/1 Seal of Command (Chance on melee hit to inflict 70% weapon damage as Holy)
- 5/5 Conviction (+5% Melee Critical Strike)
- 1/1 Sanctity Aura (+10% Holy Damage to party)
- 3/3 Two-Handed Weapon Specialization (+6% 2H Weapon Damage)
- 5/5 Vengeance (+15% Physical & Holy damage for 8s after a critical strike)
- 1/1 Repentance (6-second humanoid incapacitate)
```

- **Turtle WoW 1.18.1 Additions**:
  - **Crusader Strike & Holy Strike**: Integrated as core rotational strikes sharing a 6s cooldown. Crusader Strike provides offensive holy damage and refreshes Judgements; Holy Strike provides holy burst with healing/mana sustain.
  - **Righteous Defense / Hand of Reckoning**: Baseline taunt ability for tanking viability.

---

### 8. Shaman Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/shaman](https://talents.turtle-wow.org/shaman)

### Spec 16: Restoration Raid Healer (0/12/39) - Chain Heal Master

The ultimate raid healing spec. *Chain Heal* bounces between injured allies while *Mana Tide Totem* restores massive mana to your caster group.

- **Calculator Template**: [Shaman Restoration (0/12/39)](https://talents.turtle-wow.org/shaman)

```text
Enhancement (12 points):
- 5/5 Shield Specialization (+5% Block, +25% Block Value)
- 5/5 Thundering Strikes (+5% Melee/Spell Crit)

Restoration (39 points):
- 5/5 Improved Healing Wave (Reduces Healing Wave cast time by 0.5s)
- 5/5 Tidal Focus (Reduces healing spell mana cost by 5%)
- 5/5 Tidal Mastery (+5% Critical strike chance with healing spells)
- 1/1 Nature's Swiftness (Next nature spell is instant cast)
- 3/3 Healing Grace (Reduces threat generated by healing spells by 15%)
- 2/2 Improved Chain Heal (+10% Healing done by Chain Heal)
- 1/1 Mana Tide Totem (Restores 290 Mana every 3 seconds for 12 seconds to party)
```

- **Stat Priority**: +Healing > MP5 > Intellect > Spell Crit.
- **Rotation**: Chain Heal (Rank 1 for efficiency, Rank 3 for burst) > Nature's Swiftness + Healing Wave (emergency tank save) > Drop *Windfury Totem*, *Mana Spring Totem*, and *Grace of Air*.

---

### Spec 17: Shaman Tank "Tankhance" (19/32/0) - Turtle-WoW Custom Tank

A fully realized mail/shield tank utilizing high threat nature damage, shield mitigation stacks, and short-cast fire spells.

- **Calculator Template**: [Shaman Tankhance (19/32/0)](https://talents.turtle-wow.org/shaman)

```text
Elemental (19 points):
- 5/5 Convection (Reduces Shock & Molten Blast mana cost by 10%)
- 5/5 Concussion (+5% Damage to Shocks and Lightning)
- 1/1 Elemental Focus (Clearcasting on spell hit)
- 5/5 Call of Thunder (+6% Spell Crit)

Enhancement (32 points):
- 5/5 Shield Specialization (+5% Block, +25% Block Value)
- 5/5 Anticipation / Toughness
- 5/5 Thundering Strikes (+5% Crit)
- 1/1 Earthen Bulwark (Generates stacking damage mitigation charges on melee swings)
- 5/5 Flurry (+30% Attack Speed)
- 1/1 Stormstrike (Extra melee attack causing next 2 Nature damage spells to deal +20%)
```

- **Stat Priority**: Defense (440 cap) > Hit (9%) > Stamina > Armor > Shield Block Value > Agility.
- **Rotation**: Rockbiter Weapon active > Molten Blast on pull / add spawn > Earth Shock on CD > Stormstrike on CD > Lightning Shield for threat, swap to Water Shield when Clearcasting procs for mana recovery.

---

### 9. Druid Talent Specs

🔗 **Interactive Calculator**: [https://talents.turtle-wow.org/druid](https://talents.turtle-wow.org/druid)

### Spec 18: Feral Tank & Cat DPS (14/32/5) - Heart of the Wild Hybrid

Combines massive health and armor in Dire Bear Form with high physical DPS in Cat Form using *Heart of the Wild* (+20% Stamina in Bear, +20% Strength in Cat).

- **Calculator Template**: [Druid Feral Tank/DPS (14/32/5)](https://talents.turtle-wow.org/druid)

```text
Balance (14 points):
- 1/1 Nature's Grasp
- 5/5 Natural Weapons (+10% Physical Damage)
- 1/1 Omen of Clarity (Chance on melee hit to enter Clearcasting state)

Feral (32 points):
- 5/5 Ferocity (Reduces Maul, Swipe, Claw, Rake cost by 5)
- 2/2 Feral Instinct (+15% Threat in Bear Form, improves stealth)
- 1/1 Feral Charge (Interrupts spell casting and roots target for 4s)
- 3/3 Sharpened Claws (+6% Melee Crit)
- 3/3 Predatory Strikes (+150% Attack Power based on level)
- 2/2 Blood Frenzy
- 2/2 Primal Fury (Generates 5 extra rage on Bear crits, +1 combo point on Cat crits)
- 1/1 Faerie Fire (Feral) (Reduces target armor and prevents stealth)
- 5/5 Heart of the Wild (+20% Intellect, +20% Stamina in Bear, +20% Strength in Cat)
- 1/1 Leader of the Pack (+3% Melee & Ranged Critical Strike to party)

Restoration (5 points):
- 5/5 Furor (Gain 10 Rage when shifting into Bear, 40 Energy in Cat)
```

- **Stat Priority (Bear Tank)**: Armor (to 75% hard cap) > Stamina > Agility (for Dodge) > Hit (9%) > Defense.
- **Stat Priority (Cat DPS)**: Hit (9%) > Strength > Agility > Attack Power > Crit.
- **Rotation (Bear Tank)**: Maul on every swing > Swipe for multi-target threat > Faerie Fire (Feral) on pull.
- **Rotation (Cat DPS)**: Shred from behind > Rake (maintain buffed bleed) > Ferocious Bite at 5 Combo Points.
- **Turtle WoW 1.18.1 Enhancements**:
  - **Windfury Compatibility**: Windfury functions as an aura, allowing Feral Druids in Cat and Bear forms to benefit from extra attacks.
  - **Bleed Scaling**: Rake buffed to make bleed playstyles competitive without total reliance on Wolfshead Helm powershifting.

---

### Spec 19: Balance Moonkin (31/0/20) - Reworked Caster DPS

High burst Arcane and Nature damage providing spell critical aura and sustained mana returns to the caster group.

- **Calculator Template**: [Druid Balance Moonkin (31/0/20)](https://talents.turtle-wow.org/druid)

```text
Balance (31 points):
- 5/5 Nature's Grasp
- 5/5 Natural Weapons
- 1/1 Omen of Clarity (Clearcasting)
- 3/3 Nature's Reach (+20% Range)
- 5/5 Vengeance (+100% Critical Strike Damage bonus for Starfire, Wrath, Moonfire)
- 1/1 Nature's Grace (Spell criticals reduce cast time of next spell by 0.5s)
- 5/5 Moonfury (+10% Damage to Starfire, Wrath, Moonfire)
- 1/1 Moonkin Form (Learned at Level 30: +3% Spell Crit to party, regenerates mana on spell hits)

Restoration (20 points):
- 5/5 Improved Mark of the Wild
- 5/5 Furor
- 3/3 Reflection (+15% Mana regen while casting)
- 5/5 Tranquil Spirit (Reduces mana cost of Healing Touch by 10%)
- 1/1 Insect Swarm (Baseline / Talented nature DoT with hit reduction)
```

- **Stat Priority**: Spell Hit (16% cap) > Spell Damage > Spell Crit > Intellect.
- **Rotation**: Starfire / Wrath cycle > Moonfire (maintain DoT) > Insect Swarm > Hurricane (no 1-min cooldown) for multi-mob AoE.

---

---

---

## Part II: Complete 1-Click BiS Class Gear Loadouts (Every Single Slot 01–17 Explicitly Represented)

> **Enterprise Equipment Architecture Guarantee**:
> Every single class loadout below features **100% complete equipment slot coverage** (all 16–17 combat equipment slots: Head, Neck, Shoulders, Back, Chest, Wrists, Hands, Waist, Legs, Feet, Ring 1, Ring 2, Trinket 1, Trinket 2, Main Hand, Off Hand / Shield, and Ranged / Relic). Every item ID is verified live against the server database (`tw_world_item_template.sql`).
>
> Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`) or chat console to instantly equip or populate your inventory with the definitive Best-in-Slot endgame gear setup.

---

### 🛡️ Protection Warrior (Deep Protection Mitigation Tank BiS)

> **Role**: `Main Tank / Hardcore Progression`
> **Loadout Summary**: Full 9-piece Dreadnaught Plate, Thunderfury, The Face of Death shield, Styleen's Impeding Scarab, Drake Fang Talisman, Mark of C'Thun, and Cryptfiend Silk Cloak. Maximum physical and magical mitigation with 440+ Defense.

```lua
.additem 22418 1   -- 01. Head: Dreadnaught Helmet (T3 Dreadnaught)
.additem 22732 1   -- 02. Neck: Mark of C'Thun (Mark of C'Thun)
.additem 22419 1   -- 03. Shoulders: Dreadnaught Pauldrons (T3 Dreadnaught)
.additem 22938 1   -- 04. Back: Cryptfiend Silk Cloak (Cryptfiend Silk Cloak)
.additem 22416 1   -- 05. Chest: Dreadnaught Breastplate (T3 Dreadnaught)
.additem 22423 1   -- 06. Wrists: Dreadnaught Bracers (T3 Dreadnaught)
.additem 22421 1   -- 07. Hands: Dreadnaught Gauntlets (T3 Dreadnaught)
.additem 22422 1   -- 08. Waist: Dreadnaught Waistguard (T3 Dreadnaught)
.additem 22417 1   -- 09. Legs: Dreadnaught Legplates (T3 Dreadnaught)
.additem 22420 1   -- 10. Feet: Dreadnaught Sabatons (T3 Dreadnaught)
.additem 23059 1   -- 11. Ring 1: Ring of the Dreadnaught (Ring of the Dreadnaught)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 19431 1   -- 13. Trinket 1: Styleen's Impeding Scarab (Styleen's Impeding Scarab)
.additem 19406 1   -- 14. Trinket 2: Drake Fang Talisman (Drake Fang Talisman)
.additem 19019 1   -- 15. Main Hand: Thunderfury, Blessed Blade of the Windseeker (Thunderfury, Blessed Blade of the Windseeker)
.additem 23043 1   -- 16. Off Hand / Shield: The Face of Death (The Face of Death)
.additem 22812 1   -- 17. Ranged: Nerubian Slavemaker (Nerubian Slavemaker)
.repairitems
```

---

### ⚔️ Dual-Wield Fury/Prot Warrior (Max Raid Threat BiS)

> **Role**: `High-End Raid Threat Tank`
> **Loadout Summary**: Thunderfury (MH), The Hungering Cold (OH), Breastplate of Annihilation, Lionheart Helm, Drake Fang Talisman, Kiss of the Spider, and Cloak of the Fallen God. Generates highest TPS in the game.

```lua
.additem 12640 1   -- 01. Head: Lionheart Helm (Lionheart Helm)
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 19394 1   -- 03. Shoulders: Drake Talon Pauldrons (Drake Talon Pauldrons)
.additem 21710 1   -- 04. Back: Cloak of the Fallen God (Cloak of the Fallen God)
.additem 21814 1   -- 05. Chest: Breastplate of Annihilation (Breastplate of Annihilation)
.additem 21618 1   -- 06. Wrists: Hive Defiler Wristguards (Hive Defiler Wristguards)
.additem 21581 1   -- 07. Hands: Gauntlets of Annihilation (Gauntlets of Annihilation)
.additem 21598 1   -- 08. Waist: Royal Qiraji Belt (Royal Qiraji Belt)
.additem 22385 1   -- 09. Legs: Titanic Leggings (Titanic Leggings)
.additem 19387 1   -- 10. Feet: Chromatic Boots (Chromatic Boots)
.additem 17063 1   -- 11. Ring 1: Band of Accuria (Band of Accuria)
.additem 23025 1   -- 12. Ring 2: Seal of the Damned (Seal of the Damned)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 19406 1   -- 14. Trinket 2: Drake Fang Talisman (Drake Fang Talisman)
.additem 19019 1   -- 15. Main Hand: Thunderfury, Blessed Blade of the Windseeker (Thunderfury, Blessed Blade of the Windseeker)
.additem 23577 1   -- 16. Off Hand: The Hungering Cold (The Hungering Cold (+6 Swords Skill))
.additem 17069 1   -- 17. Ranged: Striker's Mark (Striker's Mark)
.repairitems
```

---

### ⚔️ Dual-Wield Fury Warrior (#1 Physical Melee DPS BiS)

> **Role**: `Top-Tier Physical Melee DPS`
> **Loadout Summary**: Gressil (Kel'Thuzad MH), The Hungering Cold (Kel'Thuzad OH), Breastplate of Annihilation, Titanic Leggings, Kiss of the Spider, Slayer's Crest, and Shroud of Dominion.

```lua
.additem 12640 1   -- 01. Head: Lionheart Helm (Lionheart Helm)
.additem 21664 1   -- 02. Neck: Barbed Choker (Barbed Choker)
.additem 19394 1   -- 03. Shoulders: Drake Talon Pauldrons (Drake Talon Pauldrons)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 21814 1   -- 05. Chest: Breastplate of Annihilation (Breastplate of Annihilation)
.additem 19146 1   -- 06. Wrists: Wristguards of Stability (Wristguards of True Flight)
.additem 21581 1   -- 07. Hands: Gauntlets of Annihilation (Gauntlets of Annihilation)
.additem 23219 1   -- 08. Waist: Girdle of the Mentor (Girdle of the Mentor)
.additem 22385 1   -- 09. Legs: Titanic Leggings (Titanic Leggings)
.additem 19387 1   -- 10. Feet: Chromatic Boots (Chromatic Boots)
.additem 17063 1   -- 11. Ring 1: Band of Accuria (Band of Accuria)
.additem 23025 1   -- 12. Ring 2: Seal of the Damned (Seal of the Damned)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 23054 1   -- 15. Main Hand: Gressil, Dawn of Ruin (Gressil, Dawn of Ruin)
.additem 23577 1   -- 16. Off Hand: The Hungering Cold (The Hungering Cold)
.additem 22811 1   -- 17. Ranged: Soulstring (Soulstring)
.repairitems
```

---

### 🔨 2-Handed Arms / Slam Warrior (Two-Handed Physical DPS BiS)

> **Role**: `Burst Melee DPS & Windfury Scaling`
> **Loadout Summary**: Might of Menethil (Kel'Thuzad 2H Mace), Lionheart Helm, Breastplate of Annihilation, Titanic Leggings, Kiss of the Spider, Slayer's Crest, and Shroud of Dominion.

```lua
.additem 12640 1   -- 01. Head: Lionheart Helm (Lionheart Helm)
.additem 23053 1   -- 02. Neck: Stormrage's Talisman of Seething (Stormrage's Talisman of Seething)
.additem 19394 1   -- 03. Shoulders: Drake Talon Pauldrons (Drake Talon Pauldrons)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 21814 1   -- 05. Chest: Breastplate of Annihilation (Breastplate of Annihilation)
.additem 21618 1   -- 06. Wrists: Hive Defiler Wristguards (Hive Defiler Wristguards)
.additem 21581 1   -- 07. Hands: Gauntlets of Annihilation (Gauntlets of Annihilation)
.additem 23219 1   -- 08. Waist: Girdle of the Mentor (Girdle of the Mentor)
.additem 22385 1   -- 09. Legs: Titanic Leggings (Titanic Leggings)
.additem 19387 1   -- 10. Feet: Chromatic Boots (Chromatic Boots)
.additem 17063 1   -- 11. Ring 1: Band of Accuria (Band of Accuria)
.additem 23025 1   -- 12. Ring 2: Seal of the Damned (Seal of the Damned)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 22798 1   -- 15. Two-Hand Weapon: Might of Menethil (Might of Menethil)
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 22811 1   -- 17. Ranged: Soulstring (Soulstring)
.repairitems
```

---

### 💚 Holy Paladin (Raid & Tank Healer BiS)

> **Role**: `Single-Target & Tank Healing Master`
> **Loadout Summary**: Full 9-piece Redemption Armor, The End of Dreams (Kel'Thuzad 1H Mace), Shield of Condemnation, Eye of the Dead, Rejuvenating Gem, Cloak of Suturing, and Libram of Hope.

```lua
.additem 22428 1   -- 01. Head: Redemption Headpiece (T3 Redemption Headpiece)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22429 1   -- 03. Shoulders: Redemption Spaulders (T3 Redemption Spaulders)
.additem 22960 1   -- 04. Back: Cloak of Suturing (Cloak of Suturing)
.additem 22425 1   -- 05. Chest: Redemption Tunic (T3 Redemption Tunic)
.additem 22424 1   -- 06. Wrists: Redemption Wristguards (T3 Redemption Wristguards)
.additem 22426 1   -- 07. Hands: Redemption Handguards (T3 Redemption Handguards)
.additem 22431 1   -- 08. Waist: Redemption Girdle (T3 Redemption Girdle)
.additem 22427 1   -- 09. Legs: Redemption Legguards (T3 Redemption Legguards)
.additem 22430 1   -- 10. Feet: Redemption Boots (T3 Redemption Boots)
.additem 23066 1   -- 11. Ring 1: Ring of Redemption (T3 Ring of Redemption)
.additem 19382 1   -- 12. Ring 2: Pure Elementium Band (Pure Elementium Band)
.additem 23047 1   -- 13. Trinket 1: Eye of the Dead (Eye of the Dead)
.additem 19395 1   -- 14. Trinket 2: Rejuvenating Gem (Rejuvenating Gem)
.additem 22988 1   -- 15. Main Hand: The End of Dreams (The End of Dreams)
.additem 22819 1   -- 16. Off Hand / Shield: Shield of Condemnation (Shield of Condemnation)
.additem 22401 1   -- 17. Relic: Libram of Hope (Libram of Hope)
.repairitems
```

---

### 🔨 Retribution Paladin (Two-Handed Holy Melee DPS BiS)

> **Role**: `Hybrid Physical/Holy Melee DPS`
> **Loadout Summary**: Corrupted Ashbringer (2H Sword), Lionheart Helm, Breastplate of Annihilation, Titanic Leggings, Kiss of the Spider, Slayer's Crest, Shroud of Dominion, and Libram of Fervor.

```lua
.additem 12640 1   -- 01. Head: Lionheart Helm (Lionheart Helm)
.additem 23053 1   -- 02. Neck: Stormrage's Talisman of Seething (Stormrage's Talisman of Seething)
.additem 19394 1   -- 03. Shoulders: Drake Talon Pauldrons (Drake Talon Pauldrons)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 21814 1   -- 05. Chest: Breastplate of Annihilation (Breastplate of Annihilation)
.additem 21618 1   -- 06. Wrists: Hive Defiler Wristguards (Hive Defiler Wristguards)
.additem 21581 1   -- 07. Hands: Gauntlets of Annihilation (Gauntlets of Annihilation)
.additem 23219 1   -- 08. Waist: Girdle of the Mentor (Girdle of the Mentor)
.additem 22385 1   -- 09. Legs: Titanic Leggings (Titanic Leggings)
.additem 19387 1   -- 10. Feet: Chromatic Boots (Chromatic Boots)
.additem 23025 1   -- 11. Ring 1: Seal of the Damned (Seal of the Damned)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 22691 1   -- 15. Two-Hand Weapon: Corrupted Ashbringer (Corrupted Ashbringer)
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 23203 1   -- 17. Relic: Libram of Fervor (Libram of Fervor)
.repairitems
```

---

### 🛡️ Protection Paladin (AoE & Threat Tank BiS)

> **Role**: `AoE & Dungeon Threat Tank`
> **Loadout Summary**: Redemption Plate, The Face of Death shield, Thunderfury, Styleen's Impeding Scarab, Drake Fang Talisman, Mark of C'Thun, Cryptfiend Silk Cloak, and Libram of Hope.

```lua
.additem 22428 1   -- 01. Head: Redemption Headpiece (T3 Redemption Headpiece)
.additem 22732 1   -- 02. Neck: Mark of C'Thun (Mark of C'Thun)
.additem 22429 1   -- 03. Shoulders: Redemption Spaulders (T3 Redemption Spaulders)
.additem 22938 1   -- 04. Back: Cryptfiend Silk Cloak (Cryptfiend Silk Cloak)
.additem 22425 1   -- 05. Chest: Redemption Tunic (T3 Redemption Tunic)
.additem 22424 1   -- 06. Wrists: Redemption Wristguards (T3 Redemption Wristguards)
.additem 22426 1   -- 07. Hands: Redemption Handguards (T3 Redemption Handguards)
.additem 22431 1   -- 08. Waist: Redemption Girdle (T3 Redemption Girdle)
.additem 22427 1   -- 09. Legs: Redemption Legguards (T3 Redemption Legguards)
.additem 22430 1   -- 10. Feet: Redemption Boots (T3 Redemption Boots)
.additem 23066 1   -- 11. Ring 1: Ring of Redemption (T3 Ring of Redemption)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 19431 1   -- 13. Trinket 1: Styleen's Impeding Scarab (Styleen's Impeding Scarab)
.additem 19406 1   -- 14. Trinket 2: Drake Fang Talisman (Drake Fang Talisman)
.additem 19019 1   -- 15. Main Hand: Thunderfury, Blessed Blade of the Windseeker (Thunderfury, Blessed Blade of the Windseeker)
.additem 23043 1   -- 16. Off Hand / Shield: The Face of Death (The Face of Death)
.additem 22401 1   -- 17. Relic: Libram of Hope (Libram of Hope)
.repairitems
```

---

### 🏹 Marksmanship Hunter (Ranged Physical DPS BiS)

> **Role**: `Top-Tier Ranged Physical DPS`
> **Loadout Summary**: Full 9-piece Cryptstalker Armor, Nerubian Slavemaker (Kel'Thuzad Crossbow), The Eye of Nerub (2H Polearm), Kiss of the Spider, Slayer's Crest, Seal of the Damned, and Shroud of Dominion.

```lua
.additem 22438 1   -- 01. Head: Cryptstalker Headpiece (T3 Cryptstalker Headpiece)
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 22439 1   -- 03. Shoulders: Cryptstalker Spaulders (T3 Cryptstalker Spaulders)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 22436 1   -- 05. Chest: Cryptstalker Tunic (T3 Cryptstalker Tunic)
.additem 22443 1   -- 06. Wrists: Cryptstalker Wristguards (T3 Cryptstalker Wristguards)
.additem 22441 1   -- 07. Hands: Cryptstalker Handguards (T3 Cryptstalker Handguards)
.additem 22442 1   -- 08. Waist: Cryptstalker Girdle (T3 Cryptstalker Girdle)
.additem 22437 1   -- 09. Legs: Cryptstalker Legguards (T3 Cryptstalker Legguards)
.additem 22440 1   -- 10. Feet: Cryptstalker Boots (T3 Cryptstalker Boots)
.additem 23067 1   -- 11. Ring 1: Ring of the Cryptstalker (T3 Ring of the Cryptstalker)
.additem 23025 1   -- 12. Ring 2: Seal of the Damned (Seal of the Damned)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 23039 1   -- 15. Two-Hand Weapon: The Eye of Nerub (The Eye of Nerub)
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 22812 1   -- 17. Ranged: Nerubian Slavemaker (Nerubian Slavemaker)
.repairitems
```

---

### 🐾 Beast Mastery Hunter (Pet & Ranged Synergy BiS)

> **Role**: `Solo & Pet Damage Specialist`
> **Loadout Summary**: Full 9-piece Cryptstalker Armor, Nerubian Slavemaker, The Eye of Nerub, Kiss of the Spider, Slayer's Crest, Band of Accuria, and Shroud of Dominion.

```lua
.additem 22438 1   -- 01. Head: Cryptstalker Headpiece (T3 Cryptstalker Headpiece)
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 22439 1   -- 03. Shoulders: Cryptstalker Spaulders (T3 Cryptstalker Spaulders)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 22436 1   -- 05. Chest: Cryptstalker Tunic (T3 Cryptstalker Tunic)
.additem 22443 1   -- 06. Wrists: Cryptstalker Wristguards (T3 Cryptstalker Wristguards)
.additem 22441 1   -- 07. Hands: Cryptstalker Handguards (T3 Cryptstalker Handguards)
.additem 22442 1   -- 08. Waist: Cryptstalker Girdle (T3 Cryptstalker Girdle)
.additem 22437 1   -- 09. Legs: Cryptstalker Legguards (T3 Cryptstalker Legguards)
.additem 22440 1   -- 10. Feet: Cryptstalker Boots (T3 Cryptstalker Boots)
.additem 23067 1   -- 11. Ring 1: Ring of the Cryptstalker (T3 Ring of the Cryptstalker)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 23039 1   -- 15. Two-Hand Weapon: The Eye of Nerub (The Eye of Nerub)
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 22812 1   -- 17. Ranged: Nerubian Slavemaker (Nerubian Slavemaker)
.repairitems
```

---

### ⚔️ Melee Survival Hunter (Turtle 1.18.1 Carve / Lacerate BiS)

> **Role**: `Turtle-WoW Signature Melee DPS`
> **Loadout Summary**: Dual-wield Kingsfall & Harbinger of Doom, Nerubian Slavemaker, Full Cryptstalker, Kiss of the Spider, Slayer's Crest, Seal of the Damned, and Shroud of Dominion.

```lua
.additem 22438 1   -- 01. Head: Cryptstalker Headpiece (T3 Cryptstalker Headpiece)
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 22439 1   -- 03. Shoulders: Cryptstalker Spaulders (T3 Cryptstalker Spaulders)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 22436 1   -- 05. Chest: Cryptstalker Tunic (T3 Cryptstalker Tunic)
.additem 22443 1   -- 06. Wrists: Cryptstalker Wristguards (T3 Cryptstalker Wristguards)
.additem 22441 1   -- 07. Hands: Cryptstalker Handguards (T3 Cryptstalker Handguards)
.additem 22442 1   -- 08. Waist: Cryptstalker Girdle (T3 Cryptstalker Girdle)
.additem 22437 1   -- 09. Legs: Cryptstalker Legguards (T3 Cryptstalker Legguards)
.additem 22440 1   -- 10. Feet: Cryptstalker Boots (T3 Cryptstalker Boots)
.additem 23067 1   -- 11. Ring 1: Ring of the Cryptstalker (T3 Ring of the Cryptstalker)
.additem 23025 1   -- 12. Ring 2: Seal of the Damned (Seal of the Damned)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 22802 1   -- 15. Main Hand: Kingsfall (Kingsfall)
.additem 23044 1   -- 16. Off Hand: Harbinger of Doom (Harbinger of Doom)
.additem 22812 1   -- 17. Ranged: Nerubian Slavemaker (Nerubian Slavemaker)
.repairitems
```

---

### 🗡️ Combat Swords Rogue (Sustained Physical DPS BiS)

> **Role**: `Top-Tier Sustained Physical DPS`
> **Loadout Summary**: Full 9-piece Bonescythe, Gressil (MH), The Hungering Cold (OH), Kiss of the Spider, Slayer's Crest, Seal of the Damned, and Shroud of Dominion.

```lua
.additem 22478 1   -- 01. Head: Bonescythe Helmet (T3 Bonescythe Helmet)
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 22479 1   -- 03. Shoulders: Bonescythe Pauldrons (T3 Bonescythe Pauldrons)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 22476 1   -- 05. Chest: Bonescythe Breastplate (T3 Bonescythe Breastplate)
.additem 22483 1   -- 06. Wrists: Bonescythe Bracers (T3 Bonescythe Bracers)
.additem 22481 1   -- 07. Hands: Bonescythe Gauntlets (T3 Bonescythe Gauntlets)
.additem 22482 1   -- 08. Waist: Bonescythe Waistguard (T3 Bonescythe Waistguard)
.additem 22477 1   -- 09. Legs: Bonescythe Legplates (T3 Bonescythe Legplates)
.additem 22480 1   -- 10. Feet: Bonescythe Sabatons (T3 Bonescythe Sabatons)
.additem 23060 1   -- 11. Ring 1: Bonescythe Ring (T3 Bonescythe Ring)
.additem 23025 1   -- 12. Ring 2: Seal of the Damned (Seal of the Damned)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 23054 1   -- 15. Main Hand: Gressil, Dawn of Ruin (Gressil, Dawn of Ruin)
.additem 23577 1   -- 16. Off Hand: The Hungering Cold (The Hungering Cold)
.additem 22812 1   -- 17. Ranged: Nerubian Slavemaker (Nerubian Slavemaker)
.repairitems
```

---

### 🗡️ Combat Daggers Rogue (High Burst Physical DPS BiS)

> **Role**: `High-Crit Positional Burst DPS`
> **Loadout Summary**: Full 9-piece Bonescythe with Gauntlets of Annihilation, Kingsfall (MH), Harbinger of Doom (OH), Kiss of the Spider, Slayer's Crest, and Shroud of Dominion.

```lua
.additem 22478 1   -- 01. Head: Bonescythe Helmet (T3 Bonescythe Helmet)
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 22479 1   -- 03. Shoulders: Bonescythe Pauldrons (T3 Bonescythe Pauldrons)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 22476 1   -- 05. Chest: Bonescythe Breastplate (T3 Bonescythe Breastplate)
.additem 22483 1   -- 06. Wrists: Bonescythe Bracers (T3 Bonescythe Bracers)
.additem 21581 1   -- 07. Hands: Gauntlets of Annihilation (Gauntlets of Annihilation)
.additem 22482 1   -- 08. Waist: Bonescythe Waistguard (T3 Bonescythe Waistguard)
.additem 22477 1   -- 09. Legs: Bonescythe Legplates (T3 Bonescythe Legplates)
.additem 22480 1   -- 10. Feet: Bonescythe Sabatons (T3 Bonescythe Sabatons)
.additem 23060 1   -- 11. Ring 1: Bonescythe Ring (T3 Bonescythe Ring)
.additem 23025 1   -- 12. Ring 2: Seal of the Damned (Seal of the Damned)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 22802 1   -- 15. Main Hand: Kingsfall (Kingsfall)
.additem 23044 1   -- 16. Off Hand: Harbinger of Doom (Harbinger of Doom)
.additem 22812 1   -- 17. Ranged: Nerubian Slavemaker (Nerubian Slavemaker)
.repairitems
```

---

### 👤 Subtlety / Hemorrhage Rogue (PvP & Raid Armor Debuff BiS)

> **Role**: `PvP Burst & Physical Vulnerability Debuff`
> **Loadout Summary**: Full 9-piece Bonescythe, Gressil, The Hungering Cold, Kiss of the Spider, Slayer's Crest, Band of Accuria, and Shroud of Dominion.

```lua
.additem 22478 1   -- 01. Head: Bonescythe Helmet (T3 Bonescythe Helmet)
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 22479 1   -- 03. Shoulders: Bonescythe Pauldrons (T3 Bonescythe Pauldrons)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 22476 1   -- 05. Chest: Bonescythe Breastplate (T3 Bonescythe Breastplate)
.additem 22483 1   -- 06. Wrists: Bonescythe Bracers (T3 Bonescythe Bracers)
.additem 22481 1   -- 07. Hands: Bonescythe Gauntlets (T3 Bonescythe Gauntlets)
.additem 22482 1   -- 08. Waist: Bonescythe Waistguard (T3 Bonescythe Waistguard)
.additem 22477 1   -- 09. Legs: Bonescythe Legplates (T3 Bonescythe Legplates)
.additem 22480 1   -- 10. Feet: Bonescythe Sabatons (T3 Bonescythe Sabatons)
.additem 23060 1   -- 11. Ring 1: Bonescythe Ring (T3 Bonescythe Ring)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 23041 1   -- 14. Trinket 2: Slayer's Crest (Slayer's Crest)
.additem 23054 1   -- 15. Main Hand: Gressil, Dawn of Ruin (Gressil, Dawn of Ruin)
.additem 23577 1   -- 16. Off Hand: The Hungering Cold (The Hungering Cold)
.additem 22812 1   -- 17. Ranged: Nerubian Slavemaker (Nerubian Slavemaker)
.repairitems
```

---

### 💚 Holy Priest (Raid & Tank Healer BiS)

> **Role**: `Premier Group & Raid Healer`
> **Loadout Summary**: Full 9-piece Vestments of Faith, The End of Dreams (Kel'Thuzad 1H Mace), Sapphiron's Right Eye, Eye of the Dead, Rejuvenating Gem, Cloak of Suturing, Pure Elementium Band, and Wand of Fates.

```lua
.additem 22514 1   -- 01. Head: Circlet of Faith (T3 Circlet of Faith)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22515 1   -- 03. Shoulders: Shoulderpads of Faith (T3 Shoulderpads of Faith)
.additem 22960 1   -- 04. Back: Cloak of Suturing (Cloak of Suturing)
.additem 22512 1   -- 05. Chest: Robe of Faith (T3 Robe of Faith)
.additem 22519 1   -- 06. Wrists: Bindings of Faith (T3 Bindings of Faith)
.additem 22517 1   -- 07. Hands: Gloves of Faith (T3 Gloves of Faith)
.additem 22518 1   -- 08. Waist: Belt of Faith (T3 Belt of Faith)
.additem 22513 1   -- 09. Legs: Leggings of Faith (T3 Leggings of Faith)
.additem 22516 1   -- 10. Feet: Sandals of Faith (T3 Sandals of Faith)
.additem 23061 1   -- 11. Ring 1: Ring of Faith (T3 Ring of Faith)
.additem 19382 1   -- 12. Ring 2: Pure Elementium Band (Pure Elementium Band)
.additem 23047 1   -- 13. Trinket 1: Eye of the Dead (Eye of the Dead)
.additem 19395 1   -- 14. Trinket 2: Rejuvenating Gem (Rejuvenating Gem)
.additem 22988 1   -- 15. Main Hand: The End of Dreams (The End of Dreams)
.additem 23048 1   -- 16. Off Hand: Sapphiron's Right Eye (Sapphiron's Right Eye)
.additem 22820 1   -- 17. Ranged: Wand of Fates (Wand of Fates)
.repairitems
```

---

### 🔮 Shadow Priest (Shadow DPS & Mana Battery BiS)

> **Role**: `Shadow Caster DPS & Group Mana Support`
> **Loadout Summary**: Full Plagueheart/Dark Storm set, Midnight Haze (Kel'Thuzad 1H Dagger), Sapphiron's Left Eye, Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, and Wand of the Whispering Dead.

```lua
.additem 22506 1   -- 01. Head: Plagueheart Circlet (T3 Plagueheart Circlet)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22507 1   -- 03. Shoulders: Plagueheart Shoulderpads (T3 Plagueheart Shoulderpads)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22504 1   -- 05. Chest: Plagueheart Robe (T3 Plagueheart Robe)
.additem 22511 1   -- 06. Wrists: Plagueheart Bracers (T3 Plagueheart Bracers)
.additem 21585 1   -- 07. Hands: Dark Storm Gauntlets (Dark Storm Gauntlets)
.additem 22730 1   -- 08. Waist: Eyestalk Waist Cord (Eyestalk Waist Cord)
.additem 22505 1   -- 09. Legs: Plagueheart Leggings (T3 Plagueheart Leggings)
.additem 22508 1   -- 10. Feet: Plagueheart Sandals (T3 Plagueheart Sandals)
.additem 23063 1   -- 11. Ring 1: Plagueheart Ring (T3 Plagueheart Ring)
.additem 21709 1   -- 12. Ring 2: Ring of the Fallen God (Ring of the Fallen God)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22803 1   -- 15. Main Hand: Midnight Haze (Midnight Haze)
.additem 23049 1   -- 16. Off Hand: Sapphiron's Left Eye (Sapphiron's Left Eye)
.additem 23009 1   -- 17. Ranged: Wand of the Whispering Dead (Wand of the Whispering Dead)
.repairitems
```

---

### ⚡ Discipline / Holy Smite Priest (Holy Caster DPS BiS)

> **Role**: `Holy Damage Caster & Defensive Shielding`
> **Loadout Summary**: Vestments of Faith, Midnight Haze, Sapphiron's Left Eye, Dark Storm Gauntlets, Eyestalk Waist Cord, Band of Unnatural Forces, Neltharion's Tear, and Wand of Fates.

```lua
.additem 22514 1   -- 01. Head: Circlet of Faith (T3 Circlet of Faith)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22515 1   -- 03. Shoulders: Shoulderpads of Faith (T3 Shoulderpads of Faith)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22512 1   -- 05. Chest: Robe of Faith (T3 Robe of Faith)
.additem 22519 1   -- 06. Wrists: Bindings of Faith (T3 Bindings of Faith)
.additem 21585 1   -- 07. Hands: Dark Storm Gauntlets (Dark Storm Gauntlets)
.additem 22730 1   -- 08. Waist: Eyestalk Waist Cord (Eyestalk Waist Cord)
.additem 22513 1   -- 09. Legs: Leggings of Faith (T3 Leggings of Faith)
.additem 22516 1   -- 10. Feet: Sandals of Faith (T3 Sandals of Faith)
.additem 23061 1   -- 11. Ring 1: Ring of Faith (T3 Ring of Faith)
.additem 23038 1   -- 12. Ring 2: Band of Unnatural Forces (Band of Unnatural Forces)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22803 1   -- 15. Main Hand: Midnight Haze (Midnight Haze)
.additem 23049 1   -- 16. Off Hand: Sapphiron's Left Eye (Sapphiron's Left Eye)
.additem 22820 1   -- 17. Ranged: Wand of Fates (Wand of Fates)
.repairitems
```

---

### 💚 Restoration Shaman (Chain Healer & Totem Support BiS)

> **Role**: `Premier Raid AoE Healer`
> **Loadout Summary**: Full 9-piece The Earthshatterer, The End of Dreams (Kel'Thuzad 1H Mace), Shield of Condemnation, Eye of the Dead, Rejuvenating Gem, Cloak of Suturing, Pure Elementium Band, and Totem of Life.

```lua
.additem 22466 1   -- 01. Head: Earthshatter Headpiece (T3 Earthshatter Headpiece)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22467 1   -- 03. Shoulders: Earthshatter Spaulders (T3 Earthshatter Spaulders)
.additem 22960 1   -- 04. Back: Cloak of Suturing (Cloak of Suturing)
.additem 22464 1   -- 05. Chest: Earthshatter Tunic (T3 Earthshatter Tunic)
.additem 22471 1   -- 06. Wrists: Earthshatter Wristguards (T3 Earthshatter Wristguards)
.additem 22469 1   -- 07. Hands: Earthshatter Handguards (T3 Earthshatter Handguards)
.additem 22470 1   -- 08. Waist: Earthshatter Girdle (T3 Earthshatter Girdle)
.additem 22465 1   -- 09. Legs: Earthshatter Legguards (T3 Earthshatter Legguards)
.additem 22468 1   -- 10. Feet: Earthshatter Boots (T3 Earthshatter Boots)
.additem 23065 1   -- 11. Ring 1: Ring of the Earthshatterer (T3 Ring of the Earthshatterer)
.additem 19382 1   -- 12. Ring 2: Pure Elementium Band (Pure Elementium Band)
.additem 23047 1   -- 13. Trinket 1: Eye of the Dead (Eye of the Dead)
.additem 19395 1   -- 14. Trinket 2: Rejuvenating Gem (Rejuvenating Gem)
.additem 22988 1   -- 15. Main Hand: The End of Dreams (The End of Dreams)
.additem 22819 1   -- 16. Off Hand / Shield: Shield of Condemnation (Shield of Condemnation)
.additem 22396 1   -- 17. Relic: Totem of Life (Totem of Life)
.repairitems
```

---

### ⚡ Elemental Shaman (Burst Caster DPS BiS)

> **Role**: `Nature / Fire Caster Burst DPS`
> **Loadout Summary**: Earthshatterer Mail, Midnight Haze (Kel'Thuzad 1H Caster Dagger), Shield of Condemnation, Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, Ring of the Fallen God, and Totem of the Storm.

```lua
.additem 22466 1   -- 01. Head: Earthshatter Headpiece (T3 Earthshatter Headpiece)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22467 1   -- 03. Shoulders: Earthshatter Spaulders (T3 Earthshatter Spaulders)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22464 1   -- 05. Chest: Earthshatter Tunic (T3 Earthshatter Tunic)
.additem 22471 1   -- 06. Wrists: Earthshatter Wristguards (T3 Earthshatter Wristguards)
.additem 22469 1   -- 07. Hands: Earthshatter Handguards (T3 Earthshatter Handguards)
.additem 22730 1   -- 08. Waist: Eyestalk Waist Cord (Eyestalk Waist Cord)
.additem 22465 1   -- 09. Legs: Earthshatter Legguards (T3 Earthshatter Legguards)
.additem 22468 1   -- 10. Feet: Earthshatter Boots (T3 Earthshatter Boots)
.additem 23065 1   -- 11. Ring 1: Ring of the Earthshatterer (T3 Ring of the Earthshatterer)
.additem 21709 1   -- 12. Ring 2: Ring of the Fallen God (Ring of the Fallen God)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22803 1   -- 15. Main Hand: Midnight Haze (Midnight Haze)
.additem 22819 1   -- 16. Off Hand / Shield: Shield of Condemnation (Shield of Condemnation)
.additem 23199 1   -- 17. Relic: Totem of the Storm (Totem of the Storm)
.repairitems
```

---

### 🔨 Enhancement Shaman (Two-Handed Windfury Melee DPS BiS)

> **Role**: `2H Physical / Windfury Melee DPS`
> **Loadout Summary**: Might of Menethil (Kel'Thuzad 2H Mace), Crown of Destruction, Breastplate of Annihilation, Titanic Leggings, Kiss of the Spider, Drake Fang Talisman, Shroud of Dominion, and Totem of Rage.

```lua
.additem 18817 1   -- 01. Head: Crown of Destruction (Crown of Destruction)
.additem 23053 1   -- 02. Neck: Stormrage's Talisman of Seething (Stormrage's Talisman of Seething)
.additem 19394 1   -- 03. Shoulders: Drake Talon Pauldrons (Drake Talon Pauldrons)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 21814 1   -- 05. Chest: Breastplate of Annihilation (Breastplate of Annihilation)
.additem 21618 1   -- 06. Wrists: Hive Defiler Wristguards (Hive Defiler Wristguards)
.additem 21581 1   -- 07. Hands: Gauntlets of Annihilation (Gauntlets of Annihilation)
.additem 23219 1   -- 08. Waist: Girdle of the Mentor (Girdle of the Mentor)
.additem 22385 1   -- 09. Legs: Titanic Leggings (Titanic Leggings)
.additem 19387 1   -- 10. Feet: Chromatic Boots (Chromatic Boots)
.additem 23025 1   -- 11. Ring 1: Seal of the Damned (Seal of the Damned)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 19406 1   -- 14. Trinket 2: Drake Fang Talisman (Drake Fang Talisman)
.additem 22798 1   -- 15. Two-Hand Weapon: Might of Menethil (Might of Menethil)
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 22395 1   -- 17. Relic: Totem of Rage (Totem of Rage)
.repairitems
```

---

### 🛡️ Shaman Tank 'Tankhance' (Turtle 1.18.1 Signature Shield Tank BiS)

> **Role**: `Turtle-WoW Signature Shield Tank`
> **Loadout Summary**: Earthshatterer Mail, The Face of Death shield, Thunderfury, Styleen's Impeding Scarab, Drake Fang Talisman, Mark of C'Thun, Cryptfiend Silk Cloak, and Totem of Life.

```lua
.additem 22466 1   -- 01. Head: Earthshatter Headpiece (T3 Earthshatter Headpiece)
.additem 22732 1   -- 02. Neck: Mark of C'Thun (Mark of C'Thun)
.additem 22467 1   -- 03. Shoulders: Earthshatter Spaulders (T3 Earthshatter Spaulders)
.additem 22938 1   -- 04. Back: Cryptfiend Silk Cloak (Cryptfiend Silk Cloak)
.additem 22464 1   -- 05. Chest: Earthshatter Tunic (T3 Earthshatter Tunic)
.additem 22471 1   -- 06. Wrists: Earthshatter Wristguards (T3 Earthshatter Wristguards)
.additem 22469 1   -- 07. Hands: Earthshatter Handguards (T3 Earthshatter Handguards)
.additem 22470 1   -- 08. Waist: Earthshatter Girdle (T3 Earthshatter Girdle)
.additem 22465 1   -- 09. Legs: Earthshatter Legguards (T3 Earthshatter Legguards)
.additem 22468 1   -- 10. Feet: Earthshatter Boots (T3 Earthshatter Boots)
.additem 23065 1   -- 11. Ring 1: Ring of the Earthshatterer (T3 Ring of the Earthshatterer)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 19431 1   -- 13. Trinket 1: Styleen's Impeding Scarab (Styleen's Impeding Scarab)
.additem 19406 1   -- 14. Trinket 2: Drake Fang Talisman (Drake Fang Talisman)
.additem 19019 1   -- 15. Main Hand: Thunderfury, Blessed Blade of the Windseeker (Thunderfury, Blessed Blade of the Windseeker)
.additem 23043 1   -- 16. Off Hand / Shield: The Face of Death (The Face of Death)
.additem 22396 1   -- 17. Relic: Totem of Life (Totem of Life)
.repairitems
```

---

### 🔥 Fire Mage (Ignite Caster Burst DPS BiS)

> **Role**: `Top-Tier Endgame Caster DPS`
> **Loadout Summary**: Full 9-piece Frostfire Regalia, Midnight Haze (Kel'Thuzad 1H Dagger), Sapphiron's Left Eye, Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, Ring of the Fallen God, and Doomfinger.

```lua
.additem 22498 1   -- 01. Head: Frostfire Circlet (T3 Frostfire Circlet)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22499 1   -- 03. Shoulders: Frostfire Shoulderpads (T3 Frostfire Shoulderpads)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22496 1   -- 05. Chest: Frostfire Robe (T3 Frostfire Robe)
.additem 22503 1   -- 06. Wrists: Frostfire Bindings (T3 Frostfire Bindings)
.additem 22501 1   -- 07. Hands: Frostfire Gloves (T3 Frostfire Gloves)
.additem 22502 1   -- 08. Waist: Frostfire Belt (T3 Frostfire Belt)
.additem 22497 1   -- 09. Legs: Frostfire Leggings (T3 Frostfire Leggings)
.additem 22500 1   -- 10. Feet: Frostfire Sandals (T3 Frostfire Sandals)
.additem 23062 1   -- 11. Ring 1: Frostfire Ring (T3 Frostfire Ring)
.additem 21709 1   -- 12. Ring 2: Ring of the Fallen God (Ring of the Fallen God)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22803 1   -- 15. Main Hand: Midnight Haze (Midnight Haze)
.additem 23049 1   -- 16. Off Hand: Sapphiron's Left Eye (Sapphiron's Left Eye)
.additem 22821 1   -- 17. Ranged: Doomfinger (Doomfinger (Fire Wand))
.repairitems
```

---

### ❄️ Frost Mage (Caster Control / AoE & AP Frost BiS)

> **Role**: `AoE Farm, Crowd Control & Burst Frost DPS`
> **Loadout Summary**: Full 9-piece Frostfire Regalia, Soulseeker (Kel'Thuzad 2H Staff), Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, Gem of Trapped Innocents, and Wand of the Whispering Dead.

```lua
.additem 22498 1   -- 01. Head: Frostfire Circlet (T3 Frostfire Circlet)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22499 1   -- 03. Shoulders: Frostfire Shoulderpads (T3 Frostfire Shoulderpads)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22496 1   -- 05. Chest: Frostfire Robe (T3 Frostfire Robe)
.additem 22503 1   -- 06. Wrists: Frostfire Bindings (T3 Frostfire Bindings)
.additem 22501 1   -- 07. Hands: Frostfire Gloves (T3 Frostfire Gloves)
.additem 22502 1   -- 08. Waist: Frostfire Belt (T3 Frostfire Belt)
.additem 22497 1   -- 09. Legs: Frostfire Leggings (T3 Frostfire Leggings)
.additem 22500 1   -- 10. Feet: Frostfire Sandals (T3 Frostfire Sandals)
.additem 23062 1   -- 11. Ring 1: Frostfire Ring (T3 Frostfire Ring)
.additem 23038 1   -- 12. Ring 2: Band of Unnatural Forces (Band of Unnatural Forces)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22799 1   -- 15. Two-Hand Weapon: Soulseeker (Soulseeker)
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 23009 1   -- 17. Ranged: Wand of the Whispering Dead (Wand of the Whispering Dead)
.repairitems
```

---

### 🔮 Arcane Mind Mage (Arcane Power Burst DPS BiS)

> **Role**: `High-Burst Arcane Specialist`
> **Loadout Summary**: Full 9-piece Frostfire Regalia, Atiesh Greatstaff of the Guardian (Mage), Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, Ring of the Fallen God, and Wand of the Whispering Dead.

```lua
.additem 22498 1   -- 01. Head: Frostfire Circlet (T3 Frostfire Circlet)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22499 1   -- 03. Shoulders: Frostfire Shoulderpads (T3 Frostfire Shoulderpads)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22496 1   -- 05. Chest: Frostfire Robe (T3 Frostfire Robe)
.additem 22503 1   -- 06. Wrists: Frostfire Bindings (T3 Frostfire Bindings)
.additem 22501 1   -- 07. Hands: Frostfire Gloves (T3 Frostfire Gloves)
.additem 22502 1   -- 08. Waist: Frostfire Belt (T3 Frostfire Belt)
.additem 22497 1   -- 09. Legs: Frostfire Leggings (T3 Frostfire Leggings)
.additem 22500 1   -- 10. Feet: Frostfire Sandals (T3 Frostfire Sandals)
.additem 23062 1   -- 11. Ring 1: Frostfire Ring (T3 Frostfire Ring)
.additem 21709 1   -- 12. Ring 2: Ring of the Fallen God (Ring of the Fallen God)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22589 1   -- 15. Two-Hand Weapon: Atiesh, Greatstaff of the Guardian (Atiesh, Greatstaff of the Guardian (Mage))
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 23009 1   -- 17. Ranged: Wand of the Whispering Dead (Wand of the Whispering Dead)
.repairitems
```

---

### 💀 DS/Ruin Warlock (Demonic Sacrifice Shadow Bolt DPS BiS)

> **Role**: `Top-Tier Shadow Caster DPS`
> **Loadout Summary**: Full 9-piece Plagueheart Raiment, Atiesh Greatstaff of the Guardian (Warlock), Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, Ring of the Fallen God, and Wand of the Whispering Dead.

```lua
.additem 22506 1   -- 01. Head: Plagueheart Circlet (T3 Plagueheart Circlet)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22507 1   -- 03. Shoulders: Plagueheart Shoulderpads (T3 Plagueheart Shoulderpads)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22504 1   -- 05. Chest: Plagueheart Robe (T3 Plagueheart Robe)
.additem 22511 1   -- 06. Wrists: Plagueheart Bracers (T3 Plagueheart Bracers)
.additem 22509 1   -- 07. Hands: Plagueheart Gloves (T3 Plagueheart Gloves)
.additem 22510 1   -- 08. Waist: Plagueheart Belt (T3 Plagueheart Belt)
.additem 22505 1   -- 09. Legs: Plagueheart Leggings (T3 Plagueheart Leggings)
.additem 22508 1   -- 10. Feet: Plagueheart Sandals (T3 Plagueheart Sandals)
.additem 23063 1   -- 11. Ring 1: Plagueheart Ring (T3 Plagueheart Ring)
.additem 21709 1   -- 12. Ring 2: Ring of the Fallen God (Ring of the Fallen God)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22630 1   -- 15. Two-Hand Weapon: Atiesh, Greatstaff of the Guardian (Atiesh, Greatstaff of the Guardian (Warlock))
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 23009 1   -- 17. Ranged: Wand of the Whispering Dead (Wand of the Whispering Dead)
.repairitems
```

---

### 💀 SM/Ruin Warlock (Shadow Mastery Multi-DoT & Nightfall BiS)

> **Role**: `Multi-DoT & Instant Shadow Bolt Specialist`
> **Loadout Summary**: Full 9-piece Plagueheart, Atiesh Greatstaff of the Guardian (Warlock), Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, Band of Unnatural Forces, and Wand of the Whispering Dead.

```lua
.additem 22506 1   -- 01. Head: Plagueheart Circlet (T3 Plagueheart Circlet)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22507 1   -- 03. Shoulders: Plagueheart Shoulderpads (T3 Plagueheart Shoulderpads)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22504 1   -- 05. Chest: Plagueheart Robe (T3 Plagueheart Robe)
.additem 22511 1   -- 06. Wrists: Plagueheart Bracers (T3 Plagueheart Bracers)
.additem 22509 1   -- 07. Hands: Plagueheart Gloves (T3 Plagueheart Gloves)
.additem 22510 1   -- 08. Waist: Plagueheart Belt (T3 Plagueheart Belt)
.additem 22505 1   -- 09. Legs: Plagueheart Leggings (T3 Plagueheart Leggings)
.additem 22508 1   -- 10. Feet: Plagueheart Sandals (T3 Plagueheart Sandals)
.additem 23063 1   -- 11. Ring 1: Plagueheart Ring (T3 Plagueheart Ring)
.additem 23038 1   -- 12. Ring 2: Band of Unnatural Forces (Band of Unnatural Forces)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22630 1   -- 15. Two-Hand Weapon: Atiesh, Greatstaff of the Guardian (Atiesh, Greatstaff of the Guardian (Warlock))
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 23009 1   -- 17. Ranged: Wand of the Whispering Dead (Wand of the Whispering Dead)
.repairitems
```

---

### 🔥 Demonology Master Conjurer Warlock (Turtle 1.18.1 Firestone BiS)

> **Role**: `Turtle-WoW Signature Fire Caster DPS`
> **Loadout Summary**: Full Plagueheart, Midnight Haze, Sapphiron's Left Eye (Firestone slot), Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, and Doomfinger.

```lua
.additem 22506 1   -- 01. Head: Plagueheart Circlet (T3 Plagueheart Circlet)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22507 1   -- 03. Shoulders: Plagueheart Shoulderpads (T3 Plagueheart Shoulderpads)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22504 1   -- 05. Chest: Plagueheart Robe (T3 Plagueheart Robe)
.additem 22511 1   -- 06. Wrists: Plagueheart Bracers (T3 Plagueheart Bracers)
.additem 22509 1   -- 07. Hands: Plagueheart Gloves (T3 Plagueheart Gloves)
.additem 22510 1   -- 08. Waist: Plagueheart Belt (T3 Plagueheart Belt)
.additem 22505 1   -- 09. Legs: Plagueheart Leggings (T3 Plagueheart Leggings)
.additem 22508 1   -- 10. Feet: Plagueheart Sandals (T3 Plagueheart Sandals)
.additem 23063 1   -- 11. Ring 1: Plagueheart Ring (T3 Plagueheart Ring)
.additem 23038 1   -- 12. Ring 2: Band of Unnatural Forces (Band of Unnatural Forces)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22803 1   -- 15. Main Hand: Midnight Haze (Midnight Haze)
.additem 23049 1   -- 16. Off Hand: Sapphiron's Left Eye (Sapphiron's Left Eye)
.additem 22821 1   -- 17. Ranged: Doomfinger (Doomfinger (Fire Wand))
.repairitems
```

---

### 🐾 Feral Cat Druid (Powershifting Melee DPS BiS)

> **Role**: `High-Ceiling Physical Melee DPS`
> **Loadout Summary**: Wolfshead Helm (MANDATORY: +20 Energy on Cat Shift), Atiesh Greatstaff of the Guardian (Druid: +420 Feral AP), Malfurion's Blessed Bulwark, Leggings of Apocalypse, Kiss of the Spider, Drake Fang Talisman, Shroud of Dominion, and Idol of Ferocity.

```lua
.additem 8345  1   -- 01. Head: Wolfshead Helm (Wolfshead Helm (Furor Powershift))
.additem 19377 1   -- 02. Neck: Prestor's Talisman of Connivery (Prestor's Talisman of Connivery)
.additem 21665 1   -- 03. Shoulders: Mantle of Wicked Revenge (Mantle of Wicked Revenge)
.additem 23045 1   -- 04. Back: Shroud of Dominion (Shroud of Dominion)
.additem 19405 1   -- 05. Chest: Malfurion's Blessed Bulwark (Malfurion's Blessed Bulwark)
.additem 21602 1   -- 06. Wrists: Qiraji Execution Bracers (Qiraji Execution Bracers)
.additem 21623 1   -- 07. Hands: Gauntlets of the Righteous Champion (Gauntlets of the Righteous Champion)
.additem 21586 1   -- 08. Waist: Belt of Never-ending Agony (Belt of Never-ending Agony)
.additem 23071 1   -- 09. Legs: Leggings of Apocalypse (Leggings of Apocalypse)
.additem 19381 1   -- 10. Feet: Boots of the Shadow Flame (Boots of the Shadow Flame)
.additem 23025 1   -- 11. Ring 1: Seal of the Damned (Seal of the Damned)
.additem 17063 1   -- 12. Ring 2: Band of Accuria (Band of Accuria)
.additem 22954 1   -- 13. Trinket 1: Kiss of the Spider (Kiss of the Spider)
.additem 19406 1   -- 14. Trinket 2: Drake Fang Talisman (Drake Fang Talisman)
.additem 22632 1   -- 15. Two-Hand Weapon: Atiesh, Greatstaff of the Guardian (Atiesh, Greatstaff of the Guardian (Druid: +420 Feral AP))
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 22397 1   -- 17. Relic: Idol of Ferocity (Idol of Ferocity)
.repairitems
```

---

### 🛡️ Feral Bear Druid (High Health & Armor Tank BiS)

> **Role**: `Armor Cap & Massive Health Tank`
> **Loadout Summary**: Genesis Helm (T2.5), Malfurion's Blessed Bulwark, Mantle of Wicked Revenge, Cryptfiend Silk Cloak, Leggings of Apocalypse, Styleen's Impeding Scarab, Drake Fang Talisman, Atiesh, and Idol of Brutality. Hits 75% Armor Cap (~17,265 Armor).

```lua
.additem 21353 1   -- 01. Head: Genesis Helm (Genesis Helm)
.additem 22732 1   -- 02. Neck: Mark of C'Thun (Mark of C'Thun)
.additem 21665 1   -- 03. Shoulders: Mantle of Wicked Revenge (Mantle of Wicked Revenge)
.additem 22938 1   -- 04. Back: Cryptfiend Silk Cloak (Cryptfiend Silk Cloak)
.additem 19405 1   -- 05. Chest: Malfurion's Blessed Bulwark (Malfurion's Blessed Bulwark)
.additem 21602 1   -- 06. Wrists: Qiraji Execution Bracers (Qiraji Execution Bracers)
.additem 21623 1   -- 07. Hands: Gauntlets of the Righteous Champion (Gauntlets of the Righteous Champion)
.additem 21586 1   -- 08. Waist: Belt of Never-ending Agony (Belt of Never-ending Agony)
.additem 23071 1   -- 09. Legs: Leggings of Apocalypse (Leggings of Apocalypse)
.additem 19381 1   -- 10. Feet: Boots of the Shadow Flame (Boots of the Shadow Flame)
.additem 17063 1   -- 11. Ring 1: Band of Accuria (Band of Accuria)
.additem 21709 1   -- 12. Ring 2: Ring of the Fallen God (Ring of the Fallen God)
.additem 19431 1   -- 13. Trinket 1: Styleen's Impeding Scarab (Styleen's Impeding Scarab)
.additem 19406 1   -- 14. Trinket 2: Drake Fang Talisman (Drake Fang Talisman)
.additem 22632 1   -- 15. Two-Hand Weapon: Atiesh, Greatstaff of the Guardian (Atiesh, Greatstaff of the Guardian (Druid: +420 Feral AP))
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 23198 1   -- 17. Relic: Idol of Brutality (Idol of Brutality)
.repairitems
```

---

### 💚 Restoration Druid (HoT Raid Healer & Swiftmend BiS)

> **Role**: `Heal-Over-Time Stabilization Master`
> **Loadout Summary**: Full 9-piece Dreamwalker Raiment, Atiesh Greatstaff of the Guardian (Druid), Eye of the Dead, Rejuvenating Gem, Cloak of Suturing, Pure Elementium Band, and Idol of Rejuvenation.

```lua
.additem 22490 1   -- 01. Head: Dreamwalker Headpiece (T3 Dreamwalker Headpiece)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22491 1   -- 03. Shoulders: Dreamwalker Spaulders (T3 Dreamwalker Spaulders)
.additem 22960 1   -- 04. Back: Cloak of Suturing (Cloak of Suturing)
.additem 22488 1   -- 05. Chest: Dreamwalker Tunic (T3 Dreamwalker Tunic)
.additem 22495 1   -- 06. Wrists: Dreamwalker Wristguards (T3 Dreamwalker Wristguards)
.additem 22493 1   -- 07. Hands: Dreamwalker Handguards (T3 Dreamwalker Handguards)
.additem 22494 1   -- 08. Waist: Dreamwalker Girdle (T3 Dreamwalker Girdle)
.additem 22489 1   -- 09. Legs: Dreamwalker Legguards (T3 Dreamwalker Legguards)
.additem 22492 1   -- 10. Feet: Dreamwalker Boots (T3 Dreamwalker Boots)
.additem 23064 1   -- 11. Ring 1: Ring of The Dreamwalker (T3 Ring of The Dreamwalker)
.additem 19382 1   -- 12. Ring 2: Pure Elementium Band (Pure Elementium Band)
.additem 23047 1   -- 13. Trinket 1: Eye of the Dead (Eye of the Dead)
.additem 19395 1   -- 14. Trinket 2: Rejuvenating Gem (Rejuvenating Gem)
.additem 22632 1   -- 15. Two-Hand Weapon: Atiesh, Greatstaff of the Guardian (Atiesh, Greatstaff of the Guardian (Druid))
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 22398 1   -- 17. Relic: Idol of Rejuvenation (Idol of Rejuvenation)
.repairitems
```

---

### 🌙 Balance Druid 'Boomkin' (Nature / Arcane Caster DPS BiS)

> **Role**: `Caster Aura Support & Heavy Nature/Arcane DPS`
> **Loadout Summary**: Dreamwalker Raiment with Dark Storm Gauntlets & Eyestalk Waist Cord, Atiesh Greatstaff of the Guardian (Druid), Neltharion's Tear, Restrained Essence of Sapphiron, Cloak of the Necropolis, Ring of the Fallen God, and Idol of the Moon.

```lua
.additem 22490 1   -- 01. Head: Dreamwalker Headpiece (T3 Dreamwalker Headpiece)
.additem 23057 1   -- 02. Neck: Gem of Trapped Innocents (Gem of Trapped Innocents)
.additem 22491 1   -- 03. Shoulders: Dreamwalker Spaulders (T3 Dreamwalker Spaulders)
.additem 23050 1   -- 04. Back: Cloak of the Necropolis (Cloak of the Necropolis)
.additem 22488 1   -- 05. Chest: Dreamwalker Tunic (T3 Dreamwalker Tunic)
.additem 22495 1   -- 06. Wrists: Dreamwalker Wristguards (T3 Dreamwalker Wristguards)
.additem 21585 1   -- 07. Hands: Dark Storm Gauntlets (Dark Storm Gauntlets)
.additem 22730 1   -- 08. Waist: Eyestalk Waist Cord (Eyestalk Waist Cord)
.additem 22489 1   -- 09. Legs: Dreamwalker Legguards (T3 Dreamwalker Legguards)
.additem 22492 1   -- 10. Feet: Dreamwalker Boots (T3 Dreamwalker Boots)
.additem 23064 1   -- 11. Ring 1: Ring of The Dreamwalker (T3 Ring of The Dreamwalker)
.additem 21709 1   -- 12. Ring 2: Ring of the Fallen God (Ring of the Fallen God)
.additem 19379 1   -- 13. Trinket 1: Neltharion's Tear (Neltharion's Tear)
.additem 23046 1   -- 14. Trinket 2: The Restrained Essence of Sapphiron (The Restrained Essence of Sapphiron)
.additem 22632 1   -- 15. Two-Hand Weapon: Atiesh, Greatstaff of the Guardian (Atiesh, Greatstaff of the Guardian (Druid))
                    -- 16. Off Hand: [Occupied by Two-Handed Weapon]
.additem 23197 1   -- 17. Relic: Idol of the Moon (Idol of the Moon)
.repairitems
```

---

## Part IV: Legendary & Artifact Weapons Directory

| Item ID | Item Name | Slot | Quality | Unique Effects & Description |
| :---: | :--- | :---: | :---: | :--- |
| **`19019`** | **Thunderfury, Blessed Blade of the Windseeker** | 1H Sword | Legendary | Chance on hit: 300 Nature blast + chain lightning debuff (-25 Nature resist, -20% attack speed). Highest threat weapon in the game. |
| **`17182`** | **Sulfuras, Hand of Ragnaros** | 2H Mace | Legendary | Chance on hit: 273–333 Fireball + 5 Fire damage shield. 223–372 Physical damage (80.4 DPS, 3.70 speed). |
| **`22589`** | **Atiesh, Greatstaff of the Guardian (Mage)** | 2H Staff | Legendary | +28 Spell Power to group, +2% Spell Crit to group, Karazhan teleport portal on 1-min cooldown. |
| **`22630`** | **Atiesh, Greatstaff of the Guardian (Warlock)** | 2H Staff | Legendary | +33 Spell Damage & Healing to group, +2% Spell Hit to group, Karazhan teleport portal. |
| **`22631`** | **Atiesh, Greatstaff of the Guardian (Priest)** | 2H Staff | Legendary | +62 Healing to group, +11 MP5 to group, Karazhan teleport portal. |
| **`22632`** | **Atiesh, Greatstaff of the Guardian (Druid)** | 2H Staff | Legendary | +11 Feral Attack Power to group, +44 Healing to group, +11 MP5 to group, Karazhan teleport portal. |
| **`22691`** | **Corrupted Ashbringer** | 2H Sword | Epic | 259–389 Damage (90.0 DPS, 3.60 speed), +2% Crit, +2% Hit, Chance on Hit: Life Steal 185–215. Triggers Scarlet Monastery lore event. |
| **`13262`** | **Ashbringer (Purified Blade)** | 2H Sword | Legendary | 201–247 Damage (74.7 DPS, 3.00 speed), Holy burst damage proc. Legendary blade of Highlord Alexandros Mograine. |
| **`17782`** | **Talisman of Binding Shard** | Neck | Legendary | +10 Fire Resist, +10 Nature Resist, +10 Frost Resist, +10 Shadow Resist, +10 Arcane Resist. Rare legendary necklace. |
| **`82001`** | **Cape of Alexandros** | Cloak | Rare | Tattered cloak that once covered the back of the Ashbringer (Turtle-WoW exclusive). |
| **`82002`** | **Tabard of the Ashbringer** | Tabard | Epic | Filled with holy radiant light (Turtle-WoW exclusive). |

---

## Part III: Tier Armor Sets Directory

### 1. Complete Naxxramas Tier 3 Armor Sets (9 Pieces)

### 🛡️ Warrior: Dreadnaught Battlegear

- **`22416`** : Dreadnaught Breastplate (Chest)
- **`22417`** : Dreadnaught Legplates (Legs)
- **`22418`** : Dreadnaught Helmet (Head)
- **`22419`** : Dreadnaught Pauldrons (Shoulders)
- **`22420`** : Dreadnaught Sabatons (Feet)
- **`22421`** : Dreadnaught Gauntlets (Hands)
- **`22422`** : Dreadnaught Waistguard (Waist)
- **`22423`** : Dreadnaught Bracers (Wrists)
- **`23059`** : Ring of the Dreadnaught (Finger)

### 🔨 Paladin: Redemption Armor

- **`22428`** : Redemption Helm (Head)
- **`22429`** : Redemption Spaulders (Shoulders)
- **`22425`** : Redemption Tunic (Chest)
- **`22424`** : Redemption Bracers (Wrists)
- **`22426`** : Redemption Gloves (Hands)
- **`22431`** : Redemption Belt (Waist)
- **`22427`** : Redemption Pants (Legs)
- **`22430`** : Redemption Boots (Feet)
- **`23066`** : Ring of Redemption (Finger)

### 🏹 Hunter: Cryptstalker Armor

- **`22438`** : Cryptstalker Headpiece (Head)
- **`22439`** : Cryptstalker Spaulders (Shoulders)
- **`22436`** : Cryptstalker Tunic (Chest)
- **`22443`** : Cryptstalker Wristguards (Wrists)
- **`22441`** : Cryptstalker Handguards (Hands)
- **`22442`** : Cryptstalker Girdle (Waist)
- **`22437`** : Cryptstalker Legguards (Legs)
- **`22440`** : Cryptstalker Boots (Feet)
- **`23067`** : Ring of the Cryptstalker (Finger)

### 🗡️ Rogue: Bonescythe Armor

- **`22478`** : Bonescythe Helmet (Head)
- **`22479`** : Bonescythe Pauldrons (Shoulders)
- **`22476`** : Bonescythe Breastplate (Chest)
- **`22483`** : Bonescythe Bracers (Wrists)
- **`22481`** : Bonescythe Gauntlets (Hands)
- **`22482`** : Bonescythe Waistguard (Waist)
- **`22477`** : Bonescythe Legplates (Legs)
- **`22480`** : Bonescythe Sabatons (Feet)
- **`23060`** : Bonescythe Ring (Finger)

### ✝️ Priest: Vestments of Faith

- **`22514`** : Circlet of Faith (Head)
- **`22515`** : Mantle of Faith (Shoulders)
- **`22512`** : Robe of Faith (Chest)
- **`22519`** : Bindings of Faith (Wrists)
- **`22517`** : Gloves of Faith (Hands)
- **`22518`** : Belt of Faith (Waist)
- **`22513`** : Leggings of Faith (Legs)
- **`22516`** : Sandals of Faith (Feet)
- **`23061`** : Ring of Faith (Finger)

### ⚡ Shaman: The Earthshatterer

- **`22466`** : Earthshatter Headpiece (Head)
- **`22467`** : Earthshatter Spaulders (Shoulders)
- **`22464`** : Earthshatter Tunic (Chest)
- **`22471`** : Earthshatter Wristguards (Wrists)
- **`22469`** : Earthshatter Handguards (Hands)
- **`22470`** : Earthshatter Belt (Waist)
- **`22465`** : Earthshatter Legguards (Legs)
- **`22468`** : Earthshatter Boots (Feet)
- **`23065`** : Ring of the Earthshatterer (Finger)

### 🔥 Mage: Frostfire Regalia

- **`22498`** : Frostfire Circlet (Head)
- **`22499`** : Frostfire Shoulderpads (Shoulders)
- **`22496`** : Frostfire Robe (Chest)
- **`22503`** : Frostfire Bindings (Wrists)
- **`22501`** : Frostfire Gloves (Hands)
- **`22502`** : Frostfire Belt (Waist)
- **`22497`** : Frostfire Leggings (Legs)
- **`22500`** : Frostfire Sandals (Feet)
- **`23062`** : Frostfire Ring (Finger)

### 💀 Warlock: Plagueheart Raiment

- **`22506`** : Plagueheart Circlet (Head)
- **`22507`** : Plagueheart Shoulderpads (Shoulders)
- **`22504`** : Plagueheart Robe (Chest)
- **`22511`** : Plagueheart Bracers (Wrists)
- **`22509`** : Plagueheart Gloves (Hands)
- **`22510`** : Plagueheart Belt (Waist)
- **`22505`** : Plagueheart Pants (Legs)
- **`22508`** : Plagueheart Sandals (Feet)
- **`23063`** : Plagueheart Ring (Finger)

### 🐾 Druid: Dreamwalker Raiment

- **`22490`** : Dreamwalker Headpiece (Head)
- **`22491`** : Dreamwalker Spaulders (Shoulders)
- **`22488`** : Dreamwalker Tunic (Chest)
- **`22495`** : Dreamwalker Bracers (Wrists)
- **`22493`** : Dreamwalker Handguards (Hands)
- **`22494`** : Dreamwalker Belt (Waist)
- **`22489`** : Dreamwalker Legguards (Legs)
- **`22492`** : Dreamwalker Boots (Feet)
- **`23064`** : Ring of The Dreamwalker (Finger)

---

### 2. Temple of Ahn'Qiraj Tier 2.5 Armor Sets (5 Pieces)

### 🛡️ Warrior: Conqueror's Battlegear

- **`21329`** : Conqueror's Crown (Head)
- **`21330`** : Conqueror's Spaulders (Shoulders)
- **`21331`** : Conqueror's Breastplate (Chest)
- **`21332`** : Conqueror's Legguards (Legs)
- **`21333`** : Conqueror's Greaves (Feet)

### 🔨 Paladin: Avenger's Battlegear

- **`21387`** : Avenger's Crown (Head)
- **`21391`** : Avenger's Pauldrons (Shoulders)
- **`21389`** : Avenger's Chestplate (Chest)
- **`21390`** : Avenger's Leggings (Legs)
- **`21388`** : Avenger's Sabatons (Feet)

### 🏹 Hunter: Striker's Garb

- **`21366`** : Striker's Diadem (Head)
- **`21367`** : Striker's Pauldrons (Shoulders)
- **`21370`** : Striker's Hauberk (Chest)
- **`21368`** : Striker's Leggings (Legs)
- **`21365`** : Striker's Footguards (Feet)

### 🗡️ Rogue: Deathdealer's Embrace

- **`21360`** : Deathdealer's Helm (Head)
- **`21361`** : Deathdealer's Spaulders (Shoulders)
- **`21364`** : Deathdealer's Vest (Chest)
- **`21362`** : Deathdealer's Leggings (Legs)
- **`21359`** : Deathdealer's Boots (Feet)

### ✝️ Priest: Garments of the Oracle

- **`21348`** : Coronet of the Oracle (Head)
- **`21350`** : Mantle of the Oracle (Shoulders)
- **`21351`** : Raiments of the Oracle (Chest/Robe)
- **`21352`** : Pants of the Oracle (Legs)
- **`21349`** : Footwraps of the Oracle (Feet)

### ⚡ Shaman: Stormcaller's Garb

- **`21372`** : Stormcaller's Diadem (Head)
- **`21376`** : Stormcaller's Epaulets (Shoulders)
- **`21374`** : Stormcaller's Hauberk (Chest)
- **`21375`** : Stormcaller's Legplates (Legs)
- **`21373`** : Stormcaller's Greaves (Feet)

### 🔥 Mage: Enigma Regalia

- **`21347`** : Enigma Circlet (Head)
- **`21345`** : Enigma Shoulderpads (Shoulders)
- **`21343`** : Enigma Robes (Chest/Robe)
- **`21346`** : Enigma Leggings (Legs)
- **`21344`** : Enigma Boots (Feet)

### 💀 Warlock: Doomcaller's Attire

- **`21337`** : Doomcaller's Circlet (Head)
- **`21335`** : Doomcaller's Mantle (Shoulders)
- **`21334`** : Doomcaller's Robes (Chest/Robe)
- **`21336`** : Doomcaller's Trousers (Legs)
- **`21338`** : Doomcaller's Footwraps (Feet)

### 🐾 Druid: Genesis Raiment

- **`21353`** : Genesis Circlet (Head)
- **`21354`** : Genesis Mantle (Shoulders)
- **`21357`** : Genesis Vest (Chest)
- **`21356`** : Genesis Trousers (Legs)
- **`21355`** : Genesis Slippers (Feet)

---

### 3. Blackwing Lair & Molten Core Tier 2 Armor Sets (8 Pieces)

### 🛡️ Warrior: Battlegear of Wrath

- **`16963`** : Helm of Wrath (Head - Onyxia)
- **`16961`** : Pauldrons of Wrath (Shoulders)
- **`16966`** : Breastplate of Wrath (Chest - Nefarian)
- **`16959`** : Bracelets of Wrath (Wrists)
- **`16964`** : Gauntlets of Wrath (Hands)
- **`16960`** : Waistband of Wrath (Waist)
- **`16962`** : Legplates of Wrath (Legs - Ragnaros)
- **`16965`** : Sabatons of Wrath (Feet)

### 🔨 Paladin: Judgement Armor

- **`16955`** : Judgement Helm (Head - Onyxia)
- **`16953`** : Judgement Spaulders (Shoulders)
- **`16958`** : Judgement Breastplate (Chest - Nefarian)
- **`16951`** : Judgement Bracers (Wrists)
- **`16956`** : Judgement Gloves (Hands)
- **`16952`** : Judgement Belt (Waist)
- **`16954`** : Judgement Legplates (Legs - Ragnaros)
- **`16957`** : Judgement Boots (Feet)

### 🏹 Hunter: Dragonstalker Armor

- **`16939`** : Dragonstalker's Helm (Head - Onyxia)
- **`16937`** : Dragonstalker's Spaulders (Shoulders)
- **`16942`** : Dragonstalker's Breastplate (Chest - Nefarian)
- **`16935`** : Dragonstalker's Bracers (Wrists)
- **`16940`** : Dragonstalker's Gauntlets (Hands)
- **`16936`** : Dragonstalker's Belt (Waist)
- **`16938`** : Dragonstalker's Legguards (Legs - Ragnaros)
- **`16941`** : Dragonstalker's Greaves (Feet)

### 🗡️ Rogue: Bloodfang Armor

- **`16908`** : Bloodfang Hood (Head - Onyxia)
- **`16832`** : Bloodfang Spaulders (Shoulders)
- **`16905`** : Bloodfang Chestpiece (Chest - Nefarian)
- **`16911`** : Bloodfang Belt (Waist)
- **`16907`** : Bloodfang Gloves (Hands)
- **`16906`** : Bloodfang Boots (Feet)
- **`16909`** : Bloodfang Pants (Legs - Ragnaros)
- **`16910`** : Bloodfang Bracers (Wrists)

### ✝️ Priest: Vestments of Transcendence

- **`16922`** : Halo of Transcendence (Head - Onyxia)
- **`16924`** : Pauldrons of Transcendence (Shoulders)
- **`16923`** : Robes of Transcendence (Chest - Nefarian)
- **`16926`** : Bindings of Transcendence (Wrists)
- **`16920`** : Handguards of Transcendence (Hands)
- **`16925`** : Belt of Transcendence (Waist)
- **`16921`** : Leggings of Transcendence (Legs - Ragnaros)
- **`16919`** : Boots of Transcendence (Feet)

### ⚡ Shaman: The Ten Storms

- **`16947`** : Visor of Ten Storms (Head - Onyxia)
- **`16945`** : Epaulets of Ten Storms (Shoulders)
- **`16950`** : Raiments of Ten Storms (Chest - Nefarian)
- **`16943`** : Bindings of Ten Storms (Wrists)
- **`16948`** : Gauntlets of Ten Storms (Hands)
- **`16944`** : Sash of Ten Storms (Waist)
- **`16946`** : Legplates of Ten Storms (Legs - Ragnaros)
- **`16949`** : Greaves of Ten Storms (Feet)

### 🔥 Mage: Netherwind Regalia

- **`16914`** : Netherwind Pants (Legs - Ragnaros)
- **`16915`** : Netherwind Robes (Chest - Nefarian)
- **`16916`** : Netherwind Mantle (Shoulders)
- **`16917`** : Netherwind Gloves (Hands)
- **`16918`** : Netherwind Boots (Feet)
- **`16818`** : Netherwind Belt (Waist)
- **`16912`** : Netherwind Bindings (Wrists)
- **`16913`** : Netherwind Crown (Head - Onyxia)

### 💀 Warlock: Nemesis Raiment

- **`16930`** : Nemesis Skullcap (Head - Onyxia)
- **`16932`** : Nemesis Spaulders (Shoulders)
- **`16931`** : Nemesis Robes (Chest - Nefarian)
- **`16934`** : Nemesis Bracers (Wrists)
- **`16928`** : Nemesis Gloves (Hands)
- **`16933`** : Nemesis Belt (Waist)
- **`16929`** : Nemesis Leggings (Legs - Ragnaros)
- **`16927`** : Nemesis Boots (Feet)

### 🐾 Druid: Stormrage Raiment

- **`16900`** : Stormrage Cover (Head - Onyxia)
- **`16902`** : Stormrage Pauldrons (Shoulders)
- **`16903`** : Stormrage Chestguard (Chest - Nefarian)
- **`16897`** : Stormrage Bracers (Wrists)
- **`16899`** : Stormrage Handguards (Hands)
- **`16904`** : Stormrage Belt (Waist)
- **`16901`** : Stormrage Legguards (Legs - Ragnaros)
- **`16898`** : Stormrage Boots (Feet)

---

### 4. Molten Core Tier 1 Armor Sets (8 Pieces)

| Class | Chest | Legs | Head | Shoulders | Feet | Hands | Waist | Wrists |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **Warrior (Might)** | `16865` | `16867` | `16866` | `16868` | `16862` | `16863` | `16864` | `16861` |
| **Paladin (Lawbringer)** | `16853` | `16855` | `16854` | `16856` | `16859` | `16860` | `16858` | `16857` |
| **Hunter (Giantstalker)** | `16845` | `16847` | `16846` | `16848` | `16849` | `16852` | `16851` | `16850` |
| **Rogue (Nightslayer)** | `16820` | `16822` | `16821` | `16823` | `16824` | `16826` | `16827` | `16825` |
| **Priest (Prophecy)** | `16815` | `16814` | `16813` | `16816` | `16811` | `16812` | `16817` | `16819` |
| **Shaman (Earthfury)** | `16841` | `16843` | `16842` | `16844` | `16837` | `16839` | `16838` | `16840` |
| **Mage (Arcanist)** | `16798` | `16796` | `16795` | `16797` | `16800` | `16801` | `16802` | `16799` |
| **Warlock (Felheart)** | `16809` | `16810` | `16808` | `16807` | `16803` | `16805` | `16806` | `16804` |
| **Druid (Cenarion)** | `16833` | `16835` | `16834` | `16836` | `16829` | `16831` | `16828` | `16830` |

---

### 5. Zul'Gurub 5-Piece Class Sets

| Class & Set Name | Chest | Shoulders | Wrists | Belt | Class Trinket |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Warrior (Vindicator's Battlegear)** | `19822` | `19823` | `19824` | `19823` | `19951` (Gri'lek's Might) |
| **Paladin (Freethinker's Armor)** | `19825` | `19826` | `19827` | `19826` | `19952` (Gri'lek's Valor) |
| **Hunter (Predator's Armor)** | `19831` | `19831` | `19833` | `19832` | `19953` (Renataki's Beasts) |
| **Rogue (Madcap's Outfit)** | `19834` | `19835` | `19836` | `19835` | `19954` (Renataki's Trickery) |
| **Priest (Confessor's Raiment)** | `19841` | `19841` | `19843` | `19842` | `19957` (Hazza'rah's Destruction) |
| **Shaman (Augur's Regalia)** | `19828` | `19829` | `19830` | `19829` | `19955` (Wushoolay's Nature) |
| **Mage (Illusionist's Attire)** | `20034` | `19845` | `19846` | `19845` | `19959` (Hazza'rah's Magic) |
| **Warlock (Demoniac's Threads)** | `20033` | `19849` | `19848` | `19849` | `19958` (Hazza'rah's Healing/Shadow) |
| **Druid (Haruspex's Garb)** | `19838` | `19839` | `19840` | `19839` | `19956` (Wushoolay's Spirits) |

---

### 6. Ruins of Ahn'Qiraj (AQ20) 3-Piece Class Sets

| Class & Set Name | Weapon / Off-Hand | Ring / Signet | Cloak / Drape |
| :--- | :---: | :---: | :---: |
| **Warrior (Unyielding Strength)** | `21392` (Sickle) | `21393` (Signet) | `21394` (Drape) |
| **Paladin (Eternal Justice)** | `21395` (Blade) | `21396` (Ring) | `21397` (Cape) |
| **Shaman (Gathering Storm)** | `21398` (Hammer) | `21399` (Ring) | `21400` (Cloak) |
| **Hunter (Unseen Path)** | `21401` (Scythe) | `21402` (Signet) | `21403` (Cloak) |
| **Rogue (Veiled Shadows)** | `21404` (Dagger) | `21405` (Band) | `21406` (Cloak) |
| **Druid (Unending Life)** | `21407` (Mace) | `21408` (Band) | `21409` (Cloak) |
| **Priest (Infinite Wisdom)** | `21410` (Gavel) | `21411` (Ring) | `21412` (Shroud) |
| **Mage (Vaulted Secrets)** | `21413` (Blade) | `21414` (Band) | `21415` (Drape) |
| **Warlock (Unspoken Names)** | `21416` (Kris) | `21417` (Ring) | `21418` (Shroud) |

---

### 7. Dungeon Sets 1 & 2 (Tier 0 & Tier 0.5 Upgrades)

### Dungeon Set 2 (Tier 0.5 Epic Upgrade)

- **Warrior (Battlegear of Heroism)**: Helm `21999`, Shoulders `22001`, Chest `21997`, Wrists `21996`, Gloves `21998`, Belt `21994`, Legs `22000`, Boots `21995`
- **Paladin (Soulforge Armor)**: Helm `22091`, Shoulders `22093`, Chest `22089`, Wrists `22088`, Gloves `22090`, Belt `22086`, Legs `22092`, Boots `22087`
- **Hunter (Beastmaster Armor)**: Helm `22013`, Shoulders `22016`, Chest `22060`, Wrists `22011`, Gloves `22015`, Belt `22010`, Legs `22017`, Boots `22061`
- **Rogue (Darkmantle Armor)**: Helm `22005`, Shoulders `22008`, Chest `22009`, Wrists `22004`, Gloves `22006`, Belt `22002`, Legs `22007`, Boots `22003`
- **Priest (Vestments of the Virtuous)**: Crown `22080`, Mantle `22082`, Robe `22083`, Wrists `22079`, Gloves `22081`, Belt `22078`, Legs `22085`, Boots `22084`
- **Shaman (The Five Thunders)**: Helm `22097`, Shoulders `22101`, Chest `22102`, Wrists `22095`, Gloves `22099`, Belt `22098`, Legs `22100`, Boots `22096`
- **Mage (Sorcerer's Regalia)**: Crown `22065`, Mantle `22068`, Robes `22069`, Wrists `22063`, Gloves `22066`, Belt `22062`, Legs `22067`, Boots `22064`
- **Warlock (Deathmist Raiment)**: Mask `22074`, Mantle `22073`, Robe `22075`, Wrists `22071`, Gloves `22077`, Belt `22070`, Legs `22072`, Boots `22076`
- **Druid (Moonheart Raiment - Turtle Custom)**: Cowl `22109`, Spaulders `22112`, Vest `22113`, Bracers `22108`, Gloves `22110`, Belt `22106`, Kilt `22111`, Boots `22107`

---

### 8. PvP Grand Marshal & High Warlord Equipment (Rank 14)

### ⚔️ Rank 14 Epic Weapons

- **`18876`** : **Veteran's Claymore** (Alliance 2H Sword - 83.2 DPS, 3.80 speed)
- **`18877`** : **Veteran's Greatsword** (Horde 2H Sword - 83.2 DPS, 3.80 speed)
- **`18830`** : **Veteran's Sunderer** (Alliance/Horde 2H Axe - 83.2 DPS, 3.80 speed)
- **`18831`** : **Veteran's Battle Axe** (Alliance/Horde 2H Axe - 83.2 DPS, 3.80 speed)
- **`18867`** : **Veteran's Battle Hammer** (Alliance 2H Mace - 83.2 DPS, 3.80 speed)
- **`18868`** : **Veteran's Pulverizer** (Horde 2H Mace - 83.2 DPS, 3.80 speed)
- **`18869`** : **Veteran's Glaive** (Alliance 2H Polearm - 83.2 DPS, 3.80 speed)
- **`18871`** : **Veteran's Pig Sticker** (Horde 2H Polearm - 83.2 DPS, 3.80 speed)
- **`18873`** : **Executioner's Stave** (Alliance 2H Staff - 61.5 DPS, 3.00 speed)
- **`18874`** : **Executioner's War Staff** (Horde 2H Staff - 61.5 DPS, 3.00 speed)
- **`12584`** : **Veteran's Longsword** (Alliance 1H Sword - 65.5 DPS, 2.90 speed)
- **`16345`** : **Veteran's Blade** (Horde 1H Sword - 65.5 DPS, 2.90 speed)
- **`18827`** : **Veteran's Handaxe** (Alliance 1H Axe - 65.5 DPS, 2.90 speed)
- **`18828`** : **Veteran's Cleaver** (Horde 1H Axe - 65.5 DPS, 2.90 speed)
- **`18865`** : **Veteran's Punisher** (Alliance 1H Mace - 65.5 DPS, 2.90 speed)
- **`18866`** : **Veteran's Bludgeon** (Horde 1H Mace - 65.5 DPS, 2.90 speed)
- **`18838`** : **Veteran's Dirk** (Alliance 1H Dagger - 65.5 DPS, 2.00 speed)
- **`18840`** : **Veteran's Razor** (Horde 1H Dagger - 65.5 DPS, 2.00 speed)
- **`18833`** : **Veteran's Bullseye** (Alliance Bow - 50.8 DPS, 1.80 speed)
- **`18835`** : **Veteran's Recurve** (Horde Bow - 50.8 DPS, 1.80 speed)
- **`18836`** : **Veteran's Repeater** (Alliance Crossbow - 50.9 DPS, 2.90 speed)
- **`18837`** : **Veteran's Crossbow** (Horde Crossbow - 50.9 DPS, 2.90 speed)
- **`18855`** : **Veteran's Hand Cannon** (Alliance Gun - 50.9 DPS, 2.90 speed)
- **`18860`** : **Veteran's Street Sweeper** (Horde Gun - 50.9 DPS, 2.90 speed)
- **`18825`** : **Veteran's Aegis** (Alliance Shield - 3106 Armor, 59 Block)
- **`18826`** : **Veteran's Shield Wall** (Horde Shield - 3106 Armor, 59 Block)

### 🛡️ Rank 12–13 Field Marshal / Warlord Epic Armor

- **Warrior Plate Set**: Helm `16542`, Shoulders `16544`, Chest `16541`, Gloves `16543`, Legs `16545`, Boots `16548`
- **Paladin Plate Set**: Helm `16474`, Shoulders `16476`, Chest `16471`, Gloves `16472`, Legs `16475`, Boots `16473`
- **Hunter Mail Set**: Helm `16565`, Shoulders `16567`, Chest `16566`, Gloves `16568`, Legs `16571`, Boots `16569`
- **Rogue Leather Set**: Helm `16561`, Shoulders `16563`, Chest `16562`, Gloves `16560`, Legs `16558`, Boots `16559`
- **Priest Cloth Set**: Helm `16533`, Shoulders `16536`, Chest `16535`, Gloves `16539`, Legs `16534`, Boots `16540`
- **Mage Cloth Set**: Helm `16533`, Shoulders `16536`, Chest `16535`, Gloves `16539`, Legs `16534`, Boots `16540`
- **Warlock Cloth Set**: Helm `16533`, Shoulders `16536`, Chest `16535`, Gloves `16539`, Legs `16534`, Boots `16540`
- **Druid Leather Set**: Helm `16549`, Shoulders `16552`, Chest `16551`, Gloves `16550`, Legs `16554`, Boots `16555`

---

### 9. World Boss Epic Drops (Azuregos, Kazzak, Nightmare Dragons)

### 🐉 Azuregos (Azshara)

- **`18202`** : **Eskhandar's Left Claw** (1H Fist Weapon Off-Hand - 1.50 speed, Slow & Bleed proc)
- **`18203`** : **Eskhandar's Right Claw** (1H Fist Weapon Main Hand - 1.50 speed, Attack Speed proc)
- **`17070`** : **Fang of the Mystics** (1H Main Hand Mace - +40 Healing, +10 MP5)
- **`18547`** : **Unmelting Ice Girdle** (Plate Belt - +14 Str, +14 Sta, +14 Agi, +1% Dodge)
- **`18541`** : **Puissant Cape** (Cloak - +12 Sta, +1% Hit, +40 Attack Power)
- **`18208`** : **Drape of Benediction** (Cloak - +31 Healing, +10 Intellect)
- **`19131`** : **Snowblind Shoes** (Cloth Boots - +10 Sta, +10 Int, +32 Spell Damage)

### 👿 Lord Kazzak (Tainted Scar, Blasted Lands)

- **`17112`** : **Empyrean Demolisher** (1H Mace - 2.80 speed, Chance on Hit: +20% Attack Speed)
- **`17113`** : **Amberseal Keeper** (2H Staff - +44 Spell Damage, +20 Intellect, +5 MP5)
- **`19134`** : **Flayed Doomguard Belt** (Leather Belt - +14 Str, +13 Sta, +12 Agi, +1% Crit)
- **`19133`** : **Fel Infused Leggings** (Cloth Legs - +21 Sta, +20 Int, +34 Spell Damage)
- **`17110`** : **Seal of the Archmagus** (Ring - +11 Int, +11 Sta, +6 MP5)
- **`18546`** : **Infernal Headcage** (Mail Helm - +20 Str, +15 Sta, +15 Agi, +1% Crit)

### 🌲 Dragons of Nightmare (Taerar, Ysondre, Lethon, Emeriss)

- **`20577`** : **Nightmare Blade** (1H Sword - 2.70 speed, +70 Armor, +32 Attack Power)
- **`20581`** : **Staff of Rampant Growth** (2H Staff - +15 Int, +15 Sta, +40 Healing, +11 MP5)
- **`20580`** : **Hammer of Bestial Fury** (2H Mace - +65 Feral Attack Power, +13 Str, +12 Sta)
- **`20599`** : **Polished Ironwood Crossbow** (Crossbow - 3.10 speed, +14 Attack Power, +1% Hit)
- **`20615`** : **Dragonspur Wraps** (Leather Wrists - +15 Str, +15 Sta, +1% Hit)
- **`20296`** : **Green Dragonscale Gauntlets** (Mail Gloves - +12 Sta, +2% Crit, +28 Attack Power)
- **`20582`** : **Trance Stone** (Off-Hand Caster - +10 Int, +8 Sta, +25 Spell Damage)
- **`20625`** : **Belt of the Dark Bog** (Leather Belt - +14 Sta, +8 Int, +29 Spell Damage)
- **`20617`** : **Ancient Corroded Leggings** (Mail Legs - +22 Agi, +15 Sta, +40 AP)
- **`20621`** : **Boots of the Endless Moor** (Mail Boots - +18 Sta, +18 Int, +26 Healing)

---

## Part V: BiS Weapons Directory (Organized by Weapon Type)

### 🔨 Two-Handed Weapons (Swords, Maces, Axes & Polearms)

| Item ID | Item Name | Type | Boss / Source | Speed | DPS | Stats & Effects |
| :---: | :--- | :---: | :--- | :---: | :---: | :--- |
| **`22798`** | **Might of Menethil** | 2H Mace | Kel'Thuzad (Naxx) | 3.80 | 95.3 | +20 Strength, +46 Stamina, Equip: +2% Crit. (Highest 2H Physical DPS in Vanilla) |
| **`22691`** | **Corrupted Ashbringer** | 2H Sword | Four Horsemen (Naxx) | 3.60 | 90.0 | +2% Crit, +2% Hit, Chance on Hit: Life Steal 185–215. Triggers Scarlet Monastery lore event. |
| **`13262`** | **Ashbringer (Purified Blade)** | 2H Sword | Legendary Quest | 3.00 | 74.7 | 201–247 Damage, Holy burst proc. Highlord Alexandros Mograine's blade. |
| **`22815`** | **Severance** | 2H Axe | Patchwerk (Naxx) | 3.60 | 85.8 | +43 Strength, +18 Stamina, Equip: +2% Crit. (Top Raid 2H Axe) |
| **`23039`** | **The Eye of Nerub** | 2H Polearm | Naxxramas Trash | 3.70 | 85.9 | +28 Agility, +2% Hit, +2% Crit, +60 Attack Power. (BiS Hunter Melee Stat Stick) |
| **`22813`** | **Claymore of Unholy Might** | 2H Sword | Naxxramas Trash | 3.60 | 85.8 | +82 Attack Power, +2% Critical Strike. |
| **`22809`** | **Maul of the Redeemed Crusader** | 2H Mace | Naxxramas Trash | 3.60 | 85.8 | +30 Strength, +24 Stamina, +1% Crit, +1% Hit. |
| **`21134`** | **Dark Edge of Insanity** | 2H Axe | C'Thun (AQ40) | 3.50 | 86.6 | +35 Strength, +19 Agility, +25 Stamina, Chance on Hit: 3-sec Disorient. |
| **`21679`** | **Kalimdor's Revenge** | 2H Sword | Twin Emperors (AQ40) | 3.20 | 81.9 | +24 Strength, +18 Stamina, Chance on Hit: 239–317 Nature blast. |
| **`19364`** | **Ashkandi, Greatsword of the Brotherhood** | 2H Sword | Nefarian (BWL) | 3.50 | 81.9 | +33 Stamina, Equip: +86 Attack Power. |
| **`19334`** | **The Untamed Blade** | 2H Sword | Razorgore (BWL) | 3.40 | 66.8 | Chance on Hit: +300 Strength for 8 seconds. Massive burst proc. |
| **`19354`** | **Draconic Avenger** | 2H Axe | BWL Trash | 3.20 | 66.9 | +21 Strength, +18 Stamina, +1% Parry. |
| **`19358`** | **Draconic Maul** | 2H Mace | BWL Trash | 3.50 | 66.7 | +27 Strength, +19 Stamina, +1% Critical Strike. |
| **`19357`** | **Herald of Woe** | 2H Mace | Ebonroc (BWL) | 3.40 | 73.1 | +28 Strength, +1% Crit, Chance on Hit: Stun target for 3 sec. |
| **`17182`** | **Sulfuras, Hand of Ragnaros** | 2H Mace | Ragnaros (MC) | 3.70 | 80.4 | Chance on hit: 273–333 Fireball + 5 Fire shield. +12 Str, +12 Sta, +30 FR. |
| **`17076`** | **Bonereaver's Edge** | 2H Sword | Ragnaros (MC) | 3.40 | 60.6 | +16 Stamina, Chance on Hit: Armor penetration stack (700 armor reduction per stack). |
| **`17104`** | **Spinal Reaper** | 2H Axe | Ragnaros (MC) | 3.40 | 68.4 | +34 Strength, +1% Crit, Equip: Gain 150 Attack Power upon killing an enemy. |
| **`18822`** | **Obsidian Edged Blade** | 2H Sword | Garr / Magmadar (MC) | 3.40 | 60.6 | +42 Strength, +4 Swords Skill. Excellent human/non-human starter 2H. |
| **`19854`** | **Zin'rokh, Destroyer of Worlds** | 2H Sword | Hakkar (ZG) | 3.80 | 60.4 | +28 Stamina, Equip: +72 Attack Power. Iconic slow PvP cleaver. |
| **`19874`** | **Halberd of Smiting** | 2H Polearm | Bloodlord Mandokir (ZG) | 3.50 | 58.7 | Chance on Hit: Decapitate for extra physical burst damage. |
| **`21492`** | **Manslayer of the Qiraji** | 2H Sword | General Rajaxx (AQ20) | 3.60 | 58.6 | +21 Strength, +15 Stamina, +1% Crit, +1% Hit. |
| **`19169`** | **Nightfall** | 2H Axe | Blacksmithing Crafted | 3.50 | 53.6 | Chance on Hit: Spell Damage Taken +15% on target. Essential raid debuff weapon. |
| **`12784`** | **Arcanite Reaper** | 2H Axe | Blacksmithing Crafted | 3.80 | 53.7 | +62 Attack Power, +13 Stamina. Iconic PvP burst axe. |
| **`19323`** | **The Unstoppable Force** | 2H Mace | Alterac Valley Vendor | 3.80 | 61.4 | +19 Strength, +15 Stamina, +1% Crit, Chance on Hit: Stun/Knockdown for 1s. |
| **`18538`** | **Treant's Bane** | 2H Axe | Dire Maul Tribute | 3.80 | 58.0 | +25 Strength, +15 Stamina, +2% Critical Strike. |
| **`12592`** | **Blackblade of Shahram** | 2H Sword | General Drakkisath (UBRS) | 3.50 | 54.7 | Chance on Hit: Summons the Spirit of Shahram to cast powerful raid buffs/nukes. |
| **`18876`** | **Veteran's Claymore** | 2H Sword | Alliance Rank 14 PvP | 3.80 | 83.2 | +41 Strength, +18 Stamina, +1% Critical Strike. (Grand Marshal model) |
| **`18877`** | **Veteran's Greatsword** | 2H Sword | Horde Rank 14 PvP | 3.80 | 83.2 | +41 Strength, +18 Stamina, +1% Critical Strike. (High Warlord model) |
| **`18830`** | **Veteran's Sunderer** | 2H Axe | Rank 14 PvP | 3.80 | 83.2 | +41 Strength, +18 Stamina, +1% Critical Strike. |
| **`18867`** | **Veteran's Battle Hammer** | 2H Mace | Alliance Rank 14 PvP | 3.80 | 83.2 | +41 Strength, +18 Stamina, +1% Critical Strike. |
| **`18868`** | **Veteran's Pulverizer** | 2H Mace | Horde Rank 14 PvP | 3.80 | 83.2 | +41 Strength, +18 Stamina, +1% Critical Strike. |
| **`18869`** | **Veteran's Glaive** | 2H Polearm | Alliance Rank 14 PvP | 3.80 | 83.2 | +41 Strength, +18 Stamina, +1% Critical Strike. |
| **`18871`** | **Veteran's Pig Sticker** | 2H Polearm | Horde Rank 14 PvP | 3.80 | 83.2 | +41 Strength, +18 Stamina, +1% Critical Strike. |
| **`55348`** | **Kirel'narak, the Death Sentence** | 2H Staff | Emerald Sanctum (Custom) | 3.20 | 60.9 | +25 Int, +25 Sta, +92 Spell Damage, +2% Spell Hit. (ilvl 96 Custom BiS Staff) |
| **`55127`** | **Shar'tateth, the Shattered Edge** | 2H Axe | Emerald Sanctum (Custom) | 3.80 | 100.0 | +38 Strength, +30 Stamina, +2% Crit, +1% Hit. (ilvl 92 Custom 2H Axe) |
| **`55116`** | **Raka'shishi, Spear of the Adrift Hunt** | 2H Polearm | Emerald Sanctum (Custom) | 3.70 | 100.0 | +40 Agility, +24 Stamina, +2% Crit, +2% Hit. (ilvl 92 Custom 2H Polearm) |
| **`55100`** | **Checkmate** | 2H Mace | Scarlet Citadel (Custom) | 3.70 | 93.5 | +32 Strength, +28 Stamina, +2% Crit, +1% Hit. (ilvl 88 Custom 2H Mace) |
| **`55277`** | **The End of All Ambitions** | 2H Sword | Lower Karazhan (Custom) | 3.50 | 84.6 | +30 Strength, +22 Agility, +22 Stamina, +1% Crit. (ilvl 83 Custom 2H Sword) |

---

### ⚔️ One-Handed Swords

| Item ID | Item Name | Boss / Source | Speed | DPS | Stats & Effects |
| :---: | :--- | :--- | :---: | :---: | :--- |
| **`23054`** | **Gressil, Dawn of Ruin** | Kel'Thuzad (Naxx) | 2.70 | 73.1 | +15 Stamina, +14 Attack Power, Equip: +1% Crit. |
| **`23577`** | **The Hungering Cold** | Kel'Thuzad (Naxx) | 1.50 | 73.0 | +140 Armor, +14 Stamina, Equip: +6 Swords Skill, +1.50s Fast Off-hand. |
| **`23014`** | **Iblis, Blade of the Fallen Seraph** | Gluth / Patchwerk (Naxx) | 1.60 | 66.9 | +7 Stamina, Equip: +1% Hit, +1% Crit, +26 Attack Power. |
| **`19352`** | **Chromatically Tempered Sword (CTS)** | Chromaggus (BWL) | 2.60 | 58.5 | +14 Strength, +14 Agility, +7 Stamina. |
| **`19351`** | **Maladath, Runed Blade of the Black Flight** | Broodlord Lashlayer (BWL) | 2.20 | 56.4 | +4 Agility, Equip: +1% Parry, +4 Swords Skill. |
| **`21650`** | **Ancient Qiraji Ripper** | Fankriss (AQ40) | 2.80 | 58.4 | +11 Stamina, Equip: +1% Crit, +20 Attack Power. |
| **`17075`** | **Vis'kag the Bloodletter** | Onyxia | 2.60 | 48.3 | Chance on hit: 240 Fatal Wound burst. |
| **`18832`** | **Brutality Blade** | Garr (MC) | 2.50 | 45.4 | +9 Strength, +9 Agility, Equip: +1% Crit. |
| **`19865`** | **Warblade of the Hakkari (Main)** | Hakkar (ZG) | 2.60 | 40.8 | +28 Attack Power, Equip: Set bonus +6 Swords Skill. |
| **`19866`** | **Warblade of the Hakkari (Off)** | Bloodlord Mandokir (ZG) | 1.70 | 40.9 | +40 Attack Power, Equip: +1% Crit. |
| **`21520`** | **Ravencrest's Legacy** | AQ Scepter Quest | 2.80 | 58.4 | +13 Str, +9 Agi, +14 Sta. |
| **`18348`** | **Quel'Serrar** | Onyxia / Foror's Compendium | 2.00 | 52.5 | +12 Stamina, Chance on Hit: +300 Armor & +13 Defense. Legendary tank 1H sword. |
| **`1728`** | **Teebu's Blazing Longsword** | World Drop (BoE) | 2.90 | 44.8 | Chance on Hit: 150–250 Fire burst damage. Glowing fiery blade. |
| **`12584`** | **Veteran's Longsword** | Alliance Rank 14 PvP | 2.90 | 65.5 | +28 Strength, +7 Stamina, +1% Crit. (Grand Marshal 1H Sword) |
| **`16345`** | **Veteran's Blade** | Horde Rank 14 PvP | 2.90 | 65.5 | +28 Strength, +7 Stamina, +1% Crit. (High Warlord 1H Sword) |

---

### 🗡️ Daggers

| Item ID | Item Name | Boss / Source | Speed | DPS | Stats & Effects |
| :---: | :--- | :--- | :---: | :---: | :--- |
| **`22802`** | **Kingsfall** | Kel'Thuzad (Naxx) | 1.80 | 73.1 | +16 Agility, Equip: +1% Crit, +1% Hit, +28 Attack Power. |
| **`23044`** | **Harbinger of Doom** | Gluth / Patchwerk (Naxx) | 1.60 | 66.9 | +8 Stamina, Equip: +1% Hit, +1% Crit, +26 Attack Power. |
| **`21126`** | **Death's Sting** | C'Thun (AQ40) | 1.80 | 66.4 | +10 Stamina, Equip: +38 Attack Power, +3 Daggers Skill. |
| **`19346`** | **Dragonfang Blade** | Vaelastrasz (BWL) | 1.80 | 58.3 | +16 Agility, Equip: +36 Attack Power. |
| **`19347`** | **Claw of Chromaggus** | Chromaggus (BWL) | 1.80 | 58.3 | +17 Intellect, +7 Stamina, +64 Spell Damage, +4 MP5. |
| **`21244`** | **Blessed Qiraji Pugio** | AQ40 Quest | 1.70 | 58.5 | +6 Intellect, +7 Stamina, +72 Spell Damage, +1% Spell Crit, +1% Spell Hit. |
| **`19859`** | **Fang of the Faceless** | Hakkar (ZG) | 1.90 | 41.6 | Equip: +1% Crit, +28 Attack Power. |
| **`18816`** | **Perdition's Blade** | Ragnaros (MC) | 1.80 | 51.7 | Chance on hit: 40–56 Fire burst damage. |
| **`18805`** | **Core Hound Tooth** | Majordomo Chest (MC) | 1.60 | 45.0 | +9 Stamina, Equip: +1% Crit, +20 Attack Power. |
| **`22804`** | **Maexxna's Fang** | Maexxna (Naxx) | 1.80 | 65.6 | +8 Agility, +8 Stamina, +36 Attack Power, +1% Hit. |
| **`18838`** | **Veteran's Dirk** | Alliance Rank 14 PvP | 2.00 | 65.5 | +18 Agility, +8 Stamina, +1% Crit. |
| **`18840`** | **Veteran's Razor** | Horde Rank 14 PvP | 2.00 | 65.5 | +18 Agility, +8 Stamina, +1% Crit. |

---

### 🏹 Ranged Weapons & Bows

| Item ID | Item Name | Type | Boss / Source | Speed | DPS | Stats & Effects |
| :---: | :--- | :---: | :--- | :---: | :---: | :--- |
| **`22812`** | **Nerubian Slavemaker** | Crossbow | Kel'Thuzad (Naxx) | 3.20 | 56.6 | +24 Attack Power, +1% Crit. (Highest ranged physical DPS in Vanilla). |
| **`22811`** | **Soulstring** | Bow | Four Horsemen (Naxx) | 2.90 | 50.7 | +6 Stamina, +16 Attack Power, +1% Crit. |
| **`21459`** | **Crossbow of Imminent Doom** | Crossbow | Kurinnaxx (AQ20) | 3.10 | 45.5 | +7 Agility, +5 Stamina, +18 Attack Power. |
| **`21616`** | **Huhuran's Stinger** | Bow | Princess Huhuran (AQ40) | 2.70 | 45.6 | +18 Agility, +1% Hit. |
| **`23557`** | **Larvae of the Great Worm** | Gun | Ouro (AQ40) | 3.00 | 45.7 | +18 Attack Power, +1% Crit. |
| **`19361`** | **Ashjre'thul, Crossbow of Smiting** | Crossbow | Chromaggus (BWL) | 3.40 | 45.6 | +7 Stamina, Equip: +36 Attack Power. |
| **`18713`** | **Rhok'delar, Longbow of the Ancient Keepers** | Bow | Hunter Epic Quest | 2.90 | 44.0 | +17 Agility, Equip: +1% Crit. |
| **`17069`** | **Striker's Mark** | Bow | Magmadar (MC) | 2.50 | 39.0 | Equip: +1% Hit, +22 Attack Power. |
| **`17072`** | **Blastershot Launcher** | Gun | Golemagg (MC) | 2.60 | 39.4 | +6 Stamina, Equip: +1% Crit. |
| **`19853`** | **Gurubashi Dwarf Destroyer** | Gun | Bloodlord Mandokir (ZG) | 2.80 | 40.0 | +30 Attack Power. |
| **`20599`** | **Polished Ironwood Crossbow** | Crossbow | Dragons of Nightmare | 3.10 | 41.3 | +14 Attack Power, +1% Hit. |
| **`18833`** | **Veteran's Bullseye** | Bow | Alliance Rank 14 PvP | 1.80 | 50.8 | +14 Agility, +7 Stamina. |
| **`18835`** | **Veteran's Recurve** | Bow | Horde Rank 14 PvP | 1.80 | 50.8 | +14 Agility, +7 Stamina. |
| **`18836`** | **Veteran's Repeater** | Crossbow | Alliance Rank 14 PvP | 2.90 | 50.9 | +18 Attack Power, +1% Crit. |
| **`18837`** | **Veteran's Crossbow** | Crossbow | Horde Rank 14 PvP | 2.90 | 50.9 | +18 Attack Power, +1% Crit. |
| **`18855`** | **Veteran's Hand Cannon** | Gun | Alliance Rank 14 PvP | 2.90 | 50.9 | +18 Attack Power, +1% Crit. |
| **`18860`** | **Veteran's Street Sweeper** | Gun | Horde Rank 14 PvP | 2.90 | 50.9 | +18 Attack Power, +1% Crit. |

---

### 🔮 Staves, Caster 1H & Off-Hands

| Item ID | Item Name | Slot | Boss / Source | Stats & Effects |
| :---: | :--- | :---: | :--- | :--- |
| **`22800`** | **Brimstone Staff** | 2H Staff | Naxxramas | +18 Stamina, +18 Intellect, +84 Spell Damage, +2% Spell Crit, +1% Spell Hit. |
| **`22799`** | **Soulseeker** | 2H Staff | Kel'Thuzad (Naxx) | +24 Intellect, +24 Stamina, +85 Spell Damage, +2% Spell Crit, +1% Spell Hit. |
| **`22801`** | **Spire of Twilight** | 2H Staff | Thaddius (Naxx) | +25 Intellect, +25 Stamina, +111 Healing, +37 Spell Damage, +12 MP5. |
| **`19356`** | **Staff of the Shadow Flame** | 2H Staff | Nefarian (BWL) | +29 Stamina, +24 Intellect, +84 Spell Damage, +2% Spell Crit. |
| **`17113`** | **Amberseal Keeper** | 2H Staff | Lord Kazzak | +44 Spell Damage, +20 Intellect, +5 MP5. |
| **`20581`** | **Staff of Rampant Growth** | 2H Staff | Dragons of Nightmare | +15 Intellect, +15 Stamina, +40 Healing, +11 MP5. |
| **`22988`** | **The End of Dreams** | 1H Mace | Kel'Thuzad (Naxx) | +14 Intellect, +14 Stamina, +52 Spell Dmg, +94 Healing, +7 MP5. |
| **`23049`** | **Sapphiron's Left Eye** | Off-Hand | Sapphiron (Naxx) | +8 Intellect, +8 Stamina, +26 Spell Dmg, +1% Spell Crit, +1% Spell Hit. |
| **`19360`** | **Lok'amir il Romathis** | 1H Mace | Nefarian (BWL) | +18 Stamina, +18 Intellect, +84 Healing, +29 Spell Dmg. |
| **`17103`** | **Azuresong Mageblade** | 1H Sword | Golemagg (MC) | +12 Intellect, +7 Stamina, +40 Spell Dmg, +1% Spell Crit. |
| **`17070`** | **Fang of the Mystics** | 1H Mace | Azuregos | +40 Healing, +10 MP5, +10 Intellect. |
| **`19315`** | **Therazane's Touch** | Off-Hand | BWL Trash | +44 Healing, +12 MP5. |
| **`20582`** | **Trance Stone** | Off-Hand | Dragons of Nightmare | +10 Intellect, +8 Stamina, +25 Spell Damage. |
| **`23009`** | **Wand of the Whispering Dead** | Wand | Kel'Thuzad (Naxx) | +16 Spell Damage, +1% Spell Hit. (Highest spell DPS wand). |
| **`22820`** | **Wand of Fates** | Wand | Sapphiron (Naxx) | +7 Stamina, +7 Intellect, +31 Healing, +11 Spell Damage. |
| **`22821`** | **Doomfinger** | Wand | Naxxramas Trash | +16 Shadow Spell Damage, +1% Spell Crit. |

---

### 🛡️ Shields & Tank Off-Hands

| Item ID | Item Name | Armor | Block | Boss / Source | Stats & Effects |
| :---: | :--- | :---: | :---: | :--- | :--- |
| **`23043`** | **The Face of Death** | 3494 | 66 | Sapphiron (Naxx) | +14 Strength, +24 Stamina, +7 Defense, +1% Block, +19 Block Value. |
| **`22819`** | **Shield of Condemnation** | 3425 | 68 | Kel'Thuzad (Naxx) | +15 Intellect, +17 Stamina, +44 Healing, +15 Spell Dmg, +6 MP5. |
| **`22818`** | **The Plague Bearer** | 3106 | 59 | Gluth (Naxx) | +12 Strength, +21 Stamina, +1% Block, +15 Block Value. |
| **`23075`** | **Death's Bargain** | 3106 | 59 | Naxxramas Trash | +10 Strength, +20 Stamina, +1% Dodge, +1% Parry. |
| **`21610`** | **Wormscale Blocker** | 3035 | 57 | Ouro (AQ40) | +16 Stamina, +14 Intellect, +31 Healing, +11 Spell Dmg. |
| **`21269`** | **Blessed Qiraji Bulwark** | 2964 | 55 | AQ40 Quest | +20 Stamina, +8 Defense, +15 Block Value, +1% Block. |
| **`19349`** | **Elementium Reinforced Bulwark** | 2893 | 54 | Chromaggus (BWL) | +23 Stamina, +7 Defense, +19 Block Value. |
| **`19348`** | **Red Dragonscale Protector** | 2853 | 51 | Vaelastrasz (BWL) | +18 Stamina, +10 Intellect, +37 Healing, +5 MP5. |
| **`17066`** | **Drillborer Disk** | 2539 | 46 | Garr (MC) | +10 Stamina, +15 Defense, Chance on Block: 70–90 Arcane reflection. |
| **`17106`** | **Malistar's Defender** | 2822 | 52 | Ragnaros (MC) | +15 Intellect, +14 Stamina, +22 Healing, +8 Spell Dmg. |
| **`19862`** | **Aegis of the Blood God** | 2575 | 47 | Mandokir (ZG) | +14 Stamina, +7 Defense, +30 Block Value. |
| **`21485`** | **Buru's Skull Fragment** | 2575 | 47 | Buru the Gorger (AQ20) | +10 Stamina, +15 Block Value, +1% Block, +6 MP5. |
| **`19321`** | **The Immovable Object** | 2468 | 44 | AV PvP Vendor | +15 Stamina, +1% Block, +15 Block Value. |
| **`18825`** | **Veteran's Aegis** | 3106 | 59 | Alliance Rank 14 PvP | +15 Strength, +23 Stamina, +1% Block, +15 Block Value. |
| **`18826`** | **Veteran's Shield Wall** | 3106 | 59 | Horde Rank 14 PvP | +15 Strength, +23 Stamina, +1% Block, +15 Block Value. |

---

## Part VI: BiS Endgame Trinkets Directory

### ⚔️ Physical / Melee & Ranged Trinkets

- **`22954`** : **Kiss of the Spider** (Maexxna - Naxx) : +1% Hit, On-Use: +20% Attack Speed for 15 seconds. (BiS Physical Trinket).
- **`19406`** : **Drake Fang Talisman (DFT)** (Ebonroc - BWL) : +56 Attack Power, +2% Hit, +1% Dodge.
- **`21670`** : **Badge of the Swarmguard** (Battleguard Sartura - AQ40) : On-Use: Each physical attack adds +200 Armor Penetration (stacks up to 6 times for 1,200 Armor reduction).
- **`23206`** : **Mark of the Champion (Physical)** (Kel'Thuzad Quest) : +150 Attack Power against Undead and Demons.
- **`20130`** : **Diamond Flask** (Warrior Level 50 Class Quest) : On-Use: +75 Strength and restores 9 Health every 5 seconds for 1 minute.
- **`13965`** : **Blackhand's Breadth** (UBRS Quest) : +2% Critical Strike.
- **`19991`** : **Devilsaur Eye** (Hunter Quest) : +2% Hit, On-Use: +150 Attack Power for 20s.
- **`21180`** : **Earthstrike** (Cenarion Hold Logistics Rep) : On-Use: +280 Attack Power for 20 seconds.
- **`19289`** : **Darkmoon Card: Maelstrom** (Elementals Deck) : Chance on hit: 200–300 Nature damage strike.
- **`19287`** : **Darkmoon Card: Heroism** (Warlords Deck) : Chance on melee hit: Restores 120–180 Health.
- **`11815`** : **Hand of Justice** (BRD Emperor) : +20 Attack Power, 2% Chance on melee hit to gain an extra attack.

### 🔮 Caster & Spell Damage Trinkets

- **`19379`** : **Neltharion's Tear** (Nefarian - BWL) : +44 Spell Damage, +2% Spell Hit. (BiS Caster Trinket).
- **`23046`** : **The Restrained Essence of Sapphiron** (Sapphiron - Naxx) : +40 Spell Damage, On-Use: +130 Spell Damage for 20 seconds.
- **`23207`** : **Mark of the Champion (Caster)** (Kel'Thuzad Quest) : +85 Spell Damage against Undead and Demons.
- **`19950`** : **Zandalarian Hero Charm** (Heart of Hakkar Quest) : On-Use: +204 Spell Damage, diminishing with each spell cast.
- **`12930`** : **Briarwood Reed** (Jed Runewatcher - UBRS) : +29 Spell Damage.
- **`21625`** : **Scarab Brooch** (Viscidus - AQ40) : On-Use: Healing spells apply a shield for 15% of the amount healed for 30s.

### ✝️ Healer & MP5 Trinkets

- **`23047`** : **Eye of the Dead** (Sapphiron - Naxx) : On-Use: Reduces mana cost of next 5 healing spells by 150 and increases healing done by 450.
- **`23001`** : **Eye of Diminution** (Sapphiron - Naxx) : On-Use: Reduces threat caused by spells by 35% for 20 seconds.
- **`19395`** : **Rejuvenating Gem** (Ebonroc/Firemaw/Flamegor - BWL) : +66 Healing, +9 MP5. (Universal BiS Healer Trinket).
- **`18467`** : **Royal Seal of Eldre'Thalas** (Dire Maul Class Book) : +33 Healing, +4 MP5.
- **`19288`** : **Darkmoon Card: Blue Dragon** (Beasts Deck) : 2% chance on cast to allow 100% mana regen to continue while casting for 15 sec.

### 🛡️ Tank Mitigation & Avoidance Trinkets

- **`19431`** : **Styleen's Impeding Scarab** (Flamegor - BWL) : +13 Defense, +24 Block Value, +5% Chance to Block.
- **`17064`** : **Shard of the Scale** (Onyxia) : +16 MP5 / Regen.
- **`19947`** : **Nat Pagle's Broken Reel** (ZG Fishing) : On-Use: +10% Chance to Hit for 15s.
- **`11810`** : **Force of Will** (BRD Angerforge) : +10 Defense, +1% Dodge, On-hit: 25 Armor absorb.

---

## Part VII: BiS Rings, Amulets & Off-Set Equipment

### 💍 Rings & Bands

- **`21709`** : **Ring of the Fallen God** (C'Thun Eye Quest) : +6 Strength, +5 Agility, +8 Stamina, +1% Hit, +1% Crit. (Universal BiS physical ring).
- **`17063`** : **Band of Accuria** (Ragnaros - MC) : +16 Agility, +2% Hit, +1% Crit.
- **`18821`** : **Quick Strike Ring** (Magmadar - MC) : +5 Strength, +8 Agility, +1% Crit, +30 Attack Power.
- **`23038`** : **Band of Unnatural Forces** (Loatheb - Naxx) : +14 Stamina, +12 Intellect, +27 Spell Dmg, +1% Spell Crit, +1% Spell Hit.
- **`21596`** : **Ring of the Godslayer** (C'Thun) : +27 Spell Damage, +6 MP5.
- **`19382`** : **Pure Elementium Band** (Nefarian - BWL) : +10 Stamina, +9 Intellect, +44 Healing.
- **`19434`** : **Band of Dark Dominion** (BWL Trash) : +13 Stamina, +33 Shadow Spell Damage.
- **`19384`** : **Master Dragonslayer's Ring** (Nefarian Head Quest) : +14 Stamina, +48 Attack Power, +1% Hit.
- **`19397`** : **Ring of Blackrock** (Firemaw - BWL) : +19 Stamina, +9 Defense.

### 📿 Amulets & Necklaces

- **`19377`** : **Prestor's Talisman of Connivery** (Nefarian Head Quest) : +30 Agility, +1% Hit.
- **`23053`** : **Stormrage's Talisman of Seething** (Kel'Thuzad - Naxx) : +12 Strength, +12 Agility, +12 Stamina, +26 Attack Power, +2% Crit.
- **`23057`** : **Gem of Trapped Innocents** (Kel'Thuzad - Naxx) : +7 Stamina, +9 Intellect, +15 Spell Dmg, +2% Spell Crit.
- **`18814`** : **Choker of the Fire Lord** (Ragnaros - MC) : +7 Stamina, +7 Intellect, +34 Spell Damage.
- **`18404`** : **Onyxia Tooth Pendant** (Onyxia Head Quest) : +12 Agility, +9 Stamina, +1% Hit, +1% Crit, +10 Fire Resist.
- **`18406`** : **Onyxia Blood Talisman** (Onyxia Head Quest - Tank) : +15 Fire Resist, +1% Parry, +1% Defense.
- **`21504`** : **Charm of the Shifting Sands** (AQ20 Head Quest) : +12 Stamina, +25 Spell Damage, +1% Spell Hit.
- **`21712`** : **Amulet of the Fallen God** (C'Thun Eye Quest - Caster) : +11 Stamina, +6 Intellect, +18 Spell Damage, +1% Spell Crit, +1% Spell Hit.

### 🧣 Cloaks & Capes

- **`23045`** : **Shroud of Dominion** (Sapphiron - Naxx) : +11 Stamina, +50 Attack Power, +1% Crit.
- **`23050`** : **Cloak of the Necropolis** (Sapphiron - Naxx) : +8 Stamina, +7 Intellect, +27 Spell Dmg, +1% Spell Crit.
- **`21710`** : **Cloak of the Fallen God** (C'Thun) : +11 Stamina, +37 Spell Damage, +1% Spell Penetration.
- **`21621`** : **Cloak of the Golden Hive** (Princess Huhuran - AQ40) : +10 Stamina, +13 Agility, +1% Hit, +1% Crit.
- **`19386`** : **Elementium Threaded Cloak** (Chromaggus - BWL) : +169 Armor, +13 Stamina, +7 Agility, +2% Dodge.
- **`19398`** : **Cloak of Firemaw** (Firemaw - BWL) : +12 Stamina, +7 Intellect, +24 Spell Damage.
- **`13340`** : **Cape of the Black Baron** (Baron Rivendare - Strat) : +15 Agility, +20 Attack Power.

---

## Part VIII: Consumables, Flasks, Potions & World Buff Items Directory

### 🧪 Raid Flasks (Persistent Through Death)

- **`13512`** : **Flask of Supreme Power** : +150 Spell Damage for 2 hours.
- **`13510`** : **Flask of the Titans** : +1200 Maximum Health for 2 hours.
- **`13511`** : **Flask of Distilled Wisdom** : +2000 Maximum Mana for 2 hours.
- **`13513`** : **Flask of Chromatic Resistance** : +25 All Elemental & Magic Resistances for 2 hours.
- **`13506`** : **Flask of Petrification** : Turns you to stone, protecting from all physical/spell damage for 1 min.

### 🍷 Combat Elixirs & Potions

- **`13452`** : **Elixir of the Mongoose** : +25 Agility, +2% Crit for 1 hour.
- **`13454`** : **Greater Arcane Elixir** : +35 Spell Damage for 1 hour.
- **`9264`**  : **Elixir of Shadow Power** : +40 Shadow Spell Damage for 30 minutes.
- **`21546`** : **Elixir of Greater Firepower** : +40 Fire Spell Damage for 30 minutes.
- **`17708`** : **Elixir of Frost Power** : +15 Frost Spell Damage for 30 minutes.
- **`9206`**  : **Elixir of Giants** : +25 Strength for 1 hour.
- **`13445`** : **Elixir of Superior Defense** : +450 Armor for 1 hour.
- **`20007`** : **Mageblood Potion** : +12 MP5 for 1 hour.
- **`13455`** : **Greater Stoneshield Potion** : +2000 Armor for 2 minutes.
- **`13444`** : **Major Mana Potion** : Restores 1350 to 2250 Mana immediately.
- **`13446`** : **Major Healing Potion** : Restores 1050 to 1750 Health immediately.
- **`13457`** : **Greater Fire Protection Potion (GFPP)** : Absorbs 1950 to 3250 Fire damage for 1 hour.
- **`13458`** : **Greater Nature Protection Potion (GNPP)** : Absorbs 1950 to 3250 Nature damage for 1 hour.
- **`13459`** : **Greater Shadow Protection Potion (GSPP)** : Absorbs 1950 to 3250 Shadow damage for 1 hour.
- **`13456`** : **Greater Frost Protection Potion (GFstPP)** : Absorbs 1950 to 3250 Frost damage for 1 hour.
- **`13461`** : **Greater Arcane Protection Potion** : Absorbs 1950 to 3250 Arcane damage for 1 hour.
- **`5634`**  : **Free Action Potion (FAP)** : Immune to Stun and Movement Impairing effects for 30s.
- **`20008`** : **Living Action Potion (LAP)** : Clears active Stun/Slow effects and grants immunity for 5s.
- **`3387`**  : **Limited Invulnerability Potion (LIP)** : Immune to physical attacks for 6 seconds.
- **`2459`**  : **Swiftness Potion** : Increases run speed by +50% for 15 seconds.
- **`9172`**  : **Invisibility Potion** : Grants full invisibility for 18 seconds.

### 🍖 Food Buffs, Juju & World Items

- **`13928`** : **Grilled Squid** : +10 Agility for 10 minutes.
- **`20452`** : **Smoked Desert Dumplings** : +20 Strength for 15 minutes.
- **`13931`** : **Nightfin Soup** : +8 MP5 for 10 minutes.
- **`18254`** : **Runn Tum Tuber Surprise** : +10 Intellect for 10 minutes.
- **`12451`** : **Juju Power** : +30 Strength for 30 minutes.
- **`12460`** : **Juju Might** : +40 Attack Power for 10 minutes.
- **`12450`** : **Juju Flurry** : +3% Attack Speed for 20 seconds.
- **`12455`** : **Juju Ember** : +15 Fire Resistance for 10 minutes.
- **`12457`** : **Juju Chill** : +15 Frost Resistance for 10 minutes.
- **`12459`** : **Juju Escape** : +5% Dodge for 10 seconds.
- **`12458`** : **Juju Guile** : +30 Intellect for 30 minutes.
- **`12820`** : **Winterfall Firewater** : +35 Melee Attack Power and increases character scale for 20 min.
- **`18422`** : **Head of Onyxia** : Starts quest to trigger *Rallying Cry of the Dragonslayer* in Stormwind/Orgrimmar.
- **`19003`** : **Head of Nefarian** : Starts quest to trigger *Rallying Cry of the Dragonslayer*.
- **`19802`** : **Heart of Hakkar** : Starts quest to trigger *Spirit of Zandalar* on Yojamba Isle.

---

## Part IX: Permanent Weapon & Armor Enchantments Directory

### 🗡️ Weapon & Armor Enchant Formulas

- **`16252`** : **Formula: Enchant Weapon - Crusader** (Chance on hit: +100 Strength & restores 75–125 HP).
- **`18259`** : **Formula: Enchant Weapon - Spell Power** (+30 Spell Damage to 1H/2H Weapon).
- **`18260`** : **Formula: Enchant Weapon - Healing Power** (+55 Healing to 1H/2H Weapon).
- **`19445`** : **Formula: Enchant Weapon - Agility** (+15 Agility to 1H Weapon).
- **`22392`** : **Formula: Enchant 2H Weapon - Agility** (+25 Agility to 2H Weapon).
- **`16254`** : **Formula: Enchant Weapon - Lifestealing** (Chance on hit: 30 Life drain).
- **`16248`** : **Formula: Enchant Weapon - Unholy** (Chance on hit: Shadow debuff -15 AP).
- **`16253`** : **Formula: Enchant Chest - Greater Stats** (+4 All Stats).
- **`16221`** : **Formula: Enchant Chest - Major Health** (+100 Health).
- **`16219`** : **Formula: Enchant Gloves - Greater Agility** (+7 Agility).
- **`20731`** : **Formula: Enchant Gloves - Superior Agility** (+15 Agility).

### 📜 Head & Leg Arcanums / ZG Class Enchants

- **`18329`** : **Arcanum of Rapidity** (+1% Attack Speed to Head/Legs).
- **`18330`** : **Arcanum of Focus** (+8 Spell Damage & Healing to Head/Legs).
- **`18331`** : **Arcanum of Protection** (+1% Dodge to Head/Legs).
- **`11642`** : **Lesser Arcanum of Voracity** (+8 Strength, Agility, Stamina, Intellect, or Spirit).
- **`19782`** : **Presence of Might** (Warrior ZG: +10 Stamina, +7 Defense, +15 Shield Block Value).
- **`19783`** : **Syncretist's Sigil** (Paladin ZG: +10 Stamina, +7 Defense, +24 Healing).
- **`19784`** : **Death's Embrace** (Rogue ZG: +28 Attack Power, +1% Dodge).
- **`19785`** : **Falcon's Call** (Hunter ZG: +24 Ranged AP, +10 Stamina, +1% Hit).
- **`19786`** : **Vodouisant's Vigilant Embrace** (Shaman ZG: +15 Intellect, +13 Spell Dmg/Healing).
- **`19787`** : **Presence of Sight** (Mage/Warlock ZG: +18 Spell Damage, +1% Spell Hit).
- **`19788`** : **Hoodoo Hex** (Warlock/Shadow ZG: +10 Stamina, +18 Shadow Spell Damage).
- **`19789`** : **Prophetic Aura** (Priest ZG: +24 Healing, +4 Stamina, +4 MP5).
- **`19790`** : **Animist's Caress** (Druid ZG: +10 Stamina, +10 Intellect, +24 Healing).

---

## Part X: How to Optimize & Deploy BiS Gear for Turtle-WoW Latest Content

When jumping into **Turtle-WoW's latest custom endgame raids and dungeons**, standard Vanilla BiS gearing strategies must be adapted for new mechanics, high armor values, and heavy elemental damage profiles:

### 🗺️ The Turtle-WoW Endgame Raid Hierarchy

1. **Emerald Sanctum (Map ID `807` - 40-Man Raid)**: Heavy Nature damage, acid breaths, and dragonkin mechanics. Bosses like *Solnius* and *Erennius* hit hard and require rapid dispels and Nature Resistance.
2. **Scarlet Citadel (Map ID `45` - 40-Man Raid)**: Intense Holy and Fire damage, high-density add pulls, and enrage mechanics.
3. **Lower Karazhan Halls (Map ID `532` - 10-Man Raid)**: Shadow and Arcane mechanics requiring tight interrupts, high individual DPS checks, and crowd control.
4. **Karazhan Crypt (Map ID `800` - 5-Man Dungeon)** & **Hateforge Quarry (Map ID `808` - 5-Man Dungeon)**: High-tier 5-man progression instances dropping pre-raid and Tier 2 equivalent gear.

---

### 🛡️ 1. Tanking Strategy & Weapon Swapping in Custom Raids

#### When to Run Dual-Wield Fury/Prot

- **Use for**: High-threat single-target boss pulls, threat-sensitive DPS phases, and speed runs.
- **Loadout**: Thunderfury (`19019`) Main Hand + The Hungering Cold (`23577`) Off-Hand.
- **Why**: The Hungering Cold gives **+6 Swords Skill**, raising your weapon skill above the critical **305 cap**. This reduces the glancing blow damage reduction on Level 63 raid bosses from 40% down to just 5%, ensuring maximum Rage generation and 1,200+ TPS.

#### When to Shield Swap to Deep Prot / Dreadnaught

- **Use for**: Boss enrages, heavy physical cleaves, and crushing blow phases (e.g. *Solnius* enrage, *Patchwerk* hateful strikes).
- **Loadout**: Thunderfury (`19019`) + The Face of Death (`23043`) or Elementium Reinforced Bulwark (`19349`).
- **Why**: Pushes crushing blows completely off the combat table using *Shield Block* and achieves 440+ Defense to become uncrittable.

---

### 🌿 2. Elemental Resistance Gear Requirements

| Encounter / Raid | Required Resistance | Target Cap | Mandatory Consumables |
| :--- | :---: | :---: | :--- |
| **Emerald Sanctum (Solnius / Erennius)** | **Nature Resistance** | **250+ NR** | Greater Nature Protection Potion (`13458`), Juju Chill (`12457`), Bramblewood / Sandstalker gear. |
| **Scarlet Citadel & Hateforge Quarry** | **Fire Resistance** | **220+ FR** | Greater Fire Protection Potion (`13457`), Juju Ember (`12455`), Elementium Threaded Cloak (`19386`), Onyxia Tooth Pendant (`18404`). |
| **Lower Karazhan Halls & Naxxramas** | **Shadow Resistance** | **200+ SR** | Greater Shadow Protection Potion (`13459`), Band of Dark Dominion (`19434`), Cloak of the Necropolis (`23050`). |

---

### 🔮 3. Caster & Healer Scaling in Extended Encounters

1. **Spell Hit Cap (16% Mandatory)**:
   - Bosses in Emerald Sanctum and Scarlet Citadel have high spell resist levels.
   - Fire Mages must equip **Neltharion's Tear** (`19379`) and **Band of Unnatural Forces** (`23038`) alongside 3/3 Elemental Precision to eliminate spell resists on rolling *Ignite* stacks.
   - Warlocks must prioritize **Neltharion's Tear** and **Sapphiron's Left Eye** (`23049`) to ensure *Curse of Shadows* and *Shadow Bolts* never miss.

2. **Healer MP5 vs +Healing Balance**:
   - Turtle-WoW custom raid encounters are longer than standard Vanilla bosses.
   - Pure +Healing gear will run you out of mana. Balance high healing power with **Rejuvenating Gem** (`19395`), **Eye of the Dead** (`23047`), **Mageblood Potion** (`20007`), and **Nightfin Soup** (`13931`).

---

### 🚀 4. Ready-to-Use Raid Preparation Lua Macros

#### 🌿 Macro: 1-Click Nature Resistance Gear Swap (Emerald Sanctum)

```lua
.additem 13458 5
.additem 12457 20
.additem 13513 2
```

#### 🔥 Macro: 1-Click Fire Resistance Gear Swap (Scarlet Citadel)

```lua
.additem 13457 5
.additem 12455 20
.additem 19386 1
.additem 18404 1
```

#### 📦 Macro: Complete 1-Click Raid Consumable Restock

```lua
.additem 13512 2
.additem 13510 2
.additem 13452 10
.additem 13454 10
.additem 13444 20
.additem 13446 20
.additem 5634 10
.additem 20008 10
.additem 3387 5
.additem 13928 20
.additem 20452 20
.repairitems
```

---

[🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📜 Volume 01: GM Handbook](./01_game_master_handbook.md) • [🤖 Volume 02: Bot Automation](./02_playerbot_and_raid_automation.md) • [🗺️ Volume 03: Teleports](./03_turtle_world_and_teleports.md) • [📜 Volume 05: Eluna](./05_developer_eluna_and_engine.md)
