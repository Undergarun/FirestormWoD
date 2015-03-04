////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Jeron Emberfall <Warspear Tower Guardian> - 88178
class npc_jeron_emberfall : public CreatureScript
{
    public:
        npc_jeron_emberfall() : CreatureScript("npc_jeron_emberfall") { }

        struct npc_jeron_emberfallAI : public ScriptedAI
        {
            npc_jeron_emberfallAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CheckAroundingPlayersTimer = 0;
                m_PhoenixStrikeTimer = 0;
            }

            enum eSpells
            {
                Fireball                = 176652,
                Ignite                  = 176600,
                CombustionNova          = 176605,
                CombustionNovaStun      = 176608,
                LivingBomb              = 176670,
                SummonLavaFury          = 176664,

                TargetedByTheTowerMage  = 176076,
                PhoenixStrikeSearcher   = 176086,
                PhoenixStrikeMissile    = 176066
            };

            enum eTalk
            {
                TalkAggro,
                TalkSlay,
                TalkDeath,
                TalkSpell,
                TalkLivingBomb
            };

            enum eEvents
            {
                EventFireball = 1,
                EventIgnite,
                EventLivingBomb,
                EventCombustionNova
            };

            EventMap m_Events;

            uint32 m_CheckAroundingPlayersTimer;
            uint32 m_PhoenixStrikeTimer;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventFireball, 4000);
                m_Events.ScheduleEvent(eEvents::EventIgnite, 8000);
                m_Events.ScheduleEvent(eEvents::EventLivingBomb, 12000);
                m_Events.ScheduleEvent(eEvents::EventCombustionNova, 15000);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(eTalk::TalkDeath);
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo)
            {
                if (p_Victim == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::PhoenixStrikeSearcher:
                        me->CastSpell(p_Victim, eSpells::PhoenixStrikeMissile, false);
                        break;
                    case eSpells::CombustionNova:
                        if (p_Victim->HasAura(eSpells::Ignite))
                            me->CastSpell(p_Victim, eSpells::CombustionNovaStun, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                {
                    ScheduleTargetingPlayers(p_Diff);
                    SchedulePhoenixStrike(p_Diff);
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFireball:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::Fireball, false);
                        m_Events.ScheduleEvent(eEvents::EventFireball, 10000);
                        break;
                    case eEvents::EventIgnite:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::Ignite, true);
                        m_Events.ScheduleEvent(eEvents::EventIgnite, 9000);
                        break;
                    case eEvents::EventLivingBomb:
                        Talk(eTalk::TalkLivingBomb);
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::LivingBomb, false);
                        m_Events.ScheduleEvent(eEvents::EventLivingBomb, 15000);
                        break;
                    case eEvents::EventCombustionNova:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::CombustionNova, false);
                        m_Events.ScheduleEvent(eEvents::EventCombustionNova, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ScheduleTargetingPlayers(uint32 const p_Diff)
            {
                if (!m_CheckAroundingPlayersTimer)
                    return;

                if (m_CheckAroundingPlayersTimer <= p_Diff)
                {
                    m_CheckAroundingPlayersTimer = 2500;

                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 200.0f);

                    l_PlayerList.remove_if([this](Player* p_Player) -> bool
                    {
                        if (p_Player == nullptr)
                            return true;

                        if (!me->IsValidAttackTarget(p_Player))
                            return true;

                        if (p_Player->HasAura(eSpells::TargetedByTheTowerMage))
                            return true;

                        return false;
                    });

                    for (Player* l_Player : l_PlayerList)
                        l_Player->CastSpell(l_Player, eSpells::TargetedByTheTowerMage, true, nullptr, nullptr, me->GetGUID());
                }
                else
                    m_CheckAroundingPlayersTimer -= p_Diff;
            }

            void SchedulePhoenixStrike(uint32 const p_Diff)
            {
                if (!m_PhoenixStrikeTimer)
                    return;

                if (m_PhoenixStrikeTimer <= p_Diff)
                {
                    if (!me->isInCombat())
                        me->CastSpell(me, eSpells::PhoenixStrikeSearcher, true);
                    m_PhoenixStrikeTimer = 10000;
                }
                else
                    m_PhoenixStrikeTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_jeron_emberfallAI(p_Creature);
        }
};

