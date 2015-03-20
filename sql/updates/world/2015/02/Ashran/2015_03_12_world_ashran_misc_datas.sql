DELETE FROM creature WHERE id IN (83435, 84471, 84466, 84650, 84645, 84646, 84651, 84652, 84470, 81883, 82200, 84906) AND map = 1191;
DELETE FROM gameobject WHERE id IN (233285, 234082, 234083, 234067, 234081) AND map = 1191;

UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, InhabitType = 4 WHERE entry IN (84683, 84471);
UPDATE creature_template SET unit_class = 8, dmg_multiplier = 5, ScriptName = 'npc_ashran_kauper' WHERE entry = 84466;
UPDATE creature_template SET unit_class = 8, minlevel = 101, maxlevel = 101, dmg_multiplier = 5, faction = 1604, ScriptName = 'npc_ashran_zaram_sunraiser' WHERE entry = 84468;
UPDATE creature_template SET minlevel = 100, maxlevel = 100, faction = 2110 WHERE entry = 83948;
UPDATE creature_template SET minlevel = 101, maxlevel = 101, unit_class = 8, dmg_multiplier = 5, faction = 118, ScriptName = 'npc_ashran_horde_gateway_guardian' WHERE entry IN (84645, 84646);
UPDATE creature_template SET minlevel = 101, maxlevel = 101, unit_class = 8, dmg_multiplier = 5, faction = 2163, ScriptName = 'npc_ashran_alliance_gateway_guardian' WHERE entry IN (84651, 84652);
UPDATE creature_template SET unit_flags = unit_flags | (0x02|0x2000000) WHERE entry IN (84631, 84636);
UPDATE creature_template SET dmg_multiplier = 10, ScriptName = 'npc_ashran_fangraal', mechanic_immune_mask = 617299839 WHERE entry = 81859;
UPDATE creature_template SET dmg_multiplier = 10, ScriptName = 'npc_ashran_kronus', mechanic_immune_mask = 617299839 WHERE entry = 82201;
UPDATE creature_template SET ScriptName = 'npc_ashran_underpowered_earth_fury' WHERE entry = 82200;
UPDATE creature_template SET ScriptName = 'npc_ashran_lifeless_ancient' WHERE entry = 81883;
UPDATE creature_template SET ScriptName = 'npc_ashran_stormshield_stormcrow' WHERE entry = 82895;
UPDATE creature_template SET dmg_multiplier = 15, ScriptName = 'npc_ashran_warspear_gladiator' WHERE entry = 85811;
UPDATE creature_template SET dmg_multiplier = 15, ScriptName = 'npc_ashran_stormshield_gladiator' WHERE entry = 85812;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_ashran_mandragoraster' WHERE entry = 83683;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_ashran_panthora' WHERE entry = 83691;
UPDATE creature_template SET dmg_multiplier = 500, ScriptName = 'npc_ashran_ancient_inferno' WHERE entry = 84875;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = 'npc_ashran_volcano' WHERE entry = 88227;
UPDATE creature_template SET dmg_multiplier = 500, ScriptName = 'npc_ashran_goregore' WHERE entry = 84875;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_ashran_ashmaul_magma_caster' WHERE entry = 84906;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = 'npc_ashran_volcanic_ground' WHERE entry = 84952;
UPDATE creature_template SET dmg_multiplier = 300, ScriptName = 'npc_ashran_elder_darkweaver_kath' WHERE entry = 85771;
UPDATE creature_template SET ScriptName = 'npc_ashran_shadow_figurine' WHERE entry = 78620;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_ashran_ashmaul_destroyer' WHERE entry = 84876;
UPDATE creature_template SET ScriptName = 'npc_ashran_excavator_rustshiv' WHERE entry = 88568;
UPDATE creature_template SET ScriptName = 'npc_ashran_excavator_hardtooth' WHERE entry = 88567;

DELETE FROM gameobject_template WHERE entry IN (234082, 234083);
INSERT INTO gameobject_template VALUES
(234082, 22, 12984, 'Horde''s gate', '', '', '', 83, 32, 4, 0, 0, 0, 0, 0, 0, 166511, 4294967295, 0, 1, 1, 27752, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 19342),
(234083, 22, 12984, 'Horde''s gate', '', '', '', 83, 32, 4, 0, 0, 0, 0, 0, 0, 166512, 4294967295, 0, 1, 1, 27752, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 19342);

DELETE FROM creature_template_addon WHERE entry IN (84466, 84468);
INSERT INTO creature_template_addon VALUES
(84466, 0, 0, 0, 1, 333, ''),
(84468, 0, 0, 0, 1, 333, '');

DELETE FROM creature_equip_template WHERE entry IN (84468, 84646, 84645);
INSERT INTO creature_equip_template VALUE
(84468, 1, 32055, 0, 0),
(84646, 1, 30910, 48032, 0),
(84645, 1, 30910, 48032, 0);

DELETE FROM areatrigger_template WHERE spell_id IN (161519, 176144);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUES
(161519, 0, 2317, 1, 1, 4096, 'areatrigger_ashran_splitting_breath'),
(161519, 1, 2318, 1, 1, 4096, 'areatrigger_ashran_splitting_breath'),
(161519, 2, 2319, 1, 1, 4096, 'areatrigger_ashran_splitting_breath'),
(176144, 0, 3440, 1, 1, 4096, 'areatrigger_ashran_volcano');

DELETE FROM spell_script_names WHERE spell_id IN (161520, 162908, 170896, 168232, 170407, 176542, 158830, 176187);
INSERT INTO spell_script_names VALUES
(161520, 'spell_ashran_splitting_breath'),
(162908, 'spell_ashran_vile_blood'),
(170896, 'spell_ashran_stone_empowerment'),
(168232, 'spell_ashran_pocket_flying_machine'),
(170407, 'spell_ashran_pocket_flying_machine'),
(176542, 'spell_ashran_shadow_claws'),
(158830, 'spell_ashran_darkness_within'),
(176187, 'spell_ashran_earth_smash');

DELETE FROM spell_proc_event WHERE entry IN (162908, 170896);
INSERT INTO spell_proc_event (entry, procFlags, CustomChance) VALUES
(162908, 0x14, 33),
(170896, 0x01000000, 100);

UPDATE gameobject_template SET data2 = 0 WHERE entry = 237624;

DELETE FROM creature_model_info WHERE modelid IN (58973, 61275);
INSERT INTO creature_model_info VALUES
(58973, 4, 10, 0, 0),
(61275, 7.602, 9, 0, 0);

DELETE FROM spell_target_position WHERE id IN (176242, 166511, 166512, 169258, 169259);
INSERT INTO spell_target_position VALUE
(176242, 0, 1191, 5043.58, -4077.59, 41.92, 4.13),
(166511, 1, 1191, 4027.89, -4495.86, 85.70, 1.92),
(166512, 1, 1191, 4770.96, -3712.67, 1.34, 0.5723),
(169258, 1, 1191, 4944.44, -3746.54, 2.06, 5.7583),
(169259, 1, 1191, 4169.51, -4540.30, 78.28, 0.340574);

DELETE FROM gossip_menu WHERE entry IN (17304, 65591, 65592, 65593, 65594, 65595);
DELETE FROM gossip_menu WHERE text_id IN (90814, 84919, 85463, 84923, 89853, 83895);
INSERT INTO gossip_menu (entry, text_id) VALUES
(17304, 90814),
(65591, 84919), -- Mages
(65592, 85463), -- Warlocks
(65593, 84923), -- Warrior/Paladin
(65594, 89853), -- Kronus
(65595, 83895); -- Fangraal

DELETE FROM npc_text WHERE ID IN (84919, 85463, 84923, 89853, 83895, 90814);
INSERT INTO npc_text (ID, text0_0, text0_1) VALUES
(
    84919,
    'We''ve spawned the Portals and they''re up and running. Good work!',
    'We''ve spawned the Portals and they''re up and running. Good work!'
),
(
    85463,
    'The warlocks have finished activating the Gateways and they are now fully functional. Good work, $n.',
    'The warlocks have finished activating the Gateways and they are now fully functional. Good work, $n.'
),
(
    84923,
    'The riders are out and on duty now. Good job procuring those artifacts, soldier.',
    'The riders are out and on duty now. Good job procuring those artifacts, soldier.'
),
(
    89853,
    'Kronus is born! Good work, $n.',
    'Kronus is born! Good work, $n.'
),
(
    83895,
    'Fangraal is born! He is pushing forward on the Road of Glory at this moment.',
    'Fangraal is born! He is pushing forward on the Road of Glory at this moment.'
),
(
    90814,
    'The sign reads:$B$B"I''m Commander $n, and this is my favorite merchant in Warspear."',
    'The sign reads:$B$B"I''m Commander $n, and this is my favorite merchant in Warspear."'
);

