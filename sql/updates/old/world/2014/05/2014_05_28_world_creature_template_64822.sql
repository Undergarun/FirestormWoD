#64822/NPC - Wrathion

	UPDATE `creature_template` SET `ScriptName` = 'npc_wrathion_mason' WHERE `entry` = 64822;
	
	DELETE FROM creature_text WHERE entry = 64822;
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','0','0','I often venture out here to think. The view is breathtaking, would you agree ?','12','0','100','0','0','29124','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','1','0','Pandaria is indeed a jewel. Such Beauty. It makes you want to protect her. Yes ?','12','0','100','0','0','29125','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','2','0','I feel the same way about Azeroth herself.','12','0','100','0','0','29126','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','3','0','You have done great things, friend. I can assure you this is but the beginning of our work together.','12','0','100','0','0','29127','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','4','0','Behold : The power of the foes you\'ve vanquished.','12','0','100','0','0','29128','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','5','0','And the wisdom of those you\'ve outsmarted.','12','0','100','0','0','29129','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','6','0','Here ! The embodiment of the fear that you\'ve conquered. May it now strike terror into the hearts of your enemies !','12','0','100','0','0','29130','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','7','0','Finally, a gift from me. Make no mistake : the Black Dragonflight lives on !','12','0','100','0','0','29131','');
	INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('64822','8','0','There. My work is done. You can now greatly augment the power of any sha-touched weapon.','12','0','100','0','0','29132','');

	DELETE FROM locales_creature_text WHERE entry = 64822;
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','0','0',NULL,'je viens souvent ici pour méditer. La vue est époustouflante, ne trouvez-vous pas ?',NULL,NULL,NULL,'A menudo suelo pensar aquí. La vista es impresionante, ¿No está de acuerdo?','A menudo suelo pensar aquí. La vista es impresionante, ¿No está de acuerdo?',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','1','0',NULL,'Pandaria est évidemment un joyau. Quelle beauté. Ça vous donne envie de la protéger. N\'est-ce pas ?',NULL,NULL,NULL,'Pandaria es en verdad una joya. Cuya belleza. Te dan ganas de protegerla. ¿Sí? ','Pandaria es en verdad una joya. Cuya belleza. Te dan ganas de protegerla. ¿Sí? ',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','2','0',NULL,'Je ressens la même chose pour tout Azeroth.',NULL,NULL,NULL,'Me siento de la misma manera acerca de Azeroth en sí misma. ','Me siento de la misma manera acerca de Azeroth en sí misma. ',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','3','0',NULL,'Vous avez accompli de grandes choses. Mais je peux vous assurer que ce n\'est que le début.',NULL,NULL,NULL,'Usted ha hecho grandes cosas, amigo. Le puedo asegurar que esto es tan solo el comienzo de nuestro trabajo juntos. ','Usted ha hecho grandes cosas, amigo. Le puedo asegurar que esto es tan solo el comienzo de nuestro trabajo juntos. ',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','4','0',NULL,'Contemplez : Le pouvoir des ennemis vaincus !',NULL,NULL,NULL,'Mirad: El poder de los enemigos que has vencido. ','Mirad: El poder de los enemigos que has vencido. ',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','5','0',NULL,'Et la sagesse de ceux que vous avez surpassés.',NULL,NULL,NULL,'Y la sabiduría de aquellos que has burlado. ','Y la sabiduría de aquellos que has burlado. ',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','6','0',NULL,'La personnification de la peur que vous avez maîtrisée ! Utilisez cette terreur pour frapper le coeur de vos ennemis !',NULL,NULL,NULL,'Aquí! La realización del miedo que has conquistado. Que ahora infunda terror en los corazones de sus enemigos! ','Aquí! La realización del miedo que has conquistado. Que ahora infunda terror en los corazones de sus enemigos! ',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','7','0',NULL,'Et enfin, je vous fais un don. Ne vous y trompez pas : Le vol draconique noir n\'est pas mort !',NULL,NULL,NULL,'Finalmente, un regalo de mi parte. No nos equivoquemos: el Vuelo Negro vive! ','Finalmente, un regalo de mi parte. No nos equivoquemos: el Vuelo Negro vive! ',NULL,NULL,NULL);
	INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('64822','8','0',NULL,'Voila. Mon oeuvre est terminée. Vous pouvez maintenant augmenter la puissance de n\'importe quelle arme touchée par les Sha.',NULL,NULL,NULL,'Ya está. Mi trabajo está hecho. Ahora puede aumentar en gran medida el poder de cualquier arma influenciado por el Sha.','Ya está. Mi trabajo está hecho. Ahora puede aumentar en gran medida el poder de cualquier arma influenciado por el Sha.',NULL,NULL,NULL);

	DELETE FROM script_waypoint WHERE entry = 64822;
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64822','0','755.302','-518.921','447.633','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64822','1','764.192','-526.117','450.604','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64822','2','773.089','-534.699','453.627','0',NULL);
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64822','3','775.223','-541.617','453.708','0','Panorama');
	INSERT INTO `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) VALUES('64822','4','777.286','-538.242','454.061','0',NULL);

	
	
/*
I often venture out here to think. The view is breathtaking, would you agree ?
je viens souvent ici pour méditer. La vue est époustouflante, ne trouvez-vous pas ?
A menudo suelo pensar aquí. La vista es impresionante, ¿No está de acuerdo?

Pandaria is indeed a jewel. Such Beauty. It makes you want to protect her. Yes ?
Pandaria est évidemment un joyau. Quelle beauté. Ça vous donne envie de la protéger. N'est-ce pas ?
Pandaria es en verdad una joya. Cuya belleza. Te dan ganas de protegerla. ¿Sí? 

I feel the same way about Azeroth herself.
Je ressens la même chose pour tout Azeroth.
Me siento de la misma manera acerca de Azeroth en sí misma. 

*Goes to place for event*

You have done great things, friend. I can assure you this is but the beginning of our work together.
Vous avez accompli de grandes choses. Mais je peux vous assurer que ce n'est que le début.
Usted ha hecho grandes cosas, amigo. Le puedo asegurar que esto es tan solo el comienzo de nuestro trabajo juntos. 

*self cast spell*
*scene object launched after 4 seconds*

Behold : The power of the foes you've vanquished.
Contemplez : Le pouvoir des ennemis vaincus !
Mirad: El poder de los enemigos que has vencido. 

And the wisdom of those you've outsmarted.
Et la sagesse de ceux que vous avez surpassés.
Y la sabiduría de aquellos que has burlado. 

Here ! The embodiment of the fear that you've conquered. May it now strike terror into the hearts of your enemies !
La personnification de la peur que vous avez maîtrisée ! Utilisez cette terreur pour frapper le coeur de vos ennemis !
Aquí! La realización del miedo que has conquistado. Que ahora infunda terror en los corazones de sus enemigos! 

*explosion*

Finally, a gift from me. Make no mistake : the Black Dragonflight lives on !
Et enfin, je vous fais un don. Ne vous y trompez pas : Le vol draconique noir n'est pas mort !
Finalmente, un regalo de mi parte. No nos equivoquemos: el Vuelo Negro vive! 

*scene object just stopped, flag questgiver set, kill credit applied*

There. My work is done. You can now greatly augment the power of any sha-touched weapon.
Voila. Mon oeuvre est terminée. Vous pouvez maintenant augmenter la puissance de n'importe quelle arme touchée par les Sha.
Ya está. Mi trabajo está hecho. Ahora puede aumentar en gran medida el poder de cualquier arma influenciado por el Sha.


http://pastebin.com/9xG6VGyR

*/