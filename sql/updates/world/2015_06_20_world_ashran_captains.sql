SET NAMES utf8;

DELETE FROM creature_text WHERE entry IN (80499, 80484, 79902, 80498, 80488, 85129, 80495, 85140, 85137, 80497, 80490, 80493, 80492, 80491, 79900, 80489, 80486, 80496, 87690, 85138, 88109);
INSERT INTO creature_text VALUES
(80499, 0, 0, 'To the face!', 14, 0, 100, 0, 0, 0, 'AvengerTurleySlay1'),
(80499, 0, 1, 'Eat dirt!', 14, 0, 100, 0, 0, 0, 'AvengerTurleySlay2'),
(80499, 1, 0, 'Hmm, I never had this problem in Alterac Valley.', 14, 0, 100, 0, 0, 0, 'AvengerTurleyDeath'),

(80484, 0, 0, 'Fury Bajheera has been unleashed!', 14, 0, 100, 0, 0, 0, 'JacksonBajheeraSpawn'),
(80484, 1, 0, 'Failure is not an option, I will return, after training.', 14, 0, 100, 0, 0, 0, 'JacksonBajheeraDeath'),

(79902, 0, 0, 'Hey everyone, John Swifty here!', 14, 0, 100, 0, 0, 0, 'JohnSwiftySpawn'),
(79902, 1, 0, 'I only had one shot.', 14, 0, 100, 0, 0, 0, 'JohnSwiftyDeath'),

(80498, 0, 0, 'My return will be your end.', 14, 0, 100, 0, 0, 0, 'BrockTheCrazedDeath'),

(80488, 0, 0, 'The power of sun and moon flow through me with a deafening boom!', 14, 0, 100, 0, 0, 0, 'AluneWindmaneSpawn'),
(80488, 1, 0, 'BOOM!', 14, 0, 100, 0, 0, 0, 'AluneWindmaneKill1'),
(80488, 1, 1, 'KA-BOOM!', 14, 0, 100, 0, 0, 0, 'AluneWindmaneKill2'),
(80488, 1, 2, 'BA-BA-BOOM!', 14, 0, 100, 0, 0, 0, 'AluneWindmaneKill3'),

(85129, 0, 0, 'KIIILLLLLLLL!!!!', 14, 0, 100, 0, 0, 0, 'ChaniMalflameKill1'),
(85129, 0, 1, 'DIIIIIEEEEE!', 14, 0, 100, 0, 0, 0, 'ChaniMalflameKill2'),
(85129, 0, 2, 'DEAAATTTH!!!', 14, 0, 100, 0, 0, 0, 'ChaniMalflameKill3'),
(85129, 1, 0, 'Nooooo!', 14, 0, 100, 0, 0, 0, 'ChaniMalflameDeath'),

(80495, 0, 0, 'Fear the power of the gnomes!', 14, 0, 100, 0, 0, 0, 'HildieHackerguardKill1'),
(80495, 0, 1, 'Gnome power!', 14, 0, 100, 0, 0, 0, 'HildieHackerguardKill2'),
(80495, 0, 2, 'Small and sneaky!', 14, 0, 100, 0, 0, 0, 'HildieHackerguardKill3'),
(80495, 1, 0, 'Just don''t touch my pigtails please...', 14, 0, 100, 0, 0, 0, 'HildieHackerguardDeath'),

(85140, 0, 0, 'I am unimpressed.', 14, 0, 100, 0, 0, 0, 'AnneOttherKill1'),
(85140, 0, 1, 'You had no chance.', 14, 0, 100, 0, 0, 0, 'AnneOttherKill2'),
(85140, 1, 0, 'This is not the end.', 14, 0, 100, 0, 0, 0, 'AnneOttherDeath'),

(85137, 0, 0, 'Fear not Alliance, for fear is now your ally.', 14, 0, 100, 0, 0, 0, 'MathiasZunnSpawn'),
(85137, 1, 0, 'Defeated but not destroyed.', 14, 0, 100, 0, 0, 0, 'MathiasZunnDeath'),

