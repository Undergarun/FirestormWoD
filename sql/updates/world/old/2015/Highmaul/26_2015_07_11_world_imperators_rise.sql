SET NAMES utf8;

# Gorian Warden
UPDATE creature_template SET
    minlevel       = 101,
    maxlevel       = 101,
    faction        = 2734,
    dmg_multiplier = 4,
    ScriptName     = 'npc_highmaul_gorian_warden'
WHERE entry = 87619;

UPDATE creature_model_info SET bounding_radius = 0.9, combat_reach = 4.5 WHERE modelid = 60422;

# Night-Twisted Fanatic
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 1693,
    dmg_multiplier = 2,
    ScriptName     = 'npc_highmaul_night_twisted_fanatic'
WHERE entry = 87768;

UPDATE creature_model_info SET bounding_radius = 1.24, combat_reach = 4 WHERE modelid = 57226;

# Gorian Rune Mender
UPDATE creature_template SET
    minlevel       = 101,
    maxlevel       = 101,
    faction        = 2734,
    dmg_multiplier = 2,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_gorian_rune_mender'
WHERE entry = 87910;

UPDATE creature_model_info SET bounding_radius = 0.9, combat_reach = 4.5 WHERE modelid = 60608;

# Ogron Warbringer
UPDATE creature_template SET ScriptName = 'npc_highmaul_ogron_warbringer' WHERE entry = 87589;

# Warden Thultok <Defender of the Rise>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_warden_thultok'
WHERE entry = 81269;

UPDATE creature_model_info SET bounding_radius = 1.2, combat_reach = 6 WHERE modelid = 60608;

# Gorian Royal Guardsman <Kings Guard>
UPDATE creature_template SET ScriptName = 'npc_highmaul_gorian_royal_guardsman' WHERE entry = 81806;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 56643;

# Gorian High Sorcerer <Kings Guard>
UPDATE creature_template SET
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_gorian_high_sorcerer'
WHERE entry = 86256;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 59426;

# Ogron Mauler
UPDATE creature_template SET ScriptName = 'npc_highmaul_ogron_mauler' WHERE entry = 81779;

UPDATE creature_model_info SET bounding_radius = 0.3, combat_reach = 9 WHERE modelid = 55821;

# Guard Captain Thag <Kings Guard>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    ScriptName     = 'npc_highmaul_guard_captain_thag'
WHERE entry = 81780;

UPDATE creature_model_info SET bounding_radius = 1.2, combat_reach = 6 WHERE modelid = 56642;

# Rune
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, InhabitType = 4 WHERE entry = 77429;

# Councilor Nouk <Gorian War Council>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_councilor_nouk'
WHERE entry = 81807;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 56328;

# Councilor Gorluk <Gorian War Council>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_councilor_gorluk'
WHERE entry = 81809;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 56639;

# Councilor Magknor <Gorian War Council>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_councilor_magknor'
WHERE entry = 81808;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 56638;

# Councilor Daglat <Gorian War Council>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_councilor_daglat'
WHERE entry = 81810;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 56640;

# High Councilor Malgris <Gorian War Council>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_high_councilor_malgris'
WHERE entry = 81811;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 56641;

# Imperator Margok <Sorcerer King>
UPDATE creature_template SET
    minlevel       = 103,
    maxlevel       = 103,
    faction        = 16,
    dmg_multiplier = 8,
    ScriptName     = 'boss_imperator_margok'
WHERE entry = 77428;

UPDATE creature_model_info SET bounding_radius = 1.2, combat_reach = 6 WHERE modelid = 54329;

# Equipments
DELETE FROM creature_equip_template WHERE entry IN (87619, 87910, 81269, 86256, 81806, 81780, 81807, 81809, 81808, 81810, 81811);
INSERT INTO creature_equip_template VALUES
(87619, 1, 110457, 110457, 0),
(87910, 1, 111046, 0, 0),
(81269, 1, 118533, 0, 0),
(86256, 1, 111743, 0, 0),
(81806, 1, 110901, 0, 0),
(81780, 1, 110251, 110160, 0),
(81807, 1, 111743, 0, 0),
(81809, 1, 111743, 0, 0),
(81808, 1, 111743, 0, 0),
(81810, 1, 111743, 0, 0),
(81811, 1, 111743, 0, 0);

