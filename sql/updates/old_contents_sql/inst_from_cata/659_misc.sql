/*
DELETE FROM `creature` WHERE `guid`=345440 AND `id`=42362;
DELETE FROM `creature_addon` WHERE `guid`=345440;
DELETE FROM `creature` WHERE `guid`=345441 AND `id`=42362;
DELETE FROM `creature_addon` WHERE `guid`=345441;
DELETE FROM `creature` WHERE `guid`=335420 AND `id`=41270;
DELETE FROM `creature_addon` WHERE `guid`=335420;
DELETE FROM `creature` WHERE `guid`=335426 AND `id`=41918;
DELETE FROM `creature_addon` WHERE `guid`=335426;
DELETE FROM `creature` WHERE `guid`=335438 AND `id`=41918;
DELETE FROM `creature_addon` WHERE `guid`=335438;
DELETE FROM `creature` WHERE `guid`=335437 AND `id`=41918;
DELETE FROM `creature_addon` WHERE `guid`=335437;
DELETE FROM `creature` WHERE `guid`=335425 AND `id`=41918;
DELETE FROM `creature_addon` WHERE `guid`=335425;
DELETE FROM `creature` WHERE `guid`=335427 AND `id`=41918;
DELETE FROM `creature_addon` WHERE `guid`=335427;
DELETE FROM `creature` WHERE `guid`=335420 AND `id`=41270;
DELETE FROM `creature_addon` WHERE `guid`=335420;
DELETE FROM `creature` WHERE `guid`=334812 AND `id`=41442;
DELETE FROM `creature_addon` WHERE `guid`=334812;
*/

DELETE FROM `creature` WHERE `map`=669 AND `id` IN (47196, 41440, 41841, 41576, 41961, 50030, 41901, 49811);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (47196, 41440, 41841, 41576, 41961, 50030, 41901, 49811);

DELETE FROM `creature` WHERE `map`=669 AND `id` IN (42733, 42934, 49447, 49416, 43362, 43361);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (42733, 42934, 49447, 49416, 43362, 43361);

DELETE FROM `creature` WHERE `map`=669 AND `id` IN (41546, 41879, 41807, 42121, 49740, 41445, 42958, 42947, 42960, 42949, 42951, 42954, 42956);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (41546, 41879, 41807, 42121, 49740, 41445, 42958, 42947, 42960, 42949, 42951, 42954, 42956);

DELETE FROM `creature` WHERE `map`=669 AND `id` IN (42347, 41843, 42321, 41620, 41789, 41767, 49447, 41806, 49416);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (42347, 41843, 42321, 41620, 41789, 41767, 49447, 41806, 49416);

DELETE FROM `creature` WHERE `map`=669 AND `id` IN (42733, 42934, 42920, 42897);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (42733, 42934, 42920, 42897);

DELETE FROM `creature` WHERE `map`=669 AND `id` IN (49801, 43404, 43396, 43407, 43402, 43400);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (49801, 43404, 43396, 43407, 43402, 43400);

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (41570, 42347, 42186, 42166,  42179,  42178, 42180, 41378, 41442, 43296, 44418); 
-- UPDATE `creature_addon` SET `auras`='' WHERE `guid` IN (334268, 334812, 335036, 345443, 345437, 345436, 345438, 345439, 334171, 335406, 334174, 335405);

UPDATE `creature_template` SET `vehicleId`=0 WHERE `entry` IN (42166,  42179,  42178, 42180);

UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=43361; 
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=43362; 

UPDATE `creature` SET `position_x`=-302.127, `position_y`=-42.25, `position_z`=211.535, `orientation`=4.36019 WHERE `id`=41570;

UPDATE `creature` SET `position_x`=-327.81, `position_y`=-411.58, `position_z`=213.87, `orientation`=1.58 WHERE `id`=42166;
UPDATE `creature` SET `position_x`=-319.47, `position_y`=-410.67, `position_z`=213.87, `orientation`=1.81 WHERE `id`=42178;
UPDATE `creature` SET `position_x`=-336.206, `position_y`=-411.855, `position_z`=214.034, `orientation`=1.12671 WHERE `id`=42179;
UPDATE `creature` SET `position_x`=-309.381, `position_y`=-409.954, `position_z`=213.991, `orientation`=2.24983 WHERE `id`=42180;
UPDATE `creature` SET `position_x`=-324.561, `position_y`=-422.36, `position_z`=213.892, `orientation`=1.58084 WHERE `id`=42186;

