DELETE FROM spell_loot_template WHERE entry IN (167950,165564,165304,165456,165460,165461,165463,165464,165465,165466,165467);
DELETE FROM skill_discovery_template WHERE reqSpell IN (167950,165564,165304,165456,165460,165461,165463,165464,165465,165466,165467);
DELETE FROM spell_script_names WHERE spell_id IN (167950,165564,165304,165456,165460,165461,165463,165464,165465,165466,165467);

INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(167950, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(167948, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165564, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165304, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165456, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165460, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165461, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165463, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165464, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165465, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165466, "spell_Inscription_Research");
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(165467, "spell_Inscription_Research");