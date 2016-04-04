DELETE FROM `creature` WHERE `map`=671 AND `id` IN (44652, 44645, 44650, 44797, 44641, 44687, 45026);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (44652, 44645, 44650, 44797, 44641, 44687, 45026);

DELETE FROM `creature` WHERE `map`=671 AND `id` IN (46296, 46374, 46448, 46364, 46588, 40718, 46304, 46301, 48798, 50008, 50014);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (46296, 46374, 46448, 46364, 46588, 40718, 46304, 46301, 48798, 50008, 50014);

DELETE FROM `creature` WHERE `map`=671 AND `id` IN (43735, 46147, 51608, 46277, 48415, 47265, 48436, 49863, 48018, 46588);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (46147, 51608, 46277, 48415, 47265, 48436, 49863, 48018, 46588);

DELETE FROM `creature` WHERE `map`=671 AND `id` IN (44201, 41264, 47501, 47040, 44845, 44824, 44747, 45452, 45420, 44553, 43691, 45476, 49432, 49518, 49517);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (44201, 41264, 47501, 47040, 44845, 44824, 44747, 45452, 45420, 44553, 43691, 45476, 49432, 49518, 49517);

DELETE FROM `creature` WHERE `map`=671 AND `id` IN (46965, 43393, 43406, 47020, 47017, 43603, 43592, 47019, 47016, 43622, 44045, 43585, 43707, 45848, 45858, 45685, 45676,  45885, 43888, 43999);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (46965, 43393, 43406, 47020, 47017, 43603, 43592, 47019, 47016, 43622, 44045, 43585, 43707, 45848, 45858, 45685, 45676,  45885, 43888, 43999);

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (49825, 49817, 44600, 45992, 45993, 43687, 43686, 43688, 43689, 43735, 43324, 45213);

UPDATE `creature` SET `phaseMask`=65535, `position_x`=-741.007, `position_y`=-614.859, `position_z`=836.694, `orientation`=4.72725 WHERE `id`=45992; 
UPDATE `creature` SET `phaseMask`=65535, `position_x`=-740.975, `position_y`=-754.839, `position_z`=836.694, `orientation`=1.5621 WHERE `id`=45993; 

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205087;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205088;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205222;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205223;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205224;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205225;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205226;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205227;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205228;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205951;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205950;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=205901;
UPDATE `gameobject` SET `state` = 0 WHERE `id` IN (205222, 205223, 205224, 205225, 205226, 205227);

UPDATE `creature_template` SET `faction_A`=16, `faction_H`=16 WHERE `entry`=44687;
UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16 WHERE `entry`=43622;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=45885;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=47019;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=47020;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=43585;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=43999;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=46965;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=44747;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=44824;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=45452;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=45420;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=45476;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=44845;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=46374;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=46448;
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0 WHERE `entry`=44045;
UPDATE `creature` SET `position_x`=-289.01, `position_y`=-711.88, `orientation`=5.45 WHERE `id`=44600;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40718;
UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry`=46304;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=46304;
UPDATE `creature_template` SET `IconName`='Interact', `npcflag`=1 WHERE `entry`=46301;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=48798;
UPDATE `creature_template` SET `InhabitType`=5, `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=50014;
UPDATE `creature_template` SET `RegenHealth`='0' WHERE `entry`=45213;
UPDATE `creature_template` SET  `minlevel`=87, `maxlevel`=87, `faction_a`=16, `faction_h`=16, `flags_extra`=128 WHERE `entry`=46588;
UPDATE `creature_template` SET  `minlevel`=87, `maxlevel`=87, `faction_a`=16, `faction_h`=16 WHERE `entry`=49863;
UPDATE `creature_template` SET  `minlevel`=87, `maxlevel`=87, `faction_a`=16, `faction_h`=16, `flags_extra`=128 WHERE `entry`=48018;
UPDATE `creature_template` SET `InhabitType`=4, `flags_extra`=128 WHERE `entry`=51608;
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry`=48436;
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry`=49967;
UPDATE `creature_template` SET `rank`=1, `mechanic_immune_mask`=33554432 WHERE `entry`=48415;
UPDATE `creature_template` SET `rank`=1, `mechanic_immune_mask`=33554432 WHERE `entry`=49969;
UPDATE `creature_template` SET `unit_class`=2 WHERE `entry`=46277;

UPDATE `instance_template` SET `script`='instance_bastion_of_twilight' WHERE `map`=671;

DELETE FROM `areatrigger_scripts` WHERE `entry` IN (6437, 6442, 6625, 6626, 6627, 6444, 6312);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES 
(6437, 'at_bt_halfus'),
(6442, 'at_bt_valiona_theralion'),
(6625, 'at_bt_council_1'),
(6626, 'at_bt_council_2'),
(6627, 'at_bt_council_3'),
(6444, 'at_bt_chogall'),
(6312, 'at_bt_sinestra');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (45700, 45687 ,45699, 3622);
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_twilight_portal_shaper' WHERE `entry`=45700;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_twilight_shifter' WHERE `entry`=45687;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_twilight_shadow_mender' WHERE `entry`=45699;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_chogall_dlg' WHERE `entry`=46965;

