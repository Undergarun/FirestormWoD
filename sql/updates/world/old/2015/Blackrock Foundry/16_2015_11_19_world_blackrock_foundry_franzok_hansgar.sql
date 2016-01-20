SET NAMES utf8;

DELETE FROM areatrigger_template WHERE spell_id = 160259;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, scale_x, scale_y, flags, ScriptName) VALUES
(160259, 0, 6742, 2, 12, 12, 16384, 'areatrigger_foundry_fire_bomb');

DELETE FROM `areatrigger_template` WHERE spell_id = 159182;
INSERT INTO `areatrigger_template` (`spell_id`, `eff_index`, `entry`, `type`, `scale_x`, `scale_y`, `flags`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`) VALUES
(159182, 0, 6766, 1, 1, 1, 2048, 19, 7.5, 3, 19, 7.5, 3, 0, 0, '');

DELETE FROM spell_script_names WHERE spell_id IN (160092, 155665, 156938, 156892);
INSERT INTO spell_script_names VALUE
(160092, 'spell_foundry_rending_slash'),
(155665, 'spell_foundry_pumped_up'),
(156938, 'spell_foundry_crippling_suplex'),
(156892, 'spell_foundry_body_slam_red_arrow');

DELETE FROM areatrigger_scripts WHERE entry = 9998;
INSERT INTO areatrigger_scripts VALUE
(9998, 'areatrigger_at_foundry_hansgar_and_franzok_entrance');

DELETE FROM conditions WHERE SourceEntry IN (177379, 156611, 156613);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 177379, 31, 0, 3, 237598),
(13, 1, 177379, 31, 0, 3, 237597),
(13, 1, 156611, 31, 0, 3, 76974),
(13, 1, 156613, 31, 0, 3, 76973);

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
    'Dis donc, Franz... Franzok ! La machine est coincée !',
    'Hey Franz? Franzok!! Das Ding steckt fest!',
    'Ey, Franz. ¡¡Franzok!! ¡Esto está atascado!',
    'Франц? Франзок! Конвейер встал!'
),
(
    @REF_HANSGAR, 1, 0,
    'Jamais de la vie ! Je l''ai déjà décoincée la dernière fois !',
    'Ich reparier''s nich''! Ich war das letzte Mal dran!',
    '¡No voy a desatascarlo! ¡Ya lo desatasqué la última vez!',
    'Я уже в прошлый раз все чинил! Хватит!'
),
(
    @REF_HANSGAR, 2, 0,
    'Hé, vous ! Rentrez là-dedans et décoincez-moi cette presse !',
    'Ey, du! Spuck in die Hände und reparier den Stanzer!',
    '¡Oye, tú! ¡Ven y desatasca la estampadora!',
    'Эй, вы! Быстро сюда, почините эту штуку!'
),
(
    @REF_HANSGAR, 3, 0,
    'Je vais vous ÉCRASER !',
    'Ich mach Euch PLATT!',
    '¡Voy a ELIMINARTE!',
    'Ну все, вам крышка!'
),
(
    @REF_HANSGAR, 4, 0,
    'Arrêtez de vous cogner !',
    'Hört auf, Euch selbst zu hau''n!',
    '¡Dejen de pegarse a sí mismos!',
    'Хватит лупить себя!'
),
(
    @REF_HANSGAR, 5, 0,
    'Je reviendrai !',
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
    'Aarrgh ! Javais pas encore fini...',
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
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n est sur le point de se faire toucher par un |cFFF00000|Hspell:159646|h[Plaquage]|h|r !',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n ist Ziel von |cFFF00000|Hspell:159646|h[Bodycheck]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t¡$n va a recibir un |cFFF00000|Hspell:159646|h[Embate corporal]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n сейчас получит урон от способности |cFFF00000|Hspell:159646|h[Мощный удар]|h|r!'
),
(
    @REF_FRANZOK, 0, 0,
    'Et alors ? T''as qu''à la décoincer !',
    'Und? Geh''s halt reparieren.',
    '¿Y? ¡Desatáscalo, entonces!',
    'Ну так почини его!'
),
(
    @REF_FRANZOK, 1, 0,
    'Tu veux aller dire à Main-Noire qu''on a un laminoir qui lamine pas ?',
    'Willst du Schwarzfaust sagen gehen, dass wir einen Stanzer haben, der nich'' stanzt?',
    '¿Quieres decirle a Puño Negro que tenemos una estampadora que no estampa?',
    'Рискнешь сказать Черноруку, что нам штамповать нечем?'
),
(
    @REF_FRANZOK, 2, 0,
    'Ha ha ha ! Il y aura des impuretés dans ce lot.',
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
    'Faites chauffer la forge !',
    'Wir pumpen den Ofen in den Turbogang!',
    '¡Eleva la potencia de la forja para conseguir la Ultravelocidad!',
    'Включаем режим перегрева!'
),
(
    @REF_FRANZOK, 5, 0,
    'Ha, ça vous a plu ? Attendez pour voir !!!',
    'Heh, das hat Euch beeindruckt?! Wartet''s nur ab!!',
    'Je, ¡¿piensas que eso estuvo bueno?! ¡Ya verás!',
    'И это, по-твоему, круто? Ну погоди!'
),
(
    @REF_FRANZOK, 6, 0,
    'Si ça saigne, c''est qu''on peut le tuer !',
    'Wenn es blutet, können wir es töten!',
    'Si sangra, ¡podemos matarlo!',
    'Течет кровь - можно убить!'
),
(
    @REF_FRANZOK, 7, 0,
    'Il ne peut en rester que deux, ha ha ha !',
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
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t%s commence à lancer |cFFF00000|Hspell:160838|h[Rugissement perturbant]|h|r !',
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t%s fängt an, |cFFF00000|Hspell:160838|h[Störendes Brüllen]|h|r zu wirken!',
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t¡%s empieza a lanzar |cFFF00000|Hspell:160838|h[Rugido perturbador]|h|r!',
    '|TInterface\\Icons\\ability_garrosh_hellscreams_warsong.blp:20|t%s начинает произносить заклинание |cFFF00000|Hspell:160838|h[Разрушительный рев]|h|r!'
),
(
    @REF_FRANZOK, 10, 0,
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n est sur le point de se faire toucher par un |cFFF00000|Hspell:159646|h[Plaquage]|h|r !',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n ist Ziel von |cFFF00000|Hspell:159646|h[Bodycheck]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t¡$n va a recibir un |cFFF00000|Hspell:159646|h[Embate corporal]|h|r!',
    '|TInterface\\Icons\\ability_hanzandfranz_bodyslam.blp:20|t$n сейчас получит урон от способности |cFFF00000|Hspell:159646|h[Мощный удар]|h|r!'
);

DELETE FROM `reference_loot_template` WHERE entry = @REF_HANSGAR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_HANSGAR, 113897, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113898, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113899, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113900, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113901, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113902, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113903, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113904, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113905, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113906, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113907, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113908, 0, 1, 1, 1, 1),
(@REF_HANSGAR, 113910, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_HANSGAR WHERE `entry`= @REF_HANSGAR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_HANSGAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_HANSGAR, 1, 100, 1, 0, -@REF_HANSGAR, 6);

DELETE FROM creature_groupsizestats WHERE entry = @REF_HANSGAR;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_HANSGAR, 14, 10, 44546433),
(@REF_HANSGAR, 14, 11, 52341265),
(@REF_HANSGAR, 14, 12, 57387012),
(@REF_HANSGAR, 14, 13, 62351978),
(@REF_HANSGAR, 14, 14, 67420906),
(@REF_HANSGAR, 14, 15, 72459420),
(@REF_HANSGAR, 14, 16, 77253273),
(@REF_HANSGAR, 14, 17, 82476427),
(@REF_HANSGAR, 14, 18, 87213765),
(@REF_HANSGAR, 14, 19, 92155771),
(@REF_HANSGAR, 14, 20, 97186865),
(@REF_HANSGAR, 14, 21, 102344375),
(@REF_HANSGAR, 14, 22, 107243697),
(@REF_HANSGAR, 14, 23, 112151337),
(@REF_HANSGAR, 14, 24, 117058978),
(@REF_HANSGAR, 14, 25, 122040170),
(@REF_HANSGAR, 14, 26, 127011613),
(@REF_HANSGAR, 14, 27, 131983056),
(@REF_HANSGAR, 14, 28, 136953493),
(@REF_HANSGAR, 14, 29, 141791015),
(@REF_HANSGAR, 14, 30, 146858223),
(@REF_HANSGAR, 15, 10, 59519829),
(@REF_HANSGAR, 15, 11, 65471812),
(@REF_HANSGAR, 15, 12, 71648823),
(@REF_HANSGAR, 15, 13, 78128438),
(@REF_HANSGAR, 15, 14, 84049982),
(@REF_HANSGAR, 15, 15, 90512674),
(@REF_HANSGAR, 15, 16, 96647702),
(@REF_HANSGAR, 15, 17, 102724383),
(@REF_HANSGAR, 15, 18, 108927534),
(@REF_HANSGAR, 15, 19, 115097227),
(@REF_HANSGAR, 15, 20, 121323830),
(@REF_HANSGAR, 15, 21, 127624752),
(@REF_HANSGAR, 15, 22, 133913077),
(@REF_HANSGAR, 15, 23, 140102914),
(@REF_HANSGAR, 15, 24, 146326447),
(@REF_HANSGAR, 15, 25, 152563759),
(@REF_HANSGAR, 15, 26, 158895085),
(@REF_HANSGAR, 15, 27, 164821538),
(@REF_HANSGAR, 15, 28, 171231418),
(@REF_HANSGAR, 15, 29, 177386238),
(@REF_HANSGAR, 15, 30, 183741897);

DELETE FROM creature_groupsizestats WHERE entry = @REF_FRANZOK;
INSERT INTO creature_groupsizestats(entry, difficulty, groupSize, health) VALUES
(@REF_FRANZOK, 14, 10, 44546433),
(@REF_FRANZOK, 14, 11, 52341265),
(@REF_FRANZOK, 14, 12, 57387012),
(@REF_FRANZOK, 14, 13, 62351978),
(@REF_FRANZOK, 14, 14, 67420906),
(@REF_FRANZOK, 14, 15, 72459420),
(@REF_FRANZOK, 14, 16, 77253273),
(@REF_FRANZOK, 14, 17, 82476427),
(@REF_FRANZOK, 14, 18, 87213765),
(@REF_FRANZOK, 14, 19, 92155771),
(@REF_FRANZOK, 14, 20, 97186865),
(@REF_FRANZOK, 14, 21, 102344375),
(@REF_FRANZOK, 14, 22, 107243697),
(@REF_FRANZOK, 14, 23, 112151337),
(@REF_FRANZOK, 14, 24, 127011613),
(@REF_FRANZOK, 14, 25, 122040170),
(@REF_FRANZOK, 14, 26, 41440618),
(@REF_FRANZOK, 14, 27, 131983056),
(@REF_FRANZOK, 14, 28, 136953493),
(@REF_FRANZOK, 14, 29, 141791015),
(@REF_FRANZOK, 14, 30, 146858223),
(@REF_FRANZOK, 15, 10, 59519829),
(@REF_FRANZOK, 15, 11, 65471812),
(@REF_FRANZOK, 15, 12, 71648823),
(@REF_FRANZOK, 15, 13, 78128438),
(@REF_FRANZOK, 15, 14, 84049982),
(@REF_FRANZOK, 15, 15, 90512674),
(@REF_FRANZOK, 15, 16, 96647702),
(@REF_FRANZOK, 15, 17, 102724383),
(@REF_FRANZOK, 15, 18, 108927534),
(@REF_FRANZOK, 15, 19, 115097227),
(@REF_FRANZOK, 15, 20, 121323830),
(@REF_FRANZOK, 15, 21, 127624752),
(@REF_FRANZOK, 15, 22, 133913077),
(@REF_FRANZOK, 15, 23, 140102914),
(@REF_FRANZOK, 15, 24, 146326447),
(@REF_FRANZOK, 15, 25, 152563759),
(@REF_FRANZOK, 15, 26, 158895085),
(@REF_FRANZOK, 15, 27, 164821538),
(@REF_FRANZOK, 15, 28, 171231418),
(@REF_FRANZOK, 15, 29, 177386238),
(@REF_FRANZOK, 15, 30, 183741897);
