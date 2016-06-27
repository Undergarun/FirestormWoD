UPDATE `world`.`smart_scripts` SET `action_param2` = '3' WHERE `entryorguid` = '68137' AND `source_type` = '0' AND `id` = '13' AND `link` = '0'; 
UPDATE `world`.`smart_scripts` SET `action_param2` = '2' WHERE `entryorguid` = '68137' AND `source_type` = '0' AND `id` = '12' AND `link` = '0'; 

DELETE FROM spell_script_names WHERE ScriptName = "spell_gen_appraisal";
INSERT INTO spell_script_names VALUE(134280, "spell_gen_appraisal");

INSERT INTO areatrigger_teleport VALUE (8708, "Pursuing the black harvest - Roof", 1112, 784.45, 304.44, 320, 0.03);

INSERT INTO `world`.`phase_definitions` (`zoneId`, `entry`, `phaseId`, `comment`) VALUES ('6613', '0', '1982', 'Pursuing the black Harvest (Scenario)'); 