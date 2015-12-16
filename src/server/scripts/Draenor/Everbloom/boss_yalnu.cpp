////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "the_everbloom.hpp"

enum eYalnuSpells
{
	SpellColossalBlow            = 169179,
	SpellEntanglmentDummy        = 169247,
	SpellEntaglementTrigger      = 169251,  
	SpellGenesis                 = 169613, 
    SpellGenesisProjectile       = 175975,
	SpellGerminateAborbladeDummy = 169265,
	SpellGeiminateAborbladeAura  = 169266,
    SpellFontOfLife              = 169120,
    SpellFontOfLifeSummon        = 169121,
    SpellDragonBreathVisual      = 101837,
    SpellChannelArcaneYalnu      = 171984,
    SpellEntanglmentAura         = 169240,
    SpellEntanglementVisualR     = 169192,
    SpellPyroBlast               = 170738,
    SpellPyroBlastFinish         = 170741
};

enum eYalnuEvents
{
    EventColossalBlow = 1,
    EventEntaglement,
    EventGenesis,
    EventGerminateAbroblade,
    EventFontOfLife,
    EventFeralLasherActivate,
    EventNoxiousBreath,
    EventLumberingSwipe,
    Event1,
    Event2,
    Event3,
    EventMove1,
    EventMove2,
    EventMove3 
};

enum eYalnuTalks
{
    /// Lady Baihu
    LadyBaihuIntro    = 50,  ///< The portal is lost! we must stop the beast before it can escape!
    LadyBaihuAggro    = 51,  ///< We've got its attention!
    LadyBaihuSpell01  = 52,  ///< The vines are overtaking everything!
    LadyBaihuSpell02  = 53   ///< Quickly. drow it towards the flames!
};

enum eYalnuCreatures
{
    CreatureFeralLasher       = 86684,
    CreatureSwiftSproutling   = 84401,
    CreatureViciousMandragora = 84399,
    CreatureGnarledAncient    = 84312,
    CreatureEntangling        = 84499
};

enum eYalnuActions
{
    ActionAchievementWeedWhacker = 1
};

Position g_PositionYalnuMoveToPortal = { 623.67f, 1730.01f, 143.357f };
Position g_PositionPortalToOverLook =  { 876.61f, -1221.62f, 196.700f, 0.076986f };
Position g_PositionYalnuMoveToMiddle = { 963.22f, -1229.370f, 181.250f };
Position g_PositionBaihuSpawn = { 955.670f, -1239.352f, 181.249f, 0.920571f };

Position g_PositionWallPos[7] =
{
    { 659.50f, 1677.284f, 133.746f, 1.980766f },
    { 651.77f, 1674.292f, 133.555f, 1.910080f },
    { 645.53f, 1671.924f, 132.954f, 4.796197f },
    { 639.40f, 1670.620f, 131.356f, 1.780489f },
    { 634.46f, 1670.076f, 130.416f, 1.658752f },
    { 629.35f, 1669.642f, 130.290f, 1.639117f },
    { 622.48f, 1670.313f, 131.578f, 1.466329f }
};

Position g_PositionMages[6] =
{
    { 948.28f, -1227.79f, 181.249f, 0.193210f },
    { 950.53f, -1219.82f, 181.255f, 5.653301f },
    { 959.88f, -1214.64f, 181.250f, 5.029704f },
    { 972.45f, -1215.30f, 181.250f, 4.046984f },
    { 978.21f, -1228.64f, 181.299f, 2.867515f },
    { 968.93f, -1240.47f, 181.250f, 1.952525f }
};

static void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
{
    std::list<Creature*> l_CreatureList;
    GetCreatureListWithEntryInGrid(l_CreatureList, p_Object, p_Entry, 300.0f);

    for (Creature* l_Iter : l_CreatureList)
        l_Iter->DespawnOrUnsummon();
}

#define INVISIBILE_DISPLAY 11686
#define FriendlyFaction 35
#define HostileFaction 16

