SET NAMES utf8;

DELETE FROM instance_template WHERE map = 1228;
INSERT INTO instance_template VALUE
(1228, 1116, 'instance_highmaul', 1);

-- Normal, Heroic, Mythic and Raid Tools
UPDATE creature SET spawnmask = 245760 WHERE map = 1228;
UPDATE gameobject SET spawnmask = 245760 WHERE map = 1228;

DELETE FROM areatrigger_teleport WHERE id = 10272;
INSERT INTO areatrigger_teleport VALUE
(10272, 'Highmaul Raid - Instance Entrance', 1228, 3486.48, 7603.32, 10.4853, 4.0252);

UPDATE creature_template SET gossip_menu_id = 87311 WHERE entry = 87311;
UPDATE creature_template SET gossip_menu_id = 84971, ScriptName = 'npc_highmaul_gharg_arena_master' WHERE entry = 84971;

DELETE FROM gossip_menu WHERE entry IN (87311, 84971);
DELETE FROM gossip_menu WHERE text_id IN (90358, 86506);
INSERT INTO gossip_menu VALUE
(87311, 90358),
(84971, 86506);

DELETE FROM npc_text WHERE ID IN (90358, 86506);
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(
    90358,
    '<Kharg seems uninterested.>',
    '<Kharg seems uninterested.>'
),
(
    86506,
    'So eager to die?',
    'So eager to die?'
);

DELETE FROM locales_npc_text WHERE entry IN (90358, 86506);
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
(
    90358,
    '<Kharg semble indifférent.>',
    '<Kharg semble indifférent.>',
    '<Kharg wirkt desinteressiert.>',
    '<Kharg wirkt desinteressiert.>',
    '<Kharg no se ve interesado.>',
    '<Kharg no se ve interesado.>',
    '<Кажется, Харгу это неинтересно.>',
    '<Кажется, Харгу это неинтересно.>'
),
(
    86506,
    'Vous avez donc tellement hâte de mourir ?',
    'Vous avez donc tellement hâte de mourir ?',
    'Wir sind wohl sehr begierig darauf zu sterben?',
    'Wir sind wohl sehr begierig darauf zu sterben?',
    '¿Tan ansioso por morir?',
    '¿Tan ansioso por morir?',
    'Торопишься навстречу смерти?',
    'Торопишься навстречу смерти?'
);

DELETE FROM gossip_menu_option WHERE menu_id IN (84971);
INSERT INTO gossip_menu_option VALUES
(84971, 0, 0, 'We are ready, Gharg!', 1, 3, 0, 0, 0, 0, '');

DELETE FROM locales_gossip_menu_option WHERE menu_id IN (84971);
--                                                   French            German            Spanish           Russian
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc2, option_text_loc3, option_text_loc6, option_text_loc8) VALUES
(
    84971, 0,
    'Nous sommes prêts, Gharg !',
    'Wir sind bereit, Gharg!',
    '¡Estamos listos, Gharg!',
    'Мы готовы, Гарг!'
);

DELETE FROM creature_text WHERE entry IN (83377, 83378, 80048, 83268, 78714, 78926);
INSERT INTO creature_text VALUES
(83377, 0, 0, 'This''s gotta be a mistake! These puny squishies be the scourge of Nagrand? They look like Elekk poodoo.', 14, 0, 100, 0, 0, 43580, 'JhornIntro1'),
(83377, 1, 0, 'Ha! Today''s games honor great sorcerer king! We gonna see his bloody victory over the orcs at the Battle of Red Spires!', 14, 0, 100, 0, 0, 43581, 'JhornIntro2'),
(83377, 2, 0, 'We''ll see what dese competitors be made of, Thoktar, just as soon as what dey be made of gets splattered across the arena! HAHAHAHA!', 14, 0, 100, 0, 0, 43582, 'JhornIntro3'),
(83377, 3, 0, 'Now, playing our mighty king, scream his name and bash a skull for the Shadow of Highmaul, champion of games, Vul''gor!', 14, 0, 100, 0, 0, 43583, 'JhornIntro4'),
(83377, 4, 0, 'Time to spill blood and die for Highmaul! The Battle of Red Spires begins!', 14, 0, 100, 0, 0, 43584, 'JhornIntro5'),
(83377, 5, 0, 'These wimps be taking more limbs than losing em!', 14, 0, 100, 0, 0, 43587, 'JhornTrash1'),
(83377, 6, 0, 'Looks like they don''t care they''re supposed to die.', 14, 0, 100, 0, 0, 43588, 'JhornTrash2'),
(83377, 7, 0, 'The punies... defeated, Vul''gor!?', 14, 0, 100, 0, 0, 43585, 'JhornKargath1'),
(83377, 8, 0, 'The sorcerer king!? Here!? King only leaves palace to witness legendary battles.', 14, 0, 100, 0, 0, 43586, 'JhornKargath2'),

