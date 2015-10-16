SET NAMES utf8;

SET @REF_HEART_OF_THE_MOUNTAIN = 76806;
SET @REF_FOREMAN_FELDSPAR = 76809;
SET @REF_BLACKHAND_COSMETIC = 76831;
SET @REF_PRIMAL_ELEMENTALIST = 76815;

DELETE FROM creature_text WHERE entry IN (@REF_HEART_OF_THE_MOUNTAIN, @REF_FOREMAN_FELDSPAR, @REF_BLACKHAND_COSMETIC, @REF_PRIMAL_ELEMENTALIST, 77504);
INSERT INTO creature_text VALUES
(@REF_HEART_OF_THE_MOUNTAIN, 0, 0, 'Freeeedommmmm!', 14, 0, 100, 0, 0, 43751, 'Phase3Freedom'),
(@REF_HEART_OF_THE_MOUNTAIN, 1, 0, 'I will reduce you all to ashes!', 14, 0, 100, 0, 0, 43749, 'HeatLevelRising1'),
(@REF_HEART_OF_THE_MOUNTAIN, 1, 1, 'My flames will lick the flesh from your bones!', 14, 0, 100, 0, 0, 43758, 'HeatLevelRising2'),
(@REF_HEART_OF_THE_MOUNTAIN, 2, 0, 'SUFFER!', 14, 0, 100, 0, 0, 43759, 'Blast'),
(@REF_HEART_OF_THE_MOUNTAIN, 3, 0, 'Drown in fire!', 14, 0, 100, 0, 0, 43757, 'Melt'),
(@REF_HEART_OF_THE_MOUNTAIN, 4, 0, 'I consume all!', 14, 0, 100, 0, 0, 43752, 'Slay1'),
(@REF_HEART_OF_THE_MOUNTAIN, 4, 1, 'Die!', 14, 0, 100, 0, 0, 43753, 'Slay2'),
(@REF_HEART_OF_THE_MOUNTAIN, 4, 2, 'Burn!', 14, 0, 100, 0, 0, 43754, 'Slay3'),
(@REF_HEART_OF_THE_MOUNTAIN, 5, 0, 'I will burn this mountain unto ash!', 14, 0, 100, 0, 0, 43755, 'Wipe'),
(@REF_HEART_OF_THE_MOUNTAIN, 6, 0, 'My fire is... Exting..uished.', 14, 0, 100, 0, 0, 43750, 'Death'),
(@REF_HEART_OF_THE_MOUNTAIN, 7, 0, '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tThe Heart of the Mountain is about to release a |cFFFF0000|Hspell:155209|h[Blast]|h|r of intense heat!', 41, 0, 100, 0, 0, 0, 'BlastWarning'),
(@REF_HEART_OF_THE_MOUNTAIN, 8, 0, '|TInterface\\Icons\\spell_sandexplosion.blp:20|tYou have a |cFFFF0000|Hspell:155187|h[Bomb]|h|r on you!', 42, 0, 100, 0, 0, 0, 'BombWarning'),
(@REF_HEART_OF_THE_MOUNTAIN, 9, 0, '|TInterface\\Icons\\Spell_Fire_LavaSpawn.blp:20|tA Firecaller has affected you with |cFFFF0000|Hspell:176121|h[Volatile Fire]|h|r!', 42, 0, 100, 0, 0, 0, 'VolatileFireWarning'),
(@REF_HEART_OF_THE_MOUNTAIN, 10, 0, '|TInterface\\Icons\\ability_foundryraid_melt.blp:20|tThe Heart of the Mountain is going to |cFFFF0000|Hspell:155225|h[Melt]|h|r the ground under your feet!', 42, 0, 100, 0, 0, 0, 'MeltWarning'),