(80497, 0, 0, 'I bring the gift of death to the Alliance.', 14, 0, 100, 0, 0, 0, 'LordMesSpawn'),
(80497, 1, 0, 'The hour belongs to my enemy but I will return when the odds are in my favor.', 14, 0, 100, 0, 0, 0, 'LordMesDeath'),

(80490, 0, 0, 'Are you prepared to meet Azeroth''s greatest champion?!', 14, 0, 100, 0, 0, 0, 'MindbenderTalbadarSpawn'),

(80493, 0, 0, 'Shattered!', 14, 0, 100, 0, 0, 0, 'ElliotVanRookSlay1'),
(80493, 0, 1, 'Free win!', 14, 0, 100, 0, 0, 0, 'ElliotVanRookSlay2'),
(80493, 1, 0, 'Down goes Van Rook eeeeeeeee....', 14, 0, 100, 0, 0, 0, 'ElliotVanRookDeath'),

(80492, 0, 0, 'The day has come for true retribution!', 14, 0, 100, 0, 0, 0, 'VanguardSamuelleKill1'),
(80492, 0, 1, 'Return to the light!', 14, 0, 100, 0, 0, 0, 'VanguardSamuelleKill2'),
(80492, 0, 2, 'You have been judged, unworthy.', 14, 0, 100, 0, 0, 0, 'VanguardSamuelleKill2'),
(80492, 1, 0, 'I must find a way to enhance my performance for next time.', 14, 0, 100, 0, 0, 0, 'VanguardSamuelleDeath'),

(80491, 0, 0, 'I return to the earth, but I will rise again enflamed!', 14, 0, 100, 0, 0, 0, 'ElementalistNovoDeath'),

(79900, 0, 0, 'Bwahahahaha!', 14, 0, 100, 0, 0, 0, 'CaptainHoodrychKill1'),
(79900, 0, 1, 'Ahahaha, major choppable!', 14, 0, 100, 0, 0, 0, 'CaptainHoodrychKill2'),
(79900, 1, 0, 'B-b-b-b-bladestorm!', 14, 0, 100, 0, 0, 0, 'CaptainHoodrychBladestorm'),
(79900, 2, 0, 'Aaargh, I''ll stick to tavern games from now on.', 14, 0, 100, 0, 0, 0, 'CaptainHoodrychDeath'),

(80489, 0, 0, 'Alright boys and girls, it''s prime time!', 14, 0, 100, 0, 0, 0, 'SoulbrewerNadagastSpawn'),
(80489, 1, 0, 'Chopped and brewed.', 14, 0, 100, 0, 0, 0, 'SoulbrewerNadagastKilled'),

(80486, 0, 0, 'My power is surpassed only by my stature!', 14, 0, 100, 0, 0, 0, 'NecrolordAzaelKill'),
(80486, 1, 0, 'Defeated? No! Charles, where were you?!', 14, 0, 100, 0, 0, 0, 'NecrolordAzaelDeath'),

(80496, 0, 0, 'Another trophy for my wall.', 14, 0, 100, 0, 0, 0, 'RifthunterYoskeSlay1'),
(80496, 0, 1, 'My pet hungers.', 14, 0, 100, 0, 0, 0, 'RifthunterYoskeSlay2'),
(80496, 0, 2, 'Shots fired!', 14, 0, 100, 0, 0, 0, 'RifthunterYoskeSlay3'),
(80496, 1, 0, 'The hunt is over...for now.', 14, 0, 100, 0, 0, 0, 'RifthunterYoskeDeath'),

(87690, 0, 0, 'You slow me down, I''m gonna drop you.', 14, 0, 100, 0, 0, 0, 'KazEndskySlay1'),
(87690, 0, 1, 'As for you, well you''re easy.', 14, 0, 100, 0, 0, 0, 'KazEndskySlay2'),
(87690, 0, 2, 'Orders are orders.', 14, 0, 100, 0, 0, 0, 'KazEndskySlay3'),
(87690, 0, 3, 'You''re dead weight!', 14, 0, 100, 0, 0, 0, 'KazEndskySlay4'),
(87690, 1, 0, 'If only Max were here.', 14, 0, 100, 0, 0, 0, 'RifthunterYoskeDeath'),

