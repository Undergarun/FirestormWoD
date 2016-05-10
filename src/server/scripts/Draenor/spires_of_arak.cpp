////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "spires_of_arak.h"
#include "GameObjectAI.h"
#include "NPCHandler.h"
#include "Vehicle.h"
#include "PhaseMgr.h"
#include "Common.h"


/// 83746 - Rukhmar
class boss_rukhmar : public CreatureScript
{
    public:
        boss_rukhmar() : CreatureScript("boss_rukhmar") { }

        struct boss_rukhmarAI : public ScriptedAI
        {
            boss_rukhmarAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            float GetDataZ() { return m_ZRef; }

            enum eEvents
            {
                EventMove = 1
            };

            enum eMovements
            {
                MovementUp    = 8000,
                MovementDown  = 8001
            };

            EventMap m_Events;
            float m_ZRef;
            float m_ZNew;

            void Reset() override
            {
                m_ZRef               = 0.0f;
                me->m_CombatDistance = 90.0f;
                me->SetCanFly(true);
                m_Events.Reset();

                me->SetReactState(REACT_DEFENSIVE);

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SNARE, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
                me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->AddAura(SpiresOfArakSpells::SouthshoreMobScalingAura, me);
            }

            void JustSummoned(Creature* p_Summon) override
            {
                summons.Summon(p_Summon);
            }

            void SummonedCreatureDespawn(Creature* p_Summon) override
            {
                summons.Despawn(p_Summon);
            }

            void LaunchGroundEvents()
            {
                  m_Events.ScheduleEvent(SpiresOfArakEvents::EventSharpBeak, 3000);
                  m_Events.ScheduleEvent(SpiresOfArakEvents::EventBloodFeather, 7000);
                  m_Events.ScheduleEvent(SpiresOfArakEvents::EventSolarBreath, 12000);
                  m_Events.ScheduleEvent(SpiresOfArakEvents::EventBlazeOfGlory, 19000);
                  m_Events.ScheduleEvent(SpiresOfArakEvents::EventLooseQuills, 38000);
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                me->SetHomePosition(*me);
                me->AddAura(SpiresOfArakSpells::SpellSolarRadiationAura, me);
                LaunchGroundEvents();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                summons.DespawnAll();
                m_Events.Reset();

                std::list<Creature*> l_CreatureList;
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureEnergizedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureDepletedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreaturePileOfAsh, 200.0f);

                for (Creature* l_Creature : l_CreatureList)
                    l_Creature->DespawnOrUnsummon();
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();
                summons.DespawnAll();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->ClearUnitState(UNIT_STATE_ROOT);
                m_Events.Reset();

                std::list<Creature*> l_CreatureList;
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureEnergizedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureDepletedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreaturePileOfAsh, 200.0f);

                for (Creature* l_Creature : l_CreatureList)
                    l_Creature->DespawnOrUnsummon();

