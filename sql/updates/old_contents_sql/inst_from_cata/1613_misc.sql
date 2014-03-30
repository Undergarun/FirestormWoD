DELETE FROM `creature` WHERE `map`=720 AND `id` IN 
(53158, 53986, 53089, 53896, 
53554, 53541, 53520, 53375, 
53734, 53693, 53698, 53372, 
53898, 53784, 53487, 53787,
54020, 35592);

UPDATE `creature` SET `spawnmask`=0 WHERE `map`=720 AND `id` IN 
(54019, 54015, 53786, 53791, 
53795, 53793, 53914, 53794,
53789, 341088, 53900, 53680,
53792);

UPDATE `creature` SET `position_x`=10.62, `position_y`=-317.29, `position_z`=58.0, `orientation`=2.41 WHERE `id`=52530;

UPDATE `creature_addon` SET `bytes1`=0, `auras`='100712 99905 99595' WHERE `guid`=337598;
UPDATE `creature_template_addon` SET `bytes1`=0, `auras`='100712 99905 99595' WHERE `entry` IN (52530, 54044, 54045, 54046);
UPDATE `creature_template_addon` SET `auras`='98462' WHERE `entry`=53158;
UPDATE `creature_template` SET `Inhabittype`=7 WHERE `entry` IN (53158, 53554, 53541, 53698);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=53554;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=53541;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=53698;
UPDATE `creature_model_info` SET `bounding_radius`=10, `combat_reach`=10 WHERE `modelid`=38446;
UPDATE `creature_template` SET `flags_extra` = `flags_extra` | 128 WHERE `entry` IN (53693, 53698);
UPDATE `creature_template` SET `npcflag`=1, `unit_flags`=0, `type_flags`=1024 WHERE `entry`=53089;
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0, `flags_extra`=128 WHERE `entry` IN (53554, 53541);
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN (53734, 54055, 53896, 54063, 54064, 54065);

UPDATE `creature_template` SET `ScriptName`='boss_alysrazor' WHERE `entry`=52530;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_molten_feather' WHERE `entry`=53089;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_blazing_power' WHERE `entry`=53554;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_incindiary_cloud' WHERE `entry`=53541;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_fiery_vortex' WHERE `entry`=53693;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_fiery_tornado' WHERE `entry`=53698;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_blazing_talon_initiate' WHERE `entry`=53896;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_blazing_talon_clawshaper' WHERE `entry`=53734;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_brushfire' WHERE `entry`=53372;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_blazing_broodmother' WHERE `entry`=53900;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_blazing_broodmother' WHERE `entry`=53680;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_molten_egg' WHERE `entry`=53681;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_voracious_hatchling' WHERE `entry`=53509;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_voracious_hatchling' WHERE `entry`=53898;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_plump_lava_worm' WHERE `entry`=53520;
UPDATE `creature_template` SET `ScriptName`='npc_alysrazor_molten_meteor' WHERE `entry` IN (53489, 53784);

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101223, 'spell_alysrazor_fieroblast'),
(101294, 'spell_alysrazor_fieroblast'),
(101295, 'spell_alysrazor_fieroblast'),
(101296, 'spell_alysrazor_fieroblast'),
(100640, 'spell_alysrazor_harsh_winds'),
(97128, 'spell_alysrazor_molten_feather'),
(97734, 'spell_alysrazor_molten_feather_script'),
(102111, 'spell_alysrazor_cataclysm');

DELETE FROM `creature_text` WHERE `entry` = 52530;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(52530, 0, 0, 'I serve a new master now, mortals!', 14, 0, 100, 0, 0, 24426, 'Alysrazor aggro'),
(52530, 1, 0, 'The light... mustn\'t... burn out...', 14, 0, 100, 0, 0, 24429, 'Alysrazor death'),
(52530, 2, 0, 'BURN!', 14, 0, 100, 0, 0, 24430, 'Alysrazor kill 01'),
(52530, 2, 1, 'For his Glory!', 14, 0, 100, 0, 0, 24431, 'Alysrazor kill 02'),
(52530, 3, 0, 'These skies are MINE!', 14, 0, 100, 0, 0, 24434, 'Alysrazor spiral 01'),
(52530, 3, 1, 'I will burn you from the sky', 14, 0, 100, 0, 0, 24435, 'Alysrazor spiral 02'),
(52530, 4, 0, 'Fire... fire...', 14, 0, 100, 0, 0, 24436, 'Alysrazor transition 01'),
(52530, 5, 0, 'Я переродилась в огне!', 14, 0, 100, 0, 0, 24437, 'Alysrazor transition 02');

DELETE FROM `locales_creature_text` WHERE `entry` = 52530;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(52530, 0, 0, 'Теперь я служу новому господину, смертные!'),
(52530, 1, 0, 'Свет... не должен... погаснуть...'),
(52530, 2, 0, 'ГОРИ!'),
(52530, 2, 1, 'Во славу его!'),
(52530, 3, 0, 'Небо над вами принадлежит МНЕ!'),
(52530, 3, 1, 'Я буду жечь вас с неба!'),
(52530, 4, 0, 'Огонь... Огонь...'),
(52530, 5, 0, 'Я переродилась в огне!');

DELETE FROM `creature_text` WHERE `entry` = 53375;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53375, 0, 0, 'None escape the rage of the Firelands!', 14, 0, 100, 0, 0, 0, 'Herald of the Burning End');

DELETE FROM `locales_creature_text` WHERE `entry` = 53375;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53375, 0, 0, 'Никто не избежит гнева Огненных Просторов!');

DELETE FROM `creature_text` WHERE `entry` = 53734;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53734, 0, 0, 'Together we call upon the lord of fire!', 14, 0, 100, 0, 0, 0, 'Blazing Talon Clawshaper');

DELETE FROM `locales_creature_text` WHERE `entry` = 53734;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53734, 0, 0, 'Вместе мы взываем к владыке огня!');

DELETE FROM `creature_text` WHERE `entry` = 53896;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53896, 0, 0, 'Behold His power!', 14, 0, 100, 0, 0, 0, 'Blazing Talon Initiate'),
(53896, 0, 1, 'Let the unbelievers perish in fire!', 14, 0, 100, 0, 0, 0, 'Blazing Talon Initiate'),
(53896, 0, 2, 'We call upon you, Firelord!', 14, 0, 100, 0, 0, 0, 'Blazing Talon Initiate'),
(53896, 0, 3, 'Witness the majesty of flame!', 14, 0, 100, 0, 0, 0, 'Blazing Talon Initiate');

DELETE FROM `locales_creature_text` WHERE `entry` = 53896;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53896, 0, 0, 'Узрите его силу!'),
(53896, 0, 1, 'Пусть неверные горят в огне!'),
(53896, 0, 2, 'Взываем к тебе, Повелитель огня!'),
(53896, 0, 3, 'Узрите величие огня.');