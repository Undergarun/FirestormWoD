-- English texts for Un'sok
DELETE FROM creature_text WHERE entry = 62511;
INSERT INTO creature_text (entry, groupid, id, TEXT, TYPE, probability, sound, COMMENT) VALUES
(62511, 0, 0, "Ah, I see you have found a few of my experiments. Step closer, and you may witness the full glory of my work here.", 14, 100, 31559, "VO_MP_UNSOK_INTRO_01"),
(62511, 1, 0, "You must be powerful, to have come this far. Yes, yes... you will make worthy test subjects.", 14, 100, 31555, "VO_MP_UNSOK_AGRO_01"),
(62511, 2, 0, "Watch your step!", 14, 100, 31563, "VO_MP_UNSOK_SPELL_02"),
(62511, 3, 0, "Feel fortunate to receive this gift.", 14, 100, 31562, "VO_MP_UNSOK_SPELL_01"),
(62511, 4, 0, "From death comes life, and from life, death...", 14, 100, 31564, "VO_MP_UNSOK_SPELL_03"),
(62511, 5, 0, "You must think yourself mighty. How little you know. Behold now, my greatest creation!", 14, 100, 31557, "VO_MP_UNSOK_EVENT_PHASE2_01"),
(62511, 6, 0, "What have you done? No, this will not be how my work ends. I will not allow it!", 14, 100, 31558, "VO_MP_UNSOK_EVENT_PHASE3_01"),
(62511, 7, 0, "Is that all?", 14, 100, 31560, "VO_MP_UNSOK_SLAY_01"),
(62511, 7, 1, "Excellent!", 14, 100, 31561, "VO_MP_UNSOK_SLAY_02"),
(62511, 8, 0, "Forgive me, Empress...", 14, 100, 31556, "VO_MP_UNSOK_DEATH_01");

-- English text for Amber Monstruosity
DELETE FROM creature_text WHERE entry = 62711;
INSERT INTO creature_text (entry, groupid, id, TEXT, TYPE, probability, sound, COMMENT) VALUES
(62711, 0, 0, "Fa... ther...", 14, 100, 29238, "VO_MP_MONSTROSITY_DEATH_01");