UPDATE `creature_template` SET `npcflag`=0 WHERE `entry`=42347;
UPDATE `creature_template` SET `minlevel`=88, `maxlevel`=88, `faction_A`=16, `faction_H`=16, `unit_flags`=33554432 WHERE `entry`=42347;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=88, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=41843;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16 WHERE entry = 42321; 


UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87,  `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=42733;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87,  `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=42934;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87,  `faction_A`=16, `faction_H`=16 WHERE `entry`=42897;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87,  `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=42920;


UPDATE `creature_template` SET `faction_A`=16, `faction_H`=16 WHERE `entry`=41961;
UPDATE `creature_template` SET `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=41901;
UPDATE `creature_template` SET `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=50030;


UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=44202;
UPDATE `creature_template` SET `baseattacktime`=3000 WHERE `entry`=43296;

DELETE FROM `npc_text` WHERE `ID` IN (666660 ,666661, 666662 ,666663, 666664, 666665);
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES
(666660, 'Leaping Leper Gnomes! You\'re a sight for sore eyes. I was worried the giant magma worm out front would deter visitors. Now that you\'re here you can give me a hand.'),
(666661, 'What? Of course not! I could easily escape this simple contraption, but I\'m much too busy. See, a while back I spent a few months inside the belly of a beast... '),
(666662, 'It wasn\'t strictly voluntary, but I discovered something truly incredible. The beast seemed to be able to consume almost anything with no ill effects. '),
(666663, 'Exactly. I ran some tests on the residual digestive fluid coating my suit and...\r\n\r\n<Finkle trails off>\r\n\r\nSay, you haven\'t seen my skinning knife anywhere, have you? I seem to have misplaced it. '),
(666664, 'Oh yes, the bile; it\'s amazing stuff. With the proper catalysts it renders anything to which it\'s applied practically immortal*. I\'ve been harvesting it with my reprogrammed Bile-O-Tron 800 and I\'m certain I\'ve got the mixture right this time. I just need a few volunteers to test it out against a suitable creature. What do you say, friend?\r\n\r\n\'Immortality subject to certain restrictions. '),
(666665, 'Oops! That lever activates the Bile-O-Tron. Oh, and if you happen to come across a key... ');

UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=41807;
UPDATE `creature_template` SET `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=42121;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=41445;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=42958;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=42947;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=42960;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=42949;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=42951;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=42954;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=42956;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=41879;
UPDATE `creature_template` SET `movementId`=0, `minlevel`=88, `maxlevel`=88, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=41546;

DELETE FROM `gameobject` WHERE `id`=24388 AND `map`=669;
DELETE FROM `gameobject` WHERE `id`=202211 AND `map`=669;
DELETE FROM `gameobject` WHERE `id`=205079 AND `map`=669;
DELETE FROM `gameobject` WHERE `id`=207207 AND `map`=669;
DELETE FROM `gameobject` WHERE `id`=185216 AND `map`=669;
DELETE FROM `gameobject` WHERE `id`=185217 AND `map`=669;
DELETE FROM `gameobject` WHERE `id`=185218 AND `map`=669;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=41445;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=42958;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=42947;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=42960;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=42949;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=42951;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=42954;
UPDATE `creature_template` SET `IconName`='Interact' WHERE `entry`=42956;

UPDATE `instance_template` SET `script`='instance_blackwing_descent' WHERE `map`=669;

UPDATE `creature_template` SET `ScriptName`='npc_golem_sentry' WHERE `entry`=42800;
UPDATE `creature_template` SET `ScriptName`='npc_flash_bomb' WHERE `entry`=43361; 
UPDATE `creature_template` SET `ScriptName`='npc_laser_strike' WHERE `entry`=43362;  
UPDATE `creature_template` SET `ScriptName`='npc_drakonid_drudge' WHERE `entry`=42362;
UPDATE `creature_template` SET `ScriptName`='npc_drakonid_chainwielder' WHERE `entry`=42649;
UPDATE `creature_template` SET `ScriptName`='npc_maimgor' WHERE `entry`=42768;
UPDATE `creature_template` SET `ScriptName`='npc_ivoroc' WHERE `entry`=42767;
UPDATE `creature_template` SET `ScriptName`='npc_pyreclaw' WHERE `entry`=42764;
UPDATE `creature_template` SET `ScriptName`='npc_drakonid_slayer' WHERE `entry`=42802;
UPDATE `creature_template` SET `ScriptName`='npc_drakeadon_mongrel' WHERE `entry`=42803;
UPDATE `creature_template` SET `ScriptName`='npc_drakeadon_mongrel' WHERE `entry`=46083;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43119;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43126;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43127;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43130;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43122;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43128;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43125;
UPDATE `creature_template` SET `ScriptName`='npc_bd_spirit_of_dwarf' WHERE `entry`=43129;

