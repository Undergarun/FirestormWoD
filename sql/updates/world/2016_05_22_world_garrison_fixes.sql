-- Altar of bones

	UPDATE creature_template SET minlevel = 100, maxlevel = 100, npcflag = npcflag | 0x01000000 | 0x00000001, npcflag2 = npcflag2 | 0x00000020, unit_flags2 = unit_flags2 | 0x04000000 | 0x00008000 | 0x00002000, unit_flags = unit_flags | 0x00000004 | 0x00000100, scriptname = "npc_AltarOfBones", inhabittype = 4 WHERE entry = 86639;

-- Kuros - 86677

	UPDATE creature_template SET scriptname = "npc_Kuros_Garr", InhabitType = 4, npcflag = 3 WHERE entry = 86677;
	
	replace into creature_queststarter value (86677, 36876);
	replace into creature_questender value (86677, 36876);
	
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -159 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','86639','4.61352','0.178786','0.991531','6.04645');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','-236744','3.5327','4.20775','0.605667','6.07079');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','-236721','3.40289','4.23528','1.45593','5.94908');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','86677','4.97842','0.486778','2.2811','0.16855');
	
	-- Raza'kul - 86629

	UPDATE creature_template SET scriptname = "npc_RazaKul_Garr", InhabitType = 4, npcflag = 3 WHERE entry = 86629;
	
	REPLACE INTO creature_queststarter VALUE (86629, 36874);
	REPLACE INTO creature_questender VALUE (86629, 36874);
	
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -159 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','86629','4.04582','-1.19722','1.76241','0.209029');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','-236744','5.242','4.2493','0.541428','5.88353');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','-236721','5.242','4.2493','1.55293','5.82463');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','86639','4.37512','-0.176027','1.08931','0.008749');
	
	-- LEVEL 1

	-- Zee <Master Steamsmith>
	-- Lunarfall Engineer (84980)

	-- Plot content
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -162 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-162','1','84286','9.4447','-13.7488','1.17109','0.114666'),
	('-162','1','84980','7.97175','-9.43137','1.22384','2.14492'),
	('-162','1','84980','11.9157','-11.5916','1.20908','4.70767');

	UPDATE creature_template SET faction = 35 WHERE entry IN (84286, 84980);

	-- 84286/Zee <Master Steamsmith>
	REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES('84286','37091');
	REPLACE INTO `creature_questender` (`id`, `quest`) VALUES('84286','37091');

	UPDATE creature_template SET scriptname = "npc_Zee_Garrison" WHERE entry = 84286;


	UPDATE quest_template SET rewardmoney = 114000, RewardMoneyMaxLevel = 5 WHERE id = 37091;

	-- Overcharged Siege Engine
	UPDATE creature_template SET vehicleid = 3946, minlevel = 100, maxlevel = 100, faction = 2203, spell1 = 177291, spell2 = 177290, spell3 = 177293, spell4 = 177292 WHERE entry = 88694;

	-- Overcharged Demolisher
	UPDATE creature_template SET vehicleid = 3946, minlevel = 100, maxlevel = 100, faction = 2203, spell1 = 177291, spell2 = 177290, spell3 = 177293, spell4 = 177292 WHERE entry = 88695;

	-- Sentry Turret
	UPDATE creature_template SET minlevel = 100, maxlevel = 100, faction = 2203, unit_flags = 33554436, scriptname = "npc_GearshopWorkshopTurret_Garr" WHERE entry =  84143;

-- LEVEL 2

