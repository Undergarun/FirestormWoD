DELETE FROM spell_script_names WHERE spell_id = 139200 AND ScriptName = "spell_warl_drain_fel_energy";
INSERT INTO spell_script_names VALUE(139200, "spell_warl_drain_fel_energy");
DELETE FROM npc_spellclick_spells WHERE npc_entry = 70271 AND spell_id = 101508;
DELETE FROM npc_spellclick_spells WHERE npc_entry = 70271 AND spell_id = 137206;