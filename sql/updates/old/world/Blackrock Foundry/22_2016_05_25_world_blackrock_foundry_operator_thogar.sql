# Thogar trashes
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_man_at_arms" WHERE entry = 78832;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_raider" WHERE entry = 81197;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_crack_shot" WHERE entry = 81315;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_firemender" WHERE entry = 87841;

# Trains
UPDATE creature_template SET InhabitType = 4, ScriptName = "npc_foundry_train" WHERE entry = 76907;

REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(81197, 1, 107703, 107703, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(87841, 1, 108772, 107801, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(81315, 1, 0, 0, 108426);

DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_cauterizing_bolt" AND spell_id = 160140;
INSERT INTO spell_script_names VALUE (160140, "spell_foundry_cauterizing_bolt");

DELETE FROM areatrigger_template WHERE spell_id IN (156553);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, morph_curve_id, data0, data1, data2, data3, data4, data5, data6, data7, ScriptName) VALUES
(156553, 0, 6756, 1, 1, 2180, 702, 17.5, 14.25, 5, 17.5, 14.25, 5, 0, 0, "");

SET @REF_THOGAR = 76865;

DELETE FROM creature_text WHERE entry IN (@REF_THOGAR);
INSERT INTO creature_text VALUES
(@REF_THOGAR, 0, 0, "Track one.", 14, 0, 100, 0, 0, 41697, "Track 1_0"),
(@REF_THOGAR, 0, 1, "Live iron on track one!", 14, 0, 100, 0, 0, 41698, "Track 1_1"),
(@REF_THOGAR, 1, 0, "Track two!", 14, 0, 100, 0, 0, 41699, "Track 2_0"),
(@REF_THOGAR, 1, 1, "Incoming on two.", 14, 0, 100, 0, 0, 41700, "Track 2_1"),
(@REF_THOGAR, 2, 0, "Track three.", 14, 0, 100, 0, 0, 41701, "Track 3_0"),
(@REF_THOGAR, 2, 1, "Track three inbound.", 14, 0, 100, 0, 0, 41702, "Track 3_1"),
(@REF_THOGAR, 3, 0, "Track four.", 14, 0, 100, 0, 0, 41703, "Track 4_0"),
(@REF_THOGAR, 3, 1, "Inbound on four.", 14, 0, 100, 0, 0, 41704, "Track 4_1"),
(@REF_THOGAR, 4, 0, "Express, coming through.", 14, 0, 100, 0, 0, 41694, "Express 0"),
(@REF_THOGAR, 4, 1, "Redball incoming!", 14, 0, 100, 0, 0, 41693, "Express 1"),
(@REF_THOGAR, 4, 2, "Send'er on down the line!", 14, 0, 100, 0, 0, 41695, "Express 2"),
(@REF_THOGAR, 4, 3, "Faster! Bat the stack off her!", 14, 0, 100, 0, 0, 41696, "Express 3"),
(@REF_THOGAR, 5, 0, "Blackhand wants to see these shipments in Talador. NOW!", 14, 0, 100, 0, 0, 41709, "Intro"),
(@REF_THOGAR, 6, 0, "Blackhand won't tolerate anymore delays.", 14, 0, 100, 0, 0, 41670, "Aggro"),
(@REF_THOGAR, 7, 0, "Ah - reinforcements.", 14, 0, 100, 0, 0, 41705, "Reinforcements 0"),
(@REF_THOGAR, 7, 1, "Reinforcements, right on time.", 14, 0, 100, 0, 0, 41706, "Reinforcements 1"),
(@REF_THOGAR, 7, 2, "Troop train - inbound!", 14, 0, 100, 0, 0, 41707, "Reinforcements 2"),
(@REF_THOGAR, 7, 3, "Here they come - hit the grit, boys!", 14, 0, 100, 0, 0, 41708, "Reinforcements 3"),
(@REF_THOGAR, 8, 0, "Coming in hot.", 14, 0, 100, 0, 0, 41672, "Iron Gunnery Sergeant 0"),
(@REF_THOGAR, 8, 1, "Here come the boomers!", 14, 0, 100, 0, 0, 41674, "Iron Gunnery Sergeant 1"),
(@REF_THOGAR, 8, 2, "Here's my artillery.", 14, 0, 100, 0, 0, 41673, "Iron Gunnery Sergeant 2"),
(@REF_THOGAR, 9, 0, "The command car is here.", 14, 0, 100, 0, 0, 41675, "Grom'kar Man-At-Arms 0"),
(@REF_THOGAR, 9, 1, "Here comes the brass.", 14, 0, 100, 0, 0, 41676, "Grom'kar Man-At-Arms 1"),
(@REF_THOGAR, 10, 0, "Trains inbound!", 14, 0, 100, 0, 0, 41683, "Triple Train (Mythic only) 0"),
(@REF_THOGAR, 10, 1, "Double time.", 14, 0, 100, 0, 0, 41684, "Triple Train (Mythic only) 1"),
(@REF_THOGAR, 10, 2, "Clear the tracks!", 14, 0, 100, 0, 0, 41685, "Triple Train (Mythic only) 2"),
(@REF_THOGAR, 11, 0, "Let's step up the pace!", 14, 0, 100, 0, 0, 41678, "Deforester Trains (Mythic only) 0"),
(@REF_THOGAR, 11, 1, "You're just in time for the rush!", 14, 0, 100, 0, 0, 41679, "Deforester Trains (Mythic only)) 1"),
(@REF_THOGAR, 11, 2, "I'm not impressed - more trains are inbound!", 14, 0, 100, 0, 0, 41680, "Deforester Trains (Mythic only) 2"),
(@REF_THOGAR, 11, 3, "I have a schedule to keep!", 14, 0, 100, 0, 0, 41681, "Deforester Trains (Mythic only) 3"),
(@REF_THOGAR, 11, 4, "You're running out of time.", 14, 0, 100, 0, 0, 41682, "Deforester Trains (Mythic only) 4"),
(@REF_THOGAR, 12, 0, "Right. On. Time.", 14, 0, 100, 0, 0, 41692, "Killing a player 0"),
(@REF_THOGAR, 12, 1, "All aboard.", 14, 0, 100, 0, 0, 41691, "Killing a player 1"),
(@REF_THOGAR, 12, 2, "Terminated!", 14, 0, 100, 0, 0, 41689, "Killing a player 2"),
(@REF_THOGAR, 12, 3, "End of the line.", 14, 0, 100, 0, 0, 41688, "Killing a player 3"),
(@REF_THOGAR, 12, 4, "Hah, flat as a copper piece.", 14, 0, 100, 0, 0, 41690, "Killing a player 4"),
(@REF_THOGAR, 13, 0, "Its the end of the line!", 14, 0, 100, 0, 0, 41671, "Berserk"),
(@REF_THOGAR, 14, 0, "Couldn't keep up, eh?", 14, 0, 100, 0, 0, 41686, "Wipe 0"),
(@REF_THOGAR, 14, 1, "Hah hah, this timetable is a killer!", 14, 0, 100, 0, 0, 41687, "Wipe 1"),
(@REF_THOGAR, 15, 0, "That wasn't on the... schedule...", 14, 0, 100, 0, 0, 41677, "Death");

DELETE FROM locales_creature_text WHERE entry IN (@REF_THOGAR);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES