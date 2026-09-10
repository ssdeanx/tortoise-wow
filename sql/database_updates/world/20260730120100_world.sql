-- Graveyard coverage for dungeon sub-zones.
-- Turtle splits Scarlet Monastery, Scholomance, Dire Maul and Shadowfang Keep
-- into several zones, but only the main zone was mapped. Derives the missing
-- rows from the ones that exist, so it adds no new WorldSafeLocs references.

-- Graveyard assignments for dungeon zones that have none.
--
-- Apply to the WORLD database, then `reload game_graveyard_zone`.
-- The second half needs tools/add_worldsafelocs_dungeons.py and a restart.
--
-- Turtle splits several instances into multiple *zones* - Scarlet Monastery
-- Library and Graveyard, three Scholomance zones, five Dire Maul wings, three
-- Blackwing Lair zones, six Shadowfang Keep zones - and only the main zone got
-- a game_graveyard_zone row. GetZoneId() at the death position returns the
-- wing, GetClosestGraveYard finds nothing, and RepopAtGraveyard does not
-- teleport: the ghost appears at its own corpse. Real players get no corpse
-- run, and any "resurrect solo in a dungeon" feature silently does nothing.
--
-- Part one: let the wings inherit their main zone's graveyards.

INSERT IGNORE INTO `game_graveyard_zone` (`id`, `ghost_zone`, `faction`)
SELECT g.`id`, q.`target`, g.`faction`
FROM (
    SELECT  209 AS source, 5132 AS target UNION ALL SELECT  209, 5150
    UNION ALL SELECT  209, 5161 UNION ALL SELECT  209, 5169
    UNION ALL SELECT  209, 5173 UNION ALL SELECT  209, 5177   -- Shadowfang Keep
    UNION ALL SELECT  721, 5152 UNION ALL SELECT  721, 5162   -- Gnomeregan
    UNION ALL SELECT  796, 5135 UNION ALL SELECT  796, 5136   -- Scarlet Monastery
    UNION ALL SELECT 2057, 5142 UNION ALL SELECT 2057, 5156
    UNION ALL SELECT 2057, 5165                               -- Scholomance
    UNION ALL SELECT 2557, 5145 UNION ALL SELECT 2557, 5157
    UNION ALL SELECT 2557, 5166 UNION ALL SELECT 2557, 5171
    UNION ALL SELECT 2557, 5175                               -- Dire Maul
    UNION ALL SELECT 2677, 5146 UNION ALL SELECT 2677, 5158
    UNION ALL SELECT 2677, 5167                               -- Blackwing Lair
    UNION ALL SELECT 3428, 5147                               -- Ahn'Qiraj
    UNION ALL SELECT 3457, 5557                               -- Rock of Desolation
    UNION ALL SELECT 2366, 2367                               -- Old Hillsbrad
) q
JOIN `game_graveyard_zone` g ON g.`ghost_zone` = q.`source`;

-- Shadowfang Keep's graveyard was Horde only, so Alliance had none at all -
-- an oddity, every other instance graveyard here is shared. Both sides run it.
UPDATE `game_graveyard_zone` SET `faction` = 0 WHERE `id` = 32;

-- The five Turtle-built dungeons that have no graveyard anywhere on their
-- map are deliberately NOT handled here: they need WorldSafeLocs ids that
-- do not exist in a stock DBC. See sql/tools/graveyards_turtle_dungeons.sql.
