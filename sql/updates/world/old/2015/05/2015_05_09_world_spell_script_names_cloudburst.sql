DELETE FROM spell_script_names WHERE scriptName IN ("spell_sha_cloudburst_totem", "spell_sha_cloudburst");
INSERT INTO spell_script_names VALUES
(157504, "spell_sha_cloudburst_totem"),
(157503, "spell_sha_cloudburst");
UPDATE creature_template SET spell1 = 157504 WHERE entry = 78001;
