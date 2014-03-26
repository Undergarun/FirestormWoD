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

// TODO : Finish Heroic mode

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "heart_of_fear.h"

enum ePlatforms
{
    PLATFORM_ZORLOK_SW              = 1,    // Platform South-west (Force and verve)
    PLATFORM_ZORLOK_NE              = 2,    // Platform North-east (attenuation)
    PLATFORM_ZORLOK_SE              = 3,    // Platform South-east (Convert)
};

enum eTalk
{
    TALK_AGGRO                      = 1,
    TALK_DEATH                      = 2,
    TALK_EVENT_IDLE_1A              = 3,    // 1st phase of trash mobs
    TALK_EVENT_IDLE_1B              = 4,
    TALK_EVENT_IDLE_1C              = 5,
    TALK_EVENT_IDLE_2               = 6,    // 2nd phase of trash mobs
    TALK_EVENT_IDLE_3               = 7,    // 3rd phase of trash mobs
    TALK_EVENT_IDLE_4               = 8,    // 4th phase of trash mobs
    TALK_EVENT_PHASE_1              = 9,
    TALK_EVENT_PHASE_2              = 10,
    TALK_EVENT_PHASE_3              = 11,
    TALK_EVENT_TRASH_A_COMBAT       = 12,
    TALK_EVENT_TRASH_A_DIES         = 13,
    TALK_EVENT_TRASH_B_COMBAT       = 14,
    TALK_EVENT_TRASH_B_DIES         = 15,
    TALK_EVENT_TRASH_C_COMBAT       = 16,
    TALK_EVENT_TRASH_C_DIES         = 17,
    TALK_SLAY_01                    = 18,   // Killing a player
    TALK_SLAY_02                    = 19,
    TALK_EXHALE                     = 20,
    TALK_INHALE                     = 21,
    TALK_CONVERT                    = 22,
    TALK_PITCH                      = 23,   // Echoes of power
};

Position zorlokPlatforms[3] =
{
    {-2317.21f,     300.67f,    409.90f,    0.0f},  // SW Platform
    {-2234.36f,     216.88f,    409.90f,    0.0f},  // NE Platform
    {-2315.77f,     218.20f,    409.90f,    0.0f},  // SE Platform
};

Position zorlokReachPoints[3] = 
{
    {-2317.21f,     300.67f,    420.0f,     0.0f},  // NE Platform
    {-2234.36f,     216.88f,    420.0f,     0.0f},  // SW Platform
    {-2315.77f,     218.20f,    420.0f,     0.0f},  // SE Platform
};

Position oratiumCenter[2] =
{
    {-2274.80f,     259.19f,    420.0f,    0.318021f},
    {-2274.80f,     259.19f,    406.5f,    0.318021f}
};

float tabCenter[3] = {-2274.8f, 259.187f, 406.5f};

float rangeAttenuation1[2][2] =
{
    -2256.0f, -2208.0f,
      190.0f,   240.0f
};

float rangeAttenuation2[2][2] =
{
    // Coords to redone
    -2297.0f, -2250.0f,
      237.0f,   280.0f
};

// Zorlok - 62980
// Echo of Attenuation - 65173
// Echo of Force and Verve - 65174
class boss_zorlok : public CreatureScript
{
    public:
        boss_zorlok() : CreatureScript("boss_zorlok") { }

        struct boss_zorlokAI : public BossAI
        {
            boss_zorlokAI(Creature* creature) : BossAI(creature, DATA_ZORLOK)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            bool isActive;
            std::list<uint8> platforms;
            uint8 numPlat;
            uint8 phase;
            uint32 platformInUse;
            uint8 sonicSpirals;
            bool clocksideRings;
            bool isEcho;
            bool isAttEcho;
            bool isFaVEcho;
            
            void Reset()
            {
                events.Reset();
                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_ZORLOK);
                // Make sure we can target zorlok
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                // Set fying
                me->SetCanFly(true);
                me->SetSpeed(MOVE_FLIGHT, 4.5f);
                me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_HOVER);
                me->SetDisableGravity(true);
                me->SetReactState(REACT_PASSIVE);
                me->RemoveAllAreasTrigger();

