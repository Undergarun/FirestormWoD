////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eArchmageSpells
{
    SpellParasiticGrowth                        = 168885,
    SpellParasiticInjection                     = 166891,
    SpellThoughtChrySailis                      = 166474,
    ///Arcane
    SpellPrimalAffinityArcane                   = 166477, 
    SpellArcaneBrust                            = 166466,
    ///Fire
    SpellPrimalAffinityFire                     = 166475,
    SpellFireBloomPreDummy                      = 166489,
    SpellFireBloomDummy                         = 166490,
    SpellFireBloomAreatriggerUnknown            = 166491,
    SpellFireBloomAreatriggerTick               = 166492,
    SpellFireBloomAreatriggerUnknownFlower      = 166560,
    SpellFireBloomAreatriggerUnknownFlowerBomb  = 166562,
    SpellFireBloomUnknown2                      = 166567,
    ///Ice
    SpellPrimalAffinityFrost                    = 166476,
    SpellFrozenRainSpawn                        = 166723,
    SpellFrozenRainAuraPeriodicDamage           = 166726,
    SpellFrozenRainAura                         = 166913,
    ///Frost
    SpellFrozenSnapDummy                        = 169847,
    SpellFrozenSnapEffectTrigger                = 169848,
    SpellFrozenSnapStun                         = 169850,
    ///Rp Phase
    SpellNoxiusEruption                         = 177145,
    SpellEntanglement                           = 180836,
    SpellArcaneChanneling                       = 161837,
    SpellBubble                                 = 170665
};

enum eArchmageEvents
{
    EventArcaneBrust             = 1,
    EventFireBloom,
    EventFrozenRain,
    EventParasiticGrowth,
    EventPrimalAffinityArcane,
    EventPrimalAffinityFire,
    EventPrimalAffinityFrost,
    EventThoughtChrySailis,
    EventParasiticInjection,
    EventTransform,
    EventTransformRemovePassive,
    EventFrostBall
};

enum eArchmageActions
{
    ActionTransport    = 1,
    ActionChoosePower,
    ActionStopPreevent,
};

enum eArchmageCreatures
{
    CreatureSporeImage             = 84386,
    CreatureSporeImage02           = 84984,
    CreatureSporeImage03           = 84985,
    CreatureTriggerPlants          = 213125,
    CreatureTriggerArcaneBeam      = 342252,
    CreatureTriggerFrozenAura      = 424255,
    CreatureTriggerFrozenSnap      = 321432,
    CreatureTriggerNoxiousEruption = 213151,
    TriggerTick = 0,
};

enum eArchmageTalks
{
    KirinTorMageIntro     = 30,     ///< No...you Cant...its...i Cant Fight It!(45481)
    KirinTorMageAggro     = 31,     ///< Yes, Flee! Fire The Vines! They Control Us All!(45479)
    KirinTorMageSpell01   = 32,     ///< Yes, Fire! Fire Will Burn Away The Contagion.(45484)
    KirinTorMageSpell02   = 33,     ///< Surely The Cold Make Them Wither?(45485)
    KirinTorMageSpell03   = 34,     ///< Get Away..get Away!(45486)
    KirinTorMageKill01    = 35,     ///< No! They Will Take You Too!(45482)
    KirinTorMageKill02    = 36,     ///< You Musnt Give Them Anoter Host!(45483)
    KirinTorMageDeath     = 37,     ///< Burn My Body. Its  The Only Way To Stop Them.(45480)
};

static void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
{
    std::list<Creature*> l_Creatures;
    GetCreatureListWithEntryInGrid(l_Creatures, object, entry, 300.0f);

    if (l_Creatures.empty())
        return;

    for (std::list<Creature*>::iterator iter = l_Creatures.begin(); iter != l_Creatures.end(); ++iter)
        (*iter)->DespawnOrUnsummon();
}

#define HostileFaction 16
#define FriendlyFaction 35

/// Archmage Sol - 82682
class boss_archmage_sol : public CreatureScript
{
public:
    boss_archmage_sol() : CreatureScript("boss_archmage_sol") { }

