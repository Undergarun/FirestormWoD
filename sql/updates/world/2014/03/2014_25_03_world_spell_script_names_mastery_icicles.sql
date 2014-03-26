DELETE FROM spell_script_names WHERE ScriptName = 'spell_mage_mastery_icicles';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_mage_icicles';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_mage_ice_lance';

-- fix Mastery: Icicles
INSERT INTO `spell_script_names` VALUES ('116','spell_mastery_icicles'); -- Frostbolt
INSERT INTO `spell_script_names` VALUES ('44614','spell_mastery_icicles'); -- Frostfire bolt

-- Icicles hits for Mastery: Icicles
INSERT INTO `spell_script_names` VALUES ('148017', 'spell_mastery_icicles_hit');
INSERT INTO `spell_script_names` VALUES ('148018', 'spell_mastery_icicles_hit');
INSERT INTO `spell_script_names` VALUES ('148019', 'spell_mastery_icicles_hit');
INSERT INTO `spell_script_names` VALUES ('148020', 'spell_mastery_icicles_hit');

-- Ice Lance for Mastery: Icicles
INSERT INTO `spell_script_names` VALUES ('30455', 'spell_mastery_icicles_trigger'); -- Ice Lance