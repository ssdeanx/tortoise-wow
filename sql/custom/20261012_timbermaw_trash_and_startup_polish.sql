-- ============================================================================
-- Timbermaw Hold Trash Script Bindings & Startup Polish
-- Date: 2026-10-12
-- ============================================================================

-- 1. Bind ALL Timbermaw Hold (Map 819) Trash Creatures to C++ Script AI
UPDATE `creature_template` SET `script_name` = 'mob_withermaw_shaman' WHERE `entry` IN (62871, 62872);
UPDATE `creature_template` SET `script_name` = 'mob_withermaw_defiler' WHERE `entry` IN (62870, 62875, 62878, 62879);
UPDATE `creature_template` SET `script_name` = 'mob_withermaw_totemic' WHERE `entry` IN (62873, 62942);
UPDATE `creature_template` SET `script_name` = 'mob_withermaw_ursa' WHERE `entry` IN (62877, 62867, 62876);
UPDATE `creature_template` SET `script_name` = 'mob_vile_skitterer' WHERE `entry` = 62874;
UPDATE `creature_template` SET `script_name` = 'mob_foulheart_hellcaller' WHERE `entry` IN (62884, 62882, 62881, 62883);
UPDATE `creature_template` SET `script_name` = 'mob_son_of_ursol' WHERE `entry` IN (62885, 62880);

-- 2. Fix creature_spells Timer and Probability Errors
UPDATE `creature_spells` SET `delayRepeatMax_1` = 60 WHERE `entry` IN (63065, 63066) AND `delayRepeatMax_1` = 0;
UPDATE `creature_spells` SET `delayRepeatMin_1` = 1, `delayRepeatMax_1` = 2 WHERE `entry` IN (113500, 113510) AND `delayRepeatMin_1` = 2 AND `delayRepeatMax_1` = 1;
UPDATE `creature_spells` SET `probability_2` = 100 WHERE `entry` = 201100 AND `probability_2` = 0;
UPDATE `creature_spells` SET `probability_3` = 100 WHERE `entry` = 201100 AND `probability_3` = 0;
UPDATE `creature_spells` SET `probability_1` = 100 WHERE `entry` = 180133 AND `probability_1` = 0;
UPDATE `creature_spells` SET `probability_2` = 100 WHERE `entry` = 180211 AND `probability_2` = 0;
UPDATE `creature_spells` SET `probability_3` = 100 WHERE `entry` = 180319 AND `probability_3` = 0;

-- 3. Fix creature_equip_template Hand/Item Errors
UPDATE `creature_equip_template` SET `equipentry2` = 0 WHERE `entry` IN (20153, 20184, 20185, 20186) AND `equipentry2` = 4130;
UPDATE `creature_equip_template` SET `equipentry2` = 0 WHERE `entry` = 20162 AND `equipentry2` = 27754;
UPDATE `creature_equip_template` SET `equipentry1` = 0 WHERE `entry` = 20183 AND `equipentry1` = 23505;
UPDATE `creature_equip_template` SET `equipentry2` = 0 WHERE `entry` = 20189 AND `equipentry2` = 18661;
UPDATE `creature_equip_template` SET `equipentry1` = 0 WHERE `entry` = 20195 AND `equipentry1` = 7873;
UPDATE `creature_equip_template` SET `equipentry1` = 0 WHERE `entry` = 20196 AND `equipentry1` = 5085;
UPDATE `creature_equip_template` SET `equipentry1` = 0 WHERE `entry` = 20445 AND `equipentry1` = 4294;
UPDATE `creature_equip_template` SET `equipentry1` = 0 WHERE `entry` = 20464 AND `equipentry1` = 1682;

-- 4. Clean Nonexistent Spells in spell_extra
DELETE FROM `spell_extra` WHERE `entry` IN (30997, 90001, 90002, 90003, 90004, 90005, 90006);


-- 5. Fix Dimensional Portal Trap Spell Error
UPDATE `gameobject_template` SET `type` = 5 WHERE `entry` = 210287 AND `type` = 22;

-- 6. Clean Nonexistent BroadcastText References in npc_text
UPDATE `npc_text` SET `BroadcastTextID0` = 0 WHERE `ID` IN (6230301, 6230206, 6230202, 6230201, 6230107, 6230302, 6244701, 6230099);

-- 7. Clean Nonexistent Custom Spells on item_template
UPDATE `item_template` SET `spellid_2` = 0 WHERE `entry` = 55023 AND `spellid_2` = 90001;
UPDATE `item_template` SET `spellid_2` = 0 WHERE `entry` = 55026 AND `spellid_2` = 90002;
UPDATE `item_template` SET `spellid_2` = 0 WHERE `entry` = 55030 AND `spellid_2` = 90003;
UPDATE `item_template` SET `spellid_3` = 0 WHERE `entry` = 55032 AND `spellid_3` = 90005;
