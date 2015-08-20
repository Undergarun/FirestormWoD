#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "auchindon.hpp"

enum NyamiSpells
{
    SPELL_SHADOW_WORD_PAIN = 154477,
    SPELL_SOUL_VESSEL_DUMMY = 155327,
    SPELL_SOUL_BUBBLE_VISUAL = 177549,
    SPELL_SOUL_BUBBLE_BUFF = 177550,
    //SPELL_SOUL_VESSEL_DUMMY = 153477, //  Twists the souls of fallen Draenei, forming multiple tori of Shadow damage at a specific location. The epicenter of this ability is safe from damage.
    SPELL_SOUL_VESSEL_DMG = 154187,
    SPELL_SOUL_VESSEL_AREATRIGGER = 153888,
    SPELL_TORN_SPRITS_DUMMY = 153994,
    SPELL_TORN_SPRITS_DUMMY_TWO = 153991, // Rips the spirits out of fallen Auchenai partisans, forcing them to fight.

    // Spiteful Arbitrer
    SPELL_ARBITRER_HAMMER = 154218,
    SPELL_RADIANT_FURY_VISUAL_STAR = 157787,
    SPELL_RADIANT_FURY_DUMMY = 154261,
    SPELL_RADIANT_FULLY_VISUAL = 154264,
    SPELL_RADIANT_DAMAGE = 154301,
    SPELL_RADIANT_FURY_JUMP = 154262,

    // Malefic Defender
    SPELL_CRUSADER_STRIKE = 176931,

    // Twisted Magus
    SPELL_ARCANE_BOLT = 154235,
};
enum NyamiEvents
{
    // Nyami
    EVENT_SHADOW_WORD_PAIN = 56,
    EVENT_SOUL_VESSEL = 57,
    EVENT_TORN_SPIRIT= 61,
    EVENT_TORN_SPIRIT_NYAMI_EFFECT = 62,
    EVENT_TORN_SPIRITS_DUMMY = 70,

    // Adds
    // Spiritful_arbitar
    EVENT_ARBITRER_HAMMER = 63,
    EVENT_RADIANT_FURY = 64,
    EVENT_RADIANT_FURY_SUMMON_TRIGGER = 65,
    EVENT_RADIANT_FURY_STOP = 66,
    // Twisted_magus
    EVENT_ARCANE_BOLT = 67,
    // Malefic Defender
    EVENT_CRUSADER_STRIKE = 69,
};
enum NyamiTalks
{
    NYAMI_SPELL1 = 1,   // Your oath is unfinished! (43647)
    NYAMI_SPELL3 = 2,   // The spirits are mine to command! (43649)
    NYAMI_SPELL2 = 3,   // Return to this world! (43648)
    NYAMI_SPELL4 = 4,   // I will drown this world in shadows!(43650)
    NYAMI_AGGRO = 5,   // I will coil your souls into darkness!(43636)
    NYAMI_SLAY = 6,   // Shadows Envelop you! (43646)
    NYAMI_DEATH = 7,   // Too late...my Master...comes...(43637) 
};
enum NyamiActions
{
    ACTION_SUMMON_SPIRITS = 700,
};
enum NyamiCreatures
{
    CREATURE_SPITEFUL_ARBITRER = 76284,
    CREATURE_TWISTED_MAGUS = 76296,
    CREATURE_MALEFIC_DEFENDER = 76283,
};
enum NyamiTriggers
{
    TRIGGER_RADIANT_FURY = 432626,
    TRIGGER_SOUL_VESSEL_HACK_BUBBLE_EFFECT = 342652,
};
Position dead_guards[4] =
{
    { 1645.85f, 2937.17f, 35.166f, 1.454027f },
    { 1645.02f, 2971.30f, 35.140f, 6.229243f },
    { 1675.58f, 2972.18f, 35.139f, 3.197360f },
    { 1674.51f, 2932.11f, 35.119f, 3.219360f },
};
Position SAFE_ZONE_SOUL_VESSEL = { 1658.81f, 2957.50f, 34.291f, 5.099889f };
Position Bubble_Position = {1660.65f, 2953.06f, 34.291f, 1.542292f };
enum WardenTalks
{
    AUCHENAI_WARDEN_1 = 32, // Champions! Nyami's agents - they went this way.
    AUCHENAI_WARDEN_2 = 33, // No...Oh...no...
    AUCHENAI_WARDEN_3 = 34, // The barrier protecting Auchindoun is asunder.
    AUCHENAI_WARDEN_4 = 35, // A foul force has penetrated these sacerd chambers.
    AUCHENAI_WARDEN_5 = 36,// Scount ahead while we establish a foothold.
};
Position WardenSpawnPoint = {1660.97f, 2918.35f, 34.763f, 1.672848f};
Position WardenPosition1st = { 1686.05f, 2840.95f, 35.140f };
/*
Position WardenPosition2nd = { 1669.53f, 2876.96f, 35.279f, 5.298377f };
Position WardenPosition3rd = { 1669.53f, 2876.96f, 35.279f, 5.298377f };
Position WardenPosition4th = { 1669.53f, 2876.96f, 35.279f, 5.298377f };
Position WardenPosition5th = { 1669.53f, 2876.96f, 35.279f, 5.298377f };
*/

