-- Lady Jaina Proudmoore
UPDATE `creature_template` SET `ScriptName`='npc_jaina_or_sylvanas_hor' WHERE `entry`=37221;

-- Lady Sylvanas Windrunner
UPDATE `creature_template` SET `ScriptName`='npc_jaina_or_sylvanas_hor' WHERE `entry`=37223;

-- Frostmourne Altar Bunny (Quel'Delar)
UPDATE `creature_template` SET `InhabitType`=4,`flags_extra`=128 WHERE `entry`=37704;

-- Frostsworn General
UPDATE `creature_template` SET `ScriptName`='npc_frostworn_general' WHERE `entry`=36723;
UPDATE `creature_template` SET `faction_A`=16,`faction_H`=16 WHERE `entry`=37720; -- for hc

-- Spiritual Reflection
UPDATE `creature_template` SET /*`modelid1`=57,*/`ScriptName`='npc_spiritual_reflection' WHERE `entry`=37068;

-- Falric and Marwyn bind players to instance on heroic
UPDATE `creature_template` SET `flags_extra` = 1 WHERE `entry` IN (38599,38603);

-- Areatriggers
DELETE FROM `areatrigger_scripts` WHERE `entry` IN (5697,5632,5740);
INSERT INTO `areatrigger_scripts` VALUES
(5697,'at_hor_waves_restarter'),
(5632,'at_hor_intro_start'),
(5740, 'at_hor_door_teleport');

DELETE FROM `creature_text` WHERE `entry`=36723 OR `entry`=36954 OR `entry`=36955 OR `entry`=37554 OR `entry`=30344 OR `entry`=30824;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
-- Frostsworn General
(36723,0,0,'You are not worthy to face the Lich King!',14,0,100,0,0,16921,'general SAY_AGGRO'),
(36723,1,0,'Master, I have failed...',14,0,100,0,0,16922,'general SAY_DEATH'),
-- The Lich King (at chase event)
(36954,0,0,'Your allies have arrived, Jaina, just as you promised. You will all become powerful agents of the Scourge.',14,0,100,0,0,17212, 'Lich King SAY_LICH_KING_AGGRO_A'),
(36954,1,0,'I will not make the same mistake again, Sylvanas. This time there will be no escape. You will all serve me in death!',14,0,100,0,0,17213,'Lich King SAY_LICH_KING_AGGRO_H'),
(36954,2,0,'Succumb to the chill of the grave.',14,0,100,0,0,17218,'Lich King SAY_LICH_KING_WALL_02'),
(36954,3,0,'Another dead end.',14,0,100,0,0,17219,'Lich King SAY_LICH_KING_WALL_03'),
(36954,4,0,'How long can you fight it?',14,0,100,0,0,17220,'Lich King SAY_LICH_KING_WALL_04'),
(36954,5,0,'There is no escape!', 14,0,100,0,0,17217,'Lich King SAY_LICH_KING_WALL_01'),
(36954,6,0,'Rise minions, do not left them us!',14,0,100,0,0,17216,'Lich King SAY_LICH_KING_GNOUL'),
(36954,7,0,'Minions sees them. Bring their corpses back to me!',14,0,100,0,0,17222,'Lich King SAY_LICH_KING_ABON'),
(36954,8,0,'Death''s cold embrace awaits.',14,0,100,0,0,17221,'Lich King SAY_LICH_KING_WINTER'),
(36954,9,0,'Nowhere to run! You''re mine now...',14,0,100,0,0,17223,'Lich King SAY_LICH_KING_END_DUN'),
(36954,10,0,'All is lost!',14,0,100,0,0,17215,'Lich King SAY_LICH_KING_WIN'),
-- Lady Jaina Proudmoore (at chase event)
(36955,0,0,'He is too powerful, we must leave this place at once! My magic will hold him in place for only a short time! Come quickly, heroes!',14,0,100,1,0,16644,'Jaina SAY_JAINA_AGGRO'),
(36955,1,0,'I will destroy this barrier. You must hold the undead back!',14,0,100,0,0,16607,'Jaina SAY_JAINA_WALL_01'),
(36955,2,0,'Another ice wall! Keep the undead from interrupting my incantation so that I may bring this wall down!',14,0,100,0,0,16608,'Jaina SAY_JAINA_WALL_02'),
(36955,3,0,'He is toying with us! I will show him what happens to ice when it meets fire!',14,0,100,0,0,16609,'Jaina SAY_JAINA_WALL_03'),
(36955,4,0,'Your barriers can''t hold us back much longer, monster. I will shatter them all!',14,0,100,0,0,16610,'Jaina SAY_JAINA_WALL_04'),
(36955,5,0,'There''s an opening up ahead. GO NOW!', 14,0,100,1,0,16645,'Jaina SAY_JAINA_ESCAPE_01'),
(36955,6,0,'We''re almost there... Don''t give up!',14,0,100,0,0,16646,'Jaina SAY_JAINA_ESCAPE_02'),
(36955,7,0,'It... It''s a dead end. We have no choice but to fight. Steel yourself heroes, for this is our last stand!',14,0,100,1,0,16647,'Jaina SAY_JAINA_TRAP'),
(36955,8,0,'Forgive me heroes, I should have listened to Uther. I ... I had to see for myself, to look into his eyes one last time ... I am sorry.',14,0,100,1,0,16648,'Jaina SAY_JAINA_FINAL_2'),
(36955,9,0,'We now know what must be done. I will deliver this news to King Varian and Highlord Fordring.',14,0,100,1,0,16649,'Jaina SAY_JAINA_FINAL_3'),
-- Lady Sylvanas Windrunner (at chase event)
(37554,0,0,'He''s too powerful! Heroes, quickly, come to me! We must leave this place immediately! I will do what I can do hold him in place while we flee.',14,0,100,0,0,17058,'Sylvanas SAY_SYLVANA_AGGRO'),
(37554,1,0,'No wall can hold the Banshee Queen! Keep the undead at bay, heroes! I will tear this barrier down!',14,0,100,0,0,17029,'Sylvanas SAY_SYLVANA_WALL_01'),
(37554,2,0,'Another barrier? Stand strong, champions! I will bring the wall down!',14,0,100,0,0,17030,'Sylvanas SAY_SYLVANA_WALL_02'),
(37554,3,0,'I grow tired of these games, Arthas! Your walls can\'t stop me!',14,0,100,0,0,17031,'Sylvanas SAY_SYLVANA_WALL_03'),
(37554,4,0,'You won''t impede our escape, fiend. Keep the undead off me while I bring this barrier down!',14,0,100,0,0,17032,'Sylvanas SAY_SYLVANA_WALL_04'),
(37554,5,0,'There''s an opening up ahead. GO NOW!',14,0,100,1,0,17059,'Sylvanas SAY_SYLVANA_ESCAPE_01'),
(37554,6,0,'We''re almost there! Don''t give up!',14,0,100,0,0,17060,'Sylvanas SAY_SYLVANA_ESCAPE_02'),
(37554,7,0,'BLASTED DEAD END! So this is how it ends. Prepare yourselves, heroes, for today we make our final stand!',14,0,100,5,0,17061,'Sylvanas SAY_SYLVANA_TRAP'),
(37554,8,0,'We are safe... for now. His strength has increased tenfold since our last battle. It will take a mighty army to destroy the Lich King. An army greater than even the Horde can rouse.',14,0,100,1,0,17062,'Sylvanas SAY_SYLVANA_FINAL_2'),
-- High Captain Justin Bartlett
(30344,0,0,'Fire, fire!',14,0,100,0,0,16721,'Bartlett SAY_BARTLETT_FIRE'),
(30344,1,0,'Quickly, climb aboard! We mustn''t tarry here. There''s no telling when this whole mountainside will collapse!',14,0,100,0,0,16722,'Bartlett SAY_BARTLETT_FINAL_1'),
-- Sky-Reaver Korm Blackscar
(30824,0,0,'FIRE! FIRE!',14,0,100,0,0,0,'Korm SAY_KORM_FIRE'),
(30824,1,0,'Quickly, climb aboard! We mustn''t tarry here. There''s no telling when this whole mountainside will collapse!',14,0,100,0,0,0,'Korm SAY_KORM_FINAL_1');

-- DELETE FROM `creature` WHERE `guid` = 365133;
UPDATE `creature_template` SET `gossip_menu_id` = 10950 WHERE `entry` = 37223;
UPDATE `creature_template` SET `difficulty_entry_1` = 0 WHERE `entry` = 37221;

DELETE FROM `creature_text` WHERE `entry` IN (37226,37221,37223,37225,38112,38113);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
('37226','0','0','SILENCE, PALADIN!','14','0','100','0','0','17225','Lich King SAY_LK_INTRO_1'),
('37226','1','0','So you wish to commune with the dead? You shall have your wish.','14','0','100','0','0','17226','Lich King SAY_LK_INTRO_2'),
('37226','2','0','Falric. Marwyn. Bring their corpses to my chamber when you are through.','14','0','100','0','0','17227','Lich King SAY_LK_INTRO_3'),
('37226','3','0','Foolish girl. You seek that, which I killed long ago. He is nearly a ghost now. A fate ago in my mind.','14','0','100','0','0','17229','Lich King SAY_LK_JAINA_INTRO_END'),
('37226','4','0','I will not make the same mistake again. This time there will be no escape. You fail to serve me in undead. Now, all that remains for you is oblivion!','14','0','100','0','0','17228','Lich King SAY_LK_SYLVANAS_INTRO_END'),
('37221','0','0','The chill of this place... Brr... I can feel my blood freezing.','14','0','100','0','0','16631','Jaina SAY_JAINA_INTRO_1'),
('37221','1','0','What is that? Up ahead! Could it be... ? Heroes at my side!','14','0','100','0','0','16632','Jaina SAY_JAINA_INTRO_2'),
('37221','2','0','Frostmourne! The blade that destroyed our kingdom...','14','0','100','0','0','16633','Jaina SAY_JAINA_INTRO_3'),
('37221','3','0','Stand back! Touch that blade and your soul will be scarred for all eternity! I must attempt to commune with the spirits locked away within Frostmourne. Give me space, back up please!','14','0','100','0','0','16634','Jaina SAY_JAINA_INTRO_4'),
('37221','4','0','Uther! Dear Uther! ... I... I\'m so sorry.','12','0','100','0','0','16635','Jaina SAY_JAINA_INTRO_5'),
('37221','5','0','Arthas is here? Maybe I...','12','0','100','0','0','16636','Jaina SAY_JAINA_INTRO_6'),
('37221','6','0','But Uther, if there\'s any hope of reaching Arthas. I... I must try.','12','0','100','0','0','16637','Jaina SAY_JAINA_INTRO_7'),
('37221','7','0','Tell me how, Uther? How do I destroy my prince? My...','12','0','100','0','0','16638','Jaina SAY_JAINA_INTRO_8'),
('37221','8','0','You\'re right, Uther. Forgive me. I... I don\'t know what got a hold of me. We will deliver this information to the King and the knights that battle the Scourge within Icecrown Citadel.','12','0','100','0','0','16639','Jaina SAY_JAINA_INTRO_9'),
('37221','9','0','Who could bear such a burden?','12','0','100','0','0','16640','Jaina SAY_JAINA_INTRO_10'),
('37221','10','0','Then maybe there is still hope...','12','0','100','0','0','16641','Jaina SAY_JAINA_INTRO_11'),
('37221','11','0','You won\'t deny me this Arthas! I must know! I must find out!','14','0','100','0','0','16642','Jaina SAY_JAINA_INTRO_END'),
('37223','0','0','I... I don\'t believe it! Frostmourne stands before us, unguarded! Just as the Gnome claimed. Come, heroes!','14','0','100','0','0','17049','Sylvanas SAY_SYLVANAS_INTRO_1'),
('37223','1','0','Standing this close to the blade that ended my life... The pain... It is renewed.','14','0','100','0','0','17050','Sylvanas SAY_SYLVANAS_INTRO_2'),
('37223','2','0','I dare not touch it. Stand back! Stand back as I attempt to commune with the blade! Perhaps our salvation lies within...','14','0','100','0','0','17051','Sylvanas SAY_SYLVANAS_INTRO_3'),
('37223','3','0','Uther...Uther the Lightbringer. How...','12','0','100','0','0','17052','Sylvanas SAY_SYLVANAS_INTRO_4'),
('37223','4','0','The Lich King is here? Then my destiny shall be fulfilled today!','12','0','100','0','0','17053','Sylvanas SAY_SYLVANAS_INTRO_5'),
('37223','5','0','There must be a way... ','12','0','100','0','0','17054','Sylvanas SAY_SYLVANAS_INTRO_6'),
('37223','6','0','Who could bear such a burden?','12','0','100','0','0','17055','Sylvanas SAY_SYLVANAS_INTRO_7'),
('37223','7','0','The Frozen Throne...','12','0','100','0','0','17056','Sylvanas SAY_SYLVANAS_INTRO_8'),
('37223','8','0','You will not escape me that easily, Arthas! I will have my vengeance!','14','0','100','0','0','17057','Sylvanas SAY_SYLVANAS_INTRO_END'),
('37225','0','0','Jaina! Could it truly be you?','14','0','100','0','0','16666','Uther SAY_UTHER_INTRO_A2_1'),
('37225','1','0','Jaina you haven\'t much time. The Lich King sees what the sword sees. He will be here shortly!','12','0','100','0','0','16667','Uther SAY_UTHER_INTRO_A2_2'),
('37225','2','0','No, girl. Arthas is not here. Arthas is merely a presence within the Lich King\'s mind. A dwindling presence...','12','0','100','0','0','16668','Uther SAY_UTHER_INTRO_A2_3'),
('37225','3','0','Jaina, listen to me. You must destroy the Lich King. You cannot reason with him. He will kill you and your allies and raise you all as powerful soldiers of the Scourge.','12','0','100','0','0','16669','Uther SAY_UTHER_INTRO_A2_4'),
('37225','4','0','Snap out of it, girl. You must destroy the Lich King at the place where he merged with Ner\'zhul - atop the spire, at the Frozen Throne. It is the only way.','12','0','100','0','0','16670','Uther SAY_UTHER_INTRO_A2_5'),
('37225','5','0','There is... something else that you should know about the Lich King. Control over the Scourge must never be lost. Even if you were to strike down the Lich King, another would have to take his place. For without the control of its master, the Scourge would run rampant across the world - destroying all living things.','12','0','100','0','0','16671','Uther SAY_UTHER_INTRO_A2_6'),
('37225','6','0','A grand sacrifice by a noble soul...','12','0','100','0','0','16672','Uther SAY_UTHER_INTRO_A2_7'),
('37225','7','0','I do not know, Jaina. I suspect that the piece of Arthas that might be left inside the Lich King is all that holds the Scourge from annihilating Azeroth.','12','0','100','0','0','16673','Uther SAY_UTHER_INTRO_A2_8'),
('37225','8','0','No, Jaina! Aargh! He... He is coming! You... You must...','12','0','100','0','0','16674','Uther SAY_UTHER_INTRO_A2_9'),
('37225','9','0','Careful, girl. I\'ve heard talk of that cursed blade saving us before. Look around you and see what has been born of Frostmourne.','12','0','100','0','0','16659','Uther SAY_UTHER_INTRO_H2_1'),
('37225','10','0','You haven\'t much time. The Lich King sees what the sword sees. He will be here shortly.','12','0','100','0','0','16660','Uther SAY_UTHER_INTRO_H2_2'),
('37225','11','0','You cannot defeat the Lich King. Not here. You would be a fool to try. He will kill those who follow you and raise them as powerful servants of the Scourge. But for you, Sylvanas, his reward for you would be worse than the last.','12','0','100','0','0','16661','Uther SAY_UTHER_INTRO_H2_3'),
('37225','12','0','Perhaps, but know this: there must always be a Lich King. Even if you were to strike down Arthas, another would have to take his place, for without the control of the Lich King, the Scourge would wash over this world like locusts, destroying all that they touched.','12','0','100','0','0','16662','Uther SAY_UTHER_INTRO_H2_4'),
('37225','13','0','I do not know, Banshee Queen. I suspect that the piece of Arthas that might be left inside the Lich King is all that holds the Scourge from annihilating Azeroth.','12','0','100','0','0','16663','Uther SAY_UTHER_INTRO_H2_5'),
('37225','14','0','Alas, the only way to defeat the Lich King is to destroy him at the place he was created.','12','0','100','0','0','16664','Uther SAY_UTHER_INTRO_H2_6'),
('37225','15','0','I... Aargh... He... He is coming... You... You must...','12','0','100','0','0','16665','Uther SAY_UTHER_INTRO_H2_7'),
('38112','0','0','Men, women and children... None were spared the master\'s wrath. Your death will be no different.','14','0','100','0','0','16710','falric SAY_AGGRO'),
('38112','1','0','Sniveling maggot!','14','0','100','0','0','16711','falric SAY_SLAY_1'),
('38112','1','1','The children of Stratholme fought with more ferocity!','14','0','100','0','0','16712','falric SAY_SLAY_2'),
('38112','2','0','Marwyn, finish them...','14','0','100','0','0','16713','falric SAY_DEATH'),
('38112','3','0','Despair... so delicious...','14','0','100','0','0','16715','falric SAY_IMPENDING_DESPAIR'),
('38112','4','0','Fear... so exhilarating...','14','0','100','0','0','16716','falric SAY_DEFILING_HORROR'),
('38112','5','0','As you wish, my lord.','14','0','100','0','0','16717','Falric SAY_FALRIC_INTRO_1'),
('38112','6','0','Soldiers of Lordaeron, rise to meet your master\'s call!','14','0','100','0','0','16714','Falric SAY_FALRIC_INTRO_2'),
('38113','0','0','Death is all that you will find here!','14','0','100','0','0','16734','marwyn SAY_AGGRO'),
('38113','1','0','I saw the same look in his eyes when he died. Terenas could hardly believe it. Hahahaha!','14','0','100','0','0','16735','marwyn SAY_SLAY_1'),
('38113','1','1','Choke on your suffering!','14','0','100','0','0','16736','marwyn SAY_SLAY_2'),
('38113','2','0','Yes... Run... Run to meet your destiny... Its bitter, cold embrace, awaits you.','14','0','100','0','0','16737','marwyn SAY_DEATH'),
('38113','3','0','Your flesh has decayed before your very eyes!','14','0','100','0','0','16739','marwyn SAY_CORRUPTED_FLESH_1'),
('38113','3','1','Waste away into nothingness!','14','0','100','0','0','16740','marwyn SAY_CORRUPTED_FLESH_2'),
('38113','4','0','As you wish, my lord.','14','0','100','0','0','16741','marwyn SAY_MARWYN_INTRO_1');

DELETE FROM `locales_creature_text` WHERE `entry` IN (37226,37221,37223,37225,38112,38113,36723);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
('37226','0','0','Замолчи, паладин.'),
('37226','1','0','Так ты хочешь поговорить с мертвыми? Нет ничего проще!'),
('37226','2','0','Фалрик, Марвин, когда закончите, принесите их тела в мои покои.'),
('37226','3','0','Глупая девчонка! Тот кого ты ищещь давно убит! Теперь он лишь призрак, слабый отзвук в моем сознании!'),
('37226','4','0','Я не повторю прежней ошибки, Сильвана. На этот раз тебе не спастись. Ты не оправдала моего доверия и теперь тебя ждет только забвение!'),
('37221','0','0','Как же тут холодно... Кровь стынет в жилах.'),
('37221','1','0','Что это там впереди? Неужели? Скорее герои!'),
('37221','2','0','Ледяная Скорбь, клинок, разрушивший наше королевство...'),
('37221','3','0','Отойдите. Тот кто коснется этого клинка обречет себя на вечные муки. Я попытаюсь заговорить с душами заключенными в Ледяной скорби. Расступитесь... Чуть назад! Прошу.'),
('37221','4','0','Утер? Милый Утер! Мне... Мне так жаль.'),
('37221','5','0','Артас здесь? Может я...'),
('37221','6','0','Но если есть малейшая надежда вернуть Артаса... Я должна попытаться!'),
('37221','7','0','Но как Утер? Как мне убить моего принца, моего...'),
('37221','8','0','Ты прав Утер, прости меня... Я не знаю что на меня нашло. Мы передадим твои слова Королю и рыцарям, которые сражаются с Плетью в Цитадели Ледяной Короны.'),
('37221','9','0','Кому по силам такое бремя?'),
('37221','10','0','Но может еще есть надежда?'),
('37221','11','0','Ты от меня не отмахнешься Артас. Я должна понять, я должна знать.'),
('37223','0','0','Я... Я не верю своим глазам. Ледяная скорбь перед нами без всякой охраны! Как и говорил гном. Вперед герои!'),
('37223','1','0','Боль снова захлестывает меня, когда я так близко вижу меч, отнявший у меня жизнь.'),
('37223','2','0','Я не смею его коснуться. Назад! Отступите! Я попробую установить связь с мечом. Возможно, спасение находится внутри!'),
('37223','3','0','Утер? Утер Светоносный? Как...'),
('37223','4','0','Король - Лич здесь? Значит моя судьба решится сегодня!'),
('37223','5','0','Должен быть способ!'),
('37223','6','0','Кому по силам такое бремя?'),
('37223','7','0','Ледяной Трон!'),
('37223','8','0','Ты так просто от меня не уйдешь Артас. Я отомщу тебе!'),
('37225','0','0','Джайна? Неужели это ты?'),
('37225','1','0','Джайна, у вас мало времени. Король - Лич видит все что видит Ледяная Скорбь. Вскоре он будет здесь.'),
('37225','2','0','Нет девочка. Артаса здесь нет. Артас лишь тень, мелькающая в сознании Короля - Лича. Смутная тень.'),
('37225','3','0','Джайна послушай меня. Вам нужно уничтожить Короля - Лича. С ним нельзя договориться. Он убьет вас всех и превратит в могущественных воинов Плети.'),
('37225','4','0','Забудь об этом девочка. Короля - Лича нужно уничтожить на том месте, где он слился с Нерзулом. На самой вершине, у Ледяного Трона!'),
('37225','5','0','Тебе нужно знать еще кое что о Короле - Личе. Плеть не должна выйти из под контроля. Даже если вы убьете Короля - Лича, кто-то должен будет занять его место. Без Короля Плеть налетит на мир как стая саранчи и уничтожит все живое.'),
('37225','6','0','Великая жертва, благородной души.'),
('37225','7','0','Не знаю Джайна... мне кажется если бы не Артас, который все еще является частью Короля - Лича, Плеть давно бы уже уничтожила Азерот.'),
('37225','8','0','Нет Джайна... Эээээ... Он... Он приближается... Вы... Вы должны...'),
('37225','9','0','Осторожней девочка! Однажды мне уже говорили, что этот проклятый меч может нас спасти. Посмотри вокруг, и ты увидишь, что из этого вышло.'),
('37225','10','0','У вас мало времени. Король - Лич видит все что видит Ледяная Скорбь. Вскоре он будет здесь.'),
('37225','11','0','Вам не победить Короля - Лича. Покрайней мере не здесь. Глупо и пытаться. Он убьет твоих соратников и воскресит их как воинов плети. Но что до тебя Сильвана, он готовит тебе участь еще страшнее, чем в прошлый раз.'),
('37225','12','0','Возможно... Но знай! Король - Лич должен быть всегда. Даже если вы убьете Артаса кто то обязан будет занять его место. Лишившись правителя Плеть налетит на мир как стая саранчи и уничтожит все на своем пути.'),
('37225','13','0','Не знаю, Королева Баньши... Если бы не Артас, который все еще является частью Короля - Лича, Плеть давно бы уже уничтожила Азерот.'),
('37225','14','0','Увы единственый способ одолеть Короля - Лича - это убить его там где он был порожден.'),
('37225','15','0','Да... Эээээ... Он... Он приближается... Вы... Вы должны...'),
('38112','0','0','Мужчины, Женщины и дети... Никто не избежал гнева господина. Вы разделите их участь!'),
('38112','1','0','Сопливый червяк!'),
('38112','1','1','Стратхольмские детишки - и те сражались отчаяннее!'),
('38112','2','0','Марвин... Добей их...'),
('38112','3','0','Как сладостно отчаянье!'),
('38112','4','0','Как приятен страх!'),
('38112','5','0','Как пожелаете, мой господин!'),
('38112','6','0','Солдаты Лордерона, восстаньте по зову Господина!'),
('38113','0','0','Вы найдете здесь лишь смерть!'),
('38113','1','0','У Теренаса был такой же взгляд в миг смерти, он никак не мог поверить... Ха-ха-ха-ха-ха...'),
('38113','1','1','Захлебнись страданием!'),
('38113','2','0','Эээээ... Да... Бегите навстречу судьбе. Ее жестокие и холодные обьятия ждут вас...'),
('38113','3','0','Вы увидите как разлагается ваша плоть!'),
('38113','3','1','Сгиньте без следа!'),
('38113','4','0','Как пожелаете, мой господин!'),
('36723','0','0','Вы недостойны предстать перед Королем - Личом!'),
('36723','1','0','Господин... Я подвел вас...');

-- UPDATE `gossip_menu_option` SET `id` = 0 WHERE `menu_id` = 11031 AND `id` = 1;
-- UPDATE `gossip_menu_option` SET `id` = 1 WHERE `menu_id` = 11031 AND `id` = 2;

DELETE FROM `locales_gossip_menu_option` WHERE `menu_id` IN (10950,11031);
INSERT INTO `locales_gossip_menu_option` (`menu_id`,`id`,`option_text_loc8`) VALUES
(10950, 0, 'Ты можешь сделать что-нибудь с этим мечом?'),
(10950, 1, 'Темная Госпожа! Кажется, я слышу поступь Артаса. Чтобы вы ни задумали, поторопитесь!'),
(11031, 0, 'Ты можешь сделать что-нибудь с этим мечом?'),
(11031, 1, 'Моя Леди! Кажется, я слышу поступь Артаса. Чтобы вы ни задумали, поторопитесь!');

UPDATE `creature_template` SET `difficulty_entry_1` = 0 WHERE `entry` = 37221;

/*-----------------------------------------*/

UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'boss_lich_king_hor' WHERE `entry` = 36954;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'npc_jaina_and_sylvana_hor_part2' WHERE `entry` IN (36955,37554);

DELETE FROM `creature` WHERE `id` IN (37554, 37226);

DELETE FROM script_waypoint WHERE entry IN (36955,36954,37554);
INSERT INTO script_waypoint VALUES
-- Jaina
(36955, 0, 5587.682,2228.586,733.011, 0, 'WP1'),
(36955, 1, 5600.715,2209.058,731.618, 0, 'WP2'),
(36955, 2, 5606.417,2193.029,731.129, 0, 'WP3'),
(36955, 3, 5598.562,2167.806,730.918, 0, 'WP4 - Summon IceWall 01'),
(36955, 4, 5556.436,2099.827,731.827, 0, 'WP5 - Spell Channel'),
(36955, 5, 5543.498,2071.234,731.702, 0, 'WP6'),
(36955, 6, 5528.969,2036.121,731.407, 0, 'WP7'),
(36955, 7, 5512.045,1996.702,735.122, 0, 'WP8'),
(36955, 8, 5504.490,1988.789,735.886, 0, 'WP9 - Spell Channel'),
(36955, 9, 5489.645,1966.389,737.653, 0, 'WP10'),
(36955, 10, 5475.517,1943.176,741.146, 0, 'WP11'),
(36955, 11, 5466.930,1926.049,743.536, 0, 'WP12'),
(36955, 12, 5445.157,1894.955,748.757, 0, 'WP13 - Spell Channel'),
(36955, 13, 5425.907,1869.708,753.237, 0, 'WP14'),
(36955, 14, 5405.118,1833.937,757.486, 0, 'WP15'),
(36955, 15, 5370.324,1799.375,761.007, 0, 'WP16'),
(36955, 16, 5335.422,1766.951,767.635, 0, 'WP17 - Spell Channel'),
(36955, 17, 5311.438,1739.390,774.165, 0, 'WP18'),
(36955, 18, 5283.589,1703.755,784.176, 0, 'WP19'),
(36955, 19, 5260.400,1677.775,784.301, 3000, 'WP20'),
(36955, 20, 5262.439,1680.410,784.294, 0, 'WP21'),
(36955, 21, 5260.400,1677.775,784.301, 0, 'WP22'),
-- Sylvana
(37554, 0, 5587.682,2228.586,733.011, 0, 'WP1'),
(37554, 1, 5600.715,2209.058,731.618, 0, 'WP2'),
(37554, 2, 5606.417,2193.029,731.129, 0, 'WP3'),
(37554, 3, 5598.562,2167.806,730.918, 0, 'WP4 - Summon IceWall 01'),
(37554, 4, 5556.436,2099.827,731.827, 0, 'WP5 - Spell Channel'),
(37554, 5, 5543.498,2071.234,731.702, 0, 'WP6'),
(37554, 6, 5528.969,2036.121,731.407, 0, 'WP7'),
(37554, 7, 5512.045,1996.702,735.122, 0, 'WP8'),
(37554, 8, 5504.490,1988.789,735.886, 0, 'WP9 - Spell Channel'),
(37554, 9, 5489.645,1966.389,737.653, 0, 'WP10'),
(37554, 10, 5475.517,1943.176,741.146, 0, 'WP11'),
(37554, 11, 5466.930,1926.049,743.536, 0, 'WP12'),
(37554, 12, 5445.157,1894.955,748.757, 0, 'WP13 - Spell Channel'),
(37554, 13, 5425.907,1869.708,753.237, 0, 'WP14'),
(37554, 14, 5405.118,1833.937,757.486, 0, 'WP15'),
(37554, 15, 5370.324,1799.375,761.007, 0, 'WP16'),
(37554, 16, 5335.422,1766.951,767.635, 0, 'WP17 - Spell Channel'),
(37554, 17, 5311.438,1739.390,774.165, 0, 'WP18'),
(37554, 18, 5283.589,1703.755,784.176, 0, 'WP19'),
(37554, 19, 5260.400,1677.775,784.301, 3000, 'WP20'),
(37554, 20, 5262.439,1680.410,784.294, 0, 'WP21'),
(37554, 21, 5260.400,1677.775,784.301, 0, 'WP22'),
-- Lich King
(36954, 0, 5577.187,2236.003,733.012, 0, 'WP1'),
(36954, 1, 5587.682,2228.586,733.011, 0, 'WP2'),
(36954, 2, 5600.715,2209.058,731.618, 0, 'WP3'),
(36954, 3, 5606.417,2193.029,731.129, 0, 'WP4'),
(36954, 4, 5598.562,2167.806,730.918, 0, 'WP5'),
(36954, 5, 5559.218,2106.802,731.229, 0, 'WP6'),
(36954, 6, 5543.498,2071.234,731.702, 0, 'WP7'),
(36954, 7, 5528.969,2036.121,731.407, 0, 'WP8'),
(36954, 8, 5512.045,1996.702,735.122, 0, 'WP9'),
(36954, 9, 5504.490,1988.789,735.886, 0, 'WP10'),
(36954, 10, 5489.645,1966.389,737.653, 0, 'WP10'),
(36954, 11, 5475.517,1943.176,741.146, 0, 'WP11'),
(36954, 12, 5466.930,1926.049,743.536, 0, 'WP12'),
(36954, 13, 5445.157,1894.955,748.757, 0, 'WP13'),
(36954, 14, 5425.907,1869.708,753.237, 0, 'WP14'),
(36954, 15, 5405.118,1833.937,757.486, 0, 'WP15'),
(36954, 16, 5370.324,1799.375,761.007, 0, 'WP16'),
(36954, 17, 5335.422,1766.951,767.635, 0, 'WP17'),
(36954, 18, 5311.438,1739.390,774.165, 0, 'WP18'),
(36954, 19, 5283.589,1703.755,784.176, 0, 'WP19'),
(36954, 20, 5278.694,1697.912,785.692, 0, 'WP20'),
(36954, 21, 5283.589,1703.755,784.176, 0, 'WP19');

UPDATE `creature_template` SET `equipment_id` = 2425, `speed_walk` = 2.5, `speed_run` = 2.5 WHERE `entry` = 36954;

DELETE FROM `conditions` WHERE `SourceEntry` = 70194;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13, 3, 70194, 0, 0, 31, 0, 3, 36954, 0, 0, 0, '', NULL);

