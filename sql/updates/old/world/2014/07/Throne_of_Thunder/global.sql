DELETE FROM instance_template WHERE map = 1098;
INSERT INTO instance_template VALUE (1098, 870, 'instance_throne_of_thunder', 1);

DELETE FROM areatrigger_teleport WHERE id = 8881;
INSERT INTO areatrigger_teleport VALUES
(8881, 'Throne of Thunder - Exit', 1064, 7255.129, 5024.551, 76.163, 2.360233),
(8882, 'Throne of Thunder - Entrance', 1098, 5891.801, 6617.557, 106.108, 4.700731);