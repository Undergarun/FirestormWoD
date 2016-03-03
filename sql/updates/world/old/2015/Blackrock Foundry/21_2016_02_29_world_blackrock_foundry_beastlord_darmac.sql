SET NAMES utf8;

DELETE FROM spell_script_names WHERE spell_id IN (162653, 161300, 155365, 155702, 155416, 155385, 155515, 155423);
INSERT INTO spell_script_names VALUES
(162653, "spell_foundry_electrical_storm"),
(161300, "spell_foundry_hitching_post_chain"),
(155365, "spell_foundry_ranged_targets_searcher"),
(155702, "spell_foundry_target_vehicle"),
(155416, "spell_foundry_ranged_targets_searcher"),
(155385, "spell_foundry_ranged_targets_searcher"),
(155515, "spell_foundry_ranged_targets_searcher"),
(155423, "spell_foundry_ranged_targets_searcher");

DELETE FROM areatrigger_template WHERE spell_id IN (162657, 162757, 155026);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`) VALUES
(162657, 0, 6976, 7.5, 7.5, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_electrical_storm"),
(162757, 0, 6982, 6, 6, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_ice_trap"),
(155026, 0, 6417, 3, 3, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "");

DELETE FROM conditions WHERE SourceEntry IN (173790);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 173790, 31, 0, 3, 80423);

DELETE FROM achievement_criteria_data WHERE criteria_id IN (24675, 24676, 24677);
INSERT INTO achievement_criteria_data VALUES
(24675, 18, 0, 0, ""),
(24676, 18, 0, 0, ""),
(24677, 18, 0, 0, "");

SET @REF_DARMAC = 76865;

DELETE FROM creature_text WHERE entry IN (80400, 80409, @REF_DARMAC, 76945, 76874);
INSERT INTO creature_text VALUES
(80400, 0, 0, "You will not enter the Iron Assembly!", 12, 0, 100, 0, 0, 0, "AssemblyWarden"),
(80409, 0, 0, "Rargh!", 12, 0, 100, 0, 0, 0, "MarkogAbadir1"),
(80409, 0, 1, "Ugh! Get them!", 12, 0, 100, 0, 0, 0, "MarkogAbadir2"),

(@REF_DARMAC, 0, 0, "Come! Your meat and bones will feed my animals!", 14, 0, 100, 0, 0, 41952, "Aggro"),
(@REF_DARMAC, 1, 0, "Aid me!", 14, 0, 100, 0, 0, 41956, "Mounting Cruelfang"),
(@REF_DARMAC, 2, 0, "Rage unending... Face the fury of the wolf!", 14, 0, 100, 0, 0, 41959, "Cruelfang's Swiftness"),
(@REF_DARMAC, 3, 0, "Grant me your strength!", 14, 0, 100, 0, 0, 41955, "Mounting Ironcrusher"),
(@REF_DARMAC, 4, 0, "No! I will wield your strength...", 14, 0, 100, 0, 0, 41958, "Ironcrusher's Rage"),
(@REF_DARMAC, 5, 0, "Let us crush them!", 14, 0, 100, 0, 0, 41954, "Mounting Dreadwing"),
(@REF_DARMAC, 6, 0, "Your heart... dreams of fire...", 14, 0, 100, 0, 0, 41957, "Dreadwing's Flame"),
(@REF_DARMAC, 7, 0, "I will NOT die here! Faultline, come aid your master!", 14, 0, 100, 0, 0, 41960, "Mounting Faultline (Mythic Only)"),
(@REF_DARMAC, 8, 0, "Faultline! I will avenge you! Yes... There. Will. Be. BLOOD!", 14, 0, 100, 0, 0, 41961, "Faultline's Determination (Mythic Only)"),
(@REF_DARMAC, 9, 0, "Bleed for me!", 14, 0, 100, 0, 0, 41966, "Rend and Tear"),
(@REF_DARMAC, 10, 0, "Feel my fury!", 14, 0, 100, 0, 0, 41965, "Tantrum"),
(@REF_DARMAC, 11, 0, "I will roast your flesh!", 14, 0, 100, 0, 0, 41967, "Superheated Shrapnel"),
(@REF_DARMAC, 12, 0, "Animality!", 14, 0, 100, 0, 0, 41962, "Killing a player 1"),
(@REF_DARMAC, 12, 1, "That one shall not die alone.", 14, 0, 100, 0, 0, 41963, "Killing a player 2"),
(@REF_DARMAC, 12, 2, "Weak.", 14, 0, 100, 0, 0, 41964, "Killing a player 3"),
(@REF_DARMAC, 13, 0, "The others... will avenge... me...", 14, 0, 100, 0, 0, 41953, "Death"),
(@REF_DARMAC, 14, 0, "%s prepares |cFFFF0404|Hspell:155499|h[Superheated Shrapnel]|h|r!", 41, 0, 100, 0, 0, 0, "Superheated Shrapnel Warning"),
(@REF_DARMAC, 15, 0, "%s goes into a wild |cFFFF0404|Hspell:162275|h[Tantrum]|h|r!", 41, 0, 100, 0, 0, 0, "Tantrum Warning"),

(76945, 0, 0, "%s goes into a wild |cFFFF0404|Hspell:155222|h[Tantrum]|h|r!", 41, 0, 100, 0, 0, 0, "Tantrum Warning"),

(76874, 0, 0, "%s readies her |cFFFF0404|Hspell:154989|h[Inferno Breath]|h|r!", 41, 0, 100, 0, 0, 0, "Inferno Breath Warning");

DELETE FROM locales_creature_text WHERE entry IN (80400, 80409, @REF_DARMAC, 76945, 76874);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    80400, 0, 0,
    "Vous n'entrerez pas dans l'assemblée de Fer !",
    "Ihr werdet die Eiserne Fertigung nicht betreten!",
    "¡No entrarás a la Asamblea de Hierro!",
    "Вам не добраться до Железного цеха!"
),
(
    80409, 0, 0,
    "Rargh !",
    "Rargh!",
    "¡Argh!",
    "Ар-р-р!"
),
(
    80409, 0, 1,
    "Attrapez-les !",
    "Ugh! Schnappt sie!",
    "¡Agh! ¡Atrápenlos!",
    "Эй! Взять их!"
),
(
    @REF_DARMAC, 0, 0,
    "Viens ! Je nourrirai mes animaux de ta chair et de tes os...",
    "Kommt nur! Meine Tiere freuen sich schon auf Euer Fleisch!",
    "¡Ven! ¡Tu carne y huesos alimentarán a mis animales!",
    "Подходите! Я накормлю зверей вашим мясом."
),
(
    @REF_DARMAC, 1, 0,
    "Aide-moi !",
    "Hilf mir!",
    "¡Ayúdame!",
    "На помощь!"
),
(
    @REF_DARMAC, 2, 0,
    "Rage interminable... Affrontez la fureur du loup !",
    "Endlose Wut... Stellt Euch dem Zorn des Wolfs.",
    "Ira interminable... Enfrenten la furia del lobo.",
    "Этот гнев... Почувствуйте ярость волка!"
),
(
    @REF_DARMAC, 3, 0,
    "Accorde-moi ta force !",
    "Gib mir deine Stärke!",
    "¡Concédeme tu fuerza!",
    "Дай мне свою силу!"
),
(
    @REF_DARMAC, 4, 0,
    "Non ! Je vais prendre ta force...",
    "Nein! Deine Stärke wird die meine sein...",
    "¡No! Yo portaré tu fuerza...",
    "Нет! Я возьму твою силу..."
),
(
    @REF_DARMAC, 5, 0,
    "Écrasons-les !",
    "Zermalmen wir sie!",
    "¡Aplastémoslos!",
    "Раздробим их кости!"
),
(
    @REF_DARMAC, 6, 0,
    "Ton cœur... Des rêves de feu...",
    "Dein Herz... träumt von Feuer...",
    "Tu corazón... sueña con fuego...",
    "Сердце... полное огня..."
),
(
    @REF_DARMAC, 7, 0,
    "Je ne mourrai PAS ici ! Tectonique, viens aider ton maître !",
    "NIEMALS werde ich hier sterben! Karst, komm und hilf deinem Meister!",
    "¡Yo NO moriré aquí! Falla, ¡ayuda a tu maestro!",
    "Я не собираюсь здесь умирать! Разлом, на помощь!"
),
(
    @REF_DARMAC, 8, 0,
    "Tectonique ! Je te vengerai ! LE SANG VA COULER !",
    "Karst! Ich werde dich rächen! Das hier endet BLUTIG!",
    "¡Falla! ¡Te vengaré! ¡Va a correr SANGRE!",
    "Разлом! Я отомщу за тебя! Враги. Умоются. Кровью."
),
(
    @REF_DARMAC, 9, 0,
    "Saigne !",
    "Blutet für mich!",
    "¡Sangra para mí!",
    "Истекайте кровью!"
),
(
    @REF_DARMAC, 10, 0,
    "Ressens ma fureur !",
    "Spürt meinen Zorn!",
    "¡Siente mi furia!",
    "Ощутите мой гнев!"
),
(
    @REF_DARMAC, 11, 0,
    "Je ferai rôtir ta chair !",
    "Ich brate Euer Fleisch!",
    "¡Asaré tu carne!",
    "Поджарьтесь!"
),
(
    @REF_DARMAC, 12, 0,
    "Bestial !",
    "Animalisch!",
    "¡Animalidad!",
    "Пойдешь на корм!"
),
(
    @REF_DARMAC, 12, 1,
    "Celui-là ne mourra pas seul.",
    "Diesem Tod werden weitere folgen.",
    "Ese no habrá de morir solo.",
    "Эта смерть – не последняя."
),
(
    @REF_DARMAC, 12, 2,
    "Faible.",
    "Schwach.",
    "Débil.",
    "Ничтожество."
),
(
    @REF_DARMAC, 13, 0,
    "Les autres... ils me... vengeront...",
    "Die anderen... werden mich... rächen...",
    "Los otros... me... vengarán...",
    "За меня... отомстят..."
),
(
    @REF_DARMAC, 14, 0,
    "%s prépare |cFFFF0404|Hspell:155499|h[Shrapnel surchauffé]|h|r !",
    "%s bereitet |cFFFF0404|Hspell:155499|h[Supererhitztes Schrapnell]|h|r! vor!",
    "¡%s prepara |cFFFF0404|Hspell:155499|h[Metralla sobrecalentada]|h|r!",
    "%s готовит |cFFFF0404|Hspell:155499|h[Раскаленные осколки]|h|r!"
),
(
    @REF_DARMAC, 15, 0,
    "%s se met dans une violente |cFFFF0404|Hspell:162275|h[Colère]|h|r !",
    "%s bekommt einen mächtigen |cFFFF0404|Hspell:162275|h[Wutanfall]|h|r!",
    "¡%s entra en una salvaje |cFFFF0404|Hspell:162275|h[Rabieta]|h|r!",
    "%s поддается ужасной |cFFFF0404|Hspell:162275|h[Вспышке гнева]|h|r!"
),
(
    76945, 0, 0,
    "%s part dans une violente |cFFFF0404|Hspell:155222|h[Colère]|h|r !",
    "%s bekommt einen mächtigen |cFFFF0404|Hspell:155222|h[Wutanfall]|h|r!",
    "¡%s entra en una salvaje |cFFFF0404|Hspell:155222|h[Rabieta]|h|r!",
    "%s поддается ужасной |cFFFF0404|Hspell:155222|h[Вспышке гнева]|h|r!"
),
(
    76874, 0, 0,
    "%s prépare |cFFFF0404|Hspell:154989|h[Souffle infernal]|h|r !",
    "%s bereitet ihren |cFFFF0404|Hspell:154989|h[Infernoatem]|h|r vor!",
    "¡%s prepara su |cFFFF0404|Hspell:154989|h[Aliento Infernal]|h|r!",
    "%s готовится применить |cFFFF0404|Hspell:154989|h[Дыхание преисподней]|h|r!"
);