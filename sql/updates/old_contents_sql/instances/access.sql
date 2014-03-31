replace into areatrigger_teleport values
(6620, 'Lost City of Tolvir - Entrance Target', 755, -10700.4, -1312.69, 17.6029, 3.45575),
(6485, 'Halls of Origination - Entrance Target', 644, -954.106, 462.408, 51.9705, 1.5708),
(7151, 'Well of Eternity - Entrance Target', 939, 3238.55, -4998.39, 194.093, 2.35619),
(7152, 'End Time - Entrance Target', 938, 3726.54, -400.181, 110.834, 1.85005);

update access_requirement set level_min = 85 WHERE mapId IN (938, 644, 755, 939);
