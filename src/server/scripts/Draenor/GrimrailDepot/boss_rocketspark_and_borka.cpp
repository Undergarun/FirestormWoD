////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "grimrail_depot.hpp"

enum eRocketsparkTalks
{
    TalkRocketsparkDeath,   ///< This was not the plan [43731]
    TalkRocketsparkEvent03, ///< [43732]
    TalkRocketsparkEvent04, ///< That was my entire collection of heartstone cards [43733]
    TalkRocketsparkEvent05, ///< [43734]
    TalkRocketsparkEvent06, ///< [43735]
    TalkRocketsparkEvent07, ///< Time to go to plan B! [43736]
    TalkRocketsparkEvent08, ///< All systems online! Fire! [43737]
    TalkRocketsparkIntro,   ///< Overthere why do you keep asking? you're always move the boxes from here to there [43738]
    TalkRocketsparkKill01,  ///<  Hah, got one! [43740]
    TalkRocketsparkKill02   ///< Stop asking and get back to work [43741]
};

enum eBorkaTalks
{
    TalkBorkaAggro = 1,     ///< I got this boss [45775]
    TalkBorkaDeath,         ///<   [45776]
    TalkBorkaSpell01,       ///< Don't get in my way [45782]
    TalkBorkaSpell02,       ///< I'm unstoppable! [45783]
    TalkBorkaIntro01,       ///< [45778]
    TalkBorkaIntro02,       ///< [45779]
    TalkBorkaKill01,        ///<  Hahaha is to much fun [45780]
    TalkBorkaKill02         ///< [45781]
};

enum eRocksparkAndBorkaActions
{
    ActionBorkaUnmanagedAggression = 1,
    ActionRocketSparkNewPlan,
    ActionBorkaActivateMadDash,
    ActionRocketSparkShootMissilesVX18
};

enum eRocksparkAndBorkaMovementInformed
{
    MovementInformedRocketsparkHigherGroundsStartx2101aMissileBarrage = 1
};

enum eRocksparkAndBorkaCreatures
{
    CreatureTriggerLineOfSight = 0
};

Position const g_PositionRandomRocketSparkMovements[4] =
{
    { 1718.639f, 1572.846f, 7.7136f, 3.616752f },
    { 1687.059f, 1572.249f, 7.7136f, 0.032194f },
    { 1719.248f, 1553.528f, 7.7136f, 2.866693f },
    { 1692.416f, 1583.240f, 7.7136f, 5.500137f }
};

Position const g_PostionRocketSparkHigherPlatform[2]= /// X21-01A Missile Barrage
{
    { 1708.924f, 1647.510f, 17.315f, 1.610062f },
    { 1726.215f, 1580.797f, 14.062f, 4.130380f }
};

static void StartRocketSparkAndBorka(InstanceScript* p_Instance, Creature* /*p_Me*/, Unit* /*p_Target*/)
{
    if (p_Instance == nullptr)
        return;

    p_Instance->SetBossState(GrimrailDepotData::DataRocketspark, EncounterState::IN_PROGRESS);
    if (Creature* l_Borka = p_Instance->instance->GetCreature(p_Instance->GetData64(GrimrailDepotData::DataBorka)))
        l_Borka->SetInCombatWithZone();
    if (Creature* l_Rocketspark = p_Instance->instance->GetCreature(p_Instance->GetData64(GrimrailDepotData::DataRocketspark)))
        l_Rocketspark->SetInCombatWithZone();
}

