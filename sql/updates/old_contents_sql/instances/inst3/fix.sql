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

DELETE FROM `creature_text` WHERE `entry`=40825;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40825, 0, 0, 'I thank you, my friends. Forgive me. I... I have grown too weak to be of any assistance.', 14, 0, 0, 0, 0, 18662, 'VO_TT_Erunak_Freed');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (40825, 1, 0, 'I am... weary. You must carry on without me. I shall take my leave once my strength has returned.', 14, 0, 0, 0, 0, 18664, 'VO_TT_Erunak_PostFight');

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

DELETE FROM `creature` WHERE `id` IN (49576, 44864) AND `map`=643;
UPDATE `gameobject_template` SET `faction`=0, `flags`=0 WHERE `entry`=205216;
UPDATE `gameobject_template` SET `faction`=0, `flags`=0 WHERE `entry`=207973;

DELETE FROM `creature` WHERE `map`=940 AND `id` IN
(54600, 54995, 55415, 55549, 
54644, 54735, 55445,
55377, 55427, 55441, 55467,
55468, 55469);

UPDATE `creature` SET `spawnmask`=0 WHERE `map`=940 AND `id` IN 
(55779, 54972, 54634, 54971, 
55636, 55911, 55969, 54696, 
55913, 55967, 58233, 54646);

UPDATE `creature` SET `position_x`=4251.114258, `position_y`=599.126465, `position_z`=-5.82, `orientation`=5.70 WHERE `id`=54968 AND `map`=940;

UPDATE `creature` SET `phasemask`=2 WHERE `map`=940 AND `id`=55466;

UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry` IN 
(54599, 54522, 55377, 55467, 55466);

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry` IN 
(54599, 54522, 54560, 54598, 
55384, 55385, 55389, 57197, 
54628, 55377, 55427, 55441, 
55467, 55468, 55469);

UPDATE `creature_template` SET `npcflag`=16777217 WHERE `entry`=55549;
UPDATE `creature_template` SET `scale`=1 WHERE `entry`=54628;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=54686;
UPDATE `creature_template` SET `spell1`=0 WHERE `entry` IN (55377, 55467);
UPDATE `creature_template` SET `scale`=1 WHERE `entry` IN (55377, 55467);

UPDATE `access_requirement` SET `level_min`=85 WHERE `mapId`=940 AND `difficulty`=1;

UPDATE `instance_template` SET `script`='instance_hour_of_twilight' WHERE `map`=940;

UPDATE `creature_template` SET `ScriptName`='boss_arcurion' WHERE `entry`=54590;
UPDATE `creature_template` SET `ScriptName`='npc_arcurion_frozen_servitor' WHERE `entry`=54600;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (102480);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102480, 'spell_arcurion_icy_boulder');

UPDATE `creature_template` SET `ScriptName`='boss_asira_dawnslayer' WHERE `entry`=54968;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (103419, 103562, 103597, 102726);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(103419, 'spell_asira_dawnslayer_blade_barrier'),
(103562, 'spell_asira_dawnslayer_blade_barrier'),
(103597, 'spell_asira_dawnslayer_throw_knife'),
(102726, 'spell_asira_dawnslayer_mark_of_silence');

UPDATE `creature_template` SET `ScriptName`='npc_hour_of_twilight_life_warden' WHERE `entry`=55549;

UPDATE `creature_template` SET `ScriptName`='boss_archbishop_benedictus' WHERE `entry`=54938;
UPDATE `creature_template` SET `ScriptName`='npc_archbishop_benedictus_purifying_light' WHERE `entry` IN (55377, 55468);
UPDATE `creature_template` SET `AIName`='PassiveAI', `ScriptName`='' WHERE `entry`=55466;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (103600, 103768, 103149, 103362);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(103600, 'spell_archbishop_benedictus_purifying_light_targeting'),
(103768, 'spell_archbishop_benedictus_purifying_light_targeting'),
(103149, 'spell_archbishop_benedictus_righteous_snear_aoe'),
(103362, 'spell_archbishop_benedictus_righteous_snear_aoe');

DELETE FROM `conditions` WHERE `SourceEntry` IN (103762);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 103762, 0, 31, 0, 3, 54971, 'Archbishop Benedictus - Engulfing Twilight');

