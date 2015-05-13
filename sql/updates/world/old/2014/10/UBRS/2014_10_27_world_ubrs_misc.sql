DELETE FROM instance_template WHERE map = 1358;
INSERT INTO instance_template VALUE (1358, 0, 'instance_upper_blackrock_spire', 0);

DELETE FROM areatrigger_teleport WHERE id IN (1468, 1470, 10205);
INSERT INTO areatrigger_teleport VALUES
(1468, 'Upper Blackrock Spire - Entrance Target', 1358, 120.472, -319.1041, 70.9541, 6.2642),
(10205, 'Upper Blackrock Spire - Exit Target', 0, -7493.2876, -1328.8904, 301.3641, 1.7104);

DELETE FROM game_graveyard_zone WHERE ghost_zone = 7307;
INSERT INTO game_graveyard_zone VALUE
(1469, 7307, 0);

UPDATE creature_template SET ScriptName = 'mob_black_iron_grunt' WHERE entry = 76179;

DELETE FROM creature_text WHERE entry = 76179;
INSERT INTO creature_text VALUES
(76179, 0, 0, 'Iron Horde, we have unwanted visitors!', 12, 0, 100, 0, 0, 0, 'TALK_IRON_GRUNT_AGGRO_1'),
(76179, 0, 1, 'Help! We have intruders!', 12, 0, 100, 0, 0, 0, 'TALK_IRON_GRUNT_AGGRO_2'),
(76179, 1, 0, 'We need backup!', 12, 0, 100, 0, 0, 0, 'TALK_IRON_GRUNT_NEAR_DEATH');

DELETE FROM locales_creature_text WHERE entry = 76179;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(76179, 0, 0, 'Horde de Fer, nous avons de la visite !', '¡Horda de Hierro, tenemos visitantes no deseados!'),
(76179, 0, 1, 'À l''aide ! Nous avons des intrus !', '¡Ayuda! ¡Tenemos intrusos!'),
(76179, 1, 0, 'Il nous faut des renforts !', '¡Necesitamos cobertura!');

DELETE FROM trinity_string WHERE entry IN (14057, 14058, 14059, 14060);
INSERT INTO trinity_string (entry, content_default, content_loc2, content_loc6) VALUES
(14057, '|TInterface\Icons\mocking_banner:20|t Black Iron Grunt puts a |cFFFF0000|Hspell:153799|h[Rallying Banner]|h|r to call his nearby allies!', '|TInterface\Icons\mocking_banner:20|t Grunt fer-noir place une |cFFFF0000|Hspell:153799|h[Bannière de ralliement]|h|r pour appeler ses alliés proches !', '|TInterface\Icons\mocking_banner:20|t Bruto Hierro Umbrío pone una |cFFFF0000|Hspell:153799|h[Bandera de Rallying]|h|r para llamar a los aliados cercanos!'),
(14058, 'Disrupt Rune Conduits to weaken Gor''ashan''s defenses.', 'Bloquez les conduits runiques pour affaiblir les défenses de Gor''ashan.', 'La runa de perturbación debilita las defensas de Gor''ashan.'),
(14059, 'Black Iron Summoner begins to summon in a Black Iron Dreadweaver!', 'Invocatrice fer-noir commence à invoquer un Tisseur d''effroi fer-noir !', '¡Invocador Hierro Negro empieza a invocar un Tejenieblas Hierro Negro!'),
(14060, 'Black Iron Summoner summons a Black Iron Veteran to his aid!', 'Invocatrice fer-noir commence à invoquer un Vétéran fer-noir !', '¡Invocador Hierro Negro invoca un Veterano Hierro Negro como apoyo!');

UPDATE creature_template SET ScriptName = 'mob_black_iron_leadbelcher' WHERE entry = 76157;
UPDATE creature_template SET ScriptName = 'mob_sentry_cannon' WHERE entry = 76314;
UPDATE creature_template SET ScriptName = 'mob_ragemaw_worg' WHERE entry = 76181;
UPDATE creature_template SET ScriptName = 'mob_ragemaw_worg' WHERE entry = 76181;
UPDATE creature_template SET ScriptName = 'mob_black_iron_warcaster' WHERE entry = 76151;

