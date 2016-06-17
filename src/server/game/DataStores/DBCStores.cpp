////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "DBCStores.h"
#include "DB2Stores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "DBCfmt.h"
#include "ItemPrototype.h"
#include "TransportMgr.h"
#include "Battleground.h"
#include "Player.h"

#include <iostream>
#include <fstream>
#include "WowTime.hpp"
#include <ace/OS_NS_time.h>

typedef std::map<uint16, uint32> AreaFlagByAreaID;
typedef std::map<uint32, uint32> AreaFlagByMapID;

struct WMOAreaTableTripple
{
    WMOAreaTableTripple(int32 r, int32 a, int32 g) :  groupId(g), rootId(r), adtId(a)
    {

    }

    bool operator <(const WMOAreaTableTripple& b) const
    {
        return memcmp(this, &b, sizeof(WMOAreaTableTripple))<0;
    }

    // Ordered by entropy; that way memcmp will have a minimal medium runtime
    int32 groupId;
    int32 rootId;
    int32 adtId;
};

typedef std::map<WMOAreaTableTripple, WMOAreaTableEntry const*> WMOAreaInfoByTripple;
ItemSetSpellsByItemID sItemSetSpellsByItemIDStore;

DBCStorage <AreaTableEntry>               sAreaStore(AreaTableEntryfmt);
static AreaFlagByAreaID                   sAreaFlagByAreaID;
static AreaFlagByMapID                    sAreaFlagByMapID;                    // For instances without generated *.map files

static WMOAreaInfoByTripple               sWMOAreaInfoByTripple;

DBCStorage <AnimKitEntry>                 sAnimKitStore(AnimKitfmt);
DBCStorage <AreaTriggerEntry>             sAreaTriggerStore(AreaTriggerEntryfmt);
DBCStorage <ArmorLocationEntry>           sArmorLocationStore(ArmorLocationfmt);
DBCStorage <BankBagSlotPricesEntry>       sBankBagSlotPricesStore(BankBagSlotPricesEntryfmt);
DBCStorage <BattlemasterListEntry>        sBattlemasterListStore(BattlemasterListEntryfmt);
DBCStorage <CharTitlesEntry>              sCharTitlesStore(CharTitlesEntryfmt);
DBCStorage <ChatChannelsEntry>            sChatChannelsStore(ChatChannelsEntryfmt);
DBCStorage <ChrClassesEntry>              sChrClassesStore(ChrClassesEntryfmt);
DBCStorage <ChrRacesEntry>                sChrRacesStore(ChrRacesEntryfmt);
DBCStorage <ChrSpecializationsEntry>      sChrSpecializationsStore(ChrSpecializationsfmt);

DBCStorage <CinematicCameraEntry>         sCinematicCameraStore(CinematicCameraEntryfmt);
DBCStorage <CreatureDisplayInfoExtraEntry> sCreatureDisplayInfoExtraStore(CreatureDisplayInfoExtrafmt);
DBCStorage <CreatureFamilyEntry>          sCreatureFamilyStore(CreatureFamilyfmt);
DBCStorage <CreatureModelDataEntry>       sCreatureModelDataStore(CreatureModelDatafmt);

DBCStorage <DifficultyEntry>              sDifficultyStore(DifficultyEntryfmt);

DBCStorage <DungeonEncounterEntry>        sDungeonEncounterStore(DungeonEncounterfmt);
DBCStorage <DurabilityCostsEntry>         sDurabilityCostsStore(DurabilityCostsfmt);

DBCStorage <EmotesEntry>                  sEmotesStore(EmotesEntryfmt);
DBCStorage <EmotesTextEntry>              sEmotesTextStore(EmotesTextEntryfmt);

typedef std::tuple<uint32, uint32, uint32> EmotesTextSoundKey;
static std::map<EmotesTextSoundKey, EmotesTextSoundEntry const*> sEmotesTextSoundMap;
DBCStorage <EmotesTextSoundEntry>         sEmotesTextSoundStore(EmotesTextSoundEntryfmt);

typedef std::map<uint32, SimpleFactionsList> FactionTeamMap;
static FactionTeamMap                     sFactionTeamMap;
DBCStorage <FactionEntry>                 sFactionStore(FactionEntryfmt);
DBCStorage <FactionTemplateEntry>         sFactionTemplateStore(FactionTemplateEntryfmt);

DBCStorage <FileDataEntry>                sFileDataStore(FileDatafmt);

DBCStorage <GameObjectDisplayInfoEntry>   sGameObjectDisplayInfoStore(GameObjectDisplayInfofmt);
DBCStorage <GemPropertiesEntry>           sGemPropertiesStore(GemPropertiesEntryfmt);
DBCStorage <GlyphPropertiesEntry>         sGlyphPropertiesStore(GlyphPropertiesfmt);

DBCStorage <gtArmorMitigationByLvlEntry>  sgtArmorMitigationByLvlStore(gtArmorMitigationByLvlfmt);
DBCStorage <GtBarberShopCostBaseEntry>    sGtBarberShopCostBaseStore(GtBarberShopCostBasefmt);
DBCStorage <GtCombatRatingsEntry>         sGtCombatRatingsStore(GtCombatRatingsfmt);
DBCStorage <GtChanceToMeleeCritBaseEntry> sGtChanceToMeleeCritBaseStore(GtChanceToMeleeCritBasefmt);
DBCStorage <GtChanceToMeleeCritEntry>     sGtChanceToMeleeCritStore(GtChanceToMeleeCritfmt);
DBCStorage <GtChanceToSpellCritBaseEntry> sGtChanceToSpellCritBaseStore(GtChanceToSpellCritBasefmt);
DBCStorage <GtChanceToSpellCritEntry>     sGtChanceToSpellCritStore(GtChanceToSpellCritfmt);
DBCStorage <GtOCTLevelExperienceEntry>    sGtOCTLevelExperienceStore(GtOCTLevelExperiencefmt);
DBCStorage <GtOCTHpPerStaminaEntry>       sGtOCTHpPerStaminaStore(GtOCTHpPerStaminafmt);
DBCStorage <GtRegenMPPerSptEntry>         sGtRegenMPPerSptStore(GtRegenMPPerSptfmt);
DBCStorage <GtSpellScalingEntry>          sGtSpellScalingStore(GtSpellScalingfmt);
DBCStorage <GtOCTBaseHPByClassEntry>      sGtOCTBaseHPByClassStore(GtOCTBaseHPByClassfmt);
DBCStorage <GtOCTBaseMPByClassEntry>      sGtOCTBaseMPByClassStore(GtOCTBaseMPByClassfmt);


DBCStorage <ItemSetSpellEntry>            sItemSetSpellStore(ItemSetSpellFmt);
DBCStorage <ItemArmorQualityEntry>        sItemArmorQualityStore(ItemArmorQualityfmt);
DBCStorage <ItemArmorShieldEntry>         sItemArmorShieldStore(ItemArmorShieldfmt);
DBCStorage <ItemArmorTotalEntry>          sItemArmorTotalStore(ItemArmorTotalfmt);
DBCStorage <ItemBagFamilyEntry>           sItemBagFamilyStore(ItemBagFamilyfmt);
DBCStorage <ItemDamageEntry>              sItemDamageAmmoStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageOneHandStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageOneHandCasterStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageRangedStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageThrownStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageTwoHandStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageTwoHandCasterStore(ItemDamagefmt);
DBCStorage <ItemDamageEntry>              sItemDamageWandStore(ItemDamagefmt);
DBCStorage <gtItemSocketCostPerLevelEntry> sgtItemSocketCostPerLevelStore(gtItemSocketCostPerLevelfmt);

//DBCStorage <ItemDisplayInfoEntry>       sItemDisplayInfoStore(ItemDisplayTemplateEntryfmt); // Not used currently
DBCStorage <ItemSetEntry>                 sItemSetStore(ItemSetEntryfmt);

DBCStorage <LFGDungeonEntry>             sLFGDungeonStore(LFGDungeonEntryfmt);
DBCStorage <LiquidTypeEntry>             sLiquidTypeStore(LiquidTypefmt);
DBCStorage <LockEntry>                   sLockStore(LockEntryfmt);
DBCStorage <MapEntry>                    sMapStore(MapEntryfmt);

// DBC used only for initialization sMapDifficultyMap at startup.
DBCStorage <MapDifficultyEntry>          sMapDifficultyStore(MapDifficultyEntryfmt); // Only for loading
MapDifficultyMap                         sMapDifficultyMap;

DBCStorage <MinorTalentEntry>            sMinorTalentStore(MinorTalentfmt);
DBCStorage <MovieEntry>                  sMovieStore(MovieEntryfmt);

DBCStorage <PowerDisplayEntry>           sPowerDisplayStore(PowerDisplayfmt);
DBCStorage <PvPDifficultyEntry>          sPvPDifficultyStore(PvPDifficultyfmt);

DBCStorage <QuestFactionRewEntry>        sQuestFactionRewardStore(QuestFactionRewardfmt);
DBCStorage <RandomPropertiesPointsEntry> sRandomPropertiesPointsStore(RandomPropertiesPointsfmt);

DBCStorage <ScenarioStepEntry> sScenarioStepStore(ScenarioStepEntryfmt);

DBCStorage <SkillLineEntry>              sSkillLineStore(SkillLinefmt);
DBCStorage <SkillLineAbilityEntry>       sSkillLineAbilityStore(SkillLineAbilityfmt);

