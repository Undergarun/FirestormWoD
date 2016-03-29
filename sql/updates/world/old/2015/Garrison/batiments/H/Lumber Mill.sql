-- LEVEL 1

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -40 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-40','0','84247','-2.28769','0.578986','1.47505','1.4317');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-40','0','-233832','0.694003','0.907112','0.979095','3.80939');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-40','0','79835','5.49697','2.238','1.72977','1.17038');

	REPLACE INTO trinity_string (entry, content_default, content_loc2, content_loc6) VALUE (14112, "|cADFF2FFF+$1i Timber |r", "|cADFF2FFF+$1i Bois |r", "|cADFF2FFF+$1i Madera |r");
	DELETE FROM gameobject WHERE id = 234568 AND map = 1116; -- Stump, temp summoned when wood is taken

	#84247/NPC - Lumber Lord Oktron

		REPLACE INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(84247, "Lumber Lord Oktron", "Work Orders", "workorders", 0, 0, 7, 58229, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 95, 95, 29, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		REPLACE INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(84247, 1, 853, 0, 0);
		UPDATE creature_template SET npcflag = npcflag | 1 | 2, npcflag2 = 32, scriptname = "npc_LumberLordOktron_Garr" WHERE entry = 84247;
		REPLACE INTO creature_queststarter VALUE (84247, 36137);
		REPLACE INTO creature_queststarter VALUE (84247, 36138);

	#233832/Gob - Lumber Mill Work Order

		REPLACE INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(233832, 45, 15585, "Lumber Mill Work Order", "", "", "", 0, 0, 1, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		UPDATE gameobject_template SET scriptname = "go_garrison_shipment_container" WHERE entry = 233832;

	#233604 - 234080/Gob - Small Timber

		UPDATE gameobject_template SET flags = flags & ~0x04, scriptname = "go_garrison_timber" WHERE entry IN (
		234193,
		234197,
		233604,
		234080,
		234122,
		234126,
		234109,
		234110,
		233922,
		234097,
		234000,
		234022,
		234098,
		234119,
		234123,
		234127,
		234194,
		234196,
		234198,
		233634,
		233635,
		234007,
		234099,
		234120,
		234124,
		234128,
		234195,
		234199);
		
	#83985/NPC - Peon

		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(83985, "Peon", "", "", 0, 0, 7, 58069, 58070, 58071, 58072, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 5, 5, 29, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 167957, 168523, 170079, 167958, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		DELETE FROM creature_text WHERE entry = 83985;
		INSERT INTO creature_text (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`) VALUES(83985, 0, 0, "Work complete!", 12, 0, 100) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `groupid` = VALUES(`groupid`), `id` = VALUES(`id`), `text` = VALUES(`text`), `type` = VALUES(`type`), `language` = VALUES(`language`), `probability` = VALUES(`probability`);
		UPDATE creature_template SET scriptname = "npc_Timber_Peon" WHERE entry = 83985;
		REPLACE INTO creature_template_addon (entry, emote) VALUE(83985, 599);
		DELETE FROM smart_scripts WHERE entryorguid = 83985;
		INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES('83985','0','0','0','25','0','100','0','0','0','0','0','1','0','4000','0','0','0','0','0','0','0','0','0','0','0','0','New Script');

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -41 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-41','0','84247','5.07749','4.08816','1.472','1.37066');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-41','0','79835','-3.38423','-1.59738','1.47354','4.30805');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-41','0','-233832','13.3513','3.26322','0.974777','3.27132');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -138 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-138','0','-233832','9.90092','3.29341','0.974869','1.79477');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-138','0','84247','-0.977483','-4.90326','1.4749','4.14704');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-138','0','79835','-0.96153','4.85086','1.47467','2.21103');
