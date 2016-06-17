REPLACE INTO character_queststatus 
  SELECT 
    character_queststatus_rewarded.guid,
    "33868",
    "1",
    "0",
    "1466150107" 
  FROM
    character_queststatus_rewarded 
    LEFT JOIN characters 
      ON character_queststatus_rewarded.guid = characters.guid 
  WHERE (
      characters.race IN (2, 5, 6, 8, 9, 10, 26) 
      AND character_queststatus_rewarded.quest = 34446
    ) 
    AND 
    (SELECT 
      COUNT(*) 
    FROM
      character_queststatus_rewarded 
    WHERE quest = 33868
      AND guid = characters.`guid`) = 0 
    AND 
    (SELECT 
      COUNT(*) 
    FROM
      character_queststatus 
    WHERE quest = 33868
      AND STATUS != 1 
      AND guid = characters.`guid`) = 0 ; -- horde
  
  
  REPLACE INTO character_queststatus 
  SELECT 
    character_queststatus_rewarded.guid,
    "34575",
    "1",
    "0",
    "1466150107" 
  FROM
    character_queststatus_rewarded 
    LEFT JOIN characters 
      ON character_queststatus_rewarded.guid = characters.guid 
  WHERE (
      characters.race IN (1, 3, 4, 7, 11, 22, 25) 
      AND character_queststatus_rewarded.quest = 35884
    ) 
    AND 
    (SELECT 
      COUNT(*) 
    FROM
      character_queststatus_rewarded 
    WHERE quest = 34575
      AND guid = characters.`guid`) = 0 
    AND 
    (SELECT 
      COUNT(*) 
    FROM
      character_queststatus 
    WHERE quest = 34575
      AND STATUS != 1 
      AND guid = characters.`guid`) = 0 ; -- alliance
  