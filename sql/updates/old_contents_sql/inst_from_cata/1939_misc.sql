DELETE FROM `creature` WHERE `id`=54969;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(54969, 939, 3, 65535, 0, 54969, 3347.73, -5736.66, 14.9964, 2.56975, 300, 0, 0, 99592800, 0, 0, 0, 0, 0);

UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `item`=77952;
UPDATE `gameobject_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `item`=77952;
UPDATE `creature_loot_template` SET `mincountOrRef`=1, `maxcount`=1 WHERE `item`=77952 AND `entry` IN
(55265, 57771, 55308, 55310, 55312, 55314, 55689, 57955, 56427, 57847);
UPDATE `creature_loot_template` SET `mincountOrRef`=1, `maxcount`=3 WHERE `item`=77952 AND `entry` IN
(55265, 57771, 55308, 55310, 55312, 55314, 55689, 57955, 56427, 57847);

UPDATE `creature_template` SET AIName='', `ScriptName`='npc_dragon_soul_crimson_globule' WHERE `entry`=57386;
UPDATE `creature_template` SET AIName='', `ScriptName`='npc_dragon_soul_acidic_globule' WHERE `entry`=57333;
UPDATE `creature_template` SET AIName='', `ScriptName`='npc_dragon_soul_dark_globule' WHERE `entry`=57382;
UPDATE `creature_template` SET AIName='', `ScriptName`='npc_dragon_soul_shadowed_globule' WHERE `entry`=57388;
UPDATE `creature_template` SET AIName='', `ScriptName`='npc_dragon_soul_cobalt_globule' WHERE `entry`=57384;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (109389, 108222);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(109389, 'spell_dragon_soul_shadowed_globule_deep_corruption'),
(108222, 'spell_dragon_soul_cobalt_globule_mana_void');

DELETE FROM `conditions` WHERE `SourceEntry`=108224;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 108224, 0, 31, 0, 3, 57384, 'Cobalt Crobule - Mana Void');

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN 
(57386, 57428, 57387, 57429, 57333, 57425, 57382, 57426, 57388, 57430, 57384, 57427);

DELETE FROM `creature` WHERE `map`=967 AND `id` IN (55865, 55862, 55864, 55867, 55863, 55866);
-- DELETE FROM `creature` WHERE `guid`=343659 AND `id`=55544 AND `map`=967;

UPDATE `creature` SET `spawnMask`=15 WHERE `id`=55312 AND `map`=967;

UPDATE `creature_template_addon` SET `auras`='108203' WHERE `entry` IN 
(57328, 57377, 57378, 57379,  57443, 57882);
-- UPDATE `creature_addon` SET `auras`='108203' WHERE `guid` IN 
-- (343210, 343212, 344192, 344316, 343192, 345724, 343211, 344315, 344317);
UPDATE `creature_template` SET `modelid1`=24719, `modelid2`=0, `npcflag`=1, `unit_flags`=0, `InhabitType`=5 WHERE `entry` IN (57328, 57377, 57378, 57379,  57443, 57882);
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry`=57288;
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (57328, 57377, 57378, 57379,  57443, 57882);


UPDATE `creature_template` SET `modelid2`=0, `inhabittype`=5 WHERE `entry`=55544;

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (55312, 55313, 55314, 55315);
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN 
(55865, 57368, 57369, 57370,
55862, 57359, 57360, 57361,
55864, 57371, 57372, 57373,
55867, 57362, 57363, 57364,
55863, 57374, 57375, 57376,
55866, 57365, 57366, 57367);
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (56265, 57434, 57435, 57436);
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (56231, 57437, 57438, 57439);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=97 WHERE `entry` IN (55312, 55313);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=110 WHERE `entry` IN (55314, 55315);

