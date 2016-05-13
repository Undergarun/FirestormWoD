UPDATE `creature_template` SET `unit_class`=2 WHERE `entry` IN (45213, 49744);
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=45688;
UPDATE `creature_template` SET `ScriptName`='boss_sinestra' WHERE `entry`=45213;

DELETE FROM `locales_creature_text` WHERE `entry`=45993;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(45993, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Господин отдал приказ тебе, Валиона. Я слишком занят, чтобы с кем-то воевать.'),
(45993, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я бесполезен?! Сейчас ты увидишь, почему мать всегда любила меня больше, чем тебя!'),
(45993, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Дай мне взглянуть на твою душу.'),
(45993, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я так и знал – не надо было связываться...'),
(45993, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ты мне не указ, Валиона! Я сам подожгу все, что нужно!'),
(45993, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вам не уйти от ярости сумеречных драконов!'),
(45993, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'УМРИТЕ В МУКАХ!');

DELETE FROM `locales_creature_text` WHERE `entry`=45992;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(45992, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Выполни волю хозяина, Тералион! Убей их!'),
(45992, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ты бесполезен, Тералион!'),
(45992, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Это ваш последний миг!'),
(45992, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Одно утешает... Тералион погиб вместе со мной...'),
(45992, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Тералион, я подожгу коридор. Не дай им уйти!'),
(45992, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вас поглотит сумрак!');

DELETE FROM `locales_creature_text` WHERE `entry`=43687;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43687, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Как вы посмели вторгнуться в святилище нашего господина?'),
(43687, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Умри!'),
(43687, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я заморожу кровь в ваших жилах!'),
(43687, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'А теперь узрите истинное…');

DELETE FROM `locales_creature_text` WHERE `entry`=43686;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43686, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы заплатите жизнями за свою дерзость!'),
(43686, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Пища для огня.'),
(43686, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'ГОРИТЕ!'),
(43686, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '...могущество стихий!');

DELETE FROM `locales_creature_text` WHERE `entry`=43688;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43688, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Окончим этот фарс!'),
(43688, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Всего лишь дуновение ветра…'),
(43688, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ветер, явись на мой зов!'),
(43688, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ваше упорство...');

DELETE FROM `locales_creature_text` WHERE `entry`=43689;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43689, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Твои кости истлеют в земле.'),
(43689, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мы займемся ими!'),
(43689, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Земля поглотит вас!'),
(43689, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '...поражает нас.');

DELETE FROM `locales_creature_text` WHERE `entry`=43735;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43735, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'ВОТ ВАША СМЕРТЬ!'),
(43735, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Уничтожать...'),
(43735, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Истреблять...'),
(43735, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Невозможно...'),
(43735, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'ВОТ ВАША СМЕРТЬ!');

DELETE FROM `locales_creature_text` WHERE `entry`=43324;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43324, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Кровь для господина!)'),
(43324, 1, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Таков твой удел.'),
(43324, 1, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Слабая плоть!'),
(43324, 1, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Кончай их! Убей их всех!)'),
(43324, 1, 4, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Примите свою смерть.'),
(43324, 1, 5, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Дух, плоть и разум тьма поглотит разом.)'),
(43324, 1, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Теперь они в руках господина.'),
(43324, 1, 7, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Хаос, безумие! Услада для мозга!)'),
(43324, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Надели меня силой!'),
(43324, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Узрите мощь Молота!'),
(43324, 2, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(А теперь – время Молота!)'),
(43324, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Довольно! (Все склонится перед ним, перед Хаосом самим!) Да, именно так.'),
(43324, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Бесконечное страдание!)'),
(43324, 4, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(безумный смех с похрюкиванием)'),
(43324, 4, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Смотрите, как погибает ваш хрупкий мир!'),
(43324, 4, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Еще. Еще!'),
(43324, 4, 4, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Придите, братья! Отнимите надежду!)'),
(43324, 4, 5, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Еще. Им нужно еще.)'),
(43324, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'В конце концов все сдаются.'),
(43324, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Теперь они видят тьму! У них открылись глаза!)'),
(43324, 7, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Брат, этого не может быть… (Час пробил!) Моя сила... (Слишком гордые! Прекрасная речь! Слушайте!) Мой разум... (Мы служим во славу Его!) Что... происходит… (Рассвет померк. Придите, тени сумерек. Приди, безумие! Приди, смута! Приди, вечная ночь!)'),
(43324, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Когти судьбы, разорвите нечестивцев!)'),
(43324, 8, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Господин видит, Господин видит!)'),
(43324, 8, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Око Господина видит все, что было, что будет и чего никогда не было!)'),
(43324, 9, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Что за безумие мы учинили? (Еще не учинили!)'),
(43324, 10, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я... больше не могу… (Брат, это только начало!)'),
(43324, 11, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Брат, мы проиграли битву. (Но Господин... я же слышу... злой.. Господин?)'),
(43324, 12, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Брат, мы проиграли битву. Пусть она прикончит их. (Господин не хочет, чтобы они до нее... Брат? Господин в ярости... )');

DELETE FROM `locales_creature_text` WHERE `entry`=46965;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(46965, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы – его избранники... (Он зовет вас, он знает вас.) Сотканные из плоти... (Плоти.) Очищенные кровью... (Кровью.) Рожденные быть бессмертными!'),
(46965, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Внемлите его гласу! Растопчите еретиков!'),
(46965, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Братья Молота, ко мне! (Они все еще владеют собой.) Еретики все еще дышат. Уничтожьте их! (Растерзайте тела, сломите дух!)'),
(46965, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Смертные, вы едва можете отделить зерна от плевел! (Негодные, негодные!)'),
(46965, 9, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Валиона, Тералион, укажите им их место.'),
(46965, 10, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '(Ближе, ближе… Дайте увидеть ваши лица. Все склонятся перед его властью.)'),
(46965, 11, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Приветствую, смертные. (Маленькие беззащитные дети.) Немногим довелось увидеть это святилище. (Они хотят прикоснуться к огню!)'),
(46965, 12, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Здесь начинается жизненный путь избранных… (Уходящий в вечность.) И здесь же закончится ваш. (Смерть плоти. Смерть плоти!)'),
(46965, 13, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Братья Сумерек! Молот взывает к вам! (Огонь, вода, земля, воздух.) Сбросьте свою смертную оболочку. (Огонь, вода, земля, воздух.) Отныне и вечно пребывайте в своем новом облике! (Жгите, топите, крушите, душите!) Используйте свой дар, чтобы уничтожать еретиков. (Жгите, топите, крушите, душите!)'),
(46965, 14, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Глупые смертные... (Узурпаторское отродье!) Все ваши усилия… (Дети низшего бога!) Я тут говорю! (Слова, слова, слова. Господин хочет убивать.) ВСЕ сгинет в хаосе. ВСЕ исчезнет. (Хаос, хаос!) Ваши подвиги ничего не изменят. (Хаос, хаос, всему придет конец.) Смертным нельзя видеть то, что увидели вы. Пришел ваш смертный час!');

DELETE FROM `locales_creature_text` WHERE `entry`=45213;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(45213, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нельзя было доверять этому идиоту Чо\'Галлу такое важное поручение. Я сама с вами разберусь!'),
(45213, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Смертокрыл, я подвела тебя... наш род обречен...'),
(45213, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мои дети обглодают твои кости!'),
(45213, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Слабое существо...'),
(45213, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ты так в этом уверен? Глупец!'),
(45213, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы меня утомляете. Искры силы, вложенные в каждого из моих детей, принадлежат мне. Узрите же мощь, какую вы и представить себе не могли!'),
(45213, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ''),
(45213, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Довольно! Это может затянуться на месяцы. Я окажу тебе честь и потрачу на тебя весь запас энергии. Пора умереть!'),
(45213, 7, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ешьте, дети мои! Пусть их мясо насытит вас!'),
(45213, 8, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Я наделила своих детей силой, и я же заберу ее обратно!'),
(45213, 9, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'СТРАДАНИЕ!'),
(45213, 10, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'ИСПЕПЕЛЯЮЩАЯ НЕНАВИСТЬ!'),
(45213, 11, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Придите, исчадья сумрака!'),
(45213, 12, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ваша мать зовет!');

DELETE FROM `locales_creature_text` WHERE `entry`=46277;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(46277, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Герои, вы не одиноки во тьме!'),
(46277, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Герои, моя сила иссякает...'),
(46277, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Синтария! Ты заплатишь жизнью за поступки своего господина.'),
(46277, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Пламя гаснет, защитники... Возьмите то, что осталось от моей силы. Да сопутствует вам удача там, где я потерпел поражение. Отомстите за меня. Отомстите за весь мир...'),
(46277, 4, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Огонь жизни, разгорись во мне и надели меня силой!'),
(46277, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ты слабеешь, Синтария. Смирись с неизбежным.'),
(46277, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Все пропало... Прости меня, моя королева...');