////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_UNITAI_H
#define TRINITY_UNITAI_H

#include "Define.h"
#include "Unit.h"
#include "Containers.h"
#include "Common.h"

class Unit;
class Player;
struct AISpellInfoType;

// Default script texts
enum GeneralScriptTexts
{
    DEFAULT_TEXT                = -1000000,
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,
    EMOTE_GENERIC_FRENZY        = -1000002,
    EMOTE_GENERIC_ENRAGED       = -1000003,
    EMOTE_GENERIC_BERSERK       = -1000004,
    EMOTE_GENERIC_BERSERK_RAID  = -1000005 // RaidBossEmote version of the previous one
};

//Selection method used by SelectTarget
enum SelectAggroTarget
{
    SELECT_TARGET_RANDOM = 0,                               //Just selects a random target
    SELECT_TARGET_TOPAGGRO,                                 //Selects targes from top aggro to bottom
    SELECT_TARGET_BOTTOMAGGRO,                              //Selects targets from bottom aggro to top
    SELECT_TARGET_NEAREST,
    SELECT_TARGET_FARTHEST
};

// default predicate function to select target based on distance, player and/or aura criteria
struct DefaultTargetSelector : public std::unary_function<Unit*, bool>
{
    const Unit* me;
    float m_dist;
    bool m_playerOnly;
    int32 m_aura;

    // unit: the reference unit
    // dist: if 0: ignored, if > 0: maximum distance to the reference unit, if < 0: minimum distance to the reference unit
    // playerOnly: self explaining
    // aura: if 0: ignored, if > 0: the target shall have the aura, if < 0, the target shall NOT have the aura
    DefaultTargetSelector(Unit const* unit, float dist, bool playerOnly, int32 aura) : me(unit), m_dist(dist), m_playerOnly(playerOnly), m_aura(aura) {}

    bool operator()(Unit const* target) const
    {
        if (!me)
            return false;

        if (!target)
            return false;

        if (m_playerOnly && (target->GetTypeId() != TYPEID_PLAYER))
            return false;

        if (m_dist > 0.0f && !me->IsWithinCombatRange(target, m_dist))
            return false;

        if (m_dist < 0.0f && me->IsWithinCombatRange(target, -m_dist))
            return false;

        if (m_aura)
        {
            if (m_aura > 0)
            {
                if (!target->HasAura(m_aura))
                    return false;
            }
            else
            {
                if (target->HasAura(-m_aura))
                    return false;
            }
        }

        return true;
    }
};

// Target selector for spell casts checking range, auras and attributes
// TODO: Add more checks from Spell::CheckCast
struct SpellTargetSelector : public std::unary_function<Unit*, bool>
{
    public:
        SpellTargetSelector(Unit* caster, uint32 spellId);
        bool operator()(Unit const* target) const;

    private:
        Unit const* _caster;
        SpellInfo const* _spellInfo;
};

// Very simple target selector, will just skip main target
// NOTE: When passing to UnitAI::SelectTarget remember to use 0 as position for random selection
//       because tank will not be in the temporary list
struct NonTankTargetSelector : public std::unary_function<Unit*, bool>
{
    public:
        NonTankTargetSelector(Creature* source, bool playerOnly = true) : _source(source), _playerOnly(playerOnly) { }
        bool operator()(Unit const* target) const;

    private:
        Creature const* _source;
        bool _playerOnly;
};

class UnitAI
{
    protected:
        Unit* const me;
    public:
        explicit UnitAI(Unit* unit) : me(unit) {}
        virtual ~UnitAI() {}

        virtual bool CanAIAttack(Unit const* p_Target) const
        {
            UNUSED(p_Target);
            return true;
        }
        virtual void AttackStart(Unit* target);
        virtual void UpdateAI(uint32 const p_Diff) = 0;

        virtual void InitializeAI() { if (!me->isDead()) Reset(); }

        virtual void Reset() {}

        // Called when unit is charmed
        virtual void OnCharmed(bool apply) = 0;

        // Pass parameters between AI
        virtual void DoAction(int32 const p_Action)
        {
            UNUSED(p_Action);
        }

        virtual uint32 GetData(uint32 p_ID = 0)
        {
            UNUSED(p_ID);
            return 0;
        }

        virtual float GetFData(uint32 p_ID = 0) const
        {
            UNUSED(p_ID);
            return 0.0f;
        }