DELETE FROM locales_npc_text WHERE entry IN (84919, 85463, 84923, 89853, 83895, 90814);
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUES
(
    84919,
    'Nous avons invoqué les portails, et ils sont opérationnels. Du boulot bien fait !',
    'Nous avons invoqué les portails, et ils sont opérationnels. Du boulot bien fait !',
    '', '',
    'Hemos generado los portales, los cuales ya están en funcionamiento. ¡Buen trabajo!',
    'Hemos generado los portales, los cuales ya están en funcionamiento. ¡Buen trabajo!',
    '', ''
),
(
    85463,
    'Les démonistes sont parvenus à activer les portes, elles sont désormais opérationnelles. C''est du bon travail, $n.',
    'Les démonistes sont parvenus à activer les portes, elles sont désormais opérationnelles. C''est du bon travail, $n.',
    '', '',
    'Los brujos terminaron de activar los portales y ya operan en su totalidad. Buen trabajo, $n.',
    'Los brujos terminaron de activar los portales y ya operan en su totalidad. Buen trabajo, $n.',
    '', ''
),
(
    84923,
    'Les chevaucheurs sont en patrouille. Merci de nous avoir procuré ces artéfacts, soldat.',
    'Les chevaucheurs sont en patrouille. Merci de nous avoir procuré ces artéfacts, soldat.',
    '', '',
    'Los jinetes están de servicio. Buen trabajo consiguiendo esos artefactos, soldado.',
    'Los jinetes están de servicio. Buen trabajo consiguiendo esos artefactos, soldado.',
    '', ''
),
(
    89853,
    'Kronus est désormais des nôtres ! Bon travail, $n.',
    'Kronus est désormais des nôtres ! Bon travail, $n.',
    '', '',
    '¡Kronus ha nacido! Buen trabajo, $n.',
    '¡Kronus ha nacido! Buen trabajo, $n.',
    '', ''
),
(
    83895,
    'Crograal va arpenter la route de la Gloire pour éliminer les forces de la Horde.',
    'Crograal va arpenter la route de la Gloire pour éliminer les forces de la Horde.',
    '', '',
    '¡Nació Fangraal! En este momento avanza por el Camino de Gloria.',
    '¡Nació Fangraal! En este momento avanza por el Camino de Gloria.',
    '', ''
),
(
    90814,
    'Sur la pancarte, on peut lire :$B$B« Je suis le commandant $n, et ce marchand est mon préféré de Fer-de-Lance. »',
    'Sur la pancarte, on peut lire :$B$B« Je suis le commandant $n, et ce marchand est mon préféré de Fer-de-Lance. »',
    '', '',
    'El letrero dice:$B$B"Soy $Gel:la; comandante $n y este es mi mercader favorito en Lanza de guerra".',
    'El letrero dice:$B$B"Soy $Gel:la; comandante $n y este es mi mercader favorito en Lanza de guerra".',
    '', ''
);

DELETE FROM creature_text WHERE entry IN (83995, 82966, 83997, 82660, 82204, 81870, 83830, 82893, 81859, 83895, 80256, 88568, 88567);
INSERT INTO creature_text VALUES
(83995, 0, 0, 'We have activated the |c00FFFF00|Hspell:178539|h[Mage Portal]|h|r!', 12, 0, 100, 0, 0, 0, 'HordeMagePortalActivated'),
(83995, 1, 0, 'The |c00FFFF00|Hspell:178539|h[Mage Portal]|h|r has been destroyed by the Alliance!', 12, 0, 100, 0, 0, 0, 'HordeMagePortalDeactivated'),

(82966, 0, 0, 'We have activated the |c00FFFF00|Hspell:178538|h[Mage Portal]|h|r!', 12, 0, 100, 0, 0, 0, 'AllianceMagePortalActivated'),
(82966, 1, 0, 'The |c00FFFF00|Hspell:178538|h[Mage Portal]|h|r has been destroyed by the Horde!', 12, 0, 100, 0, 0, 0, 'AllianceMagePortalDeactivated'),

(83997, 0, 0, 'The |c00FFFF00|Hspell:178534|h[Warlock Gateways]|h|r has been activated! Consult your map to the locations.', 12, 0, 100, 0, 0, 0, 'HordeWarlockGatewaysActivated'),
(83997, 1, 0, 'The |c00FFFF00|Hspell:178534|h[Warlock Gateways]|h|r has been destroyed by the Alliance!', 12, 0, 100, 0, 0, 0, 'HordeWarlockGatewaysDeactivated'),

(82660, 0, 0, 'The |c00FFFF00|Hspell:178535|h[Warlock Gateways]|h|r has been activated! Consult your map to the locations.', 12, 0, 100, 0, 0, 0, 'AllianceWarlockGatewaysActivated'),
(82660, 1, 0, 'The |c00FFFF00|Hspell:178535|h[Warlock Gateways]|h|r has been destroyed by the Horde!', 12, 0, 100, 0, 0, 0, 'AllianceWarlockGatewaysDeactivated'),

(82204, 0, 0, '|c00FFFF00|Hspell:178359|h[Kronus]|h|r has been energized! He''s out to patrol Ashran!!', 12, 0, 100, 0, 0, 0, 'HordeKronusActivated'),
(82204, 1, 0, 'You fools! The Alliance has killed |c00FFFF00|Hspell:178359|h[Kronus]|h|r!', 12, 0, 100, 0, 0, 0, 'HordeKronusDeactivated'),

(81870, 0, 0, 'We''ve done it! |c00FFFF00|Hspell:178358|h[Fangraal]|h|r marches into Ashran to cleanse the Horde filth. Don''t let him die, Alliance!', 12, 0, 100, 0, 0, 0, 'AllianceFangraalActivated'),
(81870, 1, 0, '|c00FFFF00|Hspell:178358|h[Fangraal]|h|r has been slain by the Horde! Poor lil'' guy.', 12, 0, 100, 0, 0, 0, 'AllianceFangraalDeactivated'),

(83830, 0, 0, 'The |c00FFFF00|Hspell:178536|h[Wolf Riders]|h|r are out for duty!', 12, 0, 100, 0, 0, 0, 'HordeWolfRidersActivated'),
(83830, 1, 0, 'The |c00FFFF00|Hspell:178536|h[Wolf Riders]|h|r have been slain by the Alliance!', 12, 0, 100, 0, 0, 0, 'HordeWolfRidersDeactivated'),

(82893, 0, 0, 'The |c00FFFF00|Hspell:178537|h[Knight Riders]|h|r are out for duty!', 12, 0, 100, 0, 0, 0, 'AllianceKnightRidersActivated'),
(82893, 1, 0, 'The |c00FFFF00|Hspell:178537|h[Knight Riders]|h|r have been slain by the Horde!', 12, 0, 100, 0, 0, 0, 'AllianceKnightRidersDeactivated'),

(81859, 0, 0, 'Thank you Druids... you''ve got the nature''s touch.', 12, 0, 100, 0, 0, 0, 'FangraalAwake1'),
(81859, 1, 0, 'And now, let''s clear those Horde from this land.', 12, 0, 100, 0, 0, 0, 'FangraalAwake2'),

(83895, 0, 0, 'For the Horde!', 12, 0, 100, 0, 0, 0, 'WolfRiderAggro'),
(83895, 1, 0, 'Lok''tar ogar! We must capture this last point and our Headhunters can take out those |c00FFFF00|Hspell:178363|h[Stormshield Gladiators]|h|r, then we can storm Stormshield Stronghold. Tremblade will die this day!', 12, 0, 100, 0, 0, 0, 'WolfRiderLastPoint'),
(83895, 2, 0, 'Retreat!', 12, 0, 100, 0, 0, 0, 'WolfRiderEvade0'),
(83895, 2, 1, 'Run!', 12, 0, 100, 0, 0, 0, 'WolfRiderEvade1'),

