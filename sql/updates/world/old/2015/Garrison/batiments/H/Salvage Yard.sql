-- LEVEL 1

	REPLACE INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(237172, 8, 22926, "L’Incinérateur", "", "", "", 0, 0, 1, 0, 0, 0, 0, 0, 0, 1853, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -52 AND faction_index = 0;
	insert into `garrison_plot_content` (`id`, `plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('676','-52','0','-237172','-1.62342','-0.523125','0.860039','0.107887');
	insert into `garrison_plot_content` (`id`, `plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('677','-52','0','79857','5.72437','-1.64478','1.36153','0.033265');

	UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=79857;
	DELETE FROM `npc_vendor` WHERE `entry` = 79857 AND `type` = 1;
	INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
	(79857, 0, 2880, 0, 0, 0, 1), 
	(79857, 0, 2901, 0, 0, 0, 1), 
	(79857, 0, 3466, 0, 0, 0, 1), 
	(79857, 0, 3857, 0, 0, 0, 1), 
	(79857, 0, 5956, 0, 0, 0, 1), 
	(79857, 0, 18567, 0, 0, 0, 1);

-- LEVEL 2

-- LEVEL 3