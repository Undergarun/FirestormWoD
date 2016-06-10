SET NAMES utf8;

# Thogar trashes
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_man_at_arms" WHERE entry IN (78832, 77687);
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_raider" WHERE entry = 81197;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_crack_shot" WHERE entry = 81315;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_firemender" WHERE entry = 87841;

# Trains
UPDATE creature_template SET unit_flags = unit_flags | 0x2000302, InhabitType = 4, ScriptName = "npc_foundry_train_controller" WHERE entry = 76907;
UPDATE creature_template SET unit_flags = unit_flags | 0x2000302 WHERE entry IN (77181, 89074, 81752);

REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(81197, 1, 107703, 107703, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(87841, 1, 108772, 107801, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(81315, 1, 0, 0, 108426);

DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_cauterizing_bolt_searcher" AND spell_id = 160177;
DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_cauterizing_bolt" AND spell_id = 160140;
DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_delayed_siege_bomb_periodic" AND spell_id = 159481;
DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_berated" AND spell_id = 156281;
INSERT INTO spell_script_names VALUES
(160177, "spell_foundry_cauterizing_bolt_searcher"),
(160140, "spell_foundry_cauterizing_bolt"),
(159481, "spell_foundry_delayed_siege_bomb_periodic"),
(156281, "spell_foundry_berated");

DELETE FROM areatrigger_template WHERE spell_id IN (156553, 165194);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, morph_curve_id, data0, data1, data2, data3, data4, data5, data6, data7, ScriptName) VALUES
(156553, 0, 6756, 1, 1, 2180, 702, 17.5, 14.25, 5, 17.5, 14.25, 5, 0, 0, "areatrigger_foundry_moving_train"),
(165194, 0, 7282, 5, 5, 16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_prototype_pulse_grenade");

SET @REF_THOGAR = 76906;

UPDATE creature_template SET dmg_multiplier = 10, ScriptName = "boss_operator_thogar", mechanic_immune_mask = 617299839 WHERE entry = @REF_THOGAR;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_gunnery_sergeant" WHERE entry IN (78981, 81318);
UPDATE creature_template SET ScriptName = "npc_foundry_siege_engine" WHERE entry IN (78982, 81316);
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_raider" WHERE entry = 77394;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_crack_shot" WHERE entry = 77476;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_firemender" WHERE entry = 77487;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_man_at_arms" WHERE entry = 80791;

DELETE FROM creature_text WHERE entry IN (@REF_THOGAR);
INSERT INTO creature_text VALUES
(@REF_THOGAR, 0, 0, "Track one.", 14, 0, 100, 0, 0, 41697, "Track 1_0"),
(@REF_THOGAR, 0, 1, "Live iron on track one!", 14, 0, 100, 0, 0, 41698, "Track 1_1"),
(@REF_THOGAR, 1, 0, "Track two!", 14, 0, 100, 0, 0, 41699, "Track 2_0"),
(@REF_THOGAR, 1, 1, "Incoming on two.", 14, 0, 100, 0, 0, 41700, "Track 2_1"),
(@REF_THOGAR, 2, 0, "Track three.", 14, 0, 100, 0, 0, 41701, "Track 3_0"),
(@REF_THOGAR, 2, 1, "Track three inbound.", 14, 0, 100, 0, 0, 41702, "Track 3_1"),
(@REF_THOGAR, 3, 0, "Track four.", 14, 0, 100, 0, 0, 41703, "Track 4_0"),
(@REF_THOGAR, 3, 1, "Inbound on four.", 14, 0, 100, 0, 0, 41704, "Track 4_1"),
(@REF_THOGAR, 4, 0, "Express, coming through.", 14, 0, 100, 0, 0, 41694, "Express 0"),
(@REF_THOGAR, 4, 1, "Redball incoming!", 14, 0, 100, 0, 0, 41693, "Express 1"),
(@REF_THOGAR, 4, 2, "Send'er on down the line!", 14, 0, 100, 0, 0, 41695, "Express 2"),
(@REF_THOGAR, 4, 3, "Faster! Bat the stack off her!", 14, 0, 100, 0, 0, 41696, "Express 3"),
(@REF_THOGAR, 5, 0, "Blackhand wants to see these shipments in Talador. NOW!", 14, 0, 100, 0, 0, 41709, "Intro"),
(@REF_THOGAR, 6, 0, "Blackhand won't tolerate anymore delays.", 14, 0, 100, 0, 0, 41670, "Aggro"),
(@REF_THOGAR, 7, 0, "Ah - reinforcements.", 14, 0, 100, 0, 0, 41705, "Reinforcements 0"),
(@REF_THOGAR, 7, 1, "Reinforcements, right on time.", 14, 0, 100, 0, 0, 41706, "Reinforcements 1"),
(@REF_THOGAR, 7, 2, "Troop train - inbound!", 14, 0, 100, 0, 0, 41707, "Reinforcements 2"),
(@REF_THOGAR, 7, 3, "Here they come - hit the grit, boys!", 14, 0, 100, 0, 0, 41708, "Reinforcements 3"),
(@REF_THOGAR, 8, 0, "Coming in hot.", 14, 0, 100, 0, 0, 41672, "Iron Gunnery Sergeant 0"),
(@REF_THOGAR, 8, 1, "Here come the boomers!", 14, 0, 100, 0, 0, 41674, "Iron Gunnery Sergeant 1"),
(@REF_THOGAR, 8, 2, "Here's my artillery.", 14, 0, 100, 0, 0, 41673, "Iron Gunnery Sergeant 2"),
(@REF_THOGAR, 9, 0, "The command car is here.", 14, 0, 100, 0, 0, 41675, "Grom'kar Man-At-Arms 0"),
(@REF_THOGAR, 9, 1, "Here comes the brass.", 14, 0, 100, 0, 0, 41676, "Grom'kar Man-At-Arms 1"),
(@REF_THOGAR, 10, 0, "Trains inbound!", 14, 0, 100, 0, 0, 41683, "Triple Train (Mythic only) 0"),
(@REF_THOGAR, 10, 1, "Double time.", 14, 0, 100, 0, 0, 41684, "Triple Train (Mythic only) 1"),
(@REF_THOGAR, 10, 2, "Clear the tracks!", 14, 0, 100, 0, 0, 41685, "Triple Train (Mythic only) 2"),
(@REF_THOGAR, 11, 0, "Let's step up the pace!", 14, 0, 100, 0, 0, 41678, "Deforester Trains (Mythic only) 0"),
(@REF_THOGAR, 11, 1, "You're just in time for the rush!", 14, 0, 100, 0, 0, 41679, "Deforester Trains (Mythic only)) 1"),
(@REF_THOGAR, 11, 2, "I'm not impressed - more trains are inbound!", 14, 0, 100, 0, 0, 41680, "Deforester Trains (Mythic only) 2"),
(@REF_THOGAR, 11, 3, "I have a schedule to keep!", 14, 0, 100, 0, 0, 41681, "Deforester Trains (Mythic only) 3"),
(@REF_THOGAR, 11, 4, "You're running out of time.", 14, 0, 100, 0, 0, 41682, "Deforester Trains (Mythic only) 4"),
(@REF_THOGAR, 12, 0, "Right. On. Time.", 14, 0, 100, 0, 0, 41692, "Killing a player 0"),
(@REF_THOGAR, 12, 1, "All aboard.", 14, 0, 100, 0, 0, 41691, "Killing a player 1"),
(@REF_THOGAR, 12, 2, "Terminated!", 14, 0, 100, 0, 0, 41689, "Killing a player 2"),
(@REF_THOGAR, 12, 3, "End of the line.", 14, 0, 100, 0, 0, 41688, "Killing a player 3"),
(@REF_THOGAR, 12, 4, "Hah, flat as a copper piece.", 14, 0, 100, 0, 0, 41690, "Killing a player 4"),
(@REF_THOGAR, 13, 0, "Its the end of the line!", 14, 0, 100, 0, 0, 41671, "Berserk"),
(@REF_THOGAR, 14, 0, "Couldn't keep up, eh?", 14, 0, 100, 0, 0, 41686, "Wipe 0"),
(@REF_THOGAR, 14, 1, "Hah hah, this timetable is a killer!", 14, 0, 100, 0, 0, 41687, "Wipe 1"),
(@REF_THOGAR, 15, 0, "That wasn't on the... schedule...", 14, 0, 100, 0, 0, 41677, "Death");

DELETE FROM locales_creature_text WHERE entry IN (@REF_THOGAR);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_THOGAR, 0, 0,
    "Voie une.",
    "Gleis eins.",
    "Vía uno.",
    "Первый путь."
),
(
    @REF_THOGAR, 0, 1,
    "Voie une en service !",
    "Unterwegs auf Gleis eins!",
    "¡Carga pesada en vía uno!",
    "Поезд на первом пути!"
),
(
    @REF_THOGAR, 1, 0,
    "Voie deux !",
    "Gleis zwei!",
    "¡Vía dos!",
    "Второй путь!"
),
(
    @REF_THOGAR, 1, 1,
    "Voie deux, train en approche.",
    "Bahn kommt auf zwei.",
    "Aproximándose en la dos.",
    "Прибывает на второй."
),
(
    @REF_THOGAR, 2, 0,
    "Voie trois.",
    "Gleis drei.",
    "Vía tres.",
    "Третий путь."
),
(
    @REF_THOGAR, 2, 1,
    "Train entrant sur la voie trois.",
    "Zug auf Gleis drei.",
    "Vía tres de llegada.",
    "Прибытие на третий."
),
(
    @REF_THOGAR, 3, 0,
    "Voie quatre !",
    "Gleis vier!",
    "¡Vía cuatro!",
    "Четвертый путь!"
),
(
    @REF_THOGAR, 3, 1,
    "Arrivage sur la voie quatre.",
    "Zug kommt. Gleis vier.",
    "Llegada en la cuatro.",
    "Поезд на четвертом."
),
(
    @REF_THOGAR, 4, 0,
    "Train express en approche, libérez le passage !",
    "Expressdurchfahrt.",
    "Expreso en camino.",
    "Скорый поезд!"
),
(
    @REF_THOGAR, 4, 1,
    "Train rapide en approche !",
    "Schnellzug hat Vorrang!",
    "¡Se aproxima uno a toda velocidad!",
    "На всех парах!"
),
(
    @REF_THOGAR, 4, 2,
    "Train express, pas d'arrêt !",
    "Weg mit Euch, der Zug muss hier durch!",
    "¡Envíenlo por la vía!",
    "Поезд идет без остановки!"
),
(
    @REF_THOGAR, 4, 3,
    "Plus vite ! Encore plus vite !",
    "Schneller! Das geht doch noch schneller!",
    "¡Más rápido! ¡Tan rápido como puedas!",
    "Быстрей! Подкиньте угля!"
),
(
    @REF_THOGAR, 5, 0,
    "Main-Noire veut que ces cargaisons soient livrées en Talador. MAINTENANT !",
    "Schwarzfaust braucht diese Lieferungen in Talador. Und zwar JETZT!",
    "Puño Negro quiere ver estos envíos en Talador. ¡AHORA!",
    "Шевелитесь! Чернорук ждет эти поставки в Таладоре!"
),
(
    @REF_THOGAR, 6, 0,
    "Main-Noire ne tolérera pas un retard de plus.",
    "Schwarzfaust wird keine Verzögerungen mehr tolerieren.",
    "Puño Negro no tolerará más retrasos.",
    "Чернорук не потерпит новых задержек."
),
(
    @REF_THOGAR, 7, 0,
    "Ah, des renforts.",
    "Ah  Verstärkung.",
    "Ah... refuerzos.",
    "А, подкрепление."
),
(
    @REF_THOGAR, 7, 1,
    "Des renforts, juste au bon moment.",
    "Verstärkung! Gerade rechtzeitig!",
    "Refuerzos, justo a tiempo.",
    "Подкрепление, как раз вовремя."
),
(
    @REF_THOGAR, 7, 2,
    "Train de soldats en approche !",
    "Truppentransport nähert sich!",
    "¡Tren de tropas llegando!",
    "Войска прибыли!"
),
(
    @REF_THOGAR, 7, 3,
    "Les voilà ! Sortez de là, les gars !",
    "Da seid Ihr ja  los geht's, Jungs!",
    "Aquí vienen... ¡salgan de ahí, chicos!",
    "Вот и они – выгружайтесь, парни!"
),
(
    @REF_THOGAR, 8, 0,
    "Attention, ça va péter !",
    "Heißes Eisen!",
    "Descargando fuego.",
    "Добавим огня!"
),
(
    @REF_THOGAR, 8, 1,
    "Voilà le bataillon d'artillerie !",
    "Die Kracher sind da!",
    "¡Aquí vienen los explosivos!",
    "Огневая поддержка!"
),
(
    @REF_THOGAR, 8, 2,
    "Voilà mon artillerie.",
    "Hier ist meine Artillerie.",
    "Aquí está mi artillería.",
    "Вот и артиллерия."
),
(
    @REF_THOGAR, 9, 0,
    "La voiture de commandement est ici.",
    "Der Befehlswagon ist eingetroffen.",
    "El vehículo militar llegó.",
    "Прибыл командный поезд."
),
(
    @REF_THOGAR, 9, 1,
    "Voilà les gradés.",
    "Hier kommt der Stab.",
    "Aquí viene el líder.",
    "А вот и командиры."
),
(
    @REF_THOGAR, 10, 0,
    "Trains en approche !",
    "Züge kommen!",
    "¡Trenes llegando!",
    "Поезда прибывают!"
),
(
    @REF_THOGAR, 10, 1,
    "Arrivée simultanée.",
    "Beeilung!",
    "Apresúrense.",
    "Больше поездов!"
),
(
    @REF_THOGAR, 10, 2,
    "Dégagez les voies !",
    "Runter von den Schienen!",
    "¡Despejen las vías!",
    "Очистить пути!"
),
(
    @REF_THOGAR, 11, 0,
    "Augmentons un peu la cadence.",
    "Machen wir mal ein wenig Dampf.",
    "Subamos el ritmo.",
    "Прибавим скорость."
),
(
    @REF_THOGAR, 11, 1,
    "Juste à temps pour l'heure de pointe.",
    "Gerade rechtzeitig zur Stoßzeit.",
    "Justo a tiempo para acelerar.",
    "Вот-вот начнется час пик."
),
(
    @REF_THOGAR, 11, 2,
    "Ça ne m'impressionne pas. D'autres trains arrivent !",
    "Ich bin nicht beeindruckt. Mehr Züge!",
    "No estoy impresionado. ¡Más trenes en camino!",
    "Не впечатляет. Прибывают еще поезда!"
),
(
    @REF_THOGAR, 11, 3,
    "J'ai des horaires à respecter !",
    "Ich habe einen Zeitplan einzuhalten!",
    "¡Tengo un horario qué seguir!",
    "Не выбиваться из расписания!"
),
(
    @REF_THOGAR, 11, 4,
    "Vous n'avez presque plus de temps...",
    "Euch läuft die Zeit davon...",
    "Se les acaba el tiempo...",
    "Времени все меньше..."
),
(
    @REF_THOGAR, 12, 0,
    "Pile. À. L'heure.",
    "Pünktlich wie die Eisenbahn.",
    "Justo. A. Tiempo.",
    "Точно... по... графику."
),
(
    @REF_THOGAR, 12, 1,
    "Tout le monde à bord !",
    "Alle an Bord?",
    "¡Todos a bordo!",
    "Занимаем места!"
),
(
    @REF_THOGAR, 12, 2,
    "Voilà ce qui arrive aux retardataires !",
    "Alles aussteigen!",
    "¡Terminado!",
    "Не стой на путях!"
),
(
    @REF_THOGAR, 12, 3,
    "Terminus.",
    "Endstation.",
    "Final de la línea.",
    "Все, приехали."
),
(
    @REF_THOGAR, 12, 4,
    "Ha ! Aplati comme une crêpe !",
    "Platt wie eine Kupfermünze.",
    "Ja, plano como una moneda de cobre.",
    "Сплющило, как медный грош."
),
(
    @REF_THOGAR, 13, 0,
    "TERMINUS, tout le monde descend !",
    "Dies ist Eure ENDSTATION!",
    "¡Es el FINAL de la LÍNEA!",
    "Конечная! Освободить вагоны!"
),
(
    @REF_THOGAR, 14, 0,
    "Difficile de suivre, hein ?",
    "Konntet wohl nicht mithalten, was?",
    "No pudieron con ello, ¿eh?",
    "Не угнаться, да?"
),
(
    @REF_THOGAR, 14, 1,
    "Haha, vous avez un train de retard !",
    "Haha, dieser Fahrplan ist tödlich!",
    "¡Ja, ja, este horario es para morirse!",
    "Ха-ха, ваш поезд ушел!"
),
(
    @REF_THOGAR, 15, 0,
    "Un train... peut en cacher un autre...",
    "Das stand nicht auf dem... Fahrplan...",
    "Eso no estaba... programado...",
    "В расписании... этого не было..."
);

DELETE FROM `reference_loot_template` WHERE entry = @REF_THOGAR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_THOGAR, 113953, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113954, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113955, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113956, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113957, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113958, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113959, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113960, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113961, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113962, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113963, 0, 1, 1, 1, 1),
(@REF_THOGAR, 113964, 0, 1, 1, 1, 1);

