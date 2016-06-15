
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "ScriptedCreature.h"
#include "auchindoun.hpp"

enum eTerongorSpells
{
    SpellAgony                              = 156925,
    SpellChaosBolt                          = 156975,
    SpellChaosWaveDummy                     = 157001,
    SpellChaosWaveDmg                       = 157002,
    SpellConflagrate                        = 154083,
    SpellCorruptionDummy                    = 170608,
    SpellCorruptionDmg                      = 156842,
    SpellCurseOfExhaustionDebuff            = 164841,
    SpellDemonicLeapDummy                   = 157039,
    SpellDemonicLeapAreatriger              = 157040,
    SpellDoomBuff                           = 156965,
    SpellDrainLife                          = 156854,
    SpellImmolate                           = 156964,
    SpellIncinrate                          = 156963,
    SpellRainOfFire                         = 165757,
    SpellSeedOfMalevolenceApplyAura         = 156921,
    SpellSeedOfMalevolenceBuff              = 166451,
    SpellSeedOfMalevolenceDmg               = 156924,
    SpellSeedOfMalevolenceVisualTriger      = 166462,
    SpellShadowBolt                         = 156829,
    SpellTouchOfChaosDummy                  = 169028,
    SpellTouchOfChaosDmg                    = 156968,
    SpellUnstableAffliction                 = 156954,
    SpellWrathCleave                        = 159035,
    SpellWrathStorm                         = 159033,
    SpellTranscend                          = 164009,
    SpellAfflictionTransform                = 156863,
    SpellDestructionTransform               = 156866,
    SpellDemonologyTransform                = 156919,
    SpellSummonAbyssalMeteor                = 164508,
    SpellSummonAbyssalGroundEffect          = 159681,
    SpellSummonAbyssalDummy                 = 157214,
    SpellSummonAbyssalSummonSpell           = 157216,
    SpellDemonicCircleVisual                = 149133,
    SpellTeronogorShield                    = 157017,
    SpellBrokenSouls                        = 72398,
    SpellSoulBarrage                        = 72305
};

enum eTerongorEvents
{
    EventAgony = 1,
    EventChaosBolt,
    EventChaosWave,
    EventConflagrate,
    EventCorruption,
    EventCurseOfExhaustion,
    EventDemonicLeap,
    EventDoom,
    EventDrainLife,
    EventImmolate,
    EventIncinrate,
    EventRainOfFire,
    EventSeedOfMalevolence,
    EventTouchOfChaos,
    EventUnstableAffliction,
    EventShadowBolt,
    EventWrathcleave,
    EventWrathstorm,
    EventBloomOfMalevolence,
    EventDepassive,
    EventTransform,
    EventTransformRemovePassive
};

enum eTerongorTalks
{
    TERONGOR_INTRO_01 = 42, ///< p_Who know the draenei held such...delicious treasure in their temple?  (44423)
    TERONGOR_INTRO_02 = 43, ///< Do you dare challenge me,defenders of Auchindoun?!(44424)
    TERONGOR_INTRO_03 = 44, ///< Such decadence...it will all burn. (44425)
    TERONGOR_INTRO_05 = 45, ///< Gul'dan, such foolshness. This gift...so much more you could ever know...(44427)
    TERONGOR_INTRO_06 = 46, ///< Long have i waited...(44428)
    TERONGOR_INTRO_07 = 47, ///< ...hungered... to be more... (44429)
    TERONGOR_INTRO_08 = 48, ///<  And now, all shall bow before me!(44430)
    TERONGOR_KILL_01  = 49, ///< All will fall before me!(44431)
    TERONGOR_AGGRO_01 = 50, ///< This power. you will be the first to know it.(44418)
    TERONGOR_SPELL_01 = 52, ///< Destruction!(44433)
    TERONGOR_SPELL_02 = 53, ///< Wither.. away! (44434)
    TERONGOR_SPELL_03 = 53, ///< I become something greater!(44435)
    TERONGOR_SPELL_04 = 54, ///< Die! (44436)
    TERONGOR_SPELL_05 = 55, ///< Your demise awaits! (44437)
    TERONGOR_SPELL_06 = 56, ///< Quickly now. (44438)
    TERONGOR_SPELL_07 = 57, ///< More! I...need...more!(44439)
    TERONGOR_EVENT_01 = 60, ///< Jorrun.. p_Who battle demons in life, in death.. your power will feed them! (44420)
    TERONGOR_EVENT_02 = 61, ///< Joraa, paragon of order - i shall twist your soul to power Destruction!(44421)
    TERONGOR_EVENT_03 = 61, ///< Elum, life time of healing... now, you shall fuel Deziz and Decay!(44422)
    TERONGOR_DEATH    = 100 ///< (44419)
};

enum eTeronogorActions
{
    ActionTransport = 1,
    ActionChoosePower
};

enum eTeronogorCreatures
{
    TriggerSummonAbyssal = 213593
};

enum eTeronogorTransformations
{
    TransformationAffliction  = 0,
    TransformationDestruction = 1,
    TransformationDemonology  = 2,
    TransformationPreChannel  = 3,
    TransformationOccur       = 4,
    TransformationOccured     = 5
};

class EventTeronogorTransform : public BasicEvent
{
    public:

