////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SPELL_H
#define __SPELL_H

#include "GridDefines.h"
#include "SharedDefines.h"
#include "ObjectMgr.h"
#include "SpellInfo.h"
#include "PathGenerator.h"

class Unit;
class Player;
class GameObject;
class DynamicObject;
class WorldObject;
class Aura;
class SpellScript;
class ByteBuffer;

#define SPELL_CHANNEL_UPDATE_INTERVAL (1 * IN_MILLISECONDS)

enum SpellCastFlags
{
    CAST_FLAG_NONE              = 0x00000000,
    CAST_FLAG_PENDING           = 0x00000001,   ///< AOE combat log?
    CAST_FLAG_HAS_TRAJECTORY    = 0x00000002,
    CAST_FLAG_UNKNOWN_3         = 0x00000004,
    CAST_FLAG_UNKNOWN_4         = 0x00000008,   ///< ignore AOE visual
    CAST_FLAG_UNKNOWN_5         = 0x00000010,
    CAST_FLAG_PROJECTILE        = 0x00000020,
    CAST_FLAG_UNKNOWN_7         = 0x00000040,
    CAST_FLAG_UNKNOWN_8         = 0x00000080,
    CAST_FLAG_UNKNOWN_9         = 0x00000100,
    CAST_FLAG_UNKNOWN_10        = 0x00000200,
    CAST_FLAG_UNKNOWN_11        = 0x00000400,
    CAST_FLAG_POWER_LEFT_SELF   = 0x00000800,
    CAST_FLAG_UNKNOWN_13        = 0x00001000,
    CAST_FLAG_UNKNOWN_14        = 0x00002000,
    CAST_FLAG_UNKNOWN_15        = 0x00004000,
    CAST_FLAG_UNKNOWN_16        = 0x00008000,
    CAST_FLAG_UNKNOWN_17        = 0x00010000,
    CAST_FLAG_ADJUST_MISSILE    = 0x00020000,
    CAST_FLAG_NO_GCD            = 0x00040000,   ///< No GCD for spells casted by charm/summon (vehicle spells is an example)
    CAST_FLAG_VISUAL_CHAIN      = 0x00080000,
    CAST_FLAG_UNKNOWN_21        = 0x00100000,
    CAST_FLAG_RUNE_LIST         = 0x00200000,
    CAST_FLAG_UNKNOWN_23        = 0x00400000,
    CAST_FLAG_UNKNOWN_24        = 0x00800000,
    CAST_FLAG_UNKNOWN_25        = 0x01000000,
    CAST_FLAG_UNKNOWN_26        = 0x02000000,
    CAST_FLAG_IMMUNITY          = 0x04000000,
    CAST_FLAG_UNKNOWN_28        = 0x08000000,
    CAST_FLAG_UNKNOWN_29        = 0x10000000,
    CAST_FLAG_UNKNOWN_30        = 0x20000000,
    CAST_FLAG_HEAL_PREDICTION   = 0x40000000,
    CAST_FLAG_NO_COOLDOWN       = 0x80000000
};

/// Only on 18 bits yet
enum CastFlagsEx
{
    CAST_FLAG_EX_NONE         = 0x00000000,
    CAST_FLAG_EX_UNK_1        = 0x00000001,
    CAST_FLAG_EX_UNK_2        = 0x00000002,
    CAST_FLAG_EX_UNK_3        = 0x00000004,
    CAST_FLAG_EX_UNK_4        = 0x00000008,
    CAST_FLAG_EX_UNK_5        = 0x00000010,
    CAST_FLAG_EX_UNK_6        = 0x00000020,
    CAST_FLAG_EX_UNK_7        = 0x00000040,
    CAST_FLAG_EX_UNK_8        = 0x00000080,
    CAST_FLAG_EX_UNK_9        = 0x00000100,
    CAST_FLAG_EX_UNK_10       = 0x00000200,
    CAST_FLAG_EX_UNK_11       = 0x00000400,
    CAST_FLAG_EX_UNK_12       = 0x00000800,
    CAST_FLAG_EX_UNK_13       = 0x00001000,
    CAST_FLAG_EX_UNK_14       = 0x00002000,
    CAST_FLAG_EX_UNK_15       = 0x00004000,
    CAST_FLAG_EX_TOY_COOLDOWN = 0x00008000,
    CAST_FLAG_EX_UNK_17       = 0x00010000,
    CAST_FLAG_EX_UNK_18       = 0x00020000
};

enum PredictionType
{
    PredictionHealth    = -2,
    PredictionPower     = 2,
    PredictionRunes     = 5
};

enum SpellRangeFlag
{
    SPELL_RANGE_DEFAULT = 0,
    SPELL_RANGE_MELEE   = 1,     ///< Melee
    SPELL_RANGE_RANGED  = 2      ///< Hunter range and ranged weapon
};

struct SpellDestination
{
    SpellDestination();
    SpellDestination(float x, float y, float z, float orientation = 0.0f, uint32 mapId = MAPID_INVALID);
    SpellDestination(Position const& pos);
    SpellDestination(WorldObject const& wObj);

    WorldLocation _position;
    uint64 _transportGUID;
    Position _transportOffset;
};

struct SpellLog_EnergyzeHelper
{
    uint32 Value;
    float Multiplier;
    uint8 PowerType;
    uint64 Guid;
};

struct SpellLog_ExtraAttack
{
    uint64 Victim;
    uint32 NumAttacks;
};

struct SpellLog_DurabilityDamage
{
    uint64 Victim;
    uint32 ItemID;
    uint32 Amount;
};