/// Yalnu - 83846
class boss_yalnu : public CreatureScript
{
    public:
        boss_yalnu() : CreatureScript("boss_yalnu") { }

        struct boss_yalnu_AI : public BossAI
        {
            boss_yalnu_AI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomDatas::DataYalnu)
            {
                m_Instance = me->GetInstanceScript();
                m_Intro = false;
            }

            InstanceScript* m_Instance;
            bool m_Intro;
            bool m_Achievement;

            void Reset()
            {
                _Reset();
                events.Reset();

                me->SetInCombatWithZone();
                me->setFaction(7);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                m_Achievement = true;

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

                DespawnCreaturesInArea(eEverbloomCreatures::CreatureKirinTorBattleMage, me);
                DespawnCreaturesInArea(eEverbloomCreatures::CreatureLadyBayeu, me);
                DespawnCreaturesInArea(eYalnuCreatures::CreatureSwiftSproutling, me);
                DespawnCreaturesInArea(eYalnuCreatures::CreatureViciousMandragora, me);
                DespawnCreaturesInArea(eYalnuCreatures::CreatureGnarledAncient, me);
                DespawnCreaturesInArea(eYalnuCreatures::CreatureFeralLasher, me);
            }

            void SummonMages()
            {
                for (uint8 l_I = 0; l_I <= 5; l_I++)
                {
                    if (Creature* l_Mages = me->SummonCreature(eEverbloomCreatures::CreatureKirinTorBattleMage, g_PositionMages[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        l_Mages->CastSpell(l_Mages, eEverbloomSpells::SpellBlinkMageUponSummon);
                }

                if (Creature* l_Baihu = me->SummonCreature(eEverbloomCreatures::CreatureLadyBayeu, g_PositionBaihuSpawn, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Baihu->CastSpell(l_Baihu, eEverbloomSpells::SpellBlinkMageUponSummon);

                    if (l_Baihu->AI())
                        l_Baihu->AI()->Talk(eYalnuTalks::LadyBaihuAggro);
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eYalnuActions::ActionAchievementWeedWhacker:
                        if (m_Achievement)
                            m_Achievement = false;
                        break;
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                _EnterCombat();

                events.ScheduleEvent(eYalnuEvents::EventGenesis, 45 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eYalnuEvents::EventColossalBlow, 25 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eYalnuEvents::EventFontOfLife, 30 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eYalnuEvents::EventEntaglement, 15 * TimeConstants::IN_MILLISECONDS);

                /// Summon Mages and Lady Baihu
                SummonMages();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

                    DoZoneInCombat();
                }

                /// Move to the Middle
                me->GetMotionMaster()->MovePoint(0, g_PositionYalnuMoveToMiddle);

                /// Set new Home Position
                me->SetHomePosition(g_PositionYalnuMoveToMiddle.GetPositionX(), g_PositionYalnuMoveToMiddle.GetPositionY(), g_PositionYalnuMoveToMiddle.GetPositionZ(), 2.992742f);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                if (m_Achievement && m_Instance != nullptr && IsHeroic())
                    m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchivementWeedWhacker);
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
                    case eYalnuEvents::EventGenesis:
                    {
                        me->MonsterTextEmote("|TInterface\\Icons\\Ability_Druid_EmpoweredTouch:20|t Yalnu begins to channel |cFFFF0404|Hspell:169613|h[Genesis]|h|r.", LANG_UNIVERSAL, me->GetGUID());

                        me->CastSpell(me, eYalnuSpells::SpellGenesis);
                        events.ScheduleEvent(eYalnuEvents::EventGenesis, 45 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eYalnuEvents::EventColossalBlow:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f))
                        {
                            me->SetFacingToObject(l_Target);

                            me->CastSpell(l_Target, eYalnuSpells::SpellColossalBlow);
                        }

                        events.ScheduleEvent(eYalnuEvents::EventColossalBlow, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eYalnuEvents::EventFontOfLife:
                    {
                        me->CastSpell(me, eYalnuSpells::SpellFontOfLife);

                        events.ScheduleEvent(eYalnuEvents::EventFontOfLife, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eYalnuEvents::EventEntaglement:
                    {
                        /// If Heroic 50% to strike players
                        if (IsHeroic())
                        {
                            if (roll_chance_i(50))
                            {
                                /// Target players
                                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                {
                                    if (Creature* l_Wrap = DoSummon(eYalnuCreatures::CreatureEntangling, l_Target, 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                                    {
                                        if (l_Wrap->GetAI())
                                            l_Wrap->AI()->SetGUID(l_Target->GetGUID());
                                    }
                                }
                            }
                            else
                            {
                                std::list<Creature*> l_MagesAndBaihu;
                                me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreatures::CreatureLadyBayeu, 100.0f);
                                me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreatures::CreatureKirinTorBattleMage, 100.0f);

                                if (l_MagesAndBaihu.empty())
                                    return;

                                std::list<Creature*>::const_iterator l_It = l_MagesAndBaihu.begin();
                                std::advance(l_It, urand(0, l_MagesAndBaihu.size() - 1));

                                if (Creature* l_Wrap = DoSummon(eYalnuCreatures::CreatureEntangling, (*l_It), 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                                {
                                    if (l_Wrap->GetAI())
                                        l_Wrap->AI()->SetGUID((*l_It)->GetGUID());
                                }
                            }
                        }
                        /// If not heroic, 100% to strike npcs.
                        else
                        {
                            std::list<Creature*> l_MagesAndBaihu;
                            me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreatures::CreatureLadyBayeu, 100.0f);
                            me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreatures::CreatureKirinTorBattleMage, 100.0f);

                            if (l_MagesAndBaihu.empty())
                                return;

                            std::list<Creature*>::const_iterator l_It = l_MagesAndBaihu.begin();
                            std::advance(l_It, urand(0, l_MagesAndBaihu.size() - 1));

                            if (Creature* l_Wrap = DoSummon(eYalnuCreatures::CreatureEntangling, (*l_It), 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                            {
                                if (l_Wrap->GetAI())
                                    l_Wrap->AI()->SetGUID((*l_It)->GetGUID());
                            }
                        }

                        events.ScheduleEvent(eYalnuEvents::EventEntaglement, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_yalnu_AI(p_Creature);
        }
};

/// Rp Yalnu - 84336
class the_everbloom_yalnu_rp : public CreatureScript
{
    public:
        the_everbloom_yalnu_rp() : CreatureScript("the_everbloom_yalnu_rp") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->setFaction(FriendlyFaction);

                me->CastSpell(me, eYalnuSpells::SpellChannelArcaneYalnu);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS,  eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Undermage Kealson - 85496
class the_everbloom_undermage_kealson : public CreatureScript
{
    public:
        the_everbloom_undermage_kealson() : CreatureScript("the_everbloom_undermage_kealson") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
                m_MovementTrigger = true;

                for (uint8 l_I = 0; l_I < 7; l_I++)
                {
                    GameObject* l_GameObject = me->SummonGameObject(eEverbloomGameObjects::ObjectVineWall, g_PositionWallPos[l_I], 0, 0, 0, 0, 0);
                    if (l_GameObject != nullptr)
                        m_GuidsWalls.push_back(l_GameObject->GetGUID());
                }
            }

            InstanceScript* m_Instance;
            std::list<uint64> m_GuidsWalls;
            bool m_MovementTrigger;
            bool m_HasShotFire;

            void Reset() override
            {
                events.Reset();

                me->setFaction(FriendlyFaction);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS,  eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eEverbloomActions::ActionYalnuEvent:
                        events.ScheduleEvent(eYalnuEvents::EventMove1, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 8.0f) && m_MovementTrigger && m_HasShotFire)
                {
                    m_MovementTrigger = false;
                    events.ScheduleEvent(eYalnuEvents::Event2, 2 * TimeConstants::IN_MILLISECONDS);
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id)
            {
                switch (p_Id)
                {
                    case 1:
                        me->GetMotionMaster()->MovePoint(2, 637.804f, 1622.777f, 123.359f);
                        break;
                    case 2:
                        me->GetMotionMaster()->MovePoint(3, 633.912f, 1648.690f, 125.876f);
                        break;
                    case 3:
                        events.ScheduleEvent(eYalnuEvents::Event1, 4 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case eYalnuEvents::EventMove1:
                        me->GetMotionMaster()->MovePoint(1, 637.775f, 1585.777f, 115.899f);
                        break;
                    case eYalnuEvents::Event1:
                        if (Creature* l_Nearest = me->FindNearestCreature(eEverbloomCreatures::CreatureTriggerFire, 100.0f))
                        {
                            m_HasShotFire = true;

                            me->CastSpell(l_Nearest, eYalnuSpells::SpellDragonBreathVisual);
                            m_MovementTrigger = true;

                            for (uint64 l_Guid : m_GuidsWalls)
                            {
                                if (GameObject* l_Wall = GameObject::GetGameObject(*me, l_Guid))
                                    l_Wall->Delete();
                            }
                        }
                        break;
                    case eYalnuEvents::Event2:
                        if (m_Instance != nullptr)
                        {
                            if (Creature* l_RpYalnu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomCreatures::CreatureRpYalnu)))
                            {
                                l_RpYalnu->CastStop();
                                l_RpYalnu->GetMotionMaster()->MovePoint(0, g_PositionYalnuMoveToPortal);
                                l_RpYalnu->DespawnOrUnsummon(4 * TimeConstants::IN_MILLISECONDS);

                                me->SummonCreature(eEverbloomCreatures::CreatureTriggerTeleportToYalnu, g_PositionYalnuMoveToPortal.GetPositionX(), g_PositionYalnuMoveToPortal.GetPositionY(), g_PositionYalnuMoveToPortal.GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            }
                        }
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Trigger Teleport - 324251
class the_everbloom_fake_areatrigger_teleport : public CreatureScript
{
    public:
        the_everbloom_fake_areatrigger_teleport() : CreatureScript("the_everbloom_fake_areatrigger_teleport") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->setFaction(FriendlyFaction);
                me->SetDisplayId(INVISIBILE_DISPLAY);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 4.0f))
                {
                    p_Who->NearTeleportTo(g_PositionPortalToOverLook.GetPositionX(), g_PositionPortalToOverLook.GetPositionY(), g_PositionPortalToOverLook.GetPositionZ(), g_PositionPortalToOverLook.GetOrientation());
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// PreFight trigger - 324242
class the_everbloom_prefight_trigger : public CreatureScript
{
    public:
        the_everbloom_prefight_trigger() : CreatureScript("the_everbloom_prefight_trigger") { }

        struct Everbloom_triggers : public ScriptedAI
        {
            Everbloom_triggers(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
                m_MovementTrigger = false;
            }

            InstanceScript* m_Instance;
            bool m_MovementTrigger;

            void Reset() override
            {
                me->setFaction(FriendlyFaction);
                me->SetDisplayId(INVISIBILE_DISPLAY);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 15.0f) && !m_MovementTrigger)
                {
                    events.ScheduleEvent(eYalnuEvents::Event1, 500);
                    m_MovementTrigger = true;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                events.Update(p_Diff);

                if (m_Instance != nullptr)
                {
                    switch (events.ExecuteEvent())
                    {
                        case eYalnuEvents::Event1:
                            if (Creature* Yalnu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossYalnu)))
                            {
                                Yalnu->GetMotionMaster()->MovePoint(0, g_PositionYalnuMoveToMiddle);
                                Yalnu->SetHomePosition(g_PositionYalnuMoveToMiddle.GetPositionX(), g_PositionYalnuMoveToMiddle.GetPositionY(), g_PositionYalnuMoveToMiddle.GetPositionZ(), 2.992742f);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new Everbloom_triggers(p_Creature);
        }
};

/// Lady Baihu - 84358
class the_everbloom_lady_baihu : public CreatureScript
{
    public:
        the_everbloom_lady_baihu() : CreatureScript("the_everbloom_lady_baihu") { }

        struct the_everbloom_creaturesAI : public Scripted_NoMovementAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
                m_Intro = false;
            }

            InstanceScript* m_Instance;
            bool m_Intro;

            void Reset() override
            {
                events.Reset();

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

                DoZoneInCombat();

                me->MonsterYell("The portal is lost! we must stop the beast before it can escape!", Language::LANG_UNIVERSAL, me->GetGUID());
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro)
                {
                    m_Intro = true;
                    Talk(eYalnuTalks::LadyBaihuIntro);
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (Creature* Yalnu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossYalnu)))
                {
                    if (Yalnu->GetAI())
                        Yalnu->GetAI()->DoAction(eYalnuActions::ActionAchievementWeedWhacker);
                }
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                events.ScheduleEvent(eEverbloomEvents::EventFireBall,  10 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEverbloomEvents::EventFlamestike, 35 * TimeConstants::IN_MILLISECONDS);

                Talk(eYalnuTalks::LadyBaihuAggro);

                me->MonsterYell("We've got its attention!", Language::LANG_UNIVERSAL, me->GetGUID());
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Yalnu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossYalnu)))
                    {
                        switch (events.ExecuteEvent())
                        {
                            case eEverbloomEvents::EventFireBall:
                            {
                                Talk(eYalnuTalks::LadyBaihuSpell02);
                                me->CastSpell(l_Yalnu, eEverbloomSpells::SpellFireBall);
                                events.ScheduleEvent(eEverbloomEvents::EventFireBall, 10 * TimeConstants::IN_MILLISECONDS);
                                break;
                            }
                            case eEverbloomEvents::EventFlamestike:
                            {
                                Talk(eYalnuTalks::LadyBaihuSpell01);
                                me->CastSpell(l_Yalnu, eEverbloomSpells::SpellFlamestikeAreatriger);
                                events.ScheduleEvent(eEverbloomEvents::EventFlamestike, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
                                break;
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Kirin Tor Mage - 84329
class the_everbloom_kirin_tor_mage : public CreatureScript
{
    public:
        the_everbloom_kirin_tor_mage() : CreatureScript("the_everbloom_kirin_tor_mage") { }

        struct the_everbloom_creaturesAI : public Scripted_NoMovementAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                events.Reset();

                me->setFaction(FriendlyFaction);

                me->SetInCombatWithZone();
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                switch (urand(0, 2))
                {
                    case 0:
                        events.ScheduleEvent(eEverbloomEvents::EventFrostbolt, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 1:
                        events.ScheduleEvent(eEverbloomEvents::EventArcaneBlast, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 2:
                        events.ScheduleEvent(eEverbloomEvents::EventFireBall, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Yalnu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossYalnu)))
                    {
                        if (l_Yalnu->GetAI())
                            l_Yalnu->GetAI()->DoAction(eYalnuActions::ActionAchievementWeedWhacker);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f))
                {
                    switch (events.ExecuteEvent())
                    {
                        case eEverbloomEvents::EventArcaneBlast:
                        {
                            me->CastSpell(l_Target, eEverbloomSpells::SpellArcaneBlast4Th);
                            events.ScheduleEvent(eEverbloomEvents::EventFireBall, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                        case eEverbloomEvents::EventArcaneOrb:
                        {
                            me->CastSpell(l_Target, eEverbloomSpells::SpellArcaneOrbDummy);
                            events.ScheduleEvent(eEverbloomEvents::EventFireBall, 25 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        case eEverbloomEvents::EventFireBall:
                        {
                            me->CastSpell(l_Target, eEverbloomSpells::SpellFireBall);
                            events.ScheduleEvent(eEverbloomEvents::EventFireBall, 15 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        case eEverbloomEvents::EventFlamestike:
                        {
                            me->CastSpell(l_Target, eEverbloomSpells::SpellFlamestikeAreatriger);
                            events.ScheduleEvent(eEverbloomEvents::EventFlamestike, 40 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        case eEverbloomEvents::EventFrostbolt:
                        {
                            me->CastSpell(l_Target, eEverbloomSpells::SpellFrostbolt4Th);
                            events.ScheduleEvent(eEverbloomEvents::EventFrostbolt, 15 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                        case eEverbloomEvents::EventIceComet:
                        {
                            me->CastSpell(l_Target, eEverbloomSpells::SpellIceComet);
                            events.ScheduleEvent(eEverbloomEvents::EventIceComet, 40 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Swift Sproutling - 84401
class the_everbloom_swift_sproutling : public CreatureScript
{
    public:
        the_everbloom_swift_sproutling() : CreatureScript("the_everbloom_swift_sproutling") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEverbloomEvents::EventTendonRip, urand(7 * TimeConstants::IN_MILLISECONDS, 11 * TimeConstants::IN_MILLISECONDS));
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
                    case eEverbloomEvents::EventTendonRip:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eEverbloomSpells::SpellTendonRip);

                        events.ScheduleEvent(eEverbloomEvents::EventTendonRip, urand(7 * TimeConstants::IN_MILLISECONDS, 11 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Vicious Mandragora - 84399
class the_everbloom_vicious_mandragora : public CreatureScript
{
    public:
        the_everbloom_vicious_mandragora() : CreatureScript("the_everbloom_vicious_mandragora") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eYalnuEvents::EventNoxiousBreath, 12 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eYalnuEvents::EventNoxiousBreath:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eEverbloomSpells::SpellNoxiousBreath);

                        events.ScheduleEvent(eYalnuEvents::EventNoxiousBreath, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Gnarled Ancient - 84312
class the_everbloom_gnarled_ancient : public CreatureScript
{
    public:
        the_everbloom_gnarled_ancient() : CreatureScript("the_everbloom_gnarled_ancient") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eYalnuEvents::EventLumberingSwipe, urand(11 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
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
                    case eYalnuEvents::EventLumberingSwipe:
                        DoCastAOE(eEverbloomSpells::SpellLumberingSwipe);
                        events.ScheduleEvent(eYalnuEvents::EventLumberingSwipe, urand(12 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Entangling - 84499
class the_everbloom_entangling : public CreatureScript
{
    public:
        the_everbloom_entangling() : CreatureScript("the_everbloom_entangling") { }

        struct the_everbloom_creaturesAI : public NullCreatureAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : NullCreatureAI(p_Creature), m_VictimGUID(0) { }

            uint64 m_VictimGUID;

            void Reset() override
            {
                me->CastSpell(me, eYalnuSpells::SpellEntanglementVisualR);
                me->SetDisplayId(INVISIBILE_DISPLAY);
            }

            void SetGUID(uint64 p_Guid, int32 /*p_Param*/) override
            {
                m_VictimGUID = p_Guid;

                if (m_VictimGUID)
                {
                    if (Unit* l_Victim = Unit::GetUnit(*me, m_VictimGUID))
                        l_Victim->AddAura(eYalnuSpells::SpellEntanglmentAura, l_Victim);
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_VictimGUID)
                {
                    if (Unit* l_Victim = Unit::GetUnit(*me, m_VictimGUID))
                        l_Victim->RemoveAura(eYalnuSpells::SpellEntanglmentAura);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

// Feral Lasher - 86684
class the_everbloom_feral_lasher : public CreatureScript
{
    public:
        the_everbloom_feral_lasher() : CreatureScript("the_everbloom_feral_lasher") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_SleepMode;

            void Reset() override
            {
                m_SleepMode = true;

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddAura(eEverbloomSpells::SpellSubmerge, me);
                me->setFaction(FriendlyFaction);

                events.ScheduleEvent(eYalnuEvents::EventFeralLasherActivate, 14 * TimeConstants::IN_MILLISECONDS);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eEverbloomEvents::EventLasherVenom, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
            }

            void Trample() ///< Hardcoded
            {
                if (me->FindNearestPlayer(1.8f, true) && m_SleepMode)
                {
                    m_SleepMode = false;
                    me->Kill(me);
                    me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                    me->CastSpell(me, 154535);
                }
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case eEverbloomEvents::EventLasherVenom:
                    {
                        if (!UpdateVictim())
                            return;

                        if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                            return;

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eEverbloomSpells::SpellLasherVenom);

                        events.ScheduleEvent(eEverbloomEvents::EventLasherVenom, urand(3 * TimeConstants::IN_MILLISECONDS, 6 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eYalnuEvents::EventFeralLasherActivate:
                    {
                        m_SleepMode = false;

                        me->setFaction(HostileFaction);
                        me->RemoveAllAuras();
                        me->RemoveAura(eEverbloomSpells::SpellSubmerge);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        break;
                    }
                    default:
                        break;
                }

                Trample();

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
        return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Genesis Tick - 169613
class the_everbloom_genesis_tick : public SpellScriptLoader
{
    public:
        the_everbloom_genesis_tick() : SpellScriptLoader("the_everbloom_genesis_tick") { }

        class everbloom_spells : public SpellScript
        {
            PrepareSpellScript(everbloom_spells);

            void GenesisTick()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    for (uint8 l_I = 0; l_I <= 13; l_I++)
                        l_Caster->CastSpell(l_Caster, eYalnuSpells::SpellGenesisProjectile, true);
                }
            }

            void Register() override
            {
                BeforeCast += SpellCastFn(everbloom_spells::GenesisTick);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new everbloom_spells();
        }
};

/// Font of Life - 169121
class the_everbloom_font_of_life : public SpellScriptLoader
{
    public:
        the_everbloom_font_of_life() : SpellScriptLoader("the_everbloom_font_of_life") { }

        class everbloom_spells : public SpellScript
        {
            PrepareSpellScript(everbloom_spells);

            void HandleSummon(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (SpellInfo const* l_Spell = sSpellMgr->GetSpellInfo(eYalnuSpells::SpellFontOfLifeSummon))
                    {
                        uint32 l_Value = 0;

                        switch (urand(0, 2))
                        {
                            case 0:
                                l_Value = eYalnuCreatures::CreatureViciousMandragora;
                                break;
                            case 1:
                                l_Value = eYalnuCreatures::CreatureSwiftSproutling;
                                break;
                            case 2:
                                l_Value = eYalnuCreatures::CreatureGnarledAncient;
                                break;
                            default:
                                break;
                        }

                        Position l_Pos;
                        GetExplTargetDest()->GetPosition(&l_Pos);

                        l_Caster->SummonCreature(l_Value, l_Pos, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, l_Spell->GetDuration());
                    }
                }
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(everbloom_spells::HandleSummon, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new everbloom_spells();
        }
};

/// Colossal Blow - 169179 
class the_everbloom_yalnu_cone_strike : public SpellScriptLoader
{
    public:
        the_everbloom_yalnu_cone_strike() : SpellScriptLoader("the_everbloom_yalnu_cone_strike") { }

        class spell_warr_storm_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_storm_bolt_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (!l_Target->isInFront(l_Caster))
                            SetHitDamage(0);
                    }
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_warr_storm_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_warr_storm_bolt_SpellScript();
        }
};

void AddSC_boss_yalnu()
{
    /// Boss
	new boss_yalnu();

    /// Npcs
    new the_everbloom_yalnu_rp();
    new the_everbloom_undermage_kealson();
    new the_everbloom_fake_areatrigger_teleport();
    new the_everbloom_prefight_trigger();
    new the_everbloom_lady_baihu();
    new the_everbloom_kirin_tor_mage();
    new the_everbloom_swift_sproutling();
    new the_everbloom_vicious_mandragora();
    new the_everbloom_gnarled_ancient();
    new the_everbloom_entangling();
    new the_everbloom_feral_lasher();

    /// Spells
    new the_everbloom_font_of_life();
    new the_everbloom_genesis_tick();
    new the_everbloom_yalnu_cone_strike();
}