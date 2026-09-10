# Selected fork integration — 2026-09-05

Scope: selective native fixes, not ancestry-only merging of whole forks. Existing
ManTech scheduling, player login priority, transport simulation and trainer
semantics remain unchanged. No new runtime diagnostics or config keys.

## Integrated into source

| Origin | Change | Adaptation / limits |
|---|---|---|
| Shyalya `ef4ca228ea89a6a70de2c8b8de65d1df1c35a0ae` | Nearby AB banners are not rejected by the ground-level static LOS discovery filter. | Preserves native same-map range checks, banner eligibility and capture spells. No generic LOS or PvP rule bypass. In-game capture remains untested. |
| JonahSimon `e13a01110f634cbe4f868816e1c702ab3a49ed76` | Both path-calculation overloads reject null owners with NOPATH and clear stale points. | Extracted only the core guard, not the unrelated F2 module/comment changes. Ownerless path calculation is still unsupported; this is safe failure, not a new cross-map solver. |
| Ildourol `750a9856e9b6ac964f170ad64e0d936268ce7c96` | Bot channel dispatch respects EnableBroadcasts. | Existing routing and probabilities retained. |
| Melhart9 `224353490029b69719f4966b3580d339ab323a4e` | Reduces repeated pet-existence database polling. | Only the DB fallback is cached for 30 seconds per native value instance; live pet checks keep the original calculated-value cadence. A live pet or explicit value Reset invalidates fallback state. No shared table, scheduler change or busy-bot releveling policy. DB-only changes can remain cached up to 30 seconds. |

## Prepared SQL, not applied

`sql/custom/20260905_selected_fork_gameplay.sql`, from Penqle commits
`41a3bd79e7696a2a457dfb3ba986023c9086c26c` and
`6992814c7b373295e8fc6030391b7821927c3b6b`, reviewed through trikkizerg:

- Tauren/Troll quest rewards cast their existing native learn-spell wrappers
  (51669 -> 45500; 47263 -> 45504), rather than the combat spells directly.
- Adds missing Clearcasting spell-affect and Seismic Strength proc masks.
- Updates only expected old rewards and inserts only missing keys. Does not
  overwrite unexpected custom records. Deployment must stop on preflight
  conflict rows; those SELECTs are validation, not an automatic SQL abort.

Read-only production validation confirmed the wrong old quest reward IDs,
correct wrapper effects/targets, and absence of the proposed mask records.
The SQL was not executed against production or a disposable database in this
integration. Schema/effect inspection does not substitute for quest/proc playtests.

## Excluded from this integration

- Already-present auction, trainer, VMAP, engine-reset and battleground fixes:
  no duplicate imports.
- DungeonClear routes, rosters, longer trails and test-login blacklist: optional
  module absent from the current build. Some changes add unbounded tracking or
  server-specific GUIDs; no benefit to this deployment as-is.
- Tournament/LLM/Eluna modules, dynamic XP, loot-sharing, autonomous marching,
  cosmetic gossip and inventory automation: not selected as required fixes.
- Shared CC maps, old threading and wholesale transport/session replacements:
  incompatible unchanged with current ownership or redundant architecture.
- LFG stub bundle: **held, not approved**. Activates currently dormant bot queue
  paths; native queue ownership/callers and repeated template scans need a
  complete compatible integration, not just replacing empty return values.
- Item-set bundle: **held, not discarded as useless**. Shared spell-completion,
  combo, periodic damage and resurrection changes need coherent binding/data
  integration and class regressions. Not represented as safely merged.
- BG vendor restoration: **held** pending intended item/reputation/cost validation;
  sparse current inventories alone do not prove every proposed item is correct.
- Orphan cleanup, auction-safety removal and forced interaction shortcuts:
  no justified blind application to our current data/architecture.

## Validation and deployment boundary

- Release server build completed with four compiler workers; no server smoke run.
- Release and AddressSanitizer architecture suites: 22/22 passed each.
- New ForkIntegrationTest executes extracted native boundaries for null-owner
  failure, retained valid-owner entry, broadcast gating, DB fallback expiry and
  timer wrap, live pet death/revival, and nearby same-map banner discovery.
- These tests use deterministic stand-ins, not a full live spell/AI/network engine.
- No production SQL, share overwrite, restart, extra runtime logging or bot-count
  change performed. A new binary is a candidate for normal operator-controlled
  deployment, not proof of 6,000-bot performance improvement.
- Pre-existing local audit changes remain preserved separately; this integration
  does not claim to commit or certify them. The packaged build includes the current
  working source, as did the prior deployed audit build.
