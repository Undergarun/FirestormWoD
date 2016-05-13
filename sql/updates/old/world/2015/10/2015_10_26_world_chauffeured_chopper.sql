DELETE FROM achievement_reward WHERE entry = 9909;
INSERT INTO achievement_reward VALUE
(9909, 0, 0, 122718, 7406, 'A Most Splendid Convenience', 'Adventurer! Your wondrous deeds have been celebrated all around the world. In gratuity for your great services in preventing the world''s destruction several times over, I, Oglethorpe Obnoticus, on behalf of the citizens of Azeroth, do hereby present you with the ability to relax no matter where you are traveling!');

DELETE FROM item_script_names WHERE Id IN (122718, 120968, 122703);
INSERT INTO item_script_names VALUES
(122718, 'item_script_clinking_present'),
(120968, 'item_script_chauffeured_chopper'),
(122703, 'item_script_chauffeured_chopper');

DELETE FROM spell_script_names WHERE spell_id IN (179244, 179245);
INSERT INTO spell_script_names VALUES
(179244, 'spell_item_summon_chauffeur'),
(179245, 'spell_item_summon_chauffeur');

DELETE FROM item_loot_template WHERE entry = 122718;