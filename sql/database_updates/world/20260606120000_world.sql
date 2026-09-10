-- Fix throwing-weapon starting items: stackable=1 items given in quantity 100/200
-- caused EQUIP_ERR_INVENTORY_FULL during bot (and player) character creation.
-- Rogues and Troll Warriors should start with exactly 1 throwing weapon.
UPDATE `playercreateinfo_item` SET `amount` = 1 WHERE `itemid` IN (2947, 3111);
