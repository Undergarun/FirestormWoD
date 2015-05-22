-- Blade Lord Ta'yak
UPDATE creature SET spawndist = 0, MovementType = 0 WHERE id = 62543;

-- Templates for Zor'lok, Ta'yak and Garalon
UPDATE creature_template SET minlevel = 93, maxlevel = 93 WHERE entry IN (62980, 62543);
UPDATE creature_template SET VehicleId = 2320 WHERE entry = 62980;
UPDATE creature_template SET npcflag2 = 0, attackpower = 62114, mechanic_immune_mask = 769638399, flags_extra = 1, Health_mod = 345 WHERE entry = 62543;
UPDATE creature_template SET VehicleId = 584 WHERE entry = 62164;

-- Missing template for trash 64355 - Kor'thik Silentwing
UPDATE creature_template SET
minlevel = 92, maxlevel = 92,
faction_A = 16, faction_H = 16,
baseattacktime = 1500, rangeattacktime = 2000,
Health_mod = 1.80,
mindmg = 4750, maxdmg = 11118,
attackpower = 55538,
unit_flags = 32768, unit_flags2 = 2099200,
ScriptName = 'mob_kor_thik_silentwing'
WHERE entry = 64355;

-- Fix wrong data for trashes
UPDATE creature_template SET minlevel = 92, maxlevel = 92 WHERE entry IN (64353, 64357, 64358);
UPDATE creature_template SET Health_mod = 2.70 WHERE entry = 64353;


-- Missing trashes in Ta'yak room
DELETE FROM creature WHERE id = 64355;
INSERT INTO creature (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, movementType) VALUES
(64355, 1009, 8, 1, -2164.398, 348.6389, 420.6158, 3.844991, 7200, 0, 0),
(64355, 1009, 8, 1, -2160.469, 345.8455, 420.6158, 3.60322, 7200, 0, 0),
(64355, 1009, 8, 1, -2158.28, 341.6406, 420.6158, 3.365936, 7200, 0, 0),
(64355, 1009, 8, 1, -2158.408, 333.5121, 420.5076, 2.9572, 7200, 0, 0),
(64355, 1009, 8, 1, -2164.538, 326.1979, 420.6158, 2.45617, 7200, 0, 0),
(64355, 1009, 8, 1, -2160.366, 329.0972, 420.6158, 2.711655, 7200, 0, 0);

-- Adds of Ta'yak
UPDATE creature_template SET VehicleId = 2326 WHERE entry = 63278;
UPDATE creature_template SET scriptname = 'npc_tempest_slash_tornado' WHERE entry = 64373;

-- Prevent some trash to move
UPDATE creature SET MovementType = 0 WHERE id IN (63048, 63049, 63032) AND position_y > 348.5 AND position_y < 403.3;

-- Spellscript
DELETE FROM spell_script_names WHERE spell_id = 122853;
INSERT INTO spell_script_names VALUES (122853, 'spell_tempest_slash');