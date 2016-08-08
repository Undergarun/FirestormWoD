DROP TABLE IF EXISTS `spell_aura_not_saved`;
CREATE TABLE `spell_aura_not_saved`
(
    `SpellID` INT(10) UNSIGNED NOT NULL,
    `Comment` TEXT NOT NULL,
    PRIMARY KEY (`SpellID`)
);

INSERT INTO `spell_aura_not_saved` VALUES
(29519, "Silithyst"),
(40075, "Fel Flak Fire"),
(55849, "Power Spark"),
(96206, "Glyph of Rejuvenation"),
(99158, "Dark Flames"),
(101619, "Magma, Fall of Azeroth"),
(97151, "Magma, Firelands"),
(81114, "Blackwing Descent"),
(57634, "Magma, CoA Black / Chamber"),
(42201, "Eternal Silence"),
(36444, "Wintergrasp Water"),
(28801, "Slime, Naxxramas"),
(168506, "Ancient Artifact (Ashran PvP Zone)"),
(69700, "Ice Tomb Untargettable (Sindragosa)"),
(70157, "Ice Tomb DoT (Sindragosa)"),
(71665, "Asphyxiation (Sindragosa)");