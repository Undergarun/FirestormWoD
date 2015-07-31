#include "ScriptedCreature.h"
#include "auchindon.hpp"

enum AzzakelSpells
{
    SPELL_CLAWS_OF_ARGUS_BUFF = 153762,
    SPELL_CLAWS_OF_ARGUS_VISUAL = 153764,
    SPELL_CLAWS_OF_ARGUS_DMG = 153772,
    SPELL_CURTAIN_OF_FLAME_AURA = 153392,
    SPELL_CURTAIN_OF_FLAME_FORCE_CAST = 153396, //(daniel:need player to chack this...)
    SPELL_CURTAIN_OF_FLAME_VISUAL = 153400,
    SPELL_FEL_LASH_VISUAL = 153234,
    SPELL_FEL_LASH_DUMMY = 174872,
    SPELL_FEL_LASH_DEBUFF = 177120,
    SPELL_FEL_LASH_DEBUFF_TWO = 177121,
    SPELL_MALEVILENT_CRUSH = 153499, // JUMP
    SPELL_FEL_POOL_AREATRIGER = 153500, // visual part of Malevolent Crash
    SPELL_FEL_POOL_DEBUFF_DMG = 153616,

    // visuals
    SPELL_VISUAL_FEL_BURST = 169682,

    // heroics
    SPELL_FEL_SPARK_AREA_TRIGGER = 153725,
    SPELL_FEL_SPARK_DAMAGE = 153726,
    SPELL_FEL_SPARK_PERIOIDIC_CREATION = 153727,
};
enum AzzakelEvents
{
    // azzakel
    EVENT_CLAWS_OF_ARGUS = 70,
    EVENT_CURTAIN_OF_FLAME = 71,
    EVENT_FEL_LASH = 72,
    EVENT_MALEVILENT_CRUSH = 73,
    EVENT_FEL_POOL = 74,
    EVENT_MALEVOLENT_CRUSH_2 = 75,
    // controller
    EVENT_SUMMON_AZZAKEL = 76,
    EVENT_SUMMON_AZZAKEL_2 = 77,
    // Heroic
    EVENT_FEL_SPARK = 78,
};
enum AzzakelTalks
{
    AZZAKEL_INTRO = 37, // Who dares meddlie in the works of the legion?! (46776) 
    AZZAKEL_AGGRO = 38, // This world...ALL world...shell burn!(46774)
    AZZAKEL_SPELL_03 = 39, // Die, insect!(46781)
    AZZAKEL_SPELL_02 = 40, // Burn in the master's Fire!(46780)
    AZZAKEL_SPELL_01 = 41, // Come forth, servants!(46779)
    AZZAKEL_KILL_01 = 42, // The masters Blase your soul! (46777)
    AZZAKEL_KILL_02 = 43, // Burn! (46778)
    AZZAKEL_DEATH = 44, // (46775)
};
enum AzzakelTriggers
{
    TRIGGER_FEL_POOL = 326526,
    TRIGGER_FEL_SPARK = 76197,
    TRIGGER_DEMON_SUMMONING = 432636,
};
enum AzzakelActions
{
    ACTION_FEL_SPARK = 595,
    ACTION_SUMMON_DEMONS = 596,
    ACTION_RENEW_EVENTS = 597,
    ACTION_BOOL_ACTIVATE = 598,
    ACTION_BOOL_DEACTIVATE = 599,
};
Position Azzakel_Blackgate = { 1929.65f, 2699.27f, 30.799f, 4.428220f };
Position SpawningFlyCoords[2] =
{
    { 1912.13f, 2720.44f, 49.818f, 1.600908f },
    { 1911.65f, 2757.73f, 30.799f, 4.748000f },
};
Position Azzakel_Blackgate_little[4] =
{
    { 1911.90f, 2680.62f, 31.418f, 1.450705f },
    { 1911.79f, 2764.35f, 31.418f, 4.721891f },
    { 1953.55f, 2722.47f, 31.418f, 3.139304f },
    { 1869.70f, 2722.45f, 31.418f, 0.001632f },
};

class auchindon_azzakel_controller : public CreatureScript
{
public:
    auchindon_azzakel_controller() : CreatureScript("auchindon_azzakel_controller") { }

