-- LEVEL 1

-- Quests for cosmetic Rewards (20)

REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES
('87991','37152'),
('87992','37153'),
('87994','37157'),
('87995','37155'),
('87996','37156'),
('87997','37154'),
('87998','37158'),
('87999','37159'),
('88000','37161'),
('88001','37162'),
('88002','37163'),
('88003','37164'),
('88004','37165'),
('88005','37166'),
('88006','37167'),
('88007','37160'),
('88024','37148'),
('88025','37150'),
('88026','37151'),
('88027','37149');

REPLACE INTO `creature_questender` (`id`, `quest`) VALUES
('87991','37152'),
('87992','37153'),
('87994','37157'),
('87995','37155'),
('87996','37156'),
('87997','37154'),
('87998','37158'),
('87999','37159'),
('88000','37161'),
('88001','37162'),
('88002','37163'),
('88003','37164'),
('88004','37165'),
('88005','37166'),
('88006','37167'),
('88007','37160'),
('88024','37148'),
('88025','37150'),
('88026','37151'),
('88027','37149');

-- Daily quests for Heroic Dungeons (15)

REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES
('87991','37228'),
('87999','37235'),
('87992','37229'),
('88001','37237'),
('88000','37236'),
('87998','37234'),
('88003','37239'),
('88006','37242'),
('88005','37241'),
('88002','37238'),
('87996','37232'),
('87997','37233'),
('88004','37240'),
('87995','37231'),
('88007','37243');

REPLACE INTO `creature_questender` (`id`, `quest`) VALUES
('87991','37228'),
('87999','37235'),
('87992','37229'),
('88001','37237'),
('88000','37236'),
('87998','37234'),
('88003','37239'),
('88006','37242'),
('88005','37241'),
('88002','37238'),
('87996','37232'),
('87997','37233'),
('88004','37240'),
('87995','37231'),
('88007','37243');

-- Daily quests for Dungeons (8)

REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES
('88013','37142'),
('88023','37147'),
('88024','37244'),
('88017','37145'),
('88026','37209'),
('88022','37146'),
('88025','37227'),
('88027','37245');

REPLACE INTO `creature_questender` (`id`, `quest`) VALUES
('88013','37142'),
('88023','37147'),
('88024','37244'),
('88017','37145'),
('88026','37209'),
('88022','37146'),
('88025','37227'),
('88027','37245');

-- GARRISON PLOT CONTENT

INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(77376, "Lunarfall Woodcutter", "", "", 0, 0, 7, 53841, 58226, 56615, 58227, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 35, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 156782, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(84947, "Lysa Serion", "Headhunter", "", 0, 0, 7, 38318, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 35, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(77368, "Madison Clark", "Cook", "", 0, 0, 7, 53833, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1733, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(85510, "Lilton Reese", "Chef's Assistant", "", 0, 0, 7, 7606, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1733, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(83810, "Adventurer", "", "", 0, 0, 7, 33021, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 35, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(88013, "Lunk", "", "", 0, 0, 7, 6692, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 47, 47, 2110, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(88013, 1, 2202, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);

INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(88017, "Budd", "", "", 0, 0, 7, 21702, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 83, 83, 2110, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(88017, 1, 2202, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);

DELETE FROM garrison_plot_content WHERE plot_type_or_building = -34 AND faction_index = 1;
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
('-34','1','85510','-1.58643','-3.10806','0.831017','3.23974'),
('-34','1','77368','4.29658','-7.00207','0.825394','4.72808'),
('-34','1','84947','6.25039','-0.285222','0.815781','0.160978'),
('-34','1','77376','7.52664','-7.15661','0.827118','5.96035'),
('-34','1','77376','10.4965','-7.8686','0.881149','2.85803'),
('-34','1','77376','4.82282','-1.98048','0.820175','3.68542'),
('-34','1','77376','2.77307','-2.14389','0.818718','5.69683'),
('-34','1','77376','10.6427','3.82122','0.93544','1.52241'),
('-34','1','77376','10.8004','7.47997','0.935577','4.71506'),
('-34','1','-233460','10.8004','7.47997','0.435577','4.71506'),
('-34','1','-233460','10.618','3.91835','0.435783','1.37711'),
('-34','1','-233460','10.4423','-7.91924','0.379066','2.98719'),
('-34','1','-233460','7.66503','-7.20229','0.327553','5.97783'),
('-34','1','-233460','4.84011','-2.02342','0.320763','3.91851'),
('-34','1','-233460','2.44275','-1.93866','0.315498','5.59535');

UPDATE creature_template SET scriptname = "npc_MadisonClark_Garr" WHERE entry = 77368;

-- LEVEL 2

-- LEVEL 3

-- Quest Fixes

	DELETE FROM quest_template_objective WHERE QuestID IN (
	37162,
	37161,
	37148,
	37159,
	37158,
	37152,
	37153,
	37157,
	37155,
	37154,
	37156,
	37165,
	37166,
	37150,
	37163,
	37164,
	37149,
	37167,
	37160,
	37151,
	37237,
	37236,
	37235,
	37234,
	37228,
	37229,
	37230,
	37231,
	37232,
	37233,
	37241,
	37240,
	37239,
	37238,
	37243,
	37242,
	37244,
	37147,
	37142,
	37145,
	37146,
	37227,
	37245,
	37209);

	INSERT INTO quest_template_objective (QuestID, `Type`, `Index`, ObjectID, Amount) VALUES
	(37162, 1, 0, 118624, 1),
	(37161, 1, 0, 118623, 1),
	(37148, 1, 0, 118650, 1),
	(37159, 1, 0, 118622, 1),
	(37158, 1, 0, 118621, 1),
	(37152, 1, 0, 118534, 1),
	(37153, 1, 0, 118616, 1),
	(37157, 1, 0, 118617, 1),
	(37155, 1, 0, 118618, 1),
	(37154, 1, 0, 118620, 1),
	(37156, 1, 0, 118619, 1),
	(37165, 1, 0, 118627, 1),
	(37166, 1, 0, 118628, 1),
	(37150, 1, 0, 118652, 1),
	(37163, 1, 0, 118625, 1),
	(37164, 1, 0, 118626, 1),
	(37149, 1, 0, 118651, 1),
	(37167, 1, 0, 118643, 1),
	(37160, 1, 0, 118644, 1),
	(37151, 1, 0, 118653, 1),
	(37237, 1, 0, 118624, 1),
	(37236, 1, 0, 118623, 1),
	(37235, 1, 0, 118622, 1),
	(37234, 1, 0, 118621, 1),
	(37228, 1, 0, 118534, 1),
	(37229, 1, 0, 118616, 1),
	(37230, 1, 0, 118617, 1),
	(37231, 1, 0, 118618, 1),
	(37232, 1, 0, 118619, 1),
	(37233, 1, 0, 118620, 1),
	(37241, 1, 0, 118628, 1),
	(37240, 1, 0, 118627, 1),
	(37239, 1, 0, 118626, 1),
	(37238, 1, 0, 118625, 1),
	(37243, 1, 0, 118644, 1),
	(37242, 1, 0, 118643, 1),
	(37244, 1, 0, 118650, 1),
	(37147, 1, 0, 118649, 1),
	(37142, 1, 0, 118646, 1),
	(37145, 1, 0, 118647, 1),
	(37146, 1, 0, 118648, 1),
	(37227, 1, 0, 118652, 1),
	(37245, 1, 0, 118651, 1),
	(37209, 1, 0, 118653, 1),
	(37162, 0, 1,  77120, 1),
	(37161, 0, 1,  77120, 1),
	(37148, 0, 1,  77120, 1),
	(37159, 0, 1,  76266, 1),
	(37158, 0, 1,  76266, 1),
	(37152, 0, 1,  74790, 1),
	(37153, 0, 1,  74790, 1),
	(37157, 0, 1,  83612, 1),
	(37155, 0, 1,  83612, 1),
	(37154, 0, 1,  77734, 1),
	(37156, 0, 1,  77734, 1),
	(37165, 0, 1,  83846, 1),
	(37166, 0, 1,  83846, 1),
	(37150, 0, 1,  83846, 1),
	(37163, 0, 1,  76407, 1),
	(37164, 0, 1,  76407, 1),
	(37149, 0, 1,  76407, 1),
	(37167, 0, 1,  80005, 1),
	(37160, 0, 1,  80005, 1),
	(37151, 0, 1,  80005, 1),
	(37237, 0, 1,  77120, 1),
	(37236, 0, 1,  77120, 1),
	(37235, 0, 1,  76266, 1),
	(37234, 0, 1,  76266, 1),
	(37228, 0, 1,  74790, 1),
	(37229, 0, 1,  74790, 1),
	(37230, 0, 1,  83612, 1),
	(37231, 0, 1,  83612, 1),
	(37232, 0, 1,  77734, 1),
	(37233, 0, 1,  77734, 1),
	(37241, 0, 1,  83846, 1),
	(37240, 0, 1,  83846, 1),
	(37239, 0, 1,  76407, 1),
	(37238, 0, 1,  76407, 1),
	(37243, 0, 1,  80005, 1),
	(37242, 0, 1,  80005, 1),
	(37244, 0, 1,  77120, 1),
	(37147, 0, 1,  76407, 1),
	(37142, 0, 1,  74790, 1),
	(37145, 0, 1,  83612, 1),
	(37146, 0, 1,  77734, 1),
	(37227, 0, 1,  83846, 1),
	(37245, 0, 1,  76407, 1),
	(37209, 0, 1,  80005, 1);

	REPLACE INTO gameobject_loot_template VALUES
	(237469, 118624, -100, 1, 1, 1, 1, ''),
	(237468, 118623, -100, 1, 1, 1, 1, ''),
	(237481, 118650, -100, 1, 1, 1, 1, ''),
	(237461, 118616, -100, 1, 1, 1, 1, ''),
	(237463, 118618, -100, 1, 1, 1, 1, ''),
	(237465, 118620, -100, 1, 1, 1, 1, ''),
	(237480, 118649, -100, 1, 1, 1, 1, ''),
	(237477, 118646, -100, 1, 1, 1, 1, ''),
	(237478, 118647, -100, 1, 1, 1, 1, ''),
	(237467, 118622, -100, 1, 1, 1, 1, ''),
	(237466, 118621, -100, 1, 1, 1, 1, ''),
	(237462, 118617, -100, 1, 1, 1, 1, ''),
	(237464, 118619, -100, 1, 1, 1, 1, ''),
	(237479, 118648, -100, 1, 1, 1, 1, ''),
	(237483, 118652, -100, 1, 1, 1, 1, ''),
	(237472, 118627, -100, 1, 1, 1, 1, ''),
	(237473, 118628, -100, 1, 1, 1, 1, ''),
	(237470, 118625, -100, 1, 1, 1, 1, ''),
	(237471, 118626, -100, 1, 1, 1, 1, ''),
	(237482, 118651, -100, 1, 1, 1, 1, ''),
	(237474, 118643, -100, 1, 1, 1, 1, ''),
	(237475, 118644, -100, 1, 1, 1, 1, ''),
	(237484, 118653, -100, 1, 1, 1, 1, '');

	UPDATE gameobject_template SET data1 = entry, data17 = 0, flags = flags & ~0x00000004 WHERE entry IN (
	237469,
	237468,
	237481,
	237461,
	237463,
	237465,
	237480,
	237477,
	237478,
	237467,
	237466,
	237462,
	237464,
	237479,
	237483,
	237472,
	237473,
	237470,
	237471,
	237482,
	237474,
	237475,
	237484);

	-- Everbloom
		DELETE FROM `gameobject` WHERE `id` IN (237472, 237473, 237483) AND `map` = 1279;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237472","1279","7109","7109","6","1","762.92","1442.21","88.4421","5.07356","0","0","0","0","0","255","1","0",0),
		("237473","1279","7109","7109","6","1","326.981","1490.24","102.041","0.623439","0","0","0","0","0","255","1","0",0),
		("237483","1279","7109","7109","2","1","746.571","1567.3","108.295","0.0687468","0","0","0","0","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237472","3","6807","Strangely-Glowing Frond","questinteract","Collecting","","0","4","6","118627","0","0","0","0","0","1691","56764","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237483","3","24596","Rustling Peachick Nest","questinteract","Collecting","","0","4","1","118652","0","0","0","0","0","1691","56775","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237473","3","20499","Overgrown Artifact","questinteract","Collecting","","0","4","1","118628","0","0","0","0","0","1691","56765","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Grimrail Depot
		DELETE FROM `gameobject` WHERE `id` IN (237474, 237475, 237484) AND `map` = 1208;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237474","1208","7109","7383","6","1","1724.637","1610.083","7.6916531","3.157218","0","0","0","0","0","255","1","0",0),
		("237475","1208","7109","7109","6","1","1645.542","1632.62","108.4119","4.327895","-0.5236163","-0.154665","-0.7469788","0.3793777","0","255","1","0",0),
		("237484","1208","7109","7109","2","1","1656.748","1836.932","109.7467","5.318283","0","0","-0.4639521","0.8858603","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237474","3","17016","Huge Crate of Weapons","questinteract","Collecting","","0","4","0.5","118643","0","0","0","0","0","1691","0","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237475","3","23157","Iron Limbcleaver","questinteract","Collecting","","0","4","2","118644","0","0","0","0","0","1691","56767","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237484","3","16604","Iron Autocannon","questinteract","Collecting","","0","4","0.3","118653","0","0","0","0","0","1691","56776","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Shadowmoon Burial Grounds
		DELETE FROM `gameobject` WHERE `id` IN (237470, 237471, 237482) AND `map` = 1176;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237470","1176","7109","7383","6","1","1830.549","121.7431","294.5555","3.02442","0","0","0.9982843","0.05855254","0","255","1","0",0),
		("237471","1176","7109","7109","6","1","1663.392","-231.9792","253.4646","4.6750775","-0.1768336","0.6938286","-0.1654396","0.6782046","0","255","1","0",0),
		("237482","1176","7109","7109","2","1","1796.335","71.72569","264.1181","0","0","0","0","1","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237470","3","16790","Dark Parchment","questinteract","Collecting","","0","4","2","118625","0","0","0","0","0","1691","56762","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237471","3","11524","Silver-Lined Arrow","questinteract","Collecting","","0","4","1.5","118626","0","0","0","0","0","1691","56763","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237482","3","21962","Void-Gate Key","questinteract","Collecting","","0","4","1.5","118651","0","0","0","0","0","1691","56774","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Skyreach
		DELETE FROM `gameobject` WHERE `id` IN (237467, 237466) AND `map` = 1209;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237467","1209","6988","6990","6","1","1236.616","1721.323","178.2672","2.991205","0","0","0.9971743","0.07512315","0","255","1","0",0),
		("237466","1209","6988","6990","6","1","1663.392","-231.9792","253.4646","4.6750775","-0.1768336","0.6938286","-0.1654396","0.6782046","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237467","3","13726","Pristine Plumage","questinteract","Collecting","","0","4","1.5","118622","0","0","0","0","0","1691","56759","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237466","3","18978","Sun Crystal","questinteract","Collecting","","0","4","0.5","118621","0","0","0","0","0","1691","56758","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Auchindoun
		DELETE FROM `gameobject` WHERE `id` IN (237464, 237479) AND `map` = 1182;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237464","1182","6912","6912","6","1","1685.444","2913.046","36.72615","6.200767","0.7028551","-0.03005886","-0.0282135","0.7101374","0","255","1","0",0),
		("237479","1182","6912","6912","2","1","1734.241","2793.382","40.64299","6.212135","0","0","-0.03551769","0.999369","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237464","3","16683","Soulsever Blade","questinteract","Collecting","","0","4","1","118619","0","0","0","0","0","1691","56756","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237479","3","5871","Nightmare Bell","questinteract","Collecting","","0","4","1","118648","0","0","0","0","0","1691","56771","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Iron Docks
		DELETE FROM `gameobject` WHERE `id` = 237462 AND `map` = 1195;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUE
		("237462","1195","6951","7309","6","1","6689.765","-768.1146","7.780828","0","0","0","0","1","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237462","3","23153","Horribly Acidic Solution","questinteract","Collecting","","0","4","2","118617","0","0","0","0","0","1691","56754","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");

		
	
	UPDATE creature_template SET npcflag = 3, scriptname = 'npc_lysa_serion_garr' WHERE entry = 84947;

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -35 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','1','85510','-0.538097','6.96231','0.635078','1.44424');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','1','77368','-12.6942','-1.69218','1.20133','6.26658');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','1','84947','-8.93744','-7.15514','1.18672','1.76625');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -36 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','1','85510','-11.0836','-1.50432','1.19492','0.019266');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','1','77368','9.01369','6.03496','0.888191','4.51567');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','1','84947','-0.024993','8.65036','0.63884','4.62406');
