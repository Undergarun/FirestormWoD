////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_DBCSTORES_H
#define TRINITY_DBCSTORES_H

#include "Common.h"
#include "DBCStore.h"
#include "DBCStructure.h"

typedef std::list<uint32> SimpleFactionsList;
SimpleFactionsList const* GetFactionTeamList(uint32 faction);

#ifndef CROSS
EmotesTextSoundEntry const* FindTextSoundEmoteFor(uint32 p_Emote, uint32 p_Race, uint32 p_Gender);

#endif /* not CROSS */
char const* GetPetName(uint32 petfamily, uint32 dbclang);
SpellEffectEntry const* GetSpellEffectEntry(uint32 spellId, uint32 effect, uint32 difficulty);
SpellEffectScalingEntry const* GetSpellEffectScalingEntry(uint32 effectId);

int32 GetAreaFlagByAreaID(uint32 area_id);                  // -1 if not found
AreaTableEntry const* GetAreaEntryByAreaID(uint32 area_id);
AreaTableEntry const* GetAreaEntryByAreaFlagAndMap(uint32 area_flag, uint32 map_id);
uint32 GetAreaFlagByMapId(uint32 mapid);

WMOAreaTableEntry const* GetWMOAreaTableEntryByTripple(int32 rootid, int32 adtid, int32 groupid);

uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId);

const std::string* GetRandomCharacterName(uint8 race, uint8 gender);

enum ContentLevels
{
    CONTENT_1_60    = 0,
    CONTENT_61_70   = 1,
    CONTENT_71_80   = 2,
    CONTENT_81_85   = 3,
    CONTENT_86_90   = 4,
    CONTENT_91_100  = 5,
    MAX_CONTENT
};

uint32 GetMaxLevelForExpansion(uint32 expansion);

ContentLevels GetContentLevelsForMapAndZone(uint32 mapid, uint32 zoneId);

bool IsTotemCategoryCompatiableWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId);

void Zone2MapCoordinates(float &x, float &y, uint32 zone);
void Map2ZoneCoordinates(float &x, float &y, uint32 zone);

typedef std::unordered_map<uint32, std::unordered_map<uint32, MapDifficulty>> MapDifficultyMap;
MapDifficulty const* GetDefaultMapDifficulty(uint32 p_MapID, Difficulty* p_Difficulty = nullptr);
MapDifficulty const* GetMapDifficultyData(uint32 p_MapID, Difficulty p_Difficulty);
MapDifficulty const* GetDownscaledMapDifficultyData(uint32 p_MapID, Difficulty& p_Difficulty);
void HotfixLfgDungeonsData();

uint32 GetLiquidFlags(uint32 liquidType);

typedef std::unordered_map<uint32, std::list<ItemSetSpellEntry const*> > ItemSetSpellsByItemID;
extern ItemSetSpellsByItemID sItemSetSpellsByItemIDStore;

