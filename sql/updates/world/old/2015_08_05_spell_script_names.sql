DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_sha_ascendance';

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_sha_ascendance_flame', 'spell_sha_ascendance_air', 'spell_sha_ascendance_water');
INSERT INTO `spell_script_names` VALUES
    (114050, 'spell_sha_ascendance_flame'),
    (114051, 'spell_sha_ascendance_air'),
    (114052, 'spell_sha_ascendance_water');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_sha_ascendance_water_heal';
INSERT INTO `spell_script_names` VALUES (114083, 'spell_sha_ascendance_water_heal');
