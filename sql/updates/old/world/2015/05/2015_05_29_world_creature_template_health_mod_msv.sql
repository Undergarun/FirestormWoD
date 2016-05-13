/* --- Stone Guard --- */
/* Trashs */
-- Stone Quilen
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 60576;
UPDATE creature_template SET Health_mod = 11.97 WHERE entry = 460576;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 560576;
UPDATE creature_template SET Health_mod = 11.97 WHERE entry = 660576;
UPDATE creature_template SET Health_mod = 8.66 WHERE entry = 760576;
-- Enormous Stone Quilen
UPDATE creature_template SET Health_mod = 18 WHERE entry = 64183;
UPDATE creature_template SET Health_mod = 16.2 WHERE entry = 464183;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 564183;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 664183;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 764183;

/* Boss */
-- Jasper, Jade, Amethyst and Cobalt Guardians
UPDATE creature_template SET unit_class = 1, Health_mod = 270 WHERE entry IN (59915, 60047, 60051, 60043);
UPDATE creature_template SET unit_class = 1, Health_mod = 756 WHERE entry IN (459915, 460047, 460051, 460043);
UPDATE creature_template SET unit_class = 1, Health_mod = 486 WHERE entry IN (559915, 560047, 560051, 560043);
UPDATE creature_template SET unit_class = 1, Health_mod = 1587.6 WHERE entry IN (659915, 660047, 660051, 660043);
UPDATE creature_template SET unit_class = 1, Health_mod = 487.5 WHERE entry IN (759915, 760047, 760051, 760043);

/* --- Feng the Accursed --- */
/* Trashs */
-- Cursed Mogu Sculptures
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 61334;
UPDATE creature_template SET Health_mod = 50.4 WHERE entry = 461334;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 561334;
UPDATE creature_template SET Health_mod = 20.4 WHERE entry = 661334;
UPDATE creature_template SET Health_mod = 42 WHERE entry = 761334;
-- (alternative entry)
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 61989;
UPDATE creature_template SET Health_mod = 40.5 WHERE entry = 461334;
UPDATE creature_template SET Health_mod = 13.5 WHERE entry = 561334;
UPDATE creature_template SET Health_mod = 40.5 WHERE entry = 661334;
UPDATE creature_template SET Health_mod = 33.75 WHERE entry = 761334;

/* Boss */
-- Feng the Accursed
UPDATE creature_template SET Health_mod = 315 WHERE entry = 60009;
UPDATE creature_template SET Health_mod = 945 WHERE entry = 460009;
UPDATE creature_template SET Health_mod = 432 WHERE entry = 560009;
UPDATE creature_template SET Health_mod = 1296 WHERE entry = 660009;
UPDATE creature_template SET Health_mod = 657 WHERE entry = 760009;
-- Soul Fragment (HM only)
UPDATE creature_template SET Health_mod = 7.56 WHERE entry = 60781;
UPDATE creature_template SET Health_mod = 11.2 WHERE entry = 660781;

/* --- Gara'jal the Spirit Binder --- */
/* Trashs */
-- Zandalari Skullcharger
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 60375;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 460375;
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 560375;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 660375;
UPDATE creature_template SET Health_mod = 21.6 WHERE entry = 760375;
-- Zandalari Fire-Dancer
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 60402;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 460402;
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 560402;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 660402;
UPDATE creature_template SET Health_mod = 12 WHERE entry = 760402;
-- Zandalari Infiltrator
UPDATE creature_template SET Health_mod = 9 WHERE entry = 60381;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 460381;
UPDATE creature_template SET Health_mod = 9 WHERE entry = 560381;
UPDATE creature_template SET Health_mod = 22.5 WHERE entry = 660381;
UPDATE creature_template SET Health_mod = 16 WHERE entry = 760381;

/* Boss */
-- Gara'jal the Spirit Binder
UPDATE creature_template SET Health_mod = 243 WHERE entry = 60143;
UPDATE creature_template SET Health_mod = 639 WHERE entry = 460143;
UPDATE creature_template SET Health_mod = 369.9 WHERE entry = 560143;
UPDATE creature_template SET Health_mod = 1120.5 WHERE entry = 660143;
UPDATE creature_template SET Health_mod = 607 WHERE entry = 760143;
-- Spirit Totem
UPDATE creature_template SET Health_mod = 0.54 WHERE entry = 60240;
UPDATE creature_template SET Health_mod = 1.08 WHERE entry = 460240;
UPDATE creature_template SET Health_mod = 0.81 WHERE entry = 560240;
UPDATE creature_template SET Health_mod = 1.62 WHERE entry = 660240;
UPDATE creature_template SET Health_mod = 1.15 WHERE entry = 760240;
-- Shadowy Minion
UPDATE creature_template SET Health_mod = 0.72 WHERE entry = 60184;
UPDATE creature_template SET Health_mod = 1.35 WHERE entry = 460184;
UPDATE creature_template SET Health_mod = 0.9 WHERE entry = 560184;
UPDATE creature_template SET Health_mod = 1.8 WHERE entry = 660184;
UPDATE creature_template SET Health_mod = 0.9 WHERE entry = 760184;
-- (alternative entry)
UPDATE creature_template SET Health_mod = 0.62 WHERE entry = 60940;
UPDATE creature_template SET Health_mod = 0.16 WHERE entry = 460940;
UPDATE creature_template SET Health_mod = 0.77 WHERE entry = 560940;
UPDATE creature_template SET Health_mod = 1.54 WHERE entry = 660940;
UPDATE creature_template SET Health_mod = 0.77 WHERE entry = 760940;

