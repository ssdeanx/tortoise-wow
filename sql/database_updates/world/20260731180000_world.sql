-- ==============================================
-- FILE: shield_specialization_rage_per_rank.sql
-- GENERATED: 20260731180000
-- ==============================================
-- Shield Specialization granted one rage on every rank.
--
-- All five ranks trigger the same spell, 23602, and that spell energizes a fixed
-- amount: effect 30, effectBasePoints1 = 9, so ten points. Rage is held in
-- tenths, which makes exactly one rage no matter how many talent points are in
-- it.
--
-- The per rank value sits on the talent itself, in the base points of its second
-- effect - 0/1/2/3/4, meaning 1 to 5 - and was never passed on:
-- HandleProcTriggerSpellAuraProc casts the trigger without base points of its
-- own, so 23602 decides. That cannot be fixed in this table alone, hence the
-- script, which casts 23602 with the rank's amount instead.

UPDATE `spell_template`
SET `script_name` = 'spell_warrior_shield_specialization'
WHERE `entry` IN (12298, 12724, 12725, 12726, 12727);
