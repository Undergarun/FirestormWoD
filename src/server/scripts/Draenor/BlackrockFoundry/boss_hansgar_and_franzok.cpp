////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Hans'gar - 76973
class boss_hansgar : public CreatureScript
{
    public:
        boss_hansgar() : CreatureScript("boss_hansgar") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3
        };

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        enum eCosmeticEvents
        {
        };

        enum eActions
        {
            ActionIntro
        };

        enum eVisuals
        {
        };

        enum eCreatures
        {
        };

        enum eDatas
        {
        };

        struct boss_hansgarAI : public BossAI
        {
            boss_hansgarAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataHansgarAndFranzok)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            InstanceScript* m_Instance;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            bool m_IntroDone;

            void Reset() override
            {
                m_Events.Reset();
                m_CosmeticEvents.Reset();

                _Reset();

                me->RemoveAllAreasTrigger();
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
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo) override
            {
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionIntro:
                    {
                        if (m_IntroDone)
                            break;

                        m_IntroDone = true;

                        Talk(eTalks::Intro1);

                        uint32 l_Time = 11 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro2);
                        });

                        l_Time += 10 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro3);

                            /// Launch trashes
                        });

                        break;
                    }
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

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
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
            return new boss_hansgarAI(p_Creature);
        }
};

/// Franzok - 76974
class boss_franzok : public CreatureScript
{
    public:
        boss_franzok() : CreatureScript("boss_franzok") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3
        };

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        enum eCosmeticEvents
        {
        };

        enum eActions
        {
            ActionIntro
        };

        enum eVisuals
        {
        };

        enum eCreatures
        {
        };

        enum eDatas
        {
        };

        struct boss_franzokAI : public BossAI
        {
            boss_franzokAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataHansgarAndFranzok)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            InstanceScript* m_Instance;

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            bool m_IntroDone;

            void Reset() override
            {
                m_Events.Reset();
                m_CosmeticEvents.Reset();

                _Reset();

                me->RemoveAllAreasTrigger();
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
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                summons.DespawnAll();

                _JustDied();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Attacker, SpellInfo const* p_SpellInfo) override
            {
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionIntro:
                    {
                        if (m_IntroDone)
                            break;

                        m_IntroDone = true;

                        uint32 l_Time = 5 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro1);
                        });

                        l_Time += 11 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro2);
                        });

                        l_Time += 11 * TimeConstants::IN_MILLISECONDS;
                        AddTimedDelayedOperation(l_Time, [this]() -> void
                        {
                            Talk(eTalks::Intro3);
                        });

                        break;
                    }
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

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                switch (p_ID)
                {
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
            return new boss_franzokAI(p_Creature);
        }
};

void AddSC_boss_hansgar_and_franzok()
{
    /// Bosses
    new boss_hansgar();
    new boss_franzok();
}