# Auchindoun Datas
UPDATE access_requirement SET level_min = 100, level_max = 100 WHERE mapId = 1182 AND difficulty = 8;
UPDATE creature SET spawnMask = spawnMask | 256 WHERE map = 1182;
UPDATE gameobject SET spawnMask = spawnMask | 256 WHERE map = 1182;

DELETE FROM gameobject WHERE map = 1182 AND id IN (211674, 211989);
INSERT INTO gameobject (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, state) VALUES
(211674, 1182, 6912, 6912, 262, 1, 1799.76, 3178.47, 35.1397, 0.253945, 0, 0, 0.126632, 0.99195, 1),
(211989, 1182, 6912, 6912, 262, 1, 1820.06, 3182.08, 31.0, 0.0890102, 0, 0, 0.0444904, 0.99901, 1);

# Iron Docks Datas
UPDATE creature SET spawnMask = spawnMask | 256, phaseMask = 1 WHERE map = 1195;

DELETE FROM gossip_menu WHERE entry = 13872;
INSERT INTO gossip_menu VALUE (13872, 61903);

DELETE FROM npc_text WHERE ID = 61903;
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(61903, "Would you like to start the Challenge?", "Would you like to start the Challenge?");

DELETE FROM locales_npc_text WHERE entry = 61903;
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
(
    61903,
    "Voulez-vous lancer le défi ?", "Voulez-vous lancer le défi ?",
    "Möchtet Ihr mit der Herausforderung beginnen?", "Möchtet Ihr mit der Herausforderung beginnen?",
    "¿Quieres comenzar el desafío?", "¿Quieres comenzar el desafío?",
    "Начать испытание?", "Начать испытание?"
);

# Challenge mode vendors gossips
UPDATE creature_template SET gossip_menu_id = 88155 WHERE entry = 88155; -- Alliance
UPDATE creature_template SET gossip_menu_id = 88161 WHERE entry = 88161; -- Horde

DELETE FROM gossip_menu WHERE entry IN (88155, 88161);
INSERT INTO gossip_menu VALUES
(88155, 90961),
(88161, 90961);

DELETE FROM npc_text WHERE ID = 90961;
INSERT INTO npc_text (ID, text0_0, text0_1) VALUES
(
    90961,
    "Greetings, $r.$B$BI can see from your demeanor that you are no ordinary adventurer.$B$BDraenor is a harsh world and many dangerous enemies lurk in both palaces and crypts. They aim to bring terrible destruction to this land using devastating weapons and magic. Securing these places and their treasures would mean excellent rewards for you and your companions.$B$BHowever, do not tread idly into these places, for other adventurers are nipping at your heels and only a race against time will prove your skills to be unmatched!$B$BAre you up for the new challenge Draenor has to offer?",
    "Greetings, $r.$B$BI can see from your demeanor that you are no ordinary adventurer.$B$BDraenor is a harsh world and many dangerous enemies lurk in both palaces and crypts. They aim to bring terrible destruction to this land using devastating weapons and magic. Securing these places and their treasures would mean excellent rewards for you and your companions.$B$BHowever, do not tread idly into these places, for other adventurers are nipping at your heels and only a race against time will prove your skills to be unmatched!$B$BAre you up for the new challenge Draenor has to offer?"
);

