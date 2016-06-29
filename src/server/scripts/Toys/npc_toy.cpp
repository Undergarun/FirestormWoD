////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "ScriptedCreature.h"

/// Mini Mana Bomb - 65903
class npc_toy_mini_maba_bomb : public CreatureScript
{
    public:
        npc_toy_mini_maba_bomb() : CreatureScript("npc_toy_mini_maba_bomb") { }

        enum eSpell
        {
            ArcaneExplosionVisualLarge = 98122
        };

        struct npc_toy_mini_maba_bombAI : public ScriptedAI
        {
            npc_toy_mini_maba_bombAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS + 900, [this]() -> void
                {
                    me->CastSpell(me, eSpell::ArcaneExplosionVisualLarge, true);
                    me->DespawnOrUnsummon();
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_toy_mini_maba_bombAI(p_Creature);
        }
};

#ifndef CROSS
/// Wind-Up Train Wrecker - 33404
class npc_train_wrecker : public CreatureScript
{
    public:
        npc_train_wrecker() : CreatureScript("npc_train_wrecker") { }

        enum eWreckerDatas
        {
            GoToyTrain                  = 193963,
            ToyTrainPulse               = 61551,
            WreckTrain                  = 62943,
            ExultingWrecker             = 81071,
            ThogarTrain                 = 76907,
            TheresAlwaysABiggerTrain    = 8982,
            /// Action
            ActionWrecked = 1,
            /// Events
            EventDoJump = 1,
            EventDoFacing,
            EventDoWreck,
            EventDoDance,
            EventThogarAchiev,
            /// Movements
            MoveChase = 1,
            MoveJump
        };

        struct npc_train_wreckerAI : public NullCreatureAI
        {
            npc_train_wreckerAI(Creature* p_Creature) : NullCreatureAI(p_Creature)
            {
                m_IsSearching   = true;
                m_NextAction    = 0;
                m_Timer         = 1 * TimeConstants::IN_MILLISECONDS;
            }

            bool m_IsSearching;
            uint8 m_NextAction;
            uint32 m_Timer;
            uint64 m_TargetGuid;

            GameObject* VerifyTarget() const
            {
                if (GameObject* l_Target = GameObject::GetGameObject(*me, m_TargetGuid))
                    return l_Target;

                me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_RUDE);
                me->DespawnOrUnsummon(3 * TimeConstants::IN_MILLISECONDS);
                return nullptr;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_IsSearching)
                {
                    if (p_Diff < m_Timer)
                        m_Timer -= p_Diff;
                    else
                    {
                        if (GameObject* l_Target = me->FindNearestGameObject(eWreckerDatas::GoToyTrain, 15.0f))
                        {
                            m_IsSearching = false;
                            m_TargetGuid = l_Target->GetGUID();
                            me->SetWalk(true);

                            Position l_Pos;
                            l_Target->GetNearPosition(l_Pos, 3.0f, l_Target->GetAngle(me));
                            me->GetMotionMaster()->MovePoint(eWreckerDatas::MoveChase, l_Pos);
                        }
                        else
                            m_Timer = 3 * TimeConstants::IN_MILLISECONDS;
                    }
                }
                else
                {
                    switch (m_NextAction)
                    {
                        case eWreckerDatas::EventDoJump:
                        {
                            if (GameObject* l_Target = VerifyTarget())
                                me->GetMotionMaster()->MoveJump(*l_Target, 5.0, 10.0, eWreckerDatas::MoveJump);

                            m_NextAction = 0;
                            break;
                        }
                        case eWreckerDatas::EventDoFacing:
                        {
                            if (GameObject* l_Target = VerifyTarget())
                            {
                                me->SetFacingTo(l_Target->GetOrientation());
                                me->HandleEmoteCommand(Emote::EMOTE_ONESHOT_ATTACK1H);
                                m_Timer = uint32(1.5f * TimeConstants::IN_MILLISECONDS);
                                m_NextAction = eWreckerDatas::EventDoWreck;
                            }
                            else
                                m_NextAction = 0;

                            break;
                        }
                        case eWreckerDatas::EventDoWreck:
                        {
                            if (p_Diff < m_Timer)
                            {
                                m_Timer -= p_Diff;
                                break;
                            }

                            if (GameObject* l_Target = VerifyTarget())
                            {
                                me->CastSpell(l_Target, eWreckerDatas::WreckTrain, false);
                                l_Target->Delete();
                                m_Timer = 2 * TimeConstants::IN_MILLISECONDS;
                                m_NextAction = eWreckerDatas::EventDoDance;
                            }
                            else
                                m_NextAction = 0;

                            break;
                        }
                        case eWreckerDatas::EventDoDance:
                        {
                            if (p_Diff < m_Timer)
                            {
                                m_Timer -= p_Diff;
                                break;
                            }

                            me->UpdateEntry(eWreckerDatas::ExultingWrecker);
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_ONESHOT_DANCE);
                            me->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
                            m_NextAction = eWreckerDatas::EventThogarAchiev;
                            break;
                        }
                        case eWreckerDatas::EventThogarAchiev:
                        {
                            if (GameObject* l_Train = me->FindNearestGameObject(eWreckerDatas::ThogarTrain, 10.0f))
                            {
                                if (InstanceScript* l_InstanceScript = l_Train->GetInstanceScript())
                                {
                                    l_InstanceScript->DoCompleteAchievement(eWreckerDatas::TheresAlwaysABiggerTrain);
                                    me->DespawnOrUnsummon();
                                    break;
                                }
                            }

                            m_NextAction = eWreckerDatas::EventThogarAchiev;
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID) override
            {
                switch (p_ID)
                {
                    case eWreckerDatas::MoveChase:
                        m_NextAction = eWreckerDatas::EventDoJump;
                        break;
                    case eWreckerDatas::MoveJump:
                        m_NextAction = eWreckerDatas::EventDoFacing;
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_train_wreckerAI(creature);
        }
};

#endif /* not CROSS */
#ifndef __clang_analyzer__
void AddSC_npc_toy()
{
    new npc_toy_mini_maba_bomb();
#ifndef CROSS
    new npc_train_wrecker();
#endif /* not CROSS */
}
#endif
