DELETE FROM creature WHERE id = 80728 AND map = 1228;

UPDATE creature_template SET ScriptName = 'npc_highmaul_maggot' WHERE entry = 80728;

DELETE FROM achievement_criteria_data WHERE criteria_id = 24693;
INSERT INTO achievement_criteria_data VALUE
(24693, 11, 0, 0, 'achievement_highmaul_pair_annihilation');

UPDATE creature_template SET mechanic_immune_mask = 617299807 WHERE entry = 78121;