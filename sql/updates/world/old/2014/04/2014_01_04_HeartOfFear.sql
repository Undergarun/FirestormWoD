-- Spellscripts for trash 63048
DELETE FROM spell_script_names WHERE spell_id IN (123421, 123422);
INSERT INTO spell_script_names VALUES
(123421, 'spell_vital_strikes'),
(123422, 'spell_arterial_bleed');

-- Spellscripts for Zor'lok
DELETE FROM spell_script_names WHERE spell_id IN (122761, 122760, 122336);
INSERT INTO spell_script_names VALUES
(122760, 'spell_zorlok_exhale_damage'),
(122761, 'spell_zorlok_exhale'),
(122336, 'spell_sonic_ring');