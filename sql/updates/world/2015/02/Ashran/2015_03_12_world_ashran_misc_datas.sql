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

DELETE FROM gossip_menu WHERE entry IN (65591, 65592, 65593, 65594, 65595);
DELETE FROM gossip_menu WHERE text_id IN (84919, 85463, 84923, 89853, 83895);
INSERT INTO gossip_menu (entry, text_id) VALUES
(65591, 84919), -- Mages
(65592, 85463), -- Warlocks
(65593, 84923), -- Warrior/Paladin
(65594, 89853), -- Kronus
(65595, 83895); -- Fangraal

DELETE FROM npc_text WHERE ID IN (84919, 85463, 84923, 89853, 83895);
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
);

DELETE FROM locales_npc_text WHERE entry IN (84919, 85463, 84923, 89853, 83895);
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
);

DELETE FROM creature_text WHERE entry IN (83995, 82966, 83997, 82660, 82204, 81870, 83830, 82893, 81859);
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
(81859, 1, 0, 'And now, let''s clear those Horde from this land.', 12, 0, 100, 0, 0, 0, 'FangraalAwake2');

DELETE FROM locales_creature_text WHERE entry IN (83995, 82966, 83997, 82660, 82204, 81870, 83830, 82893, 81859);
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