REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54544, 0, 0, 0, 0, 0, 39617, 0, 0, 0, 'Echo of Tyrande', '', '', 0, 87, 87, 3, 14, 14, 0, 1, 0.992063, 1, 1, 495, 742, 0, 186, 91, 2000, 2000, 2, 32832, 2048, 12, 0, 0, 0, 0, 0, 396, 594, 149, 7, 104, 54544, 0, 0, 0, 0, 0, 0, 0, 0, 102181, 102606, 102605, 102472, 0, 0, 0, 0, 0, 0, 19976, 19976, '', 0, 3, 1, 80, 20, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 617299967, 1, 'boss_echo_of_tyrande', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=54544);
INSERT INTO `creature_loot_template` VALUES 
(54544, 72798, 0, 1, 1, 1, 1),
(54544, 72799, 0, 1, 1, 1, 1),
(54544, 72800, 0, 1, 1, 1, 1),
(54544, 72801, 0, 1, 1, 1, 1),
(54544, 72802, 0, 1, 1, 1, 1),
(54544, 72803, 0, 1, 1, 1, 1),
(54544, 72804, 0, 1, 1, 1, 1),
(54544, 72805, 0, 1, 1, 1, 1),
(54544, 72806, 0, 1, 1, 1, 1),
(54544, 72807, 0, 1, 1, 1, 1),
(54544, 72812, 0, 1, 1, 1, 1),
(54544, 72813, 0, 1, 1, 1, 1);

