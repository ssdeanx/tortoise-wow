# 19. Eluna Lua Engine & Scripting Framework

> **Target Core**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + Eluna Lua Engine  
> **Source Verification**: `src/modules/Eluna/`, `src/game/Eluna/CustomMethods.h` & `docs/ELUNA.md`  
> **Pinned Submodule Revision**: `1b06f28ff3a00054d915d824c725fb4283fee74d` (Compatibility Backend: `ELUNA_VMANGOS`, Expansion: Vanilla `0`)  
> **Quick Navigation**: [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📖 Project Readme](../README.md) • [⚡ Reload Manual (02_server_and_reloads.md)](./02_server_and_reloads.md) • [🛠️ Developer Diagnostics (16_developer_and_diagnostics.md)](./16_developer_and_diagnostics.md)

---

## Complete 20-Module Reference Library

| Security & Server | World & Characters | Gameplay & Items | Bots & Modules |
| :--- | :--- | :--- | :--- |
| [00. Self & Own Character](./00_self_and_player_commands.md) | [03. Teleports & Movement](./03_teleports_and_movement.md) | [06. Quests & Instances](./06_quests_instances_and_events.md) | [11. Playerbot Suite](./11_playerbot_suite.md) |
| [01. Security & Accounts](./01_security_and_account.md) | [04. Stats & Visuals](./04_character_stats_and_visuals.md) | [07. Items & Economy](./07_items_economy_and_mail.md) | [12. RandomBot Manager](./12_randombot_manager.md) |
| [02. Server & Reloads](./02_server_and_reloads.md) | [05. Combat & Spells](./05_combat_spells_and_skills.md) | [08. Spawning & Editing](./08_spawning_and_world_editing.md) | [13. Bot Whispers & Macros](./13_bot_whispers_and_macros.md) |
| [09. Moderation & Anticheat](./09_moderation_tickets_and_anticheat.md) | [10. Turtle Custom](./10_turtle_custom_features.md) | [17. Tier Sets & Gear](./17_tier_items_and_endgame_gear.md) | [14. Bot Strategies & Tactics](./14_bot_strategies_and_tactics.md) |
| [16. Diagnostics & Lookups](./16_developer_and_diagnostics.md) | [18. Class Talent Specs](./18_class_talent_specs.md) | *(Standalone Portals)* | [15. Dungeon Clear AI](./15_dungeon_clear_module.md) |
| *(Server Scripting)* | *(Engine Core)* | *(Runtime Lua API)* | **[19. Eluna Lua Engine](./19_eluna_lua_engine.md)** |

---

## 1. Categorized In-Game Lua Macros (Top Priority)

Copy and paste these macros directly into your World of Warcraft **Macro UI** (`/m`) or run them directly in chat:

### Category A: Live Script Hot-Reloading & State Maintenance (Top Priority)

#### 🔄 Macro 1: Hot-Reload All Eluna Lua Scripts Globally

Re-reads and compiles all scripts under `./lua_scripts` across all active maps and global states without dropping player connections:

```lua
.reload eluna
```

#### 🗺️ Macro 2: Hot-Reload Eluna Scripts for Current Map Only

Reloads Lua states exclusively for a specific map instance (e.g. Map 0 = Eastern Kingdoms, Map 1 = Kalimdor, Map 33 = Shadowfang Keep):

```lua
.reload eluna 0
```

---

### Category B: Eluna Diagnostics & Error Log Audit (Middle Priority)

#### 📜 Macro 3: Dump Active Server Scripting & Engine Info

Displays active core revision, server uptime, and loaded modules:

```lua
.server info
.debug instancedata
```

---

## 2. In-Game Eluna Commands & Security Matrix

Verified against Eluna command hooks (`src/modules/Eluna/hooks/PlayerHooks.cpp:75-95`):

