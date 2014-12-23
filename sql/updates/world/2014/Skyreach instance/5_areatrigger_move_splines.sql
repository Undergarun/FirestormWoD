CREATE TABLE IF NOT EXISTS areatrigger_move_splines
(
	move_curve_id 	int NOT NULL,
	path_id		   	int,
	path_x			float,
	path_y			float,
	path_z			float,
	PRIMARY KEY (move_curve_id, path_id)
);

CREATE TABLE IF NOT EXISTS areatrigger_move_template
(
	move_curve_id 	int NOT NULL,
	path_size		int,
	duration		int,
	PRIMARY KEY(move_curve_id)
);

REPLACE INTO areatrigger_move_template VALUES ('515', '22', '6000');

REPLACE INTO areatrigger_move_splines VALUES ('515', '0', '0', '0', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '1', '0', '0', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '2', '-0.192', '4.997', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '3', '-0.42', '9.992', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '4', '-0.683', '14.985', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '5', '-0.982', '19.978', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '6', '-1.316', '24.969', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '7', '-1.686', '29.96', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '8', '-16.24', '24.448', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '9', '-26.175', '19.098', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '10', '-29.502', '13.979', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '11', '-27.212', '9.056', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '12', '-24.201', '4.158', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '13', '-20.808', '-0.726', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '14', '-16.815', '-5.59', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '15', '-11.664', '10.413', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '16', '0.193', '-15.002', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '17', '3.815', '-9.873', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '18', '3.786', '-4.871', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '19', '0.108', '0.004', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '20', '0', '0', '0');
REPLACE INTO areatrigger_move_splines VALUES ('515', '21', '0', '0', '0');

UPDATE areatrigger_move_splines SET path_y=-path_y WHERE move_curve_id=463;
UPDATE areatrigger_move_template SET duration=3000 WHERE move_curve_id=463;

