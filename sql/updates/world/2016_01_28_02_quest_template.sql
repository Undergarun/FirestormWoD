-- -------------------------------------------
-- www.wowhead.com/npc=63596 Audrey Burnhep - Alliance
-- -------------------------------------------

-- www.wowhead.com/quest=32008 -- Audrey Burnhep
UPDATE `quest_template` SET `NextQuestId`=31316 WHERE `id`=32008;
UPDATE `quest_template` SET `PrevQuestId`=32008,`NextQuestId`=31724 WHERE `id`=31316; 
UPDATE `quest_template` SET `PrevQuestId`=31316,`NextQuestId`=31725 WHERE `id`=31724;
UPDATE `quest_template` SET `PrevQuestId`=31724,`NextQuestId`=31726 WHERE `id`=31725;
UPDATE `quest_template` SET `PrevQuestId`=31725,`NextQuestId`=31729 WHERE `id`=31726;
UPDATE `quest_template` SET `PrevQuestId`=31726,`NextQuestId`=31728 WHERE `id`=31729;
UPDATE `quest_template` SET `PrevQuestId`=31729,`NextQuestId`=31917 WHERE `id`=31728;
UPDATE `quest_template` SET `PrevQuestId`=31728 WHERE `id`=31917;

-- Daily quests of the questline 32008 -- Audrey Burnhep
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31780; -- show daily 31780 in npc 65648 if 31917 is completed.
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31781; -- show daily 31781 in npc 65651 if 31917 is completed.
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31850; -- show daily 31850 in npc 65655 if 31917 is completed.
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31852; -- show daily 31852 in npc 63194 if 31917 is completed.
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31851; -- show daily 31851 in npc 65656 if 31917 is completed.
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31693;

-- www.wowhead.com/quest=31984 -- Battle Pet Tamers: Cataclysm
UPDATE `quest_template` SET `NextQuestId`=31966 WHERE `id`=31984;
UPDATE `quest_template` SET `PrevQuestId`=31984,`NextQuestId`=31970 WHERE `id`=31966; 
UPDATE `quest_template` SET `PrevQuestId`=31966 WHERE `id`=31970;

-- Daily quests of the questline 31984 -- Battle Pet Tamers: Cataclysm
UPDATE `quest_template` SET `PrevQuestId`=31970 WHERE `id`=31971;

-- www.wowhead.com/quest=31902 -- Battle Pet Tamers: Eastern Kingdoms
UPDATE `quest_template` SET `NextQuestId`=31915 WHERE `id`=31902;
UPDATE `quest_template` SET `PrevQuestId`=31902 WHERE `id`=31915;

-- Daily quests of the questline 31902 -- Battle Pet Tamers: Eastern Kingdoms
UPDATE `quest_template` SET `PrevQuestId`=31915 WHERE `id`=31916;

-- www.wowhead.com/quest=31889 -- Battle Pet Tamers: Kalimdor
UPDATE `quest_template` SET `NextQuestId`=31316 WHERE `id`=31889;
UPDATE `quest_template` SET `PrevQuestId`=31889 WHERE `id`=31897;

-- Daily quests of the questline 31889 -- Battle Pet Tamers: Kalimdor
UPDATE `quest_template` SET `PrevQuestId`=31897 WHERE `id`=31909;

-- www.wowhead.com/quest=31981 -- Battle Pet Tamers: Northrend
UPDATE `quest_template` SET `NextQuestId`=31927 WHERE `id`=31981;
UPDATE `quest_template` SET `PrevQuestId`=31981,`NextQuestId`=31928 WHERE `id`=31927; 
UPDATE `quest_template` SET `PrevQuestId`=31927 WHERE `id`=31928;

-- Daily quests of the questline 31981 -- Battle Pet Tamers: Northrend
UPDATE `quest_template` SET `PrevQuestId`=31928 WHERE `id`=31935;

-- www.wowhead.com/quest=31975 -- Battle Pet Tamers: Outland
UPDATE `quest_template` SET `NextQuestId`=31976 WHERE `id`=31975;
UPDATE `quest_template` SET `PrevQuestId`=31975,`NextQuestId`=31919 WHERE `id`=31976; 
UPDATE `quest_template` SET `PrevQuestId`=31976,`NextQuestId`=31920 WHERE `id`=31919;
UPDATE `quest_template` SET `PrevQuestId`=31919 WHERE `id`=31920;

-- Daily quests of the questline 31975 -- Battle Pet Tamers: Outland
UPDATE `quest_template` SET `PrevQuestId`=31920 WHERE `id`=31926;

-- www.wowhead.com/quest=31985 -- Battle Pet Tamers: Pandaria
UPDATE `quest_template` SET `NextQuestId`=31930 WHERE `id`=31985;
UPDATE `quest_template` SET `PrevQuestId`=31985,`NextQuestId`=31951 WHERE `id`=31930; 
UPDATE `quest_template` SET `PrevQuestId`=31930 WHERE `id`=31951;

-- Daily quests of the questline 31985 -- Battle Pet Tamers: Pandaria
UPDATE `quest_template` SET `PrevQuestId`=31951 WHERE `id`=31958;

-- www.wowhead.com/quest=31591 -- Learning the Ropes
UPDATE `quest_template` SET `NextQuestId`=31592 WHERE `id`=31591;
UPDATE `quest_template` SET `PrevQuestId`=31591,`NextQuestId`=31821 WHERE `id`=31592; 
UPDATE `quest_template` SET `PrevQuestId`=31592,`NextQuestId`=31593 WHERE `id`=31821;
UPDATE `quest_template` SET `PrevQuestId`=31821 WHERE `id`=31593;


