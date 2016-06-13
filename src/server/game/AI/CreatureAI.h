////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_CREATUREAI_H
#define TRINITY_CREATUREAI_H

#include "Creature.h"
#include "UnitAI.h"
#include "Common.h"
#include "InstanceScript.h"
#include "Player.h"

class WorldObject;
class Unit;
class Creature;
class Player;
class SpellInfo;

struct SpellDestination;

#define TIME_INTERVAL_LOOK   5000
#define VISIBILITY_RANGE    10000

//Spell targets used by SelectSpell
enum SelectTargetType
{
    SELECT_TARGET_DONTCARE = 0,                             //All target types allowed

    SELECT_TARGET_SELF,                                     //Only Self casting

    SELECT_TARGET_SINGLE_ENEMY,                             //Only Single Enemy
    SELECT_TARGET_AOE_ENEMY,                                //Only AoE Enemy
    SELECT_TARGET_ANY_ENEMY,                                //AoE or Single Enemy

    SELECT_TARGET_SINGLE_FRIEND,                            //Only Single Friend
    SELECT_TARGET_AOE_FRIEND,                               //Only AoE Friend
    SELECT_TARGET_ANY_FRIEND                                //AoE or Single Friend
};

//Spell Effects used by SelectSpell
enum SelectEffect
{
    SELECT_EFFECT_DONTCARE = 0,                             //All spell effects allowed
    SELECT_EFFECT_DAMAGE,                                   //Spell does damage
    SELECT_EFFECT_HEALING,                                  //Spell does healing
    SELECT_EFFECT_AURA                                      //Spell applies an aura
};

enum SCEquip
{
    EQUIP_NO_CHANGE = -1,
    EQUIP_UNEQUIP   = 0
};

class CreatureAI : public UnitAI
{
    protected:
        Creature* const me;

        bool UpdateVictim();
        bool UpdateVictimWithGaze();

        void SetGazeOn(Unit* target);

        Creature* DoSummon(uint32 entry, Position const& pos, uint32 despawnTime = 30000, TempSummonType summonType = TEMPSUMMON_CORPSE_TIMED_DESPAWN);
        Creature* DoSummon(uint32 entry, WorldObject* obj, float radius = 5.0f, uint32 despawnTime = 30000, TempSummonType summonType = TEMPSUMMON_CORPSE_TIMED_DESPAWN);
        Creature* DoSummonFlyer(uint32 entry, WorldObject* obj, float flightZ, float radius = 5.0f, uint32 despawnTime = 30000, TempSummonType summonType = TEMPSUMMON_CORPSE_TIMED_DESPAWN);

    public:
        void Talk(uint8 id, uint64 WhisperGuid = 0, uint32 range = 0);
        explicit CreatureAI(Creature* creature) : UnitAI(creature), me(creature), m_MoveInLineOfSight_locked(false), m_canSeeEvenInPassiveMode(false) {}

        virtual ~CreatureAI() {}

        /// == Reactions At =================================

        // Called if IsVisible(Unit* who) is true at each who move, reaction at visibility zone enter
        void MoveInLineOfSight_Safe(Unit* who);

        bool CanSeeEvenInPassiveMode() { return m_canSeeEvenInPassiveMode; }
        void SetCanSeeEvenInPassiveMode(bool canSeeEvenInPassiveMode) { m_canSeeEvenInPassiveMode = canSeeEvenInPassiveMode; }

        // Called in Creature::Update when deathstate = DEAD. Inherited classes may maniuplate the ability to respawn based on scripted events.
        virtual bool CanRespawn() { return true; }

        // Called for reaction at stopping attack at no attackers or targets
        virtual void EnterEvadeMode();

        // Called for reaction at enter to combat if not in combat yet (enemy can be NULL)
        virtual void EnterCombat(Unit* p_Attacker)
        {
            UNUSED(p_Attacker);
        }

        // Called when the creature is despawned
        virtual void JustDespawned() {}

        // Called when the creature is killed
        virtual void JustDied(Unit* p_Killer)
        {
            UNUSED(p_Killer);
        }

