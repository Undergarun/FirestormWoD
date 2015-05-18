
DELETE FROM `creature` WHERE `id`=55294 AND `map`=967;

/*
UPDATE `creature` SET `phaseMask`=17 WHERE `guid`=344360 AND `map`=967;
UPDATE `creature` SET `phaseMask`=17 WHERE `guid`=344326 AND `map`=967;
UPDATE `creature` SET `phaseMask`=17 WHERE `guid`=344319 AND `map`=967;
UPDATE `creature` SET `phaseMask`=17 WHERE `guid`=344357 AND `map`=967;
UPDATE `creature` SET `phaseMask`=17 WHERE `guid`=344364 AND `map`=967;
UPDATE `creature` SET `phaseMask`=17 WHERE `guid`=343321 AND `map`=967;
*/
UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry` IN (55294, 56576, 56577, 56578);

UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=56668;
UPDATE `creature_template_addon` SET `auras`='109247' WHERE `entry` IN (55294, 56576, 56577, 56578);

UPDATE `creature_template` SET `ScriptName`='boss_ultraxion' WHERE `entry`=55294;
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_thrall' WHERE `entry` IN (56667);

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(109176, 103327, 105925, 110070, 
110069, 110068, 109075, 110080, 
110079, 110078, 106162, 110327,
106108, 105984);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(109176, 'spell_ultraxion_twilight_instability'),
(103327, 'spell_ultraxion_hour_of_twilight_dmg'),
(105925, 'spell_ultraxion_fading_light'),
(110070, 'spell_ultraxion_fading_light'),
(110069, 'spell_ultraxion_fading_light'),
(110068, 'spell_ultraxion_fading_light'),
(109075, 'spell_ultraxion_fading_light'),
(110080, 'spell_ultraxion_fading_light'),
(110079, 'spell_ultraxion_fading_light'),
(110078, 'spell_ultraxion_fading_light'),
(106162, 'spell_ultraxion_last_defender_of_azeroth'),
(110327, 'spell_ultraxion_last_defender_of_azeroth_dummy'),
(106108, 'spell_ultraxion_heroic_will'),
(105984, 'spell_ultraxion_time_loop');

DELETE FROM `conditions` WHERE `SourceEntry` IN 
(108472, 108471, 108473, 108243, 
108242, 105929, 106042, 106044, 
109345, 109440, 106049, 106047, 
109344, 109342, 106050, 106048, 
109347, 109346);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 108472, 0, 31, 0, 3, 56668, 'Ultraxion - Charge Dragon Soul'),
(13, 1, 108471, 0, 31, 0, 3, 56668, 'Ultraxion - Charge Dragon Soul'),
(13, 1, 108243, 0, 31, 0, 3, 56668, 'Ultraxion - Charge Dragon Soul'),
(13, 1, 108242, 0, 31, 0, 3, 56668, 'Ultraxion - Charge Dragon Soul'),
(13, 1, 108473, 0, 31, 0, 3, 56668, 'Ultraxion - Charge Dragon Soul'),
(13, 1, 105929, 0, 31, 0, 3, 55294, 'Ultraxion - Ultraxion Normal Realm Cosmetic'),
(13, 1, 106042, 0, 31, 0, 3, 45979, 'Ultraxion - Gift of Live'),
(13, 1, 106044, 0, 31, 0, 3, 45979, 'Ultraxion - Gift of Live'),
(13, 1, 109345, 0, 31, 0, 3, 45979, 'Ultraxion - Gift of Live'),
(13, 1, 109440, 0, 31, 0, 3, 45979, 'Ultraxion - Gift of Live'),
(13, 1, 106049, 0, 31, 0, 3, 45979, 'Ultraxion - Essence of Dreams'),
(13, 1, 106047, 0, 31, 0, 3, 45979, 'Ultraxion - Essence of Dreams'),
(13, 1, 109344, 0, 31, 0, 3, 45979, 'Ultraxion - Essence of Dreams'),
(13, 1, 109342, 0, 31, 0, 3, 45979, 'Ultraxion - Essence of Dreams'),
(13, 1, 106050, 0, 31, 0, 3, 45979, 'Ultraxion - Source of Magic'),
(13, 1, 106048, 0, 31, 0, 3, 45979, 'Ultraxion - Source of Magic'),
(13, 1, 109347, 0, 31, 0, 3, 45979, 'Ultraxion - Source of Magic'),
(13, 1, 109346, 0, 31, 0, 3, 45979, 'Ultraxion - Source of Magic');

REPLACE INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(105900, 0, 0, 0, 0, 0, 278528, 0, 0, 0, 0);

DELETE FROM `spell_group` WHERE `id`=3000;
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (3000, 105896);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (3000, 105900);
INSERT INTO `spell_group` (`id`, `spell_id`) VALUES (3000, 105903);
REPLACE INTO `spell_group_stack_rules` (`group_id`, `stack_rule`) VALUES (3000, 1);


DELETE FROM `creature_text` WHERE `entry`=55294;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55294, 0, 0, 'Now is the Hour of Twilight!', 14, 0, 100, 0, 0, 26314, 'Ultraxion aggro'),
(55294, 1, 0, 'I WILL DRAG YOU WITH ME INTO FLAME AND DARKNESS!', 14, 0, 100, 0, 0, 26315, 'Ultraxion berserk'),
(55294, 2, 0, 'But...but...I am...Ul...trax...ionnnnnn...', 14, 0, 100, 0, 0, 26316, 'Ultraxion death'),
(55294, 3, 0, 'I am the beginning of the end...the shadow which blots out the sun...the bell which tolls your doom...', 14, 0, 100, 0, 0, 26317, 'Ultraxion intro 01'),
(55294, 4, 0, 'For this moment ALONE was I made. Look upon your death, mortals, and despair!', 14, 0, 100, 0, 0, 26318, 'Ultraxion intro 02'),
(55294, 5, 0, 'Fall before Ultraxion!', 14, 0, 100, 0, 0, 26319, 'Ultraxion slay 01'),
(55294, 5, 1, 'You have no hope!', 14, 0, 100, 0, 0, 26320, 'Ultraxion slay 02'),
(55294, 5, 2, 'Hahahahaha!', 14, 0, 100, 0, 0, 26321, 'Ultraxion slay 03'),
(55294, 6, 0, 'The final shred of light fades, and with it, your pitiful mortal existence!', 14, 0, 100, 0, 0, 26323, 'Ultraxion spell 01'),
(55294, 7, 0, 'Through the pain and fire my hatred burns!', 14, 0, 100, 0, 0, 26325, 'Ultraxion spell 03');

DELETE FROM `locales_creature_text` WHERE `entry`=55294;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55294, 0, 0, 'Настало Время Сумерек!'),
(55294, 1, 0, 'Я ЗАБЕРУ ВАС С СОБОЙ ВО ТЬМУ И ПЛАМЯ!'),
(55294, 2, 0, 'Но... но... я Уль... тра... ксиоооон.'),
(55294, 3, 0, 'Я – начало конца... Тень, что заслоняет солнце... Звонящий по вам колокол...'),
(55294, 4, 0, 'Я был создан ради этого единственного момента. Узрите же свою погибель, смертные!'),
(55294, 5, 0, 'Пади перед Ультраксионом!'),
(55294, 5, 1, 'Тебе не на что надеяться!'),
(55294, 5, 2, 'Ха-ха-ха!'),
(55294, 6, 0, 'Угасают последние отблески света, а с ними – и ваши жалкие жизни!'),
(55294, 7, 0, 'Огонь и боль питают мою ненависть!');

DELETE FROM `creature_text` WHERE `entry`=56630;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(56630, 0, 0, 'They... are my clutch no longer. Bring them down.', 12, 0, 100, 0, 0, 26505, 'Alextrasza pre ultraxion'),
(56630, 1, 0, 'Take heart, heroes, life will always blossom from the darkest soil!', 14, 0, 100, 0, 0, 26506, 'Alextrasza ultraxion'),
(56630, 2, 0, 'It is done! Our power now resides within the Dragon Soul! Our fate lies with you, Earth-Warder!', 12, 0, 100, 0, 0, 26507, 'Alextrasza ultraxion victory');

DELETE FROM `locales_creature_text` WHERE `entry`=56630;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(56630, 0, 0, 'Они... больше не мои дети. Убейте их.'),
(56630, 1, 0, 'Крепитесь, герои. Свет жизни пробьется даже сквозь самую страшную тьму!'),
(56630, 2, 0, 'Свершилось! Теперь наша сила в Душе Дракона! Наша судьба – в твоих руках, хранитель Земли!');

DELETE FROM `creature_text` WHERE `entry`=56667;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(56667, 0, 0, 'Strength of the Earth, hear my call! Shield them in this dark hour, the last defenders of Azeroth!', 14, 0, 100, 0, 0, 25907, 'Thrall ultraxion'),
(56667, 1, 0, 'Taretha... Cairne... Aggra... I will not fail you. I will not fail this world!', 12, 0, 100, 0, 0, 25908, 'Thrall ultraxion victory');

DELETE FROM `locales_creature_text` WHERE `entry`=56667;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(56667, 0, 0, 'Сила Земли, храни их в этот темный час, ибо они последние защитники Азерота!'),
(56667, 1, 0, 'Тарета...Кэрн... Аггра... Я не подведу вас. Я не подведу этот мир!');

DELETE FROM `creature_text` WHERE `entry`=56665;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(56665, 0, 0, 'I sense a great disturbance in the balance approaching. The chaos of it burns my mind!', 12, 0, 100, 0, 0, 26148, 'Ysera pre ultraxion'),
(56665, 1, 0, 'In dreams, we may overcome any obstacle.', 14, 0, 100, 0, 0, 26148, 'Ysera ultraxion');

DELETE FROM `locales_creature_text` WHERE `entry`=56665;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(56665, 0, 0, 'Я чувствую приближение Хаоса... Мой разум не в силах этого выдержать!'),
(56665, 1, 0, 'Во снах мы способны преодолеть любые препятствия!');

DELETE FROM `creature_text` WHERE `entry`=56664;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(56664, 0, 0, 'Winds of the arcane be at their backs, and refresh them in this hour of darkness!', 14, 0, 100, 0, 0, 26267, 'Kalecgos ultraxion');

DELETE FROM `locales_creature_text` WHERE `entry`=56664;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(56664, 0, 0, 'Ветра тайной магии, направьте их и придайте им сил в этот темный час!');

DELETE FROM `creature_text` WHERE `entry`=56666;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(56666, 0, 0, 'The cycle of time brings an end to all things.', 14, 0, 100, 0, 0, 25954, 'Nozdormu ultraxion');

DELETE FROM `locales_creature_text` WHERE `entry`=56666;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(56666, 0, 0, 'Со временем всему приходит конец.');