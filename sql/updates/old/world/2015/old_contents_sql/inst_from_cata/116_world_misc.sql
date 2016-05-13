/*
DELETE FROM `creature` WHERE `guid`=327611 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327611;
DELETE FROM `creature` WHERE `guid`=327610 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327610;
DELETE FROM `creature` WHERE `guid`=327663 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327663;
DELETE FROM `creature` WHERE `guid`=327665 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327665;
DELETE FROM `creature` WHERE `guid`=327666 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327666;
DELETE FROM `creature` WHERE `guid`=327661 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327661;
DELETE FROM `creature` WHERE `guid`=327664 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327664;
DELETE FROM `creature` WHERE `guid`=327662 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327662;
DELETE FROM `creature` WHERE `guid`=327522 AND `id`=39990;
DELETE FROM `creature_addon` WHERE `guid`=327522;
DELETE FROM `creature` WHERE `guid`=327518 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327518;
DELETE FROM `creature` WHERE `guid`=327711 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327711;
DELETE FROM `creature` WHERE `guid`=327712 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327712;
DELETE FROM `creature` WHERE `guid`=327710 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327710;
DELETE FROM `creature` WHERE `guid`=327708 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327708;
DELETE FROM `creature` WHERE `guid`=327709 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327709;
DELETE FROM `creature` WHERE `guid`=327698 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327698;
DELETE FROM `creature` WHERE `guid`=327699 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327699;
DELETE FROM `creature` WHERE `guid`=327707 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327707;
DELETE FROM `creature` WHERE `guid`=327697 AND `id`=39987;
DELETE FROM `creature_addon` WHERE `guid`=327697;
DELETE FROM `creature` WHERE `guid`=327654 AND `id`=40023;
DELETE FROM `creature_addon` WHERE `guid`=327654;
DELETE FROM `creature` WHERE `guid`=327622 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327622;
DELETE FROM `creature` WHERE `guid`=327621 AND `id`=40019;
DELETE FROM `creature_addon` WHERE `guid`=327621;
DELETE FROM `creature` WHERE `guid`=327621 AND `id`=40019;
DELETE FROM `creature_addon` WHERE `guid`=327621;
DELETE FROM `creature` WHERE `guid`=327620 AND `id`=40019;
DELETE FROM `creature_addon` WHERE `guid`=327620;
DELETE FROM `creature` WHERE `guid`=327624 AND `id`=40019;
DELETE FROM `creature_addon` WHERE `guid`=327624;
DELETE FROM `creature` WHERE `guid`=327623 AND `id`=40019;
DELETE FROM `creature_addon` WHERE `guid`=327623;
DELETE FROM `creature` WHERE `guid`=327600 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327600;
DELETE FROM `creature` WHERE `guid`=327597 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327597;
DELETE FROM `creature` WHERE `guid`=327598 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327598;
DELETE FROM `creature` WHERE `guid`=327599 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327599;
DELETE FROM `creature` WHERE `guid`=327660 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327660;
DELETE FROM `creature` WHERE `guid`=327573 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327573;
DELETE FROM `creature` WHERE `guid`=327656 AND `id`=40017;
DELETE FROM `creature_addon` WHERE `guid`=327656;
*/
DELETE FROM `creature` WHERE `id`=40401 AND `map`=645;
DELETE FROM `creature` WHERE `id`=40447 AND `map`=645;
DELETE FROM `creature` WHERE `id`=50376 AND `map`=645;
DELETE FROM `creature` WHERE `id`=53488 AND `map`=645;
DELETE FROM `creature` WHERE `id`=40164 AND `map`=645;
DELETE FROM `creature` WHERE `id`=50284 AND `map`=645;
DELETE FROM `creature` WHERE `id`=40817 AND `map`=645;
DELETE FROM `creature` WHERE `id`=50417 AND `map`=645;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40817;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=50284;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40164;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=50376;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40447;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40401;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=50417;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39698;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39699;
DELETE FROM `creature_addon` WHERE `guid`=327649;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39705;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39706;
-- DELETE FROM `creature_addon` WHERE `guid`=327593;

UPDATE `creature` SET `position_x`=214.59, `position_y`=950.40, `position_z`=191.10, `orientation`=3.17 WHERE `guid`=327588;

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=39990;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=39991;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=40019;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=40020;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=40017;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=40018;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=40008;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=40009;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=39705;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=39706;

UPDATE `instance_template` SET `script`='instance_blackrock_caverns' WHERE `map`=645;

UPDATE `creature_template` SET `ScriptName`='boss_romogg_bonecrusher' WHERE `entry`=39665;
UPDATE `creature_template` SET `ScriptName`='npc_chains_of_woe' WHERE `entry`=40447;

UPDATE `creature_template` SET `ScriptName`='boss_corla_herald_of_twilight' WHERE `entry`=39679;

UPDATE `creature_template` SET `ScriptName`='boss_karsh_steelbender' WHERE `entry`=39698;

