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
                PhoenixStrikeMissile    = 176066,

                ConjureRefreshment      = 176351
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

            void Reset() override
            {
                m_Events.Reset();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventFireball, 4000);
                m_Events.ScheduleEvent(eEvents::EventIgnite, 8000);
                m_Events.ScheduleEvent(eEvents::EventLivingBomb, 12000);
                m_Events.ScheduleEvent(eEvents::EventCombustionNova, 15000);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalk::TalkDeath);
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo) override
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

            void UpdateAI(uint32 const p_Diff) override
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

                EnterEvadeIfOutOfCombatArea(p_Diff);
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

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                p_Player->PlayerTalkClass->SendCloseGossip();
                me->CastSpell(p_Player, eSpells::ConjureRefreshment, false);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_jeron_emberfallAI(p_Creature);
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

            void Reset() override
            {
                m_Events.ScheduleEvent(eDatas::EventCosmetic, 5000);
            }

            void UpdateAI(uint32 const p_Diff) override
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

        CreatureAI* GetAI(Creature* p_Creature) const override
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

            void ScheduleAllTalks()
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

        CreatureAI* GetAI(Creature* p_Creature) const override
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

            void Reset() override
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

            void DoAction(int32 const p_Action) override
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

        CreatureAI* GetAI(Creature* p_Creature) const override
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

        CreatureAI* GetAI(Creature* p_Creature) const override
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

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_legionnaire_hellaxeAI(p_Creature);
        }
};

/// Kalgan <Warspear Warrior Leader> - 83830
class npc_ashran_kalgan : public CreatureScript
{
    public:
        npc_ashran_kalgan() : CreatureScript("npc_ashran_kalgan") { }

        enum eGossip
        {
            RidersSummoned = 84923
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(p_Creature->GetZoneId());
            if (l_ZoneScript == nullptr)
                return false;

            if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
            {
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_HORDE, eArtifactsDatas::CountForWarriorPaladin))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::RidersSummoned, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_kalganAI : public ScriptedAI
        {
            npc_ashran_kalganAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                /// "Take all of my Artifact Fragments" is always 0
                if (p_Action)
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, true);
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount * CURRENCY_PRECISION), false);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                {
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, eArtifactsDatas::CountForWarriorPaladin, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_kalganAI(p_Creature);
        }
};

/// Fura <Warspear Mage Leader> - 83995
class npc_ashran_fura : public CreatureScript
{
    public:
        npc_ashran_fura() : CreatureScript("npc_ashran_fura") { }

        enum eGossip
        {
            PortalInvoked = 84919
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(p_Creature->GetZoneId());
            if (l_ZoneScript == nullptr)
                return false;

            if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
            {
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_HORDE, eArtifactsDatas::CountForMage))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::PortalInvoked, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_furaAI : public ScriptedAI
        {
            npc_ashran_furaAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                /// "Take all of my Artifact Fragments" is always 0
                if (p_Action)
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, true);
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount * CURRENCY_PRECISION), false);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                {
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, eArtifactsDatas::CountForMage, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_furaAI(p_Creature);
        }
};

/// Nisstyr <Warspear Warlock Leader> - 83997
class npc_ashran_nisstyr : public CreatureScript
{
    public:
        npc_ashran_nisstyr() : CreatureScript("npc_ashran_nisstyr") { }

        enum eGossip
        {
            GatewaysInvoked = 85463
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(p_Creature->GetZoneId());
            if (l_ZoneScript == nullptr)
                return false;

            if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
            {
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_HORDE, eArtifactsDatas::CountForWarlock))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::GatewaysInvoked, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_nisstyrAI : public ScriptedAI
        {
            npc_ashran_nisstyrAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                /// "Take all of my Artifact Fragments" is always 0
                if (p_Action)
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, true);
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount * CURRENCY_PRECISION), false);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                {
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, eArtifactsDatas::CountForWarlock, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_nisstyrAI(p_Creature);
        }
};

/// Atomik <Warspear Shaman Leader> - 82204
class npc_ashran_atomik : public CreatureScript
{
    public:
        npc_ashran_atomik() : CreatureScript("npc_ashran_atomik") { }

        enum eGossip
        {
            KronusInvoked = 89853
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(p_Creature->GetZoneId());
            if (l_ZoneScript == nullptr)
                return false;

            if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
            {
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_HORDE, eArtifactsDatas::CountForDruidShaman))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::KronusInvoked, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_atomikAI : public ScriptedAI
        {
            npc_ashran_atomikAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                /// "Take all of my Artifact Fragments" is always 0
                if (p_Action)
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                uint32 l_ArtifactCount = p_Player->GetCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, true);
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount * CURRENCY_PRECISION), false);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                {
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_HORDE, eArtifactsDatas::CountForDruidShaman, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_atomikAI(p_Creature);
        }
};

