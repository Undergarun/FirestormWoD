SET @REF_KAGRAZ = 76814;

DELETE FROM `reference_loot_template` WHERE entry = @REF_KAGRAZ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAGRAZ, 113913, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113914, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113915, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113916, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113917, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113918, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113919, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113920, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113921, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113922, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113923, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113924, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113925, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 120077, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 119193, 0, 1, 1, 1, 1);

# Set tokens
DELETE FROM `reference_loot_template` WHERE entry = @REF_KAGRAZ + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAGRAZ + 1, 119305, 0, 14, 1, 1, 1),
(@REF_KAGRAZ + 1, 119315, 0, 14, 1, 1, 1),
(@REF_KAGRAZ + 1, 119318, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_KAGRAZ WHERE `entry`= @REF_KAGRAZ;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_KAGRAZ;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAGRAZ, 1, 100, 1, 0, -@REF_KAGRAZ, 6),
(@REF_KAGRAZ, 2, 100, 2, 0, -(@REF_KAGRAZ + 1), 2),
(@REF_KAGRAZ, 3, 100, 4, 0, -(@REF_KAGRAZ + 1), 1),
(@REF_KAGRAZ, 4, 100, 8, 0, -(@REF_KAGRAZ + 1), 1);

SET @REF_HEART_OF_THE_MOUNTAIN = 76806;

DELETE FROM `reference_loot_template` WHERE entry = @REF_HEART_OF_THE_MOUNTAIN;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_HEART_OF_THE_MOUNTAIN, 113885, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113886, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113887, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113888, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113889, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113890, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113891, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113892, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113893, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113894, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113895, 0, 1, 1, 1, 1),
(@REF_HEART_OF_THE_MOUNTAIN, 113896, 0, 1, 1, 1, 1);

DELETE FROM`reference_loot_template` WHERE entry = @REF_HEART_OF_THE_MOUNTAIN + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_HEART_OF_THE_MOUNTAIN + 1, 119307, 0, 14, 1, 1, 1), -- Leggings of the Iron Conqueror (Normal)
(@REF_HEART_OF_THE_MOUNTAIN + 1, 119313, 0, 14, 1, 1, 1), -- Leggings of the Iron Vanquisher (Normal)
(@REF_HEART_OF_THE_MOUNTAIN + 1, 119320, 0, 14, 1, 1, 1); -- Leggings of the Iron Protector (Normal)

UPDATE `creature_template` SET `lootid`= @REF_HEART_OF_THE_MOUNTAIN WHERE `entry`= @REF_HEART_OF_THE_MOUNTAIN;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_HEART_OF_THE_MOUNTAIN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_HEART_OF_THE_MOUNTAIN, 1, 100, 1, 0, -@REF_HEART_OF_THE_MOUNTAIN, 6),
(@REF_HEART_OF_THE_MOUNTAIN, 2, 100, 2, 0, -(@REF_HEART_OF_THE_MOUNTAIN + 1), 2),
(@REF_HEART_OF_THE_MOUNTAIN, 3, 100, 4, 0, -(@REF_HEART_OF_THE_MOUNTAIN + 1), 1),
(@REF_HEART_OF_THE_MOUNTAIN, 4, 100, 8, 0, -(@REF_HEART_OF_THE_MOUNTAIN + 1), 1);

SET @REF_KROMOG = 77692;

DELETE FROM `reference_loot_template` WHERE entry = @REF_KROMOG;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KROMOG, 113926, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113927, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113928, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113929, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113930, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113931, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113932, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113933, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113934, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113935, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113936, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113937, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113938, 0, 1, 1, 1, 1);

DELETE FROM reference_loot_template WHERE entry = @REF_KROMOG + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KROMOG + 1, 119308, 0, 14, 1, 1, 1),
(@REF_KROMOG + 1, 119312, 0, 14, 1, 1, 1),
(@REF_KROMOG + 1, 119321, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_KROMOG WHERE `entry`= @REF_KROMOG;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_KROMOG;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KROMOG, 1, 100, 1, 0, -@REF_KROMOG, 6),
(@REF_KROMOG, 2, 100, 2, 0, -(@REF_KROMOG + 1), 2),
(@REF_KROMOG, 3, 100, 4, 0, -(@REF_KROMOG + 1), 1),
(@REF_KROMOG, 4, 100, 8, 0, -(@REF_KROMOG + 1), 1);