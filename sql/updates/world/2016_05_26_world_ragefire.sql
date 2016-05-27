DELETE FROM creature WHERE id = 61800;
DELETE FROM gameobject WHERE id = 211792;
REPLACE INTO `instance_template`(`map`,`parent`,`script`,`allowMount`) VALUES ( '389','1','instance_ragefire_chasm','0');
UPDATE creature_template SET ScriptName = "boss_lava_guard_gordoth" WHERE entry = 61528;