-- Canonical 1.18.1 moving-transport manifest. Safe to replay.
-- Moving-object transport GUIDs use the template entry, matching the Vanilla
-- client protocol and the working CMaNGOS/vMaNGOS implementations. The period
-- column documents the period calculated from the released 1.18.1 DBC path.
INSERT INTO `transports` (`guid`, `entry`, `name`, `period`) VALUES
(20808,  20808,  'Ratchet and Booty Bay',                         363740),
(20809,  20809,  'Spadowprey Village and Moonhoof Village',      355277),
(176244, 176244, 'Teldrassil and Auberdine',                     316341),
(176231, 176231, 'Menethil Harbor and Theramore Isle',           329159),
(181646, 181646, 'Stormwind and Auberdine',                      234460),
(177233, 177233, 'Forgotten Coast and Feathermoon Stronghold',   316916),
(164871, 164871, 'Orgrimmar and Undercity',                      356175),
(175080, 175080, 'Grom''Gol Base Camp and Orgrimmar',            303309),
(176495, 176495, 'Grom''Gol Base Camp and Undercity',            332878),
(190549, 190549, 'Orgrimmar and Thunder Bluff',                  566367),
(190550, 190550, 'Sparkwater Port and Revantusk Village',        244960),
(190552, 190552, 'Orgrimmar and Kargath',                        373728),
(176250, 176250, 'Alah''Thalas and Auberdine',                   300917)
ON DUPLICATE KEY UPDATE
    `guid` = VALUES(`guid`),
    `name` = VALUES(`name`),
    `period` = VALUES(`period`);

-- Released 1.18.1 TaxiPath ids. These replace obsolete development ids that
-- do not exist in the shipped DBC files.
UPDATE `gameobject_template` SET `data0` = 72  WHERE `entry` = 20808;
UPDATE `gameobject_template` SET `data0` = 348 WHERE `entry` = 20809;
UPDATE `gameobject_template` SET `data0` = 121 WHERE `entry` = 164871;
UPDATE `gameobject_template` SET `data0` = 110 WHERE `entry` = 175080;
UPDATE `gameobject_template` SET `data0` = 116 WHERE `entry` = 176231;
UPDATE `gameobject_template` SET `data0` = 117 WHERE `entry` = 176244;
UPDATE `gameobject_template` SET `data0` = 323 WHERE `entry` = 176250;
UPDATE `gameobject_template` SET `data0` = 120 WHERE `entry` = 176495;
UPDATE `gameobject_template` SET `data0` = 122 WHERE `entry` = 177233;
UPDATE `gameobject_template` SET `data0` = 294 WHERE `entry` = 181646;
UPDATE `gameobject_template` SET `data0` = 295 WHERE `entry` = 190549;
UPDATE `gameobject_template` SET `data0` = 296 WHERE `entry` = 190550;
UPDATE `gameobject_template` SET `data0` = 297 WHERE `entry` = 190552;
