UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry = 75584;

# Croman
UPDATE creature_template SET
minlevel    = 100,
maxlevel    = 100,
exp         = 5,
unit_flags  = 33280,
unit_flags2 = 1073741824,
ScriptName  = 'mob_bloodmaul_croman'
WHERE entry = 75242;

# Croman (the real one)
UPDATE creature_template SET
minlevel        = 100,
maxlevel        = 100,
exp             = 5,
gossip_menu_id  = 16541,
unit_flags      = 32776,
unit_flags2     = 1073741824,
Health_mod      = 3,
type_flags      = type_flags | 4096,
ScriptName      = 'mob_bloodmaul_croman_real'
WHERE entry = 81032;

# Wheel of Pain
UPDATE creature_template SET
modelid1    = 51880,
modelid2    = 0,
minlevel    = 91,
maxlevel    = 91,
faction     = 7,
unit_flags  = 33587968,
unit_flags2 = 71305216,
VehicleId   = 3129,
InhabitType = 4
WHERE entry = 75252;

DELETE FROM creature WHERE map = 1175 AND id IN (75242, 75252);
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, unit_flags, unit_flags2) VALUE
(75242, 1175, 6874, 6874, 262, 1, 1995.9, -397.6617, 222.6319, 2.189103, 33280, 1073741824),
(75252, 1175, 6874, 6874, 262, 1, 1984.497, -403.1962, 219.9519, 0.6183069, 33587968, 71305216);

DELETE FROM creature_text WHERE entry IN (81032);
INSERT INTO creature_text VALUES
(81032, 0, 0, 'Ha! I can see you are brave, but danger is my trade!', 12, 0, 100, 0, 0, 0, 'CromanIntro1'),
(81032, 1, 0, 'Very well. Come with me if you want to live.', 12, 0, 100, 0, 0, 0, 'CromanIntro2'),

(81032, 2, 0, 'For me, there is no spring.', 12, 0, 100, 0, 0, 0, 'CromanAggro1'),
(81032, 2, 1, 'Nothing can hurt me! Only pain.', 12, 0, 100, 0, 0, 0, 'CromanAggro2'),
(81032, 2, 2, 'Enough talk!', 12, 0, 100, 0, 0, 0, 'CromanAggro3'),
(81032, 2, 3, 'No man shall live in chains!', 12, 0, 100, 0, 0, 0, 'CromanAggro4'),
(81032, 2, 4, 'Do it now!', 12, 0, 100, 0, 0, 0, 'CromanAggro5'),
(81032, 2, 5, 'All that matters is that we few stood against many.', 12, 0, 100, 0, 0, 0, 'CromanAggro6'),
(81032, 2, 6, 'I''m your worst nightmare!', 12, 0, 100, 0, 0, 0, 'CromanAggro7'),
(81032, 2, 7, 'If it bleeds, we can kill it!', 12, 0, 100, 0, 0, 0, 'CromanAggro8'),

(81032, 3, 0, 'Not men, not women, not beasts...', 12, 0, 100, 0, 0, 0, 'CromanSword1'),
(81032, 4, 0, 'This I can trust.', 12, 0, 100, 0, 0, 0, 'CromanSword2'),

(81032, 5, 0, 'Ah, at last.', 12, 0, 100, 0, 0, 0, 'CromanFreed1'),
(81032, 6, 0, 'I live, I love, I slay, and I am content.', 12, 0, 100, 0, 0, 0, 'CromanFreed2');

