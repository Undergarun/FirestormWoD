DROP TABLE IF EXISTS guild_challenge_reward;

CREATE TABLE `guild_challenge_reward` (
  `Type` INT(10) UNSIGNED,
  `Experience` INT(10) UNSIGNED,
  `Gold` INT(10) UNSIGNED,
  `Gold2` INT(10) UNSIGNED,
  `Count` INT(10) UNSIGNED
) ENGINE=INNODB DEFAULT CHARSET=latin1;

INSERT INTO guild_challenge_reward VALUES
(0, 0, 0, 0, 0), -- None
(1, 300000, 250, 125, 7), -- Dungeon
(2, 3000000, 1000, 500, 1), -- Raid
(3, 1500000, 500, 250, 3), -- Dungeon Challenge
(4, 50000, 250, 125, 15), -- Scenarios
(5, 1000000, 500, 250, 3); -- Rated BGs