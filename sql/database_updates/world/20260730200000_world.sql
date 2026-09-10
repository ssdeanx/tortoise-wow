-- Moonglade druid flights: point the gossip scripts at the right taxi paths.
--
-- Symptom
--   Bunthen Plainswind (11798) and Silva Fil'naveth (11800) in Nighthaven offer
--   their flight in the gossip window, but taking it answers "You are too far
--   away from taxi stand" no matter where the druid stands.
--
-- Cause
--   Both are driven from gossip_scripts through SCRIPT_COMMAND_SEND_TAXI_PATH,
--   and the taxi path ids were the vanilla ones, 315 and 316. Turtle added its
--   own flight points, and under this TaxiPath.dbc those two ids mean something
--   else entirely:
--
--     315  Nordanaar, Hyjal      -> Everlook, Winterspring
--     316  Southshore, Hillsbrad -> Ravenshire, Gilneas
--
--   The server therefore measures the distance to a start node in Hyjal or
--   Hillsbrad, which is never within range of Moonglade. Nothing is wrong with
--   the NPCs, the conditions or the gossip menus - only the two path ids.
--
--   Same class of defect as the battleground graveyards: stale vanilla ids
--   against Turtle's reworked DBC files.
--
-- The correct paths, and they are unambiguous - each NPC stands within four
-- yards of the start node of exactly the route its subtitle promises:
--
--     134  Nighthaven, Moonglade (node 62) -> Rut'theran Village   Silva, 4 yd
--     135  Nighthaven, Moonglade (node 63) -> Thunder Bluff        Bunthen, 3 yd
--
-- Note that Sindrayl, the ordinary hippogryph master in Moonglade, is unaffected:
-- he carries the flightmaster flag and sits at node 49, 337 yards away, and goes
-- through the normal taxi window rather than a gossip script.
--
-- Reloading `gossip_scripts` on a busy realm tends to be refused with "DB scripts
-- used currently, please attempt reload later" - the change lands on the next
-- restart.

UPDATE `gossip_scripts` SET `datalong` = 134
WHERE `id` = 4041 AND `command` = 30 AND `datalong` = 315;

UPDATE `gossip_scripts` SET `datalong` = 135
WHERE `id` = 4042 AND `command` = 30 AND `datalong` = 316;
