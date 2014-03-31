DELETE FROM `creature` WHERE `map`=939 AND `id` IN 
(54513, 54927, 55154, 54500, 
57329, 56182, 54695, 56308,
56248, 32639, 54728, 54884,
54883, 54882,  54865, 54864,
56036, 56002, 57410, 55762,
55502, 55837, 55419, 55838);

-- DELETE FROM `creature` WHERE `guid`=340540 AND `id`=55500 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340523 AND `id`=55500 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340530 AND `id`=55500 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340697 AND `id`=57117 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340698 AND `id`=57118 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340695 AND `id`=54645 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340696 AND `id`=54645 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340431 AND `id`=54020 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340428 AND `id`=45979 AND `map`=939;
-- DELETE FROM `creature` WHERE `guid`=340429 AND `id`=45979 AND `map`=939;

-- UPDATE `creature` SET `spawnmask`=0 WHERE `guid`=339887 AND `id`=55656 AND `map`=939;
-- UPDATE `creature` SET `spawnmask`=0 WHERE `guid`=339886 AND `id`=55656 AND `map`=939;
-- UPDATE `creature` SET `spawnmask`=0 WHERE `guid`=339885 AND `id`=55654 AND `map`=939;
UPDATE `creature` SET `spawnmask`=0 WHERE `map`=939 AND `id` IN 
(54513, 55965, 56389, 55700, 55393, 55394, 55391, 57458, 56102, 55395, 55366, 55390, 55453, 55426, 55465);

-- UPDATE `creature_addon` SET `auras`='' WHERE `guid`=339888;
-- UPDATE `creature_addon` SET `auras`='' WHERE `guid`=339884;

-- DELETE FROM `waypoint_data` WHERE `id` IN (340397, 340398, 340424);
-- UPDATE `creature_addon` SET `path_id`=0 WHERE `guid`=340397;
-- UPDATE `creature_addon` SET `path_id`=0 WHERE `guid`=340398;
-- UPDATE `creature` SET `MovementType`=0 WHERE `guid`=340397;
-- UPDATE `creature` SET `MovementType`=0 WHERE `guid`=340398;
-- UPDATE `creature_addon` SET `path_id`=0 WHERE `guid`=340424;
-- UPDATE `creature` SET `MovementType`=0 WHERE `guid`=340424;

UPDATE `creature` SET `position_x`=3127.861572, `position_y`=-5560.419922, `position_z`=18.02, `orientation`=6.03 WHERE `guid`=340424 AND `id`=55570 AND `map`=939;
UPDATE `creature` SET `position_x`=3125.645508, `position_y`=-5552.871094, `position_z`=18.03, `orientation`=5.78 WHERE `guid`=340425 AND `id`=55532 AND `map`=939;

UPDATE `creature_template_addon` SET `auras`='104746' WHERE `entry`=55532;
-- UPDATE `creature_addon` SET `auras`='104746' WHERE `guid`=340425;

UPDATE `creature_template` SET `unit_flags`=64 WHERE `entry`=55503;
UPDATE `creature_template` SET `unit_flags`=32768, `dynamicflags`=4 WHERE `entry`=55085;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=5 WHERE `entry`=55656;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=55654;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=14, `faction_H`=14, `flags_extra`=128 WHERE `entry`=56248;
UPDATE `creature_template` SET `dynamicflags`=4, `mechanic_immune_mask`=655889, `flags_extra`=1 WHERE `entry` IN (54884, 54883, 54882);
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=54865;
UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry`=54864;
UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry`=56087;
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=54864;
UPDATE `creature_template` SET `modelid2`=0, `InhabitType`=7 WHERE `entry`=55078;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=55085;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=55510;
UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry`=55502;
UPDATE `gameobject_template` SET `flags`=0, `faction`=0 WHERE `entry`=210025;
UPDATE `gameobject_template` SET `faction`=0, `flags`=0 WHERE `entry`=209541;

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(55502, 0, 0, 0, 1, 0, '103892');

UPDATE `access_requirement` SET `level_min`=85 WHERE `mapId`=939 AND `difficulty`=1;
UPDATE `instance_template` SET `script`='instance_well_of_eternity' WHERE `map`=939;

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(107934, 939, 3219.319336, -4995.555664, 194.10, 1.87),
(107979, 939, 3490.910400, -5005.292480, 197.63, 2.56),
(107691, 939, 3062.740479, -5564.296387, 18.13, 0.0);

UPDATE `gameobject_template` SET `ScriptName`='go_well_of_eternity_teleport' WHERE `entry` IN (209997, 209998, 209999, 210000);

REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES 
(13162, 0, 0, ' I am ready to be hidden by your shadowcloak.', 1, 1, 0, 0, 0, 0, '');
REPLACE INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc1`, `option_text_loc2`, `option_text_loc3`, `option_text_loc4`, `option_text_loc5`, `option_text_loc6`, `option_text_loc7`, `option_text_loc8`, `box_text_loc1`, `box_text_loc2`, `box_text_loc3`, `box_text_loc4`, `box_text_loc5`, `box_text_loc6`, `box_text_loc7`, `box_text_loc8`) VALUES 
(13162, 0, '', '', '', '', '', '', '', 'Мы готовы.', '', '', '', '', '', '', '', '');

UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_nozdormu' WHERE `entry`=55624;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_legion_demon' WHERE `entry`=55503;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_illidan_1' WHERE `entry`=55500;

UPDATE `creature_template` SET `ScriptName`='boss_perotharn' WHERE `entry`=55085;
UPDATE `creature_template` SET `ScriptName`='npc_perotharn_eye_of_perotharn' WHERE `entry` IN (55868, 55879);

REPLACE INTO `creature_loot_currency` (`creature_id`, `CurrencyId1`, `CurrencyId2`, `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3`) VALUES 
(55085, 395, NULL, NULL, 7000, NULL, NULL);

DELETE FROM `conditions` WHERE `SourceEntry` IN (105543, 105546, 105509);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 3, 105543, 0, 31, 0,  3, 55085, 'Illidan (WoE) - Absorb Fel Energy'),
(13, 1, 105546, 0, 31, 0,  3, 55085, 'Illidan (WoE) - Shadow Bolt'),
(13, 3, 105509, 0, 31, 0,  3, 55085, 'Perotharn - Attack Me Perotharn');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (104905, 104906, 107536);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(104905, 'spell_perotharn_drain_essence'),
(104906, 'spell_perotharn_drain_essence_dmg'),
(107536, 'spell_perotharn_punishing_flames_dmg');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=18618;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `ScriptName`) VALUES 
(18618, 11, 'achievement_lazy_eye');

UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_eternal_champion' WHERE `entry`=54612;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_eye_of_legion' WHERE `entry`=54747;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_enchanted_highmistress' WHERE `entry`=56579;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_enchanted_highmistress' WHERE `entry`=54589;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_royal_handmaiden' WHERE `entry`=54645;

-- REPLACE INTO `creature_formations` (`leaderGUID`, `memberGUID`, `dist`, `angle`, `groupAI`) VALUES
-- (340392, 340392, 0, 0, 2),
-- (340392, 340393, 5, 90, 2),
-- (340392, 340394, 5, 270, 2);

UPDATE `creature_template` SET `ScriptName`='boss_queen_azshara' WHERE `entry`=54853;
UPDATE `creature_template` SET `ScriptName`='npc_queen_azshara_enchanted_magus' WHERE `entry` IN (54884, 54883, 54882);
UPDATE `creature_template` SET `ScriptName`='npc_queen_azshara_hammer_of_divinity' WHERE `entry`=54864;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (102465);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102465, 'spell_queen_azshara_coldflame');

UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_illidan_2' WHERE `entry`=55532;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_doomguard_annihilator' WHERE `entry`=55519;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_tyrande' WHERE `entry`=55524;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_malfurion' WHERE `entry`=55570;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_abyssal_doombringer' WHERE `entry`=55510;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_dragon_soul' WHERE `entry`=54864;
UPDATE `creature_template` SET `ScriptName`='npc_well_of_eternity_portal_to_twisting_nether' WHERE `entry`=56087;

UPDATE `creature_template` SET `ScriptName`='boss_mannoroth' WHERE `entry`=54969;
UPDATE `creature_template` SET `ScriptName`='npc_mannoroth_varothen' WHERE `entry`=55419;
UPDATE `creature_template` SET `ScriptName`='npc_mannoroth_doomguard_debilitator' WHERE `entry`=55762;

DELETE FROM `conditions` WHERE `SourceEntry` IN 
(104961, 104625, 104648, 105041, 104678, 105073, 105075, 104688, 105524, 105523, 104822);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 104961, 0, 31, 0, 3, 55419, 'Mannoroth - Fel Drain'),
(13, 1, 104625, 0, 31, 0, 3, 45979, 'Mannoroth - Nether Portal'),
(13, 1, 104648, 0, 31, 0, 3, 45979, 'Mannoroth - Nether Portal'),
(13, 1, 105041, 0, 31, 0, 3, 54020, 'Mannoroth - Nether Tear'),
(13, 1, 104678, 0, 31, 0, 3, 55524, 'Mannoroth - Debilitating Flay'),
(13, 1, 105073, 0, 31, 0, 3, 56036, 'Mannoroth - Wrath of Elune'),
(13, 1, 105073, 1, 31, 0, 3, 56002, 'Mannoroth - Wrath of Elune'),
(13, 1, 105073, 2, 31, 0, 3, 56001, 'Mannoroth - Wrath of Elune'),
(13, 1, 105073, 3, 31, 0, 3, 57410, 'Mannoroth - Wrath of Elune'),
(13, 1, 105075, 0, 31, 0, 3, 56036, 'Mannoroth - Wrath of Elune'),
(13, 1, 105075, 1, 31, 0, 3, 56002, 'Mannoroth - Wrath of Elune'),
(13, 1, 105075, 2, 31, 0, 3, 56001, 'Mannoroth - Wrath of Elune'),
(13, 1, 105075, 3, 31, 0, 3, 57410, 'Mannoroth - Wrath of Elune'),
(13, 1, 104688, 0, 31, 0, 3, 56036, 'Mannoroth - Lunar Shot'),
(13, 1, 104688, 1, 31, 0, 3, 56002, 'Mannoroth - Lunar Shot'),
(13, 1, 104688, 2, 31, 0, 3, 56001, 'Mannoroth - Lunar Shot'),
(13, 1, 104688, 3, 31, 0, 3, 57410, 'Mannoroth - Lunar Shot'),
(13, 1, 105523, 0, 31, 0, 3, 54969, 'Mannoroth - Magistrike Arc'),
(13, 3, 104822, 0, 31, 0, 3, 54969, 'Mannoroth - Magistrike Arc');

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(105054, 939, 3338.699951, -5699.775879, 13.01, 3.87),
(105058, 939, 3338.699951, -5699.775879, 13.01, 3.87),
(105059, 939, 3338.699951, -5699.775879, 13.01, 3.87);

DELETE FROM `spell_script_names` WHERE `spell_id`=104998;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(104998, 'spell_mannoroth_gift_of_sargeras');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (18476, 18474, 18363);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `ScriptName`) VALUES 
(18363, 11, 'achievement_thats_not_cannon');

DELETE FROM `creature_text` WHERE `entry`=55624;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55624, 0, 0, 'The Dragon Soul is safe once again. Quickly, into the time portal, before this world sunders!', 14, 0, 100, 0, 0, 25960, 'woe Nozdormu event 1'),
(55624, 1, 0, 'I see you\'ve arrived. This is the eve of the sundering, when the collapse of the Well of Eternity fractured the continents of the world.', 12, 0, 100, 0, 0, 25961, 'woe Nozdormu intro 1'),
(55624, 2, 0, 'Here, we will snatch up the Dragon Soul before it is lost to the mists of time.', 12, 0, 100, 0, 0, 25962, 'woe Nozdormu intro 2'),
(55624, 3, 0, 'But first, you must bring down the protective wards of Azshara\'s Highborne lackeys. You will find them within the palace. I will scout on ahead.', 12, 0, 100, 0, 0, 25963, 'woe Nozdormu intro 3'),
(55624, 4, 0, 'Good luck, heroes!', 12, 0, 100, 0, 0, 25964, 'woe Nozdormu intro 4');

DELETE FROM `locales_creature_text` WHERE `entry`=55624;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55624, 0, 0, 'Душа Дракона в безопасности. Скорее, в портал, нужно выбираться отсюда!'),
(55624, 1, 0, 'Вот-вот случится Великий Раскол – Источник Вечности будет разрушен, и мир навсегда изменится.'),
(55624, 2, 0, 'Мы должны перехватить Душу Дракона, не дав ей пропасть в тумане времени.'),
(55624, 3, 0, 'Первым делом вам придется уничтожить защитные барьеры высокорожденных – приспешников Азшары. Вы найдете их во дворце. Я же пойду вперед.'),
(55624, 4, 0, 'Удачи, герои!');

