-- LEVEL 1

	-- Guild Vault - 205494
	INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(205494, 34, 7605, "Guild Vault", "", "", "", 2159, 0, 1.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `faction` = VALUES(`faction`), `flags` = VALUES(`flags`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);

	UPDATE creature_template SET npcFlag = npcFlag | 131072 WHERE entry=79862;
	REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `AIName`, `ScriptName`, `BuildVerified`) VALUE('237257','3','17115','Crate of Surplus Material','questinteract','Gathering','','0','0','1.5','118417','0','0','0','0','0','43','-1','0','0','1','0','-1','0','37060','0','0','0','0','0','24982','0','0','29244','0','0','0','0','0','0','0','0','0','0','0','0','56620','1','0','0','','','1');

	REPLACE INTO creature_queststarter VALUE (79862, 37060);
	REPLACE INTO creature_questender VALUE (79862, 37060);
	UPDATE quest_template_objective SET flags = flags | 0x04 WHERE questid IN (37087, 37060); -- tempfix for first quest
	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-51 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-51','0','79862','1.66546','-0.009018','1.31065','-0.01372');

-- LEVEL 2

	-- Guild Vault - 205494
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -142 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-142','0','-205494','2.30385','-5.72452','0.393913','2.54402');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-142','0','79862','1.97064','3.37736','0.893913','5.87175');


-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -143 AND faction_index = 0;

	-- Gardien du coffre Maeelthas
	UPDATE creature_template SET npcFlag=npcFlag | 536870912 WHERE entry = 85288;

	-- Tisse-dimensions Farshlah
	UPDATE creature_template SET npcFlag=npcFlag | 268435456 WHERE entry = 85289;
	
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','0','79862','3.18113','2.77264','1.22987','5.81442');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','0','-205494','0.577961','-2.4591','0.729866','1.64788');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','0','85288','-0.986507','5.12795','1.31058','5.15077');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','0','85289','-1.83379','-1.11583','1.31058','1.53402');