# Set tokens
DELETE FROM `reference_loot_template` WHERE entry = @REF_THOGAR + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_THOGAR + 1, 119309, 0, 14, 1, 1, 1),
(@REF_THOGAR + 1, 119314, 0, 14, 1, 1, 1),
(@REF_THOGAR + 1, 119322, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_THOGAR WHERE `entry`= @REF_THOGAR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_THOGAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_THOGAR, 1, 100, 1, 0, -@REF_THOGAR, 6),
(@REF_THOGAR, 2, 100, 2, 0, -(@REF_THOGAR + 1), 2),
(@REF_THOGAR, 3, 100, 4, 0, -(@REF_THOGAR + 1), 1),
(@REF_THOGAR, 4, 100, 8, 0, -(@REF_THOGAR + 1), 1);

DELETE FROM creature_groupsizestats WHERE entry = @REF_THOGAR;
INSERT INTO creature_groupsizestats VALUES
(@REF_THOGAR, 14, 10, 47094109),
(@REF_THOGAR, 14, 11, 52033079),
(@REF_THOGAR, 14, 12, 56975672),
(@REF_THOGAR, 14, 13, 61934505),
(@REF_THOGAR, 14, 14, 66871101),
(@REF_THOGAR, 14, 15, 71828326),
(@REF_THOGAR, 14, 16, 76821657),
(@REF_THOGAR, 14, 17, 81716706),
(@REF_THOGAR, 14, 18, 86649498),
(@REF_THOGAR, 14, 19, 91610886),
(@REF_THOGAR, 14, 20, 96530680),
(@REF_THOGAR, 14, 21, 101473184),
(@REF_THOGAR, 14, 22, 106477613),
(@REF_THOGAR, 14, 23, 111361857),
(@REF_THOGAR, 14, 24, 116309341),
(@REF_THOGAR, 14, 25, 121245511),
(@REF_THOGAR, 14, 26, 126195424),
(@REF_THOGAR, 14, 27, 131166262),
(@REF_THOGAR, 14, 28, 136132126),
(@REF_THOGAR, 14, 29, 141048388),
(@REF_THOGAR, 14, 30, 145983877),
(@REF_THOGAR, 15, 10, 58870264),
(@REF_THOGAR, 15, 11, 65040585),
(@REF_THOGAR, 15, 12, 71296699),
(@REF_THOGAR, 15, 13, 77410563),
(@REF_THOGAR, 15, 14, 83610317),
(@REF_THOGAR, 15, 15, 89757635),
(@REF_THOGAR, 15, 16, 95936013),
(@REF_THOGAR, 15, 17, 102138644),
(@REF_THOGAR, 15, 18, 108340729),
(@REF_THOGAR, 15, 19, 114637852),
(@REF_THOGAR, 15, 20, 120673522),
(@REF_THOGAR, 15, 21, 126846817),
(@REF_THOGAR, 15, 22, 133041340),
(@REF_THOGAR, 15, 23, 139196208),
(@REF_THOGAR, 15, 24, 145429983),
(@REF_THOGAR, 15, 25, 151606844),
(@REF_THOGAR, 15, 26, 157793861),
(@REF_THOGAR, 15, 27, 163921987),
(@REF_THOGAR, 15, 28, 170098463),
(@REF_THOGAR, 15, 29, 176293574),
(@REF_THOGAR, 15, 30, 182484327);