                // Set Echo
                isAttEcho = me->GetEntry() == NPC_ECHO_OF_ATTENUATION;
                isFaVEcho = me->GetEntry() == NPC_ECHO_OF_FORCE_AND_VERVE;
                isEcho    = (isAttEcho || isFaVEcho);

                isActive = false;
                numPlat = 0;
                phase = 0;
                platformInUse = 0;
                sonicSpirals = 0;
                clocksideRings = true;

                platforms.clear();
                // In heroic mode, the platforms are ordered, so we just need to increase numPlat and having it matching to ePlatforms values, which
                // are heroic ordered, to get the right platform.
                //
                // In normal mode, platforms are picked in random order, so we need to put them in a list, to remove them when they're used (in order
                // to avoid Zorlok to use them again).
                if (!IsHeroic() && !isEcho)
                {
                    platforms.push_back(PLATFORM_ZORLOK_SW);
                    platforms.push_back(PLATFORM_ZORLOK_NE);
                    platforms.push_back(PLATFORM_ZORLOK_SE);
                }
            }

            void JustDied()
            {
                events.Reset();
                summons.DespawnAll();
                me->RemoveAllAreasTrigger();

                if (isEcho)
                    return;

                me->SetCanFly(false);
                me->SetDisableGravity(true);

                Talk(TALK_DEATH);

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_ZORLOK, DONE);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CONVERT);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FORCE_AND_VERVE);
                }
                _JustDied();

            }

            void JustReachedHome()
            {
                if (isEcho)
                    return;

                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_ZORLOK, FAIL);
            }

            void EnterCombat(Unit* attacker)
            {
                pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
            }

            void MoveInLineOfSight(Unit* attacker)
            {
                // Do nothing if not in attack phase (ie. flying), or if the unit beside isn't a player
                if (!me->HasReactState(REACT_AGGRESSIVE) || attacker->GetTypeId() != TYPEID_PLAYER)
                    return;

                // If is using Song of Empress, stop it
                if (me->HasAura(SPELL_SONG_OF_THE_EMPRESS))
                    me->RemoveAura(SPELL_SONG_OF_THE_EMPRESS);

                // Start attacking player
                me->AddThreat(attacker, 0.0f);
                AttackStart(attacker);
                me->SetInCombatWith(attacker);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_SLAY_01 + urand(0, 1));
            }

            uint32 ChoosePlatform()
            {
                // In Heroic mode, the platforms order is fixed
                if (IsHeroic())
                    return platformInUse = numPlat++;

                // Using a while loop considering it's possible none of the platforms could be selected due to the rand condition : if so, we need to redo
                // another loop(s) until a platform is picked.
                while (true)
                {
                    for (auto platform : platforms)
                    {
                        if (urand(0,1))
                        {
                            platforms.remove(platform);
                            ++numPlat;
                            return platformInUse = platform;
                        }
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if (isEcho)
                    return;

                if ((!me->HasReactState(REACT_AGGRESSIVE) && isActive) || (attacker->GetTypeId() != TYPEID_PLAYER))
                    return;

                if (!isActive)
                {
                    isActive = true;
                    if (!isEcho)
                    {
                        Talk(TALK_AGGRO);
                        me->CastSpell(tabCenter[0], tabCenter[1], tabCenter[2], SPELL_PHEROMONES_CLOUD, false);
                        phase = PHASE_ZORLOK1;
                        if (pInstance)
                            pInstance->SetBossState(DATA_ZORLOK, IN_PROGRESS);
                    }
                }

                if (me->HasAura(SPELL_SONG_OF_THE_EMPRESS))
                {
                    me->RemoveAurasDueToSpell(SPELL_SONG_OF_THE_EMPRESS);
                    AttackStart(attacker);
                    me->AddThreat(attacker, 0.0f);
                    me->SetInCombatWith(attacker);
                }

                if (phase == PHASE_ZORLOK2)
                    return;

                // Phase 1
                if (me->HealthBelowPctDamaged(100 - 20 * numPlat, damage))
                {
                    // No event while flying, and events of a platforms should be cancelled when using another platform
                    events.Reset();

                    // Switching platforms at 100%, 80% and 60% remaining life
                    if (numPlat < 3)
                    {
                        uint32 newPlatform = ChoosePlatform();
                        me->GetMotionMaster()->MoveTakeoff(newPlatform, zorlokReachPoints[newPlatform - 1]);
                        me->MonsterTextEmote("Imperial Vizier Zor'lok is flying to one of his platforms!", 0, true);
                    }
                    // At 40% remaining phase, switch on phase 2
                    else
                    {
                        phase = PHASE_ZORLOK2;
                        me->GetMotionMaster()->MoveTakeoff(phase, oratiumCenter[0]);
                    }

                    // Set Flying
                    me->SetDisableGravity(true);
                    me->SetCanFly(true);
                    // Stop attacking
                    me->SetReactState(REACT_PASSIVE);
                    me->AttackStop();
                    me->DeleteThreatList();
                    me->getThreatManager().resetAllAggro();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                }
            }

            void DoAction (int32 const action)
            {
                switch (action)
                {
                    case ACTION_SONIC_CARD:
                    {
                        if (sonicSpirals == 3)
                        {
                            sonicSpirals = 0;
                            clocksideRings = (urand(0, 1) ? true : false);
                        }
                        ++sonicSpirals;

                        me->SummonCreature(NPC_SONIC_RING_FORWARD,  me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_BACK,     me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_90LEFT,   me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_90RIGHT,  me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        
                        events.ScheduleEvent((clocksideRings ? EVENT_SUMMON_RINGS1 : EVENT_SUMMON_RINGS2), 1000);
                        break;
                    }
                    case ACTION_SONIC_QUAD1:
                    {
                        me->SummonCreature(NPC_SONIC_RING_30LEFT,   me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_150RIGHT, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_120LEFT,  me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_60RIGHT,  me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());

                        if (!clocksideRings)
                        {
                            if (sonicSpirals < 3)
                                events.ScheduleEvent(EVENT_SUMMON_RINGS, 1000);
                            else
                            {
                                me->SetReactState(REACT_AGGRESSIVE);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_STUNNED);
                                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_ENEMY_INTERACT);
                            }
                        }
                        else
                            events.ScheduleEvent(EVENT_SUMMON_RINGS2, 1000);
                        break;
                    }
                    case ACTION_SONIC_QUAD2:
                    {
                        me->SummonCreature(NPC_SONIC_RING_60LEFT,   me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_120RIGHT, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_150LEFT,  me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                        me->SummonCreature(NPC_SONIC_RING_30RIGHT,  me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());

                        if (clocksideRings)
                        {
                            if (sonicSpirals < 3)
                                events.ScheduleEvent(EVENT_SUMMON_RINGS, 1000);
                            else
                            {
                                me->SetReactState(REACT_AGGRESSIVE);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_STUNNED);
                                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_ENEMY_INTERACT);
                            }
                        }
                        else
                            events.ScheduleEvent(EVENT_SUMMON_RINGS1, 1000);

                        break;
                    }
                    case ACTION_INHALE_PHEROMONES:
                    {
                        me->RemoveAreaTrigger(SPELL_PHEROMONES_CLOUD);
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_OF_ZEAL);
                        me->GetMotionMaster()->MoveLand(PHASE_ZORLOK2 + 10,  oratiumCenter[1]);
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 ChooseAction()
            {
                uint32 choice = urand(1, 3);
                switch (choice)
                {
                    case 1:
                        return EVENT_ATTENUATION;
                    case 2:
                        return EVENT_CONVERT;
                    case 3:
                        return EVENT_FORCE_AND_VERVE;
                    default:
                        break;
                }
                return 0;
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (isEcho || id == 0 || type != EFFECT_MOTION_TYPE)
                    return;

                // General rule :
                // * if id < 10, the reach point is in the air,
                // * if id < 4, Zor'lok is going on a platform
                // * if id = 4, Zor'lok is going on the center of the room to absorb pheromones
                // * if id > 10, Zor'lok is landing from the air point to the ground

                // Phase 1 : going onto landing points
                if (id < 4)
                {
                    if (numPlat == 1)
                        Talk(TALK_EVENT_PHASE_1);
                    me->GetMotionMaster()->MoveLand(id + 10, zorlokPlatforms[id - 1]);
                    return;
                }

                // Switching from phase 1 to phase 2
                if (id == 4)
                {
                    me->CastSpell(me, SPELL_INHALE_PHEROMONES, true);
                    me->MonsterTextEmote("Imperial Vizier Zor'lok inhales Pheromones of Zeal!", 0, true);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_PHEROMONES_CLOUD);
                }

                // Landing
                if (id > 4 && me->HasReactState(REACT_PASSIVE))
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetDisableGravity(false);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    me->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_TRACK_UNIT |UNIT_DYNFLAG_SPECIALINFO);
                    events.ScheduleEvent(EVENT_INHALE, 15000);
                    events.ScheduleEvent(EVENT_SONG_EMPRESS, 1000);

                    switch (id)
                    {
                        // Force & Verve platform
                        case (PLATFORM_ZORLOK_SW + 10):
                        {
                            events.ScheduleEvent(EVENT_FORCE_AND_VERVE, urand(10000, 20000));
                            break;
                        }
                        // Attenuation Platform
                        case (PLATFORM_ZORLOK_NE + 10):
                        {
                            events.ScheduleEvent(EVENT_ATTENUATION, urand(10000, 20000));
                            break;
                        }
                        // Convert Platform
                        case (PLATFORM_ZORLOK_SE + 10):
                        {
                            events.ScheduleEvent(EVENT_CONVERT, urand (10000, 20000));
                            break;
                        }
                        // Phase 2 - Center of the room
                        case (PHASE_ZORLOK2 + 10):
                        {
                            Talk(TALK_EVENT_PHASE_2);
                            // Let's choose a random technic to use, as Zor'lok can use any of the ones he had on platforms
                            uint8 eventChoice = ChooseAction();
                            events.ScheduleEvent(eventChoice, urand(25000, 35000));
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                UpdateVictim();
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    // All-time events
                    case EVENT_INHALE:
                    {
                        // Can't inhale if already casting
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            events.RescheduleEvent(EVENT_INHALE, 25000);
                        else
                        {
                            // Exhale is triggered when Zor'lok has 3-4 stacks of inhale
                            AuraPtr inhale = me->GetAura(SPELL_INHALE);
                            if (!inhale || inhale->GetStackAmount() < 3)
                            {
                                Talk(TALK_INHALE);
                                me->MonsterTextEmote("Imperial Vizier Zor'lok |cFFFF0000|Hspell:122852|h[Inhale]|h|r a big air breath!", 0, true);
                                me->CastSpell(me, SPELL_INHALE, false);
                            }
                            else
                            {
                                if ((inhale->GetStackAmount() == 3 && urand(0, 1)) || inhale->GetStackAmount() >= 4)
                                    events.ScheduleEvent(EVENT_EXHALE, 1000);
                            }
                            events.ScheduleEvent(EVENT_INHALE, urand(25000, 50000));
                        }
                        break;
                    }
                    case EVENT_EXHALE:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            events.RescheduleEvent(EVENT_EXHALE, 5000);
                        else
                        {
                            // Retrieving all players in range
                            std::list<Player*> playerList;
                            GetPlayerListInGrid(playerList, me, 60.0f);

                            // No player ? Nothing to do...
                            if (playerList.empty())
                                return;

                            // Picking a random victim
                            Player* target;
                            bool pickingTarget = true;

                            while (pickingTarget)
                                for (auto player : playerList)
                                    if (urand(0, 1))
                                    {
                                        target = player;
                                        pickingTarget = false;
                                        break;
                                    }

                            Talk(TALK_EXHALE);
                            me->CastSpell(target, SPELL_EXHALE, true);
                        }
                        break;
                    }
                    case EVENT_SONG_EMPRESS:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING) || me->isAttackingPlayer())
                        {
                            events.RescheduleEvent(EVENT_SONG_EMPRESS, 1000);
                            return;
                        }

                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 7.0f);

                        if (playerList.empty() && me->GetReactState() == REACT_AGGRESSIVE)
                        {
                            me->CastSpell(me, SPELL_SONG_OF_THE_EMPRESS, true);
                            me->RemoveAurasDueToSpell(SPELL_SONG_OF_THE_EMPRESS);
                        }

                        else if (!me->getVictim())
                        {
                            for (auto target : playerList)
                            {
                                AttackStart(target);
                                me->SetInCombatWith(target);
                                break;
                            }
                        }
                        else if (Unit* target = me->getVictim())
                        {
                            AttackStart(target);
                            me->SetInCombatWith(target);
                        };

                        events.ScheduleEvent(EVENT_SONG_EMPRESS, 1000);
                        break;
                    }
                    // Attenuation platform
                    case EVENT_ATTENUATION:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.RescheduleEvent(EVENT_ATTENUATION, 10000);
                            return;
                        }
                        me->CastSpell(me, SPELL_ATTENUATION, true);
                        if (Unit* target = me->getVictim())
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED|UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_STUNNED);
                        me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                        me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_ALLOW_ENEMY_INTERACT);

                        uint32 action = (phase == PHASE_ZORLOK1 ? EVENT_ATTENUATION : ChooseAction());
                        events.ScheduleEvent(action, 40000);

                        break;
                    }
                    case EVENT_SUMMON_RINGS1:
                    {
                        DoAction(ACTION_SONIC_QUAD1);
                        break;
                    }
                    case EVENT_SUMMON_RINGS2:
                    {
                        DoAction(ACTION_SONIC_QUAD2);
                        break;
                    }
                    case EVENT_SUMMON_RINGS:
                    {
                        DoAction(ACTION_SONIC_CARD);
                        break;
                    }
                    // Convert platform
                    case EVENT_CONVERT:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.RescheduleEvent(EVENT_CONVERT, 10000);
                            return;
                        }

                        me->MonsterTextEmote("Imperial Vizier Zor'lok is using is voice to |cFFFF0000|Hspell:122740|h[Convert]|h|r members of the raid and to call them by his side !", 0, true);

                        // Creating target list
                        uint8 victimCount = Is25ManRaid() ? 5 : 2;
                        std::list<Player*> playerList;
                        std::list<Player*> targetList;

                        GetPlayerListInGrid(playerList, me, 60.0f);

                        if (playerList.size() <= victimCount)
                            targetList = playerList;
                        else
                        {
                            while (victimCount)
                            {
                                for (auto player : playerList)
                                {
                                    if (urand(0, 1))
                                    {
                                        targetList.push_back(player);
                                        if (!--victimCount)
                                            break;
                                    }
                                }
                            }
                        }

                        Talk(TALK_CONVERT);
                        for (auto target : targetList)
                            me->CastSpell(target, SPELL_CONVERT, true);

                        uint32 action = (phase == PHASE_ZORLOK1 ? EVENT_CONVERT : ChooseAction());
                        events.ScheduleEvent(action, 40000);

                        break;
                    }
                    // Force and Verve platform
                    case EVENT_FORCE_AND_VERVE:
                    {
                        if (me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            events.RescheduleEvent(EVENT_FORCE_AND_VERVE, 10000);
                            return;
                        }
                        // Creating Noise Cancelling zones
                        for (int i = 0; i < 3; ++i)
                            me->CastSpell(me, SPELL_MISSILE_NOISE_CANC, false);
                        me->AddUnitState(UNIT_STATE_CASTING);
                        events.ScheduleEvent(EVENT_CAST_FANDV, 4000);
                        break;
                    }
                    case EVENT_CAST_FANDV:
                    {
                        me->SetReactState(REACT_PASSIVE);
                        me->MonsterTextEmote("Imperial Vizier Zor'lok shouts with |cFFFF0000|Hspell:122713|h[Force et brio]|h|r!", 0, true);
                        me->CastSpell(me, SPELL_FORCE_AND_VERVE, true);
                        uint32 action = (phase == PHASE_ZORLOK1 ? EVENT_FORCE_AND_VERVE : ChooseAction());
                        events.ScheduleEvent(action, 40000);
                        break;
                    }
                    default:
                        break;
                }
                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_zorlokAI(creature);
        }
};

