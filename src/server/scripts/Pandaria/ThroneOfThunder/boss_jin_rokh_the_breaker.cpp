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
    SPELL_STATIC_BURST                          = 137162,
    SPELL_STATIC_WOUND                          = 138349,
    SPELL_STATIC_WOUND_DAMAGE                   = 138389,

    SPELL_FOCUSED_LIGHTNING                     = 137399,
    SPELL_FOCUSED_LIGHTNING_VISUAL              = 139233,
    SPELL_FOCUSED_LIGHTNING_PERIODIC            = 137389,
    SPELL_FOCUSED_LIGHTNING_AREA                = 137429,
    SPELL_FOCUSED_LIGHTNING_DAMAGE              = 137423,
    SPELL_FOCUSED_LIGHTNING_DETONATION          = 139211,
    SPELL_FOCUSED_LIGHTNING_EYES                = 137422,
    SPELL_FOCUSED_LIGHTNING_IMPLOSION           = 137507,

    SPELL_LIGHTNING_FISSURE_SUMMON              = 137479,
    SPELL_LIGHTNING_FISSURE_VISUAL              = 137480,
    SPELL_LIGHTNING_FISSURE_PERIODIC            = 137484,

    // In EffectMovementGenerator::MovementInform(Unit &unit)
    SPELL_THUNDERING_THROW_AOE                  = 137167,
    SPELL_THUNDERING_THROW_STUN_PLAYER          = 137371,

    SPELL_CONTROL_VEHICLE                       = 43671,
    SPELL_THUNDERING_THROW_VEHICLE              = 137161,
    SPELL_THUNDERING_THROW_DAMAGE               = 137370,
    SPELL_THUNDERING_THROW_EJECT_PLAYER         = 137180,
    SPELL_THUNDERING_THROW_JUMP_DEST            = 137173,

    SPELL_CONDUCTIVE_WATER_SUMMON               = 137145,
    SPELL_CONDUCTIVE_WATER_VISUAL               = 137277,
    SPELL_CONDUCTIVE_WATER_FOUNTAIN             = 137340,
    SPELL_CONDUCTIVE_WATER_GROW_SCALE           = 137676,
    SPELL_CONDUCTIVE_WATER_DAMAGE_TAKEN         = 138470,
    SPELL_CONDUCTIVE_WATER_FLUIDITY             = 138002,
    SPELL_CONDUCTIVE_WATER_ELECTRIFIED          = 137978,
    SPELL_CONDUCTIVE_WATER_LIGHT_VISUAL         = 138568,

    SPELL_ELECTRIFIED_WATERS_PERIODIC_DMG       = 138006,
    SPELL_FOCUSED_LIGHTNING_VIOLENT_DETONATION  = 138990,

    SPELL_FOCUSED_LIGHTNING_CONDUCTION          = 137530,
    SPELL_LIGHTNING_FISSURE_CONDUCTION          = 138133,

    SPELL_LIGHTNING_STORM                       = 137313,
    SPELL_LIGHTNING_STORM_SUMMON                = 137283,
    SPELL_LIGHTNING_STORM_SPAWN_EFFECT          = 137260,
    SPELL_LIGHTNING_STORM_SMALL                 = 140811,
    SPELL_LIGHTNING_STORM_BIG                   = 140555,
    SPELL_LIGHTNING_STORM_SMALL_FLOOR           = 140819,

    // Heroic mode
    SPELL_IONIZATION                            = 138732,
    SPELL_IONIZATION_DAMAGE                     = 138733,
    SPELL_IONIZATION_CONDUCTION                 = 138743,
    SPELL_LIGHTNING_STRIKE_PILLAR_VISUAL        = 138299,
    SPELL_LIGHTNING_STRIKE_SUMMON               = 138012,
    SPELL_LIGHTNING_STRIKE_DAMAGE               = 137647,
    SPELL_LIGHTNING_DIFFUSION                   = 137905,

    SPELL_JINROKH_THE_BREAKER_BONUS             = 139674
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
    EVENT_LIGHTNING_STORM               = 9,
    EVENT_CHECK_LIGHTNING_FISSURES      = 10,

    // Heroic mode
    EVENT_IONIZATION                    = 11,
    EVENT_SPAWN_LIGHTNING_SPARKS        = 12,
    EVENT_LIGHTNING_DIFFUSION           = 13
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
    ACTION_ELECTRIFY_CONDUCTIVE_WATERS,
    ACTION_SPAWN_LIGHTNING_PILLARS
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
            uint32 electrifyTimer;

            void Reset()
            {
                statuesPhase    = PHASE_NONE;
                electrifyTimer  = 0;

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
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, NOT_STARTED);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STATIC_WOUND);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONTROL_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_STUN_PLAYER);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IONIZATION);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FOCUSED_LIGHTNING_IMPLOSION);

                    for (uint32 i = 0; i < 4; ++i)
                    {
                        switch (i)
                        {
                            case 0:
                                if (GameObject* moguFoutain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_NW)))
                                    moguFoutain->SetGoState(GO_STATE_READY);
                                break;
                            case 1:
                                if (GameObject* moguFoutain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_SW)))
                                    moguFoutain->SetGoState(GO_STATE_READY);
                                break;
                            case 2:
                                if (GameObject* moguFoutain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_NE)))
                                    moguFoutain->SetGoState(GO_STATE_READY);
                                break;
                            case 3:
                                if (GameObject* moguFoutain = GameObject::GetGameObject(*me, pInstance->GetData64(GOB_MOGU_FOUNTAIN_SE)))
                                    moguFoutain->SetGoState(GO_STATE_READY);
                                break;
                            default:
                                break;
                        }
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
                {
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, IN_PROGRESS);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }

                _EnterCombat();
                events.ScheduleEvent(EVENT_ENRAGE, 540 * IN_MILLISECONDS); // 9 min
                events.ScheduleEvent(EVENT_STATIC_BURST, 13000);
                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 8000);
                events.ScheduleEvent(EVENT_THUNDERING_THROW, 30000);
                events.ScheduleEvent(EVENT_LIGHTNING_STORM, 90000);

                if (IsHeroic())
                    events.ScheduleEvent(EVENT_IONIZATION, 60000);
            }

            void JustDied(Unit* killer)
            {
                Talk(TALK_DEATH);

                _JustDied();

                summons.DespawnAll();
                DespawnAllVisuals();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_JIN_ROKH_THE_BREAKER, DONE);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STATIC_WOUND);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONTROL_VEHICLE);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_THUNDERING_THROW_STUN_PLAYER);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_IONIZATION);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FOCUSED_LIGHTNING_IMPLOSION);
                }

                Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                {
                    if (Player* l_Player = l_Itr->getSource())
                        me->CastSpell(l_Player, SPELL_JINROKH_THE_BREAKER_BONUS, true);
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootDistribution(me, l_Player->GetGroup());
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ELECTRIFY_CONDUCTIVE_WATERS:
                    {
                        std::list<Creature*> waterList;
                        me->GetCreatureListWithEntryInGrid(waterList, NPC_CONDUCTIVE_WATER, 200.0f);

                        for (auto itr : waterList)
                        {
                            if (itr->HasAura(SPELL_CONDUCTIVE_WATER_VISUAL))
                            {
                                itr->RemoveAura(SPELL_CONDUCTIVE_WATER_VISUAL);
                                itr->AddAura(SPELL_CONDUCTIVE_WATER_ELECTRIFIED, itr);
                            }
                        }
                        break;
                    }
                    case ACTION_SPAWN_LIGHTNING_PILLARS:
                    {
                        if (!IsHeroic())
                            break;

                        float rotation = frand(0, 2 * M_PI);
                        float x = me->GetPositionX() + (frand(5.0f, 35.0f) * cos(rotation));
                        float y = me->GetPositionY() + (frand(5.0f, 35.0f) * sin(rotation));

                        me->SummonCreature(NPC_LIGHTNING_PILLER_STALKER, x, y, me->GetPositionZ());
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                    {
                        me->CombatStop();
                        EnterEvadeMode();
                    }

                    return;
                }

                if (electrifyTimer)
                {
                    if (electrifyTimer <= diff)
                    {
                        DoAction(ACTION_ELECTRIFY_CONDUCTIVE_WATERS);
                        electrifyTimer = 0;
                    }
                    else
                        electrifyTimer -= diff;
                }

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
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_FOCUSED_LIGHTNING, false);
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
                        electrifyTimer = 10000;
                        events.ScheduleEvent(EVENT_LIGHTNING_STORM, 90000);
                        break;
                    case EVENT_IONIZATION:
                        me->CastSpell(me, SPELL_IONIZATION, false);
                        events.ScheduleEvent(EVENT_IONIZATION, 90000);
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

                visualList.clear();
                me->GetCreatureListWithEntryInGrid(visualList, NPC_CONDUCTIVE_WATER, 200.0f);
                for (auto itr : visualList)
                    itr->DespawnOrUnsummon();

                visualList.clear();
                me->GetCreatureListWithEntryInGrid(visualList, NPC_STATUE, 200.0f);
                for (auto itr : visualList)
                    itr->RemoveAura(SPELL_CONDUCTIVE_WATER_FOUNTAIN);

                visualList.clear();
                me->GetCreatureListWithEntryInGrid(visualList, NPC_LIGHTNING_SPARK, 200.0f);
                for (auto itr : visualList)
                    itr->DespawnOrUnsummon();

                visualList.clear();
                me->GetCreatureListWithEntryInGrid(visualList, NPC_LIGHTNING_PILLER_STALKER, 200.0f);
                for (auto itr : visualList)
                    itr->DespawnOrUnsummon();
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
            bool exploded;
            EventMap events;

            void Reset()
            {
                focusedGuid = 0;
                exploded = false;

                events.Reset();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);

                me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_VISUAL, true);
                me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_PERIODIC, true);
            }

            void SetGUID(uint64 guid, int32 id /*= 0*/)
            {
                focusedGuid = guid;

                if (Player* target = Player::GetPlayer(*me, focusedGuid))
                {
                    me->CastSpell(target, SPELL_FOCUSED_LIGHTNING_EYES, true);
                    me->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);

                    if (target && me->Attack(target, true))
                    {
                        me->GetMotionMaster()->MoveChase(target, 0.5f);
                        events.ScheduleEvent(EVENT_CHECK_LIGHTNING_FISSURES, 100);
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (exploded)
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHECK_LIGHTNING_FISSURES:
                    {
                        if (Creature* fissure = GetClosestCreatureWithEntry(me, NPC_LIGHTNING_FISSURE, 1.0f))
                        {
                            exploded = true;
                            fissure->CastSpell(fissure, SPELL_FOCUSED_LIGHTNING_IMPLOSION, true);
                            me->DespawnOrUnsummon();
                            fissure->DespawnOrUnsummon(50);
                            return;
                        }

                        events.ScheduleEvent(EVENT_CHECK_LIGHTNING_FISSURES, 100);
                    }
                    default:
                        break;
                }

                if (Player* target = Player::GetPlayer(*me, focusedGuid))
                {
                    if (me->GetDistance(target) <= 1.0f)
                    {
                        me->CastSpell(me, SPELL_LIGHTNING_FISSURE_SUMMON, true);
                        me->DespawnOrUnsummon(50);
                        exploded = true;

                        std::list<Creature*> conductiveWaters;
                        me->GetCreatureListWithEntryInGrid(conductiveWaters, NPC_CONDUCTIVE_WATER, 200.0f);

                        for (auto itr : conductiveWaters)
                        {
                            float scale = itr->GetFloatValue(OBJECT_FIELD_SCALE);
                            float distToCheck = 30.0f * (scale / 12.0f);
                            if (itr->GetDistance(me) <= distToCheck)
                            {
                                if (itr->HasAura(SPELL_CONDUCTIVE_WATER_ELECTRIFIED))
                                    itr->CastSpell(itr, SPELL_FOCUSED_LIGHTNING_VIOLENT_DETONATION, true);
                                else
                                {
                                    itr->CastSpell(itr, SPELL_FOCUSED_LIGHTNING_CONDUCTION, true);
                                    me->CastSpell(target, SPELL_FOCUSED_LIGHTNING_DETONATION, true);
                                }
                            }
                        }
                    }
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
                    float scale = itr->GetFloatValue(OBJECT_FIELD_SCALE);
                    float distToCheck = 30.0f * (scale / 12.0f);

                    if (itr->GetDistance(me) <= distToCheck)
                    {
                        itr->CastSpell(itr, SPELL_LIGHTNING_FISSURE_CONDUCTION, true);
                        found = true;
                    }
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

                me->SetReactState(REACT_PASSIVE);
                me->SetCanFly(true);
                me->SetDisableGravity(true);

                events.Reset();
            }

            void SetGUID(uint64 guid, int32 type)
            {
                returned = false;
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
                        if (!me->HasAura(SPELL_CONDUCTIVE_WATER_ELECTRIFIED))
                            me->CastSpell(me, SPELL_CONDUCTIVE_WATER_FLUIDITY, true);
                        else
                            me->CastSpell(me, SPELL_ELECTRIFIED_WATERS_PERIODIC_DMG, true);

                        me->CastSpell(me, SPELL_CONDUCTIVE_WATER_DAMAGE_TAKEN, true);
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

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_call_da_storm_stalkerAI(creature);
        }
};

