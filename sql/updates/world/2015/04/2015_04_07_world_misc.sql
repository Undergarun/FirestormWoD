
-- fix Ember Tap stacking
INSERT INTO `spell_script_names` VALUES (114635, 'spell_warl_ember_tap_glyph');

-- fix Chaos Bolt, removes stacks of Backdraft
INSERT INTO `spell_script_names` VALUES (116858, 'spell_warl_chaos_bolt');

-- fix Divine Insight (shadow) if proc while player is casting
INSERT INTO `spell_script_names` VALUES ('8092', 'spell_pri_mind_blast');

-- fix flight after death
DELETE FROM `spell_area` WHERE `area` IN (6757, 5785, 5805, 5841, 5842, 5840, 6134, 6138, 6006, 6141, 6142, 6134, 6661) and spell=55164;
INSERT INTO `spell_area` VALUES
(55164, 6757, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 5785, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 5805, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 5841, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 5842, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 5840, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 6138, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 6006, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 6141, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 6142, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 6134, 0, 0, 8326, 0, 2, 1, 64, 11),
(55164, 6661, 0, 0, 8326, 0, 2, 1, 64, 11);
