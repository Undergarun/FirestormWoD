DELETE FROM spell_script_names WHERE ScriptName = 'spell_warr_ravager';
INSERT INTO spell_script_names VALUE (152277, 'spell_warr_ravager');
UPDATE creature_template SET ScriptName = 'npc_warrior_ravager' WHERE entry = 76168;