UPDATE `creature_template` SET `ScriptName`='boss_beauty' WHERE `entry`=39700;

UPDATE `creature_template` SET `ScriptName`='boss_ascendant_lord_obsidius' WHERE `entry`=39705;
UPDATE `creature_template` SET `ScriptName`='npc_shadow_of_obsidius' WHERE `entry`=40817;

DELETE FROM `spell_script_names` where `spell_id`=75272;
DELETE FROM `spell_script_names` where `spell_id`=82189;
DELETE FROM `spell_script_names` where `spell_id`=75437;
DELETE FROM `spell_script_names` where `spell_id`=82137;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (75272, 'spell_romoogg_quake');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (82189, 'spell_romoogg_chains_of_woe');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (75437, 'spell_romoogg_chains_of_woe_tele');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (82137, 'spell_romoogg_call_for_help');

DELETE from `creature_text` where `entry` = 39665;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39665, 0, 0, 'Boss Cho\'gall not gonna be happy \'bout dis!', 1, 0, 0, 0, 0, 18925, 'VO_BC_Romogg_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39665, 1, 0, 'Dat\'s what you get! Noting!', 1, 0, 0, 0, 0, 18926, 'VO_BC_Romogg_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39665, 2, 0, 'Stand still! Rom\'ogg crack your skulls!', 1, 0, 0, 0, 0, 18927, 'VO_BC_Romogg_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39665, 3, 0, 'Rom\'ogg sorry...', 1, 0, 0, 0, 0, 18928, 'VO_BC_Romogg_Event04');

DELETE FROM `locales_creature_text` WHERE `entry`=39665;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES	
(39665, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Начальник Чо\'Галл будет недовольный!'),
(39665, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вот вы что получите! Ничего!'),
(39665, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Стойте смирно! Ром\'огг расколоть вам череп!'),
(39665, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ром\'оггу стыдно...');

DELETE from `creature_text` where `entry` = 39679;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 0, 0, 'HERETICS! You will suffer for this interruption!', 1, 0, 0, 0, 0, 18589, 'VO_BC_Corla_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 1, 0, 'There is only one true path of enlightenment! DEATH!', 1, 0, 0, 0, 0, 18590, 'VO_BC_Corla_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 2, 0, 'Bask in his power! Rise as an agent of the master\'s rage!', 1, 0, 0, 0, 0, 18592, 'VO_BC_Corla_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 3, 0, 'For the master I\'d die a thousand times... A thousan...', 1, 0, 0, 0, 0, 18594, 'VO_BC_Corla_Event04');

DELETE FROM `locales_creature_text` WHERE `entry`=39679;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(39679, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Еретики! Вы поплатитесь за ваше вторжение!'),
(39679, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Есть лишь один путь к просветлению! СМЕРТЬ!'),
(39679, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Купайся в его силе! Восстань как воплощение ярости господина!'),
(39679, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'За господина я умру и тысячу раз... Тыся…');

DELETE from `creature_text` where `entry` = 39698;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 0, 0, 'Bodies to test my armaments upon!', 1, 0, 0, 0, 0, 18852, 'VO_BC_Karsh_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 1, 0, 'Merely an impurity in the compound...', 1, 0, 0, 0, 0, 18853, 'VO_BC_Karsh_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 2, 0, 'Feel the burn!', 1, 0, 0, 0, 0, 18854, 'VO_BC_Karsh_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 3, 0, 'We number in the millions! Your efforts are wasted...', 1, 0, 0, 0, 0, 18855, 'VO_BC_Karsh_Event04');

DELETE FROM `locales_creature_text` WHERE `entry`=39698;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(39698, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вот на ком я испытаю свою броню!'),
(39698, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Просто сторонние примеси в сплаве...'),
(39698, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Сейчас вам станет жарко!'),
(39698, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нас миллионы! Вы зря стараетесь…');

DELETE from `creature_text` where `entry` = 39705;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 0, 0, 'You come seeking answers? Then have them! Look upon your answer to living!', 1, 0, 0, 0, 0, 18899, 'VO_BC_Obsidius_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 1, 0, 'Your kind has no place in the master\'s world.', 1, 0, 0, 0, 0, 18900, 'VO_BC_Obsidius_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 2, 0, 'Earth can be shaped, molded... You cannot! You are useless!', 1, 0, 0, 0, 0, 18901, 'VO_BC_Obsidius_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 3, 0, 'I cannot be destroyed... Only... de... layed...', 1, 0, 0, 0, 0, 18902, 'VO_BC_Obsidius_Event04');

DELETE FROM `locales_creature_text` WHERE `entry`=39705;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(39705, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы пришли за ответами? Так получите их! Вот разгадка тайны бытия!'),
(39705, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вашему роду не место в мире господина.'),
(39705, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Земля податлива, ей можно придать любую форму... А вам – нет! Вы бесполезны!'),
(39705, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы меня не убьете... Только... задержите…');