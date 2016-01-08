SET NAMES utf8;

DELETE FROM creature_template WHERE entry = 159753;
INSERT INTO `creature_template` (`entry`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
('159753','26077','0','0','0','Toran','Experience Rate Master','','159735','82','82','2','35','1','1','1.14286','0.5','4','127','500','500','500','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','1','4','1.5','1','1','0','0','0','0','0','0','0','0','1','0','0','0','npc_rate_xp_modifier');

DELETE FROM gossip_menu WHERE entry IN (159735, 159736, 159737, 159738) AND text_id IN (159735, 159736, 159737, 159738);
INSERT INTO gossip_menu VALUES
(159735, 159735),
(159736, 159736),
(159737, 159737),
(159738, 159738);

DELETE FROM npc_text WHERE ID IN (159735, 159736, 159737, 159738);
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(
    159735,
    "I'm pleased to meet you $c, may I help you for something?",
    "I'm pleased to meet you $c, may I help you for something?"
),
(
    159736,
    "It's done $c, now you have an experience rate of 1.",
    "It's done $c, now you have an experience rate of 1."
),
(
    159737,
    "It's done $c, now you have an experience rate of 3.",
    "It's done $c, now you have an experience rate of 3."
),
(
    159738,
    "It's done $c, now you have an experience rate of 5.",
    "It's done $c, now you have an experience rate of 5."
);

DELETE FROM locales_npc_text WHERE entry IN (159735, 159736, 159737, 159738);
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
(
    159735,
    "Je suis ravi de vous rencontrer $c, puis-je vous aider pour quelque chose ?",
    "Je suis ravi de vous rencontrer $c, puis-je vous aider pour quelque chose ?",
    "Ich bin froh Euch zu treffen, $c, kann ich Euch mit etwas weiterhelfen?",
    "Ich bin froh Euch zu treffen, $c, kann ich Euch mit etwas weiterhelfen?",
    "Encantado de conocerte, $c. ¿Puedo ayudarte con algo?",
    "Encantado de conocerte, $c. ¿Puedo ayudarte con algo?",
    "Приятно познакомиться $c, чем я могу помочь?",
    "Приятно познакомиться $c, чем я могу помочь?"
),
(
    159736,
    "C'est fait $c, désormais vous avez un ratio d'expérience de 1.",
    "C'est fait $c, désormais vous avez un ratio d'expérience de 1.",
    "Es ist geschafft, $c, nun habt Ihr eine Erfahrungsrate von 1.",
    "Es ist geschafft, $c, nun habt Ihr eine Erfahrungsrate von 1.",
    "Está hecho $c, ahora tienes un rating de experiencia de 1.",
    "Está hecho $c, ahora tienes un rating de experiencia de 1.",
    "Готово $c, теперь у тебя рейтинг опыта 1.",
    "Готово $c, теперь у тебя рейтинг опыта 1."
),
(
    159737,
    "C'est fait $c, désormais vous avez un ratio d'expérience de 3.",
    "C'est fait $c, désormais vous avez un ratio d'expérience de 3.",
    "Es ist geschafft, $c, nun habt Ihr eine Erfahrungsrate von 3",
    "Es ist geschafft, $c, nun habt Ihr eine Erfahrungsrate von 3.",
    "Está hecho $c, ahora tienes un rating de experiencia de 3.",
    "Está hecho $c, ahora tienes un rating de experiencia de 3.",
    "Готово $c, теперь у тебя рейтинг опыта 3.",
    "Готово $c, теперь у тебя рейтинг опыта 3."
),
(
    159738,
    "C'est fait $c, désormais vous avez un ratio d'expérience de 5.",
    "C'est fait $c, désormais vous avez un ratio d'expérience de 5.",
    "Es ist geschafft, $c, nun habt Ihr eine Erfahrungsrate von 5.",
    "Es ist geschafft, $c, nun habt Ihr eine Erfahrungsrate von 5.",
    "Está hecho $c, ahora tienes un rating de experiencia de 5.",
    "Está hecho $c, ahora tienes un rating de experiencia de 5.",
    "Готово $c, теперь у тебя рейтинг опыта 5.",
    "Готово $c, теперь у тебя рейтинг опыта 5."
);

DELETE FROM gossip_menu_option WHERE menu_id IN (159735, 159736, 159737, 159738);
INSERT INTO gossip_menu_option VALUES
(159735, 0, 0, "I would like to have an experience rate of 1.", 1, 3, 159736, 0, 0, 0, ''),
(159735, 1, 0, "I would like to have an experience rate of 3.", 1, 3, 159737, 0, 0, 0, ''),
(159735, 2, 0, "I would like to have an experience rate of 5.", 1, 3, 159738, 0, 0, 0, ''),
(159735, 3, 0, "I would like to get back my original experience rate of 5.", 1, 3, 159738, 0, 0, 0, '');

DELETE FROM locales_gossip_menu_option WHERE menu_id IN (159735, 159736, 159737, 159738);
--                                                   French            German            Spanish           Russian
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc2, option_text_loc3, option_text_loc6, option_text_loc8) VALUES
(
    159735, 0,
    "J'aimerais avoir un ratio d'expérience de 1.",
    "Ich würde gerne eine Erfahrungsrate von 1 haben.",
    "Me gustaría establecer mis rates de experiencia en 1.",
    "Я хочу изменить цифру умножения опыта на 1."
),
(
    159735, 1,
    "J'aimerais avoir un ratio d'expérience de 3.",
    "Ich würde gerne eine Erfahrungsrate von 3 haben.",
    "Me gustaría establecer mis rates de experiencia en 3.",
    "Я хочу изменить цифру умножения опыта на 3."
),
(
    159735, 2,
    "J'aimerais avoir un ratio d'expérience de 5.",
    "Ich würde gerne eine Erfahrungsrate von 5 haben.",
    "Me gustaría establecer mis rates de experiencia en 5.",
    "Я хочу изменить цифру умножения опыта на 5."
),
(
    159735, 3,
    "J'aimerais retrouver mon ratio d'expérience original de 5.",
    "Ich würde gerne zurück zu meiner ursprünglichen Erfahrungsrate von 5 zurückkehren.",
    "Me gustaría restablecer los rates de experiencia a 5.",
    "Я бы хотел вернуть свой стандартный рейтинг опыта на 5."
);