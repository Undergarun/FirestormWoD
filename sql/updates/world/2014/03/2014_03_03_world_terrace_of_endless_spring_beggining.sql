DELETE FROM access_requirement WHERE mapId = 996;
INSERT INTO access_requirement VALUES
(996, 3, 90, 90, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'),
(996, 4, 90, 90, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'),
(996, 5, 90, 90, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'),
(996, 6, 90, 90, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring'),
(996, 7, 90, 90, 0, 0, 0, 0, 0, 0, 0, '', 'Terrace of Endless Spring');

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