DELETE FROM creature_groupsizestats WHERE entry = 77394;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(77394, 14, 10, 484209),
(77394, 14, 11, 534806),
(77394, 14, 12, 600091),
(77394, 14, 13, 608685),
(77394, 14, 14, 657761),
(77394, 14, 15, 703574),
(77394, 14, 16, 753352),
(77394, 14, 17, 802176),
(77394, 14, 18, 858911),
(77394, 14, 19, 932952),
(77394, 14, 20, 945227),
(77394, 14, 21, 994799),
(77394, 14, 22, 1040821),
(77394, 14, 23, 1089894),
(77394, 14, 24, 1138678),
(77394, 14, 25, 1191729),
(77394, 14, 26, 1233847),
(77394, 14, 27, 1287559),
(77394, 14, 28, 1335605),
(77394, 14, 29, 1397923),
(77394, 14, 30, 1429300),
(77394, 15, 10, 568935),
(77394, 15, 11, 639131),
(77394, 15, 12, 707281),
(77394, 15, 13, 758086),
(77394, 15, 14, 819755),
(77394, 15, 15, 877667),
(77394, 15, 16, 942141),
(77394, 15, 17, 1007340),
(77394, 15, 18, 1058991),
(77394, 15, 19, 1184353),
(77394, 15, 20, 1214742),
(77394, 15, 21, 1241924),
(77394, 15, 22, 1331244),
(77394, 15, 23, 1362924),
(77394, 15, 24, 1423157),
(77394, 15, 25, 1491457),
(77394, 15, 26, 1564891),
(77394, 15, 27, 1614869),
(77394, 15, 28, 1665964),
(77394, 15, 29, 1727370),
(77394, 15, 30, 1788761);

