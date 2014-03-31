#include "ScriptPCH.h"
#include "halls_of_origination.h"

enum ScriptTexts
{
    SAY_DEATH   = 0,
    SAY_AGGRO   = 1,
    SAY_KILL    = 2,
};

enum Spells
{
    SPELL_FLAME_BOLT                = 77370,
    SPELL_FLAME_BOLT_DMG            = 75540, 
    SPELL_FLAME_BOLT_DMG_H          = 89881, 
    SPELL_RAGING_SMASH              = 83650,
    SPELL_EARTH_SPIKE               = 94974,
    SPELL_EARTH_SPIKE_DMG           = 75339,
    SPELL_EARTH_SPIKE_DMG_H         = 89882,
    SPELL_EARTH_SPIKE_KILL          = 89398,
    SPELL_QUICKSAND                 = 75546,
    SPELL_QUICKSAND_AOE             = 75547,
    SPELL_QUICKSAND_AOE_H           = 89880,
    SPELL_QUICKSAND_DIS             = 89396,

    SPELL_SAND_VORTEX_DUMMY1        = 79441,
    SPELL_SAND_VORTEX_DUMMY2        = 93570,
    SPELL_SAND_VORTEX               = 83097,
    SPELL_SAND_VORTEX_DMG           = 83096, 

    SPELL_SUBMERGE                  = 53421,

    SPELL_SUMMON_DUSTBONE_HORROR    = 75521,
    SPELL_SUMMON_JEWELED_SCARAB     = 75462,

    SPELL_SMASH                     = 75453,
};
 
enum Events
{
    EVENT_FLAME_BOLT    = 1,
    EVENT_RAGING_SMASH  = 2,
    EVENT_EARTH_POINT   = 3,
    EVENT_SUBMERGE      = 4,
    EVENT_SUMMON        = 5,      
    EVENT_STORM_MOVE    = 6,
    EVENT_VORTEX_DUST   = 7,
    EVENT_SMASH         = 8,
    EVENT_MERGE         = 9,
};
 
enum Adds
{
    NPC_QUICKSAND               = 40503, // 75546
    NPC_DUSTBONE_HORROR         = 40450,
    NPC_JEWELED_SCARAB          = 40458,
    NPC_TUMULTUOUS_EARTHSTORM   = 40406,
    NPC_BEETLE_STALKER          = 40459,
};
 
class boss_earthrager_ptah : public CreatureScript
{
    public:
        boss_earthrager_ptah() : CreatureScript("boss_earthrager_ptah") {}
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_earthrager_ptahAI(creature);
        }

        struct boss_earthrager_ptahAI : public BossAI
        {
            boss_earthrager_ptahAI(Creature* pCreature) : BossAI(pCreature, DATA_EARTHRAGER_PTAH)
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
            }

            uint8 count;
            uint8 phase;

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(HOScriptName))
                    me->IsAIEnabled = false;
                else if (!me->isDead())
                    Reset();
            }

            void Reset()
            {
                _Reset();

                me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                
                phase = 0;
            }

            void EnterCombat(Unit * who)
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_FLAME_BOLT, urand(5000, 8000));
                events.ScheduleEvent(EVENT_RAGING_SMASH, urand(7000, 10000));
                events.ScheduleEvent(EVENT_EARTH_POINT, urand(12000, 15000));
                DoZoneInCombat();
                instance->SetBossState(DATA_EARTHRAGER_PTAH, IN_PROGRESS);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }
            
            void KilledUnit(Unit* who)
            {
                Talk(SAY_KILL);
            }
      
            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                                       
                events.Update(diff);
 
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->HealthBelowPct(50) && phase == 0)
                {
                    phase = 1;
                    me->RemoveAllAuras();
                    me->AttackStop();
                    me->SetReactState(REACT_PASSIVE);
                    DoCast(me, SPELL_SUBMERGE);
                    me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                    events.Reset();
                    events.ScheduleEvent(EVENT_SUBMERGE, 2000);
                    return;
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_SUBMERGE:
                            for (uint8 i = 0; i < 2; ++i)
                                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                    DoCast(pTarget, SPELL_SUMMON_DUSTBONE_HORROR, true);
                            
                            for (uint8 i = 0; i < 6; ++i)
                                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                    DoCast(pTarget, SPELL_SUMMON_JEWELED_SCARAB, true);
                            
                            DoCast(me, SPELL_QUICKSAND, true);
                            DoCast(me, SPELL_SAND_VORTEX, true);
                            events.ScheduleEvent(EVENT_MERGE, 30000);
                            break;
                        case EVENT_MERGE:
                            me->RemoveAllAuras();
                            me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->MoveChase(me->getVictim());
                            events.ScheduleEvent(EVENT_FLAME_BOLT, urand(5000, 8000));
                            events.ScheduleEvent(EVENT_RAGING_SMASH, urand(7000, 10000));
                            events.ScheduleEvent(EVENT_EARTH_POINT, urand(12000, 15000));
                            break;
                        case EVENT_FLAME_BOLT:
                            DoCast(me, SPELL_FLAME_BOLT);
                            events.ScheduleEvent(EVENT_FLAME_BOLT, urand(16000, 20000));
                            break;
                        case EVENT_RAGING_SMASH:
                            DoCastVictim(SPELL_RAGING_SMASH);
                            events.ScheduleEvent(EVENT_RAGING_SMASH, urand(10000, 15000));
                            break;
                        case EVENT_EARTH_POINT:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(pTarget, SPELL_EARTH_SPIKE);
                            events.ScheduleEvent(EVENT_EARTH_POINT, urand(20000, 25000));
                            break;                            
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_ptah_dustbone_horror: public CreatureScript
{
    public:
        npc_ptah_dustbone_horror() : CreatureScript("npc_ptah_dustbone_horror") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_ptah_dustbone_horrorAI(pCreature);
        }

        struct npc_ptah_dustbone_horrorAI : public ScriptedAI
        {
            npc_ptah_dustbone_horrorAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void EnterCombat(Unit* who)
            {
                events.ScheduleEvent(EVENT_SMASH, urand(2000, 8000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

			    while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SMASH:
                            DoCast(me->getVictim(), SPELL_SMASH);
                            events.ScheduleEvent(EVENT_SMASH, urand(5000, 10000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

void AddSC_boss_earthrager_ptah()
{
    new boss_earthrager_ptah();
    new npc_ptah_dustbone_horror();
}
