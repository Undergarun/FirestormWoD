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


INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(79828, "Murg", "Cook", "", 0, 0, 7, 56578, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1734, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(87300, "Vora Strongarm", "Bartender", "", 0, 0, 7, 27957, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1734, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(87305, "Akanja", "Headhunter", "", 0, 0, 7, 17419, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 35, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(88013, "Lunk", "", "", 0, 0, 7, 6692, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 47, 47, 2110, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(88013, 1, 2202, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);

DELETE FROM garrison_plot_content WHERE plot_type_or_building = -34 AND faction_index = 0;
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-34','0','79828','2.81785','-0.262591','1.29671','6.24151');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-34','0','87305','11.3304','2.73706','1.26816','2.00821');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-34','0','87300','-1.03095','-0.3539','1.27022','0.174304');

UPDATE creature_template SET scriptname = "npc_Murg_Garr" WHERE entry = 79828;

#34822/Quest - What we need

	REPLACE INTO creature_loot_template VALUE
	(80174, 111907, -100, 1, 0, 1, 2, '');

	REPLACE INTO gameobject_loot_template VALUES
	(230881, 111907, -100, 1, 0, 1, 2, ''),
	(230879, 111907, -100, 1, 0, 1, 2, ''),
	(230880, 111907, -100, 1, 0, 1, 2, '');

#38293/Quest - Somptuous Fur

	REPLACE INTO creature_queststarter VALUE (91025, 38293);
	REPLACE INTO creature_questender VALUES
	(91025, 38293),
	(91034, 38293);

#91034/NPC - Calvo Klyne <Fur Trader>

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91034;
	DELETE FROM creature WHERE id = 91034;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91034','1330','7004','7210','2','4294967295','0','0','5553.67','4598.79','141.164','5.81351','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91034, 120945, 5815), -- 5 somptuous fur
	(91034, 122549, 5829), -- 60 somptuous fur
	(91034, 122716, 5829),
	(91034, 127724, 5829),
	(91034, 127742, 5829);

#91025/NPC - Dorothy Two

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91025;
	DELETE FROM creature WHERE id = 91025;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91025','1331','7078','7211','2','1','0','0','1937.66','330.089','91.764','6.21431','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91025, 120945, 5815), -- 5 somptuous fur
	(91025, 122549, 5829), -- 60 somptuous fur
	(91025, 122716, 5829),
	(91025, 127724, 5829),
	(91025, 127742, 5829);

#91404/NPC - Samantha Scarlet <Herb Trader>

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91404;
	DELETE FROM creature WHERE id = 91404;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91404','1331','7078','7211','2','1','0','0','1938.53','331.616','91.7648','5.90408','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91404, 122710, 5824), -- 60 Starflower
	(91404, 122741, 5827), -- 20 Cerulean Pigments
	(91404, 122553, 5827),
	(91404, 122713, 5827),
	(91404, 127723, 5827),
	(91404, 127728, 5827),
	(91404, 122548, 5827),
	(91404, 127746, 5827),
	(91404, 128409, 5827),
	(91404, 128410, 5827),
	(91404, 128411, 5827),
	(91404, 128412, 5827),
	(91404, 128413, 5827),
	(91404, 128414, 5810), -- 10 Alchemical Catalyst
	(91404, 122599, 5810),
	(91404, 122600, 5810),
	(91404, 122605, 5810),
	(91404, 128160, 5810),
	(91404, 120945, 5296); -- 5 Talador Orchid (5822 seems okay also)

#38296/Quest - Herbs Galore

	REPLACE INTO creature_queststarter VALUE (91404, 38296);
	REPLACE INTO creature_questender VALUES
	(91404, 38296),
	(91031, 38296);

#91031/NPC - Nicholas Mitrik

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91031;
	DELETE FROM creature WHERE id = 91031;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91031','1330','7004','7210','2','1','0','0','5554.42','4600.68','141.163','5.51585','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91031, 122710, 5824), -- 60 Starflower
	(91031, 122741, 5827), -- 20 Cerulean Pigments
	(91031, 122553, 5827),
	(91031, 122713, 5827),
	(91031, 127723, 5827),
	(91031, 127728, 5827),
	(91031, 122548, 5827),
	(91031, 127746, 5827),
	(91031, 128409, 5827),
	(91031, 128410, 5827),
	(91031, 128411, 5827),
	(91031, 128412, 5827),
	(91031, 128413, 5827),
	(91031, 128414, 5810), -- 10 Alchemical Catalyst
	(91031, 122599, 5810),
	(91031, 122600, 5810),
	(91031, 122605, 5810),
	(91031, 128160, 5810),
	(91031, 120945, 5296); -- 5 Talador Orchid (5822 seems okay also)

#37961/Quest - Bringing the Bass -- Horde

	UPDATE creature_template SET npcflag = 2 WHERE entry = 91072;
	DELETE FROM creature WHERE id = 91072;
--	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91072','1330','7004','7210','2','1','0','0','5588.78','4602.95','136.588','0.0270831','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
-- add spawns later when jukebox is scripted
	REPLACE INTO creature_queststarter VALUE (91072, 37961);
	REPLACE INTO creature_questender   VALUE (91072, 37961);

	REPLACE INTO gameobject_loot_template VALUE (240568, 122565, -100, 1, 0, 1, 1, '');
	REPLACE INTO gameobject_loot_template VALUE (240575, 122566, -100, 1, 0, 1, 1, '');

	REPLACE INTO creature_loot_template VALUE (83988, 122567, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (83987, 122567, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (83989, 122567, -100, 1, 0, 1, 1, '');

	REPLACE INTO creature_loot_template VALUE (75815, 122568, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (86262, 122569, -100, 1, 0, 1, 1, '');

	-- SPAWNS

#38356/Quest - Bringing the Bass -- Alliance

	DELETE FROM creature WHERE id = 91589;
--	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91589','1331','7078','7211','2','4294967295','0','0','1924.43','310.853','88.9662','3.78067','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
-- add spawns later when jukebox is scripted

	UPDATE creature_template SET npcflag = 2 WHERE entry = 91589;
	REPLACE INTO creature_queststarter VALUE (91589, 38356);
	REPLACE INTO creature_questender   VALUE (91589, 38356);