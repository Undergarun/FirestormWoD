////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Language.h"

/// Tarlna the Ageless - 81535
class boss_tarlna_the_ageless : public CreatureScript
{
    public:
        boss_tarlna_the_ageless() : CreatureScript("boss_tarlna_the_ageless") { }

        enum eDatas
        {
            BossTarlnaTheAgeless,
            MainHandEquipID         = 118056,
            MaxGiantLasherSpawns    = 15,
            HealthScalingCoeff      = 20
        };

        enum eSpells
        {
            SpellColossalBlow           = 175973,
            SpellSavageVines            = 176001,   ///< Trigger 176004 after 8s
            SpellGrowUntamedMandragora  = 176013,   ///< Missile 176014 on random destination
            SpellGenesisMissile         = 175975,   ///< Missile 175980 on random destination (summon Giant Lasher)
            SpellGenesis                = 175979    ///< Trigger 175978 after 6s -> SPELL_EFFECT_226 on TARGET_UNIT_SRC_AREA_ENTRY miscA = 422409
        };

        enum eEvents
        {
            EventColossalBlow = 1,
            EventSavageVines,
            EventGrowUntamedMandragora,
            EventGenesis
        };

        struct boss_tarlna_the_agelessAI : public BossAI
        {
            boss_tarlna_the_agelessAI(Creature* p_Creature) : BossAI(p_Creature, eDatas::BossTarlnaTheAgeless)
            {
                m_BaseHP = me->GetMaxHealth();
            }

            EventMap m_Events;

            bool m_FirstVictim;
            uint32 m_BaseHP;

            void Reset()
            {
                m_Events.Reset();

                _Reset();

                summons.DespawnAll();

                SetEquipmentSlots(false, eDatas::MainHandEquipID);

                m_FirstVictim = true;
                me->SetHealth(m_BaseHP);
            }

            void JustDied(Unit* p_Killer)
            {
                summons.DespawnAll();
            }

            void EnterCombat(Unit*)
            {
                m_Events.ScheduleEvent(eEvents::EventColossalBlow, 14 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSavageVines, 7 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventGrowUntamedMandragora, 18 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventGenesis, 20 * TimeConstants::IN_MILLISECONDS);
            }

