replace into areatrigger_teleport values
(6201, 'Throne of Tides - Entrance Target', 643, -601.151, 809.526, 244.809, 0),
(6200, 'Abyssal Maw - Teleport to Throne of the Tides Entrance', 0, -5621.06, 5468.73, -1801, 1),
(7154, 'Hour of Twilight - Entrance Target', 940, 4927.03, 315.03, 100.157, 4.71239);

update access_requirement set level_min=85 WHERE mapId IN (940, 643);
