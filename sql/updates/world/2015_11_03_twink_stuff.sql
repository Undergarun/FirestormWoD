-- fix Pyroblast Clearcasting Driver
DELETE FROM `spell_proc_event` WHERE `entry`='44448';
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES ('44448', 0x00000001 + 0x00000002 + 0x00000400);
