UPDATE spell_bonus_data SET ap_bonus = 0.6 WHERE entry = 114014;
DELETE FROM spell_script_names WHERE ScriptName = 'spell_rog_shuriken_toss';
INSERT INTO spell_script_names VALUE (114014, 'spell_rog_shuriken_toss');