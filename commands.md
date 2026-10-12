# Tortoise-WoW Server & Operations Master Reference

> **Target Core**: Turtle-WoW 1.18.1 (Build 7272) • CMaNGOS + PlayerBots Framework + Eluna Engine
> **Master Portals**: [🏠 Master GM Portal (gm_commands.md)](./gm_commands.md) • [🤖 Volume 02: Bot Automation (references/02_playerbot_and_raid_automation.md)](./references/02_playerbot_and_raid_automation.md) • [📜 Volume 01: GM Handbook (references/01_game_master_handbook.md)](./references/01_game_master_handbook.md)

---

## 🧭 Master Quick-Navigation Hub

| Scope | Reference Document | Purpose |
| :--- | :--- | :--- |
| **System & Host** | **`commands.md` (Current File)** | Linux systemd services, build/deploy steps, and server maintenance. |
| **Complete GM Codex** | [**`gm_commands.md`**](./gm_commands.md) | 100% of GM commands, cheats, world edits, security, and reloads. |
| **RandomBots** | [**`rndbot_commands.md`**](./rndbot_commands.md) | Server-wide bot population, telemetry, and governor tuning. |
| **PlayerBots & Raid** | [**Volume 02**](./references/02_playerbot_and_raid_automation.md) | Party bot control, whisper shortcuts, strategies, and `.dc` autopilot. |
| **Teleports & Dungeons** | [**Volume 03**](./references/03_turtle_world_and_teleports.md) | 647+ `.tele` tokens including Turtle-WoW custom instances & keys. |
| **Class BiS Loadouts** | [**Volume 04**](./references/04_class_specs_and_endgame_gear.md) | 1-Click BiS gear loadouts, Tier 1/2/3 set IDs, and talent builds. |
| **Eluna Scripting** | [**Volume 05**](./references/05_developer_eluna_and_engine.md) | Runtime Lua scripting, hook events, and live reloads. |
| **Raid Automation** | [**Volume 06**](./references/06_automated_raid_and_dungeon_operations.md) | Autonomous 40-man raid clearing pipeline (`.raid ready`). |
| **Guild Operations** | [**Volume 07**](./references/07_guilds_and_guild_operations.md) | Guild bots, scope commands (`guild`), charters, orders & 25 bot guilds. |
| **Open World & Strats** | [**Volume 08**](./references/08_open_world_control_and_strategies.md) | World simulation, grinding, RPG life, strategy catalog & live feedback. |

---

## 1. Linux Host & Systemd Service Management

These are **systemd user units** (run as normal user with `systemctl --user`, without `sudo`). Units reside in `~/.config/systemd/user/`.

- `turtle-mangosd.service` — World server (processes world state, combat, scripts, and playerbot AI).
- `turtle-realmd.service` — Authentication and realm list server.
- `turtle-logrotate.timer` — Rotates bot logs automatically.
- **MariaDB Database**: System service (`mariadb.service`). Always leave running; lightweight when idle.

### Daily Service Control

```bash
# Sit down to play: Start world server and realm auth daemon
systemctl --user start turtle-mangosd turtle-realmd

# Done playing: Stop servers to return CPU to idle
systemctl --user stop turtle-mangosd turtle-realmd

# Restart world server (e.g. after updating conf or rebuilding binary)
systemctl --user restart turtle-mangosd

# Check service health and live status
systemctl --user status turtle-mangosd turtle-realmd
```

### Log Monitoring & Crash Diagnostics

```bash
# Follow live world server log stream in real time
journalctl --user -u turtle-mangosd -f

# View recent logs from the past 5 minutes (shows startup crashes and assertion faults)
journalctl --user -u turtle-mangosd --since "5 min ago"

# View logs across the entire current machine boot
journalctl --user -u turtle-mangosd -b

# Follow auth/realmd logs
journalctl --user -u turtle-realmd -f
```

### Recover from Stuck / Rate-Limited State (`reset-failed`)

If the server crashes repeatedly on startup, systemd rate-limits restart attempts and silently drops new start requests. Clear the error state before starting again:

```bash
systemctl --user reset-failed turtle-mangosd turtle-realmd
systemctl --user start turtle-mangosd turtle-realmd
systemctl --user status turtle-mangosd
```

### Build, Compile & Deploy

```bash
# 1. Stop services so you do not overwrite a running binary
systemctl --user stop turtle-mangosd turtle-realmd

# 2. Configure release build with Eluna Lua engine and Playerbots
# Explicitly disable mod-llm-chat to keep LLM features off
cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/home/sam/server \
  -DBUILD_PLAYERBOTS=ON \
  -DBUILD_ELUNA=ON \
  -DENABLE_SOAP=ON \
  -DMODULES=static \
  -DMODULE_MOD_LLM_CHAT=disabled \
  -DUSE_EXTRACTORS=ON \
  -DBUILD_TESTING=ON \
  -DUSE_TRACY=ON \
  -DTRACY_ENABLE=ON \
  -DENABLE_LSAN=ON \
  -DPROFILE_USE=ON



# 3. Compile across all CPU cores
cmake --build build -j$(nproc)

# 2. Install binaries and assets to /home/sam/server
cmake --install build

# 4. Clear rate-limit locks and launch
systemctl --user reset-failed turtle-mangosd turtle-realmd
systemctl --user start turtle-mangosd turtle-realmd
systemctl --user status turtle-mangosd
```

