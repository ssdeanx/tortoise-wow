-- MANUAL deployment only, with world stopped, against tw_world.
-- No schema/version change. Idempotent; does not install replacement abilities.
-- Upstream deliberately deleted these lists but left the template pointers:
-- 20260630194548_world.sql: 62051, 62263 (health-triggered EventAI enrages).
-- 20260710045142_world.sql: 62177 (EventAI pull abilities).
-- 20260711131631_world.sql: 622100, 62226, 62227 (cleanup / EventAI).
-- Preserve native EventAI and script bindings, and skip any subsequently
-- restored list or altered template. King 59967 is NOT included: its missing
-- list has not been traced to an intentional upstream deletion.

SELECT c.entry, c.name, c.spell_list_id AS retired_list
FROM creature_template c
LEFT JOIN creature_spells s ON s.entry = c.spell_list_id
WHERE c.ai_name = 'EventAI' AND c.script_name = '' AND s.entry IS NULL
  AND ((c.entry IN (62051,62177,62226,62227,62263) AND c.spell_list_id = c.entry)
       OR (c.entry = 62210 AND c.spell_list_id = 622100));

UPDATE creature_template c
LEFT JOIN creature_spells s ON s.entry = c.spell_list_id
SET c.spell_list_id = 0
WHERE c.ai_name = 'EventAI' AND c.script_name = '' AND s.entry IS NULL
  AND ((c.entry IN (62051,62177,62226,62227,62263) AND c.spell_list_id = c.entry)
       OR (c.entry = 62210 AND c.spell_list_id = 622100));

SELECT ROW_COUNT() AS changed_templates;
SELECT entry, name, spell_list_id, ai_name, script_name
FROM creature_template WHERE entry IN (62051,62177,62210,62226,62227,62263);