(@REF_FOREMAN_FELDSPAR, 0, 0, 'Protect the furnace at all costs!', 14, 0, 100, 0, 0, 44390, 'Aggro'),
(@REF_FOREMAN_FELDSPAR, 1, 0, 'Get those billows pumping!', 14, 0, 100, 0, 0, 44392, 'BellowsOperator1'),
(@REF_FOREMAN_FELDSPAR, 1, 1, 'Turn up the heat boys, we''ll sweat ''em out!', 14, 0, 100, 0, 0, 44393, 'BellowsOperator2'),
(@REF_FOREMAN_FELDSPAR, 2, 0, 'Call in more security!', 14, 0, 100, 0, 0, 44394, 'FirstHeatRegulatorDestroyed'),
(@REF_FOREMAN_FELDSPAR, 3, 0, 'I will crush you!', 14, 0, 100, 0, 0, 44399, 'Ability1'),
(@REF_FOREMAN_FELDSPAR, 3, 1, 'Die scum!', 14, 0, 100, 0, 0, 44400, 'Ability2'),
(@REF_FOREMAN_FELDSPAR, 4, 0, 'Weak!', 14, 0, 100, 0, 0, 44396, 'Slay1'),
(@REF_FOREMAN_FELDSPAR, 4, 1, 'Pathetic!', 14, 0, 100, 0, 0, 44397, 'Slay2'),
(@REF_FOREMAN_FELDSPAR, 5, 0, 'Can''t take the heat? Get out of the furnace!', 14, 0, 100, 0, 0, 44398, 'Wipe'),
(@REF_FOREMAN_FELDSPAR, 6, 0, 'awww...aaaaggg...', 14, 0, 100, 0, 0, 44391, 'Death'),
(@REF_FOREMAN_FELDSPAR, 7, 0, '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tThe frequency of the Heart of the Mountain''s |cFFFF0000|Hspell:155209|h[Blast]|h|r has been increased!', 41, 0, 100, 0, 0, 0, 'BlastIncreased'),
(@REF_FOREMAN_FELDSPAR, 8, 0, '|TInterface\\Icons\\ability_rhyolith_magmaflow_whole.blp:20|tForeman Feldspar is going to create a |cFFFF0000|Hspell:156934|h[Rupture]|h|r under your feet!', 42, 0, 100, 0, 0, 0, 'RuptureWarning'),

(@REF_BLACKHAND_COSMETIC, 0, 0, 'Guards! Intruders are meddling with the Blast Furnace!', 14, 0, 100, 0, 0, 45466, 'Intro'),
(@REF_BLACKHAND_COSMETIC, 1, 0, 'One Pressure Regulator has been destroyed. One remains.', 41, 0, 100, 0, 0, 0, 'FirstHeatRegulatorDestroyedBefore'),
(@REF_BLACKHAND_COSMETIC, 2, 0, 'The machinery is damaged. Keep those scum from sabotaging the furnace!', 14, 0, 100, 0, 0, 45463, 'FirstHeatRegulatorDestroyedAfter'),
(@REF_BLACKHAND_COSMETIC, 3, 0, 'Both Heat Regulators have been destroyed. The Heart of the Mountain is unleashed!', 41, 0, 100, 0, 0, 0, 'HeartExposedBefore'),
(@REF_BLACKHAND_COSMETIC, 4, 0, 'The furnace is overheating! Fools! If the fury breaks free, our work will be ruined!', 14, 0, 100, 0, 0, 45464, 'HeartExposedAfter'),
(@REF_BLACKHAND_COSMETIC, 5, 0, 'Defeat the Primal Elementalists to release the Heart of the Mountain from its bindings.', 41, 0, 100, 0, 0, 0, 'Phase2'),
(@REF_BLACKHAND_COSMETIC, 6, 0, 'Three Primal Elementalists remain.', 41, 0, 100, 0, 0, 0, '3PrimalRemaining'),
(@REF_BLACKHAND_COSMETIC, 7, 0, 'Two Primal Elementalists remain.', 41, 0, 100, 0, 0, 0, '2PrimalRemaining'),
(@REF_BLACKHAND_COSMETIC, 8, 0, 'One Primal Elementalist remains.', 41, 0, 100, 0, 0, 0, '1PrimalRemaining'),
(@REF_BLACKHAND_COSMETIC, 9, 0, 'All Primal Elementalists have been defeated. The Heart of the Mountain is free!', 41, 0, 100, 0, 0, 0, 'Phase3Freedom1'),
(@REF_BLACKHAND_COSMETIC, 10, 0, 'A small victory, but now you will perish in the flame that burns at the heart of Blackrock!', 14, 0, 100, 0, 0, 45465, 'Phase3Freedom2'),
(@REF_BLACKHAND_COSMETIC, 11, 0, 'What happened to my Furnace? Where are my guards? Shut these intruders down!', 14, 0, 100, 0, 0, 45460, 'HeartDeath'),

