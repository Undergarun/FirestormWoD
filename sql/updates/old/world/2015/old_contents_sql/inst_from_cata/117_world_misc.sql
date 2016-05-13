/*
DELETE FROM `creature` WHERE `guid`=332167 AND `id`=45915;
DELETE FROM `creature_addon` WHERE `guid`=332167;
DELETE FROM `creature` WHERE `guid`=332204 AND `id`=45704;
DELETE FROM `creature_addon` WHERE `guid`=332204;
DELETE FROM `creature` WHERE `guid`=332208 AND `id`=45704;
DELETE FROM `creature_addon` WHERE `guid`=332208;
DELETE FROM `creature` WHERE `guid`=332209 AND `id`=45704;
DELETE FROM `creature_addon` WHERE `guid`=332209;
DELETE FROM `creature` WHERE `guid`=332205 AND `id`=45704;
DELETE FROM `creature_addon` WHERE `guid`=332205;
DELETE FROM `creature` WHERE `guid`=332207 AND `id`=45704;
DELETE FROM `creature_addon` WHERE `guid`=332207;
DELETE FROM `creature` WHERE `guid`=332206 AND `id`=45704;
DELETE FROM `creature_addon` WHERE `guid`=332206;
DELETE FROM `creature` WHERE `guid`=332087 AND `id`=45924;
DELETE FROM `creature_addon` WHERE `guid`=332087;
DELETE FROM `creature` WHERE `guid`=332085 AND `id`=45924;
DELETE FROM `creature_addon` WHERE `guid`=332085;
DELETE FROM `creature` WHERE `guid`=332088 AND `id`=45924;
DELETE FROM `creature_addon` WHERE `guid`=332088;
DELETE FROM `creature` WHERE `guid`=332086 AND `id`=45924;
DELETE FROM `creature_addon` WHERE `guid`=332086;
DELETE FROM `creature` WHERE `guid`=332124 AND `id`=45981;
DELETE FROM `creature_addon` WHERE `guid`=332124;
*/
DELETE FROM `creature` WHERE `id`=46007 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=46007;
DELETE FROM `creature` WHERE `id`=46492 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=46492;
DELETE FROM `creature` WHERE `id`=52019 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=52019;
DELETE FROM `creature` WHERE `id`=46387 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=46387;
DELETE FROM `creature` WHERE `id`=47000 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=47000;
DELETE FROM `creature` WHERE `id`=47342 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=47342;
DELETE FROM `creature` WHERE `id`=47305 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=47305;
DELETE FROM `creature` WHERE `id`=45455 AND `map`=657;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=45455;
DELETE FROM `creature` WHERE `id`=53488 AND `map`=657;
DELETE FROM `creature` WHERE `id`=42844 AND `map`=657;
DELETE FROM `creature` WHERE `id`=47085 AND `map`=657;
DELETE FROM `creature` WHERE `id`=45981 AND `map`=657;

INSERT IGNORE INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`) VALUES (45455, 657, 3, 1, 0, 0, -1198.79, 107.046, 740.707, 6.0055, 300, 0, 0, 64496, 0);
INSERT IGNORE INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`) VALUES (45455, 657, 3, 1, 0, 0, -765.44, -41.37, 639.93, 4.16842, 604800, 0, 0, 42, 0);

UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0, `InhabitType`=1 WHERE `entry`=43873;
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0, `InhabitType`=1 WHERE `entry`=43874;

UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=46007;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=47305;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=46492;

UPDATE `instance_template` SET `script`='instance_the_vortex_pinnacle' WHERE `map`=657;

