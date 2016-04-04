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

DELETE FROM areatrigger_scripts WHERE entry IN (10276, 10277);
INSERT INTO areatrigger_scripts VALUES
(10276, 'areatrigger_at_foundry_first_floor_trap'),
(10277, 'areatrigger_at_foundry_second_floor_trap');

DELETE FROM spell_script_names WHERE spell_id IN (175624, 175644, 175752, 175088, 169402, 155534, 155080, 173191, 155326, 155323, 155301);
INSERT INTO spell_script_names VALUES
(175624, 'spell_foundry_grievous_mortal_wounds'),
(175644, 'spell_foundry_spinning_blade'),
(175752, 'spell_foundry_slag_breath'),
(175088, 'spell_foundry_animate_slag'),
(169402, 'spell_foundry_gronnling_smash'),
(155534, 'spell_foundry_rage_regeneration'),
(155080, 'spell_foundry_inferno_slice'),
(173191, 'spell_foundry_cave_in'),
(155326, 'spell_foundry_petrifying_slam_aoe'),
(155323, 'spell_foundry_petrifying_slam'),
(155301, 'spell_foundry_overhead_smash');

DELETE FROM areatrigger_template WHERE spell_id IN (175644);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, scale_x, scale_y, flags) VALUES
(175644, 0, 8034, 2, 5, 5, 0x4004);

DELETE FROM conditions WHERE SourceEntry IN (175088);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 175088, 31, 0, 3, 87595);

UPDATE creature_template SET modelid1 = 38795, modelid2 = 0, ScriptName = 'npc_foundry_spinning_blade' WHERE entry = 88008;
UPDATE creature_template SET minlevel = 101, maxlevel = 101, faction = 14, ScriptName = 'npc_foundry_living_flames' WHERE entry = 87993;

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
(@REF_GRUUL, 113862, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113863, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113864, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113865, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113866, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113867, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113868, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113869, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113870, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113871, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113872, 0, 1, 1, 1, 1),
(@REF_GRUUL, 113873, 0, 1, 1, 1, 1),
(@REF_GRUUL, 118114, 0, 1, 1, 1, 1),
(@REF_GRUUL, 120078, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_GRUUL WHERE `entry`= @REF_GRUUL;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_GRUUL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GRUUL, 1, 100, 1, 0, -@REF_GRUUL, 6);

DELETE FROM creature_groupsizestats WHERE entry = 76877;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
-- Normal mode
(76877,14,10,43336804),
(76877,14,11,47888479),
(76877,14,12,52434322),
(76877,14,13,56988341),
(76877,14,14,61535107),
(76877,14,15,66085165),
(76877,14,16,70631315),
(76877,14,17,75197436),
(76877,14,18,79781563),
(76877,14,19,84316512),
(76877,14,20,88867910),
(76877,14,21,93383937),
(76877,14,22,97959953),
(76877,14,23,102481581),
(76877,14,24,107078356),
(76877,14,25,111589662),
(76877,14,26,116140648),
(76877,14,27,120689288),
(76877,14,28,125267562),
(76877,14,29,129788681),
(76877,14,30,134333535),
-- Heroic mode
(76877,15,10,57427693),
(76877,15,11,63454603),
(76877,15,12,69493876),
(76877,15,13,75510546),
(76877,15,14,81537803),
(76877,15,15,87561581),
(76877,15,16,93601901),
(76877,15,17,99613554),
(76877,15,18,105656195),
(76877,15,19,111698190),
(76877,15,20,117723770),
(76877,15,21,123757626),
(76877,15,22,129775912),
(76877,15,23,135810540),
(76877,15,24,141835129),
(76877,15,25,147847598),
(76877,15,26,153889168),
(76877,15,27,159915494),
(76877,15,28,165947324),
(76877,15,29,171955593),
(76877,15,30,177996990),
-- LFR
(76877,17,16,53005134),
(76877,17,17,56846435),
(76877,17,18,59798358),
(76877,17,19,63831589),
(76877,17,20,66627787),
(76877,17,21,70040736),
(76877,17,22,73465202),
(76877,17,23,76860343),
(76877,17,24,80282107),
(76877,17,25,83683912);