(@REF_PRIMAL_ELEMENTALIST, 0, 0, 'The fury has broken free!', 14, 0, 100, 0, 0, 42891, 'HeartExposed1'),
(@REF_PRIMAL_ELEMENTALIST, 1, 0, 'Quick, get it under control!', 14, 0, 100, 0, 0, 42892, 'HeartExposed2'),
(@REF_PRIMAL_ELEMENTALIST, 2, 0, 'Warlord, we have it under control.', 14, 0, 100, 0, 0, 42893, 'HeartExposed3'),
(@REF_PRIMAL_ELEMENTALIST, 3, 0, 'It... is free. Warlord, we have failed...', 14, 0, 100, 0, 0, 42894, 'Phase3Freedom'),

(77504, 0, 0, '%s casts |cFFFF0000|Hspell:156446|h[Blast Wave]|h|r! Hide!', 41, 0, 100, 0, 0, 0, 'BlastWave');

DELETE FROM locales_creature_text WHERE entry IN (@REF_HEART_OF_THE_MOUNTAIN, @REF_FOREMAN_FELDSPAR, @REF_BLACKHAND_COSMETIC, @REF_PRIMAL_ELEMENTALIST, 77504);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_HEART_OF_THE_MOUNTAIN, 0, 0,
    'Je suis liiiiiibre !',
    'Endlich frei!',
    '¡Libertaaaaad!',
    'Свобо-о-о-о-ода!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 1, 0,
    'Je vous réduirai tous en cendres !',
    'Ich werde Euch alle einäschern!',
    '¡Los haré a todos cenizas!',
    'Я обращу вас в пепел!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 1, 1,
    'Mes flammes dévoreront vos chairs !',
    'Mein Feuer wird das Fleisch von Euren Knochen brennen!',
    '¡Mis llamas lamerán la carne de sus huesos!',
    'Мой огонь обглодает плоть с ваших костей!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 2, 0,
    'SOUFFREZ !',
    'LEIDET!',
    '¡SUFRAN!',
    'СТРАДАЙТЕ!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 3, 0,
    'Que le feu vous engloutisse !',
    'Ertrinkt in den Flammen!',
    '¡Que el fuego los asfixie!',
    'Горите!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 4, 0,
    'Je suis le feu !',
    'Ich verschlinge alles!',
    '¡Consumiré todo!',
    'Я поглощу все!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 4, 1,
    'À mort !',
    'Sterbt!',
    '¡Mueran!',
    'Умри!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 4, 2,
    'Brûlez !',
    'Brennt!',
    '¡Ardan!',
    'Горите!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 5, 0,
    'Je ferai de cette montagne un tas de cendres !',
    'Ich lege diesen Berg in Schutt und Asche!',
    '¡Quemaré esta montaña hasta las cenizas!',
    'Я спалю эту гору дотла!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 6, 0,
    'Mes flammes... Elles meurent...',
    'Mein Feuer wurde... ausgelöscht.',
    'Mi fuego está... extin... guido.',
    'Мой огонь... угас.'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 7, 0,
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tLe Cœur de la montagne est sur le point de libérer une |cFFFF0000|Hspell:155209|h[Explosion]|h|r d''intense chaleur !',
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tDas Herz des Berges wird gleich eine |cFFFF0000|Hspell:155209|h[Flammenzunge]|h|r gewaltiger Hitze entfesseln!',
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|t¡El Corazón de la Montaña está a punto de liberar un |cFFFF0000|Hspell:155209|h[Estallido]|h|r de calor intenso!',
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tСердце Горы вот-вот устроит |cFFFF0000|Hspell:155209|h[Взрыв]|h|r нестерпимого жара!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 8, 0,
    '|TInterface\\Icons\\spell_sandexplosion.blp:20|tVous portez une |cFFFF0000|Hspell:155187|h[Bombe]|h|r !',
    '|TInterface\\Icons\\spell_sandexplosion.blp:20|tIhr tragt eine |cFFFF0000|Hspell:155187|h[Bombe]|h|r!',
    '|TInterface\\Icons\\spell_sandexplosion.blp:20|t¡Tienes una |cFFFF0000|Hspell:155187|h[Bomba]|h|r sobre ti!',
    '|TInterface\\Icons\\spell_sandexplosion.blp:20|tНа вас наложили заклинание |cFFFF0000|Hspell:155187|h[Бомба]|h|r!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 9, 0,
    '|TInterface\\Icons\\Spell_Fire_LavaSpawn.blp:20|tUn mandefeu vous a lancé |cFFFF0000|Hspell:176121|h[Feu volatil]|h|r !',
    '|TInterface\\Icons\\Spell_Fire_LavaSpawn.blp:20|tEin Feuerrufer hat Euch mit |cFFFF0000|Hspell:176121|h[flüchtigem Feuer]|h|r belegt!',
    '|TInterface\\Icons\\Spell_Fire_LavaSpawn.blp:20|t¡Un clamafuegos te afectó con |cFFFF0000|Hspell:176121|h[Fuego volátil]|h|r!',
    '|TInterface\\Icons\\Spell_Fire_LavaSpawn.blp:20|tПризыватель Огня наложил на вас заклинание |cFFFF0000|Hspell:176121|h[Неустойчивый огонь]|h|r!'
),
(
    @REF_HEART_OF_THE_MOUNTAIN, 10, 0,
    '|TInterface\\Icons\\ability_foundryraid_melt.blp:20|tLe Cœur de la montagne va |cFFFF0000|Hspell:155225|h[Fondre]|h|r le sol sous vos pieds !',
    '|TInterface\\Icons\\ability_foundryraid_melt.blp:20|tDas Herz des Berges wird den Boden unter Euren Füßen |cFFFF0000|Hspell:155225|h[schmelzen]|h|r!',
    '|TInterface\\Icons\\ability_foundryraid_melt.blp:20|t¡El Corazón de la Montaña va a |cFFFF0000|Hspell:155225|h[Fundir]|h|r el suelo bajo tus pies!',
    '|TInterface\\Icons\\ability_foundryraid_melt.blp:20|tСердце Горы собирается применить |cFFFF0000|Hspell:155225|h[Растапливание]|h|r к земле под вашими ногами!'
),
(
    @REF_FOREMAN_FELDSPAR, 0, 0,
    'Protégez le haut fourneau à tout prix !',
    'Schützt den Ofen um jeden Preis!',
    '¡Protejan el horno a toda costa!',
    'Защищайте горнило любой ценой!'
),
(
    @REF_FOREMAN_FELDSPAR, 1, 0,
    'Actionnez-moi ces soufflets !',
    'An die Blasebälge!',
    '¡Pongan a funcionar esos fuelles!',
    'Раздуть меха!'
),
(
    @REF_FOREMAN_FELDSPAR, 1, 1,
    'Faites grimper la température, les gars, ça les réchauffera !',
    'Macht ihnen Feuer unterm Hintern!',
    '¡Aumenten el calor! ¡Sudarán hasta el fin!',
    'Поддайте жару, парни, пусть попотеют!'
),
(
    @REF_FOREMAN_FELDSPAR, 2, 0,
    'Appelez des renforts !',
    'Ruft Verstärkung herbei!',
    '¡Llamen a más guardias!',
    'Где вся охрана?!'
),
(
    @REF_FOREMAN_FELDSPAR, 3, 0,
    'Je vais vous écrabouiller !',
    'Ich werd Euch zermalmen!',
    '¡Los aplastaré!',
    'Раздавлю!'
),
(
    @REF_FOREMAN_FELDSPAR, 3, 1,
    'Meurs, vermine !',
    'Sterbt, dreckiges Pack!',
    '¡Muere, basura!',
    'Умрите!'
),
(
    @REF_FOREMAN_FELDSPAR, 4, 0,
    'Ridicule !',
    'Schwach!',
    '¡Débil!',
    'Кишка тонка!'
),
(
    @REF_FOREMAN_FELDSPAR, 4, 1,
    'Pitoyable !',
    'Erbärmlich!',
    '¡Patético!',
    'Ничтожество!'
),
(
    @REF_FOREMAN_FELDSPAR, 5, 0,
    'Vous avez chaud ? Alors sortez !',
    'Zu heiß für Euch? Dann spielt nicht mit dem Feuer!',
    '¿No soportan el calor? ¡Salgan del horno!',
    'Слишком жарко? Убирайтесь из горнила!'
),
(
    @REF_FOREMAN_FELDSPAR, 6, 0,
    'arrr... aaaaggg...',
    'Aaaahhrrrg...',
    'ooohhh... aaagghhh...',
    'А-а-а-а... а-а-а-х-х-х...'
),
(
    @REF_FOREMAN_FELDSPAR, 7, 0,
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tLa fréquence des |cFFFF0000|Hspell:155209|h[Explosions]|h|r du Cœur de la montagne augmente !',
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tDie Frequenz der |cFFFF0000|Hspell:155209|h[Flammenzungen]|h|r des Herzens des Berges wurde erhöht!',
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|t¡Aumentó la frecuencia de |cFFFF0000|Hspell:155209|h[Reventar]|h|r del Corazón de la Montaña!',
    '|TInterface\\Icons\\spell_shaman_improvedfirenova.blp:20|tЧастота |cFFFF0000|Hspell:155209|h[взрывов]|h|r Сердца Горы увеличилась!'
),
(
    @REF_FOREMAN_FELDSPAR, 8, 0,
    '|TInterface\\Icons\\ability_rhyolith_magmaflow_whole.blp:20|tLe contremaître Feldspath s''apprête à créer une |cFFFF0000|Hspell:156934|h[Rupture]|h|r sous vos pieds !',
    '|TInterface\\Icons\\ability_rhyolith_magmaflow_whole.blp:20|tVorarbeiter Feldspat wird einen |cFFFF0000|Hspell:156934|h[Bodenriss]|h|r unter Euch erzeugen!',
    '|TInterface\\Icons\\ability_rhyolith_magmaflow_whole.blp:20|t¡El supervisor Feldespato va a crear una |cFFFF0000|Hspell:156934|h[Ruptura]|h|r bajo tus pies!',
    '|TInterface\\Icons\\ability_rhyolith_magmaflow_whole.blp:20|tШтейгер Полевой Шпат создает |cFFFF0000|Hspell:156934|h[Разлом]|h|r у вас под ногами!'
),
(
    @REF_BLACKHAND_COSMETIC, 0, 0,
    'Gardes ! Des intrus envahissent le haut fourneau !',
    'Wachen! Eindringlinge machen sich am Schmelzofen zu schaffen!',
    '¡Guardias! ¡Los intrusos están interfiriendo con el Alto horno!',
    'Стража! Чужаки вторглись в Горнило!'
),
(
    @REF_BLACKHAND_COSMETIC, 1, 0,
    'Un régulateur de pression a été détruit. Il n''en reste plus qu''un.',
    'Ein Hitzeregler wurde zerstört, ein weiterer ist noch da.',
    'Se destruyó un regulador de presión. Falta otro.',
    'Один регулятор давления уничтожен, остался еще один.'
),
(
    @REF_BLACKHAND_COSMETIC, 2, 0,
    'La machinerie est endommagée. Empêchez cette vermine de saboter le fourneau !',
    'Der Mechanismus ist beschädigt. Lasst diesen Abschaum nicht den Ofen sabotieren!',
    'La maquinaria está dañada. ¡Eviten que esos malditos saboteen el horno!',
    'Оборудование повреждено. Не дайте им вывести из строя Горнило!'
),
(
    @REF_BLACKHAND_COSMETIC, 3, 0,
    'Les deux régulateurs de chaleur ont été détruits. Le Cœur de la montagne a été libéré !',
    'Beide Hitzeregler wurden zerstört. Das Herz des Berges ist freigesetzt!',
    'Se destruyeron ambos reguladores de calor. ¡Se desató el Corazón de la Montaña!',
    'Оба регулятора температуры уничтожены. Сердце Горы на свободе!'
),
(
    @REF_BLACKHAND_COSMETIC, 4, 0,
    'La fournaise est en surchauffe ! Imbéciles ! Si la fureur s''échappe, tout notre travail sera anéanti !',
    'Der Schmelzofen überhitzt! Ihr Narren! Wenn der Zorn ausbricht, macht er alles zunichte!',
    '¡El horno se está sobrecalentando! Tontos, si la furia es liberada, ¡nuestro trabajo se habrá arruinado!',
    'Горнило перегревается! Если Гнев вырвется на свободу, все пропало!'
),
(
    @REF_BLACKHAND_COSMETIC, 5, 0,
    'Triomphez des élémentalistes primordiaux pour libérer le Cœur de la montagne de ses entraves.',
    'Besiegt die Urelementaristen, um das Herz des Berges von seinen Fesseln zu befreien.',
    'Derrota a los elementalistas primigenios para liberar al Corazón de la Montaña de sus ataduras.',
    'Расправьтесь с повелительницами изначальных стихий, чтобы высвободить Сердце Горы.'
),
(
    @REF_BLACKHAND_COSMETIC, 6, 0,
    'Il reste trois élémentalistes primordiaux.',
    'Noch drei Urelementaristen.',
    'Faltan tres elementalistas primigenios.',
    'Осталось три повелительницы изначальных стихий.'
),
(
    @REF_BLACKHAND_COSMETIC, 7, 0,
    'Il reste deux élémentalistes primordiaux.',
    'Noch zwei Urelementaristen.',
    'Faltan dos elementalistas primigenios.',
    'Осталось две повелительницы изначальных стихий.'
),
(
    @REF_BLACKHAND_COSMETIC, 8, 0,
    'Il reste un élémentaliste primordial.',
    'Noch ein Urelementarist.',
    'Falta una elementalista primigenia.',
    'Осталась одна повелительница изначальных стихий.'
),
(
    @REF_BLACKHAND_COSMETIC, 9, 0,
    'Tous les élémentalistes primordiaux ont été vaincus. Le Cœur de la montagne n''est plus entravé !',
    'Alle Urelementaristen wurden besiegt. Das Herz des Berges ist frei!',
    'Derrotaste a todos los elementalistas primigenios. ¡El Corazón de la Montaña es libre!',
    'Все повелительницы изначальных стихий повержены, Сердце Горы на свободе.'
),
(
    @REF_BLACKHAND_COSMETIC, 10, 0,
    'Savourez cette petite victoire, maintenant vous allez périr dans les flammes qui rugissent dans le cœur de Rochenoire !',
    'Ein unbedeutender Sieg. Denn jetzt werdet Ihr in den Flammen im Herzen des Schwarzfels aufgehen!',
    'Una pequeña victoria, ¡pero ahora perecerás en la llama que arde en el corazón de Roca Negra!',
    'Скромная победа... а теперь сгорите в пламени сердца Черной горы!'
),
(
    @REF_BLACKHAND_COSMETIC, 11, 0,
    'Qu''est-il arrivé à mon fourneau ? Où sont mes gardes ? Débarrassez-moi de ces intrus !',
    'Was ist mit meiner Gießerei passiert? Wo sind die Wachen? Schiebt diesen Eindringlingen einen Riegel vor!',
    '¿Qué le ha pasado a mi horno? ¿Dónde están mis guardias? ¡Encierren a estos intrusos!',
    'Что случилось с моим горнилом? Где стража? Убейте этих чужаков!'
),
(
    @REF_PRIMAL_ELEMENTALIST, 0, 0,
    'La Fureur est sortie !',
    'Der Zorn ist ausgebrochen!',
    '¡La Furia se ha liberado!',
    'Гнев на свободе!'
),
(
    @REF_PRIMAL_ELEMENTALIST, 1, 0,
    'Vite, elle ne doit pas nous échapper !',
    'Schnell, bringt ihn unter Kontrolle!',
    '¡Rápido, tómenla bajo su control!',
    'Скорее подчините его!'
),
(
    @REF_PRIMAL_ELEMENTALIST, 2, 0,
    'Seigneur de guerre, tout est sous contrôle.',
    'Kriegsherr, wir haben ihn unter Kontrolle.',
    'Señor de la Guerra, lo tenemos bajo control.',
    'Вождь, у нас все под контролем.'
),
(
    @REF_PRIMAL_ELEMENTALIST, 3, 0,
    'Elle... s''est libérée. Nous avons échoué...',
    'Er... er ist frei. Kriegsherr, wir haben versagt...',
    'Está... libre. Señor de la Guerra, hemos fracasado...',
    'Он... вырвался. Мы подвели тебя...'
),
(
    77504, 0, 0,
    '%s incante une |cFFFF0000|Hspell:156446|h[Vague Explosive]|h|r ! Cachez-vous !',
    '%s entfesstelt eine |cFFFF0000|Hspell:156446|h[Druckwelle]|h|r! In Deckung!',
    '¡%s está lanzando una |cFFFF0000|Hspell:156446|h[Ola Explosiva]|h|r! ¡Escondéos!',
    '%s использует |cFFFF0000|Hspell:156446|h[Взрывную волну]|h|r! Прячься!'
);