UPDATE `creature_template` SET `ScriptName`='npc_lord_victor_nefarius_heroic' WHERE `entry`=49226;

UPDATE `creature_template` SET `ScriptName`='boss_magmaw' WHERE `entry`=41570;
UPDATE `creature_template` SET `ScriptName`='npc_magmaw_head' WHERE `entry`=42347;
UPDATE `creature_template` SET `ScriptName`='npc_lava_parasite' where `entry`=41806;
UPDATE `creature_template` SET `ScriptName`='npc_lava_parasite' where `entry`=42321;
UPDATE `creature_template` SET `ScriptName`='npc_pillar_of_flame' WHERE `entry`=41843;
UPDATE `creature_template` SET `ScriptName`='npc_blazing_bone_construct' WHERE `entry`=49416;

UPDATE `creature_template` SET `ScriptName`='boss_omnotron' WHERE `entry`=42186;
UPDATE `creature_template` SET `ScriptName`='boss_arcanotron' WHERE `entry`=42166;
UPDATE `creature_template` SET `ScriptName`='boss_electron' WHERE `entry`=42179;
UPDATE `creature_template` SET `ScriptName`='boss_magmatron' WHERE `entry`=42178;
UPDATE `creature_template` SET `ScriptName`='boss_toxitron' WHERE `entry`=42180;
UPDATE `creature_template` SET `ScriptName`='npc_poison_bomb' WHERE `entry`=42897;
UPDATE `creature_template` SET `ScriptName`='npc_poison_puddle' WHERE `entry`=42920;
UPDATE `creature_template` SET `ScriptName`='npc_power_generator' WHERE `entry`=42733;
UPDATE `creature_template` SET `ScriptName`='npc_chemical_cloud' WHERE `entry`=42934;

UPDATE `creature_template` SET `ScriptName`='boss_maloriak' WHERE `entry`=41378;
UPDATE `creature_template` SET `ScriptName`='npc_maloriak_flash_freeze' WHERE `entry`=41576;
UPDATE `creature_template` SET `ScriptName`='npc_magma_jet_summon' WHERE `entry`=41901;
UPDATE `creature_template` SET `ScriptName`='npc_magma_jet' WHERE `entry`=50030;
UPDATE `creature_template` SET `ScriptName`='npc_absolute_zero' WHERE `entry`=41961;
UPDATE `creature_template` SET `ScriptName`='npc_prime_subject' WHERE `entry`=41841;
UPDATE `creature_template` SET `ScriptName`='npc_aberration' WHERE `entry`=41440;
UPDATE `creature_template` SET `ScriptName`='npc_vile_swill' WHERE `entry`=49811;

UPDATE `creature_template` SET `ScriptName`='boss_chimaeron' WHERE `entry`=43296;
UPDATE `creature_template` SET `ScriptName`='npc_finkle_einhorn' WHERE `entry`=44202;
UPDATE `creature_template` SET `ScriptName`='npc_bile_o_tron_800' WHERE `entry`=44418;

UPDATE `creature_template` SET `ScriptName`='npc_roaring_flame' WHERE `entry`=41807;
UPDATE `creature_template` SET `ScriptName`='npc_roaring_flame_target' WHERE `entry`=42121;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=41445;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=42958;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=42947;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=42960;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=42949;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=42951;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=42954;
UPDATE `creature_template` SET `ScriptName`='npc_atramedes_gong' WHERE `entry`=42956;
UPDATE `creature_template` SET `ScriptName`='npc_tracking_flames' WHERE `entry`=41879;
UPDATE `creature_template` SET `ScriptName`='boss_atramedes' WHERE `entry`=41442;
UPDATE `creature_template` SET `ScriptName`='npc_sonar_pulse' WHERE `entry`=41546;
UPDATE `creature_template` SET `ScriptName`='npc_abnoxious_fiend' WHERE `entry`=49740;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (80051, 91910);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(80051, 'spell_drakonid_chainwielder_grievous_wound'),
(91910, 'spell_drakonid_chainwielder_grievous_wound');

