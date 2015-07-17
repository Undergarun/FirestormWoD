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

char const PathNodeEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_END
};

char const LocationEntryfmt[] =
{
    FT_INDEX,
    FT_FLOAT,
    FT_FLOAT,
    FT_FLOAT,
    FT_FLOAT,
    FT_FLOAT,
    FT_FLOAT,
    FT_END
};

const char CurrencyTypesfmt[] = "nisssiiiiiis";
char const CurvePointEntryfmt[] = "niiff";
const char GroupFinderActivityfmt[] = "niiiiiiiiiiiiss";
const char GroupFinderCategoryfmt[] = "nsii";
const char Itemfmt[] = "niiiiiiii";
const char ItemBonusfmt[] = "diiiii";
const char ItemBonusTreeNodefmt[] = "niiii";
const char ItemXBonusTreefmt[] = "nii";
const char ItemCurrencyCostfmt[] = "in";
const char ItemSparsefmt[] = "niiiiffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffffffiiifisssssiiiiiiiiiiiiiiiiiiifiiifiii";
const char ItemEffectFmt[] = "niiiiiiii";
const char HeirloomFmt[] = "nissiiiiiiii";
const char PvpItemfmt[] = "ini";
const char ItemModifiedAppearanceFmt[] = "niiiii";
const char ItemAppearanceFmt[] = "nii";
const char ItemExtendedCostEntryfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiii";
const char SpellReagentsEntryfmt[] = "diiiiiiiiiiiiiiiiii";
const char ItemUpgradeEntryfmt[] = "niiiii";
const char RulesetItemUpgradeEntryfmt[] = "niii";
const char SceneScriptEntryfmt[] = "nssii";
const char SceneScriptPackageEntryfmt[] = "ns";
const char TaxiNodesEntryfmt[] = "nifffsiiiiiff";
const char TaxiPathEntryfmt[] = "niii";
const char TaxiPathNodeEntryfmt[] = "niiifffiiii";
const char SpellRuneCostfmt[] = "niiiii";
const char SpellCastingRequirementsEntryfmt[] = "niiiiii";
const char SpellAuraRestrictionsEntryfmt[] = "niiiiiiii";
const char AreaPOIEntryfmt[] = "niiiiiiffssiiii";
const char Holidaysfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiisii";
const char OverrideSpellDatafmt[] = "niiiiiiiiiiii";
const char SpellEffectGroupSizefmt[] = "inf";
const char SpellMiscEntryfmt[] = "niiiiiiiiiiiiiiiiifiiiiif";
const char SpellPowerEntryfmt[] = "niiiiiiiiiffif";
const char SpellTotemsEntryfmt[] = "niiii";
const char SpellClassOptionsEntryfmt[] = "niiiiii";
const char SoundEntriesfmt[] = "nisiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiififfiifffffii";
const char MapChallengeModeEntryfmt[] = "niiiiiiiii";
const char QuestPackageItemEntryfmt[] = "niiii";
const char MountEntryfmt[] = "niiisssiii";
const char PlayerConditionEntryfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiisiii";
const char VignetteEntryfmt[] = "nsiiff";
const char WbAccessControlListfmt[] = "nsiiii";
const char WbCertWhitelistfmt[] =
{
    FT_INDEX,
    FT_STRING,
    FT_BYTE,
    FT_BYTE,
    FT_INT,
    FT_END
};

const char GlyphRequiredSpecfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_END
};

//////////////////////////////////////////////////////////////////////////
/// Garrison DB2
//////////////////////////////////////////////////////////////////////////
const char GarrSiteLevelEntryfmt[] = "niiiiffiiii";
const char GarrSiteLevelPlotInstEntryfmt[] = "niiffi";
const char GarrPlotInstanceEntryfmt[] = "nis";
const char GarrPlotEntryfmt[] = "niiisiiii";
const char GarrPlotUICategoryEntryfmt[] = "nsi";
const char GarrMissionEntryfmt[] = "niiiiiiiisssiiiiiii";
const char GarrMissionRewardEntryfmt[] = "niiiiiiiii";
const char GarrMissionXEncouterEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_END
};
const char GarrBuildingEntryfmt[] = "niiiiissssiiiiiiiiiiiiii";
const char GarrPlotBuildingEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_END
};
const char GarrFollowerEntryfmt[] = "niiiiiiiiiiiiiissiiii";
const char GarrAbilityEntryfmt[] = "nissiii";
const char GarrAbilityEffectEntryfmt[] = "niiiiiffiii";
const char GarrFollowerXAbilityEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_END
};
const char GarrBuildingPlotInstEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_END
};
const char GarrMechanicTypeEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_STRING,
    FT_STRING,
    FT_INT,
    FT_END
};
const char GarrMechanicEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_FLOAT,
    FT_END
};
const char GarrEncouterXMechanicEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_END
};
const char GarrFollowerLevelXPEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_NA,
    FT_END
};
const char GarrSpecializationEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_FLOAT,
    FT_INT,
    FT_STRING,
    FT_STRING,
    FT_END
};
const char CharShipmentEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_END
};
const char CharShipmentContainerEntryfmt[] =
{
    FT_INDEX,
    FT_INT,
    FT_INT,
    FT_STRING,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_INT,
    FT_STRING,
    FT_INT,
    FT_INT,
    FT_END
};

const char BattlePetAbilityfmt[] = "niiiiiss";
const char BattlePetSpeciesEntryfmt[] = "iniiiiiss";
const char BattlePetAbilityEffectfmt[] = "niiiiiiiiiii";
const char BattlePetAbilityTurnfmt[] = "niiiii";
const char BattlePetAbilityStatefmt[] = "niii";
const char BattlePetStatefmt[] = "nisi";
const char BattlePetEffectPropertiesfmt[] = "nissssssiiiiii";
const char BattlePetBreedQualityfmt[] = "nif";
const char BattlePetBreedStatefmt[] = "niii";
const char BattlePetSpeciesfmt[] = "niiiiiiss";
const char BattlePetSpeciesStatefmt[] = "niii";
const char BattlePetSpeciesXAbilityfmt[] = "niiii";

#endif
