DELETE FROM spell_script_names WHERE ScriptName = 'spell_skill_engineering_gearspring_parts';
INSERT INTO spell_script_names VALUE (169080, 'spell_skill_engineering_gearspring_parts');

DELETE FROM spell_script_names WHERE ScriptName = 'spell_skill_alchemy_alchemical_catalyst';
INSERT INTO spell_script_names VALUE (156587, 'spell_skill_alchemy_alchemical_catalyst');

DELETE FROM spell_loot_template WHERE entry IN (171391, 171690);