/*
* Copyright (C) 2014-20xx AshranCore <http://www.ashran.com/>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "frostfire_ridge.h"

// 80167 - Groog
class npc_groog : public CreatureScript
{
    public:
        npc_groog() : CreatureScript("npc_groog") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_groogAI(creature);
        }

        struct npc_groogAI : public ScriptedAI
        {
            npc_groogAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->setFaction(14);
            }

            void EnterCombat(Unit* p_Victim)
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(FrostfireRidgeEvents::EventEarthrendingSlam, 3000);
                m_Events.ScheduleEvent(FrostfireRidgeEvents::EventRampage, 7000);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case FrostfireRidgeEvents::EventEarthrendingSlam:
                        me->CastSpell(me, FrostfireRidgeSpells::SpellEarthrendingSlam, false);
                        m_Events.ScheduleEvent(FrostfireRidgeEvents::EventEarthrendingSlam, 15000);
                        break;
                    case FrostfireRidgeEvents::EventRampage:
                        me->AddAura(FrostfireRidgeSpells::SpellRampage, me);
                        m_Events.ScheduleEvent(FrostfireRidgeEvents::EventRampage, 15000);
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };
};

/// Rampage - 148852
class spell_groog_rampage : public SpellScriptLoader
{
    public:
        spell_groog_rampage() : SpellScriptLoader("spell_groog_rampage") { }

        class spell_groog_rampage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_groog_rampage_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                PreventDefaultAction();

                std::list<Player*> l_PlayerList;
                l_Caster->GetPlayerListInGrid(l_PlayerList, 2.0f);

                l_Caster->HandleEmoteCommand(EMOTE_ONESHOT_ATTACK1H);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->HasUnitState(UNIT_STATE_ROOT))
                        continue;

                    if (l_Player->IsNonMeleeSpellCasted(true))
                        l_Player->InterruptNonMeleeSpells(true);

                    l_Player->KnockbackFrom(l_Player->m_positionX, l_Player->m_positionY, 10.0f, 10.0f);
                    l_Player->SetKnockBackTime(getMSTime());

                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_groog_rampage_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_groog_rampage_AuraScript();
        }
};

void AddSC_frostfire_ridge()
{
    new npc_groog();
    new spell_groog_rampage();
}