(83378, 0, 0, 'Dey gonna look like puddles after dis fight.', 14, 0, 100, 0, 0, 45938, 'ThoktarIntro1'),
(83378, 1, 0, 'I hope dese weaklings die better''n they smell.', 14, 0, 100, 0, 0, 45939, 'ThoktarIntro2'),
(83378, 2, 0, 'HAHAHAHAHA!', 14, 0, 100, 0, 0, 45940, 'ThoktarIntro3'),
(83378, 3, 0, 'Didn''t we win Battle O'' Red Spires?', 14, 0, 100, 0, 0, 45942, 'ThoktarTrash1'),
(83378, 4, 0, 'Disrespectin'' our heritage won''t get em invited to next year''s games.', 14, 0, 100, 0, 0, 45943, 'ThoktarThrash2'),
(83378, 5, 0, 'How tiny mens beat mighty Vul''gor?!', 14, 0, 100, 0, 0, 45941, 'ThoktarKargath1'),

(80048, 0, 0, 'Glory and death!', 14, 0, 100, 0, 0, 45788, 'VulgorAggro'),
(80048, 1, 0, 'Butchered!', 14, 0, 100, 0, 0, 45790, 'VulgorSlay1'),
(80048, 1, 1, 'Me rip, you die!', 14, 0, 100, 0, 0, 45791, 'VulgorSlay2'),
(80048, 2, 0, 'Break you!', 14, 0, 100, 0, 0, 45792, 'VulgorSpell1'),

(83268, 0, 0, 'You honor us, Kargath. I will enjoy watching you fight in the arena again, even with your... handicap.', 14, 0, 100, 0, 0, 44756, 'SorckingEvent12'),
(83268, 1, 0, 'Gladiators! Seize this opportunity to die gloriously, for your names will be carved into history. Let the battle begin!', 14, 0, 100, 0, 0, 44757, 'SorckingEvent13'),
(83268, 2, 0, 'Finish him.', 14, 0, 100, 0, 0, 44758, 'SorckingEvent14'),

(78714, 0, 0, 'Enough! I will show these weaklings what a true champion is capable of.', 14, 0, 100, 0, 0, 44521, 'KargathIntro1'),
(78714, 1, 0, 'The pleasure is mine. The crowd deserves a superior gladiator, seeing as the last one fell so easily.', 14, 0, 100, 0, 0, 44522, 'KargathIntro2'),
(78714, 2, 0, 'You owe me a fight, cowards.', 14, 0, 100, 0, 0, 44515, 'KargathAggro'),
(78714, 3, 0, 'There is no escape!', 14, 0, 100, 0, 0, 44527, 'KargathBerserkerRush'),
(78714, 4, 0, 'Let''s give the crowd something to chew on!', 14, 0, 100, 0, 0, 44528, 'KargathChainHurl'),
(78714, 5, 0, 'Time to spill your blood!', 14, 0, 100, 0, 0, 44529, 'KargathImpale'),
(78714, 6, 0, 'I''ve toyed with you long enough!', 14, 0, 100, 0, 0, 44517, 'KargathBerserk'),
(78714, 7, 0, 'You think you''re clever?! You''re next to burn!', 14, 0, 100, 0, 0, 44518, 'KargathEvent1'),
(78714, 7, 1, 'Fool! You''ll need more than flames to finish me!', 14, 0, 100, 0, 0, 44519, 'KargathEvent2'),
(78714, 7, 2, 'Gah! COWARD! You can''t hide behind these traps forever!', 14, 0, 100, 0, 0, 44520, 'KargathEvent3'),
(78714, 8, 0, 'You break so easily...', 14, 0, 100, 0, 0, 44523, 'KargathKill1'),
(78714, 8, 1, 'You''ll always be remembered as a smudge on my boot.', 14, 0, 100, 0, 0, 44524, 'KargathKill2'),
(78714, 8, 2, 'No mercy.', 14, 0, 100, 0, 0, 44525, 'KargathKill3'),
(78714, 9, 0, 'And that''s... one hundred.', 12, 0, 100, 0, 0, 44516, 'KargathDeath'),