static void WipingConditionRocketSparkAndBorka(InstanceScript* p_Instance, Creature* /*p_Me*/)
{
    if (p_Instance == nullptr)
        return;

    if (Creature* l_Borka = p_Instance->instance->GetCreature(p_Instance->GetData64(GrimrailDepotData::DataBorka)))
    {
        if (Creature* l_Rocketspark = p_Instance->instance->GetCreature(p_Instance->GetData64(GrimrailDepotData::DataRocketspark)))
        {
            p_Instance->SetBossState(GrimrailDepotData::DataRocketspark, EncounterState::FAIL);
            l_Borka->Respawn();
            l_Borka->GetMotionMaster()->MovePoint(0, l_Borka->GetHomePosition().GetPositionX(), l_Borka->GetHomePosition().GetPositionY(), l_Borka->GetHomePosition().GetPositionZ());
            l_Rocketspark->Respawn();
            l_Rocketspark->GetMotionMaster()->MovePoint(0, l_Rocketspark->GetHomePosition().GetPositionX(), l_Rocketspark->GetHomePosition().GetPositionY(), l_Rocketspark->GetHomePosition().GetPositionZ());
        }
    }
}

static void WiningConditionRocketsparkAndBurka(InstanceScript* p_Instance, Creature* /*p_Me*/, Unit* l_Killer)
{
    if (p_Instance == nullptr)
        return;

    if (Creature* l_Borka = p_Instance->instance->GetCreature(p_Instance->GetData64(GrimrailDepotData::DataBorka)))
    {
        if (Creature* l_Rocketspark = p_Instance->instance->GetCreature(p_Instance->GetData64(GrimrailDepotData::DataRocketspark)))
        {
            if (l_Borka->isDead() || l_Rocketspark->isDead())
            {
                p_Instance->SetBossState(GrimrailDepotData::DataRocketspark, EncounterState::DONE);
                l_Rocketspark->SetLootRecipient(l_Killer);
            }
        }
    }
}


/// Railmaster Rocketspark - 77803 [boss]
class boss_rocketspark : public CreatureScript
{
public:

    boss_rocketspark() : CreatureScript("boss_rocketspark") { }

    struct boss_rocketsparkAI : public BossAI
    {
        boss_rocketsparkAI(Creature* p_Creature) : BossAI(p_Creature, GrimrailDepotData::DataRocketspark)
        {
            m_Instance = me->GetInstanceScript();
            m_First = false;
        }

        enum eRocketsparkSpells
        {
            SpellAcquiringTargetsDummy               = 162489,
            SpellAcquiringTargetsAura                = 162507,
            SpellBetterPositionDummy                 = 162571,
            SpellBetterPositionJump                  = 162171,
            SpellLockingOn                           = 166561,
            SpellLockingOnAura                       = 166559,
            SpellMissileSmoke                        = 162752,
            SpellNewPlanDummy                        = 163927,
            SpellNewPlanAura                         = 161091,
            SpellNewPlanDamage                       = 163929,
            SpellX2101aMissileBarrage                = 162407,
            SpellX2101aMissileBarrageDamage          = 162422,
            SpellVx18BTargetEliminatorDummy          = 162500,
            SpellVx18BTargetEliminatorTriggerMissile = 162509,
            SpellVx18BTargetEliminatorDamage         = 162513,
            SpellMadDashAura                         = 169225,
            SpellRecovering                          = 163947
        };

        enum eRocketsparkEvents
        {
            EventBetterPosition = 1,
            EventNewPlan,
            EventVx18BTargetEliminator,
            EventX2101a,
            Eventx2101aCancelation,
            EventRandomMovement
        };

        InstanceScript* m_Instance;
        bool m_First;
        bool m_x210IsActivated;
        bool m_NewPlan;
        uint8 m_MovementPosition;
        uint8 m_MovementHigherGround;
  
        void Reset() override
        {
            events.Reset();
            m_NewPlan = false;
            m_x210IsActivated = false;
            m_MovementPosition     = 0;
            m_MovementHigherGround = 0;
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            if (m_Instance != nullptr)
                WipingConditionRocketSparkAndBorka(m_Instance, me);
        }

