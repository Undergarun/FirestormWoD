DELETE FROM `creature` WHERE `id`=43917 AND `map`=725;
DELETE FROM `creature` WHERE `id`=49267 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43743 AND `map`=725;
DELETE FROM `creature` WHERE `id`=42428 AND `map`=725;
DELETE FROM `creature` WHERE `id`=42499 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43242 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43286 AND `map`=725;
DELETE FROM `creature` WHERE `id`=49473 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43159 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43357 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43020 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43014 AND `map`=725;

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43917;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=49267;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43743;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42428;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42499;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43242;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43286;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=49473;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43159;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43357;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42333;

UPDATE `creature_template` SET `unit_flags`=0, `type_flags`=72, `dynamicflags`=0 WHERE `entry`=43438;
UPDATE `creature_template` SET `unit_flags`=0, `type_flags`=72, `dynamicflags`=0 WHERE `entry`=49642;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=42810;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=49647;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0, `InhabitType`=1 WHERE `entry`=43214;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0, `InhabitType`=1 WHERE `entry`=49538;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=49624;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=42333;

UPDATE `instance_template` SET `script`='instance_the_stonecore' WHERE `map`=725;

UPDATE `creature_template` SET `ScriptName`='boss_corborus' WHERE `entry`=43438;
UPDATE `creature_template` SET `ScriptName`='npc_rock_borer' WHERE `entry`=43917;
UPDATE `creature_template` SET `ScriptName`='npc_crystal_shard' WHERE `entry`=49267;

UPDATE `creature_template` SET `ScriptName`='boss_slabhide' WHERE `entry`=43214;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128, `ScriptName`='npc_lava_fissure' WHERE `entry`=43242;

UPDATE `creature_template` SET `ScriptName`='boss_ozruk' WHERE `entry`=42188;
DELETE FROM `creature_text` WHERE `entry`=42188;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42188, 0, 0, 'None may pass into the World\'s Heart!', 1, 0, 0, 0, 0, 21919, 'VO_SC_OZRUK aggro'),
(42188, 1, 0, 'The cycle is complete.', 1, 0, 0, 0, 0, 21920, 'VO_SC_OZRUK kill'),
(42188, 2, 0, 'Break yourselves upon my body. Feel the strength of the earth!', 1, 0, 0, 0, 0, 21921, 'VO_SC_OZRUK'),
(42188, 3, 0, 'A protector has fallen. The World\'s Heart lies exposed!', 1, 0, 0, 0, 0, 21922, 'VO_SC_OZRUK death');

UPDATE `creature_template` SET `ScriptName`='boss_high_priestess_azil' WHERE `entry`=42333;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128, `ScriptName`='npc_gravity_well' WHERE `entry`=42499;

DELETE FROM `spell_script_names` WHERE `spell_id`=79251;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (79251, 'high_priestess_azil_gravity_well_script');

DELETE FROM `creature_text` WHERE `entry`=42333;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42333, 0, 0, '', 1, 0, 0, 0, 0, 21627, 'VO_SC_Azil_Event01'),
(42333, 1, 0, 'Witness the power bestowed upon me by Deathwing! Feel the fury of earth!', 1, 0, 0, 0, 0, 21628, 'VO_SC_Azil spell01'),
(42333, 2, 0, 'For my death, countless more will fall. The burden is now yours to bear.', 1, 0, 0, 0, 0, 21633, 'VO_SC_Azil death'),
(42333, 3, 0, 'The world will be reborn in flames!', 1, 0, 0, 0, 0, 21634, 'VO_SC_Azil aggro1'),
(42333, 3, 1, 'A sacrifice for you, master.', 1, 0, 0, 0, 0, 21635, 'VO_SC_Azil aggro2');

