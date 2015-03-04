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
--                                                       French     Spanish    German     Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(80858, 0, 0, 'Je suis de retour ! Pauvres gueux, soumettez-vous à la puissance de l’empire ogre !', '', '¡He vuelto! ¡Arrodillaos ante el poder del gran Imperio ogro!', ''),
(80858, 1, 0, 'Petits guerriers bleus, vous vous êtes montrés dignes d''échapper à mon courroux.', '', 'Guerreros azules pequeños, que han demostrado que son dignos de evitar mi ira.', ''),
(80858, 2, 0, 'Les rouges, oui, ils seront témoins de ma dévastation !', '', 'Los rojos, sí, ¡van a dar testimonio de mi devastación!', ''),
(80858, 3, 0, 'Plus d''ennemis ! Donnez-moi plus d''ennemis à égorger !', '', '¡Más, traer más a morir en mi espada!', ''),
(80858, 3, 1, 'Quelle folie de vous opposer à moi, gringalets !', '', '¡Eres un necio para enfrentarte a mí, debilucho!', ''),
(80858, 3, 2, 'Des insectes, vous n''êtes que des insectes pour moi !', '', '¡Insectos, sois insectos para mí!', ''),
(80858, 3, 3, 'Ha ha ha ha !', '', '¡Ja, ja, ja, ja!', ''),
(80858, 3, 4, 'Une facilité déconcertante !', '', '¡Una facilidad desconcertante!', ''),
(80858, 4, 0, 'L''empire... renaîtra !', '', '¡El... Imperio... renacerá!', '');

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

UPDATE creature, creature_equip_template SET creature.`equipment_id` = creature_equip_template.`id` WHERE
creature.`id` = creature_equip_template.`entry`;

DELETE FROM gameobject WHERE id = 233282 AND map = 1191;
DELETE FROM creature WHERE id = 85441 AND map = 1191;

DELETE FROM creature WHERE id IN (86201, 86202);
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, curhealth, curmana, unit_flags2) VALUES
(86201, 1191, 6941, 7099, 1, 1, 1, 5098.05, -4100.28, 59.7991, 5.28609, 300, 1415988, 0, 2048),
(86202, 1191, 6941, 7099, 1, 1, 1, 5093.30, -4107.56, 59.7992, 0.035701, 300, 1417220, 96656, 2048);

-- Artifact fragments
-- Horde
UPDATE creature_template SET gossip_menu_id = 65536, ScriptName = 'npc_ashran_atomik' WHERE entry = 82204;
UPDATE creature_template SET gossip_menu_id = 65537, ScriptName = '' WHERE entry = 83830;
UPDATE creature_template SET gossip_menu_id = 65539, ScriptName = '' WHERE entry = 83995;
UPDATE creature_template SET gossip_menu_id = 65540, ScriptName = '' WHERE entry = 83997;

UPDATE creature_template SET gossip_menu_id = 65538, ScriptName = '' WHERE entry = 83869;
UPDATE creature_template SET gossip_menu_id = 65541, ScriptName = 'npc_ashran_commander_anne_dunworthy' WHERE entry = 84173;
UPDATE creature_template SET gossip_menu_id = 65542, ScriptName = 'npc_ashran_general_ushet_wolfbarger' WHERE entry = 84473;
UPDATE creature_template SET gossip_menu_id = 65543, ScriptName = '' WHERE entry = 84660;
UPDATE creature_template SET gossip_menu_id = 65544, ScriptName = '' WHERE entry = 88826;
UPDATE creature_template SET gossip_menu_id = 65545, ScriptName = '' WHERE entry = 88448;
UPDATE creature_template SET gossip_menu_id = 65546 WHERE entry = 82200;
UPDATE creature_template SET gossip_menu_id = 65547, ScriptName = '' WHERE entry = 82883;
UPDATE creature_template SET gossip_menu_id = 65548 WHERE entry = 86201;
UPDATE creature_template SET gossip_menu_id = 65549 WHERE entry = 86202;
UPDATE creature_template SET gossip_menu_id = 65550 WHERE entry = 88777;
UPDATE creature_template SET gossip_menu_id = 65551 WHERE entry = 88778;
UPDATE creature_template SET ScriptName = 'npc_ashran_warspear_shaman' WHERE entry = 82438;
UPDATE creature_template SET ScriptName = 'npc_ashran_illandria_belore' WHERE entry = 88675;
UPDATE creature_template SET ScriptName = 'npc_ashran_examiner_rahm_flameheart' WHERE entry = 88676;
UPDATE creature_template SET ScriptName = 'npc_ashran_centurion_firescream' WHERE entry = 88771;
UPDATE creature_template SET ScriptName = 'npc_ashran_legionnaire_hellaxe' WHERE entry = 88772;
UPDATE creature_template SET npcflag = 0x3 WHERE entry = 86366;