DELETE FROM `conditions` WHERE `SourceEntry` IN (80875, 80878, 80871, 80872, 80877, 80874, 80876, 80873);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '80875', '31', '1', '43119', 'bestowal of ironstar'),
('13', '1', '80875', '31', '1', '43126', 'bestowal of ironstar'),
('13', '1', '80875', '31', '1', '43130', 'bestowal of ironstar'),
('13', '1', '80878', '31', '1', '43126', 'bestowal of angerforge'),
('13', '1', '80878', '31', '1', '43127', 'bestowal of angerforge'),
('13', '1', '80878', '31', '1', '43130', 'bestowal of angerforge'),
('13', '1', '80871', '31', '1', '43119', 'bestowal of thaurissan'),
('13', '1', '80871', '31', '1', '43127', 'bestowal of thaurissan'),
('13', '1', '80871', '31', '1', '43130', 'bestowal of thaurissan'),
('13', '1', '80872', '31', '1', '43119', 'bestowal of burningeye'),
('13', '1', '80872', '31', '1', '43126', 'bestowal of burningeye'),
('13', '1', '80872', '31', '1', '43127', 'bestowal of burningeye'),
('13', '1', '80877', '31', '1', '43128', 'bestowal of corehammer'),
('13', '1', '80877', '31', '1', '43125', 'bestowal of corehammer'),
('13', '1', '80877', '31', '1', '43129', 'bestowal of corehammer'),
('13', '1', '80874', '31', '1', '43122', 'bestowal of anvilrage'),
('13', '1', '80874', '31', '1', '43125', 'bestowal of anvilrage'),
('13', '1', '80874', '31', '1', '43129', 'bestowal of anvilrage'),
('13', '1', '80876', '31', '1', '43122', 'bestowal of moltenfist'),
('13', '1', '80876', '31', '1', '43128', 'bestowal of moltenfist'),
('13', '1', '80876', '31', '1', '43129', 'bestowal of moltenfist'),
('13', '1', '80873', '31', '1', '43122', 'bestowal of shadowforge'),
('13', '1', '80873', '31', '1', '43128', 'bestowal of shadowforge'),
('13', '1', '80873', '31', '1', '43125', 'bestowal of shadowforge');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (89773, 91912, 94616, 94617, 78941, 91913, 94678, 94679);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(89773, 'spell_magmaw_mangle'),
(91912, 'spell_magmaw_mangle'),
(94616, 'spell_magmaw_mangle'),
(94617, 'spell_magmaw_mangle'),
(78941, 'spell_parasitic_infection'),
(91913, 'spell_parasitic_infection'),
(94678, 'spell_parasitic_infection'),
(94679, 'spell_parasitic_infection');

DELETE FROM `conditions` WHERE `SourceEntry`=91881;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '91881', '31', '3', '42733', 'Lord Victor Nefarian - Overcharge (omnotron heroic mode)');

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(92050, 92023, 91881, 92051, 92135, 91849, 79505, 91531,
91532, 91533, 78741, 79582, 91516, 91517, 91518);

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(92050, 'spell_lord_victor_nefarius_shadow_infusion'),
(92023, 'spell_lord_victor_nefarius_encasing_shadows'),
(91881, 'spell_lord_victor_nefarius_overcharge'),
(92051, 'spell_lord_victor_nefarius_shadow_conductor'),
(92135, 'spell_lord_victor_nefarius_shadow_conductor'),
(91849, 'spell_lord_victor_nefarius_grip_of_death'),
(79505, 'spell_magmatron_flamethrower'),
(91531, 'spell_magmatron_flamethrower'),
(91532, 'spell_magmatron_flamethrower'),
(91533, 'spell_magmatron_flamethrower'),
(78741, 'spell_omnotron_active_trigger'),
(79582, 'spell_magmatron_barrier'),
(91516, 'spell_magmatron_barrier'),
(91517, 'spell_magmatron_barrier'),
(91518, 'spell_magmatron_barrier');

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(77615, 77569, 77991, 77699, 92978,
92979, 92980, 77912, 92965, 92966, 92967);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(77615, 'spell_maloriak_debilitating_slime'),
(77569, 'spell_maloriak_release_aberrations'),
(77991, 'spell_maloriak_release_all_minions'),
(77699, 'spell_maloriak_flash_freeze'),
(92978, 'spell_maloriak_flash_freeze'),
(92979, 'spell_maloriak_flash_freeze'),
(92980, 'spell_maloriak_flash_freeze'),
(77912, 'spell_maloriak_remedy'),
(92965, 'spell_maloriak_remedy'),
(92966, 'spell_maloriak_remedy'),
(92967, 'spell_maloriak_remedy');

DELETE FROM `conditions` WHERE `SourceEntry` IN (77925, 77932, 77937, 92831);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '77925', '31', '1', '41505', 'throw red bottle'),
('13', '1', '77932', '31', '1', '41505', 'throw blue bottle'),
('13', '1', '77937', '31', '1', '41505', 'bthrow green bottle'),
('13', '1', '92831', '31', '1', '41505', 'throw black bottle');

DELETE FROM `conditions` WHERE `SourceEntry`=91304;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '91304', '31', '1', '43296', 'shadow whip');