DELETE FROM `creature_text` WHERE `entry`=43391;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(43391, 0, 0, 'Follow me if you dare!', 1, 0, 0, 0, 0, 21783, 'vo_sc_millhouse_event01'),
(43391, 0, 1, 'Prison taught me one very important lesson, well, two if you count how to hold your soap, but yes! SURVIVAL!', 1, 0, 0, 0, 0, 21789, 'vo_sc_millhouse_event07'),
(43391, 1, 0, 'It\'s time for a tactical retreat!', 1, 0, 0, 0, 0, 21784, 'vo_sc_millhouse_event02'),
(43391, 1, 1, 'You\'re gonna be sorry!', 1, 0, 0, 0, 0, 21785, 'vo_sc_millhouse_event03'),
(43391, 2, 0, 'Don\'t say I didn\'t warn ya!', 1, 0, 0, 0, 0, 21786, 'vo_sc_millhouse_event04'),
(43391, 2, 1, 'Ah-ha! I\'ve got you right where I want you!', 1, 0, 0, 0, 0, 21787, 'vo_sc_millhouse_event05'),
(43391, 3, 0, 'Now... witness the full power of Millhouse Manastorm!', 1, 0, 0, 0, 0, 21788, 'vo_sc_millhouse_event06'),
(43391, 4, 0, '', 1, 0, 0, 0, 0, 21794, 'vo_sc_millhouse_death01');

DELETE FROM `creature` WHERE `id`=30621 AND `map`=670;
DELETE FROM `creature` WHERE `id`=39388 AND `map`=670;
DELETE FROM `creature` WHERE `id`=39892 AND `map`=670;
DELETE FROM `creature` WHERE `id`=39900 AND `map`=670;
DELETE FROM `creature` WHERE `id`=39961 AND `map`=670;
DELETE FROM `creature` WHERE `id`=39390 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40030 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40040 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40197 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40228 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40269 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40320 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40355 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40357 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40365 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40567 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40600 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40953 AND `map`=670;
DELETE FROM `creature` WHERE `id`=41040 AND `map`=670;
DELETE FROM `creature` WHERE `id`=41045 AND `map`=670;
DELETE FROM `creature` WHERE `id`=41091 AND `map`=670;
DELETE FROM `creature` WHERE `id`=41095 AND `map`=670;
DELETE FROM `creature` WHERE `id`=44314 AND `map`=670;
DELETE FROM `creature` WHERE `id`=48711 AND `map`=670;
DELETE FROM `creature` WHERE `id`=48844 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40718 AND `map`=670;
DELETE FROM `creature` WHERE `id`=48798 AND `map`=670;
DELETE FROM `creature` WHERE `id`=40846 AND `map`=670;

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=30621;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39388;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39892;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39900;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39961;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39390;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40030;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40040;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40197;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40228;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40269;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40320;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40355;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40357;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40365;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40567;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40600;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40953;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=41040;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=41045;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=41091;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=41095;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44314;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=48711;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=48844;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40718;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=48798;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40846;

UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40030;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40040;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40228;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16 WHERE `entry`=40357;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40355;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40365;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=44314;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16 WHERE `entry`=48844;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16 WHERE `entry`=40600;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=44314;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16 WHERE `entry`=39388;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40567;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=40228;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=48711;

UPDATE `creature_template` SET `vehicleid`=737 WHERE `entry`=40320;

UPDATE `instance_template` SET `script`='instance_grim_batol' WHERE `map`=670;

