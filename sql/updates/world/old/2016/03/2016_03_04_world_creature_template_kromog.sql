UPDATE creature_template SET unit_flags = unit_flags & ~0x200000, dmg_multiplier = 15 WHERE entry = 77692;
UPDATE creature SET unit_flags = unit_flags & ~0x200000 WHERE id = 77692;