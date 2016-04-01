
DELETE FROM `creature` WHERE `map`=940 AND `id` IN
(54600, 54995, 55415, 55549, 
54644, 54735, 55445,
55377, 55427, 55441, 55467,
55468, 55469);

UPDATE `creature` SET `spawnmask`=0 WHERE `map`=940 AND `id` IN 
(55779, 54972, 54634, 54971, 
55636, 55911, 55969, 54696, 
55913, 55967, 58233, 54646);

UPDATE `creature` SET `position_x`=4251.114258, `position_y`=599.126465, `position_z`=-5.82, `orientation`=5.70 WHERE `id`=54968 AND `map`=940;

UPDATE `creature` SET `phasemask`=2 WHERE `map`=940 AND `id`=55466;

UPDATE `creature_addon` SET `auras`='102254' WHERE `guid` IN (339389, 339390, 339391);

UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry` IN 
(54599, 54522, 55377, 55467, 55466);

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry` IN 
(54599, 54522, 54560, 54598, 
55384, 55385, 55389, 57197, 
54628, 55377, 55427, 55441, 
55467, 55468, 55469);

UPDATE `creature_template` SET `npcflag`=16777217 WHERE `entry`=55549;
UPDATE `creature_template` SET `scale`=1 WHERE `entry`=54628;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=54686;
UPDATE `creature_template` SET `spell1`=0 WHERE `entry` IN (55377, 55467);
UPDATE `creature_template` SET `scale`=1 WHERE `entry` IN (55377, 55467);

UPDATE `access_requirement` SET `level_min`=85 WHERE `mapId`=940 AND `difficulty`=1;

UPDATE `instance_template` SET `script`='instance_hour_of_twilight' WHERE `map`=940;

UPDATE `creature_template` SET `ScriptName`='boss_arcurion' WHERE `entry`=54590;
UPDATE `creature_template` SET `ScriptName`='npc_arcurion_frozen_servitor' WHERE `entry`=54600;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (102480);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102480, 'spell_arcurion_icy_boulder');

UPDATE `creature_template` SET `ScriptName`='boss_asira_dawnslayer' WHERE `entry`=54968;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (103419, 103562, 103597, 102726);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(103419, 'spell_asira_dawnslayer_blade_barrier'),
(103562, 'spell_asira_dawnslayer_blade_barrier'),
(103597, 'spell_asira_dawnslayer_throw_knife'),
(102726, 'spell_asira_dawnslayer_mark_of_silence');

UPDATE `creature_template` SET `ScriptName`='npc_hour_of_twilight_life_warden' WHERE `entry`=55549;

UPDATE `creature_template` SET `ScriptName`='boss_archbishop_benedictus' WHERE `entry`=54938;
UPDATE `creature_template` SET `ScriptName`='npc_archbishop_benedictus_purifying_light' WHERE `entry` IN (55377, 55468);
UPDATE `creature_template` SET `AIName`='PassiveAI', `ScriptName`='' WHERE `entry`=55466;


DELETE FROM `spell_script_names` WHERE `spell_id` IN (103600, 103768, 103149, 103362);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(103600, 'spell_archbishop_benedictus_purifying_light_targeting'),
(103768, 'spell_archbishop_benedictus_purifying_light_targeting'),
(103149, 'spell_archbishop_benedictus_righteous_snear_aoe'),
(103362, 'spell_archbishop_benedictus_righteous_snear_aoe');

DELETE FROM `conditions` WHERE `SourceEntry` IN (103762);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 103762, 0, 31, 0, 3, 54971, 'Archbishop Benedictus - Engulfing Twilight');

DELETE FROM `creature_text` WHERE `entry`=54590;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54590, 0, 0, 'Give up the Dragon Soul, and I may yet allow you to live.', 14, 0, 100, 0, 0, 25796, 'arcurion aggro'),
(54590, 1, 0, 'Nothing! Nothing...', 14, 0, 100, 0, 0, 25797, 'arcurion death'),
(54590, 2, 0, 'Shaman! The Dragon Soul is not yours. Give it up, and you may yet walk away with your life.', 14, 0, 100, 0, 0, 25798, 'arcurion event 1'),
(54590, 3, 0, 'Destroy them all, but bring the Shaman to me!', 14, 0, 100, 0, 0, 25799, 'arcurion event 2'),
(54590, 4, 0, 'You will go nowhere, Shaman.', 14, 0, 100, 0, 0, 25800, 'arcurion event 3'),
(54590, 5, 0, 'The Hour of Twilight falls - the end of all things - you cannot stop it. You are nothing. NOTHING!', 14, 0, 100, 0, 0, 25801, 'arcurion event 4'),
(54590, 6, 0, 'You are a fool if you think to take your place as the Aspect of Earth, Shaman!', 14, 0, 100, 0, 0, 25802, 'arcurion intro 1'),
(54590, 7, 0, 'Mere mortals...', 14, 0, 100, 0, 0, 25803, 'arcurion kill 1'),
(54590, 8, 0, 'You\'re a mere mortal. It is time you die like one.', 14, 0, 100, 0, 0, 25804, 'arcurion intro 2'),
(54590, 7, 1, 'Your shaman can\'t protect you.', 14, 0, 100, 0, 0, 25805, 'arcurion kill 2'),
(54590, 7, 2, 'The Aspects misplaced their trust.', 14, 0, 100, 0, 0, 25806, 'arcurion kill 3'),
(54590, 9, 0, 'Enough, shaman!', 14, 0, 100, 0, 0, 25807, 'arcurion spell 1'),
(54590, 9, 1, 'None will survive!', 14, 0, 100, 0, 0, 25808, 'arcurion spell 2'),
(54590, 9, 2, 'The Shaman is mine, focus on his companions!', 14, 0, 100, 0, 0, 25809, 'arcurion spell 3'),
(54590, 9, 3, 'Freeze!', 14, 0, 100, 0, 0, 25810, 'arcurion spell 4');

