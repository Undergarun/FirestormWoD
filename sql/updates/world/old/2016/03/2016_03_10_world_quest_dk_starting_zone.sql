UPDATE creature_template SET unit_flags = 33280, unit_flags2 = 2048, dynamicflags = 8, type_flags = 4096, equipment_id = 448, scriptname = "npc_koltira_deathweaver_escort", AIName = "SmartAI", TYPE = 6, minlevel = 55, maxlevel = 55, mechanic_immune_mask = 8388624 WHERE entry = 45112;

DELETE FROM creature_text WHERE entry IN (28912, 45112);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
('45112','0','0','I\'ll need to get my runeblade and armor... Just need a little more time.','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT1'),
('45112','1','0','I\'m still weak, but I think I can get an anti-magic barrier up. Stay inside it or you\'ll be destroyed by their spells.','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT2'),
('45112','2','0','Maintaining this barrier will require all of my concentration. Kill them all!','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT3'),
('45112','3','0','There are more coming. Defend yourself! Don\'t fall out of the anti-magic field! They\'ll tear you apart without its protection!','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT4'),
('45112','4','0','I can\'t keep barrier up much longer... Where is that coward?','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT5'),
('45112','5','0','The High Inquisitor comes! Be ready, death knight! Do not let him draw you out of the protective bounds of my anti-magic field! Kill him and take his head!','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT6'),
('45112','6','0','Stay in the anti-magic field! Make them come to you!','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT7'),
('45112','7','0','The death of the High Inquisitor of New Avalon will not go unnoticed. You need to get out of here at once! Go, before more of them show up. I\'ll be fine on my own.','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT8'),
('45112','8','0','I\'ll draw their fire, you make your escape behind me.','12','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT9'),
('45112','9','0','Your High Inquisitor is nothing more than a pile of meat, Crusaders! There are none beyond the grasp of the Scourge!','14','0','100','0','0','0','koltira deathweaver SAY_BREAKOUT10');

DELETE FROM script_waypoint WHERE entry IN (28912, 45112);
INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES
('45112','0','1653.52','-6038.37','127.585','1000','Jump off'),
('45112','1','1653.98','-6034.61','127.585','5000','To Box'),
('45112','2','1653.85','-6034.73','127.585','0','Equip'),
('45112','3','1652.3','-6035.67','127.585','1000','Recover'),
('45112','4','1639.76','-6046.34','127.948','0','Escape'),
('45112','5','1640.96','-6028.12','134.74','0',''),
('45112','6','1625.81','-6029.2','134.74','0',''),
('45112','7','1626.84','-6015.08','134.74','0',''),
('45112','8','1649.15','-6016.98','133.24','0',''),
('45112','9','1653.06','-5974.84','132.652','5000','Mount'),
('45112','10','1654.75','-5926.42','121.191','0','Disappear');