UPDATE `creature_template` SET `ScriptName`='npc_halfus_dragon' WHERE `entry`=44645;
UPDATE `creature_template` SET `ScriptName`='npc_halfus_dragon' WHERE `entry`=44650;
UPDATE `creature_template` SET `ScriptName`='npc_halfus_dragon' WHERE `entry`=44652;
UPDATE `creature_template` SET `ScriptName`='npc_halfus_dragon' WHERE `entry`=44797;
UPDATE `creature_template` SET `ScriptName`='npc_orphaned_whelp' WHERE `entry`=44641;
UPDATE `creature_template` SET `ScriptName`='npc_proto_behemoth' WHERE `entry`=44687;
UPDATE `gameobject_template` SET `ScriptName`='go_whelp_cage' WHERE `entry`=205087;

UPDATE `creature_template` SET `ScriptName`='npc_valiona_twilight_flames' WHERE `entry`=40718;
UPDATE `creature_template` SET `ScriptName`='npc_dazzling_destruction_stalker' WHERE `entry`=46374;
UPDATE `creature_template` SET `ScriptName`='npc_fabolous_flames' WHERE `entry`=46448;
UPDATE `creature_template` SET `ScriptName`='npc_valiona_twilight_flames' WHERE `entry`=40718;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_rift' WHERE `entry`=50014;
UPDATE `creature_template` SET `ScriptName`='npc_collapsing_twilight_portal' WHERE `entry`=46301;
UPDATE `creature_template` SET `ScriptName`='npc_unstable_twilight' WHERE `entry`=46304;
UPDATE `creature_template` SET `ainame`='', `ScriptName`='npc_twilight_sentry' WHERE `entry`=50008;

UPDATE `creature_template` SET `ScriptName`='boss_feludius' WHERE `entry`=43687;
UPDATE `creature_template` SET `ScriptName`='boss_ignacious' WHERE `entry`=43686;
UPDATE `creature_template` SET `ScriptName`='boss_arion' WHERE `entry`=43688;
UPDATE `creature_template` SET `ScriptName`='boss_terrastra' WHERE `entry`=43689;
UPDATE `creature_template` SET `ScriptName`='boss_elementium_monstrosity' WHERE `entry`=43735;
UPDATE `creature_template` SET `ScriptName`='npc_violent_cyclone' WHERE `entry`=44747;
UPDATE `creature_template` SET `ScriptName`='npc_bt_gravity_well' WHERE `entry`=44824;
UPDATE `creature_template` SET `ScriptName`='npc_liquid_ice' WHERE `entry`=45452;
UPDATE `creature_template` SET `ScriptName`='npc_ascendant_council_plume_stalker' WHERE `entry`=45420;
UPDATE `creature_template` SET `ScriptName`='npc_eruption_target' WHERE `entry`=44845;

UPDATE `creature_template` SET `ScriptName`='boss_chogall' WHERE `entry`=43324;
UPDATE `creature_template` SET `ScriptName`='npc_chogall_shadow_portal' WHERE `entry`=47019;
UPDATE `creature_template` SET `ScriptName`='npc_chogall_fire_portal' WHERE `entry`=47020;
UPDATE `creature_template` SET `ScriptName`='npc_chogall_fire_elemental' WHERE `entry`=47017;
UPDATE `creature_template` SET `ScriptName`='npc_chogall_shadow_lord' WHERE `entry`=47016;
UPDATE `creature_template` SET `ScriptName`='npc_chogall_blaze' WHERE `entry`=43585;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_chogall_corrupting_adherent' WHERE `entry`=43622;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_chogall_blood_of_the_old_god' WHERE `entry`=43707;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_chogall_darkened_creation' WHERE `entry`=44045;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_chogall_corruption' WHERE `entry`=43999;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_chogall_malformation' WHERE `entry`=43888;

UPDATE `creature_template` SET `ScriptName`='boss_sinestra' WHERE `entry`=45213;
UPDATE `creature_template` SET `ScriptName`='npc_sinestra_calen' WHERE `entry`=46277;
UPDATE `creature_template` SET `ScriptName`='npc_sinestra_twilight_flames' WHERE `entry`=46588;
UPDATE `creature_template` SET `ScriptName`='npc_sinestra_pulsing_twilight_egg' WHERE `entry`=46842;
UPDATE `creature_template` SET `ScriptName`='npc_sinestra_barrier_stalker' WHERE `entry`=51608;
UPDATE `creature_template` SET `ScriptName`='npc_sinestra_shadow_orb' WHERE `entry`=49863;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_sinestra_twilight_whelp' WHERE `entry`=47265;
UPDATE `creature_template` SET `ScriptName`='npc_sinestra_twilight_essence' WHERE `entry`=48018;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_sinestra_twilight_drake' WHERE `entry`=48436;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_sinestra_twilight_spitecaller' WHERE `entry`=48415;


-- Halfus Wyrmbreaker
DELETE FROM `conditions` WHERE `SourceEntry` IN (83609, 87683, 84092, 83601, 83603, 83611);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '83609', '31', '3', '44687', 'Atrophic poison'),
('13', '1', '87683', '31', '3', '44600', 'Dragon vengeance'),
('13', '1', '84092', '31', '3', '44600', 'Cyclone winds'),
('13', '1', '83601', '31', '3', '44687', 'Time dilaton'),
('13', '1', '83603', '31', '3', '44600', 'Stone grip'),
('13', '1', '83611', '31', '3', '44600', 'Nether blindness');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (83719, 83603);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(83603, 'spell_halfus_stone_grip'),
(83719, 'spell_halfus_fireball_barrage');

-- Theralion and Valiona
DELETE FROM `conditions` WHERE `SourceEntry`=86840;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '86840', '31', '3', '48798', 'Valiona - Devouring flames');

