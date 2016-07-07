////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AchievementMgr.h"
#include "DB2Stores.h"
#include "DBCStores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "DB2fmt.h"
#include "Item.h"
#include "Common.h"

std::map<uint32, DB2StorageBase*> sDB2PerHash;

DB2Storage <PathNodeEntry>                  sPathNodeStore(PathNodeEntryfmt);
DB2Storage <LocationEntry>                  sLocationStore(LocationEntryfmt);

std::map<uint32 /*curveID*/, std::map<uint32/*index*/, CurvePointEntry const*, std::greater<uint32>>> HeirloomCurvePoints;
std::unordered_map<uint32 /*ItemID*/, HeirloomEntry const*> HeirloomEntryByItemID;
std::map<uint32 /*itemID*/, uint32 /*filedataID*/> g_ItemFileDataId;
std::map<uint32, uint32> g_ItemDisplayIDs;
std::map<uint32, uint32> g_ItemTemplateDisplayIDs;
std::map<uint32, uint32> g_FollowerAbilitiesClass;

DB2Storage <AchievementEntry>               sAchievementStore(Achievementfmt);
DB2Storage <CriteriaEntry>                  sCriteriaStore(Criteriafmt);
DB2Storage <CriteriaTreeEntry>              sCriteriaTreeStore(CriteriaTreefmt);
DB2Storage <ModifierTreeEntry>              sModifierTreeStore(ModifierTreefmt);

DB2Storage <CurrencyTypesEntry>             sCurrencyTypesStore(CurrencyTypesfmt);
DB2Storage <CurvePointEntry>                sCurvePointStore(CurvePointEntryfmt);
DB2Storage <GroupFinderActivityEntry>       sGroupFinderActivityStore(GroupFinderActivityfmt);
DB2Storage <GroupFinderCategoryEntry>       sGroupFinderCategoryStore(GroupFinderCategoryfmt);
DB2Storage <ItemEntry>                      sItemStore(Itemfmt);
DB2Storage <ItemBonusEntry>                 sItemBonusStore(ItemBonusfmt);
DB2Storage <ItemBonusTreeNodeEntry>         sItemBonusTreeNodeStore(ItemBonusTreeNodefmt);
DB2Storage <ItemXBonusTreeEntry>            sItemXBonusTreeStore(ItemXBonusTreefmt);
DB2Storage <ItemCurrencyCostEntry>          sItemCurrencyCostStore(ItemCurrencyCostfmt);
DB2Storage <ItemExtendedCostEntry>          sItemExtendedCostStore(ItemExtendedCostEntryfmt);
DB2Storage <ItemSparseEntry>                sItemSparseStore(ItemSparsefmt);
DB2Storage <ItemEffectEntry>                sItemEffectStore(ItemEffectFmt);
DB2Storage <HeirloomEntry>                  sHeirloomStore(HeirloomFmt);
DB2Storage <PvpItemEntry>                   sPvpItemStore(PvpItemfmt);
DB2Storage <QuestV2CliTaskEntry>            sQuestV2CliTaskStore(QuestV2CliTaskFmt);
DB2Storage <QuestPOIPointCliTaskEntry>      sQuestPOIPointCliTaskStore(QuestPOIPointCliTaskfmt);
DB2Storage <ItemModifiedAppearanceEntry>    sItemModifiedAppearanceStore(ItemModifiedAppearanceFmt);
DB2Storage <ItemAppearanceEntry>            sItemAppearanceStore(ItemAppearanceFmt);
DB2Storage <SpellReagentsEntry>             sSpellReagentsStore(SpellReagentsEntryfmt);
DB2Storage <SpellReagentsCurrencyEntry>     sSpellReagentsCurrencyStore(SpellReagentsCurrencyfmt);
DB2Storage <ItemUpgradeEntry>               sItemUpgradeStore(ItemUpgradeEntryfmt);
DB2Storage <RulesetItemUpgradeEntry>        sRulesetItemUpgradeStore(RulesetItemUpgradeEntryfmt);
DB2Storage <SceneScriptEntry>               sSceneScriptStore(SceneScriptEntryfmt);
DB2Storage <SceneScriptPackageEntry>        sSceneScriptPackageStore(SceneScriptPackageEntryfmt);
DB2Storage <SoundEntriesEntry>              sSoundEntriesStore(SoundEntriesfmt);
DB2Storage <TaxiNodesEntry>                 sTaxiNodesStore(TaxiNodesFormat);
DB2Storage <TaxiPathEntry>                  sTaxiPathStore(TaxiPathFormat);
DB2Storage <TaxiPathNodeEntry>              sTaxiPathNodeStore(TaxiPathNodeFormat);
DB2Storage <SpellEffectGroupSizeEntry>      sSpellEffectGroupSizeStore(SpellEffectGroupSizefmt);
DB2Storage <SpellRuneCostEntry>             sSpellRuneCostStore(SpellRuneCostfmt);
DB2Storage <SpellCastingRequirementsEntry>  sSpellCastingRequirementsStore(SpellCastingRequirementsEntryfmt);
DB2Storage <SpellAuraRestrictionsEntry>     sSpellAuraRestrictionsStore(SpellAuraRestrictionsEntryfmt);
DB2Storage <AreaPOIEntry>                   sAreaPOIStore(AreaPOIEntryfmt);
DB2Storage <HolidaysEntry>                  sHolidaysStore(Holidaysfmt);
DB2Storage <OverrideSpellDataEntry>         sOverrideSpellDataStore(OverrideSpellDatafmt);
DB2Storage <SpellMiscEntry>                 sSpellMiscStore(SpellMiscEntryfmt);
DB2Storage <SpellPowerEntry>                sSpellPowerStore(SpellPowerEntryfmt);
DB2Storage <SpellTotemsEntry>               sSpellTotemsStore(SpellTotemsEntryfmt);
DB2Storage <SpellClassOptionsEntry>         sSpellClassOptionsStore(SpellClassOptionsEntryfmt);
DB2Storage <SpellXSpellVisualEntry>         sSpellXSpellVisualStore(SpellXSpellVisualfmt);
DB2Storage <MapChallengeModeEntry>          sMapChallengeModeStore(MapChallengeModeEntryfmt);
DB2Storage <QuestPackageItemEntry>          sQuestPackageItemStore(QuestPackageItemEntryfmt);
DB2Storage <MountEntry>                     sMountStore(MountEntryfmt);
DB2Storage <PlayerConditionEntry>           sPlayerConditionStore(PlayerConditionEntryfmt);
DB2Storage <VignetteEntry>                  sVignetteStore(VignetteEntryfmt);
DB2Storage <GlyphRequiredSpecEntry>         sGlyphRequiredSpecStore(GlyphRequiredSpecfmt);
DB2Storage <MailTemplateEntry>              sMailTemplateStore(MailTemplateEntryfmt);
DB2Storage <SpecializationSpellEntry>       sSpecializationSpellStore(SpecializationSpellsfmt);
DB2Storage <QuestPOIPointEntry>             sQuestPOIPointStore(QuestPOIPointfmt);
DB2Storage <WbAccessControlListEntry>       sWbAccessControlListStore(WbAccessControlListfmt);
DB2Storage <WbCertWhitelistEntry>           sWbCertWhitelistStore(WbCertWhitelistfmt);

