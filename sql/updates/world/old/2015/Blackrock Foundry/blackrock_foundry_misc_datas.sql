DELETE FROM instance_template WHERE map = 1205;
INSERT INTO instance_template VALUE (1205, 1116, 'instance_blackrock_foundry', 0);

DELETE FROM access_requirement WHERE mapId = 1205;
INSERT INTO access_requirement (mapId, difficulty, level_min, level_max, itemlevel_min, comment) VALUES
(1205, 14, 100, 100, 635, 'Blackrock Foundry - Normal'),
(1205, 15, 100, 100, 635, 'Blackrock Foundry - Heroic'),
(1205, 16, 101, 101, 635, 'Blackrock Foundry - Mythic'),
(1205, 17, 101, 101, 635, 'Blackrock Foundry - RaidTools');

DELETE FROM areatrigger_teleport WHERE id IN (10241, 10242);
INSERT INTO areatrigger_teleport VALUE
(10241, 'Blackrock Foundry - Entrance', 1205, 131.171875, 3429.481, 319.8291, 0.00135),
(10242, 'Blackrock Foundry - Exit Target', 1116, 8074.5845, 859.86652, 34.3671, 2.8979);