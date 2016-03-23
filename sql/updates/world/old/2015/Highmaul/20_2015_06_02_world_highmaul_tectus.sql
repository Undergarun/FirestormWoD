SET NAMES utf8;

DELETE FROM creature_text WHERE entry IN (78948, 80551, 80557, 80599, 80822);
INSERT INTO creature_text VALUES
(78948, 0, 0, 'ALL THAT LIVES, TO DUST.', 14, 0, 100, 0, 0, 43509, 'TectusAggro'),
(78948, 1, 0, 'TOO... MUCH!', 14, 0, 100, 0, 0, 43511, 'TectusShattered1'),
(78948, 1, 1, 'WHAT... IS... THIS...', 14, 0, 100, 0, 0, 43512, 'TectusShattered2'),
(78948, 2, 0, 'BREAK.', 14, 0, 100, 0, 0, 43517, 'TectusTectonicUpheaval1'),
(78948, 2, 1, 'SHATTER.', 14, 0, 100, 0, 0, 43520, 'TectusTectonicUpheaval2'),
(78948, 3, 0, 'I... AM... ETERNAL!', 14, 0, 100, 0, 0, 43519, 'TectusTectonicUpheavalCompleted'),
(78948, 4, 0, 'RISE, MOUNTAINS!', 14, 0, 100, 0, 0, 43521, 'EarthenPillar'),
(78948, 5, 0, 'FLESH. WEAK.', 14, 0, 100, 0, 0, 43516, 'TectusSlay1'),
(78948, 5, 1, 'RETURN TO THE EARTH.', 14, 0, 100, 0, 0, 43515, 'TectusSlay2'),
(78948, 6, 0, 'EVEN THE MOUNTAIN... FALLS...', 14, 0, 100, 0, 0, 43510, 'TectusDeath'),
(78948, 7, 0, 'Tectus directs his Crystalline Barrage towards you!', 41, 0, 100, 0, 0, 0, 'CrystallineBarrage'),

(80551, 0, 0, 'ALL THAT LIVES, TO DUST.', 14, 0, 100, 0, 0, 43509, 'TectusAggro'),
(80551, 1, 0, 'TOO... MUCH!', 14, 0, 100, 0, 0, 43511, 'TectusShattered1'),
(80551, 1, 1, 'WHAT... IS... THIS...', 14, 0, 100, 0, 0, 43512, 'TectusShattered2'),
(80551, 2, 0, 'BREAK.', 14, 0, 100, 0, 0, 43517, 'TectusTectonicUpheaval1'),
(80551, 2, 1, 'SHATTER.', 14, 0, 100, 0, 0, 43520, 'TectusTectonicUpheaval2'),
(80551, 3, 0, 'I... AM... ETERNAL!', 14, 0, 100, 0, 0, 43519, 'TectusTectonicUpheavalCompleted'),
(80551, 4, 0, 'RISE, MOUNTAINS!', 14, 0, 100, 0, 0, 43521, 'EarthenPillar'),
(80551, 5, 0, 'FLESH. WEAK.', 14, 0, 100, 0, 0, 43516, 'TectusSlay1'),
(80551, 5, 1, 'RETURN TO THE EARTH.', 14, 0, 100, 0, 0, 43515, 'TectusSlay2'),
(80551, 6, 0, 'EVEN THE MOUNTAIN... FALLS...', 14, 0, 100, 0, 0, 43510, 'TectusDeath'),
(80551, 7, 0, 'Tectus directs his Crystalline Barrage towards you!', 41, 0, 100, 0, 0, 0, 'CrystallineBarrage'),

