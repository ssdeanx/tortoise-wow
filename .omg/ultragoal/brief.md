# Ultragoal Brief: Enterprise Reference Library — Eluna Deep Audit & Final Master Polish

## Objective

Elevate the entire Tortoise-WoW reference library (`references/01_*.md` through `05_*.md`, `gm_commands.md`, and `rndbot_commands.md`) to **perfection**:

1. **Eluna Core Deep Audit**: Deeply audit `src/modules/Eluna/` for all C++ bindings, all event registrations, all object methods (`Player`, `Creature`, `GameObject`, `Map`, `Spell`, `Item`), database queries (`WorldDBQuery`, `CharDBQuery`), global methods, and custom Turtle engine hooks.
2. **Volume 05 Overhaul**: Transform `references/05_developer_eluna_and_engine.md` into an exhaustive, production-grade guide with complete event tables, object APIs, hot-reload patterns, and copy-paste Lua script blueprints for server operators.
3. **Cross-Volume Gap Closing**: Exhaustively audit Volumes 01, 02, 03, 04, `gm_commands.md`, and `rndbot_commands.md` to ensure zero missing mechanics, complete command coverage, full link harmony, and flawless usability.
4. **Master Verification**: Run programmatic test suite `scripts/validate_reference_suite.py` ensuring 100% pass across all quality gates.

## Core Architecture Boundaries

- **Server Core**: Turtle-WoW 1.18.1 (Build 7272) CMaNGOS (`src/game/Chat/Chat.cpp`, `Commands.cpp`).
- **Bot Engine**: `modules/mod-playerbots` (`PlayerbotMgr.cpp`, `RandomPlayerbotMgr.cpp`, `ChatFilter.cpp`, `ChatTriggerContext.h`, `ChatActionContext.h`).
- **Dungeon Engine**: `modules/mod-dungeon-clear` (`DungeonClearCommand.cpp`, `mod_dungeon_clear.conf.dist`).
- **Script Engine**: Eluna Lua module (`src/modules/Eluna/`).
- **Local Scripts & Test Suite**: Reside exclusively in `scripts/` (strictly ignored by `.gitignore`).
