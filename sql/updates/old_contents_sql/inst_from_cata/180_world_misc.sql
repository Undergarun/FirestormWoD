/*DELETE FROM `creature` WHERE `guid`=345529 AND `id`=43552;
DELETE FROM `creature_addon` WHERE `guid`=345529;
DELETE FROM `creature` WHERE `guid`=329688 AND `id`=43537;
DELETE FROM `creature_addon` WHERE `guid`=329688;
DELETE FROM `creature` WHERE `guid`=329684 AND `id`=43430;
DELETE FROM `creature_addon` WHERE `guid`=329684;
DELETE FROM `creature` WHERE `guid`=329685 AND `id`=42696;
DELETE FROM `creature_addon` WHERE `guid`=329685;
DELETE FROM `creature` WHERE `guid`=329683 AND `id`=43430;
DELETE FROM `creature_addon` WHERE `guid`=329683;
DELETE FROM `creature` WHERE `guid`=329681 AND `id`=43430;
DELETE FROM `creature_addon` WHERE `guid`=329681;
DELETE FROM `creature` WHERE `guid`=329687 AND `id`=43537;
DELETE FROM `creature_addon` WHERE `guid`=329687;
DELETE FROM `creature` WHERE `guid`=329690 AND `id`=42696;
DELETE FROM `creature_addon` WHERE `guid`=329690;
DELETE FROM `creature` WHERE `guid`=329691 AND `id`=43537;
DELETE FROM `creature_addon` WHERE `guid`=329691;
DELETE FROM `creature` WHERE `guid`=329689 AND `id`=43537;
DELETE FROM `creature_addon` WHERE `guid`=329689;
DELETE FROM `creature` WHERE `guid`=329682 AND `id`=43430;
DELETE FROM `creature_addon` WHERE `guid`=329682;
DELETE FROM `creature` WHERE `guid`=329686 AND `id`=42696;
DELETE FROM `creature_addon` WHERE `guid`=329686;
DELETE FROM `creature` WHERE `guid`=329692 AND `id`=42696;
DELETE FROM `creature_addon` WHERE `guid`=329692;
*/

DELETE FROM `creature` WHERE `id`=43917 AND `map`=725;
DELETE FROM `creature` WHERE `id`=49267 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43743 AND `map`=725;
DELETE FROM `creature` WHERE `id`=42428 AND `map`=725;
DELETE FROM `creature` WHERE `id`=42499 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43242 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43286 AND `map`=725;
DELETE FROM `creature` WHERE `id`=49473 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43159 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43357 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43020 AND `map`=725;
DELETE FROM `creature` WHERE `id`=43014 AND `map`=725;

UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43917;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=49267;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43743;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42428;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42499;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43242;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43286;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=49473;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43159;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=43357;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=42333;
DELETE FROM `creature_addon` where `guid`=329512;
DELETE FROM `creature_addon` where `guid`=329514;

UPDATE `creature_template` SET `unit_flags`=0, `type_flags`=72, `dynamicflags`=0 WHERE `entry`=43438;
UPDATE `creature_template` SET `unit_flags`=0, `type_flags`=72, `dynamicflags`=0 WHERE `entry`=49642;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=42810;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=49647;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0, `InhabitType`=1 WHERE `entry`=43214;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0, `InhabitType`=1 WHERE `entry`=49538;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=49624;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=42333;
UPDATE `creature` SET `position_x`=1285.66, `position_y`=1216.81, `position_z`=246.932, `orientation`=3.58754 WHERE `guid`=329512;

UPDATE `instance_template` SET `script`='instance_the_stonecore' WHERE `map`=725;

UPDATE `creature_template` SET `ScriptName`='boss_corborus' WHERE `entry`=43438;
UPDATE `creature_template` SET `ScriptName`='npc_rock_borer' WHERE `entry`=43917;
UPDATE `creature_template` SET `ScriptName`='npc_crystal_shard' WHERE `entry`=49267;

UPDATE `creature_template` SET `ScriptName`='boss_slabhide' WHERE `entry`=43214;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128, `ScriptName`='npc_lava_fissure' WHERE `entry`=43242;

