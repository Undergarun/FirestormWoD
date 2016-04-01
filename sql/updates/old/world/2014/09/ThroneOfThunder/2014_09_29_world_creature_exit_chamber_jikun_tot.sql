-- 70734 - Exit Chamber [DNT]
UPDATE `creature_template` SET
`modelid1` = 46710,
`minlevel` = 90, `maxlevel` = 90,
`baseattacktime` = 2000, `rangeattacktime` = 2000,
`unit_flags` = 512, `unit_flags2` = 2048,
ScriptName = "mob_jikun_exit_chamber"
WHERE `entry` = 70734;