| Command | Security Level | Scope | Description | Usage / Example |
| :--- | :---: | :---: | :--- | :--- |
| `.reload eluna` | Configurable (`Eluna.ReloadSecurityLevel`, default 3) | Global | Hot-reloads all Lua states and re-executes all scripts in `./lua_scripts`. | `.reload eluna` |
| `.reload eluna <mapId>` | Configurable (`Eluna.ReloadSecurityLevel`, default 3) | Map-Specific | Hot-reloads Lua state for the specified Map ID only. | `.reload eluna 43` |

> [!NOTE]
> In addition to the built-in `.reload eluna` command, custom in-game slash commands can be registered dynamically inside Lua scripts via the `PLAYER_EVENT_ON_COMMAND` (Event ID `42`) hook.

---

## 3. Configuration & Runtime Architecture (`mangosd.conf`)

Eluna is configured via the `Eluna.*` section in `src/mangosd/mangosd.conf.dist.in` (and active `mangosd.conf`):

| Config Key | Default Value | Description |
| :--- | :---: | :--- |
| `Eluna.Enabled` | `1` | Master runtime toggle for the Eluna Lua engine. (Build switch is `-DBUILD_ELUNA=ON`). |
| `Eluna.ScriptPath` | `./lua_scripts` | Path to directory where server-side Lua scripts (`*.lua`) are loaded from. |
| `Eluna.TraceBack` | `1` | Enables full Lua stack tracebacks in error logs upon runtime script exceptions. |
| `Eluna.ReloadCommand` | `1` | Enables the `.reload eluna` in-game command. |
| `Eluna.ReloadSecurityLevel` | `3` | Minimum security rank required to run `.reload eluna` (3 = `SEC_DEVELOPER`). |
| `Eluna.UseUnsafeMethods` | `0` | Allows scripts to access unsafe OS/file system Lua libraries (`io`, `os.execute`). |
| `Eluna.UseDeprecatedMethods` | `1` | Maintains backwards compatibility with deprecated Eluna API calls. |
| `Eluna.OnlyOnMaps` | `""` | Restricts Eluna state creation to specific map IDs (empty string = all maps enabled). |
| `Eluna.RequirePaths` | `""` | Additional module search paths for Lua `require()` statements. |
| `Eluna.RequireCPaths` | `""` | Additional C module search paths for Lua binary libraries (`.so` / `.dll`). |
| `ElunaErrorLogFile` | `ElunaErrors.log` | Filename where runtime Lua compilation and runtime errors are logged. |

---

## 4. Eluna Event Registrations & Custom Command API

Eluna connects Lua functions to core events via `Register*Event` functions:

### 🌟 1. Custom Chat & In-Game Command Dispatch (`PLAYER_EVENT_ON_COMMAND = 42`)

You can create custom `.commands` or `/commands` in Lua by listening to player command events:

```lua
-- lua_scripts/custom_commands.lua
local function OnPlayerCommand(event, player, command)
    -- command contains the full text entered (e.g. "tele stormwind" or "buffme")
    if command == "buffme" then
        if player then
            player:AddAura(25898, player) -- Greater Blessing of Kings
            player:AddAura(27681, player) -- Prayer of Fortitude
            player:SendBroadcastMessage("You have been fully buffed!")
            return false -- Suppresses "There is no such command" core error
        end
    end
    
    if command:find("teleport ") == 1 then
        local dest = command:sub(10)
        if dest == "mall" then
            player:Teleport(1, 16226.5, 16258.2, 13.2, 1.5)
            return false
        end
    end
    
    return true -- Let other handlers process standard core commands
end

RegisterPlayerEvent(42, OnPlayerCommand) -- 42 = PLAYER_EVENT_ON_COMMAND
```

---

### 📋 2. Essential Event Registration Types

