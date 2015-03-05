UPDATE creature_template SET ScriptName = 'npc_ashran_farseer_kylanda' WHERE entry = 82901;
UPDATE creature_template SET ScriptName = 'npc_ashran_chris_clarkie' WHERE entry = 82909;
UPDATE creature_template SET ScriptName = 'npc_ashran_harrison_jones' WHERE entry = 84223;
UPDATE creature_template SET ScriptName = 'npc_ashran_belloc_brightblade' WHERE entry = 88448;
UPDATE creature_template SET ScriptName = 'npc_ashran_angry_zurge' WHERE entry = 83869;
UPDATE creature_template SET ScriptName = 'npc_ashran_mare_wildrunner' WHERE entry = 84660;
UPDATE creature_template SET faction = 1801, npcflag = 1 WHERE entry = 88178;
UPDATE creature_template SET dmg_multiplier = 10 WHERE entry IN (82876, 82877);
UPDATE creature_template SET dmg_multiplier = 5 WHERE entry IN (82880, 82878, 82882, 82883);
UPDATE creature_template SET unit_flags = unit_flags & ~0x00200000 WHERE entry IN (83717, 80256);
UPDATE creature_template SET faction = 123 WHERE entry = 79990;

DELETE FROM creature_equip_template WHERE entry IN (79990, 82880, 88178, 88448);
INSERT INTO creature_equip_template VALUES
(79990, 1, 0, 0, 72876),
(82880, 1, 28771, 43085, 0),
(88178, 1, 76364, 0, 0),
(88448, 1, 24324, 24324, 0);

UPDATE creature SET equipment_id = 1 WHERE id IN (79990, 82880, 88448);

DELETE FROM creature WHERE id IN (82880, 82878, 82882, 82883) AND map = 1191;

DELETE FROM spell_target_position WHERE id IN (167193, 169295);
INSERT INTO spell_target_position VALUES
(167193, 1, 1191, 3950.54, -4037.21, 55.198, 6.261),
(169295, 1, 1191, 5069.33, -4095.62, 49.378, 3.803);