DBCStorage <SpellItemEnchantmentEntry>   sSpellItemEnchantmentStore(SpellItemEnchantmentfmt);
DBCStorage <SpellEntry>                  sSpellStore(SpellEntryfmt);
SpellSkillingList                        sSpellSkillingList;
PetFamilySpellsStore                     sPetFamilySpellsStore;

DBCStorage <SpellScalingEntry>           sSpellScalingStore(SpellScalingEntryfmt);
DBCStorage <SpellTargetRestrictionsEntry> sSpellTargetRestrictionsStore(SpellTargetRestrictionsEntryfmt);
DBCStorage <SpellLevelsEntry>            sSpellLevelsStore(SpellLevelsEntryfmt);
DBCStorage <SpellInterruptsEntry>        sSpellInterruptsStore(SpellInterruptsEntryfmt);
DBCStorage <SpellEquippedItemsEntry>     sSpellEquippedItemsStore(SpellEquippedItemsEntryfmt);
DBCStorage <SpellCooldownsEntry>         sSpellCooldownsStore(SpellCooldownsEntryfmt);
DBCStorage <SpellAuraOptionsEntry>       sSpellAuraOptionsStore(SpellAuraOptionsEntryfmt);

SpellEffectMap                           sSpellEffectMap;
SpellReagentMap                          sSpellReagentMap;

DBCStorage <SpellCategoriesEntry>        sSpellCategoriesStore(SpellCategoriesEntryfmt);
DBCStorage <SpellCategoryEntry>          sSpellCategoryStore(SpellCategoryEntryfmt);
DBCStorage <SpellEffectEntry>            sSpellEffectStore(SpellEffectEntryfmt);
DBCStorage <SpellEffectScalingEntry>     sSpellEffectScalingStore(SpellEffectScalingEntryfmt);
DBCStorage <SpellFocusObjectEntry>       sSpellFocusObjectStore(SpellFocusObjectfmt);
DBCStorage <SpellShapeshiftEntry>        sSpellShapeshiftStore(SpellShapeshiftEntryfmt);
DBCStorage <SpellShapeshiftFormEntry>    sSpellShapeshiftFormStore(SpellShapeshiftFormfmt);
DBCStorage <SummonPropertiesEntry>       sSummonPropertiesStore(SummonPropertiesfmt);
DBCStorage <TalentEntry>                 sTalentStore(TalentEntryfmt);
DBCStorage <VehicleEntry>                sVehicleStore(VehicleEntryfmt);
DBCStorage <VehicleSeatEntry>            sVehicleSeatStore(VehicleSeatEntryfmt);
DBCStorage <WMOAreaTableEntry>           sWMOAreaTableStore(WMOAreaTableEntryfmt);
DBCStorage <WorldMapAreaEntry>           sWorldMapAreaStore(WorldMapAreaEntryfmt);
DBCStorage<WorldMapTransformsEntry>      sWorldMapTransformsStore(WorldMapTransformsfmt);
DBCStorage <World_PVP_AreaEntry>         sWorld_PVP_AreaStore(World_PVP_AreaEntryfmt);
DBCStorage <WorldSafeLocsEntry>          sWorldSafeLocsStore(WorldSafeLocsEntryfmt);
DBCStorage <PhaseEntry>                  sPhaseStores(PhaseEntryfmt);
DBCStorage <GtBattlePetXPEntry>          sGtBattlePetXPStore(GtBattlePetXPfmt);
DBCStorage <GtBattlePetTypeDamageModEntry> sGtBattlePetTypeDamageModStore(GtBattlePetTypeDamageModfmt);
DBCStorage <WorldStateEntry>             sWorldStateStore(WorldStateEntryfmt);
DBCStorage <WorldStateExpressionEntry>   sWorldStateExpressionStore(WorldStateExpressionEntryfmt);

typedef std::list<std::string> StoreProblemList;

uint32 DBCFileCount = 0;

