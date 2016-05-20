////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "SpellScript.h"
#include "Common.h"
#include "GridNotifiers.h"
#include "ScriptedCreature.h"
#include "auchindon.hpp"

/// 1st STarting Event
class EventTuulaniIntroduction : public BasicEvent
{
    public:
        explicit EventTuulaniIntroduction(Unit* unit, int value) : BasicEvent(), m_Obj(unit), m_Modifier(value)
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
                            if (Creature* l_Guard = l_Instance->instance->GetCreature(l_Instance->GetData64(eDataAuchindonDatas::DataGuard)))
                            {
                                if (l_Nyami->GetAI() && l_Tuulina->GetAI() && l_Guard->GetAI())
                                {
                                    switch (m_Modifier)
                                    {
                                        case 0: ///< TUULINA
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

                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK18);
                                            l_Tuulina->GetMotionMaster()->MovePoint(1, g_PositionTuulaniPreMovement02nd.GetPositionX(), g_PositionTuulaniPreMovement03rd.GetPositionY(), g_PositionTuulaniPreMovement03rd.GetPositionZ());
                                            break;
                                        }
                                        case 1: ///< TUULINA
                                        {
                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK3);
                                            l_Tuulina->GetMotionMaster()->MovePoint(2, g_PositionTuulaniPreMovement03rd.GetPositionX(), g_PositionTuulaniPreMovement03rd.GetPositionY(), g_PositionTuulaniPreMovement03rd.GetPositionZ());
                                            break;
                                        }
                                        case 3: ///< GUARD
                                        {
                                            l_Guard->RemoveAura(eAuchindonSpells::SpellKneel);
                                            l_Guard->AI()->Talk(eAuchindonTalk::AUCHENAIDEFENDERTALK1);
                                            l_Guard->SetFacingToObject(l_Tuulina);

                                            l_Guard->m_Events.AddEvent(new EventTuulaniIntroduction(l_Guard, 4), l_Guard->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 4: ///< TUULANI
                                        {
                                            l_Tuulina->GetMotionMaster()->MovePoint(3, g_PositionTuulaniMovement3rd.GetPositionX(), g_PositionTuulaniMovement3rd.GetPositionY(), g_PositionTuulaniMovement3rd.GetPositionZ());
                                            break;
                                        }
                                        case 5: ///< TUULANI
                                        {
                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK4);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 6), l_Tuulina->m_Events.CalculateTime(4 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 6:
                                        {
                                            l_Tuulina->CastSpell(l_Tuulina, eAuchindonSpells::SpellTuulaniUnlock);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 7), l_Tuulina->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 7:
                                        {
                                            if (GameObject* l_NearestHolyWall = l_Tuulina->FindNearestGameObject(eAuchindonObjects::GameobjectHolyWall, 60.0f))
                                            {
                                                l_NearestHolyWall->Delete();
                                            }

                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK2);
                                            l_Tuulina->GetMotionMaster()->MovePoint(4, g_PositionTuulaniMovement4th.GetPositionX(), g_PositionTuulaniMovement4th.GetPositionY(), g_PositionTuulaniMovement4th.GetPositionZ());
                                            break;
                                        }
                                        case 8:
                                        {
                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK5);
                                            l_Tuulina->GetMotionMaster()->MovePoint(5, g_PositionTuulaniMovement5th.GetPositionX(), g_PositionTuulaniMovement5th.GetPositionY(), g_PositionTuulaniMovement5th.GetPositionZ());
                                            break;
                                        }
                                        case 9:
                                        {
                                            l_Tuulina->GetMotionMaster()->MovePoint(6, g_PositionTuulaniMovement6th.GetPositionX(), g_PositionTuulaniMovement6th.GetPositionY(), g_PositionTuulaniMovement6th.GetPositionZ());
                                            break;
                                        }
                                        case 10:
                                        {
                                            l_Tuulina->GetMotionMaster()->MovePoint(7, g_PositionTuulaniMovement7th.GetPositionX(), g_PositionTuulaniMovement7th.GetPositionY(), g_PositionTuulaniMovement7th.GetPositionZ());
                                            break;
                                        }
                                        case 11:
                                        {
                                            l_Tuulina->GetMotionMaster()->MovePoint(8, g_PositionTuulaniMovement8th.GetPositionX(), g_PositionTuulaniMovement8th.GetPositionY(), g_PositionTuulaniMovement8th.GetPositionZ());
                                            break;
                                        }
                                        case 12:
                                        {
                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK6);
                                            l_Tuulina->GetMotionMaster()->MovePoint(9, g_PositionTuulaniMovement9th.GetPositionX(), g_PositionTuulaniMovement9th.GetPositionY(), g_PositionTuulaniMovement9th.GetPositionZ());
                                            break;
                                        }
                                        case 13:
                                        {
                                            l_Tuulina->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                                            l_Tuulina->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                                            l_Tuulina->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                                            l_Tuulina->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK7);

                                            l_Tuulina->GetMotionMaster()->MovePoint(10, g_PositionTuulaniMovement10th.GetPositionX(), g_PositionTuulaniMovement10th.GetPositionY(), g_PositionTuulaniMovement10th.GetPositionZ());
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 15), l_Tuulina->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 15:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK1);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 16), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 16:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK2);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 17), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 17:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK3);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 18), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 18:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK4);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 19), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 19:
                                        {
                                            l_Tuulina->AI()->Talk(eAuchindonTalk::TUULANITALK8);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 20), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                            break;
                                        }
                                        case 20:
                                        {
                                            l_Nyami->AI()->Talk(eAuchindonTalk::NYAMITALK5);
                                            l_Tuulina->m_Events.AddEvent(new EventTuulaniIntroduction(l_Tuulina, 21), l_Tuulina->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
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
        int m_Modifier;
};

/// Clapping Event
class ClappingEvent : public BasicEvent
{
    public:
        explicit ClappingEvent(Unit* unit, int value) : BasicEvent(),  m_Obj(unit), m_Modifier(value)
        {
        }

        bool Execute(uint64 /*currTime*/, uint32 /*p_Diff*/)
        {
            if (m_Obj)
            {
                if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
                {
                    switch (m_Modifier)
                    {
                        case 0:
                            if (Player* nearest = m_Obj->FindNearestPlayer(50.0f, true))
                            {
                                m_Obj->SetFacingToObject(nearest);
                                m_Obj->CastSpell(m_Obj, eAuchindonSpells::SpellApplaud);

                                m_Obj->m_Events.AddEvent(new ClappingEvent(m_Obj, 1), m_Obj->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                            }
                            break;
                        case 1:
                        {
                            m_Obj->RemoveAllAuras();
                            break;
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

/// Arcane Bomb
class ArcaneBombEvent : public BasicEvent
{
    public:
        explicit ArcaneBombEvent(Unit* unit, int value) : BasicEvent(), m_Obj(unit), m_Modifier(value)
        {
        }

        bool Execute(uint64 /*currTime*/, uint32 /*p_Diff*/)
        {
            if (m_Obj)
            {
                if (InstanceScript* m_Instance = m_Obj->GetInstanceScript())
                {
                    switch (m_Modifier)
                    {
                        case 0:
                            if (Creature* l_Nearest = m_Obj->FindNearestCreature(eAuchindonCreatures::CreatureArcaneBomb, 50.0f, true))
                            {
                                m_Obj->CastSpell(l_Nearest, eAuchindonSpells::SpellArcaneBombAreaTrigger);
                                m_Obj->m_Events.AddEvent(new ArcaneBombEvent(m_Obj, 0), m_Obj->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                            }
                        break;
                    }
                }
            }
            return true;
        }

    private:
        Unit* m_Obj;
        int m_Modifier;
};

/// Talador Portal - 236689
class gob_talador_portal : public GameObjectScript
{
    public:
        gob_talador_portal() : GameObjectScript("gob_talador_portal") { }

        bool OnGossipHello(Player* p_Player, GameObject* /*p_Gameobject*/) override
        {
            p_Player->TeleportTo(1116, 1488.52f, 3077.65f, 108.920f, 4.653427f);
            return true;
        }
};

/// Tuulani - 79248
class auchindon_creature_tuulani : public CreatureScript
{
    public:
        auchindon_creature_tuulani() : CreatureScript("auchindon_creature_tuulani") {}

        struct auchindon_creaturesAI : public ScriptedAI
        {
            auchindon_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
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
                m_FirstDiff = 4000;
            }

            void LaunchSpawning()
            {
                if (Creature* l_Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossTeronogor)))
                {
                    for (int32 i = 0; i <= 5; i++)
                    {
                        if (Creature* l_Guard = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionGuards[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Guard->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 505);
                            l_Guard->SetReactState(ReactStates::REACT_PASSIVE);
                        }
                    }

                    /// Auchindon Summon Clappers
                    for (int32 i = 0; i < 2; i++)
                    {
                        if (Creature* l_Clapper = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionGuards2nd[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Clapper->m_Events.AddEvent(new ClappingEvent(l_Clapper, 0), l_Clapper->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                            l_Clapper->SetReactState(ReactStates::REACT_PASSIVE);
                        }
                    }

                    /// Cicrular Mobs - Magus
                    for (int32 i = 0; i < 3; i++)
                    {
                        if (Creature* l_Magus = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionCircularMages[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Magus->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteHover);
                            l_Magus->SetReactState(ReactStates::REACT_PASSIVE);
                        }
                    }

                    /// Cicrular Mobs - Priest
                    for (int32 i = 0; i < 2; i++)
                    {
                        if (Creature* l_Priest = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniSoulPriest, g_PositionCircularPriests[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Priest->CastSpell(l_Priest, eAuchindonSpells::SpellKneel);
                            l_Priest->SetReactState(ReactStates::REACT_PASSIVE);
                        }
                    }

                    /// Cicrular Mobs - Cleric
                    for (int32 i = 0; i < 2; i++)
                    {
                        if (Creature* l_Cleric = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniCleric, g_PositionCircularHolies[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Cleric->CastSpell(l_Cleric, eAuchindonSpells::SpellKneel);
                            l_Cleric->SetReactState(ReactStates::REACT_PASSIVE);
                        }
                    }

                    /// Hovering Magus near preacher
                    for (int32 i = 0; i < 2; i++)
                    {
                        if (Creature* l_Magus = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionAuchenaiMagus2nd[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Magus->CastSpell(l_Magus, eAuchindonSpells::SpellKneel);
                            l_Magus->SetReactState(ReactStates::REACT_PASSIVE);
                        }
                    }

                    /// Preacher near 2 magus
                    if (Creature* l_Preacher = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniArbiter, g_PositionAuchenaiReader1st, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Preacher->CastSpell(l_Preacher, eAuchindonSpells::SpellKneel);
                        l_Preacher->SetReactState(ReactStates::REACT_PASSIVE);
                    }

                    for (int32 i = 0; i < 4; i++)
                    {
                        if (Creature* l_Vigilant = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniVigiliant, g_PositionAuchenaiVigilant[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Vigilant->CastSpell(l_Vigilant, eAuchindonSpells::SpellGuard);
                            l_Vigilant->SetReactState(ReactStates::REACT_PASSIVE);
                            l_Vigilant->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                            l_Vigilant->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                            l_Vigilant->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        }
                    }

                    for (int32 i = 0; i < 2; i++)
                    {
                        /// Two Guarding Hopilite
                        if (Creature* l_Hopilite = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniHoplite, g_PositionHopilliteGuardState[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            l_Hopilite->CastSpell(l_Hopilite, eAuchindonSpells::SpellGuard);
                            l_Hopilite->SetReactState(ReactStates::REACT_PASSIVE);
                        }
                    }

                    /// Defender that reads near two guarding hopilite
                    if (Creature* l_Defender = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionDefenderWhoReadsNearTwoHopilite, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Defender->SetReactState(ReactStates::REACT_PASSIVE);
                        l_Defender->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteRead);
                    }

                    /// Priest who meditates
                    if (Creature* l_Priest = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniSoulPriest, g_PositionSoulPriestWhoMeditates, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Priest->SetReactState(ReactStates::REACT_PASSIVE);
                        l_Priest->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteHover);
                    }

                    /// Magus who cast arcane bomb near hovering priest.
                    if (Creature* l_Mage = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionMagusWhoCastArcane, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Mage->SetReactState(ReactStates::REACT_PASSIVE);
                        l_Mage->CastSpell(l_Mage, eAuchindonSpells::SpellArcaneChanneling);
                        l_Mage->m_Events.AddEvent(new ArcaneBombEvent(l_Mage, 0), l_Mage->m_Events.CalculateTime(20 * TimeConstants::IN_MILLISECONDS));
                    }

                    /// Magus who talk to defender
                    if (Creature* l_Magus = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionMagusWhoTalksToDefender, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Magus->SetReactState(ReactStates::REACT_PASSIVE);
                        l_Magus->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteTalk);
                    }

                    /// Soul Priest who talks to defenders
                    if (Creature* l_SoulPriest = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniSoulPriest, g_PositionSoulPriestTalksToTwoDefender, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_SoulPriest->SetReactState(ReactStates::REACT_PASSIVE);
                        l_SoulPriest->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteTalk);
                    }
                }
            }

            void HandleTuulaniAtSpawn()
            {
                LaunchSpawning();

                me->setFaction(eAuchindonInformation::InformationFriendlyFaction);
                Talk(eAuchindonTalk::TUULANITALK1);
                me->SetSpeed(UnitMoveType::MOVE_RUN, 1.2f, true);
                me->SetSpeed(UnitMoveType::MOVE_WALK, 1.2f, true);
                me->GetMotionMaster()->MovePoint(0, g_PositionTuulaniPreMovement01st.GetPositionX(), g_PositionTuulaniPreMovement01st.GetPositionY(), g_PositionTuulaniPreMovement01st.GetPositionZ());

                events.ScheduleEvent(eAuchindonEvents::EventBeginRp, 4 * TimeConstants::IN_MILLISECONDS);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {
                switch (p_Id)
                {
                    case eAuchindonMovementInform::MovementInformTuulani01:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 0), me->m_Events.CalculateTime(1000));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani02:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 1), me->m_Events.CalculateTime(1000));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani03:
                        if (Creature* l_Guard = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataGuard)))
                            l_Guard->m_Events.AddEvent(new EventTuulaniIntroduction(l_Guard, 3), l_Guard->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani04:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 5), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani05:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 8), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani06:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 9), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani07:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 10), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani08:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 11), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani09:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 12), me->m_Events.CalculateTime(200));
                        break;
                    case eAuchindonMovementInform::MovementInformTuulani10:
                        me->m_Events.AddEvent(new EventTuulaniIntroduction(me, 13), me->m_Events.CalculateTime(200));
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (!m_First)
                {
                    if (m_FirstDiff <= p_Diff)
                    {
                        m_First = true;

                        HandleTuulaniAtSpawn();
                    }
                    else
                        m_FirstDiff -= p_Diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new auchindon_creaturesAI(p_Creature);
        }
};

/// Sargerei Soulbinder - 77812
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

            void EnterCombat(Unit* /*p_Attacker*/) override
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

/// Sargerei Cleric - 77134
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

            void EnterCombat(Unit* /*p_Attacker*/) override
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

/// Sargerei Ritualist - 77130
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

                if (Creature* l_Trigger = me->SummonCreature(eAuchindonCreatures::CreatureShadowBeam, g_PositionKaatharCrystalPosition, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    me->CastSpell(l_Trigger, eAuchindonSpells::SpellShadowBeam); ///< Bind triggers
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->CastStop();
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

/// Sargerei Zealot - 77132
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

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventSeverTendom, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (Creature* Kaathar = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossKathaar)))
                {
                    if (Kaathar->isAlive())
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
                    case eAuchindonEvents::EventSeverTendom:
                        if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(target, eAuchindonSpells::SpellSeverTendonAura);

                        events.ScheduleEvent(eAuchindonEvents::EventSeverTendom, urand(11 * TimeConstants::IN_MILLISECONDS, 17 * TimeConstants::IN_MILLISECONDS));
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

/// Sargerei Spirit Tender - 77131
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

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventVoidMending, 10 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eAuchindonEvents::EventVoidShift, 16 * TimeConstants::IN_MILLISECONDS);
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
                    case eAuchindonEvents::EventVoidMending:
                        if (Unit* friendUnit = DoSelectLowestHpFriendly(85)) // heal
                            me->CastSpell(friendUnit, eAuchindonSpells::SpellVoidMendingDummy);

                        events.ScheduleEvent(eAuchindonEvents::EventVoidMending, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eAuchindonEvents::EventVoidShift:
                        me->CastSpell(me, eAuchindonSpells::SpellVoidShiftDummy);
                        events.ScheduleEvent(eAuchindonEvents::EventVoidShift, 16 * TimeConstants::IN_MILLISECONDS);
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

/// Sargerei Hopilite - 77133
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

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventShieldBash, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eAuchindonEvents::EventVoidStrikes, 18 * TimeConstants::IN_MILLISECONDS);
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
                    case eAuchindonEvents::EventShieldBash:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eAuchindonSpells::SpellShieldBash);

                        events.ScheduleEvent(eAuchindonEvents::EventShieldBash, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eAuchindonEvents::EventVoidStrikes:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eAuchindonSpells::SpellVoidStrikes);

                        events.ScheduleEvent(eAuchindonEvents::EventVoidStrikes, 18 * TimeConstants::IN_MILLISECONDS);
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

