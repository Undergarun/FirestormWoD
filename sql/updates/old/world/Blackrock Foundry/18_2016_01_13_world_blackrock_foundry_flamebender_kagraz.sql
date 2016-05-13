SET NAMES utf8;

SET @REF_KAGRAZ = 76814;

DELETE FROM areatrigger_scripts WHERE entry = 9999;
INSERT INTO areatrigger_scripts VALUE
(9999, "areatrigger_at_foundry_hansgar_and_franzok_exit");

DELETE FROM creature_text WHERE entry IN (80708, @REF_KAGRAZ);
INSERT INTO creature_text VALUES
(80708, 0, 0, "You worthless swine! Get back to work. We have a quota to make!", 14, 0, 100, 0, 0, 0, "TaskmasterIntro"),
(80708, 1, 0, "Don't let them get by us!", 14, 0, 100, 0, 0, 0, "TaskmasterOutro"),

(@REF_KAGRAZ, 0, 0, "My work. You've interrupted my WORK!", 14, 0, 100, 0, 0, 41990, "Aggro"),
(@REF_KAGRAZ, 1, 0, "Aknor! You are an embarrassment to your clan.", 14, 0, 100, 0, 0, 41991, "AknorDeath"),
(@REF_KAGRAZ, 2, 0, "Burn!", 14, 0, 100, 0, 0, 42002, "Firestorm1"),
(@REF_KAGRAZ, 2, 1, "Your flesh feeds the fire.", 14, 0, 100, 0, 0, 42003, "Firestorm2"),
(@REF_KAGRAZ, 2, 2, "Iron and flame!", 14, 0, 100, 0, 0, 42004, "Firestorm3"),
(@REF_KAGRAZ, 3, 0, "Come my pets.", 14, 0, 100, 0, 0, 42005, "CinderWolves1"),
(@REF_KAGRAZ, 3, 1, "Feeding time!", 14, 0, 100, 0, 0, 42006, "CinderWolves2"),
(@REF_KAGRAZ, 3, 2, "I have more friends for you to meet!", 14, 0, 100, 0, 0, 42007, "CinderWolves3"),
(@REF_KAGRAZ, 4, 0, "That's enough! You will burn... You will ALL burn!", 14, 0, 100, 0, 0, 41992, "Berserk"),
(@REF_KAGRAZ, 5, 0, "Another falls to my blade!", 14, 0, 100, 0, 0, 42000, "Slay1"),
(@REF_KAGRAZ, 5, 1, "Too hot for you?", 14, 0, 100, 0, 0, 42001, "Slay2"),
(@REF_KAGRAZ, 6, 0, "Your bodies will make good kindling.", 14, 0, 100, 0, 0, 41999, "Wipe"),
(@REF_KAGRAZ, 7, 0, "My flame... burned too bright.", 14, 0, 100, 0, 0, 41997, "Death"),
(@REF_KAGRAZ, 8, 0, "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t%s summons|cFFFF0000 Cinder Wolves|r!", 41, 0, 100, 0, 0, 0, "CinderWolvesWarn"),
(@REF_KAGRAZ, 9, 0, "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t%s begins to channel |cFFFF0404|Hspell:155493|h[Firestorm]|h|r!", 41, 0, 100, 0, 0, 0, "FirestormWarn");