struct SpellLogHelper
{
    std::list<uint64>                       Targets;
    std::list<SpellLog_EnergyzeHelper>      Energizes;
    std::list<uint32>                       CreatedItems;
    std::list<SpellLog_DurabilityDamage>    DurabilityDamages;
    std::list<SpellLog_ExtraAttack>         ExtraAttacks;
    
    SpellLogHelper()
    {
        Targets.clear();
        Energizes.clear();
        CreatedItems.clear();
        DurabilityDamages.clear();
        ExtraAttacks.clear();
    }

    void AddTarget(ObjectGuid guid)
    {
        Targets.push_back(guid);
    }

    void AddCreatedItem(uint32 id)
    {
        CreatedItems.push_back(id);
    }

    void AddEnergize(SpellLog_EnergyzeHelper energize)
    {
        Energizes.push_back(energize);
    }

    void AddEnergize(uint64 guid, float mult, uint32 val, uint8 type)
    {
        SpellLog_EnergyzeHelper helper;
        helper.Value        = val;
        helper.PowerType    = type;
        helper.Guid         = guid;
        helper.Multiplier   = mult;

        AddEnergize(helper);
    }

    void AddDurabilityDamage(uint64 p_Victim, uint32 p_ItemID, uint32 p_Amount)
    {
        SpellLog_DurabilityDamage l_Data;
        l_Data.Victim = p_Victim;
        l_Data.ItemID = p_ItemID;
        l_Data.Amount = p_Amount;

        DurabilityDamages.push_back(l_Data);
    }

    void AddExtraAttack(uint64 p_Victim, uint32 p_NumAttacks)
    {
        SpellLog_ExtraAttack l_Data;
        l_Data.Victim       = p_Victim;
        l_Data.NumAttacks   = p_NumAttacks;

        ExtraAttacks.push_back(l_Data);
    }
};

class SpellCastTargets
{
public:
    SpellCastTargets();
    ~SpellCastTargets();

    void Read(ByteBuffer& data, Unit* caster);
    void Write(ByteBuffer& data);

    void Initialize(uint32 flags, uint64 target, uint64 itemTarget, uint64 dest, WorldLocation destPos, uint64 src, WorldLocation srcPos)
    {
        m_targetMask = flags;
        m_objectTargetGUID = target;
        m_itemTargetGUID = itemTarget;

        // dest
        m_dst._transportGUID = dest;
        m_dst._position = destPos;
        if (dest)
            m_dst._transportOffset = destPos;

        // src
        m_src._transportGUID = src;
        m_src._position = srcPos;
        if (src)
            m_src._transportOffset = srcPos;
    }

    uint32 GetTargetMask() const { return m_targetMask; }
    void SetTargetMask(uint32 newMask) { m_targetMask = newMask; }

    void SetTargetFlag(SpellCastTargetFlags flag) { m_targetMask |= flag; }

    uint64 GetUnitTargetGUID() const;
    Unit* GetUnitTarget() const;
    void SetUnitTarget(Unit* target);

    uint64 GetGOTargetGUID() const;
    GameObject* GetGOTarget() const;
    void SetGOTarget(GameObject* target);

    uint64 GetCorpseTargetGUID() const;
    Corpse* GetCorpseTarget() const;

    WorldObject* GetObjectTarget() const;
    uint64 GetObjectTargetGUID() const;
    void RemoveObjectTarget();

    uint64 GetItemTargetGUID() const { return m_itemTargetGUID; }
    Item* GetItemTarget() const { return m_itemTarget; }
    uint32 GetItemTargetEntry() const { return m_itemTargetEntry; }
    void SetItemTarget(Item* item);
    void SetTradeItemTarget(Player* caster);
    void UpdateTradeSlotItem();

    SpellDestination const* GetSrc() const;
    Position const* GetSrcPos() const;
    void SetSrc(float x, float y, float z);
    void SetSrc(Position const& pos);
    void SetSrc(WorldObject const& wObj);
    void ModSrc(Position const& pos);
    void RemoveSrc();

    SpellDestination const* GetDst() const;
    WorldLocation const* GetDstPos() const;
    void SetDst(float x, float y, float z, float orientation, uint32 mapId = MAPID_INVALID);
    void SetDst(Position const& pos);
    void SetDst(WorldObject const& wObj);
    void SetDst(SpellCastTargets const& spellTargets);
    void ModDst(Position const& pos);
    void RemoveDst();

    bool HasSrc() const { return GetTargetMask() & TARGET_FLAG_SOURCE_LOCATION; }
    bool HasDst() const { return GetTargetMask() & TARGET_FLAG_DEST_LOCATION; }
    bool HasTraj() const { return m_speed != 0; }

    float GetElevation() const { return m_elevation; }
    void SetElevation(float elevation) { m_elevation = elevation; }
    float GetSpeed() const { return m_speed; }
    void SetSpeed(float speed) { m_speed = speed; }

    float GetDist2d() const { return m_src._position.GetExactDist2d(&m_dst._position); }
    float GetSpeedXY() const { return m_speed * std::cos(m_elevation); }
    float GetSpeedZ() const { return m_speed * std::sin(m_elevation); }

    void Update(Unit* caster);
    void OutDebug() const;

    void ClearExtraTargets()
    {
        m_extraTargets.clear();
        m_targetMask &= ~TARGET_FLAG_EXTRA_TARGETS;
    }

    void AddExtraTarget(uint64 guid, WorldLocation pos)
    {
        SpellDestination extraTarget;
        extraTarget._position = pos;
        extraTarget._transportGUID = guid;

        m_extraTargets.push_back(extraTarget);

        m_targetMask |= TARGET_FLAG_EXTRA_TARGETS;
    }

    size_t GetExtraTargetsCount() const { return m_extraTargets.size(); }
    std::list<SpellDestination> GetExtraTargets() const { return m_extraTargets; }

private:
    uint32 m_targetMask;

