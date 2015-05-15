-- Shek'zeer & Sha of fear spawns
DELETE FROM creature WHERE id IN (62837, 63942);
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs) VALUES
(62837, 1009, 6297, 6297, 760, 1, -2478.769, 1068.421, 573.1021, 4.712389, 604800),
(63942, 1009, 6297, 6297, 760, 1, -2478.769, 1068.421, 573.1021, 4.712389, 7200);