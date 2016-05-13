-- Add maxdist for activate anticheat
UPDATE `battleground_template` SET `StartMaxDist` = '15' WHERE `id` = '708';
UPDATE `battleground_template` SET `StartMaxDist` = '15' WHERE `id` = '108';
UPDATE `battleground_template` SET `StartMaxDist` = '15' WHERE `id` = '120';
UPDATE `battleground_template` SET `StartMaxDist` = '15' WHERE `id` = '754';
UPDATE `battleground_template` SET `StartMaxDist` = '15' WHERE `id` = '9';
-- This arena is removed since MoP 
UPDATE `battleground_template` SET `MinLvl` = '101', `MaxLvl` = '101', `Comment` = 'The Ring of Valor (Disabled)' WHERE `id` = '11';