DELETE FROM spell_linked_spell WHERE spell_trigger = 60103;
DELETE FROM spell_script_names WHERE scriptName IN
(
"spell_sha_lava_lash",
"spell_sha_liquid_magma",
"spell_sha_liquid_magma_visual"
);

INSERT INTO spell_script_names VALUES
(157501, "spell_sha_liquid_magma_visual"),
(152255, "spell_sha_liquid_magma"),
(60103, "spell_sha_lava_lash");