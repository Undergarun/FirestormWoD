DELETE FROM `creature` WHERE `id`=53488 AND `map`=643;
DELETE FROM `creature` WHERE `id`=41201 AND `map`=643;
DELETE FROM `creature` WHERE `id`=22515 AND `map`=643;
DELETE FROM `creature` WHERE `id`=40784 AND `map`=643;
DELETE FROM `creature` WHERE `id`=48983 AND `map`=643;
DELETE FROM `creature` WHERE `id`=45469 AND `map`=643;
DELETE FROM `creature` WHERE `id`=40861 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44658 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44715 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44648 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44752 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44841 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44801 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44834 AND `map`=643;
DELETE FROM `creature` WHERE `id`=44404 AND `map`=643;
DELETE FROM `creature` WHERE `id`=40633 AND `map`=643;
DELETE FROM `creature` WHERE `id`=48571 AND `map`=643;
DELETE FROM `creature` WHERE `id`=49108 AND `map`=643;
DELETE FROM `creature` WHERE `id`=40597 AND `map`=643;
DELETE FROM `creature` WHERE `id`=49108 AND `map`=643;
DELETE FROM `creature` WHERE `id`=49108 AND `map`=643;
DELETE FROM `creature` WHERE `id`=49108 AND `map`=643;
DELETE FROM `creature` WHERE `id`=40923 AND `map`=643;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=45469;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40861;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44658;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44715;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44648;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44752;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44841;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44801;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44834;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=44404;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40633;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=48571;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=49108;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40597;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40784;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=40765;

/*
DELETE FROM `creature` WHERE `guid`=331794 AND `id`=45620;
DELETE FROM `creature_addon` WHERE `guid`=331794;
DELETE FROM `creature` WHERE `guid`=332010 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=332010;
DELETE FROM `creature` WHERE `guid`=331795 AND `id`=45620;
DELETE FROM `creature_addon` WHERE `guid`=331795;
DELETE FROM `creature` WHERE `guid`=332021 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=332021;
DELETE FROM `creature` WHERE `guid`=332008 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=332008;
DELETE FROM `creature` WHERE `guid`=331999 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331999;
DELETE FROM `creature` WHERE `guid`=331993 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331993;
DELETE FROM `creature` WHERE `guid`=331899 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331899;
DELETE FROM `creature` WHERE `guid`=331878 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331878;
DELETE FROM `creature` WHERE `guid`=331877 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331877;
DELETE FROM `creature` WHERE `guid`=331875 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331873;
DELETE FROM `creature` WHERE `guid`=331873 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331873;
DELETE FROM `creature` WHERE `guid`=331867 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331867;
DELETE FROM `creature` WHERE `guid`=331897 AND `id`=45672;
DELETE FROM `creature_addon` WHERE `guid`=331897;
DELETE FROM `creature` WHERE `guid`=331858 AND `id`=40655;
DELETE FROM `creature_addon` WHERE `guid`=331858;
DELETE FROM `creature` WHERE `guid`=331891 AND `id`=41139;
DELETE FROM `creature_addon` WHERE `guid`=331891;
DELETE FROM `creature` WHERE `guid`=331890 AND `id`=41139;
DELETE FROM `creature_addon` WHERE `guid`=331890;
DELETE FROM `creature` WHERE `guid`=331723 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331723;
DELETE FROM `creature` WHERE `guid`=331718 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331718;
DELETE FROM `creature` WHERE `guid`=331717 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331717;
DELETE FROM `creature` WHERE `guid`=331714 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331714;
DELETE FROM `creature` WHERE `guid`=331715 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331715
DELETE FROM `creature` WHERE `guid`=331716 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331716;
DELETE FROM `creature` WHERE `guid`=331721 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331721;
DELETE FROM `creature` WHERE `guid`=331722 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331722;
DELETE FROM `creature` WHERE `guid`=331719 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331719;
DELETE FROM `creature` WHERE `guid`=331720 AND `id`=39960;
DELETE FROM `creature_addon` WHERE `guid`=331720;
*/
UPDATE `creature` SET `position_x`=191.59, `position_y`=802.23, `position_z`=807.64, `orientation`=3.19 WHERE `id`=40586;
UPDATE `creature` SET `position_x`=54.21, `position_y`=800.90, `position_z`=805.74, `orientation`=3.19 WHERE `id`=40765;


