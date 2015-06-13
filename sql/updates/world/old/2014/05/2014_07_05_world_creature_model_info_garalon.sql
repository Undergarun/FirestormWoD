-- Bouncing radius for Garalon
UPDATE creature_model_info SET bounding_radius = 17 WHERE modelid = (SELECT modelid1 FROM creature_template WHERE entry = 62164);