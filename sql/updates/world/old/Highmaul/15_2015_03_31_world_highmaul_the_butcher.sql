DELETE FROM creature_text WHERE entry IN (77404);
INSERT INTO creature_text VALUES
(77404, 0, 0, 'Come here you scrawny grabbers!', 14, 0, 100, 0, 0, 41631, 'TheButcherIntro1'),
(77404, 1, 0, 'All crunch and no meat!', 14, 0, 100, 0, 0, 41632, 'TheButcherIntro2'),
(77404, 2, 0, 'Gonna make a Pale stew.', 14, 0, 100, 0, 0, 41633, 'TheButcherIntro3'),
(77404, 3, 0, 'Come and get it!', 14, 0, 100, 0, 0, 41626, 'TheButcherAggro'),
(77404, 4, 0, 'Get the blood flowing!', 14, 0, 100, 0, 0, 41637, 'TheButcherBoundingCleave'),
(77404, 5, 0, 'Time for the meat grinder!', 14, 0, 100, 0, 0, 41628, 'TheButcherFrenzy'),
(77404, 6, 0, 'Just a slice off the top.', 14, 0, 100, 0, 0, 41636, 'TheButcherCleave'),
(77404, 7, 0, 'I will break you!', 14, 0, 100, 0, 0, 41627, 'TheButcherBerserk'),
(77404, 8, 0, 'So juicy!', 14, 0, 100, 0, 0, 41630, 'TheButcherSlay1'),
(77404, 8, 1, 'Give it to us raw... and wiggling!', 14, 0, 100, 0, 0, 41629, 'TheButcherSlay2'),
(77404, 9, 0, 'Looks like meat''s back on the menu!', 14, 0, 100, 0, 0, 41639, 'TheButcherWipe'),
(77404, 10, 0, 'Lanky, gamey, little runts!', 14, 0, 100, 0, 0, 41639, ''),
(77404, 11, 0, 'Get off! You ain''t worth eatin''.', 14, 0, 100, 0, 0, 41639, ''),
(77404, 12, 0, 'Smash!', 14, 0, 100, 0, 0, 41639, '');

DELETE FROM locales_creature_text WHERE entry IN (77404);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    77404, 0, 0,
    'Venez donc, bande de casse-pieds maigrichons !',
    'Kommt her, ihr mickrigen Raffkes.',
    '¡Vengan acá, esqueléticos!',
    'Сюда, паршивцы!'
),
(
    77404, 1, 0,
    'Plein d’os et pas de viande.',
    'Nur Haut und Knochen.',
    'Todos crujientes y sin carne.',
    'Мяса нет, жилы одни.'
),
(
    77404, 2, 0,
    'Vous ferez un ragoût bien fade.',
    'Das gibt leckeren Fahleneintopf.',
    'Haré un estofado insulso.',
    'Бледное выйдет жаркое.'
),
(
    77404, 3, 0,
    'Venez par ici !',
    'Versucht es doch!',
    '¡Vengan!',
    'Подходи по одному!'
),
(
    77404, 4, 0,
    'Je vais vous saigner !',
    'Lasst das Blut fließen!',
    '¡Que la sangre corra!',
    'Выпустить всю кровь!'
),
(
    77404, 5, 0,
    'Il est temps de vous passer au hachoir !',
    'Zeit für den Fleischwolf!',
    '¡Hora de triturar la carne!',
    'Пускаю в дело мясорубку!'
),
(
    77404, 6, 0,
    'Juste une petite tranche !',
    'Nur eine Scheibe oben abschneiden!',
    '¡Solo una rebanada de arriba!',
    'Я только макушку срежу!'
),
(
    77404, 7, 0,
    'Je briserai tous vos os !',
    'ICH WERDE EUCH BRECHEN!',
    '¡LOS APLASTARÉ!',
    'Я ВАС ЗАЛОМАЮ!'
),
(
    77404, 8, 0,
    'De la viande juteuse !',
    'So saftig!',
    '¡Qué jugoso!',
    'Сочно!'
),
(
    77404, 8, 1,
    'Je les préfère crus et frétillants !',
    'Gib ihn uns roh und zappelnd!',
    '¡Dánoslo, crudo y retorciéndose!',
    'Дай его нам живого, трепыхающегося!'
),
(
    77404, 9, 0,
    'Il y a à nouveau de la viande au menu !',
    'Ab sofort steht wieder Fleisch auf dem Speiseplan!',
    '¡Parece que de nuevo hay carne en el menú!',
    'Мясо снова в меню!'
),
(
    77404, 10, 0,
    'Avortons rachitiques et puants !',
    'Schlaksige, zähe Kümmerlinge...',
    'Desagradables insignificantes...',
    'Тощая, вонючая мелюзга...'
),
(
    77404, 11, 0,
    'Dégagez ! Vous ne valez même pas d''être mangés !',
    'Runter mit euch! Ihr seid kaum eine Mahlzeit wert!',
    '¡Basta! ¡No vale la pena comerlos!',
    'Пшли вон! В вас есть нечего!'
),
(
    77404, 12, 0,
    'Je vous écraserai !',
    'Zerschmettern!',
    '¡Machaaaaque!',
    'Шмя-а-а-ак!'
);

