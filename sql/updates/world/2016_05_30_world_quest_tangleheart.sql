-- Quest 36564 - Bonus Objective: Tangleheart
UPDATE `quest_template_objective` SET Flags=0, `ObjectID`=232491, `Type`=2 WHERE ID = 274919;
UPDATE `gameobject_template` SET `ScriptName`="go_gorgrond_pollen_pod" WHERE `entry`=232491;
UPDATE `gameobject` SET `spawntimesecs`=60 WHERE `id`=232491;