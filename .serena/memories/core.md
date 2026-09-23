# Tortoise-WoW Core Index

## Project Scope
Turtle-WoW 1.18.1 (Build 7272) private server engine fork based on CMaNGOS Classic. Features native C++ PlayerBots integration (~1000 persistent autonomous AI bots), solo-play scaling, custom PvP Honor system, and embedded Eluna Lua scripting engine.

## Top-Level Source Map
- `src/mangosd/`: World server daemon entry point and primary simulation loop (`MainThread`).
- `src/realmd/`: Authentication daemon and realm router service.
- `src/game/`: Core game mechanics: entity models, spell handling, combat formulas, maps, instances, motion, quests.
- `src/shared/`: Cross-cutting utilities, networking layer, database connection wrappers, threading primitives.
- `src/framework/`: Memory allocation wrappers, platform abstractions, and thread models.
- `modules/mod-playerbots/`: Embedded C++ PlayerBots AI engine (bot decision trees, combat strategies, travel nodes, AhBot).
- `modules/mod-dungeon-clear/`: Dungeon clearing AI automation and event hooks.
- `src/modules/Eluna/`: Embedded Lua scripting engine integrating custom gameplay scripts.
- `run/`: Operational runtime working directory containing active configurations (`mangosd.conf`, `realmd.conf`, `aiplayerbot.conf`) and logs (`run/logs/`).
- `sql/`: Database schema definitions and incremental migration updates (`sql/database_updates/world/`, `sql/database_updates/character/`).
- `tests/architecture/`: CTest architectural contract test suite validating native regression fragments and subsystem invariants.
- `docs/`: Reference audit ledgers and subsystem manuals (`docs/CORE_SYSTEMS_GUIDE.md`, `docs/CORE_COMPATIBILITY_AUDIT_2026-09-05.md`).

## Project-Wide Invariants
- Core preservation rule: Retain native MaNGOS systems, generic mechanics, and custom Turtle-WoW semantics. Never introduce bypassing hacks, hardcoded spell scripts, or duplicated parallel subsystems.
- Runtime working directory invariant: The server daemons expect execution from `/home/sam/tortoise-wow/run` (or via systemd user services). Relative paths to `DataDir` (maps, DBCs, vmaps, mmaps) and log output directories depend strictly on this root.
- Strict database query freeze: Do not run unprompted, exploratory, or ad-hoc queries against MariaDB. Automatic schema updates are permanently disabled (`Database.AutoUpdate.Enabled = 0`).
- Remote synchronization pipeline: Always follow the strict linear 9-step pipeline (stop services -> stash -> pull -> restore stash -> diff audit -> conditional migrations -> build -> reset systemd -> restart & verify).

## Subsystem Navigation & Detailed References
- For compiler flags, CMake options, dependencies (Boost, OpenSSL, MariaDB), and systemd service profiles, consult `mem:tech_stack`.
- For systemd lifecycle management, journalctl log inspection, targeted CMake builds, and CTest commands, consult `mem:suggested_commands`.
- For architectural preservation guidelines, coding standards, threading contracts, and database locks, consult `mem:conventions`.
- For the pre-completion verification checklist, compiler warning audits, and runtime smoke testing, consult `mem:task_completion`.
- For PlayerBots AI engine mechanics, travel node generation, strategy bindings, and `ai_playerbot_equip_cache` management, consult `mem:playerbots/core`.
- For MariaDB credentials, schema layout, conditional migration application SOP, and weekly honor table maintenance, consult `mem:database/core`.
