-- fix Cat Form , Claws of Shirvallah with Savage Roar
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_cat_form';
INSERT INTO `spell_script_names` VALUES (768, 'spell_dru_cat_form');
INSERT INTO `spell_script_names` VALUES (171745, 'spell_dru_cat_form');

-- Mastery: Primal Tenacity
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_primal_tenacity';
INSERT INTO `spell_script_names` VALUES (159195, 'spell_mastery_primal_tenacity');

-- fix usebug with Carrying Seaforium
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_carrying_seaforium';
INSERT INTO `spell_script_names` VALUES (52410, 'spell_gen_carrying_seaforium');

-- fix Enrage proc on absorb
DELETE FROM `spell_proc_event` WHERE `entry`=13046;
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES (13046, 0x0000002 + 0x0000400);

-- fix Mastery: Master Demonologist
UPDATE `spell_script_names` SET `spell_id`=104027 WHERE `spell_id`=6353 AND `ScriptName`='spell_mastery_master_demonologist';
