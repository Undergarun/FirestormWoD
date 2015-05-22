UPDATE creature_template SET ScriptName='boss_SlaveWatcherCrushto', mechanic_immune_mask = 617299839, flags_extra = flags_extra | 0x01 WHERE entry=74787;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry IN (75303, 75585, 75586, 83650, 83808);

DELETE FROM creature_text WHERE entry IN (75210, 74787, 74355);
INSERT INTO creature_text VALUES
(75210, 0, 0, 'Me stop them!', 14, 0, 100, 0, 0, 0, 'WarderAttack1'),
(75210, 0, 1, 'We make them pay!', 14, 0, 100, 0, 0, 0, 'WarderAttack2'),
(75210, 1, 0, 'AAAAAHHH! SO MUCH FIRE!', 14, 0, 100, 0, 0, 0, 'WarderFire'),

(74787, 0, 0, 'Guards! Stop yapping, start smashing!', 14, 0, 100, 0, 0, 41647, 'CrushtoIntro0'),
(74787, 0, 1, 'My slaves! Those were Bloodmaul property!', 14, 0, 100, 0, 0, 41648, 'CrushtoIntro1'),
(74787, 1, 0, 'Mmm-hnng - yeah, come here! I''ll put you in chains.', 14, 0, 100, 0, 0, 41645, 'CrushtoAggro'),
(74787, 2, 0, 'Get that one!', 14, 0, 100, 0, 0, 41652, 'CrushtoMiner0'),
(74787, 2, 1, 'Slaves! Fight or die!', 14, 0, 100, 0, 0, 41654, 'CrushtoMiner1'),
(74787, 3, 0, 'Run away, little ones! Ha ha!', 14, 0, 100, 0, 0, 41653, 'CrushtoFerociousYell'),
(74787, 4, 0, 'Ha ha! Smashed.', 14, 0, 100, 0, 0, 41649, 'CrushtoSlay0'),
(74787, 4, 1, 'Not fit for capture!', 14, 0, 100, 0, 0, 41650, 'CrushtoSlay1'),
(74787, 4, 2, 'Next! Ha ha!', 14, 0, 100, 0, 0, 41651, 'CrushtoSlay2'),
(74787, 5, 0, 'Ugg... we kill you all for this...', 14, 0, 100, 0, 0, 41646, 'CrushtoDeath'),

(74355, 0, 0, 'Here''s to a less miserable future!', 12, 0, 25, 10, 0, 0, 'Captured miner'),
(74355, 0, 1, 'I will be strong and stand with you!', 12, 0, 25, 10, 0, 0, 'Captured miner'),
(74355, 0, 2, 'We will not be slaves again!', 12, 0, 25, 10, 0, 0, 'Captured miner');

DELETE FROM locales_creature_text WHERE entry IN (75210, 74787, 74355);
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(75210, 0, 0, 'Je arrêter eux !', '¡Yo parar ellos!'),
(75210, 0, 1, 'On leur fait payer !', '¡Nosotros hacer pagar!'),
(75210, 1, 0, 'HAAAAAAAAAAAA ! TOUT CE FEU !', '¡AAAAAHHH! ¡TANTO FUEGO!'),

(74787, 0, 0, 'Gardes ! Arrêtez de jacasser, et frappez !', '¡Guardias! ¡Dejad de ladrar y luchad!'),
(74787, 0, 1, '... Mes esclaves ! Ils appartenaient à la Masse-Sanglante !', '... ¡Mis esclavos! ¡Pertenecen a los Machacasangre!'),
(74787, 1, 0, 'Mmm-hmm... Approchez ! Je vais vous enchaîner.', 'Mmm-hmm... ¡Acercáos! Os voy a encadenar.'),
(74787, 2, 0, 'Capturez cette punaise !', '¡Capturad a ese bicho!'),
(74787, 2, 1, 'Esclaves ! Battez-vous ou mourez !', '¡Esclavos! ¡Luchad o morid!'),
(74787, 3, 0, 'Fuyez, misérables vermines ! Ha ha !', '¡Huid, miserables sabandijas! ¡Ja Já!'),
(74787, 4, 0, 'Ha ha ! De la bouillie !', '¡Ja já! ¡Papilla!'),
(74787, 4, 1, 'Du menu fretin !', '¡Un menú frito!'),
(74787, 4, 2, 'Suivant ! Ha ha !', '¡Siguiente! ¡Ja já!'),
(74787, 5, 0, 'Argh... vous mourrez pour ça...', 'Argh... Moriréis por esto...'),

(74355, 0, 0, 'Voici un avenir moins misérable !', '¡Os daré un futuro menos miserable!'),
(74355, 0, 1, 'Je serais fort et avec vous !', '¡Seré fuerte con vosotros!'),
(74355, 0, 2, 'Nous ne serons plus jamais des esclaves !', '¡No seremos nunca más esclavos!');