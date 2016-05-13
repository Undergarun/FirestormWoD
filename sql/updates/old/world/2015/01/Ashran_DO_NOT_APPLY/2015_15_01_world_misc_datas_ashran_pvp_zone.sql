DELETE FROM outdoorpvp_template WHERE TypeId = 6;
INSERT INTO outdoorpvp_template VALUE
(6, 'outdoorpvp_ashran', 'Ashran');

UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=85113;
DELETE FROM `npc_vendor` WHERE `entry` = 85113 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(85113, 0, 2880, 0, 0, 0, 1), 
(85113, 0, 2901, 0, 0, 0, 1), 
(85113, 0, 3466, 0, 0, 0, 1), 
(85113, 0, 3857, 0, 0, 0, 1), 
(85113, 0, 5956, 0, 0, 0, 1), 
(85113, 0, 18567, 0, 0, 0, 1);

UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=82877;
DELETE FROM `npc_vendor` WHERE `entry` = 82877 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(82877, 0, 111067, 0, 0, 77777, 1), 
(82877, 0, 111068, 0, 0, 77777, 1), 
(82877, 0, 111069, 0, 0, 77777, 1), 
(82877, 0, 111070, 0, 0, 77777, 1), 
(82877, 0, 111071, 0, 0, 77777, 1), 
(82877, 0, 111072, 0, 0, 77777, 1), 
(82877, 0, 111073, 0, 0, 77777, 1), 
(82877, 0, 111074, 0, 0, 77777, 1), 
(82877, 0, 111075, 0, 0, 77777, 1), 
(82877, 0, 111076, 0, 0, 77777, 1), 
(82877, 0, 111077, 0, 0, 77777, 1), 
(82877, 0, 111078, 0, 0, 77777, 1), 
(82877, 0, 111079, 0, 0, 77777, 1), 
(82877, 0, 111080, 0, 0, 77777, 1), 
(82877, 0, 111081, 0, 0, 77777, 1), 
(82877, 0, 111082, 0, 0, 77777, 1), 
(82877, 0, 111083, 0, 0, 77777, 1), 
(82877, 0, 111084, 0, 0, 77777, 1), 
(82877, 0, 111085, 0, 0, 77777, 1), 
(82877, 0, 111086, 0, 0, 77777, 1), 
(82877, 0, 111087, 0, 0, 77777, 1), 
(82877, 0, 111088, 0, 0, 77777, 1), 
(82877, 0, 111089, 0, 0, 77777, 1), 
(82877, 0, 111090, 0, 0, 77777, 1), 
(82877, 0, 111091, 0, 0, 77777, 1), 
(82877, 0, 111092, 0, 0, 77777, 1), 
(82877, 0, 111093, 0, 0, 77777, 1), 
(82877, 0, 111094, 0, 0, 77777, 1), 
(82877, 0, 111095, 0, 0, 77777, 1), 
(82877, 0, 111096, 0, 0, 77777, 1), 
(82877, 0, 111097, 0, 0, 77777, 1), 
(82877, 0, 111098, 0, 0, 77777, 1), 
(82877, 0, 111099, 0, 0, 77777, 1), 
(82877, 0, 111100, 0, 0, 77777, 1), 
(82877, 0, 111101, 0, 0, 77777, 1), 
(82877, 0, 111112, 0, 0, 77777, 1), 
(82877, 0, 111113, 0, 0, 77777, 1), 
(82877, 0, 111114, 0, 0, 77777, 1), 
(82877, 0, 111115, 0, 0, 77777, 1), 
(82877, 0, 111116, 0, 0, 77777, 1), 
(82877, 0, 111117, 0, 0, 77777, 1), 
(82877, 0, 111118, 0, 0, 77777, 1), 
(82877, 0, 111119, 0, 0, 77777, 1), 
(82877, 0, 111120, 0, 0, 77777, 1), 
(82877, 0, 111121, 0, 0, 77777, 1), 
(82877, 0, 111122, 0, 0, 77777, 1), 
(82877, 0, 111123, 0, 0, 77777, 1), 
(82877, 0, 111124, 0, 0, 77777, 1), 
(82877, 0, 111125, 0, 0, 77777, 1), 
(82877, 0, 111126, 0, 0, 77777, 1), 
(82877, 0, 111127, 0, 0, 77777, 1), 
(82877, 0, 111128, 0, 0, 77777, 1), 
(82877, 0, 111129, 0, 0, 77777, 1), 
(82877, 0, 111130, 0, 0, 77777, 1), 
(82877, 0, 111131, 0, 0, 77777, 1), 
(82877, 0, 111132, 0, 0, 77777, 1), 
(82877, 0, 111133, 0, 0, 77777, 1), 
(82877, 0, 111134, 0, 0, 77777, 1), 
(82877, 0, 111135, 0, 0, 77777, 1), 
(82877, 0, 111136, 0, 0, 77777, 1), 
(82877, 0, 111137, 0, 0, 77777, 1), 
(82877, 0, 111138, 0, 0, 77777, 1), 
(82877, 0, 111139, 0, 0, 77777, 1), 
(82877, 0, 111140, 0, 0, 77777, 1), 
(82877, 0, 111141, 0, 0, 77777, 1), 
(82877, 0, 111142, 0, 0, 77777, 1), 
(82877, 0, 111143, 0, 0, 77777, 1), 
(82877, 0, 111144, 0, 0, 77777, 1), 
(82877, 0, 111145, 0, 0, 77777, 1), 
(82877, 0, 111146, 0, 0, 77777, 1), 
(82877, 0, 111147, 0, 0, 77777, 1), 
(82877, 0, 111148, 0, 0, 77777, 1), 
(82877, 0, 111149, 0, 0, 77777, 1), 
(82877, 0, 111155, 0, 0, 77777, 1), 
(82877, 0, 111156, 0, 0, 77777, 1), 
(82877, 0, 111157, 0, 0, 77777, 1), 
(82877, 0, 111158, 0, 0, 77777, 1), 
(82877, 0, 111159, 0, 0, 77777, 1), 
(82877, 0, 111160, 0, 0, 77777, 1), 
(82877, 0, 111161, 0, 0, 77777, 1), 
(82877, 0, 111162, 0, 0, 77777, 1), 
(82877, 0, 111163, 0, 0, 77777, 1), 
(82877, 0, 111174, 0, 0, 77777, 1), 
(82877, 0, 111175, 0, 0, 77777, 1), 
(82877, 0, 111176, 0, 0, 77777, 1), 
(82877, 0, 111177, 0, 0, 77777, 1), 
(82877, 0, 111178, 0, 0, 77777, 1), 
(82877, 0, 111179, 0, 0, 77777, 1), 
(82877, 0, 111180, 0, 0, 77777, 1), 
(82877, 0, 111181, 0, 0, 77777, 1), 
(82877, 0, 111182, 0, 0, 77777, 1), 
(82877, 0, 111198, 0, 0, 77777, 1), 
(82877, 0, 111199, 0, 0, 77777, 1), 
(82877, 0, 111200, 0, 0, 77777, 1), 
(82877, 0, 111201, 0, 0, 77777, 1), 
(82877, 0, 111202, 0, 0, 77777, 1), 
(82877, 0, 111203, 0, 0, 77777, 1), 
(82877, 0, 111204, 0, 0, 77777, 1), 
(82877, 0, 111205, 0, 0, 77777, 1), 
(82877, 0, 111206, 0, 0, 77777, 1), 
(82877, 0, 111207, 0, 0, 77777, 1), 
(82877, 0, 111208, 0, 0, 77777, 1), 
(82877, 0, 111209, 0, 0, 77777, 1), 
(82877, 0, 111210, 0, 0, 77777, 1), 
(82877, 0, 111211, 0, 0, 77777, 1), 
(82877, 0, 111212, 0, 0, 77777, 1), 
(82877, 0, 111213, 0, 0, 77777, 1), 
(82877, 0, 111214, 0, 0, 77777, 1), 
(82877, 0, 111215, 0, 0, 77777, 1), 
(82877, 0, 111216, 0, 0, 77777, 1), 
(82877, 0, 111217, 0, 0, 77777, 1), 
(82877, 0, 111218, 0, 0, 77777, 1), 
(82877, 0, 111219, 0, 0, 77777, 1), 
(82877, 0, 111220, 0, 0, 77777, 1), 
(82877, 0, 111221, 0, 0, 77777, 1), 
(82877, 0, 111222, 0, 0, 77777, 1), 
(82877, 0, 111223, 0, 0, 77777, 1), 
(82877, 0, 111224, 0, 0, 77777, 1), 
(82877, 0, 111225, 0, 0, 77777, 1), 
(82877, 0, 111226, 0, 0, 77777, 1), 
(82877, 0, 111227, 0, 0, 77777, 1), 
(82877, 0, 111228, 0, 0, 77777, 1), 
(82877, 0, 111229, 0, 0, 77777, 1), 
(82877, 0, 111230, 0, 0, 77777, 1), 
(82877, 0, 111231, 0, 0, 77777, 1), 
(82877, 0, 111232, 0, 0, 77777, 1), 
(82877, 0, 111233, 0, 0, 77777, 1), 
(82877, 0, 111271, 0, 0, 77777, 1), 
(82877, 0, 111272, 0, 0, 77777, 1), 
(82877, 0, 111273, 0, 0, 77777, 1), 
(82877, 0, 111274, 0, 0, 77777, 1), 
(82877, 0, 111275, 0, 0, 77777, 1), 
(82877, 0, 111276, 0, 0, 77777, 1), 
(82877, 0, 111277, 0, 0, 77777, 1), 
(82877, 0, 111278, 0, 0, 77777, 1), 
(82877, 0, 111279, 0, 0, 77777, 1), 
(82877, 0, 111280, 0, 0, 77777, 1), 
(82877, 0, 111281, 0, 0, 77777, 1), 
(82877, 0, 111282, 0, 0, 77777, 1), 
(82877, 0, 111283, 0, 0, 77777, 1), 
(82877, 0, 111284, 0, 0, 77777, 1), 
(82877, 0, 111285, 0, 0, 77777, 1), 
(82877, 0, 111286, 0, 0, 77777, 1), 
(82877, 0, 111287, 0, 0, 77777, 1), 
(82877, 0, 111288, 0, 0, 77777, 1), 
(82877, 0, 111289, 0, 0, 77777, 1), 
(82877, 0, 111290, 0, 0, 77777, 1), 
(82877, 0, 111291, 0, 0, 77777, 1), 
(82877, 0, 111292, 0, 0, 77777, 1), 
(82877, 0, 120099, 0, 0, 77777, 1), 
(82877, 0, 120103, 0, 0, 77777, 1), 
(82877, 0, 120107, 0, 0, 77777, 1);

UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=85115;
DELETE FROM `npc_vendor` WHERE `entry` = 85115 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(85115, 0, 115500, 0, 0, 0, 1), 
(85115, 0, 115518, 0, 0, 0, 1), 
(85115, 0, 116397, 0, 0, 0, 1);

UPDATE creature_template SET ScriptName = 'npc_ashran_herald' WHERE entry = 84113;
DELETE FROM creature_text WHERE entry = 84113;
INSERT INTO creature_text VALUES
(84113, 0, 0, 'The Marketplace graveyard has become a neutral zone.', 14, 0, 100, 0, 0, 0, 'ANNOUNCE_NEUTRAL'),
(84113, 1, 0, 'The Horde has taken the control of the Marketplace graveyard for 15 minutes.', 14, 0, 100, 0, 0, 0, 'ANNOUNCE_HORDE'),
(84113, 2, 0, 'The Alliance has taken the control of the Marketplace graveyard for 15 minutes.', 14, 0, 100, 0, 0, 0, 'ANNOUNCE_ALLIANCE');

DELETE FROM locales_creature_text WHERE entry = 84113;
INSERT INTO locales_creature_text VALUES
(84113, 0, 0, '', 'Le cimetière du Marché est devenu une zone neutre.', '', '', '', 'El cementerio de la zona del Mercado se ha convertido en una zona neutral.', '', '', '', ''),
(84113, 1, 0, '', 'La Horde a le contrôle du cimetière du Marché pendant 15 minutes.', '', '', '', 'La Horda ha tomado el control de la zona del Mercado durante 15 minutos.', '', '', '', ''),
(84113, 2, 0, '', 'L''Alliance a le contrôle du cimetière du Marché pendant 15 minutes.', '', '', '', 'La Alianza ha tomado el control de la zona del Mercado durante 15 minutos.', '', '', '', '');

UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = 'npc_slg_generic_mop' WHERE entry = 68553;
DELETE FROM creature_text WHERE entry = 68553;
INSERT INTO creature_text VALUES
(68553, 0, 0, 'The Horde won this phase of battle!', 41, 0, 100, 0, 0, 0, 'ANNOUNCE_HORDE_VICTORY'),
(68553, 1, 0, 'The Alliance has taken the Warspear Outpost!', 41, 0, 100, 0, 0, 0, 'ANNOUNCE_ALLIANCE_KILL_BOSS'),
(68553, 2, 0, 'The Alliance won this phase of battle!', 41, 0, 100, 0, 0, 0, 'ANNOUNCE_ALLIANCE_VICTORY'),
(68553, 3, 0, 'The Horde has taken the Stormshield Stronghold!', 41, 0, 100, 0, 0, 0, 'ANNOUNCE_HORDE_KILL_BOSS');

DELETE FROM locales_creature_text WHERE entry = 68553;
INSERT INTO locales_creature_text VALUES
(68553, 0, 0, '', 'La Horde a gagné cette phase de la bataille !', '', '', '', '¡La horda gana esta fase de la batalla!', '', '', '', ''),
(68553, 1, 0, '', 'L''Alliance a conquis l''avant poste Fer-de-Lance !', '', '', '', '¡La alianza ha cogido la avanzada Lanza de Guerra!', '', '', '', ''),
(68553, 2, 0, '', 'L''Alliance a gagné cette phase de la bataille !', '', '', '', '¡La alianza gana esta fase de la batalla!', '', '', '', ''),
(68553, 3, 0, '', 'La Horde a conquis le Bastion de Bouclier-des-Tempêtes !', '', '', '', '¡La horda ha cogido la fortaleza Escudo de Tormenta!', '', '', '', '');