DELETE FROM `spell_script_names` WHERE `spell_id` IN
(86788, 92876, 92877, 92878, 86408, 88518, 86199, 92868, 92869, 92870, 86228,
92867, 86406, 92926, 92927, 92928, 93055, 93051, 86844, 92872, 92873, 92874);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(86788, 'spell_valiona_blackout'),
(92876, 'spell_valiona_blackout'),
(92877, 'spell_valiona_blackout'),
(92878, 'spell_valiona_blackout'),
(88518, 'spell_valiona_twilight_meteorite'),
(86408, 'spell_theralion_dazzling_destruction'),
(86199, 'spell_valiona_twilight_flame_dmg_1'),
(92868, 'spell_valiona_twilight_flame_dmg_1'),
(92869, 'spell_valiona_twilight_flame_dmg_1'),
(92870, 'spell_valiona_twilight_flame_dmg_1'),
(86228, 'spell_valiona_twilight_flame_dmg_2'),
(92867, 'spell_valiona_twilight_flame_dmg_2'),
(86406, 'spell_theralion_dazzling_destruction_dmg'),
(92926, 'spell_theralion_dazzling_destruction_dmg'),
(92927, 'spell_theralion_dazzling_destruction_dmg'),
(92928, 'spell_theralion_dazzling_destruction_dmg'),
(93055, 'spell_shifting_reality'),
(93051, 'spell_twilight_shift_stack'),
(86844, 'spell_valiona_devouring_flames_dmg'),
(92872, 'spell_valiona_devouring_flames_dmg'),
(92873, 'spell_valiona_devouring_flames_dmg'),
(92874, 'spell_valiona_devouring_flames_dmg');

-- Ascendant Council
DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(82746, 92506, 92507, 92508, 83300, 83067, 92469, 92470, 92471, 83565, 92544, 
92545, 92546, 83675, 83087, 83498, 92476, 92477, 92478, 83581, 84918, 82285);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(82746, 'spell_feludius_glaciate'),
(92506, 'spell_feludius_glaciate'),
(92507, 'spell_feludius_glaciate'),
(92508, 'spell_feludius_glaciate'),
(83300, 'spell_arion_chain_lightning'),
(83067, 'spell_arion_thundershock'),
(92469, 'spell_arion_thundershock'),
(92470, 'spell_arion_thundershock'),
(92471, 'spell_arion_thundershock'),
(83565, 'spell_terrastra_quake'),
(92544, 'spell_terrastra_quake'),
(92545, 'spell_terrastra_quake'),
(92546, 'spell_terrastra_quake'),
(83675, 'spell_terrastra_eruption'),
(83087, 'spell_arion_disperse'),
(83498, 'spell_arion_lashing_winds'),
(92476, 'spell_arion_lashing_winds'),
(92477, 'spell_arion_lashing_winds'),
(92478, 'spell_arion_lashing_winds'),
(83581, 'spell_terrastra_grounded'),
(84918, 'spell_monstrosity_cryogenic_aura'),
(82285, 'spell_elemental_statis');

-- Cho'gall
DELETE FROM `conditions` WHERE `SourceEntry` IN 
(91331, 93206, 93207, 93208, 81194, 93264, 93265,
 93266, 81572, 93218, 93219, 93220, 82630, 82356);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '91331', '31', '3', '43324', 'Chogall - twisted devotion'),
('13', '1', '93206', '31', '3', '43324', 'Chogall - twisted devotion'),
('13', '1', '93207', '31', '3', '43324', 'Chogall - twisted devotion'),
('13', '1', '93208', '31', '3', '43324', 'Chogall - twisted devotion'),
('13', '1', '81194', '31', '3', '43324', 'Chogall - fire destruction'),
('13', '1', '93264', '31', '3', '43324', 'Chogall - fire destruction'),
('13', '1', '93265', '31', '3', '43324', 'Chogall - fire destruction'),
('13', '1', '93266', '31', '3', '43324', 'Chogall - fire destruction'),
('13', '1', '81572', '31', '3', '43324', 'Chogall - empowered shadows'),
('13', '1', '93218', '31', '3', '43324', 'Chogall - empowered shadows'),
('13', '1', '93219', '31', '3', '43324', 'Chogall - empowered shadows'),
('13', '1', '93220', '31', '3', '43324', 'Chogall - empowered shadows'),
('13', '1', '82630', '31', '3', '43622', 'Chogall - consume blood of the old god'),
('13', '1', '82356', '31', '3', '43999', 'Chogall - corruption of the old god visual');