INSERT IGNORE INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (207997, 643, 3, 1, -315.556, 758.404, 275.665, -1.5708, 0, 0, -0.707107, 0.707107, 604800, 255, 1);
INSERT IGNORE INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (207998, 643, 3, 1, -319.797, 857.663, 271.239, 1.5708, 0, 0, 0.707107, 0.707107, 604800, 255, 1);
INSERT IGNORE INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (208302, 643, 3, 1, -309.753, 685.567, 286.198, 1.5708, 0, 0, 0.707107, 0.707107, 604800, 255, 1);
INSERT IGNORE INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES (208301, 643, 3, 1, -311.465, 932.898, 247.927, -1.5708, 0, 0, -0.707107, 0.707107, 604800, 255, 1);

UPDATE `creature_template` SET `npcflag`=16777217 WHERE `entry`=51395;
UPDATE `creature_template` SET `npcflag`=16777217 WHERE `entry`=51391;
UPDATE `creature_template` SET `AIName`='' WHERE `entry`=48571;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=48571;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=40597;
UPDATE `creature_template` SET `unit_class`=8 WHERE `entry`=44404;
UPDATE `creature_template` SET `unit_class`=8 WHERE `entry`=49093;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `Inhabittype`=7, `flags_extra`=128 WHERE `entry`=40784;
UPDATE `creature_template` SET `unit_class`=8 WHERE `entry`=40825;
UPDATE `creature_template` SET `unit_class`=8 WHERE `entry`=49072;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=45469;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=40861;
UPDATE `creature` SET `position_x`=-141.449, `position_y`=635.26 WHERE `id`=40788 AND `map`=643;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry`=45469;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry`=40861;
UPDATE `creature_template` SET `unit_class`=8 WHERE `entry`=44715;
UPDATE `creature_template` SET `unit_class`=8 WHERE `entry`=49107;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `flags_extra`=128 WHERE `entry`=44801;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=40765;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0 WHERE `entry`=49064;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=204338;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=204339;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=204340;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=204341;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=208302;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=208301;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=207997;
UPDATE `gameobject_template` SET `flags`=4 WHERE entry=207998;

UPDATE `instance_template` SET `script`='instance_throne_of_the_tides' WHERE `map`=643;

UPDATE `gameobject_template` SET `scriptname`='go_totd_defense_system' WHERE `entry`=203199;
UPDATE `creature_template` SET `ScriptName`='npc_lady_nazjar_event' WHERE `entry`=39959;
UPDATE `creature_template` SET `ScriptName`='npc_throne_of_the_tides_teleporter' WHERE `entry`=51391;
UPDATE `creature_template` SET `ScriptName`='npc_throne_of_the_tides_teleporter' WHERE `entry`=51395;
DELETE FROM `areatrigger_scripts` WHERE `entry`=5834;
DELETE FROM `areatrigger_scripts` WHERE `entry`=5873;
DELETE FROM `areatrigger_scripts` WHERE `entry`=6646;
INSERT INTO `areatrigger_scripts` VALUES (5834, 'at_tott_lady_nazjar_event');
INSERT INTO `areatrigger_scripts` VALUES (5873, 'at_tott_commander_ulthok');
INSERT INTO `areatrigger_scripts` VALUES (6646, 'at_tott_ozumat');

UPDATE `creature_template` SET `ScriptName`='boss_lady_nazjar' WHERE `entry`=40586;
UPDATE `creature_template` SET `ScriptName`='npc_lady_nazjar_honnor_guard' WHERE `entry`=40633;
UPDATE `creature_template` SET `ScriptName`='npc_lady_nazjar_tempest_witch' WHERE `entry`=44404;
UPDATE `creature_template` SET `ScriptName`='npc_lady_nazjar_waterspout' WHERE `entry`=48571;
UPDATE `creature_template` SET `ScriptName`='npc_lady_nazjar_waterspout' WHERE `entry`=49108;
UPDATE `creature_template` SET `ScriptName`='npc_lady_nazjar_geyser' WHERE `entry`=40597;

UPDATE `creature_template` SET `ScriptName`='boss_commander_ulthok' WHERE `entry`=40765;
UPDATE `creature_template` SET `ScriptName`='npc_ulthok_dark_fissure' WHERE `entry`=40784;

