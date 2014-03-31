-- DELETE FROM `creature` WHERE `id`=52059 AND `guid`=335593;
DELETE FROM `creature` WHERE `id` IN (52061, 52147, 52160, 52387, 53108);

UPDATE `creature_template` SET `faction_a`=14 WHERE `faction_a` IN (16, 7) AND `entry` IN 
(15010, 45979, 46464, 52053,
52059, 52076, 52077, 52079,
52081, 52082, 52085, 52086,
52087, 52088, 52089, 52148,
52151, 52155, 52167, 52222,
52271, 52305, 52306, 52311,
52322, 52323, 52325, 52327,
52331, 52332, 52339, 52340,
52345, 52347, 52348, 52351,
52352, 52353, 52354, 52362,
52364, 52373, 52375, 52376,
52377, 52379, 52380, 52381,
52392, 52402, 52405, 52413,
52414, 52417, 52418, 52419,
52422, 52429, 52430, 52434,
52435, 52436, 52437, 52438,
52439, 52440, 52441, 52442,
52446, 52449, 52450, 52451,
52452, 52453, 52454, 52455,
52529, 52532, 52598, 52606,
52608, 52624, 52820, 52956,
52958, 52962, 53022, 53023,
53024, 53025, 53040, 53043,
53044, 53088, 53151, 53488);

UPDATE `creature_template` SET `faction_h`=14 WHERE `faction_h` IN (16, 7) AND `entry` IN 
(15010, 45979, 46464, 52053,
52059, 52076, 52077, 52079,
52081, 52082, 52085, 52086,
52087, 52088, 52089, 52148,
52151, 52155, 52167, 52222,
52271, 52305, 52306, 52311,
52322, 52323, 52325, 52327,
52331, 52332, 52339, 52340,
52345, 52347, 52348, 52351,
52352, 52353, 52354, 52362,
52364, 52373, 52375, 52376,
52377, 52379, 52380, 52381,
52392, 52402, 52405, 52413,
52414, 52417, 52418, 52419,
52422, 52429, 52430, 52434,
52435, 52436, 52437, 52438,
52439, 52440, 52441, 52442,
52446, 52449, 52450, 52451,
52452, 52453, 52454, 52455,
52529, 52532, 52598, 52606,
52608, 52624, 52820, 52956,
52958, 52962, 53022, 53023,
53024, 53025, 53040, 53043,
53044, 53088, 53151, 53488);

UPDATE `creature` SET `spawnmask`=2 WHERE `id`=52059;
-- UPDATE `creature_addon` SET `auras`='8279' WHERE `guid`=338987;
UPDATE `creature_template_addon` SET `auras`='8279' WHERE `entry`=52059;

UPDATE `creature_template` SET `inhabittype`=5, `unit_flags`=0 WHERE `entry`=52160;
UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry` IN (52160, 52387);
UPDATE `creature_template` SET `flags_extra`=0 WHERE `entry`=52147;
UPDATE `creature_template` SET `unit_flags`=33554432 WHERE `entry`=52147;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry` IN (52147, 52160, 52387);
UPDATE `creature_template` SET `dmg_multiplier`=70 WHERE `entry` IN (52059, 52393);
UPDATE `creature_template` SET `unit_flags`=32768 WHERE `entry`=52061;

UPDATE `gameobject_template` SET `flags`=`flags`|4 WHERE `entry`=180497;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=53088;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(53088, 98178, 3, 0);

UPDATE `creature_template` SET `ScriptName`='boss_kilnara' WHERE `entry`=52059;
UPDATE `creature_template` SET `ScriptName`='npc_kilnara_pride_of_bethekk' WHERE `entry`=52061;
UPDATE `creature_template` SET `ScriptName`='npc_kilnara_wave_of_agony' WHERE `entry` IN (52147, 52160, 52387);
UPDATE `creature_template` SET `ScriptName`='npc_kilnara_temple_rat' WHERE `entry` IN (53088, 53108);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (98269, 96457, 98238);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(98269, 'spell_kilnara_wave_of_agony_target'),
(96457, 'spell_kilnara_wave_of_agony_start'),
(98238, 'spell_kilnara_rat_lure');

DELETE FROM `creature_text` WHERE `entry`=52059;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52059, 0, 0, 'No. NO! Get out! Leave me here with the memory of my sister!', 14, 0, 0, 0, 0, 24269, 'Kilnara aggro'),
(52059, 1, 0, 'This body is useless!', 14, 0, 0, 0, 0, 24273, 'Kilnara death feral'),
(52059, 2, 0, 'Ha! You are nothing but a plaything!', 14, 0, 0, 0, 0, 24275, 'Kilnara kill feral 01'),
(52059, 2, 1, 'Share in my pain!', 14, 0, 0, 0, 0, 24276, 'Kilnara kill feral 02'),
(52059, 3, 0, 'I told you to leave!', 14, 0, 0, 0, 0, 24277, 'Kilnara kill 01'),
(52059, 3, 1, 'I have sworn to protect this place.', 14, 0, 0, 0, 0, 24278, 'Kilnara kill 02'),
(52059, 4, 0, 'Feel my agony!', 14, 0, 0, 0, 0, 24284, 'Kilnara spell 01'),
(52059, 5, 0, 'What have you done? I can\'t... control... RRAAAARRRGHHH!', 14, 0, 0, 0, 0, 24286, 'Kilnara transform 01'),
(52059, 6, 0, 'Haaa ha ha haa! Now, heathens... face the TRUE might of Bethekk!', 14, 0, 0, 0, 0, 24288, 'Kilnara transform 02 feral');

DELETE FROM `locales_creature_text` WHERE `entry`=52059;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(52059, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нет. НЕТ! Прочь! Дайте мне оплакать сестру!'),
(52059, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Это тело бесполезно...'),
(52059, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ха! С тобой я наигралась.'),
(52059, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Раздели мою боль!'),
(52059, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я же просила вас уйти.'),
(52059, 3, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я поклялась защищать это место!'),
(52059, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Почувствуйте мое страдание!'),
(52059, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Что вы наделали? Я больше... не могу...'),
(52059, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ха-ха-ха-хааа! Узрите истинную мощь Бетекк!');