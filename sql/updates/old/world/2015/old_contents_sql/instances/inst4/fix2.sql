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

DELETE FROM `creature_text` WHERE `entry`=43875;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 0, 0, 'YOU tread upon the sacrosanct! Mortals have no place amidst the clouds.', 1, 0, 0, 0, 0, 20867, 'VO_VP_AsimAlAkir_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 1, 0, 'BACK to the earth with you!', 1, 0, 0, 0, 0, 20868, 'VO_VP_AsimAlAkir_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 2, 0, 'Al\'Akir, your servant calls for aid!', 1, 0, 0, 0, 0, 20869, 'VO_VP_AsimAlAkir_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 3, 0, 'The winds take me!', 1, 0, 0, 0, 0, 20870, 'VO_VP_AsimAlAkir_Event04');

UPDATE `creature_template` SET `InhabitType`=7 WHERE `entry`=45455;
DELETE FROM `conditions` WHERE `SourceEntry` IN (86911, 87517);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '86911', '0', '31', '0', '3', '46492', 'Asaad - Grounding field visual beams'),
('13', '1', '87517', '0', '31', '0', '3', '46492', 'Asaad - Grounding field visual beams');

UPDATE `creature` SET `spawnmask`=0 WHERE `id`=42895 AND `map`=657;

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=46492;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=45455 AND `spell_id`=87742;

UPDATE `creature_template` SET `npcflag`=0, `unit_flags`=0 WHERE `entry`=45455;
DELETE FROM `creature_template_addon` WHERE `entry`=43873;
DELETE FROM `creature_template_addon` WHERE `entry`=43874;

DELETE FROM `creature_text` WHERE `entry`=43878;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43878, 0, 0, 'Filthy beasts! Your presence in Skywall will not be tolerated!', 14, 0, 0, 0, 0, 20876, 'VO_VP_Ertan_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43878, 1, 0, 'Ravaged!', 14, 0, 0, 0, 0, 20877, 'VO_VP_Ertan_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43878, 2, 0, 'AHHHHHHHHH!', 14, 0, 0, 0, 0, 20878, 'VO_VP_Ertan_Event03');

DELETE FROM `creature_text` WHERE `entry`=43875;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 0, 0, 'YOU tread upon the sacrosanct! Mortals have no place amidst the clouds.', 14, 0, 0, 0, 0, 20867, 'VO_VP_AsimAlAkir_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 1, 0, 'BACK to the earth with you!', 14, 0, 0, 0, 0, 20868, 'VO_VP_AsimAlAkir_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 2, 0, 'Al\'Akir, your servant calls for aid!', 14, 0, 0, 0, 0, 20869, 'VO_VP_AsimAlAkir_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43875, 3, 0, 'The winds take me!', 14, 0, 0, 0, 0, 20870, 'VO_VP_AsimAlAkir_Event04');
