SET NAMES utf8;

UPDATE gameobject_template SET ScriptName = 'go_highmaul_portal' WHERE entry = 231776;

UPDATE creature_template SET ScriptName = 'npc_highmaul_highmaul_conscript' WHERE entry = 82519;
UPDATE creature_template SET ScriptName = 'npc_highmaul_ogron_earthshaker' WHERE entry = 82399;
UPDATE creature_template SET ScriptName = 'npc_highmaul_gorian_arcanist' WHERE entry = 82528;
UPDATE creature_template SET ScriptName = 'npc_highmaul_ogron_brute' WHERE entry = 82400;

DELETE FROM creature_template_addon WHERE entry = 15214;

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_rending_slash', 'spell_highmaul_shield_charge', 'spell_highmaul_earthdevastating_slam', 'spell_highmaul_earthdevastating_slam_dmg');
DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_arcane_barrage', 'spell_highmaul_decimate', 'spell_highmaul_warming_up', 'spell_highmaul_disposition', 'spell_highmaul_enfeebling_roar');
DELETE FROM spell_script_names WHERE ScriptName IN ('spell_highmaul_pol_shield_charge', 'spell_highmaul_twin_ogron_dispositions', 'spell_highmaul_pulverize_third_wave', 'spell_highmaul_phemos_whirlwind');
INSERT INTO spell_script_names VALUE
(166185, 'spell_highmaul_rending_slash'),
(166178, 'spell_highmaul_shield_charge'),
(166174, 'spell_highmaul_earthdevastating_slam'),
(166175, 'spell_highmaul_earthdevastating_slam_dmg'),
(178023, 'spell_highmaul_arcane_barrage'),
(166189, 'spell_highmaul_decimate'),
(173425, 'spell_highmaul_warming_up'),
(157953, 'spell_highmaul_disposition'),
(158026, 'spell_highmaul_enfeebling_roar'),
(158134, 'spell_highmaul_pol_shield_charge'),
(157951, 'spell_highmaul_twin_ogron_dispositions'),
(158016, 'spell_highmaul_twin_ogron_dispositions'),
(158017, 'spell_highmaul_twin_ogron_dispositions'),
(158420, 'spell_highmaul_pulverize_third_wave'),
(157943, 'spell_highmaul_phemos_whirlwind');

