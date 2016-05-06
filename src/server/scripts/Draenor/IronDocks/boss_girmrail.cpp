////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eTalks
{
    /// Noxx
    TalkNoxxIntro       = 50,   ///< Is there killing to be done? Let me have 'em! (46128)
    TalkNoxxSlay        = 3,    ///< Tick.. tick.. tick.. BOOM! (46129)
    TalkNoxxDeath       = 4,    ///< I'm hit... (46127)
    TalkNoxxSpell01     = 1,    ///< Now you ain't so tall no more! (46130)
    TalkNoxxSpell02     = 2,    ///< You better step back. I'm calling in the bomb squad! (46131)
    TalkNoxxSpell03     = 5,    ///< Time is money friend, and it looks like you out of both! (46132)
    TalkMakoggIntro     = 60,   ///<  Hah! (45800)
    TalkMakoggDeath     = 61,   ///<  This.. is.. far, from over.. (45799)
    TalkMakoggSpell01   = 62,   ///<  You will burn! (45801)
    TalkKoramar07       = 29,   ///< That was an insignificant loss... let them exhaust themselves playing against our weak.. against those who're unworthy of serving in Black Hands army. That's all what Orc understand(46906)
    TalkZoggosh06       = 15   ///< Sir.. half of our army has been taken out. Don't you think we should.. (44053)
};

enum eCreatures
{
    CreatureLavaSweep           = 95353,
    CreatureOgreTrap            = 88758,
    CreatureBombsquad           = 80875
};

enum eActions
{
    ActionMakoggWinCheck = 1
};

uint32 g_Entries[3] = { eIronDocksCreatures::CreatureMakogg, eIronDocksCreatures::CreatureDuguru, eIronDocksCreatures::CreatureNox };

Position const g_Position[3] =
{
    { 6508.250f, -1127.709f, 4.958000f, 2.062029f },
    { 6512.830f, -1129.689f, 4.958000f, 2.022608f },
    { 6504.740f, -1131.180f, 4.958000f, 2.120224f }
};