(85138, 0, 0, 'And no a hair out of place.', 14, 0, 100, 0, 0, 0, 'RazorGuerraKill'),
(85138, 1, 0, 'I never asked for this.', 14, 0, 100, 0, 0, 0, 'RazorGuerraDeath'),

(88109, 0, 0, 'Forged in flame, my power knows no bounds, my allies fear nothing!', 14, 0, 100, 0, 0, 0, 'KimilynSpawn'),
(88109, 1, 0, 'My body has fallen, but my spirit rises. I will always be with you.', 14, 0, 100, 0, 0, 0, 'KimilynDeath');

DELETE FROM locales_creature_text WHERE entry IN (80499, 80484, 79902, 80498, 80488, 85129, 80495, 85140, 85137, 80497, 80490, 80493, 80492, 80491, 79900, 80489, 80486, 80496, 87690, 85138, 88109);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    80499, 0, 0,
    'Tiens, mange !',
    'Mitten ins Gesicht!',
    '¡A la cara!',
    'На-ка по морде!'
),
(
    80499, 0, 1,
    'Mords la poussière !',
    'Fresst Staub!',
    '¡Come tierra!',
    'Получай!'
),
(
    80499, 1, 0,
    'Hum, je n''ai jamais rencontré ce problème dans la vallée d''Alterac.',
    'Hm, das Problem hatte ich im Alteractal nie.',
    'Mm, nunca tuve este problema en el Valle de Alterac.',
    'Хм, в Альтеракской долине у меня таких проблем не возникало.'
),
(
    80484, 0, 0,
    '« Bajheera le Furieux » est de sortie !',
    'Der tobende Bajheera wurde entfesselt!',
    '¡Liberaron a la Furia Bajheera!',
    'Гнев Баджиры вырвался на волю!'
),
(
    80484, 1, 0,
    'Je ne tolère pas l''échec. Je reviendrai après m''être entraîné.',
    'Versagen ist keine Option. Ich werde trainieren und danach zurückkehren.',
    'El fracaso no es una opción. Regresaré luego de entrenar.',
    'Я не потерплю неудач. Вернусь после тренировки.'
),
(
    79902, 0, 0,
    'Salut tout le monde, ici John Swifty !',
    'Hallo allerseits, John Swifty ist da!',
    '¡Oigan todos, John Presto por aquí!',
    'Привет всем, я Джон Свифти!'
),
(
    79902, 1, 0,
    'J''ai laissé passer ma chance...',
    'Ich hatte nur einen Versuch.',
    'Solo tenía una oportunidad.',
    'У меня был только один шанс.'
),
(
    80498, 0, 0,
    'Mon retour signera votre fin.',
    'Meine Rückkehr ist Euer Untergang.',
    'Mi regreso será tu fin.',
    'Мое возвращение станет твоей гибелью.'
),
(
    80488, 0, 0,
    'Je sens les pouvoirs du soleil et de la lune gronder en moi dans un fracas assourdissant !',
    'Die Macht der Sonne und des Monds durchströmen mich mit ohrenbetäubendem Donnern!',
    '¡El poder del sol y la luna fluye en mi interior con un ruido ensordecedor!',
    'Сила солнца и луны течет сквозь меня, оглушительно ухая, звонко звеня.'
),
(
    80488, 1, 0,
    'BOUM !',
    'BUMM!',
    '¡BUM!',
    'БУМ!'
),
(
    80488, 1, 1,
    'KA-BOUM !',
    'KA-BUMM!',
    '¡KA-BUM!',
    'БДЖЖЖ!'
),
(
    80488, 1, 2,
    'BA-DA-BOUM !',
    'RUMM BUMM WUMM!',
    '¡BA-BA-BUM!',
    'ТЫДЫЖ!'
),
(
    85129, 0, 0,
    'TUEEEEEEER !!!!',
    'TÖÖÖÖÖÖÖÖTEN!',
    '¡MATARRRR!',
    'УБИ-И-И-ИТЬ!'
),
(
    85129, 0, 1,
    'MOUREEEEEZ !',
    'STEEEEEERBT!',
    '¡MUEREEEE!',
    'УМРИ-И-И-И-И-И!'
),
(
    85129, 0, 2,
    'MOOORRRT !!!',
    'TOOOOOOD!!!',
    '¡MUERTEEEE!',
    'СМЕ-Е-Е-Е-ЕРТЬ!'
),
(
    85129, 1, 0,
    'Nooooon !',
    'Neiiinnn!',
    '¡Nooooo!',
    'Н-е-ет!'
),
(
    80495, 0, 0,
    'Craignez la puissance des gnomes !',
    'Fürchtet die Macht der Gnome!',
    '¡Teme el poder de los gnomos!',
    'Бойся силы гномов!'
),
(
    80495, 0, 1,
    'Le pouvoir aux gnomes !',
    'Gnomenkraft!',
    '¡Poder gnomo!',
    'Гномья мощь!'
),
(
    80495, 0, 2,
    'Petits et rusés !',
    'Klein und gemein!',
    '¡Pequeños y sigilosos!',
    'Маленький и проворный!'
),
(
    80495, 1, 0,
    'Ne touchez pas à mes couettes, s''il vous plaît...',
    'Fasst einfach meine Zöpfe nicht an. Bitte...',
    'No me toques las trencitas, por favor...',
    'Пожалуйста, не трогай мои косички...'
),
(
    85140, 0, 0,
    'Vous me laissez de marbre.',
    'Ich bin nicht beeindruckt.',
    'No me impresionan.',
    'Не впечатляет.'
),
(
    85140, 0, 1,
    'Vous n''aviez aucune chance.',
    'Ihr hattet keine Chance.',
    'No tenías ninguna oportunidad.',
    'У тебя не было ни шанса.'
),
(
    85140, 1, 0,
    'C''est pas terminé.',
    'Das ist nicht das Ende.',
    'Esto no ha terminado.',
    'Это еще не конец.'
),
(
    85137, 0, 0,
    'N''ayez crainte, soldats de l''Alliance. Désormais, c''est l''ennemi qui a des raisons d''avoir peur.',
    'Fürchtet die Allianz nicht, denn die Furcht ist nun Euer Verbündeter.',
    'No teman, Alianza, pues el miedo ahora es su aliado.',
    'Не бойтесь Альянса, ибо страх отныне ваш союзник.'
),
(
    85137, 1, 0,
    'Je suis vaincu, mais je me relèverai.',
    'Besiegt, aber nicht vernichtet.',
    'Derrotado, pero no destruido.',
    'Побежден, но не уничтожен.'
),
(
    80497, 0, 0,
    'J''apporte le baiser de la mort à l''Alliance.',
    'Ich bringe der Allianz die Gabe des Todes.',
    'Traigo la ofrenda de la muerte a la Alianza.',
    'Я принес Альянсу дар смерти.'
),
(
    80497, 1, 0,
    'Pour l''heure c''est l''ennemi qui l''emporte, mais je reviendrai quand le contexte jouera en ma faveur.',
    'Vorerst mag mein Feind obsiegen. Aber ich werde zurückkehren, sobald meine Chancen besser stehen.',
    'La hora pertenece a mi enemigo, pero volveré cuando las probabilidades estén a mi favor.',
    'Сейчас враг торжествует. Но я вернусь, когда преимущество будет на моей стороне.'
),
(
    80490, 0, 0,
    'Apprêtez-vous à rencontrer le plus grand champion d''Azeroth !',
    'Seid Ihr bereit, Azeroths größtem Champion gegenüberzutreten?!',
    '¡¿Estás $glisto:lista; para conocer al campeón más grande de Azeroth?!',
    'Вы готовы сразиться с величайшим воителем Азерота?!'
),
(
    80493, 0, 0,
    'Brisé !',
    'Zerschmettert!',
    '¡Congelado!',
    'Все кончено!'
),
(
    80493, 0, 1,
    'Une victoire bien trop facile !',
    'Geschenkter Sieg!',
    '¡Victoria fácil!',
    'Чистая победа!'
),
(
    80493, 1, 0,
    'C''en est fini de Van Rook...iiiiii...',
    'Das war''s mit Van Rook eeeeeeeee...',
    'Adiós Van Grajo...',
    'Ван Рук пал. А-а-а-а-а...'
),
(
    80492, 0, 0,
    'Le jour du véritable châtiment est arrivé !',
    'Der Tag der wahren Rache ist gekommen!',
    '¡Llegó el día de la auténtica reprensión!',
    'Пришел день истинной расплаты!'
),
(
    80492, 0, 1,
    'Retournez à la Lumière !',
    'Kehrt zurück ins Licht!',
    '¡Regresa a la luz!',
    'Назад, к Свету!'
),
(
    80492, 0, 2,
    'Indignes, vous avez été jugés.',
    'Ihr wurdet gerichtet, Unwürdiger.',
    'Fuiste $gjuzgado y considerado indigno:juzgada y considerada indigna;.',
    'Молот правосудия пал на тебя.'
),
(
    80492, 1, 0,
    'Je dois trouver le moyen de m''améliorer pour la prochaine fois.',
    'Ich muss einen Weg finden, um meine Leistung fürs nächste Mal zu verbessern.',
    'Debo encontrar un modo de mejorar mi actuación.',
    'Я должен проявить себя лучше в следующий раз.'
),
(
    80491, 0, 0,
    'Je retourne à la terre, mais je renaîtrai de mes cendres !',
    'Ich kehre zur Erde zurück, doch werde ich in Flammen auferstehen!',
    'Vuelvo a la tierra, pero ¡volveré a alzarme envuelto en llamas!',
    'Земля примет меня, и я восстану в огне!'
),
(
    79900, 0, 0,
    'Bwa ha ha ha ha !',
    'Buahahahaha!',
    '¡Muajaja!',
    'Ха-ха-ха-ха-ха!'
),
(
    79900, 0, 1,
    'Ha ha ha, j''en ai fait du petit bois !',
    'Ahahaha, ordentlich klein gehackt!',
    'Ja, ja, ja, ja, ¡gran comestible!',
    'А-ха-ха! Порубил на куски!'
),
(
    79900, 1, 0,
    'T-t-t-t-tempête de lames !',
    'K-k-k-k-klingensturm!',
    '¡F-f-f-filotormenta!',
    'Вихрь клинко-о-о-о-ов!'
),
(
    79900, 2, 0,
    'Aaargh ! Je crois que je vais dorénavant m''en tenir aux piliers de taverne.',
    'Aaargh! Ich begnüge mich ab jetzt mit Kneipenspielen.',
    '¡Aaargh! Me limitaré a juegos de taberna por ahora.',
    'Гр-р-р! Все, я буду играть только в тавернах.'
),
(
    80489, 0, 0,
    'Bon les gars, c''est le coup d''envoi !',
    'Also los, Jungs und Mädels, jetzt ist die beste Zeit!',
    '¡Muy bien, chicos y chicas, es horario estelar!',
    'Ну что, мальчики и девочки, пришло время повеселиться!'
),
(
    80489, 1, 0,
    'Emballé, c''est pesé.',
    'Kleingehackt und gebraut.',
    'Troceado y servido.',
    'Порубили и сварили.'
),
(
    80486, 0, 0,
    'Ma puissance n''est surpassée que par ma stature !',
    'Meine Macht wird nur noch durch meine Statur übertroffen!',
    '¡Mi poder solo es superado por mi estatura!',
    'Мою силу превосходит только мой рост!'
),
(
    80486, 1, 0,
    'Vaincu ? Non ! Charles, où étiez-vous ?!',
    'Besiegt? Nein! Charles, wo wart Ihr?!',
    '¿Derrotado? ¡No! Charles, ¡¿dónde estabas?!',
    'Побежден? Нет! Чарльз, где ты?'
),
(
    80496, 0, 0,
    'Un nouveau trophée pour mon mur.',
    'Eine weitere Trophäe für meine Wand.',
    'Otro trofeo para mi pared.',
    'Еще один трофей на мою стену.'
),
(
    80496, 0, 1,
    'Mon familier a faim.',
    'Mein Tier hat Hunger.',
    'Mi mascota tiene hambre.',
    'Мой питомец голоден.'
),
(
    80496, 0, 2,
    'Coups de feu !',
    'Schüsse abgefeuert!',
    '¡Disparados!',
    'Зря ты $gбросил:бросила; мне вызов!'
),
(
    80496, 1, 0,
    'La chasse est terminée... pour le moment.',
    'Die Jagd ist vorüber... vorerst.',
    'La caza se acabó... por ahora.',
    'Охота подошла к концу... но позже она продолжится.'
),
(
    87690, 0, 0,
    'Vous me ralentissez, je vais devoir vous laisser.',
    'Wenn Ihr mich aufhaltet, lasse ich Euch fallen.',
    'Me demoras, voy a dejarte.',
    'Ты балласт, и я тебя сброшу.'
),
(
    87690, 0, 1,
    'Quant à toi, eh bien, rien de bien difficile.',
    'Und was Euch betrifft, nun, Ihr seid nicht schwierig.',
    'En cuanto a ti... bueno, eres fácil.',
    'Ты-то весом полегче.'
),
(
    87690, 0, 2,
    'Les ordres sont les ordres.',
    'Befehl ist Befehl.',
    'Órdenes son órdenes.',
    'Приказ есть приказ.'
),
(
    87690, 0, 3,
    'Tu n''es qu''un poids mort !',
    'Ihr seid nur Ballast!',
    '¡Eres un peso muerto!',
    'Мертвый груз!'
),
(
    87690, 1, 0,
    'Si seulement Max était là.',
    'Wenn doch nur Max hier wäre.',
    'Si Max estuviera aquí...',
    'Ах, если бы только Макс был здесь.'
),
(
    85138, 0, 0,
    'Et ma coiffure n''a même pas bougé.',
    'Und die Frisur sitzt.',
    'Y ni un pelo fuera de lugar.',
    'Идеально ровный надрез.'
),
(
    85138, 1, 0,
    'Je n''ai pas signé pour ça.',
    'Das habe ich nie gewollt.',
    'Yo nunca pedí esto.',
    'Я на такое не подписывался.'
),
(
    88109, 0, 0,
    'Ma puissance est née au cœur des flammes et ne connaît aucune limite. Mes alliés n''ont rien à craindre !',
    'Meine in Flammen geschmiedeten Kräfte sind grenzenlos, und meine Verbündeten kennen keine Furcht!',
    'Forjado en las llamas, mi poder no conoce límites, ¡mis aliados no le temen a nada!',
    'Моя мощь, закаленная пламенем, безгранична, а мои товарищи не ведают страха!'
),
(
    88109, 1, 0,
    'Mon corps me trahit, mais mon âme s''élève. Je serai toujours à vos côtés.',
    'Mein Körper geht zugrunde, doch mein Geist erhebt sich. Ich werde immer bei Euch sein.',
    'Mi cuerpo ha caído, pero mi espíritu se levanta. Siempre estaré contigo.',
    'Мое тело повержено, но дух воспарит. Я всегда буду с тобой.'
);

