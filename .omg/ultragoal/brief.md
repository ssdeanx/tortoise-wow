# Ultragoal Brief: Comprehensive Audit of .dc on Raid Mechanics & Blackwing Lair (BWL) Automation Architecture

## Core Objectives
1. **Deep Architectural Audit of `mod-dungeon-clear` (.dc on)**:
   - Audit the engine lifecycle of `.dc on`: activation, leader election, non-combat/combat trigger ladders, and status publishing.
   - Analyze event-driven raid mechanics in Molten Core (`MoltenCoreEvents.cpp`), Karazhan Crypts / Dragonmaw (`dc_roster.txt`, `DragonmawEvents.cpp`), Zul'Gurub (`ZulGurubEvents.cpp`), and Blackrock Depths (`BlackrockDepthsEvents.cpp`).
   - Catalog the reusable facilities in `modules/mod-dungeon-clear/src/Ai/Dungeon/DungeonClear/Util/` (`DungeonEventExecutor`, `DcPullPlanner`, `DcLeaderSignal`, `DcTargeting`, `DcHazard`, `DcDoorPolicy`, `DcSmartRest`, `DcApproachState`, etc.) and their integration with `mod-playerbots`.
2. **Blackwing Lair (BWL) Engine Integration & Missing Components**:
   - Audit the current state of Blackwing Lair (Map 469) across `mod-dungeon-clear` and `mod-playerbots`.
   - Identify critical gaps (missing Broodlord Lashlayer entry 12017 in `DcBossEntries1121.h` and strategies, lack of `BlackwingLairEvents.cpp`, unhandled mechanics like Razorgore orb/eggs, Vaelastrasz tank suicide/swap, Suppression Room whelp/device push, Drake Wing taunt/LoS rotations, Chromaggus dispels/sand, Nefarian class calls).
   - Formulate the event, trigger, and action architecture for full BWL automation.
3. **Perfection of Blackwing Lair Combat Strategies**:
   - Deeply audit `BlackwingLairDungeonStrategies.cpp` and `.h`, `BlackwingLairDungeonTriggers.h`, and `BlackwingLairDungeonActions.h`.
   - Formulate precise, leak-free, fail-safe strategies for every boss and trash gauntlet.
4. **Comprehensive Documentation**:
   - Update repository reference manuals (`references/02_playerbot_and_raid_automation.md` and `references/06_automated_raid_and_dungeon_operations.md`).

## Architecture Constraints & User Rules
- **ZERO SHELL COMMANDS**: Live server processes are running and the user compiles manually. Never invoke `run_command`.
- **Core Preservation**: Adhere to `AGENTS.md` and native engine contracts.
