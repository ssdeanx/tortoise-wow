# CMaNGOS bot technology integration — September 5, 2026

User-directed integration into the current Turtle working tree, above b6be2a57.
Reference: playerbots-mantech-integration 811d6f1e, cross-checked against the
TBC/Wrath bot trees eb90e45d and their native core taxi implementations.
Existing uncommitted system-audit changes are preserved. No new GitHub commit
or push is implied by this document.

## Changes

1. Adapted the CMaNGOS per-engine failed-execution retry cache, exponential
   delay, size/TTL limits, expiry/eviction and existing telemetry. This is NOT
   the old blanket eligibility cache: usefulness, prerequisites and isPossible
   run first; impossible actions are never cached. Combat, reactions, urgent
   actions, human-led bots and explicit commands are excluded. Position,
   health/power/money changes and reset/transition boundaries invalidate work.
   Zero in either retry setting disables it. Default background retry ceiling
   is two seconds; internal conditions not represented by those invalidations
   can still wait until that deadline. No claim of instantaneous background
   recovery under every condition is made.
2. Unified purposeful and ambient RPG taxi activation through the existing
   MovementAction::UseTaxi adapter. It respects both native endpoint masks,
   faction mount availability and matching flight-master interaction. Only the
   source can be discovered legitimately; unknown destinations are rejected
   before native activation. No anticheat bypass or global taxi cheat enabled.
3. MinimalMove retains a rejected flight leg instead of cutting its route and
   reporting success. Existing nextTeleport timing bounds subsequent attempts.
4. RPG taxi temporary funding is restored on failure as well as success; the
   existing free ambient-flight policy remains. No permanent money grant.
5. Corrected wandermaz to wandermax, restoring the medium-range upper bound.
6. Ported CMaNGOS crowd-control target range allowance and its wait-for-attack
   exception for non-threatening actions; preserved Turtle's native movement,
   spell handling, druid strategies and role logic.
7. Restored CMaNGOS default dungeon strategy wiring to combat/noncombat/dead/
   reaction factories. Existing registered strategies handle instance entry
   and exit. No new boss-update loop or fabricated custom-encounter behavior.
8. Ported configurable creature avoidance: command, stored value, creature-ID
   lookup, proximity trigger, strategy and action registration. The movement
   action delegates to Turtle's existing MoveAwayFromCreature implementation.
   Did not import the reference's redundant multiplier returning a constant 1,
   nor its second movement helper that ignores its creature-entry argument.
   Commands: `avoid creature ?`, `avoid creature <entry>`,
   `avoid creature -<entry>`, `avoid creature reset`, using existing bot-command
   authorization and persistence. An empty list performs no creature scans.
9. Staged EnableGreet=0 to match Classic/TBC ambient behavior. The rest of the
   production bot configuration, including 6,000 min/max and account settings,
   is preserved. Wrath has greetings enabled; this is a deliberate profile
   choice, not a claim of different greeting algorithms.

## Kept and consolidated

Kept Turtle's strategy-set no-op detection, deferred engine resets, map and bot
transition generations, native taxi/transport and environmental-hazard logic,
3D loot validation/stale-corpse handling, custom druid/role and battleground
behavior. Existing host hooks and scheduling still own execution. No second AI
engine, replacement movement simulator, or additional spell-capability cache.
No bot population reduction. No unrelated CMaNGOS expansion content imported.

## Validation and limits

Full Windows Release mangosd build succeeded (4 compiler workers).
24/24 architecture tests passed in Release and 24/24 in the ASAN test build.
New tests extract native retry/cache policy and taxi/failed-route functions;
they cover bounds, expiry, time wrap, context recovery, player/combat exclusions,
known/unknown endpoints, both factions, discovery failure, activation failure,
fund rollback, retained route legs and the existing spell-click fallback.
Source contract checks enforce eligibility-before-retry and no explicit-command
retry gating. These use mocks at native service boundaries, not a live realm.

