/*
DELETE FROM `creature` WHERE `guid`=337779 AND `id`=52076;
DELETE FROM `creature_addon` WHERE `guid`=337779;
DELETE FROM `creature` WHERE `guid`=335706 AND `id`=52079;
DELETE FROM `creature_addon` WHERE `guid`=335706;
DELETE FROM `creature` WHERE `guid`=335710 AND `id`=52598;
DELETE FROM `creature_addon` WHERE `guid`=335710;
DELETE FROM `creature` WHERE `guid`=335675 AND `id`=52956;
DELETE FROM `creature_addon` WHERE `guid`=335675;
DELETE FROM `creature` WHERE `guid`=335677 AND `id`=52956;
DELETE FROM `creature_addon` WHERE `guid`=335677;
DELETE FROM `creature` WHERE `guid`=338988 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=338988;
DELETE FROM `creature` WHERE `guid`=338989 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=338989;
DELETE FROM `creature` WHERE `guid`=338990 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=338990;
DELETE FROM `creature` WHERE `guid`=336845 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=336845;
DELETE FROM `creature` WHERE `guid`=336843 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=336843;
DELETE FROM `creature` WHERE `guid`=336847 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=336847;
DELETE FROM `creature` WHERE `guid`=336849 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=336849;
DELETE FROM `creature` WHERE `guid`=336852 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=336852;
DELETE FROM `creature` WHERE `guid`=336844 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=336844;
DELETE FROM `creature` WHERE `guid`=336846 AND `id`=15010;
DELETE FROM `creature_addon` WHERE `guid`=336846;

*/
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN (52055, 52054);

DELETE FROM `creature` WHERE `map`=859 AND `id` IN (52062, 52055, 52054);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (52062, 52055, 52054);

UPDATE `creature_template` SET `ScriptName`='boss_zanzil' WHERE `entry`=52053;
UPDATE `creature_template` SET `ScriptName`='npc_zanzil_berserker' WHERE `entry`=52054;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (96342, 96914, 96331);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(96342, 'spell_zanzil_pursuit'),
(96914, 'spell_zanzil_fire'),
(96331, 'spell_frostburn_formula');

DELETE FROM `creature_text` WHERE `entry`=52053;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52053, 0, 0, 'What? You\'ve come to laugh at Zanzil, too? Not again! I\ll make you pay!', 14, 0, 0, 0, 0, 24337, 'Zanzil aggro'),
(52053, 1, 0, 'You\'ll all suffer for this! Zul\'Gurub is NOTHING without Zan... Zan...', 14, 0, 0, 0, 0, 24338, 'Zanzil death'),
(52053, 2, 0, 'Hahahaha!', 14, 0, 0, 0, 0, 24340, 'Zanzil gas 01'),
(52053, 2, 1, 'Breathe deep, friends! Breathe it all in!', 14, 0, 0, 0, 0, 24341, 'Zanzil gas 02'),
(52053, 3, 0, 'Don\'t worry, Zanzil won\'t leave you dead for long...', 14, 0, 0, 0, 0, 24342, 'Zanzil kill 01'),
(52053, 3, 1, 'How does it taste?', 14, 0, 0, 0, 0, 24343, 'Zanzil kill 02'),
(52053, 3, 2, 'Oooh, pretty... Zanzil\'s got big plans for this one.', 14, 0, 0, 0, 0, 24344, 'Zanzil kill 03'),
(52053, 4, 0, 'Maybe you need someone bigger to play with!', 14, 0, 0, 0, 0, 24349, 'Zanzil raise brute 01'),
(52053, 4, 1, 'Go, little one! Fight them! KILL THEM!', 14, 0, 0, 0, 0, 24350, 'Zanzil raise brute 02'),
(52053, 5, 0, 'Who needs friends? Zanzil just makes his own!', 14, 0, 0, 0, 0, 24351, 'Zanzil raise trolls 01'),
(52053, 5, 1, 'Rise up! Zanzil\'s elixir gives you life!', 14, 0, 0, 0, 0, 24352, 'Zanzil raise trolls 02');

DELETE FROM `locales_creature_text` WHERE `entry`=52053;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(52053, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Чего? И вы тоже пришли поиздеваться над Занзилом? Не позволю!'),
(52053, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы заплатите за это! Зул\'Гуруб без Занзила - нич... нич... ничто!'),
(52053, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ха-ха-ха!'),
(52053, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Дышите глубже, друзья! Вдыхайте все!'),
(52053, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Расслабься. Занзил вернет тебя с того света.'),
(52053, 3, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нравится?'),
(52053, 3, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'О-о-о-о! Чудесно! У Занзила на тебя большие планы!'),
(52053, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Хотите поиграть с большим мальчиком?'),
(52053, 4, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вперед, малыш! Дерись! УБЕЙ ИХ!'),
(52053, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Да кому это надо - заводить друзей! Занзил своих сам делает!'),
(52053, 5, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Восстаньте! Эликсир Занзила вернет вас к жизни!');