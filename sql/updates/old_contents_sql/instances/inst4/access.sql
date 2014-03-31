replace into areatrigger_teleport values
(6612, 'The Vortex Pinnacle - Entrance Target', 657, -337.627, 15.3073, 626.979, 3.89208),
(6354, 'Twilight Bastion - Entrance Target', 671, -561.351, -531.174, 890.593, 0);

update access_requirement set level_min=85 WHERE mapId IN (657, 671);
