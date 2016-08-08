SET NAMES utf8;

SET @REF_BLACKHAND = 77325;

UPDATE creature_template SET unit_flags = 33587264, unit_flags2 = 0, InhabitType = 4 WHERE entry = 87987;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_forgemistress_flamehand" WHERE entry = 87989;
UPDATE creature_template SET dmg_multiplier = 10, InhabitType = 1, ScriptName = "boss_blackhand", mechanic_immune_mask = 617299839 WHERE entry = @REF_BLACKHAND;
UPDATE creature_template SET unit_flags = 33587264, unit_flags2 = 4194304, InhabitType = 4, ScriptName = "npc_foundry_blackrock_foundry" WHERE entry = 77558;
UPDATE creature_template SET unit_flags = 33554432, unit_flags2 = 2097152, InhabitType = 4, ScriptName = "npc_foundry_slag_bomb" WHERE entry = 77343;
UPDATE creature_template SET unit_flags = 33554432, unit_flags2 = 2097152, InhabitType = 4, ScriptName = "npc_foundry_ceiling_stalker" WHERE entry = 77570;
UPDATE creature_template SET unit_flags = 33587264, unit_flags2 = 2097152, InhabitType = 4, ScriptName = "npc_foundry_brf_demolition_stalker" WHERE entry = 77572;
UPDATE creature_template SET unit_flags = 33587264, unit_flags2 = 2097152, InhabitType = 4, ScriptName = "npc_foundry_brf_massive_demolition_stalker" WHERE entry = 77577;
UPDATE creature_template SET unit_flags = 33587264, unit_flags2 = 0, InhabitType = 4, ScriptName = "npc_foundry_rubble_pile" WHERE entry = 77405;
UPDATE creature_template SET unit_flags = 33587520, unit_flags2 = 2097152, InhabitType = 1, ScriptName = "npc_foundry_siegemaker" WHERE entry = 77342;
UPDATE creature_template SET unit_flags = 33587200, unit_flags2 = 2048 WHERE entry = 77755;
UPDATE creature_template SET unit_flags = 33554432, ScriptName = "npc_foundry_blaze_controller" WHERE entry = 77610;
UPDATE creature_template SET unit_flags = 33554432, unit_flags2 = 2097152, ScriptName = "npc_foundry_blaze_controller" WHERE entry = 77612;
UPDATE creature_template SET unit_flags = 33554432, unit_flags2 = 2097152, InhabitType = 4, ScriptName = "npc_foundry_blackrock_foundry_third_phase" WHERE entry = 77628;
UPDATE creature_template SET unit_flags = 33587456, unit_flags2 = 4229120, InhabitType = 4, ScriptName = "npc_foundry_iron_sentry" WHERE entry = 77720;
UPDATE creature_template SET unit_flags = 32768, unit_flags2 = 4194304, unit_flags3 = 32, ScriptName = "npc_foundry_iron_soldier" WHERE entry = 77665;
UPDATE creature_template SET unit_flags = 33554432, unit_flags2 = 2129920, ScriptName = "npc_foundry_slag_hole" WHERE entry = 77357;
UPDATE creature_template SET unit_flags = 33554432, unit_flags2 = 32768, ScriptName = "npc_foundry_slag_crater" WHERE entry = 77774;
UPDATE creature_template SET unit_flags = 33587264, unit_flags2 = 0, InhabitType = 4, ScriptName = "npc_foundry_achievement_stalker" WHERE entry = 80831;

UPDATE gameobject_template SET WorldEffectID = 662553366 WHERE entry = 239118;

DELETE FROM conditions WHERE SourceEntry IN (175576, 177554, 156044, 156425, 156452, 156795, 157322, 156771);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 175576, 31, 0, 3, 87987),
(13, 1, 177554, 31, 0, 3, 87987),
(13, 2, 156044, 31, 0, 3, 77342),
(13, 15, 156425, 31, 0, 3, 77570),
(13, 1, 156452, 31, 0, 3, 77572),
(13, 1, 156795, 31, 0, 3, 77342),
(13, 1, 157322, 31, 0, 3, 77342),
(13, 1, 156771, 31, 0, 3, 77720);