UPDATE `creature_template` SET `ScriptName`='boss_yorsahj_the_unsleeping' WHERE `entry`=55312;
UPDATE `creature_template` SET `ScriptName`='npc_yorsahj_the_unsleeping_globule' WHERE `entry` IN (55865, 55862, 55864, 55867, 55863, 55866);
UPDATE `creature_template` SET `ScriptName`='npc_yorsahj_the_unsleeping_forgotten_one' WHERE `entry`=56265;
UPDATE `creature_template` SET `ScriptName`='npc_yorsahj_the_unsleeping_mana_void' WHERE `entry`=56231;
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_teleport' WHERE `entry` IN (57288, 57328);

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(109894, 109895, 109896, 109897, 109898, 103628, 105571, 105530);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(109894, 'spell_yorsahj_the_unsleeping_whisper'),
(109895, 'spell_yorsahj_the_unsleeping_whisper'),
(109896, 'spell_yorsahj_the_unsleeping_whisper'),
(109897, 'spell_yorsahj_the_unsleeping_whisper'),
(109898, 'spell_yorsahj_the_unsleeping_whisper'),
(103628, 'spell_yorsahj_the_unsleeping_deep_corruption'),
(105571, 'spell_yorsahj_the_unsleeping_digestive_acid_aoe'),
(105530, 'spell_yorsahj_the_unsleeping_mana_void');

DELETE FROM `conditions` WHERE `SourceEntry` IN 
(105420, 105435, 105436, 105437, 105439, 105440, 105031, 105904, 103635, 105536);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 105420, 0, 31, 0, 3, 55863, 'Yor\'sahj The Unsleeping - Color Combination 1'),
(13, 1, 105420, 1, 31, 0, 3, 55862, 'Yor\'sahj The Unsleeping - Color Combination 1'),
(13, 1, 105420, 2, 31, 0, 3, 55866, 'Yor\'sahj The Unsleeping - Color Combination 1'),
(13, 1, 105420, 3, 31, 0, 3, 55867, 'Yor\'sahj The Unsleeping - Color Combination 1'),
(13, 1, 105435, 0, 31, 0, 3, 55862, 'Yor\'sahj The Unsleeping - Color Combination 2'),
(13, 1, 105435, 1, 31, 0, 3, 55865, 'Yor\'sahj The Unsleeping - Color Combination 2'),
(13, 1, 105435, 2, 31, 0, 3, 55867, 'Yor\'sahj The Unsleeping - Color Combination 2'),
(13, 1, 105435, 3, 31, 0, 3, 55866, 'Yor\'sahj The Unsleeping - Color Combination 2'),
(13, 1, 105436, 0, 31, 0, 3, 55862, 'Yor\'sahj The Unsleeping - Color Combination 3'),
(13, 1, 105436, 1, 31, 0, 3, 55864, 'Yor\'sahj The Unsleeping - Color Combination 3'),
(13, 1, 105436, 2, 31, 0, 3, 55865, 'Yor\'sahj The Unsleeping - Color Combination 3'),
(13, 1, 105436, 3, 31, 0, 3, 55867, 'Yor\'sahj The Unsleeping - Color Combination 3'),
(13, 1, 105437, 0, 31, 0, 3, 55866, 'Yor\'sahj The Unsleeping - Color Combination 4'),
(13, 1, 105437, 1, 31, 0, 3, 55863, 'Yor\'sahj The Unsleeping - Color Combination 4'),
(13, 1, 105437, 2, 31, 0, 3, 55864, 'Yor\'sahj The Unsleeping - Color Combination 4'),
(13, 1, 105437, 3, 31, 0, 3, 55862, 'Yor\'sahj The Unsleeping - Color Combination 4'),
(13, 1, 105439, 0, 31, 0, 3, 55866, 'Yor\'sahj The Unsleeping - Color Combination 5'),
(13, 1, 105439, 1, 31, 0, 3, 55867, 'Yor\'sahj The Unsleeping - Color Combination 5'),
(13, 1, 105439, 2, 31, 0, 3, 55864, 'Yor\'sahj The Unsleeping - Color Combination 5'),
(13, 1, 105439, 3, 31, 0, 3, 55863, 'Yor\'sahj The Unsleeping - Color Combination 5'),
(13, 1, 105440, 0, 31, 0, 3, 55863, 'Yor\'sahj The Unsleeping - Color Combination 6'),
(13, 1, 105440, 1, 31, 0, 3, 55865, 'Yor\'sahj The Unsleeping - Color Combination 6'),
(13, 1, 105440, 2, 31, 0, 3, 55867, 'Yor\'sahj The Unsleeping - Color Combination 6'),
(13, 1, 105440, 3, 31, 0, 3, 55864, 'Yor\'sahj The Unsleeping - Color Combination 6'),
(13, 1, 105031, 0, 31, 0, 3, 55544, 'Yor\'sahj The Unsleeping - Digestive Acid'),
(13, 1, 103635, 0, 31, 0, 3, 55865, 'Yor\'sahj The Unsleeping - Fusing Vapors heal'),
(13, 1, 103635, 1, 31, 0, 3, 55862, 'Yor\'sahj The Unsleeping - Fusing Vapors heal'),
(13, 1, 103635, 2, 31, 0, 3, 55864, 'Yor\'sahj The Unsleeping - Fusing Vapors heal'),
(13, 1, 103635, 3, 31, 0, 3, 55867, 'Yor\'sahj The Unsleeping - Fusing Vapors heal'),
(13, 1, 103635, 4, 31, 0, 3, 55863, 'Yor\'sahj The Unsleeping - Fusing Vapors heal'),
(13, 1, 103635, 5, 31, 0, 3, 55866, 'Yor\'sahj The Unsleeping - Fusing Vapors heal'),
(13, 1, 105904, 0, 31, 0, 3, 55865, 'Yor\'sahj The Unsleeping - Fusing Vapors immune'),
(13, 1, 105904, 1, 31, 0, 3, 55862, 'Yor\'sahj The Unsleeping - Fusing Vapors immune'),
(13, 1, 105904, 2, 31, 0, 3, 55864, 'Yor\'sahj The Unsleeping - Fusing Vapors immune'),
(13, 1, 105904, 3, 31, 0, 3, 55867, 'Yor\'sahj The Unsleeping - Fusing Vapors immune'),
(13, 1, 105904, 4, 31, 0, 3, 55863, 'Yor\'sahj The Unsleeping - Fusing Vapors immune'),
(13, 1, 105904, 5, 31, 0, 3, 55866, 'Yor\'sahj The Unsleeping - Fusing Vapors immune'),
(13, 1, 105536, 0, 31, 0, 3, 55312, 'Yor\'sahj The Unsleeping - Mana Void dummy');

