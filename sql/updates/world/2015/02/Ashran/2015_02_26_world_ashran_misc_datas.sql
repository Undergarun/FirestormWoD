DELETE FROM creature_text WHERE entry = 80858;
INSERT INTO creature_text VALUES
(80858, 0, 0, 'I have returned! Submit, peasants, to the might of the great Ogre Empire!', 14, 0, 100, 0, 0, 0, 'Awakening'),
(80858, 1, 0, 'Little blue warriors, you have proven yourself worthy of avoiding my wrath.', 14, 0, 100, 0, 0, 0, 'Recruited by Alliance'),
(80858, 2, 0, 'The red ones, yes, they will bear witness to my devastation!', 14, 0, 100, 0, 0, 0, 'Recruited by Horde'),
(80858, 3, 0, 'More, bring more to die at my blade!', 14, 0, 100, 0, 0, 0, 'Killing'),
(80858, 3, 1, 'You are a fool to oppose me weakling!', 14, 0, 100, 0, 0, 0, 'Killing'),
(80858, 3, 2, 'Bugs, you are bugs to me!', 14, 0, 100, 0, 0, 0, 'Killing'),
(80858, 3, 3, 'Ahahahaha!', 14, 0, 100, 0, 0, 0, 'Killing'),
(80858, 3, 4, 'Disconcerting ease!', 14, 0, 100, 0, 0, 0, 'Killing'),
(80858, 4, 0, 'The empire... will rise again!', 14, 0, 100, 0, 0, 0, 'Death');

DELETE FROM locales_creature_text WHERE entry = 80858;
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    80858, 0, 0,
    'Je suis de retour ! Pauvres gueux, soumettez-vous à la puissance de l’empire ogre !',
    'Ich bin zurückgekehrt! Bauern, unterwerft euch der Macht des großen Ogerimperiums!',
    '¡He vuelto! ¡Arrodillaos ante el poder del gran Imperio ogro!',
    'Я вернулся! Склонитесь, презренные, перед мощью великой империи огров!'
),
(
    80858, 1, 0,
    'Petits guerriers bleus, vous vous êtes montrés dignes d''échapper à mon courroux.',
    'Kleine blaue Krieger, Ihr habt euch bewährt und werdet verschont.',
    'Guerreros azules pequeños, que han demostrado que son dignos de evitar mi ira.',
    'Маленькие синие воины, вы заслужили честь избежать моего гнева.'
),
(
    80858, 2, 0,
    'Les rouges, oui, ils seront témoins de ma dévastation !',
    'Ihr Roten, ja, Ihr werdet Zeugen der Zerstörung!',
    'Los rojos, sí, ¡van a dar testimonio de mi devastación!',
    'Красные воины, я обрушу ярость на ваших врагов!'
),
(
    80858, 3, 0,
    'Plus d''ennemis ! Donnez-moi plus d''ennemis à égorger !',
    'Mehr, bringt noch mehr für meine Klinge!',
    '¡Más, traer más a morir en mi espada!',
    'Еще! Мой клинок жаждет еще крови!'
),
(
    80858, 3, 1,
    'Quelle folie de vous opposer à moi, gringalets !',
    'Schwächling, es ist dumm, Widerstand zu leisten!',
    '¡Eres un necio para enfrentarte a mí, debilucho!',
    'Глупцы! Вам ли сражаться со мной?'
),
(
    80858, 3, 2,
    'Des insectes, vous n''êtes que des insectes pour moi !',
    'Maden, nichts als Maden!',
    '¡Insectos, sois insectos para mí!',
    'Букашки, вы всего лишь букашки!'
),
(
    80858, 3, 3,
    'Ha ha ha ha !',
    'Ahahahaha!',
    '¡Ja, ja, ja, ja!',
    'А-ха-ха-ха-ха!'
),
(
    80858, 3, 4,
    'Une facilité déconcertante !',
    'Zu einfach.',
    '¡Una facilidad desconcertante!',
    'Слишком просто'
),
(
    80858, 4, 0,
    'L''empire... renaîtra !',
    'Das Imperium... wird wiederauferstehen!',
    '¡El... Imperio... renacerá!',
    'Империя… восстанет вновь!'
);

UPDATE creature_template SET faction = 2618, ScriptName = 'npc_ashran_korlok', mechanic_immune_mask = 617299839 WHERE entry = 80858;
UPDATE creature_template SET ScriptName = 'npc_ashran_faction_champions', mechanic_immune_mask = 617299839 WHERE entry IN (81725, 81726);

DELETE FROM trinity_string WHERE entry IN (14065, 14066, 14067);
INSERT INTO trinity_string (entry, content_default, content_loc2, content_loc6) VALUES
(14065, '|cFF0000FF+$1i Slain |r', '|cFF0000FF+$1i victime(s) |r', '|cFF0000FF+$1i asesinado |r'),
(14066, '|c00FFFF00|Hspell:178380|h[Kor''lok, the ogre king]|h|r is awakening!', '|c00FFFF00|Hspell:178380|h[Kor''lok, le roi des ogres]|h|r se réveille !', '¡|c00FFFF00|Hspell:178380|h[Kor''lok, el roy ogro]|h|r está despertando!'),
(14067, 'The |c00FFFF00|Hspell:178380|h[Event: Kor''lok, the ogre king]|h|r begins at the Throne of Kor''lok in 3 minutes.', 'L''|c00FFFF00|Hspell:178380|h[Évènement : Kor''lok, le roi des ogres]|h|r commence au trône de Kor''lok dans 3 minutes.', 'El |c00FFFF00|Hspell:178380|h[Evento : Kor''lok, el roy ogro]|h|r comienzará en el Trono de Kor''lok en 3 minutos.');

DELETE FROM spell_script_names WHERE ScriptName IN
(
    'spell_blade_twister',
    'spell_ashran_blade_twister',
    'spell_emberfall_living_bomb',
    'spell_ashran_emberfall_living_bomb',
    'spell_ashran_booming_shout',
    'spell_ashran_curse_of_krong'
);

INSERT INTO spell_script_names VALUES
(178795, 'spell_ashran_blade_twister'),
(176670, 'spell_ashran_emberfall_living_bomb'),
(177150, 'spell_ashran_booming_shout'),
(165134, 'spell_ashran_curse_of_krong');

DELETE FROM creature_equip_template WHERE entry IN (80858, 81725, 81726, 82883, 85811, 86201, 86202, 88771, 88772, 88774);
INSERT INTO creature_equip_template VALUES
(80858, 1, 116377, 0, 0),
(81725, 1, 110590, 0, 0),
(81726, 1, 110590, 0, 0),
(82883, 1, 47034, 0, 2550),
(85811, 1, 108942, 0, 23889),
(86201, 1, 29924, 29924, 0),
(86202, 1, 17105, 73468, 0),
(88771, 1, 110173, 110173, 2550),
(88772, 1, 110173, 110173, 2550),
(88774, 1, 107367, 0, 2550);

DELETE FROM gameobject WHERE id = 233282 AND map = 1191;
DELETE FROM creature WHERE id IN (85441, 88803) AND map = 1191;

DELETE FROM creature WHERE id IN (86201, 86202, 88679, 84223, 85509, 86940, 88697, 82974) AND map = 1191;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, curhealth, curmana, unit_flags2) VALUES
(86201, 1191, 6941, 7099, 1, 1, 1, 5098.05, -4100.28, 59.7991, 5.28609, 300, 1415988, 0, 2048),
(86202, 1191, 6941, 7099, 1, 1, 1, 5093.30, -4107.56, 59.7992, 0.035701, 300, 1417220, 96656, 2048),
(88679, 1191, 6941, 7100, 1, 1, 1, 3973.55, -4021.17, 55.7718, 4.68391, 300, 97854, 0, 2048),
(84223, 1191, 6941, 7100, 1, 1, 1, 3978.89, -4010.07, 55.3015, 4.73889, 300, 1012300, 0, 2048),
(85509, 1191, 6941, 7100, 1, 1, 1, 3994.82, -4016.98, 57.0982, 0.788465, 300, 587124, 0, 2048),
(86940, 1191, 6941, 7100, 1, 1, 0, 3990.59, -4012.25, 58.0998, 1.55016, 300, 40, 0, 2048),
(88697, 1191, 6941, 7100, 1, 1, 1, 3924.77, -4074.51, 66.8324, 0.104766, 300, 1417220, 0, 2048),
(82974, 1191, 6941, 7100, 1, 1, 1, 3913.74, -4098.84, 67.2971, 1.10615, 300, 1417220, 0, 2048),
(82974, 1191, 6941, 7100, 1, 1, 1, 3910.14, -4075.40, 75.804, 6.03216, 300, 1417220, 0, 2048);

DELETE FROM creature WHERE id = 85812 AND map = 1191 AND position_x = 3989.01 AND position_y =  -4030.99;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, curhealth, curmana, unit_flags2) VALUE
(85812, 1191, 6941, 7100, 1, 1, 1, 3989.01, -4030.99, 57.1706, 4.68091, 300, 2024600, 0, 2048);
DELETE FROM creature WHERE id = 85812 AND map = 1191 AND position_x = 3997.71 AND position_y =  -4031.23;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, curhealth, curmana, unit_flags2) VALUE
(85812, 1191, 6941, 7100, 1, 1, 1, 3997.71, -4031.23, 57.1706, 4.68091, 300, 2024600, 0, 2048);

UPDATE creature_template SET unit_flags = unit_flags & ~0x00200000 WHERE entry = 85812;

UPDATE creature, creature_equip_template SET creature.`equipment_id` = creature_equip_template.`id` WHERE
creature.`id` = creature_equip_template.`entry`;

