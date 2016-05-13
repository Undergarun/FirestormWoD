-- Unsing only one NPC from now on Sonic ring
UPDATE creature_template SET ScriptName = "" WHERE entry IN (62694, 62696, 62716, 62717, 62718, 62719, 62726, 62727, 62743, 62744, 62746);