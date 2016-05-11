////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// A'shran Herald - 84113
class npc_ashran_herald : public CreatureScript
{
    public:
        npc_ashran_herald() : CreatureScript("npc_ashran_herald") { }

        struct npc_ashran_heraldAI : public ScriptedAI
        {
            npc_ashran_heraldAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eTalk
            {
                AnnounceNeutralGraveyard,
                AnnounceHordeGraveyard,
                AnnounceAllianceGraveyard
            };

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAshranActions::AnnounceMarketplaceGraveyard:
                        Talk(eTalk::AnnounceNeutralGraveyard, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    case eAshranActions::AnnounceHordeGraveyard:
                        Talk(eTalk::AnnounceHordeGraveyard, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    case eAshranActions::AnnounceAllianceGraveyard:
                        Talk(eTalk::AnnounceAllianceGraveyard, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_heraldAI(p_Creature);
        }
};

/// SLG Generic MoP (Large AOI) - 68553
class npc_slg_generic_mop : public CreatureScript
{
    public:
        npc_slg_generic_mop() : CreatureScript("npc_slg_generic_mop") { }

        struct npc_slg_generic_mopAI : public ScriptedAI
        {
            npc_slg_generic_mopAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eTalk
            {
                HordeVictory,
                AllianceKillBoss,
                AllianceVictory,
                HordeKillBoss
            };

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAshranActions::AnnounceHordeVictory:
                        Talk(eTalk::HordeVictory, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    case eAshranActions::AnnounceAllianceKillBoss:
                        Talk(eTalk::AllianceKillBoss, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    case eAshranActions::AnnounceAllianceVictory:
                        Talk(eTalk::AllianceVictory, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    case eAshranActions::AnnounceHordeKillBoss:
                        Talk(eTalk::HordeKillBoss, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_slg_generic_mopAI(p_Creature);
        }
};

/// High Warlord Volrath - 82877
/// Grand Marshal Tremblade - 82876
class npc_faction_boss : public CreatureScript
{
    public:
        npc_faction_boss() : CreatureScript("npc_faction_boss") { }

        struct npc_faction_bossAI : public ScriptedAI
        {
            npc_faction_bossAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_ZoneScript = sOutdoorPvPMgr->GetZoneScript(p_Creature->GetZoneId());
            }

            enum eSpells
            {
                SpellBladeTwisterSearcher   = 178798,   ///< Uses 178797 on the target (Only 1)
                SpellBladeTwisterMissile    = 178797,   ///< Launch 178795, Summons 89320
                SpellMortalCleave           = 177147,
                SpellEnableUnitFrame        = 177684,

                SpellAshranLaneMobScaling   = 178838,
                AshranLaneMobScalingAura    = 164310
            };

            enum eTalk
            {
                TalkIntro,
                TalkAggro,
                TalkSlay,
                TalkDeath,
                TalkVictory
            };

            enum eEvents
            {
                EventMortalCleave = 1,
                EventBladeTwister
            };

            EventMap m_Events;
            ZoneScript* m_ZoneScript;

            void Reset() override
            {
                m_Events.Reset();

                me->RemoveAura(eSpells::SpellEnableUnitFrame);
                me->RemoveAura(eSpells::SpellAshranLaneMobScaling);

                if (me->GetEntry() == eCreatures::GrandMarshalTremblade)
                    me->setFaction(12); ///< Alliance
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                Talk(eTalk::TalkAggro, 0, TextRange::TEXT_RANGE_MAP);

                m_Events.ScheduleEvent(eEvents::EventMortalCleave, 5000);
                m_Events.ScheduleEvent(eEvents::EventBladeTwister, 15000);

                me->CastSpell(me, eSpells::SpellEnableUnitFrame, true);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay, 0, TextRange::TEXT_RANGE_MAP);
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalk::TalkDeath, 0, TextRange::TEXT_RANGE_MAP);

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)m_ZoneScript)
                {
                    uint64 l_GenericGuid = l_Ashran->GetFactionGenericMoP(me->GetEntry() == eCreatures::GrandMarshalTremblade ? TeamId::TEAM_ALLIANCE : TeamId::TEAM_HORDE);
                    if (Creature* l_GenericMoP = sObjectAccessor->FindCreature(l_GenericGuid))
                        l_GenericMoP->AI()->DoAction(me->GetEntry() == eCreatures::GrandMarshalTremblade ? eAshranActions::AnnounceHordeKillBoss : eAshranActions::AnnounceAllianceKillBoss);

                    l_Ashran->HandleFactionBossDeath(me->GetEntry() == eCreatures::GrandMarshalTremblade ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE);
                }

                /// Upon successfully defeating the enemy leader, those present receive 50 Honor and 250 Conquest
                std::list<Player*> l_PlayerList;
                me->GetPlayerListInGrid(l_PlayerList, 100.0f);

                l_PlayerList.remove_if([this](Player* p_Player) -> bool
                {
                    if (p_Player == nullptr)
                        return true;

                    if (!me->IsValidAttackTarget(p_Player))
                        return true;

                    return false;
                });

                for (Player* l_Player : l_PlayerList)
                    l_Player->RewardHonor(l_Player, 1, 50 * 100);

                /// Trigger strongboxes loot for near players
                if (me->GetEntry() == eCreatures::GrandMarshalTremblade)
                    p_Killer->CastSpell(p_Killer, eAshranSpells::SpellAllianceReward, true);
                else
                    p_Killer->CastSpell(p_Killer, eAshranSpells::SpellHordeReward, true);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAshranActions::WarspearOutpostInFight:
                    case eAshranActions::StormshieldStrongholdInFight:
                        Talk(eTalk::TalkIntro, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    case eAshranActions::WarspearVictory:
                    case eAshranActions::StormshieldVictory:
                        Talk(eTalk::TalkVictory, 0, TextRange::TEXT_RANGE_MAP);
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_SpellInfo->Id == eSpells::SpellBladeTwisterSearcher)
                    me->CastSpell(p_Target, eSpells::SpellBladeTwisterMissile, false);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        EnterEvadeMode();
                    return;
                }

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventMortalCleave:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellMortalCleave, false);
                        m_Events.ScheduleEvent(eEvents::EventMortalCleave, 15000);
                        break;
                    case eEvents::EventBladeTwister:
                        me->CastSpell(me, eSpells::SpellBladeTwisterSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventBladeTwister, 30000);
                        break;
                    default:
                        break;
                }

