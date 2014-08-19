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

enum eSpells
{
    // Intro
    SPELL_FORCE_TO_JUMP         = 138359,

    // Ji-Kun
    SPELL_TALON_RAKE            = 134366,
    SPELL_CAW_FIRST             = 138923,
    SPELL_CAW_MISSILE           = 138926,
    SPELL_QUILLS                = 134380,
    SPELL_INFECTED_TALONS_PROC  = 140094,
    SPELL_INFECTED_TALONS_AURA  = 140092
};

enum eEvents
{
    EVENT_TALON_RAKE    = 1,
    EVENT_CAW,
    EVENT_QUILLS
};

enum eActions
{
};

Position const waypointPos[52] =
{
    { 6211.530f, 4352.063f, -41.23960f, 0.0f },
    { 6219.918f, 4323.225f, -50.82288f, 0.0f },
    { 6200.393f, 4290.322f, -58.99046f, 0.0f },
    { 6167.124f, 4255.000f, -71.33135f, 0.0f },
    { 6130.888f, 4247.688f, -77.82609f, 0.0f },
    { 6094.024f, 4258.713f, -85.37042f, 0.0f },
    { 6080.453f, 4294.052f, -87.51177f, 0.0f },
    { 6083.603f, 4316.073f, -83.81752f, 0.0f },
    { 6091.601f, 4337.876f, -79.24355f, 0.0f },
    { 6115.493f, 4360.595f, -71.60667f, 0.0f },
    { 6143.307f, 4364.279f, -64.42132f, 0.0f },
    { 6170.492f, 4353.606f, -61.10223f, 0.0f },
    { 6189.739f, 4341.566f, -57.74512f, 0.0f },
    { 6203.971f, 4317.918f, -49.33831f, 0.0f },
    { 6206.109f, 4289.632f, -44.60070f, 0.0f },
    { 6189.743f, 4265.107f, -39.40984f, 0.0f },
    { 6163.300f, 4250.018f, -32.56945f, 0.0f },
    { 6136.179f, 4247.880f, -25.26046f, 0.0f },
    { 6093.636f, 4266.221f, -8.476698f, 0.0f },
    { 6075.295f, 4302.147f, 9.5958010f, 0.0f },
    { 6086.486f, 4356.149f, 34.423870f, 0.0f },
    { 6135.314f, 4386.748f, 58.916970f, 0.0f },
    { 6174.528f, 4380.014f, 65.794550f, 0.0f },
    { 6208.828f, 4352.507f, 70.634090f, 0.0f },
    { 6215.530f, 4317.869f, 71.754340f, 0.0f },
    { 6198.188f, 4279.812f, 75.545790f, 0.0f },
    { 6149.543f, 4285.440f, 75.545790f, 0.0f },
    { 6132.934f, 4319.959f, 75.545790f, 0.0f },
    { 6164.056f, 4341.441f, 74.958320f, 0.0f },
    { 6192.802f, 4302.389f, 72.754180f, 0.0f },
    { 6180.481f, 4254.817f, 67.915810f, 0.0f },
    { 6135.318f, 4246.200f, 61.224780f, 0.0f },
    { 6086.058f, 4267.385f, 53.709340f, 0.0f },
    { 6075.467f, 4313.128f, 51.114910f, 0.0f },
    { 6105.487f, 4349.319f, 43.703910f, 0.0f },
    { 6141.576f, 4359.033f, 34.561810f, 0.0f },
    { 6167.522f, 4340.946f, 27.807100f, 0.0f },
    { 6165.942f, 4305.924f, 20.013400f, 0.0f },
    { 6142.460f, 4289.088f, 9.9207350f, 0.0f },
    { 6121.972f, 4307.348f, -4.077662f, 0.0f },
    { 6130.896f, 4333.039f, -10.56661f, 0.0f },
    { 6155.038f, 4336.327f, -13.13866f, 0.0f },
    { 6169.749f, 4320.901f, -14.18135f, 0.0f },
    { 6174.592f, 4306.330f, -15.45293f, 0.0f },
    { 6172.712f, 4289.999f, -17.64817f, 0.0f },
    { 6162.471f, 4278.408f, -20.11584f, 0.0f },
    { 6151.760f, 4278.437f, -21.28869f, 0.0f },
    { 6144.905f, 4284.731f, -22.05752f, 0.0f },
    { 6142.729f, 4294.742f, -24.24618f, 0.0f },
    { 6145.100f, 4315.314f, -26.73017f, 0.0f },
    { 6148.108f, 4324.893f, -25.96644f, 0.0f },
    { 6160.587f, 4354.137f, -24.45477f, 0.0f }
};

Position const bossPlatformPos = { 6112.219f, 4285.634f, -30.04051f, 0.f };

