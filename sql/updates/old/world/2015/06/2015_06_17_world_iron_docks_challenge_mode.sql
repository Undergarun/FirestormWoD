UPDATE creature SET spawnMask = 262 WHERE map = 1195;
UPDATE gameobject SET spawnMask = 262 WHERE map = 1195;

DELETE FROM gameobject WHERE map = 1195 AND id IN (211674, 211989);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `animprogress`, `state`) VALUES
(211674, 1195, 6749.275, -545.1094, 4.925439, 1.052915, 256, 255, 1),
(211989, 1195, 6752.142, -553.7674, 4.925452, 2.104889, 256, 255, 1);

# Now we use the instance portal
DELETE FROM areatrigger_teleport WHERE id = 10284;
UPDATE areatrigger_teleport SET target_position_x = 6746.09, target_position_y = -545.04, target_position_z = 4.89, target_orientation = 4.8989 WHERE id = 10098;

UPDATE gameobject_template SET ScriptName = 'go_iron_docks_instance_portal' WHERE entry = 232490;