DELETE FROM spell_script_names WHERE spell_id IN (151990, 152024, 156135, 156138, 156197, 156152, 166225, 174465, 174474);
INSERT INTO spell_script_names VALUES
(151990, 'spell_highmaul_chain_grip'),
(152024, 'spell_highmaul_chain_grip_aura'),
(156135, 'spell_highmaul_heavy_handed'),
(156138, 'spell_highmaul_heavy_handed_proc'),
(156197, 'spell_highmaul_bounding_cleave_dummy'),
(156152, 'spell_highmaul_gushing_wounds'),
(166225, 'spell_highmaul_boars_rush'),
(174465, 'spell_highmaul_unstoppable_charge'),
(174474, 'spell_highmaul_corrupted_blood_shield');

UPDATE creature_template SET ScriptName = 'npc_highmaul_gorian_guardsman' WHERE entry = 81270;
UPDATE creature_template SET ScriptName = 'npc_highmaul_night_twisted_devout' WHERE entry = 82698;
UPDATE creature_template SET ScriptName = 'npc_highmaul_gorian_runemaster' WHERE entry = 81272;
UPDATE creature_template SET ScriptName = 'npc_highmaul_gorian_enforcer' WHERE entry IN (82900, 86275);
UPDATE creature_template SET ScriptName = 'npc_highmaul_underbelly_vagrant' WHERE entry = 86290;
UPDATE creature_template SET ScriptName = 'npc_highmaul_gorian_sorcerer' WHERE entry = 85225;
UPDATE creature_template SET ScriptName = 'npc_highmaul_night_twisted_brute' WHERE entry = 85241;
UPDATE creature_template SET ScriptName = 'npc_highmaul_night_twisted_soothsayer' WHERE entry = 85240;
UPDATE creature_template SET ScriptName = 'npc_highmaul_void_aberration' WHERE entry = 85243;
UPDATE creature_template SET ScriptName = 'npc_highmaul_krush' WHERE entry = 82532;
UPDATE creature_template SET ScriptName = 'npc_highmaul_iron_flame_technician' WHERE entry = 86607;
UPDATE creature_template SET ScriptName = 'npc_highmaul_iron_warmaster' WHERE entry = 86609;
UPDATE creature_template SET ScriptName = 'npc_highmaul_iron_blood_mage' WHERE entry = 87229;
UPDATE creature_template SET faction = 14, ScriptName = 'npc_highmaul_night_twisted_cadaver' WHERE entry = 82505;

DELETE FROM areatrigger_template WHERE spell_id IN (175648, 163042);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUES
(175648, 0, 3417, 1, 1, 16384, 'areatrigger_highmaul_rune_of_disintegration'),
(163042, 0, 2426, 1, 1, 16384, 'areatrigger_highmaul_pale_vitriol');

UPDATE creature_template SET minlevel = 103, maxlevel = 103, unit_flags = unit_flags & ~0x00200000, unit_class = 4, dmg_multiplier = 10, ScriptName = 'boss_the_butcher', mechanic_immune_mask = 617299839, flags_extra = (0x01 | 0x00200000), Health_mod = 1100 WHERE entry = 77404;

DELETE FROM access_requirement WHERE mapId = 1228;
INSERT INTO access_requirement (mapId, difficulty, level_min, level_max, itemlevel_min, itemlevel_max, COMMENT) VALUES
(1228, 14, 100, 100, 615, 0, 'Highmaul - Normal'),
(1228, 15, 100, 100, 615, 0, 'Highmaul - Heroic'),
(1228, 16, 101, 101, 615, 0, 'Highmaul - Mythic'),
(1228, 17, 100, 100, 615, 0, 'Highmaul - Raid Tool');

SET @REF_BUTCHER_N = 77404;
SET @REF_BUTCHER_LFR = 1777404;

DELETE FROM `reference_loot_template` WHERE entry = @REF_BUTCHER_N;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BUTCHER_N, 113606, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113607, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113608, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113609, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113610, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113611, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113612, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113632, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113633, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113634, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113636, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113637, 0, 1, 1, 1, 1),
(@REF_BUTCHER_N, 113638, 0, 1, 1, 1, 1);

DELETE FROM `reference_loot_template` WHERE entry = @REF_BUTCHER_LFR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BUTCHER_LFR, 116026, 0, 1, 1, 1, 1),
(@REF_BUTCHER_LFR, 116290, 0, 1, 1, 1, 1),
(@REF_BUTCHER_LFR, 116209, 0, 1, 1, 1, 1),
(@REF_BUTCHER_LFR, 115998, 0, 1, 1, 1, 1),
(@REF_BUTCHER_LFR, 116230, 0, 1, 1, 1, 1),
(@REF_BUTCHER_LFR, 116297, 0, 1, 1, 1, 1),
(@REF_BUTCHER_LFR, 116281, 0, 1, 1, 1, 1),
(@REF_BUTCHER_LFR, 116361, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_BUTCHER_N WHERE `entry`= @REF_BUTCHER_N;
UPDATE `creature_template` SET `lootid`= @REF_BUTCHER_LFR WHERE `entry`= @REF_BUTCHER_LFR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_BUTCHER_N;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BUTCHER_N, 1, 100, 1, 0, -@REF_BUTCHER_N, 6);

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_BUTCHER_LFR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BUTCHER_LFR, 1, 100, 1, 0, -@REF_BUTCHER_LFR, 6);

UPDATE creature_classlevelstats SET basehp0 = 108328, basehp1 = 108328, basehp2 = 108328, basehp3 = 108328, basehp4 = 108328, basehp5 = 108328 WHERE level = 103 AND class IN (4, 8);