//////////////////////////////////////////////////////////////////////////
/// Garrison DB2
//////////////////////////////////////////////////////////////////////////
DB2Storage <GarrSiteLevelEntry>             sGarrSiteLevelStore(GarrSiteLevelEntryfmt);
DB2Storage <GarrSiteLevelPlotInstEntry>     sGarrSiteLevelPlotInstStore(GarrSiteLevelPlotInstEntryfmt);
DB2Storage <GarrPlotInstanceEntry>          sGarrPlotInstanceStore(GarrPlotInstanceEntryfmt);
DB2Storage <GarrPlotEntry>                  sGarrPlotStore(GarrPlotEntryfmt);
DB2Storage <GarrPlotUICategoryEntry>        sGarrPlotUICategoryStore(GarrPlotUICategoryEntryfmt);
DB2Storage <GarrMissionEntry>               sGarrMissionStore(GarrMissionEntryfmt);
DB2Storage <GarrMissionRewardEntry>         sGarrMissionRewardStore(GarrMissionRewardEntryfmt);
DB2Storage <GarrMissionXEncouterEntry>      sGarrMissionXEncouterStore(GarrMissionXEncouterEntryfmt);
DB2Storage <GarrBuildingEntry>              sGarrBuildingStore(GarrBuildingEntryfmt);
DB2Storage <GarrPlotBuildingEntry>          sGarrPlotBuildingStore(GarrPlotBuildingEntryfmt);
DB2Storage <GarrFollowerEntry>              sGarrFollowerStore(GarrFollowerEntryfmt);
DB2Storage <GarrFollowerTypeEntry>          sGarrFollowerTypeStore(GarrFollowerTypefmt);
DB2Storage <GarrAbilityEntry>               sGarrAbilityStore(GarrAbilityEntryfmt);
DB2Storage <GarrAbilityEffectEntry>         sGarrAbilityEffectStore(GarrAbilityEffectEntryfmt);
DB2Storage <GarrFollowerXAbilityEntry>      sGarrFollowerXAbilityStore(GarrFollowerXAbilityEntryfmt);
DB2Storage <GarrBuildingPlotInstEntry>      sGarrBuildingPlotInstStore(GarrBuildingPlotInstEntryfmt);
DB2Storage <GarrMechanicTypeEntry>          sGarrMechanicTypeStore(GarrMechanicTypeEntryfmt);
DB2Storage <GarrMechanicEntry>              sGarrMechanicStore(GarrMechanicEntryfmt);
DB2Storage <GarrEncouterXMechanicEntry>     sGarrEncouterXMechanicStore(GarrEncouterXMechanicEntryfmt);
DB2Storage <GarrFollowerLevelXPEntry>       sGarrFollowerLevelXPStore(GarrFollowerLevelXPEntryfmt);
DB2Storage <GarrSpecializationEntry>        sGarrSpecializationStore(GarrSpecializationEntryfmt);
DB2Storage <CharShipmentEntry>              sCharShipmentStore(CharShipmentEntryfmt);
DB2Storage <CharShipmentContainerEntry>     sCharShipmentContainerStore(CharShipmentContainerEntryfmt);

//////////////////////////////////////////////////////////////////////////
/// Battle pet
//////////////////////////////////////////////////////////////////////////
DB2Storage<BattlePetAbilityEntry>           sBattlePetAbilityStore(BattlePetAbilityfmt);
DB2Storage<BattlePetAbilityEffectEntry>     sBattlePetAbilityEffectStore(BattlePetAbilityEffectfmt);
DB2Storage<BattlePetAbilityTurnEntry>       sBattlePetAbilityTurnStore(BattlePetAbilityTurnfmt);
DB2Storage<BattlePetAbilityStateEntry>      sBattlePetAbilityStateStore(BattlePetAbilityStatefmt);
DB2Storage<BattlePetStateEntry>             sBattlePetStateStore(BattlePetStatefmt);
DB2Storage<BattlePetEffectPropertiesEntry>  sBattlePetEffectPropertiesStore(BattlePetEffectPropertiesfmt);
DB2Storage<BattlePetBreedQualityEntry>      sBattlePetBreedQualityStore(BattlePetBreedQualityfmt);
DB2Storage<BattlePetBreedStateEntry>        sBattlePetBreedStateStore(BattlePetBreedStatefmt);
DB2Storage<BattlePetSpeciesEntry>           sBattlePetSpeciesStore(BattlePetSpeciesfmt);
DB2Storage<BattlePetSpeciesStateEntry>      sBattlePetSpeciesStateStore(BattlePetSpeciesStatefmt);
DB2Storage<BattlePetSpeciesXAbilityEntry>   sBattlePetSpeciesXAbilityStore(BattlePetSpeciesXAbilityfmt);

DB2Storage <AreaGroupEntry>               sAreaGroupStore(AreaGroupEntryfmt);
DB2Storage <AreaGroupMemberEntry>         sAreaGroupMemberStore(AreaGroupMemberEntryfmt);
DB2Storage <AuctionHouseEntry>            sAuctionHouseStore(AuctionHouseEntryfmt);
DB2Storage <BarberShopStyleEntry>         sBarberShopStyleStore(BarberShopStyleEntryfmt);
DB2Storage <CharStartOutfitEntry>         sCharStartOutfitStore(CharStartOutfitEntryfmt);
DB2Storage <ChrClassXPowerTypesEntry>     sChrClassXPowerTypesStore(ChrClassesXPowerTypesfmt);
DB2Storage <CinematicSequencesEntry>      sCinematicSequencesStore(CinematicSequencesEntryfmt);
DB2Storage <CreatureDisplayInfoEntry>     sCreatureDisplayInfoStore(CreatureDisplayInfofmt);
DB2Storage <CreatureTypeEntry>            sCreatureTypeStore(CreatureTypefmt);
DB2Storage <DestructibleModelDataEntry>   sDestructibleModelDataStore(DestructibleModelDatafmt);
DB2Storage <DurabilityQualityEntry>       sDurabilityQualityStore(DurabilityQualityfmt);
DB2Storage <GlyphSlotEntry>               sGlyphSlotStore(GlyphSlotfmt);
DB2Storage <GuildPerkSpellsEntry>         sGuildPerkSpellsStore(GuildPerkSpellsfmt);
DB2Storage <ImportPriceArmorEntry>        sImportPriceArmorStore(ImportPriceArmorfmt);
DB2Storage <ImportPriceQualityEntry>      sImportPriceQualityStore(ImportPriceQualityfmt);
DB2Storage <ImportPriceShieldEntry>       sImportPriceShieldStore(ImportPriceShieldfmt);
DB2Storage <ImportPriceWeaponEntry>       sImportPriceWeaponStore(ImportPriceWeaponfmt);
DB2Storage <ItemPriceBaseEntry>           sItemPriceBaseStore(ItemPriceBasefmt);
DB2Storage <ItemClassEntry>               sItemClassStore(ItemClassfmt);
DB2Storage <ItemDisenchantLootEntry>      sItemDisenchantLootStore(ItemDisenchantLootfmt);
DB2Storage <ItemLimitCategoryEntry>       sItemLimitCategoryStore(ItemLimitCategoryEntryfmt);
DB2Storage <ItemRandomPropertiesEntry>    sItemRandomPropertiesStore(ItemRandomPropertiesfmt);
DB2Storage <ItemRandomSuffixEntry>        sItemRandomSuffixStore(ItemRandomSuffixfmt);
DB2Storage <ItemSpecEntry>                sItemSpecStore(ItemSpecEntryfmt);
DB2Storage <ItemSpecOverrideEntry>        sItemSpecOverrideStore(ItemSpecOverrideEntryfmt);
DB2Storage <MountCapabilityEntry>         sMountCapabilityStore(MountCapabilityfmt);
DB2Storage <MountTypeEntry>               sMountTypeStore(MountTypefmt);
DB2Storage <MountTypeXCapabilityEntry>    sMountTypeXCapabilityStore(MountTypeXCapabilityfmt);
DB2Storage <NameGenEntry>                 sNameGenStore(NameGenfmt);
DB2Storage <QuestSortEntry>               sQuestSortStore(QuestSortEntryfmt);
DB2Storage <QuestV2Entry>                 sQuestV2Store(QuestV2fmt);
DB2Storage <QuestXPEntry>                 sQuestXPStore(QuestXPfmt);
DB2Storage <ResearchBranchEntry>          sResearchBranchStore(ResearchBranchfmt);
DB2Storage <ResearchSiteEntry>            sResearchSiteStore(ResearchSitefmt);
DB2Storage <ResearchProjectEntry>         sResearchProjectStore(ResearchProjectfmt);
DB2Storage <ScalingStatDistributionEntry> sScalingStatDistributionStore(ScalingStatDistributionfmt);
DB2Storage <ScenarioEntry>                sScenarioStore(ScenarioEntryfmt);
DB2Storage <SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore(SpellItemEnchantmentConditionfmt);
DB2Storage <SpellProcsPerMinuteEntry>     sSpellProcsPerMinuteStore(SpellProcsPerMinuteEntryfmt);
DB2Storage <SpellProcsPerMinuteModEntry>  sSpellProcsPerMinuteModStore(SpellProcsPerMinuteModfmt);
DB2Storage <SpellCastTimesEntry>          sSpellCastTimesStore(SpellCastTimefmt);
DB2Storage <SpellDurationEntry>           sSpellDurationStore(SpellDurationfmt);
DB2Storage <SpellRadiusEntry>             sSpellRadiusStore(SpellRadiusfmt);
DB2Storage <SpellRangeEntry>              sSpellRangeStore(SpellRangefmt);
DB2Storage <TotemCategoryEntry>           sTotemCategoryStore(TotemCategoryEntryfmt);
DB2Storage <TransportAnimationEntry>      sTransportAnimationStore(TransportAnimationfmt);
DB2Storage <TransportRotationEntry>       sTransportRotationStore(TransportRotationfmt);
DB2Storage <WorldMapOverlayEntry>         sWorldMapOverlayStore(WorldMapOverlayEntryfmt);

