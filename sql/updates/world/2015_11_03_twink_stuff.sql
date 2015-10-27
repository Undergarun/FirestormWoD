-- fix Pyroblast Clearcasting Driver
DELETE FROM `spell_proc_event` WHERE `entry`='44448';
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES ('44448', 0x00000001 + 0x00000002 + 0x00000400);

-- fix Command Demon
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_command_demon_spells';
INSERT INTO `spell_script_names` VALUES (119905, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119907, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119909, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119910, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119911, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119913, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119914, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119915, 'spell_warl_command_demon_spells');
