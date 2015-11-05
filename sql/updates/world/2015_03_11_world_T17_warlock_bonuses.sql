DELETE FROM spell_script_names WHERE spell_id = 157701 AND ScriptName = 'spell_warl_chaos_bolt';
INSERT INTO spell_script_names VALUES
(157701, 'spell_warl_chaos_bolt');

UPDATE creature_template SET ScriptName = 'spell_npc_warl_inner_demon' WHERE entry = 82927;