DELETE FROM spell_script_names WHERE ScriptName = 'spell_warl_hellfire_periodic';
INSERT INTO spell_script_names VALUE (1949, 'spell_warl_hellfire_periodic');
UPDATE spell_bonus_data SET dot_bonus = 0.21 WHERE entry = 1949;
UPDATE spell_bonus_data SET direct_bonus = 0.21 WHERE entry = 5857;