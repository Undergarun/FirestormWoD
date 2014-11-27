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
/*Table structure for table `pet_stats` */

CREATE TABLE `pet_stats` (
  `entry` int(11) unsigned NOT NULL COMMENT 'entry of the pet, 1 for hunter pet',
  `speed` float NOT NULL DEFAULT '1.14' COMMENT 'speed of the pet',
  `powerstatbase` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'which owner''s stat using to compute attack & spell power (0 = AP, 1 = SP)',
  `armor_coef` float NOT NULL DEFAULT '1' COMMENT '% of armor of the owner the pet have',
  `apsp_coef` float NOT NULL DEFAULT '1' COMMENT '% of the attack power / spellpower of the owner the pet have',
  `health_coef` float NOT NULL DEFAULT '1' COMMENT '% of health of the owner the pet have',
  `damage_coef` float NOT NULL DEFAULT '0.85' COMMENT '% of the attack power / spell power to use to compute min/max dmg',
  `attackspeed` float NOT NULL DEFAULT '1.82' COMMENT '% of the base attack speed of the pet',
  `powertype` int(11) unsigned NOT NULL,
  `createpower` float NOT NULL,
  `secondarystat_coef` float NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Data for the table `pet_stats` */

insert  into `pet_stats`(`entry`,`speed`,`powerstatbase`,`armor_coef`,`apsp_coef`,`health_coef`,`damage_coef`,`attackspeed`,`powertype`,`createpower`,`secondarystat_coef`) values (1,1.14,0,1.7,0.6,0.7,0.85,1.82,2,100,0),(510,1.14,1,3,0.75,0.5,1,2,0,1,0);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