                EnterEvadeIfOutOfCombatArea(p_Diff);
                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_faction_bossAI(p_Creature);
        }
};

/// Shevan Manille <Flight Master> - 87672
/// Tina Kelatara <Flight Master> - 87617
class npc_ashran_flight_masters : public CreatureScript
{
    public:
        npc_ashran_flight_masters() : CreatureScript("npc_ashran_flight_masters") { }

        bool OnGossipSelect(Player* p_Player, Creature*, uint32, uint32) override
        {
            if (p_Player == nullptr || !p_Player->IsInWorld())
                return true;

            if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
                p_Player->ActivateTaxiPathTo(eAshranDatas::TaxiPathBaseHordeToAlliance);
            else
                p_Player->ActivateTaxiPathTo(eAshranDatas::TaxiPathBaseAllianceToHorde);

            return false;
        }

        struct npc_ashran_flight_mastersAI : public ScriptedAI
        {
            npc_ashran_flight_mastersAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_ATTACKABLE_1 | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_flight_mastersAI(p_Creature);
        }
};

/// Alliance Spirit Guide - 80723
/// Horde Spirit Guide - 80724
class npc_ashran_spirit_healer : public CreatureScript
{
    public:
        npc_ashran_spirit_healer() : CreatureScript("npc_ashran_spirit_healer") { }

        struct npc_ashran_spirit_healerAI : public ScriptedAI
        {
            npc_ashran_spirit_healerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->setDeathState(DeathState::DEAD);
                me->SetGuidValue(EUnitFields::UNIT_FIELD_CHANNEL_OBJECT, me->GetGUID());
                me->SetChannelSpellID(eAshranSpells::SpellSpiritHeal);
                me->SetFloatValue(EUnitFields::UNIT_FIELD_MOD_CASTING_SPEED, 1.0f);
                me->SetFloatValue(EUnitFields::UNIT_FIELD_MOD_SPELL_HASTE, 1.0f);
                DoCast(me, eAshranSpells::SpellSpiritHeal);
            }

