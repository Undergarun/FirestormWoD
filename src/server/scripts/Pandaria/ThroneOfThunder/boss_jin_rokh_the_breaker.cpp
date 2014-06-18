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

enum eSpells
{
    SPELL_STATIC_BURST                      = 137162,
    SPELL_STATIC_WOUND                      = 138349,
    SPELL_STATIC_WOUND_DAMAGE               = 138389,

    SPELL_FOCUSED_LIGHTNING                 = 137194,
    SPELL_FOCUSED_LIGHTNING_VISUAL          = 139233,
    SPELL_FOCUSED_LIGHTNING_PERIODIC        = 137389,
    SPELL_FOCUSED_LIGHTNING_AREA            = 137429,
    SPELL_FOCUSED_LIGHTNING_DAMAGE          = 137423,
    SPELL_FOCUSED_LIGHTNING_DETONATION      = 139211,

    SPELL_LIGHTNING_FISSURE_SUMMON          = 137479,
    SPELL_LIGHTNING_FISSURE_VISUAL          = 137480,
    SPELL_LIGHTNING_FISSURE_PERIODIC        = 137484,

    // In EffectMovementGenerator::MovementInform(Unit &unit)
    SPELL_THUNDERING_THROW_AOE              = 137167,
    SPELL_THUNDERING_THROW_STUN_PLAYER      = 137371,

    SPELL_CONTROL_VEHICLE                   = 43671,
    SPELL_THUNDERING_THROW_VEHICLE          = 137161,
    SPELL_THUNDERING_THROW_DAMAGE           = 137370,
    SPELL_THUNDERING_THROW_EJECT_PLAYER     = 137180,
    SPELL_THUNDERING_THROW_JUMP_DEST        = 137173,

    SPELL_CONDUCTIVE_WATER_SUMMON           = 137145,
    SPELL_CONDUCTIVE_WATER_VISUAL           = 137277,
    SPELL_CONDUCTIVE_WATER_FOUNTAIN         = 137340,
    SPELL_CONDUCTIVE_WATER_GROW_SCALE       = 137676,
    SPELL_CONDUCTIVE_WATER_DAMAGE_TAKEN     = 138470,
    SPELL_CONDUCTIVE_WATER_FLUIDITY         = 138002,
    SPELL_CONDUCTIVE_WATER_ELECTRIFIED      = 137978,
    SPELL_CONDUCTIVE_WATER_LIGHT_VISUAL     = 138568,

    SPELL_FOCUSED_LIGHTNING_CONDUCTION      = 137530,
    SPELL_LIGHTNING_FISSURE_CONDUCTION      = 138133,

    SPELL_LIGHTNING_STORM                   = 137313,
    SPELL_LIGHTNING_STORM_SUMMON            = 137283,
    SPELL_LIGHTNING_STORM_SPAWN_EFFECT      = 137260,
    SPELL_LIGHTNING_STORM_SMALL             = 140811,
    SPELL_LIGHTNING_STORM_BIG               = 140555
};

enum eEvents
{
    EVENT_ENRAGE                        = 1,
    EVENT_STATIC_BURST                  = 2,
    EVENT_FOCUSED_LIGHTNING             = 3,
    EVENT_THUNDERING_THROW              = 4,
    EVENT_EJECT_PLAYER                  = 5,
    EVENT_CONDUCTIVE_WATER_DAMAGE_TAKEN = 6,
    EVENT_STOP_FOUNTAINS                = 7,
    EVENT_SPAWN_CONDUCTIVE_WATER        = 8,
    EVENT_LIGHTNING_STORM               = 9
};

enum eSays
{
    TALK_INTRO,
    TALK_AGGRO,
    TALK_FOCUSED_LIGHTNING,
    TALK_STATIC_BURST,
    TALK_THUNDERING_THROW,
    TALK_LIGHTNING_STORM,
    TALK_SLAY,
    TALK_ENRAGE,
    TALK_DEATH
};

enum eActions
{
};

enum ePhases
{
    PHASE_NONE,
    PHASE_STATUE_NW,
    PHASE_STATUE_SW,
    PHASE_STATUE_NE,
    PHASE_STATUE_SE
};

enum eDatas
{
    DATA_PHASE
};

