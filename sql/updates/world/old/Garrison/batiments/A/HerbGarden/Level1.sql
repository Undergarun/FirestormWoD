INSERT INTO npc_text (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `BuildVerified`) VALUES(86898, "Beautiful day commander.", "Beautiful day commander.", 0, 100, 19342) ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `text0_0` = VALUES(`text0_0`), `text0_1` = VALUES(`text0_1`), `lang0` = VALUES(`lang0`), `prob0` = VALUES(`prob0`), `BuildVerified` = VALUES(`BuildVerified`);
INSERT INTO gossip_menu (`entry`, `text_id`) VALUES(16962, 86898) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `text_id` = VALUES(`text_id`);
UPDATE `creature_template` SET gossip_menu_id = 16962, npcflag=3 WHERE `entry`=85344;

-- Work order NPC
INSERT INTO npc_text (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `BuildVerified`) VALUES(86941, "Draenic seeds can be gathered from herbalism nodes across draenor.  Return to me once you have 5 or more and I will grow them into herbs for you.", "Draenic seeds can be gathered from herbalism nodes across draenor.  Return to me once you have 5 or more and I will grow them into herbs for you.", 0, 100, 19342) ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `text0_0` = VALUES(`text0_0`), `text0_1` = VALUES(`text0_1`), `lang0` = VALUES(`lang0`), `prob0` = VALUES(`prob0`), `BuildVerified` = VALUES(`BuildVerified`);
UPDATE `creature_template` SET gossip_menu_id = 16966, npcFlag=3, npcFlag2=128, minlevel=90, maxlevel=90, ScriptName="npc_OllyNimkip_Garr" WHERE `entry`=85514;

UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=77355;
DELETE FROM `npc_vendor` WHERE `entry` = 77355 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(77355, 0, 2901, 0, 0, 0, 1), 
(77355, 0, 3371, 0, 0, 0, 1), 
(77355, 0, 4470, 0, 0, 0, 1), 
(77355, 0, 5956, 0, 0, 0, 1), 
(77355, 0, 6217, 0, 0, 0, 1), 
(77355, 0, 6256, 0, 0, 0, 1), 
(77355, 0, 7005, 0, 0, 0, 1), 
(77355, 0, 20815, 0, 0, 0, 1), 
(77355, 0, 38682, 0, 0, 0, 1), 
(77355, 0, 39354, 0, 0, 0, 1), 
(77355, 0, 39505, 0, 0, 0, 1), 
(77355, 0, 79249, 0, 0, 0, 1), 
(77355, 0, 85663, 0, 0, 0, 1), 
(77355, 0, 112322, 0, 0, 0, 1), 
(77355, 0, 115351, 0, 0, 0, 1), 
(77355, 0, 117445, 0, 0, 0, 1), 
(77355, 0, 117452, 0, 0, 0, 1);

DELETE FROM creature_queststarter WHERE id=85344 AND quest=36404;
INSERT INTO creature_queststarter(`id`, `quest`) values('85344','36404');

DELETE FROM creature_questender WHERE id=85344 AND quest=36404;
INSERT INTO creature_questender(`id`, `quest`) values('85344','36404');

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-29 AND faction_index = 1;
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','85344','3.57653','-7.82384','0.767654','4.75504');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','77903','-0.933269','-2.83758','1.47805','6.28319');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','77369','-0.928431','-1.12068','1.47805','6.28319');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','77355','-11.3134','-9.16087','0.767654','6.01708');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','85514','-10.9741','-10.5749','0.767654','5.87562');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','-233835','17.5351','-7.30508','-1.79285','4.89865');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','-233835','19.1577','-11.6836','-1.73195','1.08949');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','1','-235885','-10.3621','-8.1722','0.184349','5.48667');

UPDATE gameobject_template SET flags=0, ScriptName="go_garrison_shipment_container" WHERE TYPE=45;