            void OnHostileReferenceAdded(Unit* p_Ennemy)
            {
                if (p_Ennemy->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (m_FirstVictim)
                {
                    m_FirstVictim = false;
                    return;
                }

                float l_HealthPct = me->GetHealthPct();
                uint32 l_AddedValue = m_BaseHP / eDatas::HealthScalingCoeff;

                me->SetMaxHealth(me->GetMaxHealth() + l_AddedValue);
                me->SetHealth(CalculatePct(me->GetMaxHealth(), l_HealthPct));
            }

            void OnHostileReferenceRemoved(Unit* p_Ennemy)
            {
                if (p_Ennemy->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                float l_HealthPct = me->GetHealthPct();
                uint32 l_AddedValue = m_BaseHP / eDatas::HealthScalingCoeff;

                if ((me->GetMaxHealth() - l_AddedValue) < m_BaseHP)
                {
                    me->SetMaxHealth(m_BaseHP);
                    me->SetHealth(CalculatePct(m_BaseHP, l_HealthPct));
                    return;
                }

                me->SetMaxHealth(me->GetMaxHealth() - l_AddedValue);
                me->SetHealth(CalculatePct(me->GetMaxHealth(), l_HealthPct));
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventColossalBlow:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                        {
                            me->SetFacingTo(me->GetAngle(l_Target));
                            me->CastSpell(l_Target, eSpells::SpellColossalBlow, false);
                        }

                        m_Events.ScheduleEvent(eEvents::EventColossalBlow, 60 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSavageVines:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->SetFacingTo(me->GetAngle(l_Target));
                            me->CastSpell(l_Target, eSpells::SpellSavageVines, false);
                        }

                        m_Events.ScheduleEvent(eEvents::EventSavageVines, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventGrowUntamedMandragora:
                    {
                        me->CastSpell(me, eSpells::SpellGrowUntamedMandragora, true);
                        m_Events.ScheduleEvent(eEvents::EventGrowUntamedMandragora, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventGenesis:
                    {
                        me->MonsterTextEmote(TrinityStrings::LangWarnGenesis, 0, true);
                        SummonGiantLashers(eDatas::MaxGiantLasherSpawns);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->SetFacingTo(me->GetAngle(l_Target));
                            me->CastSpell(l_Target, eSpells::SpellGenesis, false);
                        }

                        m_Events.ScheduleEvent(eEvents::EventGenesis, 45 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void SummonGiantLashers(uint32 p_Count)
            {
                for (uint32 l_I = 0; l_I < p_Count; ++l_I)
                    me->CastSpell(me, eSpells::SpellGenesisMissile, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_tarlna_the_agelessAI(p_Creature);
        }
};

/// Untamed Mandragora - 88150
class npc_untamed_mandragora : public CreatureScript
{
    public:
        npc_untamed_mandragora() : CreatureScript("npc_untamed_mandragora") { }

        enum eSpells
        {
            SpellNoxiousSpitAT  = 176035
        };

        enum eEvents
        {
            EventNoxiousSpit = 1
        };

        struct npc_untamed_mandragoraAI : public CreatureAI
        {
            npc_untamed_mandragoraAI(Creature* p_Creature) : CreatureAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveAllAreasTrigger();
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(eEvents::EventNoxiousSpit, 8 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventNoxiousSpit:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellNoxiousSpitAT, true);
                        m_Events.ScheduleEvent(eEvents::EventNoxiousSpit, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_untamed_mandragoraAI(p_Creature);
        }
};

/// Giant Lasher - 88142
class npc_giant_lasher : public CreatureScript
{
    public:
        npc_giant_lasher() : CreatureScript("npc_giant_lasher") { }

        enum eSpells
        {
            SpellGenesisAura    = 175977,  ///< SPELL_AURA_395 -> AreaTrigger
            SpellGenesisAwake   = 175978,
            SpellSubmerged      = 175123,
            SpellLasherVenom    = 176000
        };

        struct npc_giant_lasherAI : public CreatureAI
        {
            npc_giant_lasherAI(Creature* p_Creature) : CreatureAI(p_Creature)
            {
                m_CheckPlayerTimer = 0;
            }

            uint32 m_CheckPlayerTimer;

            void Reset()
            {
                m_CheckPlayerTimer = 500;

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                me->CastSpell(me, eSpells::SpellGenesisAura, true);
                me->CastSpell(me, eSpells::SpellSubmerged, true);
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::SpellGenesisAwake)
                {
                    m_CheckPlayerTimer = 0;

                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAura(eSpells::SpellGenesisAura);
                    me->RemoveAura(eSpells::SpellSubmerged);
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (me->HasAura(eSpells::SpellGenesisAura))
                {
                    CheckNearPlayers(p_Diff);
                    return;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                /// No melee attack
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                    me->CastSpell(l_Target, eSpells::SpellLasherVenom, false);
            }

            void CheckNearPlayers(uint32 const p_Diff)
            {
                if (!m_CheckPlayerTimer)
                    return;

                if (m_CheckPlayerTimer <= p_Diff)
                {
                    if (Player* l_Player = me->SelectNearestPlayerNotGM(5.0f))
                        me->DespawnOrUnsummon();

                    m_CheckPlayerTimer = 500;
                }
                else
                    m_CheckPlayerTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_giant_lasherAI(p_Creature);
        }
};

/// Noxious Spit - 176035
class areatrigger_tarlna_noxious_spit : public AreaTriggerEntityScript
{
    public:
        areatrigger_tarlna_noxious_spit() : AreaTriggerEntityScript("areatrigger_tarlna_noxious_spit") { }

        enum Spells
        {
            SpellNoxiousSpitDot = 176037
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            if (p_AreaTrigger->GetCaster() == nullptr)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 4.0f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(p_AreaTrigger) > l_Radius)
                    continue;

                p_AreaTrigger->GetCaster()->CastSpell(l_Unit, Spells::SpellNoxiousSpitDot, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new areatrigger_tarlna_noxious_spit();
        }
};

void AddSC_gorgrond()
{
    new boss_tarlna_the_ageless();
    new npc_untamed_mandragora();
    new npc_giant_lasher();
    new areatrigger_tarlna_noxious_spit();
}