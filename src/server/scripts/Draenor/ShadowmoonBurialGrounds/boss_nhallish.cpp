////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "shadowmoon_burial_grounds.hpp"

enum eNhalishSpells
{
    SpellSoulless                            = 154947,
    SpellRegainYourSoul                      = 153007,
    SpellSoulShred                           = 152979,
    SpellShadowChannel                       = 46757,
    SpellLootSparkles                        = 92376,
    SpellCloneMe                             = 45204
};

enum eNhalishCreatures
{
   CreatureDistrubedSouls     = 76400,
   CreatureSoul               = 75899,
   CreatureNhalishProp        = 432561,
   CreatureDevestationTrigger = 543531 /// Hardcoded
};

enum eNhalishGameObjects
{
    GameObjectNhalishDoor     = 227851,
    GameObjectNhalishEntrance = 227852
};

enum eNhallishTalks
{
    TalkAggro = 1, ///< Yes... come closer, your souls are strong and proud! I will feast on your terror.. [43197]
    TalkDeath,     ///< You are just maggots.. sworm into a corpse of a dying god! [43198]
    TalkIntro01,   ///< Cower before me! [43199]
    TalkIntro02,   ///< Your undying souls will fuel my ascention from the Darkness! [43200]
    TalkIntro03,   ///< Your blisper afterlife is over! [43201]
    TalkKill01,    ///< Your mortal body are so fragile! [43202]
    TalkKill02,    ///< Another soul to DEVOUR! [43203]
    TalkSpell01,   ///< None escape the void! [43204]
    TalkSpell02,   ///< You cannot flee! [43205]
    TalkSpell03,   ///< *Nhallish laughs* [43206]
    TalkSpell04    ///< Flee... flee from your your formal masters! [43207]
};

enum eNhalishActions
{
    ActionActivateVortex = 1,
    ActionDeactivateVortex
};

/// Nhalish - 75829
class boss_nhalish : public CreatureScript
{
public:

    boss_nhalish() : CreatureScript("boss_nhalish") { }

    struct boss_nhalishAI : public BossAI
    {
        boss_nhalishAI(Creature* p_Creature) : BossAI(p_Creature, eShadowmoonBurialGroundsDatas::DataBossNhallish)
        {
            m_Instance = me->GetInstanceScript();
            m_First = false;
        }

        enum eNhalishSpells
        {
            SpellPlanarShift                          = 153623,
            SpellTeleportSelf                         = 153621,
            SpellVoidVortexDamage                     = 152800,
            SpellVoidVortexAreaTrigger                = 152801,
            SpellVoidBlastSpellAuraDummy              = 152792,
            SpellVoidBlastDot                         = 153501,
            SpellVoidDevastationPeriodicTriggerSpell  = 153067,
            SpellVoidDevstationProjectile             = 153068,
            SpellVoidDevestationDebuff                = 153070l,
            SpellVoidDevstationAreaTrigger            = 153072,
            SpellSoulStealForceCast                   = 152962,
            SpellSoulStealPeriodicDummy               = 156755,
            SpellSoulless                             = 154947,
            SpellRegainYourSoul                       = 153007,
            SpellSoulShred                            = 152979,
            SpellShadowChannel                        = 46757,
            SpellLootSparkles                         = 92376,
            SpellCloneMe                              = 45204
        };

        enum eNhalishEvents
        {
            EventPlanarShift = 1,
            EventVoidVortex,
            EventVoidBlast,
            EventVoidDevastation,
            EventSoulSteal,
            EventExhumeCrypt
        };

        InstanceScript* m_Instance;
        uint32 m_PhaseCounter;
        uint32 m_VortexDiff;
        bool m_Vortex;
        bool m_First;

