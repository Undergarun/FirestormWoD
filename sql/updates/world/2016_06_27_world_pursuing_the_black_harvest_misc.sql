DELETE FROM spell_script_names WHERE ScriptName = "spell_gen_appraisal";
INSERT INTO spell_script_names VALUE(134280, "spell_gen_appraisal");

INSERT INTO areatrigger_teleport VALUE (8708, "Pursuing the black harvest - Roof", 1112, 784.45, 304.44, 320, 0.03);

INSERT INTO phase_definitions (`zoneId`, `entry`, `phaseId`, `comment`) VALUES ('6613', '0', '1982', 'Pursuing the black Harvest (Scenario)');
