/*
SQLyog Ultimate v9.02 
MySQL - 5.5.35 : Database - 540_world
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

/*Table structure for table `vehicleseat_dbc` */
DROP TABLE IF EXISTS `vehicleseat_dbc`;

CREATE TABLE `vehicleseat_dbc` (
  `ID` int(11) unsigned NOT NULL,
  `flags` int(11) unsigned NOT NULL,
  `attachmentID` int(11) NOT NULL,
  `attachmentOffsetX` float NOT NULL,
  `attachmentOffsetY` float NOT NULL,
  `attachmentOffsetZ` float NOT NULL,
  `flagsB` int(11) unsigned DEFAULT NULL,
  `comment` char(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `vehicleseat_dbc` */

INSERT  INTO `vehicleseat_dbc`(`ID`,`flags`,`attachmentID`,`attachmentOffsetX`,`attachmentOffsetY`,`attachmentOffsetZ`,`flagsB`,`comment`) VALUES
(20000, 8192, 0, 16.5, -12.5, 0, 0, NULL),
(20001, 8192, 0, 16.5, 12.5, 0, 0, NULL),
(20002, 8192, 0, -22.5, -15, 0, 0, NULL),
(20003, 8192, 0, -22.5, 15, 0, 0, NULL);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