UPDATE `creature_template` SET `ScriptName`='boss_mindbender_ghursha' WHERE `entry`=40788;
UPDATE `creature_template` SET `ScriptName`='boss_erunak_stonespeaker' WHERE `entry`=40825;
UPDATE `creature_template` SET `ScriptName`='npc_erunak_earth_shards' WHERE `entry`=45469;
UPDATE `creature_template` SET `ScriptName`='npc_ghursha_mind_fog' WHERE `entry`=40861;

UPDATE `creature_template` SET `ScriptName`='npc_neptulon' WHERE `entry`=40792;
UPDATE `creature_template` SET `ScriptName`='npc_vicious_mindslasher' WHERE `entry`=44715;
UPDATE `creature_template` SET `ScriptName`='npc_unyielding_behemoth' WHERE `entry`=44648;
UPDATE `creature_template` SET `ScriptName`='npc_faceless_sapper' WHERE `entry`=44752;
UPDATE `creature_template` SET `ScriptName`='npc_blight_of_ozumat' WHERE `entry`=44801;

DELETE FROM `conditions` WHERE `SourceEntry`=83463;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 83463, 31, 3, 40792, 'Ozumat - Entangling Grasp');

DELETE FROM `creature_text` WHERE `entry`=39959;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39959, 0, 0, 'Armies of the depths, wash over our enemies as a tide of death!', 14, 0, 0, 0, 0, 18890, 'VO_TT_Nazjar_Event05');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39959, 1, 0, 'Meddlesome gnats, you\'ve thought us defeated so easily.', 14, 0, 0, 0, 0, 18891, 'VO_TT_Nazjar_Event06');