UPDATE creature_template SET gossip_menu_id = 65536 WHERE entry = 82204;
UPDATE creature_template SET gossip_menu_id = 65537 WHERE entry = 83830;
UPDATE creature_template SET gossip_menu_id = 65539 WHERE entry = 83995;
UPDATE creature_template SET gossip_menu_id = 65540 WHERE entry = 83997;
UPDATE creature_template SET gossip_menu_id = 65538 WHERE entry = 83869;
UPDATE creature_template SET gossip_menu_id = 65541, ScriptName = 'npc_ashran_commander_anne_dunworthy' WHERE entry = 84173;
UPDATE creature_template SET gossip_menu_id = 65542, ScriptName = 'npc_ashran_general_ushet_wolfbarger' WHERE entry = 84473;
UPDATE creature_template SET gossip_menu_id = 65543 WHERE entry = 84660;
UPDATE creature_template SET gossip_menu_id = 65544 WHERE entry = 88826;
UPDATE creature_template SET gossip_menu_id = 65545 WHERE entry = 88448;
UPDATE creature_template SET gossip_menu_id = 65546 WHERE entry = 82200;
UPDATE creature_template SET gossip_menu_id = 65547, npcflag = 129 WHERE entry = 82883;
UPDATE creature SET npcflag = 129 WHERE id = 82883;
UPDATE creature_template SET gossip_menu_id = 65548 WHERE entry = 86201;
UPDATE creature_template SET gossip_menu_id = 65549 WHERE entry = 86202;
UPDATE creature_template SET gossip_menu_id = 65550 WHERE entry = 88777;
UPDATE creature_template SET gossip_menu_id = 65551 WHERE entry = 88778;
UPDATE creature_template SET ScriptName = 'npc_ashran_warspear_shaman' WHERE entry = 82438;
UPDATE creature_template SET ScriptName = 'npc_ashran_illandria_belore' WHERE entry = 88675;
UPDATE creature_template SET ScriptName = 'npc_ashran_examiner_rahm_flameheart' WHERE entry = 88676;
UPDATE creature_template SET ScriptName = 'npc_ashran_centurion_firescream' WHERE entry = 88771;
UPDATE creature_template SET ScriptName = 'npc_ashran_legionnaire_hellaxe' WHERE entry = 88772;
UPDATE creature_template SET ScriptName = 'npc_ashran_grimnir_sternhammer' WHERE entry = 88679;
UPDATE creature_template SET ScriptName = 'npc_ashran_misirin_stouttoe' WHERE entry = 88682;
UPDATE creature_template SET gossip_menu_id = 65578, ScriptName = 'npc_ashran_stormshield_druid' WHERE entry = 81887;
UPDATE creature_template SET ScriptName = 'npc_ashran_officer_rumsfeld' WHERE entry = 88696;
UPDATE creature_template SET ScriptName = 'npc_ashran_officer_ironore' WHERE entry = 88697;
UPDATE creature_template SET gossip_menu_id = 65560 WHERE entry = 82893;
UPDATE creature_template SET gossip_menu_id = 65563 WHERE entry = 81870;
UPDATE creature_template SET gossip_menu_id = 65566 WHERE entry = 82660;
UPDATE creature_template SET gossip_menu_id = 65569 WHERE entry = 82966;
UPDATE creature_template SET gossip_menu_id = 65572 WHERE entry = 82909;
UPDATE creature_template SET gossip_menu_id = 65573 WHERE entry = 82939;
UPDATE creature_template SET gossip_menu_id = 65574, npcflag = 1 WHERE entry = 85509;
UPDATE creature SET npcflag = 1 WHERE id = 85509;
UPDATE creature_template SET gossip_menu_id = 65575 WHERE entry = 86203;
UPDATE creature_template SET gossip_menu_id = 65576 WHERE entry = 86820;
UPDATE creature_template SET gossip_menu_id = 65577 WHERE entry = 85749;
UPDATE creature_template SET gossip_menu_id = 65579 WHERE entry = 85280;
UPDATE creature_template SET gossip_menu_id = 65580 WHERE entry = 85354;
UPDATE creature_template SET gossip_menu_id = 65581 WHERE entry = 85285;
UPDATE creature_template SET gossip_menu_id = 65582 WHERE entry = 85757;
UPDATE creature_template SET gossip_menu_id = 65583 WHERE entry = 83814;
UPDATE creature_template SET gossip_menu_id = 65584, npcflag = 1 WHERE entry = 80825;
UPDATE creature SET npcflag = 1 WHERE id = 80825;
UPDATE creature_template SET gossip_menu_id = 65585 WHERE entry = 81883;
UPDATE creature_template SET gossip_menu_id = 65586, npcflag = 3 WHERE entry = 84223;
UPDATE creature SET npcflag = 3 WHERE id = 84223;
UPDATE creature_template SET gossip_menu_id = 65587 WHERE entry = 88224;
UPDATE creature_template SET gossip_menu_id = 65588 WHERE entry = 88178;
UPDATE creature_template SET gossip_menu_id = 65589, npcflag = 3 WHERE entry = 86366;
UPDATE creature SET npcflag = 3 WHERE id = 86366;
UPDATE creature_template SET gossip_menu_id = 65590 WHERE entry = 86664;

ALTER TABLE `gossip_menu` CHANGE `entry` `entry` INT(10) UNSIGNED DEFAULT '0' NOT NULL, CHANGE `text_id` `text_id` INT(10) UNSIGNED DEFAULT '0' NOT NULL;
DELETE FROM gossip_menu WHERE entry IN (65536, 65537, 65538, 65539, 65540, 65541, 65542, 65543, 65544, 65545, 65546, 65547, 65548, 65549, 65550, 65551, 65552, 65553, 65554, 65555, 65556, 65557, 65558, 65559, 65560, 65561, 65562, 65563, 65564, 65565);
DELETE FROM gossip_menu WHERE entry IN (65566, 65567, 65568, 65569, 65570, 65571, 65572, 65573, 65574, 65575, 65576, 65577, 65578, 65579, 65580, 65581, 65582, 65583, 65584, 65585, 65586, 65587, 65588, 65589, 65590);
DELETE FROM gossip_menu WHERE text_id IN (89855, 85290, 85346, 85455, 85464, 85651, 86045, 84857, 91496, 82999, 83825, 87402, 88550, 88548, 91432, 91435, 85343, 85461, 85460, 85334, 84631, 84476, 85336, 86036, 84924, 85339, 84928, 83896);
DELETE FROM gossip_menu WHERE text_id IN (84627, 85341, 84642, 84922, 84926, 90226, 85108, 87228, 87226, 88551, 88743, 87224, 83826, 86756, 86864, 86758, 87229, 86207, 87162, 90534, 90385, 89897);
INSERT INTO gossip_menu (entry, text_id) VALUES
(65536, 89855),
(65537, 85290),
(65538, 85346),
(65539, 85455),
(65540, 85464),
(65541, 85651),
(65542, 86045),
(65543, 84857),
(65544, 91496),
(65545, 82999),
(65546, 83825),
(65547, 87402),
(65548, 88550),
(65549, 88548),
(65550, 91432),
(65551, 91435),
-- Kronus Infos
(65552, 85343),
-- Warlock Gateways Infos
(65553, 85461),
-- Mage Portals Infos
(65554, 85460),
-- Wolf Riders Infos
(65555, 85334),
-- Shaman artifacts
(65556, 84631),
-- Warlock artifacts
(65557, 84476),
-- Mage artifacts
(65558, 85336),
-- Warrior artifacts
(65559, 86036),
(65560, 84924),
-- Paladin artifacts
(65561, 85339),
-- Paladin Infos
(65562, 84928),
(65563, 83896),
-- Druid artifacts
(65564, 85340),
-- Druid Infos
(65565, 84624),
(65566, 84627),
-- Warlock artifacts
(65567, 85341),
-- Warlock Infos
(65568, 84642),
(65569, 84922),
-- Mage artifacts
(65570, 84926),
-- Mage Infos
(65571, 90226),
(65572, 85108),
(65573, 87228),
(65574, 87226),
(65575, 88551),
(65576, 88743),
(65577, 87224),
(65578, 83826),
(65579, 86756),
(65580, 86864),
(65581, 86758),
(65582, 87229),
(65583, 86207),
(65584, 87162),
(65585, 83825),
(65586, 82999),
(65587, 90534),
(65588, 90385),
(65589, 87225),
(65590, 89897);

ALTER TABLE `gossip_menu_option` CHANGE `menu_id` `menu_id` INT(10) UNSIGNED DEFAULT '0' NOT NULL, CHANGE `id` `id` SMALLINT(5) UNSIGNED DEFAULT '0' NOT NULL;
DELETE FROM gossip_menu_option WHERE menu_id IN (65536, 65537, 65539, 65540, 65543, 65545, 65546, 65547, 65560, 65563, 65566, 65569, 65586, 65587, 65588);
INSERT INTO gossip_menu_option VALUES
-- Shaman
(65536, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65536, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65556, 0, 0, 0, ''),
(65536, 2, 0, 'What does Kronus do?', 1, 3, 65552, 0, 0, 0, ''),
-- Warlock
(65540, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65540, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65557, 0, 0, 0, ''),
(65540, 2, 0, 'What do the Warlock Gateways do?', 1, 3, 65553, 0, 0, 0, ''),
-- Mage
(65539, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65539, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65558, 0, 0, 0, ''),
(65539, 2, 0, 'What do the Mage Portals do?', 1, 3, 65554, 0, 0, 0, ''),
-- Warrior
(65537, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65537, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65559, 0, 0, 0, ''),
(65537, 2, 0, 'What do the Wolf Riders do?', 1, 3, 65555, 0, 0, 0, ''),
-- Farseer
(65543, 0, 0, 'Show me Brute''s Rise.', 1, 1, 0, 0, 0, 0, ''),
(65543, 1, 0, 'Show me the Molten Quarry.', 1, 1, 0, 0, 0, 0, ''),
(65543, 2, 0, 'Show me the Amphitheater Of Annihilation.', 1, 1, 0, 0, 0, 0, ''),
(65543, 3, 0, 'Show me the Ashmaul Burial Grounds.', 1, 1, 0, 0, 0, 0, ''),
-- Belloc
(65586, 0, 0, 'Take all of my Artifact Fragments!', 1, 1, 0, 0, 0, 0, ''),
-- Harrison Jones
(65545, 0, 0, 'Take all of my Artifact Fragments!', 1, 1, 0, 0, 0, 0, ''),
-- Honor Quartermarster
(65547, 0, 1, 'I would like to buy from you.', 3, 128, 0, 0, 0, 0, ''),
-- Paladin
(65560, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65560, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65561, 0, 0, 0, ''),
(65560, 2, 0, 'What do the Knight Riders do?', 1, 3, 65562, 0, 0, 0, ''),
-- Druid
(65563, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65563, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65564, 0, 0, 0, ''),
(65563, 2, 0, 'What does the Ancient of War do?', 1, 3, 65565, 0, 0, 0, ''),
-- Warlock
(65566, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65566, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65567, 0, 0, 0, ''),
(65566, 2, 0, 'What do the Gateways do?', 1, 3, 65568, 0, 0, 0, ''),
-- Mage
(65569, 0, 0, 'Take all of my Artifact Fragments!', 1, 3, 0, 0, 0, 0, ''),
(65569, 1, 0, 'How many Artifact Fragments have we collected?', 1, 3, 65570, 0, 0, 0, ''),
(65569, 2, 0, 'What do the Mage Portals do?', 1, 3, 65571, 0, 0, 0, ''),
-- Rylai Crestfall
(65587, 0, 0, 'Could you conjure me some food?', 1, 3, 0, 0, 0, 0, ''),
-- Jeron Emberfall
(65588, 0, 0, 'Could you conjure me some food?', 1, 3, 0, 0, 0, 0, '');

