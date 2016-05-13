# Highmaul Conscript loot
UPDATE creature_loot_template SET ChanceOrQuestChance = 1 WHERE item IN (119335, 119344);

# Vulgor
UPDATE creature_template SET exp = 5, Health_mod = 119 WHERE entry = 80048;

DELETE FROM gameobject_template WHERE entry = 229649;
INSERT INTO gameobject_template VALUE
(229649, 0, 17686, 'Earthen Pillar', '', '', '', 0, 32, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 19865);

# Volatile Anomaly
UPDATE creature_template SET minlevel = 102, maxlevel = 102, Health_mod = 2.44 WHERE entry = 79956;

DELETE FROM `creature_template` WHERE `entry` IN (1579956, 1679956);
INSERT INTO `creature_template` VALUES
('1579956','0','0','54282','0','0','0','Volatile Anomaly (15)','','','','0','102','102','5','0','14','0','0','1','1.14286','1.14286','1','1','0','2','2000','2000','1','1','2','33555200','2048','0','0','0','0','0','0','0','0','4','2097224','128','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','3.05','1','1','1','0','0','0','0','0','0','0','157','0','0','1','0','0','0','','19116'),
('1679956','0','0','54282','0','0','0','Volatile Anomaly (16)','','','','0','102','102','5','0','14','0','0','1','1.14286','1.14286','1','1','0','3','2000','2000','1','1','2','33555200','2048','0','0','0','0','0','0','0','0','4','2097224','128','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','5.33','1','1','1','0','0','0','0','0','0','0','157','0','0','1','0','0','0','','19116');

DELETE FROM `creature_template_difficulty` WHERE `entry` = 79956;
INSERT INTO `creature_template_difficulty` VALUES
(79956, 16, 1579956),
(79956, 17, 1679956);

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_suppression_field_aura', 'spell_highmaul_frozen_core');
INSERT INTO spell_script_names VALUES
(161328, 'spell_highmaul_suppression_field_aura'),
(174404, 'spell_highmaul_frozen_core');