DELETE FROM `spell_script_names` WHERE `spell_id`=88826;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (88826, 'spell_chimaeron_double_attack');

DELETE FROM `conditions` WHERE `SourceEntry` IN (77611, 78864, 77782);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '77611', '31', '1', '41442', 'Resonating clash'),
('13', '1', '78864', '31', '1', '42949', 'Sonic flames'),
('13', '1', '77782', '31', '1', '42949', 'Sonic flames');

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(77717, 92389, 92390, 92391, 78100, 92553,
78023, 78353, 77675, 77612, 77611, 77709);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(77611, 'spell_atramedes_resonating_clash'),
(77709, 'spell_atramedes_resonating_clash_1'),
(77717, 'spell_atramedes_vertigo'),
(92389, 'spell_atramedes_vertigo'),
(92390, 'spell_atramedes_vertigo'),
(92391, 'spell_atramedes_vertigo'); 

-- Omnotron Defence System
DELETE FROM `creature_text` WHERE `entry`=42186;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42186, 0, 0, 'Primary defense matrix initiated.', 14, 0, 0, 0, 0, 21863, 'VO_BD_Council aggro'),
(42186, 1, 0, 'Defense systems obliterated. Powering down.', 14, 0, 0, 0, 0, 21864, 'VO_BD_Council death'),
(42186, 2, 0, 'Electron unit activated.', 14, 0, 0, 0, 0, 21865, 'VO_BD_Council electron activated'),
(42186, 3, 0, 'Magmatron unit activated.', 14, 0, 0, 0, 0, 21866, 'VO_BD_Council magmatron activated'),
(42186, 4, 0, 'Toxitron unit activated.', 14, 0, 0, 0, 0, 21867, 'VO_BD_Council toxitron activated'),
(42186, 5, 0, 'Arcatron unit activated.', 14, 0, 0, 0, 0, 21868, 'VO_BD_Council arcatron activated'),
(42186, 6, 0, 'Electron unit shield systems online.', 14, 0, 0, 0, 0, 21869, 'VO_BD_Council electron shield'),
(42186, 7, 0, 'Magmatron unit shield systems online.', 14, 0, 0, 0, 0, 21870, 'VO_BD_Council magmatron shield'),
(42186, 8, 0, 'Toxitron unit shield systems online.', 14, 0, 0, 0, 0, 21871, 'VO_BD_Council toxitron shield'),
(42186, 9, 0, 'Arcanotron unit shield systems online.', 14, 0, 0, 0, 0, 21872, 'VO_BD_Council arcanotron shield'),
(42186, 10, 0, 'Rerouting excess energy to Magmatron unit. Engage flamethrower device.', 14, 0, 0, 0, 0, 21873, 'VO_BD_Council magmatron flamethrower');

-- Maloriak
DELETE FROM `creature_text` WHERE `entry`=41378;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(41378, 0, 0, 'There can be no disruptions! Mustn\'t keep the master waiting! Mustn\'t fail again!', 14, 0, 0, 0, 0, 19847, 'VO_BD_Maloriak aggro'),
(41378, 1, 0, 'There will never be another like me..', 14, 0, 0, 0, 0, 19848, 'VO_BD_Maloriak death'),
(41378, 2, 0, 'Nothing goes to waste...', 14, 0, 0, 0, 0, 19849, 'VO_BD_Maloriak_event02'),
(41378, 2, 1, 'Strip the flesh, harvest the organs!', 14, 0, 0, 0, 0, 19850, 'VO_BD_Maloriak_event03'),
(41378, 3, 0, 'Mix and stir, apply heat...', 14, 0, 0, 0, 0, 19851, 'VO_BD_Maloriak_event05'),
(41378, 4, 0, 'This one\'s a little unstable, but what\'s progress without failure?', 14, 0, 0, 0, 0, 19852, 'VO_BD_Maloriak_event06'),
(41378, 5, 0, 'How well does the mortal shell handle extreme temperature change? Must find out! For science!', 14, 0, 0, 0, 0, 19853, 'VO_BD_Maloriak_event07'),
(41378, 6, 0, 'When pushed to the edge, results may become unpredictable!', 14, 0, 0, 0, 0, 19854, 'VO_BD_Maloriak_event08'),
(41378, 7, 0, 'What they lack in intelligence they make up for in ferocity!', 14, 0, 0, 0, 0, 19855, 'VO_BD_Maloriak_event09'),
(41378, 7, 1, 'My failings will be your downfall!', 14, 0, 0, 0, 0, 19856, 'VO_BD_Maloriak_event10'),
(41378, 8, 0, 'Too early, but no choice... They must be tested! Face now my prime subjects!', 14, 0, 0, 0, 0, 19857, 'VO_BD_Maloriak_event11'),
(41378, 8, 1, 'Meet the brawn to my brains! Prime subjects, devour them!', 14, 0, 0, 0, 0, 19858, 'VO_BD_Maloriak_event12');

