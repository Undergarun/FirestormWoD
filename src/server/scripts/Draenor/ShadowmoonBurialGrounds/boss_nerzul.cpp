////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadowmoon_burial_grounds.hpp"

enum eNerzulTalks
{
    TalkAggro = 1, ///< The Shadowmoon cannot be defeated.. we walk in the realm of death itself!  [43682]
    TalkDeath,     ///< My power... will repel across eternity.. [43683]
    TalkIntro01,   ///< The darkness it flows within me.. it seeps from every vein. Come stranger gaze into the eye of the void! Tender your souls... to ME! *Nerz'ul laughs* [43684]
    TalkIntro02,   ///< Peer into the gateway.. I command! cross into the realm of the shadowland.. they should deliever you to your death! [43685]
    TalkKill01,    ///< You soul.. shall serve me... [43686]
    TalkKill02,    ///< You bend to my will...  [43687]
    TalkSpell01,   ///< I am the herald of the END! [43688]
    TalkSpell02,   ///< A kindeling of bones and shadows.. army of the dead.. ARISE! [43689]
    TalkSpell03,   ///< Into the ABYSS! [43690]
    TalkSpell04    ///< Feel the void, cold embrace.. [43691]
}; 

enum eNerzulCreatures
{
    CreatureOmenOfDeath                  = 76462,
    CreatureRitualOfBones                = 76518,
    CreatureRitualOfBonesDarknessTrigger = 534556,
    CreatureVoidDoorSpawnCounter         = 645456
};

enum eNerzulActions
{
    ActionNerzulPropIntroduction = 1
};

Position const l_NerzulTeleportPosition = { 1723.754f, -799.859f, 73.735f, 4.222427f };

Position const l_InitialPositionRight   = { 1694.928f, -785.322f, 73.735f, 4.234663f };

Position const l_InitialPositionLeft    = { 1677.648f, -825.009f, 73.306f, 0.882368f };

/// Nerz'ul - 76407
class boss_nerzul : public CreatureScript
{
public:

    boss_nerzul() : CreatureScript("boss_nerzul") { }

    struct boss_nerzulAI : public BossAI
    {
        boss_nerzulAI(Creature* p_Creature) : BossAI(p_Creature, eShadowmoonBurialGroundsDatas::DataBossNerzul)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eNerzulSpells
        {
            SpellMalevolance                    = 154442,
            SpellOmenOfDeathPeriodicSpell       = 175988,
            SpellOmenOfDeathSummon              = 154350,
            SpellOmenOfDeathVisualRune          = 154351,
            SpellOmenOfDeathLightningt          = 154352,
            SpellOmenOfDeathLightningDamageTick = 154353,
            SpellOmenOfDeathBreath              = 175988,
            SpellOmenOfDeathDummy               = 177691,
            SpellRitualOfBones                  = 154469,
            SpellRitualOfBonesDamage            = 154468,
            SpellRitualOfBonesDot               = 154469,
            SpellRitualOfBonesThirdVisual       = 154559,
            SpellRitualOfBonesPeriodic          = 156312,
            SpellRitualOfBonesWeirdVisualPoop   = 160445,
            SpellRitualOfBonesInvisibiltiy      = 160537,
            SpellNerzulChannel                  = 160674
        };

        enum eNerzulEvents
        {
            EventMalevolance,
            EventRitualOfSouls,
            EventOmenOfDeath
        };

        InstanceScript* m_Instance;
        float m_X;
        float m_Y;

