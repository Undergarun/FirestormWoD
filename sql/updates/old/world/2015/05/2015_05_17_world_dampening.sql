DELETE FROM spell_script_names WHERE ScriptName IN ("spell_gen_dampening");
INSERT INTO spell_script_names VALUES
(110310, "spell_gen_dampening");


DELETE FROM spell_area WHERE spell = 110310;
INSERT INTO spell_area (spell, AREA, autocast) VALUES
(110310, 6732, 1), -- The Tiger's Peak
(110310, 6296, 1), -- Tol'viron Arena
(110310, 3702, 1), -- Blade's Edge Arena
(110310, 4378, 1), -- Dalaran Arena
(110310, 3968, 1), -- Ruins of Lordearon
(110310, 3698, 1), -- Nagrand Arena
(110310, 4406, 1); -- The Ring of Valor