            void JustRespawned() override { }

            void UpdateAI(uint32 const) override
            {
                if (!me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    DoCast(me, eAshranSpells::SpellSpiritHeal);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_spirit_healerAI(p_Creature);
        }
};

/// Kor'lok <The Ogre King> - 80858
class npc_ashran_korlok : public CreatureScript
{
    public:
        npc_ashran_korlok() : CreatureScript("npc_ashran_korlok") { }

        struct npc_ashran_korlokAI : public ScriptedAI
        {
            npc_ashran_korlokAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_OutdoorPvP = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(p_Creature->GetZoneId());
                m_IsAwake = false;
                m_InFight = false;
            }

            enum eSpells
            {
                SpellBoomingShout   = 177150,
                SpellBoonOfKorlok   = 177164,
                SpellCrushingLeap   = 164819,
                SpellMASSIVEKick    = 177157,
                SpellOgreicLanding  = 165096,
                SpellOgreicLeap     = 164854,

                /// Debuff self casted
                SpellCurseOfKorlok  = 165192,
                SpellCurseOfKrong   = 165134,

                /// Cosmetic
                ShadowyGhostCosmeticSpawnSpellBlue  = 156204,
                AshranLaneMobScalingAura            = 164310
            };

            enum eTalk
            {
                TalkAwake,
                TalkRecruitedByAlliance,
                TalkRecruitedByHorde,
                TalkSlay,
                TalkDeath
            };

            enum eEvents
            {
                EventCrushingLeap = 1,
                EventBoomingShout,
                EventMASSIVEKick,
                EventBoonOfKorlok,
                EventCurseOfKorlok
            };

            enum eActions
            {
                ActionHordeRecruit,
                ActionAllianceRecruit
            };

            EventMap m_Events;
            OutdoorPvP* m_OutdoorPvP;

            bool m_IsAwake;
            bool m_InFight;

            void Reset() override
            {
                me->DisableHealthRegen();

                if (!m_IsAwake)
                    Talk(eTalk::TalkAwake, 0, TextRange::TEXT_RANGE_MAP);

                m_Events.Reset();

                m_IsAwake = true;

                if (!m_InFight)
                {
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    me->setFaction(eFactions::KorlokNeutral);
                    me->CastSpell(me, eSpells::ShadowyGhostCosmeticSpawnSpellBlue, true);
                    me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);
                }
            }

