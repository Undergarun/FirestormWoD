-- Amber Pool Stalker
UPDATE creature_template SET
modelid1 = 16925, modelid2 = 16925,
unit_flags = 33554432, unit_flags2 = 2048,
baseattacktime = 2000, rangeattacktime = 2000,
faction_A = 14, faction_H = 14,
minlevel = 90, maxlevel = 90,
ScriptName = "mob_amber_pool_stalker"
WHERE entry = 62762;

-- Spawn
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, npcflag2, unit_flags, unit_flags2, dynamicflags, isActive, protec_anti_doublet) VALUES
(62762, 1009, 6297, 6297, 8, 1, 0, 0, -2409.33, 770.0, 582.98, 0.0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(62762, 1009, 6297, 6297, 8, 1, 0, 0, -2549.87, 630.0, 582.98, 0.0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(62762, 1009, 6297, 6297, 8, 1, 0, 0, -2409.33, 630.0, 582.98, 0.0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(62762, 1009, 6297, 6297, 8, 1, 0, 0, -2549.87, 770.0, 582.98, 0.0, 7200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);