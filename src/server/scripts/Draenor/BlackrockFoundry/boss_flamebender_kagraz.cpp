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
            AllowMoltenTorrentCast      = 155912,
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

        enum eCreature
        {
            FlamebenderKagrazTrigger = 76996
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

            uint64 m_LavaSlashTarget;

            bool CanRespawn() override
            {
                return false;
            }

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                summons.DespawnAll();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                _Reset();

                me->RemoveAllAreasTrigger();

                me->CastSpell(me, eSpells::AllowMoltenTorrentCast, true);
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

                m_LavaSlashTarget = 0;
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

            void JustSummoned(Creature* p_Summon) override
            {
                if (p_Summon->GetEntry() == eCreature::FlamebenderKagrazTrigger && p_Summon->IsAIEnabled)
                    p_Summon->AI()->SetGUID(m_LavaSlashTarget, 0);
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
                        Unit* l_Target = nullptr;
                        if (l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -5.0f))
                            me->CastSpell(l_Target, eSpells::LavaSlashMissile, false);
                        else if (l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::LavaSlashMissile, false);

                        m_LavaSlashTarget = l_Target->GetGUID();

                        Position l_Dest = *l_Target;
                        Position l_Src  = *me;

                        AddTimedDelayedOperation(500, [this, l_Dest, l_Src]() -> void
                        {
                            uint8 l_Dist = l_Src.GetExactDist2d(&l_Dest);
                            for (uint8 l_I = 0; l_I <= l_Dist; ++l_I)
                            {
                                Position l_Target;

                                float l_O = l_Src.GetAngle(&l_Dest);
                                float l_X = l_Src.m_positionX + (l_I * cos(l_O));
                                float l_Y = l_Src.m_positionY + (l_I * sin(l_O));
                                float l_Z = l_Src.m_positionZ;

                                l_Target.m_positionX    = l_X;
                                l_Target.m_positionY    = l_Y;
                                l_Target.m_positionZ    = l_Z;
                                l_Target.m_orientation  = l_O;

                                me->CastSpell(l_Target, eSpells::LavaSlashAreaTrigger, true);
                            }
                        });

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

/// Flamebender Ka'graz Trigger - 76996
class npc_foundry_flamebender_kagraz_trigger : public CreatureScript
{
    public:
        npc_foundry_flamebender_kagraz_trigger() : CreatureScript("npc_foundry_flamebender_kagraz_trigger") { }

        enum eSpells
        {
            LavaSlashSearcherSecond = 155357,
            LavaSlashMissile        = 155297,
            LavaSlashAreaTrigger    = 154915
        };

        struct npc_foundry_flamebender_kagraz_triggerAI : public ScriptedAI
        {
            npc_foundry_flamebender_kagraz_triggerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint64 m_LavaSlashTarget;

            void Reset() override
            {
                m_LavaSlashTarget = 0;

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                me->CastSpell(me, eSpells::LavaSlashSearcherSecond, true);
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
                m_LavaSlashTarget = p_Guid;
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::LavaSlashSearcherSecond:
                    {
                        if (p_Target->GetGUID() == m_LavaSlashTarget)
                            break;

                        me->CastSpell(p_Target, eSpells::LavaSlashMissile, true);

                        Position l_Dest = *p_Target;
                        Position l_Src  = *me;

                        AddTimedDelayedOperation(200, [this, l_Dest, l_Src]() -> void
                        {
                            uint8 l_Dist = l_Src.GetExactDist2d(&l_Dest);
                            for (uint8 l_I = 0; l_I <= l_Dist; ++l_I)
                            {
                                Position l_Target;

                                float l_O = l_Src.GetAngle(&l_Dest);
                                float l_X = l_Src.m_positionX + (l_I * cos(l_O));
                                float l_Y = l_Src.m_positionY + (l_I * sin(l_O));
                                float l_Z = l_Src.m_positionZ;

                                l_Target.m_positionX    = l_X;
                                l_Target.m_positionY    = l_Y;
                                l_Target.m_positionZ    = l_Z;
                                l_Target.m_orientation  = l_O;

                                me->CastSpell(l_Target, eSpells::LavaSlashAreaTrigger, true);
                            }
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_flamebender_kagraz_triggerAI(p_Creature);
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

/// Lava Slash - 154915
class areatrigger_foundry_lava_slash_pool : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_lava_slash_pool() : AreaTriggerEntityScript("areatrigger_foundry_lava_slash_pool") { }

        enum eSpell
        {
            LavaSlashDoT = 155314
        };

        std::set<uint64> m_AffectedPlayers;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 0.5f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                std::set<uint64> l_Targets;

                for (Unit* l_Iter : l_TargetList)
                {
                    l_Targets.insert(l_Iter->GetGUID());

                    if (!l_Iter->HasAura(eSpell::LavaSlashDoT))
                    {
                        m_AffectedPlayers.insert(l_Iter->GetGUID());
                        l_Iter->CastSpell(l_Iter, eSpell::LavaSlashDoT, true);
                    }
                }

                for (std::set<uint64>::iterator l_Iter = m_AffectedPlayers.begin(); l_Iter != m_AffectedPlayers.end();)
                {
                    if (l_Targets.find((*l_Iter)) != l_Targets.end())
                    {
                        ++l_Iter;
                        continue;
                    }

                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, (*l_Iter)))
                    {
                        l_Iter = m_AffectedPlayers.erase(l_Iter);
                        l_Unit->RemoveAura(eSpell::LavaSlashDoT);

                        continue;
                    }

                    ++l_Iter;
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                for (uint64 l_Guid : m_AffectedPlayers)
                {
                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, l_Guid))
                        l_Unit->RemoveAura(eSpell::LavaSlashDoT);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_lava_slash_pool();
        }
};

void AddSC_boss_flamebender_kagraz()
{
    /// Boss
    new boss_flamebender_kagraz();

    /// Creatures
    new npc_foundry_aknor_steelbringer();
    new npc_foundry_flamebender_kagraz_trigger();

    /// Spells
    new spell_foundry_drop_the_hammer_aura();

    /// AreaTriggers
    new areatrigger_foundry_lava_slash_pool();
}