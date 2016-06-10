UPDATE quest_template SET Flags = 65536 WHERE Id IN (32317, 32324);
UPDATE quest_template SET Flags = 3211264 WHERE Id = 32307;
DELETE FROM creature_queststarter WHERE quest IN (32307, 32317, 32324, 32325);
DELETE FROM creature_questender WHERE quest IN (32295, 32317, 32324);