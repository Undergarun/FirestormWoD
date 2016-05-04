////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
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

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventFireball, 4000);
                m_Events.ScheduleEvent(eEvents::EventIgnite, 8000);
                m_Events.ScheduleEvent(eEvents::EventLivingBomb, 12000);
                m_Events.ScheduleEvent(eEvents::EventCombustionNova, 15000);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->IsPlayer())
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* /*p_Killer*/) override
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

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 /*p_Action*/) override
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

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 p_Action) override
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

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 p_Action) override
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

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 p_Action) override
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

            void sGossipSelect(Player* p_Player, uint32 /*p_Sender*/, uint32 p_Action) override
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

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->CastSpell(me, eSpells::SpellMoltenArmor, true);

                m_Events.ScheduleEvent(eEvents::EventFlamestrike, 7000);
                m_Events.ScheduleEvent(eEvents::EventFireball, 3000);
                m_Events.ScheduleEvent(eEvents::EventBlastWave, 9000);
            }

            void JustDied(Unit* /*p_Killer*/) override
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

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->CastSpell(me, eSpells::SpellFelArmor, true);

                m_Events.ScheduleEvent(eEvents::EventImmolate, 1000);
                m_Events.ScheduleEvent(eEvents::EventIncinerate, 2000);
                m_Events.ScheduleEvent(eEvents::EventChaosBolt, 5000);
            }

            void JustDied(Unit* /*p_Killer*/) override
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

                if (Aura* l_Scaling = me->AddAura(eSpells::AshranLaneMobScalingAura, me))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_PlayerCount);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
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

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                Position l_Pos;
                me->GetPosition(&l_Pos);
                me->SetHomePosition(l_Pos);

                m_Events.ScheduleEvent(eEvents::EventFracture, 5000);
                m_Events.ScheduleEvent(eEvents::EventGroundPound, 12000);
                m_Events.ScheduleEvent(eEvents::EventRockShield, 9000);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
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

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_voljins_spear_battle_standardAI(p_Creature);
        }
};

/// Warspear Headhunter - 88691
class npc_ashran_warspear_headhunter : public CreatureScript
{
    public:
        npc_ashran_warspear_headhunter() : CreatureScript("npc_ashran_warspear_headhunter") { }

        enum eSpells
        {
            SpellShoot              = 163921,
            SpellKillShot           = 173642,
            SpellHeadhuntersMark    = 177203,
            SpellConcussiveShot     = 17174
        };

        enum eEvents
        {
            SearchTarget = 1,
            EventShoot,
            EventKillShot,
            EventHeadhuntersMark,
            EventConcussiveShot,
            EventClearEvade
        };

        struct npc_ashran_warspear_headhunterAI : public ScriptedAI
        {
            npc_ashran_warspear_headhunterAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CosmeticEvent.Reset();
            }

            EventMap m_Events;
            EventMap m_CosmeticEvent;

