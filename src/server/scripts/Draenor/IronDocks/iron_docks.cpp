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
#include "Vehicle.h"

enum Spells
{
    // battle master
    SPELL_BLADESTORM = 167232,
    SPELL_CHAIN_DRAG = 172860,
    SPELL_CHARGING_SLASH = 172889,
    SPELL_CHARGING_SLASH_JUMP = 172885,
    //footsoldier
    SPELL_TACTICAL_KICK = 169413,
    SPELL_DEMORALIZING_SHOUT = 169341,
    //deadeye
    SPELL_IRON_SHOT = 167239,
    SPELL_LEG_SHOT = 167240,
    //incinerator
    SPELL_INCENDIARY_SLUG = 172771,
    SPELL_INCEDINARY_SLUGS_VISUAL_MOB = 172802,
    SPELL_INCIDENARY_SLUGS_DUMMY = 176281,
    SPELL_INCIDENARY_SLUGS = 176902, // actual full spell, blizzards are just a bunch of r tards.
    SPELL_SHARPNEL_BLAST = 167516,
    // Deckhand
    SPELL_HATCHET_TOSS = 173112,
    // Champion Darona
    SPELL_BARBED_ARROW_BARRAGE_DUMMY = 166923,
    SPELL_BARBED_ARROW_AREA_TRIGGER = 164278,
    SPELL_BARBED_ARROW_AURA = 164370,
    SPELL_BARBED_ARROW_UNKNOWN_DAMAGE = 164648,
    SPELL_BARBED_ARROW_VISUAL = 166914,
    SPELL_BURNING_ARROW_DUMMY = 172810,
    SPELL_BURNING_ARROW_AREA_TRIGGER = 164234,
    SPELL_BURNING_ARROW_DAMAGE = 164632,
    SPELL_BURNING_ARROW_DOT = 173149,
    SPELL_CHAMPIONS_PRESENCE = 173091,
    // Siege Master Olugar
    SPELL_GATECRASHER_MISSILE_DUMMY = 172953,
    SPELL_GATECRASHER_MISSILE = 172952,
    SPELL_GATECRASHER_MISSILE_PLUS_TRIGGER = 172961, // need weapon display
    SPELL_GATECRASHER_DAMAGE = 172963,
    SPELL_THROW_GATECRASHER = 172952,
    SPELL_PIT_FIGHTER = 173455, // Upon disarm
    SPELL_SHATTERING_STRIKE = 172982, 
    // Pitwarden Gwarnok 
    SPELL_BRTUAL_INSPIRATION = 172943,
    // Gromkar Technician
    SPELL_FLING_HAMMER = 172703,
    SPELL_HIGH_EXPLOSIVE_GRENADE = 178298,
    SPELL_GREASE_VIAL = 172649,
    SPELL_GREASE_VIAL_EFFECT = 172636,
    // Unruly Orgon
    SPELL_FLURRY = 178412,
    SPELL_FLURRY_DAMAGE = 178414,
    SPELL_THUNDERING_STOMP = 173135,
    // Rushing 
    SPELL_RUSHING_STAMPEDE_VISUAL_MOVEMENT = 173350,
    SPELL_RUSHING_STAMPEDE_VISUAL_HORNING = 173351,
    SPELL_RUSHING_STAMPEDE_VISUAL_JUMP = 173352,
    SPELL_RUSHING_STAMPEDE_DUMMY_CAST = 173384,
    SPELL_RUSHING_STAMPEDE_DAMAGE = 173351,
    SPELL_SPINY_HORNS = 158349,
    // Thunderlord wrangler
    SPELL_CULTTRAPS = 173336,
    SPELL_CULTTRAPS_DAMAGE = 173324,
    SPELL_DISENGAGE = 173311,
    SPELL_RENDING_CLEAVE = 167815,
    SPELL_SPEAR_THROW = 167095,
    // Ironwing flamespitter
    SPELL_LAVA_BARRAGE_DUMMY = 173480,
    SPELL_LAVA_BARRAGE_AREA_TRIGGER = 173482,
    SPELL_LAVA_BARRAGE_DOT = 173489,
    SPELL_LAVA_BLAST = 173514,
    // Iron Star - Object
    SPELL_QUIET_SUICIDE = 163832,
    SPELL_LAUNCH_FORWARD = 167299,
    // Misc
    SPELL_SELF_STUN = 94563,
    SPELL_EMOTE_WORK = 43831,
    SPELL_EMOTE_CHEER = 84070,
    SPELL_EMOTE_FIGHT = 36, // actual standstate
    SPELL_CARRY_CRATE = 173166,
    SPELL_CARRY_SACK = 167539,
};
enum Events
{
    // Thunderlord wrangler
    EVENT_CULTTRAPS = 1000,
    EVENT_SPEAR_THROW = 1001,
    EVENT_RENDING_CLEAVE = 1002,
    // Rushing
    EVENT_RUSHING_STAMPEDE = 1003,
    EVENT_RUSHING_STAMPEDE_2 = 1004,
    // gromkar battlemaster
    EVENT_BLADESTORM = 900,
    EVENT_CHAIN_DRAG = 901,
    EVENT_CHARGING_SLASH = 902,
    // footsoldier
    EVENT_TACTICAL_KICK = 903,
    EVENT_DEMORALIZING_SHOUT = 904,
    // deadeye
    EVENT_IRON_SHOT = 905,
    EVENT_LEG_SHOT = 906,
    // incinerator
    EVENT_INCEDINARY_SLUG = 907,
    EVENT_SHARPNEL_BLAST = 908,
    // Champion Daruna
    EVENT_BURNING_ARROW = 909,
    EVENT_BARBED_ARROW = 910,
    EVENT_CHAMPION_PRESENCE = 911,
    // Siege Master Olugar
    EVENT_GATECRASHER = 912,
    EVENT_PIT_FIGHTER = 913,
    EVENT_SHATTERING_STRIKE = 914,
    EVENT_BRUTAL_INSPIRATION = 915,
    // Gromkar technician
    EVENT_GREASE_VIAL = 916,
    EVENT_FLING_HAMMER = 917,
    EVENT_HIGH_EXPLOSIVE_GRENADE = 918,
    // Ogron
    EVENT_FLURRY = 919,
    EVENT_THUNDERING_STOMP = 920,
    // Deck-hand
    EVENT_HATCHET_TOSS = 921,
    // ironwing flamespitter
    EVENT_LAVA_BARRAGE = 922,
    EVENT_LAVA_BLAST = 923,
};
enum actions
{
    ACTION_QUIET_DEATH = 744,
};
Position stunnedadds[3] = 
{
    {6848.13f, -557.67f, 5.134f, 0.167117f},
    {6856.76f, -570.57f, 4.741f, 1.034980f},
    {6868.90f, -571.93f, 5.115f, 2.466761f},
};
Position cheeringsoldiers[6] = 
{
    // right
    {6807.25f, -676.79f, 4.835f, 5.925450f},
    {6806.89f, -683.76f, 4.997f, 5.921500f}, 
    {6806.85f, -690.98f, 4.922f, 6.282260f},
    // left
    {6841.64f, -683.00f, 4.835f, 3.288370f},
    {6841.33f, -690.54f, 4.835f, 3.146160f},
    {6846.15f, -696.68f, 4.914f, 2.999297f},
};
Position practicingwarriors[2] =
{
    { 6826.10f, -688.61f, 4.835f, 2.412430f },
    { 6823.62f, -686.06f, 4.835f, 5.513440f },
};
Position cheeringsoldiers2[10] =
{
    { 6620.58f, -676.56f, 4.599f, 3.959485f },
    { 6623.64f, -680.51f, 4.601f, 3.774916f },
    { 6624.83f, -687.18f, 4.618f, 3.158379f },
    { 6623.84f, -694.58f, 4.607f, 2.514352f },
    { 6620.52f, -697.97f, 4.615f, 2.202549f },
    { 6612.53f, -698.07f, 4.660f, 1.637062f },
    { 6604.38f, -697.10f, 4.623f, 0.887006f },
    { 6601.72f, -693.82f, 4.629f, 0.741707f },
    { 6600.86f, -687.21f, 4.658f, 6.278765f },
    { 6603.08f, -678.38f, 4.626f, 5.607255f },
};
Position practicingwarriors2[2] =
{
    { 6614.71f, -688.92f, 4.763f, 2.389464f },
    { 6611.09f, -685.70f, 4.773f, 5.494924f },
};

