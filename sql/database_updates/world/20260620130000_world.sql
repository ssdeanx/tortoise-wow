-- Restore player starting zones for Goblin and High Elf.
-- Bot spawn override is handled in RandomPlayerbotFactory::CreateRandomBot via Relocate().
UPDATE `playercreateinfo` SET `map` = 1, `zone` = 5536, `position_x` = -233, `position_y` = -7177, `position_z` = 16.52, `orientation` = 1.76 WHERE `race` = 9;
UPDATE `playercreateinfo` SET `map` = 0, `zone` = 5225, `position_x` = 3212.63, `position_y` = -2501.44, `position_z` = 111.71, `orientation` = 0.79 WHERE `race` = 10;
