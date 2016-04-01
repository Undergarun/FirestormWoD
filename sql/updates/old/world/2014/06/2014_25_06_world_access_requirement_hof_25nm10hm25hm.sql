-- Opening Heart of Fear in 25NM, 10HM and 25HM
UPDATE access_requirement SET level_min = 90 WHERE mapId = 1009 AND difficulty IN (4, 5, 6);