static bool LoadDBC_assert_print(uint32 fsize, uint32 rsize, const std::string& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

template<class T>
inline void LoadDBC(uint32& availableDbcLocales, StoreProblemList& errors, DBCStorage<T>& storage, std::string const& dbcPath, std::string const& filename, std::string const* customFormat = NULL, std::string const* customIndexName = NULL)
{
    // Compatibility format and C++ structure sizes
    ASSERT(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDBC_assert_print(DBCFileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DBCFileCount;
    std::string dbcFilename = dbcPath + filename;
    SqlDbc * sql = NULL;
    if (customFormat)
        sql = new SqlDbc(&filename, customFormat, customIndexName, storage.GetFormat());

    if (storage.Load(dbcFilename.c_str(), sql))
    {
        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
        {
            if (!(availableDbcLocales & (1 << i)))
                continue;

            std::string localizedName(dbcPath);
            localizedName.append(localeNames[i]);
            localizedName.push_back('/');
            localizedName.append(filename);

            if (!storage.LoadStringsFrom(localizedName.c_str()))
                availableDbcLocales &= ~(1<<i);                 // Mark as not available for speedup next checks
        }
    }
    else
    {
        // Sort problematic dbc to (1) non compatible and (2) non-existed
        if (FILE* f = fopen(dbcFilename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100, " (exists, but has %u fields instead of " SIZEFMTD ") Possible wrong client version.", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errors.push_back(dbcFilename + buf);
            fclose(f);
        }
        else
            errors.push_back(dbcFilename);
    }

    delete sql;
}

void LoadDBCStores(const std::string& dataPath)
{
    uint32 oldMSTime = getMSTime();

    std::string dbcPath = dataPath+"dbc/";

    StoreProblemList bad_dbc_files;
    uint32 availableDbcLocales = 0xFFFFFFFF;

    LoadDBC(availableDbcLocales, bad_dbc_files, sAreaStore,                   dbcPath, "AreaTable.dbc");

    // Must be after sAreaStore loading
    for (uint32 i = 0; i < sAreaStore.GetNumRows(); ++i)           // Areaflag numbered from 0
    {
        if (AreaTableEntry const* area = sAreaStore.LookupEntry(i))
        {
            // Fill AreaId->DBC records
            sAreaFlagByAreaID.insert(AreaFlagByAreaID::value_type(uint16(area->ID), area->AreaBit));

            // fill MapId->DBC records (skip sub zones and continents)
            if (area->ParentAreaID == 0 && area->ContinentID != 0 && area->ContinentID != 1 && area->ContinentID != 530 && area->ContinentID != 571)
                sAreaFlagByMapID.insert(AreaFlagByMapID::value_type(area->ContinentID, area->AreaBit));
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sAnimKitStore,                dbcPath, "AnimKit.dbc");                                                      // 19865
    LoadDBC(availableDbcLocales, bad_dbc_files, sAreaTriggerStore,            dbcPath, "AreaTrigger.dbc");                                                  // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sArmorLocationStore,          dbcPath, "ArmorLocation.dbc");                                                // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sBankBagSlotPricesStore,      dbcPath, "BankBagSlotPrices.dbc");                                            // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sBattlemasterListStore,       dbcPath, "BattlemasterList.dbc");                                             // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sCharTitlesStore,             dbcPath, "CharTitles.dbc");                                                   // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sChatChannelsStore,           dbcPath, "ChatChannels.dbc");                                                 // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sChrClassesStore,             dbcPath, "ChrClasses.dbc");                                                   // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sChrRacesStore,               dbcPath, "ChrRaces.dbc");                                                     // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sChrSpecializationsStore,     dbcPath, "ChrSpecialization.dbc");                                            // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sCinematicCameraStore,        dbcPath, "CinematicCamera.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureDisplayInfoExtraStore, dbcPath, "CreatureDisplayInfoExtra.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureFamilyStore,         dbcPath, "CreatureFamily.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sCreatureModelDataStore,      dbcPath, "CreatureModelData.dbc");                                            // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sDifficultyStore,             dbcPath, "Difficulty.dbc");                                                   // 19027
    LoadDBC(availableDbcLocales, bad_dbc_files, sDungeonEncounterStore,       dbcPath, "DungeonEncounter.dbc");                                             // 17399

    /// Gruul Encounter (Blackrock Foundry)
    if (DungeonEncounterEntry const* l_Encounter = sDungeonEncounterStore.LookupEntry(1691))
        ((DungeonEncounterEntry*)l_Encounter)->CreatureDisplayID = 55050;

    LoadDBC(availableDbcLocales, bad_dbc_files, sDurabilityCostsStore,        dbcPath, "DurabilityCosts.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sEmotesStore,                 dbcPath, "Emotes.dbc");                                                       // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sEmotesTextStore,             dbcPath, "EmotesText.dbc");                                                   // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sEmotesTextSoundStore,        dbcPath, "EmotesTextSound.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sFactionStore,                dbcPath, "Faction.dbc");                                                      // 17399

    for (uint32 l_I = 0; l_I < sEmotesTextSoundStore.GetNumRows(); ++l_I)
    {
        if (EmotesTextSoundEntry const* l_Entry = sEmotesTextSoundStore.LookupEntry(l_I))
            sEmotesTextSoundMap[EmotesTextSoundKey(l_Entry->EmotesTextID, l_Entry->RaceID, l_Entry->Gender)] = l_Entry;
    }

    for (uint32 i=0; i<sFactionStore.GetNumRows(); ++i)
    {
        FactionEntry const* faction = sFactionStore.LookupEntry(i);
        if (faction && faction->ParentFactionID)
        {
            SimpleFactionsList &flist = sFactionTeamMap[faction->ParentFactionID];
            flist.push_back(i);
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sFactionTemplateStore,        dbcPath, "FactionTemplate.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sFileDataStore,               dbcPath, "FileData.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sGameObjectDisplayInfoStore,  dbcPath, "GameObjectDisplayInfo.dbc");                                        // 17399

    for (uint32 i = 0; i < sGameObjectDisplayInfoStore.GetNumRows(); ++i)
    {
        if (GameObjectDisplayInfoEntry const* info = sGameObjectDisplayInfoStore.LookupEntry(i))
        {
            if (info->maxX < info->minX)
                std::swap(*(float*)(&info->maxX), *(float*)(&info->minX));
            if (info->maxY < info->minY)
                std::swap(*(float*)(&info->maxY), *(float*)(&info->minY));
            if (info->maxZ < info->minZ)
                std::swap(*(float*)(&info->maxZ), *(float*)(&info->minZ));
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sGemPropertiesStore,          dbcPath, "GemProperties.dbc");                                                // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGlyphPropertiesStore,        dbcPath, "GlyphProperties.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sgtArmorMitigationByLvlStore, dbcPath, "gtArmorMitigationByLvl.dbc");                                       // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtBarberShopCostBaseStore,   dbcPath, "gtBarberShopCostBase.dbc");                                         // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtCombatRatingsStore,        dbcPath, "gtCombatRatings.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToMeleeCritBaseStore,dbcPath, "gtChanceToMeleeCritBase.dbc");                                      // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToMeleeCritStore,    dbcPath, "gtChanceToMeleeCrit.dbc");                                          // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToSpellCritBaseStore,dbcPath, "gtChanceToSpellCritBase.dbc");                                      // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtChanceToSpellCritStore,    dbcPath, "gtChanceToSpellCrit.dbc");                                          // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTLevelExperienceStore, dbcPath, "gtOCTLevelExperience.dbc");                                           // 19027
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTHpPerStaminaStore,      dbcPath, "gtOCTHpPerStamina.dbc");                                            // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtRegenMPPerSptStore,        dbcPath, "gtRegenMPPerSpt.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtSpellScalingStore,         dbcPath, "gtSpellScaling.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTBaseHPByClassStore,     dbcPath, "gtOCTBaseHPByClass.dbc");                                           // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtOCTBaseMPByClassStore,     dbcPath, "gtOCTBaseMPByClass.dbc");                                           // 17399

    LoadDBC(availableDbcLocales, bad_dbc_files, sItemSetSpellStore,           dbcPath, "ItemSetSpell.dbc");                                                 // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemBagFamilyStore,          dbcPath, "ItemBagFamily.dbc");                                                // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemSetStore,                dbcPath, "ItemSet.dbc");                                                      // 17399

    LoadDBC(availableDbcLocales, bad_dbc_files, sItemArmorQualityStore,       dbcPath, "ItemArmorQuality.dbc");                                             // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemArmorShieldStore,        dbcPath, "ItemArmorShield.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemArmorTotalStore,         dbcPath, "ItemArmorTotal.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageAmmoStore,         dbcPath, "ItemDamageAmmo.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageOneHandStore,      dbcPath, "ItemDamageOneHand.dbc");                                            // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageOneHandCasterStore,dbcPath, "ItemDamageOneHandCaster.dbc");                                      // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageRangedStore,       dbcPath, "ItemDamageRanged.dbc");                                             // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageThrownStore,       dbcPath, "ItemDamageThrown.dbc");                                             // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageTwoHandStore,      dbcPath, "ItemDamageTwoHand.dbc");                                            // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageTwoHandCasterStore,dbcPath, "ItemDamageTwoHandCaster.dbc");                                      // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sItemDamageWandStore,         dbcPath, "ItemDamageWand.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sgtItemSocketCostPerLevelStore, dbcPath, "gtItemSocketCostPerLevel.dbc");                                   // 19034

    LoadDBC(availableDbcLocales, bad_dbc_files, sLFGDungeonStore,             dbcPath, "LfgDungeons.dbc");                                                  // 17399

    HotfixLfgDungeonsData();

    LoadDBC(availableDbcLocales, bad_dbc_files, sLiquidTypeStore,             dbcPath, "LiquidType.dbc");                                                   // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sLockStore,                   dbcPath, "Lock.dbc");                                                         // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sPhaseStores,                 dbcPath, "Phase.dbc");                                                        // 17399

    LoadDBC(availableDbcLocales, bad_dbc_files, sMapStore,                    dbcPath, "Map.dbc");                                                          // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sMapDifficultyStore, dbcPath, "MapDifficulty.dbc");                                                         // 17399

    /// Make shipyards instances
    if (MapEntry* l_MapEntry = const_cast<MapEntry*>(sMapStore.LookupEntry(1473)))
        l_MapEntry->instanceType = MAP_INSTANCE;

    sMapDifficultyMap[1473][1] = MapDifficulty(1, 0, 40, 0, 0);

    if (MapEntry* l_MapEntry = const_cast<MapEntry*>(sMapStore.LookupEntry(1474)))
        l_MapEntry->instanceType = MAP_INSTANCE;

    sMapDifficultyMap[1474][1] = MapDifficulty(1, 0, 40, 0, 0);

    /// Fill data
    {
        sMapDifficultyMap[0][0] = MapDifficulty(DifficultyNone, 0, 0, 0, false);                                                                              // Map 0 is missingg from MapDifficulty.dbc use this till its ported to sql

        for (uint32 i = 0; i < sMapDifficultyStore.GetNumRows(); ++i)
        {
            if (MapDifficultyEntry const* l_MapDiffculty = sMapDifficultyStore.LookupEntry(i))
            {
                uint32 l_ItemBonusTreeDiff = l_MapDiffculty->ItemBonusTreeDifficulty;

                switch (l_MapDiffculty->Id)
                {
                    case 2952:  ///< Highmaul - Heroic
                        l_ItemBonusTreeDiff = 5;
                        break;
                    case 2953:  ///< Highmaul - Mythic
                        l_ItemBonusTreeDiff = 6;
                        break;
                    default:
                        break;
                }

                sMapDifficultyMap[l_MapDiffculty->MapId][l_MapDiffculty->Difficulty] = MapDifficulty(l_MapDiffculty->Difficulty, l_MapDiffculty->ResetTime,
                    l_MapDiffculty->MaxPlayers, l_ItemBonusTreeDiff, l_MapDiffculty->AreaTriggerText[0] > 0);
            }
        }

        sMapDifficultyStore.Clear();
    }
    
    /// Hackfix to make Tarren Mill as a continent
    MapEntry* l_Map = (MapEntry*)sMapStore.LookupEntry(1280);
    if (l_Map)
        l_Map->instanceType = InstanceTypes::MAP_COMMON;    

    LoadDBC(availableDbcLocales, bad_dbc_files, sMinorTalentStore,            dbcPath, "MinorTalent.dbc");
    LoadDBC(availableDbcLocales, bad_dbc_files, sMovieStore,                  dbcPath, "Movie.dbc");                                                        // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sPowerDisplayStore,           dbcPath, "PowerDisplay.dbc");                                                 // 19116
    LoadDBC(availableDbcLocales, bad_dbc_files, sPvPDifficultyStore,          dbcPath, "PvpDifficulty.dbc");                                                // 17399

    for (uint32 i = 0; i < sPvPDifficultyStore.GetNumRows(); ++i)
    {
        if (PvPDifficultyEntry const* entry = sPvPDifficultyStore.LookupEntry(i))
        {
            if (entry->bracketId > MS::Battlegrounds::Brackets::Count)
                ASSERT(false && "Need update Brackets::Count by DBC data");
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sQuestFactionRewardStore,     dbcPath, "QuestFactionReward.dbc");                                           // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sRandomPropertiesPointsStore, dbcPath, "RandPropPoints.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sScenarioStepStore,           dbcPath, "ScenarioStep.dbc");                                                 // 19027
    LoadDBC(availableDbcLocales, bad_dbc_files, sSkillLineStore,              dbcPath, "SkillLine.dbc");                                                    // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSkillLineAbilityStore,       dbcPath, "SkillLineAbility.dbc");                                             // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellStore,                  dbcPath, "Spell.dbc"/*, &CustomSpellEntryfmt, &CustomSpellEntryIndex*/);      // 17399

    for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
    {
        SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);

        if (!skillLine)
            continue;

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
        SpellMiscEntry const* spellMisc = sSpellMiscStore.LookupEntry(spellInfo ? spellInfo->SpellMiscId : 0);
        if (spellMisc && spellMisc->Attributes & SPELL_ATTR0_PASSIVE)
        {
            for (uint32 i = 1; i < sCreatureFamilyStore.GetNumRows(); ++i)
            {
                SpellLevelsEntry const* levels = sSpellLevelsStore.LookupEntry(i);
                if (!levels)
                    continue;

                CreatureFamilyEntry const* cFamily = sCreatureFamilyStore.LookupEntry(i);
                if (!cFamily)
                    continue;

                if (skillLine->skillId != cFamily->skillLine[0] && skillLine->skillId != cFamily->skillLine[1])
                    continue;
                if (levels->spellLevel)
                    continue;

                if (skillLine->learnOnGetSkill != ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL)
                    continue;

                sPetFamilySpellsStore[i].insert(spellInfo->Id);
            }
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellScalingStore,           dbcPath,"SpellScaling.dbc");                                                  // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellTargetRestrictionsStore,dbcPath,"SpellTargetRestrictions.dbc");                                       // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellLevelsStore,            dbcPath,"SpellLevels.dbc");                                                   // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellInterruptsStore,        dbcPath,"SpellInterrupts.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellEquippedItemsStore,     dbcPath,"SpellEquippedItems.dbc");                                            // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCooldownsStore,         dbcPath,"SpellCooldowns.dbc");                                                // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellAuraOptionsStore,       dbcPath,"SpellAuraOptions.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCategoriesStore,        dbcPath,"SpellCategories.dbc");                                               // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellCategoryStore,          dbcPath,"SpellCategory.dbc");                                                 // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellEffectStore,            dbcPath,"SpellEffect.dbc");                                                   // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellEffectScalingStore,     dbcPath,"SpellEffectScaling.dbc");                                            // 17399

    for (uint32 i = 1; i < sSpellEffectStore.GetNumRows(); ++i)
    {
        if (SpellEffectEntry const *spellEffect = sSpellEffectStore.LookupEntry(i))
            sSpellEffectMap[spellEffect->EffectSpellId].effects[spellEffect->EffectDifficulty][spellEffect->EffectIndex] = spellEffect;
    }

    for (uint32 i = 1; i < sSpellStore.GetNumRows(); ++i)
    {
        if (SpellEntry const * spell = sSpellStore.LookupEntry(i))
        {
            if (const SpellEffectEntry* spellEffect = spell->GetSpellEffect(EFFECT_1, 0))
            {
                if (spellEffect->Effect == SPELL_EFFECT_SKILL && IsProfessionSkill(spellEffect->EffectMiscValue))
                    sSpellSkillingList.push_back(spell);
            }
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellFocusObjectStore,       dbcPath, "SpellFocusObject.dbc");                                             // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellItemEnchantmentStore,   dbcPath, "SpellItemEnchantment.dbc");                                         // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellShapeshiftStore,        dbcPath, "SpellShapeshift.dbc");                                              // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSpellShapeshiftFormStore,    dbcPath, "SpellShapeshiftForm.dbc");                                          // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sSummonPropertiesStore,       dbcPath, "SummonProperties.dbc");                                             // 17399

    // Since mop, we count 7 entries with slot = -1, we must set them at 0, if not, crash !
    for (uint32 i = 0; i < sSummonPropertiesStore.GetNumRows(); ++i)
    {
        if (SummonPropertiesEntry const* prop = sSummonPropertiesStore.LookupEntry(i))
        {
            if (prop->Slot >= MAX_SUMMON_SLOT)
                ((SummonPropertiesEntry*)prop)->Slot = 0;
        }
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sTalentStore,                 dbcPath, "Talent.dbc");                                                       // 17399

    for (uint32 i = 0; i < sTransportAnimationStore.GetNumRows(); ++i)
    {
        TransportAnimationEntry const* anim = sTransportAnimationStore.LookupEntry(i);
        if (!anim)
            continue;

        sTransportMgr->AddPathNodeToTransport(anim->TransportEntry, anim->TimeSeg, anim);
    }

    for (uint32 i = 0; i < sTransportRotationStore.GetNumRows(); ++i)
    {
        TransportRotationEntry const* rot = sTransportRotationStore.LookupEntry(i);
        if (!rot)
            continue;

        sTransportMgr->AddPathRotationToTransport(rot->TransportEntry, rot->TimeSeg, rot);
    }
    LoadDBC(availableDbcLocales, bad_dbc_files, sVehicleStore,                dbcPath, "Vehicle.dbc");                                                      // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sVehicleSeatStore,            dbcPath, "VehicleSeat.dbc", &CustomVehicleSeatEntryfmt, &CustomVehicleSeatEntryIndex);                                                // 17399

    // @TODO: Move this hack to vehicle_seat_dbc table
    if (VehicleEntry * vehicle = (VehicleEntry*)sVehicleStore.LookupEntry(584))
    {
        vehicle->m_seatID[0] = 20000;
        vehicle->m_seatID[1] = 20001;
        vehicle->m_seatID[2] = 20002;
        vehicle->m_seatID[3] = 20003;
    }

    LoadDBC(availableDbcLocales, bad_dbc_files, sWMOAreaTableStore,           dbcPath, "WMOAreaTable.dbc");                                                 // 17399
    for (uint32 i = 0; i < sWMOAreaTableStore.GetNumRows(); ++i)
        if (WMOAreaTableEntry const* entry = sWMOAreaTableStore.LookupEntry(i))
            sWMOAreaInfoByTripple.insert(WMOAreaInfoByTripple::value_type(WMOAreaTableTripple(entry->rootId, entry->adtId, entry->groupId), entry));

    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldMapAreaStore,             dbcPath, "WorldMapArea.dbc");                                                 // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldMapTransformsStore,       dbcPath, "WorldMapTransforms.dbc");                                           // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorld_PVP_AreaStore,           dbcPath, "World_PVP_Area.dbc");                                               // 19027
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldSafeLocsStore,            dbcPath, "WorldSafeLocs.dbc");                                                // 17399

    for (uint32 l_I = 0; l_I < sWorldSafeLocsStore.GetNumRows(); ++l_I)
    {
        WorldSafeLocsEntry* l_WorldSafeLocs = (WorldSafeLocsEntry*)sWorldSafeLocsStore.LookupEntry(l_I);
        if (l_WorldSafeLocs == nullptr)
            continue;

        // Facing is in degrees ... WTF
        l_WorldSafeLocs->o *= M_PI / 180;
    }

    // Battle pets
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtBattlePetXPStore,            dbcPath, "gtBattlePetXP.dbc");                                                // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sGtBattlePetTypeDamageModStore, dbcPath, "gtBattlePetTypeDamageMod.dbc");                                     // 17399
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldStateStore,               dbcPath, "WorldState.dbc");                                                   // 19865
    LoadDBC(availableDbcLocales, bad_dbc_files, sWorldStateExpressionStore,     dbcPath, "WorldStateExpression.dbc");                                         // 19865

    /// Uncomment this to disam world state expressions
    ///for (uint32 l_I = 0; l_I < sWorldStateExpressionStore.GetNumRows(); l_I++)
    ///{
    ///    WorldStateExpressionEntry const* l_WorldStateExpression = sWorldStateExpressionStore.LookupEntry(i);
    ///
    ///    if (!l_WorldStateExpression)
    ///        continue;
    ///
    ///    std::vector<std::string> l_Lines;
    ///    l_WorldStateExpression->Eval(nullptr, &l_Lines);
    ///
    ///    FILE * l_File = fopen(("wse/WorldStateExpression_" + std::to_string(l_I) + ".txt").c_str(), "w+");
    ///
    ///    if (!l_File)
    ///        continue;
    ///
    ///    for (auto l_Line : l_Lines)
    ///        fprintf(l_File, "%s\n", l_Line.c_str());
    ///
    ///    fclose(l_File);
    ///}

    for (uint32 i = 0; i < sItemSetSpellStore.GetNumRows(); i++)
    {
        ItemSetSpellEntry const* setSpells = sItemSetSpellStore.LookupEntry(i);

        if (!setSpells)
            continue;

        sItemSetSpellsByItemIDStore[setSpells->ItemSetID].push_back(setSpells);
    }

    // error checks
    if (bad_dbc_files.size() >= DBCFileCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Incorrect DataDir value in worldserver.conf or ALL required *.dbc files (%d) not found by path: %sdbc", DBCFileCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_dbc_files.empty())
    {
        std::string str;
        for (StoreProblemList::iterator i = bad_dbc_files.begin(); i != bad_dbc_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "Some required *.dbc files (%u from %d) not found or not compatible:\n%s", (uint32)bad_dbc_files.size(), DBCFileCount, str.c_str());
        exit(1);
    }

    // Check loaded DBC files proper version
    if (!sAreaStore.LookupEntry(6420)          ||     // Last area (areaflag) added in 6.2.0 (20216)
        !sCharTitlesStore.LookupEntry(457)     ||     // Last char title added in 6.2.0 (20216)
        !sGemPropertiesStore.LookupEntry(2544) ||     // Last gem property added in 6.2.0 (20216)
        !sMapStore.LookupEntry(1497)           ||     // Last map added in 6.2.0 (20216)
        !sSpellStore.LookupEntry(191981)       )      // Last spell added in 6.2.0 (20216)
    {
        sLog->outError(LOG_FILTER_GENERAL, "You have _outdated_ DBC files. Please extract correct versions from current using client.");
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Initialized %d DBC data stores in %u ms", DBCFileCount, GetMSTimeDiffToNow(oldMSTime));
}

SimpleFactionsList const* GetFactionTeamList(uint32 faction)
{
    FactionTeamMap::const_iterator itr = sFactionTeamMap.find(faction);
    if (itr != sFactionTeamMap.end())
        return &itr->second;

    return NULL;
}

char const* GetPetName(uint32 petfamily, uint32 /*dbclang*/)
{
    if (!petfamily)
        return NULL;
    CreatureFamilyEntry const* pet_family = sCreatureFamilyStore.LookupEntry(petfamily);
    if (!pet_family)
        return NULL;
    return pet_family->Name ? pet_family->Name : NULL;
}

SpellEffectEntry const* GetSpellEffectEntry(uint32 spellId, uint32 effect, uint32 difficulty)
{
    SpellEffectMap::const_iterator itr = sSpellEffectMap.find(spellId);
    if (itr == sSpellEffectMap.end())
        return NULL;

    if (itr->second.effects[difficulty][effect])
        return itr->second.effects[difficulty][effect];

    return itr->second.effects[DifficultyNone][effect];
}

SpellEffectScalingEntry const* GetSpellEffectScalingEntry(uint32 effectId)
{
    return sSpellEffectScalingStore.LookupEntry(effectId);
}

SpellReagentsEntry const* GetSpellReagentEntry(uint32 spellId, uint8 reagent)
{
    SpellReagentMap::const_iterator itr = sSpellReagentMap.find(spellId);
    if (itr == sSpellReagentMap.end())
        return NULL;

    return itr->second.reagents[reagent];
}

/// still usefull ?
TalentSpellPos const* GetTalentSpellPos(uint32 /*spellId*/)
{
    return NULL;
    /*TalentSpellPosMap::const_iterator itr = sTalentSpellPosMap.find(spellId);
    if (itr == sTalentSpellPosMap.end())
        return NULL;

    return &itr->second;*/
}

uint32 GetTalentSpellCost(uint32 spellId)
{
    if (TalentSpellPos const* pos = GetTalentSpellPos(spellId))
        return pos->rank+1;

    return 0;
}

int32 GetAreaFlagByAreaID(uint32 area_id)
{
    AreaFlagByAreaID::iterator i = sAreaFlagByAreaID.find(area_id);
    if (i == sAreaFlagByAreaID.end())
        return -1;

    return i->second;
}

EmotesTextSoundEntry const* FindTextSoundEmoteFor(uint32 p_Emote, uint32 p_Race, uint32 p_Gender)
{
    auto l_Iter = sEmotesTextSoundMap.find(EmotesTextSoundKey(p_Emote, p_Race, p_Gender));
    return l_Iter != sEmotesTextSoundMap.end() ? l_Iter->second : nullptr;
}

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid)
{
    WMOAreaInfoByTripple::iterator i = sWMOAreaInfoByTripple.find(WMOAreaTableTripple(rootid, adtid, groupid));
        if (i == sWMOAreaInfoByTripple.end())
            return NULL;
        return i->second;
}

AreaTableEntry const* GetAreaEntryByAreaID(uint32 area_id)
{
    int32 areaflag = GetAreaFlagByAreaID(area_id);
    if (areaflag < 0)
        return NULL;

    return sAreaStore.LookupEntry(areaflag);
}

AreaTableEntry const* GetAreaEntryByAreaFlagAndMap(uint32 area_flag, uint32 map_id)
{
    if (area_flag)
        return sAreaStore.LookupEntry(area_flag);

    if (MapEntry const* mapEntry = sMapStore.LookupEntry(map_id))
        return GetAreaEntryByAreaID(mapEntry->AreaTableID);

    return NULL;
}

uint32 GetAreaFlagByMapId(uint32 mapid)
{
    AreaFlagByMapID::iterator i = sAreaFlagByMapID.find(mapid);
    if (i == sAreaFlagByMapID.end())
        return 0;
    else
        return i->second;
}

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId)
{
    if (mapid != 530 && mapid != 571 && mapid != 732)   // Speed for most cases
        return mapid;

    if (WorldMapAreaEntry const* wma = sWorldMapAreaStore.LookupEntry(zoneId))
        return wma->virtual_map_id >= 0 ? wma->virtual_map_id : wma->map_id;

    return mapid;
}

uint32 GetMaxLevelForExpansion(uint32 p_Expansion)
{
    switch (p_Expansion)
    {
        case CONTENT_1_60:
            return 60;
        case CONTENT_61_70:
            return 70;
        case CONTENT_71_80:
            return 80;
        case CONTENT_81_85:
            return 85;
        case CONTENT_86_90:
            return 90;
        case CONTENT_91_100:
            return 100;
        default:
            break;

    }
    return 0;
}

/*
Used only for calculate xp gain by content lvl.
Calculation on Gilneas and group maps of LostIslands calculated as CONTENT_1_60.
Vaj'shir, Uldum, Deepholm and Mount Hyjal are Cataclysm zone ( CONTENT_81_85 ).
*/
ContentLevels GetContentLevelsForMapAndZone(uint32 mapid, uint32 zoneId)
{
    mapid = GetVirtualMapForMapAndZone(mapid, zoneId);
    if (mapid < 2)
    {
        switch (zoneId)
        {
            case 5144:  // Vaj'shir
            case 5145:  // Vaj'shir
            case 4815:  // Vaj'shir
            case 4816:  // Vaj'shir
            case 5146:  // Vaj'shir
            case 5034:  // Uldum
            case 5052:  // Deepholm
            case 616:   // Mount Hyjal
                return CONTENT_81_85;
            default:
                return CONTENT_1_60;
        }
    };

    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    if (!mapEntry)
        return CONTENT_1_60;

    // No need enum all maps from phasing
    if (mapEntry->ParentMapID >= 0)
        mapid = mapEntry->ParentMapID;

    switch (mapid)
    {
        case 648:   // Lost Islands
        case 654:   // Gilneas
            return CONTENT_1_60;
        default:
            return ContentLevels(mapEntry->Expansion());
    }
}

bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId)
{
    if (requiredTotemCategoryId == 0)
        return true;
    if (itemTotemCategoryId == 0)
        return false;

    TotemCategoryEntry const* itemEntry = sTotemCategoryStore.LookupEntry(itemTotemCategoryId);
    if (!itemEntry)
        return false;
    TotemCategoryEntry const* reqEntry = sTotemCategoryStore.LookupEntry(requiredTotemCategoryId);
    if (!reqEntry)
        return false;

    if (itemEntry->categoryType != reqEntry->categoryType)
        return false;

    return (itemEntry->categoryMask & reqEntry->categoryMask) == reqEntry->categoryMask;
}

void Zone2MapCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // If not listed then map coordinates (instance)
    if (!maEntry)
        return;

    std::swap(x, y);                                         // At client map coords swapped
    x = x*((maEntry->x2-maEntry->x1)/100)+maEntry->x1;
    y = y*((maEntry->y2-maEntry->y1)/100)+maEntry->y1;      // Client y coord from top to down
}

void Map2ZoneCoordinates(float& x, float& y, uint32 zone)
{
    WorldMapAreaEntry const* maEntry = sWorldMapAreaStore.LookupEntry(zone);

    // If not listed then map coordinates (instance)
    if (!maEntry)
        return;

    x = (x-maEntry->x1)/((maEntry->x2-maEntry->x1)/100);
    y = (y-maEntry->y1)/((maEntry->y2-maEntry->y1)/100);    // Client y coord from top to down
    std::swap(x, y);                                        // Client have map coords swapped
}

MapDifficulty const* GetDefaultMapDifficulty(uint32 p_MapID, Difficulty* p_Difficulty /*= nullptr*/)
{
    auto l_It = sMapDifficultyMap.find(p_MapID);
    if (l_It == sMapDifficultyMap.end())
        return nullptr;

    if (l_It->second.empty())
        return nullptr;

    for (auto& l_Pair : l_It->second)
    {
        DifficultyEntry const* l_DifficultyEntry = sDifficultyStore.LookupEntry(l_Pair.first);
        if (!l_DifficultyEntry)
            continue;

        if (l_DifficultyEntry->Flags & DIFFICULTY_FLAG_DEFAULT)
        {
            if (p_Difficulty)
                *p_Difficulty = Difficulty(l_Pair.first);

            return &l_Pair.second;
        }
    }

    if (p_Difficulty)
        *p_Difficulty = Difficulty(l_It->second.begin()->first);

    return &l_It->second.begin()->second;
}

MapDifficulty const* GetMapDifficultyData(uint32 p_MapID, Difficulty p_Difficulty)
{
    auto l_It = sMapDifficultyMap.find(p_MapID);
    if (l_It == sMapDifficultyMap.end())
        return nullptr;

    auto diffItr = l_It->second.find(p_Difficulty);
    if (diffItr == l_It->second.end())
        return nullptr;

    return &diffItr->second;
}

// @todo: add support for the new difficulty SCENARIO_HEROIC_DIFFICULTY, SCENARIO_DIFFICULTY, and DYNAMIC_DIFFICULTY
MapDifficulty const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &l_Difficulty)
{
    DifficultyEntry const* l_DifficultyEntry = sDifficultyStore.LookupEntry(l_Difficulty);
    if (!l_DifficultyEntry)
        return GetDefaultMapDifficulty(mapId, &l_Difficulty);

    uint32 l_TempDifficulty = l_Difficulty;
    MapDifficulty const* l_MapDifficulty = GetMapDifficultyData(mapId, Difficulty(l_TempDifficulty));

    while (!l_MapDifficulty)
    {
        l_TempDifficulty = l_DifficultyEntry->FallbackDifficultyID;
        l_DifficultyEntry = sDifficultyStore.LookupEntry(l_TempDifficulty);

        if (!l_DifficultyEntry)
            return GetDefaultMapDifficulty(mapId, &l_Difficulty);

        // pull new data
        l_MapDifficulty = GetMapDifficultyData(mapId, Difficulty(l_TempDifficulty)); // we are 10 normal or 25 normal
    }

    l_Difficulty = Difficulty(l_TempDifficulty);
    return l_MapDifficulty;
}

std::vector<uint32> const* GetTalentTreePrimarySpells(uint32 /*talentTree*/)
{
    return NULL;
    /*TalentTreePrimarySpellsMap::const_iterator itr = sTalentTreePrimarySpellsMap.find(talentTree);
    if (itr == sTalentTreePrimarySpellsMap.end())
        return NULL;

    return &itr->second;*/
}

uint32 GetLiquidFlags(uint32 liquidType)
{
    if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(liquidType))
        return 1 << liq->Type;

    return 0;
}

