SET NAMES utf8;

DELETE FROM areatrigger_template WHERE spell_id = 160259;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUES
(160259, 0, 6742, 12, 12, 16384, 'areatrigger_foundry_fire_bomb');

-- DELETE FROM areatrigger_template WHERE spell_id IN (155995, 160578);
-- INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, data0, data1, data6) VALUES
-- (155995, 0, 6731, 1, 1, 4096, -10, 7.5, 4),
-- (155995, 0, 6731, 1, 1, 4096, -10, -7.5, 4),
-- (155995, 0, 6731, 1, 1, 4096, 10, -7.5, 4),
-- (155995, 0, 6731, 1, 1, 4096, 10, 7.5, 4),
-- (160578, 0, 6771, 1, 1, 4096, -10, 7.5, 4),
-- (160578, 0, 6771, 1, 1, 4096, -10, -7.5, 4),
-- (160578, 0, 6771, 1, 1, 4096, 10, -7.5, 4),
-- (160578, 0, 6771, 1, 1, 4096, 10, 7.5, 4);

DELETE FROM spell_script_names WHERE spell_id IN (160092, 155665);
INSERT INTO spell_script_names VALUE
(160092, 'spell_foundry_rending_slash'),
(155665, 'spell_foundry_pumped_up');

DELETE FROM areatrigger_scripts WHERE entry = 9998;
INSERT INTO areatrigger_scripts VALUE
(9998, 'areatrigger_at_foundry_hansgar_and_franzok_entrance');

DELETE FROM conditions WHERE SourceEntry IN (177379);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 177379, 31, 0, 3, 237598),
(13, 1, 177379, 31, 0, 3, 237597);

SET @REF_HANSGAR = 76973;
SET @REF_FRANZOK = 76974;

DELETE FROM creature_text WHERE entry IN (@REF_HANSGAR, @REF_FRANZOK);
INSERT INTO creature_text VALUES
(@REF_HANSGAR, 0, 0, 'Hey Franz? Franzok! We got a jam!', 14, 0, 100, 0, 0, 47411, 'HansgarIntro1'),
(@REF_HANSGAR, 1, 0, 'I ain''t un-jammin it! I un-jammed it last time!', 14, 0, 100, 0, 0, 47412, 'HansgarIntro2'),
(@REF_HANSGAR, 2, 0, 'Ey you! Get in dere and un-jam that stamper!', 14, 0, 100, 0, 0, 47414, 'HansgarIntro3'),
(@REF_HANSGAR, 3, 0, 'I''m going to take you DOWN!', 14, 0, 100, 0, 0, 47409, 'HansgarAggro'),
(@REF_HANSGAR, 4, 0, 'Ha! Quit... hitting yourselves!', 14, 0, 100, 0, 0, 47416, 'HansgarCripplingSuplex'),
(@REF_HANSGAR, 5, 0, 'I''ll be back!', 14, 0, 100, 0, 0, 47418, 'HansgarActivateAssemblyLine'),
(@REF_HANSGAR, 6, 0, 'Wait till they get a load of me.', 14, 0, 100, 0, 0, 47419, 'HansgarReturningFromControls'),
(@REF_HANSGAR, 7, 0, 'No, Aaagh! I wasn''t done yet!', 14, 0, 100, 0, 0, 47415, 'HansgarSlay'),
(@REF_HANSGAR, 8, 0, 'No, not like this...', 14, 0, 100, 0, 0, 47410, 'HansgarDeath'),
(@REF_HANSGAR, 9, 0, '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n is going to be hit by |cFFF00000|Hspell:159646|h[Body Slam]|h|r!', 14, 0, 100, 0, 0, 47410, 'HansgarBodySlam'),

