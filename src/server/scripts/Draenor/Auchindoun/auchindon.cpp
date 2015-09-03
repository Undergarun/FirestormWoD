
#include "ScriptedCreature.h"
#include "auchindon.hpp"

/// 1st STarting Event
class tuulani_1st_event : public BasicEvent
{
public:
    explicit tuulani_1st_event(Unit* unit, int value) : m_Obj(unit), m_Modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Tuulina = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataTuulani)))
                {
                    if (Creature* l_Nyami = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataNyami)))
                    {
                        if (l_Nyami->GetAI() && l_Tuulina->GetAI())
                        {
                            switch (m_Modifier)
                            {
                                case 0: // TUULINA         
                                {
                                    l_Nyami->AddAura(eAuchindonSpells::SpellDarkFire, l_Nyami);
                                    l_Nyami->AddAura(eAuchindonSpells::SpellLevitateNyami, l_Nyami);
                                    l_Nyami->SetReactState(ReactStates::REACT_PASSIVE);
                                    l_Nyami->CastSpell(l_Nyami, eAuchindonSpells::SpellPrisonAura);

                                    l_Tuulina->GetAI()->DoAction(eAuchindonGeneralActions::ActionConfirmTuulaniEvent);

                                    if (Creature* l_Trigger = l_Nyami->FindNearestCreature(100055, 40.0f, true))
                                    {
                                        l_Trigger->AddAura(eAuchindonSpells::SpellVoidFormTriggerBuff, l_Trigger);
                                        l_Nyami->CastSpell(l_Trigger, eAuchindonSpells::SpellShadowBeam);
                                    }

                                    Creature* l_Guard = m_Obj->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, l_PositionGuardPositionTalker, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                                    if (l_Guard)
                                        l_Guard->CastSpell(l_Guard, eAuchindonSpells::SpellKneel);

                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK2);
                                    l_Tuulina->GetMotionMaster()->MovePoint(1, l_PositionTuulaniPreMovement02.GetPositionX(), l_PositionTuulaniPreMovement02.GetPositionY(), l_PositionTuulaniPreMovement02.GetPositionZ());
                                    break;
                                }
                                case 1: // TUULINA
                                {
                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK3);
                                    l_Tuulina->GetMotionMaster()->MovePoint(2, l_PositionTuulaniPreMovement03.GetPositionX(), l_PositionTuulaniPreMovement03.GetPositionY(), l_PositionTuulaniPreMovement03.GetPositionZ());
                                    break;
                                }
                                case 3: // GUARD
                                {
                                    Creature* l_Guard = m_Obj->ToCreature();
                                    if (l_Guard && l_Guard->GetAI())
                                    {
                                        l_Guard->RemoveAura(eAuchindonSpells::SpellKneel);
                                        l_Guard->AI()->Talk(eAuchindonTalk::AUCHENAIDEFENDERTALK1);
                                        l_Guard->SetFacingToObject(l_Tuulina);

                                        l_Guard->m_Events.AddEvent(new tuulani_1st_event(l_Guard, 4), l_Guard->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                }
                                case 4: // TUULANI
                                {
                                    l_Tuulina->GetMotionMaster()->MovePoint(3, l_PositionTuulaniMovement000.GetPositionX(), l_PositionTuulaniMovement000.GetPositionY(), l_PositionTuulaniMovement000.GetPositionZ());
                                    break;
                                }
                                case 5: // TUULANI                    
                                {
                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK4);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 6), l_Tuulina->m_Events.CalculateTime(4 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 6:
                                {
                                    l_Tuulina->CastSpell(l_Tuulina, eAuchindonSpells::SpellTuulaniUnlock);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 7), l_Tuulina->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 7:
                                {
                                    if (GameObject* nearestdoor = l_Tuulina->FindNearestGameObject(, 60.0f))
                                    {
                                        nearestdoor->Delete();
                                    }

                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK2);
                                    l_Tuulina->GetMotionMaster()->MovePoint(4, l_PositionTuulaniMovement04.GetPositionX(), l_PositionTuulaniMovement04.GetPositionY(), l_PositionTuulaniMovement04.GetPositionZ());
                                    break;
                                }
                                case 8:
                                {
                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK5);
                                    l_Tuulina->GetMotionMaster()->MovePoint(5, l_PositionTuulaniMovement05.GetPositionX(), l_PositionTuulaniMovement05.GetPositionY(), l_PositionTuulaniMovement05.GetPositionZ());
                                    break;
                                }
                                case 9:
                                {
                                    l_Tuulina->GetMotionMaster()->MovePoint(6, l_PositionTuulaniMovement06.GetPositionX(), l_PositionTuulaniMovement06.GetPositionY(), l_PositionTuulaniMovement06.GetPositionZ());
                                    break;
                                }
                                case 10:
                                {
                                    l_Tuulina->GetMotionMaster()->MovePoint(7, l_PositionTuulaniMovement07.GetPositionX(), l_PositionTuulaniMovement07.GetPositionY(), l_PositionTuulaniMovement07.GetPositionZ());
                                    break;
                                }
                                case 11:
                                {
                                    l_Tuulina->GetMotionMaster()->MovePoint(8, l_PositionTuulaniMovement08.GetPositionX(), l_PositionTuulaniMovement08.GetPositionY(), l_PositionTuulaniMovement08.GetPositionZ());
                                    break;
                                }
                                case 12:
                                {
                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK6);
                                    l_Tuulina->GetMotionMaster()->MovePoint(9, l_PositionTuulaniPreMovement03.GetPositionX(), l_PositionTuulaniMovement09.GetPositionY(), l_PositionTuulaniMovement09.GetPositionZ());
                                    break;
                                }
                                case 13:
                                {
                                    l_Tuulina->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                    l_Tuulina->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                    l_Tuulina->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                    l_Tuulina->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK7);

                                    l_Tuulina->GetMotionMaster()->MovePoint(10, l_PositionTuulaniMovement10th.GetPositionX(), l_PositionTuulaniMovement10th.GetPositionY(), l_PositionTuulaniMovement10th.GetPositionZ());
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 15), l_Tuulina->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 15:
                                {
                                    l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK1);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 16), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 16:
                                {
                                    l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK2);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 17), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 17:
                                {
                                    l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK3);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 18), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 18:
                                {
                                    l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK4);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 19), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 19:
                                {
                                    l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK8);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 20), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 20:
                                {
                                    l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK5);
                                    l_Tuulina->m_Events.AddEvent(new tuulani_1st_event(l_Tuulina, 21), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                            }
                        }
                        return true;
                    }
                }
            }
        }
    }