    struct auchindon_creatures : public Scripted_NoMovementAI
    {
        auchindon_creatures(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            counting = 0;
            summoned = false;

            me->setFaction(35);
            me->SetReactState(REACT_PASSIVE);
            /*
            // Get Barrier Down
            if (GameObject* felbarrier = instance->instance->GetGameObject(instance->GetData64(DATA_FEL_BARRIER)))
            {
            felbarrier->SetLootState(GO_READY);
            felbarrier->UseDoorOrButton();
            }
            */

            // manual shite
            std::list<GameObject*> list;
            me->GetGameObjectListWithEntryInGrid(list, GAMEOBJECT_FEL_BARRIER, 100.0f);

            if (list.empty())
                return;

            for (auto itr : list)
            {
                itr->Delete();
            }

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_DISABLE_TURN);
        }

        InstanceScript* instance = me->GetInstanceScript();

        int32 counting;
        bool summoned;
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_COUNT_PRE_3ST_BOSS_KILL:
                counting = counting + 1;

                if (counting > 6 && !summoned)
                {
                    summoned = true;
                    me->NearTeleportTo(1911.50f, 2722.52f, 30.799f, Azzakel_Blackgate.GetOrientation());

                    me->CastSpell(me, SPELL_VISUAL_FEL_BURST);
                    events.ScheduleEvent(EVENT_SUMMON_AZZAKEL, 2000);
                }
                break;
            }
        }
        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_SUMMON_AZZAKEL:
            {
                Azzakel = me->SummonCreature(BOSS_AZAAKEL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                GameObject* portal = me->SummonGameObject(GAMEOBJECT_DEMONIC_PORTAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), 0, 0, 0, 0, 0);
                if (portal)
                portal->SetFlag(11, GO_FLAG_NOT_SELECTABLE | GO_FLAG_NODESPAWN | GO_FLAG_INTERACT_COND);
                if (Azzakel)
                Azzakel->GetMotionMaster()->MoveJump(SpawningFlyCoords[1].GetPositionX(), SpawningFlyCoords[1].GetPositionY(), SpawningFlyCoords[1].GetPositionZ(), 30.0f, 25.0f);
                events.ScheduleEvent(EVENT_SUMMON_AZZAKEL_2, 1000);
                break;
            }
            case EVENT_SUMMON_AZZAKEL_2:
                Azzakel->GetMotionMaster()->MoveCharge(1911.93f, 2754.40f, 30.973f, 42.0f);
                //Azzakel->SetFacingToObject(me);
                Azzakel->setFaction(16);
                break;
            }
        }
    private:
        Creature* Azzakel;
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
class auchindon_boss_azzakel : public CreatureScript
{
public:
    auchindon_boss_azzakel() : CreatureScript("auchindon_boss_azzakel") { }

    struct boss_azzakelAI : public BossAI
    {
        boss_azzakelAI(Creature* creature) : BossAI(creature, DATA_AZZAKAEL)
        {
            instance = me->GetInstanceScript();
            intro = false;

            me->setFaction(35);
        }

        bool intro;
        uint32 interval;

