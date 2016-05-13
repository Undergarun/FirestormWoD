DELETE FROM `command` WHERE `name` = 'reload motd';
INSERT INTO `command` VALUES ('reload motd', 7, 'Syntax: .reload motd\r\n\r\nReload the server motd');