DELETE FROM locales_creature_text WHERE entry IN (80708, @REF_KAGRAZ);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    80708, 0, 0,
    "Pourceaux incapables ! Remettez-vous au travail. On a un quota à respecter !",
    "Wertloses Schwein! Zurück an die Arbeit! Wir haben hier Vorgaben zu erfüllen!",
    "¡Cerdo inútil! De vuelta al trabajo. ¡Tenemos una meta que cumplir!",
    "Грязная свинья! А ну за работу! Надо план выполнять!"
),
(
    80708, 1, 0,
    "Ne les laissez pas passer !",
    "Lasst sie nicht vorbei!",
    "¡No dejen que nos pasen!",
    "Не позволяйте им приблизиться к нам!"
),
(
    @REF_KAGRAZ, 0, 0,
    "Mon travail. Vous avez interrompu mon TRAVAIL !",
    "Meine Arbeit. Ihr stört meine ARBEIT!",
    "Mi trabajo. ¡Interrumpieron mi TRABAJO!",
    "Работа! Вы отрываете меня от работы!"
),
(
    @REF_KAGRAZ, 1, 0,
    "Aknor ! Tu es la honte de ton clan.",
    "Aknor! Ihr seid eine Schande für Euren Klan.",
    "¡Aknor! Eres una vergüenza para tu clan.",
    "Акнор! Ты – позор для клана"
),
(
    @REF_KAGRAZ, 2, 0,
    "Brûlez !",
    "Brennt!",
    "¡Ardan!",
    "Горите!"
),
(
    @REF_KAGRAZ, 2, 1,
    "Le feu se nourrit de votre chair ",
    "Euer Fleisch nährt das Feuer!",
    "Su carne alimenta el fuego.",
    "Пламя пожрет вашу плоть."
),
(
    @REF_KAGRAZ, 2, 2,
    "Par le fer et la flamme !",
    "Eisen und Flamme!",
    "¡Hierro y llamas!",
    "Огонь и железо!"
),
(
    @REF_KAGRAZ, 3, 0,
    "Venez, mes chéris.",
    "Zu mir, meine Schoßhunde!",
    "Vengan, mascotas.",
    "Ко мне, звери."
),
(
    @REF_KAGRAZ, 3, 1,
    "C'est l'heure du repas !",
    "Jetzt gibt es was zu fressen!",
    "¡Hora de comer!",
    "Пора обедать!"
),
(
    @REF_KAGRAZ, 3, 2,
    "J'ai d'autres amis à vous présenter !",
    "Hier sind noch mehr meiner Freunde!",
    "¡Tengo más amigos para que conozcas!",
    "Познакомьтесь с моими друзьями!"
),
(
    @REF_KAGRAZ, 4, 0,
    "Ça suffit... Vous allez brûler ! VOUS ALLEZ TOUS BRÛLER !",
    "Es reicht  Ihr werdet brennen! IHR WERDET ALLE BRENNEN!",
    "Es suficiente... ¡arderán! ¡TODOS USTEDES ARDERÁN!",
    "Довольно! Вы сгорите! Вы все сгорите!"
),
(
    @REF_KAGRAZ, 5, 0,
    "Une nouvelle victime de ma lame !",
    "Ein weiteres Opfer meiner Klinge!",
    "¡Otro más cae ante mi espada!",
    "Мой клинок разит без промаха!"
),
(
    @REF_KAGRAZ, 5, 1,
    "C'est trop chaud pour vous ?",
    "Zu heiß für Euch?",
    "¿Está muy caliente?",
    "Что, горячо?"
),
(
    @REF_KAGRAZ, 6, 0,
    "Vos corps feront un excellent combustible.",
    "Eure Körper sind hervorragender Zunder.",
    "Sus cuerpos servirán de leña.",
    "Ваши трупы пойдут на растопку."
),
(
    @REF_KAGRAZ, 7, 0,
    "Ma flamme... Elle brûlait trop intensément.",
    "Meine Flamme... brannte zu hell.",
    "Mi llama... fue demasiado intensa.",
    "Слишком ярок... Был мой огонь."
),
(
    @REF_KAGRAZ, 8, 0,
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t%s invoque des|cFFFF0000 Loups de braise|r !",
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t%s beschwört |cFFFF0000Glutwölfe|r!",
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t¡%s invoca |cFFFF0000lobos de ceniza|r!",
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t%s призывает |cFFFF0000 магматических волков|r!"
),
(
    @REF_KAGRAZ, 9, 0,
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t%s commence à canaliser une |cFFFF0404|Hspell:155493|h[Tempête de feu]|h|r !",
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t%s beginnt einen |cFFFF0404|Hspell:155493|h[Feuersturm]|h|r zu kanalisieren!",
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t¡%s comienza a canalizar |cFFFF0404|Hspell:155493|h[Tormenta de Fuego]|h|r!",
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t%s начинает творить заклинание |cFFFF0404|Hspell:155493|h[Огненная буря]|h|r!"
);

