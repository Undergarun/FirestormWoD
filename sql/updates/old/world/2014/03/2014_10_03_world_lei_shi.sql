UPDATE creature_template SET
minlevel = 93,
maxlevel = 93, 
rank = 3,
ScriptName = 'boss_lei_shi',
mechanic_immune_mask = 769638399,
flags_extra = 0x01,
faction_A = 14,
faction_H = 14
WHERE entry = 62983;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
ScriptName = 'mob_animated_protector',
faction_A = 14,
faction_H = 14,
mindmg = 5174,
maxdmg = 12574,
attackpower = 62114,
dmg_multiplier = 4,
baseattacktime = 2000
WHERE entry = 62995;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93, 
rank = 3,
ScriptName = 'mob_lei_shi_hidden',
mechanic_immune_mask = 769638399,
flags_extra = 0x01,
faction_A = 14,
faction_H = 14
WHERE entry = 63099;

DELETE FROM `creature` WHERE `id` = 62995 AND `map` = 996;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(62995, 996, 760, 1, -989.3629, -2902.622, 19.17827, 0.6423108, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1011.262, -2888.345, 19.70614, 4.302896, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1012.432, -2941.378, 19.43974, 3.005274, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1008.399, -2900.63, 19.70614, 4.643414, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1045.229, -2925.26, 19.17827, 1.960813, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -994.75, -2925.177, 19.17827, 4.801074, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1023.323, -2936.851, 19.43974, 3.936672, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1013.615, -2925.484, 19.70614, 1.112243, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1042.057, -2888.434, 19.17827, 1.974275, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1028.252, -2923.663, 19.70614, 0, 7200, 0, 0), -- 62995 (Area: -1) 
(62995, 996, 760, 1, -1044.986, -2901.198, 19.17827, 3.013757, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1038.372, -2934.913, 19.17827, 2.312799, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -992.842, -2888.924, 19.17827, 2.489937, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -998.684, -2934.783, 19.17827, 5.882029, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1023.389, -2888.509, 19.70614, 0.8620354, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1035.009, -2910.316, 19.70614, 3.573017, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1002.585, -2913.233, 19.70614, 4.014608, 7200, 0, 0), -- 62995 (Area: -1)
(62995, 996, 760, 1, -1027.068, -2899.241, 19.70614, 1.261874, 7200, 0, 0); -- 62995 (Area: -1)

DELETE FROM spell_target_position WHERE id = 123441;
INSERT INTO spell_target_position VALUE
(123441, 0, 996, -1017.93, -2911.3, 19.902, 4.74);

DELETE FROM spell_script_names WHERE spell_id IN (123461, 123244, 123233, 123705, 123712);
INSERT INTO spell_script_names VALUES
(123461, 'spell_get_away'),
(123244, 'spell_hide'),
(123233, 'spell_hide_stacks'),
(123705, 'spell_scary_fog_dot'),
(123712, 'spell_scary_fog_stacks');

UPDATE gameobject SET position_x = -1017.93, position_y = -2911.3, position_z = 19.902 WHERE id IN (214851, 214852);

DELETE FROM creature_text WHERE entry = 62983;
INSERT INTO creature_text VALUES
(62983, 0, 0, 'Wh-what are you doing here!? G-go away!', 14, 0, 100, 0, 0, 29328, 'LEI_SHI_AGGRO'),
(62983, 1, 0, 'I''m hiding until you leave!', 14, 0, 100, 0, 0, 29333, 'LEI_SHI_HIDE_01'),
(62983, 1, 1, 'I don''t want to see you anymore!', 14, 0, 100, 0, 0, 29334, 'LEI_SHI_HIDE_02'),
(62983, 2, 0, 'Stay away from me!', 14, 0, 100, 0, 0, 29332, 'LEI_SHI_GET_AWAY_01'),
(62983, 2, 1, 'Get AWAY!', 14, 0, 100, 0, 0, 29331, 'LEI_SHI_GET_AWAY_02'),
(62983, 3, 0, 'S-sorry!', 14, 0, 100, 0, 0, 29329, 'LEI_SHI_SLAY_01'),
(62983, 3, 1, 'It''s your fault it happened!', 14, 0, 100, 0, 0, 29330, 'LEI_SHI_SLAY_02'),
(62983, 4, 0, 'I... ah... oh! Did I...? Was I...? It was... so... cloudy.', 14, 0, 100, 0, 0, 29325, 'LEI_SHI_DEFEATED_01'),
(62983, 4, 1, 'I have to go now.', 14, 0, 100, 0, 0, 29326, 'LEI_SHI_DEFEATED_02'),
(62983, 4, 2, 'I have to make the water not cloudy anymore. Will... will you help?', 14, 0, 100, 0, 0, 29327, 'LEI_SHI_DEFEATED_03');

DELETE FROM locales_creature_text WHERE entry = 62983;
INSERT INTO locales_creature_text VALUES
(62983, 0, 0, '', 'Oh... euh... qu''est-ce que vous faites là ?! Partez !', '', '', '', '', '', '', '', ''),
(62983, 1, 0, '', 'Je me cache jusqu''à ce que vous partiez !', '', '', '', '', '', '', '', ''),
(62983, 1, 1, '', 'Je ne veux plus vous voir !', '', '', '', '', '', '', '', ''),
(62983, 2, 0, '', 'Ne vous approchez pas de moi !', '', '', '', '', '', '', '', ''),
(62983, 2, 1, '', 'Allez-vous-EN !', '', '', '', '', '', '', '', ''),
(62983, 3, 0, '', 'Oh... dé-désolée !', '', '', '', '', '', '', '', ''),
(62983, 3, 1, '', 'C''est votre faute si c''est arrivé !', '', '', '', '', '', '', '', ''),
(62983, 4, 0, '', 'Je… ah… oh ! J''ai… ? Tout était… si… embrouillé.', '', '', '', '', '', '', '', ''),
(62983, 4, 1, '', 'Je dois partir maintenant.', '', '', '', '', '', '', '', ''),
(62983, 4, 2, '', 'Je dois faire en sorte que l''eau ne soit plus trouble. Vous… vous m''aideriez ?', '', '', '', '', '', '', '', '');