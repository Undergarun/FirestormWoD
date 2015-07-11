-- fix Divine Purpose
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_divine_purpose' AND `spell_id`=157048;
INSERT INTO `spell_script_names` VALUES (157048, 'spell_pal_divine_purpose');