ALTER TABLE `gossip_menu` CHANGE `entry` `entry` INT(10) UNSIGNED DEFAULT '0' NOT NULL, CHANGE `text_id` `text_id` INT(10) UNSIGNED DEFAULT '0' NOT NULL;
DELETE FROM gossip_menu WHERE entry IN (65536, 65537, 65538, 65539, 65540, 65541, 65542, 65543, 65544, 65545, 65546, 65547, 65548, 65549, 65550, 65551, 65552, 65553, 65554, 65555, 65556, 65557, 65558, 65559);
DELETE FROM gossip_menu WHERE text_id IN (89855, 85290, 85346, 85455, 85464, 86045, 84857, 91496, 82999, 83825, 87402, 88550, 88548, 91432, 91435, 85343, 85461, 85460, 85334, 84631, 84476, 85336, 86036);
INSERT INTO gossip_menu (entry, text_id) VALUES
(65536, 89855),
(65537, 85290),
(65538, 85346),
(65539, 85455),
(65540, 85464),
-- (65541, 0),
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
(65559, 86036);

ALTER TABLE `gossip_menu_option` CHANGE `menu_id` `menu_id` INT(10) UNSIGNED DEFAULT '0' NOT NULL, CHANGE `id` `id` SMALLINT(5) UNSIGNED DEFAULT '0' NOT NULL;
DELETE FROM gossip_menu_option WHERE menu_id IN (65536, 65537, 65539, 65540, 65543, 65546, 65547);
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
(65545, 0, 0, 'Take all of my Artifact Fragments!', 1, 1, 0, 0, 0, 0, ''),
-- Honor Quartermarster
(65547, 0, 1, 'I would like to buy from you.', 1, 128, 0, 0, 0, 0, '');

ALTER TABLE `locales_gossip_menu_option` CHANGE `menu_id` `menu_id` INT(10) UNSIGNED DEFAULT '0' NOT NULL;
DELETE FROM locales_gossip_menu_option WHERE menu_id IN (65536, 65537, 65539, 65540, 65543, 65545, 65547);
--                                                   French            German            Spanish           Russian
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc2, option_text_loc3, option_text_loc6, option_text_loc8) VALUES
(
    65536, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '',
    ''
),
(
    65536, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '',
    ''
),
(
    65536, 2,
    'À quoi sert Kronus ?',
    '',
    '',
    ''
),
(
    65540, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '',
    ''
),
(
    65540, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '',
    ''
),
(
    65540, 2,
    'À quoi servent les portes de démoniste ?',
    '',
    '',
    ''
),
(
    65539, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '',
    ''
),
(
    65539, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '',
    ''
),
(
    65539, 2,
    'À quoi servent les portails de mage ?',
    '',
    '',
    ''
),
(
    65537, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '',
    ''
),
(
    65537, 1,
    'Combien de fragments d''artéfacts avons-nous rassemblés ?',
    '',
    '',
    ''
),
(
    65537, 2,
    'Quel est le rôle des Chevaucheurs de loups ?',
    '',
    '',
    ''
),
(
    65543, 0,
    'Montrez-moi la cime de la Brute.',
    '',
    '',
    ''
),
(
    65543, 1,
    'Montrez-moi la carrière de la Fournaise.',
    '',
    '',
    ''
),
(
    65543, 2,
    'Montrez-moi l''amphitéâtre de l''Annihilation.',
    '',
    '',
    ''
),
(
    65543, 3,
    'Montrez-moi le cimetière des Cognecendres.',
    '',
    '',
    ''
),
(
    65545, 0,
    'Prenez tous mes fragments d''artéfacts !',
    '',
    '',
    ''
),
(
    65547, 0,
    'Je voudrais vous acheter quelque chose.',
    '',
    '',
    ''
);

