DELETE FROM gameobject WHERE map = 1358 AND id = 175185;
INSERT INTO gameobject (id, map, position_x, position_y, position_z, orientation, spawnMask, animprogress, state) VALUES(175185, 1358, 194.2348, -420.225, 110.7608, 3.141593, 262, 255, 1);

UPDATE creature_template SET minlevel = 100, maxlevel = 100 WHERE entry = 76599;
UPDATE creature_template SET minlevel = 102, maxlevel = 102 WHERE entry IN (276599, 876599);