        void Reset() override
        {
            _Reset();
            events.Reset();
            m_Vortex = false;
            m_VortexDiff = 0;
            m_PhaseCounter = 0;
            me->CastSpell(me, eNhalishSpells::SpellShadowChannel);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            HandleDoorEntranceActivation();
            if (!m_First)
            {
                HandleDoorActivation();
                m_First = true;
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();
            DespawnCreaturesInArea(eShadowmoonBurialGroundsCreatures::CreatureExhumeSpirit, me);
            if (m_Instance != nullptr)
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNhallish, EncounterState::FAIL);
            /// Adding back to Visibility List
            std::list<Player*> l_ListVisibility;
            me->GetPlayerListInGrid(l_ListVisibility, 300.0f);
            if (l_ListVisibility.empty())
                return;

            for (Player* l_Itr : l_ListVisibility)
                l_Itr->SetPhaseMask(1, true);
        }


        void HandleDoorActivation() /// Only upon boss Defeat
        {
            std::list<GameObject*> l_ListDoors;
            me->GetGameObjectListWithEntryInGrid(l_ListDoors, eNhalishGameObjects::GameObjectNhalishDoor, 300.0f);
            if (l_ListDoors.empty())
                return;

            for (GameObject* l_Itr : l_ListDoors)
            {
                /// Activate
                l_Itr->SetLootState(LootState::GO_READY);
                l_Itr->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
            }
        }

        void HandleDoorEntranceActivation() /// Upon wipe
        {
            /*
            std::list<GameObject*> l_ListDoors;
            me->GetGameObjectListWithEntryInGrid(l_ListDoors, eNhalishGameObjects::GameObjectNhalishEntrance, 300.0f);
            if (l_ListDoors.empty())
                return;

            for (GameObject* l_Itr : l_ListDoors)
            {
                if (!l_Itr)
                    continue;

                l_Itr->Delete();
            }

            if (!p_Status)
            {
                me->SummonGameObject();
            }
            */
        }

        void JustSummoned(Creature* p_Summon) override
        {
            if (p_Summon)
            {
                switch (p_Summon->GetEntry())
                {
                    case eNhalishCreatures::CreatureDistrubedSouls:
                        p_Summon->CastSpell(p_Summon, eShadowmoonBurialGroundsSpells::SpellVortexVisual);
                        break;
                    default:
                        break;
                }
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eNhalishActions::ActionActivateVortex:
                    m_Vortex = true;
                    m_VortexDiff = 2 * TimeConstants::IN_MILLISECONDS;
                    break;
                case eNhalishActions::ActionDeactivateVortex:
                    m_Vortex = false;
                    if (m_Instance != nullptr)
                        m_Instance->DoRemoveForcedMovementsOnPlayers(me->GetGUID());
                    break;
                default:
                    break;
            }
        }