DELETE FROM `reference_loot_template` WHERE entry = @REF_KAGRAZ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAGRAZ, 113913, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113914, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113915, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113916, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113917, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113918, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113919, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113920, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113921, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113922, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113923, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113924, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 113925, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 120077, 0, 1, 1, 1, 1),
(@REF_KAGRAZ, 119193, 0, 1, 1, 1, 1);

# Set tokens
DELETE FROM `reference_loot_template` WHERE entry = @REF_KAGRAZ + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAGRAZ + 1, 119305, 0, 14, 1, 1, 1),
(@REF_KAGRAZ + 1, 119315, 0, 14, 1, 1, 1),
(@REF_KAGRAZ + 1, 119318, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_KAGRAZ WHERE `entry`= @REF_KAGRAZ;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_KAGRAZ;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAGRAZ, 1, 100, 1, 0, -@REF_KAGRAZ, 6),
(@REF_KAGRAZ, 2, 100, 2, 0, -(@REF_KAGRAZ + 1), 2),
(@REF_KAGRAZ, 3, 100, 4, 0, -(@REF_KAGRAZ + 1), 1),
(@REF_KAGRAZ, 4, 100, 8, 0, -(@REF_KAGRAZ + 1), 1);

DELETE FROM creature_groupsizestats WHERE entry = @REF_KAGRAZ;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_KAGRAZ, 14, 10, 53086003),
(@REF_KAGRAZ, 14, 11, 58663046),
(@REF_KAGRAZ, 14, 12, 64240089),
(@REF_KAGRAZ, 14, 13, 69805116),
(@REF_KAGRAZ, 14, 14, 75374933),
(@REF_KAGRAZ, 14, 15, 80949070),
(@REF_KAGRAZ, 14, 16, 86522937),
(@REF_KAGRAZ, 14, 17, 92107697),
(@REF_KAGRAZ, 14, 18, 97680886),
(@REF_KAGRAZ, 14, 19, 103247373),
(@REF_KAGRAZ, 14, 20, 108817562),
(@REF_KAGRAZ, 14, 21, 114392510),
(@REF_KAGRAZ, 14, 22, 119692754),
(@REF_KAGRAZ, 14, 23, 125554690),
(@REF_KAGRAZ, 14, 24, 131130906),
(@REF_KAGRAZ, 14, 25, 136707122),
(@REF_KAGRAZ, 14, 26, 142277844),
(@REF_KAGRAZ, 14, 27, 147861757),
(@REF_KAGRAZ, 14, 28, 153460845),
(@REF_KAGRAZ, 14, 29, 159011207),
(@REF_KAGRAZ, 14, 30, 164561569),
(@REF_KAGRAZ, 15, 11, 70641009),
(@REF_KAGRAZ, 15, 12, 77338648),
(@REF_KAGRAZ, 15, 13, 84060804),
(@REF_KAGRAZ, 15, 14, 90758644),
(@REF_KAGRAZ, 15, 15, 97473710),
(@REF_KAGRAZ, 15, 16, 104193814),
(@REF_KAGRAZ, 15, 17, 110917530),
(@REF_KAGRAZ, 15, 18, 117612579),
(@REF_KAGRAZ, 15, 19, 124342435),
(@REF_KAGRAZ, 15, 20, 131030184),
(@REF_KAGRAZ, 15, 21, 137734114),
(@REF_KAGRAZ, 15, 22, 144465126),
(@REF_KAGRAZ, 15, 23, 151173399),
(@REF_KAGRAZ, 15, 24, 157883487),
(@REF_KAGRAZ, 15, 25, 164621628),
(@REF_KAGRAZ, 15, 26, 171338499),
(@REF_KAGRAZ, 15, 27, 178004718),
(@REF_KAGRAZ, 15, 28, 184709934),
(@REF_KAGRAZ, 15, 29, 191439355),
(@REF_KAGRAZ, 15, 30, 198150808);

