SET NAMES utf8;

DELETE FROM creature_text WHERE entry IN (79015);
INSERT INTO creature_text VALUES
(79015, 0, 0, 'You dare enter these halls?', 14, 0, 100, 0, 0, 41977, 'KoraghIntro'),
(79015, 1, 0, 'Your magic is powerless here!', 14, 0, 100, 0, 0, 41970, 'KoraghAggro'),
(79015, 2, 0, 'YOU WILL PAY FOR THIS!', 14, 0, 100, 0, 0, 41974, 'KoraghBarrierShattered1'),
(79015, 2, 1, 'AHHHHHH!', 14, 0, 100, 0, 0, 41973, 'KoraghBarrierShattered2'),
(79015, 3, 0, 'Your attacks are weak!', 14, 0, 100, 0, 0, 41985, 'KoraghExpelMagic1'),
(79015, 3, 1, 'I am unstoppable.', 14, 0, 100, 0, 0, 41988, 'KoraghExpelMagic2'),
(79015, 3, 2, 'Broken.', 14, 0, 100, 0, 0, 41983, 'KoraghExpelMagic3'),
(79015, 3, 3, 'Get out!', 14, 0, 100, 0, 0, 41989, 'KoraghExpelMagic4'),
(79015, 4, 0, 'I will tear you in half!', 14, 0, 100, 0, 0, 41987, 'KoraghSuppressionField1'),
(79015, 4, 1, 'I will crush you!', 14, 0, 100, 0, 0, 41986, 'KoraghSuppressionField2'),
(79015, 4, 2, 'Silence!', 14, 0, 100, 0, 0, 41981, 'KoraghSuppressionField3'),
(79015, 4, 3, 'Quiet!', 14, 0, 100, 0, 0, 41982, 'KoraghSuppressionField4'),
(79015, 5, 0, 'I am in control here!', 14, 0, 100, 0, 0, 41968, 'KoraghDominatingPower1'),
(79015, 5, 1, 'We control the flow of power here!', 14, 0, 100, 0, 0, 41969, 'KoraghDominatingPower2'),
(79015, 6, 0, 'I am finished with you!', 14, 0, 100, 0, 0, 41971, 'KoraghBerserker'),
(79015, 7, 0, 'Worthless!', 14, 0, 100, 0, 0, 41978, 'KoraghSlay1'),
(79015, 7, 1, 'Weak!', 14, 0, 100, 0, 0, 41979, 'KoraghSlay2'),
(79015, 7, 2, 'Your efforts were in vain!', 14, 0, 100, 0, 0, 41980, 'KoraghSlay3'),
(79015, 8, 0, 'Impossible...', 14, 0, 100, 0, 0, 41972, 'KoraghDeath'),
(79015, 9, 0, '|TInterface\\Icons\\INV_Fabric_Felcloth_Ebon.blp:20|t |cFFFF0404|Hspell:161242|h[Caustic Energy]|h|r begins to flow into $n.', 41, 0, 100, 0, 0, 0, 'KoraghCausticEnergyWarn'),
(79015, 10, 0, '|TInterface\\Icons\\spell_fire_felfirenova.blp:20|t You are afflicted with |cFFFF0404|Hspell:172895|h[Expel Magic: Fel]|h|r!', 41, 0, 100, 0, 0, 0, 'KoraghExpelMagicFelWarn'),
(79015, 11, 0, '|TInterface\\Icons\\Spell_Arcane_Blast.blp:20|t You are afflicted with |cFFFF0404|Hspell:162186|h[Expel Magic: Arcane]|h|r!', 41, 0, 100, 0, 0, 0, 'KoraghExpelMagicArcaneWarn');

