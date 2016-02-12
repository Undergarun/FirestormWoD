////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Kromog <Legend of the Mountain> - 77692
class boss_kromog : public CreatureScript
{
    public:
        boss_kromog() : CreatureScript("boss_kromog") { }

        enum eTalks
        {
        };

        enum eSpells
        {
            /// Misc
            StoneBreathChannel      = 156852,
            WarpedArmor             = 156766,
            Slam                    = 156704,
            /// Rippling Smash
            RipplingSmashSearcher   = 161049,
            RipplingSmashCast       = 157592
        };

        enum eEvents
        {
            EventCheckMeleePlayers = 1,
            EventBerserker,
            EventStoneBreath,
            EventWarpedArmor,
            EventSlam,
            EventRipplingSmash,
            EventGraspingEarth
        };

        enum eTimers
        {
            TimerCheckMeleePlayers  = 500,
            TimerBerserker          = 540 * TimeConstants::IN_MILLISECONDS,
            TimerStoneBreath        = 9 * TimeConstants::IN_MILLISECONDS,
            TimerStoneBreathAgain   = 24 * TimeConstants::IN_MILLISECONDS,
            TimerWarpedArmor        = 14 * TimeConstants::IN_MILLISECONDS,
            TimerSlam               = 18 * TimeConstants::IN_MILLISECONDS,
            TimerSlamAgain          = 23 * TimeConstants::IN_MILLISECONDS,
            TimerRipplingSmash      = 23 * TimeConstants::IN_MILLISECONDS + 500,
            TimerRipplingSmashAgain = 21 * TimeConstants::IN_MILLISECONDS,
            TimerGraspingEarth      = 50 * TimeConstants::IN_MILLISECONDS
        };

        enum eCosmeticEvents
        {
        };

        enum eActions
        {
        };

        enum eCreatures
        {
        };

        enum eVisuals
        {
        };

        struct boss_kromogAI : public BossAI
        {
            boss_kromogAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataKromog)
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

                summons.DespawnAll();

                _Reset();

                me->RemoveAllAreasTrigger();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

                me->RemoveAura(eFoundrySpells::Berserker);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                m_Events.ScheduleEvent(eEvents::EventCheckMeleePlayers, eTimers::TimerCheckMeleePlayers);
                m_Events.ScheduleEvent(eEvents::EventBerserker, eTimers::TimerBerserker);
                m_Events.ScheduleEvent(eEvents::EventStoneBreath, eTimers::TimerStoneBreath);
                m_Events.ScheduleEvent(eEvents::EventWarpedArmor, eTimers::TimerWarpedArmor);
                m_Events.ScheduleEvent(eEvents::EventSlam, eTimers::TimerSlam);
                m_Events.ScheduleEvent(eEvents::EventRipplingSmash, eTimers::TimerRipplingSmash);
            }

            void EnterEvadeMode() override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                CreatureAI::EnterEvadeMode();

                Reset();
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
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
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
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
                    case eEvents::EventCheckMeleePlayers:
                    {
                        Map::PlayerList const& l_PlayerList = me->GetMap()->GetPlayers();
                        if (l_PlayerList.isEmpty())
                        {
                            EnterEvadeMode();
                            break;
                        }

                        bool l_InMelee = false;
                        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
                        {
                            if (Player* l_Player = l_Iter->getSource())
                            {
                                if (l_Player->IsWithinMeleeRange(me))
                                {
                                    l_InMelee = true;
                                    break;
                                }
                            }
                        }

                        /// If there are no valid targets in melee range, Kromog will continue to breathe until he finds one.
                        if (!l_InMelee)
                            me->CastSpell(me, eSpells::StoneBreathChannel, false);

                        m_Events.ScheduleEvent(eEvents::EventCheckMeleePlayers, eTimers::TimerCheckMeleePlayers);
                        break;
                    }
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        break;
                    }
                    case eEvents::EventStoneBreath:
                    {
                        me->CastSpell(me, eSpells::StoneBreathChannel, false);
                        m_Events.ScheduleEvent(eEvents::EventStoneBreath, eTimers::TimerStoneBreathAgain);
                        break;
                    }
                    case eEvents::EventWarpedArmor:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::WarpedArmor, true);
                        m_Events.ScheduleEvent(eEvents::EventWarpedArmor, eTimers::TimerWarpedArmor);
                        break;
                    }
                    case eEvents::EventSlam:
                    {
                        me->CastSpell(me, eSpells::Slam, false);
                        m_Events.ScheduleEvent(eEvents::EventSlam, eTimers::TimerSlamAgain);
                        break;
                    }
                    case eEvents::EventRipplingSmash:
                    {
                        m_Events.ScheduleEvent(eEvents::EventRipplingSmash, eTimers::TimerRipplingSmashAgain);
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
            return new boss_kromogAI(p_Creature);
        }
};

/// Slam - 156704
class spell_foundry_slam : public SpellScriptLoader
{
    public:
        spell_foundry_slam() : SpellScriptLoader("spell_foundry_slam") { }

        class spell_foundry_slam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_slam_SpellScript);

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Boss = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        /// Kromog strikes the ground beneath his primary target, dealing up to 780000 Physical damage to all players, reduced based on their distance from the impact point.
                        float l_Distance = l_Target->GetDistance(l_Boss);
                        if (l_Distance <= 1.0f)
                            return;

                        int32 l_Damage = float(GetSpell()->GetDamage()) / l_Distance;

                        GetSpell()->SetDamage(l_Damage);
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_foundry_slam_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_slam_SpellScript();
        }
};

/// Fists of Stone - 162348
class spell_foundry_fists_of_stone : public SpellScriptLoader
{
    public:
        spell_foundry_fists_of_stone() : SpellScriptLoader("spell_foundry_fists_of_stone") { }

        enum eSpell
        {
            FistsOfStoneProc = 162349
        };

        class spell_foundry_fists_of_stone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_fists_of_stone_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() != nullptr)
                    return;

                if (Creature* l_Kromog = GetTarget()->ToCreature())
                {
                    if (l_Kromog->HasSpellCooldown(eSpell::FistsOfStoneProc))
                        return;

                    if (Unit* l_Target = p_EventInfo.GetActionTarget())
                    {
                        l_Kromog->CastSpell(l_Target, eSpell::FistsOfStoneProc, true);

                        uint32 l_Time = 2 * TimeConstants::IN_MILLISECONDS;
                        if (urand(0, 1))
                            l_Time *= 2;

                        l_Kromog->_AddCreatureSpellCooldown(eSpell::FistsOfStoneProc, l_Time);
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_foundry_fists_of_stone_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_fists_of_stone_AuraScript();
        }
};

void AddSC_boss_kromog()
{
    /// Boss
    new boss_kromog();

    /// Creatures

    /// Spells
    new spell_foundry_slam();
    new spell_foundry_fists_of_stone();
}