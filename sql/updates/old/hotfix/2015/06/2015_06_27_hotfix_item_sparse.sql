-- Set transmog set 1524 transmogrifiable - http://www.wowhead.com/transmog-set=1524/silent-assassin-armor

UPDATE item_sparse SET flags2 = (flags2 & ~0x200000) WHERE id IN (90119, 90120, 90121, 90122, 90123, 90124, 90125, 90126);