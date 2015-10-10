/*
SQLyog Ultimate v9.02 
MySQL - 5.5.42-37.1 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `character_renderer_queue` (
	`guid` int (11),
	`race` tinyint (3),
	`gender` tinyint (3),
	`class` tinyint (3),
	`skinColor` tinyint (3),
	`face` tinyint (3),
	`hairStyle` tinyint (3),
	`hairColor` tinyint (3),
	`facialHair` tinyint (3),
	`equipment` blob 
); 