Live 6,000-bot timing, all dungeon/raid tactics, custom encounters and every
class rotation remain unvalidated for this candidate. CMaNGOS-derived features
are not automatically correct for every Turtle content variant. Follow-up
should observe rejected taxis, repeated-action counts, human casting/loot,
formation movement and encounter activation at the unchanged population.

No SQL migration or realmd change is required. Deploy only after mangosd exits;
the user starts the server. Diagnostic controls and overhead are tracked in
doc/TURTLE_DIAGNOSTICS.md. Deployment status/hashes belong to the local package
manifest, not an assumption that a successful build was installed.

## Follow-up: crowded RPG movement, September 5 evening

The deployed taxi corrections stopped new unknown-node warnings in the sampled
boot. This did not establish that every visible direction reversal was fixed.
Comparison reopened the native CMaNGOS RPG choice/approach implementations;
most of this logic is shared, so copying it would also copy these defects.

- Crowd avoidance previously scanned neighbors for each candidate and skipped
  the check entirely at 200 neighbors. GetTargetCounts now makes one local
  tally, then applies the same randomized 5-15 occupancy threshold by lookup.
  Existing safe-player, bot-activity and human-master exemptions remain. It
  is not a global registry, additional scheduler or population reduction.
- RPG approach now uses the point corrected by ClosestCorrectPoint, rather
  than discarding that correction and moving to the original XYZ. Failed
  correction uses the existing ignore/reset path so another target can be
  selected. Missing targets stop the usefulness check immediately.
- Only creature-type Unit targets enter creature patrol-pause handling.
  Player targets previously underwent an invalid static downcast.
- ClosestCorrectPoint leaves coordinates unchanged for a failed/empty/nonfinite
  nav query and returns false when the map query is unavailable. Its other
  consumer, corpse recovery, already checks the failure result. No replacement
  pathfinder or invented navigation point was added.
- RPG target/action weighted shuffles reuse the core random generator instead
  of reinitializing an identical generator from second-resolution time on each
  choice. Weights and native random selection remain. Candidate inspection
  restores its temporary next-action override, including the debug caller.

Full Release mangosd compilation succeeded. BotRpgMovementTest extracts native
code and covers corrected destinations, failed/empty/nonfinite/missing nav
queries, player vs creature targets, stale targets, movement failure, and 1,000
neighbors with the original eligibility exclusions. The 25-test suite passed
the ordinary and ASAN builds. These boundary mocks are not live Southshore
acceptance or proof that every remaining action/rotation is optimal.

The apparent live freeze during this work was accompanied by AFK bots, while
map updates and world telemetry continued. The user reported activity resumed
after switching from the GM observer to a normal mage. The native nearby-player
check excludes invisible GMs. Its idle policy, WHO cooldown, population,
combat activity and all production configuration are unchanged by this follow-up.
No new diagnostic stream/counter was added; no SQL or realmd update is needed.
The candidate is staged separately, not applied to the running world server.

## September 6 investigation: persistent Southshore traffic

This section supersedes any implication that the previous travel builds solved
the reported behavior. Baseline inspected: Turtle `cc9df979`, local ManTech
playerbots reference `811d6f1e`. This pass changes documentation only. The user
requested investigation and a game plan before another candidate build.

### Evidence and its limits

- The latest server log (`server_2026-09-06_01-21-17.log`) records normalization
  of 270 flight routes to native route preference. The user still reports the
  behavior. Restoring the 3,600 divisor corrected our earlier change but did
  not establish the cause of the continuing traffic. The basic graph search
  and original divisor are also present in the local Classic reference.
- Production `AiPlayerbot.BehaviorTrace=0`. The most recent detailed records
  found in perf.log are from September 5 at 22:11, before this candidate.
  They cannot verify the September 6 build. Earlier flight-master rejections
  are historical evidence, not a diagnosis of the current run.
- Read-only production SQL confirms Southshore node 1272 has five outgoing
  flight edges, including Dun Morogh (path 26), Menethil (272), Refuge Pointe
  (274), Aerie Peak (230), and Chillwind (347). All 270 persisted flight links
  have stored geometry. This excludes a completely absent Southshore flight
  graph; it does not prove runtime DBC eligibility or successful completion.
