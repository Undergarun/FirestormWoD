-- fix Sanctified Wrath
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_sanctified_wrath';
INSERT INTO `spell_script_names` VALUES (31884, 'spell_pal_sanctified_wrath');