-- The Lich King (at chase event)
DELETE FROM `creature_text` WHERE `entry` IN (36954,36955,37554,30344,30824);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(36954,0,0,'Your allies have arrived, Jaina, just as you promised. You will all become powerful agents of the Scourge.',14,0,100,0,0,17212, 'Lich King SAY_LICH_KING_AGGRO_A'),
(36954,1,0,'I will not make the same mistake again, Sylvanas. This time there will be no escape. You will all serve me in death!',14,0,100,0,0,17213,'Lich King SAY_LICH_KING_AGGRO_H'),
(36954,2,0,'There is no escape!', 14,0,100,0,0,17217,'Lich King SAY_LICH_KING_WALL_01'),
(36954,3,0,'Succumb to the chill of the grave.',14,0,100,0,0,17218,'Lich King SAY_LICH_KING_WALL_02'),
(36954,4,0,'Another dead end.',14,0,100,0,0,17219,'Lich King SAY_LICH_KING_WALL_03'),
(36954,5,0,'How long can you fight it?',14,0,100,0,0,17220,'Lich King SAY_LICH_KING_WALL_04'),
(36954,6,0,'Rise minions, do not left them us!',14,0,100,0,0,17216,'Lich King SAY_LICH_KING_GNOUL'),
(36954,7,0,'Minions sees them. Bring their corpses back to me!',14,0,100,0,0,17222,'Lich King SAY_LICH_KING_ABON'),
(36954,8,0,'Death''s cold embrace awaits.',14,0,100,0,0,17221,'Lich King SAY_LICH_KING_WINTER'),
(36954,9,0,'Nowhere to run! You''re mine now...',14,0,100,0,0,17223,'Lich King SAY_LICH_KING_END_DUN'),
(36954,10,0,'All is lost!',14,0,100,0,0,17215,'Lich King SAY_LICH_KING_WIN'),
-- Lady Jaina Proudmoore (at chase event)
(36955,0,0,'He is too powerful, we must leave this place at once! My magic will hold him in place for only a short time! Come quickly, heroes!',14,0,100,1,0,16644,'Jaina SAY_JAINA_AGGRO'),
(36955,1,0,'I will destroy this barrier. You must hold the undead back!',14,0,100,0,0,16607,'Jaina SAY_JAINA_WALL_01'),
(36955,2,0,'Another ice wall! Keep the undead from interrupting my incantation so that I may bring this wall down!',14,0,100,0,0,16608,'Jaina SAY_JAINA_WALL_02'),
(36955,3,0,'He is toying with us! I will show him what happens to ice when it meets fire!',14,0,100,0,0,16609,'Jaina SAY_JAINA_WALL_03'),
(36955,4,0,'Your barriers can''t hold us back much longer, monster. I will shatter them all!',14,0,100,0,0,16610,'Jaina SAY_JAINA_WALL_04'),
(36955,5,0,'There''s an opening up ahead. GO NOW!', 14,0,100,1,0,16645,'Jaina SAY_JAINA_ESCAPE_01'),
(36955,6,0,'We''re almost there... Don''t give up!',14,0,100,0,0,16646,'Jaina SAY_JAINA_ESCAPE_02'),
(36955,7,0,'It... It''s a dead end. We have no choice but to fight. Steel yourself heroes, for this is our last stand!',14,0,100,1,0,16647,'Jaina SAY_JAINA_TRAP'),
(36955,8,0,'Forgive me heroes, I should have listened to Uther. I ... I had to see for myself, to look into his eyes one last time ... I am sorry.',14,0,100,1,0,16648,'Jaina SAY_JAINA_FINAL_1'),
(36955,9,0,'We now know what must be done. I will deliver this news to King Varian and Highlord Fordring.',14,0,100,1,0,16649,'Jaina SAY_JAINA_FINAL_2'),
-- Lady Sylvanas Windrunner (at chase event)
(37554,0,0,'He''s too powerful! Heroes, quickly, come to me! We must leave this place immediately! I will do what I can do hold him in place while we flee.',14,0,100,0,0,17058,'Sylvanas SAY_SYLVANA_AGGRO'),
(37554,1,0,'No wall can hold the Banshee Queen! Keep the undead at bay, heroes! I will tear this barrier down!',14,0,100,0,0,17029,'Sylvanas SAY_SYLVANA_WALL_01'),
(37554,2,0,'Another barrier? Stand strong, champions! I will bring the wall down!',14,0,100,0,0,17030,'Sylvanas SAY_SYLVANA_WALL_02'),
(37554,3,0,'I grow tired of these games, Arthas! Your walls can\'t stop me!',14,0,100,0,0,17031,'Sylvanas SAY_SYLVANA_WALL_03'),
(37554,4,0,'You won''t impede our escape, fiend. Keep the undead off me while I bring this barrier down!',14,0,100,0,0,17032,'Sylvanas SAY_SYLVANA_WALL_04'),
(37554,5,0,'There''s an opening up ahead. GO NOW!',14,0,100,1,0,17059,'Sylvanas SAY_SYLVANA_ESCAPE_01'),
(37554,6,0,'We''re almost there! Don''t give up!',14,0,100,0,0,17060,'Sylvanas SAY_SYLVANA_ESCAPE_02'),
(37554,7,0,'BLASTED DEAD END! So this is how it ends. Prepare yourselves, heroes, for today we make our final stand!',14,0,100,5,0,17061,'Sylvanas SAY_SYLVANA_TRAP'),
(37554,8,0,'We are safe... for now. His strength has increased tenfold since our last battle. It will take a mighty army to destroy the Lich King. An army greater than even the Horde can rouse.',14,0,100,1,0,17062,'Sylvanas SAY_SYLVANA_FINAL_2'),
-- High Captain Justin Bartlett
(37182,0,0,'Fire, fire!',14,0,100,0,0,16721,'Bartlett SAY_BARTLETT_FIRE'),
(37182,1,0,'Quickly, climb aboard! We mustn''t tarry here. There''s no telling when this whole mountainside will collapse!',14,0,100,0,0,16722,'Bartlett SAY_BARTLETT_FINAL_1'),
-- Sky-Reaver Korm Blackscar
(37833,0,0,'FIRE! FIRE!',14,0,100,0,0,0,'Korm SAY_KORM_FIRE'),
(37833,1,0,'Quickly, climb aboard! We mustn''t tarry here. There''s no telling when this whole mountainside will collapse!',14,0,100,0,0,0,'Korm SAY_KORM_FINAL_1');

