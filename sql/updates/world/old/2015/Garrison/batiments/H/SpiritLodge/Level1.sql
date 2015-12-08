-- HORDE lvl 1

INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(240601, 45, 20508, "Spirit Lodge Work Order", "", "", "", 0.75, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);

-- plot content

DELETE FROM garrison_plot_content WHERE plot_type_or_building = -37 AND faction_index = 0; -- horde
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','79836','6.73913','-0.149402','2.188','6.25459');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87564','3.89666','-5.44055','2.38396','4.78983');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87564','-1.13302','3.74507','2.38443','2.44448');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','-240601','24.2633','5.83074','-0.068878','0.068653');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87760','14.4524','-15.1711','0.498535','5.46826');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','87761','18.5279','-12.338','0.498535','5.61591');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','82571','16.5685','7.13024','2.27689','0.860329');
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','91577','23.0865','9.00142','0.493866','5.96116');

DELETE FROM quest_template_objective WHERE questID IN (38351, 38354);
INSERT INTO quest_template_objective (QuestID, TYPE, ObjectID, Amount, Description) VALUES
(38351, 3, 91574, 1, 'Speak with Varsha'),
(38354, 3, 91574, 1, 'Speak with Var\'Nath');

#79836/NPC - Gez la -- QuestGiver

	UPDATE creature_template SET npcflag = npcflag | 2 WHERE entry = 79836;
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
