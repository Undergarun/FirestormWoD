/*
* Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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
 
#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"
 
enum eLeiShiSpells
{
    SPELL_AFRAID    = 123181,
    SPELL_SPRAY     = 123121,
    SPELL_HIDE      = 123244,
    SPELL_GET_AWAY  = 123461,
    SPELL_PROTECT   = 123250
};
 
enum eLeiShiEvents
{
    EVENT_HIDE      = 1,
    EVENT_GET_AWAY  = 2
};
 
enum eLeiShiActions
{
};
 
enum eLeiShiSays
{
};
 
class boss_lei_shi : public CreatureScript
{
    public:
        boss_lei_shi() : CreatureScript("boss_lei_shi") { }
 
        struct boss_lei_shiAI : public BossAI
        {
            boss_lei_shiAI(Creature* creature) : BossAI(creature, DATA_LEI_SHI)
            {
                pInstance = creature->GetInstanceScript();
            }
 
            InstanceScript* pInstance;
            EventMap events;
 
            uint8 nextAfraidPct;
            uint8 nextProtectPct;
            uint8 protectorsToSummon;
 
            bool hidden;
            bool getAwayPhase;
            float getAwayHealthPct;
 
            void Reset()
            {
                _Reset();
 
                events.Reset();
 
                events.ScheduleEvent(EVENT_HIDE, 40000);
                events.ScheduleEvent(EVENT_GET_AWAY, 30000);
 
                summons.DespawnAll();
 
                hidden = false;
                getAwayPhase = false;
 
                nextAfraidPct = 90;
                nextProtectPct = 80;
                protectorsToSummon = 3;
                me->RemoveAura(SPELL_AFRAID);
 
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
 
                    if (pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION))
                        me->AddAura(SPELL_RITUAL_OF_PURIFICATION, me);
                }
            }
 
            void JustReachedHome()
            {
                _JustReachedHome();
 
                if (pInstance)
                    pInstance->SetBossState(DATA_LEI_SHI, FAIL);
            }
 
            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_LEI_SHI, IN_PROGRESS);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                }
            }
 
            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }
 
            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }
 
            void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    return;
            }
 
            void JustDied(Unit* killer)
            {
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_LEI_SHI, DONE);
                    _JustDied();
                }
            }
 
            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (!pInstance)
                    return;
 
                if (nextAfraidPct)
                {
                    if (me->HealthBelowPctDamaged(nextAfraidPct, damage))
                    {
                        if (nextAfraidPct >= 90)
                            me->CastSpell(me, SPELL_AFRAID, true);
                        else
                        {
                            if (AuraEffectPtr afraid = me->GetAuraEffect(SPELL_AFRAID, EFFECT_0))
                                afraid->ChangeAmount(afraid->GetAmount() + 8);
                        }
 
                        nextAfraidPct -= 10;
                    }
 
                    if (me->HealthBelowPctDamaged(int32(getAwayHealthPct - 4.0f), damage))
                    {
                        me->RemoveAura(SPELL_GET_AWAY);
                        getAwayPhase = false;
                    }
 
                    if (me->HealthBelowPctDamaged(nextProtectPct, damage))
                    {
                        nextProtectPct -= 20;
                        me->CastSpell(me, SPELL_PROTECT, false);
 
                        if (nextProtectPct <= 40)
                            ++protectorsToSummon;
                    }
                }
            }
 
            void DoAction(const int32 action)
            {
                switch (action)
                {
                    default:
                        break;
                }
            }
 
            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (pInstance && pInstance->GetData(SPELL_RITUAL_OF_PURIFICATION) == false)
                        me->RemoveAura(SPELL_RITUAL_OF_PURIFICATION);
 
                    return;
                }
 
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
 
                if (hidden)
                    return;
 
                events.Update(diff);
 
                switch (events.ExecuteEvent())
                {
                    case EVENT_HIDE:
                    {
                        me->CastSpell(me, SPELL_HIDE, false);
                        hidden = true;
                        events.ScheduleEvent(EVENT_HIDE, 40000);
                        return;
                    }
                    case EVENT_GET_AWAY:
                    {
                        me->CastSpell(me, SPELL_GET_AWAY, false);
                        getAwayPhase = true;
                        getAwayHealthPct = me->GetHealthPct();
                        events.ScheduleEvent(EVENT_GET_AWAY, 30000);
                        return;
                    }
                    default:
                        return;
                }
 
                // No need event, this spell replace melee attacks
                if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                    me->CastSpell(target, SPELL_SPRAY, false);
            }
        };
 
        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_lei_shiAI(creature);
        }
};
 
void AddSC_boss_lei_shi()
{
    new boss_lei_shi();
}
