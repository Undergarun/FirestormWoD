-- LEVEL 1

REPLACE INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(79864, "Warra the Weaver", "Tailor", "trainer", 0, 0, 7, 56711, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 35, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 19702);
REPLACE INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(79863, "Turga", "Work Orders", "workorders", 0, 0, 7, 56710, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 35, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 19702);
REPLACE INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(56711, 0.389, 1.5, 0);
REPLACE INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(56710, 0.389, 1.5, 0);

UPDATE creature_template SET ScriptName="npc_WarraTheWeaver_Garr", npcFlag=3, minlevel=90, maxlevel=90 WHERE entry=79864;
UPDATE creature_template SET ScriptName="npc_Turga_Garr", npcFlag=3, npcFlag2=32, minlevel=90, maxlevel=90 WHERE entry=79863;

DELETE FROM creature_queststarter    WHERE id=79864 AND quest=37575;
DELETE FROM creature_questender WHERE id=79863 AND quest=37575;
INSERT INTO `creature_queststarter` (`id`, `quest`) values('79864','37575');
INSERT INTO `creature_questender` (`id`, `quest`) values('79863','37575');

REPLACE INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(237181, 45, 20508, "Tailoring Work Order", "", "", "", 1, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19702);

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-94 AND faction_index = 0;
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-94','0','79864','2.44121','-1.80831','1.18031','0.552143');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-94','0','79863','0.99776','0.836435','1.18031','6.22665');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-94','0','-237181','8.26871','-7.67436','-0.058136','0.849014');

UPDATE gameobject_template SET flags=0, ScriptName="go_garrison_shipment_container" WHERE TYPE=45;

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-127 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-127','0','79864','2.16999','3.46194','1.22659','4.29232');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-127','0','79863','0.616883','-3.59242','1.22659','0.934741');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-127','0','-237181','10.9672','-2.36038','0.000267','5.91024');


-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-128 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-128','0','79863','2.457','4.96839','0.705803','3.60117');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-128','0','79864','1.10638','-4.90766','0.705803','1.79083');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-128','0','-237181','10.996','-2.17787','0.001602','6.14586');
