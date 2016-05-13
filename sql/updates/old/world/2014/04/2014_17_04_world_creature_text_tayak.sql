DELETE FROM creature_text WHERE entry = 62543;
INSERT INTO creature_text VALUES
(62543, 0, 0, 'On your guard, invaders. I, Ta''yak, Lord of Blades, will be your opponent.', 14, 0, 100, 0, 0, 29252, 'VO_HOF_TAYAK_AGRO_01'),
(62543, 1, 0, 'I''d always hoped to cut someone like that someday, to hear that sound. But to have it happen to my own prothorax is ridiculous.', 14, 0, 100, 0, 0, 29253, 'VO_HOF_TAYAK_DEATH_01'),
(62543, 2, 0, 'Mere initiates.', 14, 0, 25, 0, 0, 29254, 'VO_HOF_TAYAK_EVENT_ROOM_01'),
(62543, 2, 1, 'They should have studied harder.', 14, 0, 25, 0, 0, 29255, 'VO_HOF_TAYAK_EVENT_ROOM_02'),
(62543, 2, 2, 'One technique alone is not enough.', 14, 0, 25, 0, 0, 29256, 'VO_HOF_TAYAK_EVENT_ROOM_03'),
(62543, 2, 3, 'They failed their test. Will you?', 14, 0, 25, 0, 0, 29257, 'VO_HOF_TAYAK_EVENT_ROOM_04'),
(62543, 3, 0, 'They approach? Good. Now, if only my students were here to see and learn from the coming display of martial prowess...', 14, 0, 100, 0, 0, 29258, 'VO_HOF_TAYAK_INTRO_01'),
(62543, 4, 0, 'A perfect cut.', 14, 0, 50, 0, 0, 29259, 'VO_HOF_TAYAK_SLAY_01'),
(62543, 4, 1, 'This is the technique of a Blade Lord.', 14, 0, 50, 0, 0, 29260, 'VO_HOF_TAYAK_SLAY_02'),
(62543, 5, 0, 'Can you follow my blade?', 14, 0, 100, 0, 0, 29261, 'VO_HOF_TAYAK_SPELL_01');

DELETE FROM locales_creature_text WHERE entry = 62543;
INSERT INTO locales_creature_text VALUES
(62543, 0, 0, 'Mettez-vous en garde, envahisseurs. Moi, Ta''yak, seigneur des Lames, je serai votre adversaire.', '', '', '', '', '', '', '', '', ''),
(62543, 1, 0, 'J''ai toujours espéré réussir cette découpe, pour entendre ce bruit. Mais que ça arrive sur mon propre prothorax, c?est ridicule.', '', '', '', '', '', '', '', '', ''),
(62543, 2, 0, 'Simple introduction.', '', '', '', '', '', '', '', '', ''),
(62543, 2, 1, 'Ils auraient du étudier plus dur.', '', '', '', '', '', '', '', '', ''),
(62543, 2, 2, 'Une seule technique ne suffit pas.', '', '', '', '', '', '', '', '', ''),
(62543, 2, 3, 'Ils ont raté leur test. Et vous ?', '', '', '', '', '', '', '', '', ''),
(62543, 3, 0, 'Ils approchent ? Bien. Maintenant, si seulement mes élèves pouvaient être là pour voir et apprendre de la démonstration de prouesses martiales à venir...', '', '', '', '', '', '', '', '', ''),
(62543, 4, 0, 'Une coupe parfaite.', '', '', '', '', '', '', '', '', ''),
(62543, 4, 1, 'Voilà la technique d''un seigneur des lames.', '', '', '', '', '', '', '', '', ''),
(62543, 5, 0, 'Êtes-vous capable de suivre ma lame ?', '', '', '', '', '', '', '', '', '');