DELETE FROM `areatrigger_template` WHERE spell_id IN (175579, 156037, 156605, 156859);
INSERT INTO `areatrigger_template` (`spell_id`, `eff_index`, `entry`, `type`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`) VALUES
(175579, 0, 8031, 1, 0, 0, 2052, 0, 20, 6, 5, 20, 6, 5, 0, 0, "areatrigger_foundry_flame_jet"),
(156037, 0, 6326, 2, 2, 2, 16388, 0, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_slag_bomb"),
(156605, 0, 6380, 2, 5, 5, 16388, 0, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_blaze"),
(156859, 0, 6423, 2, 1, 1, 16388, 0, 70, 70, 5, 70, 70, 5, 0, 0, "areatrigger_foundry_overheated"),
(163295, 0, 7022, 3, 2, 2, 8192, 0, 2, 2, 6, 6, 0.3, 0.3, 0, 0, "areatrigger_foundry_falling_ash");

DELETE FROM areatrigger_scripts WHERE entry IN (10275, 10101);
INSERT INTO areatrigger_scripts VALUES
(10275, "areatrigger_at_foundry_blackhand_entrance"),
(10101, "areatrigger_at_foundry_crucible_balcony");

DELETE FROM spell_script_names WHERE spell_id IN (156497, 156479, 156096, 156533, 157987, 156652, 156031, 156033, 156034, 156698, 156928, 157000, 158054);
INSERT INTO spell_script_names VALUES
(156497, "spell_foundry_demolition"),
(156479, "spell_foundry_massive_demolition"),
(156096, "spell_foundry_marked_for_death"),
(156533, "spell_foundry_blaze_growth"),
(157987, "spell_foundry_siegemaker_energize"),
(156652, "spell_foundry_fixate_searcher"),
(156031, "spell_foundry_throw_slag_bombs"),
(156033, "spell_foundry_throw_slag_bombs"),
(156034, "spell_foundry_throw_slag_bombs"),
(156698, "spell_foundry_explosive_round"),
(156928, "spell_foundry_slag_eruption"),
(157000, "spell_foundry_attach_slag_bombs"),
(158054, "spell_foundry_massive_shattering_smash");

DELETE FROM `reference_loot_template` WHERE entry = @REF_BLACKHAND;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BLACKHAND, 113988, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113984, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113990, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113989, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113981, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113982, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113980, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113979, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113985, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113987, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113986, 0, 1, 1, 1, 1),
(@REF_BLACKHAND, 113983, 0, 1, 1, 1, 1);

# Set faction tokens
DELETE FROM `reference_loot_template` WHERE entry = @REF_BLACKHAND + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
-- Horde tokens
(@REF_BLACKHAND + 1, 119323, 0, 14, 1, 1, 1),
(@REF_BLACKHAND + 1, 119316, 0, 14, 1, 1, 1),
(@REF_BLACKHAND + 1, 119310, 0, 14, 1, 1, 1),
-- Alliance tokens
(@REF_BLACKHAND + 1, 120279, 0, 14, 1, 1, 1),
(@REF_BLACKHAND + 1, 120278, 0, 14, 1, 1, 1),
(@REF_BLACKHAND + 1, 120277, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_BLACKHAND WHERE `entry`= @REF_BLACKHAND;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_BLACKHAND;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BLACKHAND, 1, 100, 1, 0, -@REF_BLACKHAND, 6),
(@REF_BLACKHAND, 2, 100, 2, 0, -(@REF_BLACKHAND + 1), 2),
(@REF_BLACKHAND, 3, 100, 4, 0, -(@REF_BLACKHAND + 1), 1),
(@REF_BLACKHAND, 4, 100, 8, 0, -(@REF_BLACKHAND + 1), 1);

-- Add condition for faction tokens
DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = @REF_BLACKHAND + 1 AND SourceEntry IN (119323, 119316, 119310, 120279, 120278, 120277);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1) VALUES
(1, @REF_BLACKHAND + 1, 119323, 6, 67),
(1, @REF_BLACKHAND + 1, 119316, 6, 67),
(1, @REF_BLACKHAND + 1, 119310, 6, 67),
(1, @REF_BLACKHAND + 1, 120279, 6, 469),
(1, @REF_BLACKHAND + 1, 120278, 6, 469),
(1, @REF_BLACKHAND + 1, 120277, 6, 469);

DELETE FROM creature_text WHERE entry IN (@REF_BLACKHAND, 77343);
INSERT INTO creature_text VALUES
(@REF_BLACKHAND, 0, 0, "You? You're the dogs who have been wreaking havoc throughout my foundry? Step forward! I will put you in your place.", 14, 0, 100, 378, 0, 45434, "Intro"),
(@REF_BLACKHAND, 1, 0, "I will hang your broken bodies from the gates of this foundry!", 14, 0, 100, 0, 0, 45419, "Aggro 1"),
(@REF_BLACKHAND, 1, 1, "You destroyed my furnace... Now, my furnace will destroy YOU.", 14, 0, 100, 0, 0, 45447, "Aggro 2"),
(@REF_BLACKHAND, 2, 0, "Open the flumes! Flood the room with molten slag.", 14, 0, 100, 0, 0, 45448, "Molten Slag 1"),
(@REF_BLACKHAND, 2, 1, "I'll burn you alive, even if I have to flood the whole foundry!", 14, 0, 100, 0, 0, 45449, "Molten Slag 2"),
(@REF_BLACKHAND, 3, 0, "|TInterface\\Icons\\ability_blackhand_demolition.blp:20|t %s casts |cFFFF0000|Hspell:156425|h[Demolition]|h|r!", 41, 0, 100, 0, 0, 0, "Demolition Warn"),
(@REF_BLACKHAND, 4, 0, "I'll bring this whole place down around your heads!", 14, 0, 100, 0, 0, 45421, "Demolition 1"),
(@REF_BLACKHAND, 4, 1, "I'll tear this place apart if I have to!", 14, 0, 100, 0, 0, 45422, "Demolition 2"),
(@REF_BLACKHAND, 5, 0, "|TInterface\\Icons\\ability_blackhand_marked4death:20|tYou are |cFFFF0000|Hspell:156096|h[Marked for Death]|h|r!", 42, 0, 100, 0, 0, 0, "Marked for Death Warn"),
(@REF_BLACKHAND, 6, 0, "You're slow!", 14, 0, 100, 0, 0, 45450, "Impaling Throw hits target 1"),
(@REF_BLACKHAND, 6, 1, "You are weak.", 14, 0, 100, 0, 0, 45451, "Impaling Throw hits target 2"),
(@REF_BLACKHAND, 6, 2, "You should've ducked.", 14, 0, 100, 0, 0, 45452, "Impaling Throw hits target 3"),
(@REF_BLACKHAND, 7, 0, "Coward!", 14, 0, 100, 0, 0, 45454, "Impaling Throw misses target 1"),
(@REF_BLACKHAND, 7, 1, "You hide like children!", 14, 0, 100, 0, 0, 45455, "Impaling Throw misses target 2"),
(@REF_BLACKHAND, 7, 2, "Get out here and fight!", 14, 0, 100, 0, 0, 45456, "Impaling Throw misses target 3"),
(@REF_BLACKHAND, 8, 0, "We will rebuild this place... but no one will be able to put YOU back together.", 14, 0, 100, 0, 0, 45423, "Phase 2 1"),
(@REF_BLACKHAND, 8, 1, "Everyone DOWN.", 14, 0, 100, 0, 0, 45440, "Phase 2 2"),
(@REF_BLACKHAND, 9, 0, "%s calls in a distant |cFFFF0000Siegemaker|r!", 41, 0, 100, 0, 0, 0, "Siegemaker Warn"),
(@REF_BLACKHAND, 10, 0, "Bring in the Siegemaker.", 14, 0, 100, 0, 0, 45457, "Siegemaker 1"),
(@REF_BLACKHAND, 10, 1, "Siege machines! Tear this place apart!", 14, 0, 100, 0, 0, 45458, "Siegemaker 2"),
(@REF_BLACKHAND, 10, 2, "Machines, open fire! Destroy them at any cost!", 14, 0, 100, 0, 0, 45459, "Siegemaker 3"),
(@REF_BLACKHAND, 11, 0, "This one should go out with a bang!", 14, 0, 100, 0, 0, 45443, "Enhanced Siegemaker 1"),
(@REF_BLACKHAND, 11, 1, "This one's packing a little extra heat.", 14, 0, 100, 0, 0, 45444, "Enhanced Siegemaker 2"),
(@REF_BLACKHAND, 11, 2, "This prototype has double armor plating!", 14, 0, 100, 0, 0, 45445, "Enhanced Siegemaker 3"),
(@REF_BLACKHAND, 11, 3, "The fastest machine in our arsenal!", 14, 0, 100, 0, 0, 45446, "Enhanced Siegemaker 4"),
(@REF_BLACKHAND, 12, 0, "Snipers! Gun them down.", 14, 0, 100, 0, 0, 45431, "Iron Soldier 1"),
(@REF_BLACKHAND, 12, 1, "Soldiers! Open fire!", 14, 0, 100, 0, 0, 45432, "Iron Soldier 2"),
(@REF_BLACKHAND, 12, 2, "Blackrock warriors - fire!", 14, 0, 100, 0, 0, 45433, "Iron Soldier 3"),
(@REF_BLACKHAND, 13, 0, "Burn - you will all BURN!", 14, 0, 100, 0, 0, 45441, "Phase 3 1"),
(@REF_BLACKHAND, 13, 1, "This foundry's molten heart will devour you!", 14, 0, 100, 0, 0, 45442, "Phase 3 2"),
(@REF_BLACKHAND, 14, 0, "There's no place to hide, now!", 14, 0, 100, 0, 0, 45453, "Impaling Throw (Phase 3)"),
(@REF_BLACKHAND, 15, 0, "|TInterface\\Icons\\spell_fire_ragnaros_lavabolt.blp:20|t %s casts |cFFFF0000|Hspell:156928|h[Slag Eruption]|h|r!", 41, 0, 100, 0, 0, 0, "Slag Eruption Warn"),
(@REF_BLACKHAND, 16, 0, "Your bodies will fuel the flames!", 14, 0, 100, 0, 0, 45425, "Slag Eruption 1"),
(@REF_BLACKHAND, 16, 1, "The furnace will consume you!", 14, 0, 100, 0, 0, 45426, "Slag Eruption 2"),
(@REF_BLACKHAND, 16, 2, "The hotter the flame, the stronger the steel!", 14, 0, 100, 0, 0, 45427, "Slag Eruption 3"),
(@REF_BLACKHAND, 17, 0, "This cauldron is your end.", 14, 0, 100, 0, 0, 45428, "Massive Shattering Smash 1"),
(@REF_BLACKHAND, 17, 1, "Your time runs out.", 14, 0, 100, 0, 0, 45429, "Massive Shattering Smash 2"),
(@REF_BLACKHAND, 17, 2, "This crucible is your molten grave.", 14, 0, 100, 0, 0, 45430, "Massive Shattering Smash 3"),
(@REF_BLACKHAND, 18, 0, "Weak.", 14, 0, 100, 0, 0, 45435, "Killing a player 1"),
(@REF_BLACKHAND, 18, 1, "You have no discipline.", 14, 0, 100, 0, 0, 45436, "Killing a player 2"),
(@REF_BLACKHAND, 18, 2, "Our weapons are superior.", 14, 0, 100, 0, 0, 45437, "Killing a player 3"),
(@REF_BLACKHAND, 18, 3, "Pah! Dead.", 14, 0, 100, 0, 0, 45438, "Killing a player 4"),
(@REF_BLACKHAND, 18, 4, "That's all you can muster?", 14, 0, 100, 0, 0, 45439, "Killing a player 5"),
(@REF_BLACKHAND, 19, 0, "I will finish this myself!!", 14, 0, 100, 0, 0, 45420, "Berserk"),
(@REF_BLACKHAND, 20, 0, "We are... the Warchief's weapon... Without us, he...", 14, 0, 100, 0, 0, 45424, "Death"),

(77343, 0, 0, "$n triggered a Slag Bomb.", 16, 0, 100, 0, 0, 0, "Slag Bomb Triggered");

DELETE FROM locales_creature_text WHERE entry IN (@REF_BLACKHAND, 77343);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_BLACKHAND, 0, 0,
    "Vous ? Ainsi donc, voilà les chiens qui sèment le chaos dans ma fonderie ? Approchez ! Je vais vous remettre à votre place.",
    "Ihr? Ihr seid die Hunde, die meine Gießerei auf den Kopf stellen? Tretet vor! Ich zeige Euch, wer hier das Sagen hat.",
    "¿Ustedes? ¿Ustedes son los perros que han estado desatando el caos en mi fundición? ¡Den un paso adelante! Los pondré en su lugar.",
    "Так это вы?! Вы - те самые шавки, которые устроили хаос в моей литейной? Я поставлю вас на место."
),
(
    @REF_BLACKHAND, 1, 0,
    "Je vais accrocher vos carcasses aux portes de cette fonderie !",
    "Eure zerschlagenen Leichen werden mein Eingangstor schmücken!",
    "¡Colgaré sus cuerpos despedazados en las puertas de esta fundición!",
    "Я развешу ваши трупы на воротах литейной!"
),
(
    @REF_BLACKHAND, 1, 1,
    "Vous avez détruit mon fourneau... Maintenant, il va vous rendre la politesse.",
    "Ihr habt meinen Ofen zerstört... jetzt zerstört mein Ofen EUCH!",
    "Destruyeron mi horno... Ahora mi horno los destruirá A USTEDES.",
    "Вы уничтожили горнило... Теперь оно уничтожит вас."
),
(
    @REF_BLACKHAND, 2, 0,
    "Ouvrez les vannes ! Remplissez-moi cette salle de scories en fusion.",
    "Öffnet die Schleusen! Flutet den Raum mit heißer Schlacke.",
    "¡Abran los saetines! Inunden la habitación de escoria fundida.",
    "Открыть заслонки! Залейте все шлаком!"
),
(
    @REF_BLACKHAND, 2, 1,
    "Je vais vous carboniser, quitte à noyer cette fonderie dans le métal en fusion...",
    "Ich verbrenne Euch alle, selbst wenn ich die ganze Gießerei fluten muss...",
    "Los quemaré vivos, aunque tenga que inundar toda la fundición...",
    "Я сожгу вас живьем, пусть даже вместе со всей литейной..."
),
(
    @REF_BLACKHAND, 3, 0,
    "|TInterface\\Icons\\ability_blackhand_demolition.blp:20|t %s lance |cFFFF0000|Hspell:156425|h[Démolition]|h|r !",
    "|TInterface\\Icons\\ability_blackhand_demolition.blp:20|t %s löst einen |cFFFF0000|Hspell:156425|h[Einsturz]|h|r aus!",
    "¡|TInterface\\Icons\\ability_blackhand_demolition.blp:20|t%s lanza |cFFFF0000|Hspell:156425|h[Demolición]|h|r!",
    "|TInterface\\Icons\\ability_blackhand_demolition.blp:20|t %s вызывает |cFFFF0000|Hspell:156425|h[обрушение]|h|r!"
),
(
    @REF_BLACKHAND, 4, 0,
    "Je vais vous ensevelir dans les décombres de ce bâtiment !",
    "Ich lasse hier alles auf Euch einstürzen!",
    "¡Destruiré este lugar alrededor de sus cabezas!",
    "Я обрушу литейную на ваши головы!"
),
(
    @REF_BLACKHAND, 4, 1,
    "Je n'hésiterai pas à raser cet endroit, s'il le faut !",
    "Wenn es sein muss, lege ich alles in Schutt und Asche!",
    "¡Destrozaré este sitio si es necesario!",
    "Если придется, я здесь все разнесу!"
),
(
    @REF_BLACKHAND, 5, 0,
    "|TInterface\\Icons\\ability_blackhand_marked4death:20|t Vous êtes |cFFFF0000|Hspell:156096|h[$GDésigné:Désignée; pour mourir]|h|r !",
    "|TInterface\\Icons\\ability_blackhand_marked4death:20|tÜber Euch ist ein |cFFFF0000|Hspell:156096|h[Todesurteil]|h|r verhängt worden!",
    "|TInterface\\Icons\\ability_blackhand_marked4death:20|t¡Estás |cFFFF0000|Hspell:156096|h[$gMarcado:Marcada; para morir]|h|r!",
    "|TInterface\\Icons\\ability_blackhand_marked4death:20|tНа вас поставили |cFFFF0000|Hspell:156096|h[метку смерти]|h|r."
),
(
    @REF_BLACKHAND, 6, 0,
    "Quelle lenteur.",
    "Ihr seid langsam.",
    "Son lentos.",
    "Слишком медленно."
),
(
    @REF_BLACKHAND, 6, 1,
    "Vous êtes faibles.",
    "Ihr seid schwach.",
    "Son débiles.",
    "Вы слабы."
),
(
    @REF_BLACKHAND, 6, 2,
    "Vous auriez dû esquiver.",
    "Ihr hättet Euch ducken sollen.",
    "Debieron agacharse.",
    "Нужно было пригнуться."
),
(
    @REF_BLACKHAND, 7, 0,
    "Lâche !",
    "Feigling!",
    "¡Cobarde!",
    "Трус!"
),
(
    @REF_BLACKHAND, 7, 1,
    "Des gamins dans les jupons de leurs mères !",
    "Ihr versteckt Euch wie Kinder!",
    "¡Se esconden como niños!",
    "Прячешься как ребенок!"
),
(
    @REF_BLACKHAND, 7, 2,
    "Sortez de là et venez vous battre !",
    "Kommt her und kämpft!",
    "¡Salgan y peleen!",
    "Выходи и дерись!"
),
(
    @REF_BLACKHAND, 8, 0,
    "Nous rebâtirons les lieux… c'est VOUS qu'on ne pourra pas retaper, après ça.",
    "Die Anlage bauen wir wieder auf... aber EUCH flickt niemand wieder zusammen.",
    "Ya reconstruiremos este lugar... pero nadie podrá volverlos a unir a USTEDES.",
    "Литейную можно отстроить заново, а вот вас никто по кусочкам не соберет."
),
(
    @REF_BLACKHAND, 8, 1,
    "Tout le monde à TERRE.",
    "Auf die KNIE.",
    "Todo el mundo ABAJO.",
    "Всем ЛЕЧЬ!"
),
(
    @REF_BLACKHAND, 9, 0,
    "%s appelle un |cFFFF0000Assiégeur|r au loin !",
    "%s ruft aus der Ferne einen |cFFFF0000Mauerbrecher|r herbei!",
    "¡%s llama a un lejano |cFFFF0000Asediador|r!",
    "%s призывает |cFFFF0000осадный танк|r!"
),
(
    @REF_BLACKHAND, 10, 0,
    "Envoyez l'assiégeur.",
    "Her mit dem Mauerbrecher.",
    "Traigan al asediador.",
    "Где осадный танк?"
),
(
    @REF_BLACKHAND, 10, 1,
    "Engins de siège ! Détruisez les lieux !",
    "Belagerungsmaschinen! Legt alles in Schutt und Asche!",
    "¡Máquinas de asedio! ¡Destrocen este sitio!",
    "Осадные машины! Разрушьте все!"
),
(
    @REF_BLACKHAND, 10, 2,
    "Que les engins de siège ouvrent le feu ! Anéantissez-les !",
    "Maschinen, Feuer frei! Vernichtet sie, koste es, was es wolle!",
    "Máquinas, ¡abran fuego! ¡Destrúyanlos a cualquier precio!",
    "Машины, огонь! Уничтожьте их любой ценой!"
),
(
    @REF_BLACKHAND, 11, 0,
    "Attention, ça va faire un grand boum...",
    "Diese hier hinterlässt einen bleibenden Eindruck...",
    "Este debe irse como los grandes...",
    "Эта модель просто взорвет вам мозг."
),
(
    @REF_BLACKHAND, 11, 1,
    "Je peux vous dire que ça va chauffer.",
    "Diese Maschine heizt Euch ein.",
    "Este se lleva un poco de calor extra.",
    "Сейчас станет жарко."
),
(
    @REF_BLACKHAND, 11, 2,
    "Ce prototype dispose d'un blindage renforcé.",
    "Dieser Prototyp hat die doppelte Panzerung.",
    "Este prototipo tiene doble blindaje de armadura.",
    "У этой модели двойная обшивка брони."
),
(
    @REF_BLACKHAND, 11, 3,
    "Voilà l'engin le plus rapide de notre arsenal !",
    "Die schnellste Maschine in unserem Arsenal!",
    "¡La máquina más rápida de nuestro arsenal!",
    "Это наша самая быстрая машина!"
),
(
    @REF_BLACKHAND, 12, 0,
    "Tireurs ! Abattez-les.",
    "Scharfschützen! Erschießt sie!",
    "¡Francotiradores! Disparen hasta matarlos.",
    "Снайперы! Пристрелите их!"
),
(
    @REF_BLACKHAND, 12, 1,
    "Soldats ! Ouvrez le feu !",
    "Soldaten, Feuer frei!",
    "¡Soldados! ¡Abran fuego!",
    "Солдаты! Открыть огонь!"
),
(
    @REF_BLACKHAND, 12, 2,
    "Guerriers rochenoires... feu !",
    "Schwarzfelskrieger, Feuer!",
    "Guerreros Roca Negra: ¡fuego!",
    "Воины Черной горы, огонь!"
),
(
    @REF_BLACKHAND, 13, 0,
    "Brûler... vous allez tous BRÛLER !",
    "Brennt  Ihr sollt alle BRENNEN!",
    "Arderán... ¡Todos ustedes ARDERÁN!",
    "Вы все СГОРИТЕ!!"
),
(
    @REF_BLACKHAND, 13, 1,
    "Le cœur brûlant de cette fonderie va vous carboniser.",
    "Das lodernde Herz der Gießerei verschlingt Euch.",
    "¡El corazón derretido de esta fundición los devorará!",
    "Вы утонете в огненном чреве литейной!"
),
(
    @REF_BLACKHAND, 14, 0,
    "Fini de se cacher, maintenant.",
    "Ihr könnt Euch nirgends verstecken.",
    "No hay dónde esconderse ahora.",
    "Тебе больше негде прятаться."
),
(
    @REF_BLACKHAND, 15, 0,
    "|TInterface\\Icons\\spell_fire_ragnaros_lavabolt.blp:20|t %s lance |cFFFF0000|Hspell:156928|h[Éruption de scories]|h|r !",
    "|TInterface\\Icons\\spell_fire_ragnaros_lavabolt.blp:20|t %s wirkt |cFFFF0000|Hspell:156928|h[Schlackeausbruch]|h|r!",
    "|TInterface\\Icons\\spell_fire_ragnaros_lavabolt.blp:20|t¡%s lanza |cFFFF0000|Hspell:156928|h[Erupción de escoria]|h|r!",
    "|TInterface\\Icons\\spell_fire_ragnaros_lavabolt.blp:20|t %s вызывает |cFFFF0000|Hspell:156928|h[Извержение шлака]|h|r!"
),
(
    @REF_BLACKHAND, 16, 0,
    "Vos cadavres vont alimenter les flammes.",
    "Eure Leichen werden Zunder sein.",
    "Sus cuerpos alimentarán las llamas.",
    "Пламя пожрет ваши трупы."
),
(
    @REF_BLACKHAND, 16, 1,
    "La fournaise va vous consumer !",
    "Der Ofen verschlingt Euch!",
    "¡El horno los consumirá!",
    "Горнило поглотит вас!"
),
(
    @REF_BLACKHAND, 16, 2,
    "Plus la flamme est brûlante, plus l'acier est solide...",
    "Je heißer die Flamme, desto stärker der Stahl...",
    "Entre más caliente la flama, más fuerte el acero...",
    "Чем жарче огонь - тем крепче сталь..."
),
(
    @REF_BLACKHAND, 17, 0,
    "Ce chaudron sera la dernière chose que vous verrez.",
    "Dieser Kessel ist Euer Ende.",
    "Este caldero es su fin.",
    "В этом котле - ваша смерть."
),
(
    @REF_BLACKHAND, 17, 1,
    "Le temps joue contre vous.",
    "Euch geht die Zeit aus.",
    "Su tiempo se agota.",
    "Ваше время вышло."
),
(
    @REF_BLACKHAND, 17, 2,
    "Ce creuset sera votre tombeau !",
    "Dieser Schmelztiegel wird Euer Grab!",
    "¡Este crisol es su tumba derretida!",
    "Это горнило станет вашей могилой!"
),
(
    @REF_BLACKHAND, 18, 0,
    "Faible.",
    "Schwach.",
    "Débil.",
    "Ничтожество."
),
(
    @REF_BLACKHAND, 18, 1,
    "Vous n'avez aucune discipline.",
    "Undiszipliniert.",
    "No tienen disciplina.",
    "Никакой дисциплины."
),
(
    @REF_BLACKHAND, 18, 2,
    "Nos armes sont bien supérieures.",
    "Vorsprung durch Technik.",
    "Nuestras armas son superiores.",
    "Наше оружие лучше."
),
(
    @REF_BLACKHAND, 18, 3,
    "Un de moins.",
    "Pah! Tot.",
    "¡Pah! Muerto.",
    "Бах! И все."
),
(
    @REF_BLACKHAND, 18, 4,
    "C'est tout ce que vous avez dans le ventre ?",
    "Das ist alles?",
    "¿Es todo lo que pueden exhibir?",
    "И это все?"
),
(
    @REF_BLACKHAND, 19, 0,
    "Je vais mettre moi-même un terme à cette histoire !",
    "Ich bringe es selbst zu Ende!",
    "¡Yo terminaré con esto!",
    "Я сам покончу с вами!"
),
(
    @REF_BLACKHAND, 20, 0,
    "Nous sommes... le bras armé du chef de guerre... sans nous, il...",
    "Wir sind... die Waffe des Kriegshäuptlings... Ohne uns hat er...",
    "Somos... el arma del Jefe de Guerra... Sin nosotros, estaría...",
    "Мы... Мы – орудие вождя... Без нас он..."
),
(
    77343, 0, 0,
    "$n a amorcé une bombe à scories.",
    "$n hat eine Schlackebombe ausgelöst!",
    "$n activó una bomba de escoria.",
    "$n вызывает взрыв шлакобомбы."
);

DELETE FROM creature_groupsizestats WHERE entry = @REF_BLACKHAND;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_BLACKHAND, 14, 10, 54643766),
(@REF_BLACKHAND, 14, 11, 61387477),
(@REF_BLACKHAND, 14, 12, 67988879),
(@REF_BLACKHAND, 14, 13, 74644521),
(@REF_BLACKHAND, 14, 14, 81373044),
(@REF_BLACKHAND, 14, 15, 87999668),
(@REF_BLACKHAND, 14, 16, 94640707),
(@REF_BLACKHAND, 14, 17, 101327605),
(@REF_BLACKHAND, 14, 18, 108022794),
(@REF_BLACKHAND, 14, 19, 114722068),
(@REF_BLACKHAND, 14, 20, 121312499),
(@REF_BLACKHAND, 14, 21, 128001458),
(@REF_BLACKHAND, 14, 22, 133941505),
(@REF_BLACKHAND, 14, 23, 141312442),
(@REF_BLACKHAND, 14, 24, 147990379),
(@REF_BLACKHAND, 14, 25, 154723741),
(@REF_BLACKHAND, 14, 26, 161315817),
(@REF_BLACKHAND, 14, 27, 169085401),
(@REF_BLACKHAND, 14, 28, 174692725),
(@REF_BLACKHAND, 14, 29, 181345379),
(@REF_BLACKHAND, 14, 30, 188009885),
(@REF_BLACKHAND, 15, 10, 69051141),
(@REF_BLACKHAND, 15, 11, 77450362),
(@REF_BLACKHAND, 15, 12, 85863044),
(@REF_BLACKHAND, 15, 13, 94319213),
(@REF_BLACKHAND, 15, 14, 102719147),
(@REF_BLACKHAND, 15, 15, 111148168),
(@REF_BLACKHAND, 15, 16, 119567873),
(@REF_BLACKHAND, 15, 17, 128048616),
(@REF_BLACKHAND, 15, 18, 136391863),
(@REF_BLACKHAND, 15, 19, 144954312),
(@REF_BLACKHAND, 15, 20, 157240219),
(@REF_BLACKHAND, 15, 21, 161647022),
(@REF_BLACKHAND, 15, 22, 170074512),
(@REF_BLACKHAND, 15, 23, 178487898),
(@REF_BLACKHAND, 15, 24, 186917132),
(@REF_BLACKHAND, 15, 25, 197507218),
(@REF_BLACKHAND, 15, 26, 203799546),
(@REF_BLACKHAND, 15, 27, 212235586),
(@REF_BLACKHAND, 15, 28, 220623680),
(@REF_BLACKHAND, 15, 29, 229160164),
(@REF_BLACKHAND, 15, 30, 237511981);

DELETE FROM creature_groupsizestats WHERE entry = 77342;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(77342, 14, 10, 3557075),
(77342, 14, 11, 3968415),
(77342, 14, 12, 4379755),
(77342, 14, 13, 4791095),
(77342, 14, 14, 5202435),
(77342, 14, 15, 5613775),
(77342, 14, 16, 6025115),
(77342, 14, 17, 6436455),
(77342, 14, 18, 6847795),
(77342, 14, 19, 7259135),
(77342, 14, 20, 7670475),
(77342, 14, 21, 8081815),
(77342, 14, 22, 8493155),
(77342, 14, 23, 8904495),
(77342, 14, 24, 9315835),
(77342, 14, 25, 9727175),
(77342, 14, 26, 10138515),
(77342, 14, 27, 10549855),
(77342, 14, 28, 10961195),
(77342, 14, 29, 11372535),
(77342, 14, 30, 11783875),
(77342, 15, 10, 4523818),
(77342, 15, 11, 4998818),
(77342, 15, 12, 5473818),
(77342, 15, 13, 5948818),
(77342, 15, 14, 6423818),
(77342, 15, 15, 6898818),
(77342, 15, 16, 7373818),
(77342, 15, 17, 7848818),
(77342, 15, 18, 8323818),
(77342, 15, 19, 8798818),
(77342, 15, 20, 9273818),
(77342, 15, 21, 9748818),
(77342, 15, 22, 10223818),
(77342, 15, 23, 10698818),
(77342, 15, 24, 11173818),
(77342, 15, 25, 11648818),
(77342, 15, 26, 12123818),
(77342, 15, 27, 12598818),
(77342, 15, 28, 13073818),
(77342, 15, 29, 13548818),
(77342, 15, 30, 14023818);

DELETE FROM creature_groupsizestats WHERE entry = 77665;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(77665, 14, 10, 60028),
(77665, 14, 11, 67891),
(77665, 14, 12, 75754),
(77665, 14, 13, 83617),
(77665, 14, 14, 91480),
(77665, 14, 15, 99343),
(77665, 14, 16, 107206),
(77665, 14, 17, 115069),
(77665, 14, 18, 122932),
(77665, 14, 19, 130795),
(77665, 14, 20, 138658),
(77665, 14, 21, 146521),
(77665, 14, 22, 154384),
(77665, 14, 23, 162247),
(77665, 14, 24, 170110),
(77665, 14, 25, 177973),
(77665, 14, 26, 185836),
(77665, 14, 27, 193699),
(77665, 14, 28, 201562),
(77665, 14, 29, 209425),
(77665, 14, 30, 217288),
(77665, 15, 10, 93461),
(77665, 15, 11, 106274),
(77665, 15, 12, 119087),
(77665, 15, 13, 131900),
(77665, 15, 14, 144713),
(77665, 15, 15, 157526),
(77665, 15, 16, 170339),
(77665, 15, 17, 183152),
(77665, 15, 18, 195965),
(77665, 15, 19, 208778),
(77665, 15, 20, 221591),
(77665, 15, 21, 234404),
(77665, 15, 22, 247217),
(77665, 15, 23, 260030),
(77665, 15, 24, 272843),
(77665, 15, 25, 285656),
(77665, 15, 26, 298469),
(77665, 15, 27, 311282),
(77665, 15, 28, 324095),
(77665, 15, 29, 336908),
(77665, 15, 30, 349721);