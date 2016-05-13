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