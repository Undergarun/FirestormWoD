SET NAMES utf8;

DELETE FROM spell_script_names WHERE spell_id IN (156704, 162348, 157056, 161923, 157059);
INSERT INTO spell_script_names VALUES
(156704, "spell_foundry_slam"),
(162348, "spell_foundry_fists_of_stone"),
(157056, "spell_foundry_rune_of_grasping_earth"),
(161923, "spell_foundry_rune_of_crushing_earth"),
(157059, "spell_foundry_rune_of_grasping_earth_dot");

DELETE FROM areatrigger_template WHERE spell_id IN (161437, 157246);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `move_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `ScriptName`) VALUES
(161437, 0, 6493, 1, 1, 2570, 762, 4000, 6.5, 7.5, 2, 15, 7.5, "areatrigger_foundry_rippling_smash"),
(157246, 0, 6489, 1, 1, 8196, 0, 2.5, 0.5, 8, 8, 2, 2, "areatrigger_foundry_reverberations");

DELETE FROM conditions WHERE SourceEntry IN (157056);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 157056, 31, 0, 3, 77893);

SET @REF_KROMOG = 77692;
SET @REF_COSMETIC_BLACKHAND = 76831;

DELETE FROM creature_text WHERE entry = @REF_KROMOG;
INSERT INTO creature_text VALUES
(@REF_KROMOG, 0, 0, "Oggum dror undro dukkar.", 14, 0, 100, 0, 0, 42903, "Aggro"),
(@REF_KROMOG, 1, 0, "DESTRUCTION!", 14, 0, 100, 0, 0, 42909, "Ability1"),
(@REF_KROMOG, 1, 1, "LANDSLIDE!", 14, 0, 100, 0, 0, 42910, "Ability2"),
(@REF_KROMOG, 1, 2, "Bury you!", 14, 0, 100, 0, 0, 42911, "Ability3"),
(@REF_KROMOG, 2, 0, "MOGGOK DOOM AGURR!", 14, 0, 100, 0, 0, 42912, "ThunderingBlows"),
(@REF_KROMOG, 3, 0, "Into... nothing...", 14, 0, 100, 0, 0, 42906, "Slay1"),
(@REF_KROMOG, 3, 1, "To dust....", 14, 0, 100, 0, 0, 42907, "Slay2"),
(@REF_KROMOG, 4, 0, "Smash you... into mountain.", 14, 0, 100, 0, 0, 42908, "Frenzy"),
(@REF_KROMOG, 5, 0, "RRRRRRRRRrrooaaaarrr...", 14, 0, 100, 0, 0, 42905, "Death"),
(@REF_KROMOG, 6, 0, "%s begins to cast |cFFFF0404|Hspell:157659|h[Rippling Smash]|h|r!", 41, 0, 100, 0, 0, 0, "RipplingSmash"),
(@REF_KROMOG, 7, 0, "%s begins to channel |cFFFF0404|Hspell:157059|h[Rune of Grasping Earth]|h|r, seek a rune for protection!", 41, 0, 100, 0, 0, 0, "GraspingEarth");

DELETE FROM creature_text WHERE entry = @REF_COSMETIC_BLACKHAND AND groupid = 12;
INSERT INTO creature_text VALUE
(@REF_COSMETIC_BLACKHAND, 12, 0, "That beast was irreplaceable. Where are the attackers now? I want them dead. DEAD!", 14, 0, 100, 0, 0, 45462, "KromogKilled");

DELETE FROM locales_creature_text WHERE entry = @REF_KROMOG;
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_KROMOG, 0, 0,
    "Oggum dror undro dukkar.",
    "Oggum dror undro dukkar.",
    "Oggum dror undro dukkar.",
    "Оггум дрор ундро дуккар."
),
(
    @REF_KROMOG, 1, 0,
    "DÉTRUIRE !",
    "ZERSTÖRUNG!",
    "¡DESTRUCCIÓN!",
    "УНИЧТОЖУ!"
),
(
    @REF_KROMOG, 1, 1,
    "TERRE EN COLÈRE !",
    "ERDRUTSCH!",
    "¡DERRUMBE!",
    "ОБВАЛ!"
),
(
    @REF_KROMOG, 1, 2,
    "Enterrer vous !",
    "Ich begrabe Euch!",
    "¡Entierro!",
    "Зарою!"
),
(
    @REF_KROMOG, 2, 0,
    "MOGGOK DOOM AGURR !",
    "MOGGOK DOOM AGURR",
    "MOGGOK DOOM AGURR",
    "МОГГОК ДУМ АГУРР!"
),
(
    @REF_KROMOG, 3, 0,
    "Plus rien...",
    "Ins Nichts...",
    "Nada deberá...",
    "Никто не выживет..."
),
(
    @REF_KROMOG, 3, 1,
    "Poussière...",
    "Zu Staub...",
    "Al polvo...",
    "В пыль..."
),
(
    @REF_KROMOG, 4, 0,
    "Fracasser vous... dans la montagne.",
    "Gegen den Berg.",
    "Machacarte de vuelta a la montaña.",
    "Я вобью вас в камень."
),
(
    @REF_KROMOG, 5, 0,
    "GRRRRRRRRRrrooaaaarrr...",
    "RRRRRRRRRrrooaaaarrr...",
    "ROAAARRRRRRRRrrrRrrrrrr.r..r.rrr.",
    "А-а-а-а-ар-р-р-р-гх!"
),
(
    @REF_KROMOG, 6, 0,
    "%s commence à incanter |cFFFF0404|Hspell:157659|h[Choc ondoyant]|h|r !",
    "%s beginnt,|cFFFF0404|Hspell:157659|h[Wellenschlag]|h|r zu wirken!",
    "¡%s empieza a lanzar |cFFFF0404|Hspell:157659|h[Machaque ondulante]|h|r!",
    "%s начинает читать заклинание |cFFFF0404|Hspell:157659|h[Волновой удар]|h|r!"
),
(
    @REF_KROMOG, 7, 0,
    "%s commence à canaliser |cFFFF0404|Hspell:157059|h[Rune de terre avide]|h|r. Cherchez une rune pour vous protéger !",
    "%s beginnt, |cFFFF0404|Hspell:157059|h[Rune der klammernden Erde]|h|r zu kanalisieren! Sucht nach einer Rune, die Euch Schutz gewährt!",
    "¡%s comienza a canalizar |cFFFF0404|Hspell:157059|h[Runa de tierra enredadora]|h|r, busca una runa para protegerte!",
    "%s начинает поддерживать заклинание |cFFFF0404|Hspell:157059|h[Руна удерживающей земли]|h|r! Найдите защитную руну!"
);

DELETE FROM locales_creature_text WHERE entry = @REF_COSMETIC_BLACKHAND AND textGroup = 12;
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUE
(
    @REF_COSMETIC_BLACKHAND, 12, 0,
    "Cette créature est irremplaçable. Où sont les agresseurs ? Je les veux morts. MORTS !",
    "Diese Bestie war einzigartig. Wo sind ihre Mörder jetzt? Ich will sie tot sehen. TOT!",
    "Esa bestia era irremplazable. ¿En dónde están los atacantes ahora? Los quiero muertos. ¡MUERTOS!",
    "Где я теперь найду другого зверя? Куда делись нападавшие? Убить их всех! Убить!"
);

DELETE FROM `reference_loot_template` WHERE entry = @REF_KROMOG;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KROMOG, 113926, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113927, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113928, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113929, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113930, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113931, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113932, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113933, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113934, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113935, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113936, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113937, 0, 1, 1, 1, 1),
(@REF_KROMOG, 113938, 0, 1, 1, 1, 1);

DELETE FROM reference_loot_template WHERE entry = @REF_KROMOG + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KROMOG + 1, 119308, 0, 14, 1, 1, 1),
(@REF_KROMOG + 1, 119312, 0, 14, 1, 1, 1),
(@REF_KROMOG + 1, 119321, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_KROMOG WHERE `entry`= @REF_KROMOG;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_KROMOG;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KROMOG, 1, 100, 1, 0, -@REF_KROMOG, 6),
(@REF_KROMOG, 2, 100, 2, 0, -(@REF_KROMOG + 1), 2),
(@REF_KROMOG, 3, 100, 4, 0, -(@REF_KROMOG + 1), 1),
(@REF_KROMOG, 4, 100, 8, 0, -(@REF_KROMOG + 1), 1);

DELETE FROM creature_groupsizestats WHERE entry = @REF_KROMOG;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_KROMOG, 14, 10, 42247300),
(@REF_KROMOG, 14, 11, 46682500),
(@REF_KROMOG, 14, 12, 51162600),
(@REF_KROMOG, 14, 13, 55585600),
(@REF_KROMOG, 14, 14, 60045400),
(@REF_KROMOG, 14, 15, 64421900),
(@REF_KROMOG, 14, 16, 68875100),
(@REF_KROMOG, 14, 17, 73309300),
(@REF_KROMOG, 14, 18, 77750700),
(@REF_KROMOG, 14, 19, 82176000),
(@REF_KROMOG, 14, 20, 86655100),
(@REF_KROMOG, 14, 21, 91044000),
(@REF_KROMOG, 14, 22, 95517800),
(@REF_KROMOG, 14, 23, 99880500),
(@REF_KROMOG, 14, 24, 104344400),
(@REF_KROMOG, 14, 25, 108837600),
(@REF_KROMOG, 14, 26, 113229500),
(@REF_KROMOG, 14, 27, 117664500),
(@REF_KROMOG, 14, 28, 122102300),
(@REF_KROMOG, 14, 29, 126533400),
(@REF_KROMOG, 14, 30, 131064500),
(@REF_KROMOG, 15, 10, 59045600),
(@REF_KROMOG, 15, 11, 65276700),
(@REF_KROMOG, 15, 12, 71447800),
(@REF_KROMOG, 15, 13, 77698900),
(@REF_KROMOG, 15, 14, 83849000),
(@REF_KROMOG, 15, 15, 90060100),
(@REF_KROMOG, 15, 16, 96231200),
(@REF_KROMOG, 15, 17, 102432300),
(@REF_KROMOG, 15, 18, 108633400),
(@REF_KROMOG, 15, 19, 114834500),
(@REF_KROMOG, 15, 20, 121055600),
(@REF_KROMOG, 15, 21, 127276700),
(@REF_KROMOG, 15, 22, 133457800),
(@REF_KROMOG, 15, 23, 139638900),
(@REF_KROMOG, 15, 24, 145879000),
(@REF_KROMOG, 15, 25, 152050100),
(@REF_KROMOG, 15, 26, 158281200),
(@REF_KROMOG, 15, 27, 164442300),
(@REF_KROMOG, 15, 28, 170723400),
(@REF_KROMOG, 15, 29, 176834500),
(@REF_KROMOG, 15, 30, 183045600);

SET @REF_GRASPING_EARTH = 77893;

DELETE FROM creature_groupsizestats WHERE entry = @REF_GRASPING_EARTH;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_GRASPING_EARTH, 14, 10, 628300),
(@REF_GRASPING_EARTH, 14, 11, 628300),
(@REF_GRASPING_EARTH, 14, 12, 628300),
(@REF_GRASPING_EARTH, 14, 13, 628300),
(@REF_GRASPING_EARTH, 14, 14, 628300),
(@REF_GRASPING_EARTH, 14, 15, 628300),
(@REF_GRASPING_EARTH, 14, 16, 628300),
(@REF_GRASPING_EARTH, 14, 17, 628300),
(@REF_GRASPING_EARTH, 14, 18, 628300),
(@REF_GRASPING_EARTH, 14, 19, 628300),
(@REF_GRASPING_EARTH, 14, 20, 628300),
(@REF_GRASPING_EARTH, 14, 21, 628300),
(@REF_GRASPING_EARTH, 14, 22, 628300),
(@REF_GRASPING_EARTH, 14, 23, 628300),
(@REF_GRASPING_EARTH, 14, 24, 628300),
(@REF_GRASPING_EARTH, 14, 25, 628300),
(@REF_GRASPING_EARTH, 14, 26, 628300),
(@REF_GRASPING_EARTH, 14, 27, 628300),
(@REF_GRASPING_EARTH, 14, 28, 628300),
(@REF_GRASPING_EARTH, 14, 29, 628300),
(@REF_GRASPING_EARTH, 14, 30, 628300),
(@REF_GRASPING_EARTH, 15, 10, 785400),
(@REF_GRASPING_EARTH, 15, 11, 785400),
(@REF_GRASPING_EARTH, 15, 12, 785400),
(@REF_GRASPING_EARTH, 15, 13, 785400),
(@REF_GRASPING_EARTH, 15, 14, 785400),
(@REF_GRASPING_EARTH, 15, 15, 785400),
(@REF_GRASPING_EARTH, 15, 16, 785400),
(@REF_GRASPING_EARTH, 15, 17, 785400),
(@REF_GRASPING_EARTH, 15, 18, 785400),
(@REF_GRASPING_EARTH, 15, 19, 785400),
(@REF_GRASPING_EARTH, 15, 20, 785400),
(@REF_GRASPING_EARTH, 15, 21, 785400),
(@REF_GRASPING_EARTH, 15, 22, 785400),
(@REF_GRASPING_EARTH, 15, 23, 785400),
(@REF_GRASPING_EARTH, 15, 24, 785400),
(@REF_GRASPING_EARTH, 15, 25, 785400),
(@REF_GRASPING_EARTH, 15, 26, 785400),
(@REF_GRASPING_EARTH, 15, 27, 785400),
(@REF_GRASPING_EARTH, 15, 28, 785400),
(@REF_GRASPING_EARTH, 15, 29, 785400),
(@REF_GRASPING_EARTH, 15, 30, 785400);
