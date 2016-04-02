DELETE FROM player_factionchange_items WHERE horde_id IN (111170, 111172);
DELETE FROM player_factionchange_items WHERE alliance_id IN (115697, 115699);
INSERT INTO player_factionchange_items VALUES
(0, 115697, "Primal Gladiator's Scaled Gauntlets", 0, 111170, "Primal Gladiator's Scaled Gauntlets"),
(0, 115699, "Primal Gladiator's Scaled Legguards", 0, 111172, "Primal Gladiator's Scaled Legguards");