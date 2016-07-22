REPLACE INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_req`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`) VALUES ('250000','0','0','27614','0','0','0','Jeanne',NULL,'Loyalty Points','','0','103','103','2','0','35','1','0','1','1.14286','1.14286','0.5','4','127','500','500','500','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','1','4','1','1.5','1','1','1','0','0','0','0','0','0','0','0','0','0','1','0','0','0','npc_loyalty_point','1');

DELETE FROM npc_text WHERE ID IN (92007, 92008);
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(
    92007,
    "Oh, so you came here to receive your loyalty points? Well, let me tell you! I have already given you your loyalty reward points, enjoy spending them on the Firestorm Shop. For more information about the loyalty points visit: http://fstorm.cc/loyalty-point.",
    "Oh, so you came here to receive your loyalty points? Well, let me tell you! I have already given you your loyalty reward points, enjoy spending them on the Firestorm Shop. For more information about the loyalty points visit: http://fstorm.cc/loyalty-point."
),
(
    92008,
    "Oh no! It looks like you have already claimed your loyalty reward points, please come back tomorrow!",
    "Oh no! It looks like you have already claimed your loyalty reward points, please come back tomorrow!"
);

DELETE FROM locales_npc_text WHERE entry IN (92007, 92008);

--                                   French                      Spanish
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc6, Text0_1_loc6) VALUES
(
    92007,
    "Oh, tu viens récupérer tes points de fidélité ? Les voilà ! Tu peux les dépenser sur la boutique du site internet de Firestorm ! Pour plus d'informations à propos des points de fidélité, rendez-vous à l'adresse : http://fstorm.cc/point-fidelite",
    "Oh, tu viens récupérer tes points de fidélité ? Les voilà ! Tu peux les dépenser sur la boutique du site internet de Firestorm ! Pour plus d'informations à propos des points de fidélité, rendez-vous à l'adresse : http://fstorm.cc/point-fidelite",
    "¡Acabo de hacer entrega de tus puntos de lealtad! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad",
    "¡Acabo de hacer entrega de tus puntos de lealtad! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad"
),
(
    92008,
    "Tu as déjà récupéré tes points de fidélité pour aujourd'hui ! Reviens demain.",
    "Tu as déjà récupéré tes points de fidélité pour aujourd'hui ! Reviens demain.",
	"¡Ya hice entrega de los puntos de lealtad que merecías antes! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad",
    "¡Ya hice entrega de los puntos de lealtad que merecías antes! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad"
)