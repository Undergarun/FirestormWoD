DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_storm_earth_and_fire' OR ScriptName = 'spell_monk_storm_earth_and_fire_stats';
INSERT INTO spell_script_names VALUES
(137639, 'spell_monk_storm_earth_and_fire'),
(138130, 'spell_monk_storm_earth_and_fire_stats');
UPDATE creature_template SET ScriptName = 'npc_monk_spirit' WHERE entry IN (69680, 69791, 69792);