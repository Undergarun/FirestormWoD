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
    SpellTeleportSelf                        = 153621,
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
    SpellShadowChannel                       = 46757,
    SpellLootSparkles                        = 92376,
    SpellCloneMe                             = 45204
};

enum eNhalishEvents
{
    EventPlanarShift = 1,
    EventVoidVortex,
    EventVoidBlast,
    EventVoidDevastation,
    EventSoulSteal,
    EventRandomMovement,
    EventExhumeCrypt
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
    GameObjectNhalishDoor     = 227851
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
    TalkSpell04   ///< Flee... flee from your your formal masters! [43207]
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
            HandleDoorActivation();
        }

        InstanceScript* m_Instance;
        uint32 m_PhaseCounter;
        uint32 m_VortexDiff;
        bool m_Vortex;

        void Reset() override
        {
            events.Reset();
            _Reset();

            m_PhaseCounter = 0;
            m_VortexDiff = 0;
            m_Vortex = false;
            
            me->CastSpell(me, eNhalishSpells::SpellShadowChannel);

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void HandleDoorActivation() // Only upon boss Defeat
        {
            std::list<GameObject*> l_ListDoors;
            me->GetGameObjectListWithEntryInGrid(l_ListDoors, eNhalishGameObjects::GameObjectNhalishDoor, 300.0f);

            if (l_ListDoors.empty())
                return;

            for (auto itr : l_ListDoors)
            {
                // Activate
                itr->SetLootState(LootState::GO_READY);
                itr->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
            }
        }

        void HandleDoorEntranceActivation() // Upon wipe
        {
            if (m_Instance != nullptr)
            {
                if (GameObject* l_Nhalish = m_Instance->instance->GetGameObject(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataNhalishDoorEntrance)))
                {
                    l_Nhalish->SetLootState(LootState::GO_READY);
                    l_Nhalish->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }
            }
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
                }
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();

            HandleDoorEntranceActivation();

            if (m_Instance != nullptr)
            {
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNhallish, EncounterState::FAIL);
            }

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

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eNhalishActions::ActionActivateVortex:
                    m_Vortex = true;
                    m_VortexDiff = 2000;
                    break;
                case eNhalishActions::ActionDeactivateVortex:
                    m_Vortex = false;

                    if (m_Instance != nullptr)
                        m_Instance->DoRemoveForcedMovementsOnPlayers(me->GetGUID());
                    break;
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();

            Talk(eNhallishTalks::TalkAggro);
            me->SetInCombatWithZone();

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNhallish, EncounterState::IN_PROGRESS);

                DoZoneInCombat();
            }

            HandleDoorEntranceActivation();
            HandleDoorActivation();

            events.ScheduleEvent(eNhalishEvents::EventPlanarShift, 15 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventSoulSteal, 70 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventVoidBlast, 6 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNhalishEvents::EventExhumeCrypt, 10 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eNhallishTalks::TalkKill01);
                else
                    Talk(eNhallishTalks::TalkKill02);

                p_Who->SetPhaseMask(me->GetPhaseMask(), true); // Sets dead target back to normal phase.
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();

            Talk(eNhallishTalks::TalkDeath);

            if (m_Instance != nullptr)
            {
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNhallish, EncounterState::DONE);
            }

            HandleDoorActivation();
            HandleDoorEntranceActivation();

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
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            // Reset phase Counter
            if (m_PhaseCounter >= 4)
            {
                m_PhaseCounter = 0;
            }

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

                    for (auto itr : l_ListPlayers)
                    {
                        if (itr->IsWithinDist(me, 25.0f, true))
                        {
                            if (itr->isAlive() && !itr->HasMovementForce(me->GetGUID()))
                                itr->SendApplyMovementForce(me->GetGUID(), true, l_Position, 3.0f, 1);
                            else if (!itr->isAlive() && itr->HasMovementForce(me->GetGUID()))
                                itr->SendApplyMovementForce(me->GetGUID(), false, l_Position);
                        }
                        else if (itr->HasMovementForce(me->GetGUID()))
                            itr->SendApplyMovementForce(me->GetGUID(), false, l_Position);
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
                    }

                    events.ScheduleEvent(eNhalishEvents::EventPlanarShift, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eNhalishEvents::EventVoidDevastation:
                    me->CastSpell(me, eNhalishSpells::SpellVoidDevastationPeriodicTriggerSpell);

                    Talk(eNhallishTalks::TalkSpell02);

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
                    me->CastSpell(me, eNhalishSpells::SpellVoidVortexAreaTrigger);

                    Talk(eNhallishTalks::TalkSpell01);

                    events.ScheduleEvent(eNhalishEvents::EventVoidVortex, 35 * TimeConstants::IN_MILLISECONDS);
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
                if (linkAI->m_victimGUID != NULL)
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

                        creature->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
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
        bool m_HasDied;

        void Reset()
        {         
            events.Reset();

            if (m_victimGUID != NULL)
            {
                if (Unit* victim = Unit::GetUnit(*me, m_victimGUID))
                {
                    me->CastSpell(victim, eNhalishSpells::SpellSoulShred); // automatically target the summoner TARGET_UNIT_SUMMONER 
                    me->CastSpell(me, eShadowmoonBurialGroundsSpells::SpellUnortodoxExistanceAuraDummy);
                }
            }

            me->SetReactState(ReactStates::REACT_PASSIVE);
        }

        void SetGUID(uint64 p_Guid, int32 /*p_Param*/) override
        {
            m_victimGUID = p_Guid;

            if (m_victimGUID != NULL)
            {
                if (Unit* l_Victim = Unit::GetUnit(*me, m_victimGUID))
                {
                    l_Victim->CastSpell(me, eNhalishSpells::SpellCloneMe);
                    me->CastSpell(l_Victim, eNhalishSpells::SpellSoulShred);
                }
            }

            events.ScheduleEvent(eNhalishEvents::EventRandomMovement, 1 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            m_HasDied = true;

            me->RemoveAllAuras();

            me->Respawn();
            me->setFaction(35);
            me->SetReactState(ReactStates::REACT_PASSIVE);

            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_NPC_FLAGS, NPCFlags::UNIT_NPC_FLAG_GOSSIP);

            me->CastSpell(me, eNhalishSpells::SpellLootSparkles);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
               case eNhalishEvents::EventRandomMovement:
                   me->GetMotionMaster()->MoveRandom(4.0f);
                break;
            }
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
            events.Reset();

            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
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
                    if (l_Nhalish->isAlive() && l_Nhalish->isInCombat())
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
class shadowmoon_burial_grounds_void_devestation_trigger : public CreatureScript
{
public:

    shadowmoon_burial_grounds_void_devestation_trigger() : CreatureScript("shadowmoon_burial_grounds_void_devestation_trigger") { }

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) {}

        uint32 m_Timer;

        void Reset() override
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
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
class spell_shadowmoon_burial_grounds_void_devestation_trigger : public SpellScriptLoader
{
public:

    spell_shadowmoon_burial_grounds_void_devestation_trigger() : SpellScriptLoader("spell_shadowmoon_burial_grounds_void_devestation_trigger") { }

    class spell_shadowmoon_burial_grounds_void_devestation_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_shadowmoon_burial_grounds_void_devestation_trigger_SpellScript);

        bool Load()
        {
            if (!GetSpellInfo())
                return false;

            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetA = Targets::TARGET_DEST_CASTER;
            spell->Effects[0].TargetB = Targets::TARGET_DEST_DEST_RADIUS;
            spell->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(10); // 30y
            return true;
        }

        void HandleTriggerMissile(SpellEffIndex p_EffIndex)
        {
            PreventHitDefaultEffect(p_EffIndex);

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
            OnEffectHitTarget += SpellEffectFn(spell_shadowmoon_burial_grounds_void_devestation_trigger_SpellScript::HandleTriggerMissile, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_TRIGGER_MISSILE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_shadowmoon_burial_grounds_void_devestation_trigger_SpellScript();
    }
};

/// Void Blast - 152792  
class spell_shadowmoon_burial_grounds_void_blast : public SpellScriptLoader
{
public:

    spell_shadowmoon_burial_grounds_void_blast() : SpellScriptLoader("spell_shadowmoon_burial_grounds_void_blast") { }

    class spell_shadowmoon_burial_grounds_void_blast_SpellScript : public AuraScript
    {
        PrepareAuraScript(spell_shadowmoon_burial_grounds_void_blast_SpellScript);

        void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->IsAIEnabled)
                {
                    if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    {
                        l_Caster->CastSpell(l_Target, eNhalishSpells::SpellVoidBlastDot);
                    }
                }
            }
        }

        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                    {
                        l_Instance->DoRemoveAurasDueToSpellOnPlayers(eNhalishSpells::SpellVoidBlastDot);
                    }
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_shadowmoon_burial_grounds_void_blast_SpellScript::OnApply,    SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_shadowmoon_burial_grounds_void_blast_SpellScript::OnRemove, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_shadowmoon_burial_grounds_void_blast_SpellScript();
    }
};