/// Mare Wildrunner <Warspear Farseer> - 84660
class npc_ashran_mare_wildrunner : public CreatureScript
{
    public:
        npc_ashran_mare_wildrunner() : CreatureScript("npc_ashran_mare_wildrunner") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            return false;
        }
};

/// Angry Zurge - 83869
class npc_ashran_angry_zurge : public CreatureScript
{
    public:
        npc_ashran_angry_zurge() : CreatureScript("npc_ashran_angry_zurge") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            return false;
        }
};

/// Kalgan <Warspear Warrior Leader> - 83830
class npc_ashran_kalgan : public CreatureScript
{
    public:
        npc_ashran_kalgan() : CreatureScript("npc_ashran_kalgan") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            return false;
        }
};

/// Fura <Warspear Mage Leader> - 83995
class npc_ashran_fura : public CreatureScript
{
    public:
        npc_ashran_fura() : CreatureScript("npc_ashran_fura") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            return false;
        }
};

/// Nisstyr <Warspear Warlock Leader> - 83997
class npc_ashran_nisstyr : public CreatureScript
{
    public:
        npc_ashran_nisstyr() : CreatureScript("npc_ashran_nisstyr") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            return false;
        }
};

/// Atomik <Warspear Shaman Leader> - 82204
class npc_ashran_atomik : public CreatureScript
{
    public:
        npc_ashran_atomik() : CreatureScript("npc_ashran_atomik") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            return false;
        }
};

/// Warspear Shaman - 82438
class npc_ashran_warspear_shaman : public CreatureScript
{
    public:
        npc_ashran_warspear_shaman() : CreatureScript("npc_ashran_warspear_shaman") { }

        struct npc_ashran_warspear_shamanAI : public ScriptedAI
        {
            npc_ashran_warspear_shamanAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eDatas
            {
                EventCosmetic       = 1,
                EarthFury           = 82200,
                NatureChanneling    = 164850
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.ScheduleEvent(eDatas::EventCosmetic, 5000);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eDatas::EventCosmetic)
                {
                    if (Creature* l_EarthFury = me->FindNearestCreature(eDatas::EarthFury, 15.0f))
                        me->CastSpell(l_EarthFury, eDatas::NatureChanneling, false);
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_warspear_shamanAI(p_Creature);
        }
};

/// Illandria Belore - 88675
class npc_ashran_illandria_belore : public CreatureScript
{
    public:
        npc_ashran_illandria_belore() : CreatureScript("npc_ashran_illandria_belore") { }

        enum eTalks
        {
            First,
            Second,
            Third,
            Fourth,
            Fifth
        };

        enum eData
        {
            RahmFlameheart  = 88676,
            ActionInit      = 0,
            ActionLoop      = 1,
            EventLoop       = 1
        };

        struct npc_ashran_illandria_beloreAI : public MS::AI::CosmeticAI
        {
            npc_ashran_illandria_beloreAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            bool m_Init;
            EventMap m_Events;

            void Reset() override
            {
                m_Init = false;

                if (Creature* l_Creature = me->FindNearestCreature(eData::RahmFlameheart, 15.0f))
                {
                    if (l_Creature->AI())
                    {
                        m_Init = true;
                        l_Creature->AI()->DoAction(eData::ActionInit);
                        ScheduleAllTalks();
                    }
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eData::ActionInit:
                        if (m_Init)
                            break;
                        m_Init = true;
                        ScheduleAllTalks();
                        break;
                    default:
                        break;
                }
            }

