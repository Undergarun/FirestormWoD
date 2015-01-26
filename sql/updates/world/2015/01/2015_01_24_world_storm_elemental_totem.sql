UPDATE creature_template SET spell1 = 117663 WHERE entry = 15439;
UPDATE creature_template SET spell1 = 33663 WHERE entry = 15430;
UPDATE creature_template SET spell1 = 157299 WHERE entry = 77934;
UPDATE creature_template SET modelid1 = 1204, ScriptName = 'npc_fire_elemental' WHERE entry = 15438;
UPDATE creature_template SET ScriptName = 'npc_earth_elemental' WHERE entry = 15352;
UPDATE creature_template SET ScriptName = 'npc_storm_elemental' WHERE entry = 77936;
REPLACE INTO creature_model_info (modelid, bounding_radius, combat_reach, gender, modelid_other_gender) VALUES
(4588, 1, 1, 2, 0),
(4589, 1, 0, 2, 0),
(19073, 1, 1, 2, 0),
(19074, 1, 1, 2, 0),
(30757, 1, 1, 2, 0),
(30758, 1, 1, 2, 0),
(30761, 1, 1, 2, 0),
(30762, 1, 1, 2, 0),
(1204, 1.5, 1.5, 2, 0),
(14511, 0.465, 2.25, 2, 0);