// These are timers per each event, they represent the time between spells
#define bladestorminterval 15000
#define chaindraginterval urand(20000, 30000)
#define chargingslashinterval urand(5000, 40000)
#define ironshotinterval urand(5000, 8000)
#define legshotinterval urand(20000, 25000)
#define tacticalkickinterval urand(10000, 15000)
#define demoralizingshoutinterval 25000
#define sharpnelblastinterval urand(8000, 10000)
#define incidenaryslugsinterval urand(15000, 18000)
#define gatecrasherinterval urand (15000, 18000)
#define shatteringstrikeinterval urand(8000, 12000)
#define burningarrowinterval urand(8000, 12000)
#define barbedarrowinterval urand(5000, 15000)
#define flinghammerinterval urand(6000, 12000)
#define greesevialinterval  urand(5000, 9000)
#define highexplosiveinterval 20000
#define thunderingstompinterval urand(5000, 20000)
#define flurryinterval urand(8000, 13000)
#define hatchettossinterval urand(8000, 15000)
#define culttrapsinterval 18000
#define spearthrowinterval urand(6000, 10000)
#define rendingcleaveinterval urand(8000, 12000)
#define rushingstampedeinterval 15000
#define lavaburstinterval urand(15000, 20000)
enum Talks
{
    TALK_ZOGGOSH_09 = 18, // Good idea sir! (44056)
    TALK_ZOGGOSH_10 = 19, // Oh... (44057)
    TALK_ZOGGOSH_12 = 21, // Yes! Captain! (44059)
    TALK_ZOGGOSH_13 = 22, // Koramar no... I will avenge you! (44060)
    TALK_ZOGGOSH_TWENTY_QUESTION_MARK = 99944, // Good thinking sir. (40637)

    TALK_KORAMAR_04 = 26, // Fire! Fire! Aim... then fire.. (46903)
    TALK_KORAMAR_05 = 27, // What is wrong with you?! keep firing you all! (46904)

   
    TALK_KORAMAR_08 = 30, // That is what I do Zoggosh.. I think good. (46907)

    TALK_KORAMAR_10 = 33, // I didn't mean.. the Cat... (46910)
    TALK_KORAMAR_11 = 32, // Put a muzzle on it (46909)
};

class iron_docks_mob_gromkar_battlemaster : public CreatureScript
{
public:
    iron_docks_mob_gromkar_battlemaster() : CreatureScript("iron_docks_mob_gromkar_battlemaster") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {

        }
        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_BLADESTORM, bladestorminterval);
            events.ScheduleEvent(EVENT_CHAIN_DRAG, chaindraginterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BLADESTORM:
                    me->CastSpell(me, SPELL_BLADESTORM);
                    events.ScheduleEvent(EVENT_BLADESTORM, bladestorminterval);
                    break;
                case EVENT_CHAIN_DRAG:
                    if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0F, true))
                        me->CastSpell(random, SPELL_CHAIN_DRAG);

                    events.ScheduleEvent(EVENT_CHAIN_DRAG, chaindraginterval);
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
class iron_docks_spell_charging_slash_effect : public SpellScriptLoader
{
public:
    iron_docks_spell_charging_slash_effect() : SpellScriptLoader("iron_docks_spell_charging_slash_effect") { }

    class spells_iron_docks : public SpellScript
    {
        PrepareSpellScript(spells_iron_docks);

        void OnHitTarget()
        {
            if (GetCaster() && GetHitUnit())
            {
                GetCaster()->CastSpell(GetHitUnit(), SPELL_CHARGING_SLASH);
            }
        }
        void Register()
        {
            OnHit += SpellHitFn(spells_iron_docks::OnHitTarget);
        }
    };
    SpellScript* GetSpellScript() const
    {
        return new spells_iron_docks();
    }
};
class iron_docks_mob_gromkar_deadeye : public CreatureScript
{
public:
    iron_docks_mob_gromkar_deadeye() : CreatureScript("iron_docks_mob_gromkar_deadeye") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        { }

