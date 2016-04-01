DELETE FROM disables WHERE entry = 12278;
UPDATE gameobject_template SET ScriptName = "go_dark_iron_mole_machine_wreckage" WHERE entry IN (189989, 189990);
UPDATE game_event SET `length` = `length` + 2*10080 WHERE eventEntry = 24;