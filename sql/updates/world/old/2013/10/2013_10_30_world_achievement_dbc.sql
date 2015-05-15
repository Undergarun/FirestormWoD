-- fix startup error with achievements
DELETE FROM `achievement_dbc` WHERE `ID`>=5696 AND `ID`<=5707;