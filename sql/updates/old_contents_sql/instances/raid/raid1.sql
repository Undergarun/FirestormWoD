UPDATE `gameobject_template` SET `flags`=4 WHERE `entry` IN (209137,209346);
UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry` IN (53369, 53102);
UPDATE `creature_template` SET `type_flags`=72 WHERE `entry`=53141;

UPDATE creature_template SET unit_flags=unit_flags & ~33554688 where entry in (
35592,
42098,
45979,
47242,
52409,
52447,
52498,
52524,
52530,
52558,
52571,
52577,
52581,
52593,
52659,
52672,
53086,
53087,
53094,
53095,
53096,
53102,
53115,
53116,
53119,
53120,
53121,
53127,
53128,
53129,
53130,
53134,
53141,
53167,
53178,
53185,
53186,
53187,
53188,
53206,
53209,
53216,
53222,
53223,
53224,
53231,
53237,
53244,
53266,
53361,
53369,
53393,
53395,
53420,
53433,
53435,
53467,
53474,
53487,
53488,
53490,
53492,
53494,
53495,
53529,
53545,
53575,
53585,
53617,
53619,
53621,
53630,
53639,
53640,
53648,
53680,
53691,
53723,
53732,
53784,
53787,
53788,
53789,
53792,
53794,
53795,
53900,
53901,
53914,
53952,
54015,
54019,
54020,
54073,
54143,
54161,
54274,
54275,
54276,
54277,
54299,
54348,
54367,
54401,
54402,
54563) and unit_flags & 33554688;

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN 
(52053, 52054, 53134, 53691, 52142, 53979, 54079, 54080, 52393, 52155, 52151, 52059, 52053, 52148,
52409, 53797, 53798, 53799, 52498, 53576, 53577, 53578, 52530, 54044, 54045, 54046, 52558, 52559,
52560, 52561, 52571, 53856, 53857, 53858, 53494, 53587, 53588, 53589, 53096, 54151, 54161, 54162
);

UPDATE `creature_template` SET `ScriptName`='npc_firelands_ancient_core_hound' WHERE `entry`=53134;
UPDATE `creature_template` SET `ScriptName`='npc_firelands_ancient_lava_dweller' WHERE `entry`=53130;
UPDATE `creature_template` SET `ScriptName`='npc_firelands_fire_scorpion' WHERE `entry`=53127;
UPDATE `creature_template` SET `ScriptName`='npc_firelands_fire_turtle_hatchling' WHERE `entry`=53096;
UPDATE `creature_template` SET `ScriptName`='npc_firelands_flame_archon' WHERE `entry`=54161;

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(99693, 'spell_firelands_ancient_core_hound_dinner_time'),
(99736, 'spell_firelands_ancient_core_hound_flame_breath'),
(97549, 'spell_firelands_ancient_lava_dweller_lava_shower'),
(100263, 'spell_firelands_fire_turtle_hatchling_shell_spin'),
(100797, 'spell_firelands_flame_archon_fiery_torment');

UPDATE `creature_template` SET `ScriptName`='npc_firelands_molten_lord' WHERE `entry`=53115;
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (99555, 'spell_firelands_molten_lord_summon_lava_jets');

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(53495, 0, 0, 0, 0, 0, '99258'),
(54145, 0, 0, 0, 0, 0, '97699 98250 100746');

UPDATE `creature_model_info` SET `bounding_radius`=10, `combat_reach`=10 WHERE `modelid`=38621;

UPDATE `creature_template` SET `minlevel`=88, `maxlevel`=88 WHERE `entry`=53495;

UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=97 WHERE `entry` IN (53494, 53587);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=125 WHERE `entry` IN (53588, 53589);

UPDATE `creature_template` SET `ScriptName`='boss_baleroc' WHERE `entry`=53494;
UPDATE `creature_template` SET `ScriptName`='npc_baleroc_shard_of_torment' WHERE `entry`=53495;
UPDATE `creature_template` SET `ScriptName`='npc_firelands_molten_flamefather' WHERE `entry`=54143;
UPDATE `creature_template` SET `ScriptName`='npc_firelands_magma_conduit' WHERE `entry`=54145;
UPDATE `creature_template` SET `ScriptName`='npc_firelands_magmakin' WHERE `entry`=54144;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (99259, 99489, 99515, 99517);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(99259, 'spell_baleroc_shards_of_torment_aoe'),
(99489, 'spell_baleroc_tormented_aoe'),
(99515, 'spell_baleroc_final_countdown'),
(99517, 'spell_baleroc_final_countdown_script');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (17577, 18100);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(17577, 11, 0, 0, 'achievement_share_the_pain');

DELETE FROM `creature_text` WHERE `entry`=53494;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(53494, 0, 0, 'You are forbidden from my master\'s domain, mortals.', 14, 0, 0, 0, 0, 24441, 'Baleroc aggro'),
(53494, 1, 0, 'Mortal filth... the master\'s keep is forbidden...', 14, 0, 0, 0, 0, 24444, 'Baleroc death'),
(53494, 2, 0, 'Fool mortals. Hurl yourselves into your own demise!', 14, 0, 0, 0, 0, 24446, 'Baleroc event 02'),
(53494, 3, 0, 'By the Firelord\'s command, you, too, shall perish!', 14, 0, 0, 0, 0, 24447, 'Baleroc event03'),
(53494, 4, 0, 'You have been judged.', 14, 0, 0, 0, 0, 24449, 'Baleroc kill 02'),
(53494, 5, 0, 'Your flesh is forfeit to the fires of this realm.', 14, 0, 0, 0, 0, 24450, 'Baleroc kill 03'),
(53494, 4, 1, 'Behold your weakness.', 14, 0, 0, 0, 0, 24451, 'Baleroc kill 04'),
(53494, 4, 2, 'None shall pass!', 14, 0, 0, 0, 0, 24452, 'Baleroc kill 05'),
(53494, 6, 0, 'Burn beneath my molten fury!', 14, 0, 0, 0, 0, 24459, 'Baleroc quest 03');

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

REPLACE INTO `spell_area` (`spell`, `area`, `quest_start`, `quest_end`, `aura_spell`, `racemask`, `gender`, `autocast`, `quest_start_status`, `quest_end_status`) VALUES 
(99488, 4265, 29193, 29225, 0, 0, 2, 1, 64, 11);

REPLACE INTO `creature_template_addon` (`entry`, `auras`) VALUES (53567, '99561');
UPDATE `creature_template` SET `Inhabittype`=5, `ScriptName`='npc_thyrinar' WHERE `entry`=53472;

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=209033;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=209035;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=209036;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=209037;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=209100;

UPDATE `creature_template` SET `npcflag`=0, `unit_flags`=33554434, `IconName`='Pickup', `ScriptName`='npc_firelands_dull_focus' WHERE `entry` IN (53951, 53968, 53970, 53963);

UPDATE `creature_template` SET `unit_flags`=`unit_flags`|0x02000000, `IconName`='Interact', `ScriptName`='npc_firelands_circle_of_thorns_portal' WHERE `entry`=54247;

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209098;

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|1, `ScriptName`='npc_firelands_volcanus' WHERE `entry`=53833;

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry` IN 
(53266, 53268, 53086, 53363,
53420, 53419, 53952, 53393,
53186, 53485, 53729, 53186,
53473, 53953, 54203, 54127,
54184, 54074, 53621, 53585);

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (53231, 53816, 53817, 53818);

