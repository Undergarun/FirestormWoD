ALTER TABLE spell_misc DROP COLUMN SpellVisual0;
ALTER TABLE spell_misc DROP COLUMN SpellVisual1;
ALTER TABLE item_effect ADD COLUMN Unk1 INT(11) NOT NULL AFTER SpellCategoryCooldown;