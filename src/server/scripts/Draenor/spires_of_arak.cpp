////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
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
#include <random>


/// 83746 - Rukhmar
class boss_rukhmar : public CreatureScript
{
    public:
        boss_rukhmar() : CreatureScript("boss_rukhmar") { }

        struct boss_rukhmarAI : public ScriptedAI
        {
            boss_rukhmarAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
                me->AddAura(SpiresOfArakSpells::SpellSolarRadiationAura, me);
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
                m_Events.ScheduleEvent(SpiresOfArakEvents::EventSolarBreath, 11000);
                m_Events.ScheduleEvent(SpiresOfArakEvents::EventBlazeOfGlory, 16000);
                m_Events.ScheduleEvent(SpiresOfArakEvents::EventLooseQuills, 33000);
            }

            void EnterCombat(Unit* p_Who) override
            {
                me->UpdateGroundPositionZ(p_Who->m_positionX, p_Who->m_positionY, p_Who->m_positionZ);
                LaunchGroundEvents();
            }
            void JustDied(Unit* /*p_Killer*/) override
            {
                summons.DespawnAll();

                std::list<Creature*> l_CreatureList;
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureEnergizedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureDepletedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreaturePileOfAsh, 200.0f);

                for (Creature* l_Creature : l_CreatureList)
                    l_Creature->DespawnOrUnsummon();
            }

            void EnterEvadeMode()
            {
                summons.DespawnAll();

                std::list<Creature*> l_CreatureList;
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureEnergizedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreatureDepletedPhoenix, 200.0f);
                GetCreatureListWithEntryInGrid(l_CreatureList, me, SpiresOfArakCreatures::CreaturePileOfAsh, 200.0f);

                for (Creature* l_Creature : l_CreatureList)
                    l_Creature->DespawnOrUnsummon();
            }

            void DoAction(int32 const p_Param)
            {
                LaunchGroundEvents();
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo) override
            {
                if (p_SpellInfo->Id == SpiresOfArakSpells::SpellLooseQuillsDummy)
                    me->CastSpell(p_Victim, SpiresOfArakSpells::SpellLooseQuillsMissile, true);
                else if (p_SpellInfo->Id == SpiresOfArakSpells::SpellBloodFeatherDummy)
                    me->CastSpell(me, SpiresOfArakSpells::SpellBloodFeatherMissile, true);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case SpiresOfArakEvents::EventLooseQuills:
                    {
                        float l_ZPos = me->GetPositionZ();
                        l_ZPos += 4.0f;
                        m_Events.Reset();

                        me->UpdateGroundPositionZ(me->m_positionX, me->m_positionY, l_ZPos);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->CastSpell(me, SpiresOfArakSpells::SpellLooseQuillsLauncher, true);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventLooseQuills, 33000);
                        break;
                    }
                    case SpiresOfArakEvents::EventSharpBeak:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SpiresOfArakSpells::SpellSharpBeak, false);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventSharpBeak, 33000);
                        break;
                    case SpiresOfArakEvents::EventBloodFeather:
                        me->CastSpell(me, SpiresOfArakSpells::SpellBloodFeatherDummy, false);
                        m_Events.ScheduleEvent(SpiresOfArakEvents::EventBloodFeather, 33000);
                        break;
                    case SpiresOfArakEvents::EventSolarBreath:
                        me->AddAura(SpiresOfArakSpells::SpellSolarBreath, me);
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

        CreatureAI* GetAI(Creature* p_Creature) const
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
            uint64 m_PlayerGuid;
            bool m_KilledByPlayer;

            void Reset()
            {
                me->SetSpeed(MOVE_WALK, 0.35f);
                me->SetSpeed(MOVE_RUN, 0.3f);
                me->SetSpeed(MOVE_FLIGHT, 0.3f);
                me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, 0);
                me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, 0);
                me->SetReactState(REACT_PASSIVE);
                m_KilledByPlayer = true;
                m_PlayerGuid = 0;
                m_Events.Reset();
            }

            void JustSummoned(Creature* p_Summon) override
            {
                summons.Summon(p_Summon);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 30.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (urand(0, 1))
                    {
                         m_PlayerGuid = l_Player->GetGUID();
                         me->AddThreat(l_Player, 1000000.0f);
                         me->CastSpell(l_Player, SpiresOfArakSpells::SpellFixate, false);
                         m_Events.ScheduleEvent(SpiresOfArakEvents::EventMoveToPlayer, 300);

                        break;
                    }
                }
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

            void UpdateAI(const uint32 p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case SpiresOfArakEvents::EventMoveToPlayer:
                    {
                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            {
                                if (me->GetDistance2d(l_Player) <= 1.0f)
                                {
                                    Position const* l_Pos = me;
                                    m_KilledByPlayer = false;
                                    m_Events.Reset();

                                    me->GetMotionMaster()->Clear();
                                    me->CastSpell(l_Player, SpiresOfArakSpells::SpellBlazeOfGloryDummy, true);
                                    me->SummonCreature(SpiresOfArakCreatures::CreaturePileOfAsh, *l_Pos);
                                    me->DespawnOrUnsummon();
                                }
                                else
                                {
                                    Position l_Pos;
                                    l_Player->GetPosition(&l_Pos);
                                    me->GetMotionMaster()->MovePoint(0, l_Pos);
                                    m_Events.ScheduleEvent(SpiresOfArakEvents::EventMoveToPlayer, 300);
                                }
                            }
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

        uint64 m_MainTarget;

        void HandleOnCast()
        {
            Unit* l_Caster = GetCaster();

            if (!l_Caster)
                return;

            std::list<Creature*> l_CreatureList;
            l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, SpiresOfArakCreatures::CreaturePileOfAsh, 50.0f);

            for (Creature* l_Creature : l_CreatureList)
            {
                l_Creature->DespawnOrUnsummon();
                l_Caster->SummonCreature(SpiresOfArakCreatures::CreatureEnergizedPhoenix, l_Creature->m_positionX, l_Creature->m_positionY, l_Creature->m_positionZ);
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_rukhmar_blaze_of_glory_SpellScript::HandleOnCast);
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

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                WorldObject* l_Owner = GetOwner();

                if (l_Owner)
                {
                    if (Creature* l_Creature = l_Owner->ToCreature())
                    {
                        l_Owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                        float l_ZPos = l_Owner->GetPositionZ();
                        l_ZPos -= 4.0f;
                        l_Owner->UpdateGroundPositionZ(l_Owner->m_positionX, l_Owner->m_positionY, l_ZPos);

                        if (l_Creature->AI())
                            l_Creature->AI()->DoAction(0);
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

void AddSC_spires_of_arak()
{
    new boss_rukhmar();
    new npc_energized_phoenix();
    new spell_rukhmar_blaze_of_glory();
    new spell_rukhmar_loose_quills();
}