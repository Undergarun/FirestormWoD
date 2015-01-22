/*
SQLyog Ultimate v9.02 
MySQL - 5.6.19-log : Database - wod_world
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `battlepay_display_info` */

CREATE TABLE `battlepay_display_info` (
  `DisplayInfoId` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CreatureDisplayInfoID` int(11) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(11) unsigned DEFAULT NULL,
  `Name1` varchar(255) NOT NULL,
  `Name2` varchar(255) NOT NULL,
  `Name3` varchar(255) NOT NULL,
  `Flags` int(11) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `DisplayInfoId` (`DisplayInfoId`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_display_info` */

insert  into `battlepay_display_info`(`DisplayInfoId`,`CreatureDisplayInfoID`,`FileDataID`,`Name1`,`Name2`,`Name3`,`Flags`) values (1,30507,134514,'Mini K.T.','','PigPig !',0);

/*Table structure for table `battlepay_product` */

CREATE TABLE `battlepay_product` (
  `ProductID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `NormalPriceFixedPoint` bigint(20) unsigned NOT NULL,
  `CurrentPriceFixedPoint` bigint(20) unsigned NOT NULL,
  `Type` tinyint(3) unsigned NOT NULL,
  `ChoiceType` tinyint(3) unsigned NOT NULL,
  `Flags` int(11) unsigned NOT NULL,
  `DisplayInfoID` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ProductID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_product` */

insert  into `battlepay_product`(`ProductID`,`NormalPriceFixedPoint`,`CurrentPriceFixedPoint`,`Type`,`ChoiceType`,`Flags`,`DisplayInfoID`) values (1,75,55,0,2,47,1);

/*Table structure for table `battlepay_product_group` */

CREATE TABLE `battlepay_product_group` (
  `GroupID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Name` varchar(255) NOT NULL,
  `IconFileDataID` int(11) NOT NULL,
  `DisplayType` tinyint(3) unsigned NOT NULL,
  `Ordering` int(11) NOT NULL,
  PRIMARY KEY (`GroupID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_product_group` */

insert  into `battlepay_product_group`(`GroupID`,`Name`,`IconFileDataID`,`DisplayType`,`Ordering`) values (1,'Mascottes',939380,0,1);

/*Table structure for table `battlepay_product_item` */

CREATE TABLE `battlepay_product_item` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `ProductID` int(11) unsigned NOT NULL,
  `ItemID` int(11) unsigned NOT NULL,
  `Quantity` int(11) unsigned NOT NULL,
  `DisplayID` int(11) DEFAULT NULL,
  `HasPet` tinyint(1) unsigned NOT NULL,
  `PetResult` tinyint(2) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_product_item` */

insert  into `battlepay_product_item`(`ID`,`ProductID`,`ItemID`,`Quantity`,`DisplayID`,`HasPet`,`PetResult`) values (1,1,49693,1,0,1,0);

/*Table structure for table `battlepay_shop_entry` */

CREATE TABLE `battlepay_shop_entry` (
  `EntryID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `GroupID` int(11) unsigned NOT NULL,
  `ProductID` int(11) unsigned NOT NULL,
  `Ordering` int(11) NOT NULL,
  `Flags` int(11) unsigned NOT NULL DEFAULT '0',
  `BannerType` tinyint(3) unsigned NOT NULL,
  `DisplayInfoID` int(11) unsigned NOT NULL,
  PRIMARY KEY (`EntryID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_shop_entry` */

insert  into `battlepay_shop_entry`(`EntryID`,`GroupID`,`ProductID`,`Ordering`,`Flags`,`BannerType`,`DisplayInfoID`) values (1,1,1,1,0,0,0);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
