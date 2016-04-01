DELETE csp FROM character_spell csp
JOIN characters c
  ON c.guid = csp.guid
WHERE csp.spell = 121093
AND c.race != 11;

DELETE csk FROM character_skills csk
JOIN characters c
  ON c.guid = csk.guid
WHERE csk.skill = 760
  AND c.race != 11;
