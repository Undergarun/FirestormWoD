-- INSTRUCTORS TRASH FOR LORD BLADE TA'YAK --

-- Templates of instructors
-- Kli'thak & Tak'thok
UPDATE creature_template SET
minlevel = 92, maxlevel = 92
WHERE entry IN (64338, 64339);

-- Maltik & Zarik
UPDATE creature_template SET
minlevel = 92, maxlevel = 92,
faction_A = 14, faction_H = 14,
mindmg = 4750, maxdmg = 11118,
attackpower = 55538, dmg_multiplier = 3,
baseattacktime = 1500, rangeattacktime = 2000,
unit_flags = 32768, unit_flags2 = 2099200
WHERE entry IN (64340, 64341);

UPDATE creature_template SET scriptname = "mob_instructor_maltik" WHERE entry = 64340;
UPDATE creature_template SET ScriptName = "mob_instructor_zarik" WHERE entry = 64341;

-- Spawn of instructors
DELETE FROM creature WHERE id BETWEEN 64338 AND 64341;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, MovementType) VALUES
(64338, 1009, 6297, 6297, 8, 1, -2068.488, 337.286, 420.392, 5.881857, 7200, 0, 0),
(64339, 1009, 6297, 6297, 8, 1, -2068.488, 225.424, 420.392, 0.3907246, 7200, 0, 0),
(64340, 1009, 6297, 6297, 8, 1, -2169.888, 337.286, 420.392, 6.14358, 7200, 0, 0),
(64341, 1009, 6297, 6297, 8, 1, -2169.888, 225.424, 420.392, 0.1612043, 7200, 0, 0);