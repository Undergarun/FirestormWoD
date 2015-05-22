-- 69885 - Jump to Boss Platform
UPDATE `creature_template` SET 
`modelid1` = 11686, 
`minlevel` = 90, `maxlevel` = 90, 
`baseattacktime` = 2000, `rangeattacktime` = 2000,
`unit_flags` = 512, `unit_flags2` = 2048
WHERE `entry` = 69885;