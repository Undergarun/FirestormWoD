-- Removing unused spell scripts
DELETE FROM spell_script_names WHERE spell_id IN (118063, 116779);

UPDATE creature_template SET lootid = 60708 WHERE entry = 60708;
UPDATE creature_template SET lootid = 60701 WHERE entry = 60701;
UPDATE creature_template SET lootid = 77009 WHERE entry = 77020;
UPDATE creature_template SET lootid = 77053 WHERE entry = 77064;

DELETE FROM gameobject WHERE id IN (214442, 214634, 212695, 213276, 213277, 212916, 212943) OR id BETWEEN 214483 AND 214493;
INSERT INTO `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`) VALUES
(214442,  870, 6138, 6435, 1, 1, 164.597, 4069.01, 256.203, 4.88325, 0, 0, 0, 1, 7200, 255, 1, 0),
(214442, 1009, 6297, 6297, 8, 1, -2390.733, 459.7899, 422.3407, 0, 0, 0, 0, 1, 7200, 255, 1, 0),
(214483, 1009, 6297, 6297, 8, 1, -2307.711, 459.9609, 422.3357, 3.141593, 0, 0, 0, 1, 0, 7200, 255, 0),
(214484, 1009, 6297, 6297, 8, 1, -2252.5, 404.8451, 424.0067, 1.570796, 0, 0, 0, 1, 0, 7200, 255, 0),
(214485, 1009, 6297, 6297, 8, 1, -2252.5, 346.4307, 424.0067, 1.570796, 0, 0, 0, 1, 0, 7200, 255, 0),
(214486, 1009, 6297, 6297, 8, 1, -2187.895, 281.2121, 424.0067, 3.141593, 0, 0, 0, 1, 0, 7200, 255, 1),
(214487, 1009, 6297, 6297, 8, 1, -2119.144, 387.2309, 422.2571, 4.71239, 0, 0, 0, 1, 0, 7200, 255, 1),
(214488, 1009, 6297, 6297, 8, 1, -2035.252, 475.9384, 473.2037, 3.141593, 0, 0, 0, 1, 0, 7200, 255, 1),
(214489, 1009, 6297, 6297, 8, 1, -2163.632, 475.9384, 503.6568, 3.141593, 0, 0, 0, 1, 0, 7200, 255, 1),
(214490, 1009, 6297, 6297, 8, 1, -2387.138, 475.9781, 554.5891, 3.141593, 0, 0, 0, 1, 0, 7200, 255, 0),
(214491, 1009, 6297, 6297, 8, 1, -2478.975, 567.7377, 556.2987, 4.71239, 0, 0, 0, 1, 0, 7200, 255, 0),
(214492, 1009, 6297, 6297, 8, 1, -2478.806, 625.4995, 581.6339, 4.71239, 0, 0, 0, 1, 0, 7200, 255, 1),
(214493, 1009, 6297, 6297, 8, 1, -2478.58, 823.0958, 586.7272, 4.71239, 0, 0, 0, 1, 0, 7200, 255, 1);

-- GameObject Templates for doors

DELETE FROM gameobject_template WHERE entry IN (214634, 212695, 213276, 213277, 212916, 212943) OR entry BETWEEN 214483 AND 214493;
INSERT INTO gameobject_template (entry, TYPE, displayId, NAME, faction, flags, size, data0, data1, data9, WDBVerified) VALUES
(214483, 0, 11731, 'Doodad_Mantid_Door_017', 1375, 32, 1, 1, 5755148, 0, 16048),
(214484, 0, 11731, 'Doodad_Mantid_Door_020', 1375, 32, 0.954693, 1, 5755148, 0, 16048),
(214485, 0, 11731, 'Doodad_Mantid_Door_021', 1375, 32, 1, 1, 5755148, 0, 16048),
(214486, 0, 11731, 'Doodad_Mantid_Door_022', 1375, 32, 1, 1, 5755148, 0, 16048),
(214487, 0, 11731, 'Doodad_Mantid_Door_024', 1375, 32, 1, 1, 5755148, 0, 16048),
(214488, 0, 11731, 'Doodad_Mantid_Door_025', 1375, 32, 1, 1, 5755148, 0, 16048),
(214489, 0, 11731, 'Doodad_Mantid_Door_026', 1375, 32, 1, 1, 5755148, 0, 16048),
(214490, 0, 11731, 'Doodad_Mantid_Door_027', 1375, 32, 1, 1, 5755148, 0, 16048),
(214491, 0, 11731, 'Doodad_Mantid_Door_031', 1375, 32, 1, 1, 5755148, 0, 16048),
(214492, 0, 11731, 'Doodad_Mantid_Door_032', 1375, 32, 1, 1, 5755148, 0, 16048),
(214493, 0, 11731, 'Doodad_Mantid_Door_033', 1375, 32, 1, 1, 5755148, 0, 16048),
(214634, 0,  7530, 'Door of Garalon (up)', 114, 32, 2, 0, 0, 1, 16048),
(212695, 0,  7530, 'Door of Garalon', 114, 32, 2, 1, 0, 1, 16048),
(213276, 0, 11726, 'Mantid Queen Ceil', 1375, 32, 1, 0, 0, 1, 16048),
(213277, 0, 11884, 'Empress Chamber', 1375, 32, 7.83585, 0, 0, 1, 16048),
(212916, 0,  7530, 'Heart of Fear - Zor''lok - Arena Wall', 1375, 0, 2.25, 1, 0, 0, 16048),
(212943, 0,  7530, 'Heart of Fear - Zor''lok - Final Phase Doors', 1375, 0, 2.25, 1, 0, 0, 16048);

