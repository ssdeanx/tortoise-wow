-- Add taxi nodes for Gazzik's flying machine route.
-- Node 508: Blackstone Island (Gazzik's Workshop)
-- Node 509: Sparkwater Port
-- These are referenced by TaxiPath entries 1619 and 1633 in the DBC.
INSERT IGNORE INTO `taxi_nodes` (`id`, `map_id`, `x`, `y`, `z`, `name`, `mount_creature_id1`, `mount_creature_id2`) VALUES
(508, 1, -570.74, -7849.85, 52.11, 'Blackstone Island Landing Pod', 8450, 8450),
(509, 1,  818.91, -5006.31, 19.91, 'Sparkwater Port Landing Pod',   8450, 8450);