uint32 GetQuestUniqueBitFlag(uint32 questId)
{
    QuestV2Entry const* v2 = sQuestV2Store.LookupEntry(questId);
    if (!v2)
        return 0;

    return v2->UniqueBitFlag;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static uint8 const g_WorldState_UnpackUnkTable[] = {
    0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
    0x20, 0x00, 0x28, 0x00, 0x28, 0x00, 0x28, 0x00, 0x28, 0x00, 0x28, 0x00, 0x20, 0x00, 0x20, 0x00,
    0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
    0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
    0x48, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00,
    0x84, 0x00, 0x84, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x81, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x82, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

namespace WorldStateExpressionMathOpcode
{
    enum
    {
        Add         = 1,
        Substract   = 2,
        Multiply    = 3,
        Divide      = 4,
        Modulo      = 5
    };
}

namespace WorldStateExpressionCompareOpcode
{
    enum
    {
        Equal       = 1,
        Different   = 2,
        Less        = 3,
        LessOrEqual = 4,
        More        = 5,
        MoreOrEqual = 6
    };
}

namespace WorldStateExpressionCustomOpType
{
    enum
    {
        PushUInt32          = 1,
        PushWorldStateValue = 2,
        CallFunction        = 3
    };
}

namespace WorldStateExpressionEvalResultLogic
{
    enum
    {
        FirstAndSecond      = 1,
        FirstOrSecond       = 2,
        NotFirstOrNotSecond = 3
    };
}

namespace WorldStateExpressionFunctions
{
    enum
    {
        None,
        Random,
        Month,
        Day,
        TimeOfDay,
        Region,
        ClockHour,
        DifficultyID,
        HolidayStart,
        HolidayLeft,
        HolidayActive,
        TimerCurrentTime,
        WeekNumber,
        None2,
        None3,
        None4,
        None5,
        None6,
        None7,
        None8,
        None9,
        None10
    };

    const char * Names[] =
    {
        "None",
        "Random",
        "Month",
        "Day",
        "TimeOfDay",
        "Region",
        "ClockHour",
        "DifficultyID",
        "HolidayStart",
        "HolidayLeft",
        "HolidayActive",
        "TimerCurrentTime",
        "WeekNumber",
        "None2",
        "None3",
        "None4",
        "None5",
        "None6",
        "None7",
        "None8",
        "None9",
        "None10"
    };
}

static std::function<int32(Player*, int32, int32)> g_WorldStateExpressionFunction[] =
{
    /// WorldStateExpressionFunctions::None
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::Random
    [](Player* /*p_Player*/, int32 p_Min, int32 p_Max) -> int32
    {
        return urand(p_Min, p_Max);
    },
    /// WorldStateExpressionFunctions::Month
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        MS::Utilities::WowTime l_Time;
        l_Time.SetUTCTimeFromPosixTime(sWorld->GetGameTime());

        return l_Time.Month + 1;
    },
    /// WorldStateExpressionFunctions::Day
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        MS::Utilities::WowTime l_Time;
        l_Time.SetUTCTimeFromPosixTime(sWorld->GetGameTime());

        return l_Time.MonthDay + 1;
    },
    /// WorldStateExpressionFunctions::TimeOfDay
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        MS::Utilities::WowTime l_Time;
        l_Time.SetUTCTimeFromPosixTime(sWorld->GetGameTime());

        return l_Time.GetHourAndMinutes();
    },
    /// WorldStateExpressionFunctions::Region
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return sWorld->GetServerRegionID();
    },
    /// WorldStateExpressionFunctions::ClockHour
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        MS::Utilities::WowTime l_Time;
        l_Time.SetUTCTimeFromPosixTime(sWorld->GetGameTime());

        if (l_Time.Hour <= 12)
            return l_Time.Hour;

        if (l_Time.Hour - 12)
            return l_Time.Hour - 12;

        return 12;
    },
    /// WorldStateExpressionFunctions::Region
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return sWorld->GetServerRegionID();
    },
    /// WorldStateExpressionFunctions::DifficultyID
    [](Player* p_Player, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        if (!p_Player)
            return 0;

        return p_Player->GetMap()->GetDifficultyID();
    },
    /// WorldStateExpressionFunctions::HolidayStart
    [](Player* /*p_Player*/, int32 p_HolidayID, int32 p_Arg2) -> int32
    {
        HolidaysEntry const* l_Entry = sHolidaysStore.LookupEntry(p_HolidayID);

        if (!l_Entry || (p_Arg2 > 0 && !l_Entry->Duration[p_Arg2]))
            return 0;

        int l_ChoosedDuration = l_Entry->Duration[p_Arg2];
        if (!l_ChoosedDuration)
            l_ChoosedDuration = 24;

        time_t l_CurrentTime = sWorld->GetGameTime();
        struct tm l_LocalTime;
        l_LocalTime.tm_isdst = -1;

        ACE_OS::localtime_r(&l_CurrentTime, &l_LocalTime);

        MS::Utilities::WowTime l_GameTime;
        l_GameTime.SetUTCTimeFromPosixTime(l_CurrentTime);
        l_GameTime.YearDay = l_LocalTime.tm_yday;

        for (uint32 l_I = 0; l_I < MAX_HOLIDAY_DATES; ++l_I)
        {
            MS::Utilities::WowTime l_Time;
            l_Time.Decode(l_Entry->Date[l_I]);

            if (l_Entry->flags & 1)
            {
                l_Time.YearDay = l_GameTime.YearDay;
                l_Time.ComputeRegionTime(l_Time);
            }

            if (l_Time.Minute   < 0) l_Time.Minute      = l_GameTime.Minute;
            if (l_Time.Hour     < 0) l_Time.Hour        = l_GameTime.Hour;
            if (l_Time.MonthDay < 0) l_Time.MonthDay    = l_GameTime.Minute;
            if (l_Time.Month    < 0) l_Time.Month       = l_GameTime.Month;
            if (l_Time.Year     < 0)
            {
                l_Time.Year = l_GameTime.Year;

                if (l_GameTime < l_Time)
                    --l_Time.Year;
            }

            if (l_Entry->Looping)
            {
                int32 l_I = 0;
                int32 v7 = 0;
                int32 v8 = 0;
                do
                {
                    if (l_I >= p_Arg2)
                    {
                        if (l_I > p_Arg2)
                            v8 += l_Entry->Duration[l_I];
                    }
                    else
                    {
                        v7 += l_Entry->Duration[l_I];
                        if (!l_I && !l_Entry->Duration[0])
                            v7 += 24;
                    }
                    ++l_I;
                } while (l_I < MAX_HOLIDAY_DURATIONS);

                int v10 = MS::Utilities::Globals::InMinutes::Hour * v7;
                int v11 = MS::Utilities::Globals::InMinutes::Hour * v8;
                while (!(l_GameTime <= l_Time))
                {
                    l_Time.AddHolidayDuration(v10);
                    l_Time.AddHolidayDuration(l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour);
                    if (!(l_GameTime >= l_Time))
                    {
                        int l_Result = (MS::Utilities::Globals::InMinutes::Day * (l_Time.GetDaysSinceEpoch() - l_GameTime.GetDaysSinceEpoch())) - l_GameTime.GetHourAndMinutes();
                        return l_Result + l_Time.GetHourAndMinutes();
                    }
                    l_Time.AddHolidayDuration(v11);
                }
                return l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour;
            }

            if (l_GameTime <= l_Time)
            {
                for (int32 l_Y = 0; l_Y < p_Arg2 && l_Y < MAX_HOLIDAY_DURATIONS; ++l_Y)
                {
                    uint32 l_Value = l_Entry->Duration[l_Y];
                    if (!l_Value)
                        l_Value = 24;

                    l_Time.AddHolidayDuration(l_Value * MS::Utilities::Globals::InMinutes::Hour);
                }

                l_Time.AddHolidayDuration(l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour);

                if (!(l_GameTime >= l_Time))
                    return l_Time.DiffTime(l_GameTime) / MS::Utilities::Globals::InMinutes::Hour;
            }
        }

        return l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour;
    },
    /// WorldStateExpressionFunctions::HolidayLeft
    [](Player* /*p_Player*/, int32 p_HolidayID, int32 p_Arg2) -> int32
    {
        HolidaysEntry const* l_Entry = sHolidaysStore.LookupEntry(p_HolidayID);

        if (!l_Entry || (p_Arg2 <= 0 && l_Entry->Duration[p_Arg2]))
            return 0;

        int l_ChoosedDuration = l_Entry->Duration[p_Arg2];
        if (!l_ChoosedDuration)
            l_ChoosedDuration = 24;

        time_t l_CurrentTime = sWorld->GetGameTime();
        struct tm l_LocalTime;
        l_LocalTime.tm_isdst = -1;

        ACE_OS::localtime_r(&l_CurrentTime, &l_LocalTime);

        MS::Utilities::WowTime l_GameTime;
        l_GameTime.SetUTCTimeFromPosixTime(l_CurrentTime);
        l_GameTime.YearDay = l_LocalTime.tm_yday;

        for (uint32 l_I = 0; l_I < MAX_HOLIDAY_DATES; ++l_I)
        {
            MS::Utilities::WowTime l_Time;
            l_Time.Decode(l_Entry->Date[l_I]);

            if (l_Entry->flags & 1)
            {
                l_Time.YearDay = l_GameTime.YearDay;
                l_Time.ComputeRegionTime(l_Time);
            }

            if (l_Time.Minute   < 0) l_Time.Minute      = l_GameTime.Minute;
            if (l_Time.Hour     < 0) l_Time.Hour        = l_GameTime.Hour;
            if (l_Time.MonthDay < 0) l_Time.MonthDay    = l_GameTime.Minute;
            if (l_Time.Month    < 0) l_Time.Month       = l_GameTime.Month;
            if (l_Time.Year     < 0)
            {
                l_Time.Year = l_GameTime.Year;

                if (l_GameTime < l_Time)
                    --l_Time.Year;
            }

            if (l_Entry->Looping)
            {
                int32 l_I = 0;
                int32 v7 = 0;
                int32 v8 = 0;
                do
                {
                    if (l_I >= p_Arg2)
                    {
                        if (l_I > p_Arg2)
                            v8 += l_Entry->Duration[l_I];
                    }
                    else
                    {
                        v7 += l_Entry->Duration[l_I];
                        if (!l_I && !l_Entry->Duration[0])
                            v7 += 24;
                    }
                    ++l_I;
                } while (l_I < MAX_HOLIDAY_DURATIONS);

                int v10 = MS::Utilities::Globals::InMinutes::Hour * v7;
                int v11 = MS::Utilities::Globals::InMinutes::Hour * v8;
                while (!(l_GameTime <= l_Time))
                {
                    l_Time.AddHolidayDuration(v10);
                    l_Time.AddHolidayDuration(l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour);
                    if (!(l_GameTime >= l_Time))
                    {
                        int l_Result = (MS::Utilities::Globals::InMinutes::Day * (l_Time.GetDaysSinceEpoch() - l_GameTime.GetDaysSinceEpoch())) - l_GameTime.GetHourAndMinutes();
                        return l_Result + l_Time.GetHourAndMinutes();
                    }
                    l_Time.AddHolidayDuration(v11);
                }
                return l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour;
            }

            if (l_GameTime <= l_Time)
            {
                if (p_Arg2 > 0)
                {
                    for (int32 l_Y = 0; l_Y < p_Arg2 && l_Y < MAX_HOLIDAY_DURATIONS; ++l_Y)
                    {
                        uint32 l_Value = l_Entry->Duration[l_Y];
                        if (!l_Value)
                            l_Value = 24;

                        l_Time.AddHolidayDuration(l_Value * MS::Utilities::Globals::InMinutes::Hour);
                    }
                }

                l_Time.AddHolidayDuration(l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour);

                if (!(l_GameTime >= l_Time))
                    return l_Time.DiffTime(l_GameTime) / MS::Utilities::Globals::InMinutes::Hour;
            }
        }

        return l_ChoosedDuration * MS::Utilities::Globals::InMinutes::Hour;
    },
    /// WorldStateExpressionFunctions::HolidayActive
    [](Player* p_Player, int32 p_HolidayID, int32 /*p_Arg2*/) -> int32
    {
        HolidaysEntry const* l_Entry = sHolidaysStore.LookupEntry(p_HolidayID);

        if (l_Entry)
        {
            int l_I = 0;
            uint32 const* l_Durations = l_Entry->Duration;
            while (l_I <= 0 || *l_Durations)
            {
                signed int l_CurrentDuration = *l_Durations;

                if (!l_CurrentDuration)
                    l_CurrentDuration = 24;

                if (g_WorldStateExpressionFunction[WorldStateExpressionFunctions::HolidayStart](p_Player, p_HolidayID, l_I) < (MS::Utilities::Globals::InMinutes::Hour * l_CurrentDuration))
                    return 1;

                ++l_I;
                l_Durations += 4;

                if (l_I >= MAX_HOLIDAY_DURATIONS)
                    return 0;
            }
        }

        return 0;
    },
    /// WorldStateExpressionFunctions::TimerCurrentTime
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return time(nullptr);
    },
    /// WorldStateExpressionFunctions::WeekNumber
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        time_t l_Time = sWorld->GetGameTime();
        return (l_Time - sWorld->GetServerRaidOrigin()) / WEEK;
    },
    /// WorldStateExpressionFunctions::None2
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None3
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None4
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None5
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None6
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None7
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None8
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None9
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    },
    /// WorldStateExpressionFunctions::None10
    [](Player* /*p_Player*/, int32 /*p_Arg1*/, int32 /*p_Arg2*/) -> int32
    {
        return 0;
    }
};

