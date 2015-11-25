#34822/Quest - What we need

	REPLACE INTO creature_loot_template VALUE
	(80174, 111907, -100, 1, 0, 1, 2, '');

	REPLACE INTO gameobject_loot_template VALUES
	(230881, 111907, -100, 1, 0, 1, 2, ''),
	(230879, 111907, -100, 1, 0, 1, 2, ''),
	(230880, 111907, -100, 1, 0, 1, 2, '');

#38293/Quest - Somptuous Fur

	REPLACE INTO creature_queststarter VALUE (91025, 38293);
	REPLACE INTO creature_questender VALUES
	(91025, 38293),
	(91034, 38293);

#91034/NPC - Calvo Klyne <Fur Trader>

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91034;
	DELETE FROM creature WHERE id = 91034;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91034','1330','7004','7210','2','4294967295','0','0','5553.67','4598.79','141.164','5.81351','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91034, 120945, 5815), -- 5 somptuous fur
	(91034, 122549, 5829), -- 60 somptuous fur
	(91034, 122716, 5829),
	(91034, 127724, 5829),
	(91034, 127742, 5829);

#91025/NPC - Dorothy Two

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91025;
	DELETE FROM creature WHERE id = 91025;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91025','1331','7078','7211','2','1','0','0','1937.66','330.089','91.764','6.21431','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91025, 120945, 5815), -- 5 somptuous fur
	(91025, 122549, 5829), -- 60 somptuous fur
	(91025, 122716, 5829),
	(91025, 127724, 5829),
	(91025, 127742, 5829);

#91404/NPC - Samantha Scarlet <Herb Trader>

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91404;
	DELETE FROM creature WHERE id = 91404;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('91404','1331','7078','7211','2','1','0','0','1938.53','331.616','91.7648','5.90408','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91404, 122710, 5824), -- 60 Starflower
	(91404, 122741, 5827), -- 20 Cerulean Pigments
	(91404, 122553, 5827),
	(91404, 122713, 5827),
	(91404, 127723, 5827),
	(91404, 127728, 5827),
	(91404, 122548, 5827),
	(91404, 127746, 5827),
	(91404, 128409, 5827),
	(91404, 128410, 5827),
	(91404, 128411, 5827),
	(91404, 128412, 5827),
	(91404, 128413, 5827),
	(91404, 128414, 5810), -- 10 Alchemical Catalyst
	(91404, 122599, 5810),
	(91404, 122600, 5810),
	(91404, 122605, 5810),
	(91404, 128160, 5810),
	(91404, 120945, 5296); -- 5 Talador Orchid (5822 seems okay also)

#38296/Quest - Herbs Galore

	REPLACE INTO creature_queststarter VALUE (91404, 38296);
	REPLACE INTO creature_questender VALUES
	(91404, 38296),
	(91031, 38296);

#91031/NPC - Nicholas Mitrik

	UPDATE creature_template SET npcflag = 131 WHERE entry = 91031;
	DELETE FROM creature WHERE id = 91031;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91031','1330','7004','7210','2','1','0','0','5554.42','4600.68','141.163','5.51585','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
	REPLACE INTO npc_vendor (entry, item, extendedCost) VALUES
	(91031, 122710, 5824), -- 60 Starflower
	(91031, 122741, 5827), -- 20 Cerulean Pigments
	(91031, 122553, 5827),
	(91031, 122713, 5827),
	(91031, 127723, 5827),
	(91031, 127728, 5827),
	(91031, 122548, 5827),
	(91031, 127746, 5827),
	(91031, 128409, 5827),
	(91031, 128410, 5827),
	(91031, 128411, 5827),
	(91031, 128412, 5827),
	(91031, 128413, 5827),
	(91031, 128414, 5810), -- 10 Alchemical Catalyst
	(91031, 122599, 5810),
	(91031, 122600, 5810),
	(91031, 122605, 5810),
	(91031, 128160, 5810),
	(91031, 120945, 5296); -- 5 Talador Orchid (5822 seems okay also)

#37961/Quest - Bringing the Bass -- Horde

	UPDATE creature_template SET npcflag = 2 WHERE entry = 91072;
	DELETE FROM creature WHERE id = 91072;
--	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91072','1330','7004','7210','2','1','0','0','5588.78','4602.95','136.588','0.0270831','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
-- add spawns later when jukebox is scripted
	REPLACE INTO creature_queststarter VALUE (91072, 37961);
	REPLACE INTO creature_questender   VALUE (91072, 37961);

	REPLACE INTO gameobject_loot_template VALUE (240568, 122565, -100, 1, 0, 1, 1, '');
	REPLACE INTO gameobject_loot_template VALUE (240575, 122566, -100, 1, 0, 1, 1, '');

	REPLACE INTO creature_loot_template VALUE (83988, 122567, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (83987, 122567, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (83989, 122567, -100, 1, 0, 1, 1, '');

	REPLACE INTO creature_loot_template VALUE (75815, 122568, -100, 1, 0, 1, 1, '');
	REPLACE INTO creature_loot_template VALUE (86262, 122569, -100, 1, 0, 1, 1, '');

	-- SPAWNS

#38356/Quest - Bringing the Bass -- Alliance

	DELETE FROM creature WHERE id = 91589;
--	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('91589','1331','7078','7211','2','4294967295','0','0','1924.43','310.853','88.9662','3.78067','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
-- add spawns later when jukebox is scripted

	UPDATE creature_template SET npcflag = 2 WHERE entry = 91589;
	REPLACE INTO creature_queststarter VALUE (91589, 38356);
	REPLACE INTO creature_questender   VALUE (91589, 38356);