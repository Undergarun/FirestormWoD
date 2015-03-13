////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Rylai Crestfall <Stormshield Tower Guardian> - 88224
class npc_rylai_crestfall : public CreatureScript
{
    public:
        npc_rylai_crestfall() : CreatureScript("npc_rylai_crestfall") { }

        struct npc_rylai_crestfallAI : public ScriptedAI
        {
            npc_rylai_crestfallAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_CheckAroundingPlayersTimer = 0;
                m_FreezingFieldTimer = 0;
            }

            enum eSpells
            {
                Frostbolt       = 176268,
                FrostboltVolley = 176273,
                IceBlock        = 176269,
                Hypotermia      = 41425,
                MassPolymorph   = 176204,
                FrostNovaCasted = 176327,
                NorthrendWinds  = 176267,
                FrostNova       = 176276,
                DeepFreeze      = 176278,
                SummonIceShard  = 177599,   ///< @TODO

                TowerMageTargetingAura  = 176162,   ///< Put on ennemy players around 200 yards
                FreezingFieldSearcher   = 176163,   ///< Launch frost missile on one player targeted
                FreezingFieldMissile    = 176165,

                ConjureRefreshment      = 176351
            };

            enum eTalk
            {
                TalkAggro,
                TalkSlay,
                TalkDeath,
                TalkSpell
            };

            enum eEvents
            {
                EventFrostbolt = 1,
                EventFrostboltVolley,
                EventMassPolymorph,
                EventFrostNova,
                EventNorthrendWinds
            };

            EventMap m_Events;

            uint32 m_CheckAroundingPlayersTimer;
            uint32 m_FreezingFieldTimer;

            void Reset() override
            {
                m_Events.Reset();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);

                m_CheckAroundingPlayersTimer = 2000;
                m_FreezingFieldTimer = 10000;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventFrostbolt, 4000);
                m_Events.ScheduleEvent(eEvents::EventMassPolymorph, 6000);
                m_Events.ScheduleEvent(eEvents::EventFrostboltVolley, 9000);
                m_Events.ScheduleEvent(eEvents::EventFrostNova, 12500);
                m_Events.ScheduleEvent(eEvents::EventNorthrendWinds, 15000);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalk::TalkDeath);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HealthBelowPctDamaged(20, p_Damage) && !me->HasAura(eSpells::Hypotermia))
                {
                    me->CastSpell(me, eSpells::IceBlock, true);
                    me->CastSpell(me, eSpells::Hypotermia, true);
                }
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo) override
            {
                if (p_Victim == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::FreezingFieldSearcher:
                        me->CastSpell(p_Victim, eSpells::FreezingFieldMissile, false);
                        break;
                    case eSpells::NorthrendWinds:
                        if (p_Victim->HasAura(eSpells::FrostNova))
                            me->CastSpell(p_Victim, eSpells::DeepFreeze, true);
                        else
                            me->CastSpell(p_Victim, eSpells::FrostNova, true);
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
                    ScheduleFreezingField(p_Diff);
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFrostbolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::Frostbolt, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostbolt, 10000);
                        break;
                    case eEvents::EventFrostboltVolley:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::FrostboltVolley, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostboltVolley, 20000);
                        break;
                    case eEvents::EventMassPolymorph:
                        me->CastSpell(me, eSpells::MassPolymorph, false);
                        m_Events.ScheduleEvent(eEvents::EventMassPolymorph, 25000);
                        break;
                    case eEvents::EventFrostNova:
                        me->CastSpell(me, eSpells::FrostNovaCasted, false);
                        m_Events.ScheduleEvent(eEvents::EventFrostNova, 27500);
                        break;
                    case eEvents::EventNorthrendWinds:
                        Talk(eTalk::TalkSpell);
                        me->CastSpell(me, eSpells::NorthrendWinds, false);
                        m_Events.ScheduleEvent(eEvents::EventNorthrendWinds, 30000);
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

                        if (p_Player->HasAura(eSpells::TowerMageTargetingAura))
                            return true;

                        return false;
                    });

                    for (Player* l_Player : l_PlayerList)
                        l_Player->CastSpell(l_Player, eSpells::TowerMageTargetingAura, true, nullptr, nullptr, me->GetGUID());
                }
                else
                    m_CheckAroundingPlayersTimer -= p_Diff;
            }

            void ScheduleFreezingField(uint32 const p_Diff)
            {
                if (!m_FreezingFieldTimer)
                    return;

                if (m_FreezingFieldTimer <= p_Diff)
                {
                    if (!me->isInCombat())
                        me->CastSpell(me, eSpells::FreezingFieldSearcher, true);
                    m_FreezingFieldTimer = 10000;
                }
                else
                    m_FreezingFieldTimer -= p_Diff;
            }

            void sGossipSelect(Player* p_Player, uint32 p_Sender, uint32 p_Action) override
            {
                p_Player->PlayerTalkClass->SendCloseGossip();
                me->CastSpell(p_Player, eSpells::ConjureRefreshment, false);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_rylai_crestfallAI(p_Creature);
        }
};

