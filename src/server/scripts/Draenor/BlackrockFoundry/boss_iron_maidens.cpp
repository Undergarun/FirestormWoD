////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "boss_iron_maidens.hpp"

/// Admiral Gar'an - 77557
class boss_admiral_garan : public CreatureScript
{
    public:
        boss_admiral_garan() : CreatureScript("boss_admiral_garan") { }

        enum eSpells
        {
            IronMaidenIntroConversation = 172658,
            AfterTrashesConversation    = 172686
        };

        enum eEvents
        {
        };

        enum eTimers
        {
        };

        enum eTalks
        {
        };

        struct boss_admiral_garanAI : public BossAI
        {
            boss_admiral_garanAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataIronMaidens)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            bool m_IntroDone;
            std::set<uint64> m_TrashesGuids;

            void Reset() override
            {
                m_Events.Reset();

                RespawnMaidens(m_Instance, me);

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);

                if (!m_IntroDone)
                {
                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        std::list<Creature*> l_TrashesList;

                        me->GetCreatureListWithEntryInGrid(l_TrashesList, eIronMaidensCreatures::AquaticTechnician, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_TrashesList, eIronMaidensCreatures::IronDockworker, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_TrashesList, eIronMaidensCreatures::IronEarthbinder, 150.0f);
                        me->GetCreatureListWithEntryInGridAppend(l_TrashesList, eIronMaidensCreatures::IronMauler, 150.0f);

                        for (Creature* l_Trash : l_TrashesList)
                            m_TrashesGuids.insert(l_Trash->GetGUID());
                    });
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_ID /*= 0*/) override
            {
                m_TrashesGuids.erase(p_Guid);

                if (!m_IntroDone && m_TrashesGuids.empty())
                {
                    m_IntroDone = true;

                    DoAction(eIronMaidensActions::ActionAfterTrashesIntro);
                }
            }

            void KilledUnit(Unit* p_Killed) override
            {
                /*if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartMaidens(m_Instance, me, p_Attacker);

                _EnterCombat();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void EnterEvadeMode() override
            {
                WipeMaidens(m_Instance);

                CreatureAI::EnterEvadeMode();

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eIronMaidensActions::ActionIntro:
                    {
                        me->CastSpell(me, eSpells::IronMaidenIntroConversation, false);
                        break;
                    }
                    case eIronMaidensActions::ActionAfterTrashesIntro:
                    {
                        me->CastSpell(me, eSpells::AfterTrashesConversation, false);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                /*if (me->GetDistance(me->GetHomePosition()) >= 120.0f)
                {
                    EnterEvadeMode();
                    return;
                }*/

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_admiral_garanAI(p_Creature);
        }
};

/// Enforcer Sorka - 77231
class boss_enforcer_sorka : public CreatureScript
{
    public:
        boss_enforcer_sorka() : CreatureScript("boss_enforcer_sorka") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        enum eTimers
        {
        };

        enum eTalks
        {
        };

        struct boss_enforcer_sorkaAI : public BossAI
        {
            boss_enforcer_sorkaAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataIronMaidens)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                RespawnMaidens(m_Instance, me);

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                /*if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartMaidens(m_Instance, me, p_Attacker);

                _EnterCombat();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void EnterEvadeMode() override
            {
                WipeMaidens(m_Instance);

                CreatureAI::EnterEvadeMode();

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                /*if (me->GetDistance(me->GetHomePosition()) >= 120.0f)
                {
                    EnterEvadeMode();
                    return;
                }*/

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_enforcer_sorkaAI(p_Creature);
        }
};

/// Marak the Blooded - 77477
class boss_marak_the_blooded : public CreatureScript
{
    public:
        boss_marak_the_blooded() : CreatureScript("boss_marak_the_blooded") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        enum eTimers
        {
        };

        enum eTalks
        {
        };

        struct boss_marak_the_bloodedAI : public BossAI
        {
            boss_marak_the_bloodedAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataIronMaidens)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                RespawnMaidens(m_Instance, me);

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                /*if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartMaidens(m_Instance, me, p_Attacker);

                _EnterCombat();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void EnterEvadeMode() override
            {
                WipeMaidens(m_Instance);

                CreatureAI::EnterEvadeMode();

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                /*if (me->GetDistance(me->GetHomePosition()) >= 120.0f)
                {
                    EnterEvadeMode();
                    return;
                }*/

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_marak_the_bloodedAI(p_Creature);
        }
};