DELETE FROM `spell_script_names` WHERE `spell_id`IN 
(91303, 93203, 93204, 93205, 81628, 82337, 81943, 93196, 93197,
93198, 81831, 93200, 93201, 93202, 81689, 93184, 93185, 93186, 81713,
93175, 93176, 93177, 82919, 93108, 93109, 93110, 81761, 93172, 93173, 
93174, 82363, 93169, 93170, 93171, 91317, 93365, 93366, 93367);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(91303, 'spell_chogall_conversion'),
(93203, 'spell_chogall_conversion'),
(93204, 'spell_chogall_conversion'),
(93205, 'spell_chogall_conversion'),
(81628, 'spell_chogall_summon_corrupting_adherent'),
(82337, 'spell_chogall_fester_blood_script'),
(81943, 'spell_chogall_corruption_accelerated_corruption'),
(93196, 'spell_chogall_corruption_accelerated_corruption'),
(93197, 'spell_chogall_corruption_accelerated_corruption'),
(93198, 'spell_chogall_corruption_accelerated_corruption'),
(81831, 'spell_chogall_corruption_sickness_corruption'),
(93200, 'spell_chogall_corruption_sickness_corruption'),
(93201, 'spell_chogall_corruption_sickness_corruption'),
(93202, 'spell_chogall_corruption_sickness_corruption'),
(81689, 'spell_chogall_corrupting_crash_corruption'),
(93184, 'spell_chogall_corrupting_crash_corruption'),
(93185, 'spell_chogall_corrupting_crash_corruption'),
(93186, 'spell_chogall_corrupting_crash_corruption'),
(81713, 'spell_chogall_depravity_corruption'),
(93175, 'spell_chogall_depravity_corruption'),
(93176, 'spell_chogall_depravity_corruption'),
(93177, 'spell_chogall_depravity_corruption'),
(82919, 'spell_chogall_sprayed_corruption_corruption'),
(93108, 'spell_chogall_sprayed_corruption_corruption'),
(93109, 'spell_chogall_sprayed_corruption_corruption'),
(93110, 'spell_chogall_sprayed_corruption_corruption'),
(81761, 'spell_chogall_spilled_blood_of_the_old_god_corruption'),
(93172, 'spell_chogall_spilled_blood_of_the_old_god_corruption'),
(93173, 'spell_chogall_spilled_blood_of_the_old_god_corruption'),
(93174, 'spell_chogall_spilled_blood_of_the_old_god_corruption'),
(82363, 'spell_chogall_corruption_of_the_old_god_corruption'),
(93169, 'spell_chogall_corruption_of_the_old_god_corruption'),
(93170, 'spell_chogall_corruption_of_the_old_god_corruption'),
(93171, 'spell_chogall_corruption_of_the_old_god_corruption'),
(91317, 'spell_chogall_worshipping'),
(93365, 'spell_chogall_worshipping'),
(93366, 'spell_chogall_worshipping'),
(93367, 'spell_chogall_worshipping');

DELETE FROM `spell_target_position` WHERE `id` IN (81611, 82712, 81618, 82713);
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(81611, 671, -1090.24,  -799.20, 836, 3.07),
(82712, 671, -1090.24,  -799.20, 836, 3.07), 
(81618, 671, -1231.97,  -799.29, 836, 6.16),
(82713, 671, -1231.97,  -799.29, 836, 6.16);

-- Sinestra
DELETE FROM `conditions` WHERE `SourceEntry`=95855;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '95855', '31', '3', '46588', 'Sinestra - Call Flames');

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(89435, 92956, 89421, 92955, 86227,
90044, 90045, 92946, 87299, 87323,
92941, 92852, 92954, 88146, 92950);
INSERT INTO `spell_script_names` VALUES 
(89435, 'spell_sinestra_wrack'),
(92956, 'spell_sinestra_wrack'),
(89421, 'spell_sinestra_wrack'),
(92955, 'spell_sinestra_wrack'),
(86227, 'spell_sinestra_twilight_extinction'),
(90044, 'spell_sinestra_indomitable'),
(90045, 'spell_sinestra_indomitable_aura'),
(92946, 'spell_sinestra_indomitable_aura'),
(87299, 'spell_sinestra_mana_barrier'),
(87323, 'spell_sinestra_pyrrhic_focus'),
(92941, 'spell_sinestra_pyrrhic_focus'),
(92852, 'spell_sinestra_twilight_slicer'),
(92954, 'spell_sinestra_twilight_slicer'),
(88146, 'spell_sinestra_twilight_essence'), 
(92950, 'spell_sinestra_twilight_essence');

-- Halfus Wyrmbreaker
DELETE FROM `creature_text` WHERE `entry`=44600;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(44600, 0, 0, 'Cho\'gall will have your heads! ALL OF THEM!', 14, 0, 0, 0, 0, 20186, 'VO_BT_Halfus_Event01'),
(44600, 1, 0, 'The wyrms will eat well tonight!', 14, 0, 0, 0, 0, 20187, 'VO_BT_Halfus_Event03'),
(44600, 2, 0, 'The burden of the damned falls upon you!', 14, 0, 0, 0, 0, 20188, 'VO_BT_Halfus_Event02');

-- Theralion
DELETE FROM `creature_text` WHERE `entry`=45993;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45993, 0, 0, 'The master was clearly speaking to you, Valiona. I am far too busy to attack ANYONE.', 14, 0, 0, 0, 0, 20300, 'VO_BT_Theralion_Event01'),
(45993, 1, 0, 'How dare you call me worthless! You will see why I am Mothers favored child!', 14, 0, 0, 0, 0, 20301, 'VO_BT_Theralion_Event02'),
(45993, 2, 0, 'Bare your soul to me, mortal.', 14, 0, 0, 0, 0, 20302, 'VO_BT_Theralion_Event03'),
(45993, 3, 0, 'I knew I should have stayed out of this...', 14, 0, 0, 0, 0, 20303, 'VO_BT_Theralion_Event04'),
(45993, 4, 0, 'You are not the boss of me Valiona! I will engulf as I please!', 14, 0, 0, 0, 0, 20304, 'VO_BT_Theralion_Event05'),
(45993, 5, 0, 'There is no escape from the fury of the Twilight Flight!', 14, 0, 0, 0, 0, 20305, 'VO_BT_Theralion_Event06'),
(45993, 6, 0, 'Writhe in AGONY!', 14, 0, 0, 0, 0, 20306, 'VO_BT_Theralion_Event07'),
(45993, 7, 0, 'Theralion cackles maniacally.', 16, 0, 0, 0, 0, 20307, 'VO_BT_Theralion_Laugh1');

