SET @SPELL_UNIFORM     := 71450; -- Crown Parcel Service Uniform
DELETE FROM `spell_script_names` WHERE `spell_id`=@SPELL_UNIFORM;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(@SPELL_UNIFORM,'spell_gen_service_uniform');