> [!NOTE]
> **Build Flag Details:**
>
> - `-DBUILD_ELUNA=ON`: Compiles the Eluna Lua scripting engine (`src/modules/Eluna/`).
> - `-DBUILD_PLAYERBOTS=ON`: Compiles the playerbots AI subsystem.
> - `-DMODULES=static`: Statically links active modules into `mangosd` (including `mod-playerbots` and `mod-dungeon-clear` for `.dc`).
> - `-DMODULE_MOD_LLM_CHAT=disabled`: Explicitly excludes `mod-llm-chat` at build time so no generative AI chat code is compiled.
> - `-DUSE_EXTRACTORS=ON`: Builds client map, DBC, vmap, and mmap extractor utilities.
>
> **Keep LLM Chat Off (Runtime & Config):**
>
> - In `run/modules/mod-llm-chat.conf`, verify `ModLlmChat.Enable = 0`.
>
> **Eluna Lua Scripts & Hot-Reloading:**
>
> - Script directory: `run/lua_scripts/` (executed automatically on world startup).
> - Hot-reload without server restart: Run `reload eluna` in server console or `.reload eluna` in-game.

### Autostart on System Boot

```bash
# Disable autostart (only runs when explicitly started)
systemctl --user disable turtle-mangosd turtle-realmd

# Enable autostart on system boot
systemctl --user enable turtle-mangosd turtle-realmd
```

---

## 2. Server Console & Maintenance Commands

Commands can be executed directly in the `mangosd` terminal console (without leading dot `.`) or in-game by Game Masters (with leading dot `.`):

| Server Task | Console Command | In-Game GM Command | Function / Details |
| :--- | :--- | :--- | :--- |
| **Immediate DB Save** | `saveall` | `.saveall` | Synchronously commits all online player inventories, quests, and stats to DB. |
| **Graceful Restart** | `server restart 60 Maintenance` | `.server restart 60 Maintenance` | Announces countdown in system chat and restarts in 60 seconds. |
| **Cancel Restart** | `server restart cancel` | `.server restart cancel` | Aborts a pending server restart. |
| **Idle Restart** | `server idlerestart 120` | `.server idlerestart 120` | Waits for player count or activity to drop before restarting. |
| **Graceful Shutdown** | `server shutdown 60` | `.server shutdown 60` | Broadcasts shutdown countdown and safely persists state. |
| **Set MOTD** | `server set motd <text>` | `.server set motd <text>` | Updates Message of the Day live without server restart. |
| **Clear Dead Characters** | `character clean todelete` | `.character clean todelete` | Purges deleted characters from `tw_char`. |
| **Clean Orphaned Items** | `character clean items` | `.character clean items` | Purges orphaned item instances and expired mail in DB. |
| **Report Performance** | `perf intervalreport 60` | `.perf intervalreport 60` | Emits server tick diff & memory metrics every 60 seconds. |
| **Hot-Reload Eluna** | `reload eluna` | `.reload eluna` | Live-reloads all Lua scripts in `run/lua_scripts/` without a server restart. |

---

## 3. Instant Boost & Skill/Profession Maxing Suite

> [!IMPORTANT]
> **Why `.maxskill` doesn't max professions:**
> The core engine in `Player.cpp` explicitly skips professions, secondary skills, and riding (`if (IsProfessionOrRidingSkill(pskill)) continue;`). `.maxskill` only updates weapons and defense.
> To max professions and secondary skills to 300, use `.learn all_recipes <name>` (learns all recipes and maxes rating) or `.setskill <id> 300 300`.

### 🌟 Macro 1: Complete Level 60 Instant Boost Suite (Spells + Skills + Travel + Bags)

```lua
.levelup 60
.learn all_myspells
.learn all_trainer
.maxskill
.modify money 10000000
.additem 14156 4
.taxicheat on
.explorecheat on
.repairitems
.replenish
```

### 🛠️ Macro 2: Max All 4 Secondary Skills (1-Click)

Learns all recipes and raises skills to 300/300 for all 4 secondary skills in Turtle-WoW:

```lua
.learn all_recipes first aid
.learn all_recipes cooking
.learn all_recipes fishing
.learn all_recipes survival
```

### ⚗️ Macro 3: Max Primary Professions (Run the 2 for your character)

```lua
-- Alchemy & Herbalism
.learn all_recipes alchemy
.learn all_recipes herbalism

-- Blacksmithing & Mining
.learn all_recipes blacksmithing
.learn all_recipes mining

-- Engineering & Mining
.learn all_recipes engineering
.learn all_recipes mining

-- Enchanting & Tailoring
.learn all_recipes enchanting
.learn all_recipes tailoring

-- Leatherworking & Skinning
.learn all_recipes leatherworking
.learn all_recipes skinning
```

### 🎯 Skill ID Lookup Table (`.setskill <id> <current> [max]`)

