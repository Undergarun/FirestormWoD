INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs) VALUE
(15384, 1190, 4, 72, 1, 1, -11914.12, -3209.61, -114422, 0.189513, 7200);
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = 'npc_world_invisible_trigger' WHERE entry = 15384;