DELETE FROM spell_script_names WHERE spell_id IN (137162, 138349, 138389, 137399, 137437, 137429, 137313, 138732, 138743, 137530, 138133);
INSERT INTO spell_script_names VALUES
(137162, "spell_static_burst"),
(138349, 'spell_static_wound'),
(138389, "spell_static_wound_damage"),
(137399, "spell_focused_lightning"),
(137437, "spell_focused_lightning_periodic"),
(137429, "spell_focused_lightning_damage"),
(137313, 'spell_lightning_storm_periodic'),
(138732, 'spell_ionization'),
(138743, 'spell_ionization_conduction'),
(137530, 'spell_ionization_conduction'),
(138133, 'spell_ionization_conduction');

UPDATE access_requirement SET level_min = 90, level_max = 90 WHERE mapId = 1098 AND difficulty = 3; -- 10 Normal
UPDATE access_requirement SET level_min = 90, level_max = 90 WHERE mapId = 1098 AND difficulty = 4; -- 25 Normal
UPDATE access_requirement SET level_min = 90, level_max = 90 WHERE mapId = 1098 AND difficulty = 5; -- 10 Heroic
UPDATE access_requirement SET level_min = 90, level_max = 90 WHERE mapId = 1098 AND difficulty = 6; -- 25 Heroic
UPDATE access_requirement SET level_min = 90, level_max = 90 WHERE mapId = 1098 AND difficulty = 7; -- LFR

DELETE FROM spell_proc_event WHERE entry = 138349;
INSERT INTO spell_proc_event (entry, procFlags, procEx) VALUE (138349, 0x08, 0x03);

DELETE FROM creature_text WHERE entry = 69465;
INSERT INTO creature_text VALUES
(69465, 0, 0, 'Da Thunder King give me power! Come! I show you!', 14, 0, 100, 0, 0, 35550, 'JIN_ROKH_INTRO'),
(69465, 1, 0, 'I must break you!', 14, 0, 100, 0, 0, 35547, 'JIN_ROKH_AGGRO'),
(69465, 2, 0, 'Zap!', 14, 0, 100, 0, 0, 35556, 'JIN_ROKH_FOCUSED_LIGHTNING'),
(69465, 3, 0, 'Smash!', 14, 0, 100, 0, 0, 35553, 'JIN_ROKH_STATIC_BURST'),
(69465, 4, 0, 'Crush your bones!', 14, 0, 100, 0, 0, 35554, 'JIN_ROKH_THUNDERING_THROW'),
(69465, 5, 0, 'I call da storm!', 14, 0, 100, 0, 0, 35555, 'JIN_ROKH_LIGHTNING_STORM'),
(69465, 6, 0, 'Weak!', 14, 0, 100, 0, 0, 35551, 'JIN_ROKH_SLAY_01'),
(69465, 6, 1, 'So easy to break!', 14, 0, 100, 0, 0, 35552, 'JIN_ROKH_SLAY_02'),
(69465, 7, 0, 'Bored. You die now.', 14, 0, 100, 0, 0, 35548, 'JIN_ROKH_ENRAGE'),
(69465, 8, 0, 'How... you... beat me?', 14, 0, 100, 0, 0, 35549, 'JIN_ROKH_DEATH');

DELETE FROM locales_creature_text WHERE entry = 69465;
INSERT INTO locales_creature_text VALUES
(69465, 0, 0, '', 'Le roi-tonnerre, il me donne sa force ! Je vais vous montrer !', '', '', '', '', '', '', '', ''),
(69465, 1, 0, '', 'Je dois vous briser.', '', '', '', '', '', '', '', ''),
(69465, 2, 0, '', 'Gzzzzz !', '', '', '', '', '', '', '', ''),
(69465, 3, 0, '', 'Crac !', '', '', '', '', '', '', '', ''),
(69465, 4, 0, '', 'Vos os, écrasés !', '', '', '', '', '', '', '', ''),
(69465, 5, 0, '', 'J''appelle la tempête ! ', '', '', '', '', '', '', '', ''),
(69465, 6, 0, '', 'Faible.', '', '', '', '', '', '', '', ''),
(69465, 6, 1, '', 'Si facile à briser.', '', '', '', '', '', '', '', ''),
(69465, 7, 0, '', 'Ennuyé. Vous mourrez maintenant.', '', '', '', '', '', '', '', ''),
(69465, 8, 0, '', 'Comment vous m''avez battu... ?', '', '', '', '', '', '', '', '');

UPDATE creature_template SET InhabitType = 4 WHERE entry = 69467;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
EXP = 4,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
dmg_multiplier = 2,
baseattacktime = 2000,
mechanic_immune_mask = 617299839,
ScriptName = 'boss_jin_rokh_the_breaker',
VehicleId = 2703
WHERE entry = 69465;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
modelid1 = 47698,
modelid2 = 0,
ScriptName = 'mob_focused_lightning'
WHERE entry = 69593;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
modelid1 = 39633,
modelid2 = 0,
ScriptName = 'mob_lightning_fissure'
WHERE entry = 69609;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
unit_flags = 0x02|0x04|0x2000000,
flags_extra = 0x80,
ScriptName = "mob_statue"
WHERE entry = 69467;

UPDATE creature_template SET
modelid1 = 11686,
modelid2 = 0,
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
unit_flags = 0x02|0x04|0x2000000,
ScriptName = "mob_conductive_water"
WHERE entry = 69469;

UPDATE creature_template SET
modelid1 = 39633,
modelid2 = 0,
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
unit_flags = 0x02|0x04|0x2000000,
ScriptName = "mob_call_da_storm_stalker"
WHERE entry = 69676;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_lightning_spark'
WHERE entry = 69635;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
modelid1 = 11686,
modelid2 = 0,
ScriptName = 'mob_lightning_pillar_stalker'
WHERE entry = 69813;