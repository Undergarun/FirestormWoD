SET @REF_RUKHMAR = 83746;

DELETE FROM `reference_loot_template` WHERE entry = @REF_RUKHMAR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_RUKHMAR, 115436, 56, 1, 1, 1, 1),
(@REF_RUKHMAR, 120114, 53, 1, 1, 1, 1),
(@REF_RUKHMAR, 115434, 51, 1, 1, 1, 1),
(@REF_RUKHMAR, 115438, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115437, 20, 1, 1, 1, 1),
(@REF_RUKHMAR, 120112, 20, 1, 1, 1, 1),
(@REF_RUKHMAR, 115441, 44, 1, 1, 1, 1),
(@REF_RUKHMAR, 115435, 36, 1, 1, 1, 1),
(@REF_RUKHMAR, 120111, 32, 1, 1, 1, 1),
(@REF_RUKHMAR, 115439, 30, 1, 1, 1, 1),
(@REF_RUKHMAR, 115433, 35, 1, 1, 1, 1),
(@REF_RUKHMAR, 115440, 29, 1, 1, 1, 1),
(@REF_RUKHMAR, 116771, 5, 1, 1, 1, 1),
(@REF_RUKHMAR, 120113, 27, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_RUKHMAR WHERE `entry`= @REF_RUKHMAR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_RUKHMAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_RUKHMAR, 1, 100, 1, 0, -@REF_RUKHMAR, 6);