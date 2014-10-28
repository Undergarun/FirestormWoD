INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('101', '1');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('102', '1');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('103', '1');
-- add new level
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('101', '2');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('102', '2');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('103', '2');

INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('101', '4');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('102', '4');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('103', '4');

INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('101', '8');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('102', '8');
INSERT INTO `creature_classlevelstats` (`level`, `class`) VALUES ('103', '8');

-- warrior
-- MoP
UPDATE creature_classlevelstats SET basehp4 = 7987, comment = '6.0 data'  WHERE LEVEL = 85 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 8840, comment = '6.0 data'  WHERE LEVEL = 86 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 9585, comment = '6.0 data'  WHERE LEVEL = 87 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 10174, comment = '6.0 data'  WHERE LEVEL = 88 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 10883, comment = '6.0 data'  WHERE LEVEL = 89 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 11494, comment = '6.0 data'  WHERE LEVEL = 90 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 91 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 92 AND class = 1;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 93 AND class = 1;

-- WoD
UPDATE creature_classlevelstats SET basehp5 = 17503, comment = 'verified' WHERE LEVEL = 90 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 20475, comment = 'verified' WHERE LEVEL = 91 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 22939, comment = 'test me' WHERE LEVEL = 92 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 26964, comment = 'test me' WHERE LEVEL = 93 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 31613, comment = 'verified' WHERE LEVEL = 94 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 37219, comment = 'test me' WHERE LEVEL = 95 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 45530, comment = 'test me' WHERE LEVEL = 96 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 55035, comment = 'test me' WHERE LEVEL = 97 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 65312, comment = 'test me' WHERE LEVEL = 98 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 79501, comment = 'test me' WHERE LEVEL = 99 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 97854, comment = 'test me' WHERE LEVEL = 100 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 101142, comment = 'test me' WHERE LEVEL = 101 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 1, comment = 'no data' WHERE LEVEL = 102 AND class = 1;
UPDATE creature_classlevelstats SET basehp5 = 1, comment = 'no data' WHERE LEVEL = 103 AND class = 1;

-- paladin
-- MoP
UPDATE creature_classlevelstats SET basehp4 = 7987, basemana = 8905, comment = '6.0 data'   WHERE LEVEL = 85 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 8840, basemana = 9128, comment = '6.0 data'   WHERE LEVEL = 86 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 9585, basemana = 9346, comment = '6.0 data'   WHERE LEVEL = 87 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 10174, basemana = 9464, comment = '6.0 data'   WHERE LEVEL = 88 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 10883, basemana = 9692, comment = '6.0 data'   WHERE LEVEL = 89 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 11494, basemana = 9916, comment = '6.0 data'   WHERE LEVEL = 90 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 91 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 92 AND class = 2;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 93 AND class = 2;

-- WoD
UPDATE creature_classlevelstats SET basehp5 = 17503, basemana = 9916, comment = 'test me' WHERE LEVEL = 90 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 20475, basemana = 10084, comment = 'test me' WHERE LEVEL = 91 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'no data' WHERE LEVEL = 92 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 26800, basemana = 10514, comment = 'test me'  WHERE LEVEL = 93 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 31500, basemana = 10748, comment = 'test me'  WHERE LEVEL = 94 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 37200, basemana = 10954, comment = 'test me'  WHERE LEVEL = 95 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 11164, comment = 'no data'  WHERE LEVEL = 96 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 55000, basemana = 11404, comment = 'test me'  WHERE LEVEL = 97 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 11618, comment = 'no data'  WHERE LEVEL = 98 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'no data'  WHERE LEVEL = 99 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 97800, basemana = 12082, comment = 'test me'  WHERE LEVEL = 100 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'no data'  WHERE LEVEL = 101 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 104714, basemana = 12082, comment = 'test me'  WHERE LEVEL = 102 AND class = 2;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'no data'  WHERE LEVEL = 103 AND class = 2;

-- rogue
-- MoP
UPDATE creature_classlevelstats SET basehp4 = 7987, comment = '6.0 data'  WHERE LEVEL = 85 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 8840, comment = '6.0 data'  WHERE LEVEL = 86 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 9585, comment = '6.0 data'  WHERE LEVEL = 87 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 10174, comment = '6.0 data'  WHERE LEVEL = 88 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 10883, comment = '6.0 data'  WHERE LEVEL = 89 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 11494, comment = '6.0 data'  WHERE LEVEL = 90 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 91 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 92 AND class = 4;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 93 AND class = 4;

