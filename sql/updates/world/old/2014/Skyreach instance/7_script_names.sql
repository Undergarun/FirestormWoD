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
update creature_template set ScriptName='mob_SkyreachArcanologist' where entry=76376;
update creature_template set ScriptName='boss_Araknath' where entry=76141;
update creature_template set ScriptName='mob_SkyreachSunTalon' where entry=79093;
update creature_template set ScriptName='boss_Rukhran' where entry=76143;
update creature_template set ScriptName='mob_SkyreachRavenWhisperer' where entry=76154;
update creature_template set ScriptName='mob_SolarFlare' where entry=76227;
update creature_template set ScriptName='mob_DreadRavenHatchling' where entry=76253;
update creature_template set ScriptName='mob_SolarMagnifier' where entry=77559;
update creature_template set ScriptName='mob_WhirlingDervish' where entry=79469;
update creature_template set ScriptName='mob_DefenseConstruct' where entry=76087;
update creature_template set ScriptName='mob_RadiantSupernova' where entry=79463;
update creature_template set ScriptName='mob_YoungKaliri' where entry=76121;
update creature_template set ScriptName='mob_GrandDefenseConstruct' where entry=76145;
update creature_template set ScriptName='boss_HighSageViryx' where entry=76266;
update creature_template set ScriptName='mob_SolarZealot' where entry=76267;
update creature_template set ScriptName='mob_ArakkoaMagnifyingGlassFocus' where entry=76083;
update creature_template set ScriptName='mob_SkyreachShieldConstruct' where entry=76292;
update creature_template set ScriptName='' where entry=84782;
update creature_template set ScriptName='' where entry=82376;

DELETE FROM spell_script_names WHERE spell_id IN (154140, 153544,
                                                          156515,
                                                          153581,
                                                          153315,
                                                          156793,
                                                          159215,
                                                          153827,
                                                          153828,
                                                          160066,
                                                          154159,
                                                          154177,
                                                          165845,
                                                          159381);
														  
INSERT INTO spell_script_names VALUES ('154140', 'spell_Energize');
INSERT INTO spell_script_names VALUES ('153544', 'spell_SpinningBlade');
INSERT INTO spell_script_names VALUES ('156515', 'spell_Storm');
INSERT INTO spell_script_names VALUES ('153581', 'spell_BladeDance');
INSERT INTO spell_script_names VALUES ('153315', 'spell_Windwall');
INSERT INTO spell_script_names VALUES ('156793', 'spell_FourWind');
INSERT INTO spell_script_names VALUES ('159215', 'spell_SolarStorm');
INSERT INTO spell_script_names VALUES ('153827', 'spell_SummonSolarFlare');
INSERT INTO spell_script_names VALUES ('153828', 'spell_Sunstrike');
INSERT INTO spell_script_names VALUES ('160066', 'spell_FlashBang');
INSERT INTO spell_script_names VALUES ('154159', 'spell_VisualEnergize');
INSERT INTO spell_script_names VALUES ('154177', 'spell_VisualEnergize2');
INSERT INTO spell_script_names VALUES ('165845', 'spell_CastDown');
INSERT INTO spell_script_names VALUES ('159381', 'spell_Quills');


update creature_template set modelid1=61365 where entry=76119;
UPDATE creature_template SET flags_extra=128 WHERE entry=76119;

update instance_template set script='instance_Skyreach' where map=1209;