            void EnterEvadeMode() override
            {
                /// Copy/Paste of classic EnterEvade mode but without RemoveAllAuras
                /// Sometimes bosses stuck in combat?
                me->DeleteThreatList();
                me->CombatStop(true);
                me->LoadCreaturesAddon();
                me->SetLootRecipient(NULL);
                me->ResetPlayerDamageReq();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_InFight = true;

                m_Events.Reset();

                m_Events.ScheduleEvent(eEvents::EventCrushingLeap, 10000);
                m_Events.ScheduleEvent(eEvents::EventBoomingShout, 5000);
                m_Events.ScheduleEvent(eEvents::EventMASSIVEKick, 20000);
                m_Events.ScheduleEvent(eEvents::EventBoonOfKorlok, 30000);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay, 0, TextRange::TEXT_RANGE_MAP);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/) override
            {
                if (me->getFaction() == eFactions::KorlokNeutral)
                {
                    p_Damage = 0;
                    return;
                }

                if (p_Damage >= me->GetHealth())
                {
                    Talk(eTalk::TalkDeath, 0, TextRange::TEXT_RANGE_MAP);
                    p_Damage = 0;
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    me->setFaction(eFactions::KorlokNeutral);
                    me->SetReactState(ReactStates::REACT_PASSIVE);

                    if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)m_OutdoorPvP)
                        l_Ashran->EndEvent(eAshranEvents::EventKorlokTheOgreKing);
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::ActionAllianceRecruit:
                        Talk(eTalk::TalkRecruitedByAlliance, 0, TextRange::TEXT_RANGE_MAP);
                        me->setFaction(eFactions::KorlokForAlliance);
                        HandleJumpToFight();
                        break;
                    case eActions::ActionHordeRecruit:
                        Talk(eTalk::TalkRecruitedByHorde, 0, TextRange::TEXT_RANGE_MAP);
                        me->setFaction(eFactions::KorlokForHorde);
                        HandleJumpToFight();
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* /*p_Target*/, SpellInfo const* p_SpellInfo) override
            {
                if (p_SpellInfo->Id == eSpells::SpellOgreicLeap)
                {
                    OutdoorPvPAshran* l_Ashran = ((OutdoorPvPAshran*)m_OutdoorPvP);
                    if (l_Ashran == nullptr)
                        return;

                    if (Creature* l_SLG = Creature::GetCreature(*me, l_Ashran->GetGenericMoPGuid(l_Ashran->GetCurrentBattleType())))
                    {
                        Position l_Pos;
                        l_SLG->GetPosition(&l_Pos);
                        me->GetMotionMaster()->MoveJump(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, 100.0f, 100.0f, me->GetOrientation(), 1);
                    }
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID) override
            {
                if (p_ID == 1)
                {
                    me->CastSpell(me, eSpells::SpellOgreicLanding, true);

                    Position l_Pos;
                    me->GetPosition(&l_Pos);
                    me->SetHomePosition(l_Pos);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                /// Only for Curse of Kor'lok
                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case eEvents::EventCurseOfKorlok:
                        me->CastSpell(me, eSpells::SpellCurseOfKorlok, true);   ///< Stacks every 30s ... +5% damage taken
                        events.ScheduleEvent(eEvents::EventCurseOfKorlok, 30000);
                        break;
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCrushingLeap:
                    {
                        me->CastSpell(me, eSpells::SpellCrushingLeap, false);
                        m_Events.ScheduleEvent(eEvents::EventCrushingLeap, 40000);
                        break;
                    }
                    case eEvents::EventBoomingShout:
                    {
                        me->CastSpell(me, eSpells::SpellBoomingShout, false);
                        m_Events.ScheduleEvent(eEvents::EventBoomingShout, 50000);
                        break;
                    }
                    case eEvents::EventMASSIVEKick:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::SpellMASSIVEKick, true);
                            DoModifyThreatPercent(l_Target, -100);
                        }

                        m_Events.ScheduleEvent(eEvents::EventMASSIVEKick, 60000);
                        break;
                    }
                    case eEvents::EventBoonOfKorlok:
                    {
                        me->CastSpell(me, eSpells::SpellBoonOfKorlok, true);
                        m_Events.ScheduleEvent(eEvents::EventBoonOfKorlok, 60000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }

            void HandleJumpToFight()
            {
                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)m_OutdoorPvP)
                    l_Ashran->EndEvent(eAshranEvents::EventKorlokTheOgreKing, false);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                me->CastSpell(me, eSpells::SpellCurseOfKrong, true);
                me->CastSpell(me, eSpells::SpellCurseOfKorlok, true);   ///< Stacks every 30s ... +5% damage taken
                events.ScheduleEvent(eEvents::EventCurseOfKorlok, 30000);

                me->CastSpell(me, eSpells::SpellOgreicLeap, false);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_korlokAI(p_Creature);
        }
};

/// Muk'Mar Raz <Horde Champion> - 81725
/// Gaul Dun Firok <Alliance Champion> - 81726
class npc_ashran_faction_champions : public CreatureScript
{
    public:
        npc_ashran_faction_champions() : CreatureScript("npc_ashran_faction_champions") { }

        struct npc_ashran_faction_championsAI : public ScriptedAI
        {
            npc_ashran_faction_championsAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_OutdoorPvP = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(p_Creature->GetZoneId());

                m_Rewarded = false;
            }

            enum eSpells
            {
                SpellBoomingShout           = 177150,
                SpellCrushingLeap           = 164819,
                SpellMASSIVEKick            = 177157,
                SpellEnrage                 = 164811,
                AshranLaneMobScalingAura    = 164310
            };

            enum eEvents
            {
                EventCrushingLeap = 1,
                EventBoomingShout,
                EventMASSIVEKick
            };

            enum eActions
            {
                ActionHordeRecruit,
                ActionAllianceRecruit
            };

            EventMap m_Events;
            OutdoorPvP* m_OutdoorPvP;

            bool m_Rewarded;