            void Reset() override
            {
                m_Events.Reset();

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                m_CosmeticEvent.ScheduleEvent(eEvents::SearchTarget, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventShoot, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventKillShot, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventHeadhuntersMark, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventConcussiveShot, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void EnterEvadeMode() override
            {
                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);

                CreatureAI::EnterEvadeMode();

                m_CosmeticEvent.ScheduleEvent(eEvents::EventClearEvade, 500);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_CosmeticEvent.Update(p_Diff);

                switch (m_CosmeticEvent.ExecuteEvent())
                {
                    case eEvents::SearchTarget:
                    {
                        if (Player* l_Player = me->FindNearestPlayer(40.0f))
                            AttackStart(l_Player);
                        else
                            m_CosmeticEvent.ScheduleEvent(eEvents::SearchTarget, 1 * TimeConstants::IN_MILLISECONDS);

                        break;
                    }
                    case eEvents::EventClearEvade:
                    {
                        me->ClearUnitState(UnitState::UNIT_STATE_EVADE);
                        break;
                    }
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
                    case eEvents::EventShoot:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellShoot, false);
                        m_Events.ScheduleEvent(eEvents::EventShoot, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventKillShot:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            if (l_Target->HealthBelowPct(20))
                            {
                                me->CastSpell(l_Target, eSpells::SpellKillShot, false);
                                m_Events.ScheduleEvent(eEvents::EventKillShot, 10 * TimeConstants::IN_MILLISECONDS);
                                break;
                            }
                        }

                        m_Events.ScheduleEvent(eEvents::EventKillShot, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventHeadhuntersMark:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellHeadhuntersMark, false);
                        m_Events.ScheduleEvent(eEvents::EventHeadhuntersMark, 18 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventConcussiveShot:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellConcussiveShot, false);
                        m_Events.ScheduleEvent(eEvents::EventConcussiveShot, 10 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_warspear_headhunterAI(p_Creature);
        }
};

/// Lord Mes <Horde Captain> - 80497
class npc_ashran_lord_mes : public CreatureScript
{
    public:
        npc_ashran_lord_mes() : CreatureScript("npc_ashran_lord_mes") { }

        enum eSpells
        {
            PlagueStrike    = 164063,
            DeathAndDecay   = 164334,
            DeathCoil       = 164064,
            DeathGrip       = 79894,
            DeathGripJump   = 168563
        };

        enum eEvents
        {
            EventPlagueStrike = 1,
            EventDeathAndDecay,
            EventDeathCoil,
            EventDeathGrip,
            EventMove
        };

        enum eTalks
        {
            Spawn,
            Death
        };

        enum eData
        {
            MountID = 25280
        };

        struct npc_ashran_lord_mesAI : public ScriptedAI
        {
            npc_ashran_lord_mesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Spawn = false;
            }

            EventMap m_Events;
            EventMap m_MoveEvent;
            bool m_Spawn;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                if (!m_Spawn)
                {
                    Talk(eTalks::Spawn);
                    m_Spawn = true;
                }

                me->Mount(eData::MountID);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->Mount(0);
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventPlagueStrike, 2 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDeathAndDecay, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDeathCoil, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDeathGrip, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainLordMes);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::DeathGrip)
                    p_Target->CastSpell(*me, eSpells::DeathGripJump, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventPlagueStrike:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::PlagueStrike, false);
                        m_Events.ScheduleEvent(eEvents::EventPlagueStrike, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDeathAndDecay:
                    {
                        me->CastSpell(me, eSpells::DeathAndDecay, false);
                        m_Events.ScheduleEvent(eEvents::EventDeathAndDecay, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDeathCoil:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::DeathCoil, false);
                        m_Events.ScheduleEvent(eEvents::EventDeathCoil, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDeathGrip:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::DeathGrip, false);
                        m_Events.ScheduleEvent(eEvents::EventDeathGrip, 20 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_lord_mesAI(p_Creature);
        }
};

/// Mindbender Talbadar <Horde Captain> - 80490
class npc_ashran_mindbender_talbadar : public CreatureScript
{
    public:
        npc_ashran_mindbender_talbadar() : CreatureScript("npc_ashran_mindbender_talbadar") { }

        enum eSpells
        {
            DevouringPlague = 164452,
            Dispersion      = 164444,
            MindBlast       = 164448,
            MindSear        = 177402,
            PsychicScream   = 164443,
            ShadowWordPain  = 164446
        };

        enum eEvents
        {
            EventDevouringPlague = 1,
            EventDispersion,
            EventMindBlast,
            EventMindSear,
            EventPsychicScream,
            EventShadowWordPain,
            EventMove
        };

        enum eTalk
        {
            Spawn
        };

        struct npc_ashran_mindbender_talbadarAI : public ScriptedAI
        {
            npc_ashran_mindbender_talbadarAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Spawn = false;
            }

            EventMap m_Events;
            EventMap m_MoveEvent;
            bool m_Spawn;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                if (!m_Spawn)
                {
                    Talk(eTalk::Spawn);
                    m_Spawn = true;
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventDevouringPlague, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDispersion, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMindBlast, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMindSear, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventPsychicScream, 15 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventShadowWordPain, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainMindbenderTalbadar);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventDevouringPlague:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::DevouringPlague, false);
                        m_Events.ScheduleEvent(eEvents::EventDevouringPlague, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDispersion:
                    {
                        if (me->HealthBelowPct(50))
                            me->CastSpell(me, eSpells::Dispersion, true);
                        else
                            m_Events.ScheduleEvent(eEvents::EventDispersion, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventMindBlast:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::MindBlast, false);
                        m_Events.ScheduleEvent(eEvents::EventMindBlast, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventMindSear:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::MindSear, false);
                        m_Events.ScheduleEvent(eEvents::EventMindSear, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventPsychicScream:
                    {
                        me->CastSpell(me, eSpells::PsychicScream, false);
                        m_Events.ScheduleEvent(eEvents::EventPsychicScream, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShadowWordPain:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::ShadowWordPain, false);
                        m_Events.ScheduleEvent(eEvents::EventShadowWordPain, 12 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_mindbender_talbadarAI(p_Creature);
        }
};

/// Elliott Van Rook <Horde Captain> - 80493
class npc_ashran_elliott_van_rook : public CreatureScript
{
    public:
        npc_ashran_elliott_van_rook() : CreatureScript("npc_ashran_elliott_van_rook") { }

        enum eSpells
        {
            Blizzard    = 162610,
            FrostNova   = 164067,
            Frostbolt   = 162608,
            IceLance    = 162609
        };

        enum eEvents
        {
            EventBlizzard = 1,
            EventFrostNova,
            EventFrostbolt,
            EventIceLance,
            EventMove
        };

        enum eTalks
        {
            Slay,
            Death
        };

        enum eData
        {
            MountID = 51048
        };

        struct npc_ashran_elliott_van_rookAI : public ScriptedAI
        {
            npc_ashran_elliott_van_rookAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                me->Mount(eData::MountID);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->Mount(0);
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventBlizzard, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventFrostNova, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventFrostbolt, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventIceLance, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainElliotVanRook);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventBlizzard:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Blizzard, false);
                        m_Events.ScheduleEvent(eEvents::EventBlizzard, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventFrostNova:
                        me->CastSpell(me, eSpells::FrostNova, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostNova, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventFrostbolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Frostbolt, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostbolt, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventIceLance:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::IceLance, false);
                        m_Events.ScheduleEvent(eEvents::EventIceLance, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_elliott_van_rookAI(p_Creature);
        }
};

/// Vanguard Samuelle <Horde Captain> - 80492
class npc_ashran_vanguard_samuelle : public CreatureScript
{
    public:
        npc_ashran_vanguard_samuelle() : CreatureScript("npc_ashran_vanguard_samuelle") { }

        enum eSpells
        {
            Judgment        = 162760,
            HammerOfWrath   = 162763,
            DivineShield    = 164410,
            DivineStorm     = 162641,
            HammerOfJustice = 162764,
            AvengingWrath   = 164397
        };

        enum eEvents
        {
            EventJudgment = 1,
            EventHammerOfWrath,
            EventDivineShield,
            EventDivineStorm,
            EventHammerOfJustice,
            EventAvengingWrath,
            EventMove
        };

        enum eTalks
        {
            Slay,
            Death
        };

        struct npc_ashran_vanguard_samuelleAI : public ScriptedAI
        {
            npc_ashran_vanguard_samuelleAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventJudgment, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventHammerOfWrath, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDivineShield, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDivineStorm, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventHammerOfJustice, 7 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventAvengingWrath, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainVanguardSamuelle);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventJudgment:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Judgment, false);
                        m_Events.ScheduleEvent(eEvents::EventJudgment, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventHammerOfWrath:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::HammerOfWrath, false);
                        m_Events.ScheduleEvent(eEvents::EventHammerOfWrath, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDivineShield:
                    {
                        if (me->HealthBelowPct(50))
                            me->CastSpell(me, eSpells::DivineShield, true);
                        else
                            m_Events.ScheduleEvent(eEvents::EventDivineShield, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDivineStorm:
                    {
                        me->CastSpell(me, eSpells::DivineStorm, false);
                        m_Events.ScheduleEvent(eEvents::EventDivineStorm, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventHammerOfJustice:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::HammerOfJustice, false);
                        m_Events.ScheduleEvent(eEvents::EventHammerOfJustice, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventAvengingWrath:
                    {
                        me->CastSpell(me, eSpells::AvengingWrath, false);
                        m_Events.ScheduleEvent(eEvents::EventAvengingWrath, 45 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_vanguard_samuelleAI(p_Creature);
        }
};

/// Elementalist Novo <Horde Captain> - 80491
class npc_ashran_elementalist_novo : public CreatureScript
{
    public:
        npc_ashran_elementalist_novo() : CreatureScript("npc_ashran_elementalist_novo") { }

        enum eSpells
        {
            ChainLightning  = 178060,
            Hex             = 178064,
            LavaBurst       = 178091,
            LightningBolt   = 178059,
            MagmaTotem      = 178063,
            MagmaTotemAura  = 178062
        };

        enum eEvents
        {
            EventChainLightning = 1,
            EventHex,
            EventLavaBurst,
            EventLightningBolt,
            EventMagmaTotem,
            EventMove
        };

        enum eTalk
        {
            Death
        };

        struct npc_ashran_elementalist_novoAI : public ScriptedAI
        {
            npc_ashran_elementalist_novoAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                /// Second equip is a shield
                me->SetCanDualWield(false);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventChainLightning, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventHex, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventLavaBurst, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventLightningBolt, 2 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMagmaTotem, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalk::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainElementalistNovo);
            }

            void JustSummoned(Creature* p_Summon) override
            {
                p_Summon->SetReactState(ReactStates::REACT_PASSIVE);
                p_Summon->AddAura(eSpells::MagmaTotemAura, p_Summon);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventChainLightning:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::ChainLightning, false);
                        m_Events.ScheduleEvent(eEvents::EventChainLightning, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventHex:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Hex, false);
                        m_Events.ScheduleEvent(eEvents::EventHex, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventLavaBurst:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::LavaBurst, false);
                        m_Events.ScheduleEvent(eEvents::EventLavaBurst, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventLightningBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::LightningBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventLightningBolt, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventMagmaTotem:
                        me->CastSpell(me, eSpells::MagmaTotem, false);
                        m_Events.ScheduleEvent(eEvents::EventMagmaTotem, 40 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_elementalist_novoAI(p_Creature);
        }
};

/// Captain Hoodrych <Horde Captain> - 79900
class npc_ashran_captain_hoodrych : public CreatureScript
{
    public:
        npc_ashran_captain_hoodrych() : CreatureScript("npc_ashran_captain_hoodrych") { }

        enum eSpells
        {
            SpellBladestorm = 164091,
            Shockwave       = 164092
        };

        enum eEvents
        {
            EventBladestorm = 1,
            EventShockwave,
            EventMove
        };

        enum eTalks
        {
            Slay,
            Bladestorm,
            Death
        };

        enum eData
        {
            MountID = 38607
        };

        struct npc_ashran_captain_hoodrychAI : public ScriptedAI
        {
            npc_ashran_captain_hoodrychAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                me->Mount(eData::MountID);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->Mount(0);
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventBladestorm, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventShockwave, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void EnterEvadeMode() override
            {
                me->InterruptNonMeleeSpells(true);

                CreatureAI::EnterEvadeMode();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainCaptainHoodrych);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
                    me->SetDefaultMovementType(MovementGeneratorType::WAYPOINT_MOTION_TYPE);
                    me->GetMotionMaster()->Initialize();
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                /// Update position during Bladestorm
                if (me->HasAura(eSpells::SpellBladestorm))
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    {
                        me->GetMotionMaster()->MovePoint(0, *l_Target);
                        return;
                    }
                }

                /// Update target movements here to avoid some movements problems
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                {
                    if (!me->IsWithinMeleeRange(l_Target))
                    {
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MoveChase(l_Target);
                    }
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBladestorm:
                        Talk(eTalks::Bladestorm);
                        me->CastSpell(me, eSpells::SpellBladestorm, false);
                        m_Events.ScheduleEvent(eEvents::EventBladestorm, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventShockwave:
                        me->CastSpell(me, eSpells::Shockwave, false);
                        m_Events.ScheduleEvent(eEvents::EventShockwave, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_captain_hoodrychAI(p_Creature);
        }
};

/// Soulbrewer Nadagast <Horde Captain> - 80489
class npc_ashran_soulbrewer_nadagast : public CreatureScript
{
    public:
        npc_ashran_soulbrewer_nadagast() : CreatureScript("npc_ashran_soulbrewer_nadagast") { }

        enum eSpells
        {
            ChaosBolt   = 178076,
            RainOfFire  = 178069
        };

        enum eEvents
        {
            EventChaosBolt = 1,
            EventRainOfFire,
            EventMove
        };

        enum eTalks
        {
            Spawn,
            Slay
        };

        struct npc_ashran_soulbrewer_nadagastAI : public ScriptedAI
        {
            npc_ashran_soulbrewer_nadagastAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Spawn = false;
            }

            EventMap m_Events;
            EventMap m_MoveEvent;
            bool m_Spawn;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                if (!m_Spawn)
                {
                    m_Spawn = true;
                    Talk(eTalks::Spawn);
                }

                /// Second equip is a Off-hand Frill
                me->SetCanDualWield(false);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventChaosBolt, 3 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRainOfFire, 6 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainSoulbrewerNadagast);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventChaosBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::ChaosBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventChaosBolt, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventRainOfFire:
                        me->CastSpell(me, eSpells::RainOfFire, false);
                        m_Events.ScheduleEvent(eEvents::EventRainOfFire, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_soulbrewer_nadagastAI(p_Creature);
        }
};

/// Necrolord Azael <Horde Captain> - 80486
class npc_ashran_necrolord_azael : public CreatureScript
{
    public:
        npc_ashran_necrolord_azael() : CreatureScript("npc_ashran_necrolord_azael") { }

        enum eSpells
        {
            ChaosBolt   = 178076,
            RainOfFire  = 178069
        };

        enum eEvents
        {
            EventChaosBolt = 1,
            EventRainOfFire,
            EventMove
        };

        enum eTalks
        {
            Slay,
            Death
        };

        enum eData
        {
            MountID = 51048
        };

        struct npc_ashran_necrolord_azaelAI : public ScriptedAI
        {
            npc_ashran_necrolord_azaelAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                me->Mount(eData::MountID);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->Mount(0);
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventChaosBolt, 3 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRainOfFire, 6 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainNecrolordAzael);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventChaosBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::ChaosBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventChaosBolt, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventRainOfFire:
                        me->CastSpell(me, eSpells::RainOfFire, false);
                        m_Events.ScheduleEvent(eEvents::EventRainOfFire, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_necrolord_azaelAI(p_Creature);
        }
};

/// Rifthunter Yoske <Horde Captain> - 80496
class npc_ashran_rifthunter_yoske : public CreatureScript
{
    public:
        npc_ashran_rifthunter_yoske() : CreatureScript("npc_ashran_rifthunter_yoske") { }

        enum eSpells
        {
            Shoot           = 164095,
            SerpentSting    = 162754
        };

        enum eEvents
        {
            EventShoot = 1,
            EventSerpentSting,
            EventMove
        };

        enum eTalks
        {
            Slay,
            Death
        };

        struct npc_ashran_rifthunter_yoskeAI : public ScriptedAI
        {
            npc_ashran_rifthunter_yoskeAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventShoot, 3 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSerpentSting, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainRifthunterYoske);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventShoot:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Shoot, false);
                        m_Events.ScheduleEvent(eEvents::EventShoot, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventSerpentSting:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SerpentSting, false);
                        m_Events.ScheduleEvent(eEvents::EventSerpentSting, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_rifthunter_yoskeAI(p_Creature);
        }
};

/// Mor'riz <The Ultimate Troll> - 85133
class npc_ashran_morriz : public CreatureScript
{
    public:
        npc_ashran_morriz() : CreatureScript("npc_ashran_morriz") { }

        enum eSpell
        {
            Typhoon = 164337
        };

        enum eEvents
        {
            EventTyphoon = 1,
            EventMove
        };

        struct npc_ashran_morrizAI : public ScriptedAI
        {
            npc_ashran_morrizAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventTyphoon, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainMorriz);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventTyphoon:
                    {
                        me->CastSpell(me, eSpell::Typhoon, false);
                        m_Events.ScheduleEvent(eEvents::EventTyphoon, 20 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_morrizAI(p_Creature);
        }
};

/// Kaz Endsky <Horde Captain> - 87690
class npc_ashran_kaz_endsky : public CreatureScript
{
    public:
        npc_ashran_kaz_endsky() : CreatureScript("npc_ashran_kaz_endsky") { }

        enum eSpells
        {
            PlagueStrike    = 164063,
            DeathAndDecay   = 164334,
            DeathCoil       = 164064,
            DeathGrip       = 79894,
            DeathGripJump   = 168563
        };

        enum eEvents
        {
            EventPlagueStrike = 1,
            EventDeathAndDecay,
            EventDeathCoil,
            EventDeathGrip,
            EventMove
        };

        enum eTalks
        {
            Slay,
            Death
        };

        enum eData
        {
            MountID = 25280
        };

        struct npc_ashran_kaz_endskyAI : public ScriptedAI
        {
            npc_ashran_kaz_endskyAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                me->Mount(eData::MountID);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->Mount(0);
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventPlagueStrike, 2 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDeathAndDecay, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDeathCoil, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDeathGrip, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainKazEndsky);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::DeathGrip)
                    p_Target->CastSpell(*me, eSpells::DeathGripJump, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventPlagueStrike:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::PlagueStrike, false);
                        m_Events.ScheduleEvent(eEvents::EventPlagueStrike, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDeathAndDecay:
                    {
                        me->CastSpell(me, eSpells::DeathAndDecay, false);
                        m_Events.ScheduleEvent(eEvents::EventDeathAndDecay, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDeathCoil:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::DeathCoil, false);
                        m_Events.ScheduleEvent(eEvents::EventDeathCoil, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDeathGrip:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::DeathGrip, false);
                        m_Events.ScheduleEvent(eEvents::EventDeathGrip, 20 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_kaz_endskyAI(p_Creature);
        }
};

/// Razor Guerra <Horde Captain> - 85138
class npc_ashran_razor_guerra : public CreatureScript
{
    public:
        npc_ashran_razor_guerra() : CreatureScript("npc_ashran_razor_guerra") { }

        enum eSpells
        {
            Blind           = 178058,
            CloakOfShadows  = 178055,
            Eviscerate      = 178054,
            FanOfKnives     = 178053,
            Hemorrhage      = 178052,
            Shadowstep      = 178056,
            WoundPoison     = 178050
        };

        enum eEvents
        {
            EventBlind = 1,
            EventCloakOfShadows,
            EventEviscerate,
            EventFanOfKnives,
            EventHemorrhage,
            EventShadowStep,
            EventWoundPoison,
            EventMove
        };

        enum eTalks
        {
            Slay,
            Death
        };

        enum eData
        {
            MountID = 51048
        };

        struct npc_ashran_razor_guerraAI : public ScriptedAI
        {
            npc_ashran_razor_guerraAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                me->Mount(eData::MountID);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->Mount(0);
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventBlind, 15 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventCloakOfShadows, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventEviscerate, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventFanOfKnives, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventHemorrhage, 2 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventShadowStep, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventWoundPoison, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainRazorGuerra);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventBlind:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Blind, false);
                        m_Events.ScheduleEvent(eEvents::EventBlind, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventCloakOfShadows:
                    {
                        if (me->HealthBelowPct(50))
                            me->CastSpell(me, eSpells::CloakOfShadows, true);
                        else
                            m_Events.ScheduleEvent(eEvents::EventCloakOfShadows, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventEviscerate:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Eviscerate, false);
                        m_Events.ScheduleEvent(eEvents::EventEviscerate, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventFanOfKnives:
                    {
                        me->CastSpell(me, eSpells::FanOfKnives, false);
                        m_Events.ScheduleEvent(eEvents::EventFanOfKnives, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventHemorrhage:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Hemorrhage, false);
                        m_Events.ScheduleEvent(eEvents::EventHemorrhage, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShadowStep:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::Shadowstep, false);
                        m_Events.ScheduleEvent(eEvents::EventShadowStep, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventWoundPoison:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::WoundPoison, false);
                        m_Events.ScheduleEvent(eEvents::EventWoundPoison, 12 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_razor_guerraAI(p_Creature);
        }
};

/// Jared V. Hellstrike <Horde Captain> - 85131
class npc_ashran_jared_v_hellstrike : public CreatureScript
{
    public:
        npc_ashran_jared_v_hellstrike() : CreatureScript("npc_ashran_jared_v_hellstrike") { }

        enum eSpells
        {
            BlackoutKick        = 164394,
            LegSweep            = 164392,
            RisingSunKick       = 127734,
            SpinningCraneKick   = 162759
        };

        enum eEvents
        {
            EventBlackoutKick = 1,
            EventLegSweep,
            EventRisingSunKick,
            EventSpinningCraneKick,
            EventMove
        };

        struct npc_ashran_jared_v_hellstrikeAI : public ScriptedAI
        {
            npc_ashran_jared_v_hellstrikeAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            EventMap m_MoveEvent;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventBlackoutKick, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventLegSweep, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRisingSunKick, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSpinningCraneKick, 12 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainJaredVHellstrike);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    me->SetWalk(true);
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventBlackoutKick:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::BlackoutKick, false);
                        m_Events.ScheduleEvent(eEvents::EventBlackoutKick, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventLegSweep:
                        me->CastSpell(me, eSpells::LegSweep, false);
                        m_Events.ScheduleEvent(eEvents::EventLegSweep, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventRisingSunKick:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::RisingSunKick, false);
                        m_Events.ScheduleEvent(eEvents::EventRisingSunKick, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventSpinningCraneKick:
                        me->CastSpell(me, eSpells::SpinningCraneKick, false);
                        m_Events.ScheduleEvent(eEvents::EventSpinningCraneKick, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_jared_v_hellstrikeAI(p_Creature);
        }
};

/// Kimilyn <Forged in Flame> - 88109
class npc_ashran_kimilyn : public CreatureScript
{
    public:
        npc_ashran_kimilyn() : CreatureScript("npc_ashran_kimilyn") { }

        enum eSpells
        {
            DevouringPlague = 164452,
            Dispersion      = 164444,
            MindBlast       = 164448,
            MindSear        = 177402,
            PsychicScream   = 164443,
            ShadowWordPain  = 164446
        };

        enum eEvents
        {
            EventDevouringPlague = 1,
            EventDispersion,
            EventMindBlast,
            EventMindSear,
            EventPsychicScream,
            EventShadowWordPain,
            EventMove
        };

        enum eTalks
        {
            Spawn,
            Death
        };

        enum eData
        {
            MountID = 51048
        };

        struct npc_ashran_kimilynAI : public ScriptedAI
        {
            npc_ashran_kimilynAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Spawn = false;
            }

            EventMap m_Events;
            EventMap m_MoveEvent;
            bool m_Spawn;

            void InitializeAI() override
            {
                m_MoveEvent.ScheduleEvent(eEvents::EventMove, 1 * TimeConstants::IN_MILLISECONDS);

                Reset();
            }

            void Reset() override
            {
                m_Events.Reset();

                if (!m_Spawn)
                {
                    Talk(eTalks::Spawn);
                    m_Spawn = true;
                }

                me->Mount(eData::MountID);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->Mount(0);
                me->SetHomePosition(*me);

                m_Events.ScheduleEvent(eEvents::EventDevouringPlague, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventDispersion, 1 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMindBlast, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMindSear, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventPsychicScream, 15 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventShadowWordPain, 1 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                Talk(eTalks::Death);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)me->GetZoneScript())
                    l_Ashran->HandleCaptainDeath(eSpecialSpawns::CaptainKimilyn);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_MoveEvent.Update(p_Diff);

                if (m_MoveEvent.ExecuteEvent() == eEvents::EventMove)
                {
                    /// Use same path as Kronus
                    me->LoadPath(eCreatures::Kronus);
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
                    case eEvents::EventDevouringPlague:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::DevouringPlague, false);
                        m_Events.ScheduleEvent(eEvents::EventDevouringPlague, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventDispersion:
                    {
                        if (me->HealthBelowPct(50))
                            me->CastSpell(me, eSpells::Dispersion, true);
                        else
                            m_Events.ScheduleEvent(eEvents::EventDispersion, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventMindBlast:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::MindBlast, false);
                        m_Events.ScheduleEvent(eEvents::EventMindBlast, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventMindSear:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::MindSear, false);
                        m_Events.ScheduleEvent(eEvents::EventMindSear, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventPsychicScream:
                    {
                        me->CastSpell(me, eSpells::PsychicScream, false);
                        m_Events.ScheduleEvent(eEvents::EventPsychicScream, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventShadowWordPain:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::ShadowWordPain, false);
                        m_Events.ScheduleEvent(eEvents::EventShadowWordPain, 12 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_ashran_kimilynAI(p_Creature);
        }
};

/// Speedy Horde Racer - 82903
class npc_ashran_speedy_horde_racer : public CreatureScript
{
    public:
        npc_ashran_speedy_horde_racer() : CreatureScript("npc_ashran_speedy_horde_racer") { }

        enum eSpell
        {
            HordeRacer = 166819
        };

        struct npc_ashran_speedy_horde_racerAI : public MS::AI::CosmeticAI
        {
            npc_ashran_speedy_horde_racerAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_CheckCooldown = time(nullptr) + 5;;
            }

            uint8 m_MoveIndex;
            uint32 m_CheckCooldown;

            void InitializeAI() override
            {
                Reset();
            }

            void Reset() override
            {
                me->CastSpell(me, eSpell::HordeRacer, true);
                me->ModifyAuraState(AuraStateType::AURA_STATE_UNKNOWN22, true);

                m_MoveIndex = 0;

                AddTimedDelayedOperation(500, [this]() -> void
                {
                    if (Creature* l_Rider = me->FindNearestCreature(eCreatures::HordeRider, 10.0f))
                        l_Rider->EnterVehicle(me);
                });

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->GetMotionMaster()->MovePoint(m_MoveIndex, g_HordeRacingMoves[m_MoveIndex]);
                });
            }

            void MovementInform(uint32 p_Type, uint32 /*p_ID*/) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                ++m_MoveIndex;

                if (m_MoveIndex >= eAshranDatas::HordeRacingMovesCount)
                {
                    m_MoveIndex = 0;
                    IncreaseLapCount();
                }

                AddTimedDelayedOperation(100, [this]() -> void
                {
                    me->GetMotionMaster()->MovePoint(m_MoveIndex, g_HordeRacingMoves[m_MoveIndex]);
                });
            }

            void IncreaseLapCount()
            {
                OutdoorPvP* l_Outdoor = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_Outdoor)
                    l_Ashran->SetEventData(eAshranEvents::EventStadiumRacing, TeamId::TEAM_HORDE, 1);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_speedy_horde_racerAI(p_Creature);
        }
};
#ifndef __clang_analyzer__
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
    new npc_ashran_warspear_headhunter();
    new npc_ashran_lord_mes();
    new npc_ashran_mindbender_talbadar();
    new npc_ashran_elliott_van_rook();
    new npc_ashran_vanguard_samuelle();
    new npc_ashran_elementalist_novo();
    new npc_ashran_captain_hoodrych();
    new npc_ashran_soulbrewer_nadagast();
    new npc_ashran_necrolord_azael();
    new npc_ashran_rifthunter_yoske();
    new npc_ashran_morriz();
    new npc_ashran_kaz_endsky();
    new npc_ashran_razor_guerra();
    new npc_ashran_jared_v_hellstrike();
    new npc_ashran_kimilyn();
    new npc_ashran_speedy_horde_racer();
}
#endif