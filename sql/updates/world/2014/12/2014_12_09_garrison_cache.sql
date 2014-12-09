ALTER TABLE `character_garrison`     ADD COLUMN `cache_last_usage` INT(10) UNSIGNED DEFAULT '0' NOT NULL AFTER `num_follower_activation_regen_timestamp`;
UPDATE `gameobject_template` SET `ScriptName`='go_garrison_cache' WHERE `entry`='237720';
UPDATE `gameobject_template` SET `ScriptName`='go_garrison_cache' WHERE `entry`='236916';