(80557, 0, 0, 'ALL THAT LIVES, TO DUST.', 14, 0, 100, 0, 0, 43509, 'TectusAggro'),
(80557, 1, 0, 'TOO... MUCH!', 14, 0, 100, 0, 0, 43511, 'TectusShattered1'),
(80557, 1, 1, 'WHAT... IS... THIS...', 14, 0, 100, 0, 0, 43512, 'TectusShattered2'),
(80557, 2, 0, 'BREAK.', 14, 0, 100, 0, 0, 43517, 'TectusTectonicUpheaval1'),
(80557, 2, 1, 'SHATTER.', 14, 0, 100, 0, 0, 43520, 'TectusTectonicUpheaval2'),
(80557, 3, 0, 'I... AM... ETERNAL!', 14, 0, 100, 0, 0, 43519, 'TectusTectonicUpheavalCompleted'),
(80557, 4, 0, 'RISE, MOUNTAINS!', 14, 0, 100, 0, 0, 43521, 'EarthenPillar'),
(80557, 5, 0, 'FLESH. WEAK.', 14, 0, 100, 0, 0, 43516, 'TectusSlay1'),
(80557, 5, 1, 'RETURN TO THE EARTH.', 14, 0, 100, 0, 0, 43515, 'TectusSlay2'),
(80557, 6, 0, 'EVEN THE MOUNTAIN... FALLS...', 14, 0, 100, 0, 0, 43510, 'TectusDeath'),
(80557, 7, 0, 'Tectus directs his Crystalline Barrage towards you!', 41, 0, 100, 0, 0, 0, 'CrystallineBarrage'),

(80599, 0, 0, 'No! You do not understand... must not get away...', 14, 0, 100, 0, 0, 45418, 'Intro1'),
(80599, 0, 1, 'Yjj''rmr... Xzzolos...', 14, 0, 100, 0, 0, 45415, 'Intro2'),
(80599, 0, 2, 'Yes, Tectus. Bend to master''s will!', 14, 0, 100, 0, 0, 45417, 'Intro3'),

(80822, 0, 0, 'Kral''ach... The Darkness speaks... A VOICE!', 14, 0, 100, 0, 0, 45411, 'NightTwistedAggro1'),
(80822, 0, 1, 'MASTER! I COME FOR YOU!', 14, 0, 100, 0, 0, 45412, 'NightTwistedAggro2');