- Saved character positions showed 542 characters in the rectangle x=-900..-500,
  y=-700..-300, then 159 in a later read. These are changing persistence
  snapshots, not synchronized online counts or in-memory AI state. Population
  at a hub alone cannot distinguish repeat loops from unrelated through traffic.
- The local Classic dispatch function chooses between MovePoint and MovePath.
  Turtle's `MovementAction::DispatchMovement` (MovementActions.cpp:1028) executes
  MovePoint for ordinary ground movement, then falls through to MovePath. The
  latter stops the first spline and launches another. This is a confirmed
  difference and duplicate dispatch, not yet proof of the Southshore loop.
  The relevant Turtle branch predates this week's changes.
- Turtle's compatibility `MotionMaster::MovePath` (MotionMaster.cpp:520) ignores
  moveMode and uses its separate walk argument. The bot caller supplies false
  for walk, even when masterWalking selected a walking mode. The first native
  point generator remains responsible for lifecycle while its spline was
  replaced; speed changes can initialize that generator again. This boundary
  needs executable coverage against the native movement implementation.
- `Object::BuildCreateUpdateBlockForPlayer` (Object.cpp:314) still explicitly
  omits active spline data. The inspected send-create function does not replay
  it immediately. A newcomer may therefore miss an already-running trajectory.
  This is a separate credible explanation for apparent hovering, not proof
  that a taxi is stationary on the server. The earlier client-crash workaround
  must be investigated before changing serialization.
- `MoveToTravelTargetAction` (line 170) clears failure history whenever MoveTo
  returns true. MoveTo can return true while waiting. Therefore an accepted
  command or IsMoving flag does not prove physical progress. Route reuse also
  prefers a cached path based on destination proximity; it does not validate
  that the bot has advanced along that route.
- Turtle explicitly adds wander, fishing and RPG crafting in its live strategy
  profile. The September 5 Classic configuration capture differs; a fresh
  Classic-share read failed with an SMB authentication error. Treat that config
  comparison as dated until refreshed. Native factory defaults add further
  strategies, so config text alone does not describe each bot's active engine.
- The 28 passing tests cover helpers, extracted fragments and mocked native
  service boundaries. BotTaxiIntegrationTest mocks ActivateTaxiPathTo; the
  route policy test checks arithmetic, not a journey through production graph,
  DBC, navigation, motion, visibility and arrival. They do not certify this fix.

### Investigation and implementation order

1. **Reproduce the native movement discrepancy locally.** Exercise actual
   DispatchMovement with ordinary running, walking behind a player, water,
   one-point/empty/clipped paths, hazards and interruptions. Count movement
   launches and check generator/spline ownership and speed. Establish the
   intended single-dispatch behavior from Classic using Turtle's native
   MoveOptions and lifecycle, preserving legitimate hazard avoidance.
2. **Replay real route decisions outside production.** Use read-only snapshots
   of Turtle's graph and relevant taxi/terrain inputs. Test Southshore to
   Ironforge/Coldridge, Menethil, Refuge Pointe and the reverse directions;
   vary faction, learned nodes, travel budget and party identity. Record the
   destination purpose, node sequence, rejected edges, costs and selected
   taxi legs. Compare the existing search with an independent shortest-path
   oracle; its walking-distance heuristic can overestimate cheap taxi paths
   and is also present in Classic, so this is not a Turtle-only explanation.
   Include cached path reuse, shortcutting, clipping and arrival checks.
3. **Capture complete, bounded journeys.** Extend/reuse the existing temporary
   trace to retain a sampled bot after it leaves the hub, with a journey ID,
   target purpose/entry, party leader, active movement strategy, route/leg ID,
   actual position change, spline ID/progress, and taxi acceptance/rejection
   reason. Prioritize state changes over repetitive generic action attempts.
   Sample bots arriving, leaving, waiting and visibly hovering. Compare normal
   observer, invisible GM, and no-player activity transitions. The trace must
   expire automatically and remain owner-thread safe. Activation is a later
   coordinated runtime step; no live configuration was edited by this audit.