Position const conductiveWatersPos[4] = 
{
    { 5918.442f, 6290.155f, 124.0338f, 0.0f }, // NW
    { 5864.987f, 6290.292f, 124.0336f, 0.0f }, // SW
    { 5918.523f, 6236.663f, 124.0336f, 0.0f }, // NE
    { 5865.079f, 6236.555f, 124.0335f, 0.0f }  // SE
};

// Jin'Rokh the Breaker - 69465
class boss_jin_rokh_the_breaker : public CreatureScript
{
    public:
        boss_jin_rokh_the_breaker() : CreatureScript("boss_jin_rokh_the_breaker") { }

        struct boss_jin_rokh_the_breakerAI : public BossAI
        {
            boss_jin_rokh_the_breakerAI(Creature* creature) : BossAI(creature, DATA_JIN_ROKH_THE_BREAKER), vehicle(creature->GetVehicleKit())
            {
                pInstance = creature->GetInstanceScript();
                introDone = false;
            }

            EventMap events;
            InstanceScript* pInstance;
            Vehicle* vehicle;
            uint8 statuesPhase;
            bool introDone;

            void Reset()
            {
                statuesPhase = PHASE_NONE;

                events.Reset();

                _Reset();

                summons.DespawnAll();
                DespawnAllVisuals();

                if (vehicle)
                    vehicle->RemoveAllPassengers();

                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->RemoveAura(SPELL_ENRAGE);

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, NOT_STARTED);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STATIC_WOUND);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONTROL_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_STUN_PLAYER);

                    uint32 firstMoguFountain = GOB_MOGU_FOUNTAIN_SE;
                    for (uint32 i = 0; i < 4; ++i)
                    {
                        if (GameObject* moguFoutain = GameObject::GetGameObject(*me, pInstance->GetData64(firstMoguFountain)))
                            moguFoutain->SetGoState(GO_STATE_READY);

                        firstMoguFountain++;
                    }
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

            void JustReachedHome()
            {
                if (pInstance)
                {
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, FAIL);
                }
            }

            void KilledUnit(Unit* who)
            {
                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* attacker)
            {
                Talk(TALK_AGGRO);

                if (pInstance)
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, IN_PROGRESS);

                events.ScheduleEvent(EVENT_ENRAGE, 540 * IN_MILLISECONDS); // 9 min
                events.ScheduleEvent(EVENT_STATIC_BURST, 13000);
                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 8000);
                events.ScheduleEvent(EVENT_THUNDERING_THROW, 30000);
                events.ScheduleEvent(EVENT_LIGHTNING_STORM, 90000);
            }

            void JustDied(Unit* killer)
            {
                Talk(TALK_DEATH);

                summons.DespawnAll();
                DespawnAllVisuals();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, DONE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STATIC_WOUND);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONTROL_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_STUN_PLAYER);
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

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_ENRAGE:
                        Talk(TALK_ENRAGE);
                        me->CastSpell(me, SPELL_ENRAGE, true);
                        break;
                    case EVENT_STATIC_BURST:
                        Talk(TALK_STATIC_BURST);
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_STATIC_BURST, false);
                        events.ScheduleEvent(EVENT_STATIC_BURST, 19000);
                        break;
                    case EVENT_FOCUSED_LIGHTNING:
                        Talk(TALK_FOCUSED_LIGHTNING);
                        me->CastSpell(me, SPELL_FOCUSED_LIGHTNING, false);
                        events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, urand(10000, 18000));
                        break;
                    case EVENT_THUNDERING_THROW:
                    {
                        Talk(TALK_THUNDERING_THROW);

                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 30.0f, true))
                        {
                            me->CastSpell(target, SPELL_THUNDERING_THROW_VEHICLE, true);
                            me->CastSpell(target, SPELL_CONTROL_VEHICLE, true);
                            target->ToPlayer()->EnterVehicle(me, -1);
                            target->getThreatManager().resetAllAggro();
                        }

                        events.ScheduleEvent(EVENT_EJECT_PLAYER, 2000);
                        events.ScheduleEvent(EVENT_THUNDERING_THROW, 90000);
                        break;
                    }
                    case EVENT_EJECT_PLAYER:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 30.0f, true))
                            me->CastSpell(target, SPELL_THUNDERING_THROW_EJECT_PLAYER, false);
                        break;
                    case EVENT_LIGHTNING_STORM:
                        Talk(TALK_LIGHTNING_STORM);
                        me->CastSpell(me, SPELL_LIGHTNING_STORM, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_STORM, 90000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (who->GetDistance(me) >= 50.0f)
                    return;

                if (introDone)
                    return;

                introDone = true;
                Talk(TALK_INTRO);
            }

            void DespawnAllVisuals()
            {
                std::list<Creature*> visualList;
                me->GetCreatureListWithEntryInGrid(visualList, NPC_LIGHTNING_FISSURE, 200.0f);

                for (auto itr : visualList)
                    itr->DespawnOrUnsummon();

                me->GetCreatureListWithEntryInGrid(visualList, NPC_CONDUCTIVE_WATER, 200.0f);

                for (auto itr : visualList)
                    itr->DespawnOrUnsummon();

                me->GetCreatureListWithEntryInGrid(visualList, NPC_STATUE, 200.0f);

                for (auto itr : visualList)
                    itr->RemoveAura(SPELL_CONDUCTIVE_WATER_FOUNTAIN);
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_PHASE:
                        return uint32(statuesPhase);
                    default:
                        break;
                }

                return 0;
            }

            void SetData(uint32 type, uint32 value)
            {
                if (type == DATA_PHASE)
                    statuesPhase = value;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_jin_rokh_the_breakerAI(creature);
        }
};

