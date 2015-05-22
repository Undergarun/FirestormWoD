UPDATE creature_template SET npcFlag=npcFlag | 131072 WHERE entry=79862;
DELETE FROM garrison_plot_content WHERE plot_type_or_building=-51 AND faction_index = 0;
insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-51','0','79862','1.66546','-0.009018','1.31065','-0.01372');