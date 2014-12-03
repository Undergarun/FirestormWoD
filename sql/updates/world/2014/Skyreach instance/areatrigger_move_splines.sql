CREATE TABLE areatrigger_move_splines
(
	move_curve_id 	int NOT NULL,
	path_id		   	int,
	path_x			float,
	path_y			float,
	path_z			float,
	PRIMARY KEY (move_curve_id, path_id)
);

CREATE TABLE areatrigger_move_template
(
	move_curve_id 	int NOT NULL,
	path_size		int,
	duration		int,
	PRIMARY KEY(move_curve_id)
);

INSERT INTO areatrigger_move_template VALUES ('515', '22', '6000');

INSERT INTO areatrigger_move_splines VALUES ('515', '0', '0', '0', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '1', '0', '0', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '2', '-0.192', '4.997', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '3', '-0.42', '9.992', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '4', '-0.683', '14.985', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '5', '-0.982', '19.978', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '6', '-1.316', '24.969', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '7', '-1.686', '29.96', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '8', '-16.24', '24.448', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '9', '-26.175', '19.098', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '10', '-29.502', '13.979', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '11', '-27.212', '9.056', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '12', '-24.201', '4.158', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '13', '-20.808', '-0.726', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '14', '-16.815', '-5.59', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '15', '-11.664', '10.413', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '16', '0.193', '-15.002', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '17', '3.815', '-9.873', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '18', '3.786', '-4.871', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '19', '0.108', '0.004', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '20', '0', '0', '0');
INSERT INTO areatrigger_move_splines VALUES ('515', '21', '0', '0', '0');

UPDATE areatrigger_move_splines SET path_y=-path_y WHERE move_curve_id=463;