UPDATE `creature_template` SET `ScriptName`='npc_vortex_pinnacle_slipsteam' WHERE `entry`=45455;
UPDATE `creature_template` SET `ScriptName`='npc_skyfall_star' WHERE `entry`=45932;
UPDATE `creature_template` SET `ScriptName`='npc_skyfall_star' WHERE `entry`=52019;
UPDATE `creature_template` SET `ScriptName`='npc_cloud_prince' WHERE `entry`=45917;
UPDATE `creature_template` SET `ScriptName`='npc_whipping_wind' WHERE `entry`=47238;
UPDATE `creature_template` SET `ScriptName`='npc_young_storm_dragon' WHERE `entry`=45919;
UPDATE `creature_template` SET `ScriptName`='npc_armored_mistral' WHERE `entry`=45915;
UPDATE `creature_template` SET `ScriptName`='npc_empyrean_assassin' WHERE `entry`=45922;
UPDATE `creature_template` SET `ScriptName`='npc_executor_of_the_caliph' WHERE `entry`=45928;
UPDATE `creature_template` SET `ScriptName`='npc_gust_soldier' WHERE `entry`=45477;
UPDATE `creature_template` SET `ScriptName`='npc_howling_gale' WHERE `entry`=45572;
UPDATE `creature_template` SET `ScriptName`='npc_lurking_tempest' WHERE `entry`=45704;
UPDATE `creature_template` SET `ScriptName`='npc_minister_of_air' WHERE `entry`=45930;
UPDATE `creature_template` SET `ScriptName`='npc_servant_of_asaad' WHERE `entry`=45926;
UPDATE `creature_template` SET `ScriptName`='npc_temple_adept' WHERE `entry`=45935;
UPDATE `creature_template` SET `ScriptName`='npc_turbulent_squall' WHERE `entry`=45924;
UPDATE `creature_template` SET `ScriptName`='npc_wild_vortex' WHERE `entry`=45912;

UPDATE `creature_template` SET `ScriptName`='boss_grand_vizier_ertan' WHERE `entry`=43878;
UPDATE `creature_template` SET `ScriptName`='npc_ertan_vortex' WHERE `entry`=46007;

UPDATE `creature_template` SET `ScriptName`='boss_altairus' WHERE `entry`=43873;
UPDATE `creature_template` SET `ScriptName`='npc_air_current' WHERE `entry`=47305;

UPDATE `creature_template` SET `ScriptName`='boss_asaad' WHERE `entry`=43875;
UPDATE `creature_template` SET `ScriptName`='npc_unstable_grounding_field' WHERE `entry`=46492;

DELETE FROM `spell_script_names` WHERE `spell_id`=87765;
DELETE FROM `spell_script_names` WHERE `spell_id`=85084;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (87765, 'spell_minister_of_air_lightning_lash');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (85084, '85084spell_howling_gale_howling_gale');

DELETE FROM `conditions` WHERE `SourceEntry`=86911;
DELETE FROM `conditions` WHERE `SourceEntry`=87517;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`)
VALUES (13, 1, 86911, 31, 3, 46492, 'Asaad - Grounding field visual beams');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) 
VALUES (13, 1, 87517, 31, 3, 46492, 'Asaad - Grounding field visual beams');


DELETE FROM `spell_script_names` WHERE `spell_id`=87553;
DELETE FROM `spell_script_names` WHERE `spell_id`=93994;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (87553, 'spell_asaad_supremacy_of_the_storm');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (93994, 'spell_asaad_supremacy_of_the_storm');

DELETE FROM `creature_text` WHERE `entry`=43878;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43878, 0, 0, 'Filthy beasts! Your presence in Skywall will not be tolerated!', 1, 0, 0, 0, 0, 20876, 'VO_VP_Ertan_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43878, 1, 0, 'Ravaged!', 1, 0, 0, 0, 0, 20877, 'VO_VP_Ertan_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43878, 2, 0, 'AHHHHHHHHH!', 1, 0, 0, 0, 0, 20878, 'VO_VP_Ertan_Event03');

DELETE FROM `locales_creature_text` WHERE `entry`=43878;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43878, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Грязные животные! Мы не потерпим вашего присутствия в Небесной выси!'),
(43878, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Умри!');

DELETE FROM `creature_text` WHERE `entry`=43875;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 0, 0, 'YOU tread upon the sacrosanct! Mortals have no place amidst the clouds.', 1, 0, 0, 0, 0, 20867, 'VO_VP_AsimAlAkir_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 1, 0, 'BACK to the earth with you!', 1, 0, 0, 0, 0, 20868, 'VO_VP_AsimAlAkir_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 2, 0, 'Al\'Akir, your servant calls for aid!', 1, 0, 0, 0, 0, 20869, 'VO_VP_AsimAlAkir_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 3, 0, 'The winds take me!', 1, 0, 0, 0, 0, 20870, 'VO_VP_AsimAlAkir_Event04');

DELETE FROM `locales_creature_text` WHERE `entry`=43875;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43875, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Твоя жизнь висит на волоске! Смертным не место среди облаков!'),
(43875, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Назад, в землю, из которой ты вышел!'),
(43875, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ал\'акир, твой слуга взывает о помощи!'),
(43875, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ветра приняли меня!');