/// Void Vortex - 152801 
class spell_shadowmoon_burial_grounds_void_vortex : public SpellScriptLoader
{
public:

    spell_shadowmoon_burial_grounds_void_vortex() : SpellScriptLoader("spell_shadowmoon_burial_grounds_void_vortex") { }

    class spell_shadowmoon_burial_grounds_void_vortex_SpellScript : public AuraScript
    {
        PrepareAuraScript(spell_shadowmoon_burial_grounds_void_vortex_SpellScript);

        void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->IsAIEnabled)
                {
                    if (l_Caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                    {
                        l_Caster->GetAI()->DoAction(eNhalishActions::ActionActivateVortex);
                    }
                }
            }
        }

        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Caster->GetAI()->DoAction(eNhalishActions::ActionDeactivateVortex);
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_shadowmoon_burial_grounds_void_vortex_SpellScript::OnApply, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_shadowmoon_burial_grounds_void_vortex_SpellScript::OnRemove, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_shadowmoon_burial_grounds_void_vortex_SpellScript();
    }
};

/// Soul Steal - 152962 
class spell_shadowmoon_burial_grounds_soul_steal : public SpellScriptLoader
{
public:

    spell_shadowmoon_burial_grounds_soul_steal() : SpellScriptLoader("spell_shadowmoon_burial_grounds_soul_steal") {}

