-- Garalon doors
DELETE FROM gameobject WHERE id IN (214634, 212695);

-- Walls towards Mel'jarak
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(214634, 1009, 6297, 6297, 8, 1, -2027.345, 441.7674, 476.6592, 0.9999998, 0, 0, 0, 1, 7200, 255, 1),
(214634, 1009, 6297, 6297, 8, 1, -2029.198, 509.3924, 476.6593, 5.275846, 0, 0, 0, 1, 7200, 255, 1);

-- Walls during Garalon's encounter
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(212695, 1009, 6297, 6297, 8, 1, -1977.59, 458.434, 473.1113, 3.141593, 0, 0, 0, 1, 7200, 255, 0),
(212695, 1009, 6297, 6297, 8, 1, -1977.59, 476.8941, 473.1114, 3.141593, 0, 0, 0, 1, 7200, 255, 0),
(212695, 1009, 6297, 6297, 8, 1, -1977.59, 441.099, 473.1113, 3.141593, 0, 0, 0, 1, 7200, 255, 0),
(212695, 1009, 6297, 6297, 8, 1, -1977.59, 495.2969, 473.1114, 3.141593, 0, 0, 0, 1, 7200, 255, 0),
(212695, 1009, 6297, 6297, 8, 1, -1977.59, 421.4306, 473.1113, 3.141593, 0, 0, 0, 1, 7200, 255, 0),
(212695, 1009, 6297, 6297, 8, 1, -1977.59, 513.1007, 473.1114, 3.141593, 0, 0, 0, 1, 7200, 255, 0),
(212695, 1009, 6297, 6297, 8, 1, -1977.59, 531.3837, 473.1114, 3.141593, 0, 0, 0, 1, 7200, 255, 0);
