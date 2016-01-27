INSERT INTO npc_text (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `WDBVerified`) VALUES(87206, "Think you can help us catch some fish around here?", "Think you can help us catch some fish around here?", 0, 100, 19342) ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `text0_0` = VALUES(`text0_0`), `text0_1` = VALUES(`text0_1`), `lang0` = VALUES(`lang0`), `prob0` = VALUES(`prob0`), `WDBVerified` = VALUES(`WDBVerified`);
INSERT INTO gossip_menu (`entry`, `text_id`) VALUES(16986, 87206) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `text_id` = VALUES(`text_id`);
INSERT INTO gossip_menu_option (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `box_coded`, `box_money`, `box_text`, `npc_option_npcflag`) VALUES(16986, 1, 3, "I'm interested in fishing these savage lands.", 5, 0, 0, "", 16) ON DUPLICATE KEY UPDATE `menu_id` = VALUES(`menu_id`), `id` = VALUES(`id`), `option_icon` = VALUES(`option_icon`), `option_text` = VALUES(`option_text`), `option_id` = VALUES(`option_id`), `box_coded` = VALUES(`box_coded`), `box_money` = VALUES(`box_money`), `box_text` = VALUES(`box_text`), `npc_option_npcflag` = VALUES(`npc_option_npcflag`);
UPDATE `creature_template` SET gossip_menu_id = 16986, `npcflag`=`npcflag`|17, trainer_type=2, ScriptName="npc_RonAshton_Garr" WHERE `entry`=77733;
DELETE FROM `npc_trainer` WHERE `entry`=77733;
INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
(77733, 7620, 100, 356, 0, 1), 
(77733, 7731, 500, 356, 50, 1), 
(77733, 7732, 10000, 356, 125, 1), 
(77733, 18248, 25000, 356, 200, 1), 
(77733, 33095, 100000, 356, 275, 1), 
(77733, 51294, 150000, 356, 350, 1), 
(77733, 88868, 250000, 356, 425, 1), 
(77733, 110410, 300000, 356, 500, 1);

UPDATE `creature_template` SET `npcflag`=`npcflag`|128, ScriptName="npc_Segumi_Garr" WHERE `entry`=85708;
INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(85708, 1, 84660, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
DELETE FROM `npc_vendor` WHERE `entry` = 85708 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(85708, 0, 6256, 0, 0, 0, 1), 
(85708, 0, 6365, 1, 3600, 0, 1), 
(85708, 0, 6529, 0, 0, 0, 1), 
(85708, 0, 6530, 0, 0, 0, 1), 
(85708, 0, 6532, 0, 0, 0, 1), 
(85708, 0, 6533, 2, 3600, 0, 1);

UPDATE creature_template_addon SET auras="" WHERE entry = 82436;
UPDATE `creature_template` SET ScriptName="npc_TharisStrongcast_Garr" WHERE `entry`=82436;
INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(82436, 1, 1117, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-64 AND faction_index = 1;
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','1','85708','2.28928','-2.15955','0.596008','0.2334');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','1','82436','2.97392','-6.4922','0.369904','1.32348');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','1','77733','14.3749','1.34486','0.503105','6.15809');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-64','1','-232268','6.38951','-0.564783','0.000008','4.24988');

-- Quest Chain : 34194, 36199, 36201, 36202
UPDATE quest_template SET PrevQuestId = 0, NextQuestId = 36199 WHERE Id = 34194;
UPDATE quest_template SET PrevQuestId = 34194, NextQuestId = 36201 WHERE Id = 36199;
UPDATE quest_template SET PrevQuestId = 36199, NextQuestId = 36202 WHERE Id = 36201;
UPDATE quest_template SET PrevQuestId = 36201, NextQuestId = 0 WHERE Id = 36202;
UPDATE quest_template SET RequiredSkillId = 0 WHERE id IN (34194, 36199, 36201, 36202);

REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES
('77733','34194'),
('84372','36199'),
('84372','36201'),
('84372','36202');

REPLACE INTO `creature_questender` (`id`, `quest`) VALUES
('77733','36202'),
('84372','34194'),
('84372','36199'),
('84372','36201');