UPDATE creature_template SET unit_flags = unit_flags & ~0x200000, EXP = 5, minlevel = 102, maxlevel = 102, dmg_multiplier = 3, unit_class = 1, ScriptName = 'npc_faction_boss' WHERE entry IN (82876, 82877);
UPDATE creature_template SET mechanic_immune_mask = 617299839 WHERE entry IN (82876, 82877);
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry = 89320;
DELETE FROM spell_script_names WHERE spell_id IN (178795, 176670, 178531, 178533);
INSERT INTO spell_script_names VALUE
(178795, 'spell_blade_twister'),
(176670, 'spell_emberfall_living_bomb'),
(178531, 'spell_ashran_faction_rewards'),
(178533, 'spell_ashran_faction_rewards');

DELETE FROM creature_text WHERE entry = 82877;
INSERT INTO creature_text VALUES
(82877, 0, 0, 'De Alliance be at de gates! To Arms soldiers of de Horde!', 14, 0, 100, 0, 0, 46573, 'VOLRATH_INTRO'),
(82877, 1, 0, 'You are foolish to challenge me!', 14, 0, 100, 0, 0, 46570, 'VOLRATH_AGGRO_01'),
(82877, 1, 1, 'Slay them all!', 14, 0, 100, 0, 0, 46571, 'VOLRATH_AGGRO_01'),
(82877, 2, 0, 'Another fool falls.', 14, 0, 100, 0, 0, 46567, 'VOLRATH_SLAY_01'),
(82877, 2, 1, 'You die better than ya fight.', 14, 0, 100, 0, 0, 46568, 'VOLRATH_SLAY_02'),
(82877, 2, 2, 'You made a mistake coming to Warspear.', 14, 0, 100, 0, 0, 46566, 'VOLRATH_SLAY_03'),
(82877, 3, 0, 'I have failed my Horde, my tribe, my chieftain...', 14, 0, 100, 0, 0, 46572, 'VOLRATH_DEATH'),
(82877, 4, 0, 'Backup has arrived! Dispose of dose dat remain.', 14, 0, 100, 0, 0, 46569, 'VOLRATH_VICTORY');