DELETE FROM locales_creature_text WHERE entry IN (78948, 80551, 80557, 80599, 80822);;
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    78948, 0, 0,
    'QUE TOUT CE QUI VIT DEVIENNE POUSSIÈRE.',
    'ALLES LEBEN, ZU STAUB.',
    'TODO LO QUE VIVE, SERÁ POLVO.',
    'ВСЕ ЖИВОЕ УМРЕТ.'
),
(
    78948, 1, 0,
    'TROP... DE PUISSANCE !',
    'ZU... VIEL!',
    '¡ES... DEMASIADO!',
    'СЛИШКОМ... МНОГО!'
),
(
    78948, 1, 1,
    'QUE... QU''EST-CE QUE...',
    'WAS... GESCHIEHT...',
    '¿QUÉ... ES... ESTO...?',
    'ЧТО... ЭТО...'
),
(
    78948, 2, 0,
    'FRACAS.',
    'BRECHT.',
    'DESTROZAR.',
    'СЛОМАЮ.'
),
(
    78948, 2, 1,
    'VOLEZ EN ÉCLATS.',
    'ZERSPLITTERT.',
    'TRIZAR.',
    'В ПРАХ.'
),
(
    78948, 3, 0,
    'JE... SUIS... ÉTERNEL !',
    'ICH... BIN... EWIG!',
    'YO... SOY... ¡ETERNO!',
    'Я... ВЕЧЕН!'
),
(
    78948, 4, 0,
    'MONTAGNES, ÉLEVEZ-VOUS !',
    'ERHEBT EUCH, BERGE!',
    'MONTAÑAS, ¡LEVÁNTENSE!',
    'ВОССТАНЬТЕ, ГОРЫ!'
),
(
    78948, 5, 0,
    'ÊTRE DE CHAIR. FAIBLE.',
    'FLEISCH. SCHWACH.',
    'CARNE. DÉBIL.',
    'ПЛОТЬ. СЛАБА.'
),
(
    78948, 5, 1,
    'RETOURNEZ À LA TERRE.',
    'ZURÜCK ZUR ERDE.',
    'VUELVE A LA TIERRA.',
    'ПРАХ К ПРАХУ.'
),
(
    78948, 6, 0,
    'MÊME LA PLUS HAUTE MONTAGNE... S''ÉCROULE...',
    'SELBST DER BERG... FÄLLT...',
    'AUN LAS MONTAÑAS... CAEN...',
    'ДАЖЕ ГОРЫ... НЕ ВЕЧНЫ...'
),
(
    78948, 7, 0,
    'Tectus dirige son Barrage cristalin vers vous !',
    'Tectus richtet seinen Kristallbeschuss auf Euch!',
    '¡Tectus dirige su Tromba cristalina hacia ti!',
    'Тектоник направляет на вас свой Кристаллический залп!'
),
(
    80551, 0, 0,
    'QUE TOUT CE QUI VIT DEVIENNE POUSSIÈRE.',
    'ALLES LEBEN, ZU STAUB.',
    'TODO LO QUE VIVE, SERÁ POLVO.',
    'ВСЕ ЖИВОЕ УМРЕТ.'
),
(
    80551, 1, 0,
    'TROP... DE PUISSANCE !',
    'ZU... VIEL!',
    '¡ES... DEMASIADO!',
    'СЛИШКОМ... МНОГО!'
),
(
    80551, 1, 1,
    'QUE... QU''EST-CE QUE...',
    'WAS... GESCHIEHT...',
    '¿QUÉ... ES... ESTO...?',
    'ЧТО... ЭТО...'
),
(
    80551, 2, 0,
    'FRACAS.',
    'BRECHT.',
    'DESTROZAR.',
    'СЛОМАЮ.'
),
(
    80551, 2, 1,
    'VOLEZ EN ÉCLATS.',
    'ZERSPLITTERT.',
    'TRIZAR.',
    'В ПРАХ.'
),
(
    80551, 3, 0,
    'JE... SUIS... ÉTERNEL !',
    'ICH... BIN... EWIG!',
    'YO... SOY... ¡ETERNO!',
    'Я... ВЕЧЕН!'
),
(
    80551, 4, 0,
    'MONTAGNES, ÉLEVEZ-VOUS !',
    'ERHEBT EUCH, BERGE!',
    'MONTAÑAS, ¡LEVÁNTENSE!',
    'ВОССТАНЬТЕ, ГОРЫ!'
),
(
    80551, 5, 0,
    'ÊTRE DE CHAIR. FAIBLE.',
    'FLEISCH. SCHWACH.',
    'CARNE. DÉBIL.',
    'ПЛОТЬ. СЛАБА.'
),
(
    80551, 5, 1,
    'RETOURNEZ À LA TERRE.',
    'ZURÜCK ZUR ERDE.',
    'VUELVE A LA TIERRA.',
    'ПРАХ К ПРАХУ.'
),
(
    80551, 6, 0,
    'MÊME LA PLUS HAUTE MONTAGNE... S''ÉCROULE...',
    'SELBST DER BERG... FÄLLT...',
    'AUN LAS MONTAÑAS... CAEN...',
    'ДАЖЕ ГОРЫ... НЕ ВЕЧНЫ...'
),
(
    80551, 7, 0,
    'Tectus dirige son Barrage cristalin vers vous !',
    'Tectus richtet seinen Kristallbeschuss auf Euch!',
    '¡Tectus dirige su Tromba cristalina hacia ti!',
    'Тектоник направляет на вас свой Кристаллический залп!'
),
(
    80557, 0, 0,
    'QUE TOUT CE QUI VIT DEVIENNE POUSSIÈRE.',
    'ALLES LEBEN, ZU STAUB.',
    'TODO LO QUE VIVE, SERÁ POLVO.',
    'ВСЕ ЖИВОЕ УМРЕТ.'
),
(
    80557, 1, 0,
    'TROP... DE PUISSANCE !',
    'ZU... VIEL!',
    '¡ES... DEMASIADO!',
    'СЛИШКОМ... МНОГО!'
),
(
    80557, 1, 1,
    'QUE... QU''EST-CE QUE...',
    'WAS... GESCHIEHT...',
    '¿QUÉ... ES... ESTO...?',
    'ЧТО... ЭТО...'
),
(
    80557, 2, 0,
    'FRACAS.',
    'BRECHT.',
    'DESTROZAR.',
    'СЛОМАЮ.'
),
(
    80557, 2, 1,
    'VOLEZ EN ÉCLATS.',
    'ZERSPLITTERT.',
    'TRIZAR.',
    'В ПРАХ.'
),
(
    80557, 3, 0,
    'JE... SUIS... ÉTERNEL !',
    'ICH... BIN... EWIG!',
    'YO... SOY... ¡ETERNO!',
    'Я... ВЕЧЕН!'
),
(
    80557, 4, 0,
    'MONTAGNES, ÉLEVEZ-VOUS !',
    'ERHEBT EUCH, BERGE!',
    'MONTAÑAS, ¡LEVÁNTENSE!',
    'ВОССТАНЬТЕ, ГОРЫ!'
),
(
    80557, 5, 0,
    'ÊTRE DE CHAIR. FAIBLE.',
    'FLEISCH. SCHWACH.',
    'CARNE. DÉBIL.',
    'ПЛОТЬ. СЛАБА.'
),
(
    80557, 5, 1,
    'RETOURNEZ À LA TERRE.',
    'ZURÜCK ZUR ERDE.',
    'VUELVE A LA TIERRA.',
    'ПРАХ К ПРАХУ.'
),
(
    80557, 6, 0,
    'MÊME LA PLUS HAUTE MONTAGNE... S''ÉCROULE...',
    'SELBST DER BERG... FÄLLT...',
    'AUN LAS MONTAÑAS... CAEN...',
    'ДАЖЕ ГОРЫ... НЕ ВЕЧНЫ...'
),
(
    80557, 7, 0,
    'Tectus dirige son Barrage cristalin vers vous !',
    'Tectus richtet seinen Kristallbeschuss auf Euch!',
    '¡Tectus dirige su Tromba cristalina hacia ti!',
    'Тектоник направляет на вас свой Кристаллический залп!'
),
(
    80599, 0, 0,
    'Vous ne comprenez pas ! Ce rituel ne peut pas...',
    'Ihr versteht nicht! Er darf nicht...!',
    '¡No lo entiendes! ¡Este no debe escapar!',
    'Вы не понимаете! Так не должно...'
),
(
    80599, 0, 1,
    'Yjj''rmr... Xzzolos...',
    'Yjj''rmr... Xzzolos...',
    'Yjj''rmr... Xzzolos...',
    'Ижжж''рмр... Хзззлолс...'
),
(
    80599, 0, 2,
    'Oui, Tectus. Soumets-toi... à la volonté... de notre maître...',
    'Ja, Tectus. Beug dich... dem Willen... des Meisters...',
    'Sí, Tectus. Responde... a la voluntad... de nuestro maestro...',
    'Тектоник... Склонись перед волей... хозяина'
),
(
    80822, 0, 0,
    'Kral''ach... Les ténèbres parlent... D''UNE VOIX !',
    'Kral''ach... die Finsternis spricht... EINE STIMME!',
    'Kral''ach... la oscuridad habla... ¡UNA VOZ!',
    'Крал''арх... Тьма взывает... Голос!'
),
(
    80822, 0, 1,
    'MAÎTRE ! POUR VOUS SERVIR !',
    'MEISTER! EMPFANGT MICH!',
    '¡MAESTRO! ¡VENGO POR TI!',
    'ХОЗЯИН! Я ИДУ К ТЕБЕ!'
);