DELETE FROM `locales_creature_text` WHERE `entry`=54590;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54590, 0, 0, 'Отдайте душу дракона, и я возможно оставлю вас в живых.'),
(54590, 1, 0, 'Ничто! Вы ничто...'),
(54590, 2, 0, 'Шаман! Душа Дракона не твоя. Уступи, и может быть я сохраню тебе жизнь.'),
(54590, 3, 0, 'Перебейте их всех, а шамана приведите ко мне!'),
(54590, 4, 0, 'У тебя ничего не выйдет, шаман.'),
(54590, 5, 0, 'Грядет Время Сумерек – конец всего сущего. И вам не остановить его. Вы – ничто. НИЧТО!'),
(54590, 6, 0, 'Ты всерьез считаешь, что сможешь занять место аспекта земли?'),
(54590, 7, 0, 'Жалкие смертные.'),
(54590, 8, 0, 'Ты всего лишь смертный.'),
(54590, 7, 1, 'Твой шаман не смог тебя защитить.'),
(54590, 7, 2, 'Аспекты зря тебе доверились.'),
(54590, 9, 0, 'Довольно, шаман!'),
(54590, 9, 1, 'Не выживет никто!'),
(54590, 9, 2, 'Шаман – мой! Займитесь его подручными!'),
(54590, 9, 3, 'Застынь!');

DELETE FROM `creature_text` WHERE `entry`=54968;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54968, 0, 0, 'Let\'s get to work, shall we?', 14, 0, 100, 0, 0, 25816, 'asira aggro'),
(54968, 1, 0, 'You\'re much... better... than I thought...', 14, 0, 100, 0, 0, 25817, 'asira death'),
(54968, 2, 0, 'Where do you think you\'re going, little lizard?', 14, 0, 100, 0, 0, 25818, 'asira event 01'),
(54968, 3, 0, '...and with that out of the way, you and your flock of fumbling friends are next on my list. Mmm, I thought you\'d never get here!', 14, 0, 100, 0, 0, 25819, 'asira event 02'),
(54968, 4, 0, 'Well, that was even easier than I thought.', 14, 0, 100, 0, 0, 25820, 'asira event 03'),
(54968, 5, 0, 'So soon.', 14, 0, 100, 0, 0, 25821, 'asira slay 01'),
(54968, 5, 1, 'I hope your friends can do better!', 14, 0, 100, 0, 0, 25822, 'asira slay 02'),
(54968, 5, 2, 'Too much fun!', 14, 0, 100, 0, 0, 25823, 'asira slay 03'),
(54968, 5, 3, 'Good night.', 14, 0, 100, 0, 0, 25824, 'asira slay 04'),
(54968, 6, 0, 'Bye-bye.', 14, 0, 100, 0, 0, 25825, 'asira spell 01'),
(54968, 6, 1, 'Now you see me...', 14, 0, 100, 0, 0, 25826, 'asira spell 02'),
(54968, 6, 2, 'Surprise.', 14, 0, 100, 0, 0, 25827, 'asira spell 03');

DELETE FROM `locales_creature_text` WHERE `entry`=54968;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54968, 0, 0, 'Ну что, приступим?'),
(54968, 1, 0, 'Вы... намного сильнее... чем я думала...'),
(54968, 2, 0, 'Куда это ты направляешься, маленький проныра?'),
(54968, 3, 0, '...ну а теперь перейдем к тебе и стайке твоих неуклюжих друзей. Я уж думала, вы сюда никогда не доберетесь!'),
(54968, 4, 0, 'Хм, это было даже проще, чем я думала.'),
(54968, 5, 0, 'Так быстро.'),
(54968, 5, 1, 'Надеюсь, твои друзья продержатся дольше!'),
(54968, 5, 2, 'Какое веселье!'),
(54968, 5, 3, 'Спокойной ночи.'),
(54968, 6, 0, 'Пока-пока.'),
(54968, 6, 1, 'Вам меня не видно?'),
(54968, 6, 2, 'Сюрприз.');

REPLACE INTO `creature_loot_currency` (`creature_id`, `CurrencyId1`, `CurrencyId2`, `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3`) VALUES 
(54590, 395, 0, 0, 7000, 0, 0),
(54968, 395, 0, 0, 7000, 0, 0),
(54938, 395, 0, 0, 7000, 0, 0);
