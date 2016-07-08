////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "ScriptedCreature.h"
#include "auchindoun.hpp"

enum eEmotes
{
    EMOTE_HELLO_NYAMI = 3,
 
};

Position const g_PositionKaatharCrystalPosition = { 1909.75f, 3188.70f, 66.786f, 5.401960f };

/// 1st Starting Event
class EventTuulaniIntroduction : public BasicEvent
{
    public:

    explicit EventTuulaniIntroduction(Unit* p_Unit, int p_Value, InstanceScript* p_Instance) : m_Obj(p_Unit), m_Modifier(p_Value), m_InstanceScript(p_Instance), BasicEvent()
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (m_InstanceScript != nullptr)
            {
                if (Creature* l_Tuulina = m_InstanceScript->instance->GetCreature(m_InstanceScript->GetData64(eAuchindounDatas::DataTuulani)))
                {
                    if (Creature* l_Nyami = m_InstanceScript->instance->GetCreature(m_InstanceScript->GetData64(eAuchindounDatas::DataNyami)))
                    {
                        if (Creature* l_Guard = m_InstanceScript->instance->GetCreature(m_InstanceScript->GetData64(eAuchindounDatas::DataGuard)))
                        {
                            if (l_Tuulina->IsAIEnabled && l_Guard->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                case 0:
                                {
                                    l_Nyami->SetReactState(ReactStates::REACT_PASSIVE);
                                    l_Nyami->AddAura(eAuchindounSpells::SpellDarkFire, l_Nyami);
                                    l_Nyami->CastSpell(l_Nyami, eAuchindounSpells::SpellPrisonAura);
                                    l_Nyami->AddAura(eAuchindounSpells::SpellLevitateNyami, l_Nyami);
                                    l_Nyami->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                                    if (Creature* l_Trigger = l_Nyami->FindNearestCreature(eAuchindounCreatures::CreatureLeftCrystalTrigger, 40.0f, true))
                                    {
                                        l_Trigger->AddAura(eAuchindounSpells::SpellVoidFormTriggerBuff, l_Trigger);
                                        l_Nyami->CastSpell(l_Trigger, eAuchindounSpells::SpellShadowBeam);
                                    }

                                    l_Tuulina->AI()->Talk(eAuchindounTalks::TUULANITALK18);
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani02, g_PositionTuulaniMovements[1]);
                                    break;
                                }
                                case 1:
                                {
                                    l_Tuulina->AI()->Talk(eAuchindounTalks::TUULANITALK3);
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani03, g_PositionTuulaniMovements[2]);
                                    break;
                                }
                                case 3:
                                {
                                    l_Guard->SetFacingToObject(l_Tuulina);
                                    l_Guard->RemoveAura(eAuchindounSpells::SpellKneel);
                                    l_Guard->AI()->Talk(eAuchindounTalks::AUCHENAIDEFENDERTALK1);
                                    l_Guard->m_Events.AddEvent(new EventTuulaniIntroduction(l_Guard, 4, m_InstanceScript), l_Guard->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 4:
                                {
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani04, g_PositionTuulaniMovements[4]);
                                    break;
                                }
                                case 5:
                                {
                                    l_Tuulina->AI()->Talk(eAuchindounTalks::TUULANITALK4);
                                    l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 6, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(4 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 6:
                                {
                                    l_Tuulina->CastSpell(l_Tuulina, eAuchindounSpells::SpellTuulaniUnlock);
                                    l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 7, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 7:
                                {
                                    if (GameObject* l_NearestHolyWall = l_Tuulina->FindNearestGameObject(eAuchindounObjects::GameobjectHolyWall, 60.0f))
                                        l_NearestHolyWall->Delete();

                                    l_Tuulina->AI()->Talk(eAuchindounTalks::TUULANITALK2);
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani05, g_PositionTuulaniMovements[5]);
                                    break;
                                }
                                case 8:
                                {
                                    l_Tuulina->AI()->Talk(eAuchindounTalks::TUULANITALK5);
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani06, g_PositionTuulaniMovements[6]);
                                    break;
                                }
                                case 9:
                                {
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani07, g_PositionTuulaniMovements[7]);
                                    break;
                                }
                                case 10:
                                {
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani08, g_PositionTuulaniMovements[8]);
                                    break;
                                }
                                case 11:
                                {
									std::list<Creature*> l_ListCreatures;
									uint32 l_Entries[7] = { eAuchindounCreatures::CreatureSoulBinderTuulani ,eAuchindounCreatures::CreatureSoulBinderNyami, eAuchindounBosses::BossKaathar,eAuchindounCreatures::CreatureSargeriZealot, eAuchindounCreatures::CreatureSargeriSoulBinder, eAuchindounCreatures::CreatureSargeriRitualist, eAuchindounCreatures::CreatureSargeiHoplite };

									for (uint8 l_I = 0; l_I < 7; l_I++)
									{
										l_Tuulina->GetCreatureListWithEntryInGrid(l_ListCreatures, l_Entries[l_I], 200.0f);
									}

									if (!l_ListCreatures.empty())
									{
										for (Creature* l_Itr : l_ListCreatures)
										{
											if (!l_Itr)
												continue;

											l_Itr->SetPhaseMask(4, true);
										}
									}

									std::list<Player*> l_ListPlayers;
									l_Tuulina->GetPlayerListInGrid(l_ListPlayers, 600.0f, true);
									if (!l_ListPlayers.empty())
									{
										for (Player* l_Itr : l_ListPlayers)
										{
											if (!l_Itr)
												continue;

											l_Itr->NearTeleportTo(*l_Tuulina);
											l_Itr->PlayScene(eAuchindounScenes::SpellAuchindounSceneTulaaniReachNyami, l_Itr);
										}
									}

                                    l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 15, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(12 * TimeConstants::IN_MILLISECONDS));
                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani09, g_PositionTuulaniMovements[9]);

									l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 12, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(5 * TimeConstants::IN_MILLISECONDS));

                                    break;
                                }
                                case 12:
								{							

                                    l_Tuulina->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani10, g_PositionTuulaniMovements[10]);
                                    break;
                                }
                                case 13:
                                {                                
                                    //l_Tuulina->AI()->Talk(eAuchindounTalks::TUULANITALK7);
                                    l_Tuulina->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                    l_Tuulina->AddAura(eAuchindounSpells::SpellTuulaniCapturedVoidPrison, l_Tuulina);
                                    l_Tuulina->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                    l_Tuulina->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                    //l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 15, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 15:
                                {
									std::list<Creature*> l_ListCreatures;
									uint32 l_Entries[7] = { eAuchindounCreatures::CreatureSoulBinderTuulani, eAuchindounCreatures::CreatureSoulBinderNyami, eAuchindounBosses::BossKaathar, eAuchindounCreatures::CreatureSargeriZealot, eAuchindounCreatures::CreatureSargeriSoulBinder, eAuchindounCreatures::CreatureSargeriRitualist, eAuchindounCreatures::CreatureSargeiHoplite };

									for (uint8 l_I = 0; l_I < 7; l_I++)
									{
										l_Tuulina->GetCreatureListWithEntryInGrid(l_ListCreatures, l_Entries[l_I], 200.0f);
									}

									if (!l_ListCreatures.empty())
									{
										for (Creature* l_Itr : l_ListCreatures)
										{
											if (!l_Itr)
												continue;

											l_Itr->SetPhaseMask(1, true);
										}
									}

                                    break;
                                }
                                /*
                                case 16:
                                {
                                l_Nyami->AI()->Talk(eAuchindounTalks::NYAMITALK2);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 17, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                break;
                                }
                                case 17:
                                {
                                l_Nyami->AI()->Talk(eAuchindounTalks::NYAMITALK3);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 18, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                break;
                                }
                                case 18:
                                {
                                l_Nyami->AI()->Talk(eAuchindounTalks::NYAMITALK4);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 19, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                break;
                                }
                                case 19:
                                {
                                l_Tuulina->AI()->Talk(eAuchindounTalks::TUULANITALK8);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 20, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                break;
                                }
                                case 20:
                                {
                                l_Nyami->AI()->Talk(eAuchindounTalks::NYAMITALK5);
                                l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 21, m_InstanceScript), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                break;
                                }
                                */
                                default:
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

private:
    InstanceScript* m_InstanceScript;
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};