        uint32 visual;
        void Reset()
        {
            visual = 5000;
        }
        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_IRON_SHOT, ironshotinterval);         
            events.ScheduleEvent(EVENT_LEG_SHOT, legshotinterval);
        }
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetEntry() == NPC_IRON_STAR && me->IsWithinDistInMap(who, 1.2f))
            {
                who->Kill(me);
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
            {
                if (visual <= diff)
                {
                    if (Creature* archery_target = me->FindNearestCreature(TRIGGER_ARCHERY_TARGET, 30.0f))
                    {
                        archery_target->setFaction(1);
                        me->CastSpell(archery_target, SPELL_IRON_SHOT);
                    }
                    visual = 6000;
                }
                else
                {
                    visual -= diff;
                }
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
                    /*
                case EVENT_IRON_SHOT:
                    me->CastSpell(me->getVictim(), SPELL_IRON_SHOT);
                    events.ScheduleEvent(EVENT_IRON_SHOT, ironshotinterval);
                    break;
                    */
                case EVENT_LEG_SHOT:
                    if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0F, true))
                        me->CastSpell(random, SPELL_LEG_SHOT);

                    events.ScheduleEvent(EVENT_LEG_SHOT, legshotinterval);
                    break;
                }
            }

            DoSpellAttackIfReady(SPELL_IRON_SHOT); // shooting spell?
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_mob_gromkar_footsoldier : public CreatureScript
{
public:
    iron_docks_mob_gromkar_footsoldier() : CreatureScript("iron_docks_mob_gromkar_footsoldier") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {

        }
        void EnterCombat(Unit* who)
        {
            // Scheduling an event, which will result in spell casting
            events.ScheduleEvent(EVENT_TACTICAL_KICK, tacticalkickinterval);       
            events.ScheduleEvent(EVENT_DEMORALIZING_SHOUT, demoralizingshoutinterval);

            me->RemoveAura(SPELL_SELF_STUN);
        }
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetEntry() == NPC_IRON_STAR && me->IsWithinDistInMap(who, 1.2f))
            {
                who->Kill(me);
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_DEMORALIZING_SHOUT:
                    me->CastSpell(me, SPELL_DEMORALIZING_SHOUT);
                    events.ScheduleEvent(EVENT_DEMORALIZING_SHOUT, demoralizingshoutinterval);
                    break;
                    // event definition
                case EVENT_TACTICAL_KICK:      
                    // cast spell SPELL_TACTICAL_KICK at victim aka tank or highest aggro
                    me->CastSpell(me->getVictim(), SPELL_TACTICAL_KICK);
                    events.ScheduleEvent(EVENT_TACTICAL_KICK, tacticalkickinterval);
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
class iron_docks_mob_gromkar_deck_hand : public CreatureScript
{
public:
    iron_docks_mob_gromkar_deck_hand() : CreatureScript("iron_docks_mob_gromkar_deck_hand") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
            switch (urand(0, 1))
            {
            case 0:
                me->CastSpell(me, SPELL_CARRY_SACK);
                break;
            case 1:
                me->CastSpell(me, SPELL_CARRY_CRATE);
                break;
            }
        }
        void EnterCombat(Unit* who)
        {
            me->RemoveAura(SPELL_EMOTE_WORK);
            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

            events.ScheduleEvent(EVENT_HATCHET_TOSS, hatchettossinterval);
        }
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetEntry() == NPC_IRON_STAR && me->IsWithinDistInMap(who, 1.2f))
            {
                who->Kill(me);
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_HATCHET_TOSS:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_HATCHET_TOSS);

                    events.ScheduleEvent(EVENT_HATCHET_TOSS, hatchettossinterval);
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
class iron_docks_mob_gromkar_incinerator : public CreatureScript
{
public:
    iron_docks_mob_gromkar_incinerator() : CreatureScript("iron_docks_mob_gromkar_incinerator") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 visual;
        void Reset()
        {
            visual = 6000;
        }
        void EnterCombat(Unit* who)
        {
            // Scheduling an event, which will result in spell casting
           // events.ScheduleEvent(EVENT_INCEDINARY_SLUG, incidenarysluginterval); // cuz fuck grammar
            events.ScheduleEvent(EVENT_SHARPNEL_BLAST, sharpnelblastinterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
            {
                if (visual <= diff)
                {
                    if (Creature* archery_target = me->FindNearestCreature(TRIGGER_ARCHERY_TARGET, 30.0f))
                    {
                        archery_target->setFaction(1);
                        me->CastSpell(archery_target, SPELL_IRON_SHOT);

                        visual = 6000;
                    }
                }
                else
                {
                    visual -= diff;
                }
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
                case EVENT_INCEDINARY_SLUG:
                    me->CastSpell(me->getVictim(), SPELL_INCIDENARY_SLUGS);
                    events.ScheduleEvent(EVENT_INCEDINARY_SLUG, incidenaryslugsinterval);
                    break;
                    /*
                      // event definition
                case EVENT_SHARPNEL_BLAST:
                    // cast spell SPELL_TACTICAL_KICK at victim aka tank or highest aggro
                    me->CastSpell(me->getVictim(), SPELL_TACTICAL_KICK);
                    events.ScheduleEvent(EVENT_SHARPNEL_BLAST, sharpnelblastinterval);
                    break;
                    */
                }
            }
            DoSpellAttackIfReady(SPELL_SHARPNEL_BLAST);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_mob_gromkar_technician : public CreatureScript
{
public:
    iron_docks_mob_gromkar_technician() : CreatureScript("iron_docks_mob_gromkar_technician") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 visual;

        void Reset()
        {
            visual = 6000;
        }
        void EnterCombat(Unit* who)
        {
            // Scheduling an event, which will result in spell casting
            // events.ScheduleEvent(EVENT_INCEDINARY_SLUG, incidenarysluginterval); // cuz fuck grammar
            me->RemoveAura(SPELL_EMOTE_WORK);
            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

            events.ScheduleEvent(EVENT_GREASE_VIAL, greesevialinterval);
            events.ScheduleEvent(EVENT_FLING_HAMMER, flinghammerinterval);
            events.ScheduleEvent(EVENT_HIGH_EXPLOSIVE_GRENADE, highexplosiveinterval);
        }
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetEntry() == NPC_IRON_STAR && me->IsWithinDistInMap(who, 1.2f))
            {
                who->Kill(me);
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
            {
                if (visual <= diff)
                {
                    me->RemoveAura(SPELL_EMOTE_WORK);
                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    switch (urand(0, 2))
                    {
                    case 0: // work
                        me->CastSpell(me, SPELL_EMOTE_WORK); //43831 
                        break;
                    case 1: // craft
                       // me->CastSpell(me, SP);
                        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 133);
                        break;
                    case 2: // idle                      
                        break;
                    }
                    visual = 6000;
                }
                else
                {
                    visual -= diff;
                }
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
                case EVENT_GREASE_VIAL:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_GREASE_VIAL);

                    events.ScheduleEvent(EVENT_GREASE_VIAL, greesevialinterval);
                    break;
                case EVENT_FLING_HAMMER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_FLING_HAMMER);