DELETE FROM creature_groupsizestats WHERE entry = 77476;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(77476, 14, 10, 427425),
(77476, 14, 11, 451011),
(77476, 14, 12, 534528),
(77476, 14, 13, 552943),
(77476, 14, 14, 582081),
(77476, 14, 15, 618410),
(77476, 14, 16, 667501),
(77476, 14, 17, 706191),
(77476, 14, 18, 748266),
(77476, 14, 19, 809805),
(77476, 14, 20, 833275),
(77476, 14, 21, 872961),
(77476, 14, 22, 914684),
(77476, 14, 23, 959388),
(77476, 14, 24, 1010106),
(77476, 14, 25, 1067286),
(77476, 14, 26, 1128941),
(77476, 14, 27, 1173593),
(77476, 14, 28, 1218079),
(77476, 14, 29, 1257095),
(77476, 14, 30, 1327125),
(77476, 15, 10, 518487),
(77476, 15, 11, 560276),
(77476, 15, 12, 614243),
(77476, 15, 13, 667883),
(77476, 15, 14, 723081),
(77476, 15, 15, 776206),
(77476, 15, 16, 825375),
(77476, 15, 17, 885512),
(77476, 15, 18, 933924),
(77476, 15, 19, 995480),
(77476, 15, 20, 1040593),
(77476, 15, 21, 1092627),
(77476, 15, 22, 1144686),
(77476, 15, 23, 1201879),
(77476, 15, 24, 1342791),
(77476, 15, 25, 1323764),
(77476, 15, 26, 1366254),
(77476, 15, 27, 1404843),
(77476, 15, 28, 1469061),
(77476, 15, 29, 1521387),
(77476, 15, 30, 1610065);

