-- fix locale
update trinity_string set content_loc8='%s $gполучил:получила; достижение $a!' where entry=810;

-- fix Training Dummy in Ashran
UPDATE `creature_template` SET `name` = 'Dungeoneers Training Dummy', `subname` = 'Tanking' WHERE `entry` = 88836;
UPDATE `creature_template` SET `name` = 'Dungeoneers Training Dummy', `subname` = 'Damage' WHERE `entry` = 87761;
UPDATE `creature_template` SET `unit_flags` = 131072, `unit_flags2` = 2048, `mechanic_immune_mask` = 32, `flags_extra` = 262144, `ScriptName` = 'npc_training_dummy' WHERE `entry` in (87761, 87322, 88836, 88288, 88314);

-- Misc mobs
UPDATE `creature_template` SET `minlevel` = 21, `maxlevel` = 22 WHERE `entry` = 3810;

-- fix flight masters
UPDATE `creature_template` SET `IconName` = 'taxi' WHERE `entry` = 81053;
UPDATE `creature_template` SET `IconName` = 'taxi' WHERE `entry` = 76784;
update creature_template SET `IconName` = 'taxi' where npcflag=8193 and iconname='';
UPDATE `creature_template` SET `gossip_menu_id` = 0 WHERE `entry` in (2409, 931);
update creature_template set `gossip_menu_id` = 0 where npcflag=8193 and gossip_menu_id !='0';

-- fix Marked for Death
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_marked_for_death';
INSERT INTO `spell_script_names` VALUES ('140149', 'spell_rog_marked_for_death'); -- visual

DELETE FROM `spell_linked_spell` WHERE spell_trigger = 137619;
INSERT INTO `spell_linked_spell` VALUES (137619, 140149, 2, 'Marked for Death (check caster)');

-- fix Nightfall proc from Corruption
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_nightfall';
INSERT INTO `spell_script_names` VALUES (146739, 'spell_warl_nightfall');

-- missing entry key, speedup table.
alter table locales_item add PRIMARY KEY (`entry`);
