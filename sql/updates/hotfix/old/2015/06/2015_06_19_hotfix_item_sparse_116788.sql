UPDATE `item_sparse` SET `BuyPrice` = '1000000000' , `SellPrice` = '0', `Flags` = '4096' WHERE `ID` = '116788'; 
DELETE FROM `_hotfixs` WHERE Entry = 116788;
INSERT INTO `_hotfixs` (`Entry`, `Date`, `Comment`) VALUES ('116788', UNIX_TIMESTAMP(), 'ItemSparse Fix price of 116788'); 
UPDATE `_hotfixs` SET `Hash` = '2442913102' WHERE `ID` = '231'; 