DELETE FROM spell_script_names WHERE spell_id = 51640 AND ScriptName = "spell_taunt_flag_targeting";
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(51640, "spell_taunt_flag_targeting");

DELETE FROM trinity_string WHERE entry = 14089;
INSERT INTO trinity_string(entry, content_default) VALUES(14089, "plants the Flag of Ownership in the corpse of ");