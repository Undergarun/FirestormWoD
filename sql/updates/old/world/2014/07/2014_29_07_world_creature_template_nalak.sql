UPDATE creature_template SET
mindmg = 15000,
maxdmg = 30000,
EXP = 4,
dmg_multiplier = 4,
baseattacktime = 1500,
ScriptName = 'boss_nalak',
InhabitType = 4,
Health_mod = 700,
mechanic_immune_mask = 617299839
WHERE entry = 69099;

UPDATE creature_model_info SET
bounding_radius = 1.5,
combat_reach = 25
WHERE modelid = 47227;

DELETE FROM spell_script_names WHERE spell_id IN (136350);
INSERT INTO spell_script_names VALUE (136350, 'spell_lightning_tether');

DELETE FROM creature_text WHERE entry = 69099;
INSERT INTO creature_text VALUES
(69099, 0, 0, 'I am born of thunder!', 14, 0, 100, 0, 0, 35732, 'NALAK_INTRO'),
(69099, 1, 0, 'Can you feel a chill wind blow? The storm is coming...', 14, 0, 100, 0, 0, 35730, 'NALAK_AGGRO'),
(69099, 2, 0, 'The clouds arc with vengeance!', 14, 0, 100, 0, 0, 35734, 'NALAK_ARC_NOVA'),
(69099, 3, 0, 'The air crackles with anger!', 14, 0, 100, 0, 0, 35735, 'NALAK_STORMCLOUD'),
(69099, 4, 0, 'The sky weeps for your demise!', 14, 0, 100, 0, 0, 35733, 'NALAK_SLAY'),
(69099, 5, 0, 'I am but...the darkness...before the storm...', 14, 0, 100, 0, 0, 35731, 'NALAK_DEATH');

DELETE FROM locales_creature_text WHERE entry = 69099;
INSERT INTO locales_creature_text VALUES
(69099, 0, 0, '', 'Je suis né du tonnerre !', '', '', '', '¡He nacido del trueno!', '', '', '', ''),
(69099, 1, 0, '', 'Vous sentez ce souffle glacial ? La tempête se rapproche !', '', '', '', '¿Puedes sentir el frío viento soplar? La tormenta se avecina...', '', '', '', ''),
(69099, 2, 0, '', 'Les nuages s''amoncèlent, criant vengeance !', '', '', '', '¡Las nubes se acumulan con venganza!', '', '', '', ''),
(69099, 3, 0, '', 'L''air crépite de colère !', '', '', '', '¡El aire cruje con rabia!', '', '', '', ''),
(69099, 4, 0, '', 'Le cliel pleure votre mort !', '', '', '', '¡El cielo llora tu muerte!', '', '', '', ''),
(69099, 5, 0, '', 'Je ne suis... que les ténèbres... avant la tempête...', '', '', '', 'No soy más que... la oscuridad... antes de la tormenta...', '', '', '', '');