DELETE FROM creature WHERE id IN (62980, 69680);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES
('62980','1009','6297','6297','8','1','0','0','-2275.3','258.48','430.00','0.318021','120','0','0','1','0','0','0','0','0','0','0','0'),
('69680','1009','6297','6297','8','1','0','0','-2275.3','258.48','406.38','0.000000','120',0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);

-- 69680/NPC - Storm Spirit
DELETE FROM creature_template WHERE entry = 69680;
INSERT INTO creature_template (entry, NAME, Health_Mod, modelid1, minlevel, maxlevel, faction_A, faction_H, WDBVerified) VALUES
(69680, 'Storm Spirit', 0.095, 11686, 90, 90, 35, 35, 16048);

-- Map instance for Heart of Fear
UPDATE instance_template SET script = "instance_heart_of_fear" WHERE map = 1009;

-- Sonic Rings templates
UPDATE creature_template SET
gossip_menu_id = 0,
modelid1 = 11686,
minlevel = 93,
maxlevel = 93,
faction_A = 35,
faction_H = 35,
npcflag = 0,
speed_walk = 1,
speed_run = 0.7142857,
baseattacktime = 2000,
rangeattacktime = 2000,
unit_class = 0,
unit_flags = 33554432,
unit_flags2 = 67110912,
dynamicflags = 0,
VehicleId = 0,
HoverHeight = 1,
ScriptName = "mob_sonic_ring"
WHERE NAME = "Sonic Ring";

DELETE FROM spell_script_names WHERE spell_id IN (122852, 122440, 122761);
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES
(122852, 'spell_inhale'),
(122440, 'spell_attenuation'),
(122761, 'spell_exhale'),
(122706, 'spell_noise_cancelling'),
(122718, 'spell_force_verve'),
(122336, 'spell_sonic_ring'),
(124018, 'spell_pheromones_of_zeal');

-- NPC 62980 - Imperial Vizier Zor'lok
UPDATE creature_template SET
ScriptName = "boss_zorlok",
minlevel = 93,
maxlevel = 93,
Health_mod = 400,
unit_flags = 33024,
unit_flags2 = 2164736,
type_flags = 270532716,
baseattacktime = 2000,
InhabitType = 5
WHERE entry = 62980;

-- 62980/NPC - Zor'lok
UPDATE creature SET spawntimesecs = 7200 WHERE id = 62980;
-- NPC/62980 - Zorlok
UPDATE creature_template SET npcflag2 = 65536 WHERE entry = 62980;