-- Basic text from horde NPC's, base gossips
DELETE FROM npc_text WHERE ID IN (82999, 83825, 84857, 85290, 85346, 85455, 85464, 86045, 87402, 88548, 88550, 89855, 91432, 91496, 91435, 85343, 85461, 85460, 85334, 84631);
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
-- INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
-- (
--     0,
--     'You don''t need to waste your time with alchemy. Just bring me the ingredients and I''ll do it for you.$B$BWhat are you interested in?',
--     'You don''t need to waste your time with alchemy. Just bring me the ingredients and I''ll do it for you.$B$BWhat are you interested in?'
-- );
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
);

-- Basic text from horde NPC's, base gossips
DELETE FROM locales_npc_text WHERE entry IN (82999, 83825, 84857, 85290, 85346, 85455, 85464, 86045, 87402, 88548, 88550, 89855, 91432, 91496, 91435, 85343, 85461, 85460, 85334);
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUES
(
    82999,
    'Avez-vous déniché l''un de ces artéfacts cachés ?$B$BOn trouve des fragments un peu partout dans A''shran. J''ai entendu dire qu''on avait récemment découvert en tuant des créatures rôdant à proximité de la route de la Gloire.$B$BVous pouvez me remettre ceux que vous trouverez, et je me chargerai de les redistribuer aux personnes qui en ont besoin.',
    'Avez-vous déniché l''un de ces artéfacts cachés ?$B$BOn trouve des fragments un peu partout dans A''shran. J''ai entendu dire qu''on avait récemment découvert en tuant des créatures rôdant à proximité de la route de la Gloire.$B$BVous pouvez me remettre ceux que vous trouverez, et je me chargerai de les redistribuer aux personnes qui en ont besoin.',
    '', '',
    '¿Has visto alguno de los artefactos ocultos?$B$BPuedes encontrar fragmentos en todo Ashran. He odio que matando a las criaturas en las afueras de este lado de la carretera de la gloria se han descubierto algunos recientemente.$B$BMe puedes dar alguna que encuentres, y la distribuiré a aquellos que los necesitan.',
    '¿Has visto alguno de los artefactos ocultos?$B$BPuedes encontrar fragmentos en todo Ashran. He odio que matando a las criaturas en las afueras de este lado de la carretera de la gloria se han descubierto algunos recientemente.$B$BMe puedes dar alguna que encuentres, y la distribuiré a aquellos que los necesitan.',
    '', ''
),
(
    83825,
    '<Pousse un léger gémissement gutural.>',
    '<Pousse un léger gémissement gutural.>',
    '', '',
    '<Vamos a escapar dulcemente, gemido profundo>',
    '<Vamos a escapar dulcemente, gemido profundo>',
    '', ''
),
(
    84857,
    'Plongez votre regard dans le mien $n. Vous pourrez ainsi observer à distance ce qui se passe à A''shran.$B$BQue voulez-vous voir ?',
    'Plongez votre regard dans le mien $n. Vous pourrez ainsi observer à distance ce qui se passe à A''shran.$B$BQue voulez-vous voir ?',
    '', '',
    'Mira profundamente en mis ojos, $n y podrás estudiar Ashran desde lejos.$B$B¿Qué te gustaría ver?',
    'Mira profundamente en mis ojos, $n y podrás estudiar Ashran desde lejos.$B$B¿Qué te gustaría ver?',
    '', ''
),
(
    85290,
    'J''ai besoin de beaucoup d''artéfacts pour équiper nos Chevaucheurs de loups et les envoyer au combat.$B$BCombien m''en apportez-vous ?',
    'J''ai besoin de beaucoup d''artéfacts pour équiper nos Chevaucheurs de loups et les envoyer au combat.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Tráeme de vuelta suficiente de esos artefactos y puedo orientar nuestros jinetes de lobos y enviarlos a la batalla.$B$B¿Cuántos me has traído?',
    'Tráeme de vuelta suficiente de esos artefactos y puedo orientar nuestros jinetes de lobos y enviarlos a la batalla.$B$B¿Cuántos me has traído?',
    '', ''
),
(
    85346,
    'VOUS COMPTEZ PRENDRE RACINE ICI OU QUOI ? ALLEZ ME TUER LES VERMINES DE L''ALLIANCE. RAPPORTEZ-MOI DES TÊTES, DES DENTS, DES QUEUES... PEU IMPORTE ! ET GROUILLEZ-VOUS !!',
    'VOUS COMPTEZ PRENDRE RACINE ICI OU QUOI ? ALLEZ ME TUER LES VERMINES DE L''ALLIANCE. RAPPORTEZ-MOI DES TÊTES, DES DENTS, DES QUEUES... PEU IMPORTE ! ET GROUILLEZ-VOUS !!',
    '', '',
    '¿QUÉ ESTÁS HACIENDO SIMPLEMENTE DANDO VUELTAS ALREDEDOR? VE A MATAR ALGUNA ESCORIA DE ALIANZA. ¡TRÁEME DE VUELTA SU CABEZA, DIENTES, COLAS... LO QUE SEA! Y RÁPIDO !!',
    '¿QUÉ ESTÁS HACIENDO SIMPLEMENTE DANDO VUELTAS ALREDEDOR? VE A MATAR ALGUNA ESCORIA DE ALIANZA. ¡TRÁEME DE VUELTA SU CABEZA, DIENTES, COLAS... LO QUE SEA! Y RÁPIDO !!',
    '', ''
),
(
    85455,
    'L''énergie dons nous avons besoin se trouve dans les fragments d''artéfacts, $n. En quantité suffisante, ils permettront aux mages d''invoquer un portail pour atteindre l''avant-poste de Fer-de-Lance plus rapidement.$B$BCombien m''en apportez-vous ?',
    'L''énergie dons nous avons besoin se trouve dans les fragments d''artéfacts, $n. En quantité suffisante, ils permettront aux mages d''invoquer un portail pour atteindre l''avant-poste de Fer-de-Lance plus rapidement.$B$BCombien m''en apportez-vous ?',
    '', '',
    'El poder está dentro de los fragmentos de artefactos, $n. Trae de vuelta lo suficiente de esos y los magos convocarám un portal para que podamos volver a Lanza de Guerra rápido.$B$B¿Cuántos me has traído?',
    'El poder está dentro de los fragmentos de artefactos, $n. Trae de vuelta lo suficiente de esos y los magos convocarám un portal para que podamos volver a Lanza de Guerra rápido.$B$B¿Cuántos me has traído?',
    '', ''
),
(
    85464,
    'Si nous parvenons à nous procurer assez de fragments d''artéfacts qu''on peut trouver à A''shran, nous pourrons activer les portes de déùpnostres pour nous déplacer plus rapidement d''un point à un autre.$B$BCombien m''en apportez-vous ?',
    'Si nous parvenons à nous procurer assez de fragments d''artéfacts qu''on peut trouver à A''shran, nous pourrons activer les portes de déùpnostres pour nous déplacer plus rapidement d''un point à un autre.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Si podemos reunir suficiente de los fragmentos de artefactos encontrados dentro Ashran podemos activar el portal de brujo para que podamos pasar entre las zonas más rápido.$B$B¿Cuántos me has traído?',
    'Si podemos reunir suficiente de los fragmentos de artefactos encontrados dentro Ashran podemos activar el portal de brujo para que podamos pasar entre las zonas más rápido.$B$B¿Cuántos me has traído?',
    '', ''
),
(
    86045,
    'Alors, combien de soldats de l''Alliance avez-vous tués ajourd''hui, $n ?',
    'Alors, combien de soldats de l''Alliance avez-vous tués ajourd''hui, $n ?',
    '', '',
    'Entonces, ¿cuántos Alianza has matado hoy, $n?',
    'Entonces, ¿cuántos Alianza has matado hoy, $n?',
    '', ''
),
-- INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
-- (
--     0,
--     'Ne perdez pas de temps avec l''alchimie.$B$BApportez-moi les ingrédients et je m''en occuperai pour vous.',
--     'Ne perdez pas de temps avec l''alchimie.$B$BApportez-moi les ingrédients et je m''en occuperai pour vous.',
--     '', '',
--     'No tienes que perder el tiempo con la alquimia. Sólo tráeme los ingredientes y lo haré para usted.$B$B¿En que está interesado?',
--     'No tienes que perder el tiempo con la alquimia. Sólo tráeme los ingredientes y lo haré para usted.$B$B¿En que está interesado?',
--     '', ''
-- );
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
    '¡VAMOS!', '¡VAMOS!',
    '', '',
    '', ''
),
(
    91432,
    'Si vous vous rendez dans le bois Sombre, essayez de cueillir des fleurs des chants. Elles ont de puissantes propriétés magiques. Notez néanmoins que vous devrez peut-être combattre quelques arakkoa pour les obtenir.',
    'Si vous vous rendez dans le bois Sombre, essayez de cueillir des fleurs des chants. Elles ont de puissantes propriétés magiques. Notez néanmoins que vous devrez peut-être combattre quelques arakkoa pour les obtenir.',
    '', '',
    'Los Elfos de la Noche están tratando de resucitar un ancestro para ser utilizado contra nosotros usando el poder de los artefactos encontrados aquí en Ashran.$B$BSi ves cualquier alianza por ahí, tomar cualquier fragmento que llevan por la fuerza.',
    'Los Elfos de la Noche están tratando de resucitar un ancestro para ser utilizado contra nosotros usando el poder de los artefactos encontrados aquí en Ashran.$B$BSi ves cualquier alianza por ahí, tomar cualquier fragmento que llevan por la fuerza.',
    '', ''
),
(
    91496,
    'Si l''Alliance vous pose des problèmes dans le coin, utilisez l''une des fusées de Frizzo Grilletorpille.$B$BNous ne pourrons vous aider que si nous savons où vous vous trouvez.',
    'Si l''Alliance vous pose des problèmes dans le coin, utilisez l''une des fusées de Frizzo Grilletorpille.$B$BNous ne pourrons vous aider que si nous savons où vous vous trouvez.',
    '', '',
    'Si te encuentras con algún problema con la alianza por aquí, utiliza una bengala de Frizzo Sparkrocket.$B$BNosotros no podemos ayudar si no sabemos donde estás.',
    'Si te encuentras con algún problema con la alianza por aquí, utiliza una bengala de Frizzo Sparkrocket.$B$BNosotros no podemos ayudar si no sabemos donde estás.',
    '', ''
),
(
    87402,
    'Tous les grands seigneurs de guerre ne se ressemblent pas, $c. Certains sont arrivés là à force de temps et d''efforts, d''autres par leurs apptitude au combat. Devinez à quelle catégorie j''appartiens.',
    'Tous les grands seigneurs de guerre ne se ressemblent pas, $c. Certains sont arrivés là à force de temps et d''efforts, d''autres par leurs apptitude au combat. Devinez à quelle catégorie j''appartiens.',
    '', '',
    'He oído que eres una fuerza a tener en cuenta en la arena, $n.',
    'He oído que eres una fuerza a tener en cuenta en la arena, $n.',
    '', ''
),
(
    89855,
    'Les fragments d''artéfacts contiennent suffisamment d''énergie margique pour remettre sur pied cette fureur terrestre.$B$BCombien m''en apportez-vous ?',
    'Les fragments d''artéfacts contiennent suffisamment d''énergie margique pour remettre sur pied cette fureur terrestre.$B$BCombien m''en apportez-vous ?',
    '', '',
    'Los fragmentos de artefactos contienen suficiente poder mágico para restaurar esta furia de la Tierra a toda su fuerza.$B$B¿Cuántos nos has traído?',
    'Los fragmentos de artefactos contienen suficiente poder mágico para restaurar esta furia de la Tierra a toda su fuerza.$B$B¿Cuántos nos has traído?',
    '', ''
),
(
    91435,
    'Les flammes se dégageant des braseros près de l''arène des Conquérants ont l''air ordinaires de premier abord. Cependant, je pense qu''ils ont été enchantés par l''antique magie de la Masse-Sanglante.$B$BJ''étais étrangement attirée vers eux. Quand j''en ai touché un, j''ai soudain été remplie d''énergie magique.',
    'Les flammes se dégageant des braseros près de l''arène des Conquérants ont l''air ordinaires de premier abord. Cependant, je pense qu''ils ont été enchantés par l''antique magie de la Masse-Sanglante.$B$BJ''étais étrangement attirée vers eux. Quand j''en ai touché un, j''ai soudain été remplie d''énergie magique.',
    '', '',
    '', '',
    '', ''
),
(
    85343,
    'Kronus va patrouiller le long de la route de la Gloire et se chargera d''éliminer les forces de l''Alliance qui croiseront son chemin.',
    'Kronus va patrouiller le long de la route de la Gloire et se chargera d''éliminer les forces de l''Alliance qui croiseront son chemin.',
    '', '',
    '', '',
    '', ''
),
(
    85461,
    'Les portes se trouvent au nord-ouest et au sud-est ; elles permettent de se déplacer de l''un de ces deux points à l''autre.',
    'Les portes se trouvent au nord-ouest et au sud-est ; elles permettent de se déplacer de l''un de ces deux points à l''autre.',
    '', '',
    '', '',
    '', ''
),
(
    85460,
    'Les mages ouvriront un portail à l''ouest d''A''shran. Il vous permettra de rentrer directement à l''avant-poste de Fer-de-Lance.',
    'Les mages ouvriront un portail à l''ouest d''A''shran. Il vous permettra de rentrer directement à l''avant-poste de Fer-de-Lance.',
    '', '',
    '', '',
    '', ''
),
(
    85334,
    'Nos meilleurs Chevaucheurs de loups vont arpenter A''shran pour protéger la province et servir sa population. Et mettre en pièces ces chiens de l''Alliance.',
    'Nos meilleurs Chevaucheurs de loups vont arpenter A''shran pour protéger la province et servir sa population. Et mettre en pièces ces chiens de l''Alliance.',
    '', '',
    '', '',
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
    'Actualmente hemos recogido $9250w. Necesitamos $9706w total.',
    'Actualmente hemos recogido $9250w. Necesitamos $9706w total.',
    '', ''
),
(
    84631,
    'Pour l''instant, nous en avons récolté $9261w, mais il nous en faut au total $9708w.',
    'Pour l''instant, nous en avons récolté $9261w, mais il nous en faut au total $9708w.',
    '', '',
    'Actualmente hemos recogido $9261w. Necesitamos $9708w total.',
    'Actualmente hemos recogido $9261w. Necesitamos $9708w total.',
    '', ''
),
(
    84926,
    'Pour l''instant, nous en avons récolté $9314w, mais il nous en faut au total $9705w.',
    'Pour l''instant, nous en avons récolté $9314w, mais il nous en faut au total $9705w.',
    '', '',
    'Actualmente hemos recogido $9314w. Necesitamos $9705w total.',
    'Actualmente hemos recogido $9314w. Necesitamos $9705w total.',
    '', ''
),
(
    85336,
    'Pour l''instant, nous en avons récolté $9313w, mais il nous en faut au total $9705w.',
    'Pour l''instant, nous en avons récolté $9313w, mais il nous en faut au total $9705w.',
    '', '',
    'Actualmente hemos recogido $9313w. Necesitamos $9705w total.',
    'Actualmente hemos recogido $9313w. Necesitamos $9705w total.',
    '', ''
),
(
    85339,
    'Pour l''instant, nous en avons récolté $9312w, mais il nous en faut au total $9707w.',
    'Pour l''instant, nous en avons récolté $9312w, mais il nous en faut au total $9707w.',
    '', '',
    'Actualmente hemos recogido $9312w. Necesitamos $9707w total.',
    'Actualmente hemos recogido $9312w. Necesitamos $9707w total.',
    '', ''
),
(
    85340,
    'Pour l''instant, nous en avons récolté $9262w, mais il nous en faut au total $9708w.',
    'Pour l''instant, nous en avons récolté $9262w, mais il nous en faut au total $9708w.',
    '', '',
    'Actualmente hemos recogido $9262w. Necesitamos $9708w total.',
    'Actualmente hemos recogido $9262w. Necesitamos $9708w total.',
    '', ''
),
(
    85341,
    'Pour l''instant, nous en avons récolté $9253w, mais il nous en faut au total $9706w.',
    'Pour l''instant, nous en avons récolté $9253w, mais il nous en faut au total $9706w.',
    '', '',
    'Actualmente hemos recogido $9253w. Necesitamos $9706w total.',
    'Actualmente hemos recogido $9253w. Necesitamos $9706w total.',
    '', ''
),
(
    86036,
    'Pour l''instant, nous en avons récolté $9311w, mais il nous en faut au total $9707w.',
    'Pour l''instant, nous en avons récolté $9311w, mais il nous en faut au total $9707w.',
    '', '',
    'Actualmente hemos recogido $9311w. Necesitamos $9707w total.',
    'Actualmente hemos recogido $9311w. Necesitamos $9707w total.',
    '', ''
);