// Sonic Ring - 62689, 62694, 62696, 62716, 62717, 62718, 62719, 62726, 62727, 62743, 62744, 62746
class mob_sonic_ring : public CreatureScript
{
    public:
        mob_sonic_ring() : CreatureScript("mob_sonic_ring") { }

        struct mob_sonic_ringAI : public ScriptedAI
        {
            mob_sonic_ringAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            float rateOri;
            float orientation;
            EventMap events;

            void Reset()
            {
                me->SetDisplayId(DISPLAYID_INVISIBLE);
                me->CastSpell(me, SPELL_SONIC_RING_VISUAL, false);
                me->SetReactState(REACT_PASSIVE);

                events.ScheduleEvent(EVENT_SONIC_CHECK_POSITION, 10000);

                if (Creature* zorlok = pInstance->instance->GetCreature(pInstance->GetData64(NPC_ZORLOK)))
                {
                    float zorOri = zorlok->GetOrientation();
                    // How to move : depends on the entry !
                    switch (me->GetEntry())
                    {
                        case NPC_SONIC_RING_FORWARD:
                            orientation = zorOri;
                            break;
                        case NPC_SONIC_RING_30LEFT:
                            orientation = zorOri + (M_PI / 6);
                            break;
                        case NPC_SONIC_RING_60LEFT:
                            orientation = zorOri + (M_PI / 3);
                            break;
                        case NPC_SONIC_RING_90LEFT:
                            orientation = zorOri + (M_PI / 2);
                            break;
                        case NPC_SONIC_RING_120LEFT:
                            orientation = zorOri + (2 * M_PI / 3);
                            break;
                        case NPC_SONIC_RING_150LEFT:
                            orientation = zorOri + (5 * M_PI / 6);
                            break;
                        case NPC_SONIC_RING_BACK:
                            orientation = zorOri + M_PI;
                            break;
                        case NPC_SONIC_RING_150RIGHT:
                            orientation = zorOri + (7 * M_PI / 6);
                            break;
                        case NPC_SONIC_RING_120RIGHT:
                            orientation = zorOri + (4 * M_PI / 3);
                            break;
                        case NPC_SONIC_RING_90RIGHT:
                            orientation = zorOri + (3 * M_PI / 2);
                            break;
                        case NPC_SONIC_RING_60RIGHT:
                            orientation = zorOri + (5 * M_PI / 3);
                            break;
                        case NPC_SONIC_RING_30RIGHT:
                            orientation = zorOri + (11 * M_PI / 6);
                            break;
                        default:
                            break;
                    }
                    // If needed, reset orientation into valid range [0, M_PI * 2]
                    orientation = (orientation > (M_PI * 2)) ? (orientation - (M_PI * 2)) : (orientation < 0.0f ? (orientation + (M_PI * 2)) : orientation);
                    me->SetFacingTo(orientation);

                    // Go to virtual destination
                    Position reachPoint = GetTargetPoint(orientation, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), (me->GetPositionZ() > 408.0f ? 1 : 2));
                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(1, reachPoint);
                }
                else
                    me->DespawnOrUnsummon();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (id == 1)
                    me->DespawnOrUnsummon();
            }

