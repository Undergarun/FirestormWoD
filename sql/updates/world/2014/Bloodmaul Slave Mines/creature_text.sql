-- INSERT INTO creature_text VALUES ('npc_entry', 'groupid', 'id', 'text', 'type', 'language', 'probability', 'emote', 'duration', 'sound', 'comment');

INSERT INTO creature_text VALUES ('75210', '0', '0', 'Me stop them!', '14', '7', '100', '0', '0', '0', 'Warder attack1 when mage dies.');
INSERT INTO creature_text VALUES ('75210', '0', '1', 'We make them pay!', '14', '7', '100', '0', '0', '0', 'Warder attack2 when mage dies.');

INSERT INTO creature_text VALUES ('74787', '0', '8', 'Ha Ha! Smashed!', '14', '7', '100', '0', '0', '41649', 'Crushto boss slay 1');
INSERT INTO creature_text VALUES ('74787', '0', '9', 'Run away, little ones! Ha ha!', '14', '7', '100', '0', '0', '41653', 'Crushto boss spell 2');

UPDATE creature_text SET sound=41645 WHERE entry=74787 AND id=2;
UPDATE creature_text SET sound=41646 WHERE entry=74787 AND id=7;
UPDATE creature_text SET sound=41647 WHERE entry=74787 AND id=0;
UPDATE creature_text SET sound=41648 WHERE entry=74787 AND id=1;
UPDATE creature_text SET sound=41649 WHERE entry=74787 AND id=8;
UPDATE creature_text SET sound=41651 WHERE entry=74787 AND id=6;
UPDATE creature_text SET sound=41652 WHERE entry=74787 AND id=5;
UPDATE creature_text SET sound=41654 WHERE entry=74787 AND id=3;
UPDATE creature_text SET sound=41650 WHERE entry=74787 AND id=4;