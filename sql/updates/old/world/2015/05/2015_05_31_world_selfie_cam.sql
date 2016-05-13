DELETE FROM spell_script_names WHERE scriptName = "spell_gen_selfie_camera";
INSERT INTO spell_script_names VALUES
(181765, "spell_gen_selfie_camera"),
(181884, "spell_gen_selfie_camera"),
(182483, "spell_gen_selfie_camera"),
(182576, "spell_gen_selfie_camera");

DELETE FROM spell_linked_spell WHERE spell_trigger IN (182562, 181884);
INSERT INTO spell_linked_spell VALUE (182562, 181884, 0, "S.E.L.F.I.E Camera MK II");
INSERT INTO spell_linked_spell VALUES
(-181884, -181767, 0, "S.E.L.F.I.E Filter"),
(-181884, -181779, 0, "S.E.L.F.I.E Filter"),
(-181884, -181773, 0, "S.E.L.F.I.E Filter");