DELETE FROM spell_script_names WHERE spell_id IN (153942, 154435, 166168, 154302, 154900, 154901, 155037, 161235);
INSERT INTO spell_script_names VALUES
(153942, 'spell_shrapnel_storm'),
(154435, 'spell_lodestone_spike'),
(166168, 'spell_power_conduit_hangover'),
(154302, 'spell_seal_conduit'),
(154900, 'spell_seal_conduit'),
(154901, 'spell_seal_conduit'),
(155037, 'spell_eruption'),
(161235, 'spell_vilebloom_serum');

DELETE FROM creature_text WHERE entry IN (76413, 76773);
INSERT INTO creature_text VALUES
(76413, 0, 0, 'Who dares disturb my research!', 14, 0, 100, 0, 0, 43920, 'VO_60_UBRS_GORASHAN_INTRO_01'),
(76413, 1, 0, 'But the power... it protects me...', 14, 0, 100, 0, 0, 43919, 'VO_60_UBRS_GORASHAN_DEATH_01'),
(76413, 2, 0, 'You disappoint me, I thought you''d be tougher.', 14, 0, 100, 0, 0, 43921, 'VO_60_UBRS_GORASHAN_SLAY_01'),
(76413, 2, 1, 'The weak should know better than to test me.', 14, 0, 100, 0, 0, 43922, 'VO_60_UBRS_GORASHAN_SLAY_02'),
(76413, 3, 0, 'So much power! I am invincible!', 14, 0, 100, 0, 0, 43923, 'VO_60_UBRS_GORASHAN_SPELL_01'),
(76413, 4, 0, 'You can''t handle this!', 14, 0, 100, 0, 0, 43924, 'VO_60_UBRS_GORASHAN_SPELL_02'),
(76413, 5, 0, 'I can feel this in my soul.', 14, 0, 100, 0, 0, 43925, 'VO_60_UBRS_GORASHAN_SPELL_03'),
(76413, 6, 0, 'I will destroy you.', 14, 0, 100, 0, 0, 43926, 'VO_60_UBRS_GORASHAN_SPELL_04'),
(76773, 0, 0, 'Stop them! They can''t be allowed to disturb the master from his trance!', 12, 0, 100, 0, 0, 0, 'TALK_APPRENTICE_AGGRO');

DELETE FROM locales_creature_text WHERE entry IN (76413, 76773);
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(76413, 0, 0, 'Qui ose interrompre mes recherches ?', '¡Quién osa interrumpir mi búsqueda!'),
(76413, 1, 0, 'Mais le pouvoir ... il me protège ...', 'Pero el poder... me proteje...'),
(76413, 2, 0, 'Vous me décevez, je vous aurais cru plus solide.', 'Me decepcionáis... pensaba que serían más fuertes.'),
(76413, 2, 1, 'Les faibles devraient s''abstenir de me mettre à l''épreuve.', 'Los débiles deben saber mejor que ponerme a prueba.'),
(76413, 3, 0, 'Tant de pouvoir ! Je suis invincible !', '¡Cuánto poder! ¡Soy invencible!'),
(76413, 4, 0, 'Vous n''êtes pas à la hauteur !', '¡No podréis soportar esto!'),
(76413, 5, 0, 'Je le sens dans mon âme.!', 'Puedo sentirlo en el alma.'),
(76413, 6, 0, 'Je vais vous détruire.', 'Os destruiré.'),
(76773, 0, 0, 'Arrêtez-lez ! Ils ne peuvent être autorisés à interrompre la méditation du maître !', '¡Detenedlos! ¡No pueden molestar al maestro durante su trance!');

UPDATE creature_template SET ScriptName = 'mob_black_iron_apprentice' WHERE entry = 76773;
UPDATE creature_template SET ScriptName = 'boss_orebender_gorashan' WHERE entry = 76413;

DELETE FROM conditions WHERE SourceEntry IN (154294, 154302, 154900, 154901, 154296, 161883);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 154294, 31, 0, 3, 76413),
(13, 1, 154302, 31, 0, 3, 76417),
(13, 1, 154900, 31, 0, 3, 76417),
(13, 1, 154901, 31, 0, 3, 76417),
(13, 4, 154296, 31, 0, 3, 76417),
(13, 1, 161883, 31, 0, 3, 80231);

