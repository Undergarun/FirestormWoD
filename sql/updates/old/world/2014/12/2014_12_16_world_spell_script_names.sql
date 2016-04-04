DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_mage_kindling';
INSERT INTO `spell_script_names` VALUES (133, 'spell_mage_kindling');
INSERT INTO `spell_script_names` VALUES (44614, 'spell_mage_kindling');
INSERT INTO `spell_script_names` VALUES (11366, 'spell_mage_kindling');
INSERT INTO `spell_script_names` VALUES (108853, 'spell_mage_kindling');
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_monk_hurricane_strike';
INSERT INTO `spell_script_names` VALUES (158221, 'spell_monk_hurricane_strike');
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = '152175' AND `spell_effect` = '158221' AND `type` = '0';
INSERT INTO `spell_linked_spell` VALUES ('152175', '158221', '0', 'Hurricane Strike cast Hurricane Strike (Damage)');