DELETE FROM locales_npc_text WHERE entry = 90961;
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
(
    90961,
    "Salutations, $r.$B$BAu vu de votre allure, je vois que vous n'avez rien d'$gun aventurier:une aventurière; ordinaire.$B$BDraenor est un monde hostile, et des ennemis dangereux se tapissent aussi bien dans les cryptes que dans les palais. Ils ne vivent que pour semer la destruction sur ces terres, grâce à la magie et à des armes dévastatrices. Si vous sécurisiez ces endroits ainsi que les trésors qu'ils renferment, vous et vos compagnons pourriez obtenir de très belles récompenses.$B$BCependant, hâtez-vous : d'autres aventuriers vous talonnent, et seule une course contre la montre prouvera que vous êtes $gle meilleur:la meilleure; !$B$BÊtes-vous $gprêt:prête; à relever ce nouveau défi que vous lance Draenor ?",
    "Salutations, $r.$B$BAu vu de votre allure, je vois que vous n'avez rien d'$gun aventurier:une aventurière; ordinaire.$B$BDraenor est un monde hostile, et des ennemis dangereux se tapissent aussi bien dans les cryptes que dans les palais. Ils ne vivent que pour semer la destruction sur ces terres, grâce à la magie et à des armes dévastatrices. Si vous sécurisiez ces endroits ainsi que les trésors qu'ils renferment, vous et vos compagnons pourriez obtenir de très belles récompenses.$B$BCependant, hâtez-vous : d'autres aventuriers vous talonnent, et seule une course contre la montre prouvera que vous êtes $gle meilleur:la meilleure; !$B$BÊtes-vous $gprêt:prête; à relever ce nouveau défi que vous lance Draenor ?",
    "Seid gegrüßt, $R.$B$BAn Eurer Haltung erkenne ich, dass Ihr $gkein gewöhnlicher Abenteurer:keine gewöhnliche Abenteurerin; seid.$B$BDraenor ist eine unwirtliche Welt, und viele gefährliche Feinde lauern in Palästen und auch in Gruften. Sie wollen schreckliche Zerstörung über dieses Land bringen und verwenden verheerende Waffen und Magie. Diese Orte und die Schätze darin zu sichern, kann Euch und Euren Gefährten hervorragende Belohnungen einbringen.$B$BAllerdings solltet Ihr nicht müßig an diesen Orten verweilen, denn andere Abenteurer sind Euch auf den Fersen, und nur ein Wettrennen gegen die Zeit wird beweisen, dass Ihr unübertroffene Fähigkeiten besitzt!$B$BSeid Ihr den neuen Herausforderungen, die Draenor zu bieten hat, gewachsen?",
    "Seid gegrüßt, $R.$B$BAn Eurer Haltung erkenne ich, dass Ihr $gkein gewöhnlicher Abenteurer:keine gewöhnliche Abenteurerin; seid.$B$BDraenor ist eine unwirtliche Welt, und viele gefährliche Feinde lauern in Palästen und auch in Gruften. Sie wollen schreckliche Zerstörung über dieses Land bringen und verwenden verheerende Waffen und Magie. Diese Orte und die Schätze darin zu sichern, kann Euch und Euren Gefährten hervorragende Belohnungen einbringen.$B$BAllerdings solltet Ihr nicht müßig an diesen Orten verweilen, denn andere Abenteurer sind Euch auf den Fersen, und nur ein Wettrennen gegen die Zeit wird beweisen, dass Ihr unübertroffene Fähigkeiten besitzt!$B$BSeid Ihr den neuen Herausforderungen, die Draenor zu bieten hat, gewachsen?",
    "Saludos, $r.$B$BPor tu comportamiento, veo que no eres $Gun aventurero ordinario:una aventurera ordinaria;.$B$BDraenor es un mundo duro donde los enemigos acechan en palacios y criptas. Intentan provocar una terrible destrucción con armas y magias devastadoras. Salvaguardar estos lugares y sus tesoros representaría grandes recompensas para ti y tus compañeros.$B$BPero cuídate de estos lugares, ¡pues otros aventureros te siguen de cerca y solo una carrera contra el tiempo demostrará tu valía!$B$B¿Aceptas el nuevo desafío que Draenor te presenta?",
    "Saludos, $r.$B$BPor tu comportamiento, veo que no eres $Gun aventurero ordinario:una aventurera ordinaria;.$B$BDraenor es un mundo duro donde los enemigos acechan en palacios y criptas. Intentan provocar una terrible destrucción con armas y magias devastadoras. Salvaguardar estos lugares y sus tesoros representaría grandes recompensas para ti y tus compañeros.$B$BPero cuídate de estos lugares, ¡pues otros aventureros te siguen de cerca y solo una carrera contra el tiempo demostrará tu valía!$B$B¿Aceptas el nuevo desafío que Draenor te presenta?",
    "Приветствую, |3-6($r).$B$BПо тому, как ты держишься, ясно, что ты не $gобычный путешественник:обычная путешественница;.$B$BНа Дреноре слабакам не место, многие опасные враги населяют склепы и дворцы. Они жаждут разрушить эти земли ужасающим оружием и магией. Если ты со своими спутниками решишь обезопасить эти места, то заодно разживешься ценными сокровищами.$B$BТянуть с этим не стоит, другие искатели приключений уже наступают тебе на пятки. Только действуя быстро ты останешься $gнепревзойденным:непревзойденной;!$B$BТы $gготов:готова; к новому вызову, который бросает тебе Дренор?",
    "Приветствую, |3-6($r).$B$BПо тому, как ты держишься, ясно, что ты не $gобычный путешественник:обычная путешественница;.$B$BНа Дреноре слабакам не место, многие опасные враги населяют склепы и дворцы. Они жаждут разрушить эти земли ужасающим оружием и магией. Если ты со своими спутниками решишь обезопасить эти места, то заодно разживешься ценными сокровищами.$B$BТянуть с этим не стоит, другие искатели приключений уже наступают тебе на пятки. Только действуя быстро ты останешься $gнепревзойденным:непревзойденной;!$B$BТы $gготов:готова; к новому вызову, который бросает тебе Дренор?"
);