UPDATE creature_template SET ScriptName = 'npc_highmaul_night_twisted_ritualist' WHERE entry = 85245;
UPDATE creature_template SET ScriptName = 'npc_highmaul_greater_void_aberration' WHERE entry = 85246;
UPDATE creature_template SET ScriptName = 'npc_highmaul_night_twisted_supplicant' WHERE entry = 86185;
UPDATE creature_template SET ScriptName = 'npc_highmaul_rokka_and_lokk' WHERE entry IN (86071, 86073);
UPDATE creature_template SET ScriptName = 'npc_highmaul_oro' WHERE entry = 86072;

UPDATE creature_template SET unit_class = 4, dmg_multiplier = 20, ScriptName = 'boss_tectus', mechanic_immune_mask = 617299839, flags_extra = (0x01 | 0x00200000) WHERE entry = 78948;
UPDATE creature_template SET unit_class = 4, dmg_multiplier = 8, ScriptName = 'boss_tectus', mechanic_immune_mask = 617299839, flags_extra = 0x00200000 WHERE entry IN (80551, 80557);
UPDATE creature_template SET ScriptName = 'npc_highmaul_earthen_pillar_stalker' WHERE entry = 80476;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_highmaul_night_twisted_berserker', mechanic_immune_mask = 608907135 WHERE entry = 80822;
UPDATE creature_template SET dmg_multiplier = 5, ScriptName = 'npc_highmaul_night_twisted_earthwarper', mechanic_immune_mask = 608907135 WHERE entry = 80599;

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_meteoric_earthspire', 'spell_highmaul_stonebolt_volley', 'spell_highmaul_tectus_energy_gain', 'spell_highmaul_earthen_flechettes');
DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_earthen_pillar_timer', 'spell_highmaul_accretion', 'spell_highmaul_tectonic_upheaval', 'spell_highmaul_spawn_dust_cloud');
DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_petrification', 'spell_highmaul_raving_assault');
INSERT INTO spell_script_names VALUE
(172073, 'spell_highmaul_meteoric_earthspire'),
(172058, 'spell_highmaul_stonebolt_volley'),
(162287, 'spell_highmaul_tectus_energy_gain'),
(162658, 'spell_highmaul_tectus_energy_gain'),
(162674, 'spell_highmaul_tectus_energy_gain'),
(166024, 'spell_highmaul_earthen_pillar_timer'),
(162288, 'spell_highmaul_accretion'),
(162475, 'spell_highmaul_tectonic_upheaval'),
(169949, 'spell_highmaul_spawn_dust_cloud'),
(162968, 'spell_highmaul_earthen_flechettes'),
(162892, 'spell_highmaul_petrification'),
(163312, 'spell_highmaul_raving_assault');

