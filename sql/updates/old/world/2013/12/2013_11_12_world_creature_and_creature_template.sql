UPDATE creature_template SET IconName = 'taxi' WHERE (npcflag & 8192) AND IconName = '';
UPDATE creature_template SET npcflag = npcflag|0x1 WHERE (npcflag & 8192);
UPDATE creature SET npcflag = npcflag|0x1 WHERE (npcflag & 8192);