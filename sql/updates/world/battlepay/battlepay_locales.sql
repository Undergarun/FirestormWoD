/*
SQLyog Ultimate v9.02 
MySQL - 5.6.19-log : Database - ashran_world
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `locales_battlepay_display_info` */

DROP TABLE IF EXISTS `locales_battlepay_display_info`;

CREATE TABLE `locales_battlepay_display_info` (
  `DisplayInfoId` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Name_loc1` text,
  `Name_loc2` text,
  `Name_loc3` text,
  `Name_loc4` text,
  `Name_loc5` text,
  `Name_loc6` text,
  `Name_loc7` text,
  `Name_loc8` text,
  `Name_loc9` text,
  `Name_loc10` text,
  `Description_loc1` text,
  `Description_loc2` text,
  `Description_loc3` text,
  `Description_loc4` text,
  `Description_loc5` text,
  `Description_loc6` text,
  `Description_loc7` text,
  `Description_loc8` text,
  `Description_loc9` text,
  `Description_loc10` text,
  PRIMARY KEY (`DisplayInfoId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