DELETE FROM gameobject WHERE id IN (214442, 214634, 212695, 213276, 213277, 212916, 212943) OR id BETWEEN 214483 AND 214493;
INSERT INTO `gameobject`
(`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`) VALUES
(214442,  870, 6138, 6435, 1, 1, 164.597, 4069.01, 256.203, 4.88325, 0, 0, 0, 1, 7200, 255, 1, 0),
(214442, 1009, 6297, 6297, 8, 1, -2390.733, 459.7899, 422.3407, 0, 0, 0, 0, 1, 7200, 255, 1, 0),
(214483, 1009, 6297, 6297, 8, 1, -2307.711, 459.9609, 422.3357, 3.141593, 0, 0, 1, 0, 7200, 255, 1, 0),
(214484, 1009, 6297, 6297, 8, 1, -2252.5, 404.8451, 424.0067, 1.570796, 0, 0, 1, 0, 7200, 255, 1, 0),
(214485, 1009, 6297, 6297, 8, 1, -2252.5, 346.4307, 424.0067, 1.570796, 0, 0, 1, 0, 7200, 255, 1, 0),
(214486, 1009, 6297, 6297, 8, 1, -2187.895, 281.2121, 424.0067, 3.141593, 0, 0, 1, 0, 7200, 255, 1, 1),
(214487, 1009, 6297, 6297, 8, 1, -2119.144, 387.2309, 422.2571, 4.71239, 0, 0, 1, 0, 7200, 255, 1, 1),
(214488, 1009, 6297, 6297, 8, 1, -2035.252, 475.9384, 473.2037, 3.141593, 0, 0, 1, 0, 7200, 255, 1, 1),
(214489, 1009, 6297, 6297, 8, 1, -2163.632, 475.9384, 503.6568, 3.141593, 0, 0, 1, 0, 7200, 255, 1, 1),
(214490, 1009, 6297, 6297, 8, 1, -2387.138, 475.9781, 554.5891, 3.141593, 0, 0, 1, 0, 7200, 255, 1, 0),
(214491, 1009, 6297, 6297, 8, 1, -2478.975, 567.7377, 556.2987, 4.71239, 0, 0, 1, 0, 7200, 255, 1, 0),
(214492, 1009, 6297, 6297, 8, 1, -2478.806, 625.4995, 581.6339, 4.71239, 0, 0, 1, 0, 7200, 255, 1, 1),
(214493, 1009, 6297, 6297, 8, 1, -2478.58, 823.0958, 586.7272, 4.71239, 0, 0, 1, 0, 7200, 255, 1, 1);

-- GameObject Templates for doors

DELETE FROM gameobject_template WHERE entry IN (214634, 212695, 213276, 213277, 212916, 212943) OR entry BETWEEN 214483 AND 214493;
INSERT INTO gameobject_template (entry, TYPE, displayId, NAME, faction, flags, size, data0, data1, data9, WDBVerified) VALUES
(214483, 0, 11731, 'Doodad_Mantid_Door_017', 1375, 32, 1, 1, 5755148, 0, 16048),
(214484, 0, 11731, 'Doodad_Mantid_Door_020', 1375, 32, 0.954693, 1, 5755148, 0, 16048),
(214485, 0, 11731, 'Doodad_Mantid_Door_021', 1375, 32, 1, 1, 5755148, 0, 16048),
(214486, 0, 11731, 'Doodad_Mantid_Door_022', 1375, 32, 1, 1, 5755148, 0, 16048),
(214487, 0, 11731, 'Doodad_Mantid_Door_024', 1375, 32, 1, 1, 5755148, 0, 16048),
(214488, 0, 11731, 'Doodad_Mantid_Door_025', 1375, 32, 1, 1, 5755148, 0, 16048),
(214489, 0, 11731, 'Doodad_Mantid_Door_026', 1375, 32, 1, 1, 5755148, 0, 16048),
(214490, 0, 11731, 'Doodad_Mantid_Door_027', 1375, 32, 1, 1, 5755148, 0, 16048),
(214491, 0, 11731, 'Doodad_Mantid_Door_031', 1375, 32, 1, 1, 5755148, 0, 16048),
(214492, 0, 11731, 'Doodad_Mantid_Door_032', 1375, 32, 1, 1, 5755148, 0, 16048),
(214493, 0, 11731, 'Doodad_Mantid_Door_033', 1375, 32, 1, 1, 5755148, 0, 16048),
(214634, 0,  7530, 'Door of Garalon (up)', 114, 32, 2, 0, 0, 1, 16048),
(212695, 0,  7530, 'Door of Garalon', 114, 32, 2, 1, 0, 1, 16048),
(213276, 0, 11726, 'Mantid Queen Ceil', 1375, 32, 1, 0, 0, 1, 16048),
(213277, 0, 11884, 'Empress Chamber', 1375, 32, 7.83585, 0, 0, 1, 16048),
(212916, 0,  7530, 'Heart of Fear - Zor''lok - Arena Wall', 1375, 0, 2.25, 1, 0, 0, 16048),
(212943, 0,  7530, 'Heart of Fear - Zor''lok - Final Phase Doors', 1375, 0, 2.25, 1, 0, 0, 16048);