(78926, 0, 0, 'Time to die!', 12, 0, 100, 0, 0, 0, 'IronBomber1'),
(78926, 0, 1, 'You''re coming with me!', 12, 0, 100, 0, 0, 0, 'IronBomber2');

DELETE FROM locales_creature_text WHERE entry IN (83377, 83378, 80048, 83268, 78714, 78926);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    83377, 0, 0,
    'Ça doit être une erreur ! Ces ptits trucs tout mous, c''est le fléau de Nagrand ? On dirait du popo d''elekk.',
    'Das kann doch nich'' stimmen! Diese Mickerchen soll''n die Geißel von Nagrand sein? Sehen wie Elekkköttel aus!',
    '¡Tener que haber un error! ¿Estos blandengues ser el azote de Nagrand? Parecer desechos de elekk.',
    'Это, должно быть, ошибка! Эти жалкие черви  гроза агранда? а они в плевке элекка утонут.'
),
(
    83377, 1, 0,
    'Ah ! Les jeux d''aujourd''hui honorent le grand roi-sorcier ! On va voir sa victoire sanglante sur les orcs à la bataille des Flèches-Rouges !',
    'Ha! Die heutigen Spiele ehren großen Zaubererkönig! Wir werden sehen seinen blutigen Sieg in der Schlacht der Roten Spitzen!',
    '¡Ja! ¡Los juegos de hoy honrar al gran Rey hechicero! ¡Veremos su victoria sangrienta sobre los orcos en la Batalla de los pináculos rojos!',
    'Сегодня игры проводятся в честь короля-чародея! ы увидим его победу над орками при расных иках!'
),
(
    83377, 2, 0,
    'On va voir de quoi les adversaires sont faits, Thoktar. On le saura dès qu''ils auront fait « splash » dans toute l''arène ! HAHAHAHA !',
    'Wir werden sehen, woraus diese Herausforderer gemacht sind, Thoktar. ...sobald das, woraus sie gemacht sind, den Arenaboden verziert! HAHAHAHA!',
    'Nosotros ver de qué estar hechos estos competidores, Thoktar, ¡en cuanto su relleno quedar esparcido por toda arena! ¡JA, JA, JA, JA!',
    'Посмотрим, Токтар, как они будут извиваться и молить о пощаде, когда их станут расплющивать по всей арене! Ха-ха-ха!'
),
(
    83377, 3, 0,
    'Dans le rôle de notre puissant roi, scandez son nom et écrasez un crâne pour l''Ombre de Cognefort, le champion des jeux Vul''gor !',
    'Jetzt, in der Rolle unseres mächtigen Kaisers, brüllt seinen Namen und schlagt einen Schädel ein für den Schatten von Hochfels, Vul''gor!',
    'Ahora tú actuar como nuestro poderoso rey, ¡gritar su nombre y aplastar un cráneo por la Sombra de Gran Magullador, campeón de los juegos, Vul''gor!',
    'А теперь – трепещите! В роли нашего могучего короля на арену выходит чемпион игр – Вул''гор!'
),
(
    83377, 4, 0,
    'C''est l''heure de verser le sang et de mourir pour Cognefort ! La bataille des Flèches-Rouges commence !',
    'Vergießt Blut und sterbt glorreich zu Ehren von Hochfels! Die Schlacht der Roten Spitzen beginnt!',
    '¡Ser hora de derramar sangre y morir por Gran Magullador! ¡La Batalla de los pináculos rojos comenzar!',
    'Во имя Верховного Молота! Пусть начнется битва при Красных Пиках!'
),
(
    83377, 5, 0,
    'Ces mauviettes arrachent plus de membres qu''ils en perdent !',
    'Diese Winzlinge teilen ja mehr aus, als sie einstecken!',
    '¡Estos debiluchos estar llevándose más extremidades de las que perder!',
    'Они отсекли больше конечностей, чем потеряли!'
),
(
    83377, 6, 0,
    'On dirait qu''ils ont pas compris qu''ils devaient mourir.',
    'Scheinbar ist denen nicht klar, dass sie sterben sollen.',
    'Parecer no importarles que ellos deber morir.',
    'Кажется, они не знают, что им положено умереть.'
),
(
    83377, 7, 0,
    'Les gringalets... ont battu Vul''gor ?!',
    'Die Winzlinge... Vul''gor besiegt?',
    'Los debiluchos... ¡¿haber derrotado Vul''gor?!',
    'Слабаки... победили ул''гора?!'
),
(
    83377, 8, 0,
    'Le roi-sorcier ?! Ici ?! Le roi quitte le palais pour voir que les combats légendaires !',
    'Der Zaubererkönig!? Hier!? König verlassen Palast nur, um legendäre Kämpfe zu sehen!',
    '¡¿El Rey hechicero?! ¡¿Aquí!?! ¡El Rey solo salir del palacio para presenciar batallas legendarias!',
    'Король-чародей?! Здесь?! Он же покидает дворец только ради великих битв!'
),
(
    83378, 0, 0,
    'Après ce combat, ce sera plus que de la purée.',
    'Die werden ausseh''n wie Blutwurst nach dem Kampf.',
    'Ellos verse como charcos después de esta pelea.',
    'От них останется только мокрое место.'
),
(
    83378, 1, 0,
    'J''espère que ces avortons vont mourir mieux qu''ils sentent.',
    'Hoffentlich diese Schwächlinge besser sterben als riechen.',
    'Esperar que estos debiluchos morir mejor de lo que oler.',
    'Надеюсь, умирают они лучше, чем выглядят.'
),
(
    83378, 2, 0,
    'HAHAHAHAHA !',
    'HAHAHAHAHA!',
    '¡JA, JA, JA, JA, JA, JA!',
    'ХА-ХА-ХА-ХА-ХА!'
),
(
    83378, 3, 0,
    'Mais on avait pas gagné la bataille des Flèches-Rouges ?',
    'Wir Schlacht nich'' gewonnen?',
    '¿No haber ganado Batalla de los pináculos rojos?',
    'Разве на Красных Пиках выиграли не мы?'
),
(
    83378, 4, 0,
    'S''ils respectent pas notre histoire, on les invitera plus à nos jeux.',
    'Nich'' unsere Tradition missachten, oder nächstes Jahr ihr nich'' dabei sein.',
    'Faltar respeto a nuestra herencia no lograr que invitarlos a juegos del próximo año.',
    'Уважайте нашу историю, или не позовем вас на следующие игры!'
),
(
    83378, 5, 0,
    'Comment les minus ont battu puissant Vul''gor ?!',
    'Wie die kleinen Burschen mächtigen Vul''gor besiegen konnten?',
    '¡¿Cómo ser que hombres pequeños derrotar a poderoso Vul''gor?!',
    'Как эти щенки побили Вул''гора?!'
),
(
    80048, 0, 0,
    'Gloire et mort !',
    'Ruhm und Tod!',
    '¡Gloria y muerte!',
    'Смерть и слава!'
),
(
    80048, 1, 0,
    'Boucherie !',
    'Geschlachtet!',
    '¡Destrozado!',
    'На куски!'
),
(
    80048, 1, 1,
    'Moi déchirer, toi mourir !',
    'Ich reiße, Ihr sterbt!',
    '¡Yo destripar, tú morir!',
    'Сдохни!'
),
(
    80048, 2, 0,
    'Casser vous !',
    'Zerfetzen!',
    '¡Destruirte!',
    'Раздавлю!'
),
(
    83268, 0, 0,
    'Votre présence nous honore, Kargath. Je vais prendre plaisir à vous regarder combattre à nouveau, malgré votre... handicap.',
    'Ihr ehrt uns, Kargath. Ich werde Euch mit Freuden wieder in der Arena kämpfen sehen, selbst mit Eurer... Behinderung.',
    'Nos honras, Kargath. Disfrutaré volver a verte pelear en la arena, aun con tu... limitación.',
    'Какая честь, Каргат. Я с удовольствием посмотрю, как ты снова будешь биться, несмотря на… увечье.'
),
(
    83268, 1, 0,
    'Gladiateurs ! Mourez avec panache et vos noms resteront gravés dans l''histoire. Que le combat commence !',
    'Gladiatoren! Ergreift diese Gelegenheit, glorreich zu sterben, denn eure Namen werden in die Geschichte eingehen. Lasst die Spiele beginnen!',
    '¡Gladiadores! Aprovechen la oportunidad de morir con gloria, pues sus nombres se grabarán en la historia. ¡Que comience la batalla!',
    'Гладиаторы! Я дарую вам шанс заслужить славную смерть и навеки вписать свое имя в историю! Да начнется битва!'
),
(
    83268, 2, 0,
    'Achevez-le.',
    'Macht ihn fertig.',
    'Acaba con él.',
    'Прикончите его.'
),
(
    78714, 0, 0,
    'Assez ! Je vais montrer à cette vermine ce dont est capable un vrai champion.',
    'Genug! Ich zeige diesen Schwächlingen, wozu ein wahrer Champion in der Lage ist!',
    '¡Basta! Le mostraré a estos debiluchos lo que un verdadero campeón puede hacer.',
    'Довольно! Я покажу этим щенкам, на что способен истинный чемпион.'
),
(
    78714, 1, 0,
    'Le plaisir est pour moi. La foule mérite un meilleur gladiateur que le précédent, tombé si facilement.',
    'Die Freude ist ganz meinerseits. Die Menge verdient einen überlegenen Gladiator, wo doch der letzte so leicht zu besiegen war.',
    'El honor es mío. Las multitudes merecen un gladiador de altura después de ver la facilidad con que cayó el último.',
    'Я польщен. Зрители заслуживают настоящего гладиатора, последний боец был просто бездарен.'
),
(
    78714, 2, 0,
    'Vous me devez un combat, bande de lâches !',
    'Ihr schuldet mir einen Kampf, Feiglinge.',
    'Me deben una pelea, cobardes.',
    'Начнем бой, трусы!'
),
(
    78714, 3, 0,
    'Pas d''échappatoire',
    'Es gibt kein Entkommen.',
    'No hay escapatoria.',
    'Спасения нет!'
),
(
    78714, 4, 0,
    'Donnons à la foule quelque chose à se mettre sous la dent.',
    'Die Menge will Blut sehen.',
    '¡Démosle a la multitud un trozo de la acción!',
    'Народ жаждет зрелищ, и он их получит!'
),
(
    78714, 5, 0,
    'Il est temps de faire couler le sang !',
    'Jetzt seid Ihr dran!',
    '¡Hora de derramar tu sangre!',
    'Пора пролить вашу кровь!'
),
(
    78714, 6, 0,
    'J''ai joué avec vous assez longtemps !',
    'Genug Spielereien.',
    '¡Ya me he divertido mucho tiempo!',
    'Мне надоело играть с вами!'
),
(
    78714, 7, 0,
    'Vous croyiez que ça allait marcher ?! C''est vous qui allez brûler !',
    'Ihr haltet Euch wohl für schlau. Ihr brennt als Nächstes!',
    '¡¿Te crees muy listo?! ¡Serás el siguiente en arder!',
    'Эй, умники! Гореть вам в пекле!'
),
(
    78714, 7, 1,
    'Imbéciles ! Il vous faudra plus que des flammes pour m''achever !',
    'Narren! Um mich zu besiegen, braucht Ihr mehr als nur Feuer!',
    '¡Necio! ¡Se necesitan más que llamas para derrotarme!',
    'Глупцы! Этим жалким огнем меня не напугать!'
),
(
    78714, 7, 2,
    'Lâches ! Vous ne pourrez pas vous cacher derrière ces pièges indéfiniment !',
    'Feigling! Ihr könnt Euch nicht ewig hinter Fallen verstecken!',
    '¡Cobarde! ¡No te puedes esconder tras esas trampas para siempre!',
    'Трусы! Вы не сможете вечно укрываться за ними!'
),
(
    78714, 8, 0,
    'Vous vous cassez trop vite !',
    'Ihr seid so zerbrechlich!',
    '¡Es muy fácil destrozarte!',
    'Как легко вас сломать!'
),
(
    78714, 8, 1,
    'Vous resterez dans les mémoires, comme ces saletés restent accrochées à mes bottes.',
    'Ihr werdet in die Geschichte eingehen. Als Dreck an meinem Stiefel.',
    'Siempre serás recordado como una mancha en mi bota.',
    'Придется потом отчищать от тебя ботинки.'
),
(
    78714, 8, 2,
    'Pas de pitié !',
    'Keine Gnade!',
    '¡Sin piedad!',
    'Пощады не будет!'
),
(
    78714, 9, 0,
    'Là... ça fait cent.',
    'Und das... sind Einhundert.',
    'Y con este... son cien.',
    'А-а-а-а-а... Теперь сто...'
),
(
    78926, 0, 0,
    'Votre heure est venue !',
    'Zeit, zu sterben!',
    '¡Hora de morir!',
    'Время умирать!'
),
(
    78926, 0, 1,
    'Venez avec moi !',
    'Ihr kommt mit mir!',
    '¡Tú vienes conmigo!',
    'Ты пойдешь со мной!'
);

