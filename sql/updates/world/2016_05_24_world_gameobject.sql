-- delete from gameobject where entry in (233600, 243477);
-- Horde side
INSERT INTO `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`)
VALUES
	(233600, 1, 1637, 5356, 1, 4294967295, 1649.07, -4333.97, 26.7603, 2.06837, 0, 0, 0.859446, 0.511227, 300, 0, 1, 0, 0, NULL);

-- Spell_target_position
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`)
VALUES
	(173143, 0, 1116, 5354.84, -3946.85, 32.73, 3.77);


-- Alliance Side
INSERT INTO `gameobject` ( `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`)
VALUES
	( 243477, 0, 1519, 6292, 1, 4294967295, -8402.49, 200.439, 156.003, 5.21504, 0, 0, 0.509042, -0.860742, 300, 0, 1, 0, 0, NULL);


-- Spell_target_position
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`)
VALUES
	(173140, 0, 1116, 3687.36, -3846.10, 44.98, 2.45);

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`)
VALUES
	(17, 0, 173143, 0, 0, 27, 0, 90, 3, 0, 0, 0, '', '(Horde) level 90 or higher for use the portal to Ashran'),
	(17, 0, 173140, 0, 0, 27, 0, 90, 3, 0, 0, 0, '', '(Alliance) level 90 or higher for use the portal to Ashran');