/// Grimnir Sternhammer <Explorer's League> - 88679
class npc_ashran_grimnir_sternhammer : public CreatureScript
{
    public:
        npc_ashran_grimnir_sternhammer() : CreatureScript("npc_ashran_grimnir_sternhammer") { }

        enum eTalks
        {
            First,
            Second,
            Third,
            Fourth
        };

        enum eData
        {
            MisirinStouttoe = 88682,
            ActionInit      = 0,
            ActionLoop      = 1,
            EventLoop       = 1
        };

        struct npc_ashran_grimnir_sternhammerAI : public MS::AI::CosmeticAI
        {
            npc_ashran_grimnir_sternhammerAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            bool m_Init;
            EventMap m_Events;

            void Reset() override
            {
                m_Init = false;

                if (Creature* l_Creature = me->FindNearestCreature(eData::MisirinStouttoe, 15.0f))
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
                AddTimedDelayedOperation(13 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Second); });
                AddTimedDelayedOperation(42 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Third); });
                AddTimedDelayedOperation(54 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Fourth); });
            }

            void LastOperationCalled() override
            {
                m_Events.ScheduleEvent(eData::EventLoop, 33 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eData::EventLoop)
                {
                    if (Creature* l_Creature = me->FindNearestCreature(eData::MisirinStouttoe, 15.0f))
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
            return new npc_ashran_grimnir_sternhammerAI(p_Creature);
        }
};

/// Misirin Stouttoe <Explorer's League> - 88682
class npc_ashran_misirin_stouttoe : public CreatureScript
{
    public:
        npc_ashran_misirin_stouttoe() : CreatureScript("npc_ashran_misirin_stouttoe") { }

        enum eTalks
        {
            First,
            Second,
            Third
        };

        enum eData
        {
            GrimnirSternhammer  = 88679,
            ActionInit          = 0,
            ActionLoop          = 1
        };

        struct npc_ashran_misirin_stouttoeAI : public MS::AI::CosmeticAI
        {
            npc_ashran_misirin_stouttoeAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            bool m_Init;

            void Reset() override
            {
                m_Init = false;

                if (Creature* l_Creature = me->FindNearestCreature(eData::GrimnirSternhammer, 15.0f))
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
                AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::First); });
                AddTimedDelayedOperation(48 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Second); });
                AddTimedDelayedOperation(60 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Third); });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_misirin_stouttoeAI(p_Creature);
        }
};

/// Stormshield Druid - 81887
class npc_ashran_stormshield_druid : public CreatureScript
{
    public:
        npc_ashran_stormshield_druid() : CreatureScript("npc_ashran_stormshield_druid") { }

        struct npc_ashran_stormshield_druidAI : public ScriptedAI
        {
            npc_ashran_stormshield_druidAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eDatas
            {
                EventCosmetic       = 1,
                AncientOfWar        = 81883,
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
                    if (Creature* l_EarthFury = me->FindNearestCreature(eDatas::AncientOfWar, 15.0f))
                        me->CastSpell(l_EarthFury, eDatas::NatureChanneling, false);
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_stormshield_druidAI(p_Creature);
        }
};

/// Officer Rumsfeld - 88696
class npc_ashran_officer_rumsfeld : public CreatureScript
{
    public:
        npc_ashran_officer_rumsfeld() : CreatureScript("npc_ashran_officer_rumsfeld") { }

        struct npc_ashran_officer_rumsfeldAI : public MS::AI::CosmeticAI
        {
            npc_ashran_officer_rumsfeldAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

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
            return new npc_ashran_officer_rumsfeldAI(p_Creature);
        }
};

/// Officer Ironore - 88697
class npc_ashran_officer_ironore : public CreatureScript
{
    public:
        npc_ashran_officer_ironore() : CreatureScript("npc_ashran_officer_ironore") { }

        struct npc_ashran_officer_ironoreAI : public MS::AI::CosmeticAI
        {
            npc_ashran_officer_ironoreAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

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
            return new npc_ashran_officer_ironoreAI(p_Creature);
        }
};

/// Marketa <Stormshield Warlock Leader> - 82660
class npc_ashran_marketa : public CreatureScript
{
    public:
        npc_ashran_marketa() : CreatureScript("npc_ashran_marketa") { }

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
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForWarlock))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::GatewaysInvoked, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_marketaAI : public ScriptedAI
        {
            npc_ashran_marketaAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForWarlock, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_marketaAI(p_Creature);
        }
};