UPDATE gameobject_template SET faction = 1735, flags = 40, ScriptName = 'go_highmaul_arena_elevator' WHERE entry = 233098;
UPDATE creature_template SET InhabitType = 4, ScriptName = 'npc_highmaul_fire_pillar' WHERE entry = 78757;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_highmaul_ravenous_bloodmaw' WHERE entry = 79296;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry IN (83029, 78846, 63420, 79712, 68553, 79134);
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = 'npc_highmaul_areatrigger_for_crowd' WHERE entry = 79260; -- Area Trigger for Crowd
UPDATE creature_template SET ScriptName = 'npc_highmaul_jhorn_the_mad' WHERE entry = 83377;
UPDATE creature_template SET ScriptName = 'npc_highmaul_thoktar_ironskull' WHERE entry = 83378;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_highmaul_vulgor', mechanic_immune_mask = 617299839 WHERE entry = 80048;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_highmaul_bladespire_sorcerer' WHERE entry = 80071;
UPDATE creature_template SET InhabitType = 4 WHERE entry = 82734;
UPDATE creature_template SET dmg_multiplier = 20, unit_flags = 320, ScriptName = 'boss_kargath_bladefist', mechanic_immune_mask = 617299839, flags_extra = (0x01 | 0x00200000), Health_mod = 1133 WHERE entry = 78714;
UPDATE creature SET unit_flags = 320 WHERE id = 78714;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_highmaul_somldering_stoneguard' WHERE entry = 80051;
UPDATE creature_template SET ScriptName = 'npc_highmaul_imperator_margok' WHERE entry = 83268;
UPDATE creature_template SET ScriptName = 'npc_highmaul_kargath_bladefist_trigger' WHERE entry = 78846;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_highmaul_drunken_bileslinger' WHERE entry = 78954;
UPDATE creature_template SET dmg_multiplier = 5, VehicleId = 3536, ScriptName = 'npc_highmaul_iron_bomber' WHERE entry = 78926;
UPDATE creature_template SET dmg_multiplier = 2, ScriptName = 'npc_highmaul_iron_grunt' WHERE entry = 84946;
UPDATE creature_template SET dmg_multiplier = 2, ScriptName = 'npc_highmaul_iron_grunt_second' WHERE entry = 79068;
UPDATE creature_template SET dmg_multiplier = 2, ScriptName = 'npc_highmaul_ogre_grunt' WHERE entry = 84958;
UPDATE creature_template SET dmg_multiplier = 2, ScriptName = 'npc_highmaul_ogre_grunt_second' WHERE entry = 84948;
UPDATE creature_template SET ScriptName = 'npc_highmaul_highmaul_sweeper' WHERE entry = 88874;
UPDATE creature_template SET ScriptName = 'npc_highmaul_chain_hurl_vehicle' WHERE entry = 79134;
UPDATE creature_template SET unit_flags = 33555264 WHERE entry = 84823;

