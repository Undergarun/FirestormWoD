-- Garalon legs
UPDATE `creature_template` SET `modelid1` = 42852, `modelid2` = 0, `modelid3` = 0, `modelid4` = 0 WHERE `entry` = 63053;

DELETE FROM `creature_model_info` WHERE (`modelid`=42852);
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES (42852, 15, 12, 2, 0);

-- Garalon 
DELETE FROM `creature_model_info` WHERE (`modelid`=42368);
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`, `gender`, `modelid_other_gender`) VALUES (42368, 10, 0, 2, 0);

-- Other Garalon
UPDATE creature_template SET modelid1 = 45084, modelid2 = 0, modelid3 = 0, modelid4 = 0 WHERE entry = 63191;

DELETE FROM creature_model_info WHERE modelid = 45084;
INSERT INTO creature_model_info VALUE (45084, 8, 24, 2, 0);

-- Vehicle seat for other Garalon
INSERT INTO vehicleseat_dbc VALUES (20004, 8192, 0, 0, 0, 0, 0, NULL);