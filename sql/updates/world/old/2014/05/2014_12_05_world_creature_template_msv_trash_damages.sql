-- Missing damages on trash
UPDATE creature_template SET mindmg = 4750, maxdmg = 11118
WHERE entry IN (64068, 64061, 64947, 61131, 77022, 77024, 77026, 77027, 77070, 77072, 77074, 77075, 77113, 77115, 77117, 77118);