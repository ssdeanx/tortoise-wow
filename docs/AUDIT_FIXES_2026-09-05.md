# System audit: corrections and remaining evidence gaps

Date: 2026-09-05. Local working changes on `mantech-turtle` above
`b2d5a8549194f7ffa38e324dcb7a82ccc0ba2132`; not a new committed revision.
Production reads were sequential SELECTs, not a transaction-consistent snapshot.
No production SQL, binary/config overwrite, server start, or server smoke test
was performed. User shutdown confirmation is required before deployment.

## Local corrections

1. **A2 — Deferred bot AI timing.** Map admission now snapshots the native
   elapsed clock without consuming it. Not-due work consumes its sample after
   the module advances its delay; admitted, valid work consumes on dispatch.
   Budget omissions and stale requests retain elapsed time. Existing elapsed
   caps, round-robin/count/time budgets and transition checks remain. The new
   test reproduces the old lost-delay case using extracted native functions.
2. **A3 — Stale generic action failures.** Removed the per-engine failure cache
   which could skip prerequisites and suppress an action after range, resources
   or other conditions recovered. Native usefulness, prerequisites, possible
   checks, execution, alternatives and continuers remain. Map budgets and
   path-specific retry controls remain. The four `FailedActionRetry*`/
   `FailedActionCache*` settings are retired and ignored; existing configs do
   not need replacement. Generic-cache telemetry getters return zero because
   that cache no longer exists, preserving the existing log field schema.
3. **A4 — Stale spell capability.** Removed the second spell-availability cache.
   The native player spell map already provides a fast hash lookup with removed
   and disabled-state checks. Pet spells remain queried live. Learn/unlearn,
   state changes preserving map size, pet replacement and unknown IDs are
   covered by the native-fragment regression test. No new revision/cache API.
