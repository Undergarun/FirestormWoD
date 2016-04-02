ALTER TABLE `quest_template`     CHANGE `WDBVerified` `BuildVerified` SMALLINT(6) DEFAULT '1' NOT NULL;
ALTER TABLE `quest_template_objective`     ADD COLUMN `BuildVerified` SMALLINT(6) DEFAULT '1' NOT NULL AFTER `VisualEffects`;