    explicit EventTeronogorTransform(Unit* p_Unit, int32 p_Value, int32 p_TransformationType) : m_Obj(p_Unit), m_Modifier(p_Value), m_tType(p_TransformationType), BasicEvent()
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_p_Diff*/)
    {
        enum eTeronogorTransforkmSpells
        {
            SpellDrainSoulVisual      = 156582,
            SpellAfflictionTransform  = 156863,
            SpellDestructionTransform = 156866,
            SpellDemonologyTransform  = 156919 
        };

        if (m_Obj)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
                {
                    if (Creature* l_Iruun = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataIruun)))
                    {
                        if (Creature* l_Jorra = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataJorra)))
                        {
                            if (Creature* l_Elum = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataElum)))
                            {
                                if (l_Teronogor->IsAIEnabled)
                                {
                                    switch (m_Modifier)
                                    {
                                        case eTeronogorTransformations::TransformationPreChannel:
                                        {
                                            switch (m_tType)
                                            {
                                                case eTeronogorTransformations::TransformationAffliction:
                                                    l_Teronogor->CastSpell(l_Elum, eTeronogorTransforkmSpells::SpellDrainSoulVisual);
                                                    break;
                                                case eTeronogorTransformations::TransformationDestruction:
                                                    l_Teronogor->CastSpell(l_Jorra, eTeronogorTransforkmSpells::SpellDrainSoulVisual);
                                                    break;
                                                case eTeronogorTransformations::TransformationDemonology:
                                                    l_Teronogor->CastSpell(l_Iruun, eTeronogorTransforkmSpells::SpellDrainSoulVisual);
                                                    break;
                                                default:
                                                    break;
                                            }

                                            l_Teronogor->m_Events.AddEvent(new EventTeronogorTransform(l_Teronogor, eTeronogorTransformations::TransformationOccur, m_tType), l_Teronogor->m_Events.CalculateTime(5 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case eTeronogorTransformations::TransformationOccur:
                                        {
                                            l_Teronogor->RemoveAllAuras();
                                            l_Teronogor->CastStop();

                                            switch (m_tType)
                                            {
                                                case eTeronogorTransformations::TransformationAffliction:
                                                    l_Teronogor->CastSpell(l_Teronogor, eTeronogorTransforkmSpells::SpellAfflictionTransform);
                                                    break;
                                                case eTeronogorTransformations::TransformationDestruction:
                                                    l_Teronogor->CastSpell(l_Teronogor, eTeronogorTransforkmSpells::SpellDestructionTransform);
                                                    break;
                                                case eTeronogorTransformations::TransformationDemonology:
                                                    l_Teronogor->CastSpell(l_Teronogor, eTeronogorTransforkmSpells::SpellDemonologyTransform);
                                                    break;
                                                default:
                                                    break;
                                            }
                                      
                                            l_Teronogor->UpdatePosition(*l_Teronogor);
                                            l_Teronogor->SetReactState(ReactStates::REACT_DEFENSIVE);
                                            l_Teronogor->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);                                  
                                            break;
                                        }
                                        default:
                                            break;
                                    }
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
    Unit* m_Obj;
    int32 m_Modifier;
    int32 m_tType;
    int32 m_Event;
};

class EventTeronogorPostDeath : public BasicEvent
{
    public:

    explicit EventTeronogorPostDeath(Unit* p_Unit, int32 p_Value) : m_Obj(p_Unit), m_Modifier(p_Value), BasicEvent()
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
                {
                    if (Creature* l_Tuulani = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataTuulani02)))
                    { 
                        if (l_Tuulani->IsAIEnabled && l_Teronogor->IsAIEnabled)
                        { 
                            switch (m_Modifier)
                            {
                                case 0:
                                {
                                    l_Teronogor->SetCanFly(true);
                                    l_Teronogor->SetDisableGravity(true);
                                    l_Teronogor->NearTeleportTo(l_Teronogor->GetPositionX(), l_Teronogor->GetPositionY(), 28.884939f, l_Teronogor->GetOrientation());
                                    l_Teronogor->AddAura(eTerongorSpells::SpellSoulBarrage, l_Teronogor);
                                    l_Teronogor->AddAura(eTerongorSpells::SpellBrokenSouls, l_Teronogor);
                                    l_Teronogor->m_Events.AddEvent(new EventTeronogorPostDeath(l_Teronogor, 1), l_Teronogor->m_Events.CalculateTime(5 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 1:
                                {
                                    l_Teronogor->GetMotionMaster()->MoveKnockbackFrom(l_Teronogor->GetPositionX(), l_Teronogor->GetPositionY(), 14.0f, 12.0f);
                                    l_Teronogor->m_Events.AddEvent(new EventTeronogorPostDeath(l_Teronogor, 2), l_Teronogor->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 2:
                                {
                                    if (Player* l_Player = l_Teronogor->FindNearestPlayer(1000.0f, true))
                                        l_Teronogor->AI()->JustDied(l_Player);

                                    l_Teronogor->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
                                    l_Tuulani->m_Events.AddEvent(new EventTeronogorPostDeath(l_Tuulani, 3), l_Tuulani->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 3:
                                {
                                    l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK14);
                                    l_Tuulani->m_Events.AddEvent(new EventTeronogorPostDeath(l_Tuulani, 4), l_Tuulani->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 4:
                                {
                                    l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK15);
                                    l_Teronogor->m_Events.AddEvent(new EventTeronogorPostDeath(l_Teronogor, 5), l_Teronogor->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 5:
                                {
                                    l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK16);
                                    l_Teronogor->m_Events.AddEvent(new EventTeronogorPostDeath(l_Teronogor, 6), l_Teronogor->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 6:
                                {
                                    l_Tuulani->AI()->Talk(eAuchindounTalks::TUULANITALK14);
                                    break;
                                }
                                default:
                                    break;
                            }
                        }       
                    }
                }         
            }
        }

        return true;
    }

private:
    Unit* m_Obj;
    int32 m_Modifier;
    int32 m_Event;
};

/// Teron'gor <Shadow Council> - 77734
class boss_teronogor : public CreatureScript
{
    public:

    boss_teronogor() : CreatureScript("boss_teronogor") { }

    struct boss_teronogorAI : public BossAI
    {
        boss_teronogorAI(Creature* p_Creature) : BossAI(p_Creature, eAuchindounDatas::DataBossTeronogor)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
            m_First = false;
        }

        InstanceScript* m_Instance;   
        bool m_Intro;
        bool m_First;
        bool m_SecondPhase;
        bool m_SoulTransport01;
        bool m_SoulTransport02;
        bool m_SoulTransport03;
        bool m_SoulTransport04;
        bool m_SoulTransport05;

        void Reset() override
        {
            _Reset();
            events.Reset();
            m_SecondPhase = false;
            me->SetReactState(ReactStates::REACT_DEFENSIVE);   
            me->AddAura(eTerongorSpells::SpellTeronogorShield, me);
            me->CastSpell(me, eTerongorSpells::SpellDemonicCircleVisual); 

            if (m_Instance != nullptr)
            if (Creature* l_Jorra = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataIruun)))
                me->CastSpell(l_Jorra, eAuchindounSpells::SpellDrainSoulVisual);
            if (m_First)
            {
                m_First = false;
                m_SoulTransport01 = false;
                m_SoulTransport02 = false;
                m_SoulTransport03 = false;
                m_SoulTransport04 = false;
                m_SoulTransport05 = false;
              
                me->GetMap()->SetObjectVisibility(1000.0f);        
            }
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 18.0f) && !m_Intro)
            {
                m_Intro = true;
                Talk(eTerongorTalks::TERONGOR_INTRO_01);
            }
        }

        void JustReachedHome() override
        {
            summons.DespawnAll();
            DespawnCreaturesInArea(eAuchindounCreatures::CreatureFelborneAbyssal, me);

            if (m_Instance != nullptr)
                instance->SetBossState(eAuchindounDatas::DataBossTeronogor, EncounterState::FAIL);
        }

        void DamageTaken(Unit* p_Attacker, uint32 &p_Damage, SpellInfo const* p_SpellInfo) override
        {
            if (me->GetHealthPct() <= 75 && !m_SecondPhase)
            {
                events.Reset();
                m_SecondPhase = true;        
                me->SetReactState(ReactStates::REACT_PASSIVE);
                DoAction(eTeronogorActions::ActionChoosePower);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);       
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eAuchindounActions::ActionSoulMove1:
                    m_SoulTransport01 = true;
                    break;
                case eAuchindounActions::ActionSoulMove2:
                    m_SoulTransport02 = true;
                    break;
                case eAuchindounActions::ActionSoulMove3:
                    m_SoulTransport03 = true;
                    break;
                case eAuchindounActions::ActionSoulMove4:
                    m_SoulTransport04 = true;
                    break;
                case eTeronogorActions::ActionChoosePower:
                {
                    events.Reset();                  
                    switch (urand(eTeronogorTransformations::TransformationAffliction, eTeronogorTransformations::TransformationDemonology))
                    {
                        case eTeronogorTransformations::TransformationAffliction: // Mender Elum - Affliction
                            events.Reset();
                            Talk(eTerongorTalks::TERONGOR_EVENT_03);
                            events.ScheduleEvent(eTerongorEvents::EventShadowBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventCurseOfExhaustion, 13 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTerongorEvents::EventSeedOfMalevolence, urand(22 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventAgony, 16 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTerongorEvents::EventDrainLife, urand(13 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventUnstableAffliction, 20 * TimeConstants::IN_MILLISECONDS);
                            me->m_Events.AddEvent(new EventTeronogorTransform(me, eTeronogorTransformations::TransformationPreChannel, 0), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));              
                            break;
                        case eTeronogorTransformations::TransformationDestruction: // Arcanist Jorra's - destruction
                            events.Reset();
                            Talk(eTerongorTalks::TERONGOR_EVENT_01);
                            events.ScheduleEvent(eTerongorEvents::EventChaosBolt, 20 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTerongorEvents::EventImmolate, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventConflagrate, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventRainOfFire, 24 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTerongorEvents::EventIncinrate, 16 * TimeConstants::IN_MILLISECONDS);
                            me->m_Events.AddEvent(new EventTeronogorTransform(me, eTeronogorTransformations::TransformationPreChannel, 1), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));           
                            break;
                        case eTeronogorTransformations::TransformationDemonology: // Vindication Iruun's - demonology
                            events.Reset();
                            Talk(eTerongorTalks::TERONGOR_EVENT_02);
                            events.ScheduleEvent(eTerongorEvents::EventDoom, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventDemonicLeap, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventCurseOfExhaustion, 18 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTerongorEvents::EventCorruption, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                            events.ScheduleEvent(eTerongorEvents::EventChaosBolt, 25 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTerongorEvents::EventTouchOfChaos, 16 * TimeConstants::IN_MILLISECONDS);
                            me->m_Events.AddEvent(new EventTeronogorTransform(me, eTeronogorTransformations::TransformationPreChannel, 2), me->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));             
                            break;
                        default:
                            break;
                    }
                }
                default:
                    break;         
            }
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            _EnterCombat();
            me->CastStop();
            me->RemoveAllAuras();
            Talk(eTerongorTalks::TERONGOR_AGGRO_01);
            me->RemoveAura(eTerongorSpells::SpellTeronogorShield);
            events.ScheduleEvent(eTerongorEvents::EventShadowBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTerongorEvents::EventCorruption, urand(10 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTerongorEvents::EventRainOfFire, 21 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eTerongorEvents::EventDrainLife, 16 * TimeConstants::IN_MILLISECONDS);
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

                if (me->GetMap() && me->GetMap()->IsHeroic())
                {
                    Position l_Position;
                    me->GetRandomNearPosition(l_Position, 10.0f);
                    me->SummonCreature(eTeronogorCreatures::TriggerSummonAbyssal, l_Position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                }
            }
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                 Talk(eTerongorTalks::TERONGOR_KILL_01);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            Talk(eTerongorTalks::TERONGOR_DEATH);
            DespawnCreaturesInArea(eAuchindounCreatures::CreatureFelborneAbyssal, me);
            if (m_Instance != nullptr)
                m_Instance->SetBossState(eAuchindounDatas::DataBossTeronogor, EncounterState::DONE);
            me->SummonGameObject(eAuchindounObjects::GameobjectChestAucheni, 1891.84f, 2973.80f, 16.844f, 5.664811f, 0, 0, 0, 0, 0);
            me->SummonCreature(eAuchindounCreatures::CreatureSoulBinderTuulani01, 1911.65f, 2757.72f, 30.799f, 1.566535f, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            me->m_Events.AddEvent(new EventTeronogorPostDeath(me, 1), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
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
                case eTerongorEvents::EventShadowBolt:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTerongorSpells::SpellShadowBolt);
                    events.ScheduleEvent(eTerongorEvents::EventShadowBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eTerongorEvents::EventCorruption:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTerongorSpells::SpellCorruptionDummy);
                    events.ScheduleEvent(eTerongorEvents::EventCorruption, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eTerongorEvents::EventDrainLife:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTerongorSpells::SpellDrainLife);
                    events.ScheduleEvent(eTerongorEvents::EventDrainLife, 16 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eTerongorEvents::EventCurseOfExhaustion:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                       me->CastSpell(l_Random, eTerongorSpells::SpellCurseOfExhaustionDebuff);
                    events.ScheduleEvent(eTerongorEvents::EventCurseOfExhaustion, 13 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eTerongorEvents::EventAgony:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTerongorSpells::SpellAgony);
                    events.ScheduleEvent(eTerongorEvents::EventAgony, 16 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eTerongorEvents::EventUnstableAffliction:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTerongorSpells::SpellUnstableAffliction);
                    events.ScheduleEvent(eTerongorEvents::EventUnstableAffliction, 20 * TimeConstants::IN_MILLISECONDS);    
                    break;
                }
                case eTerongorEvents::EventSeedOfMalevolence:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTerongorSpells::SpellSeedOfMalevolenceApplyAura);
                    events.ScheduleEvent(eTerongorEvents::EventSeedOfMalevolence, urand(22 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eTerongorEvents::EventChaosBolt:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(me->getVictim(), eTerongorSpells::SpellChaosBolt);
                    events.ScheduleEvent(eTerongorEvents::EventChaosBolt, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eTerongorEvents::EventImmolate:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTerongorSpells::SpellImmolate);
                    events.ScheduleEvent(eTerongorEvents::EventImmolate, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eTerongorEvents::EventConflagrate:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTerongorSpells::SpellConflagrate);
                    events.ScheduleEvent(eTerongorEvents::EventConflagrate, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eTerongorEvents::EventIncinrate:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTerongorSpells::SpellIncinrate);
                    events.ScheduleEvent(eTerongorEvents::EventIncinrate, 16 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eTerongorEvents::EventRainOfFire:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))             
                        me->CastSpell(l_Random, eTerongorSpells::SpellRainOfFire, true);
                   events.ScheduleEvent(eTerongorEvents::EventRainOfFire, 25 * TimeConstants::IN_MILLISECONDS);
                   break;
                case eTerongorEvents::EventDoom:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTerongorSpells::SpellDoomBuff);
                     events.ScheduleEvent(eTerongorEvents::EventDoom, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                     break;
                }
                case eTerongorEvents::EventDemonicLeap:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTerongorSpells::SpellDemonicLeapDummy);
                    events.ScheduleEvent(eTerongorEvents::EventDemonicLeap, urand(18 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eTerongorEvents::EventChaosWave:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTerongorSpells::SpellChaosWaveDummy);
                    events.ScheduleEvent(eTerongorEvents::EventChaosWave, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eTerongorEvents::EventTouchOfChaos:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTerongorSpells::SpellTouchOfChaosDummy);
                    events.ScheduleEvent(eTerongorEvents::EventTouchOfChaos, 16 * TimeConstants::IN_MILLISECONDS);
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
        return new boss_teronogorAI(p_Creature);
    }
};

/// Durag The Dominator <Shadow Council> - 77890
class auchindoun_teronogor_mob_durag : public CreatureScript
{
    public:

    auchindoun_teronogor_mob_durag() : CreatureScript("auchindoun_teronogor_mob_durag") { }

    struct auchindoun_teronogor_mob_duragAI : public ScriptedAI
    {
        auchindoun_teronogor_mob_duragAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_First = true;
        }

        enum eTeronogorDuragSpells
        {
            GrimoireofServitude  = 159021,
            SpellShadowBolt      = 156829,
            SpellCorruptionDummy = 170608,
            SpellChaosWaveDummy  = 157001
        };

        enum eTeronogorDuragEvents
        {
            EventShadowBolt = 1,
            EventCorruption,
            EventChaosWave
        };

        InstanceScript* m_Instance;
        bool m_First;

        void Reset() override
        {
            events.Reset();
            if (me->GetMap())
                me->GetMap()->SetObjectVisibility(1000.0f);

            /// Cosmetic channel - 
            if (m_Instance != nullptr)
            {
                if (Creature* l_Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
                {
                    me->CastStop();
                    me->CastSpell(l_Teronogor, eAuchindounSpells::SpellDrainSoulVisual);
                }
            }

            if (m_First)
            {
                m_First = false;
              
            }

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* p_Attacker) override
        { 
            me->CastStop();
            me->RemoveAllAuras();
            me->CastSpell(me, eTeronogorDuragSpells::GrimoireofServitude);
            events.ScheduleEvent(eTeronogorDuragEvents::EventShadowBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTeronogorDuragEvents::EventCorruption, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTeronogorDuragEvents::EventChaosWave, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);    

            switch (events.ExecuteEvent())
            {
                case eTeronogorDuragEvents::EventShadowBolt:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eTeronogorDuragSpells::SpellShadowBolt);
                        events.ScheduleEvent(eTeronogorDuragEvents::EventShadowBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eTeronogorDuragEvents::EventCorruption:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Random, eTeronogorDuragSpells::SpellCorruptionDummy);
                        events.ScheduleEvent(eTeronogorDuragEvents::EventCorruption, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eTeronogorDuragEvents::EventChaosWave:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eTeronogorDuragSpells::SpellChaosWaveDummy);
                        events.ScheduleEvent(eTeronogorDuragEvents::EventChaosWave, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_teronogor_mob_duragAI(p_Creature);
    }
};

/// Gulkosh <Shadow Council> - 78437
class auchindoun_teronogor_mob_gulkosh : public CreatureScript
{
    public:

    auchindoun_teronogor_mob_gulkosh() : CreatureScript("auchindoun_teronogor_mob_gulkosh") { }

    struct auchindoun_teronogor_mob_gulkoshAI : public ScriptedAI
    {
        auchindoun_teronogor_mob_gulkoshAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_First = true;
        }

        enum eTeronogorGulkoshSpells
        {
            SpellShadowBolt         = 156829,
            SpellUnstableAffliction = 156954,
            SpellDrainLife          = 156854
        };

        enum eTeronogorGulkoshEvents
        {
            EventShadowBolt = 1,
            EventUnstableAffliction,
            EventDrainLife
        };

        InstanceScript* m_Instance;
        bool m_First;

        void Reset() override
        {
            events.Reset();

            if (me->GetMap())
                me->GetMap()->SetObjectVisibility(1000.0f);

            if (m_First)
            {
                m_First = false;
              
            }

            if (m_Instance != nullptr)
            {
                if (Creature* l_Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
                {
                    me->CastStop();
                    me->CastSpell(l_Teronogor, eAuchindounSpells::SpellDrainSoulVisual);
                }
            }
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->CastStop();
            me->RemoveAllAuras();
            events.ScheduleEvent(eTeronogorGulkoshEvents::EventShadowBolt, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eTeronogorGulkoshEvents::EventUnstableAffliction, urand(10 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTeronogorGulkoshEvents::EventDrainLife, 16 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
            case eTeronogorGulkoshEvents::EventShadowBolt:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTeronogorGulkoshSpells::SpellShadowBolt);
                    events.ScheduleEvent(eTeronogorGulkoshEvents::EventShadowBolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                    break;
            case eTeronogorGulkoshEvents::EventUnstableAffliction:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eTeronogorGulkoshSpells::SpellUnstableAffliction);
                    events.ScheduleEvent(eTeronogorGulkoshEvents::EventUnstableAffliction, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                    break;
            case eTeronogorGulkoshEvents::EventDrainLife:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eTeronogorGulkoshSpells::SpellDrainLife);
                    events.ScheduleEvent(eTeronogorGulkoshEvents::EventDrainLife, 16 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_teronogor_mob_gulkoshAI(p_Creature);
    }
};

/// Shaduum - 78728
class auchindoun_teronogor_mob_shaadum : public CreatureScript
{
    public:

    auchindoun_teronogor_mob_shaadum() : CreatureScript("auchindoun_teronogor_mob_shaadum") { }

    struct auchindoun_teronogor_mob_shaadumAI : public ScriptedAI
    {
        auchindoun_teronogor_mob_shaadumAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_First = true;
        }
     
        enum eShaddumSpells
        {
            SpellWrathStorm  = 159033,
            SpellWrathCleave = 159035
        };

        enum eShaddumEvents
        {
            EventWrathcleave = 1,
            EventWrathstorm
        };

        bool m_First;

        void Reset() override
        {
            events.Reset();

            if (m_First)
            {
                m_First = false;
              
            }

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eTerongorEvents::EventWrathcleave, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eTerongorEvents::EventWrathstorm, urand(14 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eShaddumEvents::EventWrathstorm:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eShaddumSpells::SpellWrathStorm);
                    events.ScheduleEvent(eShaddumEvents::EventWrathstorm, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eShaddumEvents::EventWrathcleave:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eShaddumSpells::SpellWrathCleave);
                    events.ScheduleEvent(eShaddumEvents::EventWrathcleave, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_teronogor_mob_shaadumAI(p_Creature);
    }
};

/// Grom'tash The Destructor <Shadow Council> - 77889
class auchindoun_teronogor_mob_gromkash : public CreatureScript
{
    public:

    auchindoun_teronogor_mob_gromkash() : CreatureScript("auchindoun_teronogor_mob_gromkash") { }

    struct auchindoun_teronogor_mob_gromkashAI : public ScriptedAI
    {
        auchindoun_teronogor_mob_gromkashAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_First = true;
            m_Instance = me->GetInstanceScript();
        }

        enum eGromkashSpells
        {
            SpellGrimoireOfSacrifice = 159024,
            SpellImmolate            = 156964,
            SpellIncinrate           = 146963,
            SpellRainOfFire          = 65757
        };

        enum eGromkashEvents
        {
            EventImmolate = 1,
            EventIncinrate,
            EventRainOfFire
        };

        InstanceScript* m_Instance;
        bool m_First;

        void Reset() override
        {
            events.Reset();

            if (me->GetMap())
                me->GetMap()->SetObjectVisibility(1000.0f);  

            if (m_First)
            {
                m_First = false;
              
            }

            if (m_Instance != nullptr)
            {
                if (Creature* l_Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
                    me->CastSpell(l_Teronogor, eAuchindounSpells::SpellDrainSoulVisual);
            }
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->CastStop();
            me->RemoveAllAuras();
            events.ScheduleEvent(eTerongorEvents::EventImmolate, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eTerongorEvents::EventIncinrate, urand(10 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTerongorEvents::EventRainOfFire, 18 * TimeConstants::IN_MILLISECONDS);
            if (Creature* l_Zashoo = me->FindNearestCreature(eAuchindounCreatures::CreatureZashoo, 20.0f, true))
                me->CastSpell(l_Zashoo, eGromkashSpells::SpellGrimoireOfSacrifice); 
        }

        void JustDied(Unit* p_Killer) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
                    Teronogor->RemoveAura(eTerongorSpells::SpellTeronogorShield);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);
    
            switch (events.ExecuteEvent())
            {
                case eGromkashEvents::EventImmolate:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eGromkashSpells::SpellImmolate);
                    events.ScheduleEvent(eGromkashEvents::EventImmolate, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eGromkashEvents::EventIncinrate:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eGromkashSpells::SpellIncinrate);
                    events.ScheduleEvent(eGromkashEvents::EventIncinrate, urand(10 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eGromkashEvents::EventRainOfFire:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(l_Random, eGromkashSpells::SpellRainOfFire);
                    events.ScheduleEvent(eGromkashEvents::EventRainOfFire, 18 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_teronogor_mob_gromkashAI(p_Creature);
    }
};

/// Abyssal - 
class auchindoun_teronogor_mob_abyssal : public CreatureScript
{
    public:

    auchindoun_teronogor_mob_abyssal() : CreatureScript("auchindoun_teronogor_mob_abyssal") { }

    struct auchindoun_teronogor_mob_abyssalAI : public ScriptedAI
    {
        auchindoun_teronogor_mob_abyssalAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_First = true;
        }

        enum eTeronogorAbyssalSpells
        {
            SpellFixate = 173080,
        };

        enum eTeronogorAbyssalEvents
        {
            EventFixate = 1,
        };

        InstanceScript* m_Instance;
        uint64 m_Target;
        bool m_First;

        void Reset() override
        {      
            m_Target = 0;
            events.Reset();
 
            if (m_First)
            {
                m_First = false;
              
                me->CastSpell(me, eTerongorSpells::SpellSummonAbyssalMeteor);
            }
        }

        void BeginFixation()
        {
            if (m_Target == 0)
            {
                if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                {
                    m_Target = l_Random->GetGUID();
                    me->GetMotionMaster()->MoveChase(l_Random);
                    me->AddAura(eTeronogorAbyssalSpells::SpellFixate, l_Random);        
                }
            }
        }

        void OnAddThreat(Unit* p_Victim, float& p_FThreat, SpellSchoolMask /*p_SchoolMask*/, SpellInfo const* /*p_ThreatSpell*/) override
        {
            if (m_Target)
            {
                if (p_Victim->GetGUID() != m_Target)
                    p_FThreat = 0;
                return;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.3f, true);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || me->HasUnitState(UnitState::UNIT_STATE_STUNNED))
                return;

            if (m_Target)
            {
                ///< Burst hardcoded
                if (Player* l_Player = sObjectAccessor->GetPlayer(*me, m_Target))
                {
                    if (!l_Player->HasAura(eTeronogorAbyssalSpells::SpellFixate)) /// Fixated aura
                        me->AddAura(eTeronogorAbyssalSpells::SpellFixate, l_Player);

                    if (!me->IsMoving())
                        me->GetMotionMaster()->MoveFollow(l_Player, 0, 0, MovementSlot::MOTION_SLOT_ACTIVE);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new auchindoun_teronogor_mob_abyssalAI(p_Creature);
    }
};

class auchindoun_teronogor_mob_spirit : public CreatureScript
{
	public:

	auchindoun_teronogor_mob_spirit() : CreatureScript("auchindoun_teronogor_mob_spirit") { }

	struct auchindoun_teronogor_mob_spiritAI : public ScriptedAI
	{
		auchindoun_teronogor_mob_spiritAI(Creature* p_Creature) : ScriptedAI(p_Creature)
		{
			m_Instance = me->GetInstanceScript();
		}

		InstanceScript* m_Instance;

		void Reset() override
		{
			me->SetReactState(ReactStates::REACT_PASSIVE);
			me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
		}
	};

	CreatureAI* GetAI(Creature* p_Creature) const override
	{
		return new auchindoun_teronogor_mob_spiritAI(p_Creature);
	}
};

/// Chaos Wave - 157001
class auchindoun_teronogor_spell_chaos_wave : public SpellScriptLoader
{
    public:

    auchindoun_teronogor_spell_chaos_wave() : SpellScriptLoader("auchindoun_teronogor_spell_chaos_wave") { }

    class auchindoun_teronogor_spell_chaos_wave_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_teronogor_spell_chaos_wave_SpellScript);

        enum eChaosWaveSpells
        {
            SpellChaosWaveDmg = 157002
        };

        void HandleDummy(SpellEffIndex p_EffIndex)
        {
            if (!GetCaster() && !GetExplTargetUnit())
                return;

            GetCaster()->CastSpell(GetExplTargetUnit(), eChaosWaveSpells::SpellChaosWaveDmg);
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_teronogor_spell_chaos_wave_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_teronogor_spell_chaos_wave_SpellScript();
    }
};

/// Demonic Leap - 148969 
class auchindoun_teronogor_spell_demonic_leap : public SpellScriptLoader
{
    public:

    auchindoun_teronogor_spell_demonic_leap() : SpellScriptLoader("auchindoun_teronogor_spell_demonic_leap") { }

    class auchindoun_teronogor_spell_demonic_leap_SpellScript : public SpellScript
    {
        PrepareSpellScript(auchindoun_teronogor_spell_demonic_leap_SpellScript);

        enum eDemonicLeapSpells
        {
            SpellDemonicLeapJump = 157039
        };

        void HandleDummy(SpellEffIndex p_EffIndex)
        {
            if (!GetCaster() && !GetExplTargetUnit())
                return;

            GetCaster()->CastSpell(GetExplTargetUnit(), eDemonicLeapSpells::SpellDemonicLeapJump);
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_teronogor_spell_demonic_leap_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new auchindoun_teronogor_spell_demonic_leap_SpellScript();
    }
};

/// Seed of Malevolence - 156921 
class auchindoun_teronogor_spell_seed_of_malevolence : public SpellScriptLoader
{
    public:

    auchindoun_teronogor_spell_seed_of_malevolence() : SpellScriptLoader("auchindoun_teronogor_spell_seed_of_malevolence") { }

    class auchindoun_teronogor_spell_seed_of_malevolence_AuraScript : public AuraScript
    {
        PrepareAuraScript(auchindoun_teronogor_spell_seed_of_malevolence_AuraScript);

        enum eSeedOfMalevolanceSpells
        {
            SpellSeedOfMalevolenceDmg = 156924
        };

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (!GetTarget())
                return;

            GetTarget()->CastSpell(GetTarget(), eSeedOfMalevolanceSpells::SpellSeedOfMalevolenceDmg);
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(auchindoun_teronogor_spell_seed_of_malevolence_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DAMAGE, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new auchindoun_teronogor_spell_seed_of_malevolence_AuraScript();
    }
};

/// Teleportation Event
class auchindoun_soul_transportation_event : public BasicEvent
{
    public:

    explicit auchindoun_soul_transportation_event(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value), BasicEvent()
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* m_Instance = m_Obj->GetInstanceScript())
            {
                switch (m_Modifier)
                {
                        /// First soul transport
                    case 0:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionFirstPlatformFirstMove.GetPositionX(), g_PositionFirstPlatformFirstMove.GetPositionY(), g_PositionFirstPlatformFirstMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 1), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 1:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionFirstPlatormSecondMove.GetPositionX(), g_PositionFirstPlatormSecondMove.GetPositionY(), g_PositionFirstPlatormSecondMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 2), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 2:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionFirstPlatformThirdMove.GetPositionX(), g_PositionFirstPlatformThirdMove.GetPositionY(), g_PositionFirstPlatformThirdMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 100), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                        /// Second soul transport
                    case 4:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionSecondPlatformFirstMove.GetPositionX(), g_PositionSecondPlatformFirstMove.GetPositionY(), g_PositionSecondPlatformFirstMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 5), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 5:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionSecondPlatformSecondMove.GetPositionX(), g_PositionSecondPlatformSecondMove.GetPositionY(), g_PositionSecondPlatformSecondMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 6), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 6:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionSecondPlatformThirdMove.GetPositionX(), g_PositionSecondPlatformThirdMove.GetPositionY(), g_PositionSecondPlatformThirdMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 100), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                        /// Third soul transport
                    case 7:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionThirdPlatformFirstMove.GetPositionX(), g_PositionThirdPlatformFirstMove.GetPositionY(), g_PositionThirdPlatformFirstMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 8), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 8:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionThirdPlatformsSecondMove.GetPositionX(), g_PositionThirdPlatformsSecondMove.GetPositionY(), g_PositionThirdPlatformsSecondMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 9), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 9:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionThirdPlatformThirdMove.GetPositionX(), g_PositionThirdPlatformThirdMove.GetPositionY(), g_PositionThirdPlatformThirdMove.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 100), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                       /// Fourth soul transport and last
                    case 10:
                        m_Obj->GetMotionMaster()->MoveCharge(g_PositionFourthMovement.GetPositionX(), g_PositionFourthMovement.GetPositionY(), g_PositionFourthMovement.GetPositionZ(), 60.0f);
                        m_Obj->m_Events.AddEvent(new auchindoun_soul_transportation_event(m_Obj, 100), m_Obj->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case 100:
                        m_Obj->RemoveAura(eTerongorSpells::SpellTranscend);
                        m_Obj->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                        m_Obj->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        m_Obj->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);             
                        break;
                    default:
                        break;
                }
            }
        }

        return true;
    }

private:
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};

/// Soul Transport Object 01 - 231736
class auchindoun_teronogor_gameobject_soul_transporter_01 : public GameObjectScript
{
    public:

    auchindoun_teronogor_gameobject_soul_transporter_01() : GameObjectScript("auchindoun_teronogor_gameobject_soul_transporter_01") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Gobject)
	{
		if (InstanceScript* m_Instance = p_Gobject->GetInstanceScript())
		{
			if (Creature* m_Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
			{
				if (Creature* m_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetData64(eAuchindounDatas::DataBossAzzakael)))
				{
					if (m_Azzakel->isAlive())
						return false;

					if (boss_teronogor::boss_teronogorAI* l_LinkAI = CAST_AI(boss_teronogor::boss_teronogorAI, m_Teronogor->GetAI()))
					{
						if (l_LinkAI->m_SoulTransport01)
						{
							p_Player->AddAura(eTerongorSpells::SpellTranscend, p_Player);
							p_Player->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
							p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
							p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
							p_Player->m_Events.AddEvent(new auchindoun_soul_transportation_event(p_Player, 0), p_Player->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
							return true;
						}
					}
				}
			}
		}

		return false;
    }
};

/// Soul Transport Object 02 - 345366
class auchindoun_teronogor_gameobject_soul_transporter_02 : public GameObjectScript
{
    public:

    auchindoun_teronogor_gameobject_soul_transporter_02() : GameObjectScript("auchindoun_teronogor_gameobject_soul_transporter_02") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Gobject)
    {
        if (InstanceScript* l_Instance = p_Gobject->GetInstanceScript())
        {
            if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
            {
                if (Creature* l_Durag = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataDurag)))
                {
                    if (l_Durag->isDead())
                    {
                        if (l_Teronogor->IsAIEnabled)
                        {
                            if (boss_teronogor::boss_teronogorAI* l_LinkAI = CAST_AI(boss_teronogor::boss_teronogorAI, l_Teronogor->GetAI()))
                            {
                                if (l_LinkAI->m_SoulTransport02)
                                {
                                    p_Player->AddAura(eTerongorSpells::SpellTranscend, p_Player);
                                    p_Player->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                    p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                    p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                                    p_Player->m_Events.AddEvent(new auchindoun_soul_transportation_event(p_Player, 4), p_Player->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }
	/*
	struct auchindoun_teronogor_gameobject_soul_transporter_02AI : public GameObjectAI
	{
		auchindoun_teronogor_gameobject_soul_transporter_02AI(GameObject* p_GameObject) : GameObjectAI(p_GameObject) { }

		void Reset() override
		{
			if (InstanceScript* l_Instance = go->GetInstanceScript())
			{
				if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
				{
					go->SetLootState(LootState::GO_READY);
					go->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, l_Teronogor);
				}
			}
		}
	};
	

	GameObjectAI* GetAI(GameObject* p_GameObject) const override
	{
		return new auchindoun_teronogor_gameobject_soul_transporter_02AI(p_GameObject);
	}
	*/
};

/// Soul Transport Object 03 - 345367
class auchindoun_teronogor_gameobject_soul_transporter_03 : public GameObjectScript
{
	public:

	auchindoun_teronogor_gameobject_soul_transporter_03() : GameObjectScript("auchindoun_teronogor_gameobject_soul_transporter_03") { }

	bool OnGossipHello(Player* p_Player, GameObject* p_Gobject)
	{
		if (InstanceScript* l_Instance = p_Gobject->GetInstanceScript())
		{
			if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
			{
				if (Creature* l_Gulkosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataGulkosh)))
				{
					if (l_Gulkosh->isDead())
					{
						if (l_Teronogor->IsAIEnabled)
						{
							if (boss_teronogor::boss_teronogorAI* l_LinkAI = CAST_AI(boss_teronogor::boss_teronogorAI, l_Teronogor->GetAI()))
							{
								if (l_LinkAI->m_SoulTransport03)
								{
									p_Player->AddAura(eTerongorSpells::SpellTranscend, p_Player);
									p_Player->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
									p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
									p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
									p_Player->m_Events.AddEvent(new auchindoun_soul_transportation_event(p_Player, 7), p_Player->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
									return true;
								}
							}
						}
					}
				}
			}
		}

		return false;
	}
	/*
	struct auchindoun_teronogor_gameobject_soul_transporter_02AI : public GameObjectAI
	{
	auchindoun_teronogor_gameobject_soul_transporter_02AI(GameObject* p_GameObject) : GameObjectAI(p_GameObject) { }

	void Reset() override
	{
	if (InstanceScript* l_Instance = go->GetInstanceScript())
	{
	if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
	{
	go->SetLootState(LootState::GO_READY);
	go->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, l_Teronogor);
	}
	}
	}
	};


	GameObjectAI* GetAI(GameObject* p_GameObject) const override
	{
	return new auchindoun_teronogor_gameobject_soul_transporter_02AI(p_GameObject);
	}
	*/
};

/// Soul Transport Object 04 - 345368
class auchindoun_teronogor_gameobject_soul_transporter_04 : public GameObjectScript
{
    public:

    auchindoun_teronogor_gameobject_soul_transporter_04() : GameObjectScript("auchindoun_teronogor_gameobject_soul_transporter_04") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Gobject)
    {
        if (InstanceScript* l_Instance = p_Gobject->GetInstanceScript())
        {
            if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
            {
                if (Creature* l_Gromtash = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataGromtash)))
                {
                    if (l_Gromtash->isDead())
                    {
                        if (l_Teronogor->IsAIEnabled)
                        {
                            if (boss_teronogor::boss_teronogorAI* l_LinkAI = CAST_AI(boss_teronogor::boss_teronogorAI, l_Teronogor->GetAI()))
                            {
                                if (l_LinkAI->m_SoulTransport04)
                                {
                                    p_Player->AddAura(eTerongorSpells::SpellTranscend, p_Player);
                                    p_Player->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                    p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                    p_Player->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                                    p_Player->m_Events.AddEvent(new auchindoun_soul_transportation_event(p_Player, 10), p_Player->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }
	/*
	struct auchindoun_teronogor_gameobject_soul_transporter_02AI : public GameObjectAI
	{
	auchindoun_teronogor_gameobject_soul_transporter_02AI(GameObject* p_GameObject) : GameObjectAI(p_GameObject) { }

	void Reset() override
	{
	if (InstanceScript* l_Instance = go->GetInstanceScript())
	{
	if (Creature* l_Teronogor = l_Instance->instance->GetCreature(l_Instance->GetData64(eAuchindounDatas::DataBossTeronogor)))
	{
	go->SetLootState(LootState::GO_READY);
	go->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, l_Teronogor);
	}
	}
	}
	};


	GameObjectAI* GetAI(GameObject* p_GameObject) const override
	{
	return new auchindoun_teronogor_gameobject_soul_transporter_02AI(p_GameObject);
	}
	*/
};

void AddSC_boss_teronogor()
{
    new boss_teronogor();                                           ///< 77734
    new auchindoun_teronogor_mob_gromkash();                        ///< 77889
    new auchindoun_teronogor_mob_abyssal();                         ///< 77905
    new auchindoun_teronogor_mob_durag();                           ///< 77890
    new auchindoun_teronogor_mob_gulkosh();                         ///< 78437
    new auchindoun_teronogor_mob_shaadum();                         ///< 78728
	new auchindoun_teronogor_mob_spirit();
    new auchindoun_teronogor_spell_chaos_wave();                    ///< 157001
    new auchindoun_teronogor_spell_demonic_leap();                  ///< 148969
    new auchindoun_teronogor_spell_seed_of_malevolence();           ///< 156921
    new auchindoun_teronogor_gameobject_soul_transporter_01();      ///< 231736
    new auchindoun_teronogor_gameobject_soul_transporter_02();      ///< 345366
    new auchindoun_teronogor_gameobject_soul_transporter_03();      ///< 345367
    new auchindoun_teronogor_gameobject_soul_transporter_04();      ///< 345368
}