UPDATE creature_template SET ScriptName = 'mob_rune_of_power' WHERE entry = 76417;
UPDATE creature_template SET ScriptName = 'mob_lightning_field', InhabitType = 4 WHERE entry = 76464;
UPDATE creature_template SET ScriptName = 'mob_rune_glow' WHERE entry = 76396;
UPDATE gameobject_template SET ScriptName = 'go_runic_conduit' WHERE entry = 226704;
UPDATE creature_template SET ScriptName = 'mob_black_iron_alchemist' WHERE entry = 76100;
UPDATE creature_template SET ScriptName = 'mob_black_iron_engineer' WHERE entry = 76101;
UPDATE creature_template SET ScriptName = 'mob_drakonid_monstrosity' WHERE entry IN (76018, 82556);
UPDATE creature_template SET ScriptName = 'boss_kyrak_the_corruptor' WHERE entry = 76021;

DELETE FROM creature_text WHERE entry = 76021;
INSERT INTO creature_text VALUES
(76021, 0, 0, 'Excellent, more raw materials!', 14, 0, 100, 0, 0, 46185, 'VO_60_UBRS_KYRAK_INTRO_01'),
(76021, 1, 0, 'Please, repurpose me!', 14, 0, 100, 0, 0, 46184, 'VO_60_UBRS_KYRAK_DEATH_01'),
(76021, 2, 0, 'I think that arm is still usable, and maybe the leg.', 14, 0, 100, 0, 0, 46186, 'VO_60_UBRS_KYRAK_SLAY_01'),
(76021, 2, 1, 'Just think of all the things I will make out of your parts.', 14, 0, 100, 0, 0, 46187, 'VO_60_UBRS_KYRAK_SLAY_02'),
(76021, 2, 3, 'I hope those limps aren''t too broken.', 14, 0, 100, 0, 0, 46188, 'VO_60_UBRS_KYRAK_SLAY_03'),
(76021, 2, 4, 'So many pieces!', 14, 0, 100, 0, 0, 46189, 'VO_60_UBRS_KYRAK_SLAY_04'),
(76021, 3, 0, 'Don''t try to run, you will only die tired.', 14, 0, 100, 0, 0, 46190, 'VO_60_UBRS_KYRAK_SPELL_01'),
(76021, 4, 0, 'I will catch you and I will fix you.', 14, 0, 100, 0, 0, 46191, 'VO_60_UBRS_KYRAK_SPELL_02'),
(76021, 5, 0, 'That one! That is the one that I need!', 14, 0, 100, 0, 0, 46192, 'VO_60_UBRS_KYRAK_SPELL_03'),
(76021, 6, 0, 'I can feel the vile taint flowing through me.', 14, 0, 100, 0, 0, 46193, 'VO_60_UBRS_KYRAK_SPELL_04');

DELETE FROM locales_creature_text WHERE entry = 76021;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(76021, 0, 0, 'Excellent ! Des matériaux bruts !', '¡Excelente, más materias primas!'),
(76021, 1, 0, 'Je vous en prie, recyclez-moi...', '¡Por favor, reutilízame!'),
(76021, 2, 0, 'Ce bras me semble toujours valide, et peut-être aussi la jambe.', 'Creo que ese brazo puede ser usado todavía, y probablemente esa pierna.'),
(76021, 2, 1, 'Pensez un peu à tout ce que je vais pouvoir faire avec vos membres !', 'Tan solo piensa en todas las cosas que podré hacer con vuestras partes.'),
(76021, 2, 3, 'Jespère que ces membres ne sont pas trop cassés.', 'Creo que ese brazo puede ser usado todavía, y probablemente esa pierna.'),
(76021, 2, 4, 'Tant de morceaux !', '¡Cuantas piezas!'),
(76021, 3, 0, 'Nessayez pas de fuir, vous vous épuiseriez pour rien.', 'No intentéis escapar, tan solo moriréis cansados.'),
(76021, 4, 0, 'Je vais vous attraper et vous améliorer.', 'Os atraparé y os arreglaré.'),
(76021, 5, 0, 'Là ! Voilà les composants qu''il me faut !', '¡Este! ¡Este es el que yo necesitaba!'),
(76021, 6, 0, 'Je sens l''infection se répandre en moi.', 'Puedo sentir la mancha vil fluyendo a través de mí.');

