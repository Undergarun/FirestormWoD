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

DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_cauterizing_bolt" AND spell_id = 160140;
DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_delayed_siege_bomb_periodic" AND spell_id = 159481;
INSERT INTO spell_script_names VALUES
(160140, "spell_foundry_cauterizing_bolt"),
(159481, "spell_foundry_delayed_siege_bomb_periodic");

DELETE FROM areatrigger_template WHERE spell_id IN (156553, 165194);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, morph_curve_id, data0, data1, data2, data3, data4, data5, data6, data7, ScriptName) VALUES
(156553, 0, 6756, 1, 1, 2180, 702, 17.5, 14.25, 5, 17.5, 14.25, 5, 0, 0, ""),
(165194, 0, 7282, 5, 5, 16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_prototype_pulse_grenade");

SET @REF_THOGAR = 76906;

UPDATE creature_template SET dmg_multiplier = 10, ScriptName = "boss_operator_thogar", mechanic_immune_mask = 617299839 WHERE entry = @REF_THOGAR;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_gunnery_sergeant" WHERE entry = 81318;
UPDATE creature_template SET ScriptName = "npc_foundry_siege_engine" WHERE entry IN (78982, 81316);

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