private:
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};

/// Tuulani - 
class auchindon_creature_tuulani : public CreatureScript
{
public:
    auchindon_creature_tuulani() : CreatureScript("auchindon_creature_tuulani") {}

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript(); 

            // Handles starting event.
            m_First = true;
        }

        InstanceScript* m_Instance;
        bool m_StartEvent;
        bool m_First;

        void Reset() override
        {       
            events.Reset();
        }

        void HandleTuulaniAtSpawn()
        {
            if (m_First)
            {
                m_First = false;

                me->setFaction(35);
                Talk(eAuchindonTalk::TUULANITALK1);
                me->SetSpeed(UnitMoveType::MOVE_RUN, 1.5f, true);
                me->SetSpeed(UnitMoveType::MOVE_WALK, 1.5f, true);
                me->GetMotionMaster()->MovePoint(0, l_PositionTuulaniPreMovement01.GetPositionX(), l_PositionTuulaniPreMovement01.GetPositionY(), l_PositionTuulaniPreMovement01.GetPositionZ());

                events.ScheduleEvent(EventBeginRp, 4 * TimeConstants::IN_MILLISECONDS);
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
            case eAuchindonGeneralActions::ActionConfirmTuulaniEvent:
                m_StartEvent = true;
                break;
            }
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eAuchindonMovementInform::MovementInformTuulani01:
                       me->m_Events.AddEvent(new tuulani_1st_event(me, 0), me->m_Events.CalculateTime(200));
                       break;
                case eAuchindonMovementInform::MovementInformTuulani02:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 1), me->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani03:
                        if (Creature* l_Guard = me->FindNearestCreature(eAuchindonCreatures::CreatureAucheniDefender, 10.0F, true))
                            l_Guard->m_Events.AddEvent(new tuulani_1st_event(l_Guard, 3), l_Guard->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani04:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 5), me->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani05:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 8), me->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani06:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 9), me->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani07:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 10), me->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani08:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 11), me->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani09:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 12), me->m_Events.CalculateTime(200));
                        break;
                case eAuchindonMovementInform::MovementInformTuulani10:
                        me->m_Events.AddEvent(new tuulani_1st_event(me, 13), me->m_Events.CalculateTime(200));
                        break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!m_StartEvent)
            {
                me->m_Events.AddEvent(new tuulani_1st_event(me, 0), me->m_Events.CalculateTime(0));
            }

            switch (events.ExecuteEvent())
            {
            case EventBeginRp:
                    m_StartEvent = false;
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

/// Sargerei Soulbinder
class auchindon_creature_sargerei_soulbinder : public CreatureScript
{
public:
    auchindon_creature_sargerei_soulbinder() : CreatureScript("auchindon_creature_sargerei_soulbinder") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eAuchindonEvents::EventMindShear, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAuchindonEvents::EventBendWill, 18 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override 
        {
            if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossKathaar)))
            {
                if (l_Kaathar->isAlive() && l_Kaathar->GetAI())
                    l_Kaathar->AI()->DoAction(eAuchindonGeneralActions::ActionCountPre1StBossKill);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindonEvents::EventMindShear:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eAuchindonSpells::SpellVoidShell);

                    events.ScheduleEvent(eAuchindonEvents::EventBendWill, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eAuchindonEvents::EventBendWill:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Target, eAuchindonSpells::SpellBendWill);

                    events.ScheduleEvent(eAuchindonEvents::EventBendWill, 18 * TimeConstants::IN_MILLISECONDS);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creatures(p_Creature);
    }
};

