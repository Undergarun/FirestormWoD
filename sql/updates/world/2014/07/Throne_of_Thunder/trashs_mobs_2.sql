UPDATE creature_template SET
minlevel = 92,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_mysterious_mushroom'
WHERE entry = 70545;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 6.4,
ScriptName = 'mob_shale_stalker'
WHERE entry = 70587;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 28,
ScriptName = 'mob_fungal_growth'
WHERE entry = 70153;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 28,
ScriptName = 'mob_mist_lurker'
WHERE entry = 70594;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 4,
EXP = 4,
Health_mod = 28,
ScriptName = 'mob_cavern_burrower'
WHERE entry = 70589;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 4,
EXP = 4,
Health_mod = 28,
ScriptName = 'mob_eternal_guardian'
WHERE entry = 70586;

UPDATE creature_model_info SET
bounding_radius = 0.9,
combat_reach = 4.5
WHERE modelid = 45735;

UPDATE gameobject_template SET ScriptName = 'go_ancient_mogu_bell' WHERE entry = 218723;

DELETE FROM conditions WHERE SourceEntry IN (140600);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 2, 140600, 31, 0, 3, 70589);

DELETE FROM spell_script_names WHERE spell_id IN (140600, 140630);
INSERT INTO spell_script_names VALUES
(140600, 'spell_sonic_call'),
(140630, 'spell_siphon_life');