UPDATE `creature_template` SET `ScriptName`='npc_ascended_flameseeker' WHERE `entry`=39415;
UPDATE `creature_template` SET `ScriptName`='npc_ascended_rockbreaker' WHERE `entry`=40272;
UPDATE `creature_template` SET `ScriptName`='npc_ascended_waterlasher' WHERE `entry`=40273;
UPDATE `creature_template` SET `ScriptName`='npc_ascended_windwalker' WHERE `entry`=39414;
UPDATE `creature_template` SET `ScriptName`='npc_azureborne_guardian' WHERE `entry`=39854;
UPDATE `creature_template` SET `ScriptName`='npc_azureborne_seer' WHERE `entry`=39855;
UPDATE `creature_template` SET `ScriptName`='npc_azureborne_seer' WHERE `entry`=40291;
UPDATE `creature_template` SET `ScriptName`='npc_azureborne_warlord' WHERE `entry`=39909;
UPDATE `creature_template` SET `ScriptName`='npc_crimsonborne_guardian' WHERE `entry`=39381;
UPDATE `creature_template` SET `ScriptName`='npc_crimsonborne_seer' WHERE `entry`=40290;
UPDATE `creature_template` SET `ScriptName`='npc_crimsonborne_seer' WHERE `entry`=39405;
UPDATE `creature_template` SET `ScriptName`='npc_crimsonborne_warlord' WHERE `entry`=39626;
UPDATE `creature_template` SET `ScriptName`='npc_enslaved_burning_ember' WHERE `entry`=39892;
UPDATE `creature_template` SET `ScriptName`='npc_enslaved_rock_elemental' WHERE `entry`=39900;
UPDATE `creature_template` SET `ScriptName`='npc_enslaved_gronn_brute' WHERE `entry`=40166;
UPDATE `creature_template` SET `ScriptName`='npc_faceless_corruptor' WHERE `entry`=39392;
UPDATE `creature_template` SET `ScriptName`='npc_trogg_dweller' WHERE `entry`=39450;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_armsmaster' WHERE `entry`=40306;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_armsmaster' WHERE `entry`=41073;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_beguiler' WHERE `entry`=40167;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_drake' WHERE `entry`=41095;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_earthshaper' WHERE `entry`=39890;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_enforcer' WHERE `entry`=40448;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_enforcer' WHERE `entry`=39956;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_firecatcher' WHERE `entry`=39870;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_shadow_weaver' WHERE `entry`=39954;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_stormbreaker' WHERE `entry`=39962;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_thundercaller' WHERE `entry`=40270;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_war_mage' WHERE `entry`=40268;
UPDATE `creature_template` SET `ScriptName`='npc_twilight_wyrmcaller' WHERE `entry`=39873;
UPDATE `creature_template` SET `ScriptName`='npc_ascended_rockbreaker_fissure' WHERE `entry`=41091;
UPDATE `creature_template` SET `ScriptName`='npc_crimsonborne_warlord_empowering_flames' WHERE `entry`=41045;

UPDATE `creature_template` SET `ScriptName`='boss_general_umbriss' WHERE `entry`=39625;
UPDATE `creature_template` SET `ScriptName`='npc_malignant_trogg' WHERE `entry`=39984;
UPDATE `creature_template` SET `ScriptName`='npc_umbriss_trogg_dweller' WHERE `entry`=45467;

UPDATE `creature_template` SET `ScriptName`='boss_forgemaster_throngus' WHERE `entry`=40177;
UPDATE `creature_template` SET `ScriptName`='npc_cave_in_stalker' WHERE `entry`=40228;
UPDATE `creature_template` SET `ScriptName`='npc_throngus_fire_patch' WHERE `entry`=48711;

UPDATE `creature_template` SET `ScriptName`='boss_drahga_shadowburner' WHERE `entry`=40319;
UPDATE `creature_template` SET `ScriptName`='npc_invoked_flaming_spirit' WHERE `entry`=40357;
UPDATE `creature_template` SET `ScriptName`='npc_invocation_of_flame_stalker' WHERE `entry`=40355;
UPDATE `creature_template` SET `ScriptName`='npc_drahga_valiona' WHERE `entry`=40320;
UPDATE `creature_template` SET `ScriptName`='npc_seeping_twilight' WHERE `entry`=40365;

UPDATE `creature_template` SET `ScriptName`='boss_erudax' WHERE `entry`=40484;
UPDATE `creature_template` SET `ScriptName`='npc_erudax_faceless_corruptor' WHERE `entry`=48844;
UPDATE `creature_template` SET `ScriptName`='npc_erudax_faceless_corruptor' WHERE `entry`=40600;
UPDATE `creature_template` SET `ScriptName`='npc_alexstrasza_egg' WHERE `entry`=40486;
UPDATE `creature_template` SET `ScriptName`='npc_shadow_gale_stalker' WHERE `entry`=44314;
UPDATE `creature_template` SET `ScriptName`='npc_erudax_twilight_hatchling' WHERE `entry`=39388;