        void EnterCombat(Unit* /*p_Who*/) override
        {
            _EnterCombat();
            Talk(eNhallishTalks::TalkAggro);
            HandleDoorEntranceActivation();
            events.ScheduleEvent(eNhalishEvents::EventPlanarShift, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventSoulSteal, 70 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventVoidBlast, 4 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventExhumeCrypt, 7 * TimeConstants::IN_MILLISECONDS);
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNhallish, EncounterState::IN_PROGRESS);
            }
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eNhallishTalks::TalkKill01);
                else
                    Talk(eNhallishTalks::TalkKill02);
                p_Who->SetPhaseMask(me->GetPhaseMask(), true); /// Sets dead target back to normal phase.
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            HandleDoorActivation();
            HandleDoorEntranceActivation();
            Talk(eNhallishTalks::TalkDeath);
            /// Adding back to Visibility List
            std::list<Player*> l_ListVisibility;
            me->GetPlayerListInGrid(l_ListVisibility, 300.0f);
            if (l_ListVisibility.empty())
                return;

            for (Player* l_Itr : l_ListVisibility)
                l_Itr->SetPhaseMask(1, true);
            /// Distrubed Spirits RP moment.
            for (uint8 l_I = 0; l_I < 10; l_I++)
            {
              if (Creature* l_Creature = me->SummonCreature(eNhalishCreatures::CreatureDistrubedSouls, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10 * TimeConstants::IN_MILLISECONDS))
               {
                   l_Creature->setFaction(FriendlyFaction);
                   l_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                   l_Creature->GetMotionMaster()->MoveRandom(15.0f);
                   if (roll_chance_i(25))
                   {
                       switch (urand(0, 4))
                       {
                           case 0:
                               l_Creature->MonsterSay("Thank you.", Language::LANG_UNIVERSAL, me->GetGUID());
                               break;
                           case 1:
                               l_Creature->MonsterSay("My torment is over!", Language::LANG_UNIVERSAL, me->GetGUID());
                               break;
                           default:
                               break;
                       }
                   }
               }
            }
            if (m_Instance != nullptr)
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNhallish, EncounterState::DONE);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            // Reset phase Counter
            if (m_PhaseCounter >= 4)
                m_PhaseCounter = 0;

            if (m_Vortex)
            {
                if (m_VortexDiff <= p_Diff)
                {
                    std::list<Player*> l_ListPlayers;
                    me->GetPlayerListInGrid(l_ListPlayers, 25.0f);
                    if (l_ListPlayers.empty())
                        return;

                    Position l_Position;
                    me->GetPosition(&l_Position);
                    for (Player* l_Itr : l_ListPlayers)
                    {
                        if (l_Itr->IsWithinDist(me, 20.0f, true))
                        {
                            if (l_Itr->isAlive() && !l_Itr->HasMovementForce(me->GetGUID()))
                                l_Itr->SendApplyMovementForce(me->GetGUID(), true, l_Position, 3.0f, 1);
                            else if (!l_Itr->isAlive() && l_Itr->HasMovementForce(me->GetGUID()))
                                l_Itr->SendApplyMovementForce(me->GetGUID(), false, l_Position);
                        }
                        else if (l_Itr->HasMovementForce(me->GetGUID()))
                            l_Itr->SendApplyMovementForce(me->GetGUID(), false, l_Position);
                    }

                    m_VortexDiff = 2 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_VortexDiff -= p_Diff;
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eNhalishEvents::EventExhumeCrypt:
                    me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellExhumeTheCryptsScriptEffect, true);
                    events.ScheduleEvent(eNhalishEvents::EventExhumeCrypt, urand(8 * TimeConstants::IN_MILLISECONDS, 40 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eNhalishEvents::EventPlanarShift:
                    events.CancelEvent(eNhalishEvents::EventVoidDevastation);
                    events.CancelEvent(eNhalishEvents::EventVoidVortex);
                    me->CastSpell(me, eNhalishSpells::SpellPlanarShift);
                    Talk(eNhallishTalks::TalkSpell03);
                    switch (urand(0, 1))
                    {
                        case 0:
                            events.ScheduleEvent(eNhalishEvents::EventVoidDevastation, 5 * TimeConstants::IN_MILLISECONDS);
                            break;
                        case 1:
                            events.ScheduleEvent(eNhalishEvents::EventVoidVortex, 5 * TimeConstants::IN_MILLISECONDS);
                            break;
                        default:
                            break;
                    }
                    events.ScheduleEvent(eNhalishEvents::EventPlanarShift, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eNhalishEvents::EventVoidDevastation:
                    Talk(eNhallishTalks::TalkSpell02);
                    me->CastSpell(me, eNhalishSpells::SpellVoidDevastationPeriodicTriggerSpell);
                    events.ScheduleEvent(eNhalishEvents::EventVoidDevastation, 22 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eNhalishEvents::EventSoulSteal:
                    Talk(eNhallishTalks::TalkSpell04);
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Random, eNhalishSpells::SpellSoulStealForceCast);
                    events.ScheduleEvent(eNhalishEvents::EventSoulSteal, 65 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eNhalishEvents::EventVoidBlast:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eNhalishSpells::SpellVoidBlastSpellAuraDummy);
                    events.ScheduleEvent(eNhalishEvents::EventVoidBlast, 12 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eNhalishEvents::EventVoidVortex:
                    Talk(eNhallishTalks::TalkSpell01);
                    me->CastSpell(me, eNhalishSpells::SpellVoidVortexAreaTrigger);
                    events.ScheduleEvent(eNhalishEvents::EventVoidVortex, 35 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_nhalishAI(p_Creature);
    }
};

/// Possessed Soul - 75899
class shadowmoon_burial_grounds_nhalish_creature_soul : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nhalish_creature_soul() : CreatureScript("shadowmoon_burial_grounds_nhalish_creature_soul") { }

    enum ePossessedSoulSpells
    {
        SpellSoulless                    = 154947,
        SpellRegainYourSoul              = 153007,
        SpellSoulShred                   = 152979,
        SpellShadowChannel               = 46757,
        SpellLootSparkles                = 92376,
        SpellCloneMe                     = 45204,
        SpellUnortodoxExistanceAuraDummy = 152976
    };

    enum ePossessedSoulEvents
    {
        EventSoulSteal = 1,
        EventRandomMovement,
        EventExhumeCrypt
    };

    enum ePossessedSoulDisplays
    {
        DisplayBanshee = 9534
    };

    enum ePossessedSoulActions
    {
        ActionTimeout = 1
    };

    bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
    {
        if (p_Creature)
        {
            if (InstanceScript* l_Instance = p_Creature->GetInstanceScript())
            {
                if (Creature* l_Nhalish = l_Instance->instance->GetCreature(l_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNhallish)))
                {
                    if (shadowmoon_burial_grounds_nhalish_creature_soul::shadowmoon_burial_grounds_nhalish_creature_soulAI* l_LinkAI = CAST_AI(shadowmoon_burial_grounds_nhalish_creature_soul::shadowmoon_burial_grounds_nhalish_creature_soulAI, l_Nhalish->GetAI()))
                    {
                        if (l_LinkAI && l_LinkAI->m_HasDied)
                        {
                            p_Player->RemoveAura(ePossessedSoulSpells::SpellSoulless);
                            /// Returned Soul
                            if (p_Player->HasAura(ePossessedSoulSpells::SpellSoulShred))
                            {
                                p_Player->CastSpell(p_Player, ePossessedSoulSpells::SpellRegainYourSoul, true);
                                p_Player->RemoveAura(ePossessedSoulSpells::SpellSoulShred);
                            }
                            p_Player->SetPhaseMask(1, true);
                            p_Creature->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                        }
                    }
                }
            }
        }
        return true;
    }

    struct shadowmoon_burial_grounds_nhalish_creature_soulAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_nhalish_creature_soulAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_VictimGUID(0)
        {
            m_HasDied = false;
        }

        uint64 m_VictimGUID;
        bool m_HasDiedBeforeTimeout;
        bool m_HasDied;

        void Reset() override
        {
            events.Reset();
            me->SetReactState(ReactStates::REACT_PASSIVE);
            if (m_VictimGUID)
            {
                if (Unit* l_Victim = Unit::GetUnit(*me, m_VictimGUID))
                {
                    me->CastSpell(l_Victim, ePossessedSoulSpells::SpellSoulShred); // automatically target the summoner TARGET_UNIT_SUMMONER
                    me->CastSpell(me, ePossessedSoulSpells::SpellUnortodoxExistanceAuraDummy);
                }
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case ePossessedSoulActions::ActionTimeout:
                    m_HasDiedBeforeTimeout = true;
                    break;
                default:
                    break;
            }
        }

        void SetGUID(uint64 p_Guid, int32 /*p_Param*/) override
        {
            m_VictimGUID = p_Guid;
            events.ScheduleEvent(ePossessedSoulEvents::EventRandomMovement, 1 * TimeConstants::IN_MILLISECONDS);
            if (m_VictimGUID)
            {
                if (Unit* l_Victim = Unit::GetUnit(*me, m_VictimGUID))
                {
                    l_Victim->CastSpell(me, ePossessedSoulSpells::SpellCloneMe);
                    me->CastSpell(l_Victim, ePossessedSoulSpells::SpellSoulShred);

                    AddTimedDelayedOperation(20 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        DoAction(ePossessedSoulActions::ActionTimeout);
                    });
                }
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            me->Respawn();
            events.Reset();
            m_HasDied = true;
            me->RemoveAllAuras();
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetDisplayId(ePossessedSoulDisplays::DisplayBanshee);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN | eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
            
            if (!m_HasDiedBeforeTimeout)
            {
                if (m_VictimGUID)
                {
                    if (Unit* l_Victim = Unit::GetUnit(*me, m_VictimGUID))
                        l_Victim->AddAura(ePossessedSoulSpells::SpellRegainYourSoul, l_Victim);
                }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case ePossessedSoulEvents::EventRandomMovement:
                       me->GetMotionMaster()->MoveRandom(4.0f);
                    break;
                   default:
                       break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nhalish_creature_soulAI(p_Creature);
    }
};

/// Nhalish Prop - 75442
class shadowmoon_burial_grounds_nhalish_creature_nhalish_prop : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nhalish_creature_nhalish_prop() : CreatureScript("shadowmoon_burial_grounds_nhalish_creature_nhalish_prop") { }

    struct shadowmoon_burial_grounds_nhalish_creature_nhalish_propAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_nhalish_creature_nhalish_propAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->CastSpell(me, eNhalishSpells::SpellShadowChannel);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void UpdateAI(uint32 const /*p_Diff*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Nhalish = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNhallish)))
                {
                    if (l_Nhalish->isAlive() && l_Nhalish->isInCombat())
                        me->SetHealth((uint32)l_Nhalish->GetHealthPct());
                }
            }
        }
    };
     
    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nhalish_creature_nhalish_propAI(p_Creature);
    }
};

/// Void Devestation - 543531 // Hacked, hardcoded
class shadowmoon_burial_grounds_nhalish_creature_void_devestation : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nhalish_creature_void_devestation() : CreatureScript("shadowmoon_burial_grounds_nhalish_creature_void_devestation") { }

    struct shadowmoon_burial_grounds_nhalish_creature_void_devestationAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_nhalish_creature_void_devestationAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) {}

        enum eVoidDevestationSpells
        {
            SpellVoidDevestationDebuff = 153070
        };

        InstanceScript* m_Instance;
        uint32 m_Timer;

        void Reset() override
        {
            me->setFaction(FriendlyFaction);
            me->SetDisplayId(InvisibleDisplay);
            m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (m_Timer <= p_Diff)
            {
                std::list<Player*> l_PlayerList;
                JadeCore::AnyPlayerInObjectRangeCheck check(me, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, l_PlayerList, check);
                me->VisitNearbyObject(2.0f, searcher);
                if (!l_PlayerList.empty())
                {
                    for (std::list<Player*>::const_iterator itr = l_PlayerList.begin(); itr != l_PlayerList.end(); ++itr)
                    {
                        if (!(*itr)->HasAura(eVoidDevestationSpells::SpellVoidDevestationDebuff))
                            (*itr)->CastSpell((*itr), eVoidDevestationSpells::SpellVoidDevestationDebuff);
                    }
                }

                m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Timer -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nhalish_creature_void_devestationAI(p_Creature);
    }
};

/// Void Devestation - 153068 (TARGET_DEST_CASTER, TARGET_DEST_DEST_RADIUS)
class shadowmoon_burial_grounds_nhalish_spell_void_devestation : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_nhalish_spell_void_devestation() : SpellScriptLoader("shadowmoon_burial_grounds_nhalish_spell_void_devestation") { }

    class shadowmoon_burial_grounds_nhalish_spell_void_devestation_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_nhalish_spell_void_devestation_SpellScript)

        enum eVoidDevestationSpells
        {
            SpellVoidDevstationAreaTrigger = 153072
        };

        void HandleTriggerMissile(SpellEffIndex p_EffIndex)
        {
            PreventHitDefaultEffect(p_EffIndex);
            if (!GetCaster())
                return;

            GetCaster()->MonsterSay("hoomos", LANG_UNIVERSAL, GetCaster()->GetGUID());
   
            if (Position const* l_Pos = GetExplTargetDest())
                GetCaster()->SummonCreature(eNhalishCreatures::CreatureDevestationTrigger, l_Pos->GetPositionX(), l_Pos->GetPositionY(), l_Pos->GetPositionZ(), l_Pos->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 4 * TimeConstants::IN_MILLISECONDS);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(shadowmoon_burial_grounds_nhalish_spell_void_devestation_SpellScript::HandleTriggerMissile, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_TRIGGER_MISSILE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_nhalish_spell_void_devestation_SpellScript();
    }
};

/// Void Blast - 152792
class shadowmoon_burial_grounds_nhalish_spell_void_blast : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_nhalish_spell_void_blast() : SpellScriptLoader("shadowmoon_burial_grounds_nhalish_spell_void_blast") { }

    class shadowmoon_burial_grounds_nhalish_spell_void_blast_SpellScript : public AuraScript
    {
        PrepareAuraScript(shadowmoon_burial_grounds_nhalish_spell_void_blast_SpellScript)

        enum eVoidBlastSpells
        {
            SpellVoidBlastDot = 153501
        };

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->IsAIEnabled)
                {
                    if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        l_Caster->CastSpell(l_Target, eVoidBlastSpells::SpellVoidBlastDot);
                }
            }
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                        l_Instance->DoRemoveAurasDueToSpellOnPlayers(eVoidBlastSpells::SpellVoidBlastDot);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(shadowmoon_burial_grounds_nhalish_spell_void_blast_SpellScript::OnApply,    SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(shadowmoon_burial_grounds_nhalish_spell_void_blast_SpellScript::OnRemove, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new shadowmoon_burial_grounds_nhalish_spell_void_blast_SpellScript();
    }
};

/// Void Vortex - 152801
class shadowmoon_burial_grounds_nhalish_spell_void_vortex : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_nhalish_spell_void_vortex() : SpellScriptLoader("shadowmoon_burial_grounds_nhalish_spell_void_vortex") { }

    class shadowmoon_burial_grounds_nhalish_spell_void_vortex_SpellScript : public AuraScript
    {
        PrepareAuraScript(shadowmoon_burial_grounds_nhalish_spell_void_vortex_SpellScript)

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                {
                    if (l_Caster->IsAIEnabled)
                        l_Caster->GetAI()->DoAction(eNhalishActions::ActionActivateVortex);
                }
            }
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (l_Target->IsAIEnabled)
                    l_Caster->GetAI()->DoAction(eNhalishActions::ActionDeactivateVortex);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(shadowmoon_burial_grounds_nhalish_spell_void_vortex_SpellScript::OnApply,    SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(shadowmoon_burial_grounds_nhalish_spell_void_vortex_SpellScript::OnRemove, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new shadowmoon_burial_grounds_nhalish_spell_void_vortex_SpellScript();
    }
};

/// Soul Steal - 152962
class shadowmoon_burial_grounds_nhalish_spell_soul_steal : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_nhalish_spell_soul_steal() : SpellScriptLoader("shadowmoon_burial_grounds_nhalish_spell_soul_steal") {}

    class shadowmoon_burial_grounds_nhalish_spell_soul_steal_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_nhalish_spell_soul_steal_SpellScript)

        enum eSoulStealSpells
        {
            SpellSoulless = 15497
        };

        void HandleForceCast(SpellEffIndex /*p_EffIndex*/)
        {
            if (!GetCaster())
                return;

            if (!GetHitUnit() && GetHitUnit()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            /// Soulless
            GetHitUnit()->CastSpell(GetHitUnit(), eSoulStealSpells::SpellSoulless);
            /// Caster is boss, but doing it with pointer to prevent a crash *twerks*
            if (InstanceScript* m_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Nhalish = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNhallish)))
                {
                    if (Creature* Soul = GetCaster()->SummonCreature(eNhalishCreatures::CreatureSoul, GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), GetHitUnit()->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN, 500 * TimeConstants::IN_MILLISECONDS, GetHitUnit()->GetGUID()))
                    {
                        if (boss_nhalish::boss_nhalishAI* l_LinkAI = CAST_AI(boss_nhalish::boss_nhalishAI, l_Nhalish->GetAI()))
                        {
                            uint32 m_PhaseId = 0;

                            switch (l_LinkAI->m_PhaseCounter)
                            {
                                case 0:
                                    m_PhaseId = 2;
                                    break;
                                case 1:
                                    m_PhaseId = 4;
                                    break;
                                case 2:
                                    m_PhaseId = 6;
                                    break;
                                case 3:
                                    m_PhaseId = 8;
                                    break;
                                case 4:
                                    m_PhaseId = 10;
                                    break;
                                case 5:
                                    m_PhaseId = 12;
                                    break;
                                default:
                                    break;
                            }

                            /// Sets new phase value
                            GetHitUnit()->SetPhaseMask(m_PhaseId, true);
                            Soul->SetPhaseMask(m_PhaseId, true);
                            l_LinkAI->m_PhaseCounter++;
                            if (Soul->IsAIEnabled)
                                Soul->AI()->SetGUID(GetHitUnit()->GetGUID(), 0);
                        }
                    }
                }
            }
        }
  
        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(shadowmoon_burial_grounds_nhalish_spell_soul_steal_SpellScript::HandleForceCast, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_nhalish_spell_soul_steal_SpellScript();
    }
};

