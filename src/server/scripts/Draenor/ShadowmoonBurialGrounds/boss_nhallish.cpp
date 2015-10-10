////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadowmoon_burial_grounds.hpp"

enum eNhalishSpells
{
    SpellPlanarShift                         = 153623,
    SpellVoidVortexDamage                    = 152800,
    SpellVoidVortexAreaTrigger               = 152801,
    SpellVoidBlastSpellAuraDummy             = 152792,
    SpellVoidBlastDot                        = 153501,
    SpellVoidDevastationPeriodicTriggerSpell = 153067,
    SpellVoidDevstationProjectile            = 153068,
    SpellVoidDevestationDebuff               = 153070l,
    SpellVoidDevstationAreaTrigger           = 153072,
    SpellSoulStealForceCast                  = 152962,
    SpellSoulStealPeriodicDummy              = 156755,
    SpellSoulless                            = 154947,
    SpellRegainYourSoul                      = 153007,
    SpellSoulShred                           = 152979,
    SpellShadowChannel                       = 30742,
    SpellLootSparkles                        = 92376,
    SpellCloneMe                             = 45204,
};

enum eNhalishEvents
{
    EventPlanarShift = 1,
    EventVoidVortex,
    EventVoidBlast,
    EventVoidDevastation,
    EventSoulSteal,
    EventRandomMovement,
};

enum eNhalishCreatures
{
   CreatureDistrubedSouls     = 76400,
   CreatureSoul               = 75899,
   CreatureNhalishProp        = 432561,
   CreatureDevestationTrigger = 543531, /// Hardcoded
};

enum eNhalishGameObjects
{
    GameObjectNhalishDoor     = 227851,
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
    TalkSpell04,   ///< Flee... flee from your your formal masters! [43207]
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

