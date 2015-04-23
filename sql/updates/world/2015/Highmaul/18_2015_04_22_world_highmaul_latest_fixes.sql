UPDATE gameobject_template SET ScriptName = 'go_highmaul_instance_portal' WHERE entry = 231770;

UPDATE creature_template SET minlevel = 103, maxlevel = 103, Health_mod = 2.38674 WHERE entry = 78868;

DELETE FROM spell_script_names WHERE spell_id = 173244;
INSERT INTO spell_script_names VALUE
(173244, 'spell_highmaul_spore_shot');