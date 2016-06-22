SET @NPC_ID = 900013;
SET @NPC_DISPLAY_ID = 56855;
SET @NPC_NAME = "Warrior";
SET @NPC_SUBNAME = "Transmog";
SET @NPC_SCRIPT = "npc_fun_transmo_vendor";
SET @SLOT = 1;

DELETE FROM `creature_template` where `entry` = @NPC_ID;
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_req`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
 VALUES (@NPC_ID, '0', '0', @NPC_DISPLAY_ID, '0', '0', '0', @NPC_NAME, '', @NPC_SUBNAME, '', '0', '100', '100', '0', '0', '35', '129', '0', '2.8', '2.14286', '1.14286', '1', '0', '0', '35', '1200', '2000', '1', '1', '1', '64', '2048', '0', '0', '0', '0', '0', '0', '0', '0', '7', '76', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '100', '', '0', '3', '1', '112.5', '150', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', @NPC_SCRIPT, '20779'); 

DELETE FROM `npc_vendor` where `entry` = @NPC_ID;

 -- Season 1                                        

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24544, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24545, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24546, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24547, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24549, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28383, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28385, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 2                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30486, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30487, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30488, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30489, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30490, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32793, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32805, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 3                                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32278, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32341, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32373, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33728, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33732, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33811, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33812, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 4                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35066, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35067, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35068, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35069, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35070, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35146, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35161, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 5                                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40786, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40804, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40823, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40844, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40862, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40879, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40880, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40888, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 6                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40789, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40807, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40826, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40847, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40866, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45824, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 114504, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 7                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40790, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40810, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40829, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40850, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40870, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40884, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 8                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51363, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51364, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51541, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51542, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51543, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51544, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51545, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51362, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60418, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60419, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60420, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60421, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60422, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60512, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60513, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60521, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65580, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65581, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65582, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65583, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65584, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65608, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60508, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70513, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70254, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70255, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70256, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70257, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70258, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70322, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70323, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70326, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70477, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70478, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70479, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70480, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70481, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70505, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70322, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11                                 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73478, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73479, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73480, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73481, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73482, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73552, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73554, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73651, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73652, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73653, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73654, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73655, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73695, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73554, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12                                 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84797, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84822, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84840, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84856, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84876, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84922, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84950, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84986, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 (Recolor)                        

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84435, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84437, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84439, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84504, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84505, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84506, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84507, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84508, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84987, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84999, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85019, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85032, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85063, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85077, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85103, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84439, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13                                 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91299, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91301, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91305, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91430, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91432, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91434, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91436, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91438, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 (Recolor)                        

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94331, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94332, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94401, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94414, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94418, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94440, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94444, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94448, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91300, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91302, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91431, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91433, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91435, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91437, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91439, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91305, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100395, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100397, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100520, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100522, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100524, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100526, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100528, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103262, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100561, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100562, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100631, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100644, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100670, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100674, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100678, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 102702, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100561, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100562, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100631, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100644, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100670, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100674, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100678, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 102702, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;

-- -------------------------------------------------
-- PVE
-- -------------------------------------------------

-- Tier 1                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16861, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16862, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16863, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16864, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16865, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16866, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16867, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16868, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 1 (Recolor)         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27455, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27487, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27788, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27906, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27918, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28207, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28324, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28375, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 2                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16959, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16960, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16961, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16962, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16963, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16964, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16965, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16966, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 2 (Recolor)         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27459, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27520, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27527, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27672, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27813, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27847, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28262, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28390, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 3                

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22416, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22417, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22418, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22419, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22420, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22421, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22422, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22423, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 5                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28502, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28566, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28747, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29011, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29012, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29015, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29016, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29017, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 5 (Recolor)          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24544, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24545, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24546, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24547, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 24549, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28383, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28385, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 6                            

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30915, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30970, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30974, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30976, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30978, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30980, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32232, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32268, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 6 (Recolor)           

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32278, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32341, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32373, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33728, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33732, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33811, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33812, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 7                 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39139, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39195, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39298, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39605, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39606, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39607, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39608, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39609, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 7 (Recolor)        

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39706, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39759, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40525, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40527, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40528, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40529, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40530, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 8            

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45111, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45139, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45166, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45228, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45251, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45267, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45334, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45502, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 8 (Recolor)       

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45295, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45304, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45310, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45542, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45697, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45935, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46039, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 9                 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47253, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47283, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47312, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48386, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48387, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48388, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48389, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48390, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 10                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50078, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50079, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50080, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50081, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50082, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50788, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50802, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51000, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 10 (Recolor)         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 49903, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 49951, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 49960, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 49983, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 49986, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50003, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50036, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50037, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 10 (Recolor)         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51225, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51226, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51227, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51228, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51229, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 113805, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 113806, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 11                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 55059, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 58195, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 59470, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60328, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60329, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60330, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60331, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60332, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 11 (Recolor)          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60229, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65086, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65143, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65269, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65270, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65271, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65272, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65273, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 12               

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 69946, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70933, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70937, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70941, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70942, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70943, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70944, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70945, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 12 (Recolor)     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71405, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71420, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71432, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71443, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71444, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71458, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71459, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 13           

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76983, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76984, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76985, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76986, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76987, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 77171, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 77186, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71993, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 13 (Recolor)  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78367, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78431, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78452, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78657, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78668, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78688, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78705, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 13 (Recolor)  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78378, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78439, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78460, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78752, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78763, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78783, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78800, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 14              

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86955, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86979, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87001, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87192, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87193, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87194, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87195, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87196, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 14 (Recolor)    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86956, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87024, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87048, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87071, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 89921, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 15                

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95003, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95012, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96375, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96730, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96731, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96732, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96733, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96734, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 15 (Recolor)      

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94265, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94266, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94726, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94727, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94764, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94807, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94815, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95063, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 15 (Recolor)      

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95629, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95631, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95652, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95674, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95725, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95733, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95734, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95873, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 16          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99195, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99196, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99197, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99198, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99203, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103738, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103745, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103788, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 16 (Recolor)

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104415, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104439, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104492, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104494, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104513, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104516, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104535, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104644, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 16 (Recolor)
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103735, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103737, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103748, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103840, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104311, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