/// 19865 - sub_A671A3
bool UnkSubFunction(char p_Char)
{
    /// @TODO NEED MORE WORK
    ///if (dword_1368528)
    ///{
    ///    return !!sub_A67152(p_Char, 0);
    ///}
    
    uint16_t l_Value = *(uint16_t*)(&g_WorldState_UnpackUnkTable[p_Char * 2]);
    return !!(l_Value & 4);
}

std::string UnpackWorldStateExpression(char const* p_Input)
{
    char const* l_ExpressionStr = p_Input;
    std::string l_Unpacked;

    while (*l_ExpressionStr)
    {
        char l_Out = 0;
        char l_Temp = 0;
        if (!UnkSubFunction(l_ExpressionStr[0]))
            l_Temp = l_ExpressionStr[0] - '7';
        else
            l_Temp = l_ExpressionStr[0] - '0';

        l_Out |= l_Temp;
        l_Out *= 16;

        if (!UnkSubFunction(l_ExpressionStr[1]))
            l_Temp = l_ExpressionStr[1] - '7';
        else
            l_Temp = l_ExpressionStr[1] - '0';

        l_Out |= l_Temp;
        l_Unpacked += l_Out;

        l_ExpressionStr += 2;
    }

    return l_Unpacked;
}

int32 WorldStateExpression_EvalPush(Player* p_Player, char const** p_UnpackedExpression, std::vector<std::string>& p_Instructions)
{
#define UNPACK_UINT8(x) { x = *((uint8*)((char const*)*p_UnpackedExpression)); *p_UnpackedExpression += sizeof(uint8);}
#define UNPACK_INT32(x) { x = *((int32*)((char const*)*p_UnpackedExpression)); *p_UnpackedExpression += sizeof(int32);}
    uint8 l_OpType;
    UNPACK_UINT8(l_OpType);

    if (l_OpType == WorldStateExpressionCustomOpType::PushUInt32)
    {
        int32 l_Value;
        UNPACK_INT32(l_Value);

#ifdef _MSC_VER
        p_Instructions.push_back("mov eax, " + std::to_string(l_Value));
#endif

        return l_Value;
    }
    else if (l_OpType == WorldStateExpressionCustomOpType::PushWorldStateValue)
    {
        int32 l_WorldStateID;
        UNPACK_INT32(l_WorldStateID);

#ifdef _MSC_VER
        p_Instructions.push_back("push " + std::to_string(l_WorldStateID));
        p_Instructions.push_back("call GetWorldStateValue");
#endif

        if (p_Player)
            return p_Player->GetWorldState(l_WorldStateID);
    }
    else if(l_OpType == WorldStateExpressionCustomOpType::CallFunction)
    {
        int32 l_FunctionID;
        UNPACK_INT32(l_FunctionID);

        int l_Arg1 = WorldStateExpression_EvalPush(p_Player, p_UnpackedExpression, p_Instructions);
        int l_Arg2 = WorldStateExpression_EvalPush(p_Player, p_UnpackedExpression, p_Instructions);

        if (l_FunctionID > (sizeof(g_WorldStateExpressionFunction) / sizeof(g_WorldStateExpressionFunction[0])))
            return (uint32)-1;

#ifdef _MSC_VER
        p_Instructions.push_back("push " + std::to_string(l_Arg1));
        p_Instructions.push_back("push " + std::to_string(l_Arg2));
        p_Instructions.push_back("call " + std::string(WorldStateExpressionFunctions::Names[l_FunctionID]));
#endif

        return g_WorldStateExpressionFunction[l_FunctionID](p_Player, l_Arg1, l_Arg2);
    }

    return (uint32)-1;
#undef UNPACK_INT32
#undef UNPACK_UINT8
}