// Focused Lightning - 69593
class mob_focused_lightning : public CreatureScript
{
    public:
        mob_focused_lightning() : CreatureScript("mob_focused_lightning") { }

        struct mob_focused_lightningAI : public ScriptedAI
        {
            mob_focused_lightningAI(Creature* creature) : ScriptedAI(creature) { }

            uint64 focusedGuid;

            void Reset()
            {
                focusedGuid = 0;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_VISUAL, true);
                me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_PERIODIC, true);
            }

            void SetGUID(uint64 guid, int32 id /*= 0*/)
            {
                focusedGuid = guid;

                if (Player* target = Player::GetPlayer(*me, focusedGuid))
                    me->GetMotionMaster()->MoveFollow(target, 0.0f, me->GetOrientation());
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* target = Player::GetPlayer(*me, focusedGuid))
                {
                    if (me->GetDistance(target) <= 0.5f)
                    {
                        me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_DETONATION, true);
                        me->CastSpell(me, SPELL_LIGHTNING_FISSURE_SUMMON, true);
                        me->DespawnOrUnsummon();

                        std::list<Creature*> conductiveWaters;
                        me->GetCreatureListWithEntryInGrid(conductiveWaters, NPC_CONDUCTIVE_WATER, 200.0f);

                        for (auto itr : conductiveWaters)
                        {
                            if (itr->GetDistance(me) <= 30.0f)
                                itr->CastSpell(itr, SPELL_FOCUSED_LIGHTNING_CONDUCTION, true);
                        }
                    }
                    else
                        me->GetMotionMaster()->MoveFollow(target, 0.0f, me->GetOrientation());
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_focused_lightningAI(creature);
        }
};

// Lightning Fissure - 69609
class mob_lightning_fissure : public CreatureScript
{
    public:
        mob_lightning_fissure() : CreatureScript("mob_lightning_fissure") { }

        struct mob_lightning_fissureAI : public ScriptedAI
        {
            mob_lightning_fissureAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);

                me->CastSpell(me, SPELL_LIGHTNING_FISSURE_VISUAL, true);
                me->CastSpell(me, SPELL_LIGHTNING_FISSURE_PERIODIC, true);
            }

            void UpdateAI(const uint32 diff)
            {
                std::list<Creature*> conductiveWaters;
                me->GetCreatureListWithEntryInGrid(conductiveWaters, NPC_CONDUCTIVE_WATER, 200.0f);

                bool found = false;
                for (auto itr : conductiveWaters)
                {
                    float scale = itr->GetFloatValue(OBJECT_FIELD_SCALE_X);
                    float distToCheck = 30.0f * scale / 2.0f;
                    if (itr->GetDistance(me) <= distToCheck)
                        itr->CastSpell(itr, SPELL_LIGHTNING_FISSURE_CONDUCTION, true);

                    found = true;
                }

                if (found)
                    me->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lightning_fissureAI(creature);
        }
};

// Statue - 69467
class mob_statue : public CreatureScript
{
    public:
        mob_statue() : CreatureScript("mob_statue") { }

        struct mob_statueAI : public ScriptedAI
        {
            mob_statueAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            uint64 playerGuid;
            bool returned;
            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                playerGuid  = 0;
                returned    = false;

                events.Reset();
            }

            void SetGUID(uint64 guid, int32 type)
            {
                playerGuid = guid;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!pInstance)
                    return;

                Creature* jinRokh = Creature::GetCreature(*me, pInstance->GetData64(NPC_JIN_ROKH_THE_BREAKER));
                if (!jinRokh)
                    return;

                uint64 statue = jinRokh->AI()->GetData(DATA_PHASE);
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SPAWN_CONDUCTIVE_WATER:
                        me->CastSpell(conductiveWatersPos[statue].GetPositionX(), conductiveWatersPos[statue].GetPositionY(),
                                          conductiveWatersPos[statue].GetPositionZ(), SPELL_CONDUCTIVE_WATER_SUMMON, true);
                        jinRokh->AI()->SetData(DATA_PHASE, statue + 1);
                        break;
                    default:
                        break;
                }

                if (returned)
                    return;

                if (Player* player = Player::GetPlayer(*me, playerGuid))
                {
                    if (player->GetDistance(me) <= 1.0f)
                    {
                        me->CastSpell(player, SPELL_THUNDERING_THROW_DAMAGE, true);
                        player->RemoveAura(SPELL_THUNDERING_THROW_VEHICLE);
                            
                        GameObject* moguFountain = NULL;

                        switch (statue)
                        {
                            case PHASE_NONE:
                                player->CastSpell(conductiveWatersPos[0].GetPositionX(), conductiveWatersPos[0].GetPositionY(),
                                                conductiveWatersPos[0].GetPositionZ(), SPELL_THUNDERING_THROW_JUMP_DEST, true);

                                moguFountain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_NW));
                                break;
                            case PHASE_STATUE_NW:
                                player->CastSpell(conductiveWatersPos[1].GetPositionX(), conductiveWatersPos[1].GetPositionY(),
                                                conductiveWatersPos[1].GetPositionZ(), SPELL_THUNDERING_THROW_JUMP_DEST, true);

                                moguFountain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_SW));
                                break;
                            case PHASE_STATUE_SW:
                                player->CastSpell(conductiveWatersPos[2].GetPositionX(), conductiveWatersPos[2].GetPositionY(),
                                                conductiveWatersPos[2].GetPositionZ(), SPELL_THUNDERING_THROW_JUMP_DEST, true);

                                moguFountain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_NE));
                                break;
                            case PHASE_STATUE_NE:
                                player->CastSpell(conductiveWatersPos[3].GetPositionX(), conductiveWatersPos[3].GetPositionY(),
                                                conductiveWatersPos[3].GetPositionZ(), SPELL_THUNDERING_THROW_JUMP_DEST, true);

                                moguFountain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_SE));
                                break;
                            default:
                                break;
                        }

                        returned = true;

                        if (moguFountain)
                            moguFountain->SetGoState(GO_STATE_ACTIVE);

                        events.ScheduleEvent(EVENT_SPAWN_CONDUCTIVE_WATER, 4000);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_statueAI(creature);
        }
};

// Conductive Water - 69469
class mob_conductive_water : public CreatureScript
{
    public:
        mob_conductive_water() : CreatureScript("mob_conductive_water") { }

        struct mob_conductive_waterAI : public ScriptedAI
        {
            mob_conductive_waterAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                events.Reset();

                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->CastSpell(me, SPELL_CONDUCTIVE_WATER_VISUAL, true);
                summoner->CastSpell(summoner, SPELL_CONDUCTIVE_WATER_FOUNTAIN, true);
                me->CastSpell(me, SPELL_CONDUCTIVE_WATER_GROW_SCALE, true);

                events.ScheduleEvent(EVENT_CONDUCTIVE_WATER_DAMAGE_TAKEN, 1000);
                events.ScheduleEvent(EVENT_STOP_FOUNTAINS, 12000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CONDUCTIVE_WATER_DAMAGE_TAKEN:
                        me->CastSpell(me, SPELL_CONDUCTIVE_WATER_DAMAGE_TAKEN, true);
                        me->CastSpell(me, SPELL_CONDUCTIVE_WATER_FLUIDITY, true);
                        events.ScheduleEvent(EVENT_CONDUCTIVE_WATER_DAMAGE_TAKEN, 1000);
                        break;
                    case EVENT_STOP_FOUNTAINS:
                    {
                        std::list<Creature*> statueList;
                        me->GetCreatureListWithEntryInGrid(statueList, NPC_STATUE, 200.0f);

                        for (auto itr : statueList)
                            itr->RemoveAura(SPELL_CONDUCTIVE_WATER_FOUNTAIN);

                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_conductive_waterAI(creature);
        }
};

// Call Da Storm Stalker - 69676
class mob_call_da_storm_stalker : public CreatureScript
{
    public:
        mob_call_da_storm_stalker() : CreatureScript("mob_call_da_storm_stalker") { }

