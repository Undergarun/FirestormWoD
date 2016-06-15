SET @NPC_ID = 900012;
SET @NPC_DISPLAY_ID = 59808;
SET @NPC_NAME = "Warlock";
SET @NPC_SUBNAME = "PvP Transmo";
SET @SLOT = 1;

DELETE FROM `creature_template` where `entry` = @NPC_ID;
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_req`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
 VALUES (@NPC_ID, '0', '0', @NPC_DISPLAY_ID, '0', '0', '0', @NPC_NAME, '', @NPC_SUBNAME, '', '0', '100', '100', '0', '0', '35', '129', '0', '2.8', '2.14286', '1.14286', '1', '0', '0', '35', '1200', '2000', '1', '1', '1', '64', '2048', '0', '0', '0', '0', '0', '0', '0', '0', '7', '76', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '100', '', '0', '3', '1', '112.5', '150', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '', '20779'); 

DELETE FROM `npc_vendor` where `entry` = @NPC_ID;

 -- Season 1                                        

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28402, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28404, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30186, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30187, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30188, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30201, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 2                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31973, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31974, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31975, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31976, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32787, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32799, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 3                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33676, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33677, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33678, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33679, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33882, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33884, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 4                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35003, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35004, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35005, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35006, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35138, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35153, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 5                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41992, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42004, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42010, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42016, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45831, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 124571, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 6                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41898, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41903, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41909, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41993, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42005, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42011, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42017, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 7                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41994, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42006, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42012, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 42018, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 117316, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 8                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51329, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51338, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51537, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51538, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51539, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51540, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 125173, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60478, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60479, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60480, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60482, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60626, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60630, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60634, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65528, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65530, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65571, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65572, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65599, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 125173, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70549, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70314, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70315, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70316, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70318, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70362, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70363, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70364, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70425, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70427, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70468, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70469, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70496, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70362, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70363, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11                                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73483, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73485, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73486, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73487, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73635, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73638, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73656, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73658, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73659, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73660, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73744, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73638, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84815, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84842, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84859, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84878, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84923, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84955, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84977, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84354, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84357, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84359, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84499, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84500, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84501, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84503, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85005, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85021, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85035, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85047, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85078, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85093, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84359, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91111, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91117, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91121, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91420, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91422, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91424, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91428, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94333, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94376, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94398, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94403, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94441, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94442, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94471, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91112, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91118, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91421, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91423, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91425, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91429, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91121, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100207, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100213, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100217, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100510, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100512, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100514, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100518, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 (Recolor)                          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100563, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100606, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100628, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100633, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100671, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100672, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100701, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100208, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100214, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100511, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100513, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100515, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100519, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100217, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
