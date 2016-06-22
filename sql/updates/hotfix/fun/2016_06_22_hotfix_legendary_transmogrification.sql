DELETE FROM `_hotfixs` WHERE `Hash` = 2442913102 AND `Entry` IN (46017,19019,71086,49623,17182,77949,32837,34334,77950,32838);
INSERT INTO `_hotfixs` (`Entry`, `Hash`, `Date`, `Comment`) VALUES 	(46017, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(19019, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(71086, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(49623, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(17182, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(77949, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(32837, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(34334, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(77950, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog'),
									(32838, '2442913102', UNIX_TIMESTAMP(), 'Legendary item transmog');

UPDATE `item_sparse` SET `Flags2`=`Flags2` & ~0x00600000 WHERE ID IN (46017,19019,71086,49623,17182,77949,32837,34334,77950,32838);
