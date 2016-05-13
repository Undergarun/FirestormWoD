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

-- fix Elixir of Wandering Spirits
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_elixir_of_wandering_spirits';
INSERT INTO `spell_script_names` VALUES (147412, 'spell_gen_elixir_of_wandering_spirits');

-- fix Infernal (Demonic Servitude)
DELETE FROM `pet_stats` WHERE `entry`=78217;
INSERT INTO `pet_stats` VALUES (78217, 1.14, 1, 3, 1, 0.4, 1, 2, 3, -100, 0.065934, 'Infernal (Demonic Servitude)');

-- fix Dark Infusion with Breath of Sindragosa
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_breath_of_sindragosa';
INSERT INTO `spell_script_names` VALUES (152279 , 'spell_dk_breath_of_sindragosa');

-- fix Taunt
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=355 AND `spell_effect`=71;
REPLACE INTO `spell_linked_spell` VALUES (355, 71, 0, 'Taunt trigger Defensive Stance');

-- fix Shield Charge increase damage of Heroic Strike and Revenge
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_shield_charge_damage';
INSERT INTO `spell_script_names` VALUES (78, 'spell_warr_shield_charge_damage');
INSERT INTO `spell_script_names` VALUES (6572, 'spell_warr_shield_charge_damage');