UPDATE creature_template SET ScriptName = 'mob_black_iron_veteran' WHERE entry IN (77034, 84462);
UPDATE creature_template SET ScriptName = 'mob_black_iron_dreadweaver' WHERE entry IN (77035, 84475);
UPDATE creature_template SET ScriptName = 'mob_black_iron_summoner' WHERE entry = 77036;
UPDATE creature_template SET ScriptName = 'mob_black_iron_elite' WHERE entry = 77037;
UPDATE creature_template SET ScriptName = 'mob_black_iron_siegebreaker' WHERE entry = 77033;
UPDATE creature_template SET ScriptName = 'boss_warlord_zaela' WHERE entry = 77120;
UPDATE creature_template SET ScriptName = 'boss_commander_tharbek' WHERE entry = 79912;
UPDATE creature_template SET ScriptName = 'mob_ironbarb_skyreaver' WHERE entry = 80098;
UPDATE creature_template SET ScriptName = 'mob_imbued_iron_axe_stalker' WHERE entry = 80307;

DELETE FROM creature_text WHERE entry = 79912;
INSERT INTO creature_text VALUES
(79912, 0, 0, 'I apologize for this failure Warlord Zaela. The Iron March will take care of these invaders!', 14, 0, 100, 0, 0, 45861, 'VO_60_UBRS_THARBEK_WAVE1_DIALOG_01'),
(79912, 1, 0, 'Iron March... kill them or I will kill you!', 14, 0, 100, 0, 0, 45862, 'VO_60_UBRS_THARBEK_WAVE1_DIALOG_02'),
(79912, 2, 0, 'What? This is unacceptable!', 14, 0, 100, 0, 0, 45863, 'VO_60_UBRS_THARBEK_WAVE2_DIALOG_01'),
(79912, 3, 0, 'Kill them you fools!', 14, 0, 100, 0, 0, 45864, 'VO_60_UBRS_THARBEK_WAVE2_DIALOG_02'),
(79912, 4, 0, 'I don''t understand, these intruders fight with the fury of Grommash himself.', 14, 0, 100, 0, 0, 45865, 'VO_60_UBRS_THARBEK_WAVE3_DIALOG_01'),
(79912, 5, 0, 'Yes Warlord! I will handle this personally!', 14, 0, 100, 0, 0, 45866, 'VO_60_UBRS_THARBEK_WAVE3_DIALOG_02'),
(79912, 6, 0, 'Fetch my mount, I am tired of watching you peons disgrace my Iron Horde.', 14, 0, 100, 0, 0, 45867, 'VO_60_UBRS_THARBEK_WAVE3_DIALOG_03'),
(79912, 7, 0, 'Now let us finish this.', 14, 0, 100, 0, 0, 45856, 'VO_60_UBRS_THARBEK_INTRO_01'),
(79912, 8, 0, 'My Warlord... I have failed...', 14, 0, 100, 0, 0, 45855, 'VO_60_UBRS_THARBEK_DEATH_01'),
(79912, 9, 0, 'Your friends will join you soon!', 14, 0, 100, 0, 0, 45857, 'VO_60_UBRS_THARBEK_SLAY_01'),
(79912, 9, 1, 'Too easy.', 14, 0, 100, 0, 0, 45858, 'VO_60_UBRS_THARBEK_SLAY_02'),
(79912, 9, 2, 'How did you get this far ?', 14, 0, 100, 0, 0, 45859, 'VO_60_UBRS_THARBEK_SLAY_03'),
(79912, 9, 3, 'Warlord Zaela! I have regained my honor!', 14, 0, 100, 0, 0, 45860, 'VO_60_UBRS_THARBEK_SLAY_04');