            void Reset() override
            {
                me->DisableHealthRegen();

                m_Events.Reset();

                me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventCrushingLeap, 10000);
                m_Events.ScheduleEvent(eEvents::EventBoomingShout, 5000);
                m_Events.ScheduleEvent(eEvents::EventMASSIVEKick, 20000);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (m_OutdoorPvP == nullptr)
                    return;

                Creature* l_Korlok = sObjectAccessor->FindCreature(m_OutdoorPvP->GetCreature(eSpecialSpawns::NeutralKorlokTheOgreKing));
                if (l_Korlok == nullptr || !l_Korlok->IsAIEnabled)    ///< Shouldn't happens
                    return;

                if (p_Killer->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (p_Killer->ToPlayer()->GetTeamId() == TeamId::TEAM_ALLIANCE)
                        l_Korlok->AI()->DoAction(eActions::ActionAllianceRecruit);
                    else
                        l_Korlok->AI()->DoAction(eActions::ActionHordeRecruit);
                }
                else if (p_Killer->GetOwner() && p_Killer->GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    if (Player* l_Owner = p_Killer->GetOwner()->ToPlayer())
                    {
                        if (l_Owner->ToPlayer()->GetTeamId() == TeamId::TEAM_ALLIANCE)
                            l_Korlok->AI()->DoAction(eActions::ActionAllianceRecruit);
                        else
                            l_Korlok->AI()->DoAction(eActions::ActionHordeRecruit);
                    }
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/) override
            {
                if (p_Damage < me->GetHealth())
                {
                    if (me->HasAura(eSpells::SpellEnrage))
                        return;

                    if (me->HealthBelowPctDamaged(50, p_Damage))
                        me->CastSpell(me, eSpells::SpellEnrage, true);

                    return;
                }

                if (m_Rewarded)
                    return;

                ZoneScript* l_ZoneScript = sOutdoorPvPMgr->GetOutdoorPvPToZoneId(me->GetZoneId());
                if (l_ZoneScript == nullptr)
                    return;

                if (OutdoorPvPAshran* l_Ashran = (OutdoorPvPAshran*)l_ZoneScript)
                    l_Ashran->CastSpellOnTeam(me, TeamId::TEAM_ALLIANCE, eAshranSpells::SpellEventAllianceReward);

                m_Rewarded = true;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCrushingLeap:
                    {
                        me->CastSpell(me, eSpells::SpellCrushingLeap, false);
                        m_Events.ScheduleEvent(eEvents::EventCrushingLeap, 40000);
                        break;
                    }
                    case eEvents::EventBoomingShout:
                    {
                        me->CastSpell(me, eSpells::SpellBoomingShout, false);
                        m_Events.ScheduleEvent(eEvents::EventBoomingShout, 50000);
                        break;
                    }
                    case eEvents::EventMASSIVEKick:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->CastSpell(l_Target, eSpells::SpellMASSIVEKick, true);
                            DoModifyThreatPercent(l_Target, -100);
                        }

                        m_Events.ScheduleEvent(eEvents::EventMASSIVEKick, 60000);
                        break;
                    }
                    default:
                        break;
                }

                EnterEvadeIfOutOfCombatArea(p_Diff);
                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_faction_championsAI(p_Creature);
        }
};

/// Mandragoraster - 83683
class npc_ashran_mandragoraster : public CreatureScript
{
    public:
        npc_ashran_mandragoraster() : CreatureScript("npc_ashran_mandragoraster") { }

        struct npc_ashran_mandragorasterAI : public ScriptedAI
        {
            npc_ashran_mandragorasterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                AshranLaneMobScalingAura    = 164310,

                SpellSplittingBreath        = 161520,

                SpellPiercingChomp          = 161932,
                SpellPiercingChompAura      = 161933
            };

            enum eEvents
            {
                EventSplittingBreath = 1,
                EventPiercingChomp
            };

            EventMap m_Events;

            void Reset() override
            {
                me->DisableHealthRegen();

                me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventSplittingBreath, 5000);
                m_Events.ScheduleEvent(eEvents::EventPiercingChomp, 8000);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::SpellPiercingChomp)
                    me->CastSpell(p_Target, eSpells::SpellPiercingChompAura, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventSplittingBreath:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellSplittingBreath, false);
                        m_Events.ScheduleEvent(eEvents::EventSplittingBreath, 15000);
                        break;
                    case eEvents::EventPiercingChomp:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellPiercingChomp, false);
                        m_Events.ScheduleEvent(eEvents::EventPiercingChomp, 8000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_mandragorasterAI(p_Creature);
        }
};

