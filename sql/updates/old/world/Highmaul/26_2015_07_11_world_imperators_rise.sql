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

# Phantasmal Weapon
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 14,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_phantasmal_weapon'
WHERE entry = 87293;

UPDATE creature_model_info SET bounding_radius = 1.125, combat_reach = 2.25 WHERE modelid = 60239;

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

# Arcane Torrent
UPDATE creature_template SET
    minlevel       = 100,
    maxlevel       = 100,
    faction        = 14,
    unit_flags     = 33555200,
    unit_flags2    = 2048,
    modelid1       = 11686,
    modelid2       = 0,
    ScriptName     = 'npc_highmaul_arcane_torrent'
WHERE entry = 87256;

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
    minlevel                = 103,
    maxlevel                = 103,
    faction                 = 16,
    dmg_multiplier          = 8,
    VehicleId               = 3971,
    ScriptName              = 'boss_imperator_margok',
    mechanic_immune_mask    = 617299839,
    flags_extra             = (0x01 | 0x00200000)
WHERE entry = 77428;

UPDATE creature_model_info SET bounding_radius = 1.2, combat_reach = 6 WHERE modelid = 54329;

UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, flags_extra = 128 WHERE entry = 89185;

# Rune
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 14,
    modelid1       = 11686,
    modelid2       = 0,
    flags_extra    = 128,
    ScriptName     = 'npc_highmaul_rune_of_displacement'
WHERE entry = 77429;

UPDATE gameobject_template SET flags = 32 WHERE entry IN (231988, 231989, 231990);

UPDATE creature_template SET InhabitType = 4 WHERE entry = 68553;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry = 89081;

# Arcane Aberration
# Displacing Arcane Aberration
# Fortified Arcane Aberration
# Replicating Arcane Aberration
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    unit_class     = 8,
    dmg_multiplier = 4,
    ScriptName     = 'npc_highmaul_arcane_aberration'
WHERE entry IN (77809, 77879, 77878, 77877);

UPDATE creature_model_info SET bounding_radius = 0.7, combat_reach = 5 WHERE modelid IN (55131, 55142, 55143, 55144);

# Destructive Resonance
UPDATE creature_template SET
    modelid1       = 11686,
    modelid2       = 0,
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 14,
    ScriptName     = 'npc_highmaul_destructive_resonance'
WHERE entry = 77637;

# Destructive Resonance (Replication)
UPDATE creature_template SET
    modelid1       = 11686,
    modelid2       = 0,
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 14,
    ScriptName     = 'npc_highmaul_destructive_resonance_replication'
WHERE entry = 77681;

# Gorian Warmage
UPDATE creature_template SET
    minlevel                = 102,
    maxlevel                = 102,
    faction                 = 16,
    unit_class              = 8,
    dmg_multiplier          = 4,
    ScriptName              = 'npc_highmaul_gorian_warmage',
    mechanic_immune_mask    = 617299807
WHERE entry = 78121;

UPDATE creature_model_info SET bounding_radius = 0.9, combat_reach = 4.5 WHERE modelid = 54330;

# Volatile Anomaly
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    unit_class     = 8,
    dmg_multiplier = 4,
    ScriptName     = 'npc_highmaul_volatile_anomaly'
WHERE entry = 78077;

UPDATE creature_model_info SET bounding_radius = 0.465, combat_reach = 1.5 WHERE modelid = 54282;

# Gorian Reaver
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    ScriptName     = 'npc_highmaul_gorian_reaver'
WHERE entry = 78549;

UPDATE creature_model_info SET bounding_radius = 1.125, combat_reach = 5.625 WHERE modelid = 54586;

# Arcane Remnant
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 2,
    ScriptName     = 'npc_highmaul_arcane_remnant'
WHERE entry = 79388;

UPDATE creature_model_info SET bounding_radius = 0.2625, combat_reach = 1.875 WHERE modelid = 55373;

# Equipments
DELETE FROM creature_equip_template WHERE entry IN (87619, 87910, 81269, 86256, 81806, 81780, 81807, 81809, 81808, 81810, 81811, 87293, 77428, 78121, 78549);
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
(81811, 1, 111743, 0, 0),
(87293, 1, 79006, 79006, 0),
(77428, 1, 115015, 0, 0),
(78121, 1, 108761, 108761, 0),
(78549, 1, 107737, 0, 0);