/// Tuulani - 79248
class auchindoun_mob_tuulani : public CreatureScript
{
    public:

    auchindoun_mob_tuulani() : CreatureScript("auchindoun_mob_tuulani") {}

    struct auchindoun_mob_tuulaniAI : public ScriptedAI
    {
        auchindoun_mob_tuulaniAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript(); 
            m_First = true;
        }

        InstanceScript* m_Instance;
        bool m_First;
        uint32 m_FirstDiff;

        void Reset() override
        {       
            events.Reset();
            m_First = false;
            me->setFaction(FriendlyFaction);
            Talk(eAuchindounTalks::TUULANITALK1);
            me->SetSpeed(UnitMoveType::MOVE_RUN, 1.2f, true);
            me->SetSpeed(UnitMoveType::MOVE_WALK, 1.2f, true);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

            AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this]() -> void
            {
                 me->GetMotionMaster()->MovePoint(eAuchindounMovementInforms::MovementInformTuulani01, g_PositionTuulaniMovements[0]);
            });      
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            UpdateOperations(p_Diff);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            if (m_Instance != nullptr)
            {
                switch (p_Id)
                {
                case eAuchindounMovementInforms::MovementInformTuulani01:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 0, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani02:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 1, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani03:
                    if (m_Instance != nullptr)
                    {
                        if (Creature* l_Guard = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataGuard)))
                            l_Guard->m_Events.AddEvent(new EventTuulaniIntroduction(l_Guard, 3, m_Instance), l_Guard->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                case eAuchindounMovementInforms::MovementInformTuulani04:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 5, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani05:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 8, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani06:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 9, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani07:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 10, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani08:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 11, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani09:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 12, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eAuchindounMovementInforms::MovementInformTuulani10:
                    me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 13, m_Instance), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                    break;
                default:
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_tuulaniAI(p_Creature);
    }
};

/// Sargerei Soulbinder - 77812
class auchindoun_mob_sargerei_soulbinder : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_soulbinder() : CreatureScript("auchindoun_mob_sargerei_soulbinder") { }

    struct auchindoun_mob_sargerei_soulbinderAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_soulbinderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSargereiSoulbinderEvents
        {
            EventMindShear = 1,
            EventBendWill
        };

        enum eSargereiSoulbinderSpells
        {
            SpellVoidShell = 160312,
            SpellBendWill  = 154527
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eSargereiSoulbinderEvents::EventMindShear, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSargereiSoulbinderEvents::EventBendWill, 18 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override 
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                {
                    if (l_Kaathar->isAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(eAuchindounActions::ActionCountPre1StBossKill);
                }
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
                case eSargereiSoulbinderEvents::EventMindShear:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eSargereiSoulbinderSpells::SpellVoidShell);
                    events.ScheduleEvent(eSargereiSoulbinderEvents::EventBendWill, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eSargereiSoulbinderEvents::EventBendWill:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eSargereiSoulbinderSpells::SpellBendWill);
                    events.ScheduleEvent(eSargereiSoulbinderEvents::EventBendWill, 18 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_soulbinderAI(p_Creature);
    }
};