--                                        Phemos/Pol
DELETE FROM creature_text WHERE entry IN (78237, 78238);
INSERT INTO creature_text VALUES
(78237, 0, 0, 'Who is dat?', 14, 0, 100, 0, 0, 41932, 'PhemosIntro'),
(78237, 1, 0, 'Oy! Dey fightin''!', 14, 0, 100, 0, 0, 41928, 'PhemosAggro'),
(78237, 2, 0, '''Dis gonna hurt.', 14, 0, 100, 0, 0, 41939, 'PhemosWhirlwind'),
(78237, 3, 0, 'Let''s have some fun!', 14, 0, 100, 0, 0, 41931, 'PhemosQuake'),
(78237, 4, 0, 'Feel ''dis!', 14, 0, 100, 0, 0, 41938, 'PhemosEnfeeblingRoar'),
(78237, 5, 0, 'Bored. Let''s kill ''em, now.', 14, 0, 100, 0, 0, 41929, 'PhemosBerserk'),
(78237, 6, 0, 'I got one, too!', 14, 0, 100, 0, 0, 42087, 'PhemosSlay1'),
(78237, 6, 1, 'Dey so small.', 14, 0, 100, 0, 0, 42088, 'PhemosSlay2'),
(78237, 6, 2, 'Hah! Hah! Hah!', 14, 0, 100, 0, 0, 42089, 'PhemosSlay3'),
(78237, 7, 0, 'Dat wus fun. Let''s do it again!', 14, 0, 100, 0, 0, 41936, 'PhemosWipe'),
(78237, 8, 0, '|TInterface\\Icons\\ability_whirlwind:20|t%s readies a |cFFFF0000|Hspell:157943|h[Whirlwind]|h|r!', 41, 0, 100, 0, 0, 0, 'PhemosWhirlwindWarn'),

(78238, 0, 0, 'Da King don''t wanna see nobody! Go away!', 14, 0, 100, 0, 0, 42086, 'PolIntro'),
(78238, 1, 0, 'Fight ''em back, den.', 14, 0, 100, 0, 0, 42083, 'PolAggro'),
(78238, 2, 0, 'Hear ''dis?', 14, 0, 100, 0, 0, 42090, 'PolInterruptingShout'),
(78238, 3, 0, 'Watch out!', 14, 0, 100, 0, 0, 42092, 'PolShieldCharge'),
(78238, 4, 0, 'Heh heh... ''Dis gunna be gud.', 14, 0, 100, 0, 0, 42085, 'PolPulverize'),
(78238, 5, 0, 'Hah hah hah... got one!', 14, 0, 100, 0, 0, 41933, 'PolSlay1'),
(78238, 5, 1, 'Dey squeat when you smoosh ''em.', 14, 0, 100, 0, 0, 41935, 'PolSlay2'),
(78238, 5, 2, 'Dat one''s dead!', 14, 0, 100, 0, 0, 41934, 'PolSlay3'),
(78238, 6, 0, '|TInterface\\Icons\\ability_warrior_shieldguard:20|t%s readies a |cFFFF0000|Hspell:158136|h[Shield Charge]|h|r!', 41, 0, 100, 0, 0, 0, 'PolShieldChargeWarn');

--                                                Phemos/Pol
DELETE FROM locales_creature_text WHERE entry IN (78237, 78238);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    78237, 0, 0,
    'Quoi ça être ?',
    'Wer seid''n ihr?',
    '¿Quién es?',
    'Кого принесло?'
),
(
    78237, 1, 0,
    'Hé ! Eux combattre !',
    'Öy! Die kämpfen ja!',
    '¡Oh! ¡E''tán peleando!',
    'Ой! Они дерутся!'
),
(
    78237, 2, 0,
    'Ça faire mal.',
    'Das jetzt weh tun!',
    'Eh''to les va a doler.',
    'Будет бо-бо.'
),
(
    78237, 3, 0,
    'On va jouer !',
    'Jetzt wird''s spaßig!',
    '¡Hay que divertirnos!',
    'Ох щас будет весело!'
),
(
    78237, 4, 0,
    'Vous prendre ça !',
    'Wie sich anfühlen?',
    '¡Siente eh''to!',
    'На-ка!'
),
(
    78237, 5, 0,
    'M''ennuie. Tuer eux maintenant.',
    'Langweilig. Wir sie jetzt tot machen.',
    'Aburrido. Hay que matarlos.',
    'Скучно. Пора их прихлопнуть.'
),
(
    78237, 6, 0,
    'Moi aussi, tuer un !',
    'Ich hab auch einen!',
    '¡Tengo uno, también!',
    'Я тож одного того!'
),
(
    78237, 6, 1,
    'Eux tout petits.',
    'Die so klein sind!',
    'Qué pequeños ser.',
    'Какие мелкие.'
),
(
    78237, 6, 2,
    'Ha ! Ha ! Ha !',
    'Hah! Hah! Hah!',
    '¡Ja, ja, ja!',
    'Ха! Ха! Ха!'
),
(
    78237, 7, 0,
    'Ça être marrant. Moi vouloir refaire !',
    'Das macht Spaß. Nochmal, nochmal!',
    'Fue divertido. ¡Vamo'' otra vez!',
    'А забавно было. Повторим?'
),
(
    78237, 8, 0,
    '|TInterface\\Icons\\ability_whirlwind:20|t%s prépare un |cFFFF0000|Hspell:157943|h[Tourbillon]|h|r !',
    '|TInterface\\Icons\\ability_whirlwind:20|t%s bereitet einen |cFFFF0000|Hspell:157943|h[Wirbelwind]|h|r vor!',
    '¡|TInterface\\Icons\\ability_whirlwind:20|t%s prepara un |cFFFF0000|Hspell:157943|h[Torbellino]|h|r!',
    '|TInterface\\Icons\\ability_whirlwind:20|t%s вызывает |cFFFF0000|Hspell:157943|h[Вихрь]|h|r!'
),
(
    78238, 0, 0,
    'Le roi veut voir personne ! Vous partir !',
    'Der König will niemanden sehen! Haut ab!',
    '¡El rey no querer ver a nadie! ¡Irse ya!',
    'Король не велел никого пущать! Проваливайте!'
),
(
    78238, 1, 0,
    'On y va alors !',
    'Dann kämpf zurück.',
    'Pelear con ello'' entonces.',
    'Ну дык сдачи дай.'
),
(
    78238, 2, 0,
    'Toi entendre ça ?',
    'Hört ihr das?',
    '¿Oyes eh''to?',
    'Слышь, че?'
),
(
    78238, 3, 0,
    'Attention !',
    'Aufpassen!',
    '¡Cuidado!',
    'Поберегись!'
),
(
    78238, 4, 0,
    'Hé Hé... Nous va bien s''amuser.',
    'Freu mich schon drauf.',
    'Eh''to será bueno.',
    'Развлечемся.'
),
(
    78238, 5, 0,
    'Haha ! Gagné !',
    'Ha ha! Hab einen!',
    '¡Ja, ja! ¡Tengo uno!',
    'Ха-ха-ха... один есть!'
),
(
    78238, 5, 1,
    'Eux pleurer quand moi écraser.',
    'Die quietschen beim Quetschen!',
    'Chillan cuando los aplah''tas.',
    'Они вопят, если сдавить.'
),
(
    78238, 5, 2,
    'Çui-là mort.',
    'Der is'' kaputt.',
    'Ese ''tá muerto.',
    'Этот готов.'
),
(
    78238, 6, 0,
    '|TInterface\\Icons\\ability_warrior_shieldguard:20|t%s prépare une |cFFFF0000|Hspell:158136|h[Charge de bouclier]|h|r !',
    '|TInterface\\Icons\\ability_warrior_shieldguard:20|t%s bereitet einen |cFFFF0000|Hspell:158136|h[Schildansturm]|h|r vor!',
    '¡|TInterface\\Icons\\ability_warrior_shieldguard:20|t%s prepara una |cFFFF0000|Hspell:158136|h[Carga con escudo]|h|r!',
    '|TInterface\\Icons\\ability_warrior_shieldguard:20|t%s готовит |cFFFF0000|Hspell:158136|h[Атаку щитом]|h|r!'
);

DELETE FROM creature_equip_template WHERE entry IN (78237, 78238);
INSERT INTO creature_equip_template VALUES
(78237, 1, 113512, 113512, 0),
(78238, 1, 113544, 108626, 0);

UPDATE creature_template SET minlevel = 103, maxlevel = 103, faction = 16, rank = 3, dmg_multiplier = 10, unit_class = 4, mechanic_immune_mask = 617299839, flags_extra = (0x01 | 0x00200000) WHERE entry IN (78238, 78237);
UPDATE creature_template SET ScriptName = 'boss_twin_ogron_pol' WHERE entry = 78238;
UPDATE creature_template SET ScriptName = 'boss_twin_ogron_phemos' WHERE entry = 78237;

DELETE FROM areatrigger_template WHERE spell_id IN (162901, 168374);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, scale_curve_id, data0, data1, data2, data3, data4, data5, data6, data7, ScriptName) VALUES
(162901, 0, 6987, 1, 1, 8258, 1203, 2, 2, 6, 6, 0.3, 0.3, 0, 0, 'areatrigger_highmaul_phemos_blaze'),
(168374, 0, 6197, 1, 1, 16450, 1195, 1, 1, 0, 0, 0, 0, 0, 0, 'areatrigger_highmaul_phemos_blaze');

SET @REF_POL_NHM = 78238;

DELETE FROM `reference_loot_template` WHERE entry = @REF_POL_NHM;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_POL_NHM, 113666, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113667, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113826, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113827, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113828, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113829, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113830, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113831, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113832, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113833, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113834, 0, 1, 1, 1, 1),
(@REF_POL_NHM, 113835, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_POL_NHM WHERE `entry`= @REF_POL_NHM;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_POL_NHM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_POL_NHM, 1, 100, 1, 0, -@REF_POL_NHM, 6);