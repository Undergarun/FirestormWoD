UPDATE creature_template SET ScriptName = 'mob_rallying_banner' WHERE entry = 76222;
UPDATE creature_template SET ScriptName = 'mob_emberscale_whelping' WHERE entry = 76694;
UPDATE creature_template SET rank = 1 WHERE entry IN (77120, 277120, 877120);

UPDATE gameobject_template SET ScriptName = 'go_ubrs_whelp_cage' WHERE entry = 227011;

UPDATE areatrigger_template SET ScriptName = 'areatrigger_rallying_banner' WHERE spell_id = 153799;
UPDATE areatrigger_template SET ScriptName = 'areatrigger_vileblood_serum' WHERE spell_id = 161210;

DELETE FROM gameobject WHERE id = 233436 AND map = 0;
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, animprogress, state) VALUE
(233436, 0, 46, 254, 1, 1, -7482.42, -1323.46, 301.384, 0.118289, 300, 0, 1);

DELETE FROM spell_script_names WHERE spell_id = 154345;
INSERT INTO spell_script_names VALUE (154345, 'spell_electric_pulse');