DELETE FROM spell_script_names WHERE spell_id IN (162271, 159113, 163967, 159706, 159028, 159947, 160521, 160131, 163392, 163302, 163130, 163408, 159001, 159212);
INSERT INTO spell_script_names VALUES
(162271, 'spell_highmaul_earth_breaker'),
(159113, 'spell_highmaul_impale'),
(163967, 'spell_highmaul_fire_pillar_steam_timer'),
(159706, 'spell_highmaul_fire_pillar_activated'),
(159028, 'spell_highmaul_berserker_rush'),
(159947, 'spell_highmaul_chain_hurl'),
(160521, 'spell_highmaul_vile_breath'),
(160131, 'spell_highmaul_obscured'),
(163392, 'spell_highmaul_crowd_minion_killed'),
(163302, 'spell_highmaul_roar_of_the_crowd'),
(163130, 'spell_highmaul_inflamed'),
(163408, 'spell_highmaul_heckle'),
(159001, 'spell_highmaul_berserker_rush_periodic'),
(159212, 'spell_highmaul_blade_dance');

DELETE FROM conditions WHERE SourceEntry IN (162231, 177705, 159712, 160061, 159948, 160904);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 162231, 31, 0, 3, 80048),
(13, 1, 177705, 31, 0, 3, 80048),
(13, 1, 159712, 31, 0, 3, 78757),
(13, 1, 160061, 31, 0, 3, 79260),
(13, 1, 159948, 31, 0, 3, 79134),
(13, 1, 160904, 31, 0, 3, 0);