4. **Q1 — Quest item updates could corrupt another quest's client fields.**
   `SendQuestUpdateAddItem` wrote a packed creature/GO count to
   `slot + objective_count`, including beyond the last quest slot. Item progress
   is already updated in `ItemAddedQuestCheck`, persisted in `m_itemcount`, and
   notified by `SMSG_QUESTUPDATE_ADD_ITEM`. Removed the incorrect field write;
   retained that native packet and persistence path. This also avoids treating
   large item totals as six-bit kill counts. The extracted-handler test covers
   item-only/mixed objectives, all four item indices, repeated changes, final
   and missing quest slots, and totals over 63. Reference cross-check:
   [CMaNGOS Classic Player.cpp](https://github.com/cmangos/mangos-classic/blob/master/src/game/Entities/Player.cpp),
   `SendQuestUpdateAddItem` (read 2026-09-05), likewise sends the item notification
   without modifying packed creature/GO fields. This is an inherited source
   defect, not evidence the recent architecture introduced it.
5. **S1 — Four custom aura types were outside the native dispatch range.**
   Registered types 227–230 in both apply/remove and proc tables and expanded
   the native per-type aura lists. The actual DBC and matching source spell
   descriptions establish these contracts: 58115 adds 3% attacking rage;
   30968 reduces cooking cast time by 50%; 58189/58190 add school-filtered
   periodic damage (1%/2% Shadow); 46023/46024 reduce physical chain damage by
   40%. Calculations use existing `GetTotalAuraMultiplier*`, spell/skill index,
   `RewardRage`, damage-done/taken and cast-time paths. No spell-ID exception,
   new spell loop, DB replacement or client-file change. The profession index
   is not queried when the caster has no skill-cast aura; duplicate skill
   rows do not multiply an aura twice.
6. **S2 — Avoidance's old special case penalized the attacker.** Removed the
   `DealDamage` spell-ID check that halved the pet's outgoing AoE damage.
   Incoming AoE reduction already uses native aura 194 (80% in these spells);
   the newly registered aura 230 supplies the separate physical-chain effect.
   Native school filters, per-effect chain counts, damage-class paths and
   ignore-modifier guards remain. This supersedes the earlier assessment that
   the mere presence of an Avoidance special case meant it was implemented.

## Prepared SQL, not applied

`sql/custom/20260905_retired_creature_spell_lists.sql` clears exactly the stale
pointers for 62051, 62177, 62210, 62226, 62227 and 62263, only when their expected
EventAI/empty-script binding and old list ID still match and the list is absent.
The read-only predicate matched all six at preparation time.

The upstream migrations `20260630194548`, `20260710045142` and `20260711131631`
explicitly deleted those lists. The Dragonmaw enrages already use health-triggered
EventAI, the conjurer uses EventAI pull abilities, and the boar/pillager cleanup
also retained or removed native behavior deliberately. Reinstalling old timed
spell lists would not be a faithful repair. The SQL adds no abilities, changes
no stats, and does not delete or replace EventAI. King 59967 is excluded because
no intentional deletion of its referenced list 599670 was established.

`sql/custom/20260905_restore_native_reference_loot.sql` restores **109 exact
source rows**: store 30171=76, 150112=25 and 30559=8. These were recovered from
`sql/base/tw_world_reference_loot_template.sql`, SHA-256
`C87053B4FADA82209ACA03FEA449943ED1A5D71DA7ECA9E9FCEC08D9E3A2D862`.
An earlier line-anchored search missed the tuples in a single-line SQL dump;
that earlier "unrecovered" assessment was incorrect. All 109 item IDs exist,
the three stores are absent, and the read-only SELECT part returns 109 rows.
The guard inserts nothing if any target store exists or any required item is
missing. Probabilities, groups, counts and conditions remain exactly native.
No loot rows were written to production. This repairs the six dangling creature
loot references; it does not certify all encounter loot design.

## Expanded system-wide structural checks

The repeatable read-only collector is `tools/audit-system-data.ps1`; evidence is
under `docs/core-audit/systems-*.json`. It checked the server's actual DBC spell
source (`LoadSpellsFromSql=0`), not the unrelated SQL spell tables. Local and
deployed Spell.dbc hashes matched:
`7D9F83E7A6A3AE642D23597A1A0BC4216FA3D688D00428DD3B5D1F47465AF27F`.

Inventory: 88,841 creature spawn rows, 14,339 templates, 6,708 quests, 6,771
skill-line rows, 1,148 spell chains, 38,257 trainer rows, 393 quest-reward spell
references, 97 events, 1,777 pools and 27,917 DBC spells (all three effect slots).
These supplement the earlier 44-map/38-dungeon-directory encounter inventory.

| Check | Result and qualification |
| --- | --- |
| Quest links and required/source/reward items | No missing references in the checked columns. Not proof of quest completion logic. |
| Quest objective actor IDs | Eight raw flags, all from Snowball Wars I/II (50319/50320). The native `quest_cast_objective` rows define player-class targets; `LoadQuestSpellCastObjectives` runs after `LoadQuests` and deliberately substitutes synthetic IDs. Do not spawn eight fake NPCs. |
| Trainer and quest-reward spell IDs | No absent DBC spell IDs. Loader eligibility, costs, custom effects and all trainer interactions remain separate checks. |
| Event/pool spawn references and route destination maps | No missing references in the checked joins. Not proof of schedule, escort, activation or travel behavior. |
| All template health/damage/level min/max ordering | No reversed ranges. Does not establish intended HP, damage or level balance. |
| Eight loot stores and reference graph | Six missing creature-loot references and one missing pickpocket item; no cycles (the reference store currently has no nested reference edges). Expected loot composition/probabilities are not certified. |
| Skills, creature spells, learning links, chains | One absent skill spell (46530, skill row 6090). No absent chain or checked creature-spell IDs. |
| DBC dispatch range | Originally six out-of-array aura slots across four types. S1/S2 correct native support. Refreshed against the verified 27,917-record DBC at 21:34 UTC: zero out-of-range slots in the local candidate. This is not a live effect test. |
| DBC learning effects | 25 effect-slot references lack a learned spell. No flagged wrappers appear in trainer rows; one item binding exists: Handbook of Lethal Poisons (21302) uses wrapper 18282. Item availability and intended teaching data remain unresolved. |

## Unresolved: do not label these fixed

- Pickpocket store 988079 references absent item 1709; intended replacement or
  deliberate retirement is not established. The bad row comes from upstream
  `20260518050203_world.sql`, alongside sequential items 1707–1713; native
  store 988079 already separately contains 7909, so changing 1709 to 7909 is
  not an evidence-backed repair.
- King 59967 has its own C++ casts/timers but a missing list 599670. That does
  not prove absent mechanics; do not add a second spell loop.
- Additional behavior intended by missing named bindings remains unknown, but
  native quest/portal routes resolve most apparent missing-function flags; see
  the binding follow-up below. Do not install duplicate handlers.
- Missing skill 46530 (skill-line row 6090, skill 1006) and legacy learning
  targets require compatible intended spell definitions. Handbook item 21302
  is actually referenced by creature loot 15339 and reference stores 900748/
  900826; its wrapper 18282 has an absent target 21302 and a valid second target
  52578. Its custom description mentions Corrosive Poison, so substituting the
  stock Classic Deadly Poison spell is not justified. No client DBC rewritten.
- **A1:** global create-packet spline omission remains unchanged. Client packet
  capture/reproduction is needed before touching the earlier crash workaround.
- **A5/A6:** residual scans and instrumentation cost are candidates for measured
  profiling, not demonstrated duplicate combat/rewards or quantified overhead.

## Native binding and movement follow-up

- GO 2010924 is a native type-2 questgiver with quest relation 40583. When its
  named script is absent, `GameObject::Use` still takes the native gossip/
  quest preparation path. Basic quest offering does not need a replacement.
- Creature 2000091 (Mograine's spirit) is friendly, has questgiver flags,
  native quests 20004/20005 and gossip menu 60901. Its rank/level do not prove
  it is an unscripted combat boss. Additional custom script actions are unknown.
- King 59967's selected `npc_kara_kingAI::UpdateAI` owns void zones, pawn
  summons, curse, invulnerability removal and melee. It does not invoke the
  default `ScriptedAI::UpdateAI` spell-list loop. No missing mechanics are
  established by the unused missing list; no second loop was added.
- Fourteen `custom_dungeon_portal` visual spawns were compared with native
  `areatrigger_template`/`areatrigger_teleport`. Twelve have nearby routes
  (GO entries 112917,112918,181581,112912,112911,112940,112941,181580).
  Spawn 4014219 (112920, Scarlet Citadel) and 5001343 (112923, explicitly a
  CoT placeholder) do not. Proximity is not client-trigger or eligibility
  validation; no made-up destination or duplicate GO teleport was installed.
- Compared Turtle create-spline encoding with CMaNGOS Classic and local
  VMaNGOS: the global omission remains broader than native behavior, but the
  encoding comparison does not establish the earlier client crash's cause.
  The saved 08:41 Sep 4 report reproduces `00453885` with unknown symbols; the
  later 11:14 report is a distinct allocator-invalid-block error. Neither is
  a server packet capture. No speculative re-enable or client patch made.

The eight newly fetched upstream commits are reviewed separately in
[UPSTREAM_REVIEW_2026-09-05.md](UPSTREAM_REVIEW_2026-09-05.md). None merged.

## Validation and deployment gate

### Deployment completed 2026-09-05 (supersedes earlier unapplied/staging notes)

With explicit user authorization and world stopped, applied only
`20260905_retired_creature_spell_lists.sql` (6 changed templates) and
`20260905_restore_native_reference_loot.sql` (109 inserted rows) to production
`tw_world`. Post-reads confirmed the six zero pointers with unchanged EventAI/
script bindings and store counts 30171=76, 150112=25, 30559=8. No character,
account, population/config or schema-version changes were needed.

Overwrote `mangosd.exe` and matching `mangosd.pdb` at `\\10.0.0.109\turtle`.
Remote hashes match the local final Release candidate:

- EXE SHA256: `15D6EB57C0477BE645CC754982328976D9EDAD267F37CCD12A9E35017479820B`
- PDB SHA256: `2B503BC6CB424BA3875EE1A4D38BE71D36D8E4D7C1F23D646A92CAFA9982E064`

No realmd/config replacement, server launch, live gameplay or upstream merge
was performed. The user starts world. Earlier DB audit snapshots retain their
pre-repair timestamps; source indexes were regenerated. All unresolved findings
and runtime-validation limits below remain open.

### Stall follow-up: native travel ownership and failure cleanup

The previous live binary's world/map simulation stopped at server-log 13:02:58
on September 5. Watchdog markers identify map/bot-AI phases, not exact blocked
stacks. Malformed socket-header messages occurred over an hour later and do not
establish the initial trigger; the native socket handler already rejects them.
Remote stack inspection was unavailable; no dump was captured.

A local MSVC probe reproduced blocking when assigning an empty future over an
unfinished `std::async` search. Full AI reset now expires the native travel
target and retains the unfinished future; generic, named and quest requests
all refuse replacement while pending. Ready stale results can be discarded
without adopting them. No detached worker, travel engine or skipped map barrier
was introduced. Bot destruction still joins outstanding work: this is not
universal cancellation or a guarantee against all stalls.

`GetPartitions` now returns its existing five-worker permit through a scoped
destructor on both success and exception. Previously an exception after acquire
could permanently lose a slot. Exceptional results are consumed and logged,
then leave PREPARE through the native retry state instead of propagating through
the map update. No evidence proves five exceptions occurred in the live run.

`TravelFutureLifecycleTest` compiles native fragments and exercises unfinished
reset, completed-result discard, 12 consecutive search failures, successful
release, sixth-worker blocking/wakeup, failed-result cleanup and successful
retry. Static guards cover all three request entry points and reset invalidation.
The updated Release and AddressSanitizer suites each pass 21/21 tests. The
Windows x64 Release world binary compiled successfully; no server was launched.

The earlier Release architecture suite and AddressSanitizer suite both had
**20/20 passing tests** after S1/S2. They include native-fragment tests and static
integration guards, not a complete live server. The final Windows x64 Release
build succeeded after all code changes; no server executable was run. Build
metadata reports an archived/unknown Git version in this checkout, so identify
the candidate by SHA-256 and its local manifest, not the embedded Git string.

`NativeCustomAuraTest` compiles extracted native calculations and multiplier
helpers. It covers rage direction/removal/rate, cooking vs other skills,
duplicate skill rows, instant casts, DOT vs direct damage/schools, non-unit
sources, physical vs magical chain effects, ordinary melee and aura removal.
Dispatch guards require all 231 entries in both tables. Fixture aura lists are
not the full live application/removal, persistence, proc or combat engine.

No boss fights, wipe/reset sequences, instance saves, PvP matches, all class
spell interactions, escort completions or 6,000-bot gameplay validation ran for
this candidate. Removing stale caches improves correctness but can change the
number of action predicates evaluated; no CPU/latency improvement percentage is
claimed. Ordinary native combat, quest completion, rewards, instance/event and
transport execution are not replaced by these fixes.

**The static audit pass and these fixes are not a completed all-mechanics audit.**
Do not ask the user to stop production on the claim that every finding is fixed.
Stage locally and retain unresolved items explicitly until compatible data or
controlled client/runtime evidence establishes the appropriate correction.

### Heap-corruption and Southshore travel follow-up

The September 5 crash was not inferred from a console message. The captured
minidump reports `0xc0000374` heap corruption and resolves through
`NamedObjectContext<UntypedValue>::Erase`, `AiObjectContext::ClearExpiredValues`
and `PlayerbotAI::CleanupExpiredValuesIfDue`. The unsafe manager-side direct
erase has been replaced with an atomic cleanup request serviced under the bot's
existing update mutex. Context map create/read/reset/update/erase operations are
synchronized, and expired-value selection/deletion is atomic with respect to
that map.

Southshore trace records separately showed bots reaching the correct service
area but rejecting a cached NPC result. The taxi adapter now obtains the current
interactable flight master through the existing core grid query and retains all
native source-node, endpoint, faction, discovery and activation validation.
There are no Southshore coordinates, spawn GUIDs, forced taxi masks or
anti-cheat exceptions in the fix.

The updated normal and AddressSanitizer suites pass 26/26; the optimized Windows
Release binary links. No SQL migration is required. Runtime proof remains the
next production run: no recurrence of heap corruption and no repeated
approach/abandon loop at Southshore.

With world and login offline, Office-PC directly overwrote the production
`mangosd.exe`, matching PDB and the pre-trace 6,000-bot configuration. Remote
SHA-256 verification returned `464A992044CFC88B1A99CC04DA31D030BCFE78E35F6BA3C4187CA7C8D3349023`
for the EXE, `0CFDD887C40B6E06BACE5E8926BD1DBD2EDB113515106C7DFFD0F2D0697B0499`
for the PDB, and `5928CA1D58AAE62FAFA67382F57FA1835A4D2FAAB3E524A053F095F86DF47352`
for `aiplayerbot.conf`. No database, realmd binary or other production file was
changed, and no server was started.