DELETE FROM areatrigger_template WHERE spell_id IN (175047, 158639);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, scale_x, scale_y, flags, move_curve_id, ScriptName) VALUE
(175047, 0, 175047, 2, 1, 1, 0x4000, 0, 'areatrigger_highmaul_arcane_residue'),
(158639, 0, 6644, 2, 2, 2, 0x4080, 605, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 1, 6644, 2, 2, 2, 0x4080, 606, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 2, 6644, 2, 2, 2, 0x4080, 607, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 3, 6644, 2, 2, 2, 0x4080, 608, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 4, 6644, 2, 2, 2, 0x4080, 609, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 5, 6644, 2, 2, 2, 0x4080, 610, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 6, 6644, 2, 2, 2, 0x4080, 611, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 7, 6644, 2, 2, 2, 0x4080, 612, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 8, 6644, 2, 2, 2, 0x4080, 613, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 9, 6644, 2, 2, 2, 0x4080, 614, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 10, 6644, 2, 2, 2, 0x4080, 615, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 11, 6644, 2, 2, 2, 0x4080, 616, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 12, 6644, 2, 2, 2, 0x4080, 617, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 13, 6644, 2, 2, 2, 0x4080, 618, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 14, 6644, 2, 2, 2, 0x4080, 619, 'areatrigger_highmaul_orb_of_chaos'),
(158639, 15, 6644, 2, 2, 2, 0x4080, 620, 'areatrigger_highmaul_orb_of_chaos');

DELETE FROM spell_script_names WHERE ScriptName IN
(
    'spell_highmaul_unstable_tempest',
    'spell_highmaul_time_stop',
    'spell_highmaul_mark_of_chaos',
    'spell_highmaul_destructive_resonance',
    'spell_highmaul_branded',
    'spell_highmaul_branded_displacement',
    'spell_highmaul_branded_fortification',
    'spell_highmaul_branded_replication',
    'spell_highmaul_arcane_wrath_damage',
    'spell_highmaul_transition_visuals',
    'spell_highmaul_berserker_rush_damage',
    'spell_highmaul_dominance_aura',
    'spell_highmaul_force_nova_fortified',
    'spell_highmaul_devastating_shockwave',
    'spell_highmaul_force_nova_dot',
    'spell_highmaul_orbs_of_chaos_aura'
);
INSERT INTO spell_script_names VALUES
(174981, 'spell_highmaul_unstable_tempest'),
(174939, 'spell_highmaul_time_stop'),
(158605, 'spell_highmaul_mark_of_chaos'),
(164176, 'spell_highmaul_mark_of_chaos'),
(164178, 'spell_highmaul_mark_of_chaos'),
(164191, 'spell_highmaul_mark_of_chaos'),
(174116, 'spell_highmaul_destructive_resonance'),
(156225, 'spell_highmaul_branded'),
(164004, 'spell_highmaul_branded_displacement'),
(164005, 'spell_highmaul_branded_fortification'),
(164006, 'spell_highmaul_branded_replication'),
(156239, 'spell_highmaul_arcane_wrath_damage'),
(176580, 'spell_highmaul_transition_visuals'),
(159002, 'spell_highmaul_berserker_rush_damage'),
(174126, 'spell_highmaul_dominance_aura'),
(157323, 'spell_highmaul_force_nova_fortified'),
(158547, 'spell_highmaul_devastating_shockwave'),
(157353, 'spell_highmaul_force_nova_dot'),
(160447, 'spell_highmaul_orbs_of_chaos_aura');

DELETE FROM spell_proc_event WHERE entry IN (174116);
INSERT INTO spell_proc_event (entry, procFlags) VALUE
(174116, 0x800AAAA8);

DELETE FROM spell_target_position WHERE id IN (166090, 164336, 164751, 164810);
INSERT INTO spell_target_position VALUE
(166090, 0, 1228, 3878.66, 8593.54, 565.314, 6.192306),
(164336, 0, 1228, 3944.7, 8616.53, 565.314, 0.715585),
(164751, 0, 1228, 3894.11, 8568.27, 565.314, 3.735005),
(164810, 0, 1228, 3898.19, 8618.52, 565.341, 2.443461);

DELETE FROM creature_text WHERE entry IN (81811, 77428, 77637);
INSERT INTO creature_text VALUES
(81811, 0, 0, 'My liege, the city is lost! We must abandon the palace!', 14, 0, 100, 0, 0, 45106, 'MalgrisIntro1'),
(81811, 1, 0, 'The gates have been breached! They have come for the stones!', 14, 0, 100, 0, 0, 45107, 'MalgrisIntro2'),