        // Called when the creature kills a unit
        virtual void KilledUnit(Unit* p_Killed)
        {
            UNUSED(p_Killed);
        }

        // Called when the killer is rewarded with currencies
        virtual void CurrenciesRewarder(bool& p_Result)
        {
            UNUSED(p_Result);
        }

        /// Called when check interact distance (for example: gossips)
        virtual bool CanByPassDistanceCheck() const { return false; }

        // Called when check LOS
        virtual bool CanBeTargetedOutOfLOS() { return false; }
        virtual bool CanTargetOutOfLOS() { return false; }

        // Called when the creature summon successfully other creature
        virtual void JustSummoned(Creature* p_Summon)
        {
            UNUSED(p_Summon);
        }

        virtual void IsSummonedBy(Unit* p_Summoner)
        {
            UNUSED(p_Summoner);
        }

        /// Called when the creature summons successfully AreaTriggers
        virtual void AreaTriggerCreated(AreaTrigger* p_AreaTrigger)
        {
            UNUSED(p_AreaTrigger);
        }

        /// Called when an AreaTrigger summoned by the creature despawns
        virtual void AreaTriggerDespawned(AreaTrigger* p_AreaTrigger)
        {
            UNUSED(p_AreaTrigger);
        }

        virtual void SummonedCreatureDespawn(Creature* p_Summon)
        {
            UNUSED(p_Summon);
        }

        virtual void SummonedCreatureDies(Creature* p_Summon, Unit* p_Killer)
        {
            UNUSED(p_Summon);
            UNUSED(p_Killer);
        }

