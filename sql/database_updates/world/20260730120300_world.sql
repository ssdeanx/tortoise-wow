-- Stop the PvP trinket from dropping the battleground flag.
-- Every class version casts spell 52317, which applied physical school
-- immunity for 1 ms together with SPELL_ATTR_EX_DISPEL_AURAS_ON_IMMUNITY.
-- The flag aura has school 0, counts as negative and carries no
-- UNAFFECTED_BY_INVULNERABILITY, so it was stripped along with everything else.

-- PvP trinket ("Insignia of the ...") no longer drops the battleground flag.
--
-- Apply to the WORLD database. Spells load at startup, so this needs a restart;
-- there is no reload command for spell_template.
--
-- Symptom
--   Using the class PvP trinket while carrying the WSG flag dropped the flag.
--
-- Cause
--   Every class version of the trinket casts spell 52317 "Insignia", whose
--   tooltip promises "Dispels all types of crowd control effects". Turtle
--   implements that the usual way - apply an immunity for 1 ms with
--   SPELL_ATTR_EX_DISPEL_AURAS_ON_IMMUNITY set, so applying it strips matching
--   auras and then expires immediately. The spell carried three effects:
--
--     effect1  school immunity, mask 126 (all magic schools)
--     effect2  mechanic immunity mask 545341011
--     effect3  school immunity, mask 1 (physical)
--
--   Effect 2 alone already covers the whole tooltip: charm, disorient, fear,
--   root, sleep, snare, stun, freeze, knockout, polymorph, horror and sapped.
--
--   Effect 3 was the problem. The flag aura (23333 Warsong Flag / 23335
--   Silverwing Flag) has school 0, i.e. school mask 1, and counts as negative
--   because of its periodic trigger effect. Aura::HandleAuraModSchoolImmunity
--   removes every negative aura whose school matches, and the flag has no
--   SPELL_ATTR_UNAFFECTED_BY_INVULNERABILITY to protect it. Removing the flag
--   aura runs HandleAuraModEffectImmunity, which calls EventPlayerDroppedFlag.
--
--   Effect 1 cannot reach the flag: mask 126 does not include physical.
--
-- Effect
--   Crowd control removal is unchanged - that is effect 2. The trinket no
--   longer removes physical non-control auras such as bleeds, which matches
--   the tooltip. Divine Shield and Ice Block still drop the flag, as intended:
--   they carry SPELL_ATTR_EX2_DAMAGE_REDUCED_SHIELD and go through the
--   dedicated flag branch instead.

UPDATE `spell_template`
SET `effect3` = 0,
    `effectApplyAuraName3` = 0,
    `effectMiscValue3` = 0,
    `effectImplicitTargetA3` = 0
WHERE `entry` = 52317;