/// Zaram Sunraiser <Portal Guardian> - 84468
class npc_ashran_zaram_sunraiser : public CreatureScript
{
    public:
        npc_ashran_zaram_sunraiser() : CreatureScript("npc_ashran_zaram_sunraiser") { }

        enum eSpells
        {
            SpellMoltenArmor    = 79849,
            SpellFlamestrike    = 79856,
            SpellFireball       = 79854,
            SpellBlastWave      = 79857
        };

        enum eEvents
        {
            EventFlamestrike = 1,
            EventFireball,
            EventBlastWave
        };

        struct npc_ashran_zaram_sunraiserAI : public ScriptedAI
        {
            npc_ashran_zaram_sunraiserAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                me->CastSpell(me, eSpells::SpellMoltenArmor, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->CastSpell(me, eSpells::SpellMoltenArmor, true);

                m_Events.ScheduleEvent(eEvents::EventFlamestrike, 7000);
                m_Events.ScheduleEvent(eEvents::EventFireball, 3000);
                m_Events.ScheduleEvent(eEvents::EventBlastWave, 9000);
            }

            void JustDied(Unit* p_Killer) override
            {
                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                    l_Ashran->EndArtifactEvent(TeamId::TEAM_HORDE, eArtifactsDatas::CountForMage);
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
                    case eEvents::EventFlamestrike:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellFlamestrike, false);
                        m_Events.ScheduleEvent(eEvents::EventFlamestrike, 15000);
                        break;
                    case eEvents::EventFireball:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellFireball, false);
                        m_Events.ScheduleEvent(eEvents::EventFireball, 10000);
                        break;
                    case eEvents::EventBlastWave:
                        me->CastSpell(me, eSpells::SpellBlastWave, false);
                        m_Events.ScheduleEvent(eEvents::EventBlastWave, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_zaram_sunraiserAI(p_Creature);
        }
};

/// Gayle Plagueheart <Gateway Guardian> - 84645
/// Ilya Plagueheart <Gateway Guardian> - 84646
class npc_ashran_horde_gateway_guardian : public CreatureScript
{
    public:
        npc_ashran_horde_gateway_guardian() : CreatureScript("npc_ashran_horde_gateway_guardian") { }

        enum eSpells
        {
            SpellChaosBolt  = 79939,
            SpellFelArmor   = 165735,
            SpellImmolate   = 79937,
            SpellIncinerate = 79938
        };

        enum eEvents
        {
            EventChaosBolt = 1,
            EventImmolate,
            EventIncinerate
        };

        struct npc_ashran_horde_gateway_guardianAI : public ScriptedAI
        {
            npc_ashran_horde_gateway_guardianAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                me->CastSpell(me, eSpells::SpellFelArmor, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->CastSpell(me, eSpells::SpellFelArmor, true);

                m_Events.ScheduleEvent(eEvents::EventImmolate, 1000);
                m_Events.ScheduleEvent(eEvents::EventIncinerate, 2000);
                m_Events.ScheduleEvent(eEvents::EventChaosBolt, 5000);
            }

            void JustDied(Unit* p_Killer) override
            {
                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                    l_Ashran->EndArtifactEvent(TeamId::TEAM_HORDE, eArtifactsDatas::CountForWarlock);
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
                    case eEvents::EventChaosBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellChaosBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventChaosBolt, 12000);
                        break;
                    case eEvents::EventImmolate:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellImmolate, false);
                        m_Events.ScheduleEvent(eEvents::EventImmolate, 9000);
                        break;
                    case eEvents::EventIncinerate:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellIncinerate, false);
                        m_Events.ScheduleEvent(eEvents::EventIncinerate, 5000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_horde_gateway_guardianAI(p_Creature);
        }
};

/// Kronus <Horde Guardian> - 82201
class npc_ashran_kronus : public CreatureScript
{
    public:
        npc_ashran_kronus() : CreatureScript("npc_ashran_kronus") { }

        enum eSpells
        {
            AshranLaneMobScalingAura    = 164310,

            SpellFractureSearcher       = 170892,
            SpellFractureMissile        = 170893,   ///< Trigger 170894
            SpellGroundPound            = 170905,   ///< Periodic Trigger 177605
            SpellRockShield             = 175058,
            SpellStoneEmpowermentAura   = 170896
        };

        enum eEvents
        {
            EventFracture = 1,
            EventGroundPound,
            EventRockShield,
            EventMove
        };

        struct npc_ashran_kronusAI : public ScriptedAI
        {
            npc_ashran_kronusAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1000);