        struct mob_call_da_storm_stalkerAI : public ScriptedAI
        {
            mob_call_da_storm_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner)
            {
                me->CastSpell(me, SPELL_LIGHTNING_STORM_SPAWN_EFFECT, false);

                if (urand(0, 1))
                    me->CastSpell(me, SPELL_LIGHTNING_STORM_SMALL, true);
                else
                    me->CastSpell(me, SPELL_LIGHTNING_STORM_BIG, true);

                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_call_da_storm_stalkerAI(creature);
        }
};

// Static Burst - 137162
class spell_static_burst : public SpellScriptLoader
{
    public:
        spell_static_burst() : SpellScriptLoader("spell_static_burst") { }

        class spell_static_burst_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_static_burst_AuraScript);

            void OnPeriodic(constAuraEffectPtr aurEff)
            {
                if (Unit* target = GetTarget())
                {
                    if (Unit* caster = GetCaster())
                    {
                        if (AuraPtr staticWound = caster->AddAura(SPELL_STATIC_WOUND, target))
                            staticWound->ModStackAmount(9);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_static_burst_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_static_burst_AuraScript();
        }
};

// Static Wound - 138349
class spell_static_wound : public SpellScriptLoader
{
    public:
        spell_static_wound() : SpellScriptLoader("spell_static_wound") { }

        class spell_static_wound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_static_wound_AuraScript);

            void OnPeriodic(constAuraEffectPtr aurEff)
            {
                if (AuraPtr staticWound = aurEff->GetBase())
                    staticWound->ModStackAmount(-1);
            }

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (!eventInfo.GetActor() || eventInfo.GetActor()->GetEntry() != NPC_JIN_ROKH_THE_BREAKER)
                    return;

                if (Unit* target = GetTarget())
                {
                    int32 basePoints = aurEff->GetAmount();
                    target->CastCustomSpell(target, SPELL_STATIC_WOUND_DAMAGE, &basePoints, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_static_wound_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectProc += AuraEffectProcFn(spell_static_wound_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_static_wound_AuraScript();
        }
};

// Static Wound (Damages) - 138389
class spell_static_wound_damage : public SpellScriptLoader
{
    public:
        spell_static_wound_damage() : SpellScriptLoader("spell_static_wound_damage") { }

        class spell_static_wound_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_static_wound_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target != caster)
                            SetHitDamage(GetHitDamage() / 3);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_static_wound_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_static_wound_damage_SpellScript();
        }
};

// Focused Lightning - 137194
class spell_focused_lightning : public SpellScriptLoader
{
    public:
        spell_focused_lightning() : SpellScriptLoader("spell_focused_lightning") { }

        class spell_focused_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_focused_lightning_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        Position pos;
                        caster->GetPosition(&pos);
                    
                        if (TempSummon* summon = caster->SummonCreature(NPC_FOCUSED_LIGHTNING, pos))
                            summon->AI()->SetGUID(target->GetGUID(), 0);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_focused_lightning_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_focused_lightning_SpellScript();
        }
};

// Focused Lightning - 137437
class spell_focused_lightning_periodic : public SpellScriptLoader
{
    public:
        spell_focused_lightning_periodic() : SpellScriptLoader("spell_focused_lightning_periodic") { }

        class spell_focused_lightning_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_focused_lightning_periodic_AuraScript);

            void OnPeriodic(constAuraEffectPtr aurEff)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_FOCUSED_LIGHTNING_AREA, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_focused_lightning_periodic_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_focused_lightning_periodic_AuraScript();
        }
};

// Focused Lightning - 137429
class spell_focused_lightning_damage : public SpellScriptLoader
{
    public:
        spell_focused_lightning_damage() : SpellScriptLoader("spell_focused_lightning_damage") { }

