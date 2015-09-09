SET NAMES utf8;

DELETE FROM instance_template WHERE map = 1205;
INSERT INTO instance_template VALUE (1205, 1116, 'instance_blackrock_foundry', 0);

DELETE FROM access_requirement WHERE mapId = 1205;
INSERT INTO access_requirement (mapId, difficulty, level_min, level_max, itemlevel_min, comment) VALUES
(1205, 14, 100, 100, 635, 'Blackrock Foundry - Normal'),
(1205, 15, 100, 100, 635, 'Blackrock Foundry - Heroic'),
(1205, 16, 101, 101, 635, 'Blackrock Foundry - Mythic'),
(1205, 17, 101, 101, 635, 'Blackrock Foundry - RaidTools');

DELETE FROM areatrigger_teleport WHERE id IN (10241, 10242);
INSERT INTO areatrigger_teleport VALUE
(10241, 'Blackrock Foundry - Entrance', 1205, 131.171875, 3429.481, 319.8291, 0.00135),
(10242, 'Blackrock Foundry - Exit Target', 1116, 8074.5845, 859.86652, 34.3671, 2.8979);

DELETE FROM spell_script_names WHERE spell_id IN (175624, 175644, 175752, 175088, 169402, 155534, 155080, 173191, 155323, 155301);
INSERT INTO spell_script_names VALUES
(175624, 'spell_foundry_grievous_mortal_wounds'),
(175644, 'spell_foundry_spinning_blade'),
(175752, 'spell_foundry_slag_breath'),
(175088, 'spell_foundry_animate_slag'),
(169402, 'spell_foundry_gronnling_smash'),
(155534, 'spell_foundry_rage_regeneration'),
(155080, 'spell_foundry_inferno_slice'),
(173191, 'spell_foundry_cave_in'),
(155323, 'spell_foundry_petrifying_slam'),
(155301, 'spell_foundry_overhead_smash');

DELETE FROM areatrigger_template WHERE spell_id IN (175644);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, scale_x, scale_y, flags) VALUES
(175644, 0, 8034, 2, 5, 5, 0x4004);

DELETE FROM conditions WHERE SourceEntry IN (175088);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 175088, 31, 0, 3, 87595);

SET @REF_GRUUL = 76877;

DELETE FROM creature_text WHERE entry IN (87771, @REF_GRUUL);
INSERT INTO creature_text VALUES
(87771, 0, 0, 'Bite him! Bite him!', 12, 0, 100, 0, 0, 0, 'SlagshopWorker1'),
(87771, 0, 1, 'Come on Brute!', 12, 0, 100, 0, 0, 0, 'SlagshopWorker2'),
(87771, 0, 2, 'Hey! That one no fight fair!', 12, 0, 100, 0, 0, 0, 'SlagshopWorker3'),
(87771, 0, 3, 'I got gold on the big guy!', 12, 0, 100, 0, 0, 0, 'SlagshopWorker4'),
(87771, 0, 4, 'OUCH! That had to hurt!', 12, 0, 100, 0, 0, 0, 'SlagshopWorker5'),

(@REF_GRUUL, 0, 0, 'Gruul smell flesh...', 14, 0, 100, 0, 0, 45033, 'GruulIntro'),
(@REF_GRUUL, 1, 0, 'Hahahaha, Gruul kill you so easy!', 14, 0, 100, 0, 0, 45030, 'GruulAggro'),
(@REF_GRUUL, 2, 0, 'You stay...', 14, 0, 100, 0, 0, 45036, 'GruulPetrify1'),
(@REF_GRUUL, 2, 1, 'No escape!', 14, 0, 100, 0, 0, 45038, 'GruulPetrify2'),
(@REF_GRUUL, 2, 2, 'Scurry...', 14, 0, 100, 0, 0, 45039, 'GruulPetrify3'),
(@REF_GRUUL, 3, 0, 'GRUUL SMASH!', 14, 0, 100, 0, 0, 45037, 'GruulDestructiveRampage'),
(@REF_GRUUL, 4, 0, 'GRUUL CRUSH YOU NOW!!!', 14, 0, 100, 0, 0, 45031, 'GruulBerserk'),
(@REF_GRUUL, 5, 0, 'Unworthy.', 14, 0, 100, 0, 0, 45034, 'GruulSlay1'),
(@REF_GRUUL, 5, 1, 'No more.', 14, 0, 100, 0, 0, 45035, 'GruulSlay2'),
(@REF_GRUUL, 6, 0, 'Gruul... Fall... Down?', 14, 0, 100, 0, 0, 45032, 'GruulDeath'),
(@REF_GRUUL, 7, 0, '%s goes into a |cFFFF0404|Hspell:155539|h[Destructive Rampage]|h|r!', 41, 0, 100, 0, 0, 0, 'GruulDestructiveRampageStart'),
(@REF_GRUUL, 8, 0, '%s''s |cFFFF0404|Hspell:155539|h[Destructive Rampage]|h|r ends.', 41, 0, 100, 0, 0, 0, 'GruulDestructiveRampageEnd');

