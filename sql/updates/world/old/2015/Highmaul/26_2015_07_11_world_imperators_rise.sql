# Gorian Warden
UPDATE creature_template SET
    minlevel       = 101,
    maxlevel       = 101,
    faction        = 2734,
    dmg_multiplier = 4,
    ScriptName     = 'npc_highmaul_gorian_warden'
WHERE entry = 87619;

UPDATE creature_model_info SET bounding_radius = 0.9, combat_reach = 4.5 WHERE modelid = 60422;

# Night-Twisted Fanatic
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 1693,
    dmg_multiplier = 2,
    ScriptName     = 'npc_highmaul_night_twisted_fanatic'
WHERE entry = 87768;

UPDATE creature_model_info SET bounding_radius = 1.24, combat_reach = 4 WHERE modelid = 57226;

# Gorian Rune Mender
UPDATE creature_template SET
    minlevel       = 101,
    maxlevel       = 101,
    faction        = 2734,
    dmg_multiplier = 2,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_gorian_rune_mender'
WHERE entry = 87910;

UPDATE creature_model_info SET bounding_radius = 0.9, combat_reach = 4.5 WHERE modelid = 60608;

# Ogron Warbringer
UPDATE creature_template SET ScriptName = 'npc_highmaul_ogron_warbringer' WHERE entry = 87589;

# Warden Thultok <Defender of the Rise>
UPDATE creature_template SET
    minlevel       = 102,
    maxlevel       = 102,
    faction        = 16,
    dmg_multiplier = 4,
    unit_class     = 8,
    ScriptName     = 'npc_highmaul_warden_thultok'
WHERE entry = 81269;

UPDATE creature_model_info SET bounding_radius = 1.2, combat_reach = 6 WHERE modelid = 60608;

# Equipments
DELETE FROM creature_equip_template WHERE entry IN (87619, 87910, 81269, 86256, 81806, 81780);
INSERT INTO creature_equip_template VALUES
(87619, 1, 110457, 110457, 0),
(87910, 1, 111046, 0, 0),
(81269, 1, 118533, 0, 0),
(86256, 1, 111743, 0, 0),
(81806, 1, 110901, 0, 0),
(81780, 1, 110251, 110160, 0);

DELETE FROM areatrigger_template WHERE spell_id = 175047;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, scale_x, scale_y, flags, ScriptName) VALUE
(175047, 0, 175047, 2, 1, 1, 0x4000, 'areatrigger_highmaul_arcane_residue');