-- LEVEL 3

	-- plot content

		DELETE FROM garrison_plot_content WHERE plot_type_or_building = -65 AND faction_index = 1; -- alliance
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

		
			REPLACE INTO gameobject_loot_template VALUES
	(237469, 118624, -100, 1, 1, 1, 1, ''),
	(237468, 118623, -100, 1, 1, 1, 1, ''),
	(237481, 118650, -100, 1, 1, 1, 1, ''),
	(237461, 118616, -100, 1, 1, 1, 1, ''),
	(237463, 118618, -100, 1, 1, 1, 1, ''),
	(237465, 118620, -100, 1, 1, 1, 1, ''),
	(237480, 118649, -100, 1, 1, 1, 1, ''),
	(237477, 118646, -100, 1, 1, 1, 1, ''),
	(237478, 118647, -100, 1, 1, 1, 1, ''),
	(237467, 118622, -100, 1, 1, 1, 1, ''),
	(237466, 118621, -100, 1, 1, 1, 1, ''),
	(237462, 118617, -100, 1, 1, 1, 1, ''),
	(237464, 118619, -100, 1, 1, 1, 1, ''),
	(237479, 118648, -100, 1, 1, 1, 1, ''),
	(237483, 118652, -100, 1, 1, 1, 1, ''),
	(237472, 118627, -100, 1, 1, 1, 1, ''),
	(237473, 118628, -100, 1, 1, 1, 1, ''),
	(237470, 118625, -100, 1, 1, 1, 1, ''),
	(237471, 118626, -100, 1, 1, 1, 1, ''),
	(237482, 118651, -100, 1, 1, 1, 1, ''),
	(237474, 118643, -100, 1, 1, 1, 1, ''),
	(237475, 118644, -100, 1, 1, 1, 1, ''),
	(237484, 118653, -100, 1, 1, 1, 1, '');

	UPDATE gameobject_template SET data1 = entry, data17 = 0, flags = flags & ~0x00000004 WHERE entry IN (
	237469,
	237468,
	237481,
	237461,
	237463,
	237465,
	237480,
	237477,
	237478,
	237467,
	237466,
	237462,
	237464,
	237479,
	237483,
	237472,
	237473,
	237470,
	237471,
	237482,
	237474,
	237475,
	237484);

	-- Everbloom
		DELETE FROM `gameobject` WHERE `id` IN (237472, 237473, 237483) AND `map` = 1279;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237472","1279","7109","7109","6","1","762.92","1442.21","88.4421","5.07356","0","0","0","0","0","255","1","0",0),
		("237473","1279","7109","7109","6","1","326.981","1490.24","102.041","0.623439","0","0","0","0","0","255","1","0",0),
		("237483","1279","7109","7109","2","1","746.571","1567.3","108.295","0.0687468","0","0","0","0","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237472","3","6807","Strangely-Glowing Frond","questinteract","Collecting","","0","4","6","118627","0","0","0","0","0","1691","56764","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237483","3","24596","Rustling Peachick Nest","questinteract","Collecting","","0","4","1","118652","0","0","0","0","0","1691","56775","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237473","3","20499","Overgrown Artifact","questinteract","Collecting","","0","4","1","118628","0","0","0","0","0","1691","56765","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Grimrail Depot
		DELETE FROM `gameobject` WHERE `id` IN (237474, 237475, 237484) AND `map` = 1208;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237474","1208","7109","7383","6","1","1724.637","1610.083","7.6916531","3.157218","0","0","0","0","0","255","1","0",0),
		("237475","1208","7109","7109","6","1","1645.542","1632.62","108.4119","4.327895","-0.5236163","-0.154665","-0.7469788","0.3793777","0","255","1","0",0),
		("237484","1208","7109","7109","2","1","1656.748","1836.932","109.7467","5.318283","0","0","-0.4639521","0.8858603","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237474","3","17016","Huge Crate of Weapons","questinteract","Collecting","","0","4","0.5","118643","0","0","0","0","0","1691","0","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237475","3","23157","Iron Limbcleaver","questinteract","Collecting","","0","4","2","118644","0","0","0","0","0","1691","56767","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237484","3","16604","Iron Autocannon","questinteract","Collecting","","0","4","0.3","118653","0","0","0","0","0","1691","56776","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Shadowmoon Burial Grounds
		DELETE FROM `gameobject` WHERE `id` IN (237470, 237471, 237482) AND `map` = 1176;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237470","1176","7109","7383","6","1","1830.549","121.7431","294.5555","3.02442","0","0","0.9982843","0.05855254","0","255","1","0",0),
		("237471","1176","7109","7109","6","1","1663.392","-231.9792","253.4646","4.6750775","-0.1768336","0.6938286","-0.1654396","0.6782046","0","255","1","0",0),
		("237482","1176","7109","7109","2","1","1796.335","71.72569","264.1181","0","0","0","0","1","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237470","3","16790","Dark Parchment","questinteract","Collecting","","0","4","2","118625","0","0","0","0","0","1691","56762","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237471","3","11524","Silver-Lined Arrow","questinteract","Collecting","","0","4","1.5","118626","0","0","0","0","0","1691","56763","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237482","3","21962","Void-Gate Key","questinteract","Collecting","","0","4","1.5","118651","0","0","0","0","0","1691","56774","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Skyreach
		DELETE FROM `gameobject` WHERE `id` IN (237467, 237466) AND `map` = 1209;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237467","1209","6988","6990","6","1","1236.616","1721.323","178.2672","2.991205","0","0","0.9971743","0.07512315","0","255","1","0",0),
		("237466","1209","6988","6990","6","1","1663.392","-231.9792","253.4646","4.6750775","-0.1768336","0.6938286","-0.1654396","0.6782046","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237467","3","13726","Pristine Plumage","questinteract","Collecting","","0","4","1.5","118622","0","0","0","0","0","1691","56759","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237466","3","18978","Sun Crystal","questinteract","Collecting","","0","4","0.5","118621","0","0","0","0","0","1691","56758","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Auchindoun
		DELETE FROM `gameobject` WHERE `id` IN (237464, 237479) AND `map` = 1182;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
		("237464","1182","6912","6912","6","1","1685.444","2913.046","36.72615","6.200767","0.7028551","-0.03005886","-0.0282135","0.7101374","0","255","1","0",0),
		("237479","1182","6912","6912","2","1","1734.241","2793.382","40.64299","6.212135","0","0","-0.03551769","0.999369","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237464","3","16683","Soulsever Blade","questinteract","Collecting","","0","4","1","118619","0","0","0","0","0","1691","56756","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237479","3","5871","Nightmare Bell","questinteract","Collecting","","0","4","1","118648","0","0","0","0","0","1691","56771","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
		
	-- Iron Docks
		DELETE FROM `gameobject` WHERE `id` = 237462 AND `map` = 1195;
		INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUE
		("237462","1195","6951","7309","6","1","6689.765","-768.1146","7.780828","0","0","0","0","1","0","255","1","0",0);

		REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237462","3","23153","Horribly Acidic Solution","questinteract","Collecting","","0","4","2","118617","0","0","0","0","0","1691","56754","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");

		
	
	UPDATE creature_template SET npcflag = 3, scriptname = 'npc_lysa_serion_garr' WHERE entry = 84947;

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -35 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','1','85510','-0.538097','6.96231','0.635078','1.44424');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','1','77368','-12.6942','-1.69218','1.20133','6.26658');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','1','84947','-8.93744','-7.15514','1.18672','1.76625');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -36 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','1','85510','-11.0836','-1.50432','1.19492','0.019266');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','1','77368','9.01369','6.03496','0.888191','4.51567');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','1','84947','-0.024993','8.65036','0.63884','4.62406');

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -34 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-34','0','79828','2.81785','-0.262591','1.29671','6.24151');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-34','0','87305','11.3304','2.73706','1.26816','2.00821');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-34','0','87300','-1.03095','-0.3539','1.27022','0.174304');

	UPDATE creature_template SET scriptname = "npc_Murg_Garr" WHERE entry = 79828;
	DELETE FROM gossip_menu_option WHERE menu_id = 1 AND id = 22;
	insert into `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) values('1','22','0','I would like to recruit followers.','0','0','0','0','0','0',NULL);
	UPDATE creature_template SET npcflag = 3, scriptname = 'npc_akanja_garr' WHERE entry = 87305;

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -35 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','0','87305','1.60539','-4.17223','1.25964','2.51655');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','0','79828','-6.01615','6.82122','1.26096','2.17883');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-35','0','87300','2.63505','6.23024','1.29533','4.78636');


-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -36 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','0','87305','-3.3287','-2.98104','1.23286','2.1018');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','0','87300','-5.96617','7.7921','1.23279','4.91353');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-36','0','79828','-10.0681','8.31484','1.23389','5.0');

-- Rush Orders

REPLACE INTO spell_script_names VALUES
(181507, "spell_garrison_rush_order"),
(181518, "spell_garrison_rush_order"),
(181519, "spell_garrison_rush_order"),
(181520, "spell_garrison_rush_order"),
(181521, "spell_garrison_rush_order"),
(181522, "spell_garrison_rush_order"),
(181524, "spell_garrison_rush_order"),
(181525, "spell_garrison_rush_order");