                    events.ScheduleEvent(EVENT_FLING_HAMMER, flinghammerinterval);
                    break;
                case EVENT_HIGH_EXPLOSIVE_GRENADE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_HIGH_EXPLOSIVE_GRENADE);

                    events.ScheduleEvent(EVENT_HIGH_EXPLOSIVE_GRENADE, highexplosiveinterval);
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
class iron_docks_mob_champion_darona : public CreatureScript
{
public:
    iron_docks_mob_champion_darona() : CreatureScript("iron_docks_mob_champion_darona") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            me->CastSpell(me, SPELL_CHAMPIONS_PRESENCE);
        }
        void EnterCombat(Unit* who)
        {
            //  172810 aura 

            // Scheduling an event, which will result in spell casting
            // events.ScheduleEvent(EVENT_INCEDINARY_SLUG, incidenarysluginterval); // cuz fuck grammar

            // I don't think Darona actually shot barber arrows, wowhead is stupid. Icy veins says it aswell.
            events.ScheduleEvent(EVENT_BURNING_ARROW, burningarrowinterval);
            events.ScheduleEvent(EVENT_BARBED_ARROW, barbedarrowinterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BURNING_ARROW:
                    {
                        me->CastSpell(me, SPELL_BURNING_ARROW_DUMMY);           
                        me->MonsterYell("Light them up!", LANG_UNIVERSAL, me->GetGUID());
                        events.ScheduleEvent(EVENT_BURNING_ARROW, burningarrowinterval);
                        break;
                    }
                    case EVENT_BARBED_ARROW:
                    {
                        me->CastSpell(me, SPELL_BARBED_ARROW_BARRAGE_DUMMY);
                        me->AddAura(SPELL_BARBED_ARROW_AURA, me);
                        events.ScheduleEvent(EVENT_BARBED_ARROW, barbedarrowinterval);
                        break;
                    }
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
class iron_docks_area_trigger_oil_effect : public AreaTriggerEntityScript
{
public:
    iron_docks_area_trigger_oil_effect() : AreaTriggerEntityScript("iron_docks_area_trigger_oil_effect")
    {
        diff = 500;
    }

    uint32 diff;
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
                if (!(*itr)->HasAura(SPELL_GREASE_VIAL_EFFECT))
                {
                    (*itr)->CastSpell((*itr), SPELL_GREASE_VIAL_EFFECT);
                    m_Targets.push_back((*itr)->GetGUID());
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
            if (l_Target && l_Target->HasAura(SPELL_GREASE_VIAL_EFFECT))
                l_Target->RemoveAura(SPELL_GREASE_VIAL_EFFECT);
        }
    }
iron_docks_area_trigger_oil_effect* GetAI() const
{
    return new iron_docks_area_trigger_oil_effect();
}
};
class iron_docks_area_lava_barrage_effect : public AreaTriggerEntityScript
{
public:
    iron_docks_area_lava_barrage_effect() : AreaTriggerEntityScript("iron_docks_area_lava_barrage_effect")
    {
        diff = 500;
    }

    uint32 diff;
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
                if (!(*itr)->HasAura(SPELL_LAVA_BARRAGE_DOT))
                {
                    (*itr)->CastSpell((*itr), SPELL_LAVA_BARRAGE_DOT);
                    m_Targets.push_back((*itr)->GetGUID());
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
            if (l_Target && l_Target->HasAura(SPELL_LAVA_BARRAGE_DOT))
                l_Target->RemoveAura(SPELL_LAVA_BARRAGE_DOT);
        }
    }
    iron_docks_area_lava_barrage_effect* GetAI() const
    {
        return new iron_docks_area_lava_barrage_effect();
    }
};
class iron_docks_area_trigger_burning_arrow : public AreaTriggerEntityScript
{
public:
    iron_docks_area_trigger_burning_arrow() : AreaTriggerEntityScript("iron_docks_area_trigger_burning_arrow")
    {
    }

    uint32 diff = 500;
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
                if (!(*itr)->HasAura(SPELL_BURNING_ARROW_DAMAGE))
                {
                    (*itr)->CastSpell((*itr), SPELL_BURNING_ARROW_DAMAGE, true);
                    m_Targets.push_back((*itr)->GetGUID());
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
            if (l_Target && l_Target->HasAura(SPELL_BURNING_ARROW_DAMAGE))
                l_Target->RemoveAura(SPELL_BURNING_ARROW_DAMAGE);
        }
    }

    iron_docks_area_trigger_burning_arrow* GetAI() const
    {
        return new iron_docks_area_trigger_burning_arrow();
    }
};
class iron_docks_area_trigger_barbed_arrow : public AreaTriggerEntityScript
{
public:
    iron_docks_area_trigger_barbed_arrow() : AreaTriggerEntityScript("iron_docks_area_trigger_barbed_arrow")
    {
    }
    uint32 diff = 500;
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
                if (!(*itr)->HasAura(SPELL_BARBED_ARROW_UNKNOWN_DAMAGE))
                {
                    (*itr)->CastSpell((*itr), SPELL_BARBED_ARROW_UNKNOWN_DAMAGE);
                    m_Targets.push_back((*itr)->GetGUID());
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
            if (l_Target && l_Target->HasAura(SPELL_BARBED_ARROW_UNKNOWN_DAMAGE))
                l_Target->RemoveAura(SPELL_BARBED_ARROW_UNKNOWN_DAMAGE);
        }
    }

    iron_docks_area_trigger_barbed_arrow* GetAI() const
    {
        return new iron_docks_area_trigger_barbed_arrow();
    }
};
class iron_docks_spell_burning_arrow_aura : public SpellScriptLoader
{
public:
    iron_docks_spell_burning_arrow_aura() : SpellScriptLoader("iron_docks_spell_burning_arrow_aura") { }

    class iron_docks_Spells : public AuraScript
    {
        PrepareAuraScript(iron_docks_Spells);

        bool load()
        {
           time = 0;
        }
        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            PreventDefaultAction();
            
            if (time < 3)
            {
                if (GetCaster())
                {
                    if (Unit* target = GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                    {
                        GetCaster()->CastSpell(target, SPELL_BURNING_ARROW_AREA_TRIGGER);

                        time++;
                    }
                }
            }
        }