(@REF_FRANZOK, 0, 0, 'So? Go un-jam it den!', 14, 0, 100, 0, 0, 47400, 'FranzokIntro1'),
(@REF_FRANZOK, 1, 0, 'You wanna tell Blackhand we got a stamper that ain''t stampin''?', 14, 0, 100, 0, 0, 47401, 'FranzokIntro2'),
(@REF_FRANZOK, 2, 0, 'Ha ha ha! Guess dis batch will have some impurities.', 14, 0, 100, 0, 0, 47402, 'FranzokIntro3'),
(@REF_FRANZOK, 3, 0, 'Your bones will be crushed to dust!', 14, 0, 100, 0, 0, 47398, 'FranzokAggro'),
(@REF_FRANZOK, 4, 0, 'Pump the forge up to overdrive!', 14, 0, 100, 0, 0, 47406, 'FranzokActivateAssemblyLine'),
(@REF_FRANZOK, 5, 0, 'Hah, you think that was good? You just wait!', 14, 0, 100, 0, 0, 47408, 'FranzokReturningFromControls'),
(@REF_FRANZOK, 6, 0, 'If it bleeds, we can kill it!', 14, 0, 100, 0, 0, 47404, 'FranzokSlay'),
(@REF_FRANZOK, 7, 0, 'There can only be... two! Ha ha ha!', 14, 0, 100, 0, 0, 47407, 'FranzokWipe'),
(@REF_FRANZOK, 8, 0, 'Hear me now... and believe me... later...', 14, 0, 100, 0, 0, 47399, 'FranzokDeath'),
(@REF_FRANZOK, 9, 0, '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t%s begins to cast |cFFF00000|Hspell:160838|h[Disrupting Roar]|h|r!', 41, 0, 100, 0, 0, 0, 'FranzokDisruptingRoar'),
(@REF_FRANZOK, 10, 0, '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n is going to be hit by |cFFF00000|Hspell:159646|h[Body Slam]|h|r!', 41, 0, 100, 0, 0, 0, 'FranzokBodySlam');

