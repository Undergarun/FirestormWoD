DELETE FROM spell_script_names WHERE ScriptName = 'spell_pri_divine_aegis';
INSERT INTO spell_script_names VALUES (47515, 'spell_pri_divine_aegis');
DELETE FROM spell_proc_event WHERE entry = '47515';
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `SpellFamilyMask3`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES('47515','0','0','0','0','0','0','2376704','0','0','0','0');
