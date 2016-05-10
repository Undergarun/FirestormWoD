////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
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
    static Position const s_FieryBoulderPos1 = { 2298.648f, -218.2934f, 253.099f, 3.068156f };
    static Position const s_FieryBoulderPos2 = { 2297.642f, -211.8681f, 253.099f, 3.068156f };
    static Position const s_FieryBoulderPos3 = { 2297.642f, -204.1632f, 253.099f, 3.068156f };

    static float const s_PositionZ = 213.29f;

    static Position const s_HeatWavePos = { 2312.793f, -211.3021f, 217.0487f, 3.178043f };

    static float const s_MaxPositionX = 2304.24f;
    static float const s_MinPositionX = 2244.11f;
    static float const s_MaxPositionY = -196.27f;
    static float const s_MinPositionY = -226.51f;

    /// Roltall - 75786
    class boss_roltall : public CreatureScript
    {
        public:
            boss_roltall() : CreatureScript("boss_roltall") { }

            enum eSpells
            {
                SpellScorchingAura          = 167738,   ///< Only for Heroic mode

                /// Burning Slag
                SpellBurningSlagDummy       = 152939,   ///< 0.5s cast -> Must trigger 152918 on random player
                SpellBurningSlagMissile     = 152918,   ///< Trigger 152913 on player location -> Create AreaTrigger

                /// Heatwave
                SpellHeatWaveAuraDummy      = 152940,   ///< 1.5s cast -> 8s duration
                SpellHeatWaveAreaTrigger    = 152867,
                SpellHeatWavePeriodicDamage = 152897,
                SpellHeatWaveVisual         = 152896,

                /// Fiery Boulder
                SpellFieryBoulderCasted     = 153247,
                SpellFieryBoulderRockVisual = 163733,
                SpellFieryBoulderFireVisual = 152850,
                SpellFieryBoulderEarthquake = 153058    ///< Makes the screen shaking !
            };

            enum eEvents
            {
                EventHeatWave = 1,
                EventHeatWaveEnd,
                EventFieryBoulder,
                EventBurningSlag
            };

            enum eDatas
            {
                MaxFieryBoulder = 3,
                MaxBurningSlag  = 6,

                NoBoulderSpawn  = 0x00,
                Boulder1Spawned = 0x01,
                Boulder2Spawned = 0x02,
                Boulder3Spawned = 0x04,
                AllBoulderSpawn = 0x07
            };

            struct boss_roltallAI : public BossAI
            {
                boss_roltallAI(Creature* p_Creature) : BossAI(p_Creature, BossIds::BossRoltall)
                {
                    m_Instance = p_Creature->GetInstanceScript();
                }

                InstanceScript* m_Instance;
                EventMap m_Events;

                uint8 m_FieryBoulderCount;
                uint8 m_FieryBoulderFlags;

                uint8 m_BurningSlagCount;

                void Reset()
                {
                    m_Events.Reset();

                    _Reset();

                    /// -100% casting speed of players in melee range
                    if (!IsHeroic())
                        me->RemoveAura(eSpells::SpellScorchingAura);
                    else
                        me->AddAura(eSpells::SpellScorchingAura, me);

                    if (m_Instance != nullptr)
                        m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_FieryBoulderCount = 0;
                    m_FieryBoulderFlags = eDatas::NoBoulderSpawn;
                    m_BurningSlagCount  = eDatas::MaxBurningSlag;

                    me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->RemoveAllAreasTrigger();
                }

                void JustDied(Unit*)
                {
                    _JustDied();

                    me->RemoveAllAreasTrigger();

                    if (m_Instance != nullptr)
                        m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }

                void EnterCombat(Unit*)
                {
                    _EnterCombat();

                    m_Events.ScheduleEvent(eEvents::EventHeatWave, 20 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventFieryBoulder, 7 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventBurningSlag, 28 * TimeConstants::IN_MILLISECONDS);

                    if (m_Instance != nullptr)
                        m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                }

                void SpellHit(Unit*, SpellInfo const* p_SpellInfo)
                {
                    switch (p_SpellInfo->Id)
                    {
                        case eSpells::SpellFieryBoulderCasted:
                        {
                            if (m_FieryBoulderFlags >= eDatas::AllBoulderSpawn)
                                break;

                            Creature* l_FieryBoulder = nullptr;

                            do
                            {
                                uint8 l_Boulder = urand(0, 2);
                                uint32 l_BoulderEntry = 0;

                                if ((1 << l_Boulder) & eDatas::Boulder1Spawned && !(m_FieryBoulderFlags & eDatas::Boulder1Spawned))
                                {
                                    m_FieryBoulderFlags |= eDatas::Boulder1Spawned;
                                    l_BoulderEntry = MobEntries::FieryBoulder1;

                                    l_FieryBoulder = me->SummonCreature(l_BoulderEntry, s_FieryBoulderPos1);
                                }
                                else if ((1 << l_Boulder) & eDatas::Boulder2Spawned && !(m_FieryBoulderFlags & eDatas::Boulder2Spawned))
                                {
                                    m_FieryBoulderFlags |= eDatas::Boulder2Spawned;
                                    l_BoulderEntry = MobEntries::FieryBoulder2;

                                    l_FieryBoulder = me->SummonCreature(l_BoulderEntry, s_FieryBoulderPos2);
                                }
                                else if ((1 << l_Boulder) & eDatas::Boulder3Spawned && !(m_FieryBoulderFlags & eDatas::Boulder3Spawned))
                                {
                                    m_FieryBoulderFlags |= eDatas::Boulder3Spawned;
                                    l_BoulderEntry = MobEntries::FieryBoulder3;

                                    l_FieryBoulder = me->SummonCreature(l_BoulderEntry, s_FieryBoulderPos3);
                                }
                            }
                            while (l_FieryBoulder == nullptr);  ///< Is this safe ?

                            if (l_FieryBoulder != nullptr)
                            {
                                l_FieryBoulder->CastSpell(l_FieryBoulder, eSpells::SpellFieryBoulderRockVisual, true);
                                l_FieryBoulder->CastSpell(l_FieryBoulder, eSpells::SpellFieryBoulderFireVisual, true);
                                l_FieryBoulder->CastSpell(l_FieryBoulder, eSpells::SpellFieryBoulderEarthquake, true);
                            }
                            break;
                        }
                        case eSpells::SpellHeatWaveAuraDummy:
                        {
                            me->CastSpell(s_HeatWavePos.m_positionX, s_HeatWavePos.m_positionY, s_HeatWavePos.m_positionZ, eSpells::SpellHeatWaveAreaTrigger, true);
                            me->CastSpell(me, eSpells::SpellHeatWaveVisual, true);  ///< Heat wind visual
                            me->SetFacingTo(me->GetHomePosition().m_orientation);
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                            m_Events.ScheduleEvent(eEvents::EventHeatWaveEnd, 8 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        case eSpells::SpellBurningSlagDummy:
                        {
                            G3D::Vector3 l_SpawnPos = GetBurningSlagSpawnPos();
                            me->CastSpell(l_SpawnPos.x, l_SpawnPos.y, l_SpawnPos.z, eSpells::SpellBurningSlagMissile, true);
                            --m_BurningSlagCount;
                            break;
                        }
                        default:
                            break;
                    }
                }

                void EnterEvadeMode()
                {
                    /// Don't need classic EvadeMode, Roltall can't move
                    _EnterEvadeMode();
                    Reset();
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
                        case eEvents::EventHeatWave:
                            me->CastSpell(me, eSpells::SpellHeatWaveAuraDummy, false);
                            m_Events.ScheduleEvent(eEvents::EventHeatWave, 40 * TimeConstants::IN_MILLISECONDS);
                            break;
                        case eEvents::EventHeatWaveEnd:
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                            break;
                        case eEvents::EventFieryBoulder:
                        {
                            if (!m_FieryBoulderCount)
                                m_FieryBoulderFlags = eDatas::NoBoulderSpawn;

                            me->CastSpell(me, eSpells::SpellFieryBoulderCasted, false);
                            ++m_FieryBoulderCount;

                            if (m_FieryBoulderCount < eDatas::MaxFieryBoulder)
                                m_Events.ScheduleEvent(eEvents::EventFieryBoulder, 100);
                            else
                            {
                                m_FieryBoulderCount = 0;
                                m_Events.ScheduleEvent(eEvents::EventFieryBoulder, 30 * TimeConstants::IN_MILLISECONDS);
                            }

                            break;
                        }
                        case eEvents::EventBurningSlag:
                        {
                            if (!m_BurningSlagCount)
                            {
                                m_BurningSlagCount = eDatas::MaxBurningSlag;
                                m_Events.ScheduleEvent(eEvents::EventBurningSlag, 28 * TimeConstants::IN_MILLISECONDS);
                            }
                            else
                            {
                                me->CastSpell(me, eSpells::SpellBurningSlagDummy, false);
                                m_Events.ScheduleEvent(eEvents::EventBurningSlag, 550);
                            }
                            break;
                        }
                        default:
                            break;
                    }

                    DoMeleeAttackIfReady();
                }

                G3D::Vector3 GetBurningSlagSpawnPos()
                {
                    float l_PosX = frand(s_MinPositionX, s_MaxPositionX);
                    float l_PosY = frand(s_MinPositionY, s_MaxPositionY);
                    return G3D::Vector3(l_PosX, l_PosY, s_PositionZ);
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new boss_roltallAI(p_Creature);
            }
    };

    /// FieryBoulder - 75853, 75854, 75828
    class npc_fiery_boulder : public CreatureScript
    {
        public:
            npc_fiery_boulder() : CreatureScript("npc_fiery_boulder") { }

            enum eSpells
            {
                SpellFieryBoulderDamage = 152742,   ///< Damage impact
                SpellFieryBoulderStun   = 152843,   ///< And deal damage on the trajectory
                SpellAlphaFadeOut       = 141608
            };

            enum eEvents
            {
                EventDealDamage = 1,
                EventMovePoint
            };

            enum eMoves
            {
                FirstMove = 1,
                SecondMove  ///< Only in heroic
            };

            struct npc_fiery_boulderAI : public ScriptedAI
            {
                npc_fiery_boulderAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

                EventMap m_Events;

                void Reset()
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                    m_Events.ScheduleEvent(eEvents::EventMovePoint, 2 * TimeConstants::IN_MILLISECONDS);
                }

                void MovementInform(uint32 p_Type, uint32 p_ID)
                {
                    if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                        return;

                    bool l_Despawn = false;

                    if (IsHeroic())
                    {
                        if (p_ID == eMoves::FirstMove)  ///< Only in Heroic
                        {
                            float l_X = me->GetPositionX() + ((65.0f) * cos(me->GetOrientation() + M_PI));
                            float l_Y = me->GetPositionY() + ((65.0f) * sin(me->GetOrientation() + M_PI));

                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(eMoves::SecondMove, l_X, l_Y, s_PositionZ);
                        }
                        else if (p_ID == eMoves::SecondMove)
                            l_Despawn = true;
                    }
                    else if (p_ID == eMoves::FirstMove)
                        l_Despawn = true;

                    if (l_Despawn)
                    {
                        me->CastSpell(me, eSpells::SpellAlphaFadeOut, true);    ///< This makes ModelID disappear
                        me->DespawnOrUnsummon(200);
                        m_Events.CancelEvent(eEvents::EventDealDamage);
                    }
                }

                void UpdateAI(uint32 const p_Diff)
                {
                    m_Events.Update(p_Diff);

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventDealDamage:
                        {
                            std::list<Player*> l_PlayerList;
                            me->GetPlayerListInGrid(l_PlayerList, 5.0f);

                            for (Player* l_Player : l_PlayerList)
                                me->CastSpell(l_Player, eSpells::SpellFieryBoulderStun, true);

                            m_Events.ScheduleEvent(eEvents::EventDealDamage, 500);
                            break;
                        }
                        case eEvents::EventMovePoint:
                        {
                            me->m_positionZ = s_PositionZ;
                            me->CastSpell(me, eSpells::SpellFieryBoulderDamage, true);

                            float l_X = me->GetPositionX() + ((55.0f) * cos(me->GetOrientation()));
                            float l_Y = me->GetPositionY() + ((55.0f) * sin(me->GetOrientation()));

                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(eMoves::FirstMove, l_X, l_Y, s_PositionZ);
                            m_Events.ScheduleEvent(eEvents::EventDealDamage, 500);
                            break;
                        }
                        default:
                            break;
                    }
                }
            };

            CreatureAI* GetAI(Creature* p_Creature) const
            {
                return new npc_fiery_boulderAI(p_Creature);
            }
    };

    /// Scorching Aura - 167738
    class spell_roltall_scorching_aura : public SpellScriptLoader
    {
        public:
            spell_roltall_scorching_aura() : SpellScriptLoader("spell_roltall_scorching_aura") { }

            class spell_roltall_scorching_aura_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_roltall_scorching_aura_AuraScript);

                enum eSpells
                {
                    ScorchingAuraDebuff = 167739
                };

                void OnUpdate(uint32)
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        std::list<Unit*> l_TargetList;
                        float l_Radius = 15.0f;

                        JadeCore::NearestAttackableUnitInObjectRangeCheck u_check(l_Caster, l_Caster, l_Radius);
                        JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> searcher(l_Caster, l_TargetList, u_check);
                        l_Caster->VisitNearbyObject(l_Radius, searcher);

                        for (Unit* l_Unit : l_TargetList)
                            l_Caster->AddAura(eSpells::ScorchingAuraDebuff, l_Unit);
                    }
                }

                void Register()
                {
                    OnAuraUpdate += AuraUpdateFn(spell_roltall_scorching_aura_AuraScript::OnUpdate);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_roltall_scorching_aura_AuraScript();
            }
    };

    /// Heat Wave - 152867
    class areatrigger_roltall_heat_wave : public AreaTriggerEntityScript
    {
        public:
            areatrigger_roltall_heat_wave() : AreaTriggerEntityScript("areatrigger_roltall_heat_wave") { }

            enum eSpells
            {
                SpellPeriodicDamage = 152897
            };

            void OnUpdate(AreaTrigger* p_AreaTrigger, uint32)
            {
                if (Unit* l_Caster = p_AreaTrigger->GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    l_Caster->GetPlayerListInGrid(l_PlayerList, 70.0f);

                    for (Player* l_Player : l_PlayerList)
                    {
                        if (l_Player->IsWithinDist(p_AreaTrigger, 60.0f, true))
                        {
                            if (l_Player->isAlive() && !l_Player->HasMovementForce(p_AreaTrigger->GetGUID()))
                                l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), true, s_HeatWavePos, -7.0f, 1);
                            else if (!l_Player->isAlive() && l_Player->HasMovementForce(p_AreaTrigger->GetGUID()))
                                l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, s_HeatWavePos);
                        }
                        else if (l_Player->HasMovementForce(p_AreaTrigger->GetGUID()))
                            l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, s_HeatWavePos);

                        if (l_Player->HasAura(eSpells::SpellPeriodicDamage))
                            continue;

                        l_Caster->CastSpell(l_Player, eSpells::SpellPeriodicDamage, true);
                    }
                }
            }

            void OnRemove(AreaTrigger* p_AreaTrigger, uint32)
            {
                if (Unit* l_Caster = p_AreaTrigger->GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    l_Caster->GetPlayerListInGrid(l_PlayerList, 70.0f);

                    for (Player* l_Player : l_PlayerList)
                    {
                        l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, s_HeatWavePos);
                        l_Player->RemoveAura(eSpells::SpellPeriodicDamage);
                    }
                }
            }

            AreaTriggerEntityScript* GetAI() const
            {
                return new areatrigger_roltall_heat_wave();
            }
    };

    /// Burning Slag - 152913
    class areatrigger_roltall_burning_slag : public AreaTriggerEntityScript
    {
        public:
            areatrigger_roltall_burning_slag() : AreaTriggerEntityScript("areatrigger_roltall_burning_slag") { }

            enum eSpells
            {
                SpellPeriodicDamage = 153227
            };

            void OnUpdate(AreaTrigger* p_AreaTrigger, uint32)
            {
                if (Unit* l_Caster = p_AreaTrigger->GetCaster())
                {
                    std::list<Unit*> l_TargetList;
                    float l_Radius = 4.0f;

                    JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                    JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                    p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                    for (Unit* l_Unit : l_TargetList)
                    {
                        if (l_Unit->GetDistance(p_AreaTrigger) > l_Radius)
                            continue;

                        l_Caster->CastSpell(l_Unit, eSpells::SpellPeriodicDamage, true);
                    }
                }
            }

            AreaTriggerEntityScript* GetAI() const
            {
                return new areatrigger_roltall_burning_slag();
            }
    };
}
} } ///< namespace MS::Instances

#ifndef __clang_analyzer__
void AddSC_boss_roltall()
{
    new MS::Instances::Bloodmaul::boss_roltall();
    new MS::Instances::Bloodmaul::npc_fiery_boulder();
    new MS::Instances::Bloodmaul::spell_roltall_scorching_aura();
    new MS::Instances::Bloodmaul::areatrigger_roltall_heat_wave();
    new MS::Instances::Bloodmaul::areatrigger_roltall_burning_slag();
}
#endif
