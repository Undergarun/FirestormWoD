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
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39705;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=39706;

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

DELETE from `creature_text` where `entry` = 39679;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 0, 0, 'HERETICS! You will suffer for this interruption!', 1, 0, 0, 0, 0, 18589, 'VO_BC_Corla_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 1, 0, 'There is only one true path of enlightenment! DEATH!', 1, 0, 0, 0, 0, 18590, 'VO_BC_Corla_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 2, 0, 'Bask in his power! Rise as an agent of the master\'s rage!', 1, 0, 0, 0, 0, 18592, 'VO_BC_Corla_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39679, 3, 0, 'For the master I\'d die a thousand times... A thousan...', 1, 0, 0, 0, 0, 18594, 'VO_BC_Corla_Event04');

DELETE from `creature_text` where `entry` = 39698;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 0, 0, 'Bodies to test my armaments upon!', 1, 0, 0, 0, 0, 18852, 'VO_BC_Karsh_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 1, 0, 'Merely an impurity in the compound...', 1, 0, 0, 0, 0, 18853, 'VO_BC_Karsh_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 2, 0, 'Feel the burn!', 1, 0, 0, 0, 0, 18854, 'VO_BC_Karsh_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39698, 3, 0, 'We number in the millions! Your efforts are wasted...', 1, 0, 0, 0, 0, 18855, 'VO_BC_Karsh_Event04');

DELETE from `creature_text` where `entry` = 39705;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 0, 0, 'You come seeking answers? Then have them! Look upon your answer to living!', 1, 0, 0, 0, 0, 18899, 'VO_BC_Obsidius_Event01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 1, 0, 'Your kind has no place in the master\'s world.', 1, 0, 0, 0, 0, 18900, 'VO_BC_Obsidius_Event02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 2, 0, 'Earth can be shaped, molded... You cannot! You are useless!', 1, 0, 0, 0, 0, 18901, 'VO_BC_Obsidius_Event03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (39705, 3, 0, 'I cannot be destroyed... Only... de... layed...', 1, 0, 0, 0, 0, 18902, 'VO_BC_Obsidius_Event04');

UPDATE `creature_template` SET `exp` = 3 WHERE `entry` = 14717;
update quest_template set method=0 where id in (27929, 28874, 26314, 14472);
UPDATE `item_template` SET `spellid_1` = 0, `spellcharges_1` = 0, `spellcooldown_1` = -1, `spellcharges_2` = 0 WHERE `entry` = 49700;
UPDATE `item_template` SET `spellid_1` = 0, `spellcooldown_1` = -1 WHERE `entry` = 58169;
UPDATE `item_template` SET `spellid_1` = 0, `spellcooldown_1` = -1 WHERE `entry` = 62775;

insert ignore into spell_bonus_data (entry, direct_bonus, dot_bonus) values
(11826, 0, 0),
(23266, 0, 0),
(26168, 0, 0),
(21162, 0, 0),
(21142, 0, 0);