DELETE FROM `creature_text` WHERE `entry`=55312;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55312, 0, 0, 'Iilth qi\'uothk shn\'ma yeh\'glu Shath\'Yar! H\'IWN IILTH!', 14, 0, 100, 0, 0, 26326, 'yor\'sahj aggro'),
(55312, 1, 0, 'Ez, Shuul\'wah! Sk\'woth\'gl yu\'gaz yog\'ghyl ilfah!', 14, 0, 100, 0, 0, 26327, 'yor\'sahj death'),
(55312, 2, 0, 'Ak\'agthshi ma uhnish, ak\'uq shg\'cul vwahuhn! H\'iwn iggksh Phquathi gag OOU KAAXTH SHUUL!', 14, 0, 100, 0, 0, 26328, 'yor\'sahj intro'),
(55312, 3, 0, 'Sk\'yahf qi\'plahf PH\'MAGG!', 14, 0, 100, 0, 0, 26329, 'yor\'sahj slay 1'),
(55312, 3, 1, 'H\'iwn zaix Shuul\'wah, PHQUATHI!', 14, 0, 100, 0, 0, 26330, 'yor\'sahj slay 2'),
(55312, 3, 2, 'Shkul an\'zig qvsakf KSSH\'GA, ag\'THYZAK agthu!', 14, 0, 100, 0, 0, 26331, 'yor\'sahj slay 3'),
(55312, 4, 0, 'KYTH ag\'xig yyg\'far IIQAATH ONGG!', 14, 0, 100, 0, 0, 26332, 'yor\'sahj spell 1'),
(55312, 4, 1, 'UULL lwhuk H\'IWN!', 14, 0, 100, 0, 0, 26333, 'yor\'sahj spell 2'),
(55312, 5, 0, 'You will drown in the blood of the Old Gods! ALL OF YOU!', 42, 0, 100, 0, 0, 0, 'yor\'sahj aggro whisper'),
(55312, 6, 0, 'O, Deathwing! Your faithful servant has failed you!', 42, 0, 100, 0, 0, 0, 'yor\'sahj death whisper'),
(55312, 7, 0, 'Our numbers are endless, our power beyond reckoning! All who oppose the Destroyer will DIE A THOUSAND DEATHS!', 42, 0, 100, 0, 0, 0, 'yor\'sahj intro whisper'),
(55312, 8, 0, 'Your soul will know ENDLESS TORMENT!', 42, 0, 100, 0, 0, 0, 'yor\'sahj slay 1 whisper'),
(55312, 8, 1, 'All praise Deathwing, THE DESTROYER!', 42, 0, 100, 0, 0, 0, 'yor\'sahj slay 2 whisper'),
(55312, 8, 2, 'From its BLEAKEST DEPTHS, we RECLAIM this world!', 42, 0, 100, 0, 0, 0, 'yor\'sahj slay 3 whisper'),
(55312, 9, 0, 'SEE how we pour from the CURSED EARTH!', 42, 0, 100, 0, 0, 0, 'yor\'sahj spell 1 whisper'),
(55312, 9, 1, 'The DARKNESS devours ALL!', 42, 0, 100, 0, 0, 0, 'yor\'sahj spell 2 whisper');

