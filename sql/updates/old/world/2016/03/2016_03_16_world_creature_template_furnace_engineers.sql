UPDATE creature_template SET spell1 = 159558 WHERE entry IN (88820, 76810);

DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_bomb_overrider" AND spell_id = 159558;
INSERT INTO spell_script_names VALUE (159558, "spell_foundry_bomb_overrider");
