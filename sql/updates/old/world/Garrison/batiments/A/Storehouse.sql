-- LEVEL 1

	-- Guild Vault - 240228
	INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(240228, 34, 15449, "Guild Vault", "", "", "", 1732, 0, 0.65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `faction` = VALUES(`faction`), `flags` = VALUES(`flags`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);

	UPDATE creature_template SET npcFlag=npcFlag | 131072 WHERE entry=84857;
	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-51 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-51','1','84857','4.80532','-0.103392','1.29651','0.045699');
	UPDATE quest_template_objective SET flags = flags | 0x04 WHERE questid IN (37087, 37060); -- tempfix for first quest

	DELETE FROM creature_queststarter WHERE id = 84857;
	INSERT INTO `creature_queststarter`(`id`,`quest`) VALUES ('84857','37087');

	DELETE FROM creature_questender WHERE id = 84857;
	INSERT INTO `creature_questender`(`id`,`quest`) VALUES ('84857','37087');

-- LEVEL 2

	-- Guild Vault - 240228
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -142 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-142','1','84857','5.15749','-2.24324','1.30106','2.48612');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-142','1','-240228','1.84528','-2.10558','0.81321','1.44547');


-- LEVEL 3

	-- gardien du coffre sarniv - 85290
	-- tisse-dimensions tuviss - 85291
	UPDATE creature_template SET npcFlag = npcFlag | 268435456 WHERE entry = 85291;
	UPDATE creature_template SET npcFlag = npcFlag | 536870912 WHERE entry = 85290;
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -143 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','1','84857','5.63223','-1.75851','1.32661','2.27878');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','1','-240228','1.39855','-1.72972','0.81942','1.65439');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','1','85290','1.06823','2.79973','1.29814','4.741');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUE('-143','1','85291','-1.50635','-1.56967','1.2905','1.28916');
