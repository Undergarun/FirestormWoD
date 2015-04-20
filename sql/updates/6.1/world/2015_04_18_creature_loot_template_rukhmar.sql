SET @REF_RUKHMAR = 83746;

DELETE FROM `reference_loot_template` WHERE entry = @REF_RUKHMAR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_RUKHMAR, 115433, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115434, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115435, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115436, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115437, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115438, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115439, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115440, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 115441, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 116771, 0.01, 1, 1, 1, 1),
(@REF_RUKHMAR, 120111, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 120112, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 120113, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 120114, 100, 1, 1, 1, 1),
(@REF_RUKHMAR, 122293, 100, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_RUKHMAR WHERE `entry`= @REF_RUKHMAR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_RUKHMAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_RUKHMAR, 1, 100, 1, 0, -@REF_RUKHMAR, 6);