-- fix Sanctified Wrath
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_sanctified_wrath';
INSERT INTO `spell_script_names` VALUES (31884, 'spell_pal_sanctified_wrath');

-- fix attack speed of all pets
UPDATE `pet_stats` SET `attackspeed`='2' WHERE `entry`='1';

-- fix Power Word: Shield absorb amount calculation
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_shield_discipline' AND `spell_id`=17;

-- fix Empowered Moonkin
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_empowered_moonkin';
INSERT INTO `spell_script_names` VALUES (5176, 'spell_dru_empowered_moonkin');
INSERT INTO `spell_script_names` VALUES (78674, 'spell_dru_empowered_moonkin');
INSERT INTO `spell_script_names` VALUES (152221, 'spell_dru_empowered_moonkin');
INSERT INTO `spell_script_names` VALUES (2912, 'spell_dru_empowered_moonkin');
