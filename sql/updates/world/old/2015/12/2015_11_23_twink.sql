-- fix Cripple
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_cripple_doomguard';
INSERT INTO `spell_script_names` VALUES ('170995', 'spell_warl_cripple_doomguard');

-- fix Glyph of the Weaponmaster
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_weaponmaster';
INSERT INTO `spell_script_names` VALUES (147367, 'spell_warr_weaponmaster');