        virtual void SetData(uint32 p_ID, uint32 p_Value)
        {
            UNUSED(p_ID);
            UNUSED(p_Value);
        }

        virtual void SetFData(uint32 p_ID, float p_Value)
        {
            UNUSED(p_ID);
            UNUSED(p_Value);
        }

        virtual void SetGUID(uint64 p_Guid, int32 p_ID = 0)
        {
            UNUSED(p_Guid);
            UNUSED(p_ID);
        }

        virtual void AddHitQueue(uint32* p_Data, int32 p_ID = 0)
        {
            UNUSED(p_Data);
            UNUSED(p_ID);
        }

        virtual void DropCharge() {}

        virtual uint64 GetGUID(int32 p_ID = 0)
        {
            UNUSED(p_ID);
            return 0;
        }

        virtual void SetDestTarget(WorldLocation const* p_Dest)
        {
            UNUSED(p_Dest);
        }

        Unit* SelectTarget(SelectAggroTarget targetType, uint32 position = 0, float dist = 0.0f, bool playerOnly = false, int32 aura = 0);
        // Select the targets satisfying the predicate.
        // predicate shall extend std::unary_function<Unit*, bool>
        template <class PREDICATE> Unit* SelectTarget(SelectAggroTarget targetType, uint32 position, PREDICATE const& predicate)
        {
            const std::list<HostileReference*>& threatlist = me->getThreatManager().getThreatList();
            if (position >= threatlist.size())
                return NULL;

            std::list<Unit*> targetList;
            for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                if (predicate((*itr)->getTarget()))
                    targetList.push_back((*itr)->getTarget());

            if (position >= targetList.size())
                return NULL;

            if (targetType == SELECT_TARGET_NEAREST || targetType == SELECT_TARGET_FARTHEST)
                targetList.sort(JadeCore::ObjectDistanceOrderPred(me));

            switch (targetType)
            {
                case SELECT_TARGET_NEAREST:
                case SELECT_TARGET_TOPAGGRO:
                {
                    std::list<Unit*>::iterator itr = targetList.begin();
                    std::advance(itr, position);
                    return *itr;
                }
                case SELECT_TARGET_FARTHEST:
                case SELECT_TARGET_BOTTOMAGGRO:
                {
                    std::list<Unit*>::reverse_iterator ritr = targetList.rbegin();
                    std::advance(ritr, position);
                    return *ritr;
                }
                case SELECT_TARGET_RANDOM:
                {
                    std::list<Unit*>::iterator itr = targetList.begin();
                    std::advance(itr, urand(position, targetList.size() - 1));
                    return *itr;
                }
                default:
                    break;
            }

            return NULL;
        }

        void SelectTargetList(std::list<Unit*>& targetList, uint32 num, SelectAggroTarget targetType, float dist = 0.0f, bool playerOnly = false, int32 aura = 0);

        // Select the targets satifying the predicate.
        // predicate shall extend std::unary_function<Unit*, bool>
        template <class PREDICATE> void SelectTargetList(std::list<Unit*>& targetList, PREDICATE const& predicate, uint32 maxTargets, SelectAggroTarget targetType)
        {
            std::list<HostileReference*> const& threatlist = me->getThreatManager().getThreatList();
            if (threatlist.empty())
                return;

            for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                if (predicate((*itr)->getTarget()))
                    targetList.push_back((*itr)->getTarget());

            if (targetList.size() < maxTargets)
                return;

            if (targetType == SELECT_TARGET_NEAREST || targetType == SELECT_TARGET_FARTHEST)
                targetList.sort(JadeCore::ObjectDistanceOrderPred(me));

            if (targetType == SELECT_TARGET_FARTHEST || targetType == SELECT_TARGET_BOTTOMAGGRO)
                targetList.reverse();

            if (targetType == SELECT_TARGET_RANDOM)
                JadeCore::Containers::RandomResizeList(targetList, maxTargets);
            else
                targetList.resize(maxTargets);
        }

        /// In mostly cases, heals will be targeted too
        Player* SelectRangedTarget(bool p_AllowHeal = true, int32 p_CheckAura = 0) const;
        /// In mostly cases, tanks will not be targeted
        Player* SelectMeleeTarget(bool p_AllowTank = false) const;

        // Called at any Damage to any victim (before damage apply)
        virtual void DamageDealt(Unit* p_Victim, uint32& p_Damage, DamageEffectType p_DamageType)
        {
            UNUSED(p_Victim);
            UNUSED(p_Damage);
            UNUSED(p_DamageType);
        }