int32 WorldStateExpression_EvalArithmetic(Player* p_Player, char const** p_UnpackedExpression, std::vector<std::string>& p_Instructions, bool p_ForA)
{
    UNUSED(p_ForA);

#define UNPACK_UINT8(x) { x = *(uint8_t*)(*p_UnpackedExpression); *p_UnpackedExpression += sizeof(uint8_t);}
    int l_LeftValue = WorldStateExpression_EvalPush(p_Player, p_UnpackedExpression, p_Instructions);
    char l_Opperand;
    UNPACK_UINT8(l_Opperand);

    if (!l_Opperand)
    {
#ifdef _MSC_VER
        p_Instructions.push_back(p_ForA ? "mov pA, eax" : "mov pB, eax");
#endif
        return l_LeftValue;
    }

#ifdef _MSC_VER
    p_Instructions.push_back("mov pC, eax");
#endif

    int l_RightValue = WorldStateExpression_EvalPush(p_Player, p_UnpackedExpression, p_Instructions);

    switch (l_Opperand)
    {
        case WorldStateExpressionMathOpcode::Add:
#ifdef _MSC_VER
            p_Instructions.push_back(p_ForA ? "mov pA, [pC + eax]" : "mov pB, [pC + eax]");
#endif
            return l_RightValue + l_LeftValue;
            break;
        case WorldStateExpressionMathOpcode::Substract:
#ifdef _MSC_VER
            p_Instructions.push_back(p_ForA ? "mov pA, [pC - eax]" : "mov pB, [pC - eax]");
#endif
            return l_LeftValue - l_RightValue;
            break;
        case WorldStateExpressionMathOpcode::Multiply:
#ifdef _MSC_VER
            p_Instructions.push_back(p_ForA ? "mov pA, [pC * eax]" : "mov pB, [pC * eax]");
#endif
            return l_LeftValue * l_RightValue;
            break;
        case WorldStateExpressionMathOpcode::Divide:
#ifdef _MSC_VER
            p_Instructions.push_back(p_ForA ? "mov pA, [pC / eax]" : "mov pB, [pC / eax]");
#endif
            if (!l_RightValue)
                return 0;

            return l_LeftValue / l_RightValue;
            break;
        case WorldStateExpressionMathOpcode::Modulo:
#ifdef _MSC_VER
            p_Instructions.push_back(p_ForA ? "mov pA, [pC % eax]" : "mov pB, [pC % eax]");
#endif
            if (!l_RightValue)
                return 0;

            return l_LeftValue % l_RightValue;
            break;
        default:
            break;
    }

    return 0;
#undef UNPACK_UINT8
}

