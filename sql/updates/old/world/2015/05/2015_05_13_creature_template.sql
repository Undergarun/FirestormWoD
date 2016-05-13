-- Greater Earth Elemental - 15352
UPDATE creature_template SET spell1 = 36213, spell2 = 0, spell3 = 0, spell4 = 0, spell5 = 0, spell6 = 0, ScriptName = 'npc_earth_elemental' WHERE entry = 15352;
-- Primal Earth Elemental - 61056
UPDATE creature_template SET spell1 = 36213, spell2 = 118337, spell3 = 118345, spell4 = 118347, spell5 = 0, spell6 = 0, ScriptName = 'npc_earth_elemental' WHERE entry = 61056;

-- Greater Fire Elemental - 15438
UPDATE creature_template SET spell1 = 57984, spell2 = 0, spell3 = 0, spell4 = 0, spell5 = 0, spell6 = 0, ScriptName = 'npc_fire_elemental' WHERE entry = 15438;
-- Primal Fire Elemental - 61029
UPDATE creature_template SET spell1 = 118350, spell2 = 57984, spell3 = 117588, spell4 = 118297, spell5 = 0, spell6 = 0, ScriptName = 'npc_fire_elemental' WHERE entry = 61029;

-- Greater Storm Elemental - 77936
UPDATE creature_template SET spell1 = 0, spell2 = 0, spell3 = 0, spell4 = 0, spell5 = 0, spell6 = 0, ScriptName = 'npc_storm_elemental' WHERE entry = 77936;
-- Primal Storm Elemental - 77942
UPDATE creature_template SET spell1 = 157375, spell2 = 157382, spell3 = 0, spell4 = 0, spell5 = 0, spell6 = 0, ScriptName = 'npc_storm_elemental' WHERE entry = 77942;


DELETE FROM `pet_stats` WHERE `entry` IN (15352, 77936, 15438, 61029, 61056, 77942);
INSERT INTO `pet_stats` VALUES
(15352,1.14,0,1,1,1,0.6,2,0,0,1,'Greater Earth Elemental (Missing data)'),
(15438,1.14,0,1,1,0.75,0.6,2,0,1,1,'Greater Fire Elemental (Missing data)'),
(77936,1.14,0,1,1,0.75,0.6,2,0,0,1,'Greater Storm Elemental (Missing data)'),

(61029,1.14,0,1.8,1.8,0.9,1.08,2,0,1,1,'Primal Fire Elemental (Missing data)'),
(61056,1.14,0,1.8,1.8,1.5,1.08,2,0,1,1,'Primal Earth Elemental (Missing data)'),
(77942,1.14,0,1.8,1.8,0.9,1.08,2,0,0,1,'Primal Storm Elemental (Missing data)');