class nyami_after_fight_pre_azzakel_event : public BasicEvent
{
public:
    explicit nyami_after_fight_pre_azzakel_event(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (obj)
        {
            if (InstanceScript* instance = obj->GetInstanceScript())
            {
                if (Creature* Warden = instance->instance->GetCreature(instance->GetData64(DATA_WARDEN)))
                    {
                        switch (modifier)
                        {
                            case 0:
                            {
                                Warden->AI()->Talk(AUCHENAI_WARDEN_1);
                                Warden->RemoveAura(SPELL_KNEEL);

                                if (GameObject* door = Warden->FindNearestGameObject(GAMEOBJECT_HOLY_WALL, 30.0f))
                                    door->Delete();

                                Warden->GetMotionMaster()->MovePoint(0, WardenPosition1st);
                                //Warden->m_Events.AddEvent(new nyami_after_fight_pre_azzakel_event(Warden, 1), Warden->m_Events.CalculateTime(7000));
                                break;
                            }
                            case 1:
                            {
                                if (GameObject* door = instance->instance->GetGameObject(instance->GetData64(DATA_AUCHINDON_WINDOW)))
                                {     
                                        // Activate
                                    door->SetLootState(GO_READY);
                                    door->UseDoorOrButton(10000000);
                                }
                                // fel bourne
                                Warden->SummonCreature(CREATURE_FELBORNE_ABYSSAL, Warden->GetPositionX(), Warden->GetPositionY(), Warden->GetPositionZ(), TEMPSUMMON_MANUAL_DESPAWN);

                                Warden->SetReactState(REACT_PASSIVE);
                                Warden->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);
                                Warden->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                Warden->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                                Warden->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                Warden->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                                Warden->AI()->Talk(AUCHENAI_WARDEN_2);
                                break;
                            }
                        }
                    }
                }
                return true;
            }
    }
private:
    Creature* storm;
    Unit* obj;
    int modifier;
    int Event;
};
class auchindon_mob_warden_cosmetic : public CreatureScript
{
public:
    auchindon_mob_warden_cosmetic() : CreatureScript("auchindon_mob_warden_cosmetic") {}

    struct mob_wardenAI : public CreatureAI
    {
        mob_wardenAI(Creature* creature) : CreatureAI(creature)
        {
            me->AddAura(SPELL_KNEEL, me);
        }
        EventMap events;
        void MovementInform(uint32 /*type*/, uint32 id)
        {
            switch (id)
            {
            case 0:
                me->m_Events.AddEvent(new nyami_after_fight_pre_azzakel_event(me, 1), me->m_Events.CalculateTime(1000));
                break;
            }
        }
        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                default:
                    break;
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wardenAI(creature);
    }
};
class auchindon_boss_nyami : public CreatureScript
{
public:
    auchindon_boss_nyami() : CreatureScript("auchindon_boss_nyami") { }

