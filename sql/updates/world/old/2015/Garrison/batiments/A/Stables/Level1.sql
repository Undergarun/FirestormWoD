-- Stables

	-- templates
	
	-- plot content

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -65 AND faction_index = 0; -- horde
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-37','0','79836','6.73913','-0.149402','2.188','6.25459');

	/* Quests list : 
	Dressage de loup : 
	37027
	37026
	37028
	37025
	37022
	37024
	37023
	Dressage de potamodonte : 
	37006
	37012
	37011
	37005
	37013
	37010
	37007
	37009
	37008
	Dressage de Talbuk : 
	36975
	36981
	36973
	36980
	36974
	36971
	36982
	36979
	36976
	36978
	36972
	36977
	La dent de prédateur
	37122
	La griffe noire
	37121
	Lier un loup
	36914
	Prendre un potamodonte
	36918
	Terrasser un Talbuk
	36911*/

	REPLACE INTO creature_queststarter VALUES
	(86974, 37027),
	(86974, 37026),
	(86974, 37028),
	(86974, 37025),
	(86974, 37022),
	(86974, 37024),
	(86974, 37023),
	(86974, 37006),
	(86974, 37012),
	(86974, 37011),
	(86974, 37005),
	(86974, 37013),
	(86974, 37010),
	(86974, 37007),
	(86974, 37009),
	(86974, 37008),
	(86974, 36975),
	(86974, 36981),
	(86974, 36973),
	(86974, 36980),
	(86974, 36974),
	(86974, 36971),
	(86974, 36982),
	(86974, 36979),
	(86974, 36976),
	(86974, 36978),
	(86974, 36972),
	(86974, 36977),
	(86974, 37122),
	(86974, 37121),
	(86974, 36914),
	(86974, 36918),
	(86974, 36911);