/// Ecilam <Stormshield Mage Leader> - 82966
class npc_ashran_ecilam : public CreatureScript
{
    public:
        npc_ashran_ecilam() : CreatureScript("npc_ashran_ecilam") { }

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
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForMage))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::PortalInvoked, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_ecilamAI : public ScriptedAI
        {
            npc_ashran_ecilamAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForMage, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_ecilamAI(p_Creature);
        }
};

/// Valant Brightsworn <Stormshield Paladin Leader> - 82893
class npc_ashran_valant_brightsworn : public CreatureScript
{
    public:
        npc_ashran_valant_brightsworn() : CreatureScript("npc_ashran_valant_brightsworn") { }

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
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForWarriorPaladin))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::RidersSummoned, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_valant_brightswornAI : public ScriptedAI
        {
            npc_ashran_valant_brightswornAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForWarriorPaladin, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_valant_brightswornAI(p_Creature);
        }
};

/// Anenga <Stormshield Druid Leader> - 81870
class npc_ashran_anenga : public CreatureScript
{
    public:
        npc_ashran_anenga() : CreatureScript("npc_ashran_anenga") { }

        enum eGossip
        {
            FangraalInvoked = 83895
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(p_Creature->GetZoneId());
            if (l_ZoneScript == nullptr)
                return false;

            if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
            {
                if (l_Ashran->IsArtifactEventLaunched(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForDruidShaman))
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->SEND_GOSSIP_MENU(eGossip::FangraalInvoked, p_Creature->GetGUID());
                    return true;
                }
            }

            return false;
        }

        struct npc_ashran_anengaAI : public ScriptedAI
        {
            npc_ashran_anengaAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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
                    l_Ashran->AddCollectedArtifacts(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForDruidShaman, l_ArtifactCount);
                    l_Ashran->RewardHonorAndReputation(l_ArtifactCount, p_Player);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_anengaAI(p_Creature);
        }
};

/// Kauper <Portal Guardian> - 84466
class npc_ashran_kauper : public CreatureScript
{
    public:
        npc_ashran_kauper() : CreatureScript("npc_ashran_kauper") { }

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

        struct npc_ashran_kauperAI : public ScriptedAI
        {
            npc_ashran_kauperAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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
                    l_Ashran->EndArtifactEvent(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForMage);
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

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_kauperAI(p_Creature);
        }
};

/// Decker Watts <Gateway Guardian> - 84651
/// Falcon Atherton <Gateway Guardian> - 84652
class npc_ashran_alliance_gateway_guardian : public CreatureScript
{
    public:
        npc_ashran_alliance_gateway_guardian() : CreatureScript("npc_ashran_alliance_gateway_guardian") { }

        enum eSpells
        {
            SpellCurseOfTheElements = 79956,
            SpellRainOfFire         = 165757,
            SpellShadowBolt         = 79932,
            SpellSummonFelguard     = 79958,
            SpellSoulLink           = 79957
        };

        enum eEvents
        {
            EventShadowBolt = 1,
            EventRainOfFire
        };

        struct npc_ashran_alliance_gateway_guardianAI : public ScriptedAI
        {
            npc_ashran_alliance_gateway_guardianAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                if (me->GetEntry() == eCreatures::FalconAtherton)
                {
                    me->CastSpell(me, eSpells::SpellSummonFelguard, true);
                    me->CastSpell(me, eSpells::SpellSoulLink, true);
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->CastSpell(p_Attacker, eSpells::SpellCurseOfTheElements, true);

                m_Events.ScheduleEvent(eEvents::EventShadowBolt, 1000);
                m_Events.ScheduleEvent(eEvents::EventRainOfFire, 6000);
            }

            void JustDied(Unit* p_Killer) override
            {
                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                    l_Ashran->EndArtifactEvent(TeamId::TEAM_ALLIANCE, eArtifactsDatas::CountForWarlock);
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
                    case eEvents::EventShadowBolt:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellShadowBolt, false);
                        m_Events.ScheduleEvent(eEvents::EventShadowBolt, 4000);
                        break;
                    case eEvents::EventRainOfFire:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellRainOfFire, false);
                        m_Events.ScheduleEvent(eEvents::EventRainOfFire, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_alliance_gateway_guardianAI(p_Creature);
        }
};

void AddSC_AshranNPCAlliance()
{
    new npc_rylai_crestfall();
    new npc_ashran_grimnir_sternhammer();
    new npc_ashran_misirin_stouttoe();
    new npc_ashran_stormshield_druid();
    new npc_ashran_officer_rumsfeld();
    new npc_ashran_officer_ironore();
    new npc_ashran_marketa();
    new npc_ashran_ecilam();
    new npc_ashran_valant_brightsworn();
    new npc_ashran_anenga();
    new npc_ashran_kauper();
    new npc_ashran_alliance_gateway_guardian();
}