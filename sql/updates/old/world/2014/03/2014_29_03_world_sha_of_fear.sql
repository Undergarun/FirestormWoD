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

UPDATE creature SET spawntimesecs = 604800 WHERE map = 996;