DELETE FROM `locales_creature_text` WHERE `entry`=39959;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(39959, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Армии глубин, обрушьтесь на наших врагов волнами смерти!'),
(39959, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Жалкий планктон! Вы думаете, нас так просто победить?');

DELETE FROM `creature_text` WHERE `entry`=40586;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40586, 0, 0, 'You have interfered with our plans for the last time, mortals!', 14, 0, 0, 0, 0, 18886, 'VO_TT_Nazjar_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40586, 1, 0, 'Depths take you!', 14, 0, 0, 0, 0, 18887, 'VO_TT_Nazjar_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40586, 1, 1, 'The abyss awaits!', 14, 0, 0, 0, 0, 18888, 'VO_TT_Nazjar_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40586, 2, 0, 'Ulthok... stop them...', 14, 0, 0, 0, 0, 18889, 'VO_TT_Nazjar_Event04');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40586, 3, 0, 'Take arms, minions! Rise from the icy depths!', 14, 0, 0, 0, 0, 18892, 'VO_TT_Nazjar_Event07');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40586, 4, 0, 'Destroy these intruders! Leave them for the great dark beyond!', 14, 0, 0, 0, 0, 18893, 'VO_TT_Nazjar_Event08');

DELETE FROM `locales_creature_text` WHERE `entry`=40586;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(40586, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'В последний раз вы помешали нашим планам, смертные!'),
(40586, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вас поглотят глубины!'),
(40586, 1, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Бездна ждет!'),
(40586, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Улток, останови их...'),
(40586, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'К оружию, слуги! Восстаньте из ледяных глубин!'),
(40586, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Уничтожьте наглецов! Отправьте их в бездонные глубины!');

DELETE FROM `creature_text` WHERE `entry`=40825;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40825, 0, 0, 'I thank you, my friends. Forgive me. I... I have grown too weak to be of any assistance.', 14, 0, 0, 0, 0, 18662, 'VO_TT_Erunak_Freed');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40825, 1, 0, 'I am... weary. You must carry on without me. I shall take my leave once my strength has returned.', 14, 0, 0, 0, 0, 18664, 'VO_TT_Erunak_PostFight');

DELETE FROM `locales_creature_text` WHERE `entry`=40825;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(40825, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Благодарю вас, друзья. Простите меня. Я... Слишком ослаб и не смогу помочь вам.'),
(40825, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я изнурен... Продолжайте без меня. Я присоединюсь к вам, как только восстановлю силы.');

DELETE FROM `creature_text` WHERE `entry`=40788;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 0, 0, 'A new host must be found.', 14, 0, 0, 0, 0, 18860, 'VO_TT_Mindbender_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 1, 0, 'Too weak to serve!', 14, 0, 0, 0, 0, 18861, 'VO_TT_Mindbender_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 1, 1, 'Only your memory remains!', 14, 0, 0, 0, 0, 18862, 'VO_TT_Mindbender_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 2, 0, 'They are outside the cycle...', 14, 0, 0, 0, 0, 18863, 'VO_TT_Mindbender_Event04');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 3, 0, 'There is only Ghur\'sha.', 14, 0, 0, 0, 0, 18864, 'VO_TT_Mindbender_Event05');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 3, 1, 'Give in.', 14, 0, 0, 0, 0, 18865, 'VO_TT_Mindbender_Event06');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 3, 2, 'Who are your allies?', 14, 0, 0, 0, 0, 18866, 'VO_TT_Mindbender_Event07');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40788, 4, 0, 'Is. This. Reality.', 14, 0, 0, 0, 0, 18867, 'VO_TT_Mindbender_Event08');

DELETE FROM `locales_creature_text` WHERE `entry`=40788;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(40788, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Надо найти нового слугу.'),
(40788, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'У тебя не осталось сил, чтобы служить.'),
(40788, 1, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'О тебе останутся лишь воспоминания...'),
(40788, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Они вышли из цикла...'),
(40788, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Есть только Гур\'ша.'),
(40788, 3, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Подчинись…'),
(40788, 3, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Кто твои союзники?'),
(40788, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Неужели. Это. Случилось.');

DELETE FROM `creature_text` WHERE `entry`=40792;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 0, 0, 'Ahh - my fearless assistants, I assume.', 14, 0, 0, 0, 0, 19469, 'VO_TT_Neptulon_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 1, 0, 'You may yet regret your presence here.', 14, 0, 0, 0, 0, 19470, 'VO_TT_Neptulon_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 2, 0, 'As I purify these waters, the servants of filth will surely be stirred. Beware!', 14, 0, 0, 0, 0, 19471, 'VO_TT_Neptulon_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 3, 0, 'Augh... Their filthiness stains me!', 14, 0, 0, 0, 0, 19472, 'VO_TT_Neptulon_Fight01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 4, 0, 'I grow weary!', 14, 0, 0, 0, 0, 19473, 'VO_TT_Neptulon_Fight02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 5, 0, 'Fools! Make yourselves useful!', 14, 0, 0, 0, 0, 19474, 'VO_TT_Neptulon_Fight03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 6, 0, 'Patience guests. The waters are nearly cleansed.', 14, 0, 0, 0, 0, 19475, 'VO_TT_Neptulon_Fight04');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 7, 0, 'The beast has returned! It must not pollute my waters!', 14, 0, 0, 0, 0, 19476, 'VO_TT_Neptulon_Fight05');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 8, 0, 'Behold the power of pure water!', 14, 0, 0, 0, 0, 19477, 'VO_TT_Neptulon_Fight06');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 9, 0, 'My waters are cleansed! Drink in their power!', 14, 0, 0, 0, 0, 19478, 'VO_TT_Neptulon_Fight07');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 10, 0, 'Your kind... cannot be... trusted...', 14, 0, 0, 0, 0, 19479, 'VO_TT_Neptulon_Fight08');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 11, 0, 'You are unworthy of my domain!', 14, 0, 0, 0, 0, 19480, 'VO_TT_Neptulon_Fight09');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 11, 1, 'My attention was wasted on you!', 14, 0, 0, 0, 0, 19481, 'VO_TT_Neptulon_Fight10');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 12, 0, '', 14, 0, 0, 0, 0, 19482, 'VO_TT_Neptulon_Fight11');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40792, 13, 0, '', 14, 0, 0, 0, 0, 19483, 'VO_TT_Neptulon_Fight12');

DELETE FROM `locales_creature_text` WHERE `entry`=40792;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(40792, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'А, очевидно, это мои бесстрашные помощники.'),
(40792, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Но вы можете пожалеть, что пришли сюда.'),
(40792, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Когда я очищу эти воды, пробудятся прислужники скверны. Берегитесь!'),
(40792, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'А-а! Их энергия оскверняет меня!'),
(40792, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Терпение, гости мои. Воды почти очищены...'),
(40792, 7, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Чудовище вернулось! Оно не должно осквернить мои воды!'),
(40792, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Узрите силу чистой воды!'),
(40792, 9, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мои воды очищены! Они придадут вам сил!');





