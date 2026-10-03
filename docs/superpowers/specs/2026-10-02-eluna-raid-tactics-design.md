# Eluna Autonomous Raid Tactics & High-Threat Trash Supervisor Design

## 1. Executive Summary & Objectives

This specification defines the architecture, encounter data contracts, and tactical automation engine for **`run/lua_scripts/raid_tactics.lua`**, an enterprise-grade Eluna Lua subsystem deployed on the Turtle-WoW 1.18.1 (Build 7272) CMaNGOS core.

The system acts as an autonomous tactical supervisor during raid encounters and difficult trash clearing, addressing complex mechanics where bot AI requires real-time coordination (e.g. tank swaps, bomb run-outs, crowd control preservation, vulnerable school target locks, and phase-specific positioning).

### Design Goals:
1. **Core Preservation**: Adheres strictly to `AGENTS.md`. No parallel C++ mechanics or engine bypasses; leverages native Eluna hooks (`RegisterCreatureEvent`, `RegisterServerEvent`, `Player:RunCommand`).
2. **Zero Idle Overhead**: Event-driven hooks without unbounded global updates or entry-0 wildcards. Active combat supervisor timers exist strictly while target encounters are engaged.
3. **Engine-Verified Data Contracts**: 100% of creature IDs, spell IDs, and aura mechanics are verified directly against CMaNGOS scripts and Turtle-WoW database tables.
4. **Seamless Integration**: Operates out of `run/lua_scripts/raid_tactics.lua`, loaded automatically by `Eluna.ScriptPath`, integrated with `.raid` command surface.

---

## 2. System Architecture & Lifecycle

### 2.1 Hook Registrars & Safety Protocols
- **Creature Lifecycle Hooks**:
  - `RegisterCreatureEvent(entry, 1, OnBossEnterCombat)` (`CREATURE_EVENT_ON_ENTER_COMBAT`)
  - `RegisterCreatureEvent(entry, 4, OnBossDied)` (`CREATURE_EVENT_ON_DIED`)
  - `RegisterCreatureEvent(entry, 2, OnBossLeaveCombat)` (`CREATURE_EVENT_ON_LEAVE_COMBAT`)
  - `RegisterCreatureEvent(entry, 9, OnBossDamageTaken)` (`CREATURE_EVENT_ON_DAMAGE_TAKEN`)
- **Error Handling Protocol**:
  - Every hook callback and timer routine is guarded by `pcall()`.
  - Errors log to `PrintInfo()` and report structured warnings to the raid leader without crashing the Lua engine.
- **Stale Object Pointer Prevention**:
  - Eluna object pointers (`Player*`, `Creature*`) are validated only for the current callstack frame.
  - Timers created via `CreateLuaEvent` capture GUIDs and string names, re-resolving live references via `GetPlayerByGUID()`, `GetPlayerByName()`, or `map:GetWorldObject(guid)` on each invocation.

### 2.2 Tactical Execution & Command Dispatch
- **Leader & Bot Command Emission**:
  - Directs bot raid members using `leader:RunCommand()`:
    - Group-wide orders: `bot r @tank, taunt`, `bot r @dps, focus <target>`, `bot r @melee, avoid aoe`.
    - Whispered unit orders: `bot w <name> flee`, `bot w <name> stay`, `bot w <name> cast <spell>`.
- **Raid Audio/Visual Alerts**:
  - Center-screen banners: `player:SendAreaTriggerMessage("|cFFFF0000[RAID TACTIC]|r " .. text)`
  - Chat log broadcast: `player:SendBroadcastMessage("|cFF00FFFF[Raid Tactics]:|r " .. text)`
- **Operator Command Surface**:
  - `.raid tactics` - Display active encounter status, monitored bosses, and enabled tactical rules.
  - `.raid tactics on` - Enable automated bot command dispatch.
  - `.raid tactics off` - Switch to advisory mode (announcements only, no bot command overrides).

---

## 3. Boss Encounter Tactical Modules

### 3.1 Blackwing Lair (Map 469)
1. **Razorgore the Untamed (`12435`)**:
   - *Phase 1*: Enforces 4-corner hold; monitors `DEATH_TALON_DRAGONSPAWN` (`12422`) sleep states; suppresses AoE attacks near slept targets.
   - *Phase 2*: Detects `Warming Flames` (`23040`) / Razorgore engage; commands raid to collapse; monitors `Conflagration` (`23023`) on MT and triggers instant offtank taunt.
2. **Vaelastrasz the Corrupt (`13020`)**:
   - *Burning Adrenaline (`23620`)*: Periodic scan on raid members. When MT receives aura `23620`, alerts dying tank to run to sacrifice wall and orders Offtank #2 to immediately taunt boss.
3. **Broodlord Lashlayer (`12017`)**:
   - *Knock Away (`18670`)*: Detects 50% threat wipe on primary target; triggers immediate offtank taunt.
4. **Ebonroc (`14601`)**:
   - *Shadow of Ebonroc (`23340`)*: Detects debuff on active tank; triggers immediate offtank taunt swap to starve boss of 25,000 HP heal-on-hit.
