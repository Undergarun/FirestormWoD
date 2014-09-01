/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITY_DB2STORES_H
#define TRINITY_DB2STORES_H

#include "Common.h"
#include "DB2fmt.h"
#include "DB2Store.h"
#include "DB2Structure.h"

#include <list>

extern DB2Storage <ItemEntry> sItemStore;
extern DB2Storage <ItemCurrencyCostEntry> sItemCurrencyCostStore;
extern DB2Storage <ItemExtendedCostEntry> sItemExtendedCostStore;
extern DB2Storage <ItemSparseEntry> sItemSparseStore;
extern DB2Storage <SpellReagentsEntry> sSpellReagentsStore;
extern DB2Storage <ItemUpgradeEntry> sItemUpgradeStore;
extern DB2Storage <RulesetItemUpgradeEntry> sRulesetItemUpgradeStore;
extern DB2Storage<SceneScriptEntry> sSceneScriptStore;
extern DB2Storage<SceneScriptPackageEntry> sSceneScriptPackageStore;
extern DB2Storage<BattlePetAbilityEntry> sBattlePetAbilityStore;
extern DB2Storage<BattlePetAbilityEffectEntry> sBattlePetAbilityEffectStore;
extern DB2Storage<BattlePetAbilityTurnEntry> sBattlePetAbilityTurnStore;
extern DB2Storage<BattlePetAbilityStateEntry> sBattlePetAbilityStateStore;
extern DB2Storage<BattlePetStateEntry> sBattlePetStateStore;
extern DB2Storage<BattlePetEffectPropertiesEntry> sBattlePetEffectPropertiesStore;
extern DB2Storage<BattlePetBreedQualityEntry> sBattlePetBreedQualityStore;
extern DB2Storage<BattlePetBreedStateEntry> sBattlePetBreedStateStore;
extern DB2Storage<BattlePetSpeciesEntry> sBattlePetSpeciesStore;
extern DB2Storage<BattlePetSpeciesStateEntry> sBattlePetSpeciesStateStore;
extern DB2Storage<BattlePetSpeciesXAbilityEntry> sBattlePetSpeciesXAbilityStore;

SpellReagentsEntry const* GetSpellReagentEntry(uint32 spellId, uint8 reagent);

void LoadDB2Stores(const std::string& dataPath);

#endif