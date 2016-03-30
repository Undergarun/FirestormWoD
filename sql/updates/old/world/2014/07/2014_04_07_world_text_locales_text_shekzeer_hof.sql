-- Grand Empress Shek'zeer
-- Main
DELETE FROM creature_text WHERE entry = 62837;
INSERT INTO creature_text (entry, groupid, id, TEXT, TYPE, probability, sound, COMMENT) VALUES
(62837, 0, 0, 'Seal the gates! Bar the exits! Destroy these intruders!', 14, 100, 29278, 'VO_HOF_EMPRESS_INTRO_01'),
(62837, 1, 0, 'Death to all who dare challenge my empire!', 14, 100, 29283, 'VO_HOF_EMPRESS_AGRO_01'),
(62837, 2, 0, 'Sing... yes! Sing in agony!', 14, 100, 29281, 'VO_HOF_EMPRESS_SPELL_01'),
(62837, 3, 0, 'You will serve ME now!', 14, 100, 29282, 'VO_HOF_EMPRESS_SPELL_02'),
(62837, 4, 0, 'Power... I must have more power! Guards! Protect your empress!', 14, 100, 29270, 'VO_HOF_EMPRESS_EVENT_RECHARGE_1A'),
(62837, 5, 0, 'I will dispose of the outsiders.', 14, 100, 29271, 'VO_HOF_EMPRESS_EVENT_RECHARGE_1B'),
(62837, 6, 0, 'More... more! I must have more!', 14, 100, 29272, 'VO_HOF_EMPRESS_EVENT_RECHARGE_2'),
(62837, 7, 0, 'Such is the fate of those who question authority.', 14, 100, 29273, 'VO_HOF_EMPRESS_EVENT_RESET'),
(62837, 8, 0, 'Taste defeat, stranger!', 14, 100, 29279, 'VO_HOF_EMPRESS_SLAY_01'),
(62837, 8, 1, 'Breathe no longer!', 14, 100, 29280, 'VO_HOF_EMPRESS_SLAY_02'),
(62837, 9, 0, 'Augh... ah ... I... have failed.', 14, 100, 29268, 'VO_HOF_EMPRESS_DEATH_01'),
(62837, 10, 0, 'Yes, master.', 14, 100, 29269, 'VO_HOF_EMPRESS_EVENT_DESTROY_CHAMBER');

-- Locales
DELETE FROM locales_creature_text WHERE entry = 62837;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6, text_loc7) VALUES
(62837, 0, 0, 'Scellez les portes ! Barrez les issues ! Détruisez ces intrus !', 'Selle las puertas! Dirigir los asuntos! Destruir estos intrusos!', 'Selle las puertas! Dirigir los asuntos! Destruir estos intrusos!'),
(62837, 1, 0, 'Mort à tous ceux qui osent défier mon empire !', '¡Muerta a todo aquel que desafie a mi imperio!', '¡Muerta a todo aquel que desafie a mi imperio!'),
(62837, 2, 0, 'Chantez... oui ! Un chant d''agonie !', '¡Cantad! ¡Si, cantad de agonia!', '¡Cantad! ¡Si, cantad de agonia!'),
(62837, 3, 0, 'C''est MOI que vous servez, maintenant !', '¡Es a mí a quien usted sirve ahora!', '¡Es a mí a quien usted sirve ahora!'),
(62837, 4, 0, 'Puissance... Il me faut plus de puissance ! Gardes ! Protégez votre impératrice !', '¡Poder... necessito màs poder! ¡Guardias! ¡Proteged a vuestra Emperatriz!', '¡Poder... necessito màs poder! ¡Guardias! ¡Proteged a vuestra Emperatriz!'),
(62837, 5, 0, 'Je vais me débarrasser de ces étrangers.', 'Me encargaré de los forasteros.', 'Me encargaré de los forasteros.'),
(62837, 6, 0, 'Encore ! Plus... Il m''en faut plus !', '¡Más! ¡Más ... Debo tener más!', '¡Más! ¡Más ... Debo tener más!'),
(62837, 7, 0, 'Tel est le destin de qui s''oppose à l''autorité.', 'Tal es el destino de que se opone a la autoridad.', 'Tal es el destino de que se opone a la autoridad.'),
(62837, 8, 0, 'Apprenez le goût de la défaite !', '¡Aprende el sabor de la derrota!', '¡Aprende el sabor de la derrota!'),
(62837, 8, 1, 'Cessez de respirer !', '¡Deje de respirar!', '¡Deje de respirar!'),
(62837, 9, 0, 'Argh... ah... J''ai... éch... choué.', 'Agh... ah... He... fracasado.', 'Agh... ah... He... fracasado.'),
(62837, 10, 0, 'Oui, maître.', 'Sí, maestro', 'Sí, maestro.');