/// Panthora - 83691
class npc_ashran_panthora : public CreatureScript
{
    public:
        npc_ashran_panthora() : CreatureScript("npc_ashran_panthora") { }

        struct npc_ashran_panthoraAI : public ScriptedAI
        {
            npc_ashran_panthoraAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                AshranLaneMobScalingAura    = 164310,

                SpellShadowClaws            = 176542
            };

            enum eEvents
            {
                EventShadowClaws = 1
            };

            EventMap m_Events;

            void Reset() override
            {
                me->DisableHealthRegen();

                me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventShadowClaws, 5000);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventShadowClaws:
                        me->CastSpell(me, eSpells::SpellShadowClaws, true);
                        m_Events.ScheduleEvent(eEvents::EventShadowClaws, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_panthoraAI(p_Creature);
        }
};

/// Ancient Inferno - 84875
class npc_ashran_ancient_inferno : public CreatureScript
{
    public:
        npc_ashran_ancient_inferno() : CreatureScript("npc_ashran_ancient_inferno") { }

        struct npc_ashran_ancient_infernoAI : public ScriptedAI
        {
            npc_ashran_ancient_infernoAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                AshranLaneMobScalingAura    = 164310,

                SpellLavaBurst              = 176170,
                SpellMoltenFirestorm        = 176171,
                SpellVolcanicActivitySearch = 176130,
                SpellVolcanicActivityMissil = 176132
            };

            enum eEvents
            {
                EventLavaBurst = 1,
                EventVolcanicActivity,
                EventMoltenFirestorm
            };

            EventMap m_Events;

            void Reset() override
            {
                me->DisableHealthRegen();

                me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventLavaBurst, 2000);
                m_Events.ScheduleEvent(eEvents::EventVolcanicActivity, 8000);
                m_Events.ScheduleEvent(eEvents::EventMoltenFirestorm, 10000);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::SpellVolcanicActivitySearch)
                    me->CastSpell(p_Target, eSpells::SpellVolcanicActivityMissil, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventLavaBurst:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellLavaBurst, false);
                        m_Events.ScheduleEvent(eEvents::EventLavaBurst, 6000);
                        break;
                    case eEvents::EventVolcanicActivity:
                        me->CastSpell(me, eSpells::SpellVolcanicActivitySearch, true);
                        m_Events.ScheduleEvent(eEvents::EventVolcanicActivity, 15000);
                        break;
                    case eEvents::EventMoltenFirestorm:
                        me->CastSpell(me, eSpells::SpellMoltenFirestorm, true);
                        m_Events.ScheduleEvent(eEvents::EventMoltenFirestorm, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_ancient_infernoAI(p_Creature);
        }
};

/// Volcano - 88227
class npc_ashran_volcano : public CreatureScript
{
    public:
        npc_ashran_volcano() : CreatureScript("npc_ashran_volcano") { }

        struct npc_ashran_volcanoAI : public ScriptedAI
        {
            npc_ashran_volcanoAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpell
            {
                VolcanoAreaTrigger = 176144
            };

            void Reset() override
            {
                me->CastSpell(me, eSpell::VolcanoAreaTrigger, true);

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(uint32 const /*p_Diff*/) override { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_volcanoAI(p_Creature);
        }
};

/// Goregore - 84893
class npc_ashran_goregore : public CreatureScript
{
    public:
        npc_ashran_goregore() : CreatureScript("npc_ashran_goregore") { }

        struct npc_ashran_goregoreAI : public ScriptedAI
        {
            npc_ashran_goregoreAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                AshranLaneMobScalingAura    = 164310,
                SpellCranky                 = 169710
            };

            enum eEvents
            {
                EventCranky = 1
            };

            EventMap m_Events;

