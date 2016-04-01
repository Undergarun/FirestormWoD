DELETE FROM `creature` WHERE `id`=23965;

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN 
(41378, 49974, 49980, 49986,
 41442, 49583, 49584, 49585,
 42166, 49056, 49057, 49058, 
 42178, 49053, 49054, 49055,
 42179, 49047, 49048, 49049,
 42180, 49050, 49051, 49052,
 43296, 47774, 47775, 47776);
 
UPDATE `creature_template` SET `vehicleid`=863 WHERE `entry` IN (42166, 49056, 49057, 49058); 
UPDATE `creature_template` SET `vehicleid`=865 WHERE `entry` IN (42178, 49053, 49054, 49055);
UPDATE `creature_template` SET `vehicleid`=862 WHERE `entry` IN (42179, 49047, 49048, 49049);
UPDATE `creature_template` SET `vehicleid`=864 WHERE `entry` IN (42180, 49050, 49051, 49052);

UPDATE `creature` SET `position_x`=-328.81, `position_y`=-404.12, `position_z`=213.847, `orientation`=1.60 WHERE `id`=42166;

DELETE FROM `creature` WHERE `id`=42186;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(42186, 669, 15, 1, 0, 0, -324.77, -419.36, 229.11, 1.58084, 604800, 0, 0, 42000, 0, 0, 0, 0, 0);
UPDATE `creature_template` SET `InhabitType`=4, `flags_extra`=129 WHERE `entry`=42186;

DELETE FROM `locales_creature_text` WHERE `entry`=42186;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(42186, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Обнаружено несанкционированное вторжение. Первичная защитная матрица активирована.'),
(42186, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Защитная система уничтожена. Отключение питания.'),
(42186, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Модуль Электрон включен.'),
(42186, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Модуль Магматрон включен.'),
(42186, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Модуль Токситрон включен.'),
(42186, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Модуль Чаротрон включен.'),
(42186, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Защитная система модуля Электрон включена.'),
(42186, 7, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Защитная система модуля Магматрон включена.'),
(42186, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Защитная система модуля Токситрон включена.'),
(42186, 9, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Защитная система модуля Чаротрон включена.'),
(42186, 10, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Переправляю избыток энергии модулю Магматрон. Включаю огнемет.');

DELETE FROM `locales_creature_text` WHERE `entry`=41378;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(41378, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нельзя прерывать процесс! Нельзя заставлять господина ждать! Больше нельзя ошибаться!'),
(41378, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Такого, как я, больше никогда не будет...'),
(41378, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ничего не выбрасывайте...'),
(41378, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Отделите мясо, соберите органы!'),
(41378, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Помешивая, довести до кипения...'),
(41378, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Никак не удается стабилизировать, но без ошибок нет прогресса!'),
(41378, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Как смертная оболочка переносит резкие скачки температуры? Надо выяснить! ВО ИМЯ НАУКИ!'),
(41378, 7, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Недостаток ума они с лихвой компенсируют свирепостью!'),
(41378, 7, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мои неудачные творения станут вашей погибелью!'),
(41378, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Рановато, но выбора нет... придется опробовать их! Смотрите, это мои лучшие создания!'),
(41378, 9, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Физическое воплощение моего разума! Дети мои, поглотите их!');

DELETE FROM `locales_creature_text` WHERE `entry`=41442;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(41442, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мне не нужны глаза, чтобы видеть врагов. Вас выдает жуткая вонь и топот ног!'),
(41442, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'У тебя есть глаза, но истину ты не видишь. Это твой конец, смирись с этим!'),
(41442, 1, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Смерть ждет во тьме!'),
(41442, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Наконец-то твое жалкое существование закончится.'),
(41442, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'От пламени не спрячешься!'),
(41442, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Да, беги! С каждым шагом твое сердце бьется все быстрее. Эти громкие, оглушительные удары... Тебе некуда бежать!');

DELETE FROM `locales_creature_text` WHERE `entry`=49226;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(49226, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я чуть не разорился на постоянном ремонте этой развалюхи.');