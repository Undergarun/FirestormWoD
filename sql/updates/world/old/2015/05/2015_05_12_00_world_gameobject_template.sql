-- change WDB verified field
ALTER TABLE `gameobject_template` CHANGE `WDBVerified` `BuildVerified` SMALLINT(6) DEFAULT 1 NULL; 
ALTER TABLE `npc_text` CHANGE `WDBVerified` `BuildVerified` SMALLINT(6) DEFAULT 1 NULL;
-- ALTER TABLE `creature_template` CHANGE `WDBVerified` `BuildVerified` SMALLINT(6) DEFAULT 1 NULL;