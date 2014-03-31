-- DELETE FROM `creature` WHERE `guid`=340222 AND `id`=45979;
-- DELETE FROM `creature_addon` WHERE `guid`=340222;
DELETE FROM `creature` WHERE `map`=938 AND `id` IN 
(54574, 54580, 54581, 54582, 54939, 54940, 54941, 54942, 54197, 54952);

UPDATE `creature` SET `spawnmask`=0 WHERE `map`=938 AND `id` IN (54700, 54688, 54699, 54512, 54701);

UPDATE `creature` SET `position_x`=4169.71, `position_y`=-433.40, `position_z`=120.0, `orientation`=2.59 WHERE `id`=54432;
-- UPDATE `creature_addon` SET `bytes1`=0 WHERE `guid`=340022;
UPDATE `creature_template_addon` SET `bytes1`=0 WHERE `entry`=54432;
UPDATE `creature_template` SET `inhabittype`=1 WHERE `entry`=54432;

UPDATE `creature_template_addon` SET `auras`='102150' WHERE `entry` IN (54574, 54580, 54581, 54582);

UPDATE `creature_template` SET `unit_flags`=32832 WHERE `entry`=54544;
UPDATE `creature_template_addon` SET `bytes1`=0, `auras`='' WHERE `entry`=54544;
UPDATE `creature_template` SET `modelid2`=0, `Inhabittype`=7, `flags_extra`=128 WHERE `entry` IN (54574, 54580, 54581, 54582);
UPDATE `gameobject_template` SET `faction`=35, `flags`=0 WHERE `entry` IN (209438, 209441, 209440, 209442, 209443, 209437, 209439);

UPDATE `instance_template` SET `script`='instance_end_time' WHERE `map`=938;

UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry` IN (209438, 209441, 209440, 209442, 209443, 209437, 209439);

UPDATE `creature_template` SET `ScriptName`='boss_echo_of_tyrande' WHERE `entry`=54544;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_moonlance' WHERE `entry` IN (54574, 54580, 54581, 54582); 

UPDATE `creature_template` SET `ScriptName`='boss_echo_of_sylvanas' WHERE `entry`=54123;

UPDATE `creature_template` SET `ScriptName`='boss_murozond' WHERE `entry`=54432;

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102242, 'spell_echo_of_tyrande_tears_of_elune_script');

DELETE FROM `creature_text` WHERE `entry` = 54544;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54544, 0, 0, 'Let the peaceful light of Elune sooth your souls in this dark time.', 12, 0, 100, 0, 0, 25972, 'Tyrande '),
(54544, 1, 0, 'Moon goddess, your light dims! I am lost without your guidance!', 14, 0, 100, 0, 0, 25973, 'Tyrande 80%'),
(54544, 2, 0, 'Tyrande gains Lunar Guidance!', 41, 0, 100, 0, 0, 0, 'Tyrande 80% and 55% emote'),
(54544, 3, 0, 'The darkness closes in...my vision is clouded...', 14, 0, 100, 0, 0, 25974, 'Tyrande 55%'),
(54544, 4, 0, 'Mother moon, I can no longer see your light! Your daughter is alone in the darkness!', 14, 0, 100, 0, 0, 25975, 'Tyrande '),
(54544, 5, 0, 'Tears of Elune begin to rain from the sky!', 41, 0, 100, 0, 0, 0, 'Tyrande 30% emote'),
(54544, 6, 0, 'I can...see the light of the moon...so clearly now. It is...beautiful...', 12, 0, 100, 0, 0, 25976, 'Tyrande death'),
(54544, 7, 0, 'There is nothing left for you here, nothing but death and sorrow.', 12, 0, 100, 0, 0, 25977, 'Tyrande intro 01'),
(54544, 8, 0, 'The darkness surrounds you, the light of Elune is your only salvation.', 12, 0, 100, 0, 0, 25978, 'Tyrande event 02'),
(54544, 9, 0, 'The moonlight can bring rest to your weary souls in this forgotten place.', 12, 0, 100, 0, 0, 25979, 'Tyrande event 03'),
(54544, 10, 0, 'Give yourselves to the night, Elune will guide you from this mortal prison.', 12, 0, 100, 0, 0, 25980, 'Tyrande event 04'),
(54544, 11, 0, 'You have chosen a path of darkness. Mother moon, guide my hand; allow me to bring rest to these misbegotten souls.', 12, 0, 100, 0, 0, 25981, 'Tyrande event 05'),
(54544, 12, 0, 'Elune guide you through the night.', 12, 0, 100, 0, 0, 25982, 'Tyrande kill'),
(54544, 13, 0, 'Eyes of night, pierce this darkness!', 14, 0, 100, 0, 0, 25983, 'Tyrande spell 01'),
(54544, 14, 0, 'Spear of Elune, drive back the night!', 14, 0, 100, 0, 0, 25984, 'Tyrande spell 02');

DELETE FROM `locales_creature_text` WHERE `entry` = 54544;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54544, 0, 0, 'Пусть свет Элуны дарует покой вашим мятежным душам.'),
(54544, 1, 0, 'Богиня, твой свет угасает! Я не справлюсь без твоей помощи...'),
(54544, 2, 0, 'Тиранде даровано лунное Покровительство!'),
(54544, 3, 0, 'Тьма наступает... Все скрывается во мраке...'),
(54544, 4, 0, 'Мать-луна, я не вижу твоего света! Твоя дочь... заблудилась во тьме!'),
(54544, 5, 0, 'С неба льются слезы Элуны!'),
(54544, 6, 0, 'Я так... ясно вижу... свет луны. Она... прекрасна...'),
(54544, 7, 0, 'Здесь вы не найдете ничего, кроме смерти и скорби.'),
(54544, 8, 0, 'Вас окружает тьма, и лишь в свете Элуны вы обретете спасение.'),
(54544, 9, 0, 'Лунный свет дарует вам покой в этом забытом месте.'),
(54544, 10, 0, 'Доверьтесь ночи, и Элуна выведет вас из этой темницы.'),
(54544, 11, 0, 'Вы выбрали путь тьмы. Богиня луна, направь мою руку; позволь мне даровать покой этим заблудшим душам.'),
(54544, 12, 0, 'Да проведет тебя Элуна сквозь мрак ночи.'),
(54544, 13, 0, 'Очи богини, пронзите эту тьму!'),
(54544, 14, 0, 'Копье Элуны, заставь отступить ночь!');