ALTER IGNORE TABLE `areatrigger_template`  ADD UNIQUE INDEX `remove_dup_temp` (`spell_id`, `eff_index`, `entry`);
ALTER TABLE `areatrigger_template` DROP INDEX `remove_dup_temp`;
-- ALTER TABLE `areatrigger_template` DROP PRIMARY KEY;
ALTER TABLE `areatrigger_template`  ADD PRIMARY KEY (`spell_id`, `eff_index`, `entry`);
