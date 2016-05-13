DELETE FROM spell_script_names WHERE ScriptName IN ('spell_dru_wild_mushroom_growing', 'spell_dru_wild_mushroom_heal');
INSERT INTO spell_script_names VALUES
(102792, 'spell_dru_wild_mushroom_heal'),
(138611, 'spell_dru_wild_mushroom_growing');
DELETE FROM spell_proc_event WHERE entry = 138611;
INSERT INTO spell_proc_event (entry, SpellFamilyName, SpellFamilyMask0, procFlags) VALUE (138611, 7, 0x10, 0x40000);