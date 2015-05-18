
UPDATE `quest_template` SET `RequestItemsText`='A fine day for a ceremony. Don\'t you think, $n? ' WHERE `Id`=29307;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, $n.\r\n\r\nNow prepare to receive a hero\'s reward. ' WHERE `Id`=29307;
UPDATE `quest_template` SET `RequestItemsText`='A fine day for a ceremony. Don\'t you think, $n? ' WHERE `Id`=29308;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, $n.\r\n\r\nNow prepare to receive a hero\'s reward. ' WHERE `Id`=29308;
UPDATE `quest_template` SET `RequestItemsText`='Behold! Dragonwrath, Tarecgosa\'s Rest. ' WHERE `Id`=29309;
UPDATE `quest_template` SET `RequestItemsText`='Behold! Dragonwrath, Tarecgosa\'s Rest. ' WHERE `Id`=29312;
UPDATE `quest_template` SET `OfferRewardText`='Your heroic deeds will never be forgotten, $n. ' WHERE `Id`=29309;
UPDATE `quest_template` SET `OfferRewardText`='Your heroic deeds will never be forgotten, $n. ' WHERE `Id`=29312;

DELETE FROM `creature` WHERE `id` IN (53700, 53398, 53400, 53406);

DELETE FROM `creature` WHERE `id` IN (53392, 53396, 53422, 53394, 53408);

-- DELETE FROM `game_event_creature` WHERE `guid`=305518 AND `eventEntry`=29;
-- DELETE FROM `game_event_creature` WHERE `guid`=305529 AND `eventEntry`=29;
-- DELETE FROM `game_event_creature` WHERE `guid`=356789 AND `eventEntry`=29;

