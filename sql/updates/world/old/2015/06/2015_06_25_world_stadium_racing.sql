SET NAMES utf8;

DELETE FROM gameobject_template WHERE entry IN (239132, 239133);
INSERT INTO gameobject_template (entry, TYPE, displayId, NAME, size, data3, data7, BuildVerified) VALUES
(239132, 5, 11182, 'Racing Flag', 0.75, 1, 1, '20173'),
(239133, 5, 5913, 'Horde Racing Flag', 1.3, 1, 1, '20173');

DELETE FROM gameobject WHERE id IN (239131, 239132, 239133);

# Speedy Horde Racer
UPDATE creature_template SET faction = 83, VehicleId = 3714, ScriptName = 'npc_ashran_speedy_horde_racer' WHERE entry = 82903;
# Horde Rider
UPDATE creature_template SET unit_flags = 32768, unit_flags2 = 67635200 WHERE entry = 82864;

# Ex Alliance Racer
UPDATE creature_template SET faction = 12, VehicleId = 3715, ScriptName = 'npc_ashran_ex_alliance_racer' WHERE entry = 82884;
# Alliance Rider
UPDATE creature_template SET unit_flags = 33536, unit_flags2 = 71829504 WHERE entry = 82870;

UPDATE creature_template SET minlevel = 100, maxlevel = 100, unit_flags = 33536, unit_flags2 = 4196352, speed_walk = 0.8, speed_run = 0.2857143 WHERE entry IN (82884, 82903);

DELETE FROM trinity_string WHERE entry IN (14071, 14072);
INSERT INTO trinity_string (entry, content_default, content_loc2, content_loc3, content_loc6, content_loc8) VALUE
(
    14071,
    '|c00FFFF00|Hspell:178022|h[Event: Stadium Racing]|h|r begins at the Amphitheater of Annihilation in 3 minutes.',
    'L''|c00FFFF00|Hspell:178022|h[Évènement : Course en stade]|h|r commence dans l''Amphithéâtre de l''Annihilation dans 3 minutes.',
    '|c00FFFF00|Hspell:178022|h[Ereignis: Stadionrennen]|h|r beginnt in 3 Minuten am Amphitheater der Auslöschung.',
    'El |c00FFFF00|Hspell:178022|h[Evento: Carrera en el estadio]|h|r comienza en el Anfiteatro de Aniquilación en 3 minutos.',
    '|c00FFFF00|Hspell:178022|h[Событие: Кольцевые гонки]|h|r начнется в Амфитеатре истребления через 3 минуты.'
),
(
    14072,
    '|c00FFFF00|Hspell:178022|h[Event: Stadium Racing]|h|r has begun at the Amphiteater of Annihilation.',
    'L''|c00FFFF00|Hspell:178022|h[Évènement : Course en stade]|h|r a commencé dans l''Amphithéâtre de l''Annihilation.',
    '|c00FFFF00|Hspell:178022|h[Ereignis: Stadionrennen]|h|r begann am Amphitheater der Auslöschung.',
    'El |c00FFFF00|Hspell:178022|h[Evento: Carrera en el estadio]|h|r comienzó en el Anfiteatro de Aniquilación.',
    '|c00FFFF00|Hspell:178022|h[Событие: Кольцевые гонки]|h|r начал в Амфитеатре истребления.'
);

DELETE FROM creature_text WHERE entry = 84113 AND groupid > 5;
INSERT INTO creature_text VALUES
(84113, 6, 0, 'The Alliance is victorious!', 41, 0 ,100, 0, 0, 0, 'AllianceVictorious'),
(84113, 7, 0, 'The Horde is victorious!', 41, 0, 100, 0, 0, 0, 'HordeVictorious');

DELETE FROM locales_creature_text WHERE entry = 84113 AND textGroup > 5;
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    84113, 6, 0,
    'L''Alliance gagne !',
    'Die Allianz siegt!',
    '¡Victoria para la Alianza!',
    'Альянс одержал победу!'
),
(
    84113, 7, 0,
    'La Horde gagne !',
    'Die Horde siegt!',
    '¡Victoria para la Horda!',
    'Победа за Ордой!'
);

DELETE FROM spell_script_names WHERE ScriptName = 'spell_ashran_horde_and_alliance_racer';
INSERT INTO spell_script_names VALUE
(166819, 'spell_ashran_horde_and_alliance_racer'),
(166784, 'spell_ashran_horde_and_alliance_racer');