    struct boss_nyamiAI : public BossAI
    {
        boss_nyamiAI(Creature* creature) : BossAI(creature, DATA_BOSS_NYAMI)
        {
            pinstance = me->GetInstanceScript();

            me->SetRespawnTime(6000000);
            Reset();
        }
        InstanceScript* pinstance = me->GetInstanceScript();

        uint32 diffVisual;
        uint32 diffChannel;

        bool canchain;
        void Reset()
        {
            _Reset();
            diffVisual = 7000;
            diffChannel = 2000;
            summons.DespawnAll();
            events.Reset();

            canchain = false;
        }
        void JustReachedHome() override
        {
            if (instance)
                instance->SetBossState(DATA_NYAMI, FAIL);

            DespawnCreaturesInArea(CREATURE_TWISTED_MAGUS, me);
            DespawnCreaturesInArea(CREATURE_MALEFIC_DEFENDER, me);
            DespawnCreaturesInArea(CREATURE_SPITEFUL_ARBITRER, me);
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
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();

            if (instance)
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }

            instance->SetBossState(DATA_NYAMI, IN_PROGRESS);

            Talk(NYAMI_AGGRO);
            events.ScheduleEvent(EVENT_MIND_SPIKE, urand(8000, 15000));
            events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, urand(12000, 18000));
            events.ScheduleEvent(EVENT_SOUL_VESSEL, 20000);
            events.ScheduleEvent(EVENT_TORN_SPIRIT, 35000);
        }
        void KilledUnit(Unit* who) override
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                Talk(NYAMI_SLAY);
        }
        void JustDied(Unit* /*killer*/) override
        {
            Talk(NYAMI_DEATH);
            _JustDied();

            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);;
            instance->SetBossState(DATA_NYAMI, DONE);

            me->SummonCreature(CREATURE_WARDEN_AZZAKAEL, WardenSpawnPoint, TEMPSUMMON_MANUAL_DESPAWN);
            me->m_Events.AddEvent(new nyami_after_fight_pre_azzakel_event(me, 0), me->m_Events.CalculateTime(10000));
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
            {
                if (diffVisual <= diff)
                {
                    std::list<Creature*> trigger_corpses;
                    trigger_corpses.clear();

                    me->GetCreatureListWithEntryInGrid(trigger_corpses, TRIGGER_CORPSES_NYAMI_FIGHT, 40.0f);

                    if (trigger_corpses.empty())
                        return;

                    std::list<Creature*>::const_iterator it = trigger_corpses.begin();
                    std::advance(it, urand(0, trigger_corpses.size() - 1));
                  
                    me->CastSpell((*it), 153992);
                    me->GetMotionMaster()->MoveJump((*it)->GetPositionX(), (*it)->GetPositionY(), (*it)->GetPositionZ(), 15.0f, 5.0f);
                    me->RemoveAura(160677);

                    canchain = true;
                    diffChannel = 2000;
                    diffVisual = 15000;
                }
                else
                    diffVisual -= diff;

                if (diffChannel <= diff && canchain)
                {
                    me->CastSpell(me, 160677);
                    canchain = false;
                }
                else
                    diffChannel -= diff;
            }
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_MIND_SPIKE:
                    me->CastSpell(me->getVictim(), SPELL_MIND_SPIKE);

                    events.ScheduleEvent(EVENT_MIND_SPIKE, urand(8000, 15000));
                    break;
                case EVENT_SHADOW_WORD_PAIN:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50000.0F, true))
                        me->CastSpell(target, SPELL_SHADOW_WORD_PAIN);
             
                    events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, urand(12000, 18000));
                    break;
                case EVENT_SOUL_VESSEL:
                    instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SOUL_BUBBLE_BUFF);

                    me->MonsterTextEmote("Soulbinder Nyami begins to cast|cffff0000[Soul Vessel]|cfffaeb00!", me->GetGUID(), true);
                    me->SummonCreature(TRIGGER_SOUL_VESSEL_HACK_BUBBLE_EFFECT, Bubble_Position, TEMPSUMMON_MANUAL_DESPAWN);
                    me->CastSpell(me, SPELL_SOUL_VESSEL_DUMMY);

                    events.ScheduleEvent(EVENT_SOUL_VESSEL, 25000);
                    break;
                case EVENT_TORN_SPIRIT:
                {
                    std::list<Creature*> trigger_corpses;
                    me->GetCreatureListWithEntryInGrid(trigger_corpses, TRIGGER_CORPSES_NYAMI_FIGHT, 40.0f);

                    if (trigger_corpses.empty())
                        return;

                    std::list<Creature*>::const_iterator it = trigger_corpses.begin();
                    std::advance(it, urand(0, trigger_corpses.size() - 1));

                    //(*it)->GetAI()->DoAction(ACTION_SUMMON_SPIRITS);

                    me->SetReactState(REACT_PASSIVE);
                    me->CastSpell((*it), SPELL_DISPERSION_VISUAL_NYAMI);
                    me->SetSpeed(MOVE_RUN, 20.0f, true);
                    me->GetMotionMaster()->MovePoint(0, (*it)->GetPositionX(), (*it)->GetPositionY(), (*it)->GetPositionZ());

                    events.ScheduleEvent(EVENT_TORN_SPIRITS_DUMMY, 3000);
                    events.ScheduleEvent(EVENT_TORN_SPIRIT, 35000);
                    break;
                }
                case EVENT_TORN_SPIRITS_DUMMY:
                    me->SetSpeed(MOVE_RUN, 1.5f, true);
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->CastSpell(me, SPELL_TORN_SPRITS_DUMMY);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_nyamiAI(creature);
    }
};
class auchindon_nyami_spell_soul_vessel_dummy : public SpellScriptLoader
{
public:
    auchindon_nyami_spell_soul_vessel_dummy() : SpellScriptLoader("auchindon_nyami_spell_soul_vessel_dummy") { }

