/*
SQLyog Ultimate v9.02 
MySQL - 5.6.19-log : Database - wod_char
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `encounter_damage_log` */

CREATE TABLE IF NOT EXISTS `encounter_damage_log` (
  `encounterId` int(11) unsigned NOT NULL,
  `encounterStartTime` bigint(20) unsigned NOT NULL,
  `logTime` bigint(20) unsigned NOT NULL,
  `attackerGuid` int(11) unsigned NOT NULL,
  `damage` int(11) unsigned NOT NULL,
  `spellId` int(11) unsigned NOT NULL,
  KEY `encounterId` (`encounterId`,`encounterStartTime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `encounter_group_dump` */

CREATE TABLE IF NOT EXISTS `encounter_group_dump` (
  `encounterId` int(11) unsigned NOT NULL,
  `encounterStartTime` bigint(20) unsigned NOT NULL,
  `dumpTime` bigint(20) unsigned NOT NULL,
  `dump` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
