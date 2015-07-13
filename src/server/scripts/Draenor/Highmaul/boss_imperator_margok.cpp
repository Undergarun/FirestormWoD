////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "highmaul.hpp"

/// Imperator Mar'gok <Sorcerer King> - 77428
class boss_imperator_margok : public CreatureScript
{
    public:
        boss_imperator_margok() : CreatureScript("boss_imperator_margok") { }

        enum eSpells
        {
            CosmeticSitThrone   = 88648
        };

        enum eEvents
        {
        };

        enum eActions
        {
            ActionIntro,
            ActionFinishIntro
        };

        enum eCreatures
        {
        };

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3
        };

        enum eAnimKit
        {
        };

        enum eMove
        {
        };

        enum eDatas
        {
        };

        struct boss_imperator_margokAI : public BossAI
        {
            boss_imperator_margokAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossImperatorMargok)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;

            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->CastSpell(me, eSpells::CosmeticSitThrone, true);
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
                    case eActions::ActionIntro:
                    {
                        AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Talk(eTalks::Intro1);
                        });

                        AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            Talk(eTalks::Intro2);
                        });

                        break;
                    }
                    case eActions::ActionFinishIntro:
                    {
                        Talk(eTalks::Intro3);
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

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);
            }

            void KilledUnit(Unit* p_Killed) override
            {
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

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
                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eHighmaulDatas::BossImperatorMargok, EncounterState::FAIL);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
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
            return new boss_imperator_margokAI(p_Creature);
        }
};

void AddSC_boss_imperator_margok()
{
    new boss_imperator_margok();
}