/// Sargerei Cleric - 77134
class auchindoun_mob_sargerei_cleric : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_cleric() : CreatureScript("auchindoun_mob_sargerei_cleric") { }

    struct auchindoun_mob_sargerei_clericAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_clericAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSargereiClericEvents
        {
            EventVoidShell = 1
        };

        enum eSargereiCleircSpells
        {
            SpellVoidShell = 160312
        };

        InstanceScript* m_Instance;

        void Reset() override
        {   
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eSargereiClericEvents::EventVoidShell, 15 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        { 
            if (m_Instance != nullptr)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                {
                    if (l_Kaathar->isAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(eAuchindounActions::ActionCountPre1StBossKill);
                }
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
                case eSargereiClericEvents::EventVoidShell:
                    me->CastSpell(me, eSargereiCleircSpells::SpellVoidShell);
                    events.ScheduleEvent(eSargereiClericEvents::EventVoidShell, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    }; 

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_clericAI(p_Creature);
    }
};

/// Sargerei Ritualist - 77130
class auchindoun_mob_sargerei_ritualist : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_ritualist() : CreatureScript("auchindoun_mob_sargerei_ritualist") { }

    struct auchindoun_mob_sargerei_ritualistAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_ritualistAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSargereiRitualistEvents
        {
            EventMindSpike = 1
        };

        enum eSargereiRitualistSpells
        {
            SpellShadowBeam = 156862,
            SpellDarkFire   = 156955,
            SpellMindSpike  = 157043
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();

            if (Creature* l_Trigger = me->SummonCreature(eAuchindounCreatures::CreatureShadowBeam, g_PositionKaatharCrystalPosition, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                me->CastSpell(l_Trigger, eSargereiRitualistSpells::SpellShadowBeam);

            me->AddAura(eSargereiRitualistSpells::SpellDarkFire, me);   
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->CastStop();
            me->RemoveAllAuras();
            events.ScheduleEvent(eSargereiRitualistEvents::EventMindSpike, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                {
                    if (l_Kaathar->isAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(eAuchindounActions::ActionCountPre1StBossKill);
                }
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
                case eSargereiRitualistEvents::EventMindSpike:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eSargereiRitualistSpells::SpellMindSpike);
                    events.ScheduleEvent(eSargereiRitualistEvents::EventMindSpike, 6 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_ritualistAI(p_Creature);
    }
};

/// Sargerei Zealot - 77132
class auchindoun_mob_sargerei_zealot : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_zealot() : CreatureScript("auchindoun_mob_sargerei_zealot") { }

    struct auchindoun_mob_sargerei_zealotAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_zealotAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSargereiZealotSpells
        {
            SpellSeverTendonAura = 157165
        };

        enum eSargereiZealotEvents
        {
            EventSeverTendom = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eAuchindounEvents::EventSeverTendom, 5 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                {
                    if (Kaathar->isAlive() && Kaathar->IsAIEnabled)
                        Kaathar->AI()->DoAction(eAuchindounActions::ActionCountPre1StBossKill);
                }
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
                case eAuchindounEvents::EventSeverTendom:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eAuchindounSpells::SpellSeverTendonAura);
                    events.ScheduleEvent(eAuchindounEvents::EventSeverTendom, urand(9 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_zealotAI(p_Creature);
    }
};

/// Sargerei Spirit Tender - 77131
class auchindoun_mob_sargerei_spirit_tender : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_spirit_tender() : CreatureScript("auchindoun_mob_sargerei_spirit_tender") { }

    struct auchindoun_mob_sargerei_spirit_tenderAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_spirit_tenderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSpiritTenderSpells
        {
            SpellVoidMendingDummy = 154623,
            SpellVoidShiftDummy   = 155524
        };

        enum eSpiritTenderEvents
        {
            EventVoidMending = 1,
            EventVoidShift
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eSpiritTenderEvents::EventVoidMending, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSpiritTenderEvents::EventVoidShift, 16 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                {
                    if (l_Kaathar->isAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(eAuchindounActions::ActionCountPre1StBossKill);
                }
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
                case eSpiritTenderEvents::EventVoidMending:
                        if (Unit* l_FriendUnit = DoSelectLowestHpFriendly(85))
                            me->CastSpell(l_FriendUnit, eSpiritTenderSpells::SpellVoidMendingDummy);
                        events.ScheduleEvent(eSpiritTenderEvents::EventVoidMending, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                case eSpiritTenderEvents::EventVoidShift:
                        me->CastSpell(me, eSpiritTenderSpells::SpellVoidShiftDummy);
                        events.ScheduleEvent(eSpiritTenderEvents::EventVoidShift, 16 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_spirit_tenderAI(p_Creature);
    };
};

/// Sargerei Hopilite - 77133
class auchindoun_mob_sargerei_hopilite : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_hopilite() : CreatureScript("auchindoun_mob_sargerei_hopilite") { }

    struct auchindoun_mob_sargerei_hopiliteAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_hopiliteAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSargereiHopiliteSpells
        {
            SpellShieldBash  = 157159,
            SpellVoidStrikes = 166749
        };

        enum eSargereiHopiliteEvents
        {
            EventShieldBash = 1,
            EventVoidStrikes
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eSargereiHopiliteEvents::EventShieldBash, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eSargereiHopiliteEvents::EventVoidStrikes, 18 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossKathaar)))
                {
                    if (l_Kaathar->isAlive() && l_Kaathar->IsAIEnabled)
                        l_Kaathar->AI()->DoAction(eAuchindounActions::ActionCountPre1StBossKill);
                }
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
                case eSargereiHopiliteEvents::EventShieldBash:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSargereiHopiliteSpells::SpellShieldBash);
                    events.ScheduleEvent(eSargereiHopiliteEvents::EventShieldBash, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eSargereiHopiliteEvents::EventVoidStrikes:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSargereiHopiliteSpells::SpellVoidStrikes);
                    events.ScheduleEvent(eSargereiHopiliteEvents::EventVoidStrikes, 18 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_hopiliteAI(p_Creature);
    }
};

/// Sargerei Defender - 77042
class auchindoun_mob_sargerei_defender : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_defender() : CreatureScript("auchindoun_mob_sargerei_defender") { }

    struct auchindoun_mob_sargerei_defenderAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_defenderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
			m_False = true;
        }

        enum eSargereiDefenderSpells
        {
            SpellAvengersShield = 165715,
            SpellCrusaderStirke = 176931
        };

        enum eSargereiDefenderEvents
        {
            EventAvengersShield = 1,
            EventCrusaderStirke
        };

        InstanceScript* m_Instance;
		bool m_False;

        void Reset() override
        {
            events.Reset();

			if (m_False)
			{
				m_False = false;
			}
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eSargereiDefenderEvents::EventAvengersShield, urand(10 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eSargereiDefenderEvents::EventCrusaderStirke, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
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
                case eSargereiDefenderEvents::EventAvengersShield:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_FARTHEST, 0, 50.0f, true))
                            me->CastSpell(l_Random, eSargereiDefenderSpells::SpellAvengersShield);
                        events.ScheduleEvent(eSargereiDefenderEvents::EventAvengersShield, urand(10 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eSargereiDefenderEvents::EventCrusaderStirke:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eSargereiDefenderSpells::SpellCrusaderStirke);
                        events.ScheduleEvent(eSargereiDefenderEvents::EventCrusaderStirke, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_defenderAI(p_Creature);
    }
};

/// Sargerei Magus - 76263
class auchindoun_mob_sargerei_magus : public CreatureScript
{
    public:

    auchindoun_mob_sargerei_magus() : CreatureScript("auchindoun_mob_sargerei_magus") { }

    struct auchindoun_mob_sargerei_magusAI : public ScriptedAI
    {
        auchindoun_mob_sargerei_magusAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
			m_False = true;
            m_Instance = me->GetInstanceScript();
        }

        enum eSargereiMagusSpells
        {
            SpellArcaneChanneling = 161383,
            SpellArcaneBombDummy  = 157652,
            SpellArcaneBoltPeriod = 157505,
            SpellArcaneBoltProje  = 157931
        };

        enum eSargereiMagusEvents
        {
            EventArcaneBomb = 1,
            EventArcaneBolt
        };

        InstanceScript* m_Instance;
		bool m_False;
        std::list<uint64> l_Prisoners;

        void Reset() override
        {
			if (m_False)
			{
				m_False = false;
			}

            events.Reset();
            me->CastSpell(me, eSargereiMagusSpells::SpellArcaneChanneling);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eAuchindounMovementInforms::MovementInformFallMagusPrisoners:
                    if (!l_Prisoners.empty())
                    {
                        for (uint64 l_Itr : l_Prisoners)
                        {
                            if (!l_Itr)
                                continue;

                            if (Creature* l_Mob = Creature::GetCreature(*me, l_Itr))
                            {
                                l_Mob->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                l_Mob->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
                                l_Mob->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->RemoveAura(eSargereiMagusSpells::SpellArcaneChanneling);
            events.ScheduleEvent(eSargereiMagusEvents::EventArcaneBomb, 13 * TimeConstants::IN_MILLISECONDS);
			events.ScheduleEvent(eSargereiMagusEvents::EventArcaneBolt, 20 * TimeConstants::IN_MILLISECONDS);

            std::list<Creature*> l_mobsPrisoners;
            me->GetCreatureListWithEntryInGrid(l_mobsPrisoners, eAuchindounCreatures::CreatureAucheniSoulPriest, 20.0f);
            if (!l_mobsPrisoners.empty())
            {
                for (Creature* l_Itr : l_mobsPrisoners)
				{
					l_Itr->Kill(l_Itr);
                    l_Itr->RemoveAllAuras();
                    l_Itr->GetMotionMaster()->MoveFall(eAuchindounMovementInforms::MovementInformFallMagusPrisoners);			
					l_Itr->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                }
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
                case eSargereiMagusEvents::EventArcaneBomb:
                    me->CastSpell(me, eSargereiMagusSpells::SpellArcaneBombDummy);
                    events.ScheduleEvent(eSargereiMagusEvents::EventArcaneBomb, 13 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eSargereiMagusEvents::EventArcaneBolt:
                    me->CastSpell(me, eSargereiMagusSpells::SpellArcaneBoltPeriod);
					events.ScheduleEvent(eSargereiMagusEvents::EventArcaneBolt, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargerei_magusAI(p_Creature);
    }
};

/// Sargerei Soul Priest - 76595
class auchindoun_mob_soul_priest : public CreatureScript
{
    public:

    auchindoun_mob_soul_priest() : CreatureScript("auchindoun_mob_soul_priest") { }

    struct auchindoun_mob_soul_priestAI : public ScriptedAI
    {
        auchindoun_mob_soul_priestAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
			m_False = true;
        }

        enum eSoulPriestSpells
        {
            SpellShadowWordPainPriest = 176518,
            SpellPsychicTerrorDummy = 154356
        };

        enum eSoulPriestEvents
        {
            EventShadowWordPainSoulPriest = 1,
            EventPsychicTerrors
        };

        InstanceScript* m_Instance;
		bool m_False;

		void Reset() override
		{
			events.Reset();

			if (m_False)
			{
				m_False = false;
			}
		}

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eSoulPriestEvents::EventPsychicTerrors, 15 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSoulPriestEvents::EventShadowWordPainSoulPriest, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
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
                case eSoulPriestEvents::EventShadowWordPainSoulPriest:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true, -eAuchindounSpells::SpellShadowWordPainPriest))
                        me->CastSpell(me, eSoulPriestSpells::SpellShadowWordPainPriest);
                    events.ScheduleEvent(eSoulPriestEvents::EventShadowWordPainSoulPriest, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eSoulPriestEvents::EventPsychicTerrors:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eSoulPriestSpells::SpellPsychicTerrorDummy);
                    events.ScheduleEvent(eSoulPriestEvents::EventPsychicTerrors, 15 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_soul_priestAI(p_Creature);
    }
};

/// Sargerei Warden - 77935
class auchindoun_mob_sargeri_warden : public CreatureScript
{
    public:

    auchindoun_mob_sargeri_warden() : CreatureScript("auchindoun_mob_sargeri_warden") { }

    struct auchindoun_mob_sargeri_wardenAI : public ScriptedAI
    {
        auchindoun_mob_sargeri_wardenAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
			m_False = true;
        }

        enum eWardenSpells
        {
            SpellWardenThrowHammer = 154730,
            SpellWardenChainDot    = 154831
        };

        enum eWardenEvents
        {
            EventWardenHammer = 1,
            EventWardenChain
        };

        InstanceScript* m_Instance;
		bool m_False;

        void Reset() override
        {
            events.Reset();

			if (m_False)
			{
				m_False = false;
			}
		}

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eWardenEvents::EventWardenChain, 5 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eWardenEvents::EventWardenHammer, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
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
                case eWardenEvents::EventWardenHammer:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Random, eWardenSpells::SpellWardenThrowHammer);
                        events.ScheduleEvent(eWardenEvents::EventWardenHammer, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eWardenEvents::EventWardenChain:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Random, eWardenSpells::SpellWardenChainDot);
                        events.ScheduleEvent(eWardenEvents::EventWardenChain, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_sargeri_wardenAI(p_Creature);
    }
};

/// Felborne Abyssal - 77905
class auchindoun_mob_felborne_abyssal : public CreatureScript
{
    public:

    auchindoun_mob_felborne_abyssal() : CreatureScript("auchindoun_mob_felborne_abyssal") { }

    struct auchindoun_mob_felborne_abyssalAI : public ScriptedAI
    {
        auchindoun_mob_felborne_abyssalAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eFelborneAbyssalSpells
        {
            SpellFixate = 157168
        };

        enum eFelborneAbyssalEvents
        {
            EventFixate = 1
        };

        InstanceScript* m_Instance;
        bool m_Fixated;
        uint64 m_FixatedTargetGUID;

        void Reset() override
        {
            events.Reset();
            m_Fixated = false;  
            m_FixatedTargetGUID = NULL;
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);     
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eFelborneAbyssalEvents::EventFixate, urand(16 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eAuchindounActions::ActionDeactivateFixation:
                    m_Fixated = false;
                    break;
                default:
                    break;
            }
        }

        void OnAddThreat(Unit* p_Victim, float& p_fThreat, SpellSchoolMask /*p_SchoolMask*/, SpellInfo const /*p_ThreatSpell*/)  
        {
            if (m_Fixated)
                p_fThreat = 0;
            return;
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            if (m_Fixated)
            {
                if (m_FixatedTargetGUID)
                {
                    if (Unit* l_Target = Unit::GetUnit(*me, m_FixatedTargetGUID))
                        me->AddThreat(l_Target, 500.0f);
                }
            }

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindounEvents::EventFixate:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        m_Fixated = true;
                        m_FixatedTargetGUID = l_Random->GetGUID();
                        me->CastSpell(l_Random, eFelborneAbyssalSpells::SpellFixate);
                    }

                    events.ScheduleEvent(eFelborneAbyssalEvents::EventFixate, urand(16 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));                
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };
    
    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_felborne_abyssalAI(p_Creature);
    }
};

/// Cackling Pyrmoaniac - 76260
class auchindoun_mob_cackling_pyromaniac : public CreatureScript
{
    public:

    auchindoun_mob_cackling_pyromaniac() : CreatureScript("auchindoun_mob_cackling_pyromaniac") { }

    struct auchindoun_mob_cackling_pyromaniacAI : public ScriptedAI
    {
        auchindoun_mob_cackling_pyromaniacAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eCacklingPyromaniacSpells
        {
            SpellFelBlast      = 167092,
            SpellAbyssalVisual = 159610
        };

        enum eCacklingPyromaniacEvents
        {
            EventFelBlast = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->CastSpell(me, eCacklingPyromaniacSpells::SpellAbyssalVisual);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eCacklingPyromaniacEvents::EventFelBlast, 6 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->isAlive() && l_Azzakel->IsAIEnabled)
                        l_Azzakel->GetAI()->DoAction(eAuchindounActions::ActionDemonSoulsAchievement);

                if (Creature* l_Trigger = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataTriggerAzzakelController)))
                    if (l_Trigger->IsWithinDistInMap(me, 30.0f) && l_Trigger->IsAIEnabled)
                        l_Trigger->AI()->DoAction(eAuchindounActions::ActionCountPre3StBossKill);
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
                case eCacklingPyromaniacEvents::EventFelBlast:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eCacklingPyromaniacSpells::SpellFelBlast);
                    events.ScheduleEvent(eCacklingPyromaniacEvents::EventFelBlast, 6 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_cackling_pyromaniacAI(p_Creature);
    }
};