DELETE FROM `creature_text` WHERE `entry`=55524;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55524, 0, 0, 'Malfurion...', 12, 0, 100, 0, 0, 25989, 'woe tyrande conclusion 1'),
(55524, 1, 0, 'By the very edge...', 12, 0, 100, 0, 0, 25990, 'woe tyrande conclusion 2'),
(55524, 2, 0, 'I do not know who you are, but I thank you. Without your aid, our world would be...I do not wish to think about it. Moon goddess light your path.', 12, 0, 100, 0, 0, 25991, 'woe tyrande conclusion 3'),
(55524, 3, 0, 'He knows what we attempt. We have not much time, the forest crawls with his demons.', 12, 0, 100, 0, 0, 25993, 'woe tyrande mannoroth 1'),
(55524, 4, 0, 'Mother moon, guide us through this darkness.', 12, 0, 100, 0, 0, 25994, 'woe tyrande mannoroth 2'),
(55524, 5, 0, 'Illidan, what is in that vial? What are you doing?', 12, 0, 100, 0, 0, 25995, 'woe tyrande mannoroth 3'),
(55524, 6, 0, 'I cannot strike them! What is this demon magic?', 12, 0, 100, 0, 0, 25996, 'woe tyrande mannoroth 4'),
(55524, 7, 0, 'I will handle the demons. Elune, guide my arrows!', 12, 0, 100, 0, 0, 25997, 'woe tyrande mannoroth 5'),
(55524, 8, 0, 'Light of Elune, save me!', 14, 0, 100, 0, 0, 25998, 'woe tyrande mannoroth 6'),
(55524, 9, 0, 'I will hold them back for now!', 14, 0, 100, 0, 0, 25999, 'woe tyrande mannoroth 7'),
(55524, 10, 0, 'Illidan, I am out of arrows! Moon goddess, protect us from the darkness, that we may see your light again another night!', 12, 0, 100, 0, 0, 26000, 'woe tyrande mannoroth 8'),
(55524, 11, 0, 'Illidan...you musn\'t!', 12, 0, 100, 0, 0, 26001, 'woe tyrande mannoroth 9'),
(55524, 12, 0, 'No! Illidan!', 12, 0, 100, 0, 0, 26002, 'woe tyrande mannoroth 10'),
(55524, 13, 0, 'Malfurion, he has done it! The portal is collapsing!', 12, 0, 100, 0, 0, 26003, 'woe tyrande mannoroth 11'),
(55524, 14, 0, 'There are too many of them!', 14, 0, 100, 0, 0, 26004, 'woe tyrande mannoroth 12');

DELETE FROM `locales_creature_text` WHERE `entry`=55524;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55524, 0, 0, 'Малфурион...'),
(55524, 1, 0, 'На самом краю..'),
(55524, 2, 0, 'Я не знаю, кто вы, но... спасибо вам. Без вашей помощи наш мир... не хочу даже думать об этом. Да осветит ваш путь богиня луны.'),
(55524, 3, 0, 'Он знает, что мы замышляем. У нас мало времени. Лес кишит его демонами.'),
(55524, 4, 0, 'Богиня-луна, освети нам путь..'),
(55524, 5, 0, 'Иллидан, что в этом флаконе? Что ты делаешь?'),
(55524, 6, 0, 'Я не могу поразить их! Что это за демоническая магия?'),
(55524, 7, 0, 'Я займусь этими демонами. Элуна, направь мои стрелы!'),
(55524, 8, 0, 'Свет Элуны, спаси меня!'),
(55524, 9, 0, 'Теперь я удержу их!'),
(55524, 10, 0, 'Иллидан, у меня кончились стрелы! Богиня луны, защити нас от этой тьмы, чтобы мы снова узрели твой свет!'),
(55524, 11, 0, 'Иллидан... Нет!'),
(55524, 12, 0, 'Нет! Иллидан!'),
(55524, 13, 0, 'Малфурион, у него получилось! Портал разрушается.'),
(55524, 14, 0, 'Их слишком много!');

DELETE FROM `creature_text` WHERE `entry`=54853;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54853, 0, 0, 'Ah, welcome. You are here to join us in the coming celebration? No? A pity.', 14, 0, 100, 0, 0, 26013, 'woe Azshara aggro'),
(54853, 1, 0, 'Bold of you, to strike a queen. A lesser monarch might be enraged.', 14, 0, 100, 0, 0, 26014, 'woe Azshara event 1'),
(54853, 1, 1, 'Such insolence. My temper grows short.', 14, 0, 100, 0, 0, 26015, 'woe Azshara event 2'),
(54853, 1, 2, 'Do not ask for mercy, after such an act.', 14, 0, 100, 0, 0, 26016, 'woe Azshara event 3'),
(54853, 2, 0, 'Enough! As much as I adore playing hostess, I have more pressing matters to attend to.', 14, 0, 100, 0, 0, 26017, 'woe Azshara event 4'),
(54853, 3, 0, 'Riders, to me!', 14, 0, 100, 0, 0, 26018, 'woe Azshara event 5'),
(54853, 4, 0, 'My noble Varo\'then, do return and dispose of this murderous band.', 12, 0, 100, 0, 0, 26019, 'woe Azshara event 6'),
(54853, 5, 0, 'To prepare for a world of perfection, the imperfect must be swept away.', 14, 0, 100, 0, 0, 26020, 'woe Azshara reset 1'),
(54853, 6, 0, 'I am unimpressed.', 14, 0, 100, 0, 0, 26021, 'woe Azshara slay 01'),
(54853, 6, 1, 'Your conduct was inexcusable.', 14, 0, 100, 0, 0, 26022, 'woe Azshara slay 02'),
(54853, 6, 2, 'Unfortunate, but deserved.', 14, 0, 100, 0, 0, 26023, 'woe Azshara slay 03'),
(54853, 7, 0, 'Dance for the Eternal Court.', 14, 0, 100, 0, 0, 26024, 'woe Azshara spell 1'),
(54853, 7, 1, 'If you intend to play the fool, you may as well look the part.', 14, 0, 100, 0, 0, 26025, 'woe Azshara spell 2'),
(54853, 8, 0, 'Serve Azshara, puppets, and rejoice.', 14, 0, 100, 0, 0, 26026, 'woe Azshara spell 3'),
(54853, 9, 0, 'I have no time for such diversions. Keepers of Eternity, will you stand for your queen?', 14, 0, 100, 0, 0, 26027, 'woe Azshara spell 4'),
(54853, 10, 0, 'Still these strangers would oppose your queen\'s will. Who will stop them?', 14, 0, 100, 0, 0, 26028, 'woe Azshara spell 5'),
(54853, 11, 0, 'I beseech of you, my beloved subjects: Put an end to these miscreants.', 14, 0, 100, 0, 0, 26029, 'woe Azshara spell 6');

