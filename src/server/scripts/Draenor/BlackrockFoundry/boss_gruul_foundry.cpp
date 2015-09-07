////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Gruul - 76877
class boss_gruul_foundry : public CreatureScript
{
    public:
        boss_gruul_foundry() : CreatureScript("boss_gruul_foundry") { }

        enum eSpells
        {
            /// Misc
            GruulBonus              = 177529,
            RageRegenerationDisable = 173204,
            RageRegenerationAura    = 155534,
            /// Inferno Slice
            InfernoSlice            = 155080,
            InfernoStrike           = 162322,
            /// Overwhelming Blows
            OverwhelmingBlowsAura   = 155077,
            OverwhelmingBlowsProc   = 155078,
            /// Destructive Rampage
            DestructiveRampage      = 155539
        };

        enum eEvents
        {
            EventInfernoSlice = 1,
            EventOverwhelmingBlows,
            EventDestructiveRampage,
            EventEndOfDestructiveRampage,
            EventBerserker
        };

        enum eActions
        {
            ActionInfernoSlice
        };

        enum eCreatures
        {
        };

        enum eTalks
        {
            Intro,
            Aggro,
            Petrify,
            DestructiveRampage,
            Berserk,
            Slay,
            Death,
            DestructiveRampageStart,
            DestructiveRampageEnd
        };

        struct boss_gruul_foundryAI : public BossAI
        {
            boss_gruul_foundryAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataGruul)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            EventMap m_Events;

            bool m_IntroDone;

            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                summons.DespawnAll();

                me->RemoveAura(eFoundrySpells::Berserker);
                me->RemoveAura(eSpells::RageRegenerationAura);

                me->CastSpell(me, eSpells::OverwhelmingBlowsAura, true);
                me->CastSpell(me, eSpells::RageRegenerationDisable, true);

                _Reset();
            }

            void JustReachedHome() override
            {
                m_Events.Reset();
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionInfernoSlice:
                    {
                        if (m_Events.HasEvent(eEvents::EventInfernoSlice))
                            break;

                        m_Events.ScheduleEvent(eEvents::EventInfernoSlice, 50);
                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                Talk(eTalks::Aggro);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                me->CastSpell(me, eSpells::RageRegenerationAura, true);

                m_Events.ScheduleEvent(eEvents::EventOverwhelmingBlows, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDestructiveRampage, 100 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventEndOfDestructiveRampage, 130 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventBerserker, (IsMythic() || IsHeroic()) ? 360 * TimeConstants::IN_MILLISECONDS : 480 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                m_Events.Reset();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    CastSpellToPlayers(me->GetMap(), me, eSpells::GruulBonus, true);

                    if (IsLFR())
                    {
                        Map::PlayerList const& l_PlayerList = m_Instance->instance->GetPlayers();
                        if (l_PlayerList.isEmpty())
                            return;

                        for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                        {
                            if (Player* l_Player = l_Itr->getSource())
                            {
                                uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                                if (!me || l_Player->IsAtGroupRewardDistance(me))
                                    sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);
                            }
                        }

                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }
            }

            void EnterEvadeMode() override
            {
                me->RemoveAllAuras();

                me->InterruptNonMeleeSpells(true);

                /// Just in case, to prevent the fail Return to Home
                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);
                me->ClearUnitState(UnitState::UNIT_STATE_DISTRACTED);
                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);

                CreatureAI::EnterEvadeMode();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eFoundryDatas::DataGruul, EncounterState::FAIL);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (m_IntroDone)
                    return;

                if (p_Who->GetDistance(me) > 60.0f)
                    return;

                Talk(eTalks::Intro);
                m_IntroDone = true;
            }

            uint32 GetData(uint32 p_ID) override
            {
                return 0;
            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::InfernoSlice:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::InfernoStrike, true);

                        break;
                    }
                    default:
                        break;
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;
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
                    case eEvents::EventInfernoSlice:
                    {
                        me->SetPower(Powers::POWER_ENERGY, 0);
                        me->CastSpell(me, eSpells::InfernoSlice, false);
                        break;
                    }
                    case eEvents::EventOverwhelmingBlows:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::OverwhelmingBlowsProc, true);

                        m_Events.ScheduleEvent(eEvents::EventOverwhelmingBlows, 3 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDestructiveRampage:
                    {
                        Talk(eTalks::DestructiveRampage);
                        Talk(eTalks::DestructiveRampageStart, me->GetGUID());

                        me->CastSpell(me, eSpells::DestructiveRampage, true);
                        break;
                    }
                    case eEvents::EventEndOfDestructiveRampage:
                    {
                        Talk(eTalks::DestructiveRampageEnd, me->GetGUID());

                        me->RemoveAura(eSpells::DestructiveRampage);
                        break;
                    }
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        Talk(eTalks::Berserk);
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
            return new boss_gruul_foundryAI(p_Creature);
        }
};

/// Rage Regeneration - 155534
class spell_foundry_rage_regeneration : public SpellScriptLoader
{
    public:
        spell_foundry_rage_regeneration() : SpellScriptLoader("spell_foundry_rage_regeneration") { }

        class spell_foundry_rage_regeneration_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_rage_regeneration_AuraScript);

            enum eAction
            {
                ActionInfernoSlice
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (GetTarget() == nullptr)
                    return;

                if (Creature* l_Gruul = GetTarget()->ToCreature())
                {
                    if (!l_Gruul->IsAIEnabled)
                        return;

                    l_Gruul->EnergizeBySpell(l_Gruul, GetSpellInfo()->Id, 3, Powers::POWER_ENERGY);

                    if (l_Gruul->GetPower(Powers::POWER_ENERGY) >= 100)
                        l_Gruul->AI()->DoAction(eAction::ActionInfernoSlice);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_foundry_rage_regeneration_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_rage_regeneration_AuraScript();
        }
};

/// Inferno Slice - 155080
class spell_foundry_inferno_slice : public SpellScriptLoader
{
    public:
        spell_foundry_inferno_slice() : SpellScriptLoader("spell_foundry_inferno_slice") { }

        class spell_foundry_inferno_slice_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_inferno_slice_SpellScript);

            enum eSpell
            {
                TargetRestrict = 19114
            };

            uint8 m_TargetCount;

            bool Load() override
            {
                m_TargetCount = 0;
                return true;
            }

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });

                m_TargetCount = (uint8)p_Targets.size();
            }

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                if (m_TargetCount)
                    SetHitDamage(GetHitDamage() / m_TargetCount);
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_foundry_inferno_slice_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                OnEffectHitTarget += SpellEffectFn(spell_foundry_inferno_slice_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_inferno_slice_SpellScript();
        }
};

void AddSC_boss_gruul_foundry()
{
    /// Boss
    new boss_gruul_foundry();

    /// Spells
    new spell_foundry_rage_regeneration();
    new spell_foundry_inferno_slice();
}