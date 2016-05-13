SET NAMES utf8;

DELETE FROM spell_script_names WHERE spell_id IN (162653, 161300, 155365, 155702, 155416, 155385, 155515, 155423, 155603, 173974);
INSERT INTO spell_script_names VALUES
(162653, "spell_foundry_electrical_storm"),
(161300, "spell_foundry_hitching_post_chain"),
(155365, "spell_foundry_ranged_targets_searcher"),
(155702, "spell_foundry_target_vehicle"),
(155416, "spell_foundry_ranged_targets_searcher"),
(155385, "spell_foundry_ranged_targets_searcher"),
(155515, "spell_foundry_ranged_targets_searcher"),
(155423, "spell_foundry_ranged_targets_searcher"),
(155603, "spell_foundry_ranged_targets_searcher"),
(173974, "spell_foundry_infusion_of_flames");

DELETE FROM areatrigger_template WHERE spell_id IN (162657, 162757, 155026, 155503, 155653);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`) VALUES
(162657, 0, 6976, 7.5, 7.5, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_electrical_storm"),
(162757, 0, 6982, 6, 6, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_ice_trap"),
(155026, 0, 6417, 3, 3, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_inferno_breath"),
(155503, 0, 6416, 3, 3, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_superheated_shrapnel"),
(155653, 0, 6268, 3, 3, 16384, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_flame_infusion");

DELETE FROM conditions WHERE SourceEntry IN (173790, 155604, 155650);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 173790, 31, 0, 3, 80423),
(13, 1, 155604, 31, 0, 3, 76796),
(13, 1, 155650, 31, 0, 3, 77128);

UPDATE instance_disabled_rankings SET Comment = "Beastlord Darmac - Normal - Tests in progress" WHERE EncounterID = 1694 AND DifficultyID = 14;
UPDATE instance_disabled_rankings SET Comment = "Beastlord Darmac - Heroic - Tests in progress" WHERE EncounterID = 1694 AND DifficultyID = 15;

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

DELETE FROM `reference_loot_template` WHERE entry = @REF_DARMAC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_DARMAC, 113939, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113940, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113941, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113942, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113943, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113944, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113945, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113946, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113947, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113948, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113949, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113950, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113951, 0, 1, 1, 1, 1),
(@REF_DARMAC, 113952, 0, 1, 1, 1, 1),
(@REF_DARMAC, 119192, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_DARMAC WHERE `entry`= @REF_DARMAC;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_DARMAC;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_DARMAC, 1, 100, 1, 0, -@REF_DARMAC, 6);

DELETE FROM creature_groupsizestats WHERE entry = 76796;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(76796, 14, 10, 315021),
(76796, 14, 11, 349999),
(76796, 14, 12, 382054),
(76796, 14, 13, 414284),
(76796, 14, 14, 450586),
(76796, 14, 15, 482532),
(76796, 14, 16, 515430),
(76796, 14, 17, 546147),
(76796, 14, 18, 579842),
(76796, 14, 19, 616584),
(76796, 14, 20, 645182),
(76796, 14, 21, 685787),
(76796, 14, 22, 713317),
(76796, 14, 23, 743483),
(76796, 14, 24, 782154),
(76796, 14, 25, 813005),
(76796, 14, 26, 846089),
(76796, 14, 27, 876131),
(76796, 14, 28, 911202),
(76796, 14, 29, 941639),
(76796, 14, 30, 976819),
(76796, 15, 10, 435611),
(76796, 15, 11, 475611),
(76796, 15, 12, 508198),
(76796, 15, 13, 548202),
(76796, 15, 14, 562226),
(76796, 15, 15, 602478),
(76796, 15, 16, 641902),
(76796, 15, 17, 700350),
(76796, 15, 18, 740820),
(76796, 15, 19, 770514),
(76796, 15, 20, 806089),
(76796, 15, 21, 847095),
(76796, 15, 22, 922552),
(76796, 15, 23, 954498),
(76796, 15, 24, 971740),
(76796, 15, 25, 1013485),
(76796, 15, 26, 1072404),
(76796, 15, 27, 1096712),
(76796, 15, 28, 1145023),
(76796, 15, 29, 1188842),
(76796, 15, 30, 1222705);

DELETE FROM creature_groupsizestats WHERE entry = 76884;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(76884, 14, 10, 9142721),
(76884, 14, 11, 10037140),
(76884, 14, 12, 10992372),
(76884, 14, 13, 11950975),
(76884, 14, 14, 12896220),
(76884, 14, 15, 13857556),
(76884, 14, 16, 14805188),
(76884, 14, 17, 15779175),
(76884, 14, 18, 16713279),
(76884, 14, 19, 17691476),
(76884, 14, 20, 18632940),
(76884, 14, 21, 19580805),
(76884, 14, 22, 20544077),
(76884, 14, 23, 21478267),
(76884, 14, 24, 22683767),
(76884, 14, 25, 23398145),
(76884, 14, 26, 24351026),
(76884, 14, 27, 25306496),
(76884, 14, 28, 26247173),
(76884, 14, 29, 27210615),
(76884, 14, 30, 28156399),
(76884, 15, 10, 11274563),
(76884, 15, 11, 12558424),
(76884, 15, 12, 13743008),
(76884, 15, 13, 14932269),
(76884, 15, 14, 16120602),
(76884, 15, 15, 17319963),
(76884, 15, 16, 18507942),
(76884, 15, 17, 19720510),
(76884, 15, 18, 20961425),
(76884, 15, 19, 22084480),
(76884, 15, 20, 23273619),
(76884, 15, 21, 24490757),
(76884, 15, 22, 25662130),
(76884, 15, 23, 26853216),
(76884, 15, 24, 28095308),
(76884, 15, 25, 29250944),
(76884, 15, 26, 30424972),
(76884, 15, 27, 31617037),
(76884, 15, 28, 32826888),
(76884, 15, 29, 33965467),
(76884, 15, 30, 35193074);

DELETE FROM creature_groupsizestats WHERE entry = 76874;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(76874, 14, 10, 8713388),
(76874, 14, 11, 9719034),
(76874, 14, 12, 10512490),
(76874, 14, 13, 11432005),
(76874, 14, 14, 12352087),
(76874, 14, 15, 13248381),
(76874, 14, 16, 14163767),
(76874, 14, 17, 15072196),
(76874, 14, 18, 15990988),
(76874, 14, 19, 16897712),
(76874, 14, 20, 17842836),
(76874, 14, 21, 18720570),
(76874, 14, 22, 19640727),
(76874, 14, 23, 20544402),
(76874, 14, 24, 21492621),
(76874, 14, 25, 22383901),
(76874, 14, 26, 23301354),
(76874, 14, 27, 24194151),
(76874, 14, 28, 25144070),
(76874, 14, 29, 26022105),
(76874, 14, 30, 26933932),
(76874, 15, 10, 10945468),
(76874, 15, 11, 12002067),
(76874, 15, 12, 13141581),
(76874, 15, 13, 14279797),
(76874, 15, 14, 15445578),
(76874, 15, 15, 16601675),
(76874, 15, 16, 17707849),
(76874, 15, 17, 18845254),
(76874, 15, 18, 19985327),
(76874, 15, 19, 21127325),
(76874, 15, 20, 22264900),
(76874, 15, 21, 23417698),
(76874, 15, 22, 24555476),
(76874, 15, 23, 25720368),
(76874, 15, 24, 26824427),
(76874, 15, 25, 27966413),
(76874, 15, 26, 29109427),
(76874, 15, 27, 30250161),
(76874, 15, 28, 31395296),
(76874, 15, 29, 32534993),
(76874, 15, 30, 33667840);

DELETE FROM creature_groupsizestats WHERE entry = 76945;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(76945, 14, 10, 9483706),
(76945, 14, 11, 10476180),
(76945, 14, 12, 11468669),
(76945, 14, 13, 12462251),
(76945, 14, 14, 13473090),
(76945, 14, 15, 14461709),
(76945, 14, 16, 15473626),
(76945, 14, 17, 16462789),
(76945, 14, 18, 17437026),
(76945, 14, 19, 18436433),
(76945, 14, 20, 19431699),
(76945, 14, 21, 20423083),
(76945, 14, 22, 21436218),
(76945, 14, 23, 22446252),
(76945, 14, 24, 23411331),
(76945, 14, 25, 24404767),
(76945, 14, 26, 25400589),
(76945, 14, 27, 26393631),
(76945, 14, 28, 27396239),
(76945, 14, 29, 28387838),
(76945, 14, 30, 29389837),
(76945, 15, 10, 11860255),
(76945, 15, 11, 13103935),
(76945, 15, 12, 14342046),
(76945, 15, 13, 15597288),
(76945, 15, 14, 16840176),
(76945, 15, 15, 18152633),
(76945, 15, 16, 19342882),
(76945, 15, 17, 20556991),
(76945, 15, 18, 21834016),
(76945, 15, 19, 23043091),
(76945, 15, 20, 24316778),
(76945, 15, 21, 25727587),
(76945, 15, 22, 26778657),
(76945, 15, 23, 28032366),
(76945, 15, 24, 29289057),
(76945, 15, 25, 30504031),
(76945, 15, 26, 31748403),
(76945, 15, 27, 32991691),
(76945, 15, 28, 34238729),
(76945, 15, 29, 35520079),
(76945, 15, 30, 36730704);

DELETE FROM creature_groupsizestats WHERE entry = 77128;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(77128, 14, 10, 384648),
(77128, 14, 11, 401900),
(77128, 14, 12, 452665),
(77128, 14, 13, 481518),
(77128, 14, 14, 552456),
(77128, 14, 15, 599940),
(77128, 14, 16, 628751),
(77128, 14, 17, 655846),
(77128, 14, 18, 674374),
(77128, 14, 19, 706546),
(77128, 14, 20, 744468),
(77128, 14, 21, 781170),
(77128, 14, 22, 830847),
(77128, 14, 23, 860563),
(77128, 14, 24, 896729),
(77128, 14, 25, 934019),
(77128, 14, 26, 973207),
(77128, 14, 27, 1046191),
(77128, 14, 28, 1069838),
(77128, 14, 29, 1092838),
(77128, 14, 30, 1135168),
(77128, 15, 10, 456311),
(77128, 15, 11, 506365),
(77128, 15, 12, 555325),
(77128, 15, 13, 609237),
(77128, 15, 14, 669990),
(77128, 15, 15, 714231),
(77128, 15, 16, 750140),
(77128, 15, 17, 811872),
(77128, 15, 18, 834135),
(77128, 15, 19, 881934),
(77128, 15, 20, 931653),
(77128, 15, 21, 977800),
(77128, 15, 22, 1023045),
(77128, 15, 23, 1071478),
(77128, 15, 24, 1122044),
(77128, 15, 25, 1165377),
(77128, 15, 26, 1215289),
(77128, 15, 27, 1269031),
(77128, 15, 28, 1344873),
(77128, 15, 29, 1358041),
(77128, 15, 30, 1404132);

DELETE FROM creature_groupsizestats WHERE entry = @REF_DARMAC;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_DARMAC, 14, 10, 30491761),
(@REF_DARMAC, 14, 11, 33601108),
(@REF_DARMAC, 14, 12, 36917671),
(@REF_DARMAC, 14, 13, 39984555),
(@REF_DARMAC, 14, 14, 43408459),
(@REF_DARMAC, 14, 15, 46375099),
(@REF_DARMAC, 14, 16, 49566695),
(@REF_DARMAC, 14, 17, 52943851),
(@REF_DARMAC, 14, 18, 56075155),
(@REF_DARMAC, 14, 19, 59313454),
(@REF_DARMAC, 14, 20, 62624627),
(@REF_DARMAC, 14, 21, 65611575),
(@REF_DARMAC, 14, 22, 68860719),
(@REF_DARMAC, 14, 23, 71976819),
(@REF_DARMAC, 14, 24, 75103258),
(@REF_DARMAC, 14, 25, 78453093),
(@REF_DARMAC, 14, 26, 81678366),
(@REF_DARMAC, 14, 27, 84697803),
(@REF_DARMAC, 14, 28, 88082045),
(@REF_DARMAC, 14, 29, 91254160),
(@REF_DARMAC, 14, 30, 94615689),
(@REF_DARMAC, 15, 10, 38006277),
(@REF_DARMAC, 15, 11, 41999916),
(@REF_DARMAC, 15, 12, 45988008),
(@REF_DARMAC, 15, 13, 49981249),
(@REF_DARMAC, 15, 14, 53967413),
(@REF_DARMAC, 15, 15, 57976757),
(@REF_DARMAC, 15, 16, 61966016),
(@REF_DARMAC, 15, 17, 66050481),
(@REF_DARMAC, 15, 18, 70049161),
(@REF_DARMAC, 15, 19, 73984665),
(@REF_DARMAC, 15, 20, 77928904),
(@REF_DARMAC, 15, 21, 82071322),
(@REF_DARMAC, 15, 22, 86256514),
(@REF_DARMAC, 15, 23, 90049809),
(@REF_DARMAC, 15, 24, 94026717),
(@REF_DARMAC, 15, 25, 97928388),
(@REF_DARMAC, 15, 26, 102082720),
(@REF_DARMAC, 15, 27, 106158545),
(@REF_DARMAC, 15, 28, 110096977),
(@REF_DARMAC, 15, 29, 113842427),
(@REF_DARMAC, 15, 30, 117954731);