DELETE FROM spell_script_names WHERE spell_id IN (122852, 122440, 122761, 122706, 122718, 122336, 124018);
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES
(122852, 'spell_inhale'),
(122440, 'spell_attenuation'),
(122761, 'spell_exhale'),
(122706, 'spell_noise_cancelling'),
(122718, 'spell_force_verve'),
(122336, 'spell_sonic_ring'),
(124018, 'spell_pheromones_of_zeal');

-- 62980/NPC - Zor'lok
UPDATE creature_template SET unit_flags2 = 65536 WHERE entry = 62980;

-- 62980/NPC - Imperial Vizier Zor'lok
UPDATE creature_template SET
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 2,
baseattacktime = 2000,
rangeattacktime = 2000
WHERE entry = 62980;

-- Texts for Imperial Vizier Zor'lok
DELETE FROM creature_text WHERE entry = 62980;
INSERT INTO creature_text (entry, groupid, id, TEXT, TYPE, LANGUAGE, probability, emote, duration, sound, COMMENT) VALUES
(62980, 1, 0, 'The Divine chose us to give mortal voice to Her divine will. We are but the vessel that enacts Her will.', 14, 0, 100, 0, 0, 29301, 'VO_HOF_ZORLOK_AGGRO_01'),
(62980, 2, 0, 'We will not give in to the despair of the dark void. If Her will for us is to perish, then it shall be so.', 14, 0, 100, 0, 0, 29302, 'VO_HOF_ZORLOK_DEATH_01'),
(62980, 3, 0, 'We are the extension of our Empress''s will.', 14, 0, 100, 0, 0, 29303, 'VO_HOF_ZORLOK_EVENT_IDLE_1A'),
(62980, 4, 0, 'Ours is but to serve in Her divine name.', 14, 0, 100, 0, 0, 29304, 'VO_HOF_ZORLOK_EVENT_IDLE_1B'),
(62980, 5, 0, 'Never to question, nor to contemplate; we simply act.', 14, 0, 100, 0, 0, 29305, 'VO_HOF_ZORLOK_EVENT_IDLE_1C'),
(62980, 6, 0, 'We fight, toil, and serve so that Her vision for us is made reality.', 14, 0, 100, 0, 0, 29306, 'VO_HOF_ZORLOK_EVENT_IDLE_2'),
(62980, 7, 0, 'Her happiness is our reward, her sorrow our failure.', 14, 0, 100, 0, 0, 29307, 'VO_HOF_ZORLOK_EVENT_IDLE_3'),
(62980, 8, 0, 'We will give our lives for the Empress without hesitation. She is the light, and without Her our lives will be lost to darkness.', 14, 0, 100, 0, 0, 29308, 'VO_HOF_ZORLOK_EVENT_IDLE_4'),
(62980, 9, 0, 'Her gifts are many, our gratitude immeasurable.', 14, 0, 100, 0, 0, 29309, 'VO_HOF_ZORLOK_EVENT_PHASE1'),
(62980, 10, 0, 'She tests us. We will not disappoint Her.', 14, 0, 100, 0, 0, 29310, 'VO_HOF_ZORLOK_EVENT_PHASE2'),
(62980, 11, 0, 'We are as unwavering as the amber that shapes our halls. With Her might we will vanquish all who dare intrude upon Her divine home.', 14, 0, 100, 0, 0, 29311, 'VO_HOF_ZORLOK_EVENT_PHASE3'),
(62980, 12, 0, 'The chaff of the world tumbles across our doorstep, driven by fear; Her royal swarm will whisk them away.', 14, 0, 100, 0, 0, 29312, 'VO_HOF_ZORLOK_EVENT_TRASH_A_COMBAT'),
(62980, 13, 0, 'They were clearly unworthy of Her divine embrace.', 14, 0, 100, 0, 0, 29313, 'VO_HOF_ZORLOK_EVENT_TRASH_A_DIES'),
(62980, 14, 0, 'They are but the waves crashing upon the mountain of Her divine will. They may rise again and again; but will accomplish nothing.', 14, 0, 100, 0, 0, 29314, 'VO_HOF_ZORLOK_EVENT_TRASH_B_COMBAT'),
(62980, 15, 0, 'We are unfazed. We will stand firm.', 14, 0, 100, 0, 0, 29315, 'VO_HOF_ZORLOK_EVENT_TRASH_B_DIES'),
(62980, 16, 0, 'The Divine challenges us to face these intruders.', 14, 0, 100, 0, 0, 29316, 'VO_HOF_ZORLOK_EVENT_TRASH_C_COMBAT'),
(62980, 17, 0, 'And so it falls to us, Her chosen voice.', 14, 0, 100, 0, 0, 29317, 'VO_HOF_ZORLOK_EVENT_TRASH_C_DIES'),
(62980, 18, 0, 'The Divine will not suffer your taint here, outsider.', 14, 0, 100, 0, 0, 29318, 'VO_HOF_ZORLOK_SLAY_01'),
(62980, 19, 0, 'Ours is but to serve; yours is but to die.', 14, 0, 100, 0, 0, 29319, 'VO_HOF_ZORLOK_SLAY_02'),
(62980, 20, 0, 'Behold the voice of Her divine wrath.', 14, 0, 100, 0, 0, 29320, 'VO_HOF_ZORLOK_SPELL_EXHALE'),
(62980, 21, 0, 'Through the air we breathe, She strengthens our voice.', 14, 0, 100, 0, 0, 29321, 'VO_HOF_ZORLOK_SPELL_INHALE'),
(62980, 22, 0, 'Her will is greater than us all. You will join us or you will perish.', 14, 0, 100, 0, 0, 29322, 'VO_HOF_ZORLOK_SPELL_MINDCONTROL'),
(62980, 23, 0, 'Our voice gives form to Her will.', 14, 0, 100, 0, 0, 29323, 'VO_HOF_ZORLOK_SPELL_PITCH');

