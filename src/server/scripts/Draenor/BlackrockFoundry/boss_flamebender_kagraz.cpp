////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Flamebender Ka'graz <Shaman of the Burning Blade> - 76814
class boss_flamebender_kagraz : public CreatureScript
{
    public:
        boss_flamebender_kagraz() : CreatureScript("boss_flamebender_kagraz") { }

        enum eTalks
        {
            TalkAggro,
            TalkAknorDeath,
            TalkFirestorm,
            TalkCinderWolves,
            TalkBerserk,
            TalkSlay,
            TalkWipe,
            TalkDeath,
            TalkCinderWolvesWarn,
            TalkFirestormWarn
        };

        enum eSpells
        {
            /// Cosmetic
            PrefightCosmeticBossAura    = 156237,
            /// Lava Slash
            LavaSlashSearcher           = 154914,
            LavaSlashMissile            = 155297,   ///< Triggers 155318 - AoE damage - Summons 76996
            LavaSlashAreaTrigger        = 154915
        };

        enum eEvents
        {
            EventLavaSlash = 1,
            EventMoltenTorrent,
            EventCinderWolves,
            EventBlazingRadiance,
            EventFirestorm
        };

        enum eTimers
        {
            TimerEnergize       = 1 * TimeConstants::IN_MILLISECONDS + 200,
            TimerLavaSlash      = 11 * TimeConstants::IN_MILLISECONDS,
            TimerLavaSlashAgain = 14 * TimeConstants::IN_MILLISECONDS + 500
        };

        enum eCosmeticEvents
        {
            EventPeriodicEnergize = 1
        };

        enum eActions
        {
            ActionAknorDied
        };

        enum eGameObject
        {
        };

