# Code & Engineering Conventions

## Core Preservation Principles
- Preserve native core mechanics: Never bypass generic systems, core spell dispatch, or combat formulas with ad-hoc hardcoded scripts.
- Trace complete execution paths: Investigate caller hierarchy, object ownership, state transitions, and supported variants before making modifications.
- Upstream verification: Use CMaNGOS Classic, TBC, and WotLK implementations as structural reference, but verify compatibility with Turtle-WoW 1.18.1 contracts.
- Consult canonical documentation before changes: Review `docs/CORE_SYSTEMS_GUIDE.md` and `docs/CORE_COMPATIBILITY_AUDIT_2026-09-05.md` for baseline invariants.

## Database Protection & Query Lock
- Strict Query Freeze: Never execute unprompted, exploratory, or ad-hoc queries against MariaDB (`tw_world`, `tw_char`, `tw_logon`, `tw_logs`).
- Automatic updates disabled: `Database.AutoUpdate.Enabled = 0` in `mangosd.conf` is intentional. Never re-enable auto-updates.
- Conditional migrations only: Database updates occur strictly when incoming git commits introduce new `.sql` files in `sql/database_updates/`.
- Weekly honor table requirement: `tw_char.character_inventory_copy` must always exist for `HonorMaintenancer::DoMaintenance()`.

## Code Style & Implementation Invariants
- C++ standard: Strict C++17 conformance.
- Resource management: RAII for locks (`std::lock_guard`, `std::unique_lock`), prepared statements, and file descriptors.
- Object lifetimes: Pointers to `Player`, `Creature`, `GameObject`, and `WorldObject` must be checked for validity (`IsInWorld()`, `IsAlive()`, null checks) before access.
- Non-destructive git discipline: Never execute destructive commands (`git checkout -- <file>`, `git restore`, `git reset`, `git clean`) on active files. Use native code editing tools.
- Diagnostics tracking: Log additions and performance instrumentation must be registered and accounted for in `doc/TURTLE_DIAGNOSTICS.md`.
