DELETE FROM `creature` WHERE `id`=51293;
UPDATE `creature_template` SET `maxdmg`=600 WHERE `entry`=50063;
UPDATE `creature_template` SET `unit_flags`=`unit_flags`|33554432, `modelid1`=11686, `modelid2`=0, `minlevel`=87, `maxlevel`=87, `faction_A`=14, `faction_H`=14, `flags_extra`=0 WHERE `entry`=51293;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_akmahat' WHERE `entry`=50063;
UPDATE `creature_template` SET `ScriptName`='npc_akmahat_fury_of_the_sands' WHERE `entry`=51293;

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=-93561;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-93561, -94965, 2, 'Akma\'hat - Stone Mantle');

DELETE FROM `conditions` WHERE `SourceEntry`=94946;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 94946, 0, 31, 0, 3, 51293, 'Akma\'hat - Fury of the Sands');

REPLACE INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES 
(30010, 2, 'Cataclysm rare elite bosses');
-- UPDATE `creature` SET `spawntimesecs`=259200 WHERE `guid` IN (317090, 371477, 371491, 318795, 371485);
-- DELETE FROM `pool_creature` WHERE `guid` IN (317090, 371477, 371491, 318795, 371485);
-- INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES 
-- (317090, 30010, 0, 'Cataclysm rare bosses - Mobus'),
-- (371477, 30010, 0, 'Cataclysm rare bosses - Garr'),
-- (371491, 30010, 0, 'Cataclysm rare bosses - Xariona'),
-- (318795, 30010, 0, 'Cataclysm rare bosses - Akma\'hat'),
-- (371485, 30010, 0, 'Cataclysm rare bosses - Julak-Doom');

-- REPLACE INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES 
-- (30011, 1, 'Ankha and Magria');
-- UPDATE `creature` SET `spawntimesecs`=43200 WHERE `guid` IN (318193, 373801, 304256);
-- DELETE FROM `pool_creature` WHERE `guid` IN (318193, 373801, 304256);
-- INSERT INTO `pool_creature` (`guid`, `pool_entry`, `chance`, `description`) VALUES 
-- (318193, 30011, 0, 'Ankha and Magria - Ankha'),
-- (373801, 30011, 0, 'Ankha and Magria - Magria');

-- UPDATE `creature` SET `spawntimesecs`=86400 WHERE `guid`=371493;
-- DELETE FROM `pool_creature` WHERE `guid`=371493;

UPDATE `creature_template` SET `mechanic_immune_mask`=613097436, `InhabitType`=5 WHERE `entry`=50062;
UPDATE `creature_template` SET `ScriptName`='boss_aeonaxx' WHERE `entry`=50062;
UPDATE `creature` SET `MovementType`=2, `position_x`=691.109253, `position_y`=441.321045, `position_z`=159.917511 WHERE `id`=50062 AND `guid`=371493;

-- REPLACE INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
-- (371493, 371493, 0, 0, 0, 0, '');

DELETE FROM `waypoint_data` WHERE `id`=371493;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(371493, 1, 691.109253, 441.321045, 159.917511, 0, 0, 0, 0, 100, 0),
(371493, 2, 734.466858, 335.121704, 151.883545, 0, 0, 0, 0, 100, 0),
(371493, 3, 785.374207, 249.132980, 142.047119, 0, 0, 0, 0, 100, 0),
(371493, 4, 917.375122, 206.658371, 98.268250, 0, 0, 0, 0, 100, 0),
(371493, 5, 1032.703003, 204.170242, 65.425850, 0, 0, 0, 0, 100, 0),
(371493, 6, 1125.020020, 239.496735, 53.314648, 0, 0, 0, 0, 100, 0),
(371493, 7, 1228.294800, 289.709564, 49.963840, 0, 0, 0, 0, 100, 0),
(371493, 8, 1261.522461, 323.307587, 52.345081, 0, 0, 0, 0, 100, 0),
(371493, 9, 1277.443115, 399.509430, 79.511749, 0, 0, 0, 0, 100, 0),
(371493, 10, 1284.742798, 447.079651, 99.989288, 0, 0, 0, 0, 100, 0),
(371493, 11, 1286.424927, 560.721863, 91.720894, 0, 0, 0, 0, 100, 0),
(371493, 12, 1281.497192, 641.266663, 70.514145, 0, 0, 0, 0, 100, 0),
(371493, 13, 1273.012207, 708.533997, 56.190769, 0, 0, 0, 0, 100, 0),
(371493, 14, 1221.520996, 756.301453, 58.341640, 0, 0, 0, 0, 100, 0),
(371493, 15, 1147.755005, 807.564392, 79.985260, 0, 0, 0, 0, 100, 0),
(371493, 16, 1080.575317, 851.238586, 79.550537, 0, 0, 0, 0, 100, 0),
(371493, 17, 991.131470, 835.090698, 69.918159, 0, 0, 0, 0, 100, 0),
(371493, 18, 918.481445, 780.010864, 65.417328, 0, 0, 0, 0, 100, 0),
(371493, 19, 892.277954, 758.644409, 62.941914, 0, 0, 0, 0, 100, 0),
(371493, 20, 817.113770, 705.887756, 52.722054, 0, 0, 0, 0, 100, 0),
(371493, 21, 781.149597, 659.282410, 62.813545, 0, 0, 0, 0, 100, 0),
(371493, 22, 770.083435, 607.540649, 94.355408, 0, 0, 0, 0, 100, 0),
(371493, 23, 753.484253, 529.927979, 141.668213, 0, 0, 0, 0, 100, 0);