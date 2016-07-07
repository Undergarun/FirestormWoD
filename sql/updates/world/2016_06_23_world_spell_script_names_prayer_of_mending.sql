DELETE FROM `spell_script_names` WHERE ScriptName = "spell_pri_prayer_of_mending";
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (33076, "spell_pri_prayer_of_mending");
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (123259, "spell_pri_prayer_of_mending");

DELETE FROM `spell_script_names`  WHERE ScriptName = "spell_pri_prayer_of_mending_aura" AND spell_id = 123262;
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (123262, "spell_pri_prayer_of_mending_aura");