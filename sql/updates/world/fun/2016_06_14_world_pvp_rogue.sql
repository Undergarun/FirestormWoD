SET @NPC_ID = 900010;
SET @NPC_DISPLAY_ID = 61879;
SET @NPC_NAME = "Rogue";
SET @NPC_SUBNAME = "PvP Transmo";
SET @SLOT = 1;

DELETE FROM `creature_template` where `entry` = @NPC_ID;
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_req`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
 VALUES (@NPC_ID, '0', '0', @NPC_DISPLAY_ID, '0', '0', '0', @NPC_NAME, '', @NPC_SUBNAME, '', '0', '100', '100', '0', '0', '35', '129', '0', '2.8', '2.14286', '1.14286', '1', '0', '0', '35', '1200', '2000', '1', '1', '1', '64', '2048', '0', '0', '0', '0', '0', '0', '0', '0', '7', '76', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '100', '', '0', '3', '1', '112.5', '150', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', '', '20779'); 

DELETE FROM `npc_vendor` where `entry` = @NPC_ID;

-- Season 1                                        

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25830, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25831, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25832, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25833, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25834, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28422, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28423, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 2                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29995, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30055, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31998, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31999, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32002, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32790, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32802, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 3                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32265, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32347, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32366, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32377, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33701, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33702, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33704, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 4                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35032, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35033, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35034, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35035, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35036, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35141, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35156, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 5                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41649, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41654, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41671, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41682, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41766, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41831, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41835, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41839, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 6                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41650, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41655, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41672, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41683, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41767, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41832, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41836, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41840, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 7                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41651, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41656, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41673, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41684, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41768, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41833, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41837, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41841, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51368, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51369, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51370, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51492, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51493, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51494, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51495, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51496, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60458, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60459, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60460, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60461, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60462, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60586, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60587, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60591, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9 (Recolor)                               

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70515, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70527, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70585, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70586, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70587, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70588, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70589, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70663, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65545, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65546, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65547, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65548, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65549, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65609, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51368, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60591, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70294, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70295, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70296, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70297, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70298, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70347, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70348, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70350, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70442, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70443, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70444, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70445, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70446, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70506, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70347, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70350, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11                                 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73523, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73524, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73525, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73526, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73527, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73530, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73532, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73678, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73679, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73680, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73681, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73682, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73683, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73532, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84792, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84808, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84830, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84848, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84868, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84913, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84948, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84972, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84457, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84458, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84460, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84462, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84463, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84464, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84465, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84466, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84995, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85009, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85023, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85041, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85055, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85089, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85101, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84460, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91335, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91337, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91339, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91340, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91342, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91344, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91346, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91348, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94374, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94377, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94424, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94426, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94443, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94446, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94447, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94470, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91336, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91338, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91341, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91343, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91345, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91347, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91349, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91339, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100425, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100427, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100430, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100432, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100434, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100436, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100438, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103277, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 (Recolor)                          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100604, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100607, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100654, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100656, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100673, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100676, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100677, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 102754, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100426, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100428, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100431, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100433, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100435, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100437, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100439, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103277, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
