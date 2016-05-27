////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "boss_operator_thogar.hpp"

/// Operator Thogar <Grimrail Overseer> - 76906
class boss_operator_thogar : public CreatureScript
{
    public:
        boss_operator_thogar() : CreatureScript("boss_operator_thogar") { }

        enum eEvents
        {
            EventBerserker = 1
        };

        enum eTimers
        {
            TimerBerserker = 8 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS
        };

        struct boss_operator_thogarAI : public BossAI
        {
            boss_operator_thogarAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataOperatorThogar) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                Talk(eThogarTalks::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventBerserker, eTimers::TimerBerserker);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                Talk(eThogarTalks::TalkDeath);
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                Talk(eThogarTalks::TalkWipe);
            }

            void DoAction(int32 const /*p_Action*/) override
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
                        Talk(eThogarTalks::TalkBerserk);
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
            return new boss_operator_thogarAI(p_Creature);
        }
};

/// Train - 76907
class npc_foundry_train_controller : public CreatureScript
{
    public:
        npc_foundry_train_controller() : CreatureScript("npc_foundry_train_controller") { }

        enum eMove
        {
            MovementIntro = 5
        };

        enum eActions
        {
            IntroBegin,
            IntroEnd
        };

        struct npc_foundry_train_controllerAI : public ScriptedAI
        {
            npc_foundry_train_controllerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_Vehicle   = p_Creature->GetVehicleKit();
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            uint64 m_SummonerGUID;

            void Reset() override
            {
                m_SummonerGUID = 0;

                me->AddUnitState(UnitState::UNIT_STATE_IGNORE_PATHFINDING);
            }

            void DoAction(int32 const p_Action) override
            {
                /// Move intro train
                if (p_Action == eActions::IntroBegin && m_Vehicle != nullptr && m_Instance != nullptr)
                {
                    for (int8 l_I = 0; l_I < 4; ++l_I)
                    {
                        if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                        {
                            l_Passenger->RemoveAura(eThogarSpells::StoppedFrontAura);

                            l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingTrain, true);
                            l_Passenger->CastSpell(l_Passenger, eThogarSpells::MovingFrontAura, true);
                        }
                    }

                    me->GetMotionMaster()->MovePoint(eMove::MovementIntro, g_TrainTrackIntroEndPos, false);

                    if (GameObject* l_RightDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[eThogarMiscDatas::FourthTrack].RightDoor)))
                        l_RightDoor->SetGoState(GOState::GO_STATE_ACTIVE);

                    if (GameObject* l_LeftDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_TrackDoors[eThogarMiscDatas::FourthTrack].LeftDoor)))
                        l_LeftDoor->SetGoState(GOState::GO_STATE_ACTIVE);
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
            {
                m_SummonerGUID = p_Guid;
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_ID == eMove::MovementIntro && m_Vehicle != nullptr && m_Instance != nullptr)
                {
                    for (int8 l_I = 0; l_I < 4; ++l_I)
                    {
                        if (Unit* l_Passenger = m_Vehicle->GetPassenger(l_I))
                        {
                            l_Passenger->RemoveAura(eThogarSpells::MovingFrontAura);
                            l_Passenger->RemoveAura(eThogarSpells::MovingTrain);

                            l_Passenger->CastSpell(l_Passenger, eThogarSpells::StoppedFrontAura, true);
                        }
                    }

                    if (Unit* l_TroopTransport = m_Vehicle->GetPassenger(1))
                    {
                        l_TroopTransport->CastSpell(l_TroopTransport, eThogarSpells::TroopTransportOpen, true);

                        if (Creature* l_Summoner = Creature::GetCreature(*me, m_SummonerGUID))
                        {
                            if (Vehicle* l_Transport = l_TroopTransport->GetVehicleKit())
                            {
                                for (int8 l_I = 0; l_I < 8; ++l_I)
                                {
                                    if (Unit* l_Passenger = l_Transport->GetPassenger(l_I))
                                        l_Passenger->ExitVehicle(l_Summoner);
                                }
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_train_controllerAI(p_Creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_operator_thogar()
{
    /// Boss
    new boss_operator_thogar();

    /// Creatures
    new npc_foundry_train_controller();
}
#endif
