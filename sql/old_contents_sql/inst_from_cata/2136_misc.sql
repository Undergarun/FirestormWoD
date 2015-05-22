UPDATE `creature_template` SET `gossip_menu_id`=13160, `AIName`='SmartAI' WHERE `entry`=55488;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (57800, 57801);

DELETE FROM `gossip_menu` WHERE `entry` IN (13160, 57800);
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES 
(13160, 55488),
(57800, 57800);

REPLACE INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`, `WDBVerified`) VALUES 
(55488, 'Lost. Stolen! This is a terrible blow. Terrible!', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 1),
(57800, 'I\'ll do it for 10 000 gold.', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, 1);

REPLACE INTO `locales_npc_text` (`entry`, `Text0_0_loc1`, `Text0_0_loc2`, `Text0_0_loc3`, `Text0_0_loc4`, `Text0_0_loc5`, `Text0_0_loc6`, `Text0_0_loc7`, `Text0_0_loc8`, `Text0_1_loc1`, `Text0_1_loc2`, `Text0_1_loc3`, `Text0_1_loc4`, `Text0_1_loc5`, `Text0_1_loc6`, `Text0_1_loc7`, `Text0_1_loc8`, `Text1_0_loc1`, `Text1_0_loc2`, `Text1_0_loc3`, `Text1_0_loc4`, `Text1_0_loc5`, `Text1_0_loc6`, `Text1_0_loc7`, `Text1_0_loc8`, `Text1_1_loc1`, `Text1_1_loc2`, `Text1_1_loc3`, `Text1_1_loc4`, `Text1_1_loc5`, `Text1_1_loc6`, `Text1_1_loc7`, `Text1_1_loc8`, `Text2_0_loc1`, `Text2_0_loc2`, `Text2_0_loc3`, `Text2_0_loc4`, `Text2_0_loc5`, `Text2_0_loc6`, `Text2_0_loc7`, `Text2_0_loc8`, `Text2_1_loc1`, `Text2_1_loc2`, `Text2_1_loc3`, `Text2_1_loc4`, `Text2_1_loc5`, `Text2_1_loc6`, `Text2_1_loc7`, `Text2_1_loc8`, `Text3_0_loc1`, `Text3_0_loc2`, `Text3_0_loc3`, `Text3_0_loc4`, `Text3_0_loc5`, `Text3_0_loc6`, `Text3_0_loc7`, `Text3_0_loc8`, `Text3_1_loc1`, `Text3_1_loc2`, `Text3_1_loc3`, `Text3_1_loc4`, `Text3_1_loc5`, `Text3_1_loc6`, `Text3_1_loc7`, `Text3_1_loc8`, `Text4_0_loc1`, `Text4_0_loc2`, `Text4_0_loc3`, `Text4_0_loc4`, `Text4_0_loc5`, `Text4_0_loc6`, `Text4_0_loc7`, `Text4_0_loc8`, `Text4_1_loc1`, `Text4_1_loc2`, `Text4_1_loc3`, `Text4_1_loc4`, `Text4_1_loc5`, `Text4_1_loc6`, `Text4_1_loc7`, `Text4_1_loc8`, `Text5_0_loc1`, `Text5_0_loc2`, `Text5_0_loc3`, `Text5_0_loc4`, `Text5_0_loc5`, `Text5_0_loc6`, `Text5_0_loc7`, `Text5_0_loc8`, `Text5_1_loc1`, `Text5_1_loc2`, `Text5_1_loc3`, `Text5_1_loc4`, `Text5_1_loc5`, `Text5_1_loc6`, `Text5_1_loc7`, `Text5_1_loc8`, `Text6_0_loc1`, `Text6_0_loc2`, `Text6_0_loc3`, `Text6_0_loc4`, `Text6_0_loc5`, `Text6_0_loc6`, `Text6_0_loc7`, `Text6_0_loc8`, `Text6_1_loc1`, `Text6_1_loc2`, `Text6_1_loc3`, `Text6_1_loc4`, `Text6_1_loc5`, `Text6_1_loc6`, `Text6_1_loc7`, `Text6_1_loc8`, `Text7_0_loc1`, `Text7_0_loc2`, `Text7_0_loc3`, `Text7_0_loc4`, `Text7_0_loc5`, `Text7_0_loc6`, `Text7_0_loc7`, `Text7_0_loc8`, `Text7_1_loc1`, `Text7_1_loc2`, `Text7_1_loc3`, `Text7_1_loc4`, `Text7_1_loc5`, `Text7_1_loc6`, `Text7_1_loc7`, `Text7_1_loc8`) VALUES 
(55488, '', '', '', '', '', '', '', 'Потеряно. Украдено! Это ужасный удар. Ужасный!', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', ''),
(57800, '', '', '', '', '', '', '', 'О, понимаю. Занимаемся темными делишками, да, $c? О, не извольте беспокоиться. Мы гарантируем полную конфеденциальность.\r\n\Отличной работы кольцо, надо сказать. Мы можем зарядить его для вас... за скромную плату. Для полной зарядки кольца потребуется около половины цикла вашего мира. Говоря проще, двенадцать часов.\r\nКак насчет 10 000 золотых?\r\n', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '');

DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (13160, 13348, 57800);
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES 
(13160, 0, 0, 'What happened here? What was stolen?', 1, 1, 0, 0, 0, 0, ''),
(13160, 1, 0, 'Lord Afrasastrasz sent me. He said you have a message to decode.', 1, 1, 0, 0, 0, 0, ''),
(13348, 0, 0, 'Can you charge my cryptomancer\'s decoder ring?', 1, 1, 57800, 0, 0, 0, ''),
(57800, 0, 0, '<Pay 10 000 gold.>', 1, 1, 0, 0, 0, 100000000, 'Do you want to pay?'),
(57800, 1, 0, 'Ten thousand?! This is a robbery! Just try to go to sleep tonight - I\'ll slaughter you while you dream!', 1, 1, 0, 0, 0, 0, '');

DELETE FROM `locales_gossip_menu_option` WHERE `menu_id` IN (13160, 13348, 57800);
INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc1`, `option_text_loc2`, `option_text_loc3`, `option_text_loc4`, `option_text_loc5`, `option_text_loc6`, `option_text_loc7`, `option_text_loc8`, `box_text_loc1`, `box_text_loc2`, `box_text_loc3`, `box_text_loc4`, `box_text_loc5`, `box_text_loc6`, `box_text_loc7`, `box_text_loc8`) VALUES 
(13160, 0, '', '', '', '', '', '', '', 'Что здесь произошло? Что-то украли?', '', '', '', '', '', '', '', ''),
(13160, 1, '', '', '', '', '', '', '', 'Лорд Афрасастраз прислал меня. Он сказал, что у вас есть письмо, которое нужно расшифровать.', '', '', '', '', '', '', '', ''),
(13348, 0, '', '', '', '', '', '', '', 'Можете зарядить мое дешифрующее кольцо тайнописца?', '', '', '', '', '', '', '', ''),
(57800, 0, '', '', '', '', '', '', '', '<Заплатить 10 000 золотых.>', '', '', '', '', '', '', '', 'Вы хотите заплатить деньги?'),
(57800, 1, '', '', '', '', '', '', '', 'Десять тысяч?! Это грабеж! Только попробуй заснуть сегодня ночью - приду и во сне зарежу!', '', '', '', '', '', '', '', '');

