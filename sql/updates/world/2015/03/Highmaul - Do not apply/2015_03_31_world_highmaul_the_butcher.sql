DELETE FROM creature_text WHERE entry IN (83377, 83378, 80048, 83268, 78714, 78926);
INSERT INTO creature_text VALUES
(83377, 0, 0, 'Come here you scrawny grabbers!', 14, 0, 100, 0, 0, 41631, 'TheButcherIntro1'),
(83377, 1, 0, 'All crunch and no meat!', 14, 0, 100, 0, 0, 41632, 'TheButcherIntro2'),
(83377, 2, 0, 'Gonna make a Pale stew.', 14, 0, 100, 0, 0, 41633, 'TheButcherIntro3'),
(83377, 3, 0, 'Come and get it!', 14, 0, 100, 0, 0, 41626, 'TheButcherAggro'),
(83377, 4, 0, 'Get the blood flowing!', 14, 0, 100, 0, 0, 41637, 'TheButcherBoundingCleave'),
(83377, 5, 0, 'Time for the meat grinder!', 14, 0, 100, 0, 0, 41628, 'TheButcherFrenzy'),
(83377, 6, 0, 'Just a slice off the top.', 14, 0, 100, 0, 0, 41636, 'TheButcherCleave'),
(83377, 7, 0, 'I will break you!', 14, 0, 100, 0, 0, 41627, 'TheButcherBerserk'),
(83377, 8, 0, 'So juicy!', 14, 0, 100, 0, 0, 41630, 'TheButcherSlay1'),
(83377, 9, 0, 'Give it to us raw... and wiggling!', 14, 0, 100, 0, 0, 41629, 'TheButcherSlay2'),
(83377, 10, 0, 'Looks like meat''s back on the menu!', 14, 0, 100, 0, 0, 41639, 'TheButcherWipe'),
(83377, 11, 0, 'Lanky, gamey, little runts!', 14, 0, 100, 0, 0, 41639, ''),
(83377, 12, 0, 'Get off! You ain''t worth eatin''.', 14, 0, 100, 0, 0, 41639, ''),
(83377, 13, 0, 'Smash!', 14, 0, 100, 0, 0, 41639, '');

DELETE FROM locales_creature_text WHERE entry IN (83377, 83378, 80048, 83268, 78714, 78926);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    83377, 0, 0,
    'Venez donc, bande de casse-pieds maigrichons !',
    'Kommt her, ihr mickrigen Raffkes.',
    '¡Vengan acá, esqueléticos!',
    'Сюда, паршивцы!'
),
(
    83377, 1, 0,
    'Plein d’os et pas de viande.',
    'Nur Haut und Knochen.',
    'Todos crujientes y sin carne.',
    'Мяса нет, жилы одни.'
),
(
    83377, 2, 0,
    'Vous ferez un ragoût bien fade.',
    'Das gibt leckeren Fahleneintopf.',
    'Haré un estofado insulso.',
    'Бледное выйдет жаркое.'
),
(
    83377, 3, 0,
    'Venez par ici !',
    'Versucht es doch!',
    '¡Vengan!',
    'Подходи по одному!'
),
(
    83377, 4, 0,
    'Je vais vous saigner !',
    'Lasst das Blut fließen!',
    '¡Que la sangre corra!',
    'Выпустить всю кровь!'
),
(
    83377, 5, 0,
    'Il est temps de vous passer au hachoir !',
    'Zeit für den Fleischwolf!',
    '¡Hora de triturar la carne!',
    'Пускаю в дело мясорубку!'
),
(
    83377, 6, 0,
    'Juste une petite tranche !',
    'Nur eine Scheibe oben abschneiden!',
    '¡Solo una rebanada de arriba!',
    'Я только макушку срежу!'
),
(
    83377, 7, 0,
    'Je briserai tous vos os !',
    'ICH WERDE EUCH BRECHEN!',
    '¡LOS APLASTARÉ!',
    'Я ВАС ЗАЛОМАЮ!'
),
(
    83377, 8, 0,
    'De la viande juteuse !',
    'So saftig!',
    '¡Qué jugoso!',
    'Сочно!'
),
(
    83377, 9, 0,
    'Je les préfère crus et frétillants !',
    'Gib ihn uns roh und zappelnd!',
    '¡Dánoslo, crudo y retorciéndose!',
    'Дай его нам живого, трепыхающегося!'
),
(
    83377, 10, 0,
    'Il y a à nouveau de la viande au menu !',
    'Ab sofort steht wieder Fleisch auf dem Speiseplan!',
    '¡Parece que de nuevo hay carne en el menú!',
    'Мясо снова в меню!'
),
(
    83377, 11, 0,
    'Avortons rachitiques et puants !',
    'Schlaksige, zähe Kümmerlinge...',
    'Desagradables insignificantes...',
    'Тощая, вонючая мелюзга...'
),
(
    83377, 12, 0,
    'Dégagez ! Vous ne valez même pas d''être mangés !',
    'Runter mit euch! Ihr seid kaum eine Mahlzeit wert!',
    '¡Basta! ¡No vale la pena comerlos!',
    'Пшли вон! В вас есть нечего!'
),
(
    83377, 13, 0,
    'Je vous écraserai !',
    'Zerschmettern!',
    '¡Machaaaaque!',
    'Шмя-а-а-ак!'
);