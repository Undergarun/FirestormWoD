SET NAMES utf8;

DELETE FROM areatrigger_scripts WHERE entry = 9999;
INSERT INTO areatrigger_scripts VALUE
(9999, "areatrigger_at_foundry_hansgar_and_franzok_exit");

DELETE FROM creature_text WHERE entry IN (80708);
INSERT INTO creature_text VALUES
(80708, 0, 0, "You worthless swine! Get back to work. We have a quota to make!", 14, 0, 100, 0, 0, 0, "TaskmasterIntro"),
(80708, 1, 0, "Don't let them get by us!", 14, 0, 100, 0, 0, 0, "TaskmasterOutro");

DELETE FROM locales_creature_text WHERE entry IN (80708);
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
);

DELETE FROM conditions WHERE SourceEntry IN (163125, 163093);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 163125, 31, 0, 3, 80677),
(13, 1, 163093, 31, 0, 3, 80683),
(13, 1, 163093, 31, 0, 3, 80702);

DELETE FROM spell_script_names WHERE spell_id IN (163182);
INSERT INTO spell_script_names VALUES
(163182, "spell_foundry_crushing_slam");