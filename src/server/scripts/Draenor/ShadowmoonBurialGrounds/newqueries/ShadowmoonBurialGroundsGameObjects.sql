/*
SQLyog Community v11.51 (64 bit)
MySQL - 5.5.44 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `gameobject` (
	`id` mediumint (8),
	`map` smallint (5),
	`zoneId` smallint (8),
	`areaId` smallint (8),
	`spawnMask` int (8),
	`phaseMask` int (10),
	`position_x` float ,
	`position_y` float ,
	`position_z` float ,
	`orientation` float ,
	`rotation0` float ,
	`rotation1` float ,
	`rotation2` float ,
	`rotation3` float ,
	`spawntimesecs` int (11),
	`animprogress` tinyint (3),
	`state` tinyint (3),
	`isActive` tinyint (1),
	`custom_flags` int (10),
	`protect_anti_doublet` int (255)
); 
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('233988','1176','6932','6932','6','1','1861.66','-532.858','202.049','2.71546','0','0','1','-0.0000000437114','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('233989','1176','6932','6932','6','1','1825.1','-548.965','201.427','1.16113','0','0','1','-0.0000000437114','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('233919','1176','6932','6932','6','1','1795.51','-117.688','274.149','4.71239','0','0','1','-0.0000000437114','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('225623','1176','6932','6932','6','1','1720.89','56.8767','287.023','5.2831','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('225623','1176','6932','6932','6','1','1796.19','71.599','261.638','2.94608','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('225623','1176','6932','6932','6','1','1882.16','-98.0573','287.019','5.58554','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('212899','1176','6932','6932','6','1','1712.37','252.684','328.68','4.96675','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('233920','1176','6932','6932','6','1','1746.88','-738.365','235.519','1.19284','0','0','1','-0.0000000437114','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('233921','1176','6932','6932','6','1','1795.97','21.5179','261.368','4.69452','0','0','1','-0.0000000437114','7200','255','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('225623','1176','6932','6932','6','1','1711.64','-100.771','287.015','0.319057','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('233990','1176','6932','6932','6','1','1824.97','-496.978','201.573','1.98544','0','0','1','-0.0000000437114','7200','255','0','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('225623','1176','6932','6932','6','1','1870.83','57.4323','286.611','1.12314','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237470','1176','6932','6932','6','1','1830.55','121.743','294.556','3.02442','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237482','1176','6932','6932','6','1','1796.33','71.7257','264.118','0','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('227851','1176','6932','6932','6','1','1723.88','-246.312','251.127','5.91247','0','0','0','1','7200','255','0','1','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('227851','1176','6932','6932','6','1','1715.58','-284.375','251.745','6.22654','0','0','0','1','7200','255','0','1','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237482','1176','6932','6932','6','1','1796.33','71.7257','264.118','0','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237482','1176','6932','6932','6','1','1796.33','71.7257','264.118','0','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237471','1176','6932','6932','6','1','1663.39','-231.979','253.465','4.67508','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('237471','1176','6932','6932','6','1','1663.39','-231.979','253.465','4.67508','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('239083','1176','6932','6932','6','1','1749.4','-732.753','234.745','1.17938','0','0','0','1','7200','255','1','0','0',NULL);
insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `custom_flags`, `protect_anti_doublet`) values('227851','1176','6932','6932','6','8','1742.59','-196.503','253.417','3.14675','0','0','0.999997','-0.00257997','300','0','1','0','0',NULL);