/// Sargerei Defender - 77042
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

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventAvengersShield, urand(10 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eAuchindonEvents::EventCrusaderStirke, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (!UpdateVictim())
                    return;

                switch (events.ExecuteEvent())
                {
                    case eAuchindonEvents::EventAvengersShield:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(l_Random, eAuchindonSpells::SpellAvengersShield);
                            events.ScheduleEvent(eAuchindonEvents::EventAvengersShield, urand(10 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                        }
                        break;
                    case eAuchindonEvents::EventCrusaderStirke:
                        if (Unit* l_Target = me->getVictim())
                        {
                            me->CastSpell(l_Target, eAuchindonSpells::SpellCrusaderStirke);

                            events.ScheduleEvent(eAuchindonEvents::EventCrusaderStirke, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
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

/// Sargerei Magus - 76263
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


            void Reset() override
            {
                events.Reset();

                me->CastSpell(me, eAuchindonSpells::SpellArcaneChanneling);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                me->RemoveAura(eAuchindonSpells::SpellArcaneChanneling);
                events.ScheduleEvent(eAuchindonEvents::EventArcaneBomb, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));

                std::list<Creature*> l_CreaturesPrisoners;
                me->GetCreatureListWithEntryInGrid(l_CreaturesPrisoners, CreatureAucheniSoulPriest, 20.0f);

                if (l_CreaturesPrisoners.empty())
                    return;

                for (auto itr : l_CreaturesPrisoners)
                {
                    itr->RemoveAllAuras();
                    itr->DespawnOrUnsummon(3 * TimeConstants::IN_MILLISECONDS);
                    itr->SetCanFly(false);

                    itr->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
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
                    case eAuchindonEvents::EventArcaneBomb:
                        me->CastSpell(me, eAuchindonSpells::SpellArcaneBombDummy);
                        events.ScheduleEvent(eAuchindonEvents::EventArcaneBomb, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
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

/// Sargerei Soul Priest - 76595
class auchindon_creature_soul_priest : public CreatureScript
{
    public:
        auchindon_creature_soul_priest() : CreatureScript("auchindon_creature_soul_priest") { }

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

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventShadowWordPainSoulPriest, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eAuchindonEvents::EventPsychicTerrors, 15 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (!UpdateVictim())
                    return;

                switch (events.ExecuteEvent())
                {
                    case eAuchindonEvents::EventShadowWordPainSoulPriest:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true, -eAuchindonSpells::SpellShadowWordPainPriest))
                        {
                            me->CastSpell(l_Target, eAuchindonSpells::SpellShadowWordPainPriest);
                        }
                        events.ScheduleEvent(eAuchindonEvents::EventShadowWordPainSoulPriest, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eAuchindonEvents::EventPsychicTerrors:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        {
                            me->CastSpell(l_Target, eAuchindonSpells::SpellPsychicTerrorDummy);
                        }

                        events.ScheduleEvent(eAuchindonEvents::EventPsychicTerrors, 15 * TimeConstants::IN_MILLISECONDS);
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

/// Sargerei Warden - 77935
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

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventWardenHammer, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eAuchindonEvents::EventWardenChain, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (!UpdateVictim())
                    return;

                switch (events.ExecuteEvent())
                {
                    case eAuchindonEvents::EventWardenHammer:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(l_Random, eAuchindonSpells::SpellWardenThrowHammer);
                            events.ScheduleEvent(eAuchindonEvents::EventWardenHammer, urand(12 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eAuchindonEvents::EventWardenChain:
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->CastSpell(l_Random, eAuchindonSpells::SpellWardenChainDot);
                        }
                        events.ScheduleEvent(eAuchindonEvents::EventWardenChain, 20 * TimeConstants::IN_MILLISECONDS);
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

/// Felborne Abyssal - 77905
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
            uint64 m_FixatedTargetGUID;

            void Reset() override
            {
                events.Reset();

                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                m_Fixated = false;
                m_FixatedTargetGUID = NULL;
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventFixate, urand(16 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            }

            void DoAction(int32 const action) override
            {
                switch (action)
                {
                    case eAuchindonGeneralActions::ActionDeactivateFixation:
                        m_Fixated = false;
                        break;
                }
            }

            void OnAddThreat(Unit* /*p_Victim*/, float& p_fThreat, SpellSchoolMask /*p_SchoolMask*/, SpellInfo const /*p_ThreatSpell*/)
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

                if (m_Fixated)
                {
                    if (m_FixatedTargetGUID != 0)
                    {
                        if (Unit* l_Target = sObjectAccessor->GetUnit(*me, m_FixatedTargetGUID))
                        {
                            me->AddThreat(l_Target, 500.0f);
                        }
                    }
                }

                switch (events.ExecuteEvent())
                {
                case eAuchindonEvents::EventFixate:
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        m_FixatedTargetGUID = l_Random->GetGUID();

                        me->CastSpell(l_Random, eAuchindonSpells::SpellFixate);
                        m_Fixated = true;

                        events.ScheduleEvent(eAuchindonEvents::EventFixate, urand(16 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
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

/// Cackling Pyrmoaniac - 76260
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
                events.Reset();

                me->CastSpell(me, eAuchindonSpells::SpellAbyssalVisual);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventFelblast, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->isAlive())
                        l_Azzakel->GetAI()->DoAction(eAuchindonGeneralActions::ActionDemonSoulsAchievement);

                if (Creature* l_Trigger = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataTriggerAzzakelController)))
                    if (l_Trigger->IsWithinDistInMap(me, 30.0f) && l_Trigger->GetAI())
                        l_Trigger->AI()->DoAction(eAuchindonGeneralActions::ActionCountPre3StBossKill);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (!UpdateVictim())
                    return;

                switch (events.ExecuteEvent())
                {
                    case eAuchindonEvents::EventFelblast:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eAuchindonSpells::SpellFelblast);

                        events.ScheduleEvent(eAuchindonEvents::EventFelblast, 10 * TimeConstants::IN_MILLISECONDS);
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

/// Blazing Trickster - 79511
class auchindon_creature_blazing_trickster : public CreatureScript
{
    public:
        auchindon_creature_blazing_trickster() : CreatureScript("auchindon_creature_blazing_trickster") { }

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

                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->SetDefaultMovementType(MovementGeneratorType::RANDOM_MOTION_TYPE);

                events.ScheduleEvent(eAuchindonEvents::EventConfligrate, urand(8 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->isAlive())
                        l_Azzakel->GetAI()->DoAction(eAuchindonGeneralActions::ActionDemonSoulsAchievement);
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (!UpdateVictim())
                    return;

                switch (events.ExecuteEvent())
                {
                case eAuchindonEvents::EventConfligrate:
                    me->CastSpell(me, eAuchindonSpells::SpellConfligirate);
                    events.ScheduleEvent(eAuchindonEvents::EventConfligrate, urand(8 * TimeConstants::IN_MILLISECONDS, 16 * TimeConstants::IN_MILLISECONDS));
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

/// Felguard
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

            void Reset() override
            {
                events.Reset();
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                events.ScheduleEvent(eAuchindonEvents::EventFelStomp, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (Creature* l_Azzakel = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossAzzakael)))
                    if (l_Azzakel->isInCombat() && l_Azzakel->isAlive())
                        l_Azzakel->GetAI()->DoAction(eAuchindonGeneralActions::ActionDemonSoulsAchievement);

                if (Creature* l_Trigger = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataTriggerAzzakelController)))
                    if (l_Trigger->IsWithinDistInMap(me, 30.0f) && l_Trigger->GetAI())
                        l_Trigger->AI()->DoAction(eAuchindonGeneralActions::ActionCountPre3StBossKill);
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
                        events.ScheduleEvent(eAuchindonEvents::EventFelStomp, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
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

/// Warden Hammer
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

/// Void Mending - 154623
class auchindon_spell_void_mending : public SpellScriptLoader
{
    public:
        auchindon_spell_void_mending() : SpellScriptLoader("auchindon_spell_void_mending") { }

        class everbloom_spells : public SpellScript
        {
            PrepareSpellScript(everbloom_spells)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        l_Caster->AddAura(eAuchindonSpells::SpellVoidMendingAura, l_Target);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new everbloom_spells();
        }
};

/// Psychic Terrors - 154356
class auchindon_spell_psychic_terror : public SpellScriptLoader
{
    public:
        auchindon_spell_psychic_terror() : SpellScriptLoader("auchindon_spell_psychic_terror") { }

        class everbloom_spells : public SpellScript
        {
            PrepareSpellScript(everbloom_spells)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        std::list<Player*> l_ListPlayers;
                        l_Caster->GetPlayerListInGrid(l_ListPlayers, 4.0f);

                        if (l_ListPlayers.empty())
                            return;

                        for (auto itr : l_ListPlayers)
                        {
                            itr->AddAura(eAuchindonSpells::SpellPsychicTerrorFear, itr);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
    };

    SpellScript* GetSpellScript() const override
    {
        return new everbloom_spells();
    }
};

/// Warden's Chain - 154683
class auchindon_spell_warden_chain : public SpellScriptLoader
{
    public:
        auchindon_spell_warden_chain() : SpellScriptLoader("auchindon_spell_warden_chain") { }

        class auchindon_spells : public SpellScript
        {
            PrepareSpellScript(auchindon_spells)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetExplTargetUnit())
                    {
                        l_Caster->AddAura(eAuchindonSpells::SpellWardenChainJump, l_Target);
                        l_Caster->AddAura(eAuchindonSpells::SpellWardenChainDot, l_Target);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(auchindon_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new auchindon_spells();
        }
};

/// Warden Chain Aura - 154831
class auchindon_warden_chain_aura : public SpellScriptLoader
{
    public:
        auchindon_warden_chain_aura() : SpellScriptLoader("auchindon_warden_chain_aura") { }

        class auchindon_auras : public AuraScript
        {
            PrepareAuraScript(auchindon_auras)

            void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Target->AddAura(SpellWardenChainRoot, l_Target);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_auras::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new auchindon_auras();
        }
};

/// Void Shift
class auchindoun_void_shift : public SpellScriptLoader
{
    public:
        auchindoun_void_shift() : SpellScriptLoader("auchindoun_void_shift") { }

        class auchindoun_spells : public SpellScript
        {
            PrepareSpellScript(auchindoun_spells)

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                Unit* l_Caster = GetCaster();
                std::list<Unit*> l_ListPlayers;
                int32 l_CalcDamage = 8000;

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
                        GetCaster()->CastCustomSpell((*it), eAuchindonSpells::SpellVoidShiftDamage, &l_CalcDamage, NULL, NULL, true, NULL);
                    }
                    else
                    {
                        GetCaster()->CastCustomSpell((*it), eAuchindonSpells::SpellVoidShiftHeal, &l_CalcDamage, NULL, NULL, true, NULL);
                    }
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(auchindoun_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
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
            PrepareAuraScript(auchindon_spells)

            bool Load()
            {
                SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
                spell->Effects[0].TriggerSpell = eAuchindonSpells::SpellArcaneBombTriggerMissile;
                return true;
            }

            void HandlePeriodic(AuraEffect const* /*aurEff*/)
            {
                /// Nothing yet.
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_spells::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new auchindon_spells();
        }
    };

/// Void Shell
class auchindon_spell_void_shell_filter : public SpellScriptLoader
{
    public:
        auchindon_spell_void_shell_filter() : SpellScriptLoader("auchindon_spell_void_shell_filter") { }

        class spells_auchindon : public SpellScript
        {
            PrepareSpellScript(spells_auchindon)

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
                        if (itr->GetTypeId() == TypeID::TYPEID_PLAYER)
                            return;

                        if (itr && itr->IsInWorld())
                            targets.push_back(itr);
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spells_auchindon();
        }
};

/// Fixate
class auchindon_spell_fixate : public SpellScriptLoader
{
    public:
        auchindon_spell_fixate() : SpellScriptLoader("auchindon_spell_fixate") { }

        class spell_auchindons : public AuraScript
        {
            PrepareAuraScript(spell_auchindons)

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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
                switch (m_scriptSpellId)
                {
                    case 157168:
                        AfterEffectApply += AuraEffectApplyFn(spell_auchindons::OnApply, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_MOD_POSSESS_PET, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
                        AfterEffectRemove += AuraEffectRemoveFn(spell_auchindons::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_MOD_POSSESS_PET, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_auchindons();
        }
};

/// Tuulani Unlock Gate
class auchindon_spell_tuulani_unlock : public SpellScriptLoader
{
    public:
        auchindon_spell_tuulani_unlock() : SpellScriptLoader("auchindon_spell_tuulani_unlock") { }

        class auchindon_spells : public SpellScript
        {
            PrepareSpellScript(auchindon_spells)

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                /// Clears all targets at start, fetching new ones
                targets.clear();

                std::list<Creature*> l_ListTriggerWall;
                GetCaster()->GetCreatureListWithEntryInGrid(l_ListTriggerWall, eAuchindonCreatures::CreatureLightWallTargets, 15.0f);

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
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(auchindon_spells::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new auchindon_spells();
        }
};

/// 150905, 158565
class auchindon_spell_beam_cosmetic : public SpellScriptLoader
{
    public:
        auchindon_spell_beam_cosmetic() : SpellScriptLoader("auchindon_spell_beam_cosmetic") { }

        class spells_auchindon : public SpellScript
        {
            PrepareSpellScript(spells_auchindon)

            bool Load()
            {
                /*
                SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
                spell->Effects[0].TargetA = Targets::TARGET_UNIT_TARGET_ANY;
                spell->Effects[0].TargetB = 0;
                spell->AttributesEx9 = 0;
                spell->AttributesEx6 = 0;
                spell->AttributesEx5 = 0;
                spell->AttributesEx4 = 0;
                return true;
                */

                return true;
            }

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                targets.clear();

                if (Creature* l_Trigger = GetCaster()->FindNearestCreature(eAuchindonCreatures::CreatureShadowBeam, 200.0f, true))
                    targets.push_back(l_Trigger);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, SpellEffIndex::EFFECT_0, Targets::TARGET_UNIT_NEARBY_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spells_auchindon();
        }
};

/// Arcane Bomb Trigger
class auchindon_trigger_arcane_bomb_npc_explosion : public CreatureScript
{
    public:
        auchindon_trigger_arcane_bomb_npc_explosion() : CreatureScript("auchindon_trigger_arcane_bomb_npc_explosion") { }

        struct auchinond_creaturesAI : public Scripted_NoMovementAI
        {
            auchinond_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                /*
                Reset();
                me->Respawn(true);
                */
            }

            void Reset() override
            {
                me->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(eAuchindonInformation::InformationHostileFaction);
                me->CastSpell(me, eAuchindonSpells::SpellArcaneBombDmg, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new auchinond_creaturesAI(p_Creature);
        }
};

/// Arcane Bomb trigger
class auchindon_areatrigger_arcane_bomb : public AreaTriggerEntityScript
{
    public:
        auchindon_areatrigger_arcane_bomb() : AreaTriggerEntityScript("auchindon_areatrigger_arcane_bomb")
        {
        }

        uint32 m_Diff = 9 * TimeConstants::IN_MILLISECONDS;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Diff <= p_Time)
            {
                Position l_Pos;
                p_AreaTrigger->GetPosition(&l_Pos);

                p_AreaTrigger->SummonCreature(eAuchindonCreatures::CreatureArcaneBombExplosionNpc, l_Pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                m_Diff = 20 * TimeConstants::IN_MILLISECONDS;
            }
            else
            {
                m_Diff -= p_Time;
            }
        }

        void OnRemove(AreaTrigger* /*p_AreaTrigger*/, uint32 /*p_Time*/) override
        {
            /// Does nothing.
        }

        auchindon_areatrigger_arcane_bomb* GetAI() const override
        {
            return new auchindon_areatrigger_arcane_bomb();
        }
};

#ifndef __clang_analyzer__
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
    new auchindon_creature_soul_priest();
    new auchindon_creature_felguard();
    new auchindon_creature_cackling_pyromaniac();
    new auchindon_creature_blazing_trickster();
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
    new auchindon_spell_psychic_terror();
    new auchindon_spell_fixate();
    new auchindon_spell_tuulani_unlock();
    new auchindon_spell_beam_cosmetic();
    new gob_talador_portal();
}
#endif
