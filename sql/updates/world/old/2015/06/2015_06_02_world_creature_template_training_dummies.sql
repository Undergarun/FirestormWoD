# Training Dummy <Healing>
UPDATE creature_template SET
minlevel = 100,
maxlevel = 100,
EXP = 5,
unit_flags = unit_flags & ~0x1000,
type_flags = type_flags | 0x1000,
ScriptName = 'npc_training_dummy_healing'
WHERE entry IN (88835, 89321, 88289, 87321, 88316);

UPDATE creature SET unit_flags = unit_flags & ~0x1000, unit_flags2 = unit_flags2 & ~0x800, curhealth = 1 WHERE id IN (88835, 89321, 88289, 87321, 88316);

# Training Dummy <Damage>
UPDATE creature_template SET faction = 7, ScriptName = 'npc_training_dummy_damage' WHERE entry IN (87760, 87317, 87320, 87761, 87318, 87762);
UPDATE creature_template SET minlevel = 100, maxlevel = 100 WHERE entry = 87760;
UPDATE creature_template SET unit_flags = unit_flags & ~0x20000, unit_flags2 = unit_flags2 & ~0x800, mechanic_immune_mask = 0, flags_extra = 0 WHERE entry = 87761;
UPDATE creature_template SET unit_flags = unit_flags & ~0x4 WHERE entry = 87762;
UPDATE creature_template SET type_flags = 0x4 WHERE entry IN (87320, 87762);

UPDATE creature SET unit_flags2 = unit_flags2 & ~0x800 WHERE id IN (87760, 87317, 87320, 87761, 87318, 87762);

DELETE FROM creature_template_addon WHERE entry IN (87760, 87317, 87320, 87761, 87318, 87762);

# Training Dummy <Tanking>
UPDATE creature_template SET
unit_flags = unit_flags & ~0x4,
unit_flags = unit_flags & ~0x20000,
unit_flags2 = unit_flags2 & ~0x800,
ScriptName = 'npc_training_dummy_tanking',
mechanic_immune_mask = 0,
flags_extra = 0
WHERE entry IN (87329, 88288, 88836, 87322, 88837, 88314);
UPDATE creature_template SET dmg_multiplier = 10 WHERE entry IN (88288, 88836, 87322, 88314);
UPDATE creature_template SET dmg_multiplier = 15, type_flags = 0x4 WHERE entry IN (87329, 88837);

UPDATE creature SET unit_flags2 = unit_flags2 & ~0x800 WHERE id IN (87329, 88288, 88836, 87322, 88837, 88314);

DELETE FROM creature_template_addon WHERE entry IN (87329, 88288, 88836, 87322, 88837, 88314);