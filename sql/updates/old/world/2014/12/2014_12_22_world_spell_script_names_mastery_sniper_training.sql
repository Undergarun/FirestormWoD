DELETE FROM spell_script_names WHERE spell_id IN (76659, 168809, 168811);
DELETE FROM spell_script_names WHERE ScriptName = 'spell_hun_sniper_training';
INSERT INTO spell_script_names VALUE
(76659, 'spell_mastery_sniper_training'),
(168809, 'spell_mastery_recently_moved'),
(168811, 'spell_mastery_sniper_training_aura');