            HandleDoorActivation();
        }

        InstanceScript* m_Instance;

        static uint32 PhaseCounter;

        void Reset() override
        {
            events.Reset();
            PhaseCounter = 0;
            
            me->CastSpell(me, eNhalishSpells::SpellShadowChannel);

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void HandleDoorActivation()
        {
            std::list<GameObject*> l_ListDoors;
            me->GetGameObjectListWithEntryInGrid(l_ListDoors, eNhalishGameObjects::GameObjectNhalishDoor, 100.0f);

            if (l_ListDoors.empty())
                return;

            for (auto itr : l_ListDoors)
            {
                // Activate
                itr->SetLootState(LootState::GO_READY);
                itr->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();

            HandleDoorActivation();

            // Adding back to Visibility List
            std::list<Player*> l_ListVisibility;
            me->GetPlayerListInGrid(l_ListVisibility, 300.0f);

            if (l_ListVisibility.empty())
                return;

            for (auto itr : l_ListVisibility)
            {
                itr->SetPhaseMask(1, true);
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();

            Talk(eNhallishTalks::TalkAggro);

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNhallish, EncounterState::IN_PROGRESS);

                DoZoneInCombat();
            }

            HandleDoorActivation();

            events.ScheduleEvent(eNhalishEvents::EventPlanarShift, 30 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventVoidDevastation, 50 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventSoulSteal, 50 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventVoidVortex, 50 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventVoidBlast, 50 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eNhallishTalks::TalkKill01);
                else
                    Talk(eNhallishTalks::TalkKill02);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();

            Talk(eNhallishTalks::TalkDeath);

            // Adding back to Visibility List
            std::list<Player*> l_ListVisibility;
            me->GetPlayerListInGrid(l_ListVisibility, 300.0f);

            if (l_ListVisibility.empty())
                return;

            for (auto itr : l_ListVisibility)
            {
                itr->SetPhaseMask(1, true);
            }

            // Distrubed Spirits RP moment.
            for (int i = 0; i < 10; i++)
            {
               Creature* l_Creature = me->SummonCreature(eNhalishCreatures::CreatureDistrubedSouls, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 10 * TimeConstants::IN_MILLISECONDS);

               if (l_Creature)
               {
                   l_Creature->setFaction(35);
                   l_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                   l_Creature->GetMotionMaster()->MoveRandom(15.0f);

                   if (roll_chance_i(25))
                   {
                       switch (urand(0, 1))
                       {
                       case 0:
                           me->MonsterSay("Thank you.", Language::LANG_UNIVERSAL, me->GetGUID());
                           break;
                       case 1:
                           me->MonsterSay("My torment is over!", Language::LANG_UNIVERSAL, me->GetGUID());
                           break;
                       default:
                           break;
                       }
                   }
               }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            // Reset phase Counter
            if (PhaseCounter >= 4)
            {
                PhaseCounter = 0;
            }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case eNhalishEvents::EventPlanarShift:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eNhalishSpells::SpellPlanarShift);

                        Talk(eNhallishTalks::TalkSpell03);

                        events.ScheduleEvent(eNhalishEvents::EventPlanarShift, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eNhalishEvents::EventVoidDevastation:
                        me->CastSpell(me, eNhalishSpells::SpellVoidDevastationPeriodicTriggerSpell);
                    
                        Talk(eNhallishTalks::TalkSpell02);

                        events.ScheduleEvent(eNhalishEvents::EventVoidDevastation, 50 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eNhalishEvents::EventSoulSteal:
                        Talk(eNhallishTalks::TalkSpell04);
                        me->CastSpell(me, eNhalishSpells::SpellSoulStealForceCast);
                        
                        events.ScheduleEvent(eNhalishEvents::EventSoulSteal, 50 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eNhalishEvents::EventVoidBlast:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eNhalishSpells::SpellVoidBlastSpellAuraDummy);

                        events.ScheduleEvent(eNhalishEvents::EventVoidBlast, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eNhalishEvents::EventVoidVortex:
                        me->CastSpell(me, eNhalishSpells::SpellVoidVortexAreaTrigger);

                        Talk(eNhallishTalks::TalkSpell01);

                        events.ScheduleEvent(eNhalishEvents::EventVoidVortex, 50 * TimeConstants::IN_MILLISECONDS);
                        break;
                }
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
class shadowmoon_burial_grounds_soul : public CreatureScript
{
public:
    shadowmoon_burial_grounds_soul() : CreatureScript("shadowmoon_burial_grounds_soul") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {   
        if (shadowmoon_burial_grounds_soul::shadowmoon_burial_grounds_creaturesAI* linkAI = CAST_AI(shadowmoon_burial_grounds_soul::shadowmoon_burial_grounds_creaturesAI, creature->GetAI()))
        {
            if (linkAI && linkAI->m_HasDied)
            {
                if (linkAI->m_victimGUID)
                {
                    if (Unit* victim = Unit::GetUnit(*creature, linkAI->m_victimGUID))
                    {
                        victim->RemoveAura(eNhalishSpells::SpellSoulless);

                        // Returned Soul
                        if (victim->HasAura(eNhalishSpells::SpellSoulShred))
                        {
                            victim->CastSpell(victim, eNhalishSpells::SpellRegainYourSoul, true);
                            victim->RemoveAura(eNhalishSpells::SpellSoulShred);
                        }

                        victim->SetPhaseMask(1, true);

                        creature->DespawnOrUnsummon(1000);
                    }
                }
            }
        }
        return true;
    }

    struct shadowmoon_burial_grounds_creaturesAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_victimGUID(0)
        {
            m_HasDied = false;
        }

        uint64 m_victimGUID;
        bool   m_HasDied;

        void Reset()
        {         
            me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellUnortodoxExistanceAuraDummy);
            me->SetReactState(ReactStates::REACT_PASSIVE);
        }

        void SetGUID(uint64 guid, int32 /*param*/) override
        {
            m_victimGUID = guid;
            if (m_victimGUID)
                if (Unit* victim = Unit::GetUnit(*me, m_victimGUID))
                {
                    me->CastSpell(victim, eNhalishSpells::SpellSoulShred);
                    victim->CastSpell(me, eNhalishSpells::SpellCloneMe);
                }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            m_HasDied = true;

            me->RemoveAllAuras();

            me->Respawn();
            me->setFaction(35);
            me->SetReactState(ReactStates::REACT_PASSIVE);

            me->CastSpell(me, eNhalishSpells::SpellLootSparkles);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Nhalish Prop - 75442
class shadowmoon_burial_grounds_nhalish_prop : public CreatureScript
{
public:
    shadowmoon_burial_grounds_nhalish_prop() : CreatureScript("shadowmoon_burial_grounds_nhalish_prop") { }

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(16);

            me->CastSpell(me, eNhalishSpells::SpellShadowChannel);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (m_Instance != nullptr)
            {
                if (Creature* l_Nhalish = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNhallish)))
                {
                    me->SetHealth(l_Nhalish->GetHealthPct());
                }
            }
        }
    };
     
    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Void Devestation - 543531 // Hacked, hardcoded
class shadowmoon_void_devestation_trigger : public CreatureScript
{
public:
    shadowmoon_void_devestation_trigger() : CreatureScript("shadowmoon_void_devestation_trigger") { }

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) {}

        uint32 m_Timer;

        void Reset() override
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(35);
            me->SetDisplayId(11686);

            m_Timer = 500;
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            std::list<uint64> m_Targets;

            if (m_Timer <= p_Diff)
            {
                std::list<Player*> PL_list;

                JadeCore::AnyPlayerInObjectRangeCheck check(me, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, PL_list, check);
                me->VisitNearbyObject(2.0f, searcher);

                if (PL_list.empty())
                    return;

                for (std::list<Player*>::const_iterator itr = PL_list.begin(); itr != PL_list.end(); ++itr)
                {
                    if (!(*itr)->HasAura(eNhalishSpells::SpellVoidDevestationDebuff))
                    {
                        (*itr)->CastSpell((*itr), eNhalishSpells::SpellVoidDevestationDebuff);
                        m_Targets.push_back((*itr)->GetGUID());

                        if ((*itr)->HasAura(eNhalishSpells::SpellVoidDevestationDebuff))
                        {
                            AuraPtr aura = (*itr)->GetAura(eNhalishSpells::SpellVoidDevestationDebuff);

                            if (aura)
                            {
                                aura->SetDuration(1);
                            }
                        }
                    }
                }

                m_Timer = 500;
            }
            else
            {
                m_Timer -= p_Diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Void Devestation - 153068 (TARGET_DEST_CASTER, TARGET_DEST_DEST_RADIUS)
class spell_void_devestation_trigger_missile : public SpellScriptLoader
{
public:
    spell_void_devestation_trigger_missile() : SpellScriptLoader("spell_void_devestation_trigger_missile") { }

    class spell_void_devestation_trigger_missile_spell_script : public SpellScript
    {
        PrepareSpellScript(spell_void_devestation_trigger_missile_spell_script);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetA = Targets::TARGET_DEST_CASTER;
            spell->Effects[0].TargetB = Targets::TARGET_DEST_DEST_RADIUS;
            spell->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); // 30y
            return true;
        }

        void HandleTriggerMissile(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            if (!GetCaster())
                return;

           const WorldLocation* l_WorldLocation = GetExplTargetDest();
           const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(eNhalishSpells::SpellVoidDevstationAreaTrigger);

           if (!l_SpellInfo)
               return;

           GetCaster()->SummonCreature(eNhalishCreatures::CreatureDevestationTrigger, l_WorldLocation->GetPositionX(), l_WorldLocation->GetPositionY(), l_WorldLocation->GetPositionZ(), l_WorldLocation->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, l_SpellInfo->GetDuration());
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_void_devestation_trigger_missile_spell_script::HandleTriggerMissile, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_void_devestation_trigger_missile_spell_script();
    }
};

/// Void Blast - 152792 
class spell_void_blast : public SpellScriptLoader
{
public:
    spell_void_blast() : SpellScriptLoader("spell_void_blast") { }

    class spell_void_blast_aura_script : public AuraScript
    {
        PrepareAuraScript(spell_void_blast_aura_script);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].Effect = AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL;
            spell->Effects[0].TargetA = Targets::TARGET_UNIT_TARGET_ENEMY;
            spell->Effects[0].TargetB = 0;
            spell->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); // 30y
            return true;
        }

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            if (GetCaster())
            {
                if (GetTarget())
                {
                    GetTarget()->AddAura(eNhalishSpells::SpellVoidBlastDot, GetTarget());

                    if (AuraPtr l_Aura = GetTarget()->GetAura(eNhalishSpells::SpellVoidBlastDot))
                    {
                        l_Aura->SetDuration(1);
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_void_blast_aura_script::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_void_blast_aura_script();
    }
};

/// Void Blast - 153501 
class spell_void_blast_debuff : public SpellScriptLoader
{
public:
    spell_void_blast_debuff() : SpellScriptLoader("spell_void_blast_debuff") { }

    class spell_void_blast_debuff_aura_scripts : public AuraScript
    {
        PrepareAuraScript(spell_void_blast_debuff_aura_scripts);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->DurationEntry = sSpellDurationStore.LookupEntry(39); // 2s
            return true;
        }

        void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
        {
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_void_blast_debuff_aura_scripts::CalculateAmount, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_void_blast_debuff_aura_scripts();
    }
};

// Void Devestation - 153072  -- MISSING FROM SNIFF, hardcoding.
/*
class areatrigger_void_devestation : public AreaTriggerEntityScript
{
public:
    areatrigger_void_devestation() : AreaTriggerEntityScript("areatrigger_void_devestation")
    {
    }

    int m_Diff = 500;
    std::list<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (m_Diff <= p_Time)
        {
            std::list<Player*> PL_list;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, PL_list, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

            if (PL_list.empty())
                return;

            for (std::list<Player*>::const_iterator itr = PL_list.begin(); itr != PL_list.end(); ++itr)
            {
                if (!(*itr)->HasAura(eNhalishSpells::SpellVoidDevestationDebuff))
                {
                    (*itr)->CastSpell((*itr), eNhalishSpells::SpellVoidDevestationDebuff);
                    m_Targets.push_back((*itr)->GetGUID());

                    if ((*itr)->HasAura(eNhalishSpells::SpellVoidDevestationDebuff))
                    {
                        AuraPtr aura = (*itr)->GetAura(eNhalishSpells::SpellVoidDevestationDebuff);

                        if (aura)
                        {
                            aura->SetDuration(1);
                        }
                    }
                }
            }

            m_Diff = 500;
        }
        else
        {
            m_Diff -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        for (auto l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->HasAura(eNhalishSpells::SpellVoidDevestationDebuff))
                l_Target->RemoveAura(eNhalishSpells::SpellVoidDevestationDebuff);
        }
    }

    areatrigger_void_devestation* GetAI() const
    {
        return new areatrigger_void_devestation();
    }
};
*/

// Void Vortex - 152801  
class areatrigger_void_vortex : public AreaTriggerEntityScript
{
public:
    areatrigger_void_vortex() : AreaTriggerEntityScript("areatrigger_void_vortex")
    {
    }

    int m_Diff = 500;
    std::list<uint64> m_Targets;
    InstanceScript* m_Instance;
    Position l_Pos;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (m_Diff <= int32(p_Time))
        {
            std::list<Player*> l_ListPlayers;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 3.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_ListPlayers, check);
            p_AreaTrigger->VisitNearbyObject(3.0f, searcher);

            if (l_ListPlayers.empty())
                return;

            Position l_Pos;
            m_Instance = p_AreaTrigger->GetInstanceScript();

            if (m_Instance == nullptr)
                return;

            if (Creature* l_Nhalish = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNhallish)))
            {
                l_Nhalish->GetPosition(&l_Pos);

                for (std::list<Player*>::const_iterator itr = l_ListPlayers.begin(); itr != l_ListPlayers.end(); ++itr)
                {
                    Player* l_Player = (*itr);

                    if (l_Player->isAlive() && !l_Player->HasMovementForce(l_Nhalish->GetGUID()))
                        l_Player->SendApplyMovementForce(l_Nhalish->GetGUID(), true, l_Pos, 3.0f);
                    else if (!l_Player->isAlive() && l_Player->HasMovementForce(l_Nhalish->GetGUID()))
                        l_Player->RemoveAllMovementForces();

                    if (l_Player->IsWithinDistInMap(p_AreaTrigger, 1.0f))
                    {
                        l_Player->CastSpell(l_Player, eNhalishSpells::SpellVoidVortexDamage);
                    }

                    m_Targets.push_back(l_Player->GetGUID());
                }

            }
            m_Diff = 500;
        }
        else
        {
            m_Diff -= p_Time;
        }
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        Position l_OldPosition;

        if (Creature* l_Bonemaw = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossBonemaw)))
        {
            l_Bonemaw->GetPosition(&l_OldPosition);

            for (auto l_Guid : m_Targets)
            {
                if (Player* l_OldPlayer = sObjectAccessor->GetPlayer(*p_AreaTrigger, l_Guid))
                {
                    l_OldPlayer->RemoveAllMovementForces();
                }
            }
        }
    }

    areatrigger_void_vortex* GetAI() const
    {
        return new areatrigger_void_vortex();
    }
};

