/*
SQLyog Ultimate v11.11 (64 bit)
MySQL - 5.6.17 : Database - 623_world
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

/*Table structure for table `battlepet_npc_team_member` */

DROP TABLE IF EXISTS `battlepet_npc_team_member`;

CREATE TABLE `battlepet_npc_team_member` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `NpcID` int(10) unsigned NOT NULL,
  `Specie` int(10) unsigned NOT NULL,
  `Level` int(10) unsigned NOT NULL,
  `Ability1` int(10) NOT NULL,
  `Ability2` int(10) NOT NULL,
  `Ability3` int(10) NOT NULL,
  `Comment` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`,`NpcID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