(77428, 0, 0, 'Highmaul will never fall. The Gorian empire will not be overthrown by these peasants!', 14, 0, 100, 0, 0, 44768, 'MargokIntro1'),
(77428, 1, 0, 'End this now, Mal''gris. Remove them from my throne room.', 14, 0, 100, 0, 0, 44769, 'MargokIntro2'),
(77428, 2, 0, 'You dare assault the seat of my power? Is this the work of that traitor Cho''gall? Kneel and swear fealty to me, and I may yet spare your lives for my coliseum.', 14, 0, 100, 0, 0, 42029, 'MargokIntro3'),
(77428, 3, 0, 'You will learn why I am king, and you are little more than peasants.', 14, 0, 100, 0, 0, 42015, 'MargokAggro'),
(77428, 4, 0, 'I command the arcane!', 14, 0, 100, 0, 0, 42038, 'MargokForceNova'),
(77428, 5, 0, 'Begone from my sight.', 14, 0, 100, 0, 0, 42037, 'MargokMarkOfChaos1'),
(77428, 5, 1, 'Come, see the error of your ways.', 14, 0, 100, 0, 0, 42036, 'MargokMarkOfChaos2'),
(77428, 6, 0, 'You have earned my wrath!', 14, 0, 100, 0, 0, 42034, 'MargokArcaneWrath1'),
(77428, 6, 1, 'My power knows no limit!', 14, 0, 100, 0, 0, 42033, 'MargokArcaneWrath2'),
(77428, 7, 0, 'The arcane bends to my will, as shall you!', 14, 0, 100, 0, 0, 42035, 'MargokArcaneAberration'),
(77428, 8, 0, 'Look upon the tools of your demise! The ancient stones grant me unimaginable power!', 14, 0, 100, 0, 0, 42018, 'MargokRuneOfDisplacement'),
(77428, 9, 0, 'You have seen but a fraction of my power. Behold the ancient magics unearthed by my empire!', 14, 0, 100, 0, 0, 42019, 'MargokRuneOfFortification1'),
(77428, 10, 0, 'King''s guard, to my side! I must focus my power and awaken the stone!', 14, 0, 100, 0, 0, 42020, 'MargokRuneOfFortification2'),
(77428, 11, 0, 'Such power! It takes form to destroy you!', 14, 0, 100, 0, 0, 42021, 'MargokRuneOfFortification3'),
(77428, 12, 0, 'Its energy overflows! Soon it will be unleashed!', 14, 0, 100, 0, 0, 42022, 'MargokRuneOfFortification4'),
(77428, 13, 0, 'The boundless power of the stones flows through me! Your destruction comes!',14, 0, 100, 0, 0, 42023, 'MargokRuneOfFortification5'),
(77428, 14, 0, 'Enough! Long have the Sorcerer Kings guarded these chambers. Today you will see our secret power.', 14, 0, 100, 0, 0, 42024, 'MargokRuneOfReplication1'),
(77428, 15, 0, 'Guards, remove these peasants.', 14, 0, 100, 0, 0, 42025, 'MargokRuneOfReplication2'),
(77428, 16, 0, 'Its power grows beyond comprehension!', 14, 0, 100, 0, 0, 42026, 'MargokRuneOfReplication3'),
(77428, 17, 0, 'You can never challenge my rule! I command the power of the stones and know no equal!', 14, 0, 100, 0, 0, 42027, 'MargokRuneOfReplication4'),
(77428, 18, 0, 'Your presence bores me.', 14, 0, 100, 0, 0, 42032, 'MargokSlay1'),
(77428, 18, 1, 'Learn your place.', 14, 0, 100, 0, 0, 42031, 'MargokSlay2'),
(77428, 19, 0, 'I tire of this. Begone!', 14, 0, 100, 0, 0, 42016, 'MargokBerserk'),
(77428, 20, 0, 'I... am... king...', 14, 0, 100, 0, 0, 42017, 'MargokDeath'),
(77428, 21, 0, '|TINTERFACE\\ICONS\\ability_socererking_arcanewrath.blp:20|t You have been branded by |cFFF00000|Hspell:156238|h[Arcane Wrath]|h|r!', 42, 0, 100, 0, 0, 0, 'MargokBranded'),
(77428, 22, 0, '|TINTERFACE\\ICONS\\spell_Mage_NetherTempest.blp:20|t |cFFF00000|Hspell:158648|h[Orbs of Chaos]|h|r explode from $n!', 41, 0, 100, 0, 0, 0, 'MargokOrbsOfChaos'),

(77637, 0, 0, '$n detonated Destructive Resonance!', 16, 0, 100, 0, 0, 0, 'MargokDestructiveResonance');

