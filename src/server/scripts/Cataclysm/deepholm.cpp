#include "ScriptPCH.h"

enum CreatureIds
{
    NPC_AEONAXX = 50062,
};

class boss_aeonaxx : public CreatureScript
{
public:
    boss_aeonaxx() : CreatureScript("boss_aeonaxx") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_aeonaxxAI(pCreature);
    }

    struct boss_aeonaxxAI : public ScriptedAI
    {
        boss_aeonaxxAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
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

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

class spell_deepholm_kill_all_constructs : public SpellScriptLoader
{
public:
    spell_deepholm_kill_all_constructs() : SpellScriptLoader("spell_deepholm_kill_all_constructs") { }


    class spell_deepholm_kill_all_constructs_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_deepholm_kill_all_constructs_SpellScript);

        void HandleDummy()
        {
            if (!GetCaster())
                return;
            
            if (Player* player = GetCaster()->ToPlayer())
            {
                if (player->GetQuestStatus(26762) == QUEST_STATUS_INCOMPLETE)
                    player->KilledMonsterCredit(43984, 0);
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_deepholm_kill_all_constructs_SpellScript::HandleDummy);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_deepholm_kill_all_constructs_SpellScript();
    }
};

void AddSC_deepholm()
{
    new boss_aeonaxx();

    new spell_deepholm_kill_all_constructs();
}