    struct boss_archmage_sol_AI : public BossAI
    {
        boss_archmage_sol_AI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataArchmageSol)
        {
            m_Instance = me->GetInstanceScript();
            m_CanEvent = false;
            m_PreEvent = false;
            m_Initial = true;

            me->setFaction(FriendlyFaction);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }
         
        InstanceScript* m_Instance;

        bool m_CanEvent;
        bool m_PreEvent;
        bool m_Initial;

        uint32 m_Count;
        uint32 m_Visual;
        uint32 m_TimerToDisablePreevent;
        uint32 m_TimeBetweenNoxiousExplosion;

        void Reset() override
        {
            _Reset();
            events.Reset();

            DespawnCreaturesInArea(eArchmageCreatures::CreatureSporeImage, me);
            DespawnCreaturesInArea(eArchmageCreatures::CreatureSporeImage02, me);
            DespawnCreaturesInArea(eArchmageCreatures::CreatureSporeImage03, me);
            DespawnCreaturesInArea(eArchmageCreatures::TriggerTick, me);

            uint32 entries[4] = { eArchmageSpells::SpellPrimalAffinityArcane,
                eArchmageSpells::SpellPrimalAffinityFire,
                eArchmageSpells::SpellPrimalAffinityFrost,
                eArchmageSpells::SpellParasiticInjection };

            for (int i = 0; i < 3; i++)
            {
                me->RemoveAura(entries[i]);
            }

            m_Visual = 1 * TimeConstants::IN_MILLISECONDS;;
            m_TimeBetweenNoxiousExplosion = 12 * TimeConstants::IN_MILLISECONDS;
            m_TimerToDisablePreevent = 15 * TimeConstants::IN_MILLISECONDS;
            m_Count = 0;

            m_Instance->DoRemoveAurasDueToSpellOnPlayers(eArchmageSpells::SpellFireBloomAreatriggerTick);
        }

