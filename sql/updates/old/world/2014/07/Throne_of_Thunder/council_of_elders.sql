-- Adjusts some templates

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`exp` = 4, 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 43563, 
`maxdmg` = 69309, 
`attackpower` = 62114, 
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 4,
`unit_class` = 4,
`ScriptName` = 'boss_kazra_jin',
`flags_extra` = 0x01,
`Health_mod` = 147.2,
`mechanic_immune_mask` = 769638399
WHERE `entry` = 69134;

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`faction_A` = 14, 
`faction_H` = 14,
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0,
`unit_class` = 4,
`ScriptName` = 'boss_sul_the_sandcrawler',
`flags_extra` = 0x01,
`exp` = 4, 
`mindmg` = 43563, 
`maxdmg` = 69309, 
`Health_mod` = 147.2,
`attackpower` = 62114,
`mechanic_immune_mask` = 769638399
WHERE `entry` = 69078;

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`faction_A` = 14, 
`faction_H` = 14, 
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000,
`VehicleId` = 2665,
`unit_class` = 4, 
`ScriptName` = 'boss_king_malakk',
`exp` = 4, 
`mindmg` = 43563, 
`maxdmg` = 69309, 
`attackpower` = 62114, 
`Health_mod` = 147.2, 
`mechanic_immune_mask` = 769638399,
`flags_extra` = 0x01
WHERE `entry` = 69131;

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`faction_A` = 14, 
`faction_H` = 14, 
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0, 
`ScriptName` = 'boss_high_priestress_mar_li',
`mindmg` = 43563, 
`maxdmg` = 69309, 
`attackpower` = 62114, 
`Health_mod` = 137.4, 
`mechanic_immune_mask` = 769638399,
`flags_extra` = 0x01
WHERE `entry` = 69132;

UPDATE `creature_template` SET 
`type_flags` = 1049600, 
`ScriptName` = 'npc_gara_jal_s_soul' ,
`modelid1` = 11686, 
`modelid2` = 0
WHERE `entry` = 69182;

UPDATE `creature_template` SET
`minlevel` = 90,
`maxlevel` = 90,
`exp` = 4,
`mindmg` = 9838,
`maxdmg` = 14331,
`attackpower` = 42296,
`baseattacktime` = 2000,
`rangeattacktime` = 2000,
`unit_flags` = 33554438,
`ScriptName` = 'npc_gara_jal_the_spirit_binder'
WHERE `entry` = 69135;

UPDATE `creature_template` SET 
`modelid1` = 11686, 
`modelid2` = 0
WHERE `entry` = 68553;

UPDATE `creature_template` SET 
`ScriptName` = 'mob_living_sand', 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000,
`exp` = 4
WHERE `entry` = 69153;

UPDATE `creature_template` SET 
`faction_A` = 7, 
`faction_H` = 7, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`ScriptName` = 'mob_blessed_loa_spirit'
WHERE `entry` = 69480;

UPDATE `creature_template` SET 
`faction_A` = 7, 
`faction_H` = 7, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`ScriptName` = 'mob_shadowed_lua_spirit'
WHERE `entry` = 69548;

UPDATE `creature_template` SET 
`exp` = 4, 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0, 
`ScriptName` = 'mob_first_twisted_fate' 
WHERE `entry` = 69740;

UPDATE `creature_template` SET 
`exp` = 4, 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0, 
`ScriptName` = 'mob_second_twisted_fate' 
WHERE `entry` = 69746;

UPDATE `creature_template` SET
`modelid1` = 11686,
`modelid2` = 0
WHERE `entry` = 69453;

insert into `creature_template_addon` values('69453','0','0','0','0','0','1784');
UPDATE `creature_template` SET `flags_extra` = 128 WHERE `entry` = 69453;

-- Scriptnames

DELETE FROM spell_script_names WHERE spell_id IN (137131, 137117, 136904, 136467, 136917, 136990, 136922, 137149, 136878, 136895, 136507, 137643, 137641, 137166, 137943, 137964, 137972);
INSERT INTO spell_script_names VALUES
(137131, "spell_reckless_charge_movement"),
(137117, "spell_reckless_charge_rolling"),
(136904, "spell_frigid_assault"),
(136467, "spell_lingering_presence"),
(136917, "spell_biting_cold_malakk"),
(136990, "spell_frosbite_malakk"),
(136922, "spell_frosbite_malakk_aura"),
(137149, "spell_overload_discharge_kazra_jin"),
(137166, "spell_overload_discharge_kazra_jin"),
(136878, "spell_ensnared"),
(136895, "spell_sandstorm"),
(136507, "spell_dark_power"),
(137643, "spell_soul_fragment_switch"),
(137641, "spell_soul_fragment"),
(137943, "spell_first_twisted_fate"),
(137964, "spell_second_twisted_fate"),
(137972, "spell_twisted_fate_damage");