DELETE FROM `conditions` WHERE `SourceGroup` IN (13160, 13348) AND SourceTypeOrReferenceId IN (14, 15);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 13160, 1, 0, 0, 9, 0, 29802, 0, 0, 0, 0, '', NULL),
(15, 13160, 1, 0, 0, 2, 0, 74750, 1, 1, 1, 0, '', NULL),
(15, 13348, 0, 0, 0, 9, 0, 29802, 0, 0, 0, 0, '', NULL),
(15, 13348, 0, 0, 0, 2, 0, 74749, 1, 1, 1, 0, '', NULL),
(15, 13348, 0, 0, 0, 2, 0, 74748, 1, 1, 1, 0, '', NULL),
(15, 13348, 0, 0, 0, 2, 0, 74256, 1, 0, 0, 0, '', NULL);

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (55488, 57800, 57801);
REPLACE INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(55488, 0, 0, 0, 62, 0, 100, 0, 13160, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Talk after gossip option'),
(55488, 0, 1, 0, 62, 0, 100, 0, 13160, 1, 0, 0, 11, 104569, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Cast spell after gossip option'),
(55488, 0, 2, 0, 62, 0, 100, 0, 13160, 1, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Close gossip after gossip option'),
(57800, 0, 1, 0, 62, 0, 100, 0, 57800, 0, 0, 0, 85, 104480, 16, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Cast spell after gossip option'),
(57800, 0, 0, 0, 62, 0, 100, 0, 57800, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Close gossip after gossip option'),
(57800, 0, 0, 0, 62, 0, 100, 0, 57800, 1, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Close gossip after gossip option'),
(57801, 0, 1, 0, 62, 0, 100, 0, 57800, 0, 0, 0, 85, 104480, 16, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Cast spell after gossip option'),
(57801, 0, 0, 0, 62, 0, 100, 0, 57800, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Close gossip after gossip option'),
(57801, 0, 0, 0, 62, 0, 100, 0, 57800, 1, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Close gossip after gossip option');

REPLACE INTO `item_script_names` (`Id`, `ScriptName`) VALUES 
(74749, 'item_charging_decorder_ring');