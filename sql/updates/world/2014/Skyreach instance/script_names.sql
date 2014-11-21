update creature_template set ScriptName='mob_BlindingSolarFlare' where entry=79462;
update creature_template set ScriptName='mob_InitiateOfTheRisingSun' where entry=79466;
update creature_template set ScriptName='mob_DivingChakramSpinner' where entry=76116;
update creature_template set ScriptName='mob_WhirlingDervish' where entry=77605;
update creature_template set ScriptName='mob_BloodedBladeFeather' where entry =76205;
update creature_template set ScriptName='mob_HeraldOfSunrise' where entry=78933;
update creature_template set ScriptName='mob_SoaringChrakramMaster' where entry=76132;
update creature_template set ScriptName='mob_SolarFamiliar' where entry=76097;
update creature_template set ScriptName='mob_SkyreachOverlord' where entry=75975;
update creature_template set ScriptName='boss_Ranjit' where entry=75964;
update creature_template set ScriptName='mob_AdeptOfTheDawn' where entry=79467;
update creature_template set ScriptName='mob_WindFamiliar' where entry=76102;
update creature_template set ScriptName='mob_DrivingGaleCaller' where entry=78932;
update creature_template set ScriptName='mob_AdornedBladetalon' where entry=79303;

INSERT INTO spell_script_names VALUES ('153544', 'spell_SpinningBlade');
INSERT INTO spell_script_names VALUES ('156515', 'spell_Storm');
INSERT INTO spell_script_names VALUES ('153581', 'spell_BladeDance');
INSERT INTO spell_script_names VALUES ('153315', 'spell_Windwall');
INSERT INTO spell_script_names VALUES ('156793', 'spell_FourWind');

update creature_template set modelid1=61365 where entry=76119;
UPDATE creature_template SET flags_extra=128 WHERE entry=76119;