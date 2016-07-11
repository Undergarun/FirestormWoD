////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eArchmageActions
{
    ActionTransport         = 1,
    ActionChoosePower,
    ActionStopPrevent
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
    CreatureTriggerNoxiousEruption = 213151
};

enum eArchmageTalks
{
    TalkSolIntro     = 30,     ///< No...you Cant...its...i Cant Fight It!(45481)
    TalkSolAggro     = 31,     ///< Yes, Flee! Fire The Vines! They Control Us All!(45479)
    TalkSolSpell01   = 32,     ///< Yes, Fire! Fire Will Burn Away The Contagion.(45484)
    TalkSolSpell02   = 33,     ///< Surely The Cold Make Them Wither?(45485)
    TalkSolSpell03   = 34,     ///< Get Away..get Away!(45486)
    TalkSolKill01    = 35,     ///< No! They Will Take You Too!(45482)
    TalkSolKill02    = 36,     ///< You Musnt Give Them Anoter Host!(45483)
    TalkSolDeath     = 37      ///< Burn My Body. Its  The Only Way To Stop Them.(45480)
};

/// Archmage Sol - 82682
class boss_archmage_sol : public CreatureScript
{
public:

    boss_archmage_sol() : CreatureScript("boss_archmage_sol") { }

    struct boss_archmage_solAI : public BossAI
    {
        boss_archmage_solAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataArchmageSol)
        {
            m_Instance = me->GetInstanceScript();
            m_CanEvent = false;
            m_PreEvent = false;
            m_Initial = true;
            m_First = false;
        }
         
        enum eArchmageSolEvents
        {
            EventArcaneBrust = 1,
            EventFrostBall,
            EventFireBall,
            EventFireBloom,
            EventFrozenRain,
            EventParasiticGrowth,
            EventPrimalAffinityArcane,
            EventPrimalAffinityFire,
            EventPrimalAffinityFrost,
            EventThoughtChrySailis,
            EventParasiticInjection,
            EventTransform,
            EventTransformRemovePassive     
        };

        enum eArchmageSolSpells
        {
            SpellParasiticGrowth                       = 168885,
            SpellParasiticInjection                    = 166891,
            SpellThoughtChrySailis                     = 166474,
            SpellPrimalAffinityArcane                  = 166477,
            SpellArcaneBrust                           = 166466,
            SpellPrimalAffinityFire                    = 166475,
            SpellFireBloomFlower                       = 166562,
            SpellPrimalAffinityFrost                   = 166476,
            SpellFrozenRainSpawn                       = 166723,
            SpellFrozenRainAuraPeriodicDamage          = 166726,
            SpellFrozenRainAura                        = 166913,
            SpellFrozenSnapDummy                       = 169847,
            SpellFrozenSnapEffectTrigger               = 169848,
            SpellFrozenSnapStun                        = 169850,
            SpellNoxiusEruption                        = 177145,
            SpellEntanglement                          = 180836,
            SpellArcaneChanneling                      = 161837,
            SpellBubble                                = 170665,
            SpellFrostbolt                             = 169824,
            SpellVisualSolArcaneBeam                   = 170146,
            SpellFireBall                              = 168894,
            SpellFireBloomDmgTick                      = 166492,
            SpellSporesVisual                          = 169828,
            SpellSporesVisual2                         = 169831
        };

        InstanceScript* m_Instance;
        bool m_CanEvent;
        bool m_PreEvent;
        bool m_Initial;
        bool m_First;
        uint32 m_Count;
        uint32 m_Visual;
        uint32 m_TimerToDisablePreevent;
        uint32 m_TimeBetweenNoxiousExplosion;

        void Reset() override
        {
            _Reset();
            events.Reset();   
            m_Count  = 0;
            m_Visual = 1 * TimeConstants::IN_MILLISECONDS;;
            m_TimeBetweenNoxiousExplosion = 12 * TimeConstants::IN_MILLISECONDS;
            m_TimerToDisablePreevent      = 15 * TimeConstants::IN_MILLISECONDS;      
            uint32 l_Entries[4] = { eArchmageSolSpells::SpellPrimalAffinityArcane,
                eArchmageSolSpells::SpellPrimalAffinityFire,
                eArchmageSolSpells::SpellPrimalAffinityFrost,
                eArchmageSolSpells::SpellParasiticInjection };
            me->AddAura(eArchmageSolSpells::SpellSporesVisual, me);
            me->AddAura(eArchmageSolSpells::SpellSporesVisual2, me);
            for (uint8 l_I = 0; l_I <= 3; l_I++)
                me->RemoveAura(l_Entries[l_I]);       
        }

