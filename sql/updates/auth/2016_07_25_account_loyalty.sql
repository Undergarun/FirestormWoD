/*
SQLyog Ultimate v9.02 
MySQL - 5.5.46-37.5-log : Database - auth
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `account_loyalty` */

DROP TABLE IF EXISTS `account_loyalty`;

CREATE TABLE `account_loyalty` (
  `AccountID` int(11) unsigned NOT NULL,
  `LastClaim` int(11) NOT NULL DEFAULT '0',
  `LastEventReset` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`AccountID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `account_loyalty_event` */

DROP TABLE IF EXISTS `account_loyalty_event`;

CREATE TABLE `account_loyalty_event` (
  `AccountID` int(11) unsigned NOT NULL,
  `Event` int(11) unsigned NOT NULL DEFAULT '0',
  `Count` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`AccountID`,`Event`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
