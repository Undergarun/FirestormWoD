////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "boss_beastlord_darmac.hpp"

/// Beastlord Darmac <The Iron Claw> - 76865
class boss_beastlord_darmac : public CreatureScript
{
    public:
        boss_beastlord_darmac() : CreatureScript("boss_beastlord_darmac") { }

        enum eTalks
        {
            TalkAggro,
            TalkMountCruelfang,
            TalkCruelfangsSwiftness,
            TalkMountIroncrusher,
            TalkIroncrushersRage,
            TalkMountDreadwing,
            TalkDreadwingsFlame,
            TalkMountFaultline,
            TalkFaultlineDetermination,
            TalkRendAndTear,
            TalkTantrum,
            TalkSuperheatedShrapnel,
            TalkSlay,
            TalkDeath,
            TalkSuperheatedShrapnelWarn,
            TalkTantrumWarn
        };

        enum eSpells
        {
            /// Misc
            BeastlordDarmacBonus    = 177536,
            /// Pin Down
            PinDownSearcher         = 155365,
            PinDownMissile          = 154951,
            /// Call the Pack
            CallThePackCast         = 154975
        };

        enum eEvents
        {
            /// Stage One: Ferocious Feet
            EventBerserker = 1,
            EventCallThePack,
            EventPinDown,
            /// Stage Two: Back of the Beast
            /// Stage Three: Spiritual Successor
        };

        enum eTimers
        {
            TimerBerserker              = 12 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS,
            TimerCallThePack            = 8 * TimeConstants::IN_MILLISECONDS,
            TimerCallThePackLFR         = 18 * TimeConstants::IN_MILLISECONDS,
            TimerCallThePackCooldown    = 30 * TimeConstants::IN_MILLISECONDS,
            TimerCallThePackLFRCooldown = 40 * TimeConstants::IN_MILLISECONDS,
            TimerPinDown                = 9 * TimeConstants::IN_MILLISECONDS + 500,
            TimerPinDownCooldown        = 19 * TimeConstants::IN_MILLISECONDS + 700
        };

        enum eActions
        {
            ActionCallThePack
        };

        enum eCreatures
        {
            HitchingPost    = 79914,
            PackBeast       = 77128
        };

        enum eVisuals
        {
            EmoteUse    = 69,
            EmoteYes    = 273
        };

        enum eMoves
        {
            MoveIronCrusher = 50,
            MoveCruelfang,
            MoveDreadwing
        };

        enum ePhases
        {
            PhaseNone               = 0x00,
            PhaseFerociousFeet      = 0x01,
            PhaseBackOfTheBeast     = 0x02 | ePhases::PhaseFerociousFeet,
            PhaseSpiritualSuccessor = 0x04 | ePhases::PhaseFerociousFeet
        };

        struct boss_beastlord_darmacAI : public BossAI
        {
            boss_beastlord_darmacAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataBeastlordDarmac)
            {
                m_Instance  = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            EventMap m_Events;

            uint8 m_CosmeticMove;

            bool CanRespawn() override
            {
                return false;
            }

            void Reset() override
            {
                ClearDelayedOperations();

                summons.DespawnAll();

                _Reset();

                me->RemoveAllAreasTrigger();

                m_Events.Reset();
                m_Events.SetPhase(ePhases::PhaseNone);

                me->RemoveAura(eFoundrySpells::Berserker);

                m_CosmeticMove = eMoves::MoveIronCrusher;

                uint32 l_Time = 100;
                AddTimedDelayedOperation(l_Time, [this]() -> void
                {
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                });

                l_Time += 9 * TimeConstants::IN_MILLISECONDS;
                AddTimedDelayedOperation(l_Time, [this]() -> void
                {
                    me->HandleEmoteCommand(eVisuals::EmoteYes);
                });

                l_Time += 1 * TimeConstants::IN_MILLISECONDS;
                AddTimedDelayedOperation(l_Time, [this]() -> void
                {
                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(m_CosmeticMove + 1, g_DarmacCosmeticMoves[m_CosmeticMove - eMoves::MoveIronCrusher]);
                });

                ResetBeasts();
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->SetWalk(false);
                me->StopMoving();

                AttackStart(p_Attacker);

                ClearDelayedOperations();

                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);

                _EnterCombat();

                Talk(eTalks::TalkAggro);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                m_Events.SetPhase(ePhases::PhaseFerociousFeet);

                m_Events.ScheduleEvent(eEvents::EventBerserker, eTimers::TimerBerserker);
                m_Events.ScheduleEvent(eEvents::EventCallThePack, IsLFR() ? eTimers::TimerCallThePackLFR : eTimers::TimerCallThePack, ePhases::PhaseFerociousFeet);
                m_Events.ScheduleEvent(eEvents::EventPinDown, eTimers::TimerPinDown, ePhases::PhaseFerociousFeet);
            }