DELETE FROM `locales_creature_text` WHERE `entry` IN (36954,36955,37554,37182,37833);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(36954,0,0,'Твои союзники прибыли, Джайна! Как ты и обещала... Ха-ха-ха-ха... Все вы станете могучими солдатами Плети...'),
(36954,1,0,'Я не повторю прежней ошибки, Сильвана! На этот раз тебе не спастись. Вы все будите служить мне после смерти...'),
(36954,2,0,'Бежать некуда!'),
(36954,3,0,'Покоритесь Леденящей смерти!'),
(36954,4,0,'Вы в ловушке!'),
(36954,5,0,'Как долго вы сможете сопротивляться?'),
(36954,6,0,'Восстаньте прислужники, не дайте им сбежать!'),
(36954,7,0,'Схватите их! Принесите мне их тела!'),
(36954,8,0,'Смерть распростерла ледяные объятия.'),
(36954,9,0,'Ха-ха-ха... Бежать некуда. Теперь вы мои!'),
(36954,10,0,'Смирись с судьбой.'),
(36955,0,0,'Он слишком силен. Мы должны выбраться от сюда как можно скорее. Моя магия задержит его ненадолго, быстрее герои...'),
(36955,1,0,'Я разрушу эту преграду, а вы удерживайте нежить на расстоянии!'),
(36955,2,0,'Опять ледяная стена... Я разобью ее, только не дайте нежити прервать мои заклинания...'),
(36955,3,0,'Он с нами играет. Я  покажу ему что бывает когда лед встречается со огнем!'),
(36955,4,0,'Твои преграды больше не задержат нас, чудовище. Я смету их с пути!'),
(36955,5,0,'Я вижу выход, скорее!'),
(36955,6,0,'Мы почти выбрались, не сдавайтесь!'),
(36955,7,0,'Больше некуда бежать. Теперь нам придется сражаться. Это наша последняя битва!'),
(36955,8,0,'Простите меня. Надо было послушать Утера. Но мне... хотелось убедиться во всем самой. Последний раз взглянуть ему в глаза... Простите.'),
(36955,9,0,'Теперь мы знаем, что делать. Я расскажу обо всем королю Вариану и верховному лорду Фордрингу.'),
(37554,0,0,'Он слишком силен. Герои скорее, за мной. Мы должны выбраться отсюда немедленно. Я постараюсь его задержать, пока мы будем убегать.'),
(37554,1,0,'Никакие стены не удержат Королеву Баньши. Держите нежить на расстоянии, я сокрушу эту преграду.'),
(37554,2,0,'Еще одна преграда. Держитесь герои! Я разрушу эту стену!'),
(37554,3,0,'Я устала от этих игр Артас. Твои стены не остановят меня!'),
(37554,4,0,'Ты не помешаешь нам уйти, монстр. Сдерживайте нежить, а я уничтожу эту преграду.'),
(37554,5,0,'Я вижу выход, скорее!'),
(37554,6,0,'Мы почти выбрались, не сдавайтесь!'),
(37554,7,0,'Проклятый тупик, значит все закончится здесь. Готовьтесь герои, это наша последняя битва.'),
(37554,8,0,'Мы в безопасности... пока что. Его сила возросла десятикратно со времен нашей последней битвы. Чтобы победить Короля-лича, потребуется огромная армия. Куда большая, чем сможет собрать Орда.'),
(37182,0,0,'Огонь, огонь!'),
(37182,1,0,'Скорей, забирайтесь на корабль! Не мешкайте! Кто знает, когда эта скала обрушится.'),
(37833,0,0,'ОГОНЬ! ОГОНЬ!'),
(37833,1,0,'Скорей, забирайтесь на корабль! Не мешкайте! Кто знает, когда эта скала обрушится.');