            Position GetTargetPoint(float orientation, float posX, float posY, float posZ, uint8 phase)
            {
                /*
                 * The main idea is : a circle has 4 quarters; the principle is to define a point at the limit of the area the sonic ring can move,
                 * and use this point as a destination for MovePoint(). To calculate this point, we use the orientation to get a ratio between X and Y:
                 * if orientation is 0, we make 100% on x-axis, if orientation is pi/2, we make 100% on y-axis, and if orientation is pi/4, we make
                 * 50% on x-axis and 50% on y-axis.
                 *
                 * The range orientation from 0 to pi/2 represents a quarter circle where x and y will be both positives, and we use this quarter circle
                 * to determine general ratio between x and y. Then, we just have to "rotate" to apply this to the right orientation. According to this
                 * initial orientation, we may need to switch x and y ratio (when turned on left or right, moving forward is moving on y-axis, and not
                 * on x-axis, for instance), and/or to apply negatives values (if orientation is pi, we're moving backwards, so the x-value decreases,
                 * while if orientation is 0.0, we're moving forwards, and so, the x-value increases, but we're still on the same axis).
                 */
                
                // Retrieving absolute orientation
                float absOri = orientation;
                uint8 turn = 0;
                while (absOri > (M_PI / 2))
                {
                    absOri -= (M_PI / 2);
                    turn = 1 - turn;
                }

                // Looking for ratio between X and Y
                float percentX = ((M_PI / 2) - absOri) / (M_PI / 2);
                float percentY = 1.0f - percentX;

                // Applying negatives directions according to orientation
                if (orientation > (M_PI / 2))
                {
                    if (orientation > M_PI)
                        percentY = -percentY;
                    
                    if (orientation > (M_PI / 2) && orientation < (1.5f * M_PI))
                        percentX = -percentX;
                }

                // if turned, we need to switch X & Y
                if (turn)
                {
                    float tmpVal = percentX;
                    percentX = percentY;
                    percentY = tmpVal;
                }

                // Calculating reaching point
                float pointX = posX;
                float pointY = posY;

                if (phase == 1)
                {
                    while (((pointX + percentX) >= rangeAttenuation1[0][0]) 
                        && ((pointX + percentX) <= rangeAttenuation1[0][1])
                        && ((pointY + percentY) >= rangeAttenuation1[1][0])
                        && ((pointY + percentY) <= rangeAttenuation1[1][1]))
                    {
                        pointX += percentX;
                        pointY += percentY;
                    }
                }
                else
                {
                    while (((pointX + percentX) >= rangeAttenuation2[0][0])
                        && ((pointX + percentX) <= rangeAttenuation2[0][1])
                        && ((pointY + percentY) >= rangeAttenuation2[1][0])
                        && ((pointY + percentY) <= rangeAttenuation2[1][1]))
                    {
                        pointX += percentX;
                        pointY += percentY;
                    }
                }

                Position reachPoint = {pointX, pointY, posZ, orientation};
                return reachPoint;
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SONIC_CHECK_POSITION:
                    {
                        me->RemoveAura(SPELL_SONIC_RING_VISUAL);
                        me->DespawnOrUnsummon(1000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sonic_ringAI(creature);
        }
};

// Inhale - 122852
class spell_inhale : public SpellScriptLoader
{
    public:
        spell_inhale() : SpellScriptLoader("spell_inhale") { }

        class spell_inhale_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_inhale_SpellScript);

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, GetSpellInfo()->Effects[effIndex].BasePoints, false);
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_inhale_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        class spell_inhale_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_inhale_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_INHALE, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_inhale_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_inhale_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_inhale_AuraScript();
        }
};

