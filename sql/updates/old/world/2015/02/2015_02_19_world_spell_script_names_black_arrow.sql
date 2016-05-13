DELETE FROM spell_script_names WHERE ScriptName = 'spell_hun_lock_and_load_proc';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_hun_black_arrow';
INSERT INTO spell_script_names VALUE (3674, 'spell_hun_black_arrow');
DELETE FROM spell_bonus_data WHERE entry = 3674;