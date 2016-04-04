/* --- Protectors of the Endless --- */
/* Trash */
-- Apparition of Fear - 64368
UPDATE creature_template SET Health_mod = 31.5 WHERE entry = 64368;
UPDATE creature_template SET Health_mod = 94.5 WHERE entry = 464368;
UPDATE creature_template SET Health_mod = 31.5 WHERE entry = 564368;
UPDATE creature_template SET Health_mod = 94.5 WHERE entry = 664368;
UPDATE creature_template SET Health_mod = 79 WHERE entry = 764368;
-- Apparition of Terror - 66100
UPDATE creature_template SET Health_mod = 31.5 WHERE entry = 66100;
UPDATE creature_template SET Health_mod = 94.5 WHERE entry = 466100;
UPDATE creature_template SET Health_mod = 31.5 WHERE entry = 566100;
UPDATE creature_template SET Health_mod = 94.5 WHERE entry = 666100;
UPDATE creature_template SET Health_mod = 79 WHERE entry = 766100;

/* Boss */
-- Ancient Regail - 60585 and Ancient Asani - 60586
UPDATE creature_template SET Health_mod = 124.74 WHERE entry IN (60585, 60586);
UPDATE creature_template SET Health_mod = 374.22 WHERE entry IN (460585, 460586);
UPDATE creature_template SET Health_mod = 189 WHERE entry IN (560585, 560586);
UPDATE creature_template SET Health_mod = 567 WHERE entry IN (660585, 660586);
UPDATE creature_template SET Health_mod = 235 WHERE entry IN (760585, 760586);
-- Protector Kaolan - 60583
UPDATE creature_template SET Health_mod = 192.06 WHERE entry = 60583;
UPDATE creature_template SET Health_mod = 576.18 WHERE entry = 460583;
UPDATE creature_template SET Health_mod = 332.10 WHERE entry = 560583;
UPDATE creature_template SET Health_mod = 997.2 WHERE entry = 660583;
UPDATE creature_template SET Health_mod = 360 WHERE entry = 760583;
-- Corrupted Waters - 60621
UPDATE creature_template SET Health_mod = 4.95 WHERE entry = 60621;
UPDATE creature_template SET Health_mod = 14.85 WHERE entry = 460621;
UPDATE creature_template SET Health_mod = 8.64 WHERE entry = 560621;
UPDATE creature_template SET Health_mod = 23.76 WHERE entry = 660621;
UPDATE creature_template SET Health_mod = 5 WHERE entry = 760621;
-- Minion of Fear - 60885 (Heroic Mode only)
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 60885;
UPDATE creature_template SET Health_mod = 8.1 WHERE entry = 660885;

/* --- Tsulong --- */
/* Boss - no trash */
-- Tsulong - 62442
UPDATE creature_template SET Health_mod = 360 WHERE entry = 62442;
UPDATE creature_template SET Health_mod = 1080 WHERE entry = 462442;
UPDATE creature_template SET Health_mod = 576 WHERE entry = 562442;
UPDATE creature_template SET Health_mod = 1728 WHERE entry = 662442;
UPDATE creature_template SET Health_mod = 950 WHERE entry = 762442;
-- Dark of Night - 63346 (Heroic Mode only)
UPDATE creature_template SET Health_mod = 1.35 WHERE entry IN (63346, 663346);
-- Light of Day - 63337 (Heroic Mode only)
UPDATE creature_template SET Health_mod = 1.35 WHERE entry IN (63337, 663337);
-- Embodied Terror - 62969
UPDATE creature_template SET Health_mod = 7.92 WHERE entry = 62969;
UPDATE creature_template SET Health_mod = 23.76 WHERE entry = 462969;
UPDATE creature_template SET Health_mod = 12.96 WHERE entry = 562969;
UPDATE creature_template SET Health_mod = 37.8 WHERE entry = 662969;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 762969;
-- Tiny Terror - 62977
UPDATE creature_template SET Health_mod = 1.485 WHERE entry = 62977;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 462977;
UPDATE creature_template SET Health_mod = 4.86 WHERE entry = 562977;
UPDATE creature_template SET Health_mod = 14.5 WHERE entry = 662977;
UPDATE creature_template SET Health_mod = 0.7 WHERE entry = 762977;
-- Unstable sha - 62919
UPDATE creature_template SET Health_mod = 2.205 WHERE entry = 62919;
UPDATE creature_template SET Health_mod = 2.205 WHERE entry = 462919;
UPDATE creature_template SET Health_mod = 4.77 WHERE entry = 562919;
UPDATE creature_template SET Health_mod = 4.77 WHERE entry = 662919;
UPDATE creature_template SET Health_mod = 1.2 WHERE entry = 762919;

/* --- Lei Shi --- */
/* Boss - no trash */
-- Lei Shi - 62983
UPDATE creature_template SET Health_mod = 285.12 WHERE entry = 62983;
UPDATE creature_template SET Health_mod = 855.36 WHERE entry = 462983;
UPDATE creature_template SET Health_mod = 622.08 WHERE entry = 562983;
UPDATE creature_template SET Health_mod = 1866.24 WHERE entry = 662983;
UPDATE creature_template SET Health_mod = 589.68 WHERE entry = 762983;
-- Animated Protector - 62995
UPDATE creature_template SET Health_mod = 12.87 WHERE entry = 62995;
UPDATE creature_template SET Health_mod = 39.897 WHERE entry = 462995;
UPDATE creature_template SET Health_mod = 18.252 WHERE entry = 562995;
UPDATE creature_template SET Health_mod = 56.583 WHERE entry = 662995;
UPDATE creature_template SET Health_mod = 27.5 WHERE entry = 762995;

/* --- Sha of fear --- /
/* Boss - no trash */
-- Sha of Fear - 60999
UPDATE creature_template SET Health_mod = 381.15 WHERE entry = 60999;
UPDATE creature_template SET Health_mod = 1143.45 WHERE entry = 460999;
UPDATE creature_template SET Health_mod = 1122.66 WHERE entry = 560999;
UPDATE creature_template SET Health_mod = 3367.98 WHERE entry = 660999;
UPDATE creature_template SET Health_mod = 788.30 WHERE entry = 760999;
-- Terror Spawn - 61034
UPDATE creature_template SET Health_mod = 7.974 WHERE entry = 61034;
UPDATE creature_template SET Health_mod = 23.913 WHERE entry = 461034;
UPDATE creature_template SET Health_mod = 13.041 WHERE entry = 561034;
UPDATE creature_template SET Health_mod = 39.123 WHERE entry = 661034;
UPDATE creature_template SET Health_mod = 7.15 WHERE entry = 761034;
-- Shrine Guardians : Yang Guoshi - 61038 - Shrine Guardian Sheng Kang - 61042 - Guardian Jinlun Kun - 61046
UPDATE creature_template SET Health_mod = 29.7 WHERE entry IN (61038, 61042, 61046);
UPDATE creature_template SET Health_mod = 29.7 WHERE entry IN (461038, 461042, 461046);
UPDATE creature_template SET Health_mod = 48.6 WHERE entry IN (561038, 561042, 561046);
UPDATE creature_template SET Health_mod = 48.6 WHERE entry IN (661038, 661042, 661046);
UPDATE creature_template SET Health_mod = 14.25 WHERE entry IN (761038, 761042, 761046);
-- Dread Spawn - 61003 (Heroic Mode only)
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 61003;
UPDATE creature_template SET Health_mod = 32.4 WHERE entry = 661003;