// Exhale - 122761
class spell_exhale : public SpellScriptLoader
{
    public:
        spell_exhale() : SpellScriptLoader("spell_exhale") { }

        class spell_exhale_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_exhale_SpellScript);

            void RemoveAuras()
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveAurasDueToSpell(SPELL_INHALE);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_exhale_SpellScript::RemoveAuras);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_exhale_SpellScript();
        }
};

// Attenuation - 122440
class spell_attenuation : public SpellScriptLoader
{
    public:
        spell_attenuation() : SpellScriptLoader("spell_attenuation") { }

        class spell_attenuation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_attenuation_SpellScript);

            void Apply()
            {
                if (Creature* zorlok = GetCaster()->ToCreature())
                    zorlok->AI()->DoAction(ACTION_SONIC_CARD);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_attenuation_SpellScript::Apply);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_attenuation_SpellScript();
        }
};

// Noise cancelling - 122706
class spell_noise_cancelling : public SpellScriptLoader
{
    public:
        spell_noise_cancelling() : SpellScriptLoader("spell_noise_cancelling") { }

        class spell_noise_cancelling_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_noise_cancelling_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->AddAura(SPELL_NOISE_CANCELLING, target);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_noise_cancelling_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectApply += AuraEffectApplyFn(spell_noise_cancelling_AuraScript::Apply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_noise_cancelling_AuraScript();
        }
};