/* --- Spirit Kings --- */
/* Trashs */
-- Sorcerer Mogu
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 61250;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 461250;
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 561250;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 661250;
UPDATE creature_template SET Health_mod = 16 WHERE entry = 761250;
-- Mogu Archer
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 61345;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 461345;
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 561345;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 661345;
UPDATE creature_template SET Health_mod = 16 WHERE entry = 761345;
-- Mounted Mogu
UPDATE creature_template SET Health_mod = 9 WHERE entry = 61341;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 461341;
UPDATE creature_template SET Health_mod = 9 WHERE entry = 561341;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 661341;
UPDATE creature_template SET Health_mod = 21 WHERE entry = 761341;
-- Kingsguard
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 61347;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 461347;
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 561347;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 661347;
UPDATE creature_template SET Health_mod = 15 WHERE entry = 761347;

/* Boss */

-- Zian of the Endless Shadow, Meng the Demented, Qiang the Merciless, Subetai the Swift
UPDATE creature_template SET Health_mod = 90 WHERE entry IN (60701, 60708, 60709, 60710);
UPDATE creature_template SET Health_mod = 270 WHERE entry IN (460701, 460708, 460709, 460710);
UPDATE creature_template SET Health_mod = 135 WHERE entry IN (560701, 560708, 560709, 560710);
UPDATE creature_template SET Health_mod = 405 WHERE entry IN (660701, 660708, 660709, 660710);
UPDATE creature_template SET Health_mod = 160 WHERE entry IN (760701, 760708, 760709, 760710);
-- Pinning Arrow
UPDATE creature_template SET Health_mod = 0.9 WHERE entry = 60958;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 460958;
UPDATE creature_template SET Health_mod = 2.25 WHERE entry = 560958;
UPDATE creature_template SET Health_mod = 9.45 WHERE entry = 660958;
UPDATE creature_template SET Health_mod = 1.5 WHERE entry = 760958;
-- Undying Shadow
UPDATE creature_template SET Health_mod = 1.35 WHERE entry = 60731;
UPDATE creature_template SET Health_mod = 4.05 WHERE entry = 460731;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 560731;
UPDATE creature_template SET Health_mod = 8.1 WHERE entry = 660731;
UPDATE creature_template SET Health_mod = 1 WHERE entry = 760731;

/* --- Elegon --- */
/* Trashs */
-- Mogu Secret Keeper
UPDATE creature_template SET Health_mod = 27 WHERE entry = 61131;
UPDATE creature_template SET Health_mod = 81 WHERE entry = 461131;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 561131;
UPDATE creature_template SET Health_mod = 81 WHERE entry = 61131;
UPDATE creature_template SET Health_mod = 50 WHERE entry = 61131;
-- Mogu Warden
UPDATE creature_template SET Health_mod = 27 WHERE entry = 64061;
UPDATE creature_template SET Health_mod = 54 WHERE entry = 464061;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 564061;
UPDATE creature_template SET Health_mod = 54 WHERE entry = 664061;
UPDATE creature_template SET Health_mod = 30 WHERE entry = 764061;
-- (alternative entry)
UPDATE creature_template SET Health_mod = 18 WHERE entry = 64947;
UPDATE creature_template SET Health_mod = 54 WHERE entry = 464947;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 564947;
UPDATE creature_template SET Health_mod = 54 WHERE entry = 664947;
UPDATE creature_template SET Health_mod = 30 WHERE entry = 764947;
-- Mogu'shan Arcanist
UPDATE creature_template SET Health_mod = 12.15 WHERE entry = 64063;
UPDATE creature_template SET Health_mod = 36.45 WHERE entry = 464063;
UPDATE creature_template SET Health_mod = 12.15 WHERE entry = 564063;
UPDATE creature_template SET Health_mod = 36.45 WHERE entry = 664063;
UPDATE creature_template SET Health_mod = 24 WHERE entry = 764063;
-- Mogu'shan Engine Keeper
UPDATE creature_template SET Health_mod = 21.6 WHERE entry = 64068;
UPDATE creature_template SET Health_mod = 64.8 WHERE entry = 464068;
UPDATE creature_template SET Health_mod = 21.6 WHERE entry = 564068;
UPDATE creature_template SET Health_mod = 64.8 WHERE entry = 664068;
UPDATE creature_template SET Health_mod = 36 WHERE entry = 764068;