/// Sargerei Cleric - 
class auchindon_creature_sargerei_cleric : public CreatureScript
{
public:
    auchindon_creature_sargerei_cleric() : CreatureScript("auchindon_creature_sargerei_cleric") { }

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {   
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eAuchindonEvents::EventVoidShell, 15 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        { 
            if (Creature* Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossKathaar)))
            {
                if (Kaathar->isAlive() && Kaathar->GetAI())
                    Kaathar->AI()->DoAction(eDataAuchindonDatas::DataBossKathaar);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindonEvents::EventVoidShell:
                        me->CastSpell(me, eAuchindonSpells::SpellVoidShell);
                        events.ScheduleEvent(eAuchindonEvents::EventVoidShell, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                        break;
            }

            DoMeleeAttackIfReady();
        }
    }; 

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

/// Sargerei Ritualist - 
class auchindon_creature_sargerei_ritualist : public CreatureScript
{
public:
    auchindon_creature_sargerei_ritualist() : CreatureScript("auchindon_creature_sargerei_ritualist") { }

    struct auchindon_creaturesAI : public Scripted_NoMovementAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->AddAura(eAuchindonSpells::SpellDarkFire, me);

            if (Creature* l_Trigger = me->SummonCreature(100055, l_PositionKaatharCrystalPosition, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
            {
                me->CastSpell(l_Trigger, eAuchindonSpells::SpellShadowBeam); // bind triggers      
            }
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->RemoveAllAuras();

            events.ScheduleEvent(eAuchindonEvents::EventMindSpike, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (Creature* Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossKathaar)))
            {
                if (Kaathar->isAlive() && Kaathar->GetAI())
                    Kaathar->AI()->DoAction(eAuchindonGeneralActions::ActionCountPre1StBossKill);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindonEvents::EventMindSpike:
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(l_Target, eAuchindonSpells::SpellMindSpike);
                    }

                    events.ScheduleEvent(eAuchindonEvents::EventMindSpike, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

/// Sargerei Zealot - 
class auchindon_creature_sargerei_zealot : public CreatureScript
{
public:
    auchindon_creature_sargerei_zealot() : CreatureScript("auchindon_creature_sargerei_zealot") { }

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eAuchindonEvents::Event_Sever_Tendom, 5 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (Creature* Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::Data_Boss_Kathaar)))
            {
                if (Kaathar->isAlive())
                    Kaathar->AI()->DoAction(eAuchindonGeneralActions::Action_Count_Pre_1St_Boss_Kill);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        { 
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindonEvents::Event_Sever_Tendom:
                        if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(target, 157165); // sever tendon

                        events.ScheduleEvent(eAuchindonEvents::Event_Sever_Tendom, urand(11 * TimeConstants::IN_MILLISECONDS, 17 * TimeConstants::IN_MILLISECONDS));
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

/// Sargerei Spirit Tender -
class auchindon_creature_sargerei_spirit_tender : public CreatureScript
{
public:
    auchindon_creature_sargerei_spirit_tender() : CreatureScript("auchindon_creature_sargerei_spirit_tender") { }

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eAuchindonEvents::Event_Void_Mending, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAuchindonEvents::Event_Void_Shift, 16 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (Creature* l_Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::Data_Boss_Kathaar)))
            {
                if (l_Kaathar->isAlive() && l_Kaathar->GetAI())
                    l_Kaathar->AI()->DoAction(eAuchindonGeneralActions::Action_Count_Pre_1St_Boss_Kill);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindonEvents::Event_Void_Mending:
                        if (Unit* friendUnit = DoSelectLowestHpFriendly(85)) // heal
                            me->CastSpell(friendUnit, eAuchindonSpells::Spell_Void_Mending_Dummy);

                        events.ScheduleEvent(eAuchindonEvents::Event_Void_Mending, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                case eAuchindonEvents::Event_Void_Shift:
                        me->CastSpell(me, eAuchindonSpells::Spell_Void_Shift_Dummy);
                        events.ScheduleEvent(eAuchindonEvents::Event_Void_Shift, 16 * TimeConstants::IN_MILLISECONDS);
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    };
};

/// Sargerei Hopilite - 
class auchindon_creature_sargerei_hopilite : public CreatureScript
{
public:
    auchindon_creature_sargerei_hopilite() : CreatureScript("auchindon_creature_sargerei_hopilite") { }

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eAuchindonEvents::Event_Shield_Bash, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eAuchindonEvents::Event_Void_Strikes, 18 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (Creature* Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::Data_Boss_Kathaar)))
            {
                if (Kaathar->isAlive() && Kaathar->GetAI())
                Kaathar->AI()->DoAction(eAuchindonGeneralActions::Action_Count_Pre_1St_Boss_Kill);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindonEvents::Event_Shield_Bash:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eAuchindonSpells::Spell_Shield_Bash);
     
                    events.ScheduleEvent(eAuchindonEvents::Event_Shield_Bash, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eAuchindonEvents::Event_Void_Strikes:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eAuchindonSpells::Spell_Void_Strikes);

                        events.ScheduleEvent(eAuchindonEvents::Event_Void_Strikes, 18 * TimeConstants::IN_MILLISECONDS);
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

/// Sargerei Defender - 
class auchindon_creature_sargerei_defender : public CreatureScript
{
public:
    auchindon_creature_sargerei_defender() : CreatureScript("auchindon_creature_sargerei_defender") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(EVENT_AVENGERS_SHIELD, urand(10 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_CRUSADER_STIRKE, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_AVENGERS_SHIELD:
                    if (Unit* random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(random, SPELL_AVENGERS_SHIELD);
                        events.ScheduleEvent(EVENT_AVENGERS_SHIELD, urand(10 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
                case EVENT_CRUSADER_STIRKE:
                    if (Unit* random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(random, SPELL_CRUSADER_STIRKE);
                        events.ScheduleEvent(EVENT_CRUSADER_STIRKE, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creatures(p_Creature);
    }
};

/// Sargerei Magus - 
class auchindon_creature_sargerei_magus : public CreatureScript
{
public:
    auchindon_creature_sargerei_magus() : CreatureScript("auchindon_creature_sargerei_magus") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(EventArcaneBomb, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));

            std::list<Creature*> l_CreaturesPrisoners;
            me->GetCreatureListWithEntryInGrid(l_CreaturesPrisoners, CreatureAucheniSoulPriest, 20.0f);

            if (l_CreaturesPrisoners.empty())
                return;

            for (auto itr : l_CreaturesPrisoners)
            {
                itr->RemoveAllAuras();
                itr->SetCanFly(false);
                itr->DespawnOrUnsummon(2 * TimeConstants::IN_MILLISECONDS);

                itr->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
                itr->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                itr->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;
     
            switch (events.ExecuteEvent())
            {
                case EventArcaneBomb:
                    me->CastSpell(me, SpellArcaneBombDummy);
                    events.ScheduleEvent(EventArcaneBomb, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creatures(p_Creature);
    }
};

/// Sargerei Warden - 
class auchindon_creature_sargeri_warden : public CreatureScript
{
public:
    auchindon_creature_sargeri_warden() : CreatureScript("auchindon_creature_sargeri_warden") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(EventWardenHammer, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(EventWardenChain, 5 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            switch (events.ExecuteEvent())
            {
                case EventWardenHammer:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(l_Random, SpellWardenThrowHammer);
                        events.ScheduleEvent(EventWardenHammer, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                case EventWardenChain:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(l_Random, 154831);
                        l_Random->CastSpell(me, 154639);
                        l_Random->AddAura(154263, l_Random);
                    }
                    events.ScheduleEvent(EventWardenChain, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creatures(p_Creature);
    }
};

// Felborne Abyssal
class auchindon_creature_felborne_abyssal : public CreatureScript
{
public:
    auchindon_creature_felborne_abyssal() : CreatureScript("auchindon_creature_felborne_abyssal") { }

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        bool m_Fixated;

        void Reset() override
        {
            m_Fixated = false;
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(EventFixate, urand(16 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
        }

        void DoAction(int32 const action) override
        {
            switch (action)
            {
                case ActionDeactivateFixation:
                    m_Fixated = false;
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
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            switch (events.ExecuteEvent())
            {
            case EventFixate:
                if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                {
                    me->CastSpell(l_Random, SpellFixate);
                    m_Fixated = true;

                    events.ScheduleEvent(EventFixate, urand(16 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                }
                break;
            }
            DoMeleeAttackIfReady();
        }
    };
    
    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

/// Cackling Pyrmoaniac - 
class auchindon_creature_cackling_pyromaniac : public CreatureScript
{
public:
    auchindon_creature_cackling_pyromaniac() : CreatureScript("auchindon_creature_cackling_pyromaniac") { }

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->CastSpell(me, SpellAbyssalVisual);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(EventFelblast, 10 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (Creature* Trigger = m_Instance->instance->GetCreature(m_Instance->GetData64(DataTriggerAzzakelController)))
                if (Trigger->IsWithinDistInMap(me, 30.0f))
                    Trigger->AI()->DoAction(ActionCountPre3StBossKill);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            switch (events.ExecuteEvent())
            {
            case EventFelblast:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, SpellFelblast);

                    events.ScheduleEvent(EventFelblast, 10 * TimeConstants::IN_MILLISECONDS);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creaturesAI(p_Creature);
    }
};

/// Blazing Pyrmoaniac - 
class auchindon_creature_blazing_pyromaniac : public CreatureScript
{
public:
    auchindon_creature_blazing_pyromaniac() : CreatureScript("auchindon_creature_blazing_pyromaniac") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.ScheduleEvent(EVENT_RANDOM_MOVE, 2 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CONFLIGRATE, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_RANDOM_MOVE:
                    me->GetMotionMaster()->MoveRandom(3.5f);
                    events.ScheduleEvent(EVENT_RANDOM_MOVE, 3 * TimeConstants::IN_MILLISECONDS);
                    break;
                case EVENT_CONFLIGRATE:
                    me->CastSpell(me, SPELL_CONFLIGIRATE);
                    events.ScheduleEvent(EVENT_CONFLIGRATE, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                    break;
            }

            DoMeleeAttackIfReady();
        }

    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creatures(p_Creature);
    }
};

/// Felguard - 
class auchindon_creature_felguard : public CreatureScript
{
public:
    auchindon_creature_felguard() : CreatureScript("auchindon_creature_felguard") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eAuchindonEvents::EventFelStomp, 16 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (Creature* Trigger = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataTriggerAzzakelController)))
                    if (Trigger->IsWithinDistInMap(me, 30.0f))
                        Trigger->AI()->DoAction(eAuchindonGeneralActions::ActionCountPre3StBossKill);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            switch (events.ExecuteEvent())
            {
                case eAuchindonEvents::EventFelStomp:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(l_Random, eAuchindonSpells::SpellFelStomp);
                            events.ScheduleEvent(eAuchindonEvents::EventFelStomp, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                        }
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindon_creatures(p_Creature);
    }
};

/// Void Mending - 
class auchindon_spell_void_mending : public SpellScriptLoader
{
public:
    auchindon_spell_void_mending() : SpellScriptLoader("auchindon_spell_void_mending") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    l_Caster->AddAura(SPELL_VOID_MENDING_AURA, l_Target);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new everbloom_spells();
    }
};

/// Void Shift - 
class auchindoun_void_shift : public SpellScriptLoader
{
public:
    auchindoun_void_shift() : SpellScriptLoader("auchindoun_void_shift") { }

    class auchindoun_spells : public SpellScript
    {
        PrepareSpellScript(auchindoun_spells);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            Unit* l_Caster = GetCaster();
            std::list<Unit*> l_ListPlayers;
            int32 l_CalcDamage = NULL;

            JadeCore::AnyUnitInObjectRangeCheck check(GetCaster(), 30.0f);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(l_Caster, l_ListPlayers, check);
            l_Caster->VisitNearbyObject(30.0f, searcher);

            if (l_ListPlayers.empty())
                return;

            for (std::list<Unit*>::const_iterator it = l_ListPlayers.begin(); it != l_ListPlayers.end(); ++it)
            {
                if (!(*it))
                    return;

                if ((*it)->GetTypeId() == TypeID::TYPEID_PLAYER)
                {
                    l_CalcDamage = 5000;
                    GetCaster()->CastCustomSpell((*it), SPELL_VOID_SHIFT_DAMAGE, &l_CalcDamage, NULL, NULL, true, NULL);
                }
                else
                {
                    l_CalcDamage = 8000;
                    GetCaster()->CastCustomSpell((*it), SPELL_VOID_SHIFT_HEALO, &l_CalcDamage, NULL, NULL, true, NULL);
                }
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindoun_spells();
    }
};

/// Arcane Bomb - 157652,
class auchindon_spell_arcane_bomb_dummy : public SpellScriptLoader
{
public:
    auchindon_spell_arcane_bomb_dummy() : SpellScriptLoader("auchindon_spell_arcane_bomb_dummy") { }

    class auchindon_spells : public AuraScript
    {
        PrepareAuraScript(auchindon_spells);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TriggerSpell = 157791;
            return true;
        }

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_spells::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new auchindon_spells();
    }
};

/// Void Shell - 
class auchindon_spell_void_shell_filter : public SpellScriptLoader
{
public:
    auchindon_spell_void_shell_filter() : SpellScriptLoader("auchindon_spell_void_shell_filter") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            Unit* l_Caster = GetCaster();

            targets.clear();

            std::list<Unit*> l_TargetList;
            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(l_Caster, l_Caster, 10.0f);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(l_Caster, l_TargetList, u_check);
            l_Caster->VisitNearbyObject(10.0f, searcher);

            if (!l_TargetList.empty())
            {
                for (auto itr : l_TargetList)
                {
                    if (itr->GetTypeId() == TYPEID_PLAYER)
                        return;

                    if (itr && itr->IsInWorld())
                    targets.push_back(itr);
                }
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, EFFECT_0, TARGET_UNIT_TARGET_ANY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};

/// Fixate - 
class auchindon_spell_fixate : public SpellScriptLoader
{
public:
    auchindon_spell_fixate() : SpellScriptLoader("auchindon_spell_fixate") { }

    class spell_auchindons : public AuraScript
    {
        PrepareAuraScript(spell_auchindons);

        void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Caster->GetMotionMaster()->MoveFollow(l_Target, 0, 0, MovementSlot::MOTION_SLOT_ACTIVE);
                    l_Caster->AddThreat(l_Target, 400.0f);
                }
            }
        }

        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Caster->GetMotionMaster()->MovePoint(0, l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ());
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_auchindons::OnApply,    EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_auchindons::OnRemove, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_auchindons();
    }
};

/// Arcane Bomb trigger
class auchindon_areatrigger_arcane_bomb : public AreaTriggerEntityScript
{
public:
    auchindon_areatrigger_arcane_bomb() : AreaTriggerEntityScript("auchindon_areatrigger_arcane_bomb")
    {
    }

    int m_Diff = 9 * TimeConstants::IN_MILLISECONDS;
     
    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Diff <= p_Time)
        {
            Position l_Pos;
            p_AreaTrigger->GetPosition(&l_Pos);

            p_AreaTrigger->SummonCreature(TRIGGER_ARCANE_BOMB_EXPLOSION_NPC, l_Pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

            m_Diff = 20 * TimeConstants::IN_MILLISECONDS;
        }
        else
        {
            m_Diff -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        // Does nothing.
    }

    auchindon_areatrigger_arcane_bomb* GetAI() const override
    {
        return new auchindon_areatrigger_arcane_bomb();
    }
};

/// Tuulani Unlock Gate - 
class auchindon_spell_tuulani_unlock : public SpellScriptLoader
{
public:
    auchindon_spell_tuulani_unlock() : SpellScriptLoader("auchindon_spell_tuulani_unlock") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            // Clears all targets at start, fetching new ones
            targets.clear();

            std::list<Creature*> l_ListTriggerWall;
            GetCaster()->GetCreatureListWithEntryInGrid(l_ListTriggerWall, , 15.0f);

            if (l_ListTriggerWall.empty())
                return;

            for (auto itr : l_ListTriggerWall)
            {
                if (itr && itr->IsInWorld())
                {
                    targets.push_back(itr->ToUnit());
                }
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(auchindon_spells::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};

// 150905, 158565, 
class auchindon_spell_beam_cosmetic : public SpellScriptLoader
{
public:
    auchindon_spell_beam_cosmetic() : SpellScriptLoader("auchindon_spell_beam_cosmetic") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
            spell->Effects[0].TargetB = 0;
            spell->AttributesEx9 = 0;
            spell->AttributesEx6 = 0;
            spell->AttributesEx5 = 0;
            spell->AttributesEx4 = 0;
            return true;
        }

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            targets.clear();

            if (Creature* shadowbeamtrigger = GetCaster()->FindNearestCreature(CreatureShadowBeam, 200.0f, true))
                targets.push_back(shadowbeamtrigger);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};

/// Arcane Bomb Trigger - 
class auchindon_trigger_arcane_bomb_npc_explosion : public CreatureScript
{
public:
    auchindon_trigger_arcane_bomb_npc_explosion() : CreatureScript("auchindon_trigger_arcane_bomb_npc_explosion") { }

    struct auchinond_creaturesAI : public Scripted_NoMovementAI
    {
        auchinond_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            Reset();
            me->Respawn(true);
        }

        void Reset() override
        {
            me->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(eAuchindonInformation::InformationHostileFaction);
            me->CastSpell(me, SpellArcaneBombDmg, true);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchinond_creaturesAI(p_Creature);
    }
};

/// Warden Hammer - 
class auchindon_trigger_warden_hammer : public CreatureScript
{
public:
    auchindon_trigger_warden_hammer() : CreatureScript("auchindon_trigger_warden_hammer") { }

    struct auchindon_creaturesAI : public ScriptedAI
    {
        auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(eAuchindonInformation::InformationHostileFaction);
        }

        uint32 m_DiffHammer = 500;

        void UpdateAI(const uint32 p_Diff) override
        {
            if (m_DiffHammer <= p_Diff)
            {
                std::list<Player*> l_ListNerbyPlayers;
                me->GetPlayerListInGrid(l_ListNerbyPlayers, 3.0f);

                if (l_ListNerbyPlayers.empty())
                    return;

                for (auto itr : l_ListNerbyPlayers)
                {
                    if (itr && itr->IsInWorld())
                    {
                        me->CastSpell(itr, SpellWardenHammerLightningTempo);
                        me->CastSpell(itr, SpellWardenHammerDamage);
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
        return new auchindon_creaturesAI(p_Creature);
    }
};

void AddSC_auchindoun()
{
    new auchindon_creature_tuulani();
    new auchindon_creature_sargerei_soulbinder();
    new auchindon_creature_sargerei_cleric();
    new auchindon_creature_sargerei_ritualist();
    new auchindon_creature_sargerei_zealot();
    new auchindon_creature_sargerei_spirit_tender();
    new auchindon_creature_sargerei_hopilite();
    new auchindon_creature_felborne_abyssal();
    new auchindon_creature_felguard();
    new auchindon_creature_cackling_pyromaniac();
    new auchindon_creature_blazing_pyromaniac();
    new auchindon_creature_sargeri_warden();
    new auchindon_creature_sargerei_magus();
    new auchindon_creature_sargerei_defender();
    new auchindon_trigger_arcane_bomb_npc_explosion();
    new auchindon_trigger_warden_hammer();
    new auchindon_areatrigger_arcane_bomb();
    new auchindoun_void_shift();
    new auchindon_spell_void_mending();
    new auchindon_spell_void_shell_filter();
    new auchindon_spell_arcane_bomb_dummy();
    new auchindon_spell_fixate();
    new auchindon_spell_tuulani_unlock();
    new auchindon_spell_beam_cosmetic();
}