            void EnterEvadeMode() override
            {
                me->DespawnCreaturesInArea({ eCreatures::PackBeast });

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }

                CreatureAI::EnterEvadeMode();
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                Talk(eTalks::TalkDeath);

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    /// Allow loots and bonus loots to be enabled/disabled with a simple reload
                    if (sObjectMgr->IsDisabledEncounter(m_Instance->GetEncounterIDForBoss(me)))
                        me->SetLootRecipient(nullptr);
                    else
                        CastSpellToPlayers(me->GetMap(), me, eSpells::BeastlordDarmacBonus, true);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMoves::MoveIronCrusher:
                    case eMoves::MoveCruelfang:
                    {
                        ++m_CosmeticMove;
                        break;
                    }
                    case eMoves::MoveDreadwing:
                    {
                        m_CosmeticMove = eMoves::MoveIronCrusher;
                        break;
                    }
                    default:
                        break;
                }

                uint32 l_Time = 100;
                AddTimedDelayedOperation(100, [this]() -> void
                {
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eVisuals::EmoteUse);
                });

                l_Time += 9 * TimeConstants::IN_MILLISECONDS;
                AddTimedDelayedOperation(l_Time, [this]() -> void
                {
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                });

                l_Time += 100;
                AddTimedDelayedOperation(l_Time, [this]() -> void
                {
                    me->HandleEmoteCommand(eVisuals::EmoteYes);
                });

                l_Time += 1 * TimeConstants::IN_MILLISECONDS;
                AddTimedDelayedOperation(l_Time, [this]() -> void
                {
                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(m_CosmeticMove, g_DarmacCosmeticMoves[m_CosmeticMove - eMoves::MoveIronCrusher]);
                });
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::PinDownSearcher:
                    {
                        me->CastSpell(*p_Target, eSpells::PinDownMissile, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::CallThePackCast:
                    {
                        if (m_Instance == nullptr)
                            break;

                        if (Creature* l_Trigger = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::ThunderlordPackPens)))
                        {
                            if (l_Trigger->IsAIEnabled)
                                l_Trigger->AI()->DoAction(eActions::ActionCallThePack);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
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
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        break;
                    }
                    case eEvents::EventCallThePack:
                    {
                        me->CastSpell(me, eSpells::CallThePackCast, true);
                        m_Events.ScheduleEvent(eEvents::EventCallThePack, IsLFR() ? eTimers::TimerCallThePackLFRCooldown : eTimers::TimerCallThePackCooldown, ePhases::PhaseFerociousFeet);
                        break;
                    }
                    case eEvents::EventPinDown:
                    {
                        me->CastSpell(me, eSpells::PinDownSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventPinDown, eTimers::TimerPinDownCooldown, ePhases::PhaseFerociousFeet);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ResetBeasts()
            {
                if (m_Instance == nullptr)
                    return;

                std::vector<uint32> l_Beasts = { eFoundryCreatures::BossIroncrusher, eFoundryCreatures::BossDreadwing, eFoundryCreatures::BossCruelfang };

                for (uint32 l_Entry : l_Beasts)
                {
                    if (Creature* l_Beast = Creature::GetCreature(*me, m_Instance->GetData64(l_Entry)))
                    {
                        if (l_Beast->isDead())
                        {
                            l_Beast->DespawnOrUnsummon();
                            l_Beast->Respawn();

                            uint64 l_Guid = l_Beast->GetGUID();
                            AddTimedDelayedOperation(100, [this, l_Guid]() -> void
                            {
                                if (Creature* l_Creature = Creature::GetCreature(*me, l_Guid))
                                    l_Creature->GetMotionMaster()->MoveTargetedHome();
                            });
                        }
                        else if (l_Beast->IsAIEnabled)
                            l_Beast->AI()->EnterEvadeMode();
                    }
                }

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    std::list<Creature*> l_Hitchings;
                    me->GetCreatureListWithEntryInGrid(l_Hitchings, eCreatures::HitchingPost, 100.0f);

                    for (Creature* l_Iter : l_Hitchings)
                    {
                        if (l_Iter->IsAIEnabled)
                            l_Iter->AI()->Reset();
                    }
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_beastlord_darmacAI(p_Creature);
        }
};

/// Cruelfang - 76884
class npc_foundry_cruelfang : public CreatureScript
{
    public:
        npc_foundry_cruelfang() : CreatureScript("npc_foundry_cruelfang") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        struct npc_foundry_cruelfangAI : public ScriptedAI
        {
            npc_foundry_cruelfangAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
            }

            void UpdateAI(uint32 const p_Diff) override
            {
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
            return new npc_foundry_cruelfangAI(p_Creature);
        }
};

/// Dreadwing - 76874
class npc_foundry_dreadwing : public CreatureScript
{
    public:
        npc_foundry_dreadwing() : CreatureScript("npc_foundry_dreadwing") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        struct npc_foundry_dreadwingAI : public ScriptedAI
        {
            npc_foundry_dreadwingAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
            }

            void UpdateAI(uint32 const p_Diff) override
            {
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
            return new npc_foundry_dreadwingAI(p_Creature);
        }
};

/// Ironcrusher - 76945
class npc_foundry_ironcrusher : public CreatureScript
{
    public:
        npc_foundry_ironcrusher() : CreatureScript("npc_foundry_ironcrusher") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        struct npc_foundry_ironcrusherAI : public ScriptedAI
        {
            npc_foundry_ironcrusherAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
            }

            void UpdateAI(uint32 const p_Diff) override
            {
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
            return new npc_foundry_ironcrusherAI(p_Creature);
        }
};

/// Hitching Post - 79914
class npc_foundry_hitching_post : public CreatureScript
{
    public:
        npc_foundry_hitching_post() : CreatureScript("npc_foundry_hitching_post") { }

        enum eSpell
        {
            HitchingPostChain = 161300
        };

        struct npc_foundry_hitching_postAI : public ScriptedAI
        {
            npc_foundry_hitching_postAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpell::HitchingPostChain, false);
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_hitching_postAI(p_Creature);
        }
};

