-- Set cart icon on mini map
UPDATE creature_template SET vehicleId = 2129 WHERE entry = 71071; 
UPDATE creature_template SET vehicleId = 2128 WHERE entry = 71073;

UPDATE creature_template SET unit_flags = 33554570, ScriptName = 'npc_dg_cart' WHERE entry IN (71071, 71073);
UPDATE creature_template SET npcflag = 16777216, unit_flags = 142, unit_flags2 = 0, type_flags = 1048594 WHERE entry = 53194;
DELETE FROM npc_spellclick_spells WHERE npc_entry = 53194;
INSERT INTO npc_spellclick_spells VALUE (53194, 97388, 1, 0);

DELETE FROM spell_script_names WHERE spell_id IN (140876, 141210);
INSERT INTO spell_script_names VALUES
(140876, 'spell_mine_cart'),
(141210, 'spell_mine_cart');