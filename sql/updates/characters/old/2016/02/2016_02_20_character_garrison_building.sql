ALTER TABLE `character_garrison_building` 
   ADD COLUMN `follower_assigned` INT(10) UNSIGNED DEFAULT '0' NULL AFTER `time_built_end`;