        void MoveInLineOfSight(Unit* p_Who) override /// Motherfucker..
        {
            if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 30.0f) && !m_CanEvent && m_Initial)
            {
                m_CanEvent = true;
                m_PreEvent = true;
                m_Initial = false;
                Talk(eArchmageTalks::TalkSolIntro);
            }
        }

        void JustReachedHome() override
        {
            summons.DespawnAll();
            if (m_Instance != nullptr)
                m_Instance->SetBossState(eEverbloomData::DataArchmageSol, EncounterState::FAIL);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eArchmageActions::ActionStopPrevent:
                {
                    me->RemoveAllAuras();
                    me->setFaction(HostileFaction);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS,
                        eUnitFlags::UNIT_FLAG_NON_ATTACKABLE
                        | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE |
                        eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC |
                        eUnitFlags::UNIT_FLAG_UNK_15
                        | eUnitFlags::UNIT_FLAG_UNK_6 |
                        eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2,
                        eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                    me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                    DespawnCreaturesInArea(eArchmageCreatures::CreatureTriggerArcaneBeam, me);
                    break;
                }
                case eArchmageActions::ActionChoosePower:
                {
                        m_Count++;
                        events.Reset();                      
                        uint32 l_Entries[3] = { eArchmageSolSpells::SpellPrimalAffinityArcane,
                            eArchmageSolSpells::SpellPrimalAffinityFire,
                            eArchmageSolSpells::SpellPrimalAffinityFrost };
                        for (uint8 l_I = 0; l_I <= 2; l_I++)
                             me->RemoveAura(l_Entries[l_I]);     
                        /// Parasitic Injection
                        if (!me->HasAura(eArchmageSolSpells::SpellParasiticInjection))
                            me->CastSpell(me, eArchmageSolSpells::SpellParasiticInjection);
                        else
                        {
                            if (Aura* l_Aura = me->GetAura(eArchmageSolSpells::SpellParasiticInjection))
                                l_Aura->SetStackAmount(l_Aura->GetStackAmount() + 1);
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
                                Talk(eArchmageTalks::TalkSolSpell03);
                                me->CastSpell(me, eArchmageSolSpells::SpellPrimalAffinityArcane);
                                events.ScheduleEvent(eArchmageSolEvents::EventArcaneBrust, 6 * TimeConstants::IN_MILLISECONDS);
                                me->MonsterTextEmote("|TInterface\\Icons\\Spell_Arcane_ArcaneArmor02.blp:20|tArchmage Sol begins casting Arcane spells!", Language::LANG_UNIVERSAL, me->GetGUID());                        
                                break;
                            case 1: /// Frost
                                Talk(eArchmageTalks::TalkSolSpell01);
                                me->CastSpell(me, eArchmageSolSpells::SpellPrimalAffinityFrost);
                                events.ScheduleEvent(eArchmageSolEvents::EventFrozenRain, 5 * TimeConstants::IN_MILLISECONDS);
                                events.ScheduleEvent(eArchmageSolEvents::EventFrostBall, 6 * TimeConstants::IN_MILLISECONDS);
                                events.ScheduleEvent(eArchmageSolEvents::EventParasiticGrowth, 29 * TimeConstants::IN_MILLISECONDS);
                                me->MonsterTextEmote("|TInterface\\Icons\\Spell_Frost_FrostArmor02.blp:20|tArchmage Sol begins casting Frost spells!", Language::LANG_UNIVERSAL, me->GetGUID());
                                break;
                            default:
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
            Talk(eArchmageTalks::TalkSolAggro);
            me->CastSpell(me, eArchmageSolSpells::SpellPrimalAffinityFire);
            events.ScheduleEvent(eArchmageSolEvents::EventParasiticGrowth, 29 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eArchmageSolEvents::EventFireBall, urand(5 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eArchmageSolEvents::EventFireBloom, urand(22 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));

            me->CastStop(eArchmageSolSpells::SpellVisualSolArcaneBeam);
            me->RemoveAura(eArchmageSolSpells::SpellBubble);
            me->RemoveAura(eArchmageSolSpells::SpellArcaneChanneling);

            if (m_Instance != nullptr)
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
        }

        void KilledUnit(Unit* p_Attacker) override
        {
            if (p_Attacker && p_Attacker->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eArchmageTalks::TalkSolKill01);
                else
                    Talk(eArchmageTalks::TalkSolKill02);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            summons.DespawnAll();
            Talk(eArchmageTalks::TalkSolDeath);          
        }

        void ArcaneBeam(uint32 const p_Diff)
        {
            if (m_Visual <= p_Diff)
            {
                if (!me->HasAura(eArchmageSolSpells::SpellArcaneChanneling))
                    me->CastSpell(me, eArchmageSolSpells::SpellArcaneChanneling);
                if (!me->HasAura(eArchmageSolSpells::SpellBubble))
                    me->CastSpell(me, eArchmageSolSpells::SpellBubble);

                std::list<Creature*> l_Creatures;
                me->GetCreatureListWithEntryInGrid(l_Creatures, eArchmageCreatures::CreatureTriggerArcaneBeam, 50.0f);
                if (!l_Creatures.empty())
                {            
                    for (Creature* l_Itr : l_Creatures)
                        me->CastSpell(l_Itr, eArchmageSolSpells::SpellVisualSolArcaneBeam);
                }

                m_Visual = 3 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Visual -= p_Diff;
        }

        void SpawnNoxiousBomb(uint32 const p_Diff)
        {
            for (uint8 l_I = 0; l_I < 8; l_I++)
            {             
                Position l_Pos;
                me->GetRandomNearPosition(l_Pos, 60.0f);
                me->SummonCreature(eArchmageCreatures::CreatureTriggerNoxiousEruption, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10 * TimeConstants::IN_MILLISECONDS);
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            /// Spawning Noxius Bombs
            if (m_TimeBetweenNoxiousExplosion <= p_Diff)
            {
                SpawnNoxiousBomb(p_Diff);
                m_TimeBetweenNoxiousExplosion = 12 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_TimeBetweenNoxiousExplosion -= p_Diff;

            if (!UpdateVictim())
            ArcaneBeam(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eArchmageSolEvents::EventParasiticGrowth:
                {
                    me->CastSpell(me, eArchmageSolSpells::SpellParasiticGrowth);

                    events.ScheduleEvent(eArchmageSolEvents::EventParasiticGrowth, 29 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eArchmageSolEvents::EventArcaneBrust:
                {
                    me->CastSpell(me, eArchmageSolSpells::SpellArcaneBrust);

                    events.ScheduleEvent(eArchmageSolEvents::EventArcaneBrust, 5 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eArchmageSolEvents::EventFireBall:
                {
                    if (Unit* l_Victim = me->getVictim())
                        me->CastSpell(l_Victim, eArchmageSolSpells::SpellFireBall);

                    events.ScheduleEvent(eArchmageSolEvents::EventFireBall, 4 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eArchmageSolEvents::EventFireBloom:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))                        
                        me->CastSpell(l_Target, eArchmageSolSpells::SpellFireBloomFlower);

                    Talk(eArchmageTalks::TalkSolSpell01);

                    events.ScheduleEvent(eArchmageSolEvents::EventFireBloom, 12 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eArchmageSolEvents::EventFrostBall:
                {
                    if (Unit* l_Victim = me->getVictim())
                        me->CastSpell(l_Victim, eArchmageSolSpells::SpellFrostbolt);

                    events.ScheduleEvent(eArchmageSolEvents::EventFrostBall, 4 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eArchmageSolEvents::EventFrozenRain:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 70.0f, true))
                        me->SummonCreature(eArchmageCreatures::CreatureTriggerFrozenAura, l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                    events.ScheduleEvent(eArchmageSolEvents::EventFrozenRain, 6 * TimeConstants::IN_MILLISECONDS);
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
        return new boss_archmage_solAI(p_Creature);
    }
};

/// Spore Image - 84386
class the_everbloom_archmage_sol_mob_spore_image : public CreatureScript
{
public:

    the_everbloom_archmage_sol_mob_spore_image() : CreatureScript("the_everbloom_archmage_sol_mob_spore_image") { }

    struct the_everbloom_archmage_sol_mob_spore_imageAI : public ScriptedAI
    {
        the_everbloom_archmage_sol_mob_spore_imageAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eSporeImageSpells
        {
            SpellFrostbolt = 169824,
            SpellFireBall  = 168894
        };

        enum eSporeImageEvents
        {
            EventFireBall = 1,
            EventFrostbolt
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->SetInCombatWithZone();
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            events.ScheduleEvent(eSporeImageEvents::EventFireBall, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eSporeImageEvents::EventFrostbolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eSporeImageEvents::EventFrostbolt:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSporeImageSpells::SpellFrostbolt);

                    events.ScheduleEvent(eSporeImageEvents::EventFrostbolt, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eSporeImageEvents::EventFireBall:
                {
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eSporeImageSpells::SpellFireBall);

                    events.ScheduleEvent(eSporeImageEvents::EventFireBall, urand(10 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
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
        return new the_everbloom_archmage_sol_mob_spore_imageAI(p_Creature);
    }
};

/// Frozen Rain - 424255
class the_everbloom_archmage_sol_mob_frozen_rain : public CreatureScript
{
public:

    the_everbloom_archmage_sol_mob_frozen_rain() : CreatureScript("the_everbloom_archmage_sol_mob_frozen_rain") { }

    struct the_everbloom_archmage_sol_mob_frozen_rainAI : public Scripted_NoMovementAI
    {
        the_everbloom_archmage_sol_mob_frozen_rainAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eFrozenRainSpells
        {
            SpellFrozenRainAuraPeriodicDamage = 166726,
            SpellFrozenRainAura = 166913
        };

        InstanceScript* m_Instance;
        uint32 m_TimeBetweenDot;

        void Reset() override
        {
            me->SetInCombatWithZone();
            me->setFaction(HostileFaction);         
            me->SetReactState(ReactStates::REACT_PASSIVE);
            m_TimeBetweenDot = 1 * TimeConstants::IN_MILLISECONDS;
            me->CastSpell(me, eFrozenRainSpells::SpellFrozenRainAura);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
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

                for (std::list<Player*>::const_iterator l_Itr = l_Playerlist.begin(); l_Itr != l_Playerlist.end(); ++l_Itr)
                {
                    if (!(*l_Itr)->HasAura(eFrozenRainSpells::SpellFrozenRainAuraPeriodicDamage))
                        (*l_Itr)->CastSpell((*l_Itr), eFrozenRainSpells::SpellFrozenRainAuraPeriodicDamage);
                }

                m_TimeBetweenDot = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_TimeBetweenDot -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_archmage_sol_mob_frozen_rainAI(p_Creature);
    }
};

/// Noxious Explosion Trigger - 213151
class the_everbloom_archmage_sol_mob_noxious_explosion : public CreatureScript
{
public:

    the_everbloom_archmage_sol_mob_noxious_explosion() : CreatureScript("the_everbloom_archmage_sol_mob_noxious_explosion") { }

    struct the_everbloom_archmage_sol_mob_noxious_explosionAI : public Scripted_NoMovementAI
    {
        the_everbloom_archmage_sol_mob_noxious_explosionAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eNoxiousExplosionSpells
        {
            SpellNoxiusEruption = 177145
        };

        InstanceScript* m_Instance;

        void Reset() override
        {              
            me->SetObjectScale(0.5);
            me->setFaction(HostileFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->DespawnOrUnsummon(5 * TimeConstants::IN_MILLISECONDS);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->CastSpell(me, eNoxiousExplosionSpells::SpellNoxiusEruption, false);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);        
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_archmage_sol_mob_noxious_explosionAI(p_Creature);
    }
};

/// Cold Snap - 321432
class the_everbloom_archmage_sol_mob_cold_snap : public CreatureScript
{
public:

    the_everbloom_archmage_sol_mob_cold_snap() : CreatureScript("the_everbloom_archmage_sol_mob_cold_snap") { }

    struct the_everbloom_archmage_sol_mob_cold_snapAI : public Scripted_NoMovementAI
    {
        the_everbloom_archmage_sol_mob_cold_snapAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eColdSnapSpells
        {
            SpellFrozenSnapDummy         = 169847,
            SpellFrozenSnapEffectTrigger = 169848,
            SpellFrozenSnapStun          = 169850
        };

        InstanceScript* m_Instance;
        uint32 m_FreezeCheck;

        void Reset() override
        {
            me->SetInCombatWithZone();
            me->setFaction(HostileFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            m_FreezeCheck = 1 * TimeConstants::IN_MILLISECONDS;
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);   
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (m_FreezeCheck <= p_Diff)
            {
                me->CastSpell(me, eColdSnapSpells::SpellFrozenSnapEffectTrigger);

                if (Player* l_Nearest = me->FindNearestPlayer(1.6f, true))
                {
                    me->CastSpell(l_Nearest, eColdSnapSpells::SpellFrozenSnapStun, true);
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
        return new the_everbloom_archmage_sol_mob_cold_snapAI(p_Creature);
    }
};

/// Parasitic Change - 168885
class the_everbloom_archmage_sol_spell_parasite_change : public SpellScriptLoader
{
public:

    the_everbloom_archmage_sol_spell_parasite_change() : SpellScriptLoader("the_everbloom_archmage_sol_spell_parasite_change") { }

    class the_everbloom_archmage_sol_spell_parasite_change_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_archmage_sol_spell_parasite_change_SpellScript);

        void HandleSpecChange()
        {
            if (!GetCaster())
                return;

            if (InstanceScript* m_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Sol = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataArchmageSol)))
                {
                    if (l_Sol->IsAIEnabled)
                        l_Sol->GetAI()->DoAction(eArchmageActions::ActionChoosePower);
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(the_everbloom_archmage_sol_spell_parasite_change_SpellScript::HandleSpecChange);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_archmage_sol_spell_parasite_change_SpellScript();
    }
};

/// Firebloom - 166560
class the_everbloom_archmage_sol_at_firebloom : public AreaTriggerEntityScript
{
public:

    the_everbloom_archmage_sol_at_firebloom() : AreaTriggerEntityScript("the_everbloom_archmage_sol_at_firebloom") {}

    enum eFirebloomSpells
    {
        SpellFireBloomDamageTick = 166492
    };

    int l_Modifier = 2;
    uint32 m_Diff = 6 * TimeConstants::IN_MILLISECONDS;
    uint32 m_DiffDmg = 1 * TimeConstants::IN_MILLISECONDS;
    bool m_Bool = false;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (InstanceScript* m_Instance = p_AreaTrigger->GetInstanceScript())
        {
            if (Creature* l_Sol = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataArchmageSol)))
            {
                if (m_Diff <= p_Time)
                {
                    if (!m_Bool)
                    {
                        m_Bool = true;

                        G3D::Vector3 l_Source(p_AreaTrigger->m_positionX, p_AreaTrigger->m_positionY, p_AreaTrigger->m_positionZ);
                        G3D::Vector3 l_Dest(p_AreaTrigger->m_positionX + 30.0f, p_AreaTrigger->m_positionY, p_AreaTrigger->m_positionZ);
                        G3D::Vector3 l_Orientation(0.0f, 0.0f, 0.0f);

                        l_Sol->PlayOrphanSpellVisual(l_Source, l_Orientation, l_Dest, eEverbloomVisuals::SpellArchmageSolFireBloomFireOrphanVisual, 8.0f);
                    }
                }
                else
                    m_Diff -= p_Time;

                if (m_Bool)
                {
                    if (m_DiffDmg <= p_Time)
                    {
                        std::list<Player*> l_PlayerList;
                        p_AreaTrigger->GetPlayerListInGrid(l_PlayerList, l_Modifier);
                        if (!l_PlayerList.empty())
                        {
                            for (Player* l_Itr : l_PlayerList)
                            {
                                if (!l_Itr)
                                    continue;

                                if (!l_Itr->HasAura(eFirebloomSpells::SpellFireBloomDamageTick))
                                    l_Itr->AddAura(eFirebloomSpells::SpellFireBloomDamageTick, l_Itr);
                            }
                        }

                        if (l_Modifier < 20)
                            l_Modifier += 2;
                        else  
                            p_AreaTrigger->Remove(1 * TimeConstants::IN_MILLISECONDS);

                        m_DiffDmg = 1 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_DiffDmg -= p_Time;
                }
            }
        }
    }

    the_everbloom_archmage_sol_at_firebloom* GetAI() const override
    {
        return new the_everbloom_archmage_sol_at_firebloom();
    }
};

void AddSC_boss_archmage_sol()
{
    new boss_archmage_sol();                                    ///< 82682
    new the_everbloom_archmage_sol_mob_frozen_rain();           ///< 424255
    new the_everbloom_archmage_sol_mob_cold_snap();             ///< 321432
    new the_everbloom_archmage_sol_mob_noxious_explosion();     ///< 213151
    new the_everbloom_archmage_sol_mob_spore_image();           ///< 84386
    new the_everbloom_archmage_sol_spell_parasite_change();     ///< 168885
    new the_everbloom_archmage_sol_at_firebloom();              ///< 166560     
}