DELETE FROM `locales_creature_text` WHERE `entry`=54853;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54853, 0, 0, 'О, добро пожаловать. Вы решили присоединиться к предстоящему празднованию? Нет? Жаль.'),
(54853, 1, 0, 'Какая дерзость – поднять руку на королеву. Другой монарх на моем месте пришел бы в ярость.'),
(54853, 1, 1, 'Какая наглость. Мое терпение на исходе.'),
(54853, 1, 2, 'После такого... даже не молите о пощаде.'),
(54853, 2, 0, 'Довольно. Хоть я и люблю быть радушной хозяйкой, у меня есть более срочные дела.'),
(54853, 3, 0, 'Наездники, ко мне!'),
(54853, 4, 0, 'Дорогой Варо\'тен, пожалуйста, вернись и разберись с этой шайкой бандитов.'),
(54853, 5, 0, 'Чтобы создать совершенный мир, придется избавиться от тех, кто несовершенен.'),
(54853, 6, 0, 'Не впечатляет.'),
(54853, 6, 1, 'Твое поведение было непростительным.'),
(54853, 6, 2, 'Участь печальная, но заслуженная.'),
(54853, 7, 0, 'Танцуй для Двора Вечности!'),
(54853, 7, 1, 'Хочешь быть шутом? Давай я тебе помогу.'),
(54853, 8, 0, 'Радуйтесь, что Азшара выбрала вас своими марионетками.'),
(54853, 9, 0, 'У меня нет времени на эти развлечения. Хранители вечности! Защищайте свою королеву!'),
(54853, 10, 0, 'Эти незнакомцы все еще противятся воле вашей королевы. Кто остановит их?'),
(54853, 11, 0, 'Мои верные вассалы, уничтожьте этих чужаков!');

DELETE FROM `creature_text` WHERE `entry`=57107;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(57107, 0, 0, 'It is them!', 14, 0, 100, 0, 0, 26030, 'woe bronze dragon1 event 1'),
(57107, 1, 0, 'Sister, look! The artifact is surrounded by an aura of darkness!', 14, 0, 100, 0, 0, 26031, 'woe bronze dragon1 event 2'),
(57107, 2, 0, 'Heroes! We have been sent by Nozdormu! Quickly, on our backs, we must get you to the Dragon Soul!', 14, 0, 100, 0, 0, 26032, 'woe bronze dragon2 event 1'),
(57107, 3, 0, 'The aspects!', 14, 0, 100, 0, 0, 26033, 'woe bronze dragon2 event 2'),
(57107, 4, 0, 'The Old Gods protect the Soul!', 14, 0, 100, 0, 0, 26034, 'woe bronze dragon4 event 1'),
(57107, 5, 0, 'The link to the portal must be broken! Quickly, to Stormrage!', 14, 0, 100, 0, 0, 26036, 'woe bronze dragon5 event 1');

DELETE FROM `locales_creature_text` WHERE `entry`=57107;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(57107, 0, 0, 'Это они!'),
(57107, 1, 0, 'Сестра, смотри! Артефакт окружен аурой тьмы!'),
(57107, 2, 0, 'Герои! Нас прислал Ноздорму! Скорее, забирайтесь к нам на спины! Мы должны попасть к Душе Дракона!'),
(57107, 3, 0, 'Аспекты!'),
(57107, 4, 0, 'Артефакт охраняют древние боги!'),
(57107, 5, 0, 'Связь с порталом должна быть разорвана! Быстрее, к Малфуриону!');

DELETE FROM `creature_text` WHERE `entry`=54927;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54927, 0, 0, 'The portal is closing! Hurry!', 14, 0, 100, 0, 0, 26037, 'woe guardian demon event 01'),
(54927, 1, 0, 'There! Go! GO!!!', 14, 0, 100, 0, 0, 26037, 'woe guardian demon event 02');

DELETE FROM `locales_creature_text` WHERE `entry`=54927;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54927, 0, 0, 'Портал закрывается! Быстрее!'),
(54927, 1, 0, 'В портал! Вперед! ВПЕРЕД!!!');

DELETE FROM `creature_text` WHERE `entry`=54882;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54882, 0, 0, 'I pray that the Light of a Thousand Moons will grant me this honor.', 14, 0, 100, 0, 0, 26042, 'woe highborne event 01'),
(54882, 1, 0, 'Yes, Light of Lights! My life is yours!', 14, 0, 100, 0, 0, 26043, 'woe highborne event 02'),
(54882, 2, 0, 'The Flower of Life calls upon me. I WILL NOT fail you, my Queen.', 14, 0, 100, 0, 0, 26044, 'woe highborne event 03');

DELETE FROM `locales_creature_text` WHERE `entry`=54882;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54882, 0, 0, 'Свет тысячи лун, даруй мне эту честь.'),
(54882, 1, 0, 'О да, Светлейшая! Распоряжайся мной!'),
(54882, 2, 0, 'Цветок Жизни взывает ко мне. Я не подведу тебя, моя королева!');

DELETE FROM `creature_text` WHERE `entry`=54883;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54883, 0, 0, 'I pray that the Light of a Thousand Moons will grant me this honor.', 14, 0, 100, 0, 0, 26042, 'woe highborne event 01'),
(54883, 1, 0, 'Yes, Light of Lights! My life is yours!', 14, 0, 100, 0, 0, 26043, 'woe highborne event 02'),
(54883, 2, 0, 'The Flower of Life calls upon me. I WILL NOT fail you, my Queen.', 14, 0, 100, 0, 0, 26044, 'woe highborne event 03');

DELETE FROM `locales_creature_text` WHERE `entry`=54883;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54883, 0, 0, 'Свет тысячи лун, даруй мне эту честь.'),
(54883, 1, 0, 'О да, Светлейшая! Распоряжайся мной!'),
(54883, 2, 0, 'Цветок Жизни взывает ко мне. Я не подведу тебя, моя королева!');

