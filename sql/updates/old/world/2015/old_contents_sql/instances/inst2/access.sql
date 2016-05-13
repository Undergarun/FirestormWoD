replace into areatrigger_teleport values
(6179, 'Grim Batol Dungeon - Entrance Target', 670, -624.155, -189.365, 272.04, 4.954),
(6196, 'Stonecore - Entrance Target', 725, 851.052, 986.474, 317.266, 0);

update access_requirement set level_min=85 WHERE mapId IN (725, 670);