    // objects (can be used at spell creating and after Update at casting)
    WorldObject* m_objectTarget;
    Item* m_itemTarget;

    // object GUID/etc, can be used always
    uint64 m_objectTargetGUID;
    uint64 m_itemTargetGUID;
    uint32 m_itemTargetEntry;

    SpellDestination m_src;
    SpellDestination m_dst;
    std::list<SpellDestination> m_extraTargets;

    float m_elevation, m_speed;
    std::string m_strTarget;
};

struct SpellValue
{
    explicit  SpellValue(SpellInfo const* proto);
    int32     EffectBasePoints[SpellEffIndex::MAX_EFFECTS];
    uint32    MaxAffectedTargets;
    float     RadiusMod;
    uint8     AuraStackAmount;
};

enum SpellState
{
    SPELL_STATE_NULL        = 0,
    SPELL_STATE_PREPARING   = 1,
    SPELL_STATE_CASTING     = 2,
    SPELL_STATE_FINISHED    = 3,
    SPELL_STATE_IDLE        = 4,
    SPELL_STATE_DELAYED     = 5
};

enum SpellEffectHandleMode
{
    SPELL_EFFECT_HANDLE_LAUNCH,
    SPELL_EFFECT_HANDLE_LAUNCH_TARGET,
    SPELL_EFFECT_HANDLE_HIT,
    SPELL_EFFECT_HANDLE_HIT_TARGET
};

#define MAX_POWERS_COST MAX_POWERS + 1
#define POWER_TO_INDEX(i) (i & 0xF000000 ? MAX_POWERS_COST - 1 : i)
#define INDEX_TO_POWER(i) (i == MAX_POWERS_COST - 1 ? POWER_HEALTH : i)

class Spell
{
    friend void Unit::SetCurrentCastedSpell(Spell* pSpell);
    friend class SpellScript;
public:

