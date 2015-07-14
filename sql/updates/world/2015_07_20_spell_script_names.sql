-- fix Divine Purpose
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_divine_purpose' AND `spell_id`=157048;
INSERT INTO `spell_script_names` VALUES (157048, 'spell_pal_divine_purpose');

-- fix Metamorphosis : Demonic Circle : Teleport
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_demonic_circle_teleport';
INSERT INTO `spell_script_names` VALUES (48020, 'spell_warl_demonic_circle_teleport');
INSERT INTO `spell_script_names` VALUES (114794, 'spell_warl_demonic_circle_teleport');
INSERT INTO `spell_script_names` VALUES (104136, 'spell_warl_demonic_circle_teleport');