UPDATE creature_classlevelstats SET basehp5 = 78283 WHERE level = 100 AND class = 8;

# Alliance
DELETE FROM creature_equip_template WHERE entry IN (80499, 80484, 79902, 80494, 80498, 80488, 85129, 80495, 80500, 85122, 80485, 85140, 85137);
INSERT INTO creature_equip_template VALUES
(80499, 1, 32963, 42508, 0),
(80484, 1, 77191, 77191, 0),
(79902, 1, 21553, 0, 0),
(80494, 1, 89711, 89711, 0),
(80498, 1, 87862, 0, 0),
(80488, 1, 65484, 0, 0),
(85129, 1, 89776, 0, 0),
(80495, 1, 28184, 28184, 0),
(80500, 1, 77214, 29458, 0),
(85122, 1, 85756, 0, 72278),
(80485, 1, 110784, 0, 0),
(85140, 1, 42477, 0, 2550),
(85137, 1, 87862, 0, 0);

UPDATE creature_template SET minlevel = 100, maxlevel = 100, dmg_multiplier = 5, faction = 2618 WHERE entry IN (80499, 80484, 79902, 80494, 80498, 80488, 85129, 80495, 80500, 85122, 80485, 85140, 85137);
UPDATE creature_template SET unit_class = 2, unit_flags2 = 2099200, ScriptName = 'npc_ashran_avenger_turley' WHERE entry = 80499;
UPDATE creature_template SET ScriptName = 'npc_ashran_jackson_bajheera' WHERE entry = 80484;
UPDATE creature_template SET ScriptName = 'npc_ashran_john_swifty' WHERE entry = 79902;
UPDATE creature_template SET unit_class = 4, ScriptName = 'npc_ashran_tosan_galaxyfist' WHERE entry = 80494;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_brock_the_crazed' WHERE entry = 80498;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_alune_windmane' WHERE entry = 80488;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_chani_malflame' WHERE entry = 85129;
UPDATE creature_template SET unit_class = 4, ScriptName = 'npc_ashran_hildie_hackerguard' WHERE entry = 80495;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_taylor_dewland' WHERE entry = 80500;
UPDATE creature_template SET baseattacktime = 1000, rangeattacktime = 1000, ScriptName = 'npc_ashran_malda_brewbelly' WHERE entry = 85122;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_shani_freezewind' WHERE entry = 80485;
UPDATE creature_template SET ScriptName = 'npc_ashran_anne_otther' WHERE entry = 85140;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_mathias_zunn' WHERE entry = 85137;