DELETE FROM locales_creature_text WHERE entry = 79912;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(79912, 0, 0, 'Je m''excuse pour cet échec, seigneur de guerre Zaela. La coulée de fer va s''ocupper de ces intrus !', 'Me disculpopor este fracaso, Señora de la guerra Zaela. ¡La Marcha de Hierro se encargará de estos invasores!'),
(79912, 1, 0, 'Vous... tuez-les ou c''est moi qui vous tue !', 'Marcha de Hierro... ¡matadlos u os mataré!'),
(79912, 2, 0, 'Comment ? C''est intolérable !', '¿Qué? ¡Esto es inaceptable!'),
(79912, 3, 0, 'Tuez-les bande d''idiots !', '¡Matadlos, necios!'),
(79912, 4, 0, 'Je ne comprends pas, ces intrus se battent avec la fureur de Grommash !', 'No lo entiendo, estos intrusos luchan con la misma furia de Grommash.'),
(79912, 5, 0, 'Oui seigneur de guerre, je m''en occupe personnellement.', '¡Sí, señor de la guerra! ¡Me encargaré de esto personalmente!'),
(79912, 6, 0, 'Allez chercher ma monture ! J''en ai assez de regarder des péons déshonorer ma horde de fer !', 'Busca a mi montura, estoy cansado de veros, peones, deshonrar mi Horda de Hierro.'),
(79912, 7, 0, 'Allez, finissons-en !', 'Ahora acabemos con esto.'),
(79912, 8, 0, 'Seigneur de guerre, j''ai échoué.', 'Mi Señor de la guerra... he fallado...'),
(79912, 9, 0, 'Tes amis vont bientôt te rejoindre !', '¡Vuestros amigos se os unirán dentro de poco!'),
(79912, 9, 1, 'Trop facile !', 'Demasiado fácil.'),
(79912, 9, 2, 'Comment as-tu réussi à venir jusqu''ici ?', '¿Cómo habéis llegado tan lejos?'),
(79912, 9, 3, 'Seigneur de guerre, j''ai échoué.', '¡Señora de la guerra Zeala! ¡He recuperado mi honor!');

DELETE FROM creature_text WHERE entry = 77120;
INSERT INTO creature_text VALUES
(77120, 0, 0, 'Commander Tharbek, it seems your troops have allowed a group of pitiful heroes to sneak past them.', 14, 0, 100, 0, 0, 43564, 'VO_60_UBRS_ZAELA_WAVE1_DIALOG_01'),
(77120, 1, 0, 'Kill the one in the dress!', 14, 0, 100, 0, 0, 43565, 'VO_60_UBRS_ZAELA_WAVE1_DIALOG_02'),
(77120, 2, 0, 'Well done Commander, but it seems that the intruders are still quite alive.', 14, 0, 100, 0, 0, 43566, 'VO_60_UBRS_ZAELA_WAVE2_DIALOG_01'),
(77120, 3, 0, 'This is pitiful! Do your men not know where the sharp part of the axe goes?', 14, 0, 100, 0, 0, 43567, 'VO_60_UBRS_ZAELA_WAVE3_DIALOG_01'),
(77120, 4, 0, 'I will hear no more of these excuses. I am going to go prepare the weapon, take care of these invaders or I will show you how a true orc deals with failure.', 14, 0, 100, 0, 0, 43568, 'VO_60_UBRS_ZAELA_WAVE3_DIALOG_02');

DELETE FROM locales_creature_text WHERE entry = 77120;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(77120, 0, 0, 'Commandant Tharbek, vos troupes ont apparemment laissé passer un groupe de pitoyables héros.', 'Comandante Tharbek, parece que tus tropas han permitido que un grupo de lamentables héroes se cuelen a través de ellas.'),
(77120, 1, 0, 'Tuez les plus faibles en premiers !', '¡Mata al que tiene vestido!'),
(77120, 2, 0, 'Bien joué commandant, mais il semble que les intrus soient toujours bien vivants !', 'Bien hecho Comandante, pero parece que los intrusos siguen con vida.'),
(77120, 3, 0, 'C''est pitoyable ! Vos hommes ignorent-ils donc quoi faire de leur hache ?', '¡Esto es lamentable! ¿Tus hombres desconocen donde va la parte afilada del hacha?'),
(77120, 4, 0, 'Je ne veux plus entendre d''excuses. Je vais préparer l''arme, occupez-vous de ces envahisseurs ou je vous montrerai comment une vraie orque sanctionne les échecs.', 'No voy a escuchas más excusas. Voy a preparar el arma, hazte cargo de los invasores o te enseñaré como un verdadero orco se encarga del fracaso.');