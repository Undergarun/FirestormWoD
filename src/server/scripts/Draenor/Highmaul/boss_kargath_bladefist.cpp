////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

Position const g_VulgorMovePos = { 3449.81f, 7557.01f, 55.304f, 0.8995f };

Position const g_SorcererPos[2] =
{
    { 3452.08f, 7550.25f, 55.304f, 0.8995f },
    { 3441.68f, 7558.45f, 55.304f, 0.8995f }
};

/// Vul'gor <The Shadow of Highmaul> - 80048
class npc_highmaul_vulgor : public CreatureScript
{
    public:
        npc_highmaul_vulgor() : CreatureScript("npc_highmaul_vulgor") { }

        enum eActions
        {
            StartIntro,
            ContinueIntro
        };

        enum eMove
        {
            MoveInArena
        };

        struct npc_highmaul_vulgorAI : public ScriptedAI
        {
            npc_highmaul_vulgorAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance          = p_Creature->GetInstanceScript();
                m_HealthPct         = 30;
                m_IntroContinued    = false;
                m_SorcererGuids[0]  = 0;
                m_SorcererGuids[1]  = 0;

                p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            }

            InstanceScript* m_Instance;
            int32 m_HealthPct;
            bool m_IntroContinued;

            uint64 m_SorcererGuids[2];

            void Reset() override
            {
                if (Creature* l_FirstSorcerer = Creature::GetCreature(*me, m_SorcererGuids[0]))
                {
                    l_FirstSorcerer->Respawn();
                    l_FirstSorcerer->GetMotionMaster()->Clear();
                    l_FirstSorcerer->GetMotionMaster()->MoveTargetedHome();
                }

                if (Creature* l_SecondSorcerer = Creature::GetCreature(*me, m_SorcererGuids[1]))
                {
                    l_SecondSorcerer->Respawn();
                    l_SecondSorcerer->GetMotionMaster()->Clear();
                    l_SecondSorcerer->GetMotionMaster()->MoveTargetedHome();
                }
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::StartIntro)
                {
                    std::list<Creature*> l_BladespireSorcerers;
                    me->GetCreatureListWithEntryInGrid(l_BladespireSorcerers, eHighmaulCreatures::BladespireSorcerer, 15.0f);

                    if (l_BladespireSorcerers.size() == 2)
                    {
                        uint8 l_Count = 0;
                        for (Creature* l_Sorcerer : l_BladespireSorcerers)
                        {
                            l_Sorcerer->SetWalk(true);
                            l_Sorcerer->GetMotionMaster()->MovePoint(eMove::MoveInArena, g_SorcererPos[l_Count]);
                            l_Sorcerer->SetHomePosition(g_SorcererPos[l_Count]);
                            m_SorcererGuids[l_Count] = l_Sorcerer->GetGUID();

                            ++l_Count;
                        }
                    }

                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(eMove::MoveInArena, g_VulgorMovePos);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (m_Instance == nullptr || p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMove::MoveInArena:
                    {
                        me->SetWalk(false);
                        me->SetHomePosition(g_VulgorMovePos);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_STATE_READY1H);
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_IntroContinued || m_Instance == nullptr)
                    return;

                if (me->HealthBelowPctDamaged(m_HealthPct, p_Damage))
                {
                    m_IntroContinued = true;

                    if (Creature* l_Jhorn = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::JhornTheMad)))
                        l_Jhorn->AI()->DoAction(eActions::ContinueIntro);

                    if (Creature* l_Thoktar = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::ThoktarIronskull)))
                        l_Thoktar->AI()->DoAction(eActions::ContinueIntro);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_vulgorAI(p_Creature);
        }
};

/// Bladespire Sorcerer - 80071
class npc_highmaul_bladespire_sorcerer : public CreatureScript
{
    public:
        npc_highmaul_bladespire_sorcerer() : CreatureScript("npc_highmaul_bladespire_sorcerer") { }

        enum eMove
        {
            MoveInArena
        };

        struct npc_highmaul_bladespire_sorcererAI : public ScriptedAI
        {
            npc_highmaul_bladespire_sorcererAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMove::MoveInArena:
                    {
                        me->SetWalk(false);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_STATE_READY2H);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_bladespire_sorcererAI(p_Creature);
        }
};

/// Fire Pillar - 78757
class npc_highmaul_fire_pillar : public CreatureScript
{
    public:
        npc_highmaul_fire_pillar() : CreatureScript("npc_highmaul_fire_pillar") { }

        struct npc_highmaul_fire_pillarAI : public ScriptedAI
        {
            npc_highmaul_fire_pillarAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_fire_pillarAI(p_Creature);
        }
};

/// Ravenous Bloodmaw - 79296
class npc_highmaul_ravenous_bloodmaw : public CreatureScript
{
    public:
        npc_highmaul_ravenous_bloodmaw() : CreatureScript("npc_highmaul_ravenous_bloodmaw") { }

        struct npc_highmaul_ravenous_bloodmawAI : public ScriptedAI
        {
            npc_highmaul_ravenous_bloodmawAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_ravenous_bloodmawAI(p_Creature);
        }
};

void AddSC_boss_kargath_bladefist()
{
    /// Boss

    /// NPCs
    new npc_highmaul_vulgor();
    new npc_highmaul_bladespire_sorcerer();
    new npc_highmaul_fire_pillar();
    new npc_highmaul_ravenous_bloodmaw();
}