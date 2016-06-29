////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AnticheatMgr.h"
#include "Common.h"
#include "CreatureAIImpl.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Unit.h"
#include "QuestDef.h"
#include "Player.h"
#include "Creature.h"
#include "Spell.h"
#include "Group.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "MapManager.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "Formulas.h"
#include "Pet.h"
#include "Util.h"
#include "Totem.h"
#include "Battleground.h"
#include "OutdoorPvP.h"
#include "InstanceSaveMgr.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "CreatureGroups.h"
#include "PetAI.h"
#include "PassiveAI.h"
#include "TemporarySummon.h"
#include "Vehicle.h"
#include "Transport.h"
#include "InstanceScript.h"
#include "SpellInfo.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"
#include "ConditionMgr.h"
#include "UpdateFieldFlags.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "SpellAuraEffects.h"
#include "BattlegroundKT.h"
#include "BattlegroundWS.h"
#include "BattlegroundTP.h"
#include "BattlegroundDG.h"
#include "Guild.h"
#include "DB2Stores.h"
#include "../../Garrison/GarrisonMgr.hpp"
#include "../../../scripts/Draenor/Garrison/GarrisonScriptData.hpp"

//#include <Reporting/Reporter.hpp>

float baseMoveSpeed[MAX_MOVE_TYPE] =
{
    2.5f,                  // MOVE_WALK
    7.0f,                  // MOVE_RUN
    4.5f,                  // MOVE_RUN_BACK
    4.722222f,             // MOVE_SWIM
    2.5f,                  // MOVE_SWIM_BACK
    3.141594f,             // MOVE_TURN_RATE
    7.0f,                  // MOVE_FLIGHT
    4.5f,                  // MOVE_FLIGHT_BACK
    3.14f                  // MOVE_PITCH_RATE
};

float playerBaseMoveSpeed[MAX_MOVE_TYPE] =
{
    2.5f,                  // MOVE_WALK
    7.0f,                  // MOVE_RUN
    4.5f,                  // MOVE_RUN_BACK
    4.722222f,             // MOVE_SWIM
    2.5f,                  // MOVE_SWIM_BACK
    3.141594f,             // MOVE_TURN_RATE
    7.0f,                  // MOVE_FLIGHT
    4.5f,                  // MOVE_FLIGHT_BACK
    3.14f                  // MOVE_PITCH_RATE
};

#define SPELL_PLAYER_LIFE_STEAL 146347

// Used for prepare can/can`t triggr aura
static bool InitTriggerAuraData();
// Define can trigger auras
static bool isTriggerAura[TOTAL_AURAS];
// Define can't trigger auras (need for disable second trigger)
static bool isNonTriggerAura[TOTAL_AURAS];
// Triggered always, even from triggered spells
static bool isAlwaysTriggeredAura[TOTAL_AURAS];
// Prepare lists
static bool procPrepared = InitTriggerAuraData();

DamageInfo::DamageInfo(Unit* _attacker, Unit* _victim, uint32 _damage, SpellInfo const* _spellInfo, SpellSchoolMask _schoolMask, DamageEffectType _damageType)
: m_attacker(_attacker), m_victim(_victim), m_damage(_damage), m_spellInfo(_spellInfo), m_schoolMask(_schoolMask),
m_damageType(_damageType), m_attackType(WeaponAttackType::BaseAttack)
{
    m_absorb = 0;
    m_resist = 0;
    m_block = 0;
}
DamageInfo::DamageInfo(CalcDamageInfo& dmgInfo)
: m_attacker(dmgInfo.attacker), m_victim(dmgInfo.target), m_damage(dmgInfo.damage), m_spellInfo(NULL), m_schoolMask(SpellSchoolMask(dmgInfo.damageSchoolMask)),
m_damageType(DIRECT_DAMAGE), m_attackType(dmgInfo.attackType)
{
    m_absorb = 0;
    m_resist = 0;
    m_block = 0;
}

void DamageInfo::ModifyDamage(int32 amount)
{
    amount = std::min(amount, int32(GetDamage()));
    m_damage += amount;
}

void DamageInfo::ModifyAbsorb(int32 amount)
{
    m_absorb += amount;
}

void DamageInfo::AbsorbDamage(uint32 amount)
{
    amount = std::min(amount, GetDamage());
    m_absorb += amount;
    m_damage -= amount;
}

void DamageInfo::ResistDamage(uint32 amount)
{
    amount = std::min(amount, GetDamage());
    m_resist += amount;
    m_damage -= amount;
}

void DamageInfo::BlockDamage(uint32 amount)
{
    amount = std::min(amount, GetDamage());
    m_block += amount;
    m_damage -= amount;
}

ProcEventInfo::ProcEventInfo(Unit* actor, Unit* actionTarget, Unit* procTarget, uint32 typeMask, uint32 spellTypeMask, uint32 spellPhaseMask, uint32 hitMask, Spell* spell, DamageInfo* damageInfo, HealInfo* healInfo)
:_actor(actor), _actionTarget(actionTarget), _procTarget(procTarget), _typeMask(typeMask), _spellTypeMask(spellTypeMask), _spellPhaseMask(spellPhaseMask),
_hitMask(hitMask), _spell(spell), _damageInfo(damageInfo), _healInfo(healInfo)
{
}

// we can disable this warning for this since it only
// causes undefined behavior when passed to the base class constructor
#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif
Unit::Unit(bool isWorldObject): WorldObject(isWorldObject)
    , m_movedPlayer(NULL)
    , m_lastSanctuaryTime(0)
    , m_TempSpeed(0.0f)
    , IsAIEnabled(false)
    , NeedChangeAI(false)
    , m_ControlledByPlayer(false)
    , movespline(new Movement::MoveSpline())
    , i_AI(NULL)
    , i_disabledAI(NULL)
    , m_AutoRepeatFirstCast(false)
    , m_procDeep(0)
    , m_removedAurasCount(0)
    , i_motionMaster(this)
    , m_ThreatManager(this)
    , m_vehicle(NULL)
    , m_vehicleKit(NULL)
    , m_unitTypeMask(UNIT_MASK_NONE)
    , m_disableHealthRegen(false)
    , m_disableEnterEvadeMode(false)
    , m_HostileRefManager(this)
    , _lastDamagedTime(0)
{
#ifdef _MSC_VER
#pragma warning(default:4355)
#endif
    m_objectType |= TYPEMASK_UNIT;
    m_objectTypeId = TYPEID_UNIT;

    m_updateFlag = UPDATEFLAG_HAS_MOVEMENT_UPDATE;

    m_attackTimer[WeaponAttackType::BaseAttack] = 0;
    m_attackTimer[WeaponAttackType::OffAttack] = 0;
    m_attackTimer[WeaponAttackType::RangedAttack] = 0;
    m_modAttackSpeedPct[WeaponAttackType::BaseAttack] = 1.0f;
    m_modAttackSpeedPct[WeaponAttackType::OffAttack] = 1.0f;
    m_modAttackSpeedPct[WeaponAttackType::RangedAttack] = 1.0f;

    m_extraAttacks = 0;
    m_InsightCount = 0;
    m_canDualWield = false;

    m_rootTimes = 0;

    m_state = 0;
    m_deathState = ALIVE;

    for (uint8 i = 0; i < CURRENT_MAX_SPELL; ++i)
        m_currentSpells[i] = NULL;

    m_addDmgOnce = 0;

    for (uint8 i = 0; i < MAX_SUMMON_SLOT; ++i)
        m_SummonSlot[i] = 0;

    for (uint8 i = 0; i < MAX_GAMEOBJECT_SLOT; ++i)
        m_ObjectSlot[i] = 0;

    m_auraUpdateIterator = m_ownedAuras.end();

    m_interruptMask = 0;
    m_transform = 0;
    m_canModifyStats = false;

    for (uint8 i = 0; i < MAX_SPELL_IMMUNITY; ++i)
        m_spellImmune[i].clear();

    for (uint8 l_I = 0; l_I < UNIT_MOD_END; ++l_I)
    {
        m_auraModifiersGroup[l_I][BASE_VALUE]               = 0.0f;
        m_auraModifiersGroup[l_I][BASE_PCT_EXCLUDE_CREATE]  = 100.0f;
        m_auraModifiersGroup[l_I][BASE_PCT]                 = 1.0f;
        m_auraModifiersGroup[l_I][TOTAL_VALUE]              = 0.0f;
        m_auraModifiersGroup[l_I][TOTAL_PCT]                = 1.0f;
    }
                                                            // implement 50% base damage from offhand
    m_auraModifiersGroup[UNIT_MOD_DAMAGE_OFFHAND][TOTAL_PCT] = 0.5f;

    for (uint8 i = 0; i < WeaponAttackType::MaxAttack; ++i)
    {
        m_weaponDamage[i][MINDAMAGE] = 0.0f;
        m_weaponDamage[i][MAXDAMAGE] = 0.0f;
    }

    for (uint8 i = 0; i < MAX_STATS; ++i)
        m_createStats[i] = 0.0f;

    m_attacking = NULL;
    m_modMeleeHitChance = 0.0f;
    m_modRangedHitChance = 0.0f;
    m_modSpellHitChance = 0.0f;
    m_baseSpellCritChance = 5;

    m_CombatTimer = 0;

    simulacrumTargetGUID = 0;
    m_GlaiveOfTossTargetGUID = 0;
    iciclesTargetGUID    = 0;
    lastMoonfireTargetGUID = 0;
    lastSunfireTargetGUID = 0;

    for (uint8 i = 0; i < MAX_SPELL_SCHOOL; ++i)
        m_threatModifier[i] = 1.0f;

    m_isSorted = true;

    for (uint8 i = 0; i < MAX_MOVE_TYPE; ++i)
        m_speed_rate[i] = 1.0f;

    m_charmInfo = NULL;
    m_reducedThreatPercent = 0;
    m_misdirectionTargetGUID = 0;

    // remove aurastates allowing special moves
    for (uint8 i = 0; i < MAX_REACTIVE; ++i)
        m_reactiveTimer[i] = 0;

    m_cleanupDone = false;
    m_duringRemoveFromWorld = false;

    m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GHOST, GHOST_VISIBILITY_ALIVE);

    _focusSpell = NULL;
    _lastLiquid = NULL;
    _isWalkingBeforeCharm = false;

    // Area Skip Update
    _skipCount = 0;
    _skipDiff = 0;

    _petBattleId = 0;

    m_IsInKillingProcess = false;
    m_VisibilityUpdScheduled = false;

    for (int i = 0; i < MAX_POWERS; ++i) ///< Comparison of integers of different signs: 'int' and 'Powers'
        m_lastRegenTime[i] = getMSTime();

    for (int i = 0; i < MAX_POWERS; ++i) ///< Comparison of integers of different signs: 'int' and 'Powers'
        m_powers[i] = 0;

    m_lastVisibilityUpdPos = *this;

    m_HealingRainTrigger = 0;

    m_PersonnalChauffeur = 0;

    m_LastNotifyPosition.Relocate(-5000.0f, -5000.0f, -5000.0f, 0.0f);
    m_LastOutdoorPosition.Relocate(-5000.0f, -5000.0f, -5000.0f, 0.0f);

    m_MapSwitchDestination = -1;
}

////////////////////////////////////////////////////////////
// Methods of class GlobalCooldownMgr
bool GlobalCooldownMgr::HasGlobalCooldown(SpellInfo const* spellInfo) const
{
    GlobalCooldownList::const_iterator itr = m_GlobalCooldowns.find(spellInfo->StartRecoveryCategory);
    return itr != m_GlobalCooldowns.end() && itr->second.duration && getMSTimeDiff(itr->second.cast_time, getMSTime() + 120) < itr->second.duration;
}

void GlobalCooldownMgr::AddGlobalCooldown(SpellInfo const* spellInfo, uint32 gcd)
{
    m_GlobalCooldowns[spellInfo->StartRecoveryCategory] = GlobalCooldown(gcd, getMSTime());
}

void GlobalCooldownMgr::CancelGlobalCooldown(SpellInfo const* spellInfo)
{
    m_GlobalCooldowns[spellInfo->StartRecoveryCategory].duration = 0;
}

////////////////////////////////////////////////////////////
// Methods of class Unit
Unit::~Unit()
{
    // set current spells as deletable
    for (uint8 i = 0; i < CURRENT_MAX_SPELL; ++i)
        if (m_currentSpells[i])
        {
            m_currentSpells[i]->SetReferencedFromCurrent(false);
            m_currentSpells[i]->SetExecutedCurrently(false);
            m_currentSpells[i] = NULL;
        }

    _DeleteRemovedAuras();

    for (DmgDoneList::iterator itr = m_dmgDone.begin(); itr != m_dmgDone.end(); itr++)
        delete (*itr);

    m_dmgDone.clear();

    delete m_charmInfo;
    delete movespline;

    // TODO : Find Why it crashes
    //ASSERT(!m_duringRemoveFromWorld);
    //ASSERT(!m_attacking);
    //ASSERT(m_attackers.empty());
    //ASSERT(m_sharedVision.empty());
    //ASSERT(m_Controlled.empty());
    //ASSERT(m_appliedAuras.empty());
    //ASSERT(m_ownedAuras.empty());
    //ASSERT(m_removedAuras.empty());
    //ASSERT(m_gameObj.empty());
    //ASSERT(m_dynObj.empty());
    //ASSERT(m_AreaTrigger.empty());
}

void Unit::Update(uint32 p_time)
{
    // WARNING! Order of execution here is important, do not change.
    // Spells must be processed with event system BEFORE they go to _UpdateSpells.
    // Or else we may have some SPELL_STATE_FINISHED spells stalled in pointers, that is bad.
    m_Events.Update(p_time);

    if (!IsInWorld())
        return;

    _UpdateSpells(p_time);

    uint32 l_Now = getMSTime();

    DmgDoneList::iterator dmgDoneNext;
    for (DmgDoneList::iterator itr = m_dmgDone.begin(); itr != m_dmgDone.end(); itr = dmgDoneNext)
    {
        dmgDoneNext = itr;
        ++dmgDoneNext;

        if ((l_Now - (*itr)->s_timestamp) > 60 * IN_MILLISECONDS)
        {
            delete (*itr);
            m_dmgDone.erase(itr);
        }
    }

    // If this is set during update SetCantProc(false) call is missing somewhere in the code
    // Having this would prevent spells from being proced, so let's crash
    ASSERT(!m_procDeep);

    if (CanHaveThreatList() && getThreatManager().isNeedUpdateToClient(p_time))
        SendThreatListUpdate();

    // update combat timer only for players and pets (only pets with PetAI)
    if (isInCombat() && (IsPlayer() || (ToCreature()->isPet() && IsControlledByPlayer())))
    {
        // Check UNIT_STATE_MELEE_ATTACKING or UNIT_STATE_CHASE (without UNIT_STATE_FOLLOW in this case) so pets can reach far away
        // targets without stopping half way there and running off.
        // These flags are reset after target dies or another command is given.
        if (m_HostileRefManager.isEmpty())
        {
            // m_CombatTimer set at aura start and it will be freeze until aura removing
            if (m_CombatTimer <= p_time)
                ClearInCombat();
            else
                m_CombatTimer -= p_time;
        }
    }

    // not implemented before 3.0.2
    if (uint32 base_att = getAttackTimer(WeaponAttackType::BaseAttack))
        setAttackTimer(WeaponAttackType::BaseAttack, (base_att - p_time));
    if (uint32 ranged_att = getAttackTimer(WeaponAttackType::RangedAttack))
        setAttackTimer(WeaponAttackType::RangedAttack, (ranged_att - p_time));
    if (uint32 off_att = getAttackTimer(WeaponAttackType::OffAttack))
        setAttackTimer(WeaponAttackType::OffAttack, (off_att - p_time));

    // update abilities available only for fraction of time
    UpdateReactives(p_time);

    /// Update all the stack durations
    UpdateStackOnDuration(p_time);

    if (isAlive())
    {
        ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, HealthBelowPct(20));
        ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, HealthBelowPct(35));
        ModifyAuraState(AURA_STATE_HEALTH_ABOVE_75_PERCENT, HealthAbovePct(75));
    }

    UpdateSplineMovement(p_time);
    i_motionMaster.UpdateMotion(p_time);
}

bool Unit::haveOffhandWeapon() const
{
    if (IsPlayer())
        return ToPlayer()->GetWeaponForAttack(WeaponAttackType::OffAttack, true);
    else
        return m_canDualWield;
}

void Unit::MonsterMoveWithSpeed(float x, float y, float z, float speed, bool generatePath, bool forceDestination)
{
    Movement::MoveSplineInit init(this);
    init.MoveTo(x, y, z, generatePath, forceDestination);
    init.SetVelocity(speed);
    init.Launch();
}

uint32 const positionUpdateDelay = 400;
uint32 const g_FlightSplineSyncDelay = 5 * TimeConstants::IN_MILLISECONDS;

void Unit::UpdateSplineMovement(uint32 p_Diff)
{
    bool l_Arrived = movespline->Finalized(); ///< l_Arrived is never read 01/18/16
    if (movespline->Finalized())
        return;

    movespline->updateState(p_Diff);
    l_Arrived = movespline->Finalized();

    if (l_Arrived)
        DisableSpline();

    m_movesplineTimer.Update(p_Diff);
    if (m_movesplineTimer.Passed() || l_Arrived)
        UpdateSplinePosition();

    m_FlightSplineSyncTimer.Update(p_Diff);
    if (m_FlightSplineSyncTimer.Passed())
    {
        float l_Percent = 1.0f;
        float l_TotalTime = movespline->spline.length();
        if (l_TotalTime > 0.0f)
            l_Percent = (float)movespline->time_passed / l_TotalTime;

        SendFlightSplineSync(l_Percent);
        m_FlightSplineSyncTimer.Reset(g_FlightSplineSyncDelay);
    }
}

void Unit::UpdateSplinePosition()
{
    m_movesplineTimer.Reset(positionUpdateDelay);

    Movement::Location l_Location = movespline->ComputePosition();
    if (GetTransGUID())
    {
        Position& l_Pos = m_movementInfo.t_pos;
        l_Pos.m_positionX = l_Location.x;
        l_Pos.m_positionY = l_Location.y;
        l_Pos.m_positionZ = l_Location.z;
        l_Pos.SetOrientation(l_Location.orientation);

        if (GetVehicleBase())
        {
            if (TransportBase* l_Transport = GetDirectTransport())
                l_Transport->CalculatePassengerPosition(l_Location.x, l_Location.y, l_Location.z, l_Location.orientation);
        }
    }

    if (HasUnitState(UnitState::UNIT_STATE_CANNOT_TURN))
        l_Location.orientation = GetOrientation();

    UpdatePosition(l_Location.x, l_Location.y, l_Location.z, l_Location.orientation);

    /// Update all passengers after updating vehicle position
    /// This will prevent some base positioning if vehicles are updated in the wrong order
    if (Vehicle* l_Vehicle = GetVehicleKit())
    {
        for (int8 l_I = 0; l_I < MAX_VEHICLE_SEATS; ++l_I)
        {
            if (Unit* l_Passenger = l_Vehicle->GetPassenger(l_I))
            {
                if (l_Passenger->movespline->Initialized())
                    l_Passenger->UpdateSplinePosition();
            }
        }
     }
}

void Unit::DisableSpline()
{
    m_movementInfo.RemoveMovementFlag(MOVEMENTFLAG_FORWARD);
    movespline->_Interrupt();
}

void Unit::resetAttackTimer(WeaponAttackType type)
{
    if (m_attackTimer[type] < 0 && int32(GetAttackTime(type) * m_modAttackSpeedPct[type]) > m_attackTimer[type] * -1)
        m_attackTimer[type] = uint32(GetAttackTime(type) * m_modAttackSpeedPct[type]) + m_attackTimer[type];
    else
        m_attackTimer[type] = uint32(GetAttackTime(type) * m_modAttackSpeedPct[type]);
}

bool Unit::IsWithinCombatRange(const Unit* obj, float dist2compare) const
{
    if (!obj || !IsInMap(obj) || !InSamePhase(obj))
        return false;

    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float dz = GetPositionZ() - obj->GetPositionZ();
    float distsq = dx * dx + dy * dy + dz * dz;

    float sizefactor = GetCombatReach() + obj->GetCombatReach();
    float maxdist = dist2compare + sizefactor;

    return distsq < maxdist * maxdist;
}

bool Unit::IsWithinMeleeRange(const Unit* obj, float dist) const
{
    if (!obj || !IsInMap(obj) || !InSamePhase(obj))
        return false;

    if (IsPlayer() && (obj->IsPlayer() || obj->IsPetGuardianStuff()))
    {
        float maxDist = NOMINAL_MELEE_RANGE;   // auto-attack maximum distance
        if (dist > MELEE_RANGE)
        {
            maxDist = dist;
            if (IsMoving() && obj->IsMoving())
                maxDist += 2.5f;
        }

        return GetExactDist(obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ()) < maxDist;
    }

    float dx = GetPositionX() - obj->GetPositionX();
    float dy = GetPositionY() - obj->GetPositionY();
    float dz = GetPositionZ() - obj->GetPositionZ();
    float distsq = dx*dx + dy*dy + dz*dz;

    float sizefactor = GetMeleeReach() + obj->GetMeleeReach();
    float maxdist = dist + sizefactor;

    return distsq < maxdist * maxdist;
}

void Unit::GetRandomContactPoint(const Unit* obj, float &x, float &y, float &z, float distance2dMin, float distance2dMax) const
{
    float combat_reach = GetCombatReach();
    if (combat_reach < 0.1f) // sometimes bugged for players
        combat_reach = DEFAULT_COMBAT_REACH;

    uint32 attacker_number = getAttackers().size();
    if (attacker_number > 0)
        --attacker_number;
    GetNearPoint(obj, x, y, z, obj->GetCombatReach(), distance2dMin+(distance2dMax-distance2dMin) * (float)rand_norm()
        , GetAngle(obj) + (attacker_number ? (static_cast<float>(M_PI/2) - static_cast<float>(M_PI) * (float)rand_norm()) * float(attacker_number) / combat_reach * 0.3f : 0));
}

void Unit::UpdateInterruptMask()
{
    m_interruptMask = 0;
    for (AuraApplicationList::const_iterator i = m_interruptableAuras.begin(); i != m_interruptableAuras.end(); ++i)
        m_interruptMask |= (*i)->GetBase()->GetSpellInfo()->AuraInterruptFlags;

    if (Spell* spell = m_currentSpells[CURRENT_CHANNELED_SPELL])
        if (spell->getState() == SPELL_STATE_CASTING)
            m_interruptMask |= spell->m_spellInfo->ChannelInterruptFlags;
}

bool Unit::HasAuraTypeWithFamilyFlags(AuraType auraType, uint32 familyName, uint32 familyFlags) const
{
    if (!HasAuraType(auraType))
        return false;
    AuraEffectList const& auras = GetAuraEffectsByType(auraType);
    for (AuraEffectList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
        if (SpellInfo const* iterSpellProto = (*itr)->GetSpellInfo())
            if (iterSpellProto->SpellFamilyName == familyName && iterSpellProto->SpellFamilyFlags[0] & familyFlags)
                return true;
    return false;
}

bool Unit::HasBreakableByDamageAuraType(AuraType p_Type, uint32 p_ExcludeAura) const
{
    AuraEffectList const& l_Auras = GetAuraEffectsByType(p_Type);
    for (AuraEffectList::const_iterator l_Iter = l_Auras.begin(); l_Iter != l_Auras.end(); ++l_Iter)
    {
        if ((!p_ExcludeAura || p_ExcludeAura != (*l_Iter)->GetSpellInfo()->Id) && ///< Avoid self interrupt of channeled Crowd Control spells like Seduction
            ((*l_Iter)->GetSpellInfo()->Attributes & SPELL_ATTR0_BREAKABLE_BY_DAMAGE || (*l_Iter)->GetSpellInfo()->AuraInterruptFlags & (AURA_INTERRUPT_FLAG_TAKE_DAMAGE_AMOUNT | AURA_INTERRUPT_FLAG_TAKE_DAMAGE)))
            return true;
    }
    return false;
}

bool Unit::HasBreakableByDamageCrowdControlAura(Unit* excludeCasterChannel) const
{
    uint32 excludeAura = 0;
    if (Spell* currentChanneledSpell = excludeCasterChannel ? excludeCasterChannel->GetCurrentSpell(CURRENT_CHANNELED_SPELL) : NULL)
        excludeAura = currentChanneledSpell->GetSpellInfo()->Id; //Avoid self interrupt of channeled Crowd Control spells like Seduction

    return (   HasBreakableByDamageAuraType(SPELL_AURA_MOD_CONFUSE, excludeAura)
            || HasBreakableByDamageAuraType(SPELL_AURA_MOD_FEAR, excludeAura)
            || HasBreakableByDamageAuraType(SPELL_AURA_MOD_FEAR_2, excludeAura)
            || HasBreakableByDamageAuraType(SPELL_AURA_MOD_STUN, excludeAura)
            || HasBreakableByDamageAuraType(SPELL_AURA_MOD_ROOT, excludeAura)
            || HasBreakableByDamageAuraType(SPELL_AURA_MOD_ROOT_2, excludeAura)
            || HasBreakableByDamageAuraType(SPELL_AURA_TRANSFORM, excludeAura));
}

bool Unit::HasAurasPreventCasting() const
{
    uint32 l_UnitFlags = GetUInt32Value(UNIT_FIELD_FLAGS);

    if ((l_UnitFlags & UNIT_FLAG_SILENCED) || (l_UnitFlags & UNIT_FLAG_PACIFIED) || (l_UnitFlags & UNIT_FLAG_STUNNED) ||
        (l_UnitFlags & UNIT_FLAG_CONFUSED))
        return true;

    return false;
}

void Unit::DealDamageMods(Unit* victim, uint32 &damage, uint32* absorb)
{
    if (!victim || !victim->isAlive() || victim->HasUnitState(UNIT_STATE_IN_FLIGHT) || (victim->GetTypeId() == TYPEID_UNIT && victim->ToCreature()->IsInEvadeMode()))
    {
        if (absorb)
            *absorb += damage;
        damage = 0;
    }
}

uint32 Unit::DealDamage(Unit* victim, uint32 damage, CleanDamage const* cleanDamage, DamageEffectType damagetype, SpellSchoolMask damageSchoolMask, SpellInfo const* spellProto, bool durabilityLoss)
{
    if (victim->isDead() || victim->GetHealth() == 0)
    {
        damage = 0;
        return 0; ///< Prevent double death
    }

    // need for operations with Player class
    Player* plr = victim->ToPlayer();

    /// Reduce cooldown of Howl of Terror by 1s if hit
    if (plr && plr->getClass() == CLASS_WARLOCK && plr->HasSpellCooldown(5484) && damage)
        plr->ReduceSpellCooldown(5484, 1000);

    /// Custom WoD Script - Glyph of Fortuitous Spheres
    if (plr && IsPlayer() && victim->getClass() == CLASS_MONK && victim->HasAura(146953))
    {
        if (victim->GetHealthPct() >= 25.0f && victim->HealthBelowPctDamaged(25, damage) && !ToPlayer()->HasSpellCooldown(115460))
        {
            victim->CastSpell(victim, 115460, true);
            ToPlayer()->AddSpellCooldown(115460, 0, 30 * IN_MILLISECONDS); ///<  This effect cannot occur more often than once every 30 seconds.
        }
    }

    // Custom MoP Script - Cloak of Shadows
    if (victim && victim->getClass() == CLASS_ROGUE && damagetype == SPELL_DIRECT_DAMAGE && HasAura(31224))
        damage = 0;

    /// Custom MoP Script - Subterfuge , should be called from normal hit
    if (victim && HasAura(108208) && HasAura(115191) && !HasAura(115192) && (damagetype == DIRECT_DAMAGE || HasAura(121471)))
        CastSpell(this, 115192, true);

    /// Custom MoP Script - Desperate Measures
    if (plr && IsPlayer() && victim->getClass() == CLASS_MONK && victim->HasAura(126060))
    {
        if (victim->CountPctFromMaxHealth(35) > (victim->GetHealth() - damage))
        {
            if (plr->HasSpellCooldown(115072))
            {
                plr->RemoveSpellCooldown(115129, true);
                plr->RemoveSpellCooldown(115072, true);
            }
        }
    }

    /// Custom MoP Script - Focused Will
    if (IsPlayer() && victim->getClass() == CLASS_PRIEST)
    {
        if (plr->GetSpecializationId() == SPEC_PRIEST_DISCIPLINE)
        {
            if (victim->CountPctFromMaxHealth(15) < damage) ///< last update 6.0.1 (Tue Oct 14 2014) Build 18379
                victim->CastSpell(victim, 45242, true);
        }
    }

    /// @todo update me ?
    /// Custom MoP Script
    if (IsPlayer() && getClass() == CLASS_MONK && ToPlayer()->GetSpecializationId() == SPEC_MONK_BREWMASTER && HasAura(115315))
    {
        if (damage > 0)
        {
            if (Aura* blackOxStatue = GetAura(115315))
                blackOxStatue->SetScriptData(0, damage);
        }
    }

    /// Custom WoD script
    /// Mark of Sindragosa
    if (spellProto != nullptr && IsPlayer() && getClass() == CLASS_DEATH_KNIGHT && ToPlayer()->GetSpecializationId() == SPEC_DK_BLOOD && victim->HasAura(155166, GetGUID()))
    {
        if (damage > 0)
        {
            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(155166);
            HealBySpell(this, l_SpellInfo, CalculatePct(damage, 10));
        }
    }

    /// Mindbender
    if (GetEntry() == 62982 || GetEntry() == 67236)
    {
        /// Caster receives 0.75% mana when the Mindbender attacks
        if (Unit* l_Owner = GetOwner())
            l_Owner->ModifyPower(POWER_MANA, CalculatePct(l_Owner->GetPower(POWER_MANA), 0.75f));
    }

    // Stagger handler
    if (victim && victim->ToPlayer() && victim->getClass() == CLASS_MONK && damagetype != DamageEffectType::SELF_DAMAGE)
    {
        if (!victim->HasSpell(157533))
        {
            if (!spellProto || ((spellProto->DmgClass == SPELL_DAMAGE_CLASS_RANGED || spellProto->DmgClass == SPELL_DAMAGE_CLASS_MELEE) && damageSchoolMask & SPELL_SCHOOL_MASK_NORMAL))
            {
                if (!spellProto || (spellProto->Id != LIGHT_STAGGER && spellProto->Id != MODERATE_STAGGER && spellProto->Id != HEAVY_STAGGER))
                    damage = victim->CalcStaggerDamage(victim->ToPlayer(), damage, damageSchoolMask, spellProto);
            }
        }
        else ///< You are now able to shrug off even spells
        {
            if (!spellProto || (spellProto->Id != LIGHT_STAGGER && spellProto->Id != MODERATE_STAGGER && spellProto->Id != HEAVY_STAGGER))
                damage = victim->CalcStaggerDamage(victim->ToPlayer(), damage, damageSchoolMask, spellProto);
        }
    }

    /// Temporal Shield - 115610
    if (victim->IsPlayer() && damage != 0 && victim->HasAura(115610))
    {
        int32 l_Bp = damage / 3;

        /// Temporal Ripples : Add remaining amount to the basepoints
        l_Bp += victim->GetRemainingPeriodicAmount(victim->GetGUID(), 115611, SPELL_AURA_PERIODIC_HEAL, 0);

        victim->CastCustomSpell(victim, 115611, &l_Bp, NULL, NULL, true);
    }

    /// last update : 6.1.2 19802
    /// Stance of the Spirited Crane - 154436
    if (GetSpellModOwner() && GetSpellModOwner()->HasAura(154436) && GetTypeId() == TYPEID_PLAYER && getClass() == CLASS_MONK)
        if (!spellProto || (spellProto
        && spellProto->Id != 115129 && spellProto->Id != 125033 && spellProto->Id != 124098 && spellProto->Id != 132467
        && spellProto->Id != 130651 && spellProto->Id != 117993)) ///< Don't triggered by Zen Sphere, Chi Wave, Chi Burst, Chi Torpedo and Expel Harm
    {
        int32 l_Bp = damage / 2;
        std::list<Creature*> l_TempList;
        std::list<Creature*> l_StatueList;
        Creature* l_Statue = nullptr;

        GetSpellModOwner()->GetCreatureListWithEntryInGrid(l_TempList, 60849, 100.0f);
        GetSpellModOwner()->GetCreatureListWithEntryInGrid(l_StatueList, 60849, 100.0f);

        /// Remove other players jade statue
        for (std::list<Creature*>::iterator i = l_TempList.begin(); i != l_TempList.end(); ++i)
        {
            Unit* l_Owner = (*i)->GetOwner();
            if (l_Owner && l_Owner == ToPlayer() && (*i)->isSummon())
                continue;

            l_StatueList.remove((*i));
        }

        /// In addition, you also gain Eminence, causing you to heal the lowest health nearby target within 20 yards for an amount equal to 50% of non-autoattack damage you deal
        CastCustomSpell(this, 126890, &l_Bp, NULL, NULL, true, 0, nullptr, GetSpellModOwner()->GetGUID()); ///< Eminence

        if (l_StatueList.size() == 1)
        {
            for (auto itrBis : l_StatueList)
                l_Statue = itrBis;

            if (l_Statue && (l_Statue->isPet() || l_Statue->isGuardian()))
            {
                if (l_Statue->GetOwner() && l_Statue->GetOwner()->GetGUID() == GetGUID())
                    l_Statue->CastCustomSpell(l_Statue, 117895, &l_Bp, NULL, NULL, true, 0, nullptr, GetGUID()); ///< Eminence - statue
            }
        }
    }

    /// Health leech handling
    if (GetTypeId() == TypeID::TYPEID_PLAYER && damage > 0 && (!spellProto || spellProto->Id != SPELL_PLAYER_LIFE_STEAL))
    {
        float l_Percentage = GetFloatValue(EPlayerFields::PLAYER_FIELD_LIFESTEAL);
        int32 l_Heal = CalculatePct(damage, (int32)l_Percentage);
        CastCustomSpell(this, SPELL_PLAYER_LIFE_STEAL, &l_Heal, nullptr, nullptr, true);
    }

    if (victim->IsAIEnabled)
        victim->GetAI()->DamageTaken(this, damage, spellProto);

    if (IsAIEnabled)
        GetAI()->DamageDealt(victim, damage, damagetype);

    if (GetTypeId() == TypeID::TYPEID_PLAYER
        && victim->GetTypeId() == TypeID::TYPEID_UNIT
        && victim->ToCreature()->GetNativeTemplate()->flags_extra & CreatureFlagsExtra::CREATURE_FLAG_EXTRA_LOG_GROUP_DMG)
    {
        CreatureDamageLog l_Log;
        l_Log.AttackerGuid = GetGUIDLow();
        l_Log.Damage       = damage;
        l_Log.Spell        = spellProto ? spellProto->Id : 0;
        l_Log.Time         = time(nullptr);

        victim->ToCreature()->AddDamageLog(l_Log);
    }

    if (Player* l_Player = victim->ToPlayer())
    {
        if (!l_Player || l_Player->GetCommandStatus(CHEAT_GOD))
            return 0;

        /// Signal to pets that their owner was attacked
        Pet* l_Pet = l_Player->GetPet();

        if (l_Pet && l_Pet->isAlive())
            l_Pet->AI()->OwnerDamagedBy(this);

        sScriptMgr->OnPlayerTakeDamage(l_Player, damagetype, damage, damageSchoolMask, cleanDamage);
    }

    if (damagetype != NODAMAGE)
    {
        // interrupting auras with AURA_INTERRUPT_FLAG_DAMAGE before checking !damage (absorbed damage breaks that type of auras)
        if (spellProto)
        {
            if (!(spellProto->AttributesEx4 & SPELL_ATTR4_DAMAGE_DOESNT_BREAK_AURAS))
            {
                // Repentance shared damage removal
                if (cleanDamage && cleanDamage->mitigated_damage)
                    victim->RemoveAurasDueToSpell(20066);

                victim->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TAKE_DAMAGE, spellProto->Id);
            }
        }
        else
            victim->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TAKE_DAMAGE, 0);

        // We're going to call functions which can modify content of the list during iteration over it's elements
        // Let's copy the list so we can prevent iterator invalidation
        AuraEffectList vCopyDamageCopy(victim->GetAuraEffectsByType(SPELL_AURA_SHARE_DAMAGE_PCT));
        // copy damage to casters of this aura
        for (AuraEffectList::iterator i = vCopyDamageCopy.begin(); i != vCopyDamageCopy.end(); ++i)
        {
            // Check if aura was removed during iteration - we don't need to work on such auras
            if (!((*i)->GetBase()->IsAppliedOnTarget(victim->GetGUID())))
                continue;
            // check damage school mask
            if (((*i)->GetMiscValue() & damageSchoolMask) == 0)
                continue;

            Unit* shareDamageTarget = (*i)->GetCaster();
            if (!shareDamageTarget)
                continue;
            SpellInfo const* spell = (*i)->GetSpellInfo();

            uint32 share = CalculatePct(damage, (*i)->GetAmount());

            // Voodoo Doll
            if (spell->Id == 116000 && victim->IsPlayer())
            {
                Player* _plr = victim->ToPlayer();
                std::list<Unit*> groupList;

                _plr->GetPartyMembers(groupList);
                for (auto itr : groupList)
                {
                    // Voodoo visual
                    if (!itr->HasAura(122151))
                        continue;

                    if (itr->GetGUID() == _plr->GetGUID())
                        continue;

                    shareDamageTarget->DealDamageMods(itr, share, NULL);
                    shareDamageTarget->DealDamage(itr, share, NULL, DIRECT_DAMAGE, spell->GetSchoolMask(), spell, false);
                }
            }
            else
            {
                // TODO: check packets if damage is done by victim, or by attacker of victim
                DealDamageMods(shareDamageTarget, share, NULL);
                DealDamage(shareDamageTarget, share, NULL, NODAMAGE, spell->GetSchoolMask(), spell, false);
            }
        }
    }

    /// Rage from Damage made (only from direct weapon damage)
    if (cleanDamage && damagetype == DIRECT_DAMAGE && this != victim && getPowerType() == POWER_RAGE && IsPlayer())
    {
        Player* l_Player = ToPlayer();

        /// Only Druids in Bear form and Warrior Arms / Battle-Defensive, Fury / Battle, Prot / Battle can generate rage from autoattack
        if (l_Player->GetShapeshiftForm() == FORM_BEAR || l_Player->GetShapeshiftForm() == FORM_BATTLESTANCE || (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARRIOR_ARMS && l_Player->GetShapeshiftForm() == FORM_DEFENSIVESTANCE))
        {
            Item const* l_Weapon = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, cleanDamage->attackType == WeaponAttackType::BaseAttack ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_OFFHAND);
            float l_WeaponSpeed = (l_Weapon ? l_Weapon->GetTemplate()->Delay : BASE_ATTACK_TIME) / 1000.0f;

            float l_RageGain = l_WeaponSpeed * 1.75f;

            if (l_Player->GetShapeshiftForm() == FORM_BEAR)
            {
                l_RageGain = 5.00f;
            }
            else if (l_Player->GetSpecializationId() == SPEC_WARRIOR_ARMS)
            {
                if (l_Player->GetShapeshiftForm() == FORM_BATTLESTANCE)
                {
                    if (cleanDamage->hitOutCome & MELEE_HIT_CRIT)
                        l_RageGain *= frand(7.4375, 7.875);
                    else
                        l_RageGain *= 3.40f;
                }
                else
                {
                    if (cleanDamage->hitOutCome & MELEE_HIT_CRIT)
                        l_RageGain *= 3.40f;
                    else
                        l_RageGain *= 1.60f;
                }
            }
            else
            {
                l_RageGain *= 2.00f;
                if (cleanDamage->attackType == WeaponAttackType::OffAttack)
                    l_RageGain *= 0.5f;
            }

            if (!HasAura(147297)) ///< Glyph of the Raging Whirlwind 146968 (Aura)
                RewardRage(l_RageGain);
        }
    }

    if (damagetype != NODAMAGE && (damage || (cleanDamage && cleanDamage->absorbed_damage)))
    {
        if (victim != this && victim->IsPlayer()) // does not support creature push_back
        {
            if (damagetype != DOT || (spellProto && spellProto->IsChanneled()))
            {
                uint32 const l_SpellTypesToInterrupt[2] = { CurrentSpellTypes::CURRENT_CHANNELED_SPELL, CurrentSpellTypes::CURRENT_GENERIC_SPELL };

                for (int l_I = 0; l_I < 2; l_I++)
                {
                    if (Spell* spell = victim->m_currentSpells[l_SpellTypesToInterrupt[l_I]])
                    {
                        if (spell->getState() == SPELL_STATE_PREPARING)
                        {
                            uint32 interruptFlags = spell->m_spellInfo->InterruptFlags;
                            if (interruptFlags & SPELL_INTERRUPT_FLAG_ABORT_ON_DMG)
                                victim->InterruptNonMeleeSpells(false);
                        }
                    }
                }
            }
        }
    }

    uint32 health = victim->GetHealth();

    // duel ends when player has 1 or less hp
    bool duel_hasEnded = false;
    bool duel_wasMounted = false;
    if (victim->IsPlayer() && victim->ToPlayer()->m_Duel && damage >= (health-1))
    {
        // prevent kill only if killed in duel and killed by opponent or opponent controlled creature
        if (victim->ToPlayer()->m_Duel->opponent == this || victim->ToPlayer()->m_Duel->opponent->GetGUID() == GetOwnerGUID() || victim == this)
            damage = health - 1;

        duel_hasEnded = true;
    }
    else if (victim->IsVehicle() && damage >= (health-1) && victim->GetCharmer() && victim->GetCharmer()->IsPlayer())
    {
        Player* victimRider = victim->GetCharmer()->ToPlayer();

        if (victimRider && victimRider->m_Duel && victimRider->m_Duel->isMounted)
        {
            // prevent kill only if killed in duel and killed by opponent or opponent controlled creature
            if (victimRider->m_Duel->opponent == this || victimRider->m_Duel->opponent->GetGUID() == GetCharmerGUID())
                damage = health - 1;

            duel_wasMounted = true;
            duel_hasEnded = true;
        }
    }

    if (IsPlayer() && this != victim)
    {
        Player* killer = ToPlayer();

        // in bg, count dmg if victim is also a player
        if (victim->IsPlayer())
            if (Battleground* bg = killer->GetBattleground())
                bg->UpdatePlayerScore(killer, NULL, SCORE_DAMAGE_DONE, damage);

        killer->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE, damage, 0, 0, victim);
        killer->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT, damage);
    }
    else if (GetTypeId() == TYPEID_UNIT && this != victim && isPet())
    {
        if (GetOwner() && GetOwner()->IsPlayer())
        {
            Player* killerOwner = GetOwner()->ToPlayer();

            if (victim->IsPlayer())
                if (Battleground* bg = killerOwner->GetBattleground())
                    bg->UpdatePlayerScore(killerOwner, NULL, SCORE_DAMAGE_DONE, damage);
        }
    }

    if (victim->IsPlayer())
        victim->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED, damage);
    else if (!victim->IsControlledByPlayer() || victim->IsVehicle())
    {
        if (!victim->ToCreature()->hasLootRecipient())
            victim->ToCreature()->SetLootRecipient(this);

        if (IsControlledByPlayer())
            victim->ToCreature()->LowerPlayerDamageReq(health < damage ?  health : damage);
    }

    if (health <= damage)
    {
        if (victim->IsPlayer() && victim != this)
        {
            victim->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED, health);

            // call before auras are removed
            if (Player* killer = GetCharmerOrOwnerPlayerOrPlayerItself())
                killer->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL, 1, 0, 0, victim);
        }

        Kill(victim, durabilityLoss, spellProto ? spellProto : NULL);
    }
    else
    {
        if (victim->IsPlayer())
            victim->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED, damage);

        victim->ModifyHealth(- (int32)damage);

        if (damagetype == DIRECT_DAMAGE || damagetype == SPELL_DIRECT_DAMAGE)
            victim->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_DIRECT_DAMAGE, spellProto ? spellProto->Id : 0);

        if (victim->GetTypeId() != TYPEID_PLAYER)
            victim->AddThreat(this, float(damage), damageSchoolMask, spellProto);
        else                                                // victim is a player
        {
            // random durability for items (HIT TAKEN)
            if (roll_chance_f(sWorld->getRate(RATE_DURABILITY_LOSS_DAMAGE)))
            {
                EquipmentSlots slot = EquipmentSlots(urand(0, EQUIPMENT_SLOT_END-1));
                victim->ToPlayer()->DurabilityPointLossForEquipSlot(slot);
            }
        }

        /// Item – Warrior WoD Warrior 4P Bonus (165670) now generate Rage from damage taken.
        /// Each 1k of damage taken will generate 1 Rage, up to 5 Rage per hit.
        if (damage && this != victim && victim->IsPlayer() && victim->getPowerType() == POWER_RAGE)
        {
            if (Aura* l_Pvp4pBonus = victim->GetAura(165670))
            {
                if (damage > uint32(l_Pvp4pBonus->GetEffect(0)->GetAmount()))
                {
                    int32 l_Bp = floor(damage / l_Pvp4pBonus->GetEffect(0)->GetAmount());
                    if (l_Bp > 5)
                        l_Bp = 5;

                    victim->EnergizeBySpell(victim, 165672, l_Bp * victim->GetPowerCoeff(POWER_RAGE), POWER_RAGE);
                }
            }
        }

        /// Mastery: Primal Tenacity - 155783
        /// Proc just from physical attack
        if (damage && cleanDamage && damagetype == DIRECT_DAMAGE && this != victim && victim->IsPlayer() && victim->getClass() == CLASS_DRUID && victim->ToPlayer()->GetSpecializationId() == SPEC_DRUID_GUARDIAN)
        {
            /// Apply absorb just in case if this damage not absorbed
            if (cleanDamage->absorbed_damage == 0)
            {
                float l_Mastery = victim->ToPlayer()->GetFloatValue(EPlayerFields::PLAYER_FIELD_MASTERY) * 1.5f;
                int32 l_Value = CalculatePct(damage, l_Mastery);

                victim->CastCustomSpell(victim, 155783, nullptr, &l_Value, nullptr, true);
            }
        }

        if (IsPlayer())
        {
            // random durability for items (HIT DONE)
            if (roll_chance_f(sWorld->getRate(RATE_DURABILITY_LOSS_DAMAGE)))
            {
                EquipmentSlots slot = EquipmentSlots(urand(0, EQUIPMENT_SLOT_END-1));
                ToPlayer()->DurabilityPointLossForEquipSlot(slot);
            }
        }

        if (damagetype != NODAMAGE && damage)
        {
            if (victim != this && victim->IsPlayer()) // does not support creature push_back
            {
                if (damagetype != DOT)
                {
                    uint32 const l_SpellTypesToInterrupt[2] = { CurrentSpellTypes::CURRENT_CHANNELED_SPELL, CurrentSpellTypes::CURRENT_GENERIC_SPELL };

                    for (int l_I = 0; l_I < 2; l_I++)
                    {
                        if (Spell* spell = victim->m_currentSpells[l_SpellTypesToInterrupt[l_I]])
                        {
                            if (spell->getState() == SPELL_STATE_PREPARING)
                            {
                                uint32 interruptFlags = spell->m_spellInfo->InterruptFlags;
                                if (interruptFlags & SPELL_INTERRUPT_FLAG_ABORT_ON_DMG)
                                    victim->InterruptNonMeleeSpells(false);
                                else if (interruptFlags & SPELL_INTERRUPT_FLAG_PUSH_BACK)
                                    spell->Delayed();
                            }
                        }
                    }
                }
            }
        }

        // last damage from duel opponent
        if (duel_hasEnded)
        {
            Player* he = duel_wasMounted ? victim->GetCharmer()->ToPlayer() : victim->ToPlayer();

            ASSERT(he && he->m_Duel);

            if (duel_wasMounted) // In this case victim==mount
                victim->SetHealth(1);
            else
                he->SetHealth(1);

            he->m_Duel->opponent->CombatStopWithPets(true);
            he->CombatStopWithPets(true);

            he->CastSpell(he, 7267, true);                  // beg
            he->DuelComplete(DUEL_WON);
        }
    }

//     if (damage > 10000 && spellProto != nullptr && IsPlayer() && (ToPlayer()->InBattleground() || ToPlayer()->InArena()) && victim->IsPlayer())
//     {
//         auto l_Row = sChrSpecializationsStore.LookupEntry(ToPlayer()->GetSpecializationId(ToPlayer()->GetActiveSpec()));
//         std::string l_SpeName = "";
//         if (l_Row != nullptr)
//             l_SpeName = l_Row->specializationName;
//
//         sReporter->Report(MS::Reporting::MakeReport<MS::Reporting::Opcodes::BattlegroundDealDamageWatcher>::Craft
//                           (
//                           GetGUIDLow(),
//                           sWorld->GetRealmName(),
//                           spellProto->Id,
//                           damage,
//                           getClass(),
//                           getRace(),
//                           l_SpeName
//                           ));
//     }

    if (getClass() == CLASS_WARLOCK)
    {
        DamageDone* dmgDone = nullptr;

        if (spellProto)
            dmgDone = new DamageDone(damage, getMSTime(), spellProto->Id);
        else
            dmgDone = new DamageDone(damage, getMSTime(), 0);

        SetDamageDone(dmgDone);
    }

    return damage;
}


/// Last Update 6.1.2
uint32 Unit::CalcStaggerDamage(Player* p_Victim, uint32 p_Damage, SpellSchoolMask p_DamageSchoolMask, SpellInfo const* p_SpellProto)
{
    if (p_Victim->GetSpecializationId() != SPEC_MONK_BREWMASTER)
        return p_Damage;

    /// Stance of the Sturdy Ox
    if (!p_Victim->HasAura(115069))
        return p_Damage;

    if (p_Damage <= 0)
        return p_Damage;

    float l_Stagger = 0.80f;
    /// Mastery increases stagger amount - Mastery: Elusive Brawler
    if (p_Victim->HasAura(117906))
    {
        float l_Mastery = (p_Victim->GetFloatValue(PLAYER_FIELD_MASTERY) * 0.625f) / 100.0f;
        l_Stagger -= l_Mastery;
    }

    /// Brewmaster Training : Your Fortifying Brew also increase stagger amount by 20%
    if (p_Victim->HasSpell(120954) && p_Victim->HasAura(117967))
        l_Stagger -= 0.20f;                             ///< last update 6.1.2 19711
    /// Shuffle also increase stagger amount by 10%
    if (p_Victim->HasAura(115307))
        l_Stagger -= 0.10f;                             ///< last update 6.1.2 19711
    /// Staggering increase stagger amount by 6%
    if (p_Victim->HasAura(138233))
        l_Stagger -= 0.06f;                             ///< last update 5.4.0 17153

    if (l_Stagger < 0.0f)
        l_Stagger = 0.0f;

    /// If it's not a physical attack, such that 30% of your normal stagger amount works against magic damage.
    if (!(!p_SpellProto || ((p_SpellProto->DmgClass == SPELL_DAMAGE_CLASS_RANGED || p_SpellProto->DmgClass == SPELL_DAMAGE_CLASS_MELEE) && p_DamageSchoolMask & SPELL_SCHOOL_MASK_NORMAL)))
        l_Stagger += CalculatePct(1.0f - l_Stagger, 70);

    int32 l_Bp = CalculatePct(p_Damage, ((1.0f - l_Stagger) * 100.0f));

    if (l_Stagger == 0.0f)
        l_Bp = p_Damage;

    uint32 l_SpellId = 0;
    uint32 l_TicksNumber = 10;

    AuraEffect* l_AurEff = p_Victim->GetAuraEffect(LIGHT_STAGGER, 0, p_Victim->GetGUID());
    if (!l_AurEff)
        l_AurEff = p_Victim->GetAuraEffect(MODERATE_STAGGER, 0, p_Victim->GetGUID());
    if (!l_AurEff)
        l_AurEff = p_Victim->GetAuraEffect(HEAVY_STAGGER, 0, p_Victim->GetGUID());

    /// Add remaining ticks to damage done
    if (l_AurEff)
        l_Bp += l_AurEff->GetAmount() * (l_TicksNumber - l_AurEff->GetTickNumber());

    if (l_Bp < int32(p_Victim->CountPctFromMaxHealth(3)))
        l_SpellId = LIGHT_STAGGER;
    else if (l_Bp < int32(p_Victim->CountPctFromMaxHealth(6)))
        l_SpellId = MODERATE_STAGGER;
    else
        l_SpellId = HEAVY_STAGGER;

    l_Bp /= l_TicksNumber;

    p_Victim->RemoveAura(LIGHT_STAGGER);
    p_Victim->RemoveAura(MODERATE_STAGGER);
    p_Victim->RemoveAura(HEAVY_STAGGER);
    p_Victim->CastCustomSpell(p_Victim, l_SpellId, &l_Bp, NULL, NULL, true);

    return p_Damage *= l_Stagger;
}

void Unit::CastStop(uint32 except_spellid)
{
    for (uint32 i = CURRENT_FIRST_NON_MELEE_SPELL; i < CURRENT_MAX_SPELL; i++)
        if (m_currentSpells[i] && m_currentSpells[i]->m_spellInfo->Id != except_spellid)
            InterruptSpell(CurrentSpellTypes(i), false);
}

void Unit::CastSpell(SpellCastTargets const& targets, SpellInfo const* spellInfo, CustomSpellValues const* value, TriggerCastFlags triggerFlags, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster, float periodicDamageModifier /* = 0.0f*/)
{
    if (!spellInfo)
        return;

    // TODO: this is a workaround - not needed anymore, but required for some scripts :(
    if (!originalCaster && triggeredByAura)
        originalCaster = triggeredByAura->GetCasterGUID();

    // Override spell Id
    if (!spellInfo->OverrideSpellList.empty() && IsPlayer())
    {
        for (auto itr : spellInfo->OverrideSpellList)
        {
            if (ToPlayer()->HasSpell(itr))
            {
                SpellInfo const* overrideSpellInfo = sSpellMgr->GetSpellInfo(itr);
                if (overrideSpellInfo)
                    spellInfo = overrideSpellInfo;
                break;
            }
        }
    }

    Unit::AuraEffectList swaps = GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
    Unit::AuraEffectList const& swaps2 = GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2);
    if (!swaps2.empty())
        swaps.insert(swaps.end(), swaps2.begin(), swaps2.end());

    if (!swaps.empty())
    {
        for (Unit::AuraEffectList::const_iterator itr = swaps.begin(); itr != swaps.end(); ++itr)
        {
            if ((*itr)->IsAffectingSpell(spellInfo))
            {
                if (SpellInfo const* newInfo = sSpellMgr->GetSpellInfo((*itr)->GetAmount()))
                    spellInfo = newInfo;
                break;
            }
        }
    }

    Spell* spell = new Spell(this, spellInfo, triggerFlags, originalCaster);

    if (value)
    {
        for (CustomSpellValues::const_iterator itr = value->begin(); itr != value->end(); ++itr)
            spell->SetSpellValue(itr->first, itr->second);

        spell->SetCustomCritChance(value->GetCustomCritChance());
    }

    spell->m_CastItem = castItem;
    spell->SetPeriodicDamageModifier(periodicDamageModifier);
    spell->prepare(&targets, triggeredByAura);
}

void Unit::CastSpell(Unit* victim, uint32 spellId, bool triggered, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster, float periodicDamageModifier)
{
    CastSpell(victim, spellId, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, castItem, triggeredByAura, originalCaster, periodicDamageModifier);
}

void Unit::CastSpell(Unit* victim, uint32 spellId, TriggerCastFlags triggerFlags /*= TRIGGER_NONE*/, Item* castItem /*= NULL*/, AuraEffect const* triggeredByAura /*= NULL*/, uint64 originalCaster /*= 0*/, float periodicDamageModifier /* = 0.0f */)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    CastSpell(victim, spellInfo, triggerFlags, castItem, triggeredByAura, originalCaster, periodicDamageModifier);
}

void Unit::CastSpell(Unit* victim, SpellInfo const* spellInfo, bool triggered, Item* castItem/*= NULL*/, AuraEffect const* triggeredByAura /*= NULL*/, uint64 originalCaster /*= 0*/)
{
    CastSpell(victim, spellInfo, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, castItem, triggeredByAura, originalCaster);
}

void Unit::CastSpell(Unit* victim, SpellInfo const* spellInfo, TriggerCastFlags triggerFlags, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster, float periodicDamageModifier /* = 0.0f */)
{
    SpellCastTargets targets;
    targets.SetUnitTarget(victim);
    CastSpell(targets, spellInfo, NULL, triggerFlags, castItem, triggeredByAura, originalCaster, periodicDamageModifier);
}

void Unit::CastCustomSpell(Unit* target, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    CustomSpellValues values;
    if (bp0)
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, *bp0);
    if (bp1)
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, *bp1);
    if (bp2)
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, *bp2);
    CastCustomSpell(spellId, values, target, triggered, castItem, triggeredByAura, originalCaster);
}

void Unit::CastCustomSpell(Unit* target, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, int32 const* bp3, int32 const* bp4, int32 const* bp5, bool triggered, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    CustomSpellValues values;
    if (bp0)
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, *bp0);
    if (bp1)
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, *bp1);
    if (bp2)
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, *bp2);
    if (bp3)
        values.AddSpellMod(SPELLVALUE_BASE_POINT3, *bp3);
    if (bp4)
        values.AddSpellMod(SPELLVALUE_BASE_POINT4, *bp4);
    if (bp5)
        values.AddSpellMod(SPELLVALUE_BASE_POINT5, *bp5);
    CastCustomSpell(spellId, values, target, triggered, castItem, triggeredByAura, originalCaster);
}

void Unit::CastCustomSpell(uint32 spellId, SpellValueMod mod, int32 value, Unit* target, bool triggered, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    CustomSpellValues values;
    values.AddSpellMod(mod, value);
    CastCustomSpell(spellId, values, target, triggered, castItem, triggeredByAura, originalCaster);
}

void Unit::CastCustomSpell(uint32 spellId, CustomSpellValues const& value, Unit* victim, bool triggered, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    SpellCastTargets targets;
    targets.SetUnitTarget(victim);

    CastSpell(targets, spellInfo, &value, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, castItem, triggeredByAura, originalCaster);
}

void Unit::CastCustomSpell(float x, float y, float z, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    CustomSpellValues values;
    if (bp0)
        values.AddSpellMod(SPELLVALUE_BASE_POINT0, *bp0);
    if (bp1)
        values.AddSpellMod(SPELLVALUE_BASE_POINT1, *bp1);
    if (bp2)
        values.AddSpellMod(SPELLVALUE_BASE_POINT2, *bp2);

    SpellCastTargets targets;
    targets.SetDst(x, y, z, GetOrientation());

    CastSpell(targets, spellInfo, &values, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, castItem, triggeredByAura, originalCaster);
}

void Unit::CastSpell(Position const p_Pos, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem /*= nullptr*/, AuraEffect const* p_AurEff /*= nullptr*/, uint64 p_OriginalCaster /*= 0*/)
{
    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(p_SpellID);
    if (!l_SpellInfo)
        return;

    SpellCastTargets l_Targets;
    l_Targets.SetDst(p_Pos.m_positionX, p_Pos.m_positionY, p_Pos.m_positionZ, p_Pos.m_orientation);

    CastSpell(l_Targets, l_SpellInfo, nullptr, p_Triggered ? TriggerCastFlags::TRIGGERED_FULL_MASK : TriggerCastFlags::TRIGGERED_NONE, p_CastItem, p_AurEff, p_OriginalCaster);
}

void Unit::CastSpell(SpellDestination const* p_Dest, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem /*= nullptr*/, AuraEffect const* p_AurEff /*= nullptr*/, uint64 p_OriginalCaster /*= 0*/)
{
    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(p_SpellID);
    if (!l_SpellInfo)
        return;

    SpellCastTargets l_Targets;
    l_Targets.SetDst(p_Dest->_position.m_positionX, p_Dest->_position.m_positionY, p_Dest->_position.m_positionZ, p_Dest->_position.m_orientation);

    CastSpell(l_Targets, l_SpellInfo, nullptr, p_Triggered ? TriggerCastFlags::TRIGGERED_FULL_MASK : TriggerCastFlags::TRIGGERED_NONE, p_CastItem, p_AurEff, p_OriginalCaster);
}

void Unit::CastSpell(WorldLocation const* p_Loc, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem /*= nullptr*/, AuraEffect const* p_AurEff /*= nullptr*/, uint64 p_OriginalCaster /*= 0*/)
{
    CastSpell(p_Loc->m_positionX, p_Loc->m_positionY, p_Loc->m_positionZ, p_SpellID, p_Triggered, p_CastItem, p_AurEff, p_OriginalCaster);
}

void Unit::CastSpell(uint32 p_LocEntry, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem /*= nullptr*/, AuraEffect const* p_AurEff /*= nullptr*/, uint64 p_OriginalCaster /*= 0*/)
{
    WorldSafeLocsEntry const* l_Loc = sWorldSafeLocsStore.LookupEntry(p_LocEntry);
    if (l_Loc == nullptr)
        return;

    CastSpell(l_Loc->x, l_Loc->y, l_Loc->z, p_SpellID, p_Triggered, p_CastItem, p_AurEff, p_OriginalCaster);
}

void Unit::CastSpell(G3D::Vector3 p_Pos, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem /*= nullptr*/, AuraEffect const* p_AurEff /*= nullptr*/, uint64 p_OriginalCaster /*= 0*/)
{
    CastSpell(p_Pos.x, p_Pos.y, p_Pos.z, p_SpellID, p_Triggered, p_CastItem, p_AurEff, p_OriginalCaster);
}

void Unit::CastSpell(float x, float y, float z, uint32 spellId, bool triggered, Item* castItem, AuraEffect const* triggeredByAura, uint64 originalCaster)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    SpellCastTargets targets;
    targets.SetDst(x, y, z, GetOrientation());

    CastSpell(targets, spellInfo, NULL, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, castItem, triggeredByAura, originalCaster);
}

void Unit::CastSpell(GameObject* go, uint32 spellId, bool triggered, Item* castItem, AuraEffect* triggeredByAura, uint64 originalCaster)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return;

    SpellCastTargets targets;
    targets.SetGOTarget(go);

    CastSpell(targets, spellInfo, NULL, triggered ? TRIGGERED_FULL_MASK : TRIGGERED_NONE, castItem, triggeredByAura, originalCaster);
}

void Unit::CastSpell(Item* p_ItemTarget, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem, AuraEffect* p_TriggeredByAura, uint64 p_OriginalCaster)
{
    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(p_SpellID);
    if (!l_SpellInfo)
        return;

    SpellCastTargets l_Targets;
    l_Targets.SetItemTarget(p_ItemTarget);

    CastSpell(l_Targets, l_SpellInfo, nullptr, p_Triggered ? TriggerCastFlags::TRIGGERED_FULL_MASK : TriggerCastFlags::TRIGGERED_NONE, p_CastItem, p_TriggeredByAura, p_OriginalCaster);
}

// Obsolete func need remove, here only for comotability vs another patches
uint32 Unit::SpellNonMeleeDamageLog(Unit* victim, uint32 spellID, uint32 damage)
{
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
    SpellNonMeleeDamage damageInfo(this, victim, spellInfo->Id, spellInfo->SchoolMask);
    damage = SpellDamageBonusDone(victim, spellInfo, damage, EFFECT_0, SPELL_DIRECT_DAMAGE);
    damage = victim->SpellDamageBonusTaken(this, spellInfo, damage, SPELL_DIRECT_DAMAGE);

    CalculateSpellDamageTaken(&damageInfo, damage, spellInfo);
    DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);
    SendSpellNonMeleeDamageLog(&damageInfo);
    DealSpellDamage(&damageInfo, true);
    return damageInfo.damage;
}

void Unit::CalculateSpellDamageTaken(SpellNonMeleeDamage* damageInfo, int32 damage, SpellInfo const* spellInfo, WeaponAttackType attackType, bool crit)
{
    if (damage < 0)
        return;

    Unit* victim = damageInfo->target;
    if (!victim || !victim->isAlive())
        return;

    // WoD: Tanks now take 25% additional damage while engaged in PvP combat.
    if (IsPlayer() && victim->IsPlayer() && victim->ToPlayer()->IsActiveSpecTankSpec())
        damage += CalculatePct(damage, 25.0f);

    // WoD: Apply factor on damages depending on creature level and expansion
    if ((IsPlayer() || IsPetGuardianStuff()) && victim->GetTypeId() == TYPEID_UNIT)
        damage *= CalculateDamageDealtFactor(this, victim->ToCreature());
    else if (GetTypeId() == TYPEID_UNIT && (victim->IsPlayer() || victim->IsPetGuardianStuff()))
        damage *= CalculateDamageTakenFactor(victim, ToCreature());

    /// Apply Versatility damage bonus taken
    if (victim->GetSpellModOwner())
        damage -= CalculatePct(damage, victim->GetSpellModOwner()->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_TAKEN) + victim->GetSpellModOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT));

    SpellSchoolMask damageSchoolMask = SpellSchoolMask(damageInfo->schoolMask);

    if (IsDamageReducedByArmor(damageSchoolMask, spellInfo))
        damage = CalcArmorReducedDamage(victim, damage, spellInfo, attackType);

    bool blocked = false;
    // Per-school calc
    switch (spellInfo->DmgClass)
    {
        // Melee and Ranged Spells
        case SPELL_DAMAGE_CLASS_RANGED:
        case SPELL_DAMAGE_CLASS_MELEE:
        {
            // Physical Damage
            if (damageSchoolMask & SPELL_SCHOOL_MASK_NORMAL)
            {
                // Get blocked status
                blocked = isSpellBlocked(victim, spellInfo, attackType);
            }

            if (crit)
            {
                damageInfo->HitInfo |= SPELL_HIT_TYPE_CRIT;
                damage = MeleeCriticalDamageBonus(spellInfo, damage, victim, attackType);
            }

            // Spell weapon based damage CAN BE crit & blocked at same time
            if (blocked)
            {
                // double blocked amount if block is critical
                uint32 value = victim->GetBlockPercent();
                if (victim->isBlockCritical())
                    value *= 2; // double blocked percent
                damageInfo->blocked = CalculatePct(damage, value);
                damage -= damageInfo->blocked;
            }

            if (spellInfo->IsAffectedByResilience())
                ApplyResilience(victim, &damage);

            break;
        }
        // Magical Attacks
        case SPELL_DAMAGE_CLASS_NONE:
        case SPELL_DAMAGE_CLASS_MAGIC:
        {
            /// Magic Damage can be block only by Paladin Proteccion
            if (victim->HasAura(152261)) ///< Holy Shield
            {
                /// Get blocked status
                blocked = isSpellBlocked(victim, spellInfo, attackType);
            }

            /// If crit add critical bonus
            if (crit)
            {
                damageInfo->HitInfo |= SPELL_HIT_TYPE_CRIT;
                damage = SpellCriticalDamageBonus(spellInfo, damage, victim);
            }

            if (blocked)
            {
                // double blocked amount if block is critical
                uint32 value = victim->GetBlockPercent();
                if (victim->isBlockCritical())
                    value *= 2; // double blocked percent
                damageInfo->blocked = CalculatePct(damage, value);
                damage -= damageInfo->blocked;
            }

            if (spellInfo->IsAffectedByResilience())
                ApplyResilience(victim, &damage);
            break;
        }
        default:
            break;
    }

    if (blocked && victim->IsPlayer())
        sScriptMgr->OnPlayerBlock(victim->ToPlayer(), this);
    // Calculate absorb resist
    if (damage > 0)
    {
        damageInfo->schoolMask = CalcAbsorbResist(victim, damageSchoolMask, SPELL_DIRECT_DAMAGE, damage, &damageInfo->absorb, &damageInfo->resist, spellInfo);
        damage -= damageInfo->absorb + damageInfo->resist;
    }
    else
        damage = 0;

    damageInfo->damage = damage;
}

void Unit::DealSpellDamage(SpellNonMeleeDamage* damageInfo, bool durabilityLoss)
{
    if (damageInfo == 0)
        return;

    Unit* victim = damageInfo->target;

    if (!victim)
        return;

    if (!victim->isAlive() || victim->HasUnitState(UNIT_STATE_IN_FLIGHT) || (victim->GetTypeId() == TYPEID_UNIT && victim->ToCreature()->IsInEvadeMode()))
        return;

    SpellInfo const* spellProto = sSpellMgr->GetSpellInfo(damageInfo->SpellID);
    if (spellProto == NULL)
        return;

    // Call default DealDamage
    CleanDamage cleanDamage(damageInfo->cleanDamage, damageInfo->absorb, WeaponAttackType::BaseAttack, MELEE_HIT_NORMAL);
    DealDamage(victim, damageInfo->damage, &cleanDamage, SPELL_DIRECT_DAMAGE, SpellSchoolMask(damageInfo->schoolMask), spellProto, durabilityLoss);
}

// TODO for melee need create structure as in
void Unit::CalculateMeleeDamage(Unit* victim, uint32 damage, CalcDamageInfo* damageInfo, WeaponAttackType attackType)
{
    damageInfo->attacker         = this;
    damageInfo->target           = victim;
    damageInfo->damageSchoolMask = GetMeleeDamageSchoolMask();
    damageInfo->attackType       = attackType;
    damageInfo->damage           = 0;
    damageInfo->cleanDamage      = 0;
    damageInfo->absorb           = 0;
    damageInfo->resist           = 0;
    damageInfo->blocked_amount   = 0;

    damageInfo->TargetState      = 0;
    damageInfo->HitInfo          = 0;
    damageInfo->procAttacker     = PROC_FLAG_NONE;
    damageInfo->procVictim       = PROC_FLAG_NONE;
    damageInfo->procEx           = PROC_EX_NONE;
    damageInfo->hitOutCome       = MELEE_HIT_EVADE;

    if (!victim)
        return;

    if (!isAlive() || !victim->isAlive())
        return;

    // Select HitInfo/procAttacker/procVictim flag based on attack type
    switch (attackType)
    {
        case WeaponAttackType::BaseAttack:
            damageInfo->procAttacker = PROC_FLAG_DONE_MELEE_AUTO_ATTACK | PROC_FLAG_DONE_MAINHAND_ATTACK;
            damageInfo->procVictim   = PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK;
            break;
        case WeaponAttackType::OffAttack:
            damageInfo->procAttacker = PROC_FLAG_DONE_MELEE_AUTO_ATTACK | PROC_FLAG_DONE_OFFHAND_ATTACK;
            damageInfo->procVictim   = PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK;
            damageInfo->HitInfo      = HITINFO_OFFHAND;
            break;
        default:
            return;
    }

    // Physical Immune check
    if (damageInfo->target->IsImmunedToDamage(SpellSchoolMask(damageInfo->damageSchoolMask)))
    {
       damageInfo->HitInfo       |= HITINFO_NORMALSWING;
       damageInfo->TargetState    = VICTIMSTATE_IS_IMMUNE;

       damageInfo->procEx        |= PROC_EX_IMMUNE;
       damageInfo->damage         = 0;
       damageInfo->cleanDamage    = 0;
       return;
    }

    damage += CalculateDamage(damageInfo->attackType, false, true);
    // Add melee damage bonus
    damage = MeleeDamageBonusDone(damageInfo->target, damage, damageInfo->attackType);
    damage = damageInfo->target->MeleeDamageBonusTaken(this, damage, damageInfo->attackType);

    // WoD: Tanks now take 25% additional damage while engaged in PvP combat.
    if (IsPlayer() && victim->IsPlayer() && victim->ToPlayer()->IsActiveSpecTankSpec())
        damage += CalculatePct(damage, 25.0f);

    // WoD: Apply factor on damages depending on creature level and expansion
    if ((IsPlayer() || IsPetGuardianStuff()) && victim->GetTypeId() == TYPEID_UNIT)
        damage *= CalculateDamageDealtFactor(this, victim->ToCreature());
    else if (GetTypeId() == TYPEID_UNIT && (victim->IsPlayer() || victim->IsPetGuardianStuff()))
        damage *= CalculateDamageTakenFactor(victim, ToCreature());

    // Calculate armor reduction
    if (IsDamageReducedByArmor((SpellSchoolMask)(damageInfo->damageSchoolMask)))
    {
        damageInfo->damage = CalcArmorReducedDamage(damageInfo->target, damage, NULL, damageInfo->attackType);
        damageInfo->cleanDamage += damage - damageInfo->damage;
    }
    else
        damageInfo->damage = damage;

    damageInfo->hitOutCome = RollMeleeOutcomeAgainst(damageInfo->target, damageInfo->attackType);

    switch (damageInfo->hitOutCome)
    {
        case MELEE_HIT_EVADE:
            damageInfo->HitInfo        |= HITINFO_MISS | HITINFO_SWINGNOHITSOUND;
            damageInfo->TargetState     = VICTIMSTATE_EVADES;
            damageInfo->procEx         |= PROC_EX_EVADE;
            damageInfo->damage = 0;
            damageInfo->cleanDamage = 0;
            return;
        case MELEE_HIT_MISS:
            damageInfo->HitInfo        |= HITINFO_MISS;
            damageInfo->TargetState     = VICTIMSTATE_INTACT;
            damageInfo->procEx         |= PROC_EX_MISS;
            damageInfo->damage          = 0;
            damageInfo->cleanDamage     = 0;
            break;
        case MELEE_HIT_NORMAL:
            damageInfo->TargetState     = VICTIMSTATE_HIT;
            damageInfo->procEx         |= PROC_EX_NORMAL_HIT;
            break;
        case MELEE_HIT_CRIT:
        {
            damageInfo->HitInfo        |= HITINFO_CRITICALHIT;
            damageInfo->TargetState     = VICTIMSTATE_HIT;
            damageInfo->procEx         |= PROC_EX_CRITICAL_HIT;
            damageInfo->damage          = MeleeCriticalDamageBonus(nullptr, damageInfo->damage, victim, attackType);
            break;
        }
        case MELEE_HIT_PARRY:
            damageInfo->TargetState  = VICTIMSTATE_PARRY;
            damageInfo->procEx      |= PROC_EX_PARRY;
            damageInfo->cleanDamage += damageInfo->damage;
            damageInfo->damage = 0;
            break;
        case MELEE_HIT_DODGE:
            damageInfo->TargetState  = VICTIMSTATE_DODGE;
            damageInfo->procEx      |= PROC_EX_DODGE;
            damageInfo->cleanDamage += damageInfo->damage;
            damageInfo->damage = 0;
            break;
        case MELEE_HIT_BLOCK:
            damageInfo->TargetState = VICTIMSTATE_HIT;
            damageInfo->HitInfo    |= HITINFO_BLOCK;
            damageInfo->procEx     |= PROC_EX_BLOCK | PROC_EX_NORMAL_HIT;
            // 30% damage blocked, double blocked amount if block is critical
            damageInfo->blocked_amount = CalculatePct(damageInfo->damage, damageInfo->target->isBlockCritical() ? damageInfo->target->GetBlockPercent() * 2 : damageInfo->target->GetBlockPercent());
            damageInfo->damage      -= damageInfo->blocked_amount;
            damageInfo->cleanDamage += damageInfo->blocked_amount;
            break;
        case MELEE_HIT_GLANCING:
        {
            damageInfo->HitInfo     |= HITINFO_GLANCING;
            damageInfo->TargetState  = VICTIMSTATE_HIT;
            damageInfo->procEx      |= PROC_EX_NORMAL_HIT;
            int32 leveldif = int32(victim->getLevel()) - int32(getLevel());
            if (leveldif > 3)
                leveldif = 3;
            float reducePercent = 1 - leveldif * 0.1f;
            damageInfo->cleanDamage += damageInfo->damage - uint32(reducePercent * damageInfo->damage);
            damageInfo->damage = uint32(reducePercent * damageInfo->damage);
            break;
        }
        case MELEE_HIT_CRUSHING:
            damageInfo->HitInfo     |= HITINFO_CRUSHING;
            damageInfo->TargetState  = VICTIMSTATE_HIT;
            damageInfo->procEx      |= PROC_EX_NORMAL_HIT;
            // 150% normal damage
            damageInfo->damage += (damageInfo->damage / 2);
            break;
        default:
            break;
    }

    // Always apply HITINFO_AFFECTS_VICTIM in case its not a miss
    if (!(damageInfo->HitInfo & HITINFO_MISS))
        damageInfo->HitInfo |= HITINFO_AFFECTS_VICTIM;

    int32 resilienceReduction = damageInfo->damage;
    ApplyResilience(victim, &resilienceReduction);
    resilienceReduction = damageInfo->damage - resilienceReduction;
    damageInfo->damage      -= resilienceReduction;
    damageInfo->cleanDamage += resilienceReduction;

    // only for normal weapon damage
    if (damageInfo->attackType == WeaponAttackType::BaseAttack || damageInfo->attackType == WeaponAttackType::OffAttack)
    {
        /// last update : 6.1.2 19802
        /// Blood Horror - 111397
        if (victim->HasAura(111397))
        {
            victim->CastSpell(this, 137143, true);
            victim->RemoveAurasDueToSpell(111397);
        }

    // Calculate absorb resist
    if (int32(damageInfo->damage) > 0)
    {
        damageInfo->procVictim |= PROC_FLAG_TAKEN_DAMAGE;
        // Calculate absorb & resists
        CalcAbsorbResist(damageInfo->target, SpellSchoolMask(damageInfo->damageSchoolMask), DIRECT_DAMAGE, damageInfo->damage, &damageInfo->absorb, &damageInfo->resist);

        if (damageInfo->absorb)
        {
            damageInfo->HitInfo |= (damageInfo->damage - damageInfo->absorb == 0 ? HITINFO_FULL_ABSORB : HITINFO_PARTIAL_ABSORB);
            damageInfo->procEx  |= PROC_EX_ABSORB;
        }

        if (damageInfo->resist)
            damageInfo->HitInfo |= (damageInfo->damage - damageInfo->resist == 0 ? HITINFO_FULL_RESIST : HITINFO_PARTIAL_RESIST);

        damageInfo->damage -= damageInfo->absorb + damageInfo->resist;
    }
    else // Impossible get negative result but....
        damageInfo->damage = 0;

        // Custom MoP Script - Zen Meditation - 115176
        if (Aura* zenMeditation = victim->GetAura(115176, victim->GetGUID()))
            victim->RemoveAura(115176);
    }
}

void Unit::DealMeleeDamage(CalcDamageInfo* damageInfo, bool durabilityLoss)
{
    Unit* l_Victim = damageInfo->target;

    if (!l_Victim->isAlive() || l_Victim->HasUnitState(UNIT_STATE_IN_FLIGHT) || (l_Victim->GetTypeId() == TYPEID_UNIT && l_Victim->ToCreature()->IsInEvadeMode()))
        return;

    // Hmmmm dont like this emotes client must by self do all animations
    if (damageInfo->HitInfo & HITINFO_CRITICALHIT)
        l_Victim->HandleEmoteCommand(EMOTE_ONESHOT_WOUND_CRITICAL);
    if (damageInfo->blocked_amount && damageInfo->TargetState != VICTIMSTATE_BLOCKS)
        l_Victim->HandleEmoteCommand(EMOTE_ONESHOT_PARRY_SHIELD);

    if (damageInfo->TargetState == VICTIMSTATE_PARRY)
    {
        // Get attack timers
        float offtime  = float(l_Victim->getAttackTimer(WeaponAttackType::OffAttack));
        float basetime = float(l_Victim->getAttackTimer(WeaponAttackType::BaseAttack));
        // Reduce attack time
        if (l_Victim->haveOffhandWeapon() && offtime < basetime)
        {
            float percent20 = l_Victim->GetAttackTime(WeaponAttackType::OffAttack) * 0.20f;
            float percent60 = 3.0f * percent20;
            if (offtime > percent20 && offtime <= percent60)
                l_Victim->setAttackTimer(WeaponAttackType::OffAttack, uint32(percent20));
            else if (offtime > percent60)
            {
                offtime -= 2.0f * percent20;
                l_Victim->setAttackTimer(WeaponAttackType::OffAttack, uint32(offtime));
            }
        }
        else
        {
            float percent20 = l_Victim->GetAttackTime(WeaponAttackType::BaseAttack) * 0.20f;
            float percent60 = 3.0f * percent20;
            if (basetime > percent20 && basetime <= percent60)
                l_Victim->setAttackTimer(WeaponAttackType::BaseAttack, uint32(percent20));
            else if (basetime > percent60)
            {
                basetime -= 2.0f * percent20;
                l_Victim->setAttackTimer(WeaponAttackType::BaseAttack, uint32(basetime));
            }
        }
    }

    // Call default DealDamage
    CleanDamage cleanDamage(damageInfo->cleanDamage, damageInfo->absorb, damageInfo->attackType, damageInfo->hitOutCome);
    DealDamage(l_Victim, damageInfo->damage, &cleanDamage, DIRECT_DAMAGE, SpellSchoolMask(damageInfo->damageSchoolMask), NULL, durabilityLoss);

    // If this is a creature and it attacks from behind it has a probability to daze it's victim
    if ((damageInfo->hitOutCome == MELEE_HIT_CRIT || damageInfo->hitOutCome == MELEE_HIT_CRUSHING || damageInfo->hitOutCome == MELEE_HIT_NORMAL || damageInfo->hitOutCome == MELEE_HIT_GLANCING) &&
        GetTypeId() != TYPEID_PLAYER && !ToCreature()->IsControlledByPlayer() && !l_Victim->HasInArc(M_PI, this)
        && (l_Victim->IsPlayer() || !l_Victim->ToCreature()->isWorldBoss()))
    {
        // -probability is between 0% and 40%
        // 20% base chance
        float l_Probability = 20.0f;

        // there is a newbie protection, at level 10 just 7% base chance; assuming linear function
        if (l_Victim->getLevel() < 30)
            l_Probability = 0.65f * l_Victim->getLevel() + 0.5f;

        uint32 VictimDefense = l_Victim->GetMaxSkillValueForLevel(this);
        uint32 AttackerMeleeSkill = GetMaxSkillValueForLevel();

        l_Probability *= AttackerMeleeSkill/(float)VictimDefense;

        if (l_Probability > 40.0f)
            l_Probability = 40.0f;

        if (roll_chance_f(l_Probability))
            CastSpell(l_Victim, 1604, true);
    }

    if (IsPlayer())
        ToPlayer()->CastItemCombatSpell(l_Victim, damageInfo->attackType, damageInfo->procVictim, damageInfo->procEx);

    // Do effect if any damage done to target
    if (damageInfo->damage)
    {
        // We're going to call functions which can modify content of the list during iteration over it's elements
        // Let's copy the list so we can prevent iterator invalidation
        AuraEffectList vDamageShieldsCopy(l_Victim->GetAuraEffectsByType(SPELL_AURA_DAMAGE_SHIELD));
        for (AuraEffectList::const_iterator dmgShieldItr = vDamageShieldsCopy.begin(); dmgShieldItr != vDamageShieldsCopy.end(); ++dmgShieldItr)
        {
            SpellInfo const* l_SpellProto = (*dmgShieldItr)->GetSpellInfo();
            // Damage shield can be resisted...
            if (SpellMissInfo missInfo = l_Victim->SpellHitResult(this, l_SpellProto, false))
            {
                l_Victim->SendSpellMiss(this, l_SpellProto->Id, missInfo);
                continue;
            }

            // ...or immuned
            if (IsImmunedToDamage(l_SpellProto))
            {
                l_Victim->SendSpellDamageImmune(this, l_SpellProto->Id);
                continue;
            }

            uint32 l_Damage = (*dmgShieldItr)->GetAmount();

            if (Unit* l_Caster = (*dmgShieldItr)->GetCaster())
            {
                l_Damage = l_Caster->SpellDamageBonusDone(this, l_SpellProto, l_Damage, (*dmgShieldItr)->GetEffIndex(), SPELL_DIRECT_DAMAGE);
                l_Damage = this->SpellDamageBonusTaken(l_Caster, l_SpellProto, l_Damage, SPELL_DIRECT_DAMAGE);
            }

            // No Unit::CalcAbsorbResist here - opcode doesn't send that data - this damage is probably not affected by that
            l_Victim->DealDamageMods(this, l_Damage, NULL);

            // TODO: Move this to a packet handler
            int32 l_OverKill = int32(l_Damage) - int32(GetHealth());

            WorldPacket l_Data(SMSG_SPELL_DAMAGE_SHIELD, (2 * (16 + 2)) + 4 + 4 + 4 + 4 + 4 + 1);
            l_Data.appendPackGUID(l_Victim->GetGUID());             ///< Defender
            l_Data.appendPackGUID(GetGUID());                       ///< Attacker
            l_Data << uint32(l_SpellProto->Id);                     ///< SpellID
            l_Data << uint32(l_Damage);                             ///< TotalDamage
            l_Data << uint32(l_OverKill > 0 ? l_OverKill : 0);      ///< OverKill
            l_Data << uint32(l_SpellProto->SchoolMask);             ///< SchoolMask
            l_Data << uint32(0);                                    ///< LogAbsorbed => FIX ME: Send resisted damage, both fully resisted and partly resisted

            l_Data.WriteBit(false);                                 ///< HasLogData
            l_Data.FlushBits();

            l_Victim->SendMessageToSet(&l_Data, true);
            l_Victim->DealDamage(this, l_Damage, 0, SPELL_DIRECT_DAMAGE, l_SpellProto->GetSchoolMask(), l_SpellProto, true);
        }
    }
}

void Unit::HandleEmoteCommand(uint32 p_EmoteId)
{
    EmotesEntry const* l_EmoteInfo = sEmotesStore.LookupEntry(p_EmoteId);
    if (!l_EmoteInfo)
    {
        SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
        return;
    }

    if (!isAlive())
        return;

    switch (l_EmoteInfo->EmoteType)
    {
        case EmoteTypes::OneStep:
        {
            WorldPacket l_Data(SMSG_EMOTE, 4 + 2 + 16);
            l_Data.appendPackGUID(GetGUID());
            l_Data << uint32(p_EmoteId);
            SendMessageToSet(&l_Data, true);
            break;
        }
        case EmoteTypes::EmoteLoop:
        case EmoteTypes::StateLoop:
            SetUInt32Value(UNIT_FIELD_EMOTE_STATE, p_EmoteId);
            break;
        default:
            break;
    }
}

bool Unit::IsDamageReducedByArmor(SpellSchoolMask schoolMask, SpellInfo const* spellInfo, uint8 effIndex)
{
    /// spells with SPELL_SCHOOL_MASK_ALL can't be reduced by armor
    if (schoolMask == SPELL_SCHOOL_MASK_ALL)
        return false;
    // only physical spells damage gets reduced by armor
    if ((schoolMask & SPELL_SCHOOL_MASK_NORMAL) == 0)
        return false;
    if (spellInfo)
    {
        /// Chaos spells, even if changing school mask, don't get reduced by armor
        if (spellInfo->SchoolMask == SPELL_SCHOOL_MASK_ALL)
            return false;

        // there are spells with no specific attribute but they have "ignores armor" in tooltip
        if (spellInfo->AttributesCu & SPELL_ATTR0_CU_IGNORE_ARMOR)
            return false;

        // bleeding effects are not reduced by armor
        if (effIndex != SpellEffIndex::MAX_EFFECTS)
        {
            if (spellInfo->Effects[effIndex].ApplyAuraName == SPELL_AURA_PERIODIC_DAMAGE ||
                spellInfo->Effects[effIndex].Effect == SPELL_EFFECT_SCHOOL_DAMAGE)
                if (spellInfo->GetEffectMechanicMask(effIndex) & (1<<MECHANIC_BLEED))
                    return false;
        }

        for (uint8 i = 0; i < SpellEffIndex::MAX_EFFECTS; ++i)
        {
            if (spellInfo->Effects[i].Effect == SPELL_EFFECT_SCHOOL_DAMAGE &&
                spellInfo->GetEffectMechanicMask(i) & (1 << MECHANIC_BLEED))
                return false;
        }
    }
    return true;
}

uint32 Unit::CalcArmorReducedDamage(Unit* victim, const uint32 damage, SpellInfo const* spellInfo, WeaponAttackType /*attackType*/)
{
    uint32 newdamage = 0;
    float armor = float(victim->GetArmor());

    // bypass enemy armor by SPELL_AURA_BYPASS_ARMOR_FOR_CASTER
    int32 armorBypassPct = 0;
    AuraEffectList const & reductionAuras = victim->GetAuraEffectsByType(SPELL_AURA_BYPASS_ARMOR_FOR_CASTER);
    for (AuraEffectList::const_iterator i = reductionAuras.begin(); i != reductionAuras.end(); ++i)
    {
        if ((*i)->GetCasterGUID() == GetGUID())
            armorBypassPct += (*i)->GetAmount();
    }
    armor = CalculatePct(armor, 100 - std::min(armorBypassPct, 100));

    // Ignore enemy armor by SPELL_AURA_MOD_TARGET_RESISTANCE aura
    armor += GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_TARGET_RESISTANCE, SPELL_SCHOOL_MASK_NORMAL);

    if (spellInfo)
        if (Player* modOwner = GetSpellModOwner())
            modOwner->ApplySpellMod(spellInfo->Id, SPELLMOD_IGNORE_ARMOR, armor);

    AuraEffectList const& ResIgnoreAuras = GetAuraEffectsByType(SPELL_AURA_MOD_IGNORE_TARGET_RESIST);
    for (AuraEffectList::const_iterator j = ResIgnoreAuras.begin(); j != ResIgnoreAuras.end(); ++j)
    {
        if ((*j)->GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL)
            armor = floor(AddPct(armor, -(*j)->GetAmount()));
    }

    AuraEffectList const& armorPenetrationPct = GetAuraEffectsByType(SPELL_AURA_MOD_ARMOR_PENETRATION_PCT);
    for (AuraEffectList::const_iterator j = armorPenetrationPct.begin(); j != armorPenetrationPct.end(); ++j)
    {
        if ((*j)->GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL)
            armor -= CalculatePct(armor, (*j)->GetAmount());
    }

    if (armor < 0.0f)
        armor = 0.0f;

    int scalingLevel = std::max(getLevel() > GT_MAX_LEVEL ? GT_MAX_LEVEL - 1 : getLevel() - 1, 1);
    float tmpvalue = armor / (armor + sgtArmorMitigationByLvlStore.LookupEntry(scalingLevel)->ratio);

    if (tmpvalue < 0.0f)
        tmpvalue = 0.0f;

    // Wod MaxValue
    if (tmpvalue > 0.85f)
        tmpvalue = 0.85f;

    newdamage = std::max((int)(damage - (damage * tmpvalue)), 1);

    if (spellInfo)
        LOG_SPELL(this, spellInfo->Id, "Spell %s: CalcArmorReducedDamage(): %i - %f = %i", spellInfo->GetNameForLogging().c_str(), damage, tmpvalue * 100.0f, newdamage); ///<  ISO C++11 does not allow conversion from string literal to 'char *'

    return newdamage;
}

bool Unit::IsSpellResisted(Unit* victim, SpellSchoolMask schoolMask, SpellInfo const* spellInfo)
{
    if (!victim || !victim->isAlive())
        return false;

    Player* owner = GetCharmerOrOwnerPlayerOrPlayerItself();
    if (!owner)
        return false;

    if ((schoolMask & SPELL_SCHOOL_MASK_NORMAL) == 0 && (!spellInfo || (spellInfo->AttributesEx4 & SPELL_ATTR4_IGNORE_RESISTANCES) == 0))
    {
        float victimResistance = float(victim->GetResistance(schoolMask));
        victimResistance += float(owner->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_TARGET_RESISTANCE, schoolMask));

        victimResistance -= float(owner->GetSpellPenetrationItemMod());

        // Resistance can't be lower then 0.
        if (victimResistance < 0.0f)
            victimResistance = 0.0f;

        uint32 level = victim->getLevel();
        float resistanceConstant = 0.0f;

        // @toto update me http://wow.gamepedia.com/Combat_rating_system & SimulationCraft
        if (level >= 61)
            resistanceConstant = 150 + ((level - 60) * (level - 67.5));
        else if (level >= 21 && level <= 60)
            resistanceConstant = 50 + ((level - 20) * 2.5);
        else
            resistanceConstant = 50.0f;

        float averageResist = victimResistance / (victimResistance + resistanceConstant);

        int32 tmp = int32(averageResist * 10000);
        int32 rand = irand(0, 10000);
        return (rand < tmp);
    }
    return false;
}

SpellSchoolMask Unit::CalcAbsorbResist(Unit* victim, SpellSchoolMask schoolMask, DamageEffectType damagetype, uint32 const damage, uint32 *absorb, uint32 *resist, SpellInfo const* spellInfo)
{
    if (!victim || !victim->isAlive() || !damage)
        return schoolMask;

    DamageInfo dmgInfo = DamageInfo(this, victim, damage, spellInfo, schoolMask, damagetype);

    // Magic damage, check for resists
    // Ignore spells that cant be resisted
    if ((schoolMask & SPELL_SCHOOL_MASK_NORMAL) == 0 && (!spellInfo || (spellInfo->AttributesEx4 & SPELL_ATTR4_IGNORE_RESISTANCES) == 0))
    {
        float victimResistance = float(victim->GetResistance(schoolMask));

        Player* player_owner = GetCharmerOrOwnerPlayerOrPlayerItself();

        victimResistance += float(player_owner ? player_owner->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_TARGET_RESISTANCE, schoolMask) : GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_TARGET_RESISTANCE, schoolMask));

        if (player_owner)
            victimResistance -= float(player_owner->GetSpellPenetrationItemMod());

        // Resistance can't be lower then 0.
        if (victimResistance < 0.0f)
            victimResistance = 0.0f;

        uint32 level = victim->getLevel();
        float resistanceConstant = 0.0f;

        // http://elitistjerks.com/f15/t29453-combat_ratings_level_85_cataclysm/
        if (level >= 61)
            resistanceConstant = 150 + ((level - 60) * (level - 67.5));
        else if (level >= 21 && level <= 60)
            resistanceConstant = 50 + ((level - 20) * 2.5);
        else
            resistanceConstant = 50.0f;

        float averageResist = victimResistance / (victimResistance + resistanceConstant);
        float discreteResistProbability[11];
        for (uint32 i = 0; i < 11; ++i)
        {
            discreteResistProbability[i] = 0.5f - 2.5f * fabs(0.1f * i - averageResist);
            if (discreteResistProbability[i] < 0.0f)
                discreteResistProbability[i] = 0.0f;
        }

        if (averageResist <= 0.1f)
        {
            discreteResistProbability[0] = 1.0f - 7.5f * averageResist;
            discreteResistProbability[1] = 5.0f * averageResist;
            discreteResistProbability[2] = 2.5f * averageResist;
        }

        float r = float(rand_norm());
        uint32 i = 0;
        float probabilitySum = discreteResistProbability[0];

        while (r >= probabilitySum && i < 10)
            probabilitySum += discreteResistProbability[++i];

        float damageResisted = float(damage * i / 10);

        // There is no spell with SPELL_AURA_MOD_IGNORE_TARGET_RESIST aura in 4.3.4
        // Skip this block
        /*AuraEffectList const& ResIgnoreAuras = GetAuraEffectsByType(SPELL_AURA_MOD_IGNORE_TARGET_RESIST);
        for (AuraEffectList::const_iterator j = ResIgnoreAuras.begin(); j != ResIgnoreAuras.end(); ++j)
            if ((*j)->GetMiscValue() & schoolMask)
                AddPct(damageResisted, -(*j)->GetAmount());*/

        dmgInfo.ResistDamage(uint32(damageResisted));
    }

    // Ignore Absorption Auras
    float auraAbsorbMod = 0;
    AuraEffectList const& AbsIgnoreAurasA = GetAuraEffectsByType(SPELL_AURA_MOD_TARGET_ABSORB_SCHOOL);
    for (AuraEffectList::const_iterator itr = AbsIgnoreAurasA.begin(); itr != AbsIgnoreAurasA.end(); ++itr)
    {
        if (!((*itr)->GetMiscValue() & schoolMask))
            continue;

        if ((*itr)->GetAmount() > auraAbsorbMod)
            auraAbsorbMod = float((*itr)->GetAmount());
    }

    AuraEffectList const& AbsIgnoreAurasB = GetAuraEffectsByType(SPELL_AURA_MOD_TARGET_ABILITY_ABSORB_SCHOOL);
    for (AuraEffectList::const_iterator itr = AbsIgnoreAurasB.begin(); itr != AbsIgnoreAurasB.end(); ++itr)
    {
        if (!((*itr)->GetMiscValue() & schoolMask))
            continue;

        if (((*itr)->GetAmount() > auraAbsorbMod) && (*itr)->IsAffectingSpell(spellInfo))
            auraAbsorbMod = float((*itr)->GetAmount());
    }
    RoundToInterval(auraAbsorbMod, 0.0f, 100.0f);

    // We're going to call functions which can modify content of the list during iteration over it's elements
    // Let's copy the list so we can prevent iterator invalidation
    AuraEffectList vSchoolAbsorbCopy(victim->GetAuraEffectsByType(SPELL_AURA_SCHOOL_ABSORB));
    vSchoolAbsorbCopy.sort(JadeCore::AbsorbAuraOrderPred());

    // absorb without mana cost
    for (AuraEffectList::iterator itr = vSchoolAbsorbCopy.begin(); (itr != vSchoolAbsorbCopy.end()) && (dmgInfo.GetDamage() > 0); ++itr)
    {
        AuraEffect* absorbAurEff = *itr;
        Aura* aura = absorbAurEff->GetBase();

        // Check if aura was removed during iteration - we don't need to work on such auras
        AuraApplication const* aurApp = absorbAurEff->GetBase()->GetApplicationOfTarget(victim->GetGUID());
        if (!aurApp)
            continue;

        /// Check if we can select information about aura spell
        if (!aurApp->GetBase() || !aurApp->GetBase()->GetSpellInfo())
            continue;

        /// Check MiscValue from SpellInfo, not from loaded inforamation
        SpellInfo const* l_CurrentSpellInfo = sSpellMgr->GetSpellInfo(aurApp->GetBase()->GetSpellInfo()->Id);
        uint8 l_SpellEffIndex = absorbAurEff->GetEffIndex();
        if (!l_CurrentSpellInfo)
            continue;
        int32 l_SpellEffMiscValue = l_CurrentSpellInfo->Effects[l_SpellEffIndex].MiscValue;

        /// Check if this school absorb spell should have effect on current spell
        if (!(l_SpellEffMiscValue & schoolMask))
            continue;

        /// Check for physical damage - for example Demonbolt (SPELL_SCHOOL_MASK_ALL) into Anti-Magic Shell (SPELL_SCHOOL_MASK_SPELL)
        if (!(l_SpellEffMiscValue & SPELL_SCHOOL_MASK_NORMAL) && (schoolMask & SPELL_SCHOOL_MASK_NORMAL))
            continue;

        // get amount which can be still absorbed by the aura
        int32 currentAbsorb = absorbAurEff->GetAmount();
        // aura with infinite absorb amount - let the scripts handle absorbtion amount, set here to 0 for safety
        if (currentAbsorb < 0)
            currentAbsorb = 0;

        uint32 tempAbsorb = uint32(currentAbsorb);

        bool defaultPrevented = false;

        absorbAurEff->GetBase()->CallScriptEffectAbsorbHandlers(absorbAurEff, aurApp, dmgInfo, tempAbsorb, defaultPrevented);
        currentAbsorb = tempAbsorb;

        if (defaultPrevented)
            continue;

        // Apply absorb mod auras
        AddPct(currentAbsorb, -auraAbsorbMod);

        // absorb must be smaller than the damage itself
        currentAbsorb = RoundToInterval(currentAbsorb, 0, int32(dmgInfo.GetDamage()));

        dmgInfo.AbsorbDamage(currentAbsorb);

        tempAbsorb = currentAbsorb;
        aura->CallScriptEffectAfterAbsorbHandlers(absorbAurEff, aurApp, dmgInfo, tempAbsorb);

        // Check if our aura is using amount to count damage
        if (absorbAurEff->GetAmount() >= 0)
        {
            // Reduce shield amount
            absorbAurEff->SetAmount(absorbAurEff->GetAmount() - currentAbsorb);
            // Aura cannot absorb anything more - remove it
            if (absorbAurEff->GetAmount() <= 0 && absorbAurEff->GetBase()->GetId() != 115069) // Custom MoP Script - Stance of the Sturdy Ox shoudn't be removed at any damage
                aura->Remove(AURA_REMOVE_BY_ENEMY_SPELL);
        }
    }

    // absorb by mana cost
    AuraEffectList vManaShieldCopy(victim->GetAuraEffectsByType(SPELL_AURA_MANA_SHIELD));
    for (AuraEffectList::const_iterator itr = vManaShieldCopy.begin(); (itr != vManaShieldCopy.end()) && (dmgInfo.GetDamage() > 0); ++itr)
    {
        AuraEffect* absorbAurEff = *itr;
        Aura* aura = absorbAurEff->GetBase();

        // Check if aura was removed during iteration - we don't need to work on such auras
        AuraApplication const* aurApp = absorbAurEff->GetBase()->GetApplicationOfTarget(victim->GetGUID());
        if (!aurApp)
            continue;
        // check damage school mask

        uint32 absorbMask = absorbAurEff->GetMiscValue();
        if (!(absorbMask & schoolMask))
            continue;

        if ((schoolMask & ~absorbMask) == SPELL_SCHOOL_MASK_NORMAL)
        {
            schoolMask = SPELL_SCHOOL_MASK_NORMAL;
            continue;
        }

        // get amount which can be still absorbed by the aura
        int32 currentAbsorb = absorbAurEff->GetAmount();
        // aura with infinite absorb amount - let the scripts handle absorbtion amount, set here to 0 for safety
        if (currentAbsorb < 0)
            currentAbsorb = 0;

        uint32 tempAbsorb = currentAbsorb;

        bool defaultPrevented = false;

        absorbAurEff->GetBase()->CallScriptEffectManaShieldHandlers(absorbAurEff, aurApp, dmgInfo, tempAbsorb, defaultPrevented);
        currentAbsorb = tempAbsorb;

        if (defaultPrevented)
            continue;

        AddPct(currentAbsorb, -auraAbsorbMod);

        // absorb must be smaller than the damage itself
        currentAbsorb = RoundToInterval(currentAbsorb, 0, int32(dmgInfo.GetDamage()));

        int32 manaReduction = currentAbsorb;

        // lower absorb amount by talents
        if (float manaMultiplier = absorbAurEff->GetSpellInfo()->Effects[absorbAurEff->GetEffIndex()].CalcValueMultiplier(absorbAurEff->GetCaster()))
            manaReduction = int32(float(manaReduction) * manaMultiplier);

        int32 manaTaken = -victim->ModifyPower(POWER_MANA, -manaReduction);

        // take case when mana has ended up into account
        currentAbsorb = currentAbsorb ? int32(float(currentAbsorb) * (float(manaTaken) / float(manaReduction))) : 0;

        dmgInfo.AbsorbDamage(currentAbsorb);

        tempAbsorb = currentAbsorb;
        aura->CallScriptEffectAfterManaShieldHandlers(absorbAurEff, aurApp, dmgInfo, tempAbsorb);

        // Check if our aura is using amount to count damage
        if (absorbAurEff->GetAmount() >= 0)
        {
            absorbAurEff->SetAmount(absorbAurEff->GetAmount() - currentAbsorb);
            if ((absorbAurEff->GetAmount() <= 0))
                aura->Remove(AURA_REMOVE_BY_ENEMY_SPELL);
        }
    }

    // split damage auras - only when not damaging self
    if (victim != this)
    {
        // We're going to call functions which can modify content of the list during iteration over it's elements
        // Let's copy the list so we can prevent iterator invalidation
        AuraEffectList vSplitDamagePctCopy(victim->GetAuraEffectsByType(SPELL_AURA_SPLIT_DAMAGE_PCT));
        for (AuraEffectList::iterator itr = vSplitDamagePctCopy.begin(), next; (itr != vSplitDamagePctCopy.end()) &&  (dmgInfo.GetDamage() > 0); ++itr)
        {
            // Check if aura was removed during iteration - we don't need to work on such auras
            AuraApplication const* aurApp = (*itr)->GetBase()->GetApplicationOfTarget(victim->GetGUID());
            if (!aurApp)
                continue;

            // check damage school mask
            if (!((*itr)->GetMiscValue() & schoolMask))
                continue;

            // Damage can be splitted only if aura has an alive caster
            Unit* caster = (*itr)->GetCaster();
            if (!caster || (caster == victim) || !caster->IsInWorld() || !caster->isAlive())
                continue;

            uint32 splitDamage = CalculatePct(dmgInfo.GetDamage(), (*itr)->GetAmount());

            // absorb must be smaller than the damage itself
            splitDamage = RoundToInterval(splitDamage, uint32(0), uint32(dmgInfo.GetDamage()));

            dmgInfo.AbsorbDamage(splitDamage);

            // check if caster is immune to damage
            if (caster->IsImmunedToDamage(schoolMask))
            {
                victim->SendSpellMiss(caster, (*itr)->GetSpellInfo()->Id, SPELL_MISS_IMMUNE);
                continue;
            }

            // don't damage caster if he has immunity
            if (caster->IsImmunedToDamage((*itr)->GetSpellInfo()))
                continue;

            uint32 splitted = splitDamage;
            uint32 split_absorb = 0;
            DealDamageMods(caster, splitted, &split_absorb);

            // Need to remove all auras breakable by damage.
            caster->RemoveAurasBreakableByDamage();

            SendSpellNonMeleeDamageLog(caster, (*itr)->GetSpellInfo()->Id, splitted, schoolMask, split_absorb, 0, false, 0, false);

            CleanDamage cleanDamage = CleanDamage(splitted, 0, WeaponAttackType::BaseAttack, MELEE_HIT_NORMAL);
            DealDamage(caster, splitted, &cleanDamage, DIRECT_DAMAGE, schoolMask, (*itr)->GetSpellInfo(), false);
        }
    }

    *resist = dmgInfo.GetResist();
    *absorb = dmgInfo.GetAbsorb();

    /// Stagger handler
    if (victim && victim->IsPlayer() && victim->getClass() == CLASS_MONK && damagetype != DamageEffectType::SELF_DAMAGE)
    {
        if (!victim->HasSpell(157533))
        {
            if (!spellInfo || ((spellInfo->DmgClass == SPELL_DAMAGE_CLASS_RANGED || spellInfo->DmgClass == SPELL_DAMAGE_CLASS_MELEE) && schoolMask & SPELL_SCHOOL_MASK_NORMAL))
            if (!spellInfo || (spellInfo->Id != LIGHT_STAGGER && spellInfo->Id != MODERATE_STAGGER && spellInfo->Id != HEAVY_STAGGER))
            {
                if (*absorb)
                    *absorb = victim->CalcStaggerDamage(victim->ToPlayer(), *absorb, schoolMask, spellInfo);
            }
        }
        else ///< You are now able to shrug off even spells
        {
            if (!spellInfo || (spellInfo->Id != LIGHT_STAGGER && spellInfo->Id != MODERATE_STAGGER && spellInfo->Id != HEAVY_STAGGER))
            {
                if (*absorb)
                    *absorb = victim->CalcStaggerDamage(victim->ToPlayer(), *absorb, schoolMask, spellInfo);
            }
        }
    }
    return schoolMask;
}

void Unit::CalcHealAbsorb(Unit* victim, const SpellInfo* healSpell, uint32 &healAmount, uint32 &absorb)
{
    if (!healAmount)
        return;

    int32 RemainingHeal = healAmount;

    // Need remove expired auras after
    bool existExpired = false;

    DamageInfo dmgInfo = DamageInfo(this, victim, healAmount, healSpell, healSpell->GetSchoolMask(), HEAL);

    // absorb without mana cost
    AuraEffectList const& vHealAbsorb = victim->GetAuraEffectsByType(SPELL_AURA_SCHOOL_HEAL_ABSORB);
    for (AuraEffectList::const_iterator i = vHealAbsorb.begin(); i != vHealAbsorb.end() && RemainingHeal > 0; ++i)
    {
        AuraEffect* absorbAurEff = *i;
        Aura* aura = absorbAurEff->GetBase();

        // Check if aura was removed during iteration - we don't need to work on such auras
        AuraApplication const* aurApp = absorbAurEff->GetBase()->GetApplicationOfTarget(victim->GetGUID());
        if (!aurApp)
            continue;
        if (!(absorbAurEff->GetMiscValue() & healSpell->SchoolMask))
            continue;

        // Max Amount can be absorbed by this aura
        int32 currentAbsorb = (*i)->GetAmount();
        // aura with infinite absorb amount - let the scripts handle absorbtion amount, set here to 0 for safety
        if (currentAbsorb < 0)
            currentAbsorb = 0;

        uint32 tempAbsorb = uint32(currentAbsorb);

        bool defaultPrevented = false;

        absorbAurEff->GetBase()->CallScriptEffectAbsorbHandlers(absorbAurEff, aurApp, dmgInfo, tempAbsorb, defaultPrevented);
        currentAbsorb = tempAbsorb;

        if (defaultPrevented)
            continue;

        // absorb must be smaller than the damage itself
        currentAbsorb = RoundToInterval(currentAbsorb, 0, int32(dmgInfo.GetDamage()));

        dmgInfo.AbsorbDamage(currentAbsorb);

        tempAbsorb = currentAbsorb;
        aura->CallScriptEffectAfterAbsorbHandlers(absorbAurEff, aurApp, dmgInfo, tempAbsorb);

        // Check if our aura is using amount to count damage
        if (absorbAurEff->GetAmount() >= 0)
        {
            // Reduce shield amount
            absorbAurEff->SetAmount(absorbAurEff->GetAmount() - currentAbsorb);
            // Aura cannot absorb anything more - remove it
            if (absorbAurEff->GetAmount() <= 0)
                existExpired = true;
        }
    }

    // Remove all expired absorb auras
    if (existExpired)
    {
        for (AuraEffectList::const_iterator i = vHealAbsorb.begin(); i != vHealAbsorb.end();)
        {
            AuraEffect* auraEff = *i;
            ++i;
            if (auraEff->GetAmount() <= 0)
            {
                uint32 removedAuras = victim->m_removedAurasCount;
                auraEff->GetBase()->Remove(AURA_REMOVE_BY_ENEMY_SPELL);
                if (removedAuras+1 < victim->m_removedAurasCount)
                    i = vHealAbsorb.begin();
            }
        }
    }

    absorb = dmgInfo.GetAbsorb();
    healAmount -= absorb;
}

void Unit::AttackerStateUpdate (Unit* victim, WeaponAttackType attType, bool extra)
{
    if (HasUnitState(UNIT_STATE_CANNOT_AUTOATTACK) || HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED))
        return;

    if (!victim->isAlive())
        return;

    if (!IsAIEnabled || !GetMap()->Instanceable())
        if ((attType == WeaponAttackType::BaseAttack || attType == WeaponAttackType::OffAttack) && !IsWithinLOSInMap(victim))
            return;

    CombatStart(victim);
    RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_MELEE_ATTACK);

    if (attType != WeaponAttackType::BaseAttack && attType != WeaponAttackType::OffAttack)
        return;                                             // ignore ranged case

    // melee attack spell casted at main hand attack only - no normal melee dmg dealt
    if (attType == WeaponAttackType::BaseAttack && m_currentSpells[CURRENT_MELEE_SPELL] && !extra)
        m_currentSpells[CURRENT_MELEE_SPELL]->cast();
    else
    {
        // attack can be redirected to another target
        victim = GetMeleeHitRedirectTarget(victim);

        // Custom MoP Script
        // SPELL_AURA_STRIKE_SELF
        if (HasAuraType(SPELL_AURA_STRIKE_SELF))
        {
            // Dizzying Haze - 115180
            if (AuraApplication* aura = this->GetAuraApplication(116330))
            {
                if (roll_chance_i(aura->GetBase()->GetEffect(1)->GetAmount()))
                {
                    victim->CastSpell(this, 118022, true);
                    return;
                }
            }
        }

        CalcDamageInfo damageInfo;
        CalculateMeleeDamage(victim, 0, &damageInfo, attType);
        // Send log damage message to client
        DealDamageMods(victim, damageInfo.damage, &damageInfo.absorb);
        SendAttackStateUpdate(&damageInfo);

        //TriggerAurasProcOnEvent(damageInfo);
        ProcDamageAndSpell(damageInfo.target, damageInfo.procAttacker, damageInfo.procVictim, damageInfo.procEx, damageInfo.damage, damageInfo.absorb, damageInfo.attackType);

        DealMeleeDamage(&damageInfo, true);
    }
}

void Unit::HandleProcExtraAttackFor(Unit* victim)
{
    while (m_extraAttacks)
    {
        AttackerStateUpdate(victim, WeaponAttackType::BaseAttack, true);
        --m_extraAttacks;
    }
}

MeleeHitOutcome Unit::RollMeleeOutcomeAgainst(Unit* victim, WeaponAttackType attType)
{
    // This is only wrapper

    // Miss chance based on melee
    float miss_chance = MeleeSpellMissChance(victim, NULL, attType);

    // Critical hit chance
    float crit_chance = GetUnitCriticalChance(attType, victim);

    float dodge_chance = victim->GetUnitDodgeChance(this);
    float block_chance = victim->GetUnitBlockChance(this);
    float parry_chance = victim->GetUnitParryChance(this);

    return RollMeleeOutcomeAgainst(victim, attType, int32(crit_chance*100), int32(miss_chance*100), int32(dodge_chance*100), int32(parry_chance*100), int32(block_chance*100));
}

MeleeHitOutcome Unit::RollMeleeOutcomeAgainst(Unit* victim, WeaponAttackType attType, int32 crit_chance, int32 miss_chance, int32 dodge_chance, int32 parry_chance, int32 block_chance)
{
    if (victim->HasAuraType(SPELL_AURA_DEFLECT_FRONT_SPELLS) && victim->isInFront(this))
        return MELEE_HIT_MISS;

    MeleeHitOutcome l_HitResult = MELEE_HIT_NORMAL;
    SpellMissInfo l_SpellResult = SPELL_MISS_NONE;
    if (victim->ToCreature() && victim->IsAIEnabled)
        ((Unit*)victim)->ToCreature()->GetAI()->CheckHitResult(l_HitResult, l_SpellResult, (Unit*)this);

    if (victim->GetTypeId() == TYPEID_UNIT && victim->ToCreature()->IsInEvadeMode())
        return MELEE_HIT_EVADE;

    int32    sum = 0, tmp = 0;
    int32    roll = urand (0, 10000);

    tmp = miss_chance;

    if (tmp > 0 && roll < (sum += tmp))
        return MELEE_HIT_MISS;

    // always crit against a sitting target (except 0 crit chance)
    if (victim->IsPlayer() && crit_chance > 0 && !victim->IsStandState())
       return MELEE_HIT_CRIT;

    // Dodge chance
    // only players can't dodge if attacker is behind
    if (!(victim->IsPlayer() && !victim->HasInArc(M_PI, this) && !victim->HasAuraType(SPELL_AURA_IGNORE_HIT_DIRECTION)))
    {
        // Reduce dodge chance by attacker expertise rating
        if (IsPlayer() && victim->IsPlayer())
            dodge_chance -= int32(ToPlayer()->GetExpertiseDodgeOrParryReduction(attType) * 100);
        else
        {
            if (isPet() && GetOwner())
                if (GetOwner()->ToPlayer())
                    dodge_chance -= int32(((Player*)GetOwner())->GetExpertiseDodgeOrParryReduction(attType) * 100);
        }

        // Modify dodge chance by attacker SPELL_AURA_MOD_COMBAT_RESULT_CHANCE
        dodge_chance+= GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_COMBAT_RESULT_CHANCE, VICTIMSTATE_DODGE) * 100;
        dodge_chance = int32(float(dodge_chance) * GetTotalAuraMultiplier(SPELL_AURA_MOD_ENEMY_DODGE));

        tmp = dodge_chance;
        if ((tmp > 0) && roll < (sum += tmp))
            return MELEE_HIT_DODGE;
    }

    // parry & block chances
    // check if attack comes from behind, nobody can parry or block if attacker is behind
    if (!(!victim->HasInArc(M_PI, this) && !victim->HasAuraType(SPELL_AURA_IGNORE_HIT_DIRECTION)))
    {
        float l_ExpertisePercentage = 0.0f;
        // Reduce parry chance by attacker expertise rating
        if (IsPlayer() && victim->IsPlayer())
            l_ExpertisePercentage = int32(ToPlayer()->GetExpertiseDodgeOrParryReduction(attType) * 100.0f);
        else
        {
            if (isPet() && GetOwner())
                if (GetOwner()->ToPlayer())
                    l_ExpertisePercentage = int32(((Player*)GetOwner())->GetExpertiseDodgeOrParryReduction(attType) * 100.0f);
        }

        if (victim->getLevel() >= getLevel())
        {
            uint8 l_LevelDiff = std::min(victim->getLevel() - getLevel(), 3);
            l_ExpertisePercentage -= g_BaseEnemyParryChance[l_LevelDiff] * 100.0f;
        }

        parry_chance -= int32(l_ExpertisePercentage);

        if (victim->IsPlayer() || !(victim->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_NO_PARRY))
        {
            int32 tmp2 = parry_chance;
            if (tmp2 > 0 && roll < (sum += tmp2))
                return MELEE_HIT_PARRY;
        }

        if (victim->IsPlayer() || !(victim->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_NO_BLOCK))
        {
            tmp = block_chance;
            if (tmp > 0 && roll < (sum += tmp))
            {
                if (victim->IsPlayer())
                    sScriptMgr->OnPlayerBlock(victim->ToPlayer(), this);
                return MELEE_HIT_BLOCK;
            }
        }
    }

    // Critical chance
    tmp = crit_chance;

    if (tmp > 0 && roll < (sum += tmp))
    {
        if (!(GetTypeId() == TYPEID_UNIT && (ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_NO_CRIT)))
            return MELEE_HIT_CRIT;
    }

    // Max 40% chance to score a glancing blow against mobs that are higher level (can do only players and pets and not with ranged weapon)
    if (attType != WeaponAttackType::RangedAttack && (IsPlayer() || ToCreature()->isPet()) &&
        victim->ToCreature() && !victim->ToCreature()->isPet() && victim->getLevel() > (getLevel() + 3))
    {
        ///@todo Patch 6.0.2 (2014-10-14): All characters now have a 100% chance to hit, 0% chance to be dodged, 3% chance to be parried, and 0% chance for glancing blows, when fighting creatures up to 3 levels higher (bosses included).
        // Anytime a character makes a melee attack on a level ?? boss
        // there is a 24% chance that blow will be glancing
        // dealing 75% damage and being unable to crit.
        // See: http://us.battle.net/wow/en/forum/topic/7593402521#4
        if (roll_chance_i(24))
            return MELEE_HIT_GLANCING;
    }

    /// @todo check the data from theorycraft and other
    // Need to do some researchs about crushing blows at Mists of Pandaria
    /*if (getLevelForTarget(victim) >= victim->getLevelForTarget(this) + 4 &&
        // can be from by creature (if can) or from controlled player that considered as creature
        !IsControlledByPlayer() &&
        !(GetTypeId() == TYPEID_UNIT && ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_NO_CRUSH))
    {
        // when their weapon skill is 15 or more above victim's defense skill
        tmp = victimMaxSkillValueForLevel;
        // tmp = mob's level * 5 - player's current defense skill
        tmp = attackerMaxSkillValueForLevel - tmp;
        if (tmp >= 15)
        {
            // add 2% chance per lacking skill point, min. is 15%
            tmp = tmp * 200 - 1500;
            if (roll < (sum += tmp))
                return MELEE_HIT_CRUSHING;
        }
    }*/

    return MELEE_HIT_NORMAL;
}

uint32 Unit::CalculateDamage(WeaponAttackType attType, bool normalized, bool addTotalPct)
{
    float min_damage, max_damage;

    if (IsPlayer() && (normalized || !addTotalPct))
        ToPlayer()->CalculateMinMaxDamage(attType, normalized, addTotalPct, min_damage, max_damage);
    else
    {
        switch (attType)
        {
            case WeaponAttackType::RangedAttack:
                min_damage = GetFloatValue(UNIT_FIELD_MIN_RANGED_DAMAGE);
                max_damage = GetFloatValue(UNIT_FIELD_MAX_RANGED_DAMAGE);
                break;
            case WeaponAttackType::BaseAttack:
                min_damage = GetFloatValue(UNIT_FIELD_MIN_DAMAGE);
                max_damage = GetFloatValue(UNIT_FIELD_MAX_DAMAGE);
                break;
            case WeaponAttackType::OffAttack:
                min_damage = GetFloatValue(UNIT_FIELD_MIN_OFF_HAND_DAMAGE);
                max_damage = GetFloatValue(UNIT_FIELD_MAX_OFF_HAND_DAMAGE);
                break;
                // Just for good manner
            default:
                min_damage = 0.0f;
                max_damage = 0.0f;
                break;
        }
    }

    if (min_damage > max_damage)
        std::swap(min_damage, max_damage);

    if (max_damage == 0.0f)
        max_damage = 5.0f;

    return urand((uint32)min_damage, (uint32)max_damage);
}

void Unit::SendMeleeAttackStart(Unit* victim)
{
    WorldPacket data(SMSG_ATTACK_START, 2 * (16 + 2));

    uint64 attackerGuid = GetGUID();
    uint64 victimGuid = victim->GetGUID();

    data.appendPackGUID(attackerGuid);
    data.appendPackGUID(victimGuid);

    SendMessageToSet(&data, true);
}

void Unit::SendMeleeAttackStop(Unit* victim)
{
    WorldPacket data(SMSG_ATTACK_STOP, (2 * (16 + 2)) + 1);

    uint64 victimGUID = victim ? victim->GetGUID() : 0;
    uint64 attackerGUID = GetGUID();

    data.appendPackGUID(attackerGUID);
    data.appendPackGUID(victimGUID);
    data.WriteBit(victim ? victim->isDead() : false);
    data.FlushBits();

    SendMessageToSet(&data, true);
}

bool Unit::isSpellBlocked(Unit* victim, SpellInfo const* spellProto, WeaponAttackType /*attackType*/)
{
    // These spells can't be blocked
    if (spellProto && spellProto->Attributes & SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK)
        return false;

    if (victim->HasAuraType(SPELL_AURA_IGNORE_HIT_DIRECTION) || victim->HasInArc(M_PI, this))
    {
        // Check creatures flags_extra for disable block
        if (victim->GetTypeId() == TYPEID_UNIT &&
            victim->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_NO_BLOCK)
                return false;

        if (roll_chance_f(victim->GetUnitBlockChance(this)))
            return true;
    }
    return false;
}

bool Unit::isBlockCritical()
{
    if (roll_chance_i(GetTotalAuraModifier(SPELL_AURA_MOD_BLOCK_CRIT_CHANCE)))
    {
        // Critical Blocks (spe Protection) enrage the warrior
        if (ToPlayer() != nullptr && ToPlayer()->GetSpecializationId() == SPEC_WARRIOR_PROTECTION)
            CastSpell(this, 12880, true);
        return true;
    }

    return false;
}

int32 Unit::GetMechanicResistChance(const SpellInfo* spell)
{
    if (!spell)
        return 0;
    int32 resist_mech = 0;
    for (uint8 eff = 0; eff < spell->EffectCount; ++eff)
    {
        if (!spell->Effects[eff].IsEffect())
           break;
        int32 effect_mech = spell->GetEffectMechanic(eff);
        if (effect_mech)
        {
            int32 temp = GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_MECHANIC_RESISTANCE, effect_mech);
            if (resist_mech < temp)
                resist_mech = temp;
        }
    }
    return resist_mech;
}

uint32 Unit::GetDodgeChance(const Unit* p_Victim)
{
    int32 dodgeChance = int32(p_Victim->GetUnitDodgeChance(this));

    /// Reduce enemy dodge chance by SPELL_AURA_MOD_COMBAT_RESULT_CHANCE
    dodgeChance += GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_COMBAT_RESULT_CHANCE, VICTIMSTATE_DODGE);
    dodgeChance = int32(float(dodgeChance) * GetTotalAuraMultiplier(SPELL_AURA_MOD_ENEMY_DODGE));
    /// Reduce dodge chance by attacker expertise rating
    if (dodgeChance < 0)
        dodgeChance = 0;

    return dodgeChance;
}

uint32 Unit::GetParryChance(const Unit* p_Victim)
{
    int32 l_ParryChance = int32(p_Victim->GetUnitParryChance(this));

    if (l_ParryChance < 0)
        l_ParryChance = 0;

    return l_ParryChance;
}

uint32 Unit::GetBlockChance(const Unit* p_Victim)
{
    int32 l_BlockChance = int32(p_Victim->GetUnitBlockChance(this));

    if (l_BlockChance < 0)
        l_BlockChance = 0;

    return l_BlockChance;
}

// Melee based spells hit result calculations
SpellMissInfo Unit::MeleeSpellHitResult(Unit* victim, SpellInfo const* spell)
{
    if (victim->isInFront(this) && victim->HasAuraType(SPELL_AURA_DEFLECT_FRONT_SPELLS))
        return SPELL_MISS_DEFLECT;

    // Spells with SPELL_ATTR3_IGNORE_HIT_RESULT will additionally fully ignore
    // resist and deflect chances
    if (spell->AttributesEx3 & SPELL_ATTR3_IGNORE_HIT_RESULT || spell->IsInterruptSpell())
        return SPELL_MISS_NONE;

    WeaponAttackType attType = WeaponAttackType::BaseAttack;

    // Check damage class instead of attack type to correctly handle judgements
    // - they are meele, but can't be dodged/parried/deflected because of ranged dmg class
    if (spell->DmgClass == SPELL_DAMAGE_CLASS_RANGED)
        attType = WeaponAttackType::RangedAttack;

    int32 roll = urand(0, 10000);

    // Roll miss
    int32 tmp = int32(MeleeSpellMissChance(victim, spell, attType)) * 100;
    if (roll < tmp)
        return SPELL_MISS_MISS;

    // Roll resist
    // Chance resist mechanic (select max value from every mechanic spell effect)
    int32 l_Resist = (victim->GetMechanicResistChance(spell) * 100);
    tmp += l_Resist;
    if (roll < l_Resist)
        return SPELL_MISS_RESIST;

    // Charge spells aren't suppose to takecare of dodge parry or block
    if (spell->AttributesCu & SPELL_ATTR0_CU_CHARGE)
        return SPELL_MISS_NONE;

    bool canDodge = true;
    bool canParry = true;
    bool canBlock = spell->AttributesEx3 & SPELL_ATTR3_BLOCKABLE_SPELL;

    // Same spells cannot be parry/dodge
    if (spell->Attributes & SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK)
        return SPELL_MISS_NONE;

    // Ranged attacks can only miss, resist and deflect
    if (attType == WeaponAttackType::RangedAttack)
    {
        // only if in front
        if (victim->HasInArc(M_PI, this) || victim->HasAuraType(SPELL_AURA_IGNORE_HIT_DIRECTION))
        {
            int32 deflect_chance = victim->GetTotalAuraModifier(SPELL_AURA_DEFLECT_SPELLS) * 100;
            tmp+=deflect_chance;
            if (roll < deflect_chance)
                return SPELL_MISS_DEFLECT;
        }

        /// Since MoP, Hunter ranged attacks are dodgeable
        /// http://www.mmo-champion.com/threads/1090757-Hunter-ranged-attacks-now-able-to-be-dodged-in-MOP
        /// http://eu.battle.net/wow/en/forum/topic/9338743978
        if (getClass() == CLASS_HUNTER)
            canParry = false;
        else
            return SPELL_MISS_NONE;
    }

    // Check for attack from behind
    if (!victim->HasInArc(M_PI, this))
    {
        if (!victim->HasAuraType(SPELL_AURA_IGNORE_HIT_DIRECTION))
        {
            // Can`t dodge from behind in PvP (but its possible in PvE)
            if (victim->IsPlayer())
                canDodge = false;
            // Can`t parry or block
            canParry = false;
            canBlock = false;
        }
        else // Only deterrence as of 3.3.5
        {
            if (spell->AttributesCu & SPELL_ATTR0_CU_REQ_CASTER_BEHIND_TARGET)
                canParry = false;
        }
    }
    // Check creatures flags_extra for disable parry
    if (victim->GetTypeId() == TYPEID_UNIT)
    {
        uint32 flagEx = victim->ToCreature()->GetCreatureTemplate()->flags_extra;
        if (flagEx & CREATURE_FLAG_EXTRA_NO_PARRY)
            canParry = false;
        // Check creatures flags_extra for disable block
        if (flagEx & CREATURE_FLAG_EXTRA_NO_BLOCK)
            canBlock = false;
    }
    // Ignore combat result aura
    AuraEffectList const& ignore = GetAuraEffectsByType(SPELL_AURA_IGNORE_COMBAT_RESULT);
    for (AuraEffectList::const_iterator i = ignore.begin(); i != ignore.end(); ++i)
    {
        if (!(*i)->IsAffectingSpell(spell))
            continue;
        switch ((*i)->GetMiscValue())
        {
            case MELEE_HIT_DODGE: canDodge = false; break;
            case MELEE_HIT_BLOCK: canBlock = false; break;
            case MELEE_HIT_PARRY: canParry = false; break;
            default:
                break;
        }
    }

    if (canDodge)
    {
        int32 l_DodgeChance = GetDodgeChance(victim) * 100;

        if (IsPlayer() && victim->IsPlayer())
            l_DodgeChance -= int32(ToPlayer()->GetExpertiseDodgeOrParryReduction(attType) * 100);

        tmp += l_DodgeChance;
        if (roll < l_DodgeChance)
            return SPELL_MISS_DODGE;
    }

    if (canParry)
    {
        int32 l_ParryChance = GetParryChance(victim) * 100;

        if (IsPlayer() && victim->IsPlayer())
            l_ParryChance -= int32(ToPlayer()->GetExpertiseDodgeOrParryReduction(attType) * 100);

        tmp += l_ParryChance;
        if (roll < l_ParryChance)
            return SPELL_MISS_PARRY;
    }

    if (canBlock)
    {
        int32 l_BlockChance = GetBlockChance(victim) * 100;

        tmp += l_BlockChance;
        if (roll < l_BlockChance)
            return SPELL_MISS_BLOCK;
    }

    return SPELL_MISS_NONE;
}

SpellMissInfo Unit::MagicSpellHitResult(Unit* p_Victim, SpellInfo const* p_Spell)
{
    if (p_Victim->isInFront(this) && p_Victim->HasAuraType(SPELL_AURA_DEFLECT_FRONT_SPELLS))
        return SPELL_MISS_DEFLECT;

    // Can`t miss on dead target (on skinning for example)
    if (!p_Victim->isAlive() && p_Victim->GetTypeId() != TYPEID_PLAYER)
        return SPELL_MISS_NONE;

    if (p_Spell->IsInterruptSpell())
    {
        // only deflect works here
        int32 l_DeflectChance = p_Victim->GetTotalAuraModifier(SPELL_AURA_DEFLECT_SPELLS);
        if (roll_chance_i(l_DeflectChance))
            return SPELL_MISS_DEFLECT;

        return SPELL_MISS_NONE;
    }

    uint32 l_Roll = urand(0, 10000);

    // Roll miss
    uint32 l_Tmp = uint32(MagicSpellMissChance(p_Victim, p_Spell)) * 100;
    if (l_Roll < l_Tmp)
        return SPELL_MISS_MISS;

    // Roll resist
    // Chance resist mechanic (select max value from every mechanic spell effect)
    l_Tmp += p_Victim->GetMechanicResistChance(p_Spell) * 100;
    if (l_Roll < l_Tmp)
        return SPELL_MISS_RESIST;

    // Spells with SPELL_ATTR3_IGNORE_HIT_RESULT will additionally fully ignore
    // resist and deflect chances
    if (p_Spell->AttributesEx3 & SPELL_ATTR3_IGNORE_HIT_RESULT)
        return SPELL_MISS_NONE;

    /// Same spells cannot be parry/dodge
    if (p_Spell->Attributes & SPELL_ATTR0_IMPOSSIBLE_DODGE_PARRY_BLOCK)
        return SPELL_MISS_NONE;

    // cast by caster in front of victim
    if ((p_Victim->HasInArc(M_PI, this) || p_Victim->HasAuraType(SPELL_AURA_IGNORE_HIT_DIRECTION)))
    {
        int32 l_DeflectChance = p_Victim->GetTotalAuraModifier(SPELL_AURA_DEFLECT_SPELLS) * 100;
        l_Tmp += l_DeflectChance;
        if (l_Roll < l_Tmp)
            return SPELL_MISS_DEFLECT;
    }

    return SPELL_MISS_NONE;
}

// Calculate spell hit result can be:
// Every spell can: Evade/Immune/Reflect/Sucesful hit
// For melee based spells:
//   Miss
//   Dodge
//   Parry
// For spells
//   Resist
SpellMissInfo Unit::SpellHitResult(Unit* victim, SpellInfo const* spell, bool CanReflect)
{
    // Check for immune
    if (victim->IsImmunedToSpell(spell) && !IsFriendlyTo(victim))
        return SPELL_MISS_IMMUNE;

    // All positive spells can`t miss
    // TODO: client not show miss log for this spells - so need find info for this in dbc and use it!
    if ((spell->IsPositive() || spell->IsHealingSpell())
        &&(!IsHostileTo(victim)))  // prevent from affecting enemy by "positive" spell
        return SPELL_MISS_NONE;
    // Check for immune
    if (victim->IsImmunedToDamage(spell) && !IsFriendlyTo(victim))
        return SPELL_MISS_IMMUNE;

    if (this == victim)
        return SPELL_MISS_NONE;

    // Return evade for units in evade mode
    if (victim->GetTypeId() == TYPEID_UNIT && victim->ToCreature()->IsInEvadeMode())
        return SPELL_MISS_EVADE;

    /// Try victim reflect spell - 'Spell Reflect normally does not work with AoE spells'
    if (CanReflect && !spell->IsTargetingArea())
    {
        int32 reflectchance = victim->GetTotalAuraModifier(SPELL_AURA_REFLECT_SPELLS);
        Unit::AuraEffectList const& mReflectSpellsSchool = victim->GetAuraEffectsByType(SPELL_AURA_REFLECT_SPELLS_SCHOOL);
        for (Unit::AuraEffectList::const_iterator i = mReflectSpellsSchool.begin(); i != mReflectSpellsSchool.end(); ++i)
            if ((*i)->GetMiscValue() & spell->GetSchoolMask())
                reflectchance += (*i)->GetAmount();
        if (reflectchance > 0 && roll_chance_i(reflectchance) && !spell->IsPositive() && !IsFriendlyTo(victim))
        {
            // Hack fix for Glyph of Grounding Totem - Remove aura
            if (victim->HasAura(89523))
                victim->RemoveAura(89523);

            // Start triggers for remove charges if need (trigger only for victim, and mark as active spell)
            ProcDamageAndSpell(victim, PROC_FLAG_NONE, PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG, PROC_EX_REFLECT, 1, 0, WeaponAttackType::BaseAttack, spell);
            return SPELL_MISS_REFLECT;
        }
    }

    // Taunt-like spells can't miss since 4.0.1
    if (spell->HasEffect(SPELL_EFFECT_ATTACK_ME))
        return SPELL_MISS_NONE;

    // TODO: Write some attribute or something, cos' SPELL_ATTR3_IGNORE_HIT_RESULT doesn't affect hit level difference
    switch (spell->Id)
    {
        case 87178: // Mind Spike
            return SPELL_MISS_NONE;
        default:
            break;
    }

    Unit * checker = (isTotem() && GetOwner()) ? GetOwner() : this;
    switch (spell->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_RANGED:
        case SPELL_DAMAGE_CLASS_MELEE:
            return checker->MeleeSpellHitResult(victim, spell);
        case SPELL_DAMAGE_CLASS_NONE:
        {
            // Warrior Charge
            if (spell->SpellFamilyName == SPELLFAMILY_WARRIOR && (spell->SpellFamilyFlags[0] & 0x01000000))
                return checker->MeleeSpellHitResult(victim, spell);

            return SPELL_MISS_NONE;
        }
        case SPELL_DAMAGE_CLASS_MAGIC:
            return checker->MagicSpellHitResult(victim, spell);
    }
    return SPELL_MISS_NONE;
}

float Unit::GetUnitDodgeChance(Unit const* p_Attacker) const
{
    if (IsNonMeleeSpellCasted(false) || HasUnitState(UNIT_STATE_CONTROLLED))
        return 0.0f;

    float l_Chance = 0.0f;

    if (IsPlayer())
        l_Chance = GetFloatValue(PLAYER_FIELD_DODGE_PERCENTAGE);
    else
    {
        if (!ToCreature()->isTotem())
        {
            if (getLevel() >= p_Attacker->getLevel())
            {
                uint8 l_LevelDiff = std::min(getLevel() - p_Attacker->getLevel(), STATS_CHANCE_SIZE - 1);
                l_Chance = g_BaseEnemyDodgeChance[l_LevelDiff];
            }

            l_Chance += GetTotalAuraModifier(SPELL_AURA_MOD_DODGE_PERCENT);
        }
    }

    return l_Chance;
}

float Unit::GetUnitParryChance(Unit const* p_Attacker) const
{
    if (IsNonMeleeSpellCasted(false) || HasUnitState(UNIT_STATE_CONTROLLED))
        return 0.0f;

    float l_Chance = 0.0f;

    if (Player const* l_Player = ToPlayer())
    {
        if (l_Player->CanParry())
        {
            Item* l_Item = l_Player->GetWeaponForAttack(WeaponAttackType::BaseAttack, true);
            if (!l_Item)
                l_Item = l_Player->GetWeaponForAttack(WeaponAttackType::OffAttack, true);

            if (l_Item)
                l_Chance = GetFloatValue(PLAYER_FIELD_PARRY_PERCENTAGE);
        }
    }
    else if (GetTypeId() == TYPEID_UNIT)
    {
        if (GetCreatureType() == CREATURE_TYPE_HUMANOID)
        {
            if (getLevel() >= p_Attacker->getLevel())
            {
                uint8 l_LevelDiff = std::min(getLevel() - p_Attacker->getLevel(), STATS_CHANCE_SIZE - 1);
                l_Chance = g_BaseEnemyParryChance[l_LevelDiff];
            }

            l_Chance += GetTotalAuraModifier(SPELL_AURA_MOD_DODGE_PERCENT);

            // Tanks specializations have a better chance to parry (+3%)
            if (p_Attacker->IsPlayer())
                if (p_Attacker->ToPlayer()->IsActiveSpecTankSpec())
                    l_Chance += 3.0f;
        }
    }

    return l_Chance;
}

float Unit::GetUnitMissChancePhysical(Unit const* p_Attacker, WeaponAttackType p_AttType) const
{
    float l_Chance = 0.0f;

    if (getLevel() >= p_Attacker->getLevel())
    {
        uint8 l_LevelDiff = std::min(getLevel() - p_Attacker->getLevel(), STATS_CHANCE_SIZE - 1);
        l_Chance += g_BaseMissChancePhysical[l_LevelDiff];
    }

    if (p_AttType == WeaponAttackType::RangedAttack)
        l_Chance += GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_RANGED_HIT_CHANCE);
    else
        l_Chance += -(GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE));

    return l_Chance;
}

float Unit::GetUnitMissChanceSpell(Unit const* p_Attacker) const
{
    float l_Chance = 0.0f;

    if (getLevel() >= p_Attacker->getLevel())
    {
        uint8 l_LevelDiff = std::min(getLevel() - p_Attacker->getLevel(), STATS_CHANCE_SIZE - 1);
        // TODO check PvE and PvP spell misschance
        /// http://wow.gamepedia.com/Spell_hit
        l_Chance += g_BaseMissChanceSpell[l_LevelDiff];
    }

    l_Chance += GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE);

    return l_Chance;
}

float Unit::GetUnitBlockChance(Unit const* p_Attacker) const
{
    if (IsNonMeleeSpellCasted(false) || HasUnitState(UNIT_STATE_CONTROLLED))
        return 0.0f;

    if (Player const* l_Player = ToPlayer())
    {
        if (l_Player->CanBlock())
        {
            Item* l_Item = l_Player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (l_Item && !l_Item->CantBeUse())
                return GetFloatValue(PLAYER_FIELD_BLOCK_PERCENTAGE);
        }
        // is player but has no block ability or no not broken shield equipped
        return 0.0f;
    }
    else
    {
        if (ToCreature()->isTotem())
            return 0.0f;
        else
        {
            float l_Block = 3.0f;
            l_Block += GetTotalAuraModifier(SPELL_AURA_MOD_BLOCK_PERCENT);

            if (getLevel() > p_Attacker->getLevel())
            {
                uint8 l_LevelDiff = getLevel() - p_Attacker->getLevel();
                l_Block += 1.5f * l_LevelDiff;
            }

            return l_Block;
        }
    }
}

float Unit::GetUnitCriticalChance(WeaponAttackType attackType, const Unit* victim) const
{
    float crit;

    if (IsPlayer())
    {
        switch (attackType)
        {
            case WeaponAttackType::BaseAttack:
                crit = GetFloatValue(PLAYER_FIELD_CRIT_PERCENTAGE);
                break;
            case WeaponAttackType::OffAttack:
                crit = GetFloatValue(PLAYER_FIELD_OFFHAND_CRIT_PERCENTAGE);
                break;
            case WeaponAttackType::RangedAttack:
                crit = GetFloatValue(PLAYER_FIELD_RANGED_CRIT_PERCENTAGE);
                break;
                // Just for good manner
            default:
                crit = 0.0f;
                break;
        }
    }
    else
    {
        crit = 5.0f;
        crit += GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
        crit += GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
    }

    // flat aura mods
    if (attackType == WeaponAttackType::RangedAttack)
        crit += victim->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_CHANCE);
    else
        crit += victim->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_CHANCE);

    crit += victim->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE);

    if (crit < 0.0f)
        crit = 0.0f;
    return crit;
}

void Unit::_DeleteRemovedAuras()
{
    while (!m_removedAuras.empty())
    {
        Aura* l_Aura = m_removedAuras.front();
        l_Aura->Delink();
        delete l_Aura;
        m_removedAuras.pop_front();
    }
}

void Unit::_UpdateSpells(uint32 time)
{
    if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL])
        _UpdateAutoRepeatSpell();

    // remove finished spells from current pointers
    for (uint32 i = 0; i < CURRENT_MAX_SPELL; ++i)
    {
        if (m_currentSpells[i] && m_currentSpells[i]->getState() == SPELL_STATE_FINISHED)
        {
            m_currentSpells[i]->SetReferencedFromCurrent(false);
            m_currentSpells[i] = NULL;                      // remove pointer
        }
    }

    // m_auraUpdateIterator can be updated in indirect called code at aura remove to skip next planned to update but removed auras
    for (m_auraUpdateIterator = m_ownedAuras.begin(); m_auraUpdateIterator != m_ownedAuras.end();)
    {
        Aura*& i_aura = m_auraUpdateIterator->second;
        ++m_auraUpdateIterator;                            // need shift to next for allow update if need into aura update
        i_aura->UpdateOwner(time, this);
    }

    // remove expired auras - do that after updates(used in scripts?)
    for (AuraMap::iterator i = m_ownedAuras.begin(); i != m_ownedAuras.end();)
    {
        if (i->second->IsExpired())
            RemoveOwnedAura(i, AURA_REMOVE_BY_EXPIRE);
        else
            ++i;
    }

    for (VisibleAuraMap::iterator itr = m_visibleAuras.begin(); itr != m_visibleAuras.end(); ++itr)
        if (itr->second->IsNeedClientUpdate())
            itr->second->ClientUpdate();

    _DeleteRemovedAuras();

    if (!m_gameObj.empty())
    {
        GameObjectList::iterator itr;
        for (itr = m_gameObj.begin(); itr != m_gameObj.end();)
        {
            if (!(*itr)->isSpawned())
            {
                (*itr)->SetOwnerGUID(0);
                (*itr)->SetRespawnTime(0);
                (*itr)->Delete();
                m_gameObj.erase(itr++);
            }
            else
                ++itr;
        }
    }

    if (ToPlayer())
        ToPlayer()->UpdateCharges();
}

void Unit::_UpdateAutoRepeatSpell()
{
    // check "real time" interrupts
    // don't cancel spells which are affected by a SPELL_AURA_CAST_WHILE_WALKING or SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING effect
    if (((IsPlayer() && ToPlayer()->IsMoving()) || IsNonMeleeSpellCasted(false, false, true, m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo->Id == 75)) &&
        !HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo) && !HasAuraType(SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING))
    {
        // cancel wand shoot
        if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo->Id != 75)
            InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
        m_AutoRepeatFirstCast = true;
        return;
    }

    // apply delay (Auto Shot - 75 not affected)
    if (m_AutoRepeatFirstCast && getAttackTimer(WeaponAttackType::RangedAttack) < 500 && m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo->Id != 75)
        setAttackTimer(WeaponAttackType::RangedAttack, 500);
    m_AutoRepeatFirstCast = false;

    // castroutine
    if (isAttackReady(WeaponAttackType::RangedAttack))
    {
        // Check if able to cast
        if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->CheckCast(true) != SPELL_CAST_OK)
        {
            InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
            return;
        }

        // we want to shoot
        Spell* spell = new Spell(this, m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo, TRIGGERED_FULL_MASK);
        spell->prepare(&(m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_targets));

        // all went good, reset attack
        resetAttackTimer(WeaponAttackType::RangedAttack);
    }
}

void Unit::SetCurrentCastedSpell(Spell* pSpell)
{
    ASSERT(pSpell);                                         // NULL may be never passed here, use InterruptSpell or InterruptNonMeleeSpells

    CurrentSpellTypes CSpellType = pSpell->GetCurrentContainer();

    if (pSpell == m_currentSpells[CSpellType])             // avoid breaking self
        return;

    // break same type spell if it is not delayed
    if (Spell* l_CurrentSpell = GetCurrentSpell(CSpellType)) ///< l_CurrentSpell is not used
            InterruptSpell(CSpellType, false);

    // special breakage effects:
    switch (CSpellType)
    {
        case CURRENT_GENERIC_SPELL:
        {
            // generic spells always break channeled not delayed spells
            if (!(pSpell->getTriggerCastFlags() & TRIGGERED_IGNORE_CAST_IN_PROGRESS))
                InterruptSpell(CURRENT_CHANNELED_SPELL, false);

            // autorepeat breaking
            if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL])
            {
                // break autorepeat if not Auto Shot
                if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo->Id != 75)
                    InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
                m_AutoRepeatFirstCast = true;
            }
            if (pSpell->m_spellInfo->CalcCastTime(this) > 0)
                AddUnitState(UNIT_STATE_CASTING);

            break;
        }
        case CURRENT_CHANNELED_SPELL:
        {
            // channel spells always break generic non-delayed and any channeled spells
            InterruptSpell(CURRENT_GENERIC_SPELL, false);
            InterruptSpell(CURRENT_CHANNELED_SPELL);

            // it also does break autorepeat if not Auto Shot
            if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL] &&
                m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo->Id != 75)
                InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
            AddUnitState(UNIT_STATE_CASTING);

            break;
        }
        case CURRENT_AUTOREPEAT_SPELL:
        {
            // only Auto Shoot does not break anything
            if (pSpell->m_spellInfo->Id != 75)
            {
                // generic autorepeats break generic non-delayed and channeled non-delayed spells
                InterruptSpell(CURRENT_GENERIC_SPELL, false);
                InterruptSpell(CURRENT_CHANNELED_SPELL, false);
            }
            // special action: set first cast flag
            m_AutoRepeatFirstCast = true;

            break;
        }
        default:
            break; // other spell types don't break anything now
    }

    // current spell (if it is still here) may be safely deleted now
    if (m_currentSpells[CSpellType])
        m_currentSpells[CSpellType]->SetReferencedFromCurrent(false);

    // set new current spell
    m_currentSpells[CSpellType] = pSpell;
    pSpell->SetReferencedFromCurrent(true);

    pSpell->m_selfContainer = &(m_currentSpells[pSpell->GetCurrentContainer()]);
}

void Unit::InterruptSpell(CurrentSpellTypes spellType, bool withDelayed, bool withInstant)
{
    ASSERT(spellType < CURRENT_MAX_SPELL);

    //sLog->outDebug(LOG_FILTER_UNITS, "Interrupt spell for unit %u.", GetEntry());
    Spell* spell = m_currentSpells[spellType];
    if (spell
        && (withDelayed || spell->getState() != SPELL_STATE_DELAYED)
        && (withInstant || spell->GetCastTime() > 0))
    {
        // for example, do not let self-stun aura interrupt itself
        if (!spell->IsInterruptable())
            return;

        // send autorepeat cancel message for autorepeat spells
        if (spellType == CURRENT_AUTOREPEAT_SPELL)
            if (IsPlayer())
                ToPlayer()->SendAutoRepeatCancel(this);

        if (spell->getState() != SPELL_STATE_FINISHED)
            spell->cancel();

        m_currentSpells[spellType] = NULL;
        spell->SetReferencedFromCurrent(false);
    }
}

void Unit::FinishSpell(CurrentSpellTypes spellType, bool ok /*= true*/)
{
    Spell* spell = m_currentSpells[spellType];
    if (!spell)
        return;

    if (spellType == CURRENT_CHANNELED_SPELL)
        spell->SendChannelUpdate(0);

    spell->finish(ok);
}

bool Unit::IsNonMeleeSpellCasted(bool withDelayed, bool skipChanneled, bool skipAutorepeat, bool isAutoshoot, bool skipInstant) const
{
    // We don't do loop here to explicitly show that melee spell is excluded.
    // Maybe later some special spells will be excluded too.

    // if skipInstant then instant spells shouldn't count as being casted
    if (skipInstant && m_currentSpells[CURRENT_GENERIC_SPELL] && !m_currentSpells[CURRENT_GENERIC_SPELL]->GetCastTime())
        return false;

    // generic spells are casted when they are not finished and not delayed
    if (m_currentSpells[CURRENT_GENERIC_SPELL] &&
        (m_currentSpells[CURRENT_GENERIC_SPELL]->getState() != SPELL_STATE_FINISHED) &&
        (withDelayed || m_currentSpells[CURRENT_GENERIC_SPELL]->getState() != SPELL_STATE_DELAYED))
    {
        if (!isAutoshoot || !(m_currentSpells[CURRENT_GENERIC_SPELL]->m_spellInfo->AttributesEx2 & SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS))
            return true;
    }
    // channeled spells may be delayed, but they are still considered casted
    else if (!skipChanneled && m_currentSpells[CURRENT_CHANNELED_SPELL] &&
        (m_currentSpells[CURRENT_CHANNELED_SPELL]->getState() != SPELL_STATE_FINISHED))
    {
        if (!isAutoshoot || !(m_currentSpells[CURRENT_CHANNELED_SPELL]->m_spellInfo->AttributesEx2 & SPELL_ATTR2_NOT_RESET_AUTO_ACTIONS))
            return true;
    }
    // autorepeat spells may be finished or delayed, but they are still considered casted
    else if (!skipAutorepeat && m_currentSpells[CURRENT_AUTOREPEAT_SPELL])
        return true;

    return false;
}

void Unit::InterruptNonMeleeSpells(bool withDelayed, uint32 spell_id, bool withInstant)
{
    // generic spells are interrupted if they are not finished or delayed
    if (m_currentSpells[CURRENT_GENERIC_SPELL] && (!spell_id || m_currentSpells[CURRENT_GENERIC_SPELL]->m_spellInfo->Id == spell_id))
        InterruptSpell(CURRENT_GENERIC_SPELL, withDelayed, withInstant);

    // autorepeat spells are interrupted if they are not finished or delayed
    if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL] && (!spell_id || m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo->Id == spell_id))
        InterruptSpell(CURRENT_AUTOREPEAT_SPELL, withDelayed, withInstant);

    // channeled spells are interrupted if they are not finished, even if they are delayed
    if (m_currentSpells[CURRENT_CHANNELED_SPELL] && (!spell_id || m_currentSpells[CURRENT_CHANNELED_SPELL]->m_spellInfo->Id == spell_id))
        InterruptSpell(CURRENT_CHANNELED_SPELL, true, true);
}

void Unit::InterruptNonMeleeSpellsExcept(bool withDelayed, uint32 except, bool withInstant)
{
    // generic spells are interrupted if they are not finished or delayed
    if (m_currentSpells[CURRENT_GENERIC_SPELL] && m_currentSpells[CURRENT_GENERIC_SPELL]->m_spellInfo->Id != except)
        InterruptSpell(CURRENT_GENERIC_SPELL, withDelayed, withInstant);

    // autorepeat spells are interrupted if they are not finished or delayed
    if (m_currentSpells[CURRENT_AUTOREPEAT_SPELL] && m_currentSpells[CURRENT_AUTOREPEAT_SPELL]->m_spellInfo->Id != except)
        InterruptSpell(CURRENT_AUTOREPEAT_SPELL, withDelayed, withInstant);

    // channeled spells are interrupted if they are not finished, even if they are delayed
    if (m_currentSpells[CURRENT_CHANNELED_SPELL] && m_currentSpells[CURRENT_CHANNELED_SPELL]->m_spellInfo->Id != except)
        InterruptSpell(CURRENT_CHANNELED_SPELL, true, true);
}

Spell* Unit::FindCurrentSpellBySpellId(uint32 spell_id) const
{
    for (uint32 i = 0; i < CURRENT_MAX_SPELL; i++)
        if (m_currentSpells[i] && m_currentSpells[i]->m_spellInfo->Id == spell_id)
            return m_currentSpells[i];
    return NULL;
}

int32 Unit::GetCurrentSpellCastTime(uint32 spell_id) const
{
    if (Spell const* spell = FindCurrentSpellBySpellId(spell_id))
        return spell->GetCastTime();
    return 0;
}

bool Unit::isInFrontInMap(Unit const* target, float distance,  float arc) const
{
    return IsWithinDistInMap(target, distance) && HasInArc(arc, target);
}

bool Unit::isInBackInMap(Unit const* target, float distance, float arc) const
{
    return IsWithinDistInMap(target, distance) && !HasInArc(2 * M_PI - arc, target);
}

bool Unit::isInAccessiblePlaceFor(Creature const* c) const
{
    if (IsInWater())
        return c->canSwim();
    else
        return c->canWalk() || c->CanFly();
}

bool Unit::IsInWater() const
{
    return GetBaseMap()->IsInWater(GetPositionX(), GetPositionY(), GetPositionZ());
}

bool Unit::IsUnderWater() const
{
    return GetBaseMap()->IsUnderWater(GetPositionX(), GetPositionY(), GetPositionZ());
}

void Unit::UpdateUnderwaterState(Map* m, float x, float y, float z)
{
    if (!isPet() && !IsVehicle())
        return;

    LiquidData liquid_status;
    ZLiquidStatus res = m->getLiquidStatus(x, y, z, MAP_ALL_LIQUIDS, &liquid_status);
    if (!res)
    {
        if (_lastLiquid && _lastLiquid->SpellId)
            RemoveAurasDueToSpell(_lastLiquid->SpellId);

        RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_NOT_UNDERWATER);
        _lastLiquid = NULL;
        return;
    }

    if (uint32 liqEntry = liquid_status.entry)
    {
        LiquidTypeEntry const* liquid = sLiquidTypeStore.LookupEntry(liqEntry);
        if (_lastLiquid && _lastLiquid->SpellId && _lastLiquid->Id != liqEntry)
            RemoveAurasDueToSpell(_lastLiquid->SpellId);

        if (liquid && liquid->SpellId)
        {
            if (res & (LIQUID_MAP_UNDER_WATER | LIQUID_MAP_IN_WATER))
            {
                if (!HasAura(liquid->SpellId))
                    CastSpell(this, liquid->SpellId, true);
            }
            else
                RemoveAurasDueToSpell(liquid->SpellId);
        }

        RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_NOT_ABOVEWATER);
        _lastLiquid = liquid;
    }
    else if (_lastLiquid && _lastLiquid->SpellId)
    {
        RemoveAurasDueToSpell(_lastLiquid->SpellId);
        RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_NOT_UNDERWATER);
        _lastLiquid = NULL;
    }
}

void Unit::DeMorph()
{
    SetDisplayId(GetNativeDisplayId());
}

Aura* Unit::_TryStackingOrRefreshingExistingAura(SpellInfo const* newAura, uint32 effMask, Unit* caster, int32* baseAmount /*= NULL*/, Item* castItem /*= NULL*/, uint64 casterGUID /*= 0*/, int32 castItemLevel /*= -1*/)
{
    ASSERT(casterGUID || caster);
    if (!casterGUID)
        casterGUID = caster->GetGUID();

    // passive and Incanter's Absorption and auras with different type can stack with themselves any number of times
    if (!newAura->IsMultiSlotAura())
    {
        // check if cast item changed
        uint64 castItemGUID = 0;
        if (castItem)
            castItemGUID = castItem->GetGUID();

        // find current aura from spell and change it's stackamount, or refresh it's duration
        Aura* foundAura = GetOwnedAura(newAura->Id, casterGUID, (newAura->AttributesCu & SPELL_ATTR0_CU_ENCHANT_STACK) ? castItemGUID : 0, 0);
        if (foundAura != nullptr)
        {
            // effect masks do not match
            // extremely rare case
            // let's just recreate aura
            if (effMask != foundAura->GetEffectMask())
                return nullptr;

            // update basepoints with new values - effect amount will be recalculated in ModStackAmount
            for (uint8 i = 0; i < foundAura->GetEffectCount(); ++i)
            {
                if (!foundAura->HasEffect(i))
                    continue;

                int bp;
                if (baseAmount)
                    bp = *(baseAmount + i);
                else
                    bp = foundAura->GetSpellInfo()->Effects[i].BasePoints;

                int32* oldBP = const_cast<int32*>(&(foundAura->GetEffect(i)->m_baseAmount));
                *oldBP = bp;
            }

            // correct cast item guid if needed
            if (castItemGUID != foundAura->GetCastItemGUID())
            {
                uint64* oldGUID = const_cast<uint64 *>(&foundAura->m_castItemGuid);
                *oldGUID = castItemGUID;

                int32* oldItemLevel = const_cast<int32*>(&foundAura->m_castItemLevel);
                *oldItemLevel = castItemLevel;
            }

            // try to increase stack amount
            if (foundAura->GetId() != 980)
                foundAura->ModStackAmount(1);

            // Agony is refreshed at manual reapply
            if (foundAura->GetId() == 980)
            {
                foundAura->RefreshSpellMods();
                foundAura->RefreshTimers();
            }

            return foundAura;
        }
    }

    return nullptr;
}

void Unit::_AddAura(UnitAura* aura, Unit* caster)
{
    ASSERT(!m_cleanupDone);
    m_ownedAuras.insert(AuraMap::value_type(aura->GetId(), aura));

    _RemoveNoStackAurasDueToAura(aura);

    if (aura->IsRemoved())
        return;

    aura->SetIsSingleTarget(caster && aura->GetSpellInfo()->IsSingleTarget());
    if (aura->IsSingleTarget())
    {
        ASSERT((IsInWorld() && !IsDuringRemoveFromWorld()) || (aura->GetCasterGUID() == GetGUID()));
        // register single target aura
        caster->GetSingleCastAuras().push_back(aura);
        // remove other single target auras
        Unit::AuraList& scAuras = caster->GetSingleCastAuras();
        for (Unit::AuraList::iterator itr = scAuras.begin(); itr != scAuras.end();)
        {
            if ((*itr) != aura &&
                (*itr)->GetSpellInfo()->IsSingleTargetWith(aura->GetSpellInfo()))
            {
                (*itr)->Remove();
                itr = scAuras.begin();
            }
            else
                ++itr;
        }
    }
}

// creates aura application instance and registers it in lists
// aura application effects are handled separately to prevent aura list corruption
AuraApplication * Unit::_CreateAuraApplication(Aura* aura, uint32 effMask)
{
    // can't apply aura on unit which is going to be deleted - to not create a memory leak
    ASSERT(!m_cleanupDone);
    // aura musn't be removed
    ASSERT(!aura->IsRemoved());

    // aura mustn't be already applied on target
    ASSERT (!aura->IsAppliedOnTarget(GetGUID()) && "Unit::_CreateAuraApplication: aura musn't be applied on target");

    SpellInfo const* aurSpellInfo = aura->GetSpellInfo();
    uint32 aurId = aurSpellInfo->Id;

    // ghost spell check, allow apply any auras at player loading in ghost mode (will be cleanup after load)
    if (!isAlive() && !aurSpellInfo->IsDeathPersistent() &&
        (GetTypeId() != TYPEID_PLAYER || !ToPlayer()->GetSession()->PlayerLoading()))
        return NULL;

    Unit* caster = aura->GetCaster();

    AuraApplication * aurApp = new AuraApplication(this, caster, aura, effMask);
    m_appliedAuras.insert(AuraApplicationMap::value_type(aurId, aurApp));

    if (aurSpellInfo->AuraInterruptFlags)
    {
        m_interruptableAuras.push_back(aurApp);
        AddInterruptMask(aurSpellInfo->AuraInterruptFlags);
    }

    if (AuraStateType aState = aura->GetSpellInfo()->GetAuraState())
        m_auraStateAuras.insert(AuraStateAurasMap::value_type(uint32(aState), aurApp));

    aura->_ApplyForTarget(this, caster, aurApp);
    return aurApp;
}

void Unit::_ApplyAuraEffect(Aura* aura, uint32 effIndex)
{
    ASSERT(aura);
    ASSERT(aura->HasEffect(effIndex));
    AuraApplication * aurApp = aura->GetApplicationOfTarget(GetGUID());
    ASSERT(aurApp);
    if (!aurApp->GetEffectMask())
        _ApplyAura(aurApp, 1<<effIndex);
    else
        aurApp->_HandleEffect(effIndex, true);
}

// handles effects of aura application
// should be done after registering aura in lists
void Unit::_ApplyAura(AuraApplication* p_AurApp, uint32 p_EffMask)
{
    Aura* l_Aura = p_AurApp->GetBase();

    _RemoveNoStackAurasDueToAura(l_Aura);

    if (p_AurApp->GetRemoveMode())
        return;

    /// Update target aura state flag
    if (AuraStateType l_AState = l_Aura->GetSpellInfo()->GetAuraState())
        ModifyAuraState(l_AState, true);

    if (p_AurApp->GetRemoveMode())
        return;

    /// Sitdown on apply aura req seated
    if (l_Aura->GetSpellInfo()->AuraInterruptFlags & AURA_INTERRUPT_FLAG_NOT_SEATED && !IsSitState())
        SetStandState(UNIT_STAND_STATE_SIT);

    Unit* l_Caster = l_Aura->GetCaster();

    if (p_AurApp->GetRemoveMode())
        return;

    l_Aura->HandleAuraSpecificMods(p_AurApp, l_Caster, true, false);
    l_Aura->HandleAuraSpecificPeriodics(p_AurApp, l_Caster);

    /// Epicurean
    if (IsPlayer() &&
        (getRace() == RACE_PANDAREN_ALLI ||
        getRace() == RACE_PANDAREN_HORDE ||
        getRace() == RACE_PANDAREN_NEUTRAL))
    {
        if (l_Aura->GetSpellInfo()->AttributesEx2 & SPELL_ATTR2_FOOD_BUFF)
        {
            for (uint8 i = 0; i < l_Aura->GetEffectCount(); ++i)
            {
                if (l_Aura->GetEffect(i))
                    l_Aura->GetEffect(i)->ChangeAmount(l_Aura->GetEffect(i)->CalculateAmount(this) * 2);
            }
        }
    }

    /// apply effects of the aura
    for (uint8 i = 0; i < l_Aura->GetEffectCount(); ++i)
    {
        if (p_EffMask & 1<<i && (!p_AurApp->GetRemoveMode()))
            p_AurApp->_HandleEffect(i, true);
    }
}

// removes aura application from lists and unapplies effects
void Unit::_UnapplyAura(AuraApplicationMap::iterator &i, AuraRemoveMode removeMode)
{
    AuraApplication* aurApp = i->second;
    ASSERT(aurApp);
    ASSERT(!aurApp->GetRemoveMode());
    ASSERT(aurApp->GetTarget() == this);

    aurApp->SetRemoveMode(removeMode);
    Aura* aura = aurApp->GetBase();

    // dead loop is killing the server probably
    ASSERT(m_removedAurasCount < 0xFFFFFFFF);

    ++m_removedAurasCount;

    Unit* caster = aura->GetCaster();

    // Remove all pointers from lists here to prevent possible pointer invalidation on spellcast/auraapply/auraremove
    m_appliedAuras.erase(i);

    if (aura->GetSpellInfo()->AuraInterruptFlags)
    {
        m_interruptableAuras.remove(aurApp);
        UpdateInterruptMask();
    }

    bool auraStateFound = false;
    AuraStateType auraState = aura->GetSpellInfo()->GetAuraState();
    if (auraState)
    {
        bool canBreak = false;
        // Get mask of all aurastates from remaining auras
        for (AuraStateAurasMap::iterator itr = m_auraStateAuras.lower_bound(auraState); itr != m_auraStateAuras.upper_bound(auraState) && !(auraStateFound && canBreak);)
        {
            if (itr->second == aurApp)
            {
                m_auraStateAuras.erase(itr);
                itr = m_auraStateAuras.lower_bound(auraState);
                canBreak = true;
                continue;
            }
            auraStateFound = true;
            ++itr;
        }
    }

    aurApp->_Remove();
    aura->_UnapplyForTarget(this, caster, aurApp);

    // remove effects of the spell - needs to be done after removing aura from lists
    for (uint8 itr = 0; itr < aura->GetEffectCount(); ++itr)
    {
        if (aurApp->HasEffect(itr))
            aurApp->_HandleEffect(itr, false);
    }

    // all effect mustn't be applied
    ASSERT(!aurApp->GetEffectMask());

    // Remove totem at next update if totem loses its aura
    if (aurApp->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE && GetTypeId() == TYPEID_UNIT && ToCreature()->isTotem()&& ToTotem()->GetSummonerGUID() == aura->GetCasterGUID())
    {
        if (ToTotem()->GetSpell() == aura->GetId() && ToTotem()->GetTotemType() == TOTEM_PASSIVE)
            ToTotem()->setDeathState(JUST_DIED);
    }

    // Remove aurastates only if were not found
    if (!auraStateFound)
        ModifyAuraState(auraState, false);

    aura->HandleAuraSpecificMods(aurApp, caster, false, false);

    // only way correctly remove all auras from list
    //if (removedAuras != m_removedAurasCount) new aura may be added
        i = m_appliedAuras.begin();
}

void Unit::_UnapplyAura(AuraApplication * aurApp, AuraRemoveMode removeMode)
{
    // aura can be removed from unit only if it's applied on it, shouldn't happen
    ASSERT(aurApp->GetBase()->GetApplicationOfTarget(GetGUID()) == aurApp);
    uint32 spellId = aurApp->GetBase()->GetId();
    for (AuraApplicationMap::iterator iter = m_appliedAuras.lower_bound(spellId); iter != m_appliedAuras.upper_bound(spellId);)
    {
        if (iter->second == aurApp)
        {
            _UnapplyAura(iter, removeMode);
            return;
        }
        else
            ++iter;
    }
    ASSERT(false);
}

void Unit::_RemoveNoStackAurasDueToAura(Aura* aura)
{
    SpellInfo const* spellProto = aura->GetSpellInfo();

    // passive spell special case (only non stackable with ranks)
    if (spellProto->IsPassiveStackableWithRanks())
        return;

    bool remove = false;
    for (AuraApplicationMap::iterator i = m_appliedAuras.begin(); i != m_appliedAuras.end(); ++i)
    {
        if (remove)
        {
            remove = false;
            i = m_appliedAuras.begin();
        }

        if (aura->CanStackWith(i->second->GetBase()))
            continue;

        // Hack fix Poisoned Ammo stack with Serpent Sting
        if ((i->second->GetBase()->GetId() == 162543 && spellProto->Id == 118253) ||
            (i->second->GetBase()->GetId() == 118253 && spellProto->Id == 162543))
            continue;

        // Hack fix remove seal by consecration
        if ((i->second->GetBase()->GetId() == 105361 ||
            i->second->GetBase()->GetId() == 101423 ||
            i->second->GetBase()->GetId() == 31801 ||
            i->second->GetBase()->GetId() == 20165 ||
            i->second->GetBase()->GetId() == 20164)
            && spellProto->Id == 26573)
            continue;

        // Hack fix for Chakra remove
        if (spellProto->Id == 123267 &&
            (i->second->GetBase()->GetId() == 81206 ||
            i->second->GetBase()->GetId() == 81208 ||
            i->second->GetBase()->GetId() == 81209))
            continue;

        /// Hack fix for Corruption and Seed of Corruption
        if ((spellProto->Id == 27243 && i->second->GetBase()->GetId() == 146739) ||
            (spellProto->Id == 146739 && i->second->GetBase()->GetId() == 27243))
            continue;

        /// Hack fix for Sunfire, Rising Sun Kick and Stormstrike
        if (spellProto->Id == 164815 || spellProto->Id == 130320 || spellProto->Id == 17364)
            continue;

        RemoveAura(i, AURA_REMOVE_BY_DEFAULT);
        if (i == m_appliedAuras.end())
            break;
        remove = true;
    }
}

void Unit::_RegisterAuraEffect(AuraEffect* aurEff, bool apply)
{
    if (apply)
        m_modAuras[aurEff->GetAuraType()].push_back(aurEff);
    else
        m_modAuras[aurEff->GetAuraType()].remove(aurEff);
}

// All aura base removes should go threw this function!
void Unit::RemoveOwnedAura(AuraMap::iterator &i, AuraRemoveMode removeMode)
{
    Aura* aura = i->second;
    ASSERT(!aura->IsRemoved());

    // if unit currently update aura list then make safe update iterator shift to next
    if (m_auraUpdateIterator == i)
        ++m_auraUpdateIterator;

    m_ownedAuras.erase(i);
    m_removedAuras.push_back(aura);

    // Unregister single target aura
    if (aura->IsSingleTarget())
        aura->UnregisterSingleTarget();

    aura->_Remove(removeMode);

    i = m_ownedAuras.begin();
}

void Unit::RemoveOwnedAura(uint32 spellId, uint64 casterGUID, uint32 reqEffMask, AuraRemoveMode removeMode)
{
    for (AuraMap::iterator itr = m_ownedAuras.lower_bound(spellId); itr != m_ownedAuras.upper_bound(spellId);)
        if (((itr->second->GetEffectMask() & reqEffMask) == reqEffMask) && (!casterGUID || itr->second->GetCasterGUID() == casterGUID))
        {
            RemoveOwnedAura(itr, removeMode);
            itr = m_ownedAuras.lower_bound(spellId);
        }
        else
            ++itr;
}

void Unit::RemoveOwnedAura(Aura* aura, AuraRemoveMode removeMode)
{
    if (aura->IsRemoved())
        return;

    ASSERT(aura->GetOwner() == this);

    uint32 spellId = aura->GetId();
    for (AuraMap::iterator itr = m_ownedAuras.lower_bound(spellId); itr != m_ownedAuras.upper_bound(spellId); ++itr)
        if (itr->second == aura)
        {
            RemoveOwnedAura(itr, removeMode);
            return;
        }
    ASSERT(false);
}

Aura* Unit::GetOwnedAura(uint32 spellId, uint64 casterGUID, uint64 itemCasterGUID, uint32 reqEffMask, Aura* except) const
{
    for (AuraMap::const_iterator itr = m_ownedAuras.lower_bound(spellId); itr != m_ownedAuras.upper_bound(spellId); ++itr)
        if (((itr->second->GetEffectMask() & reqEffMask) == reqEffMask) && (!casterGUID || itr->second->GetCasterGUID() == casterGUID) && (!itemCasterGUID || itr->second->GetCastItemGUID() == itemCasterGUID) && (!except || except != itr->second))
            return itr->second;
    return nullptr;
}

void Unit::RemoveAura(AuraApplicationMap::iterator &i, AuraRemoveMode mode)
{
    AuraApplication * aurApp = i->second;
    // Do not remove aura which is already being removed
    if (aurApp->GetRemoveMode())
        return;
    Aura* aura = aurApp->GetBase();
    _UnapplyAura(i, mode);
    // Remove aura - for Area and Target auras
    if (aura->GetOwner() == this)
        aura->Remove(mode);
}

void Unit::RemoveAura(uint32 spellId, uint64 caster, uint32 reqEffMask, AuraRemoveMode removeMode)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.lower_bound(spellId); iter != m_appliedAuras.upper_bound(spellId);)
    {
        Aura const* aura = iter->second->GetBase();
        if (((aura->GetEffectMask() & reqEffMask) == reqEffMask)
            && (!caster || aura->GetCasterGUID() == caster))
        {
            RemoveAura(iter, removeMode);
            return;
        }
        else
            ++iter;
    }
}

void Unit::RemoveAura(AuraApplication * aurApp, AuraRemoveMode mode)
{
    // we've special situation here, RemoveAura called while during aura removal
    // this kind of call is needed only when aura effect removal handler
    // or event triggered by it expects to remove
    // not yet removed effects of an aura
    if (aurApp->GetRemoveMode())
    {
        // remove remaining effects of an aura
        for (uint8 itr = 0; itr < aurApp->GetEffectCount(); ++itr)
        {
            if (aurApp->HasEffect(itr))
                aurApp->_HandleEffect(itr, false);
        }
        return;
    }
    // no need to remove
    if (aurApp->GetBase()->GetApplicationOfTarget(GetGUID()) != aurApp || aurApp->GetBase()->IsRemoved())
        return;
    uint32 spellId = aurApp->GetBase()->GetId();

    if ((spellId == 51713 || spellId == 115192) && mode != AURA_REMOVE_BY_EXPIRE)
        return;

    for (AuraApplicationMap::iterator iter = m_appliedAuras.lower_bound(spellId); iter != m_appliedAuras.upper_bound(spellId);)
    {
        if (aurApp == iter->second)
        {
            RemoveAura(iter, mode);
            return;
        }
        else
            ++iter;
    }
}

void Unit::RemoveAura(Aura* aura, AuraRemoveMode mode)
{
    if (aura->IsRemoved())
        return;
    if (AuraApplication * aurApp = aura->GetApplicationOfTarget(GetGUID()))
        RemoveAura(aurApp, mode);
}

void Unit::RemoveAurasDueToSpell(uint32 spellId, uint64 casterGUID, uint32 reqEffMask, AuraRemoveMode removeMode)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.lower_bound(spellId); iter != m_appliedAuras.upper_bound(spellId);)
    {
        Aura const* aura = iter->second->GetBase();
        if (((aura->GetEffectMask() & reqEffMask) == reqEffMask)
            && (!casterGUID || aura->GetCasterGUID() == casterGUID))
        {
            RemoveAura(iter, removeMode);
            iter = m_appliedAuras.lower_bound(spellId);
        }
        else
            ++iter;
    }
}

void Unit::RemoveAuraFromStack(uint32 spellId, uint64 casterGUID, AuraRemoveMode removeMode)
{
    for (AuraMap::iterator iter = m_ownedAuras.lower_bound(spellId); iter != m_ownedAuras.upper_bound(spellId);)
    {
        Aura* aura = iter->second;
        if ((aura->GetType() == UNIT_AURA_TYPE)
            && (!casterGUID || aura->GetCasterGUID() == casterGUID))
        {
            aura->ModStackAmount(-1, removeMode);
            return;
        }
        else
            ++iter;
    }
}

void Unit::RemoveAurasDueToSpellByDispel(uint32 spellId, uint32 dispellerSpellId, uint64 casterGUID, Unit* dispeller, uint8 chargesRemoved/*= 1*/)
{
    for (AuraMap::iterator iter = m_ownedAuras.lower_bound(spellId); iter != m_ownedAuras.upper_bound(spellId);)
    {
        Aura* aura = iter->second;
        if (aura->GetCasterGUID() == casterGUID)
        {
            DispelInfo dispelInfo(dispeller, dispellerSpellId, chargesRemoved);

            // Call OnDispel hook on AuraScript
            aura->CallScriptDispel(&dispelInfo);

            if (aura->GetSpellInfo()->AttributesEx7 & SPELL_ATTR7_DISPEL_CHARGES)
                aura->ModCharges(-dispelInfo.GetRemovedCharges(), AURA_REMOVE_BY_ENEMY_SPELL);
            else
                aura->ModStackAmount(-dispelInfo.GetRemovedCharges(), AURA_REMOVE_BY_ENEMY_SPELL);

            // Call AfterDispel hook on AuraScript
            aura->CallScriptAfterDispel(&dispelInfo);

            return;
        }
        else
            ++iter;
    }
}

void Unit::RemoveAurasDueToSpellBySteal(uint32 spellId, uint64 casterGUID, Unit* stealer)
{
    for (AuraMap::iterator iter = m_ownedAuras.lower_bound(spellId); iter != m_ownedAuras.upper_bound(spellId);)
    {
        Aura* aura = iter->second;
        if (aura->GetCasterGUID() == casterGUID)
        {
            int32 damage[SpellEffIndex::MAX_EFFECTS];
            int32 baseDamage[SpellEffIndex::MAX_EFFECTS];
            uint32 effMask = 0;
            uint32 recalculateMask = 0;
            Unit* caster = aura->GetCaster();
            for (uint8 i = 0; i < aura->GetEffectCount(); ++i)
            {
                if (aura->GetEffect(i))
                {
                    baseDamage[i] = aura->GetEffect(i)->GetBaseAmount();
                    damage[i] = aura->GetEffect(i)->GetAmount();
                    effMask |= (1<<i);
                    if (aura->GetEffect(i)->CanBeRecalculated())
                        recalculateMask |= (1<<i);
                }
                else
                {
                    baseDamage[i] = 0;
                    damage[i] = 0;
                }
            }

            bool stealCharge = aura->GetSpellInfo()->AttributesEx7 & SPELL_ATTR7_DISPEL_CHARGES;
            // Cast duration to unsigned to prevent permanent aura's such as Righteous Fury being permanently added to caster
            uint32 dur = std::min(2u * MINUTE * IN_MILLISECONDS, uint32(aura->GetDuration()));

            Aura* oldAura = stealer->GetAura(aura->GetId(), aura->GetCasterGUID());
            if (oldAura != nullptr)
            {
                if (stealCharge)
                    oldAura->ModCharges(1);
                else
                    oldAura->ModStackAmount(1);
                oldAura->SetDuration(int32(dur));
            }
            else
            {
                // single target state must be removed before aura creation to preserve existing single target aura
                if (aura->IsSingleTarget())
                    aura->UnregisterSingleTarget();

                Aura* newAura = Aura::TryRefreshStackOrCreate(aura->GetSpellInfo(), effMask, stealer, NULL, &baseDamage[0], NULL, aura->GetCasterGUID());
                if (newAura != nullptr)
                {
                    // created aura must not be single target aura,, so stealer won't loose it on recast
                    if (newAura->IsSingleTarget())
                    {
                        newAura->UnregisterSingleTarget();
                        // bring back single target aura status to the old aura
                        aura->SetIsSingleTarget(true);
                        caster->GetSingleCastAuras().push_back(aura);
                    }
                    // FIXME: using aura->GetMaxDuration() maybe not blizzlike but it fixes stealing of spells like Innervate
                    newAura->SetLoadedState(aura->GetMaxDuration(), int32(dur), stealCharge ? 1 : aura->GetCharges(), 1, recalculateMask, &damage[0]);
                    newAura->ApplyForTargets();
                }
            }

            if (stealCharge)
                aura->ModCharges(-1, AURA_REMOVE_BY_ENEMY_SPELL);
            else
                aura->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);

            return;
        }
        else
            ++iter;
    }
}

void Unit::RemoveAurasDueToItemSpell(Item* castItem, uint32 spellId)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.lower_bound(spellId); iter != m_appliedAuras.upper_bound(spellId);)
    {
        if (!castItem || iter->second->GetBase()->GetCastItemGUID() == castItem->GetGUID())
        {
            RemoveAura(iter);
            iter = m_appliedAuras.upper_bound(spellId);          // overwrite by more appropriate
        }
        else
            ++iter;
    }
}

void Unit::RemoveAurasByType(AuraType auraType, uint64 casterGUID, Aura* exceptAura, uint32 exceptAuraId, bool negative, bool positive)
{
    for (AuraEffectList::iterator iter = m_modAuras[auraType].begin(); iter != m_modAuras[auraType].end();)
    {
        Aura* aura = (*iter)->GetBase();
        AuraApplication * aurApp = aura->GetApplicationOfTarget(GetGUID());

        if (!aurApp)
        {
            ++iter;
            continue;
        }

        ++iter;

        if (aura != exceptAura && aura->GetId() != exceptAuraId && (!casterGUID || aura->GetCasterGUID() == casterGUID)
            && ((negative && !aurApp->IsPositive()) || (positive && aurApp->IsPositive())))
        {
            uint32 removedAuras = m_removedAurasCount;
            RemoveAura(aurApp);
            if (m_removedAurasCount > removedAuras + 1)
                iter = m_modAuras[auraType].begin();
        }
    }
}

void Unit::RemoveEffectsByType(AuraType auraType, uint64 casterGUID, Aura* exceptAura, uint32 exceptAuraId, bool negative, bool positive)
{
    for (AuraEffectList::iterator iter = m_modAuras[auraType].begin(); iter != m_modAuras[auraType].end();)
    {
        Aura* aura = (*iter)->GetBase();
        AuraApplication * aurApp = aura->GetApplicationOfTarget(GetGUID());

        if (!aurApp)
        {
            ++iter;
            continue;
        }

        ++iter;

        if (aura != exceptAura && aura->GetId() != exceptAuraId && (!casterGUID || aura->GetCasterGUID() == casterGUID)
            && ((negative && !aurApp->IsPositive()) || (positive && aurApp->IsPositive())))
        {
            uint32 removedAuras = m_removedAurasCount;
            uint8 l_NbAuraEffectType = 0;
            uint8 l_NbAuraEffect = 0;

            for (; l_NbAuraEffect < SpellEffIndex::MAX_EFFECTS; ++l_NbAuraEffect)
            {
                if (AuraEffect* l_AuraEffect = GetAuraEffect(aurApp->GetBase()->GetSpellInfo()->Id, l_NbAuraEffect))
                {
                    if (l_AuraEffect->GetAuraType() == auraType)
                    {
                        l_AuraEffect->ChangeAmount(0);
                        ++l_NbAuraEffectType;
                    }
                }
            }

            if (l_NbAuraEffectType == l_NbAuraEffect)
            {
                RemoveAura(aurApp);
                if (m_removedAurasCount > removedAuras + 1)
                    iter = m_modAuras[auraType].begin();
            }
        }
    }
}

void Unit::RemoveAurasWithAttribute(uint32 flags)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        SpellInfo const* spell = iter->second->GetBase()->GetSpellInfo();
        if (spell->Attributes & flags)
            RemoveAura(iter);
        else
            ++iter;
    }
}

void Unit::RemoveNotOwnSingleTargetAuras(uint32 newPhase)
{
    // single target auras from other casters
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        AuraApplication const* aurApp = iter->second;
        Aura const* aura = aurApp->GetBase();

        if (aura->GetCasterGUID() != GetGUID() && aura->GetSpellInfo()->IsSingleTarget())
        {
            if (!newPhase)
                RemoveAura(iter);
            else
            {
                Unit* caster = aura->GetCaster();
                if (!caster || !caster->InSamePhase(newPhase))
                    RemoveAura(iter);
                else
                    ++iter;
            }
        }
        else
            ++iter;
    }

    // single target auras at other targets
    AuraList& scAuras = GetSingleCastAuras();
    for (AuraList::iterator iter = scAuras.begin(); iter != scAuras.end();)
    {
        Aura* aura = *iter;
        if (aura->GetUnitOwner() && aura->GetUnitOwner() != this && !aura->GetUnitOwner()->InSamePhase(newPhase))
        {
            aura->Remove();
            iter = scAuras.begin();
        }
        else
            ++iter;
    }
}

void Unit::RemoveAurasWithInterruptFlags(uint32 flag, uint32 except)
{
    if (!(m_interruptMask & flag))
        return;

    // interrupt auras
    for (AuraApplicationList::iterator iter = m_interruptableAuras.begin(); iter != m_interruptableAuras.end();)
    {
        Aura* aura = (*iter)->GetBase();
        ++iter;

        /// Censure DoT doesn't remove Blinding Light
        if (aura->GetSpellInfo()->Id == 105421 && except == 31803)
            continue;

        if ((aura->GetSpellInfo()->AuraInterruptFlags & flag) && (!except || aura->GetId() != except))
        {
            uint32 removedAuras = m_removedAurasCount;
            RemoveAura(aura);
            if (m_removedAurasCount > removedAuras + 1)
                iter = m_interruptableAuras.begin();
        }
    }

    // interrupt channeled spell
    if (Spell* spell = m_currentSpells[CURRENT_CHANNELED_SPELL])
    {
        if (spell->getState() == SPELL_STATE_CASTING
            && (spell->m_spellInfo->ChannelInterruptFlags & flag)
            && spell->m_spellInfo->Id != except
            && !(flag & AURA_INTERRUPT_FLAG_MOVE && (HasAuraTypeWithAffectMask(SPELL_AURA_CAST_WHILE_WALKING, spell->m_spellInfo) ||
            HasAuraType(SPELL_AURA_ALLOW_ALL_CASTS_WHILE_WALKING))))
        {
            /// Zen Meditation should be channeled, but apply a levitation aura, it handles a movement opcode
            if (spell->m_spellInfo->Id == 115176)
                return;

            InterruptNonMeleeSpells(false);
        }
    }

    UpdateInterruptMask();
}

void Unit::RemoveFlagsAuras()
{
    RemoveAura(131528); // Horde Insigna
    RemoveAura(131527); // Alliance Insigna

    if (Player* plr = ToPlayer())
    {
        if (Battleground* bg = plr->GetBattleground())
        {
            switch (bg->GetTypeID(true))
            {
                case BATTLEGROUND_KT:
                    ((BattlegroundKT*)bg)->EventPlayerDroppedOrb(plr);
                    break;
                case BATTLEGROUND_WS:
                    ((BattlegroundWS*)bg)->EventPlayerDroppedFlag(plr);
                    break;
                case BATTLEGROUND_TP:
                    ((BattlegroundTP*)bg)->EventPlayerDroppedFlag(plr);
                    break;
                case BATTLEGROUND_DG:
                    ((BattlegroundDG*)bg)->EventPlayerDroppedFlag(plr);
                    break;
                default:
                    break;
            }
        }
    }
}

void Unit::RemoveAurasWithFamily(SpellFamilyNames family, uint32 familyFlag1, uint32 familyFlag2, uint32 familyFlag3, uint64 casterGUID)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        Aura const* aura = iter->second->GetBase();
        if (!casterGUID || aura->GetCasterGUID() == casterGUID)
        {
            SpellInfo const* spell = aura->GetSpellInfo();
            if (spell->SpellFamilyName == uint32(family) && spell->SpellFamilyFlags.HasFlag(familyFlag1, familyFlag2, familyFlag3))
            {
                RemoveAura(iter);
                continue;
            }
        }
        ++iter;
    }
}

void Unit::RemoveMovementImpairingAuras()
{
    RemoveAurasWithMechanic((1<<MECHANIC_SNARE)|(1<<MECHANIC_ROOT));
}

void Unit::RemoveAurasBreakableByDamage()
{
    RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TAKE_DAMAGE);
    RemoveAurasWithAttribute(SPELL_ATTR0_BREAKABLE_BY_DAMAGE);

    // Hack fix for Paralysis, don't want to spend time for debuging why it doesn't remove
    if (HasAura(115078))
        RemoveAura(115078);
}

void Unit::RemoveAurasWithMechanic(uint32 mechanic_mask, AuraRemoveMode removemode, uint32 except, uint8 count)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        uint8 aurasCount = 0;
        Aura const* aura = iter->second->GetBase();

        /// Hackfix for Kargath Bladefist, Impale cannot be dispelled
        /// Maybe need to add a hook here
        if (aura->GetId() == 159113)
        {
            ++iter;
            continue;
        }

        if (!except || aura->GetId() != except)
        {
            if (aura->GetSpellInfo()->GetAllEffectsMechanicMask() & mechanic_mask)
            {
                RemoveAura(iter, removemode);
                aurasCount++;
                if (count && aurasCount == count)
                    break;

                continue;
            }
        }
        ++iter;
    }
}

void Unit::RemoveEffectsWithMechanic(uint32 mechanic_mask, AuraRemoveMode removemode, uint32 except)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        Aura const* aura = iter->second->GetBase();

        if (!except || aura->GetId() != except)
        {
            if (aura->GetSpellInfo()->GetAllEffectsMechanicMask() & mechanic_mask)
            {
                uint8 l_NbAuraEffectType = 0;
                uint8 l_NbAuraEffect = 0;

                for (uint8 i = 0; i < SpellEffIndex::MAX_EFFECTS; ++i)
                {
                    if (AuraEffect* l_AuraEffect = GetAuraEffect(iter->second->GetBase()->GetSpellInfo()->Id, i))
                    {
                        ++l_NbAuraEffect;
                        if (aura->GetSpellInfo()->GetEffectMechanicMask(i) & mechanic_mask)
                        {
                            l_AuraEffect->ChangeAmount(0);
                            ++l_NbAuraEffectType;
                        }
                    }
                }

                if (l_NbAuraEffectType == l_NbAuraEffect)
                    RemoveAura(iter, removemode);
            }
        }
        ++iter;
    }
}

void Unit::RemoveAreaAurasDueToLeaveWorld()
{
    // make sure that all area auras not applied on self are removed - prevent access to deleted pointer later
    for (AuraMap::iterator iter = m_ownedAuras.begin(); iter != m_ownedAuras.end();)
    {
        Aura* aura = iter->second;
        ++iter;
        Aura::ApplicationMap const& appMap = aura->GetApplicationMap();
        for (Aura::ApplicationMap::const_iterator itr = appMap.begin(); itr!= appMap.end();)
        {
            AuraApplication * aurApp = itr->second;
            ++itr;
            Unit* target = aurApp->GetTarget();
            if (target == this)
                continue;
            target->RemoveAura(aurApp);
            // things linked on aura remove may apply new area aura - so start from the beginning
            iter = m_ownedAuras.begin();
        }
    }

    /// We need to remove all area auras which are casted by the pet and owned by the pet owner
    if (isPet())
    {
        if (Unit* l_Owner = GetOwner())
        {
            Unit::AuraMap& auras = l_Owner->GetOwnedAuras();
            for (Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end();)
            {
                Aura* l_Aura = itr->second;
                ++itr;

                if (l_Aura->GetCasterGUID() != GetGUID())
                    continue;

                Aura::ApplicationMap const& l_AppMap = l_Aura->GetApplicationMap();
                for (Aura::ApplicationMap::const_iterator iter = l_AppMap.begin(); iter != l_AppMap.end();)
                {
                    AuraApplication * aurApp = iter->second;
                    ++iter;

                    Unit* l_Target = aurApp->GetTarget();
                    if (l_Target == this)
                        continue;

                    l_Target->RemoveAura(aurApp);

                    // things linked on aura remove may apply new area aura - so start from the beginning
                    itr  = auras.begin();
                    iter = l_AppMap.begin();
                }
            }
        }
    }

    // remove area auras owned by others
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        if (iter->second->GetBase()->GetOwner() != this)
        {
            RemoveAura(iter);
        }
        else
            ++iter;
    }
}

void Unit::RemoveAllAuras()
{
    // this may be a dead loop if some events on aura remove will continiously apply aura on remove
    // we want to have all auras removed, so use your brain when linking events
    while (!m_appliedAuras.empty() || !m_ownedAuras.empty())
    {
        AuraApplicationMap::iterator aurAppIter;
        for (aurAppIter = m_appliedAuras.begin(); aurAppIter != m_appliedAuras.end();)
            _UnapplyAura(aurAppIter, AURA_REMOVE_BY_DEFAULT);

        AuraMap::iterator aurIter;
        for (aurIter = m_ownedAuras.begin(); aurIter != m_ownedAuras.end();)
            RemoveOwnedAura(aurIter);
    }
}

void Unit::RemoveNonPassivesAuras()
{
    // this may be a dead loop if some events on aura remove will continiously apply aura on remove
    // we want to have all auras removed, so use your brain when linking events
    for (AuraApplicationMap::iterator aurAppIter = m_appliedAuras.begin(); aurAppIter != m_appliedAuras.end();)
    {
        Aura* aura = aurAppIter->second->GetBase();
        // Don't remove warsong's and twin peak's flags buffs
        if (!aura->IsPassive() && aura->GetId() != 23333 && aura->GetId() != 23335)
            _UnapplyAura(aurAppIter, AURA_REMOVE_BY_DEFAULT);
        else
            ++aurAppIter;
    }

    for (AuraMap::iterator aurIter = m_ownedAuras.begin(); aurIter != m_ownedAuras.end();)
    {
        Aura* aura = aurIter->second;
        // Don't remove warsong's and twin peak's flags buffs
        if (!aura->IsPassive() && aura->GetId() != 23333 && aura->GetId() != 23335)
            RemoveOwnedAura(aurIter);
        else
            ++aurIter;
    }
}

void Unit::RemoveArenaAuras()
{
    RemoveAura(77616);

    // in join, remove positive buffs, on end, remove negative
    // used to remove positive visible auras in arenas
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        AuraApplication const* aurApp = iter->second;
        Aura const* aura = aurApp->GetBase();
        if (!(aura->GetSpellInfo()->AttributesEx4 & SPELL_ATTR4_UNK21) // don't remove stances, shadowform, pally/hunter auras
            && !aura->IsPassive()                               // don't remove passive auras
            && (aurApp->IsPositive() || !(aura->GetSpellInfo()->AttributesEx3 & SPELL_ATTR3_DEATH_PERSISTENT))
            && !aurApp->GetRemoveMode()) // not negative death persistent auras
            RemoveAura(iter);
        else
            ++iter;
    }
}

void Unit::RemoveNegativeAuras()
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        AuraApplication const* aurApp = iter->second;
        Aura const* aura = aurApp->GetBase();
        if (!aurApp->IsPositive()                                       // don't remove stances, shadowform, pally/hunter auras
            && !aura->IsPassive()                                       // don't remove passive auras
            && !aura->IsDeathPersistent()                               // don't remove death persistent auras
            && !aurApp->GetRemoveMode())
            RemoveAura(iter);
        else
            ++iter;
    }
}

void Unit::RemoveAllAurasOnDeath()
{
    // used just after dieing to remove all visible auras
    // and disable the mods for the passive ones
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        Aura const* aura = iter->second->GetBase();
        if (!aura->IsPassive() && !aura->IsDeathPersistent())
            _UnapplyAura(iter, AURA_REMOVE_BY_DEATH);
        else
            ++iter;
    }

    for (AuraMap::iterator iter = m_ownedAuras.begin(); iter != m_ownedAuras.end();)
    {
        Aura* aura = iter->second;
        if (!aura->IsPassive() && !aura->IsDeathPersistent())
            RemoveOwnedAura(iter, AURA_REMOVE_BY_DEATH);
        else
            ++iter;
    }
}

void Unit::RemoveAllAurasRequiringDeadTarget()
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        Aura const* aura = iter->second->GetBase();
        if (!aura->IsPassive() && aura->GetSpellInfo()->IsRequiringDeadTarget())
            _UnapplyAura(iter, AURA_REMOVE_BY_DEFAULT);
        else
            ++iter;
    }

    for (AuraMap::iterator iter = m_ownedAuras.begin(); iter != m_ownedAuras.end();)
    {
        Aura* aura = iter->second;
        if (!aura->IsPassive() && aura->GetSpellInfo()->IsRequiringDeadTarget())
            RemoveOwnedAura(iter, AURA_REMOVE_BY_DEFAULT);
        else
            ++iter;
    }
}

void Unit::RemoveAllAurasExceptType(AuraType type)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        Aura const* aura = iter->second->GetBase();
        if (!aura->GetSpellInfo()->HasAura(type))
            _UnapplyAura(iter, AURA_REMOVE_BY_DEFAULT);
        else
            ++iter;
    }

    for (AuraMap::iterator iter = m_ownedAuras.begin(); iter != m_ownedAuras.end();)
    {
        Aura* aura = iter->second;
        if (!aura->GetSpellInfo()->HasAura(type))
            RemoveOwnedAura(iter, AURA_REMOVE_BY_DEFAULT);
        else
            ++iter;
    }
}

void Unit::RemoveAllAurasByType(AuraType type)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end();)
    {
        Aura const* aura = iter->second->GetBase();
        if (aura->GetSpellInfo()->HasAura(type))
            _UnapplyAura(iter, AURA_REMOVE_BY_DEFAULT);
        else
            ++iter;
    }

    for (AuraMap::iterator iter = m_ownedAuras.begin(); iter != m_ownedAuras.end();)
    {
        Aura* aura = iter->second;
        if (aura->GetSpellInfo()->HasAura(type))
            RemoveOwnedAura(iter, AURA_REMOVE_BY_DEFAULT);
        else
            ++iter;
    }
}

void Unit::RemoveAllAurasByCaster(uint64 p_Guid)
{
    for (AuraMap::iterator l_Iter = m_ownedAuras.begin(); l_Iter != m_ownedAuras.end();)
    {
        Aura* p_Aura = l_Iter->second;
        if (p_Aura->GetCasterGUID() == p_Guid)
            RemoveOwnedAura(l_Iter, AURA_REMOVE_BY_DEFAULT);
        else
            ++l_Iter;
    }
}

void Unit::DelayOwnedAuras(uint32 spellId, uint64 caster, int32 delaytime)
{
    for (AuraMap::iterator iter = m_ownedAuras.lower_bound(spellId); iter != m_ownedAuras.upper_bound(spellId);++iter)
    {
        Aura* aura = iter->second;
        if (!caster || aura->GetCasterGUID() == caster)
        {
            if (aura->GetDuration() < delaytime)
                aura->SetDuration(0);
            else
                aura->SetDuration(aura->GetDuration() - delaytime);

            // update for out of range group members (on 1 slot use)
            aura->SetNeedClientUpdateForTargets();
        }
    }
}

void Unit::_RemoveAllAuraStatMods()
{
    for (AuraApplicationMap::iterator i = m_appliedAuras.begin(); i != m_appliedAuras.end(); ++i)
        (*i).second->GetBase()->HandleAllEffects(i->second, AURA_EFFECT_HANDLE_STAT, false);
}

void Unit::_ApplyAllAuraStatMods()
{
    for (AuraApplicationMap::iterator i = m_appliedAuras.begin(); i != m_appliedAuras.end(); ++i)
        (*i).second->GetBase()->HandleAllEffects(i->second, AURA_EFFECT_HANDLE_STAT, true);
}

std::list<AuraEffect*> Unit::GetAuraEffectsByMechanic(uint32 mechanic_mask) const
{
    AuraEffectList list;
    for (AuraApplicationMap::const_iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end(); ++iter)
    {
        Aura const* aura = iter->second->GetBase();
        for (uint8 i = 0; i < SpellEffIndex::MAX_EFFECTS; ++i)
        {
            if (aura->GetSpellInfo()->GetEffectMechanicMask(i) & mechanic_mask)
            {
                if (iter->second)
                    if (iter->second->GetBase())
                        if (iter->second->GetBase()->GetEffect(i))
                            list.push_back(iter->second->GetBase()->GetEffect(i));
            }
        }
    }

    return list;
}

AuraEffect* Unit::GetAuraEffect(uint32 spellId, uint8 effIndex, uint64 caster) const
{
    for (AuraApplicationMap::const_iterator itr = m_appliedAuras.lower_bound(spellId); itr != m_appliedAuras.upper_bound(spellId); ++itr)
        if (itr->second->HasEffect(effIndex) && (!caster || itr->second->GetBase()->GetCasterGUID() == caster))
            return itr->second->GetBase()->GetEffect(effIndex);
    return nullptr;
}

AuraEffect* Unit::GetAuraEffectOfRankedSpell(uint32 spellId, uint8 effIndex, uint64 caster) const
{
    uint32 rankSpell = sSpellMgr->GetFirstSpellInChain(spellId);
    while (rankSpell)
    {
        if (AuraEffect* aurEff = GetAuraEffect(rankSpell, effIndex, caster))
            return aurEff;
        rankSpell = sSpellMgr->GetNextSpellInChain(rankSpell);
    }
    return nullptr;
}

AuraEffect* Unit::GetAuraEffect(AuraType type, SpellFamilyNames name, uint32 iconId, uint8 effIndex) const
{
    AuraEffectList const& auras = GetAuraEffectsByType(type);
    for (Unit::AuraEffectList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
    {
        if (effIndex != (*itr)->GetEffIndex())
            continue;
        SpellInfo const* spell = (*itr)->GetSpellInfo();
        if (spell->SpellIconID == iconId && spell->SpellFamilyName == uint32(name) && !spell->SpellFamilyFlags)
            return *itr;
    }
    return nullptr;
}

AuraEffect* Unit::GetAuraEffect(AuraType type, SpellFamilyNames family, uint32 familyFlag1, uint32 familyFlag2, uint32 familyFlag3, uint64 casterGUID)
{
    AuraEffectList const& auras = GetAuraEffectsByType(type);
    for (AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
    {
        SpellInfo const* spell = (*i)->GetSpellInfo();
        if (spell->SpellFamilyName == uint32(family) && spell->SpellFamilyFlags.HasFlag(familyFlag1, familyFlag2, familyFlag3))
        {
            if (casterGUID && (*i)->GetCasterGUID() != casterGUID)
                continue;
            return (*i);
        }
    }
    return nullptr;
}

AuraApplication * Unit::GetAuraApplication(uint32 spellId, uint64 casterGUID, uint64 itemCasterGUID, uint32 reqEffMask, AuraApplication * except) const
{
    for (AuraApplicationMap::const_iterator itr = m_appliedAuras.lower_bound(spellId); itr != m_appliedAuras.upper_bound(spellId); ++itr)
    {
        Aura const* aura = itr->second->GetBase();
        if (((aura->GetEffectMask() & reqEffMask) == reqEffMask) && (!casterGUID || aura->GetCasterGUID() == casterGUID) && (!itemCasterGUID || aura->GetCastItemGUID() == itemCasterGUID) && (!except || except != itr->second))
            return itr->second;
    }
    return NULL;
}

Aura* Unit::GetAura(uint32 spellId, uint64 casterGUID, uint64 itemCasterGUID, uint32 reqEffMask) const
{
    AuraApplication * aurApp = GetAuraApplication(spellId, casterGUID, itemCasterGUID, reqEffMask);
    return aurApp ? aurApp->GetBase() : nullptr;
}

AuraApplication * Unit::GetAuraApplicationOfRankedSpell(uint32 spellId, uint64 casterGUID, uint64 itemCasterGUID, uint32 reqEffMask, AuraApplication* except) const
{
    uint32 rankSpell = sSpellMgr->GetFirstSpellInChain(spellId);
    while (rankSpell)
    {
        if (AuraApplication * aurApp = GetAuraApplication(rankSpell, casterGUID, itemCasterGUID, reqEffMask, except))
            return aurApp;
        rankSpell = sSpellMgr->GetNextSpellInChain(rankSpell);
    }
    return NULL;
}

Aura* Unit::GetAuraOfRankedSpell(uint32 spellId, uint64 casterGUID, uint64 itemCasterGUID, uint32 reqEffMask) const
{
    AuraApplication * aurApp = GetAuraApplicationOfRankedSpell(spellId, casterGUID, itemCasterGUID, reqEffMask);
    return aurApp ? aurApp->GetBase() : nullptr;
}

void Unit::GetDispellableAuraList(Unit* caster, uint32 dispelMask, DispelChargesList& dispelList)
{
    // we should not be able to dispel diseases if the target is affected by unholy blight
    if (dispelMask & (1 << DISPEL_DISEASE) && HasAura(50536))
        dispelMask &= ~(1 << DISPEL_DISEASE);

    AuraMap const& auras = GetOwnedAuras();
    for (AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
    {
        Aura* aura = itr->second;
        AuraApplication * aurApp = aura->GetApplicationOfTarget(GetGUID());
        if (!aurApp)
            continue;

        // don't try to remove passive auras
        if (aura->IsPassive())
            continue;

        if (aura->GetSpellInfo()->GetDispelMask() & dispelMask)
        {
            if (aura->GetSpellInfo()->Dispel == DISPEL_MAGIC)
            {
                // do not remove positive auras if friendly target
                //               negative auras if non-friendly target
                if (aurApp->IsPositive() == IsFriendlyTo(caster))
                    continue;
            }

            // The charges / stack amounts don't count towards the total number of auras that can be dispelled.
            // Ie: A dispel on a target with 5 stacks of Winters Chill and a Polymorph has 1 / (1 + 1) -> 50% chance to dispell
            // Polymorph instead of 1 / (5 + 1) -> 16%.
            bool dispel_charges = aura->GetSpellInfo()->AttributesEx7 & SPELL_ATTR7_DISPEL_CHARGES;
            uint8 charges = dispel_charges ? aura->GetCharges() : aura->GetStackAmount();
            if (charges > 0)
                dispelList.push_back(std::make_pair(aura, charges));
        }
    }
}

StackOnDuration* Unit::GetStackOnDuration(uint32 p_SpellID)
{
    if (m_StackOnDurationMap.find(p_SpellID) != m_StackOnDurationMap.end())
        return &m_StackOnDurationMap[p_SpellID];

    return nullptr;
}

void Unit::AddToStackOnDuration(uint32 p_SpellID, uint64 p_DurationTime, int32 p_Amount)
{
    if (m_StackOnDurationMap.find(p_SpellID) == m_StackOnDurationMap.end())
        m_StackOnDurationMap.insert(std::make_pair(p_SpellID, StackOnDuration(p_DurationTime, p_Amount)));
    else
    {
        StackOnDuration* l_Stack = GetStackOnDuration(p_SpellID);
        l_Stack->m_StackDuration.push_back(std::make_pair(p_DurationTime, p_Amount));
    }
}

void Unit::RemoveStackOnDuration(uint32 p_SpellID)
{
    if (m_StackOnDurationMap.find(p_SpellID) == m_StackOnDurationMap.end())
        return;
    else
        m_StackOnDurationMap.erase(m_StackOnDurationMap.find(p_SpellID));
}

bool Unit::HasAuraEffect(uint32 spellId, uint8 effIndex, uint64 caster) const
{
    for (AuraApplicationMap::const_iterator itr = m_appliedAuras.lower_bound(spellId); itr != m_appliedAuras.upper_bound(spellId); ++itr)
        if (itr->second->HasEffect(effIndex) && (!caster || itr->second->GetBase()->GetCasterGUID() == caster))
            return true;
    return false;
}

uint32 Unit::GetAuraCount(uint32 spellId) const
{
    uint32 count = 0;
    for (AuraApplicationMap::const_iterator itr = m_appliedAuras.lower_bound(spellId); itr != m_appliedAuras.upper_bound(spellId); ++itr)
    {
        if (!itr->second->GetBase()->GetStackAmount())
            count++;
        else
            count += (uint32)itr->second->GetBase()->GetStackAmount();
    }
    return count;
}

bool Unit::HasAura(uint32 spellId, uint64 casterGUID, uint64 itemCasterGUID, uint32 reqEffMask) const
{
    if (this == nullptr) ///<  'this' pointer cannot be null in well-defined C++ code; comparison may be assumed to always evaluate to false
    {
        sLog->outAshran("Unit::HasAura(spellId: %u) this == nullptr", spellId);
        return false;
    }

    if (GetAuraApplication(spellId, casterGUID, itemCasterGUID, reqEffMask))
        return true;
    return false;
}

bool Unit::HasAuraType(AuraType auraType) const
{
    return (!m_modAuras[auraType].empty());
}

bool Unit::HasAuraTypeWithCaster(AuraType auratype, uint64 caster) const
{
    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
        if (caster == (*i)->GetCasterGUID())
            return true;
    return false;
}

bool Unit::HasAuraTypeWithMiscvalue(AuraType auratype, int32 miscvalue) const
{
    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
        if (miscvalue == (*i)->GetMiscValue())
            return true;
    return false;
}

bool Unit::HasAuraTypeWithAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const
{
    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
        if ((*i)->IsAffectingSpell(affectedSpell))
            return true;
    return false;
}

bool Unit::HasAuraTypeWithValue(AuraType auratype, int32 value) const
{
    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
        if (value == (*i)->GetAmount())
            return true;
    return false;
}

bool Unit::HasNegativeAuraWithInterruptFlag(uint32 flag, uint64 guid)
{
    if (!(m_interruptMask & flag))
        return false;
    for (AuraApplicationList::iterator iter = m_interruptableAuras.begin(); iter != m_interruptableAuras.end(); ++iter)
    {
        if (!(*iter)->IsPositive() && (*iter)->GetBase()->GetSpellInfo()->AuraInterruptFlags & flag && (!guid || (*iter)->GetBase()->GetCasterGUID() == guid))
            return true;
    }
    return false;
}

bool Unit::HasNegativeAuraWithAttribute(uint32 flag, uint64 guid)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end(); ++iter)
    {
        Aura const* aura = iter->second->GetBase();
        if (!iter->second->IsPositive() && aura->GetSpellInfo()->Attributes & flag && (!guid || aura->GetCasterGUID() == guid))
            return true;
    }
    return false;
}

bool Unit::HasAuraWithNegativeCaster(uint32 spellid)
{
    for (AuraApplicationMap::const_iterator itr = m_appliedAuras.lower_bound(spellid); itr != m_appliedAuras.upper_bound(spellid); ++itr)
    {
        if (Unit * caster = (itr->second->GetBase()->GetCaster()))
        {
            if (!caster->IsFriendlyTo(this))
                return true;
        }
    }
    return false;
}

bool Unit::HasAuraWithMechanic(uint32 mechanicMask)
{
    for (AuraApplicationMap::iterator iter = m_appliedAuras.begin(); iter != m_appliedAuras.end(); ++iter)
    {
        SpellInfo const* spellInfo  = iter->second->GetBase()->GetSpellInfo();
        if (spellInfo->Mechanic && (mechanicMask & (1 << spellInfo->Mechanic)))
            return true;

        for (uint8 i = 0; i < spellInfo->EffectCount; ++i)
            if (iter->second->HasEffect(i) && spellInfo->Effects[i].Effect && spellInfo->Effects[i].Mechanic)
                if (mechanicMask & (1 << spellInfo->Effects[i].Mechanic))
                    return true;
    }

    return false;
}

AuraEffect* Unit::IsScriptOverriden(SpellInfo const* spell, int32 script) const
{
    AuraEffectList const& auras = GetAuraEffectsByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
    for (AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
    {
        if ((*i)->GetMiscValue() == script)
            if ((*i)->IsAffectingSpell(spell))
                return (*i);
    }
    return nullptr;
}

uint32 Unit::GetDiseasesByCaster(uint64 casterGUID, bool remove)
{
    static const AuraType diseaseAuraTypes[] =
    {
        SPELL_AURA_PERIODIC_DAMAGE, // Frost Fever and Blood Plague
        SPELL_AURA_LINKED,          // Crypt Fever and Ebon Plague
        SPELL_AURA_NONE
    };

    uint32 diseases = 0;
    for (AuraType const* itr = &diseaseAuraTypes[0]; itr && itr[0] != SPELL_AURA_NONE; ++itr)
    {
        for (AuraEffectList::iterator i = m_modAuras[*itr].begin(); i != m_modAuras[*itr].end();)
        {
            // Get auras with disease dispel type by caster
            if ((*i)->GetSpellInfo()->Dispel == DISPEL_DISEASE
                && (*i)->GetCasterGUID() == casterGUID)
            {
                ++diseases;

                if (remove)
                {
                    RemoveAura((*i)->GetId(), (*i)->GetCasterGUID());
                    i = m_modAuras[*itr].begin();
                    continue;
                }
            }
            ++i;
        }
    }

    // Burning Blood, Item - Death Knight T12 Blood 2P Bonus
    if (HasAura(98957, casterGUID))
    {
        uint32 _min = 2;
        diseases = std::max(diseases, _min);
    }

    return diseases;
}

uint32 Unit::GetDoTsByCaster(uint64 casterGUID) const
{
    static const AuraType diseaseAuraTypes[] =
    {
        SPELL_AURA_PERIODIC_DAMAGE,
        SPELL_AURA_PERIODIC_DAMAGE_PERCENT,
        SPELL_AURA_NONE
    };

    uint32 dots = 0;
    for (AuraType const* itr = &diseaseAuraTypes[0]; itr && itr[0] != SPELL_AURA_NONE; ++itr)
    {
        Unit::AuraEffectList const& auras = GetAuraEffectsByType(*itr);
        for (AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
        {
            // Get auras by caster
            if ((*i)->GetCasterGUID() == casterGUID)
                ++dots;
        }
    }
    return dots;
}

int32 Unit::GetTotalAuraModifier(AuraType auratype, AuraEffect const* excludeAura /* nullptr*/, AuraEffect* includeAura /* nullptr*/) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    int32 modifier = 0;

    AuraEffectList auras(GetAuraEffectsByType(auratype));
    if (includeAura)
    {
        AuraEffectList::const_iterator iter = find(auras.begin(), auras.end(), includeAura);
        if (iter == auras.end())
            auras.push_back((includeAura));
    }

    for (AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
        if ((*i) != excludeAura)
             if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                 modifier += (*i)->GetAmount();

    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        modifier += itr->second;

    return modifier;
}

float Unit::GetTotalAuraMultiplier(AuraType auratype) const
{
    float multiplier = 1.0f;

    std::map<SpellGroup, int32> SameEffectSpellGroup;
    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        /// Check if the Aura Effect has a the Same Effect Stack Rule and if so, use the highest amount of that SpellGroupSameEffectSpellGrou
        /// If the Aura Effect does not have this Stack Rule, it returns false so we can add to the multiplier as usual
        if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
            AddPct(multiplier, (*i)->GetAmount());
    }
    /// Add the highest of the Same Effect Stack Rule SpellGroups to the multiplier
    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        AddPct(multiplier, itr->second);

    return multiplier;
}

int32 Unit::GetMaxPositiveAuraModifier(AuraType auratype)
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->GetAmount() > modifier)
            modifier = (*i)->GetAmount();
    }

    return modifier;
}

int32 Unit::GetMaxNegativeAuraModifier(AuraType auratype) const
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->GetAmount() < modifier)
        {
            if ((*i)->GetBase()->GetId() == 116 && auratype == SPELL_AURA_MOD_DECREASE_SPEED) // Frostbolt speed reduction is always at 50%
                modifier = (*i)->GetBaseAmount();
            else
                modifier = (*i)->GetAmount();
        }
    }

    return modifier;
}

int32 Unit::GetTotalAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask, AuraEffect const* excludeAura /* nullptr*/, AuraEffect* includeAura /* nullptr*/) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    int32 modifier = 0;

    AuraEffectList auras(GetAuraEffectsByType(auratype));
    if (includeAura)
    {
        AuraEffectList::const_iterator iter = find(auras.begin(), auras.end(), includeAura);
        if (iter == auras.end())
            auras.push_back((includeAura));
    }

    for (AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
         if ((*i)->GetMiscValue() & misc_mask && (*i) != excludeAura)
             if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                 modifier += (*i)->GetAmount();

    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        modifier += itr->second;

    return modifier;
}

int32 Unit::GetTotalAuraModifierByMiscBMask(AuraType auratype, uint32 misc_mask, AuraEffect const* excludeAura /* nullptr*/, AuraEffect* includeAura /* nullptr*/) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    int32 modifier = 0;

    AuraEffectList auras(GetAuraEffectsByType(auratype));

    if (includeAura)
    {
        AuraEffectList::const_iterator iter = find(auras.begin(), auras.end(), includeAura);
        if (iter == auras.end())
            auras.push_back((includeAura));
    }

    for (AuraEffectList::const_iterator i = auras.begin(); i != auras.end(); ++i)
         if ((*i)->GetMiscValueB() & misc_mask && (*i) != excludeAura)
             if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                 modifier += (*i)->GetAmount();

    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        modifier += itr->second;

    return modifier;
}

float Unit::GetTotalAuraMultiplierByMiscMask(AuraType auratype, uint32 misc_mask) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    float multiplier = 1.0f;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if (((*i)->GetMiscValue() & misc_mask))
        {
            // Check if the Aura Effect has a the Same Effect Stack Rule and if so, use the highest amount of that SpellGroup
            // If the Aura Effect does not have this Stack Rule, it returns false so we can add to the multiplier as usual
            if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                AddPct(multiplier, (*i)->GetAmount());
        }
    }
    // Add the highest of the Same Effect Stack Rule SpellGroups to the multiplier
    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        AddPct(multiplier, itr->second);

    return multiplier;
}

int32 Unit::GetMaxPositiveAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask, AuraEffect const* except) const
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if (except != (*i) && (*i)->GetMiscValue()& misc_mask && (*i)->GetAmount() > modifier)
            modifier = (*i)->GetAmount();
    }

    return modifier;
}

int32 Unit::GetMaxNegativeAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask) const
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->GetMiscValue()& misc_mask && (*i)->GetAmount() < modifier)
            modifier = (*i)->GetAmount();
    }

    return modifier;
}

int32 Unit::GetTotalAuraModifierByMiscValue(AuraType auratype, int32 misc_value) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->GetMiscValue() == misc_value)
            if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                modifier += (*i)->GetAmount();
    }

    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        modifier += itr->second;

    return modifier;
}

float Unit::GetTotalAuraMultiplierByMiscValue(AuraType auratype, int32 misc_value) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    float multiplier = 1.0f;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->GetMiscValue() == misc_value)
            if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                AddPct(multiplier, (*i)->GetAmount());
    }

    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        AddPct(multiplier, itr->second);

    return multiplier;
}

int32 Unit::GetMaxPositiveAuraModifierByMiscValue(AuraType auratype, int32 misc_value) const
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->GetMiscValue() == misc_value && (*i)->GetAmount() > modifier)
            modifier = (*i)->GetAmount();
    }

    return modifier;
}

int32 Unit::GetMaxNegativeAuraModifierByMiscValue(AuraType auratype, int32 misc_value) const
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->GetMiscValue() == misc_value && (*i)->GetAmount() < modifier)
            modifier = (*i)->GetAmount();
    }

    return modifier;
}

int32 Unit::GetTotalAuraModifierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->IsAffectingSpell(affectedSpell))
            if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                modifier += (*i)->GetAmount();
    }

    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        modifier += itr->second;

    return modifier;
}

float Unit::GetTotalAuraMultiplierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const
{
    std::map<SpellGroup, int32> SameEffectSpellGroup;
    float multiplier = 1.0f;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->IsAffectingSpell(affectedSpell))
            if (!sSpellMgr->AddSameEffectStackRuleSpellGroups((*i)->GetSpellInfo(), (*i)->GetAmount(), SameEffectSpellGroup))
                AddPct(multiplier, (*i)->GetAmount());
    }

    for (std::map<SpellGroup, int32>::const_iterator itr = SameEffectSpellGroup.begin(); itr != SameEffectSpellGroup.end(); ++itr)
        AddPct(multiplier, itr->second);

    return multiplier;
}

int32 Unit::GetMaxPositiveAuraModifierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->IsAffectingSpell(affectedSpell) && (*i)->GetAmount() > modifier)
            modifier = (*i)->GetAmount();
    }

    return modifier;
}

int32 Unit::GetMaxNegativeAuraModifierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const
{
    int32 modifier = 0;

    AuraEffectList const& mTotalAuraList = GetAuraEffectsByType(auratype);
    for (AuraEffectList::const_iterator i = mTotalAuraList.begin(); i != mTotalAuraList.end(); ++i)
    {
        if ((*i)->IsAffectingSpell(affectedSpell) && (*i)->GetAmount() < modifier)
            modifier = (*i)->GetAmount();
    }

    return modifier;
}

void Unit::_RegisterDynObject(DynamicObject* dynObj)
{
    m_dynObj.push_back(dynObj);
}

void Unit::_RegisterAreaTrigger(AreaTrigger* areaTrigger)
{
    m_AreaTrigger.push_back(areaTrigger);
}

void Unit::_UnregisterDynObject(DynamicObject* dynObj)
{
    m_dynObj.remove(dynObj);
}

void Unit::_UnregisterAreaTrigger(AreaTrigger* areaTrigger)
{
    m_AreaTrigger.remove(areaTrigger);
}

DynamicObject* Unit::GetDynObject(uint32 spellId)
{
    if (m_dynObj.empty())
        return NULL;
    for (DynObjectList::const_iterator i = m_dynObj.begin(); i != m_dynObj.end();++i)
    {
        DynamicObject* dynObj = *i;
        if (dynObj->GetSpellId() == spellId)
            return dynObj;
    }
    return NULL;
}

AreaTrigger* Unit::GetAreaTrigger(uint32 spellId)
{
    if (m_AreaTrigger.empty())
        return NULL;
    for (AreaTriggerList::const_iterator i = m_AreaTrigger.begin(); i != m_AreaTrigger.end();++i)
    {
        AreaTrigger* areaTrigger = *i;
        if (areaTrigger->GetSpellId() == spellId)
            return areaTrigger;
    }
    return NULL;
}

int32 Unit::CountDynObject(uint32 spellId)
{
    int32 count = 0;

    if (m_dynObj.empty())
        return 0;
    for (DynObjectList::const_iterator i = m_dynObj.begin(); i != m_dynObj.end();++i)
    {
        DynamicObject* dynObj = *i;
        if (dynObj->GetSpellId() == spellId)
            count++;
    }
    return count;
}

int32 Unit::CountAreaTrigger(uint32 spellId)
{
    int32 count = 0;

    if (m_AreaTrigger.empty())
        return 0;
    for (AreaTriggerList::const_iterator i = m_AreaTrigger.begin(); i != m_AreaTrigger.end();++i)
    {
        AreaTrigger* areaTrigger = *i;
        if (areaTrigger->GetSpellId() == spellId)
            count++;
    }
    return count;
}

void Unit::GetDynObjectList(std::list<DynamicObject*> &list, uint32 spellId)
{
    if (m_dynObj.empty())
        return;
    for (DynObjectList::const_iterator i = m_dynObj.begin(); i != m_dynObj.end();++i)
    {
        DynamicObject* dynObj = *i;
        if (dynObj->GetSpellId() == spellId)
            list.push_back(dynObj);
    }
}

void Unit::GetAreaTriggerList(std::list<AreaTrigger*> &list, uint32 spellId)
{
    if (m_AreaTrigger.empty())
        return;
    for (AreaTriggerList::const_iterator i = m_AreaTrigger.begin(); i != m_AreaTrigger.end();++i)
    {
        AreaTrigger* areaTrigger = *i;
        if (areaTrigger->GetSpellId() == spellId)
            list.push_back(areaTrigger);
    }
}

void Unit::RemoveDynObject(uint32 spellId)
{
    if (m_dynObj.empty())
        return;
    for (DynObjectList::iterator i = m_dynObj.begin(); i != m_dynObj.end();)
    {
        DynamicObject* dynObj = *i;
        if (dynObj->GetSpellId() == spellId)
        {
            dynObj->Remove();
            i = m_dynObj.begin();
        }
        else
            ++i;
    }
}

void Unit::RemoveAreaTrigger(uint32 spellId)
{
    if (m_AreaTrigger.empty())
        return;
    for (AreaTriggerList::iterator i = m_AreaTrigger.begin(); i != m_AreaTrigger.end();)
    {
        AreaTrigger* areaTrigger = *i;
        if (areaTrigger->GetSpellId() == spellId)
        {
            areaTrigger->Remove(0);
            i = m_AreaTrigger.begin();
        }
        else
            ++i;
    }
}

void Unit::RemoveAllDynObjects()
{
    while (!m_dynObj.empty())
        m_dynObj.front()->Remove();
}

void Unit::RemoveAllAreasTrigger()
{
    while (!m_AreaTrigger.empty())
        m_AreaTrigger.front()->Remove(0);
}

GameObject* Unit::GetGameObject(uint32 spellId) const
{
    for (GameObjectList::const_iterator i = m_gameObj.begin(); i != m_gameObj.end(); ++i)
        if ((*i)->GetSpellId() == spellId)
            return *i;

    return NULL;
}

void Unit::AddGameObject(GameObject* gameObj)
{
    if (!gameObj || !gameObj->GetOwnerGUID() == 0) ///< Logical not is only applied to the left hand side of this comparison
        return;

    m_gameObj.push_back(gameObj);
    gameObj->SetOwnerGUID(GetGUID());

    if (IsPlayer() && gameObj->GetSpellId())
    {
        SpellInfo const* createBySpell = sSpellMgr->GetSpellInfo(gameObj->GetSpellId());
        // Need disable spell use for owner
        if (createBySpell && createBySpell->IsCooldownStartedOnEvent())
            // note: item based cooldowns and cooldown spell mods with charges ignored (unknown existing cases)
            ToPlayer()->AddSpellAndCategoryCooldowns(createBySpell, 0, NULL, true);
    }
}

void Unit::RemoveGameObject(GameObject* gameObj, bool del)
{
    if (!gameObj || gameObj->GetOwnerGUID() != GetGUID())
        return;

    gameObj->SetOwnerGUID(0);

    for (uint8 i = 0; i < MAX_GAMEOBJECT_SLOT; ++i)
    {
        if (m_ObjectSlot[i] == gameObj->GetGUID())
        {
            m_ObjectSlot[i] = 0;
            break;
        }
    }

    // GO created by some spell
    if (uint32 spellid = gameObj->GetSpellId())
    {
        RemoveAurasDueToSpell(spellid);

        if (IsPlayer())
        {
            SpellInfo const* createBySpell = sSpellMgr->GetSpellInfo(spellid);
            // Need activate spell use for owner
            if (createBySpell && createBySpell->IsCooldownStartedOnEvent())
                // note: item based cooldowns and cooldown spell mods with charges ignored (unknown existing cases)
                ToPlayer()->SendCooldownEvent(createBySpell);
        }
    }

    m_gameObj.remove(gameObj);

    if (del)
    {
        gameObj->SetRespawnTime(0);
        gameObj->Delete();
    }
}

void Unit::RemoveGameObject(uint32 spellid, bool del)
{
    if (m_gameObj.empty())
        return;
    GameObjectList::iterator i, next;
    for (i = m_gameObj.begin(); i != m_gameObj.end(); i = next)
    {
        next = i;
        if (spellid == 0 || (*i)->GetSpellId() == spellid)
        {
            (*i)->SetOwnerGUID(0);
            if (del)
            {
                (*i)->SetRespawnTime(0);
                (*i)->Delete();
            }

            next = m_gameObj.erase(i);
        }
        else
            ++next;
    }
}

void Unit::RemoveAllGameObjects()
{
    // remove references to unit
    while (!m_gameObj.empty())
    {
        GameObjectList::iterator i = m_gameObj.begin();
        (*i)->SetOwnerGUID(0);
        (*i)->SetRespawnTime(0);
        (*i)->Delete();
        m_gameObj.erase(i);
    }
}

void Unit::SendSpellNonMeleeDamageLog(SpellNonMeleeDamage* log)
{
    WorldPacket data(SMSG_SPELL_NON_MELEE_DAMAGE_LOG, 73);  // we guess size (73 is from sniffs without debug flag)

    int32 overkill = log->damage - log->target->GetHealth();

    // target is sended twice
    ObjectGuid target = log->target->GetGUID();
    ObjectGuid caster = log->attacker->GetGUID();

    data.appendPackGUID(target);
    data.appendPackGUID(caster);
    data << uint32(log->SpellID);
    data << uint32(log->damage);                            // damage amount
    data << uint32(overkill > 0 ? overkill : 0);            // overkill
    data << uint8(log->schoolMask);                         // damage school
    data << uint32(log->absorb);
    data << uint32(log->resist);
    data << uint32(log->blocked);

    data.WriteBit(false);               ///< Is periodic
    data.WriteBits(log->HitInfo, 8);    ///< Flags
    data.WriteBit(false);               ///< Has debug info
    data.WriteBit(false);               ///< Has JamSpellCastLogData
    data.FlushBits();

    SendMessageToSet(&data, true);
}

void Unit::SendSpellNonMeleeDamageLog(Unit* target, uint32 SpellID, uint32 Damage, SpellSchoolMask damageSchoolMask, uint32 AbsorbedDamage, uint32 Resist, bool PhysicalDamage, uint32 Blocked, bool CriticalHit)
{
    SpellNonMeleeDamage log(this, target, SpellID, damageSchoolMask);
    log.damage = Damage - AbsorbedDamage - Resist - Blocked;
    log.absorb = AbsorbedDamage;
    log.resist = Resist;
    log.physicalLog = PhysicalDamage;
    log.blocked = Blocked;
    if (CriticalHit)
        log.HitInfo |= SPELL_HIT_TYPE_CRIT;
    SendSpellNonMeleeDamageLog(&log);
}

void Unit::ProcDamageAndSpell(Unit* victim, uint32 procAttacker, uint32 procVictim, uint32 procEx, uint32 amount, uint32 absorb /*= 0*/, WeaponAttackType attType /*= BASE_ATTACK*/, SpellInfo const* procSpell /*= NULL*/, SpellInfo const* procAura /*= NULL*/, AuraEffect const* ownerAuraEffect /*= NULL*/)
{
     // Not much to do if no flags are set.
    if (procAttacker)
        ProcDamageAndSpellFor(false, victim, procAttacker, procEx, attType, procSpell, amount, absorb, procAura, ownerAuraEffect);
    // Now go on with a victim's events'n'auras
    // Not much to do if no flags are set or there is no victim
    if (victim && victim->isAlive() && procVictim)
        victim->ProcDamageAndSpellFor(true, this, procVictim, procEx, attType, procSpell, amount, absorb, procAura, ownerAuraEffect);
}

void Unit::SendPeriodicAuraLog(SpellPeriodicAuraLogInfo* p_Info)
{
    AuraEffect const* l_Aura = p_Info->auraEff;

    uint32 l_Amount              = 0;
    uint32 l_Resisted            = 0;
    uint32 l_OverHealOrKill      = 0;
    uint32 l_SchoolMaskOrPower   = 0;
    uint32 l_AbsorbedOrAmplitude = 0;

    switch (l_Aura->GetAuraType())
    {
        case SPELL_AURA_PERIODIC_DAMAGE:
        case SPELL_AURA_PERIODIC_DAMAGE_PERCENT:
            l_Amount                = p_Info->damage;
            l_OverHealOrKill        = p_Info->overDamage;
            l_SchoolMaskOrPower     = l_Aura->GetSpellInfo()->GetSchoolMask();
            l_AbsorbedOrAmplitude   = p_Info->absorb;
            l_Resisted              = p_Info->resist;
            break;

        case SPELL_AURA_PERIODIC_HEAL:
        case SPELL_AURA_OBS_MOD_HEALTH:
            l_Amount                = p_Info->damage;
            l_OverHealOrKill        = p_Info->overDamage;
            l_SchoolMaskOrPower     = l_Aura->GetSpellInfo()->GetSchoolMask();
            l_AbsorbedOrAmplitude   = p_Info->absorb;
            break;

        case SPELL_AURA_OBS_MOD_POWER:
        case SPELL_AURA_PERIODIC_ENERGIZE:
            l_Amount            = p_Info->damage;
            l_SchoolMaskOrPower = l_Aura->GetMiscValue();
            break;

        case SPELL_AURA_PERIODIC_MANA_LEECH:
            l_Amount            = p_Info->damage;
            l_SchoolMaskOrPower = l_Aura->GetMiscValue();
            break;

        default:
            break;
    }

    WorldPacket l_Data(SMSG_SPELL_PERIODIC_AURA_LOG, 100);
    l_Data.appendPackGUID(GetGUID());                           ///< Target GUID
    l_Data.appendPackGUID(l_Aura->GetCasterGUID());             ///< Caster GUID
    l_Data << uint32(l_Aura->GetId());                          ///< Spell ID
    l_Data << uint32(1);                                        ///< Entries Count

    /// First JamPeriodicAuraLogEffect
    l_Data << uint32(l_Aura->GetAuraType());                    ///< Effect
    l_Data << uint32(l_Amount);                                 ///< Amount
    l_Data << uint32(l_OverHealOrKill);                         ///< Over Heal Or Kill
    l_Data << uint32(l_SchoolMaskOrPower);                      ///< School Mask Or Power
    l_Data << uint32(l_AbsorbedOrAmplitude);                    ///< Absorbed Or Amplitude
    l_Data << uint32(l_Resisted);                               ///< Resisted

    l_Data.WriteBit(p_Info->critical);                          ///< Crit
    l_Data.WriteBit(p_Info->multistrike);                       ///< Multistrike
    l_Data.WriteBit(false);                                     ///< Has Debug Info
    l_Data.FlushBits();

    l_Data.WriteBit(0);                                         ///< Has Log Data
    l_Data.FlushBits();

    SendMessageToSet(&l_Data, true);
}

/// Build and send SMSG_SPELL_MISS_LOG packet
/// @p_Target   : Target for the missed spell
/// @p_SpellId  : Missed spell ID
/// @p_MissInfo : Miss type
void Unit::SendSpellMiss(Unit* p_Target, uint32 p_SpellID, SpellMissInfo p_MissInfo)
{
    WorldPacket l_Data(SMSG_SPELL_MISS_LOG, 4 + 16 + 2 + 1 + 16 + 2 + 1 + 1);
    l_Data << uint32(p_SpellID);                                    ///< SpellID
    l_Data.appendPackGUID(GetGUID());                               ///< Caster
    l_Data << uint32(1);                                            ///< EntriesCount

    /// for (l_I = 0; l_I < EntriesCount; ++l_I)
        l_Data.appendPackGUID(p_Target->GetGUID());                 ///< Victim
        l_Data << uint8(p_MissInfo);                                ///< MissReason
        l_Data.WriteBit(false);                                     ///< HasDebug
        l_Data.FlushBits();

        /// if (HasDebug)
            ///data << float(0.0f);                                  ///< HitRoll
            ///data << float(0.0f);                                  ///< HitRollNeeded
        /// endif
    /// end loop

    SendMessageToSet(&l_Data, true);
}

void Unit::SendSpellDamageResist(Unit * p_Target, uint32 p_SpellID)
{
    WorldPacket l_Data(SMSG_PROCRESIST, 8+8+4+1);
    l_Data.appendPackGUID(GetGUID());
    l_Data.appendPackGUID(p_Target->GetGUID());
    l_Data << uint32(p_SpellID);
    l_Data.WriteBit(false);
    l_Data.WriteBit(false);
    l_Data.FlushBits();

    SendMessageToSet(&l_Data, true);
}

// Do not use this function
// We need to analyze UnfriendlyMessageDistDeliverer, because it's use delete pointer in somes cases
void Unit::SendMessageUnfriendlyToSetInRange(WorldPacket* data, float /*fist*/)
{
    JadeCore::UnfriendlyMessageDistDeliverer notifier(this, data, GetVisibilityRange());
    VisitNearbyWorldObject(GetVisibilityRange(), notifier);
}

void Unit::SendSpellDamageImmune(Unit* p_Target, uint32 p_SpellID)
{
    uint64 l_CasterGUID = GetGUID();
    uint64 l_TargetGUID = p_Target->GetGUID();
    bool l_IsPeriodic = sSpellMgr->GetSpellInfo(p_SpellID) ? sSpellMgr->GetSpellInfo(p_SpellID)->IsPeriodic() : false;

    WorldPacket l_Data(SMSG_SPELL_OR_DAMAGE_IMMUNE, (2 * (16 + 2)) + 4 + 1);

    l_Data.appendPackGUID(l_CasterGUID);
    l_Data.appendPackGUID(l_TargetGUID);
    l_Data << uint32(p_SpellID);
    l_Data.WriteBit(l_IsPeriodic);
    l_Data.FlushBits();

    SendMessageToSet(&l_Data, true);
}

void Unit::SendAttackStateUpdate(CalcDamageInfo* damageInfo)
{
    uint32 count = 1;
    int32 overkill = damageInfo->damage - damageInfo->target->GetHealth();

    ByteBuffer l_Buffer(1000);
    l_Buffer << uint32(damageInfo->HitInfo);
    l_Buffer.append(damageInfo->attacker->GetPackGUID());
    l_Buffer.append(damageInfo->target->GetPackGUID());
    l_Buffer << uint32(damageInfo->damage);                     // Full damage
    l_Buffer << uint32(overkill < 0 ? 0 : overkill);            // Overkill
    l_Buffer << uint8(count);                                   // Sub damage count

    for (uint32 i = 0; i < count; ++i)
    {
        l_Buffer << uint32(damageInfo->damageSchoolMask);       // School of sub damage
        l_Buffer << float(damageInfo->damage);                  // sub damage
        l_Buffer << uint32(damageInfo->damage);                 // Sub Damage
    }

    if (damageInfo->HitInfo & (HITINFO_FULL_ABSORB | HITINFO_PARTIAL_ABSORB))
    {
        for (uint32 i = 0; i < count; ++i)
            l_Buffer << uint32(damageInfo->absorb);             // Absorb
    }

    if (damageInfo->HitInfo & (HITINFO_FULL_RESIST | HITINFO_PARTIAL_RESIST))
    {
        for (uint32 i = 0; i < count; ++i)
            l_Buffer << uint32(damageInfo->resist);             // Resist
    }

    l_Buffer << uint8(damageInfo->TargetState);
    l_Buffer << uint32(0);  // Unknown attackerstate
    l_Buffer << uint32(0);  // Melee spellid

    if (damageInfo->HitInfo & HITINFO_BLOCK)
        l_Buffer << uint32(damageInfo->blocked_amount);

    if (damageInfo->HitInfo & HITINFO_RAGE_GAIN)
        l_Buffer << uint32(0);

    //! Probably used for debugging purposes, as it is not known to appear on retail servers
    if (damageInfo->HitInfo & HITINFO_UNK1)
    {
        l_Buffer << uint32(0);
        l_Buffer << float(0);
        l_Buffer << float(0);
        l_Buffer << float(0);
        l_Buffer << float(0);
        l_Buffer << float(0);
        l_Buffer << float(0);
        l_Buffer << float(0);
        l_Buffer << float(0);
        for (uint8 i = 0; i < 2; ++i)
        {
            l_Buffer << float(0);
            l_Buffer << float(0);
        }
        l_Buffer << uint32(0);
    }

    if (damageInfo->HitInfo & 0x3000)
        l_Buffer << float(0);


    WorldPacket data(SMSG_ATTACKER_STATE_UPDATE, l_Buffer.size() + 4 + 1);
    data.WriteBit(false);
    data.FlushBits();
    data << uint32(l_Buffer.size());
    data.append(l_Buffer);

    SendMessageToSet(&data, true);
}

void Unit::SendAttackStateUpdate(uint32 HitInfo, Unit* target, uint8 /*SwingType*/, SpellSchoolMask damageSchoolMask, uint32 Damage, uint32 AbsorbDamage, uint32 Resist, VictimState TargetState, uint32 BlockedAmount)
{
    CalcDamageInfo dmgInfo;
    dmgInfo.HitInfo = HitInfo;
    dmgInfo.attacker = this;
    dmgInfo.target = target;
    dmgInfo.damage = Damage - AbsorbDamage - Resist - BlockedAmount;
    dmgInfo.damageSchoolMask = damageSchoolMask;
    dmgInfo.absorb = AbsorbDamage;
    dmgInfo.resist = Resist;
    dmgInfo.TargetState = TargetState;
    dmgInfo.blocked_amount = BlockedAmount;
    SendAttackStateUpdate(&dmgInfo);
}

bool Unit::HandleHasteAuraProc(Unit* victim, uint32 /*damage*/, AuraEffect* triggeredByAura, SpellInfo const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown)
{
    SpellInfo const* hasteSpell = triggeredByAura->GetSpellInfo(); //< hasteSpell is never read 01/18/16

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    uint32 triggered_spell_id = 0;
    Unit* target = victim;
    int32 basepoints0 = 0;

    // processed charge only counting case
    if (!triggered_spell_id)
        return true;

    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(triggered_spell_id);

    if (!triggerEntry)
        return false;

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(triggered_spell_id))
        return false;

    if (basepoints0)
        CastCustomSpell(target, triggered_spell_id, &basepoints0, NULL, NULL, true, castItem, triggeredByAura);
    else
        CastSpell(target, triggered_spell_id, true, castItem, triggeredByAura);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(triggered_spell_id, 0, cooldown);

    return true;
}

bool Unit::HandleSpellCritChanceAuraProc(Unit* victim, uint32 /*damage*/, AuraEffect* triggeredByAura, SpellInfo const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown)
{
    SpellInfo const* triggeredByAuraSpell = triggeredByAura->GetSpellInfo(); ///> triggerByAuraSpell is never read 01/18/16

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    uint32 triggered_spell_id = 0;
    Unit* target = victim;
    int32 basepoints0 = 0;

    // processed charge only counting case
    if (!triggered_spell_id)
        return true;

    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(triggered_spell_id);

    if (!triggerEntry)
        return false;

    // default case
    if (!target || (target != this && !target->isAlive()))
        return false;

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(triggered_spell_id))
        return false;

    if (basepoints0)
        CastCustomSpell(target, triggered_spell_id, &basepoints0, NULL, NULL, true, castItem, triggeredByAura);
    else
        CastSpell(target, triggered_spell_id, true, castItem, triggeredByAura);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(triggered_spell_id, 0, cooldown);

    return true;
}

bool Unit::HandleAuraProcOnPowerAmount(Unit* victim, uint32 /*damage*/, AuraEffect* triggeredByAura, SpellInfo const* /*procSpell*/, uint32 procFlag, uint32 /*procEx*/, uint32 cooldown)
{
    // Get triggered aura spell info
    SpellInfo const* auraSpellInfo = triggeredByAura->GetSpellInfo();

    // Get effect index used for the proc
    uint32 effIndex = triggeredByAura->GetEffIndex(); ///< effIndex is never read 01/18/16

    // Power amount required to proc the spell
    int32 powerAmountRequired = triggeredByAura->GetAmount();
    // Power type required to proc
    Powers powerRequired = Powers(auraSpellInfo->Effects[triggeredByAura->GetEffIndex()].MiscValue);

    // Set trigger spell id, target, custom basepoints
    uint32 trigger_spell_id = auraSpellInfo->Effects[triggeredByAura->GetEffIndex()].TriggerSpell;

    Unit*  target = NULL;
    int32  basepoints0 = 0;

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    /* Try handle unknown trigger spells or with invalid power amount or misc value
    if (sSpellMgr->GetSpellInfo(trigger_spell_id) == NULL || powerAmountRequired == NULL || powerRequired >= MAX_POWER)
    {
        switch (auraSpellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
            {
                break;
            }
        }
    }*/

    // All ok. Check current trigger spell
    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(trigger_spell_id);
    if (triggerEntry == NULL)
    {
        // Not cast unknown spell
        // sLog->outError("Unit::HandleAuraProcOnPowerAmount: Spell %u have 0 in EffectTriggered[%d], not handled custom case?", auraSpellInfo->Id, triggeredByAura->GetEffIndex());
        return false;
    }

    // not allow proc extra attack spell at extra attack
    if (m_extraAttacks && triggerEntry->HasEffect(SPELL_EFFECT_ADD_EXTRA_ATTACKS))
        return false;

    if (!powerRequired || !powerAmountRequired)
        return false;

    if (GetPower(powerRequired) != powerAmountRequired)
        return false;

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(trigger_spell_id))
        return false;

    // try detect target manually if not set
    if (target == NULL)
        target = !(procFlag & (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS)) && triggerEntry && triggerEntry->IsPositive() ? this : victim;

    if (basepoints0)
        CastCustomSpell(target, trigger_spell_id, &basepoints0, NULL, NULL, true, castItem, triggeredByAura);
    else
        CastSpell(target, trigger_spell_id, true, castItem, triggeredByAura);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(trigger_spell_id, 0, cooldown);

    return true;
}

//victim may be NULL
bool Unit::HandleDummyAuraProc(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown)
{
    SpellInfo const* dummySpell = triggeredByAura->GetSpellInfo();
    uint32 effIndex = triggeredByAura->GetEffIndex();
    int32  triggerAmount = triggeredByAura->GetAmount();

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    uint32 triggered_spell_id = 0;
    uint32 cooldown_spell_id = 0; // for random trigger, will be one of the triggered spell to avoid repeatable triggers
                                  // otherwise, it's the triggered_spell_id by default
    Unit* target = victim;
    int32 basepoints0 = 0;
    uint64 originalCaster = 0;

    switch (dummySpell->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
        {
            switch (dummySpell->Id)
            {
                case 108007: // Indomitable, Indomitable Pride (normal)
                case 109785: // Indomitable, Indomitable Pride (lfr)
                case 109786: // Indomitable, Indomitable Pride (heroic)
                    if (!victim)
                        return false;
                    if (!damage)
                        return false;
                    if (effIndex != EFFECT_1)
                        return false;

                    if (!HealthBelowPctDamaged(50, damage))
                        return false;

                    if (Aura const* aur = triggeredByAura->GetBase())
                        if (AuraEffect const* aurEff = aur->GetEffect(EFFECT_1))
                            basepoints0 = int32(CalculatePct(damage, aurEff->GetAmount()));

                    triggered_spell_id = 108008;
                    break;
                case 104561:// Windsong
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (ToPlayer()->HasSpellCooldown(104561))
                        return false;

                    switch (urand(0, 2))
                    {
                        case 0: // Critical Strike
                            CastSpell(this, 104509, true);
                            break;
                        case 1: // Haste
                            CastSpell(this, 104510, true);
                            break;
                        case 2: // Mastery
                        default:
                            CastSpell(this, 104423, true);
                            break;
                    }

                    ToPlayer()->AddSpellCooldown(104561, 0, 60 * IN_MILLISECONDS);
                    return false;
                }
                case 120033:// Jade Spirit
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (ToPlayer()->HasSpellCooldown(104993))
                        return false;

                    basepoints0 = 1650;
                    int32 basepoints1 = 0;

                    if (GetPowerPct(POWER_MANA) >= 25.0f)
                        CastCustomSpell(this, 104993, &basepoints0, &basepoints1, NULL, true);
                    else
                        CastSpell(this, 104993, true);

                    ToPlayer()->AddSpellCooldown(104993, 0, 60 * IN_MILLISECONDS);
                    return false;
                }
                case 118333:// Dancing Steel
                case 142531:// Bloody Dancing Steel
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (procFlag & PROC_FLAG_DONE_MAINHAND_ATTACK)
                    {
                        if (Item* mainItem = ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                        {
                            if (GetStat(STAT_AGILITY) > GetStat(STAT_STRENGTH))
                                CastSpell(this, 118334, true, mainItem);
                            else
                                CastSpell(this, 118335, true, mainItem);
                        }
                    }
                    else if (procFlag & PROC_FLAG_DONE_OFFHAND_ATTACK)
                    {
                        if (Item* offItem = ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                        {
                            if (GetStat(STAT_AGILITY) > GetStat(STAT_STRENGTH))
                                CastSpell(this, 118334, true, offItem);
                            else
                                CastSpell(this, 118335, true, offItem);
                        }
                    }

                    break;
                }
                case 142536:// Spirit of Conquest
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (ToPlayer()->HasSpellCooldown(142535))
                        return false;

                    triggered_spell_id = 142535;
                    CastSpell(this, triggered_spell_id, true);
                    ToPlayer()->AddSpellCooldown(142535, 0, 60 * IN_MILLISECONDS);
                    return false;
                }
                // Weight of Feather, Scales of Life
                case 96879:
                case 97117:
                {
                    if (!victim)
                        return false;

                    int32 max_amount = triggeredByAura->GetAmount();
                    int32 add_heal = damage - (victim->GetMaxHealth() - victim->GetHealth());
                    if (add_heal <= 0)
                        return false;

                    int32 old_amount = 0;

                    if (AuraEffect* aurEff = victim->GetAuraEffect(96881, EFFECT_0))
                        old_amount = aurEff->GetAmount();

                    int32 new_amount = old_amount + add_heal;

                    int32 bp0 = std::min(new_amount, max_amount);
                    CastCustomSpell(victim, 96881, &bp0, 0, 0, true);
                    return true;
                }
                // Wrath of Tarecgosa
                case 101056:
                {
                    if (!procSpell)
                        return false;

                    if (!victim)
                        return false;

                    if (GetGUID() == victim->GetGUID())
                        return false;

                    if (procFlag & PROC_FLAG_DONE_PERIODIC)
                    {
                        if (!roll_chance_i(10))
                            return false;

                        basepoints0 = damage;
                        triggered_spell_id = 101085;
                        break;
                    }
                    else
                    {
                        if (!roll_chance_i(5))
                            return false;

                        triggered_spell_id = procSpell->Id;

                        // Fulmination              Improved Devouring Plague
                        if (procSpell->Id == 88767 || procSpell->Id == 63675)
                            basepoints0 = damage;

                        break;
                    }
                    break;
                }
                // Eye for an Eye
                case 25988:
                {
                    // return damage % to attacker but < 50% own total health
                    basepoints0 = int32(std::min(CalculatePct(damage, triggerAmount), CountPctFromMaxHealth(50)));
                    triggered_spell_id = 25997;
                    break;
                }
                // Sweeping Strikes
                case 18765:
                case 35429:
                {
                    target = SelectNearbyTarget(victim, NOMINAL_MELEE_RANGE, 0U, true, true, false, true);
                    if (!target)
                        return false;

                    triggered_spell_id = 26654;
                    break;
                }
                // Unstable Power
                case 24658:
                {
                    if (!procSpell || procSpell->Id == 24659)
                        return false;
                    // Need remove one 24659 aura
                    RemoveAuraFromStack(24659);
                    return true;
                }
                // Restless Strength
                case 24661:
                {
                    // Need remove one 24662 aura
                    RemoveAuraFromStack(24662);
                    return true;
                }
                // Adaptive Warding (Frostfire Regalia set)
                case 28764:
                {
                    if (!procSpell)
                        return false;

                    // find Mage Armor
                    if (!GetAuraEffect(SPELL_AURA_MOD_MANA_REGEN_INTERRUPT, SPELLFAMILY_MAGE, 0x10000000, 0, 0))
                        return false;

                    switch (GetFirstSchoolInMask(procSpell->GetSchoolMask()))
                    {
                        case SPELL_SCHOOL_NORMAL:
                        case SPELL_SCHOOL_HOLY:
                            return false;                   // ignored
                        case SPELL_SCHOOL_FIRE:   triggered_spell_id = 28765; break;
                        case SPELL_SCHOOL_NATURE: triggered_spell_id = 28768; break;
                        case SPELL_SCHOOL_FROST:  triggered_spell_id = 28766; break;
                        case SPELL_SCHOOL_SHADOW: triggered_spell_id = 28769; break;
                        case SPELL_SCHOOL_ARCANE: triggered_spell_id = 28770; break;
                        default:
                            return false;
                    }

                    target = this;
                    break;
                }
                // Obsidian Armor (Justice Bearer`s Pauldrons shoulder)
                case 27539:
                {
                    if (!procSpell)
                        return false;

                    switch (GetFirstSchoolInMask(procSpell->GetSchoolMask()))
                    {
                        case SPELL_SCHOOL_NORMAL:
                            return false;                   // ignore
                        case SPELL_SCHOOL_HOLY:   triggered_spell_id = 27536; break;
                        case SPELL_SCHOOL_FIRE:   triggered_spell_id = 27533; break;
                        case SPELL_SCHOOL_NATURE: triggered_spell_id = 27538; break;
                        case SPELL_SCHOOL_FROST:  triggered_spell_id = 27534; break;
                        case SPELL_SCHOOL_SHADOW: triggered_spell_id = 27535; break;
                        case SPELL_SCHOOL_ARCANE: triggered_spell_id = 27540; break;
                        default:
                            return false;
                    }

                    target = this;
                    break;
                }
                // Mana Leech (Passive) (Priest Pet Aura)
                case 28305:
                {
                    // Cast on owner
                    target = GetOwner();
                    if (!target)
                        return false;

                    if (GetEntry() == 62982 || GetEntry() == 67236) // Mindbender
                    {
                        target->EnergizeBySpell(target, 123051, int32(0.0075f * target->GetPower(POWER_MANA)), POWER_MANA);
                        return false;
                    }

                    triggered_spell_id = 34650;

                    // Item - Priest T13 Shadow 4P Bonus (Shadowfiend and Shadowy Apparition)
                    if (AuraEffect* eff = target->GetAuraEffect(105844, 0))
                    {
                        if (roll_chance_i(eff->GetAmount()))
                        {
                            // don't apply if allready have orbs
                            bool found = false;
                            if (Aura* orbs = target->GetAura(77487))
                                if (orbs->GetStackAmount() == 3)
                                    found = true;

                            if (!found)
                            {
                                for (int i = 0; i < eff->GetSpellInfo()->Effects[1].BasePoints; i++)
                                {
                                    target->CastSpell(target, 77487, true);
                                }
                            }
                        }
                    }
                    break;
                }
                // Mark of Malice
                case 33493:
                {
                    // Cast finish spell at last charge
                    if (triggeredByAura->GetBase()->GetCharges() > 1)
                        return false;

                    target = this;
                    triggered_spell_id = 33494;
                    break;
                }
                // Twisted Reflection (boss spell)
                case 21063:
                    triggered_spell_id = 21064;
                    break;
                // Vampiric Aura (boss spell)
                case 38196:
                {
                    basepoints0 = 3 * damage;               // 300%
                    if (basepoints0 < 0)
                        return false;

                    triggered_spell_id = 31285;
                    target = this;
                    break;
                }
                // Aura of Madness (Darkmoon Card: Madness trinket)
                //=====================================================
                // 39511 Sociopath: +35 strength (Paladin, Rogue, Druid, Warrior)
                // 40997 Delusional: +70 attack power (Rogue, Hunter, Paladin, Warrior, Druid)
                // 40998 Kleptomania: +35 agility (Warrior, Rogue, Paladin, Hunter, Druid)
                // 40999 Megalomania: +41 damage/healing (Druid, Shaman, Priest, Warlock, Mage, Paladin)
                // 41002 Paranoia: +35 spell/melee/ranged crit strike rating (All classes)
                // 41005 Manic: +35 haste (spell, melee and ranged) (All classes)
                // 41009 Narcissism: +35 intellect (Druid, Shaman, Priest, Warlock, Mage, Paladin, Hunter)
                // 41011 Martyr Complex: +35 stamina (All classes)
                // 41406 Dementia: Every 5 seconds either gives you +5% damage/healing. (Druid, Shaman, Priest, Warlock, Mage, Paladin)
                // 41409 Dementia: Every 5 seconds either gives you -5% damage/healing. (Druid, Shaman, Priest, Warlock, Mage, Paladin)
                case 39446:
                {
                    if (GetTypeId() != TYPEID_PLAYER || !isAlive())
                        return false;

                    // Select class defined buff
                    switch (getClass())
                    {
                        case CLASS_PALADIN:                 // 39511, 40997, 40998, 40999, 41002, 41005, 41009, 41011, 41409
                        case CLASS_DRUID:                   // 39511, 40997, 40998, 40999, 41002, 41005, 41009, 41011, 41409
                            triggered_spell_id = RAND(39511, 40997, 40998, 40999, 41002, 41005, 41009, 41011, 41409);
                            cooldown_spell_id = 39511;
                            break;
                        case CLASS_ROGUE:                   // 39511, 40997, 40998, 41002, 41005, 41011
                        case CLASS_WARRIOR:                 // 39511, 40997, 40998, 41002, 41005, 41011
                        case CLASS_DEATH_KNIGHT:
                            triggered_spell_id = RAND(39511, 40997, 40998, 41002, 41005, 41011);
                            cooldown_spell_id = 39511;
                            break;
                        case CLASS_PRIEST:                  // 40999, 41002, 41005, 41009, 41011, 41406, 41409
                        case CLASS_SHAMAN:                  // 40999, 41002, 41005, 41009, 41011, 41406, 41409
                        case CLASS_MAGE:                    // 40999, 41002, 41005, 41009, 41011, 41406, 41409
                        case CLASS_WARLOCK:                 // 40999, 41002, 41005, 41009, 41011, 41406, 41409
                            triggered_spell_id = RAND(40999, 41002, 41005, 41009, 41011, 41406, 41409);
                            cooldown_spell_id = 40999;
                            break;
                        case CLASS_HUNTER:                  // 40997, 40999, 41002, 41005, 41009, 41011, 41406, 41409
                            triggered_spell_id = RAND(40997, 40999, 41002, 41005, 41009, 41011, 41406, 41409);
                            cooldown_spell_id = 40997;
                            break;
                        default:
                            return false;
                    }

                    target = this;
                    if (roll_chance_i(10))
                        ToPlayer()->Say("This is Madness!", LANG_UNIVERSAL); // TODO: It should be moved to database, shouldn't it?
                    break;
                }
                // Sunwell Exalted Caster Neck (??? neck)
                // cast ??? Light's Wrath if Exalted by Aldor
                // cast ??? Arcane Bolt if Exalted by Scryers
                case 46569:
                    return false;                           // old unused version
                // Sunwell Exalted Caster Neck (Shattered Sun Pendant of Acumen neck)
                // cast 45479 Light's Wrath if Exalted by Aldor
                // cast 45429 Arcane Bolt if Exalted by Scryers
                case 45481:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // Get Aldor reputation rank
                    if (ToPlayer()->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45479;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (ToPlayer()->GetReputationRank(934) == REP_EXALTED)
                    {
                        // triggered at positive/self casts also, current attack target used then
                        if (target && IsFriendlyTo(target))
                        {
                            target = getVictim();
                            if (!target)
                            {
                                uint64 selected_guid = ToPlayer()->GetSelection();
                                target = ObjectAccessor::GetUnit(*this, selected_guid);
                                if (!target)
                                    return false;
                            }
                            if (IsFriendlyTo(target))
                                return false;
                        }

                        triggered_spell_id = 45429;
                        break;
                    }
                    return false;
                }
                // Sunwell Exalted Melee Neck (Shattered Sun Pendant of Might neck)
                // cast 45480 Light's Strength if Exalted by Aldor
                // cast 45428 Arcane Strike if Exalted by Scryers
                case 45482:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // Get Aldor reputation rank
                    if (ToPlayer()->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45480;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (ToPlayer()->GetReputationRank(934) == REP_EXALTED)
                    {
                        triggered_spell_id = 45428;
                        break;
                    }
                    return false;
                }
                // Sunwell Exalted Tank Neck (Shattered Sun Pendant of Resolve neck)
                // cast 45431 Arcane Insight if Exalted by Aldor
                // cast 45432 Light's Ward if Exalted by Scryers
                case 45483:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // Get Aldor reputation rank
                    if (ToPlayer()->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45432;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (ToPlayer()->GetReputationRank(934) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45431;
                        break;
                    }
                    return false;
                }
                // Sunwell Exalted Healer Neck (Shattered Sun Pendant of Restoration neck)
                // cast 45478 Light's Salvation if Exalted by Aldor
                // cast 45430 Arcane Surge if Exalted by Scryers
                case 45484:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // Get Aldor reputation rank
                    if (ToPlayer()->GetReputationRank(932) == REP_EXALTED)
                    {
                        target = this;
                        triggered_spell_id = 45478;
                        break;
                    }
                    // Get Scryers reputation rank
                    if (ToPlayer()->GetReputationRank(934) == REP_EXALTED)
                    {
                        triggered_spell_id = 45430;
                        break;
                    }
                    return false;
                }
                // Living Seed
                case 48504:
                {
                    triggered_spell_id = 48503;
                    basepoints0 = triggerAmount;
                    target = this;
                    break;
                }
                // Glyph of Scourge Strike
                case 58642:
                {
                    triggered_spell_id = 69961; // Glyph of Scourge Strike
                    break;
                }
                // Purified Shard of the Scale - Onyxia 10 Caster Trinket
                case 69755:
                {
                    triggered_spell_id = (procFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS) ? 69733 : 69729;
                    break;
                }
                // Shiny Shard of the Scale - Onyxia 25 Caster Trinket
                case 69739:
                {
                    triggered_spell_id = (procFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS) ? 69734 : 69730;
                    break;
                }
                case 71519: // Deathbringer's Will Normal
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (ToPlayer()->HasSpellCooldown(dummySpell->Id))
                        return false;

                    std::vector<uint32> RandomSpells;
                    switch (getClass())
                    {
                        case CLASS_WARRIOR:
                        case CLASS_PALADIN:
                        case CLASS_DEATH_KNIGHT:
                            RandomSpells.push_back(71484);
                            RandomSpells.push_back(71491);
                            RandomSpells.push_back(71492);
                            break;
                        case CLASS_SHAMAN:
                        case CLASS_ROGUE:
                            RandomSpells.push_back(71486);
                            RandomSpells.push_back(71485);
                            RandomSpells.push_back(71492);
                            break;
                        case CLASS_DRUID:
                            RandomSpells.push_back(71484);
                            RandomSpells.push_back(71485);
                            RandomSpells.push_back(71492);
                            break;
                        case CLASS_HUNTER:
                            RandomSpells.push_back(71486);
                            RandomSpells.push_back(71491);
                            RandomSpells.push_back(71485);
                            break;
                        default:
                            return false;
                    }
                    if (RandomSpells.empty()) // shouldn't happen
                        return false;

                    triggered_spell_id = RandomSpells[irand(0, (RandomSpells.size() - 1))];
                    ToPlayer()->AddSpellCooldown(dummySpell->Id, 0, cooldown);
                    break;
                }
                case 71562: // Deathbringer's Will Heroic
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (ToPlayer()->HasSpellCooldown(dummySpell->Id))
                        return false;

                    std::vector<uint32> RandomSpells;
                    switch (getClass())
                    {
                        case CLASS_WARRIOR:
                        case CLASS_PALADIN:
                        case CLASS_DEATH_KNIGHT:
                            RandomSpells.push_back(71561);
                            RandomSpells.push_back(71559);
                            RandomSpells.push_back(71560);
                            break;
                        case CLASS_SHAMAN:
                        case CLASS_ROGUE:
                            RandomSpells.push_back(71558);
                            RandomSpells.push_back(71556);
                            RandomSpells.push_back(71560);
                            break;
                        case CLASS_DRUID:
                            RandomSpells.push_back(71561);
                            RandomSpells.push_back(71556);
                            RandomSpells.push_back(71560);
                            break;
                        case CLASS_HUNTER:
                            RandomSpells.push_back(71558);
                            RandomSpells.push_back(71559);
                            RandomSpells.push_back(71556);
                            break;
                        default:
                            return false;
                    }
                    if (RandomSpells.empty()) // shouldn't happen
                        return false;

                    triggered_spell_id = RandomSpells[irand(0, (RandomSpells.size() - 1))];
                    ToPlayer()->AddSpellCooldown(dummySpell->Id, 0, cooldown);
                    break;
                }
                case 71875: // Item - Black Bruise: Necrotic Touch Proc
                case 71877:
                {
                    basepoints0 = CalculatePct(int32(damage), triggerAmount);
                    triggered_spell_id = 71879;
                    break;
                }
                // Item - Shadowmourne Legendary
                case 71903:
                {
                    if (!victim || !victim->isAlive() || HasAura(73422))  // cant collect shards while under effect of Chaos Bane buff
                        return false;

                    CastSpell(this, 71905, true, NULL, triggeredByAura);

                    // this can't be handled in AuraScript because we need to know victim
                    Aura const* dummy = GetAura(71905);
                    if (!dummy || dummy->GetStackAmount() < 10)
                        return false;

                    RemoveAurasDueToSpell(71905);
                    triggered_spell_id = 71904;
                    target = victim;
                    break;
                }
                // Shadow's Fate (Shadowmourne questline)
                case 71169:
                {
                    target = triggeredByAura->GetCaster();
                    if (!target)
                        return false;
                    Player* player = target->ToPlayer();
                    if (!player)
                        return false;
                    // not checking Infusion auras because its in targetAuraSpell of credit spell
                    if (player->GetQuestStatus(24749) == QUEST_STATUS_INCOMPLETE)       // Unholy Infusion
                    {
                        if (GetEntry() != 36678)                                        // Professor Putricide
                            return false;
                        CastSpell(target, 71518, true);                                 // Quest Credit
                        return true;
                    }
                    else if (player->GetQuestStatus(24756) == QUEST_STATUS_INCOMPLETE)  // Blood Infusion
                    {
                        if (GetEntry() != 37955)                                        // Blood-Queen Lana'thel
                            return false;
                        CastSpell(target, 72934, true);                                 // Quest Credit
                        return true;
                    }
                    else if (player->GetQuestStatus(24757) == QUEST_STATUS_INCOMPLETE)  // Frost Infusion
                    {
                        if (GetEntry() != 36853)                                        // Sindragosa
                            return false;
                        CastSpell(target, 72289, true);                                 // Quest Credit
                        return true;
                    }
                    else if (player->GetQuestStatus(24547) == QUEST_STATUS_INCOMPLETE)  // A Feast of Souls
                        triggered_spell_id = 71203;
                    break;
                }
                // Essence of the Blood Queen
                case 70871:
                {
                    basepoints0 = CalculatePct(int32(damage), triggerAmount);
                    CastCustomSpell(70872, SPELLVALUE_BASE_POINT0, basepoints0, this);
                    return true;
                }
                case 65032: // Boom aura (321 Boombot)
                {
                    if (victim->GetEntry() != 33343)   // Scrapbot
                        return false;

                    InstanceScript* instance = GetInstanceScript();
                    if (!instance)
                        return false;

                    instance->DoCastSpellOnPlayers(65037);  // Achievement criteria marker
                    break;
                }
                // Dark Hunger (The Lich King encounter)
                case 69383:
                {
                    basepoints0 = CalculatePct(int32(damage), 50);
                    triggered_spell_id = 69384;
                    break;
                }
                case 47020: // Enter vehicle XT-002 (Scrapbot)
                {
                    if (GetTypeId() != TYPEID_UNIT)
                        return false;

                    Unit* vehicleBase = GetVehicleBase();
                    if (!vehicleBase)
                        return false;

                    // Todo: Check if this amount is blizzlike
                    vehicleBase->ModifyHealth(int32(vehicleBase->CountPctFromMaxHealth(1)));
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_MAGE:
        {
            switch (dummySpell->Id)
            {
                case 37424: // Incanter's Regalia set (add trigger chance to Mana Shield)
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    target = this;
                    triggered_spell_id = 37436;
                    break;
                }
                case 44448: ///< Pyroblast Clearcasting Driver
                {
                    if (!procSpell || !procSpell->CanTriggerHotStreak())
                        return false;

                    if (!damage && !(procEx & PROC_EX_ABSORB))
                        return false;

                    if (procEx & (PROC_EX_INTERNAL_DOT | PROC_EX_INTERNAL_MULTISTRIKE))
                        return false;

                    /// Prevent proc on victim fire mage
                    if (procFlag & PROC_FLAG_TAKEN_DAMAGE)
                        return false;

                    if (procEx & PROC_EX_CRITICAL_HIT)
                    {
                        if (!HasAura(48107))
                        {
                            CastSpell(this, 48107, true, castItem); ///< Heating Up
                            return true;
                        }
                        else
                        {
                            RemoveAura(48107);
                            triggered_spell_id = 48108; ///< Pyroblast!
                            target = this;
                            break;
                        }
                    }
                    else
                    {
                        if (Aura* l_HeatingUp = GetAura(48107, GetGUID()))
                            l_HeatingUp->SetDuration(500);
                        return false;
                    }
                }
                case 64411: // Blessing of Ancient Kings (Val'anyr, Hammer of Ancient Kings)
                {
                    if (!victim)
                        return false;
                    basepoints0 = int32(CalculatePct(damage, 15));
                    if (AuraEffect* aurEff = victim->GetAuraEffect(64413, 0, GetGUID()))
                    {
                        // The shield can grow to a maximum size of 20, 000 damage absorbtion
                        aurEff->SetAmount(std::min<int32>(aurEff->GetAmount() + basepoints0, 20000));

                        // Refresh and return to prevent replacing the aura
                        aurEff->GetBase()->RefreshDuration();
                        return true;
                    }
                    target = victim;
                    triggered_spell_id = 64413;
                    break;
                }
                default:
                    break;
            }
        }
        case SPELLFAMILY_WARRIOR:
        {
            switch (dummySpell->Id)
            {
                // Item - Warrior T13 Protection 2P Bonus (Revenge)
                case 105908:
                    if (!victim)
                        return false;

                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (victim != ToPlayer()->GetSelectedUnit())
                        return false;

                    basepoints0 = int32(CalculatePct(damage, 20));
                    triggered_spell_id = 105909;

                    if (AuraEffect const* aurEff = GetAuraEffect(triggered_spell_id, EFFECT_0))
                        basepoints0 += aurEff->GetAmount();

                    target = this;

                    break;
                // Item - Warrior T12 Protection 2P Bonus
                case 99239:
                {
                    basepoints0 = int32(CalculatePct(damage, triggerAmount / 2)); // 2 ticks
                    triggered_spell_id = 99240;
                    target = victim;
                    break;
                }
                // Victorious
                case 32216:
                {
                    RemoveAura(dummySpell->Id);
                    return false;
                }
            }
            // Retaliation
            if (dummySpell->SpellFamilyFlags[1] & 0x8)
            {
                // check attack comes not from behind
                if (!HasInArc(M_PI, victim))
                    return false;

                triggered_spell_id = 22858;
                break;
            }
            break;
        }
        case SPELLFAMILY_WARLOCK:
        {
            switch (dummySpell->Id)
            {
                case 27243: // Seed of Corruption
                {
                    if (procSpell && procSpell->Id == 27285)
                        return false;

                    // if damage is more than need or target die from damage deal finish spell
                    if (triggeredByAura->GetAmount() <= int32(damage) || GetHealth() <= damage)
                    {
                        // remember guid before aura delete
                        uint64 casterGuid = triggeredByAura->GetCasterGUID();

                        // Remove aura (before cast for prevent infinite loop handlers)
                        RemoveAurasDueToSpell(triggeredByAura->GetId());

                        // Cast finish spell (triggeredByAura already not exist!)
                        if (Unit* caster = GetUnit(*this, casterGuid))
                            caster->CastSpell(this, 27285, true, castItem);
                        return true;
                    }

                    // Damage counting
                    triggeredByAura->SetAmount(triggeredByAura->GetAmount() - damage);
                    return true;
                }
                case 114790:// Soulburn : Seed of Corruption
                {
                    if (procSpell && procSpell->Id == 87385)
                        return false;

                    // if damage is more than need or target die from damage deal finish spell
                    if (triggeredByAura->GetAmount() <= int32(damage) || GetHealth() <= damage)
                    {
                        // remember guid before aura delete
                        uint64 casterGuid = triggeredByAura->GetCasterGUID();

                        // Remove aura (before cast for prevent infinite loop handlers)
                        RemoveAurasDueToSpell(triggeredByAura->GetId());

                        // Cast finish spell (triggeredByAura already not exist!)
                        if (Unit* caster = GetUnit(*this, casterGuid))
                            caster->CastSpell(this, 87385, true, castItem);
                        return true;
                    }

                    // Damage counting
                    triggeredByAura->SetAmount(triggeredByAura->GetAmount() - damage);
                    return true;
                }
                case 32863: // Seed of Corruption (no die requirements)
                {
                    // if damage is more than need deal finish spell
                    if (triggeredByAura->GetAmount() <= int32(damage))
                    {
                        // remember guid before aura delete
                        uint64 casterGuid = triggeredByAura->GetCasterGUID();

                        // Remove aura (before cast for prevent infinite loop handlers)
                        RemoveAurasDueToSpell(triggeredByAura->GetId());

                        // Cast finish spell (triggeredByAura already not exist!)
                        if (Unit* caster = GetUnit(*this, casterGuid))
                            caster->CastSpell(this, 32865, true, castItem);
                        return true;                            // no hidden cooldown
                    }
                    // Damage counting
                    triggeredByAura->SetAmount(triggeredByAura->GetAmount() - damage);
                    return true;
                }
                case 37377: // Shadowflame (Voidheart Raiment set bonus)
                {
                    triggered_spell_id = 37379;
                    break;
                }
                case 37381: // Pet Healing (Corruptor Raiment or Rift Stalker Armor)
                {
                    target = GetGuardianPet();
                    if (!target)
                        return false;

                    // heal amount
                    basepoints0 = CalculatePct(int32(damage), triggerAmount);
                    triggered_spell_id = 37382;
                    break;
                }
                case 39437: // Shadowflame Hellfire (Voidheart Raiment set bonus)
                {
                    triggered_spell_id = 37378;
                    break;
                }
                default:
                    break;
            }

            break;
        }
        case SPELLFAMILY_PRIEST:
        {
            // Vampiric Touch
            if (dummySpell->SpellFamilyFlags[1] & 0x00000400)
            {
                if (!victim || !victim->isAlive())
                    return false;

                if (effIndex != 0)
                    return false;

                // victim is caster of aura
                if (triggeredByAura->GetCasterGUID() != victim->GetGUID())
                    return false;

                // Energize 1% of max. mana
                victim->CastSpell(victim, 57669, true, castItem, triggeredByAura);
                return true;                                // no hidden cooldown
            }

            switch (dummySpell->Id)
            {
                // Shadowflame, Item - Priest T12 Shadow 2P Bonus
                case 99155:
                {
                    if (!GetOwner())
                        return false;

                    if (!(GetOwner()->GetShapeshiftForm() == FORM_SHADOW) || !GetOwner()->HasAura(99154))
                        return false;

                    target = victim;
                    basepoints0 = int32(CalculatePct(damage, 20));
                    triggered_spell_id = 99156;
                    break;
                }
                // Vampiric Embrace
                case 15286:
                {
                    if (!victim || !victim->isAlive() || !procSpell || !damage)
                        return false;

                    // heal amount
                    int32 bp = CalculatePct(int32(damage), triggerAmount);
                    CastCustomSpell(this, 15290, &bp, NULL, NULL, true, castItem, triggeredByAura);
                    return true;                                // no hidden cooldown
                }
                // Priest Tier 6 Trinket (Ashtongue Talisman of Acumen)
                case 40438:
                {
                    // Shadow Word: Pain
                    if (procSpell->SpellFamilyFlags[0] & 0x8000)
                        triggered_spell_id = 40441;
                    // Renew
                    else if (procSpell->SpellFamilyFlags[0] & 0x40)
                        triggered_spell_id = 40440;
                    else
                        return false;

                    target = this;
                    break;
                }
                // Oracle Healing Bonus ("Garments of the Oracle" set)
                case 26169:
                {
                    // heal amount
                    basepoints0 = int32(CalculatePct(damage, 10));
                    target = this;
                    triggered_spell_id = 26170;
                    break;
                }
                // Frozen Shadoweave (Shadow's Embrace set) warning! its not only priest set
                case 39372:
                {
                    if (!procSpell || (procSpell->GetSchoolMask() & (SPELL_SCHOOL_MASK_FROST | SPELL_SCHOOL_MASK_SHADOW)) == 0)
                        return false;

                    // heal amount
                    basepoints0 = CalculatePct(int32(damage), triggerAmount);
                    target = this;
                    triggered_spell_id = 39373;
                    break;
                }
                // Greater Heal (Vestments of Faith (Priest Tier 3) - 4 pieces bonus)
                case 28809:
                {
                    triggered_spell_id = 28810;
                    break;
                }
                // Priest T10 Healer 2P Bonus
                case 70770:
                    // Flash Heal
                    if (procSpell->SpellFamilyFlags[0] & 0x800)
                    {
                        triggered_spell_id = 70772;
                        SpellInfo const* blessHealing = sSpellMgr->GetSpellInfo(triggered_spell_id);
                        if (!blessHealing)
                            return false;
                        basepoints0 = int32(CalculatePct(damage, triggerAmount) / (blessHealing->GetMaxDuration() / blessHealing->Effects[0].Amplitude));
                    }
                    break;
                // Shadowy Apparition
                case 78203:
                    if (Aura* aur = GetAura(dummySpell->Id))
                    {
                        int32 chance = aur->GetEffect(0)->GetAmount();
                        if (IsMoving())
                            chance *= 5;
                        if (effIndex !=0 || !procSpell || !roll_chance_i(chance))
                            return false;

                        std::list<Creature*> summons;
                        GetAllMinionsByEntry(summons, 46954);
                        if (summons.size() > 3)
                            return false;

                        int32 bp0 = 1;
                        CastCustomSpell(this, 87426, &bp0, NULL, NULL, true);

                        std::list<Creature*> new_summons;
                        GetAllMinionsByEntry(new_summons, 46954);

                        Unit* summon = NULL;
                        for (std::list<Creature*>::iterator new_itr = new_summons.begin(); new_itr != new_summons.end(); ++new_itr)
                        {
                            summon = NULL;
                            for (std::list<Creature*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
                                if ((*new_itr)->GetGUID() == (*itr)->GetGUID())
                                    summon = *new_itr;
                            if (!summon)
                            {
                                summon = *new_itr;
                                break;
                            }
                        }
                        if (summon)
                        {
                            //summon->m_FollowingRefManager.clearReferences();
                            CastSpell(summon, 87213, true);
                            summon->CastSpell(summon, 87427, true);
                            summon->GetAI()->AttackStart(victim);
                        }
                    }
                    break;
            }
            break;
        }
        case SPELLFAMILY_DRUID:
        {
            switch (dummySpell->Id)
            {
                // Item - Shaman T12 Enhancement 4P Bonus
                case 99213:
                    triggered_spell_id = 99212;
                    target = victim;
                    break;
                // Item - Rogue T12 2P Bonus
                case 99174:
                {
                    triggerAmount = 3;
                    basepoints0 = CalculatePct(damage, triggerAmount);
                    triggered_spell_id = 99173;
                    basepoints0 += victim->GetRemainingPeriodicAmount(GetGUID(), triggered_spell_id, SPELL_AURA_PERIODIC_DAMAGE);
                    break;
                }
                // Item  Druid T12 Restoration 4P Bonus
                case 99015:
                {
                    if (!victim || !victim->ToPlayer())
                        return false;

                    Player* plr = victim->ToPlayer();
                    if (!plr)
                        return false;

                    std::list<Player*> plrList;
                    JadeCore::AnyFriendlyUnitInObjectRangeCheck check(this, this, 15.0f);
                    JadeCore::PlayerListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, plrList, check);
                    VisitNearbyObject(15.0f, searcher);
                    if (plrList.empty())
                        return false;

                    plrList.remove(plr);

                    if (plrList.empty())
                        return false;

                    plrList.sort(JadeCore::HealthPctOrderPred());
                    plrList.resize(1);

                    int32 bp0 = damage;

                    CastCustomSpell(plrList.front(), 99017, &bp0, 0, 0, true);
                    return true;
                }
                // Item - Druid T12 Feral 4P Bonus
                case 99009:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    int32 l_Chance = 20 * GetPower(Powers::POWER_COMBO_POINT);
                    if (!roll_chance_i(l_Chance))
                        return false;

                    if (Aura* l_Aura = GetAura(50334))
                        l_Aura->SetDuration(l_Aura->GetDuration() + 2000);
                    break;
                }
                // Item  Druid T12 Feral 2P Bonus
                case 99001:
                    triggerAmount /= 2;
                    basepoints0 = int32(CalculatePct(damage, triggerAmount));
                    triggered_spell_id = 99002;
                    basepoints0 += victim->GetRemainingPeriodicAmount(GetGUID(), triggered_spell_id, SPELL_AURA_PERIODIC_DAMAGE);
                    target = victim;
                    break;
                case 54825: // Glyph of Healing Touch
                {
                    if (!procSpell)
                        return false;

                    if (procSpell->Id != 5185)
                        return false;

                    /// Cooldown of Swiftmen reduced by 2s
                    ToPlayer()->ReduceSpellCooldown(132158, 2000); ///< Last update 6.0.2 (Tue Oct 14 2014) Build 18837
                    break;
                }
                case 108373:// Dream of Cenarius
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (!procSpell)
                        return false;

                    // Wrath, Starfire, Starsurge, and melee abilities increase healing done by your next healing spell by 30%. Tranquility is not affected.
                    else if ((procFlag & PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS) || (procSpell->Id == 5176 || procSpell->Id == 2912 || procSpell->Id == 78674))
                    {
                        triggered_spell_id = 108382;
                        target = this;
                    }
                    else
                        return false;

                    break;
                }
                case 46832: // Sudden Eclipse (S12 - 2P Balance)
                /// @todo Changed in Build 18711 6.0.1
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (!(procEx & PROC_EX_CRITICAL_HIT))
                        return false;

                    // Solar and Lunar Eclipse
                    if (HasAura(48517) || HasAura(48518))
                        return false;

                    if (ToPlayer()->HasSpellCooldown(46832))
                        return false;

                    ToPlayer()->AddSpellCooldown(46832, 0, 6 * IN_MILLISECONDS);

                    ModifyPower(Powers::POWER_ECLIPSE, 20);

                    if (GetPower(Powers::POWER_ECLIPSE) == 100)
                    {
                        CastSpell(this, 48517, true, 0); // Cast Lunar Eclipse
                        CastSpell(this, 16886, true); // Cast Nature's Grace
                        CastSpell(this, 81070, true); // Cast Eclipse - Give 35% of POWER_MANA
                    }
                    else if (GetPower(Powers::POWER_ECLIPSE) == 0)
                    {
                        CastSpell(this, 48518, true, 0); // Cast Lunar Eclipse
                        CastSpell(this, 16886, true); // Cast Nature's Grace
                        CastSpell(this, 81070, true); // Cast Eclipse - Give 35% of POWER_MANA
                        CastSpell(this, 107095, true);

                        if (ToPlayer()->HasSpellCooldown(48505))
                            ToPlayer()->RemoveSpellCooldown(48505, true);
                    }

                    break;
                }
                case 28719: // Healing Touch (Dreamwalker Raiment set)
                {
                    // mana back
                    basepoints0 = int32(CalculatePct(procSpell->ManaCost, 30));
                    target = this;
                    triggered_spell_id = 28742;
                    break;
                }
                case 37288: // Mana Restore (Malorne Raiment set / Malorne Regalia set)
                case 37295:
                {
                    target = this;
                    triggered_spell_id = 37238;
                    break;
                }
                case 40442: // Druid Tier 6 Trinket
                {
                    float  chance;

                    // Starfire
                    if (procSpell->SpellFamilyFlags[0] & 0x4)
                    {
                        triggered_spell_id = 40445;
                        chance = 25.0f;
                    }
                    // Rejuvenation
                    else if (procSpell->SpellFamilyFlags[0] & 0x10)
                    {
                        triggered_spell_id = 40446;
                        chance = 25.0f;
                    }
                    // Mangle (Bear)
                    else if (procSpell->SpellFamilyFlags[1] & 0x00000440)
                    {
                        triggered_spell_id = 40452;
                        chance = 40.0f;
                    }
                    else
                        return false;

                    if (!roll_chance_f(chance))
                        return false;

                    target = this;
                    break;
                }
                case 44835: // Maim Interrupt
                {
                    // Deadly Interrupt Effect
                    triggered_spell_id = 32747;
                    break;
                }
                case 70723: // Item - Druid T10 Balance 4P Bonus
                {
                    // Wrath & Starfire
                    if ((procSpell->SpellFamilyFlags[0] & 0x5) && (procEx & PROC_EX_CRITICAL_HIT))
                    {
                        triggered_spell_id = 71023;
                        SpellInfo const* triggeredSpell = sSpellMgr->GetSpellInfo(triggered_spell_id);
                        if (!triggeredSpell)
                            return false;
                        basepoints0 = CalculatePct(int32(damage), triggerAmount) / (triggeredSpell->GetMaxDuration() / triggeredSpell->Effects[0].Amplitude);
                        // Add remaining ticks to damage done
                        basepoints0 += victim->GetRemainingPeriodicAmount(GetGUID(), triggered_spell_id, SPELL_AURA_PERIODIC_DAMAGE);
                    }
                    break;
                }
                case 70664: // Item - Druid T10 Restoration 4P Bonus (Rejuvenation)
                {
                    // Proc only from normal Rejuvenation
                    if (procSpell->SpellVisual[0] != 32)
                        return false;

                    Player* caster = ToPlayer();
                    if (!caster)
                        return false;
                    if (!caster->GetGroup() && victim == this)
                        return false;

                    CastCustomSpell(70691, SPELLVALUE_BASE_POINT0, damage, victim, true);
                    return true;
                }
                case 102351:// Cenarion Ward
                {
                    if (!victim || !damage)
                        return false;

                    if (procSpell && procSpell->IsPositive())
                        return false;

                    if (!(procFlag & TAKEN_HIT_PROC_FLAG_MASK))
                        return false;

                    target = this;
                    triggered_spell_id = 102352;
                    break;
                }
            }
            break;
        }
        case SPELLFAMILY_ROGUE:
        {
            switch (dummySpell->Id)
            {
                case 51701: ///< Honor Among Thieves
                {
                    if (!triggeredByAura || !triggeredByAura->GetBase() || !triggeredByAura->GetBase()->GetCaster())
                        return false;

                    if (triggeredByAura->GetBase()->GetCaster()->GetTypeId() != TYPEID_PLAYER)
                        return false;

                    Player* l_Player = triggeredByAura->GetBase()->GetCaster()->ToPlayer();
                    if (!l_Player)
                        return false;

                    if (!l_Player->isInCombat())
                        return false;

                    if (l_Player->HasSpellCooldown(51699))
                        return false;

                    if (!(procEx & PROC_EX_CRITICAL_HIT))
                        return false;

                    triggered_spell_id = 51699;

                    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(triggered_spell_id);
                    if (!triggerEntry)
                        return false;

                    if (cooldown_spell_id == 0)
                        cooldown_spell_id = triggered_spell_id;

                    l_Player->CastSpell(target, triggered_spell_id, true, castItem, triggeredByAura, originalCaster);

                    l_Player->AddSpellCooldown(cooldown_spell_id, 0, cooldown);

                    return true;
                }
                case 63254: // Glyph of Deadly Momentum
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (!(procFlag & PROC_FLAG_KILL))
                        return false;

                    if (Aura* recuperate = GetAura(73651))
                        recuperate->RefreshDuration();

                    if (Aura* sliceAndDice = GetAura(5171))
                        sliceAndDice->RefreshDuration();

                    break;
                }
                case 51626: // Deadly Brew
                case 51667: // Cut to the Chase
                    return false;
                case 57934: // Tricks of the Trade
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    Unit* redirectTarget = GetMisdirectionTarget();
                    RemoveAura(57934);

                    // Item - Rogue T12 4P Bonus
                    if (HasAura(99175))
                    {
                        uint32 spellIds[3] = {99186, 99187, 99188};
                        uint32 crIds[3] = {CR_HASTE_MELEE, CR_CRIT_MELEE, CR_MASTERY};
                        uint32 i = urand(0, 2);
                        int32 bp0 = int32(CalculatePct(GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + crIds[i]), 25));
                        CastCustomSpell(this, spellIds[i], &bp0, 0, 0, true);
                    }
                    // Item - Rogue T13 2P Bonus
                    if (HasAura(105849))
                        CastSpell(this, 105864, true);

                    if (!redirectTarget)
                        break;
                    CastSpell(this,59628,true);
                    CastSpell(redirectTarget,57933,true);
                    break;
                }
                default:
                    break;
            }

            break;
        }
        case SPELLFAMILY_HUNTER:
        {
            switch (dummySpell->SpellIconID)
            {
                case 267: // Improved Mend Pet
                {
                    if (!roll_chance_i(triggerAmount))
                        return false;

                    triggered_spell_id = 24406;
                    break;
                }
                case 2236: // Thrill of the Hunt
                {
                    if (!procSpell)
                        return false;

                    int32 powerCost[MAX_POWERS_COST];
                    memset(powerCost, 0, sizeof(uint32)* MAX_POWERS_COST);
                    powerCost[MAX_POWERS_COST - 1] = 0;
                    for (auto itr : procSpell->SpellPowers)
                    {
                        procSpell->CalcPowerCost(this, SpellSchoolMask(procSpell->SchoolMask), powerCost);
                        basepoints0 = CalculatePct(powerCost[itr->PowerType], triggerAmount);
                        break;
                    }

                    if (basepoints0 <= 0)
                        return false;

                    target = this;
                    triggered_spell_id = 34720;
                    break;
                }
                case 3560: // Rapid Recuperation
                {
                    // This effect only from Rapid Killing (focus regen)
                    if (!(procSpell->SpellFamilyFlags[1] & 0x01000000))
                        return false;

                    target = this;
                    triggered_spell_id = 58883;
                    basepoints0 = CalculatePct(GetMaxPower(POWER_FOCUS), triggerAmount);
                    break;
                }
            }

            break;
        }
        case SPELLFAMILY_PALADIN:
        {
            switch (dummySpell->Id)
            {
                case 96887: // Variable Pulse Lightning Capacitor
                case 97119: // Variable Pulse Lightning Capacitor (Heroic)
                {
                    if (!victim)
                        return false;

                    if (Aura* aur = GetAura(96890))
                    {
                        uint8 stacks = aur->GetStackAmount();
                        if (roll_chance_i(15))
                        {
                            int32 bp0 = dummySpell->Effects[EFFECT_0].CalcValue() * stacks;
                            CastCustomSpell(victim, 96891, &bp0, 0, 0, true);
                            aur->Remove();
                            return true;
                        }
                    }
                    triggered_spell_id = 96890;
                    target = this;
                    break;
                }
                case 86172: // Divine Purpose
                {
                    if (!procSpell)
                        return false;

                    if (procSpell->PowerType != POWER_HOLY_POWER)
                        return false;

                    break;
                }
                // Item - Collecting Mana, Tyrande's Favirite Doll
                case 92272:
                {
                    if (procSpell && procSpell->ManaCostPercentage)
                    {
                        const int32 maxmana = 4200;
                        int32 mana = int32(0.2f * CalculatePct(GetCreateMana(), procSpell->ManaCostPercentage));
                        if (AuraEffect* aurEff = GetAuraEffect(92596, EFFECT_0))
                        {
                            int32 oldamount = aurEff->GetAmount();
                            if (oldamount < maxmana)
                            {
                                int32 newamount = std::min(maxmana, (oldamount + mana));
                                aurEff->ChangeAmount(newamount);
                            }
                        }
                        else
                        {
                            int32 bp = std::min(mana, maxmana);
                            CastCustomSpell(this, 92596, &bp, 0, 0, true);
                        }
                    }
                    break;
                }
                // Item - Paladin T12 Holy 4P Bonus
                case 99070:
                {
                    if (!victim || !victim->ToPlayer())
                        return false;

                    Player* plr = victim->ToPlayer();
                    if (!plr)
                        return false;

                    std::list<Player*> plrList;
                    JadeCore::AnyFriendlyUnitInObjectRangeCheck check(this, this, 15.0f);
                    JadeCore::PlayerListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, plrList, check);
                    VisitNearbyObject(15.0f, searcher);
                    if (plrList.empty())
                        return false;

                    plrList.remove(plr);

                    if (plrList.empty())
                        return false;

                    plrList.sort(JadeCore::HealthPctOrderPred());
                    plrList.resize(1);

                    int32 bp0 = int32(CalculatePct(damage, 10));

                    CastCustomSpell(plrList.front(), 99017, &bp0, 0, 0, true);
                    return true;
                }
                // Item - Paladin T12 Protection 2P Bonus
                case 99074:
                    basepoints0 = int32(CalculatePct(damage, triggerAmount));
                    triggered_spell_id = 99075;
                    target = victim;
                    break;
                // Item - Paladin T12 Retribution 2P Bonus
                case 99093:
                    if (!victim || GetGUID() == victim->GetGUID())
                        return false;
                    basepoints0 = int32(CalculatePct(damage, triggerAmount / 2)); // 2 ticks
                    triggered_spell_id = 99092;
                    target = victim;
                    break;
                /// Mastery: Illuminated Healing
                ///@ todo update me for 6.X
                case 76669:
                {
                    if (!victim || !procSpell || effIndex != 0 || GetTypeId() != TYPEID_PLAYER)
                        return false;

                    switch (procSpell->Id)
                    {
                        case 633:
                        case 85673:
                        case 19750:
                        case 82326:
                        case 20473:
                        case 25914:
                        case 82327:
                        case 86452:
                        case 114163: ///< Eternal Flame
                        case 156322: ///< Eternal Flame (DoT)
                        case 130551: ///< Word of GLory
                        case 85222: ///< Light of Dawn
                        {
                            float mastery = ToPlayer()->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.25f;
                            basepoints0 = int32(damage * float(mastery / 100.0f));
                            triggered_spell_id = 86273;

                            if (AuraEffect* aurShield = victim->GetAuraEffect(triggered_spell_id, EFFECT_0, GetGUID()))
                                basepoints0 += aurShield->GetAmount();

                            /// Illuminated Healing stacks up to a maximum of 1/3 of target's maximum health
                            int32 maxHealth = victim->GetMaxHealth() / 3;

                            if (basepoints0 > maxHealth)
                                basepoints0 = maxHealth;

                            break;
                        }
                    }
                    break;
                }
                // Ancient Crusader (player)
                case 86701:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    //if caster has no guardian of ancient kings aura then remove dummy aura
                    if (!HasAura(86698))
                    {
                        RemoveAurasDueToSpell(86701);
                        return false;
                    }

                    CastSpell(this, 86700, true);
                    return true;
                }
                // Ancient Crusader (guardian)
                case 86703:
                {
                    if (!GetOwner() || GetOwner()->GetTypeId() != TYPEID_PLAYER)
                        return false;

                    GetOwner()->CastSpell(this, 86700, true);
                    return true;
                }
                // Ancient Healer
                case 86674:
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // if caster has no guardian of ancient kings aura then remove dummy aura
                    if (!HasAura(86669))
                    {
                        RemoveAurasDueToSpell(86674);
                        return false;
                    }

                    // check for single target spell (TARGET_SINGLE_FRIEND, NO_TARGET)
                    if (!(procSpell->Effects[triggeredByAura->GetEffIndex()].TargetA.GetTarget() == TARGET_UNIT_TARGET_ALLY) &&
                        (procSpell->Effects[triggeredByAura->GetEffIndex()].TargetB.GetTarget() == 0))
                        return false;

                    std::list<Creature*> petlist;
                    GetCreatureListWithEntryInGrid(petlist, 46499, 100.0f);
                    if (!petlist.empty())
                    {
                        for (std::list<Creature*>::const_iterator itr = petlist.begin(); itr != petlist.end(); ++itr)
                        {
                            Unit* pPet = (*itr);
                            if (pPet->GetOwnerGUID() == GetGUID())
                            {
                                int32 bp0 = damage;
                                int32 bp1 = damage / 10;
                                pPet->CastCustomSpell(victim, 86678, &bp0, &bp1, NULL, true);
                            }
                        }
                    }

                    return true;
                }
                // Seal of Command
                case 105361:
                    if (effIndex != 0)
                        break;
                    triggered_spell_id = 118215;
                    break;
                // Seal of Righteousness
                case 20154:
                    triggered_spell_id = 101423;
                    break;
                case 157007:///< Beacon of Insight
                {
                    if (!victim)
                        return false;

                    if (Aura* l_BeaconOfInsight = triggeredByAura->GetBase())
                    {
                        if (victim->GetHealthPct() >= 90.0f)
                        {
                            l_BeaconOfInsight->Remove();
                            return true;
                        }
                    }

                    break;
                }
                case 28789: // Holy Power (Redemption Armor set)
                {
                    if (!victim)
                        return false;

                    // Set class defined buff
                    switch (victim->getClass())
                    {
                        case CLASS_PALADIN:
                        case CLASS_PRIEST:
                        case CLASS_SHAMAN:
                        case CLASS_DRUID:
                            triggered_spell_id = 28795;     // Increases the friendly target's mana regeneration by $s1 per 5 sec. for $d.
                            break;
                        case CLASS_MAGE:
                        case CLASS_WARLOCK:
                            triggered_spell_id = 28793;     // Increases the friendly target's spell damage and healing by up to $s1 for $d.
                            break;
                        case CLASS_HUNTER:
                        case CLASS_ROGUE:
                            triggered_spell_id = 28791;     // Increases the friendly target's attack power by $s1 for $d.
                            break;
                        case CLASS_WARRIOR:
                            triggered_spell_id = 28790;     // Increases the friendly target's armor
                            break;
                        default:
                            return false;
                    }

                    break;
                }
                case 54939: // Glyph of Divinity
                {
                    target = this;
                    triggered_spell_id = 54986;
                    basepoints0 = triggerAmount;
                    break;
                }
                case 31801: /// Seal of Truth (damage calc on apply aura)
                {                             /// Shouldn't proc for Divine storm and Hammer of the Righteous
                    if (effIndex != 0 || (procSpell && (procSpell->Id == 53385 || procSpell->Id == 88263)))  // effect 2 used by seal unleashing code
                        return false;

                    triggered_spell_id = 31803;

                    // Deals additionnal 12% weapon damage
                    CastSpell(victim, 42463, true);

                    break;
                }
                case 40470: // Paladin Tier 6 Trinket (Ashtongue Talisman of Zeal)
                {
                    if (!procSpell)
                        return false;

                    float chance;

                    // Flash of light
                    if (procSpell->SpellFamilyFlags[0] & 0xC0000000)
                    {
                        triggered_spell_id = 40471;
                        chance = 15.0f;
                    }
                    // Judgement (any)
                    else if (procSpell->GetSpellSpecific() == SpellSpecificType::SpellSpecificJudgement)
                    {
                        triggered_spell_id = 40472;
                        chance = 50.0f;
                    }
                    else
                        return false;

                    if (!roll_chance_f(chance))
                        return false;

                    break;
                }
                case 64890: // Item - Paladin T8 Holy 2P Bonus
                {
                    triggered_spell_id = 64891;
                    basepoints0 = triggerAmount * damage / 300;
                    break;
                }
                case 71406: // Tiny Abomination in a Jar
                case 71545: // Tiny Abomination in a Jar (Heroic)
                {
                    if (!victim || !victim->isAlive())
                        return false;

                    CastSpell(this, 71432, true, NULL, triggeredByAura);

                    Aura const* dummy = GetAura(71432);
                    if (!dummy || dummy->GetStackAmount() < (dummySpell->Id == 71406 ? 8 : 7))
                        return false;

                    RemoveAurasDueToSpell(71432);
                    triggered_spell_id = 71433;  // default main hand attack
                    // roll if offhand
                    if (Player const* player = ToPlayer())
                    if (player->GetWeaponForAttack(WeaponAttackType::OffAttack, true) && urand(0, 1))
                        triggered_spell_id = 71434;
                    target = victim;
                    break;
                }
                case 71880: // Item - Icecrown 25 Normal Dagger Proc
                {
                    switch (getPowerType())
                    {
                        case POWER_MANA:
                            triggered_spell_id = 71881;
                            break;
                        case POWER_RAGE:
                            triggered_spell_id = 71883;
                            break;
                        case POWER_ENERGY:
                            triggered_spell_id = 71882;
                            break;
                        case POWER_RUNIC_POWER:
                            triggered_spell_id = 71884;
                            break;
                        default:
                            return false;
                    }
                    break;
                }
                case 71892: // Item - Icecrown 25 Heroic Dagger Proc
                {
                    switch (getPowerType())
                    {
                        case POWER_MANA:
                            triggered_spell_id = 71888;
                            break;
                        case POWER_RAGE:
                            triggered_spell_id = 71886;
                            break;
                        case POWER_ENERGY:
                            triggered_spell_id = 71887;
                            break;
                        case POWER_RUNIC_POWER:
                            triggered_spell_id = 71885;
                            break;
                        default:
                            return false;
                    }
                    break;
                }
            }

            // Judgements of the Wise
            if (dummySpell->SpellIconID == 3017)
            {
                target = this;
                triggered_spell_id = 31930;
                break;
            }

            break;
        }
        case SPELLFAMILY_SHAMAN:
        {
            switch (dummySpell->Id)
            {
                case 16196: // Resurgence
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (!procSpell)
                        return false;

                    SpellInfo const* spellEnergize = sSpellMgr->GetSpellInfo(101033);
                    if (!spellEnergize)
                        return false;

                    int32 bp0 = spellEnergize->Effects[EFFECT_0].CalcValue(this);

                    switch (procSpell->Id)
                    {
                        case 77472: // Healing Wave
                            break;  // Normal bp0
                        case 8004:  // Healing Surge
                        case 61295: // Riptide
                        case 73685: // Unleash life
                            bp0 *= 0.6f;
                            break;
                        case 1064: // Chain Heal
                        case 177972:///< Chain Heal (T17 - 2P proc)
                            bp0 *= 0.33f;
                            break;
                        default:
                            bp0 = 0;
                            break;
                    }

                    if (bp0)
                        EnergizeBySpell(this, 101033, bp0, POWER_MANA);

                    break;
                }
                case 10400: ///< Flametongue
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (!(procFlag & PROC_FLAG_DONE_OFFHAND_ATTACK))
                        return false;

                    if (effIndex != EFFECT_0)
                        return false;

                    triggered_spell_id = 10444;

                    break;
                }
                case 324:   // Lightning Shield
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // Glyph of Lightning Shield
                    if (HasAura(101052))
                        CastSpell(this, 142912, true);

                    break;
                }
                // Totemic Power (The Earthshatterer set)
                case 28823:
                {
                    if (!victim)
                        return false;

                    // Set class defined buff
                    switch (victim->getClass())
                    {
                        case CLASS_PALADIN:
                        case CLASS_PRIEST:
                        case CLASS_SHAMAN:
                        case CLASS_DRUID:
                            triggered_spell_id = 28824;     // Increases the friendly target's mana regeneration by $s1 per 5 sec. for $d.
                            break;
                        case CLASS_MAGE:
                        case CLASS_WARLOCK:
                            triggered_spell_id = 28825;     // Increases the friendly target's spell damage and healing by up to $s1 for $d.
                            break;
                        case CLASS_HUNTER:
                        case CLASS_ROGUE:
                            triggered_spell_id = 28826;     // Increases the friendly target's attack power by $s1 for $d.
                            break;
                        case CLASS_WARRIOR:
                            triggered_spell_id = 28827;     // Increases the friendly target's armor
                            break;
                        default:
                            return false;
                    }
                    break;
                }
                // Lesser Healing Wave (Totem of Flowing Water Relic)
                case 28849:
                {
                    target = this;
                    triggered_spell_id = 28850;
                    break;
                }
                // Shaman Tier 6 Trinket
                case 40463:
                {
                    if (!procSpell)
                        return false;

                    float chance;
                    if (procSpell->SpellFamilyFlags[0] & 0x1)
                    {
                        triggered_spell_id = 40465;         // Lightning Bolt
                        chance = 15.0f;
                    }
                    else if (procSpell->SpellFamilyFlags[0] & 0x80)
                    {
                        triggered_spell_id = 40465;         // Lesser Healing Wave
                        chance = 10.0f;
                    }
                    else if (procSpell->SpellFamilyFlags[1] & 0x00000010)
                    {
                        triggered_spell_id = 40466;         // Stormstrike
                        chance = 50.0f;
                    }
                    else
                        return false;

                    if (!roll_chance_f(chance))
                        return false;

                    target = this;
                    break;
                }
                // Glyph of Healing Wave
                case 55440:
                {
                    // Not proc from self heals
                    if (this == victim)
                        return false;
                    basepoints0 = CalculatePct(int32(damage), triggerAmount);
                    target = this;
                    triggered_spell_id = 55533;
                    break;
                }
                // Shaman T8 Elemental 4P Bonus
                case 64928:
                {
                    basepoints0 = CalculatePct(int32(damage), triggerAmount);
                    triggered_spell_id = 64930;            // Electrified
                    break;
                }
                // Shaman T9 Elemental 4P Bonus
                case 67228:
                {
                    // Lava Burst
                    if (procSpell->SpellFamilyFlags[1] & 0x1000)
                    {
                        triggered_spell_id = 71824;
                        SpellInfo const* triggeredSpell = sSpellMgr->GetSpellInfo(triggered_spell_id);
                        if (!triggeredSpell)
                            return false;
                        basepoints0 = CalculatePct(int32(damage), triggerAmount) / (triggeredSpell->GetMaxDuration() / triggeredSpell->Effects[0].Amplitude);
                    }
                    break;
                }
                // Item - Shaman T10 Restoration 4P Bonus
                case 70808:
                {
                    // Chain Heal
                    if ((procSpell->SpellFamilyFlags[0] & 0x100) && (procEx & PROC_EX_CRITICAL_HIT))
                    {
                        triggered_spell_id = 70809;
                        SpellInfo const* triggeredSpell = sSpellMgr->GetSpellInfo(triggered_spell_id);
                        if (!triggeredSpell)
                            return false;
                        basepoints0 = CalculatePct(int32(damage), triggerAmount) / (triggeredSpell->GetMaxDuration() / triggeredSpell->Effects[0].Amplitude);
                        // Add remaining ticks to healing done
                        basepoints0 += GetRemainingPeriodicAmount(GetGUID(), triggered_spell_id, SPELL_AURA_PERIODIC_HEAL);
                    }
                    break;
                }
                // Item - Shaman T10 Elemental 2P Bonus
                case 70811:
                {
                    // Lightning Bolt & Chain Lightning
                    if (procSpell->SpellFamilyFlags[0] & 0x3)
                    {
                        if (ToPlayer()->HasSpellCooldown(16166))
                        {
                            uint32 newCooldownDelay = ToPlayer()->GetSpellCooldownDelay(16166);
                            if (newCooldownDelay < 3000)
                                newCooldownDelay = 0;
                            else
                                newCooldownDelay -= 2000;

                            ToPlayer()->AddSpellCooldown(16166, 0, newCooldownDelay);
                            ToPlayer()->ReduceSpellCooldown(16166, 2000);
                            return true;
                        }
                    }
                    return false;
                }
                // Item - Shaman T10 Elemental 4P Bonus
                case 70817:
                {
                    if (!target)
                        return false;
                    // try to find spell Flame Shock on the target
                    if (AuraEffect const* aurEff = target->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_SHAMAN, 0x10000000, 0x0, 0x0, GetGUID()))
                    {
                        Aura* flameShock  = aurEff->GetBase();
                        int32 maxDuration = flameShock->GetMaxDuration();
                        int32 newDuration = flameShock->GetDuration() + 2 * aurEff->GetAmplitude();

                        flameShock->SetDuration(newDuration);
                        // is it blizzlike to change max duration for FS?
                        if (newDuration > maxDuration)
                            flameShock->SetMaxDuration(newDuration);

                        return true;
                    }
                    // if not found Flame Shock
                    return false;
                }
                break;
            }
            // Frozen Power
            if (dummySpell->SpellIconID == 3780)
            {
                if (!target)
                    return false;
                if (GetDistance(target) < 15.0f)
                    return false;
                float chance = (float)triggerAmount;
                if (!roll_chance_f(chance))
                    return false;

                triggered_spell_id = 63685;
                break;
            }
            // Ancestral Awakening
            if (dummySpell->SpellIconID == 3065)
            {
                triggered_spell_id = 52759;
                basepoints0 = CalculatePct(int32(damage), triggerAmount);
                target = this;
                break;
            }
            // Earth Shield
            if (dummySpell->Id == 974)
            {
                // 3.0.8: Now correctly uses the Shaman's own spell critical strike chance to determine the chance of a critical heal.
                originalCaster = triggeredByAura->GetCasterGUID();
                target = this;
                basepoints0 = triggerAmount;
                triggered_spell_id = 379;
                break;
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            switch (dummySpell->Id)
            {
                // Item - Death Knight T12 DPS 4P Bonus
                case 98996:
                    basepoints0 = int32(CalculatePct(damage, triggerAmount));
                    triggered_spell_id = 99000;
                    target = victim;
                    break;
                case 49028: // Dancing Rune Weapon
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // 1 dummy aura for dismiss rune blade
                    if (effIndex != 1)
                        return false;

                    Unit* pPet = NULL;
                    for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr) // Find Rune Weapon
                        if ((*itr)->GetEntry() == 27893)
                        {
                            pPet = *itr;
                            break;
                        }

                    if (pPet && (pPet->getVictim() || getVictim()) && damage && procSpell)
                    {
                        int32 procDmg = damage / 2;

                        // Soul Reaper must be casted correctly
                        if (procSpell->Effects[0].Effect == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE && procSpell->Id != 114866)
                        {
                            pPet->SendSpellNonMeleeDamageLog(pPet->getVictim() ? pPet->getVictim() : getVictim(), procSpell->Id, procDmg, procSpell->GetSchoolMask(), 0, 0, false, 0, false);
                            pPet->DealDamage(pPet->getVictim() ? pPet->getVictim() : getVictim(), procDmg, NULL, SPELL_DIRECT_DAMAGE, procSpell->GetSchoolMask(), procSpell, true);
                        }
                        else
                        {
                            if (!pPet->ToCreature()->HasSpellCooldown(procSpell->Id))
                            {
                                pPet->SendSpellNonMeleeDamageLog(pPet->getVictim() ? pPet->getVictim() : getVictim(), procSpell->Id, procDmg, procSpell->GetSchoolMask(), 0, 0, false, 0, false);
                                pPet->DealDamage(pPet->getVictim() ? pPet->getVictim() : getVictim(), procDmg, NULL, SPELL_DIRECT_DAMAGE, procSpell->GetSchoolMask(), procSpell, true);
                                procDmg = 0;
                                pPet->ToCreature()->_AddCreatureSpellCooldown(procSpell->Id, time(nullptr) + 1);
                                pPet->CastCustomSpell(pPet->getVictim() ? pPet->getVictim() : getVictim(), procSpell->Id, &procDmg, NULL, NULL, true);
                            }
                        }

                        break;
                    }
                    // Outbreak must be casted correctly too
                    else if (pPet && (pPet->getVictim() || getVictim()) && procSpell && procSpell->Id == 77575)
                        pPet->CastSpell(pPet->getVictim() ? pPet->getVictim() : getVictim(), procSpell->Id, true);
                    else
                        return false;

                    return true; // Return true because triggered_spell_id is not exist in DBC, nothing to trigger
                }
                case 59327: // Glyph of Unholy Command
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    ToPlayer()->RemoveSpellCooldown(49560, true);
                    ToPlayer()->RemoveSpellCooldown(49576, true);
                    break;
                }
                case 61257: // Runic Power Back on Snare/Root
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // only for spells and hit/crit (trigger start always) and not start from self casted spells
                    if (procSpell == 0 || !(procEx & (PROC_EX_NORMAL_HIT|PROC_EX_CRITICAL_HIT)) || this == victim)
                        return false;
                    // Need snare or root mechanic
                    if (!(procSpell->GetAllEffectsMechanicMask() & ((1<<MECHANIC_ROOT)|(1<<MECHANIC_SNARE))))
                        return false;
                    triggered_spell_id = 61258;
                    target = this;
                    break;
                }
                case 66192: // Threat of Thassarian
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    // Must Dual Wield
                    if (!procSpell || !haveOffhandWeapon())
                        return false;

                    switch (procSpell->Id)
                    {
                        case 49020: triggered_spell_id = 66198; break; // Obliterate
                        case 49143: triggered_spell_id = 66196; break; // Frost Strike
                        case 45462: triggered_spell_id = 66216; break; // Plague Strike
                        case 49998: triggered_spell_id = 66188; break; // Death Strike
                        default:
                            return false;
                    }

                    break;
                }
                case 77606: // Dark Simulacrum
                {
                    if (!procSpell)
                        return false;

                    Unit* caster = triggeredByAura->GetCaster();
                    victim = this;

                    if (!caster || !victim)
                        return false;

                    bool l_IsStolenSpell = sObjectMgr->IsStolenSpell(procSpell->Id);
                    caster->removeSimulacrumTarget();

                    if ((!procSpell->IsCanBeStolen() || !triggeredByAura) && !l_IsStolenSpell)
                        return false;

                    if (Creature* targetCreature = victim->ToCreature())
                        if (!targetCreature->isCanGiveSpell(caster, procSpell) && !l_IsStolenSpell)
                            return false;

                    caster->setSimulacrumTarget(victim->GetGUID());

                    if (HasAura(77616))
                        return false;

                    // Replacer
                    int32  basepoints0 = procSpell->Id;
                    caster->CastCustomSpell(this, 77616, &basepoints0, NULL, NULL, true);

                    // SpellPower
                    basepoints0 = victim->SpellBaseDamageBonusDone(SpellSchoolMask(procSpell->SchoolMask));
                    caster->CastCustomSpell(caster, 94984, &basepoints0, &basepoints0, NULL, true);
                    return true;
                }
            }

            break;
        }
        case SPELLFAMILY_POTION:
        {
            // alchemist's stone
            if (dummySpell->Id == 17619)
            {
                if (procSpell->SpellFamilyName == SPELLFAMILY_POTION)
                {
                    for (uint8 i = 0; i < procSpell->EffectCount; i++)
                    {
                        if (procSpell->Effects[i].Effect == SPELL_EFFECT_HEAL)
                        {
                            triggered_spell_id = 21399;
                        }
                        else if (procSpell->Effects[i].Effect == SPELL_EFFECT_ENERGIZE)
                        {
                            triggered_spell_id = 21400;
                        }
                        else
                            continue;

                        basepoints0 = int32(CalculateSpellDamage(this, procSpell, i) * 0.4f);
                        CastCustomSpell(this, triggered_spell_id, &basepoints0, NULL, NULL, true, NULL, triggeredByAura);
                    }
                    return true;
                }
            }
            break;
        }
        case SPELLFAMILY_PET:
        {
            switch (dummySpell->SpellIconID)
            {
                // Guard Dog
                case 201:
                {
                    if (!victim)
                        return false;

                    triggered_spell_id = 54445;
                    target = this;
                    float addThreat = float(CalculatePct(procSpell->Effects[0].CalcValue(this), triggerAmount));
                    victim->AddThreat(this, addThreat);
                    break;
                }
                // Silverback
                case 1582:
                    triggered_spell_id = dummySpell->Id == 62765 ? 62801 : 62800;
                    target = this;
                    break;
            }
            break;
        }
        case SPELLFAMILY_MONK:
        {
            switch (dummySpell->Id)
            {
                case 128938:// Brewing : Elusive Brew
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (!(procEx & PROC_EX_CRITICAL_HIT))
                        return false;

                    if (!(procFlag & PROC_FLAG_DONE_MAINHAND_ATTACK) && !(procFlag & PROC_FLAG_DONE_OFFHAND_ATTACK))
                        return false;

                    if (procSpell)
                        return false;

                    Aura* elusiveBrew = AddAura(128939, this);
                    if (!elusiveBrew)
                        return false;

                    // All ModStackAmount with amount -1 because of AddAura
                    if (Item* mainItem = ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                    {
                        if (mainItem->GetTemplate())
                        {
                            if (mainItem->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                            {
                                if (mainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_STAFF)
                                {
                                    // 25% chance to generate 2 charges or 75% chance to generate 3 charges
                                    if (roll_chance_i(25))
                                        elusiveBrew->ModStackAmount(1);
                                    else
                                        elusiveBrew->ModStackAmount(2);
                                }
                                else if (mainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM)
                                {
                                    // 100% chance to generate 3 charges
                                    elusiveBrew->ModStackAmount(2);
                                }
                            }
                            else
                            {
                                // 50% chance to generate 1 charges or 50% chance to generate 2 charges
                                if (roll_chance_i(50))
                                    elusiveBrew->ModStackAmount(1);
                            }
                        }
                    }

                    break;
                }
                case 124502:// Gift of the Ox
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (ToPlayer()->HasSpellCooldown(124502))
                        return false;

                    float roll_chance = 0.0f;

                    // Gift of the Ox can proc from all successful melee attack hits
                    // On special attacks, it has a 10% proc chance.
                    if (procFlag & PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS)
                        roll_chance = 10;
                    // Gift of the Ox now has a higher proc rate on white attacks: [0.03*WeaponSpeed] for 1H weapons, and [0.06*WeaponSpeed] for 2H weapons.
                    else
                    {
                        Item* mainItem = ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                        Item* offItem = ToPlayer()->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if (procFlag & PROC_FLAG_DONE_MAINHAND_ATTACK && mainItem && mainItem->GetTemplate())
                        {
                            if (mainItem->GetTemplate()->InventoryType == INVTYPE_2HWEAPON) // 2H
                                roll_chance = 0.06f * float(mainItem->GetTemplate()->Delay) / 10.0f;
                            else // 1H
                                roll_chance = 0.03f * float(mainItem->GetTemplate()->Delay) / 10.0f;
                        }
                        else if (procFlag & PROC_FLAG_DONE_OFFHAND_ATTACK && offItem && offItem->GetTemplate())
                            roll_chance = 0.03f * float(offItem->GetTemplate()->Delay) / 10.0f;
                    }

                    if (!roll_chance_f(roll_chance))
                        return false;

                    int rand = irand(1, 2);

                    if (rand == 1)
                        triggered_spell_id = 124503;
                    else
                        triggered_spell_id = 124506;

                    target = this;

                    ToPlayer()->AddSpellCooldown(124502, 0, 1 * IN_MILLISECONDS); // Prevent multiple procs

                    break;
                }
                case 116033:// Sparring (stacks)
                {
                    if (!victim)
                        return false;

                    if (!victim->HasAura(116087))
                        return false;

                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    triggered_spell_id = 116033;
                    target = this;

                    break;
                }
                case 137639:// Storm, Earth and Fire
                {
                    if (GetTypeId() != TYPEID_PLAYER)
                        return false;

                    if (effIndex != 2)
                        return false;

                    if (!damage || !procSpell)
                        return false;

                    if (procSpell->IsPositive())
                        return false;

                    Unit* firstSpirit = NULL;
                    Unit* secondSpirit = NULL;

                    for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr) // Find spirits
                    {
                        if ((*itr)->GetEntry() == 69680 || (*itr)->GetEntry() == 69792 || (*itr)->GetEntry() == 69791)
                        {
                            if (!firstSpirit)
                            {
                                firstSpirit = *itr;
                                continue;
                            }
                            if (!secondSpirit)
                            {
                                secondSpirit = *itr;
                                continue;
                            }
                        }
                    }

                    if (firstSpirit && (firstSpirit->getVictim() || getVictim()))
                    {
                        if (firstSpirit->getVictim() != victim && !firstSpirit->ToCreature()->HasSpellCooldown(procSpell->Id))
                        {
                            firstSpirit->CastSpell(firstSpirit->getVictim() ? firstSpirit->getVictim() : getVictim(), procSpell->Id, true);
                            firstSpirit->ToCreature()->_AddCreatureSpellCooldown(procSpell->Id, time(nullptr) + 1);
                        }
                    }

                    if (secondSpirit && (secondSpirit->getVictim() || getVictim()))
                    {
                        if (secondSpirit->getVictim() != victim && !secondSpirit->ToCreature()->HasSpellCooldown(procSpell->Id))
                        {
                            secondSpirit->CastSpell(secondSpirit->getVictim() ? secondSpirit->getVictim() : getVictim(), procSpell->Id, true);
                            secondSpirit->ToCreature()->_AddCreatureSpellCooldown(procSpell->Id, time(nullptr) + 1);
                        }
                    }

                    return true;
                }
            }
            break;
        }
        default:
            break;
    }

    // if not handled by custom case, get triggered spell from dummySpell proto
    if (!triggered_spell_id)
        triggered_spell_id = dummySpell->Effects[triggeredByAura->GetEffIndex()].TriggerSpell;

    // processed charge only counting case
    if (!triggered_spell_id)
        return true;

    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(triggered_spell_id);
    if (!triggerEntry)
        return false;

    if (cooldown_spell_id == 0)
        cooldown_spell_id = triggered_spell_id;

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(cooldown_spell_id))
        return false;

    if (basepoints0)
        CastCustomSpell(target, triggered_spell_id, &basepoints0, NULL, NULL, true, castItem, triggeredByAura, originalCaster);
    else
        CastSpell(target, triggered_spell_id, true, castItem, triggeredByAura, originalCaster);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(cooldown_spell_id, 0, cooldown);

    return true;
}

bool Unit::HandleObsModEnergyAuraProc(Unit* victim, uint32 /*damage*/, AuraEffect* triggeredByAura, SpellInfo const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown)
{
    SpellInfo const* dummySpell = triggeredByAura->GetSpellInfo(); ///< dummySpell is never read 01/18/16
    //uint32 effIndex = triggeredByAura->GetEffIndex();
    //int32  triggerAmount = triggeredByAura->GetAmount();

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    uint32 triggered_spell_id = 0;
    Unit* target = victim;
    int32 basepoints0 = 0;

    // processed charge only counting case
    if (!triggered_spell_id)
        return true;

    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(triggered_spell_id);

    // Try handle unknown trigger spells
    if (!triggerEntry)
        return false;

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(triggered_spell_id))
        return false;
    if (basepoints0)
        CastCustomSpell(target, triggered_spell_id, &basepoints0, NULL, NULL, true, castItem, triggeredByAura);
    else
        CastSpell(target, triggered_spell_id, true, castItem, triggeredByAura);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(triggered_spell_id, 0, cooldown);
    return true;
}

bool Unit::HandleModDamagePctTakenAuraProc(Unit* victim, uint32 /*damage*/, AuraEffect* triggeredByAura, SpellInfo const* /*procSpell*/, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown)
{
    SpellInfo const* dummySpell = triggeredByAura->GetSpellInfo(); ///< dummySpell is never read 01/18/16

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    uint32 triggered_spell_id = 0;
    Unit* target = victim;
    int32 basepoints0 = 0;

    // processed charge only counting case
    if (!triggered_spell_id)
        return true;

    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(triggered_spell_id);

    if (!triggerEntry)
        return false;

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(triggered_spell_id))
        return false;

    if (basepoints0)
        CastCustomSpell(target, triggered_spell_id, &basepoints0, NULL, NULL, true, castItem, triggeredByAura);
    else
        CastSpell(target, triggered_spell_id, true, castItem, triggeredByAura);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(triggered_spell_id, 0, cooldown);

    return true;
}

// Used in case when access to whole aura is needed
// All procs should be handled like this...
bool Unit::HandleAuraProc(Unit* victim, uint32 /*damage*/, Aura* triggeredByAura, SpellInfo const* procSpell, uint32 /*procFlag*/, uint32 /*procEx*/, uint32 cooldown, bool * handled)
{
    SpellInfo const* dummySpell = triggeredByAura->GetSpellInfo();

    switch (dummySpell->SpellFamilyName)
    {
        case SPELLFAMILY_GENERIC:
            switch (dummySpell->Id)
            {
                // Nevermelting Ice Crystal
                case 71564:
                    RemoveAuraFromStack(71564);
                    *handled = true;
                    break;
                // Gaseous Bloat
                case 70672:
                case 72455:
                case 72832:
                case 72833:
                {
                    *handled = true;
                    uint32 stack = triggeredByAura->GetStackAmount();
                    int32 const mod = (GetMap()->GetSpawnMode() & 1) ? 1500 : 1250;
                    int32 dmg = 0;
                    for (uint8 i = 1; i < stack; ++i)
                        dmg += mod * stack;
                    if (Unit* caster = triggeredByAura->GetCaster())
                        caster->CastCustomSpell(70701, SPELLVALUE_BASE_POINT0, dmg);
                    break;
                }
                // Ball of Flames Proc
                case 71756:
                case 72782:
                case 72783:
                case 72784:
                    RemoveAuraFromStack(dummySpell->Id);
                    *handled = true;
                    break;
                // Discerning Eye of the Beast
                case 59915:
                {
                    CastSpell(this, 59914, true);   // 59914 already has correct basepoints in DBC, no need for custom bp
                    *handled = true;
                    break;
                }
                // Swift Hand of Justice
                case 59906:
                {
                    int32 bp0 = CalculatePct(GetMaxHealth(), dummySpell->Effects[EFFECT_0].CalcValue());
                    CastCustomSpell(this, 59913, &bp0, NULL, NULL, true);
                    *handled = true;
                    break;
                }
            }

            break;
        case SPELLFAMILY_PALADIN:
        {
            // Judgements of the Just
            if (dummySpell->SpellIconID == 3015)
            {
                *handled = true;
                CastSpell(victim, 68055, true);
                return true;
            }
            // Repentance aura drop
            if (dummySpell->Id == 20066)
            {
                *handled = true;
                if (procSpell && procSpell->SpellFamilyName == SPELLFAMILY_PALADIN &&
                    procSpell->SpellFamilyFlags[0] == 0x20000000 && procSpell->SpellFamilyFlags[1] == 0x00000800)
                    return false;
                return true;
            }
            break;
        }
        case SPELLFAMILY_MAGE:
        {
            switch (dummySpell->Id)
            {
                // Empowered Fire
                case 31656:
                case 31657:
                case 31658:
                {
                    *handled = true;

                    SpellInfo const* spInfo = sSpellMgr->GetSpellInfo(67545);
                    if (!spInfo)
                        return false;

                    int32 bp0 = int32(CalculatePct(GetCreateMana(), spInfo->Effects[0].CalcValue()));
                    CastCustomSpell(this, 67545, &bp0, NULL, NULL, true, NULL, triggeredByAura->GetEffect(EFFECT_0), GetGUID());
                    return true;
                }
            }
            break;
        }
        case SPELLFAMILY_DEATHKNIGHT:
        {
            switch (dummySpell->Id)
            {
                // Bone Shield cooldown
                case 49222:
                {
                    *handled = true;
                    if (cooldown && IsPlayer())
                    {
                        if (ToPlayer()->HasSpellCooldown(100000))
                            return false;
                        ToPlayer()->AddSpellCooldown(100000, 0, cooldown);
                    }
                    return true;
                }
                // Hungering Cold aura drop
                case 51209:
                    *handled = true;
                    // Drop only in not disease case
                    if (procSpell && procSpell->Dispel == DISPEL_DISEASE)
                        return false;
                    return true;
            }
            break;
        }
        case SPELLFAMILY_WARRIOR:
        {
            switch (dummySpell->Id)
            {
                // Item - Warrior T10 Protection 4P Bonus
                case 70844:
                {
                    int32 basepoints0 = CalculatePct(GetMaxHealth(), dummySpell->Effects[EFFECT_1].CalcValue());
                    CastCustomSpell(this, 70845, &basepoints0, NULL, NULL, true);
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case SPELLFAMILY_WARLOCK:
            // Soulburn
            if (dummySpell->Id == 74434)
                if (procSpell->Id == 6353)
                    // Item - Warlock T13 4P Bonus (Soulburn)
                    if (HasAura(105787))
                        EnergizeBySpell(this, 74434, 1, POWER_SOUL_SHARDS);
            break;
    }
    return false;
}

bool Unit::HandleProcTriggerSpell(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 procFlags, uint32 procEx, uint32 cooldown)
{
    // Get triggered aura spell info
    SpellInfo const* auraSpellInfo = triggeredByAura->GetSpellInfo();

    // Basepoints of trigger aura
    int32 triggerAmount = triggeredByAura->GetAmount();

    // Set trigger spell id, target, custom basepoints
    uint32 trigger_spell_id = auraSpellInfo->Effects[triggeredByAura->GetEffIndex()].TriggerSpell;

    Unit*  target = NULL;
    int32  basepoints0 = 0;

    if (triggeredByAura->GetAuraType() == SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE)
        basepoints0 = triggerAmount;

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    // Try handle unknown trigger spells
    if (sSpellMgr->GetSpellInfo(trigger_spell_id) == NULL)
    {
        switch (auraSpellInfo->SpellFamilyName)
        {
            case SPELLFAMILY_GENERIC:
                switch (auraSpellInfo->Id)
                {
                    case 23780:             // Aegis of Preservation (Aegis of Preservation trinket)
                        trigger_spell_id = 23781;
                        break;
                    case 33896:             // Desperate Defense (Stonescythe Whelp, Stonescythe Alpha, Stonescythe Ambusher)
                        trigger_spell_id = 33898;
                        break;
                    case 43820:             // Charm of the Witch Doctor (Amani Charm of the Witch Doctor trinket)
                        // Pct value stored in dummy
                        basepoints0 = victim->GetCreateHealth() * auraSpellInfo->Effects[1].CalcValue() / 100;
                        target = victim;
                        break;
                    case 139316:            ////< Putrify (Dark Animus trash - Throne of Thunder)
                    {
                        if (GetTypeId() != TYPEID_UNIT)
                            return false;

                        trigger_spell_id = 139317;
                        target = victim;
                        break;
                    }
                    case 140296:            ///< Conductive Shield (Lei Shen trash - Throne of Thunder)
                    {
                        if (GetTypeId() != TYPEID_UNIT)
                            return false;

                        trigger_spell_id = 140299;
                        target = victim;
                        break;
                    }
                    case 138201:            /// Lei Shen's gift (Lei Shen trash - Throne of Thunder)
                    {
                        if (GetTypeId() != TYPEID_UNIT)
                            return false;

                        trigger_spell_id = 138210;
                        target = victim;
                        break;
                    }
                    case 57345:             // Darkmoon Card: Greatness
                    {
                        float stat = 0.0f;
                        // strength
                        if (GetStat(STAT_STRENGTH) > stat) { trigger_spell_id = 60229;stat = GetStat(STAT_STRENGTH); }
                        // agility
                        if (GetStat(STAT_AGILITY)  > stat) { trigger_spell_id = 60233;stat = GetStat(STAT_AGILITY);  }
                        // intellect
                        if (GetStat(STAT_INTELLECT)> stat) { trigger_spell_id = 60234;stat = GetStat(STAT_INTELLECT);}
                        // spirit
                        if (GetStat(STAT_SPIRIT)   > stat) { trigger_spell_id = 60235;                               }
                        break;
                    }
                    case 64568:             // Blood Reserve
                    {
                        if (HealthBelowPctDamaged(35, damage))
                        {
                            CastCustomSpell(this, 64569, &triggerAmount, NULL, NULL, true);
                            RemoveAura(64568);
                        }
                        return false;
                    }
                    case 67702:             // Death's Choice, Item - Coliseum 25 Normal Melee Trinket
                    {
                        float stat = 0.0f;
                        // strength
                        if (GetStat(STAT_STRENGTH) > stat) { trigger_spell_id = 67708;stat = GetStat(STAT_STRENGTH); }
                        // agility
                        if (GetStat(STAT_AGILITY)  > stat) { trigger_spell_id = 67703;                               }
                        break;
                    }
                    case 67771:             // Death's Choice (heroic), Item - Coliseum 25 Heroic Melee Trinket
                    {
                        float stat = 0.0f;
                        // strength
                        if (GetStat(STAT_STRENGTH) > stat) { trigger_spell_id = 67773;stat = GetStat(STAT_STRENGTH); }
                        // agility
                        if (GetStat(STAT_AGILITY)  > stat) { trigger_spell_id = 67772;                               }
                        break;
                    }
                    // Mana Drain Trigger
                    case 27522:
                    case 40336:
                    {
                        // On successful melee or ranged attack gain $29471s1 mana and if possible drain $27526s1 mana from the target.
                        if (this && isAlive())
                            CastSpell(this, 29471, true, castItem, triggeredByAura);
                        if (victim && victim->isAlive())
                            CastSpell(victim, 27526, true, castItem, triggeredByAura);
                        return true;
                    }
                    // Evasive Maneuvers
                    case 50240:
                    {
                        // Remove a Evasive Charge
                        Aura* charge = GetAura(50241);
                        if (charge->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL))
                            RemoveAurasDueToSpell(50240);
                        break;
                    }
                    // Warrior - Vigilance, SPELLFAMILY_GENERIC
                    case 50720:
                    {
                        target = triggeredByAura->GetCaster();
                        if (!target)
                            return false;

                        break;
                    }
                }
                break;
            case SPELLFAMILY_PRIEST:
            {
                // Greater Heal Refund
                if (auraSpellInfo->Id == 37594)
                    trigger_spell_id = 37595;
                break;
            }
            case SPELLFAMILY_DRUID:
            {
                switch (auraSpellInfo->Id)
                {
                    // Druid Forms Trinket
                    case 37336:
                    {
                        switch (GetShapeshiftForm())
                        {
                            case FORM_NONE:     trigger_spell_id = 37344; break;
                            case FORM_CAT:      trigger_spell_id = 37341; break;
                            case FORM_BEAR:     trigger_spell_id = 37340; break;
                            case FORM_TREE:     trigger_spell_id = 37342; break;
                            case FORM_MOONKIN:  trigger_spell_id = 37343; break;
                            default:
                                return false;
                        }
                        break;
                    }
                    // Druid T9 Feral Relic (Lacerate, Swipe, Mangle, and Shred)
                    case 67353:
                    {
                        switch (GetShapeshiftForm())
                        {
                            case FORM_CAT:      trigger_spell_id = 67355; break;
                            case FORM_BEAR:     trigger_spell_id = 67354; break;
                            default:
                                return false;
                        }
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case SPELLFAMILY_HUNTER:
            {
                if (auraSpellInfo->SpellIconID == 3247)     // Piercing Shots
                {
                    trigger_spell_id = 63468;

                    SpellInfo const* TriggerPS = sSpellMgr->GetSpellInfo(trigger_spell_id);
                    if (!TriggerPS)
                        return false;

                    basepoints0 = CalculatePct(int32(damage), triggerAmount) / (TriggerPS->GetMaxDuration() / TriggerPS->Effects[0].Amplitude);
                    basepoints0 += victim->GetRemainingPeriodicAmount(GetGUID(), trigger_spell_id, SPELL_AURA_PERIODIC_DAMAGE);
                    break;
                }
                // Item - Hunter T9 4P Bonus
                if (auraSpellInfo->Id == 67151)
                {
                    trigger_spell_id = 68130;
                    target = this;
                    break;
                }
                break;
            }
            case SPELLFAMILY_PALADIN:
            {
                switch (auraSpellInfo->Id)
                {
                    // Healing Discount
                    case 37705:
                    {
                        trigger_spell_id = 37706;
                        target = this;
                        break;
                    }
                    // Soul Preserver
                    case 60510:
                    {
                        switch (getClass())
                        {
                            case CLASS_DRUID:
                                trigger_spell_id = 60512;
                                break;
                            case CLASS_PALADIN:
                                trigger_spell_id = 60513;
                                break;
                            case CLASS_PRIEST:
                                trigger_spell_id = 60514;
                                break;
                            case CLASS_SHAMAN:
                                trigger_spell_id = 60515;
                                break;
                        }

                        target = this;
                        break;
                    }
                    case 37657: // Lightning Capacitor
                    case 54841: // Thunder Capacitor
                    case 67712: // Item - Coliseum 25 Normal Caster Trinket
                    case 67758: // Item - Coliseum 25 Heroic Caster Trinket
                    {
                        if (!victim || !victim->isAlive() || GetTypeId() != TYPEID_PLAYER)
                            return false;

                        uint32 stack_spell_id = 0;
                        switch (auraSpellInfo->Id)
                        {
                            case 37657:
                                stack_spell_id = 37658;
                                trigger_spell_id = 37661;
                                break;
                            case 54841:
                                stack_spell_id = 54842;
                                trigger_spell_id = 54843;
                                break;
                            case 67712:
                                stack_spell_id = 67713;
                                trigger_spell_id = 67714;
                                break;
                            case 67758:
                                stack_spell_id = 67759;
                                trigger_spell_id = 67760;
                                break;
                        }

                        CastSpell(this, stack_spell_id, true, NULL, triggeredByAura);

                        Aura* dummy = GetAura(stack_spell_id);
                        if (!dummy || dummy->GetStackAmount() < triggerAmount)
                            return false;

                        RemoveAurasDueToSpell(stack_spell_id);
                        target = victim;
                        break;
                    }
                    // Item - Paladin T13 Protection 2P Bonus (Judgement)
                    case 105800:
                    {
                        if (!damage)
                            return false;

                        basepoints0 = triggerAmount * damage / 100.0f;
                        trigger_spell_id = 105801;
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case SPELLFAMILY_SHAMAN:
            {
                switch (auraSpellInfo->Id)
                {
                    // Lightning Shield (The Ten Storms set)
                    case 23551:
                    {
                        trigger_spell_id = 23552;
                        target = victim;
                        break;
                    }
                    // Damage from Lightning Shield (The Ten Storms set)
                    case 23552:
                    {
                        trigger_spell_id = 27635;
                        break;
                    }
                    // Mana Surge (The Earthfury set)
                    case 23572:
                    {
                        if (!procSpell)
                            return false;
                        basepoints0 = int32(CalculatePct(procSpell->ManaCost, 35));
                        trigger_spell_id = 23571;
                        target = this;
                        break;
                    }
                }
                break;
            }
            case SPELLFAMILY_DEATHKNIGHT:
            {
                // Item - Death Knight T10 Melee 4P Bonus
                if (auraSpellInfo->Id == 70656)
                {
                    if (GetTypeId() != TYPEID_PLAYER || getClass() != CLASS_DEATH_KNIGHT)
                        return false;

                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                        if (ToPlayer()->GetRuneCooldown(i) == 0)
                            return false;
                }
                break;
            }
            case SPELLFAMILY_ROGUE:
            {
                switch (auraSpellInfo->Id)
                {
                    // Rogue T10 2P bonus, should only proc on caster
                    case 70805:
                    {
                        if (victim != this)
                            return false;
                        break;
                    }
                    // Rogue T10 4P bonus, should proc on victim
                    case 70803:
                    {
                        target = victim;
                        break;
                    }
                }
                break;
            }
            default:
                 break;
        }
    }

    // All ok. Check current trigger spell
    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(trigger_spell_id);
    if (triggerEntry == NULL)
    {
        // Don't cast unknown spell
        // sLog->outError(LOG_FILTER_UNITS, "Unit::HandleProcTriggerSpell: Spell %u has 0 in EffectTriggered[%d]. Unhandled custom case?", auraSpellInfo->Id, triggeredByAura->GetEffIndex());
        return false;
    }

    // not allow proc extra attack spell at extra attack
    if (m_extraAttacks && triggerEntry->HasEffect(SPELL_EFFECT_ADD_EXTRA_ATTACKS))
        return false;

    // Custom requirements (not listed in procEx) Warning! damage dealing after this
    // Custom triggered spells
    switch (auraSpellInfo->Id)
    {
        // Item - Death Knight T12 Blood 2P Bonus (wrong spellname, that's warrior item set)
        case 105907:
            if (!procSpell)
                return false;
            if (procSpell->Id != 12294 && !roll_chance_i(50))
                return false;
            break;
        case 14190: ///< Seal Fate
        {
            if (GetTypeId() != TypeID::TYPEID_PLAYER)
                return false;

            if (!(procEx & PROC_EX_CRITICAL_HIT))
                return false;

            if (!procSpell || (!procSpell->HasEffect(SPELL_EFFECT_ADD_COMBO_POINTS) && procSpell->Id != 5374 && procSpell->Id != 27576))
                return false;

            break;
        }
        case 104428:// Elemental Force (DND)
        {
            if (!IsValidAttackTarget(victim))
                return false;

            break;
        }
        // Item - Mage T13 2P Bonus (Haste)
        case 105788:
            if (!procSpell)
                return false;
            if (procSpell->Id != 30451 && !roll_chance_i(50))
                return false;
            break;
        case 54937: // Glyph of Illumination
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 25912 && procSpell->Id != 25914)
                return false;

            if (!(procEx & PROC_EX_CRITICAL_HIT))
                return false;

            break;
        }
        case 58228: // Glyph of Dark Archangel
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 2944)
                return false;

            break;
        }
        // Item - Hunter T12 2P Bonus
        case 99057:
            if (!victim || GetGUID() == victim->GetGUID())
                return false;
            break;
        // Item - Warrior T12 DPS 4P Bonus
        case 99238:
            // there are 3 Raging Blow spells, filter it
            if (!procSpell || !(procSpell->Id == 12294 || procSpell->Id == 96103))
                return false;
            break;
        case 90998: // Song of Sorrow, Sorrowsong
        case 91003: // Song of Sorrow, Sorrowsong (H)
        case 92180: // Item - Proc Armor, Leaden Despair
        case 92185: // Item - Proc Armor, Leaden Despair (H)
        case 92236: // Item - Proc Mastery Below 35%, Symbiotic Worm
        case 92356: // Item - Proc Mastery Below 35%, Symbiotic Worm (H)
        case 92234: // Item - Proc Dodge Below 35%, Bedrock Talisman
        case 96947: // Loom of Fate, Spediersilk Spindle
        case 97130: // Loom of Fate, Spediersilk Spindle (H)
        case 105552: // Item - Death Knight T12 Blood 2P Bonus
            if (!HealthBelowPct(35) && !HealthBelowPctDamaged(35, damage))
                return false;
            break;
        case 109175:// Divine Insight (Shadow)
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!procSpell)
                return false;

            // procs only from Shadow Word: Pain
            if (procSpell->Id != 589 && procSpell->Id != 124464)
                return false;

            break;
        }
        case 108945:// Angelic Bulwark
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!damage)
                return false;

            if ((GetHealth() - damage) >= CountPctFromMaxHealth(30))
                return false;

            if (ToPlayer()->HasSpellCooldown(108945))
                return false;

            ToPlayer()->AddSpellCooldown(108945, 0, 90 * IN_MILLISECONDS);
            basepoints0 = int32(CountPctFromMaxHealth(15));

            break;
        }
        case 125732:// Glyph of Honor
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!procSpell)
                return false;

            if (procSpell->Id != 115080)
                return false;

            break;
        }
        case 31829: // Item Paladin - Pvp Retributon 4P bonus
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (ToPlayer()->HasSpellCooldown(31829))
                return false;

            ToPlayer()->AddSpellCooldown(31829, 0, 8 * IN_MILLISECONDS);

            break;
        }
        case 54926: // Glyph of Templar's Verdict
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!procSpell)
                return false;

            // Only for Exorcism, Templar's Verdict and Final Verdict
            if (procSpell->Id != 879 && procSpell->Id != 85256 && procSpell->Id != 157048)
                return false;

            break;
        }
        case 54943: // Glyph of Blessed Life
            return false;
        case 109306:// Trill of the Hunt
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!procSpell)
                return false;

            // ranged attack that costs Focus or Kill Command
            if (procSpell->Id != 34026 && procSpell->PowerType != POWER_FOCUS && procSpell->DmgClass != SPELL_DAMAGE_CLASS_RANGED)
                return false;

            trigger_spell_id = 34720;
            target = this;

            break;
        }
        case 2823:  // Deadly Poison
        case 3408:  // Crippling Poison
        case 5761:  // Mind-Numbling Poison
        case 8679:  // Wound Poison
        case 108211:// Leeching Poison
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            // Don't trigger poison if no damage dealed (except for absorb)
            if (!damage && !(procEx & PROC_EX_ABSORB))
                return false;

            break;
        }
        case 79684: // Arcane Missiles !
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!procSpell)
                return false;

            if (procSpell->Id == 4143 || procSpell->Id == 7268 || procSpell->Id == 155152)
                return false;

            if (ToPlayer()->GetSpecializationId() != SPEC_MAGE_ARCANE)
                return false;

            if (Aura* arcaneMissiles = GetAura(79683))
            {
                arcaneMissiles->ModCharges(1);
                arcaneMissiles->RefreshDuration();
                return false;
            }

            break;
        }
        case 144586:// Item - Paladin T16 Retribution 2P Bonus - 144586 (proc Warrior of the Light - 144587)
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 59578)
                return false;

            break;
        }
        case 144593://Item - Paladin T16 Retribution 4P Bonus
        {
            if (!procSpell)
                return false;

            if (procSpell->PowerType != POWER_HOLY_POWER)
                return false;

            break;
        }
        case 53576: // Infusion of Light
        {
            if (!procSpell)
                return false;

            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!(procSpell->Id == 25912) && !(procSpell->Id == 25914))
                return false;

            if (!(procEx & PROC_EX_CRITICAL_HIT))
                return false;

            break;
        }
        case 85043: // Grand Crusader
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!(procEx & PROC_EX_DODGE) && !(procEx & PROC_EX_PARRY))
                return false;

            break;
        }
        case 68164:// Glyph of Thunder Strike
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 6343)
                return false;

            break;
        }
        case 49530: // Sudden Doom
        {
            if (!roll_chance_i(15))
                return false;

            if (!HasAura(48265))
                return false;

            break;
        }
        case 165395:///< Necrosis
        {
            if (!procSpell)
                return false;

            if (!(procEx & PROC_EX_INTERNAL_MULTISTRIKE))
                return false;

            break;
        }
        case 165698:///< Item - Druid WoD PvP Feral 4P Bonus
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 5221)
                return false;

            if (!(procEx & PROC_EX_CRITICAL_HIT))
                return false;

            break;
        }
        case 170877:///< Item Rogue WoD PvP Subtlety 4P Bonus
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 1966)
                return false;

            break;
        }
        case 166012:///< Item - Hunter WoD PvP Survival 4P Bonus
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 3674)
                return false;

            if (procEx & PROC_EX_INTERNAL_MULTISTRIKE)
                return false;

            break;
        }
        case 144865:// Item - Druid T16 Feral 2P bonus
            if (procSpell->Id != 16864 && procSpell->Id != 135700)
                return false;
            break;
        case 145003:// Item - Shaman T16 Elemental 4P Bonus
        {
            if (!roll_chance_i(20))
                return false;

            if (procSpell->Id != 403 && procSpell->Id != 421)
                return false;

            if (ToPlayer()->GetSpecializationId() != SPEC_DK_UNHOLY)
                return false;

            break;
        }
        case 49572: // Shadow infusion
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 47632)
                return false;

            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (Pet* pet = ToPlayer()->GetPet())
            {
                /// If pet is already in Dark Transformation we can't add Shadow Infusion
                if (pet->HasAura(63560))
                    return false;

                uint8 stackAmount = 0;
                if (Aura* aura = pet->GetAura(trigger_spell_id))
                    stackAmount = aura->GetStackAmount();

                if (stackAmount >= 4) // Apply Dark Transformation
                    CastSpell(this, 93426, true);
            }

            break;
        }
        case 33371: // Glyph of Mind Spike
        {
            if (!procSpell)
                return false;

            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!victim)
                return false;

            if (procSpell->Id != 73510)
                return false;

            break;
        }
        case 122509:// Ultimatum
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 23922 || procEx != PROC_EX_CRITICAL_HIT)
                return false;

            break;
        }
        case 57954: // Glyph of Fire From the Heavens
        {
            if (!procSpell)
                return false;

            if ((procSpell->Id != 24275 && procSpell->Id != 20271) || procEx != PROC_EX_CRITICAL_HIT)
                return false;

            break;
        }
        case 145672:// Riposte (Warrior)
        {
            if (!(procEx & PROC_EX_DODGE) && !(procEx & PROC_EX_PARRY))
                return false;

            Player* plr = ToPlayer();
            if (!plr)
                return false;

            basepoints0 = CalculatePct((plr->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_PARRY) + plr->GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_DODGE)), triggerAmount);

            break;
        }
        // Item - Dragon Soul Legendary Daggers
        case 109939:
        {
            if (!victim)
                return false;

            if (HasAura(109949))
                return false;

            if (Aura* aur = GetAura(109941))
            {
                uint8 stacks = aur->GetStackAmount();
                if (stacks >= 30)
                {
                    float chance = ((1.0f / (51.0f - stacks)) * 100);
                    if (roll_chance_f(chance))
                    {
                        CastSpell(victim, 109949, true);
                        aur->Remove();
                        return false;
                    }
                }
            }
            break;
        }
        // Fusing Vapors, Yor'sahj, Dragon Soul
        case 103968:
            if (GetHealthPct() > 50.0f)
                return false;
            break;
        // Embedded Blade, Mannoroth, Well of Eternity
        case 109542:
            if (!victim)
                return false;
            target = victim;
            break;
        case 76857: // Mastery : Critical Block
        case 79147: // Sanguinary Vein
        case 91023: // Find Weakness
        case 108942:// Phantasm
        case 113043:// Omen of Clarity (new)
        case 122464:// Dematerialize
        case 54927: // Glyph of Avenging Wrath
        case 124487:// Zen Focus
        case 88764: // Rolling Thunder
        case 115946:// Glyph of Burning Anger
        case 88821: // Daybreak
        case 131542:// Relentless Grip
        case 56420: // Glyph of Denounce
        case 117967:// Brewmaster Training
        case 134563:// Healing Elixirs
        case 131564:// Arcane Intensity
            return false;
        case 35551: // Combat Potency
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (procSpell && procSpell->Id != 86392)
                return false;

            if (procSpell && procSpell->Id == 86392)
                if (!roll_chance_i(20))
                    return false;

            float offHandSpeed = GetAttackTime(WeaponAttackType::OffAttack) / IN_MILLISECONDS;

            if (!procSpell && (procFlags & PROC_FLAG_DONE_OFFHAND_ATTACK))
                if (!roll_chance_f(20.0f * offHandSpeed / 1.4f))
                    return false;

            break;
        }
        case 121152:// Blindside
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 1329)
                return false;

            if (!roll_chance_i(30))
                return false;

            break;
        }
        case 116645:// Teachings of The Monastery (Blackout Kick)
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 100784)
                return false;

            break;
        }
        // Teachings of the Monastery (Tiger Palm)
        case 118672:
        {
            if (!procSpell)
                return false;

            if (procSpell->Id != 100787)
                return false;

            break;
        }
        // Enrage
        case 13046:
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            if (!procSpell)
                return false;

            if (!(procEx & PROC_EX_CRITICAL_HIT))
                return false;

            // Devastate, Shield Slam, Mortal Strike, Bloodthirst and Colossus Smash critical strikes and critical blocks Enrage you
            if (procSpell->Id != 20243 && procSpell->Id != 23922 && procSpell->Id != 12294 && procSpell->Id != 23881 && procSpell->Id != 167105)
                return false;

            if (HasAura(115946))
                AddAura(115993, this);

            break;
        }
        // Backdraft
        case 117896:
        {
            if (!procSpell || (procSpell->Id != 17962 && procSpell->Id != 108685))
                return false;

            if (GetTypeId() != TYPEID_PLAYER || getClass() != CLASS_WARLOCK || ToPlayer()->GetSpecializationId() != SPEC_WARLOCK_DESTRUCTION)
                return false;

            break;
        }
        // Persistent Shield (Scarab Brooch trinket)
        // This spell originally trigger 13567 - Dummy Trigger (vs dummy efect)
        case 26467:
        {
            basepoints0 = int32(CalculatePct(damage, 15));
            target = victim;
            trigger_spell_id = 26470;
            break;
        }
        // Unyielding Knights (item exploit 29108\29109)
        case 38164:
        {
            if (!victim || victim->GetEntry() != 19457)  // Proc only if your target is Grillok
                return false;
            break;
        }
        // Deflection
        case 52420:
        {
            if (!HealthBelowPctDamaged(35, damage))
                return false;
            break;
        }

        // Cheat Death
        case 28845:
        {
            // When your health drops below 20%
            if (HealthBelowPctDamaged(20, damage) || HealthBelowPct(20))
                return false;
            break;
        }
        // Greater Heal Refund (Avatar Raiment set)
        case 37594:
        {
            if (!victim || !victim->isAlive())
                return false;

            // Doesn't proc if target already has full health
            if (victim->IsFullHealth())
                return false;
            // If your Greater Heal brings the target to full health, you gain $37595s1 mana.
            if (victim->GetHealth() + damage < victim->GetMaxHealth())
                return false;
            break;
        }
        // Bonus Healing (Crystal Spire of Karabor mace)
        case 40971:
        {
            // If your target is below $s1% health
            if (!victim || !victim->isAlive() || victim->HealthAbovePct(triggerAmount))
                return false;
            break;
        }
        // Rapid Recuperation
        case 53228:
        case 53232:
        {
            // This effect only from Rapid Fire (ability cast)
            if (!(procSpell->SpellFamilyFlags[0] & 0x20))
                return false;
            break;
        }
        // Decimation
        case 63156:
        case 63158:
            // Can proc only if target has hp below 25%
            if (!victim || !victim->HealthBelowPct(auraSpellInfo->Effects[EFFECT_1].CalcValue()))
                return false;
            break;
        // Deathbringer Saurfang - Blood Beast's Blood Link
        case 72176:
            basepoints0 = 3;
            break;
        // Professor Putricide - Ooze Spell Tank Protection
        case 71770:
            if (victim)
                victim->CastSpell(victim, trigger_spell_id, true);    // EffectImplicitTarget is self
            return true;
        case 45057: // Evasive Maneuvers (Commendation of Kael`thas trinket)
        case 71634: // Item - Icecrown 25 Normal Tank Trinket 1
        case 71640: // Item - Icecrown 25 Heroic Tank Trinket 1
        case 75475: // Item - Chamber of Aspects 25 Normal Tank Trinket
        case 75481: // Item - Chamber of Aspects 25 Heroic Tank Trinket
        {
            // Procs only if damage takes health below $s1%
            if (!HealthBelowPctDamaged(triggerAmount, damage))
                return false;
            break;
        }
        default:
            break;
    }

    // Custom basepoints/target for exist spell
    // dummy basepoints or other customs
    switch (trigger_spell_id)
    {
        // Auras which should proc on area aura source (caster in this case):
        // Cast positive spell on enemy target
        case 7099:  // Curse of Mending
        case 39703: // Curse of Mending
        case 29494: // Temptation
        case 20233: // Improved Lay on Hands (cast on target)
        {
            target = victim;
            break;
        }
        // Item - Druid T10 Balance 2P Bonus
        case 16870:
        {
            if (HasAura(70718))
                CastSpell(this, 70721, true);
            break;
        }
        // Enlightenment (trigger only from mana cost spells)
        case 35095:
        {
            if (!procSpell || procSpell->PowerType != POWER_MANA || (procSpell->ManaCost == 0 && procSpell->ManaCostPercentage == 0))
                return false;
            break;
        }
        case 46916:  // Slam! (Bloodsurge proc)
        case 52437:  // Sudden Death
        {
            // Item - Warrior T10 Melee 4P Bonus
            if (AuraEffect const* aurEff = GetAuraEffect(70847, 0))
            {
                if (!roll_chance_i(aurEff->GetAmount()))
                    break;
                CastSpell(this, 70849, true, castItem, triggeredByAura); // Extra Charge!
                CastSpell(this, 71072, true, castItem, triggeredByAura); // Slam GCD Reduced
                CastSpell(this, 71069, true, castItem, triggeredByAura); // Execute GCD Reduced
            }
            break;
        }
        // Sword and Board
        case 50227:
        {
            // Remove cooldown on Shield Slam
            if (IsPlayer())
                ToPlayer()->ResetCharges(procSpell->ChargeCategoryEntry);
            break;
        }
        // Maelstrom Weapon
        case 53817:
        {
            // Item - Shaman T10 Enhancement 4P Bonus
            if (AuraEffect const* aurEff = GetAuraEffect(70832, 0))
                if (Aura const* maelstrom = GetAura(53817))
                    if ((maelstrom->GetStackAmount() == maelstrom->GetSpellInfo()->StackAmount - 1) && roll_chance_i(aurEff->GetAmount()))
                        CastSpell(this, 70831, true, castItem, triggeredByAura);

            // Full Maelstrom Visual
            if (Aura const* maelstrom = GetAura(53817))
                if (maelstrom->GetStackAmount() >= 4)
                    CastSpell(this, 60349, true);

            break;
        }
        // Glyph of Death's Embrace
        case 58679:
        {
            // Proc only from healing part of Death Coil. Check is essential as all Death Coil spells have 0x2000 mask in SpellFamilyFlags
            if (!procSpell || !(procSpell->SpellFamilyName == SPELLFAMILY_DEATHKNIGHT && procSpell->SpellFamilyFlags[0] == 0x80002000))
                return false;
            break;
        }
        // Savage Defense
        case 62606:
        {
            basepoints0 = CalculatePct(triggerAmount, GetTotalAttackPowerValue(WeaponAttackType::BaseAttack));
            break;
        }
        // Shadow's Fate (Shadowmourne questline)
        case 71169:
        {
            if (GetTypeId() != TYPEID_PLAYER)
                return false;

            Player* player = ToPlayer();
            if (player->GetQuestStatus(24749) == QUEST_STATUS_INCOMPLETE)       // Unholy Infusion
            {
                if (!player->HasAura(71516) || victim->GetEntry() != 36678)    // Shadow Infusion && Professor Putricide
                    return false;
            }
            else if (player->GetQuestStatus(24756) == QUEST_STATUS_INCOMPLETE)  // Blood Infusion
            {
                if (!player->HasAura(72154) || victim->GetEntry() != 37955)    // Thirst Quenched && Blood-Queen Lana'thel
                    return false;
            }
            else if (player->GetQuestStatus(24757) == QUEST_STATUS_INCOMPLETE)  // Frost Infusion
            {
                if (!player->HasAura(72290) || victim->GetEntry() != 36853)    // Frost-Imbued Blade && Sindragosa
                    return false;
            }
            else if (player->GetQuestStatus(24547) != QUEST_STATUS_INCOMPLETE)  // A Feast of Souls
                return false;

            if (victim->GetTypeId() != TYPEID_UNIT)
                return false;
            // critters are not allowed
            if (victim->GetCreatureType() == CREATURE_TYPE_CRITTER)
                return false;
            break;
        }
        // Death's Advance
        case 96268:
        {
            if (!ToPlayer())
                return false;
            if (!ToPlayer()->GetRuneCooldown(RUNE_UNHOLY*2) || !ToPlayer()->GetRuneCooldown(RUNE_UNHOLY*2+1))
                return false;
            break;
        }
        case 135288:// Tooth and Claw
        {
            if (procSpell)
                return false;

            if (!roll_chance_i(40))
                return false;

            break;
        }
        case 3409:  // Crippling Poison
        case 112961:// Leeching Poison
        {
            // Shuriken Toss cannot trigger non lethal poison
            if (procSpell)
            {
                if (procSpell->Id == 137584 || procSpell->Id == 137585 ||
                    procSpell->Id == 140308 || procSpell->Id == 140309)
                    return false;
            }

            break;
        }
    }

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(trigger_spell_id))
        return false;

    // try detect target manually if not set
    if (target == NULL)
        target = !(procFlags & (PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS)) && triggerEntry && triggerEntry->IsPositive() ? this : victim;

    if (basepoints0)
        CastCustomSpell(target, trigger_spell_id, &basepoints0, NULL, NULL, true, castItem, triggeredByAura);
    else
        CastSpell(target, trigger_spell_id, true, castItem, triggeredByAura);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(trigger_spell_id, 0, cooldown);

    return true;
}

bool Unit::HandleOverrideClassScriptAuraProc(Unit* victim, uint32 /*damage*/, AuraEffect* triggeredByAura, SpellInfo const* /*procSpell*/, uint32 cooldown)
{
    int32 scriptId = triggeredByAura->GetMiscValue();

    if (!victim || !victim->isAlive())
        return false;

    Item* castItem = triggeredByAura->GetBase()->GetCastItemGUID() && IsPlayer()
        ? ToPlayer()->GetItemByGuid(triggeredByAura->GetBase()->GetCastItemGUID()) : NULL;

    uint32 triggered_spell_id = 0;

    switch (scriptId)
    {
        case 4533:                                          // Dreamwalker Raiment 2 pieces bonus
        {
            // Chance 50%
            if (!roll_chance_i(50))
                return false;

            switch (victim->getPowerType())
            {
                case POWER_MANA:   triggered_spell_id = 28722; break;
                case POWER_RAGE:   triggered_spell_id = 28723; break;
                case POWER_ENERGY: triggered_spell_id = 28724; break;
                default:
                    return false;
            }
            break;
        }
        case 4537:                                          // Dreamwalker Raiment 6 pieces bonus
            triggered_spell_id = 28750;                     // Blessing of the Claw
            break;
        default:
            break;
    }

    // not processed
    if (!triggered_spell_id)
        return false;

    // standard non-dummy case
    SpellInfo const* triggerEntry = sSpellMgr->GetSpellInfo(triggered_spell_id);

    if (!triggerEntry)
        return false;

    if (cooldown && IsPlayer() && ToPlayer()->HasSpellCooldown(triggered_spell_id))
        return false;

    CastSpell(victim, triggered_spell_id, true, castItem, triggeredByAura);

    if (cooldown && IsPlayer())
        ToPlayer()->AddSpellCooldown(triggered_spell_id, 0, cooldown);

    return true;
}

void Unit::setPowerType(Powers new_powertype)
{
    SetUInt32Value(UNIT_FIELD_DISPLAY_POWER, new_powertype);

    if (IsPlayer())
    {
        if (ToPlayer()->GetGroup())
            ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_POWER_TYPE);
    }
    else if (Pet* pet = ToCreature()->ToPet())
    {
        if (pet->isControlled())
        {
            Unit* owner = GetOwner();
            if (owner && (owner->IsPlayer()) && owner->ToPlayer()->GetGroup())
                owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_POWER_TYPE);
        }
    }

    switch (new_powertype)
    {
        default:
        case POWER_MANA:
            break;
        case POWER_RAGE:
            SetMaxPower(POWER_RAGE, GetCreatePowers(POWER_RAGE));
            SetPower(POWER_RAGE, 0);
            break;
        case POWER_FOCUS:
            SetMaxPower(POWER_FOCUS, GetCreatePowers(POWER_FOCUS));
            SetPower(POWER_FOCUS, GetCreatePowers(POWER_FOCUS));
            break;
        case POWER_ENERGY:
            SetMaxPower(POWER_ENERGY, GetCreatePowers(POWER_ENERGY));
            break;
    }

    SetPower(new_powertype, GetPower(new_powertype));

    if (IsPlayer())
        ToPlayer()->UpdateManaRegen();
}

FactionTemplateEntry const* Unit::getFactionTemplateEntry() const
{
    FactionTemplateEntry const* entry = sFactionTemplateStore.LookupEntry(getFaction());
    if (!entry)
    {
        static uint64 guid = 0;                             // prevent repeating spam same faction problem

        if (GetGUID() != guid)
            guid = GetGUID();
    }
    return entry;
}

// function based on function Unit::UnitReaction from 13850 client
ReputationRank Unit::GetReactionTo(Unit const* target) const
{
    // always friendly to self
    if (this == target)
        return REP_FRIENDLY;

    if (!target)
        return REP_FRIENDLY;

    // always friendly to charmer or owner
    if (GetCharmerOrOwnerOrSelf() == target->GetCharmerOrOwnerOrSelf())
        return REP_FRIENDLY;

    Player const* selfPlayerOwner = GetAffectingPlayer();
    Player const* targetPlayerOwner = target->GetAffectingPlayer();

    // check forced reputation to support SPELL_AURA_FORCE_REACTION
    if (selfPlayerOwner)
    {
        if (FactionTemplateEntry const* targetFactionTemplateEntry = target->getFactionTemplateEntry())
        {
            if (ReputationRank const* repRank = selfPlayerOwner->GetReputationMgr().GetForcedRankIfAny(targetFactionTemplateEntry))
                return *repRank;
        }
    }
    else if (targetPlayerOwner)
    {
        if (FactionTemplateEntry const* selfFactionTemplateEntry = getFactionTemplateEntry())
        {
            if (ReputationRank const* repRank = targetPlayerOwner->GetReputationMgr().GetForcedRankIfAny(selfFactionTemplateEntry))
                return *repRank;
        }
    }

    if (HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE))
    {
        if (target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE))
        {
            if (selfPlayerOwner && targetPlayerOwner)
            {
                // always friendly to other unit controlled by player, or to the player himself
                if (selfPlayerOwner == targetPlayerOwner)
                    return REP_FRIENDLY;

                // duel - always hostile to opponent
                if (selfPlayerOwner->m_Duel && selfPlayerOwner->m_Duel->opponent == targetPlayerOwner && selfPlayerOwner->m_Duel->startTime != 0)
                    return REP_HOSTILE;

                // same group - checks dependant only on our faction - skip FFA_PVP for example
                if (selfPlayerOwner->IsInRaidWith(targetPlayerOwner))
                    return REP_FRIENDLY; // return true to allow config option AllowTwoSide.Interaction.Group to work
                    // however client seems to allow mixed group parties, because in 13850 client it works like:
                    // return GetFactionReactionTo(getFactionTemplateEntry(), target);
            }

            // check FFA_PVP
            if (GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_FFA_PVP
                && target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_FFA_PVP)
                return REP_HOSTILE;

            if (selfPlayerOwner)
            {
                if (FactionTemplateEntry const* targetFactionTemplateEntry = target->getFactionTemplateEntry())
                {
                    if (ReputationRank const* repRank = selfPlayerOwner->GetReputationMgr().GetForcedRankIfAny(targetFactionTemplateEntry))
                        return *repRank;
                    if (!selfPlayerOwner->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_IGNORE_REPUTATION))
                    {
                        if (FactionEntry const* targetFactionEntry = sFactionStore.LookupEntry(targetFactionTemplateEntry->Faction))
                        {
                            if (targetFactionEntry->CanHaveReputation())
                            {
                                // check contested flags
                                if (targetFactionTemplateEntry->Flags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD
                                    && selfPlayerOwner->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP))
                                    return REP_HOSTILE;

                                // if faction has reputation, hostile state depends only from AtWar state
                                if (selfPlayerOwner->GetReputationMgr().IsAtWar(targetFactionEntry))
                                    return REP_HOSTILE;
                                return REP_FRIENDLY;
                            }
                        }
                    }
                }
            }
        }
    }
    // do checks dependant only on our faction
    return GetFactionReactionTo(getFactionTemplateEntry(), target);
}

ReputationRank Unit::GetFactionReactionTo(FactionTemplateEntry const* factionTemplateEntry, Unit const* target)
{
    // always neutral when no template entry found
    if (!factionTemplateEntry)
        return REP_NEUTRAL;

    FactionTemplateEntry const* targetFactionTemplateEntry = target->getFactionTemplateEntry();
    if (!targetFactionTemplateEntry)
        return REP_NEUTRAL;

    if (Player const* targetPlayerOwner = target->GetAffectingPlayer())
    {
        // check contested flags
        if (factionTemplateEntry->Flags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD
            && targetPlayerOwner->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP))
            return REP_HOSTILE;
        if (ReputationRank const* repRank = targetPlayerOwner->GetReputationMgr().GetForcedRankIfAny(factionTemplateEntry))
            return *repRank;
        if (!target->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_IGNORE_REPUTATION))
        {
            if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionTemplateEntry->Faction))
            {
                if (factionEntry->CanHaveReputation())
                {
                    // CvP case - check reputation, don't allow state higher than neutral when at war
                    ReputationRank repRank = targetPlayerOwner->GetReputationMgr().GetRank(factionEntry);
                    if (targetPlayerOwner->GetReputationMgr().IsAtWar(factionEntry))
                        repRank = std::min(REP_NEUTRAL, repRank);
                    return repRank;
                }
            }
        }
    }

    // common faction based check
    if (factionTemplateEntry->IsHostileTo(*targetFactionTemplateEntry))
        return REP_HOSTILE;
    if (factionTemplateEntry->IsFriendlyTo(*targetFactionTemplateEntry))
        return REP_FRIENDLY;
    if (targetFactionTemplateEntry->IsFriendlyTo(*factionTemplateEntry))
        return REP_FRIENDLY;
    if (factionTemplateEntry->Flags & FACTION_TEMPLATE_FLAG_HOSTILE_BY_DEFAULT)
        return REP_HOSTILE;
    // neutral by default
    return REP_NEUTRAL;
}

bool Unit::IsHostileTo(Unit const* unit) const
{
    return GetReactionTo(unit) <= REP_HOSTILE;
}

bool Unit::IsFriendlyTo(Unit const* unit) const
{
    return GetReactionTo(unit) >= REP_FRIENDLY;
}

bool Unit::IsHostileToPlayers() const
{
    FactionTemplateEntry const* my_faction = getFactionTemplateEntry();
    if (!my_faction || !my_faction->Faction)
        return false;

    FactionEntry const* raw_faction = sFactionStore.LookupEntry(my_faction->Faction);
    if (raw_faction && raw_faction->ReputationIndex >= 0)
        return false;

    return my_faction->IsHostileToPlayers();
}

bool Unit::IsNeutralToAll() const
{
    FactionTemplateEntry const* my_faction = getFactionTemplateEntry();
    if (!my_faction || !my_faction->Faction)
        return true;

    FactionEntry const* raw_faction = sFactionStore.LookupEntry(my_faction->Faction);
    if (raw_faction && raw_faction->ReputationIndex >= 0)
        return false;

    return my_faction->IsNeutralToAll();
}

bool Unit::Attack(Unit* victim, bool meleeAttack)
{
    if (!victim || victim == this)
        return false;

    // dead units can neither attack nor be attacked
    if (!isAlive() || !victim->IsInWorld() || !victim->isAlive())
        return false;

    // player cannot attack in mount state
    if (IsPlayer() && IsMounted())
        return false;

    // nobody can attack GM in GM-mode
    if (victim->IsPlayer())
    {
        if (victim->ToPlayer()->isGameMaster())
            return false;
    }
    else
    {
        if (!victim->ToCreature() || victim->ToCreature()->IsInEvadeMode())
            return false;
    }

    // remove SPELL_AURA_MOD_UNATTACKABLE at attack (in case non-interruptible spells stun aura applied also that not let attack)
    if (HasAuraType(SPELL_AURA_MOD_UNATTACKABLE))
        RemoveAurasByType(SPELL_AURA_MOD_UNATTACKABLE);

    if (m_attacking)
    {
        if (m_attacking == victim)
        {
            // switch to melee attack from ranged/magic
            if (meleeAttack)
            {
                if (!HasUnitState(UNIT_STATE_MELEE_ATTACKING))
                {
                    AddUnitState(UNIT_STATE_MELEE_ATTACKING);
                    SendMeleeAttackStart(victim);
                    return true;
                }
            }
            else if (HasUnitState(UNIT_STATE_MELEE_ATTACKING))
            {
                ClearUnitState(UNIT_STATE_MELEE_ATTACKING);
                SendMeleeAttackStop(victim);
                return true;
            }
            return false;
        }

        // switch target
        InterruptSpell(CURRENT_MELEE_SPELL);
        if (!meleeAttack)
            ClearUnitState(UNIT_STATE_MELEE_ATTACKING);
    }

    if (m_attacking)
        m_attacking->_removeAttacker(this);

    m_attacking = victim;
    m_attacking->_addAttacker(this);

    // Set our target
    SetTarget(victim->GetGUID());

    if (meleeAttack)
        AddUnitState(UNIT_STATE_MELEE_ATTACKING);

    // set position before any AI calls/assistance
    //if (GetTypeId() == TYPEID_UNIT)
    //    ToCreature()->SetCombatStartPosition(GetPositionX(), GetPositionY(), GetPositionZ());

    if (GetTypeId() == TYPEID_UNIT && !ToCreature()->isPet())
    {
        // should not let player enter combat by right clicking target - doesn't helps
        SetInCombatWith(victim);
        if (victim->IsPlayer())
            victim->SetInCombatWith(this);
        AddThreat(victim, 0.0f);

        ToCreature()->SendAIReaction(AI_REACTION_HOSTILE);
        ToCreature()->CallAssistance();
    }

    if (IsPlayer())
        if (ToPlayer()->GetEmoteState())
            ToPlayer()->SetEmoteState(0);

    // delay offhand weapon attack to next attack time
    if (haveOffhandWeapon())
        resetAttackTimer(WeaponAttackType::OffAttack);

    if (meleeAttack)
        SendMeleeAttackStart(victim);

    // Let the pet know we've started attacking someting. Handles melee attacks only
    // Spells such as auto-shot and others handled in WorldSession::HandleCastSpellOpcode
    if (this->IsPlayer())
    {
        Pet* playerPet = this->ToPlayer()->GetPet();

        if (playerPet && playerPet->isAlive())
            playerPet->AI()->OwnerAttacked(victim);
    }

    return true;
}

bool Unit::AttackStop()
{
    if (!m_attacking)
        return false;

    Unit* victim = m_attacking;

    m_attacking->_removeAttacker(this);
    m_attacking = NULL;

    // Clear our target
    SetTarget(0);

    ClearUnitState(UNIT_STATE_MELEE_ATTACKING);

    InterruptSpell(CURRENT_MELEE_SPELL);

    // reset only at real combat stop
    if (Creature* creature = ToCreature())
    {
        creature->SetNoCallAssistance(false);

        if (creature->HasSearchedAssistance())
        {
            creature->SetNoSearchAssistance(false);
            UpdateSpeed(MOVE_RUN, false);
        }
    }

    SendMeleeAttackStop(victim);

    if (IsPlayer())
        if (ToPlayer()->GetEmoteState())
            ToPlayer()->SetEmoteState(0);

    return true;
}

void Unit::CombatStop(bool includingCast)
{
    if (includingCast && IsNonMeleeSpellCasted(false))
        InterruptNonMeleeSpells(false);

    AttackStop();
    RemoveAllAttackers();
    if (IsPlayer())
        ToPlayer()->SendAttackSwingCancelAttack();     // melee and ranged forced attack cancel
    ClearInCombat();
}

void Unit::CombatStopWithPets(bool includingCast)
{
    CombatStop(includingCast);

    for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr)
        (*itr)->CombatStop(includingCast);
}

bool Unit::isAttackingPlayer() const
{
    if (HasUnitState(UNIT_STATE_ATTACK_PLAYER))
        return true;

    for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr)
        if ((*itr)->isAttackingPlayer())
            return true;

    for (uint8 i = 0; i < MAX_SUMMON_SLOT; ++i)
        if (m_SummonSlot[i])
            if (Creature* summon = GetMap()->GetCreature(m_SummonSlot[i]))
                if (summon->isAttackingPlayer())
                    return true;

    return false;
}

void Unit::RemoveAllAttackers()
{
    while (!m_attackers.empty())
    {
        AttackerSet::iterator iter = m_attackers.begin();
        if (!(*iter)->AttackStop())
            m_attackers.erase(iter);
    }
}

void Unit::ModifyAuraState(AuraStateType flag, bool apply)
{
    if (apply)
    {
        if (!HasFlag(UNIT_FIELD_AURA_STATE, 1<<(flag-1)))
        {
            SetFlag(UNIT_FIELD_AURA_STATE, 1<<(flag-1));
            if (IsPlayer())
            {
                PlayerSpellMap const& sp_list = ToPlayer()->GetSpellMap();
                for (PlayerSpellMap::const_iterator itr = sp_list.begin(); itr != sp_list.end(); ++itr)
                {
                    if (!itr->second)
                        continue;

                    if (itr->second->state == PLAYERSPELL_REMOVED || itr->second->disabled)
                        continue;

                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);

                    if (!spellInfo || !spellInfo->IsPassive())
                        continue;

                    if (spellInfo->CasterAuraState == uint32(flag))
                        CastSpell(this, itr->first, true, NULL);
                }
            }
            else if (Pet* pet = ToCreature()->ToPet())
            {
                for (PetSpellMap::const_iterator itr = pet->m_spells.begin(); itr != pet->m_spells.end(); ++itr)
                {
                    if (itr->second.state == PETSPELL_REMOVED)
                        continue;
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(itr->first);
                    if (!spellInfo || !spellInfo->IsPassive())
                        continue;
                    if (spellInfo->CasterAuraState == uint32(flag))
                        CastSpell(this, itr->first, true, NULL);
                }
            }
        }
    }
    else
    {
        if (HasFlag(UNIT_FIELD_AURA_STATE, 1<<(flag-1)))
        {
            RemoveFlag(UNIT_FIELD_AURA_STATE, 1<<(flag-1));

            if (flag)                  // enrage aura state triggering continues auras
            {
                Unit::AuraApplicationMap& tAuras = GetAppliedAuras();
                for (Unit::AuraApplicationMap::iterator itr = tAuras.begin(); itr != tAuras.end();)
                {
                    SpellInfo const* spellProto = (*itr).second->GetBase()->GetSpellInfo();
                    if (!spellProto)
                        continue;
                    if (spellProto->CasterAuraState == uint32(flag))
                        RemoveAura(itr);
                    else
                        ++itr;
                }
            }
        }
    }
}

uint32 Unit::BuildAuraStateUpdateForTarget(Unit* target) const
{
    uint32 auraStates = GetUInt32Value(UNIT_FIELD_AURA_STATE) &~(PER_CASTER_AURA_STATE_MASK);
    for (AuraStateAurasMap::const_iterator itr = m_auraStateAuras.begin(); itr != m_auraStateAuras.end(); ++itr)
        if ((1<<(itr->first-1)) & PER_CASTER_AURA_STATE_MASK)
            if (itr->second->GetBase()->GetCasterGUID() == target->GetGUID())
                auraStates |= (1<<(itr->first-1));

    return auraStates;
}

bool Unit::HasAuraState(AuraStateType flag, SpellInfo const* spellProto, Unit const* Caster) const
{
    if (Caster)
    {
        if (spellProto)
        {
            AuraEffectList const& stateAuras = Caster->GetAuraEffectsByType(SPELL_AURA_ABILITY_IGNORE_AURASTATE);
            for (AuraEffectList::const_iterator j = stateAuras.begin(); j != stateAuras.end(); ++j)
                if ((*j)->IsAffectingSpell(spellProto))
                    return true;
        }
        // Fix Brain Freeze (57761) - Frostfire Bolt (44614) act as if target has aurastate frozen
        if (spellProto && spellProto->Id == 44614 && Caster->HasAura(57761))
            return true;
        // Fix Fingers of Frost (44544) - Ice Lance (30455) and Deep Freeze (44572) act as if target has aurastate frozen
        if (spellProto && (spellProto->Id == 30455 || spellProto->Id == 44572) && Caster->HasAura(44544))
            return true;
        // Check per caster aura state
        // If aura with aurastate by caster not found return false
        if ((1<<(flag-1)) & PER_CASTER_AURA_STATE_MASK)
        {
            for (AuraStateAurasMap::const_iterator itr = m_auraStateAuras.lower_bound(flag); itr != m_auraStateAuras.upper_bound(flag); ++itr)
                if (itr->second->GetBase()->GetCasterGUID() == Caster->GetGUID())
                    return true;
            return false;
        }
    }

    return HasFlag(UNIT_FIELD_AURA_STATE, 1<<(flag-1));
}

void Unit::SetOwnerGUID(uint64 owner)
{
    if (GetOwnerGUID() == owner)
        return;

    SetGuidValue(UNIT_FIELD_SUMMONED_BY, owner);
    if (!owner)
        return;

    // Update owner dependent fields
    Player* player = ObjectAccessor::GetPlayer(*this, owner);
    if (!player || !player->HaveAtClient(this)) // if player cannot see this unit yet, he will receive needed data with create object
        return;

    SetFieldNotifyFlag(UF_FLAG_OWNER);

    UpdateData udata(GetMapId());
    WorldPacket packet;
    BuildValuesUpdateBlockForPlayer(&udata, player);
    if (udata.BuildPacket(&packet))
        player->SendDirectMessage(&packet);

    RemoveFieldNotifyFlag(UF_FLAG_OWNER);
}

Unit* Unit::GetOwner() const
{
    if (uint64 ownerid = GetOwnerGUID())
    {
        return ObjectAccessor::GetUnit(*this, ownerid);
    }
    return NULL;
}

Unit* Unit::GetOwnerOrUnitItself() const
{
    if (Unit* l_Owner = GetOwner())
        return l_Owner;

    return const_cast<Unit*>(this);
}

Unit* Unit::GetCharmer() const
{
    if (uint64 charmerid = GetCharmerGUID())
        return ObjectAccessor::GetUnit(*this, charmerid);
    return NULL;
}

Player* Unit::GetCharmerOrOwnerPlayerOrPlayerItself() const
{
    uint64 guid = GetCharmerOrOwnerGUID();
    if (IS_PLAYER_GUID(guid))
        return ObjectAccessor::GetPlayer(*this, guid);

    return IsPlayer() ? (Player*)this : NULL;
}

Player* Unit::GetAffectingPlayer() const
{
    if (!GetCharmerOrOwnerGUID())
        return IsPlayer() ? (Player*)this : NULL;

    if (Unit* owner = GetCharmerOrOwner())
        return owner->GetCharmerOrOwnerPlayerOrPlayerItself();
    return NULL;
}

Minion *Unit::GetFirstMinion() const
{
    if (uint64 pet_guid = GetMinionGUID())
    {
        if (Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, pet_guid))
            if (pet->HasUnitTypeMask(UNIT_MASK_MINION))
                return (Minion*)pet;

        const_cast<Unit*>(this)->SetMinionGUID(0);
    }

    return NULL;
}

Guardian* Unit::GetGuardianPet() const
{
    if (uint64 pet_guid = GetPetGUID())
    {
        if (Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*this, pet_guid))
            if (pet->HasUnitTypeMask(UNIT_MASK_GUARDIAN))
                return (Guardian*)pet;

        const_cast<Unit*>(this)->SetPetGUID(0);
    }

    return NULL;
}

Unit* Unit::GetCharm() const
{
    if (uint64 charm_guid = GetCharmGUID())
    {
        if (Unit* pet = ObjectAccessor::GetUnit(*this, charm_guid))
            return pet;

        if (Unit* creature = sObjectAccessor->FindCreature(charm_guid))
            return creature;

        const_cast<Unit*>(this)->SetGuidValue(UNIT_FIELD_CHARM, 0);
    }

    return NULL;
}

void Unit::SetMinion(Minion *minion, bool apply, PetSlot slot, bool stampeded)
{
    if (apply)
    {
        if (minion->GetOwnerGUID())
            return;

        minion->SetOwnerGUID(GetGUID());

        m_Controlled.insert(minion);

        if (IsPlayer())
        {
            minion->m_ControlledByPlayer = true;
            minion->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
        }

        // Can only have one pet. If a new one is summoned, dismiss the old one.
        if (minion->IsGuardianPet())
        {
            if (Guardian* oldPet = GetGuardianPet())
            {
                if (oldPet != minion && (oldPet->isPet() || minion->isPet() || oldPet->GetEntry() != minion->GetEntry()) && !stampeded)
                {
                    // remove existing minion pet
                    if (oldPet->isPet())
                        oldPet->ToPet()->Remove(PET_SLOT_ACTUAL_PET_SLOT, false, oldPet->ToPet()->m_Stampeded);
                    else
                        oldPet->UnSummon();
                    SetPetGUID(minion->GetGUID());
                    SetMinionGUID(0);
                }
            }
            else
            {
                SetPetGUID(minion->GetGUID());
                SetMinionGUID(0);
            }
        }

        if (slot == PET_SLOT_UNK_SLOT)
            slot = PET_SLOT_OTHER_PET;

        if (IsPlayer())
        {
            if (!minion->isHunterPet() && getClass() != CLASS_HUNTER) // If its not a hunter pet, well lets not try to use it for hunter then
                ToPlayer()->m_currentPetSlot = slot;

            if (slot >= PET_SLOT_HUNTER_FIRST && slot <= PET_SLOT_HUNTER_LAST && !stampeded) // Always save thoose spots where hunter is correct
            {
                ToPlayer()->m_currentPetSlot = slot;
                ToPlayer()->setPetSlotUsed(slot, true);
            }
        }

        if (minion->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
            AddGuidValue(UNIT_FIELD_SUMMON, minion->GetGUID());

        if (minion->m_Properties && minion->m_Properties->Type == SUMMON_TYPE_MINIPET)
            SetCritterGUID(minion->GetGUID());

        // PvP, FFAPvP
        minion->SetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1, GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1));

        // FIXME: hack, speed must be set only at follow
        /*if (IsPlayer() && minion->isPet())
        {
            for (uint8 l_I = 0; l_I < MAX_MOVE_TYPE; ++l_I)
                minion->SetSpeed(UnitMoveType(l_I), m_speed_rate[l_I], true);
        }*/

        // Ghoul pets and Warlock's pets have energy instead of mana (is anywhere better place for this code?)
        if (minion->IsPetGhoul() || (minion->GetOwner() && minion->GetOwner()->getClass() == CLASS_WARLOCK))
            minion->setPowerType(POWER_ENERGY);

        if (IsPlayer())
        {
            // Send infinity cooldown - client does that automatically but after relog cooldown needs to be set again
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(minion->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL));

            if (spellInfo && spellInfo->IsCooldownStartedOnEvent())
                ToPlayer()->AddSpellAndCategoryCooldowns(spellInfo, 0, NULL, true);
        }
    }
    else
    {
        if (minion->GetOwnerGUID() != GetGUID())
            return;

        m_Controlled.erase(minion);

        if (minion->m_Properties && minion->m_Properties->Type == SUMMON_TYPE_MINIPET)
        {
            if (GetCritterGUID() == minion->GetGUID())
                SetCritterGUID(0);
        }

        if (minion->IsGuardianPet())
        {
            if (GetPetGUID() == minion->GetGUID())
                SetPetGUID(0);
        }
        else if (minion->isTotem())
        {
            // All summoned by totem minions must disappear when it is removed.
            if (SpellInfo const* spInfo = sSpellMgr->GetSpellInfo(minion->ToTotem()->GetSpell()))
            {
                for (uint8 i = 0; i < spInfo->EffectCount; ++i)
                {
                    if (spInfo->Effects[i].Effect != SPELL_EFFECT_SUMMON)
                        continue;

                    RemoveAllMinionsByEntry(spInfo->Effects[i].MiscValue);
                }
            }

            if (minion->GetEntry() == 15439 && minion->GetOwner())
                RemoveAllMinionsByEntry(61029);
            else if (minion->GetEntry() == 15430 && minion->GetOwner())
                RemoveAllMinionsByEntry(61056);
            else if (minion->GetEntry() == 77934 && minion->GetOwner())
                RemoveAllMinionsByEntry(77942);
        }

        if (IsPlayer())
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(minion->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL));
            // Remove infinity cooldown
            if (spellInfo && spellInfo->IsCooldownStartedOnEvent())
                ToPlayer()->SendCooldownEvent(spellInfo);
        }

        //if (minion->HasUnitTypeMask(UNIT_MASK_GUARDIAN))
        {
            if (RemoveGuidValue(UNIT_FIELD_SUMMON, minion->GetGUID()))
            {
                // Check if there is another minion
                for (ControlList::iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr)
                {
                    // do not use this check, creature do not have charm guid
                    //if (GetCharmGUID() == (*itr)->GetGUID())
                    if (GetGUID() == (*itr)->GetCharmerGUID())
                        continue;

                    //ASSERT((*itr)->GetOwnerGUID() == GetGUID());
                    if ((*itr)->GetOwnerGUID() != GetGUID())
                    {
                        OutDebugInfo();
                        (*itr)->OutDebugInfo();
                        ASSERT(false);
                    }
                    ASSERT((*itr)->GetTypeId() == TYPEID_UNIT);

                    if (!(*itr)->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
                        continue;

                    if (AddGuidValue(UNIT_FIELD_SUMMON, (*itr)->GetGUID()))
                    {
                        // show another pet bar if there is no charm bar
                        if (IsPlayer() && !GetCharmGUID())
                        {
                            if ((*itr)->isPet())
                                ToPlayer()->PetSpellInitialize();
                            else
                                ToPlayer()->CharmSpellInitialize();
                        }
                    }
                    break;
                }
            }
        }
    }
}

void Unit::GetAllMinionsByEntry(std::list<Creature*>& Minions, uint32 entry)
{
    for (Unit::ControlList::iterator itr = m_Controlled.begin(); itr != m_Controlled.end();)
    {
        Unit* unit = *itr;
        ++itr;
        if (unit->GetEntry() == entry && unit->GetTypeId() == TYPEID_UNIT
            && unit->ToCreature()->isSummon()) // minion, actually
            Minions.push_back(unit->ToCreature());
    }
}

void Unit::RemoveAllMinionsByEntry(uint32 entry)
{
    for (Unit::ControlList::iterator itr = m_Controlled.begin(); itr != m_Controlled.end();)
    {
        Unit* unit = *itr;
        ++itr;
        if (unit->GetEntry() == entry && unit->GetTypeId() == TYPEID_UNIT
            && unit->ToCreature()->isSummon()) // minion, actually
            unit->ToTempSummon()->UnSummon();
        // i think this is safe because i have never heard that a despawned minion will trigger a same minion
    }
}

void Unit::SetCharm(Unit* charm, bool apply)
{
    if (apply)
    {
        if (IsPlayer())
        {
            if (!AddGuidValue(UNIT_FIELD_CHARM, charm->GetGUID()))
                sLog->outFatal(LOG_FILTER_UNITS, "Player %s is trying to charm unit %u, but it already has a charmed unit " UI64FMTD "", GetName(), charm->GetEntry(), GetCharmGUID());

            charm->m_ControlledByPlayer = true;
            // TODO: maybe we can use this flag to check if controlled by player
            charm->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
        }
        else
            charm->m_ControlledByPlayer = false;

        // PvP, FFAPvP
        charm->SetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1, GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1));

        if (!charm->AddGuidValue(UNIT_FIELD_CHARMED_BY, GetGUID()))
            sLog->outFatal(LOG_FILTER_UNITS, "Unit %u is being charmed, but it already has a charmer " UI64FMTD "", charm->GetEntry(), charm->GetCharmerGUID());

        _isWalkingBeforeCharm = charm->IsWalking();
        if (_isWalkingBeforeCharm)
            charm->SetWalk(false);

        m_Controlled.insert(charm);
    }
    else
    {
        if (IsPlayer())
        {
            if (!RemoveGuidValue(UNIT_FIELD_CHARM, charm->GetGUID()))
                sLog->outFatal(LOG_FILTER_UNITS, "Player %s is trying to uncharm unit %u, but it has another charmed unit " UI64FMTD "", GetName(), charm->GetEntry(), GetCharmGUID());
        }

        if (!charm->RemoveGuidValue(UNIT_FIELD_CHARMED_BY, GetGUID()))
            sLog->outFatal(LOG_FILTER_UNITS, "Unit %u is being uncharmed, but it has another charmer " UI64FMTD "", charm->GetEntry(), charm->GetCharmerGUID());

        if (charm->IsPlayer())
        {
            charm->m_ControlledByPlayer = true;
            charm->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            charm->ToPlayer()->UpdatePvPState();
        }
        else if (Player* player = charm->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            charm->m_ControlledByPlayer = true;
            charm->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            charm->SetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1, player->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1));
        }
        else
        {
            charm->m_ControlledByPlayer = false;
            charm->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            charm->SetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1, 0);
        }

        if (charm->IsWalking() != _isWalkingBeforeCharm)
            charm->SetWalk(_isWalkingBeforeCharm);

        if (charm->IsPlayer()
            || !charm->ToCreature()->HasUnitTypeMask(UNIT_MASK_MINION)
            || charm->GetOwnerGUID() != GetGUID())
            m_Controlled.erase(charm);
    }
}

int32 Unit::DealHeal(Unit* victim, uint32 addhealth, SpellInfo const* spellProto)
{
    int32 gain = 0;

    if (victim->IsAIEnabled)
        victim->GetAI()->HealReceived(this, addhealth);

    if (IsAIEnabled)
        GetAI()->HealDone(victim, addhealth);

    if (addhealth)
        gain = victim->ModifyHealth(int32(addhealth));

    Unit* unit = this;

    if (GetTypeId() == TYPEID_UNIT && (ToCreature()->isTotem() || ToCreature()->GetEntry() == 60849))
        unit = GetOwner();

    // Custom MoP Script
    // Purification (passive) - 16213 : Increase maximum health by 10% of the amount healed up to a maximum of 10% of health
    if (unit && unit->IsPlayer() && addhealth != 0 && unit->HasAura(16213))
    {
        int32 bp = 0;
        bp = int32(addhealth / 10);

        int32 availableBasepoints = 0;
        int32 max_amount = victim->CountPctFromMaxHealth(10);

        if (Aura* ancestralVigor = victim->GetAura(105284, unit->GetGUID()))
            if (ancestralVigor->GetEffect(EFFECT_0))
                availableBasepoints = ancestralVigor->GetEffect(EFFECT_0)->GetAmount();

        bp += availableBasepoints;
        bp = std::min(max_amount, bp);

        // Ancestral Vigor - 105284
        unit->CastCustomSpell(victim, 105284, &bp, NULL, NULL, true);
    }
    // 117907 - Mastery : Gift of the Serpent
    else if (unit && unit->IsPlayer() && unit->HasAura(117907) && unit->getLevel() >= 80 && addhealth > 0 && spellProto && spellProto->Id != 124041)
    {
        float Mastery = unit->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.22f;
        float scaling = spellProto->GetGiftOfTheSerpentScaling(this);

        if (roll_chance_f(Mastery * scaling) && unit->CountAreaTrigger(119031) < 10)
        {
            std::list<Unit*> targetList;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(unit, unit, 6.0f);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(unit, targetList, u_check);
            unit->VisitNearbyObject(6.0f, searcher);

            targetList.remove_if([this, unit](WorldObject* p_Object) -> bool
            {
                if (p_Object == nullptr || p_Object->ToUnit() == nullptr)
                    return true;

                if (!unit->IsValidAssistTarget(p_Object->ToUnit()))
                    return true;

                return false;
            });

            if (!targetList.empty())
            {
                targetList.sort(JadeCore::HealthPctOrderPred());

                for (auto itr : targetList)
                {
                    unit->CastSpell(itr, 119031, true);
                    break;
                }
            }
        }
    }

    if (Player* player = unit->ToPlayer())
    {
        if (Battleground* bg = player->GetBattleground())
            bg->UpdatePlayerScore(player, NULL, SCORE_HEALING_DONE, gain);

        // use the actual gain, as the overheal shall not be counted, skip gain 0 (it ignored anyway in to criteria)
        if (gain)
            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE, gain, 0, 0, victim);

        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED, addhealth);
    }

    if (Player* player = victim->ToPlayer())
    {
        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED, gain);
        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED, addhealth);
    }

    return gain;
}

Unit* Unit::GetMagicHitRedirectTarget(Unit* victim, SpellInfo const* spellInfo)
{
    // Patch 1.2 notes: Spell Reflection no longer reflects abilities
    if (spellInfo->Attributes & SPELL_ATTR0_ABILITY || spellInfo->AttributesEx & SPELL_ATTR1_CANT_BE_REDIRECTED || spellInfo->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        return victim;

    Unit::AuraEffectList const& magnetAuras = victim->GetAuraEffectsByType(SPELL_AURA_SPELL_MAGNET);
    for (Unit::AuraEffectList::const_iterator itr = magnetAuras.begin(); itr != magnetAuras.end(); ++itr)
    {
        if (Unit* magnet = (*itr)->GetBase()->GetCaster())
            if (spellInfo->CheckExplicitTarget(this, magnet) == SPELL_CAST_OK
                && (IsWithinLOSInMap(magnet)
                || magnet->isTotem()))
            {
                /// HackFix for Grounding Totem to not be able to redirect two spells in a row
                if ((*itr)->GetSpellInfo()->Id == 8178)
                    victim->RemoveAura(8178);
                return magnet;
            }
    }
    return victim;
}

Unit* Unit::GetMeleeHitRedirectTarget(Unit* victim, SpellInfo const* spellInfo)
{
    AuraEffectList const& hitTriggerAuras = victim->GetAuraEffectsByType(SPELL_AURA_ADD_CASTER_HIT_TRIGGER);
    for (AuraEffectList::const_iterator i = hitTriggerAuras.begin(); i != hitTriggerAuras.end(); ++i)
    {
        if (Unit* magnet = (*i)->GetBase()->GetCaster())
            if (_IsValidAttackTarget(magnet, spellInfo) && magnet->IsWithinLOSInMap(this)
                && (!spellInfo || (spellInfo->CheckExplicitTarget(this, magnet) == SPELL_CAST_OK
                && spellInfo->CheckTarget(this, magnet, false) == SPELL_CAST_OK)))
                if (roll_chance_i((*i)->GetAmount()))
                {
                    (*i)->GetBase()->DropCharge(AURA_REMOVE_BY_EXPIRE);
                    return magnet;
                }
    }
    return victim;
}

Unit* Unit::GetFirstControlled() const
{
    // Sequence: charmed, pet, other guardians
    Unit* unit = GetCharm();
    if (!unit)
        if (uint64 guid = GetMinionGUID())
            unit = ObjectAccessor::GetUnit(*this, guid);

    return unit;
}

void Unit::RemoveAllControlled()
{
    // possessed pet and vehicle
    if (IsPlayer())
        ToPlayer()->StopCastingCharm();

    while (!m_Controlled.empty())
    {
        Unit* target = *m_Controlled.begin();
        m_Controlled.erase(m_Controlled.begin());
        if (target->GetCharmerGUID() == GetGUID())
            target->RemoveCharmAuras();
        else if (target->GetOwnerGUID() == GetGUID() && target->isSummon())
            target->ToTempSummon()->UnSummon();
    }
}

Unit* Unit::GetNextRandomRaidMemberOrPet(float radius)
{
    Player* player = NULL;
    if (IsPlayer())
        player = ToPlayer();
    // Should we enable this also for charmed units?
    else if (GetTypeId() == TYPEID_UNIT && ToCreature()->isPet())
        player = GetOwner()->ToPlayer();

    if (!player)
        return NULL;
    Group* group = player->GetGroup();
    // When there is no group check pet presence
    if (!group)
    {
        // We are pet now, return owner
        if (player != this)
            return IsWithinDistInMap(player, radius) ? player : NULL;
        Unit* pet = GetGuardianPet();
        // No pet, no group, nothing to return
        if (!pet)
            return NULL;
        // We are owner now, return pet
        return IsWithinDistInMap(pet, radius) ? pet : NULL;
    }

    std::vector<Unit*> nearMembers;
    // reserve place for players and pets because resizing vector every unit push is unefficient (vector is reallocated then)
    nearMembers.reserve(group->GetMembersCount() * 2);

    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        if (Player* Target = itr->getSource())
        {
            // IsHostileTo check duel and controlled by enemy
            if (Target != this && Target->isAlive() && IsWithinDistInMap(Target, radius) && !IsHostileTo(Target))
                nearMembers.push_back(Target);

        // Push player's pet to vector
        if (Unit* pet = Target->GetGuardianPet())
            if (pet != this && pet->isAlive() && IsWithinDistInMap(pet, radius) && !IsHostileTo(pet))
                nearMembers.push_back(pet);
        }

    if (nearMembers.empty())
        return NULL;

    uint32 randTarget = urand(0, nearMembers.size()-1);
    return nearMembers[randTarget];
}

// only called in Player::SetSeer
// so move it to Player?
void Unit::AddPlayerToVision(Player* player)
{
    if (m_sharedVision.empty())
    {
        setActive(true);
        SetWorldObject(true);
    }
    m_sharedVision.push_back(player);
}

// only called in Player::SetSeer
void Unit::RemovePlayerFromVision(Player* player)
{
    m_sharedVision.remove(player);
    if (m_sharedVision.empty())
    {
        setActive(false);
        SetWorldObject(false);
    }
}

void Unit::RemoveBindSightAuras()
{
    RemoveAurasByType(SPELL_AURA_BIND_SIGHT);
}

void Unit::RemoveCharmAuras()
{
    RemoveAurasByType(SPELL_AURA_MOD_CHARM);
    RemoveAurasByType(SPELL_AURA_MOD_POSSESS_PET);
    RemoveAurasByType(SPELL_AURA_MOD_POSSESS);
    RemoveAurasByType(SPELL_AURA_AOE_CHARM);
}

void Unit::UnsummonAllTotems()
{
    for (uint8 i = 0; i < MAX_SUMMON_SLOT; ++i)
    {
        if (!m_SummonSlot[i])
            continue;

        if (Creature* OldTotem = GetMap()->GetCreature(m_SummonSlot[i]))
            if (OldTotem->isSummon())
                OldTotem->ToTempSummon()->UnSummon();
    }
}

void Unit::SendHealSpellLog(Unit* victim, uint32 SpellID, uint32 Damage, uint32 OverHeal, uint32 Absorb, bool critical /*= false*/, bool multistrike /*= false*/)
{
    // we guess size
    WorldPacket data(SMSG_SPELL_HEAL_LOG, 60);

    data.appendPackGUID(victim->GetGUID());
    data.appendPackGUID(GetGUID());
    data << uint32(SpellID);
    data << uint32(Damage);
    data << uint32(OverHeal);
    data << uint32(Absorb);

    data.WriteBit(critical);
    data.WriteBit(multistrike);                         // Multistrike
    data.WriteBit(false);                               // IsDebug
    data.WriteBit(false);                               // IsDebug 2
    data.WriteBit(false);                               // HasPowerData
    data.FlushBits();

    SendMessageToSet(&data, true);
}

int32 Unit::HealBySpell(Unit* victim, SpellInfo const* spellInfo, uint32 addHealth, bool critical /*= false*/, bool multistrike /*= false*/)
{
    // Prevent some bugs when player reveive heal when dead
    if (!victim->isAlive())
        return 0;

    uint32 absorb = 0;
    // calculate heal absorb and reduce healing
    CalcHealAbsorb(victim, spellInfo, addHealth, absorb);

    int32 gain = DealHeal(victim, addHealth, spellInfo);
    SendHealSpellLog(victim, spellInfo->Id, addHealth, uint32(addHealth - gain), absorb, critical, multistrike);
    return gain;
}

void Unit::SendEnergizeSpellLog(Unit* p_Victim, uint32 p_SpellID, uint32 p_Damage, Powers p_PowerType)
{
    WorldPacket l_Data(SMSG_SPELL_ENERGIZE_LOG, (3 * (16 + 2)) + 4 + 4 + 4 + 1);
    uint64 l_TargetGUID = p_Victim->GetGUID();
    uint64 l_CasterGUID = GetGUID();
    uint64 l_UnkGUID = 0;

    l_Data.appendPackGUID(l_TargetGUID);
    l_Data.appendPackGUID(l_CasterGUID);
    l_Data << uint32(p_SpellID);
    l_Data << uint32(p_PowerType);
    l_Data << uint32(p_Damage);

    l_Data.WriteBit(false);                       ///< HasPowerData
    l_Data.FlushBits();

    SendMessageToSet(&l_Data, true);
}

void Unit::EnergizeBySpell(Unit* victim, uint32 spellID, int32 damage, Powers powerType)
{
    SendEnergizeSpellLog(victim, spellID, damage, powerType);
    // needs to be called after sending spell log
    victim->ModifyPower(powerType, damage);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
    victim->getHostileRefManager().threatAssist(this, float(damage) * 0.5f, spellInfo);
}

uint32 Unit::SpellDamageBonusDone(Unit* victim, SpellInfo const *spellProto, uint32 pdamage, uint8 effIndex, DamageEffectType damagetype, uint32 stack /*= 1*/)
{
    if (!spellProto || !victim || damagetype == DIRECT_DAMAGE)
        return pdamage;

    // Some spells don't benefit from done mods
    if (spellProto->HasAttribute(SPELL_ATTR3_NO_DONE_BONUS) || spellProto->HasAttribute(SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS))
        return pdamage;

    // small exception for Crimson Tempest, can't find any general rule
    // should ignore ALL damage mods, they already calculated in trigger spellSPELL_ATTR3_NO_DONE_BONUS
    if (spellProto->Id == 122233 || spellProto->Id == 96172) // Crimson Tempest and Hand of Light
        return pdamage;

    // small exception for Improved Serpent Sting, can't find any general rule
    // should ignore ALL damage mods, they already calculated in trigger spell
    if (spellProto->Id == 83077 || spellProto->Id == 124051) // Improved Serpent Sting and Archimonde's Vengeance
        return pdamage;

    // small exception for Soul Link damage, can't find any general rule
    // should ignore ALL damage mods, they already calculated in trigger spell
    if (spellProto->Id == 108451)
        return pdamage;

    // small exception for Echo of Light, can't find any general rule
    // should ignore ALL damage mods, they already calculated in trigger spell
    if (spellProto->Id == 77489 || spellProto->Id == 12654) // Echo of Light and Ignite
        return pdamage;

    /// small exception for Prismatic Crystal, can't find any general rule
    /// should ignore ALL damage mods, they already calculated in trigger spell
    if (spellProto->Id == 155152)
        return pdamage;

    // For totems get damage bonus from owner
    if (GetTypeId() == TYPEID_UNIT && ToCreature()->isTotem())
        if (Unit* owner = GetOwner())
            return owner->SpellDamageBonusDone(victim, spellProto, pdamage, effIndex, damagetype);

    float ApCoeffMod = 1.0f;
    int32 DoneTotal = 0;

    if (Unit* owner = GetOwner())
    {
        AuraEffectList const& mModPetStats = owner->GetAuraEffectsByType(SPELL_AURA_MOD_PET_STATS);
        float amount = 0;
        for (AuraEffectList::const_iterator i = mModPetStats.begin(); i != mModPetStats.end(); ++i)
            if ((*i)->GetMiscValue() == INCREASE_MAGIC_DAMAGE_PERCENT)
                amount += float((*i)->GetAmount());

        DoneTotal += CalculatePct(pdamage, amount);
    }
    // 76613 - Mastery : Icicles
    if (spellProto && victim)
    {
        if (isPet())
        {
            Unit* owner = GetOwner();
            if (owner && owner->IsPlayer() && owner->HasAura(76613))
            {
                float Mastery = owner->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;
                DoneTotal += CalculatePct(pdamage, Mastery);
            }
        }
    }

    // Chaos Bolt - 116858, Chaos Bolt (Fire and Brimstone) - 157701 and Soul Fire - 6353
    // damage is increased by your critical strike chance
    if (IsPlayer() && spellProto && (spellProto->Id == 116858 || spellProto->Id == 157701 || spellProto->Id == 6353 || spellProto->Id == 104027))
    {
        /// Default is 5% of crit
        float crit_chance = 5.0f;
        crit_chance += GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + GetFirstSchoolInMask(spellProto->GetSchoolMask()));
        DoneTotal += CalculatePct(pdamage, crit_chance);
    }

    /// Fingers of Frost - 112965
    if (IsPlayer() && pdamage != 0 && ToPlayer()->GetSpecializationId() == SPEC_MAGE_FROST && spellProto && getLevel() >= 24)
    {
        bool l_HasFingerOfFrostProc = false;

        if (spellProto->Id == 116 || spellProto->Id == 44614 || spellProto->Id == 84721)    ///< Frostbolt || Frostfire Bolt
            l_HasFingerOfFrostProc = roll_chance_i(15);
        else if (spellProto->Id == 42208)                                                   ///< Blizzard
            l_HasFingerOfFrostProc = roll_chance_i(5);

        if (l_HasFingerOfFrostProc)
        {
            if (HasAura(44544))
                CastSpell(this, 126084, true); ///< Fingers of frost visual
            CastSpell(this, 44544, true);  ///< Fingers of frost proc
        }
    }

    /// Frost Orb should remove Polymorph
    if (IsPlayer() && spellProto && victim && spellProto->Id == 84721)
    {
        /// Polymorph
        if (victim->IsPolymorphed())
            victim->RemoveAurasDueToSpell(victim->getTransForm());
    }

    uint32 creatureTypeMask = victim->GetCreatureTypeMask(); ///> creatureTypeMask is unused

    // done scripted mod (take it from owner)
    Unit const* owner = GetOwner() ? GetOwner() : this;
    AuraEffectList const& mOverrideClassScript = owner->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
    for (AuraEffectList::const_iterator i = mOverrideClassScript.begin(); i != mOverrideClassScript.end(); ++i)
    {
        if (!(*i)->IsAffectingSpell(spellProto))
            continue;

        switch ((*i)->GetMiscValue())
        {
            case 4418: // Increased Shock Damage
            case 4554: // Increased Lightning Damage
            case 4555: // Improved Moonfire
            case 5142: // Increased Lightning Damage
            case 5147: // Improved Consecration / Libram of Resurgence
            case 5148: // Idol of the Shooting Star
            case 6008: // Increased Lightning Damage
            case 8627: // Totem of Hex
            {
                DoneTotal += (*i)->GetAmount();
                break;
            }
        }
    }



    // Custom scripted damage
    switch (spellProto->SpellFamilyName)
    {
        case SPELLFAMILY_DEATHKNIGHT:
            // Impurity (dummy effect)
            if (IsPlayer())
            {
                PlayerSpellMap playerSpells = ToPlayer()->GetSpellMap();
                for (PlayerSpellMap::const_iterator itr = playerSpells.begin(); itr != playerSpells.end(); ++itr)
                {
                    if (itr->second->state == PLAYERSPELL_REMOVED || itr->second->disabled)
                        continue;
                    switch (itr->first)
                    {
                        case 49220:
                        case 49633:
                        case 49635:
                        case 49636:
                        case 49638:
                            if (SpellInfo const* proto = sSpellMgr->GetSpellInfo(itr->first))
                                AddPct(ApCoeffMod, proto->Effects[0].CalcValue());
                            break;
                    }
                }
            }
            break;
    }

    // Check for table values
    float coeff = 0;

    SpellBonusEntry const* bonus = sSpellMgr->GetSpellBonusData(spellProto->Id);
    if (bonus && (spellProto->Effects[effIndex].BonusMultiplier == 0.0f && spellProto->Effects[effIndex].AttackPowerMultiplier == 0.0f))
    {
        if (damagetype == DOT)
        {
            coeff = bonus->dot_damage; ///< coeff is never read 01/18/16
            if (bonus->ap_dot_bonus > 0)
            {
                WeaponAttackType attType = (spellProto->IsRangedWeaponSpell() && spellProto->DmgClass != SPELL_DAMAGE_CLASS_MELEE && getClass() == CLASS_HUNTER) ? WeaponAttackType::RangedAttack : WeaponAttackType::BaseAttack;
                float APbonus = float(victim->GetTotalAuraModifier(attType == WeaponAttackType::BaseAttack ? SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS : SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS));
                APbonus += GetTotalAttackPowerValue(attType);
                DoneTotal += int32(bonus->ap_dot_bonus * stack * ApCoeffMod * APbonus);
            }
        }
        else
        {
            coeff = bonus->direct_damage; ///< coeff is never read 01/18/16
            if (bonus->ap_bonus > 0)
            {
                WeaponAttackType attType = (spellProto->IsRangedWeaponSpell() && spellProto->DmgClass != SPELL_DAMAGE_CLASS_MELEE && getClass() == CLASS_HUNTER) ? WeaponAttackType::RangedAttack : WeaponAttackType::BaseAttack;
                float APbonus = float(victim->GetTotalAuraModifier(attType == WeaponAttackType::BaseAttack ? SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS : SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS));
                APbonus += GetTotalAttackPowerValue(attType);
                DoneTotal += int32(bonus->ap_bonus * stack * ApCoeffMod * APbonus);
            }
        }
    }

    // Done Percentage for DOT is already calculated, no need to do it again. The percentage mod is applied in Aura::HandleAuraSpecificMods.
    float l_Multiplier = damagetype == DOT ? 1.0f : SpellDamagePctDone(victim, spellProto, damagetype);
    float tmpDamage = (int32(pdamage) + DoneTotal) * l_Multiplier;
    float tempTmpDamage = tmpDamage;

    // apply spellmod to Done damage (flat and pct)
    if (Player* modOwner = GetSpellModOwner())
        modOwner->ApplySpellMod(spellProto->Id, damagetype == DOT ? SPELLMOD_DOT : SPELLMOD_DAMAGE, tmpDamage);

    LOG_SPELL(this, spellProto->Id, "SpellDamageBonusDone(): Spell %s: ((%i + %i (DoneTotal)) * %f (SpellDamagePctDone)) = %f + %f (Mods) = %f", spellProto->GetNameForLogging().c_str(), pdamage, DoneTotal, l_Multiplier, tempTmpDamage, tmpDamage - tempTmpDamage, tmpDamage); ///< ISO C++11 does not allow conversion from string literal to 'char *'
    return uint32(std::max(tmpDamage, 0.0f));
}

float Unit::SpellDamagePctDone(Unit* victim, SpellInfo const* spellProto, DamageEffectType damagetype) const
{
    if (!spellProto || !victim || damagetype == DIRECT_DAMAGE)
        return 1.0f;

    // Some spells don't benefit from pct done mods
    if (spellProto->AttributesEx6 & SPELL_ATTR6_NO_DONE_PCT_DAMAGE_MODS)
        return 1.0f;

    // For totems pct done mods are calculated when its calculation is run on the player in SpellDamageBonusDone.
    if (GetTypeId() == TYPEID_UNIT && ToCreature()->isTotem())
        return 1.0f;

    // Done total percent damage auras
    float DoneTotalMod = 1.0f;

    // Pet damage?
    if (GetTypeId() == TYPEID_UNIT && !ToCreature()->isPet())
        DoneTotalMod *= ToCreature()->GetSpellDamageMod(ToCreature()->GetCreatureTemplate()->rank);

    /// Apply Versatility damage bonus done
    if (GetSpellModOwner())
        AddPct(DoneTotalMod, GetSpellModOwner()->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_DONE) + GetSpellModOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT));

    /// Some spells damages are modify on pvp
    if (GetSpellModOwner() && victim->GetSpellModOwner())
        AddPct(DoneTotalMod, GetDiminishingPVPDamage(spellProto));

    AuraEffectList const& mModDamagePercentDone = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
    for (AuraEffectList::const_iterator i = mModDamagePercentDone.begin(); i != mModDamagePercentDone.end(); ++i)
    {
        if (spellProto->EquippedItemClass == -1 && (*i)->GetSpellInfo()->EquippedItemClass != -1)    //prevent apply mods from weapon specific case to non weapon specific spells (Example: thunder clap and two-handed weapon specialization)
            continue;

        if ((*i)->GetMiscValue() & spellProto->GetSchoolMask())
        {
            if ((*i)->GetSpellInfo()->EquippedItemClass == -1)
                AddPct(DoneTotalMod, (*i)->GetAmount());
            else if (!((*i)->GetSpellInfo()->AttributesEx5 & SPELL_ATTR5_SPECIAL_ITEM_CLASS_CHECK) && ((*i)->GetSpellInfo()->EquippedItemSubClassMask == 0))
                AddPct(DoneTotalMod, (*i)->GetAmount());
            else if (IsPlayer() && ToPlayer()->HasItemFitToSpellRequirements((*i)->GetSpellInfo()))
                AddPct(DoneTotalMod, (*i)->GetAmount());
        }
    }

    /// Custom WoD Script - Mastery: Unshackled Fury
    if (GetTypeId() == TYPEID_PLAYER && getClass() == CLASS_WARRIOR && HasAura(76856) && HasAura(12880))
    {
        Player const* l_Player = ToPlayer();
        if (AuraEffect* l_MasteryUnshackledFury = l_Player->GetAuraEffect(76856, EFFECT_0))
        {
            float l_MasteryPct = l_Player->GetFloatValue(PLAYER_FIELD_MASTERY);
            float l_MasteryMultiplier = l_MasteryUnshackledFury->GetSpellEffectInfo()->BonusMultiplier;
            int32 l_MasteryValue = (int32)(l_MasteryMultiplier * l_MasteryPct);
            AddPct(DoneTotalMod, l_MasteryValue);
        }
    }

    AuraEffectList const& mModDamageFromPercentPower = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_DONE_FROM_PCT_POWER);
    for (AuraEffectList::const_iterator i = mModDamageFromPercentPower.begin(); i != mModDamageFromPercentPower.end(); ++i)
    {
        float l_Pct = (float(GetPower(getPowerType())) / float(GetMaxPower(getPowerType()))) * (*i)->GetAmount();
        AddPct(DoneTotalMod, l_Pct);
    }

    if ((isPet() || isGuardian()) && GetSpellModOwner())
    {
        AuraEffectList const& mModDamagePercentDone = GetSpellModOwner()->GetAuraEffectsByType(SPELL_AURA_MOD_PET_DAMAGE_DONE);
        for (AuraEffectList::const_iterator i = mModDamagePercentDone.begin(); i != mModDamagePercentDone.end(); ++i)
            AddPct(DoneTotalMod, (*i)->GetAmount());
    }

    uint32 creatureTypeMask = victim->GetCreatureTypeMask();

    AuraEffectList const& mDamageDoneVersus = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_DONE_VERSUS);
    for (AuraEffectList::const_iterator i = mDamageDoneVersus.begin(); i != mDamageDoneVersus.end(); ++i)
        if (creatureTypeMask & uint32((*i)->GetMiscValue()))
            AddPct(DoneTotalMod, (*i)->GetAmount());

    // bonus against aurastate
    AuraEffectList const& mDamageDoneVersusAurastate = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_DONE_VERSUS_AURASTATE);
    for (AuraEffectList::const_iterator i = mDamageDoneVersusAurastate.begin(); i != mDamageDoneVersusAurastate.end(); ++i)
        if (victim->HasAuraState(AuraStateType((*i)->GetMiscValue())))
            AddPct(DoneTotalMod, (*i)->GetAmount());

    // Add SPELL_AURA_MOD_DAMAGE_DONE_FOR_MECHANIC percent bonus
    AddPct(DoneTotalMod, GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_DAMAGE_DONE_FOR_MECHANIC, spellProto->Mechanic));

    // Custom scripted damage
    switch (spellProto->SpellFamilyName)
    {
        case SPELLFAMILY_MAGE:
            /// Ice Lance
            if (spellProto->SpellIconID == 186)
            {
                if (victim->HasAuraState(AURA_STATE_FROZEN, spellProto, this))
                    DoneTotalMod += 1.0f;

                /// Fingers of Frost
                if (Aura* l_Aura = GetAura(44544))
                {
                    SpellInfo const* l_FingersOfFrost = sSpellMgr->GetSpellInfo(44544);
                    if (l_FingersOfFrost)
                        AddPct(DoneTotalMod, l_FingersOfFrost->Effects[1].BasePoints);
                }
            }

            /// Torment the weak
            if (spellProto->GetSchoolMask() & SPELL_SCHOOL_MASK_ARCANE)
            {
                if (victim->HasAuraWithMechanic((1<<MECHANIC_SNARE)|(1<<MECHANIC_SLOW_ATTACK)))
                {
                    AuraEffectList const& mDumyAuras = GetAuraEffectsByType(SPELL_AURA_DUMMY);
                    for (AuraEffectList::const_iterator i = mDumyAuras.begin(); i != mDumyAuras.end(); ++i)
                    {
                        if ((*i)->GetSpellInfo()->SpellIconID == 2215)
                        {
                            AddPct(DoneTotalMod, (*i)->GetAmount());
                            break;
                        }
                    }
                }
            }
            break;
        case SPELLFAMILY_PRIEST:
            // Smite
            if (spellProto->SpellFamilyFlags[0] & 0x80)
            {
                // Glyph of Smite
                if (AuraEffect* aurEff = GetAuraEffect(55692, 0))
                    if (victim->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_PRIEST, 0x100000, 0, 0, GetGUID()))
                        AddPct(DoneTotalMod, aurEff->GetAmount());

            }
            break;
        case SPELLFAMILY_WARLOCK:
            // Fire and Brimstone
            if (spellProto->SpellFamilyFlags[1] & 0x00020040)
                if (victim->HasAuraState(AURA_STATE_CONFLAGRATE))
                {
                    AuraEffectList const& mDumyAuras = GetAuraEffectsByType(SPELL_AURA_DUMMY);
                    for (AuraEffectList::const_iterator i = mDumyAuras.begin(); i != mDumyAuras.end(); ++i)
                        if ((*i)->GetSpellInfo()->SpellIconID == 3173)
                        {
                            AddPct(DoneTotalMod, (*i)->GetAmount());
                            break;
                        }
                }
            // Shadow Bite (30% increase from each dot)
            if (spellProto->SpellFamilyFlags[1] & 0x00400000 && isPet())
                if (uint8 count = victim->GetDoTsByCaster(GetOwnerGUID()))
                    AddPct(DoneTotalMod, 30 * count);
            /// Mastery : Emberstorm - 77220
            /// Increases the damage of spells wich consume Burning Embers (Shadowburn and Chaos Bolt)
            if (IsPlayer() && HasAura(77220) && (spellProto->Id == 17877 || spellProto->Id == 116858))
            {
                float Mastery = GetFloatValue(PLAYER_FIELD_MASTERY) * 3.0f;
                AddPct(DoneTotalMod, Mastery);
            }
            break;
    }
    return DoneTotalMod;
}

uint32 Unit::SpellDamageBonusTaken(Unit* caster, SpellInfo const* spellProto, uint32 pdamage, DamageEffectType damagetype, uint32 /*stack*/)
{
    if (!spellProto || damagetype == DIRECT_DAMAGE)
        return pdamage;

    // Some spells don't benefit from done mods
    if (spellProto->HasAttribute(SPELL_ATTR3_NO_DONE_BONUS))
        return pdamage;

    // small exception for Stagger Amount, can't find any general rules
    // Light Stagger, Moderate Stagger and Heavy Stagger ignore reduction mods
    if (spellProto->Id == 124275 || spellProto->Id == 124274 || spellProto->Id == 124273)
        return pdamage;

    // small exception for Improved Serpent Sting, can't find any general rule
    // should ignore ALL damage mods, they already calculated in trigger spell
    if (spellProto->Id == 83077) // Improved Serpent Sting
        return pdamage;

    int32 TakenTotal = 0;
    float TakenTotalMod = 1.0f;
    float TakenTotalCasterMod = 0.0f;

    // get all auras from caster that allow the spell to ignore resistance
    AuraEffectList const& IgnoreResistAuras = caster->GetAuraEffectsByType(SPELL_AURA_MOD_IGNORE_TARGET_RESIST);
    for (AuraEffectList::const_iterator i = IgnoreResistAuras.begin(); i != IgnoreResistAuras.end(); ++i)
    {
        if ((*i)->GetMiscValue() & spellProto->GetSchoolMask())
            TakenTotalCasterMod += (float((*i)->GetAmount()));
    }

    // from positive and negative SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN
    // multiplicative bonus, for example Dispersion + Shadowform (0.10*0.85=0.085)
    TakenTotalMod *= GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, spellProto->GetSchoolMask());

    /// Apply Versatility damage bonus taken
    if (damagetype == DOT && GetSpellModOwner())
        pdamage -= CalculatePct(pdamage, GetSpellModOwner()->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_TAKEN) + GetSpellModOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT));

    // From caster spells
    AuraEffectList const& mOwnerTaken = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_FROM_CASTER);
    for (AuraEffectList::const_iterator i = mOwnerTaken.begin(); i != mOwnerTaken.end(); ++i)
    {
        if ((*i)->GetCasterGUID() == caster->GetGUID())
        {
            switch ((*i)->GetId())
            {
                // Vendetta, should affect to all damage
                case 79140:
                    TakenTotalMod += CalculatePct(1.0f, (*i)->GetAmount());
                    break;
                default:
                    if ((*i)->IsAffectingSpell(spellProto))
                        TakenTotalMod += CalculatePct(1.0f, (*i)->GetAmount());
                    break;
            }
        }
    }

    // Mod damage from spell mechanic
    if (uint32 mechanicMask = spellProto->GetAllEffectsMechanicMask())
    {
        AuraEffectList const& mDamageDoneMechanic = GetAuraEffectsByType(SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT);
        for (AuraEffectList::const_iterator i = mDamageDoneMechanic.begin(); i != mDamageDoneMechanic.end(); ++i)
            if (mechanicMask & uint32(1<<((*i)->GetMiscValue())))
                AddPct(TakenTotalMod, (*i)->GetAmount());
    }

    if (spellProto->IsTargetingArea() || spellProto->HasCustomAttribute(SpellCustomAttributes::SPELL_ATTR0_CU_IS_CUSTOM_AOE_SPELL))
    {
        if (IsPlayer())
            AddPct(TakenTotalMod, ToPlayer()->GetRatingBonusValue(CR_AVOIDANCE) / 100);

        int32 mult = GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_AOE_DAMAGE_AVOIDANCE, spellProto->SchoolMask);
        AddPct(TakenTotalMod, mult);

        if (caster->GetTypeId() != TYPEID_PLAYER)
        {
            int32 u_mult = GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE, spellProto->SchoolMask);
            AddPct(TakenTotalMod, u_mult);
        }
    }

    // Glyph of Inner Sanctum with Inner Fire
    if (HasAura(14771) && HasAura(588))
        AddPct(TakenTotalMod, -6.0f);

    /// Prey on the Weak
    if (AuraEffect* l_PreyOnTheWeak = caster->GetAuraEffect(131511, SpellEffIndex::EFFECT_0))
    {
        if (HasAura(408)  || ///< Kidney Shot
            HasAura(1833) || ///< Cheap Shot
            HasAura(1776) || ///< Gouge
            HasAura(6770) || ///< Sap
            HasAura(2094))   ///< Blind
            AddPct(TakenTotalMod, l_PreyOnTheWeak->GetAmount());
    }

    int32 TakenAdvertisedBenefit = SpellBaseDamageBonusTaken(spellProto->GetSchoolMask());

    // Check for table values
    float coeff = 0;
    SpellBonusEntry const* bonus = sSpellMgr->GetSpellBonusData(spellProto->Id);
    if (bonus)
        coeff = (damagetype == DOT) ? bonus->dot_damage : bonus->direct_damage;

    // Default calculation
    if (TakenAdvertisedBenefit)
    {
        //if (!bonus || coeff < 0)
            //coeff = CalculateDefaultCoefficient(spellProto, damagetype) * int32(stack);

        if (Player* modOwner = GetSpellModOwner())
        {
            coeff *= 100.0f;
            modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_BONUS_MULTIPLIER, coeff);
            coeff /= 100.0f;
        }
        TakenTotal += int32(TakenAdvertisedBenefit * coeff);
    }

    float tmpDamage = 0.0f;

    if (TakenTotalCasterMod)
    {
        if (TakenTotal < 0)
        {
            if (TakenTotalMod < 1)
                tmpDamage = ((float(CalculatePct(pdamage, TakenTotalCasterMod) + TakenTotal) * TakenTotalMod) + CalculatePct(pdamage, TakenTotalCasterMod));
            else
                tmpDamage = ((float(CalculatePct(pdamage, TakenTotalCasterMod) + TakenTotal) + CalculatePct(pdamage, TakenTotalCasterMod)) * TakenTotalMod);
        }
        else if (TakenTotalMod < 1)
            tmpDamage = ((CalculatePct(float(pdamage) + TakenTotal, TakenTotalCasterMod) * TakenTotalMod) + CalculatePct(float(pdamage) + TakenTotal, TakenTotalCasterMod));
    }
    if (!tmpDamage)
        tmpDamage = (float(pdamage) + TakenTotal) * TakenTotalMod;

    return uint32(std::max(tmpDamage, 0.0f));
}

int32 Unit::SpellBaseDamageBonusDone(SpellSchoolMask p_SchoolMask) const
{
    int32 l_DoneAdvertisedBenefit = 0;

    AuraEffectList const& l_DamageDone = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_DONE);
    for (AuraEffectList::const_iterator i = l_DamageDone.begin(); i != l_DamageDone.end(); ++i)
    {
        if (((*i)->GetMiscValue() & p_SchoolMask) != 0
            && (*i)->GetSpellInfo()->EquippedItemClass == -1               ///< -1 == any item class (not wand then)
            && (*i)->GetSpellInfo()->EquippedItemInventoryTypeMask == 0)   ///< 0 == any inventory type (not wand then)
            l_DoneAdvertisedBenefit += (*i)->GetAmount();
    }

    if (HasUnitTypeMask(UNIT_MASK_GUARDIAN))
    {
        if (Player* l_Owner = GetOwner()->ToPlayer())
            l_DoneAdvertisedBenefit += l_Owner->GetUInt32Value(PLAYER_FIELD_PET_SPELL_POWER);
    }

    if (IsPlayer())
    {
        // Base value
        l_DoneAdvertisedBenefit += ToPlayer()->GetBaseSpellPowerBonus();

        if (GetPowerIndex(POWER_MANA, getClass()) != MAX_POWERS)
            l_DoneAdvertisedBenefit += std::max(0, int32(GetStat(STAT_INTELLECT)));

        /// Spell power from SPELL_AURA_MOD_SPELL_POWER_PCT
        l_DoneAdvertisedBenefit *= GetTotalAuraMultiplier(SPELL_AURA_MOD_SPELL_POWER_PCT);

        // Damage bonus from stats
        AuraEffectList const& mDamageDoneOfStatPercent = GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_DAMAGE_OF_STAT_PERCENT);
        for (AuraEffectList::const_iterator i = mDamageDoneOfStatPercent.begin(); i != mDamageDoneOfStatPercent.end(); ++i)
        {
            if ((*i)->GetMiscValue() & p_SchoolMask)
            {
                // stat used stored in miscValueB for this aura
                Stats usedStat = Stats((*i)->GetMiscValueB());
                l_DoneAdvertisedBenefit += int32(CalculatePct(GetStat(usedStat), (*i)->GetAmount()));
            }
        }
        // ... and attack power
        AuraEffectList const& mDamageDonebyAP = GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_DAMAGE_OF_ATTACK_POWER);
        for (AuraEffectList::const_iterator i =mDamageDonebyAP.begin(); i != mDamageDonebyAP.end(); ++i)
            if ((*i)->GetMiscValue() & p_SchoolMask)
                l_DoneAdvertisedBenefit += int32(CalculatePct(GetTotalAttackPowerValue(WeaponAttackType::BaseAttack), (*i)->GetAmount()));

        AuraEffectList const& mOverrideSpellpower = GetAuraEffectsByType(SPELL_AURA_OVERRIDE_SPELL_POWER_BY_AP_PCT);
        for (AuraEffectList::const_iterator i = mOverrideSpellpower.begin(); i != mOverrideSpellpower.end(); ++i)
        {
            if (((*i)->GetMiscValue() & p_SchoolMask))
            {
                int32 attackPower = GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);
                l_DoneAdvertisedBenefit = (*i)->GetAmount() * attackPower / 100;
            }
        }
    }
    return l_DoneAdvertisedBenefit;
}

int32 Unit::SpellBaseDamageBonusTaken(SpellSchoolMask schoolMask) const
{
    int32 TakenAdvertisedBenefit = 0;

    AuraEffectList const& mDamageTaken = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_TAKEN);
    for (AuraEffectList::const_iterator i = mDamageTaken.begin(); i != mDamageTaken.end(); ++i)
        if (((*i)->GetMiscValue() & schoolMask) != 0)
            TakenAdvertisedBenefit += (*i)->GetAmount();

    return TakenAdvertisedBenefit;
}

bool Unit::IsSpellMultistrike() const
{
    if (GetSpellModOwner() == nullptr)
        return false;
    /// Pet can multistrike too with same chance as owner
    return roll_chance_f(GetSpellModOwner()->GetFloatValue(PLAYER_FIELD_MULTISTRIKE));
}

uint32 Unit::GetMultistrikeBasePoints(uint32 p_Damage) const
{
    if (GetSpellModOwner() == nullptr)
        return 0;

    return (p_Damage * GetSpellModOwner()->GetFloatValue(PLAYER_FIELD_MULTISTRIKE_EFFECT));
}

void Unit::ProcAuraMultistrike(SpellInfo const* p_ProcSpell, Unit* /*p_Target*/, int32& p_Amount)
{
    Player* l_ModOwner = GetSpellModOwner();

    if (IsSpellMultistrike() &&
        (p_ProcSpell->Id == 17 || p_ProcSpell->Id == 152118 || p_ProcSpell->Id == 114908 || p_ProcSpell->Id == 65148))
    {
        uint8 l_ProcTimes = ((l_ModOwner->GetMap() && l_ModOwner->GetMap()->IsBattlegroundOrArena()) || l_ModOwner->IsInPvPCombat()) ? 1 : 2;
        for (uint8 l_Idx = 0; l_Idx < l_ProcTimes; l_Idx++)
        {
            uint32 l_MultistrikeAbsorbAmount = GetMultistrikeBasePoints(p_Amount);

            if (IsAuraAbsorbCrit(p_ProcSpell, p_ProcSpell->GetSchoolMask()))
                l_MultistrikeAbsorbAmount = SpellCriticalAuraAbsorbBonus(p_ProcSpell, p_Amount);

            p_Amount += l_MultistrikeAbsorbAmount;
        }
    }
}

uint8 Unit::ProcTimesMultistrike(SpellInfo const* p_ProcSpell, Unit* /*p_Target*/)
{
    Player* l_ModOwner = GetSpellModOwner();

    /// ...the chance to activate up to two extra times (depending if PvE or PvP) at X% of normal effectiveness
    uint8 l_MaxProcTimes = ((l_ModOwner->GetMap() && l_ModOwner->GetMap()->IsBattlegroundOrArena()) || l_ModOwner->IsInPvPCombat()) ? 1 : 2;
    uint8 l_ProcTimes = 0;

    /// Hackfix for Blade Flurry
    if (p_ProcSpell && (p_ProcSpell->Id == 22482 || p_ProcSpell->Id == 12654))
        l_MaxProcTimes = 0;

    for (uint8 l_Idx = 0; l_Idx < l_MaxProcTimes; l_Idx++)
    {
        if (IsSpellMultistrike())
            l_ProcTimes++;
    }

    if (p_ProcSpell && p_ProcSpell->Id == 51505) ///< Lava Burst
    {
        float l_Crit_chance = (float)m_baseSpellCritChance;
        l_Crit_chance += GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, p_ProcSpell->SchoolMask);
        if (roll_chance_f(l_Crit_chance))
            l_ProcTimes++;
    }

    /// Item - Warlock T17 Destruction 4P Bonus
    /// Your next Chaos Bolt will multistrike 3 additional times.
    if (p_ProcSpell && (p_ProcSpell->Id == 116858 || p_ProcSpell->Id == 157701) && HasAura(170000))
    {
        RemoveAura(170000);
        l_ProcTimes += 3;
    }

    return l_ProcTimes;
}

uint8 Unit::ProcMultistrike(SpellInfo const* p_ProcSpell, Unit* p_Target, uint32 p_ProcFlag, uint32 p_ProcExtra, uint32 p_Damage, WeaponAttackType p_AttType /* = WeaponAttackType::BaseAttack*/ , SpellInfo const* p_ProcAura /*= NULL*/, AuraEffect const* p_OwnerAuraEffect /*= NULL*/)
{
    uint32 l_InitialDamage = p_Damage;
    Player* l_ModOwner = GetSpellModOwner();

    if (p_ProcExtra & PROC_EX_CRITICAL_HIT)
        l_InitialDamage = l_InitialDamage / ((float)SpellCriticalDamageBonus(p_ProcSpell, p_Damage, p_Target) / (float)p_Damage);

    uint8 l_ProcTimes = ProcTimesMultistrike(p_ProcSpell, p_Target);
    for (uint8 l_Idx = 0; l_Idx < l_ProcTimes; l_Idx++)
    {
        bool l_IsCrit = false;

        uint32 l_MultistrikeDamage = GetMultistrikeBasePoints(l_InitialDamage);

        if (p_ProcSpell && roll_chance_f(GetUnitSpellCriticalChance(p_Target, p_ProcSpell, p_ProcSpell->GetSchoolMask())))
            l_IsCrit = true;
        else if (!p_ProcSpell && roll_chance_f(GetUnitCriticalChance(p_AttType, p_Target)))
            l_IsCrit = true;

        if (l_IsCrit && p_ProcSpell)
            l_MultistrikeDamage = SpellCriticalDamageBonus(p_ProcSpell, l_MultistrikeDamage, p_Target);
        else if (l_IsCrit && !p_ProcSpell)
            l_MultistrikeDamage = MeleeCriticalDamageBonus(nullptr, l_MultistrikeDamage, p_Target, p_AttType);

        uint32 l_DoneProcFlag = p_ProcFlag & MULTISTRIKE_DONE_HIT_PROC_FLAG_MASK;
        uint32 l_TakenProcFlag = PROC_FLAG_TAKEN_DAMAGE;
        uint32 l_ExFlag = PROC_EX_INTERNAL_TRIGGERED | PROC_EX_INTERNAL_MULTISTRIKE;

        if (l_IsCrit)
            l_ExFlag |= PROC_EX_CRITICAL_HIT;
        else
            l_ExFlag |= PROC_EX_NORMAL_HIT;

        if (p_ProcFlag & PROC_FLAG_DONE_MELEE_AUTO_ATTACK)
            l_TakenProcFlag |= PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK;

        if (p_ProcFlag & PROC_FLAG_DONE_RANGED_AUTO_ATTACK)
            l_TakenProcFlag |= PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK;

        if (p_ProcFlag & PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS)
            l_TakenProcFlag |= PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS;

        if (p_ProcFlag & PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS)
            l_TakenProcFlag |= PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS;

        if (p_ProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS)
            l_TakenProcFlag |= PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS;

        if (p_ProcFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG)
            l_TakenProcFlag |= PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG;

        if (p_ProcFlag & PROC_FLAG_DONE_PERIODIC)
            l_TakenProcFlag |= PROC_FLAG_TAKEN_PERIODIC;

        if (p_ProcSpell && IsFriendlyTo(p_Target)) ///< Spell Heal
        {
            HealBySpell(p_Target, p_ProcSpell, l_MultistrikeDamage, l_IsCrit, true);
            ProcDamageAndSpell(p_Target, l_DoneProcFlag, l_TakenProcFlag, l_ExFlag, l_MultistrikeDamage, 0, p_AttType, p_ProcSpell);
        }
        else if (p_ProcSpell) ///< Spell Damage
        {
            if (p_Target->GetHealth() > p_Damage)
            {
                SpellNonMeleeDamage damageInfo(this, p_Target, p_ProcSpell->Id, p_ProcSpell->SchoolMask);

                if (l_IsCrit)
                    damageInfo.HitInfo |= SPELL_HIT_TYPE_CRIT;

                damageInfo.HitInfo |= SPELL_HIT_TYPE_MULTISTRIKE;
                damageInfo.damage = l_MultistrikeDamage;

                CalcAbsorbResist(p_Target, p_ProcSpell->GetSchoolMask(), p_ProcFlag & PROC_FLAG_DONE_PERIODIC ? DOT : SPELL_DIRECT_DAMAGE, damageInfo.damage, &damageInfo.absorb, &damageInfo.resist, p_ProcSpell);
                damageInfo.damage -= damageInfo.absorb + damageInfo.resist;
                DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);
                DealSpellDamage(&damageInfo, true);

                /// In case when we have SpellVisual for this spell loaded from DBC - multistrike should have spell animation too with increased speed
                if (p_ProcSpell->SpellVisual[0])
                    SendPlaySpellVisual(p_ProcSpell->SpellVisual[0], p_Target, (p_ProcSpell->Speed * 2.0f), 0.0f, Position());

                if (p_OwnerAuraEffect)
                {
                    int32 overkill = damageInfo.damage - p_Target->GetHealth() > 0 ? damageInfo.damage - p_Target->GetHealth() : 0;
                    SpellPeriodicAuraLogInfo pInfo(p_OwnerAuraEffect, damageInfo.damage, overkill, damageInfo.absorb, damageInfo.resist, 0.0f, l_IsCrit, true);
                    p_Target->SendPeriodicAuraLog(&pInfo);
                }
                else
                    SendSpellNonMeleeDamageLog(&damageInfo);

                if (p_Target->isDead())
                {
                    l_DoneProcFlag |= PROC_FLAG_KILL;
                    l_TakenProcFlag |= (PROC_FLAG_KILLED | PROC_FLAG_DEATH);
                }

                ProcDamageAndSpell(p_Target, l_DoneProcFlag, l_TakenProcFlag, l_ExFlag, damageInfo.damage, damageInfo.absorb, p_AttType, p_ProcSpell, p_ProcAura);
            }
        }
        else ///< Auto Attack
        {
            CalcDamageInfo damageInfo;
            CalculateMeleeDamage(p_Target, 0, &damageInfo, p_AttType);

            if (l_IsCrit)
                damageInfo.HitInfo |= HITINFO_CRITICALHIT;

            damageInfo.HitInfo |= HITINFO_MULTISTRIKE;
            damageInfo.damage = l_MultistrikeDamage;

            DealDamageMods(p_Target, damageInfo.damage, &damageInfo.absorb);
            DealMeleeDamage(&damageInfo, true);
            SendAttackStateUpdate(&damageInfo);
            ProcDamageAndSpell(damageInfo.target, l_DoneProcFlag, l_TakenProcFlag, l_ExFlag, damageInfo.damage, damageInfo.attackType);
        }
    }
    return l_ProcTimes;
}

bool Unit::IsSpellCrit(Unit* victim, SpellInfo const* spellProto, SpellSchoolMask schoolMask, WeaponAttackType attackType) const
{
    return roll_chance_f(GetUnitSpellCriticalChance(victim, spellProto, schoolMask, attackType));
}

bool Unit::IsAuraAbsorbCrit(SpellInfo const* p_SpellProto, SpellSchoolMask /*p_SchoolMask*/) const
{
    if (p_SpellProto->SpellFamilyName != SPELLFAMILY_PRIEST
        && p_SpellProto->Id != 65148) ///< Sacred Shield is able to crit
        return false;

    if ((p_SpellProto->AttributesEx2 & SPELL_ATTR2_CANT_CRIT))
        return false;

    float l_CritAbsorb = 0.0f;

    if (IsPlayer())
        l_CritAbsorb = GetFloatValue(PLAYER_FIELD_CRIT_PERCENTAGE);
    else
    {
        l_CritAbsorb = 5.0f;
        l_CritAbsorb += GetTotalAuraModifier(SPELL_AURA_MOD_WEAPON_CRIT_PERCENT);
        l_CritAbsorb += GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
    }

    if (l_CritAbsorb <= 0.0f)
        return false;

    return roll_chance_f(l_CritAbsorb);
}

bool Unit::IsUnitAbleToCrit() const
{
    uint32 l_Entry = GetEntry();
    switch (l_Entry)
    {
        case 15438:
        case 63508:
        case 27829:
        case 77936:
        case 55659:
        case 82927: ///< Inner Demon
        case 11859: ///< Summon Doomguard
        case 78158: ///< Grimoire Doomguard
            return true;
        default:
            return false;
    }
}

float Unit::GetUnitSpellCriticalChance(Unit* victim, SpellInfo const* spellProto, SpellSchoolMask schoolMask, WeaponAttackType attackType) const
{
    //! Mobs can't crit with spells. Player Totems can
    //! Fire Elemental (from totem) and Ebon Gargoyle can too - but this part is a hack and needs more research
    if (IS_CRE_OR_VEH_GUID(GetGUID()) && !(isTotem() && IS_PLAYER_GUID(GetOwnerGUID())) && !IsUnitAbleToCrit())
        return 0.0f;

    // not critting spell
    if ((spellProto->AttributesEx2 & SPELL_ATTR2_CANT_CRIT))
        return 0.0f;

    float crit_chance = 0.0f;
    switch (spellProto->DmgClass)
    {
        case SPELL_DAMAGE_CLASS_NONE:
            // We need more spells to find a general way (if there is any)
            switch (spellProto->Id)
            {
                case 53353: ///< Chimera Shot - Healing can crit, other spells - not
                case 34428: ///< Victory Rush
                case 6262:  ///< Healthstone
                    break;
                default:
                    if (spellProto->HasEffect(SPELL_EFFECT_HEAL_PCT))
                        return 0.0f;
                    break;
            }
        // Do not add a break here, case fallthrough is intentional! Adding a break will make above spells unable to crit.
        case SPELL_DAMAGE_CLASS_MAGIC:
        {
            /// For physical damage, but spells with SPELL_SCHOOL_MASK_ALL should select crit here
            if (schoolMask & SPELL_SCHOOL_MASK_NORMAL && schoolMask != SPELL_SCHOOL_MASK_ALL)
                crit_chance = 0.0f;
            // For other schools
            else if (IsPlayer())
            {
                /// Default is 5% of crit
                crit_chance = 5.0f;
                crit_chance += GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + GetFirstSchoolInMask(schoolMask));
            }
            else
            {
                crit_chance = (float)m_baseSpellCritChance;
                crit_chance += GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, schoolMask);
            }
            // taken
            if (victim)
            {
                if (!spellProto->IsPositive())
                {
                    // Modify critical chance by victim SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE
                    crit_chance += victim->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_ATTACKER_SPELL_CRIT_CHANCE, schoolMask);
                    // Modify critical chance by victim SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE
                    crit_chance += victim->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_SPELL_AND_WEAPON_CRIT_CHANCE);
                }
                // scripted (increase crit chance ... against ... target by x%
                AuraEffectList const& mOverrideClassScript = GetAuraEffectsByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
                for (AuraEffectList::const_iterator i = mOverrideClassScript.begin(); i != mOverrideClassScript.end(); ++i)
                {
                    if (!((*i)->IsAffectingSpell(spellProto)))
                        continue;

                    switch ((*i)->GetMiscValue())
                    {
                            // Shatter
                        case  911:
                            if (!victim->HasAuraState(AURA_STATE_FROZEN, spellProto, this))
                                break;
                            crit_chance *= 2; // double the critical chance against frozen targets
                            crit_chance += 50.0f; // plus an additional 50%
                            break;
                        default:
                            break;
                    }
                }
                /// Custom crit
                switch (spellProto->Id)
                {
                case 25912: ///< Holy Shock (damage)
                case 25914: ///< Holy Shock (heal)
                    crit_chance *= 2.0f;
                    break;
                }
                // Custom crit by class
                switch (spellProto->SpellFamilyName)
                {
                    case SPELLFAMILY_MAGE:
                    {
                        // Glyph of Fire Blast
                        if (spellProto->SpellFamilyFlags[0] == 0x2 && spellProto->SpellIconID == 12)
                        {
                            if (victim->HasAuraWithMechanic((1<<MECHANIC_STUN) | (1<<MECHANIC_KNOCKOUT)))
                                if (AuraEffect const* aurEff = GetAuraEffect(56369, EFFECT_0))
                                    crit_chance += aurEff->GetAmount();
                            break;
                        }
                        // Inferno Blast
                        if (spellProto->Id == 108853)
                        {
                            return 100.0f;
                            break;
                        }
                        // Critical Mass - 117216
                        // Multiplies the critical strike chance of your Fireball, Frostfire Bolt, Pyroblast, and Scorch by 1.30.
                        if (spellProto->Id == 133 || spellProto->Id == 44614 || spellProto->Id == 11366 || spellProto->Id == 2948)
                        {
                            if (HasAura(117216))
                                crit_chance *= 1.3f;
                            break;
                        }
                        break;
                    }
                    case SPELLFAMILY_DRUID:
                    {
                        switch (spellProto->Id)
                        {
                            case 8936:  // Regrowth ...
                                //  ... has a 60% increased chance for a critical effect.
                                crit_chance += 60.0f;

                                // Glyph of Regrowth
                                // Increases the critical strike chance of your Regrowth by 40%, but removes the periodic component of the spell.
                                if (HasAura(116218))
                                    return 100.0f;
                                break;
                        }
                        break;
                    }
                    case SPELLFAMILY_SHAMAN:
                    {
                        switch (spellProto->Id)
                        {
                            case 51505: // Lava Burst
                                return 100.0f;
                            default:
                                break;
                        }
                        break;
                    }
                    case SPELLFAMILY_WARLOCK:
                    {
                        switch (spellProto->Id)
                        {
                            case 6353:  ///< Soul Fire
                            case 104027:///< Soul Fire (Metamorphosis)
                            case 116858:///< Chaos Bolt ...
                            case 157701:///< Chaos Bolt (Fire and Brimstone)
                            case 31117: ///< Unstable Affliction dispell
                                ///< ... are always critical hit
                                return 100.0f;
                                break;
                                /// Hack fix for these spells - They deal Chaos damage, SPELL_SCHOOL_MASK_ALL
                            case 103964:///< Touch of Chaos
                            case 124915:///< Chaos Wave
                            case 6262:  ///< Healthstone
                                crit_chance += GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + SPELL_SCHOOL_MASK_NORMAL);
                                break;
                        }
                        break;
                    }
                }
            }
            break;
        }
        case SPELL_DAMAGE_CLASS_MELEE:
        {
            if (victim)
            {
                crit_chance += GetUnitCriticalChance(attackType, victim);
                crit_chance += GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, schoolMask);

                // Custom crit by class
                switch (spellProto->SpellFamilyName)
                {
                    case SPELLFAMILY_HUNTER:
                    {
                        switch (spellProto->Id)
                        {
                            case 16827: ///< Claw
                            case 17253: ///< Bite
                            case 49966: ///< Smack
                                /// Cobra Strikes - next spell will be critical
                                if (HasAura(53257) && !victim->HasAura(53480)) ///< Can't crit just if target has Roar of Sacrifice
                                    return 100.0f;
                                break;
                        }
                        break;
                    }
                    case SPELLFAMILY_DRUID:
                    {
                        switch (spellProto->Id)
                        {
                            case 5221:  ///< Shred
                                /// Incarnation: King of the Jungle increase crit of Shred
                                if (HasAura(102543))
                                    crit_chance *= 2.0f;
                                break;
                            case 22568: // Ferocious Bite
                                // Critical strike chance doubled against bleeding targets.
                                if (victim->HasAuraState(AURA_STATE_BLEEDING))
                                    crit_chance *= 2.0f;
                                break;
                        }
                        break;
                    }
                    case SPELLFAMILY_WARRIOR:
                    {
                        switch (spellProto->Id)
                        {
                            case 23881: // Bloodthirst ...
                                /// additional 30% chance to be a critical strike.
                                crit_chance += (float)spellProto->Effects[EFFECT_3].BasePoints;
                                break;
                            case 118000:// Dragon Roar ...
                                // ... is always a critical hit
                                return 100.0f;
                                break;
                        }
                        break;
                    }
                }
            }
            break;
        }
        case SPELL_DAMAGE_CLASS_RANGED:
        {
            if (victim)
            {
                crit_chance += GetUnitCriticalChance(attackType, victim);
                crit_chance += GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, schoolMask);

                // Ranged Spell (hunters)
                switch (spellProto->Id)
                {
                    case 19434: // Aimed Shot
                    case 82928: // Aimed Shot (Master Marksman)
                    case 56641: // Steady Shot
                    case 152245: // Focusing Shot
                    {
                        if (HasAura(34483)) // Careful Aim
                        {
                            ///< Increases the critical strike chance of your Steady Shot, Focusing Shot, and Aimed Shot
                            ///< by 50% on targets who are above 80% health...
                            ///< ... or while Rapid Fire is active.
                            if (victim->GetHealthPct() > 80.0f || HasAura(3045))
                                crit_chance += 50.0f;
                        }

                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        default:
            return 0.0f;
    }

    // Pets have 100% of owner's crit_chance
    if (isPet() && GetOwner())
    {
        if (GetOwner()->getClass() == CLASS_WARLOCK || GetOwner()->getClass() == CLASS_MAGE)
            crit_chance += GetOwner()->ToPlayer()->GetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + GetFirstSchoolInMask(schoolMask));
        else
        {
            crit_chance += GetOwner()->GetUnitCriticalChance(attackType, victim);
            crit_chance += GetOwner()->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, schoolMask);
        }
    }

    if (victim)
    {
        AuraEffectList const& critAuras = victim->GetAuraEffectsByType(SPELL_AURA_MOD_CRIT_CHANCE_FOR_CASTER);
        for (AuraEffectList::const_iterator i = critAuras.begin(); i != critAuras.end(); ++i)
            if ((*i)->GetCasterGUID() == GetGUID() && (*i)->IsAffectingSpell(spellProto))
                crit_chance += (*i)->GetAmount();

        // Mind Blast & Mind Spike debuff
        if (spellProto->Id == 8092)
            victim->RemoveAurasDueToSpell(87178, GetGUID());
    }

    // percent done
    // only players use intelligence for critical chance computations
    if (Player* modOwner = GetSpellModOwner())
        modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_CRITICAL_CHANCE, crit_chance);

    return crit_chance > 0.0f ? crit_chance : 0.0f;
}

uint32 Unit::MeleeCriticalDamageBonus(SpellInfo const* p_SpellProto, uint32 p_Damage, Unit* p_Victim, WeaponAttackType p_AttackType)
{
    int32 l_CritPct = 100; // 200% for all melee damage type...
    Player* l_ModOwner = GetSpellModOwner();

    if (p_Victim == nullptr)
        return  p_Damage;

    Player* l_ModVictimOwner = p_Victim->GetSpellModOwner();

    if (l_ModOwner != nullptr && l_ModVictimOwner != nullptr)
        l_CritPct = 50; ////< 150% on pvp

    if (p_AttackType == WeaponAttackType::RangedAttack)
        l_CritPct += p_Victim->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_RANGED_CRIT_DAMAGE);
    else
        l_CritPct += p_Victim->GetTotalAuraModifier(SPELL_AURA_MOD_ATTACKER_MELEE_CRIT_DAMAGE);

    if (p_SpellProto)
    {
        l_CritPct += CalculatePct(l_CritPct, GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_CRIT_DAMAGE_BONUS, p_SpellProto->GetSchoolMask()));
        /// adds additional damage to p_Damage (from talents)
        if (l_ModOwner)
            l_ModOwner->ApplySpellMod(p_SpellProto->Id, SPELLMOD_CRIT_DAMAGE_BONUS, p_Damage);
    }

    p_Damage += CalculatePct(p_Damage, l_CritPct);

    return p_Damage;
}

uint32 Unit::SpellCriticalDamageBonus(SpellInfo const* p_SpellProto, uint32 p_Damage, Unit* p_Victim)
{
    int32 l_CritPctBonus = 100; ///< 200% for all spell damage type...
    Player* l_ModOwner = GetSpellModOwner();

    if (p_Victim == nullptr)
        return  p_Damage;

    Player* l_ModVictimOwner = p_Victim->GetSpellModOwner();
    int32 l_Diff = 0;
    float l_PctSpellMod = 0.0f;

    if (l_ModOwner != nullptr && l_ModVictimOwner != nullptr)
        l_CritPctBonus = 50; ///< 150% on pvp

    /// Special case for Prismatic Crystal - 150% crit
    if (l_ModOwner != nullptr && l_ModOwner->getClass() == CLASS_MAGE && p_Victim->GetTypeId() == TYPEID_UNIT && p_Victim->HasAura(155153))
        l_CritPctBonus = 50;

    if (p_SpellProto)
    {
        l_CritPctBonus += CalculatePct(l_CritPctBonus, GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_CRIT_DAMAGE_BONUS, p_SpellProto->GetSchoolMask()));
        /// adds additional damage to p_Damage (from talents)
        int32 l_DamageTmp = p_Damage;

        if (l_ModOwner)
            l_Diff = l_ModOwner->ApplySpellMod(p_SpellProto->Id, SPELLMOD_CRIT_DAMAGE_BONUS, l_DamageTmp);
    }
    if (l_Diff > 0)
        l_PctSpellMod = 100.0f / ((float)p_Damage / (float)l_Diff);

    l_CritPctBonus += l_PctSpellMod;
    p_Damage += CalculatePct(p_Damage, l_CritPctBonus);

    return p_Damage;
}

uint32 Unit::SpellCriticalHealingBonus(SpellInfo const* /*p_SpellProto*/, uint32 p_Damage, Unit* p_Victim)
{
    int32 l_CritPctBonus = 100; ///< 200% for all healing...
    Player* l_ModOwner = GetSpellModOwner();

    if (p_Victim == nullptr)
        return  p_Damage;

    Player* l_ModVictimOwner = p_Victim->GetSpellModOwner();

    if (l_ModOwner != nullptr && l_ModVictimOwner != nullptr && ((l_ModOwner->GetMap() && l_ModOwner->GetMap()->IsBattlegroundOrArena()) || l_ModOwner->IsInPvPCombat() || l_ModVictimOwner->IsInPvPCombat()))
        l_CritPctBonus = 50; ///< 150% on pvp

    l_CritPctBonus += CalculatePct(l_CritPctBonus, GetTotalAuraModifier(SPELL_AURA_MOD_CRITICAL_HEALING_AMOUNT));

    p_Damage += CalculatePct(p_Damage, l_CritPctBonus);

    return p_Damage;
}

uint32 Unit::SpellCriticalAuraAbsorbBonus(SpellInfo const* /*p_SpellProto*/, uint32 p_Damage)
{
    int32 l_CritPctBonus = 100; ///< 200% for all absorb type...

    Player* l_ModOwner = GetSpellModOwner();

    if (l_ModOwner && ((GetMap() && GetMap()->IsBattlegroundOrArena()) || l_ModOwner->IsInPvPCombat()))
        l_CritPctBonus = 50; ///< 150% on pvp like healing

    ///< Maybe some bonus of Aura to apply?
    ///< l_CritPctBonus += CalculatePct(l_CritPctBonus, GetTotalAuraModifier(SPELL_AURA_MOD_CRITICAL_HEALING_AMOUNT));
    p_Damage += CalculatePct(p_Damage, l_CritPctBonus);

    return p_Damage;
}

uint32 Unit::SpellHealingBonusDone(Unit* victim, SpellInfo const *spellProto, uint32 healamount, uint8 effIndex, DamageEffectType damagetype, uint32 stack /*= 1*/)
{
    // For totems get healing bonus from owner (statue isn't totem in fact)
    if (GetTypeId() == TYPEID_UNIT && (isTotem() || (GetEntry() == 60849 && spellProto->Id == 115175)))
        if (Unit* owner = GetOwner())
            return owner->SpellHealingBonusDone(victim, spellProto, healamount, effIndex, damagetype, stack);

    // No bonus healing for potion spells
    if (spellProto->SpellFamilyName == SPELLFAMILY_POTION)
        return healamount;

    // No bonus for Temporal Ripples or Conductivity
    if (spellProto->Id == 115611 || spellProto->Id == 118800)
        return healamount;

    // No bonus for Devouring Plague heal or Atonement or Eminence
    if (spellProto->Id == 127626 || spellProto->Id == 81751 || spellProto->Id == 117895)
        return healamount;

    // No bonus for Leader of the Pack or Soul Leech or Soul Link heal
    if (spellProto->Id == 34299 || spellProto->Id == 108366 || spellProto->Id == 108447)
        return healamount;

    // No bonus for Living Seed or Ancestral Guidance
    if (spellProto->Id == 48503 || spellProto->Id == 114911)
        return healamount;

    // No bonus for Lifebloom : Final heal or Ysera's Gift or Leader of the Pack
    if (spellProto->Id == 33778 || spellProto->Id == 145109 || spellProto->Id == 68285)
        return healamount;

    // No bonus for Eminence (statue) and Eminence
    if (spellProto->Id == 117895 || spellProto->Id == 126890)
        return healamount;

    int32 DoneTotal = 0;

    // done scripted mod (take it from owner)
    Unit* owner = GetOwner() ? GetOwner() : this;
    AuraEffectList const& mOverrideClassScript= owner->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_CLASS_SCRIPTS);
    for (AuraEffectList::const_iterator i = mOverrideClassScript.begin(); i != mOverrideClassScript.end(); ++i)
    {
        if (!(*i)->IsAffectingSpell(spellProto))
            continue;

        switch ((*i)->GetMiscValue())
        {
            case 3736: // Hateful Totem of the Third Wind / Increased Lesser Healing Wave / LK Arena (4/5/6) Totem of the Third Wind / Savage Totem of the Third Wind
                DoneTotal += (*i)->GetAmount();
                break;
            default:
                break;
        }
    }

    // Apply Power PvP healing bonus
    if (healamount > 0 && IsPlayer() && (victim->IsPlayer() || (victim->GetTypeId() == TYPEID_UNIT && victim->isPet() && victim->GetOwner() && victim->GetOwner()->ToPlayer())))
    {
        float PvPPower = GetFloatValue(PLAYER_FIELD_PVP_POWER_HEALING);
        DoneTotal += CalculatePct(healamount, PvPPower);
    }

    /// Apply Versatility healing bonus done
    if (GetSpellModOwner())
        DoneTotal += CalculatePct(healamount, GetSpellModOwner()->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_DONE) + GetSpellModOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT));

    // Done fixed damage bonus auras
    int32 DoneAdvertisedBenefit = SpellBaseHealingBonusDone(spellProto->GetSchoolMask());

    if (!DoneAdvertisedBenefit || (SpellBaseHealingBonusDone(spellProto->GetSchoolMask()) < SpellBaseDamageBonusDone(spellProto->GetSchoolMask())))
    {
        DoneAdvertisedBenefit = SpellBaseDamageBonusDone(spellProto->GetSchoolMask());

        AuraEffectList const& mHealingDone = GetAuraEffectsByType(SPELL_AURA_MOD_HEALING_DONE);
        for (AuraEffectList::const_iterator i = mHealingDone.begin(); i != mHealingDone.end(); ++i)
            if (!(*i)->GetMiscValue() || ((*i)->GetMiscValue() & spellProto->GetSchoolMask()) != 0)
                DoneAdvertisedBenefit += (*i)->GetAmount();
    }

    // Check for table values
    float coeff = 0.0f;
    SpellBonusEntry const* bonus = sSpellMgr->GetSpellBonusData(spellProto->Id);
    if (bonus && (spellProto->Effects[effIndex].BonusMultiplier == 0.0f && spellProto->Effects[effIndex].AttackPowerMultiplier == 0.0f))
    {
        if (damagetype == DOT)
        {
            coeff = bonus->dot_damage; ///< coeff is never read 01/18/16
            if (bonus->ap_dot_bonus > 0)
                DoneTotal += int32(bonus->ap_dot_bonus * stack * GetTotalAttackPowerValue(
                    (spellProto->IsRangedWeaponSpell() && spellProto->DmgClass !=SPELL_DAMAGE_CLASS_MELEE) ? WeaponAttackType::RangedAttack : WeaponAttackType::BaseAttack));
        }
        else
        {
            coeff = bonus->direct_damage; ///< coef is never read 01/18/16
            if (bonus->ap_bonus > 0)
                DoneTotal += int32(bonus->ap_bonus * stack * GetTotalAttackPowerValue(WeaponAttackType::BaseAttack));
        }
    }
    else
    {
        // No bonus healing for SPELL_DAMAGE_CLASS_NONE class spells by default
        if (spellProto->DmgClass == SPELL_DAMAGE_CLASS_NONE)
            return healamount;
    }

    for (uint8 i = 0; i < spellProto->EffectCount; ++i)
    {
        switch (spellProto->Effects[i].ApplyAuraName)
        {
            // Bonus healing does not apply to these spells
            case SPELL_AURA_PERIODIC_LEECH:
            case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                DoneTotal = 0;
                break;
        }
        if (spellProto->Effects[i].Effect == SPELL_EFFECT_HEALTH_LEECH)
            DoneTotal = 0;
    }

    // Done Percentage for DOT is already calculated, no need to do it again. The percentage mod is applied in Aura::HandleAuraSpecificMods.
    float heal = float(int32(healamount) + DoneTotal) * (damagetype == DOT ? 1.0f : SpellHealingPctDone(victim, spellProto));

    // apply spellmod to Done amount
    if (Player* modOwner = GetSpellModOwner())
        modOwner->ApplySpellMod(spellProto->Id, damagetype == DOT ? SPELLMOD_DOT : SPELLMOD_DAMAGE, heal);

    return uint32(std::max(heal, 0.0f));
}

float Unit::SpellHealingPctDone(Unit* victim, SpellInfo const* spellProto) const
{
    // For totems pct done mods are calculated when its calculation is run on the player in SpellHealingBonusDone.
    if (GetTypeId() == TYPEID_UNIT && isTotem())
        if (GetOwner())
            return 1.0f;

    // No bonus healing for potion spells
    if (spellProto->SpellFamilyName == SPELLFAMILY_POTION)
        return 1.0f;

    /// No bonus for Ember Tap heal
    if (spellProto->Id == 114635)
        return 1.0f;

    // No bonus for Eminence (statue) and Eminence
    if (spellProto->Id == 117895 || spellProto->Id == 126890)
        return 1.0f;

    float DoneTotalMod = 1.0f;

    // Healing done percent
    AuraEffectList const& mHealingDonePct = GetAuraEffectsByType(SPELL_AURA_MOD_HEALING_DONE_PERCENT);
    for (AuraEffectList::const_iterator i = mHealingDonePct.begin(); i != mHealingDonePct.end(); ++i)
    {
        if (!((*i)->GetBase()->GetSpellInfo()->Id == 158298 && victim->GetGUID() != GetGUID())) ///< Resolve bonus healing only done to yourself
            AddPct(DoneTotalMod, (*i)->GetAmount());/// += CalculatePct(1.0f, (*i)->GetAmount());
    }

    AuraEffectList const& mHealingDoneFromHealth = GetAuraEffectsByType(SPELL_AURA_MOD_HEALING_DONE_FROM_PCT_HEALTH);
    for (AuraEffectList::const_iterator i = mHealingDoneFromHealth.begin(); i != mHealingDoneFromHealth.end(); ++i)
    {
        float l_Bonus = CalculatePct((100.0f - victim->GetHealthPct()), (*i)->GetAmount());
        DoneTotalMod += (l_Bonus / 100);
    }

    /// 6.2 : All healing and damage absorption has been reduced by 15% in PvP combat.
    if (Player* l_ModOwner = GetSpellModOwner())
    {
        if ((l_ModOwner->GetMap() && l_ModOwner->GetMap()->IsBattlegroundOrArena()) || l_ModOwner->IsInPvPCombat())
            AddPct(DoneTotalMod, -15.0f);
    }

    return DoneTotalMod;
}

uint32 Unit::SpellHealingBonusTaken(Unit* caster, SpellInfo const* spellProto, uint32 healamount, DamageEffectType damagetype, uint32 /*stack*/)
{
    float TakenTotalMod = 1.0f;

    /// Dampening, must be calculated off the raw amount
    Unit* l_OwnerOrSelf = GetOwnerOrUnitItself();

    if (AuraEffect* l_AurEff = l_OwnerOrSelf->GetAuraEffect(110310, EFFECT_0))
        healamount = CalculatePct(healamount, 100 - l_AurEff->GetAmount());

    // No bonus for Eminence (statue) and Eminence
    if (spellProto->Id == 117895 || spellProto->Id == 126890)
        return healamount;

    // No bonus for Living Seed
    if (spellProto->Id == 48503)
        return healamount;

    // No bonus for Lifebloom : Final heal
    if (spellProto->Id == 33778)
        return healamount;

    // No bonus for Devouring Plague heal
    if (spellProto->Id == 127626)
        return healamount;

    for (AuraEffect* l_Effect : GetAuraEffectsByType(SPELL_AURA_MOD_HEALING_PCT))
    {
        if (l_Effect->GetSpellInfo()->Id == 73651 && (spellProto->Id == 73651 || !HasAura(146625)))
            continue;

        AddPct(TakenTotalMod, l_Effect->GetAmount());
    }

    // Tenacity increase healing % taken
    if (AuraEffect const* Tenacity = GetAuraEffect(58549, 0))
        AddPct(TakenTotalMod, Tenacity->GetAmount());

    // Healing Done
    int32 TakenTotal = 0;

    // Taken fixed damage bonus auras
    int32 TakenAdvertisedBenefit = SpellBaseHealingBonusTaken(spellProto->GetSchoolMask());

    // Check for table values
    SpellBonusEntry const* bonus = sSpellMgr->GetSpellBonusData(spellProto->Id);
    float coeff = 0;
    if (bonus)
        coeff = (damagetype == DOT) ? bonus->dot_damage : bonus->direct_damage;
    else
    {
        // No bonus healing for SPELL_DAMAGE_CLASS_NONE class spells by default
        if (spellProto->DmgClass == SPELL_DAMAGE_CLASS_NONE)
        {
            healamount = uint32(std::max((int32(healamount) + TakenTotal) * TakenTotalMod, 0.0f));
            return healamount;
        }
    }

    // Default calculation
    if (TakenAdvertisedBenefit)
    {
        //if (!bonus || coeff < 0)
        //    coeff = CalculateDefaultCoefficient(spellProto, damagetype) * int32(stack) * 1.88f;  // As wowwiki says: C = (Cast Time / 3.5) * 1.88 (for healing spells)

        if (Player* modOwner = GetSpellModOwner())
        {
            coeff *= 100.0f;
            modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_BONUS_MULTIPLIER, coeff);
            coeff /= 100.0f;
        }

        TakenTotal += int32(TakenAdvertisedBenefit * coeff);
    }

    AuraEffectList const& mHealingGet= GetAuraEffectsByType(SPELL_AURA_MOD_HEALING_RECEIVED);
    for (AuraEffectList::const_iterator i = mHealingGet.begin(); i != mHealingGet.end(); ++i)
    {
        if (caster->GetGUID() == (*i)->GetCasterGUID() && (*i)->IsAffectingSpell(spellProto))
            AddPct(TakenTotalMod, (*i)->GetAmount());
        else if ((*i)->GetBase()->GetId() == 974) // Hack fix for Earth Shield
            AddPct(TakenTotalMod, (*i)->GetAmount());
    }

    AuraEffectList const& mHotPct = GetAuraEffectsByType(SPELL_AURA_MOD_HOT_PCT);
    for (AuraEffectList::const_iterator i = mHotPct.begin(); i != mHotPct.end(); ++i)
        if (damagetype == DOT)
            AddPct(TakenTotalMod, (*i)->GetAmount());

    for (uint8 i = 0; i < spellProto->EffectCount; ++i)
    {
        switch (spellProto->Effects[i].ApplyAuraName)
        {
            // Bonus healing does not apply to these spells
            case SPELL_AURA_PERIODIC_LEECH:
            case SPELL_AURA_PERIODIC_HEALTH_FUNNEL:
                TakenTotal = 0;
                break;
        }
        if (spellProto->Effects[i].Effect == SPELL_EFFECT_HEALTH_LEECH)
            TakenTotal = 0;
    }

    float heal = float(int32(healamount) + TakenTotal) * TakenTotalMod;
    return uint32(std::max(heal, 0.0f));
}

int32 Unit::SpellBaseHealingBonusDone(SpellSchoolMask schoolMask)
{
    int32 AdvertisedBenefit = 0;

    AuraEffectList const& mHealingDone = GetAuraEffectsByType(SPELL_AURA_MOD_HEALING_DONE);
    for (AuraEffectList::const_iterator i = mHealingDone.begin(); i != mHealingDone.end(); ++i)
        if (!(*i)->GetMiscValue() || ((*i)->GetMiscValue() & schoolMask) != 0)
            AdvertisedBenefit += (*i)->GetAmount();

    // Healing bonus of spirit, intellect and strength
    if (IsPlayer())
    {
        if (GetPowerIndex(POWER_MANA, getClass()) != MAX_POWERS)
            AdvertisedBenefit += std::max(0, int32(GetStat(STAT_INTELLECT)));

        // Base value
        AdvertisedBenefit += ToPlayer()->GetBaseSpellPowerBonus();

        // Healing bonus from stats
        AuraEffectList const& mHealingDoneOfStatPercent = GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_HEALING_OF_STAT_PERCENT);
        for (AuraEffectList::const_iterator i = mHealingDoneOfStatPercent.begin(); i != mHealingDoneOfStatPercent.end(); ++i)
        {
            // stat used dependent from misc value (stat index)
            Stats usedStat = Stats((*i)->GetSpellInfo()->Effects[(*i)->GetEffIndex()].MiscValue);
            AdvertisedBenefit += int32(CalculatePct(GetStat(usedStat), (*i)->GetAmount()));
        }

        // ... and attack power
        AuraEffectList const& mHealingDonebyAP = GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_HEALING_OF_ATTACK_POWER);
        for (AuraEffectList::const_iterator i = mHealingDonebyAP.begin(); i != mHealingDonebyAP.end(); ++i)
            if ((*i)->GetMiscValue() & schoolMask)
                AdvertisedBenefit += int32(CalculatePct(GetTotalAttackPowerValue(WeaponAttackType::BaseAttack), (*i)->GetAmount()));

        /// Apply Versatility healing bonus
        if (GetSpellModOwner())
            AdvertisedBenefit += CalculatePct(AdvertisedBenefit, GetSpellModOwner()->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_DONE) + GetSpellModOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT));
    }

    return AdvertisedBenefit;
}

int32 Unit::SpellBaseHealingBonusTaken(SpellSchoolMask schoolMask)
{
    int32 AdvertisedBenefit = 0;

    AuraEffectList const& mDamageTaken = GetAuraEffectsByType(SPELL_AURA_MOD_HEALING);
    for (AuraEffectList::const_iterator i = mDamageTaken.begin(); i != mDamageTaken.end(); ++i)
        if (((*i)->GetMiscValue() & schoolMask) != 0)
            AdvertisedBenefit += (*i)->GetAmount();

    return AdvertisedBenefit;
}

bool Unit::IsImmunedToDamage(SpellSchoolMask schoolMask)
{
    // If m_immuneToSchool type contain this school type, IMMUNE damage.
    SpellImmuneList const& schoolList = m_spellImmune[IMMUNITY_SCHOOL];
    for (SpellImmuneList::const_iterator itr = schoolList.begin(); itr != schoolList.end(); ++itr)
        if (itr->type & schoolMask)
            return true;

    // If m_immuneToDamage type contain magic, IMMUNE damage.
    SpellImmuneList const& damageList = m_spellImmune[IMMUNITY_DAMAGE];
    for (SpellImmuneList::const_iterator itr = damageList.begin(); itr != damageList.end(); ++itr)
        if (itr->type & schoolMask)
            return true;

    return false;
}

bool Unit::IsImmunedToDamage(SpellInfo const* spellInfo)
{
    if (spellInfo->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        return false;

    uint32 schoolMask = spellInfo->GetSchoolMask();
    if (spellInfo->IsNeedToCheckSchoolImmune())
    {
        // If m_immuneToSchool type contain this school type, IMMUNE damage.
        SpellImmuneList const& schoolList = m_spellImmune[IMMUNITY_SCHOOL];
        for (SpellImmuneList::const_iterator itr = schoolList.begin(); itr != schoolList.end(); ++itr)
            if (itr->type & schoolMask && !spellInfo->CanPierceImmuneAura(sSpellMgr->GetSpellInfo(itr->spellId)))
            {
                if (spellInfo->GetSchoolMask() == SPELL_SCHOOL_MASK_ALL && itr->type != SPELL_SCHOOL_MASK_ALL)
                    continue;
                return true;
            }
    }

    // If m_immuneToDamage type contain magic, IMMUNE damage.
    SpellImmuneList const& damageList = m_spellImmune[IMMUNITY_DAMAGE];
    for (SpellImmuneList::const_iterator itr = damageList.begin(); itr != damageList.end(); ++itr)
        if (itr->type & schoolMask)
            return true;

    return false;
}

bool Unit::IsImmunedToSpell(SpellInfo const* spellInfo)
{
    if (!spellInfo)
        return false;

    // Single spell immunity.
    SpellImmuneList const& idList = m_spellImmune[IMMUNITY_ID];
    for (SpellImmuneList::const_iterator itr = idList.begin(); itr != idList.end(); ++itr)
        if (itr->type == spellInfo->Id)
            return true;

    if (spellInfo->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
        return false;

    if (spellInfo->Dispel)
    {
        SpellImmuneList const& dispelList = m_spellImmune[IMMUNITY_DISPEL];
        for (SpellImmuneList::const_iterator itr = dispelList.begin(); itr != dispelList.end(); ++itr)
            if (itr->type == spellInfo->Dispel)
                return true;
    }

    // Spells that don't have effectMechanics.
    if (spellInfo->Mechanic)
    {
        SpellImmuneList const& mechanicList = m_spellImmune[IMMUNITY_MECHANIC];
        for (SpellImmuneList::const_iterator itr = mechanicList.begin(); itr != mechanicList.end(); ++itr)
            if (itr->type == spellInfo->Mechanic)
                return true;
    }

    bool immuneToAllEffects = true;
    for (uint8 i = 0; i < spellInfo->EffectCount; ++i)
    {
        // State/effect immunities applied by aura expect full spell immunity
        // Ignore effects with mechanic, they are supposed to be checked separately
        if (!spellInfo->Effects[i].IsEffect())
            continue;
        if (!IsImmunedToSpellEffect(spellInfo, i))
        {
            immuneToAllEffects = false;
            break;
        }
    }

    if (immuneToAllEffects) //Return immune only if the target is immune to all spell effects.
        return true;

    if (spellInfo->IsNeedToCheckSchoolImmune())
    {
        SpellImmuneList const& schoolList = m_spellImmune[IMMUNITY_SCHOOL];
        for (SpellImmuneList::const_iterator itr = schoolList.begin(); itr != schoolList.end(); ++itr)
        {
            SpellInfo const* immuneSpellInfo = sSpellMgr->GetSpellInfo(itr->spellId);
            if ((itr->type & spellInfo->GetSchoolMask())
                && !(immuneSpellInfo && immuneSpellInfo->IsPositive() && spellInfo->IsPositive())
                && !spellInfo->CanPierceImmuneAura(immuneSpellInfo))
            {
                if (spellInfo->GetSchoolMask() == SPELL_SCHOOL_MASK_ALL && itr->type != SPELL_SCHOOL_MASK_ALL)
                    continue;
                return true;
            }
        }
    }

    return false;
}

uint32 Unit::GetSchoolImmunityMask() const
{
    uint32 l_Mask = 0;
    SpellImmuneList const& l_MechanicList = m_spellImmune[IMMUNITY_SCHOOL];
    for (SpellImmuneList::const_iterator l_Itr = l_MechanicList.begin(); l_Itr != l_MechanicList.end(); ++l_Itr)
        l_Mask |= l_Itr->type;

    return l_Mask;
}

uint32 Unit::GetMechanicImmunityMask() const
{
    uint32 l_Mask = 0;
    SpellImmuneList const& l_MechanicList = m_spellImmune[IMMUNITY_MECHANIC];
    for (SpellImmuneList::const_iterator l_Itr = l_MechanicList.begin(); l_Itr != l_MechanicList.end(); ++l_Itr)
        l_Mask |= (1 << l_Itr->type);

    return l_Mask;
}

bool Unit::IsImmunedToSpellEffect(SpellInfo const* spellInfo, uint32 index) const
{
    if (!spellInfo || !spellInfo->Effects[index].IsEffect())
        return false;

    // If m_immuneToEffect type contain this effect type, IMMUNE effect.
    uint32 effect = spellInfo->Effects[index].Effect;
    SpellImmuneList const& effectList = m_spellImmune[IMMUNITY_EFFECT];
    for (SpellImmuneList::const_iterator itr = effectList.begin(); itr != effectList.end(); ++itr)
        if (itr->type == effect)
            return true;

    if (uint32 mechanic = spellInfo->Effects[index].Mechanic)
    {
        SpellImmuneList const& mechanicList = m_spellImmune[IMMUNITY_MECHANIC];
        for (SpellImmuneList::const_iterator itr = mechanicList.begin(); itr != mechanicList.end(); ++itr)
            if (itr->type == mechanic)
                return true;
    }

    if (uint32 aura = spellInfo->Effects[index].ApplyAuraName)
    {
        SpellImmuneList const& list = m_spellImmune[IMMUNITY_STATE];
        for (SpellImmuneList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
            if (itr->type == aura)
                return true;
        // Check for immune to application of harmful magical effects
        AuraEffectList const& immuneAuraApply = GetAuraEffectsByType(SPELL_AURA_MOD_IMMUNE_AURA_APPLY_SCHOOL);
        for (AuraEffectList::const_iterator iter = immuneAuraApply.begin(); iter != immuneAuraApply.end(); ++iter)
        {
            if (((*iter)->GetMiscValue() & spellInfo->GetSchoolMask()) &&  // Check school
                !spellInfo->IsPositiveEffect(index) && !spellInfo->CanPierceImmuneAura((*iter)->GetSpellInfo())) // Harmful
            {
                if (!(spellInfo->AttributesEx3 & SPELL_ATTR3_IGNORE_HIT_RESULT))
                    return true;
            }
        }
    }

    return false;
}

uint32 Unit::MeleeDamageBonusDone(Unit* victim, uint32 pdamage, WeaponAttackType attType, SpellInfo const* spellProto)
{
    if (!victim || pdamage == 0)
        return 0;

    uint32 creatureTypeMask = victim->GetCreatureTypeMask();

    // Done fixed damage bonus auras
    int32 DoneFlatBenefit = 0;

    // ..done
    AuraEffectList const& mDamageDoneCreature = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_DONE_CREATURE);
    for (AuraEffectList::const_iterator i = mDamageDoneCreature.begin(); i != mDamageDoneCreature.end(); ++i)
        if (creatureTypeMask & uint32((*i)->GetMiscValue()))
            DoneFlatBenefit += (*i)->GetAmount();

    // ..done
    // SPELL_AURA_MOD_DAMAGE_DONE included in weapon damage

    // ..done (base at attack power for marked target and base at attack power for creature type)
    int32 APbonus = 0;

    if (attType == WeaponAttackType::RangedAttack)
    {
        APbonus += victim->GetTotalAuraModifier(SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS);

        // ..done (base at attack power and creature type)
        AuraEffectList const& mCreatureAttackPower = GetAuraEffectsByType(SPELL_AURA_MOD_RANGED_ATTACK_POWER_VERSUS);
        for (AuraEffectList::const_iterator i = mCreatureAttackPower.begin(); i != mCreatureAttackPower.end(); ++i)
            if (creatureTypeMask & uint32((*i)->GetMiscValue()))
                APbonus += (*i)->GetAmount();
    }
    else
    {
        APbonus += victim->GetTotalAuraModifier(SPELL_AURA_MELEE_ATTACK_POWER_ATTACKER_BONUS);

        // ..done (base at attack power and creature type)
        AuraEffectList const& mCreatureAttackPower = GetAuraEffectsByType(SPELL_AURA_MOD_MELEE_ATTACK_POWER_VERSUS);
        for (AuraEffectList::const_iterator i = mCreatureAttackPower.begin(); i != mCreatureAttackPower.end(); ++i)
            if (creatureTypeMask & uint32((*i)->GetMiscValue()))
                APbonus += (*i)->GetAmount();
    }

    if (APbonus != 0)                                       // Can be negative
    {
        bool normalized = false;
        if (spellProto)
            for (uint8 i = 0; i < spellProto->EffectCount; ++i)
                if (spellProto->Effects[i].Effect == SPELL_EFFECT_NORMALIZED_WEAPON_DMG)
                {
                    normalized = true;
                    break;
                }
        DoneFlatBenefit += int32(APbonus/14.0f * GetAPMultiplier(attType, normalized));
    }

    // Done total percent damage auras
    float DoneTotalMod = 1.0f;

    if (Unit* owner = GetOwner())
    {
        AuraEffectList const& mModPetStats = owner->GetAuraEffectsByType(SPELL_AURA_MOD_PET_STATS);
        float amount = 0;
        for (AuraEffectList::const_iterator i = mModPetStats.begin(); i != mModPetStats.end(); ++i)
            if ((*i)->GetMiscValue() == INCREASE_MELEE_DAMAGE_PERCENT)
                amount += float((*i)->GetAmount());

            AddPct(DoneTotalMod, amount);
    }

    ///  Custom WoD Script - Mastery: Unshackled Fury
    if (GetTypeId() == TYPEID_PLAYER && HasAura(76856) && HasAura(12880))
    {
        Player* l_Player = ToPlayer();
        if (AuraEffect* l_MasteryUnshackledFury = l_Player->GetAuraEffect(76856, EFFECT_0))
        {
            float l_MasteryPct = l_Player->GetFloatValue(PLAYER_FIELD_MASTERY);
            float l_MasteryMultiplier = l_MasteryUnshackledFury->GetSpellEffectInfo()->BonusMultiplier;
            int32 l_MasteryValue = (int32)(l_MasteryMultiplier * l_MasteryPct);
            AddPct(DoneTotalMod, l_MasteryValue);
        }
    }

    /// Custom WoD Script - Glyph of Frostbrand Weapon
    if (IsPlayer() && victim && ToPlayer()->getClass() == CLASS_SHAMAN && ToPlayer()->GetSpecializationId() == SPEC_SHAMAN_ENHANCEMENT && attType == WeaponAttackType::BaseAttack)
    {
        if (HasAura(161654) && pdamage != 0)
            CastSpell(victim, 147732, true);
    }

    // Custom MoP Script
    // 76613 - Mastery : Frostburn for Water elemental Melee damage
    if (IsPlayer() && victim && pdamage != 0)
    {
        if (isPet())
        {
            Unit* owner = GetOwner();
            if (owner->HasAura(76613) && owner->IsPlayer())
            {
                float Mastery = owner->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;
                AddPct(DoneTotalMod, Mastery);
            }
        }
    }

    // Ghoul benefit from dmg pct mod from Shadow Infusion
    if (GetTypeId() == TYPEID_UNIT && HasAura(91342))
        if (Aura* stacks = GetAura(91342))
            if (AuraEffect* shadowInfusion = stacks->GetEffect(0))
                AddPct(DoneTotalMod, shadowInfusion->GetAmount());

    if (spellProto)
    {
        AuraEffectList const& mModDamagePercentDone = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        for (AuraEffectList::const_iterator i = mModDamagePercentDone.begin(); i != mModDamagePercentDone.end(); ++i)
        {
            if ((*i)->GetMiscValue() & spellProto->GetSchoolMask() && !(spellProto->GetSchoolMask() & SPELL_SCHOOL_MASK_NORMAL))
            {
                if ((*i)->GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL)
                    continue;
                if ((*i)->GetSpellInfo()->EquippedItemClass == -1)
                    AddPct(DoneTotalMod, (*i)->GetAmount());
                else if (!((*i)->GetSpellInfo()->AttributesEx5 & SPELL_ATTR5_SPECIAL_ITEM_CLASS_CHECK) && ((*i)->GetSpellInfo()->EquippedItemSubClassMask == 0))
                    AddPct(DoneTotalMod, (*i)->GetAmount());
                else if (ToPlayer() && ToPlayer()->HasItemFitToSpellRequirements((*i)->GetSpellInfo()))
                    AddPct(DoneTotalMod, (*i)->GetAmount());
            }
        }
    }

    if ((isPet() || isGuardian()) && GetSpellModOwner())
    {
        AuraEffectList const& mModDamagePercentDone = GetSpellModOwner()->GetAuraEffectsByType(SPELL_AURA_MOD_PET_DAMAGE_DONE);
        for (AuraEffectList::const_iterator i = mModDamagePercentDone.begin(); i != mModDamagePercentDone.end(); ++i)
            AddPct(DoneTotalMod, (*i)->GetAmount());
    }

    AuraEffectList const& mDamageDoneVersus = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_DONE_VERSUS);
    for (AuraEffectList::const_iterator i = mDamageDoneVersus.begin(); i != mDamageDoneVersus.end(); ++i)
        if (creatureTypeMask & uint32((*i)->GetMiscValue()))
            AddPct(DoneTotalMod, (*i)->GetAmount());

    // bonus against aurastate
    AuraEffectList const& mDamageDoneVersusAurastate = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_DONE_VERSUS_AURASTATE);
    for (AuraEffectList::const_iterator i = mDamageDoneVersusAurastate.begin(); i != mDamageDoneVersusAurastate.end(); ++i)
        if (victim->HasAuraState(AuraStateType((*i)->GetMiscValue())))
            AddPct(DoneTotalMod, (*i)->GetAmount());

    if (GetSpellModOwner())
        AddPct(DoneTotalMod, GetSpellModOwner()->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_DONE) + GetSpellModOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT));

    // Add SPELL_AURA_MOD_DAMAGE_DONE_FOR_MECHANIC percent bonus
    if (spellProto)
        AddPct(DoneTotalMod, GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_DAMAGE_DONE_FOR_MECHANIC, spellProto->Mechanic));

    // done scripted mod (take it from owner)
    Unit* owner = GetOwner() ? GetOwner() : this; ///< owner is never read 01/18/16

    float tmpDamage = float(int32(pdamage) + DoneFlatBenefit) * DoneTotalMod;
    float preTmpDamage = tmpDamage;

    // apply spellmod to Done damage
    if (spellProto)
        if (Player* modOwner = GetSpellModOwner())
            modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_DAMAGE, tmpDamage);

    if (spellProto)
        LOG_SPELL(this, spellProto->Id, "MeeleDamageBonusDone(): Spell %s: ((%i + %i (DoneFlatBenefit)) * %f (DoneTotalMod)) = %f + %f (Mods) = %f", spellProto->GetNameForLogging().c_str(), pdamage, DoneFlatBenefit, DoneTotalMod, preTmpDamage, tmpDamage - preTmpDamage, tmpDamage); ///< ISO C++11 does not allow conversion from string literal to 'char *'

    // bonus result can be negative
    return uint32(std::max(tmpDamage, 0.0f));
}

uint32 Unit::MeleeDamageBonusTaken(Unit* attacker, uint32 pdamage, WeaponAttackType attType, SpellInfo const* spellProto)
{
    if (pdamage == 0)
        return 0;

    int32 TakenFlatBenefit = 0;
    float TakenTotalCasterMod = 0.0f;

    // get all auras from caster that allow the spell to ignore resistance
    SpellSchoolMask attackSchoolMask = spellProto ? spellProto->GetSchoolMask() : SPELL_SCHOOL_MASK_NORMAL;
    AuraEffectList const& IgnoreResistAuras = attacker->GetAuraEffectsByType(SPELL_AURA_MOD_IGNORE_TARGET_RESIST);
    for (AuraEffectList::const_iterator i = IgnoreResistAuras.begin(); i != IgnoreResistAuras.end(); ++i)
    {
        if ((*i)->GetMiscValue() & attackSchoolMask)
            TakenTotalCasterMod += (float((*i)->GetAmount()));
    }

    // ..taken
    AuraEffectList const& mDamageTaken = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_TAKEN);
    for (AuraEffectList::const_iterator i = mDamageTaken.begin(); i != mDamageTaken.end(); ++i)
        if ((*i)->GetMiscValue() & GetMeleeDamageSchoolMask())
            TakenFlatBenefit += (*i)->GetAmount();

    if (attType != WeaponAttackType::RangedAttack)
        TakenFlatBenefit += GetTotalAuraModifier(SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN);
    else
        TakenFlatBenefit += GetTotalAuraModifier(SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN);

    // Taken total percent damage auras
    float TakenTotalMod = 1.0f;

    // ..taken
    TakenTotalMod *= GetTotalAuraMultiplierByMiscMask(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN,
    spellProto ? spellProto->GetSchoolMask() : GetMeleeDamageSchoolMask());

    // .. taken pct (special attacks)
    if (spellProto)
    {
        // From caster spells
        AuraEffectList const& mOwnerTaken = GetAuraEffectsByType(SPELL_AURA_MOD_DAMAGE_FROM_CASTER);
        for (AuraEffectList::const_iterator i = mOwnerTaken.begin(); i != mOwnerTaken.end(); ++i)
            if ((*i)->GetCasterGUID() == attacker->GetGUID())
            {
                switch ((*i)->GetId())
                {
                    // Vendetta, should affect to all damage
                    case 79140:
                        TakenTotalMod += CalculatePct(1.0, (*i)->GetAmount());
                        break;
                    default:
                        if ((*i)->IsAffectingSpell(spellProto))
                            TakenTotalMod += CalculatePct(1.0, (*i)->GetAmount());
                        break;
                }
            }

        // Mod damage from spell mechanic
        uint32 mechanicMask = spellProto->GetAllEffectsMechanicMask();

        // Shred, Maul - "Effects which increase Bleed damage also increase Shred damage"
        if (spellProto->SpellFamilyName == SPELLFAMILY_DRUID && spellProto->SpellFamilyFlags[0] & 0x00008800)
            mechanicMask |= (1<<MECHANIC_BLEED);

        if (mechanicMask)
        {
            AuraEffectList const& mDamageDoneMechanic = GetAuraEffectsByType(SPELL_AURA_MOD_MECHANIC_DAMAGE_TAKEN_PERCENT);
            for (AuraEffectList::const_iterator i = mDamageDoneMechanic.begin(); i != mDamageDoneMechanic.end(); ++i)
                if (mechanicMask & uint32(1<<((*i)->GetMiscValue())))
                    TakenTotalMod += CalculatePct(1.0, (*i)->GetAmount());
        }
    }
    else
    {
        AuraEffectList const& mOwnerTaken = GetAuraEffectsByType(SPELL_AURA_MOD_AUTOATTACK_DAMAGE_TARGET);
        for (AuraEffectList::const_iterator i = mOwnerTaken.begin(); i != mOwnerTaken.end(); ++i)
        {
            if ((*i)->GetCaster() == attacker)
                TakenTotalMod += CalculatePct(1.0, (*i)->GetAmount());
        }
    }

    if (attType != WeaponAttackType::RangedAttack)
    {
        AuraEffectList const& mModMeleeDamageTakenPercent = GetAuraEffectsByType(SPELL_AURA_MOD_MELEE_DAMAGE_TAKEN_PCT);
        for (AuraEffectList::const_iterator i = mModMeleeDamageTakenPercent.begin(); i != mModMeleeDamageTakenPercent.end(); ++i)
            TakenTotalMod += CalculatePct(1.0, (*i)->GetAmount());
    }
    else
    {
        AuraEffectList const& mModRangedDamageTakenPercent = GetAuraEffectsByType(SPELL_AURA_MOD_RANGED_DAMAGE_TAKEN_PCT);
        for (AuraEffectList::const_iterator i = mModRangedDamageTakenPercent.begin(); i != mModRangedDamageTakenPercent.end(); ++i)
            TakenTotalMod += CalculatePct(1.0, (*i)->GetAmount());
    }

    if (spellProto && (spellProto->IsTargetingArea() || spellProto->HasCustomAttribute(SpellCustomAttributes::SPELL_ATTR0_CU_IS_CUSTOM_AOE_SPELL)))
    {
        int32 mult = GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_AOE_DAMAGE_AVOIDANCE, spellProto->SchoolMask);
        TakenTotalMod += CalculatePct(1.0, mult);
        if (attacker->GetTypeId() != TYPEID_PLAYER)
        {
            int32 u_mult = GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_CREATURE_AOE_DAMAGE_AVOIDANCE, spellProto->SchoolMask);
            TakenTotalMod += CalculatePct(1.0, u_mult);
        }
    }

    /// Apply Versatility damage bonus taken
    if (GetSpellModOwner())
        TakenTotalMod -= CalculatePct(1.0, GetSpellModOwner()->GetRatingBonusValue(CR_VERSATILITY_DAMAGE_TAKEN) + GetSpellModOwner()->GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT));

    float tmpDamage = 0.0f;
    tmpDamage = ((CalculatePct(float(pdamage) + TakenFlatBenefit, TakenTotalCasterMod) * TakenTotalMod) + CalculatePct(float(pdamage) + TakenFlatBenefit, TakenTotalCasterMod));

    if (!tmpDamage)
        tmpDamage = (float(pdamage) + TakenFlatBenefit) * TakenTotalMod;

    // bonus result can be negative
    return uint32(std::max(tmpDamage, 0.0f));
}

void Unit::ApplyUberImmune(uint32 spellid, bool apply)
{
    if (apply)
        RemoveAurasWithMechanic(IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK, AURA_REMOVE_BY_DEFAULT, spellid);
    for (uint32 mech=MECHANIC_CHARM; mech!=MECHANIC_ENRAGED; ++mech)
    {
        if (mech == MECHANIC_DISARM)
            continue;
        if (1<<mech & IMMUNE_TO_MOVEMENT_IMPAIRMENT_AND_LOSS_CONTROL_MASK)
            ApplySpellImmune(spellid, IMMUNITY_MECHANIC, mech, apply);
    }
}

void Unit::ApplySpellImmune(uint32 spellId, uint32 op, uint32 type, bool apply)
{
    if (apply)
    {
        for (SpellImmuneList::iterator itr = m_spellImmune[op].begin(), next; itr != m_spellImmune[op].end(); itr = next)
        {
            next = itr; ++next;
            if (itr->type == type)
            {
                m_spellImmune[op].erase(itr);
                next = m_spellImmune[op].begin();
            }
        }
        SpellImmune Immune;
        Immune.spellId = spellId;
        Immune.type = type;
        m_spellImmune[op].push_back(Immune);
    }
    else
    {
        for (SpellImmuneList::iterator itr = m_spellImmune[op].begin(); itr != m_spellImmune[op].end(); ++itr)
        {
            if (itr->spellId == spellId && itr->type == type)
            {
                m_spellImmune[op].erase(itr);
                break;
            }
        }
    }
}

void Unit::ApplySpellDispelImmunity(const SpellInfo* spellProto, DispelType type, bool apply)
{
    ApplySpellImmune(spellProto->Id, IMMUNITY_DISPEL, type, apply);

    if (apply && spellProto->AttributesEx & SPELL_ATTR1_DISPEL_AURAS_ON_IMMUNITY)
    {
        // Create dispel mask by dispel type
        uint32 dispelMask = SpellInfo::GetDispelMask(type);
        // Dispel all existing auras vs current dispel type
        AuraApplicationMap& auras = GetAppliedAuras();
        for (AuraApplicationMap::iterator itr = auras.begin(); itr != auras.end();)
        {
            SpellInfo const* spell = itr->second->GetBase()->GetSpellInfo();
            if (spell->GetDispelMask() & dispelMask)
            {
                // Dispel aura
                RemoveAura(itr);
            }
            else
                ++itr;
        }
    }
}

float Unit::GetWeaponProcChance() const
{
    // normalized proc chance for weapon attack speed
    // (odd formula...)
    if (isAttackReady(WeaponAttackType::BaseAttack))
        return (GetAttackTime(WeaponAttackType::BaseAttack) * 1.8f / 1000.0f);
    else if (haveOffhandWeapon() && isAttackReady(WeaponAttackType::OffAttack))
        return (GetAttackTime(WeaponAttackType::OffAttack) * 1.6f / 1000.0f);
    return 0;
}

float Unit::GetPPMProcChance(uint32 WeaponSpeed, float PPM, const SpellInfo* spellProto) const
{
    // proc per minute chance calculation
    if (PPM <= 0)
        return 0.0f;

    // Apply chance modifer aura
    if (spellProto)
        if (Player* modOwner = GetSpellModOwner())
            modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_PROC_PER_MINUTE, PPM);

    return (WeaponSpeed * PPM) / 600.0f;   // result is chance in percents (probability = Speed_in_sec * (PPM / 60))
}

void Unit::Mount(uint32 mount, uint32 VehicleId, uint32 creatureEntry)
{
    if (mount)
        SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, mount);

    SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNT);

    if (Player* player = ToPlayer())
    {
        if (player->GetEmoteState())
            player->SetEmoteState(0);

        player->SetFallInformation(0, GetPositionZ());

        // mount as a vehicle
        if (VehicleId)
        {
            if (CreateVehicleKit(VehicleId, creatureEntry))
            {
                GetVehicleKit()->Reset();

                WorldPacket l_Data(SMSG_MOVE_SET_VEHICLE_REC_ID, 16 + 2 + 4 + 4);
                l_Data.appendPackGUID(GetGUID());   ///< MoverGUID
                l_Data << uint32(0);                ///< SequenceIndex
                l_Data << uint32(VehicleId);        ///< VehicleRecID
                SendMessageToSet(&l_Data, true);

                // Send others that we now have a vehicle
                l_Data.Initialize(SMSG_SET_VEHICLE_REC_ID, 16 + 2 + 4);
                l_Data.appendPackGUID(GetGUID());
                l_Data << uint32(VehicleId);
                ToPlayer()->GetSession()->SendPacket(&l_Data);

                // mounts can also have accessories
                GetVehicleKit()->InstallAllAccessories(false);
            }
        }

        if (player->HasAura(56232) && (creatureEntry == 304 || creatureEntry == 73965 || creatureEntry == 73966))
            player->CastSpell(player, 143314, true);

        // Remove subterfuge just after cast
        player->RemoveAura(115192);

        // don't unsummon pet but SetFlag UNIT_FLAG_STUNNED to disable pet's interface
        if (Pet* pet = player->GetPet())
            pet->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

        player->UnsummonCurrentBattlePetIfAny(true);
        player->SendMovementSetCollisionHeight(player->GetCollisionHeight(true));

        if ((mount == 19296 || mount == 19085 || mount == 31367 || mount == 31368 || mount == 8469 || mount == 14548 || mount == 30366 || mount == 30501 || mount == 28919) && player->HasAura(57958)) // TODO: we need to create a new trigger flag - on mount, to handle it properly
            player->AddAura(20217, player);

        sScriptMgr->OnPlayerMount(player, creatureEntry);
    }

    RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_MOUNT);
}

void Unit::Dismount()
{
    if (!IsMounted())
        return;

    SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, 0);
    RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNT);

    if (Player* thisPlayer = ToPlayer())
    {
        thisPlayer->SendMovementSetCollisionHeight(thisPlayer->GetCollisionHeight(false));
        thisPlayer->RemoveAurasDueToSpell(143314);
    }

    uint64 l_Guid = GetGUID();
    // Dismount as a vehicle
    if (IsPlayer() && GetVehicleKit())
    {
        // Remove vehicle from player
        RemoveVehicleKit(true);

        WorldPacket l_Data(SMSG_MOVE_SET_VEHICLE_REC_ID, 16 + 2 + 4 + 4);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint32(0);
        l_Data << uint32(0);
        ToPlayer()->SendMessageToSet(&l_Data, true);

        l_Data.Initialize(SMSG_SET_VEHICLE_REC_ID, 8 + 4);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint32(0);
        ToPlayer()->GetSession()->SendPacket(&l_Data);
    }

    RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_NOT_MOUNTED);

    // only re summon old pet if the player is already added to a map
    // this prevents adding a pet to a not created map which would otherwise cause a crash
    // (it could probably happen when logging in after a previous crash)
    if (Player* player = ToPlayer())
    {
        player->SetFallInformation(0, GetPositionZ());

        if (Pet* pPet = player->GetPet())
        {
            if (pPet->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED) && !pPet->HasUnitState(UNIT_STATE_STUNNED))
                pPet->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        }
        else
        {
            player->ResummonPetTemporaryUnSummonedIfAny();
            player->SummonLastSummonedBattlePet();
        }
    }
}

MountCapabilityEntry const* Unit::GetMountCapability(uint32 mountType) const
{
    if (!mountType)
        return nullptr;

    MountTypeEntry const* l_MountTypeEntry = sMountTypeStore.LookupEntry(mountType);
    if (!l_MountTypeEntry)
        return nullptr;

    uint32 l_ZoneId;
    uint32 l_AreaId;
    GetZoneAndAreaId(l_ZoneId, l_AreaId);

    uint32 l_RidingSkill = 5000;
    if (IsPlayer())
        l_RidingSkill = ToPlayer()->GetSkillValue(SKILL_RIDING);

    for (uint32 i = 0; i < MAX_MOUNT_CAPABILITIES; ++i)
    {
        MountCapabilityEntry const* l_MountCapability = sMountCapabilityStore.LookupEntry(sMountCapabilitiesMap[mountType].Capabilities[i]);
        if (!l_MountCapability)
            continue;

        if (l_RidingSkill < l_MountCapability->RequiredRidingSkill)
            continue;

        if (HasExtraUnitMovementFlag(MOVEMENTFLAG2_FULL_SPEED_PITCHING))
        {
            if (!(l_MountCapability->Flags & MOUNT_FLAG_CAN_PITCH))
                continue;
        }
        else if (HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
        {
            if (!(l_MountCapability->Flags & MOUNT_FLAG_CAN_SWIM))
                continue;
        }

        if (l_MountCapability->RequiredMap != -1 && int32(GetMapId()) != l_MountCapability->RequiredMap)
        {
            bool l_ByPass = false;

            /// Blasted Lands, level 90
            if (l_MountCapability->RequiredMap == 1 && GetMapId() == 1190)
                l_ByPass = true;

            if (!l_ByPass)
                continue;
        }

        if (l_MountCapability->RequiredArea && (l_MountCapability->RequiredArea != l_ZoneId && l_MountCapability->RequiredArea != l_AreaId))
            continue;

        if (l_MountCapability->RequiredAura && !HasAura(l_MountCapability->RequiredAura))
            continue;

        if (l_MountCapability->RequiredSpell && (GetTypeId() != TYPEID_PLAYER || !ToPlayer()->HasSpell(l_MountCapability->RequiredSpell)))
            continue;

        return l_MountCapability;
    }

    return nullptr;
}

void Unit::SendMountResult(MountResult p_Error)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    WorldPacket l_Data(SMSG_MOUNT_RESULT, 4);
    l_Data << uint32(p_Error);
    ToPlayer()->SendDirectMessage(&l_Data);
}

void Unit::SetInCombatWith(Unit* enemy)
{
    Unit* eOwner = enemy->GetCharmerOrOwnerOrSelf();
    if (eOwner->IsPvP())
    {
        SetInCombatState(true, enemy);
        return;
    }

    // check for duel
    if (eOwner->IsPlayer() && eOwner->ToPlayer()->m_Duel)
    {
        Unit const* myOwner = GetCharmerOrOwnerOrSelf();
        if (((Player const*)eOwner)->m_Duel->opponent == myOwner)
        {
            SetInCombatState(true, enemy);
            return;
        }
    }
    SetInCombatState(false, enemy);
}

void Unit::CombatStart(Unit* target, bool initialAggro)
{
    if (initialAggro)
    {
        if (!target->IsStandState())
            target->SetStandState(UNIT_STAND_STATE_STAND);

        if (!target->isInCombat() && target->GetTypeId() != TYPEID_PLAYER
            && !target->ToCreature()->HasReactState(REACT_PASSIVE) && target->ToCreature()->IsAIEnabled)
        {
            target->ToCreature()->AI()->AttackStart(this);
        }

        SetInCombatWith(target);
        target->SetInCombatWith(this);
    }
    Unit* who = target->GetCharmerOrOwnerOrSelf();
    if (who->IsPlayer())
      SetContestedPvP(who->ToPlayer());

    Player* me = GetCharmerOrOwnerPlayerOrPlayerItself();
    if (me && who->IsPvP()
        && (who->GetTypeId() != TYPEID_PLAYER
        || !me->m_Duel || me->m_Duel->opponent != who))
    {
        me->UpdatePvP(true);
        me->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    }
}

void Unit::SetInCombatState(bool p_IsPVP, Unit* p_Enemy, bool p_IsControlled)
{
    /// Only alive units can be in combat
    if (!isAlive())
        return;

    if (p_Enemy)
    {
        if (Creature* l_Creature = p_Enemy->ToCreature())
        {
            if (l_Creature->GetEntry() == 900000 || l_Creature->GetScriptName() == "npc_pvp_training_dummy") ///< Sovaks training dummy
                p_IsPVP = true;
        }
    }

    if (p_IsPVP)
    {
        if (Player* l_Player = ToPlayer())
        {
            l_Player->SetPvPTimer(15000); ///< 5 + 10 secs

            if (!l_Player->IsInPvPCombat())
                l_Player->SetInPvPCombat(true);
        }

        m_CombatTimer = 5000;
    }

    if (isInCombat() || HasUnitState(UNIT_STATE_EVADE))
        return;

    if (Creature* l_Creature = ToCreature())
    {
        if (IsAIEnabled && l_Creature->AI()->IsPassived())
            return;
    }

    SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

    if (p_IsControlled)
        SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PET_IN_COMBAT);

    for (Unit::ControlList::iterator l_Iter = m_Controlled.begin(); l_Iter != m_Controlled.end(); ++l_Iter)
    {
        if ((*l_Iter) != nullptr)
            (*l_Iter)->SetInCombatState(p_IsPVP, p_Enemy, true);
    }

    RemoveAura(121308); ///< Glyph of Disguise, only out of combat

    if (Creature* l_Creature = ToCreature())
    {
        /// Set home position at place of engaging combat for escorted creatures
        if ((IsAIEnabled && l_Creature->AI()->IsEscorted()) ||
            (GetMotionMaster() && (GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE ||
            GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)))
            l_Creature->SetHomePosition(GetPositionX(), GetPositionY(), GetPositionZ(), GetOrientation());

        if (p_Enemy)
        {
            p_Enemy->RemoveAura(121308); ///< Glyph of Disguise, only out of combat

            if (IsAIEnabled)
            {
                /// Clear emote state / anim kit id before enter in combat
                l_Creature->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                l_Creature->SetAIAnimKitId(0);

                l_Creature->AI()->EnterCombat(p_Enemy);

                if (l_Creature->isWorldBoss())
                {
                    if (InstanceScript* l_Instance = l_Creature->GetInstanceScript())
                    {
                        l_Instance->SendEncounterStart(l_Instance->GetEncounterIDForBoss(l_Creature));
                        l_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_START, l_Creature->ToUnit());

                        l_Creature->SetEncounterStartTime(time(nullptr));
                        l_Creature->DumpGroup();
                    }
                }

                RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC); ///< Unit has engaged in combat, remove immunity so players can fight back
            }

            if (l_Creature->GetFormation())
                l_Creature->GetFormation()->MemberAttackStart(l_Creature, p_Enemy);
        }

        if (isPet())
        {
            UpdateSpeed(MOVE_RUN, true);
            UpdateSpeed(MOVE_SWIM, true);
            UpdateSpeed(MOVE_FLIGHT, true);
        }

        if (!(l_Creature->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_MOUNTED_COMBAT_ALLOWED))
            Dismount();
    }
    else if (Player* l_Player = ToPlayer())
        sScriptMgr->OnPlayerEnterInCombat(l_Player);

    if (IsPlayer() && !ToPlayer()->IsInWorgenForm() && ToPlayer()->CanSwitch())
        ToPlayer()->SwitchToWorgenForm();
}

void Unit::ClearInCombat()
{
    m_CombatTimer = 0;
    RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

    // Player's state will be cleared in Player::UpdateContestedPvP
    if (Creature* creature = ToCreature())
    {
        if (creature->GetCreatureTemplate() && creature->GetCreatureTemplate()->UnitFlags1 & UNIT_FLAG_IMMUNE_TO_PC)
            SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC); // set immunity state to the one from db on evade

        ClearUnitState(UNIT_STATE_ATTACK_PLAYER);
        if (HasFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_TAPPED))
            SetUInt32Value(OBJECT_FIELD_DYNAMIC_FLAGS, creature->GetCreatureTemplate()->dynamicflags);

        if (creature->isPet() && !creature->isHunterPet()) ///< fix a problem with hunter pets , that their speed is wrong
        {
            if (Unit* owner = GetOwner())
                for (uint8 i = 0; i < MAX_MOVE_TYPE; ++i)
                    if (owner->GetSpeedRate(UnitMoveType(i)) > GetSpeedRate(UnitMoveType(i)))
                        SetSpeed(UnitMoveType(i), owner->GetSpeedRate(UnitMoveType(i)), true);
        }
        else if (!isCharmed())
            return;
    }
    else
    {
        if (Player* l_Player = ToPlayer())
        {
            l_Player->UpdatePotionCooldown();
            sScriptMgr->OnPlayerLeaveCombat(l_Player);
        }
    }

    RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PET_IN_COMBAT);
    RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_LEAVE_COMBAT);
}

bool Unit::isTargetableForAttack(bool checkFakeDeath) const
{
    if (!isAlive())
        return false;

    if (HasFlag(UNIT_FIELD_FLAGS,
        UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC) && GetEntry() != 62983)
        return false;

    if (IsPlayer() && ToPlayer()->isGameMaster())
        return false;

    return !HasUnitState(UNIT_STATE_UNATTACKABLE) && (!checkFakeDeath || !HasUnitState(UNIT_STATE_DIED));
}

bool Unit::IsValidAttackTarget(Unit const* target) const
{
    return _IsValidAttackTarget(target, NULL);
}

// function based on function Unit::CanAttack from 13850 client
bool Unit::_IsValidAttackTarget(Unit const* target, SpellInfo const* bySpell, WorldObject const* obj) const
{
    /// Pointer cannot be null in well-defined C++ code; comparison may be assumed to always evaluate to false
    if (target == nullptr)
    {
        sLog->outAshran("Unit::_IsValidAttackTarget, target is null!");
        return false;
    }

    bool areaSpell = false;
    if (bySpell)
        areaSpell = bySpell->IsAffectingArea();

    // can't attack self
    if (this == target)
        return false;

    // Sha of anger mind control and Maddening Shout
    if (target->HasAura(119626) || target->HasAura(117708))
        return true;

    /// Prismatic Crystal should be attackable only by summoner
    if (target->GetEntry() == 76933 && target->ToTempSummon() && target->ToTempSummon()->GetSummoner() && target->ToTempSummon()->GetSummoner()->GetGUID() != GetGUID())
        return false;

    // can't attack unattackable units or GMs
    if (target->HasUnitState(UNIT_STATE_UNATTACKABLE)
        || (target->IsPlayer() && target->ToPlayer()->isGameMaster()))
        return false;

    // can't attack own vehicle or passenger
    if (m_vehicle)
    {
        if (IsOnVehicle(target) || (m_vehicle->GetBase() && m_vehicle->GetBase()->IsOnVehicle(target)))
        {
            /// Allow players to attack their own vehicle if it is hostile to them
            if (!IsHostileTo(target))
                return false;
        }
    }

    // can't attack invisible (ignore stealth for aoe spells) also if the area being looked at is from a spell use the dynamic object created instead of the casting unit.
    if ((!bySpell || !(bySpell->AttributesEx6 & SPELL_ATTR6_CAN_TARGET_INVISIBLE)) && (obj ? !obj->canSeeOrDetect(target, areaSpell) : !canSeeOrDetect(target, areaSpell)))
        return false;

    // can't attack dead
    if ((!bySpell || !bySpell->IsAllowingDeadTarget()) && !target->isAlive())
       return false;

    // can't attack untargetable
    if ((!bySpell || !(bySpell->AttributesEx6 & SPELL_ATTR6_CAN_TARGET_UNTARGETABLE))
        && target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE) && target->GetEntry() != 62983)
        return false;

    if (Player const* playerAttacker = ToPlayer())
    {
        if (playerAttacker->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_UBER))
            return false;
    }

    // check flags
    if (target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_TAXI_FLIGHT | UNIT_FLAG_NOT_ATTACKABLE_1 | UNIT_FLAG_UNK_16)
        || (!HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) && target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
        || (!target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) && HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC)))
        return false;

    if ((!bySpell || !(bySpell->AttributesEx8 & SPELL_ATTR8_ATTACK_IGNORE_IMMUNE_TO_PC_FLAG))
        && (HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) && target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
        // check if this is a world trigger cast - GOs are using world triggers to cast their spells, so we need to ignore their immunity flag here, this is a temp workaround, needs removal when go cast is implemented properly
        && GetEntry() != WORLD_TRIGGER)
        return false;

    // CvC case - can attack each other only when one of them is hostile
    if (!HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) && !target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE))
        return GetReactionTo(target) <= REP_HOSTILE || target->GetReactionTo(this) <= REP_HOSTILE;

    // PvP, PvC, CvP case
    // can't attack friendly targets
    if ( GetReactionTo(target) > REP_NEUTRAL
        || target->GetReactionTo(this) > REP_NEUTRAL)
        return false;

    // Not all neutral creatures can be attacked
    if (GetReactionTo(target) == REP_NEUTRAL &&
        target->GetReactionTo(this) == REP_NEUTRAL)
    {
        if  (!(target->IsPlayer() && IsPlayer()) &&
            !(target->GetTypeId() == TYPEID_UNIT && GetTypeId() == TYPEID_UNIT))
        {
            Player const* player = target->IsPlayer() ? target->ToPlayer() : ToPlayer();
            Unit const* creature = target->GetTypeId() == TYPEID_UNIT ? target : this;
            {
                if (FactionTemplateEntry const* factionTemplate = creature->getFactionTemplateEntry())
                    if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionTemplate->Faction))
                        if (FactionState const* repState = player->GetReputationMgr().GetState(factionEntry))
                            if (!(repState->Flags & FACTION_FLAG_AT_WAR))
                                return false;
            }
        }
    }

    Creature const* creatureAttacker = ToCreature();
    if (creatureAttacker && creatureAttacker->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT)
        return false;

    Player const* playerAffectingAttacker = HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) ? GetAffectingPlayer() : NULL;
    Player const* playerAffectingTarget = target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) ? target->GetAffectingPlayer() : NULL;

    // check duel - before sanctuary checks
    if (playerAffectingAttacker && playerAffectingTarget)
        if (playerAffectingAttacker->m_Duel && playerAffectingAttacker->m_Duel->opponent == playerAffectingTarget && playerAffectingAttacker->m_Duel->startTime != 0)
            return true;

    // PvP case - can't attack when attacker or target are in sanctuary
    // however, 13850 client doesn't allow to attack when one of the unit's has sanctuary flag and is pvp
    if (target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) && HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE)
        && ((target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_SANCTUARY) || (GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_SANCTUARY)))
        return false;

    // additional checks - only PvP case
    if (playerAffectingAttacker && playerAffectingTarget)
    {
        if (target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_PVP)
            return true;

        if (GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_FFA_PVP
            && target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_FFA_PVP)
            return true;

        return (GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_UNK1)
            || (target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_UNK1);
    }
    return true;
}

bool Unit::IsValidAssistTarget(Unit const* target) const
{
    return _IsValidAssistTarget(target, NULL);
}

// function based on function Unit::CanAssist from 13850 client
bool Unit::_IsValidAssistTarget(Unit const* target, SpellInfo const* bySpell, bool duelFlag) const
{
    ASSERT(target);

    // can assist to self
    if (this == target)
        return true;

    /// on duel, the healing spells should not proc on other target than caster
    if (duelFlag && GetSpellModOwner() && GetSpellModOwner()->m_Duel)
    {
        if (target->GetSpellModOwner() == nullptr)
            return false;

        if (GetSpellModOwner()->GetGUID() != target->GetSpellModOwner()->GetGUID())
            return false;
    }

    // can't assist unattackable units or GMs
    if (target->HasUnitState(UNIT_STATE_UNATTACKABLE)
        || (target->IsPlayer() && target->ToPlayer()->isGameMaster()))
        return false;

    // can't assist own vehicle or passenger
    if (m_vehicle)
        if (IsOnVehicle(target) || (m_vehicle->GetBase() && m_vehicle->GetBase()->IsOnVehicle(target)))
            return false;

    // can't assist invisible
    if ((!bySpell || !(bySpell->AttributesEx6 & SPELL_ATTR6_CAN_TARGET_INVISIBLE)) && !canSeeOrDetect(target, bySpell && bySpell->IsAffectingArea()))
        return false;

    // can't assist dead
    if ((!bySpell || !bySpell->IsAllowingDeadTarget()) && !target->isAlive())
       return false;

    // can't assist untargetable
    if ((!bySpell || !(bySpell->AttributesEx6 & SPELL_ATTR6_CAN_TARGET_UNTARGETABLE))
        && target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
        return false;

    if (!bySpell || !(bySpell->AttributesEx6 & SPELL_ATTR6_ASSIST_IGNORE_IMMUNE_FLAG))
    {
        if (HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE))
        {
            if (target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC))
                return false;
        }
        else
        {
            if (target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                return false;
        }
    }

    // can't assist non-friendly targets
    if (GetReactionTo(target) < REP_NEUTRAL
        && target->GetReactionTo(this) < REP_NEUTRAL
        && (!ToCreature() || !(ToCreature()->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT)))
        return false;

    // PvP case
    if (target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE))
    {
        Player const* targetPlayerOwner = target->GetAffectingPlayer();
        if (HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE))
        {
            Player const* selfPlayerOwner = GetAffectingPlayer();
            if (selfPlayerOwner && targetPlayerOwner)
            {
                // can't assist player which is dueling someone
                if (selfPlayerOwner != targetPlayerOwner
                    && targetPlayerOwner->m_Duel)
                    return false;
            }
            // can't assist player in ffa_pvp zone from outside
            if ((target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_FFA_PVP)
                && !(GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_FFA_PVP))
                return false;
            // can't assist player out of sanctuary from sanctuary if has pvp enabled
            if (target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_PVP)
                if ((GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_SANCTUARY) && !(target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_SANCTUARY))
                    return false;
        }
    }
    // PvC case - player can assist creature only if has specific type flags
    // !target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE) &&
    else if (HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE)
        && (!bySpell || !(bySpell->AttributesEx6 & SPELL_ATTR6_ASSIST_IGNORE_IMMUNE_FLAG))
        && !((target->GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 1) & UNIT_BYTE2_FLAG_PVP)))
    {
        if (Creature const* creatureTarget = target->ToCreature())
            return creatureTarget->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT || creatureTarget->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_CAN_ASSIST;
    }
    return true;
}

int32 Unit::ModifyHealth(int32 dVal)
{
    int32 gain = 0;

    if (dVal == 0)
        return 0;

    // Part of Evade mechanics. Only track health lost, not gained
    if (dVal < 0 && GetTypeId() != TYPEID_PLAYER && !isPet())
        SetLastDamagedTime(time(NULL));

    int32 curHealth = (int32)GetHealth();

    int32 val = dVal + curHealth;
    if (val <= 0)
    {
        SetHealth(0);
        return -curHealth;
    }

    int32 maxHealth = (int32)GetMaxHealth();

    if (val < maxHealth)
    {
        SetHealth(val);
        gain = val - curHealth;
    }
    else if (curHealth != maxHealth)
    {
        SetHealth(maxHealth);
        gain = maxHealth - curHealth;
    }

    return gain;
}

int32 Unit::GetHealthGain(int32 dVal)
{
    int32 gain = 0;

    if (dVal == 0)
        return 0;

    int32 curHealth = (int32)GetHealth();

    int32 val = dVal + curHealth;
    if (val <= 0)
    {
        return -curHealth;
    }

    int32 maxHealth = (int32)GetMaxHealth();

    if (val < maxHealth)
        gain = dVal;
    else if (curHealth != maxHealth)
        gain = maxHealth - curHealth;

    return gain;
}

// returns negative amount on power reduction
int32 Unit::ModifyPower(Powers power, int32 dVal)
{
    int32 gain = 0;

    if (dVal == 0 && power != POWER_ENERGY) // The client will always regen energy if we don't send him the actual value
        return 0;

    int32 curPower = GetPower(power);

    int32 val = dVal + curPower;
    if (val <= GetMinPower(power))
    {
        SetPower(power, GetMinPower(power));
        return -curPower;
    }

    int32 maxPower = GetMaxPower(power);

    if (val < maxPower)
    {
        SetPower(power, val);
        gain = val - curPower;
    }
    else if (curPower != maxPower)
    {
        SetPower(power, maxPower);
        gain = maxPower - curPower;
    }

    if (ToCreature() && ToCreature()->IsAIEnabled)
        ToCreature()->AI()->PowerModified(getPowerType(), GetPower(getPowerType()));

    return gain;
}

void Unit::AddComboPoints(int8 p_Count)
{
    if (!p_Count)
        return;

    /// Without combo points lost (duration checked in aura)
    RemoveAurasByType(AuraType::SPELL_AURA_RETAIN_COMBO_POINTS);
    ModifyPower(Powers::POWER_COMBO_POINT, p_Count);
}

void Unit::ClearComboPoints()
{
    /// Without combopoints lost (duration checked in aura)
    RemoveAurasByType(SPELL_AURA_RETAIN_COMBO_POINTS);
    SetPower(Powers::POWER_COMBO_POINT, 0);
}

// returns negative amount on power reduction
int32 Unit::ModifyPowerPct(Powers power, float pct, bool apply)
{
    float amount = (float)GetMaxPower(power);
    ApplyPercentModFloatVar(amount, pct, apply);

    return ModifyPower(power, (int32)amount - GetMaxPower(power));
}

bool Unit::IsAlwaysVisibleFor(WorldObject const* seer) const
{
    if (WorldObject::IsAlwaysVisibleFor(seer))
        return true;

    // Always seen by owner
    if (uint64 guid = GetCharmerOrOwnerGUID())
        if (seer->GetGUID() == guid)
            return true;

    if (Player const* seerPlayer = seer->ToPlayer())
        if (Unit* owner =  GetOwner())
            if (Player* ownerPlayer = owner->ToPlayer())
                if (ownerPlayer->IsGroupVisibleFor(seerPlayer))
                    return true;

    return false;
}

bool Unit::IsAlwaysDetectableFor(WorldObject const* seer) const
{
    if (WorldObject::IsAlwaysDetectableFor(seer))
        return true;

    if (HasAuraTypeWithCaster(SPELL_AURA_MOD_STALKED, seer->GetGUID()))
        return true;

    return false;
}

void Unit::SetVisible(bool x)
{
    if (!x)
        m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_GAMEMASTER);
    else
        m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);

    UpdateObjectVisibility();
}

void Unit::UpdateSpeed(UnitMoveType mtype, bool forced)
{
    //if (this->ToPlayer())
    //    sAnticheatMgr->DisableAnticheatDetection(this->ToPlayer());

    float main_speed_mod  = 1.0f;
    float stack_bonus     = 1.0f;
    float non_stack_bonus = 1.0f;

    switch (mtype)
    {
        // Only apply debuffs
        case MOVE_FLIGHT_BACK:
        case MOVE_RUN_BACK:
        case MOVE_SWIM_BACK:
            break;
        case MOVE_WALK:
            return;
        case MOVE_RUN:
        {
            if (IsMounted()) // Use on mount auras
            {
                main_speed_mod = GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED);
                stack_bonus     = GetTotalAuraMultiplier(SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS);
                non_stack_bonus = GetMaxPositiveAuraModifier(SPELL_AURA_MOD_MOUNTED_SPEED_NOT_STACK);
            }
            else
            {
                main_speed_mod  = GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_SPEED);
                stack_bonus     = GetTotalAuraMultiplier(SPELL_AURA_MOD_SPEED_ALWAYS);
                non_stack_bonus = GetMaxPositiveAuraModifier(SPELL_AURA_MOD_SPEED_NOT_STACK);

            }
            break;
        }
        case MOVE_SWIM:
        {
            main_speed_mod  = GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_SWIM_SPEED);
            break;
        }
        case MOVE_FLIGHT:
        {
            if (GetTypeId() == TYPEID_UNIT && IsControlledByPlayer()) // not sure if good for pet
            {
                main_speed_mod  = GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
                stack_bonus     = GetTotalAuraMultiplier(SPELL_AURA_MOD_VEHICLE_SPEED_ALWAYS);

                // for some spells this mod is applied on vehicle owner
                float owner_speed_mod = 0.0f;

                if (Unit* owner = GetCharmer())
                    owner_speed_mod = owner->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);

                main_speed_mod = std::max(main_speed_mod, owner_speed_mod);
            }
            else if (IsMounted())
            {
                main_speed_mod  = GetMaxPositiveAuraModifier(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED);
                stack_bonus     = GetTotalAuraMultiplier(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS);
            }
            else             // Use not mount (shapeshift for example) auras (should stack)
                main_speed_mod  = GetTotalAuraModifier(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED) + GetTotalAuraModifier(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);

            non_stack_bonus += GetMaxPositiveAuraModifier(SPELL_AURA_MOD_FLIGHT_SPEED_NOT_STACK) / 100.0f;

            // Update speed for vehicle if available
            if (IsPlayer() && GetVehicle())
                GetVehicleBase()->UpdateSpeed(MOVE_FLIGHT, true);
            break;
        }
        default:
            return;
    }

    /// Now we ready for speed calculation
    float total_non_stack_bonus = main_speed_mod + non_stack_bonus;
    float speed = stack_bonus + (total_non_stack_bonus / 100);

    if (IsPlayer())
        speed += CalculatePct(speed, ToPlayer()->GetFloatValue(PLAYER_FIELD_SPEED));

    switch (mtype)
    {
        case MOVE_RUN:
        case MOVE_SWIM:
        case MOVE_FLIGHT:
        {
            // Set creature speed rate
            if (GetTypeId() == TYPEID_UNIT)
            {
                Unit* pOwner = GetCharmerOrOwner();
                if ((isPet() || isGuardian()) && !isInCombat() && pOwner) // Must check for owner or crash on "Tame Beast"
                {
                    // For every yard over 5, increase speed by 0.01
                    //  to help prevent pet from lagging behind and despawning
                    float dist = GetDistance(pOwner);
                    float base_rate = 1.00f; // base speed is 100% of owner speed

                    if (dist < 1.0f)
                        dist = 1.0f;

                    float mult = base_rate + (dist >= 1.0f ? std::min(0.5f, ((dist - 1.0f) * 0.05f)) : 0.0f);

                    speed *= pOwner->GetSpeedRate(mtype) * mult; // pets derive speed from owner when not in combat
                }
                else
                    speed *= ToCreature()->GetCreatureTemplate()->speed_run;    // at this point, MOVE_WALK is never reached
            }
            // Normalize speed by 191 aura SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED if need
            // TODO: possible affect only on MOVE_RUN
            if (int32 normalization = GetMaxPositiveAuraModifier(SPELL_AURA_USE_NORMAL_MOVEMENT_SPEED))
            {
                // Use speed from aura
                float max_speed = normalization / (IsControlledByPlayer() ? playerBaseMoveSpeed[mtype] : baseMoveSpeed[mtype]);
                if (speed > max_speed)
                    speed = max_speed;
            }
            break;
        }
        default:
            break;
    }

    // for creature case, we check explicit if mob searched for assistance
    if (GetTypeId() == TYPEID_UNIT)
    {
        if (ToCreature()->HasSearchedAssistance())
            speed *= 0.66f;                                 // best guessed value, so this will be 33% reduction. Based off initial speed, mob can then "run", "walk fast" or "walk".
    }

    // Apply strongest slow aura mod to speed
    int32 slow = GetMaxNegativeAuraModifier(SPELL_AURA_MOD_DECREASE_SPEED);

    if (slow)
        AddPct(speed, slow);

    if (float minSpeedMod = (float)GetMaxPositiveAuraModifier(SPELL_AURA_MOD_MINIMUM_SPEED))
    {
        float min_speed = minSpeedMod / 100.0f;
        if (speed < min_speed && mtype != MOVE_SWIM)
            speed = min_speed;
    }

    if (mtype == MOVE_SWIM)
    {
        if (float minSwimSpeedMod = (float)GetMaxPositiveAuraModifier(SPELL_AURA_INCREASE_MIN_SWIM_SPEED))
        {
            float min_speed = minSwimSpeedMod / 100.0f;
            if (speed < min_speed)
                speed = min_speed;
        }
    }

    if (speed > roundf(speed * 100))
        speed += 0.01f;

    SetSpeed(mtype, speed, forced);
}

void Unit::SetSpeed(UnitMoveType p_MovementType, float rate, bool forced)
{
    if (fabs(rate) <= 0.00000023841858) // From client
        rate = 0.01f;

    // Update speed only on change
    if (m_speed_rate[p_MovementType] == rate)
        return;

    /// Walk speed can't be faster then run speed
    if (m_speed_rate[MOVE_WALK] > m_speed_rate[MOVE_RUN])
        m_speed_rate[MOVE_WALK] = m_speed_rate[MOVE_RUN];

    m_speed_rate[p_MovementType] = rate;

    propagateSpeedChange();

    // Don't build packets because we've got noone to send
    // them to except self, and self is not created at client.
    if (!IsInWorld())
        return;

    uint64 l_Guid = GetGUID();
    if (!forced && GetTypeId() != TYPEID_PLAYER)
    {
        WorldPacket l_Data;

        switch (p_MovementType)
        {
            case MOVE_WALK:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_WALK_SPEED, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_RUN:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_RUN_SPEED, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_RUN_BACK:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_SWIM:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_SWIM_SPEED, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_SWIM_BACK:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_TURN_RATE:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_TURN_RATE, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_FLIGHT:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_FLIGHT_BACK:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_PITCH_RATE:
            {
                l_Data.Initialize(SMSG_SPLINE_MOVE_SET_PITCH_RATE, 16 + 2 + 4);
                l_Data.appendPackGUID(l_Guid);
                l_Data << float(GetSpeed(p_MovementType));
                break;
            }
            default:
                return;
        }

        SendMessageToSet(&l_Data, true);
    }
    else
    {
        if (IsPlayer())
        {
            // register forced speed changes for WorldSession::HandleForceSpeedChangeAck
            // and do it only for real sent packets and use run for run/mounted as client expected
            ++ToPlayer()->m_forced_speed_changes[p_MovementType];

            if (!isInCombat())
                if (Pet* pet = ToPlayer()->GetPet()) ///< pet is unused
                {
                    if (Pet* pet = ToPlayer()->GetPet())
                        pet->SetSpeed(p_MovementType, m_speed_rate[p_MovementType], forced);

                    if (Creature * l_BattlePet = ToPlayer()->GetSummonedBattlePet())
                        l_BattlePet->SetSpeed(p_MovementType, m_speed_rate[p_MovementType], forced);
                }
        }

        WorldPacket l_SelfPacket;

        switch (p_MovementType)
        {
            case MOVE_WALK:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_WALK_SPEED, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_RUN:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_RUN_SPEED, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_RUN_BACK:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_RUN_BACK_SPEED, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_SWIM:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_SWIM_SPEED, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_SWIM_BACK:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_SWIM_BACK_SPEED, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_TURN_RATE:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_TURN_RATE, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_FLIGHT:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_FLIGHT_SPEED, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_FLIGHT_BACK:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_FLIGHT_BACK_SPEED, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            case MOVE_PITCH_RATE:
            {
                l_SelfPacket.Initialize(SMSG_MOVE_SET_PITCH_RATE, 16 + 2 + 4 + 4);
                l_SelfPacket.appendPackGUID(GetGUID());
                l_SelfPacket << uint32(0);
                l_SelfPacket << float(GetSpeed(p_MovementType));
                break;
            }
            default:
                return;
        }
        SendMessageToSet(&l_SelfPacket, true);
    }
}

void Unit::SendAdjustSplineDuration(float p_Scale)
{
    WorldPacket l_Data(Opcodes::SMSG_ADJUST_SPLINE_DURATION);
    l_Data.appendPackGUID(GetGUID());
    l_Data << float(p_Scale);
    SendMessageToSetInRange(&l_Data, GetMap()->GetVisibilityRange(), false);
}

void Unit::SendFlightSplineSync(float p_SplineDist)
{
    WorldPacket l_Data(Opcodes::SMSG_FLIGHT_SPLINE_SYNC);
    l_Data.appendPackGUID(GetGUID());
    l_Data << float(p_SplineDist);
    SendMessageToSetInRange(&l_Data, GetMap()->GetVisibilityRange(), false);
}

void Unit::setDeathState(DeathState s)
{
    /// Death state needs to be updated before RemoveAllAurasOnDeath() is called, to prevent entering combat
    m_deathState = s;

    if (s != ALIVE && s != JUST_RESPAWNED)
    {
        CombatStop();
        DeleteThreatList();
        getHostileRefManager().deleteReferences();

        if (IsNonMeleeSpellCasted(false))
            InterruptNonMeleeSpells(false);

        ExitVehicle();

        UnsummonAllTotems();
        RemoveAllControlled();
        RemoveAllAurasOnDeath();
    }

    if (s == JUST_DIED)
    {
        ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
        ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
        // remove aurastates allowing special moves
        ClearAllReactives();
        ClearDiminishings();
        if (IsInWorld())
        {
            // Only clear MotionMaster for entities that exists in world
            // Avoids crashes in the following conditions :
            //  * Using 'call pet' on dead pets
            //  * Using 'call stabled pet'
            //  * Logging in with dead pets
            GetMotionMaster()->Clear(false);
            GetMotionMaster()->MoveIdle();
        }

        StopMoving();
        DisableSpline();
        // without this when removing IncreaseMaxHealth aura player may stuck with 1 hp
        // do not why since in IncreaseMaxHealth currenthealth is checked
        SetHealth(0);
        SetPower(getPowerType(), 0);

        /// Clear emote state at death, prevent some wow error.
        SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);

        // Druid: Fungal Growth
        switch (GetEntry())
        {
            case 1964:
            case 47649:
                if (Unit* owner = GetOwner())
                {
                    if (owner->GetTypeId() != TYPEID_PLAYER
                        || owner->ToPlayer()->HasSpellCooldown(81291)
                        || owner->ToPlayer()->HasSpellCooldown(81283))
                        break;

                    uint32 spellId = 0;
                    if (owner->HasAura(78788))
                        spellId = 81291;
                    else if (owner->HasAura(78789))
                        spellId = 81283;

                    if (spellId)
                        owner->ToPlayer()->AddSpellCooldown(spellId, 0, sSpellMgr->GetSpellInfo(spellId)->RecoveryTime);
                }
                break;
        }

        // players in instance don't have ZoneScript, but they have InstanceScript
        if (ZoneScript* zoneScript = GetZoneScript() ? GetZoneScript() : (ZoneScript*)GetInstanceScript())
            zoneScript->OnUnitDeath(this);

    }
    else if (s == JUST_RESPAWNED)
        RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE); // clear skinnable for creature and player (at battleground)

    /// Remove pvp bonuses from items, if we have it
    if (s == JUST_DIED && ToPlayer())
        ToPlayer()->RescaleAllItemsIfNeeded(true);
}

/*########################################
########                          ########
########       AGGRO SYSTEM       ########
########                          ########
########################################*/
bool Unit::CanHaveThreatList() const
{
    // only creatures can have threat list
    if (GetTypeId() != TYPEID_UNIT)
        return false;

    // only alive units can have threat list
    if (!isAlive() || isDying())
        return false;

    // totems can not have threat list
    if (ToCreature()->isTotem())
        return false;

    // vehicles can not have threat list
    //if (ToCreature()->IsVehicle())
    //    return false;

    // summons can not have a threat list, unless they are controlled by a creature
    if (HasUnitTypeMask(UNIT_MASK_MINION | UNIT_MASK_GUARDIAN | UNIT_MASK_CONTROLABLE_GUARDIAN) && IS_PLAYER_GUID(((Pet*)this)->GetOwnerGUID()))
        return false;

    return true;
}

//======================================================================

float Unit::ApplyTotalThreatModifier(float fThreat, SpellSchoolMask schoolMask)
{
    if (!HasAuraType(SPELL_AURA_MOD_THREAT) || fThreat < 0)
        return fThreat;

    SpellSchools school = GetFirstSchoolInMask(schoolMask);

    return fThreat * m_threatModifier[school];
}

//======================================================================

void Unit::AddThreat(Unit* victim, float fThreat, SpellSchoolMask schoolMask, SpellInfo const* threatSpell)
{
    if (this->GetTypeId() == TYPEID_UNIT && this->ToCreature()->IsAIEnabled)
        this->ToCreature()->AI()->OnAddThreat(victim, fThreat, schoolMask, threatSpell);

    // Only mobs can manage threat lists
    if (CanHaveThreatList())
        m_ThreatManager.addThreat(victim, fThreat, schoolMask, threatSpell);
}

//======================================================================

void Unit::DeleteThreatList()
{
    if (CanHaveThreatList() && !m_ThreatManager.isThreatListEmpty())
        SendClearThreatListOpcode();
    m_ThreatManager.clearReferences();
}

//======================================================================

void Unit::TauntApply(Unit* taunter)
{
    ASSERT(GetTypeId() == TYPEID_UNIT);

    if (!taunter || (taunter->IsPlayer() && taunter->ToPlayer()->isGameMaster()))
        return;

    if (!CanHaveThreatList())
        return;

    Creature* creature = ToCreature();

    if (creature->HasReactState(REACT_PASSIVE))
        return;

    Unit* target = getVictim();
    if (target && target == taunter)
        return;

    SetInFront(taunter);
    if (creature->IsAIEnabled)
        creature->AI()->AttackStart(taunter);

    //m_ThreatManager.tauntApply(taunter);
}

//======================================================================

void Unit::TauntFadeOut(Unit* taunter)
{
    ASSERT(GetTypeId() == TYPEID_UNIT);

    if (!taunter || (taunter->IsPlayer() && taunter->ToPlayer()->isGameMaster()))
        return;

    if (!CanHaveThreatList())
        return;

    Creature* creature = ToCreature();

    if (creature->HasReactState(REACT_PASSIVE))
        return;

    Unit* target = getVictim();
    if (!target || target != taunter)
        return;

    if (m_ThreatManager.isThreatListEmpty())
    {
        if (creature->IsAIEnabled)
            creature->AI()->EnterEvadeMode();
        return;
    }

    target = creature->SelectVictim();  // might have more taunt auras remaining

    if (target && target != taunter)
    {
        SetInFront(target);
        if (creature->IsAIEnabled)
            creature->AI()->AttackStart(target);
    }
}

//======================================================================

Unit* Creature::SelectVictim()
{
    // function provides main threat functionality
    // next-victim-selection algorithm and evade mode are called
    // threat list sorting etc.

    Unit* target = NULL;
    // First checking if we have some taunt on us
    AuraEffectList const& tauntAuras = GetAuraEffectsByType(SPELL_AURA_MOD_TAUNT);
    if (!tauntAuras.empty())
    {
        Unit* caster = tauntAuras.back()->GetCaster();

        // The last taunt aura caster is alive an we are happy to attack him
        if (caster && caster->isAlive())
            return getVictim();
        else if (tauntAuras.size() > 1)
        {
            // We do not have last taunt aura caster but we have more taunt auras,
            // so find first available target

            // Auras are pushed_back, last caster will be on the end
            AuraEffectList::const_iterator aura = --tauntAuras.end();
            do
            {
                --aura;
                caster = (*aura)->GetCaster();
                if (caster && canSeeOrDetect(caster, true) && IsValidAttackTarget(caster) && caster->isInAccessiblePlaceFor(ToCreature()))
                {
                    target = caster;
                    break;
                }
            }
            while
                (aura != tauntAuras.begin());
        }
        else
            target = getVictim();
    }

    if (CanHaveThreatList())
    {
        if (!target && !m_ThreatManager.isThreatListEmpty())
            // No taunt aura or taunt aura caster is dead standard target selection
            target = m_ThreatManager.getHostilTarget();
    }
    else if (!HasReactState(REACT_PASSIVE))
    {
        // We have player pet probably
        target = getAttackerForHelper();
        if (!target && isSummon())
        {
            if (Unit* owner = ToTempSummon()->GetOwner())
            {
                if (owner->isInCombat())
                    target = owner->getAttackerForHelper();
                if (!target)
                {
                    for (ControlList::const_iterator itr = owner->m_Controlled.begin(); itr != owner->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->isInCombat())
                        {
                            target = (*itr)->getAttackerForHelper();
                            if (target)
                                break;
                        }
                    }
                }
            }
        }
    }
    else
        return NULL;

    if (target && _IsTargetAcceptable(target) && canCreatureAttack(target))
    {
        SetInFront(target);
        return target;
    }

    // Case where mob is being kited.
    // Mob may not be in range to attack or may have dropped target. In any case,
    //  don't evade if damage received within the last 10 seconds
    // Does not apply to world bosses to prevent kiting to cities
    if (!isWorldBoss() && !GetInstanceId())
        if (time(NULL) - GetLastDamagedTime() <= MAX_AGGRO_RESET_TIME)
            return target;

    // last case when creature must not go to evade mode:
    // it in combat but attacker not make any damage and not enter to aggro radius to have record in threat list
    // for example at owner command to pet attack some far away creature
    // Note: creature does not have targeted movement generator but has attacker in this case
    for (AttackerSet::const_iterator itr = m_attackers.begin(); itr != m_attackers.end(); ++itr)
    {
        if ((*itr) && !canCreatureAttack(*itr) && (*itr)->GetTypeId() != TYPEID_PLAYER
        && !(*itr)->ToCreature()->HasUnitTypeMask(UNIT_MASK_CONTROLABLE_GUARDIAN))
            return NULL;
    }

    // TODO: a vehicle may eat some mob, so mob should not evade
    if (GetVehicle())
        return NULL;

    // search nearby enemy before enter evade mode
    if (HasReactState(REACT_AGGRESSIVE))
    {
        target = SelectNearestTargetInAttackDistance(m_CombatDistance ? m_CombatDistance : ATTACK_DISTANCE);

        if (target && _IsTargetAcceptable(target) && canCreatureAttack(target))
            return target;
    }

    Unit::AuraEffectList const& iAuras = GetAuraEffectsByType(SPELL_AURA_MOD_INVISIBILITY);
    if (!iAuras.empty())
    {
        for (Unit::AuraEffectList::const_iterator itr = iAuras.begin(); itr != iAuras.end(); ++itr)
        {
            if ((*itr)->GetBase()->IsPermanent())
            {
                AI()->EnterEvadeMode();
                break;
            }
        }
        return NULL;
    }

    // enter in evade mode in other case
    AI()->EnterEvadeMode();

    return NULL;
}

//======================================================================
//======================================================================
//======================================================================

float Unit::ApplyEffectModifiers(SpellInfo const* spellProto, uint8 effect_index, float value) const
{
    if (Player* modOwner = GetSpellModOwner())
    {
        modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_ALL_EFFECTS, value);
        switch (effect_index)
        {
            case EFFECT_0:
                modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_EFFECT1, value);
                break;
            case EFFECT_1:
                modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_EFFECT2, value);
                break;
            case EFFECT_2:
                modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_EFFECT3, value);
                break;
            case EFFECT_3:
                modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_EFFECT4, value);
                break;
            case EFFECT_4:
                modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_EFFECT5, value);
                break;
            default:
                break;
        }
    }
    return value;
}

// function uses real base points (typically value - 1)
int32 Unit::CalculateSpellDamage(Unit const* p_Target, SpellInfo const* p_SpellProto, uint8 p_EffectIndex, int32 const* p_BasePoints, int32 itemLevel, bool p_Log /* = false */) const
{
    return p_SpellProto->Effects[p_EffectIndex].CalcValue(this, p_BasePoints, p_Target, itemLevel, p_Log);
}

int32 Unit::CalcSpellDuration(SpellInfo const* p_SpellInfo)
{
    int32 l_Combo = m_movedPlayer ? m_movedPlayer->GetPower(Powers::POWER_COMBO_POINT) : 0;

    if (l_Combo == 0 && GetEntry() == 77726) ///< Shadow Reflection
        l_Combo = GetPower(Powers::POWER_COMBO_POINT);

    int32 l_MinDur = p_SpellInfo->GetDuration();
    int32 l_MaxDur = p_SpellInfo->GetMaxDuration();

    int32 l_Dur = 0;

    if (l_Combo && l_MinDur != -1 && l_MinDur != l_MaxDur)
        l_Dur = l_MinDur + int32((l_MaxDur - l_MinDur) * l_Combo / 5);
    else
        l_Dur = l_MinDur;

    return l_Dur;
}

int32 Unit::ModSpellDuration(SpellInfo const* spellProto, Unit const* target, int32 duration, bool positive, uint32 effectMask)
{
    // don't mod permanent auras duration
    if (duration < 0)
        return duration;

    // Channeled spells does not affected by modifer duration
    if (spellProto->HasAttribute(SPELL_ATTR1_CHANNELED_1))
        return duration;

    // some auras are not affected by duration modifiers
    if (spellProto->AttributesEx7 & SPELL_ATTR7_IGNORE_DURATION_MODS)
        return duration;

    // cut duration only of negative effects
    if (!positive)
    {
        int32 mechanic = spellProto->GetSpellMechanicMaskByEffectMask(effectMask);

        int32 durationMod;
        int32 durationMod_always = 0;
        int32 durationMod_not_stack = 0;

        for (uint8 i = 1; i <= MECHANIC_ENRAGED; ++i)
        {
            if (!(mechanic & 1<<i))
                continue;
            // Find total mod value (negative bonus)
            int32 new_durationMod_always = target->GetTotalAuraModifierByMiscValue(SPELL_AURA_MECHANIC_DURATION_MOD, i);
            // Find max mod (negative bonus)
            int32 new_durationMod_not_stack = target->GetMaxNegativeAuraModifierByMiscValue(SPELL_AURA_MECHANIC_DURATION_MOD_NOT_STACK, i);
            // Check if mods applied before were weaker
            if (new_durationMod_always < durationMod_always)
                durationMod_always = new_durationMod_always;
            if (new_durationMod_not_stack < durationMod_not_stack)
                durationMod_not_stack = new_durationMod_not_stack;
        }

        // Select strongest negative mod
        if (durationMod_always > durationMod_not_stack)
            durationMod = durationMod_not_stack;
        else
            durationMod = durationMod_always;

        if (durationMod != 0)
            AddPct(duration, durationMod);

        // there are only negative mods currently
        durationMod_always = target->GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_AURA_DURATION_BY_DISPEL, spellProto->Dispel);
        durationMod_not_stack = target->GetMaxNegativeAuraModifierByMiscValue(SPELL_AURA_MOD_AURA_DURATION_BY_DISPEL_NOT_STACK, spellProto->Dispel);

        durationMod = 0;
        if (durationMod_always > durationMod_not_stack)
            durationMod += durationMod_not_stack;
        else
            durationMod += durationMod_always;

        if (durationMod != 0)
            AddPct(duration, durationMod);
    }
    else
    {
        // else positive mods here, there are no currently
        // when there will be, change GetTotalAuraModifierByMiscValue to GetTotalPositiveAuraModifierByMiscValue

        // Mixology - duration boost
        if (target->IsPlayer())
        {
            if (spellProto->SpellFamilyName == SPELLFAMILY_POTION && (
                sSpellMgr->IsSpellMemberOfSpellGroup(spellProto->Id, SPELL_GROUP_ELIXIR_BATTLE) ||
                sSpellMgr->IsSpellMemberOfSpellGroup(spellProto->Id, SPELL_GROUP_ELIXIR_GUARDIAN)))
            {
                if (target->HasAura(53042) && target->HasSpell(spellProto->Effects[0].TriggerSpell))
                    duration *= 2;
            }
        }

        // Item - Druid T13 Restoration 4P Bonus (Rejuvenation)
        if (AuraEffect* eff = GetAuraEffect(105770, 0))
        {
            if (roll_chance_i(eff->GetAmount()) && eff->IsAffectingSpell(spellProto))
                duration *= 2;
        }
    }

    // Glyphs which increase duration of selfcasted buffs
    if (target == this)
    {
        switch (spellProto->SpellFamilyName)
        {
            case SPELLFAMILY_DRUID:
            {
                if (spellProto->SpellFamilyFlags[0] & 0x100)
                {
                    // Glyph of Thorns
                    if (AuraEffect* aurEff = GetAuraEffect(57862, 0))
                        duration += aurEff->GetAmount() * MINUTE * IN_MILLISECONDS;
                }
                break;
            }
        }
    }
    return std::max(duration, 0);
}

void Unit::ModSpellCastTime(SpellInfo const* spellProto, int32 & castTime, Spell* spell)
{
    if (!spellProto || castTime < 0)
        return;

    if (spellProto->IsChanneled() && !(spellProto->AttributesEx5 & SPELL_ATTR5_HASTE_AFFECT_DURATION))
        return;

    Unit* owner = GetOwner(); ///< owner is never read 01/18/16
    // called from caster
    Player* modOwner = GetSpellModOwner();

    if (modOwner != nullptr)
        modOwner->ApplySpellMod(spellProto->Id, SPELLMOD_CASTING_TIME, castTime, spell);

    if (!((spellProto->Attributes & (SPELL_ATTR0_ABILITY|SPELL_ATTR0_TRADESPELL)) || (spellProto->HasAttribute(SPELL_ATTR3_NO_DONE_BONUS))) &&
        ((IsPlayer() && spellProto->SpellFamilyName) || GetTypeId() == TYPEID_UNIT))
    {
        if (modOwner != nullptr)
            castTime = int32(float(castTime) * modOwner->GetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED));
    }
    else if (spellProto->Attributes & SPELL_ATTR0_REQ_AMMO && !(spellProto->AttributesEx2 & SPELL_ATTR2_AUTOREPEAT_FLAG))
        castTime = int32(float(castTime) * m_modAttackSpeedPct[WeaponAttackType::RangedAttack]);
    else if (spellProto->SpellVisual[0] == 3881 && HasAura(67556)) // cooking with Chef Hat.
        castTime = 500;

}

DiminishingLevels Unit::GetDiminishing(DiminishingGroup group)
{
    for (Diminishing::iterator i = m_Diminishing.begin(); i != m_Diminishing.end(); ++i)
    {
        if (i->DRGroup != group)
            continue;

        if (!i->hitCount)
            return DIMINISHING_LEVEL_1;

        if (!i->hitTime)
            return DIMINISHING_LEVEL_1;

        // If last spell was casted more than 18 seconds ago - reset the count.
        if (i->stack == 0 && GetMSTimeDiffToNow(i->hitTime) > (18 * TimeConstants::IN_MILLISECONDS))
        {
            i->hitCount = DIMINISHING_LEVEL_1;
            i->hitTime = getMSTime();
            return DIMINISHING_LEVEL_1;
        }
        // or else increase the count.
        else
            return DiminishingLevels(i->hitCount);
    }
    return DIMINISHING_LEVEL_1;
}

void Unit::IncrDiminishing(DiminishingGroup group)
{
    if (IsPlayer() && ToPlayer()->GetCommandStatus(CHEAT_NO_DR))
        return;

    // Checking for existing in the table
    for (Diminishing::iterator i = m_Diminishing.begin(); i != m_Diminishing.end(); ++i)
    {
        if (i->DRGroup != group)
            continue;
        if (int32(i->hitCount) < GetDiminishingReturnsMaxLevel(group))
            i->hitCount += 1;
        return;
    }
    m_Diminishing.push_back(DiminishingReturn(group, getMSTime(), DIMINISHING_LEVEL_2));
}

float Unit::ApplyDiminishingToDuration(DiminishingGroup group, int32 &duration, Unit* caster, DiminishingLevels Level, int32 limitduration)
{
    if (duration == -1 || group == DIMINISHING_NONE)
        return 1.0f;

    // test pet/charm masters instead pets/charmeds
    Unit const* targetOwner = GetCharmerOrOwner();
    Unit const* casterOwner = caster->GetCharmerOrOwner();

    if (limitduration > 0 && duration > limitduration)
    {
        Unit const* target = targetOwner ? targetOwner : this;
        Unit const* source = casterOwner ? casterOwner : caster;

        if ((target->IsPlayer()
            || (target->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_ALL_DIMINISH))
            && source->IsPlayer())
            duration = limitduration;
    }

    float mod = 1.0f;

    switch (group)
    {
        case DIMINISHING_TAUNT:
        {
            if (GetTypeId() == TYPEID_UNIT && (ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_TAUNT_DIMINISH))
            {
                DiminishingLevels diminish = Level;
                switch (diminish)
                {
                    case DIMINISHING_LEVEL_1: break;
                    case DIMINISHING_LEVEL_2: mod = 0.65f; break;
                    case DIMINISHING_LEVEL_3: mod = 0.4225f; break;
                    case DIMINISHING_LEVEL_4: mod = 0.274625f; break;
                    case DIMINISHING_LEVEL_TAUNT_IMMUNE: mod = 0.0f; break;
                    default: break;
                }
            }
            break;
        }
        case DIMINISHING_AOE_KNOCKBACK:
        {
            if ((GetDiminishingReturnsGroupType(group) == DRTYPE_PLAYER && (((targetOwner ? targetOwner : this)->ToPlayer())
                                                                            || (ToCreature() && (ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_ALL_DIMINISH))))
                || GetDiminishingReturnsGroupType(group) == DRTYPE_ALL)
            {
                DiminishingLevels diminish = Level;
                switch (diminish)
                {
                    case DIMINISHING_LEVEL_1: break;
                    case DIMINISHING_LEVEL_2: mod = 0.0f; break;
                    default: break;
                }
            }
            break;
        }
        default:
        {
            if ((GetDiminishingReturnsGroupType(group) == DRTYPE_PLAYER && (((targetOwner ? targetOwner : this)->ToPlayer())
                                                                            || (ToCreature() && (ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_ALL_DIMINISH))))
                || GetDiminishingReturnsGroupType(group) == DRTYPE_ALL)
            {
                DiminishingLevels diminish = Level;
                switch (diminish)
                {
                    case DIMINISHING_LEVEL_1: break;
                    case DIMINISHING_LEVEL_2: mod = 0.5f; break;
                    case DIMINISHING_LEVEL_3: mod = 0.25f; break;
                    case DIMINISHING_LEVEL_IMMUNE: mod = 0.0f; break;
                    default: break;
                }
            }
            break;
        }
    }

    duration = int32(duration * mod);
    return mod;
}

void Unit::ApplyDiminishingAura(DiminishingGroup group, bool apply)
{
    // Checking for existing in the table
    for (Diminishing::iterator i = m_Diminishing.begin(); i != m_Diminishing.end(); ++i)
    {
        if (i->DRGroup != group)
            continue;

        if (apply)
            i->stack += 1;
        else if (i->stack)
        {
            i->stack -= 1;
            // Remember time after last aura from group removed
            if (i->stack == 0)
                i->hitTime = getMSTime();
        }
        break;
    }
}

float Unit::GetSpellMaxRangeForTarget(Unit const* target, SpellInfo const* spellInfo) const
{
    if (!spellInfo->RangeEntry)
        return 0;

    if (spellInfo->RangeEntry->maxRangeFriend == spellInfo->RangeEntry->maxRangeHostile)
        return spellInfo->GetMaxRange();

    if (!target)
        return spellInfo->RangeEntry->maxRangeFriend;

    return spellInfo->GetMaxRange(!IsHostileTo(target));
}

float Unit::GetSpellMinRangeForTarget(Unit const* target, SpellInfo const* spellInfo) const
{
    if (!spellInfo->RangeEntry)
        return 0;
    if (spellInfo->RangeEntry->minRangeFriend == spellInfo->RangeEntry->minRangeHostile)
        return spellInfo->GetMinRange();
    return spellInfo->GetMinRange(!IsHostileTo(target));
}

Unit* Unit::GetUnit(WorldObject const& object, uint64 guid)
{
    return ObjectAccessor::GetUnit(object, guid);
}

Player* Unit::GetPlayer(WorldObject const& object, uint64 guid)
{
    return ObjectAccessor::GetPlayer(object, guid);
}

Creature* Unit::GetCreature(WorldObject const& object, uint64 guid)
{
    return object.GetMap()->GetCreature(guid);
}

uint32 Unit::GetCreatureType() const
{
    if (IsPlayer())
    {
        ShapeshiftForm form = GetShapeshiftForm();
        SpellShapeshiftFormEntry const* ssEntry = sSpellShapeshiftFormStore.LookupEntry(form);
        if (ssEntry && ssEntry->creatureType > 0)
            return ssEntry->creatureType;
        else
            return CREATURE_TYPE_HUMANOID;
    }
    else
        return ToCreature()->GetCreatureTemplate()->type;
}

bool Unit::IsInDisallowedMountForm() const
{
    if (ShapeshiftForm l_Form = GetShapeshiftForm())
    {
        SpellShapeshiftFormEntry const* l_Shapeshift = sSpellShapeshiftFormStore.LookupEntry(l_Form);
        if (!l_Shapeshift)
            return true;

        if (!(l_Shapeshift->m_Flags & 0x1))
            return true;

        if (l_Form == FORM_MOONKIN)
            return false;
    }

    if (GetDisplayId() == GetNativeDisplayId())
        return false;

    CreatureDisplayInfoEntry const* l_Display = sCreatureDisplayInfoStore.LookupEntry(GetDisplayId());
    if (!l_Display)
        return true;

    CreatureDisplayInfoExtraEntry const* l_DisplayExtra = sCreatureDisplayInfoExtraStore.LookupEntry(l_Display->ExtendedDisplayInfoID);
    if (!l_DisplayExtra)
        return true;

    CreatureModelDataEntry const* l_Model = sCreatureModelDataStore.LookupEntry(l_Display->ModelId);
    ChrRacesEntry const* l_Race = sChrRacesStore.LookupEntry(l_DisplayExtra->DisplayRaceID);

    if (l_Model && !(l_Model->Flags & 0x80))
        if (l_Race && !(l_Race->Flags & 0x4))
            return true;

    return false;
}

/*#######################################
########                         ########
########       STAT SYSTEM       ########
########                         ########
#######################################*/

bool Unit::HandleStatModifier(UnitMods unitMod, UnitModifierType modifierType, float amount, bool apply)
{
    if (unitMod >= UNIT_MOD_END || modifierType >= MODIFIER_TYPE_END)
        return false;

    switch (modifierType)
    {
        case BASE_VALUE:
        case BASE_PCT_EXCLUDE_CREATE:
        case TOTAL_VALUE:
            m_auraModifiersGroup[unitMod][modifierType] += apply ? amount : -amount;
            break;
        case BASE_PCT:
        case TOTAL_PCT:
            ApplyPercentModFloatVar(m_auraModifiersGroup[unitMod][modifierType], amount, apply);
            break;
        default:
            break;
    }

    if (!CanModifyStats())
        return false;

    switch (unitMod)
    {
        case UNIT_MOD_STAT_STRENGTH:
        case UNIT_MOD_STAT_AGILITY:
        case UNIT_MOD_STAT_STAMINA:
        case UNIT_MOD_STAT_INTELLECT:
        case UNIT_MOD_STAT_SPIRIT:
            UpdateStats(GetStatByAuraGroup(unitMod));
            break;
        case UNIT_MOD_ARMOR:
            UpdateArmor();
            break;
        case UNIT_MOD_HEALTH:
            UpdateMaxHealth();
            break;
        case UNIT_MOD_MANA:
        case UNIT_MOD_RAGE:
        case UNIT_MOD_FOCUS:
        case UNIT_MOD_ENERGY:
        case UNIT_MOD_RUNE:
        case UNIT_MOD_RUNIC_POWER:
        case UNIT_MOD_CHI:
        case UNIT_MOD_BURNING_EMBERS:
        case UNIT_MOD_SOUL_SHARDS:
        case UNIT_MOD_DEMONIC_FURY:
        case UNIT_MOD_SHADOW_ORB:
            UpdateMaxPower(GetPowerTypeByAuraGroup(unitMod));
            break;
        case UNIT_MOD_RESISTANCE_HOLY:
        case UNIT_MOD_RESISTANCE_FIRE:
        case UNIT_MOD_RESISTANCE_NATURE:
        case UNIT_MOD_RESISTANCE_FROST:
        case UNIT_MOD_RESISTANCE_SHADOW:
        case UNIT_MOD_RESISTANCE_ARCANE:
            UpdateResistances(GetSpellSchoolByAuraGroup(unitMod));
            break;
        case UNIT_MOD_ATTACK_POWER:
            UpdateAttackPowerAndDamage();
            break;
        case UNIT_MOD_ATTACK_POWER_RANGED:
            UpdateAttackPowerAndDamage(true);
            break;
        case UNIT_MOD_DAMAGE_MAINHAND:
            UpdateDamagePhysical(WeaponAttackType::BaseAttack);
            break;
        case UNIT_MOD_DAMAGE_OFFHAND:
            UpdateDamagePhysical(WeaponAttackType::OffAttack);
            break;
        case UNIT_MOD_DAMAGE_RANGED:
            UpdateDamagePhysical(WeaponAttackType::RangedAttack);
            break;
        default:
            break;
    }

    return true;
}

float Unit::GetModifierValue(UnitMods unitMod, UnitModifierType modifierType) const
{
    if (unitMod >= UNIT_MOD_END || modifierType >= MODIFIER_TYPE_END)
        return 0.0f;

    if (modifierType == TOTAL_PCT && m_auraModifiersGroup[unitMod][modifierType] <= 0.0f)
        return 0.0f;

    return m_auraModifiersGroup[unitMod][modifierType];
}

float Unit::GetTotalStatValue(Stats stat, bool l_IncludeCreateStat /*= true*/) const
{
    UnitMods unitMod = UnitMods(UNIT_MOD_STAT_START + stat);

    if (m_auraModifiersGroup[unitMod][TOTAL_PCT] <= 0.0f)
        return 0.0f;

    // value = ((base_value * base_pct) + total_value) * total_pct
    float value = CalculatePct(m_auraModifiersGroup[unitMod][BASE_VALUE], std::max(m_auraModifiersGroup[unitMod][BASE_PCT_EXCLUDE_CREATE], -100.0f));
    value += GetCreateStat(stat);

    value *= m_auraModifiersGroup[unitMod][BASE_PCT];
    value += m_auraModifiersGroup[unitMod][TOTAL_VALUE];
    value *= m_auraModifiersGroup[unitMod][TOTAL_PCT];

    if (!l_IncludeCreateStat)
        value -= GetCreateStat(stat);

    return value;
}

float Unit::GetTotalAuraModValue(UnitMods unitMod) const
{
    if (unitMod >= UNIT_MOD_END)
        return 0.0f;

    if (m_auraModifiersGroup[unitMod][TOTAL_PCT] <= 0.0f)
        return 0.0f;

    float value = CalculatePct(m_auraModifiersGroup[unitMod][BASE_VALUE], std::max(m_auraModifiersGroup[unitMod][BASE_PCT_EXCLUDE_CREATE], -100.0f));

    value *= m_auraModifiersGroup[unitMod][BASE_PCT];
    value += m_auraModifiersGroup[unitMod][TOTAL_VALUE];
    value *= m_auraModifiersGroup[unitMod][TOTAL_PCT];

    return value;
}

SpellSchools Unit::GetSpellSchoolByAuraGroup(UnitMods unitMod) const
{
    SpellSchools school = SPELL_SCHOOL_NORMAL;

    switch (unitMod)
    {
        case UNIT_MOD_RESISTANCE_HOLY:     school = SPELL_SCHOOL_HOLY;          break;
        case UNIT_MOD_RESISTANCE_FIRE:     school = SPELL_SCHOOL_FIRE;          break;
        case UNIT_MOD_RESISTANCE_NATURE:   school = SPELL_SCHOOL_NATURE;        break;
        case UNIT_MOD_RESISTANCE_FROST:    school = SPELL_SCHOOL_FROST;         break;
        case UNIT_MOD_RESISTANCE_SHADOW:   school = SPELL_SCHOOL_SHADOW;        break;
        case UNIT_MOD_RESISTANCE_ARCANE:   school = SPELL_SCHOOL_ARCANE;        break;

        default:
            break;
    }

    return school;
}

Stats Unit::GetStatByAuraGroup(UnitMods unitMod) const
{
    Stats stat = STAT_STRENGTH;

    switch (unitMod)
    {
        case UNIT_MOD_STAT_STRENGTH:    stat = STAT_STRENGTH;      break;
        case UNIT_MOD_STAT_AGILITY:     stat = STAT_AGILITY;       break;
        case UNIT_MOD_STAT_STAMINA:     stat = STAT_STAMINA;       break;
        case UNIT_MOD_STAT_INTELLECT:   stat = STAT_INTELLECT;     break;
        case UNIT_MOD_STAT_SPIRIT:      stat = STAT_SPIRIT;        break;

        default:
            break;
    }

    return stat;
}

Powers Unit::GetPowerTypeByAuraGroup(UnitMods unitMod) const
{
    switch (unitMod)
    {
        case UNIT_MOD_RAGE:
            return POWER_RAGE;
        case UNIT_MOD_FOCUS:
            return POWER_FOCUS;
        case UNIT_MOD_ENERGY:
            return POWER_ENERGY;
        case UNIT_MOD_RUNE:
            return POWER_RUNES;
        case UNIT_MOD_RUNIC_POWER:
            return POWER_RUNIC_POWER;
        case UNIT_MOD_CHI:
            return POWER_CHI;
        case UNIT_MOD_HOLY_POWER:
            return POWER_HOLY_POWER;
        case UNIT_MOD_SHADOW_ORB:
            return POWER_SHADOW_ORB;
        case UNIT_MOD_BURNING_EMBERS:
            return POWER_BURNING_EMBERS;
        case UNIT_MOD_DEMONIC_FURY:
            return POWER_DEMONIC_FURY;
        case UNIT_MOD_SOUL_SHARDS:
            return POWER_SOUL_SHARDS;
        case UNIT_MOD_MANA:
        default:
            return POWER_MANA;
    }
}

float Unit::GetTotalAttackPowerValue(WeaponAttackType attType) const
{
    if (attType == WeaponAttackType::RangedAttack)
    {
        int32 ap = GetInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER);
        if (ap < 0)
            return 0.0f;
        return ap * (1.0f + GetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER));
    }
    else
    {
        int32 ap = GetInt32Value(UNIT_FIELD_ATTACK_POWER);
        if (ap < 0)
            return 0.0f;
        return ap * (1.0f + GetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER));
    }
}

float Unit::GetWeaponDamageRange(WeaponAttackType attType, WeaponDamageRange type) const
{
    if (attType == WeaponAttackType::OffAttack && !haveOffhandWeapon())
        return 0.0f;

    return m_weaponDamage[attType][type];
}

void Unit::SetLevel(uint8 lvl)
{
    SetUInt32Value(UNIT_FIELD_LEVEL, lvl);

    // group update
    if (IsPlayer() && ToPlayer()->GetGroup())
        ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_LEVEL);

    if (IsPlayer())
        sWorld->UpdateCharacterInfoLevel(ToPlayer()->GetGUIDLow(), lvl);
}

void Unit::SetHealth(uint32 val)
{
    if (getDeathState() == JUST_DIED)
        val = 0;
    else if (IsPlayer() && getDeathState() == DEAD)
        val = 1;
    else
    {
        uint32 maxHealth = GetMaxHealth();
        if (maxHealth < val)
            val = maxHealth;
    }

    SetUInt32Value(UNIT_FIELD_HEALTH, val);

    // group update
    if (Player* player = ToPlayer())
    {
        if (player->GetGroup())
            player->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_CUR_HP);

        // Hook playerScript OnModifyHealth
        if (IsPlayer())
            sScriptMgr->OnModifyHealth(this->ToPlayer(), val);
    }
    else if (Pet* l_Pet = ToCreature()->ToPet())
    {
        if (l_Pet->isControlled())
        {
            Unit* owner = GetOwner();
            if (owner && (owner->IsPlayer()) && owner->ToPlayer()->GetGroup())
                owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_CUR_HP);
            if (owner && (owner->IsPlayer()) && owner->HasAura(171393))
            {
                if (l_Pet->GetHealthPct() < 20.0f)
                    owner->CastSpell(owner, 171397, true);
                if (l_Pet->GetHealthPct() >= 20.0f && owner->GetHealthPct() >= 20.0f)
                    owner->RemoveAura(171397);
            }
        }
    }
}

void Unit::SetMaxHealth(uint32 val)
{
    if (!val)
        val = 1;

    uint32 health = GetHealth();
    SetUInt32Value(UNIT_FIELD_MAX_HEALTH, val);

    // group update
    if (IsPlayer())
    {
        if (ToPlayer()->GetGroup())
            ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_MAX_HP);
    }
    else if (Pet* pet = ToCreature()->ToPet())
    {
        if (pet->isControlled())
        {
            Unit* owner = GetOwner();
            if (owner && (owner->IsPlayer()) && owner->ToPlayer()->GetGroup())
                owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_MAX_HP);
        }
    }

    if (val < health)
        SetHealth(val);
}

Unit::PowerTypeSet Unit::GetUsablePowers() const
{
    PowerTypeSet l_Powers;
    for (uint32 l_I = 0; l_I <= sChrClassXPowerTypesStore.GetNumRows(); ++l_I)
    {
        ChrClassXPowerTypesEntry const* powerEntry = sChrClassXPowerTypesStore.LookupEntry(l_I);
        if (!powerEntry)
            continue;

        if (powerEntry->classId != getClass())
            continue;

        if (powerEntry->power == Powers::POWER_ECLIPSE)
            continue;

        l_Powers.insert(Powers(powerEntry->power));
    }

    // POWER_RUNES isn't in ChrClassesXPowerTypes.db2
    if (getClass() == CLASS_DEATH_KNIGHT)
        l_Powers.insert(POWER_RUNES);

    return l_Powers;
}

uint32 Unit::GetPowerIndex(uint32 powerId, uint32 classId) const
{

    // See CGUnit_C::GetPowerSlot
    if (GetTypeId() != TYPEID_PLAYER)
    {
        Powers l_DisplayPower = getPowerType();
        if (l_DisplayPower == (Powers)powerId)
            return 0;
        else if (powerId == Powers::POWER_ALTERNATE_POWER)
            return 1;
        else if (powerId == Powers::POWER_COMBO_POINT)
            return 2;
        else
            return Powers::MAX_POWERS;
    }

    return GetPowerIndexByClass(classId, powerId);
};

int32 Unit::GetPower(Powers p_Power) const
{
    uint32 l_PowerIndex = GetPowerIndex(p_Power, getClass());
    if (l_PowerIndex == MAX_POWERS)
        return 0;

    return m_powers[l_PowerIndex];
}

int32 Unit::GetMaxPower(Powers power) const
{
    uint32 powerIndex = GetPowerIndex(power, getClass());
    if (powerIndex == MAX_POWERS)
        return 0;

    return GetInt32Value(UNIT_FIELD_MAX_POWER + powerIndex);
}

int32 Unit::GetPowerCoeff(Powers p_PowerType) const
{
    switch (p_PowerType)
    {
        case POWER_MANA:
        case POWER_HOLY_POWER:
        case POWER_CHI:
        case POWER_ENERGY:
        case POWER_FOCUS:
        case POWER_SHADOW_ORB:
        case POWER_DEMONIC_FURY:
            return 1;
        case POWER_RAGE:
        case POWER_RUNIC_POWER:
        case POWER_BURNING_EMBERS:
            return 10;
        case POWER_SOUL_SHARDS:
        case POWER_ECLIPSE: ///< Max is 100, but can be up to 10.000 in SMSG_UPDATE_OBJECT
            return 100;
        default:
            break;
    }

    return 1;
}

namespace EclipsePower
{
    enum
    {
        CycleDuration = 40000
    };
}

void Unit::SetPower(Powers p_PowerType, int32 p_PowerValue, bool p_Regen)
{
    uint32 l_PowerIndex = GetPowerIndex(p_PowerType, getClass());

    if (l_PowerIndex == MAX_POWERS)
        return;

    int32 l_MaxPower = int32(GetMaxPower(p_PowerType));

    /// Custom case for EclipsePower, cannot be set in GetMaxPower
    if (p_PowerType == Powers::POWER_ECLIPSE)
        l_MaxPower = EclipsePower::CycleDuration;

    if (p_PowerValue > l_MaxPower)
        p_PowerValue = l_MaxPower;

    if (ToCreature() && ToCreature()->IsAIEnabled)
        ToCreature()->AI()->SetPower(p_PowerType, p_PowerValue);

    /// Hook playerScript OnModifyPower
    if (IsPlayer())
        sScriptMgr->OnModifyPower(ToPlayer(), p_PowerType, m_powers[l_PowerIndex], p_PowerValue, p_Regen, false);

    uint32 l_OldPower = m_powers[l_PowerIndex];

    m_powers[l_PowerIndex] = p_PowerValue;

    if (IsPlayer())
        sScriptMgr->OnModifyPower(ToPlayer(), p_PowerType, l_OldPower, p_PowerValue, p_Regen, true);

    uint32 l_RegenDiff = getMSTime() - m_lastRegenTime[l_PowerIndex];

    if (!p_Regen || l_RegenDiff > 2000)
        SetInt32Value(UNIT_FIELD_POWER + l_PowerIndex, p_PowerValue);

    if (IsInWorld() && (!p_Regen || l_RegenDiff > 2000))
    {
        int l_PowerCount = 1;

        WorldPacket l_Data(SMSG_POWER_UPDATE, 2 + 16 + 4 + 4 + 1);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(l_PowerCount);

        l_Data << int32(p_PowerValue);
        l_Data << uint8(p_PowerType);

        SendMessageToSet(&l_Data, IsPlayer() ? true : false);

        m_lastRegenTime[l_PowerIndex] = getMSTime();
    }

    /// Custom MoP Script
    /// Pursuit of Justice - 26023
    if (Player * l_Player = ToPlayer())
    {
        if (l_Player->HasAura(26023))
        {
            Aura* l_Aura = l_Player->GetAura(26023);
            if (l_Aura)
            {
                int32 l_HolyPower = l_Player->GetPower(POWER_HOLY_POWER) >= 3 ? 3 : l_Player->GetPower(POWER_HOLY_POWER);
                int32 l_AddValue = (5 * l_HolyPower) + l_Aura->GetSpellInfo()->Effects[EFFECT_0].BasePoints;

                l_Aura->GetEffect(0)->ChangeAmount(l_AddValue);

                Aura* l_SecondAura = l_Player->AddAura(114695, l_Player);

                if (l_SecondAura)
                    l_SecondAura->GetEffect(0)->ChangeAmount(l_AddValue);
            }
        }
        else if (l_Player->HasAura(114695))
            l_Player->RemoveAura(114695);
    }

    /// group update
    if (Player * l_Player = ToPlayer())
    {
        if (l_Player->GetGroup())
            l_Player->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_CUR_POWER);
    }
    else if (Pet * l_Pet = ToCreature()->ToPet())
    {
        if (l_Pet->isControlled())
        {
            Unit * l_Owner = GetOwner();
            if (l_Owner && (l_Owner->IsPlayer()) && l_Owner->ToPlayer()->GetGroup())
                l_Owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_CUR_POWER);
        }
    }
}

void Unit::SetMaxPower(Powers power, int32 val)
{
    uint32 powerIndex = GetPowerIndex(power, getClass());
    if (powerIndex == MAX_POWERS)
        return;

    int32 cur_power = GetPower(power);
    SetInt32Value(UNIT_FIELD_MAX_POWER + powerIndex, val);

    // group update
    if (IsPlayer())
    {
        if (ToPlayer()->GetGroup())
            ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_MAX_POWER);
    }
    else if (Pet* pet = ToCreature()->ToPet())
    {
        if (pet->isControlled())
        {
            Unit* owner = GetOwner();
            if (owner && (owner->IsPlayer()) && owner->ToPlayer()->GetGroup())
                owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_MAX_POWER);
        }
    }

    if (val < cur_power)
        SetPower(power, val);
}

int32 Unit::GetCreatePowers(Powers power) const
{
    switch (power)
    {
        case POWER_MANA:
            return GetCreateMana();
        case POWER_RAGE:
            return 1000;
        case POWER_FOCUS:
            if (IsPlayer() && getClass() == CLASS_HUNTER)
                return 100;
            return (IsPlayer() || !((Creature const*)this)->isPet() || ((Pet const*)this)->getPetType() != HUNTER_PET ? 0 : 100);
        case POWER_ENERGY:
            return IsWarlockPet() ? 200 : 100;
        case POWER_RUNIC_POWER:
            return 1000;
        case POWER_RUNES:
            return 0;
        case POWER_SHADOW_ORB:
            return (IsPlayer() && ToPlayer()->getClass() == CLASS_PRIEST && (ToPlayer()->GetSpecializationId() == SPEC_PRIEST_SHADOW) ? 3 : 0);
        case POWER_BURNING_EMBERS:
            return (IsPlayer() && ToPlayer()->getClass() == CLASS_WARLOCK && (ToPlayer()->GetSpecializationId() == SPEC_WARLOCK_DESTRUCTION) ? 40 : 0);
        case POWER_DEMONIC_FURY:
            return (IsPlayer() && ToPlayer()->getClass() == CLASS_WARLOCK && (ToPlayer()->GetSpecializationId() == SPEC_WARLOCK_DEMONOLOGY) ? 1000 : 0);
        case POWER_SOUL_SHARDS:
            return (IsPlayer() && ToPlayer()->getClass() == CLASS_WARLOCK && (ToPlayer()->GetSpecializationId() == SPEC_WARLOCK_AFFLICTION) ? 400 : 0);
        case POWER_ECLIPSE:
            return (IsPlayer() && ToPlayer()->getClass() == CLASS_DRUID && (ToPlayer()->GetSpecializationId() == SPEC_DRUID_BALANCE) ? 100 : 0);
        case POWER_HOLY_POWER:
            return (IsPlayer() && ToPlayer()->getClass() == CLASS_PALADIN ? 3 : 0);
        case POWER_HEALTH:
            return 0;
        case POWER_COMBO_POINT:
            return (IsPlayer() && (ToPlayer()->getClass() == CLASS_ROGUE || ToPlayer()->getClass() == CLASS_DRUID) ? 5 : 0);
        case POWER_CHI:
            return (IsPlayer() && ToPlayer()->getClass() == CLASS_MONK ? 4 : 0);
        default:
            break;
    }

    return 0;
}

void Unit::AddToWorld()
{
    if (!IsInWorld())
    {
        WorldObject::AddToWorld();
    }
}

void Unit::RemoveFromWorld()
{
    // cleanup
    ASSERT(GetGUID());

    if (IsInWorld())
    {
        m_duringRemoveFromWorld = true;
        if (IsVehicle() && GetMapSwitchDestination() == -1)
            GetVehicleKit()->Uninstall();

        RemoveCharmAuras();
        RemoveBindSightAuras();
        RemoveNotOwnSingleTargetAuras();

        RemoveAllGameObjects();
        RemoveAllDynObjects();
        RemoveAllAreasTrigger();

        if (GetMapSwitchDestination() == -1)
            ExitVehicle();  // Remove applied auras with SPELL_AURA_CONTROL_VEHICLE
        UnsummonAllTotems();
        RemoveAllControlled();

        RemoveAreaAurasDueToLeaveWorld();

        if (GetCharmerGUID())
        {
            sLog->outFatal(LOG_FILTER_UNITS, "Unit %u has charmer guid when removed from world", GetEntry());
            ASSERT(false);
        }

        if (Unit* owner = GetOwner())
        {
            if (owner->m_Controlled.find(this) != owner->m_Controlled.end())
            {
                sLog->outFatal(LOG_FILTER_UNITS, "Unit %u is in controlled list of %u when removed from world", GetEntry(), owner->GetEntry());
                ASSERT(false);
            }
        }

        WorldObject::RemoveFromWorld();
        m_duringRemoveFromWorld = false;
    }
}

void Unit::CleanupBeforeRemoveFromMap(bool finalCleanup)
{
    // This needs to be before RemoveFromWorld to make GetCaster() return a valid pointer on aura removal
    InterruptNonMeleeSpells(true);

    if (IsInWorld())
        RemoveFromWorld();

    ASSERT(GetGUID());

    // A unit may be in removelist and not in world, but it is still in grid
    // and may have some references during delete
    RemoveAllAuras();
    RemoveAllGameObjects();

    if (finalCleanup)
        m_cleanupDone = true;

    m_Events.KillAllEvents(false);                      // non-delatable (currently casted spells) will not deleted now but it will deleted at call in Map::RemoveAllObjectsInRemoveList
    CombatStop();
    ClearComboPoints();

    DeleteThreatList();
    getHostileRefManager().setOnlineOfflineState(false);
    GetMotionMaster()->Clear(false);                    // remove different non-standard movement generators.

    if (GetTypeId() == TYPEID_UNIT)
    {
        if (Map* l_Map = GetMap())
            l_Map->RemoveCreatureFromMoveList(ToCreature(), true);
    }
}

void Unit::CleanupsBeforeDelete(bool finalCleanup)
{
    CleanupBeforeRemoveFromMap(finalCleanup);

    if (Creature* thisCreature = ToCreature())
        if (GetTransport())
            GetTransport()->RemovePassenger(thisCreature);
}

void Unit::UpdateCharmAI()
{
    if (IsPlayer())
        return;

    if (i_disabledAI) // disabled AI must be primary AI
    {
        if (!isCharmed())
        {
            delete i_AI;
            i_AI = i_disabledAI;
            i_disabledAI = NULL;
        }
    }
    else
    {
        if (isCharmed())
        {
            i_disabledAI = i_AI;
            if (isPossessed() || IsVehicle())
                i_AI = new PossessedAI(ToCreature());
            else
                i_AI = new PetAI(ToCreature());
        }
    }
}

CharmInfo* Unit::InitCharmInfo()
{
    if (!m_charmInfo)
        m_charmInfo = new CharmInfo(this);

    return m_charmInfo;
}

void Unit::DeleteCharmInfo()
{
    if (!m_charmInfo)
        return;

    m_charmInfo->RestoreState();
    delete m_charmInfo;
    m_charmInfo = NULL;
}

CharmInfo::CharmInfo(Unit* unit)
: m_unit(unit), m_CommandState(COMMAND_FOLLOW), m_petnumber(0), m_barInit(false), m_CharmType(CharmType::CHARM_TYPE_CHARM),
  m_isCommandAttack(false), m_isAtStay(false), m_isFollowing(false), m_isReturning(false),
  m_stayX(0.0f), m_stayY(0.0f), m_stayZ(0.0f)
{
    for (uint8 i = 0; i < MAX_SPELL_CHARM; ++i)
        m_charmspells[i].SetActionAndType(0, ACT_DISABLED);

    if (m_unit->GetTypeId() == TYPEID_UNIT)
    {
        m_oldReactState = m_unit->ToCreature()->GetReactState();
        m_unit->ToCreature()->SetReactState(REACT_PASSIVE);
    }
}

CharmInfo::~CharmInfo()
{
}

void CharmInfo::RestoreState()
{
    if (m_unit->GetTypeId() == TYPEID_UNIT)
        if (Creature* creature = m_unit->ToCreature())
            creature->SetReactState(m_oldReactState);
}

void CharmInfo::InitPetActionBar()
{
    // the first 3 SpellOrActions are attack, follow and move-to
    CommandStates l_Commands[] = { COMMAND_ATTACK, COMMAND_FOLLOW, COMMAND_MOVE_TO };
    for (uint32 i = 0; i < ACTION_BAR_INDEX_PET_SPELL_START - ACTION_BAR_INDEX_START; ++i)
        SetActionBar(ACTION_BAR_INDEX_START + i, l_Commands[i], ACT_COMMAND);

    // middle 4 SpellOrActions are spells/special attacks/abilities
    for (uint32 i = 0; i < ACTION_BAR_INDEX_PET_SPELL_END-ACTION_BAR_INDEX_PET_SPELL_START; ++i)
        SetActionBar(ACTION_BAR_INDEX_PET_SPELL_START + i, 0, ACT_PASSIVE);

    // last 3 SpellOrActions are reactions
    ReactStates l_Reactions[] = { REACT_HELPER, REACT_DEFENSIVE, REACT_PASSIVE };
    for (uint32 i = 0; i < ACTION_BAR_INDEX_END - ACTION_BAR_INDEX_PET_SPELL_END; ++i)
        SetActionBar(ACTION_BAR_INDEX_PET_SPELL_END + i, l_Reactions[i], ACT_REACTION);

    m_CharmType = CharmType::CHARM_TYPE_CHARM;
}

void CharmInfo::InitEmptyActionBar(bool withAttack)
{
    if (withAttack)
        SetActionBar(ACTION_BAR_INDEX_START, COMMAND_ATTACK, ACT_COMMAND);
    else
        SetActionBar(ACTION_BAR_INDEX_START, 0, ACT_PASSIVE);
    for (uint32 x = ACTION_BAR_INDEX_START+1; x < ACTION_BAR_INDEX_END; ++x)
        SetActionBar(x, 0, ACT_PASSIVE);

    m_CharmType = CharmType::CHARM_TYPE_CHARM;
}

void CharmInfo::InitPossessCreateSpells()
{
    InitEmptyActionBar();
    if (m_unit->GetTypeId() == TYPEID_UNIT)
    {
        for (uint32 i = 0; i < CREATURE_MAX_SPELLS; ++i)
        {
            uint32 spellId = m_unit->ToCreature()->m_spells[i];
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
            if (spellInfo && !(spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD))
            {
                if (spellInfo->IsPassive())
                    m_unit->CastSpell(m_unit, spellInfo, true);
                else
                    AddSpellToActionBar(spellInfo, ACT_PASSIVE);
            }
        }
    }

    m_CharmType = CharmType::CHARM_TYPE_POSSESS;
}

void CharmInfo::InitCharmCreateSpells()
{
    if (m_unit->IsPlayer())                // charmed players don't have spells
    {
        InitEmptyActionBar();
        return;
    }

    InitPetActionBar();

    for (uint32 x = 0; x < MAX_SPELL_CHARM; ++x)
    {
        uint32 spellId = m_unit->ToCreature()->m_spells[x];
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);

        if (!spellInfo || spellInfo->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD)
        {
            m_charmspells[x].SetActionAndType(spellId, ACT_DISABLED);
            continue;
        }

        if (spellInfo->IsPassive())
        {
            m_unit->CastSpell(m_unit, spellInfo, true);
            m_charmspells[x].SetActionAndType(spellId, ACT_PASSIVE);
        }
        else
        {
            m_charmspells[x].SetActionAndType(spellId, ACT_DISABLED);

            ActiveStates newstate = ACT_PASSIVE;

            if (!spellInfo->IsAutocastable())
                newstate = ACT_PASSIVE;
            else
            {
                if (spellInfo->NeedsExplicitUnitTarget())
                {
                    newstate = ACT_ENABLED;
                    ToggleCreatureAutocast(spellInfo, true);
                }
                else
                    newstate = ACT_DISABLED;
            }

            AddSpellToActionBar(spellInfo, newstate);
        }
    }
}

bool CharmInfo::AddSpellToActionBar(SpellInfo const* spellInfo, ActiveStates newstate)
{
    uint32 spell_id = spellInfo->Id;
    uint32 first_id = spellInfo->GetFirstRankSpell()->Id;

    // new spell rank can be already listed
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (uint32 action = PetActionBar[i].GetAction())
        {
            if (PetActionBar[i].IsActionBarForSpell() && sSpellMgr->GetFirstSpellInChain(action) == first_id)
            {
                PetActionBar[i].SetAction(spell_id);
                return true;
            }
        }
    }

    // or use empty slot in other case
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (!PetActionBar[i].GetAction() && PetActionBar[i].IsActionBarForSpell())
        {
            SetActionBar(i, spell_id, newstate == ACT_DECIDE ? spellInfo->IsAutocastable() ? ACT_DISABLED : ACT_PASSIVE : newstate);
            return true;
        }
    }
    return false;
}

bool CharmInfo::RemoveSpellFromActionBar(uint32 spell_id)
{
    uint32 first_id = sSpellMgr->GetFirstSpellInChain(spell_id);

    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (uint32 action = PetActionBar[i].GetAction())
        {
            if (PetActionBar[i].IsActionBarForSpell() && sSpellMgr->GetFirstSpellInChain(action) == first_id)
            {
                SetActionBar(i, 0, ACT_PASSIVE);
                return true;
            }
        }
    }

    return false;
}

void CharmInfo::ToggleCreatureAutocast(SpellInfo const* spellInfo, bool apply)
{
    if (spellInfo->IsPassive())
        return;

    for (uint32 x = 0; x < MAX_SPELL_CHARM; ++x)
        if (spellInfo->Id == m_charmspells[x].GetAction())
            m_charmspells[x].SetType(apply ? ACT_ENABLED : ACT_DISABLED);
}

void CharmInfo::SetPetNumber(uint32 petnumber, bool statwindow)
{
    m_petnumber = petnumber;
    if (statwindow)
        m_unit->SetUInt32Value(UNIT_FIELD_PET_NUMBER, m_petnumber);
    else
        m_unit->SetUInt32Value(UNIT_FIELD_PET_NUMBER, 0);
}

void CharmInfo::LoadPetActionBar(const std::string& data)
{
    InitPetActionBar();

    Tokenizer tokens(data, ' ');

    if (tokens.size() != (ACTION_BAR_INDEX_END-ACTION_BAR_INDEX_START) * 2)
        return;                                             // non critical, will reset to default

    uint8 index = ACTION_BAR_INDEX_START;
    Tokenizer::const_iterator iter = tokens.begin();
    for (; index < ACTION_BAR_INDEX_END; ++iter, ++index)
    {
        // use unsigned cast to avoid sign negative format use at long-> ActiveStates (int) conversion
        ActiveStates type  = ActiveStates(atol(*iter));
        ++iter;
        uint32 action = uint32(atol(*iter));

        PetActionBar[index].SetActionAndType(action, type);

        // check correctness
        if (PetActionBar[index].IsActionBarForSpell())
        {
            SpellInfo const* spelInfo = sSpellMgr->GetSpellInfo(PetActionBar[index].GetAction());
            if (!spelInfo)
                SetActionBar(index, 0, ACT_PASSIVE);
            else if (!spelInfo->IsAutocastable())
                SetActionBar(index, PetActionBar[index].GetAction(), ACT_PASSIVE);
        }
    }
}

void CharmInfo::BuildActionBar(WorldPacket* data)
{
    for (uint32 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(PetActionBar[i].packedData & 0x00FFFFFF);
        if (!spellInfo)
        {
            // Pet actions are not spell ID
            *data << uint32(PetActionBar[i].packedData);
            continue;
        }

        // This prevent to add spells with no cooldown - cheating !
        if (!spellInfo->CannotBeAddedToCharm())
        {
            *data << uint32(0x00FFFFFF);
            continue;
        }

        *data << uint32(PetActionBar[i].packedData);
    }
}

void CharmInfo::SetSpellAutocast(SpellInfo const* spellInfo, bool state)
{
    for (uint8 i = 0; i < MAX_UNIT_ACTION_BAR_INDEX; ++i)
    {
        if (spellInfo->Id == PetActionBar[i].GetAction() && PetActionBar[i].IsActionBarForSpell())
        {
            PetActionBar[i].SetType(state ? ACT_ENABLED : ACT_DISABLED);
            break;
        }
    }
}

bool Unit::isFrozen() const
{
    return HasAuraState(AURA_STATE_FROZEN);
}

struct ProcTriggeredData
{
    ProcTriggeredData(Aura* _aura)
        : aura(_aura)
    {
        effMask = 0;
        spellProcEvent = NULL;
    }
    SpellProcEventEntry const* spellProcEvent;
    Aura* aura;
    uint32 effMask;
};

typedef std::list< ProcTriggeredData > ProcTriggeredList;

// List of auras that CAN be trigger but may not exist in spell_proc_event
// in most case need for drop charges
// in some types of aura need do additional check
// for example SPELL_AURA_MECHANIC_IMMUNITY - need check for mechanic
bool InitTriggerAuraData()
{
    for (uint16 i = 0; i < TOTAL_AURAS; ++i)
    {
        isTriggerAura[i] = false;
        isNonTriggerAura[i] = false;
        isAlwaysTriggeredAura[i] = false;
    }
    isTriggerAura[SPELL_AURA_PROC_ON_POWER_AMOUNT] = true;
    isTriggerAura[SPELL_AURA_PROC_ON_POWER_AMOUNT_2] = true;
    isTriggerAura[SPELL_AURA_DUMMY] = true;
    isTriggerAura[SPELL_AURA_PERIODIC_DUMMY] = true;
    isTriggerAura[SPELL_AURA_MOD_CONFUSE] = true;
    isTriggerAura[SPELL_AURA_MOD_THREAT] = true;
    isTriggerAura[SPELL_AURA_MOD_STUN] = true; // Aura does not have charges but needs to be removed on trigger
    isTriggerAura[SPELL_AURA_MOD_DAMAGE_DONE] = true;
    isTriggerAura[SPELL_AURA_MOD_DAMAGE_TAKEN] = true;
    isTriggerAura[SPELL_AURA_MOD_RESISTANCE] = true;
    isTriggerAura[SPELL_AURA_MOD_STEALTH] = true;
    isTriggerAura[SPELL_AURA_MOD_FEAR] = true; // Aura does not have charges but needs to be removed on trigger
    isTriggerAura[SPELL_AURA_MOD_FEAR_2] = true; // Aura does not have charges but needs to be removed on trigger
    isTriggerAura[SPELL_AURA_MOD_ROOT] = true;
    isTriggerAura[SPELL_AURA_MOD_ROOT_2] = true;
    isTriggerAura[SPELL_AURA_TRANSFORM] = true;
    isTriggerAura[SPELL_AURA_REFLECT_SPELLS] = true;
    isTriggerAura[SPELL_AURA_DAMAGE_IMMUNITY] = true;
    isTriggerAura[SPELL_AURA_PROC_TRIGGER_SPELL] = true;
    isTriggerAura[SPELL_AURA_PROC_TRIGGER_DAMAGE] = true;
    isTriggerAura[SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK] = true;
    isTriggerAura[SPELL_AURA_SCHOOL_ABSORB] = true; // Savage Defense untested
    isTriggerAura[SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT] = true;
    isTriggerAura[SPELL_AURA_MOD_POWER_COST_SCHOOL] = true;
    isTriggerAura[SPELL_AURA_REFLECT_SPELLS_SCHOOL] = true;
    isTriggerAura[SPELL_AURA_MECHANIC_IMMUNITY] = true;
    isTriggerAura[SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN] = true;
    isTriggerAura[SPELL_AURA_SPELL_MAGNET] = true;
    isTriggerAura[SPELL_AURA_MOD_ATTACK_POWER] = true;
    isTriggerAura[SPELL_AURA_ADD_CASTER_HIT_TRIGGER] = true;
    isTriggerAura[SPELL_AURA_OVERRIDE_CLASS_SCRIPTS] = true;
    isTriggerAura[SPELL_AURA_MOD_MECHANIC_RESISTANCE] = true;
    isTriggerAura[SPELL_AURA_RANGED_ATTACK_POWER_ATTACKER_BONUS] = true;
    isTriggerAura[SPELL_AURA_MOD_MELEE_HASTE] = true;
    isTriggerAura[SPELL_AURA_MOD_MELEE_HASTE_3] = true;
    isTriggerAura[SPELL_AURA_MOD_ATTACKER_MELEE_HIT_CHANCE] = true;
    isTriggerAura[SPELL_AURA_RAID_PROC_FROM_CHARGE] = true;
    isTriggerAura[SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE] = true;
    isTriggerAura[SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE] = true;
    isTriggerAura[SPELL_AURA_MOD_DAMAGE_FROM_CASTER] = true;
    isTriggerAura[SPELL_AURA_MOD_SPELL_CRIT_CHANCE] = true;
    isTriggerAura[SPELL_AURA_ABILITY_IGNORE_AURASTATE] = true;
    isTriggerAura[SPELL_AURA_PROC_TRIGGER_SPELL_COPY] = true;
    isTriggerAura[SPELL_AURA_HASTE_SPELLS] = true;

    isNonTriggerAura[SPELL_AURA_MOD_POWER_REGEN] = true;
    isNonTriggerAura[SPELL_AURA_REDUCE_PUSHBACK] = true;

    isAlwaysTriggeredAura[SPELL_AURA_OVERRIDE_CLASS_SCRIPTS] = true;
    isAlwaysTriggeredAura[SPELL_AURA_MOD_FEAR] = true;
    isAlwaysTriggeredAura[SPELL_AURA_MOD_FEAR_2] = true;
    isAlwaysTriggeredAura[SPELL_AURA_MOD_ROOT] = true;
    isAlwaysTriggeredAura[SPELL_AURA_MOD_ROOT_2] = true;
    isAlwaysTriggeredAura[SPELL_AURA_MOD_STUN] = true;
    isAlwaysTriggeredAura[SPELL_AURA_TRANSFORM] = true;
    isAlwaysTriggeredAura[SPELL_AURA_SPELL_MAGNET] = true;
    isAlwaysTriggeredAura[SPELL_AURA_SCHOOL_ABSORB] = true;
    isAlwaysTriggeredAura[SPELL_AURA_MOD_STEALTH] = true;

    return true;
}

uint32 createProcExtendMask(SpellNonMeleeDamage* damageInfo, SpellMissInfo missCondition)
{
    uint32 procEx = PROC_EX_NONE;
    // Check victim state
    if (missCondition != SPELL_MISS_NONE)
        switch (missCondition)
        {
            case SPELL_MISS_MISS:    procEx|=PROC_EX_MISS;   break;
            case SPELL_MISS_RESIST:  procEx|=PROC_EX_RESIST; break;
            case SPELL_MISS_DODGE:   procEx|=PROC_EX_DODGE;  break;
            case SPELL_MISS_PARRY:   procEx|=PROC_EX_PARRY;  break;
            case SPELL_MISS_BLOCK:   procEx|=PROC_EX_BLOCK;  break;
            case SPELL_MISS_EVADE:   procEx|=PROC_EX_EVADE;  break;
            case SPELL_MISS_IMMUNE:  procEx|=PROC_EX_IMMUNE; break;
            case SPELL_MISS_IMMUNE2: procEx|=PROC_EX_IMMUNE; break;
            case SPELL_MISS_DEFLECT: procEx|=PROC_EX_DEFLECT;break;
            case SPELL_MISS_ABSORB:  procEx|=PROC_EX_ABSORB; break;
            case SPELL_MISS_REFLECT: procEx|=PROC_EX_REFLECT;break;
            default:
                break;
        }
    else
    {
        // On block
        if (damageInfo->blocked)
            procEx|=PROC_EX_BLOCK;
        // On absorb
        if (damageInfo->absorb)
            procEx|=PROC_EX_ABSORB;
        // On crit
        if (damageInfo->HitInfo & SPELL_HIT_TYPE_CRIT)
            procEx|=PROC_EX_CRITICAL_HIT;
        else
            procEx|=PROC_EX_NORMAL_HIT;
    }
    return procEx;
}

void Unit::ProcDamageAndSpellFor(bool isVictim, Unit* target, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, SpellInfo const* procSpell, uint32 damage, uint32 absorb /*= 0*/, SpellInfo const* procAura /*= NULL*/, AuraEffect const* ownerAuraEffect /*= NULL*/)
{
    // Player is loaded now - do not allow passive spell casts to proc
    if (IsPlayer() && ToPlayer()->GetSession()->PlayerLoading())
        return;
    /// Multistrike...
    int32 l_TotalDamage = damage + absorb;
    uint8 l_TotalMultistrike = 0;
    if (!(procExtra & PROC_EX_INTERNAL_MULTISTRIKE) && !(procFlag & PROC_FLAG_KILL) &&
        l_TotalDamage && target && GetSpellModOwner() && !(procSpell && (!procSpell->IsPositive() && !procSpell->IsHealingSpell()) && target->GetGUID() == GetGUID()))
        {
        /// ...grants your spells, abilities, and auto-attacks...
        if (procFlag & MULTISTRIKE_DONE_HIT_PROC_FLAG_MASK)
            l_TotalMultistrike = ProcMultistrike(procSpell, target, procFlag, procExtra, l_TotalDamage, attType, procAura, ownerAuraEffect);
        }

    // For melee/ranged based attack need update skills and set some Aura states if victim present
    if (procFlag & MELEE_BASED_TRIGGER_MASK && target)
    {
        // If exist crit/parry/dodge/block need update aura state (for victim and attacker)
        if (procExtra & (PROC_EX_CRITICAL_HIT|PROC_EX_PARRY|PROC_EX_DODGE|PROC_EX_BLOCK))
        {
            // for victim
            if (isVictim)
            {
                // if victim and dodge attack
                if (procExtra & PROC_EX_DODGE)
                {
                    // Update AURA_STATE on dodge
                    if (getClass() != CLASS_ROGUE) // skip Rogue Riposte
                    {
                        ModifyAuraState(AURA_STATE_DEFENSE, true);
                        StartReactiveTimer(REACTIVE_DEFENSE);
                    }
                }
                // if victim and parry attack
                if (procExtra & PROC_EX_PARRY)
                {
                    // For Hunters only Counterattack (skip Mongoose bite)
                    if (getClass() == CLASS_HUNTER)
                    {
                        ModifyAuraState(AURA_STATE_HUNTER_PARRY, true);
                        StartReactiveTimer(REACTIVE_HUNTER_PARRY);
                    }
                    else
                    {
                        ModifyAuraState(AURA_STATE_DEFENSE, true);
                        StartReactiveTimer(REACTIVE_DEFENSE);
                    }
                }
                // if and victim block attack
                if (procExtra & PROC_EX_BLOCK)
                {
                    ModifyAuraState(AURA_STATE_DEFENSE, true);
                    StartReactiveTimer(REACTIVE_DEFENSE);
                }
            }
            else // For attacker
            {
                // Overpower on victim dodge
                if (procExtra & PROC_EX_DODGE && getClass() == CLASS_WARRIOR)
                {
                    AddComboPoints(1);
                    StartReactiveTimer(REACTIVE_OVERPOWER);
                    CastSpell(this, 60503, true);
                }
            }
        }
    }

    /// Leader of the Pack
    if (!isVictim && IsPlayer() && ToPlayer()->getClass() == CLASS_DRUID && HasAura(17007) && (procExtra & PROC_EX_CRITICAL_HIT) &&
        (attType == WeaponAttackType::BaseAttack || (procSpell && procSpell->GetSchoolMask() == SPELL_SCHOOL_MASK_NORMAL)))
    {
        if (!ToPlayer()->HasSpellCooldown(68285))
        {
            CastSpell(this, 68285, true); // Heal himself
            ToPlayer()->AddSpellCooldown(68285, 0, 6 * IN_MILLISECONDS); // 6s ICD
        }
    }

    /// Brain Freeze
    if (procSpell && procSpell->Id == 116 && IsPlayer() && ToPlayer()->getClass() == CLASS_MAGE && ToPlayer()->GetSpecializationId() == SPEC_MAGE_FROST && HasAura(44549) && !(procExtra & PROC_EX_INTERNAL_MULTISTRIKE))
    {
        int32 l_Chance = 10;
        l_Chance += (l_TotalMultistrike * 15);
        if (roll_chance_i(l_Chance))
            CastSpell(this, 57761, true);
    }

    /// Runic Strikes
    if ((procExtra & PROC_EX_INTERNAL_MULTISTRIKE) && IsPlayer() && ToPlayer()->getClass() == CLASS_DEATH_KNIGHT && ToPlayer()->IsTwoHandUsed() && HasAura(165394) && procSpell == nullptr)
        CastSpell(this, 163948, true);

    /// Plaguebearer
    if (procSpell && target && IsPlayer() && ToPlayer()->getClass() == CLASS_DEATH_KNIGHT  && (procSpell->Id == 47541 || procSpell->Id == 49143) && HasAura(161497))
    {
        if (Aura* l_BloodPlague = target->GetAura(55078, GetGUID()))
            l_BloodPlague->SetDuration(l_BloodPlague->GetDuration() + 4000);

        if (Aura* l_FrostFever = target->GetAura(55095, GetGUID()))
            l_FrostFever->SetDuration(l_FrostFever->GetDuration() + 4000);

        if (HasAura(152281))
            CastSpell(target, 155159);
    }

    // Dematerialize
    if (target && !isVictim && target->IsPlayer() && target->ToPlayer()->getClass() == CLASS_MONK && target->HasAura(122464) && procSpell && procSpell->GetAllEffectsMechanicMask() & (1 << MECHANIC_STUN))
    {
        if (!target->ToPlayer()->HasSpellCooldown(122465))
        {
            target->CastSpell(target, 122465, true);
            target->ToPlayer()->AddSpellCooldown(122465, 0, 10 * IN_MILLISECONDS);
        }
    }

    /// Death Siphon
    if (procSpell && procSpell->Id == 108196 && !isVictim)
    {
        int32 bp = l_TotalDamage * 4;
        CastCustomSpell(this, 116783, &bp, NULL, NULL, true);
    }

    /// Words of Mending - 152117
    if (IsPlayer() && ToPlayer()->getClass() == CLASS_PRIEST && ToPlayer()->GetSpecializationId() != SPEC_PRIEST_SHADOW && HasAura(152117) && target && procSpell && (procSpell->IsHealingSpell() || procSpell->IsShieldingSpell()) && procSpell->Id != SPELL_PLAYER_LIFE_STEAL && procSpell->Id != 77489)
    {
        if (procFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS)
        {
            if (HasAura(155363))
            {
                if (HasAura(155362))
                    RemoveAura(155362);
                else
                {
                    CastSpell(target, 33076, true);
                    RemoveAura(155363);
                }
            }
        }
    }

    /// Revealing Strike - 84617
    if (IsPlayer() && ToPlayer()->getClass() == CLASS_ROGUE && target && target->HasAura(84617, GetGUID()) && procSpell && procSpell->Id == 1752)
    {
        if (roll_chance_i(25))
            AddComboPoints(1);
    }

    /// Hack Fix Ice Floes - Drop charges
    if (IsPlayer() && !isVictim && ToPlayer()->getClass() == CLASS_MAGE && HasAura(108839) && procSpell && procSpell->Id != 108839 && !(procExtra & PROC_EX_INTERNAL_DOT) &&
        ((procSpell->CastTimeEntry && procSpell->CastTimeEntry->CastTime > 0 && procSpell->CastTimeEntry->CastTime < 4000)
            || (procSpell->DurationEntry && procSpell->DurationEntry->Duration[0] > 0 && procSpell->DurationEntry->Duration[0] < 4000 && procSpell->AttributesEx & SPELL_ATTR1_CHANNELED_2)))
    {
        if (AuraApplication* aura = GetAuraApplication(108839, GetGUID()))
            aura->GetBase()->DropStack();
    }

    // Fix Drop charge for Killing Machine
    if (IsPlayer() && HasAura(51124) && getClass() == CLASS_DEATH_KNIGHT && procSpell && (procSpell->Id == 49020 || procSpell->Id == 49143 || procSpell->Id == 66198 || procSpell->Id == 66196))
    {
        if (!haveOffhandWeapon())
            RemoveAura(51124);
        else if (procSpell->Id == 66198 || procSpell->Id == 66196)
            RemoveAura(51124);
    }

    // Fix Drop charge for Blindsight
    if (IsPlayer() && HasAura(121152) && getClass() == CLASS_ROGUE && procSpell && procSpell->Id == 111240)
        RemoveAura(121153);

    /// Hack Fix Immolate - Critical strikes generate burning embers
    if (IsPlayer() && ToPlayer()->getClass() == CLASS_WARLOCK && ToPlayer()->GetSpecializationId() == SPEC_WARLOCK_DESTRUCTION && procSpell && (procSpell->Id == 348 || procSpell->Id == 157736) && (procExtra & PROC_EX_CRITICAL_HIT))
        SetPower(POWER_BURNING_EMBERS, GetPower(POWER_BURNING_EMBERS) + 1);

    // Cast Shadowy Apparitions when Shadow Word : Pain is crit
    if (IsPlayer() && procSpell && procSpell->Id == 589 && HasAura(78203) && procExtra & PROC_EX_CRITICAL_HIT)
    {
        SendPlaySpellVisual(33584, target, 6.0f, 0.0f, Position());
        CastSpell(target, 147193, true);
    }

    Unit* actor = isVictim ? target : this;
    Unit* actionTarget = !isVictim ? target : this;

    DamageInfo damageInfo = DamageInfo(actor, actionTarget, damage, procSpell, procSpell ? SpellSchoolMask(procSpell->SchoolMask) : SPELL_SCHOOL_MASK_NORMAL, SPELL_DIRECT_DAMAGE);
    HealInfo healInfo = HealInfo(actor, actionTarget, damage, procSpell, procSpell ? SpellSchoolMask(procSpell->SchoolMask) : SPELL_SCHOOL_MASK_NORMAL);
    ProcEventInfo eventInfo = ProcEventInfo(actor, actionTarget, target, procFlag, 0, 0, procExtra, NULL, &damageInfo, &healInfo);

    damageInfo.ModifyAbsorb(absorb);

    uint32 now = getMSTime();

    ProcTriggeredList procTriggered;
    // Fill procTriggered list
    for (AuraApplicationMap::const_iterator itr = GetAppliedAuras().begin(); itr!= GetAppliedAuras().end(); ++itr)
    {
        // Do not allow auras to proc from effect triggered by itself
        if (procAura && procAura->Id == itr->first)
            continue;
        ProcTriggeredData triggerData(itr->second->GetBase());

        // Defensive procs are active on absorbs (so absorption effects are not a hindrance)
        bool active = (damage + absorb) || (procExtra & PROC_EX_BLOCK && isVictim);
        if (isVictim)
            procExtra &= ~PROC_EX_INTERNAL_REQ_FAMILY;

        // only auras that has triggered spell should proc from fully absorbed damage
        SpellInfo const* spellProto = itr->second->GetBase()->GetSpellInfo();
        if ((procExtra & PROC_EX_ABSORB && isVictim) || (procFlag & PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG))
        {
            bool triggerSpell = false;
            for (int i = 0; i < spellProto->EffectCount; ++i)
                if (spellProto->Effects[i].TriggerSpell)
                    triggerSpell = true;

            if ((damage + absorb) || triggerSpell)
                active = true;
        }

        if (spellProto->Id == 77606 && procSpell && procSpell->IsCanBeStolen())  ///< Dark Simulacrum
            active = true;

        // Custom MoP Script
        // Breath of Fire DoT shoudn't remove Breath of Fire disorientation - Hack Fix
        if (procSpell && procSpell->Id == 123725 && itr->first == 123393)
            continue;

        /// Custom WoD Script
        /// Ruthlessness can proc just from finishing spells
        if (itr->first == 14161 && (!procSpell || (procSpell && procSpell->Id != 2098 && procSpell->Id != 408 && procSpell->Id != 26679 && procSpell->Id != 1943 && procSpell->Id != 121411)))
            continue;

        /// Item - Druid T17 Restoration 4P Bonus - 167714
        if (spellProto->Id == 167714)
            active = true;

        if (!IsTriggeredAtSpellProcEvent(target, triggerData.aura, procSpell, procFlag, procExtra, attType, isVictim, active, triggerData.spellProcEvent))
            continue;

        // do checks using conditions table
        if (!sConditionMgr->IsObjectMeetingNotGroupedConditions(CONDITION_SOURCE_TYPE_SPELL_PROC, spellProto->Id, eventInfo.GetActor(), eventInfo.GetActionTarget()))
            continue;

        // AuraScript Hook
        if (!triggerData.aura->CallScriptCheckProcHandlers(itr->second, eventInfo))
            continue;

        bool procSuccess = RollProcResult(target, triggerData.aura, attType, isVictim, triggerData.spellProcEvent);
        triggerData.aura->SetLastProcAttemptTime(now);
        if (!procSuccess)
            continue;

        // Triggered spells not triggering additional spells
        bool triggered = !(spellProto->AttributesEx3 & SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED) ?
            (procExtra & PROC_EX_INTERNAL_TRIGGERED && !(procFlag & PROC_FLAG_DONE_TRAP_ACTIVATION)) : false;

        for (uint8 i = 0; i < itr->second->GetEffectCount(); ++i)
        {
            if (itr->second->HasEffect(i))
            {
                AuraEffect* aurEff = itr->second->GetBase()->GetEffect(i);
                // Skip this auras
                if (isNonTriggerAura[aurEff->GetAuraType()])
                    continue;
                // If not trigger by default and spellProcEvent == NULL - skip
                if (!isTriggerAura[aurEff->GetAuraType()] && triggerData.spellProcEvent == NULL)
                    continue;
                // Some spells must always trigger
                if (!triggered || isAlwaysTriggeredAura[aurEff->GetAuraType()])
                    triggerData.effMask |= 1<<i;
            }
        }
        if (triggerData.effMask)
            procTriggered.push_front(triggerData);
    }

    // Nothing found
    if (procTriggered.empty())
        return;

    // Note: must SetCantProc(false) before return
    if (procExtra & (PROC_EX_INTERNAL_TRIGGERED | PROC_EX_INTERNAL_CANT_PROC))
        SetCantProc(true);

    // Handle effects proceed this time
    for (ProcTriggeredList::const_iterator i = procTriggered.begin(); i != procTriggered.end(); ++i)
    {
        // look for aura in auras list, it may be removed while proc event processing
        if (i->aura->IsRemoved())
            continue;

        bool useCharges  = i->aura->IsUsingCharges();
        // no more charges to use, prevent proc
        if (useCharges && !i->aura->GetCharges())
            continue;

        bool takeCharges = false;
        SpellInfo const* spellInfo = i->aura->GetSpellInfo();

        AuraApplication const* aurApp = i->aura->GetApplicationOfTarget(GetGUID());

        bool prepare = i->aura->CallScriptPrepareProcHandlers(aurApp, eventInfo);

        // For players set spell cooldown if need
        uint32 cooldown = spellInfo->InternalCooldown;
        if (prepare && IsPlayer() && i->spellProcEvent && i->spellProcEvent->cooldown)
            cooldown = i->spellProcEvent->cooldown * IN_MILLISECONDS;

        i->aura->SetLastProcSuccessTime(now);

        // Hack Fix : Stealth is not removed on absorb damage
        if (spellInfo->HasAura(SPELL_AURA_MOD_STEALTH) && procExtra & PROC_EX_ABSORB && isVictim)
            useCharges = false;

        /// Hack Fix : Subterfuge aura can't be removed by any action
        if (spellInfo->Id == 115191)
        {
            if (((!isVictim && procExtra & PROC_EX_NORMAL_HIT) || isVictim || procExtra & PROC_EX_INTERNAL_DOT) && !HasAura(115192) && !HasAura(131361) && !(procExtra & PROC_EX_ABSORB) && !(procExtra & PROC_EX_INTERNAL_MULTISTRIKE) && ((procSpell && procSpell->Id != 146347) || !procSpell))
            {
                /// Doesn't call Subterfuge from Drain Life and Touch of the Grave spells
                if ((procSpell && procSpell->Id != 146347 && procSpell->Id != 127802) || !procSpell)
                    CastSpell(this, 115192, true);
            }
        }

        /// Hack Fix for Subterfuge with Drain Life and Touch of the Grave
        if (spellInfo->Id == 115191 && procSpell && (procSpell->Id == 146347 || procSpell->Id == 127802))
            useCharges = false;

        // Hack Fix - Vanish :  If rogue has vanish aura stealth is not removed on periodic damage
        if ((spellInfo->Id == 115191 || spellInfo->Id == 1784) && HasAura(131361) && isVictim)
            useCharges = false;

        // Note: must SetCantProc(false) before return
        if (spellInfo->AttributesEx3 & SPELL_ATTR3_DISABLE_PROC)
            SetCantProc(true);

        i->aura->CallScriptProcHandlers(aurApp, eventInfo);

        // This bool is needed till separate aura effect procs are still here
        bool handled = false;
        if (HandleAuraProc(target, damage, i->aura, procSpell, procFlag, procExtra, cooldown, &handled))
            takeCharges = true;

        if (!handled)
        {
            for (uint8 effIndex = 0; effIndex < i->aura->GetEffectCount(); ++effIndex)
            {
                if (!(i->effMask & (1<<effIndex)))
                    continue;

                AuraEffect* triggeredByAura = i->aura->GetEffect(effIndex);
                ASSERT(triggeredByAura);

                bool prevented = i->aura->CallScriptEffectProcHandlers(triggeredByAura, aurApp, eventInfo);
                if (prevented)
                {
                    takeCharges = true;
                    continue;
                }

                bool l_IsCrowControlAura = false;
                bool l_DontContinue = false;

                switch (triggeredByAura->GetAuraType())
                {
                    /// crowd control auras
                    case SPELL_AURA_MOD_FEAR:
                    case SPELL_AURA_MOD_STUN:
                    case SPELL_AURA_MOD_ROOT:
                    case SPELL_AURA_TRANSFORM:
                    case SPELL_AURA_MOD_FEAR_2:
                    case SPELL_AURA_MOD_ROOT_2:
                        l_IsCrowControlAura = true;
                        break;
                    default:
                        break;
                }

                if (!l_IsCrowControlAura && triggeredByAura->GetSpellInfo()->AuraInterruptFlags & SpellAuraInterruptFlags::AURA_INTERRUPT_FLAG_TAKE_DAMAGE_AMOUNT)
                {
                    l_IsCrowControlAura = true;
                    l_DontContinue      = true;
                }

                if (l_IsCrowControlAura)
                {
                    // CC Auras which use their amount amount to drop
                    // Are there any more auras which need this?
                    if (IsNoBreakingCC(isVictim, target, procFlag, procExtra, attType, procSpell, damage, absorb, procAura, spellInfo))
                        continue;

                    damage += absorb;

                    // chargeable mods are breaking on hit
                    // Spell own direct damage at apply wont break the CC
                    if (procSpell && (procSpell->Id == triggeredByAura->GetId()))
                    {
                        Aura* aura = triggeredByAura->GetBase();
                        // called from spellcast, should not have ticked yet
                        if (aura->GetDuration() == aura->GetMaxDuration())
                            continue;
                    }

                    // chargeable mods are breaking on hit
                    if (useCharges)
                        takeCharges = true;
                    else if (isVictim && damage && (!procSpell || GetSpellMaxRangeForTarget(this, procSpell) < 100.0f))
                    {
                        int32 damageLeft = triggeredByAura->GetCrowdControlDamage();
                        // No damage left
                        if (damageLeft <= int32(damage) && triggeredByAura->GetId() != 114052)
                        {
                            i->aura->Remove();
                            l_DontContinue = false;
                        }
                        else if (triggeredByAura->GetId() != 114052)
                            triggeredByAura->SetCrowdControlDamage(damageLeft - damage);
                    }

                    if (!l_DontContinue)
                        continue;
                }

                switch (triggeredByAura->GetAuraType())
                {
                    case SPELL_AURA_PROC_TRIGGER_SPELL:
                    case SPELL_AURA_PROC_TRIGGER_SPELL_2:
                    {
                        // Don`t drop charge or add cooldown for not started trigger
                        if (HandleProcTriggerSpell(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_PROC_TRIGGER_DAMAGE:
                    {
                        // target has to be valid
                        if (!target)
                            break;

                        SpellNonMeleeDamage damageInfo(this, target, spellInfo->Id, spellInfo->SchoolMask);
                        uint32 newDamage = target->SpellDamageBonusTaken(this, spellInfo, triggeredByAura->GetAmount(), SPELL_DIRECT_DAMAGE);
                        CalculateSpellDamageTaken(&damageInfo, newDamage, spellInfo);
                        DealDamageMods(damageInfo.target, damageInfo.damage, &damageInfo.absorb);
                        SendSpellNonMeleeDamageLog(&damageInfo);
                        DealSpellDamage(&damageInfo, true);
                        takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_MANA_SHIELD:
                    case SPELL_AURA_DUMMY:
                    case SPELL_AURA_PROC_TRIGGER_SPELL_COPY:
                    {
                        if (isVictim && procSpell && procSpell->Id == 108853 && triggeredByAura->GetId() == 44448)
                            break;

                        if (HandleDummyAuraProc(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;

                        /// Stealth isn't removed by multistrikes effects
                        if ((triggeredByAura->GetId() == 1784 || triggeredByAura->GetId() == 115191 || triggeredByAura->GetId() == 5215 || triggeredByAura->GetId() == 158185 || triggeredByAura->GetId() == 115192) && (procExtra & PROC_EX_INTERNAL_MULTISTRIKE))
                            takeCharges = false;

                        break;
                    }
                    case SPELL_AURA_PROC_ON_POWER_AMOUNT:
                    case SPELL_AURA_PROC_ON_POWER_AMOUNT_2:
                    {
                        if (HandleAuraProcOnPowerAmount(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_OBS_MOD_POWER:
                        if (HandleObsModEnergyAuraProc(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;
                        break;
                    case SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN:
                        if (HandleModDamagePctTakenAuraProc(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;
                        break;
                    case SPELL_AURA_MOD_MELEE_HASTE:
                    case SPELL_AURA_MOD_MELEE_HASTE_3:
                    {
                        if (HandleHasteAuraProc(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_OVERRIDE_CLASS_SCRIPTS:
                    {
                        if (HandleOverrideClassScriptAuraProc(target, damage, triggeredByAura, procSpell, cooldown))
                            takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_RAID_PROC_FROM_CHARGE_WITH_VALUE:
                    {
                        HandleAuraRaidProcFromChargeWithValue(triggeredByAura);
                        takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_RAID_PROC_FROM_CHARGE:
                    {
                        HandleAuraRaidProcFromCharge(triggeredByAura);
                        takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_PROC_TRIGGER_SPELL_WITH_VALUE:
                    {
                        if (HandleProcTriggerSpell(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;
                        break;
                    }
                    case SPELL_AURA_MOD_CASTING_SPEED_NOT_STACK:
                        // Skip melee hits or instant cast spells
                        if (procSpell && procSpell->CalcCastTime() != 0)
                            takeCharges = true;
                        break;
                    case SPELL_AURA_REFLECT_SPELLS_SCHOOL:
                        // Skip Melee hits and spells ws wrong school
                        if (procSpell && (triggeredByAura->GetMiscValue() & procSpell->SchoolMask))         // School check
                            takeCharges = true;
                        break;
                    case SPELL_AURA_SPELL_MAGNET:
                        // Skip Melee hits and targets with magnet aura
                        if (procSpell && (triggeredByAura->GetBase()->GetUnitOwner()->ToUnit() == ToUnit()))         // Magnet
                            takeCharges = true;
                        break;
                    case SPELL_AURA_MOD_POWER_COST_SCHOOL_PCT:
                    case SPELL_AURA_MOD_POWER_COST_SCHOOL:
                        // Skip melee hits and spells ws wrong school or zero cost
                        if (procSpell &&
                            (procSpell->ManaCost != 0 || procSpell->ManaCostPercentage != 0) && // Cost check
                            (triggeredByAura->GetMiscValue() & procSpell->SchoolMask))          // School check
                            takeCharges = true;
                        break;
                    case SPELL_AURA_MECHANIC_IMMUNITY:
                        // Compare mechanic
                        if (procSpell && procSpell->Mechanic == uint32(triggeredByAura->GetMiscValue()))
                            takeCharges = true;
                        break;
                    case SPELL_AURA_MOD_MECHANIC_RESISTANCE:
                        // Compare mechanic
                        if (procSpell && procSpell->Mechanic == uint32(triggeredByAura->GetMiscValue()))
                            takeCharges = true;
                        break;
                    case SPELL_AURA_MOD_DAMAGE_FROM_CASTER:
                        // Compare casters
                        if (triggeredByAura->GetCasterGUID() == target->GetGUID())
                            takeCharges = true;
                        break;
                    case SPELL_AURA_MOD_SPELL_CRIT_CHANCE:
                        if (procSpell && HandleSpellCritChanceAuraProc(target, damage, triggeredByAura, procSpell, procFlag, procExtra, cooldown))
                            takeCharges = true;
                        break;
                    default:
                        // nothing do, just charges counter
                        // Don't drop charge for Earth Shield because of second effect
                        if (triggeredByAura->GetId() == 974)
                            break;

                        takeCharges = true;
                        break;
                } // switch (triggeredByAura->GetAuraType())

                i->aura->CallScriptAfterEffectProcHandlers(triggeredByAura, aurApp, eventInfo);
            } // for (uint8 effIndex = 0; effIndex < SpellEffIndex::MAX_EFFECTS; ++effIndex)
        } // if (!handled)

        // Dirty Tricks
        // Your Gouge and Blind no longer have an Energy cost ...
        // ... and no longer break from damage dealt by your Poison and Bleed effects.
        if ((i->aura->GetId() == 2094 || i->aura->GetId() == 1776) && procSpell && procSpell->IsPoisonOrBleedSpell() && target->HasAura(108216))
            takeCharges = false;

        /// Deep Freeze can be removed just by Ice Lance or Ice Nova damage
        if (i->aura->GetId() == 44572 && procSpell && procSpell->Id != 30455 && procSpell->Id != 157997)
            takeCharges = false;

        // Remove charge (aura can be removed by triggers)
        if (prepare && useCharges && takeCharges && !i->aura->GetSpellInfo()->IsCustomCharged(procSpell, this))
        {
            // Hack Fix for Tiger Strikes
            if (i->aura->GetId() == 120273)
            {
                if (target)
                {
                    if (attType == WeaponAttackType::BaseAttack)
                        CastSpell(target, 120274, true); // extra attack for MainHand
                    else if (attType == WeaponAttackType::OffAttack)
                        CastSpell(target, 120278, true); // extra attack for OffHand
                }
            }

            i->aura->DropCharge();
        }
        i->aura->CallScriptAfterProcHandlers(aurApp, eventInfo);

        if (spellInfo->AttributesEx3 & SPELL_ATTR3_DISABLE_PROC)
            SetCantProc(false);
    }

    // Cleanup proc requirements
    if (procExtra & (PROC_EX_INTERNAL_TRIGGERED | PROC_EX_INTERNAL_CANT_PROC))
        SetCantProc(false);
}

bool Unit::IsNoBreakingCC(bool /*isVictim*/, Unit* target, uint32 procFlag, uint32 /*procExtra*/, WeaponAttackType /*attType*/, SpellInfo const* procSpell,
                          uint32 /*damage*/, uint32 /*absorb*/ /* = 0 */, SpellInfo const* /*procAura*/ /* = NULL */, SpellInfo const* spellInfo ) const
{
    // Dragon Breath & Living Bomb
    if (spellInfo->GetCategory() == 1215 && procSpell &&
        procSpell->SpellFamilyName == SPELLFAMILY_MAGE && procSpell->SpellFamilyFlags[1] == 0x00010000)
        return true;

    // Sanguinary Vein and Gouge
    if (spellInfo->Id == 1776 && target && (procFlag & PROC_FLAG_TAKEN_PERIODIC) && (procSpell && (procSpell->GetAllEffectsMechanicMask() & (1<<MECHANIC_BLEED))))
        if (AuraEffect* aur = target->GetDummyAuraEffect(SPELLFAMILY_GENERIC, 4821, 1))
            if (roll_chance_i(aur->GetAmount()))
                return true;

    // Main Gauche & Gouge
    if (procSpell && procSpell->Id == 86392 && spellInfo->Id == 1776)
        return true;

    return false;
}

void Unit::GetProcAurasTriggeredOnEvent(std::list<AuraApplication*>& aurasTriggeringProc, std::list<AuraApplication*>* procAuras, ProcEventInfo eventInfo)
{
    // use provided list of auras which can proc
    if (procAuras)
    {
        for (std::list<AuraApplication*>::iterator itr = procAuras->begin(); itr!= procAuras->end(); ++itr)
        {
            ASSERT((*itr)->GetTarget() == this);
            if (!(*itr)->GetRemoveMode())
                if ((*itr)->GetBase()->IsProcTriggeredOnEvent(*itr, eventInfo))
                {
                    (*itr)->GetBase()->PrepareProcToTrigger(*itr, eventInfo);
                    aurasTriggeringProc.push_back(*itr);
                }
        }
    }
    // or generate one on our own
    else
    {
        for (AuraApplicationMap::iterator itr = GetAppliedAuras().begin(); itr!= GetAppliedAuras().end(); ++itr)
        {
            if (itr->second->GetBase()->IsProcTriggeredOnEvent(itr->second, eventInfo))
            {
                itr->second->GetBase()->PrepareProcToTrigger(itr->second, eventInfo);
                aurasTriggeringProc.push_back(itr->second);
            }
        }
    }
}

void Unit::TriggerAurasProcOnEvent(CalcDamageInfo& damageInfo)
{
    DamageInfo dmgInfo = DamageInfo(damageInfo);
    TriggerAurasProcOnEvent(NULL, NULL, damageInfo.target, damageInfo.procAttacker, damageInfo.procVictim, 0, 0, damageInfo.procEx, NULL, &dmgInfo, NULL);
}

void Unit::TriggerAurasProcOnEvent(std::list<AuraApplication*>* myProcAuras, std::list<AuraApplication*>* targetProcAuras, Unit* actionTarget, uint32 typeMaskActor, uint32 typeMaskActionTarget, uint32 spellTypeMask, uint32 spellPhaseMask, uint32 hitMask, Spell* spell, DamageInfo* damageInfo, HealInfo* healInfo)
{
    // prepare data for self trigger
    ProcEventInfo myProcEventInfo = ProcEventInfo(this, actionTarget, actionTarget, typeMaskActor, spellTypeMask, spellPhaseMask, hitMask, spell, damageInfo, healInfo);
    std::list<AuraApplication*> myAurasTriggeringProc;
    GetProcAurasTriggeredOnEvent(myAurasTriggeringProc, myProcAuras, myProcEventInfo);

    // prepare data for target trigger
    ProcEventInfo targetProcEventInfo = ProcEventInfo(this, actionTarget, this, typeMaskActionTarget, spellTypeMask, spellPhaseMask, hitMask, spell, damageInfo, healInfo);
    std::list<AuraApplication*> targetAurasTriggeringProc;
    if (typeMaskActionTarget)
        GetProcAurasTriggeredOnEvent(targetAurasTriggeringProc, targetProcAuras, targetProcEventInfo);

    TriggerAurasProcOnEvent(myProcEventInfo, myAurasTriggeringProc);

    if (typeMaskActionTarget)
        TriggerAurasProcOnEvent(targetProcEventInfo, targetAurasTriggeringProc);
}

void Unit::TriggerAurasProcOnEvent(ProcEventInfo& eventInfo, std::list<AuraApplication*>& aurasTriggeringProc)
{
    for (std::list<AuraApplication*>::iterator itr = aurasTriggeringProc.begin(); itr != aurasTriggeringProc.end(); ++itr)
    {
        if (!(*itr)->GetRemoveMode())
            (*itr)->GetBase()->TriggerProcOnEvent(*itr, eventInfo);
    }
}

SpellSchoolMask Unit::GetMeleeDamageSchoolMask() const
{
    return SPELL_SCHOOL_MASK_NORMAL;
}

bool Unit::IsWarlockPet() const
{
    uint32 l_Entry = GetEntry();
    return l_Entry == ENTRY_INFERNAL ||
        l_Entry == ENTRY_ABYSSAL ||
        l_Entry == ENTRY_IMP ||
        l_Entry == ENTRY_FEL_IMP ||
        l_Entry == ENTRY_VOIDWALKER ||
        l_Entry == ENTRY_VOIDLORD ||
        l_Entry == ENTRY_SUCCUBUS ||
        l_Entry == ENTRY_SHIVARRA ||
        l_Entry == ENTRY_FELHUNTER ||
        l_Entry == ENTRY_OBSERVER ||
        l_Entry == ENTRY_FELGUARD ||
        l_Entry == ENTRY_WRATHGUARD ||
        l_Entry == ENTRY_DOOMGUARD ||
        l_Entry == ENTRY_TERRORGUARD;
}

Player* Unit::GetSpellModOwner() const
{
    if (IsPlayer())
        return (Player*)this;
    if (ToCreature()->isPet() || ToCreature()->isTotem() || ToCreature()->isSummon() || ToCreature()->isGuardian())
    {
        Unit* owner = GetOwner();
        if (owner && owner->IsPlayer())
            return (Player*)owner;
    }
    return NULL;
}

///----------Pet responses methods-----------------

void Unit::SendPetCastFail(uint32 p_SpellID, SpellCastResult p_Result, uint8 p_CastCount)
{
    if (p_Result == SPELL_CAST_OK)
        return;

    Unit * l_Owner = GetCharmerOrOwner();

    if (!l_Owner || l_Owner->GetTypeId() != TYPEID_PLAYER)
        return;

    WorldPacket l_Data(SMSG_PET_CAST_FAILED, 4 + 4 + 4 + 4 + 1);
    l_Data << uint32(p_SpellID);                            ///< SpellID
    l_Data << uint32(p_Result);                             ///< Reason
    l_Data << uint32(0);                                    ///< FailedArg1
    l_Data << uint32(0);                                    ///< FailedArg2
    l_Data << uint8(p_CastCount);                           ///< CastID

    l_Owner->ToPlayer()->GetSession()->SendPacket(&l_Data);
}

void Unit::SendPetActionFeedback(uint32 p_SpellID, uint8 p_Response)
{
    Unit* l_Owner = GetOwner();
    if (!l_Owner || l_Owner->GetTypeId() != TYPEID_PLAYER)
        return;

    WorldPacket l_Data(SMSG_PET_ACTION_FEEDBACK, 1);
    l_Data << uint32(p_SpellID);
    l_Data << uint8(p_Response);

    l_Owner->ToPlayer()->GetSession()->SendPacket(&l_Data);
}

void Unit::SendPetTalk(uint32 p_Action)
{
    Unit* l_Owner = GetOwner();
    if (!l_Owner || l_Owner->GetTypeId() != TYPEID_PLAYER)
        return;

    uint64 l_UnitGUID = GetGUID();

    WorldPacket l_Data(SMSG_PET_ACTION_SOUND, 16 + 2 + 4);
    l_Data.appendPackGUID(l_UnitGUID);      ///< UnitGUID
    l_Data << uint32(p_Action);             ///< Action
    l_Owner->ToPlayer()->GetSession()->SendPacket(&l_Data);
}

void Unit::SendPetAIReaction(uint64 p_Guid)
{
    Unit * l_Owner = GetOwner();
    if (!l_Owner || l_Owner->GetTypeId() != TYPEID_PLAYER)
        return;

    WorldPacket l_Data(SMSG_AI_REACTION, 16 + 2 + 4);
    l_Data.appendPackGUID(p_Guid);
    l_Data << uint32(AI_REACTION_HOSTILE);

    l_Owner->ToPlayer()->GetSession()->SendPacket(&l_Data);
}

///----------End of Pet responses methods----------

void Unit::SendItemBonusDebug(uint32 p_Quantity, std::string p_Text, Player* p_Target /*= nullptr*/)
{
    WorldPacket l_Data(Opcodes::SMSG_ITEM_BONUS_DEBUG);

    l_Data.appendPackGUID(GetGUID());
    l_Data << int32(p_Quantity);
    l_Data << int32(0);

    l_Data.WriteBits(p_Text.size(), 12);
    l_Data.FlushBits();
    l_Data.WriteString(p_Text);

    if (p_Target != nullptr)
        p_Target->GetSession()->SendPacket(&l_Data);
    else
        SendMessageToSetInRange(&l_Data, GetVisibilityRange(), false);
}

void Unit::StopMoving()
{
    ClearUnitState(UNIT_STATE_MOVING);

    // not need send any packets if not in world or not moving
    if (!IsInWorld() || movespline->Finalized())
        return;

    // Update position using old spline
    UpdateSplinePosition();
    Movement::MoveSplineInit l_Init(this);
    l_Init.Stop();
}

bool Unit::IsSitState() const
{
    uint8 s = getStandState();
    return
        s == UNIT_STAND_STATE_SIT_CHAIR        || s == UNIT_STAND_STATE_SIT_LOW_CHAIR  ||
        s == UNIT_STAND_STATE_SIT_MEDIUM_CHAIR || s == UNIT_STAND_STATE_SIT_HIGH_CHAIR ||
        s == UNIT_STAND_STATE_SIT;
}

bool Unit::IsStandState() const
{
    uint8 s = getStandState();
    return !IsSitState() && s != UNIT_STAND_STATE_SLEEP && s != UNIT_STAND_STATE_KNEEL;
}

void Unit::SetStandState(uint8 p_State)
{
    SetByteValue(UNIT_FIELD_ANIM_TIER, 0, p_State);

    if (IsStandState())
       RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_NOT_SEATED);

    if (IsPlayer())
    {
        WorldPacket l_Update(SMSG_STANDSTATE_UPDATE, 5);
        l_Update << uint32(0);                              ///< AnimKit
        l_Update << (uint8)p_State;
        ToPlayer()->GetSession()->SendPacket(&l_Update);
    }
}

bool Unit::IsPolymorphed() const
{
    uint32 transformId = getTransForm();
    if (!transformId)
        return false;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(transformId);
    if (!spellInfo)
        return false;

    return spellInfo->GetSpellSpecific() == SpellSpecificType::SpellSpecificMagePolymorph;
}

void Unit::SetDisplayId(uint32 modelId)
{
    SetUInt32Value(UNIT_FIELD_DISPLAY_ID, modelId);

    if (GetTypeId() == TYPEID_UNIT && ToCreature()->isPet())
    {
        Pet* pet = ToPet();
        if (!pet->isControlled())
            return;
        Unit* owner = GetOwner();
        if (owner && (owner->IsPlayer()) && owner->ToPlayer()->GetGroup())
            owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_MODEL_ID);
    }
}

void Unit::RestoreDisplayId()
{
    AuraEffect* handledAura = nullptr;
    // try to receive model from transform auras
    Unit::AuraEffectList const& transforms = GetAuraEffectsByType(SPELL_AURA_TRANSFORM);
    if (!transforms.empty())
    {
        // iterate over already applied transform auras - from newest to oldest
        for (Unit::AuraEffectList::const_reverse_iterator i = transforms.rbegin(); i != transforms.rend(); ++i)
        {
            if (AuraApplication const* aurApp = (*i)->GetBase()->GetApplicationOfTarget(GetGUID()))
            {
                if (!handledAura)
                    handledAura = (*i);
                // prefer negative auras
                if (!aurApp->IsPositive())
                {
                    handledAura = (*i);
                    break;
                }
            }
        }
    }
    // transform aura was found
    if (handledAura)
        handledAura->HandleEffect(this, AURA_EFFECT_HANDLE_SEND_FOR_CLIENT, true);
    // we've found shapeshift
    else if (uint32 modelId = GetModelForForm(GetShapeshiftForm()))
        SetDisplayId(modelId);
    // no auras found - set modelid to default
    else
        SetDisplayId(GetNativeDisplayId());
}

void Unit::ClearAllReactives()
{
    for (uint8 i = 0; i < MAX_REACTIVE; ++i)
        m_reactiveTimer[i] = 0;

    if (HasAuraState(AURA_STATE_DEFENSE))
        ModifyAuraState(AURA_STATE_DEFENSE, false);

    if (getClass() == CLASS_HUNTER && HasAuraState(AURA_STATE_HUNTER_PARRY))
        ModifyAuraState(AURA_STATE_HUNTER_PARRY, false);

    if (getClass() == CLASS_WARRIOR)
        ClearComboPoints();
}

void Unit::UpdateReactives(uint32 p_time)
{
    for (uint8 i = 0; i < MAX_REACTIVE; ++i)
    {
        ReactiveType reactive = ReactiveType(i);

        if (!m_reactiveTimer[reactive])
            continue;

        if (m_reactiveTimer[reactive] <= p_time)
        {
            m_reactiveTimer[reactive] = 0;

            switch (reactive)
            {
                case REACTIVE_DEFENSE:
                    if (HasAuraState(AURA_STATE_DEFENSE))
                        ModifyAuraState(AURA_STATE_DEFENSE, false);
                    break;
                case REACTIVE_HUNTER_PARRY:
                    if (getClass() == CLASS_HUNTER && HasAuraState(AURA_STATE_HUNTER_PARRY))
                        ModifyAuraState(AURA_STATE_HUNTER_PARRY, false);
                    break;
                case REACTIVE_OVERPOWER:
                    if (getClass() == CLASS_WARRIOR)
                        ClearComboPoints();
                    break;
                default:
                    break;
            }
        }
        else
        {
            m_reactiveTimer[reactive] -= p_time;
        }
    }
}

void Unit::UpdateStackOnDuration(uint32 p_Time)
{
    for (AuraStackOnDurationMap::iterator l_Iter = m_StackOnDurationMap.begin(); l_Iter != m_StackOnDurationMap.end();)
    {
        StackOnDuration* l_Stack = GetStackOnDuration(l_Iter->first);

        std::vector<std::pair<uint64, int32>> l_StackDuration = l_Stack->GetStackDuration();

        bool l_MustContinue = false;
        uint8 l_Count = 0;
        for (std::pair<uint64, int32> l_StackParam : l_StackDuration)
        {
            if (l_StackParam.first <= p_Time)
            {
                if (l_StackDuration.size() <= 1)
                {
                    l_Iter = m_StackOnDurationMap.erase(l_Iter);
                    l_MustContinue = true;
                    break;
                }

                l_Stack->m_StackDuration.erase(l_Stack->m_StackDuration.begin() + l_Count);
                continue;
            }
            else
                l_Stack->DecreaseDuration(l_Count, p_Time);

            ++l_Count;
        }

        if (l_MustContinue)
            continue;

        ++l_Iter;
    }
}

void Unit::GetAttackableUnitListInRange(std::list<Unit*> &list, float fMaxSearchRange) const
{
    CellCoord p(JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY()));
    Cell cell(p);
    cell.SetNoCreate();

    JadeCore::AnyUnitInObjectRangeCheck u_check(this, fMaxSearchRange);
    JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(this, list, u_check);

    TypeContainerVisitor<JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck>, WorldTypeMapContainer > world_unit_searcher(searcher);
    TypeContainerVisitor<JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck>, GridTypeMapContainer >  grid_unit_searcher(searcher);

    cell.Visit(p, world_unit_searcher, *GetMap(), *this, fMaxSearchRange);
    cell.Visit(p, grid_unit_searcher, *GetMap(), *this, fMaxSearchRange);
}

void Unit::GetAreatriggerListInRange(std::list<AreaTrigger*>& p_List, float p_Range) const
{
    CellCoord l_Coords(JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY()));
    Cell l_Cell(l_Coords);
    l_Cell.SetNoCreate();

    JadeCore::AnyAreatriggerInObjectRangeCheck l_Check(this, p_Range);
    JadeCore::AreaTriggerListSearcher<JadeCore::AnyAreatriggerInObjectRangeCheck> searcher(this, p_List, l_Check);

    TypeContainerVisitor<JadeCore::AreaTriggerListSearcher<JadeCore::AnyAreatriggerInObjectRangeCheck>, WorldTypeMapContainer> l_WorldSearcher(searcher);
    TypeContainerVisitor<JadeCore::AreaTriggerListSearcher<JadeCore::AnyAreatriggerInObjectRangeCheck>, GridTypeMapContainer>  l_GridSearcher(searcher);

    l_Cell.Visit(l_Coords, l_WorldSearcher, *GetMap(), *this, p_Range);
    l_Cell.Visit(l_Coords, l_GridSearcher, *GetMap(), *this, p_Range);
}

void Unit::GetAreaTriggerListWithSpellIDInRange(std::list<AreaTrigger*>& p_List, uint32 p_SpellID, float p_Range) const
{
    CellCoord l_Coords(JadeCore::ComputeCellCoord(GetPositionX(), GetPositionY()));
    Cell l_Cell(l_Coords);
    l_Cell.SetNoCreate();

    JadeCore::AnyAreatriggerInObjectRangeCheck l_Check(this, p_Range);
    JadeCore::AreaTriggerListSearcher<JadeCore::AnyAreatriggerInObjectRangeCheck> searcher(this, p_List, l_Check);

    TypeContainerVisitor<JadeCore::AreaTriggerListSearcher<JadeCore::AnyAreatriggerInObjectRangeCheck>, WorldTypeMapContainer> l_WorldSearcher(searcher);
    TypeContainerVisitor<JadeCore::AreaTriggerListSearcher<JadeCore::AnyAreatriggerInObjectRangeCheck>, GridTypeMapContainer>  l_GridSearcher(searcher);

    l_Cell.Visit(l_Coords, l_WorldSearcher, *GetMap(), *this, p_Range);
    l_Cell.Visit(l_Coords, l_GridSearcher, *GetMap(), *this, p_Range);

    if (!p_List.empty())
    {
        p_List.remove_if([p_SpellID](AreaTrigger* p_AreaTrigger) -> bool
        {
            if (p_AreaTrigger == nullptr || p_AreaTrigger->GetSpellId() != p_SpellID)
                return true;

            return false;
        });
    }
}

Unit* Unit::SelectNearbyTarget(Unit* exclude /*= NULL*/, float dist /*= NOMINAL_MELEE_RANGE*/, uint32 p_ExludeAuraID /*= 0*/, bool p_ExcludeVictim /*= true*/, bool p_Alive /*= true*/, bool p_ExcludeStealthVictim /*=false*/, bool p_CheckValidAttack /*= false*/) const
{
    std::list<Unit*> l_Targets;
    JadeCore::AnyUnfriendlyUnitInObjectRangeCheck u_check(this, this, dist);
    JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> searcher(this, l_Targets, u_check);
    VisitNearbyObject(dist, searcher);

    // remove current target
    if (!p_ExcludeVictim)
    {
        if (getVictim())
            l_Targets.remove(getVictim());
    }

    if (exclude)
        l_Targets.remove(exclude);

    // remove not LoS targets
    for (std::list<Unit*>::iterator tIter = l_Targets.begin(); tIter != l_Targets.end();)
    {
        if (!IsWithinLOSInMap(*tIter) || (*tIter)->isTotem() || (*tIter)->isSpiritService() || (*tIter)->GetCreatureType() == CREATURE_TYPE_CRITTER)
            l_Targets.erase(tIter++);
        else if (p_ExcludeStealthVictim && (*tIter)->HasStealthAura()) ///< Remove Stealth victim
            l_Targets.erase(tIter++);
        else if (p_CheckValidAttack && !IsValidAttackTarget(*tIter))
            l_Targets.erase(tIter++);
        else
            ++tIter;
    }

    // no appropriate targets
    if (l_Targets.empty())
        return nullptr;

    if (p_ExludeAuraID)
        l_Targets.remove_if(JadeCore::UnitAuraCheck(true, p_ExludeAuraID));

    // no appropriate targets
    if (l_Targets.empty())
        return nullptr;

    if (p_Alive)
    {
        l_Targets.remove_if([this](Unit* p_Unit) -> bool
        {
            if (!p_Unit)
                return true;

            if (p_Unit->isAlive())
                return false;

            return true;
        });
    }

    // no appropriate targets
    if (l_Targets.empty())
        return nullptr;

    // select random
    return JadeCore::Containers::SelectRandomContainerElement(l_Targets);
}

Unit* Unit::SelectNearbyAlly(Unit* exclude, float dist, bool p_CheckValidAssist /*= false*/) const
{
    std::list<Unit*> targets;
    JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(this, this, dist);
    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(this, targets, u_check);
    VisitNearbyObject(dist, searcher);

    if (exclude)
        targets.remove(exclude);

    // remove not LoS targets
    for (std::list<Unit*>::iterator tIter = targets.begin(); tIter != targets.end();)
    {
        if (!IsWithinLOSInMap(*tIter) || (*tIter)->isTotem() || (*tIter)->isSpiritService() || (*tIter)->GetCreatureType() == CREATURE_TYPE_CRITTER)
            targets.erase(tIter++);
        else if (p_CheckValidAssist && !IsValidAssistTarget(*tIter))
            targets.erase(tIter++);
        else
            ++tIter;
    }

    // no appropriate targets
    if (targets.empty())
        return NULL;

    // select random
    return JadeCore::Containers::SelectRandomContainerElement(targets);
}

Unit* Unit::SelectNearbyMostInjuredAlly(Unit* p_Exclude /*= nullptr*/, float p_Dist /*= NOMINAL_MELEE_RANGE*/, uint32 p_ExcludeEntry /*= 0*/) const
{
    std::list<Unit*> l_Targets;
    JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(this, this, p_Dist);
    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(this, l_Targets, l_Check);
    VisitNearbyObject(p_Dist, l_Searcher);

    if (p_Exclude)
        l_Targets.remove(p_Exclude);

    /// No appropriate targets
    if (l_Targets.empty())
        return nullptr;

    l_Targets.remove_if([this, p_ExcludeEntry](Unit* p_Unit) -> bool
    {
        if (p_Unit == nullptr)
            return true;

        if (!IsWithinLOSInMap(p_Unit) || p_Unit->isTotem() || p_Unit->isSpiritService())
            return true;

        if (p_Unit->GetCreatureType() == CreatureType::CREATURE_TYPE_CRITTER)
            return true;

        if (p_Unit->GetEntry() == p_ExcludeEntry)
            return true;

        return false;
    });

    /// No appropriate targets
    if (l_Targets.empty())
        return nullptr;

    l_Targets.sort(JadeCore::HealthPctOrderPred(true));

    /// Select most injured
    return l_Targets.front();
}

void Unit::ApplyAttackTimePercentMod(WeaponAttackType att, float val, bool apply)
{
    float remainingTimePct = (float)m_attackTimer[att] / (GetAttackTime(att) * m_modAttackSpeedPct[att]);
    if (val > 0)
    {
        ApplyPercentModFloatVar(m_modAttackSpeedPct[att], val, !apply);
        ApplyPercentModFloatValue(UNIT_FIELD_ATTACK_ROUND_BASE_TIME+att, val, !apply);
    }
    else
    {
        ApplyPercentModFloatVar(m_modAttackSpeedPct[att], -val, apply);
        ApplyPercentModFloatValue(UNIT_FIELD_ATTACK_ROUND_BASE_TIME+att, -val, apply);
    }

    m_attackTimer[att] = uint32(GetAttackTime(att) * m_modAttackSpeedPct[att] * remainingTimePct);
}

void Unit::ApplyCastTimePercentMod(float val, bool apply)
{
    if (val > 0)
        ApplyPercentModFloatValue(UNIT_FIELD_MOD_CASTING_SPEED, val, !apply);
    else
        ApplyPercentModFloatValue(UNIT_FIELD_MOD_CASTING_SPEED, -val, apply);
}

uint32 Unit::GetCastingTimeForBonus(SpellInfo const* spellProto, DamageEffectType damagetype, uint32 CastingTime) const
{
    // Not apply this to creature casted spells with casttime == 0
    if (CastingTime == 0 && GetTypeId() == TYPEID_UNIT && !ToCreature()->isPet())
        return 3500;

    if (CastingTime > 7000) CastingTime = 7000;
    if (CastingTime < 1500) CastingTime = 1500;

    if (damagetype == DOT && !spellProto->IsChanneled())
        CastingTime = 3500;

    int32 overTime    = 0;
    uint8 effects     = 0;
    bool DirectDamage = false;
    bool AreaEffect   = false;

    for (uint8 i = 0; i < spellProto->EffectCount; i++)
    {
        switch (spellProto->Effects[i].Effect)
        {
            case SPELL_EFFECT_SCHOOL_DAMAGE:
            case SPELL_EFFECT_POWER_DRAIN:
            case SPELL_EFFECT_HEALTH_LEECH:
            case SPELL_EFFECT_ENVIRONMENTAL_DAMAGE:
            case SPELL_EFFECT_POWER_BURN:
            case SPELL_EFFECT_HEAL:
                DirectDamage = true;
                break;
            case SPELL_EFFECT_APPLY_AURA:
            case SPELL_EFFECT_APPLY_AURA_ON_PET:
                switch (spellProto->Effects[i].ApplyAuraName)
                {
                    case SPELL_AURA_PERIODIC_DAMAGE:
                    case SPELL_AURA_PERIODIC_HEAL:
                    case SPELL_AURA_PERIODIC_LEECH:
                        if (spellProto->GetDuration())
                            overTime = spellProto->GetDuration();
                        break;
                    default:
                        // -5% per additional effect
                        ++effects;
                        break;
                }
            default:
                break;
        }

        if (spellProto->Effects[i].IsTargetingArea())
            AreaEffect = true;
    }

    // Combined Spells with Both Over Time and Direct Damage
    if (overTime > 0 && CastingTime > 0 && DirectDamage)
    {
        // mainly for DoTs which are 3500 here otherwise
        uint32 OriginalCastTime = spellProto->CalcCastTime();
        if (OriginalCastTime > 7000) OriginalCastTime = 7000;
        if (OriginalCastTime < 1500) OriginalCastTime = 1500;
        // Portion to Over Time
        float PtOT = (overTime / 15000.0f) / ((overTime / 15000.0f) + (OriginalCastTime / 3500.0f));

        if (damagetype == DOT)
            CastingTime = uint32(CastingTime * PtOT);
        else if (PtOT < 1.0f)
            CastingTime  = uint32(CastingTime * (1 - PtOT));
        else
            CastingTime = 0;
    }

    // Area Effect Spells receive only half of bonus
    if (AreaEffect)
        CastingTime /= 2;

    // 50% for damage and healing spells for leech spells from damage bonus and 0% from healing
    for (uint8 j = 0; j < spellProto->EffectCount; ++j)
    {
        if (spellProto->Effects[j].Effect == SPELL_EFFECT_HEALTH_LEECH ||
            ((spellProto->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA || spellProto->Effects[j].Effect == SPELL_EFFECT_APPLY_AURA_ON_PET) && spellProto->Effects[j].ApplyAuraName == SPELL_AURA_PERIODIC_LEECH))
        {
            CastingTime /= 2;
            break;
        }
    }

    // -5% of total per any additional effect
    for (uint8 i = 0; i < effects; ++i)
        CastingTime *= 0.95f;

    return CastingTime;
}

void Unit::UpdateAuraForGroup(uint8 slot)
{
    if (slot >= MAX_AURAS)                        // slot not found, return
        return;
    if (Player* player = ToPlayer())
    {
        if (player->GetGroup())
        {
            player->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_AURAS);
            player->SetAuraUpdateMaskForRaid(slot);
        }
    }
    else if (GetTypeId() == TYPEID_UNIT && ToCreature()->isPet())
    {
        Pet* pet = ((Pet*)this);
        if (pet->isControlled())
        {
            Unit* owner = GetOwner();
            if (owner && (owner->IsPlayer()) && owner->ToPlayer()->GetGroup())
            {
                owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_AURAS);
                pet->SetAuraUpdateMaskForRaid(slot);
            }
        }
    }
}

float Unit::CalculateDefaultCoefficient(SpellInfo const *spellInfo, DamageEffectType damagetype) const
{
    // Damage over Time spells bonus calculation
    float DotFactor = 1.0f;
    if (damagetype == DOT)
    {

        int32 DotDuration = spellInfo->GetDuration();
        if (!spellInfo->IsChanneled() && DotDuration > 0)
            DotFactor = DotDuration / 15000.0f;

        if (uint32 DotTicks = spellInfo->GetMaxTicks())
            DotFactor /= DotTicks;
    }

    int32 CastingTime = spellInfo->IsChanneled() ? spellInfo->GetDuration() : spellInfo->CalcCastTime();
    // Distribute Damage over multiple effects, reduce by AoE
    CastingTime = GetCastingTimeForBonus(spellInfo, damagetype, CastingTime);

    // As wowwiki says: C = (Cast Time / 3.5)
    return (CastingTime / 3500.0f) * DotFactor;
}

float Unit::GetAPMultiplier(WeaponAttackType attType, bool normalized)
{
    if (!normalized || GetTypeId() != TYPEID_PLAYER)
        return float(GetAttackTime(attType)) / 1000.0f;

    Item* Weapon = ToPlayer()->GetWeaponForAttack(attType, true);
    if (!Weapon)
        return 2.4f;                                         // fist attack

    switch (Weapon->GetTemplate()->InventoryType)
    {
        case INVTYPE_2HWEAPON:
            return 3.3f;
        case INVTYPE_RANGED:
        case INVTYPE_RANGEDRIGHT:
        case INVTYPE_THROWN:
            return 2.8f;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
        default:
            return Weapon->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ? 1.7f : 2.4f;
    }
}

void Unit::SetContestedPvP(Player* attackedPlayer)
{
    Player* player = GetCharmerOrOwnerPlayerOrPlayerItself();

    if (!player || (attackedPlayer && (attackedPlayer == player || (player->m_Duel && player->m_Duel->opponent == attackedPlayer))))
        return;

    player->SetContestedPvPTimer(30000);
    if (!player->HasUnitState(UNIT_STATE_ATTACK_PLAYER))
    {
        player->AddUnitState(UNIT_STATE_ATTACK_PLAYER);
        player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP);
        // call MoveInLineOfSight for nearby contested guards
        UpdateObjectVisibility();
    }
    if (!HasUnitState(UNIT_STATE_ATTACK_PLAYER))
    {
        AddUnitState(UNIT_STATE_ATTACK_PLAYER);
        // call MoveInLineOfSight for nearby contested guards
        UpdateObjectVisibility();
    }
}

void Unit::AddPetAura(PetAura const* petSpell)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    m_petAuras.insert(petSpell);
    if (Pet* pet = ToPlayer()->GetPet())
        pet->CastPetAura(petSpell);
}

void Unit::RemovePetAura(PetAura const* petSpell)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    m_petAuras.erase(petSpell);
    if (Pet* pet = ToPlayer()->GetPet())
        pet->RemoveAurasDueToSpell(petSpell->GetAura(pet->GetEntry()));
}

Pet* Unit::CreateTamedPetFrom(Creature* creatureTarget, uint32 spell_id)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return NULL;

    Pet* pet = new Pet((Player*)this, HUNTER_PET);

    if (!pet->CreateBaseAtCreature(creatureTarget))
    {
        delete pet;
        return NULL;
    }

    uint8 level = creatureTarget->getLevel() + 5 < getLevel() ? (getLevel() - 5) : creatureTarget->getLevel();

    InitTamedPet(pet, level, spell_id);

    return pet;
}

Pet* Unit::CreateTamedPetFrom(uint32 creatureEntry, uint32 spell_id)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return NULL;

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(creatureEntry);
    if (!creatureInfo)
        return NULL;

    Pet* pet = new Pet((Player*)this, HUNTER_PET);

    if (!pet->CreateBaseAtCreatureInfo(creatureInfo, this) || !InitTamedPet(pet, getLevel(), spell_id))
    {
        delete pet;
        return NULL;
    }

    return pet;
}

bool Unit::InitTamedPet(Pet* pet, uint8 level, uint32 spell_id)
{
    pet->SetCreatorGUID(GetGUID());
    pet->setFaction(getFaction());
    pet->SetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL, spell_id);

    if (IsPlayer())
        pet->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);

    if (!pet->InitStatsForLevel(level))
    {
        sLog->outError(LOG_FILTER_UNITS, "Pet::InitStatsForLevel() failed for creature (Entry: %u)!", pet->GetEntry());
        return false;
    }

    pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);
    // this enables pet details window (Shift+P)
    pet->InitPetCreateSpells();
    //pet->InitLevelupSpellsForLevel();
    pet->SetFullHealth();
    return true;
}

bool Unit::IsTriggeredAtSpellProcEvent(Unit* victim, Aura* aura, SpellInfo const* procSpell, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, bool isVictim, bool active, SpellProcEventEntry const* & spellProcEvent)
{
    SpellInfo const* spellProto = aura->GetSpellInfo();

    // let the aura be handled by new proc system if it has new entry
    if (sSpellMgr->GetSpellProcEntry(spellProto->Id))
        return false;

    // Get proc Event Entry
    spellProcEvent = sSpellMgr->GetSpellProcEvent(spellProto->Id);

    // Get EventProcFlag
    uint32 EventProcFlag;
    if (spellProcEvent && spellProcEvent->procFlags) // if exist get custom spellProcEvent->procFlags
        EventProcFlag = spellProcEvent->procFlags;
    else
        EventProcFlag = spellProto->ProcFlags;       // else get from spell proto
    // Continue if no trigger exist
    if (!EventProcFlag)
        return false;

    // Additional checks for triggered spells (ignore trap casts)
    if (procExtra & PROC_EX_INTERNAL_TRIGGERED && !(procFlag & PROC_FLAG_DONE_TRAP_ACTIVATION))
    {
        if (!(spellProto->AttributesEx3 & SPELL_ATTR3_CAN_PROC_WITH_TRIGGERED))
            return false;
    }

    // Check spellProcEvent data requirements
    if (!sSpellMgr->IsSpellProcEventCanTriggeredBy(spellProcEvent, EventProcFlag, procSpell, procFlag, procExtra, active))
    {
        if (spellProto && spellProto->Id == 44448 && procSpell &&
            (procSpell->Id == 108853 || procSpell->Id == 11366 || procSpell->Id == 11129)) ///< Inferno Blast, Combustion and Pyroblast can Trigger Pyroblast!
            return true;
        // Hack fix Mutilate can Trigger Blindside
        else if (spellProto && spellProto->Id == 121152 && procSpell && procSpell->Id == 1329)
            return true;
        else if (spellProto && spellProto->Id == 76669 && procSpell && procSpell->Id == 156322) ///< Eternal flame dot should proc on Illuminated healing
            return true;
        else if (spellProto && spellProto->Id == 108446 && procSpell && !procSpell->IsPositive()) ///< Soul link should proc on every damage that warlock deal
            return true;
        /// Pyroblast! must make T17 fire 4P bonus procs!
        /// Arcane Charge must make T17 arcane 4P bonus procs!
        else if ((spellProto && spellProto->Id == 165459 && procSpell && procSpell->Id == 48108) ||
            (spellProto && spellProto->Id == 165476 && procSpell && procSpell->Id == 36032))
        {
            /// Nothing to do here
            /// We must use the ProcsPerMinuteRate calculated after that
        }
        else
            return false;
    }

    // In most cases req get honor or XP from kill
    if (EventProcFlag & PROC_FLAG_KILL && IsPlayer())
    {
        bool allow = false;

        if (victim)
            allow = ToPlayer()->isHonorOrXPTarget(victim);

        // Shadow Word: Death - can trigger from every kill
        if (aura->GetId() == 32409)
            allow = true;

        if (!allow)
            return false;
    }
    // Aura added by spell can`t trigger from self (prevent drop charges/do triggers)
    // But except periodic and kill triggers (can triggered from self)
    if (procSpell && procSpell->Id == spellProto->Id
        && !(spellProto->ProcFlags&(PROC_FLAG_TAKEN_PERIODIC | PROC_FLAG_KILL)))
        return false;

    // Check if current equipment allows aura to proc
    if (!isVictim && IsPlayer())
    {
        Player* player = ToPlayer();
        if (spellProto->EquippedItemClass == ITEM_CLASS_WEAPON)
        {
            Item* item = NULL;
            if (attType == WeaponAttackType::BaseAttack || attType == WeaponAttackType::RangedAttack)
                item = player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            else if (attType == WeaponAttackType::OffAttack)
                item = player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

            if (player->IsInFeralForm())
                return false;

            if (!item || item->CantBeUse() || item->GetTemplate()->Class != ITEM_CLASS_WEAPON || !((1 << item->GetTemplate()->SubClass) & spellProto->EquippedItemSubClassMask))
                return false;
        }
        else if (spellProto->EquippedItemClass == ITEM_CLASS_ARMOR)
        {
            // Check if player is wearing shield
            Item* item = player->GetUseableItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (!item || item->CantBeUse() || item->GetTemplate()->Class != ITEM_CLASS_ARMOR || !((1 << item->GetTemplate()->SubClass) & spellProto->EquippedItemSubClassMask))
                return false;
        }
    }

    return true;
}

bool Unit::RollProcResult(Unit* victim, Aura* aura, WeaponAttackType attType, bool isVictim, SpellProcEventEntry const* spellProcEvent)
{
    SpellInfo const* spellInfo = aura->GetSpellInfo();

    // Get chance from spell
    float chance = float(spellInfo->ProcChance);
    // If in spellProcEvent exist custom chance, chance = spellProcEvent->customChance;
    if (spellProcEvent && spellProcEvent->customChance)
        chance = spellProcEvent->customChance;
    // If PPM exist calculate chance from PPM
    float procsPerMinute = spellInfo->ProcBasePPM;
    if (spellProcEvent && spellProcEvent->ppmRate != 0.0f)
        procsPerMinute = spellProcEvent->ppmRate;

    if (procsPerMinute != 0.0f)
        chance = aura->CalcPPMProcChance(procsPerMinute, isVictim ? victim : this);

    // Apply chance modifer aura
    if (Player* modOwner = GetSpellModOwner())
        modOwner->ApplySpellMod(spellInfo->Id, SPELLMOD_CHANCE_OF_SUCCESS, chance);

    return roll_chance_f(chance);
}

bool Unit::HandleAuraRaidProcFromChargeWithValue(AuraEffect* /*triggeredByAura*/)
{
    /// Currently doesn't use by any spell
    return false;
}

bool Unit::HandleAuraRaidProcFromCharge(AuraEffect* triggeredByAura)
{
    // aura can be deleted at casts
    SpellInfo const* spellProto = triggeredByAura->GetSpellInfo();

    uint32 damageSpellId;
    switch (spellProto->Id)
    {
        case 57949:            // shiver
            damageSpellId = 57952;
            //animationSpellId = 57951; dummy effects for jump spell have unknown use (see also 41637)
            break;
        case 59978:            // shiver
            damageSpellId = 59979;
            break;
        case 43593:            // Cold Stare
            damageSpellId = 43594;
            break;
        default:
            return false;
    }

    uint64 caster_guid = triggeredByAura->GetCasterGUID();

    // jumps
    int32 jumps = triggeredByAura->GetBase()->GetCharges()-1;

    // current aura expire
    triggeredByAura->GetBase()->SetCharges(1);             // will removed at next charges decrease

    // next target selection
    if (jumps > 0)
    {
        if (Unit* caster = triggeredByAura->GetCaster())
        {
            float radius = triggeredByAura->GetSpellInfo()->Effects[triggeredByAura->GetEffIndex()].CalcRadius(caster);
            if (Unit* target = GetNextRandomRaidMemberOrPet(radius))
            {
                CastSpell(target, spellProto, true, NULL, triggeredByAura, caster_guid);
                Aura* aura = target->GetAura(spellProto->Id, caster->GetGUID());
                if (aura != nullptr)
                    aura->SetCharges(jumps);
            }
        }
    }

    CastSpell(this, damageSpellId, true, NULL, triggeredByAura, caster_guid);

    return true;
}

void Unit::SendDurabilityLoss(Player* p_Receiver, uint32 p_Percent)
{
    WorldPacket l_Data(SMSG_DURABILITY_DAMAGE_DEATH, 4);
    l_Data << uint32(p_Percent);

    p_Receiver->GetSession()->SendPacket(&l_Data);
}

void Unit::PlayOneShotAnimKit(uint32 id)
{
    WorldPacket l_Data(SMSG_PLAY_ONE_SHOT_ANIM_KIT, 7 + 2);
    l_Data.appendPackGUID(GetGUID());
    l_Data << uint16(id);
    SendMessageToSet(&l_Data, true);
}

void Unit::PlayOrphanSpellVisual(G3D::Vector3 p_Source, G3D::Vector3 p_Orientation, G3D::Vector3 p_Target, int32 p_Visual, float p_TravelSpeed, uint64 p_TargetGuid, bool p_SpeedAsTime)
{
    WorldPacket l_Data(Opcodes::SMSG_PLAY_ORPHAN_SPELL_VISUAL, 100);

    l_Data.WriteVector3(p_Source);
    l_Data.WriteVector3(p_Orientation);
    l_Data.WriteVector3(p_Target);
    l_Data.appendPackGUID(p_TargetGuid);

    l_Data << int32(p_Visual);
    l_Data << float(p_TravelSpeed);
    l_Data << float(0.0f);  ///< UnkFloat

    l_Data.WriteBit(p_SpeedAsTime);
    l_Data.FlushBits();

    SendMessageToSetInRange(&l_Data, GetMap()->GetVisibilityRange(), false);
}

void Unit::CancelOrphanSpellVisual(int32 p_SpellVisualID)
{
    WorldPacket l_Data(Opcodes::SMSG_CANCEL_ORPHAN_SPELL_VISUAL, 4);
    l_Data << int32(p_SpellVisualID);
    SendMessageToSetInRange(&l_Data, GetMap()->GetVisibilityRange(), false);
}

void Unit::Kill(Unit* p_KilledVictim, bool p_DurabilityLoss, SpellInfo const* p_SpellProto)
{
    /// Prevent killing unit twice (and giving reward from kill twice)
    if (!p_KilledVictim->GetHealth() || p_KilledVictim->m_IsInKillingProcess)
        return;

    /// Spirit of Redemption can't be killed twice
    if (p_KilledVictim->HasAura(27827))
        return;

    p_KilledVictim->m_IsInKillingProcess = true;

    /// Find player: owner of controlled `this` or `this` itself maybe
    Player* l_KillerPlayer = GetCharmerOrOwnerPlayerOrPlayerItself();
    Creature* l_KilledCreature = p_KilledVictim->ToCreature();

    bool l_IsRewardAllowed = true;
    if (l_KilledCreature)
    {
        l_IsRewardAllowed = l_KilledCreature->IsDamageEnoughForLootingAndReward();

        /// In Challenge mode difficulty, loots are disabled
        if (!l_IsRewardAllowed || l_KilledCreature->GetMap()->IsChallengeMode())
            l_KilledCreature->SetLootRecipient(NULL);
    }

    if (l_IsRewardAllowed && l_KilledCreature && l_KilledCreature->GetLootRecipient())
        l_KillerPlayer = l_KilledCreature->GetLootRecipient();

    /// Reward player, his pets, and group/raid members
    /// call kill spell proc event (before real die and combat stop to triggering auras removed at death/combat stop)
    if (l_IsRewardAllowed && l_KillerPlayer && l_KillerPlayer != p_KilledVictim)
    {
        WorldPacket l_Data(SMSG_PARTY_KILL_LOG);
        l_Data.appendPackGUID(l_KillerPlayer->GetGUID());
        l_Data.appendPackGUID(p_KilledVictim->GetGUID());

        Player* l_Looter = l_KillerPlayer;
        if (Group* l_Group = l_KillerPlayer->GetGroup())
        {
            if (p_KilledVictim->ToPlayer() && (l_KillerPlayer->GetMapId() == 1116 || l_KillerPlayer->GetMapId() == 1191)) ///< Gladiator's Sanctum
            {
                if (p_KilledVictim->ToPlayer())
                {
                    for (GroupReference* l_Ref = l_Group->GetFirstMember(); l_Ref != nullptr; l_Ref = l_Ref->next())
                    {
                        Player* l_RefPlayer = l_Ref->getSource();

                        if (!l_RefPlayer)
                            continue;

                        if (l_RefPlayer->GetDistance2d(p_KilledVictim) < 100.0f)
                        {
                            if (MS::Garrison::Manager* l_Garr = l_RefPlayer->GetGarrison())
                            {
                                if (l_Garr->HasBuildingType(MS::Garrison::Building::Type::SparringArena))
                                    l_RefPlayer->CastSpell(l_RefPlayer, 173417, true);
                            }
                        }
                    }
                }
            }

            l_Group->BroadcastPacket(&l_Data, l_Group->GetMemberGroup(l_KillerPlayer->GetGUID()));

            if (l_KilledCreature)
            {
                l_Group->UpdateLooterGuid(l_KilledCreature, true);
                if (l_Group->GetLooterGuid())
                {
                    l_Looter = ObjectAccessor::FindPlayer(l_Group->GetLooterGuid());
                    if (l_Looter)
                    {
                        l_KilledCreature->SetLootRecipient(l_Looter);   ///< Update creature loot recipient to the allowed looter.
                        l_Group->SendLooter(l_KilledCreature, l_Looter);
                    }
                    else
                        l_Group->SendLooter(l_KilledCreature, NULL);
                }
                else
                    l_Group->SendLooter(l_KilledCreature, NULL);

                l_Group->UpdateLooterGuid(l_KilledCreature);
            }
        }
        else
        {
            l_KillerPlayer->SendDirectMessage(&l_Data);

            if (p_KilledVictim->ToPlayer() && (l_KillerPlayer->GetMapId() == 1116 || l_KillerPlayer->GetMapId() == 1191))
            {
                if (l_KillerPlayer->GetDistance2d(p_KilledVictim) < 100.0f)
                {
                    if (MS::Garrison::Manager* l_Garr = l_KillerPlayer->GetGarrison())
                    {
                        if (l_Garr->HasBuildingType(MS::Garrison::Building::Type::SparringArena))
                            l_KillerPlayer->CastSpell(l_KillerPlayer, 173417, true);
                    }
                }
            }

            if (l_KilledCreature)
            {
                WorldPacket l_LootListPacket(SMSG_LOOT_LIST);

                l_LootListPacket.appendPackGUID(l_KilledCreature->GetGUID());

                l_LootListPacket.WriteBit(false); // RoundRobinWinnerGuid
                l_LootListPacket.WriteBit(false); // MasterGuid
                l_LootListPacket.FlushBits();

                l_KillerPlayer->SendMessageToSet(&l_LootListPacket, true);
            }
        }

        if (l_KilledCreature)
        {
            Loot* l_Loot = &l_KilledCreature->loot;

            if (l_KilledCreature->lootForPickPocketed)
                l_KilledCreature->lootForPickPocketed = false;

            l_Loot->clear();

            if (uint32 l_LootID = l_KilledCreature->GetCreatureTemplate()->lootid)
                l_Loot->FillLoot(l_LootID, LootTemplates_Creature, l_Looter, false, false, l_KilledCreature->GetLootMode());

            uint32 l_MinGold = l_KilledCreature->GetCreatureTemplate()->mingold;
            uint32 l_MaxGold = l_KilledCreature->GetCreatureTemplate()->maxgold;

            if (Map* l_Map = l_KilledCreature->GetMap())
            {
                if (l_Map->IsRaid())
                {
                    uint32 l_PlayerCount = l_Map->GetPlayersCountExceptGMs();

                    /// Resize item count depending on player count
                    if (l_KilledCreature->isWorldBoss() && l_Map->Expansion() == Expansion::EXPANSION_WARLORDS_OF_DRAENOR && !l_Loot->Items.empty())
                    {
                        /// Assuming we have one loot per 5 players
                        uint8 l_Count = std::max((uint8)1, (uint8)ceil((float)l_PlayerCount / 5));
                        std::vector<LootItem> l_RealLoots;

                        if (l_Loot->Items.size() > l_Count)
                        {
                            std::random_device l_RandomDevice;
                            std::mt19937 l_RandomGenerator(l_RandomDevice());
                            std::shuffle(l_Loot->Items.begin(), l_Loot->Items.end(), l_RandomGenerator);

                            for (LootItem l_LootItem : l_Loot->Items)
                            {
                                if (!l_Count)
                                    break;

                                --l_Count;
                                l_RealLoots.push_back(l_LootItem);
                            }

                            l_Loot->UnlootedCount = 0;
                            l_Loot->Items.clear();
                        }

                        l_KilledCreature->SetLootMode(LootModes::LOOT_MODE_HARD_MODE_1);

                        /// If at least 15 players, 25% chance to have a third set token
                        if (l_PlayerCount >= 15 && roll_chance_i(25))
                            l_KilledCreature->AddLootMode(LootModes::LOOT_MODE_HARD_MODE_2);

                        /// If at least 20 players, 10% chance to have a fourth set token
                        if (l_PlayerCount >= 20 && roll_chance_i(10))
                            l_KilledCreature->AddLootMode(LootModes::LOOT_MODE_HARD_MODE_3);

                        /// We must do that again to add set tokens
                        if (uint32 l_LootID = l_KilledCreature->GetCreatureTemplate()->lootid)
                            l_Loot->FillLoot(l_LootID, LootTemplates_Creature, l_Looter, false, false, l_KilledCreature->GetLootMode());

                        for (LootItem l_Item : l_RealLoots)
                        {
                            ++l_Loot->UnlootedCount;
                            l_Loot->Items.push_back(l_Item);
                        }
                    }

                    uint32 l_MaxPlayers = l_KilledCreature->GetMap()->GetEntry()->MaxPlayers;
                    if (l_MaxPlayers < 1)
                        l_MaxPlayers = 1;

                    l_MinGold /= l_MaxPlayers;
                    l_MaxGold /= l_MaxPlayers;
                }
            }

            l_Loot->generateMoneyLoot(l_MinGold, l_MaxGold);
        }

        l_KillerPlayer->RewardPersonnalCurrencies(p_KilledVictim);
        l_KillerPlayer->RewardPlayerAndGroupAtKill(p_KilledVictim, false);
    }

    /// Do KILL and KILLED procs. KILL proc is called only for the unit who landed the killing blow (and its owner - for pets and totems) regardless of who tapped the victim
    if (isPet() || isTotem())
    {
        if (Unit* l_Owner = GetOwner())
            l_Owner->ProcDamageAndSpell(p_KilledVictim, PROC_FLAG_KILL, PROC_FLAG_NONE, PROC_EX_NONE, 0);
    }

    if (p_KilledVictim->GetCreatureType() != CREATURE_TYPE_CRITTER)
    {
        if (p_KilledVictim->GetEntry() != 19833 && p_KilledVictim->GetEntry() != 19921) // snake trap can't trigger PROC_FLAG_KILL
            ProcDamageAndSpell(p_KilledVictim, PROC_FLAG_KILL, PROC_FLAG_KILLED, PROC_EX_NONE, 0, 0, WeaponAttackType::BaseAttack, p_SpellProto ? p_SpellProto : NULL, NULL);
    }

    if (p_KilledVictim->ToCreature() && IsPlayer())
    {
        ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE, 1, 0, 0, p_KilledVictim);

        if (Guild* l_Guild = ToPlayer()->GetGuild())
            l_Guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE_GUILD, 1, 0, 0, p_KilledVictim, ToPlayer());
    }

    /// Proc auras on death - must be before aura/combat remove
    p_KilledVictim->ProcDamageAndSpell(NULL, PROC_FLAG_DEATH, PROC_FLAG_NONE, PROC_EX_NONE, 0, 0, WeaponAttackType::BaseAttack, p_SpellProto ? p_SpellProto : NULL);

    /// update get killing blow achievements, must be done before setDeathState to be able to require auras on target
    /// and before Spirit of Redemption as it also removes auras
    if (l_KillerPlayer)
        l_KillerPlayer->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS, 1, 0, 0, p_KilledVictim);

    /// if talent known but not triggered (check priest class for speedup check)
    bool l_SpiritOfRedemption = false;

    if (p_KilledVictim->IsPlayer() && p_KilledVictim->getClass() == CLASS_PRIEST)
    {
        AuraEffectList const& l_DummyAuras = p_KilledVictim->GetAuraEffectsByType(SPELL_AURA_DUMMY);

        for (AuraEffectList::const_iterator l_AuraIT = l_DummyAuras.begin(); l_AuraIT != l_DummyAuras.end(); ++l_AuraIT)
        {
            if ((*l_AuraIT)->GetSpellInfo()->SpellIconID == 1654)
            {
                AuraEffect const* l_AuraEffect = *l_AuraIT;

                // save value before aura remove
                uint32 l_RessSpellId = p_KilledVictim->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL);

                if (!l_RessSpellId)
                    l_RessSpellId = p_KilledVictim->ToPlayer()->GetResurrectionSpellId();

                /// Remove all expected to remove at death auras (most important negative case like DoT or periodic triggers)
                p_KilledVictim->RemoveAllAurasOnDeath();
                /// restore for use at real death
                p_KilledVictim->SetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL, l_RessSpellId);

                /// FORM_SPIRITOFREDEMPTION and related auras
                p_KilledVictim->CastSpell(p_KilledVictim, 27827, true, NULL, l_AuraEffect);
                p_KilledVictim->CastSpell(p_KilledVictim, 27792, true);

                l_SpiritOfRedemption = true;
                break;
            }
        }
    }

    if (!l_SpiritOfRedemption)
        p_KilledVictim->setDeathState(JUST_DIED);

    /// Inform pets (if any) when player kills target)
    /// MUST come after victim->setDeathState(JUST_DIED); or pet next target
    /// selection will get stuck on same target and break pet react state
    if (l_KillerPlayer)
    {
        Pet* l_Pet = l_KillerPlayer->GetPet();

        if (l_Pet && l_Pet->isAlive() && l_Pet->isControlled())
            l_Pet->AI()->KilledUnit(p_KilledVictim);
    }

    /// 10% durability loss on death
    /// clean InHateListOf
    if (Player* l_PlayerVictim = p_KilledVictim->ToPlayer())
    {
        /// Remember victim PvP death for corpse type and corpse reclaim delay
        /// at original death (not at SpiritOfRedemtionTalent timeout)
        l_PlayerVictim->SetPvPDeath(l_KillerPlayer != NULL);

        /// There is no durability loss in challenge dungeons
        if (l_PlayerVictim->GetMap()->IsChallengeMode())
            p_DurabilityLoss = false;

        /// Only if not player and not controlled by player pet. And not at BG
        if ((p_DurabilityLoss && !l_KillerPlayer && !p_KilledVictim->ToPlayer()->InBattleground()) || (l_KillerPlayer && sWorld->getBoolConfig(CONFIG_DURABILITY_LOSS_IN_PVP)))
        {
            double l_BaseLoss = sWorld->getRate(RATE_DURABILITY_LOSS_ON_DEATH);
            uint32 l_Loss = uint32(l_BaseLoss - (l_BaseLoss * l_PlayerVictim->GetTotalAuraMultiplier(SPELL_AURA_MOD_DURABILITY_LOSS)));

            /// Durability loss is calculated more accurately again for each item in Player::DurabilityLoss
            l_PlayerVictim->DurabilityLossAll(l_BaseLoss, false);
            /// Durability lost message
            SendDurabilityLoss(l_PlayerVictim, l_Loss);
        }

        /// Call KilledUnit for creatures
        if (GetTypeId() == TYPEID_UNIT && IsAIEnabled)
            ToCreature()->AI()->KilledUnit(p_KilledVictim);

        /// last damage from non duel opponent or opponent controlled creature
        if (l_PlayerVictim->m_Duel)
        {
            l_PlayerVictim->m_Duel->opponent->CombatStopWithPets(true);
            l_PlayerVictim->CombatStopWithPets(true);
            l_PlayerVictim->DuelComplete(DUEL_INTERRUPTED);
        }

        l_PlayerVictim->SendClearLossOfControl();
    }
    /// Creature died
    else if (l_KilledCreature)
    {
        if (!l_KilledCreature->isPet())
        {
            l_KilledCreature->DeleteThreatList();

            CreatureTemplate const* l_CreatureTemplate = l_KilledCreature->GetCreatureTemplate();
            if (l_CreatureTemplate && (l_CreatureTemplate->lootid || l_CreatureTemplate->maxgold > 0))
                l_KilledCreature->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
        }

        /// Call KilledUnit for creatures, this needs to be called after the lootable flag is set
        if (GetTypeId() == TYPEID_UNIT && IsAIEnabled)
            ToCreature()->AI()->KilledUnit(p_KilledVictim);

        /// Call creature just died function
        if (l_KilledCreature->IsAIEnabled)
            l_KilledCreature->AI()->JustDied(this);

        if (TempSummon* l_TemSummon = l_KilledCreature->ToTempSummon())
        {
            if (Unit* l_Summoner = l_TemSummon->GetSummoner())
            {
                if (l_Summoner->ToCreature() && l_Summoner->IsAIEnabled)
                    l_Summoner->ToCreature()->AI()->SummonedCreatureDies(l_KilledCreature, this);
            }
        }

        /// Dungeon specific stuff, only applies to players killing creatures
        if (l_KilledCreature->GetInstanceId() && l_KillerPlayer)
        {
            Map* l_InstanceMap = l_KilledCreature->GetMap();

            if (l_InstanceMap->IsLFR())
            {
                Map::PlayerList const& l_PlayerList = l_InstanceMap->GetPlayers();
                if (l_PlayerList.isEmpty())
                    return;

                /// Handle end of dungeon rewarding for LFR
                if (l_KilledCreature->GetNativeTemplate()->flags_extra & CreatureFlagsExtra::CREATURE_FLAG_EXTRA_DUNGEON_END_BOSS)
                {
                    for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = l_Itr->getSource())
                        {
                            uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                            if (!l_KilledCreature || l_Player->IsAtGroupRewardDistance(l_KilledCreature))
                                sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);
                        }
                    }
                }

                /// Handle loot assignation for LFR
                Player* l_Player = l_PlayerList.begin()->getSource();
                if (l_Player && l_Player->GetGroup())
                    sLFGMgr->AutomaticLootAssignation(l_KilledCreature, l_Player->GetGroup());
            }

            /// @TODO: do instance binding anyway if the charmer/owner is offline
            if (l_InstanceMap->IsDungeon() && l_KillerPlayer)
            {
                if (InstanceScript* l_InstanceScript = l_KilledCreature->GetInstanceScript())
                    l_InstanceScript->OnCreatureKilled(l_KilledCreature, l_KillerPlayer);

                /// There is no lockout scheduled in challenge mode
                if (l_InstanceMap->IsRaidOrHeroicDungeon() && !l_InstanceMap->IsChallengeMode())
                {
                    if (l_KilledCreature->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
                        ((InstanceMap*)l_InstanceMap)->PermBindAllPlayers(l_KillerPlayer);
                }
                else
                {
                    /// the reset time is set but not added to the scheduler
                    /// until the players leave the instance
                    time_t l_ResetTime = l_KilledCreature->GetRespawnTimeEx() + 2 * HOUR;

                    if (InstanceSave* l_Save = sInstanceSaveMgr->GetInstanceSave(l_KilledCreature->GetInstanceId()))
                        if (l_Save->GetResetTime() < l_ResetTime) l_Save->SetResetTime(l_ResetTime);
                }
            }
        }

        float l_GroundZ = l_KilledCreature->GetMap()->GetHeight(l_KilledCreature->m_positionX, l_KilledCreature->m_positionY, l_KilledCreature->m_positionZ, true, 200.0f);

        if (l_KilledCreature->IsFlying() && l_GroundZ != INVALID_HEIGHT && l_GroundZ != l_KilledCreature->m_positionZ)
        {
            Position l_Position = *l_KilledCreature;
            l_Position.m_positionZ = l_GroundZ;

            l_KilledCreature->GetMotionMaster()->Clear();
            l_KilledCreature->GetMotionMaster()->MoveLand(EventId::EVENT_FALL_TO_GROUND, l_Position);
        }
    }

    /// Outdoor pvp things, do these after setting the death state, else the player activity notify won't work... doh...
    /// handle player kill only if not suicide (spirit of redemption for example)
    if (l_KillerPlayer && this != p_KilledVictim)
    {
        if (OutdoorPvP* l_OutdoorPVP = l_KillerPlayer->GetOutdoorPvP())
            l_OutdoorPVP->HandleKill(l_KillerPlayer, p_KilledVictim);

        if (Battlefield* l_Battlefield = sBattlefieldMgr->GetBattlefieldToZoneId(l_KillerPlayer->GetZoneId()))
            l_Battlefield->HandleKill(l_KillerPlayer, p_KilledVictim);
    }

    if (this != p_KilledVictim && p_KilledVictim->IsPlayer())
    {
        if (OutdoorPvP* l_OutdoorPvP = p_KilledVictim->ToPlayer()->GetOutdoorPvP())
            l_OutdoorPvP->HandlePlayerKilled(p_KilledVictim->ToPlayer());
    }

    /// Battleground things (do this at the end, so the death state flag will be properly set to handle in the bg->handlekill)
    if (l_KillerPlayer && l_KillerPlayer->InBattleground())
    {
        if (Battleground* l_Battleground = l_KillerPlayer->GetBattleground())
        {
            if (p_KilledVictim->IsPlayer())
                l_Battleground->HandleKillPlayer((Player*)p_KilledVictim, l_KillerPlayer);
            else
                l_Battleground->HandleKillUnit(p_KilledVictim->ToCreature(), l_KillerPlayer);
        }
    }

    /// Achievement stuff
    if (p_KilledVictim->IsPlayer())
    {
        if (GetTypeId() == TYPEID_UNIT)
            p_KilledVictim->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE, GetEntry());
        else if (IsPlayer() && p_KilledVictim != this)
            p_KilledVictim->ToPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER, 1, ToPlayer()->GetTeam());
    }

    /// Hook for OnPVPKill Event
    if (Player* l_KillerPlayer = ToPlayer())
    {
        sScriptMgr->OnKill(l_KillerPlayer, p_KilledVictim);
        if (Player* l_KilledPlayer = p_KilledVictim->ToPlayer())
            sScriptMgr->OnPVPKill(l_KillerPlayer, l_KilledPlayer);
        else if (Creature* l_KilledCreature = p_KilledVictim->ToCreature())
            sScriptMgr->OnCreatureKill(l_KillerPlayer, l_KilledCreature);
    }
    else if (Creature* l_KilledCreature = ToCreature())
    {
        if (Player* l_KilledPlayer = p_KilledVictim->ToPlayer())
            sScriptMgr->OnPlayerKilledByCreature(l_KilledCreature, l_KilledPlayer);
    }

    p_KilledVictim->m_IsInKillingProcess = false;
}

/// Get interpolated player position based on last received movement informations
/// @p_AtClientScreen : Interpolated with client network delay ?
/// @p_ProjectTime    : Time target of prediction
Position Unit::GetInterpolatedPosition(bool p_AtClientScreen, uint32 p_ProjectTime)
{
    if (!IsMoving())
        return *this;

    if ((m_movementInfo.GetMovementFlags() & (MOVEMENTFLAG_MASK_MOVING_FLY | MOVEMENTFLAG_MASK_TURNING | MOVEMENTFLAG_PITCH_UP | MOVEMENTFLAG_PITCH_DOWN | MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR | MOVEMENTFLAG_SPLINE_ELEVATION)) != 0)
        return *this;

    Position l_InterpolatedPosition;

    uint32 l_LastMoveTimeStamp = m_movementInfoLastTime;
    uint32 l_CurrentTime = getMSTime() + p_ProjectTime;

    if (p_AtClientScreen && GetTypeId() == TYPEID_PLAYER)
        l_CurrentTime += ToPlayer()->GetSession()->GetLatency();

    float l_Orientation = m_movementInfo.pos.m_orientation;
    float l_Distance = (l_CurrentTime - l_LastMoveTimeStamp) / 1000.0f;

    uint32 l_MovementFlags = m_movementInfo.GetMovementFlags();

    if ((l_MovementFlags & MOVEMENTFLAG_STRAFE_LEFT) != 0)
        l_Orientation += M_PI / 2.0f;
    else if ((l_MovementFlags & MOVEMENTFLAG_STRAFE_RIGHT) != 0)
        l_Orientation -= M_PI / 2.0f;
    else if ((l_MovementFlags & MOVEMENTFLAG_BACKWARD) != 0)
        l_Orientation -= M_PI;

    l_Orientation = NormalizeOrientation(l_Orientation);

    bool l_IsBackward = ((l_MovementFlags & MOVEMENTFLAG_BACKWARD) != 0);

    if ((l_MovementFlags & MOVEMENTFLAG_WALKING) != 0)
        l_Distance *= GetSpeed(MOVE_WALK);
    else if ((l_MovementFlags & MOVEMENTFLAG_FLYING) != 0)
        l_Distance *= GetSpeed(l_IsBackward ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
    else if ((l_MovementFlags & MOVEMENTFLAG_SWIMMING) != 0)
        l_Distance *= GetSpeed(l_IsBackward ? MOVE_SWIM_BACK : MOVE_SWIM);
    else if ((l_MovementFlags & MOVEMENTFLAG_MASK_TURNING) == 0)
        l_Distance *= GetSpeed(l_IsBackward ? MOVE_RUN_BACK : MOVE_RUN);

    l_InterpolatedPosition.m_positionX = m_movementInfo.pos.m_positionX + (std::cos(l_Orientation) * l_Distance);
    l_InterpolatedPosition.m_positionY = m_movementInfo.pos.m_positionY + (std::sin(l_Orientation) * l_Distance);
    l_InterpolatedPosition.m_positionZ = GetMap()->GetHeight(l_InterpolatedPosition.m_positionX, l_InterpolatedPosition.m_positionY, MAX_HEIGHT);
    l_InterpolatedPosition.m_orientation = m_movementInfo.pos.m_orientation;

    return l_InterpolatedPosition;
}

void Unit::SetControlled(bool apply, UnitState state)
{
    if (apply)
    {
        if (HasUnitState(state))
            return;

        AddUnitState(state);
        switch (state)
        {
            case UNIT_STATE_STUNNED:
                SetStunned(true);
                CastStop();
                break;
            case UNIT_STATE_ROOT:
                if (!HasUnitState(UNIT_STATE_STUNNED))
                    SetRooted(true);
                break;
            case UNIT_STATE_CONFUSED:
                if (!HasUnitState(UNIT_STATE_STUNNED))
                {
                    ClearUnitState(UNIT_STATE_MELEE_ATTACKING);
                    SendMeleeAttackStop();
                    // SendAutoRepeatCancel ?
                    SetConfused(true);
                    CastStop();
                }
                break;
            case UNIT_STATE_FLEEING:
                if (!HasUnitState(UNIT_STATE_STUNNED | UNIT_STATE_CONFUSED))
                {
                    ClearUnitState(UNIT_STATE_MELEE_ATTACKING);
                    SendMeleeAttackStop();
                    // SendAutoRepeatCancel ?
                    SetFeared(true);
                    CastStop();
                }
                break;
            default:
                break;
        }
    }
    else
    {
        switch (state)
        {
            case UNIT_STATE_STUNNED:
                if (HasAuraType(SPELL_AURA_MOD_STUN))
                    return;
                else
                    SetStunned(false);
                break;
            case UNIT_STATE_ROOT:
                if (HasAuraType(SPELL_AURA_MOD_ROOT) || HasAuraType(SPELL_AURA_MOD_ROOT_2) || GetVehicle())
                    return;
                else
                    SetRooted(false);
                break;
            case UNIT_STATE_CONFUSED:
                if (HasAuraType(SPELL_AURA_MOD_CONFUSE))
                    return;
                else
                    SetConfused(false);
                break;
            case UNIT_STATE_FLEEING:
                if (isFeared())
                    return;
                else
                    SetFeared(false);
                break;
            default:
                return;
        }

        ClearUnitState(state);

        /// Fix problems with CC when character is not available for control in 1-2 seconds after breaking CC
        /// After ClearUnitState player should receive control on his character, if char is still moving - we need to stop him
        StopMoving();

        if (HasUnitState(UNIT_STATE_STUNNED))
            SetStunned(true);
        else
        {
            if (HasUnitState(UNIT_STATE_ROOT))
                SetRooted(true);

            if (HasUnitState(UNIT_STATE_CONFUSED))
                SetConfused(true);
            else if (HasUnitState(UNIT_STATE_FLEEING))
                SetFeared(true);
        }
    }
}

/// Control Alert
void Unit::SendLossOfControlAuraUpdate(AuraApplication const* p_AurApp, Mechanics p_Mechanic, SpellEffIndex p_EffIndex, LossOfControlType p_Type)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    if (p_AurApp->GetBase()->GetSpellInfo()->Attributes & SPELL_ATTR0_HIDE_IN_COMBAT_LOG)
        return;

    WorldPacket l_Data(SMSG_LOSS_OF_CONTROL_AURA_UPDATE);
    l_Data << uint32(1);
    l_Data << uint8(p_AurApp->GetSlot());
    l_Data << uint8(p_EffIndex);
    l_Data << uint8(p_Type);
    l_Data << uint8(p_Mechanic);
    ToPlayer()->GetSession()->SendPacket(&l_Data);
}

void Unit::SendClearLossOfControl()
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    WorldPacket l_Data(SMSG_CLEAR_LOSS_OF_CONTROL);
    ToPlayer()->GetSession()->SendPacket(&l_Data);
}

void Unit::SendAddLossOfControl(AuraApplication const* p_AurApp, Mechanics p_Mechanic, LossOfControlType p_Type)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    Aura* l_Aura = p_AurApp->GetBase();
    if (l_Aura == nullptr)
        return;

    WorldPacket l_Data(SMSG_ADD_LOSS_OF_CONTROL);
    l_Data << uint8(p_Type);
    l_Data << uint8(p_Mechanic);
    l_Data << int32(l_Aura->GetSpellInfo()->Id);
    l_Data.appendPackGUID(l_Aura->GetCasterGUID());
    l_Data << uint32(l_Aura->GetDuration());
    l_Data << uint32(0);    ///< DurationRemainingLockoutSchoolMask
    ToPlayer()->GetSession()->SendPacket(&l_Data);
}

void Unit::SendRemoveLossOfControl(AuraApplication const* p_AurApp, LossOfControlType p_Type)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    Aura* l_Aura = p_AurApp->GetBase();
    if (l_Aura == nullptr)
        return;

    WorldPacket l_Data(SMSG_REMOVE_LOSS_OF_CONTROL);
    l_Data << uint8(p_Type);
    l_Data << int32(l_Aura->GetSpellInfo()->Id);
    l_Data.appendPackGUID(l_Aura->GetCasterGUID());
    ToPlayer()->GetSession()->SendPacket(&l_Data);
}

void Unit::SendMoveRoot(uint32 value)
{
    WorldPacket data(SMSG_MOVE_ROOT, 16 + 2 + 4);

    data.appendPackGUID(GetGUID());
    data << uint32(value);

    SendMessageToSet(&data, true);
}

void Unit::SendMoveUnroot(uint32 value)
{
    WorldPacket data(SMSG_MOVE_UNROOT, 16 + 2 + 4);

    data.appendPackGUID(GetGUID());
    data << uint32(value);

    SendMessageToSet(&data, true);
}

void Unit::SetStunned(bool apply)
{
    if (apply)
    {
        SetTarget(0);
        SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

        // MOVEMENTFLAG_ROOT cannot be used in conjunction with MOVEMENTFLAG_MASK_MOVING (tested 3.3.5a)
        // this will freeze clients. That's why we remove MOVEMENTFLAG_MASK_MOVING before
        // setting MOVEMENTFLAG_ROOT
        RemoveUnitMovementFlag(MOVEMENTFLAG_MASK_MOVING);
        AddUnitMovementFlag(MOVEMENTFLAG_ROOT);

        // Creature specific
        if (GetTypeId() != TYPEID_PLAYER)
            ToCreature()->StopMoving();
        else
            SetStandState(UNIT_STAND_STATE_STAND);

        SendMoveRoot(0);

        CastStop();
    }
    else
    {
        if (isAlive() && getVictim())
            SetTarget(getVictim()->GetGUID());

        // don't remove UNIT_FLAG_STUNNED for pet when owner is mounted (disabled pet's interface)
        Unit* owner = GetOwner();
        if (!owner || (owner->IsPlayer() && !owner->ToPlayer()->IsMounted()))
            RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

        if (!HasUnitState(UNIT_STATE_ROOT))         // prevent moving if it also has root effect
        {
            SendMoveUnroot(0);
            RemoveUnitMovementFlag(MOVEMENTFLAG_ROOT);
        }
    }
}

void Unit::SetRooted(bool apply)
{
    if (apply)
    {
        if (m_rootTimes > 0) // blizzard internal check?
            m_rootTimes++;

        // MOVEMENTFLAG_ROOT cannot be used in conjunction with MOVEMENTFLAG_MASK_MOVING (tested 3.3.5a)
        // this will freeze clients. That's why we remove MOVEMENTFLAG_MASK_MOVING before
        // setting MOVEMENTFLAG_ROOT
        RemoveUnitMovementFlag(MOVEMENTFLAG_MASK_MOVING);
        AddUnitMovementFlag(MOVEMENTFLAG_ROOT);

        if (IsPlayer())
            SendMoveRoot(m_rootTimes);
        else
        {
            ObjectGuid guid = GetGUID();
            WorldPacket data(SMSG_SPLINE_MOVE_ROOT, 16 + 2);
            data.appendPackGUID(guid);
            SendMessageToSet(&data, true);
            StopMoving();
        }
    }
    else
    {
        if (!HasUnitState(UNIT_STATE_STUNNED))      // prevent moving if it also has stun effect
        {
            if (IsPlayer())
                SendMoveUnroot(++m_rootTimes);
            else
            {
                ObjectGuid guid = GetGUID();
                WorldPacket data(SMSG_SPLINE_MOVE_UNROOT, 16 + 2);
                data.appendPackGUID(guid);
                SendMessageToSet(&data, true);
            }

            RemoveUnitMovementFlag(MOVEMENTFLAG_ROOT);
        }
    }
}

void Unit::SetFeared(bool apply)
{
    if (apply)
    {
        SetTarget(0);

        uint32 mechanic_mask = (1 << MECHANIC_FEAR) | (1 << MECHANIC_HORROR) | (1 << MECHANIC_TURN);

        Unit* caster = NULL;
        Unit::AuraEffectList const& fearAuras = GetAuraEffectsByMechanic(mechanic_mask);
        if (!fearAuras.empty())
            caster = ObjectAccessor::GetUnit(*this, fearAuras.front()->GetCasterGUID());
        if (!caster)
            caster = getAttackerForHelper();
        GetMotionMaster()->MoveFleeing(caster, fearAuras.empty() ? sWorld->getIntConfig(CONFIG_CREATURE_FAMILY_FLEE_DELAY) : 0);             // caster == NULL processed in MoveFleeing
    }
    else
    {
        if (isAlive())
        {
            if (GetMotionMaster()->GetCurrentMovementGeneratorType() == FLEEING_MOTION_TYPE)
                GetMotionMaster()->MovementExpired();
            if (getVictim())
                SetTarget(getVictim()->GetGUID());
        }
    }

    if (Player* player = ToPlayer())
        if(!player->HasUnitState(UNIT_STATE_POSSESSED))
            player->SetClientControl(this, !apply);
}

void Unit::SetConfused(bool apply)
{
    if (apply)
    {
        SetTarget(0);
        GetMotionMaster()->MoveConfused();
    }
    else
    {
        if (isAlive())
        {
            if (GetMotionMaster()->GetCurrentMovementGeneratorType() == CONFUSED_MOTION_TYPE)
                GetMotionMaster()->MovementExpired();
            if (getVictim())
                SetTarget(getVictim()->GetGUID());
        }
    }

    if (Player* player = ToPlayer())
        if (!player->HasUnitState(UNIT_STATE_POSSESSED))
            player->SetClientControl(this, !apply);
}

bool Unit::SetCharmedBy(Unit* charmer, CharmType type, AuraApplication const* aurApp)
{
    if (!charmer)
        return false;

    // dismount players when charmed
    if (IsPlayer())
        Dismount();

    ASSERT(type != CHARM_TYPE_POSSESS || charmer->IsPlayer());
    ASSERT((type == CHARM_TYPE_VEHICLE) == IsVehicle());

    if (this == charmer)
    {
        sLog->outFatal(LOG_FILTER_UNITS, "Unit::SetCharmedBy: Unit %u (GUID %u) is trying to charm itself!", GetEntry(), GetGUIDLow());
        return false;
    }

    //if (HasUnitState(UNIT_STATE_UNATTACKABLE))
    //    return false;

    if (IsPlayer() && ToPlayer()->GetTransport())
        return false;

    // Already charmed
    if (GetCharmerGUID())
        return false;

    CastStop();
    CombatStop(); // TODO: CombatStop(true) may cause crash (interrupt spells)
    DeleteThreatList();

    // Charmer stop charming
    if (charmer->IsPlayer())
    {
        charmer->ToPlayer()->StopCastingCharm();
        charmer->ToPlayer()->StopCastingBindSight();
    }

    // Charmed stop charming
    if (IsPlayer())
    {
        ToPlayer()->StopCastingCharm();
        ToPlayer()->StopCastingBindSight();
    }

    // StopCastingCharm may remove a possessed pet?
    if (!IsInWorld())
        return false;

    // charm is set by aura, and aura effect remove handler was called during apply handler execution
    // prevent undefined behaviour
    if (aurApp && aurApp->GetRemoveMode())
        return false;

    // Set charmed
    Map* map = GetMap();
    if (!IsVehicle() || (IsVehicle() && map && !map->IsBattleground()))
        setFaction(charmer->getFaction());

    charmer->SetCharm(this, true);

    if (GetTypeId() == TYPEID_UNIT)
    {
        ToCreature()->AI()->OnCharmed(true);
        GetMotionMaster()->MoveIdle();
    }
    else
    {
        Player* player = ToPlayer();
        if (player->isAFK())
            player->ToggleAFK();
        player->SetClientControl(this, 0);
    }

    // charm is set by aura, and aura effect remove handler was called during apply handler execution
    // prevent undefined behaviour
    if (aurApp && aurApp->GetRemoveMode())
        return false;

    // Pets already have a properly initialized CharmInfo, don't overwrite it.
    if (type != CHARM_TYPE_VEHICLE && !GetCharmInfo())
        InitCharmInfo();

    if (type != CHARM_TYPE_VEHICLE)
    {
        if (type == CHARM_TYPE_POSSESS)
            GetCharmInfo()->InitPossessCreateSpells();
        else
            GetCharmInfo()->InitCharmCreateSpells();
    }

    if (charmer->IsPlayer())
    {
        switch (type)
        {
            case CHARM_TYPE_VEHICLE:
                SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
                charmer->ToPlayer()->SetClientControl(this, 1);
                charmer->ToPlayer()->SetMover(this);
                charmer->ToPlayer()->SetViewpoint(this, true);
                charmer->ToPlayer()->VehicleSpellInitialize();
                break;
            case CHARM_TYPE_POSSESS:
                AddUnitState(UNIT_STATE_POSSESSED);
                SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
                charmer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                charmer->ToPlayer()->SetClientControl(this, 1);
                charmer->ToPlayer()->SetMover(this);
                charmer->ToPlayer()->SetViewpoint(this, true);
                charmer->ToPlayer()->PossessSpellInitialize();
                break;
            case CHARM_TYPE_CHARM:
                if (GetTypeId() == TYPEID_UNIT && charmer->getClass() == CLASS_WARLOCK)
                {
                    CreatureTemplate const* cinfo = ToCreature()->GetCreatureTemplate();
                    if (cinfo && cinfo->type == CREATURE_TYPE_DEMON)
                    {
                        // to prevent client crash
                        SetByteValue(UNIT_FIELD_SEX, 1, (uint8)CLASS_MAGE);

                        // just to enable stat window
                        if (GetCharmInfo())
                            GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);

                        // if charmed two demons the same session, the 2nd gets the 1st one's name
                        SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL))); // cast can't be helped
                    }
                }
                charmer->ToPlayer()->CharmSpellInitialize();
                break;
            default:
            case CHARM_TYPE_CONVERT:
                break;
        }
    }
    return true;
}

void Unit::RemoveCharmedBy(Unit* charmer)
{
    if (!isCharmed())
        return;

    if (!charmer)
        charmer = GetCharmer();
    if (charmer != GetCharmer()) // one aura overrides another?
    {
//        sLog->outFatal(LOG_FILTER_UNITS, "Unit::RemoveCharmedBy: this: " UI64FMTD " true charmer: " UI64FMTD " false charmer: " UI64FMTD,
//            GetGUID(), GetCharmerGUID(), charmer->GetGUID());
//        ASSERT(false);
        return;
    }

    CharmType type;
    if (HasUnitState(UNIT_STATE_POSSESSED))
        type = CHARM_TYPE_POSSESS;
    else if (charmer && charmer->IsOnVehicle(this))
        type = CHARM_TYPE_VEHICLE;
    else
        type = CHARM_TYPE_CHARM;

    if (type == CHARM_TYPE_VEHICLE && GetMapSwitchDestination() != -1)
        return;

    CastStop();
    CombatStop(); // TODO: CombatStop(true) may cause crash (interrupt spells)
    AttackStop();
    getHostileRefManager().deleteReferences();
    DeleteThreatList();
    Map* map = GetMap();
    if (!IsVehicle() || (IsVehicle() && map && !map->IsBattleground()))
        RestoreFaction();
    GetMotionMaster()->Clear(true);
    GetMotionMaster()->InitDefault();

    if (type == CHARM_TYPE_POSSESS)
    {
        ClearUnitState(UNIT_STATE_POSSESSED);
        RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
    }

    if (Creature* creature = ToCreature())
    {
        if (creature->AI())
            creature->AI()->OnCharmed(false);

        if (type != CHARM_TYPE_VEHICLE) // Vehicles' AI is never modified
        {
            creature->AIM_Initialize();

            if (creature->AI() && charmer && charmer->isAlive())
                creature->AI()->AttackStart(charmer);
        }
    }
    else
        ToPlayer()->SetClientControl(this, 1);

    // If charmer still exists
    if (!charmer)
        return;

    ASSERT(type != CHARM_TYPE_POSSESS || charmer->IsPlayer());
    ASSERT(type != CHARM_TYPE_VEHICLE || (GetTypeId() == TYPEID_UNIT && IsVehicle()));

    charmer->SetCharm(this, false);

    if (charmer->IsPlayer())
    {
        switch (type)
        {
            case CHARM_TYPE_VEHICLE:
                charmer->ToPlayer()->SetClientControl(charmer, 1);
                charmer->ToPlayer()->SetViewpoint(this, false);
                charmer->ToPlayer()->SetClientControl(this, 0);
                if (IsPlayer())
                    ToPlayer()->SetMover(this);
                break;
            case CHARM_TYPE_POSSESS:
                ClearUnitState(UNIT_STATE_POSSESSED);
                charmer->ToPlayer()->SetClientControl(charmer, 1);
                charmer->ToPlayer()->SetViewpoint(this, false);
                charmer->ToPlayer()->SetClientControl(this, 0);
                charmer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                if (IsPlayer())
                {
                    ToPlayer()->SetClientControl(this, 1);
                }
                break;
            case CHARM_TYPE_CHARM:
                if (GetTypeId() == TYPEID_UNIT && charmer->getClass() == CLASS_WARLOCK)
                {
                    CreatureTemplate const* cinfo = ToCreature()->GetCreatureTemplate();
                    if (cinfo && cinfo->type == CREATURE_TYPE_DEMON)
                    {
                        SetByteValue(UNIT_FIELD_SEX, 1, uint8(cinfo->unit_class));
                        if (GetCharmInfo())
                            GetCharmInfo()->SetPetNumber(0, true);
                    }
                }
                break;
            default:
            case CHARM_TYPE_CONVERT:
                break;
        }
    }

    // a guardian should always have charminfo
    if (charmer->IsPlayer() && this != charmer->GetFirstControlled())
        charmer->ToPlayer()->SendRemoveControlBar();
    else if (IsPlayer() || (GetTypeId() == TYPEID_UNIT && !ToCreature()->isGuardian()))
        DeleteCharmInfo();
}

void Unit::RestoreFaction()
{
    if (IsPlayer())
        ToPlayer()->setFactionForRace(getRace());
    else
    {
        if (HasUnitTypeMask(UNIT_MASK_MINION))
        {
            if (Unit* owner = GetOwner())
            {
                setFaction(owner->getFaction());
                return;
            }
        }

        if (CreatureTemplate const* cinfo = ToCreature()->GetCreatureTemplate())  // normal creature
            setFaction(cinfo->faction);
    }
}

bool Unit::CreateVehicleKit(uint32 id, uint32 creatureEntry)
{
    VehicleEntry const* vehInfo = sVehicleStore.LookupEntry(id);
    if (!vehInfo)
        return false;

    m_vehicleKit = new Vehicle(this, vehInfo, creatureEntry);
    m_updateFlag |= UPDATEFLAG_HAS_VEHICLE_CREATE;
    m_unitTypeMask |= UNIT_MASK_VEHICLE;
    return true;
}

void Unit::RemoveVehicleKit(bool dismount/* = false*/)
{
    if (!m_vehicleKit)
        return;

    m_vehicleKit->Uninstall(dismount);
    delete m_vehicleKit;

    m_vehicleKit = NULL;

    m_updateFlag &= ~UPDATEFLAG_HAS_VEHICLE_CREATE;
    m_unitTypeMask &= ~UNIT_MASK_VEHICLE;
    RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
    RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
}

Unit* Unit::GetVehicleBase() const
{
    return m_vehicle ? m_vehicle->GetBase() : NULL;
}

Creature* Unit::GetVehicleCreatureBase() const
{
    if (Unit* veh = GetVehicleBase())
        if (Creature* c = veh->ToCreature())
            return c;

    return NULL;
}

uint64 Unit::GetTransGUID() const
{
    if (GetVehicle())
        return GetVehicleBase()->GetGUID();
    if (GetTransport())
        return GetTransport()->GetGUID();

    return 0;
}

TransportBase* Unit::GetDirectTransport() const
{
    if (Vehicle* veh = GetVehicle())
        return veh;
    return GetTransport();
}

bool Unit::IsInPartyWith(Unit const* unit) const
{
    if (this == unit)
        return true;

    const Unit* u1 = GetCharmerOrOwnerOrSelf();
    const Unit* u2 = unit->GetCharmerOrOwnerOrSelf();
    if (u1 == u2)
        return true;

    if (u1->IsPlayer() && u2->IsPlayer())
        return u1->ToPlayer()->IsInSameGroupWith(u2->ToPlayer());
    else if ((u2->IsPlayer() && u1->GetTypeId() == TYPEID_UNIT && u1->ToCreature()->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT) ||
        (u1->IsPlayer() && u2->GetTypeId() == TYPEID_UNIT && u2->ToCreature()->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT))
        return true;
    else
        return false;
}

bool Unit::IsInRaidWith(Unit const* unit) const
{
    if (this == unit)
        return true;

    const Unit* u1 = GetCharmerOrOwnerOrSelf();
    const Unit* u2 = unit->GetCharmerOrOwnerOrSelf();
    if (u1 == u2)
        return true;

    if (!u1 || !u2)
        return false;

    if (u1->IsPlayer() && u2->IsPlayer())
        return u1->ToPlayer()->IsInSameRaidWith(u2->ToPlayer());
    else if ((u2->IsPlayer() && u1->GetTypeId() == TYPEID_UNIT && u1->ToCreature()->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT) ||
            (u1->IsPlayer() && u2->GetTypeId() == TYPEID_UNIT && u2->ToCreature()->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_TREAT_AS_RAID_UNIT))
        return true;
    else
        return false;
}

void Unit::GetPartyMembers(std::list<Unit*> &TagUnitMap)
{
    Unit* owner = GetCharmerOrOwnerOrSelf();
    Group* group = NULL;
    if (owner->IsPlayer())
        group = owner->ToPlayer()->GetGroup();

    if (group)
    {
        uint8 subgroup = owner->ToPlayer()->GetSubGroup();

        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* Target = itr->getSource();

            // IsHostileTo check duel and controlled by enemy
            if (Target && Target->GetSubGroup() == subgroup && !IsHostileTo(Target))
            {
                if (Target->isAlive() && IsInMap(Target))
                    TagUnitMap.push_back(Target);

                if (Guardian* pet = Target->GetGuardianPet())
                    if (pet->isAlive() && IsInMap(Target))
                        TagUnitMap.push_back(pet);
            }
        }
    }
    else
    {
        if (owner->isAlive() && (owner == this || IsInMap(owner)))
            TagUnitMap.push_back(owner);
        if (Guardian* pet = owner->GetGuardianPet())
            if (pet->isAlive() && (pet == this || IsInMap(pet)))
                TagUnitMap.push_back(pet);
    }
}

void Unit::GetRaidMembers(std::list<Unit*> &p_Members)
{
    Unit* l_Owner = GetCharmerOrOwnerOrSelf();
    Group* l_Group = NULL;
    if (l_Owner->IsPlayer())
        l_Group = l_Owner->ToPlayer()->GetGroup();

    if (l_Group)
    {
        for (GroupReference* l_Iter = l_Group->GetFirstMember(); l_Iter != NULL; l_Iter = l_Iter->next())
        {
            Player* l_Target = l_Iter->getSource();

            // IsHostileTo check duel and controlled by enemy
            if (l_Target && !IsHostileTo(l_Target))
            {
                if (l_Target->isAlive() && IsInMap(l_Target))
                    p_Members.push_back(l_Target);

                if (Guardian* l_Pet = l_Target->GetGuardianPet())
                if (l_Pet->isAlive() && IsInMap(l_Target))
                    p_Members.push_back(l_Pet);
            }
        }
    }
    else
    {
        if (l_Owner->isAlive() && (l_Owner == this || IsInMap(l_Owner)))
            p_Members.push_back(l_Owner);
        if (Guardian* l_Pet = l_Owner->GetGuardianPet())
        if (l_Pet->isAlive() && (l_Pet == this || IsInMap(l_Pet)))
            p_Members.push_back(l_Pet);
    }
}

Aura* Unit::ToggleAura(uint32 spellId, Unit* target)
{
    if (!target)
        return nullptr;

    if (target->HasAura(spellId))
    {
        target->RemoveAurasDueToSpell(spellId);
        return nullptr;
    }

    return target->AddAura(spellId, target);
}

Aura* Unit::AddAura(uint32 spellId, Unit* target)
{
    if (!target)
        return nullptr;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
        return nullptr;

    if (!target->isAlive() && !(spellInfo->Attributes & SPELL_ATTR0_PASSIVE) && !(spellInfo->AttributesEx2 & SPELL_ATTR2_CAN_TARGET_DEAD))
        return nullptr;

    // Glyph of Grounding Totem
    if (target->isTotem() && target->ToTempSummon() && target->ToTempSummon()->GetSummoner() && spellId == 8178)
        if (target->ToTempSummon()->GetSummoner()->HasAura(55441))
            return nullptr;

    return AddAura(spellInfo, MAX_EFFECT_MASK, target);
}

Aura* Unit::AddAura(SpellInfo const* spellInfo, uint32 effMask, Unit* target)
{
    if (!spellInfo)
        return nullptr;

    if (target->IsImmunedToSpell(spellInfo))
        return nullptr;

    for (uint8 i = 0; i < spellInfo->EffectCount; ++i)
    {
        if (!(effMask & (1<<i)))
            continue;
        if (target->IsImmunedToSpellEffect(spellInfo, i))
            effMask &= ~(1<<i);
    }

    Aura* aura = Aura::TryRefreshStackOrCreate(spellInfo, effMask, target, this);
    if (aura != nullptr)
    {
        aura->ApplyForTargets();
        return aura;
    }
    return nullptr;
}

void Unit::SetAuraStack(uint32 spellId, Unit* target, uint32 stack)
{
    Aura* aura = target->GetAura(spellId, GetGUID());
    if (!aura)
        aura = AddAura(spellId, target);
    if (aura && stack)
        aura->SetStackAmount(stack);
}

void Unit::SendPlaySpellVisualKit(uint32 p_KitRecID, uint32 p_KitType, int32 p_Duration)
{
    WorldPacket l_Data(SMSG_PLAY_SPELL_VISUAL_KIT, 16 + 2 + 4 + 4 + 4);
    l_Data.appendPackGUID(GetGUID());
    l_Data << uint32(p_KitRecID);             ///< SpellVisualKit.dbc index
    l_Data << uint32(p_KitType);
    l_Data << uint32(p_Duration);
    SendMessageToSet(&l_Data, true);
}

void Unit::CancelSpellVisualKit(int32 p_SpellVisualKitID)
{
    WorldPacket l_Data(Opcodes::SMSG_CANCEL_SPELL_VISUAL_KIT, 16 + 2 + 4);
    l_Data.appendPackGUID(GetGUID());
    l_Data << int32(p_SpellVisualKitID);
    SendMessageToSetInRange(&l_Data, GetMap()->GetVisibilityRange(), true);
}

void Unit::SendPlaySpellVisual(uint32 p_ID, Unit* p_Target, float p_Speed, float p_Orientation, Position p_Pos, bool p_ThisAsPos /*= false*/, bool p_SpeedAsTime /*= false*/)
{
    ObjectGuid l_Guid = GetGUID(); ///< l_Guid is unused
    ObjectGuid l_Target = p_Target ? p_Target->GetGUID() : 0; ///< l_Target is unused

    if (p_ThisAsPos)
        GetPosition(&p_Pos);
    else if (p_Target)
        GetPosition(&p_Pos);
    else if (p_Pos.m_positionX == 0.0f && p_Pos.m_positionY == 0.0f && p_Pos.m_positionZ == 0.0f)
    {
        p_Pos.m_positionX = 0.0f;
        p_Pos.m_positionY = 0.0f;
        p_Pos.m_positionZ = 0.0f;
    }

    WorldPacket l_Data(SMSG_PLAY_SPELL_VISUAL, 16 + 2 + 16 + 2 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 1);
    l_Data.appendPackGUID(GetGUID());
    l_Data.appendPackGUID(p_Target ? p_Target->GetGUID() : 0);
    l_Data << float(p_Pos.m_positionX);
    l_Data << float(p_Pos.m_positionY);
    l_Data << float(p_Pos.m_positionZ);
    l_Data << uint32(p_ID);
    l_Data << float(p_Speed);
    l_Data << uint16(0);            ///< MissReason
    l_Data << uint16(0);            ///< ReflectStatus
    l_Data << float(p_Orientation);
    l_Data.WriteBit(p_SpeedAsTime);

    if (IsPlayer() && ToPlayer()->GetSession())
        ToPlayer()->GetSession()->SendPacket(&l_Data);
    else
        SendMessageToSet(&l_Data, false);
}

void Unit::CancelSpellVisual(int32 p_SpellVisualID)
{
    WorldPacket l_Data(Opcodes::SMSG_CANCEL_SPELL_VISUAL);
    l_Data.appendPackGUID(GetGUID());
    l_Data << int32(p_SpellVisualID);
    SendMessageToSetInRange(&l_Data, GetMap()->GetVisibilityRange(), false);
}

void Unit::ApplyResilience(Unit const* p_Victim, int32* p_Damage) const
{
    /// Player mounted on multi-passenger mount is also classified as vehicle
    if (IsVehicle() && GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    if (p_Victim->IsVehicle() && p_Victim->GetTypeId() != TypeID::TYPEID_PLAYER)
        return;

    /// Resilience works only for players or pets against other players or pets
    if (GetTypeId() != TypeID::TYPEID_PLAYER && (GetOwner() && GetOwner()->GetTypeId() != TypeID::TYPEID_PLAYER))
        return;

    /// Don't consider resilience if not in PvP - player or pet
    if (!GetCharmerOrOwnerPlayerOrPlayerItself())
        return;

    Unit const* l_Target = nullptr;
    if (p_Victim->GetTypeId() == TypeID::TYPEID_PLAYER)
        l_Target = p_Victim;
    else if (p_Victim->GetTypeId() == TypeID::TYPEID_UNIT && p_Victim->GetOwner() && p_Victim->GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER)
        l_Target = p_Victim->GetOwner();

    if (!l_Target)
        return;

    *p_Damage -= (int32)CalculatePct(float(*p_Damage), -l_Target->GetFloatValue(EPlayerFields::PLAYER_FIELD_MOD_RESILIENCE_PERCENT));
}

// Melee based spells can be miss, parry or dodge on this step
// Crit or block - determined on damage calculation phase! (and can be both in some time)
float Unit::MeleeSpellMissChance(const Unit* p_Victim, SpellInfo const* p_Spell, WeaponAttackType p_AttType) const
{
    // Calculate miss chance
    float l_MissChance = p_Victim->GetUnitMissChancePhysical(this, p_AttType);

    if (p_Spell && !p_Spell->Id && haveOffhandWeapon() && (!ToPlayer() || ToPlayer()->GetSpecializationId() != SPEC_ROGUE_COMBAT))
        l_MissChance += 19.0f;

    // Calculate hit chance
    float l_HitChance = 100.0f;

    // Spellmod from SPELLMOD_RESIST_MISS_CHANCE
    if (p_Spell && p_Spell->Id)
    {
        if (Player* l_ModOwner = GetSpellModOwner())
            l_ModOwner->ApplySpellMod(p_Spell->Id, SPELLMOD_RESIST_MISS_CHANCE, l_HitChance);
    }

    // Increase hit chance from attacker auras and attacker ratings
    if (p_AttType == WeaponAttackType::RangedAttack)
        l_HitChance += m_modRangedHitChance;
    else
        l_HitChance += m_modMeleeHitChance;

    l_MissChance += l_HitChance - 100.0f;

    if (l_MissChance < 0.0f)
        return 0.0f;
    if (l_MissChance > 100.0f)
        return 100.0f;

    return l_MissChance;
}

float Unit::MagicSpellMissChance(const Unit* p_Victim, SpellInfo const* p_Spell) const
{
    // Calculate miss chance
    float l_MissChance = p_Victim->GetUnitMissChanceSpell(this);

    // Calculate hit chance
    float l_HitChance = 100.0f;

    // Spellmod from SPELLMOD_RESIST_MISS_CHANCE
    if (p_Spell && p_Spell->Id)
    {
        if (Player* l_ModOwner = GetSpellModOwner())
            l_ModOwner->ApplySpellMod(p_Spell->Id, SPELLMOD_RESIST_MISS_CHANCE, l_HitChance);
    }

    // Spells with SPELL_ATTR3_IGNORE_HIT_RESULT will ignore target's avoidance effects
    if (p_Spell && !(p_Spell->AttributesEx3 & SPELL_ATTR3_IGNORE_HIT_RESULT))
    {
        // Chance hit from victim SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE auras
        l_HitChance += p_Victim->GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_ATTACKER_SPELL_HIT_CHANCE, p_Spell->GetSchoolMask());
    }

    // Increase hit chance from attacker SPELL_AURA_MOD_SPELL_HIT_CHANCE and attacker ratings
    l_HitChance += m_modSpellHitChance;

    l_MissChance += l_HitChance - 100.0f;

    if (l_MissChance < 0.0f)
        return 0.0f;
    if (l_MissChance > 100.0f)
        return 100.0f;

    return l_MissChance;
}

void Unit::SetPhaseMask(uint32 newPhaseMask, bool update)
{
    if (newPhaseMask == GetPhaseMask())
        return;

    if (IsInWorld())
    {
        RemoveNotOwnSingleTargetAuras(newPhaseMask);            // we can lost access to caster or target

        // modify hostile references for new phasemask, some special cases deal with hostile references themselves
        if (GetTypeId() == TYPEID_UNIT || (!ToPlayer()->isGameMaster() && !ToPlayer()->GetSession()->PlayerLogout()))
        {
            HostileRefManager& refManager = getHostileRefManager();
            HostileReference* ref = refManager.getFirst();

            while (ref)
            {
                if (Unit* unit = ref->getSource()->getOwner())
                    if (Creature* creature = unit->ToCreature())
                        refManager.setOnlineOfflineState(creature, creature->InSamePhase(newPhaseMask));

                ref = ref->next();
            }

            // modify threat lists for new phasemask
            if (GetTypeId() != TYPEID_PLAYER)
            {
                std::list<HostileReference*> threatList = getThreatManager().getThreatList();
                std::list<HostileReference*> offlineThreatList = getThreatManager().getOfflineThreatList();

                // merge expects sorted lists
                threatList.sort();
                offlineThreatList.sort();
                threatList.merge(offlineThreatList);

                for (std::list<HostileReference*>::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                    if (Unit* unit = (*itr)->getTarget())
                        unit->getHostileRefManager().setOnlineOfflineState(ToCreature(), unit->InSamePhase(newPhaseMask));
            }
        }
    }

    WorldObject::SetPhaseMask(newPhaseMask, update);

    if (!IsInWorld())
        return;

    for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr)
        if ((*itr)->GetTypeId() == TYPEID_UNIT)
            (*itr)->SetPhaseMask(newPhaseMask, true);

    for (uint8 i = 0; i < MAX_SUMMON_SLOT; ++i)
        if (m_SummonSlot[i])
            if (Creature* summon = GetMap()->GetCreature(m_SummonSlot[i]))
                summon->SetPhaseMask(newPhaseMask, true);
}

class Unit::AINotifyTask : public BasicEvent
{
    Unit& m_owner;
public:
    explicit AINotifyTask(Unit * me) : BasicEvent(), m_owner(*me)
    {
        m_owner.m_VisibilityUpdScheduled = true;
    }

    ~AINotifyTask()
    {
        m_owner.m_VisibilityUpdScheduled = false;
    }

    virtual bool Execute(uint64 , uint32)
    {
        JadeCore::AIRelocationNotifier notifier(m_owner);
        m_owner.VisitNearbyObject(60.0f, notifier);
        return true;
    }

    static void ScheduleAINotify(Unit* me)
    {
        if (!me->m_VisibilityUpdScheduled)
            me->m_Events.AddEvent(new AINotifyTask(me), me->m_Events.CalculateTime(World::Visibility_AINotifyDelay));
    }
};

float g_RequiredMoveDistanceSq[6] =
{
    20.0f,  ///< MAP_COMMON
    25.0f,  ///< MAP_INSTANCE
    25.0f,  ///< MAP_RAID
    16.0f,  ///< MAP_BATTLEGROUND,
     1.0f,  ///< MAP_ARENA
    25.0f   ///< MAP_SCENARIO
};

class Unit::VisibilityUpdateTask : public BasicEvent
{
    Unit& m_owner;
public:
    explicit VisibilityUpdateTask(Unit * me) : BasicEvent(), m_owner(*me) {}

    virtual bool Execute(uint64 , uint32)
    {
        UpdateVisibility(&m_owner);
        return true;
    }

    static void UpdateVisibility(Unit* me)
    {
        if (!me->m_sharedVision.empty())
        {
            for (SharedVisionList::const_iterator it = me->m_sharedVision.begin(); it != me->m_sharedVision.end();)
            {
                Player * tmp = *it;
                ++it;
                tmp->UpdateVisibilityForPlayer();
            }
        }

        Map* l_Map = me->FindMap();
        if (!l_Map)
            return;

        float l_DistanceX = me->m_LastNotifyPosition.GetPositionX() - me->GetPositionX();
        float l_DistanceY = me->m_LastNotifyPosition.GetPositionY() - me->GetPositionY();
        float l_DistanceZ = me->m_LastNotifyPosition.GetPositionZ() - me->GetPositionZ();
        float l_DistanceSQ = l_DistanceX*l_DistanceX + l_DistanceY*l_DistanceY + l_DistanceZ*l_DistanceZ;

        float l_MinDistanceSQ = g_RequiredMoveDistanceSq[l_Map->GetEntry()->instanceType];
        if (l_DistanceSQ < l_MinDistanceSQ)
            return;

        me->m_LastNotifyPosition.Relocate(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());

        if (me->isType(TYPEMASK_PLAYER))
            ((Player*)me)->UpdateVisibilityForPlayer();

        me->WorldObject::UpdateObjectVisibility(true);
    }
};

void Unit::UpdateObjectVisibility(bool forced)
{
    if (forced)
    {
        if (isType(TYPEMASK_PLAYER))
            ((Player*)this)->UpdateVisibilityForPlayer();

        WorldObject::UpdateObjectVisibility(true);
    }
    else
        m_Events.AddEvent(new VisibilityUpdateTask(this), m_Events.CalculateTime(1));
    AINotifyTask::ScheduleAINotify(this);
}

void Unit::SendMoveKnockBack(Player* p_Player, float p_SpeedXY, float p_SpeedZ, float p_Cos, float p_Sin)
{
    //if (this->ToPlayer())
    //    sAnticheatMgr->DisableAnticheatDetection(this->ToPlayer());

    WorldPacket l_Packet(SMSG_MOVE_KNOCK_BACK, (1 + 8 + 4 + 4 + 4 + 4 + 4));

    l_Packet.appendPackGUID(GetGUID());
    l_Packet << uint32(0);
    l_Packet << float(p_Cos);
    l_Packet << float(p_Sin);
    l_Packet << float(p_SpeedXY);
    l_Packet << float(p_SpeedZ);

    p_Player->GetSession()->SendPacket(&l_Packet);

    WorldPacket data(SMSG_MOVE_UPDATE_KNOCK_BACK, 200);
    uint32 l_MSTime = getMSTime();

    m_movementInfo.time             = l_MSTime;
    m_movementInfo.Alive32          = 0;
    m_movementInfo.HasFallData      = true;
    m_movementInfo.fallTime         = 0;
    m_movementInfo.hasFallDirection = true;
    m_movementInfo.j_cosAngle       = p_Cos;
    m_movementInfo.j_sinAngle       = p_Sin;
    m_movementInfo.j_xyspeed        = p_SpeedXY;
    m_movementInfo.JumpVelocity     = p_SpeedZ;

    p_Player->GetSession()->WriteMovementInfo(data, &m_movementInfo);

    SendMessageToSet(&data, false);
}

void Unit::KnockbackFrom(float x, float y, float speedXY, float speedZ)
{
    Player* player = NULL;
    if (IsPlayer())
        player = ToPlayer();
    else if (Unit* charmer = GetCharmer())
    {
        player = charmer->ToPlayer();
        if (player && player->m_mover != this)
            player = NULL;
    }

    if (!player)
        GetMotionMaster()->MoveKnockbackFrom(x, y, speedXY, speedZ);
    else
    {
        float vcos, vsin;
        GetSinCos(x, y, vsin, vcos);
        SendMoveKnockBack(player, speedXY, -speedZ, vcos, vsin);
    }
}

float Unit::GetCombatRatingReduction(CombatRating cr) const
{
    if (Player const* player = ToPlayer())
        return player->GetRatingBonusValue(cr);
    // Player's pet get resilience from owner
    else if (isPet() && GetOwner())
        if (Player* owner = GetOwner()->ToPlayer())
            return owner->GetRatingBonusValue(cr);

    return 0.0f;
}

uint32 Unit::GetModelForForm(ShapeshiftForm form)
{
    switch (form)
    {
        case FORM_CAT:
        {
            // Hack for Druid of the Flame, Fandral's Flamescythe
            if (HasAura(99245))
                return 38150;

            bool kingOfTheJungle = HasAura(102543);
            bool clawsOfShirvallah = HasAura(171745); // Claws of Shirvallah
            uint32 l_ClawOfShirvallahModel = 0;
            uint32 l_KingOfTheJungleModel = 0;
            uint32 l_BaseCatModel = 0;

            // Based on Hair color
            if (getRace() == RACE_NIGHTELF)
            {
                if (clawsOfShirvallah)
                    l_ClawOfShirvallahModel =  59268; // Panther

                uint8 hairColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
                if (HasAura(107059)) ///< Glyph of the Chameleon
                    hairColor = urand(0, 10);

                switch (hairColor)
                {
                    case 7: // Violet
                    case 8:
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43764;
                        else
                            l_BaseCatModel = 29405;
                        break;
                    }
                    case 3: // Light Blue
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43763;
                        else
                            l_BaseCatModel = 29406;
                        break;
                    }
                    case 0: // Green
                    case 1: // Light Green
                    case 2: // Dark Green
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43762;
                        else
                            l_BaseCatModel = 29407;
                        break;
                    }
                    case 4: // White
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43765;
                        else
                            l_BaseCatModel = 29408;
                        break;
                    }
                    default: // original - Dark Blue
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43761;
                        else
                            l_BaseCatModel = 892;
                        break;
                    }
                }
            }
            else if (getRace() == RACE_TROLL)
            {
                if (clawsOfShirvallah)
                    l_ClawOfShirvallahModel = 59270; // Tiger

                uint8 hairColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
                if (HasAura(107059))
                    hairColor = urand(0, 12); ///< Glyph of the Chameleon
                switch (hairColor)
                {
                    case 0: // Red
                    case 1:
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43776;
                        else
                            l_BaseCatModel = 33668;
                        break;
                    }
                    case 2: // Yellow
                    case 3:
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43778;
                        else
                            l_BaseCatModel = 33667;
                        break;
                    }
                    case 4: // Blue
                    case 5:
                    case 6:
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43773;
                        else
                            l_BaseCatModel = 33666;
                        break;
                    }
                    case 7: // Purple
                    case 10:
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43775;
                        else
                            l_BaseCatModel = 33665;
                        break;
                    }
                    default: // original - white
                    {
                        if (kingOfTheJungle)
                            l_KingOfTheJungleModel = 43777;
                        else
                            l_BaseCatModel = 33669;
                        break;
                    }
                }
            }
            else if (getRace() == RACE_WORGEN)
            {
                if (clawsOfShirvallah)
                    l_ClawOfShirvallahModel = 59269; // Snowleopard

                // Based on Skin color
                uint8 skinColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_SKIN_ID);
                if (HasAura(107059)) ///< Glyph of the Chameleon
                    skinColor = urand(0, 9);
                // Male
                if (getGender() == GENDER_MALE)
                {
                    switch (skinColor)
                    {
                        case 1: // Brown
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43781;
                            else
                                l_BaseCatModel = 33662;
                            break;
                        }
                        case 2: // Black
                        case 7:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43780;
                            else
                                l_BaseCatModel = 33661;
                            break;
                        }
                        case 4: // yellow
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43784;
                            else
                                l_KingOfTheJungleModel = 33664;
                            break;
                        }
                        case 3: // White
                        case 5:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43785;
                            else
                                l_BaseCatModel = 33663;
                        }
                        default: // original - Gray
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43782;
                            else
                                l_BaseCatModel = 33660;
                            break;
                        }
                    }
                }
                // Female
                else
                {
                    switch (skinColor)
                    {
                        case 5: // Brown
                        case 6:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43781;
                            else
                                l_BaseCatModel = 33662;
                            break;
                        }
                        case 7: // Black
                        case 8:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43780;
                            else
                                l_BaseCatModel = 33661;
                            break;
                        }
                        case 3: // yellow
                        case 4:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43784;
                            else
                                l_BaseCatModel = 33664;
                            break;
                        }
                        case 2: // White
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43785;
                            else
                                l_BaseCatModel = 33663;
                            break;
                        }
                        default: // original - Gray
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43782;
                            else
                                l_BaseCatModel = 33660;
                            break;
                        }
                    }
                }
            }
            // Based on Skin color
            else if (getRace() == RACE_TAUREN)
            {
                if (clawsOfShirvallah)
                    l_ClawOfShirvallahModel = 59267; // Lion

                uint8 skinColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_SKIN_ID);
                if (HasAura(107059)) ///< Glyph of the Chameleon
                    skinColor = urand(0, 20);
                // Male
                if (getGender() == GENDER_MALE)
                {
                    switch (skinColor)
                    {
                        case 12: // White
                        case 13:
                        case 14:
                        case 18: // Completly White
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43769;
                            else
                                l_BaseCatModel = 29409;
                            break;
                        }
                        case 9: // Light Brown
                        case 10:
                        case 11:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43770;
                            else
                                l_BaseCatModel = 29410;
                            break;
                        }
                        case 6: // Brown
                        case 7:
                        case 8:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43768;
                            else
                                l_BaseCatModel = 29411;
                            break;
                        }
                        case 0: // Dark
                        case 1:
                        case 2:
                        case 3: // Dark Grey
                        case 4:
                        case 5:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43766;
                            else
                                l_BaseCatModel = 29412;
                            break;
                        }
                        default: // original - Grey
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43767;
                            else
                                l_BaseCatModel = 8571;
                            break;
                        }
                    }
                }
                // Female
                else
                {
                    switch (skinColor)
                    {
                        case 10: // White
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43769;
                            else
                                l_BaseCatModel = 29409;
                            break;
                        }
                        case 6: // Light Brown
                        case 7:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43770;
                            else
                                l_BaseCatModel = 29410;
                            break;
                        }
                        case 4: // Brown
                        case 5:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43768;
                            else
                                l_BaseCatModel = 29411;
                            break;
                        }
                        case 0: // Dark
                        case 1:
                        case 2:
                        case 3:
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43766;
                            else
                                l_BaseCatModel = 29412;
                            break;
                        }
                        default: // original - Grey
                        {
                            if (kingOfTheJungle)
                                l_KingOfTheJungleModel = 43767;
                            else
                                l_BaseCatModel = 8571;
                            break;
                        }
                    }
                }
            }
            if (l_KingOfTheJungleModel)
                return l_KingOfTheJungleModel;
            else if (l_ClawOfShirvallahModel)
                return l_ClawOfShirvallahModel;
            else if (l_BaseCatModel)
                return l_BaseCatModel;
            else if (Player::TeamForRace(getRace()) == ALLIANCE)
                return 892;
            else
                return 8571;
        }
        case FORM_BEAR:
        {
            bool ursocsSon = HasAura(102558);
            // Based on Hair color
            if (getRace() == RACE_NIGHTELF)
            {
                uint8 hairColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
                if (HasAura(107059)) ///< Glyph of the Chameleon
                    hairColor = urand(0, 8);
                switch (hairColor)
                {
                    case 0: // Green
                    case 1: // Light Green
                    case 2: // Dark Green
                    {
                        if (ursocsSon)
                            return 43759;
                        else
                            return 29413;
                    }
                    case 6: // Black
                    {
                        if (ursocsSon)
                            return 43756;
                        else
                            return 29414;
                    }
                    case 4: // White
                    {
                        if (ursocsSon)
                            return 43760;
                        else
                            return 29416;
                    }
                    case 3: // Light Blue
                    {
                        if (ursocsSon)
                            return 43757;
                        else
                            return 29415;
                    }
                    default: // original - Violet
                    {
                        if (ursocsSon)
                            return 43758;
                        else
                            return 2281;
                    }
                }
            }
            else if (getRace() == RACE_TROLL)
            {
                uint8 hairColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
                if (HasAura(107059)) ///< Glyph of the Chameleon
                    hairColor = urand(0, 14);
                switch (hairColor)
                {
                    case 0: // Red
                    case 1:
                    {
                        if (ursocsSon)
                            return 43748;
                        else
                            return 33657;
                    }
                    case 2: // Yellow
                    case 3:
                    {
                        if (ursocsSon)
                            return 43750;
                        else
                            return 33659;
                    }
                    case 7: // Purple
                    case 10:
                    {
                        if (ursocsSon)
                            return 43747;
                        else
                            return 33656;
                    }
                    case 8: // White
                    case 9:
                    case 11:
                    case 12:
                    {
                        if (ursocsSon)
                            return 43749;
                        else
                            return 33658;
                    }
                    default: // original - Blue
                    {
                        if (ursocsSon)
                            return 43746;
                        else
                            return 33655;
                    }
                }
            }
            else if (getRace() == RACE_WORGEN)
            {
                // Based on Skin color
                uint8 skinColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_SKIN_ID);
                if (HasAura(107059)) ///< Glyph of the Chameleon
                    skinColor = urand(0, 8);
                // Male
                if (getGender() == GENDER_MALE)
                {
                    switch (skinColor)
                    {
                        case 1: // Brown
                        {
                            if (ursocsSon)
                                return 43752;
                            else
                                return 33652;
                        }
                        case 2: // Black
                        case 7:
                        {
                            if (ursocsSon)
                                return 43751;
                            else
                                return 33651;
                        }
                        case 4: // Yellow
                        {
                            if (ursocsSon)
                                return 43755;
                            else
                                return 33654;
                        }
                        case 3: // White
                        case 5:
                        {
                            if (ursocsSon)
                                return 43754;
                            else
                                return 33653;
                        }
                        default: // original - Gray
                        {
                            if (ursocsSon)
                                return 43753;
                            else
                                return 33650;
                        }
                    }
                }
                // Female
                else
                {
                    switch (skinColor)
                    {
                        case 5: // Brown
                        case 6:
                        {
                            if (ursocsSon)
                                return 43752;
                            else
                                return 33652;
                        }
                        case 7: // Black
                        case 8:
                        {
                            if (ursocsSon)
                                return 43751;
                            else
                                return 33651;
                        }
                        case 3: // yellow
                        case 4:
                        {
                            if (ursocsSon)
                                return 43755;
                            else
                                return 33654;
                        }
                        case 2: // White
                        {
                            if (ursocsSon)
                                return 43754;
                            else
                                return 33653;
                        }
                        default: // original - Gray
                        {
                            if (ursocsSon)
                                return 43753;
                            else
                                return 33650;
                        }
                    }
                }
            }
            // Based on Skin color
            else if (getRace() == RACE_TAUREN)
            {
                uint8 skinColor = GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_SKIN_ID);
                if (HasAura(107059)) ///< Glyph of the Chameleon
                    skinColor = urand(0, 20);
                // Male
                if (getGender() == GENDER_MALE)
                {
                    switch (skinColor)
                    {
                        case 0: // Dark (Black)
                        case 1:
                        case 2:
                        {
                            if (ursocsSon)
                                return 43741;
                            else
                                return 29418;
                        }
                        case 3: // White
                        case 4:
                        case 5:
                        case 12:
                        case 13:
                        case 14:
                        {
                            if (ursocsSon)
                                return 43743;
                            else
                                return 29419;
                        }
                        case 9: // Light Brown/Grey
                        case 10:
                        case 11:
                        case 15:
                        case 16:
                        case 17:
                        {
                            if (ursocsSon)
                                return 43745;
                            else
                                return 29420;
                        }
                        case 18: // Completly White
                        {
                            if (ursocsSon)
                                return 43744;
                            else
                                return 29421;
                        }
                        default: // original - Brown
                        {
                            if (ursocsSon)
                                return 43742;
                            else
                                return 2289;
                        }
                    }
                }
                // Female
                else
                {
                    switch (skinColor)
                    {
                        case 0: // Dark (Black)
                        case 1:
                        {
                            if (ursocsSon)
                                return 43741;
                            else
                                return 29418;
                        }
                        case 2: // White
                        case 3:
                        {
                            if (ursocsSon)
                                return 43743;
                            else
                                return 29419;
                        }
                        case 6: // Light Brown/Grey
                        case 7:
                        case 8:
                        case 9:
                        {
                            if (ursocsSon)
                                return 43745;
                            else
                                return 29420;
                        }
                        case 10: // Completly White
                        {
                            if (ursocsSon)
                                return 43744;
                            else
                                return 29421;
                        }
                        default: // original - Brown
                        {
                            if (ursocsSon)
                                return 43742;
                            else
                                return 2289;
                        }
                    }
                }
            }
            else if (Player::TeamForRace(getRace()) == ALLIANCE)
                return 2281;
            else
                return 2289;
        }
        case FORM_FLIGHT:
        {
            if (Player::TeamForRace(getRace()) == ALLIANCE)
                return 20857;
            else
            {
                if (getRace() == RACE_TROLL)
                    return 37728;
                else
                    return 20872;
            }
        }
        case FORM_FLIGHT_EPIC:
        {
            if (Player::TeamForRace(getRace()) == ALLIANCE)
                return (getRace() == RACE_WORGEN ? 37729 : 21243);
            if (getRace() == RACE_TROLL)
                return 37730;
            return 21244;
        }
        case FORM_STAG:
        {
            if (HasAura(131113)) // Druid Form Gepard - Glyph
                return 1043;

            switch (getRace())
            {
                case RACE_NIGHTELF:
                case RACE_WORGEN:
                    return 40816;
                case RACE_TROLL:
                case RACE_TAUREN:
                    return 45339;
                default:
                    break;
            }
            break;
        }
        case FORM_MOONKIN:
        {
            /// Glyph of Stars
            if (HasAura(114301))
            {
                return GetNativeDisplayId();
            }

            bool l_ChosenOfElune = HasAura(102560);

            switch (getRace())
            {
                case RACE_NIGHTELF:
                {
                    if (l_ChosenOfElune)
                        return 43790;
                    return 15374;
                }
                case RACE_TAUREN:
                {
                    if (l_ChosenOfElune)
                        return 43786;
                    return 15375;
                }
                case RACE_WORGEN:
                {
                    if (l_ChosenOfElune)
                        return 43787;
                    return 37173;
                }
                case RACE_TROLL:
                {
                    if (l_ChosenOfElune)
                        return 43789;
                    return 37174;
                }
                default:
                    break;
            }
            break;
        }
        case FORM_AQUA:
        {
            // Glyph of the Orca
            if (HasAura(114333))
                return 4591;
            break;
        }
        case FORM_GHOSTWOLF:
        {
            // Glyph of the Spectral Wolf
            if (HasAura(58135))
                return 60247;
            break;
        }
        case FORM_METAMORPHOSIS:
        {
            // Glyph of Metamorphosis
            if (HasAura(159680))
                return 0;
            if (HasAura(137206))
                return 48088;
            return 25277;
        }
        default:
        {
            if (!HasAura(102560))
                break;

            if (HasAura(114301))
                return GetNativeDisplayId();

            switch (getRace())
            {
            case RACE_NIGHTELF:
            {
                return 43790;
            }
            case RACE_TAUREN:
            {
                 return 43786;
            }
            case RACE_WORGEN:
            {
                return 43787;
            }
            case RACE_TROLL:
            {
                return 43789;
            }
            default:
                break;
            }
            break;
        }
    }

    uint32 modelid = 0;
    SpellShapeshiftFormEntry const* formEntry = sSpellShapeshiftFormStore.LookupEntry(form);
    if (formEntry && formEntry->m_CreatureDisplayID[0])
    {
        // Take the alliance modelid as default
        if (GetTypeId() != TYPEID_PLAYER)
            return formEntry->m_CreatureDisplayID[0];
        else
        {
            if (Player::TeamForRace(getRace()) == ALLIANCE)
                modelid = formEntry->m_CreatureDisplayID[0];
            else
                modelid = formEntry->m_CreatureDisplayID[1];

            // If the player is horde but there are no values for the horde modelid - take the alliance modelid
            if (!modelid && Player::TeamForRace(getRace()) == HORDE)
                modelid = formEntry->m_CreatureDisplayID[0];
        }
    }

    if (!modelid && form != FORM_STEALTH && form != FORM_SHADOW)
        modelid = GetNativeDisplayId();
    return modelid;
}

uint32 Unit::GetModelForTotem(PlayerTotemType p_TotemType)
{
    PlayerTotemType l_OriginalTotemType = p_TotemType;

    switch (p_TotemType)
    {
        case SUMMON_TYPE_TOTEM_FIRE2:
        case SUMMON_TYPE_TOTEM_FIRE3:
        case SUMMON_TYPE_TOTEM_FIRE4:
            p_TotemType = SUMMON_TYPE_TOTEM_FIRE;
            break;
        case SUMMON_TYPE_TOTEM_EARTH2:
        case SUMMON_TYPE_TOTEM_EARTH3:
            p_TotemType = SUMMON_TYPE_TOTEM_EARTH;
            break;
        case SUMMON_TYPE_TOTEM_WATER2:
            p_TotemType = SUMMON_TYPE_TOTEM_WATER;
            break;
        case SUMMON_TYPE_TOTEM_AIR2:
        case SUMMON_TYPE_TOTEM_AIR3:
        case SUMMON_TYPE_TOTEM_AIR4:
        case SUMMON_TYPE_TOTEM_AIR5:
        case SUMMON_TYPE_TOTEM_AIR6:
            p_TotemType = SUMMON_TYPE_TOTEM_AIR;
            break;
    }

    if (l_OriginalTotemType == SUMMON_TYPE_TOTEM_FIRE4 && HasAura(147772)) // Searing Totem - Glyph of the Flaming Serpent
        return 46820; // Vol'Jin Serpent

    switch (getRace())
    {
        case RACE_ORC:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 30758;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 30757;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 30759;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 30756;
            }
            break;
        }
        case RACE_DWARF:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 30754;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 30753;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 30755;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 30736;
            }
            break;
        }
        case RACE_TROLL:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 30762;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 30761;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 30763;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 30760;
            }
            break;
        }
        case RACE_TAUREN:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 4589;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 4588;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 4587;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 4590;
            }
            break;
        }
        case RACE_DRAENEI:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 19074;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 19073;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 19075;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 19071;
            }
            break;
        }
        case RACE_GOBLIN:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 30783;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 30782;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 30784;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 30781;
            }
            break;
        }
        case RACE_PANDAREN_NEUTRAL:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 41670;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 41669;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 41671;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 41668;
            }
            break;
        }
        case RACE_PANDAREN_ALLI:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 41670;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 41669;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 41671;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 41668;
            }
            break;
        }
        case RACE_PANDAREN_HORDE:
        {
            switch (p_TotemType)
            {
                case SUMMON_TYPE_TOTEM_FIRE:    // fire
                    return 41670;
                case SUMMON_TYPE_TOTEM_EARTH:   // earth
                    return 41669;
                case SUMMON_TYPE_TOTEM_WATER:   // water
                    return 41671;
                case SUMMON_TYPE_TOTEM_AIR:     // air
                    return 41668;
            }
            break;
        }
    }
    return 0;
}

void Unit::JumpTo(float speedXY, float speedZ, bool forward)
{
    float angle = forward ? 0 : M_PI;
    if (GetTypeId() == TYPEID_UNIT)
        GetMotionMaster()->MoveJumpTo(angle, speedXY, speedZ);
    else
    {
        float vcos = std::cos(angle+GetOrientation());
        float vsin = std::sin(angle+GetOrientation());
        SendMoveKnockBack(ToPlayer(), speedXY, -speedZ, vcos, vsin);
    }
}

void Unit::JumpTo(WorldObject* obj, float speedZ)
{
    float x, y, z;
    obj->GetContactPoint(this, x, y, z);
    float speedXY = GetExactDist2d(x, y) * 10.0f / speedZ;
    GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
}

bool Unit::HandleSpellClick(Unit* clicker, int8 seatId)
{
    uint32 spellClickEntry = GetVehicleKit() ? GetVehicleKit()->GetCreatureEntry() : GetEntry();
    SpellClickInfoMapBounds clickPair = sObjectMgr->GetSpellClickInfoMapBounds(spellClickEntry);
    for (SpellClickInfoContainer::const_iterator itr = clickPair.first; itr != clickPair.second; ++itr)
    {
        //! First check simple relations from clicker to clickee
        if (!itr->second.IsFitToRequirements(clicker, this))
            return false;

        //! Check database conditions
        if (!sConditionMgr->IsObjectMeetingSpellClickConditions(spellClickEntry, itr->second.spellId, clicker, this))
            return false;

        Unit* caster = (itr->second.castFlags & NPC_CLICK_CAST_CASTER_CLICKER) ? clicker : this;
        Unit* target = (itr->second.castFlags & NPC_CLICK_CAST_TARGET_CLICKER) ? clicker : this;
        uint64 origCasterGUID = (itr->second.castFlags & NPC_CLICK_CAST_ORIG_CASTER_OWNER) ? GetOwnerGUID() : clicker->GetGUID();

        SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(itr->second.spellId);
        // if (!spellEntry) should be checked at npc_spellclick load
        if (!spellEntry)
        {
            sLog->outAshran("HandleSpellClick: spellEntry pointer is NULL!!");
            return false;
        }

        if (seatId > -1)
        {
            uint8 i = 0;
            bool valid = false;
            while (i < spellEntry->EffectCount && !valid)
            {
                if (spellEntry->Effects[i].ApplyAuraName == SPELL_AURA_CONTROL_VEHICLE)
                {
                    valid = true;
                    break;
                }
                ++i;
            }

            if (!valid)
                return false;

            if (IsInMap(caster))
                caster->CastCustomSpell(itr->second.spellId, SpellValueMod(SPELLVALUE_BASE_POINT0+i), seatId+1, target, false, NULL, nullptr, origCasterGUID);
            else    // This can happen during Player::_LoadAuras
            {
                int32 bp0[SpellEffIndex::MAX_EFFECTS];
                for (uint8 eff = 0; eff < SpellEffIndex::MAX_EFFECTS; eff++)
                    bp0[eff] = spellEntry->Effects[i].BasePoints;
                bp0[i] = seatId + 1;
                Aura::TryRefreshStackOrCreate(spellEntry, MAX_EFFECT_MASK, this, clicker, &bp0[0], NULL, origCasterGUID);
            }
        }
        else
        {
            if (IsInMap(caster))
                caster->CastSpell(target, spellEntry, false, NULL, nullptr, origCasterGUID);
            else
                Aura::TryRefreshStackOrCreate(spellEntry, MAX_EFFECT_MASK, this, clicker, NULL, NULL, origCasterGUID);
        }
    }

    Creature* creature = ToCreature();
    if (creature && creature->IsAIEnabled)
        creature->AI()->OnSpellClick(clicker);

    return true;
}

void Unit::EnterVehicle(Unit* base, int8 seatId, bool fullTriggered)
{
    CastCustomSpell(VEHICLE_SPELL_RIDE_HARDCODED, SPELLVALUE_BASE_POINT0, seatId + 1, base, fullTriggered ? TRIGGERED_FULL_MASK : TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE);
}

void Unit::_EnterVehicle(Vehicle* vehicle, int8 seatId, AuraApplication const* aurApp)
{
    // Must be called only from aura handler
    if (!isAlive() || GetVehicleKit() == vehicle || vehicle->GetBase()->IsOnVehicle(this))
        return;

    if (m_vehicle)
    {
        if (m_vehicle == vehicle)
        {
            if (seatId >= 0 && seatId != GetTransSeat())
                ChangeSeat(seatId);
            return;
        }
        else
            ExitVehicle();
    }

    if (aurApp && aurApp->GetRemoveMode())
        return;

    if (Player* player = ToPlayer())
    {
        if (vehicle->GetBase()->IsPlayer() && player->isInCombat())
            return;

        InterruptNonMeleeSpells(false);
        player->StopCastingCharm();
        player->StopCastingBindSight();
        Dismount();
        RemoveAurasByType(SPELL_AURA_MOUNTED);

        // drop flag at invisible in bg
        if (Battleground* bg = player->GetBattleground())
            bg->EventPlayerDroppedFlag(player);

        WorldPacket l_Data(SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA, 0);
        player->GetSession()->SendPacket(&l_Data);

        switch (vehicle->GetVehicleInfo()->m_ID)
        {
            case 533:   ///< Bone Spike
            case 647:   ///< Bone Spike
            case 648:   ///< Bone Spike
            case 3417:  ///< Grasping Earth
                break;
            default:
                player->UnsummonPetTemporaryIfAny();
                break;
        }
    }

    ASSERT(!m_vehicle);
    m_vehicle = vehicle;
    if (!m_vehicle->AddPassenger(this, seatId))
    {
        m_vehicle = NULL;
        return;
    }
}

void Unit::ChangeSeat(int8 seatId, bool next)
{
    if (!m_vehicle)
        return;

    if (seatId < 0)
    {
        seatId = m_vehicle->GetNextEmptySeat(GetTransSeat(), next);
        if (seatId < 0)
            return;
    }
    else if (seatId == GetTransSeat() || !m_vehicle->HasEmptySeat(seatId))
        return;

    m_vehicle->RemovePassenger(this);
    if (!m_vehicle->AddPassenger(this, seatId))
        ASSERT(false);
}

void Unit::ExitVehicle(Position const* exitPosition)
{
    //! This function can be called at upper level code to initialize an exit from the passenger's side.
    if (!m_vehicle)
        return;

    Unit* l_VerhiculeBase = GetVehicleBase();
    if (l_VerhiculeBase != nullptr)
        l_VerhiculeBase->RemoveAurasByType(SPELL_AURA_CONTROL_VEHICLE, GetGUID());

    if (m_vehicle)
        if (m_vehicle->ArePassengersSpawnedByAI())
            _ExitVehicle(exitPosition);
}

void Unit::_ExitVehicle(Position const* exitPosition)
{
    if (!m_vehicle)
        return;

    m_vehicle->RemovePassenger(this);
    Player* player = ToPlayer();

    // If player is on mounted duel and exits the mount should immediately lose the duel
    if (player && player->m_Duel && player->m_Duel->isMounted)
        player->DuelComplete(DUEL_FLED);

    // This should be done before dismiss, because there may be some aura removal
    Vehicle* vehicle = m_vehicle;
    m_vehicle = NULL;

    Position l_ExitPos;

    /// Exit position not specified
    /// This should use passenger's current position, leaving it as it is now
    /// Because we calculate positions incorrect (sometimes under map)
    if (!exitPosition)
        vehicle->GetBase()->GetPosition(&l_ExitPos);
    else
        l_ExitPos = *exitPosition;

    if (Creature* l_Me = ToCreature())
    {
        if (l_Me->IsAIEnabled)
            l_Me->AI()->OnExitVehicle(vehicle->GetBase(), l_ExitPos);
    }

    AddUnitState(UNIT_STATE_MOVE);

    if (IsPlayer())
        ToPlayer()->SetFallInformation(0, GetPositionZ());
    else if (HasUnitMovementFlag(MOVEMENTFLAG_ROOT))
    {
        WorldPacket data(SMSG_SPLINE_MOVE_UNROOT, 16 + 2);
        ObjectGuid guid = GetGUID();
        data.appendPackGUID(guid);
        SendMessageToSet(&data, false);
    }

    Movement::MoveSplineInit init(this);
    init.MoveTo(l_ExitPos.GetPositionX(), l_ExitPos.GetPositionY(), l_ExitPos.GetPositionZ(), false);
    init.SetFacing(GetOrientation());
    init.SetTransportExit();
    init.Launch();

    SetControlled(false, UNIT_STATE_ROOT);      // SMSG_MOVE_FORCE_UNROOT, ~MOVEMENTFLAG_ROOT

    //GetMotionMaster()->MoveFall();            // Enable this once passenger positions are calculater properly (see above)

    if (player)
    {
        player->ResummonPetTemporaryUnSummonedIfAny();
        player->SummonLastSummonedBattlePet();
    }

    if (vehicle->GetBase()->HasUnitTypeMask(UNIT_MASK_MINION))
        if (((Minion*)vehicle->GetBase())->GetOwner() == this)
            vehicle->Dismiss();

    if (HasUnitTypeMask(UNIT_MASK_ACCESSORY))
    {
        // Vehicle just died, we die too
        if (vehicle->GetBase()->getDeathState() == JUST_DIED)
            setDeathState(JUST_DIED);
        // If for other reason we as minion are exiting the vehicle (ejected, master dismounted) - unsummon
        else
            ToTempSummon()->UnSummon(2000); // Approximation
    }
}

bool Unit::IsFalling() const
{
    return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR) || movespline->isFalling();
}

bool Unit::SetWalk(bool p_Enable)
{
    if (p_Enable == IsWalking())
        return false;

    if (p_Enable)
        AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
    else
        RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

    static Opcodes const l_WalkModeTable[2] = { SMSG_SPLINE_MOVE_SET_RUN_MODE, SMSG_SPLINE_MOVE_SET_WALK_MODE };

    WorldPacket l_Data(l_WalkModeTable[p_Enable]);
    l_Data.appendPackGUID(GetGUID());

    SendMessageToSet(&l_Data, true);
    return true;
}

bool Unit::SetDisableGravity(bool p_Disable, bool p_PacketOnly /*= false*/)
{
    if (!p_PacketOnly)
    {
        if (p_Disable == IsLevitating())
            return false;

        if (p_Disable)
        {
            AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
            RemoveUnitMovementFlag(MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_SPLINE_ELEVATION);
            SetFall(false);
        }
        else
        {
            RemoveUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
            if (!HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY))
                SetFall(true);
        }
    }

    static Opcodes const l_GravityOpcodeTable[2][2] =
    {
        { SMSG_SPLINE_MOVE_GRAVITY_ENABLE,  SMSG_MOVE_ENABLE_GRAVITY  },
        { SMSG_SPLINE_MOVE_GRAVITY_DISABLE, SMSG_MOVE_DISABLE_GRAVITY }
    };

    bool l_IsPlayer = GetTypeId() == TYPEID_PLAYER && ToPlayer()->m_mover->GetTypeId() == TYPEID_PLAYER;

    if (l_IsPlayer)
    {
        WorldPacket l_Data(l_GravityOpcodeTable[p_Disable][1]);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);                ///< Movement counter

        SendMessageToSet(&l_Data, true);
    }
    else
    {
        WorldPacket l_Data(l_GravityOpcodeTable[p_Disable][0]);
        l_Data.appendPackGUID(GetGUID());

        SendMessageToSet(&l_Data, true);
    }

    return true;
}

bool Unit::SetFall(bool p_Enable)
{
    if (p_Enable == HasUnitMovementFlag(MOVEMENTFLAG_FALLING))
        return false;

    if (p_Enable)
    {
        AddUnitMovementFlag(MOVEMENTFLAG_FALLING);
        m_movementInfo.SetFallTime(0);
    }
    else
        RemoveUnitMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR);

    return true;
}

bool Unit::SetSwim(bool p_Enable)
{
    if (p_Enable == HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
        return false;

    if (p_Enable)
        AddUnitMovementFlag(MOVEMENTFLAG_SWIMMING);
    else
        RemoveUnitMovementFlag(MOVEMENTFLAG_SWIMMING);

    static Opcodes const l_SwimOpcodeTable[2] = { SMSG_SPLINE_MOVE_STOP_SWIM, SMSG_SPLINE_MOVE_START_SWIM };

    WorldPacket l_Data(l_SwimOpcodeTable[p_Enable]);
    l_Data.appendPackGUID(GetGUID());

    SendMessageToSet(&l_Data, true);

    return true;
}

bool Unit::SetCanFly(bool p_Enable)
{
    if (p_Enable == HasUnitMovementFlag(MOVEMENTFLAG_CAN_FLY))
        return false;

    if (p_Enable)
    {
        AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY);
        RemoveUnitMovementFlag(MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_SPLINE_ELEVATION);
        SetFall(false);
    }
    else
    {
        RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_MASK_MOVING_FLY);
        if (!IsLevitating())
            SetFall(true);
    }

    static Opcodes const l_FlyOpcodeTable[2][2] =
    {
        { SMSG_SPLINE_MOVE_UNSET_FLYING, SMSG_MOVE_UNSET_CAN_FLY },
        { SMSG_SPLINE_MOVE_SET_FLYING,   SMSG_MOVE_SET_CAN_FLY   }
    };

    bool l_IsPlayer = GetTypeId() == TYPEID_PLAYER && ToPlayer()->m_mover->GetTypeId() == TYPEID_PLAYER;

    if (l_IsPlayer)
    {
        WorldPacket l_Data(l_FlyOpcodeTable[p_Enable][1]);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);                ///< Movement counter

        SendMessageToSet(&l_Data, true);
    }
    else
    {
        WorldPacket l_Data(l_FlyOpcodeTable[p_Enable][0]);
        l_Data.appendPackGUID(GetGUID());

        SendMessageToSet(&l_Data, true);
    }

    return true;
}

bool Unit::SetWaterWalking(bool p_Enable, bool p_PacketOnly /*= false*/)
{
    if (!p_PacketOnly)
    {
        if (p_Enable == HasUnitMovementFlag(MOVEMENTFLAG_WATERWALKING))
            return false;

        if (p_Enable)
            AddUnitMovementFlag(MOVEMENTFLAG_WATERWALKING);
        else
            RemoveUnitMovementFlag(MOVEMENTFLAG_WATERWALKING);
    }

    static Opcodes const l_WaterWalkingOpcodeTable[2][2] =
    {
        { SMSG_SPLINE_MOVE_SET_LAND_WALK,  SMSG_MOVE_LAND_WALK  },
        { SMSG_SPLINE_MOVE_SET_WATER_WALK, SMSG_MOVE_WATER_WALK }
    };

    bool l_IsPlayer = GetTypeId() == TYPEID_PLAYER && ToPlayer()->m_mover->GetTypeId() == TYPEID_PLAYER;

    if (l_IsPlayer)
    {
        WorldPacket l_Data(l_WaterWalkingOpcodeTable[p_Enable][1]);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);                ///< Movement counter

        SendMessageToSet(&l_Data, true);
    }
    else
    {
        WorldPacket l_Data(l_WaterWalkingOpcodeTable[p_Enable][0]);
        l_Data.appendPackGUID(GetGUID());

        SendMessageToSet(&l_Data, true);
    }

    return true;
}

bool Unit::SetFeatherFall(bool p_Enable, bool p_PacketOnly /*= false*/)
{
    if (!p_PacketOnly)
    {
        if (p_Enable == HasUnitMovementFlag(MOVEMENTFLAG_FALLING_SLOW))
            return false;

        if (p_Enable)
            AddUnitMovementFlag(MOVEMENTFLAG_FALLING_SLOW);
        else
            RemoveUnitMovementFlag(MOVEMENTFLAG_FALLING_SLOW);
    }

    static Opcodes const l_FeatherFallOpcodeTable[2][2] =
    {
        { SMSG_SPLINE_MOVE_SET_NORMAL_FALL,  SMSG_MOVE_NORMAL_FALL  },
        { SMSG_SPLINE_MOVE_SET_FEATHER_FALL, SMSG_MOVE_FEATHER_FALL }
    };

    bool l_IsPlayer = GetTypeId() == TYPEID_PLAYER && ToPlayer()->m_mover->GetTypeId() == TYPEID_PLAYER;

    if (l_IsPlayer)
    {
        WorldPacket l_Data(l_FeatherFallOpcodeTable[p_Enable][1]);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);                ///< Movement counter

        SendMessageToSet(&l_Data, true);
    }
    else
    {
        WorldPacket l_Data(l_FeatherFallOpcodeTable[p_Enable][0]);
        l_Data.appendPackGUID(GetGUID());

        SendMessageToSet(&l_Data, true);
    }

    return true;
}

bool Unit::SetHover(bool p_Enable, bool p_PacketOnly /*= false*/)
{
    if (!p_PacketOnly)
    {
        if (p_Enable == HasUnitMovementFlag(MOVEMENTFLAG_HOVER))
            return false;

        float l_HoverHeight = GetFloatValue(UNIT_FIELD_HOVER_HEIGHT);

        if (p_Enable)
        {
            /// No need to check height on ascent
            AddUnitMovementFlag(MOVEMENTFLAG_HOVER);

            if (l_HoverHeight)
                UpdateHeight(GetPositionZ() + l_HoverHeight);
        }
        else
        {
            RemoveUnitMovementFlag(MOVEMENTFLAG_HOVER);

            if (l_HoverHeight)
            {
                float l_NewZ = GetPositionZ() - l_HoverHeight;

                UpdateAllowedPositionZ(GetPositionX(), GetPositionY(), l_NewZ);
                UpdateHeight(l_NewZ);
            }
        }
    }

    static Opcodes const l_HoverOpcodeTable[2][2] =
    {
        { SMSG_SPLINE_MOVE_UNSET_HOVER, SMSG_MOVE_UNSET_HOVER   },
        { SMSG_SPLINE_MOVE_SET_HOVER,   SMSG_MOVE_SET_HOVER     }
    };

    bool l_IsPlayer = GetTypeId() == TYPEID_PLAYER && ToPlayer()->m_mover->GetTypeId() == TYPEID_PLAYER;

    if (l_IsPlayer)
    {
        WorldPacket l_Data(l_HoverOpcodeTable[p_Enable][1]);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);                ///< Movement counter

        SendMessageToSet(&l_Data, true);
    }
    else
    {
        WorldPacket l_Data(l_HoverOpcodeTable[p_Enable][0]);
        l_Data.appendPackGUID(GetGUID());

        SendMessageToSet(&l_Data, true);
    }

    return true;
}

bool Unit::SetCollision(bool disable)
{
    if (disable == HasUnitMovementFlag(MOVEMENTFLAG_DISABLE_COLLISION))
        return false;

    if (disable)
        AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_COLLISION);
    else
        RemoveUnitMovementFlag(MOVEMENTFLAG_DISABLE_COLLISION);

    static Opcodes const l_CollisionOpcodeTable[2][2] =
    {
        { SMSG_SPLINE_MOVE_COLLISION_ENABLE,  SMSG_MOVE_ENABLE_COLLISION  },
        { SMSG_SPLINE_MOVE_COLLISION_DISABLE, SMSG_MOVE_DISABLE_COLLISION }
    };

    bool l_IsPlayer = GetTypeId() == TYPEID_PLAYER && ToPlayer()->m_mover->GetTypeId() == TYPEID_PLAYER;

    if (l_IsPlayer)
    {
        WorldPacket l_Data(l_CollisionOpcodeTable[disable][1]);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);                ///< Movement counter

        SendMessageToSet(&l_Data, true);
    }
    else
    {
        WorldPacket l_Data(l_CollisionOpcodeTable[disable][0]);
        l_Data.appendPackGUID(GetGUID());

        SendMessageToSet(&l_Data, true);
    }

    return true;
}

void Unit::NearTeleportTo(float x, float y, float z, float orientation, bool casting /*= false*/)
{
    DisableSpline();
    if (IsPlayer())
        ToPlayer()->TeleportTo(GetMapId(), x, y, z, orientation, TELE_TO_NOT_LEAVE_TRANSPORT | TELE_TO_NOT_LEAVE_COMBAT | TELE_TO_NOT_UNSUMMON_PET | (casting ? TELE_TO_SPELL : 0));
    else
    {
        Position l_Position;
        l_Position.m_positionX = x;
        l_Position.m_positionY = y;
        l_Position.m_positionZ = z;
        l_Position.m_orientation = orientation;
        SendTeleportPacket(l_Position);

        UpdatePosition(x, y, z, orientation, true);
        UpdateObjectVisibility();
    }
}

void Unit::SendTeleportPacket(Position &p_NewPosition)
{
    if (GetTypeId() == TYPEID_PLAYER)
    {
        WorldPacket l_TeleportPacket(SMSG_MOVE_TELEPORT, 38);
        bool l_HasVehicle = false;

        Position l_Position;
        l_Position.Relocate(p_NewPosition);

        if (TransportBase* l_TransportBase = GetDirectTransport())
            l_TransportBase->CalculatePassengerOffset(l_Position.m_positionX, l_Position.m_positionY, l_Position.m_positionZ, l_Position.m_orientation);

        l_TeleportPacket.appendPackGUID(GetGUID());
        l_TeleportPacket << uint32(0);                  //  SequenceIndex
        l_TeleportPacket << float(l_Position.GetPositionX());
        l_TeleportPacket << float(l_Position.GetPositionY());
        l_TeleportPacket << float(l_Position.GetPositionZ());
        l_TeleportPacket << float(l_Position.GetOrientation());
        l_TeleportPacket << float(GetOrientation());

        l_TeleportPacket.WriteBit(GetTransGUID() != 0);
        l_TeleportPacket.WriteBit(l_HasVehicle);
        l_TeleportPacket.FlushBits();

        if (GetTransGUID() != 0)
            l_TeleportPacket.appendPackGUID(GetTransGUID());

        if (l_HasVehicle)
        {
            l_TeleportPacket << uint8(0);   ///< VehicleSeatIndex
            l_TeleportPacket.WriteBit(0);   ///< VehicleExitVoluntary
            l_TeleportPacket.WriteBit(0);   ///< VehicleExitTeleport
            l_TeleportPacket.FlushBits();
        }

        ToPlayer()->SendDirectMessage(&l_TeleportPacket);
    }

    MovementInfo l_MovementInfo = m_movementInfo;

    /// Fix for near TP
    ///if (GetTypeId() != TYPEID_PLAYER)
    {
        l_MovementInfo.guid = GetGUID();
        l_MovementInfo.pos.Relocate(p_NewPosition);
        l_MovementInfo.time = getMSTime();
    }

    WorldPacket l_TeleportUpdatePacket(SMSG_MOVE_UPDATE_TELEPORT, 300);

    WorldSession::WriteMovementInfo(l_TeleportUpdatePacket, &l_MovementInfo);

    l_TeleportUpdatePacket << uint32(0);    ///< Movement force count

    l_TeleportUpdatePacket.WriteBit(true);  ///< HasWalkSpeed
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasRunSpeed
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasRunBack
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasSwimSpeed
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasSwimBack
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasFlightSpeed
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasFlightBack
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasTurnRate
    l_TeleportUpdatePacket.WriteBit(true);  ///< HasPitchRate
    l_TeleportUpdatePacket.FlushBits();

    l_TeleportUpdatePacket << float(GetSpeed(MOVE_WALK));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_RUN));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_RUN_BACK));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_SWIM));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_SWIM_BACK));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_FLIGHT));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_FLIGHT_BACK));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_TURN_RATE));
    l_TeleportUpdatePacket << float(GetSpeed(MOVE_PITCH_RATE));

    SendMessageToSet(&l_TeleportUpdatePacket, false);
}

bool Unit::UpdatePosition(float x, float y, float z, float orientation, bool teleport)
{
    // prevent crash when a bad coord is sent by the client
    if (!JadeCore::IsValidMapCoord(x, y, z, orientation))
        return false;

    bool turn = (GetOrientation() != orientation);
    bool relocated = (teleport || GetPositionX() != x || GetPositionY() != y || GetPositionZ() != z);

    if (turn)
        RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TURNING);

    if (relocated)
    {
        RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_MOVE);

        // move and update visible state if need
        if (IsPlayer())
            GetMap()->PlayerRelocation(ToPlayer(), x, y, z, orientation);
        else
        {
            GetMap()->CreatureRelocation(ToCreature(), x, y, z, orientation);
            // code block for underwater state update
            UpdateUnderwaterState(GetMap(), x, y, z);
        }
    }
    else if (turn)
        UpdateOrientation(orientation);

    return (relocated || turn);
}

//! Only server-side orientation update, does not broadcast to client
void Unit::UpdateOrientation(float orientation)
{
    SetOrientation(orientation);
    if (IsVehicle())
        GetVehicleKit()->RelocatePassengers();
}

//! Only server-side height update, does not broadcast to client
void Unit::UpdateHeight(float newZ)
{
    Relocate(GetPositionX(), GetPositionY(), newZ);
    if (IsVehicle())
        GetVehicleKit()->RelocatePassengers();
}

void Unit::SendThreatListUpdate()
{
    if (!getThreatManager().isThreatListEmpty())
    {
        uint32 l_Count = getThreatManager().getThreatList().size();

        WorldPacket l_Data(SMSG_THREAT_UPDATE, 1024);
        l_Data.appendPackGUID(GetGUID());
        l_Data << l_Count;

        std::list<HostileReference*>& l_ThreatList = getThreatManager().getThreatList();
        for (std::list<HostileReference*>::const_iterator l_Iter = l_ThreatList.begin(); l_Iter != l_ThreatList.end(); ++l_Iter)
        {
            l_Data.appendPackGUID((*l_Iter)->getUnitGuid());
            l_Data << uint32((*l_Iter)->getThreat());
        }

        SendMessageToSet(&l_Data, false);
    }
}

void Unit::SendChangeCurrentVictimOpcode(HostileReference* p_HostileReference)
{
    if (!getThreatManager().isThreatListEmpty())
    {
        uint32 l_Count = getThreatManager().getThreatList().size();

        WorldPacket l_Data(SMSG_HIGHEST_THREAT_UPDATE, 1 * 1024);
        l_Data.appendPackGUID(GetGUID());
        l_Data.appendPackGUID(p_HostileReference->getUnitGuid());
        l_Data << l_Count;

        std::list<HostileReference*>& l_ThreatList = getThreatManager().getThreatList();
        for (std::list<HostileReference*>::const_iterator l_Iter = l_ThreatList.begin(); l_Iter != l_ThreatList.end(); ++l_Iter)
        {
            l_Data.appendPackGUID((*l_Iter)->getUnitGuid());
            l_Data << uint32((*l_Iter)->getThreat());
        }

        SendMessageToSet(&l_Data, false);
    }
}

void Unit::SendClearThreatListOpcode()
{
    WorldPacket l_Data(SMSG_THREAT_CLEAR, 16 + 2);
    l_Data.appendPackGUID(GetGUID());
    SendMessageToSet(&l_Data, false);
}

void Unit::SendRemoveFromThreatListOpcode(HostileReference* p_HostileReference)
{
    WorldPacket l_Data(SMSG_THREAT_REMOVE, 16 + 2 + 16 + 2);
    l_Data.appendPackGUID(GetGUID());
    l_Data.appendPackGUID(p_HostileReference->getUnitGuid());
    SendMessageToSet(&l_Data, false);
}

void Unit::RewardRage(float baseRage)
{
    float addRage = baseRage;

    if (addRage < 0.0f)
        addRage = 0.0f;

    ModifyPower(POWER_RAGE, uint32(addRage * GetPowerCoeff(POWER_RAGE)));
}

void Unit::StopAttackFaction(uint32 faction_id)
{
    if (Unit* victim = getVictim())
    {
        if (victim->getFactionTemplateEntry()->Faction == faction_id)
        {
            AttackStop();
            if (IsNonMeleeSpellCasted(false))
                InterruptNonMeleeSpells(false);

            // melee and ranged forced attack cancel
            if (IsPlayer())
                ToPlayer()->SendAttackSwingCancelAttack();
        }
    }

    AttackerSet const& attackers = getAttackers();
    for (AttackerSet::const_iterator itr = attackers.begin(); itr != attackers.end();)
    {
        if ((*itr)->getFactionTemplateEntry()->Faction == faction_id)
        {
            (*itr)->AttackStop();
            itr = attackers.begin();
        }
        else
            ++itr;
    }

    getHostileRefManager().deleteReferencesForFaction(faction_id);

    for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr)
            (*itr)->StopAttackFaction(faction_id);
}

void Unit::OutDebugInfo() const
{
    sLog->outError(LOG_FILTER_UNITS, "Unit::OutDebugInfo");
    sLog->outInfo(LOG_FILTER_UNITS, "GUID " UI64FMTD ", entry %u, type %u, name %s", GetGUID(), GetEntry(), (uint32)GetTypeId(), GetName());
    sLog->outInfo(LOG_FILTER_UNITS, "OwnerGUID " UI64FMTD ", MinionGUID " UI64FMTD ", CharmerGUID " UI64FMTD ", CharmedGUID " UI64FMTD, GetOwnerGUID(), GetMinionGUID(), GetCharmerGUID(), GetCharmGUID());
    sLog->outInfo(LOG_FILTER_UNITS, "In world %u, unit type mask %u", (uint32)(IsInWorld() ? 1 : 0), m_unitTypeMask);
    if (IsInWorld())
        sLog->outInfo(LOG_FILTER_UNITS, "Mapid %u", GetMapId());

    std::ostringstream o;
    o << "Summon Slot: ";
    for (uint32 i = 0; i < MAX_SUMMON_SLOT; ++i)
        o << m_SummonSlot[i] << ", ";

    sLog->outInfo(LOG_FILTER_UNITS, "%s", o.str().c_str());
    o.str("");

    o << "Controlled List: ";
    for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr)
        o << (*itr)->GetGUID() << ", ";
    sLog->outInfo(LOG_FILTER_UNITS, "%s", o.str().c_str());
    o.str("");

    o << "Aura List: ";
    for (AuraApplicationMap::const_iterator itr = m_appliedAuras.begin(); itr != m_appliedAuras.end(); ++itr)
        o << itr->first << ", ";
    sLog->outInfo(LOG_FILTER_UNITS, "%s", o.str().c_str());
    o.str("");

    if (IsVehicle())
    {
        o << "Passenger List: ";
        for (SeatMap::iterator itr = GetVehicleKit()->Seats.begin(); itr != GetVehicleKit()->Seats.end(); ++itr)
            if (Unit* passenger = ObjectAccessor::GetUnit(*GetVehicleBase(), itr->second.Passenger))
                o << passenger->GetGUID() << ", ";
        sLog->outInfo(LOG_FILTER_UNITS, "%s", o.str().c_str());
    }

    if (GetVehicle())
        sLog->outInfo(LOG_FILTER_UNITS, "On vehicle %u.", GetVehicleBase()->GetEntry());
}

uint32 Unit::GetRemainingPeriodicAmount(uint64 caster, uint32 spellId, AuraType auraType, uint8 effectIndex) const
{
    uint32 amount = 0;
    AuraEffectList const& periodicAuras = GetAuraEffectsByType(auraType);
    for (AuraEffectList::const_iterator i = periodicAuras.begin(); i != periodicAuras.end(); ++i)
    {
        if ((*i)->GetCasterGUID() != caster || (*i)->GetId() != spellId || (*i)->GetEffIndex() != effectIndex || !(*i)->GetTotalTicks())
            continue;
        amount += uint32(((*i)->GetAmount() * std::max<int32>((*i)->GetTotalTicks() - int32((*i)->GetTickNumber()), 0)) / (*i)->GetTotalTicks());
        break;
    }

    return amount;
}

void Unit::SendClearTarget()
{
    WorldPacket l_Data(SMSG_BREAK_TARGET);
    l_Data.appendPackGUID(GetGUID());
    SendMessageToSet(&l_Data, false);
}

bool Unit::IsVisionObscured(Unit* victim, SpellInfo const* spellInfo)
{
    Aura* victimAura = nullptr;
    Aura* myAura = nullptr;
    Unit* victimCaster = NULL;
    Unit* myCaster = NULL;

    AuraEffectList const& vAuras = victim->GetAuraEffectsByType(SPELL_AURA_INTERFERE_TARGETTING);
    for (AuraEffectList::const_iterator i = vAuras.begin(); i != vAuras.end(); ++i)
    {
        victimAura = (*i)->GetBase();
        victimCaster = victimAura->GetCaster();
        break;
    }
    AuraEffectList const& myAuras = GetAuraEffectsByType(SPELL_AURA_INTERFERE_TARGETTING);
    for (AuraEffectList::const_iterator i = myAuras.begin(); i != myAuras.end(); ++i)
    {
        myAura = (*i)->GetBase();
        myCaster = myAura->GetCaster();
        break;
    }

    if ((myAura != nullptr && myCaster == NULL) || (victimAura != nullptr && victimCaster == NULL))
        return false; // Failed auras, will result in crash

    // E.G. Victim is in smoke bomb, and I'm not
    // Spells fail unless I'm friendly to the caster of victim's smoke bomb
    if (victimAura != nullptr && myAura == nullptr)
    {
        if (IsFriendlyTo(victimCaster) || spellInfo->GetMaxRange(false) <= 5.0f)
            return false;
        else
            return true;
    }
    // Victim is not in smoke bomb, while I am
    // Spells fail if my smoke bomb aura's caster is my enemy
    else if (myAura != nullptr && victimAura == nullptr)
    {
        if (IsFriendlyTo(myCaster))
            return false;
        else
            return true;
    }

    return false;
}

uint32 Unit::GetResistance(SpellSchoolMask mask) const
{
    int32 resist = -1;
    for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
        if (mask & (1 << i) && (resist < 0 || resist > int32(GetResistance(SpellSchools(i)))))
            resist = int32(GetResistance(SpellSchools(i)));

    // resist value will never be negative here
    return uint32(resist);
}

void CharmInfo::SetIsCommandAttack(bool val)
{
    m_isCommandAttack = val;
}

bool CharmInfo::IsCommandAttack()
{
    return m_isCommandAttack;
}

void CharmInfo::SetIsCommandFollow(bool val)
{
    _isCommandFollow = val;
}

bool CharmInfo::IsCommandFollow()
{
    return _isCommandFollow;
}

void CharmInfo::SaveStayPosition()
{
    //! At this point a new spline destination is enabled because of Unit::StopMoving()
    G3D::Vector3 const stayPos = m_unit->movespline->FinalDestination();
    m_stayX = stayPos.x;
    m_stayY = stayPos.y;
    m_stayZ = stayPos.z;
}

void CharmInfo::GetStayPosition(float &x, float &y, float &z)
{
    x = m_stayX;
    y = m_stayY;
    z = m_stayZ;
}

void CharmInfo::SetIsAtStay(bool val)
{
    m_isAtStay = val;
}

bool CharmInfo::IsAtStay()
{
    return m_isAtStay;
}

void CharmInfo::SetIsFollowing(bool val)
{
    m_isFollowing = val;
}

bool CharmInfo::IsFollowing()
{
    return m_isFollowing;
}

void CharmInfo::SetIsReturning(bool val)
{
    m_isReturning = val;
}

bool CharmInfo::IsReturning()
{
    return m_isReturning;
}

void Unit::SetInFront(WorldObject const* target)
{
    if (!HasUnitState(UNIT_STATE_CANNOT_TURN))
        SetOrientation(GetAngle(target));
}

void Unit::SetFacingTo(float ori)
{
    Movement::MoveSplineInit init(this);
    init.MoveTo(GetPositionX(), GetPositionY(), GetPositionZMinusOffset());
    if (GetTransGUID())
        init.DisableTransportPathTransformations(); // It makes no sense to target global orientation
    init.SetFacing(ori);
    init.Launch();

    SetOrientation(ori);
}

void Unit::SetFacingToObject(WorldObject* object)
{
    // never face when already moving
    if (!IsStopped())
        return;

    // TODO: figure out under what conditions creature will move towards object instead of facing it where it currently is.
    Movement::MoveSplineInit init(this);
    init.MoveTo(GetPositionX(), GetPositionY(), GetPositionZMinusOffset());
    init.SetFacing(GetAngle(object));   // when on transport, GetAngle will still return global coordinates (and angle) that needs transforming
    init.Launch();
}

void Unit::FocusTarget(Spell const* p_FocusSpell, WorldObject* p_Target)
{
    // already focused
    if (_focusSpell)
        return;

    _focusSpell = p_FocusSpell;
    SetGuidValue(UNIT_FIELD_TARGET, p_Target->GetGUID());
    if (p_FocusSpell->GetSpellInfo()->AttributesEx5 & SPELL_ATTR5_DONT_TURN_DURING_CAST)
        AddUnitState(UNIT_STATE_ROTATING);

    // Set server side orientation if needed (needs to be after attribute check)
    SetInFront(p_Target);
}

void Unit::ReleaseFocus(Spell const* focusSpell)
{
    // focused to something else
    if (focusSpell != _focusSpell)
        return;

    _focusSpell = nullptr;
    if (Unit* victim = getVictim())
        SetGuidValue(UNIT_FIELD_TARGET, victim->GetGUID());
    else
        SetGuidValue(UNIT_FIELD_TARGET, 0);

    if (focusSpell->GetSpellInfo()->AttributesEx5 & SPELL_ATTR5_DONT_TURN_DURING_CAST)
        ClearUnitState(UNIT_STATE_ROTATING);
}

void Unit::SendCanTurnWhileFalling(bool p_Apply)
{
    if (GetTypeId() != TYPEID_PLAYER)
        return;

    WorldPacket l_Data;

    if (p_Apply)
    {
        l_Data.Initialize(SMSG_MOVE_SET_CAN_TURN_WHILE_FALLING, 16 + 2 + 4);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);  // Movement counter
    }
    else
    {
        l_Data.Initialize(SMSG_MOVE_UNSET_CAN_TURN_WHILE_FALLING, 16 + 2 + 4);
        l_Data.appendPackGUID(GetGUID());
        l_Data << uint32(0);  // Movement counter
    }

    ToPlayer()->GetSession()->SendPacket(&l_Data);
}

bool Unit::IsSplineEnabled() const
{
    return movespline->Initialized() && !movespline->Finalized();
}

bool Unit::IsSplineFinished() const
{
    return movespline->Finalized();
}

uint32 Unit::GetDamageDoneInPastSecsBySpell(uint32 p_Secs, uint32 p_SpellId)
{
    uint32 damage = 0;

    for (DmgDoneList::iterator itr = m_dmgDone.begin(); itr != m_dmgDone.end(); itr++)
    {
        if (((*itr)->s_spellId && (getMSTime() - (*itr)->s_timestamp) <= (p_Secs * IN_MILLISECONDS)) && p_SpellId == (*itr)->s_spellId)
            damage += (*itr)->s_damage;
    }

    return damage;
};

void Unit::WriteMovementUpdate(WorldPacket &data) const
{
    WorldSession::WriteMovementInfo(data, (MovementInfo*)&m_movementInfo);
}

Unit* Unit::GetSoulSwapDotTarget()
{
    if (Unit* soulSwapTarget = sObjectAccessor->FindUnit(soulSwapTargetGUID))
    {
        if (soulSwapTarget->IsInWorld())
            return soulSwapTarget;
        else
            return NULL;
    }
    else
        return NULL;
}

void Unit::RemoveSoulSwapDOT(Unit* target)
{

    _SoulSwapDOTList.clear();

    AuraEffectList const mPeriodic = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
    for (AuraEffectList::const_iterator iter = mPeriodic.begin(); iter != mPeriodic.end(); ++iter)
    {
        if (!(*iter)) // prevent crash
            continue;

        if ((*iter)->GetSpellInfo()->SpellFamilyName != SPELLFAMILY_WARLOCK ||
            (*iter)->GetCasterGUID() != GetGUID()) // only warlock spells
            continue;

        _SoulSwapDOTList.push_back((*iter)->GetId());
        if (Aura* currentAura = target->GetAura((*iter)->GetId(), GetGUID()))
            _SoulSwapDOTData.insert(new SoulSwapAurasData(currentAura->GetId(), currentAura->GetDuration(), currentAura->GetStackAmount(),
            currentAura->GetEffect(0)->GetAmount(), currentAura->GetEffect(0)->GetPeriodicTimer()));
    }
}

void Unit::ApplySoulSwapDOT(Unit* caster, Unit* target)
{
    if (caster->GetGUID() != target->GetGUID())
    {
        for (AuraIdList::const_iterator iter = _SoulSwapDOTList.begin(); iter != _SoulSwapDOTList.end(); ++iter)
            AddAura((*iter), target);

        /// Restore all aura spell mods
        for (std::set<SoulSwapAurasData*>::iterator itr = _SoulSwapDOTData.begin(); itr != _SoulSwapDOTData.end(); ++itr)
        {
            if (Aura* appliedAura = target->GetAura((*itr)->m_id, GetGUID()))
            {
                /// Maybe we should refresh duration
                if (!GetSoulSwapRefreshDuration())
                    appliedAura->SetDuration((*itr)->m_duration);

                appliedAura->SetStackAmount((*itr)->m_stacks);
                if (appliedAura->GetEffect(EFFECT_0)) {
                    appliedAura->GetEffect(EFFECT_0)->SetAmount((*itr)->m_damage);
                    appliedAura->GetEffect(EFFECT_0)->SetPeriodicTimer((*itr)->m_amplitude);
                }
                appliedAura->SetNeedClientUpdateForTargets();
            }

            delete (*itr);
        }
    }

    _SoulSwapDOTList.clear();
    _SoulSwapDOTData.clear();
}

Unit* Unit::GetSimulacrumTarget()
{
    if (Unit* simulacrumTarget = sObjectAccessor->FindUnit(simulacrumTargetGUID))
    {
        if (simulacrumTarget->IsInWorld())
            return simulacrumTarget;
        else
            return NULL;
    }
    else
        return NULL;
}

void Unit::BuildValuesUpdate(uint8 updateType, ByteBuffer* data, Player* target) const
{
    if (!target)
        return;

    ByteBuffer fieldBuffer;

    UpdateMask updateMask;
    updateMask.SetCount(m_valuesCount);

    uint32* flags;
    uint32 visibleFlag = GetUpdateFieldData(target, flags);

    Creature const* creature = ToCreature();
    for (uint16 index = 0; index < m_valuesCount; ++index)
    {
        if (_fieldNotifyFlags & flags[index] ||
            ((flags[index] & visibleFlag) & UF_FLAG_SPECIAL_INFO) ||
            ((updateType == UPDATETYPE_VALUES ? _changesMask.GetBit(index) : m_uint32Values[index]) && (flags[index] & visibleFlag)) ||
            (index == UNIT_FIELD_AURA_STATE && HasFlag(UNIT_FIELD_AURA_STATE, PER_CASTER_AURA_STATE_MASK)))
        {
            updateMask.SetBit(index);

            if (index == UNIT_FIELD_NPC_FLAGS)
            {
                uint32 appendValue = m_uint32Values[UNIT_FIELD_NPC_FLAGS];

                if (creature)
                    if (!target->canSeeSpellClickOn(creature))
                        appendValue &= ~UNIT_NPC_FLAG_SPELLCLICK;

                fieldBuffer << uint32(appendValue);
            }
            else if (index == UNIT_FIELD_AURA_STATE)
            {
                // Check per caster aura states to not enable using a spell in client if specified aura is not by target
                fieldBuffer << BuildAuraStateUpdateForTarget(target);
            }
            // FIXME: Some values at server stored in float format but must be sent to client in uint32 format
            else if (index >= UNIT_FIELD_ATTACK_ROUND_BASE_TIME && index <= UNIT_FIELD_RANGED_ATTACK_ROUND_BASE_TIME)
            {
                // convert from float to uint32 and send
                fieldBuffer << uint32(m_floatValues[index] < 0 ? 0 : m_floatValues[index]);
            }
            // there are some float values which may be negative or can't get negative due to other checks
            else if ((index >= UNIT_FIELD_STAT_NEG_BUFF   && index < UNIT_FIELD_STAT_NEG_BUFF + MAX_STATS) ||
                (index >= UNIT_FIELD_STAT_POS_BUFF   && index < UNIT_FIELD_STAT_POS_BUFF + MAX_STATS) ||
                (index >= UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE  && index < (UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE + MAX_SPELL_SCHOOL)) ||
                (index >= UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE  && index < (UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE + MAX_SPELL_SCHOOL)))
            {
                fieldBuffer << uint32(m_floatValues[index]);
            }
            // Gamemasters should be always able to select units - remove not selectable flag
            else if (index == UNIT_FIELD_FLAGS)
            {
                uint32 appendValue = m_uint32Values[UNIT_FIELD_FLAGS];
                if (target->isGameMaster())
                    appendValue &= ~UNIT_FLAG_NOT_SELECTABLE;

                fieldBuffer << uint32(appendValue);
            }
            // use modelid_a if not gm, _h if gm for CREATURE_FLAG_EXTRA_TRIGGER creatures
            else if (index == UNIT_FIELD_DISPLAY_ID)
            {
                uint32 displayId = m_uint32Values[UNIT_FIELD_DISPLAY_ID];
                if (creature)
                {
                    CreatureTemplate const* cinfo = creature->GetCreatureTemplate();

                    // this also applies for transform auras
                    if (SpellInfo const* transform = sSpellMgr->GetSpellInfo(getTransForm()))
                        for (uint8 i = 0; i < transform->EffectCount; ++i)
                            if (transform->Effects[i].IsAura(SPELL_AURA_TRANSFORM))
                                if (CreatureTemplate const* transformInfo = sObjectMgr->GetCreatureTemplate(transform->Effects[i].MiscValue))
                                {
                                    cinfo = transformInfo;
                                    break;
                                }

                    if (cinfo->flags_extra & CREATURE_FLAG_EXTRA_TRIGGER)
                    {
                        if (target->isGameMaster())
                        {
                            if (cinfo->Modelid1)
                                displayId = cinfo->Modelid1; // Modelid1 is a visible model for gms
                            else
                                displayId = 17519; // world visible trigger's model
                        }
                        else
                        {
                            if (cinfo->Modelid2)
                                displayId = cinfo->Modelid2; // Modelid2 is an invisible model for players
                            else
                                displayId = 11686; // world invisible trigger's model
                        }
                    }
                }

                fieldBuffer << uint32(displayId);
            }
            // hide lootable animation for unallowed players
            else if (index == OBJECT_FIELD_DYNAMIC_FLAGS)
            {
                uint32 dynamicFlags = m_uint32Values[OBJECT_FIELD_DYNAMIC_FLAGS] & ~(UNIT_DYNFLAG_TAPPED | UNIT_DYNFLAG_TAPPED_BY_PLAYER);

                if (creature)
                {
                    if (creature->hasLootRecipient())
                    {
                        dynamicFlags |= UNIT_DYNFLAG_TAPPED;
                        if (creature->isTappedBy(target))
                            dynamicFlags |= UNIT_DYNFLAG_TAPPED_BY_PLAYER;
                    }

                    if (!target->isAllowedToLoot(creature))
                        dynamicFlags &= ~UNIT_DYNFLAG_LOOTABLE;
                }

                // unit UNIT_DYNFLAG_TRACK_UNIT should only be sent to caster of SPELL_AURA_MOD_STALKED auras
                if (dynamicFlags & UNIT_DYNFLAG_TRACK_UNIT)
                    if (!HasAuraTypeWithCaster(SPELL_AURA_MOD_STALKED, target->GetGUID()))
                        dynamicFlags &= ~UNIT_DYNFLAG_TRACK_UNIT;

                fieldBuffer << dynamicFlags;
            }
            // FG: pretend that OTHER players in own group are friendly ("blue")
            else if (index == UNIT_FIELD_SHAPESHIFT_FORM || index == UNIT_FIELD_FACTION_TEMPLATE)
            {
                uint32 l_Value = m_uint32Values[index];
                if (index == UNIT_FIELD_FACTION_TEMPLATE && creature && creature->IsAIEnabled)
                    creature->AI()->OnSendFactionTemplate(l_Value, target);

                if (IsControlledByPlayer() && target != this && sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && IsInRaidWith(target))
                {
                    FactionTemplateEntry const* ft1 = getFactionTemplateEntry();
                    FactionTemplateEntry const* ft2 = target->getFactionTemplateEntry();
                    if (ft1 && ft2 && !ft1->IsFriendlyTo(*ft2))
                    {
                        if (index == UNIT_FIELD_SHAPESHIFT_FORM)
                            // Allow targetting opposite faction in party when enabled in config
                            fieldBuffer << (m_uint32Values[UNIT_FIELD_SHAPESHIFT_FORM] & ((UNIT_BYTE2_FLAG_SANCTUARY /*| UNIT_BYTE2_FLAG_AURAS | UNIT_BYTE2_FLAG_UNK5*/) << 8)); // this flag is at uint8 offset 1 !!
                        else
                            // pretend that all other HOSTILE players have own faction, to allow follow, heal, rezz (trade wont work)
                            fieldBuffer << uint32(target->getFaction());
                    }
                    else
                        fieldBuffer << l_Value;
                }
                else
                    fieldBuffer << l_Value;
            }
            else
            {
                // send in current format (float as float, uint32 as uint32)
                fieldBuffer << m_uint32Values[index];
            }
        }
    }

    *data << uint8(updateMask.GetBlockCount());
    updateMask.AppendToPacket(data);
    data->append(fieldBuffer);
}

float Unit::CalculateDamageDealtFactor(Unit* p_Unit, Creature* p_Creature)
{
    if (!p_Unit || !p_Creature)
        return 1.0f;

    uint8 l_TargetExpansion = p_Creature->GetCreatureTemplate()->expansion;
    int32 l_LevelDiff = p_Unit->getLevel() - p_Creature->getLevel();

    float l_DamageDealtFactor = 1.0f;


    if (l_LevelDiff && l_TargetExpansion <= EXPANSION_MISTS_OF_PANDARIA)
    {
        if (l_LevelDiff < 1)
        {
            // Negative numbers fiyyy
            l_DamageDealtFactor = 1.0f;
        }
        else if (l_LevelDiff < 5)
        {
            // Ranges from 1.0625 to 1.25 vs. 1-4 LevelDiffs
            l_DamageDealtFactor = 1 + 0.0625f * l_LevelDiff;
        }
        else if (l_LevelDiff < 10)
        {
            // Ranges from 4.0 to 6.0 vs. 5-9 LevelDiffs
            l_DamageDealtFactor = 1.5f + 0.5f * l_LevelDiff;
        }
        else
        {
            // Maximum factor of 16.5 vs. 10+ LevelDiffs
            l_DamageDealtFactor = 16.5f;
        }
    }

    uint16 l_IntendedItemLevelByExpansion[MAX_EXPANSION] = { 65, 115, 200, 346, 463, 609 };
    uint16 l_MaxPlayerLevelsByExpansion[MAX_EXPANSION] = { 69, 79, 84, 89, 99, 109 };

    if (l_TargetExpansion > 0 && p_Unit->IsPlayer())
    {
        Player* p_Player = p_Unit->ToPlayer();

        if ((p_Player->getLevel() <= l_MaxPlayerLevelsByExpansion[l_TargetExpansion - 1]) && p_Player->GetAverageItemLevelEquipped() > l_IntendedItemLevelByExpansion[l_TargetExpansion - 1])
        {
            float l_AltDamageDealtFactor = 1 + 5 / 3 * 0.01f * (p_Player->GetAverageItemLevelEquipped() - l_IntendedItemLevelByExpansion[l_TargetExpansion - 1]);
            l_DamageDealtFactor = std::max(l_DamageDealtFactor, l_AltDamageDealtFactor);
        }
    }

    return l_DamageDealtFactor;
}

float Unit::CalculateDamageTakenFactor(Unit* p_Unit, Creature* p_Creature)
{
    if (!p_Unit || !p_Creature)
        return 1.0f;

    uint8 l_TargetExpansion = p_Creature->GetCreatureTemplate()->expansion;
    int32 l_LevelDiff = p_Unit->getLevel() - p_Creature->getLevel();

    float l_DamageTakenFactor = 1.0f;

    if (l_LevelDiff > 0 && l_TargetExpansion <= EXPANSION_MISTS_OF_PANDARIA)
    {
        // 10% DR per level diff, with a floor of 10%
        l_DamageTakenFactor = std::max(1.0f - 0.1f * l_LevelDiff, 0.1f);
    }

    uint16 l_IntendedItemLevelByExpansion[MAX_EXPANSION] = {65, 115, 200, 346, 463, 609};
    uint16 l_MaxPlayerLevelsByExpansion[MAX_EXPANSION] = {69, 79, 84, 89, 99, 109};

    if (l_TargetExpansion && l_LevelDiff > 0 && p_Unit->IsPlayer())
    {
        Player* p_Player = p_Unit->ToPlayer();

        if ((p_Player->getLevel() <= l_MaxPlayerLevelsByExpansion[l_TargetExpansion - 1]) && p_Player->GetAverageItemLevelEquipped() > l_IntendedItemLevelByExpansion[l_TargetExpansion - 1])
        {
            float l_ItemLevelFactor = p_Player->GetAverageItemLevelEquipped() - l_IntendedItemLevelByExpansion[l_TargetExpansion - 1];
            l_ItemLevelFactor = std::min(l_ItemLevelFactor, 99.9f);

            float l_AltDamageTakenFactor = 1 - 0.01f * l_ItemLevelFactor;
            l_DamageTakenFactor = std::min(l_DamageTakenFactor, l_AltDamageTakenFactor);
        }
    }

    return l_DamageTakenFactor;
}


float Unit::GetDiminishingPVPDamage(SpellInfo const* p_Spellproto) const
{
    /// Explicit diminishing Pvp damage
    switch (p_Spellproto->SpellFamilyName)
    {
    case SPELLFAMILY_DRUID:
    {
        /// Rake - In pvp, damages reduce by 20%
        if (p_Spellproto->SpellFamilyFlags[0] & 0x1000)
            return -20.0f;
        /// Rip - In pvp, damages reduce by 20%
        else if (p_Spellproto->SpellFamilyFlags[0] & 0x800000 && p_Spellproto->SpellFamilyFlags[2] & 0x200000)
            return -20.0f;
        /// Starsurge - In pvp, damages reduce by 15%
        else if (p_Spellproto->SpellFamilyFlags[2] & 0x2000000)
            return -15.0f;
        break;
    }
    case SPELLFAMILY_PRIEST:
    {
        /// Devouring Plague - In pvp, damages reduce by 10%
        if (p_Spellproto->SpellFamilyFlags[3] & 0x10)
            return -10.0f;
        /// Mind Blast - In pvp, damages reduce by 10%
        else if (p_Spellproto->SpellFamilyFlags[0] & 0x2000)
            return -10.0f;
        /// Mind Spike - In pvp, damages reduce by 10%
        else if (p_Spellproto->SpellFamilyFlags[2] & 0x10000)
            return -10.0f;
        break;
    }
    case SPELLFAMILY_WARLOCK:
    {
        /// Chaos Bolt - In pvp, damages increase by 33%
        if (p_Spellproto->SpellFamilyFlags[1] & 0x2000)
            return 33.0f;
        /// Corruption - In pvp, damages reduce by 10%
        else if (p_Spellproto->SpellFamilyFlags[0] & 0x2)
            return -10.0f;
        /// Agnoy - In pvp, damages reduce by 10%
        else if (p_Spellproto->SpellFamilyFlags[0] & 0x400)
            return -10.0f;
        /// Unstable Affliction - In pvp, damages reduce by 10%
        else if (p_Spellproto->SpellFamilyFlags[1] & 0x100)
            return -10.0f;
        /// Unstable ShadowBurn - In pvp, damages increase by 20%
        else if (p_Spellproto->SpellFamilyFlags[3] & 0x400000)
            return 20.0f;
        /// Haunt - In pvp, damages reduce by 25%
        else if (p_Spellproto->SpellFamilyFlags[3] & 0x20)
            return -25.0f;
        break;
    }
    case SPELLFAMILY_SHAMAN:
    {
        /// Lava Burst - In pvp, damages increase by 20%
        if (p_Spellproto->SpellFamilyFlags[1] & 0x1000)
            return 20.0f;
        break;
    }
    case SPELLFAMILY_HUNTER:
    {
        /// Chimera Shot - In pvp, damages reduce by 10%
        if (p_Spellproto->SpellFamilyFlags[2] & 0x1)
            return -10.0f;
        break;
    }
    case SPELLFAMILY_MONK:
    {
        /// Rising Sun Kick - In pvp, damage increase by 20%
        if (p_Spellproto->SpellFamilyFlags[1] & 0x80)
            return 20.0f;
        break;
    }
    case SPELLFAMILY_DEATHKNIGHT:
    {
        /// Frost Strike - In pvp, damages reduce by 10%
        if (p_Spellproto->SpellFamilyFlags[1] & 0x4)
            return -10.0f;
        /// Obliterate - In pvp, damages reduce by 10%
        if (p_Spellproto->SpellFamilyFlags[1] & 0x20000)
            return -10.0f;
        break;
    }
    case SPELLFAMILY_MAGE:
    {
        /// Ice Nova - In pvp, damage reduce by 20%
        if (p_Spellproto->SpellFamilyFlags[3] & 0x80000)
            return -20.0f;

        /// Living Bomb - In pvp, damage reduce by 15%
        if (p_Spellproto->Id == 44461)
            return -15.0f;
        break;
    }
    case SPELLFAMILY_WARRIOR:
    {
        /// Execute - In pvp, damage reduce by 10%
        if (p_Spellproto->SpellFamilyFlags[0] & 0x20000000)
            return -10.0f;
        /// Mortal Strike - In pvp, damage reduce by 10%
        if (p_Spellproto->Id == 16856)
            return -10.0f;
        break;
    }

    default:
        break;
    }

    return 0.0f;
}

void Unit::BuildEncounterFrameData(WorldPacket* p_Data, bool p_Engage, uint8 p_TargetFramePriority /*= 0*/)
{
    if (p_Engage)
    {
        p_Data->Initialize(SMSG_INSTANCE_ENCOUNTER_ENGAGE_UNIT, 16 + 2 + 1);
        p_Data->append(GetPackGUID());
        *p_Data << uint8(p_TargetFramePriority);
    }
    else
    {
        p_Data->Initialize(SMSG_INSTANCE_ENCOUNTER_DISENGAGE_UNIT, 16 + 2);
        p_Data->append(GetPackGUID());
    }
}

bool Unit::AddPoisonTarget(uint32 p_SpellID, uint32 p_LowGuid)
{
    /// First target registration
    if (m_PoisonTargets.find(p_LowGuid) == m_PoisonTargets.end())
    {
        std::set<uint32> l_SpellSet;
        l_SpellSet.insert(p_SpellID);
        m_PoisonTargets.insert(std::make_pair(p_LowGuid, l_SpellSet));
        return true;
    }

    /// Target has already this spell registered
    if (m_PoisonTargets[p_LowGuid].find(p_SpellID) != m_PoisonTargets[p_LowGuid].end())
        return false;

    /// Register new spell for target
    m_PoisonTargets[p_LowGuid].insert(p_SpellID);
    return true;
}

bool Unit::HasPoisonTarget(uint32 p_LowGuid) const
{
    for (auto l_Iter : m_PoisonTargets)
    {
        if (l_Iter.first == p_LowGuid)
            return true;
    }

    return false;
}

void Unit::RemovePoisonTarget(uint32 p_LowGuid, uint32 p_SpellID)
{
    /// Target is not registered
    if (m_PoisonTargets.find(p_LowGuid) == m_PoisonTargets.end())
        return;

    /// Spell is not registered for target
    if (m_PoisonTargets[p_LowGuid].find(p_SpellID) == m_PoisonTargets[p_LowGuid].end())
        return;

    /// Unregister spell for target
    m_PoisonTargets[p_LowGuid].erase(p_SpellID);

    /// If no spell registered, unregister target
    if (m_PoisonTargets[p_LowGuid].empty())
        m_PoisonTargets.erase(p_LowGuid);
}

void Unit::ClearPoisonTargets()
{
    m_PoisonTargets.clear();
}

void Unit::SetChannelSpellID(uint32 p_SpellID)
{
    SetChannelSpellID(sSpellMgr->GetSpellInfo(p_SpellID));
}

void Unit::SetChannelSpellID(SpellInfo const* p_SpellInfo)
{
    if (p_SpellInfo)
    {
        SetUInt32Value(UNIT_FIELD_CHANNEL_SPELL, p_SpellInfo->Id);
        SetUInt32Value(UNIT_FIELD_CHANNEL_SPELL_XSPELL_VISUAL, p_SpellInfo->FirstSpellXSpellVisualID);
    }
    else
    {
        SetUInt32Value(UNIT_FIELD_CHANNEL_SPELL, 0);
        SetUInt32Value(UNIT_FIELD_CHANNEL_SPELL_XSPELL_VISUAL, 0);
    }
}

bool Unit::IsOutdoors()
{
    if (GetExactDistSq(&m_LastOutdoorPosition) < 4.0f * 4.0f)
        return m_LastOutdoorStatus;
    else
    {
        m_LastOutdoorPosition.Relocate(GetPositionX(), GetPositionY(), GetPositionZ());
        m_LastOutdoorStatus = GetMap()->IsOutdoors(GetPositionX(), GetPositionY(), GetPositionZ());
        return m_LastOutdoorStatus;
    }
}

uint32 Unit::GetZoneId(bool p_ForceRecalc) const
{
    if (!p_ForceRecalc && GetExactDistSq(&m_LastZonePosition) < 4.0f * 4.0f)
        return m_LastZoneId;
    else
    {
        const_cast<Position*>(&m_LastZonePosition)->Relocate(GetPositionX(), GetPositionY(), GetPositionZ());
        *(const_cast<uint32*>(&m_LastZoneId)) = WorldObject::GetZoneId();
        return m_LastZoneId;
    }
}

uint32 Unit::GetAreaId(bool p_ForceRecalc) const
{
    if (!p_ForceRecalc && GetExactDistSq(&m_LastAreaPosition) < 4.0f * 4.0f)
        return m_LastAreaId;
    else
    {
        const_cast<Position*>(&m_LastAreaPosition)->Relocate(GetPositionX(), GetPositionY(), GetPositionZ());
        *(const_cast<uint32*>(&m_LastAreaId)) = WorldObject::GetAreaId();
        return m_LastAreaId;
    }
}

void Unit::GetZoneAndAreaId(uint32& p_ZoneId, uint32& p_AreaId, bool p_ForceRecalc) const
{
    if (!p_ForceRecalc && GetExactDistSq(&m_LastAreaPosition) < 4.0f * 4.0f && GetExactDistSq(&m_LastZonePosition) < 4.0f * 4.0f)
    {
        p_ZoneId = m_LastZoneId;
        p_AreaId = m_LastAreaId;
        return;
    }

    const_cast<Position*>(&m_LastZonePosition)->Relocate(GetPositionX(), GetPositionY(), GetPositionZ());
    const_cast<Position*>(&m_LastAreaPosition)->Relocate(GetPositionX(), GetPositionY(), GetPositionZ());
    WorldObject::GetZoneAndAreaId(p_ZoneId, p_AreaId);
    *(const_cast<uint32*>(&m_LastZoneId)) = p_ZoneId;
    *(const_cast<uint32*>(&m_LastAreaId)) = p_AreaId;
}
