DELETE FROM creature_text WHERE entry = 69017;
INSERT INTO creature_text VALUES
(69017, 0, 0, 'So cold, so dark...', 12, 0, 100, 0, 0, 35744, 'PRIMORDIUS_INTRO_01'),
(69017, 1, 0, 'We will not be forgotten, no, no, no...', 12, 0, 100, 0, 0, 35745, 'PRIMORDIUS_INTRO_02'),
(69017, 2, 0, 'We are the perfect one, yes, yes, not a failure, no, no, not a failure...', 12, 0, 100, 0, 0, 35746, 'PRIMORDIUS_INTRO_03'),
(69017, 3, 0, 'What is this? What do they want? To punish us? No, no...', 12, 0, 100, 0, 0, 35747, 'PRIMORDIUS_INTRO_04'),
(69017, 4, 0, 'Your flesh, yes yes, so perfect, GIVE IT TO US!', 14, 0, 100, 0, 0, 35742, 'PRIMORDIUS_AGGRO'),
(69017, 5, 0, 'No, no, no! The pain returns!', 14, 0, 100, 0, 0, 35749, 'PRIMORDIUS_EVOLUTION_01'),
(69017, 5, 1, 'It tears at us from the inside!', 14, 0, 100, 0, 0, 36112, 'PRIMORDIUS_EVOLUTION_02'),
(69017, 6, 0, 'Who is perfect now? Yes, yes...', 14, 0, 100, 0, 0, 35748, 'PRIMORDIUS_SLAY'),
(69017, 7, 0, 'Again... we are torn apart... Again... to the cold darkness...', 14, 0, 100, 0, 0, 35743, 'PRIMORDIUS_DEATH');

DELETE FROM locales_creature_text WHERE entry = 69017;
INSERT INTO locales_creature_text VALUES
(69017, 0, 0, '', 'Si froid, si sombre...', '', '', '', 'Tan frío... tan oscuro...', '', '', '', ''),
(69017, 1, 0, '', 'Nous ne serons pas oubliés, non, non, non...', '', '', '', 'Nosotros no seremos olvidados, no, no, no...', '', '', '', ''),
(69017, 2, 0, '', 'Nous sommes parfait, oui, oui, aucune faille, non, non, aucune faille...', '', '', '', 'Somos la perfección, sí, sí, no un fracaso, no no, no un fracaso...', '', '', '', ''),
(69017, 3, 0, '', 'Qu''est-ce donc ? Que veulent-ils ? Nous punir ? Non, non...', '', '', '', '¿Qué es esto? ¿Qué es lo que quieren? ¿Castigarnos? No, no...', '', '', '', ''),
(69017, 4, 0, '', 'Votre chair, oui, oui, si parfaite, DONNEZ-LA-NOUS !', '', '', '', 'Tu carne, sí sí, tan perfecta, ¡DÁNOSLA!', '', '', '', ''),
(69017, 5, 0, '', 'Non non non, la douleur revient !', '', '', '', '¡No, no, no! ¡El dolor regresa!', '', '', '', ''),
(69017, 5, 1, '', 'Elle nous déchire de l’intérieur !', '', '', '', '¡Nos rasga por dentro!', '', '', '', ''),
(69017, 6, 0, '', 'Qui est parfait maintenant ? Oui, oui...', '', '', '', '¿Quién es perfecto ahora? Sí, sí...', '', '', '', ''),
(69017, 7, 0, '', 'Encore... déchiré... Encore… les froides ténèbres...', '', '', '', 'De nuevo... estamos rasgados... De nuevo... a la fría oscuridad...', '', '', '', '');