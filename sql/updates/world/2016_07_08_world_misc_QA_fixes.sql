UPDATE creature SET phasemask = 4294967295 WHERE id = 70860;
DELETE FROM smart_scripts WHERE entryorguid = 70860 AND id = 0;

UPDATE quest_template_objective SET flags = 0 WHERE id = 271034;
UPDATE quest_template_objective SET flags = 2 WHERE id = 273038;
UPDATE creature_template SET ainame = "SmartAI" WHERE entry IN (74811, 174811, 174812, 174813);