// Force and Verve (Aura) - 122718
class spell_force_verve : public SpellScriptLoader
{
    public:
        spell_force_verve() : SpellScriptLoader("spell_force_verve") { }

        class spell_force_verve_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_force_verve_SpellScript);

            void ApplyEffect()
            {
                if (Player* target = GetHitPlayer())
                    if (target->HasAura(SPELL_NOISE_CANCELLING))
                        SetHitDamage(GetHitDamage() * 0.25);
            }

            void SetReact()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                    caster->SetReactState(REACT_AGGRESSIVE);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_force_verve_SpellScript::ApplyEffect);
                AfterCast += SpellCastFn(spell_force_verve_SpellScript::SetReact);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_force_verve_SpellScript();
        }
};

// Sonic Ring (Aura) - 122336
class spell_sonic_ring : public SpellScriptLoader
{
    public:
        spell_sonic_ring() : SpellScriptLoader("spell_sonic_ring") { }

        class spell_sonic_ring_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sonic_ring_AuraScript);

            void ApplyAura(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, caster, 3.0f);

                    for (auto player : playerList)
                        player->AddAura(SPELL_SONIC_RING_AURA, player);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sonic_ring_AuraScript::ApplyAura,   EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_sonic_ring_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sonic_ring_SpellScript);

            void Effect()
            {
                if (Player* target = GetHitPlayer())
                    if (target->HasAura(SPELL_NOISE_CANCELLING))
                        SetHitDamage(GetHitDamage() * 0.4);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sonic_ring_SpellScript::Effect);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sonic_ring_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sonic_ring_SpellScript();
        }
};

// Pheromones of Zeal - 124018
class spell_pheromones_of_zeal : public SpellScriptLoader
{
    public:
        spell_pheromones_of_zeal() : SpellScriptLoader("spell_pheromones_of_zeal") { }
        
        class spell_pheromones_of_zeal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pheromones_of_zeal_AuraScript);

            void Apply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->AddAura(SPELL_INHALE_PHEROMONES, caster);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_pheromones_of_zeal_AuraScript::Apply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_pheromones_of_zeal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pheromones_of_zeal_SpellScript);

            void Effect()
            {
                if (Creature* caster = GetCaster()->ToCreature())
                    caster->AI()->DoAction(ACTION_INHALE_PHEROMONES);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pheromones_of_zeal_SpellScript::Effect);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pheromones_of_zeal_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_pheromones_of_zeal_SpellScript();
        }

};

void AddSC_boss_zorlok()
{
    new boss_zorlok();
    new mob_sonic_ring();
    new spell_inhale();
    new spell_exhale();
    new spell_attenuation();
    new spell_noise_cancelling();
    new spell_force_verve();
    new spell_sonic_ring();
    new spell_pheromones_of_zeal();
}