UPDATE `creature_template` SET `speed_walk`=1, `speed_run`=1 WHERE `entry`=53363;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry` IN 
(53473, 53086, 53186, 53266,
53268, 53363, 53393, 53393, 
53419, 53420, 53485, 53420, 
53729, 53952, 53953, 54074,
54203, 54127, 54184, 54074);

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(53729, 0, 0, 0, 0, 0, '99908'),
(53473, 0, 0, 0, 0, 0, '99128'),
(54184, 0, 0, 0, 0, 0, '100858');


UPDATE `creature_template` SET `ScriptName`='boss_ragnaros_firelands' WHERE `entry`=52409;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_sulfuras_smash' WHERE `entry` IN (53266, 53268);
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_magma_trap' WHERE `entry`=53086;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_splitting_blow' WHERE `entry`=53393;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_sulfuras_hand_of_ragnaros' WHERE `entry` IN (53420, 53419);
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_son_of_flame' WHERE `entry`=53140;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_molten_seed_caster' WHERE `entry`=53186;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_molten_elemental' WHERE `entry`=53189;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_lava_scion' WHERE `entry`=53231;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_blazing_heat' WHERE `entry`=53473;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_living_meteor' WHERE `entry`=53500;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_malfurion_stormrage' WHERE `entry`=53875;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_cenarius' WHERE `entry`=53872;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_hamuul_runetotem' WHERE `entry`=53876;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_dreadflame_spawn' WHERE `entry`=54203;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_dreadflame' WHERE `entry`=54127;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_cloudburst' WHERE `entry`=54147;

DELETE FROM `conditions` WHERE `SourceEntry` IN 
(99054, 99051, 99172, 100175, 
100176, 100177, 99235, 100178, 
100179, 100180, 99236, 100181, 
100182, 100183, 98498, 100579,
100580, 100581, 100158, 100302,
99216, 99217, 99218, 99145,
101088, 101102, 100342, 100344,
100345, 100907, 100567, 100653,
101237);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 99054, 0, 31, 0,  3, 53140, 'Ragnaros Firelands - Invoke Sons aoe 1'),
(13, 1, 99051, 0, 31, 0,  3, 53140, 'Ragnaros Firelands - Invoke Sons aoe 2'),
(13, 1, 99172, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100175, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100176, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100177, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99235, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100178, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100179, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100180, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99236, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100181, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100182, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100183, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99216, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99217, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99218, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 2, 98498, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 2, 100579, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 2, 100580, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 2, 100581, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 7, 100158, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Power'),
(13, 7, 100302, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Power'),
(13, 1, 99145, 0, 31, 0,  3, 53140, 'Ragnaros Firelands - Blazing Heat heal'),
(13, 1, 101088, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Lavalogged'),
(13, 1, 101102, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Lavalogged'),
(13, 7, 100342, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Draw Out Firelord'),
(13, 7, 100344, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Draw Out Firelord'),
(13, 7, 100345, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Draw Out Firelord'),
(13, 1, 100907, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Freeze'),
(13, 7, 100567, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Breadth of Frost'),
(13, 3, 100653, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Entrapping Roots'),
(13, 3, 101237, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Entrapping Roots');

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(98259, 98260, 100110, 100111, 
98159, 98703, 98708, 100256, 
100257, 100258, 99054, 99012,
98473, 98518, 100252, 100253,
100254, 99172, 100175, 100176, 
100177, 99235, 100178, 100179,
100180, 99236, 100181, 100182,
100183, 99216, 99217, 99218,
99125, 99171, 100172, 100173,
100174, 100171, 100190, 99279,
99267, 101387, 101388, 101389,
100503, 100594, 100915, 101107);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(98259, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(98260, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(100110, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(100111, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(98159, 'spell_ragnaros_firelands_magma_trap_aoe'),
(98703, 'spell_ragnaros_firelands_sulfuras_smash_aoe'),
(98708, 'spell_ragnaros_firelands_sulfuras_smash'),
(100256, 'spell_ragnaros_firelands_sulfuras_smash'),
(100257, 'spell_ragnaros_firelands_sulfuras_smash'),
(100258, 'spell_ragnaros_firelands_sulfuras_smash'),
(99012, 'spell_ragnaros_firelands_splitting_blow'),
(99054, 'spell_ragnaros_firelands_invoke_sons_aoe_1'),
(98473, 'spell_ragnaros_firelands_blazing_speed'),
( 99172, 'spell_ragnaros_firelands_engulfing_flames'),
(100175, 'spell_ragnaros_firelands_engulfing_flames'),
(100176, 'spell_ragnaros_firelands_engulfing_flames'),
(100177, 'spell_ragnaros_firelands_engulfing_flames'),
(99235, 'spell_ragnaros_firelands_engulfing_flames'),
(100178, 'spell_ragnaros_firelands_engulfing_flames'),
(100179, 'spell_ragnaros_firelands_engulfing_flames'),
(100180, 'spell_ragnaros_firelands_engulfing_flames'),
(99236, 'spell_ragnaros_firelands_engulfing_flames'),
(100181, 'spell_ragnaros_firelands_engulfing_flames'),
(100182, 'spell_ragnaros_firelands_engulfing_flames'),
(100183, 'spell_ragnaros_firelands_engulfing_flames'),
(99216, 'spell_ragnaros_firelands_engulfing_flames'),
(99217, 'spell_ragnaros_firelands_engulfing_flames'),
(99218, 'spell_ragnaros_firelands_engulfing_flames'),
(99125, 'spell_ragnaros_firelands_blazing_heat_dummy'),
(99171, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100172, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100173, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100174, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100171, 'spell_ragnaros_firelands_world_in_flame'),
(100190, 'spell_ragnaros_firelands_world_in_flame'),
(99267, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(101387, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(101388, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(101389, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(99279, 'spell_ragnaros_firelands_living_meteor_aoe_2'),
(100503, 'spell_ragnaros_firelands_breadth_of_frost_script'),
(100594, 'spell_ragnaros_firelands_superheated_dmg'),
(100915, 'spell_ragnaros_firelands_superheated_dmg'),
(101107, 'spell_ragnaros_firelands_rage_of_ragnaros_aoe');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN 
(17434, 18102, 17436, 17437, 18103, 18104, 18105, 18106, 17863);

UPDATE `creature_template` SET `type_flags`=`type_flags`|4 WHERE `entry` IN (52409, 53797, 53798, 53799);

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(101384, 'spell_firelands_siphon_essence');

UPDATE `creature_template` SET `ScriptName`='npc_kalecgos_53349' WHERE `entry`=53349;

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (53752, 54112, 53695,  53694, 53713, 53819, 53724);

-- ставим корректный лвл
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry` IN (53752, 53724, 53713);

