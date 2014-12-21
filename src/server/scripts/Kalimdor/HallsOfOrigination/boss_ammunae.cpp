#include "ScriptPCH.h"
#include "halls_of_origination.h"

enum ScriptTexts
{
    SAY_DEATH                = 0,
    SAY_AGGRO                = 1,
    SAY_GROWTH               = 2,
    SAY_KILL                 = 3,
};

enum Spells
{
    //Ammunae
    SPELL_WITHER                            = 76043,
    SPELL_CONSUME_LIFE_MANA                 = 75718,
    SPELL_CONSUME_LIFE_FOCUS                = 80968,
    SPELL_CONSUME_LIFE_FOCUS_H              = 94958,
    SPELL_CONSUME_LIFE_ENERGY               = 79766,
    SPELL_CONSUME_LIFE_ENERGY_H             = 94961,
    SPELL_CONSUME_LIFE_RUNIC                = 79768,
    SPELL_CONSUME_LIFE_RUNIC_H              = 94959,
    SPELL_CONSUME_LIFE_RAGE                 = 79767,
    SPELL_CONSUME_LIFE_RAGE_H               = 94960,
    SPELL_CONSUME_LIFE_SELF                 = 75665,
    SPELL_ZERO_POWER						= 72242,
    SPELL_RAMPANT_GROWTH                    = 75790,
    SPELL_RAMPANT_GROWTH_H                  = 89888,

    // Seedling Pod
    SPELL_ENERGIZE                          = 75657,
    SPELL_ENERGIZING_GROWTH                 = 89123,   
    SPELL_SEEDLING_POD                      = 96278,

    // Bloodpetal
    SPELL_THORN_SLASH                       = 76044,

    // Spore
    SPELL_SPORE_CLOUD                       = 75701,
    SPELL_NOXIOUS_SPORE                     = 75702,
    SPELL_NOXIOUS_SPORE_H                   = 89889,
};

enum Events
{
    EVENT_WITHER                = 1,
    EVENT_CONSUME_LIFE          = 2,
    EVENT_RAMPANT_GROWTH        = 3,
    EVENT_SUMMON_POD            = 4,
    EVENT_SUMMON_SPORE          = 5,
    EVENT_ENERGIZE              = 6,
};

enum Adds
{
    NPC_SEEDLING_POD            = 40550, // 75624
    NPC_BLOODPETAL_BLOSSOM      = 40622, // 75770
    NPC_BLOODPETAL_BLOSSOM_2    = 40620,
    NPC_SPORE                   = 40585,
    NPC_SEEDLING_POD_2          = 40716, // 96278
    NPC_BUDDING_SPORE           = 40669, // 75867
    NPC_BLOODPETAL_SPROUT       = 40630, // 76486
    NPC_SEEDLING_POD_3          = 40592, // 75687
};

