-- fix Marked for Death
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_marked_for_death';
INSERT INTO `spell_script_names` VALUES ('140149', 'spell_rog_marked_for_death'); -- visual
INSERT INTO `spell_linked_spell` VALUES (137619, 140149, 2, 'Marked for Death (check caster)');

-- fix Nightfall proc from Corruption
INSERT INTO `spell_script_names` VALUES (146739, 'spell_warl_nightfall');

-- fix Ember Tap stacking
INSERT INTO `spell_script_names` VALUES (114635, 'spell_warl_ember_tap_glyph');

-- fix Chaos Bolt, removes stacks of Backdraft
INSERT INTO `spell_script_names` VALUES (116858, 'spell_warl_chaos_bolt');

-- fix Divine Insight (shadow) if proc while player is casting
INSERT INTO `spell_script_names` VALUES ('8092', 'spell_pri_mind_blast');