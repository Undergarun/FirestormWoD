DELETE FROM access_requirement WHERE mapId = 996;
INSERT INTO access_requirement VALUES
(996, 3, 90, 90, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'), -- 10 Men
(996, 4, 91, 91, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'), -- 25 Men
(996, 5, 91, 91, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'), -- 10 Men (H)
(996, 6, 91, 91, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'), -- 25 Men (H)
(996, 7, 91, 91, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'); -- LFR

DELETE FROM areatrigger_teleport WHERE id IN (8027, 8028);
INSERT INTO areatrigger_teleport VALUES
(8027, 'Terrace of Endless Spring', 996, -1021.1178, -3141.601, 27.7123, 1.5453), -- In
(8028, 'Terrace of Endless Spring', 870, 954.877, -56.4318, 511.554, 0.972922); -- Out

DELETE FROM instance_template WHERE map = 996;
INSERT INTO instance_template VALUE (996, 870, 'instance_terrace_of_endless_spring', 0);

DELETE FROM gameobject WHERE id IN (214525, 214853, 214852, 214851, 214854, 214850, 214849, 214498);
INSERT INTO `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(214525, 870, 6006, 6372, 1, 1, 957.375, -52.824, 513.151, 0, 0, 0, 0, 1, 7200, 255, 1),
(214525, 996, 6067, 6515, 8, 65535, -3157.253, -3050, 13, 0, 0, 0, 0, 1, 7200, 255, 1), -- 214525 (Area: -1)
(214852, 996, 6067, 6515, 8, 65535, -2911.348, -3050, 13, 0, 0, 0, -0.7071068, 0.7071068, 7200, 255, 1), -- 214852 (Area: -1)
(214851, 996, 6067, 6515, 8, 65535, -2911.576, -3050, 13, 0, 0, 0, -0.7071068, 0.7071068, 7200, 255, 1), -- 214851 (Area: -1)
(214850, 996, 6067, 6515, 8, 65535, -2771.996, -3050, 13, 0, 0, 0, -0.7071068, 0.7071068, 7200, 255, 1), -- 214850 (Area: -1)
(214849, 996, 6067, 6515, 8, 65535, -2771.908, -3050, 13, 0, 0, 0, -0.7071068, 0.7071068, 7200, 255, 1), -- 214849 (Area: -1)
(214853, 996, 6067, 6515, 8, 65535, -1017.96, -3049.18, 12.8247, 1.53361, 0, 0, 0, 0, 7200, 255, 1),
(214854, 996, 6067, 6515, 8, 65535, -1017.96, -3049.18, 12.8247, 1.53361, 0, 0, 0, 0, 7200, 255, 1);

DELETE FROM creature WHERE map = 996;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(36737, 996, 6067, 6515, 8, 65535, -1012.141, -3137.469, 27.18475, 4.667953, 7200, 0, 0), -- 36737 (Area: -1)
(64846, 996, 6067, 6515, 8, 65535, -1036.106, -3136.929, 27.33581, 5.415483, 7200, 0, 0), -- 64846 (Area: -1)
(60957, 996, 6067, 6515, 8, 65535, -1017.972, -2938.677, 19.43974, 4.690664, 7200, 0, 0), -- 60957 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1018.58, -2992.519, 12.39281, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -967.1077, -3021.302, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -961.7952, -3041.033, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1047.639, -2999.571, 12.20952, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -979.0972, -3006.896, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1064.377, -3077.078, 12.20952, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1070.936, -3030.722, 12.20952, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1047.977, -3096.87, 12.20952, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -980.7986, -3090.003, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -969.5156, -3075.156, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -993.757, -2999.411, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1060.609, -3012.747, 12.20952, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -963.3073, -3059.722, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1074.8, -3053.674, 12.20953, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1024.486, -3101.462, 12.2165, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1018.58, -2992.519, 12.39281, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62962, 996, 6067, 6515, 8, 65535, -1001.01, -3098.932, 12.20952, 4.655136, 7200, 0, 0), -- 62962 (Area: -1)
(62983, 996, 6067, 6515, 8, 65535, -1017.93, -2911.302, 19.9015, 4.70965, 7200, 0, 0), -- 62983 (Area: -1)
(65736, 996, 6067, 6515, 8, 65535, -1214.8, -2824.82, 41.24303, 3.507344, 7200, 0, 0), -- 65736 (Area: -1)
(65736, 996, 6067, 6515, 8, 65535, -832.0764, -2745.399, 31.67754, 0.1536942, 7200, 0, 0), -- 65736 (Area: -1)
(60788, 996, 6067, 6515, 8, 65535, -1017.835, -2771.984, 38.65444, 1.578282, 7200, 0, 0), -- 60788 (Area: -1)
(61038, 996, 6067, 6515, 8, 65535, -1214.795, -2824.823, 41.24303, 3.506719, 7200, 0, 0), -- 61038 (Area: -1)
(65736, 996, 6067, 6515, 8, 65535, -1075.2, -2577.82, 15.85183, 1.676501, 7200, 0, 0), -- 65736 (Area: -1)
(65736, 996, 6067, 6515, 8, 65535, -1214.8, -2824.82, 41.24303, 3.507344, 7200, 0, 0), -- 65736 (Area: -1)
(61042, 996, 6067, 6515, 8, 65535, -1075.201, -2577.82, 15.85184, 1.742212, 7200, 0, 0), -- 61042 (Area: -1)
(65736, 996, 6067, 6515, 8, 65535, -1075.2, -2577.82, 15.85183, 1.676501, 7200, 0, 0), -- 65736 (Area: -1)
(61038, 996, 6067, 6515, 8, 65535, -1214.795, -2824.823, 41.24303, 3.506719, 7200, 0, 0), -- 61038 (Area: -1)
(65736, 996, 6067, 6515, 8, 65535, -1214.8, -2824.82, 41.24303, 3.507344, 7200, 0, 0), -- 65736 (Area: -1)
(71095, 996, 6067, 6515, 8, 65535, -1017.91, -2771.976, 38.65443, 4.709265, 7200, 0, 0), -- 71095 (Area: -1)
(60583, 996, 6067, 6515, 8, 65535, -1017.976, -3058.065, 12.824, 4.723, 7200, 0, 0), -- Kaolan
(60586, 996, 6067, 6515, 8, 65535, -1025.808, -3044.398, 12.824, 4.723, 7200, 0, 0), -- Asani
(60585, 996, 6067, 6515, 8, 65535, -1010.243, -3043.981, 12.824, 4.723, 7200, 0, 0), -- Regail
(64368, 996, 6067, 6515, 8, 65535, -1051.87, -3064.8, 12.5693, 0.402647, 7200, 0, 0),
(64368, 996, 6067, 6515, 8, 65535, -984.876, -3034.62, 12.5693, 3.55995, 7200, 0, 0),
(66100, 996, 6067, 6515, 8, 65535, -987.185, -3065.42, 12.5698, 2.6803, 7200, 0, 0),
(66100, 996, 6067, 6515, 8, 65535, -1050.24, -3033.98, 12.5698, 5.8474, 7200, 0, 0);

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
rank = 3,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 8,
baseattacktime = 2000
WHERE entry IN (60585, 60586);
UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
rank = 3,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 16,
baseattacktime = 2000
WHERE entry = 60583;
UPDATE creature_template SET Health_mod = 192.06, ScriptName = 'boss_protector_kaolan', mechanic_immune_mask = 769638399 WHERE entry = 60583;
UPDATE creature_template SET Health_mod = 124.74, ScriptName = 'boss_ancient_regail', mechanic_immune_mask = 769638399 WHERE entry = 60585;
UPDATE creature_template SET Health_mod = 124.74, ScriptName = 'boss_ancient_asani', mechanic_immune_mask = 769638399 WHERE entry = 60586;
UPDATE creature_template SET
minlevel = 90,
maxlevel = 90,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_defiled_ground',
modelid1 = 11686,
modelid2 = 0,
flags_extra = 128
WHERE entry = 60906;
UPDATE creature_template SET minlevel = 90, maxlevel = 90, faction_A = 14, faction_H = 14, ScriptName = 'mob_coalesced_corruption' WHERE entry = 60886;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, minlevel = 90, maxlevel = 90, ScriptName = 'mob_cleansing_water' WHERE entry = 60646;
UPDATE creature_template SET minlevel = 90, maxlevel = 90, faction_A = 14, faction_H = 14, ScriptName = 'mob_corrupting_waters' WHERE entry = 60621;
UPDATE creature_template SET
minlevel = 91,
maxlevel = 91,
faction_A = 14,
faction_H = 14,
Health_mod = 31.5,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 4
WHERE entry IN (64368, 66100);
UPDATE creature_template SET ScriptName = 'npc_apparition_of_fear' WHERE entry = 64368;
UPDATE creature_template SET ScriptName = 'npc_apparition_of_terror' WHERE entry = 66100;
UPDATE creature_template SET
minlevel = 90,
maxlevel = 90,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_night_terrors',
modelid1 = 11686,
modelid2 = 0
WHERE entry = 64390;
UPDATE creature_template SET
minlevel = 90,
maxlevel = 90,
faction_A = 14,
faction_H = 14,
Health_mod = 9,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 4,
ScriptName = 'mob_night_terror_summon'
WHERE entry = 64393;
UPDATE creature_template SET minlevel = 92, maxlevel = 92, `Health_mod` = 2.7, faction_A = 14, faction_H = 14, ScriptName = 'mob_minion_of_fear' WHERE entry = 60885;
UPDATE creature_template SET ScriptName = 'mob_minion_of_fear_controller' WHERE entry = 60957;

DELETE FROM spell_script_names WHERE spell_id IN
(117955, 118064, 118040, 118053, 118054, 118055, 118077, 118004, 118005, 118007, 118008, 117988, 125736, 125745, 111850, 118191, 117905, 117283);
INSERT INTO spell_script_names VALUES
(117955, 'spell_expelled_corruption'),
(118064, 'spell_lightning_storm_aura'),
(118040, 'spell_lightning_storm_aura'),
(118053, 'spell_lightning_storm_aura'),
(118054, 'spell_lightning_storm_aura'),
(118055, 'spell_lightning_storm_aura'),
(118077, 'spell_lightning_storm_aura'),
(118004, 'spell_lightning_storm_damage'),
(118005, 'spell_lightning_storm_damage'),
(118007, 'spell_lightning_storm_damage'),
(118008, 'spell_lightning_storm_damage'),
(117988, 'spell_defiled_ground_damage'),
(125736, 'spell_night_terrors_missile'),
(125745, 'spell_night_terrors_periodic'),
(111850, 'spell_lightning_prison'),
(118191, 'spell_corrupted_essence'),
(117905, 'spell_superior_corrupted_essence'),
(117283, 'spell_cleansing_waters_regen');

DELETE FROM spell_target_position WHERE id = 117227;
INSERT INTO spell_target_position VALUE (117227, 0, 996, -1017.82, -3049.114, 12.8233, 4.675);

DELETE FROM game_graveyard_zone WHERE id = 4145;
INSERT INTO game_graveyard_zone VALUE (4145, 6067, 0);

DELETE FROM creature_text WHERE entry IN (60583, 60585, 60586);
INSERT INTO creature_text VALUES
(60583, 0, 0, 'The terrace was not meant for your kind.', 14, 0, 100, 0, 0, 28369, 'KAOLAN_INTRO'),
(60586, 1, 0, 'The endless waters will cleanse you from this place.', 14, 0, 100, 0, 0, 0, 'ASANI_AGGRO'),
(60585, 2, 0, 'A storm approaches.', 14, 0, 100, 0, 0, 28391, 'REGAIL_AGGRO'),
(60586, 3, 0, 'The tide cannot be stopped!', 14, 0, 100, 0, 0, 28383, 'KAOLAN_DIES_FIRST_ASANI'),
(60585, 4, 0, 'I command the elements, to destroy you!', 14, 0, 100, 0, 0, 28394, 'KAOLAN_DIES_FIRST_REGAIL'),
(60583, 5, 0, 'My strength grows!', 14, 0, 100, 0, 0, 28371, 'ASANI_DIES_FIRST_KAOLAN'),
(60585, 6, 0, 'I command the elements, to destroy you!', 14, 0, 100, 0, 0, 28394, 'KAOLAN_DIES_FIRST_REGAIL'),
(60586, 7, 0, 'The waters will consume you!', 14, 0, 100, 0, 0, 28382, 'CORRUPTED_WATERS'),
(60585, 8, 0, 'You cannot escape the storm!', 14, 0, 100, 0, 0, 28395, 'LIGHTNING_STORM'),
(60583, 9, 0, 'None can match my strength!', 14, 0, 100, 0, 0, 28373, 'EXPEL_CORRUPTION'),
(60586, 10, 0, 'Drown in your fear!', 14, 0, 100, 0, 0, 28384, 'REGAIL_DIES_SECOND_ASANI_01'),
(60586, 10, 1, 'Drown in fear!', 14, 0, 100, 0, 0, 28487, 'REGAIL_DIES_SECOND_ASANI_02'),
(60583, 11, 0, 'I will not be controlled!', 14, 0, 100, 0, 0, 28372, 'REGAIL_DIES_SECOND_KAOLAN'),
(60585, 12, 0, 'Your fear will consume you!', 14, 0, 100, 0, 0, 28393, 'ASANI_DIES_SECOND_REGAIL'),
(60583, 13, 0, 'I Will not be controlled!', 14, 0, 100, 0, 0, 28372, 'ASANI_DIES_SECOND_KAOLAN'),
(60586, 14, 0, 'You are unfit to stand bedore me!', 14, 0, 100, 0, 0, 28385, 'ASANI_SLAY_01'),
(60586, 14, 1, 'Flee, child!', 14, 0, 100, 0, 0, 28386, 'ASANI_SLAY_02'),
(60585, 15, 0, 'How dare you trespass here!', 14, 0, 100, 0, 0, 28396, 'REGAIL_SLAY_01'),
(60585, 15, 1, 'Shocking.', 14, 0, 100, 0, 0, 28397, 'REGAIL_SLAY_02'),
(60583, 16, 0, 'Weakling!', 14, 0, 100, 0, 0, 28374, 'KAOLAN_SLAY_01'),
(60583, 16, 1, 'Begone!', 14, 0, 100, 0, 0, 28375, 'KAOLAN_SLAY_02'),
(60586, 17, 0, 'The Sha...must be...stopped.', 14, 0, 100, 0, 0, 28381, 'ASANI_DEATH'),
(60585, 18, 0, 'The terrace... must not... fall...', 14, 0, 100, 0, 0, 28392, 'REGAIL_DEATH'),
(60583, 19, 0, 'We...are endless...', 14, 0, 100, 0, 0, 28370, 'KAOLAN_DEATH');

DELETE FROM locales_creature_text WHERE entry IN (60583, 60585, 60586);
INSERT INTO locales_creature_text VALUES
(60583, 0, 0, '', 'La terrasse n''est pas faite pour vous.', '', '', '', '', '', '', '', ''),
(60586, 1, 0, '', 'Les eaux éternelles vont vous purifier de ce lieu.', '', '', '', '', '', '', '', ''),
(60585, 2, 0, '', 'Une tempête se rapproche.', '', '', '', '', '', '', '', ''),
(60586, 3, 0, '', 'Rien n''arrêtera la marée !', '', '', '', '', '', '', '', ''),
(60585, 4, 0, '', 'J''ordonne aux éléments de vous détruire !', '', '', '', '', '', '', '', ''),
(60583, 5, 0, '', 'Ma force grandit !', '', '', '', '', '', '', '', ''),
(60585, 6, 0, '', 'J''ordonne aux éléments de vous détruire !', '', '', '', '', '', '', '', ''),
(60586, 7, 0, '', 'Les eaux vont vous engloutir !', '', '', '', '', '', '', '', ''),
(60585, 8, 0, '', 'Vous n''échapperez pas à la tempête !', '', '', '', '', '', '', '', ''),
(60583, 9, 0, '', 'Nul ne peut égaler ma force !', '', '', '', '', '', '', '', ''),
(60586, 10, 0, '', 'Noyez-vous dans vos peurs !', '', '', '', '', '', '', '', ''),
(60586, 10, 1, '', 'Noyez-vous dans vos peurs !', '', '', '', '', '', '', '', ''),
(60583, 11, 0, '', 'Nul ne me contrôlera !', '', '', '', '', '', '', '', ''),
(60585, 12, 0, '', 'Votre peur va vous dévorer !', '', '', '', '', '', '', '', ''),
(60583, 13, 0, '', 'Nul ne me contrôlera !', '', '', '', '', '', '', '', ''),
(60586, 14, 0, '', 'Vous êtes incapables de vous tenir devant moi !', '', '', '', '', '', '', '', ''),
(60586, 14, 1, '', 'Fuyez, enfants !', '', '', '', '', '', '', '', ''),
(60585, 15, 0, '', 'Comment osez-vous trépasser ici !', '', '', '', '', '', '', '', ''),
(60585, 15, 1, '', 'Choquant.', '', '', '', '', '', '', '', ''),
(60583, 16, 0, '', 'Faible !', '', '', '', '', '', '', '', ''),
(60583, 16, 1, '', 'Partez !', '', '', '', '', '', '', '', ''),
(60586, 17, 0, '', 'Les sha... il faut... les arrêter...', '', '', '', '', '', '', '', ''),
(60585, 18, 0, '', 'La terrasse... ne doit pas... tomber...', '', '', '', '', '', '', '', ''),
(60583, 19, 0, '', 'Nous... sommes... sans fin...', '', '', '', '', '', '', '', '');

UPDATE creature_template SET
IconName = 'Speak',
npcflag = npcflag|0x1,
gossip_menu_id = 60009,
minlevel = 90,
maxlevel = 90,
ScriptName = 'npc_springtender_ashani'
WHERE entry = 64846;

DELETE FROM gossip_menu WHERE entry IN (60009, 60010, 60011);
INSERT INTO gossip_menu VALUES
(60009, 60000),
(60010, 60001),
(60011, 60002);

DELETE FROM gossip_menu_option WHERE menu_id IN (60009, 60010);
INSERT INTO gossip_menu_option VALUES
(60009, 50, 0, 'This entity feeds on fear, but we do not have fear for him. Stop your ritual, we can clean this place without your help.', 1, 1, 60010, 0, 0, 0, ''),
(60010, 51, 0, 'Yes, I understand. To win against fear, you must face it, never flinching or looking away.', 1, 1, 60011, 0, 0, 0, '');

DELETE FROM npc_text WHERE ID IN (60000, 60001, 60002);
INSERT INTO npc_text (ID, text0_1) VALUES
(60000, 'The sacred waters are soiled, but all hope is not lost. Our ancestral purifying rituals persist, even face of this strong corruption. And as and when the water is restored, those who demean lose power.'),
(60001, 'If you ritual is interrupted, we''ll cannot resume it, and the corruption will be at it strongest. Your courage is admirable, but if you fail, we are all doomed. Do you understand the situation ? Do you understand the consequences of your request ?'),
(60002, 'May the spirit be with you, brave warriors.');

DELETE FROM locales_gossip_menu_option WHERE menu_id IN (60009, 60010);
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc1, option_text_loc2) VALUES
(60009, 50, 'This entity feeds on fear, but we do not have fear for him. Stop your ritual, we can clean this place without your help.', 'Cet être se nourrit de peur, mais nous n''en avons pas pour lui. Cessez votre rituel, nous pouvons purifier cet endroit sans votre aide.'),
(60010, 51, 'Yes, I understand. To win against fear, you must face it, never flinching or looking away.', 'Oui, je comprends. Pour vaincre la Peur, il faut la regarder bien en face, sans jamais fléchir ni détourner le regard.');

DELETE FROM locales_npc_text WHERE entry IN (60000, 60001, 60002);
INSERT INTO locales_npc_text (entry, Text0_1_loc1, Text0_1_loc2) VALUES
(60000, 'The sacred waters are soiled, but all hope is not lost. Our ancestral purifying rituals persist, even face of this strong corruption. And as and when the water is restored, those who demean lose power.', 'Les eaux sacrées sont souillées, mais tout espoir n''est pas perdu. Nos rituels purificateurs ancestraux persistent, même face à cette forte corruption. Et au fur et à mesure que l''eau se restaure, ceux qui l''avilissent perdent en puissance.'),
(60001, 'If you ritual is interrupted, we''ll cannot resume it, and the corruption will be at it strongest. Your courage is admirable, but if you fail, we are all doomed. Do you understand the situation ? Do you understand the consequences of your request ?', 'Si le rituel est interrompu, nous ne pourrons pas le reprendre, et la corruption sera au plus fort. Votre courage est admirable, mais si vous échouez, nous sommes tous condamnés. Comprenez-vous la situation ? Comprenez-vous les conséquences de votre demande ?'),
(60002, 'May the spirit be with you, brave warriors.', 'Que les esprits soient avec vous, braves guerriers.');

-- Protectors of the Endless
-- Ancient Asani
UPDATE creature_template SET difficulty_entry_5 = 80586 WHERE entry = 60586;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80586','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41504','0','0','0','Elder Asani (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','10','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60586','0','0','0','0','0','0','0','0','26662','117309','117227','118186','117351','117353','117052','118312','0','0','3000000','3100000','','0','3','1','189','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Ancient Regail
UPDATE creature_template SET difficulty_entry_5 = 80585 WHERE entry = 60585;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80585','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41502','0','0','0','Elder Regail (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','10','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60585','0','0','0','0','0','0','0','0','26662','118186','117187','111850','122874','118077','117353','117351','0','0','3200000','3300000','','0','3','1','189','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Protector Kaolan
UPDATE creature_template SET difficulty_entry_5 = 80583 WHERE entry = 60583;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80583','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41503','0','0','0','Protector Kaolan (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','18','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60583','0','0','0','0','0','0','0','0','26662','117986','117975','118186','117052','117519','0','0','0','0','2900000','3000000','','0','3','1','332.1','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Corrupted Waters
UPDATE creature_template SET difficulty_entry_5 = 80621 WHERE entry = 60621;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('80621','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41538','0','0','0','Corrupted Waters (5)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','10','1024','0','0','0','0','0','0','0','0','0','0','117230','117235','0','0','0','0','0','0','0','0','0','0','','0','3','1','8.64','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','16048');

-- Trashs Mobs
-- Apparition of Fear
UPDATE creature_template SET difficulty_entry_5 = 84368 WHERE entry = 64368;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('84368','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42282','0','0','0','Apparition of Fear (5)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','6','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125736','125758','0','0','0','0','0','0','0','0','0','0','','0','3','1','31.5','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Apparition of Terror
UPDATE creature_template SET difficulty_entry_5 = 86100 WHERE entry = 66100;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('86100','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','44649','0','0','0','Apparition of Terror (5)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','6','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','130115','130120','0','0','0','0','0','0','0','0','0','0','','0','3','1','31.5','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Night Terror
UPDATE creature_template SET difficulty_entry_5 = 84393 WHERE entry = 64393;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('84393','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40625','0','0','0','Night Terror (5)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','6','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125760','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','9','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');

-- Protectors of the Endless
-- Ancient Asani
UPDATE creature_template SET difficulty_entry_4 =  81586 WHERE entry = 60586;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('81586','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41504','0','0','0','Elder Asani (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60586','0','0','0','0','0','0','0','0','26662','117309','117227','118186','117351','117353','117052','118312','0','0','3000000','3100000','','0','3','1','374.22','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Ancient Regail
UPDATE creature_template SET difficulty_entry_4 = 81585 WHERE entry = 60585;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('81585','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41502','0','0','0','Elder Regail (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60585','0','0','0','0','0','0','0','0','26662','118186','117187','111850','122874','118077','117353','117351','0','0','3200000','3300000','','0','3','1','374.22','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Protector Kaolan
UPDATE creature_template SET difficulty_entry_4 = 81583 WHERE entry = 60583;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('81583','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41503','0','0','0','Protector Kaolan (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','20','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60583','0','0','0','0','0','0','0','0','26662','117986','117975','118186','117052','117519','0','0','0','0','2900000','3000000','','0','3','1','576.18','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Corrupted Waters
UPDATE creature_template SET difficulty_entry_4 = 81621 WHERE entry = 60621;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('81621','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41538','0','0','0','Corrupted Waters (4)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','10','1024','0','0','0','0','0','0','0','0','0','0','117230','117235','0','0','0','0','0','0','0','0','0','0','','0','3','1','16.5','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','16048');

-- Trashs Mobs
-- Apparition of Fear
UPDATE creature_template SET difficulty_entry_4 = 85368 WHERE entry = 64368;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('85368','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42282','0','0','0','Apparition of Fear (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','8','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125736','125758','0','0','0','0','0','0','0','0','0','0','','0','3','1','105','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Apparition of Terror
UPDATE creature_template SET difficulty_entry_4 = 87100 WHERE entry = 66100;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('87100','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','44649','0','0','0','Apparition of Terror (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','8','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','130115','130120','0','0','0','0','0','0','0','0','0','0','','0','3','1','105','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Night Terror
UPDATE creature_template SET difficulty_entry_4 = 85393 WHERE entry = 64393;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('85393','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40625','0','0','0','Night Terror (4)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','6','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125760','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','30','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');

-- Protectors of the Endless
-- Ancient Asani
UPDATE creature_template SET difficulty_entry_6 =  82586 WHERE entry = 60586;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('82586','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41504','0','0','0','Elder Asani (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60586','0','0','0','0','0','0','0','0','26662','117309','117227','118186','117351','117353','117052','118312','0','0','3000000','3100000','','0','3','1','567','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Ancient Regail
UPDATE creature_template SET difficulty_entry_6 = 82585 WHERE entry = 60585;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('82585','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41502','0','0','0','Elder Regail (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60585','0','0','0','0','0','0','0','0','26662','118186','117187','111850','122874','118077','117353','117351','0','0','3200000','3300000','','0','3','1','567','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Protector Kaolan
UPDATE creature_template SET difficulty_entry_6 = 82583 WHERE entry = 60583;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('82583','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41503','0','0','0','Protector Kaolan (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','22','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60583','0','0','0','0','0','0','0','0','26662','117986','117975','118186','117052','117519','0','0','0','0','2900000','3000000','','0','3','1','997.2','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Corrupted Waters
UPDATE creature_template SET difficulty_entry_6 = 82621 WHERE entry = 60621;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('82621','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41538','0','0','0','Corrupted Waters (6)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','10','1024','0','0','0','0','0','0','0','0','0','0','117230','117235','0','0','0','0','0','0','0','0','0','0','','0','3','1','26.4','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','16048');
-- Minion of Fear
UPDATE creature_template SET difficulty_entry_6 = 82885 WHERE entry = 60885;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('82885','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40687','0','0','0','Minion of Fear','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','118191','117905','0','0','0','0','0','0','0','0','0','0','','0','3','1','8.1','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');

-- Trashs Mobs
-- Apparition of Fear
UPDATE creature_template SET difficulty_entry_6 = 86368 WHERE entry = 64368;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('86368','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42282','0','0','0','Apparition of Fear (6)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','10','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125736','125758','0','0','0','0','0','0','0','0','0','0','','0','3','1','105','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Apparition of Terror
UPDATE creature_template SET difficulty_entry_6 = 88100 WHERE entry = 66100;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('88100','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','44649','0','0','0','Apparition of Terror (6)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','10','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','130115','130120','0','0','0','0','0','0','0','0','0','0','','0','3','1','105','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Night Terror
UPDATE creature_template SET difficulty_entry_6 = 86393 WHERE entry = 64393;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('86393','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40625','0','0','0','Night Terror (6)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','8','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125760','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','30','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');

-- Protectors of the Endless
-- Ancient Asani
UPDATE creature_template SET difficulty_entry_7 = 83586 WHERE entry = 60586;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('83586','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41504','0','0','0','Elder Asani (7)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','11','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60586','0','0','0','0','0','0','0','0','26662','117309','117227','118186','117351','117353','117052','118312','0','0','3000000','3100000','','0','3','1','235','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Ancient Regail
UPDATE creature_template SET difficulty_entry_7 = 83585 WHERE entry = 60585;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('83585','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41502','0','0','0','Elder Regail (7)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','11','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60585','0','0','0','0','0','0','0','0','26662','118186','117187','111850','122874','118077','117353','117351','0','0','3200000','3300000','','0','3','1','235','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Protector Kaolan
UPDATE creature_template SET difficulty_entry_7 = 83583 WHERE entry = 60583;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('83583','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41503','0','0','0','Protector Kaolan (7)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','19','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','60583','0','0','0','0','0','0','0','0','26662','117986','117975','118186','117052','117519','0','0','0','0','2900000','3000000','','0','3','1','360','1','1','1','0','0','0','0','0','0','0','157','1','0','769638399','0','','16048');
-- Corrupted Waters
UPDATE creature_template SET difficulty_entry_7 = 83621 WHERE entry = 60621;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('83621','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41538','0','0','0','Corrupted Waters (7)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','10','1024','0','0','0','0','0','0','0','0','0','0','117230','117235','0','0','0','0','0','0','0','0','0','0','','0','3','1','5','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','16048');

-- Trashs Mobs
-- Apparition of Fear
UPDATE creature_template SET difficulty_entry_7 = 87368 WHERE entry = 64368;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('87368','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42282','0','0','0','Apparition of Fear (7)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','7','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125736','125758','0','0','0','0','0','0','0','0','0','0','','0','3','1','79','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Apparition of Terror
UPDATE creature_template SET difficulty_entry_7 = 89100 WHERE entry = 66100;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('89100','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','44649','0','0','0','Apparition of Terror (7)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','7','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','130115','130120','0','0','0','0','0','0','0','0','0','0','','0','3','1','79','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');
-- Night Terror
UPDATE creature_template SET difficulty_entry_7 = 87393 WHERE entry = 64393;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('87393','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40625','0','0','0','Night Terror (7)','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','7','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','125760','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','22','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','16048');

-- Reference parchemin

SET @REF_PARCHO = 100000;
DELETE FROM `reference_loot_template` WHERE entry = @REF_PARCHO;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_PARCHO, 86281, 0, 1, 0, 1, 1) /* 3Pattern: Nightfire Robe */,
(@REF_PARCHO, 86380, 0, 1, 0, 1, 1) /* 3Pattern: Imperial Silk Gloves */,
(@REF_PARCHO, 86284, 0, 1, 0, 1, 1) /* 3Pattern: Raven Lord's Gloves */,
(@REF_PARCHO, 87408, 0, 1, 0, 1, 1) /* 3Plans: Unyielding Bloodplate */,
(@REF_PARCHO, 86280, 0, 1, 0, 1, 1) /* 3Pattern: Murderer's Gloves */,
(@REF_PARCHO, 86381, 0, 1, 0, 1, 1) /* 3Pattern: Legacy of the Emperor */,
(@REF_PARCHO, 87412, 0, 1, 0, 1, 1) /* 3Plans: Chestplate of Limitless Faith */,
(@REF_PARCHO, 87411, 0, 1, 0, 1, 1) /* 3Plans: Bloodforged Warfists */;


/**********************************************************************
***                  PROTECTORS OF THE ENDLESS                      ***
***********************************************************************/

-- REF NM

-- ANCIENT ASANI
SET @REF_ASANI_1 = 60586;
DELETE FROM `reference_loot_template` WHERE entry = @REF_ASANI_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_ASANI_1, 86231, 0, 1, 1, 1, 1) /* 3Regail's Band of the Endless */,
(@REF_ASANI_1, 86390, 0, 1, 1, 1, 1) /* 3Regail's Crackling Dagger */,
(@REF_ASANI_1, 86315, 0, 1, 1, 1, 1) /* 3Watersoul Signet */,
(@REF_ASANI_1, 86316, 0, 1, 1, 1, 1) /* 3Cloak of Overwhelming Corruption */,
(@REF_ASANI_1, 86233, 0, 1, 1, 1, 1) /* 3Shackle of Eversparks */,
(@REF_ASANI_1, 86234, 0, 1, 1, 1, 1) /* 3Kaolan's Withering Necklace */,
(@REF_ASANI_1, 86232, 0, 1, 1, 1, 1) /* 3Deepwater Greatboots */,
(@REF_ASANI_1, 86317, 0, 1, 1, 1, 1) /* 3Cuffs of the Corrupted Waters */,
(@REF_ASANI_1, 86320, 0, 1, 1, 1, 1) /* 3Asani's Uncleansed Sandals */,
(@REF_ASANI_1, 86230, 0, 1, 1, 1, 1) /* 3Bracers of Defiled Earth */,
(@REF_ASANI_1, 86318, 0, 1, 1, 1, 1) /* 3Casque of Expelled Corruptione */,
(@REF_ASANI_1, 89885, 0, 1, 1, 1, 1) /* 3Waterborne Shoulderguards */,
(@REF_ASANI_1, 89841, 0, 1, 1, 1, 1) /* 3Legguards of Failing Purification */,
(@REF_ASANI_1, 86319, 0, 1, 1, 1, 1) /* 3Lightning Prisoner's Boots */;

-- ANCIENT REGAIL
SET @REF_REGAIL_1 = 60585;
DELETE FROM `reference_loot_template` WHERE entry = @REF_REGAIL_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_REGAIL_1, 86231, 0, 1, 1, 1, 1) /* 3Regail's Band of the Endless */,
(@REF_REGAIL_1, 86390, 0, 1, 1, 1, 1) /* 3Regail's Crackling Dagger */,
(@REF_REGAIL_1, 86315, 0, 1, 1, 1, 1) /* 3Watersoul Signet */,
(@REF_REGAIL_1, 86316, 0, 1, 1, 1, 1) /* 3Cloak of Overwhelming Corruption */,
(@REF_REGAIL_1, 86233, 0, 1, 1, 1, 1) /* 3Shackle of Eversparks */,
(@REF_REGAIL_1, 86234, 0, 1, 1, 1, 1) /* 3Kaolan's Withering Necklace */,
(@REF_REGAIL_1, 86232, 0, 1, 1, 1, 1) /* 3Deepwater Greatboots */,
(@REF_REGAIL_1, 86317, 0, 1, 1, 1, 1) /* 3Cuffs of the Corrupted Waters */,
(@REF_REGAIL_1, 86320, 0, 1, 1, 1, 1) /* 3Asani's Uncleansed Sandals */,
(@REF_REGAIL_1, 86230, 0, 1, 1, 1, 1) /* 3Bracers of Defiled Earth */,
(@REF_REGAIL_1, 86318, 0, 1, 1, 1, 1) /* 3Casque of Expelled Corruptione */,
(@REF_REGAIL_1, 89885, 0, 1, 1, 1, 1) /* 3Waterborne Shoulderguards */,
(@REF_REGAIL_1, 89841, 0, 1, 1, 1, 1) /* 3Legguards of Failing Purification */,
(@REF_REGAIL_1, 86319, 0, 1, 1, 1, 1) /* 3Lightning Prisoner's Boots */;

-- PROTECTOR KAOLAN
SET @REF_KAOLAN_1 = 60583;
DELETE FROM `reference_loot_template` WHERE entry = @REF_KAOLAN_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAOLAN_1, 86231, 0, 1, 1, 1, 1) /* 3Regail's Band of the Endless */,
(@REF_KAOLAN_1, 86390, 0, 1, 1, 1, 1) /* 3Regail's Crackling Dagger */,
(@REF_KAOLAN_1, 86315, 0, 1, 1, 1, 1) /* 3Watersoul Signet */,
(@REF_KAOLAN_1, 86316, 0, 1, 1, 1, 1) /* 3Cloak of Overwhelming Corruption */,
(@REF_KAOLAN_1, 86233, 0, 1, 1, 1, 1) /* 3Shackle of Eversparks */,
(@REF_KAOLAN_1, 86234, 0, 1, 1, 1, 1) /* 3Kaolan's Withering Necklace */,
(@REF_KAOLAN_1, 86232, 0, 1, 1, 1, 1) /* 3Deepwater Greatboots */,
(@REF_KAOLAN_1, 86317, 0, 1, 1, 1, 1) /* 3Cuffs of the Corrupted Waters */,
(@REF_KAOLAN_1, 86320, 0, 1, 1, 1, 1) /* 3Asani's Uncleansed Sandals */,
(@REF_KAOLAN_1, 86230, 0, 1, 1, 1, 1) /* 3Bracers of Defiled Earth */,
(@REF_KAOLAN_1, 86318, 0, 1, 1, 1, 1) /* 3Casque of Expelled Corruptione */,
(@REF_KAOLAN_1, 89885, 0, 1, 1, 1, 1) /* 3Waterborne Shoulderguards */,
(@REF_KAOLAN_1, 89841, 0, 1, 1, 1, 1) /* 3Legguards of Failing Purification */,
(@REF_KAOLAN_1, 86319, 0, 1, 1, 1, 1) /* 3Lightning Prisoner's Boots */;

-- MODE : 10_NM (60586-60585-60583)

SET @10_NM_ENTRY_ASANI = 60586;
UPDATE `creature_template` SET `lootid`= @10_NM_ENTRY_ASANI WHERE `entry`= @10_NM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_NM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@10_NM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_1, 2) /* REF 1*/,
(@10_NM_ENTRY_ASANI, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @10_NM_ENTRY_REGAIL = 60585;
UPDATE `creature_template` SET `lootid`= @10_NM_ENTRY_REGAIL WHERE `entry`= @10_NM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_NM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@10_NM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_1, 2) /* REF 1*/,
(@10_NM_ENTRY_REGAIL, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @10_NM_ENTRY_KAOLAN = 60583;
UPDATE `creature_template` SET `lootid`= @10_NM_ENTRY_KAOLAN WHERE `entry`= @10_NM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_NM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@10_NM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_1, 2) /* REF 1*/,
(@10_NM_ENTRY_KAOLAN, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

-- MODE : 25_NM (60586-60585-60583)

SET @25_NM_ENTRY_ASANI = 81586;
UPDATE `creature_template` SET `lootid`= @25_NM_ENTRY_ASANI WHERE `entry`= @25_NM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_NM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@25_NM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_1, 4) /* REF 1*/,
(@25_NM_ENTRY_ASANI, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @25_NM_ENTRY_REGAIL = 81585;
UPDATE `creature_template` SET `lootid`= @25_NM_ENTRY_REGAIL WHERE `entry`= @25_NM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_NM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@25_NM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_1, 4) /* REF 1*/,
(@25_NM_ENTRY_REGAIL, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @25_NM_ENTRY_KAOLAN = 81583;
UPDATE `creature_template` SET `lootid`= @25_NM_ENTRY_KAOLAN WHERE `entry`= @25_NM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_NM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@25_NM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_1, 4) /* REF 1*/,
(@25_NM_ENTRY_KAOLAN, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;



-- Ref HM 

SET @REF_ASANI_2 = 82586;
DELETE FROM `reference_loot_template` WHERE entry = @REF_ASANI_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_ASANI_2, 87150, 0, 1, 1, 1, 1) /* 3Cloak of Overwhelming Corruption */,
(@REF_ASANI_2, 87144, 0, 1, 1, 1, 1) /* 3Regail's Band of the Endless */,
(@REF_ASANI_2, 87152, 0, 1, 1, 1, 1) /* 3Regail's Crackling Dagger */,
(@REF_ASANI_2, 87147, 0, 1, 1, 1, 1) /* 3Shackle of Eversparks */,
(@REF_ASANI_2, 87151, 0, 1, 1, 1, 1) /* 3Watersoul Signet */,
(@REF_ASANI_2, 87148, 0, 1, 1, 1, 1) /* 3Kaolan's Withering Necklace */,
(@REF_ASANI_2, 89944, 0, 1, 1, 1, 1) /* 3Waterborne Shoulderguards */,
(@REF_ASANI_2, 87145, 0, 1, 1, 1, 1) /* 3Bracers of Defiled Earth */,
(@REF_ASANI_2, 87149, 0, 1, 1, 1, 1) /* 3Cuffs of the Corrupted Waters */,
(@REF_ASANI_2, 87155, 0, 1, 1, 1, 1) /* 3Casque of Expelled Corruption */,
(@REF_ASANI_2, 87146, 0, 1, 1, 1, 1) /* 3Deepwater Greatboots */,
(@REF_ASANI_2, 87153, 0, 1, 1, 1, 1) /* 3Asani's Uncleansed Sandals */,
(@REF_ASANI_2, 87154, 0, 1, 1, 1, 1) /* 3Lightning Prisoner's Boots */,
(@REF_ASANI_2, 89943, 0, 1, 1, 1, 1) /* 3Legguards of Failing Purification */;

SET @REF_REGAIL_2 = 82585;
DELETE FROM `reference_loot_template` WHERE entry = @REF_REGAIL_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_REGAIL_2, 87150, 0, 1, 1, 1, 1) /* 3Cloak of Overwhelming Corruption */,
(@REF_REGAIL_2, 87144, 0, 1, 1, 1, 1) /* 3Regail's Band of the Endless */,
(@REF_REGAIL_2, 87152, 0, 1, 1, 1, 1) /* 3Regail's Crackling Dagger */,
(@REF_REGAIL_2, 87147, 0, 1, 1, 1, 1) /* 3Shackle of Eversparks */,
(@REF_REGAIL_2, 87151, 0, 1, 1, 1, 1) /* 3Watersoul Signet */,
(@REF_REGAIL_2, 87148, 0, 1, 1, 1, 1) /* 3Kaolan's Withering Necklace */,
(@REF_REGAIL_2, 89944, 0, 1, 1, 1, 1) /* 3Waterborne Shoulderguards */,
(@REF_REGAIL_2, 87145, 0, 1, 1, 1, 1) /* 3Bracers of Defiled Earth */,
(@REF_REGAIL_2, 87149, 0, 1, 1, 1, 1) /* 3Cuffs of the Corrupted Waters */,
(@REF_REGAIL_2, 87155, 0, 1, 1, 1, 1) /* 3Casque of Expelled Corruption */,
(@REF_REGAIL_2, 87146, 0, 1, 1, 1, 1) /* 3Deepwater Greatboots */,
(@REF_REGAIL_2, 87153, 0, 1, 1, 1, 1) /* 3Asani's Uncleansed Sandals */,
(@REF_REGAIL_2, 87154, 0, 1, 1, 1, 1) /* 3Lightning Prisoner's Boots */,
(@REF_REGAIL_2, 89943, 0, 1, 1, 1, 1) /* 3Legguards of Failing Purification */;

SET @REF_KAOLAN_2 = 82583;
DELETE FROM `reference_loot_template` WHERE entry = @REF_KAOLAN_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAOLAN_2, 87150, 0, 1, 1, 1, 1) /* 3Cloak of Overwhelming Corruption */,
(@REF_KAOLAN_2, 87144, 0, 1, 1, 1, 1) /* 3Regail's Band of the Endless */,
(@REF_KAOLAN_2, 87152, 0, 1, 1, 1, 1) /* 3Regail's Crackling Dagger */,
(@REF_KAOLAN_2, 87147, 0, 1, 1, 1, 1) /* 3Shackle of Eversparks */,
(@REF_KAOLAN_2, 87151, 0, 1, 1, 1, 1) /* 3Watersoul Signet */,
(@REF_KAOLAN_2, 87148, 0, 1, 1, 1, 1) /* 3Kaolan's Withering Necklace */,
(@REF_KAOLAN_2, 89944, 0, 1, 1, 1, 1) /* 3Waterborne Shoulderguards */,
(@REF_KAOLAN_2, 87145, 0, 1, 1, 1, 1) /* 3Bracers of Defiled Earth */,
(@REF_KAOLAN_2, 87149, 0, 1, 1, 1, 1) /* 3Cuffs of the Corrupted Waters */,
(@REF_KAOLAN_2, 87155, 0, 1, 1, 1, 1) /* 3Casque of Expelled Corruption */,
(@REF_KAOLAN_2, 87146, 0, 1, 1, 1, 1) /* 3Deepwater Greatboots */,
(@REF_KAOLAN_2, 87153, 0, 1, 1, 1, 1) /* 3Asani's Uncleansed Sandals */,
(@REF_KAOLAN_2, 87154, 0, 1, 1, 1, 1) /* 3Lightning Prisoner's Boots */,
(@REF_KAOLAN_2, 89943, 0, 1, 1, 1, 1) /* 3Legguards of Failing Purification */;

-- MODE : 10_HM (60586-60585-60583)

SET @10_HM_ENTRY_ASANI = 80586;
UPDATE `creature_template` SET `lootid`= @10_HM_ENTRY_ASANI WHERE `entry`= @10_HM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_HM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@10_HM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_2, 2) /* REF 2*/;

SET @10_HM_ENTRY_REGAIL = 80585;
UPDATE `creature_template` SET `lootid`= @10_HM_ENTRY_REGAIL WHERE `entry`= @10_HM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_HM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@10_HM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_2, 2) /* REF 2*/;

SET @10_HM_ENTRY_KAOLAN = 80583;
UPDATE `creature_template` SET `lootid`= @10_HM_ENTRY_KAOLAN WHERE `entry`= @10_HM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_HM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@10_HM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_2, 2) /* REF 2*/;

-- MODE : 25_HM (60586-60585-60583)

SET @25_HM_ENTRY_ASANI = 82586;
UPDATE `creature_template` SET `lootid`= @25_HM_ENTRY_ASANI WHERE `entry`= @25_HM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_HM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@25_HM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_2, 4) /* REF 2*/;

SET @25_HM_ENTRY_REGAIL = 82585;
UPDATE `creature_template` SET `lootid`= @25_HM_ENTRY_REGAIL WHERE `entry`= @25_HM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_HM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@25_HM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_2, 4) /* REF 2*/;

SET @25_HM_ENTRY_KAOLAN = 82583;
UPDATE `creature_template` SET `lootid`= @25_HM_ENTRY_KAOLAN WHERE `entry`= @25_HM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_HM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* 2Sigil of Power */,
(@25_HM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_2, 4) /* REF 2*/;

-- CURRENCY

DELETE FROM creature_loot_currency where creature_id IN
(@10_NM_ENTRY_ASANI,@10_NM_ENTRY_REGAIL,@10_NM_ENTRY_KAOLAN,@25_NM_ENTRY_ASANI,@25_NM_ENTRY_REGAIL,@25_NM_ENTRY_KAOLAN,
@10_HM_ENTRY_ASANI,@10_HM_ENTRY_REGAIL,@10_HM_ENTRY_KAOLAN,@25_HM_ENTRY_ASANI,@25_HM_ENTRY_REGAIL,@25_HM_ENTRY_KAOLAN);
REPLACE INTO creature_loot_currency (`creature_id`, `CurrencyId1`, `CurrencyCount1`) VALUES
(@10_NM_ENTRY_ASANI, 396, 4000),
(@10_NM_ENTRY_REGAIL, 396, 4000),
(@10_NM_ENTRY_KAOLAN, 396, 4000),
(@25_NM_ENTRY_ASANI, 396, 4000),
(@25_NM_ENTRY_REGAIL, 396, 4000),
(@25_NM_ENTRY_KAOLAN, 396, 4000),
(@10_HM_ENTRY_ASANI, 396, 4000),
(@10_HM_ENTRY_REGAIL, 396, 4000),
(@10_HM_ENTRY_KAOLAN, 396, 4000),
(@25_HM_ENTRY_ASANI, 396, 4000),
(@25_HM_ENTRY_REGAIL, 396, 4000),
(@25_HM_ENTRY_KAOLAN, 396, 4000);

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93, 
rank = 3,
ScriptName = 'boss_lei_shi',
mechanic_immune_mask = 769638399,
flags_extra = 0x01,
faction_A = 14,
faction_H = 14
WHERE entry = 62983;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
ScriptName = 'mob_animated_protector',
faction_A = 14,
faction_H = 14,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 4,
baseattacktime = 2000
WHERE entry = 62995;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93, 
rank = 3,
ScriptName = 'mob_lei_shi_hidden',
mechanic_immune_mask = 769638399,
flags_extra = 0x01,
faction_A = 14,
faction_H = 14
WHERE entry = 63099;

DELETE FROM `creature` WHERE `id` = 62995 AND `map` = 996;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(62995, 996, 760, 1, -989.3629, -2902.622, 19.17827, 0.6423108, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1011.262, -2888.345, 19.70614, 4.302896, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1012.432, -2941.378, 19.43974, 3.005274, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1008.399, -2900.63, 19.70614, 4.643414, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1045.229, -2925.26, 19.17827, 1.960813, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -994.75, -2925.177, 19.17827, 4.801074, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1023.323, -2936.851, 19.43974, 3.936672, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1013.615, -2925.484, 19.70614, 1.112243, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1042.057, -2888.434, 19.17827, 1.974275, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1028.252, -2923.663, 19.70614, 0, 7200, 0, 0), -- 62995 (Area: -1) 
(62995, 996, 760, 1, -1044.986, -2901.198, 19.17827, 3.013757, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1038.372, -2934.913, 19.17827, 2.312799, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -992.842, -2888.924, 19.17827, 2.489937, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -998.684, -2934.783, 19.17827, 5.882029, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1023.389, -2888.509, 19.70614, 0.8620354, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1035.009, -2910.316, 19.70614, 3.573017, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1002.585, -2913.233, 19.70614, 4.014608, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1027.068, -2899.241, 19.70614, 1.261874, 7200, 0, 0); -- 62995 (Area: -1)

DELETE FROM spell_target_position WHERE id = 123441;
INSERT INTO spell_target_position VALUE
(123441, 0, 996, -1017.93, -2911.3, 19.902, 4.74);

DELETE FROM spell_script_names WHERE spell_id IN (123461, 123244, 123233, 123705, 123712);
INSERT INTO spell_script_names VALUES
(123461, 'spell_get_away'),
(123244, 'spell_hide'),
(123233, 'spell_hide_stacks'),
(123705, 'spell_scary_fog_dot'),
(123712, 'spell_scary_fog_stacks');

UPDATE gameobject SET position_x = -1017.93, position_y = -2911.3, position_z = 19.902 WHERE id IN (214851, 214852);

DELETE FROM creature_text WHERE entry = 62983;
INSERT INTO creature_text VALUES
(62983, 0, 0, 'Wh-what are you doing here!? G-go away!', 14, 0, 100, 0, 0, 29328, 'LEI_SHI_AGGRO'),
(62983, 1, 0, 'I''m hiding until you leave!', 14, 0, 100, 0, 0, 29333, 'LEI_SHI_HIDE_01'),
(62983, 1, 1, 'I don''t want to see you anymore!', 14, 0, 100, 0, 0, 29334, 'LEI_SHI_HIDE_02'),
(62983, 2, 0, 'Stay away from me!', 14, 0, 100, 0, 0, 29332, 'LEI_SHI_GET_AWAY_01'),
(62983, 2, 1, 'Get AWAY!', 14, 0, 100, 0, 0, 29331, 'LEI_SHI_GET_AWAY_02'),
(62983, 3, 0, 'S-sorry!', 14, 0, 100, 0, 0, 29329, 'LEI_SHI_SLAY_01'),
(62983, 3, 1, 'It''s your fault it happened!', 14, 0, 100, 0, 0, 29330, 'LEI_SHI_SLAY_02'),
(62983, 4, 0, 'I... ah... oh! Did I...? Was I...? It was... so... cloudy.', 14, 0, 100, 0, 0, 29325, 'LEI_SHI_DEFEATED_01'),
(62983, 4, 1, 'I have to go now.', 14, 0, 100, 0, 0, 29326, 'LEI_SHI_DEFEATED_02'),
(62983, 4, 2, 'I have to make the water not cloudy anymore. Will... will you help?', 14, 0, 100, 0, 0, 29327, 'LEI_SHI_DEFEATED_03');

DELETE FROM locales_creature_text WHERE entry = 62983;
INSERT INTO locales_creature_text VALUES
(62983, 0, 0, '', 'Oh... euh... qu''est-ce que vous faites là ?! Partez !', '', '', '', '', '', '', '', ''),
(62983, 1, 0, '', 'Je me cache jusqu''à ce que vous partiez !', '', '', '', '', '', '', '', ''),
(62983, 1, 1, '', 'Je ne veux plus vous voir !', '', '', '', '', '', '', '', ''),
(62983, 2, 0, '', 'Ne vous approchez pas de moi !', '', '', '', '', '', '', '', ''),
(62983, 2, 1, '', 'Allez-vous-EN !', '', '', '', '', '', '', '', ''),
(62983, 3, 0, '', 'Oh... dé-désolée !', '', '', '', '', '', '', '', ''),
(62983, 3, 1, '', 'C''est votre faute si c''est arrivé !', '', '', '', '', '', '', '', ''),
(62983, 4, 0, '', 'Je… ah… oh ! J''ai… ? Tout était… si… embrouillé.', '', '', '', '', '', '', '', ''),
(62983, 4, 1, '', 'Je dois partir maintenant.', '', '', '', '', '', '', '', ''),
(62983, 4, 2, '', 'Je dois faire en sorte que l''eau ne soit plus trouble. Vous… vous m''aideriez ?', '', '', '', '', '', '', '', '');

-- 10 Normal
-- Lei Shi
UPDATE creature_template SET Health_mod = 285.12 WHERE entry = 62983;
-- Animated Protector
UPDATE creature_template SET Health_mod = 12.87 WHERE entry = 62995;

-- 10 Heroic
-- Lei Shi
UPDATE creature_template SET difficulty_entry_5 = 82983 WHERE entry = 62983;
INSERT INTO `creature_template` VALUES
('82983','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43783','0','0','0','Lei Shi (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097260','0','62983','0','0','0','0','0','0','0','0','123181','26662','123620','123461','123244','123250','123121','127535','0','0','0','0','','0','3','1','622.08','1','1','1','0','0','0','0','0','0','0','913','1','0','769638399','1','','16048');
-- Animated Protector
UPDATE creature_template SET difficulty_entry_5 = 82995 WHERE entry = 62995;
INSERT INTO `creature_template` VALUES
('82995','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43771','0','0','0','Animated Protector (5)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','8','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','0','123505','0','0','0','0','0','0','0','0','0','0','','0','3','1','18.252','1','1','1','0','0','0','0','0','0','0','151','1','0','0','0','','16048');

-- 25 Normal
-- Lei Shi
UPDATE creature_template SET difficulty_entry_4 = 83983 WHERE entry = 62983;
INSERT INTO `creature_template` VALUES
('83983','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43783','0','0','0','Lei Shi (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097260','0','62983','0','0','0','0','0','0','0','0','123181','26662','123620','123461','123244','123250','123121','127535','0','0','0','0','','0','3','1','855.36','1','1','1','0','0','0','0','0','0','0','913','1','0','769638399','1','','16048');
-- Animated Protector
UPDATE creature_template SET difficulty_entry_4 = 83995 WHERE entry = 62995;
INSERT INTO `creature_template` VALUES
('83995','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43771','0','0','0','Animated Protector (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','10','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','0','123505','0','0','0','0','0','0','0','0','0','0','','0','3','1','44.33','1','1','1','0','0','0','0','0','0','0','151','1','0','0','0','','16048');

-- 25 Heroic
-- Lei Shi
UPDATE creature_template SET difficulty_entry_6 = 84983 WHERE entry = 62983;
INSERT INTO `creature_template` VALUES
('84983','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43783','0','0','0','Lei Shi (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097260','0','62983','0','0','0','0','0','0','0','0','123181','26662','123620','123461','123244','123250','123121','127535','0','0','0','0','','0','3','1','1866.24','1','1','1','0','0','0','0','0','0','0','913','1','0','769638399','1','','16048');
-- Animated Protector
UPDATE creature_template SET difficulty_entry_6 = 84995 WHERE entry = 62995;
INSERT INTO `creature_template` VALUES
('84995','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43771','0','0','0','Animated Protector (6)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','0','123505','0','0','0','0','0','0','0','0','0','0','','0','3','1','56.583','1','1','1','0','0','0','0','0','0','0','151','1','0','0','0','','16048');

-- LFR
-- Lei Shi
UPDATE creature_template SET difficulty_entry_7 = 85983 WHERE entry = 62983;
INSERT INTO `creature_template` VALUES
('85983','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43783','0','0','0','Lei Shi (7)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097260','0','62983','0','0','0','0','0','0','0','0','123181','26662','123620','123461','123244','123250','123121','127535','0','0','0','0','','0','3','1','586.68','1','1','1','0','0','0','0','0','0','0','913','1','0','769638399','1','','16048');
-- Animated Protector
UPDATE creature_template SET difficulty_entry_7 = 85995 WHERE entry = 62995;
INSERT INTO `creature_template` VALUES
('85995','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','43771','0','0','0','Animated Protector (7)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','6','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','0','123505','0','0','0','0','0','0','0','0','0','0','','0','3','1','27.5','1','1','1','0','0','0','0','0','0','0','151','1','0','0','0','','16048');

-- Loots for Lei Shi
DELETE FROM gameobject_template WHERE entry IN (213073, 213074, 213076, 213075);
INSERT INTO gameobject_template VALUES
-- Normal mode (10man)
(213076, 3, 10316, 'Box of Fancy Stuff', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 213076, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Normal mode (25man)
(213074, 3, 10316, 'Box of Fancy Stuff', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 213074, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Heroic mode (10man)
(213075, 3, 10316, 'Box of Fancy Stuff', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 213075, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Heroic mode (25man)
(213073, 3, 10316, 'Box of Fancy Stuff', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 213073, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595);

-- Normal mode
SET @REF_LEI_SHI_1 = 213076;
DELETE FROM `reference_loot_template` WHERE entry = @REF_LEI_SHI_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_LEI_SHI_1, 86342, 0, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */,
(@REF_LEI_SHI_1, 86333, 0, 1, 1, 1, 1) /* 3Cuirass of the Animated Protector */,
(@REF_LEI_SHI_1, 86336, 0, 1, 1, 1, 1) /* 3Darkmist Vortex */,
(@REF_LEI_SHI_1, 86337, 0, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */,
(@REF_LEI_SHI_1, 86338, 0, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */,
(@REF_LEI_SHI_1, 86335, 0, 1, 1, 1, 1) /* 3Jin'ya, Orb of the Waterspeaker */,
(@REF_LEI_SHI_1, 86383, 0, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */,
(@REF_LEI_SHI_1, 86385, 0, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */,
(@REF_LEI_SHI_1, 86384, 0, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */,
(@REF_LEI_SHI_1, 86343, 0, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */,
(@REF_LEI_SHI_1, 86334, 0, 1, 1, 1, 1) /* 3Robes of the Unknown Fear */,
(@REF_LEI_SHI_1, 89246, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Conqueror */,
(@REF_LEI_SHI_1, 89247, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Protector */,
(@REF_LEI_SHI_1, 89248, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Vanquishe */,
(@REF_LEI_SHI_1, 86339, 0, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */,
(@REF_LEI_SHI_1, 86391, 0, 1, 1, 1, 1) /* 3Spiritsever */,
(@REF_LEI_SHI_1, 86341, 0, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */,
(@REF_LEI_SHI_1, 86331, 0, 1, 1, 1, 1) /* 3Taoren, the Soul Burner */,
(@REF_LEI_SHI_1, 86332, 0, 1, 1, 1, 1) /* 3Terror in the Mists */,
(@REF_LEI_SHI_1, 86340, 0, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */;

-- Mode 10
SET @10NM_ENTRY_LEI_SHI = 213076; 
UPDATE `gameobject_template` SET `data1`= @10NM_ENTRY_LEI_SHI WHERE `entry`= @10NM_ENTRY_LEI_SHI;
DELETE FROM `gameobject_loot_template` WHERE `entry`= @10NM_ENTRY_LEI_SHI;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10NM_ENTRY_LEI_SHI, 1, 100, 1, 0, -@REF_LEI_SHI_1, 2) /* REF 1*/;

-- Mode 25
SET @25NM_ENTRY_LEI_SHI = 213074; 
UPDATE `gameobject_template` SET `data1`= @25NM_ENTRY_LEI_SHI WHERE `entry`= @25NM_ENTRY_LEI_SHI;
DELETE FROM `gameobject_loot_template` WHERE `entry`= @25NM_ENTRY_LEI_SHI;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25NM_ENTRY_LEI_SHI, 1, 100, 1, 0, -@REF_LEI_SHI_1, 5) /* REF 1*/;

-- Heroic Mode
SET @REF_LEI_SHI_2 = 213075;
DELETE FROM `reference_loot_template` WHERE entry = @REF_LEI_SHI_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_LEI_SHI_2, 87183, 0, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */,
(@REF_LEI_SHI_2, 87171, 0, 1, 1, 1, 1) /* 3Cuirass of the Animated Protector */,
(@REF_LEI_SHI_2, 87172, 0, 1, 1, 1, 1) /* 3Darkmist Vortex */,
(@REF_LEI_SHI_2, 87178, 0, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */,
(@REF_LEI_SHI_2, 87177, 0, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */,
(@REF_LEI_SHI_2, 87170, 0, 1, 1, 1, 1) /* 3Jin'ya, Orb of the Waterspeaker */,
(@REF_LEI_SHI_2, 87184, 0, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */,
(@REF_LEI_SHI_2, 87186, 0, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */,
(@REF_LEI_SHI_2, 87185, 0, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */,
(@REF_LEI_SHI_2, 87182, 0, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */,
(@REF_LEI_SHI_2, 87169, 0, 1, 1, 1, 1) /* 3Robes of the Unknown Fear */,
(@REF_LEI_SHI_2, 89262, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Conqueror */,
(@REF_LEI_SHI_2, 89263, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Protector */,
(@REF_LEI_SHI_2, 89261, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Vanquishe */,
(@REF_LEI_SHI_2, 87179, 0, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */,
(@REF_LEI_SHI_2, 87166, 0, 1, 1, 1, 1) /* 3Spiritsever */,
(@REF_LEI_SHI_2, 87180, 0, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */,
(@REF_LEI_SHI_2, 87168, 0, 1, 1, 1, 1) /* 3Taoren, the Soul Burner */,
(@REF_LEI_SHI_2, 87167, 0, 1, 1, 1, 1) /* 3Terror in the Mists */,
(@REF_LEI_SHI_2, 87181, 0, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */;

-- Mode 10
SET @10HM_ENTRY_LEI_SHI = 213075;
UPDATE `gameobject_template` SET `data1` = @10HM_ENTRY_LEI_SHI WHERE `entry` = @10HM_ENTRY_LEI_SHI;
DELETE FROM `gameobject_loot_template` WHERE `entry`= @10HM_ENTRY_LEI_SHI;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10HM_ENTRY_LEI_SHI, 1, 100, 1, 0, -@REF_LEI_SHI_2, 2) /* REF 1*/;

-- Mode 25
SET @25HM_ENTRY_LEI_SHI = 213073;
UPDATE `gameobject_template` SET `data1` = @25HM_ENTRY_LEI_SHI WHERE `entry` = @25HM_ENTRY_LEI_SHI;
DELETE FROM `gameobject_loot_template` WHERE `entry`= @25HM_ENTRY_LEI_SHI;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25HM_ENTRY_LEI_SHI, 1, 100, 1, 0, -@REF_LEI_SHI_2, 5) /* REF 1*/;

DELETE FROM creature WHERE id IN (62442);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES('62442','996','6067','6515','8','65535','0','0','-1017.95','-2807.68','38.2767','1.53812','300','0','0','0','0','0','0','0','0','0','0');

DELETE FROM `spell_script_names` WHERE spell_id IN(122768, 125843, 122789);
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (125843, 'spell_dread_shadows_damage');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122768, 'spell_dread_shadows_malus');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122789, 'spell_sunbeam');

UPDATE creature_template SET scriptname = "npc_sunbeam" WHERE entry = 62849;

-- The Dark of Night
UPDATE creature_template SET modelid1 = 42973, modelid2 = 42973, ScriptName = "npc_dark_of_night" WHERE entry = 63346;
-- Tsulong
UPDATE creature_template SET minlevel = 93, maxlevel = 93, unit_class = 4, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 4, ScriptName = "boss_tsulong" WHERE entry = 62442;
-- Embodied Terror

UPDATE creature_template SET faction_H = 16, faction_A = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.5, ScriptName = "npc_embodied_terror" WHERE entry = 62969;

INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122855, 'spell_sun_breath');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (123018, 'spell_terrorize_player');

-- Tiny Terror
UPDATE creature_template SET faction_H = 16, faction_A = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.1, ScriptName = "npc_tiny_terror" WHERE entry = 62977;

-- Unstable sha
UPDATE creature_template SET faction_A = 16, faction_H = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.25, ScriptName ="npc_unstable_sha" WHERE entry = 62919;

-- instability
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES(123697, 'spell_instability');

DELETE FROM creature_text WHERE entry = 62442;
INSERT INTO creature_text VALUES
(62442, 0, 0, 'You do not belong here! The waters must be protected... I will cast you out, or slay you!', 14, 0, 100, 0, 0, 29350, 'VO_TES_SERPENT_AGGRO_01'),
(62442, 1, 0, 'Protect... the waters...', 14, 0, 100, 0, 0, 29351, 'VO_TES_SERPENT_DEATH_01'),
(62442, 1, 1, 'I thank you, strangers. I have been freed..', 14, 0, 100, 0, 0, 29352, 'VO_TES_SERPENT_DEATH_02'),
(62442, 2, 0, 'Day falls to night...', 14, 0, 100, 0, 0, 29353, 'VO_TES_SERPENT_EVENT_DAYTONIGHT_01'),
(62442, 2, 1, ' Darkness grows...', 14, 0, 100, 0, 0, 29354, 'VO_TES_SERPENT_EVENT_DAYTONIGHT_02'),
(62442, 2, 2, 'Light fades..', 14, 0, 100, 0, 0, 29355, 'VO_TES_SERPENT_EVENT_DAYTONIGHT_03'),
(62442, 3, 0, 'Night... becomes day!', 14, 0, 100, 0, 0, 29356, 'VO_TES_SERPENT_EVENT_NIGHTTODAY_01'),
(62442, 3, 1, 'The sun... shines through...', 14, 0, 100, 0, 0, 29357, 'VO_TES_SERPENT_EVENT_NIGHTTODAY_02'),
(62442, 3, 2, 'Once more... the dawn...', 14, 0, 100, 0, 0, 29358, 'VO_TES_SERPENT_EVENT_NIGHTTODAY_03'),
(62442, 4, 0, 'No... the waters... I must... resist... I shall not... fear...', 14, 0, 100, 0, 0, 29359, 'VO_TES_SERPENT_INTRO_01'),
(62442, 5, 0, 'Die in darkness.', 14, 0, 100, 0, 0, 29360, 'VO_TES_SERPENT_SLAY_01'),
(62442, 5, 1, 'The night surrounds you...', 14, 0, 100, 0, 0, 29361, 'VO_TES_SERPENT_SLAY_02'),
(62442, 6, 0, 'No...!', 14, 0, 100, 0, 0, 29362, 'VO_TES_SERPENT_SLAY_03'),
(62442, 6, 1, 'Forgive me!', 14, 0, 100, 0, 0, 29363, 'VO_TES_SERPENT_SLAY_04'),
(62442, 7, 0, 'Overwhelming... fear!', 14, 0, 100, 0, 0, 29364, 'VO_TES_SERPENT_SPELL_NIGGHTMARE_01'),
(62442, 7, 1, 'Flee this place... or die!', 14, 0, 100, 0, 0, 29365, 'VO_TES_SERPENT_SPELL_NIGGHTMARE_02');


DELETE FROM locales_creature_text WHERE entry = 62442;
INSERT INTO locales_creature_text VALUES
(62442, 0, 0, '', 'Vous n\'avez pas votre place ici ! Les eaux doivent être protégées… je vais vous renvoyer, ou vous tuer !', '', '', '', '', '', '', '', ''),
(62442, 1, 0, '', 'Protéger ... les eaux', '', '', '', '', '', '', '', ''),
(62442, 1, 1, '', 'Je vous remercie, étrangers. J\'ai été libéré.', '', '', '', '', '', '', '', ''),
(62442, 2, 0, '', 'Le jour s\'efface devant la nuit...', '', '', '', '', '', '', '', ''),
(62442, 2, 1, '', 'Les ombres s\'allongent...', '', '', '', '', '', '', '', ''),
(62442, 2, 2, '', 'La lumière s’estompe...', '', '', '', '', '', '', '', ''),
(62442, 3, 0, '', 'La nuit... devient le jour !', '', '', '', '', '', '', '', ''),
(62442, 3, 1, '', 'Le soleil... éclaire les ombres...', '', '', '', '', '', '', '', ''),
(62442, 3, 2, '', 'À nouveau... l\'aurore...', '', '', '', '', '', '', '', ''),
(62442, 4, 0, '', 'Non ... les eaux ... Je doit ... résister ... Je n\'aurais pas ... peur ...', '', '', '', '', '', '', '', ''),
(62442, 5, 0, '', 'Meurs dans les ténèbres.', '', '', '', '', '', '', '', ''),
(62442, 5, 1, '', 'Que la nuit t\'enveloppe', '', '', '', '', '', '', '', ''),
(62442, 6, 0, '', 'Non... !', '', '', '', '', '', '', '', ''),
(62442, 6, 1, '', 'Pardonnez-moi !', '', '', '', '', '', '', '', ''),
(62442, 7, 0, '', 'Irrésistible… terreur !', '', '', '', '', '', '', '', ''),
(62442, 7, 1, '', 'Fuyez ce lieu… ou mourez !', '', '', '', '', '', '', '', '');

-- Loots for Tsulong
DELETE FROM gameobject_template WHERE entry IN (215355, 215356, 215357, 212922);
INSERT INTO gameobject_template VALUES
-- Normal mode 10
(215355, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 215355, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Normal mode 25
(215356, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 215356, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Heroic mode 10
(215357, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 215357, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Heroic mode 25
(212922, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 212922, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595);

-- MODE : 10_NM (215355)

SET @10_NM_ENTRY = 215355;
DELETE FROM `reference_loot_template` WHERE `entry`= @10_NM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY, 86328, 14, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@10_NM_ENTRY, 86324, 14, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@10_NM_ENTRY, 89883, 13, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@10_NM_ENTRY, 86327, 14, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@10_NM_ENTRY, 86321, 14, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@10_NM_ENTRY, 86330, 13, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@10_NM_ENTRY, 86322, 13, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@10_NM_ENTRY, 86323, 12, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@10_NM_ENTRY, 89843, 10, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@10_NM_ENTRY, 86326, 9, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@10_NM_ENTRY, 89884, 9, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@10_NM_ENTRY, 86325, 9, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@10_NM_ENTRY, 89842, 8, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@10_NM_ENTRY, 86329, 9, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@10_NM_ENTRY, 87209, 8, 1, 1, 1, 1) /* 2Sigil of Wisdom */, 
(@10_NM_ENTRY, 87208, 2, 1, 1, 1, 1) /* 2Sigil of Power */, 
(@10_NM_ENTRY, 86383, 2, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@10_NM_ENTRY, 86339, 1, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@10_NM_ENTRY, 80433, 1, 1, 1, 1, 2) /* 4Blood Spirit */, 
(@10_NM_ENTRY, 86343, 1, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@10_NM_ENTRY, 86338, 1, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@10_NM_ENTRY, 86384, 0.94, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */, 
(@10_NM_ENTRY, 86342, 0.9, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@10_NM_ENTRY, 86238, 0.85, 1, 1, 1, 1) /* 3Pattern: Chestguard of Nemeses */, 
(@10_NM_ENTRY, 87412, 0.65, 1, 1, 1, 1) /* 3Plans: Chestplate of Limitless Faith */, 
(@10_NM_ENTRY, 86385, 0.77, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@10_NM_ENTRY, 86341, 0.45, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@10_NM_ENTRY, 87408, 0.49, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@10_NM_ENTRY, 86280, 0.53, 1, 1, 1, 1) /* 3Pattern: Murderer's Gloves */, 
(@10_NM_ENTRY, 86281, 0.65, 1, 1, 1, 1) /* 3Pattern: Nightfire Robe */, 
(@10_NM_ENTRY, 87413, 0.57, 1, 1, 1, 1) /* 3Plans: Gauntlets of Unbound Devotion */, 
(@10_NM_ENTRY, 86337, 0.61, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@10_NM_ENTRY, 86272, 0.57, 1, 1, 1, 1) /* 3Pattern: Fists of Lightning */, 
(@10_NM_ENTRY, 86284, 0.65, 1, 1, 1, 1) /* 3Pattern: Raven Lord's Gloves */, 
(@10_NM_ENTRY, 86379, 0.49, 1, 1, 1, 1) /* 3Pattern: Robe of Eternal Rule */, 
(@10_NM_ENTRY, 86340, 0.53, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@10_NM_ENTRY, 87409, 0.45, 1, 1, 1, 1) /* 3Plans: Gauntlets of Battle Command */, 
(@10_NM_ENTRY, 87411, 0.53, 1, 1, 1, 1) /* 3Plans: Bloodforged Warfists */, 
(@10_NM_ENTRY, 86279, 0.45, 1, 1, 1, 1) /* 3Pattern: Liferuned Leather Gloves */, 
(@10_NM_ENTRY, 86283, 0.33, 1, 1, 1, 1) /* 3Pattern: Raiment of Blood and Bone */, 
(@10_NM_ENTRY, 86381, 0.41, 1, 1, 1, 1) /* 3Pattern: Legacy of the Emperor */, 
(@10_NM_ENTRY, 86380, 0.41, 1, 1, 1, 1) /* 3Pattern: Imperial Silk Gloves */, 
(@10_NM_ENTRY, 86297, 0.37, 1, 1, 1, 1) /* 3Pattern: Stormbreaker Chestguard */, 
(@10_NM_ENTRY, 86382, 0.24, 1, 1, 1, 1) /* 3Pattern: Touch of the Light */, 
(@10_NM_ENTRY, 87410, 0.28, 1, 1, 1, 1) /* 3Plans: Ornate Battleplate of the Master */;

DELETE FROM `gameobject_loot_template` WHERE `entry`= @10_NM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY, 1, 100, 1, 0, -@10_NM_ENTRY, 2);



-- MODE : 25_NM (215356)

SET @25_NM_ENTRY = 215356;
DELETE FROM `reference_loot_template` WHERE `entry`= @25_NM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY, 86328, 34, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@25_NM_ENTRY, 86324, 30, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@25_NM_ENTRY, 89883, 39, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@25_NM_ENTRY, 86327, 29, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@25_NM_ENTRY, 86321, 30, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@25_NM_ENTRY, 86330, 33, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@25_NM_ENTRY, 86322, 32, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@25_NM_ENTRY, 86323, 25, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@25_NM_ENTRY, 89843, 31, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@25_NM_ENTRY, 86326, 26, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@25_NM_ENTRY, 89884, 28, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@25_NM_ENTRY, 86325, 27, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@25_NM_ENTRY, 89842, 27, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@25_NM_ENTRY, 86329, 19, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@25_NM_ENTRY, 87209, 9, 1, 1, 1, 1) /* 2Sigil of Wisdom */, 
(@25_NM_ENTRY, 87208, 7, 1, 1, 1, 1) /* 2Sigil of Power */, 
(@25_NM_ENTRY, 86383, 4, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@25_NM_ENTRY, 86339, 2, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@25_NM_ENTRY, 80433, 0.26, 1, 1, 1, 2) /* 4Blood Spirit */, 
(@25_NM_ENTRY, 86343, 2, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@25_NM_ENTRY, 86338, 0.26, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@25_NM_ENTRY, 86384, 1, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */, 
(@25_NM_ENTRY, 86342, 1, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@25_NM_ENTRY, 86238, 0.26, 1, 1, 1, 1) /* 3Pattern: Chestguard of Nemeses */, 
(@25_NM_ENTRY, 87412, 2, 1, 1, 1, 1) /* 3Plans: Chestplate of Limitless Faith */, 
(@25_NM_ENTRY, 86385, 1, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@25_NM_ENTRY, 86341, 2, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@25_NM_ENTRY, 87408, 0.79, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@25_NM_ENTRY, 86280, 1, 1, 1, 1, 1) /* 3Pattern: Murderer's Gloves */, 
(@25_NM_ENTRY, 86281, 0.26, 1, 1, 1, 1) /* 3Pattern: Nightfire Robe */, 
(@25_NM_ENTRY, 87413, 0.79, 1, 1, 1, 1) /* 3Plans: Gauntlets of Unbound Devotion */, 
(@25_NM_ENTRY, 86337, 0.52, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@25_NM_ENTRY, 86272, 0.52, 1, 1, 1, 1) /* 3Pattern: Fists of Lightning */, 
(@25_NM_ENTRY, 86340, 0.52, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@25_NM_ENTRY, 87409, 0.52, 1, 1, 1, 1) /* 3Plans: Gauntlets of Battle Command */, 
(@25_NM_ENTRY, 86279, 0.26, 1, 1, 1, 1) /* 3Pattern: Liferuned Leather Gloves */, 
(@25_NM_ENTRY, 86381, 0.26, 1, 1, 1, 1) /* 3Pattern: Legacy of the Emperor */, 
(@25_NM_ENTRY, 86382, 0.52, 1, 1, 1, 1) /* 3Pattern: Touch of the Light */, 
(@25_NM_ENTRY, 87410, 0.26, 1, 1, 1, 1) /* 3Plans: Ornate Battleplate of the Master */;

DELETE FROM `gameobject_loot_template` WHERE `entry`= @25_NM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY, 1, 100, 1, 0, -@25_NM_ENTRY, 5);


-- MODE : 10_HM (215357)

SET @10_HM_ENTRY = 215357;
DELETE FROM `reference_loot_template` WHERE `entry`= @10_HM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY, 80433, 9, 1, 1, 1, 2) /* 4Blood Spirit */, 
(@10_HM_ENTRY, 87164, 29, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@10_HM_ENTRY, 89945, 13, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@10_HM_ENTRY, 87162, 13, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@10_HM_ENTRY, 87163, 16, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@10_HM_ENTRY, 89946, 12, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@10_HM_ENTRY, 87157, 17, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@10_HM_ENTRY, 87156, 10, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@10_HM_ENTRY, 87408, 3, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@10_HM_ENTRY, 89947, 3, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@10_HM_ENTRY, 87158, 12, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@10_HM_ENTRY, 87160, 9, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@10_HM_ENTRY, 87159, 6, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@10_HM_ENTRY, 87161, 9, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@10_HM_ENTRY, 86283, 4, 1, 1, 1, 1) /* 3Pattern: Raiment of Blood and Bone */, 
(@10_HM_ENTRY, 89948, 4, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@10_HM_ENTRY, 87165, 1, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@10_HM_ENTRY, 87178, 3, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@10_HM_ENTRY, 87182, 1, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@10_HM_ENTRY, 87184, 1, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@10_HM_ENTRY, 87179, 1, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@10_HM_ENTRY, 87181, 3, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@10_HM_ENTRY, 87186, 3, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@10_HM_ENTRY, 87177, 3, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@10_HM_ENTRY, 87180, 3, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@10_HM_ENTRY, 87183, 3, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@10_HM_ENTRY, 87185, 3, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */;

DELETE FROM `gameobject_loot_template` WHERE `entry`= @10_HM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY, 1, 100, 1, 0, -@10_HM_ENTRY, 2);


-- MODE : 25_HM (212922)
SET @25_HM_ENTRY = 212922;
DELETE FROM `reference_loot_template` WHERE `entry`= @25_HM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY, 87164, 25, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@25_HM_ENTRY, 89945, 56, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@25_HM_ENTRY, 87162, 53, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@25_HM_ENTRY, 87163, 44, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@25_HM_ENTRY, 86238, 3, 1, 1, 1, 1) /* 3Pattern: Chestguard of Nemeses */, 
(@25_HM_ENTRY, 89946, 47, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@25_HM_ENTRY, 87157, 25, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@25_HM_ENTRY, 87156, 38, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@25_HM_ENTRY, 87408, 3, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@25_HM_ENTRY, 89947, 47, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@25_HM_ENTRY, 87158, 25, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@25_HM_ENTRY, 87409, 3, 1, 1, 1, 1) /* 3Plans: Gauntlets of Battle Command */, 
(@25_HM_ENTRY, 87160, 25, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@25_HM_ENTRY, 87159, 28, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@25_HM_ENTRY, 87161, 22, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@25_HM_ENTRY, 89948, 19, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@25_HM_ENTRY, 87165, 19, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@25_HM_ENTRY, 87178, 6, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@25_HM_ENTRY, 87182, 3, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@25_HM_ENTRY, 87184, 3, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@25_HM_ENTRY, 87179, 6, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@25_HM_ENTRY, 87181, 3, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@25_HM_ENTRY, 87186, 3, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@25_HM_ENTRY, 87177, 6, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@25_HM_ENTRY, 87180, 6, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@25_HM_ENTRY, 87183, 6, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@25_HM_ENTRY, 87185, 6, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */;
DELETE FROM `gameobject_loot_template` WHERE `entry`= @25_HM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY, 1, 100, 1, 0, -@25_HM_ENTRY, 5);


-- Tsulong
DELETE FROM creature_template WHERE entry in (624420, 624421, 624422);
-- 25 NM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('624420','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42532','0','0','0','Tsulong','','','0','93','93','4','0','2104','2104','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','6','0','0','4','0','0','0','0','0','0','0','0','0','0','0','2','2101356','0','62442','0','0','0','0','0','0','0','0','122858','26662','122768','125843','57723','124176','95809','122777','0','0','0','0','','0','3','1','1092','1','1','1','0','0','0','0','0','0','0','157','1','0','0','0','','16048');
-- 10 HM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('624421','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42532','0','0','0','Tsulong','','','0','93','93','4','0','2104','2104','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','5','0','0','4','0','0','0','0','0','0','0','0','0','0','0','2','2101356','0','62442','0','0','0','0','0','0','0','0','122858','26662','122768','125843','57723','124176','95809','122777','0','0','0','0','','0','3','1','582.4','1','1','1','0','0','0','0','0','0','0','157','1','0','0','0','','16048');
-- 25 HM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('624422','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42532','0','0','0','Tsulong','','','0','93','93','4','0','2104','2104','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','8','0','0','4','0','0','0','0','0','0','0','0','0','0','0','2','2101356','0','62442','0','0','0','0','0','0','0','0','122858','26662','122768','125843','57723','124176','95809','122777','0','0','0','0','','0','3','1','1747','1','1','1','0','0','0','0','0','0','0','157','1','0','0','0','','16048');
UPDATE creature_template SET difficulty_entry_4 = 624420, difficulty_entry_5 = 624421,  difficulty_entry_6 = 624422 WHERE entry = 62442;

-- Unstable sha
DELETE FROM creature_template WHERE entry in (629190);
-- 10HM/25HM (same template)
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('629190','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41677','0','0','0','Unstable Sha','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','3.25','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','130078','123697','122881','0','0','0','0','0','0','0','0','0','','0','3','1','4.75','1','1','1','0','0','0','0','0','0','0','84','1','0','0','0','','16048');
UPDATE creature_template SET difficulty_entry_5 = 629190,  difficulty_entry_6 = 629190 WHERE entry = 62919;

-- Embodied Terror
DELETE FROM creature_template WHERE entry in (629690, 629691, 629692);
-- 25 NM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('629690','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42420','0','0','0','Embodied Terror','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','3','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','123012','123011','0','0','0','0','0','0','0','0','0','0','','0','3','1','26.66','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','16048');
-- 10 HM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('629691','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42420','0','0','0','Embodied Terror','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','2.7','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','123012','123011','0','0','0','0','0','0','0','0','0','0','','0','3','1','13.09','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','16048');
-- 25 HM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('629692','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','42420','0','0','0','Embodied Terror','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','3.25','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','123012','123011','0','0','0','0','0','0','0','0','0','0','','0','3','1','38.18','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','16048');
UPDATE creature_template SET difficulty_entry_4 = 629690, difficulty_entry_5 = 629691,  difficulty_entry_6 = 629692 WHERE entry = 62969;


-- Fright Spawn
DELETE FROM creature_template WHERE entry in (629770, 629771, 629772);
-- 25 NM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('629770','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40625','0','0','0','Fright Spawn','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','2.1','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','123036','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','4.54','1','1','1','0','0','0','0','0','0','0','84','1','0','0','0','','16048');
-- 10 HM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('629771','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40625','0','0','0','Fright Spawn','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','2.1','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','123036','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','4.90','1','1','1','0','0','0','0','0','0','0','84','1','0','0','0','','16048');
-- 25 HM
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('629772','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','40625','0','0','0','Fright Spawn','','','0','92','92','4','0','16','16','0','0','1','1.14286','1.14286','1','1','25000','50000','0','60000','2.1','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','123036','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','14.09','1','1','1','0','0','0','0','0','0','0','84','1','0','0','0','','16048');
UPDATE creature_template SET difficulty_entry_4 = 629770, difficulty_entry_5 = 629771,  difficulty_entry_6 = 629772 WHERE entry = 62977;

UPDATE creature_template SET Health_mod = 1.35 WHERE entry = 62977;


DELETE FROM creature WHERE id IN (60999, 71095, 61038, 61042, 61046, 65736) AND map = 996;
INSERT INTO `creature` (id, map, zoneId, areaId, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, npcflag2, unit_flags, dynamicflags, isActive) VALUES
('60999','996','6067','6515','760','65535','0','0','-1018.085','-2756.895','38.5722','4.73273','300','0','0','0','0','0','0','0','0','0','0');

UPDATE creature_model_info SET bounding_radius = 17, combat_reach = 17 WHERE modelid = 45065;
UPDATE creature_template SET
unit_class = 4,
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
rank = 3,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 14,
baseattacktime = 2000,
Health_mod = 381.15,
flags_extra = 0x01,
ScriptName = 'boss_sha_of_fear'
WHERE entry = 60999;
UPDATE creature_template SET
modelid1 = 11686,
modelid2 = 0,
flags_extra = 128,
ScriptName = 'mob_pure_light_terrace'
WHERE entry = 60788;
UPDATE creature_template SET
unit_flags2 = 0x800,
ScriptName = 'mob_return_to_the_terrace'
WHERE entry = 65736;
UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
Health_mod = 7.974,
ScriptName = 'mob_terror_spawn'
WHERE entry = 61034;
UPDATE creature_template SET
ScriptName = 'mob_shrine_guardian',
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
Health_mod = 33,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 10,
baseattacktime = 2000
WHERE entry IN (61038, 61042, 61046);
UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_sha_globe'
WHERE entry = 65691;
UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
Health_mod = 10.8,
ScriptName = 'mob_dread_spawn'
WHERE entry = 61003;

DELETE FROM creature_text WHERE entry IN (60999, 61038, 61042, 61046);
INSERT INTO creature_text VALUES
(60999, 0, 0, 'Your lights will go out. The darkness will envelop you. And you will fear the shadows that move within it.', 14, 0, 100, 0, 0, 28402, 'SHA_INTRO'),
(60999, 1, 0, 'You will know fear!', 14, 0, 100, 0, 0, 28398, 'SHA_AGGRO'),
(60999, 2, 0, 'Fleeeee!', 14, 0, 100, 0, 0, 28404, 'SHA_BREATH_OF_FEAR'),
(60999, 3, 0, 'Drown in terror.', 14, 0, 100, 0, 0, 28403, 'SHA_SLAY'),
(60999, 4, 0, 'You think it is so easy to vanquish your fears?', 14, 0, 100, 0, 0, 28400, 'SHA_SLAY_HEROIC'),
(60999, 5, 0, 'Dread rises! It seeks the light. It hungers.', 14, 0, 100, 0, 0, 28401, 'SHA_SUBMERGE'),
(60999, 6, 0, 'Huddle in terror!', 14, 0, 100, 0, 0, 28405, 'SHA_HUDDLE'),
(61038, 0, 0, 'Its power is so strong. It forces me to destroy you.', 14, 0, 100, 0, 0, 0, 'YANG_GUOSHI_1'),
(61038, 0, 1, 'Seek shelter, lest I strike you down as the Sha commands!', 14, 0, 100, 0, 0, 0, 'YANG_GUOSHI_2'),
(61038, 0, 2, 'You face your fear. A moment''s... peace.', 14, 0, 100, 0, 0, 0, 'YANG_GUOSHI_3'),
(61042, 0, 0, 'The fear clouds my mind. I...cannot resist.', 14, 0, 100, 0, 0, 0, 'CHENG_KANG_1'),
(61042, 0, 1, 'Seek shelter, lest I strike you down as the Sha commands!', 14, 0, 100, 0, 0, 0, 'CHENG_KANG_2'),
(61042, 0, 2, 'You face your fear. A moment''s... peace.', 14, 0, 100, 0, 0, 0, 'CHENG_KANG_3'),
(61046, 0, 0, 'No one can stand against the fear. You will perish.', 14, 0, 100, 0, 0, 0, 'JINLUN_KUN_1'),
(61046, 0, 1, 'Seek shelter, lest I strike you down as the Sha commands!', 14, 0, 100, 0, 0, 0, 'JINLUN_KUN_2'),
(61046, 0, 2, 'You face your fear. A moment''s... peace.', 14, 0, 100, 0, 0, 0, 'JINLUN_KUN_3');

DELETE FROM locales_creature_text WHERE entry IN (60999, 61038, 61042, 61046);
INSERT INTO locales_creature_text VALUES
(60999, 0, 0, '', 'Vos lumières vont s''éteindre. Les ténèbres vont vous envelopper. Et vous craindrez les ombres qu''elles contiennent.', '', '', '', '', '', '', '', ''),
(60999, 1, 0, '', 'Vous apprendrez à avoir peur !', '', '', '', '', '', '', '', ''),
(60999, 2, 0, '', 'Fuyeeezzzz !', '', '', '', '', '', '', '', ''),
(60999, 3, 0, '', 'Noyez-vous dans la terreur.', '', '', '', '', '', '', '', ''),
(60999, 4, 0, '', 'Vous pensez que c''est si facile de vaincre vos propores peurs ?', '', '', '', '', '', '', '', ''),
(60999, 5, 0, '', 'L''effroi monte ! Il cherche la lumière. Il a faim.', '', '', '', '', '', '', '', ''),
(60999, 6, 0, '', 'Tremblez de peur !', '', '', '', '', '', '', '', ''),
(61038, 0, 0, '', 'Son pouvoir est bien trop grand. Il m’oblige à vous détruire.', '', '', '', '', '', '', '', ''),
(61038, 0, 1, '', 'Mettez-vous à l''abri ou je vous abattrai sur l''ordre du sha !', '', '', '', '', '', '', '', ''),
(61038, 0, 2, '', 'Vous affrontez vos peurs. Un moment… de paix.', '', '', '', '', '', '', '', ''),
(61042, 0, 0, '', 'La peur embrouille mon esprit. Je... ne peux résister.', '', '', '', '', '', '', '', ''),
(61042, 0, 1, '', 'Mettez-vous à l''abri ou je vous abattrai sur l''ordre du sha !', '', '', '', '', '', '', '', ''),
(61042, 0, 2, '', 'Vous affrontez vos peurs. Un moment… de paix.', '', '', '', '', '', '', '', ''),
(61046, 0, 0, '', 'Personne ne peut résister à la peur. Vous allez mourir.', '', '', '', '', '', '', '', ''),
(61046, 0, 1, '', 'Mettez-vous à l''abri ou je vous abattrai sur l''ordre du sha !', '', '', '', '', '', '', '', ''),
(61046, 0, 2, '', 'Vous affrontez vos peurs. Un moment… de paix.', '', '', '', '', '', '', '', '');

DELETE FROM spell_script_names WHERE spell_id IN (119414, 125786, 119108, 129075, 119593, 120047, 119983, 129189, 119888, 119887, 120455, 120458);
INSERT INTO spell_script_names VALUES
(119414, 'spell_breath_of_fear'),
(125786, 'spell_breath_of_fear'),
(119108, 'spell_conjure_terror_spawn'),
(129075, 'spell_penetrating_bolt'),
(119593, 'spell_ominous_cackle_cast'),
(120047, 'spell_dread_spray'),
(119983, 'spell_dread_spray_stacks'),
(129189, 'spell_sha_globe_periodic'),
(119888, 'spell_death_blossom_periodic'),
(119887, 'spell_death_blossom_damage'),
(120455, 'spell_submerge'),
(120458, 'spell_emerge');

DELETE FROM spell_target_position WHERE id IN (129234, 129235, 129236);
INSERT INTO spell_target_position VALUES
(129234, 0, 996, -810.053, -2741.738, 31.705, 3.309),
(129235, 0, 996, -1078.285, -2556.075, 15.879, 4.867),
(129236, 0, 996, -1236.727, -2833.311, 41.271, 0.363);

-- 10 Heroic
-- Sha of Fear
UPDATE creature_template SET difficulty_entry_5 = 80999 WHERE entry = 60999;
INSERT INTO `creature_template` VALUES
('80999','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','45065','0','0','0','Sha of Fear (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','16','2000','0','4','0','0','0','0','0','0','0','0','0','0','0','4','2097260','1','60999','0','0','0','0','0','0','0','0','47008','119414','125786','132007','119519','120458','129378','120629','0','0','3200000','3300000','','0','3','1','1122.66','1','1','1','0','87210','0','0','0','0','0','167','1','0','769638399','1','','16048');
-- Terror Spawn
UPDATE creature_template SET difficulty_entry_5 = 81034 WHERE entry = 61034;
INSERT INTO `creature_template` VALUES
('81034','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46113','0','0','0','Terror Spawn (5)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','72','0','0','0','0','0','0','0','0','0','0','119086','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','13.041','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Yang Guoshi
UPDATE creature_template SET difficulty_entry_5 = 81038 WHERE entry = 61038;
INSERT INTO `creature_template` VALUES
('81038','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41815','0','0','0','Yang Guoshi (5)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','48.6','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Cheng Kang
UPDATE creature_template SET difficulty_entry_5 = 81042 WHERE entry = 61042;
INSERT INTO `creature_template` VALUES
('81042','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41814','0','0','0','Cheng Kang (5)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','48.6','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Jinlun Kun
UPDATE creature_template SET difficulty_entry_5 = 81046 WHERE entry = 61046;
INSERT INTO `creature_template` VALUES
('81046','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41816','0','0','0','Jinlun Kun (5)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','48.6','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');

-- 25 Normal
-- Sha of Fear
UPDATE creature_template SET difficulty_entry_4 = 81999 WHERE entry = 60999;
INSERT INTO `creature_template` VALUES
('81999','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','45065','0','0','0','Sha of Fear (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','18','2000','0','4','0','0','0','0','0','0','0','0','0','0','0','4','2097260','1','60999','0','0','0','0','0','0','0','0','47008','119414','125786','132007','119519','120458','129378','120629','0','0','3200000','3300000','','0','3','1','1143.45','1','1','1','0','87210','0','0','0','0','0','167','1','0','769638399','1','','16048');
-- Terror Spawn
UPDATE creature_template SET difficulty_entry_4 = 82034 WHERE entry = 61034;
INSERT INTO `creature_template` VALUES
('82034','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46113','0','0','0','Terror Spawn (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','72','0','0','0','0','0','0','0','0','0','0','119086','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','23.913','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Yang Guoshi
UPDATE creature_template SET difficulty_entry_4 = 82038 WHERE entry = 61038;
INSERT INTO `creature_template` VALUES
('82038','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41815','0','0','0','Yang Guoshi (4)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','33','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Cheng Kang
UPDATE creature_template SET difficulty_entry_4 = 82042 WHERE entry = 61042;
INSERT INTO `creature_template` VALUES
('82042','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41814','0','0','0','Cheng Kang (4)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','33','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Jinlun Kun
UPDATE creature_template SET difficulty_entry_4 = 82046 WHERE entry = 61046;
INSERT INTO `creature_template` VALUES
('82046','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41816','0','0','0','Jinlun Kun (4)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','33','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');

-- 25 Heroic
-- Sha of Fear
UPDATE creature_template SET difficulty_entry_6 = 82999 WHERE entry = 60999;
INSERT INTO `creature_template` VALUES
('82999','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','45065','0','0','0','Sha of Fear (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','18','2000','0','4','0','0','0','0','0','0','0','0','0','0','0','4','2097260','1','60999','0','0','0','0','0','0','0','0','47008','119414','125786','132007','119519','120458','129378','120629','0','0','3200000','3300000','','0','3','1','3367.98','1','1','1','0','87210','0','0','0','0','0','167','1','0','769638399','1','','16048');
-- Terror Spawn
UPDATE creature_template SET difficulty_entry_6 = 83034 WHERE entry = 61034;
INSERT INTO `creature_template` VALUES
('83034','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46113','0','0','0','Terror Spawn (6)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','72','0','0','0','0','0','0','0','0','0','0','119086','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','43.423','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Yang Guoshi
UPDATE creature_template SET difficulty_entry_6 = 83038 WHERE entry = 61038;
INSERT INTO `creature_template` VALUES
('83038','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41815','0','0','0','Yang Guoshi (6)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','54','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Cheng Kang
UPDATE creature_template SET difficulty_entry_6 = 83042 WHERE entry = 61042;
INSERT INTO `creature_template` VALUES
('83042','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41814','0','0','0','Cheng Kang (6)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','54','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Jinlun Kun
UPDATE creature_template SET difficulty_entry_6 = 83046 WHERE entry = 61046;
INSERT INTO `creature_template` VALUES
('83046','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41816','0','0','0','Jinlun Kun (6)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','14','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','54','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Dread Spawn
UPDATE creature_template SET difficulty_entry_6 = 83003 WHERE entry = 61003;
INSERT INTO `creature_template` VALUES
('83003','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46114','0','0','0','Dread Spawn (6)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','2','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','4','104','0','0','0','0','0','0','0','0','0','0','120394','128419','120388','0','0','0','0','0','0','0','0','0','','0','3','1','32.4','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');

-- LFR
-- Sha of Fear
UPDATE creature_template SET difficulty_entry_7 = 83999 WHERE entry = 60999;
INSERT INTO `creature_template` VALUES
('83999','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','45065','0','0','0','Sha of Fear (7)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','5174','12574','0','62114','16','2000','0','4','0','0','0','0','0','0','0','0','0','0','0','4','2097260','1','60999','0','0','0','0','0','0','0','0','47008','119414','125786','132007','119519','120458','129378','120629','0','0','3200000','3300000','','0','3','1','788.3','1','1','1','0','87210','0','0','0','0','0','167','1','0','769638399','1','','16048');
-- Terror Spawn
UPDATE creature_template SET difficulty_entry_7 = 84034 WHERE entry = 61034;
INSERT INTO `creature_template` VALUES
('84034','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46113','0','0','0','Terror Spawn (7)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','4','72','0','0','0','0','0','0','0','0','0','0','119086','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','7.15','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Yang Guoshi
UPDATE creature_template SET difficulty_entry_7 = 84038 WHERE entry = 61038;
INSERT INTO `creature_template` VALUES
('84038','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41815','0','0','0','Yang Guoshi (7)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','14.25','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Cheng Kang
UPDATE creature_template SET difficulty_entry_7 = 84042 WHERE entry = 61042;
INSERT INTO `creature_template` VALUES
('84042','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41814','0','0','0','Cheng Kang (7)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','14.25','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');
-- Jinlun Kun
UPDATE creature_template SET difficulty_entry_7 = 84046 WHERE entry = 61046;
INSERT INTO `creature_template` VALUES
('84046','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','41816','0','0','0','Jinlun Kun (7)','Shadow of the Golden Lotus','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','5174','12574','0','62114','12','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','119888','119983','120047','119862','0','0','0','0','0','0','0','0','','0','3','1','14.25','1','1','1','0','0','0','0','0','0','0','103','1','0','0','0','','16048');

-- Normal mode
SET @REF_SHA_OF_FEAR_NORMAL = 60999;
DELETE FROM `reference_loot_template` WHERE entry = @REF_SHA_OF_FEAR_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SHA_OF_FEAR_NORMAL, 89236, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Protector */,
(@REF_SHA_OF_FEAR_NORMAL, 89234, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Vanquisher */,
(@REF_SHA_OF_FEAR_NORMAL, 89235, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Conqueror */,
(@REF_SHA_OF_FEAR_NORMAL, 86386, 0, 1, 1, 1, 1) /* 3Shin'ka, Execution of Dominion */,
(@REF_SHA_OF_FEAR_NORMAL, 86388, 0, 1, 1, 1, 1) /* 3Essence of Terror */,
(@REF_SHA_OF_FEAR_NORMAL, 86387, 0, 1, 1, 1, 1) /* 3Kilrak, Jaws of Terror */,
(@REF_SHA_OF_FEAR_NORMAL, 89886, 0, 1, 1, 1, 1) /* 3Wrap of Instant Petrification */,
(@REF_SHA_OF_FEAR_NORMAL, 89887, 0, 1, 1, 1, 1) /* 3Robes of Pinioned Eyes */,
(@REF_SHA_OF_FEAR_NORMAL, 89839, 0, 1, 1, 1, 1) /* 3Shadowgrip Girdle */,
(@REF_SHA_OF_FEAR_NORMAL, 86389, 0, 1, 1, 1, 1) /* 3Dreadwoven Leggings of Failure */;

-- Heroic mode
SET @REF_SHA_OF_FEAR_HEROIC = 80999;
DELETE FROM `reference_loot_template` WHERE entry = @REF_SHA_OF_FEAR_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SHA_OF_FEAR_HEROIC, 89260, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Protector */,
(@REF_SHA_OF_FEAR_HEROIC, 89258, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Vanquisher */,
(@REF_SHA_OF_FEAR_HEROIC, 89259, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Conqueror */,
(@REF_SHA_OF_FEAR_HEROIC, 87176, 0, 1, 1, 1, 1) /* 3Shin'ka, Execution of Dominion */,
(@REF_SHA_OF_FEAR_HEROIC, 87175, 0, 1, 1, 1, 1) /* 3Essence of Terror */,
(@REF_SHA_OF_FEAR_HEROIC, 87173, 0, 1, 1, 1, 1) /* 3Kilrak, Jaws of Terror */,
(@REF_SHA_OF_FEAR_HEROIC, 89950, 0, 1, 1, 1, 1) /* 3Wrap of Instant Petrification */,
(@REF_SHA_OF_FEAR_HEROIC, 89949, 0, 1, 1, 1, 1) /* 3Robes of Pinioned Eyes */,
(@REF_SHA_OF_FEAR_HEROIC, 89951, 0, 1, 1, 1, 1) /* 3Shadowgrip Girdle */,
(@REF_SHA_OF_FEAR_HEROIC, 87174, 0, 1, 1, 1, 1) /* 3Dreadwoven Leggings of Failure */;

-- Mode 10 Normal
SET @10NM_SHA_OF_FEAR = 60999;
UPDATE `creature_template` SET `lootid`= @10NM_SHA_OF_FEAR WHERE `entry`= @10NM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @10NM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10NM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_NORMAL, 2) /* REF 1*/;

-- Mode 25 Normal
SET @25NM_SHA_OF_FEAR = 81999;
UPDATE `creature_template` SET `lootid`= @25NM_SHA_OF_FEAR WHERE `entry`= @25NM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @25NM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25NM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_NORMAL, 2) /* REF 1*/;

-- Mode 10 Heroic
SET @10HM_SHA_OF_FEAR = 80999;
UPDATE `creature_template` SET `lootid`= @10HM_SHA_OF_FEAR WHERE `entry`= @10HM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @10HM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10HM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_HEROIC, 5) /* REF 1*/;

-- Mode 25 Heroic
SET @25HM_SHA_OF_FEAR = 82999;
UPDATE `creature_template` SET `lootid`= @25HM_SHA_OF_FEAR WHERE `entry`= @25HM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @25HM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25HM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_HEROIC, 5) /* REF 1*/;

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_SHA_OF_FEAR, @25NM_SHA_OF_FEAR, @10HM_SHA_OF_FEAR, @25HM_SHA_OF_FEAR);
REPLACE INTO creature_loot_currency (`creature_id`, `CurrencyId1`, `CurrencyCount1`) VALUES
(@10NM_SHA_OF_FEAR, 396, 4000),
(@25NM_SHA_OF_FEAR, 396, 4000),
(@10HM_SHA_OF_FEAR, 396, 4000),
(@25HM_SHA_OF_FEAR, 396, 4000); 

UPDATE creature SET spawnMask = 760 WHERE map = 996;
UPDATE gameobject SET spawnMask = 760 WHERE map = 996;
UPDATE creature SET spawntimesecs = 604800 WHERE map = 996;
UPDATE gameobject SET spawntimesecs = 604800 WHERE map = 996;