-- исправляем модели босса и псов
UPDATE `creature_model_info` SET `bounding_radius`=3, `combat_reach`=3 WHERE `modelid`=38448;
UPDATE `creature_model_info` SET `bounding_radius`=3, `combat_reach`=3 WHERE `modelid`=38449;
UPDATE `creature_model_info` SET `bounding_radius`=3, `combat_reach`=3 WHERE `modelid`=38450;

-- добавляем иммуны для псов
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (53695, 53981, 54075, 54076, 53694, 53980, 54077, 54078);

-- исправляем дамаг босса и псов
UPDATE `creature_template` SET `mindmg`=532, `maxdmg`=798 WHERE `entry` IN (53695, 53694);
UPDATE `creature_template` SET `dmg_multiplier`=70 WHERE `entry` IN (53695, 53981, 53694, 53980);
UPDATE `creature_template` SET `dmg_multiplier`=80 WHERE `entry` IN (54075, 54076, 54077, 54078);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822 WHERE `entry`=53691;
UPDATE `creature_template` SET `dmg_multiplier`=97 WHERE `entry` IN (53691, 53979);
UPDATE `creature_template` SET `dmg_multiplier`=110 WHERE `entry` IN (54079, 54080);

-- отключаем лут у босса
UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN (53691, 53979, 54079, 54080);

