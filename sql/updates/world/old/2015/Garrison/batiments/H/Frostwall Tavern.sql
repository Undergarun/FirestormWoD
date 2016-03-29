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

-- LEVEL 2

-- LEVEL 3