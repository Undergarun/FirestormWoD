/*
SQLyog Ultimate v9.02 
MySQL - 5.6.19-log : Database - auth
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `webshop_delivery_interrealm_transfer` */

DROP TABLE IF EXISTS `webshop_delivery_interrealm_transfer`;

CREATE TABLE `webshop_delivery_interrealm_transfer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `account` int(11) unsigned NOT NULL DEFAULT '0',
  `guid` int(11) unsigned NOT NULL DEFAULT '0',
  `startrealm` int(11) unsigned NOT NULL DEFAULT '0',
  `destrealm` int(11) unsigned NOT NULL DEFAULT '0',
  `revision` blob,
  `dump` longtext,
  `last_error` blob,
  `nb_attempt` int(11) unsigned NOT NULL DEFAULT '0',
  `state` int(10) unsigned NOT NULL DEFAULT '0',
  `error` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `account` (`account`),
  KEY `guid` (`guid`),
  KEY `startrealm` (`startrealm`),
  KEY `destrealm` (`destrealm`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
