-- LEVEL 1

	INSERT INTO npc_text (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `BuildVerified`) VALUES(86898, "Beautiful day commander.", "Beautiful day commander.", 0, 100, 19342) ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `text0_0` = VALUES(`text0_0`), `text0_1` = VALUES(`text0_1`), `lang0` = VALUES(`lang0`), `prob0` = VALUES(`prob0`), `BuildVerified` = VALUES(`BuildVerified`);
	INSERT INTO gossip_menu (`entry`, `text_id`) VALUES(16962, 86898) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `text_id` = VALUES(`text_id`);
	UPDATE `creature_template` SET gossip_menu_id = 16962, npcflag=3, ScriptName="npc_NaronBloomthistle_Garr" WHERE `entry`=85344;

	-- Work order NPC
	INSERT INTO npc_text (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `BuildVerified`) VALUES(86941, "Draenic seeds can be gathered from herbalism nodes across draenor.  Return to me once you have 5 or more and I will grow them into herbs for you.", "Draenic seeds can be gathered from herbalism nodes across draenor.  Return to me once you have 5 or more and I will grow them into herbs for you.", 0, 100, 19342) ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `text0_0` = VALUES(`text0_0`), `text0_1` = VALUES(`text0_1`), `lang0` = VALUES(`lang0`), `prob0` = VALUES(`prob0`), `BuildVerified` = VALUES(`BuildVerified`);
	UPDATE `creature_template` SET gossip_menu_id = 16966, npcFlag=3, npcFlag2=32, minlevel=90, maxlevel=90, ScriptName="npc_OllyNimkip_Garr" WHERE `entry`=85514;

	UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=77355;
	DELETE FROM `npc_vendor` WHERE `entry` = 77355 AND `type` = 1;
	INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
	(77355, 0, 2901, 0, 0, 0, 1), 
	(77355, 0, 3371, 0, 0, 0, 1), 
	(77355, 0, 4470, 0, 0, 0, 1), 
	(77355, 0, 5956, 0, 0, 0, 1), 
	(77355, 0, 6217, 0, 0, 0, 1), 
	(77355, 0, 6256, 0, 0, 0, 1), 
	(77355, 0, 7005, 0, 0, 0, 1), 
	(77355, 0, 20815, 0, 0, 0, 1), 
	(77355, 0, 38682, 0, 0, 0, 1), 
	(77355, 0, 39354, 0, 0, 0, 1), 
	(77355, 0, 39505, 0, 0, 0, 1), 
	(77355, 0, 79249, 0, 0, 0, 1), 
	(77355, 0, 85663, 0, 0, 0, 1), 
	(77355, 0, 112322, 0, 0, 0, 1), 
	(77355, 0, 115351, 0, 0, 0, 1), 
	(77355, 0, 117445, 0, 0, 0, 1), 
	(77355, 0, 117452, 0, 0, 0, 1);

	DELETE FROM creature_queststarter WHERE id=85344 AND quest=36404;
	INSERT INTO creature_queststarter(`id`, `quest`) values('85344','36404');

	DELETE FROM creature_questender WHERE id=85344 AND quest=36404;
	INSERT INTO creature_questender(`id`, `quest`) values('85344','36404');

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-29 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','85344','3.57653','-7.82384','0.767654','4.75504');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','77903','-0.933269','-2.83758','1.47805','6.28319');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','77369','-0.928431','-1.12068','1.47805','6.28319');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','77355','-11.3134','-9.16087','0.767654','6.01708');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','85514','-10.9741','-10.5749','0.767654','5.87562');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','-233835','17.5351','-7.30508','-1.79285','4.89865');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','-233835','19.1577','-11.6836','-1.73195','1.08949');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','-235885','-10.3621','-8.1722','0.184349','5.48667');

	UPDATE gameobject_template SET flags=0, ScriptName="go_garrison_shipment_container" WHERE TYPE=45;

	UPDATE `gameobject_template` SET `ScriptName` = 'go_garrison_herb' WHERE `entry` IN (235376, 235389, 235387, 235391, 235388, 235390); 
	INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(235388, 3, 17146, "Gorgrond Flytrap", "gatherherbs", "", "", 0, 0, 2, 0, 0, 0, 0, 0, 0, 2282, 55535, 0, 1, 1, 1, 0, 235113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26967, 91, 1, 0, 0, 0, 700, 700, 0, 0, 0, 0, 0, 0, 0, 0, 75, 20444) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `faction` = VALUES(`faction`), `flags` = VALUES(`flags`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);

	-- HerbSpawnType::TaladorOrchid
	DELETE FROM gameobject_loot_template WHERE entry=55541;
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55541, 116053, 100, 1, 0, 1, 3, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55541, 109129, 100, 1, 0, 2, 8, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55541, 120945, 10, 1, 0, 1, 3, '');

	-- HerbSpawnType::GrogrondFlytrap
	UPDATE gameobject_template SET data18 = 85, scriptname = "go_garrison_herb" WHERE entry = 235388;
	DELETE FROM gameobject_loot_template WHERE entry=55535;
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55535, 116053, 100, 1, 0, 1, 3, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55535, 109126, 100, 1, 0, 2, 8, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55535, 120945, 10, 1, 0, 1, 3, '');

	-- HerbSpawnType::NagrandArrowbloom
	DELETE FROM gameobject_loot_template WHERE entry=55539;
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55539, 116053, 100, 1, 0, 1, 3, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55539, 109128, 100, 1, 0, 2, 15, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55539, 120945, 10, 1, 0, 1, 3, '');

	-- HerbSpawnType::Frostweed
	UPDATE `gameobject_template` SET `data17` = '26967' WHERE `entry` = '235376'; 
	DELETE FROM gameobject_loot_template WHERE entry=55526;
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55526, 116053, 100, 1, 0, 1, 3, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55526, 109124, 100, 1, 0, 2, 8, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55526, 120945, 10, 1, 0, 1, 3, '');

	-- HerbSpawnType::Starflower
	DELETE FROM gameobject_loot_template WHERE entry=55537;
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55537, 116053, 100, 1, 0, 1, 3, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55537, 109127, 100, 1, 0, 2, 8, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55537, 120945, 10, 1, 0, 1, 3, '');

	-- HerbSpawnType::Fireweed
	DELETE FROM gameobject_loot_template WHERE entry=55533;
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55533, 116053, 100, 1, 0, 1, 3, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55533, 109125, 100, 1, 0, 2, 8, '');
	INSERT INTO gameobject_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES (55533, 120945, 10, 1, 0, 1, 3, '');
	
	-- These are loots for the tree (level 3)
	-- Giant Nagrand Cherry
	REPLACE INTO gameobject_loot_template VALUES 
	(56291, 118272, 99.5, 1, 1, 1, 2, ""),
	(56291, 118275, 0.5, 1, 1, 1, 1, "");

	-- Fuzzy Pear
	REPLACE INTO gameobject_loot_template VALUES 
	(56290, 118268, 99.5, 1, 1, 1, 2, ""),
	(56290, 118274, 0.5, 1, 1, 1, 1, "");

	-- Greenskin Apple
	REPLACE INTO gameobject_loot_template VALUES 
	(56283, 118269, 99.5, 1, 1, 1, 2, ""),
	(56283, 118276, 0.5, 1, 1, 1, 1, "");

	-- O'ruk Orange
	REPLACE INTO gameobject_loot_template VALUES 
	(56288, 118270, 99.5, 1, 1, 1, 2, ""),
	(56288, 118273, 0.5, 1, 1, 1, 1, "");

	-- Ironpeel Plantain
	REPLACE INTO gameobject_loot_template VALUES 
	(56289, 118271, 99.5, 1, 1, 1, 3, ""),
	(56289, 118277, 0.5, 1, 1, 1, 1, "");

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-136 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','85344','3.57653','-7.82384','0.767654','4.75504');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','77903','-0.933269','-2.83758','1.47805','6.28319');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','77369','-0.928431','-1.12068','1.47805','6.28319');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','77355','-11.3134','-9.16087','0.767654','6.01708');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','85514','-10.9741','-10.5749','0.767654','5.87562');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','-233835','17.5351','-7.30508','-1.79285','4.89865');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','-233835','19.1577','-11.6836','-1.73195','1.08949');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','1','-235885','-10.3621','-8.1722','0.184349','5.48667');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-137 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','85344','3.57653','-7.82384','0.767654','4.75504');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','77903','-0.933269','-2.83758','1.47805','6.28319');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','77369','-0.928431','-1.12068','1.47805','6.28319');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','77355','-11.3134','-9.16087','0.767654','6.01708');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','85514','-10.9741','-10.5749','0.767654','5.87562');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','-233835','17.5351','-7.30508','-1.79285','4.89865');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','-233835','19.1577','-11.6836','-1.73195','1.08949');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','1','-235885','-10.3621','-8.1722','0.184349','5.48667');