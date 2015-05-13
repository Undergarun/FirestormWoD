DELETE FROM creature WHERE id IN (86175, 86176, 87774, 88569);
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, unit_flags2) VALUES
(86175, 1116, 6941, 7332, 1, 1, 3828.11, -4004.23, 26.962, 4.85708, 600, 2048),
(86176, 1116, 6941, 7332, 1, 1, 3822.26, -4008.29, 26.9413, 4.29155, 600, 2048),
(87774, 1116, 6941, 7333, 1, 1, 5234.106, -3974.46, 11.97974, 0.8028658, 600, 2048),
(88569, 1116, 6941, 7333, 1, 1, 5238.499, -3978.063, 12.79757, 0.8214691, 600, 2048);

# Checked on live DB, no spawn
DELETE FROM gameobject WHERE id = 230609;
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation2, rotation3, spawntimesecs, state) VALUES
(230609, 1116, 6941, 7333, 1, 1, 5234.106, -3974.46, 11.97974, 0.8028658, 0.915214, -0.402967, 300, 1),
(230609, 1116, 6941, 7333, 1, 1, 5238.499, -3978.063, 12.79757, 0.8214691, 0.978756, -0.205029, 300, 1),
(230609, 1116, 6941, 7332, 1, 1, 3828.11, -4004.23, 26.962, 4.85708, 0.65146, -0.756369, 300, 1),
(230609, 1116, 6941, 7332, 1, 1, 3822.26, -4008.29, 26.9413, 4.29155, 0.65146, -0.756369, 300, 1);

UPDATE creature_template SET subname = 'Conquest Quartermaster' WHERE entry = 88569;
UPDATE creature_template SET subname = 'Honor Quartermaster' WHERE entry = 87774;
UPDATE creature_template SET minlevel = 100, maxlevel = 100, faction = 2763 WHERE entry = 86176;