DELETE FROM locales_creature_text WHERE entry = 82877;
INSERT INTO locales_creature_text VALUES
(82877, 0, 0, '', 'L''Alliance, elle est à nos portes ! Soldats de la Horde, aux armes !', '', '', '', '¡La Alianza está en nuestras puertas! ¡Soldados de la Horda, a las armas!', '', '', '', ''),
(82877, 1, 0, '', 'C''est pas malin de me défier, ça non !', '', '', '', '¡Estás loco para desafiarme!', '', '', '', ''),
(82877, 1, 1, '', 'Tuez-les tous !', '', '', '', '¡Matadlos a todos!', '', '', '', ''),
(82877, 2, 0, '', 'Un idiot de moins sur la terre.', '', '', '', 'Otro loco falla.', '', '', '', ''),
(82877, 2, 1, '', 'Tu sais mieux mourir que te battre.', '', '', '', 'Sabes morir mejor que pelear.', '', '', '', ''),
(82877, 2, 2, '', 'Venir à Fer-de-Lance, c''était une grosse erreur.', '', '', '', 'Habéis cometido un error viniendo a Lanza de Guerra.', '', '', '', ''),
(82877, 3, 0, '', 'Ma Horde, ma tribu, mon chef... je leur ai tous fait défaut...', '', '', '', 'He fallado mi Horda, mi tribu, mi jefe...', '', '', '', ''),
(82877, 4, 0, '', 'Les renforts, ils sont là ! Occupez-vous de ceux qui restent.', '', '', '', '¡Los refuerzos están aquí! Ocupaos de los que quedan.', '', '', '', '');

DELETE FROM creature_text WHERE entry = 82876;
INSERT INTO creature_text VALUES
(82876, 0, 0, 'Stormshield is under attack! To the defenses. Push them back!', 14, 0, 100, 0, 0, 46523, 'TREMBLADE_INTRO'),
(82876, 1, 0, 'For the Alliance!', 14, 0, 100, 0, 0, 46520, 'TREMBLADE_AGGRO_01'),
(82876, 1, 1, 'Your advance stops here!', 14, 0, 100, 0, 0, 46521, 'TREMBLADE_AGGRO_01'),
(82876, 2, 0, 'You should not have come here!', 14, 0, 100, 0, 0, 46516, 'TREMBLADE_SLAY_01'),
(82876, 2, 1, 'You were not match for us.', 14, 0, 100, 0, 0, 46517, 'TREMBLADE_SLAY_02'),
(82876, 2, 2, 'One less problem for the Alliance.', 14, 0, 100, 0, 0, 46518, 'TREMBLADE_SLAY_03'),
(82876, 3, 0, 'I''m wounded... leave me... protect the village below!', 14, 0, 100, 0, 0, 46522, 'TREMBLADE_DEATH'),
(82876, 4, 0, 'Reinforcements have arrived! Clear this rabble from Stormshield!', 14, 0, 100, 0, 0, 46519, 'TREMBLADE_VICTORY');

DELETE FROM locales_creature_text WHERE entry = 82876;
INSERT INTO locales_creature_text VALUES
(82876, 0, 0, '', 'Bouclier-des-Tempêtes est attaqué ! Aux postes de défenses. Repoussez-les !', '', '', '', '¡Escudo de Tormenta está siendo atacado! A los puestos de defensa. ¡Repeledlos!', '', '', '', ''),
(82876, 1, 0, '', 'Pour l''Alliance !', '', '', '', '¡Por la Alianza!', '', '', '', ''),
(82876, 1, 1, '', 'Vous n''irez pas plus loin !', '', '', '', '¡Vuestro avance se detiene aquí!', '', '', '', ''),
(82876, 2, 0, '', 'Vous n''auriez jamais du venir ici !', '', '', '', 'Nunca deberíais haber venido aquí.', '', '', '', ''),
(82876, 2, 1, '', 'Vous ne faisiez pas le poids.', '', '', '', 'No érais rivales para nosotros.', '', '', '', ''),
(82876, 2, 2, '', 'Un problème de moins pour l''Alliance.', '', '', '', 'Un problema menos para la Alianza.', '', '', '', ''),
(82876, 3, 0, '', 'Je suis blessée... laissez-moi... protégez le village !', '', '', '', 'Estoy herido... dejadme... proteged el poblado más abajo.', '', '', '', ''),
(82876, 4, 0, '', 'Les renforts sont arrivés ! Jetons cette racaille hors de Bouclier-des-Tempêtes !', '', '', '', 'Los refuerzos han llegado! ¡Echemos esta chusma de Escudo de Tormenta!', '', '', '', '');