# Enable vendors
UPDATE creature_template SET npcflag = npcflag | 0x80 WHERE entry IN (88155, 88161);
UPDATE creature SET npcflag = npcflag | 0x80 WHERE id IN (88155, 88161);

UPDATE quest_template SET RewardItemId1 = 127831 WHERE Id IN (37259, 37260, 37261, 37262, 37263, 37264, 37265, 37266);

REPLACE INTO creature_queststarter (id, quest) VALUES
(88155, 37259),
(88155, 37260),
(88155, 37261),
(88155, 37262),
(88155, 37263),
(88155, 37264),
(88155, 37265),
(88155, 37266),
(88161, 37259),
(88161, 37260),
(88161, 37261),
(88161, 37262),
(88161, 37263),
(88161, 37264),
(88161, 37265),
(88161, 37266);

REPLACE INTO creature_questender (id, quest) VALUES
(88155, 37259),
(88155, 37260),
(88155, 37261),
(88155, 37262),
(88155, 37263),
(88155, 37264),
(88155, 37265),
(88155, 37266),
(88161, 37259),
(88161, 37260),
(88161, 37261),
(88161, 37262),
(88161, 37263),
(88161, 37264),
(88161, 37265),
(88161, 37266);

UPDATE creature_template SET npcflag = npcflag | (0x01 | 0x02) WHERE entry IN (88155, 88161);
UPDATE creature SET npcflag = npcflag | (0x01 | 0x02) WHERE id IN (88155, 88161);

DELETE FROM pool_quest WHERE pool_entry = 30008;
DELETE FROM pool_template WHERE entry = 30008;
INSERT INTO pool_template VALUE (30008, 1, "Challenge mode daily quest");
INSERT INTO pool_quest VALUES
(37259, 30008, "A Worthy Challenge: Teron'gor"),
(37260, 30008, "A Worthy Challenge: Gug'rokk"),
(37261, 30008, "A Worthy Challenge: Skylord Tovra"),
(37262, 30008, "A Worthy Challenge: Skullok, Son of Gruul"),
(37263, 30008, "A Worthy Challenge: Ner'zhul"),
(37264, 30008, "A Worthy Challenge: High Sage Viryx"),
(37265, 30008, "A Worthy Challenge: Yalnu"),
(37266, 30008, "A Worthy Challenge: Warlord Zaela");

DELETE FROM item_script_names WHERE ScriptName = "item_script_challengers_strongbox" AND Id = 127831;
INSERT INTO item_script_names VALUE (127831, "item_script_challengers_strongbox");

