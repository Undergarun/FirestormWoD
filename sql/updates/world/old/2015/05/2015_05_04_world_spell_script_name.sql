-- fix Item – Rogue WoD PvP Subtlety 4P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_feint';
INSERT INTO `spell_script_names` VALUES (1966, 'spell_rog_feint');

-- fix Comet Storm
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_comet_storm';
INSERT INTO `spell_script_names` VALUES (153595, 'spell_mage_comet_storm');
INSERT INTO `spell_script_names` VALUES (153596, 'spell_mage_comet_storm');