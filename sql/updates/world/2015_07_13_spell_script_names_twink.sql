-- fix Gargoyle Strike
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_gargoyle_strike';
INSERT INTO `spell_script_names` VALUES (51963, 'spell_dk_gargoyle_strike');

-- fix Single-Minded Fury
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_single_minded_fury';
INSERT INTO `spell_script_names` VALUES (81099, 'spell_warr_single_minded_fury');
