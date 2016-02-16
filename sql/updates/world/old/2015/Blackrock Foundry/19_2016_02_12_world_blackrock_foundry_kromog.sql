-- Templates to update:
-- 77844 (Rune de Terre broyeuse)
-- 77929 (Réverbération)

-- 77893 (Grasping Earth)
-- 78055 (Rippling Smash)
-- 77692 (Kromog)

DELETE FROM spell_script_names WHERE spell_id IN (156704, 162348, 157056);
INSERT INTO spell_script_names VALUES
(156704, "spell_foundry_slam"),
(162348, "spell_foundry_fists_of_stone"),
(157056, "spell_foundry_rune_of_grasping_earth");

DELETE FROM areatrigger_template WHERE spell_id IN (161437);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `move_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `ScriptName`) VALUES
(161437, 0, 6493, 1, 1, 2570, 762, 4000, 6.5, 7.5, 2, 15, 7.5, "areatrigger_foundry_rippling_smash");

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