extern DBCStorage <AnimKitEntry>                 sAnimKitStore;
extern DBCStorage <AreaTableEntry>               sAreaStore;// recommend access using functions
extern DBCStorage <AreaTriggerEntry>             sAreaTriggerStore;
extern DBCStorage <ArmorLocationEntry>           sArmorLocationStore;
extern DBCStorage <BankBagSlotPricesEntry>       sBankBagSlotPricesStore;
extern DBCStorage <BattlemasterListEntry>        sBattlemasterListStore;
extern DBCStorage <ChatChannelsEntry>            sChatChannelsStore;
extern DBCStorage <CharTitlesEntry>              sCharTitlesStore;
extern DBCStorage <ChrClassesEntry>              sChrClassesStore;
extern DBCStorage <ChrRacesEntry>                sChrRacesStore;
extern DBCStorage <ChrSpecializationsEntry>      sChrSpecializationsStore;
extern DBCStorage <CinematicCameraEntry>         sCinematicCameraStore;
extern DBCStorage <CreatureDisplayInfoExtraEntry> sCreatureDisplayInfoExtraStore;
extern DBCStorage <CreatureFamilyEntry>          sCreatureFamilyStore;
extern DBCStorage <CreatureModelDataEntry>       sCreatureModelDataStore;
extern DBCStorage <DifficultyEntry>              sDifficultyStore;
extern DBCStorage <DungeonEncounterEntry>        sDungeonEncounterStore;
extern DBCStorage <DurabilityCostsEntry>         sDurabilityCostsStore;
extern DBCStorage <EmotesEntry>                  sEmotesStore;
extern DBCStorage <EmotesTextEntry>              sEmotesTextStore;
extern DBCStorage <FactionEntry>                 sFactionStore;
extern DBCStorage <FactionTemplateEntry>         sFactionTemplateStore;
extern DBCStorage <FileDataEntry>                sFileDataStore;
extern DBCStorage <GameObjectDisplayInfoEntry>   sGameObjectDisplayInfoStore;
extern DBCStorage <GemPropertiesEntry>           sGemPropertiesStore;
extern DBCStorage <GlyphPropertiesEntry>         sGlyphPropertiesStore;

