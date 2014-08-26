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

#ifndef TRINITY_DB2SFRM_H
#define TRINITY_DB2SFRM_H

const char Itemfmt[] = "niiiiiiii";
const char ItemCurrencyCostfmt[] = "xn";
const char ItemSparsefmt[] = "niiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifisssssiiiiiiiiiiiiiiiiiiifiiifiii";
const char ItemEffectFmt[] = "niiiiiiii";
const char ItemModifiedAppearanceFmt[] = "niiii";
const char ItemAppearanceFmt[] = "nii";
const char ItemExtendedCostEntryfmt[] = "nxxiiiiiiiiiiiixiiiiiiiiiixxxx";
const char BattlePetSpeciesEntryfmt[] = "inxxxxxxx";
const char SpellReagentsEntryfmt[] = "diiiiiiiiiiiiiiiixx";
const char ItemUpgradeEntryfmt[] = "niiiii";
const char RulesetItemUpgradeEntryfmt[] = "niii";
const char SceneScriptEntryfmt[] = "nssii";
const char SceneScriptPackageEntryfmt[] = "ns";
const char TaxiPathNodeEntryfmt[] = "diiifffiiii";
const char SpellRuneCostfmt[] = "niiiii";
const char SpellCastingRequirementsEntryfmt[] = "dixxixi";
const char SpellAuraRestrictionsEntryfmt[] = "diiiiiixx";
const char AreaPOIEntryfmt[] = "nxxxiixffxxixxx";
const char Holidaysfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiixxisix";
const char OverrideSpellDatafmt[] = "niiiiiiiiiixx";
const char SpellMiscEntryfmt[] = "niiiiiiiiiiiiiiiiifiiiiix";
const char SpellPowerEntryfmt[] = "nixiixxxxxffxx";
const char SpellTotemsEntryfmt[] = "niiii";
const char SpellClassOptionsEntryfmt[] = "dxiiiii";

//////////////////////////////////////////////////////////////////////////
/// Garrison DB2
//////////////////////////////////////////////////////////////////////////
const char GarrSiteLevelEntryfmt[] = "niiiiffiiii";
const char GarrSiteLevelPlotInstEntryfmt[] = "niiffi";
const char GarrPlotInstanceEntryfmt[] = "nis";
const char GarrPlotEntryfmt[] = "niiiiiiii";
const char GarrPlotUICategoryEntryfmt[] = "nsi";
const char GarrMissionEntryfmt[] = "niiiiiiiiisssiiiiiii";
const char GarrBuildingEntryfmt[] = "niiiiissssiiiiiiiiiiiiii";
const char GarrPlotBuildingEntryfmt[] = "nii";
const char GarrFollowerEntryfmt[] = "niiiiiiiiiiiiiissii";
const char GarrAbilityEntryfmt[] = "nissiii";
const char GarrFollowerXAbilityEntryfmt[] = "niii";

#endif