#62980/NPC - Imperial Vizier Zor'lok
DELETE FROM locales_creature_text WHERE entry = 62980;
INSERT INTO `locales_creature_text` VALUES
(62980, 1, 0, '', 'La Divine a choisi de donner une voix mortelle à Sa divine volonté. Nous ne sommes que l''instrument qui promulgue Sa volonté.', '', '', '', '', '', '', '', ''),
(62980, 2, 0, '', 'Nous ne nous laisserons pas aller au désespoir du vide. Si Sa volonté est de nous faire périr, il en sera ainsi.', '', '', '', '', '', '', '', ''),
(62980, 3, 0, '', 'Nous sommes le prolongement de la volonté impériale.', '', '', '', '', '', '', '', ''),
(62980, 4, 0, '', 'Nous ne faisons que servir en Son divin nom.', '', '', '', '', '', '', '', ''),
(62980, 5, 0, '', 'Sans réfléchir, sans remettre en question. Nous ne sommes qu''action.', '', '', '', '', '', '', '', ''),
(62980, 6, 0, '', 'Nous combattons, travaillons et servons pour que Sa vision devienne notre réalité.', '', '', '', '', '', '', '', ''),
(62980, 7, 0, '', 'Sa joie est notre récompense, Sa peine notre échec.', '', '', '', '', '', '', '', ''),
(62980, 8, 0, '', 'Nous offrons nos vies pour l''impératrice sans hésiter. Elle est la lumière, et sans elle nos vies seraient perdues dans les ténèbres.', '', '', '', '', '', '', '', ''),
(62980, 9, 0, '', 'Ses dons sont nombreux, notre gratitude incommensurable', '', '', '', '', '', '', '', ''),
(62980, 10, 0, '', 'Elle nous teste. Nous ne La décevrons pas.', '', '', '', '', '', '', '', ''),
(62980, 11, 0, '', 'Nous sommes aussi inébranlables que l''ambre qui forme nos murs. Avec Sa puissance, nous vaincrons quiconque ose s''introduire dans Sa divine demeure.', '', '', '', '', '', '', '', ''),
(62980, 12, 0, '', 'L''ivraie de ce monde s''effondre sur nos portes, poussée par la peur. Sa royale tempête les balayera.', '', '', '', '', '', '', '', ''),
(62980, 13, 0, '', 'Ils étaient clairement indignes de Sa divine étreinte.', '', '', '', '', '', '', '', ''),
(62980, 14, 0, '', 'Ils ne sont que les vagues qui s''écrasent sur la montagne de sa divine volonté. Ils reviennent encore et encore, mais n''arriveront jamais à rien.', '', '', '', '', '', '', '', ''),
(62980, 15, 0, '', 'Nous sommes impassibles. Nous serons fermes.', '', '', '', '', '', '', '', ''),
(62980, 16, 0, '', 'La Divine nous demande d''affronter ces intrus.', '', '', '', '', '', '', '', ''),
(62980, 17, 0, '', 'C''est donc à nous, Sa voix, que cela échoie.', '', '', '', '', '', '', '', ''),
(62980, 18, 0, '', 'La Divine ne tolère pas la corruption de votre présence ici.', '', '', '', '', '', '', '', ''),
(62980, 19, 0, '', 'Nous ne faisons que servir, vous ne faites que mourir.', '', '', '', '', '', '', '', ''),
(62980, 20, 0, '', 'Contemplez la voix de Sa colère divine.', '', '', '', '', '', '', '', ''),
(62980, 21, 0, '', 'Par l’air que nous respirons, Elle renforce notre voix.', '', '', '', '', '', '', '', ''),
(62980, 22, 0, '', 'Sa volonté est plus forte que nous tous. Vous allez nous rejoindre ou périr.', '', '', '', '', '', '', '', ''),
(62980, 23, 0, '', 'Notre voix donne forme à Sa volonté.', '', '', '', '', '', '', '', '');

