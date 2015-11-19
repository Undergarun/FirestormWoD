DELETE FROM areatrigger_template WHERE spell_id = 160259;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUES
(160259, 0, 6742, 12, 12, 16384, 'areatrigger_foundry_fire_bomb');

DELETE FROM spell_script_names WHERE spell_id = 160092;
INSERT INTO spell_script_names VALUE
(160092, 'spell_foundry_rending_slash');

DELETE FROM areatrigger_scripts WHERE entry = 9998;
INSERT INTO areatrigger_scripts VALUE
(9998, 'areatrigger_at_foundry_hansgar_and_franzok_entrance');

SET @REF_HANSGAR = 76973;
SET @REF_FRANZOK = 76974;

DELETE FROM creature_text WHERE entry IN (@REF_HANSGAR, @REF_FRANZOK);
INSERT INTO creature_text VALUES
(@REF_HANSGAR, 0, 0, 'Hey Franz? Franzok! We got a jam!', 14, 0, 100, 0, 0, 0, 'HansgarIntro1'),
(@REF_HANSGAR, 1, 0, 'I ain''t un-jammin it! I un-jammed it last time!', 14, 0, 100, 0, 0, 0, 'HansgarIntro2'),
(@REF_HANSGAR, 2, 0, 'Ey you! Get in dere and un-jam that stamper!', 14, 0, 100, 0, 0, 0, 'HansgarIntro3'),

(@REF_FRANZOK, 0, 0, 'So? Go un-jam it den!', 14, 0, 100, 0, 0, 0, 'FranzokIntro1'),
(@REF_FRANZOK, 1, 0, 'You wanna tell Blackhand we got a stamper that ain''t stampin''?', 14, 0, 100, 0, 0, 0, 'FranzokIntro2'),
(@REF_FRANZOK, 2, 0, 'Ha ha ha! Guess dis batch will have some impurities.', 14, 0, 100, 0, 0, 0, 'FranzokIntro3');

DELETE FROM locales_creature_text WHERE entry IN (@REF_HANSGAR, @REF_FRANZOK);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_HANSGAR, 0, 0,
    '',
    '',
    '',
    ''
),
(
    @REF_HANSGAR, 1, 0,
    '',
    '',
    '',
    ''
),
(
    @REF_HANSGAR, 2, 0,
    '',
    '',
    '',
    ''
),
(
    @REF_FRANZOK, 0, 0,
    '',
    '',
    '',
    ''
),
(
    @REF_FRANZOK, 1, 0,
    '',
    '',
    '',
    ''
),
(
    @REF_FRANZOK, 2, 0,
    '',
    '',
    '',
    ''
);