DELETE FROM `creature` WHERE `id` IN (53371, 53466);
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(53371, 1, 1, 65535, 0, 0, 1671.380121, -4356.714355, 26.77, 2.95, 1800, 0, 0, 77490, 0, 0, 0, 0, 0),
(53466, 0, 1, 65535, 0, 0, -8639.159180, 770.843079, 97.26, 2.27, 1800, 0, 0, 77490, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `gossip_menu_id`=53371 WHERE `entry`=53371;
UPDATE `creature_template` SET `gossip_menu_id`=53466 WHERE `entry`=53466;
REPLACE INTO `gossip_menu` (`entry`, `text_id`) VALUES 
(53371, 53371),
(53466, 53466);
REPLACE INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES 
(53371, 'Your deeds shall live on forever in the memories of every blue dragon, $N.', NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 1),
(53466, 'Your deeds shall live on forever in the memories of every blue dragon, $N.', NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 1);
REPLACE INTO `locales_npc_text` (`entry`, `Text0_0_loc1`, `Text0_0_loc2`, `Text0_0_loc3`, `Text0_0_loc4`, `Text0_0_loc5`, `Text0_0_loc6`, `Text0_0_loc7`, `Text0_0_loc8`, `Text0_1_loc1`, `Text0_1_loc2`, `Text0_1_loc3`, `Text0_1_loc4`, `Text0_1_loc5`, `Text0_1_loc6`, `Text0_1_loc7`, `Text0_1_loc8`, `Text1_0_loc1`, `Text1_0_loc2`, `Text1_0_loc3`, `Text1_0_loc4`, `Text1_0_loc5`, `Text1_0_loc6`, `Text1_0_loc7`, `Text1_0_loc8`, `Text1_1_loc1`, `Text1_1_loc2`, `Text1_1_loc3`, `Text1_1_loc4`, `Text1_1_loc5`, `Text1_1_loc6`, `Text1_1_loc7`, `Text1_1_loc8`, `Text2_0_loc1`, `Text2_0_loc2`, `Text2_0_loc3`, `Text2_0_loc4`, `Text2_0_loc5`, `Text2_0_loc6`, `Text2_0_loc7`, `Text2_0_loc8`, `Text2_1_loc1`, `Text2_1_loc2`, `Text2_1_loc3`, `Text2_1_loc4`, `Text2_1_loc5`, `Text2_1_loc6`, `Text2_1_loc7`, `Text2_1_loc8`, `Text3_0_loc1`, `Text3_0_loc2`, `Text3_0_loc3`, `Text3_0_loc4`, `Text3_0_loc5`, `Text3_0_loc6`, `Text3_0_loc7`, `Text3_0_loc8`, `Text3_1_loc1`, `Text3_1_loc2`, `Text3_1_loc3`, `Text3_1_loc4`, `Text3_1_loc5`, `Text3_1_loc6`, `Text3_1_loc7`, `Text3_1_loc8`, `Text4_0_loc1`, `Text4_0_loc2`, `Text4_0_loc3`, `Text4_0_loc4`, `Text4_0_loc5`, `Text4_0_loc6`, `Text4_0_loc7`, `Text4_0_loc8`, `Text4_1_loc1`, `Text4_1_loc2`, `Text4_1_loc3`, `Text4_1_loc4`, `Text4_1_loc5`, `Text4_1_loc6`, `Text4_1_loc7`, `Text4_1_loc8`, `Text5_0_loc1`, `Text5_0_loc2`, `Text5_0_loc3`, `Text5_0_loc4`, `Text5_0_loc5`, `Text5_0_loc6`, `Text5_0_loc7`, `Text5_0_loc8`, `Text5_1_loc1`, `Text5_1_loc2`, `Text5_1_loc3`, `Text5_1_loc4`, `Text5_1_loc5`, `Text5_1_loc6`, `Text5_1_loc7`, `Text5_1_loc8`, `Text6_0_loc1`, `Text6_0_loc2`, `Text6_0_loc3`, `Text6_0_loc4`, `Text6_0_loc5`, `Text6_0_loc6`, `Text6_0_loc7`, `Text6_0_loc8`, `Text6_1_loc1`, `Text6_1_loc2`, `Text6_1_loc3`, `Text6_1_loc4`, `Text6_1_loc5`, `Text6_1_loc6`, `Text6_1_loc7`, `Text6_1_loc8`, `Text7_0_loc1`, `Text7_0_loc2`, `Text7_0_loc3`, `Text7_0_loc4`, `Text7_0_loc5`, `Text7_0_loc6`, `Text7_0_loc7`, `Text7_0_loc8`, `Text7_1_loc1`, `Text7_1_loc2`, `Text7_1_loc3`, `Text7_1_loc4`, `Text7_1_loc5`, `Text7_1_loc6`, `Text7_1_loc7`, `Text7_1_loc8`) VALUES 
(53371, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Твои подвиги навсегда останутся в памяти синих драконов, $N.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(53466, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Твои подвиги навсегда останутся в памяти синих драконов, $N.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES 
(53371, 0, 0, 'I\'m ready to take part of the ceremony, Hallegosa.', 1, 1, 0, 0, 0, 0, NULL),
(53466, 0, 0, 'I\'m ready to take part of the ceremony, Hallegosa.', 1, 1, 0, 0, 0, 0, NULL);
REPLACE INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc1`, `option_text_loc2`, `option_text_loc3`, `option_text_loc4`, `option_text_loc5`, `option_text_loc6`, `option_text_loc7`, `option_text_loc8`, `box_text_loc1`, `box_text_loc2`, `box_text_loc3`, `box_text_loc4`, `box_text_loc5`, `box_text_loc6`, `box_text_loc7`, `box_text_loc8`) VALUES 
(53371, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я готов принять участие в церемонии, Халлегоса.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(53466, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я готов принять участие в церемонии, Халлегоса.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
DELETE FROM `conditions` WHERE `SourceGroup` IN (53371, 53466) AND SourceTypeOrReferenceId IN (14, 15);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(14, 53371, 53371, 0, 0, 9, 0, 29309, 0, 0, 0, 0, '', NULL),
(15, 53371, 0, 0, 0, 9, 0, 29309, 0, 0, 0, 0, '', NULL),
(14, 53466, 53466, 0, 0, 9, 0, 29312, 0, 0, 0, 0, '', NULL),
(15, 53466, 0, 0, 0, 9, 0, 29312, 0, 0, 0, 0, '', NULL);


UPDATE `creature_template` SET `Inhabittype`=5 WHERE `entry`=53422;
UPDATE `creature_template` SET `Inhabittype`=5 WHERE `entry` IN (53396, 53392);

UPDATE `creature_template` SET `ScriptName`='npc_hallegosa' WHERE `entry` IN (53371, 53466);
UPDATE `creature_template` SET `ScriptName`='npc_kalecgos_53392' WHERE `entry`=53392;
UPDATE `creature_template` SET `ScriptName`='npc_tarecgosa_53396' WHERE `entry`=53396;
UPDATE `creature_template` SET `ScriptName`='npc_blue_dragon_53394' WHERE `entry`=53394;
UPDATE `creature_template` SET `ScriptName`='npc_stalker_53422' WHERE `entry`=53422;

DELETE FROM `conditions` WHERE `SourceEntry`=98439;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 98439, 0, 31, 0, 3, 53396, 'Dragonwrath - Essence of Taracgosa'),
(13, 1, 98439, 1, 31, 0, 3, 53394, 'Dragonwrath - Essence of Taracgosa');

DELETE FROM `creature_text` WHERE `entry`=53392;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53392, 0, 0, 'Welcome, friend. We\'ve been expecting you.', 12, 0, 100, 0, 0, 24874, 'kalecgos final 1'),
(53392, 1, 0, 'You are to be congratulated for your success in the Firelands.', 12, 0, 100, 0, 0, 24875, 'kalecgos final 2'),
(53392, 2, 0, 'But praise alone will not suffice.', 12, 0, 100, 0, 0, 24876, 'kalecgos final 3'),
(53392, 3, 0, 'We owe you a great deal for the part you have played in the destiny of our flight.', 12, 0, 100, 0, 0, 24877, 'kalecgos final 4'),
(53392, 4, 0, 'Present me your staff, that we may endow it with great power.', 12, 0, 100, 0, 0, 24878, 'kalecgos final 5'),
(53392, 5, 0, 'Finally, Tarecgosa, our beloved sister whose essence you have generously borne, has chosen this staff as her permanent home.', 12, 0, 100, 0, 0, 24879, 'kalecgos final 6'),
(53392, 6, 0, 'May her presence be a source of comfort and companionship to you.', 12, 0, 100, 0, 0, 24880, 'kalecgos final 7'),
(53392, 7, 0, 'We thank you, friend of the blue dragonflight.', 12, 0, 100, 0, 0, 24881, 'kalecgos final 8');

DELETE FROM `locales_creature_text` WHERE `entry`=53392;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53392, 0, 0, 'Добро пожаловать. Мы тебя ждали.'),
(53392, 1, 0, 'Поздравляю тебя с победой на Огненных Просторах.'),
(53392, 2, 0, 'Но одних слов будет недостаточно.'),
(53392, 3, 0, 'Мы благодарны тебе за участие в судьбе нашего рода.'),
(53392, 4, 0, 'Протяни мне свой посох, и я наделю его великой силой.'),
(53392, 5, 0, 'Таресгоса, наша возлюбленная сестра, чью сущность ты хранишь, избрала этот посох своим пристанищем.'),
(53392, 6, 0, 'Пусть ее присутствие вдохновляет и ободряет тебя в пути.'),
(53392, 7, 0, 'Спасибо тебе, друг синих драконов.');