/// Blazing Trickster - 79511
class auchindoun_mob_blazing_trickster : public CreatureScript
{
    public:

    auchindoun_mob_blazing_trickster() : CreatureScript("auchindoun_mob_blazing_trickster") { }

    struct auchindoun_mob_blazing_tricksterAI : public ScriptedAI
    {
        auchindoun_mob_blazing_tricksterAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eBlazingTricksterSpells
        {
            SpellConfligirate = 154981
        };

        enum eBlazingTricksterEvents
        {
            EventConfligrate = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            me->SetDefaultMovementType(MovementGeneratorType::RANDOM_MOTION_TYPE);
            events.ScheduleEvent(eBlazingTricksterEvents::EventConfligrate, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->isAlive() && l_Azzakel->IsAIEnabled)
                        l_Azzakel->GetAI()->DoAction(eAuchindounActions::ActionDemonSoulsAchievement);
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
                case eBlazingTricksterEvents::EventConfligrate:
                        me->CastSpell(me, eBlazingTricksterSpells::SpellConfligirate);
                        events.ScheduleEvent(eBlazingTricksterEvents::EventConfligrate, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_blazing_tricksterAI(p_Creature);
    }
};

/// Felguard - 76259
class auchindoun_mob_felguard : public CreatureScript
{
    public:

