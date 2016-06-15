SET @NPC_ID = 900003;
SET @NPC_DISPLAY_ID = 25444;
SET @NPC_NAME = "Death Knight";
SET @NPC_SUBNAME = "PvP Transmo";
SET @SLOT = 1;

DELETE FROM `creature_template` where `entry` = @NPC_ID;
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_req`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
 VALUES (@NPC_ID, '0', '0', @NPC_DISPLAY_ID, '0', '0', '0', @NPC_NAME, '', @NPC_SUBNAME, '', '0', '100', '100', '0', '0', '35', '129', '0', '2.8', '2.14286', '1.14286', '1', '0', '0', '35', '1200', '2000', '1', '1', '1', '64', '2048', '0', '0', '0', '0', '0', '0', '0', '0', '7', '76', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '100', '', '0', '3', '1', '112.5', '150', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '', '20779'); 

DELETE FROM `npc_vendor` where `entry` = @NPC_ID;

-- Season 5                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40784, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40806, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40824, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40845, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40863, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40879, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40880, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40888, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 6                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40787, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40809, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40827, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40848, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40868, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40881, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40882, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40889, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 7                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40791, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40811, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40830, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40851, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40871, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40883, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40890, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 38670, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 8                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51362, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51363, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51364, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51413, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51414, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51415, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51416, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51418, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60408, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60409, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60410, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60411, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60412, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60508, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60509, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60523, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65593, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65594, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65595, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65596, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65597, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65607, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 34651, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70514, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70244, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70245, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70246, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70247, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70248, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70320, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70321, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70327, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70490, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70491, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70492, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70493, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70494, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70504, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11                                 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73553, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73555, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73616, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73617, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73618, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73619, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73620, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73696, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73738, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73739, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73740, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73741, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73742, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73554, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84795, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84810, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84835, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84853, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84872, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84918, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84949, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84985, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84372, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84373, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84374, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84375, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84376, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84434, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84436, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84438, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84993, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85000, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85028, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85046, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85059, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85086, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85102, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84438, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91147, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91149, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91151, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91153, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91155, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91297, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91303, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91306, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91148, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91150, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91152, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91154, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91156, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91298, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91304, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91306, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100243, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100245, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100247, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100249, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100251, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100393, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100399, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103263, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 (Recolor)                          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100583, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100594, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100595, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100596, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100622, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100659, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100691, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 102771, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100244, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100246, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100248, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100250, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100252, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100394, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100400, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103263, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