ALTER TABLE `locales_gossip_menu_option` CHANGE `menu_id` `menu_id` INT(10) UNSIGNED DEFAULT '0' NOT NULL;
DELETE FROM locales_gossip_menu_option WHERE menu_id IN (65536, 65537, 65539, 65540, 65543, 65545, 65547, 65560, 65563, 65566, 65569, 65586, 65587, 65588);
--                                                   French            German            Spanish           Russian
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc2, option_text_loc3, option_text_loc6, option_text_loc8) VALUES
(
    65536, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65536, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65536, 2,
    'À quoi sert Kronus ?',
    '',
    '¿A qué se dedica Kronus?',
    ''
),
(
    65540, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65540, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65540, 2,
    'À quoi servent les portes de démoniste ?',
    '',
    '¿Qué hacen los portales de brujo?',
    ''
),
(
    65539, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65539, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65539, 2,
    'À quoi servent les portails de mage ?',
    '',
    '¿Qué hacen los portales de mago?',
    ''
),
(
    65537, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65537, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65537, 2,
    'Quel est le rôle des Chevaucheurs de loups ?',
    '',
    '¿Qué hacen los Caballeros Lobo?',
    ''
),
(
    65543, 0,
    'Montrez-moi la cime de la Brute.',
    '',
    'Muéstrame el Alto del Bruto.',
    ''
),
(
    65543, 1,
    'Montrez-moi la carrière de la Fournaise.',
    '',
    'Muéstrame la Cantera de magma.',
    ''
),
(
    65543, 2,
    'Montrez-moi l''amphitéâtre de l''Annihilation.',
    '',
    'Muéstrame el Anfiteatro de la Aniquilación.',
    ''
),
(
    65543, 3,
    'Montrez-moi le cimetière des Cognecendres.',
    '',
    'Muéstrame los Cementerios de Machacacenizas.',
    ''
),
(
    65545, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65586, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65547, 0,
    'Je voudrais vous acheter quelque chose.',
    '',
    'Me gustaría comprar algo.',
    ''
),
(
    65560, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65560, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65560, 2,
    'Quel est le rôle des Chevaucheurs ?',
    '',
    '¿Qué hacen los jinetes Caballeros?',
    ''
),
(
    65563, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65563, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65563, 2,
    'À quoi sert l''ancien de la guerre ?',
    '',
    '¿Qué hace el Anciano de la Guerra?',
    ''
),
(
    65566, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65566, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65566, 2,
    'À quoi servent les portes ?',
    '',
    '¿Qué hacen los portales?',
    ''
),
(
    65569, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '¡Llévate todos mis fragmentos de artefacto!',
    ''
),
(
    65569, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '¿Cuántos fragmentos de artefacto hemos recolectado?',
    ''
),
(
    65569, 2,
    'À quoi servent les portails de mage ?',
    '',
    '¿Qué hacen los portales de mago?',
    ''
),
(
    65587, 0,
    'Pourriez-vous m''invoquer de la nourriture ?',
    '',
    '¿Podrías conjurarme algo de comer?',
    ''
),
(
    65588, 0,
    'Pourriez-vous m''invoquer de la nourriture ?',
    '',
    '¿Podrías conjurarme algo de comer?',
    ''
);

-- Basic text from horde NPC's, base gossips
DELETE FROM npc_text WHERE ID IN (82999, 83825, 84857, 85290, 85346, 85455, 85464, 86045, 87402, 88548, 88550, 89855, 91432, 91496, 91435, 85343, 85461, 85460, 85334, 84631, 84924, 84928, 83896, 84624, 84627, 84642, 84922, 90226);
DELETE FROM npc_text WHERE ID IN (85651, 85108, 87228, 87226, 88551, 88743, 87224, 83826, 86756, 86864, 86758, 87229, 86207, 87162, 90534, 90385, 87225, 89897);
INSERT INTO npc_text (ID, text0_0, text0_1) VALUES
(
    82999,
    'Have you seen any of the hidden artifacts?$B$BYou can find fragments all throughout Ashran. I hear killing the creatures in this the outskirts off of the Road of Glory has uncovered some recently.$B$BYou can give me any you find, and I''ll distribute them to those who need them.',
    'Have you seen any of the hidden artifacts?$B$BYou can find fragments all throughout Ashran. I hear killing the creatures in this the outskirts off of the Road of Glory has uncovered some recently.$B$BYou can give me any you find, and I''ll distribute them to those who need them.'
),
(
    83825,
    '<Let''s out a soft, deep groan>',
    '<Let''s out a soft, deep groan>'
),
(
    84857,
    'Look deep into my eyes, $n and you can survey Ashran from afar.$B$BWhat would you like to see?',
    'Look deep into my eyes, $n and you can survey Ashran from afar.$B$BWhat would you like to see?'
),
(
    85290,
    'Bring me back enough of those artifacts and I can gear up our Wolf Riders and send them out into battle.$B$BHow many have you brought me?',
    'Bring me back enough of those artifacts and I can gear up our Wolf Riders and send them out into battle.$B$BHow many have you brought me?'
),
(
    85346,
    'WHAT ARE YOU DOING JUST STANDING AROUND? GO SLAY SOME ALLIANCE SCUM. BRING ME BACK THEIR HEAD, TEETH, TAILS... WHATHEVER! AND HURRY UP!!',
    'WHAT ARE YOU DOING JUST STANDING AROUND? GO SLAY SOME ALLIANCE SCUM. BRING ME BACK THEIR HEAD, TEETH, TAILS... WHATHEVER! AND HURRY UP!!'
),
(
    85455,
    'The power lies within the artifact fragments, $n. Bring back enough of those and the mages will summon a portal so we can return back to Warspear Outpost faster.$B$BHow many have you brought me?',
    'The power lies within the artifact fragments, $n. Bring back enough of those and the mages will summon a portal so we can return back to Warspear Outpost faster.$B$BHow many have you brought me?'
),
(
    85464,
    'If we can gather enough of the artifact fragments found within Ashran we can activate the Warlock Gateways so we can move between the areas faster.$B$BHow many have you brought me?',
    'If we can gather enough of the artifact fragments found within Ashran we can activate the Warlock Gateways so we can move between the areas faster.$B$BHow many have you brought me?'
),
(
    86045,
    'So how many Alliance have you slain today, $n?',
    'So how many Alliance have you slain today, $n?'
),
(
    87225,
    'Someone just showed me a Tuber root they found somewhere in east of Ashran.$B$BThey rival my healing potions in power!',
    'Someone just showed me a Tuber root they found somewhere in east of Ashran.$B$BThey rival my healing potions in power!'
),
(
    88548,
    'GO!',
    'GO!'
),
(
    88550,
    'I come and go like the wind.',
    'I come and go like the wind.'
),
(
    91496,
    'If you run into some trouble with alliance around here, use one of Frizzo Sparkrocket''s flare.$B$BWe can''t help you if we don''t know where you''re at.',
    'If you run into some trouble with alliance around here, use one of Frizzo Sparkrocket''s flare.$B$BWe can''t help you if we don''t know where you''re at.'
),
(
    91432,
    'The Night Elves are attempting to resurrect an Ancient to be used against us using the power of the Artifacts found here in Ashran.$B$BIf you see any alliance out there, take any fragments they carry by any force necessary.',
    'The Night Elves are attempting to resurrect an Ancient to be used against us using the power of the Artifacts found here in Ashran.$B$BIf you see any alliance out there, take any fragments they carry by any force necessary.'
),
(
    87402,
    'I hear you''re a force to be reckoned with in the arena, $n.',
    'I hear you''re a force to be reckoned with in the arena, $n.'
),
(
    89855,
    'The artifact fragments contain enough magical power to restore this Earth Fury to full strength.$B$BHow many have you brouht us?',
    'The artifact fragments contain enough magical power to restore this Earth Fury to full strength.$B$BHow many have you brouht us?'
),
(
    91435,
    'The flames coming out of the braziers near the Ring of Conquest may look ordinary at first glance. However, I believe they have been enchanted by ancient Highmaul magic.$B$BI was strangely drawn to them. Touching one, I was suddenly filled with magical energy.',
    'The flames coming out of the braziers near the Ring of Conquest may look ordinary at first glance. However, I believe they have been enchanted by ancient Highmaul magic.$B$BI was strangely drawn to them. Touching one, I was suddenly filled with magical energy.'
),
(
    85343,
    'Kronus will patrol into the Road of Glory and clear it of any Alliance it finds.',
    'Kronus will patrol into the Road of Glory and clear it of any Alliance it finds.'
),
(
    85461,
    'The Gateways are located in the North-West and South-East and can be used to move between those two points.',
    'The Gateways are located in the North-West and South-East and can be used to move between those two points.'
),
(
    85460,
    'The mages will summon a portal west of Ashran which can be used to take you back to Warspear Outpost.',
    'The mages will summon a portal west of Ashran which can be used to take you back to Warspear Outpost.'
),
(
    85334,
    'Our best Wolf Riders will ride around Ashran to protect and serve. Tear those Alliance dogs to shreds.',
    'Our best Wolf Riders will ride around Ashran to protect and serve. Tear those Alliance dogs to shreds.'
),
(
    84924,
    'Bring me back enough of those artifacts and I can gear up our Knights and send them out into the battle.$B$BHow many have you brought me?',
    'Bring me back enough of those artifacts and I can gear up our Knights and send them out into the battle.$B$BHow many have you brought me?'
),
(
    84928,
    'Our best Knight Riders will ride around Ashran to protect and serve. You know, ruin the Horde''s day.',
    'Our best Knight Riders will ride around Ashran to protect and serve. You know, ruin the Horde''s day.'
),
(
    83896,
    'The artifact fragments contain enough magical power to restore this Ancient to full strength.$B$BHow many have you brought us?',
    'The artifact fragments contain enough magical power to restore this Ancient to full strength.$B$BHow many have you brought us?'
),
(
    84624,
    'Fangraal will march into the Road of Glory and clear it of any Horde it finds.',
    'Fangraal will march into the Road of Glory and clear it of any Horde it finds.'
),
(
    84627,
    'The Warlocks will erect Gateways so we can move around Ashran faster. We just need the magical power from those scattered artifact pieces found around Ashran.$B$BHow many have you brought me?',
    'The Warlocks will erect Gateways so we can move around Ashran faster. We just need the magical power from those scattered artifact pieces found around Ashran.$B$BHow many have you brought me?'
),
(
    84642,
    'The Gateways activate at the South-East and North-West of Ashran. Use them to teleport between the two points.',
    'The Gateways activate at the South-East and North-West of Ashran. Use them to teleport between the two points.'
),
(
    84922,
    'If you bring us enough of those artifact pieces scattered around Ashran we can use their power to conjure a Mage Porta.$B$BHow many have you brought me?',
    'If you bring us enough of those artifact pieces scattered around Ashran we can use their power to conjure a Mage Porta.$B$BHow many have you brought me?'
),
(
    90226,
    'The mages will summon a portal west of Ashran which can be used to take you back to Stormshield Stronghold.',
    'The mages will summon a portal west of Ashran which can be used to take you back to Stormshield Stronghold.'
),
(
    85651,
    'I don''t want to say we were unprepared, but we are short on supplies here in Ashran. We just didn''t bring enough through the portal.$B$BHopefully Sprocket''s team can get us the heavy machinery we need to win this battle against the Horde.',
    'I don''t want to say we were unprepared, but we are short on supplies here in Ashran. We just didn''t bring enough through the portal.$B$BHopefully Sprocket''s team can get us the heavy machinery we need to win this battle against the Horde.'
),
(
    85108,
    'Don''t go easy on the horde, $n. Bring me back a token of their demise and I''ll reward you handsomely.',
    'Don''t go easy on the horde, $n. Bring me back a token of their demise and I''ll reward you handsomely.'
),
(
    87228,
    'Healers are truly underestimated, $n. If more would choose to bring us along on their missions there would be less here in my relief hut.',
    'Healers are truly underestimated, $n. If more would choose to bring us along on their missions there would be less here in my relief hut.'
),
(
    87226,
    'Are you here to visit one of the wounded, $n?',
    'Are you here to visit one of the wounded, $n?'
),
(
    88551,
    'Tending the horses is not as easy as it seems, but at least you get to enjoy the outdoors.',
    'Tending the horses is not as easy as it seems, but at least you get to enjoy the outdoors.'
),
(
    88743,
    'Have any spare Artifact Fragments, $n? My friend Ecilam could use them.',
    'Have any spare Artifact Fragments, $n? My friend Ecilam could use them.'
),
(
    87224,
    'My Phantom Potions are perfect for sneaking around the Horde guards, or event into their main base! Have you tried them yet, $n?',
    'My Phantom Potions are perfect for sneaking around the Horde guards, or event into their main base! Have you tried them yet, $n?'
),
(
    83826,
    'Our power is not strong enough to revive this ancient, but we''ll keep trying. Are you going to help us?',
    'Our power is not strong enough to revive this ancient, but we''ll keep trying. Are you going to help us?'
),
(
    86756,
    'I was almost back to base with a sack of those artifact fragments when some Horde stole them from me. Watch your back at all times, $n.',
    'I was almost back to base with a sack of those artifact fragments when some Horde stole them from me. Watch your back at all times, $n.'
),
(
    86864,
    'I wonder what Chris Clarkie does with all those Troll Feet people bring him.',
    'I wonder what Chris Clarkie does with all those Troll Feet people bring him.'
),
(
    86758,
    'How''d you get up here? Harrison Jones didn''t mention this adventure would be so... dangerous.',
    'How''d you get up here? Harrison Jones didn''t mention this adventure would be so... dangerous.'
),
(
    87229,
    'Have you seen Feezzel? I can''t find him anywhere.',
    'Have you seen Feezzel? I can''t find him anywhere.'
),
(
    86207,
    'The mine entrance is boarded up while work goes on inside.',
    'The mine entrance is boarded up while work goes on inside.'
),
(
    87162,
    'We aren''t the only ones with Prisoners, $n.$B$BOver at the Horde base lies their prison, underneath the relief hut to the left when you go through their main gate.$B$BIf we can find some of those Warspear Prison Keys in Ashran, we can break them free.',
    'We aren''t the only ones with Prisoners, $n.$B$BOver at the Horde base lies their prison, underneath the relief hut to the left when you go through their main gate.$B$BIf we can find some of those Warspear Prison Keys in Ashran, we can break them free.'
),
(
    90534,
    'It''s a bit warm out today, don''t you think?',
    'It''s a bit warm out today, don''t you think?'
),
(
    90385,
    'The cold certainly bothers you.',
    'The cold certainly bothers you.'
),
(
    89897,
    'We aren''t the only onles with Prisoners, $c.$B$BOver at the Alliance base, undemeath their main building lies their prison.$B$BWe just need to find those Stormshield Prison Keys to break them out.',
    'We aren''t the only onles with Prisoners, $c.$B$BOver at the Alliance base, undemeath their main building lies their prison.$B$BWe just need to find those Stormshield Prison Keys to break them out.'
);

