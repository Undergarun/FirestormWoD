/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
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
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "upper_blackrock_spire.h"

enum eSpells
{
    // Boss
    SPELL_POWER_CONDUIT_AURA    = 166168,
    SPELL_BLADES_OF_STEEL       = 154417,
    SPELL_SHRAPNEL_NOVA         = 154448,
    SPELL_LODESTONE_SPIKE_DUMMY = 154435,
    SPELL_LODESTONE_SPIKE_DMG   = 154428,
    SPELL_THUNDER_CACOPHONY     = 154899,

    // Misc
    SPELL_POWER_CONDUIT_VISUAL  = 154294,   // Casted by Rune of Power on the boss
    SPELL_UNHARNESSED_POWER     = 154265,   // Stunned Aura (permanent)
    SPELL_ELECTRIC_PULSE_AURA   = 154335
};

enum eEvents
{
    EVENT_POWER_CONDUIT = 1,
    EVENT_BLADES_OF_STEEL,
    EVENT_SHRAPNEL_NOVA,
    EVENT_ELECTRIC_PULSE,
    EVENT_LODESTONE_SPIKE,
    EVENT_THUNDER_CACOPHONY
};

enum eSays
{
    TALK_AGGRO,
    TALK_DEATH,
    TALK_SLAY,
    TALK_SPELL_1,
    TALK_SPELL_2,
    TALK_SPELL_3,
    TALK_SPELL_4
};

enum eActions
{
    ACTION_POWER_CONDUIT
};

enum ePositions
{
    DATA_POS_NE,
    DATA_POS_SE,
    DATA_POS_SW,
    DATA_POS_NW
};

// DATA_POS_NE
Position const g_LightningFieldInitPos = { 161.228f, -276.8578f, 94.42406f, M_PI };

Position const g_LightningFieldMovePos[4] =
{
    { 161.228f, -276.8578f, 94.42406f, M_PI               },    // DATA_POS_NE
    { 127.786f, -276.5409f, 94.42406f, M_PI / 2.0f        },    // DATA_POS_SE
    { 127.786f, -240.7292f, 94.42406f, 0.0f               },    // DATA_POS_SW
    { 161.228f, -240.7292f, 94.42406f, M_PI + M_PI / 2.0f }     // DATA_POS_NW
};

// Orebender Gor'Ashan - 76413
class boss_orebender_gorashan : public CreatureScript
{
    public:
        boss_orebender_gorashan() : CreatureScript("boss_orebender_gorashan") { }

        struct boss_orebender_gorashanAI : public BossAI
        {
            boss_orebender_gorashanAI(Creature* p_Creature) : BossAI(p_Creature, DATA_OREBENDER_GORASHAN)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset()
            {
                _Reset();

                m_Events.Reset();

                me->ReenableEvadeMode();

                me->RemoveAllAreasTrigger();

                std::list<Creature*> l_RunesOfPower;
                me->GetCreatureListWithEntryInGrid(l_RunesOfPower, NPC_RUNE_OF_POWER, 50.f);

                for (Creature* l_Rune : l_RunesOfPower)
                    l_Rune->AI()->DoAction(ACTION_POWER_CONDUIT);
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(TALK_AGGRO);

                _EnterCombat();

                m_Events.Reset();
                m_Events.ScheduleEvent(EVENT_BLADES_OF_STEEL, 8000);
                m_Events.ScheduleEvent(EVENT_SHRAPNEL_NOVA, 12000);
                m_Events.ScheduleEvent(EVENT_ELECTRIC_PULSE, 16000);
                m_Events.ScheduleEvent(EVENT_LODESTONE_SPIKE, 20000);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(TALK_DEATH);

                _JustDied();

                // Defeat Orebender Gor'ashan without allowing him to cast Thundering Cacophony 4 times in Upper Blackrock Spire on Heroic difficulty.
                if (IsHeroic() && m_Instance && m_Instance->GetData(DATA_MAGNETS_ACHIEVEMENT) < 4)
                    m_Instance->DoCompleteAchievement(ACHIEV_MAGNETS_HOW_DO_THEY_WORK);
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == SPELL_POWER_CONDUIT_VISUAL)
                {
                    me->CastSpell(me, SPELL_POWER_CONDUIT_AURA, true);
                    m_Events.ScheduleEvent(EVENT_THUNDER_CACOPHONY, 1000);
                }
            }