(80256, 0, 0, 'For the Alliance!', 12, 0, 100, 0, 0, 0, 'StormshieldKnightAggro'),
(80256, 1, 0, 'Good work Alliance! If we now capture Emberfall Tower we can have our Sentinels take out those |c00FFFF00|Hspell:178362|h[Warspear Gladiators]|h|r. Victory is near!', 12, 0, 100, 0, 0, 0, 'StormshieldKnightLastPoint'),
(80256, 2, 0, 'We are shamed in this defeat. Fall back!', 12, 0, 100, 0, 0, 0, 'StormshieldKnightEvade0'),
(80256, 2, 1, 'Discretion is the better part of valor. Now flee!', 12, 0, 100, 0, 0, 0, 'StormshieldKnightEvade1'),

(88568, 0, 0, 'Is beer the only thing these orcs drink? What does a goblin gotta do to get a nice cocktail around here?', 12, 0, 100, 0, 0, 0, 'ExcavatorRustshiv1'),
(88568, 1, 0, 'Nostalgic? How does that swill make you nostalgic?', 12, 0, 100, 0, 0, 0, 'ExcavatorRustshiv2'),
(88568, 2, 0, 'Wow, that''s... surprisingly accurate.', 12, 0, 100, 0, 0, 0, 'ExcavatorRustshiv3'),
(88568, 3, 0, 'Man, I though it would be a pretty sweet gig coming out here with the Society. Boy was I wrong!', 12, 0, 100, 0, 0, 0, 'ExcavatorRustshiv4'),
(88568, 4, 0, 'It ain''t the digs I''m concerned about. Everywhere you turn either an ogre is trying to club your head in, or one of those Alliance goons wants to stick a sword in ya!', 12, 0, 100, 0, 0, 0, 'ExcavatorRustshiv5'),
(88568, 5, 0, 'Oh I ain''t afraid, baby. It''s just that death puts a steep cut in the profit margins.', 12, 0, 100, 0, 0, 0, 'ExcavatorRustshiv6'),

(88567, 0, 0, 'It ain''t so bad. It kinda makes me nostalgic.', 12, 0, 100, 0, 0, 0, 'ExcavatorHardtooth1'),
(88567, 1, 0, 'Well, the taste kinda reminds me of the runoff water from cooling the machines back home. It''s all we had to drink when I was a kid.', 12, 0, 100, 0, 0, 0, 'ExcavatorHardtooth2'),
(88567, 2, 0, 'What''s wrong with this place? The digs ain''t bad.', 12, 0, 100, 0, 0, 0, 'ExcavatorHardtooth3'),
(88567, 3, 0, 'I thought you were tellin'' me that you laugh in the face of danger? Sounds like you might be afraid of this place.', 12, 0, 100, 0, 0, 0, 'ExcavatorHardtooth4');

DELETE FROM locales_creature_text WHERE entry IN (83995, 82966, 83997, 82660, 82204, 81870, 83830, 82893, 81859, 83895, 80256, 88568, 88567);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    83995, 0, 0,
    'Nous avons activé le |c00FFFF00|Hspell:178539|h[Portail de mage]|h|r !',
    '',
    '¡Activamos el |c00FFFF00|Hspell:178539|h[portal de magos]|h|r!',
    ''
),
(
    83995, 1, 0,
    'Le |c00FFFF00|Hspell:178539|h[Portail de mage]|h|r a été détruit par l''Alliance !',
    '',
    '¡La Alianza destruyó el |c00FFFF00|Hspell:178539|h[portal de los magos]|h|r!',
    ''
),
(
    82966, 0, 0,
    'Nous avons activé le |c00FFFF00|Hspell:178538|h[Portail de mage]|h|r !',
    '',
    '¡Activamos el |c00FFFF00|Hspell:178538|h[portal de magos]|h|r!',
    ''
),
(
    82966, 1, 0,
    'Le |c00FFFF00|Hspell:178538|h[Portail de mage]|h|r a été détruit par la Horde !',
    '',
    '¡La Horda destruyó el |c00FFFF00|Hspell:178538|h[portal de los magos]|h|r!',
    ''
),
(
    83997, 0, 0,
    'La |c00FFFF00|Hspell:178534|h[Porte de démoniste]|h|r a été activée ! Consultez votre carte pour en connaître les emplacements.',
    '',
    '¡Activaron el |c00FFFF00|Hspell:178534|h[portal del brujo]|h|r! Revisa las ubicaciones en tu mapa.',
    ''
),
(
    83997, 1, 0,
    'La |c00FFFF00|Hspell:178534|h[Porte de démoniste]|h|r a été détruite par l''Alliance !',
    '',
    '¡La Alianza destruyó el |c00FFFF00|Hspell:178534|h[portal del brujo]|h|r!',
    ''
),
(
    82660, 0, 0,
    'La |c00FFFF00|Hspell:178535|h[Porte de démoniste]|h|r a été activée ! Consultez votre carte pour en connaître les emplacements.',
    '',
    '¡Activaron el |c00FFFF00|Hspell:178535|h[portal del brujo]|h|r! Revisa las ubicaciones en tu mapa.',
    ''
),
(
    82660, 1, 0,
    'La |c00FFFF00|Hspell:178535|h[Porte de démoniste]|h|r a été détruite par la Horde !',
    '',
    '¡La Horda destruyó el |c00FFFF00|Hspell:178535|h[portal del brujo]|h|r!',
    ''
),
(
    82204, 0, 0,
    '|c00FFFF00|Hspell:178359|h[Kronus]|h|r fait son entrée en scène !',
    '',
    '¡|c00FFFF00|Hspell:178359|h[Kronus]|h|rse alzó!',
    ''
),
(
    82204, 1, 0,
    'Imbéciles ! L''Alliance a tué |c00FFFF00|Hspell:178359|h[Kronus]|h|r !',
    '',
    '¡Tontos! ¡La Alianza mató a |c00FFFF00|Hspell:178359|h[Kronus]|h|r!',
    ''
),
(
    81870, 0, 0,
    'Nous avons réussi ! |c00FFFF00|Hspell:178358|h[Crograal]|h|r parcourt A''shran pour liquider la pourriture de la Horde ! Ne le laissez pas mourir !',
    '',
    '¡Lo logramos! |c00FFFF00|Hspell:178358|h[Fangraal]|h|r marcha sobre Ashran para deshacerse de la escoria de la Horda. ¡No debe morir, Alianza!',
    ''
),
(
    81870, 1, 0,
    '|c00FFFF00|Hspell:178358|h[Crograal]|h|r a été tué par la Horde !',
    '',
    '|c00FFFF00|Hspell:178358|h¡[Fangraal]|h|r fue asesinado por la Horda!',
    ''
),
(
    83830, 0, 0,
    'Les |c00FFFF00|Hspell:178536|h[Chevaucheurs de loups]|h|r sont de sortie !',
    '',
    '¡Los |c00FFFF00|Hspell:178536|h[jinetes de lobo]|h|r salieron a cumplir con su deber!',
    ''
),
(
    83830, 1, 0,
    'Les |c00FFFF00|Hspell:178536|h[Chevaucheurs de loups]|h|r ont été tués par l''Alliance !',
    '',
    '¡La Alianza asesinó a los |c00FFFF00|Hspell:178536|h[jinetes de lobo]|h|r!',
    ''
),
(
    82893, 0, 0,
    'Les |c00FFFF00|Hspell:178537|h[Chevaucheurs]|h|r sont de sortie !',
    '',
    '¡Los |c00FFFF00|Hspell:178537|h[jinetes caballeros]|h|r salieron a cumplir con su deber!',
    ''
),
(
    82893, 1, 0,
    'Les |c00FFFF00|Hspell:178537|h[Chevaucheurs]|h|r ont été tués par la Horde !',
    '',
    '¡La Horda asesinó a los |c00FFFF00|Hspell:178537|h[jinetes caballeros]|h|r!',
    ''
),
(
    81859, 0, 0,
    'Merci, druides... vous avez la main verte.',
    '',
    'Gracias, druidas... tienen el toque de la naturaleza.',
    ''
),
(
    81859, 1, 0,
    'Et maintenant, allons débarrasser cette terre de la Horde.',
    '',
    'Y ahora, saquemos a los de la Horda de esta tierra.',
    ''
),
(
    83895, 0, 0,
    'Pour la Horde !',
    '',
    '¡Por la Horda!',
    ''
),
(
    83895, 1, 0,
    'Lok''tar ogar ! Capturons ce dernier point et nos chasseurs de tête pourront neutraliser leurs |c00FFFF00|Hspell:178363|h[Gladiateurs de Bouclier-des-Tempêtes]|h|r, ensuite nous attaquerons le bastion de Bouclier-des-Tempêtes. Tremblelame doit mourir aujourdhui !',
    '',
    '¡Lok''tar ogar! Debemos capturar este último punto y nuestros rebanacabezas podrán eliminar a esos |c00FFFF00|Hspell:178363|h[gladiadores Escudo de Tormenta]|h|r, y entonces podremos atacar el fuerte Escudo de Tormenta. ¡Hojatemblorosa morirá hoy!',
    ''
),
(
    83895, 2, 0,
    'Battez en retraite !',
    '',
    '¡Retirada!',
    ''
),
(
    83895, 2, 1,
    'Courez !',
    '',
    '¡Corre!',
    ''
),
(
    80256, 0, 0,
    'Pour l''Alliance !',
    '',
    '¡Por la Alianza!',
    ''
),
(
    80256, 1, 0,
    'Bravo, l''Alliance ! Si nous capturons la tour de Pluie-de-Braise sans tarder, nos sentinelles pourront neutraliser ces |c00FFFF00|Hspell:178362|h[Gladiateurs de Fer-de-Lance]|h|r. La victoire est proche !',
    '',
    '¡Buen trabajo, Alianza! Si capturamos la Torre de Cascada Ígnea, podremos dejar que nuestros centinelas eliminen a esos |c00FFFF00|Hspell:178362|h[gladiadores Lanza de guerra]|h|r. ¡La victoria se acerca!',
    ''
),
(
    80256, 2, 0,
    'Cette défaite nous couvre de honte. Replions-nous !',
    '',
    'Esta derrota es humillante. ¡Retirada!',
    ''
),
(
    80256, 2, 1,
    'La discrétion est la base du courage. Maintenant, fuyez !',
    '',
    'La discreción es la mejor parte del valor. ¡Ahora huyan!',
    ''
),
(
    88568, 0, 0,
    'Ces orcs ne boivent donc rien d''autre que de la bière ? Je suis un gobelin, il me faut un cocktail, c''est quand même pas trop demander !',
    '',
    '¿Lo único que beben esos orcos es cerveza? ¿Qué tiene hacer un goblin para que le sirvan un buen cóctel?',
    ''
),
(
    88568, 1, 0,
    'Nostalgique ? Comment est-ce que cette urine de troll peut te rendre nostalgique ?',
    '',
    '¿Nostalgia? ¿Por qué te inspira nostalgia esa bazofia?',
    ''
),
(
    88568, 2, 0,
    'Wouah, étonnamment, c''est... exactement ça.',
    '',
    'Guau, es... sorprendentemente exacto.',
    ''
),
(
    88568, 3, 0,
    'Mince, je pensais que ce serait sympa de venir ici avec la société. Monumentale erreur !',
    '',
    'Vaya, pensé que venir aquí con la Sociedad sería muy bonito. ¡Vaya equivocación!',
    ''
),
(
    88568, 4, 0,
    'C''est pas les piaules, le problème. Où que tu regardes, y a soit un ogre prêt à te briser le crâne, soit un de ces benêts de l''Alliance qui te pointe une épée sous le menton !',
    '',
    'No son las excavaciones lo que me preocupa. ¡Mires donde mires, o un ogro intenta aplastarte la cabeza o un matón de la Alianza quiere presentarte a su espada!',
    ''
),
(
    88568, 5, 0,
    'Moi, avoir les foies ? Pff ! C''est juste que la mort et le profit, c''est pas franchement compatible.',
    '',
    'Oh, yo no tengo miedo. Es solo que la muerte recorta muchísimo el margen de ganancia.',
    ''
),
(
    88567, 0, 0,
    'C''est pas si mal. Ça me rend nostalgique, un peu.',
    '',
    'No está tan mal. Me hace sentir nostalgia.',
    ''
),
(
    88567, 1, 0,
    'Ben, le goût me rappelle un peu celui de l''eau qui s''écoulait du système de refroidissement des machines qu''on avait à la maison. On n''avait rien d''autre à boire quand j''étais gamin.',
    '',
    'Bueno, el sabor es como el del agua que enfría las máquinas en casa. Cuando era niña no teníamos otra cosa para beber.',
    ''
),
(
    88567, 2, 0,
    'Qu''est-ce qui te plaît pas ? Les piaules sont pas mal.',
    '',
    '¿Qué le pasa a este sitio? Las excavaciones no van mal.',
    ''
),
(
    88567, 3, 0,
    'Je croyais que le danger t''impressionnait pas ? Pourtant on dirait bien que t''as les foies, là.',
    '',
    '¿No decías que hay que reírse ante el peligro? Parecería que le tienes miedo a este sitio.',
    ''
);

