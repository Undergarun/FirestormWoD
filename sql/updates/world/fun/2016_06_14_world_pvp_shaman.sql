SET @NPC_ID = 900011;
SET @NPC_DISPLAY_ID = 53057;
SET @NPC_NAME = "Shaman";
SET @NPC_SUBNAME = "Transmog";
SET @NPC_SCRIPT = "npc_fun_transmo_vendor";
SET @SLOT = 1;

DELETE FROM `creature_template` where `entry` = @NPC_ID;
INSERT INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_req`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`)
 VALUES (@NPC_ID, '0', '0', @NPC_DISPLAY_ID, '0', '0', '0', @NPC_NAME, '', @NPC_SUBNAME, '', '0', '100', '100', '0', '0', '35', '129', '0', '2.8', '2.14286', '1.14286', '1', '0', '0', '35', '1200', '2000', '1', '1', '1', '64', '2048', '0', '0', '0', '0', '0', '0', '0', '0', '7', '76', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '100', '', '0', '3', '1', '112.5', '150', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '1', @NPC_SCRIPT, '20779'); 

DELETE FROM `npc_vendor` where `entry` = @NPC_ID;

 -- Season 1                                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25997, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25998, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25999, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 26000, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 26001, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28639, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28640, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 2                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32004, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32005, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32006, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32007, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32008, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32792, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32803, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 3                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33386, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33707, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33708, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33709, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33710, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33894, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33896, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 4                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 34373, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 34374, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35044, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35045, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35046, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35142, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 35157, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 5                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41048, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41054, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41059, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41080, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41136, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41150, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41198, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41210, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 6                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41051, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41055, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41060, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41137, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41151, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41199, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41211, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 7                                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40522, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41052, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41056, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41061, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41138, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41152, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 41212, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 8                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51371, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51372, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51373, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51504, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51505, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51506, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51508, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9                                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60433, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60434, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60435, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60436, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60437, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60533, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60534, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60535, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 9 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65559, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65560, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65561, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65562, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65563, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65611, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 125253, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70511, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70269, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70270, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70271, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70272, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70273, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70328, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70329, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70330, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 10 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70456, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70457, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70458, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70459, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70460, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70508, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 124774, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 70330, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73502, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73503, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73504, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73505, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73506, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73507, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73520, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 11 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73661, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73662, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73663, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73664, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73665, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 73676, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84798, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84820, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84844, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84860, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84879, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84924, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84946, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84983, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84467, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84468, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84470, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84472, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84473, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84474, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84475, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84476, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 12 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84988, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84997, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85011, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85029, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85048, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85075, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85097, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 84470, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13                                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91350, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91352, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91356, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91358, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91360, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91362, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91364, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91366, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 (Recolor)                         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94336, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94337, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94342, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94382, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94408, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94409, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94433, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94435, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 13 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91351, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91353, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91359, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91361, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91363, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91365, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91367, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 91356, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14                                    

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100440, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100442, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100448, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100450, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100452, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100454, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100456, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103286, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 (Recolor)                          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100566, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100572, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100612, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100638, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100639, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100663, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100665, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 102624, 0, 0, 5213, 1, 0);
SET @SLOT = @SLOT + 1;
-- Season 14 ELITE
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100441, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100443, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100449, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100451, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100453, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100455, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 100457, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103286, 0, 0, 1435, 1, 0);
SET @SLOT = @SLOT + 1;

-- -------------------------------------------------
-- PVE
-- -------------------------------------------------

-- Tier 1                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16837, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16838, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16839, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16840, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16842, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16843, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16844, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28391, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 1 (Recolor)            

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27454, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27793, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27845, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 27993, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28266, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 2                     

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16943, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16944, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16945, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16946, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16947, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16948, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16949, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 16950, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 3                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22464, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22465, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22466, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22467, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22468, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22469, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22470, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 22471, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 4                            

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28503, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28567, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28610, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29038, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29039, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29040, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29042, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 29043, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 4 (Recolor)           

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25997, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25998, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 25999, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 26000, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 26001, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28629, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 28630, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 5                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30030, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30043, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30185, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30189, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30190, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30192, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30194, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 5 (Recolor)          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32004, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32005, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32006, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32007, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32008, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32791, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32803, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 6                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 30869, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31011, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31015, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31021, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 31024, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 32258, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 34565, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 6 (Recolor)         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33707, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33708, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33709, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33710, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33895, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 33896, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 7                

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39189, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39251, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39307, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39597, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39601, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39602, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39603, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39604, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 7 (Recolor)       

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 39734, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40209, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40272, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40520, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40521, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40522, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40523, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40524, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 8                

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45412, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45414, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45415, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45416, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 40693, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 120387, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 8 (Recolor)       

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46200, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46203, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46208, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46212, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 116039, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 8 (Recolor)          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45151, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45187, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45239, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45259, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45274, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45440, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45513, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46143, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 8 (Recolor)          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45288, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45307, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45333, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45687, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45701, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 45943, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46044, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 9                   

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47265, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47280, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47295, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47687, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48300, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48301, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48303, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48304, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 9 (Recolor)             

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 46990, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47056, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 47090, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48285, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48286, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48287, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48288, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 48289, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 10              

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 49896, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 49900, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50015, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50030, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50059, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50064, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50177, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50202, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 10 (Recolor)              

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50774, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50784, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50830, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50831, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50832, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50833, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50834, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51006, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 10 (Recolor)          

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50652, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50671, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 50687, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51240, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51241, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51242, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51243, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 51244, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 11           

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 58481, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 59310, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 59334, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60318, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60319, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60320, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60321, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60322, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 11 (Recolor) 

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 60235, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65004, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65046, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65055, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65068, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65114, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65125, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 65136, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 12                  

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 69950, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71009, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71254, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71302, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71303, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71304, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71305, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 12 (Recolor)         

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71425, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71451, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71548, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71549, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71550, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 71551, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 124309, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 13               

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76757, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76758, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76759, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 76760, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 77174, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 77183, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 77125, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 13 (Recolor)      

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78370, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78405, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78435, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78666, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78685, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 78704, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 13 (Recolor)      

-- Tier 14               

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86962, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87030, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87055, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87130, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87131, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87132, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87133, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 87134, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 14 (Recolor)      

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85284, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85285, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85286, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85287, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 85288, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86168, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86319, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86342, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 14 (Recolor)      

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86624, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86625, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86626, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86627, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86628, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86826, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86877, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 86900, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 15           

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94999, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95008, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95027, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95035, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96417, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96439, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96445, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 96509, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 15 (Recolor)      

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94275, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94276, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94741, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94757, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94767, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94783, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94802, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 94975, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 15 (Recolor)      

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95643, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95649, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95663, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95673, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95695, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95701, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95714, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 95765, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 16             

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99092, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99093, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99094, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 99095, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103730, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103861, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103940, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 103955, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 16       

INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105741, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105742, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105743, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105744, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105745, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105746, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105747, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 105748, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
-- Tier 16 (Recolor)   
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104414, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104430, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104451, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104473, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104474, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104547, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104558, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
INSERT INTO `npc_vendor`(`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`, `PlayerConditionID`) VALUES(@NPC_ID, @SLOT, 104582, 0, 0, 1002, 1, 16777215);
SET @SLOT = @SLOT + 1;
