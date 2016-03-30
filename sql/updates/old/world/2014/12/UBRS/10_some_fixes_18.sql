UPDATE creature_template SET ScriptName = 'mob_rallying_banner' WHERE entry = 76222;
UPDATE creature_template SET ScriptName = 'mob_emberscale_whelping' WHERE entry = 76694;
UPDATE creature_template SET rank = 1 WHERE entry IN (77120, 277120, 877120);
UPDATE creature_template SET MovementType = 1, movementId = 84 WHERE entry = 80307;
UPDATE creature_template SET mechanic_immune_mask = 617299839 WHERE entry = 80098;
UPDATE creature_template SET ScriptName = 'mob_zaela_emberscale_ironfight' WHERE entry = 82428;
UPDATE creature_template SET ScriptName = 'mob_zaela_black_iron_wyrm_rider' WHERE entry = 82429;

UPDATE gameobject_template SET ScriptName = 'go_ubrs_whelp_cage' WHERE entry = 227011;

UPDATE areatrigger_template SET ScriptName = 'areatrigger_rallying_banner' WHERE spell_id = 153799;
UPDATE areatrigger_template SET ScriptName = 'areatrigger_vileblood_serum' WHERE spell_id = 161210;
UPDATE areatrigger_template SET ScriptName = 'areatrigger_noxious_spit' WHERE spell_id = 161827;

DELETE FROM gameobject WHERE id = 233436 AND map = 0;
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, animprogress, state) VALUE
(233436, 0, 46, 254, 1, 1, -7482.42, -1323.46, 301.384, 0.118289, 300, 0, 1);

DELETE FROM spell_script_names WHERE spell_id = 154345;
INSERT INTO spell_script_names VALUE (154345, 'spell_electric_pulse');

DELETE FROM creature_text WHERE entry = 86533;
INSERT INTO creature_text VALUES
(86533, 0, 0, 'You saved my life once, it is about time I returned the favor!', 14, 0, 100, 0, 0, 0, 'AWBEE_AGGRO');

DELETE FROM locales_creature_text WHERE entry = 86533;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(86533, 0, 0, 'Vous m''avez sauvé la vie, il est temps que je vous rende la pareille !', 'Me salvaste la vida una vez, es hora de que me volvió el favor!');

UPDATE creature_template SET minlevel = 101, maxlevel = 101, EXP = 5 WHERE entry IN (84462, 84475);