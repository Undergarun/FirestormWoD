/* --- Imperial Vizier Zor'lok --- */
/* Trashs */
-- 63048 - Kor'thik Slicer
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 63048;
UPDATE creature_template SET Health_mod = 34.2 WHERE entry = 463048;
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 563048;
UPDATE creature_template SET Health_mod = 34.2 WHERE entry = 663048;
UPDATE creature_template SET Health_mod = 29 WHERE entry = 763048;
-- 63036 - Kor'thik extremist
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 63036;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 463036;
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 563036;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 663036;
UPDATE creature_template SET Health_mod = 22 WHERE entry = 763036;
-- 63032 - Sra'thik Shield Master
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 63032;
UPDATE creature_template SET Health_mod = 34.2 WHERE entry = 463032;
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 563032;
UPDATE creature_template SET Health_mod = 34.2 WHERE entry = 663032;
UPDATE creature_template SET Health_mod = 29 WHERE entry = 763032;
-- 63049 - Set'thik Swiftblade
UPDATE creature_template SET Health_mod = 9 WHERE entry = 63049;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 463049;
UPDATE creature_template SET Health_mod = 9 WHERE entry = 563049;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 663049;
UPDATE creature_template SET Health_mod = 20 WHERE entry = 763049;
-- 67177 - 66181 - 63853 - Zar'thik Supplicant
UPDATE creature_template SET Health_mod = 4.5 WHERE entry IN (67177, 66181, 63853);
UPDATE creature_template SET Health_mod = 10.8 WHERE entry IN (467177, 466181, 463853);
UPDATE creature_template SET Health_mod = 4.5 WHERE entry IN (567177, 566181, 563853);
UPDATE creature_template SET Health_mod = 10.8 WHERE entry IN (667177, 666181, 663853);
UPDATE creature_template SET Health_mod = 8 WHERE entry IN (767177, 766181, 763853);
-- 63030 - Enslaved Bonesmasher
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 63030;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 463030;
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 563030;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 663030;
UPDATE creature_template SET Health_mod = 24 WHERE entry = 763030;
-- 63031 - Set'thik Fanatic
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 63031;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 463031;
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 563031;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 663031;
UPDATE creature_template SET Health_mod = 15 WHERE entry = 763031;
-- 63035 - Zar'thik Zealot
UPDATE creature_template SET Health_mod = 9 WHERE entry = 63035;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 463035;
UPDATE creature_template SET Health_mod = 9 WHERE entry = 563035;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 663035;
UPDATE creature_template SET Health_mod = 20 WHERE entry = 763035;

/* Boss */
-- Imperial Vizier Zor'lok
UPDATE creature_template SET Health_mod = 360 WHERE entry = 62980;
UPDATE creature_template SET Health_mod = 900 WHERE entry = 462980;
UPDATE creature_template SET Health_mod = 450 WHERE entry = 562980;
UPDATE creature_template SET Health_mod = 1350 WHERE entry = 662980;
UPDATE creature_template SET Health_mod = 500 WHERE entry = 762980;
-- Echo of Attenuation / Echo of Force and Verve (Heroic only)
UPDATE creature_template SET Health_mod = 45 WHERE entry IN (65173, 65174);
UPDATE creature_template SET Health_mod = 135 WHERE entry IN (665173, 665174);