                me->RemoveAura(SpiresOfArakSpells::SpellSolarRadiationAura);
                me->GetMotionMaster()->MoveTargetedHome();
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == SpiresOfArakActions::ActionMoveDownToward)
                {
                    m_Events.Reset();

                    if (m_ZRef)
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->ClearUnitState(UNIT_STATE_ROOT);
                        me->GetMotionMaster()->Clear(true);
                        me->GetMotionMaster()->MoveCharge(me->m_positionX, me->m_positionY, me->m_positionZ - 15.0f, 3.0f, eMovements::MovementDown);
                        LaunchGroundEvents();
                    }
                }
            }

            void MovementInform(uint32 /*p_MoveType*/, uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eMovements::MovementDown:
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventFlyOver, 800);
                        break;
                    case eMovements::MovementUp:
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventFlyDestReached, 800);
                        break;
                }
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo) override
            {
                if (!p_SpellInfo)
                    return;

                if (p_SpellInfo->Id == SpiresOfArakSpells::SpellLooseQuillsDummy)
                    me->CastSpell(p_Victim, SpiresOfArakSpells::SpellLooseQuillsMissile, true);
                else if (p_SpellInfo->Id == SpiresOfArakSpells::SpellBloodFeatherDummy)
                    me->CastSpell(me, SpiresOfArakSpells::SpellBloodFeatherMissile, true);
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->IsPlayer();
                });

                if (Aura* l_Scaling = me->GetAura(SpiresOfArakSpells::SouthshoreMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                    {
                        if ((SpiresOfArakDatas::DamageScalingCoeff * l_Count) != l_Damage->GetAmount())
                            l_Damage->ChangeAmount(SpiresOfArakDatas::DamageScalingCoeff * l_Count);
                    }
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                    {
                        if ((SpiresOfArakDatas::HealthScalingCoeff * l_Count) != l_Health->GetAmount())
                        l_Health->ChangeAmount(SpiresOfArakDatas::HealthScalingCoeff * l_Count);
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                HandleHealthAndDamageScaling();

                switch (m_Events.ExecuteEvent())
                {
                    case SpiresOfArakEvents::EventFlyDestReached:
                    {
                        std::list<Creature*> l_CreatureList;
                        std::list<Creature*> l_PhoenixList;

                        me->CastSpell(me, SpiresOfArakSpells::SpellLooseQuillsLauncher, true);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->AddUnitState(UNIT_STATE_ROOT);
                        me->GetMotionMaster()->Clear(false);
                        me->GetCreatureListWithEntryInGrid(l_CreatureList, SpiresOfArakCreatures::CreaturePileOfAsh, 150.0f);

                        for (Creature* l_Creature : l_CreatureList)
                        {
                            me->GetCreatureListWithEntryInGrid(l_PhoenixList, SpiresOfArakCreatures::CreatureDepletedPhoenix, 150.0f);

                            if (l_PhoenixList.size() >= 8)
                                break;

                            l_Creature->DespawnOrUnsummon();
                            me->SummonCreature(SpiresOfArakCreatures::CreatureDepletedPhoenix, l_Creature->m_positionX, l_Creature->m_positionY, l_Creature->m_positionZ);
                        }

                        break;
                    }
                    case SpiresOfArakEvents::EventFlyOver:
                    {
                        m_ZRef = 0.0f;
                        m_ZNew = 0.0f;
                        LaunchGroundEvents();
                        break;
                    }
                    case SpiresOfArakEvents::EventLooseQuills:
                    {
                        m_ZRef = me->GetPositionZ();
                        m_ZNew = m_ZRef + 15.0f;
                        m_Events.Reset();

                        me->GetMotionMaster()->Clear(false);
                        me->GetMotionMaster()->MoveCharge(me->m_positionX, me->m_positionY, m_ZNew, 3.0f, eMovements::MovementUp);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventLooseQuills, 120000);
                        break;
                    }
                    case SpiresOfArakEvents::EventSharpBeak:
                        me->GetMotionMaster()->Clear(false);
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SpiresOfArakSpells::SpellSharpBeak, false);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventSharpBeak, 33000);
                        break;
                    case SpiresOfArakEvents::EventBloodFeather:
                        me->CastSpell(me, SpiresOfArakSpells::SpellBloodFeatherDummy, false);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventBloodFeather, 33000);
                        break;
                    case SpiresOfArakEvents::EventSolarBreath:
                        me->CastSpell(me, SpiresOfArakSpells::SpellSolarBreath, false);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventSolarBreath, 33000);
                        break;
                    case SpiresOfArakEvents::EventBlazeOfGlory:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SpiresOfArakSpells::SpellBlazeOfGloryDummy, false);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventBlazeOfGlory, 33000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_rukhmarAI(p_Creature);
        }
};

/// 83769 - Energized Phoenix
class npc_energized_phoenix : public CreatureScript
{
    public:
        npc_energized_phoenix() : CreatureScript("npc_energized_phoenix") { }

        struct npc_energized_phoenixAI : public ScriptedAI
        {
            npc_energized_phoenixAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            uint64 m_SummonerGuid;
            uint64 m_PlayerGuid;
            bool m_KilledByPlayer;
            bool m_Fixated;

            void Reset() override
            {
                me->SetSpeed(MOVE_WALK, 2.0f);
                me->SetSpeed(MOVE_RUN, 2.0f);
                me->SetSpeed(MOVE_FLIGHT, 2.0f);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 0);
                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 0);
                me->SetReactState(REACT_PASSIVE);
                m_KilledByPlayer = true;
                m_PlayerGuid = 0;
                m_SummonerGuid = 0;
                m_Fixated = false;
                m_Events.Reset();