        struct boss_flamebender_kagrazAI : public BossAI
        {
            boss_flamebender_kagrazAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataFlamebenderKagraz)
            {
                m_Instance  = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            bool CanRespawn() override
            {
                return false;
            }

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                _Reset();

                me->RemoveAllAreasTrigger();

                me->CastSpell(me, eSpells::PrefightCosmeticBossAura, false);

                me->setPowerType(Powers::POWER_ENERGY);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->SetPower(Powers::POWER_ENERGY, 0);

                if (m_Instance)
                {
                    if (Creature* l_Aknor = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::AknorSteelbringer)))
                    {
                        l_Aknor->DespawnOrUnsummon();
                        l_Aknor->Respawn();

                        uint64 l_Guid = l_Aknor->GetGUID();
                        AddTimedDelayedOperation(100, [this, l_Guid]() -> void
                        {
                            if (Creature* l_Aknor = Creature::GetCreature(*me, l_Guid))
                                l_Aknor->GetMotionMaster()->MoveTargetedHome();
                        });
                    }
                }
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(eTalks::TalkSlay);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->InterruptNonMeleeSpells(true);

                _EnterCombat();

                Talk(eTalks::TalkAggro);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                AttackStart(p_Attacker);

                m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventPeriodicEnergize, eTimers::TimerEnergize);

                m_Events.ScheduleEvent(eEvents::EventLavaSlash, eTimers::TimerLavaSlash);
            }

            void EnterEvadeMode() override
            {
                Talk(eTalks::TalkWipe);

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }

                CreatureAI::EnterEvadeMode();
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalks::TalkDeath);

                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    default:
                        break;
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    default:
                        break;
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionAknorDied:
                    {
                        Talk(eTalks::TalkAknorDeath);

                        if (m_Instance)
                            m_Instance->SetData(eFoundryDatas::AknorDied, 1);

                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                switch (p_ID)
                {
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_ID) override
            {
                switch (p_ID)
                {
                    default:
                        break;
                }

                return 0;
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                /// Flamebender Ka'graz only regens by script
                p_Value = 0;
            }

            void SetPower(Powers p_Power, int32 p_NewValue) override
            {
                int32 l_OldValue = me->GetPower(p_Power);

                /// Flamebender Ka'graz gains energy over time allowing her to access even more powerful abilities.
                /// She gains abilities at 25, 50, 75, and 100 Molten Energy.
                if (p_NewValue >= 100)
                {
                    m_Events.ScheduleEvent(eEvents::EventFirestorm, 1);
                }
                else if (p_NewValue >= 75 && l_OldValue < 75)
                {
                    m_Events.ScheduleEvent(eEvents::EventBlazingRadiance, 1);
                }
                else if (p_NewValue >= 50 && l_OldValue < 50)
                {
                    m_Events.ScheduleEvent(eEvents::EventCinderWolves, 1);
                }
                else if (p_NewValue >= 25 && l_OldValue < 25)
                {
                    m_Events.ScheduleEvent(eEvents::EventMoltenTorrent, 1);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    case eCosmeticEvents::EventPeriodicEnergize:
                    {
                        me->ModifyPower(Powers::POWER_ENERGY, 1);

                        m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventPeriodicEnergize, eTimers::TimerEnergize);
                        break;
                    }
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventLavaSlash:
                    {
                        m_Events.ScheduleEvent(eEvents::EventLavaSlash, eTimers::TimerLavaSlashAgain);
                        break;
                    }
                    case eEvents::EventMoltenTorrent:
                    {
                        break;
                    }
                    case eEvents::EventCinderWolves:
                    {
                        Talk(eTalks::TalkCinderWolves);
                        Talk(eTalks::TalkCinderWolvesWarn);
                        break;
                    }
                    case eEvents::EventBlazingRadiance:
                    {
                        break;
                    }
                    case eEvents::EventFirestorm:
                    {
                        Talk(eTalks::TalkFirestorm);
                        Talk(eTalks::TalkFirestormWarn);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_flamebender_kagrazAI(p_Creature);
        }
};

/// Aknor Steelbringer - 77337
class npc_foundry_aknor_steelbringer : public CreatureScript
{
    public:
        npc_foundry_aknor_steelbringer() : CreatureScript("npc_foundry_aknor_steelbringer") { }

        enum eSpells
        {
            DevastatingSlam         = 156018,
            /// Drop the Hammer
            DropTheHammerSearcher   = 156038,
            DropTheHammerJump       = 156039,
            DropTheHammerAoE        = 156040
        };

        enum eEvents
        {
            EventDevastatingSlam = 1,
            EventDropTheHammer
        };

        enum eTimers
        {
            TimerDevastatingSlam        = 6 * TimeConstants::IN_MILLISECONDS,
            TimerDevastatingSlamAgain   = 8 * TimeConstants::IN_MILLISECONDS,
            TimerDropTheHammer          = 10 * TimeConstants::IN_MILLISECONDS,
            TimerDropTheHammerAgain     = 8 * TimeConstants::IN_MILLISECONDS
        };

        enum eActions
        {
            AknorDied,
            DropTheHammer
        };

        struct npc_foundry_aknor_steelbringerAI : public ScriptedAI
        {
            npc_foundry_aknor_steelbringerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            uint64 m_HammerTarget;

            void Reset() override
            {
                m_Events.Reset();

                m_HammerTarget = 0;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventDevastatingSlam, eTimers::TimerDevastatingSlam);
                m_Events.ScheduleEvent(eEvents::EventDropTheHammer, eTimers::TimerDropTheHammer);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::DropTheHammer)
                {
                    if (Unit* l_Target = Unit::GetUnit(*me, m_HammerTarget))
                        me->CastSpell(*l_Target, eSpells::DropTheHammerJump, true);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_ID == eSpells::DropTheHammerJump)
                    me->CastSpell(me, eSpells::DropTheHammerAoE, false);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::DropTheHammerSearcher:
                    {
                        m_HammerTarget = p_Target->GetGUID();
                        break;
                    }
                    default:
                        break;
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Kagraz = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossFlamebenderKagraz)))
                {
                    if (l_Kagraz->IsAIEnabled)
                        l_Kagraz->AI()->DoAction(eActions::AknorDied);
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
                    case eEvents::EventDevastatingSlam:
                    {
                        me->CastSpell(me, eSpells::DevastatingSlam, false);
                        m_Events.ScheduleEvent(eEvents::EventDevastatingSlam, eTimers::TimerDevastatingSlamAgain);
                        break;
                    }
                    case eEvents::EventDropTheHammer:
                    {
                        me->CastSpell(me, eSpells::DropTheHammerSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventDropTheHammer, eTimers::TimerDropTheHammerAgain);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_aknor_steelbringerAI(p_Creature);
        }
};

/// Drop the Hammer (aura) - 156038
class spell_foundry_drop_the_hammer_aura : public SpellScriptLoader
{
    public:
        spell_foundry_drop_the_hammer_aura() : SpellScriptLoader("spell_foundry_drop_the_hammer_aura") { }

        class spell_foundry_drop_the_hammer_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_drop_the_hammer_aura_AuraScript);

            enum eSpell
            {
                BodySlam = 155747
            };

            enum eAction
            {
                DropTheHammer = 1
            };

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode != AuraRemoveMode::AURA_REMOVE_BY_EXPIRE || GetCaster() == nullptr)
                    return;

                if (Creature* l_Aknor = GetCaster()->ToCreature())
                {
                    if (l_Aknor->IsAIEnabled)
                        l_Aknor->AI()->DoAction(eAction::DropTheHammer);
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_foundry_drop_the_hammer_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_drop_the_hammer_aura_AuraScript();
        }
};

void AddSC_boss_flamebender_kagraz()
{
    /// Boss
    new boss_flamebender_kagraz();

    /// Creatures
    new npc_foundry_aknor_steelbringer();

    /// Spells
    new spell_foundry_drop_the_hammer_aura();

    /// AreaTriggers
}