# ILvl 640 rewards
DELETE FROM item_loot_template WHERE entry = 127831;
INSERT INTO item_loot_template VALUES
(127831, 118969, 100, 1, 0, 1, 1, ""),
(127831, 118997, 100, 1, 0, 1, 1, ""),
(127831, 118998, 100, 1, 0, 1, 1, ""),
(127831, 118968, 100, 1, 0, 1, 1, ""),
(127831, 118962, 100, 1, 0, 1, 1, ""),
(127831, 118967, 100, 1, 0, 1, 1, ""),
(127831, 118958, 100, 1, 0, 1, 1, ""),
(127831, 118964, 100, 1, 0, 1, 1, ""),
(127831, 118948, 100, 1, 0, 1, 1, ""),
(127831, 118963, 100, 1, 0, 1, 1, ""),
(127831, 119013, 100, 1, 0, 1, 1, ""),
(127831, 118947, 100, 1, 0, 1, 1, ""),
(127831, 118961, 100, 1, 0, 1, 1, ""),
(127831, 118957, 100, 1, 0, 1, 1, ""),
(127831, 119009, 100, 1, 0, 1, 1, ""),
(127831, 118949, 100, 1, 0, 1, 1, ""),
(127831, 118972, 100, 1, 0, 1, 1, ""),
(127831, 118999, 100, 1, 0, 1, 1, ""),
(127831, 118946, 100, 1, 0, 1, 1, ""),
(127831, 118956, 100, 1, 0, 1, 1, ""),
(127831, 118970, 100, 1, 0, 1, 1, ""),
(127831, 119006, 100, 1, 0, 1, 1, ""),
(127831, 119016, 100, 1, 0, 1, 1, ""),
(127831, 118951, 100, 1, 0, 1, 1, ""),
(127831, 118960, 100, 1, 0, 1, 1, ""),
(127831, 118984, 100, 1, 0, 1, 1, ""),
(127831, 119012, 100, 1, 0, 1, 1, ""),
(127831, 119021, 100, 1, 0, 1, 1, ""),
(127831, 118955, 100, 1, 0, 1, 1, ""),
(127831, 118966, 100, 1, 0, 1, 1, ""),
(127831, 118971, 100, 1, 0, 1, 1, ""),
(127831, 118950, 100, 1, 0, 1, 1, ""),
(127831, 118965, 100, 1, 0, 1, 1, ""),
(127831, 118975, 100, 1, 0, 1, 1, ""),
(127831, 118978, 100, 1, 0, 1, 1, ""),
(127831, 119014, 100, 1, 0, 1, 1, ""),
(127831, 118996, 100, 1, 0, 1, 1, ""),
(127831, 118952, 100, 1, 0, 1, 1, ""),
(127831, 118954, 100, 1, 0, 1, 1, ""),
(127831, 119010, 100, 1, 0, 1, 1, ""),
(127831, 119011, 100, 1, 0, 1, 1, ""),
(127831, 118973, 100, 1, 0, 1, 1, ""),
(127831, 118974, 100, 1, 0, 1, 1, ""),
(127831, 119005, 100, 1, 0, 1, 1, ""),
(127831, 119007, 100, 1, 0, 1, 1, ""),
(127831, 119008, 100, 1, 0, 1, 1, ""),
(127831, 119018, 100, 1, 0, 1, 1, ""),
(127831, 118953, 100, 1, 0, 1, 1, ""),
(127831, 118959, 100, 1, 0, 1, 1, ""),
(127831, 118976, 100, 1, 0, 1, 1, ""),
(127831, 118977, 100, 1, 0, 1, 1, ""),
(127831, 118979, 100, 1, 0, 1, 1, ""),
(127831, 118980, 100, 1, 0, 1, 1, ""),
(127831, 118981, 100, 1, 0, 1, 1, ""),
(127831, 118982, 100, 1, 0, 1, 1, ""),
(127831, 118983, 100, 1, 0, 1, 1, ""),
(127831, 118985, 100, 1, 0, 1, 1, ""),
(127831, 118986, 100, 1, 0, 1, 1, ""),
(127831, 118987, 100, 1, 0, 1, 1, ""),
(127831, 118988, 100, 1, 0, 1, 1, ""),
(127831, 118989, 100, 1, 0, 1, 1, ""),
(127831, 118990, 100, 1, 0, 1, 1, ""),
(127831, 118991, 100, 1, 0, 1, 1, ""),
(127831, 118992, 100, 1, 0, 1, 1, ""),
(127831, 118993, 100, 1, 0, 1, 1, ""),
(127831, 118994, 100, 1, 0, 1, 1, ""),
(127831, 118995, 100, 1, 0, 1, 1, ""),
(127831, 119015, 100, 1, 0, 1, 1, ""),
(127831, 119017, 100, 1, 0, 1, 1, ""),
(127831, 119019, 100, 1, 0, 1, 1, ""),
(127831, 119020, 100, 1, 0, 1, 1, "");