-- Making the door to Un'sok openable
UPDATE gameobject_template SET
faction = 0, flags = 0, data1 = 5749660
WHERE entry = 214492;