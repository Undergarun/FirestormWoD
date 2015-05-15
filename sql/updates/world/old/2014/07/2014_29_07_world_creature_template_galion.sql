UPDATE creature_template SET
dmg_multiplier = 10,
baseattacktime = 1500,
mechanic_immune_mask = 617299839
WHERE entry = 62346;

UPDATE creature_model_info SET
bounding_radius = 1.5,
combat_reach = 17
WHERE modelid = 42439;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
unit_flags = 0,
dmg_multiplier = 3,
baseattacktime = 1500,
ScriptName = 'npc_salyin_warmonger'
WHERE entry = 62351;

UPDATE creature_template SET
ScriptName = 'npc_chief_salyis'
WHERE entry = 62352;

DELETE FROM spell_script_names WHERE spell_id = 121754;
INSERT INTO spell_script_names VALUE (121754, 'spell_impaling_pull');

DELETE FROM vehicle_template_accessory WHERE entry = 62346;
INSERT INTO vehicle_template_accessory VALUES
(62346, 62352, 2, 1, 'Chief Salyis', 8, 0);

DELETE FROM creature_text WHERE entry = 62352;
INSERT INTO creature_text VALUES
(62352, 0, 0, 'Loot and pillage, bwahahaha!', 14, 0, 100, 0, 0, 32500, 'GALLEON_SPAWN'),
(62352, 1, 0, 'Bring me their corpses!', 14, 0, 100, 0, 0, 32498, 'GALLEON_AGGRO'),
(62352, 2, 0, 'Arm the cannons! Ready! Aim! FIRE!', 14, 0, 100, 0, 0, 32502, 'GALLEON_CANON'),
(62352, 3, 0, 'They are soft, weak!', 14, 0, 100, 0, 0, 32501, 'GALLEON_SLAY'),
(62352, 4, 0, 'Retreat to the hills!', 14, 0, 100, 0, 0, 32499, 'GALLEON_DEATH');

DELETE FROM locales_creature_text WHERE entry = 62352;
INSERT INTO locales_creature_text VALUES
(62352, 0, 0, '', 'Pillages et rapines ! Mouahahah !', '', '', '', '¡Saqueo y vandalismo! ¡Jajaja!', '', '', '', ''),
(62352, 1, 0, '', 'Apportez-moi leur cadavre !', '', '', '', '¡Traedme sus cuerpos!', '', '', '', ''),
(62352, 2, 0, '', 'Armez les canons ! Prêt ! En joue ! FEU !', '', '', '', '¡Recarguen los cañones! ¡Listos! ¡Apunten! ¡FUEGO!', '', '', '', ''),
(62352, 3, 0, '', 'Ils sont mous, faibles !', '', '', '', '¡Ellos son flojos! ¡Débiles!', '', '', '', ''),
(62352, 4, 0, '', 'En arrière dans les collines !', '', '', '', '¡Retirada a las colinas!', '', '', '', '');