bool WorldStateExpression_EvalCompare(Player* p_Player, char const** p_UnpackedExpression, std::vector<std::string>& p_Instructions)
{
#define UNPACK_UINT8(x) { x = *(uint8*)(*p_UnpackedExpression); *p_UnpackedExpression += sizeof(uint8);}
    int l_LeftValue = WorldStateExpression_EvalArithmetic(p_Player, p_UnpackedExpression, p_Instructions, true);
    char l_Opperand;
    UNPACK_UINT8(l_Opperand);

    if (!l_Opperand)
    {
#ifdef _MSC_VER
        p_Instructions.push_back("mov eax, pA");
#endif
        return !!l_LeftValue;
    }

    int l_RightValue = WorldStateExpression_EvalArithmetic(p_Player, p_UnpackedExpression, p_Instructions, false);

    switch (l_Opperand)
    {
        case WorldStateExpressionCompareOpcode::Equal:
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA == pB]");
#endif
            return l_LeftValue == l_RightValue;
        case WorldStateExpressionCompareOpcode::Different:
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA != pB]");
#endif
            return l_LeftValue != l_RightValue;
        case WorldStateExpressionCompareOpcode::Less:
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA < pB]");
#endif
            return l_LeftValue < l_RightValue;
        case WorldStateExpressionCompareOpcode::LessOrEqual:
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA <= pB]");
#endif
            return l_LeftValue <= l_RightValue;
        case WorldStateExpressionCompareOpcode::More:
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA > pB]");
#endif
            return l_LeftValue > l_RightValue;
        case WorldStateExpressionCompareOpcode::MoreOrEqual:
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA >= pB]");
#endif
            return l_LeftValue >= l_RightValue;
        default:
            break;
    }

    return false;
