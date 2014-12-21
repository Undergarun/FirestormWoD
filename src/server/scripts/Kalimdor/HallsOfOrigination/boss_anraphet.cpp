#include "ScriptPCH.h"
#include "halls_of_origination.h"

enum ScriptTexts
{
    SAY_DEATH                  = 0,
    SAY_INTRO                  = 1,
    SAY_KILL                   = 2,
    SAY_ALPHA                  = 3,
    SAY_OMEGA                  = 4,
    SAY_AGGRO                  = 5,
};

enum Spells
{
	SPELL_ALPHA_BEAMS           = 76184,
    SPELL_ALPHA_BEAMS_AOE       = 76904, 
    SPELL_ALPHA_BEAM            = 76912,
    SPELL_ALPHA_BEAM_DMG        = 76956, 
    SPELL_ALPHA_BEAM_DMG_H      = 91177,
    SPELL_CRUMBLING_RUIN        = 75609, 
    SPELL_CRUMBLING_RUIN_H      = 91206, 
    SPELL_DESTRUCTION_PROTOCOL  = 77437,
    SPELL_NEMESIS_STRIKE        = 75604,
    SPELL_OMEGA_STANCE          = 75622,
};

enum Events
{
    EVENT_ALPHA_BEAMS          = 1,
    EVENT_CRUMBLING_RUIN       = 2,
    EVENT_DESTRUCTION_PROTOCOL = 3,
    EVENT_NEMESIS_STRIKE       = 4,
    EVENT_INTRO                = 5,
};

enum Adds
{
    NPC_ALPHA_BEAM      = 41133,
    NPC_OMEGA_STANCE    = 41194, // 77137 77117 
};

class boss_anraphet : public CreatureScript
{
    public:
        boss_anraphet() : CreatureScript("boss_anraphet") {}
 
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_anraphetAI(pCreature);
        }

        struct boss_anraphetAI : public BossAI
        {
            boss_anraphetAI(Creature* pCreature) : BossAI(pCreature, DATA_EARTHRAGER_PTAH)
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
                //me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
            }

            uint8 spells;

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

                spells = 0;
            }

            void EnterCombat(Unit * who)
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_ALPHA_BEAMS, 7000, 10000);
                events.ScheduleEvent(EVENT_NEMESIS_STRIKE, urand(3000, 7000));
                events.ScheduleEvent(EVENT_CRUMBLING_RUIN, 20000);

                DoZoneInCombat();
                instance->SetBossState(DATA_ANRAPHET, IN_PROGRESS);
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

            void DoAction(const int32 action)
            {
                if (action == 1)
                {
                    Talk(SAY_INTRO);
                    me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                    me->SetHomePosition(-203.93f, 368.71f, 75.92f, me->GetOrientation());
                    //DoTeleportTo(-203.93f, 368.71f, 75.92f);
                    me->GetMotionMaster()->MovePoint(0, -203.93f, 368.71f, 75.92f);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                                       
                events.Update(diff);
 
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_ALPHA_BEAMS:
                            if (spells == 3)
                            {
                                Talk(SAY_OMEGA);
                                DoCast(me, SPELL_OMEGA_STANCE);
                                spells = 0;
                            }
                            else
                            {
                                if (spells == 0)
                                    Talk(SAY_ALPHA);
                                DoCast(me, SPELL_ALPHA_BEAMS);
                                spells++;
                            }
                            events.ScheduleEvent(EVENT_ALPHA_BEAMS, 15000);
                            break;
                        case EVENT_CRUMBLING_RUIN:
                            DoCastAOE(SPELL_CRUMBLING_RUIN);
                            events.ScheduleEvent(EVENT_CRUMBLING_RUIN, 40000);
                            break;
                        case EVENT_NEMESIS_STRIKE:
                            DoCastVictim(SPELL_NEMESIS_STRIKE);
                            events.ScheduleEvent(EVENT_NEMESIS_STRIKE, urand(15000, 20000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_alpha_beam : public CreatureScript
{
    public:
        npc_alpha_beam() : CreatureScript("npc_alpha_beam") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_alpha_beamAI(creature);
        }

        struct npc_alpha_beamAI : public Scripted_NoMovementAI
        {
            npc_alpha_beamAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
				pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			}

			void Reset()
			{
				DoCast(me, SPELL_ALPHA_BEAM);
			}
			
			void UpdateAI(const uint32 diff)
			{				
			}
        };
};

void AddSC_boss_anraphet()
{
    new boss_anraphet();
    new npc_alpha_beam();
}
