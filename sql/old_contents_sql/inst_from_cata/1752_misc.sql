UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN (52558, 52559, 52560, 52561);
UPDATE `creature_template` SET `lootid`=52558 WHERE `entry`=53772;
UPDATE `creature_template` SET `lootid`=52559 WHERE `entry`=53773;
UPDATE `creature_template` SET `lootid`=52560 WHERE `entry`=53774;
UPDATE `creature_template` SET `lootid`=52561 WHERE `entry`=53775;

DELETE FROM `creature_loot_template` WHERE `entry`=52560;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 69237, 50, 1, 0, 1, 2);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 69815, -50, 1, 0, 18, 66);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71141, -75, 1, 0, 1, 3);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71414, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71415, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71416, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71417, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71418, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71419, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71420, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71421, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71422, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71423, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71424, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71425, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71426, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71617, 15, 1, 0, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71774, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71777, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71778, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71781, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71783, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71784, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52560, 71786, 0, 1, 2, 1, 1);

DELETE FROM `creature_loot_template` WHERE `entry`=52559;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 69237, 50, 1, 0, 1, 2);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 69815, -25, 1, 0, 18, 66);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 70912, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 70991, 0, 1, 4, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 70992, 0, 1, 4, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 70993, 0, 1, 3, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71003, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71004, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71005, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71006, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71007, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71009, 0, 1, 3, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71010, 0, 1, 4, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71011, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71012, 0, 1, 3, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71141, -75, 1, 0, 1, 3);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71775, 0, 1, 3, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71776, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71779, 0, 1, 4, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71780, 0, 1, 4, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71782, 0, 1, 2, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71785, 0, 1, 1, 1, 1);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES (52559, 71787, 0, 1, 3, 1, 1);

UPDATE `creature_loot_template` SET `mincountOrRef`=18, `maxcount`=23 WHERE `entry` IN (52498, 52530, 52571, 53494, 53691, 52558) AND `item`=69815;
UPDATE `creature_loot_template` SET `mincountOrRef`=23, `maxcount`=26 WHERE `entry` IN (53577, 54045, 52560, 53857, 53588, 54079) AND `item`=69815;
UPDATE `creature_loot_template` SET `mincountOrRef`=48, `maxcount`=55 WHERE `entry` IN (53576, 54044, 52559, 53856, 53587, 53979) AND `item`=69815;
UPDATE `creature_loot_template` SET `mincountOrRef`=55, `maxcount`=66 WHERE `entry` IN (53578, 54046, 52561, 53858, 53589, 54080) AND `item`=69815;

REPLACE INTO `creature_loot_currency` (`creature_id`, `CurrencyId1`, `CurrencyId2`, `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3`) VALUES 
(52558, 396, 0, 0, 12000, 0, 0),
(52560, 396, 0, 0, 12000, 0, 0),
(52559, 396, 0, 0, 14000, 0, 0),
(52561, 396, 0, 0, 14000, 0, 0);