    private:
        int time;

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_Spells::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };
    AuraScript* GetAuraScript() const
    {
        return new iron_docks_Spells();
    }
};
class iron_docks_spell_barbed_arrow_dummy : public SpellScriptLoader
{
public:
    iron_docks_spell_barbed_arrow_dummy() : SpellScriptLoader("iron_docks_spell_barbed_arrow_dummy") { }

    class iron_docks_spell_barbed_arrow_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(iron_docks_spell_barbed_arrow_dummy_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            GetCaster()->CastSpell(GetCaster(), SPELL_BARBED_ARROW_AURA);
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(iron_docks_spell_barbed_arrow_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new iron_docks_spell_barbed_arrow_dummy_SpellScript();
    }
};
class iron_docks_spell_barbed_arrow_aura : public SpellScriptLoader
{
public:
    iron_docks_spell_barbed_arrow_aura() : SpellScriptLoader("iron_docks_spell_barbed_arrow_aura") { }

    class iron_docks_Spells : public AuraScript
    {
        PrepareAuraScript(iron_docks_Spells);

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            PreventDefaultAction();

            if (GetCaster())
            {
                if (Unit* target = GetCaster()->GetAI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                {
                    GetCaster()->CastSpell(target, SPELL_BARBED_ARROW_AREA_TRIGGER);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_Spells::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };
    AuraScript* GetAuraScript() const
    {
        return new iron_docks_Spells();
    }
};
class iron_docks_siege_master_olugar : public CreatureScript
{
public:
    iron_docks_siege_master_olugar() : CreatureScript("iron_docks_siege_master_olugar") { }

    enum eTalk
    {
        RandomText
    };

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        uint32 rp;

        void Reset()
        {
            rp = 16000;
            me->SetReactState(REACT_DEFENSIVE);
        }
        void EnterCombat(Unit* who)
        {
            // I don't think Darona actually shot barber arrows, wowhead is stupid. Icy veins says it aswell.
            events.ScheduleEvent(EVENT_SHATTERING_STRIKE, shatteringstrikeinterval);
            events.ScheduleEvent(EVENT_GATECRASHER, gatecrasherinterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
            {
                if (rp <= diff)
                {
                    Talk(eTalk::RandomText);
                    rp = 16000;
                }
                else
                    rp -= diff;
            }

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            // pit fighter
            if (me->HasAuraType(SPELL_AURA_MOD_DISARM))
            {
                if (!me->HasAura(SPELL_PIT_FIGHTER))
                me->AddAura(SPELL_PIT_FIGHTER, me);
            }
            else
            {
                if (me->HasAura(SPELL_PIT_FIGHTER))
                {
                    me->RemoveAura(SPELL_PIT_FIGHTER);
                }
            }

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_SHATTERING_STRIKE:
                    {
                        me->CastSpell(me->getVictim(), SPELL_SHATTERING_STRIKE);
                        events.ScheduleEvent(EVENT_SHATTERING_STRIKE, shatteringstrikeinterval);
                        break;
                    }
                case EVENT_GATECRASHER:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0F, true))
                        {
                            me->CastSpell(target, SPELL_THROW_GATECRASHER);                        
                        }

                        events.ScheduleEvent(EVENT_GATECRASHER, gatecrasherinterval);
                        break;
                    }
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
// 86534
class iron_docks_gatecrasher_trigger : public CreatureScript
{
public:
    iron_docks_gatecrasher_trigger() : CreatureScript("iron_docks_gatecrasher_trigger") { }

    struct mob_iron_docksAI : public Scripted_NoMovementAI
    {
        mob_iron_docksAI(Creature* creature) : Scripted_NoMovementAI(creature) { }

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetDisplayId(11686);
            me->setFaction(16);
        }
        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);
            
            // handle hacked areatrigger dmg (it appears to be blizzlike since dbc handler summon that add, 
            //but why the fuck would you do that if already summon an areatrigger? are they dumb? just areatrigger update blizzard, l2p
            std::list<Player*> nearplayerslist;
            me->GetPlayerListInGrid(nearplayerslist, 4.0F);

            for (auto itr : nearplayerslist)
            {
                if (!itr->HasAura(SPELL_GATECRASHER_DAMAGE))
                {
                    me->CastSpell(itr, SPELL_GATECRASHER_DAMAGE);

                    if (AuraPtr aur_gatecrasher = itr->GetAura(SPELL_GATECRASHER_DAMAGE))
                        aur_gatecrasher->SetDuration(1);
                }
            }          
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_gwarnok : public CreatureScript
{
public:
    iron_docks_gwarnok() : CreatureScript("iron_docks_gwarnok") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            me->SetReactState(REACT_DEFENSIVE);
        }
        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_BLADESTORM, bladestorminterval);
            events.ScheduleEvent(EVENT_CHARGING_SLASH, chargingslashinterval);
            events.ScheduleEvent(EVENT_CHAIN_DRAG, chaindraginterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BLADESTORM:
                    me->CastSpell(me, SPELL_BLADESTORM);
                    events.ScheduleEvent(EVENT_BLADESTORM, bladestorminterval);
                    break;
                case EVENT_CHARGING_SLASH:
                    if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0F, true))
                        me->CastSpell(random, SPELL_CHARGING_SLASH_JUMP);

