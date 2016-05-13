-- Highmaul
UPDATE creature_template SET flags_extra = flags_extra | 0x00200000 WHERE entry IN (78714, 77404, 78491, 78948, 80551, 80557, 78237, 78238, 79015, 77428);

-- Blackrock Foundry
UPDATE creature_template SET flags_extra = flags_extra | 0x00200000 WHERE entry IN (76877, 77182, 76806, 76809, 76973, 76974, 76814);