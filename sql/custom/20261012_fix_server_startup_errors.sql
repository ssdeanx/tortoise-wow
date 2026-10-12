-- ==============================================================================
-- 20261012_fix_server_startup_errors.sql
-- Fixes startup database validation errors from server log (900+ errors).
-- Targets: tw_world
-- ==============================================================================

-- 1. Restore official custom boss dialogue to script_texts (Fixes 93 errors)
-- Sourced from migrations: 20260529182210_world.sql, 20260608174707_world.sql, 20260610134807_world.sql
INSERT IGNORE INTO `script_texts` (`entry`, `content_default`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
-- Dragonmaw Retreat (Web Master Torkon, Garlok, Elder Hollowblood, Overlord Blackheart, Slagfist)
(-1999883, 'You won\'t interrupt my plans...', 60520, 1, 0, 0, 'Dragonmaw Retreat - Web Master Torkon - Aggro'),
(-1999884, 'The brood will live on, my work will not end here!', 60521, 1, 0, 0, 'Dragonmaw Retreat - Web Master Torkon - 50% Health'),
(-1999885, 'Pointless...', 60522, 1, 0, 0, 'Dragonmaw Retreat - Web Master Torkon - Death'),
(-1999886, 'I have been tasked to keep our sacred flame, do not test me!', 60492, 1, 0, 0, 'Dragonmaw Retreat - Garlok Flamekeeper - Aggro'),
(-1999887, 'You have no place here!', 60493, 1, 0, 0, 'Dragonmaw Retreat - Garlok Flamekeeper - 50% Health'),
(-1999888, 'My duty... Is failed...', 60494, 1, 0, 0, 'Dragonmaw Retreat - Garlok Flamekeeper - Death'),
(-1999889, 'The destiny of our clan is set in stone, you can not change fate.', 60592, 0, 0, 0, 'Dragonmaw Retreat - Elder Hollowblood - Aggro'),
(-1999890, 'Behold, the power of the elements!', 60593, 0, 0, 0, 'Dragonmaw Retreat - Elder Hollowblood - 50% Health'),
(-1999891, 'My legacy...', 60594, 0, 0, 0, 'Dragonmaw Retreat - Elder Hollowblood - Death'),
(-1999892, 'More Slaves? How fortunate for you to deliver yourself to me!', 60523, 1, 0, 0, 'Dragonmaw Retreat - Overlord Blackheart - Aggro'),
(-1999893, 'Get into order, maggots!', 60524, 1, 0, 0, 'Dragonmaw Retreat - Overlord Blackheart - 50% Health'),
(-1999894, 'Overlord Blackheart begins to laugh maniacally.', 60525, 2, 0, 0, 'Dragonmaw Retreat - Overlord Blackheart - Death'),
(-1999895, 'Unidentified intruder detected.', 60501, 1, 0, 0, 'Dragonmaw Retreat - Slagfist Destroyer - Aggro'),
(-1999896, 'Execute destruction measure 13.', 60502, 1, 0, 0, 'Dragonmaw Retreat - Slagfist Destroyer - 50% Health'),
(-1999897, 'Protocal failure...', 60503, 1, 0, 0, 'Dragonmaw Retreat - Slagfist Destroyer - Death'),
-- Windhorn Canyon (Pathun Duskhide, Ambassador Vortalus, Walgan Bloodcaller, Rotag, Prophet Stormhoof)
(-1999929, 'You shall suffer for your insolence!', 60663, 1, 0, 0, 'Windhorn Canyon - Pathun Duskhide - Aggro'),
(-1999930, 'Feel the wrath of the canyon!', 60664, 1, 0, 0, 'Windhorn Canyon - Pathun Duskhide - 50% Health'),
(-1999931, 'I travel to the great beyond...', 60665, 1, 0, 0, 'Windhorn Canyon - Pathun Duskhide - Death'),
(-1999932, 'Behold the howling wind!', 60676, 1, 0, 0, 'Windhorn Canyon - Ambassador Vortalus - Aggro'),
(-1999933, 'We seek unity...', 60677, 1, 0, 0, 'Windhorn Canyon - Ambassador Vortalus - 50% Health'),
(-1999934, 'Disperse...', 60678, 1, 0, 0, 'Windhorn Canyon - Ambassador Vortalus - Death'),
(-1999935, 'Be destroyed by my hands!', 60666, 1, 0, 0, 'Windhorn Canyon - Walgan Bloodcaller - Aggro'),
(-1999936, 'Enough!', 60667, 1, 0, 0, 'Windhorn Canyon - Walgan Bloodcaller - 50% Health'),
(-1999937, 'This cannot... be...', 60668, 1, 0, 0, 'Windhorn Canyon - Walgan Bloodcaller - Death'),
(-1999940, 'I have returned...', 0, 1, 0, 0, 'Windhorn Canyon - Spirit of Champion Rotag - Spawn'),
(-1999942, 'Our destiny has been foretold!', 60669, 1, 0, 0, 'Windhorn Canyon - Prophet Stormhoof - Aggro'),
(-1999943, 'I have seen the future, and it is your death!', 60670, 1, 0, 0, 'Windhorn Canyon - Prophet Stormhoof - 50% Health'),
(-1999944, 'My sight, has left me!', 60671, 1, 0, 0, 'Windhorn Canyon - Prophet Stormhoof - Death'),
-- Frostmane Hollow (Handler Oboka, Battlemaster Ubukaz, Kan'za the Seer)
(-1999989, 'Tan\'sha, kill them all!', 60709, 1, 0, 0, 'Frostmane Hollow - Handler Oboka - Aggro'),
(-1999990, 'The Frostmane be da strongest, be da fiercest! Dis be our home, you think you can mess wit us?', 60710, 1, 0, 0, 'Frostmane Hollow - Battlemaster Ubukaz - Aggro'),
(-1999991, 'I have seen da future, and you aint in it!', 60711, 1, 0, 0, 'Frostmane Hollow - Kan\'za the Seer - Aggro');

-- 2. Clean up orphan creature_addon rows (Fixes 140 errors)
DELETE FROM `creature_addon` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);

-- 3. Reset invalid equipment_id references to 0 (Fixes 83 errors)
UPDATE `creature_template` SET `equipment_id` = 0
WHERE `equipment_id` != 0 AND `equipment_id` NOT IN (SELECT `entry` FROM `creature_equip_template`);

-- 4. Reset nonexistent spell list references to 0 (Fixes 159 errors)
UPDATE `creature_template` SET `pet_spell_list_id` = 0
WHERE `pet_spell_list_id` != 0 AND `pet_spell_list_id` NOT IN (SELECT DISTINCT `entry` FROM `pet_spell_data`);

UPDATE `creature_template` SET `spell_list_id` = 0
WHERE `spell_list_id` != 0 AND `spell_list_id` NOT IN (SELECT DISTINCT `entry` FROM `creature_spells`);

-- 5. Fix trainer flags (UNIT_NPC_FLAG_TRAINER = 16) for legitimate trainers, and purge wild beast / boss rows from npc_trainer (Fixes 67 errors)
-- Give actual trainers the trainer flag so players can train from them
UPDATE `creature_template` ct
JOIN `npc_trainer` nt ON ct.entry = nt.entry
SET ct.npc_flags = ct.npc_flags | 16
WHERE (ct.npc_flags & 16) = 0 AND (ct.subname LIKE '%Trainer%' OR ct.npc_flags IN (1, 2, 3, 4, 7));

-- Remove leftover trainer rows for creatures with npc_flags = 0 (e.g. The Rake, Shade of Medivh, Infernal)
DELETE nt FROM `npc_trainer` nt
JOIN `creature_template` ct ON nt.entry = ct.entry
WHERE ct.npc_flags = 0;

-- 6. Zero out wander_distance for idle creatures (MovementType = 0) (Fixes 42 errors)
UPDATE `creature` SET `wander_distance` = 0
WHERE `movement_type` = 0 AND `wander_distance` != 0;

-- 7. Fix invalid '0' string in creature_template.auras (Fixes 32 errors)
UPDATE `creature_template` SET `auras` = NULL
WHERE `auras` = '0' OR `auras` = '' OR `auras` = '0 ';

-- 8. Fix inhabit_type 0 on custom Turtle NPCs so they can walk/swim (Fixes 6 errors)
UPDATE `creature_template` SET `inhabit_type` = 3
WHERE `inhabit_type` = 0 AND `entry` IN (33042, 50542, 50543, 62478, 62479, 62480);

-- 9. Remove obsolete spell entries from spell_extra (Fixes 7 errors)
DELETE FROM `spell_extra` WHERE `entry` IN (30997, 90001, 90002, 90003, 90004, 90005, 90006);

-- 10. Karazhan Crypt Entrance Gate (Morgan's Plot, Deadwind Pass, Entry 177203)
-- Fix gate stuck closed on 0:
-- - Set data0 = 1 (startOpen = 1) so it starts open instead of stuck on 0
-- - Set data1 = 0 (removes Lock 1658 requirement)
-- - Clear script_name so players/bots without Karazhan Crypt Key (51356) can open/use it
-- - Set gameobject spawn state = 0 (GO_STATE_ACTIVE = Open)
UPDATE `gameobject_template`
SET `data0` = 1, `data1` = 0, `flags` = 0, `script_name` = ''
WHERE `entry` = 177203;

UPDATE `gameobject`
SET `state` = 0
WHERE `id` = 177203;

-- 11. Karazhan Crypt Internal Event Doors (Pauper's Walk 2006634 & Tomb of the Unrepentant 177312)
-- Remove GO_FLAG_NO_INTERACT (flag 16) so players/bots can interact with and open them freely
UPDATE `gameobject_template`
SET `flags` = 0
WHERE `entry` IN (2006634, 177312);

-- 12. Timbermaw Hold (Map 819) Instance and Boss Script Bindings
-- Bind instance script to map_template for Map 819
UPDATE `map_template`
SET `script_name` = 'instance_timbermaw_hold'
WHERE `entry` = 819;

-- Bind boss C++ scripts
UPDATE `creature_template` SET `script_name` = 'boss_karrsh' WHERE `entry` = 62934;
UPDATE `creature_template` SET `script_name` = 'boss_rotgrowl' WHERE `entry` = 62936;
UPDATE `creature_template` SET `script_name` = 'boss_kodiak' WHERE `entry` = 62937;
UPDATE `creature_template` SET `script_name` = 'boss_ormanos' WHERE `entry` = 62935;
UPDATE `creature_template` SET `script_name` = 'boss_archdruid_kronn' WHERE `entry` = 62938;
UPDATE `creature_template` SET `script_name` = 'boss_loktanag' WHERE `entry` = 2139;
UPDATE `creature_template` SET `script_name` = 'boss_trioch' WHERE `entry` = 62946;
UPDATE `creature_template` SET `script_name` = 'boss_selenaxx' WHERE `entry` = 62940;
UPDATE `creature_template` SET `script_name` = 'boss_partath' WHERE `entry` = 62941;

-- Bind Ursol (62947) & Peroth'arn (60686) stats and scripts
UPDATE `creature_template`
SET `script_name` = 'boss_ursol', `health_min` = 1850000, `health_max` = 1850000, `armor` = 4640
WHERE `entry` = 62947;

UPDATE `creature_template`
SET `script_name` = 'boss_perotharn', `health_min` = 2100000, `health_max` = 2100000, `mana_min` = 120000, `mana_max` = 120000, `armor` = 3790
WHERE `entry` = 60686;

-- Bind brazier interaction script
UPDATE `gameobject_template`
SET `script_name` = 'go_brazier_timbermaw'
WHERE `entry` IN (300602, 300603, 300604, 300605);

-- Ensure Barrier of Ursol starts closed, auto-managed by instance script
UPDATE `gameobject_template`
SET `flags` = 32
WHERE `entry` = 300606;

-- Spawn Ursol (62947) in Deep Maw of Ursoc if not already spawned
INSERT INTO `creature` (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecsmin`, `spawntimesecsmax`, `wander_distance`, `health_percent`, `mana_percent`, `movement_type`, `spawn_flags`)
SELECT 62947, 819, -7845.0, -3900.0, 313.0, 5.5, 7200, 7200, 0, 100, 100, 0, 0
WHERE NOT EXISTS (SELECT 1 FROM `creature` WHERE `id` = 62947 AND `map` = 819);

-- Spawn Peroth'arn (60686) in Nightmare Sanctum if not already spawned
INSERT INTO `creature` (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecsmin`, `spawntimesecsmax`, `wander_distance`, `health_percent`, `mana_percent`, `movement_type`, `spawn_flags`)
SELECT 60686, 819, -7925.0, -3850.0, 315.0, 1.5, 7200, 7200, 0, 100, 100, 0, 0
WHERE NOT EXISTS (SELECT 1 FROM `creature` WHERE `id` = 60686 AND `map` = 819);

-- 13. Timbermaw Hold Raid Teleport Locations
REPLACE INTO `game_tele` (`id`, `position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) VALUES
(941, -8133.95, -3450.47, 224.75, 0.31, 819, 'tmh'),
(942, -8133.95, -3450.47, 224.75, 0.31, 819, 'timbermawraid'),
(943, 4724.58, -4911.79, 134.97, 4.23, 1, 'timbermawportal');

