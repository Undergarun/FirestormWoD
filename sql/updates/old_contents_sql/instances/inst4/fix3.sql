REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43878, 43879, 43879, 0, 0, 0, 35181, 0, 0, 0, 'Grand Vizier Ertan', '', '', 0, 87, 87, 3, 16, 16, 0, 1.6, 1.71429, 1, 1, 495, 742, 0, 186, 102.2, 2000, 2000, 2, 32832, 2048, 12, 0, 0, 0, 0, 0, 396, 594, 149, 4, 104, 43878, 0, 0, 0, 0, 0, 0, 0, 0, 86331, 93990, 86340, 0, 0, 0, 0, 0, 0, 0, 20006, 20006, '', 0, 3, 1, 28, 50, 1, 0, 52506, 65660, 0, 0, 0, 0, 167, 1, 617299967, 0, 'boss_grand_vizier_ertan', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=43878);
INSERT INTO `creature_loot_template` VALUES 
(43878, 55830, 0, 1, 1, 1, 1),
(43878, 55831, 0, 1, 1, 1, 1),
(43878, 55832, 0, 1, 1, 1, 1),
(43878, 55833, 0, 1, 1, 1, 1),
(43878, 55834, 0, 1, 1, 1, 1),
(43878, 65660, -100, 1, 0, 1, 1),
(43878, 71715, 0.3682, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43879, 0, 0, 0, 0, 0, 35181, 0, 0, 0, 'Grand Vizier Ertan (1)', '', '', 0, 87, 87, 3, 16, 16, 0, 1.6, 1.71429, 1, 1, 495, 742, 0, 186, 125, 2000, 2000, 2, 32832, 2048, 12, 0, 0, 0, 0, 0, 396, 594, 149, 4, 104, 43879, 0, 0, 0, 0, 0, 0, 0, 0, 86331, 93990, 86340, 0, 0, 0, 0, 0, 0, 0, 20006, 20006, '', 0, 3, 1, 57, 1, 1, 0, 52506, 65660, 0, 0, 0, 0, 167, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=43879);
INSERT INTO `creature_loot_template` VALUES 
(43879, 56356, 0, 1, 1, 1, 1),
(43879, 56357, 0, 1, 1, 1, 1),
(43879, 56358, 0, 1, 1, 1, 1),
(43879, 56359, 0, 1, 1, 1, 1),
(43879, 56360, 0, 1, 1, 1, 1),
(43879, 65660, -100, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43873, 43874, 43874, 0, 0, 0, 34265, 0, 0, 0, 'Altairus', '', '', 0, 87, 87, 3, 16, 16, 0, 1.6, 1.71429, 1, 1, 532, 798, 0, 199, 95, 2000, 2000, 1, 32832, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 2, 104, 43873, 43873, 43873, 0, 0, 0, 0, 0, 0, 93989, 88308, 93988, 88357, 0, 0, 0, 0, 0, 0, 19994, 19994, '', 0, 1, 10.125, 31, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 617299967, 0, 'boss_altairus', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=43873);
INSERT INTO `creature_loot_template` VALUES 
(43873, 55835, 0, 1, 1, 1, 1),
(43873, 55838, 0, 1, 1, 1, 1),
(43873, 55839, 0, 1, 1, 1, 1),
(43873, 55840, 0, 1, 1, 1, 1),
(43873, 55841, 0, 1, 1, 1, 1),
(43873, 63040, 1, 1, 0, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE (`entry`=43873);
INSERT INTO `pickpocketing_loot_template` VALUES 
(43873, 67495, 10.7143, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=43873);
INSERT INTO `skinning_loot_template` VALUES 
(43873, 52976, 100, 1, 0, 1, 1),
(43873, 52979, 51.6568, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43874, 0, 0, 0, 0, 0, 34265, 0, 0, 0, 'Altairus (1)', '', '', 0, 87, 87, 3, 16, 16, 0, 1.6, 1.71429, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 32832, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 2, 104, 43874, 43873, 43873, 0, 0, 0, 0, 0, 0, 93989, 88308, 93988, 88357, 0, 0, 0, 0, 0, 0, 19994, 19994, '', 0, 1, 10.125, 80, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=43874);
INSERT INTO `creature_loot_template` VALUES 
(43874, 56361, 0, 1, 1, 1, 1),
(43874, 56362, 0, 1, 1, 1, 1),
(43874, 56363, 0, 1, 1, 1, 1),
(43874, 56364, 0, 1, 1, 1, 1),
(43874, 56365, 0, 1, 1, 1, 1),
(43874, 63040, 1, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43875, 43876, 43876, 0, 0, 0, 35388, 0, 0, 0, 'Asaad', 'Caliph of Zephyrs', '', 0, 87, 87, 3, 16, 16, 0, 1.6, 1.71429, 1, 1, 495, 742, 0, 186, 112.9, 2000, 2000, 2, 32832, 2048, 0, 0, 0, 0, 0, 0, 396, 594, 149, 4, 104, 43875, 0, 0, 0, 0, 0, 0, 0, 0, 93993, 87622, 87618, 86930, 0, 0, 0, 0, 0, 0, 20041, 20041, '', 0, 3, 1, 40, 25, 1, 0, 52506, 0, 0, 0, 0, 0, 167, 1, 617299967, 0, 'boss_asaad', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=43875);
INSERT INTO `creature_loot_template` VALUES 
(43875, 55842, 0, 1, 2, 1, 1),
(43875, 55844, 0, 1, 2, 1, 1),
(43875, 55845, 0, 1, 1, 1, 1),
(43875, 55846, 0, 1, 1, 1, 1),
(43875, 55847, 0, 1, 2, 1, 1),
(43875, 55848, 0, 1, 2, 1, 1),
(43875, 55849, 0, 1, 2, 1, 1),
(43875, 55850, 0, 1, 2, 1, 1),
(43875, 55851, 0, 1, 1, 1, 1),
(43875, 55852, 0, 1, 1, 1, 1),
(43875, 55853, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43876, 0, 0, 0, 0, 0, 35388, 0, 0, 0, 'Asaad (1)', 'Caliph of Zephyrs', '', 0, 87, 87, 3, 16, 16, 0, 1.6, 1.71429, 1, 1, 495, 742, 0, 186, 125, 2000, 2000, 2, 32832, 2048, 0, 0, 0, 0, 0, 0, 396, 594, 149, 4, 104, 43876, 0, 0, 0, 0, 0, 0, 0, 0, 93993, 87622, 87618, 86930, 0, 0, 0, 0, 0, 0, 20041, 20041, '', 0, 3, 1, 54, 1, 1, 0, 52506, 0, 0, 0, 0, 0, 167, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=43876);
INSERT INTO `creature_loot_template` VALUES 
(43876, 52078, 100, 1, 0, 1, 1),
(43876, 52506, -100, 1, 0, 1, 1),
(43876, 56366, 0, 1, 1, 1, 1),
(43876, 56367, 0, 1, 2, 1, 1),
(43876, 56368, 0, 1, 2, 1, 1),
(43876, 56369, 0, 1, 2, 1, 1),
(43876, 56370, 0, 1, 1, 1, 1),
(43876, 56371, 0, 1, 2, 1, 1),
(43876, 56372, 0, 1, 1, 1, 1),
(43876, 56373, 0, 1, 1, 1, 1),
(43876, 56374, 0, 1, 2, 1, 1),
(43876, 56375, 0, 1, 2, 1, 1),
(43876, 56376, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (44600, 46209, 46210, 46211, 0, 0, 34816, 0, 0, 0, 'Halfus Wyrmbreaker', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 1.71429, 1, 3, 548, 822, 0, 154, 97, 1500, 1500, 1, 32832, 2048, 0, 0, 0, 0, 0, 0, 329, 493, 123, 7, 108, 44600, 0, 0, 0, 0, 0, 0, 0, 0, 26662, 86160, 83693, 86162, 0, 0, 0, 0, 0, 0, 3778993, 3778993, '', 0, 3, 1, 302, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 617299967, 1, 'boss_halfus_wyrmbreaker', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=44600);
INSERT INTO `creature_loot_template` VALUES 
(44600, 59469, 0, 1, 1, 1, 1),
(44600, 59470, 0, 1, 2, 1, 1),
(44600, 59471, 0, 1, 1, 1, 1),
(44600, 59472, 0, 1, 2, 1, 1),
(44600, 59473, 0, 1, 1, 1, 1),
(44600, 59474, 0, 1, 2, 1, 1),
(44600, 59475, 0, 1, 1, 1, 1),
(44600, 59476, 0, 1, 2, 1, 1),
(44600, 59481, 0, 1, 1, 1, 1),
(44600, 59482, 0, 1, 2, 1, 1),
(44600, 59483, 0, 1, 1, 1, 1),
(44600, 59484, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (46209, 0, 0, 0, 0, 0, 34816, 0, 0, 0, 'Halfus Wyrmbreaker (1)', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 1.71429, 1, 3, 548, 822, 0, 205, 97, 2000, 2000, 1, 32832, 2048, 0, 0, 0, 0, 0, 0, 438, 658, 164, 7, 108, 46209, 0, 0, 0, 0, 0, 0, 0, 0, 26662, 86160, 83693, 86162, 0, 0, 0, 0, 0, 0, 3778993, 3778993, '', 0, 3, 1, 1080, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=46209);
INSERT INTO `creature_loot_template` VALUES 
(46209, 59469, 0, 1, 1, 1, 1),
(46209, 59470, 0, 1, 2, 1, 1),
(46209, 59471, 0, 1, 1, 1, 1),
(46209, 59472, 0, 1, 2, 1, 1),
(46209, 59473, 0, 1, 1, 1, 1),
(46209, 59474, 0, 1, 2, 1, 1),
(46209, 59475, 0, 1, 1, 1, 1),
(46209, 59476, 0, 1, 2, 1, 1),
(46209, 59481, 0, 1, 1, 1, 1),
(46209, 59482, 0, 1, 2, 1, 1),
(46209, 59483, 0, 1, 1, 1, 1),
(46209, 59484, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (46210, 0, 0, 0, 0, 0, 34816, 0, 0, 0, 'Halfus Wyrmbreaker (2)', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 1.71429, 1, 3, 548, 822, 0, 205, 110, 2000, 2000, 1, 32832, 2048, 0, 0, 0, 0, 0, 0, 438, 658, 164, 7, 108, 46210, 0, 0, 0, 0, 0, 0, 0, 0, 26662, 86160, 83693, 86162, 0, 0, 0, 0, 0, 0, 3778993, 3778993, '', 0, 3, 1, 600, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=46210);
INSERT INTO `creature_loot_template` VALUES 
(46210, 65133, 0, 1, 1, 1, 1),
(46210, 65134, 0, 1, 3, 1, 1),
(46210, 65135, 0, 1, 1, 1, 1),
(46210, 65136, 0, 1, 3, 1, 1),
(46210, 65137, 0, 1, 1, 1, 1),
(46210, 65138, 0, 1, 3, 1, 1),
(46210, 65139, 0, 1, 1, 1, 1),
(46210, 65140, 0, 1, 3, 1, 1),
(46210, 65141, 0, 1, 1, 1, 1),
(46210, 65142, 0, 1, 3, 1, 1),
(46210, 65143, 0, 1, 1, 1, 1),
(46210, 65144, 0, 1, 3, 1, 1),
(46210, 67423, 0, 1, 2, 1, 1),
(46210, 67424, 0, 1, 2, 1, 1),
(46210, 67425, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (46211, 0, 0, 0, 0, 0, 34816, 0, 0, 0, 'Halfus Wyrmbreaker (3)', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 1.71429, 1, 3, 548, 822, 0, 205, 110, 2000, 2000, 1, 32832, 2048, 0, 0, 0, 0, 0, 0, 438, 658, 164, 7, 108, 46211, 0, 0, 0, 0, 0, 0, 0, 0, 26662, 86160, 83693, 86162, 0, 0, 0, 0, 0, 0, 3778993, 3778993, '', 0, 3, 1, 2150, 1, 1, 0, 0, 0, 0, 0, 0, 0, 172, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=46211);
INSERT INTO `creature_loot_template` VALUES 
(46211, 65133, 0, 1, 1, 1, 1),
(46211, 65134, 0, 1, 3, 1, 1),
(46211, 65135, 0, 1, 1, 1, 1),
(46211, 65136, 0, 1, 3, 1, 1),
(46211, 65137, 0, 1, 1, 1, 1),
(46211, 65138, 0, 1, 3, 1, 1),
(46211, 65139, 0, 1, 1, 1, 1),
(46211, 65140, 0, 1, 3, 1, 1),
(46211, 65141, 0, 1, 1, 1, 1),
(46211, 65142, 0, 1, 3, 1, 1),
(46211, 65143, 0, 1, 1, 1, 1),
(46211, 65144, 0, 1, 3, 1, 1),
(46211, 67423, 0, 1, 2, 1, 1),
(46211, 67424, 0, 1, 2, 1, 1),
(46211, 67425, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (45992, 49897, 49898, 49899, 0, 0, 34812, 0, 0, 0, 'Valiona', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 411, 616, 0, 154, 125, 1500, 1500, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 329, 493, 123, 2, 108, 45992, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86840, 92859, 86013, 0, 0, 0, 0, 0, 0, 1591793, 1591793, '', 0, 3, 1, 300, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, 'boss_valiona', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=45992);
INSERT INTO `creature_loot_template` VALUES 
(45992, 59515, 0, 1, 1, 1, 1),
(45992, 59517, 0, 1, 1, 1, 1),
(45992, 59518, 0, 1, 1, 1, 1),
(45992, 63531, 0, 1, 1, 1, 1),
(45992, 63532, 0, 1, 1, 1, 1),
(45992, 63533, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49897, 0, 0, 0, 0, 0, 34812, 0, 0, 0, 'Valiona (1)', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 548, 822, 0, 205, 95, 2000, 2000, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 49897, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86840, 92859, 86013, 0, 0, 0, 0, 0, 0, 1591793, 1591793, '', 0, 3, 1, 912, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49897);
INSERT INTO `creature_loot_template` VALUES 
(49897, 59515, 0, 1, 1, 1, 1),
(49897, 59517, 0, 1, 2, 1, 1),
(49897, 59518, 0, 1, 1, 1, 1),
(49897, 63531, 0, 1, 2, 1, 1),
(49897, 63532, 0, 1, 1, 1, 1),
(49897, 63533, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49898, 0, 0, 0, 0, 0, 34812, 0, 0, 0, 'Valiona (2)', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 49898, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86840, 92859, 86013, 0, 0, 0, 0, 0, 0, 1591793, 1591793, '', 0, 3, 1, 560, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=49898);
INSERT INTO `creature_loot_template` VALUES 
(49898, 65094, 0, 1, 1, 1, 1),
(49898, 65095, 0, 1, 1, 1, 1),
(49898, 65096, 0, 1, 1, 1, 1),
(49898, 65106, 0, 1, 1, 1, 1),
(49898, 65107, 0, 1, 1, 1, 1),
(49898, 65109, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49899, 0, 0, 0, 0, 0, 34812, 0, 0, 0, 'Valiona (3)', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 49899, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86840, 92859, 86013, 0, 0, 0, 0, 0, 0, 1591793, 1591793, '', 0, 3, 1, 1920, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=49899);
INSERT INTO `creature_loot_template` VALUES 
(49899, 65094, 0, 1, 1, 1, 1),
(49899, 65095, 0, 1, 2, 1, 1),
(49899, 65096, 0, 1, 1, 1, 1),
(49899, 65106, 0, 1, 2, 1, 1),
(49899, 65107, 0, 1, 1, 1, 1),
(49899, 65109, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (45993, 49903, 49904, 49905, 0, 0, 34813, 0, 0, 0, 'Theralion', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 411, 616, 0, 154, 125, 1500, 1500, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 329, 493, 123, 2, 108, 45993, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86408, 92898, 86369, 0, 0, 0, 0, 0, 0, 1518004, 1518004, '', 0, 3, 1, 300, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, 'boss_theralion', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=45993);
INSERT INTO `creature_loot_template` VALUES 
(45993, 59512, 0, 1, 1, 1, 1),
(45993, 59516, 0, 1, 1, 1, 1),
(45993, 59519, 0, 1, 1, 1, 1),
(45993, 63534, 0, 1, 1, 1, 1),
(45993, 63535, 0, 1, 1, 1, 1),
(45993, 63536, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49903, 0, 0, 0, 0, 0, 34813, 0, 0, 0, 'Theralion (1)', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 548, 822, 0, 205, 95, 2000, 2000, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 49903, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86408, 92898, 86369, 0, 0, 0, 0, 0, 0, 1518004, 1518004, '', 0, 3, 1, 912, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49903);
INSERT INTO `creature_loot_template` VALUES 
(49903, 59512, 0, 1, 1, 1, 1),
(49903, 59516, 0, 1, 2, 1, 1),
(49903, 59519, 0, 1, 1, 1, 1),
(49903, 63534, 0, 1, 2, 1, 1),
(49903, 63535, 0, 1, 1, 1, 1),
(49903, 63536, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49904, 0, 0, 0, 0, 0, 34813, 0, 0, 0, 'Theralion (2)', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 49904, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86408, 92898, 86369, 0, 0, 0, 0, 0, 0, 1518004, 1518004, '', 0, 3, 1, 560, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49904);
INSERT INTO `creature_loot_template` VALUES 
(49904, 65091, 0, 1, 1, 1, 1),
(49904, 65092, 0, 1, 1, 1, 1),
(49904, 65093, 0, 1, 1, 1, 1),
(49904, 65105, 0, 1, 1, 1, 1),
(49904, 65108, 0, 1, 1, 1, 1),
(49904, 65112, 0, 1, 1, 1, 1);


REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49905, 0, 0, 0, 0, 0, 34813, 0, 0, 0, 'Theralion (3)', '', '', 0, 88, 88, 3, 2146, 2146, 0, 1, 1.42857, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 1, 32832, 134219776, 12, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 49905, 0, 0, 0, 0, 0, 0, 0, 0, 47008, 86408, 92898, 86369, 0, 0, 0, 0, 0, 0, 1518004, 1518004, '', 0, 3, 1, 1920, 20, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=49905);
INSERT INTO `creature_loot_template` VALUES 
(49905, 65091, 0, 1, 1, 1, 1),
(49905, 65092, 0, 1, 2, 1, 1),
(49905, 65093, 0, 1, 1, 1, 1),
(49905, 65105, 0, 1, 2, 1, 1),
(49905, 65108, 0, 1, 1, 1, 1),
(49905, 65112, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43735, 49619, 49620, 49621, 0, 0, 34825, 0, 0, 0, 'Elementium Monstrosity', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 411, 616, 0, 154, 125, 1500, 1500, 1, 0, 2048, 12, 0, 0, 0, 0, 0, 329, 493, 123, 4, 76, 43735, 0, 0, 0, 0, 0, 0, 0, 0, 84918, 84948, 92486, 84913, 0, 0, 0, 0, 0, 0, 2784316, 2784316, '', 0, 5, 1, 270, 1, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, 'boss_elementium_monstrosity', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=43735);
INSERT INTO `creature_loot_template` VALUES 
(43735, 59502, 0, 1, 1, 1, 1),
(43735, 59503, 0, 1, 1, 1, 1),
(43735, 59504, 0, 1, 1, 1, 1),
(43735, 59505, 0, 1, 1, 1, 1),
(43735, 59506, 0, 1, 1, 1, 1),
(43735, 59507, 0, 1, 1, 1, 1),
(43735, 59508, 0, 1, 1, 1, 1),
(43735, 59509, 0, 1, 1, 1, 1),
(43735, 59510, 0, 1, 1, 1, 1),
(43735, 59511, 0, 1, 1, 1, 1),
(43735, 59513, 0, 1, 1, 1, 1),
(43735, 59514, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49619, 0, 0, 0, 0, 0, 34825, 0, 0, 0, 'Elementium Monstrosity (1)', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 548, 822, 0, 205, 95, 2000, 2000, 1, 0, 2048, 12, 0, 0, 0, 0, 0, 438, 658, 164, 4, 76, 49619, 0, 0, 0, 0, 0, 0, 0, 0, 84918, 84948, 92486, 84913, 0, 0, 0, 0, 0, 0, 2784316, 2784316, '', 0, 5, 1, 850, 1, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49619);
INSERT INTO `creature_loot_template` VALUES 
(49619, 59502, 0, 1, 1, 1, 1),
(49619, 59503, 0, 1, 2, 1, 1),
(49619, 59504, 0, 1, 1, 1, 1),
(49619, 59505, 0, 1, 2, 1, 1),
(49619, 59506, 0, 1, 1, 1, 1),
(49619, 59507, 0, 1, 2, 1, 1),
(49619, 59508, 0, 1, 1, 1, 1),
(49619, 59509, 0, 1, 2, 1, 1),
(49619, 59510, 0, 1, 1, 1, 1),
(49619, 59511, 0, 1, 2, 1, 1),
(49619, 59513, 0, 1, 1, 1, 1),
(49619, 59514, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49620, 0, 0, 0, 0, 0, 34825, 0, 0, 0, 'Elementium Monstrosity (2)', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 1, 0, 2048, 12, 0, 0, 0, 0, 0, 438, 658, 164, 4, 76, 49620, 0, 0, 0, 0, 0, 0, 0, 0, 84918, 84948, 92486, 84913, 0, 0, 0, 0, 0, 0, 2784316, 2784316, '', 0, 5, 1, 494, 1, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49620);
INSERT INTO `creature_loot_template` VALUES 
(49620, 65110, 0, 1, 1, 1, 1),
(49620, 65111, 0, 1, 1, 1, 1),
(49620, 65113, 0, 1, 1, 1, 1),
(49620, 65114, 0, 1, 1, 1, 1),
(49620, 65115, 0, 1, 1, 1, 1),
(49620, 65116, 0, 1, 1, 1, 1),
(49620, 65117, 0, 1, 1, 1, 1),
(49620, 65118, 0, 1, 1, 1, 1),
(49620, 65119, 0, 1, 1, 1, 1),
(49620, 65120, 0, 1, 1, 1, 1),
(49620, 65121, 0, 1, 1, 1, 1),
(49620, 65122, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49621, 0, 0, 0, 0, 0, 34825, 0, 0, 0, 'Elementium Monstrosity (3)', '', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 1, 0, 2048, 12, 0, 0, 0, 0, 0, 438, 658, 164, 4, 76, 49621, 0, 0, 0, 0, 0, 0, 0, 0, 84918, 84948, 92486, 84913, 0, 0, 0, 0, 0, 0, 2784316, 2784316, '', 0, 5, 1, 1456, 1, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49621);
INSERT INTO `creature_loot_template` VALUES 
(49621, 65110, 0, 1, 1, 1, 1),
(49621, 65111, 0, 1, 2, 1, 1),
(49621, 65113, 0, 1, 1, 1, 1),
(49621, 65114, 0, 1, 2, 1, 1),
(49621, 65115, 0, 1, 1, 1, 1),
(49621, 65116, 0, 1, 2, 1, 1),
(49621, 65117, 0, 1, 1, 1, 1),
(49621, 65118, 0, 1, 2, 1, 1),
(49621, 65119, 0, 1, 1, 1, 1),
(49621, 65120, 0, 1, 2, 1, 1),
(49621, 65121, 0, 1, 1, 1, 1),
(49621, 65122, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43689, 49609, 49610, 49611, 0, 0, 34824, 0, 0, 0, 'Terrastra', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 382, 574, 0, 143, 125, 1500, 1500, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 306, 459, 114, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82285, 83675, 83572, 83718, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 55, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, 'boss_terrastra', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49609, 0, 0, 0, 0, 0, 34824, 0, 0, 0, 'Terrastra (1)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 102.2, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82285, 83675, 83572, 83718, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 170, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49610, 0, 0, 0, 0, 0, 34824, 0, 0, 0, 'Terrastra (2)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82285, 83675, 83572, 83718, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 97.5, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49611, 0, 0, 0, 0, 0, 34824, 0, 0, 0, 'Terrastra (3)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82285, 83675, 83572, 83718, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 286, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43688, 49606, 49607, 49608, 0, 0, 34823, 0, 0, 0, 'Arion', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 382, 574, 0, 143, 125, 1500, 1500, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 306, 459, 114, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83491, 83087, 82285, 92454, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 55, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, 'boss_arion', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49606, 0, 0, 0, 0, 0, 34823, 0, 0, 0, 'Arion (1)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 102.2, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83491, 83087, 82285, 92454, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 170, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49607, 0, 0, 0, 0, 0, 34823, 0, 0, 0, 'Arion (2)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83491, 83087, 82285, 92454, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 97.5, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49608, 0, 0, 0, 0, 0, 34823, 0, 0, 0, 'Arion (3)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83491, 83087, 82285, 92454, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 286, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43686, 49615, 49616, 49617, 0, 0, 34821, 0, 0, 0, 'Ignacious', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 382, 574, 0, 143, 125, 1500, 1500, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 306, 459, 114, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82631, 92512, 82660, 82777, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, 'boss_ignacious', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49615, 0, 0, 0, 0, 0, 34821, 0, 0, 0, 'Ignacious (1)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 102.2, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82631, 92512, 82660, 82777, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 255, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49616, 0, 0, 0, 0, 0, 34821, 0, 0, 0, 'Ignacious (2)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82631, 92512, 82660, 82777, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 149.5, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49617, 0, 0, 0, 0, 0, 34821, 0, 0, 0, 'Ignacious (3)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82631, 92512, 82660, 82777, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 442, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 0, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43687, 49612, 49613, 49614, 0, 0, 34822, 0, 0, 0, 'Feludius', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 382, 574, 0, 143, 125, 1500, 1500, 2, 64, 2048, 0, 0, 0, 0, 0, 0, 306, 459, 114, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82746, 92506, 82665, 82752, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 80, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, 'boss_feludius', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49612, 0, 0, 0, 0, 0, 34822, 0, 0, 0, 'Feludius (1)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 102.2, 2000, 2000, 2, 64, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82746, 92506, 82665, 82752, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 255, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49613, 0, 0, 0, 0, 0, 34822, 0, 0, 0, 'Feludius (2)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 64, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82746, 92506, 82665, 82752, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 149.5, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49614, 0, 0, 0, 0, 0, 34822, 0, 0, 0, 'Feludius (3)', 'Ascendant Lord', '', 0, 88, 88, 3, 16, 16, 0, 3.2, 2, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 64, 2048, 0, 0, 0, 0, 0, 0, 408, 611, 153, 4, 108, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82746, 92506, 82665, 82752, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 442, 500, 1, 0, 0, 0, 0, 0, 0, 0, 187, 1, 617299967, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43324, 50131, 50132, 50133, 0, 0, 34576, 0, 0, 0, 'Cho\'gall', '', '', 0, 88, 88, 3, 16, 16, 0, 1, 1.42857, 1, 1, 382, 574, 0, 143, 125, 1500, 1500, 2, 64, 134219776, 0, 0, 0, 0, 0, 0, 306, 459, 114, 7, 108, 43324, 0, 0, 0, 0, 0, 0, 0, 0, 64238, 82630, 93203, 91303, 0, 0, 0, 0, 0, 961, 2635676, 2635676, '', 0, 3, 1, 312, 100, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, 'boss_chogall', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=43324);
INSERT INTO `creature_loot_template` VALUES 
(43324, 59330, 0, 1, 1, 1, 1),
(43324, 59485, 0, 1, 3, 1, 1),
(43324, 59486, 0, 1, 1, 1, 1),
(43324, 59487, 0, 1, 3, 1, 1),
(43324, 59490, 0, 1, 1, 1, 1),
(43324, 59494, 0, 1, 3, 1, 1),
(43324, 59495, 0, 1, 1, 1, 1),
(43324, 59497, 0, 1, 3, 1, 1),
(43324, 59498, 0, 1, 1, 1, 1),
(43324, 59499, 0, 1, 3, 1, 1),
(43324, 59500, 0, 1, 1, 1, 1),
(43324, 59501, 0, 1, 3, 1, 1),
(43324, 63680, 0, 1, 1, 1, 1),
(43324, 64314, 0, 1, 2, 1, 1),
(43324, 64315, 0, 1, 2, 1, 1),
(43324, 64316, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (50131, 0, 0, 0, 0, 0, 34576, 0, 0, 0, 'Cho\'gall (1)', '', '', 0, 88, 88, 3, 16, 16, 0, 1, 1.42857, 1, 1, 510, 764, 0, 191, 105.4, 2000, 2000, 2, 64, 134219776, 0, 0, 0, 0, 0, 0, 408, 611, 153, 7, 108, 50131, 0, 0, 0, 0, 0, 0, 0, 0, 64238, 82630, 93203, 91303, 0, 0, 0, 0, 0, 961, 2635676, 2635676, '', 0, 3, 1, 944, 100, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=50131);
INSERT INTO `creature_loot_template` VALUES 
(50131, 59330, 0, 1, 1, 1, 1),
(50131, 59485, 0, 1, 3, 1, 1),
(50131, 59486, 0, 1, 4, 1, 1),
(50131, 59487, 0, 1, 1, 1, 1),
(50131, 59490, 0, 1, 3, 1, 1),
(50131, 59494, 0, 1, 4, 1, 1),
(50131, 59495, 0, 1, 1, 1, 1),
(50131, 59497, 0, 1, 3, 1, 1),
(50131, 59498, 0, 1, 4, 1, 1),
(50131, 59499, 0, 1, 1, 1, 1),
(50131, 59500, 0, 1, 3, 1, 1),
(50131, 59501, 0, 1, 4, 1, 1),
(50131, 63680, 0, 1, 1, 1, 1),
(50131, 64314, 0, 1, 2, 1, 1),
(50131, 64315, 0, 1, 2, 1, 1),
(50131, 64316, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (50132, 0, 0, 0, 0, 0, 34576, 0, 0, 0, 'Cho\'gall (2)', '', '', 0, 88, 88, 3, 16, 16, 0, 1, 1.42857, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 64, 134219776, 0, 0, 0, 0, 0, 0, 408, 611, 153, 7, 108, 50132, 0, 0, 0, 0, 0, 0, 0, 0, 64238, 82630, 93203, 91303, 0, 0, 0, 0, 0, 961, 2635676, 2635676, '', 0, 3, 1, 620, 100, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=50132);
INSERT INTO `creature_loot_template` VALUES 
(50132, 65087, 0, 1, 2, 1, 1),
(50132, 65088, 0, 1, 2, 1, 1),
(50132, 65089, 0, 1, 2, 1, 1),
(50132, 65090, 0, 1, 1, 1, 1),
(50132, 65123, 0, 1, 3, 1, 1),
(50132, 65124, 0, 1, 1, 1, 1),
(50132, 65125, 0, 1, 3, 1, 1),
(50132, 65126, 0, 1, 1, 1, 1),
(50132, 65127, 0, 1, 3, 1, 1),
(50132, 65128, 0, 1, 1, 1, 1),
(50132, 65129, 0, 1, 3, 1, 1),
(50132, 65130, 0, 1, 1, 1, 1),
(50132, 65131, 0, 1, 3, 1, 1),
(50132, 65132, 0, 1, 1, 1, 1),
(50132, 65145, 0, 1, 3, 1, 1),
(50132, 68600, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (50133, 0, 0, 0, 0, 0, 34576, 0, 0, 0, 'Cho\'gall (3)', '', '', 0, 88, 88, 3, 16, 16, 0, 1, 1.42857, 1, 1, 510, 764, 0, 191, 125, 2000, 2000, 2, 64, 134219776, 0, 0, 0, 0, 0, 0, 408, 611, 153, 7, 108, 50133, 0, 0, 0, 0, 0, 0, 0, 0, 64238, 82630, 93203, 91303, 0, 0, 0, 0, 0, 961, 2635676, 2635676, '', 0, 3, 1, 1918, 100, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=50133);
INSERT INTO `creature_loot_template` VALUES 
(50133, 65087, 0, 1, 2, 1, 1),
(50133, 65088, 0, 1, 2, 1, 1),
(50133, 65089, 0, 1, 2, 1, 1),
(50133, 65090, 0, 1, 1, 1, 1),
(50133, 65123, 0, 1, 3, 1, 1),
(50133, 65124, 0, 1, 4, 1, 1),
(50133, 65125, 0, 1, 1, 1, 1),
(50133, 65126, 0, 1, 3, 1, 1),
(50133, 65127, 0, 1, 4, 1, 1),
(50133, 65128, 0, 1, 1, 1, 1),
(50133, 65129, 0, 1, 3, 1, 1),
(50133, 65130, 0, 1, 4, 1, 1),
(50133, 65131, 0, 1, 1, 1, 1),
(50133, 65132, 0, 1, 3, 1, 1),
(50133, 65145, 0, 1, 4, 1, 1),
(50133, 68600, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (45213, 49744, 49744, 0, 0, 0, 34335, 0, 0, 0, 'Sinestra', 'Consort of Deathwing', '', 0, 86, 86, 3, 16, 16, 0, 1, 1.14286, 1, 1, 516, 774, 0, 193, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 413, 619, 154, 2, 108, 45213, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 500, 211.194, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 617299967, 1, 'boss_sinestra', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=45213);
INSERT INTO `creature_loot_template` VALUES 
(45213, 60226, 0, 1, 1, 1, 1),
(45213, 60227, 0, 1, 2, 1, 1),
(45213, 60228, 0, 1, 1, 1, 1),
(45213, 60229, 0, 1, 2, 1, 1),
(45213, 60230, 0, 1, 1, 1, 1),
(45213, 60231, 0, 1, 2, 1, 1),
(45213, 60232, 0, 1, 1, 1, 1),
(45213, 60233, 0, 1, 2, 1, 1),
(45213, 60234, 0, 1, 1, 1, 1),
(45213, 60235, 0, 1, 2, 1, 1),
(45213, 60236, 0, 1, 1, 1, 1),
(45213, 60237, 0, 1, 2, 1, 1),
(45213, 60238, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49744, 0, 0, 0, 0, 0, 34335, 0, 0, 0, 'Sinestra (1)', 'Consort of Deathwing', '', 0, 88, 88, 3, 16, 16, 0, 1, 1.14286, 1, 1, 548, 822, 0, 205, 125, 2000, 2000, 2, 0, 2048, 0, 0, 0, 0, 0, 0, 438, 658, 164, 2, 108, 49744, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1500, 211.194, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49744);
INSERT INTO `creature_loot_template` VALUES 
(49744, 60226, 0, 1, 1, 1, 1),
(49744, 60227, 0, 1, 2, 1, 1),
(49744, 60228, 0, 1, 3, 1, 1),
(49744, 60229, 0, 1, 1, 1, 1),
(49744, 60230, 0, 1, 2, 1, 1),
(49744, 60231, 0, 1, 3, 1, 1),
(49744, 60232, 0, 1, 1, 1, 1),
(49744, 60233, 0, 1, 2, 1, 1),
(49744, 60234, 0, 1, 3, 1, 1),
(49744, 60235, 0, 1, 1, 1, 1),
(49744, 60236, 0, 1, 2, 1, 1),
(49744, 60237, 0, 1, 3, 1, 1),
(49744, 60238, 0, 1, 1, 1, 1);
