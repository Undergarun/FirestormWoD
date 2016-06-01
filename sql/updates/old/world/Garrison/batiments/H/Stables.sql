-- LEVEL 1

	-- Stables

		-- templates
		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(81494, "Ka’la", "", "", 0, 0, 7, 56419, 0, 0, 0, 0, 0, 0, 0, 0, 0, 262144, 0, 1, 1, 1, 0, 0, 0, 0, 1, 93, 93, 2110, 2000, 2000, 4, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(81494, 1, 11922, 0, 44217) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);

		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(86979, "Tormak the Scarred", "", "", 0, 0, 7, 24425, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 1, 1, 0, 0, 0, 0, 1, 75, 75, 2110, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(86979, 1, 24512, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
		UPDATE creature_template SET npcflag = npcflag | 1, scriptname = "npc_Tormak_Garr" WHERE entry = 86979;
		
		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(87242, "Sage Paluna", "", "", 0, 0, 7, 23921, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 0, 0, 0, 0, 1, 75, 75, 2110, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(87242, 1, 24512, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
		UPDATE creature_template SET npcflag = npcflag | 1, scriptname = "npc_SagePaluna_Garr" WHERE entry = 87242;


		/* Quests list : 
		-- Elever un elekk
		36946
		Dressage d'elekk : 
		37063
		37064
		37065
		37066
		37067
		37068
		37069
		-- Capturer un sabot fourchu
		36912
		Dressage de Sabot-Fourchu
		37048
		37049
		37050
		37051
		37052
		37053
		37054
		37055
		37056
		37057
		37058
		37059
		Sangler un sanglier
		36944
		Dressage de Sanglier
		37032
		37033
		37034
		37035
		37036
		37037
		37038
		37039
		37040
		37041*/

		REPLACE INTO creature_queststarter VALUES
		(86979, 36912),
		(86979, 37068),
		(86979, 37067),
		(86979, 37069),
		(86979, 37066),
		(86979, 37063),
		(86979, 37065),
		(86979, 37064),
		(86979, 37052),
		(86979, 37058),
		(86979, 37050),
		(86979, 37057),
		(86979, 37051),
		(86979, 37048),
		(86979, 37059),
		(86979, 37056),
		(86979, 37053),
		(86979, 37055),
		(86979, 37049),
		(86979, 37054),
		(86979, 37034),
		(86979, 37040),
		(86979, 37032),
		(86979, 37039),
		(86979, 37033),
		(86979, 37041),
		(86979, 37038),
		(86979, 37035),
		(86979, 37037),
		(86979, 37036),
		(86979, 36946),
		(86979, 36944);

		REPLACE INTO creature_questender VALUES
		(86979, 36912),
		(86979, 37068),
		(86979, 37067),
		(86979, 37069),
		(86979, 37066),
		(86979, 37063),
		(86979, 37065),
		(86979, 37064),
		(86979, 37052),
		(86979, 37058),
		(86979, 37050),
		(86979, 37057),
		(86979, 37051),
		(86979, 37048),
		(86979, 37059),
		(86979, 37056),
		(86979, 37053),
		(86979, 37055),
		(86979, 37049),
		(86979, 37054),
		(86979, 37034),
		(86979, 37040),
		(86979, 37032),
		(86979, 37039),
		(86979, 37033),
		(86979, 37041),
		(86979, 37038),
		(86979, 37035),
		(86979, 37037),
		(86979, 37036),
		(86979, 36946),
		(86979, 36944);

		-- sage paluna


		/* Quests list : 
		-- Lier un Loup
		36950
		Dressage de loup : 
		37105 
		37106 
		37107 
		37108 
		37109 
		37110 
		37111
		-- Prendre un potamodonte
		36945
		Dressage de potamodonte
		37071
		37072
		37073
		37074
		37075
		37076
		37077
		37078
		37079
		Terrasser un Talbuk
		36917
		Dressage de Talbuk
		37093
		37094
		37095
		37096
		37097
		37098
		37099
		37100
		37101
		37102
		37103
		37104

		*/
		
		REPLACE INTO creature_queststarter VALUES
		(87242, 36950),
		(87242, 37105),
		(87242, 37106),
		(87242, 37107),
		(87242, 37108),
		(87242, 37109),
		(87242, 37110),
		(87242, 37111),
		(87242, 36945),
		(87242, 37071),
		(87242, 37072),
		(87242, 37073),
		(87242, 37074),
		(87242, 37075),
		(87242, 37076),
		(87242, 37077),
		(87242, 37078),
		(87242, 37079),
		(87242, 36917),
		(87242, 37093),
		(87242, 37094),
		(87242, 37095),
		(87242, 37096),
		(87242, 37097),
		(87242, 37098),
		(87242, 37099),
		(87242, 37100),
		(87242, 37101),
		(87242, 37102),
		(87242, 37103),
		(87242, 37104);
		
		237169, 3, 7922, "Griffe noire de Sethe"
		237170, 3, 18860, "Collier en dents de garn"

		REPLACE INTO creature_questender VALUES
		(87242, 36950),
		(87242, 37105),
		(87242, 37106),
		(87242, 37107),
		(87242, 37108),
		(87242, 37109),
		(87242, 37110),
		(87242, 37111),
		(87242, 36945),
		(87242, 37071),
		(87242, 37072),
		(87242, 37073),
		(87242, 37074),
		(87242, 37075),
		(87242, 37076),
		(87242, 37077),
		(87242, 37078),
		(87242, 37079),
		(87242, 36917),
		(87242, 37093),
		(87242, 37094),
		(87242, 37095),
		(87242, 37096),
		(87242, 37097),
		(87242, 37098),
		(87242, 37099),
		(87242, 37100),
		(87242, 37101),
		(87242, 37102),
		(87242, 37103),
		(87242, 37104);
		
		REPLACE INTO gameobject_loot_template VALUES
		(56642, 118469, 100, 1, 0, 1, 1, ''), -- Black Claw of Sethe
		(56643, 118470, 100, 1, 0, 1, 1, ''); -- Garn-Tooth Necklace

		UPDATE gameobject_template SET NAME = "Black Claw of Sethe" WHERE entry = 237169;
		UPDATE gameobject_template SET NAME = "Garn-Tooth Necklace" WHERE entry = 237170;

		-- plot content (only fixed spawn are tormak and ka'la (81494), else is dynamic)

		DELETE FROM garrison_plot_content WHERE plot_type_or_building = -65 AND faction_index = 0; -- horde
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-65','0','86979','4.78931','-3.73092','0.930573','1.86515');

-- LEVEL 2

		DELETE FROM garrison_plot_content WHERE plot_type_or_building = -66 AND faction_index = 0; -- horde
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-66','0','-237169','12.5275','-7.15362','0.363678','2.81372');
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-66','0','86979','1.26598','-6.91367','0.741592','0.689225');


-- LEVEL 3

		DELETE FROM garrison_plot_content WHERE plot_type_or_building = -67 AND faction_index = 0; -- horde
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-67','0','-237169','22.5984','-7.442','1.0994','3.09447');
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-67','0','-237170','22.5541','-8.3928','2.19916','3.09447');
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-67','0','86979','7.36096','-5.45125','1.16763','0.99847');