    void EffectNULL(SpellEffIndex effIndex);
    void EffectUnused(SpellEffIndex effIndex);
    void EffectDistract(SpellEffIndex effIndex);
    void EffectPull(SpellEffIndex effIndex);
    void EffectSchoolDMG(SpellEffIndex effIndex);
    void EffectEnvironmentalDMG(SpellEffIndex effIndex);
    void EffectInstaKill(SpellEffIndex effIndex);
    void EffectDummy(SpellEffIndex effIndex);
    void EffectTeleportUnits(SpellEffIndex effIndex);
    void EffectApplyAura(SpellEffIndex effIndex);
    void EffectSendEvent(SpellEffIndex effIndex);
    void EffectPowerBurn(SpellEffIndex effIndex);
    void EffectPowerDrain(SpellEffIndex effIndex);
    void EffectHeal(SpellEffIndex effIndex);
    void EffectBind(SpellEffIndex effIndex);
    void EffectHealthLeech(SpellEffIndex effIndex);
    void EffectQuestComplete(SpellEffIndex effIndex);
    void EffectCreateItem(SpellEffIndex effIndex);
    void EffectCreateItem2(SpellEffIndex effIndex);
    void EffectCreateRandomItem(SpellEffIndex effIndex);
    void EffectPersistentAA(SpellEffIndex effIndex);
    void EffectEnergize(SpellEffIndex effIndex);
    void EffectOpenLock(SpellEffIndex effIndex);
    void EffectSummonChangeItem(SpellEffIndex effIndex);
    void EffectProficiency(SpellEffIndex effIndex);
    void EffectApplyAreaAura(SpellEffIndex effIndex);
    void EffectSummonType(SpellEffIndex effIndex);
    void EffectLearnSpell(SpellEffIndex effIndex);
    void EffectDispel(SpellEffIndex effIndex);
    void EffectDualWield(SpellEffIndex effIndex);
    void EffectPickPocket(SpellEffIndex effIndex);
    void EffectAddFarsight(SpellEffIndex effIndex);
    void EffectUntrainTalents(SpellEffIndex effIndex);
    void EffectHealMechanical(SpellEffIndex effIndex);
    void EffectJump(SpellEffIndex effIndex);
    void EffectJumpDest(SpellEffIndex effIndex);
    void EffectDeathGrip(SpellEffIndex effIndex);
    void EffectLeapBack(SpellEffIndex effIndex);
    void EffectQuestClear(SpellEffIndex effIndex);
    void EffectTeleUnitsFaceCaster(SpellEffIndex effIndex);
    void EffectLearnSkill(SpellEffIndex effIndex);
    void EffectPlayMovie(SpellEffIndex effIndex);
    void EffectTradeSkill(SpellEffIndex effIndex);
    void EffectEnchantItemPerm(SpellEffIndex effIndex);
    void EffectEnchantItemTmp(SpellEffIndex effIndex);
    void EffectTameCreature(SpellEffIndex effIndex);
    void EffectSummonPet(SpellEffIndex effIndex);
    void EffectLearnPetSpell(SpellEffIndex effIndex);
    void EffectWeaponDmg(SpellEffIndex effIndex);
    void EffectForceCast(SpellEffIndex effIndex);
    void EffectTriggerSpell(SpellEffIndex effIndex);
    void EffectTriggerMissileSpell(SpellEffIndex effIndex);
    void EffectThreat(SpellEffIndex effIndex);
    void EffectHealMaxHealth(SpellEffIndex effIndex);
    void EffectInterruptCast(SpellEffIndex effIndex);
    void EffectSummonObjectWild(SpellEffIndex effIndex);
    void EffectScriptEffect(SpellEffIndex effIndex);
    void EffectSanctuary(SpellEffIndex effIndex);
    void EffectAddComboPoints(SpellEffIndex effIndex);
    void EffectDuel(SpellEffIndex effIndex);
    void EffectStuck(SpellEffIndex effIndex);
    void EffectSummonPlayer(SpellEffIndex effIndex);
    void EffectActivateObject(SpellEffIndex effIndex);
    void EffectApplyGlyph(SpellEffIndex effIndex);
    void EffectEnchantHeldItem(SpellEffIndex effIndex);
    void EffectSummonObject(SpellEffIndex effIndex);
    void EffectResurrect(SpellEffIndex effIndex);
    void EffectParry(SpellEffIndex effIndex);
    void EffectBlock(SpellEffIndex effIndex);
    void EffectLeap(SpellEffIndex effIndex);
    void EffectTransmitted(SpellEffIndex effIndex);
    void EffectDisEnchant(SpellEffIndex effIndex);
    void EffectInebriate(SpellEffIndex effIndex);
    void EffectFeedPet(SpellEffIndex effIndex);
    void EffectDismissPet(SpellEffIndex effIndex);
    void EffectReputation(SpellEffIndex effIndex);
    void EffectForceDeselect(SpellEffIndex effIndex);
    void EffectSelfResurrect(SpellEffIndex effIndex);
    void EffectSkinning(SpellEffIndex effIndex);
    void EffectCharge(SpellEffIndex effIndex);
    void EffectChargeDest(SpellEffIndex effIndex);
    void EffectProspecting(SpellEffIndex effIndex);
    void EffectMilling(SpellEffIndex effIndex);
    void EffectRenamePet(SpellEffIndex effIndex);
    void EffectSendTaxi(SpellEffIndex effIndex);
    void EffectSummonCritter(SpellEffIndex effIndex);
    void EffectKnockBack(SpellEffIndex effIndex);
    void EffectPullTowards(SpellEffIndex effIndex);
    void EffectDispelMechanic(SpellEffIndex effIndex);
    void EffectSummonDeadPet(SpellEffIndex effIndex);
    void EffectDestroyAllTotems(SpellEffIndex effIndex);
    void EffectDurabilityDamage(SpellEffIndex effIndex);
    void EffectSkill(SpellEffIndex effIndex);
    void EffectTaunt(SpellEffIndex effIndex);
    void EffectDurabilityDamagePCT(SpellEffIndex effIndex);
    void EffectModifyThreatPercent(SpellEffIndex effIndex);
    void EffectResurrectNew(SpellEffIndex effIndex);
    void EffectAddExtraAttacks(SpellEffIndex effIndex);
    void EffectSpiritHeal(SpellEffIndex effIndex);
    void EffectSkinPlayerCorpse(SpellEffIndex effIndex);
    void EffectStealBeneficialBuff(SpellEffIndex effIndex);
    void EffectUnlearnSpecialization(SpellEffIndex effIndex);
    void EffectHealPct(SpellEffIndex effIndex);
    void EffectEnergizePct(SpellEffIndex effIndex);
    void EffectTriggerRitualOfSummoning(SpellEffIndex effIndex);
    void EffectSummonRaFFriend(SpellEffIndex effIndex);
    void EffectUnlockGuildVaultTab(SpellEffIndex effIndex);
    void EffectKillCreditPersonal(SpellEffIndex effIndex);
    void EffectKillCredit(SpellEffIndex effIndex);
    void EffectQuestFail(SpellEffIndex effIndex);
    void EffectQuestStart(SpellEffIndex effIndex);
    void EffectRedirectThreat(SpellEffIndex effIndex);
    void EffectGameObjectDamage(SpellEffIndex effIndex);
    void EffectGameObjectRepair(SpellEffIndex effIndex);
    void EffectGameObjectSetDestructionState(SpellEffIndex effIndex);
    void EffectActivateRune(SpellEffIndex effIndex);
    void EffectCreateTamedPet(SpellEffIndex effIndex);
    void EffectDiscoverTaxi(SpellEffIndex effIndex);
    void EffectTitanGrip(SpellEffIndex effIndex);
    void EffectEnchantItemPrismatic(SpellEffIndex effIndex);
    void EffectPlayMusic(SpellEffIndex effIndex);
    void EffectSpecCount(SpellEffIndex effIndex);
    void EffectActivateSpec(SpellEffIndex effIndex);
    void EffectPlaySound(SpellEffIndex effIndex);
    void EffectRemoveAura(SpellEffIndex effIndex);
    void EffectDamageFromMaxHealthPCT(SpellEffIndex effIndex);
    void EffectCastButtons(SpellEffIndex effIndex);
    void EffectRechargeManaGem(SpellEffIndex effIndex);
    void EffectGiveCurrency(SpellEffIndex effIndex);
    void EffectDestroyItem(SpellEffIndex effIndex);
    void EffectUnlearnTalent(SpellEffIndex effIndex);
    void EffectCreateAreatrigger(SpellEffIndex effIndex);
    void EffectResurrectWithAura(SpellEffIndex effIndex);
    void EffectLootBonus(SpellEffIndex p_EffIndex);
    void EffectTeleportToDigsite(SpellEffIndex effIndex);
    void EffectPlaySceneObject(SpellEffIndex effIndex);
    void EffectLearnBluePrint(SpellEffIndex p_EffIndex);
    void EffectGarrisonFinalize(SpellEffIndex p_EffIndex);
    void EffectIncreaseSkill(SpellEffIndex p_EffIndex);
    void EffectCreateGarrison(SpellEffIndex p_EffIndex);
    void EffectObtainFollower(SpellEffIndex p_EffIndex);
    void EffectUpgradeFolloweriLvl(SpellEffIndex p_EffIndex);
    void EffectRerollFollowerAbilities(SpellEffIndex p_EffIndex);
    void EffectGiveExperience(SpellEffIndex p_EffIndex);
    void EffectResurectPetBattles(SpellEffIndex effIndex);
    void EffectUncagePetBattle(SpellEffIndex effIndex);
    void EffectCanPetBattle(SpellEffIndex effIndex);
    void EffectThreatAll(SpellEffIndex p_EffIndex);
    void EffectForcePlayerInteraction(SpellEffIndex p_EffIndex);
    void EffectBecomeUntargettable(SpellEffIndex p_EffIndex);
    void EffectDespawnAreaTrigger(SpellEffIndex p_EffIndex);
    void EffectRandomizeArchaeologyDigsites(SpellEffIndex p_EffIndex);
    void EffectStampede(SpellEffIndex p_EffIndex);
    void EffectCreateHeirloom(SpellEffIndex p_EffIndex);
    void EffectApplyEnchantIllusion(SpellEffIndex p_EffIndex);
    void EffectLearnFollowerAbility(SpellEffIndex p_EffIndex);
    void EffectUpgradeHeirloom(SpellEffIndex p_EffIndex);
    void EffectFinishGarrisonMission(SpellEffIndex p_EffIndex);
    void EffectChangeItemBonus(SpellEffIndex p_EffIndex);
    void EffectFinishGarrisonShipment(SpellEffIndex p_EffIndex);

