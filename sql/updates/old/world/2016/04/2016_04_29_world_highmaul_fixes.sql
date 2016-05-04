# LFR wall near Tectus
UPDATE gameobject SET spawnMask = 0 WHERE id = 239126 AND map = 1228;

# Kargath Bladefist flags
UPDATE creature_template SET unit_flags = 320 WHERE entry = 78714;