-- Valiona
DELETE FROM `creature_text` WHERE `entry` = 45992;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45992, 0, 0, 'Do as the master commands, Theralion! Kill them!', 14, 0, 0, 0, 0, 21894, 'VO_BT_Valiona_Event01'),
(45992, 1, 0, 'You are worthless, Theralion!', 14, 0, 0, 0, 0, 21895, 'VO_BT_Valiona_Event02'),
(45992, 2, 0, 'Breathe your dying breath!', 14, 0, 0, 0, 0, 21896, 'VO_BT_Valiona_Event03'),
(45992, 3, 0, 'At least Theralion dies with me...', 14, 0, 0, 0, 0, 21897, 'VO_BT_Valiona_Event04'),
(45992, 4, 0, 'Theralion! I will engulf the Hallway! Cover their escape!', 14, 0, 0, 0, 0, 21898, 'VO_BT_Valiona_Event05'),
(45992, 5, 0, 'Enter Twilight!', 14, 0, 0, 0, 0, 21899, 'VO_BT_Valiona_Event06');

-- Aquarion
DELETE FROM `creature_text` WHERE `entry` = 43687;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43687, 0, 0, 'You dare invade our lord\'s sanctum?', 14, 0, 0, 0, 0, 20162, 'VO_BT_Feludius_Event01'),
(43687, 1, 0, 'Perish!', 14, 0, 0, 0, 0, 20163, 'VO_BT_Feludius_Event02'),
(43687, 2, 0, 'I will freeze the blood in your veins!', 14, 0, 0, 0, 0, 20164, 'VO_BT_Feludius_Event03'),
(43687, 3, 0, 'But now, witness true power...', 14, 0, 0, 0, 0, 20165, 'VO_BT_Feludius_Event04'),
(43687, 4, 0, 'BEHOLD YOUR DOOM!', 14, 0, 0, 0, 0, 20166, 'VO_BT_Feludius_Event05');

-- Ignacious
DELETE FROM `creature_text` WHERE `entry` = 43686;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43686, 0, 0, 'You will die for your insolence!', 14, 0, 0, 0, 0, 20285, 'VO_BT_Ignacious_Event01'),
(43686, 1, 0, 'More fuel for the fire!', 14, 0, 0, 0, 0, 20286, 'VO_BT_Ignacious_Event02'),
(43686, 2, 0, 'BURN!', 1, 0, 0, 0, 0, 20287, 'VO_BT_Ignacious_Event03'),
(43686, 3, 0, '...the fury of the elements!', 14, 0, 0, 0, 0, 20288, 'VO_BT_Ignacious_Event04'),
(43686, 4, 0, 'BEHOLD YOUR DOOM!', 14, 0, 0, 0, 0, 20289, 'VO_BT_Ignacious_Event05');

-- Arion
DELETE FROM `creature_text` WHERE `entry`=43688;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43688, 0, 0, 'Enough of this foolishness!', 14, 0, 0, 0, 0, 20237, 'VO_BT_Arion_Event01'),
(43688, 1, 0, 'Merely a whisper in the wind...', 14, 0, 0, 0, 0, 20238, 'VO_BT_Arion_Event02'),
(43688, 2, 0, 'Wind, hear my call!', 14, 0, 0, 0, 0, 20239, 'VO_BT_Arion_Event03'),
(43688, 3, 0, 'An impressive display...', 14, 0, 0, 0, 0, 20240, 'VO_BT_Arion_Event04'),
(43688, 4, 0, 'BEHOLD YOUR DOOM!', 14, 0, 0, 0, 0, 20241, 'VO_BT_Arion_Event05');

-- Terrastra
DELETE FROM `creature_text` WHERE `entry`=43689;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43689, 0, 0, 'The soil welcomes your bones!', 14, 0, 0, 0, 0, 21842, 'VO_BT_Terrastra_Event01'),
(43689, 1, 0, 'We will handle them!', 14, 0, 0, 0, 0, 21843, 'VO_BT_Terrastra_Event02'),
(43689, 2, 0, 'The earth will devour you!', 14, 0, 0, 0, 0, 21844, 'VO_BT_Terrastra_Event03'),
(43689, 3, 0, '...to have made it this far.', 14, 0, 0, 0, 0, 21845, 'VO_BT_Terrastra_Event04'),
(43689, 4, 0, 'BEHOLD YOUR DOOM!', 14, 0, 0, 0, 0, 21846, 'VO_BT_Terrastra_Event05');

-- Elementium Monstrosity
DELETE FROM `creature_text` WHERE `entry`=43735;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43735, 0, 0, 'BEHOLD YOUR DOOM!', 14, 0, 0, 0, 0, 20396, 'VO_BT_Monstrosity_Event01'),
(43735, 1, 0, 'Annihilate...', 14, 0, 0, 0, 0, 20397, 'VO_BT_Monstrosity_Event02'),
(43735, 2, 0, 'Eradicate...', 14, 0, 0, 0, 0, 20398, 'VO_BT_Monstrosity_Event03'),
(43735, 3, 0, 'Impossible...', 14, 0, 0, 0, 0, 20399, 'VO_BT_Monstrosity_Event04'),
(43735, 4, 0, 'FEEL THE POWER!', 14, 0, 0, 0, 0, 20400, 'VO_BT_Monstrosity_Event05');