/* --- Blade Lord Ta'yak --- */
/* Trashs */
-- 64358 - Set'thik Tempest
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 64358;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 464358;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 564358;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 664358;
UPDATE creature_template SET Health_mod = 4 WHERE entry = 764358;
-- 64353 - Set'thik Gale-Slicer
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 64353;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 464353;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 564353;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 664353;
UPDATE creature_template SET Health_mod = 4 WHERE entry = 764353;
-- 64357 - Kor'thik Swarmer
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 64357;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 464357;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 564357;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 664357;
UPDATE creature_template SET Health_mod = 4 WHERE entry = 764357;
-- 64355 - Kor'thik Silentwing
UPDATE creature_template SET Health_mod = 1.8 WHERE entry = 64355;
UPDATE creature_template SET Health_mod = 3.6 WHERE entry = 464355;
UPDATE creature_template SET Health_mod = 1.8 WHERE entry = 564355;
UPDATE creature_template SET Health_mod = 3.6 WHERE entry = 664355;
UPDATE creature_template SET Health_mod = 2.5 WHERE entry = 764355;
-- 64338 - Instructor Kli'thak <Keeper of Wind Step>
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 64338;
UPDATE creature_template SET Health_mod = 45 WHERE entry = 464338;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 564338;
UPDATE creature_template SET Health_mod = 45 WHERE entry = 664338;
UPDATE creature_template SET Health_mod = 35 WHERE entry = 764338;
-- 64339 - Instructor Tak'thok <Keeper of Overwhelming Assault>
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 64339;
UPDATE creature_template SET Health_mod = 45 WHERE entry = 464339;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 564339;
UPDATE creature_template SET Health_mod = 45 WHERE entry = 664339;
UPDATE creature_template SET Health_mod = 35 WHERE entry = 764339;
-- 64340 - Instructor Maltik <Keeper of Unseen Strike>
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 64340;
UPDATE creature_template SET Health_mod = 45 WHERE entry = 464340;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 564340;
UPDATE creature_template SET Health_mod = 45 WHERE entry = 664340;
UPDATE creature_template SET Health_mod = 35 WHERE entry = 764340;
-- 64341 - Instructor Zarik <Keeper of Tempest Slash>
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 64341;
UPDATE creature_template SET Health_mod = 31.5 WHERE entry = 464341;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 564341;
UPDATE creature_template SET Health_mod = 31.5 WHERE entry = 664341;
UPDATE creature_template SET Health_mod = 50 WHERE entry = 764341;

/* Boss */
-- Lord Blade Ta'yak
UPDATE creature_template SET Health_mod = 310.5 WHERE entry = 62543;
UPDATE creature_template SET Health_mod = 931.5 WHERE entry = 462543;
UPDATE creature_template SET Health_mod = 405 WHERE entry = 562543;
UPDATE creature_template SET Health_mod = 1210.5 WHERE entry = 662543;
UPDATE creature_template SET Health_mod = 675 WHERE entry = 762543;

/* --- Garalon --- */
/* Trashs */
-- 63593 - Set'thik Zephyrian
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 63593;
UPDATE creature_template SET Health_mod = 40.5 WHERE entry = 463593;
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 563593;
UPDATE creature_template SET Health_mod = 40.5 WHERE entry = 663593;
UPDATE creature_template SET Health_mod = 30 WHERE entry = 763593;
-- 63592 - Set'thik Gustwing
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 63592;
UPDATE creature_template SET Health_mod = 8.1 WHERE entry = 463592;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 563592;
UPDATE creature_template SET Health_mod = 8.1 WHERE entry = 663592;
UPDATE creature_template SET Health_mod = 6 WHERE entry = 763592;

/* Boss */
-- Garalon
UPDATE creature_template SET Health_mod = 450 WHERE entry = 62164;
UPDATE creature_template SET Health_mod = 1350 WHERE entry = 462164;
UPDATE creature_template SET Health_mod = 600 WHERE entry = 562164;
UPDATE creature_template SET Health_mod = 1800 WHERE entry = 662164;
UPDATE creature_template SET Health_mod = 800 WHERE entry = 762164;
-- Garalon's leg
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 63053;
UPDATE creature_template SET Health_mod = 40.5 WHERE entry = 463053;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 563053;
UPDATE creature_template SET Health_mod = 54 WHERE entry = 663053;
UPDATE creature_template SET Health_mod = 24 WHERE entry = 763053;