/// Heavy Spear - 76796
class npc_foundry_heavy_spear : public CreatureScript
{
    public:
        npc_foundry_heavy_spear() : CreatureScript("npc_foundry_heavy_spear") { }

        enum eSpells
        {
            PinDownVisualAura   = 155376,
            PinnedDownDamage    = 154960
        };

        struct npc_foundry_heavy_spearAI : public ScriptedAI
        {
            npc_foundry_heavy_spearAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            std::set<uint64> m_AffectedTargets;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->CastSpell(me, eSpells::PinDownVisualAura, true);
                me->CastSpell(me, eSpells::PinnedDownDamage, true);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::PinnedDownDamage)
                    m_AffectedTargets.insert(p_Target->GetGUID());
            }

            void JustDied(Unit* p_Killer) override
            {
                for (uint64 l_Guid : m_AffectedTargets)
                {
                    if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                        l_Player->RemoveAura(eSpells::PinnedDownDamage, me->GetGUID());
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_heavy_spearAI(p_Creature);
        }
};

/// Thunderlord Pack Pens - 77631
class npc_foundry_thunderlord_pack_pens : public CreatureScript
{
    public:
        npc_foundry_thunderlord_pack_pens() : CreatureScript("npc_foundry_thunderlord_pack_pens") { }

