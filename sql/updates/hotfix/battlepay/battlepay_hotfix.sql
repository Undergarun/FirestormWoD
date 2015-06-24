/*
SQLyog Ultimate v9.02 
MySQL - 5.6.19-log : Database - 612_hotfix
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `wb_access_control_list` */

DROP TABLE IF EXISTS `wb_access_control_list`;

CREATE TABLE `wb_access_control_list` (
  `ID` int(11) unsigned NOT NULL,
  `Url` text NOT NULL,
  `AccessFlags` int(11) unsigned NOT NULL,
  `Unk1` int(11) unsigned NOT NULL,
  `Restricted` int(11) unsigned NOT NULL,
  `Region` int(11) unsigned NOT NULL,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `wb_access_control_list` */

insert  into `wb_access_control_list`(`ID`,`Url`,`AccessFlags`,`Unk1`,`Restricted`,`Region`,`BuildVerified`) values (1,'*://',2154,1,0,0,0),(2,'http://',2154,1,0,0,0),(80,'https://',2154,1,0,0,0);

/*Table structure for table `wb_access_control_list_locale` */

DROP TABLE IF EXISTS `wb_access_control_list_locale`;

CREATE TABLE `wb_access_control_list_locale` (
  `ID` int(11) unsigned NOT NULL,
  `UrlLang_koKR` text CHARACTER SET latin1,
  `UrlLang_frFR` text CHARACTER SET latin1,
  `UrlLang_deDE` text CHARACTER SET latin1,
  `UrlLang_zhCN` text CHARACTER SET latin1,
  `UrlLang_zhTW` text CHARACTER SET latin1,
  `UrlLang_esES` text CHARACTER SET latin1,
  `UrlLang_esMX` text CHARACTER SET latin1,
  `UrlLang_ruRU` text CHARACTER SET latin1,
  `UrlLang_ptPT` text CHARACTER SET latin1,
  `UrlLang_itIT` text CHARACTER SET latin1,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `wb_access_control_list_locale` */

/*Table structure for table `wb_cert_whitelist` */

DROP TABLE IF EXISTS `wb_cert_whitelist`;

CREATE TABLE `wb_cert_whitelist` (
  `ID` int(10) unsigned NOT NULL,
  `Url` text NOT NULL,
  `Unk1` tinyint(3) unsigned NOT NULL,
  `Unk2` tinyint(3) unsigned NOT NULL,
  `Unk3` int(10) unsigned NOT NULL,
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `wb_cert_whitelist` */

insert  into `wb_cert_whitelist`(`ID`,`Url`,`Unk1`,`Unk2`,`Unk3`,`BuildVerified`) values (13,'*.com',1,0,0,0);

/*Table structure for table `wb_cert_whitelist_locale` */

DROP TABLE IF EXISTS `wb_cert_whitelist_locale`;

CREATE TABLE `wb_cert_whitelist_locale` (
  `ID` int(11) unsigned NOT NULL,
  `UrlLang_koKR` text CHARACTER SET latin1,
  `UrlLang_frFR` text CHARACTER SET latin1,
  `UrlLang_deDE` text CHARACTER SET latin1,
  `UrlLang_zhCN` text CHARACTER SET latin1,
  `UrlLang_zhTW` text CHARACTER SET latin1,
  `UrlLang_esES` text CHARACTER SET latin1,
  `UrlLang_esMX` text CHARACTER SET latin1,
  `UrlLang_ruRU` text CHARACTER SET latin1,
  `UrlLang_ptPT` text CHARACTER SET latin1,
  `UrlLang_itIT` text CHARACTER SET latin1,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `wb_cert_whitelist_locale` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
