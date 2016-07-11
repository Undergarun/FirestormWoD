-- Description : 
-- This scripts add, depending on faction, the first draenor quest if last tanaan jungle intro quest has been done

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
 
 
 
-- DESCRIPTION : 
-- THIS SCRIPTS PREVENT WRONG FACTION QUESTS DISTRIBUTION ABOUT FIRST QUESTS OF DRAENOR

-- HORDE

-- add horde quest for horde characters which have alliance quest
 REPLACE INTO character_queststatus 
SELECT 
  character_queststatus.guid,
  33868,
  1,
  1,
  1467104177 
FROM
  character_queststatus 
  LEFT JOIN characters 
    ON character_queststatus.guid = characters.guid 
WHERE (
    characters.race IN (2, 5, 6, 8, 9, 10, 26) 
    AND character_queststatus.quest = 34575
  ) ;

 -- remove alliance quest from horde characters
 
DELETE 
FROM
  character_queststatus 
  LEFT JOIN characters 
    ON character_queststatus.guid = characters.guid 
WHERE (
    characters.race IN (2, 5, 6, 8, 9, 10, 26) 
    AND character_queststatus.quest = 34575
  ) ;


-- ALLIANCE

-- add alliance quest for alliance characters which have horde quest
 REPLACE INTO character_queststatus 
SELECT 
  character_queststatus.guid,
  34575,
  1,
  1,
  1467104177 
FROM
  character_queststatus 
  LEFT JOIN characters 
    ON character_queststatus.guid = characters.guid 
WHERE (
    characters.race IN (1, 3, 4, 7, 11, 22, 25) 
    AND character_queststatus.quest = 33868
  ) ;

 -- remove horde quest from alliance characters
 
DELETE 
FROM
  character_queststatus 
  LEFT JOIN characters 
    ON character_queststatus.guid = characters.guid 
WHERE (
    characters.race IN (1, 3, 4, 7, 11, 22, 25) 
    AND character_queststatus.quest = 33868
  ) ;