                /// Kronus no longer scales their health based the number of players he's fighting.
                /// Each faction guardian's health now scales based on the number of enemy players active at the time when they're summoned.
                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                uint32 l_PlayerCount = 0;
                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                    l_PlayerCount = l_Ashran->CountPlayersForTeam(TeamId::TEAM_ALLIANCE);

                if (AuraPtr l_Scaling = me->AddAura(eSpells::AshranLaneMobScalingAura, me))
                {
                    if (AuraEffectPtr l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_PlayerCount);
                    if (AuraEffectPtr l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_PlayerCount);
                }
            }

            void Reset() override
            {
                me->DisableHealthRegen();
                me->CastSpell(me, eSpells::SpellStoneEmpowermentAura, true);

                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Position l_Pos;
                me->GetPosition(&l_Pos);
                me->SetHomePosition(l_Pos);

                m_Events.ScheduleEvent(eEvents::EventFracture, 5000);
                m_Events.ScheduleEvent(eEvents::EventGroundPound, 12000);
                m_Events.ScheduleEvent(eEvents::EventRockShield, 9000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (p_Damage < me->GetHealth())
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                {
                    if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_HORDE, eArtifactsDatas::CountForDruidShaman))
                    {
                        l_Ashran->CastSpellOnTeam(me, TeamId::TEAM_ALLIANCE, eAshranSpells::SpellEventAllianceReward);
                        l_Ashran->EndArtifactEvent(TeamId::TEAM_HORDE, eArtifactsDatas::CountForDruidShaman);
                    }
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::SpellFractureSearcher)
                    me->CastSpell(p_Target, eSpells::SpellFractureMissile, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    me->SetWalk(true);
                    me->LoadPath(me->GetEntry());
                    me->SetDefaultMovementType(MovementGeneratorType::WAYPOINT_MOTION_TYPE);
                    me->GetMotionMaster()->Initialize();
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFracture:
                        me->CastSpell(me, eSpells::SpellFractureSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventFracture, 16000);
                        break;
                    case eEvents::EventGroundPound:
                        me->CastSpell(me, eSpells::SpellGroundPound, false);
                        m_Events.ScheduleEvent(eEvents::EventGroundPound, 43000);
                        break;
                    case eEvents::EventRockShield:
                        me->CastSpell(me, eSpells::SpellRockShield, true);
                        m_Events.ScheduleEvent(eEvents::EventRockShield, 39000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_kronusAI(p_Creature);
        }
};

/// Underpowered Earth Fury <Horde Guardian> - 82200
class npc_ashran_underpowered_earth_fury : public CreatureScript
{
    public:
        npc_ashran_underpowered_earth_fury() : CreatureScript("npc_ashran_underpowered_earth_fury") { }

        struct npc_ashran_underpowered_earth_furyAI : public ScriptedAI
        {
            npc_ashran_underpowered_earth_furyAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eData
            {
                WarspearShaman = 82438
            };

            EventMap m_Events;

            void Reset() override
            {
                std::list<Creature*> l_WarspearShamans;
                me->GetCreatureListWithEntryInGrid(l_WarspearShamans, eData::WarspearShaman, 20.0f);

                for (Creature* l_Creature : l_WarspearShamans)
                {
                    if (l_Creature->AI())
                        l_Creature->AI()->Reset();
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_underpowered_earth_furyAI(p_Creature);
        }
};

/// Warspear Gladiator - 85811
class npc_ashran_warspear_gladiator : public CreatureScript
{
    public:
        npc_ashran_warspear_gladiator() : CreatureScript("npc_ashran_warspear_gladiator") { }

        enum eSpells
        {
            SpellCleave         = 119419,
            SpellDevotionAura   = 165712,
            SpellMortalStrike   = 19643,
            SpellNet            = 81210,
            SpellSnapKick       = 15618
        };

        enum eEvents
        {
            EventCleave = 1,
            EventMortalStrike,
            EventNet,
            EventSnapKick
        };

        struct npc_ashran_warspear_gladiatorAI : public ScriptedAI
        {
            npc_ashran_warspear_gladiatorAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::SpellDevotionAura, true);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->CastSpell(me, eSpells::SpellDevotionAura, true);
                me->CastSpell(p_Attacker, eSpells::SpellNet, true);

