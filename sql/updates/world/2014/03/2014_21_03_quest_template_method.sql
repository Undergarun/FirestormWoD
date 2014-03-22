	update quest_template set method = 2 WHERE method = 0 AND (RequiredItemCount1 != O OR RequiresItemCount2 != 0 OR RequiresItemCount3 != 0 OR RequiresItemCount4 != 0 OR RequiresItemCount5 != 0);