                me->AddAura(SpiresOfArakSpells::SouthshoreMobScalingAura, me);
                m_Events.ScheduleEvent(SpiresOfArakEvents::EventPhoenixFixatePlr, 200);
            }

            void JustSummoned(Creature* p_Summon) override
            {
                summons.Summon(p_Summon);
            }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                /// Select some random player to focus on
                m_SummonerGuid = p_Summoner->GetGUID();
            }

            void JustDied(Unit* p_Killer) override
            {
                if (m_KilledByPlayer)
                {
                    Position const* l_Pos = me;

                    me->CastSpell(p_Killer, SpiresOfArakSpells::SpellBlazeOfGloryDummy, true);
                    me->SummonCreature(SpiresOfArakCreatures::CreaturePileOfAsh, *l_Pos);
                    me->DespawnOrUnsummon();
                }
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->IsPlayer();
                });

                if (Aura* l_Scaling = me->GetAura(SpiresOfArakSpells::SouthshoreMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                    {
                        if ((SpiresOfArakDatas::DamageScalingCoeff * l_Count) != l_Damage->GetAmount())
                            l_Damage->ChangeAmount(SpiresOfArakDatas::DamageScalingCoeff * l_Count);
                    }
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                    {
                        if ((SpiresOfArakDatas::HealthScalingCoeff * l_Count) != l_Health->GetAmount())
                            l_Health->ChangeAmount(SpiresOfArakDatas::HealthScalingCoeff * l_Count);
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                m_Events.Update(p_Diff);
                HandleHealthAndDamageScaling();

                switch (m_Events.ExecuteEvent())
                {
                    case SpiresOfArakEvents::EventPhoenixFixatePlr:
                    {
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 100.0f);

                        if (!l_PlayerList.empty())
                        {
                            JadeCore::RandomResizeList(l_PlayerList, 1);

                            if (Player* l_Player = l_PlayerList.front())
                            {
                                m_PlayerGuid = l_Player->GetGUID();
                                me->AddThreat(l_Player, 100000.0f);
                                me->CastSpell(l_Player, SpiresOfArakSpells::SpellFixate, false);
                                m_Events.ScheduleEvent(SpiresOfArakEvents::EventMoveToPlayer, 200);
                            }
                        }
                        break;
                    }
                    case SpiresOfArakEvents::EventMoveToPlayer:
                    {
                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            {
                                if (me->GetDistance2d(l_Player) <= 1.2f)
                                {
                                    m_KilledByPlayer = false;
                                    m_Events.Reset();

                                    me->GetMotionMaster()->Clear();
                                    me->CastSpell(l_Player, SpiresOfArakSpells::SpellBlazeOfGloryDummy, true);
                                    m_Events.ScheduleEvent(SpiresOfArakEvents::EventTurnPhoenixToAsh, 700);
                                }
                                else
                                {
                                    me->GetMotionMaster()->Clear();
                                    me->GetMotionMaster()->MoveCharge(l_Player->m_positionX, l_Player->m_positionY, l_Player->m_positionZ, 2.0f);
                                    m_Events.ScheduleEvent(SpiresOfArakEvents::EventMoveToPlayer, 200);
                                }
                            }
                        }
                        break;
                    }
                    case SpiresOfArakEvents::EventTurnPhoenixToAsh:
                    {
                        if (Creature* l_Rukhmar = me->GetCreature(*me, m_SummonerGuid))
                        {
                            Position const* l_Pos = me;
                            me->SummonCreature(SpiresOfArakCreatures::CreaturePileOfAsh, *l_Pos);
                            me->DespawnOrUnsummon();
                        }
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_energized_phoenixAI(p_Creature);
        }
};

/// 167629 - Blaze Of Glory
class spell_rukhmar_blaze_of_glory : public SpellScriptLoader
{
    public:
        spell_rukhmar_blaze_of_glory() : SpellScriptLoader("spell_rukhmar_blaze_of_glory") { }

        class spell_rukhmar_blaze_of_glory_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rukhmar_blaze_of_glory_SpellScript);

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                std::list<Creature*> l_CreatureList;
                std::list<Creature*> l_PhoenixList;

                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, SpiresOfArakCreatures::CreaturePileOfAsh, 150.0f);

                for (Creature* l_Creature : l_CreatureList)
                {
                    l_Caster->GetCreatureListWithEntryInGrid(l_PhoenixList, SpiresOfArakCreatures::CreatureEnergizedPhoenix, 150.0f);

                    if (l_PhoenixList.size() >= 8)
                        break;

                    l_Creature->DespawnOrUnsummon();
                    l_Caster->SummonCreature(SpiresOfArakCreatures::CreatureEnergizedPhoenix, l_Creature->m_positionX, l_Creature->m_positionY, l_Creature->m_positionZ);
                }
            }

            void CorrectRange(std::list<WorldObject*>& p_Targets)
            {
                if (!GetCaster())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Obj) -> bool
                {
                    return p_Obj == GetCaster();
                });
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_rukhmar_blaze_of_glory_SpellScript::HandleOnCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rukhmar_blaze_of_glory_SpellScript::CorrectRange, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rukhmar_blaze_of_glory_SpellScript();
        }
};

/// 167647 - Loose Quills
class spell_rukhmar_loose_quills : public SpellScriptLoader
{
    public:
        spell_rukhmar_loose_quills() : SpellScriptLoader("spell_rukhmar_loose_quills") {}

        class spell_rukhmar_loose_quills_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rukhmar_loose_quills_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                WorldObject* l_Owner = GetOwner();

                if (l_Owner)
                {
                    if (Creature* l_Creature = l_Owner->ToCreature())
                    {
                        if (l_Creature->GetAI())
                            l_Creature->GetAI()->DoAction(SpiresOfArakActions::ActionMoveDownToward);
                    }
                }
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_rukhmar_loose_quills_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rukhmar_loose_quills_AuraScript();
        }
};

/// 167647 - Loose Quills
class spell_aura_pierced_armor : public SpellScriptLoader
{
    public:
        spell_aura_pierced_armor() : SpellScriptLoader("spell_aura_pierced_armor") {}

        class spell_aura_pierced_armor_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_aura_pierced_armor_AuraScript);

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                WorldObject* l_Owner = GetOwner();

                if (!l_Owner)
                    return;

                if (Creature* l_Rukhmar = l_Owner->ToCreature())
                    l_Rukhmar->CastSpell(l_Rukhmar, SpiresOfArakSpells::SpellSolarBreathDamage, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_aura_pierced_armor_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_aura_pierced_armor_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_spires_of_arak()
{
    new boss_rukhmar();
    new npc_energized_phoenix();
    new spell_rukhmar_blaze_of_glory();
    new spell_rukhmar_loose_quills();
    new spell_aura_pierced_armor();
}
#endif