DELETE FROM `creature_text` WHERE `entry`=54884;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54884, 0, 0, 'I pray that the Light of a Thousand Moons will grant me this honor.', 14, 0, 100, 0, 0, 26042, 'woe highborne event 01'),
(54884, 1, 0, 'Yes, Light of Lights! My life is yours!', 14, 0, 100, 0, 0, 26043, 'woe highborne event 02'),
(54884, 2, 0, 'The Flower of Life calls upon me. I WILL NOT fail you, my Queen.', 14, 0, 100, 0, 0, 26044, 'woe highborne event 03');

DELETE FROM `locales_creature_text` WHERE `entry`=54884;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54884, 0, 0, 'Свет тысячи лун, даруй мне эту честь.'),
(54884, 1, 0, 'О да, Светлейшая! Распоряжайся мной!'),
(54884, 2, 0, 'Цветок Жизни взывает ко мне. Я не подведу тебя, моя королева!');

DELETE FROM `creature_text` WHERE `entry`=55500;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55500, 0, 0, 'Return to the shadows!', 12, 0, 100, 0, 0, 26048, 'woe illidan boss cloak 1'),
(55500, 1, 0, 'Nothing will stop me. Not even you, demon.', 12, 0, 100, 0, 0, 26049, 'woe illidan boss enter'),
(55500, 2, 0, 'Another demon, ready to be slaughtered.', 12, 0, 100, 0, 0, 26050, 'woe illidan boss precombat 1'),
(55500, 3, 0, 'The hunter became the prey.', 12, 0, 100, 0, 0, 26051, 'woe illidan boss victory 01'),
(55500, 4, 0, 'You did well, but for now I must continue alone. Good hunting.', 12, 0, 100, 0, 0, 26052, 'woe illidan boss victory 02'),
(55500, 5, 0, 'Your magic is pathetic. Let me show you mine.', 12, 0, 100, 0, 0, 26053, 'woe illidan break 01'),
(55500, 6, 0, 'We now hide in shadows, hidden from our enemies.', 12, 0, 100, 0, 0, 26054, 'woe illidan cloak 1'),
(55500, 7, 0, 'We\'re leaving. Stay close.', 12, 0, 100, 0, 0, 26055, 'woe illidan combat 1'),
(55500, 8, 0, 'My blades hunger.', 12, 0, 100, 0, 0, 26056, 'woe illidan combat 4'),
(55500, 9, 0, 'This won\'t hurt a bit.', 12, 0, 100, 0, 0, 26057, 'woe illidan combat 3'),
(55500, 10, 0, 'Death to the Legion!', 12, 0, 100, 0, 0, 26058, 'woe illidan combat 2'),
(55500, 11, 0, 'I will hold them back so we can get past. Be ready.', 12, 0, 100, 0, 0, 26063, 'woe illidan distract'),
(55500, 12, 0, 'My magic is fading. I\'m going through!', 12, 0, 100, 0, 0, 26064, 'woe illidan distract end'),
(55500, 13, 0, 'Come with me if you\'d like to live long enough to see me save this world!', 12, 0, 100, 0, 0, 26065, 'woe illidan escort 1'),
(55500, 14, 0, 'So many demons, not enough time.', 12, 0, 100, 0, 0, 26066, 'woe illidan escort 2'),
(55500, 15, 0, 'They will get what they deserve, in due time.', 12, 0, 100, 0, 0, 26067, 'woe illidan escort 3'),
(55500, 16, 0, 'I\'ve seen a single Guardian Demon slaughter a hundred elves. Tread lightly.', 12, 0, 100, 0, 0, 26068, 'woe illidan escort 4'),
(55500, 17, 0, 'They come endlessly from the palace.', 12, 0, 100, 0, 0, 26069, 'woe illidan escort 5'),
(55500, 18, 0, 'The stench of sulfur and brimstone... These portals are as foul as the demons themselves.', 12, 0, 100, 0, 0, 26070, 'woe illidan escort 6'),
(55500, 19, 0, 'Cut this one down from the shadows.', 12, 0, 100, 0, 0, 26071, 'woe illidan escort 7'),
(55500, 20, 0, 'Let us shut down this final portal and finish this.', 12, 0, 100, 0, 0, 26072, 'woe illidan escort 8'),
(55500, 21, 0, 'The demons should all be leaving. We will be at the palace in no time.', 12, 0, 100, 0, 0, 26073, 'woe illidan escort 9'),
(55500, 22, 0, 'The demons are no longer pouring from the palace. We can move ahead.', 12, 0, 100, 0, 0, 26074, 'woe illidan escort 10'),
(55500, 23, 0, 'Too easy.', 12, 0, 100, 0, 0, 26075, 'woe illidan escort 11'),
(55500, 24, 0, 'Over here, in the shadows.', 12, 0, 100, 0, 0, 26076, 'woe illidan greeting 1'),
(55500, 25, 0, 'Explain your presence.', 12, 0, 100, 0, 0, 26080, 'woe illidan greeting idle 1'),
(55500, 26, 0, 'Attack. I don\'t like to be kept waiting.', 12, 0, 100, 0, 0, 26081, 'woe illidan idle 1'),
(55500, 27, 0, 'I\'ll let you have the first kill. Don\'t make me regret that.', 12, 0, 100, 0, 0, 26082, 'woe illidan idle 2'),
(55500, 28, 0, 'Waiting to attack...', 12, 0, 100, 0, 0, 26083, 'woe illidan idle 3'),
(55500, 29, 0, 'Waiting to attack...', 12, 0, 100, 0, 0, 26100, 'woe illidan precombat 1'),
(55500, 30, 0, 'They stand unaware that their deaths are moments away.', 12, 0, 100, 0, 0, 26101, 'woe illidan precombat 1 1'),
(55500, 31, 0, 'My strength returns.', 12, 0, 100, 0, 0, 26102, 'woe illidan recovery 1'),
(55500, 32, 0, 'Destroy the crystal so we can move on.', 12, 0, 100, 0, 0, 26103, 'woe illidan waiting 1'),
(55500, 33, 0, 'Smash the crystal. We need to move.', 12, 0, 100, 0, 0, 26104, 'woe illidan waiting 2'),
(55500, 34, 0, 'Destroy the portal energy focus!', 12, 0, 100, 0, 0, 26105, 'woe illidan waiting 3');

DELETE FROM `locales_creature_text` WHERE `entry`=55500;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55500, 0, 0, 'Скорее, обратно в тени!'),
(55500, 1, 0, 'Ничто не остановит меня. Даже ты, демон.'),
(55500, 2, 0, 'Еще одному демону не терпится умереть.'),
(55500, 3, 0, 'Охотник стал добычей.'),
(55500, 4, 0, 'Вы помогли мне, но дальше я должен идти один. Удачной охоты!'),
(55500, 5, 0, 'Твоя жалкая магия не сравнится с моей.'),
(55500, 6, 0, 'Теперь тени скроют нас от врагов.'),
(55500, 7, 0, 'Все, уходим. Не отставайте.'),
(55500, 8, 0, 'Мои клинки жаждут крови.'),
(55500, 9, 0, 'Совсем не больно, правда?'),
(55500, 10, 0, 'Смерть Легиону!'),
(55500, 11, 0, 'Я задержу их. Это даст нам возможность пройти дальше. Приготовьтесь.'),
(55500, 12, 0, 'Моя магия слабеет. Нужно идти!'),
(55500, 13, 0, 'Теперь за мной, если хотите дожить до того момента, как я спасу мир!'),
(55500, 14, 0, 'Сколько здесь демонов! А у нас так мало времени...'),
(55500, 15, 0, 'Они получат по заслугам. В свое время.'),
(55500, 16, 0, 'Я видел, как один такой демон расправился с сотней эльфов. Будьте осторожны.'),
(55500, 17, 0, 'Им нет числа...'),
(55500, 18, 0, 'Этот запах серы... Порталы так же отвратительны, как и демоны, что их создали.'),
(55500, 19, 0, 'Подберитесь поближе и атакуйте его из теней.'),
(55500, 20, 0, 'Закройте последний портал и покончим с этим.'),
(55500, 21, 0, 'Похоже, все демоны ушли в порталы. Мы почти во дворце!'),
(55500, 22, 0, 'Поток демонов из дворца, похоже, прекратился. Мы можем двигаться дальше.'),
(55500, 23, 0, 'Слишком просто.'),
(55500, 24, 0, 'Я здесь, под прикрытием теней.'),
(55500, 25, 0, 'Зачем вы здесь?'),
(55500, 26, 0, 'Атакуйте же! Не заставляйте меня ждать.'),
(55500, 27, 0, 'Ваш удар – первый! Надеюсь, я не буду сожалеть об этом?'),
(55500, 28, 0, 'Я не люблю ждать.'),
(55500, 29, 0, 'Пора нанести удар...'),
(55500, 30, 0, 'Они даже не подозревают, что смерть стоит у них за спиной.'),
(55500, 31, 0, 'Силы возвращаются ко мне.'),
(55500, 32, 0, 'Уничтожьте кристалл – и мы продолжим путь.'),
(55500, 33, 0, 'Разбейте кристалл. Надо двигаться дальше!'),
(55500, 34, 0, 'Уничтожьте этот портал!');

DELETE FROM `creature_text` WHERE `entry`=55532;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55532, 0, 0, 'The artifact!', 14, 0, 100, 0, 0, 26059, 'woe illidan conclusion 1'),
(55532, 1, 0, 'Brother. A timely arrival...', 12, 0, 100, 0, 0, 26060, 'woe illidan conclusion 2'),
(55532, 2, 0, 'Aye. It\'s been twisted and turned by too many spells. The fuss we - especially you - made with the portal was too much! The same spell that sent the Burning Legion back into their foul realm now works on the well! It\'s devouring itself and taking its surroundings with it! Fascinating, isn\'t it?', 12, 0, 100, 0, 0, 26061, 'woe illidan conclusion 3'),
(55532, 3, 0, 'If you\'ve a way out of here, we should probably use it! I\'ve tried casting myself and Tyrande out of here, but the well is too much in flux!', 12, 0, 100, 0, 0, 26062, 'woe illidan conclusion 4'),
(55532, 4, 0, 'Can you close the portal, brother?', 12, 0, 100, 0, 0, 26084, 'woe illidan mannoroth 1'),
(55532, 5, 0, 'Very well, we shall break it for you.', 12, 0, 100, 0, 0, 26086, 'woe illidan mannoroth 2'),
(55532, 6, 0, 'Let them come.', 12, 0, 100, 0, 0, 26085, 'woe illidan mannoroth 3'),
(55532, 7, 0, 'So many demons.', 12, 0, 100, 0, 0, 26087, 'woe illidan mannoroth 4'),
(55532, 8, 0, 'Weak, pitiful creatures. Hardly worthy of being called a legion.', 12, 0, 100, 0, 0, 26088, 'woe illidan mannoroth 5'),
(55532, 9, 0, 'Oh this will be fun...', 12, 0, 100, 0, 0, 26089, 'woe illidan mannoroth 6'),
(55532, 10, 0, 'Wait, I have an idea.', 12, 0, 100, 0, 0, 26090, 'woe illidan mannoroth 7'),
(55532, 11, 0, 'What our people could not.', 12, 0, 100, 0, 0, 26091, 'woe illidan mannoroth 8'),
(55532, 12, 0, 'Yes...YES. I can feel the raw power of the Well of Eternity! It is time to end this charade!', 12, 0, 100, 0, 0, 26092, 'woe illidan mannoroth 9'),
(55532, 13, 0, 'They are not where they appear to be! Strike in an area, it is the only way to uncover the real one!', 12, 0, 100, 0, 0, 26093, 'woe illidan mannoroth 10'),
(55532, 14, 0, 'Handle Varo\'then. Mannoroth is mine.', 12, 0, 100, 0, 0, 26094, 'woe illidan mannoroth 11'),
(55532, 15, 0, 'The sword has pierced his infernal armor! Strike him down!', 14, 0, 100, 0, 0, 26095, 'woe illidan mannoroth 14'),
(55532, 16, 0, 'You are not the sole wielder of Sargeras\' power, Mannoroth! Behold!', 14, 0, 100, 0, 0, 26096, 'woe illidan mannoroth 15'),
(55532, 17, 0, 'I will be the savior of our people! I WILL FULFILL MY DESTINY!', 14, 0, 100, 0, 0, 26098, 'woe illidan mannoroth 16'),
(55532, 18, 0, '[Demonic] Revos ill ok mordanas archim maz naztheros! Archim xi ante maz-re mishun te nagas!', 14, 0, 100, 0, 0, 26097, 'woe illidan mannoroth 17'),
(55532, 19, 0, 'He is still connected to the Well somehow! Focus your attacks on Mannoroth, we must disrupt his concentration!', 14, 0, 100, 0, 0, 26099, 'woe illidan mannoroth 18');