DELETE FROM locales_creature_text WHERE entry IN (79015);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    79015, 0, 0,
    'Vous osez pénétrer en ces lieux ?',
    'Ihr wagt es, diese Hallen zu betreten?',
    '¿Osan entrar a estos salones? Los Rompevileza nunca han caído en batalla.',
    'Кто посмел войти в эти залы? Скверноломов никому не победить!'
),
(
    79015, 1, 0,
    'Votre magie est impuissante ici.',
    'Eure Magie ist hier machtlos.',
    'Su magia es inútil aquí.',
    'Ваша магия здесь бессильна.'
),
(
    79015, 2, 0,
    'VOUS PAIEREZ POUR ÇA !!',
    'Dafür werdet Ihr ZAHLEN!',
    '¡¡¡Van a PAGAR POR ESTO!!!',
    'Вы заплатите за это!'
),
(
    79015, 2, 1,
    'AHHHHHH !',
    'AHHHHH!',
    '¡AHHHHHH!',
    'А-а-а-а-а!'
),
(
    79015, 3, 0,
    'Vos attaques sont faibles !',
    'Eure Angriffe sind jämmerlich.',
    '¡Sus ataques son débiles!',
    'Какие же вы слабые!'
),
(
    79015, 3, 1,
    'Rien ne peut m''arrêter.',
    'Nichts kann mich aufhalten.',
    'Soy imparable.',
    'Меня не остановить.'
),
(
    79015, 3, 2,
    'Je vais vous briser.',
    'Zerbrochen.',
    'Roto.',
    'Сломаю.'
),
(
    79015, 3, 3,
    'Sortez !',
    'Verschwindet!',
    '¡Largo!',
    'Прочь!'
),
(
    79015, 4, 0,
    'Je vous réduirai en pièces !',
    'Ich reiße Euch in Stücke!',
    '¡Los partiré por la mitad!',
    'Я разорву вас на части!'
),
(
    79015, 4, 1,
    'Je vous écraserai !',
    'Ich werde Euch zermalmen!',
    '¡Los aplastaré!',
    'Я сокрушу вас!'
),
(
    79015, 4, 2,
    'Silence !',
    'Schweigt!',
    '¡Silencio!',
    'Молчать!'
),
(
    79015, 4, 3,
    'Taisez-vous !',
    'Ruhe!',
    '¡Calla!',
    'Тихо!'
),
(
    79015, 5, 0,
    'C''est moi qui contrôle tout, ici.',
    'Hier habe ich das Sagen.',
    'Yo soy quien manda aquí.',
    'Здесь повелеваю я.'
),
(
    79015, 5, 1,
    'Nous contrôlons le flux de pouvoir.',
    'Wir kontrollieren hier die Energien!',
    'Aquí controlamos el flujo del poder.',
    'Здесь мы управляем силой.'
),
(
    79015, 6, 0,
    'J''en ai fini avec vous.',
    'Ich habe genug von Euch!',
    'Ya me harté de ustedes.',
    'Я покончу с вами.'
),
(
    79015, 7, 0,
    'Futile.',
    'Wertlos.',
    'Despreciable.',
    'Ничтожество.'
),
(
    79015, 7, 1,
    'Faible.',
    'Schwach.',
    'Débil.',
    'Ничтожество.'
),
(
    79015, 7, 2,
    'Vos efforts sont vains !',
    'Ihr bemüht Euch vergebens!',
    '¡Tus esfuerzos fueron en vano!',
    'Не стоило и пытаться!'
),
(
    79015, 8, 0,
    'Impossible...',
    'Unmöglich...',
    'Imposible...',
    'Не может быть...'
),
(
    79015, 9, 0,
    '|TInterface\\Icons\\INV_Fabric_Felcloth_Ebon.blp:20|t L''|cFFFF0404|Hspell:161242|h[Énergie caustique]|h|r commence à se déverser dans $n.',
    '|TInterface\\Icons\\INV_Fabric_Felcloth_Ebon.blp:20|t |cFFFF0404|Hspell:161242|h[Ätzende Energie]|h|r beginnt, in $n hineinzufließen.',
    '|TInterface\\Icons\\INV_Fabric_Felcloth_Ebon.blp:20|t |cFFFF0404|Hspell:161242|h[Energía cáustica]|h|r comienza a fluir en $n.',
    '|TInterface\\Icons\\INV_Fabric_Felcloth_Ebon.blp:20|t |cFFFF0404|Hspell:161242|h[Жгучая энергия]|h|r перетекает в |3-1($n).'
),
(
    79015, 10, 0,
    '|TInterface\\Icons\\spell_fire_felfirenova.blp:20|t Vous subissez |cFFFF0404|Hspell:172895|h[Projection de magie : Gangrène]|h|r !',
    '|TInterface\\Icons\\spell_fire_felfirenova.blp:20|t Ihr seid von |cFFFF0404|Hspell:172895|h[Magie ausstoßen: Teufelsenergie]|h|r betroffen!',
    '|TInterface\\Icons\\spell_fire_felfirenova.blp:20|t¡Te ha afectado |cFFFF0404|Hspell:172895|h[Expulsar magia: vil]|h|r!',
    '|TInterface\\Icons\\spell_fire_felfirenova.blp:20|t На вас действует |cFFFF0404|Hspell:172895|h[Исторжение магии: Скверна]|h|r!'
),
(
    79015, 11, 0,
    '|TInterface\\Icons\\Spell_Arcane_Blast.blp:20|t Vous subissez |cFFFF0404|Hspell:162186|h[Projection de magie : Arcanes]|h|r !',
    '|TInterface\\Icons\\Spell_Arcane_Blast.blp:20|t Ihr seid von |cFFFF0404|Hspell:162186|h[Magie ausstoßen: Arkan]|h|r betroffen.',
    '|TInterface\\Icons\\Spell_Arcane_Blast.blp:20|t ¡Estás afectado por |cFFFF0404|Hspell:162186|h[Expulsar magia: Arcana]|h|r!',
    '|TInterface\\Icons\\Spell_Arcane_Blast.blp:20|t На вас действует |cFFFF0404|Hspell:162186|h[Исторжение магии: тайная магия]|h|r!'
);

