DELETE FROM spell_proc_event WHERE entry IN (166020, 166021);
INSERT INTO spell_proc_event (entry, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, SpellFamilyMask3) VALUES
(166020, 15, 0x02000000, 0, 0, 0);

INSERT INTO spell_proc_event (entry) VALUE 
(166021);