-- -----------------------------------
-- www.wowhead.com/npc=63626 Varzok - Horde
-- -----------------------------------

-- www.wowhead.com/quest=31983 -- Battle Pet Tamers: Cataclysm
UPDATE `quest_template` SET `NextQuestId`=31967 WHERE `id`=31983;
UPDATE `quest_template` SET `PrevQuestId`=31983,`NextQuestId`=31970 WHERE `id`=31967; 
UPDATE `quest_template` SET `PrevQuestId`=31967 WHERE `id`=31970;

-- Daily quests of the questline 31983 -- Battle Pet Tamers: Cataclysm
UPDATE `quest_template` SET `PrevQuestId`=31970 WHERE `id`=31971;

-- www.wowhead.com/quest=31903 -- Battle Pet Tamers: Eastern Kingdoms
UPDATE `quest_template` SET `NextQuestId`=31915 WHERE `id`=31903;
UPDATE `quest_template` SET `PrevQuestId`=31903 WHERE `id`=31915;

-- Daily quests of the questline 31903 -- Battle Pet Tamers: Eastern Kingdoms
UPDATE `quest_template` SET `PrevQuestId`=31915 WHERE `id`=31916;

-- www.wowhead.com/quest=31891 -- Battle Pet Tamers: Kalimdor
UPDATE `quest_template` SET `NextQuestId`=31897 WHERE `id`=31891;
UPDATE `quest_template` SET `PrevQuestId`=31891 WHERE `id`=31897;

-- Daily quests of the questline 31891 -- Battle Pet Tamers: Kalimdor
UPDATE `quest_template` SET `PrevQuestId`=31897 WHERE `id`=31909;

-- www.wowhead.com/quest=31982 -- Battle Pet Tamers: Northrend
UPDATE `quest_template` SET `NextQuestId`=31929 WHERE `id`=31982;
UPDATE `quest_template` SET `PrevQuestId`=31982,`NextQuestId`=31928 WHERE `id`=31929; 
UPDATE `quest_template` SET `PrevQuestId`=31929 WHERE `id`=31928;

-- Daily quests of the questline 31982 -- Battle Pet Tamers: Northrend
UPDATE `quest_template` SET `PrevQuestId`=31928 WHERE `id`=31935;

-- www.wowhead.com/quest=31977 -- Battle Pet Tamers: Outland
UPDATE `quest_template` SET `NextQuestId`=31980 WHERE `id`=31977;
UPDATE `quest_template` SET `PrevQuestId`=31977,`NextQuestId`=31921 WHERE `id`=31980; 
UPDATE `quest_template` SET `PrevQuestId`=31980,`NextQuestId`=31920 WHERE `id`=31921; 
UPDATE `quest_template` SET `PrevQuestId`=31921 WHERE `id`=31920;

-- Daily quests of the questline 31977 -- Battle Pet Tamers: Outland
UPDATE `quest_template` SET `PrevQuestId`=31920 WHERE `id`=31926;

-- www.wowhead.com/quest=31986 -- Battle Pet Tamers: Pandaria
UPDATE `quest_template` SET `NextQuestId`=31952 WHERE `id`=31986;
UPDATE `quest_template` SET `PrevQuestId`=31986,`NextQuestId`=31951 WHERE `id`=31952; 
UPDATE `quest_template` SET `PrevQuestId`=31952 WHERE `id`=31951;

-- Daily quests of the questline 31986 -- Battle Pet Tamers: Pandaria
UPDATE `quest_template` SET `PrevQuestId`=31951 WHERE `id`=31958;

-- www.wowhead.com/quest=31588 -- Learning the Ropes
UPDATE `quest_template` SET `NextQuestId`=31589 WHERE `id`=31588;
UPDATE `quest_template` SET `PrevQuestId`=31588,`NextQuestId`=31827 WHERE `id`=31589; 
UPDATE `quest_template` SET `PrevQuestId`=31589,`NextQuestId`=31590 WHERE `id`=31827; 
UPDATE `quest_template` SET `PrevQuestId`=31827 WHERE `id`=31590;

-- www.wowhead.com/quest=32009 -- Zunta, The Pet Tamer
UPDATE `quest_template` SET `NextQuestId`=31812 WHERE `id`=32009;
UPDATE `quest_template` SET `PrevQuestId`=32009,`NextQuestId`=31813 WHERE `id`=31812; 
UPDATE `quest_template` SET `PrevQuestId`=31812,`NextQuestId`=31814 WHERE `id`=31813;
UPDATE `quest_template` SET `PrevQuestId`=31813,`NextQuestId`=31815 WHERE `id`=31814;
UPDATE `quest_template` SET `PrevQuestId`=31814,`NextQuestId`=31817 WHERE `id`=31815;
UPDATE `quest_template` SET `PrevQuestId`=31815,`NextQuestId`=31870 WHERE `id`=31817;
UPDATE `quest_template` SET `PrevQuestId`=31817,`NextQuestId`=31918 WHERE `id`=31870;
UPDATE `quest_template` SET `PrevQuestId`=31870 WHERE `id`=31918;

-- Daily quests of the questline 32009 -- Zunta, The Pet Tamer
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31819;
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31854;
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31862;
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31872;
UPDATE `quest_template` SET `PrevQuestId`=31917 WHERE `id`=31904;