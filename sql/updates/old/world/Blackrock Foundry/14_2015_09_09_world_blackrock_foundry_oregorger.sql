SET NAMES utf8;

DELETE FROM areatrigger_template WHERE spell_id IN (159121, 159518, 156186, 156388);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, flags, scale_x, scale_y, ScriptName) VALUES
(159121, 0, 6689, 2, 0x4000, 3, 3, 'areatrigger_foundry_acidback_puddle'),
(159518, 2, 6709, 2, 0x4004, 2.5, 2.5, ''),
(156186, 0, 6349, 2, 0x4000, 7, 7, 'areatrigger_foundry_retched_blackrock'),
(156388, 0, 6360, 2, 0x4000, 7, 7, 'areatrigger_foundry_explosive_shard');

DELETE FROM spell_script_names WHERE spell_id IN (159518, 156297, 156324, 156240, 155898, 163453, 163455);
INSERT INTO spell_script_names VALUES
(159518, 'spell_foundry_shattering_charge'),
(156297, 'spell_foundry_acid_torrent'),
(156324, 'spell_foundry_acid_torrent_aoe'),
(156240, 'spell_foundry_acid_torrent_searcher'),
(155898, 'spell_foundry_rolling_fury_aura'),
(163453, 'spell_foundry_harvest_volatile_blackrock'),
(163455, 'spell_foundry_throw_volatile_ore');

UPDATE creature_template SET modelid1 = 53766, modelid2 = 0, minlevel = 100, maxlevel = 100, faction = 14 WHERE entry = 77261;

SET @REF_OREGORGER = 77182;

DELETE FROM creature_text WHERE entry IN (@REF_OREGORGER);
INSERT INTO creature_text VALUES
(@REF_OREGORGER, 0, 0, '|Hunit:77182:Oregorger|hOregorger prepares to cast |cFFFF0000|Hspell:156879|h[Blackrock Barrage]|h|r.', 41, 0, 100, 0, 0, 0, 'BlackrockBarrage'),
(@REF_OREGORGER, 1, 0, 'Oregorger has grown hungry and wants to eat more blackrock ore!', 41, 0, 100, 0, 0, 0, 'Phase2'),
(@REF_OREGORGER, 2, 0, 'Oregorger is full of blackrock ore!', 41, 0, 100, 0, 0, 0, 'ReturnToPhase1');

DELETE FROM locales_creature_text WHERE entry IN (@REF_OREGORGER);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_OREGORGER, 0, 0,
    '|Hunit:77182:Oregorger|hMangeroc se prépare à lancer |cFFFF0000|Hspell:156879|h[Barrage rochenoire]|h|r.',
    '|Hunit:77182:Oregorger|hErzfresser bereitet sich darauf vor, |cFFFF0000|Hspell:156879|h[Schwarzfelssperrfeuer]|h|r zu wirken!',
    '|Hunit:77182:Oregorger|hZampamenas se prepara para lanzar |cFFFF0000|Hspell:156879|h[Tromba de Roca Negra]|h|r.',
    '|Hunit:77182:Oregorger|hРудожуй готовится применить |cFFFF0000|Hspell:156879|h[Шквал Черной горы]|h|r.'
),
(
    @REF_OREGORGER, 1, 0,
    'Mangeroc a faim et veut manger plus de minerai rochenoire !',
    'Erzfresser ist jetzt hungrig und will mehr Schwarzfelserz fressen!',
    '¡Zampamenas tiene hambre y quiere comer más mena de Roca Negra!',
    'Рудожуй проголодался и хочет еще руды Черной горы!'
),
(
    @REF_OREGORGER, 2, 0,
    'Mangeroc est rempli de minerai rochenoire !',
    'Erzfresser ist voller Schwarzfelserz!',
    '¡Zampamenas está lleno de mena de Roca Negra!',
    'Рудожуй наелся руды Черной горы!'
);

DELETE FROM creature_groupsizestats WHERE entry = @REF_OREGORGER;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
-- Normal
(@REF_OREGORGER,14,10,39001391),
(@REF_OREGORGER,14,11,43117825),
(@REF_OREGORGER,14,12,47202400),
(@REF_OREGORGER,14,13,51292485),
(@REF_OREGORGER,14,14,55382369),
(@REF_OREGORGER,14,15,59476000),
(@REF_OREGORGER,14,16,63593745),
(@REF_OREGORGER,14,17,67680445),
(@REF_OREGORGER,14,18,71765114),
(@REF_OREGORGER,14,19,75869003),
(@REF_OREGORGER,14,20,79956107),
(@REF_OREGORGER,14,21,84053664),
(@REF_OREGORGER,14,22,88140861),
(@REF_OREGORGER,14,23,92239703),
(@REF_OREGORGER,14,24,96337792),
(@REF_OREGORGER,14,25,100489912),
(@REF_OREGORGER,14,26,104530096),
(@REF_OREGORGER,14,27,108646554),
(@REF_OREGORGER,14,28,112725495),
(@REF_OREGORGER,14,29,116813903),
(@REF_OREGORGER,14,30,120935139),
-- Heroic
(@REF_OREGORGER,15,10,54165022),
(@REF_OREGORGER,15,11,59889024),
(@REF_OREGORGER,15,12,65568042),
(@REF_OREGORGER,15,13,71226495),
(@REF_OREGORGER,15,14,76956501),
(@REF_OREGORGER,15,15,82626135),
(@REF_OREGORGER,15,16,88318040),
(@REF_OREGORGER,15,17,94022105),
(@REF_OREGORGER,15,18,99664737),
(@REF_OREGORGER,15,19,105371910),
(@REF_OREGORGER,15,20,111039511),
(@REF_OREGORGER,15,21,116726722),
(@REF_OREGORGER,15,22,122424868),
(@REF_OREGORGER,15,23,128114515),
(@REF_OREGORGER,15,24,133814197),
(@REF_OREGORGER,15,25,139483496),
(@REF_OREGORGER,15,26,145186362),
(@REF_OREGORGER,15,27,150858264),
(@REF_OREGORGER,15,28,156544656),
(@REF_OREGORGER,15,29,162224207),
(@REF_OREGORGER,15,30,167996102),
-- LFR
(@REF_OREGORGER,17,18,50281633),
(@REF_OREGORGER,17,19,53113927),
(@REF_OREGORGER,17,20,56011537),
(@REF_OREGORGER,17,21,58844429),
(@REF_OREGORGER,17,22,61710392),
(@REF_OREGORGER,17,23,64567493),
(@REF_OREGORGER,17,24,67433747),
(@REF_OREGORGER,17,25,70299883);

DELETE FROM `reference_loot_template` WHERE entry = @REF_OREGORGER;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_OREGORGER, 113874, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113875, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113876, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113877, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113878, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113879, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113880, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113881, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113882, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113883, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 113884, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 119194, 0, 1, 1, 1, 1),
(@REF_OREGORGER, 119448, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_OREGORGER WHERE `entry`= @REF_OREGORGER;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_OREGORGER;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_OREGORGER, 1, 100, 1, 0, -@REF_OREGORGER, 6);