/// Planar Shift - 153623
class shadowmoon_burial_grounds_nhalish_spell_planar_shift : public SpellScriptLoader
{
public:

    shadowmoon_burial_grounds_nhalish_spell_planar_shift() : SpellScriptLoader("shadowmoon_burial_grounds_nhalish_spell_planar_shift") {}

    class shadowmoon_burial_grounds_nhalish_spell_planar_shift_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_nhalish_spell_planar_shift_SpellScript)

        enum ePlanarShiftSpells
        {
            SpellTeleportSelf = 153621
        };

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            if (!GetCaster()->IsAIEnabled)
                return;

            if (Unit* l_Target = GetCaster()->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                GetCaster()->CastSpell(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), ePlanarShiftSpells::SpellTeleportSelf, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(shadowmoon_burial_grounds_nhalish_spell_planar_shift_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_nhalish_spell_planar_shift_SpellScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_nhalish()
{
    new boss_nhalish();                                                     ///< 75829
    new shadowmoon_burial_grounds_nhalish_creature_soul();                  ///< 75899
    new shadowmoon_burial_grounds_nhalish_creature_nhalish_prop();          ///< 75442
    new shadowmoon_burial_grounds_nhalish_creature_void_devestation();      ///< 543531
    new shadowmoon_burial_grounds_nhalish_spell_void_devestation();         ///< 153068
    new shadowmoon_burial_grounds_nhalish_spell_planar_shift();             ///< 153623
    new shadowmoon_burial_grounds_nhalish_spell_soul_steal();               ///< 152962
    new shadowmoon_burial_grounds_nhalish_spell_void_blast();               ///< 152792
    new shadowmoon_burial_grounds_nhalish_spell_void_vortex();              ///< 152801
}
#endif
