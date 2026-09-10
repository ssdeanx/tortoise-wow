# Core compatibility and encounter inventory audit

Date: 2026-09-05. Source: `mantech-turtle`, `b2d5a8549194f7ffa38e324dcb7a82ccc0ba2132`. Database: read-only `tw_world` snapshot captured 19:53:42 UTC; sequential queries, not an atomic snapshot. No account/character data was collected.

## Outcome and scope

**Follow-up status:** the baseline observations below are retained as history.
The local corrections and expanded data checks are recorded in
[AUDIT_FIXES_2026-09-05.md](AUDIT_FIXES_2026-09-05.md). A2, A3 and A4 have local
corrections; a quest-item field corruption was also corrected. Six of seven
missing spell-list pointers have a guarded, unapplied SQL cleanup. This does
not close the unresolved content/protocol findings or certify every mechanic.

The inspected architecture paths do **not** show two complete AI/combat engines running side by side. Native creature AI, spell handling, instance hooks and door/event helpers remain underneath the new scheduling. Boss scripts do not require individual "new architecture" ports to participate.

However, there are confirmed compatibility changes and unresolved timing/cache risks. There are also concrete content-reference gaps in the database. A running server and successful helper tests are not sufficient to certify those boundaries.

This audit combines:

- Lexical indexing and hashing of **2,476 source/header files** across game, scripts, shared code and modules, including **280 files in all 38 dungeon source directories**. This is not a claim that every line was manually semantically reviewed.
- **1,231 literal legacy script registrations**, with file/build/loader candidates. Typed/dynamic registrations and runtime Lua require separate inspection.
- All **14,339 creature templates**, **11,582 map/template references** across all four spawn-entry slots, and all **70 map definitions**.
- A **1,520-row encounter matrix** covering stored creature-template references on all dungeon/raid maps plus rank-3 creatures on other maps. Rank 3 is a candidate filter, not a complete or accurate boss taxonomy.
- All **44 database dungeon/raid map definitions**, including three with no stored creature or GO rows: CashTest (29), Old Scarlet Citadel (44), Frostmane Retreat (806). Empty test/obsolete-looking maps are not automatically missing production content.
- Targeted native-code and ownership review, extending the previous architecture audit at `3a0d2d6c`. The only source changes from that revision to this baseline are the trainer correction and its test integration, not another architecture revision.

Full per-map counts and all 38 directories are in [COVERAGE.md](core-audit/COVERAGE.md). Per-creature data, registration evidence and review flags are in [encounter-creature-matrix.json](core-audit/encounter-creature-matrix.json). The durable entry-point/contract reference is [CORE_SYSTEMS_GUIDE.md](CORE_SYSTEMS_GUIDE.md).

**Not performed:** live boss fights, door/event progression, expected-content reconciliation against an authoritative design list, all spell validity/loot probabilities/reference cycles, all quest/PvP scenarios, packet capture, server race detection or production runtime sampling. This report does not claim every boss/add/trash is present or correct. Dynamic summons, event/pool activation, phases, respawn and client assets can change what is actually visible.

## Confirmed data-reference findings

### D1 — Six creature-loot references have no reference store

| Creature loot store | Missing reference | Named template using the store |
| --- | --- | --- |
| 50112 | 150112 | Snowball (50112) |
| 50112 | 30171 | Snowball (50112) |
| 61376 | 30559 | Livia Strongarm (61376) |
| 61377 | 30559 | Luke Agamand (61377) |
| 61378 | 30559 | Blackthorn Footpad (61378) |
| 61379 | 30559 | Greta Longpike (61379) |

Evidence: [db-loot_gaps.json](core-audit/db-loot_gaps.json), template snapshot, and [LootMgr.cpp](../src/game/LootMgr.cpp), `LootTemplate::Process` at 1341–1346. A missing referenced template is skipped; native startup validation can report it. Those referenced contributions cannot roll through this path. This does not mean every item in the creature's loot store is unavailable, or that every listed creature is currently active.

Corrective direction: recover the intended reference contents or correct the erroneous reference from compatible source data. Do not invent loot, blindly delete entries, or add a runtime fallback that conceals broken content. No SQL was changed in this audit. The audit does not establish when these gaps were introduced.

### D2 — Three encounter templates name nonexistent spell lists

| Map | Creature | Missing `creature_spells.entry` | Interpretation |
| --- | --- | --- | --- |
| 814 | King (59967) | 599670 | `npc_kara_king` has independent C++ casts/timers. This is a real bad reference, not proof the boss has no mechanics. |
| 816 | Dragonmaw Veteran (62051) | 62051 | EventAI with one event row; its configured spell list is absent. Inspect that event and intended abilities before deciding what behavior is missing. |
| 816 | Dragonmaw Marauder (62263) | 62263 | Same distinction: missing list does not erase every possible EventAI/native action. |

