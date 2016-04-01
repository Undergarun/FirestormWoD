UPDATE creature_template SET
modelid1 = 48113,
modelid2 = 0,
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
EXP = 4,
Health_mod = 483,
InhabitType = 4,
ScriptName = 'boss_megaera'
WHERE entry = 68065;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 4,
EXP = 4,
Health_mod = 69,
InhabitType = 4,
ScriptName = 'boss_megaera_head'
WHERE entry IN (70247, 70212, 70235, 70248);

UPDATE creature_model_info SET
bounding_radius = 1.573613,
combat_reach = 25.75
WHERE modelid IN (47414, 48240, 48241, 48242, 47415, 48243, 48245, 48244, 47416, 48238, 47417, 48239);

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 69,
InhabitType = 4,
ScriptName = 'boss_megaera_back_head'
WHERE entry IN (70229, 70250, 70251, 70252);

UPDATE creature_template SET
modelid1 = 11686,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
InhabitType = 4
WHERE entry = 70175;

UPDATE creature_template SET
modelid1 = 11686,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
InhabitType = 4
WHERE entry = 68136;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
modelid1 = 11686,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_cinders'
WHERE entry = 70432;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
modelid1 = 11686,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_acid_rain'
WHERE entry = 70435;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
modelid1 = 11686,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_icy_ground'
WHERE entry = 70446;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
EXP = 4,
Health_mod = 3,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_nether_wyrm'
WHERE entry = 70507;

DELETE FROM conditions WHERE SourceEntry IN (139848);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 139848, 31, 0, 3, 70435);

DELETE FROM spell_script_names WHERE spell_id IN (139822, 139843, 139850, 139993, 140138, 139458);
INSERT INTO spell_script_names VALUES
(139822, 'spell_cinders'),
(139843, 'spell_arctic_freeze'),
(139850, 'spell_acid_rain_damage'),
(139993, 'spell_diffusion'),
(140138, 'spell_nether_tear'),
(139458, 'spell_rampage_periodic');

DELETE FROM gameobject_template WHERE entry IN (218805, 218806, 218807, 218808);
INSERT INTO gameobject_template VALUES
-- 10 Normal
(218805, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 218805, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- 25 Normal
(218806, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 218806, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- 10 Heroic
(218807, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 218807, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- 25 Heroic
(218808, 3, 10317, 'Cache of Ancient Treasures', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 218808, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595);