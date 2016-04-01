DELETE FROM spell_script_names WHERE ScriptName = 'spell_sha_pet_spirit_hunt';
INSERT INTO spell_script_names VALUE (58877, 'spell_sha_pet_spirit_hunt');
UPDATE creature_template_addon SET auras = '' WHERE entry = 29264;