-- Basic text from horde NPC's, base gossips
DELETE FROM locales_npc_text WHERE entry IN (82999, 83825, 84857, 85290, 85346, 85455, 85464, 86045, 87402, 88548, 88550, 89855, 91432, 91496, 91435, 85343, 85461, 85460, 85334, 84924, 84928, 83896, 84624, 84627, 84642, 84922, 90226);
DELETE FROM locales_npc_text WHERE entry IN (85651, 85108, 87228, 87226, 88551, 88743, 87224, 83826, 86756, 86864, 86758, 87229, 86207, 87162, 90534, 90385, 87225, 89897);
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUES
(
    82999,
    'Avez-vous déniché l''un de ces artéfacts cachés ?$B$BOn trouve des fragments un peu partout dans A''shran. J''ai entendu dire qu''on avait récemment découvert en tuant des créatures rôdant à proximité de la route de la Gloire.$B$BVous pouvez me remettre ceux que vous trouverez, et je me chargerai de les redistribuer aux personnes qui en ont besoin.',
    'Avez-vous déniché l''un de ces artéfacts cachés ?$B$BOn trouve des fragments un peu partout dans A''shran. J''ai entendu dire qu''on avait récemment découvert en tuant des créatures rôdant à proximité de la route de la Gloire.$B$BVous pouvez me remettre ceux que vous trouverez, et je me chargerai de les redistribuer aux personnes qui en ont besoin.',
    '', '',
    '¿Viste alguno de los artefactos ocultos?$B$BPuedes encontrar fragmentos por todo Ashran. Escuché que matar a las criaturas en la periferia del Camino de Gloria dejó al descubierto algunos recientemente.$B$BPuedes darme los que encuentres y yo los entregaré a.',
    '¿Viste alguno de los artefactos ocultos?$B$BPuedes encontrar fragmentos por todo Ashran. Escuché que matar a las criaturas en la periferia del Camino de Gloria dejó al descubierto algunos recientemente.$B$BPuedes darme los que encuentres y yo los entregaré a.',
    '', ''
),
(
    83825,
    '<Pousse un léger gémissement gutural.>',
    '<Pousse un léger gémissement gutural.>',
    '', '',
    '<Suelta un gemido suave y profundo>',
    '<Suelta un gemido suave y profundo>',
    '', ''
),
(
    84857,
    'Plongez votre regard dans le mien $n. Vous pourrez ainsi observer à distance ce qui se passe à A''shran.$B$BQue voulez-vous voir ?',
    'Plongez votre regard dans le mien $n. Vous pourrez ainsi observer à distance ce qui se passe à A''shran.$B$BQue voulez-vous voir ?',
    '', '',
    'Veme detenidamente a los ojos, $n, y podrás sondear Ashran desde lejos.$B$B¿Qué te gustaría ver?',
    'Veme detenidamente a los ojos, $n, y podrás sondear Ashran desde lejos.$B$B¿Qué te gustaría ver?',
    '', ''
),
(
    85290,
    'J''ai besoin de beaucoup d''artéfacts pour équiper nos Chevaucheurs de loups et les envoyer au combat.$B$BCombien m''en apportez-vous ?',
    'J''ai besoin de beaucoup d''artéfacts pour équiper nos Chevaucheurs de loups et les envoyer au combat.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Tráeme suficientes de esos artefactos y podré equipar a nuestros caballeros lobo para enviarlos a la batalla.$B$B¿Cuántos me trajiste?',
    'Tráeme suficientes de esos artefactos y podré equipar a nuestros caballeros lobo para enviarlos a la batalla.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    85346,
    'VOUS COMPTEZ PRENDRE RACINE ICI OU QUOI ? ALLEZ ME TUER LES VERMINES DE L''ALLIANCE. RAPPORTEZ-MOI DES TÊTES, DES DENTS, DES QUEUES... PEU IMPORTE ! ET GROUILLEZ-VOUS !!',
    'VOUS COMPTEZ PRENDRE RACINE ICI OU QUOI ? ALLEZ ME TUER LES VERMINES DE L''ALLIANCE. RAPPORTEZ-MOI DES TÊTES, DES DENTS, DES QUEUES... PEU IMPORTE ! ET GROUILLEZ-VOUS !!',
    '', '',
    '¿QUÉ HACES AHÍ SIN HACER NADA? VE A MATAR ALGUNAS ESCORIAS DE LA ALLIANZA. TRÁEME SUS CABEZAS, DIENTES, COLAS... ¡LO QUE SEA! ¡VE, AHORA!',
    '¿QUÉ HACES AHÍ SIN HACER NADA? VE A MATAR ALGUNAS ESCORIAS DE LA ALLIANZA. TRÁEME SUS CABEZAS, DIENTES, COLAS... ¡LO QUE SEA! ¡VE, AHORA!',
    '', ''
),
(
    85455,
    'L''énergie dons nous avons besoin se trouve dans les fragments d''artéfacts, $n. En quantité suffisante, ils permettront aux mages d''invoquer un portail pour atteindre l''avant-poste de Fer-de-Lance plus rapidement.$B$BCombien m''en apportez-vous ?',
    'L''énergie dons nous avons besoin se trouve dans les fragments d''artéfacts, $n. En quantité suffisante, ils permettront aux mages d''invoquer un portail pour atteindre l''avant-poste de Fer-de-Lance plus rapidement.$B$BCombien m''en apportez-vous ?',
    '', '',
    'El poder yace en los fragmentos del artefacto, $n. Regresa los suficientes y los magos invocarán los portales para poder regresar más rápido a Lanza de guerra.$B$B¿Cuántos me trajiste?',
    'El poder yace en los fragmentos del artefacto, $n. Regresa los suficientes y los magos invocarán los portales para poder regresar más rápido a Lanza de guerra.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    85464,
    'Si nous parvenons à nous procurer assez de fragments d''artéfacts qu''on peut trouver à A''shran, nous pourrons activer les portes de déùpnostres pour nous déplacer plus rapidement d''un point à un autre.$B$BCombien m''en apportez-vous ?',
    'Si nous parvenons à nous procurer assez de fragments d''artéfacts qu''on peut trouver à A''shran, nous pourrons activer les portes de déùpnostres pour nous déplacer plus rapidement d''un point à un autre.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Si podemos recolectar suficientes de los fragmentos de artefacto hallados en Ashran, podemos activar los portales de brujo para movernos más rápido entre áreas.$B$B¿Cuántos me trajiste?',
    'Si podemos recolectar suficientes de los fragmentos de artefacto hallados en Ashran, podemos activar los portales de brujo para movernos más rápido entre áreas.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    86045,
    'Alors, combien de soldats de l''Alliance avez-vous tués ajourd''hui, $n ?',
    'Alors, combien de soldats de l''Alliance avez-vous tués ajourd''hui, $n ?',
    '', '',
    '¿A cuántos de la Alianza mataste hoy, $n?',
    '¿A cuántos de la Alianza mataste hoy, $n?',
    '', ''
),
(
    87225,
    'Quelqu''un m''a montré un tubercule découvert dans la partie est d''A''shran. $B$BSes vertus médicinales égalent celles de mes potions de soins !',
    'Quelqu''un m''a montré un tubercule découvert dans la partie est d''A''shran. $B$BSes vertus médicinales égalent celles de mes potions de soins !',
    '', '',
    'Alguien me mostró una raíz de tubérculo que encontraron en algún lugar al este de Ashran.$B$B¡Rivaliza en poder con mis pociones de sanació!',
    'Alguien me mostró una raíz de tubérculo que encontraron en algún lugar al este de Ashran.$B$B¡Rivaliza en poder con mis pociones de sanació!',
    '', ''
),
(
    88550,
    'Je tranche la gorge de n''importe quel membre de l''Alliance pour une pièce de cuivre.',
    'Je tranche la gorge de n''importe quel membre de l''Alliance pour une pièce de cuivre.',
    '', '',
    'Voy y vengo como el viento.',
    'Voy y vengo como el viento.',
    '', ''
),
(
    88548,
    'PARTEZ !', 'PARTEZ !',
    '', '',
    '¡VAMOS!', '¡VAMOS!',
    '', ''
),
(
    91432,
    'Si vous vous rendez dans le bois Sombre, essayez de cueillir des fleurs des chants. Elles ont de puissantes propriétés magiques. Notez néanmoins que vous devrez peut-être combattre quelques arakkoa pour les obtenir.',
    'Si vous vous rendez dans le bois Sombre, essayez de cueillir des fleurs des chants. Elles ont de puissantes propriétés magiques. Notez néanmoins que vous devrez peut-être combattre quelques arakkoa pour les obtenir.',
    '', '',
    'Los elfos de la noche intentan resucitar a un Anciano para usarlo contra nosotros, y utilizan el poder de los artefactos hallados en Ashran.$B$BSi ves ferzas de la Alianza aquí, captura por cualquier medio necesario los fragmentos que lleven.',
    'Los elfos de la noche intentan resucitar a un Anciano para usarlo contra nosotros, y utilizan el poder de los artefactos hallados en Ashran.$B$BSi ves ferzas de la Alianza aquí, captura por cualquier medio necesario los fragmentos que lleven.',
    '', ''
),
(
    91496,
    'Si l''Alliance vous pose des problèmes dans le coin, utilisez l''une des fusées de Frizzo Grilletorpille.$B$BNous ne pourrons vous aider que si nous savons où vous vous trouvez.',
    'Si l''Alliance vous pose des problèmes dans le coin, utilisez l''une des fusées de Frizzo Grilletorpille.$B$BNous ne pourrons vous aider que si nous savons où vous vous trouvez.',
    '', '',
    'Si te encuentras con la Alianza por aquí, usa una de las bengalas de Frizzo Chispamisil.$B$BNo podemos ayudarte si no sabemos dónde estás.',
    'Si te encuentras con la Alianza por aquí, usa una de las bengalas de Frizzo Chispamisil.$B$BNo podemos ayudarte si no sabemos dónde estás.',
    '', ''
),
(
    87402,
    'Tous les grands seigneurs de guerre ne se ressemblent pas, $c. Certains sont arrivés là à force de temps et d''efforts, d''autres par leurs apptitude au combat. Devinez à quelle catégorie j''appartiens.',
    'Tous les grands seigneurs de guerre ne se ressemblent pas, $c. Certains sont arrivés là à force de temps et d''efforts, d''autres par leurs apptitude au combat. Devinez à quelle catégorie j''appartiens.',
    '', '',
    'No todos los Grandes Señores de la Guerra son iguales, $c. Unos llegamos por persistencia y tiempo, otros por habilidad en la lucha. ¿De qué clase crees que soy?',
    'No todos los Grandes Señores de la Guerra son iguales, $c. Unos llegamos por persistencia y tiempo, otros por habilidad en la lucha. ¿De qué clase crees que soy?',
    '', ''
),
(
    89855,
    'Les fragments d''artéfacts contiennent suffisamment d''énergie margique pour remettre sur pied cette fureur terrestre.$B$BCombien m''en apportez-vous ?',
    'Les fragments d''artéfacts contiennent suffisamment d''énergie margique pour remettre sur pied cette fureur terrestre.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Los fragmentos del artefacto contienen suficiente poder mágico para devolverle toda su fuerza a esta Furia de tierra.$B$B¿Cuántos me trajiste?',
    'Los fragmentos del artefacto contienen suficiente poder mágico para devolverle toda su fuerza a esta Furia de tierra.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    91435,
    'Les flammes se dégageant des braseros près de l''arène des Conquérants ont l''air ordinaires de premier abord. Cependant, je pense qu''ils ont été enchantés par l''antique magie de la Masse-Sanglante.$B$BJ''étais étrangement attirée vers eux. Quand j''en ai touché un, j''ai soudain été remplie d''énergie magique.',
    'Les flammes se dégageant des braseros près de l''arène des Conquérants ont l''air ordinaires de premier abord. Cependant, je pense qu''ils ont été enchantés par l''antique magie de la Masse-Sanglante.$B$BJ''étais étrangement attirée vers eux. Quand j''en ai touché un, j''ai soudain été remplie d''énergie magique.',
    '', '',
    'Las llamas de los braseros cerca del Anillo de conquista pueden parecer ordinarias a primera vista. Pero creo que están hechizadas con magia antigua de Gran Magullador.$B$BMe sentí extrañamente atraída hacia ellas y, al tocar una, me inundó de repente la ener.',
    'Las llamas de los braseros cerca del Anillo de conquista pueden parecer ordinarias a primera vista. Pero creo que están hechizadas con magia antigua de Gran Magullador.$B$BMe sentí extrañamente atraída hacia ellas y, al tocar una, me inundó de repente la ener.',
    '', ''
),
(
    85343,
    'Kronus va patrouiller le long de la route de la Gloire et se chargera d''éliminer les forces de l''Alliance qui croiseront son chemin.',
    'Kronus va patrouiller le long de la route de la Gloire et se chargera d''éliminer les forces de l''Alliance qui croiseront son chemin.',
    '', '',
    'Kronus patrullará el Camino de Gloria y lo limpiará de cualquier fuerza de la Alianza.',
    'Kronus patrullará el Camino de Gloria y lo limpiará de cualquier fuerza de la Alianza.',
    '', ''
),
(
    85461,
    'Les portes se trouvent au nord-ouest et au sud-est ; elles permettent de se déplacer de l''un de ces deux points à l''autre.',
    'Les portes se trouvent au nord-ouest et au sud-est ; elles permettent de se déplacer de l''un de ces deux points à l''autre.',
    '', '',
    'Los portales se ubican al noreste y sureste, y se pueden usar para moverse entre esos dos puntos.',
    'Los portales se ubican al noreste y sureste, y se pueden usar para moverse entre esos dos puntos.',
    '', ''
),
(
    85460,
    'Les mages ouvriront un portail à l''ouest d''A''shran. Il vous permettra de rentrer directement à l''avant-poste de Fer-de-Lance.',
    'Les mages ouvriront un portail à l''ouest d''A''shran. Il vous permettra de rentrer directement à l''avant-poste de Fer-de-Lance.',
    '', '',
    'Los magos invocarán un portal al oeste de Ashran que se puede usar para devolverte al Puesto Lanza de Guerra.',
    'Los magos invocarán un portal al oeste de Ashran que se puede usar para devolverte al Puesto Lanza de Guerra.',
    '', ''
),
(
    85334,
    'Nos meilleurs Chevaucheurs de loups vont arpenter A''shran pour protéger la province et servir sa population. Et mettre en pièces ces chiens de l''Alliance.',
    'Nos meilleurs Chevaucheurs de loups vont arpenter A''shran pour protéger la province et servir sa population. Et mettre en pièces ces chiens de l''Alliance.',
    '', '',
    'Nuestro mejor jinete de lobo cabalgará en Ashran para proteger y servir. Despedaza a esos perros de la Alianza.',
    'Nuestro mejor jinete de lobo cabalgará en Ashran para proteger y servir. Despedaza a esos perros de la Alianza.',
    '', ''
),
(
    84924,
    'J''ai besoin de beaucoup d''artéfacts pour équiper nos Chevaucheurs et les envoyer au combat.$B$BCombien m''en apportez-vous ?',
    'J''ai besoin de beaucoup d''artéfacts pour équiper nos Chevaucheurs et les envoyer au combat.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Tráeme suficientes de esos artefactos y podré equipar a nestros caballeros para enviarlos a la batalla.$B$B¿Cuántos me trajiste?',
    'Tráeme suficientes de esos artefactos y podré equipar a nestros caballeros para enviarlos a la batalla.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    84928,
    'Nos meilleurs Chevaucheurs vont arpenter A''shran pour protéger la province, servir la population... et faire vivre un cauchemar à la Horde.',
    'Nos meilleurs Chevaucheurs vont arpenter A''shran pour protéger la province, servir la population... et faire vivre un cauchemar à la Horde.',
    '', '',
    'Nuestro mejor jinete caballero cabalgará en Ashran para proteger y servir. Ya sabes, para arruinarle el día a la Horda.',
    'Nuestro mejor jinete caballero cabalgará en Ashran para proteger y servir. Ya sabes, para arruinarle el día a la Horda.',
    '', ''
),
(
    83896,
    'Les fragments d''artéfacts contiennent suffisamment d''énergie magique pour remettre sur pied cet ancien. $B$BCombien m''en apportez-vous ?',
    'Les fragments d''artéfacts contiennent suffisamment d''énergie magique pour remettre sur pied cet ancien. $B$BCombien m''en apportez-vous ?',
    '', '',
    'Los fragmentos del artefacto contienen suficiente poder mágico para revivir a este anciano y darle toda su fuerza.$B$B¿Cuántos me trajiste?',
    'Los fragmentos del artefacto contienen suficiente poder mágico para revivir a este anciano y darle toda su fuerza.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    84624,
    'Crograal va arpenter la route de la Gloire pour éliminer les forces de la Horde.',
    'Crograal va arpenter la route de la Gloire pour éliminer les forces de la Horde.',
    '', '',
    'Fangraal marchará por el Camino de Gloria y lo limpiará de cualquier fuerza de la Horda.',
    'Fangraal marchará por el Camino de Gloria y lo limpiará de cualquier fuerza de la Horda.',
    '', ''
),
(
    84627,
    'Les démonistes vont ouvrir des portes pour que nous puissions nous déplacer plus rapidement dans A''shran. Mais pour ça, nous avons besoin de la puissance magique renfermée dans les fragments d''artéfacts disséminés par ici.$B$BCombien m''en apportez-vous ?',
    'Les démonistes vont ouvrir des portes pour que nous puissions nous déplacer plus rapidement dans A''shran. Mais pour ça, nous avons besoin de la puissance magique renfermée dans les fragments d''artéfacts disséminés par ici.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Los brujos erigirán portales para que podamos movernos más rápido por Ashran. Solo necesitamos el poder mágico de esas piezas de artefacto desperdigadas por todo Ashran.$B$B¿Cuántos me trajiste?',
    'Los brujos erigirán portales para que podamos movernos más rápido por Ashran. Solo necesitamos el poder mágico de esas piezas de artefacto desperdigadas por todo Ashran.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    84642,
    'Les portes sont activées au sud-est et au nord-ouest d''A''shran. Servez-vous en pour vous téléporter d''une zone à l''autre.',
    'Les portes sont activées au sud-est et au nord-ouest d''A''shran. Servez-vous en pour vous téléporter d''une zone à l''autre.',
    '', '',
    'Los portales se activan hacia el sureste y el noroeste de Ashran. Úsalos para teletransportarte entre ambos puntos.',
    'Los portales se activan hacia el sureste y el noroeste de Ashran. Úsalos para teletransportarte entre ambos puntos.',
    '', ''
),
(
    84922,
    'Si vous récupérez assez des fragments d''artéfacts qu''on peut trouver à A''shran, nous pourrons utiliser l''énergie qu''ils renferment pour invoquer un portail de mage.$B$BCombien m''en apportez-vous ?',
    'Si vous récupérez assez des fragments d''artéfacts qu''on peut trouver à A''shran, nous pourrons utiliser l''énergie qu''ils renferment pour invoquer un portail de mage.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Si nos traes suficiente de estas piezas de artefacto que están dispersas en Ashran, podemos usar su poder para conjurar un portal de mago.$B$B¿Cuántos me trajiste?',
    'Si nos traes suficiente de estas piezas de artefacto que están dispersas en Ashran, podemos usar su poder para conjurar un portal de mago.$B$B¿Cuántos me trajiste?',
    '', ''
),
(
    90226,
    'Les mages vont invoquer dans l''ouest d''A''shran un portail qui vous permettra de rentrer au bastion de Bouclier-des-Tempêtes.',
    'Les mages vont invoquer dans l''ouest d''A''shran un portail qui vous permettra de rentrer au bastion de Bouclier-des-Tempêtes.',
    '', '',
    'Los magos invocarán un portal al oeste de Ashran que se puede usar para devolverte al Fuerte Escudo de Tormenta.',
    'Los magos invocarán un portal al oeste de Ashran que se puede usar para devolverte al Fuerte Escudo de Tormenta.',
    '', ''
),
(
    85651,
    'Je n''irai pas jusqu''à dire que nous n''étions pas préparés, mais on manque de réserves à Ashran. On n''en a pas emporté assez en traversant le portail. $B$BJ''espère que l''équipe de Brac pourra nous fournir la machinerie lourde nécessaire à notre victoire sur la Horde.',
    'Je n''irai pas jusqu''à dire que nous n''étions pas préparés, mais on manque de réserves à Ashran. On n''en a pas emporté assez en traversant le portail. $B$BJ''espère que l''équipe de Brac pourra nous fournir la machinerie lourde nécessaire à notre victoire sur la Horde.',
    '', '',
    'No quiero decir que no estábamos preparados, pero en Ashran escasean los suministros. No trajimos suficientes por el portal.$B$BEspero que el equipo de Piñón nos traiga la maquinaria pesada que necesitamos para ganar esta batalla contra la Horda.',
    'No quiero decir que no estábamos preparados, pero en Ashran escasean los suministros. No trajimos suficientes por el portal.$B$BEspero que el equipo de Piñón nos traiga la maquinaria pesada que necesitamos para ganar esta batalla contra la Horda.',
    '', ''
),
(
    85108,
    'Ne ménagez pas la Horde, $n. Rapportez-moi une preuve de sa défaite, et je vous récompenserai généreusement.',
    'Ne ménagez pas la Horde, $n. Rapportez-moi une preuve de sa défaite, et je vous récompenserai généreusement.',
    '', '',
    'Sé $Gduro:dura; con la Horda, $n. Tráeme una prueba de su desaparición y serás muy bien $Grecompensado:recompensada;.',
    'Sé $Gduro:dura; con la Horda, $n. Tráeme una prueba de su desaparición y serás muy bien $Grecompensado:recompensada;.',
    '', ''
),
(
    87228,
    'On sous-estime clairement le rôle des soigneurs, $n. Si on pensait plus souvent à nous inclure aux effectifs chargés de partir en mission, il y aurait moins de monde dans ma hutte de guérison.',
    'On sous-estime clairement le rôle des soigneurs, $n. Si on pensait plus souvent à nous inclure aux effectifs chargés de partir en mission, il y aurait moins de monde dans ma hutte de guérison.',
    '', '',
    'Se nos subestima a los sanadores, $n. Si nos eligieran más como acompañantes en sus misiones, habría menos en este refugio de sanación.',
    'Se nos subestima a los sanadores, $n. Si nos eligieran más como acompañantes en sus misiones, habría menos en este refugio de sanación.',
    '', ''
),
(
    87226,
    'Vous venez rendre visite à l''un des blessés, $n ?',
    'Vous venez rendre visite à l''un des blessés, $n ?',
    '', '',
    '¿Has venido aquí a visitar a los heridos, $n?',
    '¿Has venido aquí a visitar a los heridos, $n?',
    '', ''
),
(
    88551,
    'Ce n''est pas si facile que ça de s''occuper des chevaux, mais au moins on profite du grand air.',
    'Ce n''est pas si facile que ça de s''occuper des chevaux, mais au moins on profite du grand air.',
    '', '',
    'Cuidar a los caballos no es tan fácil como parece, pero al menos disfrutas del aire libre.',
    'Cuidar a los caballos no es tan fácil como parece, pero al menos disfrutas del aire libre.',
    '', ''
),
(
    88743,
    'Il vous reste des fragments d''artéfacts, $n ? Mon ami Ecilam en aurait bien besoin.',
    'Il vous reste des fragments d''artéfacts, $n ? Mon ami Ecilam en aurait bien besoin.',
    '', '',
    '¿Tienes algunos fragmentos de artefacto de sobra, $n? Mi amigo Ecilam podría usarlos.',
    '¿Tienes algunos fragmentos de artefacto de sobra, $n? Mi amigo Ecilam podría usarlos.',
    '', ''
),
(
    87224,
    'Nos potions fantomatiques sont parfaites pour éviter les gardes de la Horde ou aller se promener dans leur base principale ! Vous avez déjà essayé ça, $n ?',
    'Nos potions fantomatiques sont parfaites pour éviter les gardes de la Horde ou aller se promener dans leur base principale ! Vous avez déjà essayé ça, $n ?',
    '', '',
    'Mis pociones fantasma son perfectas para infiltrarse entre los guardias de la Horda, jo hasta para entrar en su base principal! ¿Ya las has probado, $n?',
    'Mis pociones fantasma son perfectas para infiltrarse entre los guardias de la Horda, jo hasta para entrar en su base principal! ¿Ya las has probado, $n?',
    '', ''
),
(
    83826,
    'Nos pouvoirs ne sont pas suffisants pour revitaliser cet ancien, mais nous n''abandonnerons pas. Nous aiderez-vous ?',
    'Nos pouvoirs ne sont pas suffisants pour revitaliser cet ancien, mais nous n''abandonnerons pas. Nous aiderez-vous ?',
    '', '',
    'Nestro poder no es lo suficientemente fuerte como para revivir a esto ancestro, pero lo seguiremos intentando. ¿Vas a ayudarnos?',
    'Nestro poder no es lo suficientemente fuerte como para revivir a esto ancestro, pero lo seguiremos intentando. ¿Vas a ayudarnos?',
    '', ''
),
(
    86756,
    'J''étais pratiquement rentré à la base après avoir fait le plein de fragments d''artéfacts quand je me suis fait dépouiller par ces vermines de la Horde. Je vous conseille de rester sur vos gardes, $n.',
    'J''étais pratiquement rentré à la base après avoir fait le plein de fragments d''artéfacts quand je me suis fait dépouiller par ces vermines de la Horde. Je vous conseille de rester sur vos gardes, $n.',
    '', '',
    'Estaba por llegar a la base con un montón de esos fragmentos de artefacto cuando la Horda me los robó. Cuídate en todo momento, $n.',
    'Estaba por llegar a la base con un montón de esos fragmentos de artefacto cuando la Horda me los robó. Cuídate en todo momento, $n.',
    '', ''
),
(
    86864,
    'Je me demande ce que Chris Clarkie peut faire de tous les pieds de troll qu''on lui rapporte.',
    'Je me demande ce que Chris Clarkie peut faire de tous les pieds de troll qu''on lui rapporte.',
    '', '',
    'Me pregunto qué es lo que Chris Clarkie hace con todos esos Pies de Trol que la traen.',
    'Me pregunto qué es lo que Chris Clarkie hace con todos esos Pies de Trol que la traen.',
    '', ''
),
(
    86758,
    'Comment êtes-vous $garrivé:arrivée; jusqu''ici ? Harrison Jones n''avait pas dit que cette aventure se révélerait aussi... dangereuse.',
    'Comment êtes-vous $garrivé:arrivée; jusqu''ici ? Harrison Jones n''avait pas dit que cette aventure se révélerait aussi... dangereuse.',
    '', '',
    '¿Cómo llegaste aquí? Harrison Jones nunca dijo que esta aventura sería tan... peligrosa.',
    '¿Cómo llegaste aquí? Harrison Jones nunca dijo que esta aventura sería tan... peligrosa.',
    '', ''
),
(
    87229,
    'Vous avez croisé Feezzel ? Je n''arrive pas à lui mettre la main dessus.',
    'Vous avez croisé Feezzel ? Je n''arrive pas à lui mettre la main dessus.',
    '', '',
    '¿Has visto a Feezzel? No lo encuentro por ninguna parte.',
    '¿Has visto a Feezzel? No lo encuentro por ninguna parte.',
    '', ''
),
(
    86207,
    'L''entrée de la mine est barrée, mais à l''intérieur le travail continue.',
    'L''entrée de la mine est barrée, mais à l''intérieur le travail continue.',
    '', '',
    'La entrada a la mina está bloqueada mientras estén trabajando adentro.',
    'La entrada a la mina está bloqueada mientras estén trabajando adentro.',
    '', ''
),
(
    87162,
    'Nous ne sommes pas les seuls à faire des prisonniers, $n.$B$BLa base de la Horde dispose d''une prison située sous la hutte de guérison qui se trouve sur la gauche, de l''autre côté de la porte principale.$B$BSi nous parvenons à mettre la main sur les clés de la prison de Fer-de-Lance qu''on peut trouver à A''shran, nous pourrons libérer les captifs.',
    'Nous ne sommes pas les seuls à faire des prisonniers, $n.$B$BLa base de la Horde dispose d''une prison située sous la hutte de guérison qui se trouve sur la gauche, de l''autre côté de la porte principale.$B$BSi nous parvenons à mettre la main sur les clés de la prison de Fer-de-Lance qu''on peut trouver à A''shran, nous pourrons libérer les captifs.',
    '', '',
    'No somos los únicos con prisioneros, $n.$B$BEn la base de la Horda está su prisión, bajo el puesto de auxilio que hay a la izquierda tras cruzar la puerta principal.$B$BSi encontramos algunas de esas llaves de prisión Lanza de guerra en Ashran, podremos liber.',
    'No somos los únicos con prisioneros, $n.$B$BEn la base de la Horda está su prisión, bajo el puesto de auxilio que hay a la izquierda tras cruzar la puerta principal.$B$BSi encontramos algunas de esas llaves de prisión Lanza de guerra en Ashran, podremos liber.',
    '', ''
),
(
    90534,
    'Il fait un peu chaud, aujourd''hui, vous ne trouvez pas ?',
    'Il fait un peu chaud, aujourd''hui, vous ne trouvez pas ?',
    '', '',
    'Hoy hace calor, ¿no crees?',
    'Hoy hace calor, ¿no crees?',
    '', ''
),
(
    90385,
    'Difficile de combattre par ce froid, pas vrai ?',
    'Difficile de combattre par ce froid, pas vrai ?',
    '', '',
    'El frío de verdad te molesta.',
    'El frío de verdad te molesta.',
    '', ''
),
(
    89897,
    'Nous ne sommes pas les seuls à faire des prisonniers, $c.$B$BLa base de l''Alliance dispose d''une prison, située sous le bâtiment principal.$B$BIl nous suffirait de trouver les clés de la prison de Bouclier-des-Tempêtes pour pouvoir libérer les prisonniers.',
    'Nous ne sommes pas les seuls à faire des prisonniers, $c.$B$BLa base de l''Alliance dispose d''une prison, située sous le bâtiment principal.$B$BIl nous suffirait de trouver les clés de la prison de Bouclier-des-Tempêtes pour pouvoir libérer les prisonniers.',
    '', '',
    'No somos los únicos que tienen prisioneros, $c.$B$BEn la base de la Alianza, debajo del edificio principal, hay una prisión.$B$BSolo tenemos que encontrar esas llaves de la prisión de Escudo de Tormenta para liberarlos.',
    'No somos los únicos que tienen prisioneros, $c.$B$BEn la base de la Alianza, debajo del edificio principal, hay una prisión.$B$BSolo tenemos que encontrar esas llaves de la prisión de Escudo de Tormenta para liberarlos.',
    '', ''
);