        void Reset() override
        {
            _Reset();
            events.Reset();
            m_X = 10.375f;
            m_Y = 4.538f;
            me->setFaction(HostileFaction); 
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();
            DespawnCreaturesInArea(eNerzulCreatures::CreatureOmenOfDeath, me);
            DespawnCreaturesInArea(eNerzulCreatures::CreatureRitualOfBones, me);
            DespawnCreaturesInArea(eNerzulCreatures::CreatureRitualOfBonesDarknessTrigger, me);
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();
            Talk(eNerzulTalks::TalkAggro);
            events.ScheduleEvent(eNerzulEvents::EventOmenOfDeath, 15 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNerzulEvents::EventRitualOfSouls, 40 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNerzulEvents::EventMalevolance, 20 * TimeConstants::IN_MILLISECONDS);
            if (m_Instance != nullptr)
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                switch (urand(0, 1))
                {
                    case 0:
                        Talk(eNerzulTalks::TalkKill01);
                        break;
                    case 1:
                        Talk(eNerzulTalks::TalkKill02);
                        break;
                    default:
                        break;
                }
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            summons.DespawnAll();
            Talk(eNerzulTalks::TalkDeath);
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
                case eNerzulEvents::EventMalevolance:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eNerzulSpells::SpellMalevolance);
                    Talk(eNerzulTalks::TalkSpell01);
                    events.ScheduleEvent(eNerzulEvents::EventMalevolance, TimeConstants::IN_MILLISECONDS);
                    break;
                case eNerzulEvents::EventOmenOfDeath:
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Target, eNerzulSpells::SpellOmenOfDeathSummon);
                    Talk(eNerzulTalks::TalkSpell03);
                    events.ScheduleEvent(eNerzulEvents::EventOmenOfDeath, TimeConstants::IN_MILLISECONDS);
                    break;
                case eNerzulEvents::EventRitualOfSouls:
                {
                    Talk(eNerzulTalks::TalkSpell02);
                    switch (urand(0, 1))
                    {
                        case 0: /// Right
                        {
                            for (uint8 l_I = 0; l_I <= 6; l_I++)
                            {
                                me->SummonCreature(eNerzulCreatures::CreatureRitualOfBones, l_InitialPositionRight.GetPositionX() + (m_X * l_I),
                                l_InitialPositionRight.GetPositionY() + (m_Y * l_I), l_InitialPositionRight.GetPositionZ(),
                                l_InitialPositionRight.GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 40 * TimeConstants::IN_MILLISECONDS);
                            }
                            break;
                        }
                        case 1: /// Left
                        {
                            for (uint8 l_I = 0; l_I <= 6; l_I++)
                            {
                                me->SummonCreature(eNerzulCreatures::CreatureRitualOfBones, l_InitialPositionLeft.GetPositionX() + (m_X * l_I),
                                l_InitialPositionLeft.GetPositionY() + (m_Y * l_I), l_InitialPositionLeft.GetPositionZ(),
                                l_InitialPositionLeft.GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 40 * TimeConstants::IN_MILLISECONDS);
                            }
                            break;
                        }
                        default:
                            break;
                    }
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
        return new boss_nerzulAI(p_Creature);
    }
};

/// Omen Of Death - 76462
class shadowmoon_burial_grounds_nerzul_creature_omen_of_death : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nerzul_creature_omen_of_death() : CreatureScript("shadowmoon_burial_grounds_nerzul_creature_omen_of_death") {}

    struct shadowmoon_burial_grounds_nerzul_creature_omen_of_deathAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_nerzul_creature_omen_of_deathAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eOmenOfDeathSpells
        {
            SpellOmenOfDeathVisualRune = 154351,
            SpellOmenOfDeathLightning  = 154352
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->CastSpell(me, eOmenOfDeathSpells::SpellOmenOfDeathVisualRune);
            me->CastSpell(me, eOmenOfDeathSpells::SpellOmenOfDeathLightning);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nerzul_creature_omen_of_deathAI(p_Creature);
    }
};

/// Ritual Of Bones - 76518
class shadowmoon_burial_grounds_nerzul_creature_ritual_of_bones : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nerzul_creature_ritual_of_bones() : CreatureScript("shadowmoon_burial_grounds_nerzul_creature_ritual_of_bones") {}

    struct shadowmoon_burial_grounds_nerzul_creature_ritual_of_bonesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_nerzul_creature_ritual_of_bonesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eRitualOfBonesSpells
        {
            SpellRitualOfBonesWeirdVisualPoop = 160445
        };

        uint32 m_Diff;
        InstanceScript* m_Instance;

        void Reset() override
        {
            me->RemoveAllAuras();
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.5f);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_FORWARD);
            me->CastSpell(me, eRitualOfBonesSpells::SpellRitualOfBonesWeirdVisualPoop);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (m_Diff <= p_Diff)
            {
                me->SummonCreature(eNerzulCreatures::CreatureRitualOfBonesDarknessTrigger, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                m_Diff = 3 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Diff -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nerzul_creature_ritual_of_bonesAI(p_Creature);
    }
};

