DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_improved_presences';

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_presences';
INSERT INTO `spell_script_names` VALUES
(48263, 'spell_dk_presences'),
(48265, 'spell_dk_presences'),
(48266, 'spell_dk_presences');
