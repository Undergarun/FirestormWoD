-- LEVEL 1

	DELETE FROM creature_template WHERE entry=79826;
	DELETE FROM creature_template WHERE entry=86696;
	INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(79826, "Pozzlow", "Engineer", "trainer", 0, 0, 7, 56936, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1734, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 19865) ON DUPLICATE KEY UPDATE entry=entry;
	INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(86696, "Garbra Fizzwonk", "Work Orders", "workorders", 0, 0, 7, 36010, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1734, 2000, 2000, 8, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 19865) ON DUPLICATE KEY UPDATE entry=entry;

	DELETE FROM creature_model_info WHERE modelid=56936;
	DELETE FROM creature_model_info WHERE modelid=36010;
	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(56936, 0.306, 1.5, 0) ON DUPLICATE KEY UPDATE modelid=modelid;
	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(36010, 0.306, 1.5, 1) ON DUPLICATE KEY UPDATE modelid=modelid;

	UPDATE creature_template SET ScriptName="npc_Pozzlow_Garr", npcFlag=131, npcFlag2=128, minlevel=90, maxlevel=90, gossip_menu_id = 0 WHERE entry=79826;
	UPDATE creature_template SET ScriptName="npc_GarbraFizzwonk_Garr", npcFlag=3, npcFlag2=32, minlevel=90, maxlevel=90, gossip_menu_id = 0 WHERE entry=86696;

	DELETE FROM `npc_vendor` WHERE `entry` = 79826 AND `type` = 1;
	INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
	(79826, 0, 90146, 0, 0, 0, 1), 
	(79826, 0, 111921, 0, 0, 0, 1), 
	(79826, 0, 118476, 0, 0, 5476, 1), 
	(79826, 0, 118477, 0, 0, 5476, 1), 
	(79826, 0, 118478, 0, 0, 5476, 1), 
	(79826, 0, 118480, 0, 0, 5476, 1), 
	(79826, 0, 118481, 0, 0, 5476, 1), 
	(79826, 0, 118484, 0, 0, 5476, 1), 
	(79826, 0, 118485, 0, 0, 5476, 1), 
	(79826, 0, 118487, 0, 0, 5476, 1), 
	(79826, 0, 118488, 0, 0, 5476, 1), 
	(79826, 0, 118489, 0, 0, 5476, 1), 
	(79826, 0, 118490, 0, 0, 5476, 1), 
	(79826, 0, 118491, 0, 0, 5479, 1), 
	(79826, 0, 118492, 0, 0, 5479, 1), 
	(79826, 0, 118493, 0, 0, 5476, 1), 
	(79826, 0, 118495, 0, 0, 5476, 1), 
	(79826, 0, 118497, 0, 0, 5476, 1), 
	(79826, 0, 118498, 0, 0, 5476, 1), 
	(79826, 0, 118499, 0, 0, 5476, 1), 
	(79826, 0, 118500, 0, 0, 5476, 1), 
	(79826, 0, 119177, 0, 0, 5476, 1), 
	(79826, 0, 120267, 0, 0, 5479, 1), 
	(79826, 0, 120268, 0, 0, 5479, 1);

	DELETE FROM creature_queststarter WHERE id=79826 AND quest=37571;
	INSERT INTO creature_queststarter(`id`, `quest`) values('79826','37571');

	DELETE FROM creature_questender WHERE id=86696 AND quest=37571;
	INSERT INTO creature_questender(`id`, `quest`) values('86696','37571');

	DELETE FROM gameobject_template WHERE entry=237139;
	INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(237139, 45, 20508, "Engineering Work Order", "", "", "", 0, 0, 1, 0, 0, 0, 0, 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19865) ON DUPLICATE KEY UPDATE entry=entry;
	UPDATE gameobject_template SET flags=0, ScriptName="go_garrison_shipment_container" WHERE TYPE=45;

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-91 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-91','0','-237139','8.33867','-7.83219','0','5.56061');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-91','0','86696','1.89719','-2.04317','1.33368','5.61557');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-91','0','79826','3.17767','1.83158','1.33375','6.0122');

	UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=88610;
	DELETE FROM `npc_vendor` WHERE `entry` = 88610 AND `type` = 1;
	INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
	(88610, 0, 109574, 0, 0, 0, 1), 
	(88610, 0, 109644, 0, 0, 0, 1), 
	(88610, 0, 117402, 0, 0, 0, 1), 
	(88610, 0, 117403, 0, 0, 0, 1), 
	(88610, 0, 117438, 0, 0, 0, 1);
	
	-- Walter
	UPDATE creature_template SET scriptname = "npc_GarrisonWalter" WHERE entry = 77894;

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-123 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-123','0','-237139','9.85514','-2.81477','0.556633','6.09036');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-123','0','86696','3.43377','4.39184','1.23864','4.41746');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-123','0','79826','1.84012','-5.34018','1.32708','4.37426');


-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-124 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-124','0','-237139','10.507','-2.36388','-0.001656','5.95291');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-124','0','86696','2.70305','4.24053','1.26994','4.92011');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-124','0','79826','1.91871','-5.07517','1.28078','4.17791');
