-- fix Cat Form , Claws of Shirvallah with Savage Roar
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_cat_form';
INSERT INTO `spell_script_names` VALUES (768, 'spell_dru_cat_form');
INSERT INTO `spell_script_names` VALUES (171745, 'spell_dru_cat_form');

-- Mastery: Primal Tenacity
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_primal_tenacity';
INSERT INTO `spell_script_names` VALUES (159195, 'spell_mastery_primal_tenacity');'