std::vector<std::vector<uint8> >          sPowersByClassStore;
NameGenVectorArraysMap                    sGenNameVectoArraysMap;

TaxiMask                                  sTaxiNodesMask;
TaxiMask                                  sOldContinentsNodesMask;
TaxiMask                                  sHordeTaxiNodesMask;
TaxiMask                                  sAllianceTaxiNodesMask;
TaxiPathSetBySource                       sTaxiPathSetBySource;
TaxiPathNodesByPath                       sTaxiPathNodesByPath;

std::set<ResearchSiteEntry const*>        sResearchSiteSet;
std::set<ResearchProjectEntry const*>     sResearchProjectSet;

SpellTotemMap       sSpellTotemMap;
MountCapabilitiesMap sMountCapabilitiesMap;
std::map<uint32, std::vector<uint32>> sItemEffectsByItemID;
std::map<uint32, std::vector<ItemBonusEntry const*>> sItemBonusesByID;
std::map<uint32, std::vector<ItemXBonusTreeEntry const*>> sItemBonusTreeByID;
std::map<uint32, std::vector<QuestPackageItemEntry const*>> sQuestPackageItemsByGroup;
std::unordered_map<uint32, std::vector<SpellProcsPerMinuteModEntry const*>> sSpellProcsPerMinuteMods;
std::map<uint32, uint32> g_PvPItemStoreLevels;

AreaGroupMemebersByID sAreaGroupMemebersByIDStore;

typedef std::list<std::string> StoreProblemList1;

uint32 DB2FilesCount = 0;

static bool LoadDB2_assert_print(uint32 fsize,uint32 rsize, const std::string& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

struct LocalDB2Data
{
    LocalDB2Data(LocaleConstant loc) : defaultLocale(loc), availableDb2Locales(0xFFFFFFFF) {}

    LocaleConstant defaultLocale;

    // bitmasks for index of fullLocaleNameList
    uint32 availableDb2Locales;
};

template<class T>
inline void LoadDB2(StoreProblemList1& errlist, DB2Storage<T>& storage, const std::string& db2_path, const std::string& filename, std::string customTableName = "", std::string customIndexName = "")
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DB2FilesCount;
    std::string db2_filename = db2_path + filename;
    std::string l_SQLFormat;
    SqlDb2 * sql = NULL;
    if (!customTableName.empty())
    {
        l_SQLFormat = std::string(strlen(storage.GetFormat()), FT_SQL_PRESENT);
        l_SQLFormat.append(1, FT_SQL_SUP);

        sql = new SqlDb2(customTableName, l_SQLFormat, customIndexName, storage.GetFormat());
    }

    if (!storage.Load(db2_filename.c_str(), sql, sWorld->GetDefaultDbcLocale()))
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE * f = fopen(db2_filename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100,"(exist, but have %u fields instead " SIZEFMTD ") Wrong client version DBC file?", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errlist.push_back(db2_filename + buf);
            fclose(f);
        }
        else
            errlist.push_back(db2_filename);
    }

    if (sDB2PerHash.find(storage.GetHash()) == sDB2PerHash.end())
        sDB2PerHash[storage.GetHash()] = &storage;
}

SpellTotemsEntry const* GetSpellTotemEntry(uint32 spellId, uint8 totem)
{
    SpellTotemMap::const_iterator itr = sSpellTotemMap.find(spellId);
    if (itr == sSpellTotemMap.end())
        return NULL;

    return itr->second.totems[totem];
}

uint32 GetItemDisplayID(uint32 p_ItemID, uint32 p_AppearanceModID)
{
    auto l_Iter = g_ItemDisplayIDs.find(p_ItemID | (p_AppearanceModID << 24));
    if (l_Iter != g_ItemDisplayIDs.end())
        return l_Iter->second;

    /// Fall back to unmodified appearance
    if (p_AppearanceModID)
    {
        l_Iter = g_ItemDisplayIDs.find(p_ItemID);
        if (l_Iter != g_ItemDisplayIDs.end())
            return l_Iter->second;
    }

    return 0;
}

