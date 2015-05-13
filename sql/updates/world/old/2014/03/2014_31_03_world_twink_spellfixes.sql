-- fix health of Xuen
update creature_template set `Expansion`=1 where `entry`='63508';

-- fix Jade Serpent Statue: can't move, immune to heal, can't attack.
UPDATE `creature_template` SET `unit_flags`='131596', `mechanic_immune_mask`='32768' WHERE `entry`='60849';

-- fix Muscle Memory
INSERT INTO `spell_script_names` VALUES (100780,'spell_monk_muscle_memory');