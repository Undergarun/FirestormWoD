CREATE TABLE `player_race_stats` (
  `name` varchar(11) NOT NULL DEFAULT '',
  `human` int(11) unsigned NOT NULL,
  `dwarf` int(11) unsigned NOT NULL,
  `nightelf` int(11) unsigned NOT NULL,
  `orc` int(11) unsigned NOT NULL,
  `tauren` int(11) unsigned NOT NULL,
  `undead` int(11) unsigned NOT NULL,
  `gnome` int(11) unsigned NOT NULL,
  `troll` int(11) unsigned NOT NULL,
  `bloodelf` int(11) unsigned NOT NULL,
  `draenei` int(11) unsigned NOT NULL,
  `goblin` int(11) unsigned NOT NULL,
  `worgen` int(11) unsigned NOT NULL,
  `pandaren` int(11) unsigned NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `player_race_stats` (`name`, `human`, `dwarf`, `nightelf`, `orc`, `tauren`, `undead`, `gnome`, `troll`, `bloodelf`, `draenei`, `goblin`, `worgen`, `pandaren`)
VALUES
('Str', 0, 5, -4, 3, 5, -1, -5, 1, -3, 1, -3, 3, 0),
('Agi', 0, -4, 4, -3, -4, -2, 2, 2, 2, -3, 2, 2, -2),
('Sta', 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1),
('int', 0, -1, 0, -3, -4, -2, 3, -4, 3, 0, 3, -4, -1),
('Spt', 0, -1, 0, 2, 2, 5, 0, 1, -2, 2, -2, -1, 2);