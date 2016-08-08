UPDATE quest_template SET RequiredRaces = 18875469 WHERE id = 11821;

DELETE FROM creature_queststarter WHERE id = 25937 AND quest = 11821;
DELETE INTO creature_queststarter WHERE id = 25903 AND quest = 11821;
INSERT INTO creature_queststarter VALUE (25903,11821);


DELETE FROM creature_questender WHERE id = 25903 AND quest = 11821;
INSERT INTO creature_questender VALUE (25903, 11821);