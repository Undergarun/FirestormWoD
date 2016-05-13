DELETE FROM `creature` WHERE `id` IN (33387, 33802, 34034);
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN (34184, 34219, 32906,33241, 33360);
UPDATE `creature_template` SET `unit_flags`=64 WHERE `entry`=33113;
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=34286;

-- 51 острокрылая

UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN ('34188','34189');
UPDATE `creature_template` SET `InhabitType` = 7, `HoverHeight` = 12 WHERE `entry` IN ('34724','33186');
UPDATE `creature` SET `position_x` = 587.073, `position_y` = -203.205, `position_z` = 441.237, `orientation` = 1.60532 WHERE `id` = 33186;
UPDATE `creature_model_info` SET `combat_reach` = 10 WHERE `modelid` = 28787;
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10066,10067) AND `type` = 18;
DELETE FROM `conditions` WHERE `SourceEntry` = 63317 AND `ConditionValue1` = 4;
INSERT INTO `conditions` VALUES
(13, 3, 63317, 0, 3, 31, 0, 4, 0, 0, 0, 0, '', '');

-- 53 разрушитель

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10080,10081) AND `type`=18;

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_xt002_gravity_bomb_aura_target';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(63024, 'spell_xt002_gravity_bomb_aura_target'),
(64234, 'spell_xt002_gravity_bomb_aura_target');

UPDATE `creature_template` SET `ScriptName`='mob_void_zone' WHERE `entry`=34001;

-- 63 игнис

UPDATE `creature_template` SET `mechanic_immune_mask`=650853247 WHERE `entry` IN (33118,33190);

-- 64 треш