SET @REF_AKNOR = 77337;

DELETE FROM creature_groupsizestats WHERE entry = @REF_AKNOR;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_AKNOR, 14, 10, 693750),
(@REF_AKNOR, 14, 11, 937782),
(@REF_AKNOR, 14, 12, 1152534),
(@REF_AKNOR, 14, 13, 1341893),
(@REF_AKNOR, 14, 14, 1721307),
(@REF_AKNOR, 14, 15, 2163384),
(@REF_AKNOR, 14, 16, 2689960),
(@REF_AKNOR, 14, 17, 2709464),
(@REF_AKNOR, 14, 18, 3204997),
(@REF_AKNOR, 14, 19, 3484735),
(@REF_AKNOR, 14, 20, 3564292),
(@REF_AKNOR, 14, 21, 3650649),
(@REF_AKNOR, 14, 22, 3754582),
(@REF_AKNOR, 14, 23, 3971962),
(@REF_AKNOR, 14, 24, 4496226),
(@REF_AKNOR, 14, 25, 4750281),
(@REF_AKNOR, 14, 26, 5659170),
(@REF_AKNOR, 14, 27, 6279397),
(@REF_AKNOR, 14, 28, 6512114),
(@REF_AKNOR, 14, 29, 7055949),
(@REF_AKNOR, 14, 30, 7539357),
(@REF_AKNOR, 15, 11, 3723523),
(@REF_AKNOR, 15, 12, 4077898),
(@REF_AKNOR, 15, 13, 4407097),
(@REF_AKNOR, 15, 14, 4793927),
(@REF_AKNOR, 15, 15, 5143001),
(@REF_AKNOR, 15, 16, 5466795),
(@REF_AKNOR, 15, 17, 5905279),
(@REF_AKNOR, 15, 18, 6192872),
(@REF_AKNOR, 15, 19, 6861811),
(@REF_AKNOR, 15, 20, 6902714),
(@REF_AKNOR, 15, 21, 7222577),
(@REF_AKNOR, 15, 22, 7581530),
(@REF_AKNOR, 15, 23, 7935181),
(@REF_AKNOR, 15, 24, 8398971),
(@REF_AKNOR, 15, 25, 8628995),
(@REF_AKNOR, 15, 26, 8981789),
(@REF_AKNOR, 15, 27, 9335234),
(@REF_AKNOR, 15, 28, 9716375),
(@REF_AKNOR, 15, 29, 10060055),
(@REF_AKNOR, 15, 30, 10389522);

SET @REF_CINDER_WOLF = 76794;

