-- LEVEL 1

	REPLACE INTO creature_template (entry, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(77359, "Auria Irondreamer", "Forgeronne", "trainer", 0, 0, 7, 53956, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1733, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	REPLACE INTO creature_template (entry, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(77792, "Yulia Samras", "Assistante de la forgeronne", "pickup", 0, 0, 7, 54624, 54625, 54626, 54627, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1733, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	UPDATE `creature_template_addon` SET `bytes1`='0',`bytes2`='0' WHERE `entry`='77792';

	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(53956, 0.208, 1.5, 1) ON DUPLICATE KEY UPDATE `modelid` = VALUES(`modelid`), `bounding_radius` = VALUES(`bounding_radius`), `combat_reach` = VALUES(`combat_reach`), `gender` = VALUES(`gender`);
	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(54624, 0.208, 1.5, 1) ON DUPLICATE KEY UPDATE `modelid` = VALUES(`modelid`), `bounding_radius` = VALUES(`bounding_radius`), `combat_reach` = VALUES(`combat_reach`), `gender` = VALUES(`gender`);
	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(54625, 0.208, 1.5, 1) ON DUPLICATE KEY UPDATE `modelid` = VALUES(`modelid`), `bounding_radius` = VALUES(`bounding_radius`), `combat_reach` = VALUES(`combat_reach`), `gender` = VALUES(`gender`);
	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(54626, 0.208, 1.5, 1) ON DUPLICATE KEY UPDATE `modelid` = VALUES(`modelid`), `bounding_radius` = VALUES(`bounding_radius`), `combat_reach` = VALUES(`combat_reach`), `gender` = VALUES(`gender`);
	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(54627, 0.208, 1.5, 1) ON DUPLICATE KEY UPDATE `modelid` = VALUES(`modelid`), `bounding_radius` = VALUES(`bounding_radius`), `combat_reach` = VALUES(`combat_reach`), `gender` = VALUES(`gender`);

	UPDATE creature_template SET npcFlag=131, npcFlag2=128, minlevel=90, maxlevel=90, gossip_menu_id = 0, ScriptName="npc_AuriaIrondreamer_Garr" WHERE entry = 77359;
	UPDATE creature_template SET npcFlag=3, npcFlag2=32, minlevel=90, maxlevel=90, gossip_menu_id = 0, ScriptName="npc_YuliaSamras_Garr", IconName = "workorders" WHERE entry = 77792;

	DELETE FROM `npc_vendor` WHERE `entry` = 77359 AND `type` = 1;
	INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
	(77359, 0, 115356, 0, 0, 0, 1), 
	(77359, 0, 116726, 0, 0, 0, 1), 
	(77359, 0, 116727, 0, 0, 0, 1), 
	(77359, 0, 116728, 0, 0, 0, 1), 
	(77359, 0, 116729, 0, 0, 5336, 1), 
	(77359, 0, 116730, 0, 0, 5336, 1), 
	(77359, 0, 116731, 0, 0, 5336, 1), 
	(77359, 0, 116732, 0, 0, 5336, 1), 
	(77359, 0, 116733, 0, 0, 5336, 1), 
	(77359, 0, 116734, 0, 0, 5336, 1), 
	(77359, 0, 116735, 0, 0, 5336, 1), 
	(77359, 0, 116736, 0, 0, 5336, 1), 
	(77359, 0, 116737, 0, 0, 5336, 1), 
	(77359, 0, 116738, 0, 0, 5336, 1), 
	(77359, 0, 116739, 0, 0, 5336, 1), 
	(77359, 0, 116740, 0, 0, 5336, 1), 
	(77359, 0, 116741, 0, 0, 5336, 1), 
	(77359, 0, 116742, 0, 0, 5336, 1), 
	(77359, 0, 116743, 0, 0, 5337, 1), 
	(77359, 0, 116744, 0, 0, 5337, 1), 
	(77359, 0, 116745, 0, 0, 5337, 1), 
	(77359, 0, 116746, 0, 0, 5337, 1), 
	(77359, 0, 116764, 0, 0, 5336, 1), 
	(77359, 0, 118044, 0, 0, 5336, 1), 
	(77359, 0, 119329, 0, 0, 5336, 1), 
	(77359, 0, 120260, 0, 0, 5336, 1), 
	(77359, 0, 120262, 0, 0, 5336, 1);

	DELETE FROM creature_queststarter WHERE id=77359 AND quest=35168;
	INSERT INTO creature_queststarter(`id`, `quest`) values('77359','35168');

	DELETE FROM creature_questender WHERE id=77792 AND quest=35168;
	INSERT INTO creature_questender(`id`, `quest`) values('77792','35168');

	REPLACE INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(227910, 8, 14650, "Anvil", "", "", "", 0, 64, 1, 0, 0, 0, 0, 0, 0, 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	REPLACE INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(227916, 5, 14839, "Table", "", "", "", 0, 0, 0.92, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	REPLACE INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(227917, 5, 15461, "Iron Bar", "", "", "", 0, 0, 0.95, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	REPLACE INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(227915, 8, 14649, "Forge", "", "", "", 0, 0, 0.83, 0, 0, 0, 0, 0, 0, 3, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	REPLACE INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(236640, 45, 15585, "Blacksmithing Work Order", "", "", "", 0, 0, 1, 0, 0, 0, 0, 0, 0, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	UPDATE gameobject_template SET flags=0, ScriptName="go_garrison_shipment_container" WHERE TYPE=45;

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -60 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','-227915','1.81854','6.49117','0.633835','4.73481');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','-227910','1.52175','-0.977332','0.628296','4.14969');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','-236640','7.75991','-2.93461','0','1.58536');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','-227917','1.64237','-6.65966','1.99026','1.58536');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','-227916','-3.32278','-2.31376','0.701897','1.58929');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','77359','-2.07944','4.28341','1.27255','0.114686');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','77792','5.22097','-2.88356','1.30399','6.18021');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-60','1','-192628','1.63364','-1.04777','1.74815','2.40128');

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -117 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-117','1','-227915','0.854408','5.93353','0.771446','4.69644');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-117','1','-236640','7.18639','-3.83883','0.004173','6.09051');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-117','1','77359','0.657339','2.76521','1.29008','1.77082');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-117','1','77792','1.77735','-5.0696','1.27263','1.48022');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-117','1','-192628','-0.386163','-2.44214','0.763718','0.430717');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -118 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-118','1','-227915','-0.127898','6.06771','0.771469','4.8378');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-118','1','-236640','7.24517','-2.48403','0.004456','6.13763');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-118','1','77359','-0.134707','2.72505','1.27319','1.77867');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-118','1','77359','1.53842','-5.42674','1.27296','1.6373');
