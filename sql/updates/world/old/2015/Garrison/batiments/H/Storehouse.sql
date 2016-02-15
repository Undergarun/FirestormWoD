-- LEVEL 1

	UPDATE creature_template SET npcFlag=npcFlag | 131072 WHERE entry=79862;
	REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `AIName`, `ScriptName`, `BuildVerified`) VALUE('237257','3','17115','Crate of Surplus Material','questinteract','Gathering','','0','0','1.5','118417','0','0','0','0','0','43','-1','0','0','1','0','-1','0','37060','0','0','0','0','0','24982','0','0','29244','0','0','0','0','0','0','0','0','0','0','0','0','56620','1','0','0','','','1');

	REPLACE INTO creature_queststarter VALUE (79862, 37060);
	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-51 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-51','0','79862','1.66546','-0.009018','1.31065','-0.01372');

-- LEVEL 2

-- LEVEL 3