// Lightning Spark - 69635
class mob_lightning_spark : public CreatureScript
{
    public:
        mob_lightning_spark() : CreatureScript("mob_lightning_spark") { }

        struct mob_lightning_sparkAI : public ScriptedAI
        {
            mob_lightning_sparkAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                events.ScheduleEvent(EVENT_LIGHTNING_DIFFUSION, 500);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_LIGHTNING_DIFFUSION:
                        me->CastSpell(me, SPELL_LIGHTNING_DIFFUSION, true);
                        events.ScheduleEvent(EVENT_LIGHTNING_DIFFUSION, 500);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lightning_sparkAI(creature);
        }
};

// Lightning Pillar Spark Stalker - 69813
class mob_lightning_pillar_stalker : public CreatureScript
{
    public:
        mob_lightning_pillar_stalker() : CreatureScript("mob_lightning_pillar_stalker") { }

        struct mob_lightning_pillar_stalkerAI : public ScriptedAI
        {
            mob_lightning_pillar_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                events.Reset();

                me->CastSpell(me, SPELL_LIGHTNING_STRIKE_PILLAR_VISUAL, true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                events.ScheduleEvent(EVENT_SPAWN_LIGHTNING_SPARKS, 4000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SPAWN_LIGHTNING_SPARKS:
                    {
                        me->CastSpell(me, SPELL_LIGHTNING_STRIKE_DAMAGE, true);

                        float orientation = frand(0, M_PI * 2);
                        for (uint8 i = 0; i < 4; ++i)
                        {
                            if (Creature* spark = me->SummonCreature(NPC_LIGHTNING_SPARK, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), orientation, TEMPSUMMON_TIMED_DESPAWN, 10000))
                            {
                                float x = spark->GetPositionX() + (150.0f * cos(orientation));
                                float y = spark->GetPositionY() + (150.0f * sin(orientation));

                                spark->ClearUnitState(UNIT_STATE_STUNNED | UNIT_STATE_CASTING);
                                spark->GetMotionMaster()->MovePoint(1, x, y, spark->GetPositionZ());
                            }

                            orientation += (M_PI * 2) / 4;
                        }

                        me->DespawnOrUnsummon();
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lightning_pillar_stalkerAI(creature);
        }
};

// Static Burst - 137162
class spell_static_burst: public SpellScriptLoader
{
    public:
        spell_static_burst() : SpellScriptLoader("spell_static_burst") { }

