-- Adjusts some templates

UPDATE `creature_template` SET
`minlevel` = 93,
`maxlevel` = 93,
`exp` = 4,
`faction_A` = 14,
`faction_H` = 14,
`mindmg` = 43563,
`maxdmg` = 69309,
`attackpower` = 62114,
`Health_mod` = 399.9,
`dmg_multiplier` = 2,
`unit_class` = 2000,
`mechanic_immune_mask` = 769638399,
`flags_extra` = 1,
`WDBVerified` = 2000,
`ScriptName` = 'boss_primordius'
WHERE `entry` = 69017;

UPDATE `creature_template` SET
`exp` = 4,
`faction_A` = 14,
`faction_H` = 14,
`mindmg` = 9838,
`maxdmg` = 14331,
`attackpower` = 42296,
`baseattacktime` = 2000,
`rangeattacktime` = 2000,
`type_flags` = 0,
`Health_mod` = 1.44,
`ScriptName` = 'mob_living_fluid'
WHERE `entry` = 69069;

UPDATE `creature_template` SET
`faction_A` = 14,
`faction_H` = 14,
`mindmg` = 9838,
`maxdmg` = 14331,
`attackpower` = 42296,
`baseattacktime` = 2000,
`rangeattacktime` = 2000,
`Health_mod` = 13.6,
`dmg_multiplier` = 7,
`ScriptName` = 'mob_viscous_horror'
WHERE `entry` = 69070;

-- Scriptnames

DELETE FROM spell_script_names WHERE spell_id IN (136051, 139144);
INSERT INTO spell_script_names VALUES 
(136051, "spell_congeal_blood"),
(139144, "spell_primordius_evolution");

-- Boss Spawns

DELETE FROM creature WHERE id = 69017;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES
	('69017','1098','6622','6622','8','1','0','0','5623.95','4712.57','55.1493','4.19242','300','0','0','218068496','0','0','0','0','0','2048','0','0',NULL); -- Primordius

-- Gameobject Spawns 

insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218858','1098','0','0','760','1','5635.65','4581.27','55.7779','5.23599','0','0','-0.5','0.866025','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218859','1098','0','0','760','1','5667.92','4612.98','55.7779','5.75959','0','0','-0.258819','0.965926','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218860','1098','0','0','760','1','5678.84','4656.27','55.7779','0','0','0','0','1','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218861','1098','0','0','760','1','5667.19','4699.53','55.7779','0.523598','0','0','0.258819','0.965926','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218862','1098','0','0','760','1','5635.48','4731.17','55.7779','1.0472','0','0','0.5','0.866025','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218863','1098','0','0','760','1','5548.93','4731.07','55.7779','2.09439','0','0','0.866025','0.500001','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218864','1098','0','0','760','1','5517.29','4699.36','55.7779','2.61799','0','0','0.965925','0.258821','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218865','1098','0','0','760','1','5505.74','4656.07','55.7779','3.14159','0','0','-1','0','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218866','1098','0','0','760','1','5517.39','4612.81','55.7779','3.6652','0','0','-0.965925','0.258821','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218867','1098','0','0','760','1','5549.1','4581.17','55.7779','4.18879','0','0','-0.866025','0.500001','0','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('218553','1098','6622','6622','760','65535','5592.25','4654.9077','55.7931','0.23','0','0','0.0556206','0.998452','300','0','1','0',NULL);




-- Talks

-- Templates All Modes

-- Primordius

UPDATE creature_template SET difficulty_entry_4 = 469017, difficulty_entry_5 = 569017, difficulty_entry_6 = 669017 WHERE entry = 69017;


-- N25

DELETE FROM creature_template WHERE entry = 469017;
insert into `creature_template` values('469017','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47009','0','0','0','Primordius','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','15','2097260','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3100000','3200000','','0','3','1','500','0.01','1','1','0','0','0','0','0','0','0','180','1','0','769638399','1','boss_primordius','2000');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 1199.9 WHERE `entry` = 469017;
-- H10

DELETE FROM creature_template WHERE entry = 569017;
insert into `creature_template` values('569017','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47009','0','0','0','Primordius','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','15','2097260','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3100000','3200000','','0','3','1','500','0.01','1','1','0','0','0','0','0','0','0','180','1','0','769638399','1','boss_primordius','2000');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 473.6 WHERE `entry` = 569017;
-- H25

DELETE FROM creature_template WHERE entry = 669017;
insert into `creature_template` values('669017','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47009','0','0','0','Primordius','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','0','0','1','0','0','0','0','0','0','0','0','0','0','0','15','2097260','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3100000','3200000','','0','3','1','500','0.01','1','1','0','0','0','0','0','0','0','180','1','0','769638399','1','boss_primordius','2000');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 1420.8 WHERE `entry` = 669017;


-- Viscous Horror

UPDATE creature_template SET difficulty_entry_6 = 669070 WHERE entry = 69070;

DELETE FROM creature_template WHERE entry = 669070;
insert into `creature_template` values('669070','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48061','0','0','0','Viscous Horror','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','1','9838','14331','0','42296','1','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','15','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','13.6','1','1','1','0','0','0','0','0','0','0','164','1','0','0','0','mob_viscous_horror','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 27.2 WHERE `entry` = 669070;

-- Living Fluid

UPDATE creature_template SET difficulty_entry_4 = 469069, difficulty_entry_5 = 569069, difficulty_entry_6 = 669069 WHERE entry = 69069;


-- N25

DELETE FROM creature_template WHERE entry = 469069;
insert into `creature_template` values('469069','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48060','0','0','0','Living Fluid','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','0','9838','14331','0','42296','1','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','15','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1.44','1','1','1','0','0','0','0','0','0','0','922','1','0','0','0','mob_living_fluid','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 4.3 WHERE `entry` = 469069;
-- H10

DELETE FROM creature_template WHERE entry = 569069;
insert into `creature_template` values('569069','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48060','0','0','0','Living Fluid','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','0','9838','14331','0','42296','1','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','15','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1.44','1','1','1','0','0','0','0','0','0','0','922','1','0','0','0','mob_living_fluid','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 2.17 WHERE `entry` = 569069;
-- H25

DELETE FROM creature_template WHERE entry = 669069;
insert into `creature_template` values('669069','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48060','0','0','0','Living Fluid','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','0','9838','14331','0','42296','1','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','15','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1.44','1','1','1','0','0','0','0','0','0','0','922','1','0','0','0','mob_living_fluid','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 6.5 WHERE `entry` = 669069;