-- Скрипты
UPDATE `creature_template` SET `ScriptName`='boss_shannox' WHERE `entry`=53691;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_riplimb' WHERE `entry`=53694;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_rageface' WHERE `entry`=53695;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_spear_of_shannox' WHERE `entry`=53752;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_immolation_trap' WHERE `entry`=53724;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_crystal_prison_trap' WHERE `entry`=53713;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_crystal_prison' WHERE `entry`=53819;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (101111, 99837, 99838, 101208, 101209, 101210);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101111, 'spell_shannox_riplimb_dogged_determination'),
(99837, 'spell_shannox_crystal_prison_trap'),
(99838, 'spell_shannox_immolation_trap'),
(101208, 'spell_shannox_immolation_trap'),
(101209, 'spell_shannox_immolation_trap'),
(101210, 'spell_shannox_immolation_trap');

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN 
(53450, 53474, 53642, 53635,
53631, 53745, 53082, 53433,
53450,  53474, 52524,
52581, 52447, 53745, 53082, 
53433, 53178, 53492, 53490);

UPDATE `creature` SET `spawnmask`=15 WHERE `id`=52498;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=53082 AND `spell_id`=98997;

UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry`=53082;
UPDATE `creature_template` SET `scale`=1 WHERE `entry` IN (52524, 53599, 53600, 53601);

UPDATE `creature_template` SET `unit_class`=2 WHERE `entry` IN 
(52498, 53576, 53577, 53578,
52581, 53582, 53584, 53584);

UPDATE `gameobject_template` SET `flags`=36 WHERE `entry`=208877;

UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN (52498, 53576, 53577, 53578);

UPDATE `creature_template` SET `ScriptName`='boss_bethtilac' WHERE `entry`=52498;
UPDATE `creature_template` SET `ScriptName`='npc_bethtilac_spiderweb_filament' WHERE `entry`=53082;
UPDATE `creature_template` SET `ScriptName`='npc_bethtilac_cinderweb_spinner' WHERE `entry`=52524;
UPDATE `creature_template` SET `ScriptName`='npc_bethtilac_cinderweb_drone' WHERE `entry`=52581;
UPDATE `creature_template` SET `ScriptName`='npc_bethtilac_cinderweb_spiderling' WHERE `entry`=52447;
UPDATE `creature_template` SET `ScriptName`='npc_bethtilac_engorged_broodling' WHERE `entry`=53745;

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(99052, 98934, 100648, 100834, 100835,
98471, 100826, 100827, 100828);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(99052, 'spell_bethtilac_smoldering_devastation'),
(98934, 'spell_bethtilac_ember_flare'),
(100648, 'spell_bethtilac_ember_flare'),
(100834, 'spell_bethtilac_ember_flare'),
(100835, 'spell_bethtilac_ember_flare'),
(98471, 'spell_bethtilac_burning_acid'),
(100826, 'spell_bethtilac_burning_acid'),
(100827, 'spell_bethtilac_burning_acid'),
(100828, 'spell_bethtilac_burning_acid');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (17542, 18096);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(17542, 11, 0, 0, 'achievement_death_from_above');

REPLACE INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES (53082, 98997, 1, 0);

UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_rhyolith' WHERE `entry`=53772;

UPDATE `creature_template` SET `flags_extra`=128, `ScriptName`='' WHERE `entry`=55544 LIMIT 1;
UPDATE `creature_template` SET `MovementType`=1 WHERE `entry`=56231 LIMIT 1;
UPDATE `creature_template` SET `MovementType`=1 WHERE `entry`=57437 LIMIT 1;
UPDATE `creature_template` SET `MovementType`=1 WHERE `entry`=57438 LIMIT 1;
UPDATE `creature_template` SET `MovementType`=1 WHERE `entry`=57439 LIMIT 1;
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=56231 LIMIT 1;
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=57437 LIMIT 1;
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=57438 LIMIT 1;
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=57439 LIMIT 1;

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (53500, 53813, 53814, 53815);
