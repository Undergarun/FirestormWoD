DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_leader_of_the_pack';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_leader_of_the_pack_critical';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_dream_of_cenarius';
INSERT INTO `spell_script_names` VALUES (158501, 'spell_dru_dream_of_cenarius');
DELETE FROM `spell_proc_event` WHERE `entry` IN (16961, 159286);
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_primal_fury';
INSERT INTO `spell_script_names` VALUES
(16961, 'spell_dru_primal_fury'),
(159286, 'spell_dru_primal_fury');
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_healing_touch';
INSERT INTO `spell_script_names` VALUES (5185, 'spell_dru_healing_touch');
