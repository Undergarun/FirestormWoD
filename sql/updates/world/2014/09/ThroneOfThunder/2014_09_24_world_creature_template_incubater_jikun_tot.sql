-- Incubater
UPDATE `creature_template` SET
`minlevel` = 92, `maxlevel` = 92,
`faction_A` = 14, `faction_H` = 14,
`baseattacktime` = 2000, `rangeattacktime` = 2000,
`unit_flags` = 33554688, `unit_flags2` = 2623488,
scriptname = "mob_incubater"
WHERE `entry` = 69626;