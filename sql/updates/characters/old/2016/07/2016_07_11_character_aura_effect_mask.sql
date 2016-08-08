ALTER TABLE `character_aura`   
  CHANGE `effect_mask` `effect_mask` INT(11) UNSIGNED DEFAULT 0  NOT NULL;
ALTER TABLE `character_aura`   
  CHANGE `recalculate_mask` `recalculate_mask` INT(11) UNSIGNED DEFAULT 0  NOT NULL;