/* --- Windblade Mel'jarak --- */
/* Boss - no trash */
-- Wind Lord Mel'jarak
UPDATE creature_template SET Health_mod = 558 WHERE entry = 62397;
UPDATE creature_template SET Health_mod = 1763.1 WHERE entry = 462397;
UPDATE creature_template SET Health_mod = 1215 WHERE entry = 563053;
UPDATE creature_template SET Health_mod = 3645 WHERE entry = 663053;
UPDATE creature_template SET Health_mod = 920 WHERE entry = 763053;
-- Kor'thik Elite Blademaster & Sra'thik Amber Trapper & Zar'thik Battle-Mender
UPDATE creature_template SET Health_mod = 117 WHERE entry IN (62402, 62405, 62408);
UPDATE creature_template SET Health_mod = 351 WHERE entry IN (462402, 462405, 462408);
UPDATE creature_template SET Health_mod = 117 WHERE entry IN (562402, 562405, 562408);
UPDATE creature_template SET Health_mod = 351 WHERE entry IN (662402, 662405, 662408);
UPDATE creature_template SET Health_mod = 270 WHERE entry IN (762402, 762405, 762408);

/* --- Amber-shaper Un'sok --- */
/* Trashs */
-- 63597 / 63594 - Coagulated Amber
UPDATE creature_template SET Health_mod = 2.7 WHERE entry IN (63597, 63594);
UPDATE creature_template SET Health_mod = 9 WHERE entry IN (463597, 463594);
UPDATE creature_template SET Health_mod = 2.7 WHERE entry IN (563597, 563594);
UPDATE creature_template SET Health_mod = 9 WHERE entry IN (663597, 663594);
UPDATE creature_template SET Health_mod = 7.5 WHERE entry IN (763597, 763594);
-- 64916 - Kor'thik Swarmguard
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 64916;
UPDATE creature_template SET Health_mod = 50.5 WHERE entry = 469416;
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 569416;
UPDATE creature_template SET Health_mod = 40.5 WHERE entry = 669416;
UPDATE creature_template SET Health_mod = 33.25 WHERE entry = 769416;
-- 64917 - Sra'thik Ambercaller - 63570 - Sra'thik Pool-Tender
UPDATE creature_template SET Health_mod = 9 WHERE entry IN (64917, 63570);
UPDATE creature_template SET Health_mod = 27 WHERE entry IN (464917, 463570);
UPDATE creature_template SET Health_mod = 9 WHERE entry IN (564917, 563570);
UPDATE creature_template SET Health_mod = 27 WHERE entry IN (664917, 663570);
UPDATE creature_template SET Health_mod = 22.5 WHERE entry IN (764917, 763570);
-- 64902 - Kor'thik Fleshrender
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 64902;
UPDATE creature_template SET Health_mod = 67.5 WHERE entry = 464902;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 564902;
UPDATE creature_template SET Health_mod = 67.5 WHERE entry = 664902;
UPDATE creature_template SET Health_mod = 56 WHERE entry = 764902;
-- 63569 - Amber Searsting
UPDATE creature_template SET Health_mod = 36 WHERE entry = 63569;
UPDATE creature_template SET Health_mod = 108 WHERE entry = 463569;
UPDATE creature_template SET Health_mod = 36 WHERE entry = 563569;
UPDATE creature_template SET Health_mod = 108 WHERE entry = 663569;
UPDATE creature_template SET Health_mod = 90 WHERE entry = 763569;
-- 63568 - Amber-Ridden Mushan
UPDATE creature_template SET Health_mod = 18 WHERE entry = 63568;
UPDATE creature_template SET Health_mod = 54 WHERE entry = 463568;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 563568;
UPDATE creature_template SET Health_mod = 54 WHERE entry = 663568;
UPDATE creature_template SET Health_mod = 36 WHERE entry = 763568;

