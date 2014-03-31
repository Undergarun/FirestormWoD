#include"ScriptPCH.h"
#include"Spell.h"
#include"shadowfang_keep.h"



#define SAY_AGGRO_ALLIANCE "Ivar's pack failed horribly, so now he sends mercenaries. No matter, your journey ends in agony."
#define SAY_AGGRO_HORDE "That bitch Sylvanas sent you too, did she? No matter, your journey ends in agony."
#define SAY_DEATH "I thank you for the clean death. I would not of done the same..."
#define SAY_KILL1 "Murder never gets old."
#define SAY_KILL2 "This is the quality of assassin sent to kill me? Laughable."

enum ScriptTexts
{
};

enum Spells
{
	SPELL_PISTOL_BARRAGE				= 93520,
	SPELL_PISTOL_BARRAGE_DUMMY			= 96345,
	SPELL_PISTOL_BARRAGE_AURA			= 93566,
	SPELL_PISTOL_BARRAGE_DMG			= 93564,
	SPELL_PISTOL_BARRAGE_DMG_H			= 93784,
	SPELL_PISTOL_BARRAGE_MISSILE_SUM	= 96344,
	SPELL_PISTOL_BARRAGE_SUM			= 96343,
	SPELL_PISTOL_BARRAGE_DUMMY_1		= 93557,
	SPELL_PISTOL_BARRAGE_DUMMY_2		= 93558,
    SPELL_PISTOLS_VISUAL_PASSIVE        = 93597,
	SPELL_SUMMON_BLOODTHIRSTY_GHOULS	= 93707,
	SPELL_SUMMON_BLOODTHIRSTY_GHOULS_M	= 93709,
	SPELL_CURSED_BULLET					= 93629,
	SPELL_CURSED_BULLET_H				= 93629,
	SPELL_MORTAL_WOUND					= 93675,
	SPELL_MORTAL_WOUND_H				= 93675,
};

enum Events
{
	EVENT_PISTOL_BARRAGE                = 1,
	EVENT_SUMMON_BLOODTHIRSTY_GHOULS    = 2,
	EVENT_CURSED_BULLET                 = 3,
	EVENT_MORTAL_WOUND                  = 4,
};

enum Adds
{
	NPC_BLOODTHRISTY_GHOUL	= 50561,
	NPC_PISTOL_BARRAGE		= 52065,
};

class boss_lord_godfrey : public CreatureScript
{
    public:
        boss_lord_godfrey() : CreatureScript("boss_lord_godfrey") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_lord_godfreyAI(pCreature);
        }

	    struct boss_lord_godfreyAI : public BossAI
        {
		    boss_lord_godfreyAI(Creature* pCreature) : BossAI(pCreature, DATA_GODFREY)
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
            }

		    void Reset()
		    {
                _Reset();
		    }
    		
		    void EnterCombat(Unit* pWho)
		    {
			    events.ScheduleEvent(EVENT_MORTAL_WOUND, 10000);
			    events.ScheduleEvent(EVENT_CURSED_BULLET, 15000);
			    events.ScheduleEvent(EVENT_SUMMON_BLOODTHIRSTY_GHOULS, 17000);
			    //events.ScheduleEvent(EVENT_PISTOL_BARRAGE, 22000);
			    instance->SetBossState(DATA_GODFREY, IN_PROGRESS);
			    if (instance->GetData(DATA_TEAM) == ALLIANCE)
				    me->MonsterYell(SAY_AGGRO_ALLIANCE, 0, 0);
			    else
				    me->MonsterYell(SAY_AGGRO_HORDE, 0, 0);
			    DoZoneInCombat();
		    }
    		
		    void SpellHit(Unit* caster, SpellInfo const* spell)
		    {
                if (spell->HasEffect(SPELL_EFFECT_INTERRUPT_CAST))
			        if (me->GetCurrentSpell(CURRENT_GENERIC_SPELL))
				        if ((me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_CURSED_BULLET	) || 
					        (me->GetCurrentSpell(CURRENT_GENERIC_SPELL)->m_spellInfo->Id == SPELL_CURSED_BULLET_H))
							    me->InterruptSpell(CURRENT_GENERIC_SPELL, false);
		    }

		    void KilledUnit(Unit* who)
		    {
			    me->MonsterYell(urand(0, 1)? SAY_KILL1: SAY_KILL2, 0, 0);
		    }

		    void JustDied(Unit* who)
		    {
                _JustDied();
			    me->MonsterYell(SAY_DEATH, 0, 0);
		    }

		    void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
				    return;

			    events.Update(uiDiff);

			    if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
    		        
                while(uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
				        case EVENT_MORTAL_WOUND:
					        DoCast(me->getVictim(), SPELL_MORTAL_WOUND);
					        events.ScheduleEvent(EVENT_MORTAL_WOUND, 10000);
					        break;
				        case EVENT_CURSED_BULLET:
					        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
						        DoCast(target, SPELL_CURSED_BULLET);
					        events.ScheduleEvent(EVENT_CURSED_BULLET, 15000);
					        break;
				        case EVENT_SUMMON_BLOODTHIRSTY_GHOULS:
					        DoCast(SPELL_SUMMON_BLOODTHIRSTY_GHOULS);
					        events.ScheduleEvent(EVENT_SUMMON_BLOODTHIRSTY_GHOULS, 17000);
					        break;
				        case EVENT_PISTOL_BARRAGE:
					        //me->CastCustomSpell(SPELL_PISTOL_BARRAGE_MISSILE_SUM, SPELLVALUE_MAX_TARGETS, 1);
					        //DoCast(SPELL_PISTOL_BARRAGE);
					        events.ScheduleEvent(EVENT_PISTOL_BARRAGE, 17000);
					        break;
				    }
			    }
			    DoMeleeAttackIfReady();
		    }
	     };
};

class npc_godfrey_pistol_barrage : public CreatureScript
{
    public:
        npc_godfrey_pistol_barrage() : CreatureScript("npc_godfrey_pistol_barrage") { }
        
        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_godfrey_pistol_barrageAI(pCreature);
        }
        struct npc_godfrey_pistol_barrageAI : public Scripted_NoMovementAI
        {
		    npc_godfrey_pistol_barrageAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
            {
            }

		    void Reset()
		    {
		    }
    		
    		
		    void UpdateAI(const uint32 uiDiff)
            {
		    }
	 };
};

class spell_godfrey_summon_bloodthirsty_ghouls : public SpellScriptLoader
{
    public:
        spell_godfrey_summon_bloodthirsty_ghouls() : SpellScriptLoader("spell_godfrey_summon_bloodthirsty_ghouls") { }

        class spell_godfrey_summon_bloodthirsty_ghouls_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_godfrey_summon_bloodthirsty_ghouls_AuraScript);

            void HandleDummyTick(constAuraEffectPtr aurEff)
            {
				GetCaster()->CastSpell(GetCaster(), SPELL_SUMMON_BLOODTHIRSTY_GHOULS_M, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_godfrey_summon_bloodthirsty_ghouls_AuraScript::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
			}
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_godfrey_summon_bloodthirsty_ghouls_AuraScript();
        }
};

void AddSC_boss_lord_godfrey()
{
	new boss_lord_godfrey();
	new npc_godfrey_pistol_barrage();
	new spell_godfrey_summon_bloodthirsty_ghouls();
}



