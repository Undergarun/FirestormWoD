UPDATE gameobject_template SET ScriptName = 'go_highmaul_instance_portal' WHERE entry = 231770;

UPDATE creature_template SET minlevel = 103, maxlevel = 103, Health_mod = 2.38674 WHERE entry = 78868;

DELETE FROM spell_script_names WHERE spell_id IN (173244, 163180, 159410, 159386);
INSERT INTO spell_script_names VALUES
(173244, 'spell_highmaul_spore_shot'),
(163180, 'spell_highmaul_correct_searchers'),
(159410, 'spell_highmaul_correct_searchers'),
(159386, 'spell_highmaul_correct_searchers');