-- fix Marked for Death
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_marked_for_death';
INSERT INTO `spell_script_names` VALUES ('140149', 'spell_rog_marked_for_death'); -- visual

DELETE FROM `spell_linked_spell` WHERE spell_trigger = 137619;
INSERT INTO `spell_linked_spell` VALUES (137619, 140149, 2, 'Marked for Death (check caster)');

-- fix Nightfall proc from Corruption
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_nightfall';
INSERT INTO `spell_script_names` VALUES (146739, 'spell_warl_nightfall');

-- fix Ember Tap stacking
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_ember_tap_glyph';
INSERT INTO `spell_script_names` VALUES (114635, 'spell_warl_ember_tap_glyph');

-- fix Chaos Bolt, removes stacks of Backdraft
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_chaos_bolt';
INSERT INTO `spell_script_names` VALUES (116858, 'spell_warl_chaos_bolt');

-- fix Divine Insight (shadow) if proc while player is casting
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_mind_blast';
INSERT INTO `spell_script_names` VALUES ('8092', 'spell_pri_mind_blast');

-- fix Remove Curse, should remove Hex
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_remove_curse';
INSERT INTO `spell_script_names` VALUES (475, 'spell_mage_remove_curse');

-- fix Doom Bolt, damage calculation
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_doom_bolt';
INSERT INTO `spell_script_names` VALUES (85692, 'spell_gen_doom_bolt');

-- fix Chaos Wave, trigger Molten Core
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_chaos_wave';
INSERT INTO `spell_script_names` VALUES (124916, 'spell_warl_chaos_wave');

-- fix Poisoned Ammo damage coef
UPDATE `spell_bonus_data` SET `ap_dot_bonus`='0.046' WHERE `entry`='162543';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_poisoned_ammo';

-- fix Kill Command damage
DELETE FROM `spell_bonus_data` WHERE `entry`='83381';

-- fix Explosive Shot damage
DELETE FROM `spell_bonus_data` WHERE `entry`='53301';
INSERT INTO `spell_bonus_data` (`entry`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES ('53301', '0.553', '0.553', 'Hunter - Explosive Shot');

-- fix Item - Hunter WoD PvP Marksmanship 4P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_aimed_shot';
INSERT INTO `spell_script_names` VALUES (19434, 'spell_hun_aimed_shot');

-- fix Poisoned Ammo
DELETE FROM `spell_bonus_data` WHERE `entry`='162543';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_poisoned_ammo';
INSERT INTO `spell_script_names` VALUES (162543, 'spell_hun_poisoned_ammo');

-- fix Item - Priest WoD PvP Shadow 2P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_dispersion';
INSERT INTO `spell_script_names` VALUES (47585, 'spell_pri_dispersion');

-- fix Item - Mage WoD PvP Frost 2P Bonus
DELETE FROM `areatrigger_template` WHERE `spell_id`=180723;
INSERT INTO `areatrigger_template` (`spell_id`, `entry`, `scale_x`, `scale_y`, `flags`, `ScriptName`) VALUES (180723, 180723, 1, 1, 16384, 'at_mage_wod_frost_2p_bonus');

-- fix Explosive Shot
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_explosive_shot';

-- fix Item � Rogue WoD PvP Subtlety 4P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_feint';
INSERT INTO `spell_script_names` VALUES (1966, 'spell_rog_feint');

-- fix Comet Storm
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_comet_storm';
INSERT INTO `spell_script_names` VALUES (153595, 'spell_mage_comet_storm');
INSERT INTO `spell_script_names` VALUES (153596, 'spell_mage_comet_storm');

-- fix Fingers of Frost
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_fingers_of_frost';
INSERT INTO `spell_script_names` VALUES (44544, 'spell_mage_fingers_of_frost');

-- fix Dream of Cenarius (feral)
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_dream_of_cenarius_feral';
INSERT INTO `spell_script_names` VALUES (5185, 'spell_dru_dream_of_cenarius_feral');
INSERT INTO `spell_script_names` VALUES (774, 'spell_dru_dream_of_cenarius_feral');

-- fix Item - Druid WoD PvP Restoration 2P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_wod_pvp_2p_restoration';
INSERT INTO `spell_script_names` VALUES (22812, 'spell_dru_wod_pvp_2p_restoration');
INSERT INTO `spell_script_names` VALUES (102342, 'spell_dru_wod_pvp_2p_restoration');

-- fix Backstab
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_backstab';
INSERT INTO `spell_script_names` VALUES (53, 'spell_rog_backstab');

-- fix Subterfuge
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_stealth';
INSERT INTO `spell_script_names` VALUES (1784, 'spell_rog_stealth');
INSERT INTO `spell_script_names` VALUES (115191, 'spell_rog_stealth');
INSERT INTO `spell_script_names` VALUES (115192, 'spell_rog_stealth');

-- fix Vanish
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_vanish';
INSERT INTO `spell_script_names` VALUES (131361, 'spell_rog_vanish');

-- fix Sudden Death
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_sudden_death';

-- fix Glyph of Ignite
DELETE FROM `spell_linked_spell` WHERE `comment` LIKE '%Glyph of Ignite%';
INSERT INTO `spell_linked_spell` VALUES (-182287, -12654, 0, 'Glyph of Ignite - remove ignite with speed decrease effect');

-- fix Improved Rake
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_rake_triggered';
INSERT INTO `spell_script_names` VALUES (155722, 'spell_dru_rake_triggered');