        enum eSpell
        {
            CallThePackSummon = 155668
        };

        enum eAction
        {
            ActionCallThePack
        };

        enum eData
        {
            CallThePackCount = 7
        };

        struct npc_foundry_thunderlord_pack_pensAI : public ScriptedAI
        {
            npc_foundry_thunderlord_pack_pensAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::ActionCallThePack && m_Instance != nullptr)
                {
                    if (GameObject* l_Door = GameObject::GetGameObject(*me, m_Instance->GetData64(eFoundryGameObjects::BeastsEnclosureDoor)))
                        l_Door->Use(me);

                    for (uint8 l_I = 0; l_I < eData::CallThePackCount; ++l_I)
                        me->CastSpell(me, eSpell::CallThePackSummon, true);
                }
            }

            void JustSummoned(Creature* p_Summon)
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Darmac = Creature::GetCreature(*me, m_Instance->GetData64(eFoundryCreatures::BossBeastlordDarmac)))
                {
                    if (Unit* l_Target = l_Darmac->getVictim())
                    {
                        if (p_Summon->IsAIEnabled)
                            p_Summon->AI()->AttackStart(l_Target);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_thunderlord_pack_pensAI(p_Creature);
        }
};

/// Hitching Post Chain - 161300
class spell_foundry_hitching_post_chain : public SpellScriptLoader
{
    public:
        spell_foundry_hitching_post_chain() : SpellScriptLoader("spell_foundry_hitching_post_chain") { }

        class spell_foundry_hitching_post_chain_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_hitching_post_chain_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || p_Object->IsPlayer())
                        return true;

                    if (p_Object->GetEntry() != eFoundryCreatures::BossIroncrusher &&
                        p_Object->GetEntry() != eFoundryCreatures::BossDreadwing &&
                        p_Object->GetEntry() != eFoundryCreatures::BossCruelfang)
                        return true;

                    return false;
                });

                if (p_Targets.empty())
                    return;

                p_Targets.sort(JadeCore::ObjectDistanceOrderPred(GetCaster()));

                if (p_Targets.size() > 1)
                {
                    WorldObject* l_Target = p_Targets.front();
                    p_Targets.clear();
                    p_Targets.push_back(l_Target);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_hitching_post_chain_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_hitching_post_chain_SpellScript();
        }
};

/// Pin Down (searcher) - 155365
class spell_foundry_pin_down_searcher : public SpellScriptLoader
{
    public:
        spell_foundry_pin_down_searcher() : SpellScriptLoader("spell_foundry_pin_down_searcher") { }

        class spell_foundry_pin_down_searcher_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_pin_down_searcher_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr || !p_Object->IsPlayer())
                        return true;

                    if (!p_Object->ToPlayer()->IsRangedDamageDealer())
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_pin_down_searcher_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_pin_down_searcher_SpellScript();
        }
};

void AddSC_boss_beastlord_darmac()
{
    /// Boss
    new boss_beastlord_darmac();

    /// Creatures
    new npc_foundry_cruelfang();
    new npc_foundry_dreadwing();
    new npc_foundry_ironcrusher();
    new npc_foundry_hitching_post();
    new npc_foundry_heavy_spear();
    new npc_foundry_thunderlord_pack_pens();

    /// Spells
    new spell_foundry_hitching_post_chain();
    new spell_foundry_pin_down_searcher();

    /// AreaTriggers
}