UPDATE `creature_template` SET `spell1` = 64852, `spell2` = 64870, `spell3` = 64847, `AIName` = 'SmartAI' WHERE `entry` = 34234;
UPDATE `creature_template` SET `spell1` = 64852, `spell2` = 64870, `spell3` = 64847 WHERE `entry` = 34235;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34234);
INSERT INTO `smart_scripts` VALUES 
(34234, 0, 0, 0, 0, 0, 100, 0, 2000, 2000, 10000, 10000, 11, 64852, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Runeforged Sentry - Cast Flaming Rune'),
(34234, 0, 1, 0, 0, 0, 100, 0, 3000, 5000, 5000, 7000, 11, 64870, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Runeforged Sentry - Cast Lava Burst'),
(34234, 0, 2, 0, 0, 0, 100, 0, 2500, 3000, 12000, 12000, 11, 64847, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Runeforged Sentry - Cast Runed Flame Jets');

UPDATE `creature_template` SET `spell1` = 62845, `spell2` = 57780, `spell3` = 50370, `AIName` = 'SmartAI' WHERE `entry` = 33236;
UPDATE `creature_template` SET `spell1` = 62845, `spell2` = 57780, `spell3` = 50370 WHERE `entry` = 34113;
UPDATE `creature` SET `spawntimesecs` = 604800 WHERE `id` IN (33236, 33838);
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33236);
INSERT INTO `smart_scripts` VALUES 
(33236, 0, 0, 0, 0, 0, 100, 0, 4000, 6000, 15000, 20000, 11, 62845, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Steelforged Defender - Cast Hamstring'),
(33236, 0, 1, 0, 0, 0, 100, 0, 2000, 4000, 6000, 8000, 11, 57780, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Steelforged Defender - Cast Lightning Bolt'),
(33236, 0, 2, 0, 0, 0, 100, 0, 5000, 6000, 4000, 6000, 11, 50370, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Steelforged Defender - Cast Sunder Armor');

UPDATE `creature_template` SET `spell1` = 64693, `spell2` = 64953, `AIName` = 'SmartAI' WHERE `entry` = 34164;
UPDATE `creature_template` SET `spell1` = 64693, `spell2` = 64953 WHERE `entry` = 34165;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34164);
INSERT INTO `smart_scripts` VALUES 
(34164, 0, 0, 0, 0, 0, 100, 0, 3000, 4000, 6000, 8000, 11, 64693, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Mechagnome Battletank - Cast Flame Cannon'),
(34164, 0, 1, 0, 0, 0, 100, 0, 10000, 10000, 15000, 20000, 11, 64953, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,'Mechagnome Battletank - Cast Jump Attack');

UPDATE `creature_template` SET `spell1` = 62625, `AIName` = 'SmartAI' WHERE `entry` = 33237;
UPDATE `creature_template` SET `spell1` = 62625 WHERE `entry` = 34105;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33237);
INSERT INTO `smart_scripts` VALUES 
(33237, 0, 0, 0, 0, 0, 100, 0, 8000, 10000, 15000, 20000, 11, 62625, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Ulduar Colossus - Cast Ground Slam');

UPDATE `creature_template` SET `spell1` = 64697, `spell2` = 64698, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34069;
UPDATE `creature_template` SET `spell1` = 64697, `spell2` = 64698, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34185;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34069);
INSERT INTO `smart_scripts` VALUES 
(34069, 0, 0, 0, 0, 0, 100, 0, 6000, 10000, 10000, 12000, 11, 64697, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Molten Colossus - Cast Earthquake'),
(34069, 0, 1, 0, 0, 0, 100, 0, 10000, 10000, 15000, 20000, 11, 64698, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Molten Colossus - Cast Pyroblast');

UPDATE `creature_template` SET `spell1` = 64773, `spell2` = 64746, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34086;
UPDATE `creature_template` SET `spell1` = 64773, `spell2` = 64746, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34201;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34086);
INSERT INTO `smart_scripts` VALUES 
(34086, 0, 0, 0, 0, 0, 100, 0, 2000, 4000, 6000, 8000, 11, 64773, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Magma Rager - Cast Fire Blast'),
(34086, 0, 1, 0, 0, 0, 100, 0, 8000, 16000, 15000, 25000, 11, 64746, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Magma Rager - Cast Superheated winds');
UPDATE `creature_template` SET `unit_flags` = 33718790, modelid1 = 11686, modelid2 = 0, `spell1` = 64724, `AIName` = 'SmartAI' WHERE `entry` = 34194;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34194);
INSERT INTO `smart_scripts` VALUES 
(34194, 0, 0, 0, 0, 0, 100, 0, 1000, 2000, 10000, 10000, 11, 64724, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Superheated Winds');

UPDATE `creature_template` SET `spell1` = 64719, `spell2` = 64720, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34085;
UPDATE `creature_template` SET `spell1` = 64719, `spell2` = 64721, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34186;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34085);
INSERT INTO `smart_scripts` VALUES 
(34085, 0, 0, 0, 0, 0, 100, 0, 8000, 12000, 10000, 16000, 11, 64719, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,'Forge Construct - Cast Charge'),
(34085, 0, 1, 0, 0, 0, 100, 2, 2000, 6000, 6000, 8000, 11, 64720, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Forge Construct - Cast Flame Emission 10'),
(34085, 0, 2, 0, 0, 0, 100, 4, 2000, 6000, 6000, 8000, 11, 64721, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Forge Construct - Cast Flame Emission 25');

UPDATE `creature_template` SET `spell1` = 65080, `spell2` = 65084, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34273;
UPDATE `creature_template` SET `spell1` = 65104, `spell2` = 65084, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34274;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34273);
INSERT INTO `smart_scripts` VALUES 
(34273, 0, 0, 0, 2, 0, 100, 0, 30, 20, 0, 0, 11, 65084, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'XB-488 Disposalbot - Cast Self Destruct'),
(34273, 0, 1, 0, 0, 0, 100, 2, 2000, 6000, 10000, 15000, 11, 65080, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'XB-488 Disposalbot - Cast Cut Scrap Metal 10'),
(34273, 0, 2, 0, 0, 0, 100, 4, 2000, 6000, 10000, 15000, 11, 65104, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'XB-488 Disposalbot - Cast Cut Scrap Metal 25');

UPDATE `creature_template` SET `spell1` = 65071, `spell2` = 65070, `mingold` = 7100, `maxgold` = 7600, `mechanic_immune_mask` = 33554432, `AIName` = 'SmartAI' WHERE `entry` = 34267;
UPDATE `creature_template` SET `spell1` = 65071, `spell2` = 65070, `mingold` = 14200, `maxgold` = 15600, `mechanic_immune_mask` = 33554432 WHERE `entry` = 34268;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34267);
INSERT INTO `smart_scripts` VALUES 
(34267, 0, 0, 0, 0, 0, 100, 0, 8000, 12000, 10000, 15000, 11, 65071, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,'Parts Recovery Technician - Cast Mechano Kick'),
(34267, 0, 1, 0, 0, 0, 100, 0, 6000, 8000, 20000, 25000, 11, 65070, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Parts Recovery Technician - Cast Defense Matrix');

UPDATE `creature_template` SET `spell1` = 65073, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34271;
UPDATE `creature_template` SET `spell1` = 65106, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34272;
UPDATE `creature_template` SET `mingold` = 7100, `maxgold` = 7600 WHERE `entry` = 34269;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34271);
INSERT INTO `smart_scripts` VALUES 
(34271, 0, 0, 0, 0, 0, 100, 2, 8000, 12000, 15000, 20000, 11, 65073, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,'XD-175 Compactobot - Cast Trash Compactor 10'),
(34271, 0, 1, 0, 0, 0, 100, 4, 8000, 12000, 15000, 20000, 11, 65106, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0,'XD-175 Compactobot - Cast Trash Compactor 25');

-- 88 разрушитель, острокрылая, игнис
UPDATE `gameobject_template` SET `flags` = 32 WHERE `entry` IN (194542,194541,194543,194519);

UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (33221,34001);
UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16 WHERE `entry` IN (34004,34005);

DELETE FROM `creature` WHERE `id` IN (34004,34005,33233);
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
('33233','603','3','65535','0','0','572.411','-138.656','393.904','0','120','0','0','1','0','0','0','0','0'),
('33233','603','3','65535','0','0','630.244','-276.259','392.312','0','120','0','0','1','0','0','0','0','0'),
('33233','603','3','65535','0','0','638.242','-272.173','392.135','0','120','0','0','1','0','0','0','0','0'),
('33233','603','3','65535','0','0','605.928','-140.109','393.797','0','120','0','0','1','0','0','0','0','0'),
('33233','603','3','65535','0','0','589.729','-137.115','393.901','0','120','0','0','1','0','0','0','0','0'),
('33233','603','3','65535','0','0','585.928','-146.588','391.6','0','120','0','0','1','0','0','0','0','0'),
('33233','603','3','65535','0','0','560.133','-143.035','393.823','0','120','0','0','1','0','0','0','0','0');

DELETE FROM `creature_template` WHERE `entry` = 33233;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
('33233','0','0','0','0','0','169','11686','0','0','Razorscale Controller','','','0','80','80','2','0','35','35','0','1','1.14286','1','1','422','586','0','642','7.5','2000','0','1','33554432','2048','8','0','0','0','0','0','345','509','103','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','255','1','1','1','0','0','0','0','0','0','0','0','1','0','0','2','boss_razorscale_controller','15595');

-- 97 собрание

UPDATE `creature_template` SET `mechanic_immune_mask`=617296767, `flags_extra`=1 WHERE `entry` IN (32857);   

DELETE FROM `spell_script_names` WHERE `spell_id` IN (61902, 61911, 63495, 61920);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(61902, 'spell_steelbreaker_electrical_charge'),
(61911, 'spell_steelbreaker_static_disruption'),
(63495, 'spell_steelbreaker_static_disruption'),
(61920, 'spell_supercharge');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10082,10083,10084,10085,10086,10087,10088,10418,10419,10089,10420,10421,10090,10422,10423,10091,10424,10425);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10082, 11, 0, 0, 'achievement_i_choose_you'),
(10082, 12, 0, 0, ''),
(10083, 11, 0, 0, 'achievement_i_choose_you'),
(10083, 12, 0, 0, ''),
(10084, 11, 0, 0, 'achievement_i_choose_you'), 
(10084, 12, 0, 0, ''),
(10085, 11, 0, 0, 'achievement_i_choose_you_25'),
(10085, 12, 1, 0, ''),
(10086, 11, 0, 0, 'achievement_i_choose_you_25'),
(10086, 12, 1, 0, ''),
(10087, 11, 0, 0, 'achievement_i_choose_you_25'), 
(10087, 12, 1, 0, ''),
(10088, 11, 0, 0, 'achievement_but_i_am_on_your_side'), 
(10088, 12, 0, 0, ''),
(10418, 11, 0, 0, 'achievement_but_i_am_on_your_side'), 
(10418, 12, 0, 0, ''),
(10419, 11, 0, 0, 'achievement_but_i_am_on_your_side'), 
(10419, 12, 0, 0, ''),
(10089, 11, 0, 0, 'achievement_but_i_am_on_your_side_25'), 
(10089, 12, 1, 0, ''),
(10420, 11, 0, 0, 'achievement_but_i_am_on_your_side_25'),
(10420, 12, 1, 0, ''),
(10421, 11, 0, 0, 'achievement_but_i_am_on_your_side_25'),
(10421, 12, 1, 0, ''),
(10090, 11, 0, 0, 'achievement_cant_do_that_while_stunned'),
(10090, 12, 0, 0, ''),
(10422, 11, 0, 0, 'achievement_cant_do_that_while_stunned'), 
(10422, 12, 0, 0, ''),
(10423, 11, 0, 0, 'achievement_cant_do_that_while_stunned'), 
(10423, 12, 0, 0, ''),
(10091, 11, 0, 0, 'achievement_cant_do_that_while_stunned_25'), 
(10091, 12, 1, 0, ''),
(10424, 11, 0, 0, 'achievement_cant_do_that_while_stunned_25'),
(10424, 12, 1, 0, ''),
(10425, 11, 0, 0, 'achievement_cant_do_that_while_stunned_25'), 
(10425, 12, 1, 0, '');

-- 201 собрание
UPDATE `creature_template` SET `lootid` = 32857 WHERE `entry` = 32857;
UPDATE `creature_template` SET `lootid` = 32867 WHERE `entry` = 32867;
UPDATE `creature_template` SET `lootid` = 32927 WHERE `entry` = 32927;
UPDATE `creature_template` SET `lootid` = 33692 WHERE `entry` = 33692;
UPDATE `creature_template` SET `lootid` = 33693 WHERE `entry` = 33693;
UPDATE `creature_template` SET `lootid` = 33694 WHERE `entry` = 33694;

DELETE FROM `creature_loot_template` WHERE `entry` IN (32857,33694,32867,32927,33693,33692);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
('32857','1','100','1','0','-34359','2'),
('32857','47241','100','1','0','1','1'),
('32867','1','100','1','0','-34359','2'),
('32867','25455','0','1','1','1','1'),
('32867','45087','75','1','0','1','1'),
('32867','45447','0','1','1','1','1'),
('32867','45448','0','1','1','1','1'),
('32867','45449','0','1','1','1','1'),
('32867','45456','0','1','1','1','1'),
('32867','45506','100','1','0','1','1'),
('32867','47241','100','1','0','2','2'),
('32927','1','100','1','0','-34359','2'),
('32927','45087','75','1','0','1','1'),
('32927','45506','100','2','0','1','1'),
('32927','47241','100','1','0','2','2'),
('33692','1','100','1','0','-34360','3'),
('33692','2','10','1','0','-34154','1'),
('33692','45038','8','1','0','1','1'),
('33692','45087','75','1','0','1','1'),
('33692','45506','100','1','0','1','1'),
('33692','47241','100','1','0','2','2'),
('33693','1','100','1','0','-34360','3'),
('33693','2','10','1','0','-34154','1'),
('33693','45038','18','1','0','1','1'),
('33693','45087','75','1','0','2','2'),
('33693','45241','0','1','1','1','1'),
('33693','45242','0','1','1','1','1'),
('33693','45243','0','1','1','1','1'),
('33693','45244','0','1','1','1','1'),
('33693','45245','0','1','1','1','1'),
('33693','45506','100','1','0','1','1'),
('33693','45607','0','1','1','1','1'),
('33693','47241','100','1','0','2','2'),
('33694','1','100','1','0','-34360','3'),
('33694','2','10','1','0','-34154','1'),
('33694','45038','8','1','0','1','1'),
('33694','45087','75','1','0','1','1'),
('33694','47241','100','1','0','1','2');

DELETE FROM `reference_loot_template` WHERE `entry` IN (34359,34154,34360);
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
('34154','45088','4','1','1','1','1'),
('34154','45089','4','1','1','1','1'),
('34154','45090','4','1','1','1','1'),
('34154','45091','4','1','1','1','1'),
('34154','45092','4','1','1','1','1'),
('34154','45093','4','1','1','1','1'),
('34154','45094','4','1','1','1','1'),
('34154','45095','4','1','1','1','1'),
('34154','45096','4','1','1','1','1'),
('34154','45097','4','1','1','1','1'),
('34154','45098','4','1','1','1','1'),
('34154','45099','4','1','1','1','1'),
('34154','45100','4','1','1','1','1'),
('34154','45101','4','1','1','1','1'),
('34154','45102','4','1','1','1','1'),
('34154','45103','4','1','1','1','1'),
('34154','45104','4','1','1','1','1'),
('34154','45105','4','1','1','1','1'),
('34154','46027','14','1','1','1','1'),
('34154','46348','14','1','1','1','1'),
('34359','45322','0','1','1','1','1'),
('34359','45324','0','1','1','1','1'),
('34359','45329','0','1','1','1','1'),
('34359','45330','0','1','1','1','1'),
('34359','45331','0','1','1','1','1'),
('34359','45332','0','1','1','1','1'),
('34359','45333','0','1','1','1','1'),
('34359','45378','0','1','1','1','1'),
('34359','45418','0','1','1','1','1'),
('34359','45423','0','1','1','1','1'),
('34360','45193','0','1','1','1','1'),
('34360','45224','0','1','1','1','1'),
('34360','45225','0','1','1','1','1'),
('34360','45226','0','1','1','1','1'),
('34360','45227','0','1','1','1','1'),
('34360','45228','0','1','1','1','1'),
('34360','45232','0','1','1','1','1'),
('34360','45233','0','1','1','1','1'),
('34360','45234','0','1','1','1','1'),
('34360','45235','0','1','1','1','1'),
('34360','45236','0','1','1','1','1'),
('34360','45237','0','1','1','1','1'),
('34360','45238','0','1','1','1','1'),
('34360','45239','0','1','1','1','1'),
('34360','45240','0','1','1','1','1');

-- 202 треш

UPDATE `creature_template` SET `spell1` = 64820, `spell2` = 64825, `mingold` = 7100, `maxgold` = 7600, `mechanic_immune_mask` = 33554496, `AIName` = 'SmartAI' WHERE `entry` = 34197;
UPDATE `creature_template` SET `spell1` = 64943, `spell2` = 64944, `mingold` = 14200, `maxgold` = 15600, `mechanic_immune_mask` = 33554496 WHERE `entry` = 34226;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34197);
INSERT INTO `smart_scripts` VALUES
(34197, 0, 0, 0, 0, 0, 100, 2, 4000, 8000, 6000, 8000, 11, 64820, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Chamber Overseer - Cast Devastating Leap 10'),
(34197, 0, 1, 0, 0, 0, 100, 4, 4000, 8000, 6000, 8000, 11, 64943, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Chamber Overseer - Cast Devastating Leap 25'),
(34197, 0, 2, 0, 0, 0, 100, 2, 10000, 12000, 8000, 12000, 11, 64825, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Chamber Overseer - Cast Staggering Roar 10'),
(34197, 0, 3, 0, 0, 0, 100, 4, 10000, 12000, 8000, 12000, 11, 64944, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Chamber Overseer - Cast Staggering Roar 25');

UPDATE `creature_template` SET `spell1` = 64852, `spell2` = 64870, `spell3` = 64847, `mechanic_immune_mask` = 33554496, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34196;
UPDATE `creature_template` SET `spell1` = 64852, `spell2` = 64870, `spell3` = 64847, `mechanic_immune_mask` = 33554496, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34245;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34196);
INSERT INTO `smart_scripts` VALUES
(34196, 0, 0, 0, 0, 0, 100, 0, 2000, 2000, 10000, 10000, 11, 64852, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Rune Etched Sentry - Cast Flaming Rune'),
(34196, 0, 1, 0, 0, 0, 100, 0, 3000, 5000, 5000, 7000, 11, 64870, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Rune Etched Sentry - Cast Lava Burst'),
(34196, 0, 2, 0, 0, 0, 100, 0, 2500, 3000, 12000, 15000, 11, 64847, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Rune Etched Sentry - Cast Runed Flame Jets');

UPDATE `creature_template` SET `spell1` = 64918, `spell2` = 64903, `spell3` = 64897, `mechanic_immune_mask` = 33554496, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34198;
UPDATE `creature_template` SET `spell1` = 64971, `spell2` = 64970, `spell3` = 64968, `mechanic_immune_mask` = 33554496, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34236;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34198);
INSERT INTO `smart_scripts` VALUES
(34198, 0, 0, 0, 0, 0, 100, 2, 2000, 4000, 4000, 6000, 11, 64918, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Iron Mender - Cast Electro Shock 10'),
(34198, 0, 1, 0, 0, 0, 100, 4, 2000, 4000, 4000, 6000, 11, 64971, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Iron Mender - Cast Electro Shock 25'),
(34198, 0, 2, 0, 0, 0, 100, 2, 3000, 6000, 10000, 15000, 11, 64903, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Iron Mender - Cast Fuse Lightning 10'),
(34198, 0, 3, 0, 0, 0, 100, 4, 3000, 6000, 10000, 15000, 11, 64970, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Iron Mender - Cast Fuse Lightning 25'),
(34198, 0, 4, 0, 0, 0, 100, 2, 10000, 25000, 30000, 45000, 11, 64897, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Iron Mender - Cast Fuse Metal 10'),
(34198, 0, 5, 0, 0, 0, 100, 4, 10000, 25000, 30000, 45000, 11, 64968, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Iron Mender - Cast Fuse Metal 25');

UPDATE `creature_template` SET `spell1` = 64877, `spell2` = 64874, `mingold` = 7100, `maxgold` = 7600, `mechanic_immune_mask` = 33554496, `AIName` = 'SmartAI' WHERE `entry` = 34190;
UPDATE `creature_template` SET `spell1` = 64877, `spell2` = 64967, `mingold` = 14200, `maxgold` = 15600, `mechanic_immune_mask` = 33554496 WHERE `entry` = 34229;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34190);
INSERT INTO `smart_scripts` VALUES
(34190, 0, 0, 0, 0, 0, 100, 0, 4000, 8000, 25000, 30000, 11, 64877, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Hardened Iron Golem - Cast Harden Fists'),
(34190, 0, 1, 0, 0, 0, 100, 2, 5000, 7000, 20000, 30000, 11, 64874, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Hardened Iron Golem - Cast Rune Punch 10'),
(34190, 0, 2, 0, 0, 0, 100, 4, 5000, 7000, 20000, 30000, 11, 64967, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Hardened Iron Golem - Cast Rune Punch 25');

UPDATE `creature_template` SET `spell1` = 64889, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI' WHERE `entry` = 34199;
UPDATE `creature_template` SET `spell1` = 64975, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34237;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34199);
INSERT INTO `smart_scripts` VALUES
(34199, 0, 0, 0, 0, 0, 100, 2, 1000, 2000, 10000, 15000, 11, 64889, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Lightning Charged Iron Dwarf - Cast Lightning Charged 10'),
(34199, 0, 1, 0, 0, 0, 100, 4, 1000, 2000, 10000, 15000, 11, 64975, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Lightning Charged Iron Dwarf - Cast Lightning Charged 25');

-- 218 кологарн
UPDATE `creature` SET `position_x` = 1797.15, `position_y` = -24.4027, `position_z` = 448.741, `orientation` = 3.1935 WHERE `id` = 32930;
UPDATE `creature_model_info` SET `combat_reach` = 20 WHERE `modelid` = 28638;

UPDATE `gameobject` SET `state` = 0 WHERE `id` = 194232;
UPDATE `gameobject_template` SET `flags` = 40 WHERE `entry` = 194232;

UPDATE `creature_template` SET `ScriptName` = 'npc_kologarn_arm' WHERE `entry` IN (32933,32934);

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10284, 10722, 10285, 10095, 10290, 10133, 10286, 10099);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10284, 11, 0, 0, 'achievement_disarmed'),
(10284, 12, 0, 0, ''),
(10722, 11, 0, 0, 'achievement_disarmed_25'),
(10722, 12, 1, 0, ''),
(10285, 11, 0, 0, 'achievement_with_open_arms'),
(10285, 12, 0, 0, ''),
(10095, 11, 0, 0, 'achievement_with_open_arms_25'),
(10095, 12, 1, 0, ''),
(10290, 11, 0, 0, 'achievement_rubble_and_roll'),
(10290, 12, 0, 0, ''),
(10133, 11, 0, 0, 'achievement_rubble_and_roll_25'),
(10133, 12, 1, 0, ''),
(10286, 11, 0, 0, 'achievement_if_looks_could_kill'),
(10286, 12, 0, 0, ''),
(10099, 11, 0, 0, 'achievement_if_looks_could_kill_25'),
(10099, 12, 1, 0, '');

-- 231

UPDATE `creature_template` SET `RegenHealth` = 0 WHERE `entry` IN (33329, 33995);

DELETE FROM `script_texts` WHERE `npc_entry`=33118;
DELETE FROM `creature_text` WHERE `entry` = 33118;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33118, 0, 0, 'Insolent whelps! Your blood will temper the weapons used to reclaim this world!', 14, 0, 100, 0, 0, 15564, 'Ignis SAY_AGGRO'),
(33118, 1, 0, 'More scraps for the scrapheap!', 14, 0, 100, 0, 0, 15569, 'Ignis SAY_SLAY_1'),
(33118, 1, 1, 'Your bones will serve as kindling!', 14, 0, 100, 0, 0, 15570, 'Ignis SAY_SLAY_2'),
(33118, 2, 0, 'I. Have. Failed.', 14, 0, 100, 0, 0, 15572, 'Ignis SAY_DEATH'),
(33118, 3, 0, 'Arise, soldiers of the Iron Crucible! The Makers'' will be done!', 14, 0, 100, 0, 0, 15565, 'Ignis SAY_SUMMON'),
(33118, 4, 0, 'I will burn away your impurities!', 14, 0, 100, 0, 0, 15566, 'Ignis SAY_SLAG_POT'),
(33118, 5, 0, 'Let the inferno consume you!', 14, 0, 100, 0, 0, 15567, 'Ignis SAY_SCORCH_1'),
(33118, 5, 1, 'BURN! Burn in the makers fire!', 14, 0, 100, 0, 0, 15568, 'Ignis SAY_SCORCH_2'),
(33118, 6, 0, 'Let it be finished!', 14, 0, 100, 0, 0, 15571, 'Ignis SAY_BERSERK'),
(33118, 7, 0, '%s begins to cast Flame Jets!', 41, 0, 100, 0, 0, 0, 'Ignis EMOTE_JETS');

DELETE FROM `locales_creature_text` WHERE `entry` IN (33118);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(33118, 0, 0, 'Дерзкие глупцы! Ваша кровь закалит оружие, которым был завоеван этот мир!'),
(33118, 1, 0, 'Тебе место в мусорной куче!'),
(33118, 1, 1, 'Твои кости пойдут на растопку!'),
(33118, 2, 0, 'Я... Проиграл...'),
(33118, 3, 0, 'Вперед, воины Железного Тигеля! Да свершится... воля Творца.'),
(33118, 4, 0, 'Пройдите очищение огнем!'),
(33118, 5, 0, 'Да поглотит вас пламя преисподней!'),
(33118, 5, 1, 'ГОРИТЕ!!! Горите в огне Творца.'),
(33118, 6, 0, 'ПОКОНЧИМ С ЭТИМ!!'),
(33118, 7, 0, '%s читает заклинание "Огненная струя"!');


-- Razorscale
DELETE FROM `script_texts` WHERE `npc_entry` IN (33186, 33210, 33287, 33233);
DELETE FROM `creature_text` WHERE `entry` IN (33186, 33210, 33287, 33233);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33186, 0, 0, '%s takes a deep breath...', 41, 0, 100, 0, 0, 0, 'Razorscale EMOTE_BREATH'),
(33186, 1, 0, '%s grounded permanently!', 41, 0, 100, 0, 0, 0, 'Razorscale EMOTE_PERMA'),
(33210, 0, 0, 'Welcome, champions! All of our attempts at grounding her have failed. We could use a hand in bring her down with these harpoon guns.', 14, 0, 100, 0, 0, 15647, 'Exp Commander SAY_GREET'),
(33210, 1, 0, 'Move! Quickly! She won''t remain grounded for long.', 14, 0, 100, 0, 0, 15648, 'Exp Commander SAY_GROUND_PHASE'),
(33210, 2, 0, 'Be on the lookout! Mole machines will be surfacing soon with those nasty Iron dwarves aboard!', 14, 0, 100, 0, 0, 0, 'Exp Commander SAY_SUMMON_MOLE_MACHINES'),
(33287, 0, 0, 'Give us a moment to prepare to build the turrets.', 14, 0, 100, 0, 0, 0, 'Exp Engineer SAY_PREPARED'),
(33287, 1, 0, 'Ready to move out, keep those dwarves off of our backs!', 14, 0, 100, 0, 0, 0, 'Exp Engineer SAY_READY'),
(33287, 2, 0, 'Fires out! Let''s rebuild those turrets!', 14, 0, 100, 0, 0, 0, 'Exp Engineer SAY_TURRETS'),
(33233, 0, 0, 'Harpoon Turret is ready for use!', 41, 0, 100, 0, 0, 0, 'Razorscale Controller EMOTE_HARPOON');

DELETE FROM `locales_creature_text` WHERE `entry` IN (33186, 33210, 33287, 33233);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(33186, 0, 0, '%s делает глубокий вдох.'),
(33186, 1, 0, '%s обессилена и больше не может летать!'),
(33210, 0, 0, 'Приветствую вас, герои! Все наши попытки сбить ее не увенчались успехом. Нам бы пригодилась ваша помощь. Гарпунных пушек на всех хватит.'),
(33210, 1, 0, 'Быстрее! Сейчас она снова взлетит!'),
(33210, 2, 0, 'Не спать! Скоро покажутся буровые машины этих отвратительных железных дворфов!'),
(33287, 0, 0, 'Дайте время подготовить пушки'),
(33287, 1, 0, 'Готовы выходить, охраняйте тылы от дворфов!'),
(33287, 2, 0, 'Огонь прекратился! Надо починить пушки!'),
(33233, 0, 0, 'Гарпунная пушка готова');

DELETE FROM `script_texts` WHERE `npc_entry` IN (33293, 33343);
DELETE FROM `creature_text` WHERE `entry` IN (33293, 33343);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33293, 0, 0, 'New toys? For me? I promise I won''t break them this time!', 14, 0, 100, 0, 0, 15724, 'XT002 SAY_AGGRO'),
(33293, 1, 0, 'So tired. I will rest for just a moment!', 14, 0, 100, 0, 0, 15725, 'XT002 SAY_HEART_OPENED'),
(33293, 2, 0, 'I''m ready to play!', 14, 0, 100, 0, 0, 15726, 'XT002 SAY_HEART_CLOSED'),
(33293, 3, 0, 'NO! NO! NO! NO! NO!', 14, 0, 100, 0, 0, 15727, 'XT002 SAY_TYMPANIC_TANTRUM'),
(33293, 4, 0, 'I... I think I broke it.', 14, 0, 100, 0, 0, 15728, 'XT002 SAY_SLAY_1'),
(33293, 4, 1, 'I guess it doesn''t bend that way.', 14, 0, 100, 0, 0, 15729, 'XT002 SAY_SLAY_2'),
(33293, 5, 0, 'I''m tired of these toys. I don''t want to play anymore!', 14, 0, 100, 0, 0, 15730, 'XT002 SAY_BERSERK'),
(33293, 6, 0, 'You are bad... Toys... Very... Baaaaad!', 14, 0, 100, 0, 0, 15731, 'XT002 SAY_DEATH'),
(33293, 7, 0, 'Time for a new game! My old toys will fight my new toys!', 14, 0, 100, 0, 0, 15732, 'XT002 SAY_SUMMON'),
(33293, 8, 0, '%s begins to cause the earth to quake.', 41, 0, 100, 0, 0, 0, 'XT002 EMOTE_TYMPANIC'),
(33293, 9, 0, 'XT-002 Deconstructor''s heart is exposed and leaking energy.', 41, 0, 100, 0, 0, 0, 'XT002 EMOTE_HEART'),
(33343, 0, 0, '%s consumes a scrap bot to repair himself!', 41, 0, 100, 0, 0, 0, 'XS-013 Scrapbot EMOTE_REPAIR');

DELETE FROM `locales_creature_text` WHERE `entry` IN (33293, 33343);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(33293, 0, 0, 'Новые игрушки? Для меня? Обещаю, в этот раз я их не поломаю!'),
(33293, 1, 0, 'Я так устал. Отдохну чуток ...'),
(33293, 2, 0, 'Продолжаем игру!'),
(33293, 3, 0, 'НЕТ! НЕТ! НЕТ! НЕТ! НЕТ!'),
(33293, 4, 0, 'Кажется, я сломал эту игрушку.'),
(33293, 4, 1, 'Наверное, в эту сторону голова не гнулась ...'),
(33293, 5, 0, 'Я устал от этих игрушек. Не хочу больше играть!'),
(33293, 6, 0, 'Плохие... игрушки... очень... плохиеееее.'),
(33293, 7, 0, 'Время для новой игры! Мои старые игрушки должны подраться с моими новыми игрушками!'),
(33293, 8, 0, '%s вызывает землетрясение.'),
(33293, 9, 0, 'Сердце Разрушителя XT-002 обнажено и истекает энергией.'),
(33343, 0, 0, '%s потребляет хламобота для восстановления!');

-- 249 телепорт

DELETE FROM `spell_target_position` WHERE `id` IN (64014,64032,64028,64031,64030,64029,64024,64025,64027);
INSERT INTO `spell_target_position` VALUES
(64014, 0, 603, -706.122, -92.60, 429.876, 0.05),
(64032, 0, 603, 131.2480, -35.38, 409.803, 0),
(64028, 0, 603, 553.2329, -12.32, 410.120, 0),
(64031, 0, 603, 926.2920, -11.46, 418.595, 3.20),
(64030, 0, 603, 1498.090, -24.25, 420.967, 0),
(64029, 0, 603, 1859.450, -24.10, 448.812, 0),
(64024, 0, 603, 2086.270, -24.31, 421.239, 0),
(64025, 0, 603, 2517.058, 2569.1, 412.699, 6.28),
(64027, 0, 603, 1854.778, -11.58, 334.975, 5.50);

DELETE FROM `trinity_string` WHERE `entry` > 11999 AND `entry` < 12010;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc8`) VALUES
(12000, 'Teleport to the Expedition Base Camp', 'Переместиться в Главный лагерь экспедиции'),
(12001, 'Teleport to the Formation Grounds', 'Переместиться на Плац'),
(12002, 'Teleport to the Colossal Forge', 'Переместиться в Гигантскую кузню'),
(12003, 'Teleport to the Scrapyard', 'Переместиться на Мусорную свалку'),
(12004, 'Teleport to the Antechamber of Ulduar', 'Переместиться в Вестибюль'),
(12005, 'Teleport to the Shattered Walkway', 'Переместиться в Разрушенную аллею'),
(12006, 'Teleport to the Conservatory of Life', 'Переместиться в Консерваторию'),
(12007, 'Teleport to the Spark of Imagination', 'Переместиться в зал Искры воображения'),
(12008, 'Teleport to the Descent into Madness', 'Переместиться в тюрьму Йогг-Сарона'),
(12009, 'Activate Harpoons!', 'Мы готовы помочь!');

-- 251 ауриайя

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10399,10243,10400,10184) AND `type` = 18;
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (34098,34174);
DELETE FROM `creature` WHERE `id` = 34014;
UPDATE `creature` SET `position_x` = 420.728, `position_y` = -15.3721, `position_z` = 409.804, `orientation` = 3.05873 WHERE `id` = 33113;

-- 262 кологарн

UPDATE `creature_template` SET `ScriptName` = 'npc_focused_eyebeam' WHERE `entry` IN (33632,33802);

-- 275 кологарн

UPDATE `creature_model_info` SET `bounding_radius` = 0.31, `combat_reach` = 20 WHERE `modelid` IN (28821,28822);
UPDATE `gameobject` SET `position_x` = 1837.59, `position_y` = -35.6824, `position_z` = 448.808 WHERE `id` = 195047;

DELETE FROM `script_texts` WHERE `npc_entry`=32930;
DELETE FROM `creature_text` WHERE `entry`=32930;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(32930, 0, 0, 'None shall pass!', 14, 0, 100, 0, 0, 15586, 'Kologarn SAY_AGGRO'),
(32930, 1, 0, 'KOL-THARISH!', 14, 0, 100, 0, 0, 15587, 'Kologarn SAY_SLAY_1'),
(32930, 1, 1, 'YOU FAIL!', 14, 0, 100, 0, 0, 15588, 'Kologarn SAY_SLAY_2'),
(32930, 2, 0, 'Just a scratch!', 14, 0, 100, 0, 0, 15589, 'Kologarn SAY_LEFT_ARM_GONE'),
(32930, 3, 0, 'Only a flesh wound!', 14, 0, 100, 0, 0, 15590, 'Kologarn SAY_RIGHT_ARM_GONE'),
(32930, 4, 0, 'OBLIVION!', 14, 0, 100, 0, 0, 15591, 'Kologarn SAY_SHOCKWAVE'),
(32930, 5, 0, 'I will squeeze the life from you!', 14, 0, 100, 0, 0, 15592, 'Kologarn SAY_GRAB_PLAYER'),
(32930, 6, 0, 'Master, they come...', 14, 0, 100, 0, 0, 15593, 'Kologarn SAY_DEATH'),
(32930, 7, 0, 'I am invincible!', 14, 0, 100, 0, 0, 15594, 'Kologarn SAY_BERSERK'),
(32930, 8, 0, 'The Left Arm has regrown!', 41, 0, 100, 0, 0, 0, 'Kologarn EMOTE_LEFT'),
(32930, 9, 0, 'The Right Arm has regrown!', 41, 0, 100, 0, 0, 0, 'Kologarn EMOTE_RIGHT'),
(32930, 10, 0, '%s casts Stone Grip!', 41, 0, 100, 0, 0, 0, 'Kologarn EMOTE_STONE');

DELETE FROM `locales_creature_text` WHERE `entry` IN (32930);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(32930, 0, 0, 'Вам не пройти!'),
(32930, 1, 0, 'КОЛ-ТАРИШ!'),
(32930, 1, 1, 'ТЕБЕ КОНЕЦ!'),
(32930, 2, 0, 'Царапина...'),
(32930, 3, 0, 'Всего лишь плоть!'),
(32930, 4, 0, 'ЗАБВЕНИЕ!'),
(32930, 5, 0, 'Я лишу тебя жизни!'),
(32930, 6, 0, 'Повелитель, они идут...'),
(32930, 7, 0, 'Я не победим!'),
(32930, 8, 0, 'Левая рука отросла заново!'),
(32930, 9, 0, 'Правая рука отросла заново!'),
(32930, 10, 0, '%s применяет каменную хватку!');

-- 332 ауриайя

DELETE FROM `spell_script_names` WHERE `ScriptName`	= 'spell_auriaya_strenght_of_the_pack';

DELETE FROM `script_texts` WHERE `npc_entry` = 33515;
DELETE FROM `creature_text` WHERE `entry` = 33515;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33515, 0, 0, 'Some things are better left alone!', 14, 0, 100, 0, 0, 15473, 'Auriaya SAY_AGGRO'),
(33515, 1, 0, 'The secret dies with you!', 14, 0, 100, 0, 0, 15474, 'Auriaya SAY_SLAY_1'),
(33515, 1, 1, 'There is no escape!', 14, 0, 100, 0, 0, 15475, 'Auriaya SAY_SLAY_2'),
(33515, 2, 0, 'You waste my time!', 14, 0, 100, 0, 0, 15477, 'Auriaya SAY_BERSERK'),
(33515, 3, 0, 'Auriaya begins to cast Terrifying Screech.', 41, 0, 100, 0, 0, 0, 'Auriaya - EMOTE_FEAR'),
(33515, 4, 0, 'Auriaya begins to activate the Feral Defender!', 41, 0, 100, 0, 0, 0, 'Auriaya - EMOTE_DEFENDER');

DELETE FROM `locales_creature_text` WHERE `entry` = 33515;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(33515, 0, 0, 'Вы зря сюда заявились!'),
(33515, 1, 0, 'Эта тайна умрет вместе с тобой.'),
(33515, 1, 1, 'Спасения нет.'),
(33515, 2, 0, 'Вы попусту тратите мое время!'),
(33515, 3, 0, '%s начинает читать заклиание "Ужасающий вопль"'),
(33515, 4, 0, '%s начинает активировать дикого защитника!');

-- 334 собрание

-- Assembly of Iron
DELETE FROM `script_texts` WHERE `npc_entry` IN (32867, 32927, 32857);
DELETE FROM `creature_text` WHERE `entry` IN (32867, 32927, 32857);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(32867, 0, 0, 'You will not defeat the Assembly of Iron so easily, invaders!', 14, 0, 100, 0, 0, 15674, 'Steelbreaker SAY_AGGRO'),
(32867, 1, 0, 'So fragile and weak!', 14, 0, 100, 0, 0, 15675, 'Steelbreaker SAY_SLAY_1'),
(32867, 1, 1, 'Flesh... such a hindrance.', 14, 0, 100, 0, 0, 15676, 'Steelbreaker SAY_SLAY_2'),
(32867, 2, 0, 'You seek the secrets of Ulduar? Then take them!', 14, 0, 100, 0, 0, 15677, 'Steelbreaker SAY_POWER'),
(32867, 3, 0, 'My death only serves to hasten your demise.', 14, 0, 100, 0, 0, 15678, 'Steelbreaker SAY_DEATH_1'),
(32867, 3, 1, 'Impossible!', 14, 0, 100, 0, 0, 15679, 'Steelbreaker SAY_DEATH_2'),
(32867, 4, 0, 'This meeting of the Assembly of Iron is adjourned!', 14, 0, 100, 0, 0, 15680, 'Steelbreaker SAY_BERSERK'),
(32927, 0, 0, 'Nothing short of total decimation will suffice.', 14, 0, 100, 0, 0, 15657, 'Molgeim SAY_AGGRO'),
(32927, 1, 0, 'The world suffers yet another insignificant loss.', 14, 0, 100, 0, 0, 15658, 'Molgeim SAY_SLAY_1'),
(32927, 1, 1, 'Death is the price of your arrogance.', 14, 0, 100, 0, 0, 15659, 'Molgeim SAY_SLAY_2'),
(32927, 2, 0, 'Decipher this!', 14, 0, 100, 0, 0, 15660, 'Molgeim SAY_RUNE_DEATH'),
(32927, 3, 0, 'Face the lightning surge!', 14, 0, 100, 0, 0, 15661, 'Molgeim SAY_SUMMON'),
(32927, 4, 0, 'The legacy of storms shall not be undone.', 14, 0, 100, 0, 0, 15662, 'Molgeim SAY_DEATH_1'),
(32927, 4, 1, 'What have you gained from my defeat? You are no less doomed, mortals!', 14, 0, 100, 0, 0, 15663, 'Molgeim SAY_DEATH_2'),
(32927, 5, 0, 'This meeting of the Assembly of Iron is adjourned!', 14, 0, 100, 0, 0, 15664, 'Molgeim SAY_BERSERK'),
(32857, 0, 0, 'Whether the world''s greatest gnats or the world''s greatest heroes, you''re still only mortal!', 14, 0, 100, 0, 0, 15684, 'Brundir SAY_AGGRO'),
(32857, 1, 0, 'A merciful kill!', 14, 0, 100, 0, 0, 15685, 'Brundir SAY_SLAY_1'),
(32857, 1, 1, 'HAH!', 14, 0, 100, 0, 0, 15686, 'Brundir SAY_SLAY_2'),
(32857, 2, 0, 'Stand still and stare into the light!', 14, 0, 100, 0, 0, 15687, 'Brundir SAY_SPECIAL'),
(32857, 3, 0, 'Let the storm clouds rise and rain down death from above!', 14, 0, 100, 0, 0, 15688, 'Brundir SAY_FLIGHT'),
(32857, 4, 0, 'The power of the storm lives on...', 14, 0, 100, 0, 0, 15689, 'Brundir SAY_DEATH_1'),
(32857, 4, 1, 'You rush headlong into the maw of madness!', 14, 0, 100, 0, 0, 15690, 'Brundir SAY_DEATH_2'),
(32857, 5, 0, 'This meeting of the Assembly of Iron is adjourned!', 14, 0, 100, 0, 0, 15691, 'Brundir SAY_BERSERK'),
(32857, 6, 0, 'Stormcaller Brundir begins to Overload!', 41, 0, 100, 0, 0, 0, 'Brundir EMOTE_OVERLOAD');

DELETE FROM `locales_creature_text` WHERE `entry` IN (32867, 32927, 32857);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(32867, 0, 0, 'Чужаки! Вам не одолеть Железное Собрание!'),
(32867, 1, 0, 'Смертные слабы и немощны!'),
(32867, 1, 1, 'Плоть... это пройденный этап.'),
(32867, 2, 0, 'Желаете знать, что скрывает Ульдуар? Я покажу вам!'),
(32867, 3, 0, 'Мое поражение лишь приблизит вашу погибель.'),
(32867, 3, 1, 'Невозможно...'),
(32867, 4, 0, 'Заседание Железного собрания объявляется закрытым!'),
(32927, 0, 0, 'Я буду спокоен, лишь когда окончательно истреблю вас.'),
(32927, 1, 0, 'Мир переживет эту несущественную потерю.'),
(32927, 1, 1, 'Смерть – это расплата за твое высокомерие.'),
(32927, 2, 0, 'Расшифруйте вот это!'),
(32927, 3, 0, 'Узрите ярость молнии!'),
(32927, 4, 0, 'Наследие бурь не умрет вместе со мной.'),
(32927, 4, 1, 'И что вам дало мое поражение? Вы все так же обречены, смертные.'),
(32927, 5, 0, 'Заседание Железного собрания объявляется закрытым!'),
(32857, 0, 0, 'Кто бы вы ни были – жалкие бродяги или великие герои… Вы всего лишь смертные!'),
(32857, 1, 0, 'Другие позавидуют твоей участи!'),
(32857, 1, 1, 'ХА!'),
(32857, 2, 0, 'Замрите и узрите СВЕТ!'),
(32857, 3, 0, 'И застелют небо грозовые тучи, и прольются смертоносным ливнем!'),
(32857, 4, 0, 'Никто не превзойдет силу шторма ...'),
(32857, 4, 1, 'Вас ждет омут безумия!'),
(32857, 5, 0, 'Заседание Железного собрания объявляется закрытым!'),
(32857, 6, 0, '%s начинает Перегрузку!');

-- 335 кологарн

DELETE FROM `creature` WHERE `id` IN (33661,33809);
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
('33661','603','3','1','11686','0','1772.19','20.1654','448.888','4.69494','180','0','0','26066','0','0','0','0','0'),
('33661','603','3','1','11686','0','1772.63','9.11681','448.888','4.69494','180','0','0','26066','0','0','0','0','0'),
('33809','603','3','1','11686','0','1776.97','-44.8396','448.888','0','180','0','0','26066','0','0','0','0','0'),
('33809','603','3','1','11686','0','1777.82','-3.50803','448.888','0','180','0','0','26066','0','0','0','0','0');

UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (33661,33809);

-- 349 ходир

DELETE FROM `script_texts` WHERE `npc_entry` = 32845;
DELETE FROM `creature_text` WHERE `entry` = 32845;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(32845, 0, 0, 'You will suffer for this trespass!', 14, 0, 100, 0, 0, 15552, 'Hodir SAY_AGGRO'),
(32845, 1, 0, 'Tragic. To come so far, only to fail.', 14, 0, 100, 0, 0, 15553, 'Hodir SAY_SLAY_1'),
(32845, 1, 1, 'Welcome to the endless winter.', 14, 0, 100, 0, 0, 15554, 'Hodir SAY_SLAY_2'),
(32845, 2, 0, 'Winds of the north consume you!', 14, 0, 100, 0, 0, 15555, 'Hodir SAY_FLASH_FREEZE'),
(32845, 3, 0, '%s roars furious.', 41, 0, 100, 0, 0, 15556, 'Hodir SAY_STALACTITE'),
(32845, 4, 0, 'I... I am released from his grasp... at last.', 14, 0, 100, 0, 0, 15557, 'Hodir SAY_DEATH'),
(32845, 5, 0, 'Enough! This ends now!', 14, 0, 100, 0, 0, 15558, 'Hodir SAY_BERSERK'),
(32845, 6, 0, '%s shatters the Rare Cache of Hodir!', 41, 0, 100, 0, 0, 0, 'Hodir SAY_HARD_MODE_FAILED'),
(32845, 7, 0, '%s begins to cast Flash Freeze!', 41, 0, 100, 0, 0, 0, 'Hodir - EMOTE_FREEZE'),
(32845, 8, 0, '%s gains Frozen Blows!', 41, 0, 100, 0, 0, 0, 'Hodir - EMOTE_BLOW');

DELETE FROM `locales_creature_text` WHERE `entry` = 32845;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(32845, 0, 0, 'Вы будете наказаны за это вторжение!'),
(32845, 1, 0, 'Как трагично. Проделать такой путь – чтобы умереть...'),
(32845, 1, 1, 'Отправляйся туда, где вечная зима.'),
(32845, 2, 0, 'Да поглотят вас северные ветра!'),
(32845, 3, 0, '%s издает яростный рев.'),
(32845, 4, 0, 'Наконец-то я... свободен от его оков ...'),
(32845, 5, 0, 'Ну хватит! Больше вам не жить!'),
(32845, 6, 0, '%s разрушает Зимние скоровища!'),
(32845, 7, 0, '%s начинает читать заклинание "Ледяная вспышка"!'),
(32845, 8, 0, '%s получает эффект "Ледяные дуновения"!');

UPDATE `creature_template` SET `faction_A` = 16, `faction_H` = 16, `speed_walk` = 1.6, `mindmg` = 509, `maxdmg` = 683, `attackpower` = 805,`unit_flags` = 32832, `mechanic_immune_mask`=1|2|4|8|16|32|64|256|512|2048|4096|8192|65536|131072|524288|4194304|8388608|33554432|67108864|536870912, `flags_extra` = 1 WHERE `entry` IN (32845, 32846);
UPDATE `creature_template` SET `dmg_multiplier` = 35 WHERE `entry` = 32845;
UPDATE `creature_template` SET `dmg_multiplier` = 70 WHERE `entry` = 32846;

UPDATE `creature_template` SET `faction_A` = 1665, `faction_H` = 1665, `unit_flags` = 32768, `mindmg` = 346, `maxdmg` = 499, `attackpower` = 287, `dmg_multiplier` = 7.5 WHERE `entry` IN (32893,32897,32900,32901,32941,32946,32948,32950,33325,33326,33327,33328,33330,33331,33332,33333);
UPDATE `creature_template` SET `unit_flags` = 393220 WHERE `entry` IN (32938,33353);

DELETE FROM `creature` WHERE `id` = 32938;
DELETE FROM `creature` WHERE `id` IN (32948,33333,33330,33331,32941,33332,32950,32946,32901,33325,32900,32893,33326,33328);

DELETE FROM `creature_template` WHERE `entry` IN (32926,33352);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`, `exp_unk`, `unit_flags2`, `type_flags2`, `mana_mod_extra`) VALUES
('32926','33352','0','0','0','0','19725','25865','0','0','Flash Freeze','','','0','80','80','2','14','14','0','1','1.14286','1','0','422','586','0','642','1','2000','0','1','0','8','0','0','0','0','0','345','509','103','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','3.1746','1','1','0','0','0','0','0','0','0','0','1','0','612597599','0','npc_flash_freeze','15595','0','2048','0','1'),
('33352','0','0','0','0','0','19725','25865','0','0','Flash Freeze (1)','','','0','80','80','2','14','14','0','1','1.14286','1','0','422','586','0','642','1','2000','0','1','393220','8','0','0','0','0','0','345','509','103','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','7.93651','1','1','0','0','0','0','0','0','0','0','1','0','612597599','0','','15595','0','2048','0','1');

UPDATE creature_template SET `faction_A` = 16, `faction_H` = 16, minlevel = 80, maxlevel = 80, unit_flags = 33587200 WHERE `entry` IN (33169,33173);
UPDATE creature_template SET `faction_A` = 14, `faction_H` = 14, minlevel = 80, maxlevel = 80, unit_flags = 33948166 WHERE `entry` IN (33174);

UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` = 194173;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10223, 10240, 10241, 10229, 10238, 10239, 10452, 10458, 10247, 10248, 10259, 10261, 10258, 10260);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10240, 11, 0, 0, 'achievement_staying_buffed_all_winter'),
(10240, 12, 0, 0, ''),
(10241, 11, 0, 0, 'achievement_staying_buffed_all_winter'),
(10241, 12, 0, 0, ''),
(10229, 11, 0, 0, 'achievement_staying_buffed_all_winter_25'),
(10229, 12, 1, 0, ''),
(10238, 11, 0, 0, 'achievement_staying_buffed_all_winter_25'),
(10238, 12, 1, 0, ''),
(10239, 11, 0, 0, 'achievement_staying_buffed_all_winter_25'),
(10239, 12, 1, 0, ''),
(10452, 11, 0, 0, 'achievement_i_could_say_that_this_cache_was_rare'),
(10452, 12, 0, 0, ''),
(10458, 11, 0, 0, 'achievement_i_could_say_that_this_cache_was_rare_25'),
(10458, 12, 1, 0, ''),
(10247, 11, 0, 0, 'achievement_getting_cold_in_here'),
(10247, 12, 0, 0, ''),
(10248, 11, 0, 0, 'achievement_getting_cold_in_here_25'),
(10248, 12, 1, 0, ''),
(10259, 11, 0, 0, 'achievement_cheese_the_freeze'),
(10259, 12, 0, 0, ''),
(10261, 11, 0, 0, 'achievement_cheese_the_freeze_25'),
(10261, 12, 1, 0, ''),
(10258, 11, 0, 0, 'achievement_i_have_the_coolest_friends'),
(10258, 12, 0, 0, ''),
(10260, 11, 0, 0, 'achievement_i_have_the_coolest_friends_25'),
(10260, 12, 1, 0, '');

UPDATE `spell_dbc` SET `Attributes` = 0, `ProcChance` = 0, `EffectImplicitTargetA1` = 22, `EffectImplicitTargetB1` = 30 WHERE `Id` = 64899;

UPDATE `gameobject_template` SET `flags` = 50 WHERE `entry` IN (194200,194201,194307,194308);
UPDATE `gameobject` SET `position_x` = 2038.29, `position_y` = -200.702, `position_z` = 432.687, `orientation` = 3.12232, `spawntimesecs` = 604800 WHERE `id` IN (194200,194201);
UPDATE `gameobject` SET `position_x` = 1966.43, `position_y` = -203.906, `position_z` = 432.687, `orientation` = -0.90757, `spawntimesecs` = 604800 WHERE `id` IN (194307,194308);

-- 350 треш

UPDATE `creature_template` SET `spell1` = 64639, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` = 34133;
UPDATE `creature_template` SET `spell1` = 64652, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34139;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34133);
INSERT INTO `smart_scripts` VALUES 
(34133, 0, 0, 0, 0, 0, 100, 2, 3000, 6000, 12000, 16000, 11, 64639, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Champion of Hodir - Cast Stomp 10'),
(34133, 0, 1, 0, 0, 0, 100, 4, 3000, 6000, 12000, 16000, 11, 64652, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Champion of Hodir - Cast Stomp 25');

UPDATE `creature_template` SET `spell1` = 64638, `AIName` = 'SmartAI', `ScriptName` = ''  WHERE `entry` = 34137;
UPDATE `creature_template` SET `spell1` = 64638 WHERE `entry` = 34140;
DELETE FROM `creature_template_addon` WHERE `entry` = 34137;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34137);
INSERT INTO `smart_scripts` VALUES 
(34137, 0, 0, 0, 0, 0, 100, 0, 3000, 6000, 6000, 9000, 11, 64638, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Winter Jormungar - Cast Acidic Bite');

UPDATE `creature_template` SET `spell1` = 64642, `spell2` = 64643, `spell3` = 64644, `mechanic_immune_mask` = 33554496, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName` = ''  WHERE `entry` = 34134;
UPDATE `creature_template` SET `spell1` = 64653, `spell2` = 64643, `spell3` = 64644, `mechanic_immune_mask` = 33554496, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 34141;
DELETE FROM `creature_template_addon` WHERE `entry` = 34134;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34134);
INSERT INTO `smart_scripts` VALUES 
(34134, 0, 0, 0, 0, 0, 100, 2, 8000, 12000, 15000, 20000, 11, 64642, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Winter Revenant - Cast Blizzard 10'),
(34134, 0, 1, 0, 0, 0, 100, 4, 8000, 12000, 15000, 20000, 11, 64653, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Winter Revenant - Cast Blizzard 25'),
(34134, 0, 2, 0, 0, 0, 100, 0, 3000, 5000, 10000, 12000, 11, 64643, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Winter Revenant - Cast Whirling Strike'),
(34134, 0, 3, 0, 0, 0, 100, 0, 15000, 20000, 60000, 75000, 11, 64644, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Winter Revenant - Cast Shield of the Winter Revenant');

UPDATE `creature_template` SET `spell1` = 64645, `spell2` = 64647, `mingold` = 7100, `maxgold` = 7600, `mechanic_immune_mask` = 33554496, `ScriptName` = '' , `AIName` = 'SmartAI' WHERE `entry` = 34135;
UPDATE `creature_template` SET `spell1` = 64645, `spell2` = 64654, `mingold` = 14200, `maxgold` = 15600, `mechanic_immune_mask` = 33554496 WHERE `entry` = 34142;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=34135);
INSERT INTO `smart_scripts` VALUES 
(34135, 0, 0, 0, 0, 0, 100, 0, 6000, 12000, 10000, 16000, 11, 64645, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Winter Rumbler - Cast Cone of Cold'),
(34135, 0, 1, 0, 0, 0, 100, 2, 3000, 6000, 8000, 12000, 11, 64647, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Winter Rumbler - Cast Snow Blindness 10'),
(34135, 0, 2, 0, 0, 0, 100, 4, 3000, 6000, 8000, 12000, 11, 64654, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Winter Rumbler - Cast Snow Blindness 25');

DELETE FROM `creature` WHERE `id` = 34137;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
('34137','603','3','1','24862','0','1847.27','-262.982','413.15','1.01229','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1921.84','-166.264','410.243','3.00197','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1883.92','-202.018','408.454','2.11185','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1940','-345.904','417.956','2.21657','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1785.64','-349.025','407.647','4.85202','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1865.86','-147.47','408.727','6.17846','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1842.28','-344.044','408.305','5.75959','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1944.57','-390.783','422.564','1.71042','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1866.67','-382.491','408.028','1.85005','604800','5','0','91231','0','1','0','0','0'),
('34137','603','3','1','24862','0','1892.89','-345.69','408.066','3.735','604800','5','0','91231','0','1','0','0','0');

-- 357 торим

DELETE FROM `spell_script_names` WHERE `spell_id` IN (62560,62016,62392,62942);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(62560, 'spell_thorim_berserk'),
(62016, 'spell_thorim_charge_orb_targeting'),
(62392, 'spell_thorim_lightning_destruction'),
(62942, 'spell_thorim_runic_fortification');

UPDATE `creature_template` SET `ScriptName`='boss_thorim' WHERE `entry`=32865;
UPDATE `creature_template` SET `unit_flags` = 320 WHERE `entry` IN (32865, 33147);
REPLACE INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(1845, 41177, 39384, 0);
UPDATE `creature_template` SET `equipment_id` = 1845 WHERE `entry` = 33147;

DELETE FROM `creature` WHERE `id` IN (33378,33196);

UPDATE `creature_template` SET `ScriptName`='npc_lightning_orb' WHERE `entry` = 33138;
DELETE FROM `waypoint_data` WHERE `id` = 33138;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`) VALUES
(33138, 1, 2227.6, -263.7, 412.2),
(33138, 2, 2227.6, -432.6, 412.2),
(33138, 3, 2199.3, -433.7, 420),
(33138, 4, 2167.5 ,-440.2, 438.5),
(33138, 5, 2134.8 ,-440.2, 438.5),
(33138, 6, 2134.8 ,-303.4, 438.5);

UPDATE `creature_template` SET `faction_A` = 1692, `faction_H` = 1692 WHERE `entry` IN (32886, 33159);

UPDATE `creature_template` SET `ScriptName`='npc_thorim_controller' WHERE `entry` = 32879;
DELETE FROM `creature` WHERE `id` = 32879;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(32879, 603, 3, 1, 0, 0, 2134.77, -262.307, 420.694, 1.3439, 604800, 0, 0, 12600, 0, 0, 0, 0, 0);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=62042;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(62042, 62470, 1, 'Stormhammer => Deafening Thunder');

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_stormhammer_targeting';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(62042, 'spell_stormhammer_targeting');

DELETE FROM `conditions` WHERE `SourceEntry`=62016;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 62016, 31, 3, 33378, 'Lightning Shock');

UPDATE `creature_template` SET `ScriptName`='npc_thorim_pre_phase_add' WHERE `entry` IN (32885,32883,32908,32907,32882,32886);
REPLACE INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(1849, 42345, 42524, 0),
(1850, 42351, 42569, 0),
(1851, 42389, 0, 0),
(1852, 39745, 0, 0);
UPDATE `creature_template` SET `equipment_id` = 1849 WHERE `entry`=33152;
UPDATE `creature_template` SET `equipment_id` = 1850 WHERE `entry` IN (32885,33153);
UPDATE `creature_template` SET `equipment_id` = 1851 WHERE `entry` IN (32908,33151);
UPDATE `creature_template` SET `equipment_id` = 1852 WHERE `entry` = 33150;
DELETE FROM `creature` WHERE `id` IN (32882,32908,32885,32886,32907,32883); 

UPDATE `creature_template` SET `mechanic_immune_mask`=1|2|4|8|16|64|256|512|1024|2048|4096|8192|65536|131072|524288|4194304|8388608|33554432|67108864|536870912, `flags_extra`=1, `ScriptName`='npc_runic_colossus' WHERE `entry`=32872;
UPDATE `creature_template` SET `mechanic_immune_mask`= 1|2|4|8|16|64|256|512|1024|2048|4096|8192|65536|131072|524288|4194304|8388608|33554432|67108864|536870912, `flags_extra`=1, `ScriptName`='npc_ancient_rune_giant' WHERE `entry`=32873;
UPDATE `creature_template` SET `mechanic_immune_mask`= 1|2|4|8|16|64|256|512|1024|2048|4096|8192|65536|131072|524288|4194304|8388608|33554432|67108864|536870912, `flags_extra`=1, `ScriptName`='npc_sif' WHERE `entry`=33196;

UPDATE `creature_template` SET `mechanic_immune_mask`=1|2|4|8|16|64|256|512|1024|2048|4096|8192|65536|131072|524288|4194304|8388608|33554432|67108864|536870912 WHERE `entry` IN (33149, 33148, 33196);
UPDATE `creature_template` SET `ScriptName`='npc_thorim_arena_phase_add' WHERE `entry` IN (32876, 32904, 32878, 32877, 32874, 32875, 33110);

-- DELETE FROM `creature_addon` WHERE `guid` IN (131817, 131934);
-- INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
-- (131817, 0, 0, 0, 1, 0, '40775'),
-- (131934, 0, 0, 0, 1, 0, '40775');

DELETE FROM `creature` WHERE `id` IN (33140, 33141); 
UPDATE `creature_template` SET `ScriptName`='npc_runic_smash' WHERE `entry` IN (33140, 33141);

UPDATE `gameobject_template` SET `ScriptName`='go_thorim_lever' WHERE `entry`=194264;
UPDATE `gameobject_template` SET `size`=3, `data2`=6000, `flags`=16|32, `ScriptName`='go_thorim_lever' WHERE `entry`  =179148;
-- UPDATE `gameobject` SET `id` = 179148 WHERE `guid` = 5504;

DELETE FROM `script_texts` WHERE `npc_entry` IN (33413, 32865, 32872);
DELETE FROM `creature_text` WHERE `entry` IN (33413, 32865, 32872);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(32865, 0, 0, 'Interlopers! You mortals who dare to interfere with my sport will pay... Wait--you...', 14, 0, 100, 0, 0, 15733, 'Thorim SAY_AGGRO_1'),
(32865, 1, 0, 'I remember you... In the mountains... But you... what is this? Where am\-\-', 14, 0, 100, 0, 0, 15734, 'Thorim SAY_AGGRO_2'),
(32865, 2, 0, 'Behold the power of the storms and despair!', 14, 0, 100, 0, 0, 15735, 'Thorim SAY_SPECIAL_1'),
(32865, 2, 1, 'Do not hold back! Destroy them!', 14, 0, 100, 0, 0, 15736, 'Thorim SAY_SPECIAL_2'),
(32865, 2, 2, 'Have you begun to regret your intrusion?', 14, 0, 100, 0, 0, 15737, 'Thorim SAY_SPECIAL_3'),
(32865, 3, 0, 'Impertinent whelps! You dare challenge me atop my pedestal! I will crush you myself!', 14, 0, 100, 0, 0, 15738, 'Thorim SAY_JUMPDOWN'),
(32865, 4, 0, 'Can''t you at least put up a fight!?', 14, 0, 100, 0, 0, 15739, 'Thorim SAY_SLAY_1'),
(32865, 4, 1, 'Pathetic!', 14, 0, 100, 0, 0, 15740, 'Thorim SAY_SLAY_2'),
(32865, 5, 0, 'My patience has reached its limit!', 14, 0, 100, 0, 0, 15741, 'Thorim SAY_BERSERK'),
(32865, 6, 0, 'Failures! Weaklings!', 14, 0, 100, 0, 0, 15742, 'Thorim SAY_WIPE'),
(32865, 7, 0, 'Stay your arms! I yield!', 14, 0, 100, 0, 0, 15743, 'Thorim SAY_DEATH'),
(32865, 8, 0, 'I feel as though I am awakening from a nightmare, but the shadows in this place yet linger.', 14, 0, 100, 0, 0, 15744, 'Thorim SAY_END_NORMAL_1'),
(32865, 8, 1, 'Sif... was Sif here? Impossible\-\-she died by my brother''s hand. A dark nightmare indeed....', 14, 0, 100, 0, 0, 15745, 'Thorim SAY_END_NORMAL_2'),
(32865, 8, 2, 'I need time to reflect.... I will aid your cause if you should require it. I owe you at least that much. Farewell.', 14, 0, 100, 0, 0, 15746, 'Thorim SAY_END_NORMAL_3'),
(32865, 9, 0, 'You! Fiend! You are not my beloved! Be gone!', 14, 0, 100, 0, 0, 15747, 'Thorim SAY_END_HARD_1'),
(32865, 9, 1, 'Behold the hand behind all the evil that has befallen Ulduar! Left my kingdom in ruins, corrupted my brother and slain my wife!', 14, 0, 100, 0, 0, 15748, 'Thorim SAY_END_HARD_2'),
(32865, 9, 2, 'And now it falls to you, champions, to avenge us all! The task before you is great, but I will lend you my aid as I am able. You must prevail!', 14, 0, 100, 0, 0, 15749, 'Thorim SAY_END_HARD_3'),
(32872, 0, 0, 'Runic Colossus surrounds itself with a crackling Runic Barrier!', 41, 0, 100, 0, 0, 0, 'Runic Colossus EMOTE_BARRIER');

UPDATE `creature` SET `spawnMask` = 3 WHERE `id` = 32873;

UPDATE `gameobject_template` SET `data1` = 194312 WHERE `entry` = 194312;
UPDATE `gameobject_template` SET `data1` = 194313 WHERE `entry` = 194313;
UPDATE `gameobject_template` SET `data1` = 194314 WHERE `entry` = 194314;
UPDATE `gameobject_template` SET `data1` = 194315 WHERE `entry` = 194315;

UPDATE `gameobject_template` SET `flags` = 50 WHERE `entry` IN (194558,194557);
UPDATE `gameobject_template` SET `flags` = 48 WHERE `entry` = 194560;

UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `faction_A` = 16, `faction_H` = 16, `unit_flags` = 33587200 WHERE `entry` IN (33196,33234);

-- 364 треш

UPDATE `creature_template` SET `spell1` = 63541, `spell2` = 63630, `mingold` = 7100, `maxgold` = 7600, `mechanic_immune_mask` = 33554496, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` IN (33722, 33699);
UPDATE `creature_template` SET `spell1` = 63541, `spell2` = 63630, `mingold` = 14200, `maxgold` = 15600, `mechanic_immune_mask` = 33554496 WHERE `entry` IN (33723, 33700);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (33722, 33699);
INSERT INTO `smart_scripts` VALUES
(33722, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 75, 63630, 33699, 0, 0, 0, 0, 11, 33699, 50, 0, 0, 0, 0, 0,'Storm Tempered Keeper - Cast Vengeful Surge on death'),
(33722, 0, 1, 0, 0, 0, 100, 0, 3000, 6000, 10000, 15000, 11, 63541, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Storm Tempered Keeper - Cast Forked Lightning'),
(33722, 0, 2, 0, 0, 0, 100, 0, 3000, 3000, 3000, 3000, 11, 63539, 0, 0, 0, 0, 0, 9, 33699, 15, 50, 0, 0, 0, 0,'Storm Tempered Keeper - Cast Separation Anxiety if separated'),
(33699, 0, 0, 0, 6, 0, 100, 0, 0, 0, 0, 0, 75, 63630, 33722, 0, 0, 0, 0, 11, 33722, 50, 0, 0, 0, 0, 0,'Storm Tempered Keeper - Cast Vengeful Surge on death'),
(33699, 0, 1, 0, 0, 0, 100, 0, 3000, 6000, 10000, 15000, 11, 63541, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Storm Tempered Keeper - Cast Forked Lightning'),
(33699, 0, 2, 0, 0, 0, 100, 0, 3000, 3000, 3000, 3000, 11, 63539, 0, 0, 0, 0, 0, 9, 33722, 15, 50, 0, 0, 0, 0,'Storm Tempered Keeper - Cast Separation Anxiety if separated');

UPDATE `creature_template` SET `ScriptName`='', `AIName`='SmartAI' WHERE `entry`=33755;
DELETE FROM `smart_scripts` WHERE `entryorguid`=33755;
INSERT INTO `smart_scripts` (`entryorguid`, `event_type`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
(33755, 0, 6, 2000, 2000, 2000, 2000, 11, 63615, 2, 'Dark Rune Ravager - Cast Ravage Armor');

UPDATE `creature_template` SET `ScriptName`='', `AIName`='SmartAI' WHERE `entry`=33754;
DELETE FROM `smart_scripts` WHERE `entryorguid`=33754;
INSERT INTO `smart_scripts` (`entryorguid`, `id`, `event_type`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `target_type`, `comment`) VALUES
(33754, 0, 0, 2, 3000, 3000, 2000, 3000, 11, 63612, 2, 'Dark Rune Thunderer - Cast Lightning Brand'),
(33754, 1, 0, 4, 3000, 3000, 2000, 3000, 11, 63673, 2, 'Dark Rune Thunderer - Cast Lightning Brand');

UPDATE `creature_template` SET `spell1` = 63007, `spell2` = 63047, `mingold` = 7100, `maxgold` = 7600, `mechanic_immune_mask` = 33554496, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33430;
UPDATE `creature_template` SET `spell1` = 63007, `spell2` = 63550, `mingold` = 14200, `maxgold` = 15600, `mechanic_immune_mask` = 33554496 WHERE `entry` = 33732;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33430);
INSERT INTO `smart_scripts` VALUES
(33430, 0, 0, 0, 4, 0, 100, 0, 0, 0, 0, 0, 75, 63007, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Guardian Lasher - Cast Guardian Pheromones on aggro'),
(33430, 0, 1, 0, 0, 0, 100, 2, 3000, 6000, 8000, 12000, 11, 63047, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Guardian Lasher - Cast Guardian''s Lash 10'),
(33430, 0, 2, 0, 0, 0, 100, 4, 3000, 6000, 8000, 12000, 11, 63550, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Guardian Lasher - Cast Guardian''s Lash 25');

UPDATE `creature_template` SET `spell1` = 63059, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33431;
UPDATE `creature_template` SET `spell1` = 63059, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 33731;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33431);
INSERT INTO `smart_scripts` VALUES
(33431, 0, 0, 0, 0, 0, 100, 0, 3000, 9000, 10000, 20000, 11, 63059, 0, 0, 0, 0, 0, 11, 33430, 40, 0, 0, 0, 0, 0,'Forest Swarmer - Cast Pollinate');
DELETE FROM conditions WHERE SourceEntry = 63059;

UPDATE `creature_template` SET `spell1` = 63226, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33528;
UPDATE `creature_template` SET `spell1` = 63551, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 33733;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33528);
INSERT INTO `smart_scripts` VALUES
(33528, 0, 0, 0, 0, 0, 100, 2, 3000, 9000, 15000, 20000, 11, 63226, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Guardian of Life - Cast Poison Breath 10'),
(33528, 0, 1, 0, 0, 0, 100, 4, 3000, 9000, 15000, 20000, 11, 63551, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Guardian of Life - Cast Poison Breath 25');

UPDATE `creature_template` SET `spell1` = 63247, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33527;
UPDATE `creature_template` SET `spell1` = 63568, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 33741;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33527);
INSERT INTO `smart_scripts` VALUES
(33527, 0, 0, 0, 0, 0, 100, 2, 3000, 9000, 18000, 24000, 11, 63247, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Nature Blade - Cast Living Tsunami 10'),
(33527, 0, 1, 0, 0, 0, 100, 4, 3000, 9000, 18000, 24000, 11, 63568, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Nature Blade - Cast Living Tsunami 25');

UPDATE `creature_template` SET `spell1` = 63240, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33526;
UPDATE `creature_template` SET `spell1` = 63553, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 33734;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33526);
INSERT INTO `smart_scripts` VALUES
(33526, 0, 0, 0, 0, 0, 100, 2, 3000, 8000, 12000, 16000, 11, 63240, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Ironroot Lasher - Cast Ironroot Thorns 10'),
(33526, 0, 1, 0, 0, 0, 100, 4, 3000, 8000, 12000, 16000, 11, 63553, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Ironroot Lasher - Cast Ironroot Thorns 25');

UPDATE `creature_template` SET `spell1` = 63272, `spell2` = 63242, `spell3` = 63241, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33525;
UPDATE `creature_template` SET `spell1` = 63272, `spell2` = 63556, `spell3` = 63554, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 33735;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33525);
INSERT INTO `smart_scripts` VALUES
(33525, 0, 0, 0, 0, 0, 100, 0, 8000, 12000, 16000, 20000, 11, 63272, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Mangrove Ent - Cast Hurricane'),
(33525, 0, 1, 0, 0, 0, 100, 2, 4000, 6000, 12000, 16000, 11, 63242, 0, 0, 0, 0, 0, 11, 0, 30, 0, 0, 0, 0, 0,'Mangrove Ent - Cast Nourish 10'),
(33525, 0, 2, 0, 0, 0, 100, 4, 4000, 6000, 12000, 16000, 11, 63556, 0, 0, 0, 0, 0, 11, 0, 30, 0, 0, 0, 0, 0,'Mangrove Ent - Cast Nourish 25'),
(33525, 0, 3, 0, 0, 0, 100, 2, 20000, 25000, 20000, 25000, 11, 63241, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Mangrove Ent - Cast Tranquility 10'),
(33525, 0, 4, 0, 0, 0, 100, 4, 20000, 25000, 20000, 25000, 11, 63554, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Mangrove Ent - Cast Tranquility 25');

UPDATE `creature_template` SET `spell1` = 63082, `spell2` = 63111, `spell3` = 63136, `mingold` = 7100, `maxgold` = 7600, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33355;
UPDATE `creature_template` SET `spell1` = 63559, `spell2` = 63562, `spell3` = 63564, `mingold` = 14200, `maxgold` = 15600 WHERE `entry` = 33737;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33355);
INSERT INTO `smart_scripts` VALUES
(33355, 0, 0, 0, 0, 0, 100, 2, 8000, 12000, 16000, 20000, 11, 63082, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Misguided Nymph - Cast Bind Life 10'),
(33355, 0, 1, 0, 0, 0, 100, 4, 8000, 12000, 16000, 20000, 11, 63559, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Misguided Nymph - Cast Bind Life 25'),
(33355, 0, 2, 0, 0, 0, 100, 2, 4000, 6000, 12000, 16000, 11, 63111, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Misguided Nymph - Cast Frost Spear 10'),
(33355, 0, 3, 0, 0, 0, 100, 4, 4000, 6000, 12000, 16000, 11, 63562, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Misguided Nymph - Cast Frost Spear 25'),
(33355, 0, 4, 0, 0, 0, 100, 2, 15000, 20000, 15000, 20000, 11, 63136, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Misguided Nymph - Cast Winter''s Embrace 10'),
(33355, 0, 5, 0, 0, 0, 100, 4, 15000, 20000, 15000, 20000, 11, 63564, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,'Misguided Nymph - Cast Winter''s Embrace 25');

UPDATE `creature_template` SET `spell1` = 63169, `spell2` = 63149, `mingold` = 7100, `maxgold` = 7600, `mechanic_immune_mask` = 33554496, `AIName` = 'SmartAI', `ScriptName`='' WHERE `entry` = 33354;
UPDATE `creature_template` SET `spell1` = 63549, `spell2` = 63149, `mingold` = 14200, `maxgold` = 15600, `mechanic_immune_mask` = 33554496 WHERE `entry` = 33729;
DELETE FROM `smart_scripts` WHERE (`entryorguid`=33354);
INSERT INTO `smart_scripts` VALUES
(33354, 0, 0, 0, 0, 0, 100, 2, 2000, 4000, 16000, 20000, 11, 63169, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Corrupted Servitor - Cast Petrify Joints 10'),
(33354, 0, 1, 0, 0, 0, 100, 4, 2000, 4000, 16000, 20000, 11, 63549, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,'Corrupted Servitor - Cast Petrify Joints 25'),
(33354, 0, 2, 0, 0, 0, 100, 0, 6000, 8000, 12000, 16000, 11, 63149, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0,'Corrupted Servitor - Cast Violent Earth');

-- 365

UPDATE `creature_model_info` SET `bounding_radius` = 1.085, `combat_reach` = 10.5 WHERE `modelid` = 29185;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN ('10430','10431');
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
('10430','11','0','0','achievement_ignis_hot_pocket'),
('10430','12','0','0',''),
('10431','11','0','0','achievement_ignis_hot_pocket'),
('10431','12','1','0','');

UPDATE `creature_template` SET `mechanic_immune_mask`=1|2|4|8|16|32|64|256|512|2048|4096|8192|65536|131072|524288|4194304|8388608|33554432|67108864|536870912 WHERE `entry` IN (32930, 33909);

UPDATE `creature_template` SET `unit_flags` = 0 WHERE `entry` IN (32933,33910);

-- 366 левиафан

DELETE FROM `gameobject` WHERE `id` IN (194261,194262);
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
('194261','603','3','1','130.553','-128.063','409.804','2.87979','0','0','0','1','180','255','1'),
('194261','603','3','1','138.516','56.1078','409.804','2.87979','0','0','0','1','180','255','1'),
('194262','603','3','1','131.895','-128.264','409.804','0','0','0','0','1','180','255','1'),
('194262','603','3','1','139.857','55.9064','409.804','0','0','0','0','1','180','255','1');

UPDATE `creature_template` SET `unit_flags`=2 WHERE `entry`=33167;
UPDATE `creature_template` SET `VehicleId`=338 WHERE `entry`=33109;

DELETE FROM `vehicle_template_accessory` WHERE `entry`=33109;
INSERT INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `minion`, `description`, `summontype`, `summontimer`) VALUES
(33109, 33167, 2, 1, 'Salvaged Demolisher', 6, 30000);

UPDATE `creature_template` SET `ScriptName`='npc_liquid_pyrite' WHERE `entry` = 33189;

UPDATE `creature_template` SET `RegenHealth` = 0 WHERE `entry` IN (33062,33060,33109);

UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_ironwork_cannon' WHERE `entry` = 33264;

-- 376 (исправить)

DELETE FROM `creature` WHERE `id` = 33121;

-- UPDATE `creature` SET `spawnMask` = 3 WHERE guid IN (131946,131947);

DELETE FROM `creature` WHERE `id`=33282 AND `phaseMask`=2;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(33282, 603, 2, 1, 11686, 0, 560.46, -139.56, 391.51, 5.15, 180, 0, 0, 1, 0, 0, 0, 0, 0),
(33282, 603, 2, 1, 11686, 0, 606.85, -135.94, 391.41, 4.26, 180, 0, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `spell_script_names` WHERE `spell_id` = 62669;
INSERT INTO `spell_script_names` VALUES
(62669, 'spell_razorscale_firebolt');

DELETE FROM `conditions` WHERE `SourceEntry` = 62669;
INSERT INTO `conditions` VALUES
(13, 1, 62669, 0, 0, 31, 0, 3, 33282, 0, 0, 0, '', NULL);

-- 382 торим 

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10289,10314,10440,10457) AND `type` = 0;
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10289,10314,10440,10457) AND `type` = 18;
REPLACE INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10440, 11, 0, 0, 'achievement_lose_illusion'),
(10457, 11, 0, 0, 'achievement_lose_illusion_25');

DELETE FROM `spell_dbc` WHERE `id` = 64980;
INSERT INTO `spell_dbc` (`Id`, `CastingTimeIndex`, `RangeIndex`, `EquippedItemClass`, `Effect1`, `EffectImplicitTargetA1`, `EffectImplicitTargetB1`, `EffectRadiusIndex1`, `Comment`) VALUES
(64980, 1, 1, -1, 3, 22, 15, 28, 'Thorim achievement credit marker');

UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `minlevel` = 80, `maxlevel` = 80 WHERE `entry` IN (33140,33141);
UPDATE `creature_template` SET `mechanic_immune_mask` = 0 WHERE `entry` IN (32877,33155);
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `minlevel` = 80, `maxlevel` = 80, `speed_walk` = 9.6, `speed_run` = 1.14286 WHERE `entry` IN (33138,33378);

-- 409

UPDATE `creature_template` SET `ScriptName` = 'npc_razorscale_harpoon_fire_state' WHERE `entry` = 33282;

-- 460 ходир

UPDATE `creature_template` SET `mechanic_immune_mask` = 617299839 WHERE `entry` IN (32926,33352,32938,33353);

-- 469 мимирон

UPDATE `gameobject_template` SET `flags`=32, `data2`=3000, `ScriptName`='go_call_tram' WHERE `entry` IN (194914, 194912, 194437);
DELETE FROM `gameobject_template` WHERE `entry` = 194438;
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`, `WDBVerified`) VALUES
('194438','1','8504','Activate Tram','','','','0','32','1','0','0','0','0','0','0','0','0','3000','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','go_call_tram','15595');

DELETE FROM `gameobject` WHERE `id` IN (194914, 194912, 194437, 194438);
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(194437, 603, 3, 1, 2306.87, 274.237, 424.288, 1.52255, 0, 0, 0.689847, 0.723956, 300, 0, 1),
(194914, 603, 3, 1, 2277.42, 298.693, 419.246, 3.14159, 0, 0, 0, 1, 180, 255, 1),
(194912, 603, 3, 1, 2338.32, 2565.08, 419.246, 0, 0, 0, 0, 1, 180, 255, 1),
(194438, 603, 3, 1, 2306.99, 2589.35, 424.382, 4.71676, 0, 0, 0.705559, -0.708651, 300, 0, 1);

DELETE FROM `locales_gameobject` WHERE `entry` = 194438;
INSERT INTO `locales_gameobject` (`entry`, `name_loc8`) VALUES
(194438, 'Отправка поезда');

-- 473 фрея

DELETE FROM `spell_dbc` WHERE `id` IN (65296);
INSERT INTO `spell_dbc` (`Id`, `Attributes`, `AttributesEx`, `AttributesEx2`,`AttributesEx3`,`AttributesEx4`,`AttributesEx5`,`AttributesEx6`,`Effect1`,`EffectImplicitTargetA1`,`EffectImplicitTargetB1`,`comment`) VALUES
(65296, 0x09800100, 0x00000420, 0x00004005, 0x10040000, 0x00000080, 0x00000008, 0x00001000, 3, 1, 0, 'Ulduar - Lumberjacked - Achievement Check');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10720,10721) AND `type` = 0;
DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10720,10721) AND `type` = 18;
INSERT INTO `achievement_criteria_data` VALUES
(10720, 18, 0, 0, ''),
(10721, 18, 0, 0, '');

-- 489 мимирон

UPDATE `creature_template` SET `ScriptName`='boss_mimiron' WHERE `entry`=33350;

UPDATE `creature_template` SET `vehicleid`=370, `ScriptName`='boss_leviathan_mk' WHERE `entry`=33432;
UPDATE `creature_template` SET `vehicleid`=370, `flags_extra`=`flags_extra` |1 WHERE `entry`=34106;
UPDATE `creature` SET `spawntimesecs`=604800 WHERE `id`=33236;

UPDATE `creature_template` SET `ScriptName`='boss_leviathan_mk_turret' WHERE `entry`=34071;
DELETE FROM `vehicle_template_accessory` WHERE `entry`=33432;
INSERT INTO `vehicle_template_accessory` VALUES
(33432, 34071, 3, 1, 'Leviathan Mk II turret', 8, 0);
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=33432;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(33432, 46598, 1, 0);

UPDATE `creature_template` SET `ScriptName`='npc_proximity_mine' WHERE `entry`=34362;
UPDATE `creature_model_info` SET `bounding_radius`=0.5, `combat_reach`=7 WHERE `modelid` IN (28831, 28841, 28979);

UPDATE `creature_template` SET `flags_extra`=`flags_extra` |1, `vehicleid`=371, `ScriptName`='boss_vx_001' WHERE `entry`=33651;
UPDATE `creature_template` SET `minlevel`=83, `maxlevel`=83, `flags_extra`=`flags_extra` |1 WHERE `entry`=34108;
UPDATE `creature_template` SET `faction_A`=35, `faction_H`=35 WHERE `entry`=34050;
UPDATE `creature_template` SET `unit_flags`=4|512|131072|33554432, `flags_extra`=`flags_extra` |2 WHERE `entry`=34211;
UPDATE `creature_template` SET `ScriptName`='npc_rocket_strike' WHERE `entry`=34047;

UPDATE `creature_template` SET `flags_extra`=`flags_extra` |1, `ScriptName`='boss_aerial_unit', `vehicleid`=372 WHERE `entry`=33670;
UPDATE `creature_template` SET `minlevel`=83, `maxlevel`=83, `flags_extra`=1 WHERE `entry`=34109;
UPDATE `creature_template` SET `ScriptName`='npc_magnetic_core' WHERE `entry`=34068;
UPDATE `creature_template` SET `ScriptName`='npc_assault_bot' WHERE `entry`=34057;
UPDATE `creature_template` SET `difficulty_entry_1`=34148, `ScriptName`='npc_emergency_bot' WHERE `entry`=34147;

UPDATE `gameobject_template` SET `flags`=`flags` &~16, `ScriptName`='go_mimiron_hard_mode_activator' WHERE `entry`=194739;
UPDATE `creature_template` SET `difficulty_entry_1`=34361, `ScriptName`='npc_frost_bomb' WHERE `entry`=34149;
UPDATE `creature_template` SET `speed_walk`=0.15, `speed_run`=0.15, `ScriptName`='npc_mimiron_flame_trigger' WHERE `entry` =34363;
UPDATE `creature_template` SET `ScriptName`='npc_mimiron_flame_spread' WHERE `entry`=34121;
UPDATE `creature_template` SET `ScriptName`='npc_mimiron_bomb_bot' WHERE `entry`=33836;
UPDATE `creature_template` SET `ScriptName`='npc_boomer_xp' WHERE `entry`=34192;

DELETE FROM `creature` WHERE `id`=34071;
UPDATE `creature_template` SET `unit_flags`=`unit_flags` |2|33554432 WHERE `entry`=34143;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10450, 10463);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10450, 12, 0, 0, ''),
(10450, 11, 0, 0, 'achievement_firefighter'),
(10463, 12, 1, 0, ''),
(10463, 11, 0, 0, 'achievement_firefighter_25');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (63382, 63016, 63027);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(63382, 'spell_rapid_burst');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(63027, 'spell_proximity_mines');

DELETE FROM `script_texts` WHERE `npc_entry` IN (33350, 33432);
DELETE FROM `creature_text` WHERE `entry` IN (33350, 33432);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33350, 0, 0, 'Oh, my! I wasn''t expecting company! The workshop is such a mess! How embarrassing!', 14, 0, 100, 0, 0, 15611, 'Mimiron SAY_AGGRO'),
(33350, 1, 0, 'Now why would you go and do something like that? Didn''t you see the sign that said ''DO NOT PUSH THIS BUTTON!''? How will we finish testing with the self-destruct mechanism active?', 14, 0, 100, 0, 0, 15629, 'Mimiron SAY_HARDMODE_ON'),
(33350, 2, 0, 'We haven''t much time, friends! You''re going to help me test out my latest and greatest creation. Now, before you change your minds, remember, that you kind of owe it to me after the mess you made with the XT-002.', 14, 0, 100, 0, 0, 15612, 'Mimiron SAY_MKII_ACTIVATE'),
(33350, 3, 0, 'MEDIC!', 14, 0, 100, 0, 0, 15613, 'Mimiron SAY_MKII_SLAY_1'),
(33350, 3, 1, 'I can fix that... or, maybe not! Sheesh, what a mess...', 14, 0, 100, 0, 0, 15614, 'Mimiron SAY_MKII_SLAY_2'),
(33350, 4, 0, 'WONDERFUL! Positively marvelous results! Hull integrity at 98.9 percent! Barely a dent! Moving right along.', 14, 0, 100, 0, 0, 15615, 'Mimiron SAY_MKII_DEATH'),
(33350, 5, 0, 'Behold the VX-001 Anti-personnel Assault Cannon! You might want to take cover.', 14, 0, 100, 0, 0, 15616, 'Mimiron SAY_VX001_ACTIVATE'),
(33350, 6, 0, 'Fascinating. I think they call that a "clean kill".', 14, 0, 100, 0, 0, 15617, 'Mimiron SAY_VX001_SLAY_1'),
(33350, 6, 1, 'Note to self: Cannon highly effective against flesh.', 14, 0, 100, 0, 0, 15618, 'Mimiron SAY_VX001_SLAY_2'),
(33350, 7, 0, 'Thank you, friends! Your efforts have yielded some fantastic data! Now, where did I put- oh, there it is!', 14, 0, 100, 0, 0, 15619, 'Mimiron SAY_VX001_DEATH'),
(33350, 8, 0, 'Isn''t it beautiful? I call it the magnificent aerial command unit!', 14, 0, 100, 0, 0, 15620, 'Mimiron SAY_AERIAL_ACTIVATE'),
(33350, 9, 0, 'Outplayed!', 14, 0, 100, 0, 0, 15621, 'Mimiron SAY_AERIAL_SLAY_1'),
(33350, 9, 1, 'You can do better than that!', 14, 0, 100, 0, 0, 15622, 'Mimiron SAY_AERIAL_SLAY_2'),
(33350, 10, 0, 'Preliminary testing phase complete. Now comes the true test!!', 14, 0, 100, 0, 0, 15623, 'Mimiron SAY_AERIAL_DEATH'),
(33350, 11, 0, 'Gaze upon its magnificence! Bask in its glorious, um, glory! I present you... V-07-TR-0N!', 14, 0, 100, 0, 0, 15624, 'Mimiron SAY_V07TRON_ACTIVATE'),
(33350, 12, 0, 'Prognosis: Negative!', 14, 0, 100, 0, 0, 15625, 'Mimiron SAY_V07TRON_SLAY_1'),
(33350, 12, 1, 'You''re not going to get up from that one, friend.', 14, 0, 100, 0, 0, 15626, 'Mimiron SAY_V07TRON_SLAY_2'),
(33350, 13, 0, 'It would appear that I''ve made a slight miscalculation. I allowed my mind to be corrupted by the fiend in the prison, overriding my primary directive. All systems seem to be functional now. Clear.', 14, 0, 100, 0, 0, 15627, 'Mimiron SAY_V07TRON_DEATH'),
(33350, 14, 0, 'Oh, my! It would seem that we are out of time, my friends!', 14, 0, 100, 0, 0, 15628, 'Mimiron SAY_BERSERK'),
(33432, 0, 0, 'Leviathan MK II begins to cast Plasma Blast!', 41, 0, 100, 0, 0, 0, 'Leviathan MK II EMOTE_PLASMA_BLAST');

UPDATE `creature` SET `phaseMask` = 1, `position_x` = 2742.53, position_y = 2560.99, position_z = 364.397, orientation = 1.39626 WHERE id = 33350;
UPDATE `creature` SET `spawnMask` = 3 WHERE `id` = 33432;
UPDATE `creature_template` SET `unit_flags` = 33554432 WHERE `entry` = 34050;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (34071,34149,34361,34121,34211,34050,34363,34149,34361);
UPDATE `creature_template` SET `flags_extra` = 0x00000002 WHERE `entry` = 34143;
UPDATE `gameobject_template` SET faction = 0, data0 = 1634, data3 = 1, data11 = 1, data13 = 1, data15 = 1, WDBVerified = 12340 WHERE entry IN (194789,194957,194956,194958);

-- 490

UPDATE `gameobject_template` SET `faction` = 0, `flags` = 34 WHERE `entry` = 194255;
UPDATE `gameobject` SET `animprogress` = 255 WHERE `id` = 194255;

-- 496 везакс

DELETE FROM `spell_script_names` WHERE `spell_id` IN (62692, 63276, 63278, 63323, 64623);
INSERT INTO `spell_script_names` VALUES
(62692,'spell_aura_of_despair_aura'),
(63276,'spell_mark_of_the_faceless'),
(63278,'spell_mark_of_the_faceless_drain'),
(63323,'spell_saronite_vapors'),
(64623,'spell_frost_bomb');

DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=63277;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(63277, 65269, 2, 'Shadow Crash auras');

UPDATE `creature_template` SET `flags_extra`=`flags_extra` |1 WHERE `entry`=33524;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10451, 10462, 10173, 10306, 10543, 10544, 10545, 10546, 10547, 10548);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(10451, 11, 0, 0, 'achievement_i_love_the_smell_of_saronite_in_the_morning'),
(10451, 12, 0, 0, ''),
(10462, 11, 0, 0, 'achievement_i_love_the_smell_of_saronite_in_the_morning_25'),
(10462, 12, 1, 0, ''),
(10173, 11, 0, 0, 'achievement__shadowDodger'),
(10173, 12, 0, 0, ''),
(10306, 11, 0, 0, 'achievement__shadowDodger_25'),
(10306, 12, 1, 0, ''),
(10543, 11, 0, 0, 'achievement_set_up_us_the_bomb__boombot'),
(10543, 12, 0, 0, ''),
(10544, 11, 0, 0, 'achievement_set_up_us_the_bomb__proximity'),
(10544, 12, 0, 0, ''),
(10545, 11, 0, 0, 'achievement_set_up_us_the_bomb__rockets'),
(10545, 12, 0, 0, ''),
(10546, 11, 0, 0, 'achievement_set_up_us_the_bomb__boombot_25'),
(10546, 12, 1, 0, ''),
(10547, 11, 0, 0, 'achievement_set_up_us_the_bomb__proximity_25'),
(10547, 12, 1, 0, ''),
(10548, 11, 0, 0, 'achievement_set_up_us_the_bomb__rockets_25'),
(10548, 12, 1, 0, '');

DELETE FROM `script_texts` WHERE `npc_entry` IN (33271, 33488);
DELETE FROM `creature_text` WHERE `entry` IN (33271, 33488);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33271, 0, 0, 'Your destruction will herald a new age of suffering!', 14, 0, 100, 0, 0, 15542, 'General Vezax SAY_AGGRO'),
(33271, 1, 0, 'You thought to stand before the legions of death... and survive?', 14, 0, 100, 0, 0, 15543, 'General Vezax SAY_SLAY_1'),
(33271, 1, 1, 'Defiance... a flaw of mortality.', 14, 0, 100, 0, 0, 15544, 'General Vezax SAY_SLAY_2'),
(33271, 2, 0, 'The black blood of Yogg-Saron courses through me! I. AM. UNSTOPPABLE!', 14, 0, 100, 0, 0, 15545, 'General Vezax SAY_SURGE_DARKNESS'),
(33271, 3, 0, 'Oh, what horrors await....', 14, 0, 100, 0, 0, 15546, 'General Vezax SAY_DEATH'),
(33271, 4, 0, 'Your defeat was inevitable!', 14, 0, 100, 0, 0, 15547, 'General Vezax SAY_BERSERK'),
(33271, 5, 0, 'Behold, now! Terror, absolute!', 14, 0, 100, 0, 0, 15548, 'General Vezax SAY_HARDMODE_ON'),
(33271, 6, 0, 'The saronite vapors mass and swirl violently, merging into a monstrous form!', 41, 0, 100, 0, 0, 0, 'General Vezax - EMOTE_ANIMUS'),
(33271, 7, 0, 'A saronite barrier appears around General Vezax!', 41, 0, 100, 0, 0, 0, 'General Vezax - EMOTE_BARRIER'),
(33271, 8, 0, 'General Vezax roars and surges with dark might!', 41, 0, 100, 0, 0, 0, 'General Vezax - EMOTE_SURGE_OF_DARKNESS'),
(33488, 0, 0, 'A cloud of saronite vapors coalesces nearby!', 41, 0, 100, 0, 0, 0, 'Saronite Vapor - EMOTE_VAPOR');

-- 529 торим

DELETE FROM `locales_creature_text` WHERE `entry` IN (32865,32872);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(32865, 0, 0, 'Незваные гости! Вы заплатите за то, что посмели вмешаться... Погодите, вы...'),
(32865, 1, 0, 'Я помню вас... это было в горах... Но вы... что это? Где я?..'),
(32865, 2, 0, 'Узрите мощь надвигающейся грозы!'),
(32865, 2, 1, 'Не отступать! Уничтожить их!'),
(32865, 2, 2, 'Вы уже начали жалеть о содеянном?'),
(32865, 3, 0, 'Бесстыжие выскочки, вы решили бросить вызов мне лично? Я сокрушу вас всех!'),
(32865, 4, 0, 'Ты хоть знаешь, что значит "драка"?'),
(32865, 4, 1, 'Убожество.'),
(32865, 5, 0, 'Мое терпение лопнуло!'),
(32865, 6, 0, 'Неудачники! Заморыши!'),
(32865, 7, 0, 'Придержите мечи! Я сдаюсь.'),
(32865, 8, 0, 'Кошмар, терзавший меня, развеялся, но Тьма еще не отступила от этих мест.'),
(32865, 8, 1, 'Сиф... она была тут? Не может быть – она погибла от руки моего брата. И в самом деле дурной сон…'),
(32865, 8, 2, 'Мне нужно все осознать... Если потребуется, я встану на вашу сторону. Я обязан вам куда большим. Удачи.'),
(32865, 9, 0, 'Ты! Ты не моя возлюбленная! Сгинь, нечисть!'),
(32865, 9, 1, 'Вот источник зла, заполонившего Ульдуар. Он разрушил мое королевство, затмил разум моего брата и убил мою жену.'),
(32865, 9, 2, 'Герои, теперь вам предстоит отомстить за нас всех. Это будет непросто, но я готов помочь вам. Вы должны победить!'),
(32872, 0, 0, '%s окружает себя потрескивающей Рунической преградой.');

DELETE FROM `creature` WHERE `id` IN (32874,33110,32875,33110);

UPDATE `spell_dbc` SET `RangeIndex` = 13, `AttributesEx2` = 0x00000004 WHERE `Id` = 64980;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10304,10313);
INSERT INTO `achievement_criteria_data` VALUES
(10304, 12, 0, 0, ''),
(10304, 11, 0, 0, 'achievement_who_needs_bloodlust'),
(10313, 12, 1, 0, ''),
(10313, 11, 0, 0, 'achievement_who_needs_bloodlust_25');

-- 552 фрея

DELETE FROM `creature_template_addon` WHERE `entry` IN (32916,33400);

-- 568 торим

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10305,10309);
INSERT INTO `achievement_criteria_data` VALUES
(10305, 11, 0, 0, 'achievement_do_not_stand_in_the_lighting'),
(10305, 12, 0, 0, ''),
(10309, 11, 0, 0, 'achievement_do_not_stand_in_the_lighting_25'),
(10309, 12, 1, 0, '');