-- Artifact count gossips
DELETE FROM npc_text WHERE ID IN (84476, 84631, 84926, 85336, 85339, 85340, 85341, 86036);
INSERT INTO npc_text (ID, text0_0, text0_1) VALUES
(84476, 'We currently have $9250w collected. We need $9706w total.', 'We currently have $9250w collected. We need $9706w total.'), -- Horde Warlock
(84631, 'We currently have $9261w collected. We need $9708w total.', 'We currently have $9261w collected. We need $9708w total.'), -- Horde Shaman
(84926, 'We currently have $9314w collected. We need $9705w total.', 'We currently have $9314w collected. We need $9705w total.'), -- Alliance Mage
(85336, 'We currently have $9313w collected. We need $9705w total.', 'We currently have $9313w collected. We need $9705w total.'), -- Horde Mage
(85339, 'We currently have $9312w collected. We need $9707w total.', 'We currently have $9312w collected. We need $9707w total.'), -- Alliance Paladin
(85340, 'We currently have $9262w collected. We need $9708w total.', 'We currently have $9262w collected. We need $9708w total.'), -- Alliance Druid
(85341, 'We currently have $9253w collected. We need $9706w total.', 'We currently have $9253w collected. We need $9706w total.'), -- Alliance Warlock
(86036, 'We currently have $9311w collected. We need $9707w total.', 'We currently have $9311w collected. We need $9707w total.'); -- Horde Warrior