void LoadDB2Stores(const std::string& dataPath)
{
    std::string db2Path = dataPath + "dbc/";

    StoreProblemList1 bad_db2_files;

    LoadDB2(bad_db2_files, sAchievementStore,            db2Path, "Achievement.db2");
    LoadDB2(bad_db2_files, sModifierTreeStore,           db2Path, "ModifierTree.db2");
    LoadDB2(bad_db2_files, sCriteriaStore,               db2Path, "Criteria.db2");
    LoadDB2(bad_db2_files, sCriteriaTreeStore,           db2Path, "CriteriaTree.db2");

    /// Ko'ragh Achievement - Pair Annihilation
    if (CriteriaEntry const* l_Criteria = sCriteriaStore.LookupEntry(24693))
        ((CriteriaEntry*)l_Criteria)->Type = AchievementCriteriaDataType::ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT;

    //////////////////////////////////////////////////////////////////////////
    /// Misc DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sSoundEntriesStore,              db2Path, "SoundEntries.db2"                                                     );
    LoadDB2(bad_db2_files, sCurrencyTypesStore,             db2Path, "CurrencyTypes.db2",               "currency_types",               "ID");
    LoadDB2(bad_db2_files, sPathNodeStore,                  db2Path, "PathNode.db2"                                                         );
    LoadDB2(bad_db2_files, sLocationStore,                  db2Path, "Location.db2"                                                         );
    LoadDB2(bad_db2_files, sAreaPOIStore,                   db2Path, "AreaPOI.db2"                                                          );
    LoadDB2(bad_db2_files, sCurvePointStore,                db2Path, "CurvePoint.db2",                  "curve_point",                  "ID");
    LoadDB2(bad_db2_files, sGroupFinderActivityStore,       db2Path, "GroupFinderActivity.db2"                                              );
    LoadDB2(bad_db2_files, sGroupFinderCategoryStore,       db2Path, "GroupFinderCategory.db2"                                              );
    LoadDB2(bad_db2_files, sHolidaysStore,                  db2Path, "Holidays.db2"                                                         );
    LoadDB2(bad_db2_files, sMapChallengeModeStore,          db2Path, "MapChallengeMode.db2",            "map_challenge_mode",           "ID");
    LoadDB2(bad_db2_files, sMountStore,                     db2Path, "Mount.db2",                       "mount",                        "ID");
    LoadDB2(bad_db2_files, sMountTypeStore,                 db2Path, "MountType.db2",                   "mount_type",                   "ID");
    LoadDB2(bad_db2_files, sMountCapabilityStore,           db2Path, "MountCapability.db2",             "mount_capability",             "ID");
    LoadDB2(bad_db2_files, sMountTypeXCapabilityStore,      db2Path, "MountTypeXCapability.db2",        "mount_type_x_capability",      "ID");
    LoadDB2(bad_db2_files, sPlayerConditionStore,           db2Path, "PlayerCondition.db2"                                                  );
    LoadDB2(bad_db2_files, sVignetteStore,                  db2Path, "Vignette.db2"                                                         );
    LoadDB2(bad_db2_files, sGlyphRequiredSpecStore,         db2Path, "GlyphRequiredSpec.db2"                                                );
    LoadDB2(bad_db2_files, sQuestPOIPointStore,             db2Path, "QuestPOIPoint.db2"                                                    );
    LoadDB2(bad_db2_files, sAreaGroupStore,                 db2Path, "AreaGroup.db2"                                                        );
    LoadDB2(bad_db2_files, sAreaGroupMemberStore,           db2Path, "AreaGroupMember.db2"                                                  );

    //////////////////////////////////////////////////////////////////////////
    /// Quest DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sQuestPackageItemStore,          db2Path, "QuestPackageItem.db2",            "quest_package_item",           "ID");
    LoadDB2(bad_db2_files, sQuestV2CliTaskStore,            db2Path, "QuestV2CliTask.db2"                                                   );
    LoadDB2(bad_db2_files, sQuestPOIPointCliTaskStore,      db2Path, "QuestPOIPointCliTask.db2"                                             );
  
    //////////////////////////////////////////////////////////////////////////
    /// Scene Script DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sSceneScriptStore,               db2Path, "SceneScript.db2"                                                      );
    LoadDB2(bad_db2_files, sSceneScriptPackageStore,        db2Path, "SceneScriptPackage.db2"                                               );

    //////////////////////////////////////////////////////////////////////////
    /// Taxi DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sTaxiNodesStore,                 db2Path, "TaxiNodes.db2"                                                        );
    LoadDB2(bad_db2_files, sTaxiPathStore,                  db2Path, "TaxiPath.db2"                                                         );
    LoadDB2(bad_db2_files, sTaxiPathNodeStore,              db2Path, "TaxiPathNode.db2"                                                     );

    //////////////////////////////////////////////////////////////////////////
    /// Item DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sItemStore,                      db2Path, "Item.db2",                        "item",                         "ID");
    LoadDB2(bad_db2_files, sItemCurrencyCostStore,          db2Path, "ItemCurrencyCost.db2",            "item_currency_cost",           "ID");
    LoadDB2(bad_db2_files, sItemSparseStore,                db2Path, "Item-sparse.db2",                 "item_sparse",                  "ID");
    LoadDB2(bad_db2_files, sItemEffectStore,                db2Path, "ItemEffect.db2",                  "item_effect",                  "ID");
    LoadDB2(bad_db2_files, sItemModifiedAppearanceStore,    db2Path, "ItemModifiedAppearance.db2",      "item_modified_appearance",     "ID");
    LoadDB2(bad_db2_files, sItemAppearanceStore,            db2Path, "ItemAppearance.db2",              "item_appearance",              "ID");
    LoadDB2(bad_db2_files, sItemExtendedCostStore,          db2Path, "ItemExtendedCost.db2",            "item_extended_cost",           "ID");
    LoadDB2(bad_db2_files, sHeirloomStore,                  db2Path, "Heirloom.db2"                                                         );
    LoadDB2(bad_db2_files, sPvpItemStore,                   db2Path, "PvpItem.db2",                     "pvp_item",                     "ID");

    for (uint32 l_I = 0; l_I < sPvpItemStore.GetNumRows(); ++l_I)
    {
        if (PvpItemEntry const* l_Entry = sPvpItemStore.LookupEntry(l_I))
            g_PvPItemStoreLevels[l_Entry->itemId] = l_Entry->ilvl;
    }

    LoadDB2(bad_db2_files, sItemUpgradeStore,               db2Path, "ItemUpgrade.db2"                                                      );
    LoadDB2(bad_db2_files, sRulesetItemUpgradeStore,        db2Path, "RulesetItemUpgrade.db2"                                               );

    //////////////////////////////////////////////////////////////////////////
    /// Item Bonus DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sItemBonusStore,                 db2Path, "ItemBonus.db2",                   "item_bonus",                   "ID");
    LoadDB2(bad_db2_files, sItemBonusTreeNodeStore,         db2Path, "ItemBonusTreeNode.db2",           "item_bonus_tree_node",         "ID");
    LoadDB2(bad_db2_files, sItemXBonusTreeStore,            db2Path, "ItemXBonusTree.db2",              "item_x_bonus_tree",            "ID");

    //////////////////////////////////////////////////////////////////////////
    /// Spell DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sSpellEffectGroupSizeStore,      db2Path, "SpellEffectGroupSize.db2",        "spell_effect_group_size",      "ID");
    LoadDB2(bad_db2_files, sSpellReagentsStore,             db2Path, "SpellReagents.db2"                                                    );
    LoadDB2(bad_db2_files, sSpellReagentsCurrencyStore,     db2Path, "SpellReagentsCurrency.db2"                                            );
    LoadDB2(bad_db2_files, sSpellRuneCostStore,             db2Path, "SpellRuneCost.db2"                                                    );
    LoadDB2(bad_db2_files, sSpellCastingRequirementsStore,  db2Path, "SpellCastingRequirements.db2",    "spell_casting_requirements",   "ID");
    LoadDB2(bad_db2_files, sSpellAuraRestrictionsStore,     db2Path, "SpellAuraRestrictions.db2",       "spell_aura_restrictions",      "ID");
    LoadDB2(bad_db2_files, sOverrideSpellDataStore,         db2Path, "OverrideSpellData.db2"                                                );
    LoadDB2(bad_db2_files, sSpellMiscStore,                 db2Path, "SpellMisc.db2",                   "spell_misc",                   "ID");
    LoadDB2(bad_db2_files, sSpellPowerStore,                db2Path, "SpellPower.db2"                                                       );
    LoadDB2(bad_db2_files, sSpellTotemsStore,               db2Path, "SpellTotems.db2"                                                      );
    LoadDB2(bad_db2_files, sSpellClassOptionsStore,         db2Path, "SpellClassOptions.db2"                                                );
    LoadDB2(bad_db2_files, sSpellXSpellVisualStore,         db2Path, "SpellXSpellVisual.db2"                                                );

    //////////////////////////////////////////////////////////////////////////
    /// Garrison DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sGarrSiteLevelStore,             db2Path, "GarrSiteLevel.db2"                                                    );
    LoadDB2(bad_db2_files, sGarrSiteLevelPlotInstStore,     db2Path, "GarrSiteLevelPlotInst.db2"                                            );
    LoadDB2(bad_db2_files, sGarrPlotInstanceStore,          db2Path, "GarrPlotInstance.db2"                                                 );
    LoadDB2(bad_db2_files, sGarrPlotStore,                  db2Path, "GarrPlot.db2"                                                         );
    LoadDB2(bad_db2_files, sGarrPlotUICategoryStore,        db2Path, "GarrPlotUICategory.db2"                                               );
    LoadDB2(bad_db2_files, sGarrMissionStore,               db2Path, "GarrMission.db2"                                                      );
    LoadDB2(bad_db2_files, sGarrMissionRewardStore,         db2Path, "GarrMissionReward.db2"                                                );
    LoadDB2(bad_db2_files, sGarrMissionXEncouterStore,      db2Path, "GarrMissionXEncounter.db2"                                            );
    LoadDB2(bad_db2_files, sGarrBuildingStore,              db2Path, "GarrBuilding.db2"                                                     );
    LoadDB2(bad_db2_files, sGarrPlotBuildingStore,          db2Path, "GarrPlotBuilding.db2"                                                 );
    LoadDB2(bad_db2_files, sGarrFollowerStore,              db2Path, "GarrFollower.db2"                                                     );
    LoadDB2(bad_db2_files, sGarrFollowerTypeStore,          db2Path, "GarrFollowerType.db2"                                                 );
    LoadDB2(bad_db2_files, sGarrAbilityStore,               db2Path, "GarrAbility.db2",                  "garr_ability",                "ID");
    LoadDB2(bad_db2_files, sGarrAbilityEffectStore,         db2Path, "GarrAbilityEffect.db2"                                                );
    LoadDB2(bad_db2_files, sGarrFollowerXAbilityStore,      db2Path, "GarrFollowerXAbility.db2"                                             );
    LoadDB2(bad_db2_files, sGarrBuildingPlotInstStore,      db2Path, "GarrBuildingPlotInst.db2"                                             );
    LoadDB2(bad_db2_files, sGarrMechanicTypeStore,          db2Path, "GarrMechanicType.db2"                                                 );
    LoadDB2(bad_db2_files, sGarrMechanicStore,              db2Path, "GarrMechanic.db2"                                                     );
    LoadDB2(bad_db2_files, sGarrEncouterXMechanicStore,     db2Path, "GarrEncounterXMechanic.db2"                                           );
    LoadDB2(bad_db2_files, sGarrFollowerLevelXPStore,       db2Path, "GarrFollowerLevelXP.db2"                                              );
    LoadDB2(bad_db2_files, sGarrSpecializationStore,        db2Path, "GarrSpecialization.db2"                                               );
    LoadDB2(bad_db2_files, sCharShipmentStore,              db2Path, "CharShipment.db2"                                                     );
    LoadDB2(bad_db2_files, sCharShipmentContainerStore,     db2Path, "CharShipmentContainer.db2"                                            );

    //////////////////////////////////////////////////////////////////////////
    /// Battle pet DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sBattlePetAbilityStore,          db2Path, "BattlePetAbility.db2"                                                 );
    LoadDB2(bad_db2_files, sBattlePetAbilityEffectStore,    db2Path, "BattlePetAbilityEffect.db2"                                           );
    LoadDB2(bad_db2_files, sBattlePetAbilityTurnStore,      db2Path, "BattlePetAbilityTurn.db2"                                             );
    LoadDB2(bad_db2_files, sBattlePetAbilityStateStore,     db2Path, "BattlePetAbilityState.db2"                                            );
    LoadDB2(bad_db2_files, sBattlePetStateStore,            db2Path, "BattlePetState.db2"                                                   );
    LoadDB2(bad_db2_files, sBattlePetEffectPropertiesStore, db2Path, "BattlePetEffectProperties.db2"                                        );
    LoadDB2(bad_db2_files, sBattlePetBreedQualityStore,     db2Path, "BattlePetBreedQuality.db2"                                            );
    LoadDB2(bad_db2_files, sBattlePetBreedStateStore,       db2Path, "BattlePetBreedState.db2"                                              );
    LoadDB2(bad_db2_files, sBattlePetSpeciesStore,          db2Path, "BattlePetSpecies.db2",            "battle_pet_species",           "ID");
    LoadDB2(bad_db2_files, sBattlePetSpeciesStateStore,     db2Path, "BattlePetSpeciesState.db2"                                            );
    LoadDB2(bad_db2_files, sBattlePetSpeciesXAbilityStore,  db2Path, "BattlePetSpeciesXAbility.db2"                                         );

    LoadDB2(bad_db2_files,  sAuctionHouseStore,           db2Path, "AuctionHouse.db2");                                                 // 17399
    LoadDB2(bad_db2_files,  sBarberShopStyleStore,        db2Path, "BarberShopStyle.db2");                                              // 17399
    LoadDB2(bad_db2_files,  sCharStartOutfitStore,        db2Path, "CharStartOutfit.db2");                                              // 17399
    LoadDB2(bad_db2_files,  sChrClassXPowerTypesStore,    db2Path, "ChrClassesXPowerTypes.db2");                                        // 17399
    LoadDB2(bad_db2_files,  sCinematicSequencesStore,     db2Path, "CinematicSequences.db2");                                           // 17399
    LoadDB2(bad_db2_files,  sCreatureDisplayInfoStore,    db2Path, "CreatureDisplayInfo.db2");                                          // 17399
    LoadDB2(bad_db2_files,  sCreatureTypeStore,           db2Path, "CreatureType.db2");                                                 // 17399
    LoadDB2(bad_db2_files,  sDestructibleModelDataStore,  db2Path, "DestructibleModelData.db2");                                        // 17399
    LoadDB2(bad_db2_files,  sDurabilityQualityStore,      db2Path, "DurabilityQuality.db2");                                            // 17399
    LoadDB2(bad_db2_files,  sGlyphSlotStore,              db2Path, "GlyphSlot.db2");                                                    // 19027
    LoadDB2(bad_db2_files,  sGuildPerkSpellsStore,        db2Path, "GuildPerkSpells.db2");                                              // 17399
    LoadDB2(bad_db2_files,  sImportPriceArmorStore,       db2Path, "ImportPriceArmor.db2");                                             // 17399
    LoadDB2(bad_db2_files,  sImportPriceQualityStore,     db2Path, "ImportPriceQuality.db2");                                           // 17399
    LoadDB2(bad_db2_files,  sImportPriceShieldStore,      db2Path, "ImportPriceShield.db2");                                            // 17399
    LoadDB2(bad_db2_files,  sImportPriceWeaponStore,      db2Path, "ImportPriceWeapon.db2");                                            // 17399
    LoadDB2(bad_db2_files,  sItemPriceBaseStore,          db2Path, "ItemPriceBase.db2");                                                // 17399
    LoadDB2(bad_db2_files,  sItemClassStore,              db2Path, "ItemClass.db2");                                                    // 17399
    LoadDB2(bad_db2_files,  sItemLimitCategoryStore,      db2Path, "ItemLimitCategory.db2");                                            // 17399
    LoadDB2(bad_db2_files,  sItemRandomPropertiesStore,   db2Path, "ItemRandomProperties.db2");                                         // 17399
    LoadDB2(bad_db2_files,  sItemRandomSuffixStore,       db2Path, "ItemRandomSuffix.db2");                                             // 17399
    LoadDB2(bad_db2_files,  sItemSpecOverrideStore,       db2Path, "ItemSpecOverride.db2", "item_spec_override","ID");                  // 17399
    LoadDB2(bad_db2_files,  sItemSpecStore,               db2Path, "ItemSpec.db2");                                                     // 19116
    LoadDB2(bad_db2_files,  sItemDisenchantLootStore,     db2Path, "ItemDisenchantLoot.db2");                                           // 17399
    LoadDB2(bad_db2_files,  sNameGenStore,                db2Path, "NameGen.db2");                                                      // 17399
    LoadDB2(bad_db2_files,  sQuestV2Store,                db2Path, "QuestV2.db2");                                                      // 19342
    LoadDB2(bad_db2_files,  sQuestXPStore,                db2Path, "QuestXP.db2");                                                      // 17399
    LoadDB2(bad_db2_files,  sQuestSortStore,              db2Path, "QuestSort.db2");                                                    // 17399
    LoadDB2(bad_db2_files,  sResearchBranchStore,         db2Path, "ResearchBranch.db2");                                               // 17399
    LoadDB2(bad_db2_files,  sResearchProjectStore,        db2Path, "ResearchProject.db2");                                              // 17399
    LoadDB2(bad_db2_files,  sResearchSiteStore,           db2Path, "ResearchSite.db2");
    LoadDB2(bad_db2_files,  sScalingStatDistributionStore,db2Path, "ScalingStatDistribution.db2");                                      // 17399
    LoadDB2(bad_db2_files,  sScenarioStore,               db2Path, "Scenario.db2");                                                     // 19027
    LoadDB2(bad_db2_files,  sSpellProcsPerMinuteStore,    db2Path,"SpellProcsPerMinute.db2", "spell_procs_per_minute", "ID");
    LoadDB2(bad_db2_files,  sSpellProcsPerMinuteModStore, db2Path,"SpellProcsPerMinuteMod.db2", "spell_procs_per_minute_mod", "ID");
    LoadDB2(bad_db2_files,  sSpellCastTimesStore,         db2Path, "SpellCastTimes.db2");                                               // 17399
    LoadDB2(bad_db2_files,  sSpellDurationStore,          db2Path, "SpellDuration.db2");                                                // 17399
    LoadDB2(bad_db2_files,  sSpellItemEnchantmentConditionStore, db2Path, "SpellItemEnchantmentCondition.db2");                         // 17399
    LoadDB2(bad_db2_files,  sSpellRadiusStore,            db2Path, "SpellRadius.db2");                                                  // 17399
    LoadDB2(bad_db2_files,  sSpellRangeStore,             db2Path, "SpellRange.db2");                                                   // 17399
    LoadDB2(bad_db2_files,  sTotemCategoryStore,          db2Path, "TotemCategory.db2");                                                // 17399
    LoadDB2(bad_db2_files,  sTransportAnimationStore,     db2Path, "TransportAnimation.db2");
    LoadDB2(bad_db2_files,  sTransportRotationStore,      db2Path, "TransportRotation.db2");
    LoadDB2(bad_db2_files,  sWorldMapOverlayStore,        db2Path, "WorldMapOverlay.db2");                                              // 17399
    LoadDB2(bad_db2_files,  sMailTemplateStore,           db2Path, "MailTemplate.db2");                                                 // 17399
    LoadDB2(bad_db2_files,  sSpecializationSpellStore,    db2Path, "SpecializationSpells.db2");                                         // 17399

    sPowersByClassStore.resize(MAX_CLASSES);

    for (uint32 l_I = 0; l_I <= sChrClassXPowerTypesStore.GetNumRows(); ++l_I)
    {
        ChrClassXPowerTypesEntry const* l_PowerEntry = sChrClassXPowerTypesStore.LookupEntry(l_I);
        if (!l_PowerEntry)
            continue;

        sPowersByClassStore[l_PowerEntry->classId].push_back(l_PowerEntry->power);
    }

    for (int l_Class = 0; l_Class < MAX_CLASSES; ++l_Class)
        std::sort(sPowersByClassStore[l_Class].begin(), sPowersByClassStore[l_Class].end());

    for (uint32 l_ID = 0; l_ID < sMountTypeStore.GetNumRows(); ++l_ID)
    {
        MountTypeEntry const* l_Entry = sMountTypeStore.LookupEntry(l_ID);
        if (!l_Entry)
            continue;

        MountCapabilities l_Capabilities;
        memset(&l_Capabilities, 0, sizeof(l_Capabilities));

        sMountCapabilitiesMap.insert({l_ID, l_Capabilities});
    }

    for (uint32 l_ID = 0; l_ID < sMountTypeXCapabilityStore.GetNumRows(); ++l_ID)
    {
        MountTypeXCapabilityEntry const* l_Entry = sMountTypeXCapabilityStore.LookupEntry(l_ID);
        if (!l_Entry)
            continue;

        assert("MAX_MOUNT_CAPABILITIES too small, needs increase" && l_Entry->Index < MAX_MOUNT_CAPABILITIES);
        sMountCapabilitiesMap[l_Entry->MountTypeID].Capabilities[l_Entry->Index] = l_Entry->CapabilityID;
    }

    for (uint32 i = 0; i < sSpellProcsPerMinuteModStore.GetNumRows(); ++i)
    {
        if (SpellProcsPerMinuteModEntry const* ppmMod = sSpellProcsPerMinuteModStore.LookupEntry(i))
            sSpellProcsPerMinuteMods[ppmMod->SpellProcsPerMinuteID].push_back(ppmMod);
    }

    //////////////////////////////////////////////////////////////////////////
    /// WebBrowser DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sWbAccessControlListStore,       db2Path, "WbAccessControlList.db2",          "wb_access_control_list",      "ID");
    LoadDB2(bad_db2_files, sWbCertWhitelistStore,           db2Path, "WbCertWhitelist.db2",              "wb_cert_whitelist",           "ID");

    std::set<uint32> scalingCurves;
    for (uint32 i = 0; i < sScalingStatDistributionStore.GetNumRows(); ++i)
        if (ScalingStatDistributionEntry const* ssd = sScalingStatDistributionStore.LookupEntry(i))
            scalingCurves.insert(ssd->CurveProperties);

    for (uint32 i = 0; i < sCurvePointStore.GetNumRows(); ++i)
        if (CurvePointEntry const* curvePoint = sCurvePointStore.LookupEntry(i))
            if (scalingCurves.count(curvePoint->CurveID))
                HeirloomCurvePoints[curvePoint->CurveID][curvePoint->Index] = curvePoint;

    for (uint32 l_Y = 0; l_Y < sItemModifiedAppearanceStore.GetNumRows(); l_Y++)
    {
        if (ItemModifiedAppearanceEntry const* l_ModifiedAppearanceEntry = sItemModifiedAppearanceStore.LookupEntry(l_Y))
        {
            if (ItemAppearanceEntry const* l_Appearance = sItemAppearanceStore.LookupEntry(l_ModifiedAppearanceEntry->AppearanceID))
                g_ItemDisplayIDs[l_ModifiedAppearanceEntry->ItemID | (l_ModifiedAppearanceEntry->AppearanceModID << 24)] = l_Appearance->DisplayID;

            if (l_ModifiedAppearanceEntry->ItemID != 0 && l_ModifiedAppearanceEntry->Index == 0)
            {
                ItemAppearanceEntry const* l_AppearanceEntry = sItemAppearanceStore.LookupEntry(l_ModifiedAppearanceEntry->AppearanceID);

                uint32 l_DisplayID = 0;

                if (l_AppearanceEntry)
                {
                    l_DisplayID = l_AppearanceEntry->DisplayID;
                    g_ItemFileDataId[l_ModifiedAppearanceEntry->ItemID] = l_AppearanceEntry->IconFileDataID;
                }

                if (ItemEntry* l_Entry = const_cast<ItemEntry*>(sItemStore.LookupEntry(l_ModifiedAppearanceEntry->ItemID)))
                    g_ItemTemplateDisplayIDs[l_ModifiedAppearanceEntry->ItemID] = l_DisplayID;
            }
        }
    }

    for (uint32 l_I = 1; l_I < sItemEffectStore.GetNumRows(); ++l_I)
    {
        if (ItemEffectEntry const* l_Entry = sItemEffectStore.LookupEntry(l_I))
            sItemEffectsByItemID[l_Entry->ItemID].push_back(l_I);
    }

    for (uint32 l_ID = 0; l_ID < sAreaGroupMemberStore.GetNumRows(); l_ID++)
        if (AreaGroupMemberEntry const* l_Entry = sAreaGroupMemberStore.LookupEntry(l_ID))
            sAreaGroupMemebersByIDStore[l_Entry->AreaGroupID].push_back(l_Entry->AreaID);

    for (uint32 l_I = 0; l_I < sItemBonusStore.GetNumRows(); l_I++)
    {
        ItemBonusEntry const* l_Entry = sItemBonusStore.LookupEntry(l_I);
        if (!l_Entry)
            continue;

        if (!sItemBonusesByID[l_Entry->Entry].size())
        {
            sItemBonusesByID[l_Entry->Entry].resize(MAX_ITEM_BONUS);
            for (int l_J = 0; l_J < MAX_ITEM_BONUS; l_J++)
                sItemBonusesByID[l_Entry->Entry][l_J] = nullptr;
        }

        sItemBonusesByID[l_Entry->Entry][l_Entry->Index] = l_Entry;
    }

    /// Load quest package items
    for (uint32 l_I = 0; l_I < sQuestPackageItemStore.GetNumRows(); l_I++)
    {
        QuestPackageItemEntry const* l_QuestPackageItem = sQuestPackageItemStore.LookupEntry(l_I);
        if (l_QuestPackageItem == nullptr)
            continue;

        sQuestPackageItemsByGroup[l_QuestPackageItem->PackageID].push_back(l_QuestPackageItem);
    }

    /// Load Item Bonus Tree
    for (uint32 l_I = 0; l_I < sItemXBonusTreeStore.GetNumRows(); l_I++)
    {
        auto l_ItemXBonusTree = sItemXBonusTreeStore.LookupEntry(l_I);
        if (l_ItemXBonusTree == nullptr)
            continue;

        sItemBonusTreeByID[l_ItemXBonusTree->ItemId].push_back(l_ItemXBonusTree);
    }

    for (uint32 l_I = 0; l_I < sTaxiPathStore.GetNumRows(); l_I++)
    {
        TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(l_I);

        if (!entry)
            continue;

        sTaxiPathSetBySource[entry->From][entry->To] = TaxiPathBySourceAndDestination(entry->ID, entry->Cost);
    }

    uint32 pathCount = sTaxiPathStore.GetNumRows();

    /// Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount);                           ///< 0 and some other indexes not used

    for (uint32 l_I = 0; l_I < sTaxiPathNodeStore.GetNumRows(); l_I++)
    {
        TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(l_I);

        if (!entry)
            continue;

        if (pathLength[entry->PathID] < entry->NodeIndex + 1)
            pathLength[entry->PathID] = entry->NodeIndex + 1;
    }

    /// Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 ///< 0 and some other indexes not used
    for (uint32 i = 0; i < sTaxiPathNodesByPath.size(); ++i)
        sTaxiPathNodesByPath[i].resize(pathLength[i]);

    /// fill data
    for (uint32 l_I = 0; l_I < sTaxiPathNodeStore.GetNumRows(); l_I++)
    {
        TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(l_I);

        if (!entry)
            continue;

        sTaxiPathNodesByPath[entry->PathID][entry->NodeIndex] = entry;
    }

    /// Initialize global taxinodes mask
    /// include existed nodes that have at least single not spell base (scripted) path
    {
        if (sTaxiNodesStore.GetNumRows())
        {
            WPError(TaxiMaskSize >= ((sTaxiNodesStore.GetNumRows() - 1) / 8) + 1,
                "TaxiMaskSize is not large enough to contain all taxi nodes!"   );
        }

        sTaxiNodesMask.fill(0);
        sOldContinentsNodesMask.fill(0);
        sHordeTaxiNodesMask.fill(0);
        sAllianceTaxiNodesMask.fill(0);
        for (uint32 l_I = 0; l_I < sTaxiNodesStore.GetNumRows(); l_I++)
        {
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(l_I);

            if (!node)
                continue;

            if (!(node->Flags & (TAXI_NODE_FLAG_ALLIANCE | TAXI_NODE_FLAG_HORDE)))
                continue;

            /// Valid taxi network node
            uint8  field = (uint8)((node->ID - 1) / 8);
            uint32 submask = 1 << ((node->ID - 1) % 8);

            sTaxiNodesMask[field] |= submask;
            if (node->Flags & TAXI_NODE_FLAG_HORDE)
                sHordeTaxiNodesMask[field] |= submask;
            if (node->Flags & TAXI_NODE_FLAG_ALLIANCE)
                sAllianceTaxiNodesMask[field] |= submask;

            uint32 nodeMap;
            DeterminaAlternateMapPosition(node->MapID, node->x, node->y, node->z, &nodeMap);
            if (nodeMap < 2)
                sOldContinentsNodesMask[field] |= submask;
        }
    }

    for (uint32 l_ID = 0; l_ID < sHeirloomStore.GetNumRows(); ++l_ID)
    {
        HeirloomEntry const* l_Heirloom = sHeirloomStore.LookupEntry(l_ID);

        if (!l_Heirloom)
            continue;

        HeirloomEntryByItemID.insert({ l_Heirloom->ItemID, l_Heirloom });

        for (uint32 l_X = 0; l_X < 2; l_X++)
            if (uint32 l_OlderItemID = l_Heirloom->OldHeirloomID[l_X])
                HeirloomEntryByItemID.insert({ l_OlderItemID, l_Heirloom});

        if (uint32 l_HeroicID = l_Heirloom->HeroicVersion)
            HeirloomEntryByItemID.insert({ l_HeroicID, l_Heirloom});
    }
        
    for (uint32 i = 0; i < sNameGenStore.GetNumRows(); ++i)
        if (NameGenEntry const* entry = sNameGenStore.LookupEntry(i))
            sGenNameVectoArraysMap[entry->race].stringVectorArray[entry->gender].push_back(entry->name->Get(LocaleConstant::LOCALE_enUS));
    sNameGenStore.Clear();

    for (uint32 i =0; i < sResearchProjectStore.GetNumRows(); ++i)
    {
        ResearchProjectEntry const* rp = sResearchProjectStore.LookupEntry(i);
        if (!rp)
            continue;

        // Branch 0 is for Blizzard tests
        if (rp->branchId == 0)
            continue;

        sResearchProjectSet.insert(rp);
    }
    //sResearchProjectStore.Clear();

    for (uint32 i =0; i < sResearchSiteStore.GetNumRows(); ++i)
    {
        ResearchSiteEntry const* rs = sResearchSiteStore.LookupEntry(i);
        if (!rs)
            continue;
        if (rs->ID == 140           // template
            || rs->ID == 142        // template
            || rs->ID == 161        // template
            || rs->ID == 471        // vashj'ir
            || rs->ID == 473        // vashj'ir
            || rs->ID == 475)       // vashj'ir
            continue;
        sResearchSiteSet.insert(rs);
    }
    //sResearchSiteStore.Clear();

    /// Fill map to dissociate abilities class-specific
    for (uint32 l_Itr = 0; l_Itr < sGarrAbilityStore.GetNumRows(); ++l_Itr)
    {
        GarrAbilityEntry const* l_GarrAbility = sGarrAbilityStore.LookupEntry(l_Itr);

        if (!l_GarrAbility)
            continue;

        char const* l_Description = l_GarrAbility->Description->Get(LocaleConstant::LOCALE_enUS);

        Classes l_Class = Classes::CLASS_PALADIN;

        if (std::strstr(l_Description, "paladin"))
            l_Class = Classes::CLASS_PALADIN;
        else if (std::strstr(l_Description, "mage"))
            l_Class = Classes::CLASS_MAGE;
        else if (std::strstr(l_Description, "warrior"))
            l_Class = Classes::CLASS_WARRIOR;
        else if (std::strstr(l_Description, "monk"))
            l_Class = Classes::CLASS_MONK;
        else if (std::strstr(l_Description, "mage"))
            l_Class = Classes::CLASS_MAGE;
        else if (std::strstr(l_Description, "priest"))
            l_Class = Classes::CLASS_PRIEST;
        else if (std::strstr(l_Description, "warlock"))
            l_Class = Classes::CLASS_WARLOCK;
        else if (std::strstr(l_Description, "rogue"))
            l_Class = Classes::CLASS_ROGUE;
        else if (std::strstr(l_Description, "hunter"))
            l_Class = Classes::CLASS_HUNTER;
        else if (std::strstr(l_Description, "druid"))
            l_Class = Classes::CLASS_DRUID;
        else if (std::strstr(l_Description, "shaman"))
            l_Class = Classes::CLASS_SHAMAN;
        else if (std::strstr(l_Description, "death knight"))
            l_Class = Classes::CLASS_DEATH_KNIGHT;

        g_FollowerAbilitiesClass.insert(std::make_pair(l_GarrAbility->ID, l_Class));
    }

    /// error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdb2", DB2FilesCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount,str.c_str());
        exit(1);
    }

    /// Check loaded DB2 files proper version
    if (!sItemStore.LookupEntry(128706) ||              ///< Last item added in 6.2.0 (20216)
        !sItemExtendedCostStore.LookupEntry(5923) )     ///< Last item extended cost added in 6.2.0 (20216)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Please extract correct db2 files from client 6.2.0 (20216)");
        exit(1);
    }
    sLog->outInfo(LOG_FILTER_GENERAL, ">> Initialized %d DB2 data stores.", DB2FilesCount);
}