        void MoveInLineOfSight(Unit* p_Who) override
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 30.0f) && !m_CanEvent && m_Initial)
            {
                m_CanEvent = true;
                m_PreEvent = true;
                m_Initial = false;

                Talk(eArchmageTalks::KirinTorMageIntro);
            }
        }

        void JustReachedHome() override
        {
            summons.DespawnAll();

            if (m_Instance != nullptr)
            {
                m_Instance->SetBossState(eEverbloomData::DataArchmageSol, EncounterState::FAIL);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(eArchmageSpells::SpellFireBloomAreatriggerTick);
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eArchmageActions::ActionStopPreevent:
                        me->RemoveAllAuras();
                        me->setFaction(HostileFaction);

                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS,
                            eUnitFlags::UNIT_FLAG_NON_ATTACKABLE 
                            | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE |
                            eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC |
                            eUnitFlags::UNIT_FLAG_UNK_15 
                            | eUnitFlags::UNIT_FLAG_UNK_6 |
                            eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS2,
                            eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        
                        me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                        DespawnCreaturesInArea(eArchmageCreatures::CreatureTriggerArcaneBeam, me);
                        break;
                case eArchmageActions::ActionChoosePower:
                    {
                        events.Reset();
                        m_Count++;

                        uint32 l_Entries[3] = { eArchmageSpells::SpellPrimalAffinityArcane,
                            eArchmageSpells::SpellPrimalAffinityFire,
                            eArchmageSpells::SpellPrimalAffinityFrost};

                        for (int i = 0; i < 2; i++)
                        {
                            me->RemoveAura(l_Entries[i]);
                        }
             
                        /// parasitic injection
                        if (!me->HasAura(eArchmageSpells::SpellParasiticInjection))
                        {
                            me->CastSpell(me, eArchmageSpells::SpellParasiticInjection);
                        }
                        else
                        {
                            AuraPtr l_Aura = me->GetAura(eArchmageSpells::SpellParasiticInjection);

                            if (l_Aura)
                            {
                                l_Aura->SetStackAmount(l_Aura->GetStackAmount() + 1);
                            }
                        }

                        /// Heroic Spore Image
                        if (me->GetMap() && me->GetMap()->IsHeroic())
                        {
                            Position l_Pos;
                            me->GetRandomNearPosition(l_Pos, 30.0f);

                            me->SummonCreature(eArchmageCreatures::CreatureSporeImage, l_Pos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        }

                        switch (m_Count)
                        {
                            case 2: /// Arcane Magic
                                events.ScheduleEvent(eArchmageEvents::EventArcaneBrust, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                                me->CastSpell(me, eArchmageSpells::SpellPrimalAffinityArcane);
                                // events.CancelEvent(eArchmageEvents::EventParasiticGrowth);   Apparently it should get casted even after the 2 stacks. Interesting..

                                me->MonsterTextEmote("|TInterface\\Icons\\Spell_Arcane_ArcaneArmor02.blp:20|tArchmage Sol begins casting Arcane spells!", LANG_UNIVERSAL, me->GetGUID());
                                Talk(eArchmageTalks::KirinTorMageSpell03);
                                break;
                            case 1: /// Frost
                                events.ScheduleEvent(eArchmageEvents::EventFrozenRain, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                                events.ScheduleEvent(eArchmageEvents::EventFrostBall, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));

                                events.ScheduleEvent(eArchmageEvents::EventParasiticGrowth, 29 * TimeConstants::IN_MILLISECONDS);
                                me->CastSpell(me, eArchmageSpells::SpellPrimalAffinityFrost);

                                me->MonsterTextEmote("|TInterface\\Icons\\Spell_Frost_FrostArmor02.blp:20|tArchmage Sol begins casting Frost spells!", LANG_UNIVERSAL, me->GetGUID());
                                Talk(eArchmageTalks::KirinTorMageSpell02);
                                break;
                        }
                        break;
                   }
                default:
                    break;
            }
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            _EnterCombat();
            Talk(eArchmageTalks::KirinTorMageAggro);

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eEverbloomData::DataArchmageSol, EncounterState::IN_PROGRESS);
            }

            events.ScheduleEvent(eArchmageEvents::EventParasiticGrowth, 29 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eEverbloomEvents::EventFireBall,  urand(5 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eArchmageEvents::EventFireBloom, urand(22 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));

            me->CastSpell(me, eArchmageSpells::SpellPrimalAffinityFire);
        }

        void KilledUnit(Unit* p_Attacker) override
        {
            if (p_Attacker->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eArchmageTalks::KirinTorMageKill01);
            }
        }

        void JustDied(Unit* /*p_Killer*/ ) override
        {
            _JustDied();

            Talk(eArchmageTalks::KirinTorMageDeath);

            if (m_Instance != nullptr)
            {
                m_Instance->SetBossState(eEverbloomData::DataArchmageSol, EncounterState::DONE);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(eArchmageSpells::SpellFireBloomAreatriggerTick);
            } 
        }

        void ArcaneBeam(uint32 const p_Diff)
        {
            if (m_Visual <= p_Diff)
            {
                std::list<Creature*> l_Creatures;
                me->GetCreatureListWithEntryInGrid(l_Creatures, eArchmageCreatures::CreatureTriggerArcaneBeam, 50.0f);

                if (!me->HasAura(eArchmageSpells::SpellArcaneChanneling))
                    me->CastSpell(me, eArchmageSpells::SpellArcaneChanneling);

                if (!me->HasAura(eArchmageSpells::SpellBubble))
                    me->CastSpell(me, eArchmageSpells::SpellBubble);

                if (l_Creatures.empty())
                    return;

                for (auto itr : l_Creatures)
                {
                    me->CastSpell(itr, eEverbloomSpells::SpellVisualSolArcaneBeam);
                }

                m_Visual = 3 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Visual -= p_Diff;
        }
        
        void StopEvent(uint32 const p_Diff)
        {
            if (m_TimerToDisablePreevent <= p_Diff)
            {
                m_PreEvent = false;
                DoAction(eArchmageActions::ActionStopPreevent);
            }
            else
                m_TimerToDisablePreevent -= p_Diff;
        }

        void SpawnNoxiousBomb(uint32 const p_Diff)
        {
            for (int i = 0; i < 8; i++)
            {             
                Position l_Pos;
                me->GetRandomNearPosition(l_Pos, 60.0f);

                me->SummonCreature(eArchmageCreatures::CreatureTriggerNoxiousEruption, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10 * TimeConstants::IN_MILLISECONDS);
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            // Spawning Noxius Bombs
            if (m_TimeBetweenNoxiousExplosion <= p_Diff)
            {
                SpawnNoxiousBomb(p_Diff);
                m_TimeBetweenNoxiousExplosion = 12 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_TimeBetweenNoxiousExplosion -= p_Diff;

            // Pre Event Action
            if (m_PreEvent)
            {
                StopEvent(p_Diff);
            }

            // Boss AI
            if (!UpdateVictim())
            {
                // Arcane Visuals
                ArcaneBeam(p_Diff);
                return;
            }
                 
            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (Unit* l_Victim = me->getVictim())
            {
                switch (events.ExecuteEvent())
                {
                    case eArchmageEvents::EventParasiticGrowth:
                    {
                        me->CastSpell(me, eArchmageSpells::SpellParasiticGrowth);
                        events.ScheduleEvent(eArchmageEvents::EventParasiticGrowth, 29 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eArchmageEvents::EventArcaneBrust:
                    {
                        me->CastSpell(me, eArchmageSpells::SpellArcaneBrust);
                        events.ScheduleEvent(eArchmageEvents::EventArcaneBrust, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEverbloomEvents::EventFireBall:
                    {
                        me->CastSpell(l_Victim, eEverbloomSpells::SpellFireBall);
                        events.ScheduleEvent(eEverbloomEvents::EventFireBall, urand(5 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eArchmageEvents::EventFireBloom:
                    {
                        me->CastSpell(me, eArchmageSpells::SpellFireBloomPreDummy);
                        events.ScheduleEvent(eArchmageEvents::EventFireBloom, urand(22 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eArchmageEvents::EventFrostBall:
                    {
                        me->CastSpell(l_Victim, eEverbloomSpells::SpellFrostbolt);

                        events.ScheduleEvent(eArchmageEvents::EventFrostBall, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eArchmageEvents::EventFrozenRain:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        {
                            me->SummonCreature(eArchmageCreatures::CreatureTriggerFrozenAura, l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            events.ScheduleEvent(eArchmageEvents::EventFrozenRain, 18 * TimeConstants::IN_MILLISECONDS);
                        }
                        break;
                    }
                    default:
                        break;
                    }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_archmage_sol_AI(p_Creature);
    }
};

/// Spore Image - 84386
class the_everbloom_spore_image : public CreatureScript
{
public:
    the_everbloom_spore_image() : CreatureScript("the_everbloom_spore_image") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetInCombatWithZone();

            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            me->setFaction(HostileFaction);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eEverbloomEvents::EventFireBall, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eEverbloomEvents::EventFrostbolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eEverbloomEvents::EventFrostbolt:
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eEverbloomSpells::SpellFrostbolt);
                        events.ScheduleEvent(eEverbloomEvents::EventFrostbolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eEverbloomEvents::EventFireBall:
                    {
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eEverbloomSpells::SpellFireBall);
                        events.ScheduleEvent(eEverbloomEvents::EventFireBall, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
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
        return new the_everbloom_creaturesAI(p_Creature);
    }
};

/// Frozen Rain - 424255
class the_everbloom_sol_frozen_aura: public CreatureScript
{
public:
    the_everbloom_sol_frozen_aura() : CreatureScript("the_everbloom_sol_frozen_aura") { }

    struct the_everbloom_creaturesAI : public Scripted_NoMovementAI
    {
        the_everbloom_creaturesAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint32 m_TimeBetweenDot;

        void Reset() override
        {
            events.Reset();

            me->SetInCombatWithZone();
            me->setFaction(HostileFaction);
            
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

            m_TimeBetweenDot = 1 * TimeConstants::IN_MILLISECONDS;
            me->CastSpell(me, eArchmageSpells::SpellFrozenRainAura); // m_Visual
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (m_TimeBetweenDot <= p_Diff)
            {
                std::list<Player*> l_Playerlist;

                JadeCore::AnyPlayerInObjectRangeCheck check(me, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, l_Playerlist, check);
                me->VisitNearbyObject(2.0f, searcher);

                if (l_Playerlist.empty())
                    return;

                for (std::list<Player*>::const_iterator itr = l_Playerlist.begin(); itr != l_Playerlist.end(); ++itr)
                {
                    if (!(*itr)->HasAura(eArchmageSpells::SpellFrozenRainAuraPeriodicDamage))
                    {
                        (*itr)->CastSpell((*itr), eArchmageSpells::SpellFrozenRainAuraPeriodicDamage);
                    }

                    if ((*itr)->HasAura(eArchmageSpells::SpellFrozenRainAuraPeriodicDamage))
                    {
                        AuraPtr l_Aura = (*itr)->GetAura(eArchmageSpells::SpellFrozenRainAuraPeriodicDamage);

                        if (l_Aura)
                        {
                            l_Aura->SetDuration(1);
                        }
                    }
                }

                m_TimeBetweenDot = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_TimeBetweenDot -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_creaturesAI(p_Creature);
    }
};

/// Noxious Explosion Trigger - 213151
class the_everbloom_noxious_explosion : public CreatureScript
{
public:
    the_everbloom_noxious_explosion() : CreatureScript("the_everbloom_noxious_explosion") { }

    struct the_everbloom_creaturesAI : public Scripted_NoMovementAI
    {
        the_everbloom_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->SetInCombatWithZone();
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->setFaction(HostileFaction);

            me->SetDisplayId(11686);
            me->SetObjectScale(0.5);

            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->CastSpell(me, eArchmageSpells::SpellNoxiusEruption, false);

            me->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_creaturesAI(p_Creature);
    }
};

/// Cold Snap Trigger - 321432
class the_everbloom_cold_snap : public CreatureScript
{
public:
    the_everbloom_cold_snap() : CreatureScript("the_everbloom_cold_snap") { }

    struct the_everbloom_creaturesAI : public Scripted_NoMovementAI
    {
        the_everbloom_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint32 m_FreezeCheck;

        void Reset() override
        {
            events.Reset();
            me->SetInCombatWithZone();
          
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

            me->setFaction(HostileFaction);
            m_FreezeCheck = 1 * TimeConstants::IN_MILLISECONDS;
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (m_FreezeCheck <= p_Diff)
            {
                me->CastSpell(me, eArchmageSpells::SpellFrozenSnapEffectTrigger);

                if (Player* l_Nearest = me->FindNearestPlayer(1.6f, true))
                {
                    me->CastSpell(l_Nearest, eArchmageSpells::SpellFrozenSnapStun, true);
                    me->DespawnOrUnsummon();
                }

                m_FreezeCheck = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_FreezeCheck -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_creaturesAI(p_Creature);
    }
};

/// Firebloom - 754332
class the_everbloom_firebloom_trigger_dot : public CreatureScript
{
public:
    the_everbloom_firebloom_trigger_dot() : CreatureScript("the_everbloom_firebloom_trigger_dot") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->AddUnitState(UnitState::UNIT_STATE_CANNOT_AUTOATTACK);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

            std::list<Player*> l_Playerlist;
            me->GetPlayerListInGrid(l_Playerlist, 100.0f);

            if (l_Playerlist.empty())
                return;

            for (auto itr : l_Playerlist)
            {
                itr->AddAura(eArchmageSpells::SpellFireBloomAreatriggerTick, itr);
            }

            me->CastSpell(me, eArchmageSpells::SpellFireBloomUnknown2);
            me->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_creaturesAI(p_Creature);
    }
};

/// Firebloom Trigger -  234256 / 213125
class the_everbloom_firebloom_trigger: public CreatureScript
{
public:
    the_everbloom_firebloom_trigger() : CreatureScript("the_everbloom_firebloom_trigger") { }

    struct the_everbloom_creaturesAI : public ScriptedAI
    {
        the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->CastSpell(me, eArchmageSpells::SpellFireBloomAreatriggerUnknown);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_creaturesAI(p_Creature);
    }
};

/// Firebloom - 
class the_everbloom_areatrigger_boss_archmage_sol_firebloom : public AreaTriggerEntityScript
{
public:
    the_everbloom_areatrigger_boss_archmage_sol_firebloom() : AreaTriggerEntityScript("the_everbloom_areatrigger_boss_archmage_sol_firebloom")
    {
    }

    uint32 p_Diff = 2 * TimeConstants::IN_MILLISECONDS;
    uint32 m_Despawn = 8 * TimeConstants::IN_MILLISECONDS;
    std::list<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (p_Diff <= p_Time)
        {
            p_AreaTrigger->SummonCreature(eArchmageCreatures::TriggerTick, p_AreaTrigger->GetPositionX(), p_AreaTrigger->GetPositionY(), p_AreaTrigger->GetPositionZ(), p_AreaTrigger->GetOrientation(),  TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            p_Diff = 3 * TimeConstants::IN_MILLISECONDS;
        }
        else
        {
            p_Diff -= p_Time;
        }

        if (m_Despawn <= p_Time)
        {
            //p_AreaTrigger->Remove(1 * TimeConstants::IN_MILLISECONDS);
            m_Despawn = 25 * TimeConstants::IN_MILLISECONDS;
        }
        else
        {
            m_Despawn -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Targets.empty())
            return;

        for (auto l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->HasAura(eArchmageSpells::SpellFireBloomAreatriggerTick))
                l_Target->RemoveAura(eArchmageSpells::SpellFireBloomAreatriggerTick);
        }
    }

    the_everbloom_areatrigger_boss_archmage_sol_firebloom* GetAI() const override
    {
        return new the_everbloom_areatrigger_boss_archmage_sol_firebloom();
    }
};

/// Firebloom - 166489
class the_everbloom_firebloom_preflower : public SpellScriptLoader
{
public:
    the_everbloom_firebloom_preflower() : SpellScriptLoader("the_everbloom_firebloom_preflower") { }

    class the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spells);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            for (int i = 0; i <= 2; i++)
            {
                Position l_Position;
                GetCaster()->GetRandomNearPosition(l_Position, 60.0f);

                if (Creature* l_Target = GetCaster()->SummonCreature(eArchmageCreatures::CreatureTriggerPlants, l_Position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    GetCaster()->CastSpell(l_Target, eArchmageSpells::SpellFireBloomAreatriggerUnknownFlowerBomb);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(the_everbloom_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_spells();
    }
};

/// Parasitic Change - 168885
class the_everbloom_parasite_change : public SpellScriptLoader
{
public:
    the_everbloom_parasite_change() : SpellScriptLoader("the_everbloom_parasite_change") { }

    class the_everbloom_spells : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spells);

        void HandleSpecChange()
        {
            if (!GetCaster())
                return;

            if (InstanceScript* m_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Sol = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataArchmageSol)))
                {
                    if (l_Sol->GetAI())
                        l_Sol->GetAI()->DoAction(eArchmageActions::ActionChoosePower);
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(the_everbloom_spells::HandleSpecChange);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_spells();
    }
};

void AddSC_boss_sol()
{
    new boss_archmage_sol();
    new the_everbloom_sol_frozen_aura();
    new the_everbloom_cold_snap();
    new the_everbloom_noxious_explosion();
    new the_everbloom_spore_image();
    new the_everbloom_firebloom_trigger_dot();
    new the_everbloom_firebloom_trigger();
    new the_everbloom_firebloom_preflower();
    new the_everbloom_parasite_change();
    new the_everbloom_areatrigger_boss_archmage_sol_firebloom();
}