-- Artifact count gossips
DELETE FROM locales_npc_text WHERE entry IN (84476, 84631, 84926, 85336, 85339, 85340, 85341, 86036);
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUES
(
    84476,
    'Pour l''instant, nous en avons récolté $9250w, mais il nous en faut au total $9706w.',
    'Pour l''instant, nous en avons récolté $9250w, mais il nous en faut au total $9706w.',
    '', '',
    'Por el momento tenemos $9250w. Necesitamos un total de $9706w.',
    'Por el momento tenemos $9250w. Necesitamos un total de $9706w.',
    '', ''
),
(
    84631,
    'Pour l''instant, nous en avons récolté $9261w, mais il nous en faut au total $9708w.',
    'Pour l''instant, nous en avons récolté $9261w, mais il nous en faut au total $9708w.',
    '', '',
    'Por el momento tenemos $9261w. Necesitamos un total de $9708w.',
    'Por el momento tenemos $9261w. Necesitamos un total de $9708w.',
    '', ''
),
(
    84926,
    'Pour l''instant, nous en avons récolté $9314w, mais il nous en faut au total $9705w.',
    'Pour l''instant, nous en avons récolté $9314w, mais il nous en faut au total $9705w.',
    '', '',
    'Por el momento tenemos $9314w. Necesitamos un total de $9705w.',
    'Por el momento tenemos $9314w. Necesitamos un total de $9705w.',
    '', ''
),
(
    85336,
    'Pour l''instant, nous en avons récolté $9313w, mais il nous en faut au total $9705w.',
    'Pour l''instant, nous en avons récolté $9313w, mais il nous en faut au total $9705w.',
    '', '',
    'Por el momento tenemos $9313w. Necesitamos un total de $9705w.',
    'Por el momento tenemos $9313w. Necesitamos un total de $9705w.',
    '', ''
),
(
    85339,
    'Pour l''instant, nous en avons récolté $9312w, mais il nous en faut au total $9707w.',
    'Pour l''instant, nous en avons récolté $9312w, mais il nous en faut au total $9707w.',
    '', '',
    'Por el momento tenemos $9312w. Necesitamos un total de $9707w.',
    'Por el momento tenemos $9312w. Necesitamos un total de $9707w.',
    '', ''
),
(
    85340,
    'Pour l''instant, nous en avons récolté $9262w, mais il nous en faut au total $9708w.',
    'Pour l''instant, nous en avons récolté $9262w, mais il nous en faut au total $9708w.',
    '', '',
    'Por el momento tenemos $9262w. Necesitamos un total de $9708w.',
    'Por el momento tenemos $9262w. Necesitamos un total de $9708w.',
    '', ''
),
(
    85341,
    'Pour l''instant, nous en avons récolté $9253w, mais il nous en faut au total $9706w.',
    'Pour l''instant, nous en avons récolté $9253w, mais il nous en faut au total $9706w.',
    '', '',
    'Por el momento tenemos $9253w. Necesitamos un total de $9706w.',
    'Por el momento tenemos $9253w. Necesitamos un total de $9706w.',
    '', ''
),
(
    86036,
    'Pour l''instant, nous en avons récolté $9311w, mais il nous en faut au total $9707w.',
    'Pour l''instant, nous en avons récolté $9311w, mais il nous en faut au total $9707w.',
    '', '',
    'Por el momento tenemos $9311w. Necesitamos un total de $9707w.',
    'Por el momento tenemos $9311w. Necesitamos un total de $9707w.',
    '', ''
);