5. **Chromaggus (`14020`)**:
   - *Brood Afflictions*: Monitors Blue (`23153`), Black (`23154`), Red (`23155`), Bronze (`23170`), and Green (`23169`); triggers high-priority cleanse warnings and Bronze cure (`Item 19183`).
   - *Frenzy (`23342`)*: Issues 1-second hunter Tranquilizing Shot prompt.
6. **Nefarian (`11583`)**:
   - *Class Calls*: Identifies active class call (Priests heal damage, Druids forced cat, Mages wild polymorph, Shaman totems, Warrior berserker stance) and orders mitigation.
   - *Phase 3 (20% HP)*: Detects Bone Construct (`14605`) resurrection; commands mage frost/slow kite while DPS burn Nefarian.

### 3.2 Molten Core (Map 409)
1. **Baron Geddon (`12056`)**:
   - *Living Bomb (`20475`)*: Scans raid; when player/bot receives aura `20475`, issues center-screen alert and orders that bot to flee 25yd out from raid cluster.
2. **Majordomo Executus (`12018`)**:
   - *Reflective Shields*: Detects `Damage Shield` (`21075` - reflects melee) vs `Magic Reflection` (`20619` - reflects spells); commands melee or casters to pause attacks or swap targets.
3. **Shazzrah (`12264`)**:
   - *Blink & Arcane Explosion*: Detects Gate teleport; orders tanks to intercept and priests/shamans to purge Magic Grounding.
4. **Garr (`12057`)**:
   - *Firesworn (`12099`)*: Monitors add deaths; commands melee to step back before death explosion.
5. **Ragnaros (`11502`)**:
   - *Submerge*: Detects Sons of Flame spawn; commands priority burn and repositioning.

### 3.3 Temple & Ruins of Ahn'Qiraj (Maps 531 & 509)
1. **Twin Emperors (`15275` / `15276`)**:
   - *Teleport Swap (`Spell 800`)*: Detects teleport cast; commands tank swap (Warrior onto Vek'nilash `15275`, Warlock onto Vek'lor `15276`) and healer re-anchoring across the 60yd room divider.
2. **Princess Huhuran (`15509`)**:
   - *Berserk (30% HP)*: Alerts raid for Acid Spit volley; triggers defensive cooldowns and Nature Protection potion reminders for soak tanks.
3. **C'Thun (`15727`)**:
   - *Eye Beam*: Enforces 10yd spread warning; detects Stomach of C'Thun transfers and prioritizes Flesh Tentacles.
4. **General Rajaxx (`15341`)**:
   - *Officer Wave Defense*: Manages wave progress and keeps party anchored to protect General Andorov.

### 3.4 Turtle Custom Raids
1. **Emerald Sanctum (Map 807)**:
   - *Erennius (`60747`) & Solnius (`60748`)*: Breath cone facing alerts; marks Solnius add waves as primary kill targets.
2. **Lower Karazhan Halls (Map 532)**:
   - *Brood Queen Araxxna (`61221`) & Clawlord Howlfang (`61223`)*: Poison cleanse and bleed alerts.
   - *Lord Blackwald II (`61222`)*: Clears apprentice waves and positions Blackwald.
   - *Moroes (`61225`)*: Vanish / Garrote tracker with quick heals and stage transitions.
3. **Scarlet Citadel (Map 45)**:
   - Holy / Fire mitigation alerts and elite Crusader add focus.

---

## 4. High-Threat Trash Pack Management

1. **Core Hound Packs (`11671` - Molten Core)**:
   - Monitored within 40yd combat radius.
   - When any hound reaches <15% HP, issues `@dps, focus` warnings to balance damage across all living hounds to prevent the 15-second revival.
2. **Lava Surgers & Firelords (`12101` / `11668` - Molten Core)**:
   - Firelords marked with Skull; priority target lock to eliminate multiplying Lava Spawns (`12265`).
   - Surger Charge: alerts tanks to intercept and stun.
3. **Death Talon Wyrmguards (`12460` - Blackwing Lair)**:
   - Identifies active vulnerability buff on combat engage:
     - Fire (`22277`) -> Calls Fire Mages / Warlocks.
     - Frost (`22278`) -> Calls Frost Mages.
     - Shadow (`22279`) -> Calls Warlocks / Shadow Priests.
     - Nature (`22280`) -> Calls Hunters / Elemental Shamans.
     - Arcane (`22281`) -> Calls Arcane Mages.
4. **Anubisath Sentinels & Defenders (`15264` / `15277` - AQ40/AQ20)**:
   - Identifies active ability buff on combat engage:
     - *Shadow Storm (`2148`)* -> Raid closes into <15yd safe melee zone.
     - *Reflect Arcane/Fire (`13022`) / Shadow/Frost (`19595`)* -> Affected casters switch to wand or physical DPS.
     - *Thorns (`25777`)* -> Melee back off; casters burn.
     - *Explode* -> Tank pulls Sentinel 20yd away from raid at 10% HP.

---

## 5. Verification & Acceptance Criteria

1. **Syntax & Engine Validation**:
   - `run/lua_scripts/raid_tactics.lua` passes Lua 5.2 syntax validation (`luac -p`).
   - Re-running `.reload eluna` in `mangosd` loads the script without warnings or errors.
2. **Runtime Verification**:
   - `.raid tactics` command returns status, active modes, and registered encounter modules.
   - Tested against test-runs (`.dc test start <token>`) and live raid groups.
