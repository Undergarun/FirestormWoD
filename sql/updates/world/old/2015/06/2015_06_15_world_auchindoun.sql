/*
    Auchindoun

Only ennemi npc are blizzlike
Kaathar is the wrong id the cosmetic entry is used

*/

-- 
-- Vigilant Kaathar
Delete from creature_template where entry in (86217, 500640, 500641, 286217, 886217);
delete from creature_template_difficulty where entry = 86217;

INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`) VALUES (86217, 'CHALLENGE_MODE_DIFFICULTY', 886217);
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`) VALUES (86217, 'HEROIC_5_DIFFICULTY', 286217);

-- normal
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`) VALUES
(86217, 0, 0, 58263, 0, 0, 0, 'Vigilant Kaathar', NULL, 'Protector of Auchindoun', '', 0, 97, 97, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 3, 0, 2, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 270532712, 0, 75839, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 35, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 0, 1, 86217, 617562111, 1, 'auchindon_boss_kaathar', 19116);

-- heroic
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`) VALUES
(286217, 0, 0, 58263, 0, 0, 0, 'Vigilant Kaathar (2)', NULL, 'Protector of Auchindoun', '', 0, 103, 103, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 3, 0, 4, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 270532712, 0, 286217, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 43.50, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 0, 1, 86217, 617562111, 1, '', 19116);

-- challenge
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`) VALUES
(886217, 0, 0, 58263, 0, 0, 0, 'Vigilant Kaathar (8)', NULL, 'Protector of Auchindoun', '', 0, 103, 103, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 3, 0, 5, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 270532712, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 43.50, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 0, 0, 1, 86217, 617562111, 1, '', 19116);

DELETE FROM creature_loot_template WHERE entry = 286217;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`)
VALUES
    (286217, 109776, 1, 1, 0, 1, 1, ''),
    (286217, 109770, 1, 1, 0, 1, 1, ''),
    (286217, 109777, 1, 1, 0, 1, 1, ''),
    (286217, 109781, 1, 1, 0, 1, 1, ''),
    (286217, 109767, 1, 1, 0, 1, 1, ''),
    (286217, 109768, 1, 1, 0, 1, 1, ''),
    (286217, 110045, 8, 1, 0, 1, 1, ''),
    (286217, 109839, 3, 1, 0, 1, 1, ''),
    (286217, 109837, 3, 1, 0, 1, 1, ''),
    (286217, 109841, 3, 1, 0, 1, 1, ''),
    (286217, 109838, 3, 1, 0, 1, 1, ''),
    (286217, 109779, 2, 1, 0, 1, 1, ''),
    (286217, 109761, 2, 1, 0, 1, 1, ''),
    (286217, 109760, 2, 1, 0, 1, 1, ''),
    (286217, 109831, 2, 1, 0, 1, 1, ''),
    (286217, 109840, 2, 1, 0, 1, 1, ''),
    (286217, 109759, 3, 1, 0, 1, 1, ''),
    (286217, 109762, 2, 1, 0, 1, 1, ''),
    (286217, 109830, 2, 1, 0, 1, 1, ''),
    (286217, 109765, 2, 1, 0, 1, 1, ''),
    (286217, 109780, 2, 1, 0, 1, 1, ''),
    (286217, 109829, 2, 1, 0, 1, 1, ''),
    (286217, 109842, 2, 1, 0, 1, 1, ''),
    (286217, 109835, 2, 1, 0, 1, 1, ''),
    (286217, 109771, 2, 1, 0, 1, 1, ''),
    (286217, 109763, 2, 1, 0, 1, 1, ''),
    (286217, 109782, 2, 1, 0, 1, 1, ''),
    (286217, 109828, 2, 1, 0, 1, 1, ''),
    (286217, 109772, 2, 1, 0, 1, 1, ''),
    (286217, 109764, 2, 1, 0, 1, 1, ''),
    (286217, 109843, 2, 1, 0, 1, 1, ''),
    (286217, 109834, 2, 1, 0, 1, 1, ''),
    (286217, 109826, 2, 1, 0, 1, 1, ''),
    (286217, 109766, 2, 1, 0, 1, 1, ''),
    (286217, 109836, 2, 1, 0, 1, 1, ''),
    (286217, 109832, 2, 1, 0, 1, 1, ''),
    (286217, 109774, 2, 1, 0, 1, 1, ''),
    (286217, 109833, 2, 1, 0, 1, 1, ''),
    (286217, 109773, 2, 1, 0, 1, 1, ''),
    (286217, 109824, 2, 1, 0, 1, 1, ''),
    (286217, 109825, 2, 1, 0, 1, 1, ''),
    (286217, 109827, 1.5, 1, 0, 1, 1, ''),
    (286217, 109775, 1, 1, 0, 1, 1, ''),
    (286217, 109783, 1, 1, 0, 1, 1, ''),
    (286217, 109769, 1, 1, 0, 1, 1, '');

