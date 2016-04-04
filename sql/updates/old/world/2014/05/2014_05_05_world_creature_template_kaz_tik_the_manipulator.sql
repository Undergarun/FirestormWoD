#64013/NPC - Kaz tik the Manipulator

	UPDATE `creature_template` SET `ScriptName` = 'mob_second_kaz_tik_the_manipulator' WHERE `entry` = 64013;
	
	DELETE FROM script_waypoint WHERE entry = 64013;
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','0','-849.44','3816.6','-0.36','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','1','-860.89','3813.53','-0.26','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','2','-875.88','3811.46','-0.37','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','3','-891.61','3821.75','0.32','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','4','-910.83','3826.96','-0.14','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','5','-933.82','3837.96','0.05','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','6','-952.33','3844.66','0.39','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','7','-958.33','3865.52','0.29','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','8','-961.59','3876.36','-0.36','0','First stop');
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','9','-980.374','3892.83','-0.326','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','10','-1012.74','3916.78','0.179','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','11','-1044.31','3928.69','0.599','0','second stop');
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','12','-1103.34','3943.14','0.007','0','third stop');
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','13','-1136.54','3936.81','0.552','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64013','14','-1150.45','3906.24','1.835','0',NULL);
