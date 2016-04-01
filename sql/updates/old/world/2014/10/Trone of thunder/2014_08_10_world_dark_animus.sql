DELETE FROM creature_text WHERE entry = 69702;
INSERT INTO creature_text VALUES
(69702, 0, 0, 'Stop them! The Animus must not be disturbed!', 14, 0, 100, 0, 0, 35725, 'DARK_ANIMUS_INTRO'),
(69702, 1, 0, 'The power... It cannot be wielded this way! You tempt forces you do not understand!', 14, 0, 100, 0, 0, 35722, 'DARK_ANIMUS_AGGRO_01'),
(69702, 1, 1, 'The power... It cannot be wielded this way! You tempt forces you do not understand!', 14, 0, 100, 0, 0, 35723, 'DARK_ANIMUS_AGGRO_02'),
(69702, 2, 0, 'You have doomed yourselves. It... is... unleashed.', 14, 0, 100, 0, 0, 35724, 'DARK_ANIMUS_DEATH');

DELETE FROM locales_creature_text WHERE entry = 69702;
INSERT INTO locales_creature_text VALUES
(69702, 0, 0, '', 'Arrêtez-les ! L''Animus ne doit pas être perturbé !', '', '', '', '¡Detenlos! ¡El Animus no puede ser molestado!', '', '', '', ''),
(69702, 1, 0, '', 'Le pouvoir... il ne doit pas être utilisé comme ça ! Vous convoitez des forces qui vous dépassent.', '', '', '', 'El poder... ¡No puede ser controlado de esta manera! ¡Os involucráis en fuerzas que no entiendéis!', '', '', '', ''),
(69702, 1, 1, '', 'Le pouvoir... il ne doit pas être utilisé comme ça ! Vous convoitez des forces qui vous dépassent.', '', '', '', 'El poder... ¡No puede ser controlado de esta manera! ¡Os involucráis en fuerzas que no entiendéis!', '', '', '', ''),
(69702, 2, 0, '', 'Vous vous êtes condamnés. Il... Il est... libéré.', '', '', '', 'Os habéis condenado vosotros mismos. Ha... sido... liberado.', '', '', '', '');

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 2,
EXP = 4,
minlevel = 93,
maxlevel = 93,
rank = 3,
Health_mod = 148.56,
ScriptName = 'boss_dark_animus',
flags_extra = flags_extra | 0x01
WHERE entry = 69427;

UPDATE creature_model_info SET bounding_radius = 40, combat_reach = 16 WHERE modelid = 47527;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 40,
ScriptName = 'mob_dark_ritualist'
WHERE entry = 69702;

UPDATE creature_model_info SET bounding_radius = 0.825, combat_reach = 4.125 WHERE modelid = 47653;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
unit_class = 4,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 4000,
EXP = 4,
Health_mod = 2.278,
ScriptName = 'mob_anima_golem'
WHERE entry = 69701;

UPDATE creature_model_info SET bounding_radius = 5, combat_reach = 2 WHERE modelid = 47652;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
unit_class = 4,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 17.2624,
ScriptName = 'mob_large_anima_golem'
WHERE entry = 69700;

UPDATE creature_model_info SET bounding_radius = 12.5, combat_reach = 5 WHERE modelid = 47651;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
unit_class = 4,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 64,
ScriptName = 'mob_massive_anima_golem'
WHERE entry = 69699;

UPDATE creature_model_info SET bounding_radius = 25, combat_reach = 10 WHERE modelid = 47650;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_anima_orb'
WHERE entry = 69756;

UPDATE creature_model_info SET bounding_radius = 3, combat_reach = 6 WHERE modelid = 47700;

UPDATE creature_template SET
modelid1 = 43164,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
minlevel = 93,
maxlevel = 93,
ScriptName = 'mob_crimson_wake'
WHERE entry = 69951;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 2.4,
ScriptName = 'mob_quivering_blob'
WHERE entry = 69383;

UPDATE creature_model_info SET bounding_radius = 0.248, combat_reach = 0.8 WHERE modelid = 48115;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 5.6
WHERE entry = 69382;

UPDATE creature_model_info SET bounding_radius = 0.775, combat_reach = 3.75 WHERE modelid = 48115;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 40,
ScriptName = 'mob_ritual_guard'
WHERE entry = 70179;

UPDATE creature_model_info SET bounding_radius = 1.05, combat_reach = 5.25 WHERE modelid = 47906;

UPDATE creature_template SET modelid1 = 47700, ScriptName = 'mob_anima_orb' WHERE entry = 69756;

DELETE FROM conditions WHERE SourceEntry IN (138118);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 138118, 31, 0, 3, 69701);

DELETE FROM spell_script_names WHERE spell_id IN (137994, 138118, 138378, 138482, 138484, 138486, 138609, 139919, 136954, 139537, 138644, 138659, 138691, 138694, 138734, 138780);
INSERT INTO spell_script_names VALUES
(137994, 'spell_long_ritual_lightning'),
(138118, 'spell_transfusion_aoe'),
(138378, 'spell_transfusion_searcher'),
(138482, 'spell_crimson_wake_slow'),
(138484, 'spell_crimson_wake_medium'),
(138486, 'spell_fixated'),
(138609, 'spell_matter_swap'),
(139919, 'spell_targeted_matter_swap'),
(136954, 'spell_anima_ring'),
(139537, 'spell_activation_sequence'),
(138644, 'spell_siphon_anima'),
(138659, 'spell_touch_of_the_animus'),
(138691, 'spell_anima_font'),
(138694, 'spell_anima_font_dummy'),
(138734, 'spell_full_power'),
(138780, 'spell_empower_golem');