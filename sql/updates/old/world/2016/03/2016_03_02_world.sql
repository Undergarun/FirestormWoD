ALTER TABLE `disenchant_loot_template` CHANGE `maxcount` `maxcount` INT(11)  UNSIGNED  NOT NULL  DEFAULT '1';

ALTER TABLE `research_loot` CHANGE `race` `race` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';

ALTER TABLE `spell_phase` CHANGE `terrainswapmap` `terrainswapmap` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `spell_phase` CHANGE `phasemask` `phasemask` INT(11)  UNSIGNED  NOT NULL  DEFAULT '1';

ALTER TABLE `phase_definitions` CHANGE `phasemask` `phasemask` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `phase_definitions` CHANGE `zoneId` `zoneId` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `phase_definitions` CHANGE `entry` `entry` INT(11)  UNSIGNED  NOT NULL  AUTO_INCREMENT;

ALTER TABLE `phase_definitions` CHANGE `phaseId` `phaseId` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `phase_definitions` CHANGE `terrainswapmap` `terrainswapmap` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';

ALTER TABLE `quest_template` CHANGE `RewardCurrencyId1` `RewardCurrencyId1` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardCurrencyId2` `RewardCurrencyId2` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardCurrencyId3` `RewardCurrencyId3` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RewardCurrencyId4` `RewardCurrencyId4` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';

ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId1` `RequiredSourceItemId1` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId2` `RequiredSourceItemId2` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId3` `RequiredSourceItemId3` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemId4` `RequiredSourceItemId4` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';

ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount1` `RequiredSourceItemCount1` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount2` `RequiredSourceItemCount2` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount3` `RequiredSourceItemCount3` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
ALTER TABLE `quest_template` CHANGE `RequiredSourceItemCount4` `RequiredSourceItemCount4` INT(11)  UNSIGNED  NOT NULL  DEFAULT '0';