-- Cho'gall
DELETE FROM `creature_text` WHERE `entry`=43324;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43324, 1, 0, '(Blood for the master!)', 14, 0, 0, 0, 0, 22069, 'VO_BT_Chogall_BotEvent16'),
(43324, 1, 1, 'Such is your fate.', 14, 0, 0, 0, 0, 22070, 'VO_BT_Chogall_BotEvent17'),
(43324, 1, 2, 'Pathetic flesh!', 14, 0, 0, 0, 0, 22071, 'VO_BT_Chogall_BotEvent18'),
(43324, 1, 3, '(Finish it! Finish them all!)', 14, 0, 0, 0, 0, 22072, 'VO_BT_Chogall_BotEvent19'),
(43324, 1, 4, 'Embrace your end.', 14, 0, 0, 0, 0, 22073, 'VO_BT_Chogall_BotEvent20'),
(43324, 14, 0, '', 14, 0, 0, 0, 0, 22074, 'VO_BT_Chogall_BotEvent21'),
(43324, 14, 1, '', 14, 0, 0, 0, 0, 22075, 'VO_BT_Chogall_BotEvent22'),
(43324, 14, 2, '', 14, 0, 0, 0, 0, 22076, 'VO_BT_Chogall_BotEvent23'),
(43324, 14, 3, '', 14, 0, 0, 0, 0, 22077, 'VO_BT_Chogall_BotEvent24'),
(43324, 2, 0, 'Give me your strength!', 14, 0, 0, 0, 0, 22078, 'VO_BT_Chogall_BotEvent25'),
(43324, 2, 1, 'Behold the might of the hammer!', 14, 0, 0, 0, 0, 22079, 'VO_BT_Chogall_BotEvent26'),
(43324, 2, 2, '(Stop. Hammertime!)', 14, 0, 0, 0, 0, 22080, 'VO_BT_Chogall_BotEvent27'),
(43324, 3, 0, 'Enough! (To His will all flesh succumbs. From His kingdom, chaos comes.) Yes - what he said.', 14, 0, 0, 0, 0, 22081, 'VO_BT_Chogall_BotEvent28'),
(43324, 4, 0, '(Infinite suffering!)', 14, 0, 0, 0, 0, 22082, 'VO_BT_Chogall_BotEvent29'),
(43324, 4, 1, '(*Snort*)', 14, 0, 0, 0, 0, 22083, 'VO_BT_Chogall_BotEvent30'),
(43324, 4, 2, 'Behold the destiny of your fragile world!', 14, 0, 0, 0, 0, 22084, 'VO_BT_Chogall_BotEvent31'),
(43324, 4, 3, 'More. More!', 14, 0, 0, 0, 0, 22085, 'VO_BT_Chogall_BotEvent32'),
(43324, 4, 4, '(Come, brothers! Feast on hope!)', 14, 0, 0, 0, 0, 22086, 'VO_BT_Chogall_BotEvent33'),
(43324, 4, 5, '(More. They need more.)', 14, 0, 0, 0, 0, 22087, 'VO_BT_Chogall_BotEvent34'),
(43324, 1, 5, '(Body and soul, to be swallowed whole!)', 14, 0, 0, 0, 0, 22088, 'VO_BT_Chogall_BotEvent35'),
(43324, 5, 0, 'Everyone succumbs in the end.', 14, 0, 0, 0, 0, 22089, 'VO_BT_Chogall_BotEvent36'),
(43324, 6, 0, '(Now they see the darkness! Their eyes are open!)', 14, 0, 0, 0, 0, 22090, 'VO_BT_Chogall_BotEvent37'),
(43324, 1, 6, 'Now the Master has them.', 14, 0, 0, 0, 0, 22091, 'VO_BT_Chogall_BotEvent38'),
(43324, 1, 7, '(Chaos, madness! Like a hug for your brain.)', 14, 0, 0, 0, 0, 22092, 'VO_BT_Chogall_BotEvent39'),
(43324, 7, 0, 'Brother, it cannot be... (Destiny has come!) My strength... (Too proud! He speaks in blood! Listen!) My mind... (It is for His glory we serve!) What... is happening... (Gone is the dawn. Come shades of Twilight. Come madness, come havoc, come Infinite Night!)', 14, 0, 0, 0, 0, 22093, 'VO_BT_Chogall_BotEvent40'),
(43324, 8, 0, '(The claws of fate grasp the unbelievers!)', 14, 0, 0, 0, 0, 22094, 'VO_BT_Chogall_BotEvent41'),
(43324, 8, 1, '(The Master sees, the Master sees!)', 14, 0, 0, 0, 0, 22095, 'VO_BT_Chogall_BotEvent42'),
(43324, 9, 0, 'What madness have we wrought? (Not yet enough!)', 14, 0, 0, 0, 0, 22096, 'VO_BT_Chogall_BotEvent43'),
(43324, 10, 0, 'I... can\'t take much more... (Brother, it merely begins!)', 14, 0, 0, 0, 0, 22097, 'VO_BT_Chogall_BotEvent44'),
(43324, 8, 2, '(The Master\'s eye sees all that was and all to be, or not!)', 14, 0, 0, 0, 0, 22098, 'VO_BT_Chogall_BotEvent45'),
(43324, 11, 0, 'Brother, we are shattered. (But the Master... I still hear... so angry... Master...?)', 14, 0, 0, 0, 0, 22099, 'VO_BT_Chogall_BotEvent46'),
(43324, 12, 0, 'Brother, we are shattered. Let her finish them. (The Master does not want them near her... Brother? The Master seethes...)', 14, 0, 0, 0, 0, 22100, 'VO_BT_Chogall_BotEvent47'),
(43324, 13, 0, 'It is finished. (The fate of all.)', 14, 0, 0, 0, 0, 22101, 'VO_BT_Chogall_BotEvent48');

