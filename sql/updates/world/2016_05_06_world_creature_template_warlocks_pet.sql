UPDATE `creature_template` SET modelid1 = 63968 WHERE entry = 58965;
UPDATE `creature_template` SET modelid1 = 64802 WHERE entry = 17252;
UPDATE `creature_template` SET modelid1 = 64799 WHERE entry = 416;
DELETE FROM `creature_model_info` WHERE modelid=64802;
DELETE FROM `creature_model_info` WHERE modelid=63968;
DELETE FROM `creature_model_info` WHERE modelid=64799;
INSERT INTO `creature_model_info` (modelid, bounding_radius, combat_reach, gender, modelid_other_gender) VALUES (64802, 1.15, 2.3, 2, 0);
INSERT INTO `creature_model_info` (modelid, bounding_radius, combat_reach, gender, modelid_other_gender) VALUES (63968, 1, 1.5, 1, 0);
INSERT INTO `creature_model_info` (modelid, bounding_radius, combat_reach, gender, modelid_other_gender) VALUES (64799, 0.5, 0.75, 0, 0);