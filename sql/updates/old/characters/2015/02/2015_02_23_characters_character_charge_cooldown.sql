TRUNCATE TABLE `character_charges_cooldown`;
ALTER TABLE `character_charges_cooldown` CHANGE `spell_id` `category` INT(10) UNSIGNED NOT NULL; 