            void Reset() override
            {
                me->DisableHealthRegen();

                me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventCranky, 1000);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCranky:
                        me->CastSpell(me, eSpells::SpellCranky, true);
                        m_Events.ScheduleEvent(eEvents::EventCranky, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_goregoreAI(p_Creature);
        }
};

/// Ashmaul Magma Caster - 84906
class npc_ashran_ashmaul_magma_caster : public CreatureScript
{
    public:
        npc_ashran_ashmaul_magma_caster() : CreatureScript("npc_ashran_ashmaul_magma_caster") { }

        struct npc_ashran_ashmaul_magma_casterAI : public ScriptedAI
        {
            npc_ashran_ashmaul_magma_casterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                SpellLavaBurstVolley    = 169725,
                SpellVolcanicGround     = 169724
            };

            enum eEvents
            {
                EventLavaBurstVolley = 1,
                EventVolcanicGround
            };

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventLavaBurstVolley, 3000);
                m_Events.ScheduleEvent(eEvents::EventVolcanicGround, 8000);
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
                    case eEvents::EventLavaBurstVolley:
                        me->CastSpell(me, eSpells::SpellLavaBurstVolley, false);
                        m_Events.ScheduleEvent(eEvents::EventLavaBurstVolley, 10000);
                        break;
                    case eEvents::EventVolcanicGround:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellVolcanicGround, false);
                        m_Events.ScheduleEvent(eEvents::EventVolcanicGround, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_ashmaul_magma_casterAI(p_Creature);
        }
};

/// Volcanic Ground - 84952
class npc_ashran_volcanic_ground : public CreatureScript
{
    public:
        npc_ashran_volcanic_ground() : CreatureScript("npc_ashran_volcanic_ground") { }

        struct npc_ashran_volcanic_groundAI : public ScriptedAI
        {
            npc_ashran_volcanic_groundAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpell
            {
                VolcanicGround = 169723
            };

            void Reset() override
            {
                me->CastSpell(me, eSpell::VolcanicGround, true);

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(uint32 const /*p_Diff*/) override { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_volcanic_groundAI(p_Creature);
        }
};

/// Elder Darkweaver Kath - 85771
class npc_ashran_elder_darkweaver_kath : public CreatureScript
{
    public:
        npc_ashran_elder_darkweaver_kath() : CreatureScript("npc_ashran_elder_darkweaver_kath") { }

        struct npc_ashran_elder_darkweaver_kathAI : public ScriptedAI
        {
            npc_ashran_elder_darkweaver_kathAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                AshranLaneMobScalingAura    = 164310,

                SpellDarknessWithin         = 158830,
                SpellDarknessWithinSearcher = 158844,
                SpellDarknessWithinMissile  = 158845,
                SpellShadowFigurinesSearch  = 158854,
                SpellShadowFigurinesSpawn   = 158719
            };

            enum eEvents
            {
                EventDarknessWithin = 1,
                EventShadowFigurines
            };

            EventMap m_Events;
            uint8 m_SummonCount;