DELETE FROM creature_groupsizestats WHERE entry = 78981;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(78981, 14, 10, 497983),
(78981, 14, 11, 554988),
(78981, 14, 12, 608539),
(78981, 14, 13, 640450),
(78981, 14, 14, 696012),
(78981, 14, 15, 741976),
(78981, 14, 16, 817218),
(78981, 14, 17, 845151),
(78981, 14, 18, 895436),
(78981, 14, 19, 947280),
(78981, 14, 20, 1029961),
(78981, 14, 21, 1051419),
(78981, 14, 22, 1184964),
(78981, 14, 23, 1170488),
(78981, 14, 24, 1200323),
(78981, 14, 25, 1287029),
(78981, 14, 26, 1342984),
(78981, 14, 27, 1323024),
(78981, 14, 28, 1423516),
(78981, 14, 29, 1461101),
(78981, 14, 30, 1522258),
(78981, 15, 10, 658463),
(78981, 15, 11, 706463),
(78981, 15, 12, 737185),
(78981, 15, 13, 892847),
(78981, 15, 14, 923341),
(78981, 15, 15, 951040),
(78981, 15, 16, 1077334),
(78981, 15, 17, 1054394),
(78981, 15, 18, 1119714),
(78981, 15, 19, 1204613),
(78981, 15, 20, 1247331),
(78981, 15, 21, 1326004),
(78981, 15, 22, 1377788),
(78981, 15, 23, 1470338),
(78981, 15, 24, 1525949),
(78981, 15, 25, 1566880),
(78981, 15, 26, 1624684),
(78981, 15, 27, 1715949),
(78981, 15, 28, 1833786),
(78981, 15, 29, 1855985),
(78981, 15, 30, 1886270);

