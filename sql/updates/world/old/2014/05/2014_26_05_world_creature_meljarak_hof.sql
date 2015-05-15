-- Wind Lord Mel'jarak
-- Template
UPDATE creature_template SET
mindmg = 43563, maxdmg = 69309,
attackpower = 62114, dmg_multiplier = 2,
baseattacktime = 1500, rangeattacktime = 2000,
unit_flags = 32768, unit_flags2 = 2099200,
Health_mod = 620, mechanic_immune_mask = 769638399,
flags_extra = 1
WHERE entry = 62397;

-- Spawn
DELETE FROM creature WHERE id = 62397;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs) VALUES
(62397, 1009, 6297 ,6297, 8, 1, -2074.34, 475.91, 503.57, 3.141593, 604800);