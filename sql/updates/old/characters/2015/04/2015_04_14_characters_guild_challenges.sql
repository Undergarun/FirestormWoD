/*
SQLyog Ultimate v9.02 
MySQL - 5.6.12-log : Database - 602_characters
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*Table structure for table `guild_challenges` */

DROP TABLE IF EXISTS `guild_challenges`;

CREATE TABLE `guild_challenges` (
  `GuildId` int(10) unsigned NOT NULL,
  `ChallengeType` int(1) NOT NULL DEFAULT '0',
  `ChallengeCount` int(1) DEFAULT NULL,
  PRIMARY KEY (`GuildId`,`ChallengeType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;