            void DoAction(const int32 p_Action)
            {
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
                    case EVENT_BLADES_OF_STEEL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_BLADES_OF_STEEL, false);
                        m_Events.ScheduleEvent(EVENT_BLADES_OF_STEEL, 20000);
                        break;
                    case EVENT_SHRAPNEL_NOVA:
                        me->CastSpell(me, SPELL_SHRAPNEL_NOVA, false);
                        m_Events.ScheduleEvent(EVENT_SHRAPNEL_NOVA, 25000);
                        break;
                    case EVENT_ELECTRIC_PULSE:
                        me->SummonCreature(NPC_LIGHTNING_FIELD, g_LightningFieldInitPos);
                        m_Events.ScheduleEvent(EVENT_ELECTRIC_PULSE, 30000);
                        break;
                    case EVENT_LODESTONE_SPIKE:
                        me->CastSpell(me, SPELL_LODESTONE_SPIKE_DUMMY, false);
                        m_Events.ScheduleEvent(EVENT_LODESTONE_SPIKE, 20000);
                        break;
                    case EVENT_THUNDER_CACOPHONY:
                        if (m_Instance)
                            m_Instance->SetData(DATA_MAGNETS_ACHIEVEMENT, 1);
                        me->CastSpell(me, SPELL_THUNDER_CACOPHONY, false);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_orebender_gorashanAI(p_Creature);
        }
};

// Black Iron Apprentice - 76773
class mob_black_iron_apprentice : public CreatureScript
{
    public:
        mob_black_iron_apprentice() : CreatureScript("mob_black_iron_apprentice") { }

        struct mob_black_iron_apprenticeAI : public ScriptedAI
        {
            mob_black_iron_apprenticeAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_InCombat;

            void Reset()
            {
                me->ReenableEvadeMode();

                m_InCombat = false;
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (m_InCombat)
                    return;

                m_InCombat = true;
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER || m_InCombat)
                    return;

                if (p_Who->GetDistance(me) >= 11.f)
                    return;

                m_InCombat = true;

                std::list<Creature*> l_ApprenticeList;
                me->GetCreatureListWithEntryInGrid(l_ApprenticeList, NPC_BLACK_IRON_APPRENTICE, 150.f);

                for (Creature* l_Apprentice : l_ApprenticeList)
                    l_Apprentice->AI()->AttackStart(p_Who);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim() || !m_InCombat)
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_apprenticeAI(p_Creature);
        }
};

// Rune of Power - 76417
class mob_rune_of_power : public CreatureScript
{
    public:
        mob_rune_of_power() : CreatureScript("mob_rune_of_power") { }

        struct mob_rune_of_powerAI : public ScriptedAI
        {
            mob_rune_of_powerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                me->RemoveAura(SPELL_UNHARNESSED_POWER);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_POWER_CONDUIT:
                        m_Events.ScheduleEvent(EVENT_POWER_CONDUIT, 2000);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_POWER_CONDUIT:
                        me->CastSpell(me, SPELL_POWER_CONDUIT_VISUAL, false);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_rune_of_powerAI(p_Creature);
        }
};

// Lightning Field - 76464
class mob_lightning_field : public CreatureScript
{
    public:
        mob_lightning_field() : CreatureScript("mob_lightning_field") { }

        struct mob_lightning_fieldAI : public ScriptedAI
        {
            mob_lightning_fieldAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

                me->CastSpell(me, SPELL_ELECTRIC_PULSE_AURA, true);

                me->SetSpeed(MOVE_FLIGHT, 0.2f, true);

                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint((DATA_POS_NE + 1), g_LightningFieldMovePos[DATA_POS_NE + 1]);
            }

            void MovementInform(uint32 p_Type, uint32 p_ID)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case DATA_POS_NE:
                    case DATA_POS_SE:
                    case DATA_POS_SW:
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint((p_ID + 1), g_LightningFieldMovePos[p_ID + 1]);
                        break;
                    case DATA_POS_NW:
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(DATA_POS_NE, g_LightningFieldMovePos[DATA_POS_NE]);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff) { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_lightning_fieldAI(p_Creature);
        }
};

// Lodestone Spike - 154435
class spell_lodestone_spike : public SpellScriptLoader
{
    public:
        spell_lodestone_spike() : SpellScriptLoader("spell_lodestone_spike") { }

        class spell_lodestone_spike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lodestone_spike_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (Unit* l_Caster = GetCaster())
                        l_Caster->CastSpell(l_Target, SPELL_LODESTONE_SPIKE_DMG, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_lodestone_spike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lodestone_spike_SpellScript();
        }
};

void AddSC_boss_orebender_gorashan()
{
    new boss_orebender_gorashan();
    new mob_black_iron_apprentice();
    new mob_rune_of_power();
    new mob_lightning_field();
    new spell_lodestone_spike();
}