// Soul Steal - 152962 
class spell_soul_steal : public SpellScriptLoader
{
public:
    spell_soul_steal() : SpellScriptLoader("spell_soul_steal") {}

    class spell_soul_steal_spell_script : public SpellScript
    {
        PrepareSpellScript(spell_soul_steal_spell_script);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TriggerSpell = 0;
            return true;
        }

        void HandleForceCast(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            if (!GetHitUnit() && GetHitUnit()->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            // Soulless
            GetHitUnit()->CastSpell(GetHitUnit(), eNhalishSpells::SpellSoulless);     

            // Caster is boss, but doing it with pointer to prevent a crash *twerks*
            if (InstanceScript* m_Instance = GetCaster()->GetInstanceScript())
            {
                if (Creature* l_Nhalish = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNhallish)))
                {
                    if (Creature* Soul = GetCaster()->SummonCreature(eNhalishCreatures::CreatureSoul, GetHitUnit()->GetPositionX(), GetHitUnit()->GetPositionY(), GetHitUnit()->GetPositionZ(), GetHitUnit()->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 20 * TimeConstants::IN_MILLISECONDS, GetHitUnit()->GetGUID()))
                    {
                        if (boss_nhalish::boss_nhalishAI* linkAI = CAST_AI(boss_nhalish::boss_nhalishAI, l_Nhalish->GetAI()))
                        {
                            switch (linkAI->PhaseCounter)
                            {
                            case 0:
                                m_phaseId = 2;
                                break;
                            case 1:
                                m_phaseId = 4;
                                break;
                            case 2:
                                m_phaseId = 6;
                                break;
                            case 3:
                                m_phaseId = 8;
                                break;
                            case 4:
                                m_phaseId = 10;
                                break;
                            case 5:
                                m_phaseId = 2;
                                break;
                            default:
                                break;;
                            }

                            linkAI->PhaseCounter++;

                            if (Creature* l_PropNhalish = GetCaster()->SummonCreature(eNhalishCreatures::CreatureNhalishProp, l_Nhalish->GetPositionX(), l_Nhalish->GetPositionY(), l_Nhalish->GetPositionZ(), l_Nhalish->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_PropNhalish->SetPhaseMask(m_phaseId, true);
                                GetHitUnit()->SetPhaseMask(m_phaseId, true);

                                if (Soul && Soul->GetAI())
                                {
                                    Soul->AI()->SetGUID(GetHitUnit()->GetGUID());
                                }
                            }
                        }
                    }
                }
            }          
        }

    private: 
        uint32 m_phaseId;

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_soul_steal_spell_script::HandleForceCast, EFFECT_0, SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_soul_steal_spell_script();
    }
};

void AddSC_nhalish()
{
    new boss_nhalish();

    new spell_soul_steal();
    new spell_void_devestation_trigger_missile();

    new areatrigger_void_vortex();

    new shadowmoon_burial_grounds_nhalish_prop();
    new shadowmoon_burial_grounds_soul();
    new shadowmoon_void_devestation_trigger();
}