-- Cho'gall Event
DELETE FROM `creature_text` WHERE `entry`=46965;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46965, 0, 0, '', 14, 0, 0, 0, 0, 22054, 'VO_BT_Chogall_BotEvent01'),
(46965, 1, 0, 'Halfus! Hear me! (The Master calls, the Master wants) Protect our secrets, Halfus. Destroy the intruders. (Murder for His glory. Murder for His hunger.)', 14, 0, 0, 0, 0, 22055, 'VO_BT_Chogall_BotEvent02'),
(46965, 2, 0, 'Flesh and sinew, weak but proud. Dare they part the Master\'s shroud? They stumble fumble groping blind, Finding fate and chaos intertwined.', 14, 0, 0, 0, 0, 22056, 'VO_BT_Chogall_BotEvent03'),
(46965, 3, 0, 'Brothers in chaos, the Twilight has come! (Shadows lengthen, endless night.) Deathwing has sundered this world. (Doors once opened never close.) So that its true Lords may return. (The Master comes, He comes He comes.)', 14, 0, 0, 0, 0, 22057, 'VO_BT_Chogall_BotEvent04'),
(46965, 4, 0, 'You are His chosen ones. (He calls you, He knows you). Born of flesh- (flesh) purified by blood- (blood) destined for immortality!', 14, 0, 0, 0, 0, 22058, 'VO_BT_Chogall_BotEvent05'),
(46965, 5, 0, 'Heed His call! Cast down the heretics!', 14, 0, 0, 0, 0, 22059, 'VO_BT_Chogall_BotEvent06'),
(46965, 6, 0, 'Brothers of the Hammer, to me! (Their will is still their own.) The unbelievers still stand. Destroy them! (Crush their bodies, crush their will.)', 14, 0, 0, 0, 0, 22060, 'VO_BT_Chogall_BotEvent07'),
(46965, 7, 0, '', 14, 0, 0, 0, 0, 22061, 'VO_BT_Chogall_BotEvent08'),
(46965, 8, 0, 'Mortals, you merely separate the wheat from the chaff. (Unworthy, unworthy).', 14, 0, 0, 0, 0, 22062, 'VO_BT_Chogall_BotEvent09'),
(46965, 9, 0, 'Valiona, Theralion, put them in their place.', 14, 0, 0, 0, 0, 22063, 'VO_BT_Chogall_BotEvent10'),
(46965, 10, 0, '(Come closer, closer. Let us see your faces. All will be humbled before his power.)', 14, 0, 0, 0, 0, 22064, 'VO_BT_Chogall_BotEvent11'),
(46965, 11, 0, 'Welcome, mortals. (Frail little children). Few have laid eyes on this inner sanctum. (They reach out to touch the fire!)', 14, 0, 0, 0, 0, 22065, 'VO_BT_Chogall_BotEvent12'),
(46965, 12, 0, 'It is here the chosen realize their destiny. (One with eternity). And it is here that you will realize yours.(End of the flesh. End of the flesh)!', 14, 0, 0, 0, 0, 22066, 'VO_BT_Chogall_BotEvent13'),
(46965, 13, 0, 'Brothers of Twilight! The Hammer calls to you! (Fire water earth air). Leave your mortal shell behind. (Fire water earth air). Embrace your new forms for here and everafter! (Burn and drown and crush and suffocate). And use your gifts to destroy the unbelievers. (Burn and drown and crush and suffocate)!!', 14, 0, 0, 0, 0, 22067, 'VO_BT_Chogall_BotEvent14'),
(46965, 14, 0, 'Foolish mortals-(Usurper\'s children!) nothing you have done- (Spawn of a lesser god!) I am TRYING to speak here. (Words, words, words. The Master wants murder.) ALL falls to chaos. ALL will be destroyed. (Chaos, chaos!) Your work here today changes nothing. (Chaos, chaos, all things end) No mortal may see what you have and live. Your end has come.', 14, 0, 0, 0, 0, 22068, 'VO_BT_Chogall_BotEvent15');

