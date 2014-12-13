UPDATE creature SET spawnMask = 262 WHERE map = 1358;
UPDATE gameobject SET spawnMask = 262 WHERE map = 1358;

DELETE FROM gameobject_template WHERE entry IN (211674, 211989);
INSERT INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `WDBVerified`) VALUES
(211989, 0, 11638, "Mode défi - Porte de départ (ligne 2.0)", "", "", "", 1375, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19116);
INSERT INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `WDBVerified`) VALUES
(211674, 10, 11554, "Orbe de défi", "", "", "", 0, 262144, 1.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13872, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 19116);

DELETE FROM gameobject WHERE map = 1358 AND id IN (211674, 211989);
INSERT INTO gameobject (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `spawnMask`, `animprogress`, `state`) VALUES
(211674, 1358, 121.5834, -319.0634, 70.95565, 1.913468, 256, 255, 1),
(211989, 1358, 128.9877, -319.5031, 70.95632, 0.008281732, 256, 255, 1);

DELETE FROM gossip_menu_option WHERE menu_id = 13872;
INSERT INTO gossip_menu_option VALUE
(13872, 0, 0, 'Oui, lancer le défi.', 1, 1, 22277, 0, 0, 0, '');

UPDATE gameobject_template SET ScriptName = 'go_challenge_orb' WHERE entry = 211674;