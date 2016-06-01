-- Quest 34724 - Bonus Objective: The Forgotten Caves

UPDATE `creature_template` SET unit_flags= `unit_flags` | 4 WHERE entry= 76548;
UPDATE `creature` SET unit_flags= `unit_flags` | 4 WHERE id= 76548;