DELETE FROM `locales_creature_text` WHERE `entry`=55532;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55532, 0, 0, 'Артефакт!'),
(55532, 1, 0, 'Брат... как вовремя...'),
(55532, 2, 0, 'Да. Он был искажен и изменен всеми этими заклинаниями. Мы – и особенно ты – перестарались и, похоже, нарушили баланс! Заклинание, что отправило Пылающий легион назад в их мерзостный мир, овладело и самим Источником! Он поглощает сам себя вместе с окрестностями! Потрясающе!'),
(55532, 3, 0, 'Ты знаешь, как выбраться отсюда? Источник создает сильные магические помехи. Я не смог вытащить себя и Тиранду!'),
(55532, 4, 0, 'Ты сможешь закрыть портал, брат?'),
(55532, 5, 0, 'Тогда мы сделаем это за тебя.'),
(55532, 6, 0, 'Мы избавимся от них.'),
(55532, 7, 0, 'Их так много.'),
(55532, 8, 0, 'Слабые, жалкие создания. И это недоразумение называют легионом?'),
(55532, 9, 0, 'Хм, а вот это уже веселее.'),
(55532, 10, 0, 'Постойте, есть идея.'),
(55532, 11, 0, 'То, что не смог сделать наш народ.'),
(55532, 12, 0, 'Да... ДА. Я чувствую энергию Источника Вечности! Пора заканчивать этот фарс!'),
(55532, 13, 0, 'Они не там, где тебе кажется! Бей по площади, и тогда попадешь по настоящей цели!'),
(55532, 14, 0, 'Займитесь Варо\'теном. Маннорот – мой.'),
(55532, 15, 0, 'Меч пронзил его броню! Добейте его!'),
(55532, 16, 0, 'Мощь Саргераса дарована не только тебе, Маннорот! Трепещи!'),
(55532, 17, 0, 'Я спасу наш народ! Я ИСПОЛНЮ ПРЕДНАЧЕРТАННОЕ СУДЬБОЙ!'),
(55532, 18, 0, '[Язык демонов] Ревос илль ок морданас архим маз назтерос. Архим зи антэ маз-ре мишун те нагаз!'),
(55532, 19, 0, 'Он все еще связан с Источником! Атакуйте Маннорота! Помешайте ему!');

DELETE FROM `creature_text` WHERE `entry`=55085;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55085, 0, 0, 'No mortal may stand before me and live!', 12, 0, 100, 0, 0, 26112, 'woe perotharn aggro'),
(55085, 1, 0, 'Noooo! How could this be?', 12, 0, 100, 0, 0, 26113, 'woe perotharn'),
(55085, 2, 0, 'ENOUGH! It is time to end this game!', 12, 0, 100, 0, 0, 26117, 'woe perotharn event 4'),
(55085, 3, 0, 'He is near, lurking in the shadows... I can sense it.', 12, 0, 100, 0, 0, 26118, 'woe perotharn event 5'),
(55085, 4, 0, 'You, Felguard. Hold this area.', 12, 0, 100, 0, 0, 26120, 'woe perotharn event 7'),
(55085, 5, 0, 'The rest of you, secure the courtyard.', 12, 0, 100, 0, 0, 26121, 'woe perotharn event 8'),
(55085, 6, 0, 'I can see you!', 12, 0, 100, 0, 0, 26122, 'woe perotharn event 9'),
(55085, 7, 0, 'You hide well, worms. But how long you can delay your doom?', 12, 0, 100, 0, 0, 26123, 'woe perotharn event 10'),
(55085, 8, 0, 'I WILL find you!', 12, 0, 100, 0, 0, 26124, 'woe perotharn event 11'),
(55085, 8, 1, 'The shadows will not save you!', 12, 0, 100, 0, 0, 26125, 'woe perotharn event 12'),
(55085, 8, 2, 'Cower in hiding', 12, 0, 100, 0, 0, 26126, 'woe perotharn event 13'),
(55085, 9, 0, 'Who shut down the portals!? Clever. Little worms.', 12, 0, 100, 0, 0, 26127, 'woe perotharn intro 1'),
(55085, 9, 1, 'None will reach the palace without besting Peroth\'arn...first of the feltouched.', 12, 0, 100, 0, 0, 26128, 'woe perotharn intro 2'),
(55085, 10, 0, 'laughs', 16, 0, 100, 0, 0, 26129, 'woe perotharn slay 1'),
(55085, 10, 1, 'You lose! Hah!', 12, 0, 100, 0, 0, 26130, 'woe perotharn slay 2'),
(55085, 10, 2, 'None compare to Peroth\'arn.', 12, 0, 100, 0, 0, 26131, 'woe perotharn slay 3'),
(55085, 11, 0, 'Your essence is MINE!', 12, 0, 100, 0, 0, 26132, 'woe perotharn spell 1'),
(55085, 12, 0, 'The shadows serve me now!', 12, 0, 100, 0, 0, 26133, 'woe perotharn spell 2');

DELETE FROM `locales_creature_text` WHERE `entry`=55085;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55085, 0, 0, 'Ни один смертный не сможет противостоять мне!'),
(55085, 1, 0, 'НЕТ! Как это возможно?!'),
(55085, 2, 0, 'ХВАТИТ! Эта игра мне надоела!'),
(55085, 3, 0, 'Он где-то здесь, прячется в тенях...'),
(55085, 4, 0, 'Страж Скверны... Ты останешься здесь.'),
(55085, 5, 0, 'Остальные – охраняйте двор.'),
(55085, 6, 0, 'Я тебя вижу.'),
(55085, 7, 0, 'Неплохо прячетесь. Но долго ли вы сможете противиться судьбе?'),
(55085, 8, 0, 'Я найду тебя!'),
(55085, 8, 1, 'Тени не спасут вас.'),
(55085, 8, 2, 'Прячьтесь и дрожите от страха.'),
(55085, 9, 0, 'Кто закрыл порталы? Ах вы, пронырливые создания!'),
(55085, 9, 1, 'Никто не войдет во дворец, не победив Перот\'арна, первого, кто причастился Скверны!'),
(55085, 10, 0, 'смеется'),
(55085, 10, 01, 'Тебе не повезло.'),
(55085, 10, 2, 'Никто не сравнится с Перот\'арном.'),
(55085, 11, 0, 'Ваша сущность принадлежит мне.'),
(55085, 12, 0, 'Теперь тени служат мне...');

