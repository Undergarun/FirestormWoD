DELETE FROM character_archaeology_sites;
INSERT INTO character_archaeology_sites(guid, map, sites) (SELECT guid, 0, "9999 9999 9999 9999" FROM character_archaeology);
INSERT INTO character_archaeology_sites(guid, map, sites) (SELECT guid, 1, "9999 9999 9999 9999" FROM character_archaeology);
INSERT INTO character_archaeology_sites(guid, map, sites) (SELECT guid, 530, "9999 9999 9999 9999" FROM character_archaeology);
INSERT INTO character_archaeology_sites(guid, map, sites) (SELECT guid, 571, "9999 9999 9999 9999" FROM character_archaeology);
INSERT INTO character_archaeology_sites(guid, map, sites) (SELECT guid, 870, "9999 9999 9999 9999" FROM character_archaeology);
INSERT INTO character_archaeology_sites(guid, map, sites) (SELECT guid, 1116, "9999 9999 9999 9999" FROM character_archaeology);

UPDATE character_archaeology SET counts="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";