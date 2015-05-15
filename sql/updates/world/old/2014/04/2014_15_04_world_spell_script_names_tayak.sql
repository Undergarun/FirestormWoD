-- Reapplying spell script for Tayak
DELETE FROM spell_script_names WHERE spell_id IN (123459, 124258, 124783, 122853, 123814, 124024, 123633, 123600, 123616);
INSERT INTO spell_script_names VALUES
(123459, 'spell_tayak_wind_step'),
(124258, 'spell_tayak_storms_vehicle'),
(124783, 'spell_tayak_storm_unleashed_dmg'),
(122853, 'spell_tempest_slash'),
(123814, 'spell_tayak_su_visual'),
(124024, 'spell_su_dummy_visual'),
(123633, 'spell_gale_winds'),
(123600, 'spell_su_dummy'),
(123616, 'spell_su_dumaura');

UPDATE creature_template SET scriptname = "npc_tempest_slash_tornado" WHERE entry = 64373;
UPDATE creature_template SET scriptname = "npc_storm_unleashed_tornado" WHERE entry = 63278;
UPDATE creature_template SET scriptname = "mob_gale_winds_stalker" WHERE entry = 63292;