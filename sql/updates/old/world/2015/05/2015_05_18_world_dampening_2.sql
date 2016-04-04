DELETE FROM spell_script_names WHERE ScriptName IN ("spell_gen_dampening");
INSERT INTO spell_script_names VALUES
(110310, "spell_gen_dampening");


DELETE FROM spell_area WHERE spell = 110310;
INSERT INTO spell_area (spell, AREA, autocast) VALUES
(110310, 6732, 0), -- The Tiger's Peak
(110310, 6296, 0), -- Tol'viron Arena
(110310, 3702, 0), -- Blade's Edge Arena
(110310, 4378, 0), -- Dalaran Arena
(110310, 3968, 0), -- Ruins of Lordearon
(110310, 3698, 0), -- Nagrand Arena
(110310, 4406, 0); -- The Ring of Valor