| Skill Line | Skill ID | Apprentice Spell | Set to 300 (Level 60) | Check Rating |
| :--- | :---: | :---: | :--- | :--- |
| **First Aid** | `129` | `3273` | `.setskill 129 300 300` | `.getskillvalue 129` |
| **Survival (Turtle)** | `142` | `50000` | `.setskill 142 300 300` | `.getskillvalue 142` |
| **Cooking** | `185` | `2550` | `.setskill 185 300 300` | `.getskillvalue 185` |
| **Fishing** | `356` | `7620` | `.setskill 356 300 300` | `.getskillvalue 356` |
| **Riding** | `762` | — | `.setskill 762 300 300` | `.getskillvalue 762` |
| **Alchemy** | `171` | `2259` | `.setskill 171 300 300` | `.getskillvalue 171` |
| **Blacksmithing** | `164` | `2018` | `.setskill 164 300 300` | `.getskillvalue 164` |
| **Enchanting** | `333` | `7411` | `.setskill 333 300 300` | `.getskillvalue 333` |
| **Engineering** | `202` | `4036` | `.setskill 202 300 300` | `.getskillvalue 202` |
| **Herbalism** | `182` | `2366` | `.setskill 182 300 300` | `.getskillvalue 182` |
| **Leatherworking** | `165` | `2108` | `.setskill 165 300 300` | `.getskillvalue 165` |
| **Mining** | `186` | `2575` | `.setskill 186 300 300` | `.getskillvalue 186` |
| **Skinning** | `393` | `8613` | `.setskill 393 300 300` | `.getskillvalue 393` |
| **Tailoring** | `197` | `3908` | `.setskill 197 300 300` | `.getskillvalue 197` |
| **Lockpicking (Rogue)** | `633` | — | `.setskill 633 300 300` | `.getskillvalue 633` |

---

## 4. Game Master Cheats & Power Tools

| Command | Sec | Exact Function |
| :--- | :---: | :--- |
| `.god [on\|off]` | `DEV` | Toggles invulnerability HP floor (HP cannot fall below 1). |
| `.gm options <flags> [off]` | `ADMIN` | **Engine-level GM cheats**: `cd` (zero cooldowns), `cast` (instant cast), `power` (infinite mana/energy/rage), `crit` (100% crit), `proc` (100% proc chance), `checkcast` (bypass spell requirements). |
| `.combatstop` | `MOD` | Instantly drops combat state for yourself or selected target. |
| `.cooldown [id]` | `DEV` | Clears all spell cooldowns, or clears specific spell ID. |
| `.hover [on\|off]` | `ADMIN` | Enables collision-free 3D flight/levitation. |
| `.waterwalk [on\|off]` | `DEV` | Enables walking on water for selected player. |
| `.cometome` | `DEV` | Teleports the selected target directly to your position without pathing. |
| `.mailbox` | `DEV` | Summons an instant remote mailbox interface at your feet. |
| `.bank` | `DEV` | Opens your personal character bank vault from anywhere. |
| `.balance <account> [coins]` | `DEV` | Views or sets Turtle Token donation coin balance. |
| `.copy <character_name>` | `PLAYER` | Copies appearance of another character using Appearance Change Token. |
| `.loothelper [entry]` | `DEV` | Spawns test NPC that generates and rolls loot drops for any creature ID. |

---

## 5. PlayerBot & RandomBot Quick Control

```lua
-- Bootstrap Party Bots (Level 60 + BiS Gear + Enchants + Spells + Prep)
.bot add *
.bot summon *
.bot level * 60
.bot gear * best
.bot enchants *
.bot train *
.bot prep *

-- Bot Whispers & Management
/w <botname> skill                     -- View bot's skills & ratings
/w <botname> skill unlearn <prof>      -- Order bot to unlearn a profession
/w <botname> trainer learn             -- Order bot to learn all spells from nearby trainer
/w <botname> craft <itemlink>          -- Order bot to gather materials & craft item
/w <botname> cheat ?                   -- View active cheats on bot
/w <botname> cheat +health             -- Enable infinite health on bot
/w <botname> cheat +mana               -- Enable infinite mana on bot
/w <botname> cheat +cooldown           -- Remove ability cooldowns on bot
```

---

## 6. Auction House Bot (`.ahbot`) Reference

| Command | Sec | Description |
| :--- | :---: | :--- |
| `.ahbot status [all]` | `MOD` | Displays live auction counts across Alliance, Horde, and Neutral auction houses. |
| `.ahbot reload` | `MOD` | Hot-reloads configuration from `ahbot.conf` without restarting the server. |
| `.ahbot rebuild [all]` | `MOD` | Queues complete market expiration and refills item stock from database. |
| `.ahbot update` | `MOD` | Forces an immediate market check and posting cycle. |
| `.ahbot item <id>` | `MOD` | Inspects baseline AH pricing and posting chance for a specific item. |
| `.ahbot item <id> <val> [c [min [max]]]` | `MOD` | Sets manual pricing override (setting value `0` bans bot from trading item). |
| `.ahbot item <id> reset` | `MOD` | Resets item override back to `ahbot.conf` defaults. |