    typedef std::set<Aura*> UsedSpellMods;

    Spell(Unit* caster, SpellInfo const* info, TriggerCastFlags triggerFlags, uint64 originalCasterGUID = 0, bool skipCheck = false);
    ~Spell();

    void InitExplicitTargets(SpellCastTargets const& targets);
    void SelectExplicitTargets();

    void SelectSpellTargets();
    void SelectEffectImplicitTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32& processedEffectMask);
    void SelectImplicitChannelTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType);
    void SelectImplicitNearbyTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32 effMask);
    void SelectImplicitConeTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32 effMask);
    void SelectImplicitAreaTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, uint32 effMask);
    void SelectImplicitCasterDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType);
    void SelectImplicitTargetDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType);
    void SelectImplicitDestDestTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType);
    void SelectImplicitCasterObjectTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType);
    void SelectImplicitTargetObjectTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType);
    void SelectImplicitChainTargets(SpellEffIndex effIndex, SpellImplicitTargetInfo const& targetType, WorldObject* target, uint32 effMask);
    void SelectImplicitTrajTargets();

    void SelectEffectTypeImplicitTargets(uint8 effIndex);

    uint32 GetSearcherTypeMask(SpellTargetObjectTypes objType, ConditionContainer* condList);
    template<class SEARCHER> void SearchTargets(SEARCHER& searcher, uint32 containerMask, Unit* referer, Position const* pos, float radius);

    WorldObject* SearchNearbyTarget(float range, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectionType, ConditionContainer* condList = NULL);
    void SearchAreaTargets(std::list<WorldObject*>& targets, float range, Position const* position, Unit* referer, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectionType, ConditionContainer* condList);
    void SearchChainTargets(std::list<WorldObject*>& targets, uint32 chainTargets, WorldObject* target, SpellTargetObjectTypes objectType, SpellTargetCheckTypes selectType, ConditionContainer* condList, bool isChainHeal);

    void prepare(SpellCastTargets const* targets, AuraEffect const* triggeredByAura = nullptr);
    void cancel();
    void update(uint32 difftime);
    void cast(bool skipCheck = false);
    void finish(bool ok = true);
    void TakePower();
    void TakeAmmo();

    void TakeRunePower(bool didHit);
    void TakeReagents();
    void TakeCastItem();

    SpellCastResult CheckCast(bool strict);
    SpellCastResult CheckPetCast(Unit* target);

    bool CheckInterrupt();

    // handlers
    void handle_immediate();
    uint64 handle_delayed(uint64 t_offset);
    // handler helpers
    void _handle_immediate_phase();
    void _handle_finish_phase();

    SpellCastResult CheckItems();
    SpellCastResult CheckRange(bool strict);
    SpellCastResult CheckPower();
    SpellCastResult CheckRuneCost(uint32 runeCostID);
    SpellCastResult CheckCasterAuras() const;
    SpellCastResult CheckArenaAndRatedBattlegroundCastRules(Battleground const* p_Battleground);

    int32 CalculateDamage(uint8 i, Unit const* target, bool p_Log = false) const
    {
        Unit* l_Caster = m_originalCaster ? m_originalCaster : m_caster;
        return l_Caster->CalculateSpellDamage(target, m_spellInfo, i, &m_spellValue->EffectBasePoints[i], m_castItemLevel, p_Log);
    }

    bool HaveTargetsForEffect(uint8 effect) const;
    void Delayed();
    void DelayedChannel();
    uint32 getState() const { return m_spellState; }
    void setState(uint32 state) { m_spellState = state; }

    void setTriggerCastFlags(TriggerCastFlags p_Flags) { _triggeredCastFlags = p_Flags; }
    TriggerCastFlags getTriggerCastFlags() const { return _triggeredCastFlags; }

    void DoCreateItem(uint32 i, uint32 itemtype, bool vellum = false);

    bool CheckEffectTarget(Unit const* target, uint32 eff) const;
    bool CanAutoCast(Unit* target);
    void CheckSrc() { if (!m_targets.HasSrc()) m_targets.SetSrc(*m_caster); }
    void CheckDst() { if (!m_targets.HasDst()) m_targets.SetDst(*m_caster); }
    bool LOSAdditionalRules(Unit const* target, int8 eff = -1) const;

    static void SendCastResult(Player* caster, SpellInfo const* spellInfo, uint8 cast_count, SpellCastResult result, SpellCustomErrors customError = SPELL_CUSTOM_ERROR_NONE);
    void SendCastResult(SpellCastResult result);
    void SendSpellStart();
    void SendSpellGo();
    void SendSpellCooldown();
    void SendLogExecute();
    void ExecuteLogEffectTakeTargetPower(uint8 effIndex, Unit* target, uint32 powerType, uint32 powerTaken, float gainMultiplier);
    void ExecuteLogEffectExtraAttacks(uint8 effIndex, Unit* victim, uint32 attCount);
    void ExecuteLogEffectInterruptCast(uint8 effIndex, Unit* victim, uint32 spellId);
    void ExecuteLogEffectDurabilityDamage(uint8 effIndex, Unit* victim, uint32 p_ItemID, uint32 damage);
    void ExecuteLogEffectOpenLock(uint8 effIndex, Object* obj);
    void ExecuteLogEffectCreateItem(uint8 effIndex, uint32 entry);
    void ExecuteLogEffectDestroyItem(uint8 effIndex, uint32 entry);
    void ExecuteLogEffectSummonObject(uint8 effIndex, WorldObject* obj);
    void ExecuteLogEffectUnsummonObject(uint8 effIndex, WorldObject* obj);
    void ExecuteLogEffectResurrect(uint8 effIndex, Unit* target);
    void SendInterrupted(uint8 result);
    void SendChannelUpdate(uint32 time);
    void SendChannelStart(uint32 duration);
    void SendResurrectRequest(Player* p_Target);

    void HandleHolyPower(Player* caster);
    void HandleEffects(Unit* p_UnitTarget, Item* p_ItemTarget, GameObject* p_GOTarget, AreaTrigger* p_AreaTrigger, uint32 i, SpellEffectHandleMode mode);
    void HandleThreatSpells();

    SpellInfo const* const m_spellInfo;
    Item* m_CastItem;
    uint64 m_castItemGUID;
    int32 m_castItemLevel;
    uint8 m_cast_count;
    uint32 m_CastItemEntry;
    uint32 m_Misc[2];
    uint32 m_preCastSpell;
    SpellCastTargets m_targets;
    int8 m_comboPointGain;
    SpellCustomErrors m_customError;
    bool isStolen;

    UsedSpellMods m_appliedMods;

    int32 GetCastTime() const { return m_casttime; }
    bool IsAutoRepeat() const { return m_autoRepeat; }
    void SetAutoRepeat(bool rep) { m_autoRepeat = rep; }
    void ReSetTimer() { m_timer = m_casttime > 0 ? m_casttime : 0; }
    int32 GetTimer() const { return m_timer; }
    bool IsNextMeleeSwingSpell() const;
    bool IsTriggered() const { return _triggeredCastFlags & TRIGGERED_FULL_MASK; };
    bool IsChannelActive() const { return m_caster->GetUInt32Value(UNIT_FIELD_CHANNEL_SPELL) != 0; }
    bool IsAutoActionResetSpell() const;
    bool IsCritForTarget(Unit* target) const;

    bool IsDeletable() const { return !m_referencedFromCurrentSpell && !m_executedCurrently; }
    void SetReferencedFromCurrent(bool yes) { m_referencedFromCurrentSpell = yes; }
    bool IsInterruptable() const { return !m_executedCurrently; }
    void SetExecutedCurrently(bool yes) { m_executedCurrently = yes; }
    uint64 GetDelayStart() const { return m_delayStart; }
    void SetDelayStart(uint64 m_time) { m_delayStart = m_time; }
    uint64 GetDelayMoment() const { return m_delayMoment; }

    float GetCustomCritChance() const { return m_CustomCritChance; }
    void SetCustomCritChance(float p_CustomCritChance) { m_CustomCritChance = p_CustomCritChance; }

    bool IsNeedSendToClient() const;

    CurrentSpellTypes GetCurrentContainer() const;

    Unit* GetCaster() const { return m_caster; }
    Unit* GetOriginalCaster() const { return m_originalCaster; }
    SpellInfo const* GetSpellInfo() const { return m_spellInfo; }
    int32 GetPowerCost(Powers power) const { return m_powerCost[POWER_TO_INDEX(power)]; }

    void UpdatePointers();                              // must be used at call Spell code after time delay (non triggered spell cast/update spell call/etc)

    void CleanupTargetList();

    void SetSpellValue(SpellValueMod mod, int32 value);
    int32 GetSpellValue(SpellValueMod p_Mod) const;

    Unit* GetUnitTarget() { return m_targets.GetUnitTarget() ? m_targets.GetUnitTarget() : unitTarget; }

    void SetPeriodicDamageModifier(float newModifier) { m_periodicDamageModifier = newModifier; }

    WorldLocation* GetDestTarget() const { return destTarget; }
    uint32 GetUnitTargetCount() const { return m_UniqueTargetInfo.size(); }

    void SetDamage(uint32 p_Damage) { damage = p_Damage; }
    uint32 GetDamage() const { return damage; }

