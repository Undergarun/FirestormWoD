DELETE FROM `conditions` WHERE `sourcegroup` IN (15555);
DELETE FROM `conditions` WHERE `SourceEntry` IN (137206,101508, 139200);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,
`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
-- (14,15555,22350,0,0,15,0,256,0,0,0,0,'','Jubeka - Show gossip only for Warlocks'),
(14,15555,22350,0,0,28,0,32325,0,0,0,0,'','Jubeka- Show gossip only if quest completed 32325'),
(17,0,139200,0,1,28,0,32325,0,0,0,0,'','Katherand - Spell doesnt proc if player hasnt quest completed 32325'),
(17,0,139200,0,2,8,0,32325,0,0,0,0,'','Katherand - Spell doesnt proc if player hasnt quest rewarded 32325');
