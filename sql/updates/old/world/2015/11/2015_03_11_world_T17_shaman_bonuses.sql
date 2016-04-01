DELETE FROM spell_script_names WHERE spell_id = 177972 AND ScriptName IN ('spell_sha_high_tide', 'spell_sha_improved_chain_heal', 'spell_sha_tidal_waves');
INSERT INTO spell_script_names VALUES
(177972, 'spell_sha_high_tide'),
(177972, 'spell_sha_improved_chain_heal'),
(177972, 'spell_sha_tidal_waves');

DELETE FROM spell_script_names WHERE spell_id = 170523;
INSERT INTO spell_script_names VALUE
(170523, 'spell_sha_windfury');

DELETE FROM spell_proc_event WHERE entry = 170523;
INSERT INTO spell_proc_event (entry, procFlags, CustomChance, Cooldown) VALUE
(170523, 0x170523, 20, 4);