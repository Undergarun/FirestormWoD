-- gunship transport flags
UPDATE gameobject_template SET flags = 40 WHERE entry IN (195121, 195276);

-- vehicles spell
UPDATE creature_template SET spell1 = 67440, spell2 = 67441, VehicleId = 509 WHERE entry = 34775;
UPDATE creature_template SET spell1 = 67796, spell2 = 67797, VehicleId = 435 WHERE entry = 34776;
UPDATE creature_template SET spell1 = 66456, spell2 = 67195 WHERE entry = 34802;
UPDATE creature_template SET spell1 = 67796, spell2 = 67797, VehicleId = 514 WHERE entry = 35069;
UPDATE creature_template SET spell1 = 67034, spell2 = 67195 WHERE entry = 35273;