-- Boss Spawns

DELETE FROM creature WHERE id IN (69182, 69135, 69132, 69131, 69078, 69134);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES
		('69134','1098','6622','6705','760','1','0','0','6056.48','5386.44','136.171','2.11191','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Kazra'jin
		('69078','1098','6622','6705','760','1','0','0','6068.75','5396.01','136.171','2.32446','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Sul the Sandcrawler
		('69131','1098','6622','6705','760','1','0','0','6025.24','5393.14','136.171','0.699817','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Frost King Malakk
		('69132','1098','6622','6705','760','1','0','0','6035.9','5385.33','136.171','1.17586','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- High Priestess Mar'li
		('69182','1098','6622','6705','760','1','0','0','6046.76','5432.27','136.171','4.7123','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Gara'jal's Soul
		('69135','1098','6622','6705','760','1','0','0','6046.78','5432.15','137.694','4.69419','604800','0','0','1','0','0','0','0','0','0','0','0',NULL); -- Gara'jal the Spiritbin




-- Talks

-- Gara'Jal the SpiritBinder - 69135

DELETE FROM creature_text WHERE entry = 69135;
DELETE FROM locales_creature_text WHERE entry = 69135;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69135','0','0','Witness the powa of the Spiritbinder!','14','0','100','0','0','35398','Tot CoE aggro');
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69135','0','0',NULL,'Contemplez la puissance du lieur d\'esprit !',NULL,NULL,NULL,'¡Observa el podeh del Vinculador de Espíritus!','¡Observa el podeh del Vinculador de Espíritus!',NULL,NULL,NULL);

-- High Priestress Mar'Li - 69132

DELETE FROM creature_text WHERE entry = 69132;
DELETE FROM locales_creature_text WHERE entry = 69132;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','0','0','Death ta all who appose da empire!','14','0','100','0','0','35432','Mar\'Li aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','1','0','Da spiritbinder reveals yer soul ta me!','14','0','100','0','0','35434','Mar\'Li possession');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','2','0','Yer soul belongs ta me!','14','0','100','0','0','35439','Mar\'Li shadowed Loa 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','3','0','Succumb ta her venom!','14','0','100','0','0','35438','Mar\'Li shadowed Loa 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','4','0','Embrace yer demise!','14','0','100','0','0','35440','Mar\'Li shadowed Loa 3');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','5','0','Shadra\'s rage will destroy you!','14','0','100','0','0','35435','Mar\'Li shadowed Loa 4');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','6','0','She will feast on yer soul!','14','0','100','0','0','35437','Mar\'Li player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','7','0','Another offering to da loa!','14','0','100','0','0','35436','Mar\'Li player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','8','0','Shadra... save... me...','14','0','100','0','0','35433','Mar\'Li death');
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','0','0',NULL,'Mort à tous ceux qui s\'opposent à l\'empire !',NULL,NULL,NULL,'¡Muerte a todos los que se oponen al imperio!','¡Muerte a todos los que se oponen al imperio!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','1','0',NULL,'Le lieur d\'esprits... Il m\'a montré ton âme !',NULL,NULL,NULL,'¡El Vinculador de Espíritus me revela tu alma!','¡El Vinculador de Espíritus me revela tu alma!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','2','0',NULL,'Votre âme.. Elle m\'appartient !',NULL,NULL,NULL,'¡Tu alma me pertenece!','¡Tu alma me pertenece!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','3','0',NULL,'Succombez à son venin !',NULL,NULL,NULL,'¡Abraza tu muerte!','¡Abraza tu muerte!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','4','0',NULL,'Embrassez votre défaite !',NULL,NULL,NULL,'¡La ira de Shadra te destruirá!','¡La ira de Shadra te destruirá!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','5','0',NULL,'Que la rage de Shadra vous détruise !',NULL,NULL,NULL,'¡La ira de Shadra te destruirá!','¡La ira de Shadra te destruirá!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','6','0',NULL,'Elle va se nourrir de votre âme !',NULL,NULL,NULL,'¡Ella festejará en tu alma!','¡Ella festejará en tu alma!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','7','0',NULL,'Une autre offrande au Loa !',NULL,NULL,NULL,'¡Otra ofrenda para la Loa!','¡Otra ofrenda para la Loa!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69132','8','0',NULL,'Shadra... Sauve-moi...',NULL,NULL,NULL,'Shadra... sálvame...','Shadra... sálvame...',NULL,NULL,NULL);


-- Kaz'Ra Jin - 69134