/// Loading Chain - 78767
class npc_foundry_loading_chain : public CreatureScript
{
    public:
        npc_foundry_loading_chain() : CreatureScript("npc_foundry_loading_chain") { }

        enum eSpells
        {
            RideLoadingChain    = 158646,
            LoadingChainVisual  = 159086
        };

        enum eEvent
        {
        };

        struct npc_foundry_loading_chainAI : public ScriptedAI
        {
            npc_foundry_loading_chainAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_ChainID(0) { }

            uint8 m_ChainID;

            void Reset() override
            {
                me->CastSpell(me, eSpells::LoadingChainVisual, true);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetUInt32Value(EUnitFields::UNIT_FIELD_INTERACT_SPELL_ID, eSpells::RideLoadingChain);

                /// Init chain ID
                for (Position const l_Pos : g_LoadingChainsSpawnPos)
                {
                    if (me->IsNearPosition(&l_Pos, 0.5f))
                        break;

                    ++m_ChainID;
                }
            }

            uint32 GetData(uint32 p_ID /*= 0*/) override
            {
                return uint32(m_ChainID);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_loading_chainAI(p_Creature);
        }
};

/// Uk'urogg <Deckhand of Marak the Blooded> - 78341
class npc_foundry_ukurogg : public CreatureScript
{
    public:
        npc_foundry_ukurogg() : CreatureScript("npc_foundry_ukurogg") { }

        enum eSpells
        {
            CarryingCrate = 171198
        };

        enum eEvents
        {
        };

        enum eMoves
        {
            MoveCarryCrate = 1,
            MoveThrowCrate
        };

        struct npc_foundry_ukuroggAI : public ScriptedAI
        {
            npc_foundry_ukuroggAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            Position const m_UkuroggThrowCratePos = { 495.576f, 3273.3f, 141.388f, 0.0f };
            Position const m_UkuroggCarryCratePos = { 478.2083f, 3280.669f, 141.388f, 0.0f };

            float const m_UkuroggFinalFacing = 2.859696f;

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetWalk(false);

                ClearDelayedOperations();
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::CarryingCrate)
                {
                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->SetWalk(true);

                        me->GetMotionMaster()->MovePoint(eMoves::MoveThrowCrate, m_UkuroggThrowCratePos);
                    });
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveCarryCrate:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->SetFacingTo(m_UkuroggFinalFacing);
                        });

                        break;
                    }
                    case eMoves::MoveThrowCrate:
                    {
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->RemoveAura(eSpells::CarryingCrate);

                            me->GetMotionMaster()->MovePoint(eMoves::MoveCarryCrate, m_UkuroggCarryCratePos);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_ukuroggAI(p_Creature);
        }
};

/// Iron Maidens Boat - 9945
class areatrigger_at_foundry_iron_maidens_boat : public AreaTriggerScript
{
    public:
        areatrigger_at_foundry_iron_maidens_boat() : AreaTriggerScript("areatrigger_at_foundry_iron_maidens_boat") { }

        enum eSpell
        {
            OnABoatPeriodic = 158726
        };

        void OnEnter(Player* p_Player, AreaTriggerEntry const* /*p_AreaTrigger*/) override
        {
            p_Player->CastSpell(p_Player, eSpell::OnABoatPeriodic, true);
        }

        void OnExit(Player* p_Player, AreaTriggerEntry const* /*p_AreaTrigger*/) override
        {
            p_Player->RemoveAura(eSpell::OnABoatPeriodic);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_iron_maidens()
{
    /// Bosses
    new boss_admiral_garan();
    new boss_enforcer_sorka();
    new boss_marak_the_blooded();

    /// Creatures
    new npc_foundry_loading_chain();
    new npc_foundry_ukurogg();

    /// AreaTrigger (world)
    new areatrigger_at_foundry_iron_maidens_boat();
}
#endif