DELETE FROM locales_creature_text WHERE entry IN (81811, 77428, 77637);
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
),
(
    77428, 3, 0,
    'Vous allez comprendre pourquoi je suis le roi, et vous de simples paysans.',
    'Ihr werdet lernen, warum ich der König bin und ihr nichts weiter als Bauerntrampel.',
    'Aprenderán por qué soy el rey y ustedes poco más que campesinos.',
    'Вы узнаете, почему я – король, а вы – ничтожества.'
),
(
    77428, 4, 0,
    'Je commande aux Arcanes !',
    'Ich befehlige das Arkane!',
    '¡Ordeno a los arcanos!',
    'Я повелеваю магией!'
),
(
    77428, 5, 0,
    'Disparaissez de ma vue.',
    'Geht mir aus den Augen.',
    'Desaparezcan de mi vista.',
    'Прочь с глаз моих.'
),
(
    77428, 5, 1,
    'Approchez, je vous ferai comprendre vos erreurs.',
    'Kommt, lasst mich euch den wahren Weg zeigen.',
    'Vengan, vean el error de sus acciones.',
    'Сейчас вы поймете свои ошибки.'
),
(
    77428, 6, 0,
    'Tremblez devant mon courroux !',
    'Ihr verdient meinen Groll!',
    '¡Se han ganado mi ira!',
    'Вы заслужили мой гнев!'
),
(
    77428, 6, 1,
    'Mon pouvoir n''a aucune limite !',
    'Meine Macht kennt keine Grenzen!',
    '¡Mi poder no conoce límite!',
    'Моя сила безгранична!'
),
(
    77428, 7, 0,
    'Les Arcanes se plient à ma volonté, et vous en ferez autant !',
    'Das Arkane beugt sich meinem Willen, genau wie ihr!',
    '¡Los arcanos se someten a mi voluntad, y ustedes también lo harán!',
    'Магия покорна мне, покоритесь и вы!'
),
(
    77428, 8, 0,
    'Contemplez les armes qui causeront votre chute ! Les pierres antiques me confèrent une puissance inimaginable !',
    'Dies wird euer Untergang sein! Die uralten Steine gewähren mir unvorstellbare Macht!',
    '¡Observen las herramientas de su fin! ¡Las piedras antiguas me dan poder inimaginable!',
    'Смотрите! Эти камни несут погибель! Они даруют мне невиданную силу!'
),
(
    77428, 9, 0,
    'Ce n''était qu''un aperçu de mon pouvoir. Admirez les magies ancestrales que mon empire a exhumées !',
    'Das war nur ein Bruchteil meiner Kräfte! Werdet Zeuge der uralten Magie, die mein Reich zutage gefördert hat.',
    'Han visto solo una parte de mi poder. Contemplen las magias antiguas desenterradas por mi imperio.',
    'Это лишь часть моей мощи! А теперь узрите древнюю магию моей империи!'
),
(
    77428, 10, 0,
    'Gardes du roi, défendez-moi ! Je dois concentrer mon énergie et réveiller la pierre !',
    'Königswache, zu mir. Ich muss mich konzentrieren, um den Stein zu erwecken!',
    '¡Guardias reales, a mi lado! Debo concentrar mi poder y despertar a la piedra.',
    'Гвардия, защищайте меня! Я должен пробудить силу камня!'
),
(
    77428, 11, 0,
    'Tant de puissance ! Elle prend forme pour vous annihiler !',
    'Welch eine Macht! Sie nimmt Gestalt an, um euch zu zerstören!',
    '¡Tanto poder! Toma forma para destruirte.',
    'Какая мощь! И она уничтожит вас!'
),
(
    77428, 12, 0,
    'Son énergie déborde ! Bientôt, elle sera libérée !',
    'Ihre Energie quillt über. Bald schon wird sie entfesselt!',
    '¡Su poder se desborda! ¡Pronto será liberado!',
    'Он полон энергии. И скоро она высвободится!'
),
(
    77428, 13, 0,
    'La puissance infinie des pierres déferle dans mes veines ! Je vais vous pulvériser !',
    'Die grenzenlose Energie der Steine erfüllt mich! Die Zeit eurer Vernichtung naht!',
    '¡El poder sin límites de las piedras fluye a través de mí! ¡Su destrucción se acerca!',
    'По моим жилам бежит бесконечная мощь камней! Близится ваша погибель!'
),
(
    77428, 14, 0,
    'Assez ! Les rois-sorciers protègent ces salles depuis la nuit des temps. Aujourd''hui, vous allez découvrir leur puissance cachée.',
    'Genug! Lange schon hüten die Zaubererkönige diese Hallen. Heute werdet ihr Zeuge unserer geheimen Macht.',
    '¡Suficiente! Los Reyes hechiceros han protegido estas cámaras por mucho tiempo. Hoy verán nuestro poder secreto.',
    'Хватит! Короли-чародеи веками охраняли эти залы. И сейчас вы узнаете, почему.'
),
(
    77428, 15, 0,
    'Gardes, débarrassez-vous de ces paysans.',
    'Wache, entfernt diesen Pöbel.',
    'Guardias, llévense a estos campesinos.',
    'Стража! Убрать это отребье!'
),
(
    77428, 16, 0,
    'Son pouvoir grandit au-delà de toute compréhension !',
    'Die Macht wächst ins Unermessliche.',
    '¡Su poder va más allá de la comprensión!',
    'Эта сила беспредельна!'
),
(
    77428, 17, 0,
    'Mon règne est absolu ! Je commande la puissance des pierres ; nul ne peut rivaliser avec moi !',
    'Zweifelt niemals meine Herrschaft an! Ich beherrsche die Macht der Steine! Niemand ist mir ebenbürtig!',
    '¡No podrán jamás desafiar mi mando! ¡Comando el poder de las piedras y no hay nadie igual!',
    'Вам не удастся меня свергнуть! Я повелеваю силой камней! Мне нет равных!'
),
(
    77428, 18, 0,
    'Votre présence m''assomme.',
    'Eure Anwesenheit langweilt mich.',
    'Su presencia me molesta.',
    'Ты утомляешь меня.'
),
(
    77428, 18, 1,
    'Mon pouvoir n''a aucune limite !',
    'Meine Macht kennt keine Grenzen!',
    '¡Mi poder no conoce límite!',
    'Моя сила безгранична!'
),
(
    77428, 19, 0,
    'Vous me fatiguez. Fichez le camp !',
    'Ich habe genug. Hinfort mit euch!',
    'Me aburre esto. ¡Largo!',
    'Я устал. Прочь!'
),
(
    77428, 20, 0,
    'Je... suis... le roi...',
    'Ich... bin... der König...',
    'Yo... soy... rey...',
    'Я... Король...'
),
(
    77428, 21, 0,
    '|TINTERFACE\\ICONS\\ability_socererking_arcanewrath.blp:20|t Vous avez été $gmarqué:marquée; par |cFFF00000|Hspell:156238|h[Colère des Arcanes]|h|r !',
    '|TINTERFACE\\ICONS\\ability_socererking_arcanewrath.blp:20|t Ihr wurdet von |cFFF00000|Hspell:156238|h[Arkanem Zorn]|h|r gebrandmarkt!',
    '¡|TINTERFACE\\ICONS\\ability_socererking_arcanewrath.blp:20|t Te ha marcado |cFFF00000|Hspell:156238|h[Cólera Arcana]|h|r!',
    '|TINTERFACE\\ICONS\\ability_socererking_arcanewrath.blp:20|t Вы отмечены заклинанием |cFFF00000|Hspell:156238|h[\"Чародейский гнев\"]|h|r!'
),
(
    77428, 22, 0,
    '|TINTERFACE\\ICONS\\spell_Mage_NetherTempest.blp:20|t Des |cFFF00000|Hspell:158648|h[Orbes du chaos]|h|r jaillissent de $n !',
    '|TINTERFACE\\ICONS\\spell_Mage_NetherTempest.blp:20|t |cFFF00000|Hspell:158648|h[Kugeln des Chaos]|h|r explodieren aus $n!',
    '¡|TINTERFACE\\ICONS\\spell_Mage_NetherTempest.blp:20|t |cFFF00000|Hspell:158648|h[Orbe del caos]|h|r explotó por $n!',
    '|TINTERFACE\\ICONS\\spell_Mage_NetherTempest.blp:20|t |cFFF00000|Hspell:158648|h[Сферы хаоса]|h|r взрываются вокруг |3-1($n)!'
),
(
    77637, 0, 0,
    '$n a fait exploser la résonance destructrice !',
    '$n hat destruktive Resonanz detonieren lassen!',
    '¡$n detonó Resonancia destructiva!',
    '$n вызывает взрыв Разрушительного резонанса!'
);

SET @REF_MARGOK_NHM = 77428;

DELETE FROM `reference_loot_template` WHERE entry = @REF_MARGOK_NHM;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_MARGOK_NHM, 113848, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113849, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113850, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113851, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113852, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113853, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113854, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113855, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113856, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113857, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113858, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113859, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113860, 0, 1, 1, 1, 1),
(@REF_MARGOK_NHM, 113861, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_MARGOK_NHM WHERE `entry`= @REF_MARGOK_NHM;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_MARGOK_NHM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_MARGOK_NHM, 1, 100, 1, 0, -@REF_MARGOK_NHM, 6);