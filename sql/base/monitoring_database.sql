#
# SQL Export
# Created by Querious (962)
# Created: March 31, 2015 at 3:09:02 PM CST
# Encoding: Unicode (UTF-8)
#


DROP TABLE IF EXISTS `stats`;
DROP TABLE IF EXISTS `last_update`;
DROP TABLE IF EXISTS `character_db`;


CREATE TABLE `character_db` (
  `host` varchar(255) DEFAULT NULL,
  `user` varchar(255) DEFAULT NULL,
  `pass` varchar(255) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


CREATE TABLE `last_update` (
  `timestamp` int(11) unsigned DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


CREATE TABLE `stats` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `TimeStamp` int(11) unsigned DEFAULT NULL,
  `OnlinePlayers` int(11) unsigned DEFAULT NULL,
  `OnlineGameMasters` int(11) unsigned DEFAULT NULL,
  `Uptime` int(11) unsigned DEFAULT NULL,
  `UpdateDiff` int(11) unsigned DEFAULT NULL,
  `Upload` int(11) unsigned DEFAULT NULL,
  `Download` int(11) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=119147 DEFAULT CHARSET=latin1;




