UPDATE creature_template SET ScriptName = 'npc_transcendence_spirit' WHERE entry = 54569;
DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_transcendence_transfer';
INSERT INTO spell_script_names VALUE (119996, 'spell_monk_transcendence_transfer');