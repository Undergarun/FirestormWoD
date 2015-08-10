-- Add automatic faction switch for paladin set
DELETE FROM player_factionchange_items WHERE alliance_id IN (115237, 115234, 115236, 115235, 115106);
INSERT INTO player_factionchange_items (alliance_id, commentA, horde_id, commentH) VALUES
(115237, "Primal Aspirant's Scaled Shoulders", 120014, "Primal Aspirant's Scaled Shoulders"),
(115234, "Primal Aspirant's Scaled Gauntlets", 120011, "Primal Aspirant's Scaled Gauntlets"),
(115236, "Primal Aspirant's Scaled Legguards", 120013, "Primal Aspirant's Scaled Legguards"),
(115235, "Primal Aspirant's Scaled Helm",      120012, "Primal Aspirant's Scaled Helm"     ),
(115106, "Primal Aspirant's Scaled Chestpiece", 119889, "Primal Aspirant's Scaled Chestpiece");