DELETE FROM creature_groupsizestats WHERE entry = 77487;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(77487, 14, 10, 1621315),
(77487, 14, 11, 1865638),
(77487, 14, 12, 1960526),
(77487, 14, 13, 2474720),
(77487, 14, 14, 2930775),
(77487, 14, 15, 3534448),
(77487, 14, 16, 3677757),
(77487, 14, 17, 4019575),
(77487, 14, 18, 4021962),
(77487, 14, 19, 4190279),
(77487, 14, 20, 4466763),
(77487, 14, 21, 4512213),
(77487, 14, 22, 4891044),
(77487, 14, 23, 5009880),
(77487, 14, 24, 5162031),
(77487, 14, 25, 5550787),
(77487, 14, 26, 6257082),
(77487, 14, 27, 7072872),
(77487, 14, 28, 7236767),
(77487, 14, 29, 8869029),
(77487, 14, 30, 8877713),
(77487, 15, 10, 2154697),
(77487, 15, 11, 2470877),
(77487, 15, 12, 2673930),
(77487, 15, 13, 3252980),
(77487, 15, 14, 4790416),
(77487, 15, 15, 5139058),
(77487, 15, 16, 5641557),
(77487, 15, 17, 5770006),
(77487, 15, 18, 5858554),
(77487, 15, 19, 5961591),
(77487, 15, 20, 6065890),
(77487, 15, 21, 6221543),
(77487, 15, 22, 6522326),
(77487, 15, 23, 7375457),
(77487, 15, 24, 7642401),
(77487, 15, 25, 7768997),
(77487, 15, 26, 7866533),
(77487, 15, 27, 8049428),
(77487, 15, 28, 8594499),
(77487, 15, 29, 9028333),
(77487, 15, 30, 9455751);

