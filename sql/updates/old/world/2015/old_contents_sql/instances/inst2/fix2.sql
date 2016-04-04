update creature set movementtype=0 where id in (39450, 39873);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (40177, 48702, 48702, 0, 0, 0, 33429, 0, 0, 0, 'Forgemaster Throngus', '', '', 0, 87, 87, 3, 1771, 1771, 0, 2, 1.42857, 1, 1, 399, 598, 0, 150, 121.2, 1500, 1500, 1, 64, 67584, 12, 0, 0, 0, 0, 0, 319, 478, 120, 5, 72, 40177, 0, 0, 0, 0, 0, 0, 0, 0, 74976, 90737, 75056, 90756, 0, 0, 0, 0, 0, 733, 19994, 19994, '', 0, 3, 1, 25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 151, 1, 617299967, 0, 'boss_forgemaster_throngus', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=40177);
INSERT INTO `creature_loot_template` VALUES 
(40177, 56118, 0, 1, 1, 1, 1),
(40177, 56119, 0, 1, 1, 1, 1),
(40177, 56120, 0, 1, 1, 1, 1),
(40177, 56121, 0, 1, 1, 1, 1),
(40177, 56122, 0, 1, 1, 1, 1),
(40177, 71638, 0.6659, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (48702, 0, 0, 0, 0, 0, 33429, 0, 0, 0, 'Forgemaster Throngus (1)', '', '', 0, 87, 87, 3, 1771, 1771, 0, 2, 1.42857, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 64, 67584, 12, 0, 0, 0, 0, 0, 426, 638, 159, 5, 72, 48702, 0, 0, 0, 0, 0, 0, 0, 0, 74976, 90737, 75056, 90756, 0, 0, 0, 0, 0, 733, 19994, 19994, '', 0, 3, 1, 60, 1, 1, 0, 0, 0, 0, 0, 0, 0, 151, 1, 617299967, 1, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (39892, 48747, 48747, 0, 0, 0, 34999, 0, 0, 0, 'Enslaved Burning Ember', '', '', 0, 84, 84, 3, 1771, 1771, 0, 1.55556, 1.14286, 1, 1, 363, 544, 0, 136, 48.3, 1500, 1500, 1, 0, 33556480, 0, 0, 0, 0, 0, 0, 290, 435, 109, 4, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 90846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2.5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_enslaved_burning_ember', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (48747, 0, 0, 0, 0, 0, 34999, 0, 0, 0, 'Enslaved Burning Ember (1)', '', '', 0, 85, 85, 3, 1771, 1771, 0, 1.55556, 1.14286, 1, 1, 500, 750, 0, 187, 35, 2000, 2000, 1, 0, 33556480, 0, 0, 0, 0, 0, 0, 400, 600, 150, 4, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 90846, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 3.5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, '', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (40319, 48784, 48784, 0, 0, 0, 31792, 0, 0, 0, 'Drahga Shadowburner', 'Twilight\'s Hammer Courier', '', 0, 87, 87, 3, 2146, 2146, 0, 1, 1.14286, 1, 1, 495, 742, 0, 186, 102.1, 2000, 2000, 2, 32832, 2048, 12, 0, 0, 0, 0, 0, 396, 594, 149, 7, 2147483720, 40319, 0, 0, 0, 0, 0, 0, 0, 0, 90915, 75245, 76303, 0, 0, 0, 0, 0, 0, 0, 20026, 20026, '', 0, 3, 1, 8, 20, 1, 0, 66927, 0, 0, 0, 0, 0, 0, 1, 617299967, 0, 'boss_drahga_shadowburner', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=40319);
INSERT INTO `creature_loot_template` VALUES 
(40319, 56123, 0, 1, 1, 1, 1),
(40319, 56124, 0, 1, 1, 1, 1),
(40319, 56125, 0, 1, 1, 1, 1),
(40319, 56126, 0, 1, 1, 1, 1),
(40319, 56127, 0, 1, 1, 1, 1),
(40319, 66927, -100, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (48784, 0, 0, 0, 0, 0, 31792, 0, 0, 0, 'Drahga Shadowburner (1)', 'Twilight\'s Hammer Courier', '', 0, 87, 87, 3, 2146, 2146, 0, 1, 1.14286, 1, 1, 495, 742, 0, 186, 125, 2000, 2000, 2, 32832, 2048, 12, 0, 0, 0, 0, 0, 396, 594, 149, 7, 2147483720, 48784, 0, 0, 0, 0, 0, 0, 0, 0, 90915, 75245, 76303, 0, 0, 0, 0, 0, 0, 0, 20026, 20026, '', 0, 3, 1, 10.5, 20, 1, 0, 66927, 0, 0, 0, 0, 0, 0, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=48784);
INSERT INTO `creature_loot_template` VALUES 
(48784, 56450, 0, 1, 1, 1, 1),
(48784, 56451, 0, 1, 1, 1, 1),
(48784, 56452, 0, 1, 1, 1, 1),
(48784, 56453, 0, 1, 1, 1, 1),
(48784, 56454, 0, 1, 1, 1, 1),
(48784, 66927, -100, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (39625, 48337, 48337, 0, 0, 0, 31498, 0, 0, 0, 'General Umbriss', 'Servant of Deathwing', '', 0, 87, 87, 3, 1771, 1771, 0, 0.888888, 1.42857, 1, 1, 399, 598, 0, 150, 121.2, 1500, 1500, 1, 67141696, 2048, 12, 0, 0, 0, 0, 0, 319, 478, 120, 2, 72, 39625, 39625, 39625, 0, 0, 0, 0, 0, 0, 91937, 74846, 90250, 74670, 0, 0, 0, 0, 0, 0, 20004, 20004, '', 0, 3, 1, 20, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 0, 'boss_general_umbriss', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=39625);
INSERT INTO `creature_loot_template` VALUES 
(39625, 56112, 0, 1, 1, 1, 1),
(39625, 56113, 0, 1, 1, 1, 1),
(39625, 56114, 0, 1, 1, 1, 1),
(39625, 56115, 0, 1, 1, 1, 1),
(39625, 56116, 0, 1, 1, 1, 1),
(39625, 71715, 0.433, 1, 0, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE (`entry`=39625);
INSERT INTO `pickpocketing_loot_template` VALUES 
(39625, 52976, 100, 1, 0, 1, 2),
(39625, 52979, 71.9315, 1, 0, 1, 1),
(39625, 67495, 6.7555, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=39625);
INSERT INTO `skinning_loot_template` VALUES 
(39625, 52976, 64.1848, 1, 0, 1, 1),
(39625, 52979, 35.6619, 1, 0, 1, 1),
(39625, 67495, 3.0768, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (48337, 0, 0, 0, 0, 0, 31498, 0, 0, 0, 'General Umbriss (1)', 'Servant of Deathwing', '', 0, 87, 87, 3, 1771, 1771, 0, 0.888888, 1.42857, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 67141696, 2048, 12, 0, 0, 0, 0, 0, 426, 638, 159, 2, 72, 48337, 39625, 39625, 0, 0, 0, 0, 0, 0, 91937, 74846, 90250, 74670, 0, 0, 0, 0, 0, 0, 20004, 20004, '', 0, 3, 1, 50, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=48337);
INSERT INTO `creature_loot_template` VALUES 
(48337, 56440, 0, 1, 1, 1, 1),
(48337, 56441, 0, 1, 1, 1, 1),
(48337, 56442, 0, 1, 1, 1, 1),
(48337, 56443, 0, 1, 1, 1, 1),
(48337, 56444, 0, 1, 1, 1, 1),
(48337, 67495, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (40177, 48702, 48702, 0, 0, 0, 33429, 0, 0, 0, 'Forgemaster Throngus', '', '', 0, 87, 87, 3, 1771, 1771, 0, 2, 1.42857, 1, 1, 399, 598, 0, 150, 121.2, 1500, 1500, 1, 64, 67584, 12, 0, 0, 0, 0, 0, 319, 478, 120, 5, 72, 40177, 0, 0, 0, 0, 0, 0, 0, 0, 74976, 90737, 75056, 90756, 0, 0, 0, 0, 0, 733, 19994, 19994, '', 0, 3, 1, 25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 151, 1, 617299967, 0, 'boss_forgemaster_throngus', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=40177);
INSERT INTO `creature_loot_template` VALUES 
(40177, 56118, 0, 1, 1, 1, 1),
(40177, 56119, 0, 1, 1, 1, 1),
(40177, 56120, 0, 1, 1, 1, 1),
(40177, 56121, 0, 1, 1, 1, 1),
(40177, 56122, 0, 1, 1, 1, 1),
(40177, 71638, 0.6659, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (48702, 0, 0, 0, 0, 0, 33429, 0, 0, 0, 'Forgemaster Throngus (1)', '', '', 0, 87, 87, 3, 1771, 1771, 0, 2, 1.42857, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 64, 67584, 12, 0, 0, 0, 0, 0, 426, 638, 159, 5, 72, 48702, 0, 0, 0, 0, 0, 0, 0, 0, 74976, 90737, 75056, 90756, 0, 0, 0, 0, 0, 733, 19994, 19994, '', 0, 3, 1, 60, 1, 1, 0, 0, 0, 0, 0, 0, 0, 151, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=48702);
INSERT INTO `creature_loot_template` VALUES 
(48702, 56445, 0, 1, 1, 1, 1),
(48702, 56446, 0, 1, 1, 1, 1),
(48702, 56447, 0, 1, 1, 1, 1),
(48702, 56448, 0, 1, 1, 1, 1),
(48702, 56449, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (40484, 48822, 48822, 0, 0, 0, 33428, 0, 0, 0, 'Erudax', 'The Duke of Below', '', 0, 87, 87, 3, 16, 16, 0, 1, 1.28571, 1, 1, 399, 598, 0, 150, 121.2, 1500, 1500, 1, 64, 134219776, 12, 0, 0, 0, 0, 0, 319, 478, 120, 10, 2147483720, 40484, 0, 0, 0, 0, 0, 0, 0, 0, 79466, 91081, 75789, 91091, 0, 0, 0, 0, 0, 0, 20003, 20003, '', 0, 3, 1, 35, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 617299967, 0, 'boss_erudax', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=40484);
INSERT INTO `creature_loot_template` VALUES 
(40484, 56128, 0, 1, 2, 1, 1),
(40484, 56129, 0, 1, 2, 1, 1),
(40484, 56130, 0, 1, 1, 1, 1),
(40484, 56131, 0, 1, 1, 1, 1),
(40484, 56132, 0, 1, 1, 1, 1),
(40484, 56133, 0, 1, 2, 1, 1),
(40484, 56135, 0, 1, 2, 1, 1),
(40484, 56136, 0, 1, 1, 1, 1),
(40484, 56137, 0, 1, 1, 1, 1),
(40484, 56138, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (48822, 0, 0, 0, 0, 0, 33428, 0, 0, 0, 'Erudax (1)', 'The Duke of Below', '', 0, 87, 87, 3, 16, 16, 0, 1, 1.28571, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 64, 134219776, 12, 0, 0, 0, 0, 0, 426, 638, 159, 10, 2147483720, 48822, 0, 0, 0, 0, 0, 0, 0, 0, 79466, 91081, 75789, 91091, 0, 0, 0, 0, 0, 0, 20003, 20003, '', 0, 3, 1, 75, 1, 1, 0, 0, 0, 0, 0, 0, 0, 133, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=48822);
INSERT INTO `creature_loot_template` VALUES 
(48822, 52078, 100, 1, 0, 1, 1),
(48822, 56455, 0, 1, 2, 1, 1),
(48822, 56456, 0, 1, 1, 1, 1),
(48822, 56457, 0, 1, 2, 1, 1),
(48822, 56458, 0, 1, 1, 1, 1),
(48822, 56459, 0, 1, 1, 1, 1),
(48822, 56460, 0, 1, 2, 1, 1),
(48822, 56461, 0, 1, 1, 1, 1),
(48822, 56462, 0, 1, 1, 1, 1),
(48822, 56463, 0, 1, 1, 1, 1),
(48822, 56464, 0, 1, 2, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43438, 49642, 49642, 0, 0, 0, 33477, 0, 0, 0, 'Corborus', '', '', 0, 87, 87, 3, 14, 14, 0, 2.8, 1.42857, 1, 1, 532, 798, 0, 199, 95, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 4, 72, 43438, 0, 43438, 0, 0, 0, 0, 0, 0, 92650, 82415, 0, 0, 0, 0, 0, 0, 0, 0, 19998, 19998, '', 0, 3, 1, 25, 1, 1, 0, 52506, 0, 0, 0, 0, 0, 154, 1, 617299967, 0, 'boss_corborus', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=43438);
INSERT INTO `creature_loot_template` VALUES 
(43438, 55792, 0, 1, 1, 1, 1),
(43438, 55793, 0, 1, 1, 1, 1),
(43438, 55794, 0, 1, 1, 1, 1),
(43438, 55795, 0, 1, 1, 1, 1),
(43438, 55796, 0, 1, 1, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=43438);
INSERT INTO `skinning_loot_template` VALUES 
(43438, 52185, 5.1356, 1, 0, 1, 1),
(43438, 52327, 11.2572, 1, 0, 1, 1),
(43438, 60486, 83.6072, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49642, 0, 0, 0, 0, 0, 33477, 0, 0, 0, 'Corborus (1)', '', '', 0, 87, 87, 3, 14, 14, 0, 2.8, 1.42857, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 4, 72, 49642, 0, 43438, 0, 0, 0, 0, 0, 0, 92650, 82415, 0, 0, 0, 0, 0, 0, 0, 0, 19998, 19998, '', 0, 3, 1, 35, 1, 1, 0, 52506, 0, 0, 0, 0, 0, 154, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49642);
INSERT INTO `creature_loot_template` VALUES 
(49642, 52506, -100, 1, 0, 1, 1),
(49642, 56328, 0, 1, 1, 1, 1),
(49642, 56329, 0, 1, 1, 1, 1),
(49642, 56330, 0, 1, 1, 1, 1),
(49642, 56331, 0, 1, 1, 1, 1),
(49642, 56332, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (43214, 49538, 49538, 0, 0, 0, 36476, 0, 0, 0, 'Slabhide', '', '', 0, 87, 87, 3, 16, 16, 0, 2.8, 1.42857, 1, 1, 532, 798, 0, 199, 95, 2000, 2000, 1, 67141696, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 2, 72, 43214, 0, 43214, 0, 0, 0, 0, 0, 0, 92265, 92656, 80807, 0, 0, 0, 0, 0, 0, 0, 20011, 20011, '', 0, 1, 11.25, 38, 1, 1, 0, 0, 0, 0, 0, 0, 0, 154, 1, 617299967, 0, 'boss_slabhide', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=43214);
INSERT INTO `creature_loot_template` VALUES 
(43214, 55797, 0, 1, 1, 1, 1),
(43214, 55798, 0, 1, 1, 1, 1),
(43214, 55799, 0, 1, 1, 1, 1),
(43214, 55800, 0, 1, 1, 1, 1),
(43214, 55801, 0, 1, 1, 1, 1),
(43214, 71715, 0.4585, 1, 0, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=43214);
INSERT INTO `skinning_loot_template` VALUES 
(43214, 52976, 89.4257, 1, 0, 1, 1),
(43214, 52979, 51.6864, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49538, 0, 0, 0, 0, 0, 36476, 0, 0, 0, 'Slabhide (1)', '', '', 0, 87, 87, 3, 16, 16, 0, 2.8, 1.42857, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 67141696, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 2, 72, 49538, 0, 43214, 0, 0, 0, 0, 0, 0, 92265, 92656, 80807, 0, 0, 0, 0, 0, 0, 0, 20011, 20011, '', 0, 1, 11.25, 50, 1, 1, 0, 0, 0, 0, 0, 0, 0, 154, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49538);
INSERT INTO `creature_loot_template` VALUES 
(49538, 56333, 0, 1, 1, 1, 1),
(49538, 56334, 0, 1, 1, 1, 1),
(49538, 56335, 0, 1, 1, 1, 1),
(49538, 56336, 0, 1, 1, 1, 1),
(49538, 56337, 0, 1, 1, 1, 1),
(49538, 63043, 1, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (42188, 49654, 49654, 0, 0, 0, 36475, 0, 0, 0, 'Ozruk', '', '', 0, 87, 87, 3, 14, 14, 0, 0.888888, 1.5873, 1, 1, 532, 798, 0, 199, 95, 2000, 2000, 1, 32832, 134219776, 0, 0, 0, 0, 0, 0, 426, 638, 159, 5, 584, 42188, 42188, 42188, 0, 0, 0, 0, 0, 0, 78939, 92659, 78835, 92429, 0, 0, 0, 0, 0, 867, 20011, 20011, '', 0, 3, 1, 38, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 617299967, 0, 'boss_ozruk', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=42188);
INSERT INTO `creature_loot_template` VALUES 
(42188, 55802, 0, 1, 1, 1, 1),
(42188, 55803, 0, 1, 1, 1, 1),
(42188, 55804, 0, 1, 1, 1, 1),
(42188, 55810, 0, 1, 1, 1, 1),
(42188, 55811, 0, 1, 1, 1, 1);

DELETE FROM `skinning_loot_template` WHERE (`entry`=42188);
INSERT INTO `skinning_loot_template` VALUES 
(42188, 52177, 21.3923, 1, 0, 1, 1),
(42188, 52178, 20.9305, 1, 0, 1, 1),
(42188, 52179, 20.1916, 1, 0, 1, 1),
(42188, 52180, 20.0185, 1, 0, 1, 1),
(42188, 52181, 21.2768, 1, 0, 1, 1),
(42188, 52182, 19.8684, 1, 0, 1, 1),
(42188, 60485, 80.351, 1, 0, 1, 1),
(42188, 60486, 19.0834, 1, 0, 1, 1);

DELETE FROM `pickpocketing_loot_template` WHERE (`entry`=42188);
INSERT INTO `pickpocketing_loot_template` VALUES 
(42188, 52177, 45.0766, 1, 0, 1, 2),
(42188, 52178, 44.6389, 1, 0, 1, 2),
(42188, 52179, 35.2298, 1, 0, 1, 2),
(42188, 52180, 38.512, 1, 0, 1, 1),
(42188, 52181, 42.8884, 1, 0, 1, 2),
(42188, 52182, 38.512, 1, 0, 1, 2),
(42188, 60485, 100, 1, 0, 1, 1),
(42188, 60486, 35.2298, 1, 0, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49654, 0, 0, 0, 0, 0, 36475, 0, 0, 0, 'Ozruk (1)', '', '', 0, 87, 87, 3, 14, 14, 0, 0.888888, 1.5873, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 32832, 134219776, 0, 0, 0, 0, 0, 0, 426, 638, 159, 5, 584, 49654, 42188, 42188, 0, 0, 0, 0, 0, 0, 78939, 92659, 78835, 92429, 0, 0, 0, 0, 0, 867, 20011, 20011, '', 0, 3, 1, 40, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 617299967, 1, '', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=49654);
INSERT INTO `creature_loot_template` VALUES 
(49654, 56338, 0, 1, 1, 1, 1),
(49654, 56339, 0, 1, 1, 1, 1),
(49654, 56340, 0, 1, 1, 1, 1),
(49654, 56341, 0, 1, 1, 1, 1),
(49654, 56342, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (42333, 49624, 49624, 0, 0, 0, 26448, 0, 0, 0, 'High Priestess Azil', '', '', 0, 87, 87, 3, 14, 14, 0, 1.2, 1.42857, 1, 1, 532, 798, 0, 199, 105, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 7, 72, 42333, 0, 0, 0, 0, 0, 0, 0, 0, 92663, 79345, 82858, 79050, 0, 0, 0, 0, 0, 903, 20024, 20024, '', 0, 3, 1, 25, 1, 1, 0, 0, 0, 0, 0, 0, 0, 721, 1, 617299967, 0, 'boss_high_priestess_azil', 15595);

DELETE FROM `creature_loot_template` WHERE (`entry`=42333);
INSERT INTO `creature_loot_template` VALUES 
(42333, 55812, 0, 1, 2, 1, 1),
(42333, 55813, 0, 1, 2, 1, 1),
(42333, 55814, 0, 1, 1, 1, 1),
(42333, 55815, 0, 1, 2, 1, 1),
(42333, 55816, 0, 1, 1, 1, 1),
(42333, 55817, 0, 1, 2, 1, 1),
(42333, 55818, 0, 1, 2, 1, 1),
(42333, 55819, 0, 1, 1, 1, 1),
(42333, 55820, 0, 1, 2, 1, 1),
(42333, 55821, 0, 1, 1, 1, 1);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (49624, 0, 0, 0, 0, 0, 26448, 0, 0, 0, 'High Priestess Azil (1)', '', '', 0, 87, 87, 3, 14, 14, 0, 1.2, 1.42857, 1, 1, 532, 798, 0, 199, 125, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 7, 72, 49624, 0, 0, 0, 0, 0, 0, 0, 0, 92663, 79345, 82858, 79050, 0, 0, 0, 0, 0, 903, 20024, 20024, '', 0, 3, 1, 30, 1, 1, 0, 0, 0, 0, 0, 0, 0, 721, 1, 617299967, 1, '', 15595);


DELETE FROM `creature_loot_template` WHERE (`entry`=49624);
INSERT INTO `creature_loot_template` VALUES 
(49624, 52078, 100, 1, 0, 1, 1),
(49624, 56343, 0, 1, 2, 1, 1),
(49624, 56344, 0, 1, 2, 1, 1),
(49624, 56345, 0, 1, 1, 1, 1),
(49624, 56346, 0, 1, 2, 1, 1),
(49624, 56347, 0, 1, 1, 1, 1),
(49624, 56348, 0, 1, 2, 1, 1),
(49624, 56349, 0, 1, 2, 1, 1),
(49624, 56350, 0, 1, 1, 1, 1),
(49624, 56351, 0, 1, 1, 1, 1),
(49624, 56352, 0, 1, 2, 1, 1);

