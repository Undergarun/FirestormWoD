--  Garalon & Garalon's leg template
UPDATE creature_template SET minlevel = 93, maxlevel = 93 WHERE entry IN (63053, 62164);
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, modelid3 = 0, modelid4 = 0 WHERE entry = 63021;
UPDATE creature_template SET scriptname = 'boss_garalon' WHERE entry = 62164;
UPDATE creature_template SET scriptname = 'npc_pheromone_trail' WHERE entry = 63021;
UPDATE creature_template SET scriptname = 'npc_garalon_leg' WHERE entry = 63053;

INSERT INTO vehicle_template_accessory (entry, accessory_entry, seat_id, minion, description, summontype, summontimer) VALUES (62164, 63053, 0, 1, "Garalon legs", 8, 0);
INSERT INTO vehicle_template_accessory (entry, accessory_entry, seat_id, minion, description, summontype, summontimer) VALUES (62164, 63053, 1, 1, "Garalon legs", 8, 0);
INSERT INTO vehicle_template_accessory (entry, accessory_entry, seat_id, minion, description, summontype, summontimer) VALUES (62164, 63053, 2, 1, "Garalon legs", 8, 0);
INSERT INTO vehicle_template_accessory (entry, accessory_entry, seat_id, minion, description, summontype, summontimer) VALUES (62164, 63053, 3, 1, "Garalon legs", 8, 0);
INSERT INTO `npc_spellclick_spells`(`npc_entry`,`spell_id`,`cast_flags`,`user_type`) VALUES ( '62164','123432','1','0'); 

-- Spell scripts
INSERT INTO spell_script_names VALUES
(122735, 'spell_garalon_furious_swipe'),
(123808, 'spell_garalon_pheromones_forcecast'),
(123495, 'spell_garalon_mend_leg'),
(117709, 'spell_garalon_crush_trigger'),
(123109, 'spell_garalon_pheromones_taunt'),
(122786, 'spell_garalon_broken_leg'),
(123818, 'spell_garalon_damaged'),
(128573, 'spell_garalon_pheromones_summon'),
(123120, 'spell_garalon_pheromones_trail_dmg'),
(123100, 'spell_garalon_pheromones_switch');