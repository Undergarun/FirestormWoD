-- Tsulong Heroic : Light of the day
UPDATE creature_template SET
npcflag = 16777216, type_flags2 = 6,
ScriptName = "npc_light_of_day"
WHERE entry = 63337;