DELETE FROM `creature_text` WHERE `entry`=55570;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55570, 0, 0, 'Hush, Tyrande. Where is Illidan?', 12, 0, 100, 0, 0, 26490, 'woe malfurion conclusion 01'),
(55570, 1, 0, 'Illidan! The well is out of control!', 14, 0, 100, 0, 0, 26491, 'woe malfurion conclusion 02'),
(55570, 2, 0, 'Not if we\'re caught up in it! Why weren\'t you running? What have you been doing with your hand in the Well?', 12, 0, 100, 0, 0, 26492, 'woe malfurion conclusion 03'),
(55570, 3, 0, 'This way!', 14, 0, 100, 0, 0, 26493, 'woe malfurion conclusion 04'),
(55570, 4, 0, 'It is being maintained by the will of a powerful demon... Mannoroth.', 12, 0, 100, 0, 0, 26494, 'woe malfurion mannoroth 01'),
(55570, 5, 0, 'I cannot break his will alone...', 12, 0, 100, 0, 0, 26495, 'woe malfurion mannoroth 02');

DELETE FROM `locales_creature_text` WHERE `entry`=55570;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55570, 0, 0, 'Тише, Тиранда. Где Иллидан?'),
(55570, 1, 0, 'Иллидан! Источник вышел из-под контроля!'),
(55570, 2, 0, 'Он может поглотить и нас! Почему ты не бежал? Зачем ты опустил руку в Источник?'),
(55570, 3, 0, 'За мной!'),
(55570, 4, 0, 'Его поддерживает воля могущественного демона... Маннорота.'),
(55570, 5, 0, 'Я не смогу справиться с ним один...');

DELETE FROM `creature_text` WHERE `entry`=55419;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55419, 0, 0, 'For you, Azshara!', 12, 0, 100, 0, 0, 26134, 'woe varothen aggro'),
(55419, 1, 0, 'Lights of lights! I have failed you. I am sorry, my Azshara...', 12, 0, 100, 0, 0, 26135, 'woe varothen death'),
(55419, 2, 0, 'None may cross the queen!', 14, 0, 100, 0, 0, 26138, 'woe varothen kill 01'),
(55419, 2, 1, 'A deserved dead!', 14, 0, 100, 0, 0, 26139, 'woe varothen kill 02'),
(55419, 2, 2, 'In Azshara\'s name!', 14, 0, 100, 0, 0, 26140, 'woe varothen kill 03');

DELETE FROM `locales_creature_text` WHERE `entry`=55419;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55419, 0, 0, 'За тебя, Азшара.'),
(55419, 1, 0, 'Светлейшая... я подвел тебя. Мне так жаль... Моя королева...'),
(55419, 2, 0, 'Никому не дозволено поднимать руку на королеву!'),
(55419, 2, 1, 'Заслуженная смерть!'),
(55419, 2, 2, 'Во имя Азшары!');

DELETE FROM `creature_text` WHERE `entry`=54969;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54969, 0, 0, 'Come, Stormrage, and I will show you what happens to those that betray the lord of the Legion!', 14, 0, 100, 0, 0, 26478, 'woe mannoroth aggro'),
(54969, 1, 0, 'No... no! This victory will not be ripped from my grasp! I will not return to him in failure! I will not be torn from this pitiful world! No... NOOOOOOOO!!', 14, 0, 100, 0, 0, 26479, 'woe mannoroth death'),
(54969, 2, 0, 'Varo\'then, see that I am not disrupted by this rabble.', 14, 0, 100, 0, 0, 26480, 'woe mannoroth event 01 varothen'),
(54969, 3, 0, 'Lord Sargeras, I will not fail you! Sweep your molten fist through this world so that it may be reborn in flames and darkness!', 14, 0, 100, 0, 0, 26481, 'woe mannoroth event 02 sargeras'),
(54969, 4, 0, 'Rrraaaghhh!', 14, 0, 100, 0, 0, 26482, 'woe mannoroth event 03'),
(54969, 5, 0, 'Your blood is MINE, elf!', 14, 0, 100, 0, 0, 26483, 'woe mannoroth event 04'),
(54969, 6, 0, 'Yes... yes! I can feel his burning eyes upon me, he is close...so close. And then your world will be unmade, your lives as nothing!', 14, 0, 100, 0, 0, 26484, 'woe mannoroth event 05'),
(54969, 7, 0, 'Fall and die before me!', 14, 0, 100, 0, 0, 26485, 'woe mannoroth kill 01'),
(54969, 7, 1, 'Squirm...Scream. Hahahaha!', 14, 0, 100, 0, 0, 26486, 'woe mannoroth kill 02'),
(54969, 7, 2, 'Useless!', 14, 0, 100, 0, 0, 26487, 'woe mannoroth kill 03'),
(54969, 8, 0, '[Demonic] Amanare maev il azgalada zila ashj ashj zila enkil!', 14, 0, 100, 0, 0, 26488, 'woe mannoroth spell 01');

DELETE FROM `locales_creature_text` WHERE `entry`=54969;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(54969, 0, 0, 'Ты предал владыку Легиона, Иллидан. Сейчас ты узнаешь, что ждет изменников.'),
(54969, 1, 0, 'Нет... Нет! Вы не вырвете эту победу из моих рук! Я не вернусь к нему побежденным! Вы не изгоните меня из этого... несчастного... мира! Нет! Нееееееееееееееет!!!'),
(54969, 2, 0, 'Варо\'тен, проследи, чтобы мне не мешали!'),
(54969, 3, 0, 'Владыка Саргерас! Я не подведу тебя! Смети все с лица этого мира, и да возродится он в огне и мраке!'),
(54969, 4, 0, 'Ррррааааааа!!'),
(54969, 5, 0, 'Я уничтожу тебя, эльф!'),
(54969, 6, 0, 'Да... Да! Я чувствую его горящий взгляд, он рядом... совсем рядом! И когда он придет, наступит конец вашему миру!'),
(54969, 7, 0, 'Хахаха, пади передо мной, умри!'),
(54969, 7, 1, 'Корчись... в муках. Хахаха!'),
(54969, 7, 2, 'Ничтожество!'),
(54969, 8, 0, '[Язык демонов] Аманарэ маэв ил азгалада зила ашж ашж зила энкил.');