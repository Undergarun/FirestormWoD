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
    "Pourceaux incapables ! Remettez-vous au travail. On a un quota à respecter !",
    "Wertloses Schwein! Zurück an die Arbeit! Wir haben hier Vorgaben zu erfüllen!",
    "¡Cerdo inútil! De vuelta al trabajo. ¡Tenemos una meta que cumplir!",
    "Грязная свинья! А ну за работу! Надо план выполнять!"
),
(
    80708, 1, 0,
    "Ne les laissez pas passer !",
    "Lasst sie nicht vorbei!",
    "¡No dejen que nos pasen!",
    "Не позволяйте им приблизиться к нам!"
),
(
    @REF_KAGRAZ, 0, 0,
    "Mon travail. Vous avez interrompu mon TRAVAIL !",
    "Meine Arbeit. Ihr stört meine ARBEIT!",
    "Mi trabajo. ¡Interrumpieron mi TRABAJO!",
    "Работа! Вы отрываете меня от работы!"
),
(
    @REF_KAGRAZ, 1, 0,
    "Aknor ! Tu es la honte de ton clan.",
    "Aknor! Ihr seid eine Schande für Euren Klan.",
    "¡Aknor! Eres una vergüenza para tu clan.",
    "Акнор! Ты – позор для клана"
),
(
    @REF_KAGRAZ, 2, 0,
    "Brûlez !",
    "Brennt!",
    "¡Ardan!",
    "Горите!"
),
(
    @REF_KAGRAZ, 2, 1,
    "Le feu se nourrit de votre chair ",
    "Euer Fleisch nährt das Feuer!",
    "Su carne alimenta el fuego.",
    "Пламя пожрет вашу плоть."
),
(
    @REF_KAGRAZ, 2, 2,
    "Par le fer et la flamme !",
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
    "C'est l'heure du repas !",
    "Jetzt gibt es was zu fressen!",
    "¡Hora de comer!",
    "Пора обедать!"
),
(
    @REF_KAGRAZ, 3, 2,
    "J'ai d'autres amis à vous présenter !",
    "Hier sind noch mehr meiner Freunde!",
    "¡Tengo más amigos para que conozcas!",
    "Познакомьтесь с моими друзьями!"
),
(
    @REF_KAGRAZ, 4, 0,
    "Ça suffit... Vous allez brûler ! VOUS ALLEZ TOUS BRÛLER !",
    "Es reicht  Ihr werdet brennen! IHR WERDET ALLE BRENNEN!",
    "Es suficiente... ¡arderán! ¡TODOS USTEDES ARDERÁN!",
    "Довольно! Вы сгорите! Вы все сгорите!"
),
(
    @REF_KAGRAZ, 5, 0,
    "Une nouvelle victime de ma lame !",
    "Ein weiteres Opfer meiner Klinge!",
    "¡Otro más cae ante mi espada!",
    "Мой клинок разит без промаха!"
),
(
    @REF_KAGRAZ, 5, 1,
    "C'est trop chaud pour vous ?",
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
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t%s invoque des|cFFFF0000 Loups de braise|r !",
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t%s beschwört |cFFFF0000Glutwölfe|r!",
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t¡%s invoca |cFFFF0000lobos de ceniza|r!",
    "|TInterface\\Icons\\item_summon_cinderwolf.blp:20|t%s призывает |cFFFF0000 магматических волков|r!"
),
(
    @REF_KAGRAZ, 9, 0,
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t%s commence à canaliser une |cFFFF0404|Hspell:155493|h[Tempête de feu]|h|r !",
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t%s beginnt einen |cFFFF0404|Hspell:155493|h[Feuersturm]|h|r zu kanalisieren!",
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t¡%s comienza a canalizar |cFFFF0404|Hspell:155493|h[Tormenta de Fuego]|h|r!",
    "|TInterface\\Icons\\spell_shaman_stormearthfire.blp:20|t%s начинает творить заклинание |cFFFF0404|Hspell:155493|h[Огненная буря]|h|r!"
);

DELETE FROM conditions WHERE SourceEntry IN (163125, 163093);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 163125, 31, 0, 3, 80677),
(13, 1, 163093, 31, 0, 3, 80683);

DELETE FROM spell_script_names WHERE spell_id IN (163182, 177860, 156038);
INSERT INTO spell_script_names VALUES
(163182, "spell_foundry_crushing_slam"),
(177860, "spell_foundry_ember_in_the_wind_damage"),
(156038, "spell_foundry_drop_the_hammer_aura");