        bool argus;
        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {
            _Reset();
            interval = 2500;
            argus = false;

            DespawnCreaturesInArea(TRIGGER_DEMON_SUMMONING, me);
            DespawnCreaturesInArea(CREATURE_CACKLING_PYROMANIAC, me);
            DespawnCreaturesInArea(CREATURE_BLAZING_TRICKSTER, me);
            DespawnCreaturesInArea(CREATURE_FELGUARD, me);
        }
        void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 5000.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
        void MoveInLineOfSight(Unit* who)
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 18.0f) && !intro)
            {
                intro = true;
                Talk(AZZAKEL_INTRO);
            }
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_BOOL_DEACTIVATE:
                argus = false;
                me->GetMotionMaster()->Clear(); 
                break;
            case ACTION_BOOL_ACTIVATE:
                argus = true;
                break;
            case ACTION_FEL_SPARK:
                events.ScheduleEvent(EVENT_FEL_SPARK, 1000);
                break;
            case ACTION_SUMMON_DEMONS:
            {
                me->SummonCreature(CREATURE_FELGUARD, Azzakel_Blackgate_little[1], TEMPSUMMON_MANUAL_DESPAWN);
                me->SummonCreature(CREATURE_CACKLING_PYROMANIAC, Azzakel_Blackgate_little[2], TEMPSUMMON_MANUAL_DESPAWN);
                me->SummonCreature(CREATURE_BLAZING_TRICKSTER, Azzakel_Blackgate_little[3], TEMPSUMMON_MANUAL_DESPAWN);
                me->SummonCreature(CREATURE_BLAZING_TRICKSTER, Azzakel_Blackgate_little[4], TEMPSUMMON_MANUAL_DESPAWN);
                break;
            }
            case ACTION_RENEW_EVENTS:
            {
                events.Reset();
                me->GetMotionMaster()->Clear();

                if (me->getVictim())
                    me->GetMotionMaster()->MoveFollow(me->getVictim(), 0, 0, MOTION_SLOT_ACTIVE);

                events.ScheduleEvent(EVENT_FEL_LASH, 8000);
                events.ScheduleEvent(EVENT_CLAWS_OF_ARGUS, 45000);
                events.ScheduleEvent(EVENT_CURTAIN_OF_FLAME, 14000);
                events.ScheduleEvent(EVENT_MALEVOLENT_CRUSH_2, 20000);
                break;
            }
            }
        }
        void JustReachedHome() override
        {
            _JustReachedHome();

            // manual shite
            std::list<GameObject*> list;
            me->GetGameObjectListWithEntryInGrid(list, GAMEOBJECT_FEL_BARRIER, 100.0f);

            if (list.empty())
                return;

            for (auto itr : list)
            {
                itr->Delete();
            }

            if (instance)
                instance->SetBossState(DATA_AZZAKAEL, FAIL);

            DespawnCreaturesInArea(CREATURE_FELGUARD, me);
            DespawnCreaturesInArea(CREATURE_CACKLING_PYROMANIAC, me);
            DespawnCreaturesInArea(CREATURE_BLAZING_TRICKSTER, me);
        }
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();
            Talk(AZZAKEL_AGGRO);

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }

            instance->SetBossState(DATA_AZZAKAEL, IN_PROGRESS);

            me->SummonGameObject(GAMEOBJECT_FEL_BARRIER, 1911.01f, 2722.89f, 30.799f, Azzakel_Blackgate.GetOrientation(), 0, 0, 0, 0, 0);

            events.ScheduleEvent(EVENT_FEL_LASH, 8000);
            events.ScheduleEvent(EVENT_CLAWS_OF_ARGUS, 45000);
            events.ScheduleEvent(EVENT_CURTAIN_OF_FLAME, 14000);
            events.ScheduleEvent(EVENT_MALEVOLENT_CRUSH_2, 20000);
        }
        void KilledUnit(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                if (roll_chance_i(50))
                    Talk(AZZAKEL_KILL_01);
                else
                    Talk(AZZAKEL_KILL_02);
        }
        void JustDied(Unit* /*killer*/) override
        {
            Talk(AZZAKEL_DEATH);
            _JustDied();

            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);;
            instance->SetBossState(DATA_AZZAKAEL, DONE);

            DespawnCreaturesInArea(CREATURE_FELGUARD, me);
            DespawnCreaturesInArea(CREATURE_CACKLING_PYROMANIAC, me);
            DespawnCreaturesInArea(CREATURE_BLAZING_TRICKSTER, me);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (argus) // claws of argus
            {
                if (interval <= diff)
                {
                    DoAction(ACTION_SUMMON_DEMONS);
                    argus = false;
                }
                else
                    interval -= diff;
            }


            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FEL_LASH:
                    me->CastSpell(me->getVictim(), SPELL_FEL_LASH_VISUAL);
                    events.ScheduleEvent(EVENT_FEL_LASH, urand(8000, 12000));
                    break;
                case EVENT_CLAWS_OF_ARGUS:
                {
                    Talk(AZZAKEL_SPELL_01);

                    events.Reset();
                    me->CastSpell(me, 153764); // right spell for claws of argus
                    me->MonsterTextEmote("Azzakel casts |cffff0000[Azzakael casts [Claws of Agrus]|cfffaeb00!", me->GetGUID(), true);
                    events.ScheduleEvent(EVENT_CLAWS_OF_ARGUS, 45000);
                    break;
                }
                case EVENT_CURTAIN_OF_FLAME:
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        Talk(AZZAKEL_SPELL_03);

                        me->AddAura(SPELL_CURTAIN_OF_FLAME_AURA, target);

                        std::string str;
                        str += "Azzakel casts |cffff0000[Azzakael casts Curtain of Flame]|cfffaeb00! on ";
                        str += target->GetName();

                        me->MonsterTextEmote(str.c_str(), me->GetGUID(), true);

                        events.ScheduleEvent(EVENT_CURTAIN_OF_FLAME, urand(8000, 15000));
                        break;
                    }
                }
                case EVENT_MALEVILENT_CRUSH:
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->GetMotionMaster()->MoveJump(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 15.0f, 20.0f);
                    Talk(AZZAKEL_SPELL_02);

                    events.ScheduleEvent(EVENT_MALEVOLENT_CRUSH_2, 20000);
                    break;
                }
                case EVENT_MALEVOLENT_CRUSH_2:
                {
                    me->SummonCreature(TRIGGER_FEL_POOL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                    me->CastSpell(me, SPELL_FEL_POOL_AREATRIGER);
                    break;
                }
                case EVENT_FEL_SPARK:
                {
                    Position pos;
                    me->GetPosition(&pos);

                    for (int i = 0; i < 2; i++)
                    {
                        me->SummonCreature(TRIGGER_FEL_SPARK, pos, TEMPSUMMON_MANUAL_DESPAWN);
                    }
                    break;
                }
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_azzakelAI(creature);
    }
};
class auchindon_azzakel_spell_curtain_flames : public SpellScriptLoader
{
public:
    auchindon_azzakel_spell_curtain_flames() : SpellScriptLoader("auchindon_azzakel_spell_curtain_flames") { }

