UPDATE creature_template SET ScriptName = '' WHERE entry = 63508;
DELETE FROM spell_bonus_data WHERE entry = 123996;
INSERT INTO spell_bonus_data VALUE
(123996, 0.505, 0, 0, 0, 'Monk - Crackling Tiger Lightning');