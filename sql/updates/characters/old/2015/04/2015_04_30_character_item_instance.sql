-- Need to be null by default
ALTER TABLE `item_instance` CHANGE `bonuses` `bonuses` TEXT CHARACTER SET utf8 COLLATE utf8_general_ci NULL;