DELETE FROM spell_script_names WHERE spell_id IN (156345, 158424, 160379, 155192, 174716, 156934, 158247, 155176, 158345, 176121, 155225);
INSERT INTO spell_script_names VALUES
(156345, 'spell_foundry_ignite_aura'),
(158424, 'spell_foundry_blast_wave'),
(160379, 'spell_foundry_defense_aura'),
(155192, 'spell_foundry_bomb_overrider'),
(174716, 'spell_foundry_bomb_overrider'),
(156934, 'spell_foundry_rupture_aura'),
(158247, 'spell_foundry_hot_blooded_aura'),
(155176, 'spell_foundry_damage_shield'),
(158345, 'spell_foundry_shields_down'),
(176121, 'spell_foundry_volatile_fire'),
(155225, 'spell_foundry_melt_aura');

DELETE FROM gameobject_template WHERE entry = 227616;
INSERT INTO gameobject_template VALUE
(227616, 0, 10267, 'Volcanic Bomb', '', '', '', 16, 32, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 19865);

DELETE FROM conditions WHERE SourceEntry IN (155180, 155188, 155265, 155239, 177434);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 155180, 31, 0, 3, 76806),
(13, 1, 155188, 31, 0, 3, 76808),
(13, 1, 155265, 31, 0, 3, 76806),
(13, 1, 155239, 31, 0, 3, 78463),
(13, 1, 177434, 31, 0, 3, 76806);