4. **Resolve the remaining behavior from that evidence.** Check competing RPG,
   travel and group-follow goals, leader waits and follower restrictions;
   flightmaster range/faction/known-node checks; native flight advancement,
   finalization and dismount; repeated target expiry/reselection; destination
   weighting and maintenance teleports that replenish the hub. For hovering,
   compare server spline progress with what a newly arriving client receives.
   Fix the owning native boundary for each demonstrated defect. Review recent
   cost/distribution/retry changes and remove any shown to conflict with it.
5. **Validate and package the resulting corrections together.** Require complete
   outbound/return trips and measurable progress, correct flight completion,
   purposeful group behavior and no unexplained repeated hub-to-hub loop.
   Test at Southshore plus other faction flight hubs, with the requested 6,000
   population, human casting/looting and GM map changes. Keep custom travel,
   boats, dungeon followers and ordinary player taxis in the regression set.
   Build/tests are prerequisites; runtime acceptance is a separate result.
   Record which journeys pass and which remain unresolved before calling the
   behavior fixed. Retire the bounded diagnostics after acceptance.

No account deletion, population reduction or speculative production SQL is
justified by this evidence. A production replacement still follows the user's
shutdown confirmation; server startup remains the user's action.

## 2026-09-06: bot reset exposed a provisioning crash

Both `crash_20260906_014310.dmp` and `crash_20260906_015638.dmp`
contain the same C++ exception and matching stack-address candidates, resolved
against the deployed binary's PDB to `std::_Throw_future_error2` and
`RandomPlayerbotFactory::CreateRandomBots` at the account-future `wait()`
(RVA `0x631b17`, pre-fix line 1038). These were minidump stack-memory candidates,
not a debugger unwind. The native source and isolated reproduction establish
the invalid-state defect independently: the tail account futures were consumed
by `get()` and then reused by the later character-save phase. The reference
Classic factory also reuses the account vector but only `wait()`s, explaining
why adding a consuming drain in Turtle exposed this latent phase coupling.

Correction: clear the completed account window and use the existing separate
character-save vector. Bound saves to eight outstanding tasks, drain with
`get()` and clear before native cache registration/session cleanup. Native
character creation, Turtle race homebind handling, `SaveToDB`, and gameplay AI
are unchanged. Save exceptions are no longer hidden by `wait()`.

`BotCreationLifecycleTest` extracts the two production loops and uses real
futures with mock account/player services. Before the change it reproduced
`future_error: no state`; afterward all 52 scenarios and all 29 architecture
tests passed. Cases include empty phases, partial/full eight-task windows,
1,000/1,001 new accounts, existing accounts, and character-only provisioning.
This does not validate actual SQL persistence or full realm startup.

Read-only follow-up found 9,011 character rows, including Sd, Tim and Yes,
and no pending `bot_delete` event. The user reported successful startup with
playerbots disabled; that skips the failing provisioning path. No additional
account reset, production SQL, or bot-count reduction is required for this
correction. It is separate from the unresolved Southshore movement audit above.

## September 6: movement-owner correction and journey evidence

The user still observes Southshore traffic after bot recreation and the separate
startup-crash correction. Resetting the population has not established a fix.

`BotMovementDispatchTest` reproduced multiple spline launches from one request
using the actual Turtle dispatcher, native MovePath, and native point-generator
initialize/update bodies. The dispatcher now uses exclusive direct-versus-path
branches (as in the Classic reference), retains Turtle's generated hazard
detours, preserves the first clipped route vertex, handles single-point and
empty requests, and passes the explicit native walking argument. It no longer
creates a point generator immediately before overwriting its spline. This
corrects the demonstrated ownership/dispatch defect; it does not by itself
prove the Southshore goal-selection or repeat-trip symptom resolved.

