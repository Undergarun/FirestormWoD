////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "violet_hold.h"

enum Spells
{
    SPELL_SHROUD_OF_DARKNESS                    = 54524,
    H_SPELL_SHROUD_OF_DARKNESS                  = 59745,
    SPELL_SUMMON_VOID_SENTRY                    = 54369,
    SPELL_VOID_SHIFT                            = 54361,
    H_SPELL_VOID_SHIFT                          = 59743,

    SPELL_ZURAMAT_ADD_2                         = 54342,
    H_SPELL_ZURAMAT_ADD_2                       = 59747
};

enum ZuramatCreatures
{
    CREATURE_VOID_SENTRY                        = 29364
};

enum Yells
{
    SAY_AGGRO                                   = -1608037,
    SAY_SLAY_1                                  = -1608038,
    SAY_SLAY_2                                  = -1608039,
    SAY_SLAY_3                                  = -1608040,
    SAY_DEATH                                   = -1608041,
    SAY_SPAWN                                   = -1608042,
    SAY_SHIELD                                  = -1608043,
    SAY_WHISPER                                 = -1608044
};

#define DATA_VOID_DANCE                         2153

class boss_zuramat : public CreatureScript
{
public:
    boss_zuramat() : CreatureScript("boss_zuramat") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_zuramatAI (creature);
    }

    struct boss_zuramatAI : public ScriptedAI
    {
        boss_zuramatAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 SpellVoidShiftTimer;
        uint32 SpellSummonVoidTimer;
        uint32 SpellShroudOfDarknessTimer;
        bool voidDance;

        void Reset()
        {
            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }

            SpellShroudOfDarknessTimer = 22000;
            SpellVoidShiftTimer = 15000;
            SpellSummonVoidTimer = 12000;
            voidDance = true;
        }

        void AttackStart(Unit* who)
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
            if (instance)
            {
                if (GameObject* pDoor = instance->instance->GetGameObject(instance->GetData64(DATA_ZURAMAT_CELL)))
                    if (pDoor->GetGoState() == GO_STATE_READY)
                    {
                        EnterEvadeMode();
                        return;
                    }
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (SpellSummonVoidTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SUMMON_VOID_SENTRY, false);
                SpellSummonVoidTimer = 20000;
            } else SpellSummonVoidTimer -=diff;

            if (SpellVoidShiftTimer <= diff)
            {
                 if (Unit* unit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(unit, SPELL_VOID_SHIFT);
                SpellVoidShiftTimer = 20000;
            } else SpellVoidShiftTimer -=diff;

            if (SpellShroudOfDarknessTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHROUD_OF_DARKNESS);
                SpellShroudOfDarknessTimer = 20000;
            } else SpellShroudOfDarknessTimer -=diff;

            DoMeleeAttackIfReady();
        }

        void SummonedCreatureDies(Creature* summoned, Unit* /*who*/)
        {
            if (summoned->GetEntry() == CREATURE_VOID_SENTRY)
                voidDance = false;
        }

        uint32 GetData(uint32 type)
        {
            if (type == DATA_VOID_DANCE)
                return voidDance ? 1 : 0;

            return 0;
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);

            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    instance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    instance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 13);
                }
            }
        }

        void KilledUnit(Unit* victim)
        {
            if (victim == me)
                return;

            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3), me);
        }

        void JustSummoned(Creature* summon)
        {
            summon->AI()->AttackStart(me->getVictim());
            summon->AI()->DoCastAOE(SPELL_ZURAMAT_ADD_2);
            summon->SetPhaseMask(17, true);
        }
    };

};

class achievement_void_dance : public AchievementCriteriaScript
{
    public:
        achievement_void_dance() : AchievementCriteriaScript("achievement_void_dance")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Zuramat = target->ToCreature())
                if (Zuramat->AI()->GetData(DATA_VOID_DANCE))
                    return true;

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_zuramat()
{
    new boss_zuramat();
    new achievement_void_dance();
}
#endif