/* Boss */
-- Elegon
UPDATE creature_template SET Health_mod = 607.5 WHERE entry = 60410;
UPDATE creature_template SET Health_mod = 1822.5 WHERE entry = 460410;
UPDATE creature_template SET Health_mod = 701.1 WHERE entry = 560410;
UPDATE creature_template SET Health_mod = 2102.4 WHERE entry = 660410;
UPDATE creature_template SET Health_mod = 700 WHERE entry = 760410;
-- Empyreal Focus
UPDATE creature_template SET Health_mod = 3.15 WHERE entry = 60776;
UPDATE creature_template SET Health_mod = 9.9 WHERE entry = 460410;
UPDATE creature_template SET Health_mod = 4.5 WHERE entry = 560410;
UPDATE creature_template SET Health_mod = 13.95 WHERE entry = 660410;
UPDATE creature_template SET Health_mod = 8.5 WHERE entry = 760410;
-- Celestial Protector
UPDATE creature_template SET Health_mod = 9 WHERE entry = 60793;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 460793;
UPDATE creature_template SET Health_mod = 8.775 WHERE entry = 560793;
UPDATE creature_template SET Health_mod = 26.325 WHERE entry = 660793;
UPDATE creature_template SET Health_mod = 20 WHERE entry = 760793;
-- Cosmic Spark
UPDATE creature_template SET Health_mod = 1.8 WHERE entry = 62618;
UPDATE creature_template SET Health_mod = 5.4 WHERE entry = 462618;
UPDATE creature_template SET Health_mod = 2.7 WHERE entry = 562618;
UPDATE creature_template SET Health_mod = 8.1 WHERE entry = 662618;
UPDATE creature_template SET Health_mod = 2.5 WHERE entry = 762618;
-- Energy Charge
UPDATE creature_template SET Health_mod = 1.125 WHERE entry = 60913;
UPDATE creature_template SET Health_mod = 3.375 WHERE entry = 460913;
UPDATE creature_template SET Health_mod = 1.687 WHERE entry = 560913;
UPDATE creature_template SET Health_mod = 5.062 WHERE entry = 660913;
UPDATE creature_template SET Health_mod = 2.25 WHERE entry = 760913;

/* --- Will of Emperor --- */
/* Boss (no trash) */
-- Qin-Xi & Jan-Xi
UPDATE creature_template SET Health_mod = 324 WHERE entry IN (60399, 60400);
UPDATE creature_template SET Health_mod = 972 WHERE entry IN (460399, 460400);
UPDATE creature_template SET Health_mod = 486 WHERE entry IN (560399, 560400);
UPDATE creature_template SET Health_mod = 1458 WHERE entry IN (660399, 660400);
UPDATE creature_template SET Health_mod = 450 WHERE entry IN (760399, 760400);
-- Emperor's Rage
UPDATE creature_template SET Health_mod = 3.6 WHERE entry = 60396;
UPDATE creature_template SET Health_mod = 7.2 WHERE entry = 460396;
UPDATE creature_template SET Health_mod = 6.3 WHERE entry = 560396;
UPDATE creature_template SET Health_mod = 10.8 WHERE entry = 660396;
UPDATE creature_template SET Health_mod = 6 WHERE entry = 760396;
-- Emperor's Strength
UPDATE creature_template SET Health_mod = 9 WHERE entry = 60397;
UPDATE creature_template SET Health_mod = 18 WHERE entry = 460396;
UPDATE creature_template SET Health_mod = 16.2 WHERE entry = 560396;
UPDATE creature_template SET Health_mod = 27 WHERE entry = 660396;
UPDATE creature_template SET Health_mod = 12.5 WHERE entry = 760396;
-- Emperor's Courage
UPDATE creature_template SET Health_mod = 3.96 WHERE entry = 60398;
UPDATE creature_template SET Health_mod = 7.93 WHERE entry = 460398;
UPDATE creature_template SET Health_mod = 6.75 WHERE entry = 560398;
UPDATE creature_template SET Health_mod = 11.89 WHERE entry = 660398;
UPDATE creature_template SET Health_mod = 5.5 WHERE entry = 760398;
-- Titan Spark (Heroic only)
UPDATE creature_template SET Health_mod = 3.48 WHERE entry = 60480;
UPDATE creature_template SET Health_mod = 5.4 WHERE entry = 660480;
