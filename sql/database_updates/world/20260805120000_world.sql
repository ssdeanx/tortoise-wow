-- ==============================================
-- FILE: shield_specialization_unwire_talents.sql
-- GENERATED: 20260805120000
-- ==============================================
-- Shield Specialization was fixed twice. Our version hung an AuraScript on
-- the five talent ranks (migration 20260731180000). Upstream solves the same
-- thing with a SpellScript on the trigger spell 23602 (migration
-- 20260802171013).
--
-- The merge kept the upstream version, since that is where it is maintained.
-- The script name now points at a SpellScript, so the talent ranks must not
-- carry it any more - otherwise a SpellScript hangs off an aura.
UPDATE `spell_template`
SET `script_name` = ''
WHERE `entry` IN (12298, 12724, 12725, 12726, 12727)
  AND `script_name` = 'spell_warrior_shield_specialization';