static void GrimailEnforcersStart(InstanceScript* p_Instance, Creature* p_Me)
{
    if (p_Instance == nullptr)
        return;

    if (Creature* l_Makogg = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
        l_Makogg->SetInCombatWithZone();
    if (Creature* l_Duguru = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
        l_Duguru->SetInCombatWithZone();
    if (Creature* l_Noxx = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
        l_Noxx->SetInCombatWithZone();
}

static void GrimailEnforcersWipingCondition(InstanceScript* p_Instance, Creature* p_Me)
{
    if (p_Instance == nullptr)
        return;

    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);
    DespawnCreaturesInArea(eCreatures::CreatureOgreTrap, p_Me);
    DespawnCreaturesInArea(eCreatures::CreatureBombsquad, p_Me);
    if (Creature* l_Makogg = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
    {
        if (Creature* l_Duguru = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
        {
            if (Creature* l_Noxx = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
            {
                l_Makogg->Respawn();
                l_Duguru->Respawn();
                l_Noxx->Respawn();
                l_Makogg->GetMotionMaster()->MoveTargetedHome();
                l_Duguru->GetMotionMaster()->MoveTargetedHome();
                l_Noxx->GetMotionMaster()->MoveTargetedHome();
            }
        }
    }
}

static void GrimailEnforcersWiningCondition(InstanceScript* p_Instance, Creature* p_Me, uint64 p_KillerGuid)
{
    if (p_Instance == nullptr)
        return;

    if (p_KillerGuid == 0)
        return;

    if (Creature* l_Makogg = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
    {
        if (Creature* l_Duguru = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
        {
            if (Creature* l_Noxx = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
            {
                if (l_Duguru->isDead() && l_Noxx->isDead() && l_Makogg->isDead())
                {
                    if (Player* l_Player = Player::GetPlayer(*p_Me, p_KillerGuid))
                    {
                        p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);
                        DespawnCreaturesInArea(eCreatures::CreatureOgreTrap, p_Me);
                        DespawnCreaturesInArea(eCreatures::CreatureBombsquad, p_Me);
                        l_Makogg->SetLootRecipient(l_Player);
                        p_Instance->SetBossState(eIronDocksDatas::DataGrimrail, EncounterState::DONE);
                    }
                }
            }
        }
    }
}

class basicevent_aftergrimrail : public BasicEvent
{
    public:

        explicit basicevent_aftergrimrail(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (l_Koramar->IsAIEnabled && l_Zoggosh->IsAIEnabled)
                        {
                            switch (m_Modifier)
                            {
                            case 0:
                                l_Zoggosh->AI()->Talk(eTalks::TalkZoggosh06);
                                l_Koramar->m_Events.AddEvent(new basicevent_aftergrimrail(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                break;
                            case 1:
                                l_Koramar->AI()->Talk(eTalks::TalkKoramar07);
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }

            return true;
        }
    private:

        Unit* m_Obj;
        int m_Modifier;
};

/// Train - 83673
class iron_docks_grimrail_mob_train : public CreatureScript
{
public:

    iron_docks_grimrail_mob_train() : CreatureScript("iron_docks_grimrail_mob_train") { }

    struct iron_docks_grimrail_mob_trainAI : public ScriptedAI
    {
        iron_docks_grimrail_mob_trainAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eTrainMovements
        {
            MovementMiddleSpawnBosses = 1,
            MovementEndDespawnTrain
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            if (me->GetMap())
            {
                me->GetMap()->SetObjectVisibility(1000.0f);
            }
         
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetSpeed(UnitMoveType::MOVE_RUN, 10.0f, true);
            me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 10.0f, true);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->GetMotionMaster()->MoveTakeoff(eTrainMovements::MovementMiddleSpawnBosses, g_TrainMovePos[0].GetPositionX(), g_TrainMovePos[0].GetPositionY(), g_TrainMovePos[0].GetPositionZ());
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eTrainMovements::MovementMiddleSpawnBosses:
                {
                    if (m_Instance != nullptr)
                    {
                        if (Creature* l_Skulloc = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                        {
                            /// Stops the bombardement event.
                            l_Skulloc->m_Events.KillAllEvents(true);
                        }
                    }
        
                    for (uint8 l_I = 0; l_I < 3; l_I++)
                    {
                        if (Creature* l_Creature = me->SummonCreature(g_Entries[l_I], *me))
                        {
                            l_Creature->GetMotionMaster()->MoveJump(g_Position[l_I], 20.0f, 15.0f);
                            l_Creature->SetHomePosition(g_Position[l_I]);
                        }
                    }
                    me->GetMotionMaster()->MovePoint(eTrainMovements::MovementEndDespawnTrain, g_TrainMovePos[1].GetPositionX(), g_TrainMovePos[1].GetPositionY(), g_TrainMovePos[1].GetPositionZ());
                    break;
                }
                case eTrainMovements::MovementEndDespawnTrain:
                    me->DespawnOrUnsummon(2 * TimeConstants::IN_MILLISECONDS);
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_grimrail_mob_trainAI(p_Creature);
    }
};

/// Ahri'ok Duguru - 80816
class boss_grimrail_duguru : public CreatureScript
{
public:

    boss_grimrail_duguru() : CreatureScript("boss_grimrail_duguru") { }

    struct boss_grimrail_duguruAI : public BossAI
    {
        boss_grimrail_duguruAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataGrimrailDuguru)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eDuguruSpells
        {
            SpellBloodBolt          = 165122,
            SpellSanguineSphere     = 163689,
            SpellAbruptRestoration  = 163705,
            SpellTaintedBlood       = 163740
        };

        enum eDuguruEvents
        {
            EventBloodBolt = 1,
            EventSanguineSphere,
            EventTaintedBlood
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            if (m_Instance != nullptr)
                GrimailEnforcersStart(m_Instance, me);

            events.ScheduleEvent(eDuguruEvents::EventBloodBolt, 6 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eDuguruEvents::EventSanguineSphere, 4 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eDuguruEvents::EventTaintedBlood, 14 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* p_Killer) override
        {
            me->SetLootRecipient(NULL);
            if (m_Instance != nullptr)
                GrimailEnforcersWiningCondition(m_Instance, me, p_Killer->GetGUID());
         }

        void JustReachedHome() override
        {
            if (m_Instance != nullptr)
            GrimailEnforcersWipingCondition(m_Instance, me);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eDuguruEvents::EventBloodBolt:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eDuguruSpells::SpellBloodBolt);
                    events.ScheduleEvent(eDuguruEvents::EventBloodBolt, 6 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eDuguruEvents::EventSanguineSphere:
                {
                    std::list<Creature*> l_ListFellowGrimrail;
                    me->GetCreatureListWithEntryInGrid(l_ListFellowGrimrail, eIronDocksCreatures::CreatureMakogg, 100.0f);
                    me->GetCreatureListWithEntryInGrid(l_ListFellowGrimrail, eIronDocksCreatures::CreatureNox, 100.0f);
                    l_ListFellowGrimrail.push_back(me);
                    if (l_ListFellowGrimrail.empty())
                        return;

                    l_ListFellowGrimrail.sort(JadeCore::HealthPctOrderPred());
                    l_ListFellowGrimrail.resize(3);
                    if(Unit* l_Target = l_ListFellowGrimrail.front())
                        me->CastSpell(l_Target, eDuguruSpells::SpellSanguineSphere, true);

                    me->MonsterTextEmote("Ahir'ok Dugru begins to cast |cFFFF0404|Hspell:164275|h[Sanguine Sphere]!|h|r!", me->GetGUID(), true);
                    events.ScheduleEvent(eDuguruEvents::EventSanguineSphere, 16 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eDuguruEvents::EventTaintedBlood:
                    if (Unit * l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eDuguruSpells::SpellTaintedBlood);
                    events.ScheduleEvent(eDuguruEvents::EventTaintedBlood, 14 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_grimrail_duguruAI(p_Creature);
    }
};

/// Makogg Emberblade - 80805
class boss_grimrail_makogg : public CreatureScript
{
    public:

        boss_grimrail_makogg() : CreatureScript("boss_grimrail_makogg") { }

        struct boss_grimrail_makoggAI : public BossAI
        {
            boss_grimrail_makoggAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataGrimrailMakogg)
            {
                m_Instance = me->GetInstanceScript();
                m_First = false;
            }

            enum eMakoggSpells
            {
                SpellFlamingSlashDummy   = 163665,
                SpellFlamingSlashUnkAura = 163669,
                SpellFlamingSlashDamage  = 163668,
                SpellLavaSweepDamage     = 165152,
                SpellLavaWaveCosmetic    = 98873,
                SpellLavaWaveSwing       = 170899,
                SpellFeignDeath          = 103750,
                SpellCosmeticFeignDeath  = 166925
            };

            enum eMakoggEvents
            {
                /// Makogg
                EventFlamingSlash = 1,
                EventLavaSweep,        
                EventLavaSweepSpawn
            };

            InstanceScript* m_Instance;
            uint8 m_LavaSweepers;
            bool m_First;
            bool m_LavaSweeping;
            bool m_Dead;

            void Reset() override
            {  
                events.Reset();
                m_Dead = false;
                m_LavaSweeping = false;
                if (!m_First)
                {
                    m_First = true;
                    Talk(eTalks::TalkMakoggIntro);
                }
            }

            void JustSummoned(Creature* p_Summon) override
            {
                if (p_Summon)
                {
                    switch (p_Summon->GetEntry())
                    {
                        case eCreatures::CreatureLavaSweep:
                        {
                            p_Summon->AddAura(eMakoggSpells::SpellLavaWaveCosmetic, p_Summon);
                            p_Summon->AddAura(eMakoggSpells::SpellLavaSweepDamage, p_Summon);
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 100.0F, true))
                                p_Summon->GetMotionMaster()->MovePoint(0, l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ());
                            break;
                        }
                    }
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_Instance != nullptr)
                    GrimailEnforcersStart(m_Instance, me);
             
                events.ScheduleEvent(eMakoggEvents::EventLavaSweep, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eMakoggEvents::EventFlamingSlash, 13 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalks::TalkMakoggDeath);
                me->SetLootRecipient(NULL);
                if (m_Instance != nullptr)
                    GrimailEnforcersWiningCondition(m_Instance, me, p_Killer->GetGUID());
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_Instance == nullptr)
                    return;

                if (p_Damage && p_Damage > 0)
                {
                    if (p_Attacker && p_Attacker->IsInWorld())
                    {
                        if (!m_Dead)
                        {
                            if (me->GetHealthPct() <= 10)
                            {
                                m_Dead = true;
                                events.Reset();
                                me->RemoveAllAuras();
                                me->SetFullHealth();
                                me->CastSpell(me, eMakoggSpells::SpellFeignDeath);
                                me->CastSpell(me, eMakoggSpells::SpellCosmeticFeignDeath);
                                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH | eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                me->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                GrimailEnforcersWiningCondition(m_Instance, me, p_Attacker->GetGUID());
                            }
                        }
                    }
                }
            }

            void DoAction(const int32 p_Action) override
            {
                if (p_Action == eActions::ActionMakoggWinCheck)
                {
                    if (m_Instance != nullptr && m_Dead)
                        GrimailEnforcersStart(m_Instance, me);
                }
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                    GrimailEnforcersWipingCondition(m_Instance, me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eMakoggEvents::EventFlamingSlash:
                        {
                            Talk(eTalks::TalkMakoggSpell01);
                            me->CastSpell(me, eMakoggSpells::SpellFlamingSlashDummy);
                            events.ScheduleEvent(eMakoggEvents::EventFlamingSlash, 25 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eMakoggEvents::EventLavaSweep:
                        {
                            m_LavaSweepers = 0;
                            m_LavaSweeping = true;
                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);                   
                            events.ScheduleEvent(eMakoggEvents::EventLavaSweepSpawn, 2 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eMakoggEvents::EventLavaSweepSpawn:
                        {
                            m_LavaSweepers++;
                            me->CastSpell(me, eMakoggSpells::SpellLavaWaveSwing);
                            me->SummonCreature(eCreatures::CreatureLavaSweep, *me, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 8 * TimeConstants::IN_MILLISECONDS);                       
                            if (m_LavaSweepers >= 2)
                            {
                                m_LavaSweepers = 0;     
                                m_LavaSweeping = true;
                                me->SetReactState(ReactStates::REACT_AGGRESSIVE);                           
                                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                events.CancelEvent(eMakoggEvents::EventLavaSweepSpawn);
                                events.ScheduleEvent(eMakoggEvents::EventLavaSweep, 50 * TimeConstants::IN_MILLISECONDS);
                            }
                            events.ScheduleEvent(eMakoggEvents::EventLavaSweepSpawn, 3 * TimeConstants::IN_MILLISECONDS);
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
            return new boss_grimrail_makoggAI(p_Creature);
        }
};

/// Neesa Nox - 80808
class boss_grimrail_noxx : public CreatureScript
{
    public:
        boss_grimrail_noxx() : CreatureScript("boss_grimrail_noxx") { }

        struct boss_grimrail_noxxAI : public BossAI
        {
            boss_grimrail_noxxAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataGrimrailNoxx)
            {
                m_Instance = me->GetInstanceScript();
                m_First = false;
            }

            enum eNoxxSpells
            {
                /// Noxx
                SpellGutShot             = 163334,
                SpellOgreTrapOpenTeeth   = 177391,
                SpellOgreTrapClosedTeeth = 177396,
                SpellShreddingTendons    = 163276,
                SpellJumperCables        = 163376,
                SpellCharge              = 163635,
                SpellBigBoom             = 163379,
                SpellBombSquad           = 163362,
                SpellExplosionVisual     = 34602
            };

            enum eNoxxEvents
            {
                /// Noxx
                EventGutShot = 1,
                EventOgreTrap,
                EventJumperCables,
                EventBombSquad
            };

            InstanceScript* m_Instance;
            bool m_First;

            void Reset() override
            {
                events.Reset();
                if (!m_First)
                {
                    m_First = true;
                    Talk(eTalks::TalkNoxxIntro);
                }
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                if (m_Instance != nullptr)
                    GrimailEnforcersStart(m_Instance, me);

                events.ScheduleEvent(eNoxxEvents::EventOgreTrap, urand(18 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eNoxxEvents::EventJumperCables, 15 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eNoxxEvents::EventGutShot, 4 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eNoxxEvents::EventBombSquad, 30 * TimeConstants::IN_MILLISECONDS);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::TalkNoxxSlay);
            }


            void JustSummoned(Creature* p_Summon) override
            {
                if (p_Summon)
                {
                    switch (p_Summon->GetEntry())
                    {
                        case eCreatures::CreatureBombsquad:           
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                p_Summon->GetMotionMaster()->MoveJump(*l_Target, 40.0f, 20.0f);
                            p_Summon->CastSpell(p_Summon, eNoxxSpells::SpellBigBoom);
                            p_Summon->DespawnOrUnsummon(7 * TimeConstants::IN_MILLISECONDS);
                            p_Summon->SetReactState(ReactStates::REACT_PASSIVE);
                            break;
                    }
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                Talk(eTalks::TalkNoxxDeath);
                me->SetLootRecipient(NULL);
                if (m_Instance != nullptr)
                    GrimailEnforcersWiningCondition(m_Instance, me, p_Killer->GetGUID());
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                    GrimailEnforcersWipingCondition(m_Instance, me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;
   
                switch (events.ExecuteEvent())
                {
                    case eNoxxEvents::EventGutShot:
                        {
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eNoxxSpells::SpellGutShot);
                            events.ScheduleEvent(eNoxxEvents::EventGutShot, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eNoxxEvents::EventOgreTrap:
                        {                  
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            {
                                float l_Range = 5.0f;
                                float l_Angle = 0.0f;
                                float l_Step = (2 * M_PI) / 8;
                                for (uint8 l_I = 0; l_I < 8; ++l_I)
                                {
                                    float l_X = l_Target->GetPositionX() + (l_Range * cos(l_Angle));
                                    float l_Y = l_Target->GetPositionY() + (l_Range * sin(l_Angle));
                                    me->SummonCreature(eCreatures::CreatureOgreTrap, l_X, l_Y, l_Target->GetPositionZ(), l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                    l_Angle += l_Step;
                                }
                            }
                            Talk(eTalks::TalkNoxxSpell03);
                            events.ScheduleEvent(eNoxxEvents::EventOgreTrap, urand(40 * TimeConstants::IN_MILLISECONDS, 60 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eNoxxEvents::EventJumperCables:
                        {                
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                                me->CastSpell(l_Target, eNoxxSpells::SpellJumperCables);
                            Talk(eTalks::TalkNoxxSpell01);
                            events.ScheduleEvent(eNoxxEvents::EventJumperCables, 27 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eNoxxEvents::EventBombSquad:
                        {
                            me->CastSpell(me, eNoxxSpells::SpellBombSquad);
                            events.ScheduleEvent(eNoxxEvents::EventBombSquad, 50 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        default:
                            break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_grimrail_noxxAI(p_Creature);
        }
};

/// Ogre Trap - 88758
class iron_docks_grimrail_mob_ogre_trap : public CreatureScript
{
    public:
        iron_docks_grimrail_mob_ogre_trap() : CreatureScript("iron_docks_grimrail_mob_ogre_trap") { }

        struct iron_docks_grimrail_mob_ogre_trapAI : public Scripted_NoMovementAI
        {
            iron_docks_grimrail_mob_ogre_trapAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {           
                m_Instance = me->GetInstanceScript();
            }

            enum eOgreTrapSpells
            {
                SpellOgreTrapOpenTeeth   = 177391,
                SpellOgreTrapClosedTeeth = 177396,
                SpellShreddingTendons    = 163276
            };

            InstanceScript* m_Instance;
            bool m_Activated;
            uint32 m_DespawnTimer;

            void Reset() override
            {
                events.Reset();
                m_Activated = false;
                me->setFaction(HostileFaction);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->CastSpell(me, eOgreTrapSpells::SpellOgreTrapOpenTeeth);
                me->DespawnOrUnsummon(58 * TimeConstants::IN_MILLISECONDS);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);          
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_Activated)
                {
                    if (Player* l_Nearest = me->FindNearestPlayer(0.6f, true))
                    {
                        m_Activated = true;
                        me->RemoveAllAuras();
                        me->AddAura(eOgreTrapSpells::SpellOgreTrapClosedTeeth, me);
                        me->AddAura(eOgreTrapSpells::SpellShreddingTendons, l_Nearest);
                        me->DespawnOrUnsummon(6 * TimeConstants::IN_MILLISECONDS);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_grimrail_mob_ogre_trapAI(p_Creature);
        }
};

/// Lava Sweep - 95353
class iron_docks_grimrail_mob_lava_wave : public CreatureScript
{
public:
    iron_docks_grimrail_mob_lava_wave() : CreatureScript("iron_docks_grimrail_mob_lava_wave") { }

    struct iron_docks_grimrail_mob_lava_waveAI : public ScriptedAI
    {
        iron_docks_grimrail_mob_lava_waveAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eLavaWaveSpells
        {
            SpellLavaSweep = 165152
        };

        InstanceScript* m_Instance;
        uint32 m_EffectDiff;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);         
            me->SetReactState(ReactStates::REACT_PASSIVE);
            m_EffectDiff = 1 * TimeConstants::IN_MILLISECONDS;
            me->AddUnitState(UnitState::UNIT_STATE_CANNOT_AUTOATTACK);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (m_EffectDiff <= p_Diff)
            {
                std::list<Player*> l_ListPlayers;
                me->GetPlayerListInGrid(l_ListPlayers, 1.4f);
                if (!l_ListPlayers.empty())
                {
                    for (auto l_Itr : l_ListPlayers)
                    {
                        l_Itr->CastSpell(l_Itr, eLavaWaveSpells::SpellLavaSweep);
                    }
                }
                m_EffectDiff = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_EffectDiff -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_grimrail_mob_lava_waveAI(p_Creature);
    }
};

/// Sanguine Sphere - 163689
class iron_docks_grimrail_spell_sanguine_sphere : public SpellScriptLoader
{
    public:
        iron_docks_grimrail_spell_sanguine_sphere() : SpellScriptLoader("iron_docks_grimrail_spell_sanguine_sphere") { }

        enum eSanguineSphereSpells
        {
            SpellSanguineSphere    = 163689,
            SpellAbruptRestoration = 163705,
            SpellTaintedBlood      = 163740
        };

        class iron_docks_grimrail_spell_sanguine_sphere_AuraScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_grimrail_spell_sanguine_sphere_AuraScript);

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (l_Target->GetTypeId() == TypeID::TYPEID_PLAYER)
                        return;

                    std::list<Creature*> l_ListCreatures;
                    if (InstanceScript* l_Instance = l_Target->GetInstanceScript())
                    {
                        if (Creature* l_Makogg = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataGrimrailMakogg)))
                        {
                            if (Creature* l_Duguru = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataGrimrailDuguru)))
                            {
                                if (Creature* l_Noxx = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataGrimrailNoxx)))
                                {
                                    l_ListCreatures.push_back(l_Makogg);
                                    l_ListCreatures.push_back(l_Duguru);
                                    l_ListCreatures.push_back(l_Noxx);
                                    l_ListCreatures.push_back(l_Target->ToCreature());

                                    if (l_ListCreatures.empty())
                                        return;
                                  
                                    for (auto l_Itr : l_ListCreatures)
                                    {
                                        l_Itr->CastSpell(l_Itr, eSanguineSphereSpells::SpellAbruptRestoration);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(iron_docks_grimrail_spell_sanguine_sphere_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_grimrail_spell_sanguine_sphere_AuraScript();
        }
};


/// Big Boom - 163379  
class iron_docks_grimrail_spell_big_boom : public SpellScriptLoader
{
public:
    iron_docks_grimrail_spell_big_boom() : SpellScriptLoader("iron_docks_grimrail_spell_big_boom") { }

    class iron_docks_grimrail_spell_big_boom_SpellScript : public SpellScript
    {
        PrepareSpellScript(iron_docks_grimrail_spell_big_boom_SpellScript);

        enum eSpells
        {
            SpellBigBoom         = 163379,
            SpellExplosionVisual = 34602
        };

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            GetCaster()->CastSpell(GetCaster(), eSpells::SpellExplosionVisual);
        }

        void Register()
        {
            AfterCast += SpellCastFn(iron_docks_grimrail_spell_big_boom_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new iron_docks_grimrail_spell_big_boom_SpellScript();
    }
};

/// Starts the Grimrail event - 10314
class iron_docks_grimrail_at_event : public AreaTriggerScript
{
public:

    iron_docks_grimrail_at_event()
        : AreaTriggerScript("iron_docks_grimrail_at_event")
    {
    }

    void OnEnter(Player* p_Player, AreaTriggerEntry const* p_AreaTrigger) 
    {
        if (p_Player)
        {
            if (InstanceScript* l_Instance = p_Player->GetInstanceScript())
            {
                if (Creature* l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                {
                    if (l_Skulloc->IsAIEnabled)
                    {
                        l_Skulloc->GetAI()->DoAction(eIronDocksActions::ActionActivateGrimrailEvent);
                    }
                }
            }
        }
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_grimrail()
{
    /// Bosses
    new boss_grimrail_noxx(); /// 80808
    new boss_grimrail_makogg(); /// 80805
    new boss_grimrail_duguru(); /// 80816
    new iron_docks_grimrail_mob_train(); /// 83673
    new iron_docks_grimrail_mob_ogre_trap(); /// 88758
    new iron_docks_grimrail_mob_lava_wave(); /// 95353
    /// Spells
    new iron_docks_grimrail_spell_sanguine_sphere(); /// 163689
    new iron_docks_grimrail_spell_big_boom(); /// 163379 Visual
    //new iron_docks_grimrail_spell_lava_wave(); /// 98928 
    /// Areatrigger
    new iron_docks_grimrail_at_event(); /// 10314
}
#endif