update creature set movementtype=0 where id in (54543);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54543, 0, 0, 0, 0, 0, 39524, 0, 0, 0, 'Time-Twisted Drake', '', '', 0, 85, 85, 3, 103, 103, 0, 1.2, 1.71429, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 32768, 2048, 12, 0, 0, 0, 0, 0, 400, 600, 150, 2, 72, 54543, 0, 54543, 0, 0, 0, 0, 0, 0, 102134, 102135, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 6, 10, 1, 1, 0, 0, 0, 0, 0, 0, 0, 165, 1, 0, 0, 'generic_creature', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=54543);
INSERT INTO `creature_loot_template` VALUES 
(54543, 52976, 87.7604, 1, 0, 1, 6),
(54543, 52979, 48.8467, 1, 0, 1, 5),
(54543, 60388, 63.4451, 1, 0, 1, 1),
(54543, 76154, 0.5611, 1, 0, 1, 1),
(54543, 76155, 0.6873, 1, 0, 1, 1),
(54543, 76156, 0.8136, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=54543);
INSERT INTO `skinning_loot_template` VALUES 
(54543, 52976, 87.7604, 1, 0, 1, 6),
(54543, 52979, 48.8467, 1, 0, 1, 5);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54552, 0, 0, 0, 0, 0, 8311, 0, 0, 0, 'Time-Twisted Breaker', '', '', 0, 85, 85, 3, 103, 103, 0, 1.2, 1.71429, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 32768, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 2, 72, 54552, 0, 54552, 0, 0, 0, 0, 0, 0, 102132, 102124, 0, 0, 0, 0, 0, 0, 0, 0, 8355, 8355, '', 0, 3, 1, 7, 1, 1, 0, 0, 0, 0, 0, 0, 0, 165, 1, 0, 0, 'generic_creature', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=54552);
INSERT INTO `creature_loot_template` VALUES 
(54552, 52976, 100, 1, 0, 1, 6),
(54552, 52979, 46.6165, 1, 0, 1, 6),
(54552, 53010, 11.0388, 1, 0, 1, 1),
(54552, 58256, 6.1421, 1, 0, 1, 1),
(54552, 58268, 11.0388, 1, 0, 1, 1),
(54552, 76154, 0.5944, 1, 0, 1, 1),
(54552, 76155, 1.0473, 1, 0, 1, 1),
(54552, 76156, 1.3303, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=54552);
INSERT INTO `skinning_loot_template` VALUES 
(54552, 52976, 100, 1, 0, 1, 6),
(54552, 52979, 46.6165, 1, 0, 1, 6);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54553, 0, 0, 0, 0, 0, 8249, 0, 0, 0, 'Time-Twisted Seer', '', '', 0, 85, 85, 3, 103, 103, 0, 1.2, 1.71429, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 32768, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 2, 72, 54553, 0, 54553, 0, 0, 0, 0, 0, 0, 102156, 102158, 0, 0, 0, 0, 0, 0, 0, 0, 8222, 8222, '', 0, 3, 1, 7, 1, 1, 0, 0, 0, 0, 0, 0, 0, 165, 1, 0, 0, 'generic_creature', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=54553);
INSERT INTO `creature_loot_template` VALUES 
(54553, 52976, 89.0899, 1, 0, 1, 6),
(54553, 52979, 47.8978, 1, 0, 1, 5),
(54553, 53010, 9.8025, 1, 0, 1, 1),
(54553, 58256, 5.536, 1, 0, 1, 1),
(54553, 58268, 9.8025, 1, 0, 1, 1),
(54553, 76154, 1.0226, 1, 0, 1, 1),
(54553, 76155, 1.1989, 1, 0, 1, 1),
(54553, 76156, 0.8463, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=54553);
INSERT INTO `skinning_loot_template` VALUES 
(54553, 52976, 89.0899, 1, 0, 1, 6),
(54553, 52979, 47.8978, 1, 0, 1, 5);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54431, 0, 0, 0, 0, 0, 38791, 0, 0, 0, 'Echo of Baine', '', '', 0, 87, 87, 3, 16, 16, 0, 2.4, 1.42857, 1, 1, 399, 598, 0, 150, 112.9, 1500, 1500, 1, 32832, 2048, 12, 0, 0, 0, 0, 0, 319, 478, 120, 7, 72, 54431, 0, 0, 0, 0, 0, 0, 0, 0, 101840, 101627, 101625, 0, 0, 0, 0, 0, 0, 0, 19996, 19996, '', 0, 3, 1, 86, 16, 1, 0, 0, 0, 0, 0, 0, 0, 152, 1, 617299967, 1, 'boss_echo_of_baine', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=54431);
INSERT INTO `creature_loot_template` VALUES 
(54431, 72798, 0, 1, 1, 1, 1),
(54431, 72799, 0, 1, 1, 1, 1),
(54431, 72800, 0, 1, 1, 1, 1),
(54431, 72801, 0, 1, 1, 1, 1),
(54431, 72802, 0, 1, 1, 1, 1),
(54431, 72803, 0, 1, 1, 1, 1),
(54431, 72804, 0, 1, 1, 1, 1),
(54431, 72805, 0, 1, 1, 1, 1),
(54431, 72806, 0, 1, 1, 1, 1),
(54431, 72807, 0, 1, 1, 1, 1),
(54431, 72814, 0, 1, 1, 1, 1),
(54431, 72815, 0, 1, 1, 1, 1);