The existing bounded trace now retains sampled GUIDs outside the enrollment
area, takes an AI-owner progress sample at most every five seconds (including
action waits), and records group leader, purpose/goal, leg type, spline progress,
and retries. Generic action spam has a separate five-second throttle. Limits
remain 12 slots, at most 8 records/bot/second, 8,000 records/process and ten
minutes. It stays disabled unless BehaviorTrace is enabled; no live config was
changed during this pass. See the diagnostic ledger for costs/removal sites.

Read-only production graph checks reconfirmed Southshore node 1272's five flight
exits and ground links to surrounding nodes, the inn and spirit healer. This
does not replay per-bot eligibility/route selection, or prove arrivals. The
full-graph/eligibility replay and runtime journey acceptance in the plan above
remain outstanding; no blanket route-weight or destination-policy change was
made based on the screenshot. The 30-test Release suite passed, including the
new native boundary regression and retained-GUID/rate-limit trace tests.

The first enabled-trace deployment then crashed in TraceBehavior itself before
the first loaded bots had initialized movement paths. The dump's faulting
instruction and a newly added full-snapshot regression identified an unchecked
Duration() read from an empty spline. Corrected the diagnostic caller to require
native Initialized(); no gameplay, shared spline API, account or database change.
The test reproduces the old invalid read and passes after the guard. All 31
Release architecture tests passed; see the diagnostic ledger for crash evidence
and the gap in the previous test coverage. Southshore runtime validation remains
outstanding, not inferred from this crash repair.

### September 6 live test with Tim (server log 02-56-13)

Read-only checks confirmed Tim online in map 0 at (-715.9, -511.1), zone
267. The share executable hash was
`3B56DDF0D7E64242619AA57F6ED4989CAFA5C18BF056CBF38EF16310334002A4`.
The process passed the prior trace-crash point; the latest memory sample at
03:07:18 reported 3,341 bots and 7,152.58 MB private memory. No newer dump than
02:46:03 was present during this check. This is startup progress, not a full
6,000-bot stability acceptance. Log times differ from share filesystem times.

The captured new-format trace spans 02:58:17 through 03:07:20 (2,476 records).
It contains 85 `taxi_reject: no matching interactable flight master` records.
The earlier five-minute slice already showed 71 such records across eight
sampled GUIDs; none of those samples showed a taxi activation or taxi=1.
Do not generalize that limited sample to every bot or all taxi departures.

Melidaran provides a concrete sequence: at 03:02:18 it reaches
(-715.15, -512.13, 26.54); repeated flight-master lookup failures follow,
movement retries reach 12, travel enters cooldown, and at 03:03:31/03:03:40
native RPG movement starts toward another Southshore NPC. Thus at least part
of the visible hub-to-NPC traffic follows failed flight attempts, rather than
being explained solely by route crowding or the corrected double dispatch.

Production spawn 15324, Darla Harris (2432), is configured at
(-715.146, -512.134, 26.6793), map 0, stationary. The Southshore taxi node is
14; its configured coordinates are (-711.48, -515.48, 26.11). These data do
not indicate a misplaced spawn. They do not establish Darla's live combat,
life, flags, visibility or interaction state. The current rejection combines
an empty candidate search, native interaction rejection and source-node
mismatch. The trace does not identify which subcheck failed or record the
requested taxi entry. Narrow that native boundary before changing it; do not
bypass interaction checks or claim this is a verified Southshore fix.

No runtime files, configuration, accounts or production DB rows were changed
during these observations. The bounded trace remains configured to expire
automatically; the captured endpoint above precedes that expiry.

The completed sample subsequently ended at 03:08:17: 2,768 records, 85 taxi
lookup rejections, no taxi activation record in this bounded sample. A targeted
diagnostic update now forwards requested path/from/to, probes the nearest raw
flight master after admission, and obtains the exact native interaction reason.
Existing gameplay checks and order were compared against HEAD and are unchanged
after removing the reason annotations. All 32 Release tests and three targeted
ASAN tests passed; the optimized executable/PDB linked at local 05:41. Production
deployment and runtime identification of the failing subcheck remain pending;
this diagnostic build is not a verified fix for the Southshore behavior.