UPDATE `creature_template` SET `ScriptName`='boss_ozruk' WHERE `entry`=42188;
DELETE FROM `creature_text` WHERE `entry`=42188;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42188, 0, 0, 'None may pass into the World\'s Heart!', 1, 0, 0, 0, 0, 21919, 'VO_SC_OZRUK aggro'),
(42188, 1, 0, 'The cycle is complete.', 1, 0, 0, 0, 0, 21920, 'VO_SC_OZRUK kill'),
(42188, 2, 0, 'Break yourselves upon my body. Feel the strength of the earth!', 1, 0, 0, 0, 0, 21921, 'VO_SC_OZRUK'),
(42188, 3, 0, 'A protector has fallen. The World\'s Heart lies exposed!', 1, 0, 0, 0, 0, 21922, 'VO_SC_OZRUK death');

UPDATE `creature_template` SET `ScriptName`='boss_high_priestess_azil' WHERE `entry`=42333;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128, `ScriptName`='npc_gravity_well' WHERE `entry`=42499;

DELETE FROM `spell_script_names` WHERE `spell_id`=79251;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (79251, 'high_priestess_azil_gravity_well_script');

DELETE FROM `creature_text` WHERE `entry`=42333;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(42333, 0, 0, '', 1, 0, 0, 0, 0, 21627, 'VO_SC_Azil_Event01'),
(42333, 1, 0, 'Witness the power bestowed upon me by Deathwing! Feel the fury of earth!', 1, 0, 0, 0, 0, 21628, 'VO_SC_Azil spell01'),
(42333, 2, 0, 'For my death, countless more will fall. The burden is now yours to bear.', 1, 0, 0, 0, 0, 21633, 'VO_SC_Azil death'),
(42333, 3, 0, 'The world will be reborn in flames!', 1, 0, 0, 0, 0, 21634, 'VO_SC_Azil aggro1'),
(42333, 3, 1, 'A sacrifice for you, master.', 1, 0, 0, 0, 0, 21635, 'VO_SC_Azil aggro2');

DELETE FROM `creature_text` WHERE `entry`=43391;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES 
(43391, 0, 0, 'Follow me if you dare!', 1, 0, 0, 0, 0, 21783, 'vo_sc_millhouse_event01'),
(43391, 0, 1, 'Prison taught me one very important lesson, well, two if you count how to hold your soap, but yes! SURVIVAL!', 1, 0, 0, 0, 0, 21789, 'vo_sc_millhouse_event07'),
(43391, 1, 0, 'It\'s time for a tactical retreat!', 1, 0, 0, 0, 0, 21784, 'vo_sc_millhouse_event02'),
(43391, 1, 1, 'You\'re gonna be sorry!', 1, 0, 0, 0, 0, 21785, 'vo_sc_millhouse_event03'),
(43391, 2, 0, 'Don\'t say I didn\'t warn ya!', 1, 0, 0, 0, 0, 21786, 'vo_sc_millhouse_event04'),
(43391, 2, 1, 'Ah-ha! I\'ve got you right where I want you!', 1, 0, 0, 0, 0, 21787, 'vo_sc_millhouse_event05'),
(43391, 3, 0, 'Now... witness the full power of Millhouse Manastorm!', 1, 0, 0, 0, 0, 21788, 'vo_sc_millhouse_event06'),
(43391, 4, 0, '', 1, 0, 0, 0, 0, 21794, 'vo_sc_millhouse_death01');

DELETE FROM `locales_creature_text` WHERE `entry`=42188;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(42188, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Никто не пройдет к сердцу мира!'),
(42188, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Цикл завершен.'),
(42188, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Разбейтесь о меня. Почувствуйте силу земли!'),
(42188, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Хранитель повержен... Сердце мира беззащитно!');

DELETE FROM `locales_creature_text` WHERE `entry`=42333;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(42333, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Узрите мощь, дарованную мне Смертокрылом! Почувствуйте ярость земли!'),
(42333, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Моя гибель откроет бесконечную череду смертей. Мое бремя ложится на вас.'),
(42333, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мир переродится в пламени!'),
(42333, 3, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Жертва для тебя, господин.');

DELETE FROM `locales_creature_text` WHERE `entry`=43391;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43391, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Следуйте за мной, если осмелитесь!'),
(43391, 0, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'В тюрьме я усвоил один очень важный урок. Ну, или два... Первый: держи мыло крепко! Второй: выживай любой ценой!'),
(43391, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Пора начать тактическое отступление!'),
(43391, 1, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы об этом пожалеете!'),
(43391, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Не говорите, что я не предупреждал!'),
(43391, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ахаха! Вы как раз там, где мне и надо!'),
(43391, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'А теперь... Узрите всю мощь Миллхауса Манашторма!');