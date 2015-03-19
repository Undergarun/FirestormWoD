/*
SQLyog Ultimate - MySQL GUI v8.2 
MySQL - 5.6.17 : Database - wod_monitoring
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `character_db` */

DROP TABLE IF EXISTS `character_db`;

CREATE TABLE `character_db` (
  `host` varchar(255) DEFAULT NULL,
  `user` varchar(255) DEFAULT NULL,
  `pass` varchar(255) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `character_db` */

insert  into `character_db`(`host`,`user`,`pass`,`name`) values ('127.0.0.1','root','','chars');

/*Table structure for table `last_update` */

DROP TABLE IF EXISTS `last_update`;

CREATE TABLE `last_update` (
  `timestamp` int(11) unsigned DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `last_update` */

insert  into `last_update`(`timestamp`) values (0);

/*Table structure for table `stats` */

DROP TABLE IF EXISTS `stats`;

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
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=latin1;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
