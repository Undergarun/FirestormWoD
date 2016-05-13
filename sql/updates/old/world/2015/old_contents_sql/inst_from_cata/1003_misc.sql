-- База
-- удаляем лишних нпц
DELETE FROM `creature` WHERE `map`=720 AND `id` IN (53752, 54112, 53695,  53694, 53713, 53819, 53724);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (53752, 54112, 53695,  53694, 53713, 53819, 53724);

-- ставим корректный лвл
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry` IN (53752, 53724, 53713);

-- перемещаем босса
UPDATE `creature` SET `spawnMask`=15, `position_x`=-149.515, `position_y`=192.755, `position_z`=46.170, `orientation`=3.73 WHERE `id`=53691 AND `map`=720;

-- исправляем модели босса и псов
UPDATE `creature_model_info` SET `bounding_radius`=3, `combat_reach`=3 WHERE `modelid`=38448;
UPDATE `creature_model_info` SET `bounding_radius`=3, `combat_reach`=3 WHERE `modelid`=38449;
UPDATE `creature_model_info` SET `bounding_radius`=3, `combat_reach`=3 WHERE `modelid`=38450;

-- добавляем иммуны для псов
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (53695, 53981, 54075, 54076, 53694, 53980, 54077, 54078);

-- исправляем дамаг босса и псов
UPDATE `creature_template` SET `mindmg`=532, `maxdmg`=798 WHERE `entry` IN (53695, 53694);
UPDATE `creature_template` SET `dmg_multiplier`=70 WHERE `entry` IN (53695, 53981, 53694, 53980);
UPDATE `creature_template` SET `dmg_multiplier`=80 WHERE `entry` IN (54075, 54076, 54077, 54078);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822 WHERE `entry`=53691;
UPDATE `creature_template` SET `dmg_multiplier`=97 WHERE `entry` IN (53691, 53979);
UPDATE `creature_template` SET `dmg_multiplier`=110 WHERE `entry` IN (54079, 54080);

-- отключаем лут у босса
UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN (53691, 53979, 54079, 54080);

-- отключаем специальные ачивки
REPLACE INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES 
(4, 17576, 0, '', '', 'Shannox - Backet List'),
(4, 17775, 0, '', '', 'Shannox - Backet List'),
(4, 17776, 0, '', '', 'Shannox - Backet List'),
(4, 17777, 0, '', '', 'Shannox - Backet List'),
(4, 17778, 0, '', '', 'Shannox - Backet List'),
(4, 17779, 0, '', '', 'Shannox - Backet List');

-- Скрипты
UPDATE `creature_template` SET `ScriptName`='boss_shannox' WHERE `entry`=53691;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_riplimb' WHERE `entry`=53694;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_rageface' WHERE `entry`=53695;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_spear_of_shannox' WHERE `entry`=53752;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_immolation_trap' WHERE `entry`=53724;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_crystal_prison_trap' WHERE `entry`=53713;
UPDATE `creature_template` SET `ScriptName`='npc_shannox_crystal_prison' WHERE `entry`=53819;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (101111, 99837, 99838, 101208, 101209, 101210);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101111, 'spell_shannox_riplimb_dogged_determination'),
(99837, 'spell_shannox_crystal_prison_trap'),
(99838, 'spell_shannox_immolation_trap'),
(101208, 'spell_shannox_immolation_trap'),
(101209, 'spell_shannox_immolation_trap'),
(101210, 'spell_shannox_immolation_trap');

-- Тексты
DELETE FROM `creature_text` WHERE `entry`=53691;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(53691, 0, 0, 'Aha! The interlopers... Kill them! EAT THEM!', 14, 0, 0, 0, 0, 24565, 'Shannox aggro'),
(53691, 1, 0, 'The pain... Lord of fire, it hurts...', 14, 0, 0, 0, 0, 24568, 'Shannox death'),
(53691, 2, 0, 'Fetch your supper', 14, 0, 0, 0, 0, 24569, 'Shannox dog attack 05'),
(53691, 3, 0, 'Sic \'em!', 14, 0, 0, 0, 0, 24570, 'Shannox dog attack 04'),
(53691, 3, 1, 'KILL!', 14, 0, 0, 0, 0, 24571, 'Shannox dog attack 03'),
(53691, 3, 2, 'Tear them down!', 14, 0, 0, 0, 0, 24572, 'Shannox dog attack 02'),
(53691, 3, 3, 'Go for the throat!', 14, 0, 0, 0, 0, 24573, 'Shannox dog attack 01'),
(53691, 4, 0, 'Riplimb! No... no! Oh, you terrible little beasts! HOW COULD YOU?!', 14, 0, 0, 0, 0, 24574, 'Shannox dogdeath 01'),
(53691, 5, 0, 'You murderers! Why... why would you kill such a noble animal?!', 14, 0, 0, 0, 0, 24575, 'Shannox dogdeath 02'),
(53691, 6, 0, 'Now you BURN!!', 14, 0, 0, 0, 0, 24576, 'Shannox fireattack 01'),
(53691, 6, 1, 'Twist in flames, interlopers!', 14, 0, 0, 0, 0, 24577, 'Shannox fireattack 02'),
(53691, 7, 0, 'Dog food!', 14, 0, 0, 0, 0, 24578, 'Shannox kill 04'),
(53691, 7, 1, 'Now you stay dead!', 14, 0, 0, 0, 0, 24579, 'Shannox kill 03'),
(53691, 7, 2, 'The Firelord will be most pleased!', 14, 0, 0, 0, 0, 24580, 'Shannox kill 02'),
(53691, 7, 3, 'Yes... oh yes!', 14, 0, 0, 0, 0, 24581, 'Shannox kill 01'),
(53691, 8, 0, 'Yes... I smell them too, Riplimb. Outsiders encroach on the Firelord\'s private grounds. Find their trail, find them for me, that I may dispense punishment!', 14, 0, 0, 0, 0, 24584, 'Shannox spawn'),
(53691, 9, 0, 'Get away from me, outsiders!', 14, 0, 0, 0, 0, 24585, 'Shannox spear attack 04'),
(53691, 9, 1, 'Back, filth!', 14, 0, 0, 0, 0, 24586, 'Shannox spear attack 03'),
(53691, 9, 2, 'Back! Stay back!', 14, 0, 0, 0, 0, 24587, 'Shannox spear attack 02'),
(53691, 9, 3, 'Step off!', 14, 0, 0, 0, 0, 24588, 'Shannox spear attack 01');

DELETE FROM `locales_creature_text` WHERE `entry`=53691;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(53691, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'А-ха-ха-ха! Незваные гости! Убить их. СОЖРАТЬ ИХ!'),
(53691, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ооо... Как больно. Повелитель Огня... больно...'),
(53691, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Лови свой ужин.'),
(53691, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Взять!'),
(53691, 3, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Убить!'),
(53691, 3, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Разорвать их!'),
(53691, 3, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Хватай за горло!'),
(53691, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Лютогрыз! Нет. НЕТ! Ах вы маленькие чудовища, как вы могли?'),
(53691, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ах вы убийцы! Зачем, зачем было убивать такое благородное животное!'),
(53691, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Сейчас вы сгорите!'),
(53691, 6, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Горите в огне, чужаки!'),
(53691, 7, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Свежие косточки!'),
(53691, 7, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вот так и лежи!'),
(53691, 7, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Повелитель огня будет очень доволен.'),
(53691, 7, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Да... о, да!'),
(53691, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Да... Я тоже чувствую, что они здесь, Лютогрыз. Чужаки во владениях повелителя огня. Бери след, найди их, и они понесут наказание!'),
(53691, 9, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Назад, ничтожества!'),
(53691, 9, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Не приближайтесь, чужаки!'),
(53691, 9, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Назад! Не подходить!'),
(53691, 9, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Прочь!');