DELETE FROM waypoint_data WHERE id IN (82201, 81859);
INSERT INTO waypoint_data VALUES
(82201, 0, 5043.57, -4175.56, 45.45, 3.0774, 0, 0, 0, 100, 0),
(82201, 1, 5002.72, -4171.59, 44.70, 3.0185, 0, 0, 0, 100, 0),
(82201, 2, 4957.99, -4164.31, 38.50, 3.0617, 0, 0, 0, 100, 0),
(82201, 3, 4859.88, -4216.94, 34.58, 3.6389, 0, 0, 0, 100, 0),
(82201, 4, 4829.05, -4224.28, 32.75, 3.9115, 0, 0, 0, 100, 0),
(82201, 5, 4758.01, -4282.60, 21.56, 3.8290, 0, 0, 0, 100, 0),
(82201, 6, 4730.77, -4278.52, 19.36, 2.9219, 0, 0, 0, 100, 0),
(82201, 7, 4698.32, -4232.56, 10.09, 2.2582, 0, 0, 0, 100, 0),
(82201, 8, 4637.40, -4221.69, 7.510, 2.9650, 0, 0, 0, 100, 0),
(82201, 9, 4529.20, -4216.23, 7.105, 3.0946, 0, 0, 0, 100, 0),
(82201, 10, 4442.18, -4190.00, 6.630, 3.1268, 0, 0, 0, 100, 0),
(82201, 11, 4360.95, -4192.26, 10.22, 3.7630, 0, 0, 0, 100, 0),
(82201, 12, 4290.00, -4248.10, 19.96, 2.8009, 0, 0, 0, 100, 0),
(82201, 13, 4247.25, -4231.60, 28.61, 2.3218, 0, 0, 0, 100, 0),
(82201, 14, 4223.01, -4205.63, 31.18, 2.5974, 0, 0, 0, 100, 0),
(82201, 15, 4057.54, -4105.50, 48.43, 3.0333, 0, 0, 0, 100, 0),
(82201, 16, 3999.58, -4099.20, 57.39, 2.7820, 0, 0, 0, 100, 0),
(81859, 0, 3999.58, -4099.20, 57.39, 2.7820, 0, 0, 0, 100, 0),
(81859, 1, 4057.54, -4105.50, 48.43, 3.0333, 0, 0, 0, 100, 0),
(81859, 2, 4223.01, -4205.63, 31.18, 2.5974, 0, 0, 0, 100, 0),
(81859, 3, 4247.25, -4231.60, 28.61, 2.3218, 0, 0, 0, 100, 0),
(81859, 4, 4290.00, -4248.10, 19.96, 2.8009, 0, 0, 0, 100, 0),
(81859, 5, 4360.95, -4192.26, 10.22, 3.7630, 0, 0, 0, 100, 0),
(81859, 6, 4442.18, -4190.00, 6.630, 3.1268, 0, 0, 0, 100, 0),
(81859, 7, 4529.20, -4216.23, 7.105, 3.0946, 0, 0, 0, 100, 0),
(81859, 8, 4637.40, -4221.69, 7.510, 2.9650, 0, 0, 0, 100, 0),
(81859, 9, 4698.32, -4232.56, 10.09, 2.2582, 0, 0, 0, 100, 0),
(81859, 10, 4730.77, -4278.52, 19.36, 2.9219, 0, 0, 0, 100, 0),
(81859, 11, 4758.01, -4282.60, 21.56, 3.8290, 0, 0, 0, 100, 0),
(81859, 12, 4829.05, -4224.28, 32.75, 3.9115, 0, 0, 0, 100, 0),
(81859, 13, 4859.88, -4216.94, 34.58, 3.6389, 0, 0, 0, 100, 0),
(81859, 14, 4957.99, -4164.31, 38.50, 3.0617, 0, 0, 0, 100, 0),
(81859, 15, 5002.72, -4171.59, 44.70, 3.0185, 0, 0, 0, 100, 0),
(81859, 16, 5043.57, -4175.56, 45.45, 3.0774, 0, 0, 0, 100, 0);