class boss_ammunae : public CreatureScript
{
    public:
        boss_ammunae() : CreatureScript("boss_ammunae") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_ammunaeAI(pCreature);
        }

        struct boss_ammunaeAI : public BossAI
        {
            boss_ammunaeAI(Creature* pCreature) : BossAI(pCreature, DATA_AMMUNAE)
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

                DoCast(me, SPELL_ZERO_POWER);
                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 0);
            }

            void KilledUnit(Unit* /*Killed*/)
            {
                Talk(SAY_KILL);
            }

            void JustDied(Unit* /*Kill*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void EnterCombat(Unit* /*Ent*/)
            {
                Talk(SAY_AGGRO);

                events.ScheduleEvent(EVENT_WITHER, urand(3000, 6000));
                events.ScheduleEvent(EVENT_CONSUME_LIFE, urand(8000, 12000));
                events.ScheduleEvent(EVENT_SUMMON_POD, urand(7000, 14000));
                events.ScheduleEvent(EVENT_SUMMON_SPORE, urand(14000, 16000));

                DoZoneInCombat();
                instance->SetBossState(DATA_AMMUNAE, IN_PROGRESS);
            }

            void JustReachedHome()
		    {
			    _JustReachedHome();
		    }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
				    return;

                 if (me->GetPower(POWER_ENERGY) > 99)
                 {
                     Talk(SAY_GROWTH);
                     DoCastAOE(SPELL_RAMPANT_GROWTH);
                     return;
                 }

			    while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WITHER:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(pTarget, SPELL_WITHER);
                            events.ScheduleEvent(EVENT_WITHER, urand(15000, 20000));
                            break;
                        case EVENT_SUMMON_SPORE:
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                                me->SummonCreature(NPC_SPORE, pTarget->GetPositionX() + rand()%10, pTarget->GetPositionY() + rand()%10, pTarget->GetPositionZ(), 0.0f);
                            events.ScheduleEvent(EVENT_SUMMON_SPORE, urand(20000, 23000));
                            break;
                        case EVENT_SUMMON_POD:
                            me->SummonCreature(NPC_SEEDLING_POD, me->GetPositionX() + rand()%10, me->GetPositionY() + rand()%10, me->GetPositionZ(), 0.0f);
                            events.ScheduleEvent(EVENT_SUMMON_POD, urand(15000, 23000));
                            break;
                        case EVENT_CONSUME_LIFE:
                            DoCast(me, SPELL_CONSUME_LIFE_SELF, true);
                            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                            {
                                switch (pTarget->getPowerType())
                                {
                                    case POWER_FOCUS:
                                        DoCast(pTarget, SPELL_CONSUME_LIFE_FOCUS);
                                        break;
                                    case POWER_ENERGY:
                                        DoCast(pTarget, SPELL_CONSUME_LIFE_ENERGY);
                                        break;
                                    case POWER_RUNIC_POWER:
                                        DoCast(pTarget, SPELL_CONSUME_LIFE_RUNIC);
                                        break;
                                    case POWER_RAGE:
                                        DoCast(pTarget, SPELL_CONSUME_LIFE_RAGE);
                                        break;
                                    default:
                                        DoCast(pTarget, SPELL_CONSUME_LIFE_MANA);
                                        break;
                                }
                            }
                            events.ScheduleEvent(EVENT_CONSUME_LIFE, urand(18000, 20000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class npc_ammunae_seedling_pod : public CreatureScript
{
    public:
        npc_ammunae_seedling_pod() : CreatureScript("npc_ammunae_seedling_pod") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_ammunae_seedling_podAI(pCreature);
        }

        struct npc_ammunae_seedling_podAI : public Scripted_NoMovementAI
        {
            npc_ammunae_seedling_podAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
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

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                DoCast(me, SPELL_SEEDLING_POD, true);
                if (Creature* pAmmunae = me->FindNearestCreature(NPC_AMMUNAE, 100.0f, true))
                    DoCast(pAmmunae, SPELL_ENERGIZE);
            }

            void UpdateAI(uint32 const diff)
            {
            }

            void JustDied(Unit* /*killer*/)
            {
                me->DespawnOrUnsummon();
            }
        };

};

class npc_ammunae_spore : public CreatureScript
{
    public:
        npc_ammunae_spore() : CreatureScript("npc_ammunae_spore") { }

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_ammunae_sporeAI(pCreature);
        }

        struct npc_ammunae_sporeAI : public ScriptedAI
        {
            npc_ammunae_sporeAI(Creature* pCreature) : ScriptedAI(pCreature)
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
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                me->GetMotionMaster()->MoveRandom(15.0f);
            }
            
            void DamageTaken(Unit* /*who*/, uint32& damage)
            {
                if (damage >= me->GetHealth())
                {
					damage = 0;
					me->GetMotionMaster()->Clear();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                    me->SetStandState(UNIT_STAND_STATE_DEAD);
                    me->SetHealth(me->GetMaxHealth());
                    me->RemoveAllAuras();
					DoCast(me, SPELL_SPORE_CLOUD); 
                }
            }

            void UpdateAI(uint32 const diff)
            {
            }
        };
};

void AddSC_boss_ammunae()
{
    new boss_ammunae();
    new npc_ammunae_seedling_pod();
    new npc_ammunae_spore();
}
