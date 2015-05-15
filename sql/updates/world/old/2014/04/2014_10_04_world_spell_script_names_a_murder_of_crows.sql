UPDATE creature_template SET ScriptName = '' WHERE ScriptName = 'npc_murder_of_crows';
DELETE FROM spell_bonus_data WHERE entry = 131900;
INSERT INTO spell_bonus_data VALUE (131900, 0, 0, 0.288, 0, 'Hunter - A Murder of Crows (triggered)');