protected:
    bool HasGlobalCooldown() const;
    void TriggerGlobalCooldown();
    void CancelGlobalCooldown();
    bool IsDarkSimulacrum() const;
    bool IsCommandDemonSpell() const;
    bool IsMorePowerfulAura(Unit const* target) const;
    bool IsSpellTriggeredAfterCast() const;

    void SendLoot(uint64 guid, LootType loottype);

    Unit* const m_caster;

    SpellValue * const m_spellValue;

    uint64 m_originalCasterGUID;                        // real source of cast (aura caster/etc), used for spell targets selection
    // e.g. damage around area spell trigered by victim aura and damage enemies of aura caster
    Unit* m_originalCaster;                             // cached pointer for m_originalCaster, updated at Spell::UpdatePointers()

    Spell** m_selfContainer;                            // pointer to our spell container (if applicable)

    //Spell data
    SpellSchoolMask m_spellSchoolMask;                  // Spell school (can be overwrite for some spells (wand shoot for example)
    WeaponAttackType m_attackType;                      // For weapon based attack
    int32 m_powerCost[MAX_POWERS_COST];                 // Calculated spell cost per power initialized only in Spell::prepare
    int32 m_casttime;                                   // Calculated spell cast time initialized only in Spell::prepare
    int32 m_channeledDuration;                          // Calculated channeled spell duration in order to calculate correct pushback.
    bool m_canReflect;                                  // can reflect this spell?
    bool m_autoRepeat;
    uint8 m_runesState;

    uint8 m_delayAtDamageCount;
    bool isDelayableNoMore()
    {
        if (m_delayAtDamageCount >= 2)
            return true;

        m_delayAtDamageCount++;
        return false;
    }

    // Delayed spells system
    uint64 m_delayStart;                                // time of spell delay start, filled by event handler, zero = just started
    uint64 m_delayMoment;                               // moment of next delay call, used internally
    bool m_immediateHandled;                            // were immediate actions handled? (used by delayed spells only)

    // These vars are used in both delayed spell system and modified immediate spell system
    bool m_referencedFromCurrentSpell;                  // mark as references to prevent deleted and access by dead pointers
    bool m_executedCurrently;                           // mark as executed to prevent deleted and access by dead pointers
    bool m_needComboPoints;
    uint32 m_applyMultiplierMask;
    float m_damageMultipliers[32];
    float m_periodicDamageModifier;

    // Current targets, to be used in SpellEffects (MUST BE USED ONLY IN SPELL EFFECTS)
    Unit* unitTarget;
    Item* itemTarget;
    GameObject* gameObjTarget;
    AreaTrigger* m_AreaTrigger;
    WorldLocation* destTarget;
    int32 damage;
    SpellEffectHandleMode effectHandleMode;
    // used in effects handlers
    Aura* m_spellAura;

    // this is set in Spell Hit, but used in Apply Aura handler
    DiminishingLevels m_diminishLevel;
    DiminishingGroup m_diminishGroup;

    // -------------------------------------------
    GameObject* focusObject;

    // Damage and healing in effects need just calculate
    int32 m_damage;           // Damge   in effects count here
    int32 m_healing;          // Healing in effects count here
    int32 m_final_damage;     // Final damage in effects count here
    int32 m_absorbed_damage;   // Final absorbed damage in effects count here

    // ******************************************
    // Spell trigger system
    // ******************************************
    uint32 m_procAttacker;                // Attacker trigger flags
    uint32 m_procVictim;                  // Victim   trigger flags
    uint32 m_procEx;
    void   prepareDataForTriggerSystem(AuraEffect const* triggeredByAura);

    // ******************************************
    // Spell custom values
    // ******************************************
    float m_CustomCritChance;

    // *****************************************
    // Spell target subsystem
    // *****************************************
    // Targets store structures and data
    struct TargetInfo
    {
        uint64 targetGUID;
        uint64 timeDelay;
        SpellMissInfo missCondition : 8;
        SpellMissInfo reflectResult : 8;
        uint32  effectMask : 32;
        bool   processed : 1;
        bool   alive : 1;
        bool   crit : 1;
        bool   scaleAura : 1;
        int32  damage;
    };
    std::list<TargetInfo> m_UniqueTargetInfo;
    uint32 m_channelTargetEffectMask;                        // Mask req. alive targets

    struct GOTargetInfo
    {
        uint64 targetGUID;
        uint64 timeDelay;
        uint32  effectMask : 32;
        bool   processed : 1;
    };
    std::list<GOTargetInfo> m_UniqueGOTargetInfo;

    struct ItemTargetInfo
    {
        Item  *item;
        uint32 effectMask;
    };
    std::list<ItemTargetInfo> m_UniqueItemInfo;

    struct AreaTriggerTargetInfo
    {
        uint64 targetGUID;
        uint64 timeDelay;
        uint32 effectMask : 32;
        bool   processed  : 1;
    };
    std::list<AreaTriggerTargetInfo> m_UniqueAreaTriggerTargetInfo;

    SpellDestination m_destTargets[SpellEffIndex::MAX_EFFECTS];

    void AddUnitTarget(Unit* target, uint32 effectMask, bool checkIfValid = true, bool implicit = true, uint8 effectIndex = EFFECT_0);
    void AddGOTarget(GameObject* target, uint32 effectMask);
    void AddItemTarget(Item* item, uint32 effectMask);
    void AddAreaTriggerTarget(AreaTrigger* p_Target, uint32 p_EffectMask);
    void AddDestTarget(SpellDestination const& dest, uint32 effIndex);

    void DoAllEffectOnTarget(TargetInfo* target);
    SpellMissInfo DoSpellHitOnUnit(Unit* unit, uint32 effectMask, bool scaleAura);
    void DoTriggersOnSpellHit(Unit* unit, uint32 effMask);
    void DoAllEffectOnTarget(GOTargetInfo* target);
    void DoAllEffectOnTarget(ItemTargetInfo* target);
    void DoAllEffectOnTarget(AreaTriggerTargetInfo* p_Target);
    bool UpdateChanneledTargetList();
    bool IsValidDeadOrAliveTarget(Unit const* target) const;
    void HandleLaunchPhase();
    void DoAllEffectOnLaunchTarget(TargetInfo& targetInfo, float* multiplier);

    void PrepareTargetProcessing();
    void FinishTargetProcessing();

    // spell execution log
    void InitEffectExecuteData(uint8 effIndex);
    void CheckEffectExecuteData();

    // Scripting system
    void LoadScripts();
    void CallScriptBeforeCastHandlers();
    void CallScriptOnCastHandlers();
    void CallScriptAfterCastHandlers();
    void CallScriptOnPrepareHandlers();
    bool CallScriptCheckInterruptHandlers();
    SpellCastResult CallScriptCheckCastHandlers();
    void PrepareScriptHitHandlers();
    bool CallScriptEffectHandlers(SpellEffIndex effIndex, SpellEffectHandleMode mode);
    void CallScriptBeforeHitHandlers();
    void CallScriptOnHitHandlers();
    void CallScriptAfterHitHandlers();
    void CallScriptObjectAreaTargetSelectHandlers(std::list<WorldObject*>& targets, SpellEffIndex effIndex);
    void CallScriptObjectTargetSelectHandlers(WorldObject*& target, SpellEffIndex effIndex);
    std::list<SpellScript*> m_loadedScripts;

    struct HitTriggerSpell
    {
        SpellInfo const* triggeredSpell;
        SpellInfo const* triggeredByAura;
        // uint8 triggeredByEffIdx          This might be needed at a later stage - No need known for now
        int32 chance;
    };

    bool CanExecuteTriggersOnHit(uint32 effMask, SpellInfo const* triggeredByAura = NULL) const;
    bool CanProcOnTarget(Unit *target) const;
    void PrepareTriggersExecutedOnHit();
    typedef std::list<HitTriggerSpell> HitTriggerSpellList;
    HitTriggerSpellList m_hitTriggerSpells;

    // effect helpers
    void SummonGuardian(uint32 i, uint32 entry, SummonPropertiesEntry const* properties, uint32 numSummons);
    void CalculateJumpSpeeds(uint8 i, float dist, float & speedxy, float & speedz);

    SpellCastResult CanOpenLock(uint32 effIndex, uint32 lockid, SkillType& skillid, int32& reqSkillValue, int32& skillValue);
    // -------------------------------------------

    uint32 m_spellState;
    int32 m_timer;

    TriggerCastFlags _triggeredCastFlags;

    // if need this can be replaced by Aura copy
    // we can't store original aura link to prevent access to deleted auras
    // and in same time need aura data and after aura deleting.
    SpellInfo const* m_triggeredByAuraSpell;

    bool m_skipCheck;
    uint32 m_auraScaleMask;
    PathGenerator m_preGeneratedPath;

    typedef std::map<uint32, SpellLogHelper> LogHelperMap;
    LogHelperMap m_effectExecuteData;

    bool m_redirected;
    uint32 m_SpellVisualID;