DELETE FROM creature_groupsizestats WHERE entry = @REF_CINDER_WOLF;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_CINDER_WOLF, 14, 10, 2301532),
(@REF_CINDER_WOLF, 14, 11, 2551532),
(@REF_CINDER_WOLF, 14, 12, 2791532),
(@REF_CINDER_WOLF, 14, 13, 3031532),
(@REF_CINDER_WOLF, 14, 14, 3271532),
(@REF_CINDER_WOLF, 14, 15, 3511532),
(@REF_CINDER_WOLF, 14, 16, 3761532),
(@REF_CINDER_WOLF, 14, 17, 4021532),
(@REF_CINDER_WOLF, 14, 18, 4261532),
(@REF_CINDER_WOLF, 14, 19, 4481532),
(@REF_CINDER_WOLF, 14, 20, 4731532),
(@REF_CINDER_WOLF, 14, 21, 4991532),
(@REF_CINDER_WOLF, 14, 22, 5211532),
(@REF_CINDER_WOLF, 14, 23, 5451532),
(@REF_CINDER_WOLF, 14, 24, 5691532),
(@REF_CINDER_WOLF, 14, 25, 5971532),
(@REF_CINDER_WOLF, 14, 26, 6171532),
(@REF_CINDER_WOLF, 14, 27, 6421532),
(@REF_CINDER_WOLF, 14, 28, 6661532),
(@REF_CINDER_WOLF, 14, 29, 6901532),
(@REF_CINDER_WOLF, 14, 30, 7141532),
(@REF_CINDER_WOLF, 15, 10, 2881532),
(@REF_CINDER_WOLF, 15, 11, 3181532),
(@REF_CINDER_WOLF, 15, 12, 3481532),
(@REF_CINDER_WOLF, 15, 13, 3801532),
(@REF_CINDER_WOLF, 15, 14, 4091532),
(@REF_CINDER_WOLF, 15, 15, 4441532),
(@REF_CINDER_WOLF, 15, 16, 4721532),
(@REF_CINDER_WOLF, 15, 17, 5021532),
(@REF_CINDER_WOLF, 15, 18, 5331532),
(@REF_CINDER_WOLF, 15, 19, 5601532),
(@REF_CINDER_WOLF, 15, 20, 5921532),
(@REF_CINDER_WOLF, 15, 21, 6231532),
(@REF_CINDER_WOLF, 15, 22, 6511532),
(@REF_CINDER_WOLF, 15, 23, 6821532),
(@REF_CINDER_WOLF, 15, 24, 7111532),
(@REF_CINDER_WOLF, 15, 25, 7431532),
(@REF_CINDER_WOLF, 15, 26, 7761532),
(@REF_CINDER_WOLF, 15, 27, 8051532),
(@REF_CINDER_WOLF, 15, 28, 8321532),
(@REF_CINDER_WOLF, 15, 29, 8691532),
(@REF_CINDER_WOLF, 15, 30, 8931532);

DELETE FROM conditions WHERE SourceEntry IN (163125, 163093, 155776, 163644, 163634);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 163125, 31, 0, 3, 80677),
(13, 1, 163093, 31, 0, 3, 80683),
(13, 1, 155776, 31, 0, 3, 77043),
(13, 1, 163644, 31, 0, 3, 80683),
(13, 1, 163634, 31, 0, 3, 80947);

DELETE FROM spell_script_names WHERE spell_id IN (163182, 177860, 156038, 154932, 155912, 154961, 154950, 154952, 155493, 168996, 163634, 156689, 155382);
INSERT INTO spell_script_names VALUES
(163182, "spell_foundry_crushing_slam"),
(177860, "spell_foundry_ember_in_the_wind_damage"),
(156038, "spell_foundry_drop_the_hammer_aura"),
(154932, "spell_foundry_molten_torrent_aura"),
(155912, "spell_foundry_allow_molten_torrent_cast"),
(154961, "spell_foundry_fiery_link"),
(154950, "spell_foundry_overheated"),
(154952, "spell_foundry_fixate"),
(155493, "spell_foundry_firestorm_aura"),
(168996, "spell_foundry_firestorm_v2_periodic_lava_stalker"),
(163634, "spell_foundry_firestorm_v2_pick_stalker_to_fire"),
(156689, "spell_foundry_unquenchable_flame_periodic"),
(155382, "spell_foundry_blazing_radiance");

DELETE FROM areatrigger_template WHERE spell_id IN (154915, 155847, 163630);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, flags, scale_x, scale_y, scale_curve_id, ScriptName) VALUES
(154915, 0, 6229, 2, 0x4000, 1, 1, 0, "areatrigger_foundry_lava_slash_pool"),
(155847, 0, 1790, 2, 0x4042, 10, 10, 489, "areatrigger_foundry_molten_torrent_ball"),
(163630, 0, 2476, 2, 0x4046, 5, 5, 958, "");