    class spell_auras : public AuraScript
    {
        PrepareAuraScript(spell_auras)

            void OnPeriodic(constAuraEffectPtr aurEff)
        {
            if (!GetTarget())
                return;

            if (GetTarget()->GetTypeId() != TYPEID_PLAYER)
                return;

            std::list<Player*> pl_list;
            GetTarget()->GetPlayerListInGrid(pl_list, 4.0f);

            if (pl_list.empty())
                return;

            for (auto itr : pl_list)
            {
                if (GetTarget()->GetGUID() != itr->GetGUID())
                {
                    GetTarget()->AddAura(SPELL_CURTAIN_OF_FLAME_AURA, itr);
                    GetTarget()->CastSpell(GetTarget(), SPELL_CURTAIN_OF_FLAME_VISUAL);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_auras::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_auras();
    }
};
class auchindon_azzakel_spell_claws_of_argus : public SpellScriptLoader
{
public:
    auchindon_azzakel_spell_claws_of_argus() : SpellScriptLoader("auchindon_azzakel_spell_claws_of_argus") { }

    class auchindon_auras : public AuraScript
    {
        PrepareAuraScript(auchindon_auras);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[1].Effect = 0;
            spell->Effects[1].TargetA = 0;
            return true;
        }
        void HandlePeriodic(constAuraEffectPtr aurEff)
        {
  
        }
        void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                caster->SetCanFly(true);
                caster->GetMotionMaster()->MoveTakeoff(0, caster->GetPositionX(), caster->GetPositionY(), 42.118f);
                caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                //caster->ToCreature()->SetReactState(REACT_PASSIVE);  
                caster->GetAI()->DoAction(ACTION_BOOL_ACTIVATE);
            }
        }
        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (caster->GetTypeId() != TYPEID_PLAYER)
                {
                    caster->GetAI()->DoAction(ACTION_BOOL_DEACTIVATE);

                    caster->GetAI()->DoAction(ACTION_RENEW_EVENTS);
                    caster->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    caster->RemoveAllAuras();
                    if (caster->GetMap()->IsHeroic())
                    {
                        if (Player* nearest = caster->FindNearestPlayer(20.0f, true))
                        {
                            caster->GetMotionMaster()->MoveCharge(nearest->GetPositionX(), nearest->GetPositionY(), nearest->GetPositionZ(), 42.0f);
                            caster->GetAI()->DoAction(ACTION_FEL_SPARK);
                        }
                    }
                    else
                    {
                        GetCaster()->GetMotionMaster()->MovePoint(0, GetTarget()->GetPositionX(), GetTarget()->GetPositionY(), 35.590f);
                    }

                    caster->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_auras::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            AfterEffectApply += AuraEffectApplyFn(auchindon_auras::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(auchindon_auras::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new auchindon_auras();
    }
};
// called by
/*
case 0:
entry = 164080; //CREATURE_FELGUARD;
break;
case 1:
entry = 164127;// CREATURE_CACKLING_PYROMANIAC;
break;
case 2:
entry = 153775;// CREATURE_BLAZING_TRICKSTER;
break;
*/
class auchindon_azzakel_spell_target_chance : public SpellScriptLoader
{
public:
    auchindon_azzakel_spell_target_chance() : SpellScriptLoader("auchindon_azzakel_spell_target_chance") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);
        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
            spell->Effects[0].TargetB = 0;
            return true;
        }
        void CorrectTargets(std::list<WorldObject*>& targets)
        {
        }
        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, EFFECT_0, TARGET_DEST_CASTER_RADIUS);
        }
    };
    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};

class auchindon_azzakel_fel_spark_area_trigger : public AreaTriggerEntityScript
{
public:
    auchindon_azzakel_fel_spark_area_trigger() : AreaTriggerEntityScript("auchindon_azzakel_fel_spark_area_trigger")
    {
    }

