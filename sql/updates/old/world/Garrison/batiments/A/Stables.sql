-- LEVEL 1
	-- Stables


		
		UPDATE creature_template SET npcflag = npcflag | 1, scriptname = "npc_FannyFirebeard_Garr" WHERE entry = 86974;
		UPDATE creature_template SET npcflag = npcflag | 1, scriptname = "npc_KeeganFirebeard_Garr" WHERE entry = 86973;
		
		UPDATE creature_template SET vehicleID = 3889 WHERE entry IN (87076, 87078, 87079, 87080, 87081, 87082);

		INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(237169, 3, 7922, "Griffe noire de Sethe", "", "", "", 94, 0, 1, 0, 0, 0, 0, 0, 0, 57, 56642, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27647, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `faction` = VALUES(`faction`), `flags` = VALUES(`flags`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);
		INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(237170, 3, 18860, "Collier en dents de garn", "", "", "", 94, 0, 1, 0, 0, 0, 0, 0, 0, 57, 56643, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27647, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `faction` = VALUES(`faction`), `flags` = VALUES(`flags`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);


		-- Fanny Firebeard
		/* Quests list : 
		Lier un loup
		36914
		Dressage de loup : 
		37022
		37023
		37024
		37025
		37026
		37027
		37028
		Prendre un potamodonte
		36918
		Dressage de potamodonte : 
		37005
		37006
		37007
		37008
		37009
		37010
		37011
		37012
		37013
		Terrasser un Talbuk
		36911
		Dressage de Talbuk : 
		36971
		36972
		36973
		36974
		36975
		36976
		36977
		36978
		36979
		36980
		36981
		36982
		La dent de prédateur
		37122 (available level 3)
		La griffe noire
		37121 (available level 2)*/

		-- Keegan Firebeard
		/* Quests list : 
		Elever un Elekk*
		36915
		Dressage d'Elekk
		37015
		37016
		37017
		37018
		37019
		37020
		37021
		Capturer un Sabot-Fourchu
		36916
		Dressage de Sabot-Fourchu
		36983
		36984
		36985
		36986
		36987
		36988
		36989
		36990
		36991
		36992
		36993
		36994
		Sangler un Sanglier
		36913
		Dressage de Sanglier
		36995
		36996
		36997
		36998
		36999
		37000
		37001
		37002
		37003
		37004
		*/

-- Quest Fixes

	-- meadowstomper
	DELETE FROM creature WHERE id = 86852;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86852','1116','6755','6755','1','4294967295','0','0','3452.83','4478.39','160.287','0.105695','60','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86852','1116','6755','6755','1','4294967295','0','0','3454.9','4500.59','162.081','6.22269','60','0','0','55035','0','0','0','0','0','2048','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86852','1116','6755','7226','1','4294967295','0','0','3476.9','4512.22','164.766','4.03537','60','0','0','55035','0','0','0','0','0','2048','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86852','1116','6755','6755','1','4294967295','0','0','3478.57','4489.64','161.19','4.95429','60','0','0','55035','0','0','0','0','0','2048','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86852','1116','6755','6755','1','4294967295','0','0','3428.63','4496.14','161.886','4.20031','60','0','0','55035','0','0','0','0','0','2048','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86852','1116','6755','6755','1','4294967295','0','0','3399.89','4483.96','161.065','4.31811','60','0','0','55035','0','0','0','0','0','2048','0','0','0','0',NULL);

	DELETE FROM script_waypoint WHERE entry = 86852;
	INSERT INTO script_waypoint (entry, pointid, location_x, location_y, location_z) VALUES
	(86852, 0, 3461.2332, 4501.8013, 162.8392),
	(86852, 1, 3473.8462, 4503.0107, 163.7070),
	(86852, 2, 3486.0850, 4496.1191, 161.6440),
	(86852, 3, 3503.8198, 4484.0005, 161.5515),
	(86852, 4, 3520.8782, 4479.8193, 160.4291),
	(86852, 5, 3533.2112, 4479.9766, 158.2132),
	(86852, 6, 3555.9678, 4482.5674, 153.9302),
	(86852, 7, 3589.1233, 4499.9775, 148.2920),
	(86852, 8, 3608.0911, 4483.7832, 150.5127),
	(86852, 9, 3622.0200, 4475.0205, 159.8244),
	(86852, 10, 3605.2258, 4464.1753, 167.3931),
	(86852, 11, 3582.2375, 4453.8179, 183.5773),
	(86852, 12, 3563.7036, 4432.5845, 195.5525),
	(86852, 13, 3544.2903, 4432.4399, 200.2682),
	(86852, 14, 3526.4973, 4406.5225, 208.7544),
	(86852, 15, 3512.1736, 4392.6641, 217.1996),
	(86852, 16, 3493.1543, 4395.9990, 221.7722),
	(86852, 17, 3439.0730, 4393.7300, 227.7611), -- escort paused, jump
	-- (86852, 18, 3419.6072, 4388.9111, 228.3024),
	(86852, 18, 3404.6514, 4398.2324, 229.2862),
	(86852, 19, 3388.1492, 4394.8599, 229.0199),
	(86852, 20, 3360.3599, 4422.6021, 229.4613),
	(86852, 21, 3358.5996, 4483.6162, 214.4389),
	(86852, 22, 3347.9763, 4502.9644, 208.8071),
	(86852, 23, 3358.1404, 4530.1030, 198.1263),
	(86852, 24, 3328.0862, 4551.5718, 199.6204),
	(86852, 25, 3328.0862, 4551.5718, 199.6204),
	(86852, 26, 3327.4707, 4526.9248, 195.8049), -- escort paused, jump
	-- (86852, 28, 3323.6699, 4510.6533, 180.0031), -- escort paused, jump
	-- (86852, 29, 3322.0098, 4475.1860, 169.4178),
	(86852, 27, 3303.5442, 4460.9072, 175.7258),
	(86852, 28, 3309.4106, 4428.6411, 179.2745),
	(86852, 29, 3306.6887, 4393.2637, 190.1172); -- remove aura, kill credit, creature despawn

	-- riverwallow
	DELETE FROM creature WHERE id = 86848;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86848','1116','6662','6662','1','4294967295','0','0','2281','2109.8','105.69','1.48898','60','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86848','1116','6662','6662','1','4294967295','0','0','2256.34','2064.11','105.053','5.44216','60','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86848','1116','6662','6927','1','4294967295','0','0','2311.6','2087.36','104.779','3.2545','60','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86848','1116','6662','6662','1','4294967295','0','0','2279.17','2128.44','109.02','0.389416','60','0','0','31613','0','0','0','0','0','2048','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86848','1116','6662','6662','1','4294967295','0','0','2303.57','2142.72','111.49','0.342292','60','0','0','31613','0','0','0','0','0','2048','0','0','0','0',NULL);

	DELETE FROM script_waypoint WHERE entry = 86848;
	INSERT INTO script_waypoint (entry, pointid, location_x, location_y, location_z) VALUES
	(86848, 0, 2329.4868, 2105.2712, 107.2448),
	(86848, 1, 2351.1873, 2120.2751, 107.2448),
	(86848, 2, 2383.2788, 2138.2573, 107.2448),
	(86848, 3, 2414.6587, 2137.1733, 107.2448),
	(86848, 4, 2445.2168, 2126.9802, 107.2448),
	(86848, 5, 2466.8640, 2117.3088, 107.2459),
	(86848, 6, 2479.4407, 2130.0464, 107.2456),
	(86848, 7, 2487.4885, 2130.7446, 107.2468),
	(86848, 8, 2512.5242, 2122.5847, 107.2459),
	(86848, 9, 2530.2834, 2111.1304, 107.2454),
	(86848, 10, 2526.6626, 2102.2239, 107.2454),
	(86848, 11, 2553.8845, 2086.6619, 107.2458),
	(86848, 12, 2573.4333, 2066.9446, 107.2458),
	(86848, 13, 2580.0691, 2045.3123, 107.2458),
	(86848, 14, 2577.4753, 2010.3032, 107.2458),
	(86848, 15, 2567.5542, 1990.4546, 107.2458),
	(86848, 16, 2571.8577, 1962.7745, 107.2465),
	(86848, 17, 2560.5178, 1926.8342, 107.2460),
	(86848, 18, 2558.9658, 1901.3855, 107.4034),
	(86848, 19, 2556.4790, 1886.8954, 105.9909),
	(86848, 20, 2541.1614, 1866.1229, 107.2457),
	(86848, 21, 2509.6428, 1866.9553, 107.2457),
	(86848, 22, 2460.3684, 1873.9409, 110.0706),
	(86848, 23, 2432.0720, 1840.0482, 108.6715),
	(86848, 24, 2432.7585, 1829.6078, 108.3421),
	(86848, 25, 2425.6108, 1817.1708, 108.7563),
	(86848, 26, 2431.0066, 1811.3870, 107.5432),
	(86848, 27, 2470.6614, 1816.1877, 108.5347),
	(86848, 28, 2483.8865, 1820.9728, 108.1860); -- remove aura, kill credit, creature despawn

	-- snarler
	DELETE FROM creature WHERE id = 86851;
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2673.37','6138.44','103.466','1.929','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2661.53','6153.45','103.954','2.14792','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2678.47','6154.81','104.249','0.922451','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2629.17','6145.83','92.2418','4.53314','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2607.81','6140.93','93.4868','4.40023','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7172','1','1','0','0','2652.03','6063.66','81.1065','5.6895','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7172','1','1','0','0','2678.5','6105.22','89.4064','6.22434','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7172','1','1','0','0','2637.84','6052.02','81.6663','0.798997','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7172','1','1','0','0','2642.29','6050.16','81.6663','1.97349','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2607.61','6135.6','92.6105','3.67717','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2642.26','6185.96','92.8474','2.8464','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2654.07','6138.57','100.323','0.886977','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7151','1','1','0','0','2657.33','6162','104.141','1.73382','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86851','1116','6755','7172','1','1','0','0','2653.38','6103.06','88.5089','1.27885','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);

	UPDATE creature SET phasemask = 4294967295, spawntimesecs = 60 WHERE id = 86851;
	DELETE FROM script_waypoint WHERE entry = 86851;
	INSERT INTO script_waypoint (entry, pointid, location_x, location_y, location_z) VALUES
	(86851, 0, 2676.9797, 6117.9585, 93.9686),
	(86851, 1, 2661.1433, 6129.2295, 99.8117),
	(86851, 2, 2643.5398, 6125.2666, 93.6162),
	(86851, 3, 2628.2134, 6143.8159, 91.8304),
	(86851, 4, 2594.5562, 6144.9351, 91.9249), -- escort paused, movejump
	-- (86851, 5, 2572.6372, 6142.9878, 80.2156),
	(86851, 5, 2532.0461, 6144.8135, 80.4673), -- escort paused movejump
	-- (86851, 7, 2515.4197, 6132.1719, 80.0046),
	(86851, 6, 2522.0435, 6117.6670, 80.2096),
	(86851, 7, 2497.0620, 6111.9102, 80.4426), -- escort paused movejump
	-- (86851, 10, 2483.2539, 6104.6841, 83.2874),
	(86851, 8, 2485.7622, 6089.6416, 83.4058),
	(86851, 9, 2468.6960, 6080.5000, 86.3377),
	(86851, 10, 2468.9653, 6053.4287, 82.1083),
	(86851, 11, 2479.5999, 6033.1963, 79.6553),
	(86851, 12, 2483.7844, 6006.3252, 76.6586),
	(86851, 13, 2503.6831, 6007.7314, 76.8922),
	(86851, 14, 2512.2585, 5992.4316, 75.8708),
	(86851, 15, 2528.1631, 5986.1992, 77.8283),
	(86851, 16, 2531.3025, 5968.4341, 78.6686),
	(86851, 17, 2548.5698, 5948.5698, 78.5443),
	-- (86851, 21, 2559.3125, 5947.0806, 84.5657), -- escort paused movejump
	(86851, 18, 2576.8103, 5949.7192, 80.4125),
	(86851, 19, 2585.6558, 5965.5850, 81.4914),
	(86851, 20, 2605.6001, 5964.8330, 80.6792),
	(86851, 21, 2620.9834, 5941.4839, 79.6774), -- escort paused movejump
	-- (86851, 26, 2642.7153, 5938.2900, 84.2296),
	(86851, 22, 2658.7375, 5949.7607, 86.0734),
	(86851, 23, 2651.5349, 5963.5444, 84.9159), -- escort paused movejump
	-- (86851, 29, 2641.6606, 5977.7524, 82.7445),
	(86851, 24, 2619.3411, 5998.4346, 83.1915), -- escort paused movejump
	-- (86851, 31, 2611.9395, 6015.2808, 78.9488),
	(86851, 25, 2613.8584, 6024.2744, 80.3841),
	(86851, 26, 2609.6594, 6035.2539, 81.6320),
	(86851, 27, 2627.1196, 6050.0283, 81.2361),
	(86851, 28, 2639.4084, 6056.7354, 81.5824),
	(86851, 29, 2636.4233, 6067.9551, 81.5827),
	(86851, 30, 2648.1108, 6081.0977, 83.4802),
	(86851, 31, 2661.9114, 6084.0630, 83.6629),
	(86851, 32, 2679.1094, 6094.3452, 86.9982),
	(86851, 33, 2673.3152, 6116.1758, 93.4207),
	(86851, 34, 2678.9150, 6126.3066, 98.4885),
	(86851, 35, 2668.4287, 6132.3594, 101.3485),
	(86851, 36, 2673.7244, 6146.7188, 104.2849); -- remove aura, kill credit, creature despawn

	-- rockstuck

	DELETE FROM creature WHERE id = 86850;
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7596.09','1446.77','50.2197','5.94848','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7578.74','1463.83','55.5786','4.51454','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7556.6','1476.69','61.4301','4.92203','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7567.7','1452.04','54.7301','0.657216','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7606','1471.35','50.2789','3.14159','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7568.13','1413.74','48.4084','0.612269','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7528.8','1462.74','66.4659','2.89455','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7572.3','1432.83','48.3978','4.48904','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7582.54','1547.95','74.1939','5.48949','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7575.79','1532.7','74.6137','1.14388','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6934','1','4294967295','0','0','7542.04','1525.01','72.3002','5.61322','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6934','1','4294967295','0','0','7531.12','1505.71','72.1941','5.41567','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86850','1116','6721','6892','1','4294967295','0','0','7598.55','1528.25','74.7387','3.34596','60','10','0','1','0','1','0','0','32768','2048','0','0','0','0',NULL);

	UPDATE creature SET phasemask = 4294967295, spawntimesecs = 60 WHERE id = 86850;
	DELETE FROM script_waypoint WHERE entry = 86850;
	INSERT INTO script_waypoint (entry, pointid, location_x, location_y, location_z) VALUES
	(86850, 0, 7557.2720, 1460.8109, 60.0041),
	(86850, 1, 7545.3936, 1459.0100, 59.1350),
	(86850, 2, 7551.3516, 1447.1659, 55.4001),
	(86850, 3, 7544.3911, 1435.5256, 56.0667),
	(86850, 4, 7550.4268, 1418.7128, 55.6617),
	(86850, 5, 7539.0024, 1405.8782, 57.0709),
	(86850, 6, 7544.2178, 1389.5587, 48.5036),
	(86850, 7, 7535.5278, 1367.1577, 52.3421),
	(86850, 8, 7526.4277, 1342.4080, 51.9569),
	(86850, 9, 7527.3999, 1326.8324, 54.0550),
	(86850, 10, 7511.9814, 1309.8945, 54.9479),
	(86850, 11, 7511.8462, 1279.1993, 56.4777),
	(86850, 12, 7490.2520, 1267.9197, 56.7565),
	(86850, 13, 7485.4551, 1256.4960, 56.0274),
	(86850, 14, 7471.7314, 1272.1776, 63.8047),
	(86850, 15, 7466.3188, 1287.0187, 74.9085),
	(86850, 16, 7454.3384, 1294.9656, 86.0552),
	(86850, 17, 7436.2837, 1288.9624, 102.2655),
	(86850, 18, 7425.2104, 1302.5812, 112.4546),
	(86850, 19, 7413.6455, 1306.3721, 116.9519),
	(86850, 20, 7399.1611, 1298.0750, 107.8482),
	(86850, 21, 7389.5659, 1303.6643, 110.1741),
	(86850, 22, 7383.9888, 1284.7167, 106.9273),
	(86850, 23, 7392.8438, 1277.1240, 106.3759),
	(86850, 24, 7405.0439, 1297.2078, 111.4743),
	(86850, 25, 7417.7651, 1304.7021, 115.2764),
	(86850, 26, 7412.9111, 1319.3885, 125.1240),
	(86850, 27, 7402.5361, 1330.6210, 131.5959),
	(86850, 28, 7388.8955, 1325.7338, 129.4975),
	(86850, 29, 7356.5518, 1333.7715, 116.6700),
	(86850, 30, 7354.4355, 1338.9290, 113.8619),
	(86850, 31, 7330.8584, 1314.2473, 110.2433),
	(86850, 32, 7328.8418, 1287.9463, 98.2216),
	(86850, 33, 7306.5801, 1286.7927, 88.6961),
	(86850, 34, 7308.3906, 1270.1785, 89.8727),
	(86850, 35, 7319.6782, 1264.0875, 86.8079),
	(86850, 36, 7330.0688, 1248.5522, 87.9326),
	(86850, 37, 7314.2544, 1240.4128, 81.5284),
	(86850, 38, 7284.5884, 1244.4735, 74.0562),
	(86850, 39, 7269.5684, 1235.5865, 73.1700),
	(86850, 40, 7256.0757, 1262.4355, 70.6940),
	(86850, 41, 7231.5620, 1256.6781, 66.4054),
	(86850, 42, 7211.3574, 1274.7236, 64.6157),
	(86850, 43, 7214.6519, 1308.1775, 63.7093),
	(86850, 44, 7214.7734, 1327.0421, 60.7605),
	(86850, 45, 7193.4229, 1331.1555, 56.6156); -- remove aura, kill credit, creature despawn

	-- silverpelt

	DELETE FROM creature WHERE id = 86801;
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1285.16','-1159.4','-41.3644','0.457098','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1324.48','-1185.82','-21.5943','4.09948','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1327.08','-1173.15','-23.1775','5.28718','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1312.13','-1138.05','-38.1479','5.03043','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1300.1','-1184.81','-30.236','3.70195','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1294.23','-1182.48','-32.4386','2.40694','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1271.44','-1205.08','-30.9682','3.27092','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1284.2','-1112.52','-45.2365','0.0214811','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1292.25','-1199.39','-28.7153','0.881302','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1305.87','-1133.04','-40.5794','1.97415','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1320.85','-1139.06','-34.7458','1.00571','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1306.95','-1119.68','-39.7318','3.60306','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1317.41','-1113.13','-36.1232','2.5665','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1323.37','-1168.51','-24.9395','4.41355','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1275.34','-1108.26','-48.3947','3.6926','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1280.07','-1169','-40.6029','4.70862','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1301.77','-1121.35','-41.3244','0.496852','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1320.86','-1111.05','-35.507','2.78183','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1325.68','-1183.97','-21.7386','4.45745','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1333.32','-1143.49','-29.9662','6.17167','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1297.11','-1197.42','-28.3782','0.805321','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1278.57','-1162.43','-42.8712','2.95942','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1316.44','-1145.08','-35.0154','1.78851','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1304.35','-1125.9','-40.835','5.81736','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1275.63','-1204.41','-30.2737','0.00195312','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1319.93','-1114.76','-36.1128','0.755085','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1289.6','-1198.2','-29.3891','5.04741','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1280.96','-1154.33','-44.7602','3.37541','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1275.6','-1113.13','-49.1176','0.0410327','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1289.08','-1198.75','-29.4995','2.12009','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1271.94','-1203.69','-31.3032','3.16147','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1290.28','-1184.29','-32.5846','4.15982','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1328.73','-1182.37','-21.5801','1.382','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1301.94','-1125.08','-41.4354','4.85066','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1326.4','-1146.82','-30.8409','0.266766','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1309.81','-1146.12','-36.7848','1.90397','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86801','1116','6719','7492','1','1','0','0','1316.55','-1114.1','-36.4094','5.3535','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);

	UPDATE creature SET phasemask = 4294967295, spawntimesecs = 60 WHERE id = 86801;
	DELETE FROM script_waypoint WHERE entry = 86801;
	INSERT INTO script_waypoint (entry, pointid, location_x, location_y, location_z) VALUES
	(86801, 0, 1332.6537, -1202.8055, -18.4024),
	(86801, 1, 1323.9479, -1216.2640, -17.9684),
	(86801, 2, 1324.8407, -1230.8895, -15.8669),
	(86801, 3, 1316.6874, -1247.7476, -15.1503),
	(86801, 4, 1329.8785, -1255.8800, -12.0278),
	(86801, 5, 1349.4236, -1256.1873, -5.9662),
	(86801, 6, 1358.5023, -1274.7120, 0.0370),
	(86801, 7, 1366.1583, -1277.1997, 3.6541),
	(86801, 8, 1374.8246, -1300.9353, 7.2106),
	(86801, 9, 1395.4918, -1317.6079, 10.0820),
	(86801, 10, 1414.2557, -1296.6494, 11.6101),
	(86801, 11, 1444.8483, -1287.6680, 15.1892),
	(86801, 12, 1461.0919, -1259.1460, 16.0972),
	(86801, 13, 1463.3787, -1235.9309, 14.3068),
	(86801, 14, 1482.4413, -1202.3406, 5.8031),
	(86801, 15, 1509.0601, -1182.2521, 7.7290),
	(86801, 16, 1516.7192, -1154.4749, 2.4798),
	(86801, 17, 1545.3916, -1160.0406, 7.8416),
	(86801, 18, 1590.3232, -1137.4006, 11.5131),
	(86801, 19, 1608.3828, -1121.2640, 19.4202),
	(86801, 20, 1586.3698, -1104.6002, 25.9739),
	(86801, 21, 1577.0366, -1096.1448, 26.8856),
	(86801, 22, 1577.0366, -1096.1448, 26.8856),
	(86801, 23, 1545.9785, -1082.4507, 26.6608),
	(86801, 24, 1540.8218, -1065.8976, 24.6720), -- escort paused, movejump
	-- (86801, 25, 1511.6106, -1065.3923, 12.7051), -- escort paused, movejump
	-- (86801, 26, 1470.9410, -1051.7019, -4.3017),
	(86801, 25, 1438.3502, -1055.6283, -8.7313),
	(86801, 26, 1422.6118, -1035.8879, -6.8786),
	(86801, 27, 1424.6046, -1017.0059, -4.8030),
	(86801, 28, 1413.8878, -991.0676, -10.4500),
	(86801, 31, 1394.2507, -983.7310, -19.1788),
	(86801, 29, 1373.5912, -952.8008, -25.4474),
	(86801, 30, 1359.6609, -924.2994, -29.8936); -- remove aura, kill credit, creature despawn

	-- icehoof

	DELETE FROM creature WHERE id = 86847;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7441.05','4190.83','117.388','4.68102','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7511.68','4244.83','115.842','3.46769','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7518.67','4214.43','118.885','5.51695','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7487.57','4252.79','117.055','3.34307','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7442.9','4210.86','117.633','2.0104','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7390.87','4251.93','114.548','0.966374','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7462.21','4102.75','117.893','3.46097','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7362.17','4241.97','111.34','1.57484','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7488.16','4178.62','117.8','2.03092','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7361.45','4245.05','111.319','1.67129','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7507.42','4248.62','115.538','5.67022','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7479.01','4198.21','116.605','1.43444','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7524.8','4209.01','118.432','0.725456','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7454.13','4225.96','115.243','2.19672','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7498.36','4135.92','119.355','0.198298','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7472.02','4140.16','117.721','0.184927','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7458.65','4228.66','114.746','4.42738','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7489.07','4252.03','117.184','1.80601','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7575.79','4205.15','111.207','0.0253852','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7534.69','4180.34','119.642','0.89819','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7355.86','4272.87','112.429','3.30385','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('86847','1116','6720','7006','1','1','0','0','7423.74','4242.29','114.817','2.13593','120','10','0','1','0','1','0','0','0','0','0','0','0','0',NULL);

	UPDATE creature SET phasemask = 4294967295, spawntimesecs = 60 WHERE id = 86847;
	DELETE FROM script_waypoint WHERE entry = 86847;
	INSERT INTO script_waypoint (entry, pointid, location_x, location_y, location_z) VALUES
	(86847, 0, 7436.7007, 4193.4600, 116.9164),
	(86847, 1, 7445.5190, 4188.5757, 117.4695),
	(86847, 2, 7458.3018, 4174.2148, 115.4373),
	(86847, 3, 7471.3784, 4157.4482, 117.3632),
	(86847, 4, 7476.5625, 4149.6069, 118.4125),
	(86847, 5, 7487.2974, 4174.7310, 117.6812),
	(86847, 6, 7511.4761, 4177.7891, 117.5897),
	(86847, 7, 7499.5254, 4204.0132, 115.9189),
	(86847, 8, 7470.9966, 4211.5444, 114.6070),
	(86847, 9, 7447.7397, 4205.4595, 116.9335),
	(86847, 10, 7433.6274, 4190.6646, 116.0175),
	(86847, 11, 7408.6660, 4200.1836, 113.8156),
	(86847, 12, 7391.2896, 4181.0776, 113.0883),
	(86847, 13, 7369.9282, 4189.4204, 111.0339),
	(86847, 14, 7372.0107, 4216.2979, 112.3856),
	(86847, 15, 7367.8115, 4242.9312, 111.5507),
	(86847, 16, 7343.0024, 4237.2051, 110.3054),
	(86847, 17, 7319.9727, 4214.6753, 107.6676),
	(86847, 18, 7310.6465, 4195.5283, 106.5107),
	(86847, 19, 7327.0562, 4178.9478, 107.8452),
	(86847, 20, 7328.9087, 4159.8213, 110.1952),
	(86847, 21, 7330.5947, 4124.3291, 116.4928),
	(86847, 22, 7356.3286, 4116.8530, 119.5312),
	(86847, 23, 7360.8174, 4096.5713, 121.1069),
	(86847, 24, 7380.3477, 4075.5122, 122.8589),
	(86847, 25, 7406.1401, 4064.9331, 124.2646),
	(86847, 26, 7435.5610, 4071.1223, 118.1252),
	(86847, 27, 7451.4526, 4089.9060, 117.1178),
	(86847, 28, 7463.2705, 4096.2842, 117.5267),
	(86847, 29, 7488.8760, 4096.0269, 117.0659),
	(86847, 30, 7514.9370, 4083.0352, 118.3340),
	(86847, 31, 7535.9951, 4098.4873, 119.2219),
	(86847, 32, 7561.4048, 4141.1050, 116.6906),
	(86847, 33, 7523.4565, 4150.8755, 120.0477),
	(86847, 34, 7499.6455, 4146.6831, 118.2433),
	(86847, 35, 7490.7681, 4137.2061, 119.5016),
	(86847, 36, 7463.8989, 4165.2490, 116.6490),
	(86847, 37, 7453.4453, 4171.9570, 117.0677),
	(86847, 38, 7445.2656, 4193.7285, 117.9744),
	(86847, 39, 7439.0786, 4225.4712, 116.6916),
	(86847, 40, 7416.1885, 4253.0947, 114.2837),
	(86847, 41, 7457.0605, 4265.3540, 114.6554),
	(86847, 42, 7466.2139, 4279.8418, 114.9435),
	(86847, 43, 7444.4238, 4310.5942, 116.3136); -- remove aura, kill credit, creature despawn

	-- GENERIC SCRIPT
	UPDATE creature_template SET scriptname = "npc_GarrisonStablesCreatures" WHERE entry in (86847, 86850, 86801, 86852, 86848, 86851);
	REPLACE INTO spell_script_names VALUES
	(173686, "spell_garrison_stables_lasso"),
	(174070, "spell_garrison_stables_lasso");

	REPLACE INTO spell_script_names VALUES
	(174221, "spell_garrison_stables_training_mounts"),
	(174219, "spell_garrison_stables_training_mounts"),
	(174218, "spell_garrison_stables_training_mounts"),
	(174216, "spell_garrison_stables_training_mounts"),
	(174220, "spell_garrison_stables_training_mounts"),
	(174222, "spell_garrison_stables_training_mounts");

	-- /!\ HOTFIX DATABASE /!\
	REPLACE INTO mount (ID, SpellID, Category, CreatureDisplayID, Flags, `Name`, Description) VALUES 
	(600002, 174216, 230, 59757, 0, "Snarler in Training", "Summons a Snarler in training"), -- grogneur/loup/snarler/wolf - 86851
	(600003, 174218, 230, 59320, 0, "Icehoof in Training", "Summons an Icehoof in training"),-- sabot-fourchu/clefthoof/sabot glacé/icehoof - 86847
	(600004, 174219, 230, 59340, 0, "Meadowstomper in Training", "Summons a Meadowstomper in training"),-- foule-prairie/Meadowstomper - 86852
	(600005, 174220, 230, 59743, 0, "Riverwallow in Training", "Summons a Riverwallow in training"), -- bourbefleuve/potamodonte/Riverwallow - 86848
	(600006, 174221, 230, 59735, 0, "Rocktusk in Training", "Summons an Rocktusk in training"), -- Brochepierre/Rocktusk - 86850
	(600007, 174222, 230, 59365, 0, "Silverpelt in Training", "Summons an Silverpelt in training"); -- Pelargent/Silverpelt - 86801