        void EnterCombat(Unit* p_Who) override
        {
            if (m_Instance != nullptr)
            {
                StartRocketSparkAndBorka(m_Instance, me, p_Who);
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
            }

            _EnterCombat();
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            events.ScheduleEvent(eRocketsparkEvents::EventVx18BTargetEliminator, 0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eRocketsparkEvents::EventX2101a, 0 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eRocketsparkTalks::TalkRocketsparkKill01);
                else
                    Talk(eRocketsparkTalks::TalkRocketsparkKill02);
            }
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eRocksparkAndBorkaMovementInformed::MovementInformedRocketsparkHigherGroundsStartx2101aMissileBarrage:
                    me->CastSpell(me, eRocketsparkSpells::SpellX2101aMissileBarrage);
                    break;
                default:
                    break;
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eRocksparkAndBorkaActions::ActionRocketSparkShootMissilesVX18:
                    for (uint8 l_I = 0; l_I < 6; l_I)
                    {
                        Position l_Position;
                        me->GetRandomNearPosition(l_Position, 55.0f);
                        me->CastSpell(l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetPositionZ(), eRocketsparkSpells::SpellVx18BTargetEliminatorTriggerMissile, true);
                    }
                    break;
                case eRocksparkAndBorkaActions::ActionRocketSparkNewPlan:
                    if (!m_NewPlan)
                        m_NewPlan = true;

                    events.Reset();
                    events.ScheduleEvent(eRocketsparkEvents::EventNewPlan, 5 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }

        void JustDied(Unit* p_Killer) override
        {
            _JustDied();
            Talk(eRocketsparkTalks::TalkRocketsparkDeath);

            if (m_Instance != nullptr)
            {
                WiningConditionRocketsparkAndBurka(m_Instance, me, p_Killer);
                if (Creature* l_Borka = m_Instance->instance->GetCreature(m_Instance->GetData64(GrimrailDepotData::DataBorka)))
                {
                    if (l_Borka->isAlive())
                    {
                        if (l_Borka->IsAIEnabled)
                            l_Borka->GetAI()->DoAction(eRocksparkAndBorkaActions::ActionBorkaUnmanagedAggression);
                    }
                }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            ///< While Newplan is active all other abilities are disabled (auto attack included)
            if (m_NewPlan)
                return;

            ///< In order to stop the x210 session, players must bring borka with mad dash towards x210 location.
            if (m_x210IsActivated)
            {
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Borka = m_Instance->instance->GetCreature(m_Instance->GetData64(GrimrailDepotData::DataBorka)))
                    {
                        if (l_Borka->HasAura(eRocketsparkSpells::SpellMadDashAura))
                        {
                            if (l_Borka->IsWithinDistInMap(me, 2.0f, true))
                            {
                                me->RemoveAllAuras();
                                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                me->CastSpell(me, eRocketsparkSpells::SpellRecovering);
                                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                            }
                        }
                    }
                }
            }

