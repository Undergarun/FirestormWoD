-- fix Mastery: Master Demonologist
UPDATE `spell_script_names` SET `spell_id`=104027 WHERE `spell_id`=6353 AND `ScriptName`='spell_mastery_master_demonologist';
INSERT INTO `spell_script_names` VALUES (157695, 'spell_mastery_master_demonologist');
