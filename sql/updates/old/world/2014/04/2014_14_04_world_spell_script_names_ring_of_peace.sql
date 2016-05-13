DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_ring_of_peace' OR ScriptName = 'spell_monk_ring_of_peace';
INSERT INTO spell_script_names VALUES
(116844, 'spell_monk_ring_of_peace'),
(140023, 'spell_monk_ring_of_peace_dummy');

DELETE FROM spell_proc_event WHERE entry = 140023;
INSERT INTO spell_proc_event (entry, procFlags) VALUE (140023, 12915028);