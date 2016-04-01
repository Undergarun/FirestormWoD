-- Spawn Kri'thik Weapon Rack in Mel'jarak's room
DELETE FROM gameobject WHERE id = 211675 AND map = 1009;
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation3, spawntimesecs, state, isActive) VALUES
(211675, 1009, 6297, 6297, 760, 1, -2152.47, 443.82, 503.57, 0.28, 1, 7200, 1, 1),
(211675, 1009, 6297, 6297, 760, 1, -2153.86, 448.01, 503.57, 0.28, 1, 7200, 1, 1),
(211675, 1009, 6297, 6297, 760, 1, -2152.47, 508.06, 503.57, 6.00, 1, 7200, 1, 1),
(211675, 1009, 6297, 6297, 760, 1, -2153.86, 503.87, 503.57, 6.00, 1, 7200, 1, 1);