    auchindoun_mob_felguard() : CreatureScript("auchindoun_mob_felguard") { }

    struct auchindoun_mob_felguardAI : public ScriptedAI
    {
        auchindoun_mob_felguardAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eFelguardSpells
        {
            SpellFelStomp = 157173
        };

        enum eFelguardEvents
        {
            EventFelStomp = 1
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eFelguardEvents::EventFelStomp, 10 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->isAlive() && l_Azzakel->IsAIEnabled)
                        l_Azzakel->GetAI()->DoAction(eAuchindounActions::ActionDemonSoulsAchievement);

                if (Creature* l_Trigger = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataTriggerAzzakelController)))
                    if (l_Trigger->IsWithinDistInMap(me, 30.0f) && l_Trigger->IsAIEnabled)
                        l_Trigger->AI()->DoAction(eAuchindounActions::ActionCountPre3StBossKill);
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
                case eFelguardEvents::EventFelStomp:
                    me->CastSpell(me, eFelguardSpells::SpellFelStomp);
                    events.ScheduleEvent(eFelguardEvents::EventFelStomp, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_felguardAI(p_Creature);
    }
};

/// Warden Hammer - 76655
class auchindoun_mob_warden_hammer : public CreatureScript
{
    public:

    auchindoun_mob_warden_hammer() : CreatureScript("auchindoun_mob_warden_hammer") { }

