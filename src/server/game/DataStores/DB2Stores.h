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

extern DB2Storage <ItemEntry>                       sItemStore;
extern DB2Storage <ItemCurrencyCostEntry>           sItemCurrencyCostStore;
extern DB2Storage <ItemExtendedCostEntry>           sItemExtendedCostStore;
extern DB2Storage <ItemSparseEntry>                 sItemSparseStore;
extern DB2Storage <ItemEffectEntry>                 sItemEffectStore;
extern DB2Storage<PvpItemEntry>                     sPvpItemStore;
extern DB2Storage <ItemModifiedAppearanceEntry>     sItemModifiedAppearanceStore;
extern DB2Storage <ItemAppearanceEntry>             sItemAppearanceStore;
extern DB2Storage <SpellReagentsEntry>              sSpellReagentsStore;
extern DB2Storage <ItemUpgradeEntry>                sItemUpgradeStore;
extern DB2Storage <RulesetItemUpgradeEntry>         sRulesetItemUpgradeStore;
extern DB2Storage <SceneScriptEntry>                sSceneScriptStore;
extern DB2Storage <SceneScriptPackageEntry>         sSceneScriptPackageStore;
extern DB2Storage <TaxiNodesEntry>                  sTaxiNodesStore;
extern DB2Storage <TaxiPathEntry>                   sTaxiPathStore;
extern DB2Storage <TaxiPathNodeEntry>               sTaxiPathNodeStore;
extern DB2Storage <SpellRuneCostEntry>              sSpellRuneCostStore;
extern DB2Storage <SpellCastingRequirementsEntry>   sSpellCastingRequirementsStore;
extern DB2Storage <SpellAuraRestrictionsEntry>      sSpellAuraRestrictionsStore;
extern DB2Storage <AreaPOIEntry>                    sAreaPOIStore;
extern DB2Storage <HolidaysEntry>                   sHolidaysStore;
extern DB2Storage <OverrideSpellDataEntry>          sOverrideSpellDataStore;
extern DB2Storage <SpellMiscEntry>                  sSpellMiscStore;
extern DB2Storage <SpellPowerEntry>                 sSpellPowerStore;
extern DB2Storage <SpellTotemsEntry>                sSpellTotemsStore;
extern DB2Storage <SpellClassOptionsEntry>          sSpellClassOptionsStore;

//////////////////////////////////////////////////////////////////////////
/// Garrison DB2
//////////////////////////////////////////////////////////////////////////
extern DB2Storage <GarrSiteLevelEntry>              sGarrSiteLevelStore;
extern DB2Storage <GarrSiteLevelPlotInstEntry>      sGarrSiteLevelPlotInstStore;
extern DB2Storage <GarrPlotInstanceEntry>           sGarrPlotInstanceStore;
extern DB2Storage <GarrPlotEntry>                   sGarrPlotStore;
extern DB2Storage <GarrPlotUICategoryEntry>         sGarrPlotUICategoryStore;
extern DB2Storage <GarrMissionEntry>                sGarrMissionStore;
extern DB2Storage <GarrBuildingEntry>               sGarrBuildingStore;
extern DB2Storage <GarrPlotBuildingEntry>           sGarrPlotBuildingStore;
extern DB2Storage <GarrFollowerEntry>               sGarrFollowerStore;
extern DB2Storage <GarrAbilityEntry>                sGarrAbilityStore;
extern DB2Storage <GarrFollowerXAbilityEntry>       sGarrFollowerXAbilityStore;
extern DB2Storage <GarrBuildingPlotInstEntry>       sGarrBuildingPlotInstStore;

//////////////////////////////////////////////////////////////////////////
/// Battle pet
//////////////////////////////////////////////////////////////////////////
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
SpellTotemsEntry const* GetSpellTotemEntry(uint32 spellId, uint8 totem);

extern std::map<uint32, std::vector<uint32>> sItemEffectsByItemID;

void LoadDB2Stores(const std::string& dataPath);

struct TaxiPathNodePtr
{
    TaxiPathNodePtr() : i_ptr(NULL) {}
    TaxiPathNodePtr(TaxiPathNodeEntry const* ptr) : i_ptr(ptr) {}
    TaxiPathNodeEntry const* i_ptr;
    operator TaxiPathNodeEntry const& () const { return *i_ptr; }
};

typedef Path<TaxiPathNodePtr, TaxiPathNodeEntry const> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

extern TaxiPathNodesByPath                       sTaxiPathNodesByPath;

#endif