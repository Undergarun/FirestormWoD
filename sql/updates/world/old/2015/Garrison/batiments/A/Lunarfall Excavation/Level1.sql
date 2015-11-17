INSERT INTO npc_text (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`) VALUES(87264, "Draenic stone can be mined from mining deposits across draenor.  Return to me once you have 5 or more and I will refine them into ore for you.", "Draenic stone can be mined from mining deposits across draenor.  Return to me once you have 5 or more and I will refine them into ore for you.", 0, 100) ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `text0_0` = VALUES(`text0_0`), `text0_1` = VALUES(`text0_1`), `lang0` = VALUES(`lang0`), `prob0` = VALUES(`prob0`);
DELETE FROM creature WHERE id = 77361;
UPDATE creature_template_addon SET auras="" WHERE entry = 77361;

UPDATE `creature_template` SET `npcflag`=`npcflag`|4224 WHERE `entry`=81348;
DELETE FROM `npc_vendor` WHERE `entry` = 81348 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(81348, 0, 2880, 0, 0, 0, 1), 
(81348, 0, 2901, 0, 0, 0, 1), 
(81348, 0, 3466, 0, 0, 0, 1), 
(81348, 0, 3857, 0, 0, 0, 1), 
(81348, 0, 5956, 0, 0, 0, 1), 
(81348, 0, 18567, 0, 0, 0, 1);

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-61 AND faction_index = 1;

-- Alliance Miners
/*
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','11.1349','13.5558','-34.9398','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','-0.993771','8.68111','-33.3508','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','6.28446','-37.8387','-30.8215','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','-6.28463','-23.7491','-30.6559','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','7.70396','-13.5887','-29.9909','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','18.7029','-29.3607','-30.9258','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','30.6946','-48.5749','-28.1097','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','56.0952','-50.0695','-27.5936','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','55.2605','-83.6182','-22.6694','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','41.0277','-83.3388','-21.6817','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','10.627','-104.148','-0.694412','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','-16.5159','-107.353','11.426','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','-8.15332','-92.1833','14.0505','5.93412');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-61','1','77361','-8.87052','-74.1551','20.7718','5.93412');*/

-- Timothy Leens
UPDATE creature_template SET faction = 35, scriptname = "npc_TimothyLeens_Garr" WHERE entry = 77730;
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-61','1','77730','36.5396','-62.3678','30.6719','5.1964');

-- Rachelle Black <Blacksmith>
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-61','1','81348','43.5391','-59.5392','30.6719','5.57049');

-- Mine Shipment
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-61','1','-235886','38.5198','-61.2837','30.0886','4.71848');

-- Thermal Anvil
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-61','1','-232529','42.2523','-62.4901','30.2248','0.061085');

-- Forge
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-61','1','-232530','42.2627','-62.4907','30.0491','3.18522');

-- Miners
DELETE FROM creature_template_addon WHERE entry = 77361;
insert into `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`, `animkit`) values('77361','0','0','0','2','233','','0');

DELETE FROM gameobject WHERE id IN (232542, 232543, 232544, 232545) AND map in (1331, 1330); -- Garrison lvl 2 (A/H)
UPDATE gameobject_template SET data17 = 43000, scriptname = "go_garrison_deposit" WHERE entry IN (232542, 232543, 232544, 232545);