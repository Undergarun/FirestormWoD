////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

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
    SpellPyroBlast               = 170738,
    SpellPyroBlastFinish         = 170741
};

enum eYalnuEvents
{
	EventColossalBlow        = 1,
	EventEntaglement,
	EventGenesis,
	EventGerminateAbroblade,
    EventFontOfLife,
    EventFeralLasherActivate,
    EventNoxiousBreath,
    EventLumberingSwipe,
};

enum eYalnuMovementInformed
{
    MovementInformedKealson01 = 1,
    MovementYalnuPoint1
};

enum eYalnuTalks
{
    ///< Lady Baihu
    LadyBaihuIntro    = 50 , ///< The portal is lost! we must stop the beast before it can escape!
    LadyBaihuAggro    = 51,  ///< We've got its attention!
    LadyBaihuSpell01  = 52,  ///< The vines are overtaking everything!
    LadyBaihuSpell02  = 53,  ///< Quickly. drow it towards the flames!
    LadyBaihuSpell03  = 54   ///< Look out!
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

Position const g_PositionYalnuMoveToPortal     = {623.67f, 1730.01f, 143.357f};

Position const g_PositionPortalToOverLook      =  {876.61f, -1221.62f, 196.700f, 0.076986f};

Position const g_PositionYalnuMoveToMiddle     = {963.22f, -1229.370f, 181.250f};

Position const g_PositionBaihuSpawn            = {955.670f, -1239.352f, 181.249f, 0.920571f};

Position const g_PositionLadyBayeu             = { 693.75f, -1227.05f, 181.249f, 6.011444f };

Position const g_PositionMoveToVineWallKealson = { 635.642f, 1640.213f, 124.989f };

Position const g_PositionWallPos[7] =
{
    {659.50f, 1677.284f, 133.746f, 1.980766f},
    {651.77f, 1674.292f, 133.555f, 1.910080f},
    {645.53f, 1671.924f, 132.954f, 4.796197f},
    {639.40f, 1670.620f, 131.356f, 1.780489f},
    {634.46f, 1670.076f, 130.416f, 1.658752f},
    {629.35f, 1669.642f, 130.290f, 1.639117f},
    {622.48f, 1670.313f, 131.578f, 1.466329f}
};

Position const g_PositionMages[6] =
{
    {948.28f, -1227.79f, 181.249f, 0.193210f},
    {950.53f, -1219.82f, 181.255f, 5.653301f},
    {959.88f, -1214.64f, 181.250f, 5.029704f},
    {972.45f, -1215.30f, 181.250f, 4.046984f},
    {978.21f, -1228.64f, 181.299f, 2.867515f},
    {968.93f, -1240.47f, 181.250f, 1.952525f}
};

/// Yalnu - 83846
class boss_yalnu : public CreatureScript
{
public:

    boss_yalnu() : CreatureScript("boss_yalnu") { }

    struct boss_yalnuAI : public BossAI
    {
        boss_yalnuAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataYalnu)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
            m_First = false;
            m_EncounterBegin = false;
        }

        enum eYalnuEvents
        {
            EventColossalBlow = 1,
            EventEntaglement,
            EventGenesis,
            EventFontOfLife
        };

        enum eYalnuSpells
        {
            SpellColossalBlow            = 169179,
            SpellEntanglmentDummy        = 169247,
            SpellEntaglementTrigger      = 169251,
            SpellGenesis                 = 169613,
            SpellGenesisProjectile       = 175975,
            SpellFontOfLife              = 169120,
            SpellFontOfLifeSummon        = 169121,
            SpellChannelArcaneYalnu      = 171984,
            SpellBlinkMageUponSummon     = 142193
        };
 
        bool m_Intro;
        bool m_First;
        bool m_EncounterBegin;
        bool m_Achievement;
        uint64 m_BaihuGuid;
        InstanceScript* m_Instance;

