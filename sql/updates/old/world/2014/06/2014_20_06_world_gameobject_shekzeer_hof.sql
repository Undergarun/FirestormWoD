-- Gameobjects in Shek'zeer room: Mantid Queen Ceil and Empress Chamber
DELETE FROM gameobject WHERE id IN (213276, 213277);
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(213276, 1009, 760, 1, -2478.516, 1013.514, 700.4783, 3.141593, 0, 0, 1, -4.371139E-08, 7200, 255, 1),
(213277, 1009, 760, 1, -2478.769, 1068.421, 573.1021, 1.570796, 0, 0, 1, -4.371139E-08, 7200, 255, 1);