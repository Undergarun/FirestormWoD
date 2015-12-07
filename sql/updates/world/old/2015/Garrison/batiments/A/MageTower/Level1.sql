-- ALLIANCE lvl 1

INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(240602, 45, 15585, "Mage Tower Work Order", "", "", "", 1, 0, 0, 0, 0, 0, 0, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);

-- plot content

DELETE FROM garrison_plot_content WHERE plot_type_or_building = -37 AND faction_index = 1; -- alliance
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','82571','14.8682','-9.20942','1.96319','5.57371');  -- Atheeru Palestar
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','91582','24.48','6.2002','0.500618','6.05671');     -- Apprentice Var'Nath
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','77367','8.67655','-0.406824','1.80521','6.27663'); -- Archmage Kem
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','84960','4.713','7.21672','1.80444','1.90196');     -- Lunarfall Arcanist
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','83810','2.25137','-4.56007','1.80375','3.81048');  -- Adventurer
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','87760','21.7892','-11.3573','0.502243','5.40877'); -- Training Dummy
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','87761','17.0097','-15.3535','0.500702','5.40877'); -- Dungeoneers Training Dummy
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','1','-240601','23.4583','10.5873','0.000046','5.986');  -- Mage Tower Work Orders

#77367/NPC - Archmage Kem -- QuestGiver

	UPDATE creature_template SET npcflag = npcflag | 2 WHERE entry = 77367;
	DELETE FROM creature_queststarter WHERE id = 77367;
	DELETE FROM creature_questender WHERE id = 77367;
	INSERT INTO creature_queststarter VALUES (77367, 38354), (77367, 36848);
	INSERT INTO creature_questender VALUES (77367, 38354), (77367, 36848);

#91582/NPC - Apprentice Var Nath -- Work Orders

	UPDATE creature_template SET npcflag = npcflag | 1, npcflag2 = 32, subname = "Work Orders", scriptname = "npc_ApprenticeVarNath_Garr" WHERE entry = 91582;

#82571/NPC - Atheeru Palestar -- Summons special pnjs (Amperial Construct)

	