INSERT INTO spell_script_names VALUE (129356, 'spell_sha_of_anger_overcome_by_anger');
INSERT INTO spell_script_names VALUES
(108557, 'spell_monk_power_strikes'),
(115698, 'spell_monk_power_strikes'),
(115687, 'spell_monk_power_strikes'),
(115693, 'spell_monk_power_strikes'),
(115695, 'spell_monk_power_strikes');
DELETE FROM spell_area WHERE spell = 75207;
DELETE FROM spell_bonus_data WHERE entry IN (124464, 124465, 124467, 124468);
INSERT INTO spell_bonus_data VALUES
(124464, 0, 0, 0, 0, 'Priest - Mastery: Shadowy Recall (Shadow Word: Pain)'),
(124465, 0, 0, 0, 0, 'Priest - Mastery: Shadowy Recall (Vampiric Touch)'),
(124467, 0, 0, 0, 0, 'Priest - Mastery: Shadowy Recall (Devouring Plague)'),
(124468, 0, 0, 0, 0, 'Priest - Mastery: Shadowy Recall (Mind Flay)');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_dk_pestilence';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_dk_blood_strike';
INSERT INTO spell_script_names VALUE (31884, 'spell_pal_sanctified_wrath');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_mage_slow';
INSERT INTO spell_script_names VALUE (108499, 'spell_warl_grimoire_of_supremacy');
INSERT INTO trinity_string VALUE (11013,'You have sent too many mails in a short time interval.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'');
INSERT INTO spell_linked_spell VALUES (2812, -115654, 0, 'Glyph of Denounce');
-- Selfless Healer
DELETE FROM spell_linked_spell WHERE spell_trigger=19750;
INSERT INTO spell_linked_spell VALUES ('19750', '-114250', '0', 'Selfless Healer');
DELETE FROM spell_proc_event WHERE entry=85804;
INSERT INTO `spell_proc_event` VALUES (114250, 0, 10, 0, 0, 16384, 16384, 0, 0, 100, 0);
UPDATE `creature_template` SET `difficulty_entry_1`=0, `difficulty_entry_2`=0, `difficulty_entry_3`=0, `exp`=3 WHERE `entry` IN 
(12050,
 13326,
 13331,
 13422,
 13358,
 11949,
 11948,
 12053,
 13328,
 13332,
 13421,
 13359,
 11947,
 11946,
 14763,
 14762,
 14764,
 14765,
 14773,
 14776,
 14772,
 14777,
 10987,
 11600,
 11602,
 11657,
13396,
13080,
13098,
13078,
13397,
13099,
13081,
13079,
 11603,
 11604,
 11605,
 11677,
 10982,
13317,
13096,
13087,
13086,
13316,
13097,
13089,
13088,
14848,
 2225, 
 3343, 
 3625, 
 4255, 
 4257, 
 5134, 
 5135, 
 5139, 
 10364, 
 10367, 
 10981,  
 10986, 
 10990,  
 11675, 
 11678, 
 11839, 
 11947, 
 11948, 
 11949, 
 11997, 
 12051, 
 12096, 
 12097, 
 12127, 
 13176, 
 13179,  
 13216, 
 13218, 
 13236, 
 13257,  
 13284, 
 13438,  
 13442, 
 13443, 
 13447,  
 13577, 
 13617, 
 13797, 
 13798, 
 13816,  
 14185, 
 14186, 
 14187, 
 14188, 
 14282, 
 14283, 
 14284, 
 11946, 
 11948, 
 11947, 
 11949);

DELETE FROM `disables` WHERE `sourceType`=3 AND `entry`=9;

DELETE FROM `gameobject` WHERE `map`=628;

UPDATE `creature_template_addon` SET `emote`=0 WHERE `entry` IN (31681, 29941);
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=17591;
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry`=17641;

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=191305;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=191306;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=191307;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=191308;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=191309;
UPDATE `gameobject_template` SET `faction`=1801, `flags`=0 WHERE `entry`=191310;
UPDATE `gameobject_template` SET `faction`=1802 WHERE `entry`=191309;
UPDATE `gameobject_template` SET `faction`=1801 WHERE `entry`=191308;
UPDATE `gameobject_template` SET `faction`=1802 WHERE `entry`=191307;
UPDATE `gameobject_template` SET `faction`=1801 WHERE `entry`=191306;
UPDATE `gameobject_template` SET `faction`=1802 WHERE `entry`=191305;
UPDATE `gameobject_template` SET `faction`=0, `flags`=0 WHERE `entry`=192834;
UPDATE `gameobject_template` SET `ScriptName` = 'go_seaforium_charge' WHERE `entry` = 190753;

UPDATE `creature_template` SET `faction_a`=35, `faction_h`=35 WHERE `entry` IN (34924, 34922, 34918, 34919);

UPDATE `creature_template` SET `unit_flags`=`unit_flags`|33554432, `dynamicflags`=8 WHERE `entry` IN (34935, 34929, 35410, 35427);

UPDATE `creature_classlevelstats` SET `basehp1`=`basehp0` WHERE (`level`>=1 AND `level` <=57) AND `basehp1`=1;
UPDATE `creature_classlevelstats` SET `basehp2`=`basehp0` WHERE (`level`>=1 AND `level` <=57) AND `basehp2` = 1;
UPDATE `creature_classlevelstats` SET `basehp3`=`basehp0` WHERE (`level`>=1 AND `level` <=57) AND `basehp3` = 1;

UPDATE `creature_classlevelstats` SET `basehp2`=`basehp1` WHERE (`level`>=58 AND `level` <=67) AND `basehp2` = 1;
UPDATE `creature_classlevelstats` SET `basehp3`=`basehp1` WHERE (`level`>=58 AND `level` <=67) AND `basehp3` = 1;

UPDATE `creature_classlevelstats` SET `basehp3`=`basehp2` WHERE (`level`>=68 AND `level` <=79) AND `basehp3` = 1;

UPDATE `creature_template` SET `spell1`=96212 WHERE `entry`=27894;
UPDATE `creature_template` SET `spell1`=96212 WHERE `entry`=32795;

UPDATE `creature_template` SET `spell1`=96212 WHERE `entry`=27894;
UPDATE `creature_template` SET `spell1`=96212 WHERE `entry`=32795;

DELETE FROM `disables` WHERE `sourceType`=0 AND `entry`=86425;
DELETE FROM `conditions` WHERE `SourceEntry`=86425;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(17, 0, 86425, 0, 0, 31, 1, 3, 46393, 0, 0, 0, '', 'Billy Goat Blaster - Billy Goat Blast');

REPLACE INTO `spell_bonus_data` (`entry`, `direct_bonus`, `dot_bonus`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES 
(18222, 0, 0, 0, 0, 'Poached Sunscale Salmon - Health Regeneration');

UPDATE `creature_template` SET `mechanic_immune_mask`=344275770 WHERE `entry` IN (28781, 32796);

UPDATE creature_template SET ScriptName='boss_isle_of_conquest' WHERE entry IN (34922, 34924);
UPDATE creature_template SET ScriptName='npc_four_car_garage' WHERE entry IN (34775, 35069, 35273, 34793, 34776, 34802);

DELETE FROM `creature` WHERE `id` IN (23472, 22515) AND `map`=628;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(23472, 628, 15, 1, 0, 0, 1143.25, -779.599, 48.629, 1.64061, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(23472, 628, 15, 1, 0, 0, 1236.53, -669.415, 48.2729, 0.104719, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 628, 15, 1, 0, 0, 1233.27, -844.526, 48.8824, -0.0174525, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(23472, 628, 15, 1, 0, 0, 311.92, -913.972, 48.8159, 3.08918, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 628, 15, 1, 0, 0, 1235.53, -683.872, 49.304, -3.08918, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 628, 15, 1, 0, 0, 397.089, -859.382, 48.8993, 1.64061, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(23472, 628, 15, 1, 0, 0, 324.635, -749.128, 49.3602, 0.0174525, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(23472, 628, 15, 1, 0, 0, 425.675, -857.09, 48.5104, -1.6057, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 628, 15, 1, 0, 0, 323.54, -888.361, 48.9197, 0.0349063, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 628, 15, 1, 0, 0, 326.285, -777.366, 49.0208, 3.12412, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(23472, 628, 15, 1, 0, 0, 1235.09, -857.898, 48.9163, 3.07177, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 628, 15, 1, 0, 0, 1158.76, -746.182, 48.6277, -1.51844, 120, 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `gameobject_template` SET `data16`=1 WHERE `entry`=180418;
DELETE FROM `gameobject` WHERE `id`=180322 AND `map`=30;

UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=184142;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=184141;

UPDATE `gameobject_template` SET `faction`=0, `flags`=0 WHERE `entry` IN (195235, 195331, 190752);

DELETE FROM `gameobject` WHERE `map` IN (529, 566) AND `id` IN (179871, 179904, 179905);
-- Wild Mushroom : Bloom, fix coefficient (for us it is 20,1% idk why. Must be 62,1% of SPD, at the moment on official server it is 124,2% of SPD but it was increased to x2 in 5.3.0 patch, so 124,2/2=62,1%.)
UPDATE spell_bonus_data SET direct_bonus='0,621' WHERE entry=102792; 

-- Druid Starsurge, for patch 5.0.5 it is 213,3% of SPD.
UPDATE spell_bonus_data SET direct_bonus='2,133' WHERE entry=78674;

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('127266','spell_item_amber_prison');
UPDATE creature_template SET unit_flags=4 WHERE entry IN (59390,59398);

-- Starting quest class

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('100','spell_quests_spell_hit');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('122','spell_quests_spell_hit');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('172','spell_quests_spell_hit');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('589','spell_quests_spell_hit');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('2098','spell_quests_spell_hit');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('8921','spell_quests_spell_hit');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('56641','spell_quests_spell_hit');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES('73899','spell_quests_spell_hit');

-- Old hack fix !
UPDATE `quest_template` SET `RequiredNpcOrGo1` = 44175, `RequiredSpellCast1` = 0 WHERE `Id` = 26948;

-- 1 = Battle Elixir

-- esprit 

INSERT INTO `spell_group` VALUES (1, 8112);
INSERT INTO `spell_group` VALUES (1, 8113);
INSERT INTO `spell_group` VALUES (1, 8114);
INSERT INTO `spell_group` VALUES (1, 12177);
INSERT INTO `spell_group` VALUES (1, 89342);
INSERT INTO `spell_group` VALUES (1, 33080);
INSERT INTO `spell_group` VALUES (1, 43197);
INSERT INTO `spell_group` VALUES (1, 48103);
INSERT INTO `spell_group` VALUES (1, 48104);

-- agilité 

INSERT INTO `spell_group` VALUES (1, 8115);
INSERT INTO `spell_group` VALUES (1, 8116);
INSERT INTO `spell_group` VALUES (1, 8117);
INSERT INTO `spell_group` VALUES (1, 12174);
INSERT INTO `spell_group` VALUES (1, 33077);
INSERT INTO `spell_group` VALUES (1, 43194);
INSERT INTO `spell_group` VALUES (1, 58450);
INSERT INTO `spell_group` VALUES (1, 58451);
INSERT INTO `spell_group` VALUES (1, 89343);

-- intelligence

INSERT INTO `spell_group` VALUES (1, 8096);
INSERT INTO `spell_group` VALUES (1, 8097);
INSERT INTO `spell_group` VALUES (1, 8098);
INSERT INTO `spell_group` VALUES (1, 12176);
INSERT INTO `spell_group` VALUES (1, 33078);
INSERT INTO `spell_group` VALUES (1, 43195);
INSERT INTO `spell_group` VALUES (1, 48099);
INSERT INTO `spell_group` VALUES (1, 48100);
INSERT INTO `spell_group` VALUES (1, 89347);

-- force

INSERT INTO `spell_group` VALUES (1, 8118);
INSERT INTO `spell_group` VALUES (1, 8119);
INSERT INTO `spell_group` VALUES (1, 8120);
INSERT INTO `spell_group` VALUES (1, 12179);
INSERT INTO `spell_group` VALUES (1, 33082);
INSERT INTO `spell_group` VALUES (1, 43199);
INSERT INTO `spell_group` VALUES (1, 58448);
INSERT INTO `spell_group` VALUES (1, 58449);
INSERT INTO `spell_group` VALUES (1, 89346);

-- 2 = Guardian Elixir

-- endurance

INSERT INTO `spell_group` VALUES (2, 8099);
INSERT INTO `spell_group` VALUES (2, 8100);
INSERT INTO `spell_group` VALUES (2, 8101);
INSERT INTO `spell_group` VALUES (2, 12178);
INSERT INTO `spell_group` VALUES (2, 89345);
INSERT INTO `spell_group` VALUES (2, 33081);
INSERT INTO `spell_group` VALUES (2, 43198);
INSERT INTO `spell_group` VALUES (2, 48101);
INSERT INTO `spell_group` VALUES (2, 48102);

-- protection

INSERT INTO `spell_group` VALUES (2, 8091);
INSERT INTO `spell_group` VALUES (2, 8094);
INSERT INTO `spell_group` VALUES (2, 8095);
INSERT INTO `spell_group` VALUES (2, 12175);
INSERT INTO `spell_group` VALUES (2, 89344);
INSERT INTO `spell_group` VALUES (2, 33079);
INSERT INTO `spell_group` VALUES (2, 41396);
INSERT INTO `spell_group` VALUES (2, 58452);

INSERT INTO spell_script_names VALUE (57405, 'spell_gen_ds_flush_knockback');

DELETE FROM gameobject_template WHERE entry IN (208779, 208780, 208781);
INSERT INTO gameobject_template VALUES
(208779, 10, 6271, 'Lighthouse Banner', '', 'Capturing', '', 35, 32, 1, 0, 0, 0, 0, 0, 0, 1479, 0, 0, 3000, 0, 0, 0, 0, 0, 0, 23936, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(208780, 10, 6271, 'Waterworks Banner', '', 'Capturing', '', 35, 0, 1, 0, 0, 0, 0, 0, 0, 1479, 0, 0, 3000, 0, 0, 0, 0, 0, 0, 23936, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
(208781, 10, 6271, 'Mines Banner', '', 'Capturing', '', 35, 0, 1, 0, 0, 0, 0, 0, 0, 1479, 0, 0, 3000, 0, 0, 0, 0, 0, 0, 23936, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595);

UPDATE gameobject_template SET flags = 32 WHERE entry = 190753; -- Seaforium Bomb
UPDATE gameobject_template SET flags = 0 WHERE entry IN (191305, 191306, 191307, 191308, 191309, 191310); -- Graveyards Flags
UPDATE gameobject_template SET faction = 1802 WHERE entry IN (191305, 191307, 191309); -- Horde
UPDATE gameobject_template SET faction = 1801 WHERE entry IN (191306, 191308, 191310); -- Aliance
UPDATE creature_template SET unit_flags = 16793600, unit_flags2 = 2048, dynamicflags = 4, mechanic_immune_mask = 344275770 WHERE entry = 28781; -- Battleground Demolisher
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`)
VALUES('211584','10','8504','Console de commande antique','','','','0','0','3','0','0','0','0','0','0','0','0','0','3000','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','go_ancien_control_console','16048');
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES('19105','11','0','0','achievement_show_me_you_moves');

INSERT INTO spell_script_names VALUE (120764, 'spell_ghost_essence');