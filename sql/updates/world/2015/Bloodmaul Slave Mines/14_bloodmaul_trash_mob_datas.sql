DELETE FROM spell_script_names WHERE ScriptName IN ('spell_ChainGrip', 'spell_SuppressionField', 'spell_FerociousYell', 'spell_RaiseTheMiners', 'spell_VolcanicEruption', 'spell_LavaSplash');
INSERT INTO spell_script_names VALUES ('151990', 'spell_ChainGrip');
INSERT INTO spell_script_names VALUES ('151581', 'spell_SuppressionField');
INSERT INTO spell_script_names VALUES ('150759', 'spell_FerociousYell');
INSERT INTO spell_script_names VALUES ('150801', 'spell_RaiseTheMiners');
INSERT INTO spell_script_names VALUES ('151698', 'spell_VolcanicEruption');
INSERT INTO spell_script_names VALUES ('152809', 'spell_LavaSplash');

UPDATE creature_template SET ScriptName='mob_BloodmaulEnforcer' WHERE entry=84978;
UPDATE creature_template SET ScriptName='mob_BloodmaulSlaver' WHERE entry=75191;
UPDATE creature_template SET ScriptName='mob_BloodmaulOverseer' WHERE entry=75426;
UPDATE creature_template SET ScriptName='mob_BloodmaulOverseer' WHERE entry=75193;
UPDATE creature_template SET ScriptName='mob_BloodmaulWarder' WHERE entry=75210;
UPDATE creature_template SET ScriptName='mob_BloodmaulOgreMage' WHERE entry=75272;
UPDATE creature_template SET ScriptName='mob_BloodmaulGeomancer' WHERE entry=75198;
UPDATE creature_template SET ScriptName='mob_Slagna' where entry=75406;
UPDATE creature_template SET ScriptName='mob_MoltenEarthElemental' where entry=75209;
UPDATE creature_template SET ScriptName='mob_MagmaLord' where entry=75211;
UPDATE creature_template SET unit_flags = unit_flags | (0x02 | 0x100 | 0x200 | 0x02000000) WHERE entry = 75192;

UPDATE creature_template SET faction=1780 WHERE entry=75857;
UPDATE creature_template SET faction=1780 WHERE entry=75360;
UPDATE creature_template SET faction=1780 WHERE entry=75820;
UPDATE creature_template SET faction=35 WHERE entry=75846;

UPDATE areatrigger_template SET ScriptName='AreaTrigger_SuppresionField' where spell_id=151582;

DELETE FROM areatrigger_scripts WHERE entry = 9725;
INSERT INTO areatrigger_scripts VALUES ('9725', 'at_SpawnSlagna');