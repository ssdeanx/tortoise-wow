-- ==============================================
-- FILE: spell_chain_proc_event_threat_cleanup.sql
-- GENERATED: 20260906190000
-- ==============================================
-- A clean boot logs 45 complaints across `spell_chain`, `spell_proc_event` and
-- `spell_threat`. All 45 were traced through SpellMgr::LoadSpellChains,
-- LoadSpellProcEvents and LoadSpellThreats to find out what actually happens to
-- the row once the warning fires - not just what the message says.

-- ==============================================
-- FILE: spell_chain_dead_rows.sql
-- ==============================================
-- 26 `spell_chain` rows are inert today, each for one of two reasons:
--
-- 19 rows are byte-for-byte identical to what the engine already computes on its
-- own from Talent.dbc / skill_line_ability (SpellMgr.cpp:1629-1665: when a row's
-- rank/prev/first match the auto-derived entry and it carries no req_spell, it is
-- logged "already added ... and non need" and discarded without being used):
--
--     13165 chain: 13165, 14318, 14319, 14320, 14321, 14322, 25296
--     20043 chain: 20043, 20190
--     51346 chain: 51346, 51565, 51566
--     51433 chain: 51433, 51434, 51435
--     52714 chain: 52714, 52715, 52716, 52717
--
-- 7 rows are custom cross-links that never take effect, because the real chain
-- data - already correct - comes from elsewhere and wins over them:
--
--     24858 (Moonkin Form) <-> 45734 (Owlkin Frenzy) claim to be ranks of each
--     other. They are unrelated spells; skill_line_ability shows Moonkin Form's
--     real successor is 51430, and both entries are rejected on load
--     ("expected prev/rank ... by DBC data") while the correct auto-derived
--     entry underneath is left untouched.
--
--     45599/45560/45960 attempt to graft an unrelated talent (45560,
--     "Recurring Shield") onto the Crush/"Zerschmettern" rank line as a fake
--     rank 2. 45599 itself is structurally invalid (rank 1 with a non-zero
--     prev_spell) and is rejected outright; 45560 and 45960 cascade from it.
--     The real Crush chain (1464 -> 8820 -> 11605 -> 45961) is built
--     automatically from skill_line_ability and needs none of this.
--
--     45910/45911 ("Mana Funnel" rank 1/2 by name only) are not actually
--     linked: skill_line_ability shows 45911's real successor is spell 1941,
--     unrelated to 45910. 45911's row is rejected ("expected rank 1 by DBC
--     data") and its real, correct entry is untouched; 45910's own row loads
--     but the engine already flags it "single rank data, so redundant" since
--     nothing points back to it.
--
-- In every case above, deleting the row changes nothing at runtime: it is
-- already being ignored, or the engine already considers it redundant.
DELETE FROM `spell_chain` WHERE `spell_id` IN (
    13165, 14318, 14319, 14320, 14321, 14322, 25296,
    20043, 20190,
    51346, 51565, 51566,
    51433, 51434, 51435,
    52714, 52715, 52716, 52717,
    24858, 45734,
    45599, 45560, 45960,
    45910, 45911
);

-- ==============================================
-- FILE: spell_chain_entry_3035_typo.sql
-- ==============================================
-- Spell 3035 ("Steady Shot" rank 1) is the only rank-1 row in the whole table
-- with first_spell = 0 instead of first_spell = spell_id - every other rank-1
-- row follows the (id, 0, id, 1, 0) pattern:
--
--     Spell 3035 (prev: 0, first: 0, rank: 1, req: 0) listed in `spell_chain`
--     has not existing first rank spell.
--
-- Spell 0 does not exist, so the row is rejected and never enters the chain
-- map. That in turn breaks rank 2:
--
--     Spell 3036 (prev: 3035, first: 3035, rank: 2, req: 0) listed in
--     `spell_chain` has not found previous rank spell in table.
--
-- Fixing the one wrong value resolves both.
UPDATE `spell_chain` SET `first_spell` = 3035
WHERE `spell_id` = 3035 AND `first_spell` = 0;

-- ==============================================
-- FILE: spell_chain_natures_grasp_req.sql
-- ==============================================
-- Talent 16689 (Nature's Grasp rank 1) carries req_spell = 339 (Entangling
-- Roots rank 1), but Talent.dbc's own DependsOnSpell for this talent is 0:
--
--     Talent 16689 (prev: 0, first: 16689, rank: 1, req: 339) listed in
--     `spell_chain` has wrong required spell.
--
-- The mismatch gets the whole row rejected, so rank 1 never enters the chain
-- map at all - which then breaks rank 2's lookup of its previous rank:
--
--     Spell 16810 (prev: 16689, first: 16689, rank: 2, req: 1062) listed in
--     `spell_chain` has not found previous rank spell in table.
--
-- req_spell is read in exactly one place at runtime - the trainer spell list
-- (NPCHandler.cpp) - and Nature's Grasp is not taught by any trainer (0 rows
-- in npc_trainer for any of its 6 ranks); it is talent-point only. The value
-- has no live effect either way. Its own tooltip (confirmed against Turtle's
-- item/spell database) always triggers "Entangling Roots (Rank 1)" regardless
-- of what rank the caster knows, so there was never a rank-matched requirement
-- for this to encode.
--
-- Clearing it to match Talent.dbc lets rank 1 register correctly, which fixes
-- rank 2's lookup and restores normal spellbook rank replacement when a player
-- trains rank 2 and up.
UPDATE `spell_chain` SET `req_spell` = 0
WHERE `spell_id` = 16689 AND `req_spell` = 339;

-- ==============================================
-- FILE: spell_proc_event_dead_custom_ranks.sql
-- ==============================================
-- 12 `spell_proc_event` rows are custom ranks with ppmRate = 0. SpellMgr only
-- allows a non-first rank to carry its own row when it has a rank-specific ppm
-- rate (SpellMgr.cpp: DoSpellProcEvent::IsValidCustomRank); without one the row
-- is rejected on load and never inserted:
--
--     Spell 15335/15336/15337/15338 listed in `spell_proc_event` is not first
--     rank (15270) in chain
--     Spell 12971/12972/12973/12974 listed in `spell_proc_event` is not first
--     rank (12319) in chain
--     Spell 12967/12968/12969/12970 listed in `spell_proc_event` is not first
--     rank (12966) in chain
--
-- The engine's own FillHigherRanks pass then copies the first rank's proc data
-- onto these spells anyway, so the outcome is identical with or without the
-- row - it is dead data.
DELETE FROM `spell_proc_event` WHERE `entry` IN (
    15335, 15336, 15337, 15338,
    12971, 12972, 12973, 12974,
    12967, 12968, 12969, 12970
);

-- ==============================================
-- FILE: spell_threat_redundant_rank.sql
-- ==============================================
-- Spell 25918's `spell_threat` row carries the exact same threat, multiplier
-- and ap_bonus as its rank 1, so the engine's own consistency check already
-- flags it as duplicate data it would have filled in from rank 1 regardless:
--
--     Spell 25918 listed in `spell_threat` as custom rank has same data as
--     Rank 1, so redundant
DELETE FROM `spell_threat` WHERE `entry` = 25918;