DELETE FROM creature_text WHERE entry IN (88675, 88676, 88771, 88772);
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
(88772, 0, 1, 'Have you seen how many Death Knights are within our ranks? Why is it that we still can''t penetrate Stormshield? Are they not using Necrotic Plague?', 12, 0, 100, 0, 0, 0, 'LegionnaireHellaxe1');

DELETE FROM locales_creature_text WHERE entry IN (88675, 88676, 88771, 88772);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    88675, 0, 0,
    'Ces gobelins de Gentepression... faudrait les enfermer. Ils feraient n''importe quoi pour de l''argent !',
    '', '', ''
),
(
    88675, 1, 0,
    'Ça y est, ils vendent des souvenirs !',
    '', '', ''
),
(
    88675, 2, 0,
    'J''aimerais bien. Ils assemblent des bouts de reliques à la va-vite et vendent ça comme bijoux. Traiter des objets aussi inestimables de la sorte, franchement !',
    '', '', ''
),
(
    88675, 3, 0,
    'Est-ce que Belloc en sait plus sur l''endroit où se trouve ce mystérieux artéfact ?',
    '', '', ''
),
(
    88675, 4, 0,
    'Nous devons nous hâter si nous voulons le trouver avant le docteur Jones.',
    '', '', ''
),
(
    88676, 0, 0,
    'Qu''est-ce qu''ils ont encore fait ?',
    '', '', ''
),
(
    88676, 1, 0,
    'On se remet à peine de notre assaut sur la Porte des ténèbres, et ils montent un business touristique ? C''est une plaisanterie !',
    '', '', ''
),
(
    88676, 2, 0,
    'Il a suivi quelques pistes, mais rien de vraiment concret pour l''instant.',
    '', '', ''
),
(
    88676, 3, 0,
    'Je suis d''accord. On ne sait pas ce dont l''Alliance serait capable avec une arme aussi puissante. Nous devons trouver plus de soutien avant que notre quête ne devienne une cause perdue.',
    '', '', ''
),
(
    88771, 0, 0,
    'Nos espions nous ont appris que l''Alliance construit un engin de forage. Vous pensez que c''est pour la mine ?',
    '', '', ''
),
(
    88771, 0, 1,
    'Une trêve ? Avec l''Alliance ? Plutôt mourir.',
    '', '', ''
),
(
    88772, 0, 0,
    'Si seulement nous pouvions faire passer un zeppelin à travers le portail. Nous pourrions raser Bouclier-des-Tempêtes depuis les airs !',
    '', '', ''
),
(
    88772, 0, 1,
    'Vous avez vu le nombre de chevaliers de la mort parmi nous ? Comment se fait-il que nous n''arrivions toujours pas à entrer dans Bouclier-des-Tempêtes ? N''utilisent-ils donc pas leur Peste nécrotique ?',
    '', '', ''
);