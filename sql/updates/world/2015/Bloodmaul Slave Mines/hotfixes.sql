SET @MAPID = 1175;

UPDATE creature SET spawnMask=262 WHERE map=@MAPID;
UPDATE gameobject SET spawnMask=262 WHERE map=@MAPID;
UPDATE creature_template SET exp = 5 WHERE entry IN (SELECT id FROM creature WHERE map=@MAPID);
UPDATE gameobject_template SET displayid=16411 WHERE entry=233282;
UPDATE gameobject_template SET displayid=16411 WHERE entry=212899;
UPDATE creature SET equipment_id = 1 WHERE map=@MAPID AND id IN (84978, 75426, 75427, 75191, 75199, 75196, 75454, 75195, 75198, 75193, 75210, 75272, 74787, 75214, 74349, 75192, 74790, 74356, 74356, 74355, 74357, 74357, 75647, 75646, 75814, 83621, 83622, 83620, 83623, 83624, 74366, 81767, 83823, 83929, 80930, 86946);
UPDATE creature_addon SET auras='' WHERE guid IN (SELECT guid FROM creature c WHERE c.map=1175 AND c.id=75191 AND c.position_x NOT LIKE 2037.37 AND c.position_y NOT LIKE -257.264);
UPDATE creature_addon SET auras='55701' WHERE guid IN (SELECT guid FROM creature c WHERE c.map=1175 AND c.id=75191 AND c.position_x LIKE 2112.63 AND c.position_y LIKE -242.477);
