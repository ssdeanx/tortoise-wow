-- ==============================================
-- FILE: dagran_moss_agate.sql
-- GENERATED: 20261007161727
-- ==============================================
UPDATE `npc_vendor_template`
SET `maxcount` = 3,
    `incrtime` = 14400
WHERE `entry` = 1500149
AND `item` = 1206;

-- ==============================================
-- FILE: deprecated_redridge_creatures.sql
-- GENERATED: 20261007161727
-- ==============================================
DELETE FROM `creature`
WHERE `guid` IN (
    6163, 6167, 6168, 6169, 6170
    );

-- ==============================================
-- FILE: Florette_DeMure.sql
-- GENERATED: 20261007161727
-- ==============================================
UPDATE `npc_vendor`
SET `maxcount` = 2,
    `incrtime` = 14400
WHERE `entry` = 62150
AND `item` IN (
    3355, 3356
    );

-- ==============================================
-- FILE: starter_throwing_weapons.sql
-- GENERATED: 20261007161727
-- ==============================================
UPDATE `playercreateinfo_item`
SET `amount` = 1
WHERE `itemid` IN (
    2947, 3111
    );