DELETE FROM areatrigger_template WHERE spell_id IN (156933, 155224);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, flags, scale_x, scale_y, morph_curve_id, ScriptName) VALUES
(156933, 0, 6432, 2, 0x4080, 6, 6, 513, 'areatrigger_foundry_rupture'),
(155224, 0, 6221, 2, 0x4080, 1, 5, 1120, 'areatrigger_foundry_melt');

DELETE FROM areatrigger_template WHERE spell_id = 155738;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, type, flags, data0, data1, data6, ScriptName) VALUE
(155738, 0, 6269, 0, 0x1000, -16.86198, 5.687012, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -18.87587, 5.460449, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -19.24045, 4.552490, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -18.57031, 3.417969, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -19.13976, 1.558594, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -19.83247, 1.338135, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -22.77344, 1.569824, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -22.61719, 0.439697, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -21.65712, -0.74097, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -20.79948, -3.80957, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -17.78038, -2.82764, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -17.12587, -2.96582, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -15.30122, -4.73999, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -15.06857, -6.34424, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -15.44705, -8.13843, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -13.08247, -10.1707, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -9.546005, -11.3738, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -6.610245, -12.3972, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -6.323792, -15.8867, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -7.138016, -16.9380, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -7.257813, -18.0171, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -5.002609, -19.8147, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -3.674484, -17.3565, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -2.143234, -17.5464, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -1.318573, -16.7593, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 0.5269012, -16.8120, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 0.8463593, -15.7505, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 6.1258700, -15.0335, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 7.9262080, -21.8572, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 10.644960, -20.7705, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 10.865450, -19.6480, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 12.639760, -19.1567, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 9.7908020, -14.5447, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 8.6796880, -14.1567, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 11.855030, -10.3008, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 11.662320, -9.21240, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 14.634550, -4.94678, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 19.167530, -7.36499, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 19.912320, -1.71582, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 22.773440, -1.14209, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 22.381070, -0.16626, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 22.610240, 1.986572, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 21.811630, 2.655762, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 22.099820, 4.718262, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 20.302950, 4.640137, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 19.919270, 6.360596, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 18.830730, 7.254639, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 17.891490, 9.407471, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 18.311630, 10.52393, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 20.804690, 11.91699, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 19.603300, 12.63745, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 18.690110, 14.38501, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 17.375870, 14.60889, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 16.356770, 15.91260, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 15.177950, 14.94824, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 14.254330, 14.98486, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 13.523440, 15.69312, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 11.953990, 15.75830, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 10.052950, 16.84058, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 9.7908020, 17.88574, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 11.077260, 20.16187, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 9.6692660, 19.80688, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 7.8307340, 21.00391, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 5.6883700, 20.24072, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, 1.2855830, 19.45874, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -4.431427, 19.98657, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -9.042542, 21.85718, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -10.69531, 18.03857, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -9.702255, 17.48999, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -8.757813, 15.89526, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -9.438370, 14.43433, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -9.271698, 12.97168, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -10.64323, 10.74854, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -11.65192, 7.027344, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -13.01823, 4.738281, 76, 'areatrigger_foundry_slag_pool'),
(155738, 0, 6269, 0, 0x1000, -14.78038, 4.577637, 76, 'areatrigger_foundry_slag_pool');