DELETE FROM locales_creature_text WHERE entry IN (81032);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    81032, 0, 0,
    'Ha ! Vous êtes $gcourageux:courageuse;, mais le danger, c''est mon métier !',
    'Ha! Ich kann sehen, dass Ihr tapfer seid, Aber die Gefahr ist mein Leben!',
    '¡Ja! Veo que eres valiente, ¡pero el peligro es lo mío!',
    'Ха! Я вижу, храбрости тебе не занимать – но я просто обожаю опасность!'
),
(
    81032, 1, 0,
    'Très bien. Venez avec moi si vous voulez vivre.',
    'Nun gut. Kommt mit mir, wenn Ihr leben wollt.',
    'Muy bien. Ven conmigo si quieres vivir.',
    'Что ж... Идём со мной, если хочешь жить.'
),
(
    81032, 2, 0,
    'Pas de printemps pour moi.',
    'Für mich gibt es keinen Frühling.',
    'Para mí, no existe la primavera.',
    'Не для меня придет весна.'
),
(
    81032, 2, 1,
    'Rien ne peut me faire souffrir ! À part la douleur.',
    'Nichts kann mir wehtun! Höchstens Schmerzen.',
    '¡Nada puede hacerme daño! Solo el dolor.',
    'Ничто не может причинить мне боль! Только боль.'
),
(
    81032, 2, 2,
    'Assez parlé !',
    'Genug geredet!',
    '¡Suficiente plática!',
    'Хватит болтать!'
),
(
    81032, 2, 3,
    'Personne ne devrait vivre enchaîné !',
    'Niemand sollte in Ketten leben müssen!',
    '¡Ningún hombre debería vivir encadenado!',
    'Никто не должен жить в рабстве!'
),
(
    81032, 2, 4,
    'Exécution !',
    'Tut es jetzt!',
    '¡Hazlo ahora!',
    'Давай, сейчас!'
),
(
    81032, 2, 5,
    'Tout ce qui compte, c''est que nous n''étions qu''une poignée contre une armée.',
    'Das Einzige, was zählt, ist, dass wir wenige gegen viele waren.',
    'Lo único que importa es que unos pocos de nosotros nos enfrentamos a muchos.',
    'Важно лишь то, что горстка наших выстояла против их орды.'
),
(
    81032, 2, 6,
    'Je suis ton pire cauchemar !',
    'Ich bin Euer schlimmster Alptraum!',
    '¡Soy tu peor pesadilla!',
    'Я твой самый страшный кошмар!'
),
(
    81032, 2, 7,
    'Si ça saigne, c''est qu''on peut le tuer !',
    'Wenn es blutet, können wir es auch töten!',
    'Si sangra, ¡lo podemos matar!',
    'Если врага можно ранить, значит, его можно и убить!'
),
(
    81032, 3, 0,
    'Par sur les hommes, ni les femmes, ni les bêtes...',
    'Weder Männer, noch Frauen, noch Bestien...',
    'Ni hombres, ni mujeres, ni bestias...',
    'Я не доверяю ни мужчинам, ни женщинам, ни зверям...'
),
(
    81032, 4, 0,
    'Mais ça, je peux compter dessus.',
    'Dem kann ich trauen.',
    'En esto puedo confiar.',
    'Но этому я могу довериться.'
),
(
    81032, 5, 0,
    'Ah, enfin.',
    'Ah, endlich.',
    'Ah, por fin.',
    'Ну наконец-то.'
),
(
    81032, 6, 0,
    'Vivre, aimer, tuer. Il m''en faut peu pour être heureux.',
    'Ich lebe, ich liebe, ich töte und ich bin glücklich.',
    'Vivo, amo, mato y soy feliz.',
    'Я живу, люблю, убиваю – и совершенно счастлив!'
);

DELETE FROM gossip_menu WHERE entry = 16541 OR text_id = 83167;
INSERT INTO gossip_menu (entry, text_id) VALUE (16541, 83167);

DELETE FROM npc_text WHERE ID = 83167;
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(
    83167,
    'Together, we have crushed the slavers and brought upon them swift destruction. They who have taken everything from me.$B$BBut, I have one last thing to give. To be a part of something greater... I''d forgotten what that feels like.',
    'Together, we have crushed the slavers and brought upon them swift destruction. They who have taken everything from me.$B$BBut, I have one last thing to give. To be a part of something greater... I''d forgotten what that feels like.'
);

DELETE FROM locales_npc_text WHERE entry = 83167;
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
(
    83167,
    'Ensemble, nous avons fait du petit bois de ces esclavagistes... les vermines qui m''ont pris tout ce que j''avais.$B$BEnfin, pas tout... il me reste encore quelque chose à offrir au monde. Servir une cause supérieure... j''avais oublié ce que ça faisait.',
    'Ensemble, nous avons fait du petit bois de ces esclavagistes... les vermines qui m''ont pris tout ce que j''avais.$B$BEnfin, pas tout... il me reste encore quelque chose à offrir au monde. Servir une cause supérieure... j''avais oublié ce que ça faisait.',
    'Zusammen haben wir die Sklavenhalter zerschmettert und ihnen schnelle Vernichtung gebracht! Sie haben mir alles genommen.$B$BDoch ich habe eine letzte Sache, die ich geben kann. Teil von etwas Größerem zu sein... Ich habe schon vergessen, wie sich das anfühlt.',
    'Zusammen haben wir die Sklavenhalter zerschmettert und ihnen schnelle Vernichtung gebracht! Sie haben mir alles genommen.$B$BDoch ich habe eine letzte Sache, die ich geben kann. Teil von etwas Größerem zu sein... Ich habe schon vergessen, wie sich das anfühlt.',
    'Juntos, hemos aplastado a los esclavistas y con ello logramos su destrucción. Ellos que me quitaron todo.Solo tengo una cosa más para dar.$B$BPor ser parte de algo más grande... habría olvidado qué se siente.',
    'Juntos, hemos aplastado a los esclavistas y con ello logramos su destrucción. Ellos que me quitaron todo.Solo tengo una cosa más para dar.$B$BPor ser parte de algo más grande... habría olvidado qué se siente.',
    'Вместе мы нанесли рабовладельцам сокрушительное поражение. Казалось, они отняли у меня все... Однако мне еще есть, что отдать.$B$BЯ могу внести свой вклад в большое дело... Я уже почти забыл, каково это.',
    'Вместе мы нанесли рабовладельцам сокрушительное поражение. Казалось, они отняли у меня все... Однако мне еще есть, что отдать.$B$BЯ могу внести свой вклад в большое дело... Я уже почти забыл, каково это.'
);