Evidence: snapshot/matrix; [CreatureAI.cpp](../src/game/AI/CreatureAI.cpp) `SetSpellsList:183` logs and does not load a nonexistent list; [boss_kings_council.cpp](../src/scripts/dungeons/upper_karazhan_halls/boss_kings_council.cpp) `npc_kara_kingAI:438` supplies its own combat logic.

Corrective direction: reconcile template binding and intended source data, preserving C++ mechanics. No custom fallback spell loop was added.

## Binding questions, not confirmed broken encounters

- **Spirit of Alexandros Mograine**, entry 2000091, map 0, names `npc_alexandros_mograine`. No matching registration was found in the searched C++/Lua source. This remains an unresolved binding; runtime external scripts were not inventoried, and rank 3 does not prove a boss fight.
- **`custom_dungeon_portal`** is named on 11 map/object-entry combinations, including entrance/exit visuals. The type-5 gameobjects have no matching registration in the searched source. Native area-trigger teleport routes or external scripts may provide actual travel. Test both directions and resolve the intended owner before adding another portal implementation.
- **`go_curious_leaf`**, object 2010924 on map 1, likewise lacks a found source registration. Investigate its intended interaction and runtime scripts.
- **Nine dungeon registration candidates lack a found active loader invocation**: Wushoolay, Hazzarah, Gri'lek, Mother Smolderweb, Kormok, Headless Horseman, Azshir, Silver Hand bosses, Sandstalker. Several loader calls are explicitly commented out. None of those literal script names is assigned to a creature template in the captured DB. This is not proof those encounters are missing: alternate native/EventAI implementations or unused source can be intentional. Do not enable all old scripts indiscriminately.
- **21 dungeon source registrations have no matching captured creature/map/GO/event/area-trigger binding.** The source-only candidate list is not a dead-code removal list. It does not resolve dynamic summons, explicit AI construction or external scripts.

See [binding matrix](core-audit/instance-object-binding-matrix.json), [registration index](core-audit/script-registration-index.json), and [source-only candidates](core-audit/source-only-registration-candidates.json).

### False positives resolved

- The first scripts-folder-only scan flagged **52 `generic_spell_ai` references**. Its implementation and registration are in `src/game/AI/GenericSpellAI.cpp`, listed in game CMake and called by ScriptLoader. The scanner was expanded to include core and module sources.
- **231 encounter references name EventAI but have no event rows.** Thirty also name a C++ script, which may take precedence. Of the 201 with no named C++ script, 84 name a spell list and 117 do not. Native EventAI can still run a spell list and melee with an empty event vector. These are review signals, not 231 proven failures.
- There are **zero missing creature-template/map references** among the captured spawn-entry slots. There are **zero nonzero loot IDs without a creature loot store** and **zero reversed health/damage min/max ranges in the encounter matrix**. These limited checks do not prove complete spawns, correct stat balance, valid chance distributions or all loot references; D1 illustrates why store existence alone is insufficient.

## Architecture compatibility findings retained at this baseline

### A1 — Global omission of create-packet movement splines

**Confirmed protocol bypass; gameplay extent unmeasured.** [Object.cpp](../src/game/Objects/Object.cpp):314 calls `BuildMovementUpdate(..., false)` during object creation. The implementation at 431 clears `MOVEFLAG_SPLINE_ENABLED` and omits the spline. This is broader than a GM teleport or one zone. The inspected listener/create path has no corresponding immediate replay of the omitted trajectory; entering visibility during movement can lack that trajectory until a suitable later packet.

The comment associates this with a 1.12.1 client crash, but the audit did not establish that crash's root cause or reproduce a resulting visual fault. Investigate native packet serialization and create-before-movement ordering with a client capture. **Do not blindly restore the field and reintroduce the earlier crash.** Dungeon adds, patrols and open-world movement share this boundary.

### A2 — Budget-deferred bot AI can lose elapsed-delay accounting

**Source-level timing defect candidate; live impact unmeasured.** [Map.cpp](../src/game/Maps/Map.cpp):1427 consumes the elapsed timestamp before idle requests pass the count/time budget. [PlayerbotScripts.cpp](../modules/mod-playerbots/src/playerbot/PlayerbotScripts.cpp) leaves a due delay for `UpdateAI` to consume. A budget-omitted request is discarded without that consumption, but its timestamp has advanced.

Example: remaining delay 150 ms, elapsed 200 ms, due request omitted; next elapsed sample 50 ms can make the old 150 ms delay appear not due and reduce it to 100 ms. Age promotion does not restore the discarded elapsed accounting. A regression should exhaust budgets across alternating long/short ticks and require already-due work to remain due, with time accounted exactly once. This is not proof of a population-specific login stall or duplicate boss AI.

### A3 — Failure backoff can delay newly valid bot actions