Position const featherPos[5] =
{
    { 6125.65f, 4355.60f, -31.8626f, 5.25605f },
    { 6110.11f, 4296.95f, -31.8626f, 0.49317f },
    { 6169.00f, 4281.05f, -31.8626f, 2.10022f },
    { 6184.19f, 4339.61f, -31.8627f, 3.66889f },
    { 6084.16f, 4428.31f, -119.633f, 5.20972f }
};

// Ji-Kun - 69712
class boss_ji_kun : public CreatureScript
{
    public:
        boss_ji_kun() : CreatureScript("boss_ji_kun") { }

        struct boss_ji_kunAI : public BossAI
        {
            boss_ji_kunAI(Creature* p_Creature) : BossAI(p_Creature, DATA_JI_KUN)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Activated = false;
                m_ActualWaypoint = 0;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            bool m_Activated;
            uint32 m_ActualWaypoint;

            void Reset()
            {
                m_Events.Reset();

                _Reset();

                me->CastSpell(me, SPELL_INFECTED_TALONS_PROC, true);
                me->ReenableEvadeMode();

                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFECTED_TALONS_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TALON_RAKE);
                }

                if (!m_Activated)
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(m_ActualWaypoint, waypointPos[0]);
                }
            }

            void EnterCombat(Unit* attacker)
            {
                m_Events.ScheduleEvent(EVENT_TALON_RAKE, 24000);
                m_Events.ScheduleEvent(EVENT_CAW, urand(18000, 50000));
                m_Events.ScheduleEvent(EVENT_QUILLS, urand(42500, 60000));
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                uint32 l_NextId = p_Id < 51 ? p_Id++ : 0;
                me->GetMotionMaster()->MovePoint(l_NextId, waypointPos[l_NextId]);
            }

            void JustDied(Unit* killer)
            {
                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                }
            }

            void DoAction(const int32 action)
            {
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_TALON_RAKE:
                        DoCastVictim(SPELL_TALON_RAKE);
                        m_Events.ScheduleEvent(EVENT_TALON_RAKE, urand(20000, 30000));
                        break;
                    case EVENT_CAW:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_CAW_FIRST, false);
                        m_Events.ScheduleEvent(EVENT_CAW, urand(18000, 50000));
                        break;
                    case EVENT_QUILLS:
                        me->CastSpell(me, SPELL_QUILLS, true);
                        m_Events.ScheduleEvent(EVENT_QUILLS, urand(42500, 60000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_ji_kunAI(creature);
        }
};

// Jump to Boss Platform - 69885
class mob_jump_to_boss_platform : public CreatureScript
{
    public:
        mob_jump_to_boss_platform() : CreatureScript("mob_jump_to_boss_platform") { }

        struct mob_jump_to_boss_platformAI : public ScriptedAI
        {
            mob_jump_to_boss_platformAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_CheckTimer;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);

                m_CheckTimer = 500;
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= diff)
                    {
                        std::list<Player*> l_PlayerList;
                        me->GetPlayerListInGrid(l_PlayerList, 13.f);

                        for (Player* l_Player : l_PlayerList)
                            l_Player->CastSpell(bossPlatformPos.m_positionX, bossPlatformPos.m_positionY, bossPlatformPos.m_positionZ, SPELL_FORCE_TO_JUMP, true);

                        m_CheckTimer = 500;
                    }
                    else
                        m_CheckTimer -= diff;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_jump_to_boss_platformAI(creature);
        }
};

// Caw - 138923
class spell_caw : public SpellScriptLoader
{
    public:
        spell_caw() : SpellScriptLoader("spell_caw") { }

        class spell_caw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_caw_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_CAW_MISSILE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_caw_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_caw_SpellScript();
        }
};

// Infected Talons - 140094
class spell_infected_talons : public SpellScriptLoader
{
    public:
        spell_infected_talons() : SpellScriptLoader("spell_infected_talons") { }

        class spell_infected_talons_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_infected_talons_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Unit* caster = GetTarget();

                if (!caster || !eventInfo.GetActor() || !caster->ToCreature())
                    return;

                if (caster != eventInfo.GetActor())
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo() && eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_INFECTED_TALONS_AURA)
                    return;

                if (!eventInfo.GetDamageInfo()->GetDamage())
                    return;

                if (Unit* target = caster->getVictim())
                    caster->CastSpell(target, SPELL_INFECTED_TALONS_AURA, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_infected_talons_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_infected_talons_AuraScript();
        }
};

void AddSC_boss_ji_kun()
{
    new boss_ji_kun();
    new mob_jump_to_boss_platform();
    new spell_caw();
    new spell_infected_talons();
}
