-- Quest 34724 - Bonus Objective: The Forgotten Caves

UPDATE `creature_template` SET unit_flags= `unit_flags` | 4 WHERE entry= 76548;

UPDATE `creature` SET unit_flags= `unit_flags` | 4 WHERE id= 76548;

DELETE FROM creature WHERE id=77602;

UPDATE creature_template SET faction=14, unit_flags=0, unit_flags2=2048, `VehicleId`=0, `type_flags`=0 WHERE entry=76496;
UPDATE creature SET unit_flags=0, unit_flags2=2048, npcFlag=0 WHERE id=76496;
UPDATE `creature_model_info` SET combat_reach=1.5 WHERE modelid=58895;