    struct auchindoun_mob_warden_hammerAI : public ScriptedAI
    {
        auchindoun_mob_warden_hammerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eWardenHammerSpells
        {
            SpellWardenHammerLightningVisual = 154775,
            SpellWardenHammerDamage          = 154773
        };

        InstanceScript* m_Instance;
        uint32 m_DiffHammer;

        void Reset() override
        {
            me->setFaction(HostileFaction);
            m_DiffHammer = 1 * TimeConstants::IN_MILLISECONDS;
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);         
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (m_DiffHammer <= p_Diff)
            {
                std::list<Player*> l_ListNerbyPlayers;
                me->GetPlayerListInGrid(l_ListNerbyPlayers, 3.0f);
                if (!l_ListNerbyPlayers.empty())
                {
                    for (Player* l_Itr : l_ListNerbyPlayers)
                    {
                        if (!l_Itr)
                            continue;

                        if (l_Itr && l_Itr->IsInWorld())
                        {
                            me->CastSpell(l_Itr, eWardenHammerSpells::SpellWardenHammerLightningVisual);
                            me->CastSpell(l_Itr, eWardenHammerSpells::SpellWardenHammerDamage);
                        }
                    }
                }

                m_DiffHammer = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_DiffHammer -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_mob_warden_hammerAI(p_Creature);
    }
};

/// Void Mending - 154623 
class auchindoun_spell_void_mending : public SpellScriptLoader
{
    public:

    auchindoun_spell_void_mending() : SpellScriptLoader("auchindoun_spell_void_mending") { }

    class auchindoun_spell_void_mending_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_void_mending_SpellScript);