        class spell_static_burst_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_static_burst_AuraScript);

            void OnPeriodic(AuraEffect const* aurEff)
            {
                if (Unit* target = GetTarget())
                {
                    if (Unit* caster = GetCaster())
                    {
                        if (Aura* staticWound = caster->AddAura(SPELL_STATIC_WOUND, target))
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
class spell_static_wound: public SpellScriptLoader
{
    public:
        spell_static_wound() : SpellScriptLoader("spell_static_wound") { }

        class spell_static_wound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_static_wound_AuraScript);

            void OnPeriodic(AuraEffect const* aurEff)
            {
                if (Aura* staticWound = aurEff->GetBase())
                    staticWound->ModStackAmount(-1);
            }

            void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
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
class spell_static_wound_damage: public SpellScriptLoader
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

// Focused Lightning - 137399
class spell_focused_lightning: public SpellScriptLoader
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
class spell_focused_lightning_periodic: public SpellScriptLoader
{
    public:
        spell_focused_lightning_periodic() : SpellScriptLoader("spell_focused_lightning_periodic") { }

        class spell_focused_lightning_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_focused_lightning_periodic_AuraScript);

            void OnPeriodic(AuraEffect const* aurEff)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_FOCUSED_LIGHTNING_AREA, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_focused_lightning_periodic_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_MOD_INCREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_focused_lightning_periodic_AuraScript();
        }
};

// Focused Lightning - 137429
class spell_focused_lightning_damage: public SpellScriptLoader
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
class spell_thundering_throw_eject: public SpellScriptLoader
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
class spell_lightning_storm_periodic: public SpellScriptLoader
{
    public:
        spell_lightning_storm_periodic() : SpellScriptLoader("spell_lightning_storm_periodic") { }

        class spell_lightning_storm_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lightning_storm_periodic_AuraScript);

            void OnPeriodic(AuraEffect const* aurEff)
            {
                if (Unit* target = GetTarget())
                {
                    target->CastSpell(target, SPELL_LIGHTNING_STORM_SUMMON, true);
                    target->CastSpell(target, SPELL_LIGHTNING_STORM_SUMMON, true);
                    target->CastSpell(target, SPELL_LIGHTNING_STORM_SUMMON, true);
                    target->CastSpell(target, SPELL_LIGHTNING_STORM_SMALL_FLOOR, true);
                    target->ToCreature()->AI()->DoAction(ACTION_SPAWN_LIGHTNING_PILLARS);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lightning_storm_periodic_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lightning_storm_periodic_AuraScript();
        }
};

// Ionization - 138732
class spell_ionization: public SpellScriptLoader
{
    public:
        spell_ionization() : SpellScriptLoader("spell_ionization") { }

        class spell_ionization_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ionization_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                std::list<WorldObject*> targetsToRemove;
                for (auto target : targets)
                {
                    if (!target->ToPlayer())
                        continue;

                    Player* plr = target->ToPlayer();
                    if (plr->GetRoleForGroup(plr->GetSpecializationId(plr->GetActiveSpec())) == ROLE_TANK)
                        targetsToRemove.push_back(target);
                }

                for (auto itr : targetsToRemove)
                    targets.remove(itr);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ionization_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ionization_SpellScript();
        }

        class spell_ionization_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ionization_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        int32 basepoints = aurEff->GetAmount();
                        caster->CastCustomSpell(target, SPELL_IONIZATION_DAMAGE, &basepoints, NULL, NULL, true);

                        if (target->FindNearestCreature(NPC_CONDUCTIVE_WATER, 30.0f))
                            target->CastCustomSpell(target, SPELL_IONIZATION_CONDUCTION, &basepoints, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_ionization_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ionization_AuraScript();
        }
};

// Ionization Conduction - 138743
// Focused Lightning Conduction - 137530
// Lightning Fissure Conduction - 138133
class spell_ionization_conduction: public SpellScriptLoader
{
    public:
        spell_ionization_conduction() : SpellScriptLoader("spell_ionization_conduction") { }

        class spell_ionization_conduction_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ionization_conduction_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                std::list<WorldObject*> targetsToRemove;
                for (auto target : targets)
                {
                    if (!target->FindNearestCreature(NPC_CONDUCTIVE_WATER, 30.0f))
                        targetsToRemove.push_back(target);
                }

                for (auto itr : targetsToRemove)
                    targets.remove(itr);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case 138743:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ionization_conduction_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                    break;
                case 138133:
                case 137530:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ionization_conduction_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                    break;
                default:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ionization_conduction_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
                    break;
                }
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ionization_conduction_SpellScript();
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
    new mob_lightning_spark();
    new mob_lightning_pillar_stalker();
    new spell_static_burst();
    new spell_static_wound();
    new spell_static_wound_damage();
    new spell_focused_lightning();
    new spell_focused_lightning_periodic();
    new spell_focused_lightning_damage();
    new spell_thundering_throw_eject();
    new spell_lightning_storm_periodic();
    new spell_ionization();
    new spell_ionization_conduction();
}