-- Sargerei Cleric
UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96', `Health_mod` = '8' WHERE `entry`='77134';
delete from creature_template_difficulty where entry = 77134;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (77134, 'HEROIC_5_DIFFICULTY', 277134),
    (77134, 'CHALLENGE_MODE_DIFFICULTY', 877134);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (277134, 0, 0, 60883, 0, 0, 0, 'Sargerei Cleric (2)', NULL, '', '', 0, 96, 96, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 77134, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, '10.05', 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (877134, 0, 0, 60883, 0, 0, 0, 'Sargerei Cleric (8)', NULL, '', '', 0, 96, 96, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, '10.05', 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);

-- Cackling Pyromaniac
UPDATE `creature_template` SET `minlevel` = '95', `maxlevel` = '95' WHERE `entry`='76260';

delete from creature_template_difficulty where entry = 76260;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (76260, 'HEROIC_5_DIFFICULTY', 276260),
    (76260, 'CHALLENGE_MODE_DIFFICULTY', 876260);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (276260, 0, 0, 60188, 0, 0, 0, 'Cackling Pyromaniac (2)', NULL, '', '', 0, 101, 101, 5, 0, 14, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1.93, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (876260, 0, 0, 60188, 0, 0, 0, 'Cackling Pyromaniac (8)', NULL, '', '', 0, 101, 101, 5, 0, 14, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1.93, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);


-- Blazing Trickster
UPDATE `creature_template` SET `minlevel` = '95', `maxlevel` = '95' WHERE `entry`='79511';

delete from creature_template_difficulty where entry = 79511;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (79511, 'HEROIC_5_DIFFICULTY', 279511),
    (79511, 'CHALLENGE_MODE_DIFFICULTY', 879511);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (279511, 0, 0, 53112, 0, 0, 0, 'Blazing Trickster (2)', NULL, '', '', 0, 101, 101, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 6, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2097224, 0, 79511, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 0.57, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 268435456, '', 0),
    (879511, 0, 0, 53112, 0, 0, 0, 'Blazing Trickster (8)', NULL, '', '', 0, 101, 101, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 6, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 0.57, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 268435456, '', 0);

-- Felguard
UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96' WHERE `entry`='76259';

delete from creature_template_difficulty where entry = 76259;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (76259, 'HEROIC_5_DIFFICULTY', 276259),
    (76259, 'CHALLENGE_MODE_DIFFICULTY', 876259);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (276259, 0, 0, 39908, 0, 0, 0, 'Felguard (2)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 4.83, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 268435456, '', 0),
    (876259, 0, 0, 39908, 0, 0, 0, 'Felguard (8)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 4.83, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 268435456, '', 0);

-- Zipteq
UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96' , `Health_mod` = '5.2' WHERE `entry`='78734';

delete from creature_template_difficulty where entry = 78734;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (78734, 'HEROIC_5_DIFFICULTY', 278734),
    (78734, 'CHALLENGE_MODE_DIFFICULTY', 878734);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (278734, 0, 0, 23268, 0, 0, 0, 'Zipteq (2)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 6.3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (878734, 0, 0, 23268, 0, 0, 0, 'Zipteq (8)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 6.3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);

-- Durag the Dominator
UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96' , `Health_mod` = '16'WHERE `entry`='77890';

delete from creature_template_difficulty where entry = 77890;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (77890, 'HEROIC_5_DIFFICULTY', 277890),
    (77890, 'CHALLENGE_MODE_DIFFICULTY', 877890);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (277890, 0, 0, 54166, 0, 0, 0, 'Durag the Dominator (2)', NULL, 'Shadow Council', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 5, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 77890, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 20.88, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (877890, 0, 0, 54166, 0, 0, 0, 'Durag the Dominator (8)', NULL, 'Shadow Council', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 5, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 20.88, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);

-- Shaadum
UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96' WHERE `entry`='78728';

delete from creature_template_difficulty where entry = 78728;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (78728, 'HEROIC_5_DIFFICULTY', 278728),
    (78728, 'CHALLENGE_MODE_DIFFICULTY', 878728);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (278728, 0, 0, 18531, 0, 0, 0, 'Shaadum (2)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 78728, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 14.50, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (878728, 0, 0, 18531, 0, 0, 0, 'Shaadum (8)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 14.50, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);

-- Gul'kosh
UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96', `Health_mod` = '24' WHERE `entry`='78437';

delete from creature_template_difficulty where entry = 78437;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (78437, 'HEROIC_5_DIFFICULTY', 278437),
    (78437, 'CHALLENGE_MODE_DIFFICULTY', 878437);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (278437, 0, 0, 54517, 0, 0, 0, 'Gul\'kosh (2)', NULL, 'Shadow Council', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 78437, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 30.93, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (878437, 0, 0, 54517, 0, 0, 0, 'Gul\'kosh (8)', NULL, 'Shadow Council', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 30.93, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);


-- Zar'shuul
UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96' WHERE `entry`='78735';

delete from creature_template_difficulty where entry = 78735;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (78735, 'HEROIC_5_DIFFICULTY', 278735),
    (78735, 'CHALLENGE_MODE_DIFFICULTY', 878735);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (278735, 0, 0, 16575, 0, 0, 0, 'Zar\'shuul (2)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 78735, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 19.33, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (878735, 0, 0, 16575, 0, 0, 0, 'Zar\'shuul (8)', NULL, '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 268435528, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 270532680, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 19.33, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);


-- Grom'tash the Destructor
delete from creature_template_difficulty where entry = 77889;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (77889, 'HEROIC_5_DIFFICULTY', 277889),
    (77889, 'CHALLENGE_MODE_DIFFICULTY', 877889);

UPDATE `creature_template` SET `minlevel` = '96', `maxlevel` = '96', `Health_mod` = '15.2' WHERE `entry`='77889';

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (277889, 0, 0, 54165, 0, 0, 0, 'Grom\'tash the Destructor (2)', NULL, 'Shadow Council', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 77889, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 20.10, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0),
    (877889, 0, 0, 54165, 0, 0, 0, 'Grom\'tash the Destructor (8)', NULL, 'Shadow Council', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 1, 2000, 2000, 1, 1, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 20.10, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 0);


-- Teron'gor
Delete from creature_template where entry in (77734, 500628, 500629);
delete from creature_template_difficulty where entry = 77734;

INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (77734, 'HEROIC_5_DIFFICULTY', 277734),
    (77734, 'CHALLENGE_MODE_DIFFICULTY', 877734);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (77734, 0, 0, 54036, 0, 0, 0, 'Teron\'gor', NULL, 'Shadow Council', '', 0, 97, 97, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 104, 128, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 39, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 77734, 617562111, 268435456, 'auchindon_boss_terongor', 0),
    (277734, 0, 0, 54036, 0, 0, 0, 'Teron\'gor (2)', NULL, 'Shadow Council', '', 0, 103, 103, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 4, 2000, 2000, 1, 1, 1, 104, 128, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 47.36, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 277734, 617562111, 268435456, '', 0),
    (877734, 0, 0, 54036, 0, 0, 0, 'Teron\'gor (8)', NULL, 'Shadow Council', '', 0, 103, 103, 5, 0, 16, 0, 0, 1, 1, 1.14286, 1, 1, 0, 5, 2000, 2000, 1, 1, 1, 104, 128, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 47.36, 13, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 617562111, 268435456, '', 0);

UPDATE `creature_template` SET `lootid`=77734 WHERE `entry`=77734;
DELETE FROM `creature_loot_template` WHERE `entry`=77734;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(77734, 109784, 2, 1, 0, 1, 1,'') /* Sandals of Arcane Mystery */, 
(77734, 109785, 2, 1, 0, 1, 1,'') /* Frost-Touched Boots */, 
(77734, 109786, 2, 1, 0, 1, 1,'') /* Sandals of Swirling Light */, 
(77734, 109787, 2, 1, 0, 1, 1,'') /* Boots of Burning Focus */, 
(77734, 109788, 2, 1, 0, 1, 1,'') /* Bloodfeather Treads */, 
(77734, 109789, 2, 1, 0, 1, 1,'') /* Crystalbinder Sandals */, 
(77734, 109790, 2, 1, 0, 1, 1,'') /* Streamslither Boots */, 
(77734, 109791, 2, 1, 0, 1, 1,'') /* Sharpeye Greaves */, 
(77734, 109792, 2, 1, 0, 1, 1,'') /* Rockhide Treads */, 
(77734, 109793, 2, 1, 0, 1, 1,'') /* Incarnadine Greaves */, 
(77734, 109794, 2, 1, 0, 1, 1,'') /* Verdant Plate Treads */, 
(77734, 109795, 2, 1, 0, 1, 1,'') /* Goldsteel Sabatons */, 
(77734, 109796, 2, 1, 0, 1, 1,'') /* Lightbinder Treads */, 
(77734, 109797, 2, 1, 0, 1, 1,'') /* Felflame Sandals */, 
(77734, 109798, 2, 1, 0, 1, 1,'') /* Leafmender Sandals */, 
(77734, 109799, 2, 1, 0, 1, 1,'') /* Blackwater Boots */, 
(77734, 109800, 2, 1, 0, 1, 1,'') /* Lavalink Stompers */, 
(77734, 109801, 2, 1, 0, 1, 1,'') /* Morningscale Treads */, 
(77734, 109802, 2, 1, 0, 1, 1,'') /* Gutcrusher Stompers */, 
(77734, 109803, 2, 1, 0, 1, 1,'') /* Rivet-Sealed Treads */, 
(77734, 109804, 2, 1, 0, 1, 1,'') /* Trousers of Arcane Mystery */, 
(77734, 109805, 2, 1, 0, 1, 1,'') /* Frost-Touched Legwraps */, 
(77734, 109806, 2, 1, 0, 1, 1,'') /* Leggings of Swirling Light */, 
(77734, 109807, 2, 1, 0, 1, 1,'') /* Lightbinder Leggings */, 
(77734, 109808, 2, 1, 0, 1, 1,'') /* Felflame Legwraps */, 
(77734, 109809, 2, 1, 0, 1, 1,'') /* Legguards of Burning Focus */, 
(77734, 109810, 2, 1, 0, 1, 1,'') /* Bloodfeather Leggings */, 
(77734, 109811, 2, 1, 0, 1, 1,'') /* Crystalbinder Legwraps */, 
(77734, 109812, 2, 1, 0, 1, 1,'') /* Leafmender Legwraps */, 
(77734, 109813, 2, 1, 0, 1, 1,'') /* Streamslither Legguards */, 
(77734, 109814, 2, 1, 0, 1, 1,'') /* Sharpeye Legguards */, 
(77734, 109815, 2, 1, 0, 1, 1,'') /* Rockhide Leggings */, 
(77734, 109816, 2, 1, 0, 1, 1,'') /* Lavalink Legguards */, 
(77734, 109817, 2, 1, 0, 1, 1,'') /* Morningscale Leggings */, 
(77734, 109818, 2, 1, 0, 1, 1,'') /* Incarnadine Legplates */, 
(77734, 109819, 2, 1, 0, 1, 1,'') /* Verdant Plate Legguards */, 
(77734, 109820, 2, 1, 0, 1, 1,'') /* Goldsteel Legplates */, 
(77734, 109821, 2, 1, 0, 1, 1,'') /* Gutcrusher Legplates */, 
(77734, 109822, 2, 1, 0, 1, 1,'') /* Rivet-Sealed Legplates */, 
(77734, 109823, 2, 1, 0, 1, 1,'') /* Blackwater Leggings */, 
(77734, 109904, 2, 1, 0, 1, 1,'') /* Cloak of Cascading Blades */, 
(77734, 109905, 2, 1, 0, 1, 1,'') /* Deadshot Greatcloak */, 
(77734, 109906, 2, 1, 0, 1, 1,'') /* Cloak of Violent Harmony */, 
(77734, 109907, 2, 1, 0, 1, 1,'') /* Felbone Drape */, 
(77734, 109908, 2, 1, 0, 1, 1,'') /* Cloak of Arcane Mysteries */, 
(77734, 109909, 2, 1, 0, 1, 1,'') /* Drape of Iron Sutures */, 
(77734, 109910, 2, 1, 0, 1, 1,'') /* Cloak of Mending Magics */, 
(77734, 109911, 2, 1, 0, 1, 1,'') /* Cape of Annealing Flesh */, 
(77734, 109912, 2, 1, 0, 1, 1,'') /* Bloody-Blade Drape */, 
(77734, 109913, 2, 1, 0, 1, 1,'') /* Headscythe Greatcloak */, 
(77734, 109914, 2, 1, 0, 1, 1,'') /* Drape of Dripping Runnels */, 
(77734, 109915, 2, 1, 0, 1, 1,'') /* Rigid Scale Cloak */, 
(77734, 109916, 2, 1, 0, 1, 1,'') /* Drape of Swirling Deflection */, 
(77734, 109917, 2, 1, 0, 1, 1,'') /* Three-Clefthoof Cape */, 
(77734, 109918, 2, 1, 0, 1, 1,'') /* Soot-Scarred Longcloak */, 
(77734, 109925, 2, 1, 0, 1, 1,'') /* Forgeflame Greatcloak */, 
(77734, 109926, 2, 1, 0, 1, 1,'') /* Drape of Frozen Dreams */, 
(77734, 109927, 2, 1, 0, 1, 1,'') /* Mistwoven Windcloak */, 
(77734, 109928, 2, 1, 0, 1, 1,'') /* Skullcracker Cloak */, 
(77734, 109929, 2, 1, 0, 1, 1,'') /* Cloak of Steeled Nerves */, 
(77734, 110005, 2, 1, 0, 1, 1,'') /* Crystalline Blood Drop */, 
(77734, 110010, 2, 1, 0, 1, 1,'') /* Mote of Corruption */, 
(77734, 110049, 2, 1, 0, 1, 1,'') /* Bloodblade of Teron'Gor */, 
(77734, 110050, 2, 1, 0, 1, 1,'') /* Dagger of the Sanguine Emeralds */;

DELETE FROM `creature_loot_template` WHERE `entry`=277734;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(277734, 109784, 2, 1, 0, 1, 1,'') /* Sandals of Arcane Mystery */, 
(277734, 109785, 2, 1, 0, 1, 1,'') /* Frost-Touched Boots */, 
(277734, 109786, 2, 1, 0, 1, 1,'') /* Sandals of Swirling Light */, 
(277734, 109787, 2, 1, 0, 1, 1,'') /* Boots of Burning Focus */, 
(277734, 109788, 2, 1, 0, 1, 1,'') /* Bloodfeather Treads */, 
(277734, 109789, 2, 1, 0, 1, 1,'') /* Crystalbinder Sandals */, 
(277734, 109790, 2, 1, 0, 1, 1,'') /* Streamslither Boots */, 
(277734, 109791, 2, 1, 0, 1, 1,'') /* Sharpeye Greaves */, 
(277734, 109792, 2, 1, 0, 1, 1,'') /* Rockhide Treads */, 
(277734, 109793, 2, 1, 0, 1, 1,'') /* Incarnadine Greaves */, 
(277734, 109794, 2, 1, 0, 1, 1,'') /* Verdant Plate Treads */, 
(277734, 109795, 2, 1, 0, 1, 1,'') /* Goldsteel Sabatons */, 
(277734, 109796, 2, 1, 0, 1, 1,'') /* Lightbinder Treads */, 
(277734, 109797, 2, 1, 0, 1, 1,'') /* Felflame Sandals */, 
(277734, 109798, 2, 1, 0, 1, 1,'') /* Leafmender Sandals */, 
(277734, 109799, 2, 1, 0, 1, 1,'') /* Blackwater Boots */, 
(277734, 109800, 2, 1, 0, 1, 1,'') /* Lavalink Stompers */, 
(277734, 109801, 2, 1, 0, 1, 1,'') /* Morningscale Treads */, 
(277734, 109802, 2, 1, 0, 1, 1,'') /* Gutcrusher Stompers */, 
(277734, 109803, 2, 1, 0, 1, 1,'') /* Rivet-Sealed Treads */, 
(277734, 109804, 2, 1, 0, 1, 1,'') /* Trousers of Arcane Mystery */, 
(277734, 109805, 2, 1, 0, 1, 1,'') /* Frost-Touched Legwraps */, 
(277734, 109806, 2, 1, 0, 1, 1,'') /* Leggings of Swirling Light */, 
(277734, 109807, 2, 1, 0, 1, 1,'') /* Lightbinder Leggings */, 
(277734, 109808, 2, 1, 0, 1, 1,'') /* Felflame Legwraps */, 
(277734, 109809, 2, 1, 0, 1, 1,'') /* Legguards of Burning Focus */, 
(277734, 109810, 2, 1, 0, 1, 1,'') /* Bloodfeather Leggings */, 
(277734, 109811, 2, 1, 0, 1, 1,'') /* Crystalbinder Legwraps */, 
(277734, 109812, 2, 1, 0, 1, 1,'') /* Leafmender Legwraps */, 
(277734, 109813, 2, 1, 0, 1, 1,'') /* Streamslither Legguards */, 
(277734, 109814, 2, 1, 0, 1, 1,'') /* Sharpeye Legguards */, 
(277734, 109815, 2, 1, 0, 1, 1,'') /* Rockhide Leggings */, 
(277734, 109816, 2, 1, 0, 1, 1,'') /* Lavalink Legguards */, 
(277734, 109817, 2, 1, 0, 1, 1,'') /* Morningscale Leggings */, 
(277734, 109818, 2, 1, 0, 1, 1,'') /* Incarnadine Legplates */, 
(277734, 109819, 2, 1, 0, 1, 1,'') /* Verdant Plate Legguards */, 
(277734, 109820, 2, 1, 0, 1, 1,'') /* Goldsteel Legplates */, 
(277734, 109821, 2, 1, 0, 1, 1,'') /* Gutcrusher Legplates */, 
(277734, 109822, 2, 1, 0, 1, 1,'') /* Rivet-Sealed Legplates */, 
(277734, 109823, 2, 1, 0, 1, 1,'') /* Blackwater Leggings */, 
(277734, 109904, 2, 1, 0, 1, 1,'') /* Cloak of Cascading Blades */, 
(277734, 109905, 2, 1, 0, 1, 1,'') /* Deadshot Greatcloak */, 
(277734, 109906, 2, 1, 0, 1, 1,'') /* Cloak of Violent Harmony */, 
(277734, 109907, 2, 1, 0, 1, 1,'') /* Felbone Drape */, 
(277734, 109908, 2, 1, 0, 1, 1,'') /* Cloak of Arcane Mysteries */, 
(277734, 109909, 2, 1, 0, 1, 1,'') /* Drape of Iron Sutures */, 
(277734, 109910, 2, 1, 0, 1, 1,'') /* Cloak of Mending Magics */, 
(277734, 109911, 2, 1, 0, 1, 1,'') /* Cape of Annealing Flesh */, 
(277734, 109912, 2, 1, 0, 1, 1,'') /* Bloody-Blade Drape */, 
(277734, 109913, 2, 1, 0, 1, 1,'') /* Headscythe Greatcloak */, 
(277734, 109914, 2, 1, 0, 1, 1,'') /* Drape of Dripping Runnels */, 
(277734, 109915, 2, 1, 0, 1, 1,'') /* Rigid Scale Cloak */, 
(277734, 109916, 2, 1, 0, 1, 1,'') /* Drape of Swirling Deflection */, 
(277734, 109917, 2, 1, 0, 1, 1,'') /* Three-Clefthoof Cape */, 
(277734, 109918, 2, 1, 0, 1, 1,'') /* Soot-Scarred Longcloak */, 
(277734, 109925, 2, 1, 0, 1, 1,'') /* Forgeflame Greatcloak */, 
(277734, 109926, 2, 1, 0, 1, 1,'') /* Drape of Frozen Dreams */, 
(277734, 109927, 2, 1, 0, 1, 1,'') /* Mistwoven Windcloak */, 
(277734, 109928, 2, 1, 0, 1, 1,'') /* Skullcracker Cloak */, 
(277734, 109929, 2, 1, 0, 1, 1,'') /* Cloak of Steeled Nerves */, 
(277734, 110005, 2, 1, 0, 1, 1,'') /* Crystalline Blood Drop */, 
(277734, 110010, 2, 1, 0, 1, 1,'') /* Mote of Corruption */, 
(277734, 110049, 2, 1, 0, 1, 1,'') /* Bloodblade of Teron'Gor */, 
(277734, 110050, 2, 1, 0, 1, 1,'') /* Dagger of the Sanguine Emeralds */;


-- Azzakel
delete from creature_template_difficulty where entry = 75927;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (75927, 'HEROIC_5_DIFFICULTY', 275927),
    (75927, 'CHALLENGE_MODE_DIFFICULTY', 875927);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (275927, 0, 0, 52964, 0, 0, 0, 'Azzakel (2)', '', 'Vanguard of the Legion', '', 0, 97, 97, 5, 0, 14, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2097256, 0, 275927, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 37.70, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 155, 0, 0, 1, 75927, 617562111, 268435456, '', 19116),
    (875927, 0, 0, 52964, 0, 0, 0, 'Azzakel (8)', '', 'Vanguard of the Legion', '', 0, 97, 97, 5, 0, 14, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 37.70, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 155, 0, 0, 1, 75927, 617562111, 268435456, '', 19116);

UPDATE `creature_template` SET `lootid`=275927 WHERE `entry`=275927;
DELETE FROM `creature_loot_template` WHERE `entry`=275927;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(275927, 109844, 2, 1, 0, 1, 1,'') /* Gloves of Arcane Mystery */, 
(275927, 109845, 2, 1, 0, 1, 1,'') /* Frost-Touched Gloves */, 
(275927, 109846, 2, 1, 0, 1, 1,'') /* Gloves of Swirling Light */, 
(275927, 109847, 2, 1, 0, 1, 1,'') /* Lightbinder Gloves */, 
(275927, 109848, 2, 1, 0, 1, 1,'') /* Gauntlets of Burning Focus */, 
(275927, 109849, 2, 1, 0, 1, 1,'') /* Bloodfeather Grips */, 
(275927, 109850, 1, 1, 0, 1, 1,'') /* Crystalbinder Gloves */, 
(275927, 109851, 2, 1, 0, 1, 1,'') /* Leafmender Grips */, 
(275927, 109852, 1, 1, 0, 1, 1,'') /* Streamslither Gauntlets */, 
(275927, 109853, 1, 1, 0, 1, 1,'') /* Sharpeye Gauntlets */, 
(275927, 109854, 1, 1, 0, 1, 1,'') /* Rockhide Gloves */, 
(275927, 109855, 1, 1, 0, 1, 1,'') /* Lavalink Grips */, 
(275927, 109856, 3, 1, 0, 1, 1,'') /* Incarnadine Gauntlets */, 
(275927, 109857, 3, 1, 0, 1, 1,'') /* Verdant Plate Grips */, 
(275927, 109858, 3, 1, 0, 1, 1,'') /* Goldsteel Gloves */, 
(275927, 109859, 3, 1, 0, 1, 1,'') /* Gutcrusher Gauntlets */, 
(275927, 109860, 3, 1, 0, 1, 1,'') /* Rivet-Sealed Crushers */, 
(275927, 109861, 2, 1, 0, 1, 1,'') /* Felflame Grips */, 
(275927, 109862, 2, 1, 0, 1, 1,'') /* Blackwater Grips */, 
(275927, 109863, 2, 1, 0, 1, 1,'') /* Morningscale Gauntlet */, 
(275927, 109864, 2, 1, 0, 1, 1,'') /* Bracers of Arcane Mystery */, 
(275927, 109865, 2, 1, 0, 1, 1,'') /* Frost-Touched Wristwraps */, 
(275927, 109866, 2, 1, 0, 1, 1,'') /* Bracers of Swirling Light */, 
(275927, 109867, 2, 1, 0, 1, 1,'') /* Lightbinder Wristwraps */, 
(275927, 109868, 2, 1, 0, 1, 1,'') /* Bracers of Burning Focus */, 
(275927, 109869, 2, 1, 0, 1, 1,'') /* Bloodfeather Bracers */, 
(275927, 109870, 2, 1, 0, 1, 1,'') /* Crystalbinder Wristguards */, 
(275927, 109871, 2, 1, 0, 1, 1,'') /* Leafmender Wraps */, 
(275927, 109872, 1, 1, 0, 1, 1,'') /* Streamslither Bracers */, 
(275927, 109873, 2, 1, 0, 1, 1,'') /* Sharpeye Bracers */, 
(275927, 109874, 1, 1, 0, 1, 1,'') /* Rockhide Wristguards */, 
(275927, 109875, 1, 1, 0, 1, 1,'') /* Lavalink Bracers */, 
(275927, 109876, 3, 1, 0, 1, 1,'') /* Incarnadine Bracers */, 
(275927, 109877, 3, 1, 0, 1, 1,'') /* Verdant Plate Wristguards */, 
(275927, 109878, 2, 1, 0, 1, 1,'') /* Goldsteel Bindings */, 
(275927, 109879, 3, 1, 0, 1, 1,'') /* Gutcrusher Bracers */, 
(275927, 109880, 3, 1, 0, 1, 1,'') /* Rivet-Sealed Bracers */, 
(275927, 109881, 2, 1, 0, 1, 1,'') /* Felflame Bracers */, 
(275927, 109882, 2, 1, 0, 1, 1,'') /* Blackwater Wristguards */, 
(275927, 109883, 1, 1, 0, 1, 1,'') /* Morningscale Bracers */, 
(275927, 109995, 8, 1, 0, 1, 1,'') /* Blood Seal of Azzakel */, 
(275927, 110048, 3, 1, 0, 1, 1,'') /* Azzakel's Boltslinger */, 
(275927, 113681, -7, 1, 0, 3, 10,'') /* Iron Horde Scraps */, 
(275927, 113821, 1, 1, 0, 1, 1,'') /* Battered Iron Horde Helmet */, 
(275927, 113822, 1, 1, 0, 1, 1,'') /* Ravaged Iron Horde Belt */, 
(275927, 113823, 2, 1, 0, 1, 1,'') /* Crusted Iron Horde Pauldrons */, 
(275927, 118197, 1, 1, 0, 1, 1,'') /* Auction Memory Socket */, 
(275927, 118331, 1, 1, 0, 1, 1,'') /* Auction Connecting Valve */, 
(275927, 118332, 1, 1, 0, 1, 1,'') /* Auction A.D.D.O.N.S Installer */;

-- Nyami
delete from creature_template_difficulty where entry = 76177;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (76177, 'HEROIC_5_DIFFICULTY', 276177),
    (76177, 'CHALLENGE_MODE_DIFFICULTY', 876177);

UPDATE `creature_template` SET `Health_mod` = '28.10' WHERE `entry`='76177';

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (276177, 0, 0, 53089, 0, 0, 0, 'Soulbinder Nyami (2)', '', 'Shadow Council', '', 0, 97, 97, 5, 0, 14, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 276177, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40.60, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 1, 0, 617562111, 268435456, '', 19116),
    (876177, 0, 0, 53089, 0, 0, 0, 'Soulbinder Nyami (8)', '', 'Shadow Council', '', 0, 97, 97, 5, 0, 14, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 40.60, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 150, 0, 0, 1, 0, 617562111, 268435456, '', 19116);

UPDATE `creature_template` SET `lootid`=76177 WHERE `entry`=76177;
DELETE FROM `creature_loot_template` WHERE `entry`=76177;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(76177, 21525, 0.45, 1, 0, 1, 1,'') /* Green Winter Hat */, 
(76177, 109884, 2, 1, 0, 1, 1,'') /* Chestguard of Burning Focus */, 
(76177, 109885, 2, 1, 0, 1, 1,'') /* Bloodfeather Chestwrap */, 
(76177, 109886, 2, 1, 0, 1, 1,'') /* Crystalbinder Chestguard */, 
(76177, 109887, 2, 1, 0, 1, 1,'') /* Streamslither Chestguard */, 
(76177, 109888, 2, 1, 0, 1, 1,'') /* Sharpeye Chestguard */, 
(76177, 109889, 2, 1, 0, 1, 1,'') /* Rockhide Ringmail */, 
(76177, 109890, 2, 1, 0, 1, 1,'') /* Lavalink Ringmail */, 
(76177, 109891, 2, 1, 0, 1, 1,'') /* Morningscale Chestguard */, 
(76177, 109892, 2, 1, 0, 1, 1,'') /* Incarnadine Breastplate */, 
(76177, 109893, 3, 1, 0, 1, 1,'') /* Verdant Plate Chest */, 
(76177, 109894, 2, 1, 0, 1, 1,'') /* Goldsteel Chestguard */, 
(76177, 109895, 2, 1, 0, 1, 1,'') /* Gutcrusher Chestplate */, 
(76177, 109896, 2, 1, 0, 1, 1,'') /* Rivet-Sealed Breastplate */, 
(76177, 109897, 2, 1, 0, 1, 1,'') /* Leafmender Robes */, 
(76177, 109898, 2, 1, 0, 1, 1,'') /* Blackwater Wrap */, 
(76177, 109899, 1, 1, 0, 1, 1,'') /* Robes of Arcane Mystery */, 
(76177, 109900, 1, 1, 0, 1, 1,'') /* Frost-Touched Robes */, 
(76177, 109901, 1, 1, 0, 1, 1,'') /* Robes of Swirling Light */, 
(76177, 109902, 1, 1, 0, 1, 1,'') /* Lightbinder Robes */, 
(76177, 109903, 1, 1, 0, 1, 1,'') /* Felflame Robes */, 
(76177, 109950, 3, 1, 0, 1, 1,'') /* Stormshot Choker */, 
(76177, 109951, 3, 1, 0, 1, 1,'') /* Fireblade Collar */, 
(76177, 109952, 3, 1, 0, 1, 1,'') /* Skulltooth Chain */, 
(76177, 109953, 3, 1, 0, 1, 1,'') /* Windseal Necklace */, 
(76177, 109954, 3, 1, 0, 1, 1,'') /* Magister's Chain */, 
(76177, 109955, 3, 1, 0, 1, 1,'') /* Demonbinder Cabochon */, 
(76177, 109956, 3, 1, 0, 1, 1,'') /* Necklace of Endless Shadow */, 
(76177, 109957, 3, 1, 0, 1, 1,'') /* Alc's Pendant of Fiery Dreams */, 
(76177, 109958, 1, 1, 0, 1, 1,'') /* Healing Leaf Necklace */, 
(76177, 109959, 1, 1, 0, 1, 1,'') /* Chain of Soothing Light */, 
(76177, 109960, 1, 1, 0, 1, 1,'') /* Wavesurge Choker */, 
(76177, 109961, 0.97, 1, 0, 1, 1,'') /* Pendant of Purifying Mists */, 
(76177, 109962, 2, 1, 0, 1, 1,'') /* Bloodmist Pendant */, 
(76177, 109963, 2, 1, 0, 1, 1,'') /* Goreclasp Choker */, 
(76177, 109964, 2, 1, 0, 1, 1,'') /* Necklace of Furious Zeal */, 
(76177, 109965, 1, 1, 0, 1, 1,'') /* Fistbreak Choker */, 
(76177, 109966, 1, 1, 0, 1, 1,'') /* Reinforced Bloodsteel Gorget */, 
(76177, 109967, 1, 1, 0, 1, 1,'') /* Necklace of Holy Deflection */, 
(76177, 109968, 2, 1, 0, 1, 1,'') /* Flesh Beetle Brooch */, 
(76177, 109969, 1, 1, 0, 1, 1,'') /* Choker of Weeping Viscera */, 
(76177, 110046, 6, 1, 0, 1, 1,'') /* Hammer of the Soulbinder */, 
(76177, 110047, 6, 1, 0, 1, 1,'') /* Soulcutter Mageblade */, 
(76177, 113681, -7, 1, 0, 3, 10,'') /* Iron Horde Scraps */, 
(76177, 113821, 1, 1, 0, 1, 1,'') /* Battered Iron Horde Helmet */, 
(76177, 113822, 1, 1, 0, 1, 1,'') /* Ravaged Iron Horde Belt */, 
(76177, 113823, 2, 1, 0, 1, 1,'') /* Crusted Iron Horde Pauldrons */, 
(76177, 118197, 0.96, 1, 0, 1, 1,'') /* Auction Memory Socket */, 
(76177, 118331, 1, 1, 0, 1, 1,'') /* Auction Connecting Valve */, 
(76177, 118332, 1, 1, 0, 1, 1,'') /* Auction A.D.D.O.N.S Installer */;


UPDATE `creature_template` SET `lootid`=276177 WHERE `entry`=276177;
DELETE FROM `creature_loot_template` WHERE `entry`=276177;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(276177, 21525, 0.45, 1, 0, 1, 1,'') /* Green Winter Hat */, 
(276177, 109884, 2, 1, 0, 1, 1,'') /* Chestguard of Burning Focus */, 
(276177, 109885, 2, 1, 0, 1, 1,'') /* Bloodfeather Chestwrap */, 
(276177, 109886, 2, 1, 0, 1, 1,'') /* Crystalbinder Chestguard */, 
(276177, 109887, 2, 1, 0, 1, 1,'') /* Streamslither Chestguard */, 
(276177, 109888, 2, 1, 0, 1, 1,'') /* Sharpeye Chestguard */, 
(276177, 109889, 2, 1, 0, 1, 1,'') /* Rockhide Ringmail */, 
(276177, 109890, 2, 1, 0, 1, 1,'') /* Lavalink Ringmail */, 
(276177, 109891, 2, 1, 0, 1, 1,'') /* Morningscale Chestguard */, 
(276177, 109892, 2, 1, 0, 1, 1,'') /* Incarnadine Breastplate */, 
(276177, 109893, 3, 1, 0, 1, 1,'') /* Verdant Plate Chest */, 
(276177, 109894, 2, 1, 0, 1, 1,'') /* Goldsteel Chestguard */, 
(276177, 109895, 2, 1, 0, 1, 1,'') /* Gutcrusher Chestplate */, 
(276177, 109896, 2, 1, 0, 1, 1,'') /* Rivet-Sealed Breastplate */, 
(276177, 109897, 2, 1, 0, 1, 1,'') /* Leafmender Robes */, 
(276177, 109898, 2, 1, 0, 1, 1,'') /* Blackwater Wrap */, 
(276177, 109899, 1, 1, 0, 1, 1,'') /* Robes of Arcane Mystery */, 
(276177, 109900, 1, 1, 0, 1, 1,'') /* Frost-Touched Robes */, 
(276177, 109901, 1, 1, 0, 1, 1,'') /* Robes of Swirling Light */, 
(276177, 109902, 1, 1, 0, 1, 1,'') /* Lightbinder Robes */, 
(276177, 109903, 1, 1, 0, 1, 1,'') /* Felflame Robes */, 
(276177, 109950, 3, 1, 0, 1, 1,'') /* Stormshot Choker */, 
(276177, 109951, 3, 1, 0, 1, 1,'') /* Fireblade Collar */, 
(276177, 109952, 3, 1, 0, 1, 1,'') /* Skulltooth Chain */, 
(276177, 109953, 3, 1, 0, 1, 1,'') /* Windseal Necklace */, 
(276177, 109954, 3, 1, 0, 1, 1,'') /* Magister's Chain */, 
(276177, 109955, 3, 1, 0, 1, 1,'') /* Demonbinder Cabochon */, 
(276177, 109956, 3, 1, 0, 1, 1,'') /* Necklace of Endless Shadow */, 
(276177, 109957, 3, 1, 0, 1, 1,'') /* Alc's Pendant of Fiery Dreams */, 
(276177, 109958, 1, 1, 0, 1, 1,'') /* Healing Leaf Necklace */, 
(276177, 109959, 1, 1, 0, 1, 1,'') /* Chain of Soothing Light */, 
(276177, 109960, 1, 1, 0, 1, 1,'') /* Wavesurge Choker */, 
(276177, 109961, 0.97, 1, 0, 1, 1,'') /* Pendant of Purifying Mists */, 
(276177, 109962, 2, 1, 0, 1, 1,'') /* Bloodmist Pendant */, 
(276177, 109963, 2, 1, 0, 1, 1,'') /* Goreclasp Choker */, 
(276177, 109964, 2, 1, 0, 1, 1,'') /* Necklace of Furious Zeal */, 
(276177, 109965, 1, 1, 0, 1, 1,'') /* Fistbreak Choker */, 
(276177, 109966, 1, 1, 0, 1, 1,'') /* Reinforced Bloodsteel Gorget */, 
(276177, 109967, 1, 1, 0, 1, 1,'') /* Necklace of Holy Deflection */, 
(276177, 109968, 2, 1, 0, 1, 1,'') /* Flesh Beetle Brooch */, 
(276177, 109969, 1, 1, 0, 1, 1,'') /* Choker of Weeping Viscera */, 
(276177, 110046, 6, 1, 0, 1, 1,'') /* Hammer of the Soulbinder */, 
(276177, 110047, 6, 1, 0, 1, 1,'') /* Soulcutter Mageblade */, 
(276177, 113681, -7, 1, 0, 3, 10,'') /* Iron Horde Scraps */, 
(276177, 113821, 1, 1, 0, 1, 1,'') /* Battered Iron Horde Helmet */, 
(276177, 113822, 1, 1, 0, 1, 1,'') /* Ravaged Iron Horde Belt */, 
(276177, 113823, 2, 1, 0, 1, 1,'') /* Crusted Iron Horde Pauldrons */, 
(276177, 118197, 0.96, 1, 0, 1, 1,'') /* Auction Memory Socket */, 
(276177, 118331, 1, 1, 0, 1, 1,'') /* Auction Connecting Valve */, 
(276177, 118332, 1, 1, 0, 1, 1,'') /* Auction A.D.D.O.N.S Installer */;



-- sargerei magus
UPDATE `creature_template` SET `unit_class` = '2', `Health_mod` = '6.23' WHERE `entry`='76263';

delete from creature_template_difficulty where entry = 76263;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (76263, 'HEROIC_5_DIFFICULTY', 276263),
    (76263, 'CHALLENGE_MODE_DIFFICULTY', 876263);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (276263, 0, 0, 60880, 0, 0, 0, 'Sargerei Magus (2)', '', '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 76263, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 68.03, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 19116),
    (876263, 0, 0, 60880, 0, 0, 0, 'Sargerei Magus (8)', '', '', '', 0, 102, 102, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2097224, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 68.03, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 19116);

-- Sargerei Defender
UPDATE `creature_template` SET `Health_mod` = '5' WHERE `entry`='77042';

delete from creature_template_difficulty where entry = 77042;
INSERT INTO `creature_template_difficulty` (`entry`, `difficulty`, `difficulty_entry`)
VALUES
    (77042, 'HEROIC_5_DIFFICULTY', 277042),
    (77042, 'CHALLENGE_MODE_DIFFICULTY', 877042);

INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
VALUES
    (277042, 0, 0, 53136, 0, 0, 0, 'Sargerei Defender (2)', '', '', NULL, 0, 101, 101, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 77042, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 1),
    (877042, 0, 0, 53136, 0, 0, 0, 'Sargerei Defender (8)', '', '', NULL, 0, 101, 101, 5, 0, 16, 0, 0, 1, 1.14286, 1.14286, 1, 1, 0, 2, 2000, 2000, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 1);







