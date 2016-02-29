-- LEVEL 1

	INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(240601, 45, 20508, "Spirit Lodge Work Order", "", "", "", 0.75, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);

	-- plot content

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -37 AND faction_index = 0; -- horde
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','79836','6.73913','-0.149402','2.188','6.25459'); -- Gez'la
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87564','3.89666','-5.44055','2.38396','4.78983'); -- Frostwall Shaman
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87564','-1.13302','3.74507','2.38443','2.44448'); -- Frostwall Shaman
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','-240601','24.2633','5.83074','-0.068878','0.068653'); -- work order
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87760','14.4524','-15.1711','0.498535','5.46826'); -- Training Dummy
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87761','18.5279','-12.338','0.498535','5.61591'); -- Dungeoneers Training Dummy
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','82571','16.5685','7.13024','2.27689','0.860329'); -- Atheeru Palestar
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','91577','23.0865','9.00142','0.493866','5.96116'); -- Varsha

	DELETE FROM quest_template_objective WHERE questID IN (38351, 38354);
	INSERT INTO quest_template_objective (QuestID, TYPE, ObjectID, Amount, Description) VALUES
	(38351, 3, 91574, 1, 'Speak with Varsha'),
	(38354, 3, 91574, 1, 'Speak with Var Nath');

	#79836/NPC - Gez la -- QuestGiver

		UPDATE creature_template SET npcflag = npcflag | 1 | 2 WHERE entry = 79836;
		DELETE FROM creature_queststarter WHERE id = 79836;
		DELETE FROM creature_questender WHERE id = 79836;
		INSERT INTO creature_queststarter VALUES (79836, 38351), (79836, 36848);
		INSERT INTO creature_questender VALUES (79836, 38351), (79836, 36848);

	#91577/NPC - Varsha -- Work Orders

		UPDATE creature_template SET npcflag = npcflag | 1, npcflag2 = 32, subname = "Work Orders", scriptname = "npc_Varsha_Garr" WHERE entry = 91577;

	-- Ancient Waygate Protector

	-- What business do you have here, mortal?

	-- My life is dedicated to protecting this ancient waygates. If you wish to activate them, you will need to procure a collection of ancient artifacts known as ogre waystones.

	DELETE FROM creature WHERE id IN (
	89233, -- Talador
	89236, -- Shadowmoon
	89234, -- Gorgrond
	89235, -- Frostfire Ridge
	89232, -- Spires of Arak
	89230); -- Nagrand
	-- 95976 (Tanaan Jungle)

	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('89230','1116','6755','6755','1','4294967295','0','1','3168.17','7478.21','6.23879','5.27425','120','0','0','1','0','0','1','0','0','0','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('89232','1116','6722','7035','1','4294967295','0','1','-694.908','1368.19','32.5861','0.869972','120','0','0','1','0','0','1','0','768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('89233','1116','6662','6662','1','4294967295','0','0','2503.86','2544.55','163.14','4.2176','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('89234','1116','6721','7320','1','4294967295','0','1','7717.12','328.979','131.987','2.21534','120','0','0','1','0','0','1','0','768','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('89235','1116','6720','6778','1','4294967295','0','0','6341.25','3858.31','92.1371','0.339622','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('89236','1116','6719','7122','1','4294967295','0','1','1068.45','-1118.52','-81.8779','0.0155017','120','0','0','1','0','0','1','0','0','0','0','0','0','0',NULL);

	UPDATE creature_template SET npcflag = npcflag | 1, scriptname = "npc_AncientWaygateProtector_Garr" WHERE entry IN (89233, 89236, 89234, 89235, 89232, 89230);

	REPLACE INTO trinity_string (entry, content_default, content_loc2, content_loc3) VALUES
	(14113, "You have not enough Ogre Waystones.", "Vous n avez pas assez de Pierres d accès Ogre.", "Du hast nicht genügend Ogerportalsteine."),
	(14114, "You already have the maximum activated portals count, depending of your Mage Tower/Spirit Lodge level.", "Vous avez déjà le nombre maximum de portails actifs, correspondant au niveau de votre tour des mages.", "Sie haben bereits die maximale anzahl der aktivierten Portale, je nach der Stufe Ihres Magierturms/Geisterhauses."), 
	(14115, "You dont own the required building to use this portal yet.", "Vous n avez pas encore construit le bâtiment nécessaire pour utiliser ce portail.", "Sie besitzen nicht das erforderliche Gebäude, um das Portal zu verwenden");

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -38 AND faction_index = 0; -- horde
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-38','0','87760','20.608','-11.9596','0.498398','5.55403');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-38','0','87760','17.1056','-15.8797','0.498398','5.55403');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-38','0','87761','18.8735','-13.9011','0.498398','5.55403');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-38','0','82571','24.7577','-7.28306','0.43074','0.04839');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-38','0','91577','25.6116','6.2611','0.423203','6.07739');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-38','0','79836','8.04859','0.521322','1.78081','0.069091');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-38','0','-240601','23.8425','7.43829','-0.014267','3.46201');


