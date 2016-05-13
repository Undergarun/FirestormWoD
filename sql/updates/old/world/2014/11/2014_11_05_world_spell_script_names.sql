DELETE FROM spell_script_names WHERE ScriptName IN
(
"spell_sha_enhanced_chain_lightning",
"spell_sha_feral_spirit",
"spell_sha_fulmination_proc",
"spell_sha_lava_surge"
);

INSERT INTO spell_script_names VALUES
(77762, "spell_sha_lava_surge"),
(157765, "spell_sha_enhanced_chain_lightning"),
(88766, "spell_sha_fulmination_proc"),
(51533, "spell_sha_feral_spirit");


REPLACE INTO spell_proc_event (entry, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, SpellFamilyMask3, procFlags, CustomChance) VALUES
(166096, 11, 0, 0, 0, 0x00000040, 0, 0),
(77756, 11, 0x10000000, 0, 0, 0, 0, 0),
(157765, 11,0x00000002, 0x00000004, 0, 0, 0x00010000, 100),
(88766, 11, 0x00000003, 0, 0, 0, 0x00010000, 0);

REPLACE INTO spell_linked_spell VALUES
(-114050, -166100, 0, "Ascendance Sham WoD PvP 2 removal");

-- Item - Shaman WoD PvP Enhancement 2P Bonus
-- Item - Shaman WoD PvP Enhancement 4P Bonus
-- Fulmination
-- Improved Lightning Shields - 5 stacks increase
-- Magma totem (forgot to mention before)
-- Lighning Shield
-- Aura 416 - Decrease CD by haste (Bear Form, Flurry, Sanctify Of Battle, Fighting Style, Headlong Rush)
-- Earthquake damage 
-- Enhanced Chain Lightning
-- Lava Surge
-- Item - Shaman WoD PvP Elemental 2P Bonus
