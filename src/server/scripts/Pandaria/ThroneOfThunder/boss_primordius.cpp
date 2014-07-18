/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "throne_of_thunder.h"
#include "Vehicle.h"
#include "ThreatManager.h"

enum eSpells
{
    SPELL_PRIMORDIAL_STRIKE         = 136037
};

enum eEvents
{
    EVENT_PRIMORDIAL_STRIKE         = 1
};

enum eActions
{
};

enum eDatas
{
};

// Primordius - 69017
class boss_primordius : public CreatureScript
{
public:
    boss_primordius() : CreatureScript("boss_primordius") { }

    struct boss_primordiusAI : public BossAI
    {
        boss_primordiusAI(Creature* creature) : BossAI(creature, DATA_PRIMORDIUS)
        {
            pInstance = creature->GetInstanceScript();
        }

        EventMap events;
        InstanceScript* pInstance;

        void Reset()
        {
            if (pInstance)
            {
                if (pInstance->GetBossState(FAIL))
                    pInstance->SetBossState(DATA_PRIMORDIUS, NOT_STARTED);
            }

            _Reset();
            me->GetMotionMaster()->MoveTargetedHome();
            me->ReenableEvadeMode();
        }

        void EnterEvadeMode()
        {
            if (pInstance)
            {
                pInstance->SetBossState(DATA_PRIMORDIUS, FAIL);
                pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            me->SetFullHealth();
            me->RemoveAllAuras();
            _EnterEvadeMode();
            me->GetMotionMaster()->MoveTargetedHome();
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }

        void EnterCombat(Unit* who)
        {
            events.Reset();

            events.ScheduleEvent(EVENT_PRIMORDIAL_STRIKE, 2000);
        }

        void JustDied(Unit* killer)
        {
            summons.DespawnAll();

            if (pInstance)
            {
                pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                pInstance->SetBossState(DATA_PRIMORDIUS, DONE);
            }
        }

        void DoAction(const int32 action)
        {
        }

        void DamageTaken(Unit* /*killer*/, uint32 &damage)
        {
        }

        void KilledUnit(Unit* victim)
        {
            Talk(urand(5, 6));
        }

        void UpdateAI(const uint32 diff)
        {
            if (pInstance)
            {
                if (pInstance->IsWipe())
                {
                    EnterEvadeMode();
                    return;
                }
            }

            if (!UpdateVictim())
            {
                if (me->isInCombat())
                    me->CombatStop();
                EnterEvadeMode();
                return;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_PRIMORDIAL_STRIKE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(target, SPELL_PRIMORDIAL_STRIKE, false);
                    events.ScheduleEvent(EVENT_PRIMORDIAL_STRIKE, 20000);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_primordiusAI(creature);
    }
};

void AddSC_boss_primordius()
{
    new boss_primordius();
}