DELETE FROM `spell_script_names` WHERE `spell_id` = 62466;
INSERT INTO `spell_script_names` VALUES
(62466, 'spell_thorim_lightning_release');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 64098;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`Comment`) VALUES
(13, 1, 64098, 31, 0, 3, 32865, 'Thorim - Lighting Bolt');

UPDATE `creature_template` SET `ScriptName` = 'npc_thorim_combat_trigger' WHERE `entry` = 34055;

-- 592 левиафан

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62363;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition` ,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,62363,0,1,31,0,3,33236,0,0,0,'','Anti-Air Rocket can hit Steelforged Defender'),
(13,2,62363,0,1,31,0,3,33236,0,0,0,'','Anti-Air Rocket can hit Steelforged Defender'),
(13,1,62363,0,2,31,0,3,34234,0,0,0,'','Anti-Air Rocket can hit Runeforged Sentry'),
(13,2,62363,0,2,31,0,3,34234,0,0,0,'','Anti-Air Rocket can hit Runeforged Sentry'),
(13,1,62363,0,3,31,0,3,33237,0,0,0,'','Anti-Air Rocket can hit Ulduar Colossus'),
(13,2,62363,0,3,31,0,3,33237,0,0,0,'','Anti-Air Rocket can hit Ulduar Colossus'),
(13,1,62363,0,4,31,0,3,34164,0,0,0,'','Anti-Air Rocket can hit Mechagnome Battletank'),
(13,2,62363,0,4,31,0,3,34164,0,0,0,'','Anti-Air Rocket can hit Mechagnome Battletank'),
(13,1,62363,0,5,31,0,3,33264,0,0,0,'','Anti-Air Rocket can hit Ironwork Canon'),
(13,2,62363,0,5,31,0,3,33264,0,0,0,'','Anti-Air Rocket can hit Ironwork Canon'),
(13,1,62363,0,6,31,0,3,33214,0,0,0,'','Anti-Air Rocket can hit Mechanolift 304-A'),
(13,2,62363,0,6,31,0,3,33214,0,0,0,'','Anti-Air Rocket can hit Mechanolift 304-A'),
(13,1,62363,0,7,31,0,3,33113,0,0,0,'','Anti-Air Rocket can hit Flame Leviathan'),
(13,2,62363,0,7,31,0,3,33113,0,0,0,'','Anti-Air Rocket can hit Flame Leviathan'),
(13,1,62363,0,8,31,0,3,34161,0,0,0,'','Anti-Air Rocket can hit Mechanostriker 54-A'),
(13,2,62363,0,8,31,0,3,34161,0,0,0,'','Anti-Air Rocket can hit Mechanostriker 54-A'),
(13,1,62363,0,9,31,0,3,34275,0,0,0,'','Anti-Air Rocket can hit Ward of Life'),
(13,2,62363,0,9,31,0,3,34275,0,0,0,'','Anti-Air Rocket can hit Ward of Life'),
(13,1,62363,0,10,31,0,3,33387,0,0,0,'','Anti-Air Rocket can hit Writhing Lasher'),
(13,2,62363,0,10,31,0,3,33387,0,0,0,'','Anti-Air Rocket can hit Writhing Lasher');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62307;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition` ,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,62307,0,1,31,0,3,33236,0,0,0,'','Hurl Boulder can hit Steelforged Defender'),
(13,1,62307,0,2,31,0,3,34234,0,0,0,'','Hurl Boulder can hit Runeforged Sentry'),
(13,1,62307,0,3,31,0,3,33237,0,0,0,'','Hurl Boulder can hit Ulduar Colossus'),
(13,1,62307,0,4,31,0,3,34164,0,0,0,'','Hurl Boulder can hit Mechagnome Battletank'),
(13,1,62307,0,5,31,0,3,33264,0,0,0,'','Hurl Boulder can hit Ironwork Canon'),
(13,1,62307,0,6,31,0,3,33214,0,0,0,'','Hurl Boulder can hit Mechanolift 304-A'),
(13,1,62307,0,7,31,0,3,33113,0,0,0,'','Hurl Boulder can hit Flame Leviathan'),
(13,1,62307,0,8,31,0,3,33090,0,0,0,'','Hurl Boulder can hit Pool of Tar'),
(13,1,62307,0,9,31,0,3,34161,0,0,0,'','Hurl Boulder can hit Mechanostriker 54-A'),
(13,1,62307,0,10,31,0,3,34275,0,0,0,'','Hurl Boulder can hit Ward of Life'),
(13,1,62307,0,11,31,0,3,33387,0,0,0,'','Hurl Boulder can hit Writhing Lasher');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62635;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition` ,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,62635,0,1,31,0,3,33236,0,0,0,'','Mortar can hit Steelforged Defender'),
(13,1,62635,0,2,31,0,3,34234,0,0,0,'','Mortar can hit Runeforged Sentry'),
(13,1,62635,0,3,31,0,3,33237,0,0,0,'','Mortar can hit Ulduar Colossus'),
(13,1,62635,0,4,31,0,3,34164,0,0,0,'','Mortar can hit Mechagnome Battletank'),
(13,1,62635,0,5,31,0,3,33264,0,0,0,'','Mortar can hit Ironwork Canon'),
(13,1,62635,0,6,31,0,3,33214,0,0,0,'','Mortar can hit Mechanolift 304-A'),
(13,1,62635,0,7,31,0,3,33113,0,0,0,'','Mortar can hit Flame Leviathan'),
(13,1,62635,0,8,31,0,3,33090,0,0,0,'','Mortar can hit Pool of Tar'),
(13,1,62635,0,9,31,0,3,34161,0,0,0,'','Mortar can hit Mechanostriker 54-A'),
(13,1,62635,0,10,31,0,3,34275,0,0,0,'','Mortar can hit Ward of Life'),
(13,1,62635,0,11,31,0,3,33387,0,0,0,'','Mortar can hit Writhing Lasher');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62357;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition` ,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,62357,0,1,31,0,3,33236,0,0,0,'','Fire Cannon can hit Steelforged Defender'),
(13,1,62357,0,2,31,0,3,34234,0,0,0,'','Fire Cannon can hit Runeforged Sentry'),
(13,1,62357,0,3,31,0,3,33237,0,0,0,'','Fire Cannon can hit Ulduar Colossus'),
(13,1,62357,0,4,31,0,3,34164,0,0,0,'','Fire Cannon can hit Mechagnome Battletank'),
(13,1,62357,0,5,31,0,3,33264,0,0,0,'','Fire Cannon can hit Ironwork Canon'),
(13,1,62357,0,6,31,0,3,33214,0,0,0,'','Fire Cannon can hit Mechanolift 304-A'),
(13,1,62357,0,7,31,0,3,33113,0,0,0,'','Fire Cannon can hit Flame Leviathan'),
(13,1,62357,0,8,31,0,3,33090,0,0,0,'','Fire Cannon can hit Pool of Tar'),
(13,1,62357,0,9,31,0,3,34161,0,0,0,'','Fire Cannon can hit Mechanostriker 54-A'),
(13,1,62357,0,10,31,0,3,34275,0,0,0,'','Fire Cannon can hit Ward of Life'),
(13,1,62357,0,11,31,0,3,33387,0,0,0,'','Fire Cannon can hit Writhing Lasher');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62290;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition` ,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,62290,0,1,31,0,3,33236,0,0,0,'','Burning Tar can hit Steelforged Defender'),
(13,1,62290,0,2,31,0,3,34234,0,0,0,'','Burning Tar can hit Runeforged Sentry'),
(13,1,62290,0,3,31,0,3,33237,0,0,0,'','Burning Tar can hit Ulduar Colossus'),
(13,1,62290,0,4,31,0,3,34164,0,0,0,'','Burning Tar can hit Mechagnome Battletank'),
(13,1,62290,0,5,31,0,3,33264,0,0,0,'','Burning Tar can hit Ironwork Canon'),
(13,1,62290,0,6,31,0,3,33214,0,0,0,'','Burning Tar can hit Mechanolift 304-A'),
(13,1,62290,0,7,31,0,3,33113,0,0,0,'','Burning Tar can hit Flame Leviathan'),
(13,1,62290,0,8,31,0,3,33090,0,0,0,'','Burning Tar can hit Pool of Tar'),
(13,1,62290,0,9,31,0,3,34161,0,0,0,'','Burning Tar can hit Mechanostriker 54-A'),
(13,1,62290,0,10,31,0,3,34275,0,0,0,'','Burning Tar can hit Ward of Life'),
(13,1,62290,0,11,31,0,3,33387,0,0,0,'','Burning Tar can hit Writhing Lasher');

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=62489;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition` ,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13,1,62489,0,1,31,0,3,33236,0,0,0,'','Blue Pyrite can hit Steelforged Defender'),
(13,1,62489,0,2,31,0,3,34234,0,0,0,'','Blue Pyrite can hit Runeforged Sentry'),
(13,1,62489,0,3,31,0,3,33237,0,0,0,'','Blue Pyrite can hit Ulduar Colossus'),
(13,1,62489,0,4,31,0,3,34164,0,0,0,'','Blue Pyrite can hit Mechagnome Battletank'),
(13,1,62489,0,5,31,0,3,33264,0,0,0,'','Blue Pyrite can hit Ironwork Canon'),
(13,1,62489,0,6,31,0,3,33214,0,0,0,'','Blue Pyrite can hit Mechanolift 304-A'),
(13,1,62489,0,7,31,0,3,33113,0,0,0,'','Blue Pyrite can hit Flame Leviathan'),
(13,1,62489,0,8,31,0,3,33090,0,0,0,'','Blue Pyrite can hit Pool of Tar'),
(13,1,62489,0,9,31,0,3,34161,0,0,0,'','Blue Pyrite can hit Mechanostriker 54-A'),
(13,1,62489,0,10,31,0,3,34275,0,0,0,'','Blue Pyrite can hit Ward of Life'),
(13,1,62489,0,11,31,0,3,33387,0,0,0,'','Blue Pyrite can hit Writhing Lasher');

-- 593 левиафан

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (10056,10059,10057,10060,10058,10061,10218,10219);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
('10056','11','0','0','achievement_orbital_bombardment'),
('10056','12','0','0',''),
('10057','11','0','0','achievement_orbital_devastation'),
('10057','12','0','0',''),
('10058','11','0','0','achievement_nuked_from_orbit'),
('10058','12','0','0',''),
('10059','11','0','0','achievement_orbital_bombardment'),
('10059','12','1','0',''),
('10060','11','0','0','achievement_orbital_devastation'),
('10060','12','1','0',''),
('10061','11','0','0','achievement_nuked_from_orbit'),
('10061','12','1','0',''),
('10218','11','0','0','achievement_orbit_uary'),
('10218','12','0','0',''),
('10219','11','0','0','achievement_orbit_uary'),
('10219','12','1','0','');

-- 600 йогсарон

DELETE FROM `creature` WHERE `id` = 33288;