-- Sinestra
DELETE FROM `creature_text` WHERE `entry`=45213;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45213, 0, 0, 'We were fools to entrust an imbecile like Cho\'gall with such a sacred duty! I will deal with you intruders myself!', 14, 0, 0, 0, 0, 20199, 'VO_BT_Sinestra_Aggro01'),
(45213, 1, 0, 'Deathwing! I have fallen.... The brood... is lost.', 14, 0, 0, 0, 0, 20200, 'VO_BT_Sinestra_Death01'),
(45213, 2, 0, 'My brood will feed on your bones!', 14, 0, 0, 0, 0, 20201, 'VO_BT_Sinestra_Kill01'),
(45213, 2, 1, 'Powerless....', 14, 0, 0, 0, 0, 20202, 'VO_BT_Sinestra_Kill02'),
(45213, 3, 0, 'You mistake this for weakness? Fool!', 14, 0, 0, 0, 0, 20203, 'VO_BT_Sinestra_Losing01'),
(45213, 4, 0, 'I tire of this. Do you see this clutch amidst which you stand? I have nurtured the spark within them, but that life-force is and always will be mine. Behold, power beyond your comprehension!', 14, 0, 0, 0, 0, 20204, 'VO_BT_Sinestra_Phase01'),
(45213, 5, 0, '', 14, 0, 0, 0, 0, 20205, 'VO_BT_Sinestra_Phase02'),
(45213, 6, 0, 'Enough! Drawing upon this source will set us back months. You should feel honored to be worthy of its expenditure. Now... die!', 14, 0, 0, 0, 0, 20206, 'VO_BT_Sinestra_Phase03'),
(45213, 7, 0, 'Feed, children! Take your fill from their meaty husks!', 14, 0, 0, 0, 0, 20207, 'VO_BT_Sinestra_Spec01'),
(45213, 8, 0, 'The energy infuse within my clutch is mine to reclaim!', 14, 0, 0, 0, 0, 20208, 'VO_BT_Sinestra_Spec02'),
(45213, 9, 0, 'SUFFER!', 14, 0, 0, 0, 0, 20209, 'VO_BT_Sinestra_Spec03'),
(45213, 10, 0, 'FEEL MY HATRED!', 14, 0, 0, 0, 0, 20210, 'VO_BT_Sinestra_Spec04'),
(45213, 11, 0, '', 14, 0, 0, 0, 0, 20211, 'VO_BT_Sinestra_Spec05'),
(45213, 12, 0, '', 14, 0, 0, 0, 0, 20212, 'VO_BT_Sinestra_Spec06');

-- Calen
DELETE FROM `creature_text` WHERE `entry`=46277;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(46277, 0, 0, 'Heroes! You are not alone in this dark place!', 14, 0, 0, 0, 0, 21588, 'VO_BT_Caelestrasz_Intro01'),
(46277, 1, 0, 'Heroes! My power wanes....', 14, 0, 0, 0, 0, 21589, 'VO_BT_Caelestrasz_Losing01'),
(46277, 2, 0, 'Sintharia! Your master owes me a great debt... one that I intend to extract from his consort\'s hide!', 14, 0, 0, 0, 0, 21590, 'VO_BT_Caelestrasz_Phase2'),
(46277, 3, 0, 'The fires dim, champions.... Take this, the last of my power. Succeed where I have failed. Avenge me. Avenge the world....', 14, 0, 0, 0, 0, 21591, 'VO_BT_Caelestrasz_Phase3'),
(46277, 4, 0, 'Flame of life, burn within me and renew my vigor!', 14, 0, 0, 0, 0, 21592, 'VO_BT_Caelestrasz_Recharge01'),
(46277, 5, 0, 'You are weakening, Sintharia! Accept the inevitable!', 14, 0, 0, 0, 0, 21593, 'VO_BT_Caelestrasz_Winning01'),
(46277, 6, 0, 'All is lost.... Forgive me, my Queen....', 14, 0, 0, 0, 0, 21598, 'VO_BT_Caelestrasz_Death');

DELETE FROM `conditions` WHERE `SourceEntry` IN 
(83603, 83611, 84092, 83601, 83609);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 83603, 0, 31, 0, 3, 44600, 'Halfus Wyrmbreaker - Stone Grip'),
(13, 7, 83611, 0, 31, 0, 3, 44600, 'Halfus Wyrmbreaker - Nether Blindness'),
(13, 1, 84092, 0, 31, 0, 3, 44600, 'Halfus Wyrmbreaker - Cyclone Winds'),
(13, 1, 83601, 0, 31, 0, 3, 44687, 'Halfus Wyrmbreaker - Time Dilation'),
(13, 1, 83609, 0, 31, 0, 3, 44687, 'Halfus Wyrmbreaker - Atrothic Poison');

UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry` IN (44687, 46227, 46228, 46229);

UPDATE `creature_template` SET `npcflag`=0, `unit_flags`=0 WHERE `entry` IN
(44652, 46224, 46225, 46226,
44645, 46212, 46213, 46214,
44650, 46218, 46219, 46220,
44797, 46215, 46216, 46217,
44641, 46221, 46222, 46223);

DELETE FROM `creature_template_addon` WHERE `entry` IN
(44652, 46224, 46225, 46226,
44645, 46212, 46213, 46214,
44650, 46218, 46219, 46220,
44797, 46215, 46216, 46217,
44641, 46221, 46222, 46223);

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (44600, 46209, 46210, 46211);

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN
(44652, 46224, 46225, 46226,
44645, 46212, 46213, 46214,
44650, 46218, 46219, 46220,
44797, 46215, 46216, 46217,
44641, 46221, 46222, 46223,
44600, 46209, 46210, 46211);

UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822 WHERE `entry` IN 
(44600, 46209, 46210, 46211);
UPDATE `creature_template` SET `dmg_multiplier`=97 WHERE `entry` IN
(44600, 46209);
UPDATE `creature_template` SET `dmg_multiplier`=110 WHERE `entry` IN
(46210, 46211);

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (15472, 16025);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `ScriptName`) VALUES 
(15472, 11, 'achievement_i_cant_hear_you_over_the_sound_of_how_awesome_i_am');