DELETE FROM creature_text WHERE entry IN (88675, 88676, 88771, 88772, 88679, 88682, 88696, 88697);
INSERT INTO creature_text VALUES
(88675, 0, 0, 'These Steamwheedle goblins are reprehensible. They''ll do anything to make a quick coin!', 12, 0, 100, 0, 0, 0, 'IllandriaBelore0'),
(88675, 1, 0, 'They''ve started selling souvenirs!', 12, 0, 100, 0, 0, 0, 'IllandriaBelore1'),
(88675, 2, 0, 'I wish it were a jest. They''ve been stringing together artifact fragments and selling them as jewelry. The nerve of them to treat a priceless such a way!', 12, 0, 100, 0, 0, 0, 'IllandriaBelore2'),
(88675, 3, 0, 'Has Belloc come any closer to finding the location of this mysterious artifact?', 12, 0, 100, 0, 0, 0, 'IllandriaBelore3'),
(88675, 4, 0, 'We must make haste if we wish to find it before Dr. Jones gets his hands on it.', 12, 0, 100, 0, 0, 0, 'IllandriaBelore4'),

(88676, 0, 0, 'What did they do this time?', 12, 0, 100, 0, 0, 0, 'ExaminerRahmFlameheart0'),
(88676, 1, 0, 'We''re barely recovering from our assault on the dark portal, and they''re setting up a tourism racket? Surely you must be joking.', 12, 0, 100, 0, 0, 0, 'ExaminerRahmFlameheart1'),
(88676, 2, 0, 'He''s had some leads, but nothing terribly solid yet.', 12, 0, 100, 0, 0, 0, 'ExaminerRahmFlameheart2'),
(88676, 3, 0, 'Agreed, there is no telling what the Alliance might do with such a powerful weapon. We must petition for more aid before our cause becomes lost.', 12, 0, 100, 0, 0, 0, 'ExaminerRahmFlameheart3'),