DELETE FROM creature_equip_template WHERE entry IN (79015, 86326, 86329, 86330);
INSERT INTO creature_equip_template VALUES
(79015, 1, 116377, 0, 0),
(86326, 1, 77405, 0, 0),
(86329, 1, 77404, 0, 0),
(86330, 1, 77406, 0, 0);

UPDATE creature_template SET
minlevel = 102,
maxlevel = 102,
faction = 16,
rank = 1,
dmg_multiplier = 5
WHERE entry IN (86326, 86329, 86330);

UPDATE creature_template SET ScriptName = 'npc_highmaul_breaker_of_fel' WHERE entry = 86330;
UPDATE creature_template SET ScriptName = 'npc_highmaul_breaker_of_fire' WHERE entry = 86329;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, faction = 16, ScriptName = 'npc_highmaul_wild_flames' WHERE entry = 86875;
UPDATE creature_template SET ScriptName = 'npc_highmaul_breaker_of_frost' WHERE entry = 86326;

UPDATE creature_template SET
minlevel = 103,
maxlevel = 103,
faction = 16,
rank = 3,
dmg_multiplier = 10,
baseattacktime = 1500,
VehicleId = 2164,
ScriptName = 'boss_koragh',
mechanic_immune_mask = 617299839,
flags_extra = (0x01 | 0x00200000)
WHERE entry = 79015;

UPDATE creature_template SET
minlevel = 102,
maxlevel = 102,
faction = 16,
unit_class = 2,
type_flags = 2097224,
type_flags2 = 128,
ScriptName = 'npc_highmaul_koragh_volatile_anomaly'
WHERE entry = 79956;

UPDATE creature_model_info SET bounding_radius = 1.5, combat_reach = 9.5 WHERE modelid = 54825;

UPDATE gameobject_template SET ScriptName = 'go_highmaul_chain' WHERE entry = 233127;

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_frozen_core', 'spell_highmaul_wild_flames_areatrigger', 'spell_highmaul_nullification_barrier', 'spell_highmaul_caustic_energy');
DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_caustic_energy_dot', 'spell_highmaul_expel_magic_fire', 'spell_highmaul_expel_magic_arcane', 'spell_highmaul_nullification_barrier_player');
DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_expel_magic_frost_aura');
INSERT INTO spell_script_names VALUE
(174404, 'spell_highmaul_frozen_core'),
(173616, 'spell_highmaul_wild_flames_areatrigger'),
(156803, 'spell_highmaul_nullification_barrier'),
(160720, 'spell_highmaul_caustic_energy'),
(161242, 'spell_highmaul_caustic_energy_dot'),
(162185, 'spell_highmaul_expel_magic_fire'),
(162186, 'spell_highmaul_expel_magic_arcane'),
(163134, 'spell_highmaul_nullification_barrier_player'),
(172813, 'spell_highmaul_expel_magic_frost_aura');

DELETE FROM areatrigger_template WHERE spell_id IN (173824, 160720, 161330, 172747, 161574, 173048);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, scale_x, scale_y, flags, data0, data1, data2, data3, data4, data5, data6, data7, ScriptName) VALUES
(173824, 0, 7913, 3, 1, 1, 8192, 3, 3, 10, 10, 0.3, 0.3, 0, 0, 'areatrigger_highmaul_wild_flames'),
(160720, 0, 6784, 3, 1, 1, 8196, 9, 9, 10, 10, 0.3, 0.3, 0, 0, ''),
(161330, 0, 6838, 3, 1, 1, 8192, 8, 8, 4, 4, 0.3, 0.3, 0, 0, 'areatrigger_highmaul_suppression_field'),
(172747, 0, 7853, 3, 1, 1, 8192, 40, 40, 10, 10, 0.3, 0.3, 0, 0, 'areatrigger_highmaul_expel_magic_frost'),
(161574, 0, 6863, 3, 1, 1, 8192, 2, 2, 6, 6, 0.3, 0.3, 0, 0, 'areatrigger_highmaul_overflowing_energy'),
(173048, 0, 7863, 2, 1, 1, 16384, 1, 1, 0, 0, 0, 0, 0, 0, '');

DELETE FROM spell_target_position WHERE id = 161574;
INSERT INTO spell_target_position VALUE
(161574, 0, 1228, 3904.2, 8607.48, 397.853, 0);

SET @REF_KORAGH_NHM = 79015;

DELETE FROM `reference_loot_template` WHERE entry = @REF_KORAGH_NHM;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KORAGH_NHM, 113836, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113837, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113838, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113839, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113840, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113841, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113842, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113843, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113844, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113845, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113846, 0, 1, 1, 1, 1),
(@REF_KORAGH_NHM, 113847, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_KORAGH_NHM WHERE `entry`= @REF_KORAGH_NHM;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_KORAGH_NHM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KORAGH_NHM, 1, 100, 1, 0, -@REF_KORAGH_NHM, 6);