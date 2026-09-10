# Eight upstream commits: review, not integration

Reviewed `Shyalya/tortoise-wow`, `playerbots-integration-gh`, through
`ef4ca228ea89a6a70de2c8b8de65d1df1c35a0ae` on 2026-09-05. Range is the eight
first-parent commits after `6be01e53`; the trainer merge also brings its two
ancestors into a non-first-parent log. None was merged/cherry-picked here.

Seven non-merge commits credit **Claude Fable 5.1** as co-author. This documents
attribution, not correctness, test coverage or the amount of human review.

| Commit | Plain-language change | Assessment for our build |
| --- | --- | --- |
| `ef4ca228` | Bots find nearby Arathi Basin banners without the extra ground-level static LOS filter. | Best candidate for a targeted backport. Native banner-ID, spawn/state, interaction distance, control and spell execution checks remain downstream. Does not modify scheduling. Upstream reports supporting bot traces but explicitly says the changed behavior was not measured on its test realm. Requires an AB capture test; not merged by this review. |
| `c45a1883` | Saves failed dungeon-test character logins across restarts. | Do not import unchanged. Includes the author's realm-specific GUID 886 in a tracked blacklist file. A transient timeout can exclude a healthy character permanently; there is no expiry/retry recovery. These GUIDs are not portable between our DBs. |
| `4aa37b20` | Orders Sunken Temple test bosses so Jammal'an precedes the dragons. | Test-roster dependency correction, not new boss mechanics/spawns. Useful if enabling that test module. |
| `6743c07e` | Merges our trainer purchase PR #23. | Already present: current NPCHandler.cpp matches upstream. No second trainer fix needed. |
| `667e1ab8` | Retains 384 leader breadcrumbs instead of 128. | Three times the history/traversal bound, not a proven performance improvement. Author reports this alone did not solve off-trail followers. Optional test module only. |
| `fdf592f4` | Far-behind dungeon followers try the nearest walkable breadcrumb. | Local path-recovery policy; retain our same-map/instance ownership checks if integrated. Per-bot logging map has no removal lifecycle. Author's tests still reported substantial failed joins. |
| `f0f1fd9f` | Explains mana waits and eventually lets dungeon-test parties proceed despite a stuck mana bar. | Behavioral policy, not unequivocal correctness fix: the exemption includes healers and lasts 15 minutes. Static leader/member tracking never erases keys; observations separated by long gaps can count as continuous waiting. Needs lifecycle/reset correction before adoption. |
| `f426cda3` | Limits one dungeon-follow fallback log to every 10 seconds per bot. | Reduces that log's frequency, but introduces a static GUID timestamp map without cleanup. Use existing bounded throttles if adopting. |

## Build relevance and architecture preservation

Our verified cache uses `MODULES=disabled`, `MODULE_MOD_PLAYERBOTS=static`, and
`MODULE_MOD_DUNGEON_CLEAR=default`; the generated Ninja build has no
`DcFollowerActions.cpp` object. Consequently six of the eight commits affect an
optional dungeon-clear/test module **not running in our current binary**.

These eight contain no SQL migrations and do not replace our world/map owner
jobs, packet scheduling, bot admission budgets, human login priority or native
combat loop. A conflict-free merge still would not prove behavioral safety.
The AB change is relevant to the compiled playerbot module; the trainer merge
is already equivalent. Do not describe all eight as missing gameplay fixes.

Native AB path inspected: `BGTactics::atFlag` -> filtered banner selection ->
`CanInteract`/`IsWithinDistInMap` -> native capture `SpellStart`; existing native
capture eligibility remains the authority. This is a review recommendation,
not an AB runtime validation or authorization to update upstream/production.
