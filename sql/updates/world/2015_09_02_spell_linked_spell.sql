DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 156052 AND `spell_effect` = 156055;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 156059 AND `spell_effect` = 156060;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 173286 AND `spell_effect` = 173288;
INSERT INTO `spell_linked_spell` VALUES
    (156052, 156055, 2, 'Engineering - Oglethorpe\'s Missile Splitter'),
    (156059, 156060, 2, 'Engineering - Megawatt Filament'),
    (173286, 173288, 2, 'Engineering - Hemet\'s Heartseeker');