#ifdef MAP_BASED_RAND_GEN
    int32 irand(int32 min, int32 max)       { return int32(m_caster->GetMap()->mtRand.randInt(max - min)) + min; }
    uint32 urand(uint32 min, uint32 max)    { return m_caster->GetMap()->mtRand.randInt(max - min) + min; }
    int32 rand32()                          { return m_caster->GetMap()->mtRand.randInt(); }
    double rand_norm()                      { return m_caster->GetMap()->mtRand.randExc(); }
    double rand_chance()                    { return m_caster->GetMap()->mtRand.randExc(100.0); }
#endif
};

namespace JadeCore
{
    struct WorldObjectSpellTargetCheck
    {
        Unit* _caster;
        Unit* _referer;
        SpellInfo const* _spellInfo;
        SpellTargetCheckTypes _targetSelectionType;
        ConditionSourceInfo* _condSrcInfo;
        ConditionContainer* _condList;

        WorldObjectSpellTargetCheck(Unit* caster, Unit* referer, SpellInfo const* spellInfo,
            SpellTargetCheckTypes selectionType, ConditionContainer* condList);
        ~WorldObjectSpellTargetCheck();
        bool operator()(WorldObject* target);
    };

    struct WorldObjectSpellNearbyTargetCheck : public WorldObjectSpellTargetCheck
    {
        float _range;
        Position const* _position;
        WorldObjectSpellNearbyTargetCheck(float range, Unit* caster, SpellInfo const* spellInfo,
            SpellTargetCheckTypes selectionType, ConditionContainer* condList);
        bool operator()(WorldObject* target);
    };

