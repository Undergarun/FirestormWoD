#include "ScriptPCH.h"

enum Spells
{
    // Akma'hat
    SPELL_STONE_MANTLE              = 93561,
    SPELL_STONE_MANTLE_BUFF         = 94965,
    SPELL_SHOCKWAVE                 = 94968,
    SPELL_FURY_OF_THE_SANDS         = 94946,
    SPELL_FURY_OF_THE_SANDS_DUMMY   = 94941,
    SPELL_FURY_OF_THE_SANDS_SUMMON  = 93574,
    SPELL_SANDS_OF_TIME             = 93578
};

enum Events
{
    // Akma'hat
    EVENT_SHOCKWAVE         = 1,
    EVENT_FURY_OF_THE_SANDS = 2,
    EVENT_STONE_MANTLE      = 3,
    EVENT_SANDS_OF_TIME     = 4
};

enum CreatureIds
{
    NPC_AKMAHAT             = 50063,
    NPC_FURY_OF_THE_SANDS   = 51293
};

// 50063
class boss_akmahat : public CreatureScript
{
public:
    boss_akmahat() : CreatureScript("boss_akmahat") { }
    
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_akmahatAI(pCreature);
    }
    
    struct boss_akmahatAI : public ScriptedAI
    {
        boss_akmahatAI(Creature* pCreature) : ScriptedAI(pCreature), summons(me)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            me->setActive(true);
        }
        
        void Reset()
        {
            events.Reset();
            summons.DespawnAll();
        }
        
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_SHOCKWAVE, 16000);
            events.ScheduleEvent(EVENT_FURY_OF_THE_SANDS, 9000);
            events.ScheduleEvent(EVENT_SANDS_OF_TIME, urand(20000, 30000));
            events.ScheduleEvent(EVENT_STONE_MANTLE, 22000);
        }
        
        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }
        
        void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }
        
        void JustDied(Unit* /*killer*/)
        {
            events.Reset();
            summons.DespawnAll();
        }
        
        void UpdateAI(const uint32 diff)
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
                    case EVENT_STONE_MANTLE:
                        if (!me->HasAura(SPELL_STONE_MANTLE))
                        {
                            DoCast(me, SPELL_STONE_MANTLE);
                            events.ScheduleEvent(EVENT_STONE_MANTLE, urand(43000, 45000));
                        }
                        else
                            events.ScheduleEvent(EVENT_STONE_MANTLE, urand(10000, 15000));
                        break;
                    case EVENT_SHOCKWAVE:
                        DoCastVictim(SPELL_SHOCKWAVE);
                        events.ScheduleEvent(EVENT_SHOCKWAVE, 16000);
                        break;
                    case EVENT_FURY_OF_THE_SANDS:
                        for (uint8 i = 0; i < 9; ++i)
                            DoCast(me, SPELL_FURY_OF_THE_SANDS_SUMMON, true);
                        DoCast(me, SPELL_FURY_OF_THE_SANDS);
                        events.ScheduleEvent(EVENT_FURY_OF_THE_SANDS, 16000);
                        break;
                    case EVENT_SANDS_OF_TIME:
                        DoCastAOE(SPELL_SANDS_OF_TIME);
                        events.ScheduleEvent(EVENT_SANDS_OF_TIME, 25000);
                        break;
                    default:
                        break;
                }
            }
            
            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        SummonList summons;
    };
};

// 51293
class npc_akmahat_fury_of_the_sands : public CreatureScript
{
public:
    npc_akmahat_fury_of_the_sands() : CreatureScript("npc_akmahat_fury_of_the_sands") { }
    
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_akmahat_fury_of_the_sandsAI(pCreature);
    }
    
    struct npc_akmahat_fury_of_the_sandsAI : public Scripted_NoMovementAI
    {
        npc_akmahat_fury_of_the_sandsAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
        }
        
        void IsSummonedBy(Unit* /*owner*/)
        {
            DoCast(me, SPELL_FURY_OF_THE_SANDS_DUMMY, true);
        }
        
        void SpellHit(Unit* /*caster*/, const SpellInfo* spellInfo)
        {
            if (spellInfo->Id == SPELL_FURY_OF_THE_SANDS)
                me->DespawnOrUnsummon(1000);
        }
    };
};

void AddSC_uldum()
{
    new boss_akmahat();
    new npc_akmahat_fury_of_the_sands();
}