            ///< While x210 is activated all other abilities are disabled. (auto attack included).
            if (m_x210IsActivated)
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eRocketsparkEvents::EventX2101a:
                    m_x210IsActivated = true;
                    m_MovementHigherGround = 0;
                    m_MovementHigherGround = urand(0, 1);
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->CastSpell(me, eRocketsparkSpells::SpellLockingOnAura);
                    me->CastSpell(me, eRocketsparkSpells::SpellBetterPositionJump, true);
                    me->GetMotionMaster()->MoveJump(g_PostionRocketSparkHigherPlatform[m_MovementHigherGround], 20.0f, 15.0f);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    events.ScheduleEvent(eRocketsparkEvents::EventX2101a, 40 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eRocketsparkEvents::Eventx2101aCancelation, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eRocketsparkEvents::Eventx2101aCancelation:
                    events.Reset();
                    me->RemoveAllAuras();
                    m_x210IsActivated = false;
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    break;
                case eRocketsparkEvents::EventVx18BTargetEliminator:
                    me->CastSpell(me, eRocketsparkSpells::SpellVx18BTargetEliminatorDummy);
                    events.ScheduleEvent(eRocketsparkEvents::EventVx18BTargetEliminator, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eRocketsparkEvents::EventNewPlan:
                    me->CastSpell(me, eRocketsparkSpells::SpellNewPlanAura);
                    break;
                case eRocketsparkEvents::EventRandomMovement:
                {
                    if (m_MovementPosition > 4)
                        m_MovementPosition = 0;

                    m_MovementPosition++;

                    me->CastSpell(g_PositionRandomRocketSparkMovements[m_MovementPosition].GetPositionX(), g_PositionRandomRocketSparkMovements[m_MovementPosition].GetPositionY(), g_PositionRandomRocketSparkMovements[m_MovementPosition].GetPositionZ(), eRocketsparkSpells::SpellBetterPositionJump, true);
                    events.ScheduleEvent(eRocketsparkEvents::EventRandomMovement, 8 * TimeConstants::IN_MILLISECONDS);
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
        return new boss_rocketsparkAI(p_Creature);
    }
};

/// Borka The Brute - 86226
class grimrail_depot_rocketspark_mob_Borka : public CreatureScript
{
public:

    grimrail_depot_rocketspark_mob_Borka() : CreatureScript("grimrail_depot_rocketspark_mob_Borka") { }

    struct grimrail_depot_rocketspark_mob_BorkaAI : public ScriptedAI
    {
        grimrail_depot_rocketspark_mob_BorkaAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eBorkaSpells
        {
            SpellMadDashDummy        = 161088,
            SpellMadDashDamage       = 161089,
            SpellMadDashCharge       = 161102,
            SpellMadDashAura         = 169225,
            SpellSlam                = 161087,
            SpellUnmanagedAggression = 161092
        };

        enum eBorkaEvents
        {
            EventMadDash = 1,
            EventMadDashActionDelayed,
            EventSlam,
            EventUnmanagedAggression
        };

        InstanceScript* m_Instance;
        uint64 m_MadDashTargetGUID;
        bool m_Intro;

        void Reset() override
        {
            events.Reset();
            m_MadDashTargetGUID = 0;
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && !m_Intro && me->IsWithinDistInMap(p_Who, 10.0f))
            {

            }
        }

        void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
        {
            if (p_Damage && p_Damage > 0)
            {

            }
        }

        void JustReachedHome() override
        {
            if (m_Instance != nullptr)
                WipingConditionRocketSparkAndBorka(m_Instance, me);
        }

        void EnterCombat(Unit* p_Who) override
        {
            if (m_Instance != nullptr)
                StartRocketSparkAndBorka(m_Instance, me, p_Who);


            Talk(eBorkaTalks::TalkBorkaAggro);
            events.ScheduleEvent(eBorkaEvents::EventSlam, 0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBorkaEvents::EventMadDash, 0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eBorkaEvents::EventUnmanagedAggression, 0 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                switch (urand(0, 1))
                {
                    case 0:
                        Talk(eBorkaTalks::TalkBorkaKill01);
                        break;
                    case 1:
                        Talk(eBorkaTalks::TalkBorkaKill02);
                        break;
                    default:
                        break;
                }
            }
        }

        void JustDied(Unit* p_Killer) override
        {
            /// New Plan! - Incase Borka dies first.
            if (m_Instance != nullptr)
            {
                if (Creature* l_RocketSpark = m_Instance->instance->GetCreature(m_Instance->GetData64(GrimrailDepotData::DataRocketspark)))
                {
                    if (l_RocketSpark->IsAIEnabled)
                        l_RocketSpark->GetAI()->DoAction(eRocksparkAndBorkaActions::ActionRocketSparkNewPlan);
                }
            }

            if (m_Instance != nullptr)
                WiningConditionRocketsparkAndBurka(m_Instance, me, p_Killer);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eRocksparkAndBorkaActions::ActionBorkaActivateMadDash:
                    if (!m_MadDashTargetGUID)
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        {
                            m_MadDashTargetGUID = l_Target->GetGUID();
                            l_Target->CastSpell(l_Target, eBorkaSpells::SpellMadDashAura, true);
                        }

                        events.ScheduleEvent(eBorkaEvents::EventMadDashActionDelayed, 0 * TimeConstants::IN_MILLISECONDS);
                    }
                    else
                    break;
                case eRocksparkAndBorkaActions::ActionBorkaUnmanagedAggression:
                    me->CastSpell(me, eBorkaSpells::SpellUnmanagedAggression);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            /// Mad Rush
            if (m_Instance != nullptr)
            {
                if (m_MadDashTargetGUID)
                {
                    if (Player* l_Target = Player::GetPlayer(*me, m_MadDashTargetGUID))
                    {
                        if (Creature* l_NearestLineOfSight = me->FindNearestCreature(eRocksparkAndBorkaCreatures::CreatureTriggerLineOfSight, 1.0f, true))
                        {
                            if (Creature* l_RocketSpark = m_Instance->instance->GetCreature(m_Instance->GetData64(GrimrailDepotData::DataRocketspark)))
                            {
                                if (l_RocketSpark->IsWithinDistInMap(l_NearestLineOfSight, 5.0f, true)) /// If it's Rocketspark line of sight.
                                {
                                    me->StopMoving();
                                    m_MadDashTargetGUID = 0;
                                    me->CastSpell(me, eBorkaSpells::SpellMadDashDamage);
                                    if (Unit* l_Victim = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                                        me->Attack(l_Victim, true);
                                }
                            }
                        }
                    }
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eBorkaEvents::EventMadDash: /// 01
                    me->CastSpell(me, eBorkaSpells::SpellMadDashDummy);
                    events.ScheduleEvent(eBorkaEvents::EventMadDash, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eBorkaEvents::EventMadDashActionDelayed: /// 02
                    if (!m_MadDashTargetGUID)
                    {
                        if (Player* l_Player = Player::GetPlayer(*me, m_MadDashTargetGUID))
                            me->CastSpell(l_Player, eBorkaSpells::SpellMadDashCharge);
                    }
                    break;
                case eBorkaEvents::EventSlam:
                    me->CastSpell(me, eBorkaSpells::SpellSlam);
                    events.ScheduleEvent(eBorkaEvents::EventSlam, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eBorkaEvents::EventUnmanagedAggression:
                    events.ScheduleEvent(eBorkaEvents::EventUnmanagedAggression, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_rocketspark_mob_BorkaAI(p_Creature);
    }
};

/// Mad Dash - 161088
class grimrail_depot_rocketspark_spell_mad_dash : public SpellScriptLoader
{
public:

    grimrail_depot_rocketspark_spell_mad_dash() : SpellScriptLoader("grimrail_depot_rocketspark_spell_mad_dash") { }

    class grimrail_depot_rocketspark_spell_mad_dash_SpellScript : public SpellScript
    {
        PrepareSpellScript(grimrail_depot_rocketspark_spell_mad_dash_SpellScript)

        void HandleDummy(SpellEffIndex /*p_EffIndex*/)
        {
            if (GetCaster())
            {
                if (GetCaster()->IsAIEnabled)
                    GetCaster()->GetAI()->DoAction(eRocksparkAndBorkaActions::ActionBorkaActivateMadDash);
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(grimrail_depot_rocketspark_spell_mad_dash_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new grimrail_depot_rocketspark_spell_mad_dash_SpellScript();
    }
};

/// VX18-B Target Eliminator - 162500
class grimrail_depot_rocketspark_spell_vx18_target_eliminator : public SpellScriptLoader
{
public:

    grimrail_depot_rocketspark_spell_vx18_target_eliminator() : SpellScriptLoader("grimrail_depot_rocketspark_spell_vx18_target_eliminator") { }

    class grimrail_depot_rocketspark_spell_vx18_target_eliminator_SpellScript : public SpellScript
    {
        PrepareSpellScript(grimrail_depot_rocketspark_spell_vx18_target_eliminator_SpellScript)

        void HandleDummy(SpellEffIndex /*p_EffIndex*/)
        {
            if (GetCaster())
            {
                if (GetCaster()->IsAIEnabled)
                    GetCaster()->GetAI()->DoAction(eRocksparkAndBorkaActions::ActionRocketSparkShootMissilesVX18);
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(grimrail_depot_rocketspark_spell_vx18_target_eliminator_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new grimrail_depot_rocketspark_spell_vx18_target_eliminator_SpellScript();
    }
};

/// X21-01A Missile Barrage - 162407
class grimrail_depot_rocketspark_spell_x21_01a_missile_barrage : public SpellScriptLoader
{
public:
    grimrail_depot_rocketspark_spell_x21_01a_missile_barrage() : SpellScriptLoader("grimrail_depot_rocketspark_spell_x21_01a_missile_barrage") { }

    class grimrail_depot_rocketspark_spell_x21_01a_missile_barrage_AuraScript : public AuraScript
    {
        PrepareAuraScript(grimrail_depot_rocketspark_spell_x21_01a_missile_barrage_AuraScript)

        enum eSpells
        {
            SpellX2101AMissileBarrage   = 162422,
            SpellX2101AMissileBarrage01 = 162423
        };

        void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
        {
            PreventDefaultAction();

            /// There are two entries, both must be casted at once. (It seems like a pretty tough fight for dem scrubs).
            uint32 l_Entries[2] = { eSpells::SpellX2101AMissileBarrage01, eSpells::SpellX2101AMissileBarrage };

            if (GetCaster())
            {
                if (InstanceScript* l_Instance = GetCaster()->GetInstanceScript())
                {
                    if (Creature* l_Borka = l_Instance->instance->GetCreature(l_Instance->GetData64(GrimrailDepotData::DataBorka)))
                    {
                        Position l_Position;
                        l_Borka->GetRandomNearPosition(l_Position, 40.0f);

                        for (uint8 l_I = 0; l_I < 2; l_I++)
                            GetCaster()->CastSpell(l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetPositionZ(), l_Entries[l_I], true);
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(grimrail_depot_rocketspark_spell_x21_01a_missile_barrage_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new grimrail_depot_rocketspark_spell_x21_01a_missile_barrage_AuraScript();
    }
};

/// New Plan - 161091 
class grimrail_depot_rocketspark_spell_new_plan : public SpellScriptLoader
{
public:
    grimrail_depot_rocketspark_spell_new_plan() : SpellScriptLoader("grimrail_depot_rocketspark_spell_new_plan") { }

    class grimrail_depot_rocketspark_spell_new_plan_AuraScript : public AuraScript
    {
        PrepareAuraScript(grimrail_depot_rocketspark_spell_new_plan_AuraScript)

        enum eSpells
        {
            SpellNewPlanMissile   = 163929,
            SpellNewPlanMissile01 = 163930
        };

        void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
        {
            PreventDefaultAction();

            /// There are two entries, both must be casted at once. (It seems like a pretty tough fight for dem scrubs).
            uint32 l_Entries[2] = { eSpells::SpellNewPlanMissile, eSpells::SpellNewPlanMissile01 };

            if (GetCaster())
            {
                Position l_Position;
                GetCaster()->GetRandomNearPosition(l_Position, 40.0f);

                for (uint8 l_I = 0; l_I < 2; l_I++)
                    GetCaster()->CastSpell(l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetPositionZ(), l_Entries[l_I], true);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(grimrail_depot_rocketspark_spell_new_plan_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new grimrail_depot_rocketspark_spell_new_plan_AuraScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_RocketsparkandBorka()
{
    new boss_rocketspark();
    new grimrail_depot_rocketspark_mob_Borka();
    new grimrail_depot_rocketspark_spell_x21_01a_missile_barrage();
    new grimrail_depot_rocketspark_spell_vx18_target_eliminator();
    new grimrail_depot_rocketspark_spell_new_plan();
    new grimrail_depot_rocketspark_spell_mad_dash();
};
#endif
