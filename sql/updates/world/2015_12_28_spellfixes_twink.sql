-- fix Searing Totem size
UPDATE `creature_template` SET `scale`='3' WHERE `entry`='2523';

-- fix Grand Crusader
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_grand_crusader';
INSERT INTO `spell_script_names` VALUES (35395, 'spell_pal_grand_crusader');
INSERT INTO `spell_script_names` VALUES (53595, 'spell_pal_grand_crusader');