DELETE FROM `conditions` WHERE `SourceEntry`=76095;
DELETE FROM `conditions` WHERE `SourceEntry`=76097;
DELETE FROM `conditions` WHERE `SourceEntry`=76029;
DELETE FROM `conditions` WHERE `SourceEntry`=76036;
DELETE FROM `conditions` WHERE `SourceEntry`=76816;
DELETE FROM `conditions` WHERE `SourceEntry`=90872;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 76095, 18, 1, 39414, 'Ascended waterlasher - absorb thunder');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 2, 76097, 18, 1, 39414, 'Ascended waterlasher - lightning cloud');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 76029, 18, 1, 40273, 'Ascended windwalker - absorb water');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 2, 76036, 18, 1, 40273, 'Ascended windwalker - water infusied blades');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 76816, 18, 1, 41095, 'Twilight wyrmcaller - feed pet');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 90872, 18, 1, 41095, 'Twilight wyrmcaller - feed pet');
DELETE FROM `spell_script_names` WHERE `spell_id`=76579;
DELETE FROM `spell_script_names` WHERE `spell_id`=90858;
DELETE FROM `spell_script_names` WHERE `spell_id`=90673;
DELETE FROM `spell_script_names` WHERE `spell_id`=76414;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (76579, 'spell_twilight_thundercaller_electric_blast');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (90858, 'spell_twilight_thundercaller_electric_blast');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (90673, 'spell_twilight_shadow_weaver_shadow_weave');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (76414, 'spell_twilight_enforcer_meat_grinder');

DELETE FROM `conditions` WHERE `SourceEntry`=74670;
DELETE FROM `conditions` WHERE `SourceEntry`=90250;
DELETE FROM `conditions` WHERE `SourceEntry`=74634;
DELETE FROM `conditions` WHERE `SourceEntry`=90249;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 3, 74670, 18, 1, 40040, 'General Umbriss - blitz');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 3, 90250, 18, 1, 40040, 'General Umbriss - blitz');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 7, 74634, 18, 1, 40030, 'General Umbriss - ground siege');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 7, 90249, 18, 1, 40030, 'General Umbriss - ground siege');

DELETE from `spell_script_names` where `spell_id`=75000;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (75000, 'spell_throngus_pick_weapon');

DELETE FROM `spell_script_names` where `spell_id`=75238;
DELETE FROM `spell_script_names` where `spell_id`=90972;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (75238, 'spell_drahga_supernova');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (90972, 'spell_drahga_supernova');

DELETE FROM `spell_target_position` WHERE `id`=75704;
DELETE FROM `spell_target_position` WHERE `id`=91072;
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) 
VALUES (75704, 670, -641.515,  -827.8, 235.5, 3.069); 
INSERT INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) 
VALUES (91072, 670, -641.515,  -827.8, 235.5, 3.069);

DELETE FROM `conditions` WHERE `sourceentry`=75520;
DELETE FROM `conditions` WHERE `sourceentry`=75809;
DELETE FROM `conditions` WHERE `sourceentry`=91049;
DELETE FROM `conditions` WHERE `sourceentry`=75763;
DELETE FROM `conditions` WHERE `sourceentry`=91040;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 75763, 18, 1, 40484, 'Faceless Corruptor - umbral mending');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 91040, 18, 1, 40484, 'Faceless Corruptor - umbral mending');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 75520, 18, 1, 40486, 'Faceless Corruptor - Twilight Corruption');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 75809, 18, 1, 40600, 'Erudax - shield of nightmares');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 75809, 18, 1, 48844, 'Erudax - shield of nightmares');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 91049, 18, 1, 40486, 'Faceless Corruptor - Twilight Corruption');

DELETE FROM `spell_script_names` where `spell_id`=75692;
DELETE FROM `spell_script_names` where `spell_id`=91087;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (75692, 'spell_erudax_shadow_gale');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (91087, 'spell_erudax_shadow_gale');

DELETE FROM `creature_text` WHERE `entry`=39625;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39625, 0, 0, 'A million more await my orders. What chance do you have?', 14, 0, 0, 0, 0, 18530, 'VO_GB_Umbriss_Event01'),
(39625, 1, 0, 'Vermin, your dinner awaits!', 14, 0, 0, 0, 0, 18531, 'VO_GB_Umbriss_Event02'),
(39625, 1, 1, 'Messy...', 14, 0, 0, 0, 0, 18532, 'VO_GB_Umbriss_Event03'),
(39625, 2, 0, '', 14, 0, 0, 0, 0, 18533, 'VO_GB_Umbriss_Event04'),
(39625, 3, 0, '', 14, 0, 0, 0, 0, 18534, 'VO_GB_Umbriss_Event05'),
(39625, 4, 0, 'Attack you cowardly vermin!', 14, 0, 0, 0, 0, 18535, 'VO_GB_Umbriss_Event06'),
(39625, 5, 0, 'Reinforce the front! We are being invaded!', 14, 0, 0, 0, 0, 18536, 'VO_GB_Umbriss_Event07'),
(39625, 6, 0, 'Cover the rear! Alexstrasza\'s brood decimate our ranks!', 14, 0, 0, 0, 0, 18537, 'VO_GB_Umbriss_Event08');