                    events.ScheduleEvent(EVENT_CHARGING_SLASH, chargingslashinterval);
                    break;
                case EVENT_CHAIN_DRAG:
                    if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0F, true))
                    {
                        me->CastSpell(random, SPELL_CHAIN_DRAG);
                        random->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 8.0f, 5.0f, 10.0f);
                    }
                    events.ScheduleEvent(EVENT_CHAIN_DRAG, chaindraginterval);
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
class iron_docks_mob_ogron : public CreatureScript
{
public:
    iron_docks_mob_ogron() : CreatureScript("iron_docks_mob_ogron") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
        }
        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_THUNDERING_STOMP, thunderingstompinterval);
            events.ScheduleEvent(EVENT_FLURRY, flurryinterval);

            switch (urand(0, 2))
            {
            case 0:
                me->MonsterYell("CRUSH THEM!!", LANG_UNIVERSAL, me->GetGUID());
                break;
            }
        }
        void MoveInLineOfSight(Unit* who) override
        {
            if (who->GetEntry() == NPC_IRON_STAR && me->IsWithinDistInMap(who, 1.2f))
            {
                who->Kill(me);
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_THUNDERING_STOMP:
                    me->CastSpell(me, SPELL_THUNDERING_STOMP);
                    events.ScheduleEvent(EVENT_THUNDERING_STOMP, thunderingstompinterval);
                    break;
                case EVENT_FLURRY:
                    me->CastSpell(me->getVictim(), SPELL_FLURRY);
                    events.ScheduleEvent(EVENT_FLURRY, flurryinterval);
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
class iron_docks_spell_flurry_periodic : public SpellScriptLoader
{
public:
    iron_docks_spell_flurry_periodic() : SpellScriptLoader("iron_docks_spell_flurry_periodic") { }

    class iron_docks_auras : public AuraScript
    {
        PrepareAuraScript(iron_docks_auras);


        void HandlePeriodic(constAuraEffectPtr aurEff)
        {
            PreventDefaultAction();
            if (GetCaster())
            {
                std::list<Player*> targets;
                GetCaster()->GetPlayerListInGrid(targets, 5.0f);

                for (auto itr : targets)
                {
                    if (GetCaster()->isInFront(itr, M_PI * 0.5f))
                        GetCaster()->CastSpell(itr, SPELL_FLURRY_DAMAGE);
                }          
            }
        }
        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_auras::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new iron_docks_auras();
    }
};
class iron_docks_stunned_soldiers_trigger : public CreatureScript
{
public:
    iron_docks_stunned_soldiers_trigger() : CreatureScript("iron_docks_stunned_soldiers_trigger") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature) 
        {
            std::list<Creature*> Oldstunnedcreatures;
            
            me->GetCreatureListWithEntryInGrid(Oldstunnedcreatures, NPC_GROMKAR_FOOT_SOLDIER_2, 10.0f);

            for (auto itr : Oldstunnedcreatures)
                itr->DespawnOrUnsummon();

            for (int i = 0; i <= 2; i++)
            {
                Creature* foot_soldier_stunned = me->SummonCreature(NPC_GROMKAR_FOOT_SOLDIER_2, stunnedadds[i], TEMPSUMMON_DEAD_DESPAWN);
                if (foot_soldier_stunned)
                foot_soldier_stunned->AddAura(SPELL_SELF_STUN, foot_soldier_stunned);
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_stunned_cheering_practicing_trigger : public CreatureScript
{
public:
    iron_docks_stunned_cheering_practicing_trigger() : CreatureScript("iron_docks_stunned_cheering_practicing_trigger") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {                
            // Fighting Soldiers
            for (int i = 0; i < 2; i++)
            {
                Creature* foot_soldiers = me->SummonCreature(NPC_GROMKAR_FOOT_SOLDIER, practicingwarriors[i], TEMPSUMMON_DEAD_DESPAWN);
               if (foot_soldiers)
                foot_soldiers->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, SPELL_EMOTE_FIGHT);
            }
            // Praciticng Soldiers
            for (int i = 0; i < 5; i++)
            {    
                Creature* cheeringsoldierscreature = me->SummonCreature(NPC_GROMKAR_FOOT_SOLDIER, cheeringsoldiers[i], TEMPSUMMON_DEAD_DESPAWN);          
                if (cheeringsoldierscreature)
                    cheeringsoldierscreature->CastSpell(cheeringsoldierscreature, 84062);
            }   
        }
        InstanceScript* pinscription = me->GetInstanceScript();
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_stunned_cheering_practicing_trigger_second_segement : public CreatureScript
{
public:
    iron_docks_stunned_cheering_practicing_trigger_second_segement() : CreatureScript("iron_docks_stunned_cheering_practicing_trigger_second_segement") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
            /*
            std::list<Creature*> list_p;
            JadeCore::AnyCreatureInObjectRangeCheck check(me, 12.0f);
            JadeCore::CreatureListSearcher<JadeCore::AnyCreatureInObjectRangeCheck> searcher(me, list_p, check);
            me->VisitNearbyObject(12.0f, searcher);

            for (std::list<Creature*>::const_iterator itr = list_p.begin(); itr != list_p.end(); itr++)
            {
                if ((*itr)->GetEntry() != NPC_GROMKAR_FOOT_SOLDIER)
                    return;

                if ((*itr)->IsWithinDistInMap(me, 12.0f, true))
                    (*itr)->DespawnOrUnsummon();
            }
            */
            // Fighting Soldiers
            for (int i = 0; i < 2; i++)
            {
                Creature* foot_soldiers = me->SummonCreature(NPC_GROMKAR_FOOT_SOLDIER, practicingwarriors2[i], TEMPSUMMON_DEAD_DESPAWN);
                if (foot_soldiers)
                foot_soldiers->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, SPELL_EMOTE_FIGHT);
            }
            // Praciticng Soldiers
            for (int i = 0; i < 10; i++)
            {
                Creature* cheeringsoldierscreature = NULL;
                cheeringsoldierscreature = me->SummonCreature(NPC_GROMKAR_FOOT_SOLDIER, cheeringsoldiers2[i], TEMPSUMMON_DEAD_DESPAWN);
                  
                if (roll_chance_i(30))
                if (cheeringsoldierscreature)
                    cheeringsoldierscreature->CastSpell(cheeringsoldierscreature, 84062);
            }
        }
        InstanceScript* pinscription = me->GetInstanceScript();
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_archery_target : public CreatureScript
{
public:
    iron_docks_archery_target() : CreatureScript("iron_docks_archery_target") { }

    struct mob_iron_docksAI : public Scripted_NoMovementAI
    {
        mob_iron_docksAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            me->SetReactState(REACT_PASSIVE);
            me->AddUnitState(UNIT_STATE_CANNOT_AUTOATTACK);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_mob_gromkar_technician_deckhand_leader : public CreatureScript
{
public:
    iron_docks_mob_gromkar_technician_deckhand_leader() : CreatureScript("iron_docks_mob_gromkar_technician_deckhand_leader") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
            // Fighting Soldiers
            for (int i = 0; i < 6; i++)
            {
                Position pos;

                me->GetRandomNearPosition(pos, 10.0f);

                Creature* deckhands = me->SummonCreature(NPC_GROMKAR_DECKHAND, pos, TEMPSUMMON_DEAD_DESPAWN);
                if (deckhands)
                {
                    deckhands->GetMotionMaster()->MoveFollow(me, urand(1, 3), urand(40, 120), MOTION_SLOT_ACTIVE);
                    m_DeckhandList.push_back(deckhands->GetGUID());
                }
            }
        }

        std::list<uint64> m_DeckhandList;

        void Reset()
        {
            me->SetSpeed(MOVE_RUN, 0.5f, true);

            for (uint64 l_Guid : m_DeckhandList)
            {
                if (Creature* l_DeckHand = Creature::GetCreature(*me, l_Guid))
                {
                    if (!l_DeckHand->isAlive())
                        continue;

                    l_DeckHand->GetMotionMaster()->MoveFollow(me, urand(1, 3), urand(40, 120), MOTION_SLOT_ACTIVE);
                }
            }
        }
        void EnterCombat(Unit* who)
        {
            // Scheduling an event, which will result in spell casting
            // events.ScheduleEvent(EVENT_INCEDINARY_SLUG, incidenarysluginterval); // cuz fuck grammar
            me->RemoveAura(SPELL_EMOTE_WORK);
            //  me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

            events.ScheduleEvent(EVENT_GREASE_VIAL, greesevialinterval);
            events.ScheduleEvent(EVENT_FLING_HAMMER, flinghammerinterval);
            events.ScheduleEvent(EVENT_HIGH_EXPLOSIVE_GRENADE, highexplosiveinterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
            {
                for (uint64 l_Guid : m_DeckhandList)
                {
                    if (Creature* l_DeckHand = Creature::GetCreature(*me, l_Guid))
                    {
                        if (l_DeckHand->isMoving())
                            continue;

                        l_DeckHand->GetMotionMaster()->MoveFollow(me, urand(1, 3), urand(40, 120), MOTION_SLOT_ACTIVE);
                    }
                }

                return;
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_GREASE_VIAL:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_GREASE_VIAL);

                    events.ScheduleEvent(EVENT_GREASE_VIAL, greesevialinterval);
                    break;
                case EVENT_FLING_HAMMER:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_FLING_HAMMER);

                    events.ScheduleEvent(EVENT_FLING_HAMMER, flinghammerinterval);
                    break;
                case EVENT_HIGH_EXPLOSIVE_GRENADE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                        me->CastSpell(target, SPELL_HIGH_EXPLOSIVE_GRENADE);

                    events.ScheduleEvent(EVENT_HIGH_EXPLOSIVE_GRENADE, highexplosiveinterval);
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
class iron_docks_mob_thundering_wandler : public CreatureScript
{
public:
    iron_docks_mob_thundering_wandler() : CreatureScript("iron_docks_mob_thundering_wandler") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
        }
        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_CULTTRAPS, culttrapsinterval);
            events.ScheduleEvent(EVENT_SPEAR_THROW, spearthrowinterval);
            events.ScheduleEvent(EVENT_RENDING_CLEAVE, rendingcleaveinterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CULTTRAPS:
                    me->CastSpell(me->getVictim(), SPELL_CULTTRAPS, true);
                    me->GetMotionMaster()->MoveKnockbackFrom(me->GetPositionX(), me->GetPositionY(), 10.0f, 8.0f);
                    events.ScheduleEvent(EVENT_CULTTRAPS, culttrapsinterval);
                    break;
                case EVENT_SPEAR_THROW:
                    me->CastSpell(me->getVictim(), SPELL_SPEAR_THROW);
                    events.ScheduleEvent(EVENT_SPEAR_THROW, spearthrowinterval);
                    break;
                case EVENT_RENDING_CLEAVE:
                    me->CastSpell(me->getVictim(), SPELL_RENDING_CLEAVE);
                    events.ScheduleEvent(EVENT_RENDING_CLEAVE, rendingcleaveinterval);
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
class iron_docks_mob_rampaging_clefthoof : public CreatureScript
{
public:
    iron_docks_mob_rampaging_clefthoof() : CreatureScript("iron_docks_mob_rampaging_clefthoof") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        Unit* stampedeTar = NULL;
        void Reset()
        {
            me->AddAura(SPELL_SPINY_HORNS, me);
        }
        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_RUSHING_STAMPEDE, greesevialinterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())            
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_RUSHING_STAMPEDE:
                    me->CastSpell(me, SPELL_RUSHING_STAMPEDE_VISUAL_MOVEMENT);

                    stampedeTar = NULL;

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    {
                        me->CastSpell(target, SPELL_RUSHING_STAMPEDE_DUMMY_CAST);
                        stampedeTar = target;
                    }
                    events.ScheduleEvent(EVENT_RUSHING_STAMPEDE, greesevialinterval);
                    events.ScheduleEvent(EVENT_RUSHING_STAMPEDE_2, 2500);
                    break;
                case EVENT_RUSHING_STAMPEDE_2:
                {
                    if (stampedeTar)
                    {
                        me->GetMotionMaster()->MoveCharge(stampedeTar->GetPositionX(), stampedeTar->GetPositionY(), stampedeTar->GetPositionZ(), 42.0f);
                        me->CastSpell(stampedeTar, SPELL_RUSHING_STAMPEDE_VISUAL_JUMP);
                        me->CastSpell(stampedeTar, SPELL_RUSHING_STAMPEDE_DAMAGE);
                        me->CastSpell(stampedeTar, SPELL_RUSHING_STAMPEDE_VISUAL_HORNING);

                        me->GetMotionMaster()->Clear();
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
class iron_docks_mob_drake : public CreatureScript
{
public:
    iron_docks_mob_drake() : CreatureScript("iron_docks_mob_drake") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void Reset()
        {
        }
        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_LAVA_BARRAGE, lavaburstinterval);
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_LAVA_BARRAGE:
                    for (int i = 0; i <= 3; i++)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(target, SPELL_LAVA_BARRAGE_AREA_TRIGGER, true);
                    }

                    events.ScheduleEvent(EVENT_LAVA_BARRAGE, lavaburstinterval);
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
// 167239 
class iron_docks_spell_shoot_damage_decrease_on_dummies : public SpellScriptLoader
{
public:
    iron_docks_spell_shoot_damage_decrease_on_dummies() : SpellScriptLoader("iron_docks_spell_shoot_damage_decrease_on_dummies") { }

    class iron_docks_spells : public SpellScript
    {
        PrepareSpellScript(iron_docks_spells);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            if (GetHitUnit()->GetEntry() == 79423)
            PreventHitDamage();
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new iron_docks_spells();
    }
};
class explosion_iron_star : public BasicEvent
{
public:
    explicit explosion_iron_star(Unit* unit, Unit* unit2,int value) : obj(unit), obj2(unit2), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
            switch (modifier)
            {
            case 0:
                if (obj)
                    obj->ToCreature()->DespawnOrUnsummon();
                obj->CastSpell(obj, SPELL_QUIET_SUICIDE);
                break;
            case 1:
                if (obj && obj2 && obj2->IsInWorld())
                    obj2->EnterVehicle(obj, 0, true);
               // obj->GetVehicleKit()->RemoveAllPassengers();
                break;
            }       
        return true;
    }
private:
    Creature* storm;
    Unit* obj;
    Unit* obj2;
    int modifier;
    int Event;
};
class iron_docks_spell_charge_forward : public SpellScriptLoader
{
public:
    iron_docks_spell_charge_forward() : SpellScriptLoader("iron_docks_spell_charge_forward") { }

    class iron_docks_spell_charge_forward_SpellScript : public SpellScript
    {
        PrepareSpellScript(iron_docks_spell_charge_forward_SpellScript);

        void HandleCharge()
        {
            if (!GetCaster())
                return;

            GetCaster()->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_FORCE_MOVEMENT);

            GetCaster()->GetAI()->DoAction(ACTION_QUIET_DEATH);

            //Unit* pl = GetCaster()->GetVehicleKit()->GetPassenger(0);

            GetCaster()->SetSpeed(MOVE_RUN, 12.0f, true);

            GetCaster()->m_Events.AddEvent(new explosion_iron_star(GetCaster(), NULL,0), GetCaster()->m_Events.CalculateTime(6000));
            //if (pl && pl->IsInWorld())
            //GetCaster()->m_Events.AddEvent(new explosion_iron_star(GetCaster(), pl, 1), GetCaster()->m_Events.CalculateTime(500));

            // START VISUAL EVENT
            if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                instance->SetData(DATA_SECOND_EVENT, uint32(true));

            GetCaster()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            GetCaster()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void Register()
        {
            AfterCast += SpellCastFn(iron_docks_spell_charge_forward_SpellScript::HandleCharge);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new iron_docks_spell_charge_forward_SpellScript();
    }
};
class iron_docks_mob_iron_star : public CreatureScript
{
public:
    iron_docks_mob_iron_star() : CreatureScript("iron_docks_mob_iron_star") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        int32 timerperexplosion;
        bool canKill;

        void Reset()
        {
            timerperexplosion = 0;
            me->setFaction(35);
            canKill = false;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_QUIET_DEATH:
                timerperexplosion = 12000;
                me->setFaction(35);
                me->SetSpeed(MOVE_RUN, 12.0f, true);
                me->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_FORCE_MOVEMENT);

                me->GetAI()->DoAction(ACTION_QUIET_DEATH);

                break;
            }
        }
        void UpdateAI(uint32 const diff)
        {
            /*
            std::list<Creature*> hostileCreatures;

            std::list<Creature*> list_p;
            JadeCore::AllCreaturesInRange check(me, 4.0f);
            JadeCore::CreatureListSearcher<JadeCore::AllCreaturesInRange> searcher(me, list_p, check);
            me->VisitNearbyObject(1.0f, searcher);

            if (hostileCreatures.empty())
                return;

            printf("works");

            for (std::list<Creature*>::const_iterator itr = list_p.begin(); itr != list_p.end(); itr++)
            {
                me->Kill((*itr));

                printf("work 222222222222s");
            }
            */
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_trigger_stand_third_event : public CreatureScript
{
public:
    iron_docks_trigger_stand_third_event() : CreatureScript("iron_docks_trigger_stand_third_event") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        int32 timerperexplosion;
        bool canEvent;

        void Reset()
        {
            timerperexplosion = 0;
            me->setFaction(35);
            canEvent = false;
        }
        void MoveInLineOfSight(Unit* who)
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 15.0f) && !canEvent)
            {
                canEvent = true;

                if (InstanceScript* instance = me->GetInstanceScript())
                    instance->SetData(DATA_THIRD_EVENT, uint32(true));
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};
class iron_docks_trigger_cannon : public CreatureScript
{
public:
    iron_docks_trigger_cannon() : CreatureScript("iron_docks_trigger_cannon") { }

    struct mob_iron_docksAI : public Scripted_NoMovementAI
    {
        mob_iron_docksAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
        }

        void Reset()
        {
            me->setFaction(16);
            me->SetReactState(REACT_PASSIVE);
            me->AddUnitState(UNIT_STATE_CANNOT_AUTOATTACK);
            me->SetHealth(6000000);
            me->SetMaxHealth(6000000);
            me->GetMap()->SetObjectVisibility(1000.0f);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_iron_docksAI(creature);
    }
};

void AddSC_iron_docks_cpp()
{
    new iron_docks_mob_gromkar_battlemaster();
    new iron_docks_mob_gromkar_deadeye();
    new iron_docks_mob_gromkar_footsoldier();
    new iron_docks_mob_gromkar_incinerator();
    new iron_docks_mob_gromkar_technician();
    new iron_docks_siege_master_olugar();
    new iron_docks_mob_champion_darona();  
    new iron_docks_gwarnok();
    new iron_docks_mob_iron_star();
    new iron_docks_mob_ogron();
    new iron_docks_mob_gromkar_deck_hand();
    new iron_docks_stunned_cheering_practicing_trigger_second_segement();
    new iron_docks_mob_gromkar_technician_deckhand_leader();
    new iron_docks_stunned_cheering_practicing_trigger();
    new iron_docks_stunned_soldiers_trigger();
    new iron_docks_trigger_stand_third_event();
    new iron_docks_trigger_cannon();
    new iron_docks_archery_target();
    new iron_docks_spell_flurry_periodic();
    new iron_docks_spell_charge_forward();
    new iron_docks_area_trigger_burning_arrow();
    new iron_docks_area_lava_barrage_effect();
    new iron_docks_area_trigger_barbed_arrow();
    new iron_docks_area_trigger_oil_effect();
    new iron_docks_gatecrasher_trigger();
    new iron_docks_spell_shoot_damage_decrease_on_dummies();
    new iron_docks_spell_barbed_arrow_aura();
    new iron_docks_spell_barbed_arrow_dummy();
    new iron_docks_spell_burning_arrow_aura();
    new iron_docks_spell_charging_slash_effect();
    new iron_docks_mob_thundering_wandler();
    new iron_docks_mob_rampaging_clefthoof();
    new iron_docks_mob_drake();
}