            void ScheduleAllTalks(uint32 p_MoreTime = 0)
            {
                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::First); });
                AddTimedDelayedOperation(18 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Second); });
                AddTimedDelayedOperation(36 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Third); });
                AddTimedDelayedOperation(66 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Fourth); });
                AddTimedDelayedOperation(83 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Fifth); });
            }

            void LastOperationCalled() override
            {
                m_Events.ScheduleEvent(eData::EventLoop, 48 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eData::EventLoop)
                {
                    if (Creature* l_Creature = me->FindNearestCreature(eData::RahmFlameheart, 15.0f))
                    {
                        if (l_Creature->AI())
                        {
                            l_Creature->AI()->DoAction(eData::ActionLoop);
                            ScheduleAllTalks();
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_illandria_beloreAI(p_Creature);
        }
};

/// Examiner Rahm Flameheart <The Reliquary> - 88676
class npc_ashran_examiner_rahm_flameheart : public CreatureScript
{
    public:
        npc_ashran_examiner_rahm_flameheart() : CreatureScript("npc_ashran_examiner_rahm_flameheart") { }

        enum eTalks
        {
            First,
            Second,
            Third,
            Fourth
        };

        enum eData
        {
            IllandriaBelore = 88675,
            ActionInit      = 0,
            ActionLoop      = 1
        };

        struct npc_ashran_examiner_rahm_flameheartAI : public MS::AI::CosmeticAI
        {
            npc_ashran_examiner_rahm_flameheartAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            bool m_Init;

            void Reset()
            {
                m_Init = false;

                if (Creature* l_Creature = me->FindNearestCreature(eData::IllandriaBelore, 15.0f))
                {
                    if (l_Creature->AI())
                    {
                        m_Init = true;
                        l_Creature->AI()->DoAction(eData::ActionInit);
                        ScheduleAllTalks();
                    }
                }
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eData::ActionInit:
                        m_Init = true;
                        ScheduleAllTalks();
                        break;
                    case eData::ActionLoop:
                        ScheduleAllTalks();
                        break;
                    default:
                        break;
                }
            }

            void ScheduleAllTalks()
            {
                AddTimedDelayedOperation(10 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::First); });
                AddTimedDelayedOperation(27 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Second); });
                AddTimedDelayedOperation(75 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Third); });
                AddTimedDelayedOperation(92 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Fourth); });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_examiner_rahm_flameheartAI(p_Creature);
        }
};

/// Centurion Firescream <Warspear Tactician> - 88771
class npc_ashran_centurion_firescream : public CreatureScript
{
    public:
        npc_ashran_centurion_firescream() : CreatureScript("npc_ashran_centurion_firescream") { }

        struct npc_ashran_centurion_firescreamAI : public MS::AI::CosmeticAI
        {
            npc_ashran_centurion_firescreamAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(0); });
            }

            void LastOperationCalled() override
            {
                Reset();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_centurion_firescreamAI(p_Creature);
        }
};

/// Legionnaire Hellaxe <Warspear Strategist> - 88772
class npc_ashran_legionnaire_hellaxe : public CreatureScript
{
    public:
        npc_ashran_legionnaire_hellaxe() : CreatureScript("npc_ashran_legionnaire_hellaxe") { }

        struct npc_ashran_legionnaire_hellaxeAI : public MS::AI::CosmeticAI
        {
            npc_ashran_legionnaire_hellaxeAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                AddTimedDelayedOperation(30 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(0); });
            }

            void LastOperationCalled() override
            {
                Reset();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_legionnaire_hellaxeAI(p_Creature);
        }
};

void AddSC_AshranNPCHorde()
{
    new npc_jeron_emberfall();
    new npc_ashran_mare_wildrunner();
    new npc_ashran_angry_zurge();
    new npc_ashran_kalgan();
    new npc_ashran_fura();
    new npc_ashran_nisstyr();
    new npc_ashran_atomik();
    new npc_ashran_warspear_shaman();
    new npc_ashran_illandria_belore();
    new npc_ashran_examiner_rahm_flameheart();
    new npc_ashran_centurion_firescream();
    new npc_ashran_legionnaire_hellaxe();
}