DELETE FROM `locales_creature_text` WHERE `entry`=55312;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55312, 0, 0, 'Иилт ки\'уотк шн\'ма йе\'глу Шат\'Яр! Х\'ИУН ИИЛТ!'),
(55312, 1, 0, 'Эз, Шуул\'уах! Ск\'вот\'гл ю\'газ ех\'гыл иилт!'),
(55312, 2, 0, 'Ак\'агтши ма ухниш, ак\'ук шг\'кул ввахун! Х\'ивн иггкш Фкуати гаг ООУ КААЗТ ШУУЛ!'),
(55312, 3, 0, 'Ск\'яхв ки\'плахф Ф\'МАГГ!'),
(55312, 3, 1, 'Х\'иун заиз Шуул\'вах, ФКУАТИ!'),
(55312, 3, 2, 'Шкул ан\'циг кувсакф КШШ\'ГА, аг\'ТЫЦАК агту!'),
(55312, 4, 0, 'КИТ аг\'зиг ыыг\'фар ИИКУААТ ОНГГ!'),
(55312, 4, 1, 'УУЛЛ лухук Х\'ИУН!'),
(55312, 5, 0, 'Вы утонете в крови древних богов! ВСЕ ВЫ!'),
(55312, 6, 0, 'О Смертокрыл! Твой верный слуга подвел тебя!'),
(55312, 7, 0, 'Не счесть нам числа, и нет такой силы, что сможет противостоять нам! Всех, кто противостоит Разрушителю, настигнут ТЫСЯЧИ СМЕРТЕЙ!'),
(55312, 8, 0, 'БЕСКОНЕЧНЫЕ МУЧЕНИЯ ждут твою душу!'),
(55312, 8, 1, 'Слава Смертокрылу, РАЗРУШИТЕЛЮ!'),
(55312, 8, 2, 'Мы пришли из МРАЧНЕЙШИХ ГЛУБИН этого мира, чтобы СНОВА ПРАВИТЬ им!'),
(55312, 9, 0, 'УЗРИ же, как мы черпаем мощь из ПРОКЛЯТОЙ ЗЕМЛИ!'),
(55312, 9, 1, 'ТЬМА пожрет ВСЕ!');