    class spell_auchindons : public SpellScript
    {
        PrepareSpellScript(spell_auchindons);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;         
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_auchindons::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }

    };

    SpellScript* GetSpellScript() const
    {
        return new spell_auchindons();
    }

    class auchindon_spells : public AuraScript
    {
        PrepareAuraScript(auchindon_spells);

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            if (GetCaster())
            {
                if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                    if (Creature* TriggerMiddle = instance->instance->GetCreature(instance->GetData64(DATA_TRIGGER_MIDDLE_NYAMI_FIGHT_BUBBLE)))
                    {
                        std::list<Player*> pl_list;

                        GetCaster()->GetPlayerListInGrid(pl_list, 200.0f);

                        if (pl_list.empty())
                            return;

                        for (auto itr : pl_list)
                        {
                            itr->CastSpell(itr, SPELL_SOUL_VESSEL_DMG);
                        }
                    }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(auchindon_spells::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };
    AuraScript* GetAuraScript() const
    {
        return new auchindon_spells();
    }
};
/*
The sniff didn't had the data for this spell, no areatriggers - hacking it with a regular npc. :c
*/
/*
class auchindon_nyami_bubble_good_aura : public AreaTriggerEntityScript
{
public:
    auchindon_nyami_bubble_good_aura() : AreaTriggerEntityScript("auchindon_nyami_bubble_good_aura")
    {
    }

    int diff = 500;
    std::list<uint64> m_Targets;
    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (diff <= p_Time)
        {
            std::list<Player*> PL_list;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, PL_list, check);
            p_AreaTrigger->VisitNearbyObject(2.0f, searcher);

            if (PL_list.empty())
                return;

            for (std::list<Player*>::const_iterator itr = PL_list.begin(); itr != PL_list.end(); ++itr)
            {
                if (!(*itr)->HasAura(SPELL_SOUL_BUBBLE_BUFF))
                {
                    (*itr)->CastSpell((*itr), SPELL_SOUL_BUBBLE_BUFF);
                    m_Targets.push_back((*itr)->GetGUID());

                    if ((*itr)->HasAura(SPELL_SOUL_BUBBLE_BUFF))
                    {
                        AuraPtr aura = (*itr)->GetAura(SPELL_SOUL_BUBBLE_BUFF);

                        if (aura)
                        {
                            aura->SetDuration(3);
                        }
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
        for (auto l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->HasAura(SPELL_SOUL_BUBBLE_BUFF))
                l_Target->RemoveAura(SPELL_SOUL_BUBBLE_BUFF);
        }
    }

    auchindon_nyami_bubble_good_aura* GetAI() const
    {
        return new auchindon_nyami_bubble_good_aura();
    }
};
*/
// TRIGGER_SOUL_VESSEL_HACK_BUBBLE_EFFECT 342652 NPC HACK!
class auchindon_nyami_bubble_good_aura : public CreatureScript
{
public:
    auchindon_nyami_bubble_good_aura() : CreatureScript("auchindon_nyami_bubble_good_aura") {}

    struct auchindon_nyami_bubble_good_auraAI : public Scripted_NoMovementAI
    {
        auchindon_nyami_bubble_good_auraAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetDisplayId(11686);
        }
        void Reset()
        {
            me->CastSpell(me, SPELL_SOUL_BUBBLE_VISUAL);
        }
        void UpdateAI(const uint32 diff)
        {
            std::list<Player*> targets;

            JadeCore::AnyPlayerInObjectRangeCheck check(me, 20.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, targets, check);
            me->VisitNearbyObject(12.0f, searcher);

            if (targets.empty())
                return;

            for (std::list<Player*>::const_iterator it = targets.begin(); it != targets.end(); ++it)
            {
                if (!(*it))
                    return;

                if ((*it)->IsWithinDistInMap(me, 4.0f))
                {
                    if (!(*it)->HasAura(SPELL_SOUL_BUBBLE_BUFF))
                    {
                        me->AddAura(SPELL_SOUL_BUBBLE_BUFF, (*it));

                        AuraPtr aura = me->GetAura(SPELL_SOUL_BUBBLE_BUFF);

                        if (aura)
                        {
                            aura->SetDuration(1);
                        }
                    }
                }
                else
                {
                    if ((*it)->HasAura(SPELL_SOUL_BUBBLE_BUFF))
                        (*it)->RemoveAura(SPELL_SOUL_BUBBLE_BUFF);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_nyami_bubble_good_auraAI(creature);
    }
};
class auchindon_nyami_radiant_star : public AreaTriggerEntityScript
{
public:
    auchindon_nyami_radiant_star() : AreaTriggerEntityScript("auchindon_nyami_radiant_star")
    {
    }

    int diff = 1000;
    std::list<uint64> m_Targets;
    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (diff <= p_Time)
        {
            std::list<Player*> PL_list;

            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 3.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, PL_list, check);
            p_AreaTrigger->VisitNearbyObject(3.0f, searcher);

            if (PL_list.empty())
                return;

            for (std::list<Player*>::const_iterator itr = PL_list.begin(); itr != PL_list.end(); ++itr)
            {
                (*itr)->CastSpell((*itr), SPELL_RADIANT_DAMAGE);
            }

            diff = 100000;
        }
        else
        {
            diff -= p_Time;
        }
    }
    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
    }

    auchindon_nyami_radiant_star* GetAI() const
    {
        return new auchindon_nyami_radiant_star();
    }
};
/*
class auchindon_nyami_trigger_corpses_trigger : public CreatureScript
{
public:
    auchindon_nyami_trigger_corpses_trigger() : CreatureScript("auchindon_nyami_trigger_corpses_trigger") { }

    struct trigger_corpses_triggerAI : public ScriptedAI
    {
        trigger_corpses_triggerAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_SUMMON_SPIRITS:
                events.ScheduleEvent(EVENT_TORN_SPIRIT_NYAMI_EFFECT, 5000);
                break;
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_TORN_SPIRIT_NYAMI_EFFECT:
                if (Creature* Nyami = instance->instance->GetCreature(instance->GetData64(DATA_BOSS_NYAMI)))
                {
                    Nyami->SetReactState(REACT_AGGRESSIVE);
                    Nyami->CastSpell(Nyami, SPELL_TORN_SPRITS_DUMMY);
                }
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new trigger_corpses_triggerAI(creature);
    };
};
*/
class auchindon_nyami_torn_spirits : public SpellScriptLoader
{
public:
    auchindon_nyami_torn_spirits() : SpellScriptLoader("auchindon_nyami_torn_spirits") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            Position pos;

