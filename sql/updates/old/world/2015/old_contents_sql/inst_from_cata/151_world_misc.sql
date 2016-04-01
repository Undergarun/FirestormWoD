/*DELETE FROM `creature` WHERE `id`=52368;
DELETE FROM `creature` WHERE `id`=52389;
DELETE FROM `creature` WHERE `id`=52369;
DELETE FROM `creature` WHERE `guid`=345427 AND `id`=52428;
DELETE FROM `creature_addon` WHERE `guid`=345427;
DELETE FROM `creature` WHERE `guid`=345426 AND `id`=52428;
DELETE FROM `creature_addon` WHERE `guid`=345426;
DELETE FROM `creature` WHERE `guid`=345423 AND `id`=52428;
DELETE FROM `creature_addon` WHERE `guid`=345423;
DELETE FROM `creature` WHERE `guid`=345420 AND `id`=52428;
DELETE FROM `creature_addon` WHERE `guid`=345420;
DELETE FROM `creature` WHERE `guid`=345422 AND `id`=52428;
DELETE FROM `creature_addon` WHERE `guid`=345422;
DELETE FROM `creature` WHERE `guid`=345425 AND `id`=52428;
DELETE FROM `creature_addon` WHERE `guid`=345425;
DELETE FROM `creature` WHERE `guid`=345421 AND `id`=52428;
DELETE FROM `creature_addon` WHERE `guid`=345421;
*/

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=207619;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=208953;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=209849;

UPDATE `creature_template` SET minlevel=87, maxlevel=87, flags_extra=128 WHERE `entry`=52369;
UPDATE `creature_template` SET minlevel=87, maxlevel=87, flags_extra=128 WHERE `entry`=52368;

UPDATE `instance_template` SET `Script`='instance_baradin_hold' WHERE `map`=757;

UPDATE `creature_template` SET `ScriptName`='boss_argaloth' WHERE `entry`=47120;

UPDATE `creature_template` SET `ScriptName`='boss_occuthar' WHERE `entry`=52363;
UPDATE `creature_template` SET `ScriptName`='npc_occuthar_eyestalk' WHERE `entry`=52369;
UPDATE `creature_template` SET `ScriptName`='npc_occuthar_eye_of_occuthar' WHERE `entry`=52389;

DELETE FROM `spell_script_names` WHERE `spell_id`=96931;
INSERT INTO `spell_script_names` VALUES (96931, 'spell_occuthar_eye_of_occuthar');

UPDATE `creature_template` SET `ScriptName`='boss_alizabal' WHERE `entry`=55869;

DELETE FROM `spell_script_names` WHERE `spell_id`=105065;
INSERT INTO `spell_script_names` VALUES (105065,'spell_alizabal_seething_hate');
DELETE FROM `spell_script_names` WHERE `spell_id`=106248;
INSERT INTO `spell_script_names` VALUES (106248,'spell_alizabal_blade_dance');

DELETE FROM `creature_text` WHERE `entry`=55869;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(55869, 0, 0, 'I hate adventurers.', 14, 0, 0, 0, 0, 25777, 'VO_BH_ALIZABAL_AGGRO_01'),
(55869, 1, 0, 'I hate... every one of you...', 14, 0, 0, 0, 0, 25778, 'VO_BH_ALIZABAL_DEATH_01'),
(55869, 2, 0, 'How I HATE this place. My captors may be long-dead, but don\'t think I won\'t take it all out on you miserable treasure-hunters.', 14, 0, 0, 0, 0, 25779, 'VO_BH_ALIZABAL_INTRO_01'),
(55869, 3, 0, 'I hate incompetent raiders.', 14, 0, 0, 0, 0, 25780, 'VO_BH_ALIZABAL_RESET_01'),
(55869, 4, 0, 'I still hate you.', 14, 0, 0, 0, 0, 25781, 'VO_BH_ALIZABAL_SLAY_01'),
(55869, 4, 1, 'Do you hate me? Good.', 14, 0, 0, 0, 0, 25782, 'VO_BH_ALIZABAL_SLAY_02'),
(55869, 4, 2, 'I hate mercy.', 14, 0, 0, 0, 0, 25782, 'VO_BH_ALIZABAL_SLAY_03'),
(55869, 4, 3, 'I didn\'t hate that.', 14, 0, 0, 0, 0, 25784, 'VO_BH_ALIZABAL_SLAY_04'),
(55869, 5, 0, 'I hate armor.', 14, 0, 0, 0, 0, 25785, 'VO_BH_ALIZABAL_SPELL_01'),
(55869, 5, 1, 'I hate martyrs.', 14, 0, 0, 0, 0, 25786, 'VO_BH_ALIZABAL_SPELL_02'),
(55869, 5, 2, 'Feel my hatred!', 14, 0, 0, 0, 0, 25787, 'VO_BH_ALIZABAL_SPELL_03'),
(55869, 5, 3, 'My hatred burns!', 14, 0, 0, 0, 0, 25788, 'VO_BH_ALIZABAL_SPELL_04'),
(55869, 5, 4, 'My hate will consume you!', 14, 0, 0, 0, 0, 25789, 'VO_BH_ALIZABAL_SPELL_05'),
(55869, 6, 0, 'I hate you all!', 14, 0, 0, 0, 0, 25790, 'VO_BH_ALIZABAL_SPELL_06'),
(55869, 6, 1, 'I hate standing still.', 14, 0, 0, 0, 0, 25791, 'VO_BH_ALIZABAL_SPELL_07');

DELETE FROM `locales_creature_text` WHERE `entry`=55869;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(55869, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ненавижу искателей приключений.'),
(55869, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ненавижу... каждого из вас...'),
(55869, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Как же я НЕНАВИЖУ это место. Те, кто меня пленил, наверняка давно мертвы, но это не помешает мне выместить зло на вас, жалкие искатели сокровищ.'),
(55869, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ненавижу неумелых рейдеров.'),
(55869, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я ненавижу мертвых.'),
(55869, 4, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ненавидишь меня? Отлично.'),
(55869, 4, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я ненавижу сострадание.'),
(55869, 4, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'А вот это я не ненавижу.'),
(55869, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я ненавижу броню.'),
(55869, 5, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я ненавижу мучеников.'),
(55869, 5, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Прочувствуй мою ненависть!'),
(55869, 5, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Моя ненависть неугасима!'),
(55869, 5, 4, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Моя ненависть поглотит тебя!'),
(55869, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я ненавижу вас всех!'),
(55869, 6, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я ненавижу стоять на месте.');