DELETE FROM spell_loot_template WHERE entry IN (175093, 175094);
INSERT INTO spell_loot_template VALUES
(175093, 118093, 100, 1, 0, 1, 1, ''),
(175094, 118094, 100, 1, 0, 1, 1, '');

DELETE FROM item_script_names WHERE Id IN (118093, 118094);
INSERT INTO item_script_names VALUES
(118093, 'item_strongbox'),
(118094, 'item_strongbox');

DELETE FROM item_loot_template WHERE entry = 118094;
INSERT INTO item_loot_template VALUES
(118094, 115024, 100, 1, 0, 1, 1, ""),
(118094, 115025, 100, 1, 0, 1, 1, ""),
(118094, 115026, 100, 1, 0, 1, 1, ""),
(118094, 115027, 100, 1, 0, 1, 1, ""),
(118094, 115028, 100, 1, 0, 1, 1, ""),
(118094, 115029, 100, 1, 0, 1, 1, ""),
(118094, 115030, 100, 1, 0, 1, 1, ""),
(118094, 115031, 100, 1, 0, 1, 1, ""),
(118094, 115032, 100, 1, 0, 1, 1, ""),
(118094, 115033, 100, 1, 0, 1, 1, ""),
(118094, 115064, 100, 1, 0, 1, 1, ""),
(118094, 115065, 100, 1, 0, 1, 1, ""),
(118094, 115066, 100, 1, 0, 1, 1, ""),
(118094, 115067, 100, 1, 0, 1, 1, ""),
(118094, 115068, 100, 1, 0, 1, 1, ""),
(118094, 115069, 100, 1, 0, 1, 1, ""),
(118094, 115070, 100, 1, 0, 1, 1, ""),
(118094, 115132, 100, 1, 0, 1, 1, ""),
(118094, 115133, 100, 1, 0, 1, 1, ""),
(118094, 115134, 100, 1, 0, 1, 1, ""),
(118094, 115135, 100, 1, 0, 1, 1, ""),
(118094, 115136, 100, 1, 0, 1, 1, ""),
(118094, 115137, 100, 1, 0, 1, 1, ""),
(118094, 115138, 100, 1, 0, 1, 1, ""),
(118094, 119824, 100, 1, 0, 1, 1, ""),
(118094, 119825, 100, 1, 0, 1, 1, ""),
(118094, 119826, 100, 1, 0, 1, 1, ""),
(118094, 119827, 100, 1, 0, 1, 1, ""),
(118094, 119828, 100, 1, 0, 1, 1, ""),
(118094, 119829, 100, 1, 0, 1, 1, ""),
(118094, 119830, 100, 1, 0, 1, 1, ""),
(118094, 119831, 100, 1, 0, 1, 1, ""),
(118094, 119832, 100, 1, 0, 1, 1, ""),
(118094, 119833, 100, 1, 0, 1, 1, ""),
(118094, 119834, 100, 1, 0, 1, 1, ""),
(118094, 119835, 100, 1, 0, 1, 1, ""),
(118094, 119836, 100, 1, 0, 1, 1, ""),
(118094, 119837, 100, 1, 0, 1, 1, ""),
(118094, 119838, 100, 1, 0, 1, 1, ""),
(118094, 119839, 100, 1, 0, 1, 1, ""),
(118094, 119840, 100, 1, 0, 1, 1, ""),
(118094, 119841, 100, 1, 0, 1, 1, ""),
(118094, 119842, 100, 1, 0, 1, 1, ""),
(118094, 119843, 100, 1, 0, 1, 1, ""),
(118094, 119844, 100, 1, 0, 1, 1, ""),
(118094, 119845, 100, 1, 0, 1, 1, ""),
(118094, 119846, 100, 1, 0, 1, 1, ""),
(118094, 119847, 100, 1, 0, 1, 1, ""),
(118094, 119848, 100, 1, 0, 1, 1, ""),
(118094, 119849, 100, 1, 0, 1, 1, ""),
(118094, 119850, 100, 1, 0, 1, 1, ""),
(118094, 119851, 100, 1, 0, 1, 1, ""),
(118094, 119852, 100, 1, 0, 1, 1, ""),
(118094, 119853, 100, 1, 0, 1, 1, ""),
(118094, 119854, 100, 1, 0, 1, 1, ""),
(118094, 119855, 100, 1, 0, 1, 1, ""),
(118094, 119856, 100, 1, 0, 1, 1, ""),
(118094, 119857, 100, 1, 0, 1, 1, ""),
(118094, 119858, 100, 1, 0, 1, 1, ""),
(118094, 119859, 100, 1, 0, 1, 1, ""),
(118094, 119860, 100, 1, 0, 1, 1, ""),
(118094, 119861, 100, 1, 0, 1, 1, ""),
(118094, 119862, 100, 1, 0, 1, 1, ""),
(118094, 119863, 100, 1, 0, 1, 1, ""),
(118094, 119864, 100, 1, 0, 1, 1, ""),
(118094, 119865, 100, 1, 0, 1, 1, ""),
(118094, 119866, 100, 1, 0, 1, 1, ""),
(118094, 119867, 100, 1, 0, 1, 1, ""),
(118094, 119868, 100, 1, 0, 1, 1, ""),
(118094, 119869, 100, 1, 0, 1, 1, ""),
(118094, 119870, 100, 1, 0, 1, 1, ""),
(118094, 119871, 100, 1, 0, 1, 1, ""),
(118094, 119872, 100, 1, 0, 1, 1, ""),
(118094, 119873, 100, 1, 0, 1, 1, ""),
(118094, 119874, 100, 1, 0, 1, 1, ""),
(118094, 119875, 100, 1, 0, 1, 1, ""),
(118094, 119876, 100, 1, 0, 1, 1, ""),
(118094, 119877, 100, 1, 0, 1, 1, ""),
(118094, 119878, 100, 1, 0, 1, 1, ""),
(118094, 119879, 100, 1, 0, 1, 1, ""),
(118094, 119880, 100, 1, 0, 1, 1, ""),
(118094, 119881, 100, 1, 0, 1, 1, ""),
(118094, 119882, 100, 1, 0, 1, 1, ""),
(118094, 119883, 100, 1, 0, 1, 1, ""),
(118094, 119884, 100, 1, 0, 1, 1, ""),
(118094, 119885, 100, 1, 0, 1, 1, ""),
(118094, 119886, 100, 1, 0, 1, 1, ""),
(118094, 119887, 100, 1, 0, 1, 1, ""),
(118094, 119888, 100, 1, 0, 1, 1, ""),
(118094, 119889, 100, 1, 0, 1, 1, ""),
(118094, 119890, 100, 1, 0, 1, 1, ""),
(118094, 119891, 100, 1, 0, 1, 1, ""),
(118094, 119892, 100, 1, 0, 1, 1, ""),
(118094, 119893, 100, 1, 0, 1, 1, ""),
(118094, 119894, 100, 1, 0, 1, 1, ""),
(118094, 119895, 100, 1, 0, 1, 1, ""),
(118094, 119896, 100, 1, 0, 1, 1, ""),
(118094, 119897, 100, 1, 0, 1, 1, ""),
(118094, 119898, 100, 1, 0, 1, 1, ""),
(118094, 119899, 100, 1, 0, 1, 1, ""),
(118094, 119900, 100, 1, 0, 1, 1, ""),
(118094, 119901, 100, 1, 0, 1, 1, ""),
(118094, 119902, 100, 1, 0, 1, 1, ""),
(118094, 119903, 100, 1, 0, 1, 1, ""),
(118094, 119904, 100, 1, 0, 1, 1, ""),
(118094, 119905, 100, 1, 0, 1, 1, ""),
(118094, 119906, 100, 1, 0, 1, 1, ""),
(118094, 119907, 100, 1, 0, 1, 1, ""),
(118094, 119908, 100, 1, 0, 1, 1, ""),
(118094, 119909, 100, 1, 0, 1, 1, ""),
(118094, 119910, 100, 1, 0, 1, 1, ""),
(118094, 119911, 100, 1, 0, 1, 1, ""),
(118094, 119912, 100, 1, 0, 1, 1, ""),
(118094, 119913, 100, 1, 0, 1, 1, ""),
(118094, 119915, 100, 1, 0, 1, 1, ""),
(118094, 119916, 100, 1, 0, 1, 1, ""),
(118094, 119917, 100, 1, 0, 1, 1, ""),
(118094, 119918, 100, 1, 0, 1, 1, ""),
(118094, 119919, 100, 1, 0, 1, 1, ""),
(118094, 119920, 100, 1, 0, 1, 1, ""),
(118094, 119921, 100, 1, 0, 1, 1, ""),
(118094, 119922, 100, 1, 0, 1, 1, ""),
(118094, 119923, 100, 1, 0, 1, 1, ""),
(118094, 119924, 100, 1, 0, 1, 1, ""),
(118094, 119925, 100, 1, 0, 1, 1, ""),
(118094, 119926, 100, 1, 0, 1, 1, ""),
(118094, 119927, 100, 1, 0, 1, 1, ""),
(118094, 119928, 100, 1, 0, 1, 1, ""),
(118094, 119929, 100, 1, 0, 1, 1, ""),
(118094, 119930, 100, 1, 0, 1, 1, ""),
(118094, 119931, 100, 1, 0, 1, 1, ""),
(118094, 119932, 100, 1, 0, 1, 1, ""),
(118094, 119933, 100, 1, 0, 1, 1, ""),
(118094, 119934, 100, 1, 0, 1, 1, ""),
(118094, 119935, 100, 1, 0, 1, 1, ""),
(118094, 119936, 100, 1, 0, 1, 1, ""),
(118094, 119937, 100, 1, 0, 1, 1, ""),
(118094, 119938, 100, 1, 0, 1, 1, ""),
(118094, 119939, 100, 1, 0, 1, 1, ""),
(118094, 119940, 100, 1, 0, 1, 1, ""),
(118094, 119941, 100, 1, 0, 1, 1, ""),
(118094, 119942, 100, 1, 0, 1, 1, ""),
(118094, 119943, 100, 1, 0, 1, 1, ""),
(118094, 119944, 100, 1, 0, 1, 1, ""),
(118094, 119945, 100, 1, 0, 1, 1, ""),
(118094, 119946, 100, 1, 0, 1, 1, ""),
(118094, 119947, 100, 1, 0, 1, 1, ""),
(118094, 119948, 100, 1, 0, 1, 1, ""),
(118094, 119949, 100, 1, 0, 1, 1, ""),
(118094, 119950, 100, 1, 0, 1, 1, ""),
(118094, 119951, 100, 1, 0, 1, 1, ""),
(118094, 119952, 100, 1, 0, 1, 1, ""),
(118094, 119953, 100, 1, 0, 1, 1, ""),
(118094, 119954, 100, 1, 0, 1, 1, ""),
(118094, 119955, 100, 1, 0, 1, 1, ""),
(118094, 119956, 100, 1, 0, 1, 1, ""),
(118094, 119957, 100, 1, 0, 1, 1, ""),
(118094, 119958, 100, 1, 0, 1, 1, ""),
(118094, 119959, 100, 1, 0, 1, 1, ""),
(118094, 119960, 100, 1, 0, 1, 1, ""),
(118094, 119961, 100, 1, 0, 1, 1, ""),
(118094, 119962, 100, 1, 0, 1, 1, ""),
(118094, 119963, 100, 1, 0, 1, 1, ""),
(118094, 119964, 100, 1, 0, 1, 1, ""),
(118094, 119965, 100, 1, 0, 1, 1, ""),
(118094, 119966, 100, 1, 0, 1, 1, ""),
(118094, 119967, 100, 1, 0, 1, 1, ""),
(118094, 120049, 100, 1, 0, 1, 1, ""),
(118094, 119968, 100, 1, 0, 1, 1, ""),
(118094, 119969, 100, 1, 0, 1, 1, ""),
(118094, 119970, 100, 1, 0, 1, 1, ""),
(118094, 119971, 100, 1, 0, 1, 1, ""),
(118094, 119972, 100, 1, 0, 1, 1, ""),
(118094, 119973, 100, 1, 0, 1, 1, ""),
(118094, 119974, 100, 1, 0, 1, 1, ""),
(118094, 119975, 100, 1, 0, 1, 1, ""),
(118094, 119976, 100, 1, 0, 1, 1, ""),
(118094, 119977, 100, 1, 0, 1, 1, ""),
(118094, 119978, 100, 1, 0, 1, 1, ""),
(118094, 119979, 100, 1, 0, 1, 1, ""),
(118094, 119980, 100, 1, 0, 1, 1, ""),
(118094, 119981, 100, 1, 0, 1, 1, ""),
(118094, 119982, 100, 1, 0, 1, 1, ""),
(118094, 119983, 100, 1, 0, 1, 1, ""),
(118094, 119984, 100, 1, 0, 1, 1, ""),
(118094, 119985, 100, 1, 0, 1, 1, ""),
(118094, 119986, 100, 1, 0, 1, 1, ""),
(118094, 119987, 100, 1, 0, 1, 1, ""),
(118094, 119988, 100, 1, 0, 1, 1, ""),
(118094, 119989, 100, 1, 0, 1, 1, ""),
(118094, 119990, 100, 1, 0, 1, 1, ""),
(118094, 119991, 100, 1, 0, 1, 1, ""),
(118094, 119992, 100, 1, 0, 1, 1, ""),
(118094, 119993, 100, 1, 0, 1, 1, ""),
(118094, 119994, 100, 1, 0, 1, 1, ""),
(118094, 119995, 100, 1, 0, 1, 1, ""),
(118094, 119996, 100, 1, 0, 1, 1, ""),
(118094, 119997, 100, 1, 0, 1, 1, ""),
(118094, 119998, 100, 1, 0, 1, 1, ""),
(118094, 119999, 100, 1, 0, 1, 1, ""),
(118094, 120000, 100, 1, 0, 1, 1, ""),
(118094, 120001, 100, 1, 0, 1, 1, ""),
(118094, 120002, 100, 1, 0, 1, 1, ""),
(118094, 120003, 100, 1, 0, 1, 1, ""),
(118094, 120004, 100, 1, 0, 1, 1, ""),
(118094, 120005, 100, 1, 0, 1, 1, ""),
(118094, 120006, 100, 1, 0, 1, 1, ""),
(118094, 120007, 100, 1, 0, 1, 1, ""),
(118094, 120008, 100, 1, 0, 1, 1, ""),
(118094, 120009, 100, 1, 0, 1, 1, ""),
(118094, 120010, 100, 1, 0, 1, 1, ""),
(118094, 120011, 100, 1, 0, 1, 1, ""),
(118094, 120012, 100, 1, 0, 1, 1, ""),
(118094, 120013, 100, 1, 0, 1, 1, ""),
(118094, 120014, 100, 1, 0, 1, 1, ""),
(118094, 120015, 100, 1, 0, 1, 1, ""),
(118094, 120016, 100, 1, 0, 1, 1, ""),
(118094, 120017, 100, 1, 0, 1, 1, ""),
(118094, 120018, 100, 1, 0, 1, 1, ""),
(118094, 120019, 100, 1, 0, 1, 1, ""),
(118094, 120020, 100, 1, 0, 1, 1, ""),
(118094, 120021, 100, 1, 0, 1, 1, ""),
(118094, 120022, 100, 1, 0, 1, 1, ""),
(118094, 120023, 100, 1, 0, 1, 1, ""),
(118094, 120024, 100, 1, 0, 1, 1, ""),
(118094, 120025, 100, 1, 0, 1, 1, ""),
(118094, 120026, 100, 1, 0, 1, 1, ""),
(118094, 120027, 100, 1, 0, 1, 1, ""),
(118094, 120028, 100, 1, 0, 1, 1, ""),
(118094, 120029, 100, 1, 0, 1, 1, ""),
(118094, 120030, 100, 1, 0, 1, 1, ""),
(118094, 120031, 100, 1, 0, 1, 1, ""),
(118094, 120032, 100, 1, 0, 1, 1, ""),
(118094, 120033, 100, 1, 0, 1, 1, ""),
(118094, 120034, 100, 1, 0, 1, 1, ""),
(118094, 120035, 100, 1, 0, 1, 1, ""),
(118094, 120036, 100, 1, 0, 1, 1, ""),
(118094, 120037, 100, 1, 0, 1, 1, ""),
(118094, 120038, 100, 1, 0, 1, 1, ""),
(118094, 120039, 100, 1, 0, 1, 1, ""),
(118094, 120040, 100, 1, 0, 1, 1, ""),
(118094, 120041, 100, 1, 0, 1, 1, ""),
(118094, 120042, 100, 1, 0, 1, 1, ""),
(118094, 120043, 100, 1, 0, 1, 1, ""),
(118094, 120044, 100, 1, 0, 1, 1, ""),
(118094, 120045, 100, 1, 0, 1, 1, ""),
(118094, 120046, 100, 1, 0, 1, 1, ""),
(118094, 120047, 100, 1, 0, 1, 1, ""),
(118094, 120048, 100, 1, 0, 1, 1, "");

