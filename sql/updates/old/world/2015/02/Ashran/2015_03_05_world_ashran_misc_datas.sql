UPDATE creature_template SET ScriptName = 'npc_ashran_farseer_kylanda' WHERE entry = 82901;
UPDATE creature_template SET ScriptName = 'npc_ashran_chris_clarkie' WHERE entry = 82909;
UPDATE creature_template SET ScriptName = 'npc_ashran_harrison_jones' WHERE entry = 84223;
UPDATE creature_template SET ScriptName = 'npc_ashran_belloc_brightblade' WHERE entry = 88448;
UPDATE creature_template SET ScriptName = 'npc_ashran_angry_zurge' WHERE entry = 83869;
UPDATE creature_template SET ScriptName = 'npc_ashran_mare_wildrunner' WHERE entry = 84660;
UPDATE creature_template SET faction = 1801, npcflag = 1 WHERE entry = 88178;
UPDATE creature_template SET dmg_multiplier = 10 WHERE entry IN (82876, 82877);
UPDATE creature_template SET dmg_multiplier = 5 WHERE entry IN (82880, 82878, 82882, 82883);
UPDATE creature_template SET faction = 57 WHERE entry = 82880;
UPDATE creature_template SET unit_flags = unit_flags & ~0x00200000 WHERE entry IN (83717, 80256, 84906, 74886, 84878, 84876, 84877);
UPDATE creature_template SET faction = 123 WHERE entry = 79990;
UPDATE creature_template SET InhabitType = 4 WHERE entry IN (84102, 85753, 83681, 84644, 80511, 84926, 82895, 88690, 83720);
UPDATE creature_template SET unit_flags = unit_flags | (0x02|0x200) WHERE entry IN (85668, 86662, 85670, 86647, 85669, 85671, 85673, 86667);
UPDATE creature_template SET ScriptName = 'npc_ashran_chris_clarkie' WHERE entry = 82909;
UPDATE creature_template SET ScriptName = 'npc_ashran_kalgan' WHERE entry = 83830;
UPDATE creature_template SET ScriptName = 'npc_ashran_fura' WHERE entry = 83995;
UPDATE creature_template SET ScriptName = 'npc_ashran_nisstyr' WHERE entry = 83997;
UPDATE creature_template SET ScriptName = 'npc_ashran_atomik' WHERE entry = 82204;
UPDATE creature_template SET ScriptName = 'npc_ashran_marketa' WHERE entry = 82660;
UPDATE creature_template SET ScriptName = 'npc_ashran_ecilam' WHERE entry = 82966;
UPDATE creature_template SET ScriptName = 'npc_ashran_valant_brightsworn' WHERE entry = 82893;
UPDATE creature_template SET ScriptName = 'npc_ashran_anenga' WHERE entry = 81870;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry = 84113;

UPDATE creature SET unit_flags = (0x02|0x200) WHERE id IN (85668, 86662, 85670, 86647, 85669, 85671, 85673, 86667);

DELETE FROM creature_equip_template WHERE entry IN (79990, 82880, 88178, 88448, 84906, 74886, 84878, 84876, 84877);
INSERT INTO creature_equip_template VALUES
(79990, 1, 0, 0, 72876),
(82880, 1, 28771, 43085, 0),
(88178, 1, 76364, 0, 0),
(88448, 1, 24324, 24324, 0),
(84906, 1, 102102, 0, 0),
(74886, 1, 107409, 0, 0),
(84878, 1, 102102, 0, 0),
(84876, 1, 107431, 0, 0),
(84877, 1, 107374, 0, 0);

UPDATE creature SET equipment_id = 1 WHERE id IN (79990, 82880, 88448, 84906, 74886, 84878, 84876, 84877);

DELETE FROM creature WHERE id IN (82880, 82878, 82882, 82883, 83711, 84165, 84166) AND map = 1191;

DELETE FROM spell_target_position WHERE id IN (167193, 169295);
INSERT INTO spell_target_position VALUES
(167193, 1, 1191, 3950.54, -4037.21, 55.198, 6.261),
(169295, 1, 1191, 5069.33, -4095.62, 49.378, 3.803);

DELETE FROM creature_template_addon WHERE entry IN (82880, 82876, 82878, 82882, 82883, 82877, 84102);
INSERT INTO creature_template_addon VALUES
(82876, 0, 0, 0, 1, 0, '177684 178737'),
(82882, 0, 0, 0, 1, 333, ''),
(82883, 0, 0, 0, 1, 333, ''),
(82880, 0, 0, 0, 1, 333, ''),
(82878, 0, 0, 0, 1, 333, ''),
(82877, 0, 0, 0, 1, 0, ''),
(84102, 0, 0, 50331649, 1, 0, '');

DELETE FROM page_text WHERE entry = 4951;
INSERT INTO page_text VALUE
(4951, 'This area has been flagged as too dangerous for casual explorers.$B$BProceed with extreme caution.$B$B- H.J.', 0, 19342);

DELETE FROM locales_page_text WHERE entry = 4951;
INSERT INTO locales_page_text (entry, Text_loc2, Text_loc3, Text_loc6, Text_loc8) VALUE
(
    4951,
    'Cette zone a été signalée comme trop dangereuse pour les explorateurs du dimanche.$B$BRedoublez de précautions.$B$B- H.J.',
    'Dieses Gebiet wurde als zu gefährlich für unerfahrene Entdecker eingestuft.$B$BHöchste Vorsicht ist geboten.$B$B- H.J.',
    'Esta zona está marcada como demasiado peligrosa para exploradores inexpertos.$B$BVe con mucho cuidado.$B$B- H.J.',
    'Эта местность объявлена слишком опасной для обычных путешественников.$B$BСоблюдайте исключительную осторожность.$B$BХ.Д.'
);

DELETE FROM vehicle_template_accessory WHERE entry = 84904;
INSERT INTO vehicle_template_accessory VALUE (84904, 84906, 0, 0, 'Oraggro - Ashran rare mob', 6, 0);
DELETE FROM npc_spellclick_spells WHERE npc_entry = 84904;
INSERT INTO npc_spellclick_spells VALUE (84904, 173426, 1, 0);

DELETE FROM gameobject WHERE id IN (216990, 235733, 231219) AND map = 1191;
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, state) VALUE
(216990, 1191, 6941, 7100, 1, 1, 3998.96, -4051.13, 54.2659, 2.6872, 0, 0, 0.974302, 0.225246, 0),
(235733, 1191, 6941, 7100, 1, 1, 5017.78, -4072.49, 50.255, 0.927787, 0, 0, 0.0942, 0.335614, 1),
(235733, 1191, 6941, 7100, 1, 1, 5000.65, -4062.67, 38.8071, 0.570424, 0, 0, 0.964718, -0.263285, 1),
(235733, 1191, 6941, 7100, 1, 1, 4990.06, -4037.93, 38.809, 1.740675, 0, 0, 0.608727, -0.79338, 1),
(231219, 1191, 6941, 7100, 1, 1, 3944.79, -4072.8, 54.828, 3.89737, 0, 0, 0.929445, -0.368961, 1),
(231219, 1191, 6941, 7100, 1, 1, 3946.10, -4081.52, 54.8282, 2.76012, 0, 0, 0.981865, 0.189584, 1);

UPDATE gameobject SET state = 1 WHERE id IN (236408, 236409, 237736, 237737, 237735, 237734);

DELETE FROM spell_script_names WHERE ScriptName = 'spell_ashran_artifacts_collected';
INSERT INTO spell_script_names VALUE (177393, 'spell_ashran_artifacts_collected');