# Horde
DELETE FROM creature_equip_template WHERE entry IN (80497, 80490, 80493, 80492, 80491, 79900, 80489, 80486, 80496, 85133, 87690, 85138, 85131, 88109);
INSERT INTO creature_equip_template VALUES
(80497, 1, 42477, 0, 0),
(80490, 1, 72484, 0, 0),
(80493, 1, 104045, 0, 2550),
(80492, 1, 34247, 0, 0),
(80491, 1, 32026, 32082, 0),
(79900, 1, 49615, 0, 2550),
(80489, 1, 30910, 30872, 0),
(80486, 1, 119770, 0, 2550),
(80496, 1, 85756, 0, 72278),
(85133, 1, 106879, 0, 0),
(87690, 1, 42477, 0, 2550),
(85138, 1, 119705, 119705, 0),
(85131, 1, 112920, 112920, 0),
(88109, 1, 72484, 0, 0);

UPDATE creature_template SET minlevel = 100, maxlevel = 100, dmg_multiplier = 5, faction = 2667 WHERE entry IN (80497, 80490, 80493, 80492, 80491, 79900, 80489, 80486, 80496, 85133, 87690, 85138, 85131, 88109);
UPDATE creature_template SET ScriptName = 'npc_ashran_lord_mes' WHERE entry = 80497;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_mindbender_talbadar' WHERE entry = 80490;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_elliott_van_rook' WHERE entry = 80493;
UPDATE creature_template SET unit_class = 2, unit_flags2 = 2099200, ScriptName = 'npc_ashran_vanguard_samuelle' WHERE entry = 80492;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_elementalist_novo' WHERE entry = 80491;
UPDATE creature_template SET ScriptName = 'npc_ashran_captain_hoodrych' WHERE entry = 79900;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_soulbrewer_nadagast' WHERE entry = 80489;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_necrolord_azael' WHERE entry = 80486;
UPDATE creature_template SET baseattacktime = 1000, rangeattacktime = 1000, ScriptName = 'npc_ashran_rifthunter_yoske' WHERE entry = 80496;
UPDATE creature_template SET baseattacktime = 1000, rangeattacktime = 1000, unit_class = 8, ScriptName = 'npc_ashran_morriz' WHERE entry = 85133;
UPDATE creature_template SET ScriptName = 'npc_ashran_kaz_endsky' WHERE entry = 87690;
UPDATE creature_template SET unit_class = 4, ScriptName = 'npc_ashran_razor_guerra' WHERE entry = 85138;
UPDATE creature_template SET unit_class = 4, ScriptName = 'npc_ashran_jared_v_hellstrike' WHERE entry = 85131;
UPDATE creature_template SET unit_class = 8, ScriptName = 'npc_ashran_kimilyn' WHERE entry = 88109;

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_ashran_shockwave');
INSERT INTO spell_script_names VALUE
(164092, 'spell_ashran_shockwave');