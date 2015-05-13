-- 68202 - Ji-Kun Fledgling's Egg
-- 69628 - Mature Egg of Ji-Kun
UPDATE `creature_template` SET
`faction_A` = 14, `faction_H` = 14,
`baseattacktime` = 2000, `rangeattacktime` = 2000,
`unit_flags` = 256, `unit_flags2` = 2048,
`Health_mod` = 4,
`ScriptName` = 'mob_fledgling_egg' WHERE `entry` IN (68202, 69628);