            if (Creature* trigger = GetCaster()->FindNearestCreature(TRIGGER_CORPSES_NYAMI_FIGHT, 100.0f, true))
                trigger->GetRandomNearPosition(pos, 4.0f);

            GetCaster()->SummonCreature(CREATURE_MALEFIC_DEFENDER, pos, TEMPSUMMON_MANUAL_DESPAWN);
            GetCaster()->SummonCreature(CREATURE_TWISTED_MAGUS, pos, TEMPSUMMON_MANUAL_DESPAWN);
            GetCaster()->SummonCreature(CREATURE_SPITEFUL_ARBITRER, pos, TEMPSUMMON_MANUAL_DESPAWN);


        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindon_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};
class auchindon_nyami_spell_soul_vessel_damage : public SpellScriptLoader
{
public:
    auchindon_nyami_spell_soul_vessel_damage() : SpellScriptLoader("auchindon_nyami_spell_soul_vessel_damage") { }

    class auchindon_spells : public SpellScript
    {
        PrepareSpellScript(auchindon_spells);

        void RecalculateDamage(SpellEffIndex /*effIndex*/)
        {
            if (GetHitUnit()->HasAura(SPELL_SOUL_BUBBLE_BUFF))
                SetHitDamage(0);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(auchindon_spells::RecalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindon_spells();
    }
};
class auchindon_nyami_malefic_defender : public CreatureScript
{
public:
    auchindon_nyami_malefic_defender() : CreatureScript("auchindon_nyami_malefic_defender") { }

