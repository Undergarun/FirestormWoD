////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Rylai Crestfall <Stormshield Tower Guardian> - 88224
class npc_rylai_crestfall : public CreatureScript
{
    public:
        npc_rylai_crestfall() : CreatureScript("npc_rylai_crestfall") { }

        struct npc_rylai_crestfallAI : public ScriptedAI
        {
            npc_rylai_crestfallAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CheckAroundingPlayersTimer = 0;
                m_FreezingFieldTimer = 0;
            }

            enum eSpells
            {
                Frostbolt       = 176268,
                FrostboltVolley = 176273,
                IceBlock        = 176269,
                Hypotermia      = 41425,
                MassPolymorph   = 176204,
                FrostNovaCasted = 176327,
                NorthrendWinds  = 176267,
                FrostNova       = 176276,
                DeepFreeze      = 176278,
                SummonIceShard  = 177599,   ///< @TODO

                TowerMageTargetingAura  = 176162,   ///< Put on ennemy players around 200 yards
                FreezingFieldSearcher   = 176163,   ///< Launch frost missile on one player targeted
                FreezingFieldMissile    = 176165
            };

            enum eTalk
            {
                TalkAggro,
                TalkSlay,
                TalkDeath,
                TalkSpell
            };

            enum eEvents
            {
                EventFrostbolt = 1,
                EventFrostboltVolley,
                EventMassPolymorph,
                EventFrostNova,
                EventNorthrendWinds
            };

            EventMap m_Events;

            uint32 m_CheckAroundingPlayersTimer;
            uint32 m_FreezingFieldTimer;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);

                m_CheckAroundingPlayersTimer = 2000;
                m_FreezingFieldTimer = 10000;
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventFrostbolt, 4000);
                m_Events.ScheduleEvent(eEvents::EventMassPolymorph, 6000);
                m_Events.ScheduleEvent(eEvents::EventFrostboltVolley, 9000);
                m_Events.ScheduleEvent(eEvents::EventFrostNova, 12500);
                m_Events.ScheduleEvent(eEvents::EventNorthrendWinds, 15000);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(eTalk::TalkDeath);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (me->HealthBelowPctDamaged(20, p_Damage) && !me->HasAura(eSpells::Hypotermia))
                {
                    me->CastSpell(me, eSpells::IceBlock, true);
                    me->CastSpell(me, eSpells::Hypotermia, true);
                }
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo)
            {
                if (p_Victim == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::FreezingFieldSearcher:
                        me->CastSpell(p_Victim, eSpells::FreezingFieldMissile, false);
                        break;
                    case eSpells::NorthrendWinds:
                        if (p_Victim->HasAura(eSpells::FrostNova))
                            me->CastSpell(p_Victim, eSpells::DeepFreeze, true);
                        else
                            me->CastSpell(p_Victim, eSpells::FrostNova, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                {
                    ScheduleTargetingPlayers(p_Diff);
                    ScheduleFreezingField(p_Diff);
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFrostbolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::Frostbolt, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostbolt, 10000);
                        break;
                    case eEvents::EventFrostboltVolley:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::FrostboltVolley, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostboltVolley, 20000);
                        break;
                    case eEvents::EventMassPolymorph:
                        me->CastSpell(me, eSpells::MassPolymorph, false);
                        m_Events.ScheduleEvent(eEvents::EventMassPolymorph, 25000);
                        break;
                    case eEvents::EventFrostNova:
                        me->CastSpell(me, eSpells::FrostNovaCasted, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostNova, 27500);
                        break;
                    case eEvents::EventNorthrendWinds:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::NorthrendWinds, false);
                        m_Events.ScheduleEvent(eEvents::EventNorthrendWinds, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ScheduleTargetingPlayers(uint32 const p_Diff)
            {
                if (!m_CheckAroundingPlayersTimer)
                    return;

                if (m_CheckAroundingPlayersTimer <= p_Diff)
                {
                    m_CheckAroundingPlayersTimer = 2500;

                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 200.0f);

                    l_PlayerList.remove_if([this](Player* p_Player) -> bool
                    {
                        if (p_Player == nullptr)
                            return true;

                        if (!me->IsValidAttackTarget(p_Player))
                            return true;

                        if (p_Player->HasAura(eSpells::TowerMageTargetingAura))
                            return true;

                        return false;
                    });

                    for (Player* l_Player : l_PlayerList)
                        l_Player->CastSpell(l_Player, eSpells::TowerMageTargetingAura, true, nullptr, nullptr, me->GetGUID());
                }
                else
                    m_CheckAroundingPlayersTimer -= p_Diff;
            }

            void ScheduleFreezingField(uint32 const p_Diff)
            {
                if (!m_FreezingFieldTimer)
                    return;

                if (m_FreezingFieldTimer <= p_Diff)
                {
                    if (!me->isInCombat())
                        me->CastSpell(me, eSpells::FreezingFieldSearcher, true);
                    m_FreezingFieldTimer = 10000;
                }
                else
                    m_FreezingFieldTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_rylai_crestfallAI(p_Creature);
        }
};

void AddSC_AshranNPCAlliance()
{
    new npc_rylai_crestfall();
}