**Confirmed behavior change; severity requires gameplay measurements.** [Engine.cpp](../modules/mod-playerbots/src/playerbot/strategy/Engine.cpp):64/76/441 keys retry suppression by action/event/target/result, not every changing LOS/range/resource/cooldown condition. Expiry, success and reset provide invalidation, but transient recovery can wait for a retry deadline. Review safe failure classes and native state invalidation; test movement into range, interrupted casts, combat recovery and loot. It suppresses attempts rather than doubling them.

### A4 — Capability cache uses a fingerprint, not an authoritative revision

**Confirmed limited invalidation.** [PlayerbotAI.cpp](../modules/mod-playerbots/src/playerbot/PlayerbotAI.cpp):4438 checks pet spells live, then caches player spell capability for up to one second using level, spell-map size and free talent points. Changes preserving those values may be stale until expiry. Use native mutation notifications/revision support if appropriate; test learn/unlearn/rank-state changes. Historical wording calling this a spellbook-revision cache was too strong and is corrected in the diagnostic ledger.

### A5 — Extra scans remain; no evidence of duplicate rewards/combat

`RefreshRealPlayerActivity` builds classification through two player passes; `UpdatePlayerAI` also forms a player work list. Random-bot candidate sorting happens before bounded work. `PlayerbotHolder::Cleanup` scans outside its session slice. `PlayerbotAI::UpdateAI` revalidates the master twice with transitions between checks. These are residual work or defensive rechecks, not evidence that every bot or boss executes twice. Profile them and preserve invalidation/transition safety before removing them.

### A6 — Logging switches do not remove all diagnostic cost

[ExecutionWatch.h](../src/shared/ExecutionWatch.h) uses clocks/atomics separately from summary logging. Detailed work probes, memory telemetry and ordinary logs have separate gates. No overhead percentage was measured. Keep cleanup/removal tied to the existing [diagnostic inventory](../doc/TURTLE_DIAGNOSTICS.md); disabling one switch is not complete instrumentation removal.

## Replacement versus coexistence: what was checked

| Area | Why retained old/new pieces are not automatically duplication | Remaining boundary |
| --- | --- | --- |
| Map discovery/simulation | Serial/parallel discovery are alternatives; GUID deduplication precedes owner updates; completion has distinct duties | All global/shared state is not proven race-free |
| Native boss and instance logic | Creature AI and `i_data->Update` remain; no independent replacement boss engine found | Discovery, timing, callbacks, visibility and movement still affect encounters |
| Bot AI and sessions | Individual decisions moved to the map scheduler; module player-update hook retains manager work; session packet handling is another stage | Multiple admission/delay layers, A2–A4, reattachment/transfer edge cases |
| Motion | Native motion and deferred async work are different stages; queue/fallback branches are exclusive | Generator/topology lifetime and transition correctness |
| Transports | Manager updates transport container; old map transport-update loop is disabled | Route/template/cache correctness and passenger visibility remain separate |
| Movement viewers | New indexed creature delivery returns before old camera traversal; player broadcasting retains its native path | Index cleanup and the global create-packet omission in A1 |
| DB priority and callbacks | Operations enter one queue; callbacks are consumed, not intentionally replayed | Cross-priority ordering, shutdown and entity lifetime |
| Teleport/auction maintenance | Staged plans/queued completion reuse native final operations | Stale inputs, head-of-line blocking and single operations exceeding budget |
| Bot object packets | Field serialization is deliberately omitted for socketless recipients; current bots consume server state | Future modules expecting those packets need explicit compatibility review |

No explicit async/thread-launch signal was found by the lexical rules in the 280 dungeon source/header files. That is not a whole-program concurrency proof: called native systems and other maps still introduce concurrency.

## Validation and next corrective work

Reran the existing Release architecture suite: **16/16 passed**, including TrainerPurchaseTest, map discovery, ownership helpers, background policy, movement viewers, compression and navmesh lifetime. No new binary was compiled. `ContentHookContract` is largely a source-wiring check; the trainer test mocks native dependencies. Neither proves every encounter is functional in game.

Correction order suggested by this evidence:

1. Resolve D1/D2 against intended content data; do not fabricate missing loot or spells.
2. Reproduce A2 in an isolated regression and correct its time-accounting contract.
3. Capture/test A1 before changing the working client-crash workaround.
4. Resolve script-binding questions through native routes and runtime module inventory; do not enable obsolete scripts blindly.
5. Validate cache invalidation and recovery for A3/A4; profile A5/A6 before optimizing.

Encounter acceptance must include: initial spawn/activation, pull, phase transitions, summons, targeting/casts, wipe/evade/reset, death and loot, doors/next boss, save/reload and repeat entry. Include world-boss helpers, group/pet/bot participation and visibility during movement. Compare behavior and input latency at the intended population. The [coverage inventory](core-audit/COVERAGE.md) is the starting checklist, not a completed gameplay-test sheet.

This turn changed only local audit tooling/documentation/generated evidence. **No gameplay code, production SQL, configs, share files or service state was changed.** No new per-action logging was added. Findings are not silently marked fixed.
