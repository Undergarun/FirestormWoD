DELETE FROM spell_script_names WHERE spell_id IN (167105, 86346);

INSERT INTO spell_script_names VALUES
(167105, 'spell_warr_colossus_smash'),
(167105, 'spell_mastery_weapons_master');

DELETE FROM spell_script_names WHERE spell_id = 159362;
INSERT INTO spell_script_names VALUES
(159362, 'spell_warr_blood_craze');
