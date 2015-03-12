DELETE FROM creature WHERE id IN (83435, 84471, 84466) AND map = 1191;
DELETE FROM gameobject WHERE id = 233285 AND map = 1191;

UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, InhabitType = 4 WHERE entry = 84471;