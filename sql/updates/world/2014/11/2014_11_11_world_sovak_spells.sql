DELETE FROM spell_script_names WHERE ScriptName IN
(
"spell_sha_earthgrab",
"spell_sha_molten_earth",
"spell_sha_molten_earth_damage",
"spell_sha_molten_earth_periodic",
"spell_sha_stone_bulwark_totem",
"spell_sha_echo_of_elements"
);

INSERT INTO spell_script_names VALUES
(108283, "spell_sha_echo_of_elements"),
(64695, "spell_sha_earthgrab"),
(170374, "spell_sha_molten_earth"),
(170379, "spell_sha_molten_earth_damage"),
(170377, "spell_sha_molten_earth_periodic"),
(114889, "spell_sha_stone_bulwark_totem");

UPDATE creature_template SET spell1 = 116943 WHERE entry = 60561;
UPDATE spell_proc_event SET procFlags = 87312 WHERE entry = 108283;