DELETE FROM `locales_creature_text` WHERE `entry`=41378;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(41378, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нельзя прерывать процесс! Нельзя заставлять господина ждать! Больше нельзя ошибаться!'),
(41378, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Такого, как я, больше никогда не будет...'),
(41378, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ничего не выбрасывайте...'),
(41378, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Отделите мясо, соберите органы!'),
(41378, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Помешивая, довести до кипения...'),
(41378, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Никак не удается стабилизировать, но без ошибок нет прогресса!'),
(41378, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Как смертная оболочка переносит резкие скачки температуры? Надо выяснить! ВО ИМЯ НАУКИ!'),
(41378, 7, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Недостаток ума они с лихвой компенсируют свирепостью!'),
(41378, 7, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мои неудачные творения станут вашей погибелью!'),
(41378, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Рановато, но выбора нет... придется опробовать их! Смотрите, это мои лучшие создания!'),
(41378, 9, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Физическое воплощение моего разума! Дети мои, поглотите их!');

-- Atramedes
DELETE FROM `creature_text` WHERE `entry`=41442;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(41442, 0, 0, 'I have no need for eyes to see my enemies. Your clumsy footsteps and foul stench give you away!', 14, 0, 0, 0, 0, 20820, 'VO_BD_ATRAMEDES aggro'),
(41442, 1, 0, 'Sighted, but blind to the truth. Embrace the finality of it all!', 14, 0, 0, 0, 0, 20821, 'VO_BD_ATRAMEDES kill'),
(41442, 1, 1, 'Death waits in the darkness!', 14, 0, 0, 0, 0, 20822, 'VO_BD_ATRAMEDES aggro'),
(41442, 2, 0, 'This miserable existence finally ends...', 14, 0, 0, 0, 0, 20823, 'VO_BD_ATRAMEDES death'),
(41442, 3, 0, '', 14, 0, 0, 0, 0, 20824, 'VO_BD_ATRAMEDES'),
(41442, 4, 0, '', 14, 0, 0, 0, 0, 20825, 'VO_BD_ATRAMEDES'),
(41442, 5, 0, 'You cannot hide from searing flame!', 14, 0, 0, 0, 0, 20826, 'VO_BD_ATRAMEDES searing flame'),
(41442, 6, 0, 'Yes, run! With every step your heart quickens. The beating, loud and thunderous... Almost deafening. You cannot escape!', 14, 0, 0, 0, 0, 20827, 'VO_BD_ATRAMEDES roaring flame'),
(41442, 7, 0, '', 14, 0, 0, 0, 0, 20828, 'VO_BD_ATRAMEDES ');

DELETE FROM `locales_creature_text` WHERE `entry`=41442;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(41442, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мне не нужны глаза, чтобы видеть врагов. Вас выдает жуткая вонь и топот ног!'),
(41442, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'У тебя есть глаза, но истину ты не видишь. Это твой конец, смирись с этим!'),
(41442, 1, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Смерть ждет во тьме!'),
(41442, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Наконец-то твое жалкое существование закончится.'),
(41442, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'От пламени не спрячешься!'),
(41442, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Да, беги! С каждым шагом твое сердце бьется все быстрее. Эти громкие, оглушительные удары... Тебе некуда бежать!');

-- Chimaeron
DELETE FROM `creature_text` WHERE `entry` = 43296;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43296, 0, 0, 'Chimaeron snorts laudly.', 16, 0, 0, 0, 0, 0, 'VO_BD_Chimaeron aggro'),
(43296, 1, 0, 'Chimaeron prepares to massacre his foes!', 41, 0, 0, 0, 0, 0, 'VO_BD_Chimaeron massacre'),
(43296, 2, 0, 'Chimaeron\'s massacre has knocked the Bile-O-Tron offline!', 41, 0, 0, 0, 0, 0, 'VO_BD_Chimaeron offline'),
(43296, 3, 0, 'The Bile-O-Tron is back online!', 41, 0, 0, 0, 0, 0, 'VO_BD_Chimaeron online');

-- Nefarian Event
DELETE FROM `creature_text` WHERE `entry` = 49226;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(49226, 0, 0, 'Welcome to my personal sanctum, heroes! I hope you find the accommodations to your liking. Herein you will find many of my failed experiments and ultimately my greatest triumph!', 14, 0, 0, 0, 0, 20090, 'VO_BD_Nefarian_Event22'),
(49226, 1, 0, 'Mortals that fancy themselves HEROES have entered the Broken Hall! Oh, I do hope this \'raid\' will amuse me more than the last!', 14, 0, 0, 0, 0, 23379, 'VO_BD_Nefarian_ZoneIntro01'),
(49226, 2, 0, 'I should\'ve known better than to rely on something so stupidly named... to entertain me for long.', 14, 0, 0, 0, 0, 23373, 'VO_BD_Nefarian_OmnitronDead01'),
(49226, 3, 0, 'Hmm, the Omnotron Defense System. Centuries ago, these constructs were considered the dwarves greatest tactical achievements. With so many counters to each construct\'s attacks, I\'ll have to rectify these designs for them to provide me ANY entertainment!', 14, 0, 0, 0, 0, 23374, 'VO_BD_Nefarian_OmnitronIntro01'),
(49226, 4, 0, 'You would like to move away from that, wouldn\'t you? Too bad.', 14, 0, 0, 0, 0, 23375, 'VO_BD_Nefarian_OmnitronSpell01'),
(49226, 5, 0, 'Stupid dwarves and your fascination with runes. Why would you create something that would help your enemy?', 14, 0, 0, 0, 0, 23376, 'VO_BD_Nefarian_OmnitronSpell02'),
(49226, 6, 0, 'Lightning Conductor works much better in a crowd, yes?', 14, 0, 0, 0, 0, 23377, 'VO_BD_Nefarian_OmnitronSpell03'),
(49226, 7, 0, 'Were you planing on using Toxitron\'s chemicals to damage the other constructs? Clever plan. Let me ruin that for you.', 14, 0, 0, 0, 0, 23378, 'VO_BD_Nefarian_OmnitronSpell04'),
(49226, 8, 0, 'That malfunctioning piece of junk was murder on the repair bills.', 14, 0, 0, 0, 0, 20086, 'VO_BD_Nefarian_Event18'),
(49226, 9, 0, 'I found this fascinating speciman in the lava underneath this very room. Magmaw should provide an adequate challenge for your pathetic little band!', 14, 0, 0, 0, 0, 23367, 'VO_BD_Nefarian_MagmawIntro01'),
(49226, 10, 0, 'Finally someone to deal with that unsightly pest! You have my thanks, \"heroes.\"', 14, 0, 0, 0, 0, 20085, 'VO_BD_Nefarian_Event17'),
(49226, 11, 0, 'You won! How... disappointing. I could\'ve easily watched you all continue to stand in the fire.', 14, 0, 0, 0, 0, 23366, 'VO_BD_Nefarian_MagmawDead01'),
(49226, 12, 0, 'INCONCEIVEABLE! You may actually defeat my lava worm! Perhaps I can help... TIP the scales.', 14, 0, 0, 0, 0, 23368, 'VO_BD_Nefarian_MagmawLowHealth'),
(49226, 13, 0, 'On second thought, what fun is an adequate challenge?', 14, 0, 0, 0, 0, 23369, 'VO_BD_Nefarian_MagmawSummon01'),
(49226, 14, 0, 'Maloriak, try not to lose to these mortals! Semi-competent help is SO hard to create.', 14, 0, 0, 0, 0, 23372, 'VO_BD_Nefarian_MaloriakIntro01'),
(49226, 15, 0, 'Your mixtures are weak, Maloriak! They need a bit more... kick!', 14, 0, 0, 0, 0, 23370, 'VO_BD_Nefarian_Maloriak01'),
(49226, 16, 0, 'Congratulations! Allow me to grant you a title befitting the amazing achievement you just performed! Henceforth, you shall be known as the Slayer of Incompetent, Stupid and Disappointing Minions!', 14, 0, 0, 0, 0, 23371, 'VO_BD_Nefarian_MaloriakDead01'),
(49226, 17, 0, 'We never did see eye to eye... Good riddance I say!', 14, 0, 0, 0, 0, 20089, 'VO_BD_Nefarian_Event21'),
(49226, 18, 0, 'You should\'ve been disposed of a long time ago.', 14, 0, 0, 0, 0, 23357, 'VO_BD_Nefarian_AtramedesDead'),
(49226, 19, 0, 'These shields are proving quite the nuisance!', 14, 0, 0, 0, 0, 23358, 'VO_BD_Nefarian_AtramedesGong'),
(49226, 20, 0, 'Atramedes, the heroes are RIGHT THERE!', 14, 0, 0, 0, 0, 23359, 'VO_BD_Nefarian_AtramedesImp01'),
(49226, 21, 0, 'Atramedes, are you going deaf as well as blind? Hurry up and kill them all!', 14, 0, 0, 0, 0, 23360, 'VO_BD_Nefarian_AtramedesIntro'),
(49226, 22, 0, 'Now that was really unnecessary.', 14, 0, 0, 0, 0, 23387, 'VO_BD_Nefarian_Event19'),
(49226, 23, 0, 'Impressive! You managed to destroy one of my most horrific creations - a task I\'d thought impossible until now.', 14, 0, 0, 0, 0, 20088, 'VO_BD_Nefarian_Event20'),
(49226, 24, 0, 'A shame to lose that experiment...', 14, 0, 0, 0, 0, 23361, 'VO_BD_Nefarian_ChimaronDead01'),
(49226, 25, 0, 'Ah, Chimaeron! Truly a living testament to my scientific prowess. I reworked and twisted his form countless times over the years and the final result is truly something to behold.', 14, 0, 0, 0, 0, 23362, 'VO_BD_Nefarian_ChimaronIntro01'),
(49226, 26, 0, 'Oh. You were expecting something else to happen just then, weren\'t you? Let\'s see how YOU adapt after a new set of adjustments, shall we?', 14, 0, 0, 0, 0, 23363, 'VO_BD_Nefarian_ChimaronSpell01'),
(49226, 27, 0, 'Stop fighting yourself THIS INSTANT Chimaeron!', 14, 0, 0, 0, 0, 23364, 'VO_BD_Nefarian_ChimaronSpell02'),
(49226, 28, 0, 'This fight has gone a bit stale. Allow me to spice things up!', 14, 0, 0, 0, 0, 23365, 'VO_BD_Nefarian_ChimaronSpell03');

-- Maloriak (atramedes)
DELETE FROM `creature_text` WHERE `entry`=43404;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43404, 0, 0, 'My sincerest apologies for the disturbance, my liege. But I believe I have something you may be very interested in!', 0, 0, 0, 0, 0, 19860, 'VO_BD_Maloriak_event13'),
(43404, 1, 0, 'Yes, yes!', 0, 0, 0, 0, 0, 19861, 'VO_BD_Maloriak_event14'),
(43404, 2, 0, 'By extracting agents from the blood of various dragonflights I have created a salve that will bestow the wearer sight beyond sight!', 0, 0, 0, 0, 0, 19862, 'VO_BD_Maloriak_event15'),
(43404, 3, 0, 'Senses beyond this realm of mortality!', 0, 0, 0, 0, 0, 19863, 'VO_BD_Maloriak_event16'),
(43404, 4, 0, 'Atramedes! Your master beckons.', 0, 0, 0, 0, 0, 19864, 'VO_BD_Maloriak_event17'),
(43404, 5, 0, 'I present to you experiment number 25463-D!', 0, 0, 0, 0, 0, 19865, 'VO_BD_Maloriak_event18'),
(43404, 6, 0, 'How could this be?! I will dispose of him immediately!', 0, 0, 0, 0, 0, 19866, 'VO_BD_Maloriak_event19'),
(43404, 7, 0, 'Yes, my lord.', 0, 0, 0, 0, 0, 19867, 'VO_BD_Maloriak_event20');

-- Nefarian (atramedes)
DELETE FROM `creature_text` WHERE `entry`=43396;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43396, 0, 0, 'Ah, Maloriak. How gracious of you to finally appear from that quarantine you call a laboratory. What have you got this time?', 12, 0, 0, 0, 0, 20091, 'VO_BD_Nefarian_Event23'),
(43396, 1, 0, 'By all means, enlighten me.', 12, 0, 0, 0, 0, 20092, 'VO_BD_Nefarian_Event24'),
(43396, 2, 0, 'It appears as though your experiment has failed. That whelp has gone completely blind! Look at him.', 12, 0, 0, 0, 0, 20093, 'VO_BD_Nefarian_Event25'),
(43396, 3, 0, 'LOOK AT HIM!', 12, 0, 0, 0, 0, 20156, 'VO_BD_Nefarian_Event26'),
(43396, 4, 0, 'No, not yet.', 12, 0, 0, 0, 0, 20094, 'VO_BD_Nefarian_Event27'),
(43396, 5, 0, 'This object of your chagrin may still prove... valuable... to me.', 12, 0, 0, 0, 0, 20095, 'VO_BD_Nefarian_Event28'),
(43396, 6, 0, 'Leave now, pathetic alchemist. I anxiously await your next failure.', 12, 0, 0, 0, 0, 20096, 'VO_BD_Nefarian_Event29');