DELETE FROM areatrigger_template WHERE spell_id IN (161634, 159202, 159412);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUES
(161634, 1, 6867, 4, 4, 16384, 'areatrigger_highmaul_molten_bomb'),
(159202, 0, 6701, 10, 10, 16388, 'areatrigger_highmaul_flame_jet'),
(159412, 1, 6706, 4, 4, 16384, 'areatrigger_highmaul_mauling_brew');

SET @REF_KARGATH_N = 78714;
SET @REF_KARGATH_LFR = 1778714;

DELETE FROM `reference_loot_template` WHERE entry = @REF_KARGATH_N;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KARGATH_N, 113591, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113592, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113593, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113595, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113596, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113598, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113599, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113600, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113601, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113602, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113604, 0, 1, 1, 1, 1),
(@REF_KARGATH_N, 113605, 0, 1, 1, 1, 1);

DELETE FROM `reference_loot_template` WHERE entry = @REF_KARGATH_LFR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KARGATH_LFR, 116003, 0, 1, 1, 1, 1),
(@REF_KARGATH_LFR, 116030, 0, 1, 1, 1, 1),
(@REF_KARGATH_LFR, 116289, 0, 1, 1, 1, 1),
(@REF_KARGATH_LFR, 116236, 0, 1, 1, 1, 1),
(@REF_KARGATH_LFR, 116205, 0, 1, 1, 1, 1),
(@REF_KARGATH_LFR, 116282, 0, 1, 1, 1, 1),
(@REF_KARGATH_LFR, 116360, 0, 1, 1, 1, 1),
(@REF_KARGATH_LFR, 116298, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_KARGATH_N WHERE `entry`= @REF_KARGATH_N;
UPDATE `creature_template` SET `lootid`= @REF_KARGATH_LFR WHERE `entry`= @REF_KARGATH_LFR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_KARGATH_N;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KARGATH_N, 1, 100, 1, 0, -@REF_KARGATH_N, 6);

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_KARGATH_LFR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KARGATH_LFR, 1, 100, 1, 0, -@REF_KARGATH_LFR, 6);