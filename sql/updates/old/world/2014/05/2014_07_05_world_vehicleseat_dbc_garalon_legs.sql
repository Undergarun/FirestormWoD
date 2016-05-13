-- seats coords for legs on Garalon
DELETE FROM vehicleseat_dbc WHERE ID BETWEEN 20000 AND 20003;
INSERT INTO `vehicleseat_dbc` (`ID`, `flags`, `attachmentID`, `attachmentOffsetX`, `attachmentOffsetY`, `attachmentOffsetZ`, `flagsB`, `comment`) VALUES
('20000','8192','0','17.08' ,'-14.32','0','0',NULL), 	-- Right front leg
('20001','8192','0','17.08' ,'14.32' ,'0','0',NULL),	-- Left front leg
('20002','8192','0','-24.67','-20.9'  ,'0','0',NULL),	-- Right back leg
('20003','8192','0','-18.87','20.9' ,'0','0',NULL);	-- Left back leg