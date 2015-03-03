////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
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
                TALK_ANNOUNCE_NEUTRAL_GRAVEYARD,
                TALK_ANNOUNCE_HORDE_GRAVEYARD,
                TALK_ANNOUNCE_ALLIANCE_GRAVEYARD
            };

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eAshranActions::AnnounceMarketplaceGraveyard:
                        Talk(eTalk::TALK_ANNOUNCE_NEUTRAL_GRAVEYARD);
                        break;
                    case eAshranActions::AnnounceHordeGraveyard:
                        Talk(eTalk::TALK_ANNOUNCE_HORDE_GRAVEYARD);
                        break;
                    case eAshranActions::AnnounceAllianceGraveyard:
                        Talk(eTalk::TALK_ANNOUNCE_ALLIANCE_GRAVEYARD);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
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
                TALK_HORDE_VICTORY,
                TALK_ALLIANCE_KILL_BOSS,
                TALK_ALLIANCE_VICTORY,
                TALK_HORDE_KILL_BOSS
            };

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eAshranActions::AnnounceHordeVictory:
                        Talk(eTalk::TALK_HORDE_VICTORY);
                        break;
                    case eAshranActions::AnnounceAllianceKillBoss:
                        Talk(eTalk::TALK_ALLIANCE_KILL_BOSS);
                        break;
                    case eAshranActions::AnnounceAllianceVictory:
                        Talk(eTalk::TALK_ALLIANCE_VICTORY);
                        break;
                    case eAshranActions::AnnounceHordeKillBoss:
                        Talk(eTalk::TALK_HORDE_KILL_BOSS);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
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

        struct npc_faction_bossAI : public BossAI
        {
            npc_faction_bossAI(Creature* p_Creature) : BossAI(p_Creature, 0)
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

            void Reset()
            {
                _Reset();

                m_Events.Reset();

                me->RemoveAura(eSpells::SpellEnableUnitFrame);
                me->RemoveAura(eSpells::SpellAshranLaneMobScaling);

                if (me->GetEntry() == eCreatures::GrandMarshalTremblade)
                    me->setFaction(12); ///< Alliance
            }

            void EnterCombat(Unit* p_Attacker)
            {
                _EnterCombat();

                Talk(eTalk::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventMortalCleave, 5000);
                m_Events.ScheduleEvent(eEvents::EventBladeTwister, 15000);

                me->CastSpell(me, eSpells::SpellEnableUnitFrame, true);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void JustDied(Unit* p_Killer)
            {
                _JustDied();

                Talk(eTalk::TalkDeath);

                uint64 l_GenericGuid = ((OutdoorPvPAshran*)m_ZoneScript)->GetFactionGenericMoP(me->GetEntry() == eCreatures::GrandMarshalTremblade ? TeamId::TEAM_ALLIANCE : TeamId::TEAM_HORDE);
                if (Creature* l_GenericMoP = sObjectAccessor->FindCreature(l_GenericGuid))
                    l_GenericMoP->AI()->DoAction(me->GetEntry() == eCreatures::GrandMarshalTremblade ? eAshranActions::AnnounceHordeKillBoss : eAshranActions::AnnounceAllianceKillBoss);

                ((OutdoorPvPAshran*)m_ZoneScript)->HandleFactionBossDeath(me->GetEntry() == eCreatures::GrandMarshalTremblade ? TeamId::TEAM_HORDE : TeamId::TEAM_ALLIANCE);

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
                {
                    /// Must do a * 100 because of currency precision
                    l_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_CONQUEST_POINTS, 250 * 100);
                    l_Player->RewardHonor(l_Player, 1, 50 * 100);
                }

                /// Trigger strongboxes loot for near players
                if (me->GetEntry() == eCreatures::GrandMarshalTremblade)
                    p_Killer->CastSpell(p_Killer, eAshranSpells::SpellAllianceReward, true);
                else
                    p_Killer->CastSpell(p_Killer, eAshranSpells::SpellHordeReward, true);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eAshranActions::WarspearOutpostInFight:
                    case eAshranActions::StormshieldStrongholdInFight:
                        Talk(eTalk::TalkIntro);
                        break;
                    case eAshranActions::WarspearVictory:
                    case eAshranActions::StormshieldVictory:
                        Talk(eTalk::TalkVictory);
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Target, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::SpellBladeTwisterSearcher)
                    me->CastSpell(p_Target, eSpells::SpellBladeTwisterMissile, false);
            }

            void UpdateAI(uint32 const p_Diff)
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

                if (AuraPtr l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffectPtr l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffectPtr l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
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

        bool OnGossipSelect(Player* p_Player, Creature*, uint32, uint32)
        {
            if (p_Player == nullptr || !p_Player->IsInWorld())
                return true;

            if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
                p_Player->ActivateTaxiPathTo(eAshranDatas::TaxiPathBaseHordeToAlliance, 0, true);
            else
                p_Player->ActivateTaxiPathTo(eAshranDatas::TaxiPathBaseAllianceToHorde, 0, true);

            return false;
        }

        struct npc_ashran_flight_mastersAI : public ScriptedAI
        {
            npc_ashran_flight_mastersAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_ATTACKABLE_1 | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
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

            void Reset()
            {
                me->setDeathState(DeathState::DEAD);
                me->SetGuidValue(EUnitFields::UNIT_FIELD_CHANNEL_OBJECT, me->GetGUID());
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_CHANNEL_SPELL, eAshranSpells::SpellSpiritHeal);
                me->SetFloatValue(EUnitFields::UNIT_FIELD_MOD_CASTING_SPEED, 1.0f);
                me->SetFloatValue(EUnitFields::UNIT_FIELD_MOD_SPELL_HASTE, 1.0f);
                DoCast(me, eAshranSpells::SpellSpiritHeal);
            }

            void JustRespawned() { }

            void UpdateAI(uint32 const)
            {
                if (!me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    DoCast(me, eAshranSpells::SpellSpiritHeal);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
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

            void Reset()
            {
                me->DisableHealthRegen();

                if (!m_IsAwake)
                    Talk(eTalk::TalkAwake);

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

            void EnterEvadeMode()
            {
                /// Copy/Paste of classic EnterEvade mode but without RemoveAllAuras
                /// Sometimes bosses stuck in combat?
                me->DeleteThreatList();
                me->CombatStop(true);
                me->LoadCreaturesAddon();
                me->SetLootRecipient(NULL);
                me->ResetPlayerDamageReq();
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_InFight = true;

                m_Events.Reset();

                m_Events.ScheduleEvent(eEvents::EventCrushingLeap, 10000);
                m_Events.ScheduleEvent(eEvents::EventBoomingShout, 5000);
                m_Events.ScheduleEvent(eEvents::EventMASSIVEKick, 20000);
                m_Events.ScheduleEvent(eEvents::EventBoonOfKorlok, 30000);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalk::TalkSlay);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (me->getFaction() == eFactions::KorlokNeutral)
                {
                    p_Damage = 0;
                    return;
                }

                if (p_Damage >= me->GetHealth())
                {
                    Talk(eTalk::TalkDeath);
                    p_Damage = 0;
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    me->setFaction(eFactions::KorlokNeutral);
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    ((OutdoorPvPAshran*)m_OutdoorPvP)->EndEvent(eAshranEvents::EventKorlokTheOgreKing);
                }
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eActions::ActionAllianceRecruit:
                        Talk(eTalk::TalkRecruitedByAlliance);
                        me->setFaction(eFactions::KorlokForAlliance);
                        HandleJumpToFight();
                        break;
                    case eActions::ActionHordeRecruit:
                        Talk(eTalk::TalkRecruitedByHorde);
                        me->setFaction(eFactions::KorlokForHorde);
                        HandleJumpToFight();
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* p_Target, SpellInfo const* p_SpellInfo)
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

            void MovementInform(uint32 p_Type, uint32 p_ID)
            {
                if (p_ID == 1)
                {
                    me->CastSpell(me, eSpells::SpellOgreicLanding, true);

                    Position l_Pos;
                    me->GetPosition(&l_Pos);
                    me->SetHomePosition(l_Pos);
                }
            }

            void UpdateAI(uint32 const p_Diff)
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

                if (AuraPtr l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffectPtr l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffectPtr l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }

            void HandleJumpToFight()
            {
                ((OutdoorPvPAshran*)m_OutdoorPvP)->EndEvent(eAshranEvents::EventKorlokTheOgreKing, false);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                me->CastSpell(me, eSpells::SpellCurseOfKrong, true);
                me->CastSpell(me, eSpells::SpellCurseOfKorlok, true);   ///< Stacks every 30s ... +5% damage taken
                events.ScheduleEvent(eEvents::EventCurseOfKorlok, 30000);

                me->CastSpell(me, eSpells::SpellOgreicLeap, false);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
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

            void Reset()
            {
                me->DisableHealthRegen();

                m_Events.Reset();

                me->CastSpell(me, eSpells::AshranLaneMobScalingAura, true);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(eEvents::EventCrushingLeap, 10000);
                m_Events.ScheduleEvent(eEvents::EventBoomingShout, 5000);
                m_Events.ScheduleEvent(eEvents::EventMASSIVEKick, 20000);
            }

            void JustDied(Unit* p_Killer)
            {
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

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (me->HasAura(eSpells::SpellEnrage))
                    return;

                if (me->HealthBelowPctDamaged(50, p_Damage))
                    me->CastSpell(me, eSpells::SpellEnrage, true);
            }

            void UpdateAI(uint32 const p_Diff)
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

                if (AuraPtr l_Scaling = me->GetAura(eSpells::AshranLaneMobScalingAura))
                {
                    if (AuraEffectPtr l_Damage = l_Scaling->GetEffect(EFFECT_0))
                        l_Damage->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                    if (AuraEffectPtr l_Health = l_Scaling->GetEffect(EFFECT_1))
                        l_Health->ChangeAmount(eAshranDatas::HealthPCTAddedByHostileRef * l_Count);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ashran_faction_championsAI(p_Creature);
        }
};

void AddSC_AshranNPCNeutral()
{
    new npc_ashran_herald();
    new npc_slg_generic_mop();
    new npc_faction_boss();
    new npc_ashran_flight_masters();
    new npc_ashran_spirit_healer();
    new npc_ashran_korlok();
    new npc_ashran_faction_champions();
}