        void Reset() override
        {
            _Reset();
            events.Reset();
            me->setFaction(AttackableYetNotHostileFaction);
            m_Achievement = true;
            m_BaihuGuid = 0;
            uint32 l_Entries[6] =
            { eEverbloomCreature::CreatureKirinTorBattleMage, eEverbloomCreature::CreatureLadyBayeu, eYalnuCreatures::CreatureSwiftSproutling,
            eYalnuCreatures::CreatureViciousMandragora, eYalnuCreatures::CreatureGnarledAncient, eYalnuCreatures::CreatureFeralLasher };
            for (uint8 l_I = 0; l_I < 7; l_I++)
                DespawnCreaturesInArea(l_Entries[l_I], me);        
            if (!m_First)
            {
                m_First = true;
                me->CastSpell(me, eYalnuSpells::SpellChannelArcaneYalnu);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }
            else
            {
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }     
        }
    
        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 14.0f) && !m_EncounterBegin)
            {
				if (m_Instance->GetBossState(eEverbloomData::DataArchmageSol) == EncounterState::DONE)
				{
					m_EncounterBegin = true;
					me->RemoveAllAuras();
					me->GetMotionMaster()->MovePoint(eYalnuMovementInformed::MovementYalnuPoint1, g_PositionYalnuMoveToPortal.GetPositionX(), g_PositionYalnuMoveToPortal.GetPositionY(), g_PositionYalnuMoveToPortal.GetPositionZ());
				}
				else
					me->MonsterSay("debug: instance is not done sol kapaaaaraa", LANG_UNIVERSAL, me->GetGUID());
            }
        }

        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eYalnuMovementInformed::MovementYalnuPoint1:            
                    me->NearTeleportTo(g_PositionYalnuMoveToMiddle);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                   break;
                default:
                    break;
            }
        }
 
        void SummonMages()
        {
            for (uint8 l_I = 0; l_I <= 5; l_I++)
            {
               if (Creature* l_Mages = me->SummonCreature(eEverbloomCreature::CreatureKirinTorBattleMage, g_PositionMages[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                   l_Mages->CastSpell(l_Mages, eYalnuSpells::SpellBlinkMageUponSummon);
            }
            if (Creature* l_Baihu = me->SummonCreature(eEverbloomCreature::CreatureLadyBayeu, g_PositionBaihuSpawn, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            {
                l_Baihu->CastSpell(l_Baihu, eYalnuSpells::SpellBlinkMageUponSummon);
                l_Baihu->MonsterYell("The portal is lost! we must stop the beast before it can escape!", Language::LANG_UNIVERSAL, l_Baihu->GetGUID());

                m_BaihuGuid = l_Baihu->GetGUID();
                if (l_Baihu->IsAIEnabled)
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
            SummonMages();   /// Summon Mages and Lady Baihu
            events.ScheduleEvent(eYalnuEvents::EventGenesis, 45 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eYalnuEvents::EventColossalBlow, 25 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eYalnuEvents::EventFontOfLife, 30 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eYalnuEvents::EventEntaglement, 15 * TimeConstants::IN_MILLISECONDS);
            if (m_Instance != nullptr)
            {
                if (m_BaihuGuid)
                {
                    if (Creature * l_Baihu = Creature::GetCreature(*me, m_BaihuGuid))
                        l_Baihu->MonsterYell("We've got its attention!", Language::LANG_UNIVERSAL, l_Baihu->GetGUID());
                }
                m_Instance->SetBossState(eEverbloomData::DataYalnu, EncounterState::IN_PROGRESS);        
            }
            // Move to the Middle
            me->GetMotionMaster()->MovePoint(0, g_PositionYalnuMoveToMiddle);
            // Set new Home Position const
            me->SetHomePosition(g_PositionYalnuMoveToMiddle.GetPositionX(), g_PositionYalnuMoveToMiddle.GetPositionY(), g_PositionYalnuMoveToMiddle.GetPositionZ(), 2.992742f);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            if (m_Achievement)
            {
                if (m_Instance != nullptr)
                {
                    if (me->GetMap() && me->GetMap()->IsHeroic())
                    m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchivementWeedWhacker);
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

            switch (events.ExecuteEvent())
            {
                case eYalnuEvents::EventGenesis:
                {
                    me->MonsterTextEmote("|TInterface\\Icons\\Ability_Druid_EmpoweredTouch:20|t Yalnu begins to channel |cFFFF0404|Hspell:169613|h[Genesis]|h|r.", me->GetGUID(), true);
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
                        events.ScheduleEvent(eYalnuEvents::EventColossalBlow, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
                case eYalnuEvents::EventFontOfLife:
                {
                    me->CastSpell(me, eYalnuSpells::SpellFontOfLife);
                    events.ScheduleEvent(eYalnuEvents::EventFontOfLife, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eYalnuEvents::EventEntaglement:
                {
                    // If Heroic 50% to strike players
                    if (me->GetMap() && me->GetMap()->IsHeroic())
                    {
                        if (roll_chance_i(50))
                        {
                            ///< Target players
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                if (Creature* l_Wrap = DoSummon(eYalnuCreatures::CreatureEntangling, l_Target, 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                                {
                                    if (l_Wrap->IsAIEnabled)
                                        l_Wrap->AI()->SetGUID(l_Target->GetGUID());
                                }
                            }
                        }
                        else
                        {
                            std::list<Creature*> l_MagesAndBaihu;
                            me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreature::CreatureLadyBayeu, 100.0f);
                            me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreature::CreatureKirinTorBattleMage, 100.0f);
                            if (l_MagesAndBaihu.empty())
                                return;

                            std::list<Creature*>::const_iterator l_It = l_MagesAndBaihu.begin();
                            std::advance(l_It, urand(0, l_MagesAndBaihu.size() - 1));
                            if (Creature* l_Wrap = DoSummon(eYalnuCreatures::CreatureEntangling, (*l_It), 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                            {
                                if (l_Wrap->IsAIEnabled)
                                    l_Wrap->AI()->SetGUID((*l_It)->GetGUID());
                            }
                        }
                    }
                    else  /// If not heroic, 100% to strike npcs.
                    {
                        std::list<Creature*> l_MagesAndBaihu;
                        me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreature::CreatureLadyBayeu, 100.0f);
                        me->GetCreatureListWithEntryInGrid(l_MagesAndBaihu, eEverbloomCreature::CreatureKirinTorBattleMage, 100.0f);
                        if (l_MagesAndBaihu.empty())
                            return;
                        std::list<Creature*>::const_iterator l_It = l_MagesAndBaihu.begin();
                        std::advance(l_It, urand(0, l_MagesAndBaihu.size() - 1));
                        if (Creature* l_Wrap = DoSummon(eYalnuCreatures::CreatureEntangling, (*l_It), 0, TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                        {
                            if (l_Wrap->IsAIEnabled)
                                l_Wrap->AI()->SetGUID((*l_It)->GetGUID());
                        }
                    }
                    events.ScheduleEvent(eYalnuEvents::EventEntaglement, 15 * TimeConstants::IN_MILLISECONDS);
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
        return new boss_yalnuAI(p_Creature);
    }
};

/// Undermage Kealson - 85496
class the_everbloom_yalnu_mob_undermage_kealson : public CreatureScript
{
public:

    the_everbloom_yalnu_mob_undermage_kealson() : CreatureScript("the_everbloom_yalnu_mob_undermage_kealson") { }

    struct the_everbloom_yalnu_mob_undermage_kealsonAI : public ScriptedAI
    {
        the_everbloom_yalnu_mob_undermage_kealsonAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_MovementTrigger = true;
            m_First = false;
        }

        enum eKealsonEvents
        {
            EventKealsonStartMoving = 1,
            EventKealsonIncinierateWalls,
            EventKealsonWallsGoesDown
        };

        enum eKealsonSpells
        {
            SpellDragonBreathVisual = 101837,
            SpellBubble             = 170665
        };

        InstanceScript* m_Instance;
        bool m_MovementTrigger;
        bool m_HasShotFire;
        bool m_First;

        void Reset() override
        {
            events.Reset();
            me->setFaction(FriendlyFaction);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eEverbloomActions::ActionYalnuEvent:
                    me->CastSpell(me, eKealsonSpells::SpellBubble);
                    me->GetMotionMaster()->MovePoint(eYalnuMovementInformed::MovementInformedKealson01, g_PositionMoveToVineWallKealson);
                    break;
                default:
                    break;
            }
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id)
        {
            switch (p_Id)
            {
                case eYalnuMovementInformed::MovementInformedKealson01:
                {
                    me->RemoveAura(eKealsonSpells::SpellBubble);
                    me->CastSpell(me, eKealsonSpells::SpellDragonBreathVisual);
                    me->MonsterSay("If that beast crosses through, the unchecked growth will choke the whole of Azeroth, Hurry!", LANG_UNIVERSAL, me->GetGUID());
                    std::list<GameObject*> m_GuidsWalls;
                    me->GetGameObjectListWithEntryInGrid(m_GuidsWalls, eEverbloomGameObjects::ObjectVineWall, 250.0f);
                    if (!m_GuidsWalls.empty())
                    {
                        for (GameObject* l_Itr : m_GuidsWalls)
                            l_Itr->Delete();
                    }
                    break;
                }
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_yalnu_mob_undermage_kealsonAI(p_Creature);
    }
};

/// Kirin Tor Mage - 84329
class the_everbloom_yalnu_mob_kirin_tor_mage : public CreatureScript
{
public:

    the_everbloom_yalnu_mob_kirin_tor_mage() : CreatureScript("the_everbloom_yalnu_mob_kirin_tor_mage") { }

    struct the_everbloom_yalnu_mob_kirin_tor_mageAI : public Scripted_NoMovementAI
    {
        the_everbloom_yalnu_mob_kirin_tor_mageAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }


        enum eKirinTorEvents
        {
            EventFrostbolt = 1,
            EventArcaneBlast,
            EventFireBall,
            EventIceComet,
            EventArcaneOrb,
            EventFlamestike,
        };

        enum eKirinTorSpells
        {
            SpellFrostbolt             = 169840,
            SpellFrostbolt4Th          = 170028,
            SpellFlamestrikeAura       = 169100,
            SpellFlamestikeAreatriger  = 169094,
            SpellIceComet              = 170032,
            SpellArcaneBlast           = 169825,
            SpellArcaneBlast4Th        = 170035,
            SpellArcaneOrbDummy        = 167018,
            SpellArcaneOrbAreatriger   = 170040,
            SpellArcaneOrbDamage       = 170077,
            SpellFireball              = 168666,
            SpellDragonsBreath         = 169843,
            SpellFireBall              = 169839
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetInCombatWithZone();
            me->setFaction(FriendlyFaction);  
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            switch (urand(0, 2))
            {
                case 0:
                    events.ScheduleEvent(eKirinTorEvents::EventFrostbolt, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                    break;
                case 1:
                    events.ScheduleEvent(eKirinTorEvents::EventArcaneBlast, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                    break;
                case 2:
                    events.ScheduleEvent(eKirinTorEvents::EventFireBall, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                    break;   
                default:
                    break;
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Yalnu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataYalnu)))
                {
                    if (l_Yalnu->IsAIEnabled)
                        l_Yalnu->GetAI()->DoAction(eYalnuActions::ActionAchievementWeedWhacker);
                }
            }

            me->DespawnOrUnsummon();
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
            {
                switch (events.ExecuteEvent())
                {
                case eKirinTorEvents::EventArcaneBlast:
                    {
                        me->CastSpell(l_Target, eKirinTorSpells::SpellArcaneBlast);
                        events.ScheduleEvent(eKirinTorEvents::EventFireBall, urand(15 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                case eKirinTorEvents::EventArcaneOrb:
                    {
                        me->CastSpell(l_Target, eKirinTorSpells::SpellArcaneOrbDummy);
                        events.ScheduleEvent(eKirinTorEvents::EventFireBall, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                case eKirinTorEvents::EventFireBall:
                    {
                        me->CastSpell(l_Target, eKirinTorSpells::SpellFireBall);
                        events.ScheduleEvent(eKirinTorEvents::EventFireBall, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                case eKirinTorEvents::EventFlamestike:
                    {
                        me->CastSpell(l_Target, eKirinTorSpells::SpellFlamestikeAreatriger);
                        events.ScheduleEvent(eKirinTorEvents::EventFlamestike, 40 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                case eKirinTorEvents::EventFrostbolt:
                    {
                        me->CastSpell(l_Target, eKirinTorSpells::SpellFrostbolt4Th);
                        events.ScheduleEvent(eKirinTorEvents::EventFrostbolt, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                case eKirinTorEvents::EventIceComet:
                    {
                        me->CastSpell(l_Target, eKirinTorSpells::SpellIceComet);
                        events.ScheduleEvent(eKirinTorEvents::EventIceComet, 40 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_yalnu_mob_kirin_tor_mageAI(p_Creature);
    }
};

/// Swift Sproutling - 84401
class the_everbloom_yalnu_mob_swift_sproutling : public CreatureScript
{
public:

    the_everbloom_yalnu_mob_swift_sproutling() : CreatureScript("the_everbloom_yalnu_mob_swift_sproutling") { }

    struct the_everbloom_yalnu_mob_swift_sproutlingAI : public ScriptedAI
	{
        the_everbloom_yalnu_mob_swift_sproutlingAI(Creature* p_Creature) : ScriptedAI(p_Creature)
		{
            m_Instance = me->GetInstanceScript();
		}

        enum eSwiftSproutlingSpells
        {
            SpellTendonRip = 169876
        };

        enum eSwiftSproutlingEvents
        {
            EventTendonRip = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            DoZoneInCombat();
        }

		void EnterCombat(Unit* p_Attacker) override
		{
            events.ScheduleEvent(eSwiftSproutlingEvents::EventTendonRip, urand(7 * TimeConstants::IN_MILLISECONDS, 11 * TimeConstants::IN_MILLISECONDS));
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
            case eSwiftSproutlingEvents::EventTendonRip:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSwiftSproutlingSpells::SpellTendonRip);
                    events.ScheduleEvent(eSwiftSproutlingEvents::EventTendonRip, urand(7 * TimeConstants::IN_MILLISECONDS, 11 * TimeConstants::IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

			DoMeleeAttackIfReady();
		}
	};

    CreatureAI* GetAI(Creature* p_Creature) const override
	{
        return new the_everbloom_yalnu_mob_swift_sproutlingAI(p_Creature);
	}
};

/// Vicious Mandragora - 84399
class the_everbloom_yalnu_mob_vicious_mandragora : public CreatureScript
{
public:

    the_everbloom_yalnu_mob_vicious_mandragora() : CreatureScript("the_everbloom_yalnu_mob_vicious_mandragora") { }

    struct the_everbloom_yalnu_mob_vicious_mandragoraAI : public ScriptedAI
    {
        the_everbloom_yalnu_mob_vicious_mandragoraAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eViciousMandragoraSpells
        {
            SpellNoxiousBreath = 169878
        };

        enum eViciousMandragoraEvents
        {
            EventNoxiousBreath = 1
        };
        
        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            DoZoneInCombat();
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
                        me->CastSpell(l_Target, eViciousMandragoraSpells::SpellNoxiousBreath);
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
        return new the_everbloom_yalnu_mob_vicious_mandragoraAI(p_Creature);
    }
};

/// Gnarled Ancient - 84312
class the_everbloom_yalnu_mob_gnarled_ancient : public CreatureScript
{
public:

    the_everbloom_yalnu_mob_gnarled_ancient() : CreatureScript("the_everbloom_yalnu_mob_gnarled_ancient") { }

    struct the_everbloom_yalnu_mob_gnarled_ancientAI : public ScriptedAI
    {
        the_everbloom_yalnu_mob_gnarled_ancientAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eGnarledAncientSpells
        {
            SpellLumberingSwipe = 169929
        };

        enum eGnarledAncientEvents
        {
            EventLumberingSwipe = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        { 
            events.Reset();
            DoZoneInCombat();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eGnarledAncientEvents::EventLumberingSwipe, urand(11 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
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
            case eGnarledAncientEvents::EventLumberingSwipe:
                        DoCastAOE(eGnarledAncientSpells::SpellLumberingSwipe);
                        events.ScheduleEvent(eGnarledAncientEvents::EventLumberingSwipe, urand(12 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }
            
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_yalnu_mob_gnarled_ancientAI(p_Creature);
    }
};

/// Entangling - 84499
class the_everbloom_yalnu_mob_entangling : public CreatureScript
{
public:

    the_everbloom_yalnu_mob_entangling() : CreatureScript("the_everbloom_yalnu_mob_entangling") { }

    struct the_everbloom_yalnu_mob_entanglingAI : public NullCreatureAI
    {
        the_everbloom_yalnu_mob_entanglingAI(Creature* p_Creature) : NullCreatureAI(p_Creature), m_VictimGUID(0)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eEntanglingSpells
        {
            SpellEntanglementVisual = 169192,
            SpellEntanglmentAura    = 169240
        };

        uint64 m_VictimGUID;
        InstanceScript* m_Instance;

        void Reset() override
        {
            me->SetDisplayId(InvisibleDisplay);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->CastSpell(me, eEntanglingSpells::SpellEntanglementVisual);
        }

        void SetGUID(uint64 p_Guid, int32 /*p_Param*/) override
        {
            m_VictimGUID = p_Guid;
            if (m_VictimGUID)
            {
                if (Unit* l_Victim = Unit::GetUnit(*me, m_VictimGUID))
                    l_Victim->AddAura(eEntanglingSpells::SpellEntanglmentAura, l_Victim);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_VictimGUID)
            {
                if (Unit* l_Victim = Unit::GetUnit(*me, m_VictimGUID))
                    l_Victim->RemoveAura(eEntanglingSpells::SpellEntanglmentAura);
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_yalnu_mob_entanglingAI(p_Creature);
    }
};

/// Feral Lasher - 86684
class the_everbloom_yalnu_mob_feral_lasher : public CreatureScript
{
public:

    the_everbloom_yalnu_mob_feral_lasher() : CreatureScript("the_everbloom_yalnu_mob_feral_lasher") { }

    struct the_everbloom_yalnu_mob_feral_lasherAI : public ScriptedAI
	{
        the_everbloom_yalnu_mob_feral_lasherAI(Creature* p_Creature) : ScriptedAI(p_Creature)
		{
            m_Instance = me->GetInstanceScript();
		}

        enum eFeralLasherEvents
        {
            EventLasherVenom = 1,
            EventFeralLasherActivate
        };

        enum eFeralLasherSpells
        {
            SpellTrample     = 154535,
            SpellSubmerge    = 175123,
            SpellLasherVenom = 173563
        };

        InstanceScript* m_Instance;
        bool m_SleepMode;

		void Reset() override
		{
            events.Reset();
            DoZoneInCombat();
            m_SleepMode = true;
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->AddAura(eFeralLasherSpells::SpellSubmerge, me);
            events.ScheduleEvent(eFeralLasherEvents::EventFeralLasherActivate, 14 * TimeConstants::IN_MILLISECONDS);
		}

        void Trample() // Hardcoded
        {
            if (me->FindNearestPlayer(1.8f, true) && m_SleepMode)
            {       
                me->Kill(me);
                m_SleepMode = false;
                me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                me->CastSpell(me, eFeralLasherSpells::SpellTrample);
            }
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
                case eFeralLasherEvents::EventLasherVenom:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eFeralLasherSpells::SpellLasherVenom);
                        events.ScheduleEvent(eFeralLasherEvents::EventLasherVenom, urand(3 * TimeConstants::IN_MILLISECONDS, 6 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                case eFeralLasherEvents::EventFeralLasherActivate:
                    {
                        m_SleepMode = false;
                        me->RemoveAllAuras();
                        me->setFaction(HostileFaction);              
                        me->RemoveAura(eFeralLasherSpells::SpellSubmerge);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        events.ScheduleEvent(eFeralLasherEvents::EventLasherVenom, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
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
        return new the_everbloom_yalnu_mob_feral_lasherAI(p_Creature);
	}
};

/// Trigger Teleport - 324251
class the_everbloom_yalnu_creature_teleport_to_boss : public CreatureScript
{
public:

    the_everbloom_yalnu_creature_teleport_to_boss() : CreatureScript("the_everbloom_yalnu_creature_teleport_to_boss") { }

    struct the_everbloom_yalnu_creature_teleport_to_bossAI : public ScriptedAI
    {
        the_everbloom_yalnu_creature_teleport_to_bossAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->setFaction(FriendlyFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
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
        return new the_everbloom_yalnu_creature_teleport_to_bossAI(p_Creature);
    }
};

/// Genesis Tick - 169613
class the_everbloom_yalnu_spell_genesis : public SpellScriptLoader
{
public:

    the_everbloom_yalnu_spell_genesis() : SpellScriptLoader("the_everbloom_yalnu_spell_genesis") { }

    class the_everbloom_yalnu_spell_genesis_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_yalnu_spell_genesis_SpellScript);

        enum eGenesisSpells
        {
            SpellGenesisProjectile = 175975
        };

        void GenesisTick()
        {
            if (!GetCaster())
                return;

            for (uint8 l_I = 0; l_I <= 13; l_I++)
                GetCaster()->CastSpell(GetCaster(), eGenesisSpells::SpellGenesisProjectile, true);
        }

        void Register()
        {
            BeforeCast += SpellCastFn(the_everbloom_yalnu_spell_genesis_SpellScript::GenesisTick);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_yalnu_spell_genesis_SpellScript();
    }
};

/// Font of Life - 169121
class the_everbloom_yalnu_spell_font_of_life : public SpellScriptLoader
{
public:

    the_everbloom_yalnu_spell_font_of_life() : SpellScriptLoader("the_everbloom_yalnu_spell_font_of_life") { }

    class the_everbloom_yalnu_spell_font_of_life_SpellScript : public SpellScript
	{
        PrepareSpellScript(the_everbloom_yalnu_spell_font_of_life_SpellScript);

        enum eFontOfLifeSpells
        {
            SpellFontOfLifeSummon = 169121
        };

        void HandleSummon(SpellEffIndex p_EffIndex)
        {
            if (!GetCaster())
                return;

            if (SpellInfo const* l_Spell = sSpellMgr->GetSpellInfo(eFontOfLifeSpells::SpellFontOfLifeSummon))
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

                if (l_Value)
                {
                    Position l_Pos;
                    GetExplTargetDest()->GetPosition(&l_Pos);
                    GetCaster()->SummonCreature(l_Value, l_Pos, TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, l_Spell->GetDuration());
                }
            }
		}

        void Register()
        {
            OnEffectHit += SpellEffectFn(the_everbloom_yalnu_spell_font_of_life_SpellScript::HandleSummon, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SUMMON);
        }
	};

    SpellScript* GetSpellScript() const override
	{
        return new the_everbloom_yalnu_spell_font_of_life_SpellScript();
	}
};

/// Colossal Blow - 169179 
class the_everbloom_yalnu_spell_colossal_blow : public SpellScriptLoader
{
public:

    the_everbloom_yalnu_spell_colossal_blow() : SpellScriptLoader("the_everbloom_yalnu_spell_colossal_blow") { }

    class the_everbloom_yalnu_spell_colossal_blow_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_yalnu_spell_colossal_blow_SpellScript);

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

        void Register()
        {
            OnHit += SpellHitFn(the_everbloom_yalnu_spell_colossal_blow_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_yalnu_spell_colossal_blow_SpellScript();
    }
};


void AddSC_boss_yalnu()
{
	new boss_yalnu();                                   ///< 83846
    new the_everbloom_yalnu_mob_kirin_tor_mage();       ///< 84329
    new the_everbloom_yalnu_mob_gnarled_ancient();      ///< 84312
    new the_everbloom_yalnu_creature_teleport_to_boss();///< 324251
    new the_everbloom_yalnu_mob_feral_lasher();         ///< 86684
    new the_everbloom_yalnu_mob_swift_sproutling();     ///< 84401
    new the_everbloom_yalnu_mob_vicious_mandragora();   ///< 84399
    new the_everbloom_yalnu_mob_undermage_kealson();    ///< 85496
    new the_everbloom_yalnu_mob_entangling();           ///< 84499
    new the_everbloom_yalnu_spell_font_of_life();       ///< 169121
    new the_everbloom_yalnu_spell_genesis();            ///< 169613
    new the_everbloom_yalnu_spell_colossal_blow();      ///< 169179
}