DELETE FROM item_loot_template WHERE entry = 118093;
INSERT INTO item_loot_template VALUES
(118093, 115034, 100, 1, 0, 1, 1, ""),
(118093, 115035, 100, 1, 0, 1, 1, ""),
(118093, 115036, 100, 1, 0, 1, 1, ""),
(118093, 115037, 100, 1, 0, 1, 1, ""),
(118093, 115038, 100, 1, 0, 1, 1, ""),
(118093, 115039, 100, 1, 0, 1, 1, ""),
(118093, 115040, 100, 1, 0, 1, 1, ""),
(118093, 115041, 100, 1, 0, 1, 1, ""),
(118093, 115042, 100, 1, 0, 1, 1, ""),
(118093, 115043, 100, 1, 0, 1, 1, ""),
(118093, 115044, 100, 1, 0, 1, 1, ""),
(118093, 115045, 100, 1, 0, 1, 1, ""),
(118093, 115046, 100, 1, 0, 1, 1, ""),
(118093, 115047, 100, 1, 0, 1, 1, ""),
(118093, 115048, 100, 1, 0, 1, 1, ""),
(118093, 115049, 100, 1, 0, 1, 1, ""),
(118093, 115050, 100, 1, 0, 1, 1, ""),
(118093, 115051, 100, 1, 0, 1, 1, ""),
(118093, 115052, 100, 1, 0, 1, 1, ""),
(118093, 115053, 100, 1, 0, 1, 1, ""),
(118093, 115054, 100, 1, 0, 1, 1, ""),
(118093, 115055, 100, 1, 0, 1, 1, ""),
(118093, 115056, 100, 1, 0, 1, 1, ""),
(118093, 115057, 100, 1, 0, 1, 1, ""),
(118093, 115058, 100, 1, 0, 1, 1, ""),
(118093, 115059, 100, 1, 0, 1, 1, ""),
(118093, 115060, 100, 1, 0, 1, 1, ""),
(118093, 115061, 100, 1, 0, 1, 1, ""),
(118093, 115062, 100, 1, 0, 1, 1, ""),
(118093, 115063, 100, 1, 0, 1, 1, ""),
(118093, 115071, 100, 1, 0, 1, 1, ""),
(118093, 115072, 100, 1, 0, 1, 1, ""),
(118093, 115073, 100, 1, 0, 1, 1, ""),
(118093, 115074, 100, 1, 0, 1, 1, ""),
(118093, 115075, 100, 1, 0, 1, 1, ""),
(118093, 115076, 100, 1, 0, 1, 1, ""),
(118093, 115077, 100, 1, 0, 1, 1, ""),
(118093, 115078, 100, 1, 0, 1, 1, ""),
(118093, 115079, 100, 1, 0, 1, 1, ""),
(118093, 115080, 100, 1, 0, 1, 1, ""),
(118093, 115081, 100, 1, 0, 1, 1, ""),
(118093, 115082, 100, 1, 0, 1, 1, ""),
(118093, 115083, 100, 1, 0, 1, 1, ""),
(118093, 115084, 100, 1, 0, 1, 1, ""),
(118093, 115085, 100, 1, 0, 1, 1, ""),
(118093, 115086, 100, 1, 0, 1, 1, ""),
(118093, 115087, 100, 1, 0, 1, 1, ""),
(118093, 115088, 100, 1, 0, 1, 1, ""),
(118093, 115089, 100, 1, 0, 1, 1, ""),
(118093, 115090, 100, 1, 0, 1, 1, ""),
(118093, 115091, 100, 1, 0, 1, 1, ""),
(118093, 115092, 100, 1, 0, 1, 1, ""),
(118093, 115093, 100, 1, 0, 1, 1, ""),
(118093, 115094, 100, 1, 0, 1, 1, ""),
(118093, 115095, 100, 1, 0, 1, 1, ""),
(118093, 115096, 100, 1, 0, 1, 1, ""),
(118093, 115097, 100, 1, 0, 1, 1, ""),
(118093, 115098, 100, 1, 0, 1, 1, ""),
(118093, 115099, 100, 1, 0, 1, 1, ""),
(118093, 115100, 100, 1, 0, 1, 1, ""),
(118093, 115101, 100, 1, 0, 1, 1, ""),
(118093, 115102, 100, 1, 0, 1, 1, ""),
(118093, 115103, 100, 1, 0, 1, 1, ""),
(118093, 115104, 100, 1, 0, 1, 1, ""),
(118093, 115105, 100, 1, 0, 1, 1, ""),
(118093, 115106, 100, 1, 0, 1, 1, ""),
(118093, 115107, 100, 1, 0, 1, 1, ""),
(118093, 115108, 100, 1, 0, 1, 1, ""),
(118093, 115109, 100, 1, 0, 1, 1, ""),
(118093, 115110, 100, 1, 0, 1, 1, ""),
(118093, 115111, 100, 1, 0, 1, 1, ""),
(118093, 115112, 100, 1, 0, 1, 1, ""),
(118093, 115113, 100, 1, 0, 1, 1, ""),
(118093, 115114, 100, 1, 0, 1, 1, ""),
(118093, 115115, 100, 1, 0, 1, 1, ""),
(118093, 115116, 100, 1, 0, 1, 1, ""),
(118093, 115117, 100, 1, 0, 1, 1, ""),
(118093, 115118, 100, 1, 0, 1, 1, ""),
(118093, 115119, 100, 1, 0, 1, 1, ""),
(118093, 115120, 100, 1, 0, 1, 1, ""),
(118093, 115121, 100, 1, 0, 1, 1, ""),
(118093, 115122, 100, 1, 0, 1, 1, ""),
(118093, 115123, 100, 1, 0, 1, 1, ""),
(118093, 115124, 100, 1, 0, 1, 1, ""),
(118093, 115125, 100, 1, 0, 1, 1, ""),
(118093, 115126, 100, 1, 0, 1, 1, ""),
(118093, 115127, 100, 1, 0, 1, 1, ""),
(118093, 115128, 100, 1, 0, 1, 1, ""),
(118093, 115129, 100, 1, 0, 1, 1, ""),
(118093, 115130, 100, 1, 0, 1, 1, ""),
(118093, 115131, 100, 1, 0, 1, 1, ""),
(118093, 115139, 100, 1, 0, 1, 1, ""),
(118093, 115140, 100, 1, 0, 1, 1, ""),
(118093, 115141, 100, 1, 0, 1, 1, ""),
(118093, 115142, 100, 1, 0, 1, 1, ""),
(118093, 115143, 100, 1, 0, 1, 1, ""),
(118093, 115144, 100, 1, 0, 1, 1, ""),
(118093, 115145, 100, 1, 0, 1, 1, ""),
(118093, 115146, 100, 1, 0, 1, 1, ""),
(118093, 115147, 100, 1, 0, 1, 1, ""),
(118093, 115148, 100, 1, 0, 1, 1, ""),
(118093, 115149, 100, 1, 0, 1, 1, ""),
(118093, 115150, 100, 1, 0, 1, 1, ""),
(118093, 115151, 100, 1, 0, 1, 1, ""),
(118093, 115152, 100, 1, 0, 1, 1, ""),
(118093, 115153, 100, 1, 0, 1, 1, ""),
(118093, 115154, 100, 1, 0, 1, 1, ""),
(118093, 115155, 100, 1, 0, 1, 1, ""),
(118093, 115156, 100, 1, 0, 1, 1, ""),
(118093, 115157, 100, 1, 0, 1, 1, ""),
(118093, 115158, 100, 1, 0, 1, 1, ""),
(118093, 115159, 100, 1, 0, 1, 1, ""),
(118093, 115160, 100, 1, 0, 1, 1, ""),
(118093, 115161, 100, 1, 0, 1, 1, ""),
(118093, 115162, 100, 1, 0, 1, 1, ""),
(118093, 115163, 100, 1, 0, 1, 1, ""),
(118093, 115164, 100, 1, 0, 1, 1, ""),
(118093, 115165, 100, 1, 0, 1, 1, ""),
(118093, 115166, 100, 1, 0, 1, 1, ""),
(118093, 115167, 100, 1, 0, 1, 1, ""),
(118093, 115168, 100, 1, 0, 1, 1, ""),
(118093, 115169, 100, 1, 0, 1, 1, ""),
(118093, 115170, 100, 1, 0, 1, 1, ""),
(118093, 115171, 100, 1, 0, 1, 1, ""),
(118093, 115172, 100, 1, 0, 1, 1, ""),
(118093, 115173, 100, 1, 0, 1, 1, ""),
(118093, 115174, 100, 1, 0, 1, 1, ""),
(118093, 115175, 100, 1, 0, 1, 1, ""),
(118093, 115176, 100, 1, 0, 1, 1, ""),
(118093, 115177, 100, 1, 0, 1, 1, ""),
(118093, 115178, 100, 1, 0, 1, 1, ""),
(118093, 115179, 100, 1, 0, 1, 1, ""),
(118093, 115180, 100, 1, 0, 1, 1, ""),
(118093, 115181, 100, 1, 0, 1, 1, ""),
(118093, 115182, 100, 1, 0, 1, 1, ""),
(118093, 115183, 100, 1, 0, 1, 1, ""),
(118093, 115184, 100, 1, 0, 1, 1, ""),
(118093, 115185, 100, 1, 0, 1, 1, ""),
(118093, 115186, 100, 1, 0, 1, 1, ""),
(118093, 115187, 100, 1, 0, 1, 1, ""),
(118093, 115188, 100, 1, 0, 1, 1, ""),
(118093, 115189, 100, 1, 0, 1, 1, ""),
(118093, 115190, 100, 1, 0, 1, 1, ""),
(118093, 115521, 100, 1, 0, 1, 1, ""),
(118093, 120053, 100, 1, 0, 1, 1, ""),
(118093, 120054, 100, 1, 0, 1, 1, ""),
(118093, 120055, 100, 1, 0, 1, 1, ""),
(118093, 120056, 100, 1, 0, 1, 1, ""),
(118093, 120057, 100, 1, 0, 1, 1, ""),
(118093, 120058, 100, 1, 0, 1, 1, ""),
(118093, 120059, 100, 1, 0, 1, 1, ""),
(118093, 120060, 100, 1, 0, 1, 1, ""),
(118093, 120061, 100, 1, 0, 1, 1, ""),
(118093, 120062, 100, 1, 0, 1, 1, ""),
(118093, 120063, 100, 1, 0, 1, 1, ""),
(118093, 120064, 100, 1, 0, 1, 1, ""),
(118093, 120065, 100, 1, 0, 1, 1, ""),
(118093, 120066, 100, 1, 0, 1, 1, ""),
(118093, 120067, 100, 1, 0, 1, 1, ""),
(118093, 120068, 100, 1, 0, 1, 1, ""),
(118093, 120069, 100, 1, 0, 1, 1, ""),
(118093, 120070, 100, 1, 0, 1, 1, ""),
(118093, 120071, 100, 1, 0, 1, 1, ""),
(118093, 120072, 100, 1, 0, 1, 1, ""),
(118093, 120073, 100, 1, 0, 1, 1, ""),
(118093, 120074, 100, 1, 0, 1, 1, ""),
(118093, 120075, 100, 1, 0, 1, 1, ""),
(118093, 120076, 100, 1, 0, 1, 1, ""),
(118093, 115191, 100, 1, 0, 1, 1, ""),
(118093, 115192, 100, 1, 0, 1, 1, ""),
(118093, 115193, 100, 1, 0, 1, 1, ""),
(118093, 115194, 100, 1, 0, 1, 1, ""),
(118093, 115195, 100, 1, 0, 1, 1, ""),
(118093, 115196, 100, 1, 0, 1, 1, ""),
(118093, 115197, 100, 1, 0, 1, 1, ""),
(118093, 115198, 100, 1, 0, 1, 1, ""),
(118093, 115199, 100, 1, 0, 1, 1, ""),
(118093, 115200, 100, 1, 0, 1, 1, ""),
(118093, 115201, 100, 1, 0, 1, 1, ""),
(118093, 115202, 100, 1, 0, 1, 1, ""),
(118093, 115203, 100, 1, 0, 1, 1, ""),
(118093, 115204, 100, 1, 0, 1, 1, ""),
(118093, 115205, 100, 1, 0, 1, 1, ""),
(118093, 115206, 100, 1, 0, 1, 1, ""),
(118093, 115207, 100, 1, 0, 1, 1, ""),
(118093, 115208, 100, 1, 0, 1, 1, ""),
(118093, 115209, 100, 1, 0, 1, 1, ""),
(118093, 115210, 100, 1, 0, 1, 1, ""),
(118093, 115211, 100, 1, 0, 1, 1, ""),
(118093, 115212, 100, 1, 0, 1, 1, ""),
(118093, 115213, 100, 1, 0, 1, 1, ""),
(118093, 115214, 100, 1, 0, 1, 1, ""),
(118093, 115215, 100, 1, 0, 1, 1, ""),
(118093, 115216, 100, 1, 0, 1, 1, ""),
(118093, 115217, 100, 1, 0, 1, 1, ""),
(118093, 115218, 100, 1, 0, 1, 1, ""),
(118093, 115219, 100, 1, 0, 1, 1, ""),
(118093, 115220, 100, 1, 0, 1, 1, ""),
(118093, 115221, 100, 1, 0, 1, 1, ""),
(118093, 115222, 100, 1, 0, 1, 1, ""),
(118093, 115223, 100, 1, 0, 1, 1, ""),
(118093, 115224, 100, 1, 0, 1, 1, ""),
(118093, 115225, 100, 1, 0, 1, 1, ""),
(118093, 115226, 100, 1, 0, 1, 1, ""),
(118093, 115227, 100, 1, 0, 1, 1, ""),
(118093, 115228, 100, 1, 0, 1, 1, ""),
(118093, 115229, 100, 1, 0, 1, 1, ""),
(118093, 115230, 100, 1, 0, 1, 1, ""),
(118093, 115231, 100, 1, 0, 1, 1, ""),
(118093, 115232, 100, 1, 0, 1, 1, ""),
(118093, 115233, 100, 1, 0, 1, 1, ""),
(118093, 115234, 100, 1, 0, 1, 1, ""),
(118093, 115235, 100, 1, 0, 1, 1, ""),
(118093, 115236, 100, 1, 0, 1, 1, ""),
(118093, 115237, 100, 1, 0, 1, 1, ""),
(118093, 115238, 100, 1, 0, 1, 1, ""),
(118093, 115239, 100, 1, 0, 1, 1, ""),
(118093, 115240, 100, 1, 0, 1, 1, ""),
(118093, 115241, 100, 1, 0, 1, 1, ""),
(118093, 115242, 100, 1, 0, 1, 1, ""),
(118093, 115243, 100, 1, 0, 1, 1, ""),
(118093, 115244, 100, 1, 0, 1, 1, ""),
(118093, 115245, 100, 1, 0, 1, 1, ""),
(118093, 115246, 100, 1, 0, 1, 1, ""),
(118093, 115247, 100, 1, 0, 1, 1, ""),
(118093, 115248, 100, 1, 0, 1, 1, ""),
(118093, 115249, 100, 1, 0, 1, 1, ""),
(118093, 115250, 100, 1, 0, 1, 1, ""),
(118093, 115251, 100, 1, 0, 1, 1, ""),
(118093, 115252, 100, 1, 0, 1, 1, ""),
(118093, 115253, 100, 1, 0, 1, 1, ""),
(118093, 115254, 100, 1, 0, 1, 1, ""),
(118093, 115255, 100, 1, 0, 1, 1, ""),
(118093, 115256, 100, 1, 0, 1, 1, ""),
(118093, 115257, 100, 1, 0, 1, 1, ""),
(118093, 115258, 100, 1, 0, 1, 1, ""),
(118093, 115259, 100, 1, 0, 1, 1, ""),
(118093, 115260, 100, 1, 0, 1, 1, ""),
(118093, 115261, 100, 1, 0, 1, 1, ""),
(118093, 115262, 100, 1, 0, 1, 1, ""),
(118093, 115263, 100, 1, 0, 1, 1, ""),
(118093, 115264, 100, 1, 0, 1, 1, ""),
(118093, 115265, 100, 1, 0, 1, 1, ""),
(118093, 115266, 100, 1, 0, 1, 1, ""),
(118093, 115267, 100, 1, 0, 1, 1, ""),
(118093, 115268, 100, 1, 0, 1, 1, ""),
(118093, 115269, 100, 1, 0, 1, 1, ""),
(118093, 115270, 100, 1, 0, 1, 1, ""),
(118093, 115271, 100, 1, 0, 1, 1, "");