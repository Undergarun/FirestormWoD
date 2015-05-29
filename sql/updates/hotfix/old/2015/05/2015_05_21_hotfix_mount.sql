/*
SQLyog Ultimate v11.11 (64 bit)
MySQL - 5.6.17 : Database - 540_hotfix
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `mount` */

DROP TABLE IF EXISTS `mount`;

CREATE TABLE `mount` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Category` int(10) unsigned NOT NULL,
  `CreatureDisplayID` int(10) unsigned NOT NULL,
  `Flags` int(10) unsigned NOT NULL,
  `Name` text NOT NULL,
  `Description` text NOT NULL,
  `Icon` text NOT NULL,
  `FilterCategory` int(10) unsigned NOT NULL,
  `SpellID` int(10) unsigned NOT NULL,
  `PlayerConditionID` int(10) unsigned NOT NULL,
  `BuildVerified` int(10) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=600001 DEFAULT CHARSET=latin1;

/*Data for the table `mount` */

insert  into `mount`(`ID`,`Category`,`CreatureDisplayID`,`Flags`,`Name`,`Description`,`Icon`,`FilterCategory`,`SpellID`,`PlayerConditionID`,`BuildVerified`) values (600000,248,51323,0,'Stormcrow','Summons and dismisses a Stormcrow.  This mount changes depending on your Riding skill and location.','|cFFFFD200In-Game Shop|r',0,147595,0,19865);

/*Table structure for table `mount_locale` */

DROP TABLE IF EXISTS `mount_locale`;

CREATE TABLE `mount_locale` (
  `ID` int(11) unsigned NOT NULL,
  `Name_koKR` text CHARACTER SET latin1,
  `Name_frFR` text CHARACTER SET latin1,
  `Name_deDE` text CHARACTER SET latin1,
  `Name_zhCN` text CHARACTER SET latin1,
  `Name_zhTW` text CHARACTER SET latin1,
  `Name_esES` text CHARACTER SET latin1,
  `Name_esMX` text CHARACTER SET latin1,
  `Name_ruRU` text CHARACTER SET latin1,
  `Name_ptPT` text CHARACTER SET latin1,
  `Name_itIT` text CHARACTER SET latin1,
  `Description_koKR` text CHARACTER SET latin1,
  `Description_frFR` text CHARACTER SET latin1,
  `Description_deDE` text CHARACTER SET latin1,
  `Description_zhCN` text CHARACTER SET latin1,
  `Description_zhTW` text CHARACTER SET latin1,
  `Description_esES` text CHARACTER SET latin1,
  `Description_esMX` text CHARACTER SET latin1,
  `Description_ruRU` text CHARACTER SET latin1,
  `Description_ptPT` text CHARACTER SET latin1,
  `Description_itIT` text CHARACTER SET latin1,
  `Icon_koKR` text CHARACTER SET latin1,
  `Icon_frFR` text CHARACTER SET latin1,
  `Icon_deDE` text CHARACTER SET latin1,
  `Icon_zhCN` text CHARACTER SET latin1,
  `Icon_zhTW` text CHARACTER SET latin1,
  `Icon_esES` text CHARACTER SET latin1,
  `Icon_esMX` text CHARACTER SET latin1,
  `Icon_ruRU` text CHARACTER SET latin1,
  `Icon_ptPT` text CHARACTER SET latin1,
  `Icon_itIT` text CHARACTER SET latin1,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `mount_locale` */

insert  into `mount_locale`(`ID`,`Name_koKR`,`Name_frFR`,`Name_deDE`,`Name_zhCN`,`Name_zhTW`,`Name_esES`,`Name_esMX`,`Name_ruRU`,`Name_ptPT`,`Name_itIT`,`Description_koKR`,`Description_frFR`,`Description_deDE`,`Description_zhCN`,`Description_zhTW`,`Description_esES`,`Description_esMX`,`Description_ruRU`,`Description_ptPT`,`Description_itIT`,`Icon_koKR`,`Icon_frFR`,`Icon_deDE`,`Icon_zhCN`,`Icon_zhTW`,`Icon_esES`,`Icon_esMX`,`Icon_ruRU`,`Icon_ptPT`,`Icon_itIT`,`BuildVerified`) values (600000,NULL,'Corbeau des tempêtes',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Invoque et renvoie un corbeau des tempêtes. Cette monture change en fonction de votre compétence de monte et de l\'endroit où vous vous trouvez.',NULL,NULL,NULL,'Invoca y retira a un cuervo tormentoso. Esta montura cambia según tu habilidad en equitación y la zona en la que estés.','Invoca y retira a un cuervo tormentoso. Esta montura cambia según tu habilidad en equitación y la zona en la que estés.',NULL,NULL,NULL,NULL,'|cFFFFD200In-Game Shop|r',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);

INSERT INTO `_hotfixs` (`Entry`, `Hash`, `Date`, `Comment`) VALUES ('600000', '2524150337', '1431491750', 'Mount - Stormcrow');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