DELETE FROM locales_creature_text WHERE entry IN (@REF_HANSGAR, @REF_FRANZOK);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_HANSGAR, 0, 0,
    'Dis donc, Franz... Franzok ! La machine est coincée !',
    'Hey Franz? Franzok!! Das Ding steckt fest!',
    'Ey, Franz. ¡¡Franzok!! ¡Esto está atascado!',
    'Франц? Франзок! Конвейер встал!'
),
(
    @REF_HANSGAR, 1, 0,
    'Jamais de la vie ! Je l''ai déjà décoincée la dernière fois !',
    'Ich reparier''s nich''! Ich war das letzte Mal dran!',
    '¡No voy a desatascarlo! ¡Ya lo desatasqué la última vez!',
    'Я уже в прошлый раз все чинил! Хватит!'
),
(
    @REF_HANSGAR, 2, 0,
    'Hé, vous ! Rentrez là-dedans et décoincez-moi cette presse !',
    'Ey, du! Spuck in die Hände und reparier den Stanzer!',
    '¡Oye, tú! ¡Ven y desatasca la estampadora!',
    'Эй, вы! Быстро сюда, почините эту штуку!'
),
(
    @REF_HANSGAR, 3, 0,
    'Je vais vous ÉCRASER !',
    'Ich mach Euch PLATT!',
    '¡Voy a ELIMINARTE!',
    'Ну все, вам крышка!'
),
(
    @REF_HANSGAR, 4, 0,
    'Arrêtez de vous cogner !',
    'Hört auf, Euch selbst zu hau''n!',
    '¡Dejen de pegarse a sí mismos!',
    'Хватит лупить себя!'
),
(
    @REF_HANSGAR, 5, 0,
    'Je reviendrai !',
    'Ich komme wieder!',
    '¡Volveré!',
    'Айл би бэк!'
),
(
    @REF_HANSGAR, 6, 0,
    'Attends un peu que je m''occupe d''eux.',
    'Wartet, bis Ihr ''ne Portion Hans''gar abkriegt.',
    'Esperen a que me conozcan a mí.',
    'Они еще меня не видели.'
),
(
    @REF_HANSGAR, 7, 0,
    'Aarrgh ! Javais pas encore fini...',
    'Uups! Ich war noch nich'' fertig...',
    'Aún no había terminado...',
    'Гр-р-р! Я не закончил...'
),
(
    @REF_HANSGAR, 8, 0,
    'Non, pas comme ça...',
    'Nein, nich'' so...',
    'No, no así...',
    'Нет, не хочу так...'
),
(
    @REF_HANSGAR, 9, 0,
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n est sur le point de se faire toucher par un |cFFF00000|Hspell:159646|h[Plaquage]|h|r !',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n ist Ziel von |cFFF00000|Hspell:159646|h[Bodycheck]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t¡$n va a recibir un |cFFF00000|Hspell:159646|h[Embate corporal]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n сейчас получит урон от способности |cFFF00000|Hspell:159646|h[Мощный удар]|h|r!'
),
(
    @REF_FRANZOK, 0, 0,
    'Et alors ? T''as qu''à la décoincer !',
    'Und? Geh''s halt reparieren.',
    '¿Y? ¡Desatáscalo, entonces!',
    'Ну так почини его!'
),
(
    @REF_FRANZOK, 1, 0,
    'Tu veux aller dire à Main-Noire qu''on a un laminoir qui lamine pas ?',
    'Willst du Schwarzfaust sagen gehen, dass wir einen Stanzer haben, der nich'' stanzt?',
    '¿Quieres decirle a Puño Negro que tenemos una estampadora que no estampa?',
    'Рискнешь сказать Черноруку, что нам штамповать нечем?'
),
(
    @REF_FRANZOK, 2, 0,
    'Ha ha ha ! Il y aura des impuretés dans ce lot.',
    'Ha ha ha! Diese Fuhre wird wohl ein paar Unreinheiten haben.',
    '¡Ja ja ja! Creo que este lote tiene algunas impurezas.',
    'Ха-ха-ха! Эта партия выйдет с дефектами.'
),
(
    @REF_FRANZOK, 3, 0,
    'Vos os seront réduits en poudre !',
    'Eure Knochen mahl ich zu Staub.',
    'Tus huesos serán convertidos en polvo.',
    'Сотрем ваши кости в порошок.'
),
(
    @REF_FRANZOK, 4, 0,
    'Faites chauffer la forge !',
    'Wir pumpen den Ofen in den Turbogang!',
    '¡Eleva la potencia de la forja para conseguir la Ultravelocidad!',
    'Включаем режим перегрева!'
),
(
    @REF_FRANZOK, 5, 0,
    'Ha, ça vous a plu ? Attendez pour voir !!!',
    'Heh, das hat Euch beeindruckt?! Wartet''s nur ab!!',
    'Je, ¡¿piensas que eso estuvo bueno?! ¡Ya verás!',
    'И это, по-твоему, круто? Ну погоди!'
),
(
    @REF_FRANZOK, 6, 0,
    'Si ça saigne, c''est qu''on peut le tuer !',
    'Wenn es blutet, können wir es töten!',
    'Si sangra, ¡podemos matarlo!',
    'Течет кровь - можно убить!'
),
(
    @REF_FRANZOK, 7, 0,
    'Il ne peut en rester que deux, ha ha ha !',
    'Es kann nur zwei geben, ah hahaha!',
    '¡Solo puede haber dos, jajaja!',
    'Останутся лишь двое! Ха-ха-ха!'
),
(
    @REF_FRANZOK, 8, 0,
    'Écoutez-moi maintenant... Croyez-moi... plus tard...',
    'Hört jetzt zu... und glaubt mir... später...',
    'Escúchame ahora... créeme... después...',
    'А я... ведь... предупреждал...'
),
(
    @REF_FRANZOK, 9, 0,
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t%s commence à lancer |cFFF00000|Hspell:160838|h[Rugissement perturbant]|h|r !',
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t%s fängt an, |cFFF00000|Hspell:160838|h[Störendes Brüllen]|h|r zu wirken!',
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t¡%s empieza a lanzar |cFFF00000|Hspell:160838|h[Rugido perturbador]|h|r!',
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t%s начинает произносить заклинание |cFFF00000|Hspell:160838|h[Разрушительный рев]|h|r!'
),
(
    @REF_FRANZOK, 10, 0,
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n est sur le point de se faire toucher par un |cFFF00000|Hspell:159646|h[Plaquage]|h|r !',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n ist Ziel von |cFFF00000|Hspell:159646|h[Bodycheck]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t¡$n va a recibir un |cFFF00000|Hspell:159646|h[Embate corporal]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n сейчас получит урон от способности |cFFF00000|Hspell:159646|h[Мощный удар]|h|r!'
);