        // Called at any Damage from any attacker (before damage apply)
        // Note: it for recalculation damage or special reaction at damage
        // for attack reaction use AttackedBy called for not DOT damage in Unit::DealDamage also
        virtual void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
        {
            UNUSED(p_Attacker);
            UNUSED(p_Damage);
            UNUSED(p_SpellInfo);
        }

        // Called when we calculate hit result of a spell or a melee attack
        // Note: it allows to put some conditions to change the result of melee/spell attacks on the Unit
        virtual void CheckHitResult(MeleeHitOutcome& p_MeleeResult, SpellMissInfo& p_SpellResult, Unit* p_Attacker, SpellInfo const* const p_SpellInfo = nullptr)
        {
            UNUSED(p_MeleeResult);
            UNUSED(p_SpellResult);
            UNUSED(p_Attacker);
            UNUSED(p_SpellInfo);
        }

        /// Called when Unit::BuildValuesUpdate is called
        /// Used for send differents factions for players for a same unit
        virtual void OnSendFactionTemplate(uint32& p_FactionID, Player* p_Target)
        {
            UNUSED(p_FactionID);
            UNUSED(p_Target);
        }

        // Called when the creature receives heal
        virtual void HealReceived(Unit* p_DoneBy, uint32& p_AddHealth)
        {
            UNUSED(p_DoneBy);
            UNUSED(p_AddHealth);
        }

        // Called when the unit heals
        virtual void HealDone(Unit* p_DoneTo, uint32& p_AddHealth)
        {
            UNUSED(p_DoneTo);
            UNUSED(p_AddHealth);
        }

        /// Called when a spell is interrupted by Spell::EffectInterruptCast
        /// Use to reschedule next planned cast of spell.
        virtual void SpellInterrupted(uint32 p_SpellID, uint32 p_UnTimeMS)
        {
            UNUSED(p_SpellID);
            UNUSED(p_UnTimeMS);
        }

        void AttackStartCaster(Unit* victim, float dist);

        void DoAddAuraToAllHostilePlayers(uint32 spellid);
        void DoCast(uint32 spellId);
        void DoCast(Unit* victim, uint32 spellId, bool triggered = false);
        void DoCastToAllHostilePlayers(uint32 spellid, bool triggered = false);
        void DoCastVictim(uint32 spellId, bool triggered = false);
        void DoCastAOE(uint32 spellId, bool triggered = false);

        float DoGetSpellMaxRange(uint32 spellId, bool positive = false);

        void DoMeleeAttackIfReady();
        bool DoSpellAttackIfReady(uint32 spell);

        static AISpellInfoType* AISpellInfo;
        static void FillAISpellInfo();

        virtual void sGossipHello(Player* p_Player)
        {
            UNUSED(p_Player);
        }
        virtual void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action)
        {
            UNUSED(p_Player);
            UNUSED(p_Sender);
            UNUSED(p_Action);
        }
        virtual void sGossipSelectCode(Player* p_Player, uint32 p_Sender, uint32 p_Action, char const* p_Code)
        {
            UNUSED(p_Player);
            UNUSED(p_Sender);
            UNUSED(p_Action);
            UNUSED(p_Code);
        }
        virtual void sQuestAccept(Player* p_Player, Quest const* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }
        virtual void sQuestSelect(Player* p_Player, Quest const* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }
        virtual void sQuestComplete(Player* p_Player, Quest const* p_Quest)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
        }
        virtual void sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 p_Option)
        {
            UNUSED(p_Player);
            UNUSED(p_Quest);
            UNUSED(p_Option);
        }
        virtual bool sOnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffIndex)
        {
            UNUSED(p_Caster);
            UNUSED(p_SpellID);
            UNUSED(p_EffIndex);
            return false;
        }
        virtual void sOnGameEvent(bool p_Start, uint16 p_EventID)
        {
            UNUSED(p_Start);
            UNUSED(p_EventID);
        }
};

class PlayerAI : public UnitAI
{
    protected:
        Player* const me;
    public:
        explicit PlayerAI(Player* player) : UnitAI((Unit*)player), me(player) {}

        void OnCharmed(bool apply);
};

class SimpleCharmedAI : public PlayerAI
{
    public:
        void UpdateAI(uint32 const diff);
        SimpleCharmedAI(Player* player): PlayerAI(player) {}
};

#endif