    class spell_shadowmoon_burial_grounds_soul_steal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_shadowmoon_burial_grounds_soul_steal_SpellScript);

        bool Load()
        {
            if (!GetSpellInfo())
                return false;

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
                            uint32 m_phaseId = NULL;

                            switch (linkAI->m_PhaseCounter)
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
                                    m_phaseId = 12;
                                    break;
                                default:
                                    break;
                            }

                            /// Sets new phase value
                            GetHitUnit()->SetPhaseMask(m_phaseId, true);
                            Soul->SetPhaseMask(m_phaseId, true);

                            if (Soul->IsAIEnabled)
                            {
                                Soul->AI()->SetGUID(GetHitUnit()->GetGUID(), 0);
                            }

                            linkAI->m_PhaseCounter++;
                        }
                    }
                }
            }
        }
  
        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_shadowmoon_burial_grounds_soul_steal_SpellScript::HandleForceCast, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_shadowmoon_burial_grounds_soul_steal_SpellScript();
    }
};

/// Planar Shift - 153623   
class spell_shadowmoon_burial_grounds_planar_shift : public SpellScriptLoader
{
public:

    spell_shadowmoon_burial_grounds_planar_shift() : SpellScriptLoader("spell_shadowmoon_burial_grounds_planar_shift") {}

    class spell_shadowmoon_burial_grounds_teleport_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_shadowmoon_burial_grounds_teleport_SpellScript);

        void HandleAfterCast()
        {
            if (!GetCaster())
                return;

            if (!GetCaster()->IsAIEnabled)
                return;

            if (Unit* l_Target = GetCaster()->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                GetCaster()->CastSpell(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), eNhalishSpells::SpellTeleportSelf, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_shadowmoon_burial_grounds_teleport_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_shadowmoon_burial_grounds_teleport_SpellScript();
    }
};

/// Void Devestation - 153072 
class areatrigger_void_devestation : public AreaTriggerEntityScript
{
public:

    areatrigger_void_devestation() : AreaTriggerEntityScript("areatrigger_void_devestation")
    {
    }

    uint32 m_Diff = 500;
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

void AddSC_nhalish()
{
    new boss_nhalish();
    new shadowmoon_burial_grounds_nhalish_prop();
    new shadowmoon_burial_grounds_soul();
    new spell_shadowmoon_burial_grounds_void_devestation_trigger();
    new spell_shadowmoon_burial_grounds_planar_shift();
    new spell_shadowmoon_burial_grounds_soul_steal();
    new spell_shadowmoon_burial_grounds_void_blast();
    new spell_shadowmoon_burial_grounds_void_vortex();
    new areatrigger_void_devestation();
}