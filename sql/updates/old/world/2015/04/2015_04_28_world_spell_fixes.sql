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