                m_Events.ScheduleEvent(eEvents::EventCleave, 3000);
                m_Events.ScheduleEvent(eEvents::EventMortalStrike, 5000);
                m_Events.ScheduleEvent(eEvents::EventNet, 8000);
                m_Events.ScheduleEvent(eEvents::EventSnapKick, 9000);
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
                    case eEvents::EventCleave:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellCleave, true);
                        m_Events.ScheduleEvent(eEvents::EventCleave, 15000);
                        break;
                    case eEvents::EventMortalStrike:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellMortalStrike, true);
                        m_Events.ScheduleEvent(eEvents::EventMortalStrike, 10000);
                        break;
                    case eEvents::EventNet:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellNet, true);
                        m_Events.ScheduleEvent(eEvents::EventNet, 20000);
                        break;
                    case eEvents::EventSnapKick:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellSnapKick, true);
                        m_Events.ScheduleEvent(eEvents::EventSnapKick, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_warspear_gladiatorAI(p_Creature);
        }
};

/// Excavator Rustshiv - 88568
class npc_ashran_excavator_rustshiv : public CreatureScript
{
    public:
        npc_ashran_excavator_rustshiv() : CreatureScript("npc_ashran_excavator_rustshiv") { }

        enum eTalks
        {
            First,
            Second,
            Third,
            Fourth,
            Fifth,
            Sixth
        };

        enum eData
        {
            ExcavatorHardtooth  = 88567,
            ActionInit          = 0,
            ActionLoop          = 1,
            EventLoop           = 1
        };

        struct npc_ashran_excavator_rustshivAI : public MS::AI::CosmeticAI
        {
            npc_ashran_excavator_rustshivAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            bool m_Init;
            EventMap m_Events;

            void Reset() override
            {
                m_Init = false;

                if (Creature* l_Creature = me->FindNearestCreature(eData::ExcavatorHardtooth, 15.0f))
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

            void ScheduleAllTalks()
            {
                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::First); });
                AddTimedDelayedOperation(18 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Second); });
                AddTimedDelayedOperation(36 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Third); });
                AddTimedDelayedOperation(67 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Fourth); });
                AddTimedDelayedOperation(84 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Fifth); });
                AddTimedDelayedOperation(101 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Sixth); });
            }

            void LastOperationCalled() override
            {
                m_Events.ScheduleEvent(eData::EventLoop, 31 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eData::EventLoop)
                {
                    if (Creature* l_Creature = me->FindNearestCreature(eData::ExcavatorHardtooth, 15.0f))
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

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_excavator_rustshivAI(p_Creature);
        }
};

/// Excavator Hardtooth - 88567
class npc_ashran_excavator_hardtooth : public CreatureScript
{
    public:
        npc_ashran_excavator_hardtooth() : CreatureScript("npc_ashran_excavator_hardtooth") { }

        enum eTalks
        {
            First,
            Second,
            Third,
            Fourth
        };

        enum eData
        {
            ExcavatorRustshiv   = 88568,
            ActionInit          = 0,
            ActionLoop          = 1
        };

        struct npc_ashran_excavator_hardtoothAI : public MS::AI::CosmeticAI
        {
            npc_ashran_excavator_hardtoothAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            bool m_Init;

            void Reset() override
            {
                m_Init = false;

                if (Creature* l_Creature = me->FindNearestCreature(eData::ExcavatorRustshiv, 15.0f))
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
                AddTimedDelayedOperation(76 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Third); });
                AddTimedDelayedOperation(93 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Fourth); });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_excavator_hardtoothAI(p_Creature);
        }
};

/// Vol'jin's Spear Battle Standard - 85383
class npc_ashran_voljins_spear_battle_standard : public CreatureScript
{
    public:
        npc_ashran_voljins_spear_battle_standard() : CreatureScript("npc_ashran_voljins_spear_battle_standard") { }

        struct npc_ashran_voljins_spear_battle_standardAI : public ScriptedAI
        {
            npc_ashran_voljins_spear_battle_standardAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void JustDied(Unit* p_Killer) override
            {
                me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_voljins_spear_battle_standardAI(p_Creature);
        }
};

void AddSC_AshranNPCHorde()
{
    new npc_jeron_emberfall();
    new npc_ashran_warspear_shaman();
    new npc_ashran_illandria_belore();
    new npc_ashran_examiner_rahm_flameheart();
    new npc_ashran_centurion_firescream();
    new npc_ashran_legionnaire_hellaxe();
    new npc_ashran_kalgan();
    new npc_ashran_fura();
    new npc_ashran_nisstyr();
    new npc_ashran_atomik();
    new npc_ashran_zaram_sunraiser();
    new npc_ashran_horde_gateway_guardian();
    new npc_ashran_kronus();
    new npc_ashran_underpowered_earth_fury();
    new npc_ashran_warspear_gladiator();
    new npc_ashran_excavator_rustshiv();
    new npc_ashran_excavator_hardtooth();
    new npc_ashran_voljins_spear_battle_standard();
}