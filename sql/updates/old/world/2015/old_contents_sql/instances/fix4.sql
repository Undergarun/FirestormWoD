REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55503, 0, 0, 0, 0, 0, 39908, 0, 0, 0, 'Legion Demon', '', '', 0, 86, 86, 3, 2387, 2387, 0, 3.6, 1.28571, 1, 1, 516, 774, 0, 193, 17.4, 2000, 2000, 1, 64, 2048, 0, 0, 0, 0, 0, 0, 413, 619, 154, 3, 8, 55503, 0, 0, 0, 0, 0, 0, 0, 0, 108479, 103913, 105243, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 24.5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 138, 1, 617299967, 0, 'npc_well_of_eternity_legion_demon', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=55503);
INSERT INTO `creature_loot_template` VALUES 
(55503, 76157, 0.6062, 1, 0, 1, 1),
(55503, 76158, 0.5886, 1, 0, 1, 1),
(55503, 76159, 1.054, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55654, 0, 0, 0, 0, 0, 39280, 0, 0, 0, 'Corrupted Arcanist', '', '', 0, 85, 85, 3, 2384, 2384, 0, 2, 1.14286, 1, 1, 365, 547, 0, 137, 24.7, 2000, 2000, 8, 0, 2048, 12, 0, 0, 0, 0, 0, 292, 438, 110, 7, 0, 55654, 55654, 0, 0, 0, 0, 0, 0, 0, 107865, 107867, 107880, 0, 0, 0, 0, 0, 0, 0, 8306, 8306, '', 0, 3, 1, 24, 1, 1, 0, 0, 0, 0, 0, 0, 0, 127, 1, 0, 0, 'generic_creature', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=55654);
INSERT INTO `creature_loot_template` VALUES 
(55654, 53010, 9.8468, 1, 0, 1, 1),
(55654, 58256, 9.8468, 1, 0, 1, 1),
(55654, 58259, 7.9498, 1, 0, 1, 1),
(55654, 58261, 12.9707, 1, 0, 1, 1),
(55654, 63269, 0.2092, 1, 0, 1, 1),
(55654, 63271, 15.272, 1, 0, 1, 1),
(55654, 63272, 16.9456, 1, 0, 1, 1),
(55654, 63273, 27.8243, 1, 0, 1, 1),
(55654, 63274, 32.4268, 1, 0, 1, 1),
(55654, 63275, 29.0795, 1, 0, 1, 1),
(55654, 63300, 10.6695, 1, 0, 1, 1),
(55654, 63349, 32.4268, 1, 0, 1, 1),
(55654, 76157, 0.9057, 1, 0, 1, 1),
(55654, 76158, 0.887, 1, 0, 1, 1),
(55654, 76159, 1.461, 1, 0, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE (`entry`=55654);
INSERT INTO `pickpocketing_loot_template` VALUES 
(55654, 58259, 7.9498, 1, 0, 1, 1),
(55654, 58261, 12.9707, 1, 0, 1, 1),
(55654, 63269, 0.2092, 1, 0, 1, 1),
(55654, 63271, 15.272, 1, 0, 1, 1),
(55654, 63272, 16.9456, 1, 0, 1, 1),
(55654, 63273, 27.8243, 1, 0, 1, 1),
(55654, 63274, 32.4268, 1, 0, 1, 1),
(55654, 63275, 29.0795, 1, 0, 1, 1),
(55654, 63300, 10.6695, 1, 0, 1, 1),
(55654, 63349, 32.4268, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55656, 0, 0, 0, 0, 0, 39282, 0, 0, 0, 'Dreadlord Defender', '', '', 0, 85, 85, 3, 2384, 2384, 0, 2, 1.14286, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 0, 2048, 5, 0, 0, 0, 0, 0, 400, 600, 150, 3, 0, 55656, 55656, 0, 0, 0, 0, 0, 0, 0, 107840, 107900, 0, 0, 0, 0, 0, 0, 0, 0, 8304, 8304, '', 0, 3, 1, 27.5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 127, 1, 0, 0, 'generic_creature', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=55656);
INSERT INTO `creature_loot_template` VALUES 
(55656, 53010, 10.0049, 1, 0, 1, 1),
(55656, 58256, 10.0049, 1, 0, 1, 1),
(55656, 58259, 12.2616, 1, 0, 1, 1),
(55656, 58261, 14.4414, 1, 0, 1, 1),
(55656, 63271, 8.7193, 1, 0, 1, 1),
(55656, 63272, 19.346, 1, 0, 1, 1),
(55656, 63273, 32.1526, 1, 0, 1, 1),
(55656, 63274, 31.3351, 1, 0, 1, 1),
(55656, 63275, 22.6158, 1, 0, 1, 1),
(55656, 63300, 11.1717, 1, 0, 1, 1),
(55656, 63349, 26.4305, 1, 0, 1, 1),
(55656, 76157, 1.0376, 1, 0, 1, 1),
(55656, 76158, 0.8694, 1, 0, 1, 1),
(55656, 76159, 1.4327, 1, 0, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE (`entry`=55656);
INSERT INTO `pickpocketing_loot_template` VALUES 
(55656, 58259, 12.2616, 1, 0, 1, 1),
(55656, 58261, 14.4414, 1, 0, 1, 1),
(55656, 63271, 8.7193, 1, 0, 1, 1),
(55656, 63272, 19.346, 1, 0, 1, 1),
(55656, 63273, 32.1526, 1, 0, 1, 1),
(55656, 63274, 31.3351, 1, 0, 1, 1),
(55656, 63275, 22.6158, 1, 0, 1, 1),
(55656, 63300, 11.1717, 1, 0, 1, 1),
(55656, 63349, 26.4305, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55500, 0, 0, 0, 0, 0, 39183, 0, 0, 0, 'Illidan Stormrage', '', '', 13162, 85, 85, 3, 2386, 2386, 1, 3.2, 1.71429, 1, 0, 550, 950, 0, 36, 7, 2000, 2000, 1, 33280, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 9, 4096, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105543, 104738, 105547, 105635, 0, 0, 0, 0, 0, 1856, 0, 0, '', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 16384, 0, 'npc_well_of_eternity_illidan_1', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55624, 0, 0, 0, 0, 0, 38754, 0, 0, 0, 'Nozdormu', '', '', 13148, 88, 88, 3, 2010, 2010, 3, 1, 1.14286, 1, 1, 550, 950, 0, 56, 15, 3000, 3000, 2, 33536, 2048, 0, 0, 0, 0, 0, 0, 611, 918, 230, 7, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 500, 500, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_well_of_eternity_nozdormu', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55085, 0, 0, 0, 0, 0, 39182, 0, 0, 0, 'Peroth\'arn', '', '', 0, 87, 87, 3, 2387, 2387, 0, 3.6, 2.57143, 1.5, 1, 532, 798, 0, 199, 84.6, 2000, 2000, 1, 32768, 2048, 4, 0, 0, 0, 0, 0, 426, 638, 159, 3, 72, 55085, 0, 0, 0, 0, 0, 0, 0, 0, 104939, 104905, 105521, 105545, 0, 0, 0, 0, 0, 0, 20003, 20003, '', 0, 3, 1, 115, 1, 1, 0, 0, 0, 0, 0, 0, 0, 894, 1, 617299967, 1, 'boss_perotharn', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=55085);
INSERT INTO `creature_loot_template` VALUES 
(55085, 72827, 0, 1, 1, 1, 1),
(55085, 72828, 0, 1, 1, 1, 1),
(55085, 72829, 0, 1, 1, 1, 1),
(55085, 72830, 0, 1, 1, 1, 1),
(55085, 72831, 0, 1, 1, 1, 1),
(55085, 72832, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55656, 0, 0, 0, 0, 0, 39282, 0, 0, 0, 'Dreadlord Defender', '', '', 0, 85, 85, 3, 2384, 2384, 0, 2, 1.14286, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 3, 0, 55656, 55656, 0, 0, 0, 0, 0, 0, 0, 107840, 107900, 0, 0, 0, 0, 0, 0, 0, 0, 8304, 8304, '', 0, 3, 1, 27.5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 127, 1, 0, 0, 'generic_creature', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54612, 0, 0, 0, 0, 0, 38874, 0, 0, 0, 'Eternal Champion', 'The Queen\'s Royal Guard', '', 0, 85, 85, 3, 16, 16, 0, 1, 1.14286, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 41024, 2048, 12, 0, 0, 0, 0, 0, 400, 600, 150, 7, 72, 54612, 54612, 0, 0, 0, 0, 0, 0, 0, 102260, 102257, 0, 0, 0, 0, 0, 0, 0, 0, 8302, 8302, '', 0, 3, 1, 8, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_well_of_eternity_eternal_champion', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=54612);
INSERT INTO `creature_loot_template` VALUES 
(54612, 53010, 10.284, 1, 0, 1, 1),
(54612, 58256, 10.284, 1, 0, 1, 1),
(54612, 58259, 12.3541, 1, 0, 1, 1),
(54612, 58261, 10.8949, 1, 0, 1, 1),
(54612, 63271, 13.4241, 1, 0, 1, 1),
(54612, 63272, 12.7432, 1, 0, 1, 1),
(54612, 63273, 26.5564, 1, 0, 1, 1),
(54612, 63274, 30.7393, 1, 0, 1, 1),
(54612, 63275, 25.2918, 1, 0, 1, 1),
(54612, 63300, 9.7276, 1, 0, 1, 1),
(54612, 63349, 26.5564, 1, 0, 1, 1),
(54612, 76157, 1.0243, 1, 0, 1, 1),
(54612, 76158, 0.8079, 1, 0, 1, 1),
(54612, 76159, 1.3189, 1, 0, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE (`entry`=54612);
INSERT INTO `pickpocketing_loot_template` VALUES 
(54612, 58259, 12.3541, 1, 0, 1, 1),
(54612, 58261, 10.8949, 1, 0, 1, 1),
(54612, 63271, 13.4241, 1, 0, 1, 1),
(54612, 63272, 12.7432, 1, 0, 1, 1),
(54612, 63273, 26.5564, 1, 0, 1, 1),
(54612, 63274, 30.7393, 1, 0, 1, 1),
(54612, 63275, 25.2918, 1, 0, 1, 1),
(54612, 63300, 9.7276, 1, 0, 1, 1),
(54612, 63349, 26.5564, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54747, 0, 0, 0, 0, 0, 14430, 0, 0, 0, 'Eye of the Legion', '', '', 0, 85, 85, 3, 16, 16, 0, 1, 0.992063, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 32768, 2048, 12, 0, 0, 0, 0, 0, 400, 600, 150, 3, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102356, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_well_of_eternity_eye_of_legion', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54589, 0, 0, 0, 0, 0, 38864, 0, 0, 0, 'Enchanted Highmistress', 'The Queen\'s Royal Guard', '', 0, 85, 85, 3, 16, 16, 0, 1, 1.14286, 1, 1, 465, 697, 0, 174, 19.4, 2000, 2000, 2, 32832, 2048, 13, 0, 0, 0, 0, 0, 372, 558, 139, 7, 72, 54589, 54589, 0, 0, 0, 0, 0, 0, 0, 102267, 102266, 0, 0, 0, 0, 0, 0, 0, 0, 8256, 8256, '', 0, 3, 1, 5, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_well_of_eternity_enchanted_highmistress', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=54589);
INSERT INTO `creature_loot_template` VALUES 
(54589, 53010, 10.7141, 1, 0, 1, 1),
(54589, 58256, 10.7141, 1, 0, 1, 1),
(54589, 58259, 10.2689, 1, 0, 1, 1),
(54589, 58261, 10.0244, 1, 0, 1, 1),
(54589, 63269, 0.2445, 1, 0, 1, 1),
(54589, 63271, 13.6919, 1, 0, 1, 1),
(54589, 63272, 14.6699, 1, 0, 1, 1),
(54589, 63273, 24.4499, 1, 0, 1, 1),
(54589, 63274, 26.6504, 1, 0, 1, 1),
(54589, 63275, 28.6064, 1, 0, 1, 1),
(54589, 63300, 11.7359, 1, 0, 1, 1),
(54589, 63349, 24.6944, 1, 0, 1, 1),
(54589, 76157, 0.9412, 1, 0, 1, 1),
(54589, 76158, 0.859, 1, 0, 1, 1),
(54589, 76159, 1.3387, 1, 0, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE (`entry`=54589);
INSERT INTO `pickpocketing_loot_template` VALUES 
(54589, 58259, 10.2689, 1, 0, 1, 1),
(54589, 58261, 10.0244, 1, 0, 1, 1),
(54589, 63269, 0.2445, 1, 0, 1, 1),
(54589, 63271, 13.6919, 1, 0, 1, 1),
(54589, 63272, 14.6699, 1, 0, 1, 1),
(54589, 63273, 24.4499, 1, 0, 1, 1),
(54589, 63274, 26.6504, 1, 0, 1, 1),
(54589, 63275, 28.6064, 1, 0, 1, 1),
(54589, 63300, 11.7359, 1, 0, 1, 1),
(54589, 63349, 24.6944, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54853, 0, 0, 0, 0, 0, 39391, 0, 0, 0, 'Queen Azshara', '', '', 0, 88, 88, 3, 14, 14, 0, 0.8, 2, 1, 1, 510, 764, 0, 191, 17.7, 2000, 2000, 2, 2147516480, 2048, 12, 0, 0, 0, 0, 0, 408, 611, 153, 7, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 102915, 103241, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 50, 20, 1, 0, 0, 0, 0, 0, 0, 0, 181, 1, 0, 1, 'boss_queen_azshara', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55519, 0, 0, 0, 0, 0, 39298, 0, 0, 0, 'Doomguard Annihilator', '', '', 0, 85, 85, 3, 90, 90, 0, 1, 1, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 0, 2048, 12, 0, 0, 0, 0, 0, 400, 600, 150, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 113, 1, 0, 0, 'npc_well_of_eternity_doomguard_annihilator', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55570, 0, 0, 0, 0, 0, 39162, 0, 0, 0, 'Malfurion Stormrage', '', '', 0, 87, 87, 3, 35, 35, 0, 1, 1.14286, 1, 1, 495, 742, 0, 186, 18.2, 2000, 2000, 2, 832, 2048, 0, 0, 0, 0, 0, 0, 396, 594, 149, 7, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 55, 40, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_well_of_eternity_malfurion', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55524, 0, 0, 0, 0, 0, 39620, 0, 0, 0, 'Tyrande Whisperwind', 'High Priestess of Elune', '', 0, 87, 87, 3, 1770, 1770, 0, 1, 0.992063, 1, 1, 495, 742, 0, 186, 18.2, 2000, 2000, 2, 32832, 2048, 0, 0, 0, 0, 0, 0, 396, 594, 149, 7, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103917, 104313, 104214, 104688, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 50, 20, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_well_of_eternity_tyrande', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55510, 0, 0, 0, 0, 0, 39152, 0, 0, 0, 'Abyssal Doombringer', '', '', 0, 86, 86, 3, 14, 14, 0, 1, 1.14286, 1, 1, 516, 774, 0, 193, 17.4, 2000, 2000, 1, 0, 134219776, 12, 0, 0, 0, 0, 0, 413, 619, 154, 3, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103992, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 617299967, 0, 'npc_well_of_eternity_abyssal_doombringer', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (55419, 0, 0, 0, 0, 0, 39127, 0, 0, 0, 'Captain Varo\'then', 'The Hand of Azshara', '', 0, 87, 87, 3, 1771, 1771, 0, 1, 1.42857, 1, 1, 495, 742, 0, 186, 91, 2000, 2000, 2, 576, 2048, 12, 0, 0, 0, 0, 0, 396, 594, 149, 7, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 103669, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 70, 5, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 1, 'npc_mannoroth_varothen', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54969, 0, 0, 0, 0, 0, 38996, 0, 0, 0, 'Mannoroth', 'The Destructor', '', 0, 87, 87, 3, 1771, 1771, 0, 0.777776, 1.42857, 1, 1, 532, 798, 0, 199, 16.9, 2000, 2000, 1, 768, 2048, 12, 0, 0, 0, 0, 0, 426, 638, 159, 3, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 104961, 105093, 103888, 103966, 0, 0, 0, 0, 0, 584, 0, 0, '', 0, 3, 1, 1200, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, 'boss_mannoroth', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (57913, 0, 0, 0, 0, 0, 10008, 0, 0, 0, 'Chromie', '', '', 13362, 85, 85, 3, 35, 35, 3, 1, 1.14286, 1, 0, 550, 950, 0, 36, 7, 2000, 2000, 1, 33536, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1.35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 15595);
