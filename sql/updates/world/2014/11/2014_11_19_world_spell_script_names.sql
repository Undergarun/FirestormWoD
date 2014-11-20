DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_pri_saving_grace';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('152116', 'spell_pri_saving_grace');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_leader_of_the_pack';
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_leader_of_the_pack_critical';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
('17007', 'spell_dru_leader_of_the_pack'),
('24932', 'spell_dru_leader_of_the_pack_critical');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_dru_glyph_of_the_stag';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('114338', 'spell_dru_glyph_of_the_stag');

DELETE FROM `spell_script_names` WHERE `spell_id` = 102547 AND `ScriptName` = 'spell_dru_activate_cat_form';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES ('5215', 'spell_dru_activate_cat_form');
