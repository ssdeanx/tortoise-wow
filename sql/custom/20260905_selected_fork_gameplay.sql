-- Selected Penqle gameplay fixes, reviewed via trikkizerg/tortoise-wow:
-- 41a3bd79e7696a2a457dfb3ba986023c9086c26c
-- 6992814c7b373295e8fc6030391b7821927c3b6b
-- Target: tw_world. Prepared only; NOT applied to production.
-- Repeatable and non-destructive: preserve unexpected custom records.
-- Before deployment, the three conflict SELECTs below must return no rows.

SELECT entry, RewSpellCast AS unexpected_reward
FROM quest_template
WHERE (entry = 40348 AND RewSpellCast NOT IN (45500,51669))
   OR (entry = 40353 AND RewSpellCast NOT IN (45504,47263));
SELECT entry, effectId, SpellFamilyMask AS unexpected_mask
FROM spell_affect
WHERE entry = 45542 AND effectId = 0 AND SpellFamilyMask <> 6599486734339;
SELECT * FROM spell_proc_event
WHERE entry = 46112 AND NOT
    (SchoolMask = 0 AND SpellFamilyName = 11 AND SpellFamilyMask0 = 1125899906842624
     AND SpellFamilyMask1 = 0 AND SpellFamilyMask2 = 0 AND procFlags = 65536
     AND procEx = 524288 AND ppmRate = 0 AND CustomChance = 0 AND Cooldown = 0);

-- Native quest reward casting must cast the LEARN_SPELL wrapper, not the
-- learned combat spell. Verified wrapper -> learned spell in spell_template.
UPDATE quest_template SET RewSpellCast = 51669
WHERE entry = 40348 AND RewSpellCast = 45500
  AND EXISTS (SELECT 1 FROM spell_template WHERE entry = 51669 AND effect1 = 36 AND effectTriggerSpell1 = 45500);
UPDATE quest_template SET RewSpellCast = 47263
WHERE entry = 40353 AND RewSpellCast = 45504
  AND EXISTS (SELECT 1 FROM spell_template WHERE entry = 47263 AND effect1 = 36 AND effectTriggerSpell1 = 45504);

INSERT INTO spell_affect (entry, effectId, SpellFamilyMask)
SELECT 45542, 0, 6599486734339
WHERE NOT EXISTS (SELECT 1 FROM spell_affect WHERE entry = 45542 AND effectId = 0)
  AND EXISTS (SELECT 1 FROM spell_template WHERE entry = 45542 AND effectApplyAuraName1 = 108);

INSERT INTO spell_proc_event
    (entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1,
     SpellFamilyMask2, procFlags, procEx, ppmRate, CustomChance, Cooldown)
SELECT 46112, 0, 11, 1125899906842624, 0, 0, 65536, 524288, 0, 0, 0
WHERE NOT EXISTS (SELECT 1 FROM spell_proc_event WHERE entry = 46112)
  AND EXISTS (SELECT 1 FROM spell_template WHERE entry = 46112 AND effectTriggerSpell1 = 46111);

-- Deployment verification: expect both learned-spell wrappers and both masks.
SELECT entry, RewSpellCast FROM quest_template WHERE entry IN (40348,40353);
SELECT entry, effectId, SpellFamilyMask FROM spell_affect WHERE entry = 45542;
SELECT * FROM spell_proc_event WHERE entry = 46112;
