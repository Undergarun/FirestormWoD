DELETE FROM `_hotfixs` WHERE `Entry` IN (10002, 10003) AND `Hash` = 3146089301;
INSERT INTO `_hotfixs` (`Entry`, `Hash`, `Date`, `Comment`) VALUES ('10002', '3146089301', UNIX_TIMESTAMP(), 'Fun Exented cost 10002'); 
INSERT INTO `_hotfixs` (`Entry`, `Hash`, `Date`, `Comment`) VALUES ('10003', '3146089301', UNIX_TIMESTAMP(), 'Fun Exented cost 10003'); 

DELETE FROM `item_extended_cost` WHERE `ID` IN (10002, 10003);
INSERT INTO `item_extended_cost` (`ID`, `RequiredPersonalArenaRating`, `RequiredCurrency1`, `RequiredCurrencyCount1`, `BuildVerified`) VALUES ('10002', '2400', '392', '500000', '20779'); 
INSERT INTO `item_extended_cost` (`ID`, `RequiredArenaSlot`, `RequiredPersonalArenaRating`, `RequiredCurrency1`, `RequiredCurrencyCount1`, `BuildVerified`) VALUES ('10003', '1', '1900', '392', '500000', '20779'); 