DELETE FROM spell_proc_event WHERE entry IN (162288, 162892);
INSERT INTO spell_proc_event (entry, procFlags) VALUE
(162288, 0x800AAAA8),
(162892, 0x28);

DELETE FROM areatrigger_template WHERE spell_id IN (162371, 162894);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, TYPE, scale_x, scale_y, flags, data0, data1, data2, data3, data4, data5, ScriptName) VALUES
(162371, 0, 6957, 1, 1, 1, 2048, 2, 1.236, 1.5, 2, 1.236, 1.5, 'areatrigger_highmaul_crystalline_barrage'),
(162894, 1, 2414, 1, 1, 1, 4096, 0, 0, 0, 0, 0, 0, 'areatrigger_highmaul_gift_of_earth');

SET @REF_TECTUS_NHM = 78948;

DELETE FROM `reference_loot_template` WHERE entry = @REF_TECTUS_NHM;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_TECTUS_NHM, 113639, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113640, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113641, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113642, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113643, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113644, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113645, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113646, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113647, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113648, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113649, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113650, 0, 1, 1, 1, 1),
(@REF_TECTUS_NHM, 113651, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_TECTUS_NHM WHERE `entry`= @REF_TECTUS_NHM;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_TECTUS_NHM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_TECTUS_NHM, 1, 100, 1, 0, -@REF_TECTUS_NHM, 6);