/// Ritual Of Bones Darkness Trigger - 534556
class shadowmoon_burial_grounds_nerzul_creature_darkness : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nerzul_creature_darkness() : CreatureScript("shadowmoon_burial_grounds_nerzul_creature_darkness") {}

    struct shadowmoon_burial_grounds_nerzul_creature_darknessAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_nerzul_creature_darknessAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eRitualOfBonesSpells
        {
            SpellRitualOfBonesDot             = 154469,
            SpellRitualOfBonesThirdVisual     = 154559,
            SpellRitualOfBonesWeirdVisualPoop = 160455
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            /// Orientation check
            if (TempSummon* l_Tempo = me->ToTempSummon())
            {
                if (Unit* l_Summoner = l_Tempo->ToTempSummon())
                    me->SetFacingTo(l_Summoner->GetOrientation());
            }           
            events.Reset();
            me->setFaction(FriendlyFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->CastSpell(me, eRitualOfBonesSpells::SpellRitualOfBonesThirdVisual);
            me->CastSpell(me, eRitualOfBonesSpells::SpellRitualOfBonesWeirdVisualPoop);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);      
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            std::list<Player*> l_ListPlayer;
            me->GetPlayerListInGrid(l_ListPlayer, 100.0f);
            if (!l_ListPlayer.empty())
            {
                for (Player* l_Itr : l_ListPlayer)
                {
                    if (l_Itr->isInBack(me))
                        l_Itr->AddAura(eRitualOfBonesSpells::SpellRitualOfBonesDot, l_Itr);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nerzul_creature_darknessAI(p_Creature);
    }
};

/// Nerzul initial Teleport - 342521
class shadowmoon_burial_grounds_nerzul_creature_initial_teleport : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nerzul_creature_initial_teleport() : CreatureScript("shadowmoon_burial_grounds_nerzul_creature_initial_teleport") {}

    struct shadowmoon_burial_grounds_nerzul_creature_initial_teleportAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_nerzul_creature_initial_teleportAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
            m_HasOpened = false;
        }   

        bool m_HasOpened;
        uint32 m_Counter;
        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->setFaction(FriendlyFaction);
            me->SetDisplayId(InvisibleDisplay); 
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }
    
        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eShadowmoonBurialGroundsActions::ActionCountVoidSpawnNearNerzulGate:
                {
                    if (!m_HasOpened)
                    {
                        if (m_Counter < 2)
                            m_Counter++;
                        else
                        {
                            m_HasOpened = true;
                            if (m_Instance != nullptr)
                            {
                                if (GameObject* l_NerzulDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataNerzulDoor)))
                                {
                                    l_NerzulDoor->SetLootState(LootState::GO_READY);
                                    l_NerzulDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);

                                    if (Creature* l_NerzulProp = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataNerzulProp)))
                                    {
                                        if (l_NerzulProp->IsAIEnabled)
                                            l_NerzulProp->GetAI()->DoAction(eNerzulActions::ActionNerzulPropIntroduction);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (m_Instance != nullptr)
            {
                if (Creature* l_Nerzul = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNerzul)))
                {
                    if (!l_Nerzul->isInCombat())
                    {
                        if (Player* l_NearestPlayer = me->FindNearestPlayer(5.0f, true))
                            l_NearestPlayer->NearTeleportTo(l_NerzulTeleportPosition.GetPositionX(), l_NerzulTeleportPosition.GetPositionY(), l_NerzulTeleportPosition.GetPositionZ(), l_NerzulTeleportPosition.GetOrientation());
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nerzul_creature_initial_teleportAI(p_Creature);
    }
};

/// Nerzul prop - 79497
class shadowmoon_burial_grounds_nerzul_creature_nerzul_prop : public CreatureScript
{
public:

    shadowmoon_burial_grounds_nerzul_creature_nerzul_prop() : CreatureScript("shadowmoon_burial_grounds_nerzul_creature_nerzul_prop") {}

    struct shadowmoon_burial_grounds_nerzul_creature_nerzul_propAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_nerzul_creature_nerzul_propAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
            m_Intro = false;
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            events.Reset();
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->setFaction(FriendlyFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);          
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eNerzulActions::ActionNerzulPropIntroduction:
                    if (!m_Intro)
                        events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventTalk01, 1 * TimeConstants::IN_MILLISECONDS);
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
                case eShadowmoonBurialGroundsEvents::EventTalk01:
                    Talk(eNerzulTalks::TalkIntro01);
                    events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventTalk02, 10 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eShadowmoonBurialGroundsEvents::EventTalk02:
                    Talk(eNerzulTalks::TalkIntro02);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_nerzul_creature_nerzul_propAI(p_Creature);
    }
};

void AddSC_boss_nerzul()
{
    new boss_nerzul();                                                          ///< 76407
    new shadowmoon_burial_grounds_nerzul_creature_nerzul_prop();                ///< 79497
    new shadowmoon_burial_grounds_nerzul_creature_omen_of_death();              ///< 76462
    new shadowmoon_burial_grounds_nerzul_creature_ritual_of_bones();            ///< 76518
    new shadowmoon_burial_grounds_nerzul_creature_darkness();                   ///< 534556
    ///new shadowmoon_burial_grounds_nerzulinitial_teleport();                  ///< Create areatrigger to teleport to bottom floor. 
}