        void HandleDummy(SpellEffIndex /*p_EffIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                    l_Caster->AddAura(eAuchindounSpells::SpellVoidMendingAura, l_Target);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindoun_spell_void_mending_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_void_mending_SpellScript();
    }
};

/// Psychic Terrors - 154356  
class auchindoun_spell_psychic_terror : public SpellScriptLoader
{
    public:

    auchindoun_spell_psychic_terror() : SpellScriptLoader("auchindoun_spell_psychic_terror") { }

    class auchindoun_spell_psychic_terror_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_psychic_terror_SpellScript);

        void HandleDummy(SpellEffIndex /*p_EffIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    std::list<Player*> l_ListPlayers;
                    l_Caster->GetPlayerListInGrid(l_ListPlayers, 4.0f);
                    if (!l_ListPlayers.empty())
                    {
                        for (Player* l_Itr : l_ListPlayers)
                        {
                            l_Itr->AddAura(eAuchindounSpells::SpellPsychicTerrorFear, l_Itr);
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindoun_spell_psychic_terror_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_psychic_terror_SpellScript();
    }
};

/// Warden's Chain - 154683 
class auchindoun_spell_warden_chain : public SpellScriptLoader
{
    public:

    auchindoun_spell_warden_chain() : SpellScriptLoader("auchindoun_spell_warden_chain") { }

    class auchindoun_spell_warden_chain_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_warden_chain_SpellScript);

        enum eWardenChainSpells
        {
            SpellWardenChainJump = 154639,
            SpellWardenChainDot  = 154831
        };

        void HandleDummy(SpellEffIndex /*p_EffIndex*/) 
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    l_Caster->AddAura(eWardenChainSpells::SpellWardenChainJump, l_Target);
                    l_Caster->AddAura(eWardenChainSpells::SpellWardenChainDot, l_Target);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindoun_spell_warden_chain_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_warden_chain_SpellScript();
    }
};

/// Warden Chain Aura - 154831 
class auchindoun_warden_chain_aura : public SpellScriptLoader
{
    public:

    auchindoun_warden_chain_aura() : SpellScriptLoader("auchindoun_warden_chain_aura") { }

    class auchindoun_warden_chain_aura_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_warden_chain_aura_AuraScript);

        enum eWardenChainAuras
        {
            SpellWardenChainRoot = 154263
        };

        void HandlePeriodic(AuraEffect const* p_AurEff)
        {
            if (Unit* l_Target = GetTarget())
                l_Target->AddAura(eWardenChainAuras::SpellWardenChainRoot, l_Target);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_warden_chain_aura_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_warden_chain_aura_AuraScript();
    }
};

/// Void Shift - 155524
class auchindoun_spell_void_shift : public SpellScriptLoader
{
    public:

    auchindoun_spell_void_shift() : SpellScriptLoader("auchindoun_spell_void_shift") { }

    class auchindoun_spell_void_shift_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_void_shift_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            Unit* l_Caster = GetCaster();  
            int32 l_CalcDamage = 8000;

            std::list<Unit*> l_ListPlayers;
            JadeCore::AnyUnitInObjectRangeCheck check(GetCaster(), 30.0f);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(l_Caster, l_ListPlayers, check);
            l_Caster->VisitNearbyObject(30.0f, searcher);
            if (!l_ListPlayers.empty())
            {
                for (std::list<Unit*>::const_iterator l_It = l_ListPlayers.begin(); l_It != l_ListPlayers.end(); ++l_It)
                {
                    if (!(*l_It))
                        continue;

                    if ((*l_It)->GetTypeId() == TypeID::TYPEID_PLAYER)
                        GetCaster()->CastCustomSpell((*l_It), eAuchindounSpells::SpellVoidShiftDamage, &l_CalcDamage, NULL, NULL, true, NULL);
                    else
                        GetCaster()->CastCustomSpell((*l_It), eAuchindounSpells::SpellVoidShiftHeal,   &l_CalcDamage, NULL, NULL, true, NULL);
                }
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_spell_void_shift_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_void_shift_SpellScript();
    }
};

/// Void Shell - 160312
class auchindoun_spell_void_shell_filter : public SpellScriptLoader
{
    public:

    auchindoun_spell_void_shell_filter() : SpellScriptLoader("auchindoun_spell_void_shell_filter") { }

    class auchindoun_spell_void_shell_filter_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_void_shell_filter_SpellScript);

        void CorrectTargets(std::list<WorldObject*>& p_Targets)
        {
            p_Targets.clear();

            if (!GetCaster())
                return;

            Unit* l_Caster = GetCaster();
    
            std::list<Unit*> l_TargetList;
            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(l_Caster, l_Caster, 10.0f);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(l_Caster, l_TargetList, u_check);
            l_Caster->VisitNearbyObject(10.0f, searcher);
            if (!l_TargetList.empty())
            {
                for (Unit* l_Itr : l_TargetList)
                {
                    if (!l_Itr)
                        continue;

                    if (l_Itr->GetTypeId() == TypeID::TYPEID_PLAYER)
                        continue;

                    if (l_Itr && l_Itr->IsInWorld())
                        p_Targets.push_back(l_Itr);
                }
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(auchindoun_spell_void_shell_filter_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_SRC_AREA_ALLY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_spell_void_shell_filter_SpellScript();
    }
};

/// Tuulani Unlock Gate - 160415
class auchindoun_spell_tuulani_unlock : public SpellScriptLoader
{
    public:

    auchindoun_spell_tuulani_unlock() : SpellScriptLoader("auchindoun_spell_tuulani_unlock") { }

    class auchindoun_spell_tuulani_unlock_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_spell_tuulani_unlock_SpellScript);

        void CorrectTargets(std::list<WorldObject*>& p_Targets)
        {
            /// Clears all targets at start, fetching new ones
            p_Targets.clear();
            std::list<Creature*> l_ListTriggerWall;
            GetCaster()->GetCreatureListWithEntryInGrid(l_ListTriggerWall, eAuchindounCreatures::CreatureLightWallTargets, 15.0f);
            if (!l_ListTriggerWall.empty())
            {
                for (Creature* l_Itr : l_ListTriggerWall)
                {
                    if (l_Itr && l_Itr->IsInWorld())
                        p_Targets.push_back(l_Itr->ToUnit());
                }
            }
        }

        void HandleAfterCast()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (GameObject * l_GameObject = l_Caster->FindNearestGameObject(eAuchindounObjects::GameobjectHolyBarrierEntra, 5.0f))
                {
                    l_GameObject->Delete();
                }
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(auchindoun_spell_tuulani_unlock_SpellScript::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_DEST_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindoun_spell_tuulani_unlock_SpellScript();
    }
};

