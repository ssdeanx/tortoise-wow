-- ==============================================
-- FILE: hellador_equipment_id.sql
-- GENERATED: 20260802190000
-- ==============================================
-- Hellador Swiftluck points at equipment that does not exist.
--
-- creature_template.equipment_id is 62962 - his own entry - and there is no such
-- row in creature_equip_template, so every startup logs "not found in table
-- creature_equip_template, set to no equipment". It never showed before because
-- he had no spawn; placing him in Alah'Thalas made it visible.
--
-- Every other survival trainer carries 0, which is the normal value for a trainer
-- and produces no warning. Nothing changes visually - he has no equipment either
-- way - the startup log simply stops complaining.

UPDATE `creature_template` SET `equipment_id` = 0 WHERE `entry` = 62962;