        class spell_focused_lightning_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_focused_lightning_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_FOCUSED_LIGHTNING_DAMAGE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_focused_lightning_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_focused_lightning_damage_SpellScript();
        }
};

// Thundering Throw (Eject Player) - 137180
class spell_thundering_throw_eject : public SpellScriptLoader
{
    public:
        spell_thundering_throw_eject() : SpellScriptLoader("spell_thundering_throw_eject") { }

        class spell_thundering_throw_eject_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_thundering_throw_eject_SpellScript);

            void HandleOnHit()
            {
                if (!GetCaster())
                    return;

                if (Creature* jinRokh = GetCaster()->ToCreature())
                {
                    uint64 statue = jinRokh->AI()->GetData(DATA_PHASE);
                    if (!jinRokh->GetInstanceScript())
                        return;

                    InstanceScript* pInstance = jinRokh->GetInstanceScript();
                    if (Unit* target = GetHitUnit())
                    {
                        if (jinRokh->GetVehicleKit())
                            jinRokh->GetVehicleKit()->RemoveAllPassengers();

                        target->RemoveAura(SPELL_THUNDERING_THROW_STUN_PLAYER);
                        target->RemoveAura(SPELL_CONTROL_VEHICLE);

                        switch (statue)
                        {
                            case PHASE_NONE:
                                if (Creature* statue = Creature::GetCreature(*target, pInstance->GetData64(DATA_STATUE_0)))
                                {
                                    statue->AddAura(SPELL_THUNDERING_THROW_VEHICLE, statue);
                                    statue->AI()->SetGUID(target->GetGUID());
                                    target->CastSpell(statue, SPELL_THUNDERING_THROW_JUMP_DEST, true);
                                }
                                break;
                            case PHASE_STATUE_NW:
                                if (Creature* statue = Creature::GetCreature(*target, pInstance->GetData64(DATA_STATUE_1)))
                                {
                                    statue->AddAura(SPELL_THUNDERING_THROW_VEHICLE, statue);
                                    statue->AI()->SetGUID(target->GetGUID());
                                    target->CastSpell(statue, SPELL_THUNDERING_THROW_JUMP_DEST, true);
                                }
                                break;
                            case PHASE_STATUE_SW:
                                if (Creature* statue = Creature::GetCreature(*target, pInstance->GetData64(DATA_STATUE_2)))
                                {
                                    statue->AddAura(SPELL_THUNDERING_THROW_VEHICLE, statue);
                                    statue->AI()->SetGUID(target->GetGUID());
                                    target->CastSpell(statue, SPELL_THUNDERING_THROW_JUMP_DEST, true);
                                }
                                break;
                            case PHASE_STATUE_NE:
                                if (Creature* statue = Creature::GetCreature(*target, pInstance->GetData64(DATA_STATUE_3)))
                                {
                                    statue->AddAura(SPELL_THUNDERING_THROW_VEHICLE, statue);
                                    statue->AI()->SetGUID(target->GetGUID());
                                    target->CastSpell(statue, SPELL_THUNDERING_THROW_JUMP_DEST, true);
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_thundering_throw_eject_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_thundering_throw_eject_SpellScript();
        }
};

// Lightning Storm (Periodic) - 137313
class spell_lightning_storm_periodic : public SpellScriptLoader
{
    public:
        spell_lightning_storm_periodic() : SpellScriptLoader("spell_lightning_storm_periodic") { }

        class spell_lightning_storm_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lightning_storm_periodic_AuraScript);

            void OnPeriodic(constAuraEffectPtr aurEff)
            {
                if (Unit* target = GetTarget())
                {
                    target->CastSpell(target, SPELL_LIGHTNING_STORM_SUMMON, true);
                    target->CastSpell(target, SPELL_LIGHTNING_STORM_SUMMON, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lightning_storm_periodic_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lightning_storm_periodic_AuraScript();
        }
};

void AddSC_boss_jin_rokh_the_breaker()
{
    new boss_jin_rokh_the_breaker();
    new mob_focused_lightning();
    new mob_lightning_fissure();
    new mob_statue();
    new mob_conductive_water();
    new mob_call_da_storm_stalker();
    new spell_static_burst();
    new spell_static_wound();
    new spell_static_wound_damage();
    new spell_focused_lightning();
    new spell_focused_lightning_periodic();
    new spell_focused_lightning_damage();
    new spell_thundering_throw_eject();
    new spell_lightning_storm_periodic();
}