/// Arcane Bolt - 157505 
class auchindoun_spell_arcane_bolt : public SpellScriptLoader
{
public:

	auchindoun_spell_arcane_bolt() : SpellScriptLoader("auchindoun_spell_arcane_bolt") { }

	class auchindoun_spell_arcane_bolt_AuraScript : public AuraScript
    {
		PrepareAuraScript(auchindoun_spell_arcane_bolt_AuraScript);

        enum eArcaneBoltSpells
        {
            SpellArcaneBoltPeriod = 157505,
            SpellArcaneBoltProje = 157931
        };

        void HandlePeriodic(AuraEffect const* p_AurEff)
        {
            if (Unit* l_Caster = GetCaster())
            {   
                if (l_Caster->IsAIEnabled)
                {
                    if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        l_Caster->CastSpell(l_Target, eArcaneBoltSpells::SpellArcaneBoltProje, true);
                }
            }
        }        

        void Register()
        {
			OnEffectPeriodic += AuraEffectPeriodicFn(auchindoun_spell_arcane_bolt_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
		return new auchindoun_spell_arcane_bolt_AuraScript();
    }
};

/// Arcane Bomb - 157793 
class auchindoun_at_arcane_bomb : public AreaTriggerEntityScript
{
public:

    auchindoun_at_arcane_bomb() : AreaTriggerEntityScript("auchindoun_at_arcane_bomb")
    {
    }

    enum eArcaneBombSpells
    {
        SpellArcaneBombDamage = 157792
    };

    bool m_Used = false;
    uint32 m_Diff = 1 * TimeConstants::IN_MILLISECONDS;
    std::list<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Diff <= p_Time)
        {
            if (!m_Used)
            {
                if (Player* l_Nearest = p_AreaTrigger->FindNearestPlayer(1.0f, true))
                {
                    m_Used = true;
                    p_AreaTrigger->Remove(1);
                    p_AreaTrigger->CastSpell(l_Nearest, eArcaneBombSpells::SpellArcaneBombDamage);
                }
            }

            m_Diff = 2 * TimeConstants::IN_MILLISECONDS;
        }
        else
            m_Diff -= p_Time;
    }

    auchindoun_at_arcane_bomb* GetAI() const override
    {
        return new auchindoun_at_arcane_bomb();
    }
};

/// Talador Portal - 236689
class auchindoun_gob_talador_portal : public GameObjectScript
{
    public:

    auchindoun_gob_talador_portal() : GameObjectScript("auchindoun_gob_talador_portal") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject) override
    {
        p_Player->TeleportTo(1116, 1488.52f, 3077.65f, 108.920f, 4.653427f);
        return true;
    }
};

void AddSC_auchindoun()
{
    new auchindoun_mob_tuulani();                   ///< 79248
    new auchindoun_mob_sargerei_soulbinder();       ///< 77812  
    new auchindoun_mob_sargerei_cleric();           ///< 77134
    new auchindoun_mob_sargerei_ritualist();        ///< 77130
    new auchindoun_mob_sargerei_zealot();           ///< 77132
    new auchindoun_mob_sargerei_spirit_tender();    ///< 77131
    new auchindoun_mob_sargerei_hopilite();         ///< 77133
    new auchindoun_mob_sargeri_warden();            ///< 77935
    new auchindoun_mob_sargerei_magus();            ///< 76263
    new auchindoun_mob_sargerei_defender();         ///< 77042
    new auchindoun_mob_felborne_abyssal();          ///< 77905
    new auchindoun_mob_soul_priest();               ///< 76595
    new auchindoun_mob_felguard();                  ///< 76259
    new auchindoun_mob_cackling_pyromaniac();       ///< 76260
    new auchindoun_mob_blazing_trickster();         ///< 79511
    new auchindoun_mob_warden_hammer();             ///< 76655
    new auchindoun_spell_void_shift();              ///< 155524
    new auchindoun_spell_void_mending();            ///< 154623
    new auchindoun_spell_void_shell_filter();       ///< 160312
    new auchindoun_spell_psychic_terror();          ///< 154356
    new auchindoun_spell_tuulani_unlock();          ///< 160415
    new auchindoun_spell_arcane_bolt();             ///< 157505
    new auchindoun_at_arcane_bomb();                ///< 157793
    new auchindoun_gob_talador_portal();            ///< 236689
}