DELETE FROM `locales_gossip_menu_option` WHERE `menu_id` IN (10909);
INSERT INTO `locales_gossip_menu_option` (`menu_id`,`id`,`option_text_loc8`) VALUES
(10909, 0, 'Мы готовы! Нужно убегать скорее!!');

UPDATE `gameobject_template` SET `faction` = 114, `flags` = 32 WHERE `entry` IN (197343,202236);

/*------------------------------------------------------------*/

DELETE FROM `creature` WHERE `id` IN (38112,38113,36723);
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(38112, 668, 3, 1, 0, 1, 5338.22, 1978.67, 709.319, 2.25591, 604800, 0, 0, 377468, 0, 0, 0, 0, 0),
(38113, 668, 3, 1, 0, 1, 5278.55, 2035.78, 709.32, 5.53337, 604800, 0, 0, 539240, 0, 0, 0, 0, 0),
(36723, 668, 3, 1, 0, 1, 5413.99, 2116.61, 707.694, 3.90288, 604800, 0, 0, 315000, 0, 0, 0, 0, 0);

DELETE FROM `creature_equip_template` WHERE `entry` IN (2433,2434);
INSERT INTO `creature_equip_template` (`entry`,`itemEntry1`,`itemEntry2`,`itemEntry3`) VALUES
(2433, 50249, 49777, 0),
(2434, 50248, 50248, 0);

