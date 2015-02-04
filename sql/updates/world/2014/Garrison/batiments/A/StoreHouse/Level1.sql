UPDATE creature_template SET npcFlag=npcFlag | 131072 WHERE entry=84857;
DELETE FROM garrison_plot_content WHERE plot_type_or_building=-51 AND faction_index = 1;
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-51','1','84857','4.80532','-0.103392','1.29651','0.045699');

DELETE FROM creature_questrelation WHERE id=84857;
INSERT INTO `creature_questrelation`(`id`,`quest`) VALUES ( '84857','37087');