/* Boss */
-- 62511 - Amber-shaper Un'sok
UPDATE creature_template SET Health_mod = 450 WHERE entry = 62511;
UPDATE creature_template SET Health_mod = 1350 WHERE entry = 462511;
UPDATE creature_template SET Health_mod = 702 WHERE entry = 562511;
UPDATE creature_template SET Health_mod = 2106 WHERE entry = 662511;
UPDATE creature_template SET Health_mod = 600 WHERE entry = 762511;
-- 62701 - Mutated Construct
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 62701;
UPDATE creature_template SET Health_mod = 8.1 WHERE entry = 462701;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 562701;
UPDATE creature_template SET Health_mod = 8.1 WHERE entry = 662701;
UPDATE creature_template SET Health_mod = 6 WHERE entry = 762701;
-- 62711 - Amber Monstruosity
UPDATE creature_template SET Health_mod = 135 WHERE entry = 62701;
UPDATE creature_template SET Health_mod = 405 WHERE entry = 462701;
UPDATE creature_template SET Health_mod = 225 WHERE entry = 562701;
UPDATE creature_template SET Health_mod = 675 WHERE entry = 662701;
UPDATE creature_template SET Health_mod = 575 WHERE entry = 762701;
-- 62691 - Living Amber
UPDATE creature_template SET Health_mod = 1.35 WHERE entry = 62691;
UPDATE creature_template SET Health_mod = 4.45 WHERE entry = 462691;
UPDATE creature_template SET Health_mod = 1.62 WHERE entry = 562691;
UPDATE creature_template SET Health_mod = 4.95 WHERE entry = 662691;
UPDATE creature_template SET Health_mod = 2 WHERE entry = 762691;

/* --- Grand Empress Shek'zeer --- */
/* Trashs */
-- 64453 - Set'thik Windblade - 64454 - Zar'thik Augurer
UPDATE creature_template SET Health_mod = 18 WHERE entry IN (64453, 64454);
UPDATE creature_template SET Health_mod = 54 WHERE entry IN (464453, 464454);
UPDATE creature_template SET Health_mod = 18 WHERE entry IN (564453, 564454);
UPDATE creature_template SET Health_mod = 54 WHERE entry IN (664453, 664454);
UPDATE creature_template SET Health_mod = 45 WHERE entry IN (764453, 764454);
-- 64458 - Kor'thik Warsinger
UPDATE creature_template SET Health_mod = 21.6 WHERE entry = 64458;
UPDATE creature_template SET Health_mod = 67.5 WHERE entry = 464458;
UPDATE creature_template SET Health_mod = 21.6 WHERE entry = 564458;
UPDATE creature_template SET Health_mod = 67.5 WHERE entry = 664458;
UPDATE creature_template SET Health_mod = 40 WHERE entry = 764458;

/* Boss */
-- 62837 - Grand Empress Shek'zeer
UPDATE creature_template SET Health_mod = 405 WHERE entry = 62837;
UPDATE creature_template SET Health_mod = 1260 WHERE entry = 462837;
UPDATE creature_template SET Health_mod = 634.5 WHERE entry = 562837;
UPDATE creature_template SET Health_mod = 1940.4 WHERE entry = 62837;
UPDATE creature_template SET Health_mod = 1150 WHERE entry = 762837;
-- 63589 - Set'thik Windblade
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 63589;
UPDATE creature_template SET Health_mod = 40.5 WHERE entry = 463589;
UPDATE creature_template SET Health_mod = 37.8 WHERE entry = 563589;
UPDATE creature_template SET Health_mod = 112.5 WHERE entry = 663589;
UPDATE creature_template SET Health_mod = 30 WHERE entry = 763589;
-- 63591 - Kor'thik Reaver
UPDATE creature_template SET Health_mod = 90 WHERE entry = 63591;
UPDATE creature_template SET Health_mod = 270 WHERE entry = 463591;
UPDATE creature_template SET Health_mod = 134.1 WHERE entry = 563591;
UPDATE creature_template SET Health_mod = 400 WHERE entry = 663591;
UPDATE creature_template SET Health_mod = 50 WHERE entry = 763591;
