SET NAMES utf8;

DELETE FROM areatrigger_scripts WHERE entry = 10243;
INSERT INTO areatrigger_scripts VALUE (10243, "areatrigger_at_foundry_iron_maidens_entrance");

SET @REF_SORKA = 77231;
SET @REF_GARAN = 77557;
SET @REF_MARAK = 77477;

UPDATE creature_template SET unit_flags = unit_flags & ~(0x200000 | 0x04), dmg_multiplier = 10, ScriptName = "boss_enforcer_sorka", mechanic_immune_mask = 617299839 WHERE entry = @REF_SORKA;
UPDATE creature_template SET unit_flags = unit_flags & ~(0x200000 | 0x04), dmg_multiplier = 10, ScriptName = "boss_admiral_garan", mechanic_immune_mask = 617299839 WHERE entry = @REF_GARAN;
UPDATE creature_template SET unit_flags = unit_flags & ~(0x200000 | 0x04), dmg_multiplier = 10, ScriptName = "boss_marak_the_blooded", mechanic_immune_mask = 617299839 WHERE entry = @REF_MARAK;

REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (@REF_SORKA, 1, 114966, 113965, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (@REF_GARAN, 1, 0, 0, 112344);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (@REF_MARAK, 1, 114962, 114971, 0);

DELETE FROM creature_text WHERE entry IN (@REF_SORKA, @REF_GARAN, @REF_MARAK);
INSERT INTO creature_text VALUES
(@REF_SORKA, 0, 0, "My pleasure.", 14, 0, 100, 0, 0, 41851, "Intro 0"),
(@REF_SORKA, 1, 0, "I think I see the problem...", 12, 0, 100, 0, 0, 41852, "Intro 1"),
(@REF_SORKA, 2, 0, "Let's see how well you trained our soldiers, Gar'an.", 14, 0, 100, 0, 0, 41853, "Intro 2"),
(@REF_SORKA, 3, 0, "What brutality... What bloodshed... Beautiful!", 14, 0, 100, 0, 0, 41854, "Intro 3"),
(@REF_SORKA, 4, 0, "Don't worry, I'm coming... this looks fun.", 12, 0, 100, 0, 0, 41855, "Intro 4"),

(@REF_GARAN, 0, 0, "Patience, sisters. The sooner we finish our work, the sooner we crush our enemies.", 14, 0, 100, 0, 0, 41824, "Intro 0"),
(@REF_GARAN, 1, 0, "Sorka, ensure the peons are keeping pace!", 14, 0, 100, 0, 0, 41825, "Intro 1"),
(@REF_GARAN, 2, 0, "Marak, load the ammunition into the main cannon!", 14, 0, 100, 0, 0, 41826, "Intro 2"),
(@REF_GARAN, 3, 0, "What? The trains are behind schedule?", 14, 0, 100, 0, 0, 41827, "Intro 3"),
(@REF_GARAN, 4, 0, "You insufferable...", 14, 0, 100, 0, 0, 41829, "Intro 4"),
(@REF_GARAN, 5, 0, "Sorka! This isn't a game! Get down there!", 14, 0, 100, 0, 0, 41828, "Intro 5"),

(@REF_MARAK, 0, 0, "Right.", 14, 0, 100, 0, 0, 41710, "Intro 0");

DELETE FROM locales_creature_text WHERE entry IN (@REF_SORKA, @REF_GARAN, @REF_MARAK);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES