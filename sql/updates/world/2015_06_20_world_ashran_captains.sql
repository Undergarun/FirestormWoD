SET NAMES utf8;

DELETE FROM creature_text WHERE entry IN (80499, 80484, 79902, 80498, 80488, 85129, 80495, 85140, 85137, 80497, 80493, 80492, 80491, 79900, 80486, 80496, 87690, 85138, 88109);
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

(80493, 0, 0, 'Shattered!', 14, 0, 100, 0, 0, 0, 'ElliotVanRookSlay1'),
(80493, 0, 1, 'Free win!', 14, 0, 100, 0, 0, 0, 'ElliotVanRookSlay2'),
(80493, 1, 0, 'Down goes Van Rook eeeeeeeee....', 14, 0, 100, 0, 0, 0, 'ElliotVanRookDeath'),

(80492, 0, 0, 'I must find a way to enhance my performance for next time.', 14, 0, 100, 0, 0, 0, 'VanguardSamuelleDeath'),

(80491, 0, 0, 'I return to the earth, but I will rise again enflamed!', 14, 0, 100, 0, 0, 0, 'ElementalistNovoDeath'),

(79900, 0, 0, 'Bwahahahaha!', 14, 0, 100, 0, 0, 0, 'CaptainHoodrychKill'),
(79900, 1, 0, 'B-b-b-b-bladestorm!', 14, 0, 100, 0, 0, 0, 'CaptainHoodrychBladestorm'),

(80486, 0, 0, 'Defeated? No! Charles, where were you?!', 14, 0, 100, 0, 0, 0, 'NecrolordAzaelDeath'),

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

DELETE FROM locales_creature_text WHERE entry IN (80499, 80484, 79902, 80498, 80488, 85129, 80495, 85140, 85137, 80497, 80493, 80492, 80491, 79900, 80486, 80496, 87690, 85138, 88109);
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
    79900, 1, 0,
    'T-t-t-t-tempête de lames !',
    'K-k-k-k-klingensturm!',
    '¡F-f-f-filotormenta!',
    'Вихрь клинко-о-о-о-ов!'
),
(
    80486, 0, 0,
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