    struct auchindon_nyami_malefic_defenderAI : public ScriptedAI
    {
        auchindon_nyami_malefic_defenderAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_CRUSADER_STRIKE, 5000);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_CRUSADER_STRIKE:
                me->CastSpell(me->getVictim(), SPELL_CRUSADER_STRIKE);

                events.ScheduleEvent(EVENT_CRUSADER_STRIKE, urand(7000, 12000));
                break;
            }

            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_nyami_malefic_defenderAI(creature);
    }
};
class auchindon_nyami_spiteful_arbitrer : public CreatureScript
{
public:
    auchindon_nyami_spiteful_arbitrer() : CreatureScript("auchindon_nyami_spiteful_arbitrer") { }

    struct auchindon_nyami_spiteful_arbitrerAI : public ScriptedAI
    {
        auchindon_nyami_spiteful_arbitrerAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_RADIANT_FURY, 8000);
            events.ScheduleEvent(EVENT_ARBITRER_HAMMER, 14000);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_ARBITRER_HAMMER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(target, SPELL_ARBITRER_HAMMER);
                        events.ScheduleEvent(EVENT_ARBITRER_HAMMER, 14000);
                    }
                    break;
                case EVENT_RADIANT_FURY:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(target, SPELL_RADIANT_FURY_JUMP); // too fast?
                        Position pos;
                        me->GetPosition(&pos);

                        for (int i = 0; i < 3; i++)
                        {
                            float orientationleft = me->GetOrientation() * M_PI / 2;                       
                            Creature* LeftTrigger = me->SummonCreature(TRIGGER_RADIANT_FURY, pos.GetPositionX(), pos.GetPositionY(), orientationleft, TEMPSUMMON_MANUAL_DESPAWN);
                        
                        }
                        for (int i = 0; i < 3; i++)
                        {
                            float orientationright = me->GetOrientation() * M_PI / 4;
                            Creature* RightTrigger = me->SummonCreature(TRIGGER_RADIANT_FURY, pos.GetPositionX(), pos.GetPositionY(), orientationright, TEMPSUMMON_MANUAL_DESPAWN);
                        }
                        //events.ScheduleEvent(EVENT_RADIANT_FURY_SUMMON_TRIGGER, 100);
                        events.ScheduleEvent(EVENT_RADIANT_FURY_STOP, 1000);
                    }
                    break;
                case EVENT_RADIANT_FURY_SUMMON_TRIGGER: // ?
                {
                    Position pos;
                    me->GetPosition(&pos);

                    float orientationleft = me->GetOrientation() * M_PI / 2;
                    float orientationright = me->GetOrientation() * M_PI / 4;

                    Creature* LeftTrigger = me->SummonCreature(TRIGGER_RADIANT_FURY, pos.GetPositionX(), pos.GetPositionY(), orientationleft, TEMPSUMMON_MANUAL_DESPAWN);
                    Creature* RightTrigger = me->SummonCreature(TRIGGER_RADIANT_FURY, pos.GetPositionX(), pos.GetPositionY(), orientationright, TEMPSUMMON_MANUAL_DESPAWN);
                    events.ScheduleEvent(EVENT_RADIANT_FURY_SUMMON_TRIGGER, 100);
                    break;
                }              
                case EVENT_RADIANT_FURY_STOP:
                {
                    events.CancelEvent(EVENT_RADIANT_FURY_SUMMON_TRIGGER);
                    events.ScheduleEvent(EVENT_RADIANT_FURY, urand(15000, 20000));
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_nyami_spiteful_arbitrerAI(creature);
    }
};
class auchindon_nyami_radiant_fury_trigger : public CreatureScript
{
public:
    auchindon_nyami_radiant_fury_trigger() : CreatureScript("auchindon_nyami_radiant_fury_trigger") { }