-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -39 AND faction_index = 0; -- horde
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-39','0','87761','17.7293','-13.8664','0.499817','5.38994');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-39','0','87760','19.9027','-11.9475','0.50206','5.5745');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-39','0','87760','15.7348','-16.2334','0.500519','5.51167');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-39','0','82571','24.4198','-9.07247','0.49794','6.22324');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-39','0','79836','7.7958','0.242902','1.82927','6.08211');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-39','0','-240601','24.2914','6.78726','-0.002365','2.88632');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-39','0','91577','26.5759','6.86518','0.47229','5.87004');


-- Loots

DELIMITER @@

CREATE PROCEDURE `FillConditionTable`(IN `p_LootId` INT, IN `p_DropChance` FLOAT)
BEGIN

	REPLACE INTO creature_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (p_LootId, 117491, p_DropChance, 1, 0, 1, 3, '');
	-- 39 = CONDITION_HAS_BUILDING_TYPE, 13 = BuildingType::Type::MageTower
	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = p_LootId AND SourceEntry = 117491;
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1) VALUES (1, p_LootId, 117491, 39, 13);
END @@
DELIMITER ;


	SET @l_Entry = 75194;
	SET @l_DropRate = 0.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76101;
	SET @l_DropRate = 0;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77036;
	SET @l_DropRate = 0;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 85241;
	SET @l_DropRate = 0;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75211;
	SET @l_DropRate = 0;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87619;
	SET @l_DropRate = 2.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74254;
	SET @l_DropRate = 0.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 78728;
	SET @l_DropRate = 0.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76181;
	SET @l_DropRate = 0;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76157;
	SET @l_DropRate = 0.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77035;
	SET @l_DropRate = 0.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87910;
	SET @l_DropRate = 9.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73472;
	SET @l_DropRate = 1.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73763;
	SET @l_DropRate = 1.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75820;
	SET @l_DropRate = 0.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76176;
	SET @l_DropRate = 0.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74707;
	SET @l_DropRate = 0.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75500;
	SET @l_DropRate = 2.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75406;
	SET @l_DropRate = 0.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81750;
	SET @l_DropRate = 0.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73470;
	SET @l_DropRate = 0.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 89824;
	SET @l_DropRate = 3.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76846;
	SET @l_DropRate = 2.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77037;
	SET @l_DropRate = 0.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75814;
	SET @l_DropRate = 17.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86290;
	SET @l_DropRate = 8.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75057;
	SET @l_DropRate = 0.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73373;
	SET @l_DropRate = 0.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74475;
	SET @l_DropRate = 0.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73371;
	SET @l_DropRate = 0.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77020;
	SET @l_DropRate = 0.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74706;
	SET @l_DropRate = 0.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77927;
	SET @l_DropRate = 0.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80335;
	SET @l_DropRate = 0.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74704;
	SET @l_DropRate = 0.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75240;
	SET @l_DropRate = 0.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76918;
	SET @l_DropRate = 1.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 82900;
	SET @l_DropRate = 9.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77351;
	SET @l_DropRate = 8.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76696;
	SET @l_DropRate = 1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75209;
	SET @l_DropRate = 0.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81269;
	SET @l_DropRate = 12.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80312;
	SET @l_DropRate = 0.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76876;
	SET @l_DropRate = 7.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77034;
	SET @l_DropRate = 0.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73469;
	SET @l_DropRate = 1.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79651;
	SET @l_DropRate = 27.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81780;
	SET @l_DropRate = 17.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87846;
	SET @l_DropRate = 4.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77349;
	SET @l_DropRate = 1.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 78003;
	SET @l_DropRate = 3.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74787;
	SET @l_DropRate = 0.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81808;
	SET @l_DropRate = 23.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81807;
	SET @l_DropRate = 24.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81810;
	SET @l_DropRate = 22.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79267;
	SET @l_DropRate = 2.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 88459;
	SET @l_DropRate = 6.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 78778;
	SET @l_DropRate = 6.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79927;
	SET @l_DropRate = 18.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81809;
	SET @l_DropRate = 23.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 84260;
	SET @l_DropRate = 24.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81811;
	SET @l_DropRate = 26.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87538;
	SET @l_DropRate = 39.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79024;
	SET @l_DropRate = 2.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79266;
	SET @l_DropRate = 3.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 72873;
	SET @l_DropRate = 1.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73676;
	SET @l_DropRate = 30.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 84675;
	SET @l_DropRate = 2.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86256;
	SET @l_DropRate = 18.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79107;
	SET @l_DropRate = 51.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 78437;
	SET @l_DropRate = 24.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79623;
	SET @l_DropRate = 4.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80295;
	SET @l_DropRate = 10.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76694;
	SET @l_DropRate = 1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81806;
	SET @l_DropRate = 19.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80294;
	SET @l_DropRate = 10.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86330;
	SET @l_DropRate = 22.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86326;
	SET @l_DropRate = 24.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86329;
	SET @l_DropRate = 23.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 78174;
	SET @l_DropRate = 8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81270;
	SET @l_DropRate = 13.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74349;
	SET @l_DropRate = 14.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79629;
	SET @l_DropRate = 4.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86275;
	SET @l_DropRate = 16.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80292;
	SET @l_DropRate = 14.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87095;
	SET @l_DropRate = 18.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79626;
	SET @l_DropRate = 7.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81272;
	SET @l_DropRate = 13.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 78782;
	SET @l_DropRate = 16.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 85225;
	SET @l_DropRate = 14.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79650;
	SET @l_DropRate = 22.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87223;
	SET @l_DropRate = 20.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80283;
	SET @l_DropRate = 9.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79621;
	SET @l_DropRate = 9.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77995;
	SET @l_DropRate = 3.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80948;
	SET @l_DropRate = 44.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80949;
	SET @l_DropRate = 45.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 82619;
	SET @l_DropRate = 10.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76935;
	SET @l_DropRate = 8.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 82528;
	SET @l_DropRate = 23.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75426;
	SET @l_DropRate = 40.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73555;
	SET @l_DropRate = 5.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77033;
	SET @l_DropRate = 19.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76599;
	SET @l_DropRate = 12.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86155;
	SET @l_DropRate = 13.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 88207;
	SET @l_DropRate = 6.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 80172;
	SET @l_DropRate = 9.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 82519;
	SET @l_DropRate = 20.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 84765;
	SET @l_DropRate = 9.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74746;
	SET @l_DropRate = 32.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81767;
	SET @l_DropRate = 23.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73490;
	SET @l_DropRate = 36.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 88437;
	SET @l_DropRate = 12.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 83848;
	SET @l_DropRate = 6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75272;
	SET @l_DropRate = 29.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79402;
	SET @l_DropRate = 7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 84764;
	SET @l_DropRate = 10.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87309;
	SET @l_DropRate = 10.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 81764;
	SET @l_DropRate = 16.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 74697;
	SET @l_DropRate = 6.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87221;
	SET @l_DropRate = 18.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79022;
	SET @l_DropRate = 50.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 78578;
	SET @l_DropRate = 6.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 83641;
	SET @l_DropRate = 6.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75198;
	SET @l_DropRate = 31.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87222;
	SET @l_DropRate = 21;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 84760;
	SET @l_DropRate = 12.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 83640;
	SET @l_DropRate = 6.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75210;
	SET @l_DropRate = 28.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75193;
	SET @l_DropRate = 34.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 72967;
	SET @l_DropRate = 81.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 87310;
	SET @l_DropRate = 10.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 72752;
	SET @l_DropRate = 37.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77993;
	SET @l_DropRate = 5.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 88187;
	SET @l_DropRate = 6.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73698;
	SET @l_DropRate = 47.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77992;
	SET @l_DropRate = 4.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 83577;
	SET @l_DropRate = 7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 88657;
	SET @l_DropRate = 92.1;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 76706;
	SET @l_DropRate = 22.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 89127;
	SET @l_DropRate = 13.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79023;
	SET @l_DropRate = 59;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 84978;
	SET @l_DropRate = 28.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 90620;
	SET @l_DropRate = 8.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 83575;
	SET @l_DropRate = 6.6;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79070;
	SET @l_DropRate = 51.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 72833;
	SET @l_DropRate = 41.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79234;
	SET @l_DropRate = 18.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 86279;
	SET @l_DropRate = 87.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75191;
	SET @l_DropRate = 34.2;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 77991;
	SET @l_DropRate = 12.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 73643;
	SET @l_DropRate = 86.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 79231;
	SET @l_DropRate = 21.7;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 72987;
	SET @l_DropRate = 90.3;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75835;
	SET @l_DropRate = 60.4;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 72953;
	SET @l_DropRate = 91.8;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 72955;
	SET @l_DropRate = 90.5;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	SET @l_Entry = 75819;
	SET @l_DropRate = 72.9;
	CALL `FillConditionTable` (@l_Entry, @l_DropRate);

	DROP PROCEDURE `FillConditionTable`;