DELETE FROM areatrigger_template WHERE spell_id = 175047;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, scale_x, scale_y, flags, ScriptName) VALUE
(175047, 0, 175047, 2, 1, 1, 0x4000, 'areatrigger_highmaul_arcane_residue');

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_unstable_tempest');
INSERT INTO spell_script_names VALUES
(174981, 'spell_highmaul_unstable_tempest');

DELETE FROM creature_text WHERE entry IN (81811, 77428);
INSERT INTO creature_text VALUES
(81811, 0, 0, 'My liege, the city is lost! We must abandon the palace!', 14, 0, 100, 0, 0, 45106, 'MalgrisIntro1'),
(81811, 1, 0, 'The gates have been breached! They have come for the stones!', 14, 0, 100, 0, 0, 45107, 'MalgrisIntro2'),

(77428, 0, 0, 'Highmaul will never fall. The Gorian empire will not be overthrown by these peasants!', 14, 0, 100, 0, 0, 44768, 'MargokIntro1'),
(77428, 1, 0, 'End this now, Mal''gris. Remove them from my throne room.', 14, 0, 100, 0, 0, 44769, 'MargokIntro2'),
(77428, 2, 0, 'You dare assault the seat of my power? Is this the work of that traitor Cho''gall? Kneel and swear fealty to me, and I may yet spare your lives for my coliseum.', 14, 0, 100, 0, 0, 42029, 'MargokIntro3');

DELETE FROM locales_creature_text WHERE entry IN (81811, 77428);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    81811, 0, 0,
    'Seigneur, la cité est perdue ! Nous devons abandonner le palais !',
    'Mein Kaiser, die Stadt ist verloren! Wir müssen den Palast aufgeben!',
    'Mi señor, ¡la ciudad está perdida! ¡Debemos abandonar el lugar!',
    'Господин, город пал! Мы должны покинуть дворец!'
),
(
    81811, 1, 0,
    'Les portes ont été enfoncées ! Ils sont là pour les pierres !',
    'Die Tore sind durchbrochen! Sie haben es auf die Steine abgesehen!',
    '¡Han atravesado las puertas! ¡Vienen por las piedras!',
    'Они вошли в ворота и теперь идут за камнями!'
),
(
    77428, 0, 0,
    'Cognefort ne tombera jamais. L''empire gorien ne sera pas renversé par ces PAYSANS !',
    'Hochfels wird niemals fallen! Das gorianische Imperium wird sich nicht diesen BAUERN beugen!',
    'Gran Magullador jamás caerá. ¡El imperio Gorian no será derrotado por estos CAMPESINOS!',
    'Верховный Молот не падет. Этим рабам не сокрушить Горианскую империю!'
),
(
    77428, 1, 0,
    'Mettez un terme à tout ça, Mal''gris. Faites-les sortir de ma salle du trône.',
    'Schluss damit, Mal''gris. Entfernt diesen Unrat aus meinem Thronsaal.',
    'Termina con esto, Mal''gris. Sácalos de mi sala del trono.',
    'Довольно, Мал''грис. Очисти от них тронный зал.'
),
(
    77428, 2, 0,
    'Vous osez attaquer le siège de mon pouvoir ? Est-ce là l''œuvre de ce traître de Cho''gall ? À genoux, jurez allégeance et peut-être vous enverrai-je au colisée au lieu de vous tuer sur-le-champ.',
    'Ihr wagt es, den Herrschersitz von Hochfels anzugreifen? Steckt Cho''gall dahinter? Kniet nieder und schwört mir Lehnstreue, und vielleicht lasse ich euch in meinem Kolosseum kämpfen.',
    '¿Osan atacar la sede del poder Gran Magullador? ¿Este es el trabajo del traidor Cho''gall? Arrodíllense y júrenme fidelidad, y podría prescindir de sus vidas para mi coliseo.',
    'Кто смеет посягать на трон Верховного Молота?! Это что, происки Чо''Галла-предателя? Преклоните колени и присягните мне, и, возможно, я сохраню вам жизнь.'
);