(88771, 0, 0, 'Rogue spies have informed us that the Alliance are building machinery used to drill through rocks. Do you think its for the mines?', 12, 0, 100, 0, 0, 0, 'CenturionFirescream0'),
(88771, 0, 1, 'Truce? With the Alliance? Never.', 12, 0, 100, 0, 0, 0, 'CenturionFirescream1'),

(88772, 0, 0, 'If only we were able to bring a Zeppelin through the portal. We could take out Stormshield from the skies!', 12, 0, 100, 0, 0, 0, 'LegionnaireHellaxe0'),
(88772, 0, 1, 'Have you seen how many Death Knights are within our ranks? Why is it that we still can''t penetrate Stormshield? Are they not using Necrotic Plague?', 12, 0, 100, 0, 0, 0, 'LegionnaireHellaxe1'),

(88679, 0, 0, 'That was some rough business at the Dark Portal. How are you holding up?', 12, 0, 100, 0, 0, 0, 'GrimnirSternhammer0'),
(88679, 1, 0, 'Aye, that it does. That is, when said history isn''t chasing after you with a broadaxe!', 12, 0, 100, 0, 0, 0, 'GrimnirSternhammer1'),
(88679, 2, 0, 'So what do you supposed the nature of this fabled artifact is?', 12, 0, 100, 0, 0, 0, 'GrimnirSternhammer2'),
(88679, 3, 0, 'That seems a little far-fetched. I haven''t seen any evidence to link the titans to this world.', 12, 0, 100, 0, 0, 0, 'GrimnirSternhammer3'),

(88682, 0, 0, 'I''m a little worse for the wear, but it''s not my first scrape. Just excited that our new outpost has such fascinating history to discover.', 12, 0, 100, 0, 0, 0, 'MisirinStouttoe0'),
(88682, 1, 0, 'If it is as powerful as Dr. Jones theorizes, it may very well be of titan origin.', 12, 0, 100, 0, 0, 0, 'MisirinStouttoe1'),
(88682, 2, 0, 'That may be so, but wouldn''t it be grand to discover that the titans had reach even here? Just think of the ramifications!', 12, 0, 100, 0, 0, 0, 'MisirinStouttoe2'),

(88696, 0, 0, 'It is as we feared, we must venture into the mines. Who knows what ancient dangers are within there...', 12, 0, 100, 0, 0, 0, 'OfficerRumsfeld0'),
(88696, 0, 1, 'We must focus all of our efforts into the mines. I agree with Dr. Jones, the Artifact has to be there. It has to be!', 12, 0, 100, 0, 0, 0, 'OfficerRumsfeld1'),

(88697, 0, 0, 'How are we supposed to win this battle against the Horde without siege engines?', 12, 0, 100, 0, 0, 0, 'OfficerIronore0'),
(88697, 0, 1, 'If only we had an airship. We could take out Warspear from the skies!', 12, 0, 100, 0, 0, 0, 'OfficerIronore1');

DELETE FROM locales_creature_text WHERE entry IN (88675, 88676, 88771, 88772, 88679, 88682, 88696, 88697);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    88675, 0, 0,
    'Ces gobelins de Gentepression... faudrait les enfermer. Ils feraient n''importe quoi pour de l''argent !',
    '',
    'Esos goblins Bonvapor son lamentables. ¡Harían cualquier cosa por una moneda!',
    ''
),
(
    88675, 1, 0,
    'Ça y est, ils vendent des souvenirs !',
    '',
    '¡Empezaron a vender recuerdos!',
    ''
),
(
    88675, 2, 0,
    'J''aimerais bien. Ils assemblent des bouts de reliques à la va-vite et vendent ça comme bijoux. Traiter des objets aussi inestimables de la sorte, franchement !',
    '',
    'Me gustaría que fuera broma. Engarzan fragmentos de artefacto y los venden como joyería. ¡Cómo pueden tratar así algo que no tiene precio!',
    ''
),
(
    88675, 3, 0,
    'Est-ce que Belloc en sait plus sur l''endroit où se trouve ce mystérieux artéfact ?',
    '',
    '¿Está Belloc más cerca de encontrar la ubicación de este misterioso artefacto?',
    ''
),
(
    88675, 4, 0,
    'Nous devons nous hâter si nous voulons le trouver avant le docteur Jones.',
    '',
    'Debemos apresurarnos si queremos encontrarlo antes que el Dr. Jones.',
    ''
),
(
    88676, 0, 0,
    'Qu''est-ce qu''ils ont encore fait ?',
    '',
    '¿Qué hicieron esta vez?',
    ''
),
(
    88676, 1, 0,
    'On se remet à peine de notre assaut sur la Porte des ténèbres, et ils montent un business touristique ? C''est une plaisanterie !',
    '',
    '¿Apenas nos estamos recuperando del asalto contra el Portal Oscuro y montan un negocio turístico? Tienes que estar bromeando...',
    ''
),
(
    88676, 2, 0,
    'Il a suivi quelques pistes, mais rien de vraiment concret pour l''instant.',
    '',
    'Tiene algunas pistas, pero nada demasiado claro.',
    ''
),
(
    88676, 3, 0,
    'Je suis d''accord. On ne sait pas ce dont l''Alliance serait capable avec une arme aussi puissante. Nous devons trouver plus de soutien avant que notre quête ne devienne une cause perdue.',
    '',
    'Así es, no sabemos qué podría hacer la Alianza con un arma tan poderosa. Debemos solicitar más ayuda antes de que nestra causa esté perdida.',
    ''
),
(
    88771, 0, 0,
    'Nos espions nous ont appris que l''Alliance construit un engin de forage. Vous pensez que c''est pour la mine ?',
    '',
    'Los pícaros espías nos informaron que la Alianza está construyendo maquinaria para perforar roca. ¿Crees que es para las minas?',
    ''
),
(
    88771, 0, 1,
    'Une trêve ? Avec l''Alliance ? Plutôt mourir.',
    '',
    '¿Tregua? ¿Con de Alianza? Jamás',
    ''
),
(
    88772, 0, 0,
    'Si seulement nous pouvions faire passer un zeppelin à travers le portail. Nous pourrions raser Bouclier-des-Tempêtes depuis les airs !',
    '',
    'Si fuéramos capaces de traer un dirigible por el portal... ¡Podríamos eliminar Escudo de Tormenta desde el cielo!',
    ''
),
(
    88772, 0, 1,
    'Vous avez vu le nombre de chevaliers de la mort parmi nous ? Comment se fait-il que nous n''arrivions toujours pas à entrer dans Bouclier-des-Tempêtes ? N''utilisent-ils donc pas leur Peste nécrotique ?',
    '',
    '¿Viste cuántos caballeros de la Muerte hay en nuestras filas ? ¿Cómo es que aún no podemos penetrar Escudo de Tormenta? ¿No utilizan Peste necrótica?',
    ''
),
(
    88679, 0, 0,
    'C''est moche, ce qui s''est passé à la Porte des ténèbres. Vous tenez le coup ?',
    '',
    'Lo del Portal Oscuro fue bastante difícil. ¿Cómo te sientes?',
    ''
),
(
    88679, 1, 0,
    'Ça, c''est bien vrai. Enfin, tant que ce qu''on découvre n''essaie pas de nous arracher la tête !',
    '',
    'Es verdad. ¡Bueno, al menos cuando esa historia no te persigue con un hacha!',
    ''
),
(
    88679, 2, 0,
    'Donc à votre avis, de quelle nature est cet artéfact légendaire ?',
    '',
    '¿Cuál suponías que era la naturaleza de este legendario artefacto?',
    ''
),
(
    88679, 3, 0,
    'Cela me semble un peu tiré par les cheveux. Je n''ai rien vu qui puisse relier les titans à ce monde.',
    '',
    'No parece muy verosímil. No he visto ninguna prueba que vincule a los titanes con este mundo.',
    ''
),
(
    88682, 0, 0,
    'Quelques égratignures, mais j''en ai vu d''autres. Je suis surtout enthousiaste à l''idée de tout ce qu''il y a à découvrir à notre nouvel avant-poste !',
    '',
    'Algo lastimado por el desgaste, aunque no es mi primera vez. ¡Me encanta que nuestro nuevo puesto tenga una historia tan fascinante por descubrir!',
    ''
),
(
    88682, 1, 0,
    'S''il est aussi puissant que le pense le docteur Jones, il pourrait bien être d''origine titanesque.',
    '',
    'Si es tan poderoso como piensa el Dr. Jones, podría ser de origen titánico.',
    ''
),
(
    88682, 2, 0,
    'Peut-être, mais ne serait-il pas formidable de découvrir qu''ils sont arrivés jusqu''ici ? Pensez à tout ce que cela impliquerait !',
    '',
    'Puede ser, pero ¿no sería grandioso descubrir que los titanes llegaron incluso aquí? ¡Piensa en las implicaciones!',
    ''
),
(
    88696, 0, 0,
    'C''est bien ce que nous craignions, nous devons pénétrer dans les mines. Qui sait quels dangers antiques nous y attendent...',
    '',
    'Como temíamos, debemos adentrarnos en las minas. Quién sabe qué antiguos peligros habrá allí dentro...',
    ''
),
(
    88696, 0, 1,
    'Nous devons concentrer tous nos efforts sur les mines. Je suis d''accord avec le docteur Jones, c''est certainement là que se trouve l''artéfact. C''est forcément ça !',
    '',
    'Debemos centrar todos nuestros esfuerzos en las minas. Pienso, como el Dr. Jones, que allí está el artefacto. ¡Tiene que estar ahí!',
    ''
),
(
    88697, 0, 0,
    'Comment sommes-nous censés remporter cette bataille contre la Horde sans armes de siège ?',
    '',
    '¿Cómo vamos a gagnar esta batalla contra la Horda sin máquinas de asedio?',
    ''
),
(
    88697, 0, 1,
    'Si seulement nous avions une canonnière. On pourrait raser Fer-de-Lance depuis les airs !',
    '',
    'Si tuviéramos un dirigible... ¡Podríamos eliminar Lanza de Guerra desde el cielo!',
    ''
);