-- Quest Relations

	-- FannyQuestGiver

	REPLACE INTO creature_queststarter VALUES
	(86974, 36915),
	(86974, 37015),
	(86974, 37016),
	(86974, 37017),
	(86974, 37018),
	(86974, 37019),
	(86974, 37020),
	(86974, 37021),
	(86974, 36916),
	(86974, 36983),
	(86974, 36984),
	(86974, 36985),
	(86974, 36986),
	(86974, 36987),
	(86974, 36988),
	(86974, 36989),
	(86974, 36990),
	(86974, 36991),
	(86974, 36992),
	(86974, 36993),
	(86974, 36994),
	(86974, 36913),
	(86974, 36995),
	(86974, 36996),
	(86974, 36997),
	(86974, 36998),
	(86974, 36999),
	(86974, 37000),
	(86974, 37001),
	(86974, 37002),
	(86974, 37003),
	(86974, 37004);

	REPLACE INTO creature_questender VALUES
	(86974, 36915),
	(86974, 37015),
	(86974, 37016),
	(86974, 37017),
	(86974, 37018),
	(86974, 37019),
	(86974, 37020),
	(86974, 37021),
	(86974, 36916),
	(86974, 36983),
	(86974, 36984),
	(86974, 36985),
	(86974, 36986),
	(86974, 36987),
	(86974, 36988),
	(86974, 36989),
	(86974, 36990),
	(86974, 36991),
	(86974, 36992),
	(86974, 36993),
	(86974, 36994),
	(86974, 36913),
	(86974, 36995),
	(86974, 36996),
	(86974, 36997),
	(86974, 36998),
	(86974, 36999),
	(86974, 37000),
	(86974, 37001),
	(86974, 37002),
	(86974, 37003),
	(86974, 37004);

	-- KeeganQuestGiver

	REPLACE INTO creature_queststarter VALUES
	(86973, 36914),
	(86973, 37022),
	(86973, 37023),
	(86973, 37024),
	(86973, 37025),
	(86973, 37026),
	(86973, 37027),
	(86973, 37028),
	(86973, 36918),
	(86973, 37005),
	(86973, 37006),
	(86973, 37007),
	(86973, 37008),
	(86973, 37009),
	(86973, 37010),
	(86973, 37011),
	(86973, 37012),
	(86973, 37013),
	(86973, 36911),
	(86973, 36971),
	(86973, 36972),
	(86973, 36973),
	(86973, 36974),
	(86973, 36975),
	(86973, 36976),
	(86973, 36977),
	(86973, 36978),
	(86973, 36979),
	(86973, 36980),
	(86973, 36981),
	(86973, 36982);

	REPLACE INTO creature_questender VALUES
	(86973, 36914),
	(86973, 37022),
	(86973, 37023),
	(86973, 37024),
	(86973, 37025),
	(86973, 37026),
	(86973, 37027),
	(86973, 37028),
	(86973, 36918),
	(86973, 37005),
	(86973, 37006),
	(86973, 37007),
	(86973, 37008),
	(86973, 37009),
	(86973, 37010),
	(86973, 37011),
	(86973, 37012),
	(86973, 37013),
	(86973, 36911),
	(86973, 36971),
	(86973, 36972),
	(86973, 36973),
	(86973, 36974),
	(86973, 36975),
	(86973, 36976),
	(86973, 36977),
	(86973, 36978),
	(86973, 36979),
	(86973, 36980),
	(86973, 36981),
	(86973, 36982);

	-- SagePalunaQuestGiver

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

	-- TormakQuestGiver

	REPLACE INTO creature_queststarter VALUES
	(87242, 36946),
	(87242, 37063),
	(87242, 37064),
	(87242, 37065),
	(87242, 37066),
	(87242, 37067),
	(87242, 37068),
	(87242, 37069),
	(87242, 36912),
	(87242, 37048),
	(87242, 37049),
	(87242, 37050),
	(87242, 37051),
	(87242, 37052),
	(87242, 37053),
	(87242, 37054),
	(87242, 37055),
	(87242, 37056),
	(87242, 37057),
	(87242, 37058),
	(87242, 37059),
	(87242, 36944),
	(87242, 37032),
	(87242, 37033),
	(87242, 37034),
	(87242, 37035),
	(87242, 37036),
	(87242, 37037),
	(87242, 37038),
	(87242, 37039),
	(87242, 37040),
	(87242, 37041);

	REPLACE INTO creature_questender VALUES
	(87242, 36946),
	(87242, 37063),
	(87242, 37064),
	(87242, 37065),
	(87242, 37066),
	(87242, 37067),
	(87242, 37068),
	(87242, 37069),
	(87242, 36912),
	(87242, 37048),
	(87242, 37049),
	(87242, 37050),
	(87242, 37051),
	(87242, 37052),
	(87242, 37053),
	(87242, 37054),
	(87242, 37055),
	(87242, 37056),
	(87242, 37057),
	(87242, 37058),
	(87242, 37059),
	(87242, 36944),
	(87242, 37032),
	(87242, 37033),
	(87242, 37034),
	(87242, 37035),
	(87242, 37036),
	(87242, 37037),
	(87242, 37038),
	(87242, 37039),
	(87242, 37040),
	(87242, 37041);
	
	-- plot content

		DELETE FROM garrison_plot_content WHERE plot_type_or_building = -65 AND faction_index = 1; -- horde
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-65','1','86974','2.35014','0.122752','1.14831','0.745757');

-- LEVEL 2

		DELETE FROM garrison_plot_content WHERE plot_type_or_building = -66 AND faction_index = 1; -- alliance
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-66','1','-237169','19.4235','-5.23777','0.234062','0.051848');
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-66','1','86974','-4.2947','-5.08406','0.587532','0.130389');


-- LEVEL 3

		DELETE FROM garrison_plot_content WHERE plot_type_or_building = -67 AND faction_index = 1; -- alliance
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-67','1','86974','2.15983','2.7714','0.822304','5.07054');
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-67','1','-237169','17.7012','-5.48385','0.06636','1.70118');
		insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-67','1','-237170','18.2435','-9.17132','0.154655','3.04814');