#63030/NPC - Enslaved Bonesmasher
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_enslaved_bonesmasher' WHERE `entry` = 63030;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63030;
#63031/NPC - Set thik Fanatic
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_set_thik_fanatic' WHERE `entry` = 63031;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63031;
#63032/NPC - Sra thik Shield Master
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_sra_thik_shield_master' WHERE `entry` = 63032;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63032;
#63035/NPC - Zar Thik Zealot
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_zar_thik_zealot' WHERE `entry` = 63035;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63035;
#63036/NPC - Kor Thik Extremist
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_kor_thik_extremist' WHERE `entry` = 63036;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63036;
#63048/NPC - Kor Thik Slicer
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_kor_thik_slicer' WHERE `entry` = 63048;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63048;
#63049/NPC - Set thik Swiftblade
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_set_thik_swiftblade' WHERE `entry` = 63049;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63049;
#63592/NPC - Set Thik Gustwing
	UPDATE `creature_template` SET `minlevel` = 91, `maxlevel` = 91, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_set_thik_gustwing' WHERE `entry` = 64357;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63592;
#63593/NPC - Set thik Zephyrian
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_set_thik_zephyrian' WHERE `entry` = 63593;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63593;
#63597/NPC - Coagulated Amber
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_coagulated_amber' WHERE `entry` = 63597;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63597;
#63853/NPC - Zar thik Supplicant
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_zar_thik_supplicant' WHERE `entry` = 63853;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 63853;
#64338/NPC - Instructor Kli thak <Keeper of Wind Step>
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_instructor_kli_thak' WHERE `entry` = 64338;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 64338;
#64339/NPC - Instructor Tak thok <Keeper of Overwhelming Assault>
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_instructor_tak_thok' WHERE `entry` = 64339;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 64339;
#64353/NPC - Set thik Gale-Slicer
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_set_thik_gale_slicer' WHERE `entry` = 64353;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 64353;
#64357/NPC - Kor Thik Swarmer
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_kor_thik_swarmer' WHERE `entry` = 64357;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 64357;
#64358/NPC - Set thik Tempest
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_set_thik_tempest' WHERE `entry` = 64358;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 64358;
#66181/NPC - Zar thik Supplicant
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_zar_thik_supplicant' WHERE `entry` = 66181;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 66181;
#67177/NPC - Zar thik Supplicant
	UPDATE `creature_template` SET `minlevel` = 92, `maxlevel` = 92, `mindmg` = 4750, `maxdmg` = 11118, `attackpower` = 55538, `dmg_multiplier` = 3, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_zar_thik_supplicant' WHERE `entry` = 67177;
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 67177;
	
-- Blade Lord Ta'yak
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(62543, 1009, 6297, 6297, 8, 1, -2062.766, 281.373, 425.168, 3.141593, 7200, 5, 1);

-- 62980/NPC - Zor'lok
UPDATE creature_template SET mindmg = 43563, maxdmg = 69309 WHERE entry = 62980;