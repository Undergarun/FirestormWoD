INSERT INTO creature_text VALUES ('75210', '0', '0', 'Me stop them!', '14', '7', '100', '0', '0', '0', 'Warder attack1 when mage dies.');
INSERT INTO creature_text VALUES ('75210', '1', '0', 'We make them pay!', '14', '7', '100', '0', '0', '0', 'Warder attack2 when mage dies.');

INSERT INTO creature_text VALUES ('74787', '8', '0', 'Ha Ha! Smashed!', '14', '7', '100', '0', '0', '41649', 'Crushto boss slay 1');
INSERT INTO creature_text VALUES ('74787', '9', '0', 'Run away, little ones! Ha ha!', '14', '7', '100', '0', '0', '41653', 'Crushto boss spell 2');

INSERT INTO creature_text VALUES ('74355', '0', '0', 'Here\'s to a less miserable future!', '12', '7', '25', '10', '0', '', 'Captured miner');
INSERT INTO creature_text VALUES ('74355', '0', '1', 'I will be strong and stand with you!', '12', '7', '25', '10', '0', '', 'Captured miner');
INSERT INTO creature_text VALUES ('74355', '0', '2', 'We will not be slaves again!', '12', '7', '25', '10', '0', '', 'Captured miner');

UPDATE creature_text SET sound=41645 WHERE entry=74787 AND groupid=2;
UPDATE creature_text SET sound=41646 WHERE entry=74787 AND groupid=7;
UPDATE creature_text SET sound=41647 WHERE entry=74787 AND groupid=0;
UPDATE creature_text SET sound=41648 WHERE entry=74787 AND groupid=1;
UPDATE creature_text SET sound=41649 WHERE entry=74787 AND groupid=8;
UPDATE creature_text SET sound=41651 WHERE entry=74787 AND groupid=6;
UPDATE creature_text SET sound=41652 WHERE entry=74787 AND groupid=5;
UPDATE creature_text SET sound=41654 WHERE entry=74787 AND groupid=3;
UPDATE creature_text SET sound=41650 WHERE entry=74787 AND groupid=4;