std::vector<ItemBonusEntry const*> const* GetItemBonusesByID(uint32 Id)
{
    std::map<uint32, std::vector<ItemBonusEntry const*>>::const_iterator iter = sItemBonusesByID.find(Id);
    return iter != sItemBonusesByID.end() ? &(iter->second) : nullptr;
}

uint32 GetHeirloomItemLevel(uint32 curveId, uint32 level)
{
    /// Assuming linear item level scaling for heirlooms
    auto itr = HeirloomCurvePoints.find(curveId);
    if (itr == HeirloomCurvePoints.end())
        return 0;

    auto it2 = itr->second.begin(); ///< Highest scaling point
    if (level >= it2->second->X)
        return it2->second->Y;

    auto previousItr = it2++;
    for (; it2 != itr->second.end(); ++it2, ++previousItr)
        if (level >= it2->second->X)
            return uint32((previousItr->second->Y - it2->second->Y) / (previousItr->second->X - it2->second->X) * (float(level) - it2->second->X) + it2->second->Y);

    return uint32(previousItr->second->Y);  ///< Lowest scaling point
}

HeirloomEntry const* GetHeirloomEntryByItemID(uint32 p_ItemID)
{
    std::unordered_map<uint32, HeirloomEntry const*>::const_iterator l_Iter =  HeirloomEntryByItemID.find(p_ItemID);
    return l_Iter != HeirloomEntryByItemID.end() ? l_Iter->second : nullptr;
};

