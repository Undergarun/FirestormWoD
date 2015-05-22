/*
* Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "iron_docks.h"

enum Spells
{
    SPELL_FEEDING_AURA = 162424,
    // boss
    SPELL_BREAKOUT = 178124,
    SPELL_FEEDING_FRENZY = 162415,
    SPELL_PRIMAL_ASSAULT = 161256,
    // misv
    SPELL_BLOODY_CORPSE_EXPLOSION = 73163,
    SPELL_STRENGTH_OF_THE_PACK = 171114,
    // rylak
    //SPELL_ACIDIC_AREA_TRIGGERS = 178155,
    SPELL_ACIDIC_CAST = 171529,
};
enum Events
{
     EVENT_BREAKOUT = 900,
     EVENT_FEEDING_FRENZY = 901,
     EVENT_PRIMAL_ASSAULT = 902,
     EVENT_RELEASE = 903,
     EVENT_FEEDING_FRENZY_CANCEL = 904,
};
enum Actions
{
    ACTION_RELEASE = 424,
};
enum Talks
{
    TALK_ZOGGOSH_11 = 20, // Aye! aye! sir! (44058)
    TALK_ZOGGOSH_08 = 17, // Ahm... are you sure you want to let that thing out of it's cage? I don't think we fed it since we've found it. (44055)
    TALK_ZOGGOSH_07 = 16, // Captain they're almost here.. what are we do now!? (44054)
    TALK_KORAMAR_09 = 31, // I'm not playing these games anymore.. bring in the saber on and we'll be done with it. (46908)
    TALK_KORAMAR_11 = 32, // Put a muzzle on it (46909)
    TALK_KORAMAR_10 = 33, // I didn't mean.. the Cat... (46910)
};
enum Creatures
{
    CREATURE_WOLF = 89012,
    CREATURE_RYLAK = 89011,
    CREATURE_GUARD = 83390,
    
    // dests
    TRIGGER_WOLF_DEST = 89022,
    TRIGGER_RYLAK_DEST = 89021,
    TRIGGER_ACID_POOL = 214746,
};

Position newhomeposition = {6945.15f, -1098.29f, 4.603f, 4.579627f};
Position npcspawning = {6924.00f, -1090.61f, 4.604f, 0.102262f};
Position npcmovement[3] = 
{
    {6972.43f, -1094.44f, 4.962f, 0.903368f},
    {6969.16f, -1089.79f, 4.60f, 0.881810f},
    {6964.44f, -1085.31f, 4.603f, 0.102262f},
}; 

#define breakoutinterval 20000
#define feedingfrenzyinterval urand(50000, 60000)
#define primalassaultinterval urand(35000, 40000)
class beforeoshir_event : public BasicEvent
{
public:
    explicit beforeoshir_event(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (InstanceScript* instance = obj->GetInstanceScript())
        {
            if (Creature* Zoggosh = instance->instance->GetCreature(instance->GetData64(DATA_ZUGGOSH)))
            {
                if (Creature* Koramar = instance->instance->GetCreature(instance->GetData64(DATA_KORAMAR)))
                {
                    if (Creature* Skulloc = instance->instance->GetCreature(instance->GetData64(DATA_SKULLOC)))
                    {
                        switch (modifier)
                        {
                        case 0:
                            Zoggosh->AI()->Talk(TALK_ZOGGOSH_07);
                            Koramar->m_Events.AddEvent(new beforeoshir_event(Koramar, 1), Koramar->m_Events.CalculateTime(8000));
                            break;
                        case 1:
                            Koramar->AI()->Talk(TALK_KORAMAR_09);
                            Koramar->m_Events.AddEvent(new beforeoshir_event(Koramar, 2), Koramar->m_Events.CalculateTime(7000));
                            break;
                        case 2:
                            Zoggosh->AI()->Talk(TALK_ZOGGOSH_08);
                            Koramar->m_Events.AddEvent(new beforeoshir_event(Koramar, 3), Koramar->m_Events.CalculateTime(7000));
                            break;
                        case 3:
                            Koramar->AI()->Talk(TALK_KORAMAR_11);
                            Koramar->m_Events.AddEvent(new beforeoshir_event(Koramar, 4), Koramar->m_Events.CalculateTime(7000));
                            break;
                        case 4:
                            Zoggosh->AI()->Talk(TALK_ZOGGOSH_11);
                            Koramar->m_Events.AddEvent(new beforeoshir_event(Koramar, 5), Koramar->m_Events.CalculateTime(7000));
                            break;
                        case 5:
                            Koramar->AI()->Talk(TALK_KORAMAR_10);
                            break;
                        }
                    }
                }
            }
        }
        return true;
    }
private:
    Creature* storm;
    Unit* obj;
    int modifier;
    int Event;
};
class cosmeticaloshir_event : public BasicEvent
{
public:
    explicit cosmeticaloshir_event(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (InstanceScript* instance = obj->GetInstanceScript())
        {
            if (Creature* Oshir = instance->instance->GetCreature(instance->GetData64(DATA_OSHIR)))
            {
                        switch (modifier)
                        {
                        case 0:
                            for (int i = 0; i < 3; i++)
                            {
                                Creature* guard = Oshir->SummonCreature(CREATURE_GUARD, npcspawning, TEMPSUMMON_MANUAL_DESPAWN);
                                guard->GetMotionMaster()->MovePoint(0, npcmovement[i]);
                                guard->setFaction(35);
                            }
                            Oshir->m_Events.AddEvent(new cosmeticaloshir_event(Oshir, 1), Oshir->m_Events.CalculateTime(6000));
                            break;
                        case 1:
                            if (Creature* guard = Oshir->FindNearestCreature(CREATURE_GUARD, 100.0f))
                            {

                                Oshir->GetMotionMaster()->MoveJump(guard->GetPositionX(), guard->GetPositionY(), guard->GetPositionZ(), 25.0f, 10.0f, 10.0f);
                                   
                                //Oshir->CastSpell(guard, SPELL_BREAKOUT);
                                guard->CastSpell(guard, SPELL_BLOODY_CORPSE_EXPLOSION);
                                Oshir->Kill(guard);
                                guard->DespawnOrUnsummon(8000);
                            }
                            Oshir->m_Events.AddEvent(new cosmeticaloshir_event(Oshir, 2), Oshir->m_Events.CalculateTime(3000));
                            break;
                        case 2:
                            if (Creature* guard = Oshir->FindNearestCreature(CREATURE_GUARD, 100.0f))
                            {

                                Oshir->GetMotionMaster()->MoveJump(guard->GetPositionX(), guard->GetPositionY(), guard->GetPositionZ(), 25.0f, 10.0f, 10.0f);
                                guard->GetMotionMaster()->MoveRandom(5.0f);
                                //Oshir->CastSpell(guard, SPELL_BREAKOUT);
                                guard->CastSpell(guard, SPELL_BLOODY_CORPSE_EXPLOSION);
                                Oshir->Kill(guard);
                                guard->DespawnOrUnsummon(8000);
                            }
                            Oshir->m_Events.AddEvent(new cosmeticaloshir_event(Oshir, 3), Oshir->m_Events.CalculateTime(3000));
                            break;
                        case 3:
                            if (Creature* guard = Oshir->FindNearestCreature(CREATURE_GUARD, 100.0f))
                            {
                                guard->GetMotionMaster()->MoveRandom(5.0f);

                                Oshir->GetMotionMaster()->MoveJump(guard->GetPositionX(), guard->GetPositionY(), guard->GetPositionZ(), 25.0f, 10.0f, 10.0f);
                               // Oshir->CastSpell(guard, SPELL_BREAKOUT);
                                guard->CastSpell(guard, SPELL_BLOODY_CORPSE_EXPLOSION);
                                Oshir->Kill(guard);
                                guard->DespawnOrUnsummon(8000);
                            }
                            Oshir->m_Events.AddEvent(new cosmeticaloshir_event(Oshir, 4), Oshir->m_Events.CalculateTime(3000));
                            break;
                        case 4:
                            Oshir->SetHomePosition(newhomeposition);
                            Oshir->GetMotionMaster()->MovePoint(0, newhomeposition);
                            //Oshir->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            Oshir->setFaction(16);
                            break;
                        }
                    }
        }
        return true;
    }
private:
    Creature* storm;
    Unit* obj;
    int modifier;
    int Event;
};
class iron_docks_trigger_stand_fourth_event : public CreatureScript
{
public:
    iron_docks_trigger_stand_fourth_event() : CreatureScript("iron_docks_trigger_stand_fourth_event") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
            canEvent = false;
        }

        int32 timerperexplosion;
        bool canEvent;

        void Reset()
        {
            timerperexplosion = 0;
            me->setFaction(35);
        }
        void MoveInLineOfSight(Unit* who)
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 40.0) && !canEvent)
            {
                canEvent = true;
                me->m_Events.AddEvent(new beforeoshir_event(me, 0), me->m_Events.CalculateTime(2000));
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class boss_oshir : public CreatureScript
{
public:
    boss_oshir() : CreatureScript("boss_oshir") { }

    struct boss_oshirAI : public BossAI
    {
        boss_oshirAI(Creature* creature) : BossAI(creature, DATA_OSHIR)
        {
            intro = false;
           // me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(35);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_15);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_UNK_6);
        }

        bool intro;
        std::list<uint64> wolf_dests;
        std::list<uint64> rylak_dests;
        int32 hppact;
        void Reset() override
        {
            _Reset();
            wolf_dests.clear();
            rylak_dests.clear();

            hppact = me->GetHealthPct() * 0.95;
            DespawnCreaturesInArea(CREATURE_WOLF, me);
            DespawnCreaturesInArea(CREATURE_RYLAK, me);

            std::list<Creature*> wolfs;
            std::list<Creature*> rylak;

            me->GetCreatureListWithEntryInGrid(wolfs, TRIGGER_WOLF_DEST, 300.0f);
            me->GetCreatureListWithEntryInGrid(rylak, TRIGGER_RYLAK_DEST, 300.0f);

            for (auto itr : wolfs)
            {
                itr->GetAI()->Reset();
                wolf_dests.push_back(itr->GetGUID());
            }

            for (auto itr : rylak)
            {
                itr->GetAI()->Reset();
                rylak_dests.push_back(itr->GetGUID());
            }

            me->SetSpeed(MOVE_FLIGHT, 10.0f, true);
        }
        void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 300.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_FEEDING_FRENZY, feedingfrenzyinterval);
            events.ScheduleEvent(EVENT_BREAKOUT, breakoutinterval);
            events.ScheduleEvent(EVENT_PRIMAL_ASSAULT, primalassaultinterval);
        }
        void DamageTaken(Unit* attacker, uint32 &damage, SpellInfo const* p_SpellInfo) override
        {
            if (hppact)
            {
                if (me->HealthBelowPctDamaged(hppact, damage))
                {
                    me->CastStop();
                    me->RemoveAllAuras();
                    me->RemoveAura(SPELL_FEEDING_FRENZY);
                    me->RemoveAura(162424);
                }
            }
        }
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();

            DespawnCreaturesInArea(CREATURE_WOLF, me);
            DespawnCreaturesInArea(CREATURE_RYLAK, me);
        }
        void MoveInLineOfSight(Unit* who)
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 50.0f) && !intro)
            {
                intro = true;
                me->m_Events.AddEvent(new cosmeticaloshir_event(me, 0), me->m_Events.CalculateTime(2000));
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(162424)) // time to feed
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FEEDING_FRENZY_CANCEL:
                    me->RemoveAura(SPELL_FEEDING_FRENZY);
                    break;
                case EVENT_FEEDING_FRENZY:
                    me->RemoveAura(SPELL_FEEDING_FRENZY);

                    hppact = me->GetHealthPct() * 0.95;

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_FEEDING_FRENZY);

                    events.ScheduleEvent(EVENT_FEEDING_FRENZY, feedingfrenzyinterval);
                    events.ScheduleEvent(EVENT_FEEDING_FRENZY_CANCEL, 20000);
                    break;
                case EVENT_PRIMAL_ASSAULT:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(target, SPELL_BREAKOUT);
                        me->CastSpell(target, SPELL_PRIMAL_ASSAULT);
                    }
                    events.ScheduleEvent(EVENT_PRIMAL_ASSAULT, primalassaultinterval);
                    break;
                case EVENT_BREAKOUT:
                {
                    me->RemoveAura(SPELL_FEEDING_AURA);
                    switch (urand(0, 1))
                    {
                        case 0: // wolfs
                        {
                            if (wolf_dests.empty())
                                return;

                            std::list<uint64>::iterator it = wolf_dests.begin();
                            std::advance(it, urand(0, wolf_dests.size() - 1));

                            Creature* l_Wolf = Unit::GetCreature(*me, *it);
                            if (l_Wolf)
                            {
                                me->GetMotionMaster()->MoveJump(l_Wolf->GetPositionX(), l_Wolf->GetPositionY(), l_Wolf->GetPositionZ(), 25.0f, 10.0f, 10.0f);
                                //me->CastSpell((*it), SPELL_BREAKOUT, true);

                                if (l_Wolf->GetAI())
                                {
                                    l_Wolf->GetAI()->DoAction(ACTION_RELEASE);
                                    wolf_dests.erase(it);
                                }
                            }
                            break;
                        }
                        case 1: // rylaks
                        {
                            if (wolf_dests.empty())
                                return;

                            std::list<uint64>::iterator it = rylak_dests.begin();
                            std::advance(it, urand(0, rylak_dests.size() - 1));

                            Creature* l_Wolf = Unit::GetCreature(*me, *it);
                            if (l_Wolf)
                            {
                                me->GetMotionMaster()->MoveJump(l_Wolf->GetPositionX(), l_Wolf->GetPositionY(), l_Wolf->GetPositionZ(), 25.0f, 10.0f, 10.0f);
                                //me->CastSpell((*it), SPELL_BREAKOUT, true);

                                if (l_Wolf->GetAI())
                                {
                                    l_Wolf->GetAI()->DoAction(ACTION_RELEASE);
                                    rylak_dests.erase(it);
                                }
                            }
                            break;
                        }          
                    }
                    events.ScheduleEvent(EVENT_BREAKOUT, breakoutinterval);
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
        return new boss_oshirAI(creature);
    }
};
class iron_docks_mob_wolf_dest : public CreatureScript
{
public:
    iron_docks_mob_wolf_dest() : CreatureScript("iron_docks_mob_wolf_dest") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }
        std::list<Creature*> wolfs;
        bool released;
        InstanceScript* instance;

        void Reset()
        {
            released = false;
            summons.DespawnAll();
            wolfs.clear();

            for (int i = 0; i <= 5; i++)
            {
                Creature* wolf = me->SummonCreature(CREATURE_WOLF, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                wolf->setFaction(35);
                wolf->SetReactState(REACT_PASSIVE);

                wolfs.push_back(wolf);
            }
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_RELEASE:
                if (Creature* oshir = instance->instance->GetCreature(instance->GetData64(DATA_OSHIR)))
                {
                    if (!wolfs.empty())
                    {
                        if (!released)
                        {
                            released = true;
                            events.ScheduleEvent(EVENT_RELEASE, 5000);
                        }
                    }
                }
                break;
            }
        }
        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_RELEASE:
                    if (Creature* oshir = instance->instance->GetCreature(instance->GetData64(DATA_OSHIR)))
                    {
                        if (oshir->isInCombat() && oshir->getVictim())
                        {
                            if (!wolfs.empty())
                            {
                                for (auto itr : wolfs)
                                {
                                    if (itr && itr->IsInWorld() && itr->isAlive())
                                    {
                                        me->GetMotionMaster()->Clear();
                                        // itr->Respawn(true);
                                        itr->DespawnOrUnsummon(2000);
                                        Creature* wolf = me->SummonCreature(itr->GetEntry(), itr->GetPositionX(), itr->GetPositionY(), itr->GetPositionZ(), itr->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                                        // itr->Respawn(true);
                                        // Reset();

                                        if (wolf)
                                        {
                                            wolf->SetSpeed(MOVE_RUN, 1.9, true);
                                          //  wolf->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                           // wolf->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG2_DISABLE_TURN);
                                          //  wolf->RemoveUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                            wolf->setFaction(16);
                                            wolf->SetReactState(REACT_AGGRESSIVE);

                                            if (Player* nearest = me->FindNearestPlayer(100.0f, true))
                                                wolf->GetMotionMaster()->MovePoint(0, nearest->GetPositionX(), nearest->GetPositionY(), nearest->GetPositionZ());
                                          // wolf->AddThreat(oshir->getVictim(), 200.0f);
                                           // wolf->Attack(oshir->getVictim(), true);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_mob_rylak_dest : public CreatureScript
{
public:
    iron_docks_mob_rylak_dest() : CreatureScript("iron_docks_mob_rylak_dest") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }
        std::list<Creature*> rylaks;
        bool released;
        InstanceScript* instance;

        void Reset()
        {
            released = false;
            summons.DespawnAll();
            rylaks.clear();

            Creature* rylak = me->SummonCreature(CREATURE_RYLAK, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
            rylak->setFaction(35);
            rylak->SetReactState(REACT_PASSIVE);

            rylaks.push_back(rylak);
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_RELEASE:
                if (Creature* oshir = instance->instance->GetCreature(instance->GetData64(DATA_OSHIR)))
                {
                    if (!rylaks.empty())
                    {
                        if (!released)
                        {
                            released = true;
                            events.ScheduleEvent(EVENT_RELEASE, 5000);
                        }
                    }
                }
                break;
                }
          }
        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_RELEASE:
                    if (Creature* oshir = instance->instance->GetCreature(instance->GetData64(DATA_OSHIR)))
                    {
                        if (oshir->isInCombat() && oshir->getVictim())
                        {
                            if (!rylaks.empty())
                            {
                                for (auto itr : rylaks)
                                {
                                    if (itr && itr->IsInWorld() && itr->isAlive())
                                    {
                                        me->GetMotionMaster()->Clear();
                                        itr->DespawnOrUnsummon(2000);
                                        Creature* rykul = me->SummonCreature(itr->GetEntry(), itr->GetPositionX(), itr->GetPositionY(), itr->GetPositionZ(), itr->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                                       // itr->Respawn(true);
                                       // Reset();

                                        if (rykul)
                                        {
                                            //rykul->SetSpeed(MOVE_RUN, 1.9, true);
                                            //rykul->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                           // rykul->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG2_DISABLE_TURN);
                                            //rykul->RemoveUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                            rykul->setFaction(16);
                                            rykul->SetReactState(REACT_AGGRESSIVE);

                                            if (Player* nearest = me->FindNearestPlayer(100.0f, true))
                                                rykul->GetMotionMaster()->MovePoint(0, nearest->GetPositionX(), nearest->GetPositionY(), nearest->GetPositionZ());
                                            //rykul->AddThreat(oshir->getVictim(), 200.0f);
                                            //rykul->Attack(oshir->getVictim(), true);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_mob_rylak : public CreatureScript
{
public:
    iron_docks_mob_rylak() : CreatureScript("iron_docks_mob_rylak") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }
        uint32 acidInterval;
        InstanceScript* instance;

        void Reset()
        {
            acidInterval = urand(11000, 16000);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (acidInterval <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->SummonCreature(TRIGGER_ACID_POOL, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 15000);
                    me->CastSpell(target, SPELL_ACIDIC_CAST);
                    acidInterval = urand(11000, 16000);
                }
            }
            else
                acidInterval -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_mob_wolf : public CreatureScript
{
public:
    iron_docks_mob_wolf() : CreatureScript("iron_docks_mob_wolf") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }
        uint32 interval;
        InstanceScript* instance;

        void Reset()
        {
            interval = 1500;
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            if (interval <= diff)
            {
                me->CastSpell(me, SPELL_STRENGTH_OF_THE_PACK);

                std::list<Creature*> list_creatures;

                me->GetCreatureListWithEntryInGrid(list_creatures, me->GetEntry(), 200.0f);

                if (list_creatures.empty())
                    return;

                for (auto itr : list_creatures)
                {
                    if (itr && itr->IsInWorld() && itr->isAlive() && itr->getFaction() == 16)
                    {
                        if (AuraPtr aura = me->GetAura(SPELL_STRENGTH_OF_THE_PACK))
                        {
                            aura->SetStackAmount(list_creatures.size());
                        }
                    }
                }

                interval = 1500;
            }
            else
                interval -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_oshir_trigger_acid_spit : public CreatureScript
{
public:
    iron_docks_oshir_trigger_acid_spit() : CreatureScript("iron_docks_oshir_trigger_acid_spit") { }

    struct mob_iron_docksAI : public Scripted_NoMovementAI
    {
        mob_iron_docksAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            instance = me->GetInstanceScript();
        }
        uint32 interval;
        InstanceScript* instance;

        void Reset()
        {
            interval = 1000;
            me->setFaction(16);
            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }
        void UpdateAI(uint32 const diff)
        {
            if (interval <= diff)
            {
                std::list<Player*> PL_list;
                PL_list.clear();

                JadeCore::AnyPlayerInObjectRangeCheck check(me, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, PL_list, check);
                me->VisitNearbyObject(2.0f, searcher);

                if (PL_list.empty())
                    return;

                for (std::list<Player*>::const_iterator itr = PL_list.begin(); itr != PL_list.end(); ++itr)
                {
                    if (!(*itr)->HasAura(171533))
                    {
                        (*itr)->CastSpell((*itr), 171533);

                        if ((*itr)->HasAura(171533))
                        {
                            AuraPtr aura = (*itr)->GetAura(171533);

                            if (aura)
                            {
                                aura->SetDuration(3);
                            }
                        }
                    }
                }
                interval = 1000;
            }
            else
                interval -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
void AddSC_boss_oshir()
{
    // boss
    new boss_oshir();
    // spells
    //new spell_acid_spew();
    new iron_docks_oshir_trigger_acid_spit();
    // adds
    new iron_docks_mob_rylak();
    new iron_docks_mob_wolf();
    // triggers
    new iron_docks_mob_rylak_dest();
    new iron_docks_mob_wolf_dest();
    new iron_docks_trigger_stand_fourth_event();
}
