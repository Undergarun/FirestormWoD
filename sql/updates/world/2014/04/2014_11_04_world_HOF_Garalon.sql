--  Garalon & Garalon's leg template
UPDATE creature_template SET minlevel = 93, maxlevel = 93 WHERE entry IN (63053, 62164);
UPDATE creature_template SET scriptname = 'boss_garalon' WHERE entry = 62164;
UPDATE creature_template SET scriptname = 'npc_pheromone_trail' WHERE entry = 63021;
UPDATE creature_template SET scriptname = 'npc_garalon_leg' WHERE entry = 63053;

-- Spell scripts
INSERT INTO spell_script_names VALUES
(122735, 'spell_garalon_furious_swipe'),
(123808, 'spell_garalon_pheromones_forcecast'),
(123495, 'spell_garalon_mend_leg'),
(117709, 'spell_garalon_crush_trigger'),
(123109, 'spell_garalon_pheromones_taunt'),
(122786, 'spell_garalon_broken_leg'),
(123818, 'spell_garalon_damaged');