DELETE FROM `creature_text` WHERE `entry`=54590;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54590, 0, 0, 'Give up the Dragon Soul, and I may yet allow you to live.', 14, 0, 100, 0, 0, 25796, 'arcurion aggro'),
(54590, 1, 0, 'Nothing! Nothing...', 14, 0, 100, 0, 0, 25797, 'arcurion death'),
(54590, 2, 0, 'Shaman! The Dragon Soul is not yours. Give it up, and you may yet walk away with your life.', 14, 0, 100, 0, 0, 25798, 'arcurion event 1'),
(54590, 3, 0, 'Destroy them all, but bring the Shaman to me!', 14, 0, 100, 0, 0, 25799, 'arcurion event 2'),
(54590, 4, 0, 'You will go nowhere, Shaman.', 14, 0, 100, 0, 0, 25800, 'arcurion event 3'),
(54590, 5, 0, 'The Hour of Twilight falls - the end of all things - you cannot stop it. You are nothing. NOTHING!', 14, 0, 100, 0, 0, 25801, 'arcurion event 4'),
(54590, 6, 0, 'You are a fool if you think to take your place as the Aspect of Earth, Shaman!', 14, 0, 100, 0, 0, 25802, 'arcurion intro 1'),
(54590, 7, 0, 'Mere mortals...', 14, 0, 100, 0, 0, 25803, 'arcurion kill 1'),
(54590, 8, 0, 'You\'re a mere mortal. It is time you die like one.', 14, 0, 100, 0, 0, 25804, 'arcurion intro 2'),
(54590, 7, 1, 'Your shaman can\'t protect you.', 14, 0, 100, 0, 0, 25805, 'arcurion kill 2'),
(54590, 7, 2, 'The Aspects misplaced their trust.', 14, 0, 100, 0, 0, 25806, 'arcurion kill 3'),
(54590, 9, 0, 'Enough, shaman!', 14, 0, 100, 0, 0, 25807, 'arcurion spell 1'),
(54590, 9, 1, 'None will survive!', 14, 0, 100, 0, 0, 25808, 'arcurion spell 2'),
(54590, 9, 2, 'The Shaman is mine, focus on his companions!', 14, 0, 100, 0, 0, 25809, 'arcurion spell 3'),
(54590, 9, 3, 'Freeze!', 14, 0, 100, 0, 0, 25810, 'arcurion spell 4');

DELETE FROM `creature_text` WHERE `entry`=54968;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54968, 0, 0, 'Let\'s get to work, shall we?', 14, 0, 100, 0, 0, 25816, 'asira aggro'),
(54968, 1, 0, 'You\'re much... better... than I thought...', 14, 0, 100, 0, 0, 25817, 'asira death'),
(54968, 2, 0, 'Where do you think you\'re going, little lizard?', 14, 0, 100, 0, 0, 25818, 'asira event 01'),
(54968, 3, 0, '...and with that out of the way, you and your flock of fumbling friends are next on my list. Mmm, I thought you\'d never get here!', 14, 0, 100, 0, 0, 25819, 'asira event 02'),
(54968, 4, 0, 'Well, that was even easier than I thought.', 14, 0, 100, 0, 0, 25820, 'asira event 03'),
(54968, 5, 0, 'So soon.', 14, 0, 100, 0, 0, 25821, 'asira slay 01'),
(54968, 5, 1, 'I hope your friends can do better!', 14, 0, 100, 0, 0, 25822, 'asira slay 02'),
(54968, 5, 2, 'Too much fun!', 14, 0, 100, 0, 0, 25823, 'asira slay 03'),
(54968, 5, 3, 'Good night.', 14, 0, 100, 0, 0, 25824, 'asira slay 04'),
(54968, 6, 0, 'Bye-bye.', 14, 0, 100, 0, 0, 25825, 'asira spell 01'),
(54968, 6, 1, 'Now you see me...', 14, 0, 100, 0, 0, 25826, 'asira spell 02'),
(54968, 6, 2, 'Surprise.', 14, 0, 100, 0, 0, 25827, 'asira spell 03');

DELETE FROM `creature_text` WHERE `entry`=54938;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54938, 0, 0, 'I looked into the eyes of The Dragon, and despaired...', 14, 0, 100, 0, 0, 25865, 'benedictus death'),
(54938, 1, 0, 'Get inside! Quickly! I\'ll hold them off.', 12, 0, 100, 0, 0, 25866, 'benedictus event 01'),
(54938, 2, 0, 'And now, Shaman, you will give the Dragon Soul to me.', 12, 0, 100, 0, 0, 25867, 'benedictus event 02'),
(54938, 3, 0, 'The light will consume you!', 14, 0, 100, 0, 0, 25868, 'benedictus spell 01'),
(54938, 4, 0, 'Drown in Shadow!', 14, 0, 100, 0, 0, 25869, 'benedictus spell 02');