extern DBCStorage <gtArmorMitigationByLvlEntry>  sgtArmorMitigationByLvlStore;
extern DBCStorage <GtBarberShopCostBaseEntry>    sGtBarberShopCostBaseStore;
extern DBCStorage <GtCombatRatingsEntry>         sGtCombatRatingsStore;
extern DBCStorage <GtChanceToMeleeCritBaseEntry> sGtChanceToMeleeCritBaseStore;
extern DBCStorage <GtChanceToMeleeCritEntry>     sGtChanceToMeleeCritStore;
extern DBCStorage <GtChanceToSpellCritBaseEntry> sGtChanceToSpellCritBaseStore;
extern DBCStorage <GtChanceToSpellCritEntry>     sGtChanceToSpellCritStore;
extern DBCStorage <GtOCTLevelExperienceEntry>    sGtOCTLevelExperienceStore;
extern DBCStorage <GtOCTHpPerStaminaEntry>       sGtOCTHpPerStaminaStore;
extern DBCStorage <GtRegenMPPerSptEntry>         sGtRegenMPPerSptStore;
extern DBCStorage <GtSpellScalingEntry>          sGtSpellScalingStore;
extern DBCStorage <GtOCTBaseHPByClassEntry>      sGtOCTBaseHPByClassStore;
extern DBCStorage <GtOCTBaseMPByClassEntry>      sGtOCTBaseMPByClassStore;
extern DBCStorage <ItemSetSpellEntry>            sItemSetSpellStore;
extern DBCStorage <ItemArmorQualityEntry>        sItemArmorQualityStore;
extern DBCStorage <ItemArmorShieldEntry>         sItemArmorShieldStore;
extern DBCStorage <ItemArmorTotalEntry>          sItemArmorTotalStore;
extern DBCStorage <ItemBagFamilyEntry>           sItemBagFamilyStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageAmmoStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageOneHandStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageOneHandCasterStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageRangedStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageThrownStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageTwoHandStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageTwoHandCasterStore;
extern DBCStorage <ItemDamageEntry>              sItemDamageWandStore;
extern DBCStorage <gtItemSocketCostPerLevelEntry> sgtItemSocketCostPerLevelStore;
//extern DBCStorage <ItemDisplayInfoEntry>      sItemDisplayInfoStore; -- not used currently
extern DBCStorage <ItemSetEntry>                 sItemSetStore;
extern DBCStorage <LFGDungeonEntry>              sLFGDungeonStore;
extern DBCStorage <LiquidTypeEntry>              sLiquidTypeStore;
extern DBCStorage <LockEntry>                    sLockStore;
extern DBCStorage <MapEntry>                     sMapStore;
extern DBCStorage <MinorTalentEntry>             sMinorTalentStore;
extern DBCStorage <PhaseEntry>                   sPhaseStore;
//extern DBCStorage <MapDifficultyEntry>           sMapDifficultyStore; -- use GetMapDifficultyData insteed
extern MapDifficultyMap                          sMapDifficultyMap;
extern DBCStorage <MovieEntry>                   sMovieStore;
extern DBCStorage <PowerDisplayEntry>            sPowerDisplayStore;
extern DBCStorage <QuestFactionRewEntry>         sQuestFactionRewardStore;
extern DBCStorage <RandomPropertiesPointsEntry>  sRandomPropertiesPointsStore;
extern DBCStorage <ScenarioStepEntry>            sScenarioStepStore;
extern DBCStorage <SkillLineEntry>               sSkillLineStore;
extern DBCStorage <SkillLineAbilityEntry>        sSkillLineAbilityStore;
extern DBCStorage <SpellFocusObjectEntry>        sSpellFocusObjectStore;
extern DBCStorage <SpellItemEnchantmentEntry>    sSpellItemEnchantmentStore;
extern SpellSkillingList                         sSpellSkillingList;
extern PetFamilySpellsStore                      sPetFamilySpellsStore;
extern DBCStorage <SpellShapeshiftEntry>         sSpellShapeshiftStore;
extern DBCStorage <SpellShapeshiftFormEntry>     sSpellShapeshiftFormStore;
extern DBCStorage <SpellEntry>                   sSpellStore;
extern DBCStorage <SpellAuraOptionsEntry>        sSpellAuraOptionsStore;
extern DBCStorage <SpellCategoriesEntry>         sSpellCategoriesStore;
extern DBCStorage <SpellCategoryEntry>           sSpellCategoryStore;
extern DBCStorage <SpellCooldownsEntry>          sSpellCooldownsStore;
extern DBCStorage <SpellEffectEntry>             sSpellEffectStore;
extern DBCStorage <SpellEffectScalingEntry>      sSpellEffectScalingStore;
extern DBCStorage <SpellEquippedItemsEntry>      sSpellEquippedItemsStore;
extern DBCStorage <SpellInterruptsEntry>         sSpellInterruptsStore;
extern DBCStorage <SpellLevelsEntry>             sSpellLevelsStore;
extern DBCStorage <SpellScalingEntry>            sSpellScalingStore;
extern DBCStorage <SpellTargetRestrictionsEntry> sSpellTargetRestrictionsStore;
extern DBCStorage <SummonPropertiesEntry>        sSummonPropertiesStore;
extern DBCStorage <TalentEntry>                  sTalentStore;
extern DBCStorage <VehicleEntry>                 sVehicleStore;
extern DBCStorage <VehicleSeatEntry>             sVehicleSeatStore;
extern DBCStorage <WMOAreaTableEntry>            sWMOAreaTableStore;
extern DBCStorage <WorldMapAreaEntry>            sWorldMapAreaStore; //-- use Zone2MapCoordinates and Map2ZoneCoordinates
extern DBCStorage <World_PVP_AreaEntry>          sWorld_PVP_AreaStore;
extern DBCStorage <WorldSafeLocsEntry>           sWorldSafeLocsStore;
extern DBCStorage <GtBattlePetTypeDamageModEntry> sGtBattlePetTypeDamageModStore;
extern DBCStorage <GtBattlePetXPEntry>           sGtBattlePetXPStore;
extern DBCStorage <WorldStateEntry>              sWorldStateStore;
extern DBCStorage <WorldStateExpressionEntry>    sWorldStateExpressionStore;

void LoadDBCStores(const std::string& dataPath);

uint32 GetQuestUniqueBitFlag(uint32 questId);

void DeterminaAlternateMapPosition(uint32 p_MapID, float p_X, float p_Y, float p_Z, uint32* p_NewMapID = nullptr, float* p_NewPosX = nullptr, float* p_NewPosY = nullptr);

#endif