        // Called when hit by a spell
        virtual void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo)
        {
            UNUSED(p_Caster);
            UNUSED(p_SpellInfo);
        }

        // Called when spell hits a target
        virtual void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo)
        {
            UNUSED(p_Target);
            UNUSED(p_SpellInfo);
        }

        /// Called when spell hits a destination
        virtual void SpellHitDest(SpellDestination const* p_Dest, SpellInfo const* p_SpellInfo)
        {
            UNUSED(p_Dest);
            UNUSED(p_SpellInfo);
        }

        /// Called when spell miss a target
        virtual void SpellMissTarget(Unit* p_Target, SpellInfo const* p_SpellInfo, SpellMissInfo p_MissInfo)
        {
            UNUSED(p_Target);
            UNUSED(p_SpellInfo);
            UNUSED(p_MissInfo);
        }

        /// Called when successful cast a spell
        virtual void OnSpellCasted(SpellInfo const* p_SpellInfo)
        {
            UNUSED(p_SpellInfo);
        }

        /// Called when a spell is finished
        virtual void OnSpellFinished(SpellInfo const* p_SpellInfo)
        {
            UNUSED(p_SpellInfo);
        }

        // Called when the creature is target of hostile action: swing, hostile spell landed, fear/etc)
        //virtual void AttackedBy(Unit* attacker);
        virtual bool IsEscorted() { return false; }
        virtual bool IsPassived() { return false; }

        // Called when creature is spawned or respawned (for reseting variables)
        virtual void JustRespawned() { Reset(); }

        // Called at waypoint reached or point movement finished
        virtual void MovementInform(uint32 p_Type, uint32 p_ID)
        {
            UNUSED(p_Type);
            UNUSED(p_ID);
        }

        void OnCharmed(bool apply);

        // Called at reaching home after evade
        virtual void JustReachedHome() { }

        void DoZoneInCombat(Creature* creature = NULL, float maxRangeToNearestTarget = 50.0f);
        void DoAttackerAreaInCombat(Unit* attacker, float range, Unit* pUnit = NULL);

        // Called at text emote receive from player
        virtual void ReceiveEmote(Player* p_Player, uint32 p_EmoteID)
        {
            UNUSED(p_Player);
            UNUSED(p_EmoteID);
        }

        // Called when owner takes damage
        virtual void OwnerDamagedBy(Unit* p_Attacker)
        {
            UNUSED(p_Attacker);
        }

        // Called when owner attacks something
        virtual void OwnerAttacked(Unit* p_Target)
        {
            UNUSED(p_Target);
        }

        // Called when a creature regen one of his power
        virtual void RegeneratePower(Powers p_Power, int32& p_Value)
        {
            UNUSED(p_Power);
            UNUSED(p_Value);
        }

        // Called when a creature has one of hi power modified
        virtual void PowerModified(Powers p_Power, int32 p_Value)
        {
            UNUSED(p_Power);
            UNUSED(p_Value);
        }

        // Called when a creature has one of his power set
        virtual void SetPower(Powers p_Power, int32 p_Value)
        {
            UNUSED(p_Power);
            UNUSED(p_Value);
        }

        /// Called when taunted
        virtual void OnTaunt(Unit* p_Taunter)
        {
            UNUSED(p_Taunter);
        }

        /// Called when a casting time is calculated
        virtual void OnCalculateCastingTime(SpellInfo const* p_SpellInfo, int32& p_CastingTime)
        {
            UNUSED(p_SpellInfo);
            UNUSED(p_CastingTime);
        }

        // Called at any threat added from any attacker (before threat apply)
        virtual void OnAddThreat(Unit* p_Victim, float& p_Threat, SpellSchoolMask p_SchoolMask, SpellInfo const* p_ThreatSpell)
        {
            UNUSED(p_Victim);
            UNUSED(p_Threat);
            UNUSED(p_SchoolMask);
            UNUSED(p_ThreatSpell);
        }

        /// Called when at HandleGarrisonGetShipmentInfoOpcode() is received
        virtual int OnShipmentIDRequest(Player* p_Player)
        {
            UNUSED(p_Player);
            return -1;
        }

        /// == Triggered Actions Requested ==================

        // Called when creature attack expected (if creature can and no have current victim)
        // Note: for reaction at hostile action must be called AttackedBy function.
        //virtual void AttackStart(Unit*) {}

        // Called at World update tick
        //virtual void UpdateAI(const uint32 /*diff*/) {}

        /// == State checks =================================

        // Is unit visible for MoveInLineOfSight
        //virtual bool IsVisible(Unit*) const { return false; }

        // called when the corpse of this creature gets removed
        virtual void CorpseRemoved(uint32& p_RespawnDelay)
        {
            UNUSED(p_RespawnDelay);
        }

        // Called when victim entered water and creature can not enter water
        //virtual bool canReachByRangeAttack(Unit*) { return false; }

        /// == Fields =======================================

        // Pointer to controlled by AI creature
        //Creature* const me;

        virtual void PassengerBoarded(Unit* p_Passenger, int8 p_SeatID, bool p_Apply)
        {
            UNUSED(p_Passenger);
            UNUSED(p_SeatID);
            UNUSED(p_Apply);
        }

        virtual void OnVehicleExited(Unit* p_Vehicle)
        {
            UNUSED(p_Vehicle);
        }

        /// Called before _ExitVehicle with ExitPos parameter
        virtual void OnExitVehicle(Unit* p_Vehicle, Position& p_ExitPos)
        {
            UNUSED(p_Vehicle);
            UNUSED(p_ExitPos);
        }

        virtual void OnSpellClick(Unit* p_Clicker)
        {
            UNUSED(p_Clicker);
        }

        virtual bool CanSeeAlways(WorldObject const* p_Object)
        {
            UNUSED(p_Object);
            return false;
        }

        /// Called when the attack distance range is calculated
        virtual void OnCalculateAttackDistance(float& p_AttackDistance)
        {
            UNUSED(p_AttackDistance);
        }

    protected:
        virtual void MoveInLineOfSight(Unit* p_Who);

        bool _EnterEvadeMode();

    private:
        bool m_MoveInLineOfSight_locked;
        bool m_canSeeEvenInPassiveMode;
};

enum Permitions
{
    PERMIT_BASE_NO                 = -1,
    PERMIT_BASE_IDLE               = 1,
    PERMIT_BASE_REACTIVE           = 100,
    PERMIT_BASE_PROACTIVE          = 200,
    PERMIT_BASE_FACTION_SPECIFIC   = 400,
    PERMIT_BASE_SPECIAL            = 800
};

#endif