| Registration Function | Purpose & Scope | Key Event IDs |
| :--- | :--- | :--- |
| `RegisterPlayerEvent(eventId, fn)` | Hooks player lifecycle, chat, combat, trade, and commands. | `1` (Login), `2` (Logout), `42` (OnCommand), `18` (Chat), `33` (EnterCombat) |
| `RegisterCreatureEvent(entry, eventId, fn)` | Hooks NPC spawning, combat, aggro, death, and speech. | `1` (EnterCombat), `2` (LeaveCombat), `4` (Died), `5` (TargetKilled) |
| `RegisterCreatureGossipEvent(entry, eventId, fn)` | Hooks NPC gossip dialogue options and menu selections. | `1` (OnHello), `2` (OnSelect) |
| `RegisterGameObjectEvent(entry, eventId, fn)` | Hooks GameObjects (chests, doors, levers, shrines). | `1` (OnUse), `2` (OnGossipHello), `3` (OnGossipSelect) |
| `RegisterItemEvent(entry, eventId, fn)` | Hooks item usage, equipping, and opening. | `1` (OnUse), `2` (OnExpire) |
| `RegisterServerEvent(eventId, fn)` | Global server lifecycle and world tick events. | `14` (WorldUpdate), `16` (ServerShutdown) |
| `RegisterMapEvent(mapId, eventId, fn)` | Instance creation, map updates, and player entry. | `1` (OnCreate), `2` (OnDestroy), `3` (OnPlayerEnter) |

---

## 5. Turtle WoW Custom `SpellInfo` Compatibility Layer

In `src/game/Eluna/CustomMethods.h`, this core exposes a dedicated vanilla-compatible `SpellInfo` wrapper that directly queries Turtle WoW's `spell_template` database table, giving Lua full visibility into custom Turtle spells and SQL-modified stats:

### Available Global & Object Methods

- `GetSpellInfo(spellId)`: Returns the `ElunaSpellInfo` wrapper object.
- `Aura:GetSpellInfo()`: Returns the spell info for an active aura.
- `Spell:GetSpellInfo()`: Returns the spell info for a cast in progress.

### Supported Getters on `SpellInfo` Object

```lua
local spell = GetSpellInfo(133) -- Fireball Rank 1

local id           = spell:GetId()
local manaCost     = spell:GetManaCost()
local castTime     = spell:GetCastTime()
local minRange     = spell:GetMinRange()
local maxRange     = spell:GetMaxRange()
local duration     = spell:GetDuration()
local speed        = spell:GetSpeed()
local schoolMask   = spell:GetSchoolMask()
local procChance   = spell:GetProcChance()
local stackAmount  = spell:GetStackAmount()
```

---

## 6. Example Lua Scripts for Turtle-WoW Server Operators

### 🚀 Example 1: Custom Welcome Message & Starter Kit on First Login

```lua
-- lua_scripts/starter_welcome.lua
local function OnFirstLogin(event, player)
    player:SendBroadcastMessage("|cFF00FF00Welcome to Turtle-WoW! Enjoy your custom adventure.|r")
    player:AddItem(6948, 1)  -- Hearthstone
    player:AddItem(4540, 5)  -- Tough Jerky
    player:AddItem(159, 5)   -- Refreshing Spring Water
end

RegisterPlayerEvent(30, OnFirstLogin) -- 30 = PLAYER_EVENT_ON_FIRST_LOGIN
```

### ⚔️ Example 2: Dynamic World Boss Kill Announcer

```lua
-- lua_scripts/world_boss_announcer.lua
local WORLD_BOSSES = {
    [6109] = "Azuregos",
    [12397] = "Lord Kazzak",
    [14889] = "Taerar",
    [14888] = "Lethon"
}

local function OnBossDeath(event, creature, killer)
    local bossName = WORLD_BOSSES[creature:GetEntry()]
    if bossName and killer then
        local killerName = killer:GetName() or "a brave hero"
        SendWorldMessage(string.format("|cFFFF0000[World Announcement]|r %s has been slain by %s!", bossName, killerName))
    end
end

for entry, _ in pairs(WORLD_BOSSES) do
    RegisterCreatureEvent(entry, 4, OnBossDeath) -- 4 = CREATURE_EVENT_ON_DIED
end
```

---

[⬅ Prev: 18. Class Talent Specs](./18_class_talent_specs.md) • [🏠 Master Portal (gm_commands.md)](../gm_commands.md) • [📖 Project Readme](../README.md) • [⚡ Reload Manual (02_server_and_reloads.md)](./02_server_and_reloads.md)