DELETE FROM creature_text WHERE entry = 69134;
DELETE FROM locales_creature_text WHERE entry = 69134;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','0','0','Dis is gonna hurt!','14','0','100','0','0','35566','Kaz\'Ra Jin Aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','1','0','Ya shouldn\'t be messin\' wit da Zandalari!','14','0','100','0','0','35571','Kaz\'Ra Jin possession');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','2','0','Incoming!','14','0','100','0','0','35574','Kaz\'Ra Jin Reckless Charge 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','3','0','Out da way!','14','0','100','0','0','35573','Kaz\'Ra Jin Reckless Charge 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','4','0','Shocking!','14','0','100','0','0','35576','Kaz\'Ra Jin discharge');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','5','0','Da Amani empire cannot be stopped!','14','0','100','0','0','35570','Kaz\'Ra Jin player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','6','0','On ya knees!','14','0','100','0','0','35572','Kaz\'Ra Jin player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','7','0','Da thunder king... promised...','14','0','100','0','0','35567','Kaz\'Ra Jin death');
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','0','0',NULL,'Ca va faire mal !',NULL,NULL,NULL,'¡Eh\'to va a doler!','¡Eh\'to va a doler!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','1','0',NULL,'Vous ne devriez pas chercher les Zandalari !',NULL,NULL,NULL,'¡No deberíah buscarse problemas con los Zandalari!','¡No deberíah buscarse problemas con los Zandalari!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','2','0',NULL,'J\'arrive !',NULL,NULL,NULL,'¡En camino!','¡En camino!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','3','0',NULL,'Dégagez !',NULL,NULL,NULL,'¡Fuera de aquí!','¡Fuera de aquí!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','4','0',NULL,'Electrochocs !',NULL,NULL,NULL,'¡Impactante!','¡Impactante!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','5','0',NULL,'L\'empire Amani, il ne peut pas être arrêté !',NULL,NULL,NULL,'¡El Imperio Amani no puede ser parado!','¡El Imperio Amani no puede ser parado!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','6','0',NULL,'A genoux !',NULL,NULL,NULL,'¡En tus rodillas!','¡En tus rodillas!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69134','7','0',NULL,'Le Roi-Tonnerre... Il a promis...',NULL,NULL,NULL,'El Rey del Trueno... prometió...','El Rey del Trueno... prometió...',NULL,NULL,NULL);



-- Frost King Malakk - 69131

DELETE FROM creature_text WHERE entry = 69131;
DELETE FROM locales_creature_text WHERE entry = 69131;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','0','0','Ya have met your match, fools!','14','0','100','0','0','35387','Malakk aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','1','0','Winter is coming...','14','0','100','0','0','35389','Malakk possession');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','2','0','Freeze!','14','0','100','0','0','35394','Malakk Frostbite 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','3','0','Getting cold?','14','0','100','0','0','35393','Malakk Frostbite 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','4','0','Da\' Drakkari will never fall to de\' likes of you!','14','0','100','0','0','35390','Malakk Body Heat');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','5','0','Death\'s cold embrace.','14','0','100','0','0','35391','Malakk player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','6','0','Witness da Drakkari\'s might.','14','0','100','0','0','35392','Malakk player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','7','0','Da... empire... can\'t... fall...','14','0','100','0','0','35388','Malakk\'s death');
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','0','0',NULL,'Vous avez trouvé un avdversaire à votre hauteur, idiot !',NULL,NULL,NULL,'¡Habéih encontrao vueh\'tro desafio, necios!','¡Habéih encontrao vueh\'tro desafio, necios!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','1','0',NULL,'L\'hiver vient...',NULL,NULL,NULL,'El invierno se aproxima...','El invierno se aproxima...',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','2','0',NULL,'On ne bouge plus !',NULL,NULL,NULL,'¡Congelado!','¡Congelado!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','3','0',NULL,'Un peu froid ?',NULL,NULL,NULL,'¿Sintiendo frío?','¿Sintiendo frío?',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','4','0',NULL,'Les Drakari, ils se laisseront pas faire par des mecs comme vous !',NULL,NULL,NULL,'¡Loh Drakkari nunca caerán ante tus semejantes!','¡Loh Drakkari nunca caerán ante tus semejantes!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','5','0',NULL,'L\'étreinte froide de la mort !',NULL,NULL,NULL,'El abrazo frío de la mueh\'te.','El abrazo frío de la mueh\'te.',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','6','0',NULL,'Voilà la puissance des Drakari !',NULL,NULL,NULL,'Observa el podeh de los Drakkari.','Observa el podeh de los Drakkari.',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69131','7','0',NULL,'L\'empire... Il ne peut pas... Tomber...',NULL,NULL,NULL,'El imperio... no puede... caer...','El imperio... no puede... caer...',NULL,NULL,NULL);


-- Sul the Sandcrawler - 69078

