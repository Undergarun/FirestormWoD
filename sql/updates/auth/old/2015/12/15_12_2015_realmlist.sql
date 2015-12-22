ALTER TABLE `realmlist` ADD COLUMN `motd` VARCHAR(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT 'Welcome on our server.' AFTER `allowedSecurityLevel`;
