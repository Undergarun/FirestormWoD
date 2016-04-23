SET @REF_BRACKENSPORE_N = 78491;
SET @REF_BRACKENSPORE_LFR = 1778491;

DELETE FROM `reference_loot_template` WHERE entry = @REF_BRACKENSPORE_N;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_N, 113652, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113653, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113654, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113655, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113656, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113657, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113658, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113659, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113660, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113661, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113662, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113664, 0, 1, 1, 1, 1);

DELETE FROM `reference_loot_template` WHERE entry = @REF_BRACKENSPORE_LFR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_LFR, 115999, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116233, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116294, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116028, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116208, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116291, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116288, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_BRACKENSPORE_N WHERE `entry`= @REF_BRACKENSPORE_N;
UPDATE `creature_template` SET `lootid`= @REF_BRACKENSPORE_LFR WHERE `entry`= @REF_BRACKENSPORE_LFR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_BRACKENSPORE_N;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_N, 1, 100, 1, 0, -@REF_BRACKENSPORE_N, 6);

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_BRACKENSPORE_LFR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_LFR, 1, 100, 1, 0, -@REF_BRACKENSPORE_LFR, 6);

DELETE FROM creature_text WHERE entry IN (86609, 78491, 79092);
INSERT INTO creature_text VALUES
(86609, 0, 0, 'Argh! These flamethrowers are useless, it''s eating us alive!', 14, 0, 100, 0, 0, 46607, 'BrackensporeIntro1'),
(86609, 1, 0, 'Fall back! Drop your weapons and retreat to the ship!', 14, 0, 100, 0, 0, 46608, 'BrackensporeIntro2'),

(78491, 0, 0, '%s begins to channel |cFFFF0404|Hspell:159996|h[Infesting Spores]|h|r!', 41, 0, 100, 0, 0, 0, 'InfestingSporesWarning'),

(79092, 0, 0, 'A |cFFFF0404%s|r comes lurking from the depths!', 41, 0, 100, 0, 0, 0, 'FungalEaterWarning');

DELETE FROM locales_creature_text WHERE entry IN (86609, 78491, 79092);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    86609, 0, 0,
    'Rhah ! Ces lance-flammes ne servent à rien, on se fait massacrer !',
    'Argh! Diese Flammenwerfer sind nutzlos. Wir werden verschlungen!',
    '¡Argh! ¡Estos lanzallamas son inútiles! ¡Nos está comiendo vivos!',
    'Р-р-р! Эти огнеметы бесполезны. Нас сейчас съедят!'
),
(
    86609, 1, 0,
    'Repliez-vous ! Jetez vos armes et remontez à bord !',
    'Rückzug! Lasst die Waffen fallen und flüchtet auf das Schiff!',
    '¡Atrás! ¡Suelten sus armas y retírense a la nave!',
    'Отступаем! Бросайте оружие и на корабль!.'
),
(
    78491, 0, 0,
    '%s commence à canaliser |cFFFF0404|Hspell:159996|h[Spores infestantes]|h|r !',
    '%s beginnt, |cFFFF0404|Hspell:159996|h[Sporenbefall]|h|r zu kanalisieren!',
    '¡%s comienza a canalizar |cFFFF0404|Hspell:159996|h[Esporas infectantes]|h|r!',
    '%s начинает поддерживать заклинание |cFFFF0404|Hspell:159996|h["Заражение спорами"]|h|r!'
),
(
    79092, 0, 0,
    'Un |cFFFF0404%s|r surgit des profondeurs !',
    'Ein |cFFFF0404%s|r springt aus den Tiefen hervor!',
    '¡Un |cFFFF0404%s|r se acerca desde las profundidades!',
    'Из темных глубин показывается |cFFFF0404%s|r!'
);

UPDATE creature_template SET
minlevel = 103,
maxlevel = 103,
faction = 14,
unit_class = 1,
dmg_multiplier = 10,
ScriptName = 'boss_brackenspore',
mechanic_immune_mask = 617299839,
flags_extra = (0x01 | 0x00200000),
Health_mod = 1265
WHERE entry = 78491;

UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = 'npc_highmaul_mind_fungus' WHERE entry IN (79082, 86611);
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry = 59481;
UPDATE creature_template SET ScriptName = 'npc_highmaul_spore_shooter' WHERE entry IN (79183, 86612);
UPDATE creature_template SET ScriptName = 'npc_highmaul_fungal_flesh_eater' WHERE entry IN (79092);
UPDATE creature_template SET ScriptName = 'npc_highmaul_living_mushroom' WHERE entry IN (78884);
UPDATE creature_template SET ScriptName = 'npc_highmaul_rejuvenating_mushroom' WHERE entry IN (78868);
UPDATE creature_template SET ScriptName = 'npc_highmaul_bfc9000' WHERE entry IN (81403);

DELETE FROM spell_script_names WHERE spell_id IN (159220, 163667, 163322, 163666, 163347, 163310, 165223, 164248, 173244, 163663);
INSERT INTO spell_script_names VALUES
(159220, "spell_highmaul_necrotic_breath"),
(163667, "spell_highmaul_flamethrower_regen"),
(163322, "spell_highmaul_flamethrower_aura"),
(163666, "spell_highmaul_pulsing_heat"),
(163347, "spell_highmaul_creeping_moss"),
(163310, "spell_highmaul_flamethrower"),
(165223, "spell_highmaul_burning_infusion"),
(164248, "spell_highmaul_energy_regen"),
(173244, "spell_highmaul_spore_shot"),
(163663, "spell_highmaul_flamethrower_overrider");

DELETE FROM areatrigger_scripts WHERE entry IN (10094);
INSERT INTO areatrigger_scripts VALUE
(10094, 'areatrigger_at_highmaul_infested_waters');

DELETE FROM areatrigger_template WHERE spell_id IN (159489, 173241, 173229, 160413);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, scale_curve_id, morph_curve_id, data0, ScriptName) VALUES
(159489, 0, 6733, 15, 15, 16388, 0, 0, 0, 'areatrigger_highmaul_mind_fungus'),
(173241, 1, 7882, 3, 3, 16586, 1288, 1192, 3000, 'areatrigger_highmaul_spore_shot'),
(173229, 0, 7881, 3, 3, 16384, 0, 0, 0, 'areatrigger_highmaul_creeping_moss'),
(160413, 0, 2217, 1, 1, 16384, 0, 0, 0, 'areatrigger_highmaul_call_of_the_tides');