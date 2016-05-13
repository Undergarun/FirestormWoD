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
#include "instance_bloodmaul.hpp"

namespace MS { namespace Instances { namespace Bloodmaul
{
    /// Gug'rokk <Bloodmaul Slave Traders> - 74790
    class boss_gugrokk : public CreatureScript
    {
        public:
            boss_gugrokk() : CreatureScript("boss_gugrokk") { }

            enum eSpells
            {
                /// Magma Eruption
                SpellMagmaEruptionDummy     = 150776,   ///< 3s cast on self
                SpellMagmaEruptionVisual    = 150777,   ///< Magma visual on 74967

                /// Flame Buffet - Only in Heroic
                SpellFlameBuffet            = 163802,

                /// Molten Blast
                SpellMoltenBlast            = 150677,
                SpellMoltenCore             = 150678,
                SpellMoltenBarrage          = 150682,

                /// Summon Unstable Slag
                SpellSummonUnstableSlag     = 150755    ///< 1.5s cast and summon 74927
            };

            enum eEvents
            {
                EventMagmaEruption = 1,
                EventFlameBuffet,   ///< Only in Heroic
                EventMoltenBlast,
                EventSummonUnstableSlag
            };

            enum eTalks
            {
                TalkIntro,
                TalkAggro,
                TalkMagmaEruption,
                TalkUnstableSlag,
                TalkSlay,
                TalkDeath
            };

            enum eCreatures
            {
                NpcMagmaEruption    = 74967,
                NpcUnstableSlag     = 74927
            };

            enum eEquips
            {
                MainEquipID = 107424
            };

            struct boss_gugrokkAI : public BossAI
            {
                boss_gugrokkAI(Creature* p_Creature) : BossAI(p_Creature, BossIds::BossGugrokk)
                {
                    m_Instance = p_Creature->GetInstanceScript();
                }

                InstanceScript* m_Instance;
                EventMap m_Events;

                void Reset()
                {
                    m_Events.Reset();

                    _Reset();

                    summons.DespawnAll();

                    if (m_Instance != nullptr)
                        m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    me->RemoveAllAreasTrigger();

                    SetEquipmentSlots(false, eEquips::MainEquipID);
                }

                void JustDied(Unit*)
                {
                    _JustDied();

                    summons.DespawnAll();

                    me->RemoveAllAreasTrigger();

                    if (m_Instance != nullptr)
                        m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    Talk(eTalks::TalkDeath);
                }

                void EnterCombat(Unit*)
                {
                    _EnterCombat();

                    if (m_Instance != nullptr)
                        m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

                    Talk(eTalks::TalkAggro);

                    if (IsHeroic())
                        m_Events.ScheduleEvent(eEvents::EventFlameBuffet, 6 * TimeConstants::IN_MILLISECONDS);

                    m_Events.ScheduleEvent(eEvents::EventMagmaEruption, 7 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventMoltenBlast, 1 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventSummonUnstableSlag, 20 * TimeConstants::IN_MILLISECONDS);
                }

                void KilledUnit(Unit* p_Killed)
                {
                    if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                        Talk(eTalks::TalkSlay);
                }