            void Reset() override
            {
                m_SummonCount = 0;

                me->DisableHealthRegen();

                me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventDarknessWithin, 2000);
                m_Events.ScheduleEvent(eEvents::EventShadowFigurines, 10000);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::SpellDarknessWithinSearcher:
                        me->CastSpell(p_Target, eSpells::SpellDarknessWithinMissile, true);
                        break;
                    case eSpells::SpellShadowFigurinesSearch:
                        if (m_SummonCount >= 3)
                            break;
                        me->CastSpell(p_Target, eSpells::SpellShadowFigurinesSpawn, true);
                        ++m_SummonCount;
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                HandleHealthAndDamageScaling();

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventDarknessWithin:
                        me->CastSpell(me, eSpells::SpellDarknessWithin, false);
                        m_Events.ScheduleEvent(eEvents::EventDarknessWithin, 15000);
                        break;
                    case eEvents::EventShadowFigurines:
                        m_SummonCount = 0;
                        me->CastSpell(me, eSpells::SpellShadowFigurinesSearch, false);
                        m_Events.ScheduleEvent(eEvents::EventShadowFigurines, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void HandleHealthAndDamageScaling()
            {
                std::list<HostileReference*> l_ThreatList = me->getThreatManager().getThreatList();
                uint32 l_Count = std::count_if(l_ThreatList.begin(), l_ThreatList.end(), [this](HostileReference* p_HostileRef) -> bool
                {
                    Unit* l_Unit = Unit::GetUnit(*me, p_HostileRef->getUnitGuid());
                    return l_Unit && l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER;
                });

                if (Aura* l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffect* l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffect* l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_elder_darkweaver_kathAI(p_Creature);
        }
};

/// Shadow Figurine - 78620
class npc_ashran_shadow_figurine : public CreatureScript
{
    public:
        npc_ashran_shadow_figurine() : CreatureScript("npc_ashran_shadow_figurine") { }

        struct npc_ashran_shadow_figurineAI : public ScriptedAI
        {
            npc_ashran_shadow_figurineAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpell
            {
                ShadowChains = 158714
            };

            void Reset() override
            {
                me->CastSpell(me, eSpell::ShadowChains, true);

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void UpdateAI(uint32 const /*p_Diff*/) override { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_shadow_figurineAI(p_Creature);
        }
};

/// Ashmaul Destroyer - 84876
class npc_ashran_ashmaul_destroyer : public CreatureScript
{
    public:
        npc_ashran_ashmaul_destroyer() : CreatureScript("npc_ashran_ashmaul_destroyer") { }

        struct npc_ashran_ashmaul_destroyerAI : public ScriptedAI
        {
            npc_ashran_ashmaul_destroyerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                SpellEarthSmash = 176187
            };

            enum eEvents
            {
                EventEarthSmash = 1
            };

            EventMap m_Events;

            void Reset() override
            {
                me->DisableHealthRegen();

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvents::EventEarthSmash, 1000);
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
                    case eEvents::EventEarthSmash:
                        me->CastSpell(me, eSpells::SpellEarthSmash, false);
                        m_Events.ScheduleEvent(eEvents::EventEarthSmash, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_ashran_ashmaul_destroyerAI(p_Creature);
        }
};

/// Fen Tao - 91483
class npc_ashran_fen_tao : public CreatureScript
{
    public:
        npc_ashran_fen_tao() : CreatureScript("npc_ashran_fen_tao") { }

        enum SpellIds
        {
            SpellAddFollowerFenTao = 181526
        };

        enum MiscDatas
        {
            GossipMenuId    = 90007,
            FirstNpcTextID  = 92005,
            SecondNpcTextID = 92006
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            p_Player->PrepareGossipMenu(p_Creature, MiscDatas::GossipMenuId, false);
            GossipMenuItem const* l_Item = p_Player->PlayerTalkClass->GetGossipMenu().GetItem(MiscDatas::FirstNpcTextID);

            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, l_Item ? l_Item->Message : "Why are you here?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->SEND_GOSSIP_MENU(MiscDatas::FirstNpcTextID, p_Creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
        {
            p_Player->PlayerTalkClass->ClearMenus();

            switch (p_Action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                {
                    char const* l_NpcName = "Fen Tao";

                    p_Creature->SendAddFollowerQuery(p_Player, p_Sender, GOSSIP_ACTION_INFO_DEF + 2, l_NpcName);
                    p_Player->SEND_GOSSIP_MENU(MiscDatas::SecondNpcTextID, p_Creature->GetGUID());
                    break;
                }
                case GOSSIP_ACTION_INFO_DEF + 2:
                    p_Player->CastSpell(p_Player, SpellIds::SpellAddFollowerFenTao, true);
                    p_Player->PlayerTalkClass->SendCloseGossip();
                    break;
                default:
                    p_Player->PlayerTalkClass->SendCloseGossip();
                    break;
            }

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_AshranNPCNeutral()
{
    new npc_ashran_herald();
    new npc_slg_generic_mop();
    new npc_faction_boss();
    new npc_ashran_flight_masters();
    new npc_ashran_spirit_healer();
    new npc_ashran_korlok();
    new npc_ashran_faction_champions();
    new npc_ashran_mandragoraster();
    new npc_ashran_panthora();
    new npc_ashran_ancient_inferno();
    new npc_ashran_volcano();
    new npc_ashran_goregore();
    new npc_ashran_ashmaul_magma_caster();
    new npc_ashran_volcanic_ground();
    new npc_ashran_elder_darkweaver_kath();
    new npc_ashran_shadow_figurine();
    new npc_ashran_ashmaul_destroyer();
    new npc_ashran_fen_tao();
}
#endif