    int diff = 500;
    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (diff <= p_Time)
        {
            std::list<Player*> targets;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, targets, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

            if (targets.empty())
                return;

            for (std::list<Player*>::const_iterator it = targets.begin(); it != targets.end(); ++it)
            {
                if (!(*it))
                    return;

                if (!(*it)->HasAura(SPELL_FEL_POOL_DEBUFF_DMG))
                {
                    (*it)->AddAura(SPELL_FEL_POOL_DEBUFF_DMG, (*it));
                }
                else
                {
                    AuraPtr aura = (*it)->GetAura(SPELL_FEL_POOL_DEBUFF_DMG);

                    if (aura)
                    {
                        aura->SetMaxDuration(1);
                        aura->SetDuration(1);
                    }
                }
            }

            diff = 500;
        }
        else
        {
            diff -= p_Time;
        }
    }
    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
    }

    auchindon_azzakel_fel_spark_area_trigger* GetAI() const
    {
        return new auchindon_azzakel_fel_spark_area_trigger();
    }
};
class auchindon_azzakel_fel_pool_trigger : public CreatureScript
{
public:
    auchindon_azzakel_fel_pool_trigger() : CreatureScript("auchindon_azzakel_fel_pool_trigger") {}

    struct auchindon_triggers : public Scripted_NoMovementAI
    {
        auchindon_triggers(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetDisplayId(11686);
            me->setFaction(16);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }

        void UpdateAI(const uint32 diff)
        {
            std::list<Player*> targets;

            JadeCore::AnyPlayerInObjectRangeCheck check(me, 15.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, targets, check);
            me->VisitNearbyObject(15.0f, searcher);

            if (targets.empty())
                return;

            for (std::list<Player*>::const_iterator it = targets.begin(); it != targets.end(); ++it)
            {
                if (!(*it))
                    return;

                if ((*it)->IsWithinDistInMap(me, 4.0f))
                {
                    if (!(*it)->HasAura(SPELL_FEL_POOL_DEBUFF_DMG))
                    {
                        me->AddAura(SPELL_FEL_POOL_DEBUFF_DMG, (*it));
                    }
                }
                else
                {
                    if ((*it)->HasAura(SPELL_FEL_POOL_DEBUFF_DMG, me->GetGUID()))
                        (*it)->RemoveAura(SPELL_FEL_POOL_DEBUFF_DMG);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_triggers(creature);
    }
};

// Heroic
class auchindon_azzakel_fel_spark_trigger : public CreatureScript
{
public:
    auchindon_azzakel_fel_spark_trigger() : CreatureScript("auchindon_azzakel_fel_spark_trigger") {}

    struct auchindon_triggers : public Scripted_NoMovementAI
    {
        auchindon_triggers(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetDisplayId(11686);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            me->Respawn(true);
            me->GetAI()->Reset();
        }
        void Reset()
        {
            me->AddAura(SPELL_FEL_SPARK_PERIOIDIC_CREATION, me);
            me->DespawnOrUnsummon(10000);

            Position pos;
            me->GetRandomNearPosition(pos, 15.0f);

            me->GetMotionMaster()->MovePoint(0, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_triggers(creature);
    }
};
/*
entry = 164080; //CREATURE_FELGUARD;
break;
case 1:
entry = 164127;// CREATURE_CACKLING_PYROMANIAC;
break;
case 2:
entry = 153775;// CREATURE_BLAZING_TRICKSTER;
*/
class auchindon_azzakael_spell_demon_target_change : public SpellScriptLoader
{
public:
    auchindon_azzakael_spell_demon_target_change() : SpellScriptLoader("auchindon_azzakael_spell_demon_target_change") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            //spell->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
            //spell->Effects[0].TargetB = 0;
            return true;
        }
        void HandleTriggerMissile(SpellEffIndex effIndex)
        {
            //PreventHitDefaultEffect(effIndex);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindon_spells::HandleTriggerMissile, EFFECT_0, TARGET_DEST_CASTER_RADIUS);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};
void AddSC_azzakel()
{
    new auchindon_boss_azzakel();
    new auchindon_azzakel_controller();
    new auchindon_azzakel_spell_curtain_flames();
    new auchindon_azzakel_spell_claws_of_argus();
    new auchindon_azzakel_spell_target_chance();
    new auchindon_azzakael_spell_demon_target_change();
    new auchindon_azzakel_fel_spark_area_trigger();
    new auchindon_azzakel_fel_spark_trigger();
    new auchindon_azzakel_fel_pool_trigger();
}