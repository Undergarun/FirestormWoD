UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
unit_class = 4,
dmg_multiplier = 4,
baseattacktime = 2000,
unit_class = 4,
EXP = 4,
Health_mod = 330.17,
VehicleId = 2653,
mechanic_immune_mask = 617299839,
ScriptName = 'boss_tortos'
WHERE entry = 67977;

UPDATE creature_model_info SET
bounding_radius = 20,
combat_reach = 30
WHERE modelid = 46559;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
modelid1 = 11686,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_rockfall'
WHERE entry = 68219;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
EXP = 4,
Health_mod = 7.22,
ScriptName = 'mob_whirl_turtle'
WHERE entry = 67966;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 3.3,
InhabitType = 4,
ScriptName = 'mob_vampiric_cave_bat_summon'
WHERE entry = 68497;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
EXP = 4,
Health_mod = 80,
ScriptName = 'mob_humming_crystal'
WHERE entry = 69639;

UPDATE creature SET phaseMask = 1, spawnMask = 96 WHERE id = 69639;

DELETE FROM spell_script_names WHERE spell_id IN (134920, 134364, 140431, 134539, 134476, 136294, 140054, 134031, 134091, 136431, 137633, 137648);
INSERT INTO spell_script_names VALUES
(134920, 'spell_quake_stomp'),
(134364, 'spell_rockfall'),
(140431, 'spell_rockfall'),
(134539, 'spell_rockfall_damage'),
(134476, 'spell_rockfall_large_damage'),
(136294, 'spell_call_of_tortos'),
(140054, 'spell_shell_block'),
(134031, 'spell_kick_shell'),
(134091, 'spell_shell_concussion'),
(136431, 'spell_shell_concussion'),
(137633, 'spell_crystal_shell_damage_absorption'),
(137648, 'spell_crystal_shell_heal_absorption');

DELETE FROM conditions WHERE SourceEntry IN (134091, 136431);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 2, 134091, 31, 0, 3, 68497),
(13, 2, 134091, 31, 0, 3, 67977),
(13, 2, 136431, 31, 0, 3, 68497),
(13, 2, 136431, 31, 0, 3, 67977);