DELETE FROM `creature_equip_template` WHERE (`entry`=54431);
INSERT INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES (54431, 76289, 0, 0);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54920, 0, 0, 0, 0, 0, 19061, 0, 0, 0, 'Infinite Suppressor', '', '', 0, 85, 85, 3, 2075, 2075, 0, 1, 1.14286, 1, 1, 365, 547, 0, 137, 24.7, 2000, 2000, 8, 67141696, 2048, 13, 0, 0, 0, 0, 0, 292, 438, 110, 2, 8, 54920, 0, 54920, 0, 0, 0, 0, 0, 0, 102601, 102600, 0, 0, 0, 0, 0, 0, 0, 0, 8295, 8295, '', 0, 3, 1, 8, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_end_time_infinite_suppressor', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=54920);
INSERT INTO `creature_loot_template` VALUES 
(54920, 52976, 100, 1, 0, 1, 7),
(54920, 52979, 48.3799, 1, 0, 1, 6),
(54920, 53010, 10.7885, 1, 0, 1, 1),
(54920, 58256, 5.7663, 1, 0, 1, 1),
(54920, 58268, 10.7885, 1, 0, 1, 1),
(54920, 76154, 1.024, 1, 0, 1, 1),
(54920, 76155, 1.0695, 1, 0, 1, 1),
(54920, 76156, 1.2698, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=54920);
INSERT INTO `skinning_loot_template` VALUES 
(54920, 52976, 100, 1, 0, 1, 7),
(54920, 52979, 48.3799, 1, 0, 1, 6);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54923, 0, 0, 0, 0, 0, 19059, 0, 0, 0, 'Infinite Warden', '', '', 0, 85, 85, 3, 2075, 2075, 0, 1, 1.14286, 1, 1, 500, 750, 0, 187, 18, 2000, 2000, 1, 67141696, 2048, 12, 0, 0, 0, 0, 0, 400, 600, 150, 2, 8, 54923, 0, 54923, 0, 0, 0, 0, 0, 0, 102599, 102598, 0, 0, 0, 0, 0, 0, 0, 0, 8297, 8297, '', 0, 3, 1, 8, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_end_time_infinite_warden', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=54923);
INSERT INTO `creature_loot_template` VALUES 
(54923, 52976, 100, 1, 0, 1, 7),
(54923, 52979, 49.4669, 1, 0, 1, 6),
(54923, 53010, 10.6596, 1, 0, 1, 1),
(54923, 58256, 5.5245, 1, 0, 1, 1),
(54923, 58268, 10.6596, 1, 0, 1, 1),
(54923, 76154, 0.8634, 1, 0, 1, 1),
(54923, 76155, 1.1274, 1, 0, 1, 1),
(54923, 76156, 1.2291, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=54923);
INSERT INTO `skinning_loot_template` VALUES 
(54923, 52976, 100, 1, 0, 1, 7),
(54923, 52979, 49.4669, 1, 0, 1, 6);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (54432, 0, 0, 0, 0, 0, 38931, 0, 0, 0, 'Murozond', 'The Lord of the Infinite', '', 0, 87, 87, 3, 103, 103, 0, 1, 1.42857, 1, 1, 495, 742, 0, 186, 91, 2000, 2000, 2, 33587200, 2048, 0, 0, 0, 0, 0, 0, 396, 594, 149, 2, 104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101984, 107550, 102569, 108589, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 220, 100, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, 'boss_murozond', 15595);

REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `AIName`, `ScriptName`, `WDBVerified`) VALUES (209547, 3, 11020, 'Murozond\'s Temporal Cache', '', '', '', 0, 0, 3.5, 0, 0, 0, 0, 0, 0, 1634, 209547, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 85, 0, 0, 0, '', '', 15595);

DELETE FROM `gameobject_loot_template` WHERE (`entry`=209547);
INSERT INTO `gameobject_loot_template` VALUES 
(209547, 52078, 100, 1, 0, 1, 1),
(209547, 71636, 30, 1, 0, 1, 1),
(209547, 71715, 10, 1, 0, 1, 1),
(209547, 72816, 0, 1, 1, 1, 1),
(209547, 72817, 0, 1, 2, 1, 1),
(209547, 72818, 0, 1, 1, 1, 1),
(209547, 72819, 0, 1, 2, 1, 1),
(209547, 72820, 0, 1, 1, 1, 1),
(209547, 72821, 0, 1, 2, 1, 1),
(209547, 72822, 0, 1, 1, 1, 1),
(209547, 72823, 0, 1, 2, 1, 1),
(209547, 72824, 0, 1, 1, 1, 1),
(209547, 72825, 0, 1, 2, 1, 1),
(209547, 72826, 0, 1, 1, 1, 1),
(209547, 72897, 0, 1, 2, 1, 1);
