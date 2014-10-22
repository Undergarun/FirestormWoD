-- 70095 - Juvenile
UPDATE `creature_template` SET
`faction_A` = 14, `faction_H` = 14,
`mindmg` = 15000, `maxdmg` = 30000,
`baseattacktime` = 2000, `rangeattacktime` = 2000,
`Health_mod` = 3.6, ScriptName = "mob_juvenile"
WHERE `entry` = 70095;