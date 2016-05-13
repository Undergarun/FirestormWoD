DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_monk_uplift';
INSERT INTO `spell_script_names` VALUES (116670, 'spell_monk_uplift');
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_warr_shield_barrier';
INSERT INTO `spell_script_names` VALUES (112048, 'spell_warr_shield_barrier');
INSERT INTO `spell_script_names` VALUES (174926, 'spell_warr_shield_barrier');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_eclipse';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_eclipse_mod_damage';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
('8921', 'spell_dru_eclipse_mod_damage'),
('48505', 'spell_dru_eclipse_mod_damage'),
('2912', 'spell_dru_eclipse_mod_damage'),
('16914', 'spell_dru_eclipse_mod_damage'),
('78674', 'spell_dru_eclipse_mod_damage'),
('93402', 'spell_dru_eclipse_mod_damage'),
('88747', 'spell_dru_eclipse_mod_damage'),
('5176', 'spell_dru_eclipse_mod_damage');
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_moonfire_damage';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_moonfire_sunfire_damage';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
('164812', 'spell_dru_moonfire_sunfire_damage'),
('164815', 'spell_dru_moonfire_sunfire_damage');
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = '-171743' AND `spell_effect` = '-93431';
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = '-171744' AND `spell_effect` = '-93430';
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-171743, -93431, 0, 'Remove eclipse visual with eclipse buff'),
(-171744, -93430, 0, 'Remove eclipse visual with eclipse buff');
