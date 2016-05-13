SET NAMES utf8;

UPDATE creature_template SET minlevel = 102, maxlevel = 102, EXP = 5, unit_flags = unit_flags & ~0x100, Health_mod = 18 WHERE entry = 23682;

DELETE FROM locales_creature_text WHERE entry IN (23682, 23775);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    23682, 0, 0,
    'Lames de lumière !',
    'Klingen des Lichts!',
    '¡Espadas de luz!',
    'Клинки света!'
),
(
    23682, 1, 0,
    'Lumière, donne-moi la force !',
    'Licht, gib mir Kraft!',
    '¡Luz, dame fuerza!',
    'Свет, дай мне силу!'
),
(
    23682, 2, 0,
    '%s devient fou de rage !',
    '%s wird wütend!',
    '¡%s se enfurece!',
    '%s впадает в исступление!'
),
(
    23682, 3, 0,
    'Des infidèles ! Ils doivent être purifiés !',
    'Ungläubige! Sie müssen geläutert werden!',
    '¡Infieles! ¡Hay que purificarlos!',
    'Нечестивцы! Да покарает их Свет!'
),
(
    23682, 4, 0,
    'Je suis là, madame !',
    'Zu Euren Diensten, Mylady!',
    '¡A su lado, mi señora!',
    'Я с вами, миледи!'
),
(
    23775, 0, 0,
    'C''est tout ?',
    'Ist das alles?',
    '¡Ja! ¿Eso es todo?',
    'Это все?'
),
(
    23775, 1, 0,
    'Ça ne vaut rien.',
    'Unwürdig.',
    'Poco digno.',
    'Недостоин.'
);