-- WoD
UPDATE creature_classlevelstats SET basehp5 = 17503, comment = 'test me' WHERE LEVEL = 90 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 20475, comment = 'test me'  WHERE LEVEL = 91 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 22939, comment = 'test me' WHERE LEVEL = 92 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 26964, comment = 'test me' WHERE LEVEL = 93 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 31613, comment = 'test me'  WHERE LEVEL = 94 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 37219, comment = 'test me' WHERE LEVEL = 95 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 45530, comment = 'test me' WHERE LEVEL = 96 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 55035, comment = 'test me' WHERE LEVEL = 97 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 65312, comment = 'test me' WHERE LEVEL = 98 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 79501, comment = 'test me' WHERE LEVEL = 99 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 97854, comment = 'test me' WHERE LEVEL = 100 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 101142, comment = 'test me' WHERE LEVEL = 101 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 1, comment = 'no data' WHERE LEVEL = 102 AND class = 4;
UPDATE creature_classlevelstats SET basehp5 = 1, comment = 'no data' WHERE LEVEL = 103 AND class = 4;

-- Mage
-- MoP
UPDATE creature_classlevelstats SET basehp4 = 6390, basemana = 19394, comment = '6.0 data'  WHERE LEVEL = 85 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 7072, basemana = 19738, comment = '6.0 data'  WHERE LEVEL = 86 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 7668, basemana = 20114, comment = '6.0 data'  WHERE LEVEL = 87 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 8139, basemana = 20464, comment = '6.0 data'  WHERE LEVEL = 88 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 8706, basemana = 20846, comment = '6.0 data'  WHERE LEVEL = 89 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 9194, basemana = 21262, comment = '6.0 data'  WHERE LEVEL = 90 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 91 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 92 AND class = 8;
UPDATE creature_classlevelstats SET basehp4 = 1, comment = '6.0 data'  WHERE LEVEL = 93 AND class = 8;

-- WoD
UPDATE creature_classlevelstats SET basehp5 = 14002, basemana = 21262, comment = 'test me' WHERE LEVEL = 90 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 16380, basemana = 21620, comment = 'test me'  WHERE LEVEL = 91 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'test me' WHERE LEVEL = 92 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'test me' WHERE LEVEL = 93 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'test me'  WHERE LEVEL = 94 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 23204, comment = 'test me' WHERE LEVEL = 95 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 36375, basemana = 23578, comment = 'test me' WHERE LEVEL = 96 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'test me' WHERE LEVEL = 97 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'test me' WHERE LEVEL = 98 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'test me' WHERE LEVEL = 99 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 78283, basemana = 25282, comment = 'test me' WHERE LEVEL = 100 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 80900, basemana = 1, comment = 'test me' WHERE LEVEL = 101 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'no data' WHERE LEVEL = 102 AND class = 8;
UPDATE creature_classlevelstats SET basehp5 = 1, basemana = 1, comment = 'no data' WHERE LEVEL = 103 AND class = 8;

-- base armor gived by Celestalon (use beast lore work too)
UPDATE creature_classlevelstats SET basearmor = 445 WHERE LEVEL = 90 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 504 WHERE LEVEL = 91 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 571 WHERE LEVEL = 92 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 646 WHERE LEVEL = 93 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 731 WHERE LEVEL = 94 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 827 WHERE LEVEL = 95 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 936 WHERE LEVEL = 96 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 1059 WHERE LEVEL = 97 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 1199 WHERE LEVEL = 98 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 1275 WHERE LEVEL = 99 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 1536 WHERE LEVEL = 100 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 1670 WHERE LEVEL = 101 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 1804 WHERE LEVEL = 102 AND class IN (1, 2, 4);
UPDATE creature_classlevelstats SET basearmor = 1938 WHERE LEVEL = 103 AND class IN (1, 2, 4);

UPDATE creature_classlevelstats SET basearmor = 356 WHERE LEVEL = 90 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 403 WHERE LEVEL = 91 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 457 WHERE LEVEL = 92 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 517 WHERE LEVEL = 93 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 585 WHERE LEVEL = 94 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 662 WHERE LEVEL = 95 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 749 WHERE LEVEL = 96 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 847 WHERE LEVEL = 97 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 959 WHERE LEVEL = 98 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 1086 WHERE LEVEL = 99 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 1229 WHERE LEVEL = 100 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 1336 WHERE LEVEL = 101 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 1443 WHERE LEVEL = 102 AND class = 8;
UPDATE creature_classlevelstats SET basearmor = 1550 WHERE LEVEL = 103 AND class = 8;