UPDATE creature_template SET ScriptName = 'npc_jeron_emberfall' WHERE entry = 88178;
UPDATE creature_template SET ScriptName = 'npc_rylai_crestfall' WHERE entry = 88224;

DELETE FROM creature_text WHERE entry IN (88178, 88224);
INSERT INTO creature_text VALUES
(88178, 0, 0, 'Tick... Tock... Tick... Tock', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk6'),
(88178, 1, 0, 'Ashes to ashes.', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk1'),
(88178, 1, 1, 'The heat is on!', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk5'),
(88178, 1, 2, 'Too hot for you?', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk7'),
(88178, 2, 0, 'I will rise again... and again.', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk3'),
(88178, 2, 1, 'I''m burned out.', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk4'),
(88178, 3, 0, 'Feel the burn!', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk2'),
(88178, 4, 0, 'Who set up the bomb? Me.', 12, 0, 100, 0, 0, 0, 'JeronEmberfallTalk8'),

(88224, 0, 0, 'Chill out.', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk2'),
(88224, 0, 1, 'The battle begins!', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk9'),
(88224, 1, 0, 'Dress warmer next time.', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk3'),
(88224, 1, 1, 'Say Bahh to your healers!', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk7'),
(88224, 1, 2, 'Shattered!', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk8'),
(88224, 1, 3, 'Thought I was a pushover like Balinda? Ha!', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk10'),
(88224, 2, 0, 'Ahh... a nice cool breeze.', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk1'),
(88224, 3, 0, 'I could use a heal here!', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk4'),
(88224, 3, 1, 'I''m burning up!', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk5'),
(88224, 3, 2, 'It''s getting warm up here...', 12, 0, 100, 0, 0, 0, 'RylaiCrestfallTalk6');

DELETE FROM locales_creature_text WHERE entry IN (88178, 88224);
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(88178, 0, 0, 'Tic... Tac... Tic... Tac', 'Tic... Tac... Tic... Tac...'),
(88178, 1, 0, 'Des cendres aux cendres.', 'Cenizas a las cenizas.'),
(88178, 1, 1, 'La chaleur est là !', '¡El calor ha llegado!'),
(88178, 1, 2, 'Trop chaud pour vous ?', '¿Demasiado calor para vosotros?'),
(88178, 2, 0, 'Je me lèverai encore... et encore.', 'Me levantaré una y otra vez.'),
(88178, 2, 1, 'Je suis brûlé.', 'Estoy quemado.'),
(88178, 3, 0, 'Sentez la brûlure !', '¡Sentid las quemaduras!'),
(88178, 4, 0, 'Qui a mis en place la bombe ? Moi.', '¿Que quién ha puesto la bomba? Yo.'),

(88224, 0, 0, 'On se calme.', 'Calmaos.'),
(88224, 0, 1, 'La bataille commence !', '¡La batalla comienza!'),
(88224, 1, 0, 'Habillez-vous plus chaud la prochaine fois.', 'Abrigaos más la próxima vez.'),
(88224, 1, 1, 'Dites "Bahh !" à vos guérisseurs !', '¡Decid bahhh a vuestros healers!'),
(88224, 1, 2, 'Brisé !', '¡Roto!'),
(88224, 1, 3, 'Vous pensiez que j''étais un jeu d''enfant comme Balinda ? Ha !', '¿Pensabais que era un juego de niños como Balinda? ¡Já!'),
(88224, 2, 0, 'Ahh... une belle brise fraîche.', 'Ahh... Una brisa de aire fresco.'),
(88224, 3, 0, 'Je pourrais utiliser un soin ici !', '¡Podría utilizar una cura aquí!'),
(88224, 3, 1, 'Je brûle !', '¡Me quemo!'),
(88224, 3, 2, 'Il commence à faire chaud ici...', 'Empieza a hacer calor aquí...');

DELETE FROM creature_onkill_reputation WHERE creature_id IN (82876, 82877);
INSERT INTO creature_onkill_reputation VALUES
(82876, 1681, 0, 7, 0, 2500, 0, 0, 0, 0),
(82877, 1682, 0, 7, 0, 2500, 0, 0, 0, 0);

DELETE FROM spell_loot_template WHERE entry IN (178531, 178533);
INSERT INTO spell_loot_template VALUES
(178531, 120151, 100, 1, 0, 1, 1, ""),
(178533, 118065, 100, 1, 0, 1, 1, "");

REPLACE INTO creature_model_info VALUE (19609, 0.3, 1.5, 2, 0);
REPLACE INTO creature_model_info VALUE (13337, 0.208, 1.5, 1, 13336);
REPLACE INTO creature_model_info VALUE (13338, 0.9747, 4.05, 0, 13339);
REPLACE INTO creature_model_info VALUE (13339, 0.8725, 3.75, 1, 13338);
REPLACE INTO creature_model_info VALUE (169, 0.903, 2, 2, 0);
REPLACE INTO creature_model_info VALUE (59914, 0.306, 1.5, 0, 0);
REPLACE INTO creature_model_info VALUE (59918, 0.306, 1.5, 0, 0);
REPLACE INTO creature_model_info VALUE (59915, 0.306, 1.5, 0, 0);
REPLACE INTO creature_model_info VALUE (60784, 0.306, 1.5, 0, 0);
UPDATE creature_template SET IconName = 'taxi', gossip_menu_id = 87617, EXP = 5, npcflag = 9193, ScriptName = 'npc_ashran_flight_masters' WHERE entry = 87617;
UPDATE creature_template SET IconName = 'taxi', gossip_menu_id = 87672, EXP = 5, npcflag = 9193, ScriptName = 'npc_ashran_flight_masters' WHERE entry = 87672;
UPDATE creature_template SET unit_flags = 0x280 WHERE entry IN (87687, 87689);
UPDATE creature_template SET
minlevel = 100,
maxlevel = 100,
EXP = 5,
npcflag = 32768,
npcflag2 = 0,
unit_flags = 832,
unit_flags2 = 2048,
type_flags = 2,
flags_extra = 2,
ScriptName = 'npc_ashran_spirit_healer'
WHERE entry IN (80723, 80724);
UPDATE creature_template SET faction = 84 WHERE entry = 80723;
UPDATE creature_template SET faction = 83, unit_class = 2 WHERE entry = 80724;

DELETE FROM creature_template_addon WHERE entry IN (80723, 80724);
INSERT INTO creature_template_addon VALUES
(80723, 0, 0, 65536, 1, 0, '22011'),
(80724, 0, 0, 65536, 1, 0, '22011');

DELETE FROM gossip_menu_option WHERE menu_id IN (87617, 87672);
INSERT INTO gossip_menu_option (menu_id, id, option_icon, option_text, option_id, npc_option_npcflag) VALUES
(87617, 0, 0, 'Fly me to Stormshield stronghold!', 1, 1),
(87672, 0, 0, 'Fly me to Warspear outpost!', 1, 1);

DELETE FROM locales_gossip_menu_option WHERE menu_id IN (87617, 87672);
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc2, option_text_loc6) VALUES
(87617, 0, 'Ramenez-moi à Bouclier-des-Tempêtes !', '¡Llévame volando a la fortaleza Escudo de Tormenta!'),
(87672, 0, 'Ramenez-moi à Fer-de-Lance !', '¡Llévame volando a la avanzada Lanza de Guerra!');