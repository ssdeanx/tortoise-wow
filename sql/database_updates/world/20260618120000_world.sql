-- Fix quest_41240_dummy_triger (entry 60053): wrong display model made it visible.
-- Should use Invisible Stalker (328) like all other dummy triggers in this update.
-- Also add NOT_SELECTABLE + NON_ATTACKABLE so players cannot interact with it.
UPDATE `creature_template`
SET `display_id1` = 328,
    `unit_flags` = 4 | 33554432
WHERE `entry` = 60053;

-- Remove the two erroneous static world spawns. The NPC is used only as a
-- KilledMonster() kill-credit template in the Rommath quest script and needs
-- no physical presence in the world.
DELETE FROM `creature` WHERE `id` = 60053;
DELETE FROM `creature_addon` WHERE `guid` IN (332778, 332779);
