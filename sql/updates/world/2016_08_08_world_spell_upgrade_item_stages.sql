UPDATE spell_upgrade_item_stage SET InventoryTypeMask = InventoryTypeMask &~ 0x100000 WHERE ItemBonusTreeCategory = 205;
UPDATE spell_upgrade_item_stage SET InventoryTypeMask = InventoryTypeMask | 0x100400 WHERE ItemBonusTreeCategory = 202 AND ItemSubClassMask = 2;
UPDATE spell_upgrade_item_stage SET InventoryTypeMask = InventoryTypeMask | 0x400 WHERE ItemBonusTreeCategory = 201 AND ItemSubClassMask != 0;
UPDATE spell_upgrade_item_stage SET InventoryTypeMask = InventoryTypeMask | 0x400 WHERE ItemBonusTreeCategory = 200 AND ItemSubClassMask = 16;