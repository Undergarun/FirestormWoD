/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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
    SELECT_TARGET_ANY_FRIEND,                               //AoE or Single Friend
};

//Spell Effects used by SelectSpell
enum SelectEffect
{
    SELECT_EFFECT_DONTCARE = 0,                             //All spell effects allowed
    SELECT_EFFECT_DAMAGE,                                   //Spell does damage
    SELECT_EFFECT_HEALING,                                  //Spell does healing
    SELECT_EFFECT_AURA,                                     //Spell applies an aura
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
        virtual void EnterCombat(Unit* /*victim*/) {}

        // Called when the creature is despawned
        virtual void JustDespawned() {}

        // Called when the creature is killed
        virtual void JustDied(Unit* /*killer*/) {}

        // Called when the creature kills a unit
        virtual void KilledUnit(Unit* /*victim*/) {}

        // Called when the killer is rewarded with currencies
        virtual void CurrenciesRewarder(bool& result) { } ///< result is unused

        /// Called when check interact distance (for example: gossips)
        virtual bool CanByPassDistanceCheck() const { return false; }

        // Called when check LOS
        virtual bool CanBeTargetedOutOfLOS() { return false; }
        virtual bool CanTargetOutOfLOS() { return false; }

        // Called when the creature summon successfully other creature
        virtual void JustSummoned(Creature* /*summon*/) {}
        virtual void IsSummonedBy(Unit* /*summoner*/) {}

        /// Called when the creature summons successfully AreaTriggers
        virtual void AreaTriggerCreated(AreaTrigger* p_AreaTrigger) { } ///< p_AreaTrigger is unused

        /// Called when an AreaTrigger summoned by the creature despawns
        virtual void AreaTriggerDespawned(AreaTrigger* p_AreaTrigger) { } ///< p_AreaTrigger is unused

        virtual void SummonedCreatureDespawn(Creature* /*summon*/) {}
        virtual void SummonedCreatureDies(Creature* /*summon*/, Unit* /*killer*/) {}

        // Called when hit by a spell
        virtual void SpellHit(Unit* /*caster*/, SpellInfo const* /*spell*/) {}

        // Called when spell hits a target
        virtual void SpellHitTarget(Unit* /*target*/, SpellInfo const* /*spell*/) {}

        /// Called when spell hits a destination
        virtual void SpellHitDest(SpellDestination const* p_Dest, SpellInfo const* p_SpellInfo) { } ///< p_Dest & p_SpellInfo is unused

        /// Called when spell miss a target
        virtual void SpellMissTarget(Unit* p_Target, SpellInfo const* p_SpellInfo, SpellMissInfo p_MissInfo) { } ///< p_Target & p_SpellInfo & p_MissInfo is unused

        /// Called when successful cast a spell
        virtual void OnSpellCasted(SpellInfo const* p_SpellInfo) { } ///< p_SpellInfo is unused

        /// Called when a spell is finished
        virtual void OnSpellFinished(SpellInfo const* p_SpellInfo) { } ///< p_SpellInfo is unused

        // Called when the creature is target of hostile action: swing, hostile spell landed, fear/etc)
        //virtual void AttackedBy(Unit* attacker);
        virtual bool IsEscorted() { return false; }
        virtual bool IsPassived() { return false; }

        // Called when creature is spawned or respawned (for reseting variables)
        virtual void JustRespawned() { Reset(); }

        // Called at waypoint reached or point movement finished
        virtual void MovementInform(uint32 /*type*/, uint32 id) {} ///< id is unused

        void OnCharmed(bool apply);

        // Called at reaching home after evade
        virtual void JustReachedHome() {}

        void DoZoneInCombat(Creature* creature = NULL, float maxRangeToNearestTarget = 50.0f);
        void DoAttackerAreaInCombat(Unit* attacker, float range, Unit* pUnit = NULL);

        // Called at text emote receive from player
        virtual void ReceiveEmote(Player* /*player*/, uint32 /*emoteId*/) {}

        // Called when owner takes damage
        virtual void OwnerDamagedBy(Unit* /*attacker*/) {}

        // Called when owner attacks something
        virtual void OwnerAttacked(Unit* /*target*/) {}

        // Called when a creature regen one of his power
        virtual void RegeneratePower(Powers power, int32& value) {} ///< power & value is unused

        // Called when a creature has one of hi power modified
        virtual void PowerModified(Powers p_Power, int32 p_Value) {} ///< power & value is unused

        // Called when a creature has one of his power set
        virtual void SetPower(Powers power, int32 value) {}

        /// Called when taunted
        virtual void OnTaunt(Unit* p_Taunter) {} ///< p_Taunter is unused

        /// Called when a casting time is calculated
        virtual void OnCalculateCastingTime(SpellInfo const* p_SpellInfo, int32& p_CastingTime) {}

        // Called at any threat added from any attacker (before threat apply)
        virtual void OnAddThreat(Unit* /*victim*/, float& /*fThreat*/, SpellSchoolMask /*schoolMask*/, SpellInfo const* /*threatSpell*/) {}

        /// Called when at HandleGarrisonGetShipmentInfoOpcode() is received
        virtual int OnShipmentIDRequest(Player* p_Player) { return -1; } ///< p_Player is unused

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
        virtual void CorpseRemoved(uint32& /*respawnDelay*/) {}

        // Called when victim entered water and creature can not enter water
        //virtual bool canReachByRangeAttack(Unit*) { return false; }

        /// == Fields =======================================

        // Pointer to controlled by AI creature
        //Creature* const me;

        virtual void PassengerBoarded(Unit* /*passenger*/, int8 /*seatId*/, bool /*apply*/) {}

        virtual void OnVehicleExited(Unit* p_Vehicle) { } ///< p_Vehicle is unused

        /// Called before _ExitVehicle with ExitPos parameter
        virtual void OnExitVehicle(Unit* p_Vehicle, Position& p_ExitPos) { }

        virtual void OnSpellClick(Unit* /*clicker*/) { }

        virtual bool CanSeeAlways(WorldObject const* /*obj*/) { return false; }

        /// Called when the attack distance range is calculated
        virtual void OnCalculateAttackDistance(float& p_AttackDistance) { }

    protected:
        virtual void MoveInLineOfSight(Unit* /*who*/);

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