    struct WorldObjectSpellAreaTargetCheck : public WorldObjectSpellTargetCheck
    {
        float _range;
        Position const* _position;
        WorldObjectSpellAreaTargetCheck(float range, Position const* position, Unit* caster,
            Unit* referer, SpellInfo const* spellInfo, SpellTargetCheckTypes selectionType, ConditionContainer* condList);
        bool operator()(WorldObject* target);
    };

    struct WorldObjectSpellConeTargetCheck : public WorldObjectSpellAreaTargetCheck
    {
        float _coneAngle;
        WorldObjectSpellConeTargetCheck(float coneAngle, float range, Unit* caster,
            SpellInfo const* spellInfo, SpellTargetCheckTypes selectionType, ConditionContainer* condList);
        bool operator()(WorldObject* target);
    };

    struct WorldObjectSpellTrajTargetCheck : public WorldObjectSpellAreaTargetCheck
    {
        WorldObjectSpellTrajTargetCheck(float range, Position const* position, Unit* caster, SpellInfo const* spellInfo);
        bool operator()(WorldObject* target);
    };
}

typedef void(Spell::*pEffect)(SpellEffIndex effIndex);

class SpellEvent : public BasicEvent
{
public:
    SpellEvent(Spell* spell);
    virtual ~SpellEvent();

    virtual bool Execute(uint64 e_time, uint32 p_time);
    virtual void Abort(uint64 e_time);
    virtual bool IsDeletable() const;
protected:
    Spell* m_Spell;
};
#endif