const std::string* GetRandomCharacterName(uint8 race, uint8 gender)
{
    uint32 size = sGenNameVectoArraysMap[race].stringVectorArray[gender].size();
    uint32 randPos = urand(0,size-1);

    return &sGenNameVectoArraysMap[race].stringVectorArray[gender][randPos];
}

std::vector<uint32> GetAreasForGroup(uint32 areaGroupId)
{
    auto itr = sAreaGroupMemebersByIDStore.find(areaGroupId);
       if (itr != sAreaGroupMemebersByIDStore.end())
           return itr->second;
    
    return std::vector<uint32>();
}

uint8 GetPowerIndexByClass(uint8 p_Class, uint8 p_Power)
{
    if (p_Class >= MAX_CLASSES)
        return Powers::MAX_POWERS;

    auto const& l_List = sPowersByClassStore[p_Class];
    for (uint8 l_I = 0; l_I < l_List.size(); ++l_I)
    {
        if (l_List[l_I] == p_Power)
            return l_I;
    }

    return Powers::MAX_POWERS;
}

std::vector<SpellProcsPerMinuteModEntry const*> GetSpellProcsPerMinuteMods(uint32 spellprocsPerMinuteId)
{
    auto itr = sSpellProcsPerMinuteMods.find(spellprocsPerMinuteId);
    if (itr != sSpellProcsPerMinuteMods.end())
        return itr->second;

    return std::vector<SpellProcsPerMinuteModEntry const*>();
}