REPLACE INTO `areatrigger_move_template`(`move_curve_id`,`path_size`,`duration`) VALUES (463,4,3000),(514,22,6000),(516,22,6000),(517,22,6000),(518,22,6000),(519,22,6000),(520,22,6000),(521,22,6000),(532,22,10000);
REPLACE INTO `areatrigger_move_splines`(`move_curve_id`,`path_id`,`path_x`,`path_y`,`path_z`) VALUES (463,0,0,-0,0),(463,1,0,-0,0),(463,2,0,14.443,0),(463,3,0,14.443,0),(514,0,0,0,0),(514,1,0,0,0),(514,2,0,1.02222,0),(514,3,0,2.00952,0),(514,4,0,2.96216,0),(514,5,0,3.88,0),(514,6,0,4.76306,0),(514,7,0,5.61133,0),(514,8,0,-9.84338,0),(514,9,0,-20.7776,0),(514,10,0,-25.2447,0),(514,11,0,-23.2447,0),(514,12,0,-22.4767,0),(514,13,0,-20.3657,0),(514,14,0,-17.6681,0),(514,15,0,-13.8373,0),(514,16,0,-3.44238,0),(514,17,0,1.31262,0),(514,18,0,2.49512,0),(514,19,0,0.10498,0),(514,20,0,0,0),(514,21,0,0,0),(516,0,0,0,0),(516,1,0,0,0),(516,2,0,-4.53906,0),(516,3,0,-9.09314,0),(516,4,0,-13.6622,0),(516,5,0,-18.2463,0),(516,6,0,-22.8456,0),(516,7,0,-27.4597,0),(516,8,0,-29.1564,0),(516,9,0,-28.8999,0),(516,10,0,-25.8492,0),(516,11,0,-20.0045,0),(516,12,0,-14.692,0),(516,13,0,-8.79919,0),(516,14,0,-2.65295,0),(516,15,0,3.98291,0),(516,16,0,13.4548,0),(516,17,0,10.5286,0),(516,18,0,6.05872,0),(516,19,0,0.0454102,0),(516,20,0,0,0),(516,21,0,0,0),(517,0,0,0,0),(517,1,0,0,0),(517,2,0,3.42297,0),(517,3,0,6.87195,0),(517,4,0,10.347,0),(517,5,0,13.848,0),(517,6,0,17.375,0),(517,7,0,20.928,0),(517,8,0,28.2961,0),(517,9,0,32.2843,0),(517,10,0,31.437,0),(517,11,0,26.4783,0),(517,12,0,20.9929,0),(517,13,0,15.2273,0),(517,14,0,9.02307,0),(517,15,0,1.97144,0),(517,16,0,-9.98792,0),(517,17,0,-9.35596,0),(517,18,0,-6.05286,0),(517,19,0,-0.0784912,0),(517,20,0,0,0),(517,21,0,0,0),(518,0,0,0,0),(518,1,0,0,0),(518,2,0,-1.44482,0),(518,3,0,-2.85571,0),(518,4,0,-4.23254,0),(518,5,0,-5.57544,0),(518,6,0,-6.88428,0),(518,7,0,-8.15906,0),(518,8,0,7.396,0),(518,9,0,18.5315,0),(518,10,0,23.3444,0),(518,11,0,21.8346,0),(518,12,0,21.5293,0),(518,13,0,19.9109,0),(518,14,0,17.719,0),(518,15,0,14.4193,0),(518,16,0,4.70203,0),(518,17,0,-0.392212,0),(518,18,0,-1.99377,0),(518,19,0,-0.102661,0),(518,20,0,0,0),(518,21,0,0,0),(519,0,0,0,0),(519,1,0,0,0),(519,2,0,3.70386,0),(519,3,0,7.38391,0),(519,4,0,11.0402,0),(519,5,0,14.6726,0),(519,6,0,18.2812,0),(519,7,0,21.8661,0),(519,8,0,8.28931,0),(519,9,0,-2.19519,0),(519,10,0,-8.25562,0),(519,11,0,-10.5543,0),(519,12,0,-12.3712,0),(519,13,0,-13.9318,0),(519,14,0,-15.0909,0),(519,15,0,-15.475,0),(519,16,0,-11.3685,0),(519,17,0,-5.11121,0),(519,18,0,-1.29773,0),(519,19,0,0.0718994,0),(519,20,0,0,0),(519,21,0,0,0),(520,0,0,0,0),(520,1,0,0,0),(520,2,0,-4.3645,0),(520,3,0,-8.71167,0),(520,4,0,-13.0417,0),(520,5,0,-17.3545,0),(520,6,0,-21.65,0),(520,7,0,-25.9283,0),(520,8,0,-14.4105,0),(520,9,0,-5.13318,0),(520,10,0,0.938721,0),(520,11,0,3.8053,0),(520,12,0,7.28247,0),(520,13,0,10.0941,0),(520,14,0,12.6149,0),(520,15,0,14.574,0),(520,16,0,13.2797,0),(520,17,0,7.06506,0),(520,18,0,2.62122,0),(520,19,0,-0.0520019,0),(520,20,0,0,0),(520,21,0,0,0),(521,0,0,0,0),(521,1,0,0,0),(521,2,0,-2.81067,0),(521,3,0,-5.65088,0),(521,4,0,-8.52051,0),(521,5,0,-11.4197,0),(521,6,0,-14.3483,0),(521,7,0,-17.3064,0),(521,8,0,-26.7281,0),(521,9,0,-32.3185,0),(521,10,0,-32.4275,0),(521,11,0,-27.876,0),(521,12,0,-22.7274,0),(521,13,0,-17.2611,0),(521,14,0,-11.2977,0),(521,15,0,-4.37378,0),(521,16,0,8.11353,0),(521,17,0,8.46655,0),(521,18,0,5.79175,0),(521,19,0,0.0889893,0),(521,20,0,0,0),(521,21,0,0,0),(532,0,5.10928,13.0106,0),(532,1,5.10928,13.0106,0),(532,2,4.80472,13.4603,0),(532,3,4.01797,13.0189,0),(532,4,2.37979,11.6864,0),(532,5,0.574858,9.46277,0),(532,6,-1.06357,6.33972,0),(532,7,-1.95968,3.21668,0),(532,8,-2.57044,0.984741,0),(532,9,-2.58411,-0.356079,0),(532,10,-1.75745,-0.805786,0),(532,11,-0.348114,-0.364258,0),(532,12,0.667755,0.968262,0),(532,13,2.58548,3.19177,0),(532,14,3.56473,4.55139,0),(532,15,4.80606,6.75623,0),(532,16,6.34561,10.7704,0),(532,17,6.04312,13.8934,0),(532,18,6.41477,16.1254,0),(532,19,6.68357,17.4662,0),(532,20,5.10928,13.0106,0),(532,21,5.10928,13.0106,0);

INSERT INTO areatrigger_move_template (`move_curve_id`, `path_size`, `duration`) VALUES(516, 22, 6000) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_size` = VALUES(`path_size`), `duration` = VALUES(`duration`);

INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 1, 0, 0, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 2, 0, -4.786743, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 3, 0, -9.583862, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 4, 0, -14.39136, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 5, 0, -19.20923, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 6, 0, -24.0376, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 7, 0, -28.87622, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 8, 0, -28.40344, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 9, 0, -26.57947, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 10, 0, -22.82251, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 11, 0, -17.13245, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 12, 0, -11.81067, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 13, 0, -6.087402, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 14, 0, -0.1888428, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 15, 0, 6.048462, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 16, 0, 14.2478, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 17, 0, 10.5769, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 18, 0, 5.838013, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 19, 0, 0.03137207, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 20, 0, 0, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);
INSERT INTO areatrigger_move_splines (`move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z`) VALUES(516, 21, 0, 0, 0) ON DUPLICATE KEY UPDATE `move_curve_id` = VALUES(`move_curve_id`), `path_id` = VALUES(`path_id`), `path_x` = VALUES(`path_x`), `path_y` = VALUES(`path_y`), `path_z` = VALUES(`path_z`);

