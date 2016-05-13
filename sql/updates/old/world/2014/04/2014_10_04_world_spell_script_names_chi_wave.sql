DELETE FROM spell_script_names WHERE ScriptName IN
(('spell_monk_chi_wave'),
('spell_monk_chi_wave_bolt'),
('spell_monk_chi_wave_healing_bolt'));
INSERT INTO spell_script_names VALUES
(115098, 'spell_monk_chi_wave'),
(132467, 'spell_monk_chi_wave_bolt'),
(132463, 'spell_monk_chi_wave_bolt'),
(132464, 'spell_monk_chi_wave_healing_bolt');