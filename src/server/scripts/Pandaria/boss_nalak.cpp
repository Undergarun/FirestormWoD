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
#include "SpellScript.h"

#define BOSS_NALAK 0

enum eSpells
{
    SPELL_STATIC_SHIELD         = 136341,
    SPELL_ARC_NOVA              = 136338,
    SPELL_LIGHTNING_TETHER      = 136339,
    SPELL_LIGHTNING_TETHER_10K  = 136353,
    SPELL_LIGHTNING_TETHER_20K  = 136349,
    SPELL_STORM_CLOUD           = 136340
};

enum eEvents
{
    EVENT_ARC_NOVA  = 1,
    EVENT_LIGHTNING_TETHER,
    EVENT_STORM_CLOUD
};

enum eSays
{
    TALK_INTRO,         // I am born of thunder!
    TALK_AGGRO,         // Can you feel a chill wind blow? The storm is coming...
    TALK_ARC_NOVA,      // The clouds arc with vengeance!
    TALK_STORMCLOUD,    // The air crackles with anger!
    TALK_SLAY,          // The sky weeps for your demise!
    TALK_DEATH          // I am but...the darkness...before the storm...
};

// Nalak - 69099
class boss_nalak : public CreatureScript
{
    public:
        boss_nalak() : CreatureScript("boss_nalak") { }

        struct boss_nalakAI : public BossAI
        {
            boss_nalakAI(Creature* p_Creature) : BossAI(p_Creature, BOSS_NALAK)
            {
                m_IntroDone = false;
            }

            EventMap m_Events;

            bool m_IntroDone;

            void Reset()
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(SPELL_STATIC_SHIELD);
            }

            void MoveInLineOfSight(Unit* p_Mover)
            {
                if (m_IntroDone)
                    return;

                if (p_Mover->IsPlayer())
                {
                    Talk(TALK_INTRO);
                    m_IntroDone = true;
                }
            }

            void KilledUnit(Unit* p_Killed)
            {
                if (p_Killed->IsPlayer())
                    Talk(TALK_SLAY);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(TALK_DEATH);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                me->CastSpell(me, SPELL_STATIC_SHIELD, true);

                Talk(TALK_AGGRO);

                m_Events.ScheduleEvent(EVENT_STORM_CLOUD, 15000);
                m_Events.ScheduleEvent(EVENT_LIGHTNING_TETHER, 28000);
                m_Events.ScheduleEvent(EVENT_ARC_NOVA, 39000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_STORM_CLOUD:
                        Talk(TALK_STORMCLOUD);
                        me->CastSpell(me, SPELL_STORM_CLOUD, false);
                        m_Events.ScheduleEvent(EVENT_STORM_CLOUD, 24000);
                        break;
                    case EVENT_LIGHTNING_TETHER:
                        me->CastSpell(me, SPELL_LIGHTNING_TETHER, false);
                        m_Events.ScheduleEvent(EVENT_LIGHTNING_TETHER, 35000);
                        break;
                    case EVENT_ARC_NOVA:
                        Talk(TALK_ARC_NOVA);
                        me->CastSpell(me, SPELL_ARC_NOVA, false);
                        m_Events.ScheduleEvent(EVENT_ARC_NOVA, 42000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_nalakAI(p_Creature);
        }
};

// Lightning Tether (periodic triggered) - 136350
class spell_lightning_tether: public SpellScriptLoader
{
    public:
        spell_lightning_tether() : SpellScriptLoader("spell_lightning_tether") { }

        class spell_lightning_tether_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lightning_tether_SpellScript);

            // Only if target is above 30 yards
            void HandleScript(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target->GetDistance(l_Caster) > 30.f)
                            l_Caster->CastSpell(l_Target, SPELL_LIGHTNING_TETHER_20K, true);
                    }
                }
            }

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), SPELL_LIGHTNING_TETHER_10K, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_lightning_tether_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnEffectHitTarget += SpellEffectFn(spell_lightning_tether_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lightning_tether_SpellScript();
        }
};

void AddSC_boss_nalak()
{
    new boss_nalak();
    new spell_lightning_tether();
}