DELETE FROM creature_text WHERE entry = 69078;
DELETE FROM locales_creature_text WHERE entry = 69078;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','0','0','Da sands will consume everyting!','14','0','100','0','0','35812','Sul aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','1','0','I will bury ya all!','14','0','100','0','0','35813','Sul possessed');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','2','0','Da storm approaches!','14','0','100','0','0','35818','Sul Sandstorm');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','3','0','Watch yer step!','14','0','100','0','0','35817','Sul Quicksand');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','4','0','Dey tink dey can stop da Farraki?','14','0','100','0','0','35814','Sul Treacherous Ground');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','5','0','Da sands are endless.','14','0','100','0','0','35815','Sul player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','6','0','Da first of many!','14','0','100','0','0','35816','Sul player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','7','0','I return... to... da... sands...','14','0','100','0','0','36454','Sul\'s death');
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','0','0',NULL,'Les sables... Ils vont tout ensevelir !',NULL,NULL,NULL,'¡Las arenas consumirán todo!','¡Las arenas consumirán todo!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','1','0',NULL,'je vais tous vous enterrer !',NULL,NULL,NULL,'¡Los enterraré a todos!','¡Los enterraré a todos!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','2','0',NULL,'La temppête... Elle approche !',NULL,NULL,NULL,'¡La tormenta se acerca!','¡La tormenta se acerca!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','3','0',NULL,'Attention où vous marchez !',NULL,NULL,NULL,'¡Cuidado donde pisen!','¡Cuidado donde pisen!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','4','0',NULL,'Ils croient qu\'ils peuvent arrêter les Faraki, héhéhé...',NULL,NULL,NULL,'¿Piensan que pueden detener a los Farraki?','¿Piensan que pueden detener a los Farraki?',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','5','0',NULL,'Les sables... Ils sont infinis !',NULL,NULL,NULL,'Las arenas son interminables.','Las arenas son interminables.',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','6','0',NULL,'Le premier d\'une longue série !',NULL,NULL,NULL,'¡El primero de muchos!','¡El primero de muchos!',NULL,NULL,NULL);
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`, `text_loc9`, `text_loc10`) VALUES('69078','7','0',NULL,'Les sables... M\'engloutissent...',NULL,NULL,NULL,'Yo regresaré... a... las... arenas...','Yo regresaré... a... las... arenas...',NULL,NULL,NULL);


-- Templates All Modes

-- Kaz'Ra Jin

UPDATE creature_template SET difficulty_entry_4 = 80134, difficulty_entry_5 = 81134, difficulty_entry_6 = 82134 WHERE entry = 69134;

-- N25

DELETE FROM creature_template WHERE entry = 80134;
INSERT INTO `creature_template` VALUES('80134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80134;
-- H10

DELETE FROM creature_template WHERE entry = 81134;
INSERT INTO `creature_template` VALUES('81134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81134;
-- H25

DELETE FROM creature_template WHERE entry = 82134;
INSERT INTO `creature_template` VALUES('82134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82134;

-- Sul the Sandcrawler

UPDATE creature_template SET difficulty_entry_4 = 80078, difficulty_entry_5 = 81078, difficulty_entry_6 = 82078 WHERE entry = 69078;

-- N25

DELETE FROM creature_template WHERE entry = 80078;
INSERT INTO `creature_template` VALUES('80078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80078;
-- H10

DELETE FROM creature_template WHERE entry = 81078;
INSERT INTO `creature_template` VALUES('81078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81078;
-- H25

DELETE FROM creature_template WHERE entry = 82078;
INSERT INTO `creature_template` VALUES('82078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82078;

-- Frost King Malakk

UPDATE creature_template SET difficulty_entry_4 = 80131, difficulty_entry_5 = 81131, difficulty_entry_6 = 82131 WHERE entry = 69131;

-- N25

DELETE FROM creature_template WHERE entry = 80131;
INSERT INTO `creature_template` VALUES('80131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80131;
-- H10

DELETE FROM creature_template WHERE entry = 81131;
INSERT INTO `creature_template` VALUES('81131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81131;
-- H25

DELETE FROM creature_template WHERE entry = 82131;
INSERT INTO `creature_template` VALUES('82131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82131;

-- High Priestress Mar'Li

UPDATE creature_template SET difficulty_entry_4 = 80132, difficulty_entry_5 = 81132, difficulty_entry_6 = 82132 WHERE entry = 69132;

-- N25

DELETE FROM creature_template WHERE entry = 80132;
INSERT INTO `creature_template` VALUES('80132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 412 WHERE `entry` = 80131;
-- H10

DELETE FROM creature_template WHERE entry = 81132;
INSERT INTO `creature_template` VALUES('81132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 215.7 WHERE `entry` = 81131;
-- H25

DELETE FROM creature_template WHERE entry = 82132;
INSERT INTO `creature_template` VALUES('82132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 647.04 WHERE `entry` = 82131;