### September 6: proven cached flight-ID mismatch and native refresh

In session `server_2026-09-06_03-45-06.log`, the admitted 03:53 trace captures
Melias, Nerollio, Shalduin and Richaranor at Darla Harris (spawn 15324, entry
2432). Their requested `taxi_path=272 taxi_from=71 taxi_to=5` conflicts with
`npc_node=14 npc_source_match=0`. The raw native check reports
`npc_interactable=1 npc_reject=accepted`, with Darla alive and out of combat.
Thus the source mismatch, not her interaction flags, explains these rejections.

Read-only queries and the actual `data/dbc/TaxiPath.dbc` establish that path
272 is Morgan's Vigil -> Lakeshire; Southshore -> Menethil is path 99. All 270
persisted flight edges mismatch their current DBC endpoint IDs. All 270 have
exactly one native replacement within the generator's 15-yard endpoint radius.
Checking nearest nodes against all 1,839 graph nodes produces exactly the same
270 endpoint pairs, with no duplicate native pairs and no incomplete geometry.
The full native geometry file has holes in unrelated paths 303 and 355; neither
is eligible with the current graph endpoints. These observations do not certify
all custom flights or every world travel route.

The five Southshore exits map as follows (old -> current): Aerie Peak 230 -> 71;
Chillwind 347 -> 162; Ironforge 26 -> 17; Menethil 272 -> 99; Refuge Pointe
274 -> 101. No hardcoded mapping is used in production code.

Provenance: all 270 live `(node_id,to_node_id,object)` triples exactly match
the module's bundled classic `ai_playerbot_travel_nodes.sql`. That file entered
the module tree in Shyalya commit 8415f1b9 and is unchanged in the freshly
fetched bot branch b405b10a. Its blob ID is
`dc860f7f0fea0ad741bcf0cd18b63cf8ffac4120` in both our HEAD and that branch.
The cached-startup omission is also present there and in our CMaNGOS Classic
reference. Penqle's main at 55fdbc0c does not contain this added module; its
separate native PlayerBots implementation is not this travel cache. The bad
triples are inherited data, not IDs generated by our threading changes. This
does not establish when production first imported them or prove that every
past server stall has this cause. Earlier local behavior fixes missed this
underlying data contract.

Correction: the complete-cache startup branch now reuses the existing native
`generateTaxiPaths()` before coverage/route queries. It republishes IDs and
current point geometry through `setPathTo`; partial/full graph generation keeps
its single existing call. It does not reset accounts, force graph SQL writes,
change route preferences again, alter flight-master/anticheat rules, or add a
second travel engine. The generator validates geometry bounds/null holes before
dereferencing and logs one startup summary with generated/corrected/skipped
counts. Missing data is skipped rather than inventing a path or deleting a
custom cached link; native eligibility still applies at use time.

`BotTaxiCacheRefreshTest` extracts the production generator, startup hook,
edge-publication methods and cost setter. Deterministic fixtures cover sparse
IDs, forward/reverse replacement, current geometry, map-aware endpoint selection,
100 repeat passes, stable edge pointers, no cache dirtying, one pass for partial/
full generation, preservation of unrelated walk/boat/portal/spell-click edges,
missing nodes, missing stores and front/interior/back null geometry. This is a
native-boundary regression with mock graph/DBC services, not an in-game flight.
`BotTaxiIntegrationTest` also reproduces the captured 272 rejection and verifies
99 reaches native activation with the source/knowledge checks unchanged.

Optimized mangosd linked at local 06:13; the 33-test Release suite passed.
Final targeted sanitizer checks/deployment are recorded in the diagnostic
ledger. Live acceptance still requires the corrected startup count and admitted
taxi activations/progress; do not equate compilation with cleared Southshore
crowding. Production has not been changed by this build step.