UPDATE `creature_template` SET `equipment_id` = 2433 WHERE `entry` = 38112;
UPDATE `creature_template` SET `equipment_id` = 2434 WHERE `entry` = 38113;

UPDATE `creature_template` SET `unit_flags` = 33555264 WHERE `entry` IN (38175,38172,38177,38173,38176,38524,38525,38563,38544,38564);

DELETE FROM `spelldifficulty_dbc` WHERE `id` IN (72395,72396,72397);
INSERT INTO `spelldifficulty_dbc` (`id`,`spellid0`,`spellid1`,`spellid2`,`spellid3`) VALUES
(72395,72395,72390,0,0),
(72396,72396,72391,0,0),
(72397,72397,72393,0,0);

UPDATE `creature_template` SET `flags_extra` = 0x00000002 WHERE entry = 36954;

/*------------------------------------------------*/

UPDATE `creature_template` SET `equipment_id` = 2433 WHERE `entry` = 38599;
UPDATE `creature_template` SET `equipment_id` = 2434 WHERE `entry` = 38603;
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 36536;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_queldelar' WHERE `entry` = 37158;
UPDATE `creature` SET `position_x` = 5344.75, `position_y` = 1972.87 WHERE `id` = 38112;
UPDATE `creature` SET `position_x` = 5271.65, `position_y` = 2042.5 WHERE `id` = 38113;


/*----------------------------------------------------*/

DELETE FROM `creature` WHERE `map`=668 AND `id` IN
(38177, 38176, 38175, 38173, 
38172, 37906, 37593, 37582, 
37225, 37107, 37069, 37068,
36941, 36940, 36955, 36954, 
37554, 37221);

UPDATE `creature_template` SET `ScriptName`='npc_raging_gnoul' WHERE `entry`=36940;
UPDATE `creature_template` SET `ScriptName`='npc_risen_witch_doctor' WHERE `entry`=36941;
UPDATE `creature_template` SET `ScriptName`='npc_abon' WHERE `entry`=37069;

REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES 
(500001, 0, 9214, 'Ice Wall', '', '', '', 1375, 0, 2.5, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 1);

UPDATE `gameobject_template` SET `faction`=1375 WHERE `entry`=201385;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=201710;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=202212;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=202336;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=202337;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=202396;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=201385;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=201596;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=201710;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=202212;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=202336;
