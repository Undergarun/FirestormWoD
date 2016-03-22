UPDATE instance_disabled_rankings SET DifficultyID = 16 WHERE DifficultyID = 16;

DELETE FROM instance_disabled_rankings WHERE DifficultyID = 16 AND EncounterID IN (1721, 1706, 1720, 1722, 1719, 1723, 1705);
INSERT INTO instance_disabled_rankings VALUES
(1721, 16, "Kargath Bladefist - Mythic - Tests"),
(1706, 16, "The Butcher - Mythic - Tests"),
(1720, 16, "Brackenspore - Mythic - Not Scripted"),
(1722, 16, "Tectus - Mythic - Not Scripted"),
(1719, 16, "Twin Ogron - Mythic - Not Scripted"),
(1723, 16, "Ko'ragh - Mythic - Not Scripted"),
(1705, 16, "Imperator Mar'gok - Mythic - Not Scripted");

UPDATE access_requirement SET level_min = 100, level_max = 100 WHERE mapId = 1228 AND difficulty = 16;