#undef UNPACK_UINT8
}

bool WorldStateExpression_EvalResult(char p_LogicResult, uint8_t p_EvalResultRoutineID, bool p_EvalResult, uint8_t p_PrevResultRoutineID, bool p_PrevResult, std::vector<std::string>& p_Instructions)
{
    bool l_Result = false;

    switch (p_LogicResult)
    {
        case WorldStateExpressionEvalResultLogic::FirstAndSecond:
            l_Result = p_EvalResult != 0 && p_PrevResult != 0;
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA && pB]");
#endif
            break;

        case WorldStateExpressionEvalResultLogic::FirstOrSecond:
            l_Result = (p_PrevResult || p_EvalResult) != 0;
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [pA || pB]");
#endif
            break;

        case WorldStateExpressionEvalResultLogic::NotFirstOrNotSecond:
#ifdef _MSC_VER
            p_Instructions.push_back("mov eax, [(pA || pB) ? (!pA || !pB) : 0]");
#endif
            if (p_PrevResult || p_EvalResult)
                l_Result = (p_PrevResult == 0 || p_PrevResult == 0);
            break;
    }

    return l_Result;
}

/// Eval a worldstate expression
bool WorldStateExpressionEntry::Eval(Player* p_Player, std::vector<std::string> * p_OutStrResult) const
{
#define UNPACK_UINT8(x) { x = *l_UnpackedExpression; l_UnpackedExpression += sizeof(uint8);}
    std::vector<std::string> p_Instructions;
    std::vector<std::string> p_InstructionsSecond;

    if (this->Expression && strlen(this->Expression))
    {
        std::string l_UnpackedExpressionString = UnpackWorldStateExpression(this->Expression);
        char const* l_UnpackedExpression = l_UnpackedExpressionString.c_str();

        char l_Value;
        UNPACK_UINT8(l_Value);

        if (l_Value == 1)
        {
#ifdef _MSC_VER
            p_Instructions.push_back("========= routine 1 =========");
#endif
            bool l_Result = WorldStateExpression_EvalCompare(p_Player, &l_UnpackedExpression, p_Instructions);
#ifdef _MSC_VER
            p_Instructions.push_back("=============================");
#endif
            char l_ResultLogic;
            UNPACK_UINT8(l_ResultLogic);

            if (l_ResultLogic)
            {
#ifdef _MSC_VER
                p_Instructions.push_back("========= routine 2 =========");
#endif
                bool l_Routine2 = WorldStateExpression_EvalCompare(p_Player, &l_UnpackedExpression, p_Instructions);
#ifdef _MSC_VER
                p_Instructions.push_back("=============================");

                p_InstructionsSecond.push_back("call Routine1");
                p_InstructionsSecond.push_back("mov pA, eax");
                p_InstructionsSecond.push_back("call Routine2");
                p_InstructionsSecond.push_back("mov pB, eax");
#endif
                l_Result = WorldStateExpression_EvalResult(l_ResultLogic, 2, l_Routine2, 1, l_Result, p_InstructionsSecond);
                UNPACK_UINT8(l_ResultLogic);

                if (l_ResultLogic)
                {
#ifdef _MSC_VER
                    p_Instructions.push_back("========= routine 3 =========");
#endif
                    bool l_Routine3 = WorldStateExpression_EvalCompare(p_Player, &l_UnpackedExpression, p_Instructions);
#ifdef _MSC_VER
                    p_Instructions.push_back("=============================");

                    p_InstructionsSecond.push_back("mov pA, eax");
                    p_InstructionsSecond.push_back("call Routine3");
                    p_InstructionsSecond.push_back("mov pB, eax");
#endif
                    l_Result = WorldStateExpression_EvalResult(l_ResultLogic, 3, l_Routine3, 2, l_Result, p_InstructionsSecond);
                    UNPACK_UINT8(l_ResultLogic);
                }
            }

            for (auto l_Line : p_InstructionsSecond)
                p_Instructions.push_back("$> " + l_Line);

            if (p_OutStrResult)
                *p_OutStrResult = p_Instructions;

            return l_Result;
        }
    }

    return false;
#undef UNPACK_UINT8
}

void DeterminaAlternateMapPosition(uint32 p_MapID, float p_X, float p_Y, float p_Z, uint32* p_NewMapID /*= nullptr*/, float* p_NewPosX /*= nullptr*/, float* p_NewPosY /*= nullptr*/)
{
    ASSERT(p_NewMapID || (p_NewPosX && p_NewPosY));

    WorldMapTransformsEntry const* l_Transformation = nullptr;

    for (uint32 l_I = 0; l_I < sWorldMapTransformsStore.GetNumRows(); ++l_I)
    {
        WorldMapTransformsEntry const* l_Transform = sWorldMapTransformsStore.LookupEntry(l_I);

        if (!l_Transform)
            continue;

        if (l_Transform->MapID != p_MapID)
            continue;

        if (l_Transform->RegionMinX > p_X || l_Transform->RegionMaxX < p_X)
            continue;
        if (l_Transform->RegionMinY > p_Y || l_Transform->RegionMaxY < p_Y)
            continue;
        if (l_Transform->RegionMinZ > p_Z || l_Transform->RegionMaxZ < p_Z)
            continue;

        l_Transformation = l_Transform;
        break;
    }

    if (!l_Transformation)
    {
        if (p_NewMapID)
            *p_NewMapID = p_MapID;

        if (p_NewPosX && p_NewPosY)
        {
            *p_NewPosX = p_X;
            *p_NewPosY = p_Y;
        }
        return;
    }

    if (p_NewMapID)
        *p_NewMapID = l_Transformation->NewMapID;

    if (!p_NewPosX || !p_NewPosY)
        return;

    if (l_Transformation->RegionScale > 0.0f && l_Transformation->RegionScale < 1.0f)
    {
        p_X = (p_X - l_Transformation->RegionMinX) * l_Transformation->RegionScale + l_Transformation->RegionMinX;
        p_Y = (p_Y - l_Transformation->RegionMinY) * l_Transformation->RegionScale + l_Transformation->RegionMinY;
    }

    *p_NewPosX = p_X + l_Transformation->RegionOffsetX;
    *p_NewPosY = p_Y + l_Transformation->RegionOffsetY;
}

void HotfixLfgDungeonsData()
{
    for (uint32 i = 0; i < sLFGDungeonStore.GetNumRows(); ++i)
    {
        if (auto l_Entry = const_cast<LFGDungeonEntry*>(sLFGDungeonStore.LookupEntry(i)))
        {
            if (l_Entry->isScenarioSingle())
            {
                /// Fix access to some scenarios
                if (l_Entry->maxlevel == 0)
                    l_Entry->maxlevel = 100;

                /// fix when single scenarios are included in random
                l_Entry->grouptype = 0;
            }
        }
    }
}
