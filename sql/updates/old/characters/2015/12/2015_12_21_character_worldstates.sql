ALTER TABLE `character_worldstates`
   CHANGE `guid` `guid` INT(10) UNSIGNED NOT NULL,
   CHANGE `worldstate` `worldstate` INT(10) UNSIGNED NOT NULL,
   ADD PRIMARY KEY(`guid`, `worldstate`);