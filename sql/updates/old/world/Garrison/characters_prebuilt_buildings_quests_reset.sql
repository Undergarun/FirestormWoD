-- Replace the guid value with the guid of the char you want to reset
DELETE FROM character_queststatus_rewarded WHERE guid = 0 AND quest IN (
34846,  -- fish fight
34194,  -- alliance fishing quest
36199,  -- alliance fishing quest
36201,  -- alliance fishing quest
36202,  -- alliance fishing quest
34758,  -- horde fishing quest
36141,  -- horde fishing quest
36131,  -- horde fishing quest
36132,  -- horde fishing quest
35154,  -- horde mine quest
34192,  -- alliance mine quest
36404,  -- alliance garden quest
34193); -- horde garden quest

-- Unstuck players for fishing hut
UPDATE 
  character_garrison_building,
  character_garrison,
  character_queststatus_rewarded 
SET
  character_garrison_building.`active` = 1 
WHERE character_garrison.`character_guid` = character_queststatus_rewarded.`guid` 
  AND character_garrison.`id` = character_garrison_building.`garrison_id` 
  AND character_garrison_building.`building_id` = 64 
  AND character_garrison_building.`active` = 0 
  AND character_queststatus_rewarded.`quest` = 34846 ;