                void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo)
                {
                    if (p_Target == nullptr)
                        return;

                    switch (p_SpellInfo->Id)
                    {
                        case eSpells::SpellMoltenBlast:
                        {
                            me->CastSpell(me, eSpells::SpellMoltenCore, true);

                            if (Aura* l_MoltenCore = me->GetAura(eSpells::SpellMoltenCore))
                            {
                                if (l_MoltenCore->GetStackAmount() >= 3)
                                {
                                    me->CastSpell(me, eSpells::SpellMoltenBarrage, true);
                                    me->RemoveAura(eSpells::SpellMoltenCore);
                                    me->MonsterTextEmote(TrinityStrings::LangGugrokkWarnMoltenBarrage, 0, true);
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }

                void JustSummoned(Creature* p_Summon)
                {
                    BossAI::JustSummoned(p_Summon);

                    if (p_Summon->GetEntry() == eCreatures::NpcUnstableSlag && p_Summon->AI())
                    {
                        std::list<Creature*> l_SLGList;
                        me->GetCreatureListWithEntryInGrid(l_SLGList, MobEntries::SLGGenricMoPLargeAoI, 150.0f);

                        if (l_SLGList.empty())
                            return;

                        l_SLGList.sort(JadeCore::ObjectDistanceOrderPred(p_Summon, false));
                        p_Summon->AI()->SetGUID(l_SLGList.front()->GetGUID());
                    }
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
                        case eEvents::EventMagmaEruption:
                        {
                            Talk(eTalks::TalkMagmaEruption);
                            me->CastSpell(me, eSpells::SpellMagmaEruptionDummy, false);
                            m_Events.ScheduleEvent(eEvents::EventMagmaEruption, 28 * TimeConstants::IN_MILLISECONDS);

                            std::list<Player*> l_PlayerList;
                            me->GetPlayerListInGrid(l_PlayerList, 100.0f);

                            for (Player* l_Player : l_PlayerList)
                            {
                                Position l_Pos;
                                l_Player->GetPosition(&l_Pos);

                                if (Creature* l_MagmaEruption = me->SummonCreature(eCreatures::NpcMagmaEruption, l_Pos))
                                    l_MagmaEruption->CastSpell(l_MagmaEruption, eSpells::SpellMagmaEruptionVisual, true);
                            }

                            break;
                        }
                        case eEvents::EventFlameBuffet:
                            me->CastSpell(me, eSpells::SpellFlameBuffet, false);
                            m_Events.ScheduleEvent(eEvents::EventFlameBuffet, 12 * TimeConstants::IN_MILLISECONDS);
                            break;
                        case eEvents::EventMoltenBlast:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                                me->CastSpell(l_Target, eSpells::SpellMoltenBlast, false);
                            m_Events.ScheduleEvent(eEvents::EventMoltenBlast, 16 * TimeConstants::IN_MILLISECONDS);
                            break;
                        case eEvents::EventSummonUnstableSlag:
                            me->CastSpell(me, eSpells::SpellSummonUnstableSlag, false);
                            Talk(eTalks::TalkUnstableSlag);
                            m_Events.ScheduleEvent(eEvents::EventSummonUnstableSlag, 25 * TimeConstants::IN_MILLISECONDS);
                            break;
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new boss_gugrokkAI(p_Creature);
            }
    };

    /// Magma Eruption - 74967
    class npc_gugrokk_magma_eruption : public CreatureScript
    {
        public:
            npc_gugrokk_magma_eruption() : CreatureScript("npc_gugrokk_magma_eruption") { }

            struct npc_gugrokk_magma_eruptionAI : public ScriptedAI
            {
                npc_gugrokk_magma_eruptionAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                enum eSpells
                {
                    SpellMagmaEruptionAura  = 150783,
                    SpellPeriodicDamage     = 150784
                };

                enum eEvents
                {
                    EventCheckPlayers = 1,
                    EventDespawn,
                    EventActivate
                };

                EventMap m_Events;

                void Reset()
                {
                    m_Events.ScheduleEvent(eEvents::EventActivate, 3 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventDespawn, 2 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS);

                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                }

                void UpdateAI(uint32 const p_Diff)
                {
                    m_Events.Update(p_Diff);

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventCheckPlayers:
                        {
                            std::list<Player*> l_PlayerList;
                            me->GetPlayerListInGrid(l_PlayerList, 3.0f);

                            for (Player* l_Player : l_PlayerList)
                                me->CastSpell(l_Player, eSpells::SpellPeriodicDamage, true);

                            m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 1 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        case eEvents::EventDespawn:
                            me->DespawnOrUnsummon();
                            break;
                        case eEvents::EventActivate:
                            me->CastSpell(me, eSpells::SpellMagmaEruptionAura, true);
                            m_Events.ScheduleEvent(eEvents::EventCheckPlayers, 100);
                            break;
                        default:
                            break;
                    }
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_gugrokk_magma_eruptionAI(p_Creature);
            }
    };

    /// Unstable Slag - 74927
    class npc_gugrokk_unstable_slag : public CreatureScript
    {
        public:
            npc_gugrokk_unstable_slag() : CreatureScript("npc_gugrokk_unstable_slag") { }

            struct npc_gugrokk_unstable_slagAI : public ScriptedAI
            {
                npc_gugrokk_unstable_slagAI(Creature* p_Creature) : ScriptedAI(p_Creature)
                {
                    m_FollowingSLG = 0;
                }

                enum eSpells
                {
                    SpellEnslaveSlag        = 150756,   ///< Spawn visual
                    SpellSiphonFlames       = 150732,   ///< Fire channel to SLG
                    SpellEmpoweredFlames    = 152091
                };

                enum eEvents
                {
                    EventCheckSLG = 1
                };

                EventMap m_Events;

                uint64 m_FollowingSLG;

                void Reset()
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);

                    m_Events.ScheduleEvent(eEvents::EventCheckSLG, 1 * TimeConstants::IN_MILLISECONDS);

                    me->SetSpeed(UnitMoveType::MOVE_WALK, 0.7f);
                    me->SetSpeed(UnitMoveType::MOVE_RUN, 0.7f);
                }

                void SetGUID(uint64 p_GUID, int32 p_ID)
                {
                    m_FollowingSLG = p_GUID;

                    if (Creature* l_SLG = Creature::GetCreature(*me, m_FollowingSLG))
                    {
                        me->CastSpell(l_SLG, eSpells::SpellSiphonFlames, false);
                        me->GetMotionMaster()->MoveChase(l_SLG, 2.5f);
                    }
                }

                void UpdateAI(uint32 const p_Diff)
                {
                    m_Events.Update(p_Diff);

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventCheckSLG:
                        {
                            if (Creature* l_SLG = Creature::GetCreature(*me, m_FollowingSLG))
                            {
                                if (me->GetDistance(l_SLG) > 3.0f)
                                {
                                    me->CastSpell(l_SLG, eSpells::SpellSiphonFlames, false);
                                    me->GetMotionMaster()->MoveChase(l_SLG, 2.5f);
                                    m_Events.ScheduleEvent(eEvents::EventCheckSLG, 1 * TimeConstants::IN_MILLISECONDS);
                                    break;
                                }
                            }
                            else
                                break;

                            me->CastSpell(me, eSpells::SpellEmpoweredFlames, true);
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                            if (Unit* l_Target = me->FindNearestPlayer(60.0f))
                                AttackStart(l_Target);

                            m_FollowingSLG = 0;
                            break;
                        }
                        default:
                            break;
                    }

                    if (!m_FollowingSLG && me->GetReactState() == ReactStates::REACT_AGGRESSIVE)
                        DoMeleeAttackIfReady();
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_gugrokk_unstable_slagAI(p_Creature);
            }
    };
}
} } ///< namespace MS::Instances

void AddSC_boss_gugrokk()
{
    new MS::Instances::Bloodmaul::boss_gugrokk();
    new MS::Instances::Bloodmaul::npc_gugrokk_magma_eruption();
    new MS::Instances::Bloodmaul::npc_gugrokk_unstable_slag();
}