DELETE FROM locales_creature_text WHERE entry IN (87771, @REF_GRUUL);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    87771, 0, 0,
    'Bouffe-le ! Bouffe-le !',
    'Beißt ihn! Beißt ihn!',
    '¡Muérdelo! ¡Muérdelo!',
    'Кусай! Кусай!'
),
(
    87771, 0, 1,
    'Allez, la brute !',
    'Na los, Schläger!',
    '¡Vamos bruto!',
    'Давай же, громила!'
),
(
    87771, 0, 2,
    'Hé ! C''est pas réglo, comme manière de se battre, ça !',
    'He! Das ist kein fairer Kampf!',
    '¡Oye! ¡Ese no pelear limpio!',
    'Эй, так нечестно!'
),
(
    87771, 0, 3,
    'J''ai parié de l''or sur le gros balaise !',
    'Ich habe Gold auf den Großen gesetzt!',
    '¡Yo le aposte al grandulón!',
    'Я поставил на того здоровяка!'
),
(
    87771, 0, 4,
    'OUILLE ! Ça devait faire mal !',
    'AUTSCH! Das hat bestimmt wehgetan!',
    '¡AUCH! ¡Eso debió doler!',
    'АЙ! Это, наверное, больно!'
),
(
    @REF_GRUUL, 0, 0,
    'Gruul sentir chair fraîche...',
    'Gruul riechen Fleisch...',
    'Gruul huele carne...',
    'Груул чует плоть...'
),
(
    @REF_GRUUL, 1, 0,
    'Ha ha ha ha ! Tuer vous, facile pour Gruul !',
    'Hahahaha, Gruul euch ganz leicht töten!',
    'Ja, ja, ja, ¡Gruul matarlos muy fácil!',
    'Ха-ха-ха, Груул вас убивать!'
),
(
    @REF_GRUUL, 2, 0,
    'Pas bouger !',
    'Ihr bleiben...',
    'Ustedes quedarse...',
    'Постойте тут...'
),
(
    @REF_GRUUL, 2, 1,
    'Fuir, pas possible !',
    'Keiner entkommen!',
    '¡No escaparse!',
    'Выхода нет!'
),
(
    @REF_GRUUL, 2, 2,
    'Courir...',
    'Husch...',
    'Corran...',
    'Прочь...'
),
(
    @REF_GRUUL, 3, 0,
    'GRUUL TAPER !',
    'GRUUL MACHT PLATT!',
    '¡GRUUL MACHACA!',
    'ГРУУЛ БИТЬ!'
),
(
    @REF_GRUUL, 4, 0,
    'GRUUL ÉCRASER VOUS MAINTENANT !',
    'GRUUL ZERMATSCHEN JETZT!',
    '¡¡¡GRUUL MACHACARLOS AHORA!!!',
    'ГРУУЛ КРУШИТЬ!!!'
),
(
    @REF_GRUUL, 5, 0,
    'Indigne.',
    'Unwürdig.',
    'Indigno.',
    'Ничтожество.'
),
(
    @REF_GRUUL, 5, 1,
    'Disparu !',
    'Weg!',
    'No más.',
    'Хватит.'
),
(
    @REF_GRUUL, 6, 0,
    'Gruul... tombé ?',
    'Gruul... fällt?',
    'Gruul... ¿caer?',
    'Груул... упал?'
),
(
    @REF_GRUUL, 7, 0,
    '%s déclenche un |cFFFF0404|Hspell:155539|h[Saccage destructeur]|h|r !',
    '%s steigert sich in einen |cFFFF0404|Hspell:155539|h[zerstörerischen Amoklauf]|h|r!',
    '¡%s entra en modo |cFFFF0404|Hspell:155539|h[Desenfreno destructivo]|h|r!',
    '%s впадает в |cFFFF0404|Hspell:155539|h[Разрушительное буйство]|h|r!'
),
(
    @REF_GRUUL, 8, 0,
    'Le |cFFFF0404|Hspell:155539|h[Saccage destructeur]|h|r |2 %s prend fin.',
    'Der |cFFFF0404|Hspell:155539|h[zerstörerische Amoklauf]|h|r von %s endet.',
    'El |cFFFF0404|Hspell:155539|h[Desenfreno destructivo]|h|r de %s acaba.',
    '|cFFFF0404|Hspell:155539|h[Разрушительное буйство]|h|r |3-1(%s) заканчивается.'
);

DELETE FROM `reference_loot_template` WHERE entry = @REF_GRUUL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GRUUL, 113848, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113849, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113850, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113851, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113852, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113853, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113854, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113855, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113856, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113857, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113858, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113859, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113860, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113861, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_GRUUL WHERE `entry`= @REF_GRUUL;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_GRUUL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GRUUL, 1, 100, 1, 0, -@REF_GRUUL, 6);