    struct auchindon_nyami_radiant_fury_triggerAI : public Scripted_NoMovementAI
    {
        auchindon_nyami_radiant_fury_triggerAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

            Reset();
            me->Respawn(true);
        }

        void Reset()
        {
            me->CastSpell(me, SPELL_RADIANT_FULLY_VISUAL);
            me->DespawnOrUnsummon(1000);
        }

        InstanceScript* instance = me->GetInstanceScript();
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_nyami_radiant_fury_triggerAI(creature);
    }
};
class auchindon_nyami_twisted_magus : public CreatureScript
{
public:
    auchindon_nyami_twisted_magus() : CreatureScript("auchindon_nyami_twisted_magus") { }

    struct auchindon_nyami_twisted_magusAI : public ScriptedAI
    {
        auchindon_nyami_twisted_magusAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_ARCANE_BOLT, 4000);
            events.ScheduleEvent(EVENT_ARCANE_BOMB, 10000);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_ARCANE_BOLT:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    me->CastSpell(target, SPELL_ARCANE_BOLT);

                events.ScheduleEvent(EVENT_ARCANE_BOLT, 6000);
                break;
            case EVENT_ARCANE_BOMB:
                me->CastSpell(me, SPELL_ARCANE_BOMB_DUMMY);

                events.ScheduleEvent(EVENT_ARCANE_BOMB, 25000);
                break;
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_nyami_twisted_magusAI(creature);
    }
};
void AddSC_Nyami()
{
    new auchindon_boss_nyami();
    new auchindon_nyami_malefic_defender();
    new auchindon_nyami_spiteful_arbitrer();
    new auchindon_nyami_twisted_magus();
    new auchindon_mob_warden_cosmetic();
    new auchindon_nyami_radiant_fury_trigger();
    new auchindon_nyami_bubble_good_aura();
    //new auchindon_nyami_bubble_good_aura(); hacked.
    new auchindon_nyami_radiant_star();
    new auchindon_nyami_spell_soul_vessel_damage();
    new auchindon_nyami_torn_spirits();
    new auchindon_nyami_spell_soul_vessel_dummy();
}
