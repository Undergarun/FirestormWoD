UPDATE quest_template SET flags = 1376264 WHERE id = 35933;
UPDATE quest_template SET flags = 0x2400008 WHERE id = 34420;
UPDATE quest_template SET flags = 0x2200008 WHERE id = 34425;
UPDATE quest_template SET flags = 0x2400008 WHERE id = 34439;

UPDATE locales_quest SET title_loc6 = "Mata a tus cien", title_loc7 = "Mata a tus cien" WHERE entry = 34429;

DELETE FROM creature WHERE id = 82315;
DELETE FROM creature WHERE id = 82319;

DELETE FROM creature WHERE id = 79583;
insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('79583','1265','7025','7025','1','4294967295','0','1','4428.51','-2464.64','24.8193','3.02035','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);

DELETE FROM creature WHERE id = 79585;
insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('79585','1265','7025','7042','1','4294967295','0','1','4590.9','-2509.69','29.2499','5.51502','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);

UPDATE gameobject SET state = 1 WHERE id = 233197;

UPDATE locales_creature SET name_loc6 = "Maquinista Roca Negra", name_loc7 = "Maquinista Roca Negra" WHERE entry = 80468;
UPDATE locales_creature SET name_loc6 = "Obrero Roca Negra", name_loc7 = "Obrero Roca Negra" WHERE entry = 81063;

REPLACE INTO creature_loot_template VALUES
(81063, 117978, -34, 1, 1, 1, 1, ''),
(80468, 117978, -24, 1, 1, 1, 1, '');

UPDATE quest_template SET ExclusiveGroup = 0 WHERE id = 34958;

UPDATE quest_template SET prevquestid = 35747 WHERE id = 34445;

UPDATE quest_template SET flags = 1310728 WHERE id = 35933;
UPDATE quest_template SET flags = 1310728, flags2 = 8192 WHERE id = 34958;
UPDATE quest_template SET flags = 39845896 WHERE id IN (35884, 34446);