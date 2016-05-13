-- Gnimo & Hakmud
DELETE FROM creature WHERE id IN (32638, 32639);

-- Raid portal and meeting stone
DELETE FROM gameobject WHERE id IN (236281, 239325) AND map = 1116;
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation) VALUES
(236281, 1116, 6721, 6892, 1, 1, 8112.458, 848.441, 38.29622, 2.894769),
(239325, 1116, 6721, 6892, 1, 1, 8008.673, 836.9427, 30.71841, 2.180191);