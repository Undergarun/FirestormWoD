UPDATE `creature_template` SET gossip_menu_id = 16986, `npcflag`=`npcflag`|17, trainer_type=2 WHERE `entry`=79892;
DELETE FROM `npc_trainer` WHERE `entry`=79892;
insert into `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) values
('79892','7733','100','356','0','1'),
('79892','7734','500','356','50','1'),
('79892','55083','10000','356','125','1'),
('79892','18249','25000','356','200','1'),
('79892','54084','100000','356','275','1'),
('79892','51293','150000','356','350','1'),
('79892','88869','250000','356','425','1'),
('79892','110412','300000','356','500','1');


UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=79971;
DELETE FROM `npc_vendor` WHERE `entry` = 79971 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(79971, 0, 6256, 0, 0, 0, 1), 
(79971, 0, 6365, 1, 3600, 0, 1), 
(79971, 0, 6529, 0, 0, 0, 1), 
(79971, 0, 6530, 0, 0, 0, 1), 
(79971, 0, 6532, 0, 0, 0, 1), 
(79971, 0, 6533, 2, 3600, 0, 1);

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-64 AND faction_index = 0;
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','79892','4.10929','-1.38352','0.583206','0.213478');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','44880','2.56324','5.39814','4.2152','0.408808');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','44880','1.28986','-0.327592','8.7592','6.09967');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','79971','-0.343123','7.68761','0.713196','0.655077');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','62953','25.7635','-22.1997','6.16921','1.42364');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','44880','2.56324','5.39814','5.7152','0.408808');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','-232440','8.77231','3.06427','0.000198','4.7124');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','-230764','1.15135','7.00761','0.089203','0.633848');

-- Quest Chain : 34758, 36141, 36131, 36132
UPDATE quest_template SET PrevQuestId = 0, NextQuestId = 36141 WHERE Id = 34758;
UPDATE quest_template SET PrevQuestId = 34758, NextQuestId = 36131 WHERE Id = 36141;
UPDATE quest_template SET PrevQuestId = 36141, NextQuestId = 36132 WHERE Id = 36131;
UPDATE quest_template SET PrevQuestId = 36131, NextQuestId = 0 WHERE Id = 36132;
UPDATE quest_template SET RequiredSkillId = 0 WHERE id IN (34758, 36141, 36131, 36132);

REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES
('79892','34758'),
('79896','36131'),
('79896','36132'),
('79896','36141');

REPLACE INTO `creature_questender` (`id`, `quest`) VALUES
('79892','36132'),
('79896','34758'),
('79896','36131'),
('79896','36141');

-- Horde daily quests (35071, 35006, 35072, 35073, 35074, 35075)

UPDATE quest_template SET PrevQuestId = 36132 WHERE Id IN (35071, 35006, 35072, 35073, 35074, 35075);

DELETE FROM pool_quest WHERE pool_entry = 30005;
DELETE FROM pool_template WHERE entry = 30005;
INSERT INTO pool_template VALUE (30005, 1, "Horde Garrison Fishing daily quests");
INSERT INTO pool_quest VALUES
(35071, 30005, "Jawless Skulker"),
(35006, 30005, "Fire Ammonite"),
(35072, 30005, "Fat Sleeper"),
(35073, 30005, "Blind Lake Sturgeon"),
(35074, 30005, "Blackwater Whiptail"),
(35075, 30005, "Abyssal Gulper Eel");

-- Updates

UPDATE creature_template SET scriptname = "npc_MakJin_Garr" WHERE entry = 79892;

-- zone frostwall - 7004