DELETE FROM `creature_text` WHERE `entry` = 40177;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(40177, 0, 0, 'NO! Throngus get whipped again if he no finish!', 14, 0, 0, 0, 0, 18943, 'VO_GB_Throngus_Event01'),
(40177, 1, 0, 'Throngus use your corpse on body. Somewhere...', 14, 0, 0, 0, 0, 18944, 'VO_GB_Throngus_Event02'),
(40177, 1, 1, 'You break easy!', 14, 0, 0, 0, 0, 18945, 'VO_GB_Throngus_Event03'),
(40177, 2, 0, 'Death... Good choice. Not best choice maybe, but better than fail and live.', 14, 0, 0, 0, 0, 18946, 'VO_GB_Throngus_Event04'),
(40177, 3, 0, 'You not get through defenses!', 14, 0, 0, 0, 0, 18947, 'VO_GB_Throngus_Event05'),
(40177, 4, 0, 'Throngus SLICE you up!', 14, 0, 0, 0, 0, 18948, 'VO_GB_Throngus_Event06'),
(40177, 5, 0, 'Oh, this gonna HURT!', 14, 0, 0, 0, 0, 18949, 'VO_GB_Throngus_Event07');

DELETE FROM `creature_text` WHERE `entry` = 40319;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(40319, 0, 0, 'I will burn you from the inside out!', 14, 0, 0, 0, 0, 18610, 'VO_GB_Drahga_Event01'),
(40319, 1, 0, '', 14, 0, 0, 0, 0, 18611, 'VO_GB_Drahga_Event02'),
(40319, 2, 0, 'You should have come better prepared!', 14, 0, 0, 0, 0, 18612, 'VO_GB_Drahga_Event03'),
(40319, 3, 0, 'Valiona, finish them! Avenge me!', 14, 0, 0, 0, 0, 18614, 'VO_GB_Drahga_Event04'),
(40319, 4, 0, 'INCINERATE THEM, MINIONS!', 14, 0, 0, 0, 0, 18616, 'VO_GB_Drahga_Event05'),
(40319, 4, 1, 'BY FIRE BE... BURNED!', 14, 0, 0, 0, 0, 18619, 'VO_GB_Drahga_Event06'),
(40319, 5, 0, 'Dragon, you will do as I command! Catch me!', 14, 0, 0, 0, 0, 18621, 'VO_GB_Drahga_Event07');

DELETE FROM `creature_text` WHERE `entry`=40320;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(40320, 0, 0, 'If they do not kill you, I will do it myself!', 14, 0, 0, 0, 0, 18523, 'VO_GB_Valiona_Event01'),
(40320, 1, 0, 'I will not die for you, Drahga.', 14, 0, 0, 0, 0, 18524, 'VO_GB_Valiona_Event02');

DELETE FROM `creature_text` WHERE `entry` = 40484;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(40484, 0, 0, 'The darkest days are still ahead!', 14, 0, 0, 0, 0, 18638, 'VO_GB_Erudax_Event01'),
(40484, 1, 0, 'Erudax cackles maniacally.', 16, 0, 0, 0, 0, 18639, 'VO_GB_Erudax_Event02'),
(40484, 1, 1, 'Flesh for the offering.', 14, 0, 0, 0, 0, 18640, 'VO_GB_Erudax_Event03'),
(40484, 2, 0, 'Ywaq maq oou; ywaq maq ssaggh. Ywaq ma shg\'fhn.', 14, 0, 0, 0, 0, 18641, 'VO_GB_Erudax_Event04'),
(40484, 3, 0, 'F\'lakh ghet! The shadow\'s hunger cannot be sated!', 14, 0, 0, 0, 0, 18644, 'VO_GB_Erudax_Event05'),
(40484, 4, 0, 'Come, suffering... enter, chaos!', 14, 0, 0, 0, 0, 18646, 'VO_GB_Erudax_Event06');
