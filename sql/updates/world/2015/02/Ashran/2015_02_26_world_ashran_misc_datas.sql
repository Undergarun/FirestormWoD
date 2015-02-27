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
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(80858, 0, 0, 'Je suis de retour ! Pauvres gueux, soumettez-vous à la puissance de l’empire ogre !', '¡He vuelto! ¡Arrodillaos ante el poder del gran Imperio ogro!'),
(80858, 1, 0, 'Petits guerriers bleus, vous vous êtes montrés dignes d''échapper à mon courroux.', 'Guerreros azules pequeños, que han demostrado que son dignos de evitar mi ira.'),
(80858, 2, 0, 'Les rouges, oui, ils seront témoins de ma dévastation !', 'Los rojos, sí, ¡van a dar testimonio de mi devastación!'),
(80858, 3, 0, 'Plus d''ennemis ! Donnez-moi plus d''ennemis à égorger !', '¡Más, traer más a morir en mi espada!'),
(80858, 3, 1, 'Quelle folie de vous opposer à moi, gringalets !', '¡Eres un necio para enfrentarte a mí, debilucho!'),
(80858, 3, 2, 'Des insectes, vous n''êtes que des insectes pour moi !', '¡Insectos, sois insectos para mí!'),
(80858, 3, 3, 'Ha ha ha ha !', '¡Ja, ja, ja, ja!'),
(80858, 3, 4, 'Une facilité déconcertante !', '¡Una facilidad desconcertante!'),
(80858, 4, 0, 'L''empire... renaîtra !', '¡El... Imperio... renacerá!');

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

DELETE FROM creature_equip_template WHERE entry IN (80858, 81725, 81726);
INSERT INTO creature_equip_template VALUES
(80858, 1, 116377, 0, 0),
(81725, 1, 110590, 0, 0),
(81726, 1, 110590, 0, 0);