-- Sha of Fear
DELETE FROM creature_text WHERE entry = 63942;
INSERT INTO creature_text (entry, groupid, id, TEXT, TYPE, probability, sound, COMMENT) VALUES
(63942, 0, 0, 'USELESS! Fall limp and die before me, wretched insect. Your kind are worthless to me!', 14, 100, 32645, 'VO_HOF_FEAR_EVENT_DEFEAT_A'),
(63942, 1, 0, 'As for the rest of you, seek me if you dare. Like the others, you will all die screaming.', 14, 100, 32646, 'VO_HOF_FEAR_EVENT_DEFEAT_B'),
(63942, 2, 0, 'No more excuses, Empress! Eliminate these cretins, or I will kill you myself!', 14, 100, 32647, 'VO_HOF_FEAR_EVENT_DEFEAT_C'),
(63942, 3, 0, 'You must destroy these outsiders, Empress. They have come to conquer your people!', 14, 100, 32648, 'VO_HOF_FEAR_EVENT_RECHARGE_1'),
(63942, 4, 0, 'The mantid are under MY command, fools. See how she succumbs to me.', 14, 100, 32649, 'VO_HOF_FEAR_EVENT_RECHARGE_2');

DELETE FROM locales_creature_text WHERE entry = 63942;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6, text_loc7) VALUES
(63942, 0, 0, 'INCAPABLE ! Face contre terre et meurs à mes pieds, misérable insecte. Les tiens ne valent rien pour moi !', '¡INUTIL! Cae a mis pies y mueré, insecto despreciable. ¡Ty especie no me sirve de nada!', '¡INUTIL! Cae a mis pies y mueré, insecto despreciable. ¡Ty especie no me sirve de nada!'),
(63942, 1, 0, 'Quant à vous autres, venez me chercher si vous l''oser. Vous mourrez en hurlant, comme tous les autres.', 'En cuanto a vosotros: venid a por mi si os atrevéis. Moriréis gritando, como todos les demàs...', 'En cuanto a vosotros: venid a por mi si os atrevéis. Moriréis gritando, como todos les demàs...'),
(63942, 2, 0, 'Assez de vos excuses, Impératrice ! Eliminez ces crétins, ou je vous achève moi-même !', '¡Se acabaron las excusas, Emperatriz! ¡Acaba con estos despreciables o te mataré yo mismo!', '¡Se acabaron las excusas, Emperatriz! ¡Acaba con estos despreciables o te mataré yo mismo!'),
(63942, 3, 0, 'Vous devez détruire ces étrangers, Impératrice. Ils viennent pour conquérir votre peuple !', '¡Debes acabar con estos forasteros, Emperatriz! ¡Han venido a conquistar a tu pueblo!', '¡Debes acabar con estos forasteros, Emperatriz! ¡Han venido a conquistar a tu pueblo!'),
(63942, 4, 0, 'Les Mantides sont sous MON contrôle, imbéciles. Voyez comme elle succombe à mon charme.', 'Los Mantides están bajo mi control, los necios. Vea cómo sucumbió a mi encanto.', 'Los Mantides están bajo mi control, los necios. Vea cómo sucumbió a mi encanto.');