DELETE FROM creature_groupsizestats WHERE entry = 80791;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(80791, 14, 10, 2849700),
(80791, 14, 11, 3109800),
(80791, 14, 12, 3349900),
(80791, 14, 13, 3420000),
(80791, 14, 14, 3680100),
(80791, 14, 15, 4510200),
(80791, 14, 16, 4580300),
(80791, 14, 17, 5140400),
(80791, 14, 18, 5570500),
(80791, 14, 19, 5750600),
(80791, 14, 20, 5990700),
(80791, 14, 21, 6270800),
(80791, 14, 22, 6610900),
(80791, 14, 23, 6851000),
(80791, 14, 24, 7051100),
(80791, 14, 25, 7251200),
(80791, 14, 26, 7621300),
(80791, 14, 27, 8131400),
(80791, 14, 28, 8461500),
(80791, 14, 29, 8821600),
(80791, 14, 30, 9521700),
(80791, 15, 10, 2944100),
(80791, 15, 11, 3804200),
(80791, 15, 12, 3874400),
(80791, 15, 13, 4474300),
(80791, 15, 14, 4514500),
(80791, 15, 15, 5114800),
(80791, 15, 16, 5134900),
(80791, 15, 17, 5254700),
(80791, 15, 18, 5795100),
(80791, 15, 19, 6385200),
(80791, 15, 20, 6535700),
(80791, 15, 21, 6724600),
(80791, 15, 22, 6965400),
(80791, 15, 23, 7135500),
(80791, 15, 24, 7525300),
(80791, 15, 25, 7806000),
(80791, 15, 26, 8205800),
(80791, 15, 27, 8515900),
(80791, 15, 28, 8565000),
(80791, 15, 29, 9515600),
(80791, 15, 30, 10076100);