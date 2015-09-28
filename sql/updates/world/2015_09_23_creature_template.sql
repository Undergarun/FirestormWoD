-- Tameable creature in Frostfire Ridge
UPDATE `creature_template` SET `type_flags` = 0x1 WHERE `entry` IN (76914, 80782, 75416, 78314, 79756, 80241);

-- Tameable creature in Shadowmoon Valley
UPDATE `creature_template` SET `type_flags` = 0x1 WHERE `entry` IN (82308, 77140, 84919, 82354, 79020);
  -- Some are exotic
UPDATE `creature_template` SET `type_flags` = 0x1 + 0x10000 WHERE `entry` IN (88708, 88710);

-- Chromehound
UPDATE `creature_template` SET `type_flags` = 0x1 + 0x10000 WHERE `entry` = 50839;
