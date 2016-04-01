UPDATE creature_template SET ScriptName = 'npc_rogue_shadow_reflection' WHERE entry = 77726;
DELETE FROM spell_script_names WHERE ScriptName = 'spell_rog_shadow_reflection';
INSERT INTO spell_script_names VALUE (156744, 'spell_rog_shadow_reflection');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_rog_shadow_reflection_proc';
INSERT INTO spell_script_names VALUE (152151, 'spell_rog_shadow_reflection_proc');
DELETE FROM spell_proc_event WHERE entry = 152151;
INSERT INTO spell_proc_event (entry, procFlags) VALUE
(152151, 0x11110);