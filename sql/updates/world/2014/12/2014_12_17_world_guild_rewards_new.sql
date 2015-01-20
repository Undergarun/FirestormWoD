UPDATE guild_rewards SET achievement = 0 WHERE achievement IN (4943, 4944, 4945, 4946, 4912, 5492);
DELETE FROM guild_rewards WHERE achievement = 6634;
INSERT INTO guild_rewards VALUES
(114968, 6, 0, 3000000, 9651),
(116666, 7, 0, 40000000, 9669),
(120352, 7, 0, 1000000, 9388);